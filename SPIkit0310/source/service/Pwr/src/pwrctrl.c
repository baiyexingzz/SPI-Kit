/******************************************************************************/
/*                                                                            */
/*    $Workfile::   pwrctrl.c                                            $    */
/*                                                                            */
/*    $Revision:: 4826                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-12-20 12:02:28 +0800 (周五, 20 十二月 2013)  $    */
/*                                                                            */
/*      $Author:: ricky.lu@philips.com                                   $    */
/*                                                                            */
/*        Owner::   Erik de Wilde                                             */
/*                                                                            */
/*  Description::   This module is responsible for handling the whole         */
/*                  pwrctrl process of an UHP(DLP) Lamp.                      */
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
#include "sct.h"
#include <assert.h>

#include "system.h"
#include "platform.h"
#include "stdtypes.h"

#include "pwrctrl.h"
#include "measure.h"
#include "statemachine.h"
#include "memory.h"
#include "io.h"

#include "app_usbd_cfg.h"
#include "cdc_vcom.h"
#include "instruction.h"




/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/


extern uint8_t g_rxBuff[512];

#define PWM_PERIOD               (100) //  (300khz at 72mhz) 18k

//#define 
#define SCT0_Period_DEFAULT (USHORT)(2000)
#define SCT0_R_UP_DEFAULT (USHORT)(500)
#define SCT0_R_DOWN_DEFAULT (USHORT)(1500)
#define SCT0_G_UP_DEFAULT (USHORT)(50)
#define SCT0_G_DOWN_DEFAULT (USHORT)(150)
#define SCT0_B_UP_DEFAULT (USHORT)(500)
#define SCT0_B_DOWN_DEFAULT (USHORT)(1500)
#define SCT1_R_DEFAULT (USHORT)(2000)
#define SCT1_G_DEFAULT (USHORT)(2000)
#define SCT1_B_DEFAULT (USHORT)(2000)
#define SCT1_SENSE_DEFAULT (USHORT)(2000)
#define SCT1_CAL_DEFAULT (USHORT)(2000)

#define  COLOR_RED (UBYTE)( 0x00)
#define  COLOR_GREEN (UBYTE)( 0x01)
#define  COLOR_BLUE (UBYTE)( 0x02)
#define  COLOR_YELLOW (UBYTE)( 0x03)
#define  COLOR_MAGENTA (UBYTE)( 0x04)
#define  COLOR_CYAN (UBYTE)( 0x05)
#define  COLOR_WHITE (UBYTE)( 0x06)
#define  COLOR_BLACK (UBYTE)( 0x07)

#define MAX_COLORSEQ (250)
#define EEPROM_ADDRESS      0x032000F0
#define COLOR_SEQ_BSAE0  0x032007BF
#define COLOR_SEQ_BSAE1  0x032009BF
#define COLOR_SEQ_BSAE2  0x03200BBF
#define COLOR_SEQ_BSAE3  0x03200DBF


UBYTE ubSEQbuff[512];

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

static USHORT SCT0_R_up;
static USHORT SCT0_R_down;
static USHORT SCT0_G_up;
static USHORT SCT0_G_down;
static USHORT SCT0_B_up;
static USHORT SCT0_B_down;
static USHORT SCT0_Period;

static USHORT SCT1_R_duty;
static USHORT SCT1_G_duty;
static USHORT SCT1_B_duty;
static USHORT SCT1_Sense_duty;
static USHORT SCT1_Cal_duty;

UBYTE ubColorBuff[MAX_COLORSEQ] = {0};
USHORT ubColorTimeBuff[MAX_COLORSEQ] = {0};
UBYTE kengTime = 3;
USHORT subFramePeriod = 16667;

int i_test = 0;
int subTimeCounter = 0;

int frameFlag = 1;//0 means subframe is low,which 1 means high
int frameFlagTemp = 0,frameFlagTemp2=0;
int frameFlagOld = 1;

int firstSub = 0;
int sensorCnt = 0;
int colorCutOut = 0;

int seqLength = 0;
static void SCT0_Init(void);
static void SCT1_Init(void);

extern bool vcom_enable;

extern UBYTE cmdRight;
extern UBYTE cmdError;
extern UBYTE cmdFormatError;
extern UBYTE eeError;


/*****************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/

void PWRCTRL_vInit(void)
{
	SCT0_Period = SCT0_Period_DEFAULT;
	SCT0_R_up = SCT0_R_UP_DEFAULT;
	SCT0_R_down = SCT0_R_DOWN_DEFAULT;
	SCT0_G_up = SCT0_G_UP_DEFAULT;
	SCT0_G_down = SCT0_G_DOWN_DEFAULT;
	SCT0_B_up = SCT0_B_UP_DEFAULT;
	SCT0_B_down = SCT0_B_DOWN_DEFAULT;
	
	SCT1_R_duty = SCT1_R_DEFAULT;
	SCT1_G_duty = SCT1_G_DEFAULT;
	SCT1_B_duty = SCT1_B_DEFAULT;
	SCT1_Sense_duty = SCT1_SENSE_DEFAULT;
	SCT1_Cal_duty = SCT1_CAL_DEFAULT;	

	SCT_vInit();
	
  SCT0_Init(); // sct0 1 should be initilized/started  before sct2 3 for red blue  
  SCT1_Init();

	SCT0_vStart();
	SCT1_vStart();
}

////
void PWRCTRL_vSetPeriodForSCT0(USHORT usPeriod){
	if(usPeriod>=500 && usPeriod<=2000){
		SCT0_Period = usPeriod;
		SCT0_vStop();
		//SCT0_Period = SCT0_Period*72;//
		SCT0_Init();
		SCT0_vStart();
	}
}
void PWRCTRL_vSetRForSCT0(USHORT usRup,USHORT usRdown){
	if(usRup<=usRdown && usRdown<=SCT0_Period){
		SCT0_R_up= usRup;
		SCT0_R_down= usRdown;
		SCT0_vStop();
		SCT0_vStart();
	}
}

void PWRCTRL_vSetGForSCT0(USHORT usGup,USHORT usGdown){
	if(usGup<=usGdown && usGdown<=SCT0_Period){
		SCT0_G_up= usGup;
		SCT0_G_down= usGdown;
		SCT0_vStop();
		SCT0_vStart();
	}
}

void PWRCTRL_vSetBForSCT0(USHORT usBup,USHORT usBdown){
	if(usBup<=usBdown && usBdown<=SCT0_Period){
		SCT0_B_up= usBup;
		SCT0_B_down= usBdown;
		SCT0_vStop();
		SCT0_vStart();
	}
}

void PWRCTRL_vSetRedDutyForSCT1(USHORT usRedDuty )
{
  if(usRedDuty<PWM_PERIOD){
    SCT1_R_duty = usRedDuty;
  }
  else
  	SCT1_R_duty = PWM_PERIOD/2;
	
	SCT1_vStop();
	SCT1_vStart();
  
}
void PWRCTRL_vSetBlueDutyForSCT1(USHORT usBlueDuty)
{
  if(usBlueDuty<PWM_PERIOD)
	  SCT1_B_duty = usBlueDuty;
  else
   	 SCT1_B_duty = PWM_PERIOD/2;  
	
	SCT1_vStop();
	SCT1_vStart();
}
void PWRCTRL_vSetGreenDutyForSCT1(USHORT usGreenDuty){
	 if(usGreenDuty<PWM_PERIOD)
		SCT1_G_duty = usGreenDuty;
	 else
	 	SCT1_G_duty = PWM_PERIOD/2;
	 
		SCT1_vStop();
		SCT1_vStart();
}
void PWRCTRL_vSetSenseDutyForSCT1(USHORT usSenseDuty){
	 if(usSenseDuty<PWM_PERIOD)
		SCT1_Sense_duty = usSenseDuty;
	 else
	 	SCT1_Sense_duty = PWM_PERIOD/2;
	 
		SCT1_vStop();
		SCT1_vStart();
}
void PWRCTRL_vSetCalDutyForSCT1(USHORT usCalDuty){
	 if(usCalDuty<PWM_PERIOD)
		SCT1_Cal_duty = usCalDuty;
	 else
	 	SCT1_Cal_duty = PWM_PERIOD/2;
	 
		SCT1_vStop();
		SCT1_vStart();
}

//////

void SCT0_vStart(void)
{
    LPC_SCT0->CTRL_L    |= (1 << 2);                      // halt it by clearing bit 2 of CTRL reg  

    LPC_SCT0->COUNT_U = 0;
    LPC_SCT0->STATE_L = 0;

    LPC_SCT0->MATCH[1].L     = SCT0_R_up - 1;                          // match 1 used for p1 duty cycle 
    LPC_SCT0->MATCHREL[1].L  = SCT0_R_up - 1;                      // match 1 reload used for p2 duty cycle 
    
    LPC_SCT0->MATCH[2].L     = SCT0_R_down - 1;                          // match 2 used for p3 duty cycle 
	LPC_SCT0->MATCHREL[2].L  = SCT0_R_down - 1;                      // match 2 reload used for p3 duty cycle 

    LPC_SCT0->MATCH[3].L     = SCT0_G_up - 1;                          // match 3 used for p4 duty cycle 
    LPC_SCT0->MATCHREL[3].L  = SCT0_G_up - 1;                      // match 3 reload used for p4 duty cycle 

    LPC_SCT0->MATCH[4].L     = SCT0_G_down - 1;                          // match 4 used for p5 duty cycle 
    LPC_SCT0->MATCHREL[4].L  = SCT0_G_down - 1;                      // match 4 reload used for p5 duty cycle 
                                                               
    LPC_SCT0->MATCH[5].L     = SCT0_B_up - 1;                          // match 5 used for p6 duty cycle 
    LPC_SCT0->MATCHREL[5].L  = SCT0_B_up - 1;                      // match 5 reload used for p6 duty cycle        

    LPC_SCT0->MATCH[6].L     = SCT0_B_down - 1;                          // match 6 used for p7 duty cycle 
    LPC_SCT0->MATCHREL[6].L  = SCT0_B_down - 1;                      // match 6 reload used for p7 duty cycle  

	LPC_SCT0->MATCH[7].L     = SCT0_Period - 10;                          // match 7 used for p8 duty cycle 
	LPC_SCT0->MATCHREL[7].L  = SCT0_Period - 10;                      // match 7 reload used for p8 duty cycle 

    LPC_SCT0->OUTPUT = 0x01;    //set out0
    LPC_SCT0->CTRL_L  &= ~(1 << 2);                      // unhalt it by clearing bit 2 of CTRL reg
}
void SCT1_vStart(void)
{
    LPC_SCT1->CTRL_L    |= (1 << 2);                      // halt it by clearing bit 2 of CTRL reg  

    LPC_SCT1->COUNT_U = 0;
    LPC_SCT1->STATE_L = 0;

    LPC_SCT1->MATCH[1].L     = 105 - 1;                          // match 1 used for p1 duty cycle 
    LPC_SCT1->MATCHREL[1].L  = 105 - 1;                      // match 1 reload used for p2 duty cycle 
    
    LPC_SCT1->MATCH[2].L     = 105 - 1;                          // match 2 used for p3 duty cycle 
    LPC_SCT1->MATCHREL[2].L  = 105 - 1;                      // match 2 reload used for p3 duty cycle 
	
    LPC_SCT1->MATCH[3].L     = 105 - 1;                          // match 3 used for p4 duty cycle 
    LPC_SCT1->MATCHREL[3].L  = 105 - 1;                      // match 3 reload used for p4 duty cycle 

    LPC_SCT1->MATCH[4].L     = 105 - 1;                          // match 4 used for p5 duty cycle 
    LPC_SCT1->MATCHREL[4].L  = 105 - 1;                      // match 4 reload used for p5 duty cycle 
                                                              
    LPC_SCT1->OUTPUT = 0x01;    //set out0
    LPC_SCT1->CTRL_L    &= ~(1 << 2);                      // unhalt it by clearing bit 2 of CTRL reg
 
}
void SCT0_vStop(void)
{
    
    LPC_SCT0->CTRL_L  |= (1 << 2);                      // halt it by clearing bit 2 of CTRL reg   
    LPC_SCT0->OUTPUT = 0x00;    //clr out0 1 
}
void SCT1_vStop(void)
{
    LPC_SCT1->CTRL_L  |= (1 << 2);                      // halt it by clearing bit 2 of CTRL reg   
    LPC_SCT1->OUTPUT = 0x00;    //clr out0 1 
}

static void SCT0_Init(void)
{
    IO_SWM_vMovablePinAssignZ(IO_P1_05,SWM_SCT0_OUT0_O); // PIO 1_5 is 37

	IO_SWM_vMovablePinAssignZ(IO_P1_04,SWM_SCT0_OUT1_O); // PIO 1_4 is 36
	
	IO_SWM_vMovablePinAssignZ(IO_P1_06,SWM_SCT0_OUT2_O); // PIO 1_6 is 38
    //Chip_SWM_MovablePinAssign(SWM_SCT0_OUT0_O, 23);
		// Chip_SWM_MovablePinAssign(SWM_SCT1_OUT1_O, 27);
    LPC_SYSCON->SYSAHBCLKCTRL[1] |= EN1_SCT0;                // enable the SCT1 clock

    LPC_SCT0->CONFIG  =  (1   << 17);             // two 16-bit timers, auto limit   // auto limit at match0
		
	LPC_SCT0->CTRL_L |= (1<<3); 
	LPC_SCT0->CTRL_L |= (1 << 5)|(1 << 6)|(1 << 7)|(1 << 11); ///72 Devided the 72Mhz zz~~
  
	LPC_SCT0->MATCH[0].L     = SCT0_Period - 1;                       // match 0 
    LPC_SCT0->MATCHREL[0].L  = SCT0_Period - 1;
    LPC_SCT0->EVENT[0].STATE    = (1023 << 0);          // event 0 happens in all  state
    LPC_SCT0->EVENT[0].CTRL     = (0 << 0) |          // event0 is associated with match0 L
                                  (1 << 12)|                 // match  condition only
                                  (1 << 15) ;            // state increment is 1
    
    LPC_SCT0->MATCH[1].L     = SCT0_R_up - 1;                          // match 1 used for p1 duty cycle 
    LPC_SCT0->MATCHREL[1].L  = SCT0_R_up - 1;                      // match 1 reload used for p2 duty cycle 
    LPC_SCT0->EVENT[1].STATE    = (1023 << 0);
    LPC_SCT0->EVENT[1].CTRL     = (1 << 0) |          // event0 is associated with match1 L
                                  (1 << 12);                 // match  condition only
    
    LPC_SCT0->MATCH[2].L     = SCT0_R_down - 1;                          // match 2 used for p3 duty cycle 
    LPC_SCT0->MATCHREL[2].L  = SCT0_R_down - 1;                      // match 2 reload used for p3 duty cycle 
    LPC_SCT0->EVENT[2].STATE    = (1023 << 0);          // happens in   state2
    LPC_SCT0->EVENT[2].CTRL     = (2 << 0) |          // event0 is associated with match1 L
                                  (1 << 12);                 // match  condition only

    LPC_SCT0->MATCH[3].L     = SCT0_G_up - 1;                          // match 3 used for p4 duty cycle 
    LPC_SCT0->MATCHREL[3].L  = SCT0_G_up - 1;                      // match 3 reload used for p4 duty cycle 
    LPC_SCT0->EVENT[3].STATE    = (1023 << 0);          // happens in   state3
    LPC_SCT0->EVENT[3].CTRL     = (3 << 0) |          // event3 is associated with match3 L
                                  (1 << 12);                 // match  condition only

    LPC_SCT0->MATCH[4].L     = SCT0_G_down - 1;                          // match 4 used for p5 duty cycle 
    LPC_SCT0->MATCHREL[4].L  = SCT0_G_down - 1;                      // match 4 reload used for p5 duty cycle 
    LPC_SCT0->EVENT[4].STATE    = (1023 << 0);          // happens in   state4
    LPC_SCT0->EVENT[4].CTRL     = (4 << 0) |          // event4 is associated with match4 L
                                  (1 << 12);                 // match  condition only
                                                               
    LPC_SCT0->MATCH[5].L     = SCT0_B_up - 1;                          // match 5 used for p6 duty cycle 
    LPC_SCT0->MATCHREL[5].L  = SCT0_B_up - 1;                      // match 5 reload used for p6 duty cycle 
    LPC_SCT0->EVENT[5].STATE    = (1023 << 0);          // happens in   state5
    LPC_SCT0->EVENT[5].CTRL     = (5 << 0) |          // event5 is associated with match5 L
                                  (1 << 12);                 // match  condition only
                                                               // state increment is 0           

    LPC_SCT0->MATCH[6].L     = SCT0_B_down - 1;                          // match 6 used for p7 duty cycle 
    LPC_SCT0->MATCHREL[6].L  = SCT0_B_down - 1;                      // match 6 reload used for p7 duty cycle 
    LPC_SCT0->EVENT[6].STATE    = (1023 << 0);          // happens in   state6
    LPC_SCT0->EVENT[6].CTRL     = (6 << 0) |          // event6 is associated with match6 L
                                  (1 << 12);                 // match  condition only
                                                               // state increment is 0      


	LPC_SCT0->MATCH[7].L     = SCT0_Period - 1;                          // match 7 used for p8 duty cycle 
    LPC_SCT0->MATCHREL[7].L  = SCT0_Period - 1;                      // match 7 reload used for p8 duty cycle 
    LPC_SCT0->EVENT[7].STATE    = (1 << 7)|          // happens in   state7
                                  (1 << 8);         // happens in   state8
    LPC_SCT0->EVENT[7].CTRL     =  (7 << 0) |          // event8 is associated with match8 L
                                   (1 << 12)|                   // match  condition only
                                   (1 << 14)|                      //load state value    
                                   (7 << 15);       // state load is  0    
	
    LPC_SCT0->OUT[0].SET = (1 << 1);                       // event 0 will set   OUT0
    LPC_SCT0->OUT[0].CLR = (1 << 2);
		
    LPC_SCT0->OUT[1].SET = (1 << 3);                       // event 0 will set   OUT0
    LPC_SCT0->OUT[1].CLR = (1 << 4);
	
    LPC_SCT0->OUT[2].SET = (1 << 5);                       // event 0 will set   OUT0
    LPC_SCT0->OUT[2].CLR = (1 << 6);
}


/////////////////////////////////////////////////////////////////////////////////

static void SCT1_Init(void)
{
 	IO_SWM_vMovablePinAssignZ(IO_P0_08,SWM_SCT1_OUT0_O);//PIO 0_12  12
 	IO_SWM_vMovablePinAssignZ(IO_P0_07,SWM_SCT1_OUT1_O);//p1,9vv41 
	IO_SWM_vMovablePinAssignZ(IO_P0_06,SWM_SCT1_OUT2_O);//p1.10  42
	IO_SWM_vEnableFixedPinZ(SWM_FIXED_SCT1_OUT3);//PIO 0_2(can`t change)
	IO_SWM_vEnableFixedPinZ(SWM_FIXED_SCT1_OUT4);//PIO 0_3(can`t change)

	LPC_SYSCON->SYSAHBCLKCTRL[1] |= EN1_SCT1;                // enable the SCT1 clock

	//LPC_SCT1->CONFIG  =  (1 << 17);             // two 16-bit timers, auto limit   // auto limit at match0
	LPC_SCT1->LIMIT_L = (1 << 0)|(1 << 5);
	
	LPC_SCT1->CTRL_L |= (1 << 3); 
	LPC_SCT1->CTRL_L |= (1 << 5)|(1 << 6)|(1 << 7)|(1 << 11); ///72 Devided the 72Mhz zz~~

	//	LPC_SCT1->DMAREQ = (1 << 30);//A 1 in this bit makes the SCT set DMA request 0 when it loads the Match_L/Unified registers from the Reload_L/Unified registers.

	LPC_SCT1->MATCH[0].L     = PWM_PERIOD - 1;                       // match 0 
	LPC_SCT1->MATCHREL[0].L  = PWM_PERIOD - 1;          //240 means 300kHz ....so 4000 means 18kHz
	LPC_SCT1->EVENT[0].STATE    = (1023 << 0);          // event 0 happens in all  state
	LPC_SCT1->EVENT[0].CTRL     = (0 << 0) |          // event0 is associated with match0 L                // match  condition only
								(1 << 12)|                   // match  condition only
								(1 << 14)|                      //load state value    
							    (7 << 15);
	
	LPC_SCT1->MATCH[1].L     = 105 - 1;                          // match 1 used for p1 duty cycle 
	LPC_SCT1->MATCHREL[1].L  = 105 - 1;                      // match 1 reload used for p2 duty cycle 
	LPC_SCT1->EVENT[1].STATE    = (1023 << 0);         //   happens in   state1
	LPC_SCT1->EVENT[1].CTRL     = (1 << 0) |          // event0 is associated with match1 L
								(1 << 12);                 // match  condition only
																															 // state increment is 0
	
	LPC_SCT1->MATCH[2].L     = 105 - 1;                          // match 2 used for p3 duty cycle 
	LPC_SCT1->MATCHREL[2].L  = 105 - 1;                      // match 2 reload used for p3 duty cycle 
	LPC_SCT1->EVENT[2].STATE    = (1023 << 0);          // happens in   state2
	LPC_SCT1->EVENT[2].CTRL     = (2 << 0) |          // event0 is associated with match1 L
								(1 << 12);                 // match  condition only
																														 // state increment is 0

	LPC_SCT1->MATCH[3].L     = 105 - 1;                          // match 3 used for p4 duty cycle 
	LPC_SCT1->MATCHREL[3].L  = 105 - 1;                      // match 3 reload used for p4 duty cycle 
	LPC_SCT1->EVENT[3].STATE    = (1023 << 0);          // happens in   state3
	LPC_SCT1->EVENT[3].CTRL     = (3 << 0) |          // event3 is associated with match3 L
								(1 << 12);                 // match  condition only
																														 // state increment is 0
	LPC_SCT1->MATCH[4].L     = 105 - 1;                          // match 4 used for p5 duty cycle 
	LPC_SCT1->MATCHREL[4].L  = 105 - 1;                      // match 4 reload used for p5 duty cycle 
	LPC_SCT1->EVENT[4].STATE    = (1023 << 0);          // happens in   state4
	LPC_SCT1->EVENT[4].CTRL     = (4 << 0) |          // event4 is associated with match4 L
								(1 << 12);                 // match  condition only

	LPC_SCT1->MATCH[5].L     = 300 - 1;                          // match 4 used for p5 duty cycle 
	LPC_SCT1->MATCHREL[5].L  = 300 - 1;                      // match 4 reload used for p5 duty cycle 
	LPC_SCT1->EVENT[5].STATE    = (1023 << 0);          // happens in   state4
	LPC_SCT1->EVENT[5].CTRL     = (5 << 0) |          // event4 is associated with match4 L
								(1 << 12)|(1 << 14)|                      //load state value    
							    (7 << 15);

	LPC_SCT1->EVEN = (1 << 0)|(1 << 5);

	LPC_SCT1->OUT[0].SET = (1 << 1);                       
	LPC_SCT1->OUT[0].CLR = (1 << 0)|(1 << 5);

	LPC_SCT1->OUT[1].SET = (1 << 2);                      
	LPC_SCT1->OUT[1].CLR = (1 << 0)|(1 << 5);

	LPC_SCT1->OUT[2].SET = (1 << 3);                      
	LPC_SCT1->OUT[2].CLR = (1 << 0)|(1 << 5);

	LPC_SCT1->OUT[3].SET = (1 << 4);                       
	LPC_SCT1->OUT[3].CLR = (1 << 0)|(1 << 5);

	LPC_SCT1->OUT[4].SET = (1 << 5);
	LPC_SCT1->OUT[4].CLR = (1 << 5);

	LPC_SCT1->RES |= (1 << 8)|(1 << 9);//toggle output 4

}

void PWRCTRL_vGetColorSEQ(){
	//memcpy((char *)ubColorBuff,g_rxBuff,COMMAND_BUFF_SIZE);
	
	int colorBufCount ;
	int colorTimeBufCount ;
	USHORT TimeTemp;
	

	NVIC_DisableIRQ(SCT1_IRQn);

	colorBufCount = 0;
	colorTimeBufCount = 0;
	while(ubSEQbuff[colorBufCount+2] != 0xfe){
		ubColorBuff[colorBufCount] = ubSEQbuff[colorBufCount+2];
		colorBufCount += 1;
	}

	if(ubSEQbuff[3*colorBufCount+3] == 0xfe){
		while(colorTimeBufCount < colorBufCount){
			TimeTemp =(USHORT)((USHORT)ubSEQbuff[2*colorTimeBufCount+3+colorBufCount] << 8);
		 	TimeTemp += (USHORT)ubSEQbuff[2*colorTimeBufCount+4+colorBufCount];

			ubColorTimeBuff[colorTimeBufCount] = TimeTemp;
			colorTimeBufCount += 1;
			}
		}
	
	i_test = 0;
	
	if(colorBufCount == colorTimeBufCount && colorTimeBufCount != 0){
		seqLength = 3*colorBufCount+4;
		NVIC_EnableIRQ(SCT1_IRQn);
		if(vcom_enable == TRUE)
			vcom_write(&cmdRight,1);
	}
	else{
		NVIC_DisableIRQ(SCT1_IRQn);
		if(vcom_enable == TRUE)
			vcom_write(&cmdError,1);
	}
		
}
void Pwrctrl_vSetKengTime(UBYTE keng){

	kengTime = keng - 1;
	vcom_write(&cmdRight,1);
}
void  Pwrctrl_vGerKengTime(){
	UBYTE keng;
	int i = 0;
	keng = kengTime + 1;
	vcom_write(&cmdRight,1);
	for(i=0;i<1000;i++);
	vcom_write(&keng,1);

}

void Pwrctrl_vSetSubframePeriod(UBYTE subH, UBYTE subL){	
	subFramePeriod =  (USHORT)((USHORT)subH << 8);
		
  	subFramePeriod += (USHORT)subL;
	vcom_write(&cmdRight,1);
}

void Pwrctrl_vGetSubframePeriod(){
	UBYTE sb[2] = {0};
	int i = 0;
	sb[0] = (UBYTE)(subFramePeriod>>8);
	sb[1] = (UBYTE)(subFramePeriod&0xff);
	vcom_write(&cmdRight,1);
	for(i=0;i<1000;i++);
	vcom_write(sb,2);
	
}
void SCT1_IRQHandlerzz(){
	USHORT i_period;	
	
	static UBYTE i_color = 0,i_color_r=0,i_color_l=0;
	
	LPC_SCT1->EVFLAG = (1 << 0)|(1 << 5);///To protect the intterupt request happens all the time. it took me one day to konw it


	if(ubSEQbuff[i_test+1] == 0xfe)
		i_test = 0;
	
		i_color = ubColorBuff[i_test];
		i_period = ubColorTimeBuff[i_test];
		/*better not do this change
//20150414 change
	if(sensorCnt<3){
		i_color = ubColorBuff[i_test];
		i_period = ubColorTimeBuff[i_test];
		}
	else{
		while(ubColorBuff[i_test]>=0x10){
			i_test++;
			}
		i_color = ubColorBuff[i_test];
		i_period = ubColorTimeBuff[i_test];

		}//change end
		*/
	i_color_r = i_color & 0x0f;


	if(i_period <= 30)
		i_period = 30;
	
	
	subTimeCounter += i_period;

	i_test++;
	frameFlagOld = frameFlag;
	//vcom_write("ENTER\r\n",7);
	if(frameFlagTemp2 == 1){
		
		}
	if(frameFlagTemp == 1){
		frameFlagTemp2 = 1;

		
		if(frameFlag == 1)
			frameFlag = 0;
		else
			frameFlag = 1;
	
		}
	else
		frameFlagTemp2 = 0;
	
	
	if(subTimeCounter >= subFramePeriod/2){
		
		LPC_SCT1->MATCHREL[5].L = subFramePeriod/2 + i_period - subTimeCounter -  1;
		subTimeCounter = 0;
		i_test = 0;

		frameFlagTemp = 1;

		if(firstSub<5)
			firstSub++;

		sensorCnt = 0;

		colorCutOut = 1;
	}
	else{
		LPC_SCT1->MATCHREL[5].L = i_period + 2;
	
		frameFlagTemp = 0;
		colorCutOut = 0 ;
		}


	LPC_SCT1->MATCHREL[0].L = i_period-1;
	
	if(firstSub>=3&&colorCutOut==0){
	if(i_color_r == COLOR_RED){
			LPC_SCT1->MATCHREL[1].L  = kengTime;//R
		 	LPC_SCT1->MATCHREL[2].L  = i_period+1;//G
		 	LPC_SCT1->MATCHREL[3].L  = i_period+1;  //B 
		}
	
	if(i_color_r == COLOR_GREEN){
			LPC_SCT1->MATCHREL[1].L  = i_period+1;//R
		 	LPC_SCT1->MATCHREL[2].L  = kengTime;//G
		 	LPC_SCT1->MATCHREL[3].L  = i_period+1;  //B 
		}

	if(i_color_r == COLOR_BLUE){
			LPC_SCT1->MATCHREL[1].L  = i_period+1;//R
		 	LPC_SCT1->MATCHREL[2].L  = i_period+1;//G
		 	LPC_SCT1->MATCHREL[3].L  = kengTime;  //B 
		}

	if(i_color_r == COLOR_YELLOW){
			LPC_SCT1->MATCHREL[1].L  = kengTime;//R
		 	LPC_SCT1->MATCHREL[2].L  = kengTime;//G
		 	LPC_SCT1->MATCHREL[3].L  = i_period+1;  //B 
		}

	if(i_color_r == COLOR_MAGENTA){
			LPC_SCT1->MATCHREL[1].L  = kengTime;//R
		 	LPC_SCT1->MATCHREL[2].L  = i_period+1;//G
		 	LPC_SCT1->MATCHREL[3].L  = kengTime;  //B
		}

	if(i_color_r == COLOR_CYAN){
			LPC_SCT1->MATCHREL[1].L  = i_period+1;//R
		 	LPC_SCT1->MATCHREL[2].L  = kengTime;//G
		 	LPC_SCT1->MATCHREL[3].L  = kengTime;  //B 
		}
	
	if(i_color_r == COLOR_WHITE){
			LPC_SCT1->MATCHREL[1].L  = kengTime;//R
		 	LPC_SCT1->MATCHREL[2].L  = kengTime;//G
		 	LPC_SCT1->MATCHREL[3].L  = kengTime;  //B 
		}
	
	if(i_color_r == COLOR_BLACK){
			LPC_SCT1->MATCHREL[1].L  = i_period+1;//R
		 	LPC_SCT1->MATCHREL[2].L  = i_period+1;//G
		 	LPC_SCT1->MATCHREL[3].L  = i_period+1;  //B 
		}
	
	i_color_l = (i_color >> 4);
	i_color_l &= 0x0f;
	if(i_color_l == 0)
		LPC_SCT1->MATCHREL[4].L  = i_period+1;//s
	else if(i_color_l == 1&& sensorCnt<3/*&& subframe  is low*/)	{
		LPC_SCT1->MATCHREL[4].L  = kengTime;//s
		sensorCnt++;
	}
	else if(i_color_l == 2&& sensorCnt<3 /*&&subframe is high*/){
		sensorCnt++;
		LPC_SCT1->MATCHREL[4].L  = kengTime;//s
	}
	else{
		LPC_SCT1->MATCHREL[1].L  = i_period+1;;//R
		LPC_SCT1->MATCHREL[2].L  = i_period+1;;//G
		LPC_SCT1->MATCHREL[3].L  = i_period+1;;//B 
		LPC_SCT1->MATCHREL[4].L  = i_period+1;;//s
	}

		}
	else{
		LPC_SCT1->MATCHREL[1].L  = i_period+1;;//R
		LPC_SCT1->MATCHREL[2].L  = i_period+1;;//G
		LPC_SCT1->MATCHREL[3].L  = i_period+1;;//B 
		LPC_SCT1->MATCHREL[4].L  = i_period+1;;//s
		}
}

bool Pwrctrl_vStoreColorSeqToEE(UBYTE unit){
	uint8_t *ptr = (uint8_t *) ubSEQbuff;
	uint8_t ret_code = 0;
	UBYTE crc0=0xef,crc1=0xef,crc2=0xef,crc3=0xef;
	
	//ret_code = MEMORY_tEEPROMWrite(EEPROM_ADDRESS,ptr,64);
	if(unit == 0){
		crc0 = CRC_ubCrc8(ubSEQbuff, 0, seqLength);
		if(crc0 != 0){
			ret_code = MEMORY_tWrite(ptr,COLOR_SEQ_BSAE0,seqLength,MEMTYPE_EEPROM);
			
			ptr = (uint8_t *)&crc0;
			ret_code += MEMORY_tWrite(ptr,COLORSEQ0_CRC,1,MEMTYPE_EEPROM);
			ptr = (uint8_t *)&seqLength;
			ret_code += MEMORY_tWrite(ptr,SEQLENGTH0,1,MEMTYPE_EEPROM);
			}
		else{
			vcom_write(&eeError,1);
			return FALSE;
			}
		}
	else if(unit == 1){
		crc1 = CRC_ubCrc8(ubSEQbuff, 0, seqLength);
		if(crc1 != 0){
			ret_code = MEMORY_tWrite(ptr,COLOR_SEQ_BSAE1,seqLength,MEMTYPE_EEPROM);
			
			ptr = (uint8_t *)&crc1;
			ret_code += MEMORY_tWrite(ptr,COLORSEQ1_CRC,1,MEMTYPE_EEPROM);
			ptr = (uint8_t *)&seqLength;
			ret_code += MEMORY_tWrite(ptr,SEQLENGTH1,1,MEMTYPE_EEPROM);
			}
		else{
			vcom_write(&eeError,1);
			return FALSE;
			}
		}
	else if(unit == 2){
		crc2 = CRC_ubCrc8(ubSEQbuff, 0, seqLength);
		if(crc2 != 0){
			ret_code = MEMORY_tWrite(ptr,COLOR_SEQ_BSAE2,seqLength,MEMTYPE_EEPROM);
			
			ptr = (uint8_t *)&crc2;
			ret_code += MEMORY_tWrite(ptr,COLORSEQ2_CRC,1,MEMTYPE_EEPROM);
			ptr = (uint8_t *)&seqLength;
			ret_code += MEMORY_tWrite(ptr,SEQLENGTH2,1,MEMTYPE_EEPROM);
			}

		else{
			vcom_write(&eeError,1);
			return FALSE;
			}
		}
	else if(unit == 3){
		crc3 = CRC_ubCrc8(ubSEQbuff, 0, seqLength);
		if(crc3 != 0){
			ret_code = MEMORY_tWrite(ptr,COLOR_SEQ_BSAE3,seqLength,MEMTYPE_EEPROM);
			
			ptr = (uint8_t *)&crc3;
			ret_code += MEMORY_tWrite(ptr,COLORSEQ3_CRC,1,MEMTYPE_EEPROM);
			ptr = (uint8_t *)&seqLength;
			ret_code += MEMORY_tWrite(ptr,SEQLENGTH3,1,MEMTYPE_EEPROM);
			}
		else{
			vcom_write(&eeError,1);
			return FALSE;
			}
		}
	//vcom_write("EE store enter\r\n",16);
	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	return TRUE;
}
void Pwrctrl_vReadColorSeqFromEE(UBYTE unit){
	uint8_t *ptr = (uint8_t *) ubSEQbuff;
	uint8_t ret_code = 0;
	UBYTE crc_load0=0xef,crc_load1=0xef,crc_load2=0xef,crc_load3=0xef;
	UBYTE crc_calc = 0xfe;
	BOOL crc_equal = FALSE;
	int i;
	
	
	//ret_code = MEMORY_tEEPROMRead(EEPROM_ADDRESS,ptr,64);
	if(unit == 0){
		ptr = (uint8_t *)&seqLength;
		ret_code = MEMORY_tRead(ptr,SEQLENGTH0,1,MEMTYPE_EEPROM);
		ptr = (uint8_t *) ubSEQbuff;
		ret_code = MEMORY_tRead(ptr,COLOR_SEQ_BSAE0,seqLength,MEMTYPE_EEPROM);
		ptr = (uint8_t *)&crc_load0;
		ret_code = MEMORY_tRead(ptr,COLORSEQ0_CRC,1,MEMTYPE_EEPROM);
		crc_calc = CRC_ubCrc8(ubSEQbuff,0,seqLength);
		if(crc_calc == crc_load0)
			crc_equal = TRUE;
		}
	else if(unit == 1){
		ptr = (uint8_t *)&seqLength;
		ret_code = MEMORY_tRead(ptr,SEQLENGTH1,1,MEMTYPE_EEPROM);
		ptr = (uint8_t *) ubSEQbuff;
		ret_code = MEMORY_tRead(ptr,COLOR_SEQ_BSAE1,seqLength,MEMTYPE_EEPROM);
		ptr = (uint8_t *)&crc_load1;
		ret_code = MEMORY_tRead(ptr,COLORSEQ1_CRC,1,MEMTYPE_EEPROM);
		crc_calc = CRC_ubCrc8(ubSEQbuff,0,seqLength);
		if(crc_calc == crc_load1)
			crc_equal = TRUE;
		}
	else if(unit == 2){
		ptr = (uint8_t *)&seqLength;
		ret_code = MEMORY_tRead(ptr,SEQLENGTH2,1,MEMTYPE_EEPROM);
		ptr = (uint8_t *) ubSEQbuff;
		ret_code = MEMORY_tRead(ptr,COLOR_SEQ_BSAE2,seqLength,MEMTYPE_EEPROM);
		ptr = (uint8_t *)&crc_load2;
		ret_code = MEMORY_tRead(ptr,COLORSEQ2_CRC,1,MEMTYPE_EEPROM);
		crc_calc = CRC_ubCrc8(ubSEQbuff,0,seqLength);
		if(crc_calc == crc_load2)
			crc_equal = TRUE;
		}
	else if(unit == 3){
		ptr = (uint8_t *)&seqLength;
		ret_code = MEMORY_tRead(ptr,SEQLENGTH3,1,MEMTYPE_EEPROM);
		ptr = (uint8_t *) ubSEQbuff;
		ret_code = MEMORY_tRead(ptr,COLOR_SEQ_BSAE3,seqLength,MEMTYPE_EEPROM);
		ptr = (uint8_t *)&crc_load3;
		ret_code = MEMORY_tRead(ptr,COLORSEQ3_CRC,1,MEMTYPE_EEPROM);
		crc_calc = CRC_ubCrc8(ubSEQbuff,0,seqLength);
		if(crc_calc == crc_load3)
			crc_equal = TRUE;
		}
	

	if(ret_code == 0&&crc_equal == TRUE){
		if(vcom_enable == TRUE)
			vcom_write(&cmdRight,1);
		for(i=1;i<10000;i++);
		PWRCTRL_vGetColorSEQ();
	}
	else{
		if(vcom_enable == TRUE)
			vcom_write(&eeError,1);
		}
	
	//vcom_write("EE read enter\r\n",15);
}
int PWRCTRL_InGetFrameFlag(void){
	return frameFlag;
}
int PWRCTRL_InGetFrameOldFlag(void){
	return frameFlagOld;
}

void PWRCTRL_vSetFrameFlagDone(void){
	frameFlag = 2;
}


//--------------------------------------------------------------------------

//------------------------------


/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

