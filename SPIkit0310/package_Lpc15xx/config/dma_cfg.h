/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DMA_CFG.h                                                  $ */
/*                                                                            */
/*    $Revision:: 294                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 12:20:36 +0800 (周三, 05 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Configurations - DMA Controller                           */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef DMA_CFG_H
#define DMA_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 *  DMA channels setup table
 *  CH0-CH13:  Trigger must be defined, if DMA_CFG_HWTRIGEN is set
 *  CH14-CH17: DMA_CFG_HWTRIGEN must be set 
 *  Also need update Macro MAX_DMA_USED_CHANNELS according to following setup table
 */
#define CFG_DMA0_ENABLE                     DISABLE 
#define CFG_DMA1_ENABLE                     DISABLE 
#define CFG_DMA2_ENABLE                     DISABLE 
#define CFG_DMA3_ENABLE                     DISABLE 
#define CFG_DMA4_ENABLE                     DISABLE 
#define CFG_DMA5_ENABLE                     DISABLE 
#define CFG_DMA6_ENABLE                     DISABLE 
#define CFG_DMA7_ENABLE                     DISABLE 
#define CFG_DMA8_ENABLE                     DISABLE 
#define CFG_DMA9_ENABLE                     DISABLE 
#define CFG_DMA10_ENABLE                    DISABLE 
#define CFG_DMA11_ENABLE                    DISABLE 
#define CFG_DMA12_ENABLE                    DISABLE 
#define CFG_DMA13_ENABLE                    DISABLE 
#define CFG_DMA14_ENABLE                    DISABLE 
#define CFG_DMA15_ENABLE                    DISABLE
#define CFG_DMA16_ENABLE                    DISABLE 
#define CFG_DMA17_ENABLE                    DISABLE

/** Descriptions of per ADC channel's configurations:    
    DMA0_TRIG: Enum value of DMA_TRIGSRC_T
    DMAx_CFG: An Or'ed value of the following:
                   DMA_CFG_HWTRIGEN 
                   DMA_CFG_TRIGTYPE_EDGE DMA_CFG_TRIGPOL_RISING or DMA_CFG_TRIGPOL_FALLING
                   DMA_CFG_TRIGTYPE_LEVEL DMA_CFG_TRIGPOL_HIGH or DMA_CFG_TRIGPOL_LOW
                   DMA_CFG_BURSTPOWER_x
                   DMA_CFG_CHPRIORITY(DMA_CHPRIORITY_HIGHEST)
    DMAx_XFERCFG: An Or'ed value of the following:
                   DMA_XFERCFG_CFGVALID 
                   DMA_XFERCFG_RELOAD 
                   DMA_XFERCFG_SETINTA
                   DMA_XFERCFG_CLRTRIG
                   DMA_XFERCFG_WIDTH_x
                   DMA_XFERCFG_SRCINC_x
                   DMA_XFERCFG_DSTINC_x
    DMA_XFERCFG_RELOAD and DMA_XFERCFG_CFGVALID should be set(Or) otherwise individual controls are needed
*/

/* *************************************************
  * DMA_CH0 -> ADC0 SeqA 
 **************************************************/
#if(ENABLE == CFG_DMA0_ENABLE)
#define MEASURE_ADC0_DMACH          DMA0   /*Alias name*/
/* Channel trigger */
#define DMA0_TRIG                   DMATRIG_ADC1_SEQA_IRQ                   
/* Channel config */	                                      
#define DMA0_CFG                    (DMA_CFG_HWTRIGEN      |\
	                                 DMA_CFG_TRIGTYPE_EDGE |\
	                                 DMA_CFG_TRIGPOL_RISING|\
	                                 DMA_CFG_BURSTPOWER_4  |\
	                                 DMA_CFG_CHPRIORITY(DMA_CHPRIORITY_HIGHEST))
	                                      
/* Default channel transfer config, which is used as input of DMA_bStartChannel */	
#define DMA0_XFERCOUNT              (9)   
#define DMA0_XFERCFG                (DMA_XFERCFG_CFGVALID  |\
	                                 DMA_XFERCFG_RELOAD    |\
	                                 DMA_XFERCFG_SETINTA   |\
	                                 DMA_XFERCFG_CLRTRIG   |\
								     DMA_XFERCFG_WIDTH_16  |\
								     DMA_XFERCFG_SRCINC_2  |\
								     DMA_XFERCFG_DSTINC_1  |\
								     DMA_XFERCFG_XFERCOUNT(DMA0_XFERCOUNT))

/* DMA0 interrupt configs */
#define DMA0_INT_EN                 ENABLE         
#define DMA0_INTA_CB                Measure_DMA_IntAHandler
#undef DMA0_INTB_CB

/* DMA0 error interrupt configs */
#define DMA0_ERRINT_EN              ENABLE 
#define DMA0_INTERR_CB              Measure_DMA_IntAErrHandler
#endif

/***************************************************
  * DMA_CH1
  **************************************************/  

/***************************************************
  * DMA_CH2
  **************************************************/

/***************************************************
  * DMA_CH3
  **************************************************/

/***************************************************
  * DMA_CH4
  **************************************************/

/***************************************************
  * DMA_CH5
  **************************************************/

/***************************************************
  * DMA_CH6
  **************************************************/

/***************************************************
  * DMA_CH7
  **************************************************/

/***************************************************
  * DMA_CH8
  **************************************************/

/***************************************************
  * DMA_CH9
  **************************************************/

/***************************************************
  * DMA_CH10
  **************************************************/

/***************************************************
  * DMA_CH11
  **************************************************/

/***************************************************
  * DMA_CH12
  **************************************************/

/***************************************************
  * DMA_CH13
  **************************************************/

/***************************************************
  * DMA_CH14
  **************************************************/

/***************************************************
  * DMA_CH15
  **************************************************/

/***************************************************
  * DMA_CH16
  **************************************************/

/***************************************************
  * DMA_CH17
  **************************************************/

/* Macro def - DMA used */
#if (ENABLE == CFG_DMA0_ENABLE)||\
	(ENABLE == CFG_DMA1_ENABLE)||\
	(ENABLE == CFG_DMA2_ENABLE)||\
	(ENABLE == CFG_DMA3_ENABLE)||\
	(ENABLE == CFG_DMA4_ENABLE)||\
	(ENABLE == CFG_DMA5_ENABLE)||\
	(ENABLE == CFG_DMA6_ENABLE)||\
	(ENABLE == CFG_DMA7_ENABLE)||\
	(ENABLE == CFG_DMA8_ENABLE)||\
	(ENABLE == CFG_DMA9_ENABLE)||\
	(ENABLE == CFG_DMA10_ENABLE)||\
	(ENABLE == CFG_DMA11_ENABLE)||\
	(ENABLE == CFG_DMA12_ENABLE)||\
	(ENABLE == CFG_DMA13_ENABLE)||\
	(ENABLE == CFG_DMA14_ENABLE)||\
	(ENABLE == CFG_DMA15_ENABLE)||\
	(ENABLE == CFG_DMA16_ENABLE)||\
	(ENABLE == CFG_DMA17_ENABLE)
#define CFG_DMA_USED
#endif

/* Macro def - INTA, INTB and INTERR usage */
#if (ENABLE == CFG_DMA0_ENABLE)
	#if (ENABLE == DMA0_INT_EN)
	    #ifdef DMA0_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA0_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA0_ERRINT_EN)
	    #ifdef DMA0_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif	

#if (ENABLE == CFG_DMA1_ENABLE)
	#if (ENABLE == DMA1_INT_EN)
	    #ifdef DMA1_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA1_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA1_ERRINT_EN)
	    #ifdef DMA1_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA2_ENABLE)
	#if (ENABLE == DMA2_INT_EN)
	    #ifdef DMA2_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA2_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA2_ERRINT_EN)
	    #ifdef DMA2_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA3_ENABLE)
	#if (ENABLE == DMA3_INT_EN)
	    #ifdef DMA3_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA3_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA3_ERRINT_EN)
	    #ifdef DMA3_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA4_ENABLE)
	#if (ENABLE == DMA4_INT_EN)
	    #ifdef DMA4_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA4_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA4_ERRINT_EN)
	    #ifdef DMA4_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA5_ENABLE)
	#if (ENABLE == DMA5_INT_EN)
	    #ifdef DMA5_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA5_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA5_ERRINT_EN)
	    #ifdef DMA5_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA6_ENABLE)
	#if (ENABLE == DMA6_INT_EN)
	    #ifdef DMA6_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA6_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA6_ERRINT_EN)
	    #ifdef DMA6_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA7_ENABLE)
	#if (ENABLE == DMA7_INT_EN)
	    #ifdef DMA7_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA7_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA7_ERRINT_EN)
	    #ifdef DMA7_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA8_ENABLE)
	#if (ENABLE == DMA8_INT_EN)
	    #ifdef DMA8_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA8_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA8_ERRINT_EN)
	    #ifdef DMA8_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA9_ENABLE)
	#if (ENABLE == DMA9_INT_EN)
	    #ifdef DMA9_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA9_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA9_ERRINT_EN)
	    #ifdef DMA9_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA10_ENABLE)
	#if (ENABLE == DMA10_INT_EN)
	    #ifdef DMA10_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA10_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA10_ERRINT_EN)
	    #ifdef DMA10_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA12_ENABLE)
	#if (ENABLE == DMA12_INT_EN)
	    #ifdef DMA12_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA12_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA12_ERRINT_EN)
	    #ifdef DMA12_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA13_ENABLE)
	#if (ENABLE == DMA13_INT_EN)
	    #ifdef DMA13_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA13_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA13_ERRINT_EN)
	    #ifdef DMA13_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA14_ENABLE)
	#if (ENABLE == DMA14_INT_EN)
	    #ifdef DMA14_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA14_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA14_ERRINT_EN)
	    #ifdef DMA14_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA15_ENABLE)
	#if (ENABLE == DMA15_INT_EN)
	    #ifdef DMA15_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA15_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA15_ERRINT_EN)
	    #ifdef DMA15_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA16_ENABLE)
	#if (ENABLE == DMA16_INT_EN)
	    #ifdef DMA16_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA16_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA16_ERRINT_EN)
	    #ifdef DMA16_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#if (ENABLE == CFG_DMA17_ENABLE)
	#if (ENABLE == DMA17_INT_EN)
	    #ifdef DMA17_INTA_CB
			#ifndef CFG_INTA_USED
			#define CFG_INTA_USED
			#endif
		#endif
		#ifdef DMA17_INTB_CB
			#ifndef CFG_INTB_USED
			#define CFG_INTB_USED
			#endif
		#endif
    #endif
	#if (ENABLE == DMA17_ERRINT_EN)
	    #ifdef DMA17_INTERR_CB
			#ifndef CFG_INTERR_USED
			#define CFG_INTERR_USED
			#endif
		#endif
    #endif
#endif

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
