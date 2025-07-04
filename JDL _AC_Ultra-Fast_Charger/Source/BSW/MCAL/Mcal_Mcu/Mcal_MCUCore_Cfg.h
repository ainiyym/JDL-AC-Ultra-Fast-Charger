#ifndef  __MCAL_MCUCORE_CFG_H__
#define  __MCAL_MCUCORE_CFG_H__

#define MCAL_ENTER_CRITICAL_AREA() 	__set_PRIMASK(1)//vPortEnterCritical()
#define MCAL_EXIT_CRITICAL_AREA() 	__set_PRIMASK(0)//vPortExitCritical()

extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);
#endif
