//******************************************************************************
//* File Name: Meter_utils.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Meter module utility functions source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Meter_utils.h"

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

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static float Meter_CalculateApparentPower(float P, float Q);

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// Calculate apparent power
static float Meter_CalculateApparentPower(float P, float Q)
{
	return sqrtf(P * P + Q * Q);
}

// Floating-point number parsing (assuming the IEEE 754 standard)
float Meter_ParseFloat(uint8_t *data)
{
	union
	{
		uint32_t i;
		float f;
	} converter;

	converter.i = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
	return converter.f;
}

// Convert the original data into actual values
void Meter_ConvertRawData(MeterData_t *meter)
{
	// U = URMSx × (UrAt * 0.1) × 0.1 (V)
	float pt_factor = meter->measure.PT_ratio * 0.1f * 0.1f;
	meter->measure.Uab = meter->measure.raw_Uab * pt_factor;
	meter->measure.Ubc = meter->measure.raw_Ubc * pt_factor;
	meter->measure.Uca = meter->measure.raw_Uca * pt_factor;
	meter->measure.Ua = meter->measure.raw_Ua * pt_factor;
	meter->measure.Ub = meter->measure.raw_Ub * pt_factor;
	meter->measure.Uc = meter->measure.raw_Uc * pt_factor;

	// I = IRMSx × IrAt × 0.001 (A)
	float ct_factor = meter->measure.CT_ratio * 0.001f;
	meter->measure.Ia = meter->measure.raw_Ia * ct_factor;
	meter->measure.Ib = meter->measure.raw_Ib * ct_factor;
	meter->measure.Ic = meter->measure.raw_Ic * ct_factor;

	// P = Px × (UrAt * 0.1) × IrAt × 0.1 (W)
	float power_factor = meter->measure.PT_ratio * 0.1f * meter->measure.CT_ratio * 0.1f;
	meter->measure.Pt = meter->measure.raw_Pt * power_factor;
	meter->measure.Pa = meter->measure.raw_Pa * power_factor;
	meter->measure.Pb = meter->measure.raw_Pb * power_factor;
	meter->measure.Pc = meter->measure.raw_Pc * power_factor;

	// Q = Q × UrAt × IrAt × 0.1 (VAR)
	meter->measure.Qt = meter->measure.raw_Qt * power_factor;
	meter->measure.Qa = meter->measure.raw_Qa * power_factor;
	meter->measure.Qb = meter->measure.raw_Qb * power_factor;
	meter->measure.Qc = meter->measure.raw_Qc * power_factor;

	// Calculate apparent power
	meter->measure.St = Meter_CalculateApparentPower(meter->measure.Pt, meter->measure.Qt);
	meter->measure.Sa = Meter_CalculateApparentPower(meter->measure.Pa, meter->measure.Qa);
	meter->measure.Sb = Meter_CalculateApparentPower(meter->measure.Pb, meter->measure.Qb);
	meter->measure.Sc = Meter_CalculateApparentPower(meter->measure.Pc, meter->measure.Qc);
}

// Calculate power factor data
void Meter_CalculatePowerFactorData(MeterData_t *meter)
{
	// PF = PFx × 0.001
	meter->measure.PFt = meter->measure.raw_PFt * 0.001f;
	meter->measure.PFa = meter->measure.raw_PFa * 0.001f;
	meter->measure.PFb = meter->measure.raw_PFb * 0.001f;
	meter->measure.PFc = meter->measure.raw_PFc * 0.001f;

	// Determine whether it is resistive or capacitive
	meter->measure.PFt_type = (meter->measure.PFt > 0) ? PF_INDUCTIVE : (meter->measure.PFt < 0) ? PF_CAPACITIVE
																								 : PF_UNKNOWN;
	meter->measure.PFa_type = (meter->measure.PFa > 0) ? PF_INDUCTIVE : (meter->measure.PFa < 0) ? PF_CAPACITIVE
																								 : PF_UNKNOWN;
	meter->measure.PFb_type = (meter->measure.PFb > 0) ? PF_INDUCTIVE : (meter->measure.PFb < 0) ? PF_CAPACITIVE
																								 : PF_UNKNOWN;
	meter->measure.PFc_type = (meter->measure.PFc > 0) ? PF_INDUCTIVE : (meter->measure.PFc < 0) ? PF_CAPACITIVE
																								 : PF_UNKNOWN;

	// absolute value display
	if (DISPLAY_ABS_POWER_FACTOR)
	{
		meter->measure.PFt = fabsf(meter->measure.PFt);
		meter->measure.PFa = fabsf(meter->measure.PFa);
		meter->measure.PFb = fabsf(meter->measure.PFb);
		meter->measure.PFc = fabsf(meter->measure.PFc);
	}
}

// Calculate power data
void Meter_CalculatePowerData(MeterData_t *meter)
{
	// Ep = E× (UrAt×0.1)×IrAt (kvw)
	float energy_factor = meter->measure.PT_ratio * 0.1f * meter->measure.CT_ratio;
	meter->energy.ImpEp = meter->energy.raw_ImpEp * energy_factor;
	meter->energy.ImpEpT1 = meter->energy.raw_ImpEpT1 * energy_factor;
	meter->energy.ImpEpT2 = meter->energy.raw_ImpEpT2 * energy_factor;
	meter->energy.ImpEpT3 = meter->energy.raw_ImpEpT3 * energy_factor;
	meter->energy.ImpEpT4 = meter->energy.raw_ImpEpT4 * energy_factor;
	meter->energy.ExpEp = meter->energy.raw_ExpEp * energy_factor;
	meter->energy.ExpEpT1 = meter->energy.raw_ExpEpT1 * energy_factor;
	meter->energy.ExpEpT2 = meter->energy.raw_ExpEpT2 * energy_factor;
	meter->energy.ExpEpT3 = meter->energy.raw_ExpEpT3 * energy_factor;
	meter->energy.ExpEpT4 = meter->energy.raw_ExpEpT4 * energy_factor;
}

void Meter_PrintMeterData(MeterData_t *meter)
{
	METER_DEBUG("Meter %d CT_ratio=%2d, PT_ratio=%2d \r\n", meter->meter_id, meter->measure.CT_ratio, meter->measure.PT_ratio);
	METER_DEBUG("Meter %d Converted Data: Uab=%.2fV, Ubc=%.2fV, Uca=%.2fV, Ua=%.2fV, Ub=%.2fV, Uc=%.2fV, Ia=%.3fA, Ib=%.3fA, Ic=%.3fA\r\n",
			   meter->meter_id,
			   meter->measure.Uab,
			   meter->measure.Ubc,
			   meter->measure.Uca,
			   meter->measure.Ua,
			   meter->measure.Ub,
			   meter->measure.Uc,
			   meter->measure.Ia,
			   meter->measure.Ib,
			   meter->measure.Ic);
	METER_DEBUG("Meter %d Power Data: Pt=%.2fW, Pa=%.2fW, Pb=%.2fW, Pc=%.2fW, Qt=%.2fVAR, Qa=%.2fVAR, Qb=%.2fVAR, Qc=%.2fVAR, St=%.2fVA, Sa=%.2fVA, Sb=%.2fVA, Sc=%.2fVA\r\n",
			   meter->meter_id,
			   meter->measure.Pt,
			   meter->measure.Pa,
			   meter->measure.Pb,
			   meter->measure.Pc,
			   meter->measure.Qt,
			   meter->measure.Qa,
			   meter->measure.Qb,
			   meter->measure.Qc,
			   meter->measure.St,
			   meter->measure.Sa,
			   meter->measure.Sb,
			   meter->measure.Sc);
	METER_DEBUG("Meter %d Power Factor Data: PFt=%.3f (%s), PFa=%.3f (%s), PFb=%.3f (%s), PFc=%.3f (%s)\r\n",
			   meter->meter_id,
			   meter->measure.PFt,
			   (meter->measure.PFt_type == PF_INDUCTIVE) ? "Inductive" : (meter->measure.PFt_type == PF_CAPACITIVE) ? "Capacitive"
																													: "Unknown",
			   meter->measure.PFa,
			   (meter->measure.PFa_type == PF_INDUCTIVE) ? "Inductive" : (meter->measure.PFa_type == PF_CAPACITIVE) ? "Capacitive"
																													: "Unknown",
			   meter->measure.PFb,
			   (meter->measure.PFb_type == PF_INDUCTIVE) ? "Inductive" : (meter->measure.PFb_type == PF_CAPACITIVE) ? "Capacitive"
																													: "Unknown",
			   meter->measure.PFc,
			   (meter->measure.PFc_type == PF_INDUCTIVE) ? "Inductive" : (meter->measure.PFc_type == PF_CAPACITIVE) ? "Capacitive"
																													: "Unknown");
	METER_DEBUG("Meter %d Energy Data: ImpEp=%.3fkWh, ImpEpT1=%.3fkWh, ImpEpT2=%.3fkWh, ImpEpT3=%.3fkWh, ImpEpT4=%.3fkWh, ExpEp=%.3fkWh, ExpEpT1=%.3fkWh, ExpEpT2=%.3fkWh, ExpEpT3=%.3fkWh, ExpEpT4=%.3fkWh\r\n",
			   meter->meter_id,
			   meter->energy.ImpEp,
			   meter->energy.ImpEpT1,
			   meter->energy.ImpEpT2,
			   meter->energy.ImpEpT3,
			   meter->energy.ImpEpT4,
			   meter->energy.ExpEp,
			   meter->energy.ExpEpT1,
			   meter->energy.ExpEpT2,
			   meter->energy.ExpEpT3,
			   meter->energy.ExpEpT4);
}
/* EOL */
