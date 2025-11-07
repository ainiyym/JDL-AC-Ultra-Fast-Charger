//******************************************************************************
//* File Name: Cloud_Protocol_GagaM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud_Protocol_GagaM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_GagaM.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
    CLOUD_PROTOCOL_GAGA_RUNNING_STEP_INIT = 0,
    CLOUD_PROTOCOL_GAGA_RUNNING_STEP_PROCESS_DATA,
    CLOUD_PROTOCOL_GAGA_RUNNING_STEP_MAX
} cloud_protocol_gaga_running_step_e;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef void (*cloud_protocol_restart_device_func_t)(void);
typedef Cloud_Protocol_Send_Status_T (*cloud_protocol_send_func_t)(uint8_t frame_type, uint8_t *buffer, uint16_t buffer_size);

typedef struct
{
	bool enabled;						// Whether to enable protocol processing
	uint16_t reset_device_delay_time_s; // Device restart delay time (in seconds)
	uint16_t message_buffer_max_length; // Message buffer maximum length
	cloud_protocol_send_func_t cloud_gaga_send_func; // Function pointer for sending messages
	cloud_protocol_restart_device_func_t cloud_gaga_restart_device_func; // Function pointer for restart device
} cloud_protocol_gaga_config_t;

typedef struct
{
	cloud_protocol_gaga_config_t config;
	uint8_t running_step; // step of Gaga protocol processing
	uint16_t timer;		  // timer for Gaga protocol
	bool is_initialized;  // Initialization flag
} cloud_protocol_gaga_context_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static const cloud_protocol_gaga_config_t cloud_gaga_protocol_default_config = 
{
	.enabled = CLOUD_GAGA_PROTOCOL_ENABLE_STATUS,
	.reset_device_delay_time_s = CLOUD_GAGA_PROTOCOL_RESET_DEVICE_DELAY_TIME_S,
	.message_buffer_max_length = CLOUD_PROTOCOL_GAGA_DATA_BUFFER_MAX_LENGTH,
	.cloud_gaga_send_func = CLOUD_GAGA_PROTOCOL_CALL_SEND_FUNC,
	.cloud_gaga_restart_device_func = CLOUD_GAGA_PROTOCOL_RESTART_DEVICE
};

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static cloud_protocol_gaga_context_t cloud_gaga_protocol_context = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Cloud_GagaProtocol_Init(void)
{
	if (cloud_gaga_protocol_context.is_initialized)
	{
		return ;
	}

	cloud_gaga_protocol_context.config = cloud_gaga_protocol_default_config;

	// Initialize the context
	cloud_gaga_protocol_context.running_step = CLOUD_PROTOCOL_GAGA_RUNNING_STEP_INIT;
	cloud_gaga_protocol_context.timer = 0;
	cloud_gaga_protocol_context.is_initialized = true;
}

void Cloud_GagaProtocol_DeviceResetChecking(void)
{
	if (cloud_gaga_protocol_context.config.enabled)
	{
		cloud_gaga_protocol_context.running_step = CLOUD_PROTOCOL_GAGA_RUNNING_STEP_INIT;
		Cloud_Protocol_ResetLogInStatus();
	}
}

static void Cloud_GagaProtocol_ResetLogIn(void)
{
    cloud_gaga_protocol_context.running_step = CLOUD_PROTOCOL_GAGA_RUNNING_STEP_INIT;
    Cloud_Protocol_ResetLogInStatus();
}

static void Cloud_GagaProtocol_NormalHeartbeatProcess(void)
{
	if (Cloud_Protocol_Get_HeartbeatIsNormal())
	{
		// Process non-heartbeat messages
	}
	else
	{
		CLOUD_DEBUG("Heartbeat abnormal, resetting login status\r\n");
		Cloud_GagaProtocol_ResetLogIn();
	}
}

static void Cloud_GagaProtocol_NormalOperationProcess(void)
{
    // Placeholder for normal operation tasks
    Cloud_Protocol_Heartbeat_Handler();
    // Process not heatbeat messages
    Cloud_GagaProtocol_NormalHeartbeatProcess();
}

void Cloud_GagaProtocol_MainProcess(void)
{
	if (!cloud_gaga_protocol_context.config.enabled || !cloud_gaga_protocol_context.is_initialized)
	{
		return;
	}

	uint8_t msg_buffer[cloud_gaga_protocol_context.config.message_buffer_max_length];
	memset(msg_buffer, 0, sizeof(msg_buffer));
	cloud_protocol_send_func_t cloud_gaga_send_func = cloud_gaga_protocol_context.config.cloud_gaga_send_func;
	cloud_protocol_restart_device_func_t cloud_gaga_restart_device_func = cloud_gaga_protocol_context.config.cloud_gaga_restart_device_func;

	switch (cloud_gaga_protocol_context.running_step)
	{
		case CLOUD_PROTOCOL_GAGA_RUNNING_STEP_INIT:
			// Initialize callback functions
			Cloud_Protocol_CallbackFunc_Init();
			// send login authentication frame
			if (cloud_gaga_send_func != NULL)
			{
				cloud_gaga_send_func(0x01, msg_buffer, cloud_gaga_protocol_context.config.message_buffer_max_length);
			}
			cloud_gaga_protocol_context.running_step = CLOUD_PROTOCOL_GAGA_RUNNING_STEP_PROCESS_DATA;
			break;

		case CLOUD_PROTOCOL_GAGA_RUNNING_STEP_PROCESS_DATA:
			switch (Cloud_Protocol_GetLogInStatus())
			{
				case CLOUD_PROTOCOL_AUTHENTICATION_SUCCESS:
					// normal operation
					Cloud_GagaProtocol_NormalOperationProcess();
					cloud_gaga_protocol_context.timer = 0;
					break;

				case CLOUD_PROTOCOL_AUTHENTICATION_FAILED:
					// Authentication failed, re-initiate authentication
					Cloud_GagaProtocol_ResetLogIn();
					break;

				case CLOUD_PROTOCOL_AUTHENTICATION_INIT:
					// 30 seconds without authentication response, reset device
					if (cloud_gaga_protocol_context.timer < cloud_gaga_protocol_context.config.reset_device_delay_time_s)
					{
						cloud_gaga_protocol_context.timer++;
					}
					else
					{
						cloud_gaga_protocol_context.timer = 0;
						CLOUD_ERROR("Authentication timeout, restarting device\r\n");
						if (cloud_gaga_restart_device_func != NULL)
						{
							cloud_gaga_restart_device_func();
						}
					}
					break;

				default:
				break;
			}
			break;

		default:
			break;
	}
}
/* EOL */
