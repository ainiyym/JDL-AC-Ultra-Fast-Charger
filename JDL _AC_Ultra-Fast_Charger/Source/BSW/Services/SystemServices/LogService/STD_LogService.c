//****************************************************************************************
//*
//* File Name: STD_LogService.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/

#include "stdio.h"
#include "STD_Lib.h"
#include "STD_LogService.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

static void LogSevice_Args(Log_Module_Enum module, Log_Level_Enum level, const char *fmt, va_list args);

#define Log(X, Y) void Log##X##_Ex(Log_Module_Enum module, const char *fmt, ...)\
{\
    va_list args;\
    va_start(args, fmt);\
    LogSevice_Args(module, Y, fmt, args);\
    va_end(args);\
}

Log(Trace, LOG_LEVEL_TRACE)
Log(Debug, LOG_LEVEL_DEBUG)
Log(Info,  LOG_LEVEL_INFO)
Log(Warn,  LOG_LEVEL_WARN)
Log(Error, LOG_LEVEL_ERROR)
Log(Critical, LOG_LEVEL_CRITICAL)

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
    uint8_t message[LOGSERVICE_BUF_MAX_SIZE+1];
    uint16_t length;
} Log_item_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static uint8_t gv_ucLogStatus = 0;
/* Initial configuration module and level log output control */
static LogServiceCtrl_Struct gv_stLogServiceCtrl = {  (  LOG_STATE_ON  << LOG_MODULE_XXX)
													  | (LOG_STATE_ON  << LOG_MODULE_AUTHM)
                                                      | (LOG_STATE_ON  << LOG_MODULE_NOAUTH)
													  | (LOG_STATE_ON  << LOG_MODULE_ENERGY)
                                                      | (LOG_STATE_ON  << LOG_MODULE_CURRM)
                                                      | (LOG_STATE_ON  << LOG_MODULE_VOLTM)
													  | (LOG_STATE_ON  << LOG_MODULE_EVSEM)
													  | (LOG_STATE_ON  << LOG_MODULE_HMI)
                                                      | (LOG_STATE_ON  << LOG_MODULE_CCP)
													  | (LOG_STATE_ON  << LOG_MODULE_CAN)
													  | (LOG_STATE_ON  << LOG_MODULE_CP)
													  | (LOG_STATE_ON  << LOG_MODULE_SWITCHM)
													  | (LOG_STATE_ON  << LOG_MODULE_METER)
													  | (LOG_STATE_ON  << LOG_MODULE_RLY)
													  | (LOG_STATE_ON  << LOG_MODULE_SENSOR)
													  | (LOG_STATE_ON  << LOG_MODULE_PE)
													  | (LOG_STATE_ON  << LOG_MODULE_RFID)
													  | (LOG_STATE_ON  << LOG_MODULE_BTRCTR)
                                                      | (LOG_STATE_ON  << LOG_MODULE_RS485)
                                                      | (LOG_STATE_ON  << LOG_MODULE_AT)
                                                      | (LOG_STATE_ON  << LOG_MODULE_CLOUDNETM)
                                                      | (LOG_STATE_ON  << LOG_MODULE_NVM)
													  | (LOG_STATE_ON  << LOG_MODULE_SYSM)
													  | (LOG_STATE_ON  << LOG_MODULE_MCAL),

													    (LOG_STATE_ON << LOG_LEVEL_TRACE)
													  | (LOG_STATE_ON << LOG_LEVEL_DEBUG)
													  | (LOG_STATE_ON << LOG_LEVEL_INFO)
													  | (LOG_STATE_ON << LOG_LEVEL_WARN)
													  | (LOG_STATE_ON << LOG_LEVEL_ERROR)
													  | (LOG_STATE_ON << LOG_LEVEL_CRITICAL),
													 };

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*the Module name config setting */
static const char* const gv_LogModule_Name[] =
{
	    "XXX",
	    "AUTHM",
        "NOAUTH",
		"ENERGY",
        "CURRM",
        "VOLTM",
		"EVSEM",
		"HMI",
        "CCP",
		"CAN",
		"CP",
		"SWITCHM",
		"METER",
		"RLY",
		"SENSOR",
		"PE",
		"RFID",
		"BTRCTR",
        "RS485",
        "AT",
        "CLOUDNETM",
        "NVM",
		"SYSM",
		"MCAL",
};

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
const char *LogService_Get_Module_Name(uint32_t lv_ulModuleIdx)
{
    const char *lv_ucName = "xxx";

    if (lv_ulModuleIdx < (sizeof(gv_LogModule_Name) / sizeof(gv_LogModule_Name[0])))
    {
        lv_ucName = gv_LogModule_Name[lv_ulModuleIdx];
    }
    else
    {
        ;
    }

    return lv_ucName;
}

static void LogSevice_Args(Log_Module_Enum module, Log_Level_Enum level, const char *fmt, va_list args)
{
    Log_item_t item;
    uint16_t xResult;

    // Critical section
    if ((module < LOG_MODULE_MAX) && (gv_stLogServiceCtrl.ulLogModule_32 & (1 << module)) && (gv_stLogServiceCtrl.ulLogLevel & (1 << level)))
    {
        item.length = vsnprintf((char *)item.message, LOGSERVICE_BUF_MAX_SIZE, fmt, args);
        if (item.length > LOGSERVICE_BUF_MAX_SIZE)
        {
            item.message[LOGSERVICE_BUF_MAX_SIZE - 1] = 0;
        }
        else
        {
            item.message[item.length] = 0;
        }
        xResult = Core_Printf_AddItem((const char *)item.message);
        if (xResult != item.length)
        {
            // Queue full, discard print content
            Core_printf("Log queue full, discarded log: %s", item.message);
        }
    }
}

void LogService_Set_Module(Log_Module_Enum Module, bool Enable)
{
    if (gv_stLogServiceCtrl.ulLogDisable_Cnt == 0)
    {
        if (Module < LOG_MODULE_MAX)
        {
            if (Enable)
            {
                gv_stLogServiceCtrl.ulLogModule_32 |= 1 << Module;
            }
            else
            {
                gv_stLogServiceCtrl.ulLogModule_32 &= ~(1 << Module);
            }
        }
    }
}

void LogService_Set_Level(Log_Level_Enum Level, bool Enable)
{
    if (gv_stLogServiceCtrl.ulLogDisable_Cnt == 0)
    {
        if (Level < LOG_LEVEL_MAX)
        {
            if (Enable)
            {
                gv_stLogServiceCtrl.ulLogLevel |= 1 << Level;
            }
            else
            {
                gv_stLogServiceCtrl.ulLogLevel &= ~(1 << Level);
            }
        }
    }
}

void LogService_Init_Module_Status(uint32_t lv_ulStatus)
{
    gv_stLogServiceCtrl.ulLogModule_32 = lv_ulStatus;
}

void LogService_Init_Level_Status(uint32_t lv_ulStatus)
{
    gv_stLogServiceCtrl.ulLogLevel = lv_ulStatus;
}

void LogService_Print_Hex_Array(Log_Module_Enum module, const uint8_t *hexArray, uint32_t len, uint8_t appendNewline)
{
    if ((module >= LOG_MODULE_MAX) || !(gv_stLogServiceCtrl.ulLogModule_32 & (1 << module)) || !hexArray || len == 0)
        return;

    uint8_t buf[LOGSERVICE_HEX_BUF_MAX_SIZE + 1];
    uint32_t idx = 0;
    uint16_t xResult;

    for (uint32_t i = 0; i < len && (idx + 3) < LOGSERVICE_HEX_BUF_MAX_SIZE; i++)
    {
        int written = snprintf((char *)&buf[idx], LOGSERVICE_HEX_BUF_MAX_SIZE - idx, "%02X ", hexArray[i]);
        if (written <= 0) break;
        idx += written;
    }

    if (appendNewline && idx < LOGSERVICE_HEX_BUF_MAX_SIZE)
        buf[idx++] = '\n';

    if (gv_ucLogStatus && idx > 0)
    {
        xResult = Core_Printf_AddItem((const char *)buf);
        if (xResult != idx)
        {
            // Queue full, discard print content
            Core_printf("Log queue full, discarded log: %s", buf);
        }
    }
}

void LogService_SetLogEnable(void)
{
    gv_ucLogStatus = 1;
}

void LogService_SetLogDisable(void)
{
    gv_ucLogStatus = 0;
}

