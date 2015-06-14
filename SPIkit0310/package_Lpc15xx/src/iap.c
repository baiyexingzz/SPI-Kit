/******************************************************************************/
/*                                                                            */
/*    $Workfile::   IAP.c                                                 $   */
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
#include "platform.h"

#include "system.h"
#include "iap.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Prepare sector for write operation */
IAP_STATUS_T IAP_tPreSectorForWrite(U32 unStrSector, U32 unEndSector)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_PREWRRITE_CMD;
	command[1] = unStrSector;
	command[2] = unEndSector;

	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* Copy RAM to flash */
IAP_STATUS_T IAP_tCopyRamToFlash(U32 unDstAdd, U32 unSrcAdd, U32 unBytes)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_WRISECTOR_CMD;
	command[1] = unDstAdd;
	command[2] = unSrcAdd;
	command[3] = unBytes;
	command[4] = CLOCK_nGetSysClock()/ 1000;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* Blank check sector */
IAP_STATUS_T IAP_tBlankCheckSector(U32 unStrSector, U32 unEndSector)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_BLANK_CHECK_SECTOR_CMD;
	command[1] = unStrSector;
	command[2] = unEndSector;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* Erase sector */
IAP_STATUS_T IAP_tEraseSector(U32 unStrSector, U32 unEndSector)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_ERSSECTOR_CMD;
	command[1] = unStrSector;
	command[2] = unEndSector;
	command[3] = CLOCK_nGetSysClock() / 1000;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* Erase page */
IAP_STATUS_T IAP_tErasePage(U32 unStrPage, U32 unEndPage)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_ERASE_PAGE_CMD;
	command[1] = unStrPage;
	command[2] = unEndPage;
	command[3] = CLOCK_nGetSysClock()/ 1000;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* IAP compare */
IAP_STATUS_T IAP_tCompare(U32 unDstAdd, U32 unSrcAdd, U32 unBytes)
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_COMPARE_CMD;
	command[1] = unDstAdd;
	command[2] = unSrcAdd;
	command[3] = unBytes;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}
/*---------------------------------------*/
/* Read part identification number */
U32 IAP_nReadPID()
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_REPID_CMD;
	IAP_vEntry(command, result);

	return result[1];
}

/* Read boot code version number */
U32 IAP_nReadBootCode()
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_READ_BOOT_CODE_CMD;
	IAP_vEntry(command, result);

	return result[1];
}

/* Reinvoke ISP */
IAP_STATUS_T IAP_tReinvokeISP()
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_REINVOKE_ISP_CMD;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/* Read the unique ID */
U32 IAP_nReadUID()
{
	U32 command[IAP_CMDPARA_MAX], result[IAP_RESULT_MAX];

	command[0] = IAP_READ_UID_CMD;
	IAP_vEntry(command, result);

	return result[1];
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

