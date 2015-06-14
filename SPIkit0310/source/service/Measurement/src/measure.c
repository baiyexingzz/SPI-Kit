 
#include "measure.h"
#include "memory.h"
#include "platform.h"
#include "system.h"
#include "stdtypes.h"
#include "io.h"
#include "pwrctrl.h"
#include "package.h"

#include "app_usbd_cfg.h"
#include "cdc_vcom.h"
#include "crc.h"
#include "instruction.h"


extern USHORT subFramePeriod;
extern UBYTE kengTime;
extern UBYTE ubColorBuff[250];
extern USHORT ubColorTimeBuff[250];

extern UBYTE DLP_pattern1[23];
extern UBYTE DLP_pattern2[23];
extern UBYTE DLP_pattern3[23];
extern UBYTE modeCtl;

extern bool vcom_enable;

UBYTE subframeOffsetNumber = 0;

UBYTE subframePointer = 0;
UBYTE kengTimePointer = 0;
UBYTE redPointer = 0;
UBYTE greenPointer = 0;
UBYTE bluePointer = 0;


UBYTE autoplay = 0;
/*
UBYTE subframeCRC[16] = {0xfe};
UBYTE kengtimeCRC[16] = {0xfe};
UBYTE redPacketCRC[16] = {0xfe};
UBYTE greenPacketCRC[16] = {0xfe};
UBYTE bluePacketCRC[16] = {0xfe};
*/
extern UBYTE cmdRight;
extern UBYTE cmdError;
extern UBYTE cmdFormatError;
extern UBYTE eeError;

void Measure_vLoadMode(){
	uint32_t address;
	uint8_t *ptr;
	uint8_t ret_code=0;

	//load pointer
	ptr = (uint8_t *)&subframePointer;
	address = SubframePointerAds;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	address = KengTimePointerAds;
	ptr = (uint8_t *)&kengTimePointer;	
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	address = RedPointerAds;
	ptr = (uint8_t *)&redPointer;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	address = GreenPointerAds;
	ptr = (uint8_t *)&greenPointer;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	address = BluePointerAds;
	ptr = (uint8_t *)&bluePointer;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	if(ret_code == 0)
		vcom_write(&cmdRight,1);

}
void Measure_vLoadData(int unit){
	int ret_code = 0;
	int j;
	/*ret_code += Measure_iLoadSubframe(subframePointer);
	ret_code += Measure_iLoadKengtime(kengTimePointer);
	ret_code += Measure_iLoadDLPpat1(redPointer);
	ret_code += Measure_iLoadDLPpat2(greenPointer);
	ret_code += Measure_iLoadDLPpat3(bluePointer);
*/
	vcom_enable = FALSE;
	
	ret_code += Measure_iLoadDLPpat2(unit);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++)
	;
	ret_code += Measure_iLoadSubframe(unit);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++)
	;
	ret_code += Measure_iLoadKengtime(unit);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++)
	;
	ret_code += Measure_iLoadDLPpat1(unit);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++)
	;
	
	ret_code += Measure_iLoadDLPpat3(unit);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	vcom_enable = TRUE;
	vcom_write(&cmdRight,1);
	//if(ret_code == 0)
	//	vcom_write("LoadRight\r\n",11);
}
void Measure_vStoreData(int i){
	int ret_code = 0;
	int j;
	/*ret_code += Measure_iStoreSubframe(subframePointer);
	ret_code += Measure_iStoreKengtime(kengTimePointer);
	ret_code += Measure_iStoreDLPpat1(redPointer);
	ret_code += Measure_iStoreDLPpat2(greenPointer);
	ret_code += Measure_iStoreDLPpat3(bluePointer);*/
	vcom_enable = FALSE;
	if(i>=16)
		i=15;
	ret_code += Measure_iStoreDLPpat2(i);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++);
	ret_code += Measure_iStoreSubframe(i);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++);
	ret_code += Measure_iStoreKengtime(i);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++);
	ret_code += Measure_iStoreDLPpat1(i);

	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}
	for(j=1;j<10000;j++);
	
	ret_code += Measure_iStoreDLPpat3(i);
	if(ret_code != 0){
		vcom_enable = TRUE;
		vcom_write(&eeError,1);
		return;
		}

	Measure_vModeCtl(1);
	//if(i>=3)
		//i = 3;
		i = i % 4;
	if(Pwrctrl_vStoreColorSeqToEE(i) == FALSE)
		return;
	

	
	vcom_enable = TRUE;
	vcom_write(&cmdRight,1);

	//if(ret_code == 0)
	//	vcom_write("StoreRight\r\n",12);
 
}
void Measure_vStoreMode(){
	uint32_t address = SubframePointerAds;
	uint8_t *ptr = (uint8_t *)&subframePointer;
	uint8_t ret_code=0;

	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	address = KengTimePointerAds;
	ptr = (uint8_t *)&kengTimePointer;	
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	address = RedPointerAds;
	ptr = (uint8_t *)&redPointer;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	address = GreenPointerAds;
	ptr = (uint8_t *)&greenPointer;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	address = BluePointerAds;
	ptr = (uint8_t *)&bluePointer;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);


	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
}

int Measure_iStoreSubframe(int offsetNumber){
	uint32_t address = SUBFRAME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&subFramePeriod;
	uint8_t ret_code=0;
	UBYTE high,low,crc;
	UBYTE sf[2];

	subframePointer = offsetNumber;
	high = (UBYTE)(subFramePeriod>>8);
	low = (UBYTE)(subFramePeriod&0xff);
	sf[0] = high;
	sf[1] = low;
	address += offsetNumber*3;
	ptr = (uint8_t *)sf;
	ret_code = MEMORY_tWrite(ptr,address,2,MEMTYPE_EEPROM);
	address += 2;
	crc = CRC_ubCrc8(sf, 0, 2);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	/*high = (UBYTE)(subFramePeriod>>8);
	address += offsetNumber*2;
	ptr = (uint8_t *)&high;
	ret_code = MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	low = (UBYTE)(subFramePeriod&0xff);
	address += offsetNumber*2+1;
	ptr = (uint8_t *)&low;
	ret_code = MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);*/
	return ret_code;
}
int Measure_iStoreSubframeFullIns(int offsetNumber,UBYTE high, UBYTE low){
	uint32_t address = SUBFRAME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&subFramePeriod;
	uint8_t ret_code=0;
	UBYTE crc;
	UBYTE sf[2];

	//high = (UBYTE)(subFramePeriod>>8);
	//low = (UBYTE)(subFramePeriod&0xff);
	subframePointer = offsetNumber;
	sf[0] = high;
	sf[1] = low;
	address += offsetNumber*3;
	ptr = (uint8_t *)sf;
	ret_code = MEMORY_tWrite(ptr,address,2,MEMTYPE_EEPROM);
	address += 2;
	crc = CRC_ubCrc8(sf, 0, 2);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&eeError,1);
	
	return ret_code;
}
int Measure_iLoadSubframe(int offsetNumber){
	uint32_t address = SUBFRAME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&subFramePeriod;
	uint8_t ret_code=0;
	UBYTE high,low;
	UBYTE sf[2]= {0x11};
	UBYTE crcLoad=0xef,crcCalc=0xfe;

	address += offsetNumber*3;
	ptr = (uint8_t *)sf;
	ret_code = MEMORY_tRead(ptr,address,2,MEMTYPE_EEPROM);

	
	address += 2;
	ptr = (uint8_t *)&crcLoad;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);
	

	crcCalc = CRC_ubCrc8(sf,0,2);
	
		if(ret_code==0 && crcCalc == crcLoad && crcLoad!=0){
			if(vcom_enable == TRUE)
				vcom_write(&cmdRight,1);
			subFramePeriod = (USHORT)((USHORT)sf[0] << 8);
	  		subFramePeriod += (USHORT)sf[1];
			}
		else{
			if(vcom_enable == TRUE)
				vcom_write(&eeError,1);
			}
		
/*
	address += offsetNumber*2;
	ptr = (uint8_t *)&high;
	ret_code = MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	address += offsetNumber*2+1;
	ptr = (uint8_t *)&low;
	ret_code = MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	subFramePeriod = (USHORT)((USHORT)high << 8);
  	subFramePeriod += (USHORT)low;
	*/
	return ret_code;
}


int Measure_iStoreDLPpat1(int offsetNumber){
	uint32_t address = RED_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern1;
	uint8_t ret_code =0;
	UBYTE crc=0;

	redPointer = offsetNumber;

	address += offsetNumber*32;
	ret_code = MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(DLP_pattern1, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	
	return ret_code;
}
int Measure_iStoreDLPpat1FullIns(int offsetNumber, UBYTE * pointer, int size){
	uint32_t address = RED_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern1;
	uint8_t ret_code = 0;
	UBYTE crc=0;
	UBYTE temp[22]={0};
	int i;

	redPointer = offsetNumber;

	for(i=0;i<size;i++)
		temp[i] = *(pointer+i);
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code = MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(temp, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&eeError,1);
	
	return ret_code;
}
int Measure_iLoadDLPpat1(int offsetNumber){
	uint32_t address = RED_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern1;
	uint8_t ret_code = 0;
	UBYTE crcLoad=0xef,crcCalc=0xfe;
	UBYTE temp[22];
	int i;
	
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code += MEMORY_tRead(ptr,address,22,MEMTYPE_EEPROM);
	crcCalc = CRC_ubCrc8(temp, 0, 22);

	address += 22;
	ptr = (uint8_t *)&crcLoad;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);


		if(crcCalc == crcLoad && crcLoad!=0){
			if(vcom_enable == TRUE)
				vcom_write(&cmdRight,1);
			for(i=0;i<22;i++)
				DLP_pattern1[i] = temp[i];
			}
		else
			if(vcom_enable == TRUE)
				vcom_write(&eeError,1);
		
	return ret_code;
}
int Measure_iStoreDLPpat2(int offsetNumber){
	uint32_t address = GREEN_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern2;
	uint8_t ret_code=0;
	UBYTE crc=0;

	greenPointer = offsetNumber;

	address += offsetNumber*32;
	ret_code = MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(DLP_pattern2, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	return ret_code;
}
int Measure_iStoreDLPpat2FullIns(int offsetNumber, UBYTE * pointer, int size){
	uint32_t address = GREEN_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern2;
	uint8_t ret_code=0;
	UBYTE crc=0;
	UBYTE temp[23]={0};
		int i;

	greenPointer = offsetNumber;

	for(i=0;i<size;i++)
		temp[i] = *(pointer+i);
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code += MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(temp, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&eeError,1);
	
	return ret_code;
}
int Measure_iLoadDLPpat2(int offsetNumber){
	uint32_t address = GREEN_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern2;
	uint8_t ret_code=0;
	UBYTE crcLoad=0xef,crcCalc=0xfe;
	UBYTE temp[22];
	int i;
	
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code = MEMORY_tRead(ptr,address,22,MEMTYPE_EEPROM);
	crcCalc = CRC_ubCrc8(temp, 0, 22);

	address += 22;
	ptr = (uint8_t *)&crcLoad;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);


		if(crcCalc == crcLoad&& crcLoad!=0){
			if(vcom_enable == TRUE)
				vcom_write(&cmdRight,1);
			for(i=0;i<22;i++)
				DLP_pattern2[i] = temp[i];
			}
		else
			if(vcom_enable == TRUE)
				vcom_write(&eeError,1);
		
	return ret_code;
}
int Measure_iStoreDLPpat3(int offsetNumber){
	uint32_t address = BLUE_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern3;
	uint8_t ret_code=0;
	UBYTE crc=0;

	bluePointer = offsetNumber;
	address += offsetNumber*32;
	ret_code = MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(DLP_pattern3, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	
	return ret_code;
}
int Measure_iStoreDLPpat3FullIns(int offsetNumber, UBYTE * pointer, int size){
	uint32_t address = BLUE_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern3;
	uint8_t ret_code=0;
	UBYTE crc=0;
	UBYTE temp[22]={0};
	int i;

	bluePointer = offsetNumber;
	for(i=0;i<size;i++)
		temp[i] = *(pointer+i);
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code += MEMORY_tWrite(ptr,address,22,MEMTYPE_EEPROM);

	address += 22;
	crc = CRC_ubCrc8(temp, 0, 22);
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&eeError,1);
	
	return ret_code;
}
int Measure_iLoadDLPpat3(int offsetNumber){
	uint32_t address = BLUE_DLP_BASE;
	uint8_t *ptr = (uint8_t *)DLP_pattern3;
	uint8_t ret_code=0;
	UBYTE crcLoad=0xef,crcCalc=0xfe;
	UBYTE temp[22];
	int i;
	
	ptr = (uint8_t *)temp;
	address += offsetNumber*32;
	ret_code = MEMORY_tRead(ptr,address,22,MEMTYPE_EEPROM);
	crcCalc = CRC_ubCrc8(temp, 0, 22);

	address += 22;
	ptr = (uint8_t *)&crcLoad;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	
		if(crcCalc == crcLoad&& crcLoad!=0){
			if(vcom_enable == TRUE)
				vcom_write(&cmdRight,1);
			for(i=0;i<22;i++)
				DLP_pattern3[i] = temp[i];
			}
		else
			if(vcom_enable == TRUE)
				vcom_write(&eeError,1);
		
	return ret_code;
}



int Measure_iStoreKengtime(int offsetNumber){
	uint32_t address = KENGTIME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&kengTime;
	uint8_t ret_code = 0;
	UBYTE crc;

	kengTimePointer = offsetNumber;
	address += offsetNumber*2;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	crc = CRC_ubCrc8(&kengTime, 0, 1);
	address++;
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(vcom_enable == TRUE){
		if(ret_code == 0)
			vcom_write(&cmdRight,1);
		else
			vcom_write(&eeError,1);
		}
	return ret_code;
}
int Measure_iStoreKengtimeFullIns(int offsetNumber,UBYTE keng){
	uint32_t address = KENGTIME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&keng;
	uint8_t ret_code = 0;
	UBYTE crc;

	kengTimePointer = offsetNumber;
	
	address += offsetNumber*2;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	crc = CRC_ubCrc8(&keng, 0, 1);
	address++;
	ptr = (uint8_t *)&crc;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	
	if(ret_code == 0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&eeError,1);

	return ret_code;
}
int Measure_iLoadKengtime(int offsetNumber){
	uint32_t address = KENGTIME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&kengTime;
	uint8_t ret_code=0;
	UBYTE crcLoad=0xef,crcCalc=0xfe,temp;

	ptr = (uint8_t *)&temp;
	address += offsetNumber*2;
	ret_code = MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	crcCalc = CRC_ubCrc8(&temp, 0, 1);
	address++;
	ptr = (uint8_t *)&crcLoad;
	ret_code += MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	
		if(crcCalc == crcLoad&& crcLoad!=0){
			if(vcom_enable == TRUE)
				vcom_write(&cmdRight,1);
			kengTime = temp;
			}
		else{
			if(vcom_enable == TRUE)
				vcom_write(&eeError,1);
			}
		
	return ret_code;
}



void Measure_vModeCtl(UBYTE modeFlag){
	uint32_t address = ModeCtlAds;
	uint8_t *ptr = (uint8_t *)&modeCtl;
	uint8_t ret_code=0;
	
	
	modeCtl = modeFlag;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);

	if(ret_code == 0);
		//vcom_write("modeCtl\r\n",9);
}

UBYTE CRC_ubCrc8(const UBYTE *pubData, UBYTE ubCrc, UBYTE ubNrOfBytes)
/*
**  Description : Calculates the 8-bit CRC with a polynomial of 0x107
**  Parameters  : ubCrc: CRC seed value, normally zero
**  Returns     : The 8-bit CRC
*/
{
  static const UBYTE aubNibbleLow[] =
  {
    0x00, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83,
    0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
  };

  static const UBYTE aubNibbleHigh[] =
  {
    0x00, 0x9D, 0x23, 0xBE, 0x46, 0xDB, 0x65, 0xF8,
    0x8C, 0x11, 0xAF, 0x32, 0xCA, 0x57, 0xE9, 0x74
  };

  UBYTE ubIndex;

  while (ubNrOfBytes-- > 0)
  {
    ubIndex = (UBYTE)(*pubData++ ^ ubCrc);
    ubCrc = (UBYTE)(aubNibbleLow[ubIndex & 0x0F] ^ aubNibbleHigh[ubIndex >> 4]);
  }

  return (ubCrc);
}


int Measure_iSetAutoPlay(UBYTE offset){
	uint32_t address = AUTOPLAY;
	uint8_t *ptr = (uint8_t *)&autoplay;
	uint8_t ret_code=0;
	
	autoplay = offset;
	ret_code += MEMORY_tWrite(ptr,address,1,MEMTYPE_EEPROM);
	if(autoplay !=0)
		vcom_write(&cmdRight,1);
	else
		vcom_write(&cmdRight,1);
	return ret_code;
}

UBYTE Measure_ubLoadAutoPlay(){
	uint32_t address = AUTOPLAY;
	uint8_t *ptr = (uint8_t *)&autoplay;
	uint8_t ret_code=0;
	
	ret_code = MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	return autoplay;
}

void Measure_vGetSubframeEESetting(){

	uint8_t temp[48] = {0};
	uint32_t address = SUBFRAME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&temp;
	uint8_t ret_code=0;
	
	ret_code = MEMORY_tRead(ptr,address, 48,MEMTYPE_EEPROM);

	vcom_write(temp,48);
	
}
void Measure_vGetGapEESetting(){

	uint8_t temp[32] = {0};
	uint32_t address = KENGTIME_ADDRESS_BASE;
	uint8_t *ptr = (uint8_t *)&temp;
	uint8_t ret_code=0;
	
	ret_code = MEMORY_tRead(ptr,address, 32,MEMTYPE_EEPROM);
	vcom_write(temp,32);
}
void Measure_vGetRedPacketEESetting(int i){

	uint8_t temp[23] = {0};
	uint32_t address = RED_DLP_BASE;
	uint8_t *ptr = (uint8_t *)&temp;
	uint8_t ret_code=0;
	address += i*32;
	ret_code = MEMORY_tRead(ptr,address, 23,MEMTYPE_EEPROM);
	vcom_write(temp,23);
}
void Measure_vGetGreenPacketEESetting(int i){

	uint8_t temp[23] = {0};
	uint32_t address = GREEN_DLP_BASE;
	uint8_t *ptr = (uint8_t *)&temp;
	uint8_t ret_code=0;
	address += i*32;
	ret_code = MEMORY_tRead(ptr,address, 23,MEMTYPE_EEPROM);
	vcom_write(temp,23);
}
void Measure_vGetBluePacketEESetting(int i){

	uint8_t temp[23] = {0};
	uint32_t address = BLUE_DLP_BASE;
	uint8_t *ptr = (uint8_t *)&temp;
	uint8_t ret_code=0;
	address += i*32;
	ret_code = MEMORY_tRead(ptr,address, 23,MEMTYPE_EEPROM);
	vcom_write(temp,23);
}

void Measure_vSetSubframeEESetting(UBYTE * pointer){
	uint32_t address = SUBFRAME_ADDRESS_BASE;
	uint8_t ret_code=0;
	
	ret_code = MEMORY_tWrite(pointer,address, 48,MEMTYPE_EEPROM);
	vcom_write(&cmdRight,1);
}
void Measure_vSetGapEESetting(UBYTE * pointer){
	uint8_t ret_code=0;
	uint32_t address = KENGTIME_ADDRESS_BASE;
	
	ret_code = MEMORY_tWrite(pointer,address, 32,MEMTYPE_EEPROM);
	vcom_write(&cmdRight,1);
}
void Measure_vSetRedPktEESetting(UBYTE i, UBYTE * pointer ){
	uint8_t ret_code=0;
	uint32_t address = RED_DLP_BASE;
	address += i * 32;
	ret_code = MEMORY_tWrite(pointer,address, 23,MEMTYPE_EEPROM);
	vcom_write(&cmdRight,1);
}

void Measure_vSetGreenPktEESetting(UBYTE i, UBYTE * pointer ){
	uint8_t ret_code=0;
	uint32_t address = GREEN_DLP_BASE;
	address += i * 32;
	ret_code = MEMORY_tWrite(pointer,address, 23,MEMTYPE_EEPROM);
	vcom_write(&cmdRight,1);
}

void Measure_vSetBluePktEESetting(UBYTE i, UBYTE * pointer ){
	uint8_t ret_code=0;
	uint32_t address = BLUE_DLP_BASE;
	address += i * 32;
	ret_code = MEMORY_tWrite(pointer,address, 23,MEMTYPE_EEPROM);
	vcom_write(&cmdRight,1);
}



