#include "YeeComxxx_Callback.h"
#include "YeeComxxx_Device.h"

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
}

/* at set cmd */
void YeeCom_At_Set_SERVERn_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
    if (NULL != strstr(buf, "OK\r\n"))
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CFG_CENTER, 1);
    }
    else
    {
        YeeCom_SetDeviceParameters(YEECOM_DEVICE_PARAM_CFG_CENTER, 0);
    }
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

/* at get cmd */
void YeeCom_At_Get_SERVERnCallback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
}

void YeeCom_At_Get_GPRSMODECallback(void *arg, char *buf, int buflen)
{
    // Handle the received GPRS mode response success
}

void YeeCom_At_Get_CHMODECallback(void *arg, char *buf, int buflen)
{
    // Handle the received CH mode response success
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
            char rssi[5] = {0};
            strncpy(rssi, start, current - start);
            YeeCom_SetDeviceInfo_rssi(atoi(rssi));
        }
    }
}
