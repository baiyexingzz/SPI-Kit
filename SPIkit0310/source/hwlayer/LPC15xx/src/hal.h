/******************************************************************************/
/*                                                                            */
/*    $Workfile::   hal.h                                                $    */
/*                                                                            */
/*    $Revision:: 4248                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-04-11 10:06:37 +0800 (周四, 11 四月 2013)     $    */
/*                                                                            */
/*      $Author:: mervyn.ji@philips.com                                  $    */
/*                                                                            */
/*        Owner::   Kyle Ye                                                   */
/*                                                                            */
/*  Description::   Hardware Abstraction Layer (HAL)                          */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

#ifndef HAL_H
#define HAL_H

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

#include "platform.h"
#include "stdtypes.h"
#include "system.h"

#include "hal_cfg.h"

#include "dbase.h"
#include "dio.h"
#include "adc.h"
#include "dac.h"
#include "timer.h"
#include "interrupt.h"
#include "i2c.h"
#include "memory.h"

/*lint save */
/*lint -esym(769, *CPU*) */
/*lint -esym(756, *CPU*) */
/*lint -esym(666, HAL_vSetISetValue) */

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

// Maximum and Minimum of Iset [mA] range (output)
#define HAL_ISET_MAX_mA             (DBASE_tGetField(DriverParameters,FullScaleISet))
#define HAL_ISET_MIN_mA             (350)//mA
#define HAL_ISET_HWMAX_mA           (DBASE_tGetField(DriverParameters,InstantCurrentMaxClip))
// Maximum and Minimum of Iset [cnts] range (output)
#define HAL_ISET_MAX_CNTS           (((ULONG)1<<12) - 1)
#define HAL_ISET_MIN_CNTS            HAL_I_SET_SCALING(CALIB_usApply(CALIB_CHANNEL_ISET, HAL_ISET_MIN_mA))
#define HAL_ISET_HWMAX_CNTS          HAL_I_SET_SCALING(CALIB_usApply(CALIB_CHANNEL_ISET, HAL_ISET_HWMAX_mA))
// Maximum and Minimum of Iset range in Software according to different product
#define HAL_ISET_MAX                 HAL_ISET_HWMAX_mA
#define HAL_ISET_MIN                 HAL_ISET_MIN_mA

// Maximum of Ilamp [mA] range (input)
#define HAL_ILAMP_MAX               (DBASE_tGetField(DriverParameters,FullScaleILamp))

// WS/ILC: Sample Time of an Waveshape/ILC Transition [us]:
#define HAL_TRAN_SAMPLE_TIME      (22)
// WS/ILC: System Delay specified in Transition Samples:
#define HAL_SYSTEM_SAMPLE_DELAY   (3)

//Sync rising & falling edge interrupt priority runtime change
#define HAL_SYNC_RISING_PRIORITY  (5) //Low priority
#define HAL_SYNC_FALLING_PRIORITY (1) //High priority

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/* CPU Load Testing Functions                                                         */
/******************************************************************************/


#ifdef CFG_ENABLE_CPULOAD_TEST
    
#define HAL_vSetCPULoadSource(ubCPULoadSource)    (HAL_ubCPULoadSourceID=ubCPULoadSource)
  /*
  ** Description: Set the current selected CPU Load Source
  ** Parameters:  Desired CPU Load source ID to test
  ** Returns:     void
  */                

#define HAL_vSetCPUTestPin(_CPULOAD_SOURCE_ID_)  \
  BOOL  blPinToggled=FALSE; \
  if(((IF((HAL_ubCPULoadSourceID!=0xFF),(BOOL)(HAL_ubCPULoadSourceID==(UBYTE)_CPULOAD_SOURCE_ID_),FALSE)) \
    ||(DIO_blGet(PIN_TEST_PIN)==DIO_HIGH))&&(blPinToggled==FALSE)) \
  { \
    DIO_vToggle(PIN_TEST_PIN); \
    blPinToggled=TRUE; \
  } 
  /*
  ** Description: Returns a Bool to indicate how this CPU Load source should behave base on the current
                         selected CPU load source ID
  ** Parameters:  The Caller's source ID
  ** Returns:     True: Set the CPU Load pin to high
                        False: Set CPU Load test pin to low if already high and do nothing if low
  */     
#define HAL_vRestoreCPUTestPin() \
  if(blPinToggled) \
  { \
    DIO_vToggle(PIN_TEST_PIN); \
    blPinToggled=FALSE;\
  }
#else
#define HAL_vSetCPUTestPin(_CPULOAD_SOURCE_ID_)  
#define HAL_vRestoreCPUTestPin()  
#endif

/******************************************************************************/
/* HAL Scaling Macros                                                         */
/******************************************************************************/
// HAL_V_DOWN_SCALING: Scales the raw 12 bit ADC value to dV (deci Volt)
#define HAL_V_DOWN_SCALING(value_raw) \
           ((USHORT)(((ULONG)((value_raw)*(ULONG)HAL_VDOWN_MAX)+2048UL) >> 12UL))

// HAL_V_NTC_SCALING: Scales the raw 12 bit ADC value to mV (mili Volt)
#define HAL_V_NTC_SCALING(value_raw) \
                        ((USHORT)(((ULONG)((value_raw)*3300UL)+2048UL) >> 12UL))

// Scale a current setpoint to a digital (12 bit value to output to the DAC)
#define HAL_I_SET_SCALING(usISet) (usISet)

                    

/******************************************************************************/
/* HAL Analog Signal Definitions                                              */
/******************************************************************************/

// Definition of the available ADC channels
#define CHANNEL_V_DOWN  ADC_VLAMP
#define CHANNEL_V_DC    ADC_VDC
#define CHANNEL_NTC     ADC_NTC
//#define CHANNEL_12V     ADC_12V_LOG
#define CHANNEL_I_LAMP  ADC_ILAMP


BOOL HAL_blGetIoPin(UBYTE ubPin);
/*
** Description: Returns the value of a specified I/O pin
** Parameters:  ubPin
**  0 ~ 15 (0x00 ~ 0x0F): Port A
** 16 ~ 31 (0x10 ~ 0x1F): Port B
** 32 ~ 47 (0x20 ~ 0x2F): Port C
** 48 ~ 63 (0x30 ~ 0x3F): Port D
** 64 ~ 79 (0x40 ~ 0x4F): Port E
** Returns:     TRUE (=high) or FALSE (=low)
*/


void HAL_vSetIoPin(UBYTE ubPin,BOOL blVal);
/*
** Description: Sets the value of a specified I/O pin
** Parameters:  ubPin
**  0 ~ 15 (0x00 ~ 0x0F): Port A
** 16 ~ 31 (0x10 ~ 0x1F): Port B
** 32 ~ 47 (0x20 ~ 0x2F): Port C
** 48 ~ 63 (0x30 ~ 0x3F): Port D
** 64 ~ 79 (0x40 ~ 0x4F): Port E
**              blVal  - TRUE (=high) or FALSE (=low)
** Returns:     -
*/


#define HAL_usGetVDownRaw()  ADC_tsGet(ADC_VLAMP)
/*
** Description: Returns the last measured VDown value (Raw = 12 bit ADC value)
**   ADC Alignment: Right aligned 
** Parameters:  -
** Returns:     12 bit VDown value
*/


#define HAL_usGetVBus() (USHORT)(((ULONG)(ADC_tsGet(ADC_VDC))*HAL_VBUS_MAX) >> 12U)
/*
** Description: Returns the last measured V Bus value in V
**              the value is scaled with HAL_VBUS_MAX as maximum
** Parameters:  -
** Returns:     16 bit VBus[V]
*/

#define HAL_usGetNTCVoltage() ((USHORT)HAL_V_NTC_SCALING(ADC_tsGet(ADC_NTC)))
/*
** Description: Returns the last measured NTC voltage in mV
**              the value is scaled with 3.3V (3300mV) as maximum
** Parameters:  -
** Returns:     16 bit NTC Voltage
*/
#if defined(CFG_ENABLE_LOGDATA) && defined(CFG_ENABLE_12V_TEST)
// 12V measurement is N.A. on BV STM32 HW
#define HAL_ubGet12VSample()  ((UBYTE)(((ULONG)ADC_tsGet(ADC_12V_LOG)*HAL_12V_MAX)>>12U))
/*
** Description: Returns the last measured 12V supply value in 8 bit
**              the value is scaled
** Parameters:  -
** Returns:     8 bit voltage Value [dV]
*/
#endif

#define HAL_usGetAdc(CHANNEL) ((USHORT)ADC_tsGet(CHANNEL))
/*
** Description: Retrieve a sampled ADC channel 
**              (Shift result 6 bits (take upper 10 bits) because of data alignment)
** Parameters:  -
** Returns:     8 bit voltage Value [dV]
*/
#define HAL_usGetILamp() (USHORT)((((ULONG)ADC_tsGet(ADC_ILAMP)*HAL_ILAMP_MAX)+2048U)>>12U)
//Enable transmission
#define HAL_vEnableUartTx()    DIO_vSetMode(PIN_TXD, DIO_OUT_AF_PUSH_PULL_2MHZ)

//Disable transmission
#define HAL_vDisableUartTx()\
              DIO_vSet(PIN_TXD, DIO_HIGH);\
              DIO_vSetMode(PIN_TXD, DIO_OUT_GP_PUSH_PULL_2MHZ);

//Terminal: uart output string by none-interrupt mode
#define HAL_vSetUartToTerminal() \
  {INTERRUPT_vDisable(INTERRUPT_UART);HAL_vEnableUartTx();}

//Is a watchdog reset occured
#define HAL_bIsWatchdogReset() (SYSTEM_bWatchdogReset())
//Is a software reset occured (caused by fault exception)
#define HAL_bIsSoftwareReset() (SYSTEM_bSoftwareReset())

//Enable/Disable 2nd down convertor (DD 400w only)
//#define HAL_vEnable2ndDownConvertor()  
//#define HAL_vDisble2ndDownConvertor()  

#define HAL_IGNITION_vSetTimerFast(usStartPeriod) HAL_IGNITION_vSetTimer(usStartPeriod)

TARGET_INLINE static void HAL_vDisableExternalEeprom(void)
{
  //enable eeprom chip write protection
  DIO_vSet(PIN_E2P_DISABLE,DIO_HIGH);
  //resume I2C on MCU
  DIO_vSetMode(PIN_SCL,DIO_OUT_AF_OPEN_DRAIN_2MHZ);
  DIO_vSetMode(PIN_SDA,DIO_OUT_AF_OPEN_DRAIN_2MHZ);
}

TARGET_INLINE static void HAL_vEnableExternalEeprom(void)
{
  //disable eeprom chip write protection
  DIO_vSet(PIN_E2P_DISABLE,DIO_LOW);
  //release I2C on MCU
  DIO_vSetMode(PIN_SCL,DIO_IN_FLOATING);
  DIO_vSetMode(PIN_SDA,DIO_IN_FLOATING);
}

/* Dirty solution for ADC injected channels' scan restart */
//Not really stop ADC, but set JL = 0b00 so only one channel is scanned
#define HAL_vStopAdcScan()\
	PERI_BITBAND_ALIAS(&ADC1->CR2, 1) = 0;\
	//ADC_vSetInjSeqLength(ADC_JSQR_Length_1);

//Resume JL to 0b11 so ADC will restart scan all channels from 1st channel
#define HAL_vStartAdcScan()\
	PERI_BITBAND_ALIAS(&ADC1->CR2, 1) = 1;\
	PERI_BITBAND_ALIAS(&ADC1->CR2, 0) = 1;
	//ADC_vSetInjSeqLength(ADC_LENGTH_OF_JSQR);\

/*============================================================================*/
/*          HAL Sync functions                                                */
/*============================================================================*/

#define HAL_SYNC_usGetCaptureValue() TIMER_tGet(TIMER1_CH4CCR)

TARGET_INLINE static void HAL_vConfigSyncInterrupt(BOOL blRising)
{
  if(blRising)
  {
    TIMER_vSetMode(TIMER1_CH4, TIMER_CH_CCxP_RISING);
    INTERRUPT_vSetPriority(INTERRUPT_SYNC, HAL_SYNC_RISING_PRIORITY);
  }
  else
  {
    TIMER_vSetMode(TIMER1_CH4, TIMER_CH_CCxP_FALLING);
    INTERRUPT_vSetPriority(INTERRUPT_SYNC, HAL_SYNC_FALLING_PRIORITY);
  }
}

/*============================================================================*/
/* HAL Timer Definitions                                                      */
/*                                                                            */
/*    TIMER1_CH1      =  WS Preparation                                       */
/*    TIMER1_CH2      =  WS Commutation Timeout                               */
/*    TIMER1_CH4      =  Sync Capture Channel                                 */
/*    TIMER2_CH1      =  WS Generation Compare Channel                        */
/*    TIMER2_CH2      =  WS Timeout                                           */
/*    TIMER3_CH3      =  ZCD1 Pulse detection                                 */
/*    TIMER3_CH4      =  ZCD2 Pulse detection                                 */
/*    TIMER4_CH3      =  Comm Control PWM Channel                             */
/*    TIMER15         =  Simple 1us Timer                                     */
/*    TIMER16         =  Kernel 1ms Timer                                     */
/*                                                                            */
/*============================================================================*/

TARGET_INLINE static void HAL_IGNITION_vSetTimer(USHORT usStartPeriod)
{
  TIMER_vSet(TIMER4_CH3CCR,(usStartPeriod>>1));
  TIMER_vSet(TIMER4_ARR,(usStartPeriod));    /* total ignition freq */
}

TARGET_INLINE static void HAL_IGNITION_vSetTimerAddOne(USHORT usStartPeriod)
{
  TIMER_vSet(TIMER4_CH3CCR,(usStartPeriod>>1));
  TIMER_vSet(TIMER4_ARR,(usStartPeriod));    /* total ignition freq */
}


// blGetHVComp: Returns nonzero on an HV Comp event
#define HAL_blGetHVComp()  INTERRUPT_blGetFlag(IRQ_HV_COMP_FLAG)
// vResetHVComp: Resets the HV Comp event status
#define HAL_vResetHVComp() INTERRUPT_vClearFlag(IRQ_HV_COMP_FLAG) 
// vIgnitionStopComm: Stops the commutation timer so no commutations are done
#define HAL_vIgnitionStopComm() TIMER_vStop(TIMER4)
// vSetHighVoltageOffset: Sets an offset (UBYTE) on the HV Detect level DAC of 12 bit
#define HAL_vSetHighVoltageOffset(HVOFFSET)  \
        DAC_vOutput(DAC_IGN_CAL, ((USHORT)(HVOFFSET)<<6))

/*
** Description: Sets the new Commutation Frequency during Ignition
** Parameters:  ulFreq defines the requested Frequency in Hz
** Returns:     -
*/
void HAL_vSetIgnCommFreq(ULONG ulFreq);                                         

void HAL_vIgnitionWait_us(USHORT usTime);
//Transfer from High Freq (Takeover) to Low Freq (Runup Low),
//when need to suppress the high current spike.
typedef enum
{
  TRANSFER_TO2RUNUP, //Transfer Takeover to Runup Low
  TRANSFER_FACTORY   //Supress spike during ATE calibration/fast power up/burst/stress test
} TE_HF2LF_CASE;

extern void HAL_vTransferHFtoLF(TE_HF2LF_CASE teCase);

// Enable WS Preparation Interrupt
#define HAL_vEnableWSPreparationInterrupt()  INTERRUPT_vEnable(INTERRUPT_WS_PREPARE)

// Disable WS Preparation Interrupt
#define HAL_vDisableWSPreparationInterrupt() INTERRUPT_vDisable(INTERRUPT_WS_PREPARE)

// Enable WS Generator Interrupt
#define HAL_vEnableWSGeneratorInterrupt()    INTERRUPT_vEnable(INTERRUPT_WS_GENERATOR)

// Disable WS Generator Interrupt
#define HAL_vDisableWSGeneratorInterrupt()   INTERRUPT_vDisable(INTERRUPT_WS_GENERATOR)

// Set New WS Match time [us] (WS Generation)
#define HAL_vSetWaveShapeMatch(usMatch) TIMER_vSet(TIMER2_CH1CCR, usMatch)

// Get Current WS Timer value [us]   (WS Generation)
#define HAL_usGetWaveShapeTimeValue() TIMER_tGet(TIMER2)

// Trigger a new Commutation 
// toggle set I/O pin
#define HAL_vTriggerCommutation() DIO_vToggle(PIN_COMM_CONTROL)


// Set new Commutation timeout time [us]
TARGET_INLINE static void HAL_vSetCommutationTimeout(USHORT usTime)
{
  USHORT usCommTimeout = (USHORT)(TIMER_tGet(TIMER1)+usTime);
  TIMER_vSet(TIMER1_CH2CCR, usCommTimeout);
}

TARGET_INLINE static void HAL_vHandleCommutationTimeout(USHORT usTimeout)
{
  HAL_vTriggerCommutation();
  HAL_vSetCommutationTimeout(usTimeout);
}

// Set new WS Preparation Start trigger time [us]
 #define HAL_vSetWSPreparationStart(usTime)  \
         TIMER_vSet(TIMER1_CH1CCR, (USHORT)(TIMER_tGet(TIMER1)+usTime))

/*
** Description: Sets the new Current Setpoint on the DAC output
** Parameters:  usISet defines 12 bit set value to write to the DAC 
** Returns:     -
*/
#define HAL_vSetISetValue(usISet) \
  DAC_vOutput(DAC_I_SET, ((USHORT)(((ULONG)(usISet)*HAL_ISET_MAX_CNTS)/HAL_ISET_MAX_mA)))

void HAL_vRestartWaveShape(USHORT usMatchTime,USHORT usSyncTime,
                             USHORT usTimeOutPeriod);
/*
** Description: Restarts the WS Timer, sets Match and Period
** Parameters:  usMatchTime in us, first Match in the new period
**              usSyncTime in us, start timer value
**              usTimeoutPeriod in us, timer period from timer start
** Returns:     -
*/

/******************************************************************************/
/* HAL Kernel Definitions                                                     */
/******************************************************************************/

// Connect and Init RTC with the Kernel
void HAL_vInitKernelTimer(/*PFIRQVOID*/void (*pfKernelCallback)(void));

/******************************************************************************/
/* HAL NVRAM Definitions                                                      */
/******************************************************************************/
#define HAL_teNvmReadEeprom(Dest, Source, Size)  \
                            MEMORY_teRead(Dest, Source, Size, MEMTYPE_NVRAM)
#define HAL_teNvmWriteEeprom(Dest, Source, Size) \
                            MEMORY_teWrite(Dest, Source, Size, MEMTYPE_NVRAM)
#define HAL_teNvmReadBytes(Dest, Source, Size)  \
                            MEMORY_teRead(Dest, Source, Size, MEMTYPE_NVRAM)
#define HAL_teNvmWriteBytes(Dest, Source, Size) \
                            MEMORY_teWrite(Dest, Source, Size, MEMTYPE_NVRAM)

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

typedef enum 
{
  CPULOAD_KERNEL_FREERUN_TIME = 0,
  CPULOAD_WS_vIRQHandleCommTimeout=1,
  CPULOAD_WS_vIRQHandlePreparation=2,
  CPULOAD_WS_vIRQHandleGenerator=3,
  CPULOAD_WS_vIRQHandleTimeout=4,
  CPULOAD_RTC_vIRQHandle=5,
  CPULOAD_SYNC_vIRQHandle=6,
  CPULOAD_UARTDRV_vTxIrq=7,
  CPULOAD_UARTDRV_vRxIrq=8,
  CPULOAD_LAST_SOURCE
} TE_HAL_CPU_LOAD_SOURCE;







/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

#ifdef CFG_ENABLE_CPULOAD_TEST

extern UBYTE HAL_ubCPULoadSourceID;

#endif

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

void HAL_vInit(void);
/*
** Description: Init the HAL Driver modules
** Parameters:  -
** Returns:     -
*/

void HAL_vSetModeIgnition(void);
/*
** Description: Prepare the driver for Ignition
** Parameters:  -
** Returns:     -
*/

void HAL_vSetModeIgnitionHV(void);
/*
** Description: Prepare the driver for HV Phase of Ignition
**              Set COMM_Control to High Impedance
** Pre:         HAL_vSetModeIgnition has been called some time before
** Parameters:  -
** Returns:     -
*/

void HAL_vSetModeIgnitionTakeover(void);
/*
** Description: Prepare the driver for Takeover Phase of Ignition
** Parameters:  -
** Returns:     -
*/


//void HAL_vSetModeIgnitionMF(void);
#define HAL_vSetModeIgnitionMF()  
/*
** Description: Prepare the driver for MF Phase of Ignition
** Parameters:  -
** Returns:     -
*/

#define HAL_vSetModePwrCtrlRunUpLow()  
/*
** Description: Prepare the driver for the Low Runup Phase
** Parameters:  -
** Returns:     -
*/

void HAL_vSetModeWaveShape(ULONG ulTimerFreq);
/*
** Description: Prepare the driver for the waveshape phase
** Parameters:  -
** Returns:     -
*/

void HAL_vSetModeCalibration(void);
/*
** Description: Halt all driver activities and force it to output a waveform
** Parameters:  -
** Returns:     -
*/


void HAL_vSetModeStandby(void);
/*
** Description: Halt all driver activities and force it to a safe reset
**              Stop the DownConverter
**              Set I_Value to reset values
**              PIN_COMM_Control stable on Low
** Parameters:  -
** Returns:     -
*/

USHORT HAL_vGetFreeRunTimerValue(USHORT usBaseTimeOffset);
/*
** Description: Get the free run timer value for STM8 as current time- usBaseTimeOffset. It is the combination 
**                    of T15 and system time. if set baseoffset to 0, you can get the running timer value Resolution is 1us.
**                    Using this has quite some limitation, please check code first
** Parameters:  - usOffset. The offset to be added to the current timer value in 1us resolution
** Returns:     -  Equals current time value - usOffset
*/

void HAL_vSetDF3DModeEnable(BOOL blEnable, USHORT usSyncPeriod);
/*
** Description: 
** Parameters:   
** Returns:     
*/

//Vdownmax/Idownmax control scheme
#define VDOWNIDOWN_CTRL_DISABLED (0x00)//During Factory Test
#define VDOWNIDOWN_CTRL_ENABLED  (0xFF)//For Normal Case

#define VDOWNIDOWN_CTRL_MASK_HV  (0x01)
#define VDOWNIDOWN_CTRL_MASK_TK1 (0x02)
#define VDOWNIDOWN_CTRL_MASK_TK2 (0x04)
#define VDOWNIDOWN_CTRL_MASK_TK3 (0x08)
#define VDOWNIDOWN_CTRL_MASK_TK4 (0x10)
#define VDOWNIDOWN_CTRL_MASK_MF1 (0x20)
#define VDOWNIDOWN_CTRL_MASK_MF2 (0x40)
#define VDOWNIDOWN_CTRL_MASK_ON  (0x80)
void HAL_vVdownIdownCtrlScheme(UBYTE ubCtrlMask);
/*
** Description: Vdown/Idown control
** Parameters:  -
** Returns:     -
*/

//Flag Functions
TARGET_INLINE static void HAL_vSetFlag(void)
{
  DIO_vSet(PIN_TXD, DIO_HIGH);
  //HAL_vEnableUartTx(); /* Set PIN_TXD as UART TXD for flag usage */
}

TARGET_INLINE static void HAL_vClearFlag(void)
{
  DIO_vSet(PIN_TXD, DIO_LOW);
  //HAL_vDisableUartTx(); /* Set PIN_TXD as normal IO for flag usage */
}

/*lint -restore */
#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

