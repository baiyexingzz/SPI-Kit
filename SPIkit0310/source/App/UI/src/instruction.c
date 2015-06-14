#include "platform.h"
#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>
#include "UI.h"
#include "pwrctrl.h"
#include "measure.h"
#include "instruction.h"
#include "package.h"

UBYTE insType = 0;
UBYTE insTypeH = 0,insTypeL = 0;

extern UBYTE ubSEQbuff[512],insLength;
extern UBYTE g_rxBuff[512];
extern uint8_t usb_buff[64];
extern UBYTE transParentData[253];
extern int transParentFlag;
extern UBYTE transParentLength; 

extern int redPstep ;
extern int greenPstep ;
extern int bluePstep ;


extern USHORT redSampleMax ;
extern USHORT greenSampleMax ;
extern USHORT blueSampleMax ;

extern UBYTE cmdRight;
extern UBYTE cmdError;
extern UBYTE cmdFormatError;
extern UBYTE eeError;

void Instruction_vGetCmd(){
	int i;
	
	insType = g_rxBuff[1];
	insTypeH = ((insType&0x70)>>4);
	insTypeL =  insType&0x0f;
	
	if(insTypeH == 1){
		//Measure_vModeCtl(g_rxBuff[2]);
		if(insTypeL == 0&&insLength==4)
			//Measure_vLoadData();
			StartModeSel(g_rxBuff[2]);
		else if(insTypeL == 1&&insLength==4)
			Measure_vStoreData(g_rxBuff[2]);
		else if(insTypeL == 2&&insLength == 4)
			Measure_iSetAutoPlay(g_rxBuff[2]);
		else if(insTypeL == 3 && insLength == 3)
			vcom_write(VERSION,3);
		else
			vcom_write(&cmdFormatError,1);
		}
	else if(insTypeH == 2){
		if(insTypeL==0){
			memcpy((char *)ubSEQbuff,g_rxBuff,insLength);
			PWRCTRL_vGetColorSEQ();
			}
		else if(insTypeL==1){
			for(i=0;i<insLength-3;i++)
				transParentData[i] = g_rxBuff[i+2];
			transParentFlag=1;
			//transParentLength = insLength - 3;
			//transParentLength=67;
			transParentLength=134;
			}
		else if(insTypeL==2){
			transParentData[0] = g_rxBuff[2];
			transParentFlag=2;
			transParentLength = g_rxBuff[3]+1;
			}
		else if(insTypeL==6){
			Measure_vGetSubframeEESetting();
			}
		else if(insTypeL==7){
			Measure_vGetGapEESetting();
			}
		else if(insTypeL == 8){
			Measure_vGetRedPacketEESetting(g_rxBuff[2]);
			}
		else if(insTypeL == 9){
			Measure_vGetGreenPacketEESetting(g_rxBuff[2]);
			}
		else if(insTypeL == 10){
			Measure_vGetBluePacketEESetting(g_rxBuff[2]);
			}
		else if(insTypeL == 11){
			Measure_vSetSubframeEESetting(&g_rxBuff[2]);
			}
		else if(insTypeL == 12){
			Measure_vSetGapEESetting(&g_rxBuff[2]);
			}
		else if(insTypeL == 13){
			Measure_vSetRedPktEESetting(g_rxBuff[2],&g_rxBuff[3]);
			}
		else if(insTypeL == 14){
			Measure_vSetGreenPktEESetting(g_rxBuff[2],&g_rxBuff[3]);
			}
		else if(insTypeL == 15){
			Measure_vSetBluePktEESetting(g_rxBuff[2],&g_rxBuff[3]);
			}
		else vcom_write(&cmdFormatError,1);;
		}
	else if(insTypeH == 3){
		if(insTypeL == 0&&insLength==5)
			Pwrctrl_vSetSubframePeriod(g_rxBuff[2],g_rxBuff[3]);
		else if(insTypeL == 1&&insLength==4)
			Pwrctrl_vSetKengTime(g_rxBuff[2]);
		else if(insTypeL == 2 && insLength==6){
			redPstep = g_rxBuff[2];
			redSampleMax = (USHORT)(g_rxBuff[3]*256+g_rxBuff[4]);
			vcom_write(&cmdRight,1);
			}
		else if(insTypeL == 3 && insLength==6){
			greenPstep = g_rxBuff[2];
			greenSampleMax = (USHORT)(g_rxBuff[3]*256+g_rxBuff[4]);
			vcom_write(&cmdRight,1);
			}
		else if(insTypeL == 4 && insLength==6){
			bluePstep = g_rxBuff[2];
			blueSampleMax = (USHORT)(g_rxBuff[3]*256+g_rxBuff[4]);
			vcom_write(&cmdRight,1);
			}
		else
			vcom_write(&cmdFormatError,1);
		}
	else if(insTypeH == 4){
		//Packet_vSetPacket(insTypeL);
		if(insTypeL<=2&&insLength==25)
			Packet_vSetWholePacket(insTypeL,&g_rxBuff[2],22);	
		else if(insTypeL>2 &&insTypeL<9&&insLength==5)
			Packet_vSetPartPacket(insTypeL,g_rxBuff[2],g_rxBuff[3]);
		else if(insTypeL==10)
			Pwrctrl_vGetSubframePeriod();
		else if(insTypeL==11)
			Pwrctrl_vGerKengTime();
		else if(insTypeL==12 && insLength==3)
			Package_vGetPacketParas(0);
		else if(insTypeL==13 && insLength==3)
			Package_vGetPacketParas(1);
		else if(insTypeL==14 && insLength==3)
			Package_vGetPacketParas(2);
		else
			vcom_write(&cmdFormatError,1);
			}

	else if(insTypeH == 5){
		if(insTypeL == 0&&insLength==4&&g_rxBuff[2]<4)
			Pwrctrl_vStoreColorSeqToEE(g_rxBuff[2]);
		else if(insTypeL == 1&&insLength==4&&g_rxBuff[2]<4)
			Pwrctrl_vReadColorSeqFromEE(g_rxBuff[2]);
		else
			vcom_write(&cmdFormatError,1);
		}
	else if(insTypeH == 6){
		//
		if(insTypeL == 0){
			if(insLength == 6&&g_rxBuff[2]<16)
				Measure_iStoreSubframeFullIns(g_rxBuff[2], g_rxBuff[3], g_rxBuff[4]);
			else if(insLength==4&&g_rxBuff[2]<16)
				Measure_iStoreSubframe(g_rxBuff[2]);
			else
				vcom_write(&cmdFormatError,1);
			}
		else if(insTypeL == 1){
			if(insLength == 5&&g_rxBuff[2]<16)
				Measure_iStoreKengtimeFullIns(g_rxBuff[2], g_rxBuff[3]);
			else if(insLength==4&&g_rxBuff[2]<16)
				Measure_iStoreKengtime(g_rxBuff[2]);
			else
				vcom_write(&cmdFormatError,1);
			}
		else if(insTypeL == 2){
			if(insLength == 26&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat1FullIns(g_rxBuff[2], &g_rxBuff[3], 22);
			else if(insLength==4&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat1(g_rxBuff[2]);
			else
				vcom_write(&cmdFormatError,1);
			}
		else if(insTypeL == 3){
			if(insLength == 26&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat2FullIns(g_rxBuff[2], &g_rxBuff[3], 22);
			else if(insLength == 4&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat2(g_rxBuff[2]);
			else
				vcom_write(&cmdFormatError,1);
			}
		else if(insTypeL == 4){
			if(insLength == 26&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat3FullIns(g_rxBuff[2], &g_rxBuff[3], 22);
			else if(insLength == 4&&g_rxBuff[2]<16)
				Measure_iStoreDLPpat3(g_rxBuff[2]);
			else
				vcom_write(&cmdFormatError,1);
			}
		else if(insTypeL == 5&&insLength==4&&g_rxBuff[2]<16)
			Measure_iLoadSubframe(g_rxBuff[2]);
		else if(insTypeL == 6&&insLength==4&&g_rxBuff[2]<16)
			Measure_iLoadKengtime(g_rxBuff[2]);
		else if(insTypeL == 7&&insLength==4&&g_rxBuff[2]<16)
			Measure_iLoadDLPpat1(g_rxBuff[2]);
		else if(insTypeL == 8&&insLength==4&&g_rxBuff[2]<16)
			Measure_iLoadDLPpat2(g_rxBuff[2]);
		else if(insTypeL == 9&&insLength==4&&g_rxBuff[2]<16)
			Measure_iLoadDLPpat3(g_rxBuff[2]);
		else 
			vcom_write(&cmdFormatError,1);
		}
	
	else if(insTypeH == 7 && insLength==4)
		Packet_vSetGapTime(insTypeL, g_rxBuff[2]);
	else
		vcom_write(&cmdFormatError,1);
	for(i=0;i<64;i++)
		usb_buff[i] = 0;
		
}

