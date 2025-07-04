/*
 * ex_fun.h
 *
 *  Created on:
 *      Author:
 */

#ifndef INCLUDE_EX_FUN_H_
#define INCLUDE_EX_FUN_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
//---------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------//
typedef union
{
  uint32_t val_uint32;
  int32_t val_int32;
  float   val_float;
  struct{
      uint32_t dt0:8;
      uint32_t dt1:8;
      uint32_t dt2:8;
      uint32_t dt3:8;
  }bit;
}TDataToBytes;

typedef union
{
  int32_t   val_int32;
  struct{
      uint32_t DH:16;
      uint32_t DL:16;
  }bit;
}TInt32ToInt16;

typedef union
{
  uint16_t   val_int16;
  struct{
      uint32_t DH:8;
      uint32_t DL:8;
  }bit;
}TUintToBytes;
//---------------------------------------------------------------------------------//

#define RANGE_CHECK(val,cmp,rate)  ((val>=cmp*(1.0-(float)rate))&&(val<=cmp*(1.0+(float)rate)))
//---------------------------------------------------------------------------------//

extern void uint64_ToArray(uint64_t value,uint16_t *buf);
extern uint64_t array_ToUint64(uint16_t *buf);
extern uint64_t Uint64_ToIntel(uint64_t value);
extern uint16_t uint16_ToMotorola(uint16_t value,uint16_t len);
extern uint16_t motorola_ToUint16(uint16_t value,uint16_t len);
extern uint16_t motorola_GetValueByPos(uint64_t value,int16_t start,int16_t len);
extern int16_t motorola_GetValueByPos_signed(uint64_t value,int16_t start,int16_t len);
extern void  motorola_SetValueByPos(uint64_t *src,uint64_t des,uint16_t start,uint16_t len);
extern long exfn_pow(int16_t x,int16_t time);

extern uint32_t bytes_uint32(uint8_t *dt);
extern void uint32_bytes(uint32_t val,uint8_t *dt);
extern float bytes_float(uint8_t *dt);

extern void sum_of_average(long *sum,int *array,long dt,int idx);
extern long _IQnToIQm(long val,uint8_t n,uint8_t m);
//-------------------------------------------------------------------------//
//            Interface   TFun  fun;
//-------------------------------------------------------------------------//
struct TFun{
    void   (*uint64_ToArray)(uint64_t value,uint16_t *buf);
    uint64_t (*Uint64_ToIntel)(uint64_t value);
    uint64_t (*array_ToUint64)(uint16_t *buf);
    uint16_t (*uint16_ToMotorola)(uint16_t value,uint16_t len);
    uint16_t (*motorola_ToUint16)(uint16_t value,uint16_t len);
    uint16_t (*GetValueByPos)(uint64_t value,int16_t start,int16_t len);
    void   (*SetValueByPos)(uint64_t *src,uint64_t des,uint16_t start,uint16_t len);
    int16_t    (*GetValueByPos_signed)(uint64_t value,int16_t start,int16_t len);
    long   (*pow)(int16_t x,int16_t time);
    void   (*uint32_bytes)(uint32_t val,uint8_t *dt);
    uint32_t (*bytes_uint32)(uint8_t *dt);
    float  (*bytes_float)(uint8_t *dt);
    void   (*sum_of_average)(long *sum,int *array,long dt,int idx);
    long   (*IQnToIQm)(long val,uint8_t n,uint8_t m);
};
extern struct TFun  fun;
//-------------------------------------------------------------------------//
#define FUN_DEFAULT {\
                    uint64_ToArray,\
                    Uint64_ToIntel,\
                    array_ToUint64,\
                    uint16_ToMotorola,\
                    motorola_ToUint16,\
                    motorola_GetValueByPos,\
                    motorola_SetValueByPos,\
                    motorola_GetValueByPos_signed,\
                    exfn_pow,\
                    uint32_bytes,\
                    bytes_uint32,\
                    bytes_float,\
                    sum_of_average,\
                    _IQnToIQm\
                    }
#endif /* INCLUDE_EX_FUN_H_ */
