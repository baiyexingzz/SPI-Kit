/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ADC.c                                                   $ */
/*                                                                            */
/*    $Revision:: 294                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-05 12:20:36 +0800 (周三, 05 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                     $  */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::   Low-level macros, definition, functions - ADC controller */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "adc.h"

#ifdef CFG_ADC_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * @brief Declare external dma callback
 */
#define DECLARE_ADC_CALLBACK(_Func_)  extern void _Func_(void)
#define DECLARE_ADC_THCMP_CALLBACK(_Func_)  extern void _Func_(U32 unThcmpChFlag)
#define DECLARE_ADC_OVR_CALLBACK(_Func_)  extern void _Func_(U32 unErrChFlag)

/* ADC Calibration Freq - constant 5MHz */
#define ADC_CALIBRATION_FREQ_KHZ          (500000UL)  
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
/**
 * @brief ADC callback table
 */
#ifdef ADC0_SEQA_CB
DECLARE_ADC_CALLBACK(ADC0_SEQA_CB);
#endif
#ifdef ADC0_SEQB_CB
DECLARE_ADC_CALLBACK(ADC0_SEQB_CB);
#endif
#ifdef ADC0_THCMP_CB
DECLARE_ADC_THCMP_CALLBACK(ADC0_THCMP_CB);
#endif
#ifdef ADC0_OVR_CB
DECLARE_ADC_OVR_CALLBACK(ADC0_OVR_CB);
#endif
#ifdef ADC1_SEQA_CB
DECLARE_ADC_CALLBACK(ADC1_SEQA_CB);
#endif
#ifdef ADC1_SEQB_CB
DECLARE_ADC_CALLBACK(ADC1_SEQB_CB);
#endif
#ifdef ADC1_THCMP_CB
DECLARE_ADC_THCMP_CALLBACK(ADC1_THCMP_CB);
#endif
#ifdef ADC1_OVR_CB
DECLARE_ADC_OVR_CALLBACK(ADC1_OVR_CB);
#endif
/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Set ADC clock rate for sync mode
 * @param	adc  	: teADC peripheral group
 * @param	unRate	: rate in Hz to set ADC clock to (maximum ADC_MAX_SAMPLE_RATE)
 * @return	Nothing
 * @note	This function will not work if the ADC is used with the ASYNC
 * ADC clock (set when the ADC_CR_ASYNMODE bit is on in the ADC CTRL register).
 */
STATIC INLINE void ADC_vSetClockRate(ADC_GROUP_T teADC, U32 unRate)
{
	/* Get ADC clock source to determine base ADC rate. IN sychronous mode(reset as default),
	   the ADC base clock comes from the system clock. In ASYNC mode, it
	   comes from the ASYNC ADC clock and this function doesn't work. */
	U32 div = CLOCK_nGetSysClock() / unRate;
	div = ((div == 0)? 0 : (div-1));

    SET_REG_FIELD_VALUE(LPC_ADC[teADC]->CTRL, ADC_CR_CLKDIV_MASK, ADC_CR_CLKDIV_BITPOS, div);
}

/**
 * @brief	Set entire CTRL of ADC0/1
 * @param	teADC  	: ADC peripheral group
 * @param	unCtrl		: Select an ADC_INSEL_* value for Channel 0 input selection
 * @return	Nothing
 */
STATIC INLINE void ADC_vSetCtrl(ADC_GROUP_T teADC, U32 unCtrl)
{	
	SET_REG_FIELD(LPC_ADC[teADC]->CTRL, ADC_CR_MASK, unCtrl);
}

/**
 * @brief	Set Trim register in ADC
 * @param	teADC  	: ADC peripheral group
 * @param	unTrim	: Trim value (ADC_TRIM_VRANGE_HIGHV or ADC_TRIM_VRANGE_LOWV)
 * @return	None
 */
STATIC INLINE void ADC_vSetTrim(ADC_GROUP_T teADC, U32 unTrim)
{
	SET_REG_FIELD(LPC_ADC[teADC]->TRM, ADC_TRIM_VRANGE_MASK, unTrim);
}

/**
 * @brief	Start ADC calibration
 * @param	teADC  	: ADC peripheral group
 * @return	Nothing
 * @note	Calibration is not done as part of Chip_ADC_Init(), but
 * is required after the call to Chip_ADC_Init() or after returning
 * from a power-down state. Calibration may alter the ADC_CR_ASYNMODE
 * and ADC_CR_LPWRMODEBIT flags ni the CTRL register.
 */
STATIC INLINE void ADC_vStartCalibration(ADC_GROUP_T teADC)
{
	/* Set calibration mode */
	SET_REG_FIELD(LPC_ADC[teADC]->CTRL, ADC_CR_CALMODEBIT, ADC_CR_CALMODEBIT);

	/* Clear ASYNC bit */
	SET_REG_FIELD(LPC_ADC[teADC]->CTRL, ADC_CR_ASYNMODE, 0);

	/* Setup ADC for about 500KHz (per UM) */
	ADC_vSetClockRate(teADC, ADC_CALIBRATION_FREQ_KHZ);

	/* Clearn low power bit */
	SET_REG_FIELD(LPC_ADC[teADC]->CTRL, ADC_CR_LPWRMODEBIT, 0);

	/* Calibration is only complete when ADC_CR_CALMODEBIT bit has cleared */
}

/**
 * @brief	Read if ADC calibration is done
 * @param	teADC  	: ADC peripheral group
 * @return	TRUE if calibration is complete, otherwise FALSE.
 */
STATIC INLINE bool ADC_bIsCalibrationDone(ADC_GROUP_T teADC)
{
	return (GET_REG_FIELD(LPC_ADC[teADC]->CTRL, ADC_CR_CALMODEBIT) == 0);
}

/**
 * @brief	Select input select for channel 0 of ADC0/1
 * @param	teADC  	: ADC peripheral group
 * @param	unInp   : Select an ADC_INSEL_* value for Channel 0 input selection
 * @return	Nothing
 */
STATIC INLINE void ADC_vSetCh0Input(ADC_GROUP_T teADC, U32 unInp)
{
	SET_REG_FIELD(LPC_ADC[teADC]->INSEL, ADC_INSEL_MASK, unInp);
}


/* Set ADC sequencer Ctrl fields */
/**
 * @brief	Set entire CTRL of ADC0/1
 * @param	teADC  	: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to start burst on
 * @param	unCtrl		: Select an ADC_INSEL_* value for Channel 0 input selection
 * @return	Nothing
 */
/* Example for setting up sequencer A 
 * ADC_SET_SEQCTRL(ADC_GROUP_0, ADC_SEQA_IDX, (
 * ADC_SEQ_CTRL_CHANSEL(0) | ADC_SEQ_CTRL_CHANSEL(1) | ADC_SEQ_CTRL_CHANSEL(2) |
 * ADC_SEQ_CTRL_HWTRIG_PIO0_2 | ADC_SEQ_CTRL_HWTRIG_POLPOS | ADC_SEQ_CTRL_MODE_EOS));
 */
STATIC INLINE void ADC_vSetSeqCtrl(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex, U32 unmask)
{
     SET_REG_FIELD(LPC_ADC[teADC]->SEQ_CTRL[teSeqIndex], (ADC_SEQ_CTRL_MASK&unmask), unmask);
}

/* Clear ADC sequencer Ctrl fields */
/**
 * @brief	Set entire CTRL of ADC0/1
 * @param	teADC  	: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to start burst on
 * @param	unCtrl		: Select an ADC_INSEL_* value for Channel 0 input selection
 * @return	Nothing
 */
/* Example for setting up sequencer A 
 * ADC_SET_SEQCTRL(ADC_GROUP_0, ADC_SEQA_IDX, (
 * ADC_SEQ_CTRL_CHANSEL(0) | ADC_SEQ_CTRL_CHANSEL(1) | ADC_SEQ_CTRL_CHANSEL(2) |
 * ADC_SEQ_CTRL_HWTRIG_PIO0_2 | ADC_SEQ_CTRL_HWTRIG_POLPOS | ADC_SEQ_CTRL_MODE_EOS));
 */
STATIC INLINE void ADC_vClearSeqCtrl(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex, U32 unmask)
{
     SET_REG_FIELD(LPC_ADC[teADC]->SEQ_CTRL[teSeqIndex], (ADC_SEQ_CTRL_MASK&unmask), 0);
}

/**
 * @brief	Starts sequencer burst mode - Repeated
 * @param	teADC  	: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to start burst on
 * @return	Nothing
 * @note	This function sets the BURST bit for the sequencer to force
 * continuous conversion. Use Chip_ADC_StopBurstSequencer() to stop the
 * ADC burst sequence.
 */
STATIC INLINE void ADC_vSetBurstSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex)
{
	ADC_vSetSeqCtrl(teADC, teSeqIndex, ADC_SEQ_CTRL_BURST);
}

/**
 * @brief	Stops sequencer burst mode - Oneshot
 * @param	teADC  	: ADC peripheral group
 * @param	teSeqIndex	: Sequencer to stop burst on
 * @return	Nothing
 */
STATIC INLINE void ADC_vClearBurstSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex)
{
	ADC_vClearSeqCtrl(teADC, teSeqIndex, ADC_SEQ_CTRL_BURST);
}

/**
 * @brief	Clear ADC interrupt bits (safe)
 * @param	teADC  	: ADC peripheral group
 * @param	unIntMask	: Interrupt values to be enabled (see notes)
 * @return	None
 */
STATIC INLINE void ADC_vDisableInt(ADC_GROUP_T teADC, U32 unIntMask)
{
	/* Read and write values may not be the same, write 0 to
	   undefined bits */
	SET_REG_FIELD(LPC_ADC[teADC]->INTEN, (unIntMask&ADC_INTEN_MASK), 0);
}

/**
 * @brief	Set ADC interrupt bits (safe)
 * @param	teADC  	: ADC peripheral group
 * @param	unIntMask	: Interrupt values to be enabled (see notes)
 * @return	None
 */
/* Set ADC interrupt bits (safe) */
STATIC INLINE void ADC_vEnableInt(ADC_GROUP_T teADC, U32 unIntMask)
{
	/* Read and write values may not be the same, write 0 to
	   undefined bits */
	SET_REG_FIELD(LPC_ADC[teADC]->INTEN, (unIntMask&ADC_INTEN_MASK), unIntMask);
}

/**
 * @brief	Get flags value in ADC
 * @param	teADC  	: ADC peripheral group
 * @param	unIntMask	: ADC flag masks, which to be to be queried
 * @return  true if int status is pending else not pending
 */
STATIC INLINE U32 ADC_bGetIntStatus(ADC_GROUP_T teADC, U32 unIntMask)
{
	return (GET_REG_FIELD(LPC_ADC[teADC]->FLAGS,(unIntMask&ADC_INTFLAGS_MASK)) != 0);
}

/**
 * @brief	Check if flags in ADC are set
 * @param	teADC  	: ADC peripheral group
 * @param	unIntFlags	: ADC flag masks, which to be to be queried
 * @return  The sepecific flag values
 */
STATIC INLINE U32 ADC_nGetIntStatus(ADC_GROUP_T teADC, U32 unIntMask)
{
	return GET_REG_FIELD(LPC_ADC[teADC]->FLAGS, (unIntMask&ADC_INTFLAGS_MASK));
}

/**
 * @brief	Clear flags value in ADC
 * @param	teADC  	: ADC peripheral group
 * @param	unIntFlags	: ADC flag masks, which to be to be queried
 * @return  Nothing
 */
STATIC INLINE void ADC_vClearIntStatus(ADC_GROUP_T teADC, U32 unIntMask)
{
	/* Write 1 to clear */
	WRITE_REG(LPC_ADC[teADC]->FLAGS, unIntMask);
}

/******************************************************************************/
/*                      PRIVATE SETUP FUNCTION IMPLEMENTATIONS                */
/******************************************************************************/
#define ADC_SETUP_CLOCKRATE(adc)      ADC_vSetClockRate(adc, (adc##_INIT_RATE))   
#define ADC_SETUP_CTRL(adc)           ADC_vSetCtrl(adc, (adc##_INIT_CTRL))
#define ADC_SETUP_TRIM(adc)           ADC_vSetTrim(adc, (adc##_INIT_TRIM))
#define ADC_SETUP_CH0(adc)            ADC_vSetCh0Input(adc, (adc##_INIT_TRIM))
#define ADC_SEQACTRL_CFGVAL(adc)      ((adc##_SEQA_CHSEL)|(adc##_SEQA_TRIGGER)|((adc##_SEQA_CTRL)&ADC_SEQ_CTRL_CFG_MASK))
#define ADC_SETUP_SEQACTRL(adc)       ADC_vSetSeqCtrl(adc, ADC_SEQA, ADC_SEQACTRL_CFGVAL(adc))
#define ADC_SEQBCTRL_CFGVAL(adc)      ((adc##_SEQB_CHSEL)|(adc##_SEQB_TRIGGER)|((adc##_SEQB_CTRL)&ADC_SEQ_CTRL_CFG_MASK))
#define ADC_SETUP_SEQBCTRL(adc)       ADC_vSetSeqCtrl(adc, ADC_SEQB, ADC_SEQBCTRL_CFGVAL(adc))
#define ADC_SETUP_INTEN(adc)          ADC_vEnableInt(adc,  (adc##_INTEN_CFG))

#if (ENABLE == CFG_ADC0_ENABLE)
STATIC INLINE void ADC_vSetupADC0(void)
{	
    /* Setup ADC clock rate for ADC0*/
	ADC_SETUP_CLOCKRATE(ADC0);
	
    /* Disable ADC interrupts */
	ADC_vDisableInt(ADC0, ADC_INTEN_MASK);

	/* Set ADC entire control options */
	ADC_SETUP_CTRL(ADC0);

	/* Use higher voltage trim for both ADCs */
	ADC_SETUP_TRIM(ADC0);

    /* Set specific channel 0 for ADC0 if need */ 
	ADC_SETUP_CH0(ADC0);
	
    /* Define sequence A for ADC0 if need */ 
	#ifdef ADC0_SEQA_CHSEL
	ADC_SETUP_SEQACTRL(ADC0);
	#endif    
	
	/* Define sequence B for ADC0 if need */ 
	#ifdef ADC0_SEQB_CHSEL
	ADC_SETUP_SEQBCTRL(ADC0);
	#endif

    /* Clear all pending interrupts */
	ADC_vClearIntStatus(ADC0, ADC_INTFLAGS_MASK);

    /* Enable interrupts for ADC0 */
	#ifdef ADC0_INTEN_CFG
    ADC_SETUP_INTEN(ADC0);
	#endif
}
#endif

#if (ENABLE == CFG_ADC1_ENABLE)
STATIC INLINE void ADC_vSetupADC1(void)
{	
    /* Setup ADC clock rate for ADC0*/
	ADC_SETUP_CLOCKRATE(ADC1);
	
    /* Disable ADC interrupts */
	ADC_vDisableInt(ADC1, ADC_INTEN_MASK);

	/* Set ADC entire control options */
	ADC_SETUP_CTRL(ADC1);

	/* Use higher voltage trim for both ADCs */
	ADC_SETUP_TRIM(ADC1);

    /* Set specific channel 0 for ADC0 if need */ 
	ADC_SETUP_CH0(ADC1);
	
    /* Define sequence A for ADC0 if need */ 
	#ifdef ADC1_SEQA_CHSEL
    ADC_SETUP_SEQACTRL(ADC1);
	#endif    
	
	/* Define sequence B for ADC0 if need */ 
	#ifdef ADC1_SEQB_CHSEL
    ADC_SETUP_SEQBCTRL(ADC1);
	#endif

    /* Clear all pending interrupts */
	ADC_vClearIntStatus(ADC1, ADC_INTFLAGS_MASK);

    /* Enable interrupts for ADC1 */
	#ifdef ADC1_INTEN_CFG
	ADC_SETUP_INTEN(ADC1);
	#endif
}
#endif

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize the ADC peripheral */
void ADC_vInit(void)
{
	/**
      * Initialize ADC0
	  */
    #if (ENABLE == CFG_ADC0_ENABLE)
	/* Power up ADC and enable ADC base clock */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_ADC0_PD);
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_ADC0);
	SYSCTL_vPeriphReset(RESET_ADC0);

	ADC_vDisableInt(ADC0, ADC_INTEN_MASK);

	/* Need to do a calibration after reset and before all initialization and trim */
	ADC_vStartCalibration(ADC0);
	while (!(ADC_bIsCalibrationDone(ADC0))) {;}

    /* Setup ADC0 according to adc_cfg */
	ADC_vSetupADC0();	
    #endif	

    /**
      * Initialize ADC1
	  */
    #if (ENABLE == CFG_ADC1_ENABLE)
	/* Power up ADC and enable ADC base clock */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_ADC1_PD);
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_ADC1);
	SYSCTL_vPeriphReset(RESET_ADC1);

	ADC_vDisableInt(ADC1, ADC_INTEN_MASK);
    
	/* Need to do a calibration after reset and before all initialization and trim */
	ADC_vStartCalibration(ADC1);
	while (!(ADC_bIsCalibrationDone(ADC1))) {;}

	/* Setup ADC1 according to adc_cfg */
	ADC_vSetupADC1();
    #endif	
}

/* Reset the ADC peripheral as initliased status*/
void ADC_vReset(void)
{
	ADC_vShutdown();
	ADC_vInit();
}

/* Start a sequencer with burst mode or oneshot mode by hw trigger */
void ADC_vStartSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex, bool bBurst)
{
    /* Clear all pending interrupts */
	ADC_vClearIntStatus(teADC, ADC_nGetIntStatus(teADC, ADC_INTFLAGS_MASK));

    /* Enable ADC seq */
	ADC_vSetSeqCtrl(teADC, teSeqIndex, ADC_SEQ_CTRL_SEQ_ENA);

	/* Set burst mode or not */
    if(true == bBurst){
		ADC_vSetBurstSeq(teADC, teSeqIndex);
    }
	else{
		ADC_vClearBurstSeq(teADC, teSeqIndex);
	}
}

/* Software Start a sequencer only with oneshot mode */
void ADC_vSoftwareStartSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex)
{
    /* Stop seq firstly */
    ADC_vStopSeq(teADC, teSeqIndex);

	/* Software trigge */
	ADC_vSetSeqCtrl(teADC, teSeqIndex, ADC_SEQ_CTRL_START);
}

/* Stop a activated ADC seq */
void ADC_vStopSeq(ADC_GROUP_T teADC, ADC_SEQ_T teSeqIndex)
{
    /* Disable ADC seq and clear Start/Burst bits */
	ADC_vClearSeqCtrl(teADC, teSeqIndex, (ADC_SEQ_CTRL_SEQ_ENA|ADC_SEQ_CTRL_START|ADC_SEQ_CTRL_BURST));

	/* Clear all pending interrupts */
	ADC_vClearIntStatus(teADC, ADC_nGetIntStatus(teADC, ADC_INTFLAGS_MASK));
}
#endif /* CFG_ADC_USED */
/******************************************************************************/
/*                      ISR FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/** 
 *   ISR callbacks for ADC controllers
 **/
/* ISR for ADC0 SeqA */
void ADC0_SeqA_vISR(void)
{
	#if (ENABLE == CFG_ADC0_ENABLE)
   	if(ADC_bGetIntStatus(ADC0, ADC_INTFLAGS_SEQA_INT_MASK)){
		/* Only if EOS mode, Seq Int flag needs to be cleared. 
		   If EOC mode, Seq Int flag is cleared automatically by getting GDATA */
	    ADC_vClearIntStatus(ADC0, ADC_INTFLAGS_SEQA_INT_MASK);
		
        /* CallbackBody */
		#ifdef ADC0_SEQA_CB
		ADC0_SEQA_CB();
		#endif
	}
	#endif
}

/* ISR for ADC0 SeqB */
void ADC0_SeqB_vISR(void)
{
	#if (ENABLE == CFG_ADC0_ENABLE)
   	if(ADC_bGetIntStatus(ADC0, ADC_INTFLAGS_SEQB_INT_MASK)){
		/* Only if EOS mode, Seq Int flag is cleared. 
		   If EOC mode, Seq Int flag is cleared automatically by getting GDATA */
	    ADC_vClearIntStatus(ADC0, ADC_INTFLAGS_SEQB_INT_MASK);
		
        /* CallbackBody */
		#ifdef ADC0_SEQB_CB
		ADC0_SEQB_CB();
		#endif
	}
	#endif
}

/* ISR for ADC0 ThCMP */
void ADC0_THCMP_vISR(void)
{
	#if (ENABLE == CFG_ADC0_ENABLE)	
	U32 flags = ADC_nGetIntStatus(ADC0, ADC_INTFLAGS_THCMP_ALLMASK);
   	if(0 != tmpflags){	
        ADC_vClearIntStatus(ADC0, flags);
		
        /* CallbackBody */
		#ifdef ADC0_THCMP_CB
		ADC0_THCMP_CB(flags);
		#endif		
	}
	#endif
}

/* ISR for ADC0 Overrun */
void ADC0_OVR_vISR(void)
{
	#if (ENABLE == CFG_ADC0_ENABLE)	
	U32 flags = ADC_nGetIntStatus(ADC0, ADC_INTFLAGS_OVRRUN_ALLMASK);
   	if(0 != flags){	
        ADC_vClearIntStatus(ADC0, flags);
		
        /* CallbackBody */
		#ifdef ADC0_OVR_CB
		ADC0_OVR_CB(flags);
		#endif		
	}
	#endif
}

/* ISR for ADC1 SeqA */
void ADC1_SeqA_vISR(void)
{
	#if (ENABLE == CFG_ADC1_ENABLE)
   	if(ADC_bGetIntStatus(ADC1, ADC_INTFLAGS_SEQA_INT_MASK)){
		/* Only if EOS mode, Seq Int flag is cleared. 
		   If EOC mode, Seq Int flag is cleared automatically by getting GDATA */
	    ADC_vClearIntStatus(ADC1, ADC_INTFLAGS_SEQA_INT_MASK);

        /* CallbackBody */
		#ifdef ADC1_SEQA_CB
		ADC1_SEQA_CB();
		#endif
	}
	#endif
}

/* ISR for ADC0 SeqB */
void ADC1_SeqB_vISR(void)
{
	#if (ENABLE == CFG_ADC1_ENABLE)
   	if(ADC_bGetIntStatus(ADC1, ADC_INTFLAGS_SEQB_INT_MASK)){
		/* Only if EOS mode, Seq Int flag is cleared. 
		   If EOC mode, Seq Int flag is cleared automatically by getting GDATA */
	    ADC_vClearIntStatus(ADC1, ADC_INTFLAGS_SEQB_INT_MASK);
		
        /* CallbackBody */
		#ifdef ADC1_SEQB_CB
		ADC1_SEQB_CB();
		#endif
	}
	#endif
}

/* ISR for ADC0 ThCMP */
void ADC1_THCMP_vISR(void)
{
	#if (ENABLE == CFG_ADC1_ENABLE)	
	U32 flags = ADC_nGetIntStatus(ADC1, ADC_INTFLAGS_THCMP_ALLMASK);
   	if(0 != flags){	
        ADC_vClearIntStatus(ADC1, flags);
		
        /* CallbackBody */
		#ifdef ADC1_THCMP_CB
		ADC1_THCMP_CB(flags);
		#endif		
	}
	#endif
}

/* ISR for ADC0 Overrun */
void ADC1_OVR_vISR(void)
{
	#if (ENABLE == CFG_ADC1_ENABLE)	
	U32 flags = ADC_nGetIntStatus(ADC1, ADC_INTFLAGS_OVRRUN_ALLMASK);
   	if(0 != flags){	
        //ADC_vClearIntStatus(ADC1, flags);
		
        /* CallbackBody */
		#ifdef ADC1_OVR_CB
		ADC1_OVR_CB(flags);
		#endif		
	}
	#endif
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

