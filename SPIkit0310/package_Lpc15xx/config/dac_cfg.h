/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DAC_Cfg.h                                                  $ */
/*                                                                            */
/*    $Revision:: 264                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-29 14:54:14 +0800 (周三, 29 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Configurations for DAC Controller                        */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef DAC_CFG_H
#define DAC_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define CFG_DAC_ENABLE   ENABLE

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#if (ENABLE == CFG_DAC_ENABLE)
/* DAC External Shutoff */
#define    DAC_EXT_SHUTOFF       DISABLE               

#define DAC_HW_TRIGSRC           DISABLE
#if (ENABLE == DAC_HW_TRIGSRC)  
	/**
	*   Config for DAC hardware trigger source
	*   ENABLE:  Internal trigger source
	*   DISABLE: External trigger source
	**/
    /* Enable: Internal timer/dma source; Disable: External source */
    #define DAC_INTERNAL_TRIGSRC ENABLE 	
    #if(ENABLE == DAC_INTERNAL_TRIGSRC)
		/* Internal Timer Reload Rate (Unit: Hz Range: <= System Clock)*/
	    #define    DAC_RELOAD_FREQ_HZ    (72000000UL)          /* Unit: HZ */
    #elif(DISABLE == DAC_INTERNAL_TRIGSRC) 
		/* External DAC trigger */
	    #define    DAC_EXTTRIG_POLARITY  DAC_POLARITY_FALLING  /* DAC_POLARITY_FALLING or DAC_POLARITY_RISING */
	    #define    DAC_SYNC_BYPASS       ENABLE                /* DAC external sync bypass */
    #endif
#endif

/**
    Callback for DAC if application needs
    If hardware trigger source is used, DAC interrupt is enabled also
    which is connected to NVIC and DMA slot
**/
#undef     DAC_CB                                      
#endif

/* Macro def - HW Internal trigger source used */
#if ((ENABLE == DAC_HW_TRIGSRC)&&(ENABLE == DAC_INTERNAL_TRIGSRC))
#define DAC_HW_INT_TRIGSRC_USED
#endif

/* Macro def - HW External trigger source used */
#if ((ENABLE == DAC_HW_TRIGSRC)&&(DISABLE == DAC_INTERNAL_TRIGSRC))
#define DAC_HW_EXT_TRIGSRC_USED
#endif

#endif/* CFG_DAC_ENABLE */
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

