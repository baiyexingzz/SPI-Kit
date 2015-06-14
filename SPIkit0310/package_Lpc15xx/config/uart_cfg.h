/******************************************************************************/
/*                                                                            */
/*    $Workfile::   UART_cfg.h                                                 $ */
/*                                                                            */
/*    $Revision:: 305                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-06 16:54:02 +0800 (周四, 06 十一月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::  Configurations of UART                                     */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef UART_CFG_H
#define UART_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Enable config for ACMP0~ACMP2 - will be moved to UART cfg */
#define CFG_UART0_ENABLE        ENABLE
#define CFG_UART1_ENABLE        DISABLE
#define CFG_UART2_ENABLE        DISABLE

#if (ENABLE == CFG_UART0_ENABLE)
#define CFG_UART0_TXEN          ENABLE

#undef UART0_RDRDY_CB          //DEBUG_Re_IRQHandler
#undef UART0_TXRDY_CB          //DEBUG_Tx_IRQHandler
#endif

#if (ENABLE == CFG_UART1_ENABLE)
#define CFG_UART1_TXEN          DISABLE

#undef UART1_RDRDY_CB  
#undef UART1_TXRDY_CB  
#endif

#if (ENABLE == CFG_UART2_ENABLE)
#define CFG_UART2_TXEN          DISABLE

#undef UART2_RDRDY_CB          
#undef UART2_TXRDY_CB          
#endif

/* Macro def - ACMP used */
#if (ENABLE == CFG_UART0_ENABLE)||\
	(ENABLE == CFG_UART1_ENABLE)||\
	(ENABLE == CFG_UART2_ENABLE)
#define CFG_UART_USED
#endif

/* Macro def - UART Tx used */
#if (ENABLE == CFG_UART0_ENABLE)
	#if (ENABLE == CFG_UART0_TXEN)
		#ifndef CFG_UARTTX_USED
		#define CFG_UARTTX_USED
		#endif
	#endif
#endif

#if (ENABLE == CFG_UART1_ENABLE)
	#if (ENABLE == CFG_UART1_TXEN)
		#ifndef CFG_UARTTX_USED
		#define CFG_UARTTX_USED
		#endif
	#endif
#endif

#if (ENABLE == CFG_UART2_ENABLE)
	#if (ENABLE == CFG_UART2_TXEN)
		#ifndef CFG_UARTTX_USED
		#define CFG_UARTTX_USED
		#endif
	#endif
#endif

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

