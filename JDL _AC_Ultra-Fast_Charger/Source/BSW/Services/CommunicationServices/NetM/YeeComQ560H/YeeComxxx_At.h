#ifndef _YEECOM_XXX_AT_H_
#define _YEECOM_XXX_AT_H_

#define YEECOM_OOB_CMD_POWER_ON_BUF_LEN                 (15u)
#define YEECOM_OOB_CMD_NET_READY_BUF_LEN                (30u)
#define YEECOM_OOB_CMD_RESET_BUF_LEN                    (20u)
#define YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN         (222u)

typedef enum
{
    YEECOM_AT_CMD_GET,
    YEECOM_AT_CMD_SET
} YeeCom_AT_Cmd_Get_Param_Type; /* AT command type */

typedef enum
{
    YEECOM_WORKING_TCP = 0,
    YEECOM_WORKING_UDP = 1,
    YEECOM_WORKING_TCPS = 2,
    YEECOM_WORKING_HTTP = 8,
    YEECOM_WORKING_MQTT = 9,
    YEECOM_WORKING_MQTT_ALI_IOT = 10,
    YEECOM_WORKING_MQTT_ONENET = 11,
    YEECOM_WORKING_MQTT_IOT = 12,
    YEECOM_WORKING_HTTPS = 13,
    YEECOM_WORKING_MQTTS = 14,
    YEECOM_WORKING_WEBSOCKET = 19,
    YEECOM_WORKING_WSS = 20,
    YEECOM_WORKING_X
} YeeCom_WorkingMode_E; /* DTU working mode ENUM */

typedef enum
{
    YEECOM_ONLINE_MODE_OFFLINE,
    YEECOM_ONLINE_MODE_KEEP_ALIVE,
    YEECOM_ONLINE_MODE_WAKE_ONLINE,
    YEECOM_ONLINE_MODE_ON_DEMAND_ONLINE,
    YEECOM_ONLINE_MODE_PERIOD_ONLINE
} YeeCom_OnlineMode_E; /* DTU online mode ENUM */

typedef enum
{
    YEECOM_CENTRAL_MODE_SINGLE_MAIN_CENTER,
    YEECOM_CENTRAL_MODE_PRIMARY_BACKUP_WEITCHING,
    YEECOM_CENTRAL_MODE_MULTI_HOMED_CONNECTION_INTEGRATED,
    YEECOM_CENTRAL_MODE_MULTI_HOMED_CONNECTION_STANDALONE
} YeeCom_CentralMode_E; /* DTU Central mode ENUM */

typedef enum
{
    YEECOM_DEBUG_MODE_DISABLE,
    YEECOM_DEBUG_MODE_ECHO, 
    YEECOM_DEBUG_MODE_TRACE,
    YEECOM_DEBUG_MODE_CONNECTIVITY_STATUS_MODE
} YeeCom_DebugMode_E; /* DTU debug mode ENUM */

typedef enum
{
    YEECOM_REGPKG_DISABLE,
    YEECOM_REGPKG_ENABLE_UUID,
    YEECOM_REGPKG_ENABLE_ASCCII,
    YEECOM_REGPKG_ENABLE_HEX,
    YEECOM_REGPKG_ENABLE_RESERVED,
    YEECOM_REGPKG_ENABLE_MQTT,
    YEECOM_REGPKG_ENABLE_OTHER
} YeeCom_Regpkg_E; /* DTU Registration packet mode ENUM */

typedef enum
{
    YEECOM_AT_CMD_WORKING_MODE, /* DTU working mode */
    YEECOM_AT_CMD_GPRS_MODE, /* DTU online mode */
    YEECOM_AT_CMD_CH_MODE, /* DTU Central mode */
    YEECOM_AT_CMD_HBTIME, /* DTU Heartbeat time */
    YEECOM_AT_CMD_HBHEAD, /* DTU Heartbeat packet header */
    YEECOM_AT_CMD_REGPKG, /* DTU Registration packet mode */
    YEECOM_AT_CMD_REGHEAD, /* DTU Registration packet header */
    YEECOM_AT_CMD_DEBUG_MODE, /* DTU Debug mode */
    YEECOM_AT_CMD_USART_CFG, /* Serial port parameter configuration */
    YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME, /* Serial port data frame interval time */
    YEECOM_AT_CMD_DEVICE_RESET, /* Device reset to default configuration */
    YEECOM_AT_CMD_DEVICE_RESTART, /* Device restart cmd */
    YEECOM_AT_CMD_ICCID, /* Get SIM ICCID */
    YEECOM_AT_CMD_IMEI, /* Get device IMEI */
    YEECOM_AT_CMD_RSSI, /* Get device RSSI */
    YEECOM_AT_CMD_WAKEUP, /* Wake up DTU */
} YeeCom_AT_Cmd;

#endif
