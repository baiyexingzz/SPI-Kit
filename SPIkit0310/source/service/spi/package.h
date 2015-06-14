#include "platform.h"
#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>




void Packet_DLP(void);
void Packet_Timing(void);
void Packet_ID(void);
void Packet_Status(void);
void Packet_Driver_Capability_Packet(void);

void Packet_ADCvalue(void);


void Packet_vSetWholePacket(UBYTE type, UBYTE * packteData, int size);
void Packet_vSetPartPacket(UBYTE type, UBYTE value1, UBYTE value2);
void Packet_vSetPacket(void);

void Packet_vSendPacket(void);
void Packet_vSendPacket2(void);
void Packet_vSendPacket3(void);


void Package_vSendMessageInASubframe(void);

void MRT_vISRz(void);
void SPIdriverInterfaceRst(void);

void Packet_vSetGapTime(UBYTE type, UBYTE time);
void Package_vTransParent(int length,int flag);
void Package_vGetPacketParas(int packetNum);



