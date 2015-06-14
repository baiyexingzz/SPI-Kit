/******************************************************************************/
/*                                                                            */
/*    $Workfile::   IO.h                                                 $    */
/*                                                                            */
/*    $Revision:: 360                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-26 15:47:50 +0800 (周三, 26 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level digital I/O configuration and pin assignment    */
/*                  macros and functions for LPC1517                          */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef IO_H
#define IO_H

#include "platform.h"
#include "system.h"
#include "io_cfg.h"

/** @defgroup IO_15XX CHIP: LPC15xx DIOCON and GPIO Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */
 
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
//DIO Port enum
typedef enum {
	#if (TARGET_ICPACKAGE >= LQFP48)
	IO_PORT_0      =      (0), 
	#endif
	#if (TARGET_ICPACKAGE >= LQFP64)
	IO_PORT_1      =      (1), 	
    #endif
	#if (TARGET_ICPACKAGE >= LQFP100)
	IO_PORT_2      =      (2), 	
    #endif
}IO_PIN_PORT_T;

/* IO Port present: port A/B/C available on LQFP48/LQFP64/LQFP100 package */
/* Global pin number */
typedef enum _IO_PIN_NO  {
	#if (TARGET_ICPACKAGE >= LQFP48)
	IO_P0_00      =      (0), 
	IO_P0_01      =      (1),
	IO_P0_02      =      (2), 
	IO_P0_03      =      (3), 
	IO_P0_04      =      (4),
	IO_P0_05      =      (5),
	IO_P0_06      =      (6), 
	IO_P0_07      =      (7),
	IO_P0_08      =      (8),
	IO_P0_09      =      (9), 
	IO_P0_10      =      (10),
	IO_P0_11      =      (11),
	IO_P0_12      =      (12),
	IO_P0_13      =      (13), 
	IO_P0_14      =      (14),
	IO_P0_15      =      (15),
	IO_P0_16      =      (16),
	IO_P0_17      =      (17),
	IO_P0_18      =      (18),
	IO_P0_19      =      (19),/* Not export SWC pin */
	IO_P0_20      =      (20),/* Not export SWD pin */
	//IO_P0_21      =      (21),/* Not export RST pin */
	IO_P0_22      =      (22), 
	IO_P0_23      =      (23),
	IO_P0_24      =      (24),
	IO_P0_25      =      (25),
	IO_P0_26      =      (26), 
	IO_P0_27      =      (27),
	IO_P0_28      =      (28),
	IO_P0_29      =      (29),
	#endif
	#if (TARGET_ICPACKAGE >= LQFP64)
	IO_P0_30      =      (30),
	IO_P0_31      =      (31), 
	IO_P1_00      =      (32), 
	IO_P1_01      =      (33), 
	IO_P1_02      =      (34), 
	IO_P1_03      =      (35),   
	IO_P1_04      =      (36), 
	IO_P1_05      =      (37), 
	IO_P1_06      =      (38), 
	IO_P1_07      =      (39), 
	IO_P1_08      =      (40), 
	IO_P1_09      =      (41), 
	IO_P1_10      =      (42),
	IO_P1_11      =      (43),
	
	IO_P2_12      =      (76),
	IO_P2_13      =      (77),	
	#endif 
	#if (TARGET_ICPACKAGE >= LQFP100)
	/* To be extended for LQFP100 */
    #endif
}IO_PIN_NO_T;

//DIO Level enum
typedef enum {
    DIO_LOW           =      ((U8)0),
    DIO_HIGH          =      ((U8)1),
}IO_PIN_STATE_T;

//DIO Direction enum
typedef enum {
    GPIO_DIR_IN       =      ((U8)0),
    GPIO_DIR_OUT      =      ((U8)1),
}IO_PIN_DIR_T;

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * @brief IO pin number in a port group 
 */
#define IO_PinNumberInGroup                    (32)
/**
 * @brief Pin No in a port group 
 */
#define IO_PinNoInPort(PinNo)                  (((PinNo)%IO_PinNumberInGroup))
/**
 * @brief Port No
 */
#define IO_PortNo(PinNo)                       (((PinNo)/IO_PinNumberInGroup))
/**
 * @brief Global Pin No
 */
#define IO_PinNo(Port,PinNoInPort)             (((Port)*IO_PinNumberInGroup)+(PinNoInPort))

/**
 * @brief Pin assignment
 */
#define IO_PINASSIGN_IDX(pinMovableFunc)       (((U32)(pinMovableFunc) >> 4))
#define IO_PINSHIFT(pinMovableFunc)            (8*((U32)(pinMovableFunc)&(0xF)))

/**
 * IO Pin Usage
 * See the User Manual for specific modes and functions supported by the
 * various LPC15XX pins.
 */
#define IO_GPIO                  0x0			/*!< Selects pin function - GPIO*/
#define IO_MovableFunc           0x1		    /*!< Selects pin function - Movable Function*/
#define IO_FixedFunc             0x2			/*!< Selects pin function - Fixed Function*/

/**
 * IOCON mode selection definitions for Digital IO
 * See the User Manual for specific modes and functions supported by the
 * various LPC15XX pins.
 */
#define DIOCON_MODE_INACT        (0x0 << 3)		/*!< No addition pin function */
#define DIOCON_MODE_PULLDOWN     (0x1 << 3)		/*!< Selects pull-down function */
#define DIOCON_MODE_PULLUP       (0x2 << 3)		/*!< Selects pull-up function */
#define DIOCON_MODE_REPEATER     (0x3 << 3)		/*!< Selects pin repeater function */
#define DIOCON_HYS_EN            (0x1 << 5)		/*!< Enables hysteresis */
#define DIOCON_INV_EN            (0x1 << 6)		/*!< Enables invert function on input */
#define DIOCON_ADMODE_EN         (0x0 << 7)		/*!< Enables analog input function (analog pins only) */
#define DIOCON_DIGMODE_EN        (0x1 << 7)		/*!< Enables digital function (analog pins only) */
#define DIOCON_SFI2C_EN          (0x0 << 8)		/*!< I2C standard mode/fast-mode */
#define DIOCON_STDI2C_EN         (0x1 << 8)		/*!< I2C standard I/O functionality */
#define DIOCON_FASTI2C_EN        (0x2 << 8)		/*!< I2C Fast-mode Plus */
#define DIOCON_OPENDRAIN_EN      (0x1 << 10)    /*!< Enables open-drain function */
#define DIOCON_S_MODE_0CLK       (0x0 << 11)	/*!< Bypass input filter */
#define DIOCON_S_MODE_1CLK       (0x1 << 11)	/*!< Input pulses shorter than 1 filter clock are rejected */
#define DIOCON_S_MODE_2CLK       (0x2 << 11)	/*!< Input pulses shorter than 2 filter clock2 are rejected */
#define DIOCON_S_MODE_3CLK       (0x3 << 11)	/*!< Input pulses shorter than 3 filter clock2 are rejected */
#define DIOCON_S_MODE(clks)      ((clks) << 11)	/*!< Select clocks for digital input filter mode */
#define DIOCON_CLKDIV(div)       ((div) << 13)	/*!< Select peripheral clock divider for input filter sampling clock, 2^n, n=0-6 */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/** @defgroup IOCON_15XX CHIP: LPC15xx IO Control driver
 * @ingroup CHIP_15XX_Drivers
 * @{
 */

/**
 * @brief LPC15XX Digital IO Configuration Unit register block structure
 * Offset 0x0000 - 0x0138 (LPC_15XX)
 */
typedef struct {			/*!< LPC15XX DIOCON Structure */
	__IO U32  DPIO[3][32];
} LPC_DIOCON_T;

/**
 * @brief LPC15XX Switch Matrix register block structure
 */
typedef struct {
	__IO U32 PINASSIGN[16];	/*!< Offset 0x0000: Pin Assignment register array */
	__I  U32 RESERVED0[96];
	__IO U32 PINENABLE[2];  /*!< Offset 0x01C0: Fixed pin enable/disable registers */
} LPC_SWM_T;

/**
 * @brief  GPIO port register block structure
 */
typedef struct {		    /*!< GPIO_PORT Structure */
	__IO U8  B[128][32];	/*!< Offset 0x0000: Byte pin registers ports 0 to n; pins PIOn_0 to PIOn_31 */
	__IO U32 W[32][32];	    /*!< Offset 0x1000: Word pin registers port 0 to n */
	__IO U32 DIR[32];		/*!< Offset 0x2000: Direction registers port n */
	__IO U32 MASK[32];		/*!< Offset 0x2080: Mask register port n */
	__IO U32 PIN[32];		/*!< Offset 0x2100: Portpin register port n */
	__IO U32 MPIN[32];		/*!< Offset 0x2180: Masked port register port n */
	__IO U32 SET[32];		/*!< Offset 0x2200: Write: Set register for port n Read: output bits for port n */
	__O  U32 CLR[32];		/*!< Offset 0x2280: Clear port n */
	__O  U32 NOT[32];		/*!< Offset 0x2300: Toggle port n */
} LPC_GPIO_T;

/**
 * @brief LPC15XX Switch Matrix Movable pins
 */
typedef enum _IO_SWM_PIN_MOVABLE  {
	SWM_UART0_TXD_O         = 0x00,		/*!< PINASSIGN0 - UART0 TXD Output */
	SWM_UART0_RXD_I         = 0x01,		/*!< PINASSIGN0 - UART0 RXD Input */
	SWM_UART0_RTS_O         = 0x02,		/*!< PINASSIGN0 - UART0 RTS Output */
	SWM_UART0_CTS_I         = 0x03,		/*!< PINASSIGN0 - UART0 CTS Input */
	SWM_UART0_SCLK_IO       = 0x10,		/*!< PINASSIGN1 - UART0 SCLK I/O */
	SWM_UART1_TXD_O         = 0x11,		/*!< PINASSIGN1 - UART1 TXD Output */
	SWM_UART1_RXD_I         = 0x12,		/*!< PINASSIGN1 - UART1 RXD Input */
	SWM_UART1_RTS_O         = 0x13,		/*!< PINASSIGN1 - UART1 RTS Output */
	SWM_UART1_CTS_I         = 0x20,		/*!< PINASSIGN2 - UART1 CTS Input */
	SWM_UART1_SCLK_IO       = 0x21,		/*!< PINASSIGN2 - UART1 SCLK I/O */
	SWM_UART2_TXD_O         = 0x22,		/*!< PINASSIGN2 - UART2 TXD Output */
	SWM_UART2_RXD_I         = 0x23,		/*!< PINASSIGN2 - UART2 RXD Input */
	SWM_UART2_SCLK_IO       = 0x30,		/*!< PINASSIGN3 - UART2 SCLK I/O */
	SWM_SSP0_SCK_IO         = 0x31,		/*!< PINASSIGN3 - SSP0 SCK I/O */
	SWM_SPI0_SCK_IO         = SWM_SSP0_SCK_IO,
	SWM_SSP0_MOSI_IO        = 0x32,		/*!< PINASSIGN3 - SSP0 MOSI I/O */
	SWM_SPI0_MOSI_IO        = SWM_SSP0_MOSI_IO,
	SWM_SSP0_MISO_IO        = 0x33,		/*!< PINASSIGN3 - SSP0 MISO I/O */
	SWM_SPI0_MISO_IO        = SWM_SSP0_MISO_IO,
	SWM_SSP0_SSELSN_0_IO    = 0x40,
	SWM_SPI0_SSELSN_0_IO    = SWM_SSP0_SSELSN_0_IO,
	SWM_SSP0_SSELSN_1_IO    = 0x41,
	SWM_SPI0_SSELSN_1_IO    = SWM_SSP0_SSELSN_1_IO,
	SWM_SSP0_SSELSN_2_IO    = 0x42,
	SWM_SPI0_SSELSN_2_IO    = SWM_SSP0_SSELSN_2_IO,
	SWM_SSP0_SSELSN_3_IO    = 0x43,
	SWM_SPI0_SSELSN_3_IO    = SWM_SSP0_SSELSN_3_IO,
	SWM_SSP1_SCK_IO         = 0x50,		/*!< PINASSIGN5 - SPI1 SCK I/O */
	SWM_SPI1_SCK_IO         = SWM_SSP1_SCK_IO,
	SWM_SSP1_MOSI_IO        = 0x51,		/*!< PINASSIGN5 - SPI1 MOSI I/O */
	SWM_SPI1_MOSI_IO        = SWM_SSP1_MOSI_IO,
	SWM_SSP1_MISO_IO        = 0x52,		/*!< PINASSIGN5 - SPI1 MISO I/O */
	SWM_SPI1_MISO_IO        = SWM_SSP1_MISO_IO,
	SWM_SSP1_SSELSN_0_IO    = 0x53,		/*!< PINASSIGN5 - SPI1 SSEL I/O */
	SWM_SPI1_SSELSN_0_IO    = SWM_SSP1_SSELSN_0_IO,
	SWM_SSP1_SSELSN_1_IO    = 0x60,
	SWM_SPI1_SSELSN_1_IO    = SWM_SSP1_SSELSN_1_IO,
	SWM_CAN_TD1_O           = 0x61,
	SWM_CAN_RD1_I           = 0x62,
	SWM_USB_VBUS_I          = 0x70,
	SWM_SCT0_OUT0_O         = 0x71,
	SWM_SCT0_OUT1_O         = 0x72,
	SWM_SCT0_OUT2_O         = 0x73,
	SWM_SCT1_OUT0_O         = 0x80,
	SWM_SCT1_OUT1_O         = 0x81,
	SWM_SCT1_OUT2_O         = 0x82,
	SWM_SCT2_OUT0_O         = 0x83,
	SWM_SCT2_OUT1_O         = 0x90,
	SWM_SCT2_OUT2_O         = 0x91,
	SWM_SCT3_OUT0_O         = 0x92,
	SWM_SCT3_OUT1_O         = 0x93,
	SWM_SCT3_OUT2_O         = 0xA0,
	SWM_SCT_ABORT0_I        = 0xA1,
	SWM_SCT_ABORT1_I        = 0xA2,
	SWM_ADC0_PIN_TRIG0_I    = 0xA3,
	SWM_ADC0_PIN_TRIG1_I    = 0xB0,
	SWM_ADC1_PIN_TRIG0_I    = 0xB1,
	SWM_ADC1_PIN_TRIG1_I    = 0xB2,
	SWM_DAC_PIN_TRIG_I      = 0xB3,
	SWM_DAC_SHUTOFF_I       = 0xC0,
	SWM_ACMP0_OUT_O         = 0xC1,
	SWM_ACMP1_OUT_O         = 0xC2,
	SWM_ACMP2_OUT_O         = 0xC3,
	SWM_ACMP3_OUT_O         = 0xD0,
	SWM_CLK_OUT_O           = 0xD1,
	SWM_ROSC0_O             = 0xD2,
	SWM_ROSC_RST0_I         = 0xD3,
	SWM_USB_FRAME_TOG_O     = 0xE0,
	SWM_QEI0_PHA_I          = 0xE1,
	SWM_QEI0_PHB_I          = 0xE2,
	SWM_QEI0_IDX_I          = 0xE3,
	SWM_GPIO_INT_BMATCH_O   = 0xF0,
	SWM_SWO_O               = 0xF1,
} IO_SWM_PIN_MOVABLE_T;

/**
 * @brief LPC15XX Switch Matrix Fixed pins
 * The 8th bit means reg offset
 */
typedef enum _IO_SWM_PIN_FIXED    {
	SWM_FIXED_ADC0_0    = 0x00,	/*!< ADC0_0 fixed pin enable/disable on pin P0_8 */
	SWM_FIXED_ADC0_1    = 0x01,	/*!< ADC0_1 fixed pin enable/disable on pin P0_7 */
	SWM_FIXED_ADC0_2    = 0x02,	/*!< ADC0_2 fixed pin enable/disable on pin P0_6 */
	SWM_FIXED_ADC0_3    = 0x03,	/*!< ADC0_3 fixed pin enable/disable on pin P0_5 */
	SWM_FIXED_ADC0_4    = 0x04,	/*!< ADC0_4 fixed pin enable/disable on pin P0_4 */
	SWM_FIXED_ADC0_5    = 0x05,	/*!< ADC0_5 fixed pin enable/disable on pin P0_3 */
	SWM_FIXED_ADC0_6    = 0x06,	/*!< ADC0_6 fixed pin enable/disable on pin P0_2 */
	SWM_FIXED_ADC0_7    = 0x07,	/*!< ADC0_7 fixed pin enable/disable on pin P0_1 */
	SWM_FIXED_ADC0_8    = 0x08,	/*!< ADC0_8 fixed pin enable/disable on pin P1_0 */
	SWM_FIXED_ADC0_9    = 0x09,	/*!< ADC0_9 fixed pin enable/disable on pin P0_31 */
	SWM_FIXED_ADC0_10   = 0x0A,	/*!< ADC0_10 fixed pin enable/disable on pin P0_0 */
	SWM_FIXED_ADC0_11   = 0x0B,	/*!< ADC0_11 fixed pin enable/disable on pin P0_30 */
	SWM_FIXED_ADC1_0    = 0x0C,	/*!< ADC1_0 fixed pin enable/disable/disable on pin P1_1 */
	SWM_FIXED_ADC1_1    = 0x0D,	/*!< ADC1_1 fixed pin enable/disable on pin P0_9 */
	SWM_FIXED_ADC1_2    = 0x0E,	/*!< ADC1_2 fixed pin enable/disable on pin P0_10 */
	SWM_FIXED_ADC1_3    = 0x0F,	/*!< ADC1_3 fixed pin enable/disable on pin P0_11 */
	SWM_FIXED_ADC1_4    = 0x10,	/*!< ADC1_4 fixed pin enable/disable on pin P1_2 */
	SWM_FIXED_ADC1_5    = 0x11,	/*!< ADC1_5 fixed pin enable/disable on pin P1_3 */
	SWM_FIXED_ADC1_6    = 0x12,	/*!< ADC1_6 fixed pin enable/disable on pin P0_13 */
	SWM_FIXED_ADC1_7    = 0x13,	/*!< ADC1_7 fixed pin enable/disable on pin P0_14 */
	SWM_FIXED_ADC1_8    = 0x14,	/*!< ADC1_8 fixed pin enable/disable on pin P0_15 */
	SWM_FIXED_ADC1_9    = 0x15,	/*!< ADC1_9 fixed pin enable/disable on pin P0_16 */
	SWM_FIXED_ADC1_10   = 0x16,	/*!< ADC1_10 fixed pin enable/disable on pin P1_4 */
	SWM_FIXED_ADC1_11   = 0x17,	/*!< ADC1_11 fixed pin enable/disable on pin P1_5 */
	SWM_FIXED_DAC_OUT   = 0x18,	/*!< DAC_OUT fixed pin enable/disable on pin P0_12 */
	SWM_FIXED_ACMP_I1   = 0x19,	/*!< ACMP input 1 (common input) fixed pin enable/disable on pin P0_27 */
	SWM_FIXED_ACMP_I2   = 0x1A,	/*!< ACMP input 1 (common input) fixed pin enable/disable on pin P1_6 */
	SWM_FIXED_ACMP0_I3  = 0x1B,	/*!< ACMP comparator 0 input 3 fixed pin enable/disable on pin P0_26 */
	SWM_FIXED_ACMP0_I4  = 0x1C,	/*!< ACMP comparator 0 input 4 fixed pin enable/disable on pin P0_25 */
	SWM_FIXED_ACMP1_I3  = 0x1D,	/*!< ACMP comparator 1 input 3 fixed pin enable/disable on pin P0_28 */
	SWM_FIXED_ACMP1_I4  = 0x1E,	/*!< ACMP comparator 1 input 4 fixed pin enable/disable on pin P1_10 */
	SWM_FIXED_ACMP2_I3  = 0x1F,	/*!< ACMP comparator 2 input 3 fixed pin enable/disable on pin P0_29 */
	SWM_FIXED_ACMP2_I4  = 0x80,	/*!< ACMP comparator 2 input 4 fixed pin enable/disable on pin P1_9 */
	SWM_FIXED_ACMP3_I3  = 0x81,	/*!< ACMP comparator 3 input 3 fixed pin enable/disable on pin P1_8 */
	SWM_FIXED_ACMP3_I4  = 0x82,	/*!< ACMP comparator 3 input 4 fixed pin enable/disable on pin P1_7 */
	SWM_FIXED_I2C0_SDA  = 0x83,	/*!< I2C0_SDA fixed pin enable/disable on pin P0_23 */
	SWM_FIXED_I2C0_SCL  = 0x84,	/*!< I2C0_SCL fixed pin enable/disable on pin P0_22 */
	SWM_FIXED_SCT0_OUT3 = 0x85,	/*!< SCT0_OUT3 fixed pin enable/disable on pin P0_0 */
	SWM_FIXED_SCT0_OUT4 = 0x86,	/*!< SCT0_OUT4 fixed pin enable/disable on pin P0_1 */
	SWM_FIXED_SCT0_OUT5 = 0x87,	/*!< SCT0_OUT5 fixed pin enable/disable on pin P0_18 */
	SWM_FIXED_SCT0_OUT6 = 0x88,	/*!< SCT0_OUT6 fixed pin enable/disable on pin P0_24 */
	SWM_FIXED_SCT0_OUT7 = 0x89,	/*!< SCT0_OUT7 fixed pin enable/disable on pin P1_14 */
	SWM_FIXED_SCT1_OUT3 = 0x8A,	/*!< SCT1_OUT3 fixed pin enable/disable on pin P0_2 */
	SWM_FIXED_SCT1_OUT4 = 0x8B,	/*!< SCT1_OUT4 fixed pin enable/disable on pin P0_3 */
	SWM_FIXED_SCT1_OUT5 = 0x8C,	/*!< SCT1_OUT5 fixed pin enable/disable on pin P0_14 */
	SWM_FIXED_SCT1_OUT6 = 0x8D,	/*!< SCT1_OUT6 fixed pin enable/disable on pin P0_20 */
	SWM_FIXED_SCT1_OUT7 = 0x8E,	/*!< SCT1_OUT7 fixed pin enable/disable on pin P1_17 */
	SWM_FIXED_SCT2_OUT3 = 0x8F,	/*!< SCT2_OUT3 fixed pin enable/disable on pin P0_6 */
	SWM_FIXED_SCT2_OUT4 = 0x90,	/*!< SCT2_OUT4 fixed pin enable/disable on pin P0_29 */
	SWM_FIXED_SCT2_OUT5 = 0x91,	/*!< SCT2_OUT5 fixed pin enable/disable on pin P1_20 */
	SWM_FIXED_SCT3_OUT3 = 0x92,	/*!< SCT3_OUT3 fixed pin enable/disable on pin P0_26 */
	SWM_FIXED_SCT3_OUT4 = 0x93,	/*!< SCT3_OUT4 fixed pin enable/disable on pin P1_8 */
	SWM_FIXED_SCT3_OUT5 = 0x94,	/*!< SCT3_OUT5 fixed pin enable/disable on pin P1_24 */
	SWM_FIXED_RESETN    = 0x95,	/*!< RESETN fixed pin enable/disable on pin P0_21 */
	SWM_FIXED_SWCLK_TCK = 0x96,	/*!< SWCLK_TCK fixed pin enable/disable on pin P0_19 */
	SWM_FIXED_SWDIO     = 0x97,	/*!< SWDIO fixed pin enable/disable on pin P0_20 */
	SWM_FIXED_UNASSIGN  = 0xFF, /*!< Unassigned Fixed function */
} IO_SWM_PIN_FIXED_T;

/* Fixed func reg idx */
#define SWM_FIXED_REGIDX(ff)   (((U32) ff) >> 7)
/* Fixed func shift in a reg */
#define SWM_FIXED_SHIFT(ff)    (((U32) ff) & 0x1F)
/**
 * @brief IO pin Setup structure
 */
typedef struct {
    IO_PIN_NO_T                pin;	       /* Pin number */
    U8                         pinusage;   /* GPIO, movable function or fixed function*/
    U16                        initstat;   /* Init state for target pin */
	U32                        diocfg;     /* Digital IO pin configuration*/
}IO_PIN_SETUP_T;

/**
 * @brief IO pin and its fixed functions
 */
typedef struct {
    IO_PIN_NO_T                pin;	       /* Pin number */
    IO_SWM_PIN_FIXED_T         ff1;        /* Assigned fixed function 1 if exists */
    IO_SWM_PIN_FIXED_T         ff2;        /* Assigned fixed function 2 if exists */
}IO_PIN_FIXFUNC_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initlize all IO pins according to table IOPinSetup
 * @param	Nothing
 * @return	true if Init is successed, false if failed
 */
bool IO_bInit(void);

/**
 * @brief	Get a GPIO pin state via the GPIO byte register
 * @param	tePin		: GPIO pin to get state
 * @return	DIO_LOW or DIO_HIGH
 */
STATIC INLINE IO_PIN_STATE_T IO_GPIO_teGet(IO_PIN_NO_T tePin)
{
	return (IO_PIN_STATE_T)(READ_REG(LPC_GPIO->B[IO_PortNo(tePin)][IO_PinNoInPort(tePin)]));
}

/**
 * @brief	Set a GPIO pin state via the GPIO byte register
 * @param	tePin	: GPIO pin to set
 * @param	teState	: DIO_LOW or DIO_HIGH
 * @return	Nothing
 */
STATIC INLINE void IO_GPIO_vSet(IO_PIN_NO_T tePin, IO_PIN_STATE_T teState)
{
	WRITE_REG(LPC_GPIO->B[IO_PortNo(tePin)][IO_PinNoInPort(tePin)], (U8)teState);
}

/**
 * @brief	Set a GPIO as toggled mode
 * @param	tePin	: GPIO pin to set
 * @Note	Swap DIO_LOW <--> DIO_LOW
 */
STATIC INLINE void IO_GPIO_vToggle(IO_PIN_NO_T tePin)
{
    IO_GPIO_vSet(tePin, ((DIO_HIGH == IO_GPIO_teGet(tePin))? DIO_LOW : DIO_HIGH));
}

/**
 * @brief	Set GPIO direction for a single GPIO pin to an output or input
 * @param	tePin   : GPIO pin to get state
 * @param	teDir	: GPIO_DIR_OUT or GPIO_DIR_IN
 * @return	Nothing
 */
STATIC INLINE void IO_GPIO_vSetDir(IO_PIN_NO_T tePin, IO_PIN_DIR_T teDir)
{
	/* Set high for output GPIO pin */
	SET_REG_FIELD(LPC_GPIO->DIR[IO_PortNo(tePin)], 
                  BIT(IO_PinNoInPort(tePin)),		
                  ((GPIO_DIR_OUT == teDir)? BIT(IO_PinNoInPort(tePin)) : 0));
}

/**
 * @brief	Set GPIO Port Mask
 * @param	tePort   : GPIO port to set mask
 * @param	unMask	: GPIO_Port Mask 
 * @return	Nothing
 * @note    0 = Read MPORT: pin state; write MPORT: load output bit.
 *          1 = Read MPORT: 0; write MPORT: output bit not affected.
 */
STATIC INLINE void IO_GPIO_vSetPortMask(IO_PIN_PORT_T tePort, U32 unMask)
{
	WRITE_REG(LPC_GPIO->MASK[tePort], unMask);
}

/**
 * @brief	Get GPIO masked port state
 * @param	tePort   : GPIO Port to get state
 * @return	Masked Port value
 * @note    the value read is masked by
            ANDing with the inverted contents of the corresponding MASK register, and writing to one
            of these registers only affects output register bits that are enabled by zeros in the
            corresponding MASK register
 */
STATIC INLINE U32 IO_GPIO_unGetPortMaskedState(IO_PIN_PORT_T tePort)
{
	return READ_REG(LPC_GPIO->MPIN[tePort]);
}

/**
 * @brief	Set GPIO masked port state
 * @param	tePort   : GPIO Port to set state
 * @param	unPortState	: GPIO_Port Masked State  
 * @return	Masked Port value
 */
STATIC INLINE void IO_GPIO_vSetPortMaskedState(IO_PIN_PORT_T tePort, U32 unPortState)
{
	WRITE_REG(LPC_GPIO->MPIN[tePort], unPortState);
}
void IO_SWM_vMovablePinAssignZ(IO_PIN_NO_T tePin, IO_SWM_PIN_MOVABLE_T teMF);
void IO_SWM_vEnableFixedPinZ(IO_SWM_PIN_FIXED_T teFF);



#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

