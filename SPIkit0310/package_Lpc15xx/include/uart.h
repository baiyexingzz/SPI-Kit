/******************************************************************************/
/*                                                                            */
/*    $Workfile::   UART.h                                                  $ */
/*                                                                            */
/*    $Revision:: 298                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 14:47:11 +0800 (周三, 05 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - UART            */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef UART_H
#define UART_H

#include "platform.h"

#include "uart_cfg.h"

#ifdef CFG_UART_USED
/** @defgroup INMUX_15XX CHIP: LPC15xx UART Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define UART_MAX_NUMBER         (3)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * @brief UART CFG register definitions
 */
#define UART_CFG_ENABLE         (0x01 << 0)
#define UART_CFG_DATALEN_7      (0x00 << 2)	    /*!< UART 7 bit length mode */
#define UART_CFG_DATALEN_8      (0x01 << 2)	    /*!< UART 8 bit length mode */
#define UART_CFG_DATALEN_9      (0x02 << 2)   	/*!< UART 9 bit length mode */
#define UART_CFG_PARITY_NONE    (0x00 << 4)	    /*!< No parity */
#define UART_CFG_PARITY_EVEN    (0x02 << 4)	    /*!< Even parity */
#define UART_CFG_PARITY_ODD     (0x03 << 4)	    /*!< Odd parity */
#define UART_CFG_STOPLEN_1      (0x00 << 6)	    /*!< UART One Stop Bit Select */
#define UART_CFG_STOPLEN_2      (0x01 << 6)	    /*!< UART Two Stop Bits Select */
#define UART_MODE_32K           (0x01 << 7)	    /*!< Selects the 32 kHz clock from the RTC oscillator as the clock source to the BRG */
#define UART_CFG_CTSEN          (0x01 << 9)	    /*!< CTS enable bit */
#define UART_CFG_SYNCEN         (0x01 << 11)	/*!< Synchronous mode enable bit */
#define UART_CFG_CLKPOL         (0x01 << 12)	/*!< Un_RXD rising edge sample enable bit */
#define UART_CFG_SYNCMST        (0x01 << 14)	/*!< Select master mode (synchronous mode) enable bit */
#define UART_CFG_LOOP           (0x01 << 15)	/*!< Loopback mode enable bit */

/**
 * @brief UART CTRL register definitions
 */
#define UART_CTRL_TXBRKEN       (0x01 << 1)		/*!< Continuous break enable bit */
#define UART_CTRL_ADDRDET       (0x01 << 2)		/*!< Address detect mode enable bit */
#define UART_CTRL_TXDIS         (0x01 << 6)		/*!< Transmit disable bit */
#define UART_CTRL_CC            (0x01 << 8)		/*!< Continuous Clock mode enable bit */
#define UART_CTRL_CLRCC         (0x01 << 9)		/*!< Clear Continuous Clock bit */

/**
 * @brief UART STAT register definitions
 */
#define UART_STAT_RXRDY         (0x01 << 0)			/*!< Receiver ready */
#define UART_STAT_RXIDLE        (0x01 << 1)			/*!< Receiver idle */
#define UART_STAT_TXRDY         (0x01 << 2)			/*!< Transmitter ready for data */
#define UART_STAT_TXIDLE        (0x01 << 3)			/*!< Transmitter idle */
#define UART_STAT_CTS           (0x01 << 4)			/*!< Status of CTS signal */
#define UART_STAT_DELTACTS      (0x01 << 5)			/*!< Change in CTS state */
#define UART_STAT_TXDISINT      (0x01 << 6)			/*!< Transmitter disabled */
#define UART_STAT_OVERRUNINT    (0x01 << 8)			/*!< Overrun Error interrupt flag. */
#define UART_STAT_RXBRK         (0x01 << 10)		/*!< Received break */
#define UART_STAT_DELTARXBRK    (0x01 << 11)		/*!< Change in receive break detection */
#define UART_STAT_START         (0x01 << 12)		/*!< Start detected */
#define UART_STAT_FRM_ERRINT    (0x01 << 13)		/*!< Framing Error interrupt flag */
#define UART_STAT_PAR_ERRINT    (0x01 << 14)		/*!< Parity Error interrupt flag */
#define UART_STAT_RXNOISEINT    (0x01 << 15)		/*!< Received Noise interrupt flag */
#define UART_STAT_ABERRINT      (0x01 << 16)		/*!< Auto baud err interrupt flag */

/**
 * @brief UART INTENSET/INTENCLR register definitions
 */
#define UART_INTEN_RXRDY        (0x01 << 0)			/*!< Receive Ready interrupt */
#define UART_INTEN_TXRDY        (0x01 << 2)			/*!< Transmit Ready interrupt */
#define UART_INTEN_DELTACTS     (0x01 << 5)			/*!< Change in CTS state interrupt */
#define UART_INTEN_TXDIS        (0x01 << 6)			/*!< Transmitter disable interrupt */
#define UART_INTEN_OVERRUN      (0x01 << 8)			/*!< Overrun error interrupt */
#define UART_INTEN_DELTARXBRK   (0x01 << 11)		/*!< Change in receiver break detection interrupt */
#define UART_INTEN_START        (0x01 << 12)		/*!< Start detect interrupt */
#define UART_INTEN_FRAMERR      (0x01 << 13)		/*!< Frame error interrupt */
#define UART_INTEN_PARITYERR    (0x01 << 14)		/*!< Parity error interrupt */
#define UART_INTEN_RXNOISE      (0x01 << 15)		/*!< Received noise interrupt */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief Enum definition of UART channel
 */ 
typedef enum {
  #if (ENABLE == CFG_UART0_ENABLE)
  UART0        =  0,
  #endif
  #if (ENABLE == CFG_UART1_ENABLE)  
  UART1        =  1,
  #endif
  #if (ENABLE == CFG_UART2_ENABLE)  
  UART2        =  2,
  #endif
} UART_NUM_T;

/**
 * @brief UART register block structure
 */
typedef struct {
	__IO U32  CFG;				/*!< Configuration register */
	__IO U32  CTRL;			    /*!< Control register */
	__IO U32  STAT;			    /*!< Status register */
	__IO U32  INTENSET;		    /*!< Interrupt Enable read and set register */
	__O  U32  INTENCLR;		    /*!< Interrupt Enable clear register */
	__I  U32  RXDATA;			/*!< Receive Data register */
	__I  U32  RXDATA_STAT;		/*!< Receive Data with status register */
	__IO U32  TXDATA;			/*!< Transmit data register */
	__IO U32  BRG;				/*!< Baud Rate Generator register */
	__IO U32  INTSTAT;			/*!< Interrupt status register */
} LPC_USART_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
static LPC_USART_T* const LPC_UART[UART_MAX_NUMBER] = {LPC_USART0, LPC_USART1, LPC_USART2};

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize the UART peripheral
 * @param	Nothing
 * @return	Nothing
 */
void UART_vInit(void);

/**
 * @brief	Shutdown the UART peripheral
 * @param	teCh		: UARTx peripheral 
 * @return	Nothing
 */
void UART_vShutdown(UART_NUM_T teCh);

/**
 * @brief	Config the UART peripheral
 * @param	teCh		: UARTx peripheral 
 * @param	unDataParity : UART_CFG_PRITY_*
 * @param	unDataLen : UART_CFG_DATALEN_*
 * @param	unDataLen : UART_CFG_STOPLEN_*
 * @return	Nothing
 */
void UART_vConfig(UART_NUM_T teCh, 
                  U32 unDataLen,
                  U32 unDataParity,
                  U32 unDataStopBit,
                  U32 unBaud);

/**
 * @brief	Enable UART interrupts
 * @param	teCh		: UARTx peripheral 
 * @param	unIntMask	: OR'ed Interrupts to enable
 * @return	Nothing
 * @note	Use an OR'ed value of UART_INTEN_* definitions with this function
 *			to enable specific UART interrupts.
 */
STATIC INLINE void UART_vIntEnable(UART_NUM_T teCh, U32 unIntMask)
{
	WRITE_REG(LPC_UART[teCh]->INTENSET, unIntMask);
}

/**
 * @brief	Disable UART interrupts
 * @param	teCh		: UARTx peripheral 
 * @param	unIntMask	: OR'ed Interrupts to disable
 * @return	Nothing
 * @note	Use an OR'ed value of UART_INTEN_* definitions with this function
 *			to disable specific UART interrupts.
 */
STATIC INLINE void UART_vIntDisable(UART_NUM_T teCh, U32 unIntMask)
{
	WRITE_REG(LPC_UART[teCh]->INTENCLR, unIntMask);
}

/**
 * @brief	Transmit a single data byte through the UART peripheral
 * @param	teCh    : UARTx peripheral 
 * @param	ubData	: Byte to transmit
 * @return	Nothing
 * @note	This function attempts to place a byte into the UART transmit
 *			holding register regard regardless of UART state.
 */
STATIC INLINE void UART_vSendByte(UART_NUM_T teCh, U8 ubData)
{
	WRITE_REG(LPC_UART[teCh]->TXDATA, (U32)ubData);
}

/**
 * @brief	Read a single byte data from the UART peripheral
 * @param	teCh		: UARTx peripheral 
 * @return	A single byte of data read
 * @note	This function reads a byte from the UART receive FIFO or
 *			receive hold register regard regardless of UART state. The
 *			FIFO status should be read first prior to using this function
 */
STATIC INLINE U8 UART_ubReadByte(UART_NUM_T teCh)
{
	return (U8)(READ_REG(LPC_UART[teCh]->RXDATA) & 0x000000FF);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/



