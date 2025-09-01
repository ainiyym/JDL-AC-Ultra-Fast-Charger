#ifndef _YEECOM_XXX_AT_H_
#define _YEECOM_XXX_AT_H_
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "at_parser.h"
#include "wrappers_os.h"

#define YEECOM_OOB_CMD_POWER_ON_BUF_LEN                 (10u)
#define YEECOM_OOB_CMD_NET_READY_BUF_LEN                (20u)
#define YEECOM_OOB_CMD_RESET_BUF_LEN                    (20u)
#define YEECOM_OOB_CMD_DATA_PASSTHROUGH_BUF_LEN         (512u)

typedef enum
{
    YEECOM_AT_CMD_GET,
    YEECOM_AT_CMD_SET
} YeeCom_AT_Cmd_Get_Param_Type; /* AT command type */

typedef enum
{
    YEECOM_WORING_TCP,
    YEECOM_WORING_UDP,
    YEECOM_WORING_TCPS,
    YEECOM_WORING_HTTP,
    YEECOM_WORING_MQTT,
    YEECOM_WORING_MQTT_ALI_IOT,
    YEECOM_WORING_MQTT_ONENET,
    YEECOM_WORING_MQTT_IOT,
    YEECOM_WORING_HTTPS,
    YEECOM_WORING_MQTTS,
    YEECOM_WORING_WEBSOCKET,
    YEECOM_WORING_WSS,
    YEECOM_WORING_X
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
    YEECOM_AT_CMD_WORKING_MODE, /* DTU working mode */
    YEECOM_AT_CMD_GPRS_MODE, /* DTU online mode */
    YEECOM_AT_CMD_CH_MODE, /* DTU Central mode */
    YEECOM_AT_CMD_DEBUG_MODE, /* DTU Debug mode */
    YEECOM_AT_CMD_USART_CFG, /* Serial port parameter configuration */
    YEECOM_AT_CMD_USART_FRAME_INTERVAL_TIME, /* Serial port data frame interval time */
} YeeCom_AT_Cmd;

typedef enum
{
    YEECOM_AT_OOB_CMD_POWER_ON, /* OOB: power on */
    YEECOM_AT_OOB_CMD_SIM_READY, /* OOB: sim ready */
    YEECOM_AT_OOB_CMD_RESET, /* OOB: reset */
    YEECOM_AT_OOB_CMD_DATA_PASSTHROUGH, /* OOB: data passthrough */
} YeeCom_AT_Oob_enum;


#endif
