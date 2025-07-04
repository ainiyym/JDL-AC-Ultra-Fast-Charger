#ifndef MCAL_WDG_CFG_H
#define MCAL_WDG_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================= Includes ========================= */
/* Add necessary includes here */

/* ========================= Macros =========================== */
#define MCAL_WDG_ENABLED        (0)

/* ========================= Types ============================ */
/* Add type definitions here */

/* ========================= Function Prototypes ============== */
extern void Mcal_Iwdg_Init(void);
#if (MCAL_WDG_ENABLED)
extern void Mcal_Iwdg_Feedback(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* MCAL_WDG_CFG_H */
