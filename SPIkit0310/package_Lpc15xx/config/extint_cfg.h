/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Extint_cfg.h                                       $    */
/*                                                                            */
/*    $Revision:: 260                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-29 13:25:42 +0800 (周三, 29 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of pin int controller               */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef EXTINT_CFG_H
#define EXTINT_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define CFG_PININTCH0_ENABLE         ENABLE
#define CFG_PININTCH1_ENABLE         ENABLE
#define CFG_PININTCH2_ENABLE         ENABLE
#define CFG_PININTCH3_ENABLE         ENABLE
#define CFG_PININTCH4_ENABLE         DISABLE
#define CFG_PININTCH5_ENABLE         DISABLE
#define CFG_PININTCH6_ENABLE         DISABLE
#define CFG_PININTCH7_ENABLE         DISABLE

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/*
*    External interrupt definitions
*    Example of Level Mode: 
*    Level-sensitive interrupt pins can be HIGH- or LOW-active
*    #define CFG_PININTCH0_MODE_LEVEL    ENABLE   
*    #define CFG_PININTCH0_LEVEL_HIGH    ENABLE 
*    Example of Edge Mode:    
*    Edge-sensitive interrupt pins can interrupt on rising or falling edges or both
*    #define CFG_PININTCH0_MODE_LEVEL    DISABLE   
*    #define CFG_PININTCH0_EDGE_RISING   ENABLE 
*    #define CFG_PININTCH0_EDGE_FALLING  ENABLE 
*/

#if(ENABLE == CFG_PININTCH0_ENABLE)
#define UI_STROBE_R_PININT          PININTCH0
#define CFG_PININTCH0_PIN_SEL       UI_STROBE_R_PIN       
#define CFG_PININTCH0_MODE_LEVEL    DISABLE   
#define CFG_PININTCH0_EDGE_RISING   ENABLE 
#define CFG_PININTCH0_EDGE_FALLING  ENABLE 

/* Callback for PININTCH0 */
#undef  PININTCH0_CB
#endif   

#if(ENABLE == CFG_PININTCH1_ENABLE)
#define UI_STROBE_G_PININT          PININTCH1
#define CFG_PININTCH1_PIN_SEL       UI_STROBE_G_PIN 
#define CFG_PININTCH1_MODE_LEVEL    DISABLE   
#define CFG_PININTCH1_EDGE_RISING   ENABLE 
#define CFG_PININTCH1_EDGE_FALLING  ENABLE 

/* Callback for PININTCH1 */
#undef  PININTCH1_CB
#endif        

#if(ENABLE == CFG_PININTCH2_ENABLE)
#define UI_STROBE_B_PININT          PININTCH2
#define CFG_PININTCH2_PIN_SEL       UI_STROBE_B_PIN 
#define CFG_PININTCH2_MODE_LEVEL    DISABLE   
#define CFG_PININTCH2_EDGE_RISING   ENABLE 
#define CFG_PININTCH2_EDGE_FALLING  ENABLE 

/* Callback for PININTCH2 */
#undef  PININTCH2_CB
#endif  

#if(ENABLE == CFG_PININTCH3_ENABLE)
#define UI_STROBE_S_PININT          PININTCH3
#define CFG_PININTCH3_PIN_SEL       UI_STROBE_S_PIN 
#define CFG_PININTCH3_MODE_LEVEL    DISABLE   
#define CFG_PININTCH3_EDGE_RISING   ENABLE 
#define CFG_PININTCH3_EDGE_FALLING  ENABLE

/* Callback for PININTCH3 */
#undef  PININTCH3_CB
#endif 

#if(ENABLE == CFG_PININTCH4_ENABLE)
#define UI_TI_RESTZ_PININT          PININTCH4
#define CFG_PININTCH4_PIN_SEL       UI_TI_RESTZ_PIN
#define CFG_PININTCH4_MODE_LEVEL    DISABLE   
#define CFG_PININTCH4_EDGE_RISING   ENABLE 
#define CFG_PININTCH4_EDGE_FALLING  ENABLE 

/* Callback for PININTCH4 */
#undef  PININTCH4_CB
#endif   

/* Macro def - EXTINT used */
#if (ENABLE == CFG_PININTCH0_ENABLE)||\
	(ENABLE == CFG_PININTCH1_ENABLE)||\
	(ENABLE == CFG_PININTCH2_ENABLE)||\
	(ENABLE == CFG_PININTCH3_ENABLE)||\
	(ENABLE == CFG_PININTCH4_ENABLE)||\
	(ENABLE == CFG_PININTCH5_ENABLE)||\
	(ENABLE == CFG_PININTCH6_ENABLE)||\
	(ENABLE == CFG_PININTCH7_ENABLE)
#define CFG_EXTINT_USED
#endif

/* Macro def - EXTINT TYPE used */
#if(ENABLE == CFG_PININTCH0_ENABLE)
    #if(ENABLE == CFG_PININTCH0_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH1_ENABLE)
    #if(ENABLE == CFG_PININTCH1_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH2_ENABLE)
    #if(ENABLE == CFG_PININTCH2_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH3_ENABLE)
    #if(ENABLE == CFG_PININTCH3_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH4_ENABLE)
    #if(ENABLE == CFG_PININTCH4_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH5_ENABLE)
    #if(ENABLE == CFG_PININTCH5_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH6_ENABLE)
    #if(ENABLE == CFG_PININTCH6_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif
#if(ENABLE == CFG_PININTCH7_ENABLE)
    #if(ENABLE == CFG_PININTCH7_MODE_LEVEL)
	    #ifndef CFG_EXTINT_LEVEL_USED
	    #define CFG_EXTINT_LEVEL_USED
	    #endif
	#else 
	    #ifndef CFG_EXTINT_EDGE_USED
	    #define CFG_EXTINT_EDGE_USED
	    #endif
	#endif
#endif

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


