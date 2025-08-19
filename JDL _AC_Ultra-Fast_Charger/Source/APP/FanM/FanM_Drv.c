//******************************************************************************
//* File Name: FanM_Drv.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: FanM module driver file
/*******************************************************************************/
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "FanM_Cfg.h"
#include "Mcal_BigLittle_Endian.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
typedef struct 
{
    uint8_t  slaveAddr;
    uint16_t Reg;
    uint16_t CurrentRunningVol;
}FanM_Drv_Struct;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static FanM_Drv_Struct FanMDrv_Ctrl;

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
void FanMDrv_Init(void)
{
    memset(&FanMDrv_Ctrl, 0, sizeof(FanMDrv_Ctrl));
    FanMDrv_Ctrl.slaveAddr = 0x01;
}

uint8_t FanMDrv_GetSlaveAddr(void)
{
    return FanMDrv_Ctrl.slaveAddr;
}

uint16_t FanMDrv_GetCurrRunningVol(void)
{
    return FanMDrv_Ctrl.CurrentRunningVol;
}

/* when Cmd == MODBUS_STATE_RX_PENDING, Vol is second reg */
uint8_t FanMDrv_SetAdVolCmd(uint8_t Cmd, uint16_t REG, uint16_t Vol)
{
    uint8_t SendBuff[5] = {0};
    uint8_t SendLen = 0;

    FanMDrv_Ctrl.Reg = BIGLITTLEEND_SWAP_2_BYTES(REG);
    BIGLITTLEEND_SWAP_2_BYTES(Vol);
    memcpy(&SendBuff[SendLen], &FanMDrv_Ctrl.Reg ,sizeof(FanMDrv_Ctrl.Reg));
    SendLen += sizeof(FanMDrv_Ctrl.Reg);
    memcpy(&SendBuff[SendLen], &Vol,sizeof(Vol));
    SendLen += sizeof(Vol);

    return (uint8_t)FANM_SEND_MODBUS(FanMDrv_Ctrl.slaveAddr, Cmd, SendBuff, SendLen);
}

void FanMDrv_CurrRunningVolCallBack(uint8_t *data, uint8_t datalen)
{
    if (data != NULL && datalen > 2 &&  2 == data[0])
    {
        FanMDrv_Ctrl.CurrentRunningVol = BigLittleEnd_Swap_2_Bytes(*((uint16_t *)(data + 1)));
    }
    else
    {
        FANM_ERROR("%s: Invalid length. datalen: %d!\r\n", __func__, datalen);
        FANM_PRINT_HEX(data, datalen);
    }
}
/* EOL */
