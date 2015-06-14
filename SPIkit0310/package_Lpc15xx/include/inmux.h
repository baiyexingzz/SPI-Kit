/******************************************************************************/
/*                                                                            */
/*    $Workfile::   InMux.h                                                  $ */
/*                                                                            */
/*    $Revision:: 320                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-14 15:30:51 +0800 (周五, 14 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - Input Mux       */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef INMUX_H
#define INMUX_H

#include "platform.h"
#include "io.h"
#include "system.h"

/** @defgroup INMUX_15XX CHIP: LPC15xx Input Mux Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief LPC15xx Input Mux Register Block Structure
 */
typedef struct {					/*!< INMUX Structure */
	__IO U32  SCT0_INMUX[7];		/*!< Input mux registers for SCT0 inputs */
	__I  U32  RESERVED1[1];
	__IO U32  SCT1_INMUX[7];		/*!< Input mux registers for SCT1 inputs */
	__I  U32  RESERVED2[1];
	__IO U32  SCT2_INMUX[3];		/*!< Input mux registers for SCT2 inputs */
	__I  U32  RESERVED3[5];
	__IO U32  SCT3_INMUX[3];		/*!< Input mux registers for SCT3 inputs */
	__I  U32  RESERVED4[5];
	__I  U32  RESERVED4A[16];
	__IO U32  PINTSEL[8];			/*!< Pin interrupt select registers */
	__IO U32  DMA_ITRIG_INMUX[18];	/*!< Input mux register for DMA trigger inputs */
	__I  U32  RESERVED5[6];
	__IO U32  DMA_INMUX[4];			/*!< Input mux register for DMA trigger inputs */
	__I  U32  RESERVED6[4];
	__IO U32  FREQMEAS_REF;			/*!< Clock selection for frequency measurement ref clock */
	__IO U32  FREQMEAS_TARGET;		/*!< Clock selection for frequency measurement target clock */
} LPC_INMUX_T;

/* SCT input mux mapping selections for SCT0 inputs 0-6 */
typedef enum {
	SCT0_INMUX_PIO0_2 = 0,
	SCT0_INMUX_PIO0_3,
	SCT0_INMUX_PIO0_17,
	SCT0_INMUX_PIO0_30,
	SCT0_INMUX_PIO1_6,
	SCT0_INMUX_PIO1_7,
	SCT0_INMUX_PIO1_12,
	SCT0_INMUX_PIO1_13,
	SCT0_INMUX_SCT1_OUT4,
	SCT0_INMUX_SCT2_OUT4,
	SCT0_INMUX_SCT2_OUT5,
	SCT0_INMUX_ADC0_THCMP_IRQ,
	SCT0_INMUX_ADC1_THCMP_IRQ,
	SCT0_INMUX_ACMP0_OUT,
	SCT0_INMUX_ACMP1_OUT,
	SCT0_INMUX_ACMP2_OUT,
	SCT0_INMUX_ACMP3_OUT,
	SCT0_INMUX_SCTIPU_ABORT,
	SCT0_INMUX_SCTIPU_SAMPLE0,
	SCT0_INMUX_SCTIPU_SAMPLE1,
	SCT0_INMUX_SCTIPU_SAMPLE2,
	SCT0_INMUX_SCTIPU_SAMPLE3,
	SCT0_INMUX_DEBUG_HALTED
} SCT0_INMUX_T;

/* SCT input mux mapping selections for SCT1 inputs 0-6 */
typedef enum {
	SCT1_INMUX_PIO0_15 = 0,
	SCT1_INMUX_PIO0_16,
	SCT1_INMUX_PIO0_21,
	SCT1_INMUX_PIO0_31,
	SCT1_INMUX_PIO1_4,
	SCT1_INMUX_PIO1_5,
	SCT1_INMUX_PIO1_15,
	SCT1_INMUX_PIO1_16,
	SCT1_INMUX_SCT0_OUT4,
	SCT1_INMUX_SCT3_OUT4,
	SCT1_INMUX_SCT3_OUT5,
	SCT1_INMUX_ADC0_THCMP_IRQ,
	SCT1_INMUX_ADC1_THCMP_IRQ,
	SCT1_INMUX_ACMP0_OUT,
	SCT1_INMUX_ACMP1_OUT,
	SCT1_INMUX_ACMP2_OUT,
	SCT1_INMUX_ACMP3_OUT,
	SCT1_INMUX_SCTIPU_ABORT,
	SCT1_INMUX_SCTIPU_SAMPLE0,
	SCT1_INMUX_SCTIPU_SAMPLE1,
	SCT1_INMUX_SCTIPU_SAMPLE2,
	SCT1_INMUX_SCTIPU_SAMPLE3,
	SCT1_INMUX_DEBUG_HALTED
} SCT1_INMUX_T;

/* SCT input mux mapping selections for SCT2 inputs 0-2 */
typedef enum {
	SCT2_INMUX_PIO0_4 = 0,
	SCT2_INMUX_PIO0_27,
	SCT2_INMUX_PIO1_18,
	SCT2_INMUX_PIO1_19,
	SCT2_INMUX_SCT0_OUT4,
	SCT2_INMUX_SCT0_OUT5,
	SCT2_INMUX_SCT0_OUT7,
	SCT2_INMUX_SCT0_OUT8,
	SCT2_INMUX_ADC0_THCMP_IRQ,
	SCT2_INMUX_ADC1_THCMP_IRQ,
	SCT2_INMUX_ACMP0_OUT,
	SCT2_INMUX_ACMP1_OUT,
	SCT2_INMUX_ACMP2_OUT,
	SCT2_INMUX_ACMP3_OUT,
	SCT2_INMUX_SCTIPU_ABORT,
	SCT2_INMUX_SCTIPU_SAMPLE0,
	SCT2_INMUX_SCTIPU_SAMPLE1,
	SCT2_INMUX_SCTIPU_SAMPLE2,
	SCT2_INMUX_SCTIPU_SAMPLE3,
	SCT2_INMUX_USB_FRAME_TOGGLE,
	SCT2_INMUX_DEBUG_HALTED
} SCT2_INMUX_T;

/* SCT input mux mapping selections for SCT3 inputs 0-2 */
typedef enum {
	SCT3_INMUX_PIO0_7 = 0,
	SCT3_INMUX_PIO1_11,
	SCT3_INMUX_PIO1_21,
	SCT3_INMUX_PIO1_22,
	SCT3_INMUX_SCT1_OUT4,
	SCT3_INMUX_SCT1_OUT5,
	SCT3_INMUX_SCT1_OUT7,
	SCT3_INMUX_SCT1_OUT8,
	SCT3_INMUX_ADC0_THCMP_IRQ,
	SCT3_INMUX_ADC1_THCMP_IRQ,
	SCT3_INMUX_ACMP0_OUT,
	SCT3_INMUX_ACMP1_OUT,
	SCT3_INMUX_ACMP2_OUT,
	SCT3_INMUX_ACMP3_OUT,
	SCT3_INMUX_SCTIPU_ABORT3,
	SCT3_INMUX_SCTIPU_SAMPLE0,
	SCT3_INMUX_SCTIPU_SAMPLE1,
	SCT3_INMUX_SCTIPU_SAMPLE2,
	SCT3_INMUX_SCTIPU_SAMPLE3,
	SCT3_INMUX_USB_FRAME_TOGGLE,
	SCT3_INMUX_DEBUG_HALTED
} SCT3_INMUX_T;

/* DMA triggers that can mapped to DMA channels */
typedef enum {
	DMATRIG_ADC0_SEQA_IRQ = 0,			/*!< ADC0 sequencer A interrupt as trigger */
	DMATRIG_ADC0_SEQB_IRQ,				/*!< ADC0 sequencer B interrupt as trigger */
	DMATRIG_ADC1_SEQA_IRQ,				/*!< ADC1 sequencer A interrupt as trigger */
	DMATRIG_ADC1_SEQB_IRQ,				/*!< ADC1 sequencer B interrupt as trigger */
	DMATRIG_SCT0_DMA0,					/*!< SCT 0, DMA 0 as trigger */
	DMATRIG_SCT0_DMA1,					/*!< SCT 1, DMA 1 as trigger */
	DMATRIG_SCT1_DMA0,					/*!< SCT 0, DMA 0 as trigger */
	DMATRIG_SCT1_DMA1,					/*!< SCT 1, DMA 1 as trigger */
	DMATRIG_SCT2_DMA0,					/*!< SCT 2, DMA 0 as trigger */
	DMATRIG_SCT2_DMA1,					/*!< SCT 2, DMA 1 as trigger */
	DMATRIG_SCT3_DMA0,					/*!< SCT 3, DMA 0 as trigger */
	DMATRIG_SCT3_DMA1,					/*!< SCT 3, DMA 1 as trigger */
	DMATRIG_ACMP0_OUT,					/*!< Analog comparator 0 output as trigger */
	DMATRIG_ACMP1_OUT,					/*!< Analog comparator 1 output as trigger */
	DMATRIG_ACMP2_OUT,					/*!< Analog comparator 2 output as trigger */
	DMATRIG_ACMP3_OUT,					/*!< Analog comparator 3 output as trigger */
	DMATRIG_OUTMUX0,					/*!< DMA trigger tied to this source, Select with INMUX_SetDMAOutMux */
	DMATRIG_OUTMUX1,					/*!< DMA trigger tied to this source, Select with INMUX_SetDMAOutMux */
	DMATRIG_OUTMUX2,					/*!< DMA trigger tied to this source, Select with INMUX_SetDMAOutMux */
	DMATRIG_OUTMUX3,					/*!< DMA trigger tied to this source, Select with INMUX_SetDMAOutMux */
    DMATRIG_NOHWTRIG                    /*!< None DMA HW trigger tied to this source and Peripheral request Enable  */
} DMA_TRIGSRC_T;

/* Freqeuency mearure reference and target clock sources */
typedef enum {
	FREQMSR_MAIN_OSC = 0,			/*!< System oscillator */
	FREQMSR_IRC,					/*!< Internal RC (IRC) oscillator */
	FREQMSR_WDOSC,					/*!< Watchdog oscillator */
	FREQMSR_32KHZOSC,				/*!< 32KHz (RTC) oscillator rate */
	FREQMSR_USB_FTOGGLE,			/*!< USB FTOGGLE rate */
	FREQMSR_PIO0_5,					/*!< External pin PIO0_5 as input rate */
	FREQMSR_PIO0_19,				/*!< External pin PIO0_19 as input rate */
	FREQMSR_PIO0_30,				/*!< External pin PIO0_30 as input rate */
	FREQMSR_PIO1_27					/*!< External pin PIO1_27 as input rate */
} FREQMSR_SRC_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initial INMUX configuration
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void INMUX_vInit(void)
{
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_MUX);
	SYSCTL_vPeriphReset(RESET_MUX);
}

/**
 * @brief	Shutdown INMUX configuration
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void INMUX_vShutdown(void)
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_MUX);
}

/**
 * @brief	Selects an input source for SCT0 input 0 to 6
 * @param	ubInput	: SCT0 input to use, 0 - 6
 * @param	teSrc	: Source to map to the SCT input
 * @return	Nothing
 */
STATIC INLINE void INMUX_vSelectSCT0Src(U8 ubInput, SCT0_INMUX_T teSrc)
{
	WRITE_REG(LPC_INMUX->SCT0_INMUX[ubInput], (U32)teSrc);
}

/**
 * @brief	Selects an input source for SCT1 input 0 to 6
 * @param	ubInput	: SCT1 input to use, 0 - 6
 * @param	teSrc	: Source to map to the SCT input
 * @return	Nothing
 */
STATIC INLINE void INMUX_vSelectSCT1Src(U8 ubInput, SCT1_INMUX_T teSrc)
{
	WRITE_REG(LPC_INMUX->SCT1_INMUX[ubInput], (U32)teSrc);
}

/**
 * @brief	Selects an input source for SCT2 input 0 to 2
 * @param	ubInput	: SCT2 input to use, 0 - 2
 * @param	teSrc	: Source to map to the SCT input
 * @return	Nothing
 */
STATIC INLINE void INMUX_vSelectSCT2Src(U8 ubInput, SCT2_INMUX_T teSrc)
{
	WRITE_REG(LPC_INMUX->SCT2_INMUX[ubInput], (U32)teSrc);
}

/**
 * @brief	Selects an input source for SCT3 input 0 to 2
 * @param	ubInput	: SCT3 input to use, 0 - 2
 * @param	teSrc	: Source to map to the SCT input
 * @return	Nothing
 */
STATIC INLINE void INMUX_vSelectSCT3Src(U8 ubInput, SCT3_INMUX_T teSrc)
{
	WRITE_REG(LPC_INMUX->SCT3_INMUX[ubInput], (U32)teSrc);
}

/**
 * @brief	GPIO Pin Interrupt Pin Select (sets PINTSEL register)
 * @param	ubPintSel	: GPIO PINTSEL interrupt, should be: 0 to 7
 * @param	tePin	    : Global pin number
 * @return	Nothing
 */
STATIC INLINE void INMUX_vExtPinIntSel(U8 ubPintSel, IO_PIN_NO_T tePin)
{
	WRITE_REG(LPC_INMUX->PINTSEL[ubPintSel], tePin);
}

/**
 * @brief	Select a trigger source for a DMA channel
 * @param	ubDmaCh	: DMA channel number
 * @param	teTrig	: Trigger source for the DMA channel
 * @return	Nothing
 */
STATIC INLINE void INMUX_vSetDMATrigger(U8 ubDmaCh, DMA_TRIGSRC_T teTrig)
{
	WRITE_REG(LPC_INMUX->DMA_ITRIG_INMUX[ubDmaCh], teTrig);
}

/**
 * @brief	Selects a DMA trigger source for the DMATRIG_OUTMUXn IDs
 * @param	ubIndex	: Select 0 to 3 to sets the source for DMATRIG_OUTMUX0 to DMATRIG_OUTMUX3
 * @param	ubDmaCh	: DMA channel to select for DMATRIG_OUTMUXn source
 * @return	Nothing
 * @note	This function sets the DMA trigger (out) source used with the DMATRIG_OUTMUXn
 *			trigger source.
 */
STATIC INLINE void INMUX_vSetDMAOutMux(U8 ubIndex, U8 ubDmaCh)
{
	WRITE_REG(LPC_INMUX->DMA_INMUX[ubIndex], (U32)ubDmaCh);
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


