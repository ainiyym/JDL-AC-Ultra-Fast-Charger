//******************************************************************************
//* File Name: Cloud_Protocol_Sg_SynchronizeNetTime.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module net time synchronization source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_SynchronizeNetTime.h"
#include "Cloud_Protocol_Mqtt.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
/**
 * @brief Network time synchronization state enumeration
 */
typedef enum
{
	CLOUD_PROTOCOL_SYSN_TIME_STATE_IDLE = 0,		 // Idle state, not started
	CLOUD_PROTOCOL_SYSN_TIME_STATE_REQUEST_SENT,	 // Time sync request sent
	CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_SUCCESS,	 // Time synchronization successful
	CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED,		 // Time synchronization failed
	CLOUD_PROTOCOL_SYSN_TIME_STATE_INVALID			 // Invalid state
} cloud_protocol_sysn_time_state_e;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/**
 * @brief NTP time synchronization request structure
 */
typedef struct
{
	uint64_t device_send_time; // T1: Device send time (milliseconds)
} cloud_protocol_sysnchronize_ntp_request_t;

/**
 * @brief NTP time synchronization response structure
 */
typedef struct
{
	uint64_t device_send_time; // T1: Original device send time
	uint64_t server_recv_time; // T2: Server receive time
	uint64_t server_send_time; // T3: Server send time
} cloud_protocol_sysnchronize_ntp_response_t;

/**
 * @brief NTP time calculation structure
 */
typedef struct
{
	uint64_t t1;					 // Device send time
	uint64_t t2;					 // Server receive time
	uint64_t t3;					 // Server send time
	uint64_t t4;					 // Device receive time
	int64_t time_offset;			 // Calculated time offset (ms)
	uint64_t round_trip_delay;		 // Round trip delay (ms)
	uint64_t calculated_server_time; // Calculated current server time
} cloud_protocol_sysnchronize_ntp_calc_t;

/**
 * @brief Network time synchronization configuration structure
 */
typedef struct
{
	uint8_t max_retry_count;												   // Maximum retry attempts
	uint32_t sync_interval_ms;												   // Sync interval in milliseconds
	uint32_t retry_backoff_ms;												   // Retry backoff time in milliseconds
	bool auto_sync_enabled;													   // Auto sync enabled flag
	char ntp_request_topic[CLOUD_PROTOCOL_PUB_TOPIC_MAX_LENGTH];  // NTP request topic
	char ntp_response_topic[CLOUD_PROTOCOL_SUB_TOPIC_MAX_LENGTH]; // NTP response topic
	uint16_t max_clock_adjustment;											   // Maximum clock adjustment (ms)
	bool enable_clock_adjustment;											   // Enable automatic clock adjustment
} cloud_protocol_sysnchronize_net_time_config_t;

/**
 * @brief Network time synchronization control structure
 */
typedef struct
{
	bool net_is_connected;							  // network whether is connected
	cloud_protocol_sysn_time_state_e state;			  // Current synchronization state
	uint64_t last_sync_time;						  // Last synchronization timestamp
	uint8_t current_retry_count;					  // Current retry count
	uint64_t next_sync_time;						  // Next scheduled sync time
	int64_t time_offset_ms;							  // Calculated time offset in milliseconds
	uint64_t current_ntp_t1;						  // Current NTP T1 (device send time)
	uint64_t current_ntp_t4;						  // Current NTP T4 (device receive time)
	bool sync_required;								  // Sync required flag
	uint8_t sync_quality;							  // Sync quality indicator (0-100)
	cloud_protocol_sysnchronize_ntp_calc_t last_calc; // Last NTP calculation data
} cloud_protocol_sysnchronize_net_time_ctrl_t;

/**
 * @brief Complete network time synchronization context structure
 */
typedef struct
{
	cloud_protocol_sysnchronize_net_time_config_t config;					 // Synchronization configuration
	cloud_protocol_sysnchronize_net_time_ctrl_t ctrl;						 // Synchronization control state
	cloud_protocol_sync_net_time_complete_callback_t sync_complete_callback; // Sync complete callback
} cloud_protocol_sysnchronize_net_time_ctx_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_sysnchronize_net_time_ctx_t cloud_protocol_sysnchronize_net_time_ctx;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static int cloud_protocol_sysnchronize_ntp_calculate(cloud_protocol_sysnchronize_ntp_calc_t *calc);
static int cloud_protocol_sysnchronize_ntp_build_request_topic(char *topic_buffer, size_t buffer_size);
static int cloud_protocol_sysnchronize_ntp_build_response_topic(char *topic_buffer, size_t buffer_size);
static int cloud_protocol_sysnchronize_ntp_build_request_payload(uint64_t device_send_time, char *payload_buffer, size_t buffer_size);
static int cloud_protocol_sysnchronize_ntp_parse_time_sync_response(const char *payload, cloud_protocol_sysnchronize_ntp_response_t *response);
static int cloud_protocol_sysnchronize_net_time_start(cloud_protocol_sysnchronize_net_time_ctx_t *ctx);
static void cloud_protocol_sysnchronize_net_time_set_state(cloud_protocol_sysnchronize_net_time_ctx_t *ctx, cloud_protocol_sysn_time_state_e new_state);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Calculate NTP time offset and round trip delay
 * @param calc NTP calculation structure with T1, T2, T3, T4
 * @return 0 on success, -1 on invalid data
 */
static int cloud_protocol_sysnchronize_ntp_calculate(cloud_protocol_sysnchronize_ntp_calc_t *calc)
{
	if (!calc)
	{
		return -1;
	}

	// Validate timestamps
	if (calc->t1 == 0 || calc->t2 == 0 || calc->t3 == 0 || calc->t4 == 0)
	{
		return -1;
	}

	if (calc->t4 <= calc->t1)
	{
		return -1; // Invalid time sequence
	}

	// Calculate round trip delay: (T4 - T1) - (T3 - T2)
	calc->round_trip_delay = (calc->t4 - calc->t1) - (calc->t3 - calc->t2);

	// Calculate time offset: [(T2 - T1) + (T3 - T4)] / 2
	calc->time_offset = ((int64_t)(calc->t2 - calc->t1) + (int64_t)(calc->t3 - calc->t4)) / 2;

	// Calculate current server time: T4 + time_offset
	calc->calculated_server_time = calc->t4 + calc->time_offset;

	CLOUD_INFO("NTP Calculation:\n");
	CLOUD_INFO("  T1 (Device Send): %llu\n", calc->t1);
	CLOUD_INFO("  T2 (Server Recv): %llu\n", calc->t2);
	CLOUD_INFO("  T3 (Server Send): %llu\n", calc->t3);
	CLOUD_INFO("  T4 (Device Recv): %llu\n", calc->t4);
	CLOUD_INFO("  Round Trip Delay: %llu ms\n", calc->round_trip_delay);
	CLOUD_INFO("  Time Offset: %lld ms\n", calc->time_offset);
	CLOUD_INFO("  Calculated Server Time(ms): %llu\n", calc->calculated_server_time);

	return 0;
}

/**
 * @brief Build NTP request topic
 * @param topic_buffer Output topic buffer
 * @param buffer_size Buffer size
 * @return 0 on success, -1 on error
 */
static int cloud_protocol_sysnchronize_ntp_build_request_topic(char *topic_buffer, size_t buffer_size)
{
	if (!topic_buffer)
	{
		return -1;
	}

	const cloud_protocol_mqtt_topic_config_t* ntp_topic = Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_SYNC_CLOUD_TIME);

	Cloud_Protocol_Sg_Build_Topic(ntp_topic->publish_topic, NULL, topic_buffer, buffer_size);

	return 0;
}

/**
 * @brief Build NTP response topic
 * @param topic_buffer Output topic buffer
 * @param buffer_size Buffer size
 * @return 0 on success, -1 on error
 */
static int cloud_protocol_sysnchronize_ntp_build_response_topic(char *topic_buffer, size_t buffer_size)
{
	if (!topic_buffer)
	{
		return -1;
	}

	const cloud_protocol_mqtt_topic_config_t* ntp_topic = Cloud_Protocol_Mqtt_GetActiveTopicConfigByEnum(CLOUD_PROTOCOL_MQTT_ACTIVE_TOPIC_SYNC_CLOUD_TIME);
	Cloud_Protocol_Sg_Build_Topic(ntp_topic->subscribe_topic, NULL, topic_buffer, buffer_size);

	return 0;
}

/**
 * @brief Build NTP request payload
 * @param device_send_time T1: Device send time
 * @param payload_buffer Output payload buffer
 * @param buffer_size Buffer size
 * @return 0 on success, -1 on error
 */
static int cloud_protocol_sysnchronize_ntp_build_request_payload(uint64_t device_send_time, char *payload_buffer, size_t buffer_size)
{
	if (!payload_buffer)
	{
		return -1;
	}

	// Format: { "deviceSendTime":"1571724098000" }
	int required_size = snprintf(NULL, 0, "{\"deviceSendTime\":\"%llu\"}", device_send_time);

	if (required_size < 0 || (size_t)required_size >= buffer_size)
	{
		return -1;
	}

	snprintf(payload_buffer, buffer_size, "{\"deviceSendTime\":\"%llu\"}", device_send_time);

	return 0;
}

/**
 * @brief Parse NTP response payload
 * @param payload Response payload string
 * @param response Output response structure
 * @return 0 on success, -1 on error
 */
static int cloud_protocol_sysnchronize_ntp_parse_time_sync_response(const char *payload, cloud_protocol_sysnchronize_ntp_response_t *response)
{
	if (!payload || !response)
	{
		return -1;
	}

	cJSON *root = cJSON_Parse(payload);
	if (!root)
	{
		return -1;
	}

	int ret = -1;

	cJSON *device_send_time = cJSON_GetObjectItem(root, "deviceSendTime");
	cJSON *server_send_time = cJSON_GetObjectItem(root, "serverSendTime");
	cJSON *server_recv_time = cJSON_GetObjectItem(root, "serverRecvTime");

	if (device_send_time && server_send_time && server_recv_time)
	{
		response->device_send_time = strtoull(device_send_time->valuestring, NULL, 10);
		response->server_send_time = strtoull(server_send_time->valuestring, NULL, 10);
		response->server_recv_time = strtoull(server_recv_time->valuestring, NULL, 10);
		ret = 0;

		CLOUD_DEBUG("Time sync response: device=%llu, server_send=%llu, server_recv=%llu\n",
					response->device_send_time, response->server_send_time, response->server_recv_time);
	}

	cJSON_Delete(root);
	return ret;
}

/**
 * @brief Initialize NTP time synchronization context
 * @param product_key Device product key
 * @param device_name Device name
 * @return 0 on success, -1 on error
 */
int cloud_protocol_sysnchronize_net_time_init(const char *product_key, const char *device_name, cloud_protocol_sync_net_time_complete_callback_t sync_callback)
{
	if (!product_key || !device_name)
	{
		return -1;
	}

	// Initialize configuration with default values
	memset(&cloud_protocol_sysnchronize_net_time_ctx, 0, sizeof(cloud_protocol_sysnchronize_net_time_ctx_t));
	cloud_protocol_sysnchronize_net_time_ctx_t *ctx = &cloud_protocol_sysnchronize_net_time_ctx;
	ctx->config.max_retry_count = CLOUD_PROTOCOL_SG_SYSN_NET_TIME_DEFAULT_RETRY_COUNT; // 3 attempts
	ctx->config.sync_interval_ms = CLOUD_PROTOCOL_SG_SYSN_NET_TIME_PERIOD_MS; // 1 day
	ctx->config.retry_backoff_ms = CLOUD_PROTOCOL_SG_SYSN_NET_TIME_RETRY_BACKOFF_TIME_MS;	// 30 seconds
	ctx->config.auto_sync_enabled = true;
	ctx->config.max_clock_adjustment = CLOUD_PROTOCOL_SG_SYSN_NET_TIME_MAX_CLOCK_ADJUSTMENT_MS; // Max 5 seconds adjustment
	ctx->config.enable_clock_adjustment = false;

	// Build NTP topics
	if (cloud_protocol_sysnchronize_ntp_build_request_topic(ctx->config.ntp_request_topic,
															sizeof(ctx->config.ntp_request_topic)) != 0)
	{
		return -1;
	}

	if (cloud_protocol_sysnchronize_ntp_build_response_topic(ctx->config.ntp_response_topic,
															 sizeof(ctx->config.ntp_response_topic)) != 0)
	{
		return -1;
	}

	// Initialize control state
	ctx->ctrl.state = CLOUD_PROTOCOL_SYSN_TIME_STATE_IDLE;
	ctx->ctrl.last_sync_time = 0;
	ctx->ctrl.current_retry_count = 0;
	ctx->ctrl.next_sync_time = 0;
	ctx->ctrl.time_offset_ms = 0;
	ctx->ctrl.current_ntp_t1 = 0;
	ctx->ctrl.current_ntp_t4 = 0;
	ctx->ctrl.sync_required = true;
	ctx->ctrl.sync_quality = 0;
	memset(&ctx->ctrl.last_calc, 0, sizeof(ctx->ctrl.last_calc));

	// Initialize callbacks to NULL
	ctx->sync_complete_callback = sync_callback;

	CLOUD_INFO("NTP Time Sync Initialized:\n");
	CLOUD_INFO("Request Topic: %s\n", ctx->config.ntp_request_topic);
	CLOUD_INFO("Response Topic: %s\n", ctx->config.ntp_response_topic);

	return 0;
}

/**
 * @brief Start NTP time synchronization
 * @param ctx Time synchronization context
 * @return 0 on success, -1 on error
 */
static int cloud_protocol_sysnchronize_net_time_start(cloud_protocol_sysnchronize_net_time_ctx_t *ctx)
{
	if (!ctx)
	{
		return -1;
	}

	if (ctx->ctrl.state != CLOUD_PROTOCOL_SYSN_TIME_STATE_IDLE)
	{
		return -1; // Already in progress
	}

	// Record T1: Device send time
	ctx->ctrl.current_ntp_t1 = CLOUD_GET_TIME_MS();

	// Build NTP request payload
	char payload[128];
	if (cloud_protocol_sysnchronize_ntp_build_request_payload(ctx->ctrl.current_ntp_t1, payload, sizeof(payload)) != 0)
	{
		return -1;
	}

	// Send NTP request
	Cloud_Protocol_Mqtt_AddPublishMessage(ctx->config.ntp_request_topic, payload, CLOUD_PROTOCOL_MQTT_NEED_ACK, ctx->config.ntp_response_topic);

	// Update state
	cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_REQUEST_SENT);

	CLOUD_INFO("NTP Request Sent:\n");
	CLOUD_INFO("Topic: %s\n", ctx->config.ntp_request_topic);
	CLOUD_INFO("Payload: %s\n", payload);
	// CLOUD_INFO("T1 (Device Send): %llu\n", ctx->ctrl.current_ntp_t1);

	return 0;
}

/**
 * @brief Handle NTP time synchronization response
 * @param payload Response payload
 * @param payload_len Payload length
 * @return 1 on success, 0 on error
 */
bool cloud_protocol_sysnchronize_net_time_handle_response(const char *payload, uint16_t payload_len)
{
	if (!payload)
	{
		return false;
	}

	cloud_protocol_sysnchronize_net_time_ctx_t *ctx = &cloud_protocol_sysnchronize_net_time_ctx;
	// Check if we're expecting a response
	if (ctx->ctrl.state != CLOUD_PROTOCOL_SYSN_TIME_STATE_REQUEST_SENT)
	{
		return false; // Not expecting response
	}

	// Record T4: Device receive time
	ctx->ctrl.current_ntp_t4 = CLOUD_GET_TIME_MS();

	// Parse NTP response
	cloud_protocol_sysnchronize_ntp_response_t ntp_response;
	if (cloud_protocol_sysnchronize_ntp_parse_time_sync_response(payload, &ntp_response) != 0)
	{
		CLOUD_INFO("Failed to parse NTP response\n");
		cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED);
		return false;
	}

	// Verify T1 matches our request
	if (ntp_response.device_send_time != ctx->ctrl.current_ntp_t1)
	{
		CLOUD_INFO("NTP response T1 mismatch: expected %llu, got %llu\n", ctx->ctrl.current_ntp_t1, ntp_response.device_send_time);
		cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED);
		return false;
	}

	// Perform NTP calculation
	cloud_protocol_sysnchronize_ntp_calc_t ntp_calc;
	ntp_calc.t1 = ntp_response.device_send_time;
	ntp_calc.t2 = ntp_response.server_recv_time;
	ntp_calc.t3 = ntp_response.server_send_time;
	ntp_calc.t4 = ctx->ctrl.current_ntp_t4;

	if (cloud_protocol_sysnchronize_ntp_calculate(&ntp_calc) != 0)
	{
		CLOUD_INFO("NTP calculation failed\n");
		cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED);
		return false;
	}

	// Validate round trip delay (too high indicates network issues)
	if (ntp_calc.round_trip_delay > 10000)
	{ // More than 10 seconds
		CLOUD_INFO("NTP round trip delay too high: %llu ms\n", ntp_calc.round_trip_delay);
		cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED);
		return false;
	}

	// Check if clock adjustment is within acceptable limits
	if (ctx->config.enable_clock_adjustment)
	{
		if (abs(ntp_calc.time_offset) > ctx->config.max_clock_adjustment)
		{
			CLOUD_INFO("NTP time offset too large: %lld ms (max: %d ms)\n",
				   ntp_calc.time_offset, ctx->config.max_clock_adjustment);
			// We still consider this successful but don't adjust the clock
			ctx->ctrl.time_offset_ms = ntp_calc.time_offset;
			CLOUD_INFO("Clock adjustment skipped due to large offset\n");
		}
		else
		{
			// Apply clock adjustment
			ctx->ctrl.time_offset_ms = ntp_calc.time_offset;
			// set_system_time(get_current_timestamp_ms() + ntp_calc.time_offset);
			CLOUD_INFO("Clock adjusted by %lld ms\n", ntp_calc.time_offset);
		}
	}
	else
	{
		// Just record the offset without adjusting the clock
		ctx->ctrl.time_offset_ms = ntp_calc.time_offset;
		CLOUD_INFO("Time offset recorded: %lld ms (clock adjustment disabled)\n", ntp_calc.time_offset);
	}

	// Save calculation data
	memcpy(&ctx->ctrl.last_calc, &ntp_calc, sizeof(ntp_calc));

	// Update state
	cloud_protocol_sysnchronize_net_time_set_state(ctx, CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_SUCCESS);

	// Call completion callback
	if (ctx->sync_complete_callback)
	{
		ctx->sync_complete_callback(true, ntp_calc.time_offset, ntp_calc.round_trip_delay, ntp_calc.calculated_server_time);
	}

	// CLOUD_INFO("NTP Time Sync Successful:\n");
	// CLOUD_INFO("Time Offset: %lld ms\n", ntp_calc.time_offset);
	// CLOUD_INFO("Round Trip Delay: %llu ms\n", ntp_calc.round_trip_delay);
	// CLOUD_INFO("Calculated Server Time: %llu\n", ntp_calc.calculated_server_time);

	return true;
}

/**
 * @brief Set time synchronization state
 * @param ctx Time synchronization context
 * @param new_state New state to set
 */
static void cloud_protocol_sysnchronize_net_time_set_state(cloud_protocol_sysnchronize_net_time_ctx_t *ctx, cloud_protocol_sysn_time_state_e new_state)
{
	if (!ctx || ctx->ctrl.state == new_state)
	{
		return;
	}

	cloud_protocol_sysn_time_state_e old_state = ctx->ctrl.state;
	ctx->ctrl.state = new_state;

	// Update state-specific timers and counters
	uint64_t current_time = CLOUD_GET_TIME_MS();

	switch (new_state)
	{
		case CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_SUCCESS:
			ctx->ctrl.last_sync_time = current_time;
			ctx->ctrl.next_sync_time = ctx->ctrl.last_sync_time + ctx->config.sync_interval_ms;
			ctx->ctrl.current_retry_count = 0;
			ctx->ctrl.sync_required = false;
			break;

		case CLOUD_PROTOCOL_SYSN_TIME_STATE_SYNC_FAILED:
			ctx->ctrl.current_retry_count++;
			if (ctx->ctrl.current_retry_count >= ctx->config.max_retry_count)
			{
				ctx->ctrl.next_sync_time = current_time + ctx->config.retry_backoff_ms * 2;
			}
			else
			{
				ctx->ctrl.next_sync_time = current_time + ctx->config.retry_backoff_ms;
			}
			break;

		default:
			break;
	}

	CLOUD_INFO("NTP Time Sync State: %d -> %d\n", old_state, new_state);
}

void Cloud_Protocol_Sg_SynchronizeNetTime_SetNetworkConnectStatus(bool connect)
{
	cloud_protocol_sysnchronize_net_time_ctx.ctrl.net_is_connected = connect;
}

void Cloud_Protocol_Sg_SynchronizeNetTime_Main(void)
{
	if (!cloud_protocol_sysnchronize_net_time_ctx.ctrl.net_is_connected)
	{
		return;
	}

	// Placeholder for network time synchronization logic
	if (cloud_protocol_sysnchronize_net_time_ctx.ctrl.sync_required)
	{
		// Start synchronization
		cloud_protocol_sysnchronize_net_time_start(&cloud_protocol_sysnchronize_net_time_ctx);
		return;
	}

	if (cloud_protocol_sysnchronize_net_time_ctx.ctrl.next_sync_time != 0 && cloud_protocol_sysnchronize_net_time_ctx.ctrl.next_sync_time <= CLOUD_GET_TIME_MS())
	{
		// Time to sync again
		cloud_protocol_sysnchronize_net_time_ctx.ctrl.sync_required = true;
		cloud_protocol_sysnchronize_net_time_ctx.ctrl.state = CLOUD_PROTOCOL_SYSN_TIME_STATE_IDLE;
		CLOUD_INFO("NTP Time Sync: Scheduled sync time reached(next_sync_time %lld, current timestamp:%lld), preparing to sync again.\n", cloud_protocol_sysnchronize_net_time_ctx.ctrl.next_sync_time, CLOUD_GET_TIME_MS());
	}
}
/* EOL */
