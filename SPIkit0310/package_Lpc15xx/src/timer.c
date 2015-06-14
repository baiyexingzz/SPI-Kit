/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Timer.c                                                $  */
/*                                                                            */
/*    $Revision:: 343                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-25 10:27:38 +0800 (周二, 25 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description:: Low-level timers macros and functions - MRT, RIT and SysTick*/
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "timer.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#if (ENABLE == CFG_TIMER_MRT)
#define   MAX_MRT_INTERVAL_US         (233016UL)  /* 0x00FFFFFF/72 */
#if (TIMER1_INTERVAL > MAX_MRT_INTERVAL_US)
  #error "Error: TIMER_1(MRT_TIMER_0) interval value cannot be bigger than MAX_MRT_INTERVAL_US"
#endif
#if (TIMER2_INTERVAL > MAX_MRT_INTERVAL_US)
  #error "Error: TIMER_2(MRT_TIMER_1) interval value cannot be bigger than MAX_MRT_INTERVAL_US"
#endif
#if (TIMER3_INTERVAL > MAX_MRT_INTERVAL_US)
  #error "Error: TIMER_3(MRT_TIMER_2) interval value cannot be bigger than MAX_MRT_INTERVAL_US"
#endif
#if (TIMER4_INTERVAL > MAX_MRT_INTERVAL_US)
  #error "Error: TIMER_4(MRT_TIMER_3) interval value cannot be bigger than MAX_MRT_INTERVAL_US"
#endif
#endif

#if (ENABLE == CFG_TIMER_RIT)
#define   MAX_RIT_INTERVAL_US         (15270994830ULL)  /* 0x0000FFFFFFFFFFFF/72 */
#if (TIMER5_INTERVAL > MAX_RIT_INTERVAL_US)
  #error "Error: TIMER_5(RIT_TIMER) interval value cannot be bigger than MAX_RIT_INTERVAL_US"
#endif
#endif
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define DECLARE_TIMER_HANDLER(_Func_) extern void _Func_(void);

#ifdef TIMER0_CB
DECLARE_TIMER_HANDLER(TIMER0_CB)
#endif

#ifdef TIMER1_CB
//DECLARE_TIMER_HANDLER(TIMER1_CB)
#endif

#ifdef TIMER2_CB
//DECLARE_TIMER_HANDLER(TIMER2_CB)
#endif

#ifdef TIMER3_CB
DECLARE_TIMER_HANDLER(TIMER3_CB)
#endif

#ifdef TIMER4_CB
DECLARE_TIMER_HANDLER(TIMER4_CB)
#endif

#ifdef TIMER5_CB
DECLARE_TIMER_HANDLER(TIMER5_CB)
#endif

#define IsSYSTICKChannel(ch)  (SYSTICK_TIMER == ch)
#define SYSTICKChannel(ch)    (ch)

#if (ENABLE == CFG_TIMER_MRT)
#define IsMRTChannel(ch)      ((MRT_CHANNEL_0 == ch)||(MRT_CHANNEL_1 == ch)||\
	                           (MRT_CHANNEL_2 == ch)||(MRT_CHANNEL_3 == ch))
#define MRTChannel(ch)        (ch - MRT_CHANNEL_0)
#define MRTMode(mode)         ((MODE_REPEAT == mode)? MRT_MODE_REPEAT:MRT_MODE_ONESHOT)
#endif

#if (ENABLE == CFG_TIMER_RIT)
#define IsRITChannel(ch)      (RIT_TIMER == ch)
#define RITChannel(ch)        (ch - RIT_TIMER)
#endif
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Get interrupt status of system tick timer
 * @param	tCH: Timer channel
 * @return	Nothing
 */
STATIC INLINE bool TIMER_SYSTICK_bGetIntStatus(void)
{
    return (GET_REG_FIELD(SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk) != 0);
}

/**
 * @brief	Clear interrupt status of system tick timer
 * @param	tCH: Timer channel
 * @return	Nothing
 */
STATIC INLINE void TIMER_SYSTICK_vClearIntStatus(void)
{
    SET_REG_FIELD(SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk, 0);
}

#if (ENABLE == CFG_TIMER_MRT)
/**
 * @brief	Sets the timer mode (repeat or one-shot)
 * @param	tCH       : Timer channel
 * @param   teMode    : Timer mode
 * @return	Nothing
 */
STATIC INLINE void TIMER_MRT_vSetMode(TIMER_CH_T teCH, MRT_MODE_T teMode)
{
	SET_REG_FIELD(LPC_MRT->CHANNEL[MRTChannel(teCH)].CTRL, 
		          MRT_CTRL_MODE_MASK, 
		          (U32)teMode);
}

/**
 * @brief	Start the timer
 * @param	tCH       : Timer channel
 * @return	Nothing
 */
STATIC INLINE void TIMER_MRT_vIntEnable(TIMER_CH_T teCH)
{
	SET_REG_FIELD(LPC_MRT->CHANNEL[MRTChannel(teCH)].CTRL, 
		          MRT_CTRL_INTEN_MASK, 
		          MRT_CTRL_INTEN_MASK);
}

/**
 * @brief	Stop the timer
 * @param	tCH       : Timer channel
 * @return	Nothing
 */
STATIC INLINE void TIMER_MRT_vIntDisable(TIMER_CH_T teCH)
{
	SET_REG_FIELD(LPC_MRT->CHANNEL[MRTChannel(teCH)].CTRL, 
		          MRT_CTRL_INTEN_MASK, 
		          0);
}

/**
 * @brief	Sets the timer interval value in ns uint
 * @param	tCH       : Timer channel
 * @param   unUs :   The interval timeout (24-bits) - us unit 
 * @return	Nothing
 * @note	Setting bit 31 in timer time interval register causes the time interval value
 * to load immediately, otherwise the time interval value will be loaded in
 * next timer cycle
 */
/*Sets the timer interval value in us unit */
STATIC INLINE void TIMER_MRT_vSetInterval(TIMER_CH_T teCH, U32 unUs)
{
	U32 interval = CLOCK_nGetSysClockMHz()*unUs;
	WRITE_REG(LPC_MRT->CHANNEL[MRTChannel(teCH)].INTVAL, ((interval&MRT_INTVAL_IVALUE)|MRT_INTVAL_LOAD));
}

/**
 * @brief	Clears the interrupt pending status for one or more MRT channels
 * @param	tCH       : Timer channel
 * @return	Nothing
 * @note	Use this function to clear interrupt pending states in
 * a single call via the IRQ_FLAG register. Performs the same function 
 * for a single channel.
 */
STATIC INLINE void TIMER_MRT_vClearChIntStatus(TIMER_CH_T teCH)
{
	/*Write 1 to clear channel's interrupt status*/
	WRITE_REG(LPC_MRT->IRQ_FLAG, MRTn_INTFLAG(MRTChannel(teCH)));
}

/**
 * @brief	Returns the interrupt pending status for a singel MRT channel
 * @param	tCH       : Timer channel
 * @return	IRQ pending channel number
 */
STATIC INLINE bool TIMER_MRT_bGetChIntStatus(TIMER_CH_T teCH)
{
	return (GET_REG_FIELD(LPC_MRT->IRQ_FLAG, (MRTn_INTFLAG(MRTChannel(teCH))&MRT_INTFLAG_MASK)) != 0);
}

/**
 * @brief	Start a MRT channel
 * @param	tCH       : Timer channel
 * @param   unUs      :   The interval timeout (24-bits) - us unit 
 * @param   teMode    : Timer mode 
 * @return	Nothing
 */
STATIC INLINE void TIMER_MRT_vStart(TIMER_CH_T tCH, U32 unUs, MRT_MODE_T tMode)
{
	TIMER_MRT_vClearChIntStatus(tCH);
	TIMER_MRT_vSetInterval(tCH, unUs);
	TIMER_MRT_vSetMode(tCH, tMode);
}

/**
 * @brief	Initializes the MRT
 * @param   Nothing
 * @return	Nothing
 */
STATIC INLINE void TIMER_MRT_vInit(void)
{
	/* Enable the clock to the register interface */
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_MRT);

	/* Reset MRT */
	SYSCTL_vPeriphReset(RESET_MRT);

    /*Disable int for MRT channels */
	TIMER_vStop(MRT_CHANNEL_0);
	#ifdef TIMER1_CB
	TIMER_MRT_vIntEnable(MRT_CHANNEL_0);
	#else
	TIMER_MRT_vIntDisable(MRT_CHANNEL_0);
	#endif
	
	TIMER_vStop(MRT_CHANNEL_1);
	#ifdef TIMER2_CB
	TIMER_MRT_vIntEnable(MRT_CHANNEL_1);
	#else
	TIMER_MRT_vIntDisable(MRT_CHANNEL_1);
	#endif
	
	TIMER_vStop(MRT_CHANNEL_2);
	#ifdef TIMER3_CB
	TIMER_MRT_vIntEnable(MRT_CHANNEL_2);
	#else
	TIMER_MRT_vIntDisable(MRT_CHANNEL_2);
	#endif
	
	TIMER_vStop(MRT_CHANNEL_3);
	#ifdef TIMER4_CB
	TIMER_MRT_vIntEnable(MRT_CHANNEL_3);
	#else
	TIMER_MRT_vIntDisable(MRT_CHANNEL_3);
	#endif
}
#endif

#if (ENABLE == CFG_TIMER_RIT)
/**
 * @brief	Safely sets CTRL register field bits
 * @param	ctrlmask: Control value
 * @return	Nothing
 */
STATIC INLINE void TIMER_RIT_vSetCtrl(U32 ctrlmask)
{
	SET_REG_FIELD(LPC_RITIMER->CTRL, 
		         (RIT_CTRL_MASK & ctrlmask), 
		          ctrlmask);
}

/**
 * @brief	Safely clears CTRL register bits
 * @param	ctrlmask : RIT bits to be cleared, one or more RIT_CTRL_* values
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vClearCtrl(U32 ctrlmask)
{
	SET_REG_FIELD(LPC_RITIMER->CTRL, 
		          (RIT_CTRL_MASK & ctrlmask),
		           0);
}

/**
 * @brief	Enable Timer
 * @param	Nothing
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vEnable(void)
{
	TIMER_RIT_vSetCtrl(RIT_CTRL_TEN);
}

/**
 * @brief	Disable Timer
 * @param	Nothing
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vDisable(void)
{
	TIMER_RIT_vClearCtrl(RIT_CTRL_TEN);
}

/**
 * @brief	Enables automatic counter clear on compare
 * @param	Nothing
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vEnableCompClear(void)
{
	TIMER_RIT_vSetCtrl(RIT_CTRL_ENCLR);
}

/**
 * @brief	Set a tick value for the interrupt to time out
 * @param	unVal: tick value
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vSetCompareValue(U64 unVal)
{
	WRITE_REG(LPC_RITIMER->COMPVAL, (U32)(unVal&0xFFFFFFFF));
	WRITE_REG(LPC_RITIMER->COMPVAL_H, (U32)((unVal >> 32)&0xFFFFFFFF));
} 

/**
 * @brief	Sets the timer interval value in us uint
 * @param   unUs : The interval timeout (48-bits) - us unit 
 * @return	Nothing
 */
STATIC INLINE void TIMER_RIT_vSetInterval(U64 unUs)
{
	/* Determine approximate compare value based on clock rate and passed interval */
	U64 cmp_value = ((U64)CLOCK_nGetSysClockMHz()*(U64)unUs);	

	/* Set timer compare value and periodic mode */
	TIMER_RIT_vSetCompareValue(cmp_value);
} 

/*  */
/**
 * @brief	Sets the current timer Counter value
 * @param   unCount : timer Counter value
 * @return	Nothing
 */
STATIC INLINE void RIT_vSetCounter(U64 unCount)
{
	WRITE_REG(LPC_RITIMER->COUNTER, (U32)(unCount&0xFFFFFFFF));
	WRITE_REG(LPC_RITIMER->COUNTER, (U32)((unCount >> 32)&0xFFFFFFFF));
}

/**
 * @brief	Check whether timer interrupt is pending
 * @param	Nothing
 * @return	true if the interrupt is pending, otherwise false
 */
STATIC INLINE bool TIMER_RIT_bGetIntStatus(void)
{
	return (GET_REG_FIELD(LPC_RITIMER->CTRL, RIT_CTRL_INT) != 0);
}

/**
 * @brief	Clears the timer interrupt pending state
 * @param	Nothing
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vClearIntStatus()
{
	TIMER_RIT_vSetCtrl(RIT_CTRL_INT);
}

/**
 * @brief	Start RIT timer
 * @param   unUs : The interval timeout (48-bits) - us unit 
 * @return	None
 */
STATIC INLINE void TIMER_RIT_vStart(U64 unUs)
{
    TIMER_RIT_vDisable();  /* Stop RIT firstly */
  
    RIT_vSetCounter(0);
    TIMER_RIT_vSetInterval(unUs);
	TIMER_RIT_vEnable();
}

/**
 * @brief	Initialize the RIT
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void TIMER_RIT_vInit(void)
{
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_RIT);
	SYSCTL_vPeriphReset(RESET_RIT);

    /* Default disable */
	TIMER_RIT_vDisable();

    /* halt when debug */
	TIMER_RIT_vSetCtrl(RIT_CTRL_ENBR);
	
	/* Enable Comp Clear as default */
	TIMER_RIT_vEnableCompClear();
}
#endif

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Init all timer resource for LPC1517 */
void TIMER_vInit(void)
{
    /* Initialize Sys Tick */
	/* Config system tick timer after Cortex system is initiazlied OK */
	SysTick_Config(CLOCK_nGetSysTick());
	
	/* Initialize MRT timer including 4 channels */
    #if (ENABLE == CFG_TIMER_MRT)
    /* Powup and reset MRT */
	TIMER_MRT_vInit();  
	#endif/* CFG_TIMER_MRT */

	/* Initialize RIT timer including 1 channel */
    #if (ENABLE == CFG_TIMER_RIT)
	/* Powup and reset RIT */
    TIMER_RIT_vInit();	
    #endif/* CFG_TIMER_RIT */
}

/* Start timer with interval and mode */
void TIMER_vStartEx(TIMER_CH_T tCH, U64 unUs, TIMER_MODE_T tMode)
{	
	if(IsSYSTICKChannel(tCH)){
		/* SysTick interval is fixed 10ms */	
	}	
	#if (ENABLE == CFG_TIMER_MRT)
    else if(IsMRTChannel(tCH)){		
	    TIMER_MRT_vStart(tCH, (U32)unUs, MRTMode(tMode));
    }
	#endif	
	#if (ENABLE == CFG_TIMER_RIT)
	else if(IsRITChannel(tCH)){
		TIMER_RIT_vStart(unUs);
	}
	#endif	
}

/* Start a timer using default configurations */
void TIMER_vStart(TIMER_CH_T tCH)
{
	if(IsSYSTICKChannel(tCH)){
		/* SysTick is enabled always */	
	}
	#if (ENABLE == CFG_TIMER_MRT)
    else if(IsMRTChannel(tCH)){
	   if(TIMER_1 == (tCH)){
          TIMER_MRT_vStart(tCH, TIMER1_INTERVAL, TIMER1_MODE);
	   }
	   else if(TIMER_2 == (tCH)){
          TIMER_MRT_vStart(tCH, TIMER2_INTERVAL, TIMER2_MODE);
	   }
	   else if(TIMER_3 == (tCH)){
          TIMER_MRT_vStart(tCH, TIMER3_INTERVAL, TIMER3_MODE);
	   }
	   else if(TIMER_4 == (tCH)){
          TIMER_MRT_vStart(tCH, TIMER4_INTERVAL, TIMER4_MODE);
	   }	   
    }
	#endif
	#if (ENABLE == CFG_TIMER_RIT)
	else if(IsRITChannel(tCH)){
	   /* Set default cfg compare value */
	   TIMER_RIT_vStart(TIMER5_INTERVAL);
	}
	#endif
}

/* stop a timer */
void TIMER_vStop(TIMER_CH_T tCH)
{
	if(IsSYSTICKChannel(tCH)){
		/* SysTick is enabled always */	
	}
	#if (ENABLE == CFG_TIMER_MRT)
    else if(IsMRTChannel(tCH)){		
	    TIMER_MRT_vSetInterval(tCH, 0);    
	}
	#endif
	#if (ENABLE == CFG_TIMER_RIT)
	else if(IsRITChannel(tCH)){
	   TIMER_RIT_vDisable();
	}
	#endif
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                      ISR CALLBACK HANDLER IMPLEMENTATIONS                      */
/******************************************************************************/
/* ISR for timers */
void SYSTICK_vISR(void)
{
    if(TIMER_SYSTICK_bGetIntStatus())
    {
        TIMER_SYSTICK_vClearIntStatus();

        /* Callback handler */
		#ifdef TIMER0_CB
		TIMER0_CB();
		#endif
    }
}
void MRT_vISR(void)
{
	#if (ENABLE == CFG_TIMER_MRT)
	//TIMER_MRT_vClearChIntStatus(MRT_CHANNEL_0);
	
	
    if(TIMER_MRT_bGetChIntStatus(MRT_CHANNEL_0))
    {
        TIMER_MRT_vClearChIntStatus(MRT_CHANNEL_0);

        MRT_vISRz();
		//#ifdef TIMER1_CB
	//	TIMER1_CB();
		//#endif
    }
	
	/*	if(TIMER_MRT_bGetChIntStatus(MRT_CHANNEL_1))
    {
        TIMER_MRT_vClearChIntStatus(MRT_CHANNEL_1);
		MRT_vISR1z_cmdExe();
		
		//#ifdef TIMER2_CB
		//TIMER2_CB();
		//#endif		
    }

if(TIMER_MRT_bGetChIntStatus(MRT_CHANNEL_2))
    {
        TIMER_MRT_vClearChIntStatus(MRT_CHANNEL_2);

		
		#ifdef TIMER3_CB
		TIMER3_CB();
		#endif		
    }

	if(TIMER_MRT_bGetChIntStatus(MRT_CHANNEL_3))
    {
        TIMER_MRT_vClearChIntStatus(MRT_CHANNEL_3);

		
		#ifdef TIMER4_CB
		TIMER4_CB();
		#endif			
    }*/
	#endif    
}

void RIT_vISR(void)
{
	#if (ENABLE == CFG_TIMER_RIT)
    if(TIMER_RIT_bGetIntStatus())
    {
		TIMER_RIT_vClearIntStatus();
		
	    /* Callback handler */
	    #ifdef TIMER5_CB
	    TIMER5_CB();
	    #endif	
	}
	#endif
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

