/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SSI.h                                                 $ */
/*                                                                            */
/*    $Revision:: 343                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-25 10:27:38 +0800 (周二, 25 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - SPI0/1          */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SPI_H
#define SPI_H

#include "platform.h"
#include "system.h"

#include "ssi_cfg.h"

#ifdef CFG_SPI_USED
/** @defgroup SPI_15XX CHIP: LPC15xx SPI Controller
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Numbere of SPI0/1 */
#define  MAX_SPI_NUMBER              (2)

/* SPI Dummy data */
#define  SPI_DUMMY_DATA              (0x55)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * Macro defines for SPI Configuration register
 */
/* SPI CFG Register BitMask */
#define SPI_CFG_BITMASK                ((U32) 0xFBD)
/** SPI enable  */
#define SPI_CFG_SPI_EN                 ((U32) (1 << 0))
/** SPI Slave Mode Select */
#define SPI_CFG_SLAVE_EN               ((U32) (0 << 2))
/** SPI Master Mode Select */
#define SPI_CFG_MASTER_EN              ((U32) (1 << 2))
/** SPI MSB First mode enable */
#define SPI_CFG_MSB_FIRST_EN           ((U32) (0 << 3))	/*Data will be transmitted and received in standard order (MSB first).*/
/** SPI LSB First mode enable */
#define SPI_CFG_LSB_FIRST_EN           ((U32) (1 << 3))   /*Data will be transmitted and received in reverse order (LSB first).*/
/** SPI Clock Phase Select*/
#define SPI_CFG_CPHA_FIRST             ((U32) (0 << 4))	/*Capture data on the first edge, Change data on the following edge*/
#define SPI_CFG_CPHA_SECOND            ((U32) (1 << 4))	/*Change data on the second edge, Capture data on the following edge*/
/** SPI Clock Polarity Select*/
#define SPI_CFG_CPOL_LO                ((U32) (0 << 5))	/* The rest state of the clock (between frames) is low.*/
#define SPI_CFG_CPOL_HI                ((U32) (1 << 5))	/* The rest state of the clock (between frames) is high.*/
/** SPI control 1 loopback mode enable  */
#define SPI_CFG_LBM_EN                 ((U32) (1 << 7))

/** SPI SSEL0 Polarity Select*/
#define SPI_CFG_SPOL0_LO               ((U32) (0 << 8))	/* SSEL0 is active Low */
#define SPI_CFG_SPOL0_HI               ((U32) (1 << 8))	/* SSEL0 is active High */
/** SPI SSEL1 Polarity Select*/
#define SPI_CFG_SPOL1_LO               ((U32) (0 << 9))	/* SSEL1 is active Low */
#define SPI_CFG_SPOL1_HI               ((U32) (1 << 9))	/* SSEL1 is active High */
/** SPI SSEL2 Polarity Select*/
/** Note that SSEL2, SSEL3 is only available on SPI0 not on SPI1 */
#define SPI_CFG_SPOL2_LO               ((U32) (0 << 10))	/* SSEL2 is active Low */
#define SPI_CFG_SPOL2_HI               ((U32) (1 << 10))	/* SSEL2 is active High */
/** SPI SSEL3 Polarity Select*/
#define SPI_CFG_SPOL3_LO               ((U32) (0 << 11))	/* SSEL3 is active Low */
#define SPI_CFG_SPOL3_HI               ((U32) (1 << 11))	/* SSEL3 is active High */

/**
 * Macro defines for SPI Delay register
 */
/** SPI DLY Register Mask	*/
#define  SPI_DLY_BITMASK               ((U32) 0xFFFF)
/** Controls the amount of time between SSEL assertion and the beginning of a data frame.	*/
#define  SPI_DLY_PRE_DELAY(n)          ((U32) ((n) & 0x0F))				/* Time Unit: SPI clock time */
/** Controls the amount of time between the end of a data frame and SSEL deassertion.	*/
#define  SPI_DLY_POST_DELAY(n)         ((U32) (((n) & 0x0F) << 4))		/* Time Unit: SPI clock time */
/** Controls the minimum amount of time between adjacent data frames.	*/
#define  SPI_DLY_FRAME_DELAY(n)        ((U32) (((n) & 0x0F) << 8))		/* Time Unit: SPI clock time */
/** Controls the minimum amount of time that the SSEL is deasserted between transfers.	*/
#define  SPI_DLY_TRANSFER_DELAY(n)     ((U32) (((n) & 0x0F) << 12))	    /* Time Unit: SPI clock time */

/**
 * Macro defines for SPI Status register
 */
/* SPI STAT Register BitMask */
#define SPI_STAT_BITMASK               ((U32) 0x1FF)
/* Receiver Ready Flag */
#define SPI_STAT_RXRDY                 ((U32) (1 << 0))	/* Data is ready for read */
/* Transmitter Ready Flag */
#define SPI_STAT_TXRDY                 ((U32) (1 << 1))	/* Data may be written to transmit buffer */
/* Receiver Overrun interrupt flag */
#define SPI_STAT_RXOV                  ((U32) (1 << 2))	/* Data comes while receiver buffer is in used */
/* Transmitter Underrun interrupt flag (In Slave Mode only) */
#define SPI_STAT_TXUR                  ((U32) (1 << 3))	/* There is no data to be sent in the next input clock */
/* Slave Select Assert */
#define SPI_STAT_SSA                   ((U32) (1 << 4))	/* There is SSEL transition from deasserted to asserted */
/* Slave Select Deassert */
#define SPI_STAT_SSD                   ((U32) (1 << 5))	/* There is SSEL transition from asserted to deasserted */
/* Stalled status flag */
#define SPI_STAT_STALLED               ((U32) (1 << 6))	/* SPI is currently in a stall condition. */
/* End Transfer flag. */
#define SPI_STAT_EOT                   ((U32) (1 << 7))	/* The current frame is the last frame of the current  transfer. */
/* Master Idle status flag. */
#define SPI_STAT_MSTIDLE               ((U32) (1 << 8))	/* SPI master function is fully idle. */

/* Clear RXOV Flag */
#define SPI_STAT_CLR_RXOV              ((U32) (1 << 2))
/* Clear TXUR Flag */
#define SPI_STAT_CLR_TXUR              ((U32) (1 << 3))
/* Clear SSA Flag */
#define SPI_STAT_CLR_SSA               ((U32) (1 << 4))
/* Clear SSD Flag */
#define SPI_STAT_CLR_SSD               ((U32) (1 << 5))
/*Force an end to the current transfer */
#define SPI_STAT_FORCE_EOT             ((U32) (1 << 7))

/**
 * Macro defines for SPI Interrupt Enable read and Set register
 */
/* SPI INTENSET Register BitMask */
#define SPI_INTENSET_BITMASK           ((U32) 0x3F)
/** Enable Interrupt when receiver data is available */
#define SPI_INTENSET_RXRDYEN           ((U32) (1 << 0))
/** Enable Interrupt when the transmitter holding register is available. */
#define SPI_INTENSET_TXRDYEN           ((U32) (1 << 1))
/**  Enable Interrupt when a receiver overrun occurs */
#define SPI_INTENSET_RXOVEN            ((U32) (1 << 2))
/**  Enable Interrupt when a transmitter underrun occurs (In Slave Mode Only)*/
#define SPI_INTENSET_TXUREN            ((U32) (1 << 3))
/**  Enable Interrupt when the Slave Select is asserted.*/
#define SPI_INTENSET_SSAEN             ((U32) (1 << 4))
/**  Enable Interrupt when the Slave Select is deasserted..*/
#define SPI_INTENSET_SSDEN             ((U32) (1 << 5))

/**
 * Macro defines for SPI Interrupt Enable Clear register
 */
/* SPI INTENCLR Register BitMask */
#define SPI_INTENCLR_BITMASK           ((U32) 0x3F)
/** Disable Interrupt when receiver data is available */
#define SPI_INTENCLR_RXRDYEN           ((U32) (1 << 0))
/** Disable Interrupt when the transmitter holding register is available. */
#define SPI_INTENCLR_TXRDYEN           ((U32) (1 << 1))
/** Disable Interrupt when a receiver overrun occurs */
#define SPI_INTENCLR_RXOVEN            ((U32) (1 << 2))
/** Disable Interrupt when a transmitter underrun occurs (In Slave Mode Only)*/
#define SPI_INTENCLR_TXUREN            ((U32) (1 << 3))
/** Disable Interrupt when the Slave Select is asserted.*/
#define SPI_INTENCLR_SSAEN             ((U32) (1 << 4))
/** Disable Interrupt when the Slave Select is deasserted..*/
#define SPI_INTENCLR_SSDEN             ((U32) (1 << 5))

/**
 * Macro defines for SPI Receiver Data register
 */
/* SPI RXDAT Register BitMask */
#define SPI_RXDAT_BITMASK              ((U32) 0x1FFFFF)
/** Receiver Data  */
#define SPI_RXDAT_DATA(n)              ((U32) ((n) & 0xFFFF))
/** The state of SSEL0 pin  */
#define SPI_RXDAT_RXSSEL0_ACTIVE       ((U32) (0 << 16))	/* SSEL0 is in active state */
#define SPI_RXDAT_RXSSEL0_INACTIVE     ((U32) (1 << 16))	/* SSEL0 is in inactive state */
#define SPI_RXDAT_RXSSEL0_FLAG         ((U32) (1 << 16))	/* SSEL0 Rx Flag */
/** The state of SSEL1 pin  */
#define SPI_RXDAT_RXSSEL1_ACTIVE       ((U32) (0 << 17))	/* SSEL1 is in active state */
#define SPI_RXDAT_RXSSEL1_INACTIVE     ((U32) (1 << 17))	/* SSEL1 is in inactive state */
#define SPI_RXDAT_RXSSEL1_FLAG         ((U32) (1 << 17))	/* SSEL1 Rx Flag */
/** The state of SSEL2 pin  */
#define SPI_RXDAT_RXSSEL2_ACTIVE       ((U32) (0 << 18))	/* SSEL2 is in active state */
#define SPI_RXDAT_RXSSEL2_INACTIVE     ((U32) (1 << 18))	/* SSEL2 is in inactive state */
#define SPI_RXDAT_RXSSEL2_FLAG         ((U32) (1 << 18))	/* SSEL2 Rx Flag */
/** The state of SSEL3 pin  */
#define SPI_RXDAT_RXSSEL3_ACTIVE       ((U32) (0 << 19))	/* SSEL3 is in active state */
#define SPI_RXDAT_RXSSEL3_INACTIVE     ((U32) (1 << 19))	/* SSEL3 is in inactive state */
#define SPI_RXDAT_RXSSEL3_FLAG         ((U32) (1 << 19))	/* SSEL3 Rx Flag */

/* Mask for Slave Select bits */
#define SPI_RXDAT_SSEL_MASK            ((U32) (0x0F0000))     

/** Start of Transfer flag  */
#define SPI_RXDAT_SOT                  ((U32) (1 << 20))	/* This is the first frame received after SSEL is asserted */

/* Get application sepcific data fields */
#define SPI_FRAME_DATA(raw)            (SPI_RXDAT_DATA(raw))

/* Support macro for data channel */
#define SPI_DR(spich)                  (SPI_PTR[spich]->RXDAT)
#define SPI_DT(spich)                  (SPI_PTR[spich]->TXDAT)
#define SPI0_DR                        (0x40048014)
#define SPI1_DR                        (0x4004C014)
#define SPI0_DT                        (0x40048020)
#define SPI1_DT                        (0x4004C020)

/**
 * Macro defines for SPI Transmitter Data and Control register
 */
/* SPI TXDATCTL Register BitMask */
#define SPI_TXDATCTL_BITMASK         ((U32) 0xF7FFFFF)
/* SPI Transmit Data */
#define SPI_TXDATCTL_DATA(n)         ((U32) ((n) & 0xFFFF))
/*Assert/Deassert SSEL0 pin*/
#define SPI_TXDATCTL_ASSERT_SSEL0    ((U32) (0 << 16))
#define SPI_TXDATCTL_DEASSERT_SSEL0  ((U32) (1 << 16))
/*Assert/Deassert SSEL1 pin*/
#define SPI_TXDATCTL_ASSERT_SSEL1    ((U32) (0 << 17))
#define SPI_TXDATCTL_DEASSERT_SSEL1  ((U32) (1 << 17))
/*Assert/Deassert SSEL2 pin*/
/** Note that SSEL2, SSEL3 is only available on SPI0 not on SPI1 */
#define SPI_TXDATCTL_ASSERT_SSEL2    ((U32) (0 << 18))
#define SPI_TXDATCTL_DEASSERT_SSEL2  ((U32) (1 << 18))
/*Assert/Deassert SSEL3 pin*/
#define SPI_TXDATCTL_ASSERT_SSEL3    ((U32) (0 << 19))
#define SPI_TXDATCTL_DEASSERT_SSEL3  ((U32) (1 << 19))
/* Mask for Slave Select bits */
#define SPI_TXDATCTL_SSEL_MASK       ((U32) (0x0F0000))

/* define assert of SSEL index */
#define SPI_TXDATCTL_ASSERT(n)       ((U32) (0 << n))
#define SPI_TXDATCTL_DEASSERT(n)     ((U32) (1 << n))

/** End of Transfer flag (TRANSFER_DELAY is applied after sending the current frame)  */
#define SPI_TXDATCTL_EOT             ((U32) (1 << 20))	            /* This is the last frame of the current transfer */
/** End of Frame flag (FRAME_DELAY is applied after sending the current part) */
#define SPI_TXDATCTL_EOF             ((U32) (1 << 21))	            /* This is the last part of the current frame */
/** Receive Ignore Flag */
#define SPI_TXDATCTL_RXIGNORE        ((U32) (1 << 22))	            /* Received data is ignored */
/** Receive Ignore Flag */
#define SPI_TXDATCTL_RXRECEIVE       ((U32) (0 << 22))	            /* Received data is enabled */
/** Transmit Data Length */
#define SPI_TXDATCTL_LEN(n)          ((U32) (((n) & 0x0F) << 24))	/* Frame Length -1 */

/**
 * Macro defines for SPI Transmitter Data Register
 */
/* SPI Transmit Data */
#define SPI_TXDAT_DATA(n)            ((U32) ((n) & 0xFFFF))
#define SPI_TXDAT_DATA_MASK          (0xFFFF)

/**
 * Macro defines for SPI Transmitter Control register
 */
/* SPI TXDATCTL Register BitMask */
#define SPI_TXCTL_BITMASK            ((U32) 0xF7F0000)
/*Assert/Deassert SSEL0 pin*/
#define SPI_TXCTL_ASSERT_SSEL0       ((U32) (0 << 16))
#define SPI_TXCTL_DEASSERT_SSEL0     ((U32) (1 << 16))
/*Assert/Deassert SSEL1 pin*/
#define SPI_TXCTL_ASSERT_SSEL1       ((U32) (0 << 17))
#define SPI_TXCTL_DEASSERT_SSEL1     ((U32) (1 << 17))
/*Assert/Deassert SSEL2 pin*/
/** Note that SSEL2, SSEL3 is only available on SPI0 not on SPI1 */
#define SPI_TXCTL_ASSERT_SSEL2       ((U32) (0 << 18))
#define SPI_TXCTL_DEASSERT_SSEL2     ((U32) (1 << 18))
/*Assert/Deassert SSEL3 pin*/
#define SPI_TXCTL_ASSERT_SSEL3       ((U32) (0 << 19))
#define SPI_TXCTL_DEASSERT_SSEL3     ((U32) (1 << 19))
/*Assert/Deassert Mask*/
#define SPI_TXCTL_ASSERT_MASK        ((U32) (0x000F0000))	        

/** End of Transfer flag (TRANSFER_DELAY is applied after sending the current frame)  */
#define SPI_TXCTL_EOT                ((U32) (1 << 20))	            /* This is the last frame of the current transfer */
/** End of Frame flag (FRAME_DELAY is applied after sending the current part) */
#define SPI_TXCTL_EOF                ((U32) (1 << 21))	            /* This is the last part of the current frame */
/** Receive Ignore Flag */
#define SPI_TXCTL_RXIGNORE           ((U32) (1 << 22))	            /* Received data is ignored */
/** Transmit Data Length */
#define SPI_TXCTL_LEN(n)             ((U32) (((n) & 0x0F) << 24))	/* Frame Length -1 */
#define SPI_TXCTL_MASK               ((U32) ((0x0F) << 24))	        /* Frame Length Mask */


/**
 * Macro defines for SPI Divider register
 */
/** Rate divider value  (In Master Mode only)*/
#define SPI_DIV_VAL(n)               ((U32) ((n) & 0xFFFF))	/* SPI_CLK = PCLK/(DIV_VAL+1)*/

/**
 * Macro defines for SPI Interrupt Status register
 */
/* SPI INTSTAT Register Bitmask */
#define SPI_INTSTAT_BITMASK          ((U32) 0x3F)
/* Receiver Ready Flag */
#define SPI_INTSTAT_RXRDY            ((U32) (1 << 0))	/* Data is ready for read */
/* Transmitter Ready Flag */
#define SPI_INTSTAT_TXRDY            ((U32) (1 << 1))	/* Data may be written to transmit buffer */
/* Receiver Overrun interrupt flag */
#define SPI_INTSTAT_RXOV             ((U32) (1 << 2))	/* Data comes while receiver buffer is in used */
/* Transmitter Underrun interrupt flag (In Slave Mode only) */
#define SPI_INTSTAT_TXUR             ((U32) (1 << 3))	/* There is no data to be sent in the next input clock */
/* Slave Select Assert */
#define SPI_INTSTAT_SSA              ((U32) (1 << 4))	/* There is SSEL transition from deasserted to asserted */
/* Slave Select Deassert */
#define SPI_INTSTAT_SSD              ((U32) (1 << 5))	/* There is SSEL transition from asserted to deasserted */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* Logic numbered SPI0/1 */
typedef enum {
     #if (ENABLE == CFG_SPI0_ENABLE)
     SPI0       = 0,
     #endif
	 #if (ENABLE == CFG_SPI1_ENABLE)
     SPI1       = 1,
     #endif
}SPI_GROUP_T;

/**
 * @brief SPI register block structure
 */
typedef struct {			    /*!< SPI Structure */
	__IO U32  CFG;				/*!< SPI Configuration register*/
	__IO U32  DLY;				/*!< SPI Delay register*/
	__IO U32  STAT;			    /*!< SPI Status. register*/
	__IO U32  INTENSET;		    /*!< SPI Interrupt Enable.Set register*/
	__O  U32  INTENCLR;		    /*!< SPI Interrupt Enable Clear. register*/
	__I  U32  RXDAT;			/*!< SPI Receive Data register*/
	__IO U32  TXDATCTL;		    /*!< SPI Transmit Data with Control register*/
	__IO U32  TXDAT;			/*!< SPI Transmit Data register*/
	__IO U32  TXCTRL;			/*!< SPI Transmit Control register*/
	__IO U32  DIV;				/*!< SPI clock Divider register*/
	__I  U32  INTSTAT;			/*!< SPI Interrupt Status register*/
} LPC_SPI_T;

/** @brief SPI Mode*/
typedef enum {
	SPI_MODE_MASTER       = SPI_CFG_MASTER_EN,		    /* Master Mode */
	SPI_MODE_SLAVE        = SPI_CFG_SLAVE_EN			/* Slave Mode */
} SPI_MODE_T;

/** @brief SPI Clock Mode*/
typedef enum IP_SPI_CLOCK_MODE {
	SPI_CLOCK_CPHA0_CPOL0 = SPI_CFG_CPHA_FIRST | SPI_CFG_CPOL_LO,		    /**< CPHA = 0, CPOL = 0 */
	SPI_CLOCK_CPHA0_CPOL1 = SPI_CFG_CPHA_FIRST | SPI_CFG_CPOL_HI,		    /**< CPHA = 0, CPOL = 1 */
	SPI_CLOCK_CPHA1_CPOL0 = SPI_CFG_CPHA_SECOND| SPI_CFG_CPOL_LO,			/**< CPHA = 1, CPOL = 0 */
	SPI_CLOCK_CPHA1_CPOL1 = SPI_CFG_CPHA_SECOND|SPI_CFG_CPOL_HI,			/**< CPHA = 1, CPOL = 1 */
	SPI_CLOCK_MODE0       = SPI_CLOCK_CPHA0_CPOL0,                          /**< alias */
	SPI_CLOCK_MODE1       = SPI_CLOCK_CPHA1_CPOL0,                          /**< alias */
	SPI_CLOCK_MODE2       = SPI_CLOCK_CPHA0_CPOL1,                          /**< alias */
	SPI_CLOCK_MODE3       = SPI_CLOCK_CPHA1_CPOL1,                          /**< alias */
} SPI_CLOCK_MODE_T;

/** @brief SPI Data Order Mode*/
typedef enum IP_SPI_DATA_ORDER {
	SPI_DATA_MSB_FIRST    = SPI_CFG_MSB_FIRST_EN,			/* Standard Order */
	SPI_DATA_LSB_FIRST    = SPI_CFG_LSB_FIRST_EN,			/* Reverse Order */
} SPI_DATA_ORDER_T;

/**
 * @brief SPI Configure Struct
 */
typedef struct {
	SPI_MODE_T         Mode;		/* Mode Select */
	SPI_CLOCK_MODE_T   ClockMode;	/* CPHA CPOL Select */
	SPI_DATA_ORDER_T   DataOrder;	/* MSB/LSB First */
	U32                SSELPol;  	/* SSEL Polarity Select */
	U16                ClkDiv;		/* SPI Clock Divider Value */
} SPI_CONFIG_T;

/**
 * @brief SPI Delay Configure Struct
 */
typedef struct {
	U8     PreDelay;				/* Pre-delay value in SPI clock time */
	U8     PostDelay;				/* Post-delay value in SPI clock time */
	U8     FrameDelay;				/* Delay value between frames of a transfer in SPI clock time */
	U8     TransferDelay;			/* Delay value between transfers in SPI clock time */
} SPI_DELAY_CONFIG_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
/* Reg map of SPI0/1 */
STATIC LPC_SPI_T* const SPI_PTR[MAX_SPI_NUMBER] = {LPC_SPI0, LPC_SPI1};

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief SPI Initialization
 * @param	Nothing
 * @return	Nothing
 */
void SPI_vInit(void);

/**
 * @brief	Reset SPI peripheral
 * @param	Nothing
 * @return  Nothing
 */
void SPI_vReset(void);

/**
 * @brief	Shutdown SPI peripheral
 * @param	Nothing
 * @return  Nothing
 */
void SPI_vShutdown(void);

/**
 * @brief	Config SPI peripheral
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @param	pConfig	: configurations
 * @return  Nothing 
 * @note    Used to reconfig SPI in running time
 */
void SPI_vConfig(SPI_GROUP_T teSPI, SPI_CONFIG_T *pConfig);

/**
 * @brief	Start a SPI peripheral
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	Nothing
 */
void SPI_vStart(SPI_GROUP_T teSPI);

/**
 * @brief	Stop a SPI peripheral
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	Nothing
 */
void SPI_vStop(SPI_GROUP_T teSPI);

/**
 * @brief	Get a status flag of SPI controller
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	SPI Status (Or-ed bit value of SPI_STAT_*)
 */
STATIC INLINE U32 SPI_nGetIntStatus(SPI_GROUP_T teSPI)  
{
	return READ_REG(SPI_PTR[teSPI]->INTSTAT);
}

/**
 * @brief	Get a status flag of SPI controller
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	SPI Status (Or-ed bit value of SPI_STAT_*)
 */
STATIC INLINE U32 SPI_nGetStatus(SPI_GROUP_T teSPI)  
{
	return READ_REG(SPI_PTR[teSPI]->STAT);
}

/**
 * @brief	Get a status flag of SPI controller
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	SPI Status (Or-ed bit value of SPI_STAT_*)
 */
STATIC INLINE BOOL SPI_bGetStatus(SPI_GROUP_T teSPI, U32 nStateMask)  
{
	return ((READ_REG(SPI_PTR[teSPI]->STAT)&nStateMask)!= 0);
}

/**
 * @brief	Clear a flag of SPI status
 * @param	spi	: spi number - SPI_0/SPI_1
 * @param	unFlagmask	: Clear Flag (Or-ed bit value of SPI_STAT_CLR_*)
 *          SPI_STAT_RXRDY and SPI_STAT_TXRDY cannot be cleared 
 * @return	Nothing
 */
STATIC INLINE void SPI_vClearStatus(SPI_GROUP_T teSPI, U32 nFlagmask)  
{
	/* Write 1 to clear flag fields */
    WRITE_REG(SPI_PTR[teSPI]->STAT, (nFlagmask&SPI_STAT_BITMASK));
} 

/**
 * @brief	Send a frame data, which length is defined in spi_cfg
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @param	nData    : data content
 * @return  Nothing
 */
void SPI_vSendData(SPI_GROUP_T teSPI, U16 nData);

/**
 * @brief	Send a frame data using blocking mode, which length is defined in spi_cfg
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @param	nData    : data content
 * @return  Nothing
 */
void SPI_vSendDataBlocking(SPI_GROUP_T teSPI, U16 nData);

/**
 * @brief	Echo back the latest received data
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @return	bool    : true - the first frame; false - not
 */
STATIC INLINE void SPI_nEchoData(SPI_GROUP_T teSPI)
{
	while(!SPI_bGetStatus(teSPI, SPI_STAT_TXRDY)){;}
	
	SPI_DT(teSPI) = (U16)SPI_FRAME_DATA(SPI_DR(teSPI));
}

/**
 * @brief	Is the current received is the first frame
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @return	bool    : true - the first frame; false - not
 */
STATIC INLINE U16 SPI_nReceiveData(SPI_GROUP_T teSPI)
{
	return (U16)SPI_RXDAT_DATA(READ_REG(SPI_PTR[teSPI]->RXDAT));
}

STATIC INLINE void Chip_SPI_EnableLoopBack(SPI_GROUP_T teSPI)
{
	
	SET_REG_FIELD(SPI_PTR[teSPI]->CFG, SPI_CFG_LBM_EN, SPI_CFG_LBM_EN);
}

#ifdef __cplusplus
}
#endif
/**
 * @}
 */

#endif /* CFG_SPI_USED */

#endif

