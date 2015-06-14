/******************************************************************************/
/*                                                                            */
/*    $Workfile::   platform_cfg.h                                           $    */
/*                                                                            */
/*    $Revision:: 220                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 07 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-16 16:55:17 +0800 (周四, 16 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Global configuration of paltform.                         */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef PLATFORM_CFG_H
#define PLATFORM_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Suppress PC-Lint Info 755: global macro 'Symbol' (Location) not referenced
** This is a global module providing functionality that can optionally
** be used
*/

/* TARGET_PROCESSOR defines the processor used in the framework
** The list of supported processors can be found in platform.h
*/
/* LPC15xx chip familiy is suppored */
#define CHIP_LPC15XX

/* target processor in family */
#define TARGET_PROCESSOR    LPC1517

/* Target package information */
#define TARGET_ICPACKAGE    LQFP64

/* TARGET_BYTE_ORDER defines the processor endianness
** possible settings are __LITTLE_ENDIAN or __BIG_ENDIAN
** note that the endianness of the ARM7 is configurable, however
** little endian mode is commonly used.
*/
#define TARGET_BYTE_ORDER   LITTLE_ENDIAN

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
