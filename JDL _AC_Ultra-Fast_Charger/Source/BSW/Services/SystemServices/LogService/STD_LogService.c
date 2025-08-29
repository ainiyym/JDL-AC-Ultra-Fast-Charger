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

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static uint8_t gv_LogBuf[LOGSERVICE_BUF_MAX_SIZE + 1];
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
	uint32_t lv_ulLogSize = 0;

	if((module < LOG_MODULE_MAX) && (gv_stLogServiceCtrl.ulLogModule_32 & (1 << module)) && (gv_stLogServiceCtrl.ulLogLevel & (1 << level)))
	{
		lv_ulLogSize = vsnprintf((char *)gv_LogBuf, LOGSERVICE_BUF_MAX_SIZE, fmt, args); 
        if(lv_ulLogSize > LOGSERVICE_BUF_MAX_SIZE)
        {
            gv_LogBuf[LOGSERVICE_BUF_MAX_SIZE - 1] = 0; 
        }
        else
        {
            gv_LogBuf[lv_ulLogSize] = 0; 
        }
		if(gv_ucLogStatus != 0)
		{
            Mcal_Usart_AppSendData(LOG_SERVICE_USART_CH, gv_LogBuf, lv_ulLogSize);
		}
	}
	else
	{;}
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
        else
        {
            ;
        }
    }
    else
    {
        ;
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
        else
        {
            ;
        }
    }
    else
    {
        ;
    }
}

void LogService_Disable_All_Module(void)
{
    gv_stLogServiceCtrl.ulLogModule_32_Backup = gv_stLogServiceCtrl.ulLogModule_32;
    gv_stLogServiceCtrl.ulLogModule_32 = 0;
    gv_stLogServiceCtrl.ulLogDisable_Cnt++;
}

void LogService_Restore_All_Module(void)
{
    gv_stLogServiceCtrl.ulLogDisable_Cnt--;
    if (gv_stLogServiceCtrl.ulLogDisable_Cnt <= 0)
    {
        gv_stLogServiceCtrl.ulLogModule_32 = gv_stLogServiceCtrl.ulLogModule_32_Backup;
    }
    else
    {
        ;
    }
}

void LogService_Init_Module_Status(uint32_t lv_ulStatus)
{
    gv_stLogServiceCtrl.ulLogModule_32_Backup = gv_stLogServiceCtrl.ulLogModule_32;
    gv_stLogServiceCtrl.ulLogModule_32 = lv_ulStatus;
}

void LogService_Init_Level_Status(uint32_t lv_ulStatus)
{
    gv_stLogServiceCtrl.ulLogLevel_Backup = gv_stLogServiceCtrl.ulLogLevel;
    gv_stLogServiceCtrl.ulLogLevel = lv_ulStatus;
}

void LogService_Set_Module_Init_Value(uint8_t *lv_ucInitArr)
{
    if (NULL != lv_ucInitArr)
    {
        LIB_Copy(gv_stLogServiceCtrl.ucLogModuleInit_Value, lv_ucInitArr, 10);
    }
    else
    {
        ;
    }
}

void LogService_Set_Level_Init_Value(uint8_t *lv_ucInitArr)
{
    if (NULL != lv_ucInitArr)
    {
        LIB_Copy(gv_stLogServiceCtrl.ucLogLevelInit_Value, lv_ucInitArr, 10);
    }
    else
    {
        ;
    }
}

void LogService_Print_Hex_Array(Log_Module_Enum module, uint8_t *lv_ucHexArray, uint32_t lv_ulLen, uint8_t lv_ucR)
{
    if ((module < LOG_MODULE_MAX) && (gv_stLogServiceCtrl.ulLogModule_32 & (1 << module)))
    {
        uint8_t lv_ucBuf[LOGSERVICE_HEX_BUF_MAX_SIZE];
        uint32_t lv_ulIdx = 0;

        if ((NULL != lv_ucHexArray) && (0 != lv_ulLen) && (lv_ulLen < (LOGSERVICE_HEX_BUF_MAX_SIZE / 3)))
        {
            for (uint32_t i = 0; i < lv_ulLen; i++)
            {
                sprintf((char *)&lv_ucBuf[lv_ulIdx], "%02X ", lv_ucHexArray[i]);
                lv_ulIdx += 3;
            }

            if (lv_ucR > 0)
            {
                lv_ucBuf[lv_ulIdx] = '\n';
                lv_ulIdx++;
            }
            else
            {
                ;
            }
        }
        else
        {
            ;
        }
        if (gv_ucLogStatus != 0)
        {
            Mcal_Usart_AppSendData(LOG_SERVICE_USART_CH, lv_ucBuf, lv_ulIdx);
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
