/******************************************************************************/
/*                                                                            */
/*    $Workfile::   dbase_stm32.c                                        $    */
/*                                                                            */
/*    $Revision:: 4875                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2014-02-13 11:31:39 +0800 (周四, 13 二月 2014)     $    */
/*                                                                            */
/*      $Author:: kelvin.zhang@philips.com                               $    */
/*                                                                            */
/*        Owner::   kyleye                                                    */
/*                                                                            */
/*  Description::   Database module optimized for STM32 only.                 */
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


#if ((TARGET_DEVICE == DEVICE_INTERLEAVED)||(TARGET_DEVICE == DEVICE_SVHP))
#include "statemachine.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/*
This is the extern NVM M24C16's write page size.
Write length larger than 16 bytes is possible, but will cause the function
MEMORY_teWrite blocked for long time because during EEPROM writing its i2c
interface does not response MCU. Thus DBase table with larger size than this
(currently only LoggingData) will be seperated into several section, and
each section will be max of 16 bytes. After each section sent to memory via
i2c, kernel will schedule other procedure during eeprom writing busy stage,
to improve efficiency especially UART command respond for Image Care.
*/
#define NVM_MAX_WRITE_LENGTH (16)
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
//Intermittent info for table LoggingData saving
static UBYTE m_ubLoggingDataWritePt;

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
static void DBASE_vUpdateTableHeader(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const T_DBASE_VERSION tVersion);
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
void DBASE_vSaveTableB(TE_DBASE_TABLELIST teTable)
{
  UBYTE ubBytesToWrite;
  switch(teTable)
  {
	  case TableCalibData:
		DBASE_vUpdateTableHeader((UBYTE *)&CalibData,
								 sizeof(TS_HDR_CalibData),
								 (T_DBASE_VERSION)DBASE_VER_CalibData);
		(void)MEMORY_teWrite(&CalibData, 
							 (T_ADDRESS_SPACE)DBASE_ADR_CalibData,
							 sizeof(TS_HDR_CalibData),
							 MEMTYPE_NVRAM);
		break;

    case TableOperationalData:
      DBASE_vUpdateTableHeader((UBYTE *)&OperationalData,
                               sizeof(TS_HDR_OperationalData),
                               (T_DBASE_VERSION)DBASE_VER_OperationalData);
      (void)MEMORY_teWrite(&OperationalData, 
                           (T_ADDRESS_SPACE)DBASE_ADR_OperationalData,
                           sizeof(TS_HDR_OperationalData),
                           MEMTYPE_NVRAM);
      break;
#ifdef CFG_ENABLE_IMAGELIFE
  case TableOperationalDataBackup:
      DBASE_vUpdateTableHeader((UBYTE *)&OperationalDataBackup,
                               sizeof(TS_HDR_OperationalDataBackup),
                               (T_DBASE_VERSION)DBASE_VER_OperationalDataBackup);

      (void)MEMORY_teWrite(&OperationalDataBackup, 
                           (T_ADDRESS_SPACE)DBASE_ADR_OperationalDataBackup,
                           sizeof(TS_HDR_OperationalDataBackup),
                           MEMTYPE_NVRAM);
      break;
#endif //CFG_ENABLE_IMAGELIFE

#ifdef CFG_ENABLE_LOGDATA
    case TableLoggingData:
      if(m_ubLoggingDataWritePt == 0)
      {
        //A new save session starts, update CRC/version first 
        DBASE_vUpdateTableHeader((UBYTE *)&LoggingData,
                                 sizeof(TS_HDR_LoggingData),
                                 (T_DBASE_VERSION)DBASE_VER_LoggingData);
      }

      ubBytesToWrite = (UBYTE)(sizeof(TS_HDR_LoggingData)-m_ubLoggingDataWritePt);
      if (!SM_blSMFactoryModeEnable()) //During factory mode disable the 16-byte page size so that logging data can be written all-at-once
      {
        if (ubBytesToWrite > NVM_MAX_WRITE_LENGTH) 
        {
          ubBytesToWrite = NVM_MAX_WRITE_LENGTH;
        }
      }

      if(ubBytesToWrite > 0)
      {
        UBYTE* pubBuf = (UBYTE*)&LoggingData;
        pubBuf += m_ubLoggingDataWritePt;
        (void)MEMORY_teWrite(pubBuf, 
                             (T_ADDRESS_SPACE)DBASE_ADR_LoggingData+m_ubLoggingDataWritePt,
                             ubBytesToWrite,
                             MEMTYPE_NVRAM);
        m_ubLoggingDataWritePt += ubBytesToWrite;
      }
      else
      {
        m_ubLoggingDataWritePt = 0;
      }
      break;
#endif
    default:
      //For other tables are read only
      break;
  }
}

void DBASE_vSetLoggingDataModified(void)
{
  /*
  Content in LoggingData table has been modified. Reset the status so that 
  Next LoggingData save time the table should be witten from begin again.
  */
  m_ubLoggingDataWritePt = 0;
}

TE_SAVINGLOG_STATUS DBASE_teGetLogDataSavingStatus(void)
{
  if(m_ubLoggingDataWritePt == 0)
  {
    //Allowed to start a new log table saving session
    return SAVINGLOG_IDLE;
  }
  else if(m_ubLoggingDataWritePt < sizeof(TS_HDR_LoggingData))
  {
    /*A saving session is ongoing, calling DBASE_vSave(LoggingData)
      will continue saving from last break point.
    */
    return SAVINGLOG_ONGOING;
  }
  else //m_ubWriteSizeLogging==sizeof(TS_HDR_LoggingData)
  {
    /*Session completed.*/
    return SAVINGLOG_COMPLETED;
  }
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
static void DBASE_vUpdateTableHeader(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const T_DBASE_VERSION tVersion)
{
  //Copy version number:
  DBASE_GET_HEADER(pubTable)->tVersion = tVersion;

  //Determine checksum:
  //calculte with all table bytes exclude CRC byte itself
  DBASE_GET_HEADER(pubTable)->tChecksum = \
    DBASE_CHECK((pubTable+DBASE_HEADER_CHECKSUM_SIZE), \
                 0,(UBYTE)(ubTableSize-DBASE_HEADER_CHECKSUM_SIZE));
}

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

