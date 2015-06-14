/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ADC.h                                                 $ */
/*                                                                            */
/*    $Revision:: 256                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-28 17:01:23 +0800 (周二, 28 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of ADC controlls                    */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef ADC_CFG_H
#define ADC_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Enable definitions for ADC0/1 */
#define   CFG_ADC0_ENABLE                DISABLE
#define   CFG_ADC1_ENABLE                DISABLE

/**
  * Descrptions for per ADC channel configurations
  * ClockRate: it should be divided by system clock for ADC syn mode
  * If 12-bit data, 5MHz-50MHz is limited
  * ADC CTRL: An Or'ed value of the following:
                   ADC_CR_MODE10BIT 
                   ADC_CR_LPWRMODEBIT 
                   ADC_CR_CALMODEBIT
              Note: Zero means that all above ctrl bits are disabled !!
    ADC SYNMODE is default always enabled, so ADC_CR_SYNMODE is exculded for CFG CTRL             
  */
#if (ENABLE == CFG_ADC0_ENABLE)
/* Init configuration constants of ADC0  */
#define ALIAS_ADC0             ADC0                       /* Alias name for ADC0 */
#define ADC0_INIT_RATE         (36000000UL)               
#define ADC0_INIT_CTRL         (0UL)                      
#define ADC0_INIT_TRIM         ADC_TRIM_VRANGE_HIGHV      
#define ADC0_INIT_CH0          ADC_INSEL_TS               

/* Configurations for ADC0 Seq  */

/* Init channel selection and control of sequencer A of ADC0*/
#define ADC0_SEQA_CHSEL        (ADC_SEQ_CTRL_CHANSEL(0)|\
	                            ADC_SEQ_CTRL_CHANSEL(1))
/* Trigger source and settings of ADC0 */
#define ADC0_SEQA_TRIGGER	   (ADC0_SEQ_CTRL_HWTRIG_SCT0_OUT9)
/**  ADC sequence ctrl configs excluding START, BURST and SEQ ENABLE bits */
#define ADC0_SEQA_CTRL         (ADC_SEQ_CTRL_MODE_EOS|\
	                            ADC_SEQ_CTRL_HWTRIG_POLPOS)

#undef ADC0_SEQB_CHSEL
#undef ADC0_SEQB_TRIGGER
#undef ADC0_SEQB_CTRL

/* Interrupt enable settings of ADC0 */
/* Set 0 to disable all interrupts */
#define ADC0_INTEN_CFG        (ADC_INTEN_SEQA_ENABLE)

/* Callbacks for ADC0 ISR */
#define ADC0_SEQA_CB           Measure_ADC0_SeqAHandler
#undef  ADC0_SEQB_CB
#define ADC0_THCMP_CB          Measure_ADC0_ThcmpHandler
#undef  ADC0_OVR_CB
#endif  

/**********************************************************************
  * Configurations for ADC1
  */
#if (ENABLE == CFG_ADC1_ENABLE)
/* Init configuration constants of ADC1  */
#define BOARD_ADC_CH           ADC1                    /* Alias name for ADC1 */
#define ADC1_INIT_RATE         (36000000UL)            
#define ADC1_INIT_CTRL         (0UL)                   /* Control settings for ADC1 */
#define ADC1_INIT_TRIM         (ADC_TRIM_VRANGE_HIGHV) /* Trim voltage range for ADC1 */
#define ADC1_INIT_CH0          (ADC_INSEL_VDDA_DIV)    /* Channel 0 selection of ADC1 */

/**
  * Configurations for ADC1 Seq
  */
/* Init channel selection and control of sequencer A of ADC1 */
#define ADC1_SEQA_CHSEL        (ADC_SEQ_CTRL_CHANSEL(0)|\
	                            ADC_SEQ_CTRL_CHANSEL(1)|\
	                            ADC_SEQ_CTRL_CHANSEL(2)|\
	                            ADC_SEQ_CTRL_CHANSEL(3)|\
	                            ADC_SEQ_CTRL_CHANSEL(4)|\
	                            ADC_SEQ_CTRL_CHANSEL(5)|\
	                            ADC_SEQ_CTRL_CHANSEL(6)|\
	                            ADC_SEQ_CTRL_CHANSEL(7)|\
	                            ADC_SEQ_CTRL_CHANSEL(8))						
/* Trigger source and settings of ADC1 using seq ctrl field mask */	                            
#define ADC1_SEQA_TRIGGER	   (ADC1_SEQ_CTRL_HWTRIG_ADC1_PIN_TRIG0)
/**  ADC sequence ctrl configs excluding START, BURST and SEQ ENABLE bits */
#define ADC1_SEQA_CTRL         (ADC_SEQ_CTRL_MODE_EOS|\
	                            ADC_SEQ_CTRL_HWTRIG_POLPOS)

#undef ADC1_SEQB_CHSEL
#undef ADC1_SEQB_TRIGGER
#undef ADC1_SEQB_CTRL

/* Interrupt enable config of ADC1 */
/* Undef it to disable all seq interrupts */
#define ADC1_INTEN_CFG        (ADC_INTEN_SEQA_ENABLE)

/*  Callbacks for ADC1 ISR */
#define ADC1_SEQA_CB           Measure_ADC1_SeqAHandler
#undef  ADC1_SEQB_CB
#undef  ADC1_THCMP_CB          
#define ADC1_OVR_CB            Measure_ADC1_OverrunHandler  
#endif


/* Macro def - ADC used */
#if (ENABLE == CFG_ADC0_ENABLE)||\
	(ENABLE == CFG_ADC1_ENABLE)
#define CFG_ADC_USED
#endif

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
#endif

