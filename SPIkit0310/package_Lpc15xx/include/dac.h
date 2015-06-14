/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DAC.h                                                   $ */
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
/*  Description::   Low-level macros, definition, functions - DAC             */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef DAC_H
#define DAC_H

#include "platform.h"
#include "system.h"
#include "dac_cfg.h"

/** @defgroup DAC_15XX CHIP: LPC15xx DAC Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#if (ENABLE == CFG_DAC_ENABLE)
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/** After this field is written with a
   new VALUE, the voltage on the AOUT pin (with respect to VSSA)
   is VALUE*((VREFP_DAC - VREFN)/4095) + VREFN */
#define DAC_VALUE(n)        ((U32) ((n & 0x0FFF) << 4))

/* Bit Definitions for DAC Control register */
#define DAC_INT_DMA_FLAG    (1 << 0)
#define DAC_TRIG_SRC_MASK   (0x7 << 1)
#define DAC_TRIG_SRC_BIT    (1 << 1)
#define DAC_POLARITY        (1 << 4)
#define DAC_SYNC_BYPASS     (1 << 5)
#define DAC_TIM_ENA_BIT     (1 << 6)
#define DAC_DBLBUF_ENA      (1 << 7)
#define DAC_SHUTOFF_ENA     (1 << 8)
#define DAC_SHUTOFF_FLAG    (1 << 9)
#define DAC_DACCTRL_MASK    ((U32) 0xFF << 1)
#define DAC_CTRL_UNUSED     ((U32) 0x7FFFF << 13)

#define DAC_POLARITY_FALLING (1 << 4)
#define DAC_POLARITY_RSING   (0 << 4)


/** Value to reload interrupt/DMA timer */
#define DAC_CNT_VALUE(n)    ((U32) ((n) & 0xffff))

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief DAC register block structure
 */
typedef struct {	    /*!< DAC Structure */
	__IO U32  VAL;		/*!< DAC register. Holds the conversion data */
	__IO U32  CTRL;	    /*!< DAC control register */
	__IO U32  CNTVAL;	/*!< DAC counter value register */
} LPC_DAC_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initial DAC configuration
 * @param	Nothing
 * @return	Nothing
 */
void DAC_vInit(void);

/**
 * @brief	Shutdown DAC
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void DAC_vShutdown(void)
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_DAC);
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_DAC_PD);
}

/**
 * @brief	Update value to DAC buffer
 * @param	unDacValue	: 12 bit input value for conversion
 * @return	Nothing
 */
STATIC INLINE void DAC_vUpdateValue(U32 unDacValue)
{
	WRITE_REG(LPC_DAC->VAL, DAC_VALUE(unDacValue));
}

/**
 * @brief	Get value from DAC buffer
 * @param	nDacValue	: 12 bit input value for conversion
 * @return	Nothing
 */
STATIC INLINE U32 DAC_nGetValue(void)
{
	return READ_REG(LPC_DAC->VAL);
}

#ifdef __cplusplus
}
#endif

#endif /* CFG_DAC_ENABLE */

/**
 * @}
 */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/



