/******************************************************************************/
/*                                                                            */
/*    $Workfile::   UartIO.h                                             $    */
/*                                                                            */
/*    $Revision:: 2906                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2011-08-01 14:24:43 +0800 (周一, 01 八月 2011)     $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
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

#ifndef TERMINAL_H
#define TERMINAL_H

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

UBYTE TERMINAL_ubGetkey(void);
void TERMINAL_vWriteChar (SBYTE  sbChar);
void TERMINAL_vWriteString (const SBYTE* psbStr);
void TERMINAL_vWriteUSHORT (USHORT usFigure);
void TERMINAL_vWriteSSHORT (SSHORT ssFigure);

#if (defined(CFG_ENABLE_ILC_MONITOR) || defined(CFG_ENABLE_ILC_ANALYSE) )
void TERMINAL_vXkWriteSSHORTArray (const SSHORT*  pusXk, const char* pDescription, USHORT  usLength);
#endif
#if (defined(CFG_ENABLE_ILC_ANALYSE) || defined(CFG_ENABLE_ILC_IDENTIFICATION))
void TERMINAL_vXkWriteUSHORTArray (const USHORT*  pusXk, const char* pDescription, USHORT  usLength);
#endif

#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


