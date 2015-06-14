/******************************************************************************/
/*                                                                            */
/*    $Workfile::   logdata.c                                            $    */
/*                                                                            */
/*    $Revision:: 2929                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2011-08-15 11:20:20 +0800 (周一, 15 八月 2011)     $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
/*                                                                            */
/*        Owner::   KyleYe                                                    */
/*                                                                            */
/*  Description::   This module is responsible for logging data into EEPROM.  */
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
#include "stdtypes.h"
#include "platform.h"

#ifdef CFG_ENABLE_LOGDATA
#include "kernel.h"
#include "statemachine.h"
#include "pwrctrl.h"

#include "logdata.h"
#include "dbase.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define DBASE_vSetLogData(field,val) DBASE_vSetField(LoggingData,field,(val))
#define DBASE_tGetLogData(field)     DBASE_tGetField(LoggingData,field)
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

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/

void LOGDATA_vLampOnTime(void)
{
  USHORT usCounter = DBASE_tGetLogData(LampOnTime);
  if(usCounter < MAX_USHORT)
  {
    //Increase the counter only if counter hasn't achieved the max limitation
    DBASE_vSetLogData(LampOnTime, (USHORT)(usCounter + 1));
  }
}

void LOGDATA_vStandbyTime(void)
{
  USHORT usCounter = DBASE_tGetLogData(StandbyTime);
  if(usCounter < MAX_USHORT)
  {
    //Increase the counter only if counter hasn't achieved the max limitation
    DBASE_vSetLogData(StandbyTime, (USHORT)(usCounter + 1));
  }
}

void LOGDATA_vSuccessfulIgnTimes(void)
{
  USHORT usCounter = DBASE_tGetLogData(IgnSucceedCnt);
  if(usCounter < MAX_USHORT)
  {
    //Increase the counter only if counter hasn't achieved the max limitation
    DBASE_vSetLogData(IgnSucceedCnt, (USHORT)(usCounter + 1));
  }
}

void LOGDATA_vFailureStatistics(TS_FAULT_STATUS_INFO *ptsFailureInfo)
{
  //Get the 1st ErrCnt byte(VlaMaxErrCnt) as the base addr of ErrCnt array
  UBYTE* pubErrCntArrayBase = (UBYTE*)(&DBASE_tGetLogData(VlaMaxErrCnt));

  //Combined state: Low 4 bits - App State, High 3 bits - Power State, highest: log data CRC err
  UBYTE ubCombinedState = (UBYTE)(DBASE_tGetLogData(StateDuringLastErr) & 0x80);
  ubCombinedState |= (UBYTE)((((UBYTE)ptsFailureInfo->tePwrState) << 4) & 0x70);
  ubCombinedState |= (UBYTE)(((UBYTE)ptsFailureInfo->teDriverState) & 0x0f);  

//Increase corresponding failure counter
  if(pubErrCntArrayBase[(UBYTE)ptsFailureInfo->teFaultMode - 1] < MAX_UBYTE)
  {
    pubErrCntArrayBase[(UBYTE)ptsFailureInfo->teFaultMode - 1]++;
  }
  
  //Update recent 3 failure occured
  DBASE_vSetLogData(Last3ErrCode, DBASE_tGetLogData(Last2ErrCode)); //earliest
  DBASE_vSetLogData(Last2ErrCode, DBASE_tGetLogData(Last1ErrCode)); //middle
  DBASE_vSetLogData(Last1ErrCode, (UBYTE)ptsFailureInfo->teFaultMode); //latest
  
  //Update latest failure information
  DBASE_vSetLogData(VinDuringLastErr,   ptsFailureInfo->usVsupply);
  DBASE_vSetLogData(VlaDuringLastErr,   ptsFailureInfo->usVlamp);
  DBASE_vSetLogData(IsetDuringLastErr,  ptsFailureInfo->usIlamp);
  DBASE_vSetLogData(NTCDuringLastErr,   ptsFailureInfo->usNTC);
  DBASE_vSetLogData(StateDuringLastErr, ubCombinedState);
}
#if defined(CFG_ENABLE_12V_TEST)
void LOGDATA_v12VMeasurement(UBYTE ubAverage, UBYTE ubPeakPeak)
{
  DBASE_vSetLogData(Average12V,  ubAverage);
  DBASE_vSetLogData(PeakPeak12V, ubPeakPeak);
}
#endif
#endif /*CFG_ENABLE_LOGDATA*/

