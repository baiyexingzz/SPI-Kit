/******************************************************************************/
/*                                                                            */
/*    $Workfile::   statemachine.h                                       $    */
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
#ifndef STATEMACHINE_H
#define STATEMACHINE_H
/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Ignition check status */
#define STATUS_IGNITION_OK     (0x00)
#define STATUS_HOT_RESTRIKE    (0x01)
#define STATUS_SCI_DISABLED    (0x02)
#define STATUS_SUPPLY_LOW      (0x04)
#define STATUS_SUPPLY_HIGH     (0x08)



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
void SM_vInit(void);
/*
** Description: Initialize state machine.
** Parameters: -
** Returns: -
*/

void SM_vStart(void);
/*
** Description: State machine starts (standby).
** Parameters: -
** Returns: -
*/

TE_RESULT SM_teEventHandler( void);
/*
** Description: Event handler which can trigger SM's state change.
** Parameters: event listed in TE_STATEMACHINE_EVENT
** Returns: handler result
*/

void SM_vSetSleepModeTimeOutState(void);


void SM_vTimeoutEventHandler(void );






#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

