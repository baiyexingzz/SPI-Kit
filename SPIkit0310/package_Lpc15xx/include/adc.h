/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ADC.h                                                 $ */
/*                                                                            */
/*    $Revision:: 294                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 12:20:36 +0800 (周三, 05 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - ADC controllers */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef ADC_H
#define ADC_H

#include "platform.h"

#include "system.h"
#include "adc_cfg.h"

#ifdef CFG_ADC_USED
/** @defgroup ADC_15XX CHIP: LPC15xx ADC Controller driver
 *  @ingroup 15XX_Drivers
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/** Maximum clock rate in Hz for ADC */
#define ADC_MAX_CLOCK_RATE            (50000000UL)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/** ADC group */
#define ADC_GROUP_MAXNUM                 (2)
typedef enum {
    #if (ENABLE == CFG_ADC0_ENABLE)
	ADC0        = 0,
	#endif	
	#if (ENABLE == CFG_ADC1_ENABLE)
	ADC1        = 1,
	#endif
} ADC_GROUP_T;

/** Sequence index enumerations, used in various parts of the code for
    register indexing and sequencer selection */
typedef enum {
	ADC_SEQA = 0,
	ADC_SEQB
} ADC_SEQ_T;

/**
 * @brief ADC register support bitfields and mask
 */
/** ADC Control register bit fields */
#define ADC_CR_MASK             (0x7FFFFF00)
#define ADC_CR_CLKDIV_BITPOS    (0)						                            /*!< Bit position for Clock divider value */
#define ADC_CR_CLKDIV_MASK      (0xFF << ADC_CR_CLKDIV_BITPOS)	                    /*!< Mask for Clock divider value only for synchronous mode */
#define ADC_CR_CLKDIV_VAL(n)    (((n)<<ADC_CR_CLKDIV_BITPOS)& ADC_CR_CLKDIV_MASK)   /*!< Clock divider value */
#define ADC_CR_ASYNMODE         (1 << 8)				                            /*!< Asynchronous mode enable bit */
#define ADC_CR_MODE10BIT        (1 << 9)				                            /*!< 10-bit mode enable bit */
#define ADC_CR_LPWRMODEBIT      (1 << 10)				                            /*!< Low power mode enable bit */
#define ADC_CR_CALMODEBIT       (1 << 30)				                            /*!< Self calibration cycle enable bit */
#define ADC_CR_BITACC(n)        ((((n) & 0x1) << 9))	                            /*!< 12-bit or 10-bit ADC accuracy */
#define ADC_CR_CLKDIV(n)        ((((n) & 0xFF) << 0))	                            /*!< The APB clock (PCLK) is divided by (this value plus one) to produce the clock for the A/D */
#define ADC_SAMPLE_RATE_CONFIG_MASK (ADC_CR_CLKDIV(0xFF) | ADC_CR_BITACC(0x01))

/** ADC input select register */
#define ADC_INSEL_MASK          (0xF)				    /*!< Select ADCn_0 for channel 0 - mask */
#define ADC_INSEL_ADC0          (0x0 << 0)				/*!< Select ADCn_0 for channel 0 */
#define ADC_INSEL_CRVO          (0x1 << 0)				/*!< Selects the Core voltage regulator output for channel 0 */
#define ADC_INSEL_IVR           (0x2 << 0)				/*!< Selects the Internal voltage reference for channel 0 */
#define ADC_INSEL_TS            (0x3 << 0)				/*!< Selects the Temperature Sensor for channel 0 */
#define ADC_INSEL_VDDA_DIV      (0x4 << 0)				/*!< Selects VDDA/2 for channel 0 */
#define ADC_INSEL_NOLOAD        (0xF << 0)				/*!< No connection and load for channel 0 */

/** ADC Sequence Control register bit fields */
#define ADC_SEQ_CTRL_MASK         (0xFC0FFFFF)         /*!< Seq Ctrl register mask, mask off bit 20..25 */
#define ADC_SEQ_CTRL_CHANSEL_MASK (0xFFF)				/*!< Channel select mask */
#define ADC_SEQ_CTRL_CHANSEL(n)   (1 << (n))			/*!< Channel select macro */

/** ADC hardware trigger sources in SEQ_CTRL for ADC0 only. These sources should
 * only be used when selecting trigger sources for ADC0. */
#define ADC0_SEQ_CTRL_HWTRIG_ADC0_PIN_TRIG0 (0 << 12)	/*!< HW trigger input - ADC0_PIN_TRIG0 */
#define ADC0_SEQ_CTRL_HWTRIG_ADC0_PIN_TRIG1 (1 << 12)	/*!< HW trigger input - ADC0_PIN_TRIG1 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT0_OUT7      (2 << 12)   /*!< HW trigger input - SCT0_OUT7 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT0_OUT9      (3 << 12)	/*!< HW trigger input - SCT0_OUT9 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT1_OUT7      (4 << 12)	/*!< HW trigger input - SCT1_OUT7 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT1_OUT9      (5 << 12)	/*!< HW trigger input - SCT1_OUT9 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT2_OUT3      (6 << 12)	/*!< HW trigger input - SCT2_OUT3 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT2_OUT4      (7 << 12)	/*!< HW trigger input - SCT2_OUT4 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT3_OUT3      (8 << 12)	/*!< HW trigger input - SCT3_OUT3 */
#define ADC0_SEQ_CTRL_HWTRIG_SCT3_OUT4      (9 << 12)	/*!< HW trigger input - SCT3_OUT4 */
#define ADC0_SEQ_CTRL_HWTRIG_ACMP0_O        (10 << 12)	/*!< HW trigger input - ACMP0_O */
#define ADC0_SEQ_CTRL_HWTRIG_ACMP1_O        (11 << 12)	/*!< HW trigger input - ACMP1_O */
#define ADC0_SEQ_CTRL_HWTRIG_ACMP2_O        (12 << 12)	/*!< HW trigger input - ACMP2_O */
#define ADC0_SEQ_CTRL_HWTRIG_ACMP3_O        (13 << 12)	/*!< HW trigger input - ACMP3_O */
#define ADC0_SEQ_CTRL_HWTRIG_MASK           (0x3F << 12)/*!< HW trigger input bitfield mask */

/** ADC hardware trigger sources in SEQ_CTRL for ADC1 only. These sources should
 * only be used when selecting trigger sources for ADC1. */
#define ADC1_SEQ_CTRL_HWTRIG_ADC1_PIN_TRIG0 (0 << 12)	/*!< HW trigger input - ADC1_PIN_TRIG0 */
#define ADC1_SEQ_CTRL_HWTRIG_ADC1_PIN_TRIG1 (1 << 12)	/*!< HW trigger input - ADC1_PIN_TRIG1 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT0_OUT6      (2 << 12)	/*!< HW trigger input - SCT0_OUT6 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT0_OUT9      (3 << 12)	/*!< HW trigger input - SCT0_OUT9 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT1_OUT8      (4 << 12)	/*!< HW trigger input - SCT1_OUT8 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT1_OUT9      (5 << 12)	/*!< HW trigger input - SCT1_OUT9 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT2_OUT2      (6 << 12)	/*!< HW trigger input - SCT2_OUT2 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT2_OUT5      (7 << 12)	/*!< HW trigger input - SCT2_OUT5 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT3_OUT2      (8 << 12)	/*!< HW trigger input - SCT3_OUT2 */
#define ADC1_SEQ_CTRL_HWTRIG_SCT3_OUT5      (9 << 12)	/*!< HW trigger input - SCT3_OUT5 */
#define ADC1_SEQ_CTRL_HWTRIG_ACMP0_O        (10 << 12)	/*!< HW trigger input - ACMP0_O */
#define ADC1_SEQ_CTRL_HWTRIG_ACMP1_O        (11 << 12)	/*!< HW trigger input - ACMP1_O */
#define ADC1_SEQ_CTRL_HWTRIG_ACMP2_O        (12 << 12)	/*!< HW trigger input - ACMP2_O */
#define ADC1_SEQ_CTRL_HWTRIG_ACMP3_O        (13 << 12)	/*!< HW trigger input - ACMP3_O */
#define ADC1_SEQ_CTRL_HWTRIG_MASK           (0x3F << 12)/*!< HW trigger input bitfield mask */

/** SEQ_CTRL register bit fields */
#define ADC_SEQ_CTRL_HWTRIG_MASK         (1 << 18)		/*!< HW trigger polarity - mask */
#define ADC_SEQ_CTRL_HWTRIG_POLPOS       (1 << 18)		/*!< HW trigger polarity - positive edge */
#define ADC_SEQ_CTRL_HWTRIG_POLNEG       (0 << 18)		/*!< HW trigger polarity - negative edge */
#define ADC_SEQ_CTRL_HWTRIG_SYNCBYPASS   (1 << 19)		/*!< HW trigger bypass synchronisation */
#define ADC_SEQ_CTRL_START               (1 << 26)		/*!< Start conversion enable bit */
#define ADC_SEQ_CTRL_BURST               (1 << 27)		/*!< Repeated conversion enable bit */
#define ADC_SEQ_CTRL_SINGLESTEP          (1 << 28)		/*!< Single step enable bit */
#define ADC_SEQ_CTRL_LOWPRIO             (1 << 29)		/*!< High priority enable bit (regardless of name) */
#define ADC_SEQ_CTRL_MODE_MASK           (1 << 30)		/*!< Mode End of sequence enable bit */
#define ADC_SEQ_CTRL_MODE_EOS            (1 << 30)		/*!< Mode End of sequence enable bit */
#define ADC_SEQ_CTRL_MODE_EOC            (0 << 30)		/*!< Mode End of conversion enable bit */
#define ADC_SEQ_CTRL_SEQ_ENA             (1UL << 31)	/*!< Sequence enable bit */
#define ADC_SEQ_CTRL_CFG_MASK            (0x700C0000)

/** ADC global data register bit fields */
#define ADC_SEQ_GDAT_RESULT(n)           ((((n) >> 4) & 0xFFF))	/*!< Macro for getting the ADC data value */
#define ADC_SEQ_GDAT_RESULT_MASK         (0xFFF << 4)	/*!< Result value mask */
#define ADC_SEQ_GDAT_RESULT_BITPOS       (4)			/*!< Result start bit position */
#define ADC_SEQ_GDAT_THCMPRANGE_MASK     (0x3 << 16)	/*!< Comparion range mask */
#define ADC_SEQ_GDAT_THCMPRANGE_BITPOS   (16)			/*!< Comparison range bit position */
#define ADC_SEQ_GDAT_THCMPCROSS_MASK     (0x3 << 18)	/*!< Comparion cross mask */
#define ADC_SEQ_GDAT_THCMPCROSS_BITPOS   (18)			/*!< Comparison cross bit position */
#define ADC_SEQ_GDAT_CHAN_MASK           (0xF << 26)	/*!< Channel number mask */
#define ADC_SEQ_GDAT_CHAN_BITPOS         (26)			/*!< Channel number bit position */
#define ADC_SEQ_GDAT_OVERRUN             (1 << 30)		/*!< Overrun bit */
#define ADC_SEQ_GDAT_DATAVALID           (1UL << 31)	/*!< Data valid bit */

/* Support macro for data channel */
#define ADC_SEQ_GDAT(adcch,seq)        (LPC_ADC[adcch]->SEQ_GDAT[seq])

/** ADC Data register bit fields */
#define ADC_DR_RESULT(n)           ((((n) >> 4) & 0xFFF))	/*!< Macro for getting the ADC data value */
#define ADC_DR_THCMPRANGE_MASK     (0x3 << 16)			/*!< Comparion range mask */
#define ADC_DR_THCMPRANGE_BITPOS   (16)					/*!< Comparison range bit position */
#define ADC_DR_THCMPRANGE(n)       (((n) >> ADC_DR_THCMPRANGE_BITPOS) & 0x3)
#define ADC_DR_THCMPCROSS_MASK     (0x3 << 18)			/*!< Comparion cross mask */
#define ADC_DR_THCMPCROSS_BITPOS   (18)					/*!< Comparison cross bit position */
#define ADC_DR_THCMPCROSS(n)       (((n) >> ADC_DR_THCMPCROSS_BITPOS) & 0x3)
#define ADC_DR_CHAN_MASK           (0xF << 26)			/*!< Channel number mask */
#define ADC_DR_CHAN_BITPOS         (26)					/*!< Channel number bit position */
#define ADC_DR_CHANNEL(n)          (((n) >> ADC_DR_CHAN_BITPOS) & 0xF)	/*!< Channel number bit position */
#define ADC_DR_OVERRUN             (1 << 30)			/*!< Overrun bit */
#define ADC_DR_DATAVALID           (1UL << 31)			/*!< Data valid bit */
#define ADC_DR_DONE(n)             (((n) >> 31))

/* Support macro for data channel */
#define ADC_DR(adcch,datach)        (LPC_ADC[adcch]->DR[datach])

/** ADC low/high Threshold register bit fields */
#define ADC_THR_VAL_MASK            (0xFFF << 4)		/*!< Threshold value bit mask */
#define ADC_THR_VAL_POS             (4)					/*!< Threshold value bit position */

/** ADC Threshold select register bit fields */
#define ADC_THRSEL_MASK             (0x00000FFF)
#define ADC_THRSEL_CHAN_SEL_THR(n) (1 << (n))			/*!< Select THR1 register for channel n */

/** ADC Interrupt Enable register bit fields */
#define ADC_INTEN_MASK              (0x07FFFFFF)
#define ADC_INTEN_SEQA_ENABLE       (1 << 0)			/*!< Sequence A Interrupt enable bit */
#define ADC_INTEN_SEQB_ENABLE       (1 << 1)			/*!< Sequence B Interrupt enable bit */
#define ADC_INTEN_SEQN_ENABLE(seq)  (1 << (seq))		/*!< Sequence A/B Interrupt enable bit */
#define ADC_INTEN_OVRRUN_ENABLE     (1 << 2)			/*!< Overrun Interrupt enable bit */
#define ADC_INTEN_CMP_DISBALE       (0)					/*!< Disable comparison interrupt value */
#define ADC_INTEN_CMP_OUTSIDETH     (1)					/*!< Outside threshold interrupt value */
#define ADC_INTEN_CMP_CROSSTH       (2)					/*!< Crossing threshold interrupt value */
#define ADC_INTEN_CMP_MASK          (3)					/*!< Comparison interrupt value mask */
#define ADC_INTEN_CMP_ENABLE(isel, ch) (((isel) & ADC_INTEN_CMP_MASK) << ((2 * (ch)) + 3))	/*!< Interrupt selection for channel */

/** ADC Flags register bit fields */
#define ADC_INTFLAGS_MASK              (0xF3FFFFFF)
#define ADC_INTFLAGS_THCMP_MASK(ch)    (1 << (ch))		    /*!< Threshold comparison status for channel */
#define ADC_INTFLAGS_THCMP_ALLMASK     (0x000000FFF)        /*!< Threshold comparison status for all channels */ 
#define ADC_INTFLAGS_OVRRUN_MASK(ch)   (1 << (12 + (ch)))	/*!< Overrun status for channel */
#define ADC_INTFLAGS_OVRRUN_ALLMASK    (0x00FFF000)	        /*!< Overrun status for all channels */
#define ADC_INTFLAGS_SEQA_OVRRUN_MASK  (1 << 24)			/*!< Seq A Overrun status */
#define ADC_INTFLAGS_SEQB_OVRRUN_MASK  (1 << 25)			/*!< Seq B Overrun status */
#define ADC_INTFLAGS_SEQN_OVRRUN_MASK(seq) (1 << (24 + (seq)))	/*!< Seq A/B Overrun status */
#define ADC_INTFLAGS_SEQA_INT_MASK     (1 << 28)			/*!< Seq A Interrupt status */
#define ADC_INTFLAGS_SEQB_INT_MASK     (1 << 29)			/*!< Seq B Interrupt status */
#define ADC_INTFLAGS_SEQN_INT_MASK(seq) (1 << (28 + (seq))) /*!< Seq A/B Interrupt status */
#define ADC_INTFLAGS_THCMP_INT_MASK    (1 << 30)			/*!< Threshold comparison Interrupt status */
#define ADC_INTFLAGS_OVRRUN_INT_MASK   (1UL << 31)			/*!< Overrun Interrupt status */

/** ADC Trim register bit fields */
#define ADC_TRIM_VRANGE_MASK        (1 << 5)			/*!< Voltage range bit */
#define ADC_TRIM_VRANGE_HIGHV       (0 << 5)			/*!< Voltage range bit - High volatge (2.7V to 3.6V) */
#define ADC_TRIM_VRANGE_LOWV        (1 << 5)			/*!< Voltage range bit - Low volatge (1.8V to 2.7V) */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/** ADC sequence global data register threshold comparison range enumerations */
typedef enum {
	ADC_DR_THCMPRANGE_INRANGE = 0,
	ADC_DR_THCMPRANGE_RESERVED,
	ADC_DR_THCMPRANGE_BELOW,
	ADC_DR_THCMPRANGE_ABOVE
} ADC_DR_THCMPRANGE_T;

/** ADC sequence global data register threshold comparison cross enumerations */
typedef enum {
	ADC_DR_THCMPCROSS_NOCROSS = 0,
	ADC_DR_THCMPCROSS_RESERVED,
	ADC_DR_THCMPCROSS_DOWNWARD,
	ADC_DR_THCMPCROSS_UPWARD
} ADC_DR_THCMPCROSS_T;

/** Threshold interrupt event options */
typedef enum {
	ADC_INTEN_THCMP_DISABLE,
	ADC_INTEN_THCMP_OUTSIDE,
	ADC_INTEN_THCMP_CROSSING,
} ADC_INTEN_THCMP_T;

/** @defgroup ADC_15XX CHIP:  LPC15xx A/D conversion driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */
 
/**
 * @brief ADC register block structure
 */
typedef struct {						    /*!< ADCn Structure */
	__IO U32 CTRL;							/*!< A/D Control Register. The AD0CR register must be written to select the operating mode before A/D conversion can occur. */
	__IO U32 INSEL;						    /*!< A/D Input Select Register. This field selects the input source for channel 0. */
	__IO U32 SEQ_CTRL[ADC_SEQB + 1];	/*!< A/D Sequence A & B Control Register. Controls triggering and channel selection for sonversion sequence. */
	__IO U32 SEQ_GDAT[ADC_SEQB + 1];	/*!< A/D Sequence A & B Global Data Register. Contains the result of the most recent A/D conversion for sequence. */
	__I  U32 RESERVED1[2];
	__I  U32 DR[12];						/*!< A/D Channel Data Register. This register contains the result of the most recent conversion completed on channel n. */
	__IO U32 THR_LOW[2];					/*!< A/D Low Compare Threshold Register 0 & 1. Contains the lower threshold level for automatic threshold comparison. */
	__IO U32 THR_HIGH[2];					/*!< A/D High Compare Threshold Register 0 & 1. Contains the higher threshold level for automatic threshold comparison. */
	__IO U32 CHAN_THRSEL;					/*!< A/D Channel Threshold Select Register. Specifies which set of threshold compare registers to use. */
	__IO U32 INTEN;						    /*!< A/D Interrupt Enable Register. This register contains enable bits that enable sequence-A, sequence-B, threshold compare and overrun interrupts. */
	__IO U32 FLAGS;						    /*!< A/D Flags Register. This register contains interrupt flags. - To be checked */
	__IO U32 TRM;							/*!< A/D Trim Register. */
} LPC_ADC_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
static LPC_ADC_T* const LPC_ADC[ADC_GROUP_MAXNUM] = {LPC_ADC0, LPC_ADC1};

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize the ADC peripheral according configs in adc_cfg.h
 * @param	Nothing
 * @param	Nithing	
 */
void ADC_vInit(void);

/**
 * @brief	Reset and initliasethe ADC peripheral
 * @param	Nothing
 * @return	Nothing
 * @note	
 */
void ADC_vReset(void);

/**
 * @brief	Shutdown the ADC peripheral
 * @param	Nothing
 * @return	Nothing
 * @note	
 */
/* Shutdown ADC */
STATIC INLINE void ADC_vShutdown(void)
{
	/* Shutdown ADC0 if enabled*/	
    #if (ENABLE == CFG_ADC0_ENABLE)	
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_ADC0);
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_ADC0_PD);
	#endif

	/* Shutdown ADC1 if enabled*/
    #if (ENABLE == CFG_ADC1_ENABLE)
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_ADC1);
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_ADC1_PD);
	#endif
}

/**
 * @brief	Start a sequencer with burst mode or oneshot mode by hw trigger
 * @param	teADC  		: ADC peripheral group
 * @param	tSeqIndex	: Sequencer to enable
 * @param	bBurst	    : true to set burst mode and trigger sources are ignored
 *                        false to clear burst mode and one shot trigger sources are activated
 * @return	Nothing
 */
void ADC_vStartSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex, bool bBurst);

/**
 * @brief	Software Start a sequencer only with oneshot mode
 * @param	teADC  		: ADC peripheral group
 * @param	tSeqIndex	: Sequencer to enable
 * @param	bBurst	    : true to set burst mode and trigger sources are ignored
 *                        false to clear burst mode and one shot trigger sources are activated
 * @return	Nothing
 */
void ADC_vSoftwareStartSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex);

/**
 * @brief	Stop a activated ADC seq
 * @param	teADC  		: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to disable
 * @return	Nothing
 */
void ADC_vStopSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex);

/**
 * @brief	Read an global seq ADC data register for MODE_EOS
 * @param	teADC  	: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to enable
 * @return	Current sample value of the ADC seq data register
 * @note    If ADC_SEQ_CTRL_MODE_EOC is set, ADC_nGetGlobalData must be called
 *          to get adc samples and meanwhile interrupt flag can be cleared automatically
 */
STATIC INLINE U32 ADC_nGetSeqData(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex)
{
	return ADC_SEQ_GDAT_RESULT(READ_REG(LPC_ADC[teADC]->SEQ_GDAT[teSeqIndex]));
}

/**
 * @brief	Read an individual channel sample ADC data register for MODE_EOS
 * @param	teADC  	: ADC peripheral group
 * @param	ubIndex	: 0-11 channel
 * @return	Current sample value of the ADC data register
 * @note	This function returns the value of the sample data
 */
STATIC INLINE U32 ADC_nGetData(ADC_GROUP_T teADC, U8 ubChIndex)
{
	return ADC_DR_RESULT(READ_REG(LPC_ADC[teADC]->DR[ubChIndex]));
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif/* CFG_ADC_USED */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
