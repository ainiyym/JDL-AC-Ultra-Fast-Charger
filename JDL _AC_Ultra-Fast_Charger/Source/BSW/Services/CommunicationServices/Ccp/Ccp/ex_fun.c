/*
 * ex_fun.c
 *
 *  Created on: 2022-9-2-
 *      Author: Lis
 */

#include "ex_fun.h"

#ifdef _FLASH
// #pragma DATA_SECTION(fun, "ramdata");
#pragma CODE_SECTION(sum_of_average, ".TI.ramfunc");
#endif

struct TFun fun = FUN_DEFAULT;
//--------------------------------------------------------------//
void uint64_ToArray(uint64_t value, uint16_t *buf)
{
   int i = 0;
   uint16_t tmp = 0;

   for (i = 7; i >= 0; i--)
   {
      tmp = (value >> i * 8) & 0x000000FF;
      buf[7 - i] = tmp;
   }
}

uint64_t array_ToUint64(uint16_t *buf)
{
   int      i=0;
   uint64_t tmp=0;

   for(i=0;i<8;i++)
   {
       tmp=tmp<<8;
       tmp+=*buf;
       buf++;
   }
   return(tmp);
}

uint64_t Uint64_ToIntel(uint64_t value)
{
   int i = 0;
   uint64_t des = 0;

   for (i = 0; i < 8; i++)
   {
      des <<= 8;
      des |= (value >> 8 * i) & 0x000000FF;
      ;
   }
   return des;
}

uint16_t uint16_ToMotorola(uint16_t value,uint16_t len)
{
    uint16_t tmp;
   tmp=((value&0x00FF)<<8-(16-len));
   tmp+=((value&0xFF00)>>8);
   return tmp;
}

uint16_t motorola_ToUint16(uint16_t value,uint16_t len)
{
    uint16_t tmp;
   tmp=((value&0x00FF)<<8);
   tmp+=(value>>8-(16-len));
   return tmp;
}

uint16_t motorola_GetValueByPos(uint64_t value,int16_t start,int16_t len)
{
    uint16_t tmp;
    int16_t i=0;
    uint32_t x=0x01;

    tmp=value>>(64-(start+len));

    for(i=0;i<len-1;i++)
    {
       x<<=1;
       x|=0x01;
    }
    tmp&=x;
   //  tmp<<=start;
    //  tmp>>=start;
    return tmp;
}

int16_t motorola_GetValueByPos_signed(uint64_t value,int16_t start,int16_t len)
{
    int16_t tmp=0;
    int16_t sig=0;

    sig=1<<(len-1);

    tmp=motorola_GetValueByPos(value,start,len);

    if ((tmp&sig)!=0)
    {
       tmp&=~sig;
       tmp=-tmp;
    }

    return tmp;
}

void  motorola_SetValueByPos(uint64_t *src,uint64_t des,uint16_t start,uint16_t len)
{
    uint64_t tmp=0;

    tmp=des&(0xFFFFFFFF>>(32-len));

    tmp<<=(64-start-len);

    *src|=tmp;
}


//-----------------------------------------------------//
//           long exfn_pow(int x,int time)
//-----------------------------------------------------//
long exfn_pow(int16_t x,int16_t time)
{
   int i;
   long tmp=1;

   for(i=0;i<time;i++)
   {
      tmp*=x;
   }
   return tmp;
}


//-----------------------------------------------------//
//              int GetLen_long(long value)
//-----------------------------------------------------//
int16_t GetLen_long(long value)
{
    int16_t i=0,len=1;
  //0xFFFFFFFF=4294967295
  long tmp;

  tmp=abs(value);

  for(i=10;i>=2;i--)
  {
     if ((tmp % exfn_pow(10,i)) > exfn_pow(10,i-1))
     {
         len=i;
     }else
     {
         break;
     }
  }

  return len;
}

//-----------------------------------------------------------------//
void uint32_bytes(uint32_t val,uint8_t *dt)
{
   TDataToBytes tmp;

   tmp.val_uint32=val;

   *dt=tmp.bit.dt0;
   dt++;
   *dt=tmp.bit.dt1;
   dt++;
   *dt=tmp.bit.dt2;
   dt++;
   *dt=tmp.bit.dt3;
}
//-----------------------------------------------------------------//
uint32_t bytes_uint32(uint8_t *dt)
{
   TDataToBytes tmp;

   tmp.bit.dt0=*dt;
   dt++;
   tmp.bit.dt1=*dt;
   dt++;
   tmp.bit.dt2=*dt;
   dt++;
   tmp.bit.dt3=*dt;

   return tmp.val_uint32;
}
//-----------------------------------------------------------------//
float bytes_float(uint8_t *dt)
{
   TDataToBytes tmp;

   tmp.bit.dt0=*dt;
   dt++;
   tmp.bit.dt1=*dt;
   dt++;
   tmp.bit.dt2=*dt;
   dt++;
   tmp.bit.dt3=*dt;

   return tmp.val_float;
}

//------------------------------------------------------------------//
void sum_of_average(long *sum,int *array,long dt,int idx)
{
    array+=idx;

    *sum = *sum + dt - *array;
    *array = dt;
}

//------------------------------------------------------------------//
long _IQnToIQm(long val,uint8_t n,uint8_t m)
{
    if (n<=m)
    {
       return ((long) (val) << (m - n));
    }else{
       return ((long) (val) >> (m - n));
    }
}



