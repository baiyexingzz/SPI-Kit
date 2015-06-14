/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SSI.c                                                    $*/
/*                                                                            */
/*    $Revision:: 343                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-25 10:27:38 +0800 (周二, 25 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description:   Low-level macros, definition, functions - SPI0/1          */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "interrupt.h"
#include "io.h"

#include "ssi.h"

#ifdef CFG_SPI_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/** Pointer to User-Specific SPI receiving indicate callback  */
#define DECLARE_EVENT_CALLBACK(_Func_)  extern void _Func_(void);

#define SPI_RXIGNORE_DEFAULT (0<<22)
//loopback 0 is disable, 1 is enable
#define SPI_LOOPBACK_ENABLE  (0<<7)
#define SPI_MASTER_PREDELAY 0x4
#define SPI_MASTER_POSTDELAY 0x4
#define SPI_MASTER_FRAMEDELAY 0x4
#define SPI_MASTER_XFERDELAY 0x4



/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
#ifdef SPI0_EVENT_CB
DECLARE_EVENT_CALLBACK(SPI0_EVENT_CB);
#endif

#ifdef SPI1_EVENT_CB
DECLARE_EVENT_CALLBACK(SPI1_EVENT_CB);
#endif

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************//* Reg map of SPI0/1 */
/* SPI config settings */
#if (ENABLE == CFG_SPI0_ENABLE)
SPI_CONFIG_T  SPICfg0 = {SPI0_CFG_MODE, SPI0_CFG_CLOCK, SPI0_CFG_DORDER, SPI0_CFG_SSLPOL, SPI0_CFG_CLOCK_DIV};
#endif
#if (ENABLE == CFG_SPI1_ENABLE)
SPI_CONFIG_T  SPICfg1 = {SPI1_CFG_MODE, SPI1_CFG_CLOCK, SPI1_CFG_DORDER, SPI1_CFG_CLOCK_DIV};;
#endif

/* SPI Savle SSel */
U32 SPISsel[MAX_SPI_NUMBER] = 
{
    #if (ENABLE == CFG_SPI0_ENABLE)
	SPI0_SSEL_CFG,
    #else
	SPI_SSEL_DEFAULT,
    #endif
/*	#if (ENABLE == CFG_SPI1_ENABLE)
	SPI1_SSEL_CFG,
    #else
	SPI_SSEL_DEFAULT,
    #endif*/
};

/* SPI Frame Bit Length */
U8 SPIFrameSize[MAX_SPI_NUMBER] = 
{
    #if (ENABLE == CFG_SPI0_ENABLE)
	SPI0_FRAME_SIZE,
    #else
	SPI_FRAME_SIZE_DEFAULT,
    #endif
/*	#if (ENABLE == CFG_SPI1_ENABLE)
	SPI1_FRAME_SIZE,
    #else
	SPI_FRAME_SIZE_DEFAULT,
    #endif*/
};

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief   Set the SPI Config register
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @param	pConfig   : SPI Configuration
 * @return	Nothing
 */
void SPI_vConfig(SPI_GROUP_T teSPI, SPI_CONFIG_T *pConfig);

/**
 * @brief   Set the SPI delay register
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @param	pConfig   : SPI Configuration
 * @return	Nothing
 */
STATIC INLINE void SPI_vDelay(SPI_GROUP_T teSPI, U32 nPreDelay, U32 nPostDelay, U32 nFrameDelay, U32 nXferDelay)
{
	U32 regdata =  nPreDelay|(nPostDelay<<4)|(nFrameDelay<<8)|(nXferDelay<<12);
    WRITE_REG(SPI_PTR[teSPI]->DLY, regdata);
}

/**
 * @brief	Enable a SPI peripheral
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	Nothing
 */
STATIC INLINE void SPI_vEnable(SPI_GROUP_T teSPI)
{
	/* Enable SPI */
	SET_REG_FIELD(SPI_PTR[teSPI]->CFG, SPI_CFG_SPI_EN, SPI_CFG_SPI_EN);
}

/**
 * @brief	Disable a SPI peripheral
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @return	Nothing
 */
STATIC INLINE void SPI_vDisable(SPI_GROUP_T teSPI)
{
	/* Enable SPI */
	SET_REG_FIELD(SPI_PTR[teSPI]->CFG, SPI_CFG_SPI_EN, 0);
}

/**
 * @brief Enable SPI interrupt
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @param	unIntmask		: Interrupt mask, for example SPI_INTENSET_RXRDYEN
 * @return	Nothing
 */
STATIC INLINE void SPI_vEnableInt(SPI_GROUP_T teSPI, U32 nIntmask)
{
	SET_REG_FIELD(SPI_PTR[teSPI]->INTENSET, (nIntmask&SPI_INTENSET_BITMASK), nIntmask);
}

#ifndef SPI_IN_USED
/**
 * @brief Disable SPI interrupt
 * @param	teSPI	: spi number - SPI_0/SPI_1
 * @param	unIntmask		: Interrupt clear mask, for example SPI_INTENCLR_RXRDYEN
 * @return	Nothing
 */
STATIC INLINE void SPI_vDisableInt(SPI_GROUP_T teSPI, U32 nIntmask)
{
	SET_REG_FIELD(SPI_PTR[teSPI]->INTENCLR, (nIntmask&SPI_INTENCLR_BITMASK), nIntmask);
}
#endif

/**
 * @brief	Write a frame of the transfer with control info
 * @param	teSPI	    : spi number - SPI_0/SPI_1
 * @param	unFrame	: Transmit frame data
 * @param	ubSize	: Transmit frame data size 
 * @param	unSSel	: ssel index
 * @return	Nothing
 */
STATIC INLINE void SPI_vSendFrame(SPI_GROUP_T teSPI, U16 nFrameContent, U8 ubSize, U32 nSSel)
{
    /* Set TX data and ctrl */
	WRITE_REG(SPI_PTR[teSPI]->TXDATCTL, SPI_TXDATCTL_DATA(nFrameContent)|
	                                    (SPI_RXIGNORE_DEFAULT)|
		                                (nSSel & SPI_TXDATCTL_SSEL_MASK) | 
		                                (SPI_TXDATCTL_EOF) | (SPI_TXDATCTL_EOT) |
		                                (SPI_RXIGNORE_DEFAULT) |
		                                (SPI_TXDATCTL_LEN(ubSize - 1)));
}
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initializes the SPI peripheral */
void SPI_vInit(void)
{	
	/**
     *    Config SPI using definitions in spi_cfg.h
	 **/	
    #if (ENABLE == CFG_SPI0_ENABLE)
		CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SPI0);
		SYSCTL_vPeriphReset(RESET_SPI0);
		
		

		SPI_vConfig(SPI0, &SPICfg0);

		WRITE_REG(SPI_PTR[SPI0]->STAT, SPI_PTR[SPI0]->STAT);

		#ifdef SPI0_INEN_CFG
       	SPI_vEnableInt(SPI0, SPI0_INEN_CFG);
	    #else
        SPI_vDisableInt(SPI0, SPI_INTENCLR_BITMASK);
		#endif
	#endif

    #if (ENABLE == CFG_SPI1_ENABLE)
		CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SPI1);
		SYSCTL_vPeriphReset(RESET_SPI1);
		
	    SPI_vConfig(SPI1 ,&SPICfg1);

		WRITE_REG(SPI_PTR[SPI1]->STAT, SPI_PTR[SPI1]->STAT);
		
		#ifdef SPI1_INEN_CFG
       	SPI_vEnableInt(SPI1, SPI1_INEN_CFG);
	    #else
        SPI_vDisableInt(SPI1, SPI_INTENCLR_BITMASK);
		#endif
    #endif
}

/* Shutdown SPI peripheral */
void SPI_vShutdown(void)
{
    #if (ENABLE == CFG_SPI0_ENABLE)
	SPI_vStop(SPI0);
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_SPI0);
	#endif

    #if (ENABLE == CFG_SPI1_ENABLE)
	SPI_vStop(SPI1);
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_SPI1);
	#endif	
}

/* Reset the SPI peripheral */
void SPI_vReset(void)
{    
	SPI_vShutdown();
    SPI_vInit();
}

/* Start a SPI */
void SPI_vStart(SPI_GROUP_T teSPI)
{	
	SPI_vClearStatus(teSPI, SPI_nGetStatus(teSPI));	 

	/* Enable SPI */
	SPI_vEnable(teSPI);
}

/* Stop a SPI */
void SPI_vStop(SPI_GROUP_T teSPI)
{
	/* Disable SPI and interrupt */
	SPI_vDisable(teSPI);
}

/* Send a frame data, which length is defined in spi_cfg */
void SPI_vSendData(SPI_GROUP_T teSPI, U16 nData)
{	
	//SPI_vEnableInt(SPI0,SPI0_INEN_CFG);
	SPI_vSendFrame(teSPI, 
                   (SPI_RXIGNORE_DEFAULT)|SPI_TXDAT_DATA(nData), 
                   SPIFrameSize[teSPI], 
                   SPISsel[teSPI]);
}

/* Write a frame data, which length and ctrls are defined in spi_cfg */
void SPI_vSendDataBlocking(SPI_GROUP_T teSPI, U16 nData)
{   
	
	/* Send a data frame */	
	while(!SPI_bGetStatus(teSPI, SPI_STAT_TXRDY)){;}
	
	SPI_vSendFrame(teSPI, 
                   (SPI_RXIGNORE_DEFAULT)|SPI_TXDAT_DATA(nData), 
                   SPIFrameSize[teSPI], 
                   SPISsel[teSPI]);

	if(SPI_STAT_TXUR&SPI_nGetStatus(teSPI)){
		nData = nData;
	}
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/* Setup config of the SPI peripheral */
void SPI_vConfig(SPI_GROUP_T teSPI, SPI_CONFIG_T *pConfig)
{
	/* Disable SPI before any configurations */
    SPI_vDisable(teSPI);
		
	/* SPI Configure */
	WRITE_REG(SPI_PTR[teSPI]->CFG, (((U32) pConfig->ClockMode) | 
	                                ((U32) pConfig->DataOrder) | 
	                                ((U32) pConfig->Mode)      |
				                    ((U32) pConfig->SSELPol) |
				                    ((U32)SPI_LOOPBACK_ENABLE)));
	
	

	/* Clock Divider for SPI */
	WRITE_REG(SPI_PTR[teSPI]->DIV, SPI_DIV_VAL(pConfig->ClkDiv-1));

	/* Set default frame length in TXCTRL  */
	SET_REG_FIELD(SPI_PTR[teSPI]->TXCTRL, SPI_TXCTL_MASK, SPI_TXCTL_LEN(SPIFrameSize[teSPI]-1)); 

	if(SPI_MODE_MASTER == pConfig->Mode){
		SPI_vDelay(teSPI, SPI_MASTER_PREDELAY,
			              SPI_MASTER_POSTDELAY,
			              SPI_MASTER_FRAMEDELAY,
			              SPI_MASTER_XFERDELAY);\

	    SET_REG_FIELD(SPI_PTR[teSPI]->TXCTRL, SPI_TXCTL_ASSERT_MASK, SPISsel[teSPI]);
	}
}

#endif /* CFG_SPI_USED */
/******************************************************************************/
/*                      ISR HANDLER FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for SPI0/1 
 **/
void SPI0_vISR(void)
{	
  /*  #ifdef SPI0_EVENT_CB
	SPI0_EVENT_CB();	
	#endif  */
  SPI_vDisableInt(SPI0, SPI_INTENCLR_BITMASK);
}

void SPI1_vISR(void)
{	
    #ifdef SPI1_EVENT_CB
	SPI1_EVENT_CB();	
	#endif

}
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

