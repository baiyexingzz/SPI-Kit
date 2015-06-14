/******************************************************************************/
/*                                                                            */
/*    $Workfile::   CheckSignal.c                                        $    */
/*                                                                            */
/*    $Revision:: 2573                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::   Sep 11 2006 17:08:56                                 $    */
/*                                                                            */
/*        $Date:: 2011-03-16 16:56:40 +0800 (周三, 16 三月 2011)     $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
/*                                                                            */
/*        Owner::   Kyle Ye                                                   */
/*                                                                            */
/*  Description::   Check Signal can be used to test signals/vars to comply   */
/*                  to a test (LT, LE, EQ, NE , GE, GT) for an x amount of    */
/*                  times before 1 is returned.                               */
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

#include "CheckSignal.h"

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

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
BOOL CheckSignal_blCompare(TE_COMPARE_TYPE teCmpType,
                           TS_CHECKSIGNAL* ptsSignal,
                           T_CHECKSIGNAL_VALUE tCmpVal,
                           T_CHECKSIGNAL_COUNTER tMaxCnt,
                           UBYTE ubSeqMode)
{
  BOOL blCmpResult;

  switch(teCmpType)
  {
    case cmpEQ:
      blCmpResult = (BOOL)(ptsSignal->tValue == tCmpVal);
      break;
    case cmpNE:
      blCmpResult = (BOOL)(ptsSignal->tValue != tCmpVal);
      break;
    case cmpLT:
      blCmpResult = (BOOL)(ptsSignal->tValue < tCmpVal);
      break;
    case cmpGT:
      blCmpResult = (BOOL)(ptsSignal->tValue > tCmpVal);
      break;
    case cmpLE:
      blCmpResult = (BOOL)(ptsSignal->tValue <= tCmpVal);
      break;
    case cmpGE:
      blCmpResult = (BOOL)(ptsSignal->tValue >= tCmpVal);
      break;
    default:
      blCmpResult = FALSE;
      break;
  }

  if (blCmpResult)
  {
    if (ptsSignal->tValidCount < tMaxCnt-1)
    {
      ptsSignal->tValidCount++;
    }
    else
    {
      return TRUE;
    }
  }
  else
  {
    if (ubSeqMode == CHECKSIGNAL_COUNT_MODE)
    {
      if (ptsSignal->tValidCount > 0)
      {
        ptsSignal->tValidCount--;
      }
    }
    else if (ubSeqMode == CHECKSIGNAL_IN_SEQ_MODE)
    {
      ptsSignal->tValidCount=0;
    }
  }

  return FALSE;
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


