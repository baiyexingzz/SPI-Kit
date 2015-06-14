/******************************************************************************/
/*                                                                            */
/*    $Workfile::   memory.h                                             $    */
/*                                                                            */
/*    $Revision:: 4731                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-09-23 10:57:09 +0800 (周一, 23 九月 2013)     $    */
/*                                                                            */
/*      $Author:: ricky.lu@philips.com                                   $    */
/*                                                                            */
/*        Owner::   Team RSA                                                  */
/*                                                                            */
/*  Description::   Memory handler for the ARM7 series                        */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2008 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#ifndef MEMORY_H
#define MEMORY_H

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include "kernel.h"
#include "M24C16.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/*lint -esym(756,T_ADDRESS_SPACE) */
typedef ULONG T_ADDRESS_SPACE;

/*lint -esym(756,TE_MEMTYPES) */
/*lint -esym(769,MEMTYPE_RAM) */
/*lint -esym(769,MEMTYPE_NVRAM) */
/*lint -esym(769,MEMTYPE_EXT) */
/*lint -esym(769,MEMTYPE_ROM) */
/*lint -esym(769,MEMTYPE_FLASH) */
typedef enum
{
  MEMTYPE_RAM   = 0x00,
  MEMTYPE_NVRAM = 0x01,
  MEMTYPE_EXT   = 0x02,
  MEMTYPE_ROM   = 0x03,
  MEMTYPE_FLASH = 0x04
} TE_MEMTYPES;

typedef enum
{ 
  FLASH_BUSY        = 1,
  FLASH_ERROR_PG    = 2,
  FLASH_ERROR_WRP   = 3,
  FLASH_COMPLETE    = 4,
  FLASH_TIMEOUT     = 5,
  FLASH_LOCKED      = 6,
  FLASH_BAD_ADDRESS = 7  
}FLASH_Status;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

void MEMORY_vInit(void);

TE_RESULT MEMORY_teWrite(const void *pvSrc,
                         T_ADDRESS_SPACE tOffset,
                         UBYTE ubNrOfBytes,
                         TE_MEMTYPES teMemType);

TE_RESULT MEMORY_teRead(void *pvDst,
                        T_ADDRESS_SPACE tOffset,
                        UBYTE ubNrOfBytes,
                        TE_MEMTYPES teMemType);

//Read the whole table from EEPROM from HW concept 
TE_RESULT MEMORY_teReadTable(void *pvDst,
                        T_ADDRESS_SPACE tOffset,
                        UBYTE ubNrOfBytes,
                        TE_MEMTYPES teMemType);

void MEMORY_vWriteBusyTaskHandler(TE_EVENT_ID teEventId);
#define MEMORY_blIsNvmWritingBusy M24C16_blGetBusyFlag
#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

