//******************************************************************************
//* File Name: Meter_data.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module data header file
/*******************************************************************************/
#if !defined (__METER_DATA_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __METER_DATA_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_config.h"
#include "Meter_utils.h"
#include "STD_SysM_Cfg.h"
#include "Cloud_Protocol_Sg_ChargingOrder_Cfg.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

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
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/
extern void Meter_DataInit(void);
extern uint8_t Meter_GetMeterIdReferSlaveAddr(uint8_t slave_addr);
extern McalRetVal_t Meter_SendReadRequest(uint8_t meter_idx, uint16_t start_addr, uint16_t reg_count);
extern void Meter_ParseMeasureData(uint8_t meter_idx, uint8_t *data, uint8_t datalen, MeterReadState_t read_state);\
extern uint8_t Meter_GetMeterReadyStatus(SysConnector_Num_Enum ch);
extern uint8_t Meter_GetPowerDownStatus(void);
extern uint16_t Meter_GetPowerDownVoltValue(void);
extern uint16_t Meter_GetVoltL1(SysConnector_Num_Enum ch);
extern uint16_t Meter_GetVoltL2(SysConnector_Num_Enum ch);
extern uint16_t Meter_GetVoltL3(SysConnector_Num_Enum ch);
extern uint16_t Meter_GetCurrL1(SysConnector_Num_Enum ch);
extern uint16_t Meter_GetCurrL2(SysConnector_Num_Enum ch);
extern uint16_t Meter_GetCurrL3(SysConnector_Num_Enum ch);
extern char *Meter_GetMeterAddr(SysConnector_Num_Enum ch);
extern void Meter_GetMeterInfo(uint8_t gun_no, cloud_protocol_sg_order_measure_value_t *meter_info);
extern void Meter_GetTotalEnergy(uint8_t gun_no, cloud_protocol_sg_order_energy_t *total_energy, cloud_protocol_sg_order_op_t op);
extern uint32_t Meter_GetDeltaEnergyValue(uint8_t gun_no, cloud_protocol_sg_order_op_t op);
extern void Meter_PrintMeterDataInfo(uint8_t meter_idx);
#endif /* __METER_DATA_H */
/* EOL */
