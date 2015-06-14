/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Interrupt.h                                                 $    */
/*                                                                            */
/*    $Revision:: 354                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-26 10:28:57 +0800 (周三, 26 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level interrupt system macros and functions for       */
/*                  LPC1517 platform.                                         */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "platform.h"
#include "extint.h"
#include "io.h"
#include "interrupt_cfg.h"

/** @defgroup NVIC_15XX CHIP: LPC15xx NVIC Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/*
The table below gives the allowed values of the group priority(pre-emption) and
subpriority according to the Priority Grouping configuration.
*/
#define NVIC_PRIGRP_0  (0x7) /*!< 0 bits for preempt priority
                                    3 bits for subpriority          */
#define NVIC_PRIGRP_1  (0x6) /*!< 1 bits for preempt priority
                                    2 bits for subpriority          */
#define NVIC_PRIGRP_2  (0x5) /*!< 2 bits for preempt priority
                                    1 bits for subpriority          */
#define NVIC_PRIGRP_3  (0x4) /*!< 3 bits for preempt priority
                                    0 bits for subpriority          */                                    
/*
 * Interrrupt Configuration (Enable/Disable/Priority)
   Priority highest to lowest: 0~15
 */
#define IRQ_DISABLE (0)
#define IRQ_ENABLE  (1)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
  *interrupt set-enable
*/
/* ISER0 for IRQ 0~31 */
#define NVIC_ISER0_VALUE (((U32)IRQ_WDT         <<(U32)(WDT_IRQn))\
                         |((U32)IRQ_BOD         <<(U32)(BOD_IRQn))\
                         |((U32)IRQ_FLASH       <<(U32)(FMC_IRQn))\
                         |((U32)IRQ_EEPROM      <<(U32)(FLASHEEPROM_IRQn))\
                         |((U32)IRQ_DMA         <<(U32)(DMA_IRQn))\
                         |((U32)IRQ_GINT0       <<(U32)(GINT0_IRQn))\
                         |((U32)IRQ_GINT1       <<(U32)(GINT1_IRQn))\
                         |((U32)IRQ_PININT0     <<(U32)(PIN_INT0_IRQn))\
                         |((U32)IRQ_PININT1     <<(U32)(PIN_INT1_IRQn))\
                         |((U32)IRQ_PININT2     <<(U32)(PIN_INT2_IRQn))\
                         |((U32)IRQ_PININT3     <<(U32)(PIN_INT3_IRQn))\
                         |((U32)IRQ_PININT4     <<(U32)(PIN_INT4_IRQn))\
                         |((U32)IRQ_PININT5     <<(U32)(PIN_INT5_IRQn))\
                         |((U32)IRQ_PININT6     <<(U32)(PIN_INT6_IRQn))\
                         |((U32)IRQ_PININT7     <<(U32)(PIN_INT7_IRQn))\
                         |((U32)IRQ_RITIMER     <<(U32)(RITIMER_IRQn))\
                         |((U32)IRQ_SCT0        <<(U32)(SCT0_IRQn))\
                         |((U32)IRQ_SCT1        <<(U32)(SCT1_IRQn))\
                         |((U32)IRQ_SCT2        <<(U32)(SCT2_IRQn))\
                         |((U32)IRQ_SCT3        <<(U32)(SCT3_IRQn))\
                         |((U32)IRQ_MRT         <<(U32)(MRT_IRQn))\
                         |((U32)IRQ_UART0       <<(U32)(UART0_IRQn))\
                         |((U32)IRQ_UART1       <<(U32)(UART1_IRQn))\
                         |((U32)IRQ_UART2       <<(U32)(UART2_IRQn))\
                         |((U32)IRQ_I2C         <<(U32)(I2C_IRQn))\
                         |((U32)IRQ_SPI0        <<(U32)(SPI0_IRQn))\
                         |((U32)IRQ_SPI1        <<(U32)(SPI1_IRQn))\
                         |((U32)IRQ_CAN         <<(U32)(CAN_IRQn))\
                         |((U32)IRQ_USB         <<(U32)(USB_IRQn))\
                         |((U32)IRQ_USB_FIQ     <<(U32)(USB_FIQ_IRQn))\
                         |((U32)IRQ_USB_WAKEUP  <<(U32)(USB_WAKEUP_IRQn))\
                         |((U32)IRQ_ADC0_SEQA   <<(U32)(ADC0_SEQA_IRQn))\
                         )

/* ISER1 for IRQ 32~63 */
#define NVIC_ISER1_VALUE (((U32)IRQ_ADC0_SEQB   <<(U32)(ADC0_SEQB_IRQn - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC0_THCMP  <<(U32)(ADC0_THCMP - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC0_OVR    <<(U32)(ADC0_OVR - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC1_SEQA   <<(U32)(ADC1_SEQA_IRQn - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC1_SEQB   <<(U32)(ADC1_SEQB_IRQn - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC1_THCMP  <<(U32)(ADC1_THCMP - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ADC1_OVR    <<(U32)(ADC1_OVR - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_DAC         <<(U32)(DAC_IRQ - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ACMP0       <<(U32)(CMP0_IRQ - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ACMP1       <<(U32)(CMP1_IRQ - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ACMP2       <<(U32)(CMP2_IRQ - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_ACMP3       <<(U32)(CMP3_IRQ - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_QEI         <<(U32)(QEI_IRQn - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_RTC_ALARM   <<(U32)(RTC_ALARM_IRQn - ADC0_SEQB_IRQn))\
	                     |((U32)IRQ_RTC_WAKEUP  <<(U32)(RTC_WAKE_IRQn - ADC0_SEQB_IRQn))\
                         )



/* Final priority of interrupt including group and sub prio (group bit = 3 ,sub bit = 0) */
#define COMBINED_PRIORITY_VALUE(preem) NVIC_EncodePriority(NVIC_PRIOGRP_SETTING, preem, 0)

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief Init interrupt sources
 * @param	Nothing
 * @return	Nothing
 */
void INTERRUPT_vInit(void);

/**
 * @brief Enable NVIC interrupt 
 * @param	irqn : IRQn_Type
 * @return	Nothing
 */
STATIC INLINE void INTERRUPT_vEnable(IRQn_Type irqn)
{
	if(WDT_IRQn <= irqn){
		NVIC_ClearPendingIRQ(irqn);
    	NVIC_EnableIRQ(irqn);
	}
}

/**
 * @brief Disable NVIC interrupt 
 * @param	irqn : IRQn_Type
 * @return	Nothing
 */
STATIC INLINE void INTERRUPT_vDisable(IRQn_Type irqn)
{
    if(WDT_IRQn <= irqn){
    	NVIC_DisableIRQ(irqn);
	}
}

/**
 * @brief Enable all interrupts
 * @param	irqn : IRQn_Type
 * @return	Nothing
 */
STATIC INLINE void INTERRUPT_vEnableAll(void)
{
	WRITE_REG(NVIC->ISER[0], NVIC_ISER0_VALUE);
	WRITE_REG(NVIC->ISER[1], NVIC_ISER1_VALUE);	
}

/**
 * @brief Disable all interrupts 
 * @param	irqn : IRQn_Type
 * @return	Nothing
 */
STATIC INLINE void INTERRUPT_vDisableAll(void)
{
	WRITE_REG(NVIC->ISER[0], 0);
	WRITE_REG(NVIC->ISER[1], 0);	
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


