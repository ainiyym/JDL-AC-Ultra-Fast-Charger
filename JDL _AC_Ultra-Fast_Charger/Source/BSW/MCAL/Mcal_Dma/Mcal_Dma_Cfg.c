/*
 * Mcal_Dma.c
 * DMA driver source file
 */

#include "dma.h"

/* Static function prototypes */
static void Mcal_Dma_InternalInit(void);

/* DMA driver initialization */
void Mcal_Dma_Init(void)
{
    /* Internal initialization */
    Mcal_Dma_InternalInit();
}

/* DMA interrupt handler */
void Mcal_Dma_IRQHandler(void)
{
    /* TODO: Implement DMA interrupt handling */
}

/* Internal initialization function */
static void Mcal_Dma_InternalInit(void)
{
   MX_DMA_Init();
}
