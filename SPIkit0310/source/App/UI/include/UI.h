/**
 ******************************************************************************
 * @file    UI.h 
 * @author  inverto
 * @version V0.0.0
 * @date    13-jul-2011
 ******************************************************************************
 */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UI_H
#define _UI_H
/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/*
#define  CMD_UNDEFINED  (UBYTE)(0x00)
#define  CMD_CORRECT     (UBYTE)( 0x01)
#define  CMD_SETREDDUTY    (UBYTE)( 0x03)
#define  CMD_SETGREENDUTY    (UBYTE)( 0x04)
#define  CMD_SETBLUEDUTY    (UBYTE)( 0x05)
#define  CMD_DOCALIB    (UBYTE)( 0x06)
#define  CMD_SETREDCURRENT    (UBYTE)( 0x07)
#define  CMD_SETGREENCURRENT   (UBYTE)( 0x08)
#define  CMD_SETBLUECURRENT    (UBYTE)( 0x09)
#define  CMD_TURNONDRIVER    (UBYTE)( 0x0A)
#define  CMD_TURNOFFDRIVER    (UBYTE)( 0x0B)
#define  CMD_QUERY_CALIB    (UBYTE)( 0x0C)
*/
#define  CMD_UNDEFINED  (UBYTE)(0x00)
#define  CMD_CORRECT     (UBYTE)( 0x01)
//#define  CMD_TURNOFFDRIVER    (UBYTE)( 0x02)
#define  CMD_SETSCT0PERIOD    (UBYTE)( 0x03)
#define  CMD_SETSCT0RED    (UBYTE)( 0x04)
#define  CMD_SETSCT0GREEN    (UBYTE)( 0x05)
#define  CMD_SETSCT0BLUE    (UBYTE)( 0x06)
#define  CMD_SETSCT1RED    (UBYTE)( 0x07)
#define  CMD_SETSCT1GREEN   (UBYTE)( 0x08)
#define  CMD_SETSCT1BLUE  (UBYTE)( 0x09)
#define  CMD_SETSCT1SENSE    (UBYTE)( 0x0A)
#define  CMD_SETSCT1CAL   (UBYTE)( 0x0B)

 



void UI_EXE(void);



#endif /* _UART_H */

