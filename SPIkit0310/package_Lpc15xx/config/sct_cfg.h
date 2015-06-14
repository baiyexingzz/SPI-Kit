/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SCT_Cfg.h                                                 $ */
/*                                                                            */
/*    $Revision:: 282                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-03 14:50:32 +0800 (周一, 03 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of SCT/PWM                          */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SCT_CFG_H
#define SCT_CFG_H

#include "platform.h"

/** 
   Note:
   The SCT0 and SCT1 outputs #7 cannot be connected to external pins on the LQFP48 and
   LQFP64 packages.
   The SCT2 and SCT3 outputs #5 cannot be
   connected to external pins on the LQFP48 and LQFP64 packages. The SCT3 output #4
   cannot be connected to a pin on the LQFP48 package.
   They are all available for internal connections.
*/

/**
  * Green Ctrl Config
  */

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/*                            MACRO DEFINITIONS                            */
/******************************************************************************/
#define    CFG_SCT0_ENABLE          ENABLE
#define    CFG_SCT1_ENABLE          ENABLE 
#define    CFG_SCT2_ENABLE          ENABLE
#define    CFG_SCT3_ENABLE          ENABLE 

/* Configurations of SCT0 */
#if (ENABLE == CFG_SCT0_ENABLE)
#undef    SCT0_CB
#endif

/* Configurations of SCT1 */
#if (ENABLE == CFG_SCT1_ENABLE)
#undef    SCT1_CB
#endif

/* Configurations of SCT2 */
#if (ENABLE == CFG_SCT2_ENABLE)
#undef    SCT2_CB
#endif

/* Configurations of SCT3 */
#if (ENABLE == CFG_SCT3_ENABLE)
#undef    SCT3_CB
#endif



/* Macro def - SCT used */
#if ((ENABLE == CFG_SCT0_ENABLE)||\
	 (ENABLE == CFG_SCT1_ENABLE)||\
	 (ENABLE == CFG_SCT2_ENABLE)||\
	 (ENABLE == CFG_SCT3_ENABLE))
#define CFG_SCT_USED
#endif
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/                           

#endif /* SCT_CFG_H */

