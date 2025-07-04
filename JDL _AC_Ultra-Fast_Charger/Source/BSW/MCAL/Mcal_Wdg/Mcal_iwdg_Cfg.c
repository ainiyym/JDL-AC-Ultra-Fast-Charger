/*
 * Mcal_iwdg_Cfg.c
 * Configuration source file for CAN MCAL module
 */
#include "iwdg.h"

void Mcal_Iwdg_Init(void)
{
#if (MCAL_WDG_ENABLED)
  MX_IWDG_Init();
#endif
}

#if (MCAL_WDG_ENABLED)
void Mcal_Iwdg_Feedback(void)
{
  HAL_IWDG_Refresh(&hiwdg);
}
#endif
/* USER CODE END 1 */
