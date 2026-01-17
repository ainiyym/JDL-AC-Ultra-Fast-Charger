//******************************************************************************
//* File Name: Meter_data.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_data.h"
#include "Meter_def.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

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

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
MeterData_t meter_data[METER_COUNT] = {0};

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void Meter_DataInit(void)
{
	for (int i = 0; i < METER_COUNT; i++)
	{
		meter_data[i].meter_id = i;
		meter_data[i].slave_addr = METER_1_ADDR + i;
		meter_data[i].measure.CT_ratio = DEFAULT_CT_VALUE;
		meter_data[i].measure.PT_ratio = DEFAULT_PT_VALUE;
	}
}

uint8_t Meter_GetMeterIdReferSlaveAddr(uint8_t slave_addr)
{
	for (int i = 0; i < METER_COUNT; i++)
	{
		if (meter_data[i].slave_addr == slave_addr)
		{
			return meter_data[i].meter_id;
		}
	}
	return 255; // not found
}

McalRetVal_t Meter_SendReadRequest(uint8_t meter_idx, uint16_t start_addr, uint16_t reg_count)
{
	MeterData_t *meter = &meter_data[meter_idx];
	uint8_t request_data[4];

	request_data[0] = (start_addr >> 8) & 0xFF;
	request_data[1] = start_addr & 0xFF;
	request_data[2] = (reg_count >> 8) & 0xFF;
	request_data[3] = reg_count & 0xFF;

	McalRetVal_t ret = Meter_SendModbusRequest(meter->slave_addr, MODBUS_CMD_READ_HOLDING_REGISTERS, request_data, 4);

	return ret;
}

void Meter_ParseMeasureData(uint8_t meter_idx, uint8_t *data, uint8_t datalen, MeterReadState_t read_state)
{
	MeterData_t *meter = &meter_data[meter_idx];

	switch (read_state)
	{
		case METER_READ_VOLTAGE:
			if (datalen >= 24)
			{ // 6 floats × 4 bytes
				meter->measure.raw_Uab = Meter_ParseFloat(&data[0]);
				meter->measure.raw_Ubc = Meter_ParseFloat(&data[4]);
				meter->measure.raw_Uca = Meter_ParseFloat(&data[8]);
				meter->measure.raw_Ua = Meter_ParseFloat(&data[12]);
				meter->measure.raw_Ub = Meter_ParseFloat(&data[16]);
				meter->measure.raw_Uc = Meter_ParseFloat(&data[20]);

				// mark voltage data valid
				meter->measure.data_valid |= 0x01;
			}
			break;

		case METER_READ_CURRENT:
			if (datalen >= 12)
			{ // 3 floats × 4 bytes
				meter->measure.raw_Ia = Meter_ParseFloat(&data[0]);
				meter->measure.raw_Ib = Meter_ParseFloat(&data[4]);
				meter->measure.raw_Ic = Meter_ParseFloat(&data[8]);

				// mark current data valid
				meter->measure.data_valid |= 0x02;
			}
			break;

		case METER_READ_POWER:
			if (datalen >= 32)
			{ // 8 floats × 4 bytes
				// Active power
				meter->measure.raw_Pt = Meter_ParseFloat(&data[0]);
				meter->measure.raw_Pa = Meter_ParseFloat(&data[4]);
				meter->measure.raw_Pb = Meter_ParseFloat(&data[8]);
				meter->measure.raw_Pc = Meter_ParseFloat(&data[12]);

				// Reactive power
				meter->measure.raw_Qt = Meter_ParseFloat(&data[16]);
				meter->measure.raw_Qa = Meter_ParseFloat(&data[20]);
				meter->measure.raw_Qb = Meter_ParseFloat(&data[24]);
				meter->measure.raw_Qc = Meter_ParseFloat(&data[28]);

				// mark power data valid
				meter->measure.data_valid |= 0x04;
				// convert raw data to actual values
				Meter_ConvertRawData(meter);
			}
			break;

		case METER_READ_POWER_FACTOR:
			if (datalen >= 16)
			{ // 4 floats × 4 bytes
				meter->measure.raw_PFt = Meter_ParseFloat(&data[0]);
				meter->measure.raw_PFa = Meter_ParseFloat(&data[4]);
				meter->measure.raw_PFb = Meter_ParseFloat(&data[8]);
				meter->measure.raw_PFc = Meter_ParseFloat(&data[12]);

				// mark power factor data valid
				meter->measure.data_valid |= 0x08;
				// convert power factor data
				Meter_CalculatePowerFactorData(meter);
			}
			break;

		case METER_READ_FREQUENCY:
			if (datalen >= 4)
			{ // 1 float × 4 bytes
				meter->measure.raw_Freq = Meter_ParseFloat(&data[0]);
				meter->measure.Freq = meter->measure.raw_Freq * 0.01f;

				// mark frequency data valid
				meter->measure.data_valid |= 0x10;
				METER_INFO("Meter %d Frequency Data: Freq=%.2fHz\r\n",
						   meter_idx,
						   meter->measure.Freq);
			}
			break;

		case METER_READ_ENERGY:
			// parse energy data
			if (datalen >= 40)
			{ // 10 floats × 4 bytes
				meter->energy.raw_ImpEp = Meter_ParseFloat(&data[0]);
				meter->energy.raw_ImpEpT1 = Meter_ParseFloat(&data[4]);
				meter->energy.raw_ImpEpT2 = Meter_ParseFloat(&data[8]);
				meter->energy.raw_ImpEpT3 = Meter_ParseFloat(&data[12]);
				meter->energy.raw_ImpEpT4 = Meter_ParseFloat(&data[16]);
				meter->energy.raw_ExpEp = Meter_ParseFloat(&data[20]);
				meter->energy.raw_ExpEpT1 = Meter_ParseFloat(&data[24]);
				meter->energy.raw_ExpEpT2 = Meter_ParseFloat(&data[28]);
				meter->energy.raw_ExpEpT3 = Meter_ParseFloat(&data[32]); 
				meter->energy.raw_ExpEpT4 = Meter_ParseFloat(&data[36]);
				// mark energy data valid
				meter->energy.data_valid |= 0x01;
				Meter_CalculatePowerData(meter);
			}
			break;

		case METER_READ_CT_PT:
			if (datalen >= 4)
			{ // 2 registers × 2 bytes
				uint16_t ct = (data[0] << 8) | data[1];
				uint16_t pt = (data[2] << 8) | data[3];

				meter->measure.CT_ratio = (ct > 0) ? ct : DEFAULT_CT_VALUE;
				meter->measure.PT_ratio = (pt > 0) ? pt : DEFAULT_PT_VALUE;
				meter->measure.data_valid |= 0x20;
				METER_INFO("Meter %d CT/PT Data: CT_ratio=%d, PT_ratio=%d\r\n",
					meter_idx,
					meter->measure.CT_ratio,
					meter->measure.PT_ratio);
			}
			break;

		case METER_READ_TIME:
			if (datalen >= 12)
			{ // 6 registers × 2 bytes
				meter->time.second = (data[0] << 8) | data[1];
				meter->time.minute = (data[2] << 8) | data[3];
				meter->time.hour = (data[4] << 8) | data[5];
				meter->time.day = (data[6] << 8) | data[7];
				meter->time.month = (data[8] << 8) | data[9];
				meter->time.year = (data[10] << 8) | data[11];
				METER_INFO("Meter %d Time Data: %04d-%02d-%02d %02d:%02d:%02d\r\n",
					meter_idx,
					meter->time.year,
					meter->time.month,
					meter->time.day,
					meter->time.hour,
					meter->time.minute,
					meter->time.second);
			}
			break;

		default:
			break;
	}
}

void Meter_PrintMeterDataInfo(uint8_t meter_idx)
{
	if (meter_idx >= METER_COUNT)
	{
		METER_WARN("Invalid meter index: %d\r\n", meter_idx);
		return;
	}
	Meter_PrintMeterData(&meter_data[meter_idx]);
}

char* Meter_GetMeterAddr(SysConnector_Num_Enum ch)
{
	static char addr_str[4] = {0};
    if (SYS_CONNECTOR1 == ch)
    {
		snprintf(addr_str, sizeof(addr_str), "%02X", meter_data[0].slave_addr);
		return addr_str;
    }
    else
    {
        snprintf(addr_str, sizeof(addr_str), "%02X", meter_data[1].slave_addr);
        return addr_str;
    }
}

uint8_t Meter_GetMeterReadyStatus(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (meter_data[0].measure.data_valid == 0x3F) ? 1 : 0;
		case SYS_CONNECTOR2:
			return (meter_data[1].measure.data_valid == 0x3F) ? 1 : 0;
		default:
			return 0;
	}
}

uint8_t Meter_GetPowerDownStatus(void)
{
	if (meter_data[0].measure.data_valid == 0x3F)
	{
		if (meter_data[0].measure.Ua < METER_POWER_DOWN_VOLTAGE_THRESHOLD)
		{
			return 1; // Power down detected for meter 1
		}
	}
	return 0; // No power down
}

uint16_t Meter_GetPowerDownVoltValue(void)
{
    static uint8_t step = 0;
    uint8_t Buffer[8] = {0x01, 0x03, 0x20, 0x06, 0x00, 0x02, 0x2F, 0xCA}; // Modbus data
	uint8_t RcvBuffer[13] = {0};
	uint16_t RcvLen = 0;
	uint16_t VoltValue = 0;
    switch (step)
    {
    case 0:
        Mcal_Usart_AppSendData(MCAL_USART4_CH, Buffer, 8);
        step ++;
        break;
    case 1:
		RcvLen = Mcal_Usart_AppReceiveData(MCAL_USART4_CH, RcvBuffer, 13);
		if (RcvLen == 13)
		{
			VoltValue = (RcvBuffer[2] << 8) | RcvBuffer[3];
			break;
		}
        break;
    default:
        break;
    }
	return VoltValue;
}

uint16_t Meter_GetVoltL1(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Ua);
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Ua);
		default:
			return 0;
	}
}
uint16_t Meter_GetVoltL2(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Ub);
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Ua);
		default:
			return 0;
	}
}

uint16_t Meter_GetVoltL3(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Uc);
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Uc);
		default:
			return 0;
	}
}

uint16_t Meter_GetCurrL1(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Ia) * 100;
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Ia) * 100;
		default:
			return 0;
	}
}

uint16_t Meter_GetCurrL2(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Ib) * 100;
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Ib) * 100;
		default:
			return 0;
	}
}

uint16_t Meter_GetCurrL3(SysConnector_Num_Enum ch)
{
	switch (ch)
	{
		case SYS_CONNECTOR1:
			return (uint16_t)(meter_data[0].measure.Ic) * 100;
		case SYS_CONNECTOR2:
			return (uint16_t)(meter_data[1].measure.Ic) * 100;
		default:
			return 0;
	}
}

void Meter_GetMeterInfo(uint8_t gun_no, cloud_protocol_sg_order_measure_value_t *meter_info)
{
    if (meter_info != NULL)
    {
		switch (gun_no)
		{
			case 1:
				meter_info->voltage[0] = (uint16_t)(meter_data[0].measure.Ua * 10); // 0.1V
				meter_info->voltage[1] = (uint16_t)(meter_data[0].measure.Ub * 10);
				meter_info->voltage[2] = (uint16_t)(meter_data[0].measure.Uc * 10);
				meter_info->current[0] = (uint16_t)(meter_data[0].measure.Ia * 10); // 0.1A
				meter_info->current[1] = (uint16_t)(meter_data[0].measure.Ib * 10);
				meter_info->current[2] = (uint16_t)(meter_data[0].measure.Ic * 10);
				meter_info->power[0] = (uint16_t)(meter_data[0].measure.Pt * 0.1f); // 0.01kW
				meter_info->power[1] = (uint16_t)(meter_data[0].measure.Pa * 0.1f);
				meter_info->power[2] = (uint16_t)(meter_data[0].measure.Pb * 0.1f);
				meter_info->power[3] = (uint16_t)(meter_data[0].measure.Pc * 0.1f);
				break;
			case 2:
				meter_info->voltage[0] = (uint16_t)(meter_data[1].measure.Ua * 10); // 0.1V
				meter_info->voltage[1] = (uint16_t)(meter_data[1].measure.Ub * 10);
				meter_info->voltage[2] = (uint16_t)(meter_data[1].measure.Uc * 10);
				meter_info->current[0] = (uint16_t)(meter_data[1].measure.Ia * 10); // 0.1A
				meter_info->current[1] = (uint16_t)(meter_data[1].measure.Ib * 10);
				meter_info->current[2] = (uint16_t)(meter_data[1].measure.Ic * 10);
				meter_info->power[0] = (uint16_t)(meter_data[1].measure.Pt * 0.1f); // 0.01kW
				meter_info->power[1] = (uint16_t)(meter_data[1].measure.Pa * 0.1f);
				meter_info->power[2] = (uint16_t)(meter_data[1].measure.Pb * 0.1f);
				meter_info->power[3] = (uint16_t)(meter_data[1].measure.Pc * 0.1f);
				break;
			default:
				METER_WARN("Invalid gun number: %d\r\n", gun_no);
				break;
		}
    }
}

void Meter_GetTotalEnergy(uint8_t gun_no, cloud_protocol_sg_order_energy_t *total_energy, cloud_protocol_sg_order_op_t op)
{
	if (total_energy != NULL)
	{
		switch (gun_no)
		{
			case 1:
				if (op == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
				{
					total_energy->total = (uint32_t)(meter_data[0].energy.ImpEp * 100); // 0.1KWh
					total_energy->peak = (uint32_t)(meter_data[0].energy.ImpEpT1 * 100); // 0.1KWh
					total_energy->hight = (uint32_t)(meter_data[0].energy.ImpEpT2 * 100); // 0.1KWh
					total_energy->normal = (uint32_t)(meter_data[0].energy.ImpEpT3 * 100); // 0.1KWh
					total_energy->valley = (uint32_t)(meter_data[0].energy.ImpEpT4 * 100); // 0.1KWh
				}
				else
				{
					total_energy->total = (uint32_t)(meter_data[0].energy.ExpEp * 100); // 0.1KWH
					total_energy->peak = (uint32_t)(meter_data[0].energy.ExpEpT1 * 100); // 0.1KWh
					total_energy->hight = (uint32_t)(meter_data[0].energy.ExpEpT2 * 100); // 0.1KWh
					total_energy->normal = (uint32_t)(meter_data[0].energy.ExpEpT3 * 100); // 0.1KWh
					total_energy->valley = (uint32_t)(meter_data[0].energy.ExpEpT4 * 100); // 0.1KWh
				}
				break;
			case 2:
				if (op == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
				{
					total_energy->total = (uint32_t)(meter_data[1].energy.ImpEp * 100); // 0.1KWh
					total_energy->peak = (uint32_t)(meter_data[1].energy.ImpEpT1 * 100); // 0.1KWh
					total_energy->hight = (uint32_t)(meter_data[1].energy.ImpEpT2 * 100); // 0.1KWh
					total_energy->normal = (uint32_t)(meter_data[1].energy.ImpEpT3 * 100); // 0.1KWh
					total_energy->valley = (uint32_t)(meter_data[1].energy.ImpEpT4 * 100); // 0.1KWh
				}
				else
				{
					total_energy->total = (uint32_t)(meter_data[1].energy.ExpEp * 100); // 0.1KWH
					total_energy->peak = (uint32_t)(meter_data[1].energy.ExpEpT1 * 100); // 0.1KWh
					total_energy->hight = (uint32_t)(meter_data[1].energy.ExpEpT2 * 100); // 0.1KWh
					total_energy->normal = (uint32_t)(meter_data[1].energy.ExpEpT3 * 100); // 0.1KWh
					total_energy->valley = (uint32_t)(meter_data[1].energy.ExpEpT4 * 100); // 0.1KWh
				}
				break;
			default:
				METER_WARN("Invalid gun number: %d\r\n", gun_no);
				break;
		}
	}
}

uint32_t Meter_GetDeltaEnergyValue(uint8_t gun_no, cloud_protocol_sg_order_op_t op)
{
	uint32_t delta_energy = 0;
	switch (gun_no)
	{
		case 1:
			if (op == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
			{
				if (meter_data[0].energy.last_ImpEp != meter_data[0].energy.ImpEp)
				{
					meter_data[0].energy.energy_increment_impEp = meter_data[0].energy.ImpEp - meter_data[0].energy.last_ImpEp;
					meter_data[0].energy.last_ImpEp = meter_data[0].energy.ImpEp;
				}
				else
				{
					meter_data[0].energy.energy_increment_impEp = 0.0f;
				}
				delta_energy = (uint32_t)(meter_data[0].energy.energy_increment_impEp * 100); // 0.1KWh
			}
			else
			{
				if (meter_data[0].energy.last_ExpEp != meter_data[0].energy.ExpEp)
				{
					meter_data[0].energy.energy_increment_ExpEp = meter_data[0].energy.ExpEp - meter_data[0].energy.last_ExpEp;
					meter_data[0].energy.last_ExpEp = meter_data[0].energy.ExpEp;
				}
				else
				{
					meter_data[0].energy.energy_increment_ExpEp = 0.0f;
				}
				delta_energy = (uint32_t)(meter_data[0].energy.energy_increment_ExpEp * 100); // 0.1KWh
			}
			break;
		case 2:
			if (op == CLOUD_PROTOCOL_SG_ORDER_OP_CHARGE)
			{
				if (meter_data[1].energy.last_ImpEp != meter_data[1].energy.ImpEp)
				{
					meter_data[1].energy.energy_increment_impEp = meter_data[1].energy.ImpEp - meter_data[1].energy.last_ImpEp;
					meter_data[1].energy.last_ImpEp = meter_data[1].energy.ImpEp;
				}
				else
				{
					meter_data[1].energy.energy_increment_impEp = 0.0f;
				}
				delta_energy = (uint32_t)(meter_data[1].energy.energy_increment_impEp * 100); // 0.1KWh
			}
			else
			{
				if (meter_data[1].energy.last_ExpEp != meter_data[1].energy.ExpEp)
				{
					meter_data[1].energy.energy_increment_ExpEp = meter_data[1].energy.ExpEp - meter_data[1].energy.last_ExpEp;
					meter_data[1].energy.last_ExpEp = meter_data[1].energy.ExpEp;
				}
				else
				{
					meter_data[1].energy.energy_increment_ExpEp = 0.0f;
				}
				delta_energy = (uint32_t)(meter_data[1].energy.energy_increment_ExpEp * 100); // 0.1KWh
				
			}
			break;
		default:
			METER_WARN("Invalid gun number: %d\r\n", gun_no);
			break;
	}
	return delta_energy;
}
/* EOL */
