/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SSI_Cfg.h                                                 $ */
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
/*  Description::   Generic configuration of SPI0/1                           */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SPI_CFG_H
#define SPI_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define    CFG_SPI0_ENABLE          ENABLE
#define    CFG_SPI1_ENABLE          DISABLE
 
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/                           
/**
  * Descrptions for per SPI channel configurations
  * SPI0_CFG: An structure value including:
                   SPI_MODE_T 
                   SPI_CLOCK_MODE_T 
                   SPI_DATA_ORDER_T
                   SPI_CFG_SPOLx_LO/SPI_CFG_SPOLx_HI based on SPIx_SSEL_CFG  
                   SPI Clock Divider Value (U16)
  * SPIx_SSEL_CFG: An Or'ed value of the following:
                   SPI_RXDAT_RXSSELx_ACTIVE 
                   SPI_RXDAT_RXSSELx_INACTIVE 
  */

/**
 *   Init configurations of SPI0
 **/
#if (ENABLE == CFG_SPI0_ENABLE)
#define ALIAS_SPI0             SPI0

/* Current ssel config */
#define SPI0_FRAME_SIZE        (8)

#define SPI0_SSEL_CFG          (SPI_RXDAT_RXSSEL0_ACTIVE  |\
                                SPI_RXDAT_RXSSEL1_INACTIVE|\
                                SPI_RXDAT_RXSSEL2_INACTIVE|\
                                SPI_RXDAT_RXSSEL3_INACTIVE)

/* SPI0 Config */
#define SPI0_CFG_MODE          SPI_MODE_MASTER
#define SPI0_CFG_DORDER        SPI_DATA_MSB_FIRST  
#define SPI0_CFG_SSLPOL        SPI_CFG_SPOL0_LO
#define SPI0_CFG_CLOCK         SPI_CLOCK_CPHA1_CPOL1  
#define SPI0_CFG_CLOCK_DIV     (72)

#define SPI0_INEN_CFG SPI_INTENSET_TXRDYEN
/*
#define SPI0_INEN_CFG          (SPI_INTENSET_RXRDYEN|\
		                        SPI_INTENSET_RXOVEN |\
	                            SPI_INTENSET_TXUREN |\
	                            SPI_INTENSET_SSAEN  |\
	                            SPI_INTENSET_SSDEN)
	*/                           
//#undef  SPI0_RX_CB             /* Receive data Callback */
//#undef  SPI0_EVENT_CB          /* Event Callback for others interrupts*/
//#define SPI0_RX_CB
//#define SPI0_EVENT_CB

#endif

/**
 *   Init configurations of SPI1
 **/
#if (ENABLE == CFG_SPI1_ENABLE)
// To be configured
#endif

/* Macro def - SPI used */
#if ((ENABLE == CFG_SPI0_ENABLE)||\
	 (ENABLE == CFG_SPI1_ENABLE))
#define CFG_SPI_USED
#endif

#endif

