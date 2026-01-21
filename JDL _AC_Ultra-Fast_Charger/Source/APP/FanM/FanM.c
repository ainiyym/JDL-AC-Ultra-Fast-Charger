//******************************************************************************
//*
//* File Name: FanM.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FanM.h"
#include "FanM_Drv.h"
#include "FanM_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
/* 模块内部状态 */
typedef struct
{
	FanM_Status_t current_status; /* 当前状态 */
	uint8_t last_read_time;		  /* 上次读取时间 */
	uint8_t last_control_time;	  /* 上次控制时间 */
	uint8_t last_store_time;	  /* 上次存储时间 */
	uint8_t busy_time;		  	  /* 忙碌时间 */
	bool is_error;				  /* 故障状态 */
	bool is_charging;			  /* 充电状态 */
	bool is_control_busy;		  /* 命令待应答 */
	uint8_t pending_cmd;		  /* 待发送的命令 */
} FanM_Ctrl_t;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/
 
/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static FanM_Ctrl_t fan_m;

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static bool FanM_StoreCurrentStatus(void);
static bool FanM_CheckLiquidTempTooHigh(uint8_t temp_high_fault);
static bool FanM_CheckLiquidLevelVeryLow(uint8_t sensor_fault2, uint8_t liquid_level);
static void FanM_CheckAndUpdateFaults(void);
static void FanM_ParseStatusData(const uint8_t *data, uint8_t len);
static void FanM_HandleControlResponse(const uint8_t *data, uint8_t len);
static bool FanM_IsCharging(void);
static bool FanM_IsBusyStatus(void);
static void FanM_ReadStatusProcess(void);
static void FanM_CtrlRunningProcess(void);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
/* 模块初始化 */
void FanM_Init(void)
{    
    memset(&fan_m, 0, sizeof(fan_m));
    fan_m.is_charging = false;
    fan_m.is_control_busy = false;
    
    /* 设置初始状态 */
	fan_m.last_control_time = FANM_CONTROL_INTERVAL_MS;
	fan_m.last_read_time = FANM_READ_INTERVAL_MS;
	fan_m.last_store_time = FANM_STORE_INTERVAL_MS;
	fan_m.busy_time = 0;
    fan_m.current_status.run_mode = FANM_MODE_STANDBY;
    fan_m.current_status.fault_severity = FANM_FAULT_NONE;
}

void FanM_Deinit(void)
{    
    /* 发送停止命令 */
    uint8_t control_data[15];
    uint8_t data_len;
    FanM_BuildControlCommand(FANM_CMD_STOP, 0, 0, 0, control_data, &data_len);
    FANM_SEND_MODBUS(FANM_MODBUS_ADDR, FANM_MODBUS_CMD_WRITE_MULTIPLE_REGISTERS, control_data, data_len);  
    FANM_DEBUG("<%s>\r\n", __func__);
}

static bool FanM_IsCharging(void)
{
	if (fan_m.is_error)
	{
		return false; /* 未充电 */
	}

	if (FANM_GET_CHARGER_STATUS())
	{
		return true; /* 充电中 */
	}
	else
	{
		return false; /* 默认返回未充电 */
	}
}

/* 状态获取接口 */
const FanM_Status_t* FanM_GetCurrentStatus(void)
{
    return &fan_m.current_status;
}

/* 历史数据接口 */
static bool FanM_StoreCurrentStatus(void)
{
	FanM_HistoryRecord_t record;

	fan_m.last_store_time = FANM_STORE_INTERVAL_MS;
	/* 存储记录 */
	return FanM_Storage_Write(&record);
}

/* 检测温度过高故障 */
static bool FanM_CheckLiquidTempTooHigh(uint8_t temp_high_fault)
{
    /* 检查第9字节的温度过高故障位
       bit0: 回液温度1过高
       bit1: 回液温度1极高
       bit2: 回液温度2过高
       bit3: 回液温度2极高
       bit4: 出液温度1过高
       bit5: 出液温度1极高
       bit6: 出液温度2过高
       bit7: 出液温度2极高
       
       只要有一个温度传感器出现过热，就需要降低充电电流
    */
    return (temp_high_fault != 0);
}

/* 检测液位极低故障 */
static bool FanM_CheckLiquidLevelVeryLow(uint8_t sensor_fault2, uint8_t liquid_level)
{
    /* 检查第8字节bit2: 液位极低故障 */
    bool fault_bit = (sensor_fault2 & FANM_FAULT_BIT_LIQUID_LEVEL_VERY_LOW) != 0;
    
    /* 同时检查液位值是否为0(极低) */
    bool level_value = (liquid_level == FANM_LIQUID_LEVEL_VERY_LOW);
    
    return (fault_bit || level_value);
}

/* 检查故障条件并更新故障列表 */
static void FanM_CheckAndUpdateFaults(void)
{
	/* 检查故障严重度 */
	uint8_t severity = fan_m.current_status.fault_severity;

	/* 检查温度过高故障 */
	bool temp_high = FanM_CheckLiquidTempTooHigh(fan_m.current_status.temp_high_fault);

	/* 检查液位极低故障 */
	bool level_low = FanM_CheckLiquidLevelVeryLow(fan_m.current_status.sensor_fault2, fan_m.current_status.liquid_level);

	/* 故障消抖处理 */
	// 略

	/* 更新故障列表 */
	if (severity == FANM_FAULT_IMMEDIATE_STOP)
	{
		fan_m.is_error = true;
		// AddFault(FANM_FAULT_TYPE_SEVERITY_30, FANM_ACTION_STOP_CHARGING,
		// 		 severity, "立即停机故障：需要立即停止充电");
	}
	else if (severity == FANM_FAULT_URGENT_REPAIR)
	{
		fan_m.is_error = true;
		// AddFault(FANM_FAULT_TYPE_SEVERITY_20, FANM_ACTION_URGENT_MAINTENANCE,
		// 		 severity, "尽快检修故障：短期仍可使用，但需尽快检修");
	}
	else
	{
		fan_m.is_error = false;
		// RemoveFault(FANM_FAULT_TYPE_SEVERITY_30);
		// RemoveFault(FANM_FAULT_TYPE_SEVERITY_20);
	}

	if (temp_high)
	{
		// AddFault(FANM_FAULT_TYPE_LIQUID_TEMP_HIGH, FANM_ACTION_REDUCE_CHARGING_CURRENT,
		// 		 0, "液温过高：制冷能力不足，需要降低充电电流");
	}
	else
	{
		// RemoveFault(FANM_FAULT_TYPE_LIQUID_TEMP_HIGH);
	}

	if (level_low)
	{
		// AddFault(FANM_FAULT_TYPE_LIQUID_LEVEL_VERY_LOW, FANM_ACTION_STOP_CHARGING,
		// 		 0, "液位极低：空气进入管路，需要立即停止充电并进行加液排气");
	}
	else
	{
		// RemoveFault(FANM_FAULT_TYPE_LIQUID_LEVEL_VERY_LOW);
	}
}

/* 状态数据解析 */
static void FanM_ParseStatusData(const uint8_t *data, uint8_t len)
{
	fan_m.is_control_busy = false;

	if (data == NULL || len < 50)
	{
		FANM_ERROR("<%s> Invalid data\r\n", __func__);
		return;
	}
	FANM_DEBUG("<%s> Parsed status data\r\n", __func__);

	FanM_Status_t *status = &fan_m.current_status;

	/* 1-2字节: 运行模式和故障严重度 */
	status->run_mode = (FanM_RunMode_t)data[0];
	status->fault_severity = data[1];

	/* 3-10字节: 故障信息 */
	status->pump_fault_map = data[3];
	status->pump_fault.pump_fault1.value = data[4];
	status->pump_fault.pump_fault2.value = data[5];
	status->sensor_fault1 = data[6];
	status->sensor_fault2 = data[7];
	status->temp_high_fault = data[8];
	status->pressure_pipe_fault = data[9];

	/* 11-14字节: 产品独有故障（预留） */
	for (int i = 0; i < 4; i++)
	{
		status->reserved_fault[i] = data[10 + i];
	}

	/* 15字节: 液位值 */
	status->liquid_level = data[14];
	status->liquid_level_valid = (data[14] != FANM_VALUE_NO_HARDWARE);

	/* 16字节: 流量开关状态 */
	status->flow_switch_state = data[15];

	/* 17-22字节: 风扇和泵转速 */
	for (int i = 0; i < 4; i++)
	{
		status->fan_speed[i] = data[16 + i];
	}
	status->pump_speed[0] = data[20];
	status->pump_speed[1] = data[21];

	/* 23-26字节: 温度值 */
	for (int i = 0; i < 2; i++)
	{
		uint8_t temp_byte = data[22 + i];
		if (temp_byte == FANM_VALUE_NO_HARDWARE || temp_byte == FANM_VALUE_SENSOR_FAULT)
		{
			status->out_temp_valid[i] = false;
			status->out_temp[i] = 0.0f;
		}
		else
		{
			status->out_temp_valid[i] = true;
			status->out_temp[i] = (float)((int8_t)temp_byte - FANM_TEMP_OFFSET);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		uint8_t temp_byte = data[24 + i];
		if (temp_byte == FANM_VALUE_NO_HARDWARE || temp_byte == FANM_VALUE_SENSOR_FAULT)
		{
			status->return_temp_valid[i] = false;
			status->return_temp[i] = 0.0f;
		}
		else
		{
			status->return_temp_valid[i] = true;
			status->return_temp[i] = (float)((int8_t)temp_byte - FANM_TEMP_OFFSET);
		}
	}

	/* 27-28字节: 压力值 */
	for (int i = 0; i < 2; i++)
	{
		uint8_t pressure_byte = data[26 + i];
		if (pressure_byte == FANM_VALUE_NO_HARDWARE)
		{
			status->pressure_valid[i] = false;
			status->pressure[i] = 0.0f;
		}
		else
		{
			status->pressure_valid[i] = true;
			status->pressure[i] = (float)pressure_byte * FANM_PRESSURE_SCALE;
		}
	}

	/* 29字节: 供电电压 */
	status->supply_voltage = (float)data[28] / FANM_VOLTAGE_SCALE_DIV;

#if FANM_FAULT_HANDLING_ENABLED
    FanM_CheckAndUpdateFaults();
#endif
}

/* 处理控制响应 */
static void FanM_HandleControlResponse(const uint8_t *data, uint8_t len)
{
	fan_m.is_control_busy = false;

	if (data == NULL || len < 4)
	{
		FANM_ERROR("<%s> Invalid control response data\r\n", __func__);
		return;
	}
	FANM_DEBUG("<%s> Parsed control response data\r\n", __func__);

	/* 控制响应格式: 起始地址(2字节) + 寄存器数量(2字节) */
	uint16_t start_addr = (data[0] << 8) | data[1];
	uint16_t reg_count = (data[2] << 8) | data[3];

	if (start_addr == FANM_REG_CONTROL_START_ADDR && reg_count == FANM_REG_CONTROL_COUNT)
	{
		/* 控制命令执行成功 */
	}
	else
	{
		/* 响应格式错误 */
		FANM_ERROR("<%s> Control response format error\r\n", __func__);
	}
}

/* Modbus响应处理接口 */
void FanM_HandleModbusResponse(uint8_t addr, uint8_t cmd, uint8_t *data, uint8_t data_len)
{
	if (addr != FANM_MODBUS_ADDR)
	{
		return; /* 不是发给我们的响应 */
	}

	switch (cmd)
	{
	case FANM_MODBUS_CMD_READ_INPUT_REGISTERS:
		/* 状态读取响应 */
		if (data_len >= 1 && data[0] == 50)
		{
			/* 第一个字节是数据长度，后面是50字节数据 */
			FanM_ParseStatusData(data + 1, data_len - 1);
		}
		break;

	case FANM_MODBUS_CMD_WRITE_MULTIPLE_REGISTERS:
		/* 控制命令响应 */
		FanM_HandleControlResponse(data, data_len);
		break;

	default:
		break;
	}
}

static bool FanM_IsBusyStatus(void)
{
	if (ModbusM_GetReadyStatus(FANM_MODBUS_CHANNEL) == false)
	{
		return true; /* Modbus通道忙碌，跳过状态读取 */
	}
	else
	{
		if (fan_m.is_control_busy)
		{
			fan_m.busy_time++;
			if (fan_m.busy_time >= FANM_CONTROL_BUSY_TIME)
			{
				/* 忙碌超时，重置状态 */
				fan_m.is_control_busy = false;
				fan_m.busy_time = 0;
				return false;
			}
			else
			{
				return true; /* 正在等待控制命令响应，跳过状态读取 */
			}
		}
	}
}

static void FanM_ReadStatusProcess(void)
{
	if (FanM_IsBusyStatus())
	{
		return; /* 正在等待控制命令响应，跳过状态读取 */
	}
	else
	{
		uint8_t read_data[4];
		uint8_t data_len;
		FanM_BuildStatusReadRequest(read_data, &data_len);
		FANM_SEND_MODBUS(FANM_MODBUS_ADDR, FANM_MODBUS_CMD_READ_INPUT_REGISTERS, read_data, data_len);
		fan_m.last_read_time = FANM_READ_INTERVAL_MS;
		fan_m.is_control_busy = true;
		FANM_DEBUG("<%s> Sent status read request\r\n", __func__);
	}
}

static void FanM_CtrlRunningProcess(void)
{
	if (FanM_IsBusyStatus())
	{
		/* 正在等待控制命令响应 */
		return;
	}
	else
	{
		bool charging = FanM_IsCharging();
		if (charging != fan_m.is_charging)
		{
			fan_m.is_charging = charging;
		}
		uint8_t cmd = charging ? FANM_CMD_AUTO_START : FANM_CMD_STOP;
		FanM_SendControlCommand(cmd);
		fan_m.last_control_time = FANM_CONTROL_INTERVAL_MS;
		fan_m.is_control_busy = true;
		FANM_DEBUG("<%s> Sent control command: %s\r\n", __func__, charging ? "AUTO_START" : "STOP");
	}
}

/* 主任务函数 */
void FanM_Task_100ms(void)
{
#if FANM_READ_STATUS_ENABLED
	/* 1秒读取一次状态 */
	if (0 != fan_m.last_read_time)
	{
		fan_m.last_read_time--;
	}
	else
	{
		FanM_ReadStatusProcess();
	}
#endif

#if FANM_CONTROL_ENABLED
	/* 2秒发送一次控制命令（如果不是待发送状态） */
	if (0 != fan_m.last_control_time)
	{
		fan_m.last_control_time--;
	}
	else
	{
		FanM_CtrlRunningProcess();
	}
#endif

#if FANM_HISTORY_STORE_ENABLED
	/* 10秒存储一次历史数据 */
	if (0 != fan_m.last_store_time)
	{
		fan_m.last_store_time--;
	}
	else
	{
		FanM_StoreCurrentStatus();
	}
#endif
}
/* EOL */
