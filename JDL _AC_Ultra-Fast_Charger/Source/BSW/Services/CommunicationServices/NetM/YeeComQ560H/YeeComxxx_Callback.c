#include "YeeComxxx_Callback.h"
#include "YeeComxxx_Device.h"
#include "YeeComxxx_Device_Cfg.h"
#include "CloudNet_Protocol_Msg.h"
#include "Cloud_Protocol_Cfg.h"

/* oob cmd */
void YeeCom_At_OOB_Power_On_Callback(void *arg, char *buf, int buflen)
{
    const char *start = NULL;
    const char *current = NULL;

    if ((start = strstr(buf, "+EIND:")) != NULL)
    {
        start += strlen("+EIND:");
        while (*start == ' ')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char status_str[4] = {0};
            size_t len = current - start < sizeof(status_str) - 1 ? current - start : sizeof(status_str) - 1;
            memcpy(status_str, start, len);

            char *endptr;
            long status = strtol(status_str, &endptr, 10);

            // Add error checking
            if (endptr == status_str)
            {
                YeeCom_Err("Error: Unable to parse status value: %s\r\n", status_str);
                return;
            }
            else if (*endptr != '\0')
            {
                YeeCom_Log("Warning: Status value contains invalid characters: %s\r\n", status_str);
            }

            if (status == 128)
            {
                YeeCom_SetDeviceState(YEECOM_POWER_ON, 1);
                YeeCom_Log("Device is powered on\r\n");
            }
            else
            {
                YeeCom_SetDeviceState(YEECOM_POWER_ON, 0);
                YeeCom_Log("Device is powered off\r\n");
            }
        }
    }
}

void YeeCom_At_OOB_Net_Ready_Callback(void *arg, char *buf, int buflen)
{
    const char *start = buf;
    const char *current = buf;

    const char *csim_pos = strstr(buf, "+CSIM:");
    if (csim_pos != NULL)
    {
        start = csim_pos + strlen("+CSIM:");
        while (*start == ' ')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char status_str[20] = {0};
            size_t len = current - start < sizeof(status_str) - 1 ? current - start : sizeof(status_str) - 1;
            memcpy(status_str, start, len);
            if (strstr(status_str, "SMS Ready") != NULL)
            {
                // sim is ready
                YeeCom_SetDeviceInfo_sim(1);
            }
            else
            {
                YeeCom_Log("<%s> %s\r\n", __func__, status_str);
            }
        }
    }
}

void YeeCom_At_OOB_Net_Reset_Callback(void *arg, char *buf, int buflen)
{
    const char *start = buf;
    const char *current = buf;

    const char *reset_pos = strstr(buf, "+SYSTEM:");
    if (reset_pos != NULL)
    {
        start = reset_pos + strlen("+SYSTEM:");
        while (*start == ' ')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char status_str[15] = {0};
            size_t len = current - start < sizeof(status_str) - 1 ? current - start : sizeof(status_str) - 1;
            memcpy(status_str, start, len);
            if (strstr(status_str, "RESET 12") != NULL)
            {
                YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_POWER_ON, 1);
            }
            else if(strstr(status_str, "RESET 4") != NULL)
            {
                YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_CMD, 1);
            }
            else if(strstr(status_str, "RESET 5") != NULL)
            {
                YeeCom_SetDeviceState(YEECOM_DEVICE_RESET_CH, 1);
            }
            else
            {
  
            }

            if (strstr(status_str, "RESET") != NULL)
            {
                YeeCom_SetDeviceState(YEECOM_DEVICE_RESET, 1);
            }
            YeeCom_Log("<%s>: %s\r\n", __func__, status_str);
        }
    }
}

void YeeCom_At_OOB_Data_Passthrough_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received data passthrough response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "RCVPORT");
    if (reset_pos != NULL)
    {
        start = reset_pos + strlen("RCVPORT");
        uint8_t socket_id = 0;
        if (*start >= '0' && *start <= '9')
        {
            socket_id = *start - '0';
            YeeCom_Log("<%s> socket_id: %d\r\n", __func__, socket_id);
            start++;
        }
        const char *data_start = strchr(start, '=');
        if (data_start != NULL)
        {
            data_start++;
        }
        uint16_t hex_data_len = buflen - (data_start - buf);
        if (hex_data_len > 0)
        {
            uint8_t *binary_data = (uint8_t *)pvPortMalloc(hex_data_len);
            if (binary_data != NULL)
            {
                for (int i = 0; i < hex_data_len; i++)
                {
                    memcpy(binary_data + i, data_start + i, 1);
                }

                uint8_t msg[hex_data_len + 1];
                memcpy(&msg[1], binary_data, hex_data_len);
                vPortFree(binary_data);

                switch (socket_id)
                {
                    case TCP_ID_PROTOCOL_GAGA:
                        msg[0] = CLOUD_MESSAGE_DATA_TYPE_CLOUD_PROTOCOL;
                        YeeCom_Log("<%s> rcv len: %d send data passthrough:\r\n", __func__, hex_data_len);
                        YeeCom_Print_Hex(msg, sizeof(msg));
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, sizeof(msg), CLOUD_MESSAGE_TYPE_DATA_PASSTHROUGH);
                        break;
                    default:
                        // Handle unknown socket
                        break;
                }
            }
        }
    }
    else
    {
        YeeCom_Err("<%s> fail parse data:\r\n", __func__);
        YeeCom_Print_Hex(buf, buflen);
    }
}

/* at set cmd */
void YeeCom_At_Set_SERVERn_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    YeeCom_Log("<%s> %s\r\n", __func__,  buf);
}

void YeeCom_At_Set_GPRSMode_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received GPRSMode response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_GPRS_MODE, 1);
    }
    else 
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_GPRS_MODE, 0);
    }
    YeeCom_Log("<%s> %s\r\n", __func__,  buf);
}

void YeeCom_At_Set_CHMode_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CH_MODE, 1);
    }
    else
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CH_MODE, 0);
    }
    YeeCom_Log("<%s> %s\r\n", __func__,  buf);
}

void YeeCom_At_Set_HBTime_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received Heartbeat time response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_Log("<%s> %s\r\n", __func__,  buf);  
    }
}

void YeeCom_At_Set_HBHead_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received Heartbeat header response success
}

void YeeCom_At_Set_REGPKG_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received Registration packet mode response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_Log("<%s> %s\r\n", __func__,  buf);
    }
}

void YeeCom_At_Set_REGHEAD_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received Registration packet header response success
}

void YeeCom_At_Set_DebugMode_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received Debug mode response success
}

void YeeCom_At_Set_USART_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received USART response success
}

void YeeCom_At_Set_DFI_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received DFI response success
}

void YeeCom_At_Set_RESET_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received OOB response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_RESET, 1);
        YeeCom_Log("<%s>  OK\r\n", __func__);
    }
    else
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_RESET, 0);
        YeeCom_Log("<%s> %s\r\n", __func__,  buf);
    }
}

void YeeCom_At_Set_RESTART_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received OOB response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_RESTART, 1);
        YeeCom_Log("<%s>  OK\r\n", __func__);
    }
    else
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_RESTART, 0);
        YeeCom_Log("<%s> %s\r\n", __func__,  buf);
    }
}

void YeeCom_At_Set_WAKEUP_Callback(void *arg, char *buf, int buflen)
{
    if (NULL != strstr(buf, "OK\r\n"))
    {
        uint8_t msg[2] = {0};
        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_WAKE_UP_DTU;
        msg[1] = 1; // Success
        CloudNet_Protocol_SendMsg((uint8_t *)msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
    }
}

void YeeCom_At_Set_MQSET_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    YeeCom_Log("<%s> %s\r\n", __func__, buf);
}

void YeeCom_At_Set_MQTOP_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    YeeCom_Log("<%s> %s\r\n", __func__, buf);
}

void YeeCom_At_Set_PUBTOP_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    YeeCom_Log("<%s> %s\r\n", __func__, buf);
}

/* at get cmd */
void YeeCom_At_Get_SERVERnCallback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+SERVER");
    if (reset_pos != NULL)
    {
        uint8_t socket_id = 0;
        uint8_t connect_type = 0;
        uint8_t ip[16] = {0};
        uint16_t port = 0;
        uint8_t msg[2] = {0};

        int result = sscanf(start, "+SERVER%hhu:%hhu,%15[^,],%hu#",
                            &socket_id, &connect_type, ip, &port);

        if (result == 4)
        {
            YeeCom_Log("<%s> socket_id: %d, type: %d, ip: %s, port: %d\r\n", __func__, socket_id, connect_type, ip, port);

            if (socket_id < TCP_ID_MAXIMUM)
            {
                if (connect_type == YEECOM_WORKING_TCP || connect_type == YEECOM_WORKING_TCPS) // TCP or SSL
                {
                    if (strcmp((const char *)ip, (const char *)Cloud_Tcp_Parameter[socket_id].ip) == 0 && port == Cloud_Tcp_Parameter[socket_id].port)
                    {
                        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM;
                        msg[1] = CLOUD_DEVICE_STATUS_CONNECTED;
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else if (connect_type == YEECOM_WORKING_MQTT_ONENET)
                {
                    if (strcmp((const char *)ip, (const char *)Cloud_Tcp_Parameter[socket_id].ip) == 0 && port == Cloud_Tcp_Parameter[socket_id].port)
                    {
                        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_SET_NETWORK_PARAM;
                        msg[1] = CLOUD_DEVICE_STATUS_CONNECTED;
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else if (connect_type == YEECOM_DEFAULT_NET_TYPE)
                {
                    if (strcmp((const char *)ip, YEECOM_DEFAULT_REMOTE_IP) == 0 && port == YEECOM_DEFAULT_REMOTE_PORT)
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CFG_CENTER, 1);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else
                {
                    YeeCom_Log("<%s> socket_id: %d unsupported connection type: %d\r\n", __func__, socket_id, connect_type);
                }
            }
            else
            {
                YeeCom_Log("<%s> Unknown socket_id: %d\r\n", __func__, socket_id);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse SERVER response: %s\r\n", __func__, buf);
            return;
        }
    }
}

void YeeCom_At_Get_GPRSMODECallback(void *arg, char *buf, int buflen)
{
    // Handle the received GPRS mode response success
}

void YeeCom_At_Get_CHMODECallback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
}

void YeeCom_At_Get_HBTimeCallback(void *arg, char *buf, int buflen)
{
    // Handle the received Heartbeat time response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+HBTIME");
    if (reset_pos != NULL)
    {
        uint16_t hb_time = 0;

        int result = sscanf(start, "+HBTIME:%hu", &hb_time);

        if (result == 1)
        {
            YeeCom_Log("<%s> Heartbeat time: %d seconds\r\n", __func__, hb_time);
            if (CLOUD_4G_HEARTBEAT_INTERVAL_S == hb_time)
            {
                uint8_t msg[2] = {0};
                msg[0] = CLOUD_MESSAGE_CTRL_TYPE_SET_HEARTBEAT_PARAM;
                msg[1] = 1; // Success
                CloudNet_Protocol_SendMsg((uint8_t *)msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse HBTIME response: %s\r\n", __func__, buf);
            return;
        }
    }
}

void YeeCom_At_Get_HBHeadCallback(void *arg, char *buf, int buflen)
{
    // Handle the received Heartbeat header response success
}

void YeeCom_At_Get_REGPKGCallback(void *arg, char *buf, int buflen)
{
    // Handle the received Registration packet mode response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+REGPKG");
    if (reset_pos != NULL)
    {
        uint8_t reg_pkg = 0;

        int result = sscanf(start, "+REGPKG:%hhu", &reg_pkg);

        if (result == 1)
        {
            YeeCom_Log("<%s> Registration package mode: %d\r\n", __func__, reg_pkg);
            if (reg_pkg < YEECOM_REGPKG_ENABLE_OTHER)
            {
                uint8_t msg[2] = {0};
                msg[0] = CLOUD_MESSAGE_CTRL_TYPE_SET_REGPKG_MODE;
                msg[1] = 1; // Success
                CloudNet_Protocol_SendMsg((uint8_t *)msg, 2, CLOUD_MESSAGE_TYPE_CTRL);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse HBTIME response: %s\r\n", __func__, buf);
            return;
        }
    }
}

void YeeCom_At_Get_REGHEADCallback(void *arg, char *buf, int buflen)
{
    // Handle the received Registration packet header response success
}

void YeeCom_At_Get_DBGMODECallback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
}

void YeeCom_At_Get_UARTCallback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
}

void YeeCom_At_Get_DFICallback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
}

void YeeCom_At_Get_ICCIDCallback(void *arg, char *buf, int buflen)
{
    const char *start = buf;
    const char *current = buf;

    const char *reset_pos = strstr(buf, "+ICCID:");
    if (reset_pos != NULL)
    {
        start = reset_pos + strlen("+ICCID:");
        while (*start == ' ' || *start == ':')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char iccid[21] = {0};
            strncpy(iccid, start, current - start);
            YeeCom_SetDeviceInfo_iccid(iccid);
        }
    }
}

void YeeCom_At_Get_IMEICallback(void *arg, char *buf, int buflen)
{
    const char *start = buf;
    const char *current = buf;

    const char *reset_pos = strstr(buf, "+IMEI:");
    if (reset_pos != NULL)
    {
        start = reset_pos + strlen("+IMEI:");
        while (*start == ' ' || *start == ':')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char imei[16] = {0};
            strncpy(imei, start, current - start);
            YeeCom_SetDeviceInfo_imei(imei);
        }
    }
}

void YeeCom_At_Get_RSSICallback(void *arg, char *buf, int buflen)
{
    const char *start = buf;
    const char *current = buf;

    const char *reset_pos = strstr(buf, "+CSQ:");
    if (reset_pos != NULL)
    {
        start = reset_pos + strlen("+CSQ:");
        while (*start == ' ' || *start == ':')
        {
            start++;
        }
        current = start;
        while (*current != '\r' && *current != '\n' && *current != '\0')
        {
            current++;
        }
        if (current > start)
        {
            char rssi[5] = {0};
            strncpy(rssi, start, current - start);
            YeeCom_SetDeviceInfo_rssi(atoi(rssi));
        }
    }
}

 /* +GSTATE:1,1,0,0 */
void YeeCom_At_Get_GSTATECallback(void *arg, char *buf, int buflen)
{
    const char *reset_pos = strstr(buf, "+GSTATE:");
    if (reset_pos != NULL)
    {
        char status[4 + 1] = {0};

        int parsed = sscanf(reset_pos, "+GSTATE:%1[^,\r\n],%1[^,\r\n],%1[^,\r\n],%1[^,\r\n]",
                            &status[0], &status[1], &status[2], &status[3]);

        if (parsed == 4)
        {
            CLOUD_INFO("GSTATE parsed: =%s\r\n", status);
            for(tcp_id_enum i = TCP_ID_PROTOCOL_GAGA; i < TCP_ID_MAXIMUM; i++)
            {
                YeeCom_SetDeviceInfo_gstate(i, atoi((const char *)&status[i]));
            }
        }
        else
        {
            CLOUD_ERROR("<%s> Failed to parse GSTATE response: %s\r\n", __func__, buf);
        }
    }
    else
    {
        CLOUD_ERROR("<%s> Failed to find +GSTATE in response: %s\r\n", __func__, buf);
    }
}

/* +MQSETn:<ClientID>,< user name>,<password>
OK */
void YeeCom_At_Get_MQSET_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+MQSET");
    if (reset_pos != NULL)
    {
        int socket_id = 0;
        uint8_t ClientID[65] = {0};
        uint8_t UserName[65] = {0};
        uint8_t Password[65] = {0};
        uint8_t msg[3] = {0};

        int result = sscanf(start, "+MQSET%d:%64[^,],%64[^,],%64[^\r\n]",
                            &socket_id, ClientID, UserName, Password);

        if (result == 4)
        {
            YeeCom_Log("<%s> socket_id: %d, ClientID: %s, UserName: %s, Password: %s\r\n", __func__, socket_id, ClientID, UserName, Password);

            if (socket_id < TCP_ID_MAXIMUM)
            {
                if (socket_id == TCP_ID_PROTOCOL_SG)
                {
                    if (strcmp((const char *)ClientID, (const char *)CLOUD_PROTOCOL_SG_MQTT_CLIENT_IDCLIENT_ID) == 0 &&
                        strcmp((const char *)UserName, (const char *)CLOUD_PROTOCOL_SG_MQTT_CLIENT_IDUSERNAME) == 0 &&
                        strcmp((const char *)Password, (const char *)CLOUD_PROTOCOL_SG_MQTT_CLIENT_IDPASSWORD) == 0)
                    {
                        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
                        msg[1] = CLOUD_PROTOCOL_MQTT_CTRL_TYPE_CONNECT;
                        msg[2] = 1; // Success
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, 3, CLOUD_MESSAGE_TYPE_CTRL);
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else
                {
                    YeeCom_Log("<%s> socket_id: %d unsupported MQTT socket\r\n", __func__, socket_id);
                }
            }
            else
            {
                YeeCom_Log("<%s> Unknown socket_id: %d\r\n", __func__, socket_id);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse MQSETn response: %s\r\n", __func__, buf);
            return;
        }
    }
}

extern char* Cloud_Protocol_Sg_GetMqttTopic_Subscribe_Current(void);
extern char* Cloud_Protocol_Sg_GetMqttTopic_Publish_Current(void);
/* +MQTOPn:<sub topic>,<pub topic>
OK */
void YeeCom_At_Get_MQTOP_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+MQTOP");
    if (reset_pos != NULL)
    {
        int socket_id = 0;
        uint8_t sub_topic[128] = {0};
        uint8_t pub_topic[64] = {0};
        uint8_t msg[3] = {0};

        int result = sscanf(start, "+MQTOP%d:%128[^,],%64[^\r\n]",
                            &socket_id, sub_topic, pub_topic);

        if (result == 3)
        {
            YeeCom_Log("<%s> socket_id: %d, sub_topic: %s, pub_topic: %s\r\n", __func__, socket_id, sub_topic, pub_topic);

            if (socket_id < TCP_ID_MAXIMUM)
            {
                if (socket_id == TCP_ID_PROTOCOL_SG)
                {
                    if (strcmp((const char *)sub_topic, (const char *)Cloud_Protocol_Sg_GetMqttTopic_Subscribe_Current()) == 0 \
                        && strcmp((const char *)pub_topic, (const char *)Cloud_Protocol_Sg_GetMqttTopic_Publish_Current()) == 0)
                    {
                        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
                        msg[1] = CLOUD_PROTOCOL_MQTT_CTRL_TYPE_SUBSCRIBE_PUBLISH;
                        msg[2] = 1; // Success
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, 3, CLOUD_MESSAGE_TYPE_CTRL);
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else
                {
                    YeeCom_Log("<%s> socket_id: %d unsupported MQTT socket\r\n", __func__, socket_id);
                }
            }
            else
            {
                YeeCom_Log("<%s> Unknown socket_id: %d\r\n", __func__, socket_id);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse MQTOP response: %s\r\n", __func__, buf);
            return;
        }
    }
}

void YeeCom_At_Get_PUBTOP_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    const char *start = buf;

    const char *reset_pos = strstr(buf, "+PUBTOP");
    if (reset_pos != NULL)
    {
        int socket_id = 0;
        uint8_t pub_topic[64] = {0};
        uint8_t msg[3] = {0};

        int result = sscanf(start, "+PUBTOP%d:%64[^\r\n]",
                            &socket_id, pub_topic);

        if (result == 2)
        {
            YeeCom_Log("<%s> socket_id: %d, pub_topic: %s\r\n", __func__, socket_id, pub_topic);

            if (socket_id < TCP_ID_MAXIMUM)
            {
                if (socket_id == TCP_ID_PROTOCOL_GAGA)
                {
                    if (strcmp((const char *)pub_topic, (const char *)Cloud_Protocol_Sg_GetMqttTopic_Publish_Current()) == 0)
                    {
                        msg[0] = CLOUD_MESSAGE_CTRL_TYPE_CLOUD_MQTT_SG;
                        msg[1] = CLOUD_PROTOCOL_MQTT_CTRL_TYPE_PUBLISH;
                        msg[2] = 1; // Success
                        CloudNet_Protocol_SendMsg((uint8_t *)msg, 3, CLOUD_MESSAGE_TYPE_CTRL);
                        YeeCom_Log("<%s> socket_id: %d parameters match\r\n", __func__, socket_id);
                    }
                    else
                    {
                        YeeCom_Log("<%s> socket_id: %d parameters mismatch, reconfigure\r\n", __func__, socket_id);
                    }
                }
                else
                {
                    YeeCom_Log("<%s> socket_id: %d unsupported MQTT socket\r\n", __func__, socket_id);
                }
            }
            else
            {
                YeeCom_Log("<%s> Unknown socket_id: %d\r\n", __func__, socket_id);
            }
        }
        else
        {
            YeeCom_Log("<%s> Failed to parse PUBTOP response: %s\r\n", __func__, buf);
            return;
        }
    }
}
