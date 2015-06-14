/******************************************************************************/
/*                                                                            */
/*    $Workfile::   TIMER.h                                                 $ */
/*                                                                            */
/*    $Revision:: 217                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-16 12:53:31 +0800 (周四, 16 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level definciton ,macros and functions - MRT and RIT  */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#include "platform.h"
#include "system.h"
#include "timer_cfg.h"

/** @defgroup TIMER_15XX CHIP: MRT and RIT Controller
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
/** @defgroup MRT_15XX CHIP: LPC15xx Multi-Rate Timer driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */
#if (ENABLE == CFG_TIMER_MRT)
/**
 * @brief LPC15xx MRT chip configuration
 */
#define MRT_CHANNELS_NUM      (4)
#define MRT_NO_IDLE_CHANNEL   (0x40)

/**
 * @brief MRT register bit fields & masks
 */
/* MRT Time interval register bit fields */
#define MRT_INTVAL_IVALUE        (0x00FFFFFF)	/* Maximum interval load value and mask */
#define MRT_INTVAL_LOAD          (0x80000000UL)	/* Force immediate load of timer interval register bit */

/* MRT Control register bit fields & masks */
#define MRT_CTRL_INTEN_MASK      (0x01)
#define MRT_CTRL_MODE_MASK       (0x06)

/* MRT Status register bit fields & masks */
#define MRT_STAT_INTFLAG         (0x01)
#define MRT_STAT_RUNNING         (0x02)

/* Pointer to individual MR register blocks */
#define LPC_MRT_CH0         ((LPC_MRT_CH_T *) &LPC_MRT->CHANNEL[0])
#define LPC_MRT_CH1         ((LPC_MRT_CH_T *) &LPC_MRT->CHANNEL[1])
#define LPC_MRT_CH2         ((LPC_MRT_CH_T *) &LPC_MRT->CHANNEL[2])
#define LPC_MRT_CH3         ((LPC_MRT_CH_T *) &LPC_MRT->CHANNEL[3])
#define LPC_MRT_CH(ch)      ((LPC_MRT_CH_T *) &LPC_MRT->CHANNEL[(ch)])

/* Global interrupt flag register interrupt mask/clear values */
#define MRT0_INTFLAG        (1)
#define MRT1_INTFLAG        (2)
#define MRT2_INTFLAG        (4)
#define MRT3_INTFLAG        (8)
#define MRTn_INTFLAG(ch)    (1 << (ch))
#define MRT_INTFLAG_MASK    (0xF)
#endif

/** @defgroup RIT_15XX CHIP: LPC15xx Repetitive Interrupt Timer driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */
#if (ENABLE == CFG_TIMER_RIT)
/*
 * RIT control register
 */
/**	Set by H/W when the counter value equals the masked compare value */
#define RIT_CTRL_INT    (1 << 0)
/** Set timer enable clear to 0 when the counter value equals the masked compare value  */
#define RIT_CTRL_ENCLR  (1 << 1)
/** Set timer enable on debug */
#define RIT_CTRL_ENBR   (1 << 2)
/** Set timer enable */
#define RIT_CTRL_TEN    (1 << 3)

#define RIT_CTRL_MASK   (0xF)
#endif

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief LPC15xx Timer logic number logic definitons
 */
typedef enum {
	 SYSTICK_TIMER     =     0,
	 TIMER_0           =     SYSTICK_TIMER,
     #if (ENABLE == CFG_TIMER_MRT)
     MRT_CHANNEL_0     =     1,
     TIMER_1           =     MRT_CHANNEL_0, 
     MRT_CHANNEL_1     =     2,
     TIMER_2           =     MRT_CHANNEL_1,      
     MRT_CHANNEL_2     =     3,
     TIMER_3           =     MRT_CHANNEL_2,      
     MRT_CHANNEL_3     =     4,
     TIMER_4           =     MRT_CHANNEL_3,      
     #endif
	 #if (ENABLE == CFG_TIMER_RIT)
     RIT_TIMER         =     5,
     TIMER_5           =     RIT_TIMER,      
     #endif
	 TIMER_INVALID,
}TIMER_CH_T;

/**
 * @brief TIMER Modes enum
 */
typedef enum {
	MODE_REPEAT        =    0,	
	MODE_ONESHOT       =    1,		
} TIMER_MODE_T;

#if (ENABLE == CFG_TIMER_MRT)
/**
 * @brief MRT register block structure
 */
typedef struct {
	__IO U32 INTVAL;	/*!< Offset: 0x00 + n* 0x10, Timer interval register */
	__O  U32 TIMER;	    /*!< Offset: 0x04 + n* 0x10, Timer register */
	__IO U32 CTRL;		/*!< Offset: 0x08 + n* 0x10, Timer control register */
	__IO U32 STAT;		/*!< Offset: 0x0C + n* 0x10, Timer status register */
} LPC_MRT_CH_T;

/**
 * @brief MRT register block structure
 */
typedef struct {
	LPC_MRT_CH_T CHANNEL[MRT_CHANNELS_NUM];
	U32 unused[45];
	__O  U32 IDLE_CH;  /*!< Offset: 0xF4, Timer Idle register */
	__IO U32 IRQ_FLAG; /*!< Offset: 0xF8, Timer IRQ Flag register */
} LPC_MRT_T;

/**
 * @brief MRT Interrupt Modes enum
 */
typedef enum MRT_MODE {
	MRT_MODE_REPEAT  = (0 << 1),	/*!< MRT Repeat interrupt mode */
	MRT_MODE_ONESHOT = (1 << 1)		/*!< MRT One-shot interrupt mode */
} MRT_MODE_T;
#endif

#if (ENABLE == CFG_TIMER_RIT)
/**
 * @brief Repetitive Interrupt Timer register block structure
 */
typedef struct {	        /*!< RITIMER Structure      */
	__IO U32  COMPVAL;		/*!< Compare register       */
	__IO U32  MASK;		    /*!< Mask register. This register holds the 32-bit mask value. A 1 written to any bit will force a compare on the corresponding bit of the counter and compare register. */
	__IO U32  CTRL;		    /*!< Control register       */
	__IO U32  COUNTER;		/*!< 32-bit counter         */
	__IO U32  COMPVAL_H;	/*!< Compare upper register */
	__IO U32  MASK_H;		/*!< Mask upper register    */
	__I  U32  RESERVED0[1];
	__IO U32  COUNTER_H;	/*!< Counter upper register */
} LPC_RITIMER_T;
#endif
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize all timer resource of LPC1517 - MRT and RIT
 * @param	Nothing
 * @return	Nothing
 */
void TIMER_vInit(void);

/**
 * @brief	Set timer with interval and mode
 * @param	teCH: channel to set
 * @param	unUs: interval - us unit 
 * @param	tMode: Only for Timer1-4 (MODE_REPEAT/MODE_ONESHOT)
 *                 Timer0 or Timer5 just supports MODE_REPEAT
 * @return	Nothing
 */
void TIMER_vStartEx(TIMER_CH_T tCH, U64 unUs, TIMER_MODE_T tMode);

/**
 * @brief	Start a timer
 * @param	teCH: channel to start
 * @return	Nothing
 */
void TIMER_vStart(TIMER_CH_T teCH);

/**
 * @brief	Stop a timer
 * @param	teCH: channel to stop
 * @return	Nothing
 */
void TIMER_vStop(TIMER_CH_T teCH);

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

