/******************************************************************************/
/*                                                                            */
/*    $Workfile::   TIMER_Cfg.h                                            $    */
/*                                                                            */
/*    $Revision:: 290                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-04 16:25:26 +0800 (周二, 04 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of TIMERs - MRT and RIT.            */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef TIMER_CFG_H
#define TIMER_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* TIMER present: MRT and RIT timers ; Sys Tick is always enabled */
#define  CFG_TIMER_MRT         ENABLE
#define  CFG_TIMER_RIT         DISABLE

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* TIMER0 - Sys Tick Timer - fixed 10ms */
/* Cannot be disabled as a fundamental timer of entire system */
#define  ALIAS_TIMER0          TIMER_0
#define  TIMER0_CB             SysTick_Handler

#if (ENABLE == CFG_TIMER_MRT)
/* TIMER1 Configurations - MRT CH0  (Unit: us Range: 0-233,016) */
#define  ALIAS_TIMER1          TIMER_1
#define  TIMER1_INTERVAL       (100000UL)                
#define  TIMER1_MODE           MRT_MODE_ONESHOT
//#undef   TIMER1_CB              
#define  TIMER1_CB

/* TIMER2 Configurations - MRT CH1 (Unit: us Range: 0-233,016) */
#define  ALIAS_TIMER2          TIMER_2
#define  TIMER2_INTERVAL       (200000UL)                  
#define  TIMER2_MODE           MRT_MODE_ONESHOT
#undef   TIMER2_CB              
//#define  TIMER2_CB

/* TIMER3 Configurations - MRT CH2 (Unit: us Range: 0-233,016) */
#define  QUICK_EVENT_TIMER           TIMER_3                                        // for very quick event e.g. 5us 50us,
#define  TIMER3_INTERVAL       (200000UL)                  
#define  TIMER3_MODE           MRT_MODE_ONESHOT
#undef   TIMER3_CB             

/* TIMER4 Configurations - MRT CH3 (Unit: us Range: 0-233,016) */
#define  ALIAS_TIMER4          TIMER_4
#define  TIMER4_INTERVAL       (15000UL)              
#define  TIMER4_MODE           MRT_MODE_ONESHOT
#undef   TIMER4_CB             
#endif

/* TIMER5 Configurations - (Unit: us Range: 0-15,270,994,830) */
#if (ENABLE == CFG_TIMER_RIT)
#define  ALIAS_TIMER5          TIMER_5
#define  TIMER5_INTERVAL       (1000000ULL)               
#undef   TIMER5_CB             
#endif

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

