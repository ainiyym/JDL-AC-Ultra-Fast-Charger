//******************************************************************************
//* File Name: Cloud_Protocol_Sg_Login.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module log in source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Login.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	Cloud_Protocol_Mqtt_SignMethod method;
	const char *method_str;
} cloud_protocol_sg_SignMethod_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
static cloud_protocol_sg_SignMethod_t cloud_protocol_sg_sign_method_table[] =
{
	{CLOUD_PROTOCOL_MQTT_SIGN_HMACMD5, "hmacmd5"},
	{CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1, "hmacsha1"}
};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief get sign method string from enum
 * @param method sign method enum
 * @return corresponding string, returns "hmacsha1" if not found
 */
static const char *cloud_protocol_sg_get_sign_method_str(Cloud_Protocol_Mqtt_SignMethod method)
{
	for (int i = 0; i < sizeof(cloud_protocol_sg_sign_method_table) / sizeof(cloud_protocol_sg_sign_method_table[0]); i++)
	{
		if (cloud_protocol_sg_sign_method_table[i].method == method)
		{
			return cloud_protocol_sg_sign_method_table[i].method_str;
		}
	}
	return "hmacsha1"; // default
}

/**
 * @brief Creat sign clientId
 * @param clientid_string output clientId string buffer
 * @param deviceName device name
 * @param method sign method
 * @param timestamp timestamp in milliseconds (uint64_t)
 * @return status code
 *
 * @format: baseClientId|securemode=2,signmethod=xxx,timestamp=xxx|
 */
int cloud_protocol_sg_sign_get_clientid(char *clientid_string, const char *deviceName,
					   Cloud_Protocol_Mqtt_SignMethod method, uint64_t timestamp)
{
	if (clientid_string == NULL || deviceName == NULL)
	{
		return STATE_USER_INPUT_INVALID;
	}

	// get sign method string
	const char *sign_method_str = cloud_protocol_sg_get_sign_method_str(method);

	// creat client id string
	int ret = snprintf(clientid_string, DEV_SIGN_CLIENT_ID_MAXLEN,
					   "%s|securemode=2,signmethod=%s,timestamp=%" PRIu64 "|",
					   deviceName, sign_method_str, timestamp);

	if (ret < 0 || ret >= DEV_SIGN_CLIENT_ID_MAXLEN)
	{
		return STATE_MQTT_SIGN_CLIENTID_BUF_SHORT;
	}

	return STATE_SUCCESS;
}


/**
 * @brief Generate MQTT username in the format: deviceName&productKey
 * @param username_string Output buffer for the generated username
 * @param deviceName Device name string
 * @param productKey Product key string
 * @return int Status code (0 for success, negative for errors)
 * 
 * Format: deviceName&productKey
 * Example: 971762846196581146388515&a1D0siVHgRU
 */
int cloud_protocol_sg_sign_get_username(char *username_string, const char *deviceName, const char *productKey)
{
	if (username_string == NULL || deviceName == NULL || productKey == NULL)
	{
		return STATE_USER_INPUT_INVALID;
	}

	// Calculate required buffer size: deviceName + '&' + productKey + null terminator
	size_t required_size = strlen(deviceName) + strlen(productKey) + 2;

	// Check if output buffer is large enough (assuming USERNAME_MAX_LENGTH is defined)
	if (required_size > DEV_SIGN_USERNAME_MAXLEN)
	{
		return STATE_MQTT_SIGN_USERNAME_BUF_SHORT;
	}

	// Generate username in format: deviceName&productKey
	int ret = snprintf(username_string, DEV_SIGN_USERNAME_MAXLEN,
					   "%s&%s", deviceName, productKey);

	if (ret < 0 || ret >= DEV_SIGN_USERNAME_MAXLEN)
	{
		return STATE_MQTT_SIGN_USERNAME_BUF_SHORT;
	}

	return STATE_SUCCESS;
}

/**
 * @brief Generate signature string using specified HMAC method
 * @param device_id Device ID
 * @param device_name Device name
 * @param product_key Product Key
 * @param device_secret Device secret
 * @param sign_method Signature method to use
 * @param timestamp Timestamp in milliseconds
 * @param sign_string Output buffer to store the generated signature string
 * @param sign_string_size Size of the output buffer
 * @return Status code
 */
int cloud_protocol_sg_generate_sign_string(const char *device_id, const char *device_name, const char *product_key,
							   const char *device_secret, Cloud_Protocol_Mqtt_SignMethod sign_method,
							   const uint64_t timestamp,char *sign_string, size_t sign_string_size)
{
	char signsource[DEV_SIGN_SOURCE_MAXLEN] = {0};
	uint16_t signsource_len = 0;
	uint8_t sign_hex[41] = {0}; // Maximum size for SHA1 (20 * 2) and MD5 (16 * 2)

	// Calculate required buffer size
	signsource_len = SIGN_FMT_LEN + strlen(device_id) + strlen(device_name) + strlen(product_key);
	if (signsource_len >= DEV_SIGN_SOURCE_MAXLEN)
	{
		return STATE_MQTT_SIGN_SOURCE_BUF_SHORT;
	}

	// Build signature source string
	memset(signsource, 0, DEV_SIGN_SOURCE_MAXLEN);
	memcpy(signsource, "clientId", strlen("clientId"));
	memcpy(signsource + strlen(signsource), device_id, strlen(device_id));
	memcpy(signsource + strlen(signsource), "deviceName", strlen("deviceName"));
	memcpy(signsource + strlen(signsource), device_name, strlen(device_name));
	memcpy(signsource + strlen(signsource), "productKey", strlen("productKey"));
	memcpy(signsource + strlen(signsource), product_key, strlen(product_key));
    memcpy(signsource + strlen(signsource), "timestamp", strlen("timestamp"));
    snprintf(signsource + strlen(signsource), DEV_SIGN_SOURCE_MAXLEN - strlen(signsource), "%" PRIu64, timestamp);

	CLOUD_INFO("Sign source string: %s\r\n", signsource);
	// Generate HMAC based on specified method
	int hash_len = 0;
	switch (sign_method)
	{
	case CLOUD_PROTOCOL_MQTT_SIGN_HMACMD5:
		utils_hmac_md5(signsource, strlen(signsource), (char *)sign_hex,
					   device_secret, strlen(device_secret));
		hash_len = MD5_DIGEST_SIZE * 2;
		break;

	case CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1:
		utils_hmac_sha1(signsource, strlen(signsource), (char *)sign_hex, device_secret, strlen(device_secret));
		hash_len = SHA1_DIGEST_SIZE * 2;
		break;

	default:
		return STATE_MQTT_ASYNC_STACK_NOT_SUPPORT;
	}

	// Convert hex to string
	Cloud_Protocol_Hex2Ascii(sign_hex, hash_len, sign_string, sign_string_size);

	return STATE_SUCCESS;
}

/**
 * @brief Generate MQTT connection signature information with custom timestamp
 * @param meta Device meta information
 * @param timestamp Custom timestamp (milliseconds)
 * @param signout Output structure for MQTT signature information
 * @return Status code
 */
int32_t Cloud_Protocol_Mqtt_Sign(iotx_dev_meta_info_t *meta, uint64_t timestamp, iotx_sign_mqtt_t *signout)
{
	if (meta == NULL || signout == NULL)
	{
		return STATE_USER_INPUT_INVALID;
	}

	// Validate required meta fields
	if (meta->product_key == NULL || meta->device_name == NULL || meta->device_secret == NULL)
	{
		return STATE_USER_INPUT_INVALID;
	}

	int32_t ret = STATE_SUCCESS;

	// Generate client ID with signature method and custom timestamp
	ret = cloud_protocol_sg_sign_get_clientid(signout->clientid, meta->device_name,
											  CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1, timestamp);
	if (ret != STATE_SUCCESS)
	{
		return ret;
	}

	// Generate username
	ret = cloud_protocol_sg_sign_get_username(signout->username, meta->device_name, meta->product_key);
	if (ret != STATE_SUCCESS)
	{
		return ret;
	}

	// Generate password signature
	char sign_string[DEV_SIGN_PASSWORD_MAXLEN] = {0};
	ret = cloud_protocol_sg_generate_sign_string(meta->device_name, meta->device_name,
												 meta->product_key, meta->device_secret, CLOUD_PROTOCOL_MQTT_SIGN_HMACSHA1, timestamp, sign_string, DEV_SIGN_PASSWORD_MAXLEN);
	if (ret != STATE_SUCCESS)
	{
		return ret;
	}

	strncpy(signout->password, sign_string, sizeof(signout->password) - 1);
	signout->password[sizeof(signout->password) - 1] = '\0';

	return STATE_SUCCESS;
}
/* EOL */
