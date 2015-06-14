/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Watchdog.c                                            $   */
/*                                                                            */
/*    $Revision:: 296                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-05 12:53:00 +0800 (周三, 05 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - Watchdog        */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "watchdog.h"

#if(ENABLE == CFG_WATCHDOG_ENABLE)
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#if (WATCHDOG_TIMEOUT_MS > WWDT_TIMEOUT_MS_MAX)
  #error "Error: Watchdog timeout cannot be bigger than WWDT_TIMEOUT_MS_MAX - 133417 ms"
#elif (WATCHDOG_TIMEOUT_MS < WWDT_TIMEOUT_MS_MIN)
  #error "Error: Watchdog timeout cannot be bigger than WWDT_TIMEOUT_MS_MIN - 2 ms"
#endif

#if (WATCHDOG_WRN_TIME_MS > WWDT_WRN_TIME_MAX)
  #error "Error: Watchdog warning time cannot be bigger than WWDT_WRN_TIME_MAX - 8 ms"
#endif

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define DECLARE_WATCHDOG_WRN_CALLBACK(_Func_)  extern void _Func_(void);

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
#ifdef WATCHDOG_WRN_CB
DECLARE_WATCHDOG_WRN_CALLBACK(WATCHDOG_WRN_CB);
#endif

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Set WDT feed(timeout) constant value used for feed
 * @param	unTimeout	: WDT timeout in ms 
 * @return	nothing
 * @note	between WWDT_TIMEOUT_MS_MIN and WWDT_TIMEOUT_MS_MAX
 */
STATIC INLINE void WATCHDOG_vSetTimeOut(U32 unTimeout)
{
	WRITE_REG(LPC_WWDT->TC, WWDT_TICKS_VAL(unTimeout));
}

#if(ENABLE == WATCHDOG_WRN_MODE)
/**
 * @brief	Set WWDT warning interrupt
 * @param	unTimeout	: WDT warning in ticks, between 0 and 1023
 * @return	None
 * @note	This is the number of timeout in ms after the watchdog interrupt that the
 * warning interrupt will be generated.
 */
STATIC INLINE void WATCHDOG_vSetWarning(U32 unTimeout)
{
    WRITE_REG(LPC_WWDT->WARNINT, WWDT_WRN_TICKS_VAL(unTimeout));	
}
#endif

/**
 * @brief	Set WWDT window time
 * @param	unTimeout	: WDT timeout in ticks, between WWDT_TICKS_MIN and WWDT_TICKS_MAX
 * @return	None
 * @note	The watchdog timer must be fed between the timeout from the WWDT_SetTimeOut()
 * function and this function, with this function defining the last tick before the
 * watchdog window interrupt occurs.
 */
STATIC INLINE void WATCHDOG_vSetWindow(U32 unTimeout)
{
	WRITE_REG(LPC_WWDT->WINDOW, unTimeout);
}

/**
 * @brief	Enable a watchdog timer option as 1
 * @param	unOption	: An option
 *					  WWDT_WDMOD_WDEN, WWDT_WDMOD_WDRESET, and WWDT_WDMOD_WDPROTECT
 * @return	None
 * @note	You can enable one option at once (ie, WWDT_WDMOD_WDRESET |
 * WWDT_WDMOD_WDPROTECT), but use the WWDT_WDMOD_WDEN after all other options
 * are set (or unset) with no other options. If WWDT_WDMOD_LOCK is used, it cannot
 * be unset.
 */
STATIC INLINE void WATCHDOG_vSetOption(U32 unOption)
{
	SET_REG_FIELD(LPC_WWDT->MOD, (WWDT_WDMOD_BITMASK&unOption), unOption);
}

#if(ENABLE == WATCHDOG_WRN_MODE)
/**
 * @brief	Clear a watchdog timer option as zero
 * @param	unOption	: An option
 *					  WWDT_WDMOD_WDEN, WWDT_WDMOD_WDRESET, and WWDT_WDMOD_WDPROTECT
 * @return	None
 * @note	You can enable one option at once (ie, WWDT_WDMOD_WDRESET |
 * WWDT_WDMOD_WDPROTECT), but use the WWDT_WDMOD_WDEN after all other options
 * are set (or unset) with no other options. If WWDT_WDMOD_LOCK is used, it cannot
 * be unset.
 */
STATIC INLINE void WATCHDOG_vClearOption(U32 unOption)
{
	SET_REG_FIELD(LPC_WWDT->MOD, (WWDT_WDMOD_BITMASK&unOption), 0);
}

/**
 * @brief	Get a watchdog timer option
 * @param	unOption	: An option
 *					      WWDT_WDMOD_WDEN, WWDT_WDMOD_WDRESET, and WWDT_WDMOD_WDPROTECT
 * @return	None
 * @note	You can get one option at once (ie, WWDT_WDMOD_WDRESET |
 * WWDT_WDMOD_WDPROTECT), but use the WWDT_WDMOD_WDEN after all other options
 * are set (or unset) with no other options. If WWDT_WDMOD_LOCK is used, it cannot
 * be unset.
 */
STATIC INLINE bool WATCHDOG_bGetOption(U32 unOption)
{
	return (GET_REG_FIELD(LPC_WWDT->MOD, (WWDT_WDMOD_BITMASK&unOption)) != 0);
}

/**
 * @brief	Read WWDT status flag 
 * @param	unStatus	: Or'ed status flags of WWDT_WDMOD_WDTOF or WWDT_WDMOD_WDINT
 * @return	Watchdog status, an Or'ed value of WWDT_WDMOD_*
 */
STATIC INLINE bool WATCHDOG_bGetStatus(U32 unStatus)
{
    U32 sta = 0;
	 
	if (unStatus & WWDT_WDMOD_WDTOF) {
		sta |= WATCHDOG_bGetOption(WWDT_WDMOD_WDTOF);
	}
	
	if (unStatus & WWDT_WDMOD_WDINT) {
		sta |= WATCHDOG_bGetOption(WWDT_WDMOD_WDINT);
	}

	return (sta != 0);
}

/**
 * @brief	Clear WWDT interrupt status flags
 * @param	pWWDT	: The base of WatchDog Timer peripheral on the chip
 * @param	unStatus	: Or'ed value of status flag(s) that you want to clear, should be:
 *              - WWDT_WDMOD_WDTOF: Clear watchdog timeout flag
 *              - WWDT_WDMOD_WDINT: Clear watchdog warning flag
 * @return	None
 */
STATIC INLINE void WATCHDOG_vClearStatus(U32 unStatus)
{
	if (unStatus & WWDT_WDMOD_WDTOF) {
		WATCHDOG_vClearOption(WWDT_WDMOD_WDTOF);
	}
	
	if (unStatus & WWDT_WDMOD_WDINT) {
		WATCHDOG_vClearOption(WWDT_WDMOD_WDINT);
	}
}
#endif

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize the Watchdog timer */
void WATCHDOG_vInit(void)
{
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_WDT);
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_WDTOSC_PD);

    /*Set window value - 
      the maximum possible WDTV value, so windowing is not in effect */
	WATCHDOG_vSetWindow(WWDT_WINDOW_MASK);
	
    /*Set reload timeout value */
	WATCHDOG_vSetTimeOut(WATCHDOG_TIMEOUT_MS);	

	#if(DISABLE == WATCHDOG_WRN_MODE)
    /* Enable watch dog timeout chip reset */
	WATCHDOG_vSetOption(WWDT_WDMOD_WDRESET);
	#else
    /*Set warning timeout value */
	WATCHDOG_vSetWarning(WATCHDOG_WRN_TIME_MS);

    /* Clear status */
	WATCHDOG_vClearStatus(WWDT_WDMOD_WDINT|WWDT_WDMOD_WDTOF);

    /* Disable watch dog timeout chip reset */
	WATCHDOG_vClearOption(WWDT_WDMOD_WDRESET);
	#endif
}

/* Start Watchdog */
void WATCHDOG_vStart(void)
{
	WATCHDOG_vSetOption(WWDT_WDMOD_WDEN);
	
	WATCHDOG_vRefresh();
}
#endif
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/**
 *   Watchdog Warning ISR
 */
void WATCHDOG_WRN_vISR(void)
{ 
	#if (ENABLE == CFG_WATCHDOG_ENABLE)
	#if (ENABLE == WATCHDOG_WRN_MODE)
    if(WATCHDOG_bGetStatus(WWDT_WDMOD_WDINT|WWDT_WDMOD_WDTOF)){
		WATCHDOG_vClearStatus(WWDT_WDMOD_WDINT|WWDT_WDMOD_WDTOF);
         
		/* Callback body */
		#ifdef WATCHDOG_WRN_CB
        WATCHDOG_WRN_CB();
        #endif
    }	
	#endif
	#endif
}
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


