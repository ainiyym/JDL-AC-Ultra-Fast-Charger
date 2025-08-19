//****************************************************************************************
//*
//* File Name: STD_LogService.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/
#ifndef __STD_LOG_SERVICE_H__
#define __STD_LOG_SERVICE_H__

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include <stdbool.h>
#include <stdarg.h>
#include "STD_Types.h"
#include "Mcal_USART_Cfg.h"
#include "STD_LogService_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

#ifdef LOG_SERVICE_MODULE_ENABLE
#define LOG_TRACE(module, fmt, ...) LogTrace_Ex(module, "TRC %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)
#define LOG_DEBUG(module, fmt, ...) LogDebug_Ex(module, "DBG %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)
#define LOG_INFO(module, fmt, ...) LogInfo_Ex(module, "INF %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)
#define LOG_WARN(module, fmt, ...) LogWarn_Ex(module, "WRN %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)
#define LOG_ERROR(module, fmt, ...) LogError_Ex(module, "ERR %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)
#define LOG_CRITICAL(module, fmt, ...) LogCritical_Ex(module, "CRI %s:" fmt, LogService_Get_Module_Name(module), ##__VA_ARGS__)

#else
#define LOG_TRACE(...) 		((void)0)
#define LOG_DEBUG(...) 		((void)0)
#define LOG_INFO(...) 		((void)0)
#define LOG_WARN(...) 		((void)0)
#define LOG_EEROR(...) 		((void)0)
#define LOG_CRITICAL(...) 	((void)0)
#endif

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/* ought to be the same with gv_LogModule_Name */
typedef enum 
{
	LOG_STATE_OFF 	= 0x00,
	LOG_STATE_ON 	= 0x01,
}Log_State_Enum;

typedef enum LogLevel
{
    LOG_LEVEL_TRACE			= 0x00,                                    
    LOG_LEVEL_DEBUG			= 0x01,                                    
    LOG_LEVEL_INFO			= 0x02,                              
    LOG_LEVEL_WARN			= 0x03,                              
    LOG_LEVEL_ERROR			= 0x04,                           
    LOG_LEVEL_CRITICAL		= 0x05,
    LOG_LEVEL_MAX
}Log_Level_Enum;

typedef enum LogModule
{
    LOG_MODULE_XXX = 0x00,
    LOG_MODULE_AUTHM,
    LOG_MODULE_NOAUTH,
	LOG_MODULE_ENERGY,
    LOG_MODULE_CURRM,
    LOG_MODULE_VOLTM,
    LOG_MODULE_EVSEM,
    LOG_MODULE_HMI,
	LOG_MODULE_CCP,
	LOG_MODULE_CAN,
    LOG_MODULE_CP,
    LOG_MODULE_SWITCHM,
    LOG_MODULE_METER,
    LOG_MODULE_RLY,
    LOG_MODULE_SENSOR,
    LOG_MODULE_PE,
    LOG_MODULE_RFID,
    LOG_MODULE_BTRCTR,
    LOG_MODULE_RS485,
    
	LOG_MODULE_SYSM,
	LOG_MODULE_MCAL,
	LOG_MODULE_MAX,
}Log_Module_Enum;

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

typedef struct 
{
	uint32_t ulLogModule_32;  	/* log Module Output Enable */
	uint32_t ulLogLevel;			/* Log Level Output Enable */
	uint32_t ulLogModule_32_Backup;  	
	uint32_t ulLogLevel_Backup;	
	uint32_t ulLogDisable_Cnt;
	uint8_t  ucLogModuleInit_Value[10]; /* the EEPROM log_Module Cache config setting value */
	uint8_t  ucLogLevelInit_Value[10];	/* the EEPROM log_Level Cache config setting value */
}LogServiceCtrl_Struct;

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

extern const char *LogService_Get_Module_Name(uint32_t lv_ulModuleIdx);

extern void LogTrace_Ex(Log_Module_Enum module, const char *fmt, ...);
extern void LogDebug_Ex(Log_Module_Enum module, const char *fmt, ...);
extern void LogInfo_Ex(Log_Module_Enum module, const char *fmt, ...);
extern void LogWarn_Ex(Log_Module_Enum module, const char *fmt, ...);
extern void LogError_Ex(Log_Module_Enum module, const char *fmt, ...);
extern void LogCritical_Ex(Log_Module_Enum module, const char *fmt, ...);

extern void LogService_Set_Module(Log_Module_Enum Module, bool Enable);
extern void LogService_Set_Level(Log_Level_Enum Level, bool Enable);
extern void LogService_Disable_All_Module(void);
extern void LogService_Restore_All_Module(void);

extern void LogService_Set_Module_Init_Value(uint8_t *lv_ucInitArr);
extern void LogService_Set_Level_Init_Value(uint8_t *lv_ucInitArr);
extern void LogService_Setting_Init(void);

extern void LogService_Print_Hex_Array(Log_Module_Enum module, uint8_t *lv_ucHexArray, uint32_t lv_ulLen, uint8_t lv_ucR);

extern void LogService_SetLogEnable(void);
extern void LogService_SetLogDisable(void);
#endif

