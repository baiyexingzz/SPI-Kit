/******************************************************************************/
/*                                                                            */
/*    $Workfile::   IO.c                                                $     */
/*                                                                            */
/*    $Revision:: 320                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-14 15:30:51 +0800 (周五, 14 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level digital I/O configuration and pin assignment    */
/*                  macros and functions for LPC1517                          */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "io.h"

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/**
 * @brief IO pin setup line
 */
#define IO_CFG_SETUP_L(p) {p, p##_USAGE, p##_INITSTAT, p##_CFG}

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/**
 * @brief IO pin setup table for LPC1517
 */
const IO_PIN_SETUP_T IOPinSetup[] = {
    #if (TARGET_ICPACKAGE >= LQFP48)   
    IO_CFG_SETUP_L(IO_P0_00),
    IO_CFG_SETUP_L(IO_P0_01),
    IO_CFG_SETUP_L(IO_P0_02),
    IO_CFG_SETUP_L(IO_P0_03),
    IO_CFG_SETUP_L(IO_P0_04),
    IO_CFG_SETUP_L(IO_P0_05),
    IO_CFG_SETUP_L(IO_P0_06),
    IO_CFG_SETUP_L(IO_P0_07),
    IO_CFG_SETUP_L(IO_P0_08),
    IO_CFG_SETUP_L(IO_P0_09),
    IO_CFG_SETUP_L(IO_P0_10),
    IO_CFG_SETUP_L(IO_P0_11),
    IO_CFG_SETUP_L(IO_P0_12),
    IO_CFG_SETUP_L(IO_P0_13),
    IO_CFG_SETUP_L(IO_P0_14),
    IO_CFG_SETUP_L(IO_P0_15),
    IO_CFG_SETUP_L(IO_P0_16),
    IO_CFG_SETUP_L(IO_P0_17),
    IO_CFG_SETUP_L(IO_P0_18),
    IO_CFG_SETUP_L(IO_P0_19), 
    IO_CFG_SETUP_L(IO_P0_20), 
    //IO_CFG_SETUP_L(IO_P0_21), /* Not export SWM pins */
    IO_CFG_SETUP_L(IO_P0_22),
    IO_CFG_SETUP_L(IO_P0_23),
    IO_CFG_SETUP_L(IO_P0_24),
    IO_CFG_SETUP_L(IO_P0_25),
    IO_CFG_SETUP_L(IO_P0_26),
    IO_CFG_SETUP_L(IO_P0_27),
    IO_CFG_SETUP_L(IO_P0_28),
    IO_CFG_SETUP_L(IO_P0_29),
    #endif
    #if (TARGET_ICPACKAGE >= LQFP64)
    IO_CFG_SETUP_L(IO_P0_30),
    IO_CFG_SETUP_L(IO_P0_31),
    IO_CFG_SETUP_L(IO_P1_00),
    IO_CFG_SETUP_L(IO_P1_01),
    IO_CFG_SETUP_L(IO_P1_02),
    IO_CFG_SETUP_L(IO_P1_03),
    IO_CFG_SETUP_L(IO_P1_04),
    IO_CFG_SETUP_L(IO_P1_05),
    IO_CFG_SETUP_L(IO_P1_06),
    IO_CFG_SETUP_L(IO_P1_07),
    IO_CFG_SETUP_L(IO_P1_08),
    IO_CFG_SETUP_L(IO_P1_09),
    IO_CFG_SETUP_L(IO_P1_10),
    IO_CFG_SETUP_L(IO_P1_11),

	IO_CFG_SETUP_L(IO_P2_12),
    IO_CFG_SETUP_L(IO_P2_13),
    #endif
	#if (TARGET_ICPACKAGE >= LQFP100)
	/* To be extended for LQFP100 */
    #endif
};

/**
 * @brief IO pin and its assigned fixed function
 */
const IO_PIN_FIXFUNC_T IOPinFixedFunc[] = {
    #if (TARGET_ICPACKAGE >= LQFP48)
    {IO_P0_00,    SWM_FIXED_ADC0_10,   SWM_FIXED_SCT0_OUT3},
    {IO_P0_01,    SWM_FIXED_ADC0_7,    SWM_FIXED_SCT0_OUT4},
    {IO_P0_02,    SWM_FIXED_ADC0_6,    SWM_FIXED_SCT1_OUT3},	
	{IO_P0_03,    SWM_FIXED_ADC0_5,    SWM_FIXED_SCT1_OUT4},	
	{IO_P0_04,    SWM_FIXED_ADC0_4,    SWM_FIXED_UNASSIGN},
    {IO_P0_05,    SWM_FIXED_ADC0_3,    SWM_FIXED_UNASSIGN},
	{IO_P0_06,    SWM_FIXED_ADC0_2,    SWM_FIXED_SCT2_OUT3},
	{IO_P0_07,    SWM_FIXED_ADC0_1,    SWM_FIXED_UNASSIGN},	
	{IO_P0_08,    SWM_FIXED_ADC0_0,    SWM_FIXED_UNASSIGN},	
	{IO_P0_09,    SWM_FIXED_ADC1_1,    SWM_FIXED_UNASSIGN},	
	{IO_P0_10,    SWM_FIXED_ADC1_2,    SWM_FIXED_UNASSIGN},		
	{IO_P0_11,    SWM_FIXED_ADC1_3,    SWM_FIXED_UNASSIGN},	
	{IO_P0_12,    SWM_FIXED_DAC_OUT,   SWM_FIXED_UNASSIGN},	
	{IO_P0_13,    SWM_FIXED_ADC1_6,    SWM_FIXED_UNASSIGN},
	{IO_P0_14,    SWM_FIXED_ADC1_7,    SWM_FIXED_SCT1_OUT5},
	{IO_P0_15,    SWM_FIXED_ADC1_8,    SWM_FIXED_UNASSIGN},	
	{IO_P0_16,    SWM_FIXED_ADC1_9,    SWM_FIXED_UNASSIGN},	
	{IO_P0_17,    SWM_FIXED_UNASSIGN,  SWM_FIXED_UNASSIGN},	
	{IO_P0_18,    SWM_FIXED_SCT0_OUT5, SWM_FIXED_UNASSIGN},
	{IO_P0_19,    SWM_FIXED_SWCLK_TCK, SWM_FIXED_UNASSIGN},	  /* SWM_FIXED_SWCLK_TCK is enabled default */
	{IO_P0_20,    SWM_FIXED_SCT1_OUT6, SWM_FIXED_SWDIO},	  /* SWM_FIXED_SWDIO is enabled default */
	//{IO_P0_21,    SWM_FIXED_RESETN,    SWM_FIXED_UNASSIGN},   /* SWM_FIXED_RESETN is enabled default */
	{IO_P0_22,    SWM_FIXED_I2C0_SCL,  SWM_FIXED_UNASSIGN},	
	{IO_P0_23,    SWM_FIXED_I2C0_SDA,  SWM_FIXED_UNASSIGN},
	{IO_P0_24,    SWM_FIXED_SCT0_OUT6, SWM_FIXED_UNASSIGN},		
	{IO_P0_25,    SWM_FIXED_ACMP0_I4,  SWM_FIXED_UNASSIGN},	
	{IO_P0_26,    SWM_FIXED_ACMP0_I3,  SWM_FIXED_SCT3_OUT3},	
	{IO_P0_27,    SWM_FIXED_ACMP_I1,   SWM_FIXED_UNASSIGN},	
	{IO_P0_28,    SWM_FIXED_ACMP1_I3,  SWM_FIXED_UNASSIGN},	
	{IO_P0_29,    SWM_FIXED_ACMP2_I3,  SWM_FIXED_SCT2_OUT4},
	#endif
	#if (TARGET_ICPACKAGE >= LQFP64)
	{IO_P0_30,    SWM_FIXED_ADC0_11,   SWM_FIXED_UNASSIGN},
	{IO_P0_31,    SWM_FIXED_ADC0_9,    SWM_FIXED_UNASSIGN},
	{IO_P1_00,    SWM_FIXED_ADC0_8,    SWM_FIXED_UNASSIGN},	
	{IO_P1_01,    SWM_FIXED_ADC1_0,    SWM_FIXED_UNASSIGN},	
	{IO_P1_02,    SWM_FIXED_ADC1_4,    SWM_FIXED_UNASSIGN},	
	{IO_P1_03,    SWM_FIXED_ADC1_5,    SWM_FIXED_UNASSIGN},	
	{IO_P1_04,    SWM_FIXED_ADC1_10,   SWM_FIXED_UNASSIGN},	
	{IO_P1_05,    SWM_FIXED_ADC1_11,   SWM_FIXED_UNASSIGN},	
    {IO_P1_06,    SWM_FIXED_ACMP_I2,   SWM_FIXED_UNASSIGN},	
    {IO_P1_07,    SWM_FIXED_ACMP3_I4,  SWM_FIXED_UNASSIGN},	
    {IO_P1_08,    SWM_FIXED_ACMP3_I3,  SWM_FIXED_SCT3_OUT4},
    {IO_P1_09,    SWM_FIXED_ACMP2_I4,  SWM_FIXED_ACMP2_I4},
	{IO_P1_10,    SWM_FIXED_ACMP1_I4,  SWM_FIXED_UNASSIGN},	
	{IO_P1_11,    SWM_FIXED_UNASSIGN,  SWM_FIXED_UNASSIGN},	

    {IO_P2_12,    SWM_FIXED_UNASSIGN,  SWM_FIXED_UNASSIGN},
    {IO_P2_13,    SWM_FIXED_UNASSIGN,  SWM_FIXED_UNASSIGN},		
	#endif
	#if (TARGET_ICPACKAGE >= LQFP100)
	/* To be extended for LQFP100 */
    #endif
};


/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initialize GPIO block
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void IO_GPIO_vInit()
{
	/* Enable clock for IO Port0/1/2 */
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_GPIO0);
	#if (TARGET_ICPACKAGE >= LQFP64)
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_GPIO1);
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_GPIO2);
	#endif

	/* Enable and reset Pin mux clock */
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_MUX);	
	SYSCTL_vPeriphReset(RESET_MUX);
}

/**
 * @brief	Initialize DIO CON block
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void IO_DIOCON_vInit()
{
	/* Enable and reset DIO CON clock */
    CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_IOCON);
	SYSCTL_vPeriphReset(RESET_IOCON);
}

/**
 * @brief	Initialise the SWM module
 * @param	Nothing 
 * @return	Nothing
 */
STATIC INLINE void IO_SWM_vInit(void)
{
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SWM);
}

/**
 * @brief	Sets entire Digital I/O configuration
 * @param	tePin		: GPIO pin to get state
 * @param	unConfig	: OR'ed values or type IOCON_*
 * @return	Nothing
 */
STATIC INLINE void IO_DIOCON_vConfig(IO_PIN_NO_T tePin, U32 unConfig)
{
	WRITE_REG(LPC_DIOCON->DPIO[IO_PortNo(tePin)][IO_PinNoInPort(tePin)], unConfig);
}

/**
 * @brief	Assign movable pin function to physical pin in Switch Matrix
 * @param	tePin	: GPIO pin to get state
 * @param	teMF	: Movable pin function
 * @return	Nothing
 */
STATIC INLINE void IO_SWM_vMovablePinAssign(IO_PIN_NO_T tePin, IO_SWM_PIN_MOVABLE_T teMF)
{
	SET_REG_FIELD_VALUE(LPC_SWM->PINASSIGN[IO_PINASSIGN_IDX(teMF)],
		                (0xFF << IO_PINSHIFT(teMF)), 
		                IO_PINSHIFT(teMF), 
		                tePin);
}

/**
 * @brief	Enable fixed function in the Switch Matrix
 * @param	teFF	: fixed function to be enabled
 * @return	Nothing
 */
STATIC INLINE void IO_SWM_vEnableFixedPin(IO_SWM_PIN_FIXED_T teFF)
{
    /* Set low to enable fixed pin */
	SET_REG_FIELD(LPC_SWM->PINENABLE[SWM_FIXED_REGIDX(teFF)], 
	              BIT(SWM_FIXED_SHIFT(teFF)),
                  0);
}

/**
 * @brief	Disable fixed function in the Switch Matrix
 * @param	teFF	: fixed function to be disabled
 * @return	Nothing
 */
STATIC INLINE void IO_SWM_vDisableFixedPin(IO_SWM_PIN_FIXED_T teFF)
{
	/* Set low to enable fixed pin */
	SET_REG_FIELD(LPC_SWM->PINENABLE[SWM_FIXED_REGIDX(teFF)], 
	              BIT(SWM_FIXED_SHIFT(teFF)),
                  BIT(SWM_FIXED_SHIFT(teFF)));
}
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize all IO pins according to table IOConSetup */
bool IO_bInit(void)
{ 
	bool ret = true;
	U16  i = 0;
    U16  setuplen = sizeof(IOPinSetup)/sizeof(IO_PIN_SETUP_T);

    /* Init SWM and GPIO */
	IO_DIOCON_vInit();
    IO_SWM_vInit();
	IO_GPIO_vInit();

	/* Setup GPIO/MovableFunc/FixedFunc according to config table */
	for(i= 0; i< setuplen; i++){
		switch(IOPinSetup[i].pinusage){
			case IO_GPIO:{ 
				/* Init DIOCON register for GPIO */
     			IO_DIOCON_vConfig(IOPinSetup[i].pin, IOPinSetup[i].diocfg);
				
				/* Init pin level(defined as initstate1) for GPIO */
				IO_GPIO_vSet(IOPinSetup[i].pin, (IO_PIN_STATE_T)INIT_GPIO_LEVEL(IOPinSetup[i].initstat));
				
                /* Init pin direction (defined as initstate2) for GPIO */
				IO_GPIO_vSetDir(IOPinSetup[i].pin, (IO_PIN_DIR_T)INIT_GPIO_DIR(IOPinSetup[i].initstat));
			}
			break;
			case IO_MovableFunc:{
				/* Init DIOCON register for movable function pin */
				IO_DIOCON_vConfig(IOPinSetup[i].pin, IOPinSetup[i].diocfg);

				/* Disable assigned fixed func(s) if exists */
				if(SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff1){
					IO_SWM_vDisableFixedPin(IOPinFixedFunc[i].ff1);
				}
				
				if(SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff2){
					IO_SWM_vDisableFixedPin(IOPinFixedFunc[i].ff2);
				}

				/* Switch to movable func*/
                IO_SWM_vMovablePinAssign(IOPinSetup[i].pin, (IO_SWM_PIN_MOVABLE_T)IOPinSetup[i].initstat);	
			}
			break;
			case IO_FixedFunc:{
				/* Init DIOCON register for fixed function pin */
				IO_DIOCON_vConfig(IOPinSetup[i].pin, IOPinSetup[i].diocfg);

				/* Disable assigned fixed funcs if exists, which is not configured */
				/* Enable assigned fixed funcs if exists, which is configured */
                if((SWM_FIXED_UNASSIGN == IOPinFixedFunc[i].ff1)&&
				   (SWM_FIXED_UNASSIGN == IOPinFixedFunc[i].ff2)){
					/* Invalid configured fixed func in cfg header file */
                    ret = false;
					break;
				}				   
				
                if(((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff1) && 
					((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat != IOPinFixedFunc[i].ff1))&&
				   ((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff2) && 
					((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat != IOPinFixedFunc[i].ff2))){
					/* Invalid configured fixed func in cfg header file */
                    ret = false;
					break;
				}

				/* Enable configured fixed func1 otherwise disable it */
				if((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff1)&&
				   ((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat != IOPinFixedFunc[i].ff1)){
					IO_SWM_vDisableFixedPin(IOPinFixedFunc[i].ff1);
				}
				else if((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff1)&&
				   ((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat == IOPinFixedFunc[i].ff1)){
					IO_SWM_vEnableFixedPin((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat);
					continue;
				}				

			    /* Enable configured fixed func2 otherwise disable it */
				if((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff2)&&
				   ((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat != IOPinFixedFunc[i].ff2)){
					IO_SWM_vDisableFixedPin(IOPinFixedFunc[i].ff2);
				}
				else if((SWM_FIXED_UNASSIGN != IOPinFixedFunc[i].ff2)&&
				   ((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat == IOPinFixedFunc[i].ff2)){
					IO_SWM_vEnableFixedPin((IO_SWM_PIN_FIXED_T)IOPinSetup[i].initstat);
					continue;
				}	
			}
			break;
			default:{
				ret = false;
			}
			break;
		}
	}
	
	return ret;
}

void IO_SWM_vMovablePinAssignZ(IO_PIN_NO_T tePin, IO_SWM_PIN_MOVABLE_T teMF)
{
	IO_SWM_vMovablePinAssign(tePin, teMF);
}


void IO_SWM_vEnableFixedPinZ(IO_SWM_PIN_FIXED_T teFF)
{
    IO_SWM_vEnableFixedPin(teFF);
}


/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

