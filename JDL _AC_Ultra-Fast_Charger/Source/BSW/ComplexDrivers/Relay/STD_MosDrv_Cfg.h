//****************************************************************************************
//*
//* File Name: STD_MosDrv_Cfg.h
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-05-16 11:27:24
//* Author: JDLzhou
//* 
//****************************************************************************************/

#if !defined (_STD_MOSDRV_CFG_H)
/* polyspace<MISRA-C3:2.5:Not a defect:Other> NO fluence *//* polyspace<MISRA-C3:21.1:Not a defect:Other> NO fluence */
#define _STD_MOSDRV_CFG_H

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Std_Types.h"
#include "Mcal_Gpio_Cfg.h"
#include "STD_LogService.h"
#include "STD_SysM.h"
/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define MOSDRV_DEBUG(fmt, ...) 					LOG_DEBUG(LOG_MODULE_RLY, fmt, ##__VA_ARGS__)
#define MOS_SHORT_SELFCHECK_NUM_MAX             (0U)                                  /*short circuit self test number*/
#define MOS_DRV_LOW                             (MCAL_GPIO_PIN_RESET)                 /*MOS drive low*/
#define MOS_DRV_HIGH                            (MCAL_GPIO_PIN_SET)                   /*MOS drive high*/

#define MOSDRV_TASK_PERIOD          			(5U)                                  /*MOS switching idle*/
#define MOSDRV_WAIT_OVERTIME  			        (uint16_t)( 200u / MOSDRV_TASK_PERIOD ) /*overtime wait*/

/*******************************************************************************
|    LOWER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    UPPER LAYER CONFIGURATION PARAMETERS
|******************************************************************************/

/*******************************************************************************
|    Callback Notification
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
typedef enum
{
	MOSDRV_LOW,	 /*MOS drive low*/
	MOSDRV_HIGH, /*MOS drive high*/
} MosDrvLevel_Enum;

typedef enum
{
	MOS_DRV_CONNECTOR1_OFF,	/* Connector 1 disconnects the switch */
	MOS_DRV_CONNECTOR1_ON,	/* Connector 1: Engagement switch */
	MOS_DRV_CONNECTOR2_OFF,	/* Connector 2 disconnects the switch */
	MOS_DRV_CONNECTOR2_ON,	/* Connector 2: Engagement switch */
	MOS_DRV_CONTACTIOR_SWITCH_NUM_MAX
} MosDrvConnectorSwitch_Enum;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	GPIO_TypeDef *ucGpioPort;		/*Gpio port*/
	uint32_t ucGpioPinIndx;	/*Gpio pin*/
	uint8_t ucGpioLevelLower; /*Gpio level*/
} MosDrv_Struct;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Global Variable with extern linkage
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void MOSDRV_WriteGpioValue(uint8_t lv_ucIndex,uint8_t lv_ucLevel);
extern uint8_t MOSDRV_ReadGpioValue(uint8_t lv_ucIndex);
extern void MOSDRV_ShortSelfCheckWriteGpioValue(uint8_t lv_ucIndex,uint8_t lv_ucLevel);
extern uint8_t MOSDRV_ShortSelfCheckReadGpioValue(uint8_t lv_ucIndex);
#endif
/*EOF*/
