/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Watchdog_cfg.h                                       $    */
/*                                                                            */
/*    $Revision:: 296                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 12:53:00 +0800 (周三, 05 十一月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of Watchdog                         */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef WATCHDOG_CFG_H
#define WATCHDOG_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Configure WDog is enabled as per application requirement */
#define CFG_WATCHDOG_ENABLE            ENABLE

#if(ENABLE == CFG_WATCHDOG_ENABLE)
/* Configure watchdog's timeout (Unit: ms Range: 2-133,417) */
#define WATCHDOG_TIMEOUT_MS            (3000UL)

/* If WATCHDOG_WRN_MODE is enabled, Watchdog works at wrn interrupt mode 
   Chip doesnot reset when watchdog is timeout */
#define WATCHDOG_WRN_MODE               ENABLE
/* Configure watchdog wrn time prior to watchdog reset timeout (Unit: ms Range: 0-8)*/
#define WATCHDOG_WRN_TIME_MS           (5UL)
/* Watchdog warning callback */
#undef  WATCHDOG_WRN_CB  
#endif

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


