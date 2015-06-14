/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Interrupt.c                                          $    */
/*                                                                            */
/*    $Revision:: 357                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-26 13:12:09 +0800 (周三, 26 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros and functions for NVIC                   */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "platform.h"

#include "inmux.h"
#include "interrupt.h"

#include "stdtypes.h"

#include "pwrctrl.h"
#include "cdc_vcom.h"
#include "ssi.h"
#include "package.h"
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/** Pointer to User-Specific IRQ Function  */
#define DECLARE_IRQ_HANDLER(_Func_) extern void _Func_(void);

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE FUNCTION PROTOTYPES                       */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC FUNCTION PROTOTYPES                        */
/******************************************************************************/
/**
  * Declare IRQ according to configurations in interrupt_cfg.h 
**/
/* Cortex system user specific handler  */
/* Cortex-M3 Hard Fault Interrupt (HardFault_IRQn)  */
#ifdef ISR_HARDFT
	DECLARE_IRQ_HANDLER(ISR_HARDFT)
#endif
/* Cortex-M3 Memory Management Interrupt (MemoryManagement_IRQn) */
#ifdef ISR_MEM
	DECLARE_IRQ_HANDLER(ISR_MEM)
#endif
/* Cortex-M3 Bus Fault Interrupt (BusFault_IRQn)  */
#ifdef ISR_BUSFT
	DECLARE_IRQ_HANDLER(ISR_BUSFT)
#endif
/* Cortex-M3 Usage Fault Interrupt (UsageFault_IRQn) */
#ifdef ISR_USAGE
	DECLARE_IRQ_HANDLER(ISR_USAGE)
#endif
/* Cortex-M3 SV Call Interrupt (SVCall_IRQn)  */
#ifdef ISR_SVC
	DECLARE_IRQ_HANDLER(ISR_SVC)
#endif
/* Cortex-M3 Pend SV Interrupt (PendSV_IRQn)  */
#ifdef ISR_PSV
	DECLARE_IRQ_HANDLER(ISR_PSV)
#endif
/* Cortex-M3 System Tick Interrupt (SysTick_IRQn) */
#ifdef ISR_SYSTICK
	DECLARE_IRQ_HANDLER(ISR_SYSTICK)
#endif
/* NVIC user specific handler  */
/* Window WatchDog Interrupt (WWDG_IRQn) */
#ifdef ISR_WDT
	DECLARE_IRQ_HANDLER(ISR_WDT)
#endif
/* Brown-Out detection Interrupt (BOD) */
#ifdef ISR_BOD
	DECLARE_IRQ_HANDLER(ISR_BOD)
#endif
/* FLASH Interrupt (FMC) */
#ifdef ISR_FLASH
	DECLARE_IRQ_HANDLER(ISR_FLASH)
#endif
/* EEPROM controller interrupt (FLASHEEPROM_IRQn) */
#ifdef ISR_EEPROM
	DECLARE_IRQ_HANDLER(ISR_EEPROM)
#endif
/* DMA Interrupt (DMA_IRQn) */
#ifdef ISR_DMA
	DECLARE_IRQ_HANDLER(ISR_DMA)
#endif
/* GPIO group 0 Interrupt (GINT0_IRQn) */
#ifdef ISR_GINT0
	DECLARE_IRQ_HANDLER(ISR_GINT0)
#endif
/* GPIO group 1 Interrupt (GINT1_IRQn) */
#ifdef ISR_GINT1
	DECLARE_IRQ_HANDLER(ISR_GINT1)
#endif
/* Pin Interrupt 0 (PIN_INT0_IRQn) */
#ifdef ISR_PININT0
	DECLARE_IRQ_HANDLER(ISR_PININT0)
#endif
/* Pin Interrupt 1 (PIN_INT1_IRQn) */
#ifdef ISR_PININT1
	DECLARE_IRQ_HANDLER(ISR_PININT1)
#endif
/* Pin Interrupt 2 (PIN_INT2_IRQn) */
#ifdef ISR_PININT2
	DECLARE_IRQ_HANDLER(ISR_PININT2)
#endif
/* Pin Interrupt 3 (PIN_INT3_IRQn) */
#ifdef ISR_PININT3
	DECLARE_IRQ_HANDLER(ISR_PININT3)
#endif
/* Pin Interrupt 4 (PIN_INT4_IRQn) */
#ifdef ISR_PININT4
	DECLARE_IRQ_HANDLER(ISR_PININT4)
#endif
/* Pin Interrupt 5 (PIN_INT5_IRQn) */
#ifdef ISR_PININT5
	DECLARE_IRQ_HANDLER(ISR_PININT5)
#endif
/* Pin Interrupt6 (PIN_INT6_IRQn) */
#ifdef ISR_PININT6
	DECLARE_IRQ_HANDLER(ISR_PININT6)
#endif
/* Pin Interrupt7 (PIN_INT7_IRQn) */
#ifdef ISR_PININT7
	DECLARE_IRQ_HANDLER(ISR_PININT7)
#endif
/* RIT Timer Interrupt (RITIMER_IRQn) */
#ifdef ISR_RITIMER
	DECLARE_IRQ_HANDLER(ISR_RITIMER)
#endif
/* SCT0 interrupt (SCT0_IRQn) */
#ifdef ISR_SCT0
	DECLARE_IRQ_HANDLER(ISR_SCT0)
#endif
/* SCT1 interrupt (SCT1_IRQn) */
#ifdef ISR_SCT1
	DECLARE_IRQ_HANDLER(ISR_SCT1)
#endif
/* SCT2 interrupt (SCT2_IRQn) */
#ifdef ISR_SCT2
	DECLARE_IRQ_HANDLER(ISR_SCT2)
#endif
/* SCT3 interrupt (SCT3_IRQn) */
#ifdef ISR_SCT3
	DECLARE_IRQ_HANDLER(ISR_SCT3)
#endif
/* MRT interrupt (MRT_IRQn) */
#ifdef ISR_MRT
	DECLARE_IRQ_HANDLER(ISR_MRT)
#endif
/* UART0 interrupt (UART0_IRQn) */
#ifdef ISR_UART0
	DECLARE_IRQ_HANDLER(ISR_UART0)
#endif
/* UART1 interrupt (UART1_IRQn) */
#ifdef ISR_UART1
	DECLARE_IRQ_HANDLER(ISR_UART1)
#endif
/* UART2 interrupt (UART2_IRQn) */
#ifdef ISR_UART2
	DECLARE_IRQ_HANDLER(ISR_UART2)
#endif
/* I2C interrupt (I2C_IRQn) */
#ifdef ISR_I2C
	DECLARE_IRQ_HANDLER(ISR_I2C)
#endif
/* SPI0 interrupt (SPI0_IRQn) */
#ifdef ISR_SPI0
	DECLARE_IRQ_HANDLER(ISR_SPI0)
#endif
/* SPI1 interrupt (SPI1_IRQn) */
#ifdef ISR_SPI1
	DECLARE_IRQ_HANDLER(ISR_SPI1)
#endif
/* CAN interrupt (CAN_IRQn) */
#ifdef ISR_CAN
	DECLARE_IRQ_HANDLER(ISR_CAN)
#endif
/* USB interrupt (USB_IRQn) */
#ifdef ISR_USB
	DECLARE_IRQ_HANDLER(ISR_USB)
#endif
/* USB FIQ interrupt (USB_FIQ_IRQn) */
#ifdef ISR_USB_FIQ
	DECLARE_IRQ_HANDLER(ISR_USB_FIQ)
#endif
/* USB wake-up interrupt Interrupt (USB_WAKEUP_IRQn) */
#ifdef ISR_USB_WAKEUP
	DECLARE_IRQ_HANDLER(ISR_USB_WAKEUP)
#endif
/* ADC0_A sequencer Interrupt (ADC0_SEQA_IRQn) */
#ifdef ISR_ADC0_SEQA
	DECLARE_IRQ_HANDLER(ISR_ADC0_SEQA)
#endif
/* ADC0_B sequencer Interrupt (ADC0_SEQB_IRQn) */
#ifdef ISR_ADC0_SEQB
	DECLARE_IRQ_HANDLER(ISR_ADC0_SEQB)
#endif
/* ADC0 threshold compare interrupt (ADC0_THCMP) */
#ifdef ISR_ADC0_THCMP
	DECLARE_IRQ_HANDLER(ISR_ADC0_THCMP)
#endif
/* ADC0 overrun interrupt  (ADC0_OVR) */
#ifdef ISR_ADC0_OVR
	DECLARE_IRQ_HANDLER(ISR_ADC0_OVR)
#endif
/* ADC1_A sequencer Interrupt (ADC1_SEQA_IRQn) */
#ifdef ISR_ADC1_SEQA
	DECLARE_IRQ_HANDLER(ISR_ADC1_SEQA)
#endif
/* ADC1_B sequencer Interrupt (ADC1_SEQB_IRQn) */
#ifdef ISR_ADC1_SEQB
	DECLARE_IRQ_HANDLER(ISR_ADC1_SEQB)
#endif
/* ADC1 threshold compare interrupt (ADC1_THCMP) */
#ifdef ISR_ADC1_THCMP
	DECLARE_IRQ_HANDLER(ISR_ADC1_THCMP)
#endif
/* ADC1 overrun interrupt  (ADC1_OVR) */
#ifdef ISR_ADC1_OVR
	DECLARE_IRQ_HANDLER(ISR_ADC1_OVR)
#endif
/* DAC interrupt  (DAC_IRQ) */
#ifdef ISR_DAC
	DECLARE_IRQ_HANDLER(ISR_DAC)
#endif
/* Analog comparator 0 interrupt  (CMP0_IRQn) */
#ifdef ISR_ACMP0
	DECLARE_IRQ_HANDLER(ISR_ACMP0)
#endif
/* Analog comparator 1 interrupt  (CMP1_IRQn) */
#ifdef ISR_ACMP1
	DECLARE_IRQ_HANDLER(ISR_ACMP1)
#endif
/* Analog comparator 2 interrupt  (CMP2_IRQn) */
#ifdef ISR_ACMP2
	DECLARE_IRQ_HANDLER(ISR_ACMP2)
#endif
/* Analog comparator 3 interrupt  (CMP3_IRQn) */
#ifdef ISR_ACMP3
	DECLARE_IRQ_HANDLER(ISR_ACMP3)
#endif
/* QEI interrupt  (QEI_IRQn) */
#ifdef ISR_QEI
	DECLARE_IRQ_HANDLER(ISR_QEI)
#endif
/* RTC alarm interrupt  (RTC_ALARM_IRQn) */
#ifdef ISR_RTC_ALARM
	DECLARE_IRQ_HANDLER(ISR_RTC_ALARM)
#endif
/* RTC wake-up interrupt  (RTC_WAKE_IRQn) */
#ifdef ISR_RTC_WAKEUP
	DECLARE_IRQ_HANDLER(ISR_RTC_WAKEUP)
#endif

/******************************************************************************/
/*                         PUBLIC FUNCTION IMPLEMENTATION                     */
/******************************************************************************/
/* Init interrupt resources */
void INTERRUPT_vInit(void)
{
	/**
	 * Config interrupt priority grouping 
	 **/
    NVIC_SetPriorityGrouping(NVIC_PRIOGRP_SETTING);

    /**
	 * Set System Handler Enabling & Priority 
	**/	
	/* Cortex-M3 Non maskable Interrupt */
	#if (IRQ_NMI == IRQ_ENABLE)
	/* Set NMI source */
	SYSCTL_SetNMISource(SRC_NMI);
	/* Disable nromal IRQn, which is seleted as NIM aource */
	NVIC_DisableIRQ(SRC_NMI);
	/* Enable NMI */
	SYSCTL_EnableNMISource();
    #endif
	
	/* Cortex-M3 Hard Fault Interrupt (HardFault_IRQn)  */
	#if (IRQ_HARDFT == IRQ_ENABLE)

	/* Hard fault has fixed priority, but can be configured to enable/disable */
    #endif
	
	/* Cortex-M3 Memory Management Interrupt (MemoryManagement_IRQn) */
	#if (IRQ_MEM == IRQ_ENABLE)
	NVIC_SetPriority(MemoryManagement_IRQn, IRQ_MEM_PRIO);
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    #endif
	
    /* Cortex-M3 Bus Fault Interrupt (BusFault_IRQn)  */	
    #if (IRQ_BUSFT == IRQ_ENABLE)
    NVIC_SetPriority(BusFault_IRQn, IRQ_BUSFT_PRIO);
	SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    #endif
	
    /* Cortex-M3 Usage Fault Interrupt (UsageFault_IRQn) */	
    #if (IRQ_USAGE == IRQ_ENABLE)
    NVIC_SetPriority(UsageFault_IRQn, IRQ_USAGE_PRIO);
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
    #endif
	
    /* Cortex-M3 SV Call Interrupt (SVCall_IRQn)  */	
    #if (IRQ_SVC == IRQ_ENABLE)
    NVIC_SetPriority(SVCall_IRQn, IRQ_SVC_PRIO);
    #endif
	
    /* Cortex-M3 Pend SV Interrupt (PendSV_IRQn)  */	
    #if (IRQ_PSV == IRQ_ENABLE)
    NVIC_SetPriority(PendSV_IRQn, IRQ_PSV_PRIO);
    #endif
	
    /* Cortex-M3 System Tick Interrupt (SysTick_IRQn) */
	#if (IRQ_SYSTICK == IRQ_ENABLE)
    NVIC_SetPriority(SysTick_IRQn, IRQ_SYSTICK_PRIO);
    #endif

    /**
	 * Set NVIC priority (NVIC_IPR)
	**/
	#if (IRQ_WDT == IRQ_ENABLE)
	NVIC_SetPriority(WDT_IRQn, IRQ_WDT_PRIO);
	#endif
	
	/* Brown-Out detection Interrupt (BOD) */
    #if (IRQ_BOD == IRQ_ENABLE)
	NVIC_SetPriority(BOD_IRQn, IRQ_BOD_PRIO);
	#endif	

	/* FLASH Interrupt (FMC) */
    #if (IRQ_FLASH == IRQ_ENABLE)
	NVIC_SetPriority(FMC_IRQn, IRQ_FLASH_PRIO);
	#endif

	/* EEPROM controller interrupt (FLASHEEPROM_IRQn) */
    #if (IRQ_EEPROM == IRQ_ENABLE)
	NVIC_SetPriority(FLASHEEPROM_IRQn, IRQ_EEPROM_PRIO);
	#endif

	/* DMA Interrupt (DMA_IRQn) */
    #if (IRQ_DMA == IRQ_ENABLE)
	NVIC_SetPriority(DMA_IRQn, IRQ_DMA_PRIO);
	#endif

	/* GPIO group 0 Interrupt (GINT0_IRQn) */
    #if (IRQ_GINT0 == IRQ_ENABLE)
	NVIC_SetPriority(GINT0_IRQn, IRQ_GINT0_PRIO);
	#endif

	/* GPIO group 1 Interrupt (GINT0_IRQn) */
    #if (IRQ_GINT1 == IRQ_ENABLE)
	NVIC_SetPriority(GINT0_IRQn, IRQ_GINT1_PRIO);
	#endif

	/* Pin Interrupt 0 (PIN_INT0_IRQn) */
    #if (IRQ_PININT0 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT0_IRQn, IRQ_PININT0_PRIO);
	#endif

	/* Pin Interrupt 1 (PIN_INT1_IRQn) */
    #if (IRQ_PININT1 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT1_IRQn, IRQ_PININT1_PRIO);
	#endif

	/* Pin Interrupt 2 (PIN_INT2_IRQn) */
    #if (IRQ_PININT2 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT2_IRQn, IRQ_PININT2_PRIO);
	#endif

	/* Pin Interrupt 3 (PIN_INT3_IRQn) */
    #if (IRQ_PININT3 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT2_IRQn, IRQ_PININT3_PRIO);
	#endif

	/* Pin Interrupt 4 (PIN_INT4_IRQn) */
    #if (IRQ_PININT4 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT4_IRQn, IRQ_PININT4_PRIO);
	#endif

	/* Pin Interrupt 5 (PIN_INT5_IRQn) */
    #if (IRQ_PININT5 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT5_IRQn, IRQ_PININT5_PRIO);
	#endif

	/* Pin Interrupt 6 (PIN_INT6_IRQn) */
    #if (IRQ_PININT6 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT6_IRQn, IRQ_PININT6_PRIO);
	#endif

	/* Pin Interrupt 7 (PIN_INT7_IRQn) */
	#if (IRQ_PININT7 == IRQ_ENABLE)
	NVIC_SetPriority(PIN_INT7_IRQn, IRQ_PININT7_PRIO);
	#endif

	/* RIT Timer Interrupt (RITIMER_IRQn) */
	#if (IRQ_RITIMER == IRQ_ENABLE)
	NVIC_SetPriority(RITIMER_IRQn, IRQ_RITIMER_PRIO);
	#endif

	/* SCT0 interrupt (SCT0_IRQn) */	
	#if (IRQ_SCT0 == IRQ_ENABLE)
	NVIC_SetPriority(SCT0_IRQn, IRQ_SCT0_PRIO);
	#endif

	/* SCT1 interrupt (SCT1_IRQn) */	
	#if (IRQ_SCT1 == IRQ_ENABLE)
	NVIC_SetPriority(SCT1_IRQn, IRQ_SCT1_PRIO);
	#endif	

	/* SCT2 interrupt (SCT2_IRQn) */	
	#if (IRQ_SCT2 == IRQ_ENABLE)
	NVIC_SetPriority(SCT2_IRQn, IRQ_SCT2_PRIO);
	#endif	

	/* SCT3 interrupt (SCT3_IRQn) */	
	#if (IRQ_SCT3 == IRQ_ENABLE)
	NVIC_SetPriority(SCT3_IRQn, IRQ_SCT3_PRIO);
	#endif	

	/* MRT interrupt (MRT_IRQn) */	
	#if (IRQ_MRT == IRQ_ENABLE)
	NVIC_SetPriority(MRT_IRQn, IRQ_MRT_PRIO);
	#endif	

	/* UART0 interrupt (UART0_IRQn) */	
	#if (IRQ_UART0 == IRQ_ENABLE)
	NVIC_SetPriority(UART0_IRQn, IRQ_UART0_PRIO);
	#endif	

	/* UART1 interrupt (UART1_IRQn) */	
	#if (IRQ_UART1 == IRQ_ENABLE)
	NVIC_SetPriority(UART1_IRQn, IRQ_UART1_PRIO);
	#endif	

	/* UART2 interrupt (UART2_IRQn) */	
	#if (IRQ_UART2 == IRQ_ENABLE)
	NVIC_SetPriority(UART2_IRQn, IRQ_UART2_PRIO);
	#endif	

	/* I2C interrupt (I2C_IRQn) */
	#if (IRQ_I2C == IRQ_ENABLE)
	NVIC_SetPriority(I2C_IRQn, IRQ_I2C_PRIO);
	#endif	

	/* SPI0 interrupt (SPI0_IRQn) */
	#if (IRQ_SPI0 == IRQ_ENABLE)
	NVIC_SetPriority(SPI0_IRQn, IRQ_SPI0_PRIO);
	#endif	

	/* SPI1 interrupt (SPI1_IRQn) */
	#if (IRQ_SPI1 == IRQ_ENABLE)
	NVIC_SetPriority(SPI1_IRQn, IRQ_SPI1_PRIO);
	#endif		

	/* CAN interrupt (CAN_IRQn) */
	#if (IRQ_CAN == IRQ_ENABLE)
	NVIC_SetPriority(CAN_IRQn, IRQ_CAN_PRIO);
	#endif	

	/* USB interrupt (USB_IRQn) */
	#if (IRQ_USB == IRQ_ENABLE)
	NVIC_SetPriority(USB_IRQn, IRQ_USB_PRIO);
	#endif	

	/* USB FIQ interrupt (USB_FIQ_IRQn) */
	#if (IRQ_USB_FIQ == IRQ_ENABLE)
	NVIC_SetPriority(USB_FIQ_IRQn, IRQ_USB_FIQ_PRIO);
	#endif

	/* USB wake-up interrupt Interrupt (USB_WAKEUP_IRQn) */
	#if (IRQ_USB_WAKEUP == IRQ_ENABLE)
	NVIC_SetPriority(USB_WAKEUP_IRQn, IRQ_USB_WAKEUP_PRIO);
	#endif	

	/* ADC0_A sequencer Interrupt (ADC0_SEQA_IRQn) */
	#if (IRQ_ADC0_SEQA == IRQ_ENABLE)
	NVIC_SetPriority(ADC0_SEQA_IRQn, IRQ_ADC0_SEQA_PRIO);
	#endif	

	/* ADC0_B sequencer Interrupt (ADC0_SEQB_IRQn) */
	#if (IRQ_ADC0_SEQB == IRQ_ENABLE)
	NVIC_SetPriority(ADC0_SEQB_IRQn, IRQ_ADC0_SEQB_PRIO);
	#endif

	/* ADC0 threshold compare interrupt (ADC0_THCMP) */
	#if (IRQ_ADC0_THCMP == IRQ_ENABLE)
	NVIC_SetPriority(ADC0_THCMP, IRQ_ADC0_THCMP_PRIO);
	#endif	

	/* ADC0 overrun interrupt  (ADC0_OVR) */
	#if (IRQ_ADC0_OVR == IRQ_ENABLE)
	NVIC_SetPriority(ADC0_OVR, IRQ_ADC0_OVR_PRIO);
	#endif	

	/* ADC1_A sequencer Interrupt (ADC1_SEQA_IRQn) */
	#if (IRQ_ADC1_SEQA == IRQ_ENABLE)
	NVIC_SetPriority(ADC1_SEQA_IRQn, IRQ_ADC1_SEQA_PRIO);
	#endif	

	/* ADC1_B sequencer Interrupt (ADC1_SEQB_IRQn) */
	#if (IRQ_ADC1_SEQB == IRQ_ENABLE)
	NVIC_SetPriority(ADC1_SEQB_IRQn, IRQ_ADC1_SEQB_PRIO);
	#endif	

	/* ADC1 threshold compare interrupt (ADC1_THCMP) */
	#if (IRQ_ADC1_THCMP == IRQ_ENABLE)
	NVIC_SetPriority(ADC1_THCMP, IRQ_ADC1_THCMP_PRIO);
	#endif		

	/* ADC1 overrun interrupt  (ADC1_OVR) */
	#if (IRQ_ADC1_OVR == IRQ_ENABLE)
	NVIC_SetPriority(ADC1_OVR, IRQ_ADC1_OVR_PRIO);
	#endif	

	/* DAC interrupt  (DAC_IRQ) */
	#if (IRQ_DAC == IRQ_ENABLE)
	NVIC_SetPriority(DAC_IRQ, IRQ_DAC_PRIO);
	#endif	

	/* Analog comparator 0 interrupt  (CMP0_IRQn) */
	#if (IRQ_ACMP0 == IRQ_ENABLE)
	NVIC_SetPriority(CMP0_IRQ, IRQ_ACMP0_PRIO);
	#endif		

	/* Analog comparator 1 interrupt  (CMP1_IRQn) */
	#if (IRQ_ACMP1 == IRQ_ENABLE)
	NVIC_SetPriority(CMP1_IRQ, IRQ_ACMP1_PRIO);
	#endif	

	/* Analog comparator 2 interrupt  (CMP2_IRQn) */
	#if (IRQ_ACMP2 == IRQ_ENABLE)
	NVIC_SetPriority(CMP2_IRQ, IRQ_ACMP2_PRIO);
	#endif		

	/* Analog comparator 3 interrupt  (CMP3_IRQn) */
	#if (IRQ_ACMP3 == IRQ_ENABLE)
	NVIC_SetPriority(CMP3_IRQ, IRQ_ACMP3_PRIO);
	#endif	

	/* QEI interrupt  (QEI_IRQn) */
	#if (IRQ_QEI == IRQ_ENABLE)
	NVIC_SetPriority(QEI_IRQn, IRQ_QEI_PRIO);
	#endif	

	/* RTC alarm interrupt  (RTC_ALARM_IRQn) */
	#if (IRQ_RTC_ALARM == IRQ_ENABLE)
	NVIC_SetPriority(RTC_ALARM_IRQn, IRQ_RTC_ALARM_PRIO);
	#endif

	/* RTC wake-up interrupt  (RTC_WAKE_IRQn) */
	#if (IRQ_RTC_WAKEUP == IRQ_ENABLE)
	NVIC_SetPriority(RTC_WAKE_IRQn, IRQ_RTC_WAKEUP_PRIO);
	#endif	

	/**
	 * Enable NVIC (NVIC_ISER)
	**/
	INTERRUPT_vEnableAll();
}

/******************************************************************************/
/*                         PRIVATE FUNCTION IMPLEMENTATION                    */
/******************************************************************************/

/******************************************************************************/
/*                         NVIC ISR Handler IMPLEMENTATION                    */
/******************************************************************************/
/**
 * Cortex reserved exception global handlers:
 * NMI: linked to a speific IRQn;
**/
void NMI_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_NMI)
    #ifdef ISR_NMI
    ISR_NMI();
    #endif
	#endif
}

/* Hard fault handler*/
void HardFault_Handler(void)
{
	while(1);
	 #if (IRQ_ENABLE == IRQ_HARDFT)
     #ifdef ISR_HARDFT
     ISR_HARDFT();
     #endif
	 #endif


}

/* Memory Management Interrupt handler*/
void MemManage_Handler(void)
{
	 #if (IRQ_ENABLE == IRQ_MEM)
     #ifdef ISR_MEM
     #error "MPU IRQ unsupported!"
     #endif
	 #endif
}

/* Bus Fault handler */
void BusFault_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_BUSFT)
    #ifdef ISR_BUSFT
    ISR_BUSFT();
    #endif
	#endif
}

/* Usage Fault handler */
void UsageFault_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_USAGE)	
    #ifdef ISR_USAGE
    ISR_USAGE();
    #endif
	#endif
}

/* SV Call Interrupt handler */
void SVC_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_SVC)
    #ifdef ISR_SVC
    #error "ServiceCall IRQ unsupported!"
    #endif
	#endif
}

/* Pending SV Interrupt handler */
void PendSV_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_PSV)	
    #ifdef ISR_PSV
    #error "Pending Service IRQ unsupported!"
    #endif
	#endif
}

/* Sysyem Tick Timer handler */
void SysTick_Handler(void)
{
	#if (IRQ_ENABLE == IRQ_SYSTICK)		
    #ifdef ISR_SYSTICK	
	ISR_SYSTICK();
    #endif
	#endif
}

/**
 * LPC15XX NVIC ISR handlers:
 **/
/* Window Watchdog handler */
void WDT_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_WDT)	
    #ifdef ISR_WDT
    ISR_WDT();
    #endif
	#endif
}

/* Brown-out detection handler */
void BOD_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_BOD)
    #ifdef ISR_BOD
    ISR_BOD();
    #endif
	#endif
}

/* FMC Interrupt handler */
void FMC_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_FLASH)	
    #ifdef ISR_FLASH
    ISR_FLASH();
    #endif
	#endif
}

/* EEPROM controller interrupt */
void EEPROM_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_EEPROM)
    #ifdef ISR_EEPROM
    ISR_EEPROM();
    #endif
	#endif
}

/* DMA Interrupt handler */
void DMA_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_DMA)
    #ifdef ISR_DMA
    ISR_DMA();
    #endif
    #endif
}

/* GPIO group 0 Interrupt handler */
void GINT0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_GINT0)	
    #ifdef ISR_GINT0
    ISR_GINT0();
    #endif
	#endif
}

/* GPIO group 1 Interrupt handler */
void GINT1_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_GINT1)
    #ifdef ISR_GINT1
    ISR_GINT1();
    #endif
	#endif
}

/* Pin Interrupt 0 handler */
void PIN_INT0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT0)	
    #ifdef ISR_PININT0
    ISR_PININT0();
    #endif
	#endif
}

/* Pin Interrupt 1 handler */
void PIN_INT1_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT1)	
    #ifdef ISR_PININT1
    ISR_PININT1();
    #endif
	#endif
}

/* Pin Interrupt 2 handler */
void PIN_INT2_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT2)		
    #ifdef ISR_PININT2
    ISR_PININT2();
    #endif
	#endif
}

/* Pin Interrupt 3 handler */
void PIN_INT3_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT3)
    #ifdef ISR_PININT3
    ISR_PININT3();
    #endif
	#endif
}

/* Pin Interrupt 4 handler */
void PIN_INT4_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT4)
    #ifdef ISR_PININT4
    ISR_PININT4();
    #endif
	#endif
}

/* Pin Interrupt 5 handler */
void PIN_INT5_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT5)	
    #ifdef ISR_PININT5
    ISR_PININT5();
    #endif
	#endif
}

/* Pin Interrupt 6 handler */
void PIN_INT6_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT6)	
    #ifdef ISR_PININT6
    ISR_PININT6();
    #endif
	#endif
}

/* Pin Interrupt 7 handler */
void PIN_INT7_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_PININT7)	
    #ifdef ISR_PININT7
    ISR_PININT7();
    #endif
	#endif
}

/* RITIMER handler */
void RIT_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_RITIMER)	
    #ifdef ISR_RITIMER	
    ISR_RITIMER();
    #endif
	#endif
}

/* SCT0 interrupt handler */
void SCT0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_SCT0)	
    #ifdef ISR_SCT0	
    ISR_SCT0();
    #endif
	#endif
}

/* SCT1 interrupt handler */
void SCT1_IRQHandler(void)
{/*
	#if (IRQ_ENABLE == IRQ_SCT1)
    #ifdef ISR_SCT1	
    ISR_SCT1();
    #endif
	#endif*/
	SCT1_IRQHandlerzz();
}

/* SCT2 interrupt handler */
void SCT2_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_SCT2)
    #ifdef ISR_SCT2	
    ISR_SCT2();
    #endif
	#endif
}

/* SCT3 interrupt handler */
void SCT3_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_SCT3)
    #ifdef ISR_SCT3	
    ISR_SCT3();
    #endif
	#endif
}

/* RITIMER handler */
void MRT_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_MRT)	
	#ifdef ISR_MRT
	ISR_MRT();
    #endif
	#endif
}

/* UART0 interrupt handler */
void UART0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_UART0)	
    #ifdef ISR_UART0	
    ISR_UART0();
    #endif
	#endif
}

/* UART1 interrupt handler */
void UART1_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_UART1)	
    #ifdef ISR_UART1	
    ISR_UART1();
    #endif
	#endif
}

/* UART2 interrupt handler */
void UART2_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_UART2)
    #ifdef ISR_UART2	
    ISR_UART2();
    #endif
	#endif
}

/* I2C interrupt handler */
void I2C0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_I2C)	
    #ifdef ISR_I2C	
    ISR_I2C();
    #endif
	#endif
}

/* SPI0 interrupt handler */
void SPI0_IRQHandler(void)
{
	
	#if (IRQ_ENABLE == IRQ_SPI0)
	#ifdef ISR_SPI0	
    ISR_SPI0();
    #endif
	#endif
}

/* SPI1 interrupt handler */
void SPI1_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_SPI1)	
	#ifdef ISR_SPI1	
    ISR_SPI1();
    #endif
	#endif
}

/* CAN interrupt handler */
void CAN_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_CAN)	
    #ifdef ISR_CAN	
    ISR_CAN();
    #endif
	#endif
}

/* USB interrupt handler */
void USB_IRQHandler(void)
{/*
	#if (IRQ_ENABLE == IRQ_USB)	
    #ifdef ISR_USB	
    ISR_USB();
    #endif
	#endif*/
	USB_IRQHandlerzz();
}

/* USB FIQ interrupt handler */
void USB_FIQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_USB_FIQ)	
    #ifdef ISR_USB_FIQ	
    ISR_USB_FIQ();
    #endif
	#endif
}

/* USB Wake-up interrupt handler */
void USBWakeup_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_USB_WAKEUP)	
    #ifdef ISR_USB_WAKEUP	
    ISR_USB_WAKEUP();
    #endif
	#endif
}

/* ADC0_A sequencer Interrupt */
void ADC0A_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC0_SEQA)
    #ifdef ISR_ADC0_SEQA	
    ISR_ADC0_SEQA();
    #endif
	#endif
}

/* ADC0_B sequencer Interrupt */
void ADC0B_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC0_SEQB)	
    #ifdef ISR_ADC0_SEQB	
    ISR_ADC0_SEQB();
    #endif
	#endif
}

/* ADC0 threshold compare interrupt */
void ADC0_THCMP_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC0_THCMP)	
    #ifdef ISR_ADC0_THCMP	
    ISR_ADC0_THCMP();
    #endif
	#endif
}

/* ADC0 overrun interrupt */
void  ADC0_OVR_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC0_OVR)	
    #ifdef ISR_ADC0_OVR	
    ISR_ADC0_OVR();
    #endif
	#endif
}

/* ADC1_A sequencer Interrupt */
void ADC1A_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC1_SEQA)
    #ifdef ISR_ADC1_SEQA	
    ISR_ADC1_SEQA();
    #endif
	#endif
}

/* ADC1_B sequencer Interrupt */
void ADC1B_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC1_SEQB)	
    #ifdef ISR_ADC1_SEQB	
    ISR_ADC1_SEQB();
    #endif
	#endif
}

/* ADC1 threshold compare interrupt */
void ADC1_THCMP_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC1_THCMP)	
    #ifdef ISR_ADC1_THCMP	
    ISR_ADC1_THCMP();
    #endif
	#endif
}

/* ADC1 overrun interrupt */
void  ADC1_OVR_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ADC1_OVR)	
    #ifdef ISR_ADC1_OVR	
    ISR_ADC1_OVR();
    #endif
	#endif
}

/* DAC interrupt */
void  DAC_OVR_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_DAC)
    #ifdef ISR_DAC	
    ISR_DAC();
    #endif
	#endif
}

/* Analog comparator 0 interrupt */
void  ACMP0_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ACMP0)
    #ifdef ISR_ACMP0	
    ISR_ACMP0();
    #endif
	#endif
}

/* Analog comparator 1 interrupt */
void  ACMP1_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ACMP1)
    #ifdef ISR_ACMP1	
    ISR_ACMP1();
    #endif
	#endif
}

/* Analog comparator 2 interrupt */
void  ACMP2_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ACMP2)	
    #ifdef ISR_ACMP2	
    ISR_ACMP2();
    #endif
	#endif
}

/* Analog comparator 3 interrupt */
void  ACMP3_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_ACMP3)	
    #ifdef ISR_ACMP3	
    ISR_ACMP3();
    #endif
	#endif
}

/* QEI interrupt */
void  QEI_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_QEI)
    #ifdef ISR_QEI	
    ISR_QEI();
    #endif
	#endif
}

/* RTC alarm interrupt */
void  RTC_ALARM_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_RTC_ALARM)	
    #ifdef ISR_RTC_ALARM	
    ISR_RTC_ALARM();
    #endif
	#endif
}

/* RTC alarm interrupt */
void  RTC_WAKE_IRQHandler(void)
{
	#if (IRQ_ENABLE == IRQ_RTC_WAKEUP)
    #ifdef ISR_RTC_WAKEUP	
    ISR_RTC_WAKEUP();
    #endif
	#endif
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

