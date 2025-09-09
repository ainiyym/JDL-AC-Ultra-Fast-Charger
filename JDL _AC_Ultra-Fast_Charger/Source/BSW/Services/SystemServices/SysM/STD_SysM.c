//******************************************************************************
//*
//* File Name: STD_SysM.c
//* Project Name: ChargingSys
//* Version: v1.0
//* Date: 2025-07-04 11:27:24
//* Author: JDLzhou
//* 
/*******************************************************************************/

/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "STD_SysM.h"
#include "Mcal_App.h"
#include "Comm.h"
#include "SwitchM.h"
#include "STD_EvseM.h"
#include "CpM.h"
#include "STD_ErrorHandler.h"
#include "STD_RlyM.h"
#include "STD_AuthM.h"
#include "STD_Curr.h"
#include "STD_Volt.h"
#include "BtrM.h"
#include "ModbusM.h"
#include "FanM.h"
#include "Sensor.h"
/*******************************************************************************
|    Macro Definition
|******************************************************************************/
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t CpStatus[SYS_CONNECTOR_NUM_MAX]; /* Cp status */
	uint8_t EVSEStatus[SYS_CONNECTOR_NUM_MAX]; /* EVSE status */
	uint8_t StopChargingReason[SYS_CONNECTOR_NUM_MAX]; /* Stop charging reason */
	uint32_t ul10msCnt; /* Counter */
}SysM_BasicInfo_Struct;


typedef struct
{
	SysM_BasicInfo_Struct basic_ctrl_info; /* Basic control info */
	uint8_t ucSysReady10msCnt;	   /* System ready status counter */
	uint8_t ucCpOutMode[SYS_CONNECTOR_NUM_MAX];           /* CP output mode */
	uint8_t ucLowPowerShutdownFlag;
	uint8_t ucLowPowerShutdownCnt;
	uint8_t usRemoteResetFlag; /* 0: No reset request; 1: Reset immediately; 2:Reset when the conditions are satisfied.*/
	uint32_t ulSystemStatus[SYS_CONNECTOR_NUM_MAX];   /* System status word. Bitwise, definition see STD_SysM_SysStatus_t, 1: defined status exists; 0: does not exist.*/
} SysM_Struct;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/
static SysM_Struct stSysM;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void SYSM_ShowUserInfo(void);
static void SYSM_RemoteResetManage(void);
static void SYSM_ShowBasicInfo(void);
static void SYSM_OutPutDefaultCurrManage(void);
/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void SYSM_InitMemory( void )
{
	LIB_SetMemory( (uint8_t *)(&stSysM), 0u, (uint16_t)(sizeof(stSysM) / sizeof(uint8_t)));
}

/****************************************************************************************
 * Function Name  : void SYSM_InitZero( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System phase 0 initialization, kernel and clock initialization
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_InitZero(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();
	Mcal_Dma_Init();
	Mcal_Usart_Init();
}

/****************************************************************************************
 * Function Name  : void SYSM_InitOne( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System phase 1 initialization, memory initialization
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_InitOne(void)
{
	/* CycBuff Init*/
	CycBuf_InitMemory();
	CycBuf_Init();
	McalUsart_CycBuffCfgInit(); 
	Mcal_Can_RxCycBufCfg_Init();
}

/****************************************************************************************
 * Function Name  : void SYSM_InitTwo( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System phase 2 initialization, CAN communication initialization
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_InitTwo( void )
{
	/* System Init*/
	SYSM_InitMemory();
	/* STD_Library Init*/
	FIFO_InitMemory();
	/* Initialize the peripherals */
	MCALAPP_PeripheralInit();
	/* print user info */
	SYSM_ShowUserInfo();
	/* print reset source */
	Mcal_McuCheck_Rst();
	/* App init memory */
	ERRHDL_InitMemory(); 
	Comm_Init();
	ModbusM_Init();
	FanM_Init();
	SwitchM_Init();
	EVSEM_InitMemory();
	CPM_InitMemory();
	RELAYM_InitMemory();
	BTRM_InitMemory();
	AUTHM_InitMemory();
	NOAUTHEN_InitMemory();
	CURR_InitMemory();
	VOLT_InitMemory();
	SENSOR_InitMemory();
}

/****************************************************************************************
 * Function Name  : void SYSM_InitThree( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System phase 3 initialization, system loading and motor startup
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_InitThree(void)
{
	Mcal_Usart_IT_Enable();
	Mcal_Can_Enable();
	SwitchM_SoftTimerStart74hct4851d_Enable();
	Mcal_GpTime_AdcCollection_Start();
	Mcal_Adc_SoftTimer_Enable();
	ERRHDL_Enable();
	CURR_Enable();
	CPM_Enable();
	BTRM_Enable();
	VOLT_Enable();
}

static void SYSM_ShowUserInfo(void)
{
	LOG_TRACE(LOG_MODULE_SYSM, "(COM APP)Built@ " __DATE__" " __TIME__ "\n");
}

/****************************************************************************************
 * Function Name  : void SYSM_SleepShutdown(void)
 * Parameter      : void
 * Return Value   : void
 * Description    : Shutdown external devices before sleep
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_SleepShutdown(void)
{
	stSysM.ucLowPowerShutdownFlag = TRUE;
}

/****************************************************************************************
 * Function Name  : void SYSM_WakeupRestart( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : Initialization after wakeup
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_WakeupRestart( void )
{

}

/****************************************************************************************
 * Function Name  : uint8_t SYSM_GetResetPrepareStatus(void)
 * Parameter      : void
 * Return Value   : 0~1
 * Description    : Return reset preparation status, system must be in reset preparation status to work
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
uint8_t SYSM_GetResetPrepareStatus(void)
{
	uint8_t ReStatus = FALSE;

	if((stSysM.ucSysReady10msCnt > SYSM_RESET_PREPARE_MAX_CNT))
	{
		ReStatus = TRUE ;
	}
	else
	{
	}
	return (ReStatus);
}

/****************************************************************************************
 * Function Name  : void SYSM_SetSysStatusBit(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode)
 * Parameter      :
 * Return Value   :
 * Description    : Set specified status bit to 1 (Mode==1) or 0 (Mode==0)
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_SetSysStatusBit(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode)
{
	if (Mode) {
		stSysM.ulSystemStatus[ch] |=  (1 << SysStatusMask);
	}
	else {
		stSysM.ulSystemStatus[ch] &= ~(1 << SysStatusMask);
	}
}


/****************************************************************************************
 * Function Name  : void SYSM_GetSysStatusBit(uint32_t SysStatusMask, uint8_t Mode)
 * Parameter      :
 * Return Value   : 0: status bit is 0; 1: status bit is 1.
 * Description    : Return specified status bit status.
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
uint8_t SYSM_GetSysStatusBit(SysConnector_Num_Enum ch, uint32_t SysStatusMask)
{
	if (stSysM.ulSystemStatus[ch] & (1 << SysStatusMask)) {
		return 1;
	}
	else {
		return 0;
	}
}

/****************************************************************************************
 * Function Name  : uint32_t SYSM_GetSysStatus(SysConnector_Num_Enum ch)
 * Parameter      :
 * Return Value   :
 * Description    : Get system status word
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
uint32_t SYSM_GetSysStatus(SysConnector_Num_Enum ch)
{
	return stSysM.ulSystemStatus[ch];
}

/****************************************************************************************
 * Function Name  : uint8_t SYSM_CheckSysStatus(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode)
 * Parameter      :
 * Return Value   : If specified status bits are all 1 (Mode==1) or all 0 (Mode==0), return TRUE, otherwise return FALSE.
 * Description    : Check if specified status bits are all 1 (Mode==1) or all 0 (Mode==0)
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
uint8_t SYSM_CheckSysStatus(SysConnector_Num_Enum ch, uint32_t SysStatusMask, uint8_t Mode)
{
	uint8_t Ret = FALSE;

	if (Mode)
	{
		if ((stSysM.ulSystemStatus[ch] & SysStatusMask) == SysStatusMask)
		{
			Ret = TRUE;
		}
	}
	else
	{
		if ((stSysM.ulSystemStatus[ch] & SysStatusMask) == 0)
		{
			Ret = TRUE;
		}
	}

	return Ret;
}
/****************************************************************************************
 * Function Name  : void SYSM_ShutDownMCtrl( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System power-down management function
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_ShutDownMCtrl(void)
{

}

/****************************************************************************************
 * Function Name  : void SYSM_StandbyStatusCtrl( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System standby status management function
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
static void SYSM_StandbyStatusCtrl(void)
{

}

/****************************************************************************************
 * Function Name  : static void SYSM_RemoteResetManage(void)
 * Parameter      : void
 * Return Value   :
 * Description    : Manage asynchronous reset, trigger reset when conditions are met.
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
static void SYSM_RemoteResetManage(void)
{

}

/****************************************************************************************
 * Function Name  : static void SYSM_ShowBasicInfo(void)
 * Parameter      : void
 * Return Value   :
 * Description    : Print basic system information.
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
static void SYSM_ShowBasicInfo(void)
{
	if (!SYSM_GetResetPrepareStatus())
	{
		return;
	}

	uint8_t ucCpStatus[SYS_CONNECTOR_NUM_MAX] = {0};
	uint8_t ucEvseStatus[SYS_CONNECTOR_NUM_MAX] = {0};
	uint8_t lv_ucStopReson[SYS_CONNECTOR_NUM_MAX] = {0};

	for (SysConnector_Num_Enum i = SYS_CONNECTOR1; i < SYS_CONNECTOR_NUM_MAX; i++)
	{
		ucCpStatus[i] = CPM_GetCpVoltStatus(i);
		ucEvseStatus[i] = EVSEM_GetChargeStatus(i);
		lv_ucStopReson[i] = EVSEM_GetChargeStopReason(i);

		if ((stSysM.basic_ctrl_info.CpStatus[i] != ucCpStatus[i]) || (stSysM.basic_ctrl_info.EVSEStatus[i] != ucEvseStatus[i]))
		{
			stSysM.basic_ctrl_info.CpStatus[i] = ucCpStatus[i];
			stSysM.basic_ctrl_info.EVSEStatus[i] = ucEvseStatus[i];
			SYSM_INFO("Connecter:%d CP %d EVSE %d \r\n", i, stSysM.basic_ctrl_info.CpStatus[i], stSysM.basic_ctrl_info.EVSEStatus[i]);
		}

		if (stSysM.basic_ctrl_info.StopChargingReason[i] != lv_ucStopReson[i])
		{
			stSysM.basic_ctrl_info.StopChargingReason[i] = lv_ucStopReson[i];
			SYSM_INFO("Connecter:%d ChargingStopReason %d \r\n", i, stSysM.basic_ctrl_info.StopChargingReason[i]);
		}
	}
	if (stSysM.basic_ctrl_info.ul10msCnt < SYSM_REPORT_BASIC_INFO_CNT)
	{
		stSysM.basic_ctrl_info.ul10msCnt++;
	}
	else
	{
		stSysM.basic_ctrl_info.ul10msCnt = 0;
		SYSM_INFO("Connecter1 CP %d EVSE %d \r\n", stSysM.basic_ctrl_info.CpStatus[0], stSysM.basic_ctrl_info.EVSEStatus[0]);
		SYSM_INFO("Connector1 Front Temp value:%d Connector1 RearTemp value:%d\r\n", SENSOR_GetSensorAdcTempValue(SENSOR_CONNECTOR1_FRONT_TEMP_CH), SENSOR_GetSensorAdcTempValue(SENSOR_CONNECTOR1_REAR_TEMP_CH));
		SYSM_INFO("Oil1 InTemp value:%d Oil1 OutTemp value:%d\r\n\r\n", SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_INLET_TEMP_CH), SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_OUTLET_TEMP_CH));

		SYSM_INFO("Connecter2 CP %d EVSE %d \r\n", stSysM.basic_ctrl_info.CpStatus[1], stSysM.basic_ctrl_info.EVSEStatus[1]);
		SYSM_INFO("Connector2 Front Temp value:%d Connector2 RearTemp value:%d\r\n", SENSOR_GetSensorAdcTempValue(SENSOR_CONNECTOR1_FRONT_TEMP_CH), SENSOR_GetSensorAdcTempValue(SENSOR_CONNECTOR1_REAR_TEMP_CH));
		SYSM_INFO("Oil2 InTemp value:%d Oil2 OutTemp value:%d\r\n\r\n", SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_INLET_TEMP_CH), SENSOR_GetSensorAdcTempValue(SENSOR_OIL1_OUTLET_TEMP_CH));
	}
}

void SYSM_ImmediatelyResetManage(void)
{
	Mcal_MCU_SysRestart();
}

void SYSM_SetCpVolMode(SysConnector_Num_Enum ch, uint8_t mode)
{
	stSysM.ucCpOutMode[ch] = mode;
}

uint8_t SYSM_GetCpVolMode(SysConnector_Num_Enum ch)
{
	return (uint8_t)stSysM.ucCpOutMode[ch];
}

static void SYSM_OutPutDefaultCurrManage(void)
{
	static uint8_t stCpModeLast[SYS_CONNECTOR_NUM_MAX] = {0};

	for (SysConnector_Num_Enum i = SYS_CONNECTOR1; i < SYS_CONNECTOR_NUM_MAX; i++)
	{
		if (stCpModeLast[i] != SYSM_GetCpVolMode(i))
		{
			stCpModeLast[i] = SYSM_GetCpVolMode(i);
			if (SYSM_CP_MODE_4V == stCpModeLast[i])
			{
				SYSM_DEBUG("Connector:%d CP Mode 4V set default current to %d\r\n", i, CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL);
				CURR_SetDfltCurrVal(i, CURR_ONE_PHASE_CP4V_DFLT_CURR_VAL);
			}
			else if (SYSM_CP_MODE_12V == stCpModeLast[i])
			{
				SYSM_DEBUG("Connector:%d CP Mode 12V set default current to %d\r\n", i, CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL);
				CURR_SetDfltCurrVal(i, CURR_ONE_PHASE_CP12V_DFLT_CURR_VAL);
			}
			else
			{
			}
		}
	}
}

/****************************************************************************************
 * Function Name  : void SYSM_10msMainFunction( void )
 * Parameter      : void
 * Return Value   : void
 * Description    : System management 10ms periodic scheduling function
 * Edit Time      :
 * Remark         : Initial version
 *****************************************************************************************/
void SYSM_10msMainFunction(void)
{
	stSysM.ucSysReady10msCnt++;

	SYSM_StandbyStatusCtrl();

	SYSM_RemoteResetManage();

	// SYSM_ShowBasicInfo();

	SYSM_OutPutDefaultCurrManage();
}

void SYSM_RunningLedHandle(void)
{
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
/*EOF*/
