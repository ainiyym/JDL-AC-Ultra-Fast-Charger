/*
 * ccp.c
 *                                        CCP-Slave V1.2 for TI DSP C2000
 *
 * TI DSP C2000 is different from normal ARM serials.
 * DSP C2000 uses 16bit memory address.  1 address pointer= 1 word.
 *---------------------------------------------------------------------------------------------------------*|
 *    DSP:uses word pointer for locating memory.
 *---------------------------------------------------------------------------------------------------------*|
 *    The A2L file should be converted by TRM_A2L converter Application before it's imported into CANape.
 *---------------------------------------------------------------------------------------------------------*|
 *    Data Format: Intel
 *---------------------------------------------------------------------------------------------------------*|
 *    EEPROM:  external EERPOM��
 *             Virtual Start Address: 0xD0000 . using normal Byte pointer for memory.
 *---------------------------------------------------------------------------------------------------------*|
 *
 *                                                         Author: Lis
 *                                                     Created on: 2024-9-9
 *---------------------------------------------------------------------------------------------------------*/
#include <Ccp.h>
#include <ccp_DAQ_Tx_Queue.h>
#include "Comm.h"

#ifdef _FLASH
      #pragma CODE_SECTION(ccp_DAQ_Task_1ms, ".TI.ramfunc");
      #pragma CODE_SECTION(ccp_DTO_Transmit, ".TI.ramfunc");
      #pragma CODE_SECTION(ccp_DAQ_write_callback, ".TI.ramfunc");
#endif

//------------------------------------------------------------------------------------------------------------------//
//Tccp ccp;

void Convert_memory_addr_u8_to_u16(uint32_t *elem);
//------------------------------------------------------------------------------------------------------------------//
static uint16_t CCP_Status;
//uint16_t CCP_RX_Data[8];

//static TCCP_Data rx_pkg;         // receive data buffer
static TCCP_Data tx_pkg;         // transmit data buffer
//static bool      tx_en;          //flag of enable send data.

uint32_t CCP_MTA[2];               //32bits address
uint8_t CCP_Extension_Address[2];  //offset of address
uint8_t CCP_SessionStatus;

uint8_t CCP_ProtectionStatus;
uint8_t CCP_ResourceMask;

uint8_t CCP_Active_Page;

uint32_t CCP_CheckSumSize;
uint8_t CCP_Checksum_Flag;

TCCP_Data ccp_can;

static TCCP_DAQ_Channel DAQ_Channel;
//------------------------------------------------------------------------------------------------------------------//
#ifdef CCP_DAQ_REQUEST
    CCP_DAQ_strcut CCP_DAQ_Flag[CCP_MAX_DAQ];
    uint8_t CCP_DAQ_ODT_Used[CCP_MAX_DAQ][CCP_MAX_ODT];
    uint16_t CCP_DAQ_Prescaler[CCP_MAX_DAQ];
    uint8_t CCP_DAQ_PDU_Data_Buffer[CCP_ODT_BUF_NUM][CCP_CAN_LENGTH];
    uint8_t CCP_DAQ_PDU_Data_Buffer_Empty;
    uint8_t CCP_DAQ_PDU_Data_Buffer_Used;
    uint32_t CCP_ODT[CCP_MAX_DAQ][CCP_MAX_ODT][CCP_MAX_ELEMENT];
    uint8_t CCP_DAQ_List_Ptr;
    uint8_t CCP_DAQ_ODT_Ptr;
    uint8_t CCP_DAQ_Element_Ptr;

    uint8_t Ccp_DAQ_st;
    uint8_t CCP_Tx_Busy;
    uint8_t test_flag;
    uint8_t dummy;
    uint8_t DAQ_PDU_Data[CCP_CAN_LENGTH];
#endif
//------------------------------------------------------------------------------------------------------------------//
const uint8_t CCP_Station_ID[]={"MCU"};
const uint8_t CCP_DTO_PID = {6u};
//------------------------------------------------------------------------------------------------------------------//
const uint16_t CCP_DAQ_Event_Cycle[]={1u,5u,10u,50u,100u,200u,500u,1000u}; //unit:ms | Task Sys-inverval: 1ms
//const uint16_t CCP_DAQ_Event_Cycle[]={1u,5u,25u,50u}; //unit:ms | Task Sys-inverval: 1ms
//------------------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------------------//
void ccp_init(void)
{
    ccp_DAQ_tx_queue_init();

    CCP_MTA[0] = 0;
    CCP_MTA[1] = 0;

    CCP_Extension_Address[0] = 0;
    CCP_Extension_Address[1] = 0;

    CCP_Tx_Busy = STD_OFF;
    //---------------------------//
    DAQ_Channel.ct = 0;

    //    ccp_can.read_callback_ptr=&ccp_Read_CRO_Proc;
    //    ccp_can.write_callback_ptr=&ccp_DAQ_write_callback;
    //
    //    mos.Add(ccp_DAQ_Task_1ms , 1,_TASK_AUTO,_TASK_RUN); //create a task for DAQ
}

// ECU address uses Intel format,  Low bit first.
static uint16_t ccp_Get_ECU_Addr(TCCP_CRO v, TECU_Bits bits)
{
    uint16_t addr = 0;
    switch (bits)
    {
    case ECU_BITS_23:
        addr = (((uint16_t)v.bytes.dt3) << 8u);
        addr |= (uint16_t)v.bytes.dt2;
        break;
    case ECU_BITS_45:
        addr = (((uint16_t)v.bytes.dt5) << 8u);
        addr |= (uint16_t)v.bytes.dt4;
        break;
    }
    return addr;
}

static uint32_t ccp_Make_32Bits(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3)
{
    uint32_t CCP_Result;

#ifdef CCP_MSB
    CCP_Result = ((((uint32_t)data0) << 24u) | (((uint32_t)data1) << 16u) | (((uint32_t)data2) << 8u) | ((uint32_t)data3));
#else
    CCP_Result = ((((uint32_t)data3) << 24u) | (((uint32_t)data2) << 16u) | (((uint32_t)data1) << 8u) | ((uint32_t)data0));
#endif
    return CCP_Result;
}

/*-------------------------------------------------------------------\
*          void ccp_DTO_Transmit(void)
*
* ------------------------------------------------------------------*/
static void ccp_DTO_Transmit(void)
{
    uint8_t data_array[8];
    CCPCANMSGDATA CcpCanMsg_Data;

    if ((CCP_SessionStatus & CCP_RUN) == STD_OFF)
    {
        CcpCanMsg_Data.Data.all = tx_pkg.all;
        Comm_CcpCanMsgTx(CcpCanMsg_Data);
    }else
    {
        //if DAQ is running, the normal polling data uses TX buffer queue.
        data_array[0]=tx_pkg.bytes.dt0;
        data_array[1]=tx_pkg.bytes.dt1;
        data_array[2]=tx_pkg.bytes.dt2;
        data_array[3]=tx_pkg.bytes.dt3;
        data_array[4]=tx_pkg.bytes.dt4;
        data_array[5]=tx_pkg.bytes.dt5;
        data_array[6]=tx_pkg.bytes.dt6;
        data_array[7]=tx_pkg.bytes.dt7;

        ccp_DAQ_tx_queue_push(&data_array[0]);               //add data to the queue. Lis 2024-9-20
#if 0 
        if((ccp_DAQ_tx_queue_Is_empty() == 0) &&(CCP_Tx_Busy == STD_OFF) )
        {
              CCP_Tx_Busy = STD_ON;
              ccp_DAQ_tx_queue_pop();
        }
#endif
    }
}


//-------------------------------------------------------------------//
/********************************************************************************************************************************
Function Name   :               ccp_Test_Connect
Function ID     :
Programmer      :
Date            :

Arguments       : TCCP_CRO v
Returns         : bool            true:response  false: dumy
Notes           :
********************************************************************************************************************************/
static _Bool ccp_Test_Connect(TCCP_CRO v)
{
    uint16_t ECU_addr = 0u;
    _Bool flg = 1;

    ECU_addr = ccp_Get_ECU_Addr(v, ECU_BITS_23);

    if ((ECU_addr == CCP_NODE_ID) || (ECU_addr == CCP_BROADCAST_ID))
    {
        CCP_Status = CCP_CONNECTED;

        ccp_DAQ_tx_queue_init(); // initialize DAQ_tx_queue
    }
    else
    {
        flg = 0;
        if (CCP_Status == CCP_CONNECTED)
        {
            CCP_Status = CCP_TMP_DISCONNECTED;
        }
    }
    return flg;
}

/*-------------------------------------------------------------------\
*                  ccp_Set_MTA
* Definition :
* Parameter  :TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Set_MTA(TCCP_CRO v)
{
    uint32_t MTA_ptr = 0u;
    uint8_t MTA_Index =(uint8_t)v.bytes.dt2;

    MTA_ptr = ccp_Make_32Bits((uint8_t)v.bytes.dt4,(uint8_t)v.bytes.dt5,(uint8_t)v.bytes.dt6,(uint8_t)v.bytes.dt7);

    if (MTA_Index < CCP_MTA_NUM)
    {
        CCP_MTA[MTA_Index] = MTA_ptr;
        CCP_Extension_Address[MTA_Index] =(uint8_t) v.bytes.dt3;
    }
    else
    {
        tx_pkg.bytes.dt1=CCP_OUT_OF_RANGE; //ERR
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Download
* Definition :
* Parameter  :TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Download(TCCP_CRO v)
{
    uint8_t count = 0u;
    uint8_t Bytes_Number = 0u;
    uint16_t data_array[8];
    uint32_t MTA_Temp;
    //uint16_t tmp_dt;

    data_array[0]=v.bytes.dt0;
    data_array[1]=v.bytes.dt1;
    data_array[2]=v.bytes.dt2;
    data_array[3]=v.bytes.dt3;
    data_array[4]=v.bytes.dt4;
    data_array[5]=v.bytes.dt5;
    data_array[6]=v.bytes.dt6;
    data_array[7]=v.bytes.dt7;

#ifdef CCP_SEED_KEY
    if ((CCP_ProtectionStatus & CCP_RM_CAL) == 0u)
    {
        tx_pkg.bytes.dt1 = CCP_ACCESS_DENIED;
    }
    else
#endif
    {
        if (v.bit.cmd == CCP_DNLOAD6)
        {
            Bytes_Number = 6u;
        }
        else
        {
            Bytes_Number =data_array[2];
        }

    #ifdef CCP_16BITS_ADDRESS_MODE
        for (count = 0u; count < Bytes_Number; (count += 2u))
        {
            if (CCP_COMMAND_CODE == CCP_DNLOAD6)
            {
                *(uint8_t *)CCP_MTA[0] = CCP_Make_16Bits(data_array[count + 2u], data_array[count + 3u]);
            }
            else
            {
                *(uint8_t *)CCP_MTA[0] = CCP_Make_16Bits(data_array[count + 3u], data_array[count + 4u]);
            }
            CCP_MTA[0]++;
        }
    #else

        count=0;
        if (CCP_Active_Page == CCP_REF_PAGE_INDEX) //RAM page
        {
            MTA_Temp=CCP_MTA[0]>>1;
            while(count<Bytes_Number)
            {
                if (v.bit.cmd == CCP_DNLOAD6) // DNLOAD6   2~7
                {
                    *(uint16_t *)MTA_Temp =CCP_Make_16Bits(data_array[count+2],data_array[count+3]);
                    count+=2;
                }
                else  // DNLOAD3   3~7
                {
                    if (count==7)
                    {
                        *(uint16_t *)MTA_Temp=data_array[count+3];
                    }else{
                        *(uint16_t *)MTA_Temp =CCP_Make_16Bits(data_array[count+3],data_array[count+4]);
                    }

                    if (count==6)
                    {
                       count++;
                    }else{
                       count+=2;
                    }
                }
                MTA_Temp++;
            }
        }else{  // CCP_Active_Page = CCP_WORK_PAGE_INDEX;

            if (CCP_MTA[0]>=CCP_EEPROM_OFFSET)
            {
                MTA_Temp=CCP_MTA[0]-CCP_EEPROM_OFFSET;

                if (v.bit.cmd == CCP_DNLOAD6) // DNLOAD6   2~7
                {
                    count=2;
                }
                else  // DNLOAD3   3~7
                {
                    count=3;
                }
//                FnEEPROM.Write_Enable(true);
//                FnEEPROM.Write_Bytes(MTA_Temp,&data_array[count],Bytes_Number);
//                FnEEPROM.Write_Enable(false);
            }else{
                tx_pkg.bytes.dt1 = CCP_RESOURCE_USELESS;
            }
        }
    #endif

        CCP_MTA[0]+=Bytes_Number;

        tx_pkg.bytes.dt3 = CCP_Extension_Address[0];
        tx_pkg.bytes.dt4 = CCP_Get_1Byte(CCP_MTA[0]);
        tx_pkg.bytes.dt5 = CCP_Get_2Byte(CCP_MTA[0]);
        tx_pkg.bytes.dt6 = CCP_Get_3Byte(CCP_MTA[0]);
        tx_pkg.bytes.dt7 = CCP_Get_4Byte(CCP_MTA[0]);
    }
}


/*-------------------------------------------------------------------\
*                  ccp_Upload
* Definition :
* Parameter  :TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Upload(TCCP_CRO v)
{

     uint32_t MTA_Temp;
     uint16_t tmp_dt;
     uint16_t data_array[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
     uint8_t count = 0u;

    if ((v.bytes.dt2) > CCP_UPLOAD_MAX)
    {
        tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
    }
    else
    {
        if (v.bit.cmd == CCP_UPLOAD)  //UPLOAD 0x04
        {
            MTA_Temp = CCP_MTA[0];
        }
        else                          //SHORT_UP 0x0F
        {
            MTA_Temp = ccp_Make_32Bits(v.bytes.dt4, v.bytes.dt5, v.bytes.dt6, v.bytes.dt7);
        }

        count=0;
        if (CCP_Active_Page == CCP_REF_PAGE_INDEX) //RAM page
        {
            if (v.bytes.dt2==1)     //just read one byte.
            {
                if ((MTA_Temp % 2) ==0)
                {
                    MTA_Temp>>=1;
                    data_array[count + 3]=CCP_Get_First_Byte(*((uint16_t *) MTA_Temp));
                }else{
                    MTA_Temp>>=1;
                    data_array[count + 3]=CCP_Get_Second_Byte(*((uint16_t *) MTA_Temp));
                }
            }else{
                MTA_Temp>>=1;           //Lis, 4 byte to  2Word ,for C2000 DSP
                while(count<v.bytes.dt2)
                {
                    tmp_dt=*((uint16_t *) MTA_Temp);
                    data_array[count + 3]=CCP_Get_First_Byte(tmp_dt);
                    count++;
                    data_array[count + 3]=CCP_Get_Second_Byte(tmp_dt);
                    count++;
                    MTA_Temp++;
                }
            }
        }else{  //Flash Page        // CCP_Active_Page = CCP_WORK_PAGE_INDEX;
            if (MTA_Temp>=CCP_EEPROM_OFFSET)
            {
                MTA_Temp-=CCP_EEPROM_OFFSET;
//                FnEEPROM.Read_Bytes(MTA_Temp,&data_array[3],v.bytes.dt2);
            }
        }

        tx_pkg.bytes.dt3 = data_array[3];
        tx_pkg.bytes.dt4 = data_array[4];
        tx_pkg.bytes.dt5 = data_array[5];
        tx_pkg.bytes.dt6 = data_array[6];
        tx_pkg.bytes.dt7 = data_array[7];
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Get_Version
* Definition :
* Parameter  : none
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Get_Version(void)
{
    tx_pkg.bytes.dt3 = CCP_VERSION_MAJOR;
    tx_pkg.bytes.dt4 = CCP_VERSION_MINOR;
}

/*-------------------------------------------------------------------\
*                  ccp_Exchange_ID
* Definition :
* Parameter  :TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Exchange_ID(TCCP_CRO v)
{
    tx_pkg.bytes.dt3 = CCP_ECU_NAME_LEGNTH;
    tx_pkg.bytes.dt4= CCP_ASCII_TYPE;
    tx_pkg.bytes.dt5 = CCP_RM_CAL;
    tx_pkg.bytes.dt6= CCP_DEFAULT_ZERO;

    #ifdef CCP_SEED_KEY
        tx_pkg.bytes.dt6 |= CCP_RM_CAL;
    #endif

    #ifdef CCP_DAQ_REQUEST
        tx_pkg.bytes.dt5 |= CCP_RM_DAQ;
       #ifdef CCP_SEED_KEY
           tx_pkg.bytes.dt6 |= CCP_RM_DAQ;
       #endif
    #endif

    #ifdef CCP_FLASH
       tx_pkg.bytes.dt5 |= CCP_RM_PGM;
       #ifdef CCP_SEED_KEY
           tx_pkg.bytes.dt6 |= CCP_RM_PGM;
       #endif
    #endif

    #ifdef CCP_DAQ_REQUEST
           tx_pkg.bytes.dt5 |= CCP_RM_DAQ;
    #ifdef CCP_SEED_KEY
           tx_pkg.bytes.dt6 |= CCP_RM_DAQ;
    #endif
    #endif
    CCP_MTA[0] = (uint32_t)CCP_Station_ID; //get pointer of CCP_Station_ID[]
}
/*-------------------------------------------------------------------\
*                  ccp_Exchange_ID
* Definition :
* Parameter  :TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_DisConnect(TCCP_CRO v)
{
    uint16_t ECU_addr = 0u;

#ifdef CCP_DAQ_REQUEST
    uint8_t count = 0u;
#endif
    ECU_addr=ccp_Get_ECU_Addr(v,ECU_BITS_45);


    if ((ECU_addr == CCP_NODE_ID) || (ECU_addr == CCP_BROADCAST_ID))
    {
        if ((v.bytes.dt2 == CCP_TEM_END_SESSION)|| (v.bytes.dt2== CCP_END_SESSION))
        {
            if (v.bytes.dt2 == CCP_TEM_END_SESSION)
            {
                CCP_Status = CCP_TMP_DISCONNECTED;
            }
            else
            {
                if ((CCP_SessionStatus & CCP_SESSION_RUSUME) == CCP_SESSION_RUSUME)
                {
                #ifdef CCP_SEED_KEY
                    CCP_ProtectionStatus = CCP_RM_ALL_LOCKED;
                #endif

                #ifdef CCP_DAQ_REQUEST
                    for (count = 0u; count < CCP_MAX_DAQ; count++)
                    {
                        CCP_DAQ_Flag[count].flag = STD_OFF;
                        CCP_DAQ_Flag[count].prepare = STD_OFF;
                    }
                #endif
                }
                else
                {
                #ifdef CCP_SEED_KEY
                    CCP_ProtectionStatus = CCP_RM_DAQ;
                #endif
                }

            #ifdef CCP_STORE_CAL_DATA
                if ((CCP_SessionStatus & CCP_SESSION_STORE) == CCP_SESSION_STORE)
                {
                    Ccp_Store_Cal_Data();
                }
                else
                {
                     /* Do nothing */
                }
            #endif

                CCP_Status = CCP_DISCONNECTED;
                CCP_SessionStatus = CCP_DEFAULT_SESSION;
            }
        }
        else
        {
            tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
        }
       // Ccp_DTO_Transmit();
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Get_S_Status
* Definition :
* Parameter  : none
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Get_S_Status(void)
{
    tx_pkg.bytes.dt3 = CCP_SessionStatus;
    tx_pkg.bytes.dt4 = CCP_NO_ADDITIONAL_INFO;
}

/*-------------------------------------------------------------------\
*                  ccp_Set_S_Status
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Set_S_Status(TCCP_CRO v)
{
    CCP_SessionStatus = v.bytes.dt2;
}

/*-------------------------------------------------------------------\
*                  ccp_Start_Stop
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Start_Stop(TCCP_CRO v)
{
    uint8_t count = CCP_DEFAULT_ZERO;
    uint8_t CCP_Run_Flag = CCP_DEFAULT_ZERO;
    uint16_t temp = CCP_DEFAULT_ZERO;
    uint8_t DAQ_Index;
    uint8_t ODT_Index;

    DAQ_Index = v.bytes.dt3;
    ODT_Index = v.bytes.dt4;

#ifdef CCP_SEED_KEY
    if ((CCP_ProtectionStatus & CCP_RM_DAQ) == 0u)
    {
        tx_pkg.bytes.dt1 = CCP_ACCESS_DENIED;
    }
    else
#endif
    {
        if ((DAQ_Index < CCP_MAX_DAQ) && (ODT_Index < CCP_MAX_ODT))
        {
            if (v.bytes.dt2== CCP_STOP_DAQ)
            {

                DAQ_Channel.ct=0;  //Lis.2024-9-20

                CCP_DAQ_Flag[DAQ_Index].flag = STD_OFF;
                CCP_DAQ_Flag[count].prepare = STD_OFF;
                for (count = 0u; count <= ODT_Index; count++)
                {
                    CCP_DAQ_ODT_Used[DAQ_Index][count] = 0u;
                }

                for (count = 0u; count < CCP_MAX_DAQ; count++)
                {
                    CCP_Run_Flag += CCP_DAQ_Flag[count].flag;
                }

                if (CCP_Run_Flag == 0u)
                {
                    CCP_SessionStatus &= (uint16_t)(~CCP_RUN);
                }
                else
                {
                }
            }
            else if ((v.bytes.dt2 == CCP_START_DAQ)|| (v.bytes.dt2 == CCP_PREPARE_DAQ))
            {
                temp = CCP_Make_16Bits(v.bytes.dt6, v.bytes.dt7);
                if (temp == 0u)
                {
                    tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
                }
                else
                {
                    CCP_DAQ_Prescaler[DAQ_Index] = temp;
                    CCP_DAQ_Flag[DAQ_Index].channel = v.bytes.dt5;

                    for (count = 0u; count <= ODT_Index; count++)
                    {
                        CCP_DAQ_ODT_Used[DAQ_Index][count] = STD_ON;
                    }

                    if (v.bytes.dt2 == CCP_START_DAQ)
                    {
                        CCP_DAQ_Flag[DAQ_Index].flag = STD_ON;
                        CCP_SessionStatus |= (CCP_RUN);
                    }
                    else
                    {
                        CCP_DAQ_Flag[DAQ_Index].prepare = STD_ON;
                    }
                  //----------------------------------------------//  Lis added 2024-9-18
                    DAQ_Channel.event_ch[DAQ_Channel.ct]=v.bytes.dt5;
                    DAQ_Channel.prescale[DAQ_Channel.ct]=temp;
                    DAQ_Channel.counter[DAQ_Channel.ct]=0;
                    DAQ_Channel.ct++;
                  //----------------------------------------------//
                }
            }
            else
            {
                tx_pkg.bytes.dt1= CCP_OUT_OF_RANGE;
            }
        }
        else
        {
            tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
        }
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Start_Stop_All
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Start_Stop_All(TCCP_CRO v)
{
    uint8_t count = 0u;
    uint8_t sub_count = 0u;

#ifdef CCP_SEED_KEY
    if ((CCP_ProtectionStatus & CCP_RM_DAQ) == 0u)
    {
        tx_pkg.bytes.dt1 = CCP_ACCESS_DENIED;
    }
    else
#endif
    {
        if (v.bytes.dt2 == CCP_STOP_DAQ_ALL)
        {
            for (count = 0; count < CCP_MAX_DAQ; count++)
            {
                CCP_DAQ_Flag[count].flag = STD_OFF;
            }

            for (count = 0u; count < CCP_MAX_DAQ; count++)
            {
                for (sub_count = 0u; sub_count < CCP_MAX_ODT; sub_count++)
                {
                    CCP_DAQ_ODT_Used[count][sub_count] = STD_OFF;
                }
            }

           // CCP_DAQ_PDU_Data_Buffer_Empty = STD_ON;
           // CCP_DAQ_PDU_Data_Buffer_Used = CCP_DEFAULT_ZERO;
            CCP_SessionStatus &= (~CCP_RUN);
        }
        else if (v.bytes.dt2 == CCP_START_DAQ_ALL)
        {
            for (count = 0u;count < CCP_MAX_DAQ; count++)
            {
                if (CCP_DAQ_Flag[count].prepare == STD_ON)
                {
                    CCP_DAQ_Flag[count].flag = STD_ON;
                }
            }

            CCP_SessionStatus |= (CCP_RUN);
        }
        else
        {
            tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
        }
    }
}


/*-------------------------------------------------------------------\
*                  ccp_Clear_Memory
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
//#ifdef CCP_FLASH
static void ccp_Clear_Memory(TCCP_CRO v)
{
 //   uint32_t size = 0u;
   // uint8_t Return_Code = E_NOT_OK;

#ifdef CCP_SEED_KEY
    if ((CCP_ProtectionStatus & CCP_RM_PGM) == STD_OFF)
    {
        tx_pkg.bytes.dt1 = CCP_ACCESS_DENIED;
    }
    else
#endif
    {
      //  size = ccp_Make_32Bits(v.bytes.dt2,v.bytes.dt3,v.bytes.dt4,v.bytes.dt5);
//       // Return_Code = Ccp_FlashErase((uint32_t) CCP_Gloab_Address(CCP_MTA[0]),size);
//        if (Return_Code == E_NOT_OK)
//        {
//            tx_pkg.bytes.dt1 = CCP_RESOURCE_USELESS;
//        }
//        else
//        {
//            /*Do nothing*/
//        }
    }
}
//#endif

#ifdef CCP_SEED_KEY
/*-------------------------------------------------------------------\
*                  ccp_Get_Seed
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Get_Seed(TCCP_CRO v)
{
    uint32_t Getseed = CCP_DEFAULT_ZERO;
    uint8_t Req_Resource = v.bytes.dt2;
    if ((Req_Resource == CCP_RM_CAL)|| (Req_Resource == CCP_RM_PGM)|| (Req_Resource == CCP_RM_DAQ))
    {
        if ((CCP_ProtectionStatus & Req_Resource) == STD_OFF)
        {
            tx_pkg.bytes.dt3 = CCP_NEED_UNLOCK;
            CCP_ResourceMask = Req_Resource;
        }
        else
        {
            tx_pkg.bytes.dt3 = CCP_NO_UNLOCK;
        }
        Getseed = Ccp_GetSeed(Req_Resource);
        tx_pkg.bytes.dt4 = CCP_Get_1Byte(Getseed);
        tx_pkg.bytes.dt5 = CCP_Get_2Byte(Getseed);
        tx_pkg.bytes.dt6 = CCP_Get_3Byte(Getseed);
        tx_pkg.bytes.dt7 = CCP_Get_4Byte(Getseed);
    }
    else
    {
        tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
    }
}
/*-------------------------------------------------------------------\
*                  ccp_Get_Seed
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Unlock(TCCP_CRO v)
{
    uint8_t Result = 0u;
    uint32_t Key = CCP_DEFAULT_ZERO;
    Key = (((uint32_t)v.bytes.dt5 << 24u)| ((uint32_t)v.bytes.dt4 << 16u)| ((uint32_t)v.bytes.dt3 << 8u)| ((uint32_t)v.bytes.dt2));
    Result = Ccp_Unlock(Key);
    if (Result == CCP_WRONG_KEY)
    {
        tx_pkg.bytes.dt1 = CCP_ACCESS_DENIED;
    }
    else
    {
        CCP_ProtectionStatus |= CCP_ResourceMask;
        tx_pkg.bytes.dt3= CCP_ProtectionStatus;
    }
}
#endif
/*-------------------------------------------------------------------\
*                  ccp_Write_DAQ
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
//#ifdef CCP_DAQ_REQUEST
static void ccp_Write_DAQ(TCCP_CRO v)
{
    uint8_t count = 0u;
    uint8_t size = 0u;
    uint32_t write_daq_ptr = 0u;


    size =v.bytes.dt2;

    if ((size != 1u) && (size != 2u) && (size != 4u))
    {
        tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
    }
    else
    {
        write_daq_ptr = ccp_Make_32Bits(v.bytes.dt4,v.bytes.dt5,v.bytes.dt6,v.bytes.dt7);

       // write_daq_ptr >>=1; //For converted DSP 16bit ECU memory address ,Lis added 2024-10-15
       // write_daq_ptr=(write_daq_ptr>>1)+(write_daq_ptr%2);

    #ifdef CCP_16BITS_ADDRESS_MODE
        for (count = 0u; count < (size >> 1u); count++)
    #else
        for (count = 0u; count < size; count++)
    #endif
        {
         #ifdef CCP_16BITS_ADDRESS_MODE
            if (CCP_DAQ_Element_Ptr >= (uint8_t)0x03u)
         #else
            if (CCP_DAQ_Element_Ptr >= (uint8_t)CCP_MAX_ELEMENT)
         #endif
            {
                tx_pkg.bytes.dt1  = CCP_OUT_OF_RANGE;
                break;
            }

            CCP_ODT[CCP_DAQ_List_Ptr][CCP_DAQ_ODT_Ptr][CCP_DAQ_Element_Ptr] = write_daq_ptr;
            CCP_DAQ_Element_Ptr++;
            write_daq_ptr++;
        }
    }
}
//#endif

/*-------------------------------------------------------------------\
*                  ccp_Get_DAQ_Size
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Get_DAQ_Size(TCCP_CRO v)
{
#ifdef CCP_DAQ_REQUEST
    uint8_t count = 0u;
    uint8_t sub_count = 0u;
    if (v.bytes.dt2 < CCP_MAX_DAQ)
    {
        CCP_SessionStatus &= (~CCP_SESSION_RUN);
        tx_pkg.bytes.dt3 = CCP_MAX_ODT ;
        tx_pkg.bytes.dt4 = (uint8_t)(v.bytes.dt2 * CCP_MAX_ODT);
        for (count = 0u; count < CCP_MAX_ODT; count++)
        {
            for (sub_count = 0u; sub_count < CCP_MAX_ELEMENT; sub_count++)
            {
                CCP_ODT[v.bytes.dt2][count][sub_count] = CCP_DEFAULT_ZERO;
            }
        }
    }
    else
    {
        tx_pkg.bytes.dt3 = CCP_DEFAULT_ZERO;
        tx_pkg.bytes.dt4 = CCP_NO_FIRST_PID;
    }
#else
    tx_pkg.bytes.dt3 = CCP_DEFAULT_ZERO;
    tx_pkg.bytes.dt4 = CCP_NO_FIRST_PID;
#endif
}

/*-------------------------------------------------------------------\
*                  ccp_Set_DAQ_Ptr
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
#ifdef CCP_DAQ_REQUEST
static void ccp_Set_DAQ_Ptr(TCCP_CRO v)
{
    if ((v.bytes.dt2 >= CCP_MAX_DAQ)|| (v.bytes.dt3>= CCP_MAX_ODT)|| (v.bytes.dt4 >= CCP_MAX_ELEMENT))
    {
        v.bytes.dt1= CCP_OUT_OF_RANGE;
    }
    else
    {
        CCP_DAQ_List_Ptr = v.bytes.dt2;
        CCP_DAQ_ODT_Ptr = v.bytes.dt3;
        CCP_DAQ_Element_Ptr = v.bytes.dt4;
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Event_DAQ_Transmit
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
void Convert_memory_addr_u8_to_u16(uint32_t *elem)
{
//    uint32_t ref_addr;
    uint16_t i=1;
//
//    ref_addr=*elem;
    elem++;

    while(i<CCP_MAX_ELEMENT)
    {
       if (*elem==*(elem-1))
       {
              *elem=CCP_DEFAULT_ZERO;
       }
       elem++;
       i++;
    }

}

/*-------------------------------------------------------------------\
*                  ccp_Event_DAQ_Transmit
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Event_DAQ_Transmit(uint8_t event_channel)
{
    uint8_t DAQ_Count = CCP_DEFAULT_ZERO;
    uint8_t ODT_Count = CCP_DEFAULT_ZERO;
    uint8_t Entry_Count = CCP_DEFAULT_ZERO;
    // uint8_t Data_Count = CCP_DEFAULT_ZERO;

    uint32_t MTA_Temp = 0;

    uint8_t pdu_idx = 0;
    uint16_t tmp_dt;

#ifdef CCP_16BITS_ADDRESS_MODE
    uint8_t Two_Bytes_Count = CCP_DEFAULT_ZERO;
#endif

    if ((CCP_SessionStatus & CCP_RUN) == STD_OFF)
    {
        /* Do nothing */
    }
    else
    {
        for (DAQ_Count = 0u; DAQ_Count < CCP_MAX_DAQ; DAQ_Count++)
        {
            if ((CCP_DAQ_Flag[DAQ_Count].flag == STD_ON) && (CCP_DAQ_Flag[DAQ_Count].channel == event_channel))
            {

                for (ODT_Count = 0u; ODT_Count < CCP_MAX_ODT; ODT_Count++)
                {

                    if (CCP_DAQ_ODT_Used[DAQ_Count][ODT_Count] == STD_ON)
                    {
                        // Convert_memory_addr_u8_to_u16(&(CCP_ODT[DAQ_Count][ODT_Count][0]));

                        // Ccp_DisableInterrupt();
                        DAQ_PDU_Data[0] = (uint8_t)((DAQ_Count * CCP_MAX_ODT) + (ODT_Count));

                        Entry_Count = 0;
                        pdu_idx = 0;
                        while (Entry_Count < CCP_MAX_ELEMENT)
                        {
                            if (CCP_ODT[DAQ_Count][ODT_Count][Entry_Count] != CCP_DEFAULT_ZERO)
                            {
                                MTA_Temp = CCP_ODT[DAQ_Count][ODT_Count][Entry_Count];
                                tmp_dt = *((uint16_t *)(MTA_Temp >> 1));
                                pdu_idx++;
                                if ((MTA_Temp % 2) == 0)
                                {
                                    DAQ_PDU_Data[pdu_idx] = CCP_Get_First_Byte(tmp_dt);
                                }
                                else
                                {
                                    DAQ_PDU_Data[pdu_idx] = CCP_Get_Second_Byte(tmp_dt);
                                }
                            }
                            Entry_Count++;
                        }
                        ccp_DAQ_tx_queue_push(&DAQ_PDU_Data[0]); // add data to the queue. Lis 2024-9-20
#if 0
                        if ((ccp_DAQ_tx_queue_Is_empty() == 0) && (CCP_Tx_Busy == STD_OFF))
                        {
                            CCP_Tx_Busy = STD_ON;
                            ccp_DAQ_tx_queue_pop();
                        }
#endif
                    }
                }
            }
        }
    }
}
/*-------------------------------------------------------------------\
*                  ccp_DAQ_Task_1ms
* Definition :
* Parameter  : TMos_QueuePtr handle
* Return     : none
* Notes      :  MOS task.
* ------------------------------------------------------------------*/
void ccp_DAQ_Task_1ms(void)
{
    static uint8_t event_idx = 0;
    static uint32_t pre_max_ct;

    if ((CCP_SessionStatus & CCP_RUN) == STD_OFF)
    {
        /* Do nothing */
    }
    else
    {
        for (event_idx = 0; event_idx < DAQ_Channel.ct; event_idx++)
        {
            pre_max_ct = (uint32_t)DAQ_Channel.prescale[event_idx] * (uint32_t)CCP_DAQ_Event_Cycle[DAQ_Channel.event_ch[event_idx]];
            if (++DAQ_Channel.counter[event_idx] >= pre_max_ct)
            {
                DAQ_Channel.counter[event_idx] = 0; // reset counter
                ccp_Event_DAQ_Transmit(DAQ_Channel.event_ch[event_idx]);
            }
        }
    }
}

/*-------------------------------------------------------------------\
*                  ccp_DAQ_write_callback
* Definition :
* Parameter  : uint32_t obj_id
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
void ccp_DAQ_write_callback(void)
{

    CCP_Tx_Busy = STD_ON;
    ccp_DAQ_tx_queue_pop();
    CCP_Tx_Busy = STD_OFF;
    //   if((ccp_DAQ_tx_queue_Is_empty() == 1)  )
    //   {
    //           CcpCanMsg_Data.Data.all = 0;
    //           Comm_CcpCanMsgTx(CcpCanMsg_Data,0);
    //           CanbRegs.CAN_IF1CMD.bit.TXRQST = 0;
    //   }
}

#endif


#ifdef CCP_CAL_PAGE
/*-------------------------------------------------------------------\
*                  ccp_Set_Cal_Page
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :CCP_REF_PAGE_ADDR:    RAM  0x01
*             CCP_WORK_PAGE_ADDR:  Flash 0x02
* ------------------------------------------------------------------*/
static void ccp_Set_Cal_Page(TCCP_CRO v)
{
   // uint8_t Return_Code=E_OK;
    if ((CCP_MTA[0] == CCP_REF_PAGE_ADDR) || (CCP_MTA[0] == CCP_WORK_PAGE_ADDR))
    {
        if (CCP_MTA[0] == CCP_REF_PAGE_ADDR )
        {
            CCP_Active_Page = CCP_REF_PAGE_INDEX;
        }
        else
        {
            CCP_Active_Page = CCP_WORK_PAGE_INDEX;
        }

//       // Return_Code = Ccp_Select_Page_Indication(CCP_Active_Page);
//        if (Return_Code != E_OK)
//        {
//            tx_pkg.bytes.dt1 = CCP_RESOURCE_USELESS;
//        }
//        else
//        {
//            /*Do nothing*/
//        }
    }
    else
    {
        tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
    }
}

/*-------------------------------------------------------------------\
*                  ccp_Get_Cal_Page
* Definition :
* Parameter  : none
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Get_Cal_Page(void)
{
    uint32_t temp = 0u;
    if (CCP_Active_Page == CCP_REF_PAGE_INDEX)
    {
        temp= CCP_REF_PAGE_ADDR;
    }
    else
    {
        temp= CCP_WORK_PAGE_ADDR;
    }

    tx_pkg.bytes.dt3 = CCP_Extension_Address[0];
    tx_pkg.bytes.dt4 = CCP_Get_1Byte(temp);
    tx_pkg.bytes.dt5 = CCP_Get_2Byte(temp);
    tx_pkg.bytes.dt6 = CCP_Get_3Byte(temp);
    tx_pkg.bytes.dt7 = CCP_Get_4Byte(temp);
}
#endif
/*End of ifdef CCP_CAL_PAGE*/


//#ifdef CCP_CHECKSUM
/*-------------------------------------------------------------------\
*                  ccp_Build_Checksum
* Definition :
* Parameter  : TCCP_CRO v
* Return     : none
* Notes      :
* ------------------------------------------------------------------*/
static void ccp_Build_Checksum(TCCP_CRO v)
{
#ifdef CCP_CHECKSUM_CCITT
    tx_pkg.bytes.dt4 = 0xFFu;
    tx_pkg.bytes.dt5 = 0xFFu;
    tx_pkg.bytes.dt6 = 0xFFu;
    tx_pkg.bytes.dt7 = 0xFFu;
#else
    tx_pkg.bytes.dt4 = 0u;
    tx_pkg.bytes.dt5 = 0u;
    tx_pkg.bytes.dt6 = 0u;
    tx_pkg.bytes.dt7 = 0u;
#endif
    CCP_MTA[1] = CCP_MTA[0];
    CCP_CheckSumSize = ccp_Make_32Bits(v.bytes.dt2,v.bytes.dt3,v.bytes.dt4,v.bytes.dt5);
    CCP_Checksum_Flag = STD_ON;
}
//#endif

/*-------------------------------------------------------------------\
 *                  ccp_Move
 * Definition :
 * Parameter  : TCCP_CRO v
 * Return     : none
 * Notes      :
 * ------------------------------------------------------------------*/
static void ccp_Move(TCCP_CRO v)
{
    uint8_t count;
    uint32_t move_length = 0u;

    move_length = ccp_Make_32Bits(v.bytes.dt2, v.bytes.dt3, v.bytes.dt4, v.bytes.dt5);
    for (count = 0u; count < move_length; count++)
    {
        *(uint8_t *)CCP_MTA[1] = (*(uint8_t *)CCP_MTA[0]);
        CCP_MTA[1]++;
        CCP_MTA[0]++;
    }
}
// #ifdef CCP_FLASH
/*-------------------------------------------------------------------\
 *                  ccp_Program
 * Definition :
 * Parameter  : TCCP_CRO v
 * Return     : none
 * Notes      :
 * ------------------------------------------------------------------*/
static void ccp_Program(TCCP_CRO v)
{
    uint8_t size = CCP_DEFAULT_ZERO;
    // uint8_t Return_Code = E_NOT_OK;
    uint8_t Length_Available = STD_OFF;
    uint16_t *Data_Start = NULL;
    uint16_t data_array[8];

    uint16_t MTA_Temp;

    data_array[0] = v.bytes.dt0;
    data_array[1] = v.bytes.dt1;
    data_array[2] = v.bytes.dt2;
    data_array[3] = v.bytes.dt3;
    data_array[4] = v.bytes.dt4;
    data_array[5] = v.bytes.dt5;
    data_array[6] = v.bytes.dt6;
    data_array[7] = v.bytes.dt7;

#ifdef CCP_SEED_KEY
    if ((CCP_ProtectionStatus & CCP_RM_PGM) == STD_OFF)
    {
        CCP_TX_Data[1] = CCP_ACCESS_DENIED;
    }
    else
#endif
    {
        if (tx_pkg.bytes.dt0 == CCP_PROGRAM)
        {
            size = data_array[2];
            if (size <= CCP_PROGRAM_MAX_LENGTH)
            {
                Length_Available = STD_ON;
                Data_Start = (&data_array[3]);
            }
            else
            {
                tx_pkg.bytes.dt1 = CCP_OUT_OF_RANGE;
            }
        }
        else
        {
            size = CCP_PROGRAM6_LENGTH;
            Data_Start = (&data_array[2]);
            Length_Available = STD_ON;
        }

        if (Length_Available == STD_ON)
        {
            if (CCP_MTA[0] >= CCP_EEPROM_OFFSET)
            {
                MTA_Temp = CCP_MTA[0] - CCP_EEPROM_OFFSET;
                // Return_Code = Ccp_FlashWrite(Data_Start, CCP_Gloab_Address(CCP_MTA[0]),size);

                //              FnEEPROM.Write_Enable(true);
                //              FnEEPROM.Write_Bytes(MTA_Temp,Data_Start,size);
                //              FnEEPROM.Write_Enable(false);

                //            if (Return_Code == E_NOT_OK)
                //            {
                //                tx_pkg.bytes.dt1 = CCP_RESOURCE_USELESS;
                //            }
                //            else
                //            {
                //            #ifdef CCP_16BITS_ADDRESS_MODE
                //                CCP_MTA[0] += (size >> 1u);
                //            #else
                CCP_MTA[0] += size;
                //   #endif
            }
            else
            {
                tx_pkg.bytes.dt1 = CCP_RESOURCE_USELESS;
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    tx_pkg.bytes.dt3 = CCP_Extension_Address[0];
    tx_pkg.bytes.dt4 = CCP_Get_1Byte(CCP_MTA[0]);
    tx_pkg.bytes.dt5 = CCP_Get_2Byte(CCP_MTA[0]);
    tx_pkg.bytes.dt6 = CCP_Get_3Byte(CCP_MTA[0]);
    tx_pkg.bytes.dt7 = CCP_Get_4Byte(CCP_MTA[0]);
}
// #endif
//------------------------------------------------------------------------------------------------------/
//------------------------------------------------------------------------------------------------------/
//------------------------------------------------------------------------------------------------------/
/*-------------------------------------------------------------------\
 *                  ccp_Read_CRO_Proc
 * Definition : Read CRO Message From CAN interface
 * Parameter_1: Uint64 tmp          ///recived 64bits data
 * Parameter_2: TCCP_CRO *p
 * Return     : none
 * Notes      : put it into the ccp_read function.
 * ------------------------------------------------------------------*/
void ccp_Read_CRO_Proc(uint64_t Data)
{
    TCCP_CRO v;
    _Bool flg_reply = 1;

    v.all = fun.Uint64_ToIntel(Data); // convert received 64bits data to intel(RAM) format by bytes' index.

    tx_pkg.bytes.dt0 = 0xFF;      // PID
    tx_pkg.bytes.dt1 = 0;         // ERR
    tx_pkg.bytes.dt2 = v.bit.ctr; // CTR
    //----------------------------------------------//
    switch (v.bit.cmd)
    {
    /* Basic Commands */
    case CCP_TEST: // test connection status
    case CCP_CONNECT:
    {
        flg_reply = ccp_Test_Connect(v);
    }
    break;
    case CCP_SET_MTA:
    {
        ccp_Set_MTA(v);
    }
    break;
    case CCP_DNLOAD:
    case CCP_DNLOAD6:
    {
        ccp_Download(v);
    }
    break;
    case CCP_SHORT_UPLOAD:
    case CCP_UPLOAD:
    {
        ccp_Upload(v);
    }
    break;

    case CCP_START_STOP:
    {
        ccp_Start_Stop(v);
    }
    break;
    case CCP_DISCONNECT:
    {
        ccp_DisConnect(v);
    }
    break;
    case CCP_START_STOP_ALL:
    {
        ccp_Start_Stop_All(v);
    }
    break;
    case CCP_GET_DAQ_SIZE:
    {
        ccp_Get_DAQ_Size(v);
    }
    break;
    case CCP_SET_DAQ_PTR:
    {
        ccp_Set_DAQ_Ptr(v);
    }
    break;
    case CCP_WRITE_DAQ:
    {
        ccp_Write_DAQ(v);
    }
    break;
    case CCP_EXCHANGE_ID:
    {
        ccp_Exchange_ID(v);
    }
    break;
    case CCP_GET_CCP_VERSION:
    {
        ccp_Get_Version();
    }
    break;

    /* Optional Commands */
#ifdef CCP_CAL_PAGE
    case CCP_SET_CAL_PAGE:
    {
        ccp_Set_Cal_Page(v);
    }
    break;
    case CCP_GET_CAL_PAGE:
    {

        ccp_Get_Cal_Page();
    }
    break;
#endif
    case CCP_SET_S_STATUS:
    {
        ccp_Set_S_Status(v);
    }
    break;
    case CCP_GET_S_STATUS:
    {
        ccp_Get_S_Status();
    }
    break;

    case CCP_BUILD_CHKSUM:
    {
        ccp_Build_Checksum(v);
    }
    break;

    case CCP_CLEAR_MEMORY:
    {
        ccp_Clear_Memory(v);
    }
    break;

#ifdef CCP_SEED_KEY
    case CCP_GET_SEED:
    {
        ccp_Get_Seed(v);
    }
    break;
    case CCP_UNLOCK:
    {
        ccp_Unlock(v);
    }
    break;
#endif
    case CCP_MOVE:
    {
        ccp_Move(v);
    }
    break;
    case CCP_DIAG_SERVICE:
    {
    }
    break;
    case CCP_ACTION_SERVICE:
    {
    }
    break;
    case CCP_PROGRAM:
    case CCP_PROGRAM6:
    {
        ccp_Program(v);
    }
    break;
    }

    if (flg_reply)
        ccp_DTO_Transmit();
}
