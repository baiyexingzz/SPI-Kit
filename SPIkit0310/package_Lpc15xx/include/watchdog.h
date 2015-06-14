/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Watchdog.h                                                  $ */
/*                                                                            */
/*    $Revision:: 296                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 12:53:00 +0800 (周三, 05 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - Watchdog        */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "platform.h"

#include "system.h"
#include "watchdog_cfg.h"

#if(ENABLE == CFG_WATCHDOG_ENABLE)
/** @defgroup DMA_15XX CHIP: LPC15xx DMA Controller driver
 *  @ingroup 15XX_Drivers
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/**
 * @brief Watchdog reload keys
 */
#define WWDT_RELOAD_KEY1                (0xAA)
#define WWDT_RELOAD_KEY2                (0x55)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * @brief Watchdog range of feed timeout
 */
#define WWDT_TICKS_MIN                  (0xFF) 
#define WWDT_TICKS_MAX                  (0xFFFFFF)
#define WWDT_TIMEOUT_MS_MIN             (2UL)           /*ms (1000*WWDT_TICKS_MIN*4/SYSCTL_WDTOSC_FREQ_HZ) */
#define WWDT_TIMEOUT_MS_MAX             (133417UL)      /*ms (1000*WWDT_TICKS_MAX*4/SYSCTL_WDTOSC_FREQ_HZ) */
#define WWDT_TICKS_VAL(ms)              ((U32)((U64)ms*(U64)SYSCTL_WDTOSC_FREQ_HZ)/(U64)(4*1000))

/**
 * @brief Watchdog wrn time mask
 */
#define WWDT_WRN_TIME_MASK              (0x3FF) 
#define WWDT_WRN_TIME_MAX               (8)            /*ms (1000*WWDT_WRN_TIME_MASK*4/SYSCTL_WDTOSC_FREQ_HZ) */
#define WWDT_WRN_TICKS_VAL(ms)          ((U32)((U64)ms*(U64)SYSCTL_WDTOSC_FREQ_HZ)/(U64)(4*1000))

/**
 * @brief Watchdog Window
 */
#define WWDT_WINDOW_MASK                (0xFFFFFF) 

/**
 * @brief Watchdog Mode register definitions
 */
/** Watchdog Mode Bitmask */
#define WWDT_WDMOD_BITMASK              ((U32) 0x1F)
/** WWDT interrupt enable bit */
#define WWDT_WDMOD_WDEN                 ((U32) (1 << 0))
/** WWDT interrupt enable bit */
#define WWDT_WDMOD_WDRESET              ((U32) (1 << 1))
/** WWDT time out flag bit */
#define WWDT_WDMOD_WDTOF                ((U32) (1 << 2))
/** WDT Warning Window interrupt bit */
#define WWDT_WDMOD_WDINT                ((U32) (1 << 3))
/** WWDT Protect flag bit */
#define WWDT_WDMOD_WDPROTECT            ((U32) (1 << 4))
/** WWDT lock bit */
#define WWDT_WDMOD_LOCK                 ((U32) (1 << 5))

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief Windowed Watchdog register block structure
 */
typedef struct {		    /*!< WWDT Structure         */
	__IO U32  MOD;			/*!< Watchdog mode register. This register contains the basic mode and status of the Watchdog Timer. */
	__IO U32  TC;			/*!< Watchdog timer constant register. This register determines the time-out value. */
	__O  U32  FEED;		    /*!< Watchdog feed sequence register. Writing 0xAA followed by 0x55 to this register reloads the Watchdog timer with the value contained in WDTC. */
	__I  U32  TV;			/*!< Watchdog timer value register. This register reads out the current value of the Watchdog timer. */
	__IO U32  RESERVED;
	__IO U32  WARNINT;		/*!< Watchdog warning interrupt register. This register contains the Watchdog warning interrupt compare value. */
	__IO U32  WINDOW;		/*!< Watchdog timer window register. This register contains the Watchdog window value. */
} LPC_WWDT_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize the Watchdog timer
 * @param	Nothing
 * @return	None
 */
void WATCHDOG_vInit(void);

/**
 * @brief	Shutdown the Watchdog timer
 * @param	Nothing	
 * @return	None
 */
STATIC INLINE void WATCHDOG_vShutdown()
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_WDT);
}

/**
 * @brief	Feed watchdog timer
 * @param	Nothing
 * @return	None
 * @note	If this function isn't called, a watchdog timer warning will occur.
 * After the warning, a timeout will occur if a feed has happened.
 */
STATIC INLINE void WATCHDOG_vRefresh(void)
{
	WRITE_REG(LPC_WWDT->FEED, WWDT_RELOAD_KEY1);
    WRITE_REG(LPC_WWDT->FEED, WWDT_RELOAD_KEY2);
}

/**
 * @brief	Enable WWDT activity
 * @param	Nothing
 * @return	None
 */
void WATCHDOG_vStart(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

