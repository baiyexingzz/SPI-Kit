/******************************************************************************/
/*                                                                            */
/*    $Workfile::   IAP.h                                                 $   */
/*                                                                            */
/*    $Revision:: 357                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-26 13:12:09 +0800 (周三, 26 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  In-Application Programming of LPC15xx                     */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef IAP_H_
#define IAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "interrupt.h"

/** @defgroup COMMON_IAP CHIP: Common Chip ISP/IAP commands and return codes
 * @ingroup LPC15XX_IAP
 * @{
 */

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define IAP_CMDPARA_MAX             5
#define IAP_RESULT_MAX              5

/* Pointer to ROM IAP entry functions */
#define IAP_ENTRY_LOCATION          0x03000205UL

/* IAP command definitions */
#define IAP_PREWRRITE_CMD           50	/*!< Prepare sector for write operation command */
#define IAP_WRISECTOR_CMD           51	/*!< Write Sector command */
#define IAP_ERSSECTOR_CMD           52	/*!< Erase Sector command */
#define IAP_BLANK_CHECK_SECTOR_CMD  53	/*!< Blank check sector */
#define IAP_REPID_CMD               54	/*!< Read PartID command */
#define IAP_READ_BOOT_CODE_CMD      55	/*!< Read Boot code version */
#define IAP_COMPARE_CMD             56	/*!< Compare two RAM address locations */
#define IAP_REINVOKE_ISP_CMD        57	/*!< Reinvoke ISP */
#define IAP_READ_UID_CMD            58	/*!< Read UID */
#define IAP_ERASE_PAGE_CMD          59	/*!< Erase page */
#define IAP_EEPROM_WRITE            61	/*!< EEPROM Write command */
#define IAP_EEPROM_READ             62	/*!< EEPROM READ command */

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* IAP_ENTRY API function type */
typedef void (*IAP_ENTRY_T)(U32[IAP_CMDPARA_MAX], U32[IAP_RESULT_MAX]);

/* IAP response definitions */
typedef enum{
	IAP_CMD_SUCCESS          =   0,		/*!< Command is executed successfully */
	IAP_INVALID_COMMAND      =   1,		/*!< Invalid command */
	IAP_SRC_ADDR_ERROR       =   2,		/*!< Source address is not on word boundary */
	IAP_DST_ADDR_ERROR       =   3,		/*!< Destination address is not on a correct boundary */
	IAP_SRC_ADDR_NOT_MAPPED  =   4,		/*!< Source address is not mapped in the memory map */
	IAP_DST_ADDR_NOT_MAPPED  =   5,		/*!< Destination address is not mapped in the memory map */
	IAP_COUNT_ERROR          =   6,		/*!< Byte count is not multiple of 4 or is not a permitted value */
	IAP_INVALID_SECTOR       =   7,		/*!< Sector number is invalid or end sector number is greater than start sector number */
	IAP_SECTOR_NOT_BLANK     =   8,		/*!< Sector is not blank */
	IAP_SECTOR_NOT_PREPARED  =   9,		/*!< Command to prepare sector for write operation was not executed */
	IAP_COMPARE_ERROR        =   10,	/*!< Source and destination data not equal */
	IAP_BUSY                 =   11,	/*!< Flash programming hardware interface is busy */
	IAP_PARAM_ERROR          =   12,	/*!< nsufficient number of parameters or invalid parameter */
	IAP_ADDR_ERROR           =   13,	/*!< Address is not on word boundary */
	IAP_ADDR_NOT_MAPPED      =   14,	/*!< Address is not mapped in the memory map */
	IAP_CMD_LOCKED           =   15,	/*!< Command is locked */
	IAP_INVALID_CODE         =   16,	/*!< Unlock code is invalid */
	IAP_INVALID_BAUD_RATE    =   17,	/*!< Invalid baud rate setting */
	IAP_INVALID_STOP_BIT     =   18,	/*!< Invalid stop bit setting */
	IAP_CRP_ENABLED          =   19,	/*!< Code read protection enabled */
}IAP_STATUS_T;
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief LPC15XX IAP_ENTRY API function type
 */
static INLINE void IAP_vEntry(U32 bufCmdParam[IAP_CMDPARA_MAX], U32 bufStatus[IAP_RESULT_MAX])
{
    /*The on-chip flash memory is not accessible during erase/write operations. When the user
      application code starts executing, the interrupt vectors from the user flash area are active.
      Before making any IAP call, either disable the interrupts or ensure that the user interrupt
      vectors are active in RAM and that the interrupt handlers reside in RAM. The IAP code
      does not use or disable interrupts.*/
	
	INTERRUPT_vDisableAll();
	((IAP_ENTRY_T) IAP_ENTRY_LOCATION)(bufCmdParam, bufStatus);
	INTERRUPT_vEnableAll();
}

/**
 * @brief	Prepare sector for write operation
 * @param	unStrSector	: Start sector number
 * @param	unEndSector	: End sector number
 * @return	Status code to indicate the command is executed successfully or not
 * @note	This command must be executed before executing "Copy RAM to flash"
 *			or "Erase Sector" command.
 *			The end sector must be greater than or equal to start sector number
 */
IAP_STATUS_T IAP_tPreSectorForWrite(U32 unStrSector, U32 unEndSector);

/**
 * @brief	Copy RAM to flash
 * @param	unDstAdd  : Destination flash address where data bytes are to be written
 * @param	pSrcAdd	  : Source flash address where data bytes are to be read
 * @param	unBytes	  : Number of bytes to be written
 * @return	Status code to indicate the command is executed successfully or not
 * @note	The addresses should be a 256 byte boundary and the number of bytes
 *			should be 256 | 512 | 1024 | 4096
 */
IAP_STATUS_T IAP_tCopyRamToFlash(U32 unDstAdd, U32 unSrcAdd, U32 unBytes);

/**
 * @brief Blank check a sector or multiples sector of on-chip flash memory
 * @param	unStrSector	: Start sector number
 * @param	unEndSector	: End sector number
 * @return	Offset of the first non blank word location if the status code is SECTOR_NOT_BLANK
 * @note	The end sector must be greater than or equal to start sector number
 */
// FIXME - There are two return value (result[0] & result[1]
// Result0:Offset of the first non blank word location if the Status Code is
// SECTOR_NOT_BLANK.
// Result1:Contents of non blank word location.
IAP_STATUS_T IAP_tBlankCheckSector(U32 unStrSector, U32 unEndSector);

/**
 * @brief	Erase sector
 * @param	unStrSector	: Start sector number
 * @param	unEndSector	: End sector number
 * @return	Status code to indicate the command is executed successfully or not
 * @note	The end sector must be greater than or equal to start sector number
 */
IAP_STATUS_T IAP_tEraseSector(U32 unStrSector, U32 unEndSector);

/**
 * @brief	Erase a page or multiple papers of on-chip flash memory
 * @param	unStrPage	: Start page number
 * @param	unEndPage	: End page number
 * @return	Status code to indicate the command is executed successfully or not
 * @note	The page number must be greater than or equal to start page number
 */
// FIXME - There are four return value
// Result0:The first 32-bit word (at the lowest address)
// Result1:The second 32-bit word.
// Result2:The third 32-bit word.
// Result3:The fourth 32-bit word.
IAP_STATUS_T IAP_tErasePage(U32 unStrPage, U32 unEndPage);

/**
 * @brief	Compare the memory contents at two locations
 * @param	unDstAdd   : Destination of the RAM address of data bytes to be compared
 * @param	unSrcAdd   : Source of the RAM address of data bytes to be compared
 * @param	unBytes	   : Number of bytes to be compared
 * @return	Offset of the first mismatch of the status code is COMPARE_ERROR
 * @note	The addresses should be a word boundary and number of bytes should be
 *			a multiply of 4
 */
IAP_STATUS_T IAP_tCompare(U32 unDstAdd, U32 unSrcAdd, U32 unBytes);


/**
 * @brief	Read part identification number
 * @return	Part identification number
 */
U32 IAP_nReadPID(void);

/**
 * @brief	Read boot code version number
 * @return	Boot code version number
 */
U32 IAP_nReadBootCode(void);

/**
 * @brief	IAP reinvoke ISP to invoke the bootloader in ISP mode
 * @return	Status code to indicate the command is executed successfully or not
 */
IAP_STATUS_T  IAP_tReinvokeISP(void);

/**
 * @brief	Read the unique ID
 * @return	Status code to indicate the command is executed successfully or not
 */
U32 IAP_nReadUID(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* IAP_H_ */

