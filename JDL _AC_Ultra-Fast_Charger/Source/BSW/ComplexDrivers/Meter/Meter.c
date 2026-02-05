//******************************************************************************
//* File Name: Meter.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter.h"
#include "Meter_data.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/
enum
{
	METER_COMM_PENDING,
	METER_COMM_NORMAL,
	METER_COMM_WAITING
};
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct
{
	uint8_t comm_status;                         // communication status
	uint8_t actice_index;                        // active meter index
	uint16_t timer;								 // timer
	uint32_t last_comm_time;                     // last communication time
	MeterModule_t meter_info[METER_COUNT];		 // meter information
} Meter_Ctrl_t;
/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static Meter_Ctrl_t meter_ctrl = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void Meter_HandleIdleState(uint8_t meter_idx, uint32_t current_time);
static void Meter_HandleErrState(uint8_t meter_idx);
static McalRetVal_t Meter_ReadInfoHandle(uint8_t meter_idx, MeterReadState_t read_state, uint32_t current_time);
static void Meter_UpdateModuleAfterRequest(McalRetVal_t comm_status, uint8_t meter_idx, uint32_t current_time);
static void Meter_ProcessMeterState(uint8_t meter_idx, uint32_t current_time);
static void Meter_MeterModule_CheckTimeCalibration(uint8_t meter_idx);
static void Meter_HandleCommunicationError(uint8_t meter_idx);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void MeterModule_Init(void)
{
	Meter_DataInit();
	memset(&meter_ctrl, 0, sizeof(Meter_Ctrl_t));
	meter_ctrl.actice_index = 0;
	for (int i = 0; i < METER_COUNT; i++)
	{
		meter_ctrl.meter_info[i].read_state = METER_READ_IDLE;
		meter_ctrl.meter_info[i].next_read_state = METER_READ_CT_PT;
		meter_ctrl.meter_info[i].last_read_time = 0;
	}
}

static void Meter_HandleIdleState(uint8_t meter_idx, uint32_t current_time)
{
	MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];
	if (current_time - module->last_read_time >= METER_INFO_READ_INTERVAL)
	{
		module->read_state = module->next_read_state;
	}
}

static void Meter_HandleErrState(uint8_t meter_idx)
{
	MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];
	if (module->recover_count < METER_RECOVERY_COUNT)
	{
		module->recover_count++;
		meter_ctrl.actice_index = (meter_ctrl.actice_index + 1) % METER_COUNT;
	}
	else
	{
		module->error_flag = false;
		module->error_count = 0;
		module->recover_count = 0;
		module->read_state = METER_READ_IDLE;
		// METER_WARN("Meter %d recovered from error state\r\n", meter_idx);
	}
}

static McalRetVal_t Meter_ReadInfoHandle(uint8_t meter_idx, MeterReadState_t read_state, uint32_t current_time)
{
	McalRetVal_t comm_status = MCAL_RET_NOT_START_ERROR;
	switch (read_state)
	{
		case METER_READ_VOLTAGE:
			METER_INFO("Meter %d reading voltage\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_UAB, 12); // read 6 floats
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_CURRENT;
			break;

		case METER_READ_CURRENT:
			METER_INFO("Meter %d reading current\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_IA, 6); // read 3 floats
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_POWER;
			break;

		case METER_READ_POWER:
			METER_INFO("Meter %d reading power\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_PT_POWER, 16); // read 8 floats
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_POWER_FACTOR;
			break;

		case METER_READ_POWER_FACTOR:
			METER_INFO("Meter %d reading power factor\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_PFT, 8); // read 4 floats
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_FREQUENCY;
			break;

		case METER_READ_FREQUENCY:
			METER_INFO("Meter %d reading frequency\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_FREQ, 2); // read 1 float
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_ENERGY;
			break;

		case METER_READ_ENERGY:
			METER_INFO("Meter %d reading energy\r\n", meter_idx);
			// read energy in phases
			comm_status = Meter_SendReadRequest(meter_idx, REG_IMP_EP, 20); // read from ImpEp to ExpEpT4 10 registers (5 floats)
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_CT_PT;
			break;

		case METER_READ_CT_PT:
			METER_INFO("Meter %d reading CT and PT\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_CT, 2); // read CT and PT
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_VOLTAGE;
			break;

		case METER_READ_TIME:
			METER_INFO("Meter %d reading time\r\n", meter_idx);
			comm_status = Meter_SendReadRequest(meter_idx, REG_TIME_SECOND, 6); // read 6 time registers
			meter_ctrl.meter_info[meter_idx].next_read_state = METER_READ_VOLTAGE;
			break;
		default:
			METER_WARN("Meter %d invalid read state: %d\r\n", meter_idx, read_state);
			break;
	}
	return comm_status;
}

// update module state after sending request
static void Meter_UpdateModuleAfterRequest(McalRetVal_t comm_status, uint8_t meter_idx, uint32_t current_time)
{
	MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];

	if (comm_status == MCAL_RET_SUCCESS)
	{
		meter_ctrl.comm_status = METER_COMM_WAITING;
		meter_ctrl.last_comm_time = current_time;

		// update last read time	
		module->last_read_time = current_time;
	}
	else
	{
		module->error_count++;
		module->read_state = METER_READ_IDLE;

		METER_WARN("Meter %d send request failed: %d\r\n", meter_idx, comm_status);
	}
}

static void Meter_ProcessMeterState(uint8_t meter_idx, uint32_t current_time)
{
	MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];
	McalRetVal_t comm_status = MCAL_RET_NOT_START_ERROR;

	switch (module->read_state)
	{
		case METER_READ_IDLE:
			Meter_HandleIdleState(meter_idx, current_time);
			break;
		
		case METER_READ_ERROR:
			Meter_HandleErrState(meter_idx);
			break;

		default:
			if (meter_ctrl.comm_status != METER_COMM_WAITING && ModbusM_GetReadyStatus(MODBUS_CHANNEL_METER))
			{
				comm_status = Meter_ReadInfoHandle(meter_idx, module->read_state, current_time);
				Meter_UpdateModuleAfterRequest(comm_status, meter_idx, current_time);
			}
			break;
	}
}

// check communication timeout
static void Meter_CheckCommunicationTimeout(uint8_t meter_idx, uint32_t current_time)
{
    MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];
    
    if (meter_ctrl.comm_status == METER_COMM_WAITING)
    {
        if (current_time - meter_ctrl.last_comm_time > METER_COMM_TIMEOUT_TIME_MS)
        {
            module->error_count++;
            module->read_state = METER_READ_IDLE;
            
            METER_WARN("Meter %d communication timeout\r\n", meter_idx);
        }
    }
}

// check and perform time calibration
static void Meter_MeterModule_CheckTimeCalibration(uint8_t meter_idx)
{
	// for (int i = 0; i < METER_COUNT; i++)
	// {
	// 	// get system time
	// 	RtcTimedate_Struct sys_time;
	// 	RTCIF_GetDateTime(&sys_time);
	// }
}

// error handling function
static void Meter_HandleCommunicationError(uint8_t meter_idx)
{
	// TODO:
	MeterModule_t *module = &meter_ctrl.meter_info[meter_idx];
	if (module->error_count >= 3)
	{
		module->error_flag = true;
		module->read_state = METER_READ_ERROR;
		meter_ctrl.comm_status = METER_COMM_PENDING;
		METER_ERROR("Meter %d communication error, error count: %d\r\n", meter_idx, module->error_count);
	}
}

void MeterModule_Process(void)
{
	uint32_t current_time = METER_GET_CURRENT_TIMESTAMP();

	if (current_time < 3000)
	{
		return;
	}
	meter_ctrl.timer++;

	// machine state processing
	Meter_ProcessMeterState(meter_ctrl.actice_index, current_time);

	for (uint8_t index = 0; index < METER_COUNT; index++)
	{
		// check communication timeout
		Meter_CheckCommunicationTimeout(index, current_time);
		// check error
		Meter_HandleCommunicationError(index);
		// calibrate time
		Meter_MeterModule_CheckTimeCalibration(index);
	}

	// print meter data for debugging
	if (meter_ctrl.timer++ >= METER_PRINT_DATA_INTERVAL_MS)
	{
		meter_ctrl.timer = 0;
		for (uint8_t i = 0; i < METER_COUNT; i++)
		{
			if (meter_ctrl.meter_info[i].error_flag != 1)
			{
				Meter_PrintMeterDataInfo(i);
			}
		}
	}
}

void MeterModule_ErrHandleCallback(void)
{
	MeterModule_t *module = &meter_ctrl.meter_info[meter_ctrl.actice_index];
	module->error_flag = true;
	module->read_state = METER_READ_ERROR;
	meter_ctrl.comm_status = METER_COMM_PENDING;
	// METER_WARN("Meter %d communication error callback invoked\r\n", meter_ctrl.actice_index);
}

// Modbus response processing function
void MeterModule_HandleResponse(uint8_t addr, uint8_t *data, uint8_t datalen)
{
	uint8_t meter_idx = 255;

	meter_idx = Meter_GetMeterIdReferSlaveAddr(addr);

	if (meter_idx == 255)  
	{
		METER_WARN("<%s> Unknown meter address: 0x%02X", __func__, addr);
		return;
	}
	MeterModule_t *current = &meter_ctrl.meter_info[meter_idx];

	// update communication status
	meter_ctrl.comm_status = METER_COMM_NORMAL; // communicate normal
	meter_ctrl.last_comm_time = METER_GET_CURRENT_TIMESTAMP();
	// parse data according to read state
	if (data[0] != (datalen - 3)) // byte count + data + CRC(2 bytes)
	{
		METER_WARN("<%s> Data length mismatch for meter %d: expected %d, got %d", __func__, meter_idx, data[2], datalen - 5);
		current->error_count++;
		current->read_state = METER_READ_IDLE;
	}
	else
	{
		Meter_ParseMeasureData(meter_idx, &data[1], data[0], current->read_state);
		current->read_state = METER_READ_IDLE;
		meter_ctrl.actice_index = (meter_ctrl.actice_index + 1) % METER_COUNT;
	}
}
/* EOL */
