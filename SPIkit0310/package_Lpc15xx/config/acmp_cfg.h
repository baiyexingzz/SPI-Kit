/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ACMP_Cfg.h                                                  $ */
/*                                                                            */
/*    $Revision:: 265                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-29 16:03:26 +0800 (周三, 29 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Configurations for ACMP Controller                        */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef ACMP_CFG_H
#define ACMP_CFG_H

/**
  * Descrptions for per ACMP configurations:
  * - ACMPx_NEG_INPUT: enum value of ACMP_NEG_INPUT_T 
  * - ACMPx_POS_INPUT: enum value of ACMP_POS_INPUT_T 
  * - ACMPx_HYS: enum value of ACMP_HYS_T 
  * - ACMPx_INT_POL : ACMP_INTPOL_NOTINVERTED or ACMP_INTPOL_INVERTED for Level Int  
  * - ACMPx_LADREF:   ACMP_LADREF_VDDCMP or ACMP_LADREF_VDDA if want to enable voltage ladder
  * - ACMP0_LADSEL:   0x00~0x1F ,which should be defined if ACMPx_LADREF defined  
  * - ACMPx_INT_TYPE: ACMP_INTTYPE_EDGE or ACMP_INTTYPE_LEVEL, which should be defined if ACMPx_CB defined
  * - ACMPx_INT_EDGE: ACMP_EDGESEL_FALLING or ACMP_EDGESEL_RISING for Edge Int, which should be defined if ACMPx_CB defined  
  *
  * The following settings are shared with all ACMP channels, which are defined as default in source file:
  * - Ring Oscillator Control Settings and Reset Source 
  * - Comparator Filter Settings
  * - Propagation Delay
  */

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Enable config for ACMP0~ACMP3 */
#define CFG_ACMP0_ENABLE        ENABLE
#define CFG_ACMP1_ENABLE        ENABLE
#define CFG_ACMP2_ENABLE        DISABLE
#define CFG_ACMP3_ENABLE        DISABLE

/**
  * Configurations for ACMP0 
  */
#if (ENABLE == CFG_ACMP0_ENABLE)
/*Alias name for ACMP0 */
#define GREEN1_HIGHLIMIT_CMP_INDEX         ACMP0

/* Setting up ACMP voltage settings */
#define ACMP0_NEG_INPUT                    ACMP_NEGIN_ACMP_I3     /* Analog Comparator negative input values */ 
#define ACMP0_POS_INPUT                    ACMP_POSIN_ACMP_I4     /* Analog Comparator positive input values */
#define ACMP0_HYS                          ACMP_HYS_10MV          /* Hysteresis selection for comparator */

/* Setting up ACMP voltage ladder */
#undef  ACMP0_LADREF                                              /* Voltage reference select - VDDCMP or VDDA */
#undef  ACMP0_LADSEL                                              /* Reference voltage selection - 0x00~0x1F */

/* Setting up ACMP interrupt settings */
#undef  ACMP0_INT_TYPE                                            /* Interrupt Type Edge or Level   */   
#undef  ACMP0_INT_EDGE                                            /* Edge selection for comparator - FAILING, RISING or BOTH */
/* Callback for ACMP ISR if application needs */
#undef  ACMP0_CB                                                  /* Callback for ACMP ISR */
#endif

/**
  * Configurations for ACMP1 
  */
#if (ENABLE == CFG_ACMP1_ENABLE)
/*Alias name for ACMP1 */
#define GREEN2_HIGHLIMIT_CMP_INDEX         ACMP1

/* Setting up ACMP voltage settings */
#define ACMP1_NEG_INPUT                    ACMP_NEGIN_ACMP_I3     /* Analog Comparator negative input values */ 
#define ACMP1_POS_INPUT                    ACMP_POSIN_ACMP_I4     /* Analog Comparator positive input values */
#define ACMP1_HYS                          ACMP_HYS_NONE          /* Hysteresis selection for comparator */

/* Setting up ACMP voltage ladder */
#undef  ACMP1_LADREF                                              /* Voltage reference select - VDDCMP or VDDA */
#undef  ACMP1_LADSEL                                              /* Reference voltage selection - 0x00~0x1F */

/* Setting up ACMP interrupt settings */
#undef  ACMP1_INT_TYPE                                            /* Interrupt Type Edge or Level   */   
#undef  ACMP1_INT_EDGE   
/* Edge selection for comparator - FAILING, RISING or BOTH */

/* Callback for ACMP ISR */
#undef  ACMP1_CB  
#endif

/* Macro def - ACMP used */
#if (ENABLE == CFG_ACMP0_ENABLE)||\
	(ENABLE == CFG_ACMP1_ENABLE)||\
	(ENABLE == CFG_ACMP2_ENABLE)||\
	(ENABLE == CFG_ACMP3_ENABLE)
#define CFG_ACMP_USED
#endif 

/* Macro def - ACMP INT used */
#if (ENABLE == CFG_ACMP0_ENABLE)
	#ifdef ACMP0_INT_TYPE
	    #if(ACMP_INTTYPE_EDGE == ACMP0_INT_TYPE)
			#ifndef CFG_ACMP_INTEDGE_USED
			#define CFG_ACMP_INTEDGE_USED
			#endif
		#elif (ACMP_INTTYPE_LEVEL == ACMP0_INT_TYPE)	
			#ifndef CFG_ACMP_INTLEVEL_USED
			#define CFG_ACMP_INTLEVEL_USED
			#endif
		#endif
    #endif
	#ifdef ACMP0_LADREF
		#ifndef CFG_ACMP_LADREF_USED
		#define CFG_ACMP_LADREF_USED
		#endif
	#endif
#endif	

#if (ENABLE == CFG_ACMP1_ENABLE)
	#ifdef ACMP1_INT_TYPE
	    #if(ACMP_INTTYPE_EDGE == ACMP1_INT_TYPE)
			#ifndef CFG_ACMP_INTEDGE_USED
			#define CFG_ACMP_INTEDGE_USED
			#endif
		#elif (ACMP_INTTYPE_LEVEL == ACMP1_INT_TYPE)	
			#ifndef CFG_ACMP_INTLEVEL_USED
			#define CFG_ACMP_INTLEVEL_USED
			#endif
		#endif
    #endif
	#ifdef ACMP1_LADREF
		#ifndef CFG_ACMP_LADREF_USED
		#define CFG_ACMP_LADREF_USED
		#endif
	#endif	
#endif	

#if (ENABLE == CFG_ACMP2_ENABLE)
	#ifdef ACMP2_INT_TYPE
	    #if(ACMP_INTTYPE_EDGE == ACMP2_INT_TYPE)
			#ifndef CFG_ACMP_INTEDGE_USED
			#define CFG_ACMP_INTEDGE_USED
			#endif
		#elif (ACMP_INTTYPE_LEVEL == ACMP2_INT_TYPE)	
			#ifndef CFG_ACMP_INTLEVEL_USED
			#define CFG_ACMP_INTLEVEL_USED
			#endif
		#endif
    #endif
	#ifdef ACMP2_LADREF
		#ifndef CFG_ACMP_LADREF_USED
		#define CFG_ACMP_LADREF_USED
		#endif
	#endif	
#endif	

#if (ENABLE == CFG_ACMP3_ENABLE)
	#ifdef ACMP3_INT_TYPE
	    #if(ACMP_INTTYPE_EDGE == ACMP3_INT_TYPE)
			#ifndef CFG_ACMP_INTEDGE_USED
			#define CFG_ACMP_INTEDGE_USED
			#endif
		#elif (ACMP_INTTYPE_LEVEL == ACMP3_INT_TYPE)	
			#ifndef CFG_ACMP_INTLEVEL_USED
			#define CFG_ACMP_INTLEVEL_USED
			#endif
		#endif
    #endif
	#ifdef ACMP3_LADREF
		#ifndef CFG_ACMP_LADREF_USED
		#define CFG_ACMP_LADREF_USED
		#endif
	#endif	
#endif	

#ifdef CFG_ACMP_INTLEVEL_USED
	#ifndef CFG_ACMP_INT_USED
	#define CFG_ACMP_INT_USED
	#endif
#endif

#ifdef CFG_ACMP_INTEDGE_USED
	#ifndef CFG_ACMP_INT_USED
	#define CFG_ACMP_INT_USED
	#endif
#endif

#endif
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

