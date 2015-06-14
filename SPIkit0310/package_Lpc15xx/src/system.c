/******************************************************************************/
/*                                                                            */
/*    $Workfile::   System.c                                             $    */
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
/*  Description::   Export APIs for System Configuration                      */
/*                  including  SysCon, Clock and Power up/down                */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "system.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
STATIC SYSTEM_RESET_CAUSE_T teResetCause = SYSTEM_NO_RESET;

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Default main clock source */
#define CLOCK_MAIN_CLKSRC        SYSCTL_MAINCLKSRC_SYSPLLOUT   

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/* Reserved for isp bootloader */
STATIC INLINE bool SYSTEM_bIsEnterISP(void)
{
	return false;
	/* To be extended */
}

/* Reserved for isp bootloader */
STATIC INLINE void SYSTEM_vInvokeIsp(void)
{
	/* To be extended */
}

/**
 * @brief	Set main A system clock source
 * @param	teSrc	: Clock source for main A
 * @return	Nothing
 * @note	This function only neesd to be setup if main clock A will be
 * selected in the Chip_Clock_GetMain_B_ClockRate() function.
 */
STATIC INLINE void CLOCK_vSetMainAClockSource(SYSCTL_MAIN_A_CLKSRC_T teSrc)
{
	WRITE_REG(LPC_SYSCTL->MAINCLKSELA, (U32)teSrc);
}

/**
 * @brief	Set main B system clock source
 * @param	teSrc	: Clock source for main B
 * @return	Nothing
 */
STATIC INLINE void CLOCK_vSetMainBClockSource(SYSCTL_MAIN_B_CLKSRC_T teSrc)
{
	WRITE_REG(LPC_SYSCTL->MAINCLKSELB, (U32)teSrc);
}

/**
 * @brief	Set main system clock source
 * @param	teSrc	: Clock source for main 
 * @return	Nothing
 */
STATIC INLINE void CLOCK_vSetMainClockSource(SYSCTL_MAINCLKSRC_T teSrc)
{
	if (teSrc > 4) {
		/* Main B source only, not using main A */
		CLOCK_vSetMainBClockSource((SYSCTL_MAIN_B_CLKSRC_T) (teSrc - 4));
	}
	else {
		/* Select main A clock source and set main B source to use main A */
		CLOCK_vSetMainAClockSource((SYSCTL_MAIN_A_CLKSRC_T)teSrc);
		CLOCK_vSetMainBClockSource(SYSCTL_MAIN_B_CLKSRC_MAINCLKSELA);
	}
}

/**
 * @brief	Set System PLL clock source
 * @param	teSrc	: Clock source for system PLL
 * @return	Nothing
 */
STATIC INLINE void CLOCK_vSetSystemPLLSource(SYSCTL_PLLCLKSRC_T teSrc)
{
	WRITE_REG(LPC_SYSCTL->SYSPLLCLKSEL, (U32)teSrc);
}

/**
 * @brief	Set System PLL divider values
 * @param	ubMsel    : PLL feedback divider value. M = msel + 1.
 * @param	ubPsel    : PLL post divider value. P =  (1<<psel).
 * @return	Nothing
 * @note	See the user manual for how to setup the PLL.
 */
STATIC INLINE void CLOCK_vSetupSystemPLL(U8 ubMsel, U8 ubPsel)
{
	WRITE_REG(LPC_SYSCTL->SYSPLLCTRL, 
		      (U32)(ubMsel & SYSPLL_MSEL_MASK) | (U32)((ubPsel & SYSPLL_PSEL_MASK) << SYSPLL_PSEL_SHIFT));
}

/**
 * @brief	Read System PLL lock status
 * @return	true of the PLL is locked. false if not locked
 */
STATIC INLINE bool CLOCK_bIsSystemPLLLocked(void)
{
	return (bool) ((READ_REG(LPC_SYSCTL->SYSPLLSTAT)& SYSPLL_STATE_MASK) != 0);
}

/**
 * @brief	Set system clock as cfg value SYS_CLOCK_FREQ
 * @param	Nothing
 * @return	Nothing
 * @note	The system clock rate is the main system clock divided by this div value.
 */
STATIC INLINE void CLOCK_vCfgSysClk(void)
{
    /* Powerup main IRC (likely already powered up) */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_IRC_PD);
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_IRCOUT_PD);
	
	//SYSCTL_vPowerUp(SYSCTL_POWERDOWN_ADC0_PD);	
	//SYSCTL_vPowerDown(SYSCTL_POWERDOWN_ADC0_PD);

	/* Set system PLL input to IRC */
	CLOCK_vSetSystemPLLSource(SYSCTL_PLLCLKSRC_IRC);

	/* Power down PLL to change the PLL divider ratio */
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Setup PLL for main oscillator rate (FCLKIN = 12MHz) * 6 = 72MHz
	   MSEL = 5 (this is pre-decremented), PSEL = 1 (for P = 2)
	   FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 6 = 72MHz
	   FCCO = FCLKOUT * 2 * P = 72MHz * 2 * 2 = 288MHz (within FCCO range) */
	/* Fclkout = (MSel+1)*Fclkin = FCCO/(2*P) */
	CLOCK_vSetupSystemPLL(CFG_SYSTEM_PLL_MSEL, CFG_SYSTEM_PLL_PSEL);

	/* Powerup system PLL */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Wait for PLL to lock */
	while (!CLOCK_bIsSystemPLLLocked()) {;}

	/* Select Main clock resouce*/
	CLOCK_vSetMainClockSource(CLOCK_MAIN_CLKSRC);

	/* Set system clock divider to 1 */
	WRITE_REG(LPC_SYSCTL->SYSAHBCLKDIV , CFG_SYS_CLOCK_DIV);
}

/**
 * @brief	Assert reset for a peripheral
 * @param	tePeriph	: Peripheral to assert reset for
 * @return	Nothing
 * @note	The peripheral will stay in reset until reset is de-asserted. Call
 * Chip_SYSCTL_DeassertPeriphReset() to de-assert the reset.
 */
STATIC INLINE void SYSCTL_vAssertPeriphReset(SYSCTL_PERIPH_RESET_T tePeriph)
{
	SET_REG_FIELD(LPC_SYSCTL->PRESETCTRL[PRESETCTRL_REG_INDEX(tePeriph)],
		          PRESETCTRL_MASK(tePeriph),
		          PRESETCTRL_MASK(tePeriph)); 
}

/**
 * @brief	De-assert reset for a peripheral
 * @param	tePeriph	: Peripheral to de-assert reset for
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_vDeassertPeriphReset(SYSCTL_PERIPH_RESET_T tePeriph)
{
	SET_REG_FIELD(LPC_SYSCTL->PRESETCTRL[PRESETCTRL_REG_INDEX(tePeriph)],
		          PRESETCTRL_MASK(tePeriph),
		          0); 
}

/**
 * @brief	Get system reset status
 * @return	An Or'ed value of SYSCTL_RST_*
 * @note	This function returns the detected reset source(s). Mask with an
 * SYSCTL_RST_* value to determine if a reset has occurred.
 */
STATIC INLINE U32 SYSCTL_nGetRSTStatus(void)
{
	return READ_REG(LPC_SYSCTL->SYSRSTSTAT);
}

/**
 * @brief	Clear system reset status
 * @param	unRst	: An Or'ed value of SYSCTL_RST_* statuses to clear
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_vClearRSTStatus(U32 unRst)
{
	/* Write 1 to clear */
	WRITE_REG(LPC_SYSCTL->SYSRSTSTAT, unRst);
}


/**
 * @brief	Sets Wakeup pad disable bit
 * @return	Nothing
 * @note	Use this function to disable the wakeup pin (P0.17)
 * in which case RTC wakeup is the only option to wakeup from
 * deep power down mode.
 */
STATIC INLINE void PMU_SetWakePadDisable(void)
{
	SET_REG_FIELD(LPC_PMU->GPREG[4], PMU_GPREG4_WAKEPAD_DISABLE, PMU_GPREG4_WAKEPAD_DISABLE);
}

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Configure/initialize the System configuration (SYSCON) of the */

void Chip_SetupXtalClocking(void)
{
	volatile int i;

	/* Powerup main oscillator */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_SYSOSC_PD);

	/* Wait 200us for OSC to be stablized, no status
	   indication, dummy wait. */
	for (i = 0; i < 0x200; i++) {}

	/* Set system PLL input to main oscillator */
	CLOCK_vSetSystemPLLSource(SYSCTL_PLLCLKSRC_MAINOSC);

	/* Power down PLL to change the PLL divider ratio */
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Setup PLL for main oscillator rate (FCLKIN = 12MHz) * 6 = 72MHz
	   MSEL = 5 (this is pre-decremented), PSEL = 1 (for P = 2)
	   FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 6 = 72MHz
	   FCCO = FCLKOUT * 2 * P = 72MHz * 2 * 2 = 288MHz (within FCCO range) */
	CLOCK_vSetupSystemPLL(5, 2);

	/* Powerup system PLL */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_SYSPLL_PD);

	/* Wait for PLL to lock */
	while (!CLOCK_bIsSystemPLLLocked()) {}

	/* Set system clock divider to 1 */
	/* Set system clock divider to 1 */
	WRITE_REG(LPC_SYSCTL->SYSAHBCLKDIV , CFG_SYS_CLOCK_DIV);

	
	/* Set main clock source to the system PLL. This will drive 72MHz
	   for the main clock */
	CLOCK_vSetMainClockSource(SYSCTL_MAINCLKSRC_SYSPLLOUT);
}






void SYSTEM_vInit(void)
{
    /* Check to enter ISP mode  - to be extended */
	if(SYSTEM_bIsEnterISP())
	{
		SYSTEM_vInvokeIsp();
	}
	
    /* Config main clock(source), system clock(source) and  AHB clock*/
    //CLOCK_vCfgSysClk();
	Chip_SetupXtalClocking();

	/* Disable the wakeup pin and this pin can be used for other purposes  */
	PMU_SetWakePadDisable();
}

/* Record reset cause */
void SYSTEM_vHandleResetCause(void)
{
    U32 temp = SYSCTL_nGetRSTStatus();
	SYSCTL_vClearRSTStatus(temp);
	
	if(temp&SYSTEM_PIN_RESET)
	{
       teResetCause = SYSTEM_PIN_RESET;
	}
	else if(temp&SYSTEM_POR_RESET)
	{
       teResetCause = SYSTEM_POR_RESET;
	} 
	else if(temp&SYSTEM_SW_RESET)
	{
       teResetCause = SYSTEM_SW_RESET;
	} 
 	else if(temp&SYSTEM_WDG_RESET)
	{
       teResetCause = SYSTEM_WDG_RESET;
	}  
	else if(temp&SYSTEM_BOD_RESET)
	{
       teResetCause = SYSTEM_BOD_RESET;
	} 	
}

/* Get reset cause */
SYSTEM_RESET_CAUSE_T SYSTEM_tGetResetCause(void)
{
    return teResetCause;
}

/* Resets a peripheral block */ 
void SYSCTL_vPeriphReset(SYSCTL_PERIPH_RESET_T tePeriph)
{
	SYSCTL_vAssertPeriphReset(tePeriph);
	SYSCTL_vDeassertPeriphReset(tePeriph);
}

/* Set UART base rate */
void CLOCK_vSetUARTBaseClock(bool bEnable)
{
	U32 div, inclk;

	/* Input clock into FRG block is the main system clock */
	inclk = CFG_MAIN_CLOCK_FREQ_HZ;

	/* Get integer divider for coarse rate */
	div = inclk / CLOCK_unGetUARTBaseClock();
	if (div == 0) {
		div = 1;
	}

	/* Approximated rate with only integer divider */
	LPC_SYSCTL->UARTCLKDIV = (U32) div;

	if (bEnable) {
		U32 err = 0;
		U64 uart_fra_multiplier = 0;

		err = inclk - (CLOCK_unGetUARTBaseClock() * div);
		uart_fra_multiplier = ((U64)err * (U64)256) / (U64)((U64)CLOCK_unGetUARTBaseClock() * (U64)div);

		/* Enable fractional divider and set multiplier */
        WRITE_REG(LPC_SYSCTL->FRGCTRL, 0xFF | ((uart_fra_multiplier & 0xFF) << 8));
	}
	else {
		/* Disable fractional generator and use integer divider only */
		WRITE_REG(LPC_SYSCTL->FRGCTRL, 0);
	}
}
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

