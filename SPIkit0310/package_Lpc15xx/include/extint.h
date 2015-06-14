/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ExtInt.h                                                  $ */
/*                                                                            */
/*    $Revision:: 320                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-14 15:30:51 +0800 (周五, 14 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - Pin Interrupt   */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef EXTINT_H
#define EXTINT_H

#include "platform.h"
#include "extint_cfg.h"

#include "system.h"

/** @defgroup PININT_15XX CHIP: LPC15xx PIN Interrupt Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CFG_EXTINT_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* PIN EXT number enum */
typedef enum {
     PININTCH0       =  0,
     PININTCH1,  
     PININTCH2,
     PININTCH3,
     PININTCH4,
     PININTCH5,
     PININTCH6,
     PININTCH7,
}PININT_CH_T;

/**
 * @brief LPC15xx Pin Interrupt and Pattern Match register block structure
 */
typedef struct {		/*!< PIN_INT Structure */
	__IO U32 ISEL;		/*!< Pin Interrupt Mode register */
	__IO U32 IENR;		/*!< Pin Interrupt Enable (Rising) register */
	__IO U32 SIENR;	    /*!< Set Pin Interrupt Enable (Rising) register */
	__IO U32 CIENR;   	/*!< Clear Pin Interrupt Enable (Rising) register */
	__IO U32 IENF;		/*!< Pin Interrupt Enable Falling Edge / Active Level register */
	__IO U32 SIENF; 	/*!< Set Pin Interrupt Enable Falling Edge / Active Level register */
	__IO U32 CIENF; 	/*!< Clear Pin Interrupt Enable Falling Edge / Active Level address */
	__IO U32 RISE;		/*!< Pin Interrupt Rising Edge register */
	__IO U32 FALL;		/*!< Pin Interrupt Falling Edge register */
	__IO U32 IST;		/*!< Pin Interrupt Status register */
} LPC_PIN_INT_T;

/* PIN Interrupt mode */
typedef enum {
     PININT_MODE_EDGE           =  0,  
     PININT_MODE_LEVEL          =  1
}PININT_MODE_T;

/* EXT INT PIN state */
typedef enum {
     PININT_STATE_HIGH          =  0, 
	 PININT_STATE_LOW           =  1,         
     PININT_STATE_RISING        =  2,
     PININT_STATE_FALLING       =  3,  
}PININT_STATE_T;

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * LPC15xx Pin Interrupt channel values
 */
#define PININTCH_MASK     (0xFF)
#define PININTCH(ch)      (1 << (ch))

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize Pin interrupt block
 * @param	Nothing
 * @return	Nothing
 * @note	This function should be used after the Chip_GPIO_Init() function.
 */
void EXTINT_vInit(void);

/**
 * @brief	Shutdown Pin interrupt block
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vShutdown(void)
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_PININT);
}

#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
 
#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


