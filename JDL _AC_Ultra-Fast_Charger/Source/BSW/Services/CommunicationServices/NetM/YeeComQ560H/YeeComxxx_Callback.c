#include "YeeComxxx_Callback.h"

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
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_POWER_ON, 1);
                YeeCom_Log("Device is powered on\r\n");
            }
            else
            {
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_POWER_ON, 0);
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
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_SIM_READY, 1);
                YeeCom_Log("sim is ready\r\n");
            }
            else
            {
                // sim is not ready
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_SIM_READY, 0);
                YeeCom_Log("sim is not ready\r\n");
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
            char status_str[10] = {0};
            size_t len = current - start < sizeof(status_str) - 1 ? current - start : sizeof(status_str) - 1;
            memcpy(status_str, start, len);
            if (strstr(status_str, "RESET :") != NULL)
            {
                // reset
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_RESET, 1);
                YeeCom_Log("yeeCOM RESET: %s\r\n", status_str);
            }
            else
            {
                // reset
                YeeCom_SetDeviceStatus(YEECOM_AT_OOB_CMD_RESET, 0);
            }
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
}

void YeeCom_At_Set_Parameter_Callback(void *arg, char *buf, int buflen)
{
    // Handle the received server response success
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
