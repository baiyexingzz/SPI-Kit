/******************************************************************************/
/*                                                                            */
/*    $Workfile::                                                        $    */
/*                                                                            */
/*    $Revision:: 538                                                    $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2015-02-09 16:02:46 +0800 (Mon, 09 Feb 2015)           $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                  $    */
/*                                                                            */
/*        Owner::   Wijnand Rietman                                           */
/*                                                                            */
/*  Description::   <Enter description here>                                  */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2006 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#ifndef _CRC_H
#define _CRC_H

#include "platform.h"
#include "stdtypes.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

#ifdef __cplusplus
  #define WIN32_APIENTRY extern "C"
#else
  #define WIN32_APIENTRY extern
#endif

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

/*UBYTE CRC_ubCrc8(const UBYTE *pubData, UBYTE ubCrc, UBYTE ubNrOfBytes);
WIN32_APIENTRY 
**  Description : Calculates the 8-bit CRC
**  Parameters  : ubCrc: CRC seed value, normally zero
**  Returns     : The 8-bit CRC
*/

/*WIN32_APIENTRY USHORT CRC_usCrc16(const UBYTE *pubData, USHORT usCrc, USHORT usNrOfBytes);

**  Description : Calculates the 16-bit CRC
**  Parameters  : ubCrc: CRC seed value, normally zero
**  Returns     : The 16-bit CRC
*/


#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


