/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Interrupt_cfg.h                                           */
/*                                                                            */
/*    $Revision:: 256                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-28 17:01:23 +0800 (周二, 28 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Interrupt module user configuration for LPC1517 platform  */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef INTERRUPT_CFG_H
#define INTERRUPT_CFG_H

#include "extint.h"

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
                                                            
/* Map application interrupt ID to assigned peripheral interrupt IRQ ID:      */
/* 3 bits for preempt priority: Highest 0-7 Lowest 
   0 bits for subpriority */
#define NVIC_PRIOGRP_SETTING  (NVIC_PRIGRP_3)

/*
 *  Enable, priority and ISR configuration table
 *  Cortex-M3 Processor Exceptions Numbers ****
*/

/* Cortex-M3 Reset Vector, invoked on Power up and warm reset (Reset_IRQn) */
#undef  ISR_RESET

/* Cortex-M3 Non maskable Interrupt (NonMaskableInt_IRQn) */
#define IRQ_NMI                 IRQ_DISABLE
#undef  SRC_NMI                 /* IRQn_T (0-31)*/
#undef  ISR_NMI                 /* ISR of defined NVIC SRC_NMI */

/* Cortex-M3 Hard Fault Interrupt (HardFault_IRQn)  */
/* Hard fault has fixed priority, but can be configured to enable/disable */
#define IRQ_HARDFT              IRQ_ENABLE
#undef  ISR_HARDFT           

/* Cortex-M3 Memory Management Interrupt (MemoryManagement_IRQn) */
#define IRQ_MEM                 IRQ_DISABLE
#define IRQ_MEM_PRIO            COMBINED_PRIORITY_VALUE(0)
#undef  ISR_MEM

/* Cortex-M3 Bus Fault Interrupt (BusFault_IRQn)  */
#define IRQ_BUSFT               IRQ_ENABLE
#define IRQ_BUSFT_PRIO          COMBINED_PRIORITY_VALUE(0)
#undef  ISR_BUSFT       

/* Cortex-M3 Usage Fault Interrupt (UsageFault_IRQn) */
#define IRQ_USAGE               IRQ_ENABLE
#define IRQ_USAGE_PRIO          COMBINED_PRIORITY_VALUE(0)
#undef  ISR_USAGE        

/* Cortex-M3 SV Call Interrupt (SVCall_IRQn)  */
#define IRQ_SVC                 IRQ_DISABLE
#define IRQ_SVC_PRIO            COMBINED_PRIORITY_VALUE(0)
#undef  ISR_SVC

/* Cortex-M3 Debug Monitor Interrupt (DebugMonitor_IRQn) */
//Reserved for debug module, Application won't enable it

/* Cortex-M3 Pend SV Interrupt (PendSV_IRQn)  */
#define IRQ_PSV                 IRQ_DISABLE
#define IRQ_PSV_PRIO            COMBINED_PRIORITY_VALUE(0)
#undef  ISR_PSV  

/* Cortex-M3 System Tick Interrupt (SysTick_IRQn) */
#define IRQ_SYSTICK             IRQ_ENABLE
#define IRQ_SYSTICK_PRIO        COMBINED_PRIORITY_VALUE(5)
#define ISR_SYSTICK             SYSTICK_vISR

/*
****  Interrupt enable and priority table ****
*/
/* Window WatchDog Interrupt (WDT_IRQn) */
#define IRQ_WDT                 IRQ_ENABLE
#define IRQ_WDT_PRIO            COMBINED_PRIORITY_VALUE(0)
#define ISR_WDT                 WATCHDOG_WRN_vISR

/* Brown-Out detection Interrupt (BOD_IRQn) */
#define IRQ_BOD                 IRQ_ENABLE
#define IRQ_BOD_PRIO            COMBINED_PRIORITY_VALUE(0)
#undef  ISR_BOD

/* FLASH Interrupt (FMC_IRQn) */
#define IRQ_FLASH               IRQ_DISABLE
#define IRQ_FLASH_PRIO          COMBINED_PRIORITY_VALUE(0)
#undef  ISR_FLASH

/* EEPROM controller interrupt (FLASHEEPROM_IRQn) */
#define IRQ_EEPROM              IRQ_DISABLE
#define IRQ_EEPROM_PRIO         COMBINED_PRIORITY_VALUE(0)
#undef  ISR_EEPROM

/* DMA Interrupt (DMA_IRQn) */
#define IRQ_DMA                 IRQ_ENABLE
#define IRQ_DMA_PRIO            COMBINED_PRIORITY_VALUE(4)
#define ISR_DMA                 DMA_vISR

/* GPIO group 0 Interrupt (GINT0_IRQn) */
#define IRQ_GINT0               IRQ_DISABLE
#define IRQ_GINT0_PRIO          COMBINED_PRIORITY_VALUE(6)
#undef  ISR_GINT0

/* GPIO group 1 Interrupt (GINT1_IRQn) */
#define IRQ_GINT1               IRQ_DISABLE
#define IRQ_GINT1_PRIO          COMBINED_PRIORITY_VALUE(6)
#undef  ISR_GINT1

/* Pin Interrupt 0 (PIN_INT0_IRQn) */
#define IRQ_PININT0             IRQ_ENABLE
#define IRQ_PININT0_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT0             PININT0_vISR

/* Pin Interrupt 1 (PIN_INT1_IRQn) */
#define IRQ_PININT1             IRQ_ENABLE
#define IRQ_PININT1_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT1             PININT1_vISR

/* Pin Interrupt 2 (PIN_INT2_IRQn) */
#define IRQ_PININT2             IRQ_ENABLE
#define IRQ_PININT2_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT2             PININT2_vISR

/* Pin Interrupt 3 (PIN_INT3_IRQn) */
#define IRQ_PININT3             IRQ_ENABLE
#define IRQ_PININT3_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT3             PININT3_vISR

/* Pin Interrupt 4 (PIN_INT4_IRQn) */
#define IRQ_PININT4             IRQ_ENABLE
#define IRQ_PININT4_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT4             PININT4_vISR

/* Pin Interrupt 5 (PIN_INT5_IRQn) */
#define IRQ_PININT5             IRQ_DISABLE
#define IRQ_PININT5_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT5             PININT5_vISR

/* Pin Interrupt6 (PIN_INT6_IRQn) */
#define IRQ_PININT6             IRQ_DISABLE
#define IRQ_PININT6_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT6             PININT6_vISR

/* Pin Interrupt7 (PIN_INT7_IRQn) */
#define IRQ_PININT7             IRQ_DISABLE
#define IRQ_PININT7_PRIO        COMBINED_PRIORITY_VALUE(6)
#define ISR_PININT7             PININT7_vISR

/* RIT Timer Interrupt (RITIMER_IRQn) */
#define IRQ_RITIMER             IRQ_ENABLE
#define IRQ_RITIMER_PRIO        COMBINED_PRIORITY_VALUE(4)
#define ISR_RITIMER             RIT_vISR

/* SCT0 interrupt (SCT0_IRQn) */
#define IRQ_SCT0                IRQ_ENABLE
#define IRQ_SCT0_PRIO           COMBINED_PRIORITY_VALUE(4)
#define ISR_SCT0                SCT0_vISR

/* SCT1 interrupt (SCT1_IRQn) */
#define IRQ_SCT1                IRQ_DISABLE
#define IRQ_SCT1_PRIO           COMBINED_PRIORITY_VALUE(4)
#define ISR_SCT1                SCT1_vISR

/* SCT2 interrupt (SCT2_IRQn) */
#define IRQ_SCT2                IRQ_DISABLE
#define IRQ_SCT2_PRIO           COMBINED_PRIORITY_VALUE(4)
#define ISR_SCT2                SCT2_vISR

/* SCT3 interrupt (SCT3_IRQn) */
#define IRQ_SCT3                IRQ_DISABLE
#define IRQ_SCT3_PRIO           COMBINED_PRIORITY_VALUE(4)
#define ISR_SCT3                SCT3_vISR

/* MRT interrupt (MRT_IRQn) */
#define IRQ_MRT                 IRQ_ENABLE
#define IRQ_MRT_PRIO            COMBINED_PRIORITY_VALUE(4)
#define ISR_MRT                 MRT_vISR

/* UART0 interrupt (UART0_IRQn) */
#define IRQ_UART0               IRQ_DISABLE
#define IRQ_UART0_PRIO          COMBINED_PRIORITY_VALUE(4)
#define UART0_vISR

/* UART1 interrupt (UART1_IRQn) */
#define IRQ_UART1               IRQ_DISABLE
#define IRQ_UART1_PRIO          COMBINED_PRIORITY_VALUE(4)
#define UART1_vISR

/* UART2 interrupt (UART2_IRQn) */
#define IRQ_UART2               IRQ_DISABLE
#define IRQ_UART2_PRIO          COMBINED_PRIORITY_VALUE(4)
#define UART2_vISR

/* I2C interrupt (I2C_IRQn) */
#define IRQ_I2C                 IRQ_DISABLE
#define IRQ_I2C_PRIO            COMBINED_PRIORITY_VALUE(4)
#undef  ISR_I2C 

/* SPI0 interrupt (SPI0_IRQn) */
#define IRQ_SPI0                IRQ_ENABLE
#define IRQ_SPI0_PRIO           COMBINED_PRIORITY_VALUE(3)
#define ISR_SPI0                SPI0_vISR

/* SPI1 interrupt (SPI1_IRQn) */
#define IRQ_SPI1                IRQ_ENABLE
#define IRQ_SPI1_PRIO           COMBINED_PRIORITY_VALUE(3)
#define ISR_SPI1                SPI1_vISR

/* CAN interrupt (CAN_IRQn) */
#define IRQ_CAN                 IRQ_DISABLE
#define IRQ_CAN_PRIO            COMBINED_PRIORITY_VALUE(4)
#undef  ISR_CAN 

/* USB interrupt (USB_IRQn) */
#define IRQ_USB                 IRQ_ENABLE
#define IRQ_USB_PRIO            COMBINED_PRIORITY_VALUE(4)
#undef  ISR_USB 

/* USB FIQ interrupt (USB_FIQ_IRQn) */
#define IRQ_USB_FIQ             IRQ_DISABLE
#define IRQ_USB_FIQ_PRIO        COMBINED_PRIORITY_VALUE(4)
#undef  ISR_USB_FIQ 

/* USB Wake-up interrupt Interrupt (USB_WAKEUP_IRQn) */
#define IRQ_USB_WAKEUP          IRQ_DISABLE
#define IRQ_USB_WAKEUP_PRIO     COMBINED_PRIORITY_VALUE(6)
#undef  ISR_USB_WAKEUP 

/* ADC0_A sequencer Interrupt (ADC0_SEQA_IRQn) */
#define IRQ_ADC0_SEQA           IRQ_DISABLE
#define IRQ_ADC0_SEQA_PRIO      COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC0_SEQA           ADC0_SeqA_vISR

/* ADC0_B sequencer Interrupt (ADC0_SEQB_IRQn) */
#define IRQ_ADC0_SEQB           IRQ_DISABLE
#define IRQ_ADC0_SEQB_PRIO      COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC0_SEQB           ADC0_SeqB_vISR

/* ADC0 threshold compare interrupt (ADC0_THCMP) */
#define IRQ_ADC0_THCMP          IRQ_DISABLE
#define IRQ_ADC0_THCMP_PRIO     COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC0_THCMP          ADC0_THCMP_vISR

/* ADC0 overrun interrupt (ADC0_OVR) */
#define IRQ_ADC0_OVR            IRQ_DISABLE
#define IRQ_ADC0_OVR_PRIO       COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC0_OVR            ADC0_OVR_vISR

/* ADC1_A sequencer Interrupt (ADC1_SEQA_IRQn) */
#define IRQ_ADC1_SEQA           ENABLE
#define IRQ_ADC1_SEQA_PRIO      COMBINED_PRIORITY_VALUE(4)
#define ISR_ADC1_SEQA           ADC1_SeqA_vISR

/* ADC1_B sequencer Interrupt (ADC1_SEQB_IRQn) */
#define IRQ_ADC1_SEQB           IRQ_DISABLE
#define IRQ_ADC1_SEQB_PRIO      COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC1_SEQB           ADC1_SeqB_vISR

/* ADC1 threshold compare interrupt (ADC1_THCMP) */
#define IRQ_ADC1_THCMP          IRQ_DISABLE
#define IRQ_ADC1_THCMP_PRIO     COMBINED_PRIORITY_VALUE(5)
#define ISR_ADC1_THCMP          ADC1_THCMP_vISR

/* ADC1 overrun interrupt (ADC1_OVR) */
#define IRQ_ADC1_OVR            IRQ_DISABLE
#define IRQ_ADC1_OVR_PRIO       COMBINED_PRIORITY_VALUE(4)
#define ISR_ADC1_OVR            ADC1_OVR_vISR

/* DAC interrupt (DAC_IRQ) */
#define IRQ_DAC                 IRQ_DISABLE
#define IRQ_DAC_PRIO            COMBINED_PRIORITY_VALUE(6)
#define ISR_DAC                 DAC_vISR

/* Analog comparator 0 interrupt  (CMP0_IRQn) */
#define IRQ_ACMP0               IRQ_ENABLE
#define IRQ_ACMP0_PRIO          COMBINED_PRIORITY_VALUE(6)
#define ISR_ACMP0               ACMP0_vISR

/* Analog comparator 1 interrupt  (CMP1_IRQn) */
#define IRQ_ACMP1               IRQ_ENABLE
#define IRQ_ACMP1_PRIO          COMBINED_PRIORITY_VALUE(6)
#define ISR_ACMP1               ACMP1_vISR

/* Analog comparator 2 interrupt  (CMP2_IRQn) */
#define IRQ_ACMP2               IRQ_ENABLE
#define IRQ_ACMP2_PRIO          COMBINED_PRIORITY_VALUE(6)
#define ISR_ACMP2               ACMP2_vISR

/* Analog comparator 3 interrupt  (CMP3_IRQn) */
#define IRQ_ACMP3               IRQ_ENABLE
#define IRQ_ACMP3_PRIO          COMBINED_PRIORITY_VALUE(6)
#define ISR_ACMP3               ACMP3_vISR

/* QEI interrupt  (QEI_IRQn) */
#define IRQ_QEI                 IRQ_ENABLE
#define IRQ_QEI_PRIO            COMBINED_PRIORITY_VALUE(6)
#undef  ISR_QEI

/* RTC alarm interrupt  (RTC_ALARM_IRQn) */
#define IRQ_RTC_ALARM           IRQ_DISABLE
#define IRQ_RTC_ALARM_PRIO      COMBINED_PRIORITY_VALUE(7)
#undef  ISR_RTC_ALARM

/* RTC wake-up interrupt  (RTC_WAKE_IRQn) */
#define IRQ_RTC_WAKEUP          IRQ_DISABLE
#define IRQ_RTC_WAKEUP_PRIO     COMBINED_PRIORITY_VALUE(7)
#undef  ISR_RTC_WAKEUP

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

