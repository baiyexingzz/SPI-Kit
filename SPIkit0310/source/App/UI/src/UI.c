/**
 ******************************************************************************
 * @file    uart.c 
 * @author  inverto
 * @version V0.0.0
 * @date    13-jul-2011
 ******************************************************************************
 */ 


/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "stdtypes.h"
#include "UI.h"
#include "Calib.h"
#include "pwrctrl.h"
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"

#include <string.h>
///////////////////////////////////////////////////////////

extern uint8_t g_rxBuff[512];
/* Private typedef -----------------------------------------------------------*/
// Function pointer type. Return type is TE_RESULT, arguments void
typedef UBYTE (*T_CMD_HANDLER)(void);

typedef struct 
{ 
  UBYTE ubCmd;
  T_CMD_HANDLER pfnCmdHandler;
} TS_DPL_CMD_TABLE;

typedef struct 
{
  UBYTE ubExpectedMessageLength:4;   // Maximum of 16 bytes !!!
  T_CMD_HANDLER pfnCmdHandler;
} TS_CMD_CHAR;
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

;
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
//#define UART_MAX_TIME  (16) 
//#define UART_RECE_TIME       (8)
#define UART0_MAX_TIME  (20) 
#define UART0_RECE_TIME       (8)
/* Private variables ---------------------------------------------------------*/
/******************************************/
/********* The static instance of UART **********/   
/******************************************/


//static uint8_t driver_uart_comm_flag = 0;




static  UBYTE ubTempbuff[10];





//---------------------------------------------------------------------------
UBYTE UART_ubCMDParser(void);
static UBYTE UI_ubCmdCorrect(void);
static UBYTE UI_ubCmdErr(void);

static UBYTE UI_ubCmdSetSCT0Period(void);
static UBYTE UI_ubCmdSetSCT0Red(void);
static UBYTE UI_ubCmdSetSCT0Green(void);
static UBYTE UI_ubCmdSetSCT0Blue(void);
static UBYTE UI_ubCmdSetSCT1RedDuty(void);
static UBYTE UI_ubCmdSetSCT1GreenDuty(void);
static UBYTE UI_ubCmdSetSCT1BlueDuty(void);
static UBYTE UI_ubCmdSetSCT1SenseDuty(void);
static UBYTE UI_ubCmdSetSCT1CalDuty(void);






void UI_vLoadCharacteristics(const TS_DPL_CMD_TABLE *ptsCmdTable, UBYTE ubCmd,TS_CMD_CHAR* ptsCmdCharacteristics);

const TS_DPL_CMD_TABLE DPL_atsCmdTable[] =
{      
  /***************************************************************************/
  /*                              DPL Commands                               */
  /***************************************************************************/
  // {Command code                  ,    Command Function

  { 
    CMD_CORRECT,   UI_ubCmdCorrect
  },

  { 
    CMD_SETSCT0PERIOD,   UI_ubCmdSetSCT0Period
  },
  { 
    CMD_SETSCT0RED,   UI_ubCmdSetSCT0Red
  },
  { 
    CMD_SETSCT0GREEN,   UI_ubCmdSetSCT0Green
  },
  { 
    CMD_SETSCT0BLUE,   UI_ubCmdSetSCT0Blue
  },

  { 
    CMD_SETSCT1RED,   UI_ubCmdSetSCT1RedDuty
  },
  { 
    CMD_SETSCT1GREEN   ,   UI_ubCmdSetSCT1GreenDuty
  },
  { 
    CMD_SETSCT1BLUE,   UI_ubCmdSetSCT1BlueDuty
  },  
  { 
    CMD_SETSCT1SENSE,   UI_ubCmdSetSCT1SenseDuty
  },  

  { 
    CMD_SETSCT1CAL,   UI_ubCmdSetSCT1CalDuty
  },  

/*
  { 
    CMD_SETGREENLEADDUTY,   UI_ubCmdSetGreenLeadingPulse
  },  

  { 
    CMD_SETBLUELEADDUTY,   UI_ubCmdSetBlueLeadingPulse
  },*/      
  {
    CMD_UNDEFINED,UI_ubCmdErr
  }
};

UBYTE UART_ubCMDParser(void)
{
  //char cX;
  //USHORT usAAAA = 0;
  //USHORT usBBBB = 0;  
 static TS_CMD_CHAR  tCmhdr;
        T_CMD_HANDLER pfnCmdHandler;
 // char *pChar;
 // char * ubBuff;
  UBYTE ubResult = RESULT_ERROR;
  UI_vLoadCharacteristics(DPL_atsCmdTable, ubTempbuff[1], &tCmhdr);
  pfnCmdHandler = tCmhdr.pfnCmdHandler;
  ubResult =  (*pfnCmdHandler)();
  return ubResult;

}


//-------------------------
void UI_vLoadCharacteristics(const TS_DPL_CMD_TABLE *ptsCmdTable, UBYTE ubCmd,TS_CMD_CHAR* ptsCmdCharacteristics)

/*
** Description: Load the characteristics of a givven command from a given table
** Parameters: ptsCmdTable 
**             ubCmd
**             ptsCmdCharacteristics
** Returns:
*/
{
  UBYTE ubIndex = 0;

  // Sets the Cmd Handler function using a function pointer and
  // sets the expected message length
  ptsCmdCharacteristics->pfnCmdHandler = UI_ubCmdErr;
  //ptsCmdCharacteristics->ubExpectedMessageLength = 1;


  while (ptsCmdTable[ubIndex].pfnCmdHandler != UI_ubCmdErr)
  {
    if (ubCmd == ptsCmdTable[ubIndex].ubCmd)
    {
      ptsCmdCharacteristics->pfnCmdHandler = ptsCmdTable[ubIndex].pfnCmdHandler;
      break;
    }
    ubIndex++;
  }
}


//---------------------

//-----------------------------------------------
//user interface  command handler
void UI_EXE(void)
{
//  if(buf_uart0.UPDATE_FLAG == 1)
  //{
  //buf_uart0.UPDATE_FLAG =0;  
//  memcpy((char *)ubTempbuff,buf_uart0.buf,COMMAND_BUFF_SIZE);
	  memcpy((char *)ubTempbuff,g_rxBuff,10);
	  if(UART_ubCMDParser()==RESULT_ERROR)
	  {
  
	  }
	  
}
//---------------------------------------------------------------------------

//--------------------------------------------------
static UBYTE UI_ubCmdCorrect(void)
/*
**   
** function: 
** params: 
** pre: -
** post: 
** response : 
*/
{
	vcom_write("CmdCorrect\r\n", 12);
  return (RESULT_SUCCESS);

}
//==========================================
static UBYTE UI_ubCmdErr(void)
/*
**   
** function: 
** params: 
** pre: -
** post: 
** response : 
*/
{
	vcom_write("CmdError\r\n", 10);
  return (RESULT_ERROR);

}
 //-----------------------------------------------


 
static UBYTE UI_ubCmdSetSCT0Period(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usPeriod;
  
  
  usPeriod =(USHORT)((USHORT)ubTempbuff[2] << 8);
  usPeriod += (USHORT)ubTempbuff[3];
 
 
   PWRCTRL_vSetPeriodForSCT0(usPeriod);
  ubResult = RESULT_SUCCESS;  
  vcom_write("Set Sct0 Period\r\n", 17);

  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT0Red(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1,usDuty2;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  usDuty2 = (USHORT)((USHORT)ubTempbuff[4] << 8);
  usDuty2 += (USHORT)ubTempbuff[5];
 
   PWRCTRL_vSetRForSCT0(usDuty1,usDuty2);
  ubResult = RESULT_SUCCESS;  
  
	vcom_write("Set Sct0 Red\r\n", 14);
  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT0Green(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1,usDuty2;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  usDuty2 = (USHORT)((USHORT)ubTempbuff[4] << 8);
  usDuty2 += (USHORT)ubTempbuff[5];
 
 
   PWRCTRL_vSetGForSCT0(usDuty1,usDuty2);
  ubResult = RESULT_SUCCESS;  
  
	vcom_write("Set Sct0 Green\r\n", 16);
  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT0Blue(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1,usDuty2;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  usDuty2 = (USHORT)((USHORT)ubTempbuff[4] << 8);
  usDuty2 += (USHORT)ubTempbuff[5];
 
   PWRCTRL_vSetBForSCT0(usDuty1,usDuty2);
  ubResult = RESULT_SUCCESS;  
  vcom_write("Set Sct0 Blue\r\n", 15);

  return (ubResult);
}


static UBYTE UI_ubCmdSetSCT1RedDuty(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  
 
   PWRCTRL_vSetRedDutyForSCT1(usDuty1);
  ubResult = RESULT_SUCCESS;  
  vcom_write("Set Sct1 Red Duty\r\n", 19);

  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT1GreenDuty(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1;
  
  
 usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  
 
   PWRCTRL_vSetGreenDutyForSCT1(usDuty1);
  ubResult = RESULT_SUCCESS;  
  
  vcom_write("Set Sct1 Green Duty\r\n", 21);

  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT1BlueDuty(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1;
  
  
 usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  
 
   PWRCTRL_vSetBlueDutyForSCT1(usDuty1);
  ubResult = RESULT_SUCCESS;  
  
  vcom_write("Set Sct1 Blue Duty\r\n", 20);

  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT1SenseDuty(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  

 
   PWRCTRL_vSetSenseDutyForSCT1(usDuty1);
  ubResult = RESULT_SUCCESS;  
  vcom_write("Set Sct1 Sense Duty\r\n", 21);

  return (ubResult);
}
static UBYTE UI_ubCmdSetSCT1CalDuty(void){
	UBYTE ubResult = RESULT_ERROR ;
  
  USHORT usDuty1;
  
  
  usDuty1 = (USHORT)((USHORT)ubTempbuff[2] << 8);
  usDuty1 += (USHORT)ubTempbuff[3];
  
 
   PWRCTRL_vSetCalDutyForSCT1(usDuty1);
  ubResult = RESULT_SUCCESS;  
  
  vcom_write("Set Sct1 Cal Duty\r\n", 19);

  return (ubResult);
}





/************************************************************************/
/************* The end of UART configure functions ****************************/
/************************************************************************/
