/******************************************************************************/
/*                                                                            */
/*    $Workfile::   dbase.c                                              $    */
/*                                                                            */
/*    $Revision:: 4805                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-11-28 14:16:45 +0800 (周四, 28 十一月 2013)  $    */
/*                                                                            */
/*      $Author:: ricky.lu@philips.com                                   $    */
/*                                                                            */
/*        Owner::   kyleye                                                    */
/*                                                                            */
/*  Description::   Database module optimized for STM8 only.                  */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

#include "platform.h"
#include "stdtypes.h"

#include "dbase.h"
#include "memory.h"

#if ((TARGET_DEVICE == DEVICE_BASICVALUE)||(TARGET_DEVICE == DEVICE_SUPERVALUE))

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
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
static void DBASE_vSaveTableA(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const USHORT usAddress,
  const T_DBASE_VERSION tVersion);
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
void DBASE_vSaveTableB(TE_DBASE_TABLELIST teTable)
{
  switch(teTable)
  {
    case TableOperationalData:
      DBASE_vSaveTableA((UBYTE *)&OperationalData,
                        sizeof(TS_DAT_OperationalData),
                        (const USHORT)(DBASE_ADR_OperationalData),
                        (const T_DBASE_VERSION)(DBASE_VER_OperationalData));
      break;
    
#ifdef CFG_ENABLE_IMAGELIFE
  case TableOperationalDataBackup:
      DBASE_vSaveTableA((UBYTE *)&OperationalDataBackup,
            sizeof(TS_DAT_OperationalDataBackup),
            (const USHORT)(DBASE_ADR_OperationalDataBackup),
            (const T_DBASE_VERSION)(DBASE_VER_OperationalDataBackup));
    break;
#endif //CFG_ENABLE_IMAGELIFE

#ifdef CFG_ENABLE_LOGDATA
    case TableLoggingData:
      DBASE_vSaveTableA((UBYTE *)&LoggingData,
                        sizeof(TS_DAT_LoggingData),
                        (const USHORT)(DBASE_ADR_LoggingData),
                        (const T_DBASE_VERSION)(DBASE_VER_LoggingData));
      break;
#endif
    default:
      //For other tables are read only
      break;
  }
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
static void DBASE_vSaveTableA(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const USHORT usAddress,
  const T_DBASE_VERSION tVersion)
{
  //Copy version number
  DBASE_GET_HEADER(pubTable)->tVersion = tVersion;

  //Determine checksum
  DBASE_GET_HEADER(pubTable)->tChecksum =
      DBASE_CHECK((pubTable+DBASE_HEADER_CHECKSUM_SIZE),0,(UBYTE)(ubTableSize+DBASE_HEADER_VERSION_SIZE));

  //Start to write non-volatile memory with EOP interrupt method
  MEMORY_vStartNvmIntWrite(pubTable, usAddress, (UBYTE)(ubTableSize+DBASE_HEADER_SIZE));
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/
#endif

