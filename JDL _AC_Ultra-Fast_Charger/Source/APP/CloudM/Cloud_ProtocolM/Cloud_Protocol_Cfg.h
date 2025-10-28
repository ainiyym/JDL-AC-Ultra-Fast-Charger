//******************************************************************************
//* File Name: Cloud_Protocol_Cfg.h
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module configuration header file
/*******************************************************************************/
#if !defined (__CLOUD_PROTOCOL_CFG_H)
/* polyspace:begin<MISRA-C3:2.5:Not a defect:Justified> Normal. */
/* polyspace<MISRA-C3:21.1:Not a defect:Justified> Normal. */
#define __CLOUD_PROTOCOL_CFG_H
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Cfg.h"
#include "YeeComxxx_Device.h"

/*******************************************************************************
|    Compile Option or configuration Section (for test/debug)
|******************************************************************************/

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define CLOUD_4G_HEARTBEAT_INTERVAL_S               (0)                                            // close auto heartbeat by 4G DTU
#define CLOUD_PROTOCOL_HEARTBEAT_INTERVAL_S         (10 * 1000)                                    // Heartbeat interval in seconds
#define CLOUD_PROTOCOL_HEARTBEAT_TIMEOUT_S          (CLOUD_PROTOCOL_HEARTBEAT_INTERVAL_S * 3)      // Heartbeat timeout threshold in seconds
#define CLOUD_RESET_DEVICE_DELAY_TIME_S             (30 * 1000U / CLOUDM_TASK_PERIOD)              // Remote reset delay time

#define CLOUD_PROTOCOL_0x01_BODY_LENGTH             (30)        // SN(7)+pileType(1)+gunCount(1)+protocolVersion(1)+softwareVersion(8)+networkType(1)+simCard(10)+operator(1)
#define CLOUD_PROTOCOL_0x03_BODY_LENGTH             (9)         // SN(7)+connector id(1)+connector status(1)
#define CLOUD_PROTOCOL_0x05_BODY_LENGTH             (9)         // SN(7)+bollingmodel(2)
#define CLOUD_PROTOCOL_0x09_BODY_LENGTH             (7)         // SN(7)
#define CLOUD_PROTOCOL_0x55_BODY_LENGTH             (14)        // SN(7)+device CP56Time2a(7)
#define CLOUD_PROTOCOL_0x57_BODY_LENGTH             (8)         // SN(7)+result(1)
#define CLOUD_PROTOCOL_0x91_BODY_LENGTH             (8)         // SN(7)+result(1)

#define CLOUD_PROTOCOL_SN_LENGTH                    (7U)        // Device SN length
#define CLOUD_PROTOCOL_TRANSACTION_ID_LENGTH        (16U)       // Transaction ID length
#define CLOUD_PROTOCOL_RFID_UID_LENGTH              (8U)       // RFID length
#define CLOUD_PROTOCOL_PROTOCOL_VERSION             (0X10)      // Communication protocol version (actual version * 10, v1.0 = 0X0A, v1.6 = 0X10)
#define CLOUD_PROTOCOL_SOFTWARE_VERSION_LENGTH      (8U)        // Software version length (including terminator)
#define CLOUD_PROTOCOL_SIM_LENGTH                   (10U)       // SIM card number length (BCD code, 10 bits + 1 terminator)

#define CLOUD_PROTOCOL_RESTART_DEVICE               YeeCom_DeviceRestart          // Restart device function
/*******************************************************************************
|    Enum Definition
|******************************************************************************/
// network type
typedef enum
{
    CLOUD_PROTOCOL_NETWORK_SIM = 0x00,  // SIM
    CLOUD_PROTOCOL_NETWORK_LAN = 0x01,  // LAN
    CLOUD_PROTOCOL_NETWORK_WAN = 0x02,  // WAN
    CLOUD_PROTOCOL_NETWORK_OTHER = 0x03 // OTHER
} Cloud_Protocol_NetworkType;

// operator type
typedef enum
{
    OPERATOR_CMCC = 0x00, // China Mobile
    OPERATOR_CT = 0x02,   // China Telecom
    OPERATOR_CUCC = 0x03, // China Unicom
    OPERATOR_OTHER = 0x04 // other
} Cloud_Protocol_OperatorType;
/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Table Definition
|******************************************************************************/

/*******************************************************************************
|    Global Function Prototypes
|******************************************************************************/

#endif /* __CLOUD_PROTOCOL_CFG_H */
/* EOL */
