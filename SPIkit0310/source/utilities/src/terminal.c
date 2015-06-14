/******************************************************************************/
/*                                                                            */
/*    $Workfile::   UartIO.c                                             $    */
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
/*        Owner::   Erik de Wilde                                             */
/*                                                                            */
/*  Description::   This module is responsible for                            */
/*                                                                            */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#if (!defined(NDEBUG) \
  || defined(CFG_ENABLE_ILC_IDENTIFICATION) \
  || defined(CFG_ENABLE_ILC_MONITOR) \
  || defined(CFG_ENABLE_ILC_ANALYSE) \
  )

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

#include <assert.h>
#include <string.h>

#include "platform.h"
#include "stdtypes.h"

#include "terminal.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

#define CR     "\r"

#if (TARGET_DEVICE == DEVICE_DD2103)
#define TERMINAL_blRxEmpty() (!(U0LSR & 0x01))
#define TERMINAL_blTxEmpty() (U0LSR & 0x20)
#define TERMINAL_ubRxData()  (U0RBR)
#define TERMINAL_vTxData(ubData) U0THR=(UBYTE)(ubData)

#elif ((TARGET_DEVICE == DEVICE_INTERLEAVED)||(TARGET_DEVICE == DEVICE_SVHP))
#define TERMINAL_blRxEmpty() (!PERI_BITBAND_ALIAS(&USART1->SR, 5))
#define TERMINAL_blTxEmpty() (PERI_BITBAND_ALIAS(&USART1->SR, 7))
#define TERMINAL_ubRxData()  (UBYTE)(USART1->DR)
#define TERMINAL_vTxData(ubData) USART1->DR=(UBYTE)(ubData)

#else
#error "Terminal module for target device not implemented."
#endif

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
static void vUSHORT_To_String (USHORT  usFigure, SBYTE*  psbString);
static void vSSHORT_To_String (SSHORT  ssFigure, SBYTE*  psbString);

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/

static void vUSHORT_To_String (USHORT  usFigure, SBYTE*  psbString)
{
  USHORT usDevider = 0;
  USHORT usIdx     = 0;
  USHORT usN       = 10000;
  BOOL   bNoValue  = TRUE;

  while (usN != 0)
  {
    usDevider = usFigure / usN;
    usFigure  = (USHORT)(usFigure - usN * usDevider);

    if (usDevider == 0)
    {
      if (!bNoValue)
      {
        psbString [usIdx++] = ((SBYTE)usDevider) + '0';
      }
    }
    else
    {
      if (bNoValue)
      {
        bNoValue = FALSE;
      }

      psbString [usIdx++] = ((SBYTE)usDevider) + '0';
    }

    usN = usN / 10;
  }

  if (usIdx == 0)
  {
    psbString [usIdx++] = '0';
  }

  psbString [usIdx] = '\0';
}

static void vSSHORT_To_String (SSHORT  ssFigure, SBYTE*  psbString)
{
  USHORT usIdx     = 0;

  if (ssFigure < 0)
  {
    psbString [usIdx++] = '-';
    ssFigure = -ssFigure;
  }

  vUSHORT_To_String((USHORT)ssFigure, &(psbString[usIdx]));
}

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
UBYTE TERMINAL_ubGetkey(void)
{
  while(TERMINAL_blRxEmpty()); //pending until data received

  return TERMINAL_ubRxData();
}
///////////////////////////////////////////////////////////////////////////////
void TERMINAL_vWriteChar (SBYTE  sbChar)
{
  #define CARRIAGE_RETURN  0x0D

  if (sbChar == '\n')
  {
    while(!TERMINAL_blTxEmpty()); //pending until transmit completed (Tx empty)
    TERMINAL_vTxData(CARRIAGE_RETURN);
  }
  while(!TERMINAL_blTxEmpty()); //pending until transmit completed (Tx empty)
  TERMINAL_vTxData(sbChar);

  #undef CARRIAGE_RETURN
}
///////////////////////////////////////////////////////////////////////////////
void TERMINAL_vWriteString (const SBYTE* psbStr)
{
  USHORT usN = 0;

  while (psbStr[usN] != '\0')
  {
    TERMINAL_vWriteChar (psbStr[usN++]);
  }
}
///////////////////////////////////////////////////////////////////////////////
void TERMINAL_vWriteUSHORT (USHORT usFigure)
{
  SBYTE  sbFigure[10] = {'\0'};

  vUSHORT_To_String(usFigure, sbFigure);

  TERMINAL_vWriteString (sbFigure);
}
///////////////////////////////////////////////////////////////////////////////
void TERMINAL_vWriteSSHORT (SSHORT ssFigure)
{
  SBYTE  sbFigure[10] = {'\0'};

  vSSHORT_To_String(ssFigure, sbFigure);

  TERMINAL_vWriteString (sbFigure);
}
///////////////////////////////////////////////////////////////////////////////
#if (defined(CFG_ENABLE_ILC_MONITOR) || defined(CFG_ENABLE_ILC_ANALYSE))
void TERMINAL_vXkWriteSSHORTArray (const SSHORT*  pusXk, const char* pDescription, USHORT  usLength)
{
  USHORT  usN;
  SBYTE   sbFigure[10] = {'\0'};

  TERMINAL_vWriteString ((SBYTE*)pDescription);
  TERMINAL_vWriteChar ('\n');

  for (usN = 0; usN != usLength; usN++)
  {
    vSSHORT_To_String(pusXk [usN], sbFigure);

    TERMINAL_vWriteString (sbFigure);
    TERMINAL_vWriteChar ('\n');
  }
}
#endif
///////////////////////////////////////////////////////////////////////////////
#if (defined(CFG_ENABLE_ILC_ANALYSE) || defined(CFG_ENABLE_ILC_IDENTIFICATION))
void TERMINAL_vXkWriteUSHORTArray (const USHORT* pusXk, const char* pDescription, USHORT  usLength)
{
  USHORT  usN;
  SBYTE   sbFigure[10] = {'\0'};

  TERMINAL_vWriteString ((SBYTE*)pDescription);
  TERMINAL_vWriteChar ('\n');

  for (usN = 0; usN != usLength; usN++)
  {
    vUSHORT_To_String(pusXk [usN], sbFigure);

    TERMINAL_vWriteString (sbFigure);
    TERMINAL_vWriteChar ('\n');
  }
}
#endif
///////////////////////////////////////////////////////////////////////////////
#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

