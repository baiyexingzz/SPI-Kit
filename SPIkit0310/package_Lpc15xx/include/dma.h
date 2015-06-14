/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DMA.h                                                  $ */
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
/*  Description::   Low-level macros, definition, functions - DMA Controller  */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef DMA_H
#define DMA_H

#include "platform.h"

#include "system.h"
#include "inmux.h"
#include "dma_cfg.h"

#ifdef CFG_DMA_USED

/** @defgroup DMA_15XX CHIP: LPC15xx DMA Controller driver
 *  @ingroup 15XX_Drivers
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* On LPC15xx, Max DMA channel is 18 */
#define MAX_DMA_CHANNEL             (18)

/* DMA interrupt status bits (common) */
#define DMA_INTSTAT_ACTIVEINT       0x2		/*!< Summarizes whether any enabled interrupts are pending */
#define DMA_INTSTAT_ACTIVEERRINT    0x4		/*!< Summarizes whether any error interrupts are pending */

/* DMA all channel interrupt mask */
#define DMA_INT_MASK                (0x0003FFFF)

/* Support macro for DMA_CHDESC_T */
#define DMA_ADDR(addr)              ((U32) (addr))

/* DMA Control registers definitions */
#define DMA_CTRL_EN_MASK            (0x1)
#define DMA_CTRL_EN                 (1 << 0)
#define DMA_CTRL_DIS                (0 << 0)	

/* Support definitions for setting the configuration of a DMA channel. You
   will need to get more information on these options from the User manual. */
#define DMA_CFG_MASK            (0x0007CF73)
#define DMA_CFG_PERIPHREQEN     (1 << 0)	/*!< Enables Peripheral DMA requests */
#define DMA_CFG_HWTRIGEN        (1 << 1)	/*!< Use hardware triggering via input mux */
#define DMA_CFG_TRIGPOL_LOW     (0 << 4)	/*!< Hardware trigger is active low or falling edge */
#define DMA_CFG_TRIGPOL_HIGH    (1 << 4)	/*!< Hardware trigger is active high or rising edge */
#define DMA_CFG_TRIGPOL_FALLING (0 << 4)	/*!< Hardware trigger is active low or falling edge */
#define DMA_CFG_TRIGPOL_RISING  (1 << 4)	/*!< Hardware trigger is active high or rising edge */
#define DMA_CFG_TRIGPOL_LOW     (0 << 4)	/*!< Hardware trigger is active low or falling edge */
#define DMA_CFG_TRIGPOL_HIGH    (1 << 4)	/*!< Hardware trigger is active high or rising edge */
#define DMA_CFG_TRIGTYPE_EDGE   (0 << 5)	/*!< Hardware trigger is edge triggered */
#define DMA_CFG_TRIGTYPE_LEVEL  (1 << 5)	/*!< Hardware trigger is level triggered */
#define DMA_CFG_TRIGBURST_SNGL  (0 << 6)	/*!< Single transfer. Hardware trigger causes a single transfer */
#define DMA_CFG_TRIGBURST_BURST (1 << 6)	/*!< Burst transfer (see UM) */
#define DMA_CFG_BURSTPOWER_1    (0 << 8)	/*!< Set DMA burst size to 1 transfer */
#define DMA_CFG_BURSTPOWER_2    (1 << 8)	/*!< Set DMA burst size to 2 transfers */
#define DMA_CFG_BURSTPOWER_4    (2 << 8)	/*!< Set DMA burst size to 4 transfers */
#define DMA_CFG_BURSTPOWER_8    (3 << 8)	/*!< Set DMA burst size to 8 transfers */
#define DMA_CFG_BURSTPOWER_16   (4 << 8)	/*!< Set DMA burst size to 16 transfers */
#define DMA_CFG_BURSTPOWER_32   (5 << 8)	/*!< Set DMA burst size to 32 transfers */
#define DMA_CFG_BURSTPOWER_64   (6 << 8)	/*!< Set DMA burst size to 64 transfers */
#define DMA_CFG_BURSTPOWER_128  (7 << 8)	/*!< Set DMA burst size to 128 transfers */
#define DMA_CFG_BURSTPOWER_256  (8 << 8)	/*!< Set DMA burst size to 256 transfers */
#define DMA_CFG_BURSTPOWER_512  (9 << 8)	/*!< Set DMA burst size to 512 transfers */
#define DMA_CFG_BURSTPOWER_1024 (10 << 8)	/*!< Set DMA burst size to 1024 transfers */
#define DMA_CFG_BURSTPOWER(n)   ((n) << 8)	/*!< Set DMA burst size to 2^n transfers, max n=10 */
#define DMA_CFG_SRCBURSTWRAP    (1 << 14)	/*!< Source burst wrapping is enabled for this DMA channel */
#define DMA_CFG_DSTBURSTWRAP    (1 << 15)	/*!< Destination burst wrapping is enabled for this DMA channel */
#define DMA_CFG_CHPRIORITY(p)   ((p) << 16)	/*!< Sets DMA channel priority, min 0 (highest), max 3 (lowest) */

/* DMA channel transfer configuration registers definitions */
#define DMA_XFERCFG_MASK            (0x0000F33F)
#define DMA_XFERCFG_CFGVALID        (1 << 0)	/*!< Configuration Valid flag */
#define DMA_XFERCFG_RELOAD          (1 << 1)	/*!< Indicates whether the channels control structure will be reloaded when the current descriptor is exhausted */
#define DMA_XFERCFG_SWTRIG          (1 << 2)	/*!< Software Trigger */
#define DMA_XFERCFG_CLRTRIG         (1 << 3)	/*!< Clear Trigger */
#define DMA_XFERCFG_SETINTA         (1 << 4)	/*!< Set Interrupt flag A for this channel to fire when descriptor is complete */
#define DMA_XFERCFG_SETINTB         (1 << 5)	/*!< Set Interrupt flag B for this channel to fire when descriptor is complete */
#define DMA_XFERCFG_WIDTH_8         (0 << 8)	/*!< 8-bit transfers are performed */
#define DMA_XFERCFG_WIDTH_16        (1 << 8)	/*!< 16-bit transfers are performed */
#define DMA_XFERCFG_WIDTH_32        (2 << 8)	/*!< 32-bit transfers are performed */
#define DMA_XFERCFG_SRCINC_0        (0 << 12)	/*!< DMA source address is not incremented after a transfer */
#define DMA_XFERCFG_SRCINC_1        (1 << 12)	/*!< DMA source address is incremented by 1 (width) after a transfer */
#define DMA_XFERCFG_SRCINC_2        (2 << 12)	/*!< DMA source address is incremented by 2 (width) after a transfer */
#define DMA_XFERCFG_SRCINC_4        (3 << 12)	/*!< DMA source address is incremented by 4 (width) after a transfer */
#define DMA_XFERCFG_DSTINC_0        (0 << 14)	/*!< DMA destination address is not incremented after a transfer */
#define DMA_XFERCFG_DSTINC_1        (1 << 14)	/*!< DMA destination address is incremented by 1 (width) after a transfer */
#define DMA_XFERCFG_DSTINC_2        (2 << 14)	/*!< DMA destination address is incremented by 2 (width) after a transfer */
#define DMA_XFERCFG_DSTINC_4        (3 << 14)	/*!< DMA destination address is incremented by 4 (width) after a transfer */
#define DMA_XFERCFG_XFERCOUNT(n)    ((n - 1) << 16)	/*!< DMA transfer count in 'transfers', between (0)1 and (1023)1024 */
#define DMA_XFERCFGCNT_MASK         (0x03FF0000)

#define DMA_XFERCFG_SWTRIGGER       (DMA_XFERCFG_CFGVALID|DMA_XFERCFG_SWTRIG)
/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief DMA Controller shared registers structure
 */
typedef struct {			    /*!< DMA shared registers structure */
	__IO U32  ENABLESET;		/*!< DMA Channel Enable read and Set for all DMA channels */
	__I  U32  RESERVED0;
	__O  U32  ENABLECLR;		/*!< DMA Channel Enable Clear for all DMA channels */
	__I  U32  RESERVED1;
	__I  U32  ACTIVE;			/*!< DMA Channel Active status for all DMA channels */
	__I  U32  RESERVED2;
	__I  U32  BUSY;			    /*!< DMA Channel Busy status for all DMA channels */
	__I  U32  RESERVED3;
	__IO U32  ERRINT;			/*!< DMA Error Interrupt status for all DMA channels */
	__I  U32  RESERVED4;
	__IO U32  INTENSET;		    /*!< DMA Interrupt Enable read and Set for all DMA channels */
	__I  U32  RESERVED5;
	__O  U32  INTENCLR;		    /*!< DMA Interrupt Enable Clear for all DMA channels */
	__I  U32  RESERVED6;
	__IO U32  INTA;			    /*!< DMA Interrupt A status for all DMA channels */
	__I  U32  RESERVED7;
	__IO U32  INTB;			    /*!< DMA Interrupt B status for all DMA channels */
	__I  U32  RESERVED8;
	__O  U32  SETVALID;		    /*!< DMA Set ValidPending control bits for all DMA channels */
	__I  U32  RESERVED9;
	__O  U32  SETTRIG;			/*!< DMA Set Trigger control bits for all DMA channels */
	__I  U32  RESERVED10;
	__O  U32  ABORT;			/*!< DMA Channel Abort control for all DMA channels */
} LPC_DMA_COMMON_T;

/**
 * @brief DMA Controller shared registers structure
 */
typedef struct {			    /*!< DMA channel register structure */
	__IO U32  CFG;				/*!< DMA Configuration register */
	__I  U32  CTLSTAT;			/*!< DMA Control and status register */
	__IO U32  XFERCFG;			/*!< DMA Transfer configuration register */
	__I  U32  RESERVED;
} LPC_DMA_CHANNEL_T;

/**
 * @brief DMA Controller Channel Priority
 */
typedef enum {
    DMA_CHPRIORITY_0       = 0,
	DMA_CHPRIORITY_HIGHEST = DMA_CHPRIORITY_0,
	DMA_CHPRIORITY_1       = 1,
	DMA_CHPRIORITY_2       = 2,
	DMA_CHPRIORITY_3       = 3,
    DMA_CHPRIORITY_LOWEST  = DMA_CHPRIORITY_3,	
}LPC_DMA_CHPRIORITY_T;

/* DMA channel mapping - each channel is mapped to an individual peripheral
   and direction or a DMA imput mux trigger */
typedef enum {
    #if(ENABLE == CFG_DMA0_ENABLE)
	DMAREQ_USART0_RX = 0,					    /*!< USART0 receive DMA channel */
	DMA0 = DMAREQ_USART0_RX,
	#endif
    #if(ENABLE == CFG_DMA1_ENABLE)	
	DMAREQ_USART0_TX = 1,						/*!< USART0 transmit DMA channel */
	DMA1 = DMAREQ_USART0_TX,
	#endif
    #if(ENABLE == CFG_DMA2_ENABLE)	
	DMAREQ_USART1_RX = 2,						/*!< USART1 receive DMA channel */
	DMA2 = DMAREQ_USART1_RX,
    #endif	
    #if(ENABLE == CFG_DMA3_ENABLE)		
	DMAREQ_USART1_TX = 3,						/*!< USART1 transmit DMA channel */
	DMA3 = DMAREQ_USART1_TX,
    #endif
    #if(ENABLE == CFG_DMA4_ENABLE)	
	DMAREQ_USART2_RX = 4,						/*!< USART2 receive DMA channel */
	DMA4 = DMAREQ_USART2_RX,
    #endif	
    #if(ENABLE == CFG_DMA5_ENABLE)
	DMAREQ_USART2_TX = 5,						/*!< USART2 transmit DMA channel */
	DMA5 = DMAREQ_USART2_TX,
    #endif	
    #if(ENABLE == CFG_DMA6_ENABLE)	
	DMAREQ_SPI0_RX   = 6,				        /*!< SSP0 receive DMA channel */
	DMA6 = DMAREQ_SPI0_RX,
    #endif	
    #if(ENABLE == CFG_DMA7_ENABLE)
	DMAREQ_SPI0_TX   = 7,					    /*!< SSP0 transmit DMA channel */
	DMA7 = DMAREQ_SPI0_TX,
    #endif	
    #if(ENABLE == CFG_DMA8_ENABLE)
	DMAREQ_SPI1_RX   = 8,					    /*!< SSP1 receive DMA channel */
	DMA8 = DMAREQ_SPI1_RX,
    #endif	
    #if(ENABLE == CFG_DMA9_ENABLE)	
	DMAREQ_SPI1_TX   = 9,					    /*!< SSP1 transmit DMA channel */
	DMA9 = DMAREQ_SPI1_TX,
    #endif	
    #if(ENABLE == CFG_DMA10_ENABLE)	
	DMAREQ_I2C0_SLV  = 10,						/*!< I2C0 slave DMA channel */
	DMA10 = DMAREQ_I2C0_SLV,
    #endif	
    #if(ENABLE == CFG_DMA11_ENABLE)		
	DMAREQ_I2C0_MST  = 11,						/*!< I2C0 master DMA channel */
	DMA11 = DMAREQ_I2C0_MST,
    #endif	
    #if(ENABLE == CFG_DMA12_ENABLE)	
	DMAREQ_I2C0_MONITOR = 12,					/*!< I2C0 monitor DMA channel */
	DMA12 = DMAREQ_I2C0_MONITOR,
    #endif	
    #if(ENABLE == CFG_DMA13_ENABLE)		
	DMAREQ_DAC_IRQ      = 13,				    /*!< DAC DMA channel */
	DMA13 = DMAREQ_DAC_IRQ,
    #endif	
    #if(ENABLE == CFG_DMA14_ENABLE)	
	DMAREQ_RESERVED_14  = 14,
	DMA14 = DMAREQ_RESERVED_14,
    #endif	
    #if(ENABLE == CFG_DMA15_ENABLE)		
	DMAREQ_RESERVED_15  = 15,
	DMA15 = DMAREQ_RESERVED_15,
    #endif	
    #if(ENABLE == CFG_DMA16_ENABLE)	
	DMAREQ_RESERVED_16  = 16,
	DMA16 = DMAREQ_RESERVED_16,
    #endif	
    #if(ENABLE == CFG_DMA17_ENABLE)		
	DMAREQ_RESERVED_17  = 17,
	DMA17 = DMAREQ_RESERVED_17
    #endif	
} DMA_CHID_T;

/**
 * @brief DMA Controller register block structure
 */
typedef struct {		                        /*!< DMA Structure */
	__IO U32  CTRL;			                    /*!< DMA control register */
	__I  U32  INTSTAT;		                    /*!< DMA Interrupt status register */
	__IO U32  SRAMBASE;		                    /*!< DMA SRAM address of the channel configuration table */
	__I  U32  RESERVED2[5];
	LPC_DMA_COMMON_T DMACOMMON[1];           	/*!< DMA shared channel (common) registers */
	__I  U32  RESERVED0[225];
	LPC_DMA_CHANNEL_T DMACH[MAX_DMA_CHANNEL];	/*!< DMA channel registers */
} LPC_DMA_T;

/* DMA channel source/address/next descriptor */
typedef struct {
	U32  xfercfg;		/*!< Transfer configuration (only used in linked lists and ping-pong configs) */
	U32  source;		/*!< DMA transfer source end address */
	U32  dest;			/*!< DMA transfer desintation end address */
	U32  next;			/*!< Link to next DMA descriptor, must be 16 byte aligned */
} DMA_CHDESC_T;

/**
 * @brief IO DMA Setup structure
 */
typedef struct {
    DMA_CHID_T       ch;	        /* DMA ch number */
    DMA_TRIGSRC_T    trigsrc;       /* DMA ch trigger source */	
	U32              chcfg;         /* DMA ch cofig */
	bool             chinten;       /* Enable DMA ch interrupt - INTA, INTB and ERRINT */
}DMA_SETUP_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize DMA controller
 * @param	Nothing	
 * @return	Nothing
 */
void DMA_vInit(void);

/**
 * @brief	Reset and Initialize DMA controller
 * @param	Nothing	
 * @return	Nothing
 */
void DMA_vReset(void);

/**
 * @brief	Shutdown DMA controller
 * @param	Nothing	
 * @return	Nothing
 */
STATIC INLINE void DMA_vShutdown(void)
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_DMA);
}

/**
 * @brief	Start a new DMA channel transfer
 * @param	teCH		    : DMA channel ID
 * @param	unXfercfg		: DMA channel transfer config
 * @param	pDesc		: DMA channel descriptor list
 * @return	bool        : true - success to start a DMA channel and its transfer
 */
bool DMA_bStart(DMA_CHID_T      teCH,   
                U32             unXfercfg,
                DMA_CHDESC_T    *pDesc); 

/**
 * @brief	Addtioanlly start a DMA channel if XFERCFG_SWTRIG is not set when software trigger mode
 * @param	teCH		    : DMA channel ID
 * @param	unXfercfg		: DMA channel transfer config
 * @param	pDesc		: DMA channel descriptor list
 * @return	bool        : true - success to start a DMA channel and its transfer
 */
STATIC INLINE void DMA_bSoftwareTrigger(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACH[teCH].XFERCFG, 
		          DMA_XFERCFG_MASK&DMA_XFERCFG_SWTRIGGER, 
		          DMA_XFERCFG_SWTRIGGER);
}

/**
 * @brief	Pause a DMA channel
 * @param	teCH		    : DMA channel ID
 * @return	Nothing
 * @note    Call DMA_vResumeChannel to restart a channel, which is paused before
 */
void DMA_vPause(DMA_CHID_T teCH);

/**
 * @brief	Resume a DMA channel
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 */
void DMA_vResume(DMA_CHID_T teCH);

/**
 * @brief	Returns channel specific status flags
 * @param	teCH		: DMA channel ID
 * @param	unStatflag		: State flag
 * @return	AN Or'ed value of DMA_CTLSTAT_VALIDPENDING and DMA_CTLSTAT_TRIG
 */
STATIC INLINE bool DMA_bGetStatus(DMA_CHID_T teCH, U32 unStatflag)
{
	return (GET_REG_FIELD(LPC_DMA->DMACH[teCH].CTLSTAT, unStatflag) != 0);
}

#ifdef __cplusplus
}
#endif

#endif /* CFG_DMA_USED */

/**
 * @}
 */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

