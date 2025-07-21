/*
 * File: Ccp_CanDrv.c
 * Description: CAN Driver implementation for CCP (CAN Calibration Protocol)
 * Author: [JDL_Zhou]
 * Date: [2025/6/9]
 */

#include "Ccp_CanDrv.h"

/* Private macros -----------------------------------------------------------*/
#define CCP_CAN_DRV_VERSION    "1.0.0"

/* Private typedef ----------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

/* Exported functions -------------------------------------------------------*/
/**
 * @brief  Transmits a CCP message over CAN.
 * @param  pData: Pointer to data buffer
 * @param  length: Length of data
 * @retval 0 if success, non-zero otherwise
 */
uint8_t Ccp_CanDrv_Transmit(const uint8_t *pData, uint8_t length)
{
    /* TODO: Add CAN transmit code here */
    McalRetVal_t ret = Mcal_Can_Send_Msg(MCAL_CAN1_TX_CH, (uint8_t *)pData, length);

    if (ret != MCAL_RET_SUCCESS)
    {
        CCP_ERROR("%s Failed to send CAN message, error code: %d\n", __func__, ret);
        return 1; // Indicate failure
    }

    return 0; // Indicate success
}

void Ccp_CanDrv_ReceiveCallback(uint32_t id, const uint8_t *data, uint8_t len)
{
    CCPCANMSGDATA CcpCanMsg_Data = {0};

    CcpCanMsg_Data = *(CCPCANMSGDATA *)data; // Assuming data is a pointer to CCPCANMSGDATA
    CCP_DEBUG("Received CAN message with ID: 0x%03X, Length: %d\n", id, len);
    CCP_PRINT_HEX((uint8_t*)data, len, 1);
    // Process the received data as needed
    Comm_SetCcp(CcpCanMsg_Data); // Call the comm function to handle the reveived message
}

/* Private functions --------------------------------------------------------*/

/* End of file */
