/******************************************************************************/
/*                                                                            */
/*    $Workfile::   app.h                                                $    */
/*                                                                            */
/*    $Revision:: 3203                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2012-02-02 20:21:43 +0800 (周四, 02 二月 2012)     $    */
/*                                                                            */
/*      $Author:: bobbie.fan@philips.com                                 $    */
/*                                                                            */
/*        Owner::                                                             */
/*                                                                            */
/*  Description::                                                             */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#ifndef APP_H
#define APP_H

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
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
void APP_vStart(void);
/*
** Description: Start application
** Parameters: -
** Returns: -
*/

void APP_vCoordinator(TE_EVENT_ID teEventID);
/*
** Description: Coordinator task for application.
** Parameters: kernel events that need to be handled.
** Returns: -
*/

BOOL APP_blSelectWaveshape(UBYTE ubWaveshapeId);
/*
** Description: Select requested WS and store in operational data as default.
** Parameters: WS ID
** Returns: successful or not
*/

#endif //APP_H
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

