/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DAC.c                                                 $   */
/*                                                                            */
/*    $Revision:: 294                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-05 12:20:36 +0800 (周三, 05 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - DAC             */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "dac.h"

#if (ENABLE == CFG_DAC_ENABLE)
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
#if DAC_HW_INT_TRIGSRC_USED
/**
 * @brief	Set Interrupt/DMA trigger source as Internal timer, enable timer before this call
 * @param   bInternal	: If TRUE indicates that the internal trigger source
 *						  else it is the external trigger source
 * @return	Nothing
 */
STATIC INLINE void DAC_vSetTrigSrcInternal(bool bInternal)
{
	U32 temp = ((bInternal)? (0):(DAC_TRIG_SRC_BIT));
	
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED,  0);
	/* 0: internal src; 1: external src */
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_TRIG_SRC_BIT, temp);
}

/**
 * @brief	Enable/Disable Internal Timer, CNTVAL should be loaded before enabling timer
 * @param   bEnable	: If TRUE indicates to enable Internal Timer
 *				      else to disable Internal Timer
 * @return	Nothing
 */
STATIC INLINE void DAC_vEnableIntTimer(bool bEnable)
{
    U32 temp = ((bEnable)? (DAC_TIM_ENA_BIT):(0));
	
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED, 0);
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_TIM_ENA_BIT, temp);
}

/**
 * @brief	Set reload value for interrupt/DMA timer
 * @param	periodHz	: time out to reload for interrupt/DMA timer.
 *						  time out rate will be SysClk/(time_out + 1) 
 *                        Frequency of Timer interrupts in ms unit
 
 * @return	Nothing
 */
STATIC INLINE void DAC_vSetRelInterval(U32 unRateHz)
{
	U32 temp = CLOCK_nGetSysClockMHz() / unRateHz;
	temp = ((0 == temp)? 0:(temp-1)); 
	
	WRITE_REG(LPC_DAC->CNTVAL, DAC_CNT_VALUE(temp));
}
#endif

#if DAC_HW_EXT_TRIGSRC_USED
/**
 * @brief	Set Polarity for external trigger pin
 * @param	Nothing
 * @param   unPolarity	:DAC_POLARITY_FALLING or DAC_POLARITY_RISING
 * @return	Nothing
 */
STATIC INLINE void DAC_vSetExtTriggerPolarity(U32 unPolarity)
{
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED, 0);
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_POLARITY, unPolarity);	
}

/**
 * @brief	Enable/Disable Sync Bypass, only if external trigger is in sync with SysClk
 * @param   bEnable	: If TRUE indicates to enable Sync Bypass
 *				      else to disable Sync Bypass
 * @return	Nothing
 */
STATIC INLINE void DAC_vEnableSyncBypass(bool bEnable)
{
	U32 temp = ((bEnable)? (DAC_SYNC_BYPASS):(0));
	
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED, 0);
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_SYNC_BYPASS, temp);
}
#endif

/**
 * @brief	Enable/Disable DAC Shut Off
 * @param   bEnable	: If TRUE indicates to enable DAC Shut Off
 *				      else to disable DAC Shut Off
 * @return	Nothing
 */
STATIC INLINE void DAC_vEnableShutOff(bool bEnable)
{
    U32 temp = ((bEnable)? (DAC_SHUTOFF_ENA):(0));
	
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED, 0);
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_SHUTOFF_ENA, temp);	
}

/**
 * @brief	Enable/Disable DAC Double Buffer
 * @param   bEnable	: If TRUE indicates to enable IDAC Double Buffer
 *				      else to disable DAC Double Buffer
 * @return	Nothing
 */
STATIC INLINE void DAC_vEnableDoubleBuffer(bool bEnable)
{
    U32 temp = ((bEnable)? (DAC_DBLBUF_ENA):(0));
	
    SET_REG_FIELD(LPC_DAC->CTRL, DAC_CTRL_UNUSED, 0);	
	SET_REG_FIELD(LPC_DAC->CTRL, DAC_DBLBUF_ENA, temp);
}

/**
 * @brief	Get status for interrupt/DMA time out
 * @param	Nothing
 * @return	TRUE if interrupt/DMA flag is set else returns FALSE
 */
STATIC INLINE bool DAC_bGetIntStatus(void)
{
	return ((GET_REG_FIELD(LPC_DAC->CTRL, DAC_INT_DMA_FLAG))!= 0);
}

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initial DAC configuration  */
void DAC_vInit(void)
{
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_DAC_PD);
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_DAC);

    #if (ENABLE == DAC_HW_TRIGSRC)
        DAC_vEnableDoubleBuffer(true); /* Enable double bufferring */ 
	
	    /* Internal trigger source */
	    #if (ENABLE == DAC_INTERNAL_TRIGSRC)	    
		DAC_vEnableIntTimer(true);/* Enable internal timer */
		
	    DAC_vSetRelInterval(DAC_RELOAD_FREQ_HZ);	    

		DAC_vSetTrigSrcInternal(true);		
	    /* External trigger source */	
        #else	    
	    DAC_vEnableIntTimer(false);/* Disable internal timer */

	    #if (ENABLE == DAC_SYNC_BYPASS)
	    DAC_vEnableSyncBypass(true);
		#else
		DAC_vEnableSyncBypass(false);
	    #endif

        DAC_vSetExtTriggerPolarity(DAC_EXTTRIG_POLARITY);

        DAC_vSetTrigSrcInternal(false);
	    #endif
	#else
        DAC_vEnableDoubleBuffer(false);/* Disable double bufferring */ 
	#endif

	/* External shutoff */
	#if (ENABLE == DAC_EXT_SHUTOFF)
    	DAC_vEnableShutOff(true);
    #else
    	DAC_vEnableShutOff(false);
    #endif
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

#endif /* CFG_DAC_ENABLE */
/******************************************************************************/
/*                      ISR HANDLER FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for DAC
 **/
void DAC_vISR()
{
    #if(ENABLE == CFG_DAC_ENABLE)
	if(DAC_bGetIntStatus()){
		/* Automatically clear interrupt flag by writting DAC_VAL */
		
        /* Callback body to reload a new DAC VAL from CPU or DMA engine */
		#ifdef DAC_CB
		DAC_CB();
		#endif
	}
    #endif
}
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

