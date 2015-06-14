#include "platform.h"
#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>
#include "UI.h"
#include "crc.h"
#define SUBFRAME_ADDRESS_BASE 0x03200130
#define KENGTIME_ADDRESS_BASE 0x03200160

#define RED_DLP_BASE 0x032001C0
#define GREEN_DLP_BASE 0x032003C0
#define BLUE_DLP_BASE 0x032005C0

#define SubframePointerAds 0x03200120
#define KengTimePointerAds 0x03200121
#define RedPointerAds 0x03200122
#define GreenPointerAds 0x03200123
#define BluePointerAds 0x03200124

#define ModeCtlAds 0x03200125

#define COLORSEQ0_CRC 0x03200126
#define COLORSEQ1_CRC 0x03200127
#define COLORSEQ2_CRC 0x03200128
#define COLORSEQ3_CRC 0x03200129

#define SEQLENGTH0  0x0320012A
#define SEQLENGTH1  0x0320012B
#define SEQLENGTH2  0x0320012C
#define SEQLENGTH3  0x0320012D

#define AUTOPLAY  0x0320012E

void Measure_vLoadMode(void);
void Measure_vLoadData(int unit);
void Measure_vStoreData(int i);

void Measure_vStoreMode(void);
int Measure_iStoreSubframe(int offsetNumber);
int Measure_iStoreSubframeFullIns(int offsetNumber,UBYTE high, UBYTE low);
int Measure_iLoadSubframe(int offsetNumber);
int Measure_iStoreDLPpat1(int offsetNumber);
int Measure_iStoreDLPpat1FullIns(int offsetNumber, UBYTE * pointer, int size);
int Measure_iLoadDLPpat1(int offsetNumber);

int Measure_iStoreDLPpat2(int offsetNumber);
int Measure_iStoreDLPpat2FullIns(int offsetNumber, UBYTE * pointer, int size);
int Measure_iLoadDLPpat2(int offsetNumber);
int Measure_iStoreDLPpat3(int offsetNumber);
int Measure_iStoreDLPpat3FullIns(int offsetNumber, UBYTE * pointer, int size);
int Measure_iLoadDLPpat3(int offsetNumber);

int Measure_iStoreKengtime(int offsetNumber);
int Measure_iStoreKengtimeFullIns(int offsetNumber,UBYTE keng);

int Measure_iLoadKengtime(int offsetNumber);

void Measure_vSetPointer(void);
void Measure_vModeCtl(UBYTE modeFlag);
UBYTE CRC_ubCrc8(const UBYTE *pubData, UBYTE ubCrc, UBYTE ubNrOfBytes);
int Measure_iSetAutoPlay(UBYTE offset);

UBYTE Measure_ubLoadAutoPlay();

void Measure_vGetSubframeEESetting();
void Measure_vGetGapEESetting();
void Measure_vGetRedPacketEESetting(int i);
void Measure_vGetGreenPacketEESetting(int i);
void Measure_vGetBluePacketEESetting(int i);
void Measure_vSetSubframeEESetting(UBYTE * pointer);

void Measure_vSetGapEESetting(UBYTE * pointer);
void Measure_vSetRedPktEESetting(UBYTE i, UBYTE * pointer );

void Measure_vSetGreenPktEESetting(UBYTE i, UBYTE * pointer );
void Measure_vSetBluePktEESetting(UBYTE i, UBYTE * pointer );
