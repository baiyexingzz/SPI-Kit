//unsigned int DLS_Address UBYTE0x00;//0
//unsigned int DLS_ReservedUBYTE0xff;//1
#include "platform.h"
#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>
#include "UI.h"
#include "package.h"
#include "ssi.h"
#include "pwrctrl.h"
#include "timer.h"
#include "io.h"
#include "measure.h"
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"
#include "crc.h"
#include "instruction.h"
/*
uint16_t DLS_Address=0x00ff;//0,unsigned int
uint16_t DLS_RED_Drive_Level_Value=0x0000;//2,3
uint16_t DLS_GREEN_Drive_Level_Value=0x0000;//4,5
uint16_t DLS_BLUE_Drive_Level_Value=0x0000;//6,7
uint16_t DLS_Sample_1_Drive_Level_Value=0x0000;//8.9
uint16_t DLS_Sample_2_Drive_Level_Value=0x0000;//10,11
uint16_t DLS_YELLOW_Drive_Level_Value=0x0000;//12,13
uint16_t DLS_CYAN_Drive_Level_Value=0x0000;//14,15
uint16_t DLS_MAGENTA_Drive_Level_Value=0x0000;//16,17
uint16_t DLS_WHITE_Drive_Level_Value=0x0000;//18,19
uint16_t DLS_BLACK_Drive_Level_Value=0x0000;//20,21

uint16_t Timing_Address_Enable=0x0000;//0,1
uint16_t Timing_Strobe_Delay=0x0000;//2,3
uint16_t Timing_Strobe_Offset=0x0000;//4;


uint16_t ID_Address_Manufacturer=0x0000;//0,1
uint16_t ID_Hardware_Firmware=0x0000;//2,3
uint16_t ID_Type=0x0000;//4
*/


extern uint8_t g_rxBuff[512];
UBYTE DLS_Address = 0;
UBYTE DLS_Reseved = 0xff;
UBYTE DLS_RED_Drive_Level_Value_H = 0;
UBYTE DLS_RED_Drive_Level_Value_L = 0;
UBYTE DLS_GREEN_Drive_Level_Value_H = 0;
UBYTE DLS_GREEN_Drive_Level_Value_L = 0;
UBYTE DLS_BLUE_Drive_Level_Value_H = 0;
UBYTE DLS_BLUE_Drive_Level_Value_L = 0;
UBYTE DLS_Sample_1_Drive_Level_Value_H = 0;
UBYTE DLS_Sample_1_Drive_Level_Value_L = 0;
UBYTE DLS_Sample_2_Drive_Level_Value_H = 0;
UBYTE DLS_Sample_2_Drive_Level_Value_L= 0;
UBYTE DLS_YELLOW_Drive_Level_Value_H = 0;
UBYTE DLS_YELLOW_Drive_Level_Value_L = 0;
UBYTE DLS_CYAN_Drive_Level_Value_H = 0;
UBYTE DLS_CYAN_Drive_Level_Value_L = 0;
UBYTE DLS_MAGENTA_Drive_Level_Value_H = 0;
UBYTE DLS_MAGENTA_Drive_Level_Value_L = 0;
UBYTE DLS_WHITE_Drive_Level_Value_H = 0;
UBYTE DLS_WHITE_Drive_Level_Value_L = 0;
UBYTE DLS_BLACK_Drive_Level_Value_H = 0;
UBYTE DLS_BLACK_Drive_Level_Value_L = 0;

UBYTE Timing_Address = 0;
UBYTE Timing_Enable = 0;
UBYTE Timing_Strobe_Delay_H = 0;
UBYTE Timing_Strobe_Delay_L = 0;
UBYTE Timing_Strobe_Offset = 0;

UBYTE ID_Address = 0;
UBYTE ID_Manufacturer = 0;
UBYTE ID_Hardware = 0;
UBYTE ID_Frimware = 0;
UBYTE ID_Type = 0;

UBYTE Status_Adress = 0;
UBYTE Status_Summary = 0;

UBYTE DriverCapability_Adress = 0;
UBYTE DriverCapability_Type = 0;
UBYTE DriverCapability_IRsupport = 0;
UBYTE DriverCapability_ADCsupport = 0;


UBYTE ADCvalues_Address = 0;
UBYTE ADCvalues_ADC1Value = 0;
UBYTE ADCvalues_ADC1or2Value = 0;



uint8_t Drive_Level_Setting_Packet[22];
uint8_t Timing_Packet[5];
uint8_t ID_Packet[5];
uint8_t Status_Packet[2];
uint8_t Driver_Capability_Packet[4];
uint8_t ADCvalue_Packet[3];

UBYTE DLP_pattern1r[23] = {0x00,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88};
UBYTE DLP_pattern2r[23] = {0x10,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88};
UBYTE DLP_pattern3r[23] = {0x20,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88};

UBYTE DLP_pattern1[23] = {0x00,0x00,0x11,0x11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UBYTE DLP_pattern2[23] = {0x10,0x00,0,0,0x11,0x11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UBYTE DLP_pattern3[23] = {0x20,0x00,0,0,0,0,0x11,0x11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

USHORT sample_value[127] ={0x03FF,0x03FF,0x03EE,0x03EE,0x03DE,0x03DE,0x03CE,0x03CE,0x03BE,0x03BE,0x03AD,0x03AD,0x039D,0x039D,0x038D,0x038D,0x037D,0x037D,0x036C,0x036C,0x035C,0x035C,0x034C,0x034C,0x033C,0x033C,0x032B,0x032B,0x031B,0x031B,0x030B,0x030B,0x02FB,0x02FB,0x02EA,0x02EA,0x02DA,0x02DA,0x02CA,0x02CA,0x02BA,0x02BA,0x02AA,0x02AA,0x0299,0x0299,0x0289,0x0289,0x0279,0x0279,0x0269,0x0269,0x0258,0x0258,0x0248,0x0248,0x0238,0x0238,0x0228,0x0228,0x0217,0x0217,0x0207,0x0207,0x01F7,0x01F7,0x01E7,0x01E7,0x01D6,0x01D6,0x01C6,0x01C6,0x01B6,0x01B6,0x01A6,0x01A6,0x0195,0x0195,0x0185,0x0185,0x0175,0x0175,0x0165,0x0165,0x0155,0x0155,0x0144,0x0144,0x0134,0x0134,0x0124,0x0124,0x0114,0x0114,0x0103,0x0103,0x00F3,0x00F3,0x00E3,0x00E3,0x00D3,0x00D3,0x00C2,0x00C2,0x00B2,0x00B2,0x00A2,0x00A2,0x0092,0x0092,0x0081,0x0081,0x0071,0x0071,0x0061,0x0061,0x0051,0x0051,0x0040,0x0040,0x0030,0x0030,0x0020,0x0020,0x0010,0x0010,0x0000};
UBYTE TIming_pattern1[5] = {0x55,0x55,0x55,0x55,0x55};
UBYTE ID_pattern1[5] = {0x55,0x55,0x55,0x55,0x55};

int firstPacketSent = 0;
int secondPacketSent = 0;
int thirdPacketSent = 0;

int timeGap1 = 300;
int timeGap2 = 300;
int timeGap3 = 300;

int redPstep = 127;
int greenPstep  = 127;
int bluePstep = 127;


USHORT redSampleMax = 0xffff;
USHORT greenSampleMax = 0xffff;
USHORT blueSampleMax = 0xffff;
int redSampleFlag = 1; // 1 down, 0 up
int greenSampleFlag = 1;
int blueSampleFlag = 1;

int transParentFlag=0;
UBYTE transParentData[512] = {0};
UBYTE transParentLength = 0;
int transParentReady=0;

UBYTE redLevel1 = 0,redLevel2 = 0;
UBYTE greenLevel1 = 0, greenLevel2 = 0;
UBYTE blueLevel1 = 0, blueLevel2 = 0;

int sendRdy = 0;
bool isColorPacketSendingNow = false;

extern UBYTE ubSEQbuff[512];

extern UBYTE cmdRight;
extern UBYTE cmdError;
extern UBYTE cmdFormatError;
extern UBYTE eeError;

void Packet_DLP(){
	
	
	Drive_Level_Setting_Packet[0]=DLS_Address;//R/W(7) 每 Read/Write 0 : Write  1 : Read
												/*Clr(6:4) 每 Color
												000 : RED Solid State Driver
												001 : GREEN Solid State Driver
												010 : BLUE Solid State Driver
												011 : YELLOW Solid State Driver
												100 : CYAN Solid State Driver
												101 : MAGENTA Solid State Driver
												110 : WHITE Solid State Driver
												111 : IR Solid State Driver
												Ctrl(3:0) 每 Control
												0000 每 Drive Level Settings*/
	Drive_Level_Setting_Packet[1]=DLS_Reseved;//Reserved. [Default = 0xFF]
	Drive_Level_Setting_Packet[2]=DLS_RED_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[3]=DLS_RED_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[4]=DLS_GREEN_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[5]=DLS_GREEN_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[6]=DLS_BLUE_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[7]=DLS_BLUE_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[8]=DLS_Sample_1_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[9]=DLS_Sample_1_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[10]=DLS_Sample_2_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[11]=DLS_Sample_2_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[12]=DLS_YELLOW_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[13]=DLS_YELLOW_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[14]=DLS_CYAN_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[15]=DLS_CYAN_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[16]=DLS_MAGENTA_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[17]=DLS_MAGENTA_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[18]=DLS_WHITE_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[19]=DLS_WHITE_Drive_Level_Value_L;
	Drive_Level_Setting_Packet[20]=DLS_BLACK_Drive_Level_Value_H;
	Drive_Level_Setting_Packet[21]=DLS_BLACK_Drive_Level_Value_L;
	
}
void Packet_Timing(){
	
	Timing_Packet[0] = Timing_Address;/*R/W(7) 每 Read/Write
										0 : Write
										1 : Read
										Clr(6:4) 每 Color
										000 : RED Solid State Driver
										001 : GREEN Solid State Driver
										010 : BLUE Solid State Driver
										011 : YELLOW Solid State Driver
										100 : CYAN Solid State Driver
										101 : MAGENTA Solid State Driver
										110 : WHITE Solid State Driver
										111 : IR Solid State Driver
										Ctrl(3:0) 每 Control
										0001 每 Timing*/
	Timing_Packet[1] = Timing_Enable;/*DEM(7) 每 Driver Enable Mask
										0 : Disable Mask
										1 : Enable mask
										DE(0) 每 Driver Enable
										0 : Disable
										1 : Enable
										R 每 Reserved, set to zero (0)*/
	Timing_Packet[2] = Timing_Strobe_Delay_H;//SD(15:0) 每 Strobe Delay Range 0 - 65535  (lsb = 12.5 nsec)
	Timing_Packet[3] = Timing_Strobe_Delay_L;
	Timing_Packet[4] = Timing_Strobe_Offset;//SO(7:0) 每 Strobe Offset Range 0 - 255  	(lsb = 12.5 nsec)

	
	
}

void Packet_ID(){
	
	ID_Packet[0] = ID_Address;/*R/W(7) 每 Read/Write
								0 : Write
								1 : Read
								Clr(6:4) 每 Color
								000 : RED Solid State Driver
								001 : GREEN Solid State Driver
								010 : BLUE Solid State Driver
								011 : YELLOW Solid State Driver
								100 : CYAN Solid State Driver
								101 : MAGENTA Solid State Driver
								110 : WHITE Solid State Driver
								111 : IR Solid State Driver
								Ctrl(3:0) 每 Control
								0010 每 ID*/
	ID_Packet[1] = ID_Manufacturer;/*MID(7:0) 每 Manufacturer ID Range 0 - 254
									00h : Texas Instruments
									01h : Reserved
									＃
									FEh : Reserved*/
	ID_Packet[2] = ID_Hardware;//HID(7:0) 每 Hardware ID Range 0 - 255
	ID_Packet[3] = ID_Frimware;//FID(7:0) 每 Firmware ID Range 0 - 255
	ID_Packet[4] = ID_Type;/*RD(7) 每 RED SSI Driver  0 : Inactive  1 : Active

							GRN(6) 每 GREEN SSI Driver  0 : Inactive  1 : Active
							
							BL(5) 每 BLUE SSI Driver  0 : Inactive  1 : Active
							
							YLW(4) 每 YELLOW SSI Driver  0 : Inactive  1 : Active
							
							CYN(3) 每 CYAN SSI Driver  0 : Inactive  1 : Active
							
							MG(2) 每 MAGENTA SSI Driver  0 : Inactive  1 : Active
		
							WT(1) 每 WHITE SSI Driver  0 : Inactive  1 : Active
							
							TYPE(0) 每 SSI Type 0 : Inactive (LED)  1 : Active (Alternate Solid State Illumination source)

							*/
	
}
void Packet_Status(){
	Status_Packet[0] = Status_Adress;/*R/W(7) 每 Read/Write
										0 : Write
										1 : Read

										Clr(6:4) 每 Color
										000 : RED Solid State Driver
										001 : GREEN Solid State Driver
										010 : BLUE Solid State Driver
										011 : YELLOW Solid State Driver
										100 : CYAN Solid State Driver
										101 : MAGENTA Solid State Driver
										110 : WHITE Solid State Driver
										111 : IR Solid State Driver

										Ctrl(3:0) 每 Control
										0011 每 Status*/
	Status_Packet[1] = Status_Summary;/*TS(1) 每 Temperature Status
										0 : Inactive (One or more temperatures Out of Range)
										1 : Active (All temperatures In Range)
										LIT(0) 每 SSI Lit
										0 : Inactive (One or more SSI are OFF)
										1 : Active (All SSI are ON)
										R 每 Reserved, set to zero (0)*/
}

void Packet_Driver_Capability_Packet(){
	Driver_Capability_Packet[0] = DriverCapability_Adress;/*R/W(7) 每 Read only
															1 : Read
															Clr(6:4) 每 Color
															000 : RED Solid State Driver
															001 : GREEN Solid State Driver
															010 : BLUE Solid State Driver
															011 : YELLOW Solid State Driver
															100 : CYAN Solid State Driver
															101 : MAGENTA Solid State Driver
															110 : WHITE Solid State Driver
															111 : IR Solid State Driver
															Ctrl(3:0) 每 Control
															0100 每 Driver Capability Packet*/
	Driver_Capability_Packet[1] = DriverCapability_Type;/*RD(7) 每 RED SSI Driver
															0 : Inactive
															1 : Active
															GRN(6) 每 GREEN SSI Driver
															0 : Inactive
															1 : Active
															BL(5) 每 BLUE SSI Driver
															0 : Inactive
															1 : Active
															YLW(4) 每 YELLOW SSI Driver
															0 : Inactive
															1 : Active
															CYN(3) 每 CYAN SSI Driver
															0 : Inactive
															1 : Active
															MG(2) 每 MAGENTA SSI Driver
															0 : Inactive
															1 : Active
															WT(1) 每 WHITE SSI Driver
															0 : Inactive
															1 : Active
															TYPE(0) 每 SSI Type
															0 : Inactive (LED)
															1 : Active (Alternate Solid State Illumination source)*/	
	Driver_Capability_Packet[2] = DriverCapability_IRsupport;/*IR(7:0) 每 # of IR SSI Drive Levels Range 0 每 255*/
	Driver_Capability_Packet[3] = DriverCapability_ADCsupport;/*DB(7) 每 Bytes of ADC Data
																0 : 1 byte
																1 : 2 bytes
																ADC(6:0) 每 # of ADC Values Range 0 每 127*/
	
}

void Packet_ADCvalue(){
	ADCvalue_Packet[0] = ADCvalues_Address;/*R/W(7) 每 Read/Write
											0 : Write
											1 : Read
											Clr(6:4) 每 Color
											000 : RED Solid State Driver
											001 : GREEN Solid State Driver
											010 : BLUE Solid State Driver
											011 : YELLOW Solid State Driver
											100 : CYAN Solid State Driver
											101 : MAGENTA Solid State Driver
											110 : WHITE Solid State Driver
											111 : IR Solid State Driver
											Ctrl(3:0) 每 Control
											0101 每 ADC Values Packet*/
	ADCvalue_Packet[1] = ADCvalues_ADC1Value;/*ADC(15:0) 每 ADC Value Range 0 - 65535*/
	ADCvalue_Packet[2] = ADCvalues_ADC1or2Value;/**/
}

void Packet_vSetWholePacket(UBYTE type, UBYTE * packteData, int size){
	int i=0;
	
	if(type == 0){
		for(i=0;i<size;i++)
			DLP_pattern1[i] = *(packteData+i);
		vcom_write(&cmdRight,1);
		}
		
	else if(type == 1){
		for(i=0;i<size;i++)
			DLP_pattern2[i] = *(packteData+i);
		vcom_write(&cmdRight,1);
		}
	
	else if(type == 2){
		for(i=0;i<size;i++)
			DLP_pattern3[i] = *(packteData+i);
		vcom_write(&cmdRight,1);
		}
		
	else
		;
}
void Packet_vSetPartPacket(UBYTE type, UBYTE value1, UBYTE value2){
	if(type == 3){
		if(value1<22)
			DLP_pattern1[value1] = value2;
		vcom_write(&cmdRight,1);
		}
	else if(type == 4){
		if(value1<22)
			DLP_pattern2[value1] = value2;
		vcom_write(&cmdRight,1);
		}
	else if(type == 5){
		if(value1<22)
			DLP_pattern3[value1] = value2;
		vcom_write(&cmdRight,1);
		}
	else if(type == 6){
		DLP_pattern1[2] = g_rxBuff[2];
		DLP_pattern1[3] = g_rxBuff[3];
		
		vcom_write(&cmdRight,1);
	}
	else if(type == 7){
		DLP_pattern2[4] = g_rxBuff[2];
		DLP_pattern2[5] = g_rxBuff[3];
		
		vcom_write(&cmdRight,1);
	}
	else if(type == 8){
		
		DLP_pattern3[6] = g_rxBuff[2];
		DLP_pattern3[7] = g_rxBuff[3];
		vcom_write(&cmdRight,1);
	}	
	else
		;
		

}


void Packet_vSendPacket(){
	uint16_t dataTemp;
	int i = 1;

	//DLP_pattern1[2] = redLevel1;
	//DLP_pattern1[3] = redLevel2;
	if(redSampleFlag == 1){
		if(redPstep!=0)
			redPstep--;

		dataTemp = sample_value[redPstep];
		dataTemp = dataTemp << 6;
		if(dataTemp>redSampleMax)
			dataTemp = redSampleMax;
		
		DLP_pattern1[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern1[9] = (UBYTE)(dataTemp &0xff);
		if(redPstep == 0)
			redSampleFlag = 0;
		}
	else if(redSampleFlag == 0){
		
		dataTemp = sample_value[redPstep];
		dataTemp = dataTemp << 6;
		if(dataTemp>redSampleMax)
			dataTemp = redSampleMax;
		DLP_pattern1[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern1[9] = (UBYTE)(dataTemp &0xff);
		redPstep++;
		if(redPstep == 127)
			redSampleFlag = 1;
		}
	else;

		
	
	for(i=0;i<=22;i++){
		//dataTemp = (USHORT)((USHORT)DLP_pattern1[2*i-2] << 8);
  		//dataTemp += (USHORT)DLP_pattern1[2*i-1];
  		dataTemp = DLP_pattern1[i];
				
		SPI_vSendDataBlocking(SPI0,dataTemp);
		//if(i>0){
			
			while(!SPI_bGetStatus(SPI0, SPI_STAT_RXRDY)){;}
			dataTemp = SPI_nReceiveData(SPI0);
		//	}
		}
	
	
}
void Packet_vSendPacket2(){
	uint16_t dataTemp;
	int i = 1;
//	DLP_pattern2[4] = greenLevel1;
//	DLP_pattern2[5] = greenLevel2;
	
//
	if(greenSampleFlag == 1){
			
		if(greenPstep!=0)
			greenPstep--;

		dataTemp = sample_value[greenPstep];
		dataTemp = dataTemp << 6;
		if(dataTemp > greenSampleMax)
			dataTemp = greenSampleMax;
		DLP_pattern2[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern2[9] = (UBYTE)(dataTemp &0xff);
		if(greenPstep == 0)
			greenSampleFlag = 0;
		}
	else if(greenSampleFlag == 0){
		
		dataTemp = sample_value[greenPstep];
		dataTemp = dataTemp << 6;
		if(dataTemp > greenSampleMax)
			dataTemp = greenSampleMax;
		DLP_pattern2[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern2[9] = (UBYTE)(dataTemp &0xff);
		greenPstep++;
		if(greenPstep == 127)
			greenSampleFlag = 1;
		}
	else;
	for(i=0;i<=22;i++){
		//dataTemp = (USHORT)((USHORT)DLP_pattern1[2*i-2] << 8);
  		//dataTemp += (USHORT)DLP_pattern1[2*i-1];
		dataTemp = DLP_pattern2[i];
		
		SPI_vSendDataBlocking(SPI0,dataTemp);

	//	if(i>0){
			
			while(!SPI_bGetStatus(SPI0, SPI_STAT_RXRDY)){;}
			dataTemp = SPI_nReceiveData(SPI0);
		//	}
		}
	
	
}
void Packet_vSendPacket3(){
	uint16_t dataTemp;
	int i = 1;
	//
//	DLP_pattern3[6] = blueLevel1;
//	DLP_pattern3[7] = blueLevel2;


	if(blueSampleFlag == 1){

		if(bluePstep!=0)
			bluePstep--;

		dataTemp = sample_value[bluePstep];
		dataTemp = dataTemp << 6;
		if(dataTemp > blueSampleMax)
			dataTemp = blueSampleMax;
		DLP_pattern3[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern3[9] = (UBYTE)(dataTemp &0xff);
		if(bluePstep == 0)
			blueSampleFlag = 0;
		}
	else if(blueSampleFlag == 0){
		
		dataTemp = sample_value[bluePstep];
		dataTemp = dataTemp << 6;
		if(dataTemp > blueSampleMax)
			dataTemp = blueSampleMax;
		DLP_pattern3[8] = (UBYTE)(dataTemp >> 8);
		DLP_pattern3[9] = (UBYTE)(dataTemp &0xff);
		bluePstep++;
		if(bluePstep == 127)
			blueSampleFlag = 1;
		}
	else;
	for(i=0;i<=22;i++){
		//dataTemp = (USHORT)((USHORT)DLP_pattern1[2*i-2] << 8);
  		//dataTemp += (USHORT)DLP_pattern1[2*i-1];
		
		dataTemp = DLP_pattern3[i];
		
		SPI_vSendDataBlocking(SPI0,dataTemp);

		//if(i>0){
			
			while(!SPI_bGetStatus(SPI0, SPI_STAT_RXRDY)){;}
			dataTemp = SPI_nReceiveData(SPI0);
		//	}
		}
	
}




void Package_vSendMessageInASubframe(){
	
	
	//if(PWRCTRL_InGetFrameFlag() == 0 /*&& PWRCTRL_InGetFrameOldFlag()==1*/){
		if(IO_GPIO_teGet(IO_P0_03)==1&&sendRdy==0){
			isColorPacketSendingNow = TRUE;
			sendRdy = 1;
			firstPacketSent = 0;
			secondPacketSent = 0;
			thirdPacketSent = 0;
			//PWRCTRL_vSetFrameFlagDone();
			TIMER_vStartEx(MRT_CHANNEL_0,timeGap1,MODE_ONESHOT);//at least be 2, 1 will make it wrong
			}
		if(IO_GPIO_teGet(IO_P0_03)==0)
			sendRdy = 0;

}
void MRT_vISRz(){
	if(firstPacketSent==0 &&secondPacketSent==0&&thirdPacketSent==0/*&&transParentReady == 0*/){
		SPIdriverInterfaceRst();
		Packet_vSendPacket();
		firstPacketSent = 1;
		TIMER_vStartEx(MRT_CHANNEL_0,timeGap2,MODE_ONESHOT);
		}
	else if(firstPacketSent==1 &&secondPacketSent==0&&thirdPacketSent==0){
		SPIdriverInterfaceRst();
		Packet_vSendPacket2();
		secondPacketSent = 1;
		TIMER_vStartEx(MRT_CHANNEL_0,timeGap3,MODE_ONESHOT);
		}
	else if(firstPacketSent==1 &&secondPacketSent==1&&thirdPacketSent==0){
		SPIdriverInterfaceRst();
		Packet_vSendPacket3();
		thirdPacketSent = 1;
		isColorPacketSendingNow = false;
		}
	else{
		;
		}
		
	if(firstPacketSent==1 &&secondPacketSent==1&&thirdPacketSent==1){
		firstPacketSent = 0;
		secondPacketSent = 0;
		thirdPacketSent = 0;
	/*	if(transParentFlag==1){
			transParentReady = 1;
			TIMER_vStartEx(MRT_CHANNEL_0,400,MODE_ONESHOT);
			return;
			}*/
		}
/*	if(transParentReady == 1){
		Package_vTransParent(transParentLength,transParentFlag);
	//		Package_vTransParent(64);
			transParentLength = 0;
			transParentReady = 0;
		}*/
	
}


//before send a packet, SSI_DRV_DIRZ ladi zai lagao
void SPIdriverInterfaceRst(){
	U32 portStatue=0;
	int i = 500;

	portStatue = IO_GPIO_unGetPortMaskedState(IO_PORT_0);
	portStatue &= ~(1<<PIN_SSI_DRV_DIRZ);
	IO_GPIO_vSetPortMaskedState(IO_PORT_0,portStatue);
	while(i>=0)
		i--;
	portStatue |= (1<<PIN_SSI_DRV_DIRZ);
	IO_GPIO_vSetPortMaskedState(IO_PORT_0,portStatue);
	i=500;
	while(i>=0)
		i--;
}


void Packet_vSetGapTime(UBYTE type, UBYTE time){
	if(type == 0&&time>0){
		timeGap1 = time * 100;
		
	vcom_write(&cmdRight,1);
	}
	else if (type == 1&&time>0){
		timeGap2 = time * 100;
		vcom_write(&cmdRight,1);
		}
		
	else if (type == 2&&time>0){
		timeGap3 = time * 100;
		vcom_write(&cmdRight,1);
	}
	else
		;
}



void Package_vTransParent(int length,int flag){
	int i = 0;
	UBYTE dataTemp=0;
	if(flag !=0){
		SPIdriverInterfaceRst();
		for(i=0;i<length;i++){
			SPI_vSendDataBlocking(SPI0,transParentData[i]);

			while(!SPI_bGetStatus(SPI0, SPI_STAT_RXRDY)){;}
			dataTemp = SPI_nReceiveData(SPI0);
			transParentData[i] = dataTemp;
				
			//vcom_write(&dataTemp,1);
		}
		
		//if(flag == 1)
		//	vcom_write(&cmdRight,1);
		//else if(flag == 2){
		//	vcom_write(&cmdRight,1);

		if(flag == 1){
		        transParentData[0] = cmdRight;
			vcom_write(&transParentData[0],1);
			}
		else if(flag == 2){
			transParentData[0] = cmdRight;
			//for(i=0;i<1000;i++);
			
			//if(length-1==64||(length-1)%64==0)
			//	vcom_write(&transParentData[1],length);
			//else
			//	vcom_write(&transParentData[1],length-1);		
/*
			if(((length -1)%64) == 0)
				vcom_write(&transParentData[0],length+1);
			else
				vcom_write(&transParentData[0],length);	
				*/
			if(length <=63)
				vcom_write(&transParentData[0],length);
		
			else if(length<=126){
				vcom_write(&transParentData[0],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[63],length-63);
				}
			else if(length<=189){
				vcom_write(&transParentData[0],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[63],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[126],length-126);
				}
			else{
				vcom_write(&transParentData[0],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[63],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[126],63);
				for(i=0;i<3000;i++);
				vcom_write(&transParentData[189],length-189);

				}
		}
		else;

		for(i=0;i<length;i++)
			transParentData[i] = 0;
		
		//vcom_write("/r/n",2)	
	}
	transParentFlag = 0;
	

}

void Package_vGetPacketParas(int packetNum){
	int i = 0;

	if(packetNum==0){
		vcom_write(&cmdRight,1);
		for(i=0;i<1000;i++);
		vcom_write(DLP_pattern1,22);

		}
	else if(packetNum==1)
		{
		vcom_write(&cmdRight,1);
		for(i=0;i<1000;i++);
		vcom_write(DLP_pattern2,22);

		}
	else if(packetNum==2)
		{
		vcom_write(&cmdRight,1);
		for(i=0;i<1000;i++);
		vcom_write(DLP_pattern3,22);

		}
	else;


}





