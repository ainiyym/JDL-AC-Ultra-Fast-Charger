//******************************************************************************
//* File Name: Cloud_Protocol_Sg_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: State Grid Charging Pile Cloud Platform Protocol module configuration source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Sg_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static uint32_t cloud_protocol_message_counter = 0;

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/**
 * @brief Build topic by replacing placeholders
 * @param topic_template Topic template with {productKey} and {deviceName} placeholders
 * @param identifier Identifier to replace in the topic
 * @param topic_buffer Output buffer for the topic
 * @param buffer_size Size of output buffer
 * @return 0 on success, -1 on error
 */
int Cloud_Protocol_Sg_Build_Topic(const char *topic_template, const char* identifier, char *topic_buffer, size_t buffer_size)
{
    if (!topic_template || !topic_buffer || buffer_size == 0)
    {
        return -1;
    }

    char temp_buffer[MAX_TOPIC_LENGTH];
    strncpy(temp_buffer, topic_template, sizeof(temp_buffer) - 1);
    temp_buffer[sizeof(temp_buffer) - 1] = '\0';

    // Replace {productKey}
    char *product_key_pos = strstr(temp_buffer, "{productKey}");
    if (product_key_pos)
    {
        size_t prefix_len = product_key_pos - temp_buffer;
        size_t suffix_len = strlen(product_key_pos + strlen("{productKey}"));

        if (prefix_len + strlen(CLOUDM_SG_PRODUCT_KEY) + suffix_len + 1 >= buffer_size)
        {
            return -1;
        }

        memmove(product_key_pos + strlen(CLOUDM_SG_PRODUCT_KEY),
                product_key_pos + strlen("{productKey}"),
                suffix_len + 1);
        memcpy(product_key_pos, CLOUDM_SG_PRODUCT_KEY, strlen(CLOUDM_SG_PRODUCT_KEY));
    }

    // Replace {deviceName}
    char *device_name_pos = strstr(temp_buffer, "{deviceName}");
    if (device_name_pos)
    {
        size_t prefix_len = device_name_pos - temp_buffer;
        size_t suffix_len = strlen(device_name_pos + strlen("{deviceName}"));

        if (prefix_len + strlen(CLOUDM_SG_DEVICE_NAME) + suffix_len + 1 >= buffer_size)
        {
            return -1;
        }

        memmove(device_name_pos + strlen(CLOUDM_SG_DEVICE_NAME),
                device_name_pos + strlen("{deviceName}"),
                suffix_len + 1);
        memcpy(device_name_pos, CLOUDM_SG_DEVICE_NAME, strlen(CLOUDM_SG_DEVICE_NAME));
    }

    // Replace {identifier},if any
    if (NULL != identifier)
    {
        char *identifier_pos = strstr(temp_buffer, "{identifier}");
        if (identifier_pos && identifier)
        {
            size_t prefix_len = identifier_pos - temp_buffer;
            size_t suffix_len = strlen(identifier_pos + strlen("{identifier}"));

            if (prefix_len + strlen(identifier) + suffix_len + 1 >= buffer_size)
            {
                return -1;
            }

            memmove(identifier_pos + strlen(identifier),
                    identifier_pos + strlen("{identifier}"),
                    suffix_len + 1);
            memcpy(identifier_pos, identifier, strlen(identifier));
        }
    }

    // Copy final topic to output buffer
    strncpy(topic_buffer, temp_buffer, buffer_size - 1);
    topic_buffer[buffer_size - 1] = '\0';

    return 0;
}

/*
    * @brief Generate a unique message ID
    * @return Pointer to the generated ID string
*/
char* Cloud_Protocol_GenerateMessageId(void)
{
    static char id_buffer[11] = {0};
    
    if (++cloud_protocol_message_counter > 4294967295)
    {
        cloud_protocol_message_counter = 0;
    }
    memset(id_buffer, 0, sizeof(id_buffer));
    snprintf((char*)id_buffer, sizeof(id_buffer), "%u", cloud_protocol_message_counter);

    return id_buffer;
}

/*
    * @brief get a unique message ID
    * @return Current message ID
*/
uint32_t Cloud_Protocol_GetMessageId(void)
{
    return cloud_protocol_message_counter;
}
/* EOL */
