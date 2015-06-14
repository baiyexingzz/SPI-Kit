/******************************************************************************/
/*                                                                            */
/*    $Workfile::   hal.c                                                $    */
/*                                                                            */
/*    $Revision:: 2964                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2011-09-14 14:21:41 +0800 (周三, 14 九月 2011)     $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
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

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

#include "platform.h"
#include "stdtypes.h"
#include "system.h"

#include "hal.h"
#include "timer.h"
#include "uartdrv.h"

#include "dbase.h"
#include "calib.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

#ifdef CFG_ENABLE_CPULOAD_TEST
//This is the definition where CPU Load pin toggle matrix is defined.
//Each source for CPU load has a dedicated ID and this ID also serve as Bit Index in this array
//STM 8, the ID assign as below:
//
//  CPULOAD_KERNEL_FREERUN_TIME = 0,
//  CPULOAD_WS_vIRQHandleCommTimeout=1,
//  CPULOAD_WS_vIRQHandlePreparation=2,
//  CPULOAD_WS_vIRQHandleGenerator=3,
//  CPULOAD_WS_vIRQHandleTimeout=4,
//  CPULOAD_RTC_vIRQHandle=5,
//  CPULOAD_SYNC_vIRQHandle=6,
//  CPULOAD_UARTDRV_vTxIrq=7,
//  CPULOAD_UARTDRV_vRxIrq=8,
//  CPULOAD_LAST_SOURCE


UBYTE HAL_ubCPULoadSourceID=0xFF; //0xff to indicate invalid

#endif

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
static BOOL HAL_blVdownIdownCtrlActive;
static void (*HAL_pfRTCCallback)(void);

void HAL_vInit(void)
/*
** Description: Init the HAL Driver modules
** Parameters:  -
** Returns:     -
*/
{
  HAL_blVdownIdownCtrlActive = TRUE;
  /* SYSTEM_vInit() is already called in the startup (STM32F10x.s) before main entrance! */

  SYSTEM_vHandleResetCause();

  //Peripheral Initialization
  DIO_vInit();
  INTERRUPT_vInit();
  TIMER_vInit();
  ADC_vInit();
  DAC_vInit();
  UARTDRV_vInit();
  I2C_vInit();

  HAL_vSetModeStandby();
}


/*============================================================================*/
/* HAL SCI/Sync Definitions                                                   */
/*============================================================================*/
void HAL_vRestartWaveShape(USHORT usMatchTime,USHORT usSyncTime,
                                                  USHORT usTimeOutPeriod)
/*
** Description: Restarts the WS Timer, sets Match and Period
** Parameters:  usMatchTime in us, first Match in the new period
**              usSyncTime in us, start timer value
**              usTimeoutPeriod in us, timer period from timer start
** Returns:     -
*/
{
  TIMER_vStop(TIMER2);
  TIMER_vSet(TIMER2_CH1CCR, usMatchTime);
  INTERRUPT_vClearFlag(INTERRUPT_WS_GENERATOR);
  TIMER_vSet(TIMER2, usSyncTime);
  TIMER_vSet(TIMER2_CH2CCR, (USHORT)(usSyncTime+usTimeOutPeriod));
  INTERRUPT_vClearFlag(INTERRUPT_WS_TIMEOUT);
  TIMER_vStart(TIMER2);
}

//This function only used in Takeover
void HAL_vSetIgnCommFreq(ULONG ulFreq)                                         
{                                                                              
  USHORT usVal = (USHORT)((ULONG)(SYSTEM_VPB_CLOCK)/(ulFreq));
  usVal|=0x0001; //Set the last bit, so that it is always a Odd number. This can gurantee a excat 50% PWM
  //update ARR register after CCR interrupt because ARPE is enabled
  INTERRUPT_vClearFlag(IRQ_IGNITION_TIMER);
  while (INTERRUPT_blGetFlag(IRQ_IGNITION_TIMER) == 0) ;
  TIMER_vSet(TIMER4_ARR,usVal);    /* total ignition freq */
  //update CCR register after UPD as CCR PE is disabled
  INTERRUPT_vClearFlag(IRQ_IGNITION_TIMER_UPD);
  while (INTERRUPT_blGetFlag(IRQ_IGNITION_TIMER_UPD) == 0) ;
  TIMER_vSet(TIMER4_CH3CCR,((usVal>>1)+1));                                       
}

void HAL_vIgnitionWait_us(USHORT usTime)
{
  USHORT usTargeTime = (USHORT)(TIMER_tGet(TIMER15) + usTime);
  SSHORT ssWaitPeriod;
  do
  {
    ssWaitPeriod = (SSHORT)(usTargeTime - TIMER_tGet(TIMER15));
  }
  while(ssWaitPeriod > 0);
}

void HAL_vTransferHFtoLF(TE_HF2LF_CASE teCase)
{
  //DIO_vSet(PIN_SW_HF2LF, DIO_LOW);//discharge capacitor to resolve current spike
  //HAL_vSetISetValue(HAL_I_SET_SCALING(IGN_I_SET_LOW));
  if(teCase == TRANSFER_TO2RUNUP)
  {
    //HAL_vIgnitionWait_us(IGN_T_I_SET_BEFORE);
    HAL_vIgnitionStopComm();
  }
  //HAL_vIgnitionWait_us(IGN_T_I_SET_AFTER);
  //DIO_vSet(PIN_SW_HF2LF, DIO_HIGH);//resume HF2LF pin
}

void HAL_vSetModeIgnition(void)
/*
** Description: Prepare the driver for Ignition
** Post:        -
** Parameters:  -
** Returns:     -
*/
{
  if(HAL_blVdownIdownCtrlActive)
  {
    //For normal ignition, init Iset to full scale
    DAC_vOutput(DAC_I_SET, 0x0FFF);
  }
  else
  {
    //For stress test, init Iset to zero
    DAC_vOutput(DAC_I_SET, 0x0000);
  }
  DAC_vEnableChannel(DAC_I_SET);
  //Enable inverter
  DIO_vSet(PIN_SHUTDOWN_INV, DIO_HIGH);
}

void HAL_vSetModeIgnitionHV(void)
/*
** Description: Prepare the driver for HV Phase of Ignition
** Pre:         HAL_vSetModeIgnition has been called some time before
** Parameters:  -
** Returns:     -
*/
{
  /*Disable NVIC vector because sync(EXTI12) & HV_Comp(EXTI10)
    shares the same vector, otherwise HC_comp flag will causes
    MCU enters EXTI15_10 vector which is not expected. */
  /*Remove HV_comp's EXTI interrupt mask so that its pending flag
    then will be possible to be raised.*/
  NVIC_DisableIRQ(EXTI15_10_IRQn);
  INTERRUPT_vEnable(IRQ_HV_COMP_FLAG);
  //Configure ignition timer
  TIMER_vStop(TIMER4);
  TIMER_vSetMode(TIMER4_CH3, TIMER_CH_CCxP_HIGH);//PWM mode 1 & High on REF High. Ref is low on match
  TIMER_vSet(TIMER4_ARR,2);
  TIMER_vSet(TIMER4_CH3CCR,0);//Comm Ctrl pin is low
  TIMER_vReset(TIMER4);
  TIMER_vStart(TIMER4);

  //CH3 is connected to PIN_COMM_CONTROL
  DIO_vSetMode(PIN_COMM_CONTROL, DIO_OUT_AF_PUSH_PULL_2MHZ);
}

void HAL_vSetModeIgnitionTakeover(void)
/*
** Description: Prepare the driver for Takeover Phase of Ignition
** Parameters:  -
** Returns:     -
*/
{
  DIO_vSet(PIN_HV_PROT_ON_OFF, DIO_LOW);
}


//void HAL_vSetModeIgnitionMF(void)
/*
** Description: Prepare the driver for MF Phase of Ignition
** Parameters:  -
** Returns:     -
*/
//{
  // Nothing special to do
//}

//void HAL_vSetModePwrCtrlRunUpLow(void)
/*
** Description: Prepare the driver for the Low Runup Phase
** Parameters:  -
** Returns:     -
*/
//{
  // Nothing special to do
//}

/*lint -esym(715, ulTimerFreq) */
void HAL_vSetModeWaveShape(ULONG ulTimerFreq)
/*
** Description: Prepare the driver for the waveshape phase
** Parameters:  -
** Returns:     -
*/
{
  //Comm Ctrl pin set to DIO in waveshape mode
  DIO_vSetMode(PIN_COMM_CONTROL, DIO_OUT_GP_PUSH_PULL_2MHZ);
  // Wave Shape mode Timer Init
  TIMER_vStop(TIMER1);
  TIMER_vStop(TIMER2);

  //Generation timer setup
  TIMER_vSet(TIMER2_CH1CCR,0);        // WS Generation
  TIMER_vSet(TIMER2_CH2CCR,0);        // WS Timeout
  TIMER_vSet(TIMER2_CH3CCR,0);        // WS Timeout

  // --- Timer Settings for WS preperation & Commutation ---

  TIMER_vSet(TIMER1_CH1CCR,0);        // WS Preparation
  TIMER_vSet(TIMER1_CH2CCR,0);        // Comm Time Out

  TIMER_vReset(TIMER1);
  TIMER_vReset(TIMER2);

  //Clear potential last interrupt flags before enable
  INTERRUPT_vClearFlag(INTERRUPT_WS_PREPARE);
  INTERRUPT_vClearFlag(INTERRUPT_COMM_TIMEOUT);
  INTERRUPT_vClearFlag(INTERRUPT_WS_GENERATOR);
  INTERRUPT_vClearFlag(INTERRUPT_WS_TIMEOUT);
  INTERRUPT_vClearFlag(INTERRUPT_WS_3D_DF);
  

  //WS interrupt enable
  TIMER_vSetMode(TIMER2_CH1, TIMER_CH_CCxIE_ENABLE); // WS Generation
  TIMER_vSetMode(TIMER2_CH2, TIMER_CH_CCxIE_ENABLE); // WS Timeout
  TIMER_vSetMode(TIMER2_CH3, TIMER_CH_CCxIE_DISABLE); // WS Timeout
  TIMER_vSetMode(TIMER1_CH1, TIMER_CH_CCxIE_ENABLE); // WS Preparation
  TIMER_vSetMode(TIMER1_CH2, TIMER_CH_CCxIE_ENABLE); // Comm Time Out

  //Timer 1 begin to run
  TIMER_vStart(TIMER1);

  //Resume normal EXTI setting: sync vector available & needn't HV_comp flag.
  HAL_vResetHVComp();
  INTERRUPT_vDisable(IRQ_HV_COMP_FLAG);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_vSetModeCalibration(void)
{
  HAL_vSetModeStandby();
  HAL_vVdownIdownCtrlScheme(VDOWNIDOWN_CTRL_DISABLED);
  HAL_vSetModeIgnition();
  HAL_vVdownIdownCtrlScheme(VDOWNIDOWN_CTRL_ENABLED);
  HAL_vSetModeIgnitionHV();
  HAL_vSetModeIgnitionTakeover();
  HAL_vTransferHFtoLF(TRANSFER_FACTORY);
  HAL_vSetModePwrCtrlRunUpLow();
  HAL_vVdownIdownCtrlScheme(VDOWNIDOWN_CTRL_MASK_ON);
  DIO_vSet(PIN_INTERLEAVE, DIO_HIGH);
}

void HAL_vSetModeStandby(void)
/*
** Description: Halt all driver activities and force it to a safe reset
**              Stop the DownConverter
**              Set I_set to reset values
**              Set CoffMax IO to low
**              PIN_COMM_Control stable on Low
** Parameters:  -
** Returns:     -
*/
{
  // Shutdown Inverter Off
  DIO_vSet(PIN_SHUTDOWN_INV, DIO_LOW);
  // Iset = 0
  DAC_vDisableChannel(DAC_I_SET);
  DIO_vSet(PIN_INTERLEAVE, DIO_LOW);
  // HV Protection re-init
  DIO_vSet(PIN_HV_PROT_ON_OFF, DIO_LOW);

  if(HAL_blVdownIdownCtrlActive)
  {
    DIO_vSet(PIN_SW_V_DOWN_MAX, DIO_LOW);  // SET OCV = 175V
    DIO_vSet(PIN_SW_I_DOWN_MAX, DIO_HIGH); // SET MAX CURRENT 22.34A
  }

  TIMER_vSetMode(TIMER1_CH2, TIMER_CH_CCxIE_DISABLE);//comm timeout
  TIMER_vSetMode(TIMER2_CH2, TIMER_CH_CCxIE_DISABLE);//WS timeout
  TIMER_vSetMode(TIMER2_CH3, TIMER_CH_CCxIE_DISABLE);//WS timeout
  TIMER_vStop(TIMER1);  //WS prepare
  TIMER_vReset(TIMER1);
  TIMER_vStop(TIMER2);  // WS Generation
  TIMER_vReset(TIMER2);
  TIMER_vStop(TIMER4);  // Comm Ctrl
  TIMER_vReset(TIMER4);

  // Define Comm UC
  DIO_vSet(PIN_COMM_CONTROL, DIO_LOW);
  DIO_vSetMode(PIN_COMM_CONTROL, DIO_OUT_GP_PUSH_PULL_2MHZ);

  //Resume normal EXTI setting: sync vector available & needn't HV_comp flag.
  HAL_vResetHVComp();
  INTERRUPT_vDisable(IRQ_HV_COMP_FLAG);
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void HAL_vVdownIdownCtrlScheme(UBYTE ubCtrlMask)
{
  if(ubCtrlMask == VDOWNIDOWN_CTRL_DISABLED)
  {
    HAL_blVdownIdownCtrlActive = FALSE;
  }
  else if(ubCtrlMask == VDOWNIDOWN_CTRL_ENABLED)
  {
    HAL_blVdownIdownCtrlActive = TRUE;
  }
  else if(HAL_blVdownIdownCtrlActive)
  {
    //VdownMax Control
    if(DBASE_tGetField(DriverParameters, VdownCtrlScheme) & ubCtrlMask)
    {
      DIO_vSet(PIN_SW_V_DOWN_MAX, DIO_HIGH);
    }
    else
    {
      DIO_vSet(PIN_SW_V_DOWN_MAX, DIO_LOW);
    }

    //IdownMax Control
    if(DBASE_tGetField(DriverParameters, IdownCtrlScheme) & ubCtrlMask)
    {
      DIO_vSet(PIN_SW_I_DOWN_MAX, DIO_HIGH);
    }
    else
    {
      DIO_vSet(PIN_SW_I_DOWN_MAX, DIO_LOW);
    }
  }
}

USHORT HAL_vGetFreeRunTimerValue(USHORT usBaseTimeOffset)
{
  return (USHORT)(TIMER_tGet(TIMER15) - usBaseTimeOffset);
}

void HAL_vSetDF3DModeEnable(BOOL blEnable, USHORT usSyncPeriod)
{
  if(blEnable)
  {
    TIMER_vSet(TIMER2_CH3CCR, (USHORT)(TIMER_tGet(TIMER2)+(usSyncPeriod >> 1)));
    INTERRUPT_vClearFlag(INTERRUPT_WS_3D_DF);
    TIMER_vSetMode(TIMER2_CH3, TIMER_CH_CCxIE_ENABLE); 
  }
  else
  {
    TIMER_vSetMode(TIMER2_CH3, TIMER_CH_CCxIE_DISABLE); 
  }
}



BOOL HAL_blGetIoPin(UBYTE ubPin)
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
{
  ULONG ulPort = (ULONG)ubPin>>4U;

  if(ulPort <= DIO_PORT_E) // check if pin is valid
  {
    GPIO_TypeDef* tsGpioBase = (GPIO_TypeDef*)(GPIOA_BASE + (0x0400UL*ulPort));
    ULONG ulBitNr = (ULONG)ubPin & 0x0F;
    return (BOOL)(PERI_BITBAND_ALIAS(&tsGpioBase->IDR, ulBitNr)?DIO_HIGH:DIO_LOW);
  }

  return FALSE;
}

void HAL_vSetIoPin(UBYTE ubPin,BOOL blVal)
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
{
  ULONG ulPort = (ULONG)ubPin>>4U;

  if(ulPort <= DIO_PORT_E) // check if pin is valid
  {
    ULONG ulBitSet = (1UL<<(ubPin&0x0F));
    GPIO_TypeDef* tsGpioBase = (GPIO_TypeDef*)(GPIOA_BASE + (0x0400UL*ulPort));
    if(blVal)
    {
      tsGpioBase->BSRR = ulBitSet;
    }
    else
    {
      tsGpioBase->BRR = ulBitSet;
    }
  }
}

void HAL_vInitKernelTimer(/*PFIRQVOID*/void (*pfKernelCallback)(void))
{
  HAL_pfRTCCallback = pfKernelCallback;
  INTERRUPT_vEnable(INTERRUPT_RTC);
}

void HAL_vRTCIRQHandle(void)
{
  HAL_vSetCPUTestPin(CPULOAD_RTC_vIRQHandle); 
  HAL_pfRTCCallback();
  HAL_vRestoreCPUTestPin();
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

