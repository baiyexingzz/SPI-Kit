/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Memory.h                                                  $ */
/*                                                                            */
/*    $Revision:: 357                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-26 13:12:09 +0800 (周三, 26 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - Flash, etc      */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef MEMORY_H
#define MEMORY_H

#include "platform.h"

#include "iap.h"
#include "system.h"
#include "memory_cfg.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Default FLASH Access time */
#define SYSCTL_FLASHTIM_DEFAULT              SYSCTL_FLASHTIM_72MHZ_CPU

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Validate a address in the expected range */
#define MEMORY_VALIDATE_ADDR(addr, min, max) (((U32)((addr)+1) >= (U32)((min)+1))&&((U32)(addr) <= (max)))

/* Flash CRP definitions */
#define FLASH_CRP_ADDR                       (0x000002FCUL)

/* Flash Information */
#define FLASH_SECTOR_BYTES                   (4096)    /* Bytes per sector */
#define FLASH_PAGE_BYTES                     (256)     /* Bytes per page */

/* Start address for a page */
#define FLASH_PAGE_STARTADDRESS(page)        (CFG_FLASH_BASE_ADDR + (page)*FLASH_PAGE_BYTES)
/* End address for a page */
#define FLASH_PAGE_ENDADDRESS(page)          (FLASH_PAGE_STARTADDRESS((page)+1)-1)

/* Page count per sector */
#define FLASH_SECTORS_IN_PAGE                (16)

/* Relative address offset to flash base address */
#define FLASH_OFFSET(addr)                   ((U32)((addr) - CFG_FLASH_BASE_ADDR))

/* Sector number contains the specific address */
#define FLASH_SECTOR(addr)                   ((U32)(FLASH_OFFSET((addr))/FLASH_SECTOR_BYTES))
/* Sector page contains the specific address */
#define FLASH_PAGE(addr)                     ((U32)(FLASH_OFFSET((addr))/FLASH_PAGE_BYTES))

#if (TARGET_PROCESSOR  ==  LPC1517)
#define FLASH_SECTOR_MIN                     (0)
#define FLASH_SECTOR_MAX                     (15)

#define FLASH_PAGE_MIN                       (0)
#define FLASH_PAGE_MAX                       (255)
#else
/* To be extended for other chip types */
#endif

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* Address space type */
typedef U32 T_ADDRESS_SPACE;

typedef enum
{
    MEMTYPE_SRAM   = 0x00,
    MEMTYPE_EXT    = 0x01,
    MEMTYPE_ROM    = 0x02,
    MEMTYPE_FLASH  = 0x03,
    MEMTYPE_EEPROM = 0x04,
} LPC_MEMTYPES_T;

/* Memory_Operation result  */
typedef enum
{
  MEM_SUCCESS = 0,
  MEM_ERROR,
  MEM_BAD_PARAM,
  MEM_BAD_STATE,
  MEM_BUSY,
  MEM_IDLE,
  MEM_TIMEOUT,
  MEM_INVAIID_ADDR,
  MEM_OUT_OF_RANGE,
  MEM_UNKNOWN_PARAM,
  MEM_BACKWARDS_EVENT,
  MEM_RESULT_LAST = 0xFF,
} MEM_RESULT_T;

/* Flash CRP */
typedef enum
{
    FLASH_CRP_NORMAL   = 0x00,
    FLASH_CRP_NOISP    = 0x01,
    FLASH_CRP_1        = 0x02,
    FLASH_CRP_2        = 0x03,
    FLASH_CRP_3        = 0x04,
} LPC_FLASH_CRP_T;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize SRAM and Flash controller
 * @param	Nothing
 * @return	MEM_RESULT_T 
 */
MEM_RESULT_T MEMORY_tInit(void);

/**
 * @brief	Write data into memory
 * @param	pvSrc       : Source data buffer
 * @param	tOffset     : Address offset to specific memory base address  
 * @param	ubNrOfBytes : Size of data
 * @param	teMemType   : memory type
 * @return	MEM_RESULT_T
 */
MEM_RESULT_T MEMORY_tWrite(const void      *pvSrc,
                           T_ADDRESS_SPACE tOffset,					 
                           U16             unNrOfBytes,
                           LPC_MEMTYPES_T  teMemType);

/**
 * @brief	Read data from memory
 * @param	pvDst       : Destination data buffer
 * @param	ubNrOfBytes : Size of data
 * @param	teMemType   : memory type
 * @return	MEM_RESULT_T 
 */
MEM_RESULT_T MEMORY_tRead(void            *pvDst,
                          T_ADDRESS_SPACE tOffset,
                          U16             unNrOfBytes,
                          LPC_MEMTYPES_T  teMemType);

/**
 * @brief	Erase Flash Page(s)
 * @param	unStrPage   : Page start index
 * @param	unEndPage   : Page End index
 * @return	MEM_RESULT_T
 * @note	The page number must be greater than or equal to start page number 
 */
MEM_RESULT_T MEMORY_tEraseFlashPage(U32 unStrPage, U32 unEndPage);

/**
 * @brief	Erase Flash Sector(s)
 * @param	unStrPage   : Sector start index
 * @param	unEndPage   : Sectoe End index
 * @note	The sector number must be greater than or equal to start sector number 
 * @return	MEM_RESULT_T
 */
MEM_RESULT_T MEMORY_tEraseFlashSector(U32 unStrSector, U32 unEndSector);

/**
 * @brief	Refresh Flash CRP Code
 * @param	teCRP   : Flash CRP type
 * @return	MEM_RESULT_T
 */
MEM_RESULT_T MEMORY_tRefreshFlashCRP(LPC_FLASH_CRP_T teCRP);

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
