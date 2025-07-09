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
#include "BtrM.h"
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
	uint8_t CpStatus[SYS_CONNECTOR_NUM_MAX]; /* Cp状态 */
	uint8_t EVSEStatus[SYS_CONNECTOR_NUM_MAX]; /* EVSE状态 */
	uint8_t StopChargingReason[SYS_CONNECTOR_NUM_MAX]; /* 停止充电原因 */
	uint32_t ul10msCnt; /* 计数器 */
}SysM_BasicInfo_Struct;


typedef struct
{
	SysM_BasicInfo_Struct basic_ctrl_info; /* 基本控制信息 */
	uint8_t ucSysReady10msCnt;	   /* 系统就绪状态计数器 */
	uint8_t ucCpOutMode[SYS_CONNECTOR_NUM_MAX];           /* CP输出模式 */
	uint8_t ucLowPowerShutdownFlag;
	uint8_t ucLowPowerShutdownCnt;
	uint8_t usRemoteResetFlag; /* 0: No reset request; 1: Reset immediately; 2:Reset when the conditons are satisfied.*/
	uint32_t ulSystemStatus;   /* 系统状态字。位表示，位定义见STD_SysM_SysStatus_t，1：表示存在定义状态； 0：表示不存在。*/
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
 *函数名称  : void SYSM_InitZero( void )

 *参数      : void

 *返回值    : void

 *描述      : 系统0阶段初始化，系统内核、时钟初始化

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_InitZero(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize the SysTick */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/* Set the SysTick priority */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/****************************************************************************************
 *函数名称  : void SYSM_InitOne( void )

 *参数      : void

 *返回值    : void

 *描述      : 系统1阶段初始化，内存初始化

 *编辑时间  :

 *备注      : 初版
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
 *函数名称  : void SYSM_InitTwo( void )

 *参数      : void

 *返回值    : void

 *描述      :系统2阶段初始化，Can通信初始化

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_InitTwo( void )
{
	/* System Init*/
	SYSM_InitMemory();
	/* STD_Library Init*/
	FIFO_InitMemory();
	/* Initialize the peripherals */
	MCALAPP_PeripheralInit();
	/* Enable logging service */
	LogService_SetLogEnable();
	/* print user info */
	SYSM_ShowUserInfo();
	/* print reset source */
	Mcal_McuCheck_Rst();
	/* App init memery */
	ERRHDL_InitMemory(); 
	Comm_Init();
	SwitchM_Init();
	EVSEM_InitMemory();
	CPM_InitMemory();
	RELAYM_InitMemory();
	BTRM_InitMemory();
	AUTHM_InitMemory();
	NOAUTHEN_InitMemory();
	CURR_InitMemory();
}

/****************************************************************************************
 *函数名称  : void SYSM_InitThree( void )

 *参数      : void

 *返回值    : void

 *描述      :系统3阶段初始化，系统加载、电机加载启动

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_InitThree(void)
{
	Mcal_Usart_Enable();
	Mcal_Can_Enable();
	SwitchM_SoftTimerStart74hct4851d_Enable();
	Mcal_GpTime_AdcCollection_Start();
	Mcal_Adc_SoftTimer_Enable();
	ERRHDL_Enable();
	CURR_Enable();
	CPM_Enable();
	BTRM_Enable();
}

static void SYSM_ShowUserInfo(void)
{
	LOG_TRACE(LOG_MODULE_SYSM, "(COM APP)Built@ " __DATE__" " __TIME__ "\n");
}

/****************************************************************************************
 *函数名称  : void SYSM_SleepShutdown(void)

 *参数      : void

 *返回值    : void

 *描述      : 睡眠前关闭外部设备

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_SleepShutdown(void)
{
	stSysM.ucLowPowerShutdownFlag = TRUE;
}

/****************************************************************************************
 *函数名称  : void SYSM_WakeupRestart( void )

 *参数      : void

 *返回值    : void

 *描述      : 唤醒后初始化

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_WakeupRestart( void )
{

}

/****************************************************************************************
 *函数名称  : uint8_t SYSM_GetResetPrepareStatus(void)

 *参数      : void

 *返回值    : 0~1

 *描述      :返回复位准备状态，系统必须在复位准备状态
                     为真的情况下才可工作

 *编辑时间  :

 *备注      : 初版
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
 *函数名称  : void SYSM_SetSysStatusBit(uint32_t SysStatusMask, uint8_t Mode)

 *参数      :

 *返回值    :

 *描述      :设置指定状态位为1(Mode==1)或为0(Mode==0)

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_SetSysStatusBit(uint32_t SysStatusMask, uint8_t Mode)
{
	if (Mode) {
		stSysM.ulSystemStatus |= SysStatusMask;
	}
	else {
		stSysM.ulSystemStatus &= ~SysStatusMask;
	}
}


/****************************************************************************************
 *函数名称  : void SYSM_GetSysStatusBit(uint32_t SysStatusMask, uint8_t Mode)

 *参数      :

 *返回值    : 0：状态位为0；1：状态位为1。

 *描述      :返回指定状态位状态。

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
uint8_t SYSM_GetSysStatusBit(uint32_t SysStatusMask)
{
	if (stSysM.ulSystemStatus & SysStatusMask) {
		return 1;
	}
	else {
		return 0;
	}
}

/****************************************************************************************
 *函数名称  : uint32_t SYSM_GetSysStatus(void)

 *参数      :

 *返回值    :

 *描述      :获取系统状态字

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
uint32_t SYSM_GetSysStatus(void)
{
	return stSysM.ulSystemStatus;
}

/****************************************************************************************
 *函数名称  : uint8_t SYSM_CheckSysStatus(uint32_t SysStatusMask, uint8_t Mode)

 *参数      :

 *返回值    : 指定状态位是否全为1(Mode==1)或全为0(Mode==0)返回TRUE，否则返回FALSE.

 *描述      : 检测指定状态位是否全为1(Mode==1)或全为0(Mode==0)

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
uint8_t SYSM_CheckSysStatus(uint32_t SysStatusMask, uint8_t Mode)
{
	uint8_t Ret = FALSE;

	if (Mode)
	{
		if ((stSysM.ulSystemStatus & SysStatusMask) == SysStatusMask)
		{
			Ret = TRUE;
		}
	}
	else
	{
		if ((stSysM.ulSystemStatus & SysStatusMask) == 0)
		{
			Ret = TRUE;
		}
	}

	return Ret;
}
/****************************************************************************************
 *函数名称  : void SYSM_ShutDownMCtrl( void )

 *参数      : void

 *返回值    : void

 *描述      :系统掉电管理函数

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_ShutDownMCtrl(void)
{

}

/****************************************************************************************
 *函数名称  : void SYSM_StandbyStatusCtrl( void )

 *参数      : void

 *返回值     : void

 *描述      :系统待机状态管理函数

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
static void SYSM_StandbyStatusCtrl(void)
{

}

/****************************************************************************************
 *函数名称  : static void SYSM_RemoteResetManage(void)
 *参数      : void
 *返回值    :
 *描述      : 管理异步复位，在条件满足是触发复位。
 *编辑时间  :
 *备注      : 初版
 *****************************************************************************************/
static void SYSM_RemoteResetManage(void)
{

}

/****************************************************************************************
 *函数名称  : static void SYSM_ShowBasicInfo(void)
 *参数      : void
 *返回值    :
 *描述      : 打印系统基本信息。
 *编辑时间  :
 *备注      : 初版
 *****************************************************************************************/
static void SYSM_ShowBasicInfo(void)
{
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
			SYSM_DEBUG("Connecter:%d CP %d EVSE %d \r\n", i, stSysM.basic_ctrl_info.CpStatus[i], stSysM.basic_ctrl_info.EVSEStatus[i]);
		}
		else if (stSysM.basic_ctrl_info.ul10msCnt < SYSM_REPORT_BASIC_INFO_CNT)
		{
			stSysM.basic_ctrl_info.ul10msCnt++;
		}
		else
		{
			stSysM.basic_ctrl_info.ul10msCnt = 0;
			SYSM_DEBUG("Connecter:%d CP %d EVSE %d \r\n", i, stSysM.basic_ctrl_info.CpStatus[i], stSysM.basic_ctrl_info.EVSEStatus[i]);
		}

		if (stSysM.basic_ctrl_info.StopChargingReason[i] != lv_ucStopReson[i])
		{
			stSysM.basic_ctrl_info.StopChargingReason[i] = lv_ucStopReson[i];
			SYSM_DEBUG("Connecter:%d ChargingStopReason %d \r\n", i, stSysM.basic_ctrl_info.StopChargingReason[i]);
		}
	}
}

void SYSM_ImmediatelyResetManage(void)
{
	Mcal_MCU_SysRestart();
}

void SYSM_SendData(const uint8_t *data)
{
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
	HAL_UART_Transmit_IT(&huart2, (const uint8_t *)data, (uint16_t)strlen(data));
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
 *函数名称  : void SYSM_10msMainFunction( void )

 *参数      : void

 *返回值    : void

 *描述      :系统管理运行周期10ms调度函数

 *编辑时间  :

 *备注      : 初版
 *****************************************************************************************/
void SYSM_10msMainFunction(void)
{
	stSysM.ucSysReady10msCnt++;

	SYSM_StandbyStatusCtrl();

	SYSM_RemoteResetManage();

	SYSM_ShowBasicInfo();

	SYSM_OutPutDefaultCurrManage();
}
/*EOF*/
