/******************************************************************************/
/*                                                                            */
/*    $Workfile::   UART.c                                              $   */
/*                                                                            */
/*    $Revision:: 298                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-05 14:47:11 +0800 (周三, 05 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - UART            */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "system.h"
#include "uart.h"

#ifdef CFG_UART_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Declare macro for callback */
#define DECLARE_UART_CALLBACK(_Func_)  extern void _Func_(void);

#define UART_ERRINT_FLAGS  (UART_STAT_OVERRUNINT|UART_STAT_DELTARXBRK|\
 	                        UART_STAT_FRM_ERRINT|UART_STAT_PAR_ERRINT|\
 	                        UART_STAT_RXNOISEINT|UART_STAT_ABERRINT)
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
/* Declare UART callbacks if exist */
#ifdef UART0_RDRDY_CB
DECLARE_UART_CALLBACK(UART0_RDRDY_CB);
#endif

#ifdef UART0_TXRDY_CB
DECLARE_UART_CALLBACK(UART0_TXRDY_CB);
#endif

#ifdef UART1_RDRDY_CB
DECLARE_UART_CALLBACK(UART1_RDRDY_CB);
#endif

#ifdef UART1_TXRDY_CB
DECLARE_UART_CALLBACK(UART1_TXRDY_CB);
#endif

#ifdef UART2_RDRDY_CB
DECLARE_UART_CALLBACK(UART2_RDRDY_CB);
#endif

#ifdef UART2_TXRDY_CB
DECLARE_UART_CALLBACK(UART2_TXRDY_CB);
#endif
/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Enable the UART
 * @param	teCh		: UARTx peripheral
 * @param	bEnable		: true: enable UARTx ; false: disable UARTx ;
 * @return	Nothing
 */
STATIC INLINE void UART_vEnable(UART_NUM_T teCh, bool bEnable)
{
	U32 temp = ((bEnable)? UART_CFG_ENABLE : 0);	
	SET_REG_FIELD(LPC_UART[teCh]->CFG, UART_CFG_ENABLE, temp );
}

/**
 * @brief	Enable transmission on UART TxD pin
 * @param	teCh		: UARTx peripheral 
 * @param	bEnable		: true: enable UARTx Tx; false: disable UARTx Tx; 
 * @return Nothing
 */
STATIC INLINE void UART_vTXEnable(UART_NUM_T teCh, bool bEnable)
{
	U32 temp = ((bEnable)? 0 : UART_CTRL_TXDIS);
	SET_REG_FIELD(LPC_UART[teCh]->CTRL, 
		          UART_CTRL_TXDIS, 
		          temp);	
}

/**
 * @brief	Configure data width, parity and stop bits
 * @param	teCh		: UARTx peripheral 
 * @param	unConfig	: UART configuration, OR'ed values of select UART_CFG_* defines
 * @return	Nothing
 * @note	Select OR'ed config options for the UART from the UART_CFG_PARITY_*,
 *			UART_CFG_STOPLEN_*, and UART_CFG_DATALEN_* definitions. For example,
 *			a configuration of 8 data bits, 1 stop bit, and even (enabled) parity would be
 *			(UART_CFG_DATALEN_8 | UART_CFG_STOPLEN_1 | UART_CFG_PARITY_EVEN). Will not
 *			alter other bits in the CFG register.
 */
STATIC INLINE void UART_vConfigData(UART_NUM_T teCh, U32 unConfig)
{
	U32 mask = ((0x3 << 2) | (0x3 << 4) | (0x1 << 6));
	SET_REG_FIELD(LPC_UART[teCh]->CFG, 
		          mask, 
		          unConfig);
}

/**
 * @brief	Get the UART status register
 * @param	teCh		: UARTx peripheral 
 * @param	unStatMask	: OR'ed Interrupts status to get  
 * @return	UART status 
 * @note	Multiple statuses may be pending. Mask the return value
 *			with one or more UART_STAT_* definitions to determine
 *			statuses.
 */
STATIC INLINE U32 UART_nGetStatus(UART_NUM_T teCh, U32 unStatMask)
{
	return  (READ_REG(LPC_UART[teCh]->STAT) & unStatMask);
}

/**
 * @brief	Get the UART status register
 * @param	teCh		: UARTx peripheral 
 * @param	unStatMask	: OR'ed Interrupts status to get  
 * @return	UART status 
 * @note	Multiple statuses may be pending. Mask the return value
 *			with one or more UART_STAT_* definitions to determine
 *			statuses.
 */
STATIC INLINE bool UART_bGetStatus(UART_NUM_T teCh, U32 unStatMask)
{
	return  ((READ_REG(LPC_UART[teCh]->STAT) & unStatMask) !=  0);
}

/**
 * @brief	Clear the UART status register
 * @param	teCh		: UARTx peripheral 
 * @param	stsMask	: OR'ed statuses to disable
 * @return	Nothing
 * @note	Multiple interrupts may be pending. Mask the return value
 *			with one or more UART_INTEN_* definitions to determine
 *			pending interrupts.
 */
STATIC INLINE void UART_vClearStatus(UART_NUM_T teCh, U32 unMask)
{
	WRITE_REG(LPC_UART[teCh]->STAT, unMask);
}

/** 
 * @brief	Set baud rate for UART 
 * @param	teCh    : UARTx peripheral 
 * @param	unBaudrate	: Baud rate 
 * @return	Nothing
 * @note	Multiple interrupts may be pending. Mask the return value
 *			with one or more UART_INTEN_* definitions to determine
 *			pending interrupts.
 */
STATIC INLINE void UART_vSetBaud(UART_NUM_T teCh, U32 unBaudrate)
{
	U32 baudRateGenerator = CLOCK_unGetUARTBaseClock() / (16 * unBaudrate);
	baudRateGenerator = ((0 == baudRateGenerator)? 0 : (baudRateGenerator - 1));
	WRITE_REG(LPC_UART[teCh]->BRG, baudRateGenerator);/* baud rate */
}

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize the UART peripheral */
void UART_vInit(void)
{
    /* Set base clock for UART as main clock*/
	CLOCK_vSetUARTBaseClock(false);
	
    #if (ENABLE == CFG_UART0_ENABLE)
		/* Enable USART0 clock */
	    CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_UART0);
		/* Peripheral reset control to USART0 */
		SYSCTL_vPeriphReset(RESET_UART0);

		/* Tx Enable config */
	    #if(ENABLE == CFG_UART0_TXEN)
		UART_vTXEnable(UART0, true);
		#else
		UART_vTXEnable(UART0, false);
		#endif	

        /* Enanble UART0 */
		UART_vEnable(UART0, true);
    #endif

	#if (ENABLE == CFG_UART1_ENABLE)
		/* Enable USART1 clock */
	    CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_UART1);
		/* Peripheral reset control to USART1 */
		SYSCTL_vPeriphReset(RESET_UART1);

		/* Tx Enable config */
	    #if(ENABLE == CFG_UART1_TXEN)
		UART_vTXEnable(UART1, true);
		#else
		UART_vTXEnable(UART1, false);
		#endif	

        /* Enanble UART1 */
		UART_vEnable(UART1, true);
    #endif

	#if (ENABLE == CFG_UART2_ENABLE)		
		/* Enable USART2 clock */
	    CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_UART2);
		/* Peripheral reset control to USART2 */
		SYSCTL_vPeriphReset(RESET_UART2);

		/* Tx Enable config */
	    #if(ENABLE == CFG_UART2_TXEN)
		UART_vTXEnable(UART2, true);
		#else
		UART_vTXEnable(UART2, false);
		#endif	

        /* Enanble UART1 */
		UART_vEnable(UART2, true);
    #endif	
}

/* Shutdown the specific UART peripheral */
void UART_vShutdown(UART_NUM_T teCh)
{
	#if (ENABLE == CFG_UART0_ENABLE)
	/* Disable USART0 clock */
    CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_UART0);
    #endif
	
	#if (ENABLE == CFG_UART1_ENABLE)
	/* Disable USART1 clock */
    CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_UART1);
    #endif

	#if (ENABLE == CFG_UART2_ENABLE)		
	/* Disable USART2 clock */
    CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_UART2);
    #endif
}

/* Config the UART peripheral */
void UART_vConfig(UART_NUM_T teCh, 
                  U32 unDataLen,
                  U32 unDataParity,
                  U32 unDataStopBit,
                  U32 unBaud)
{
	/* PARITY, DataLen and StopBit */
    UART_vConfigData(teCh, unDataParity|unDataLen|unDataStopBit); 
    /* Baud Rate */
	UART_vSetBaud(teCh, unBaud);    
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
#endif

/******************************************************************************/
/*                      ISR IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for UART0/1/2
 *   Each UART has an independent NVIC slot
 **/
void UART0_vISR(void)
{
    #if (ENABLE == CFG_UART0_ENABLE)
    U32 tmperr = 0;
	
	if(UART_bGetStatus(UART0, UART_STAT_RXRDY)){	
        /* CallbackBody */
		#ifdef UART0_RDRDY_CB
		UART0_RDRDY_CB();
		#endif
	}

	if(UART_bGetStatus(UART0, UART_STAT_TXRDY)){		
        /* CallbackBody */
		#ifdef UART0_TXRDY_CB
		UART0_TXRDY_CB();
		#endif		
	}

    tmperr = UART_nGetStatus(UART0, UART_ERRINT_FLAGS);
	if(0 != tmperr){
        UART_vClearStatus(UART0, tmperr);
				
        /* CallbackBody */
	}
	#endif
}

void UART1_vISR(void)
{	
    #if (ENABLE == CFG_UART1_ENABLE)
    U32 tmperr = 0;	
	
	if(UART_bGetStatus(UART1, UART_STAT_RXRDY)){	
        /* CallbackBody */
		#ifdef UART1_RDRDY_CB
		UART1_RDRDY_CB();
		#endif
	}

	if(UART_bGetStatus(UART1, UART_STAT_TXRDY)){	
        /* CallbackBody */
		#ifdef UART1_TXRDY_CB
		UART1_TXRDY_CB();
		#endif			
	}	

    tmperr = UART_nGetStatus(UART1, UART_ERRINT_FLAGS);
	if(0 != tmperr){
        UART_vClearStatus(UART1, tmperr);
				
        /* CallbackBody */
	}
	#endif
}

void UART2_vISR(void)
{
    #if (ENABLE == CFG_UART2_ENABLE)
    U32 tmperr = 0;	
	
	if(UART_bGetStatus(UART2, UART_STAT_RXRDY)){	
        /* CallbackBody */
		#ifdef UART2_RDRDY_CB
		UART2_RDRDY_CB();
		#endif		
	}

	if(UART_bGetStatus(UART2, UART_STAT_TXRDY)){	
        /* CallbackBody */
		#ifdef UART2_TXRDY_CB
		UART2_TXRDY_CB();
		#endif			
	}	

    tmperr = UART_nGetStatus(UART2, UART_ERRINT_FLAGS);
	if(0 != tmperr){
        UART_vClearStatus(UART2, tmperr);
				
        /* CallbackBody */
	}
	#endif
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

