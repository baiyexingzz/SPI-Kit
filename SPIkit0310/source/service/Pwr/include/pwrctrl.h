/******************************************************************************/
/*                                                                            */
/*    $Workfile::   PWRCTRL.h                                            $    */
/*                                                                            */
/*    $Revision:: 3878                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2012-11-01 09:59:46 +0800 (周四, 01 十一月 2012)  $    */
/*                                                                            */
/*      $Author:: bobbie.fan@philips.com                                 $    */
/*                                                                            */
/*        Owner::   Erik de Wilde                                             */
/*                                                                            */
/*  Description::   This module is responsible for handling the whole         */
/*                  power control process of a UHP(DLP) Lamp.                 */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#ifndef PWRCTRL_H
#define PWRCTRL_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

 
/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
// request power
#define MAX_REQPWR           (USHORT)(1000)  /// 
#define MIN_REQPWR           (USHORT)(1)  /// 
#define PWR_IOUT_SCALE                                   (256)
// 1.2 x normal duty = 307x normal duty /256
#define PWRCTRL_USLEADINGDUTY(NORMALDUTY)                       (USHORT)(307*NORMALDUTY/256)
/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/




void PWRCTRL_vInit(void);
/*
** Description: Initialisation of power control, usually done at startup
** Parameters:  -
** Returns:     -
*/
void PWRCTRL_vSetPeriodForSCT0(USHORT usPeriod);
void PWRCTRL_vSetRForSCT0(USHORT usRup,USHORT usRdown);
void PWRCTRL_vSetGForSCT0(USHORT usGup,USHORT usGdown);
void PWRCTRL_vSetBForSCT0(USHORT usBup,USHORT usBdown);
void PWRCTRL_vSetRedDutyForSCT1(USHORT usRedDuty );
void PWRCTRL_vSetBlueDutyForSCT1(USHORT usBlueDuty);
void PWRCTRL_vSetGreenDutyForSCT1(USHORT usGreenDuty);
void PWRCTRL_vSetSenseDutyForSCT1(USHORT usSenseDuty);
void PWRCTRL_vSetCalDutyForSCT1(USHORT usCalDuty);
void SCT0_vStart(void);
void SCT1_vStart(void);
void SCT0_vStop(void);
void SCT1_vStop(void);


void PWRCTRL_vGetColorSEQ(void);
void Pwrctrl_vSetKengTime(UBYTE keng);

void  Pwrctrl_vGerKengTime();

void Pwrctrl_vSetSubframePeriod(UBYTE subH, UBYTE subL);
void Pwrctrl_vGetSubframePeriod();


void SCT1_IRQHandlerzz(void);


bool Pwrctrl_vStoreColorSeqToEE(UBYTE unit);
void Pwrctrl_vReadColorSeqFromEE(UBYTE unit);

int PWRCTRL_InGetFrameFlag(void);
int PWRCTRL_InGetFrameOldFlag(void);

bool PWRCTRL_bCheckInt();

void PWRCTRL_vSetFrameFlagDone(void);

#endif










/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

