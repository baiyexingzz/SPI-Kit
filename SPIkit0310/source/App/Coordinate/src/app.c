//Version 0.1 
//can generate color sequence and send packet through SPI
//can store or load subframe strobe gaptime ,RGB packet , color sequence into or from EEprom
//can send data transparently


#include "platform.h"
#include "system.h"
#include "io.h"
#include "memory.h" 

#include <string.h>
#include "stdtypes.h"
#include <stdio.h>
#include <math.h>
#include "UI.h"
#include "pwrctrl.h"
#include <string.h>
#include "app_usbd_cfg.h"
#include "cdc_vcom.h"
#include "ssi.h"
#include "package.h"
#include "timer.h"
#include "measure.h"
#include "instruction.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define TICKRATE_HZ   (1000)	/* 1000 ticks per second / every 1ms */


USBD_HANDLE_T g_hUsb;
uint8_t g_rxBuff[512];
const  USBD_API_T *g_pUsbApi;

uint8_t usb_buff[64];
int dataPieceCnt = 0;
int insLength = 0, insPiece = 0;
bool vcom_enable = FALSE;

UBYTE modeCtl = 0;

extern int transParentFlag;
//extern UBYTE transParentData[253] = {0};
extern UBYTE transParentLength;
extern bool isColorPacketSendingNow;

extern bool vcom_enable;
UBYTE cmdRight = 1;
UBYTE cmdError = 2;
UBYTE cmdFormatError = 3;
UBYTE eeError = 4;
/*****************************************************************************
 * Public functions
****************************************************************************/

void PowerUp(){
	/*1) Power is turned on
	2) The DLP Formatter activates (set low) the SSI_ENZ signal
	3) The DLP Formatter communicates to the SSI Driver via the SSI_DRV_DIN signal (SPI Interface) to
	determine SSI Driver ID and Status information
	4) The DLP Formatter communicates to the SSI Driver via the SSI_DRV_DIN signal (SPI Interface) to
	send a Timing Packet (Enabling the Driver Enable register)
	5) SSI Driver activates (set low) the SSI_LITZ signal
	6) Normal Operation begins. See Figure 2-3: STROBE Signal Usage Example and Figure 3-4: Packet
	Usage Example for Normal Operation examples.

	U32 portStatue = 0;

	portStatue = IO_GPIO_unGetPortMaskedState(IO_PORT_1);
	portStatue &= ~(1<<7);//ENZ is p1_07
	IO_GPIO_vSetPortMaskedState(IO_PORT_1,portStatue);
*/

	//sendIDandSTpacket();

	//sendTimingPacket();

}
void StartModeSel(int i){
	uint32_t address;
	uint8_t *ptr;
	uint8_t ret_code=0;

	
	address = ModeCtlAds;
	ptr = (uint8_t *)&modeCtl;
	ret_code = MEMORY_tRead(ptr,address,1,MEMTYPE_EEPROM);

	
	if(modeCtl == 0){
		//manual
		if(vcom_enable == TRUE)
			vcom_write(&cmdError,1);
		}
	else{
		//load ee
		 
			
		if(i>=15)
			i = 15;
		Measure_vLoadData(i);
	//	if(i>=3)
		//	i=3;
		i = i % 4;
		Pwrctrl_vReadColorSeqFromEE(i);	
		
				
		}

}

void USB_getCmd(void){
	uint32_t prompt = 0, rdCnt = 0;
	int i=0;
	
	/* Check if host has connected and opened the VCOM port */
		if ((vcom_connected() != 0) && (prompt == 0)) {
			prompt = 1;
		}
		
		/* If VCOM port is opened echo whatever we receive back to host. */
		if (prompt) {
			
			rdCnt = vcom_bread(&usb_buff[0], 64);
			if (rdCnt) {
				
				if(dataPieceCnt == 0){
					insLength = usb_buff[0]+ 2 * (usb_buff[1] & 0x80);
					insPiece = insLength/64;
					if(insPiece == 0){
						for(i=0;i<insLength;i++)
							g_rxBuff[i] = usb_buff[i];
						}
					else{
						for(i=0;i<64;i++)
							g_rxBuff[i] = usb_buff[i];
						}
					
				}
				else{
					for(i=0;i<64;i++)
							g_rxBuff[64*dataPieceCnt+i] = usb_buff[i];
					}
				if(dataPieceCnt < insPiece)
					dataPieceCnt++;
					
				else{
					dataPieceCnt = 0;
					if(g_rxBuff[insLength-1] == 0xfe){
					//	vcom_write("Ins format right\r\n",18);
						vcom_enable = TRUE;
						Instruction_vGetCmd();
						
						}
					else
						if(vcom_enable == TRUE)
							vcom_write(&cmdFormatError,1);
					}
					
				
			}
		}
}

int main(void)
{/*	
	
	SystemCoreClockUpdate();

	Chip_GPIO_Init(LPC_GPIO);
    
	Chip_GPIO_SetPortDIROutput(LPC_GPIO,0, (1 << PORT0_DEBUG_PIN));
  Chip_GPIO_SetPinToggle(LPC_GPIO,0, PORT0_DEBUG_PIN);
  Chip_GPIO_SetPinToggle(LPC_GPIO,0, PORT0_DEBUG_PIN);
  Chip_GPIO_SetPinToggle(LPC_GPIO,0, PORT0_DEBUG_PIN);


  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EEPROM);
	Chip_SYSCTL_PeriphReset(RESET_EEPROM);
*/	
 	if(IO_bInit() == FALSE){
;
	}
		
	

/*	
	IO_GPIO_vSetPortMaskedState(0, (1 << PORT0_DEBUG_PIN));
	IO_GPIO_vToggle(PORT0_DEBUG_PIN);
	IO_GPIO_vToggle(PORT0_DEBUG_PIN);
	IO_GPIO_vToggle(PORT0_DEBUG_PIN);
*/

	
	usb_init();
  
	TIMER_vInit();
	
	SysTick_Config(CLOCK_nGetSysTick() / TICKRATE_HZ);

  PWRCTRL_vInit();
/**/
	
	IO_SWM_vMovablePinAssignZ(0,SWM_SSP0_SCK_IO);	
	IO_SWM_vMovablePinAssignZ(5,SWM_SSP0_MOSI_IO);
	IO_SWM_vMovablePinAssignZ(16,SWM_SSP0_MISO_IO);
	IO_SWM_vMovablePinAssignZ(22 ,SWM_SSP0_SSELSN_0_IO);	//22
	
	SPI_vInit();
	
	SPI_vStart(SPI0);

	if(Measure_ubLoadAutoPlay() != 0)
		StartModeSel(0);
	
  	while (1) 
	{
		USB_getCmd();		
		
		Package_vSendMessageInASubframe();

		if(transParentFlag != 0 && isColorPacketSendingNow==FALSE){
			Package_vTransParent(transParentLength,transParentFlag);
	//		Package_vTransParent(64);
			transParentLength = 0;
			transParentFlag = 0;
//			transParentReady = 0;
		}
	}

}
