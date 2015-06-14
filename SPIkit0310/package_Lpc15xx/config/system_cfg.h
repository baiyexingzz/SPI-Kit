/******************************************************************************/
/*                                                                            */
/*    $Workfile::   System_cfg.h                                                 $ */
/*                                                                            */
/*    $Revision:: 216                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-15 16:51:24 +0800 (周三, 15 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::  Configurations of system - SysCon, Clock and Pwr           */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SYSTEM_CFG_H
#define SYSTEM_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/** Main Clock Config (IRC as default clock resource) */
#define CFG_SYSTEM_PLL_MSEL          (5)                       /* IRC MSEL */
#define CFG_SYSTEM_PLL_PSEL          (2)                       /* IRC PSEL */
#define CFG_MAIN_CLOCK_FREQ_HZ       (72000000UL)              /* IRC 12000000*(5+1)*(2*2) = 288 FCCO MHz */
#define CFG_MAIN_CLOCK_FREQ_MHZ      (72UL)                    /* MHz unit */
 
/** System Clock Config (Main clock as default clock resource) */
#define CFG_SYS_CLOCK_DIV            (1)                       /* System Clock Divider */  
#define CFG_SYS_CLOCK_FREQ_HZ         CFG_MAIN_CLOCK_FREQ_HZ   /* System Clock (Hz unit) */
#define CFG_SYS_CLOCK_FREQ_MHZ        CFG_MAIN_CLOCK_FREQ_MHZ  /* System Clock (MHz unit) */

/* UART Base ClockRate */
#define CFG_UART_BASE_CLOCK_HZ        CFG_MAIN_CLOCK_FREQ_HZ  
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

