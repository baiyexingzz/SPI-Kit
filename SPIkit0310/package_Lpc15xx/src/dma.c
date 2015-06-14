/******************************************************************************/
/*                                                                            */
/*    $Workfile::   DMA.c                                                 $   */
/*                                                                            */
/*    $Revision:: 360                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-26 15:47:50 +0800 (周三, 26 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - DMA controller  */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "dma.h"

#ifdef CFG_DMA_USED
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/ 
/**
 * @brief IO pin setup line
 */
#define DMA_SETUP_LINE(p) {p, p##_TRIG, p##_CFG, (p##_INT_EN)|(p##_ERRINT_EN)}

/**
 * @brief Declare external dma callback
 */
#define DECLARE_DMA_CALLBACK(_Func_)  extern void _Func_(void)
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/**
 * @brief DMA setup table for LPC1517
 */
STATIC const DMA_SETUP_T DMASetup[] = {
    #if(ENABLE == CFG_DMA0_ENABLE)	
	DMA_SETUP_LINE(DMA0),
    #endif
    #if(ENABLE == CFG_DMA1_ENABLE)
	DMA_SETUP_LINE(DMA1),
    #endif	
    #if(ENABLE == CFG_DMA2_ENABLE)
	DMA_SETUP_LINE(DMA2),
    #endif	
    #if(ENABLE == CFG_DMA3_ENABLE)
	DMA_SETUP_LINE(DMA3),
    #endif	
    #if(ENABLE == CFG_DMA4_ENABLE)
	DMA_SETUP_LINE(DMA4),
    #endif	
    #if(ENABLE == CFG_DMA5_ENABLE)
	DMA_SETUP_LINE(DMA5),
    #endif		
    #if(ENABLE == CFG_DMA6_ENABLE)
	DMA_SETUP_LINE(DMA6),
    #endif	
    #if(ENABLE == CFG_DMA7_ENABLE)
	DMA_SETUP_LINE(DMA7),
    #endif		
    #if(ENABLE == CFG_DMA8_ENABLE)
	DMA_SETUP_LINE(DMA8),
    #endif	
    #if(ENABLE == CFG_DMA9_ENABLE)
	DMA_SETUP_LINE(DMA9),
    #endif	
    #if(ENABLE == CFG_DMA10_ENABLE)
	DMA_SETUP_LINE(DMA10),
    #endif	
    #if(ENABLE == CFG_DMA11_ENABLE)
	DMA_SETUP_LINE(DMA11),
    #endif		
    #if(ENABLE == CFG_DMA12_ENABLE)
	DMA_SETUP_LINE(DMA12),
    #endif	
    #if(ENABLE == CFG_DMA13_ENABLE)
	DMA_SETUP_LINE(DMA13),
    #endif	
	#if(ENABLE == CFG_DMA14_ENABLE)
	DMA_SETUP_LINE(DMA14),
    #endif
	#if(ENABLE == CFG_DMA15_ENABLE)
	DMA_SETUP_LINE(DMA15),
    #endif	
	#if(ENABLE == CFG_DMA16_ENABLE)
	DMA_SETUP_LINE(DMA16),
    #endif	
	#if(ENABLE == CFG_DMA17_ENABLE)
	DMA_SETUP_LINE(DMA17),
    #endif		
};

/**
 * @brief DMA callback table for LPC1517 DMA
 */
#ifdef CFG_INTA_USED
#ifdef DMA0_INTA_CB
DECLARE_DMA_CALLBACK(DMA0_INTA_CB);
#endif
#ifdef DMA1_INTA_CB
DECLARE_DMA_CALLBACK(DMA1_INTA_CB);
#endif
#ifdef DMA2_INTA_CB
DECLARE_DMA_CALLBACK(DMA2_INTA_CB);
#endif
#ifdef DMA3_INTA_CB
DECLARE_DMA_CALLBACK(DMA3_INTA_CB);
#endif
#ifdef DMA4_INTA_CB
DECLARE_DMA_CALLBACK(DMA4_INTA_CB);
#endif
#ifdef DMA5_INTA_CB
DECLARE_DMA_CALLBACK(DMA5_INTA_CB);
#endif
#ifdef DMA6_INTA_CB
DECLARE_DMA_CALLBACK(DMA6_INTA_CB);
#endif
#ifdef DMA7_INTA_CB
DECLARE_DMA_CALLBACK(DMA7_INTA_CB);
#endif
#ifdef DMA8_INTA_CB
DECLARE_DMA_CALLBACK(DMA8_INTA_CB);
#endif
#ifdef DMA9_INTA_CB
DECLARE_DMA_CALLBACK(DMA9_INTA_CB);
#endif
#ifdef DMA10_INTA_CB
DECLARE_DMA_CALLBACK(DMA10_INTA_CB);
#endif
#ifdef DMA11_INTA_CB
DECLARE_DMA_CALLBACK(DMA11_INTA_CB);
#endif
#ifdef DMA12_INTA_CB
DECLARE_DMA_CALLBACK(DMA12_INTA_CB);
#endif
#ifdef DMA13_INTA_CB
DECLARE_DMA_CALLBACK(DMA13_INTA_CB);
#endif
#ifdef DMA14_INTA_CB
DECLARE_DMA_CALLBACK(DMA14_INTA_CB);
#endif
#ifdef DMA15_INTA_CB
DECLARE_DMA_CALLBACK(DMA15_INTA_CB);
#endif
#ifdef DMA16_INTA_CB
DECLARE_DMA_CALLBACK(DMA16_INTA_CB);
#endif
#ifdef DMA17_INTA_CB
DECLARE_DMA_CALLBACK(DMA17_INTA_CB);
#endif
#endif

#ifdef CFG_INTB_USED
#ifdef DMA0_INTB_CB
DECLARE_DMA_CALLBACK(DMA0_INTB_CB);
#endif
#ifdef DMA1_INTB_CB
DECLARE_DMA_CALLBACK(DMA1_INTB_CB);
#endif
#ifdef DMA2_INTB_CB
DECLARE_DMA_CALLBACK(DMA2_INTB_CB);
#endif
#ifdef DMA3_INTB_CB
DECLARE_DMA_CALLBACK(DMA3_INTB_CB);
#endif
#ifdef DMA4_INTB_CB
DECLARE_DMA_CALLBACK(DMA4_INTB_CB);
#endif
#ifdef DMA5_INTB_CB
DECLARE_DMA_CALLBACK(DMA5_INTB_CB);
#endif
#ifdef DMA6_INTB_CB
DECLARE_DMA_CALLBACK(DMA6_INTB_CB);
#endif
#ifdef DMA7_INTB_CB
DECLARE_DMA_CALLBACK(DMA7_INTB_CB);
#endif
#ifdef DMA8_INTB_CB
DECLARE_DMA_CALLBACK(DMA8_INTB_CB);
#endif
#ifdef DMA9_INTB_CB
DECLARE_DMA_CALLBACK(DMA9_INTB_CB);
#endif
#ifdef DMA10_INTB_CB
DECLARE_DMA_CALLBACK(DMA10_INTB_CB);
#endif
#ifdef DMA11_INTB_CB
DECLARE_DMA_CALLBACK(DMA11_INTB_CB);
#endif
#ifdef DMA12_INTB_CB
DECLARE_DMA_CALLBACK(DMA12_INTB_CB);
#endif
#ifdef DMA13_INTB_CB
DECLARE_DMA_CALLBACK(DMA13_INTB_CB);
#endif
#ifdef DMA14_INTB_CB
DECLARE_DMA_CALLBACK(DMA14_INTB_CB);
#endif
#ifdef DMA15_INTB_CB
DECLARE_DMA_CALLBACK(DMA15_INTB_CB);
#endif
#ifdef DMA16_INTB_CB
DECLARE_DMA_CALLBACK(DMA16_INTB_CB);
#endif
#ifdef DMA17_INTB_CB
DECLARE_DMA_CALLBACK(DMA17_INTB_CB);
#endif
#endif

#ifdef CFG_INTERR_USED
#ifdef DMA0_INTERR_CB
DECLARE_DMA_CALLBACK(DMA0_INTERR_CB);
#endif
#ifdef DMA1_INTERR_CB
DECLARE_DMA_CALLBACK(DMA1_INTERR_CB);
#endif
#ifdef DMA2_INTERR_CB
DECLARE_DMA_CALLBACK(DMA2_INTERR_CB);
#endif
#ifdef DMA3_INTERR_CB
DECLARE_DMA_CALLBACK(DMA3_INTERR_CB);
#endif
#ifdef DMA4_INTERR_CB
DECLARE_DMA_CALLBACK(DMA4_INTERR_CB);
#endif
#ifdef DMA5_INTERR_CB
DECLARE_DMA_CALLBACK(DMA5_INTERR_CB);
#endif
#ifdef DMA6_INTERR_CB
DECLARE_DMA_CALLBACK(DMA6_INTERR_CB);
#endif
#ifdef DMA7_INTERR_CB
DECLARE_DMA_CALLBACK(DMA7_INTERR_CB);
#endif
#ifdef DMA8_INTERR_CB
DECLARE_DMA_CALLBACK(DMA8_INTERR_CB);
#endif
#ifdef DMA9_INTERR_CB
DECLARE_DMA_CALLBACK(DMA9_INTERR_CB);
#endif
#ifdef DMA10_INTERR_CB
DECLARE_DMA_CALLBACK(DMA10_INTERR_CB);
#endif
#ifdef DMA11_INTERR_CB
DECLARE_DMA_CALLBACK(DMA11_INTERR_CB);
#endif
#ifdef DMA12_INTERR_CB
DECLARE_DMA_CALLBACK(DMA12_INTERR_CB);
#endif
#ifdef DMA13_INTERR_CB
DECLARE_DMA_CALLBACK(DMA13_INTERR_CB);
#endif
#ifdef DMA14_INTERR_CB
DECLARE_DMA_CALLBACK(DMA14_INTERR_CB);
#endif
#ifdef DMA15_INTERR_CB
DECLARE_DMA_CALLBACK(DMA15_INTERR_CB);
#endif
#ifdef DMA16_INTERR_CB
DECLARE_DMA_CALLBACK(DMA16_INTERR_CB);
#endif
#ifdef DMA17_INTERR_CB
DECLARE_DMA_CALLBACK(DMA17_INTERR_CB);
#endif
#endif
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
/* DMA SRAM table - this can be optionally used with the DMA_SetSRAMBase()
   function if a DMA SRAM table is needed. */
#if defined(__CC_ARM)
/* Keil alignement to 512 bytes */
__align(512) DMA_CHDESC_T LPC_DMA_Table[MAX_DMA_CHANNEL];
#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
/* IAR EWARM alignement to 512 bytes */
#pragma data_alignment=512
DMA_CHDESC_T LPC_DMA_Table[MAX_DMA_CHANNEL];
#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
/* GNU alignement to 512 bytes */
DMA_CHDESC_T LPC_DMA_Table[MAX_DMA_CHANNEL] __attribute__ ((aligned(512)));
#endif /* defined (__GNUC__) */

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Enable DMA controller
 * @param	Nothing	
 * @return	Nothing
 */
STATIC INLINE void DMA_vEnable(void)
{
	SET_REG_FIELD(LPC_DMA->CTRL, DMA_CTRL_EN_MASK, DMA_CTRL_EN);
}

/**
 * @brief	Set DMA controller SRAM base address
 * @param	unBase	: The base address where the DMA descriptors will be stored
 * @return	Nothing
 * @note	A 288 byte block of memory aligned on a 512 byte boundary must be
 *			provided for this function. It sets the base address used for
 *			DMA descriptor table (18 descriptors total that use 16 bytes each).<br>
 *
 *			A pre-defined table with correct alignment can be used for this
 *			function by calling DMA_SetSRAMBase(LPC_DMA, DMA_ADDR(Chip_DMA_Table));
 */
STATIC INLINE void DMA_vSetSRAMBase(U32 unBase)
{
	WRITE_REG(LPC_DMA->SRAMBASE, unBase);
}

/**
 * @brief	Enables setting and reading of a single DMA channel
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 */
STATIC INLINE void DMA_vEnableChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].ENABLESET, BIT(teCH), BIT(teCH));
}

/**
 * @brief	Disables setting and reading of a single DMA channel
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 */
STATIC INLINE void DMA_vDisableChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].ENABLECLR, BIT(teCH), BIT(teCH));
}

/**
 * @brief	Setup a DMA channel configuration
 * @param	teCH		: DMA channel ID
 * @param	cfg		: An Or'ed value of DMA_CFG_* values that define the channel's configuration
 * @return	Nothing
 * @note	This function sets up all configurable options for the DMA channel.
 *			These options are usually set once for a channel and then unchanged.<br>
 *
 *			The following example show how to configure the channel for peripheral
 *			DMA requests, burst transfer size of 1 (in 'transfers', not bytes),
 *			continuous reading of the same source address, incrementing destination
 *			address, and highest channel priority.<br>
 *			Example: Chip_DMA_SetupChannelConfig(pDMA, SSP0_RX_DMA,
 *				(DMA_CFG_PERIPHREQEN | DMA_CFG_TRIGBURST_BURST | DMA_CFG_BURSTPOWER_1 |
 *				DMA_CFG_SRCBURSTWRAP | DMA_CFG_CHPRIORITY(0)));<br>
 *
 *			The following example show how to configure the channel for an external
 *			trigger from the imput mux with low edge polarity, a burst transfer size of 8,
 *			incrementing source and destination addresses, and lowest channel
 *			priority.<br>
 *			Example: Chip_DMA_SetupChannelConfig(pDMA, DMA_CH14,
 *				(DMA_CFG_HWTRIGEN | DMA_CFG_TRIGPOL_LOW | DMA_CFG_TRIGTYPE_EDGE |
 *				DMA_CFG_TRIGBURST_BURST | DMA_CFG_BURSTPOWER_8 |
 *				DMA_CFG_CHPRIORITY(3)));<br>
 *
 *			For non-peripheral DMA triggering (DMA_CFG_HWTRIGEN definition), use the
 *			DMA input mux functions to configure the DMA trigger source for a DMA channel.
 */
STATIC INLINE void DMA_vSetupChannelConfig(DMA_CHID_T teCH, U32 unCfg)
{
	SET_REG_FIELD(LPC_DMA->DMACH[teCH].CFG, (DMA_CFG_MASK&unCfg), unCfg);
}

/**
 * @brief	Returns active status of a DMA channel
 * @param	teCH		: DMA channel ID
 * @return	An Or'ed value of all active DMA channels (0 - 17)
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) is active. A low state is inactive. A active
 *			channel indicates that a DMA operation has been started but
 *			not yet fully completed.
 */
STATIC INLINE bool DMA_bGetActiveChannel(DMA_CHID_T teCH)
{
	return (GET_REG_FIELD(LPC_DMA->DMACOMMON[0].ACTIVE, BIT(teCH)) != 0);
}

/**
 * @brief	Returns busy status of a DMA channel
 * @param	ch		: DMA channel ID
 * @return	An Or'ed value of all busy DMA channels (0 - 17)
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) is busy. A low state is not busy. A DMA
 *			channel is considered busy when there is any operation
 *			related to that channel in the DMA controller�s internal
 *			pipeline.
 */
STATIC INLINE bool DMA_bGetBusyChannel(DMA_CHID_T teCH)
{
	return (GET_REG_FIELD(LPC_DMA->DMACOMMON[0].BUSY, BIT(teCH)) != 0);
}

/**
 * @brief	Aborts a DMA operation for a single channel
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 * @note	To abort a channel, the channel should first be disabled. Then wait
 *			until the channel is no longer busy by checking the corresponding
 *			bit in BUSY. Finally, abort the channel operation. This prevents the
 *			channel from restarting an incomplete operation when it is enabled
 *			again.
 */
STATIC INLINE void DMA_vAbortChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].ABORT, BIT(teCH), BIT(teCH));
}

/**
 * @brief	Set DMA transfer config fields
 * @param	teCH		: DMA channel ID
 * @param	mask	: DMA transfer config 
 * @return	Nothing
 */
STATIC INLINE void DMA_vSetChannelTranConfig(DMA_CHID_T teCH, U32 unConfigMask)
{
	SET_REG_FIELD(LPC_DMA->DMACH[teCH].XFERCFG, (unConfigMask&DMA_XFERCFG_MASK), (unConfigMask&DMA_XFERCFG_MASK));
}

/**
 * @brief	Get DMA transfer config fields
 * @param	teCH	: DMA channel ID
 * @param	mask	: DMA transfer config 
 * @return	Nothing
 */
STATIC INLINE U32 DMA_nGetChannelTranConfig(DMA_CHID_T teCH)
{
	return GET_REG_FIELD(LPC_DMA->DMACH[teCH].XFERCFG, (DMA_XFERCFG_MASK));
}

/**
 * @brief	Set DMA transfer count fields
 * @param	teCH		: DMA channel ID
 * @param	unCount	: DMA transfer shifted count  
 * @return	Nothing
 */
STATIC INLINE void DMA_vSetChannelTranCount(DMA_CHID_T teCH, U32 unCount)
{
	SET_REG_FIELD(LPC_DMA->DMACH[teCH].XFERCFG, (DMA_XFERCFGCNT_MASK), (unCount&DMA_XFERCFGCNT_MASK));
}

/**
 * @brief	Sets up a DMA channel with the passed DMA transfer descriptor
 * @param	teCH		: DMA channel ID
 * @param	pDesc		: DMA channel descriptor table 
 * @return	bool        : true - successed to setup; false: failed
 * @note	See the User Manual for more information for what this bit does.
 *
 */
STATIC INLINE bool DMA_bSetupTranChannelDesc(DMA_CHID_T teCH, DMA_CHDESC_T *pDesc)
{
	DMA_CHDESC_T *ptr = (DMA_CHDESC_T *) READ_REG(LPC_DMA->SRAMBASE);

	if (!DMA_bGetActiveChannel(teCH)) {
		/* Channel is not active, so update the descriptor */
		ptr[teCH] = *pDesc;
		return true;
	}

	return false;
}

/**
 * @brief	Enables a single DMA channel's interrupt used in common DMA interrupt
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 */
STATIC INLINE void DMA_vEnableIntChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].INTENSET, BIT(teCH), BIT(teCH));
}

/**
 * @brief	Disables a single DMA channel's interrupt used in common DMA interrupt
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 */
STATIC INLINE void DMA_vDisableIntChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].INTENCLR, BIT(teCH), BIT(teCH));
}

#ifdef CFG_INTERR_USED
/**
 * @brief	Clears a pending error interrupt status for a single DMA channel
 * @param	teCH		: DMA channel ID
 * @return	bool    : Err interrupt is activated for ch
 */
STATIC INLINE bool DMA_bGetErrorIntChannel(U32 nErrStat, DMA_CHID_T teCH)
{
	return ((nErrStat&BIT(teCH)) != 0);
}

/**
 * @brief	Clears a pending error interrupt status for all DMA channels
 * @param	teCH		: DMA channel ID
 * @return	bool    : Err interrupt is activated for ch
 */
STATIC INLINE U32 DMA_nGetErrorIntChannel(void)
{
	return (READ_REG(LPC_DMA->DMACOMMON[0].ERRINT));
}

/**
 * @brief	Clears pending error interrupt status for multiple DMA channel
 * @param	nIntErrChs		: DMA channel IDs
 * @return	Nothing
 */
STATIC INLINE void DMA_vClearErrorIntChannels(U32 nIntErrChs)
{
	/* Writing a 1 to clear */
	WRITE_REG(LPC_DMA->DMACOMMON[0].ERRINT, nIntErrChs);
}
#endif
 
/**
 * @brief	Clears all interrupt status
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void DMA_vClearAllInt(void)
{
	/* Writing a 1 to clear */
    WRITE_REG(LPC_DMA->DMACOMMON[0].ERRINT, DMA_INT_MASK);
    /* Writing a 1 to clear */
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTA, DMA_INT_MASK);
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTB, DMA_INT_MASK);
}
 
/**
 * @brief	Clears all interrupt status for a single DMA channel
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void DMA_vClearAllIntChannel(DMA_CHID_T teCH)
{
	/* Writing a 1 to clear */
    WRITE_REG(LPC_DMA->DMACOMMON[0].ERRINT, BIT(teCH));
    /* Writing a 1 to clear */
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTA, BIT(teCH));
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTB, BIT(teCH));
}

/**
 * @brief	Sets the VALIDPENDING control bit for a single channel
 * @param	pDMA	: The base of DMA controller on the chip
 * @param	ch		: DMA channel ID
 * @return	Nothing
 * @note	See the User Manual for more information for what this bit does.
 *
 */
STATIC INLINE void DMA_vSetValidChannel(DMA_CHID_T teCH)
{
	SET_REG_FIELD(LPC_DMA->DMACOMMON[0].SETVALID , BIT(teCH), BIT(teCH));
}

/**
 * @brief	Get global pending interrupt or error interrupts - global int status
 * @param	intmask: init flag - DMA_INTSTAT_ACTIVEERRINT or DMA_INTSTAT_ACTIVEINT 
 * @return	bool - An Or'ed value of DMA_INTSTAT_* types
 * @note	If any DMA channels have an active interrupt or error interrupt
 *			pending, this functional will a common status that applies to all
 *			channels.
 */
STATIC INLINE U32 DMA_bGetIntStatus(U32 unIntMask)
{
	return (GET_REG_FIELD(LPC_DMA->INTSTAT, unIntMask) != 0);
}

#ifdef CFG_INTA_USED
/**
 * @brief	Returns active A interrupt status for a channel
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active A interrupt for the channel.
 *			A low state indicates that the A interrupt is not active.
 */
STATIC INLINE bool DMA_bGetActiveIntAChannel(U32 nIntAStat, DMA_CHID_T teCH)
{
	return ((nIntAStat&BIT(teCH)) != 0);
}

/**
 * @brief	Returns active A interrupt status for all channels
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active A interrupt for the channel.
 *			A low state indicates that the A interrupt is not active.
 */
STATIC INLINE U32 DMA_nGetActiveIntAChannel(void)
{
	return (READ_REG(LPC_DMA->DMACOMMON[0].INTA));
}

/**
 * @brief	Clears active A interrupt status for multiple channels
 * @param	nIntAChs		: DMA channel IDs
 * @return	Nothing
 */
STATIC INLINE void DMA_vClearActiveIntAChannels(U32 nIntAChs)
{
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTA, nIntAChs);
}
#endif

#ifdef CFG_INTB_USED
/**
 * @brief	Returns active B interrupt status for all channels
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active B interrupt for the channel.
 *			A low state indicates that the B interrupt is not active.
 */
STATIC INLINE bool DMA_bGetActiveIntBChannel(U32 nIntBStat, DMA_CHID_T teCH)
{
	return ((nIntBStat&BIT(teCH)) != 0);
}

/**
 * @brief	Returns active B interrupt status for all channels
 * @param	teCH		: DMA channel ID
 * @return	Nothing
 * @note	A high values in bits 0 .. 17 in the return values indicates
 *			that the channel for that bit (bit 0 = channel 0, bit 1 -
 *			channel 1, etc.) has an active A interrupt for the channel.
 *			A low state indicates that the A interrupt is not active.
 */
STATIC INLINE U32 DMA_nGetActiveIntBChannel(void)
{
	return (READ_REG(LPC_DMA->DMACOMMON[0].INTB));
}

/**
 * @brief	Clears active B interrupt status for multiple channels
 * @param	nIntBChs		: DMA channel IDs
 * @return	Nothing
 */
STATIC INLINE void DMA_vClearActiveIntBChannels(U32 nIntBChs)
{
	WRITE_REG(LPC_DMA->DMACOMMON[0].INTB, nIntBChs);
}
#endif

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize and setup DMA controller */
void DMA_vInit(void)
{
	int i = 0;
    U16  setuplen = sizeof(DMASetup)/sizeof(DMA_SETUP_T);
	
	/* Enable clock and reset DMA*/
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_DMA);
	SYSCTL_vPeriphReset(RESET_DMA);

    /* Enable DMA controller */
	DMA_vEnable();
	
    /* Assign SRAM base */
    DMA_vSetSRAMBase((U32)LPC_DMA_Table);

    /* Clear all interrupts */
	DMA_vClearAllInt();	

	/* Setup channel for the following configuration - dma_cfg: */
    for(i= 0; i< setuplen; i++){
		/* Setup channel for dma hw trigger source if defined: */
		if(DMATRIG_NOHWTRIG > DMASetup[i].trigsrc){
			INMUX_vSetDMATrigger(DMASetup[i].ch, DMASetup[i].trigsrc);
		}

		DMA_vSetupChannelConfig(DMASetup[i].ch, DMASetup[i].chcfg);

		/* Enable channel interrupt of DMA according to cfg */	
		if(DMASetup[i].chinten){
			DMA_vEnableIntChannel(DMASetup[i].ch);
		}
		else{
            DMA_vDisableIntChannel(DMASetup[i].ch);
		}
	} 
}

/* Reset and initialize DMA controller */
void DMA_vReset(void)
{
    DMA_vShutdown();
	DMA_vInit();
}

/* Start a DMA channel by hw trigger */
bool DMA_bStart(DMA_CHID_T      teCH,   
                U32             unXfercfg,                
                DMA_CHDESC_T    *pDesc)
{
	/* Enable DMA Channel */
    DMA_vEnableChannel(teCH);
	
	/* Setup descriptor sequence */
	if(0 != pDesc){
		if(!DMA_bSetupTranChannelDesc(teCH, &pDesc[0])){
             return false;
	    }	
	}
	
	/* Setup data transfer and trigger it in same call */
	DMA_vSetChannelTranCount(teCH, unXfercfg); /*Should called before DMA_vSetChannelTranConfig */
	DMA_vSetChannelTranConfig(teCH, unXfercfg);
	
	DMA_vSetValidChannel(teCH);

	return true;
}

/* Pause a DMA channel and can resume it */
/* Call DMA_vResume to restart a channel, which is paused before */
/* Call DMA_bStart to restart a channel with new xfer cfg, which is paused before */
void DMA_vPause(DMA_CHID_T teCH)
{
	/* Disable a channel till current operation is completed */
	DMA_vDisableChannel(teCH);
	
	while(DMA_bGetBusyChannel(teCH)){
		;
	}

    /* Abort */
	DMA_vAbortChannel(teCH);

	/* Clear all Interrupt status */
	DMA_vClearAllIntChannel(teCH);
}

/* Resume a DMA channel */
void DMA_vResume(DMA_CHID_T teCH)
{
	/* Enable a channel */
    DMA_vEnableChannel(teCH);

	DMA_vSetChannelTranConfig(teCH, DMA_nGetChannelTranConfig(teCH));

	/* Set Valid Bit again */
	DMA_vSetValidChannel(teCH);
}
#endif /* CFG_DMA_USED */
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                      ISR FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/** 
 *   ISR callbacks for DMA
 **/
void DMA_vISR(void)
{	
	#ifdef CFG_INTA_USED
    U32 reginta = 0;
	#endif
	#ifdef CFG_INTB_USED
	U32 regintb = 0;
	#endif
	#ifdef CFG_INTERR_USED
	U32 reginterr = 0;
	#endif

	#ifdef CFG_INTA_USED
	/* Active pending channel interrupt */
    if(DMA_bGetIntStatus(DMA_INTSTAT_ACTIVEINT))	
    {
        reginta = DMA_nGetActiveIntAChannel();

		#ifdef DMA0_INT_EN
		#if(ENABLE == DMA0_INT_EN)
		   #ifdef DMA0_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA0)){
              DMA0_INTA_CB();
		   }
		   #endif
		#endif
		#endif
		
		#ifdef DMA1_INT_EN
		#if(ENABLE == DMA1_INT_EN)
		   #ifdef DMA1_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA1)){
              DMA1_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA2_INT_EN
		#if(ENABLE == DMA2_INT_EN)
		   #ifdef DMA2_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA2)){
              DMA2_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA3_INT_EN
		#if(ENABLE == DMA3_INT_EN)
		   #ifdef DMA3_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA3)){
              DMA3_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA4_INT_EN
		#if(ENABLE == DMA4_INT_EN)
		   #ifdef DMA4_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA4)){
              DMA4_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA5_INT_EN
		#if(ENABLE == DMA5_INT_EN)
		   #ifdef DMA5_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA5)){
              DMA5_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA6_INT_EN
		#if(ENABLE == DMA6_INT_EN)
		   #ifdef DMA6_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA6)){
              DMA6_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA7_INT_EN
		#if(ENABLE == DMA7_INT_EN)
		   #ifdef DMA7_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA7)){
              DMA7_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA8_INT_EN
		#if(ENABLE == DMA8_INT_EN)
		   #ifdef DMA8_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA8)){
              DMA8_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA9_INT_EN
		#if(ENABLE == DMA9_INT_EN)
		   #ifdef DMA9_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA9)){
              DMA9_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA10_INT_EN
		#if(ENABLE == DMA10_INT_EN)
		   #ifdef DMA10_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA10)){
              DMA10_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA11_INT_EN
		#if(ENABLE == DMA11_INT_EN)
		   #ifdef DMA11_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA11)){
              DMA11_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA12_INT_EN
		#if(ENABLE == DMA12_INT_EN)
		   #ifdef DMA12_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA12)){
              DMA12_INTA_CB();
		   }
		   #endif
		#endif
		#endif
		
		#ifdef DMA13_INT_EN
		#if(ENABLE == DMA13_INT_EN)
		   #ifdef DMA13_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA13)){
              DMA13_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA14_INT_EN
		#if(ENABLE == DMA14_INT_EN)
		   #ifdef DMA14_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA14)){
              DMA14_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA15_INT_EN
		#if(ENABLE == DMA15_INT_EN)
		   #ifdef DMA15_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA15)){
              DMA15_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA16_INT_EN
		#if(ENABLE == DMA16_INT_EN)
		   #ifdef DMA16_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA16)){
              DMA16_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA17_INT_EN
		#if(ENABLE == DMA17_INT_EN)
		   #ifdef DMA17_INTA_CB
		   if(DMA_bGetActiveIntAChannel(reginta, DMA17)){
              DMA17_INTA_CB();
		   }
		   #endif
		#endif
		#endif

		DMA_vClearActiveIntAChannels(reginta);
	}
	#endif /* CFG_INTA_USED */

	#ifdef CFG_INTB_USED
	/* Active pending channel interrupt */
    if(DMA_bGetIntStatus(DMA_INTSTAT_ACTIVEINT))	
    {
		regintb = DMA_nGetActiveIntBChannel();

		#ifdef DMA0_INT_EN
		#if(ENABLE == DMA0_INT_EN)
		   #ifdef DMA0_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA0)){
              DMA0_INTB_CB();
		   }
		   #endif
		#endif
		#endif
		
		#ifdef DMA1_INT_EN
		#if(ENABLE == DMA1_INT_EN)
		   #ifdef DMA1_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA1)){
              DMA1_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA2_INT_EN
		#if(ENABLE == DMA2_INT_EN)
		   #ifdef DMA2_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA2)){
              DMA2_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA3_INT_EN
		#if(ENABLE == DMA3_INT_EN)
		   #ifdef DMA3_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA3)){
              DMA3_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA4_INT_EN
		#if(ENABLE == DMA4_INT_EN)
		   #ifdef DMA4_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA4)){
              DMA4_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA5_INT_EN
		#if(ENABLE == DMA5_INT_EN)
		   #ifdef DMA5_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA5)){
              DMA5_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA6_INT_EN
		#if(ENABLE == DMA6_INT_EN)
		   #ifdef DMA6_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA6)){
              DMA6_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA7_INT_EN
		#if(ENABLE == DMA7_INT_EN)
		   #ifdef DMA7_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA7)){
              DMA7_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA8_INT_EN
		#if(ENABLE == DMA8_INT_EN)
		   #ifdef DMA8_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA8)){
              DMA8_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA9_INT_EN
		#if(ENABLE == DMA9_INT_EN)
		   #ifdef DMA9_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA9)){
              DMA9_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA10_INT_EN
		#if(ENABLE == DMA10_INT_EN)
		   #ifdef DMA10_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA10)){
              DMA10_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA11_INT_EN
		#if(ENABLE == DMA11_INT_EN)
		   #ifdef DMA11_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA11)){
              DMA11_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA12_INT_EN
		#if(ENABLE == DMA12_INT_EN)
		   #ifdef DMA12_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA12)){
              DMA12_INTB_CB();
		   }
		   #endif
		#endif
		#endif
		
		#ifdef DMA13_INT_EN
		#if(ENABLE == DMA13_INT_EN)
		   #ifdef DMA13_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA13)){
              DMA13_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA14_INT_EN
		#if(ENABLE == DMA14_INT_EN)
		   #ifdef DMA14_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA14)){
              DMA14_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA15_INT_EN
		#if(ENABLE == DMA15_INT_EN)
		   #ifdef DMA15_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA15)){
              DMA15_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA16_INT_EN
		#if(ENABLE == DMA16_INT_EN)
		   #ifdef DMA16_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA16)){
              DMA16_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA17_INT_EN
		#if(ENABLE == DMA17_INT_EN)
		   #ifdef DMA17_INTB_CB
		   if(DMA_bGetActiveIntBChannel(regintb, DMA17)){
              DMA17_INTB_CB();
		   }
		   #endif
		#endif
		#endif

		DMA_vClearActiveIntBChannels(regintb);
	}
	#endif /* CFG_INTB_USED */

	#ifdef CFG_INTERR_USED
	/* Active pending error channel interrupt */
	if(DMA_bGetIntStatus(DMA_INTSTAT_ACTIVEERRINT))	
    {
        reginterr = DMA_nGetErrorIntChannel();
		
		#ifdef DMA0_ERRINT_EN
		#if(ENABLE == DMA0_ERRINT_EN)
		   #ifdef DMA0_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA0)){
              DMA0_INTERR_CB();
		   }
		   #endif
		#endif
		#endif
		
		#ifdef DMA1_ERRINT_EN
		#if(ENABLE == DMA1_ERRINT_EN)
		   #ifdef DMA1_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA1)){
              DMA1_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA2_ERRINT_EN
		#if(ENABLE == DMA2_ERRINT_EN)
		   #ifdef DMA2_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA2)){
              DMA2_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA3_ERRINT_EN
		#if(ENABLE == DMA3_ERRINT_EN)
		   #ifdef DMA3_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA3)){
              DMA3_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		#ifdef DMA4_ERRINT_EN
		#if(ENABLE == DMA4_ERRINT_EN)
		   #ifdef DMA4_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA4)){
              DMA4_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA5_ERRINT_EN
		#if(ENABLE == DMA5_ERRINT_EN)
		   #ifdef DMA5_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA5)){
              DMA5_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA6_ERRINT_EN
		#if(ENABLE == DMA6_ERRINT_EN)
		   #ifdef DMA6_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA6)){
              DMA6_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA7_ERRINT_EN
		#if(ENABLE == DMA7_ERRINT_EN)
		   #ifdef DMA7_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA7)){
              DMA7_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA8_ERRINT_EN
		#if(ENABLE == DMA8_ERRINT_EN)
		   #ifdef DMA8_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA8)){
              DMA8_INTERR_CB();
		   }
		   #endif
		#endif
		#endif	

		#ifdef DMA9_ERRINT_EN
		#if(ENABLE == DMA9_ERRINT_EN)
		   #ifdef DMA9_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA9)){
              DMA9_INTERR_CB();
		   }
		   #endif
		#endif
		#endif	

		#ifdef DMA10_ERRINT_EN
		#if(ENABLE == DMA10_ERRINT_EN)
		   #ifdef DMA10_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA10)){
              DMA10_INTERR_CB();
		   }
		   #endif
		#endif
		#endif

		#ifdef DMA11_ERRINT_EN
		#if(ENABLE == DMA11_ERRINT_EN)
		   #ifdef DMA11_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA11)){
              DMA11_INTERR_CB();
		   }
		   #endif
		#endif
		#endif	

		#ifdef DMA12_ERRINT_EN
		#if(ENABLE == DMA12_ERRINT_EN)
		   #ifdef DMA12_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA12)){
              DMA12_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		#ifdef DMA13_ERRINT_EN
		#if(ENABLE == DMA13_ERRINT_EN)
		   #ifdef DMA13_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA13)){
              DMA13_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		#ifdef DMA14_ERRINT_EN
		#if(ENABLE == DMA14_ERRINT_EN)
		   #ifdef DMA14_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA14)){
              DMA14_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		#ifdef DMA15_ERRINT_EN
		#if(ENABLE == DMA15_ERRINT_EN)
		   #ifdef DMA15_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA15)){
              DMA15_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		#ifdef DMA16_ERRINT_EN
		#if(ENABLE == DMA16_ERRINT_EN)
		   #ifdef DMA16_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA16)){
              DMA16_INTERR_CB();
		   }
		   #endif
		#endif
		#endif	

		#ifdef DMA17_ERRINT_EN
		#if(ENABLE == DMA17_ERRINT_EN)
		   #ifdef DMA17_INTERR_CB
	       if(DMA_bGetErrorIntChannel(reginterr, DMA17)){
              DMA17_INTERR_CB();
		   }
		   #endif
		#endif
		#endif		

		DMA_vClearErrorIntChannels(reginterr);
	}
	#endif /* CFG_INTERR_USED */
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

