/******************************************************************************/
/*                                                                            */
/*    $Workfile::   dbase.c                                              $    */
/*                                                                            */
/*    $Revision:: 4874                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2014-02-12 23:32:39 +0800 (周三, 12 二月 2014)     $    */
/*                                                                            */
/*      $Author:: kelvin.zhang@philips.com                               $    */
/*                                                                            */
/*        Owner::   R. van Zundert                                            */
/*                                                                            */
/*  Description::   Database module.                                          */
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

#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
  #include <stdio.h>
  #define __MODULE__ "DBASE"
#endif

/*lint -esym(438, usIndicator) */

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define LOGDATA_CRC_MISMATCH_FLAG (0x80)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

#define DBASE_CFG_FIELD_DEFINITIONS

//Tables data space
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  TS_HDR_##_TABLE_NAME_ _TABLE_NAME_;
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE

#undef DBASE_CFG_FIELD_DEFINITIONS

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
#define DBASE_CFG_FIELD_DEFINITIONS

//Tables default data
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  static const TS_DAT_##_TABLE_NAME_ TARGET_MEM_ROM DBASE_DEF_##_TABLE_NAME_ = {
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY) \
    _FIELD_DEFAULT_,
#define DBASE_END_TABLE(_TABLE_NAME_) \
  };
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE

///////////////////////////////////////////////////////////////////////////////

//Tables minimum data
#ifndef DBASE_DISABLE_MIN_MAX
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  static const TS_DAT_##_TABLE_NAME_ TARGET_MEM_ROM DBASE_MIN_##_TABLE_NAME_ = {
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY) \
    _FIELD_MIN_,
#define DBASE_END_TABLE(_TABLE_NAME_) \
  };
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE
#endif

///////////////////////////////////////////////////////////////////////////////

//Tables maximum data
#ifndef DBASE_DISABLE_MIN_MAX
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  static const TS_DAT_##_TABLE_NAME_ TARGET_MEM_ROM DBASE_MAX_##_TABLE_NAME_ = {
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY) \
    _FIELD_MAX_,
#define DBASE_END_TABLE(_TABLE_NAME_) \
  };
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE
#endif

///////////////////////////////////////////////////////////////////////////////
#ifdef DBASE_NEED_HELPER
#define DBASE_GET_SIGN(_TYPE_) ((_TYPE_)(-1)>0 ? 0 : 128)

#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  static const UBYTE TARGET_MEM_ROM DBASE_HLP_##_TABLE_NAME_[] = {
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY) \
    (sizeof(_FIELD_TYPE_) | DBASE_GET_SIGN(_FIELD_TYPE_)),
#define DBASE_END_TABLE(_TABLE_NAME_) \
    0\
  };
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE
#endif

///////////////////////////////////////////////////////////////////////////////
#undef DBASE_CFG_FIELD_DEFINITIONS

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
static USHORT DBASE_usLoadDatabase(void);

#ifndef DBASE_DISABLE_MIN_MAX
static SBYTE DBASE_sbCompare(const UBYTE *pubS1, const UBYTE TARGET_MEM_ROM *pubS2, UBYTE ubN);
#endif

#if 0
/* Swap byte order to deal with endianness differences */
static void DBASE_vSwap(UBYTE *pubTable, const UBYTE TARGET_MEM_ROM *pubTableHelper);
#else
/* Identical byte order, no swSMing needed */
#define DBASE_vSwap(__TABLE__,__HELPER__)
#endif

#if ((TARGET_DEVICE!=DEVICE_BASICVALUE) && (TARGET_DEVICE != DEVICE_SUPERVALUE)&& (TARGET_DEVICE!=DEVICE_INTERLEAVED)&&(TARGET_DEVICE!= DEVICE_SVHP))
static void DBASE_vSaveTableA(UBYTE *pubTable,
  const UBYTE ubTableSize,
#ifdef DBASE_NEED_HELPER
  const UBYTE *pubTableHelper,
#endif
  const USHORT usAddress,
  const T_DBASE_VERSION tVersion);
#endif

static BOOL DBASE_blLoadTableA(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const UBYTE *pubTableDefault,
#ifndef DBASE_DISABLE_MIN_MAX
  const UBYTE *pubTableMin,
  const UBYTE *pubTableMax,
#endif
#ifdef DBASE_NEED_HELPER
  const UBYTE *pubTableHelper,
#endif
  const USHORT usAddress,
  const T_DBASE_VERSION tVersion,
  const BOOL blLoadDefaultAllowed);

#ifdef CFG_ENABLE_LOGDATA
static void DBASE_vSetLogDataIncompleteFlag(void);
#else
#define DBASE_vSetLogDataIncompleteFlag()  
#endif
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
#if ((TARGET_DEVICE!=DEVICE_BASICVALUE) && (TARGET_DEVICE != DEVICE_SUPERVALUE)&& (TARGET_DEVICE!=DEVICE_INTERLEAVED)&&(TARGET_DEVICE!= DEVICE_SVHP))
/*
**  Description : Interface to write a table to non-volatile memory
**  Parameters  : teTable: table reference, use the name as defined in
**                 dbase_cfg.h
**  Returns     : -
*/
void DBASE_vSaveTableB(TE_DBASE_TABLELIST teTable)
{
  switch (teTable)
  {
#define DBASE_CFG_FIELD_DEFINITIONS
#ifdef DBASE_NEED_HELPER
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      DBASE_vSaveTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (UBYTE *)&DBASE_HLP_##_TABLE_NAME_, \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_)); \
      break;
#else
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      DBASE_vSaveTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_)); \
      break;
#endif
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE
#undef DBASE_CFG_FIELD_DEFINITIONS
    case DBASE_DUMMY:
    default:
      // Do nothing
      break;
  }
}
#endif
///////////////////////////////////////////////////////////////////////////////

/*
**  Description : Interface to read a table from non-volatile memory
**  Parameters  : teTable: table reference, use the name as defined in
**                 dbase_cfg.h
**  Returns     : -
*/
BOOL DBASE_blLoadTableB(TE_DBASE_TABLELIST teTable)
{
  BOOL blLoadResult = FALSE;
  switch (teTable)
  {
#define DBASE_CFG_FIELD_DEFINITIONS
#ifndef DBASE_DISABLE_MIN_MAX
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      blLoadResult = DBASE_blLoadTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (UBYTE *)&DBASE_DEF_##_TABLE_NAME_, \
        (UBYTE *)&DBASE_MIN_##_TABLE_NAME_, \
        (UBYTE *)&DBASE_MAX_##_TABLE_NAME_, \
        (UBYTE *)&DBASE_HLP_##_TABLE_NAME_, \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_)); \
      break;
#else
#ifdef DBASE_NEED_HELPER
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      blLoadResult = DBASE_blLoadTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (UBYTE *)&DBASE_DEF_##_TABLE_NAME_, \
        (UBYTE *)&DBASE_HLP_##_TABLE_NAME_, \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_)); \
      break;
#else
#ifdef CFG_ENABLE_LOGDATA
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      blLoadResult = DBASE_blLoadTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (UBYTE *)&DBASE_DEF_##_TABLE_NAME_, \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_), \
        (BOOL)(Table##_TABLE_NAME_ != TableLoggingData)); \
      break;
#else
#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
    case Table##_TABLE_NAME_: \
      blLoadResult = DBASE_blLoadTableA((UBYTE *)&_TABLE_NAME_, \
        sizeof(TS_DAT_##_TABLE_NAME_), \
        (UBYTE *)&DBASE_DEF_##_TABLE_NAME_, \
        (DBASE_ADR_##_TABLE_NAME_), \
        (DBASE_VER_##_TABLE_NAME_), \
         FALSE); \
      break;
#endif
#endif
#endif
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE
#undef DBASE_CFG_FIELD_DEFINITIONS
    case DBASE_DUMMY:
    default:
      // Do nothing
      break;
  }
  return blLoadResult;
}

///////////////////////////////////////////////////////////////////////////////
#if ((TARGET_DEVICE!=DEVICE_BASICVALUE) && (TARGET_DEVICE != DEVICE_SUPERVALUE)&& (TARGET_DEVICE!=DEVICE_INTERLEAVED)&&(TARGET_DEVICE!= DEVICE_SVHP))
/*
**  Description : Write a table to non-volatile memory.
**                The version id of the table will be copied into non-volatile memory.
**                The checksum will be calculated on the whole table
**                including the version id.
**  Parameters  : pubTable: Pointer to the cache.
**                ubTableSize: Size of the table.
**              [ pubTableHelper: Sizes of the individual fields in the table. ]
**                usAddress: Address of the table in the eeprom.
**                tVersion: Version id of the table.
**  Returns     : -
*/
static void DBASE_vSaveTableA(UBYTE *pubTable,
                       const UBYTE ubTableSize,
#ifdef DBASE_NEED_HELPER
                       const UBYTE TARGET_MEM_ROM *pubTableHelper,
#endif
                       const USHORT usAddress,
                       const T_DBASE_VERSION tVersion)
{
  //Swap data before storage
  DBASE_vSwap(pubTable,pubTableHelper);

  //Copy version number
  DBASE_GET_HEADER(pubTable)->tVersion = tVersion;

  //Determine checksum
  DBASE_GET_HEADER(pubTable)->tChecksum =
      DBASE_CHECK((pubTable+DBASE_HEADER_CHECKSUM_SIZE),0,(UBYTE)(ubTableSize+DBASE_HEADER_VERSION_SIZE));

  //Try to write to non-volatile memory
  if (MEMORY_teWrite(pubTable, usAddress, (UBYTE)(ubTableSize+DBASE_HEADER_SIZE), MEMTYPE_NVRAM)!=RESULT_SUCCESS)
  {
#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
    (void)printf("%s: Version mismatch (0x%8X)\n",
            __MODULE__, pubTable);
#endif
  }

  //Swap data back to native endian mode
  DBASE_vSwap(pubTable,pubTableHelper);
}
#endif
///////////////////////////////////////////////////////////////////////////////

/*
**  Description : Read a table from non-volatile memory.
**                The checksum will be checked on the whole table including the
**                version id.
**                The version id of the current table will be checked
**                with non-volatile memory.
**                If either of the above actions fails (read/version/checksum) the
**                default values will be used.
**                Next each field will be checked against the min/max boundaries
**                the value will be clipped to the min/max value if it exceeds the
**                boundary.
**  Parameters  : pubTable: Pointer to the cache.
**                ubTableSize: Size of the table.
**                pubTableDefault: Default values for the fields
**              [ pubTableMin: Minimum boundary values for the fields ]
**              [ pubTableMax: Maximum boundary values for the fields ]
**              [ pubTableHelper: Sizes of the individual fields in the table. ]
**                usAddress: Address of the table in the eeprom.
**                tVersion: Version id of the table.
**  Returns     : -
**  Note        : The min/max boundary check cannot deal with arrays/structs/unions
**                as datatype... Do we need this?
*/
static BOOL DBASE_blLoadTableA(UBYTE *pubTable,
  const UBYTE ubTableSize,
  const UBYTE TARGET_MEM_ROM *pubTableDefault,
#ifndef DBASE_DISABLE_MIN_MAX
  const UBYTE TARGET_MEM_ROM *pubTableMin,
  const UBYTE TARGET_MEM_ROM *pubTableMax,
#endif
#ifdef DBASE_NEED_HELPER
  const UBYTE TARGET_MEM_ROM *pubTableHelper,
#endif
  const USHORT usAddress,
  const T_DBASE_VERSION tVersion,
  const BOOL blLoadDefaultAllowed)
{
  UBYTE ubChecksum;
  BOOL  blVerifyOk = TRUE;
#ifndef DBASE_DISABLE_MIN_MAX
  UBYTE ubFieldSize;
#endif

  //Try to read from non-volatile memory
  if (MEMORY_teReadTable(pubTable, usAddress, (UBYTE)(ubTableSize+DBASE_HEADER_SIZE), MEMTYPE_NVRAM)==RESULT_SUCCESS)
  {
    ubChecksum = DBASE_CHECK((pubTable+DBASE_HEADER_CHECKSUM_SIZE),0,(UBYTE)(ubTableSize+DBASE_HEADER_VERSION_SIZE));

    //Swap data to native endian mode
    DBASE_vSwap(pubTable,pubTableHelper);

    //Checksum
    if (DBASE_GET_HEADER(pubTable)->tChecksum != ubChecksum)
    {
      //Checksum mismatch
      if(blLoadDefaultAllowed)
      {
        (void)MEMORY_teRead(pubTable+DBASE_HEADER_SIZE, (T_ADDRESS_SPACE)pubTableDefault, ubTableSize, MEMTYPE_FLASH);
      }
      else
      {
        DBASE_vSetLogDataIncompleteFlag();
      }
      blVerifyOk = FALSE;

#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
    (void)printf("%s: Checksum mismatch (0x%8X)\n",
            __MODULE__, pubTable);
#endif
    }
    else if (DBASE_GET_HEADER(pubTable)->tVersion != tVersion) //Version
    {
      //Version mismatch
      (void)MEMORY_teRead(pubTable+DBASE_HEADER_SIZE, (T_ADDRESS_SPACE)pubTableDefault, ubTableSize, MEMTYPE_FLASH);
      blVerifyOk = FALSE;

#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
    (void)printf("%s: Version mismatch (0x%8X)\n",
            __MODULE__, pubTable);
#endif
    }
  }
  else
  {
#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
    (void)printf("%s: Non-volatile memory read failed (0x%8X)\n",
            __MODULE__, pubTable);
#endif

    //Copy default values
    (void)MEMORY_teRead(pubTable+DBASE_HEADER_SIZE, (T_ADDRESS_SPACE)pubTableDefault, ubTableSize, MEMTYPE_FLASH);
    blVerifyOk = FALSE;
  }

#ifndef DBASE_DISABLE_MIN_MAX
  //Check boundaries for each parameter
  pubTable+=DBASE_HEADER_SIZE;
  while (*pubTableHelper)
  {
    // Remove the Sign Identification bit to get the fieldsize
    ubFieldSize = (*pubTableHelper) & 127;

    //Check minimum boundary
    if (DBASE_sbCompare(pubTable,pubTableMin,(*pubTableHelper))<0)
    {
      (void)MEMORY_teRead(pubTable, (T_ADDRESS_SPACE)pubTableMin, ubFieldSize, MEMTYPE_FLASH);

#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
      (void)printf("%s: Minimum boundary violation (0x%8X)\n",
              __MODULE__, pubTable);
#endif
    }

    //Check maximum boundary
    if (DBASE_sbCompare(pubTable,pubTableMax,(*pubTableHelper))>0)
    {
      (void)MEMORY_teRead(pubTable, (T_ADDRESS_SPACE)pubTableMax, ubFieldSize, MEMTYPE_FLASH);

#if defined(DEBUG_DBASE) && ! defined(NDEBUG)
      (void)printf("%s: Maximum boundary violation (0x%8X)\n",
              __MODULE__, pubTable);
#endif
    }

    //Next element in the struct
    pubTable += ubFieldSize;
    pubTableDefault += ubFieldSize;
    pubTableMin += ubFieldSize;
    pubTableMax += ubFieldSize;
    pubTableHelper++;
  }
#endif
  return blVerifyOk;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef WIN32 
///////////////////////////////////////////////////////////////////////////////

/*
**  Description : Write all tables to non-volatile memory.
**  Parameters  : -
**  Returns     : -
*/
void DBASE_vSaveDatabase(void)
{
#define DBASE_CFG_FIELD_DEFINITIONS

#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  DBASE_vSaveTableB(Table##_TABLE_NAME_);
#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE

#undef DBASE_CFG_FIELD_DEFINITIONS
}
#endif

///////////////////////////////////////////////////////////////////////////////

void DBASE_vInit(void)
{
#if ((TARGET_DEVICE == DEVICE_INTERLEAVED)||(TARGET_DEVICE == DEVICE_SVHP))
  DBASE_STM32_vInit();
#endif
  (void)DBASE_usLoadDatabase();
}

USHORT DBase_usRetrieveVerificationResult(void)
{
  return DBASE_usLoadDatabase();
}

UBYTE DBASE_ubRetrieveTableCRC(TE_DBASE_TABLELIST teTable)
{
  UBYTE ubChecksum = 0;
  switch (teTable)
  {
    case TableDriverParameters:
      ubChecksum = DBASE_GET_HEADER(&DriverParameters)->tChecksum;
      break;
    case TableLampParameters:
      ubChecksum = DBASE_GET_HEADER(&LampParameters)->tChecksum;
      break;
    default:
      break;
  }
  return (ubChecksum);
}

void DBASE_vResetEEpromTable(TE_DBASE_TABLELIST teTable)
{
  switch(teTable)
  {    
    case TableOperationalData:
      (void)MEMORY_teRead((UBYTE *)((UBYTE *)(&OperationalData) + DBASE_HEADER_SIZE), (T_ADDRESS_SPACE)(&DBASE_DEF_OperationalData), sizeof(TS_DAT_OperationalData), MEMTYPE_FLASH);    
      //DBASE_vSaveTable(OperationalData);
     // KERNEL_vPostEvent(EVENTID_UPDATE_OPRDATA);
      break;

    case TableCalibData:
      (void)MEMORY_teRead((UBYTE *)((UBYTE *)(&CalibData) + DBASE_HEADER_SIZE), (T_ADDRESS_SPACE)(&DBASE_DEF_CalibData), sizeof(TS_DAT_CalibData), MEMTYPE_FLASH);    
      //DBASE_vSaveTable(CalibData); //Quick and dirty solution since Calibration table will only be requested to reset during manufacturing, during whil the driver is expected to be much less busy
      break;

    case TableLoggingData:   
      (void)MEMORY_teRead((UBYTE *)((UBYTE *)(&LoggingData) + DBASE_HEADER_SIZE), (T_ADDRESS_SPACE)(&DBASE_DEF_LoggingData), sizeof(TS_DAT_LoggingData), MEMTYPE_FLASH);    
      //DBASE_vSaveTableB(teTable);
     // KERNEL_vPostEvent(EVENTID_UPDATE_LOGDATA);
      break;   

#ifdef CFG_ENABLE_IMAGELIFE
    case TableOperationalDataBackup:   
      (void)MEMORY_teRead((UBYTE *)((UBYTE *)(&OperationalDataBackup) + DBASE_HEADER_SIZE), (T_ADDRESS_SPACE)(&DBASE_DEF_OperationalDataBackup), sizeof(TS_DAT_OperationalDataBackup), MEMTYPE_FLASH);    
      //DBASE_vSaveTable(OperationalDataBackup);
     // KERNEL_vPostEvent(EVENTID_UPDATE_OPRDATA_BACKUP);
      break;  
#endif

    default:
      break;      
  }

  DBASE_vSaveTableB(teTable);
}


#ifdef CFG_ENABLE_IMAGELIFE

TE_DBASE_CHECK_RESULT DBASE_usCheckBackup(USHORT usVerifyCRCResult)
{
  USHORT usIndicator = 0x0001;
  UBYTE *pubTable = (void *)0;

  //0--not update; 1--backup update to op; 2--op update to backup
  TE_DBASE_CHECK_RESULT ubCheckResult = DBASE_NOT_UPDATE;

#define DBASE_CFG_FIELD_DEFINITIONS

#define OPERATION_TABLE_INDEX      0x0004
#define OPERATION_BACKUP_TABLE_INDEX  0x0020

#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  if(usVerifyCRCResult & OPERATION_TABLE_INDEX)\
  {\
    if(!(usVerifyCRCResult & OPERATION_BACKUP_TABLE_INDEX))\
    {\
      if(usIndicator == OPERATION_TABLE_INDEX)\
      {\
        pubTable = (UBYTE *)(&_TABLE_NAME_);\
      }\
      if(usIndicator == OPERATION_BACKUP_TABLE_INDEX)\
      {\
        (void)MEMORY_teRead(pubTable, (T_ADDRESS_SPACE)(&_TABLE_NAME_), sizeof(TS_DAT_##_TABLE_NAME_) + DBASE_HEADER_SIZE, MEMTYPE_RAM);\
        ubCheckResult = DBASE_UPDATE_OPERATION;\
      }\
    }\
  }\
  else\
  {\
    if(usIndicator == OPERATION_TABLE_INDEX)\
    {\
      pubTable = (UBYTE *)(&_TABLE_NAME_);\
    }\
    if(usIndicator == OPERATION_BACKUP_TABLE_INDEX)\
    {\
      (void)MEMORY_teRead(&_TABLE_NAME_, (T_ADDRESS_SPACE)pubTable, sizeof(TS_DAT_##_TABLE_NAME_) + DBASE_HEADER_SIZE, MEMTYPE_RAM);\
      ubCheckResult = DBASE_UPDATE_OPERATION_BACKUP;\
    }\
  }\
  usIndicator <<= 1;

#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE

#undef OPERATION_TABLE_INDEX
#undef OPERATION_BACKUP_TABLE_INDEX

#undef DBASE_CFG_FIELD_DEFINITIONS

  return ubCheckResult;
}
#endif //CFG_ENABLE_IMAGELIFE

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/*
**  Description : Read all tables from non-volatile memory.
**  Parameters  : -
**  Returns     : -
*/

static USHORT DBASE_usLoadDatabase(void)
{
  USHORT usIndicator = 0x01;
  USHORT usVerifyCRCResult = 0x00;
#define DBASE_CFG_FIELD_DEFINITIONS

#define DBASE_BEGIN_TABLE(_TABLE_NAME_,_TABLE_ADDRESS_,_TABLE_VERSION_) \
  if (DBASE_blLoadTableB(Table##_TABLE_NAME_) == FALSE)                  \
  {                                       \
    usVerifyCRCResult |= usIndicator;     \
  }                                       \
  usIndicator <<= 1;

#define DBASE_ADD_FIELD(_TABLE_NAME_,_FIELD_NAME_,_FIELD_TYPE_,_FIELD_DEFAULT_,_FIELD_MIN_,_FIELD_MAX_,_FIELD_COMMENT_,_FIELD_HELP_,FIELD_CATEGORY)
#define DBASE_END_TABLE(_TABLE_NAME_)
#include "dbase_cfg.h"
#undef DBASE_BEGIN_TABLE
#undef DBASE_ADD_FIELD
#undef DBASE_END_TABLE

#undef DBASE_CFG_FIELD_DEFINITIONS

  return usVerifyCRCResult;
}

#ifdef CFG_ENABLE_LOGDATA
static void DBASE_vSetLogDataIncompleteFlag(void)
{
  UBYTE ubState = DBASE_tGetField(LoggingData,StateDuringLastErr);
  ubState |= LOGDATA_CRC_MISMATCH_FLAG;
  DBASE_vSetField(LoggingData, StateDuringLastErr, ubState);
}
#endif

/*
**  Description : Compares two buffers, byte by byte. Little-endian mode starts the
**                comparison from the tail of the buffer. Big-endian mode starts the
**                comparison from the head of the buffer.
**  Parameters  : pubS1: Pointer to the first buffer.
**                pubS2: Pointer to the second buffer.
**                ubN: Length of both buffers.
**  Returns     : short byte greater than zero: pubS1 contains a value greater than pubS2
**                short byte equal to zero: both buffers are equal
**                short byte less than zero: pubS1 contains a value less than pubS2
*/
#ifndef DBASE_DISABLE_MIN_MAX
static SBYTE DBASE_sbCompare(const UBYTE *pubS1, const UBYTE TARGET_MEM_ROM *pubS2, UBYTE ubN)
{
  UBYTE ubVal1, ubVal2;

  // When values are of signed type then convert to unsigned (only for the MSB)
  if ((ubN & 128) == 0)
  {
    ubVal1 = ubVal2 = 0;
  }
  else
  {
    ubVal1 = ubVal2 = 128;
  }

  // Remove Sign Identification bit to get fieldsize
  ubN &= 127;

  while (ubN>0)
  {
    ubN--;

#if (TARGET_BYTE_ORDER)==(__LITTLE_ENDIAN)
    //Little endian mode: highest address means most significant, start comparison at the tail
    ubVal1 += pubS1[ubN];
    ubVal2 += pubS2[ubN];
#else
    //Big endian mode: lowest address means most significant, start comparison at the head
    ubVal1 += *pubS1;
    ubVal2 += *pubS2;

    pubS1++;
    pubS2++;
#endif

    if (ubVal1 > ubVal2)
    {
      //Val1 greater than Val2
      return 1;
    }
    if (ubVal1 < ubVal2)
    {
      //Val1 less than Val2
      return -1;
    }

    // Since we only need to correct the MSB (if type of values is signed), we can use 0 here
    ubVal1=0;
    ubVal2=0;

  }

  //Val1 and Val2 are equal.
  return 0;
}
#endif

///////////////////////////////////////////////////////////////////////////////

#if 0
static void DBASE_vSwap(UBYTE *pubTable, const UBYTE TARGET_MEM_ROM *pubTableHelper)
{
  //Check boundaries for each parameter
  pubTable+=DBASE_HEADER_SIZE;
  while (*pubTableHelper)
  {
    // Remove the Sign Identification bit to get the fieldsize
    UBYTE ubFieldSize = (*pubTableHelper) & 127;

    //Swap element
    SWAP_vSwap(pubTable,ubFieldSize);

    //Next element in the struct
    pubTable += ubFieldSize;
    pubTableHelper++;
  }
}
#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

