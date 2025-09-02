#ifndef _YEECOM_XXX_H_
#define _YEECOM_XXX_H_

#include "YeeComxxx_At.h"

extern void YeeCom_SetDeviceStatus(YeeCom_AT_Oob_enum id, uint8_t status);
extern void YeeCom_Init(void);
extern void YeeCom_MainFunc(void);
#endif
