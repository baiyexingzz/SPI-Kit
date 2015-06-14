/******************************************************************************/
/*                                                                            */
/*    $Workfile::   System.h                                                 $ */
/*                                                                            */
/*    $Revision:: 320                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-14 15:30:51 +0800 (周五, 14 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Export APIs for System Configuration                      */
/*                  including Clock.c and SysCtl.c                            */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H

/** @defgroup SYSCTL_15XX CHIP: System - SysCon, Clock and Power up/down
 * @ingroup 15XX_Drivers
 * @{
 */

#include "platform.h"
#include "system_cfg.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/** Internal oscillator frequency in Hz */
#define SYSCTL_IRC_FREQ_HZ           (12000000UL)

/** Internal watchdog oscillator frequency in Hz */
#define SYSCTL_WDTOSC_FREQ_HZ        (503000UL)

/** Internal RTC oscillator frequency in Hz */
#define SYSCTL_RTC_FREQ_HZ           (32000UL)

/* 100 ticks per second / every 10ms */
/* fixed 10ms for sys tick timer in Hz */
#define SYSTICKRATE_HZ               (100UL)	

/**
 * Power down configuration values
 */
#define SYSCTL_POWERDOWN_TBD0_PD    (1 << 0)	/*!< TBD0 wake-up power down */
#define SYSCTL_POWERDOWN_TBD1_PD    (1 << 1)	/*!< TBD1 wake-up power down */
#define SYSCTL_POWERDOWN_TBD2_PD    (1 << 2)	/*!< TBD2 wake-up power down */
#define SYSCTL_POWERDOWN_IRCOUT_PD  (1 << 3)	/*!< IRC oscillator output wake-up power down */
#define SYSCTL_POWERDOWN_IRC_PD     (1 << 4)	/*!< IRC oscillator power-down wake-up power down */
#define SYSCTL_POWERDOWN_FLASH_PD   (1 << 5)	/*!< Flash wake-up power down */
#define SYSCTL_POWERDOWN_EEPROM_PD  (1 << 6)	/*!< EEPROM wake-up power down */
#define SYSCTL_POWERDOWN_BOD_PD     (1 << 8)	/*!< BOD wake-up power down */
#define SYSCTL_POWERDOWN_USBPHY_PD  (1 << 9)	/*!< USB PHY wake-up power down */
#define SYSCTL_POWERDOWN_ADC0_PD    (1 << 10)	/*!< ADC0 wake-up power down */
#define SYSCTL_POWERDOWN_ADC1_PD    (1 << 11)	/*!< ADC1 wake-up power down */
#define SYSCTL_POWERDOWN_DAC_PD     (1 << 12)	/*!< DAC wake-up power down */
#define SYSCTL_POWERDOWN_ACMP0_PD   (1 << 13)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP1_PD   (1 << 14)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP2_PD   (1 << 15)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP3_PD   (1 << 16)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_IREF_PD    (1 << 17)	/*!< Internal voltage reference wake-up power down */
#define SYSCTL_POWERDOWN_TS_PD      (1 << 18)	/*!< Temperature sensor wake-up power down */
#define SYSCTL_POWERDOWN_VDDADIV_PD (1 << 19)	/*!< VDDA divider wake-up power down */
#define SYSCTL_POWERDOWN_WDTOSC_PD  (1 << 20)	/*!< Watchdog oscillator wake-up power down */
#define SYSCTL_POWERDOWN_SYSOSC_PD  (1 << 21)	/*!< System oscillator wake-up power down */
#define SYSCTL_POWERDOWN_SYSPLL_PD  (1 << 22)	/*!< System PLL wake-up power down */
#define SYSCTL_POWERDOWN_USBPLL_PD  (1 << 23)	/*!< USB PLL wake-up power down */
#define SYSCTL_POWERDOWN_SCTPLL_PD  (1 << 24)	/*!< SCT PLL wake-up power down */

/**
 * System reset status
 */
#define SYSCTL_RST_POR    (1 << 0)	/*!< POR reset status */
#define SYSCTL_RST_EXTRST (1 << 1)	/*!< External reset status */
#define SYSCTL_RST_WDT    (1 << 2)	/*!< Watchdog reset status */
#define SYSCTL_RST_BOD    (1 << 3)	/*!< Brown-out detect reset status */
#define SYSCTL_RST_SYSRST (1 << 4)	/*!< software system reset status */

/* PRESETCTRL0 reg index */
#define PRESETCTRL_REG_INDEX(periph) (((periph) >= 32)? 1 : 0)
/* PRESETCTRL0 shift */
#define PRESETCTRL_MASK(periph) (((periph) >= 32)?(1 << ((U32) (periph) - 32)):(1 << (U32) (periph)))

/* System and peripheral clock mask */
#define SYS_PERIPH_CLK_MASK(clk)   ((clk >= 32)?((1 << (clk - 32))):((1 << clk)))
/* System and peripheral clock reg index */
#define SYS_PERIPH_CLK_REGIDX(clk) ((clk >= 32)? 1 : 0)

/**
 * Non-Maskable Interrupt Change Source Enable/Disable and NMI IRQn value
 */
#define SYSCTL_NMISRC_ENABLE      (1UL << 31)	/*!< Enable the Non-Maskable Interrupt (NMI) source */
#define SYSCTL_NMISRC_IRQN_MASK   (0x3F)	    /*!< The IRQ number of the interrupt that acts as the Non-Maskable Interrupt */

/**
 * Peripheral interrupt wakeup events on STARTERP0 only
 */
#define SYSCTL_ERP0_WAKEUP_WDTINT       (1 << 0)	/*!< WWDT interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_BODINT       (1 << 1)	/*!< Brown out detector interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_GINT0INT     (1 << 5)	/*!< Group interrupt 0 wake-up */
#define SYSCTL_ERP0_WAKEUP_GINT1INT     (1 << 6)	/*!< Group interrupt 1 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT0INT     (1 << 7)	/*!< GPIO pin interrupt 0 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT1INT     (1 << 8)	/*!< GPIO pin interrupt 1 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT2INT     (1 << 9)	/*!< GPIO pin interrupt 2 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT3INT     (1 << 10)	/*!< GPIO pin interrupt 3 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT4INT     (1 << 11)	/*!< GPIO pin interrupt 4 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT5INT     (1 << 12)	/*!< GPIO pin interrupt 5 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT6INT     (1 << 13)	/*!< GPIO pin interrupt 6 wake-up */
#define SYSCTL_ERP0_WAKEUP_PINT7INT     (1 << 14)	/*!< GPIO pin interrupt 7 wake-up */
#define SYSCTL_ERP0_WAKEUP_USART0INT    (1 << 21)	/*!< USART0 interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_USART1INT    (1 << 22)	/*!< USART1 interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_USART2INT    (1 << 23)	/*!< USART2 interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_I2CINT       (1 << 24)	/*!< I2C interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_SPI0INT      (1 << 25)	/*!< SPI0 interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_SPI1INT      (1 << 26)	/*!< SPI1 interrupt wake-up */
#define SYSCTL_ERP0_WAKEUP_USB_WAKEUP   (1 << 30)	/*!< USB need_clock signal wake-up */

/**
 * Peripheral interrupt wakeup events on STARTERP1 only
 */
#define SYSCTL_ERP1_WAKEUP_ACMP0INT     (1 << 8)	/*!< Analog comparator 0 interrupt wake-up */
#define SYSCTL_ERP1_WAKEUP_ACMP1INT     (1 << 9)	/*!< Analog comparator 1 interrupt wake-up */
#define SYSCTL_ERP1_WAKEUP_ACMP2INT     (1 << 10)	/*!< Analog comparator 2 interrupt wake-up */
#define SYSCTL_ERP1_WAKEUP_ACMP3INT     (1 << 11)	/*!< Analog comparator 3 interrupt wake-up */
#define SYSCTL_ERP1_WAKEUP_RTCALARMINT  (1 << 13)	/*!< RTC alarm interrupt wake-up */
#define SYSCTL_ERP1_WAKEUP_RTCWAKEINT   (1 << 14)	/*!< RTC wake (1KHz wake) interrupt wake-up */

/**
 * Deep sleep to wakeup setup values
 */
#define SYSCTL_SLPWAKE_TBD0_PD      (1 << 0)	/*!< TBD0 wake-up configuration */
#define SYSCTL_SLPWAKE_TBD1_PD      (1 << 1)	/*!< TBD1 wake-up configuration */
#define SYSCTL_SLPWAKE_TBD2_PD      (1 << 2)	/*!< TBD2 wake-up configuration */
#define SYSCTL_SLPWAKE_IRCOUT_PD    (1 << 3)	/*!< IRC oscillator output wake-up configuration */
#define SYSCTL_SLPWAKE_IRC_PD       (1 << 4)	/*!< IRC oscillator power-down wake-up configuration */
#define SYSCTL_SLPWAKE_FLASH_PD     (1 << 5)	/*!< Flash wake-up configuration */
#define SYSCTL_SLPWAKE_EEPROM_PD    (1 << 6)	/*!< EEPROM wake-up configuration */
#define SYSCTL_SLPWAKE_BOD_PD       (1 << 8)	/*!< BOD wake-up configuration */
#define SYSCTL_SLPWAKE_USBPHY_PD    (1 << 9)	/*!< USB PHY wake-up configuration */
#define SYSCTL_SLPWAKE_ADC0_PD      (1 << 10)	/*!< ADC0 wake-up configuration */
#define SYSCTL_SLPWAKE_ADC1_PD      (1 << 11)	/*!< ADC1 wake-up configuration */
#define SYSCTL_SLPWAKE_DAC_PD       (1 << 12)	/*!< DAC wake-up configuration */
#define SYSCTL_SLPWAKE_ACMP0_PD     (1 << 13)	/*!< ACMP0 wake-up configuration */
#define SYSCTL_SLPWAKE_ACMP1_PD     (1 << 14)	/*!< ACMP0 wake-up configuration */
#define SYSCTL_SLPWAKE_ACMP2_PD     (1 << 15)	/*!< ACMP0 wake-up configuration */
#define SYSCTL_SLPWAKE_ACMP3_PD     (1 << 16)	/*!< ACMP0 wake-up configuration */
#define SYSCTL_SLPWAKE_IREF_PD      (1 << 17)	/*!< Internal voltage reference wake-up configuration */
#define SYSCTL_SLPWAKE_TS_PD        (1 << 18)	/*!< Temperature sensor wake-up configuration */
#define SYSCTL_SLPWAKE_VDDADIV_PD   (1 << 19)	/*!< VDDA divider wake-up configuration */
#define SYSCTL_SLPWAKE_WDTOSC_PD    (1 << 20)	/*!< Watchdog oscillator wake-up configuration */
#define SYSCTL_SLPWAKE_SYSOSC_PD    (1 << 21)	/*!< System oscillator wake-up configuration */
#define SYSCTL_SLPWAKE_SYSPLL_PD    (1 << 22)	/*!< System PLL wake-up configuration */
#define SYSCTL_SLPWAKE_USBPLL_PD    (1 << 23)	/*!< USB PLL wake-up configuration */
#define SYSCTL_SLPWAKE_SCTPLL_PD    (1 << 24)	/*!< SCT PLL wake-up configuration */

/**
 * Power down configuration values
 */
#define SYSCTL_POWERDOWN_TBD0_PD    (1 << 0)	/*!< TBD0 wake-up power down */
#define SYSCTL_POWERDOWN_TBD1_PD    (1 << 1)	/*!< TBD1 wake-up power down */
#define SYSCTL_POWERDOWN_TBD2_PD    (1 << 2)	/*!< TBD2 wake-up power down */
#define SYSCTL_POWERDOWN_IRCOUT_PD  (1 << 3)	/*!< IRC oscillator output wake-up power down */
#define SYSCTL_POWERDOWN_IRC_PD     (1 << 4)	/*!< IRC oscillator power-down wake-up power down */
#define SYSCTL_POWERDOWN_FLASH_PD   (1 << 5)	/*!< Flash wake-up power down */
#define SYSCTL_POWERDOWN_EEPROM_PD  (1 << 6)	/*!< EEPROM wake-up power down */
#define SYSCTL_POWERDOWN_BOD_PD     (1 << 8)	/*!< BOD wake-up power down */
#define SYSCTL_POWERDOWN_USBPHY_PD  (1 << 9)	/*!< USB PHY wake-up power down */
#define SYSCTL_POWERDOWN_ADC0_PD    (1 << 10)	/*!< ADC0 wake-up power down */
#define SYSCTL_POWERDOWN_ADC1_PD    (1 << 11)	/*!< ADC1 wake-up power down */
#define SYSCTL_POWERDOWN_DAC_PD     (1 << 12)	/*!< DAC wake-up power down */
#define SYSCTL_POWERDOWN_ACMP0_PD   (1 << 13)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP1_PD   (1 << 14)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP2_PD   (1 << 15)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_ACMP3_PD   (1 << 16)	/*!< ACMP0 wake-up power down */
#define SYSCTL_POWERDOWN_IREF_PD    (1 << 17)	/*!< Internal voltage reference wake-up power down */
#define SYSCTL_POWERDOWN_TS_PD      (1 << 18)	/*!< Temperature sensor wake-up power down */
#define SYSCTL_POWERDOWN_VDDADIV_PD (1 << 19)	/*!< VDDA divider wake-up power down */
#define SYSCTL_POWERDOWN_WDTOSC_PD  (1 << 20)	/*!< Watchdog oscillator wake-up power down */
#define SYSCTL_POWERDOWN_SYSOSC_PD  (1 << 21)	/*!< System oscillator wake-up power down */
#define SYSCTL_POWERDOWN_SYSPLL_PD  (1 << 22)	/*!< System PLL wake-up power down */
#define SYSCTL_POWERDOWN_USBPLL_PD  (1 << 23)	/*!< USB PLL wake-up power down */
#define SYSCTL_POWERDOWN_SCTPLL_PD  (1 << 24)	/*!< SCT PLL wake-up power down */

/* PDWAKECFG register mask */
#define PDWAKEUPUSEMASK        (0x00000000)
#define PDWAKEUPMASKTMP        (0x01FFFF78)

/* PDRUNCFG register mask */
#define PDRUNCFGUSEMASK        (0x00000000)
#define PDRUNCFGMASKTMP        (0x01FFFF78)

/* Flash access timeout */
#define SYSCTL_FLASHTIM_SHIFT  (12)
#define SYSCTL_FLASHTIM_MASK   (0x03 << SYSCTL_FLASHTIM_SHIFT)

/**
 * PMU PCON register bit fields & masks
 */
#define PMU_PCON_PM_DEEPPOWERDOWN   (0x1)			/*!< ARM WFI enter Deep Power-down mode */
#define PMU_PCON_NODPD              (1 << 3)	    /*!< Disable deep power-down mode */
#define PMU_PCON_SLEEPFLAG          (1 << 8)	    /*!< Sleep mode flag */
#define PMU_PCON_DPDFLAG            (1 << 11)	    /*!< Deep power-down flag */

/**
 * PMU GPREG[4] register bit fields & masks
 */ 
#define PMU_GPREG4_WAKEUPHYS        (1 << 0)	            /** Enable wake-up pin hysteresis */
#define PMU_GPREG4_WAKEPAD_DISABLE  (1 << 1)	            /** Disable the Wake-up */
#define PMU_GPREG4_DATA             ((U32) 0x3fffff << 10)	/** GP register 4 data field */

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* System reset cause */
typedef enum
{
  SYSTEM_NO_RESET =  0xFF,
  SYSTEM_PIN_RESET = SYSCTL_RST_EXTRST,  //External pin reset status
  SYSTEM_POR_RESET = SYSCTL_RST_POR,     //Reset by power on
  SYSTEM_SW_RESET  = SYSCTL_RST_SYSRST,  //SW reset(system reset)
  SYSTEM_WDG_RESET = SYSCTL_RST_WDT,     //Window watchdog reset
  SYSTEM_BOD_RESET = SYSCTL_RST_BOD      //Reset by BOD - too low or high power
} SYSTEM_RESET_CAUSE_T;

/**
 * @brief LPC15XX System Control block structure
 */
typedef struct {			    /*!< SYSCTL Structure */
	__IO U32  SYSMEMREMAP;		/*!< System Memory remap register */
	__I  U32  RESERVED0[2];
	__IO U32  AHBBUFEN0;		
	__IO U32  AHBBUFEN1;		
	__IO U32  SYSTCKCAL;		/*!< System tick counter calibration register */
	__I  U32  RESERVED1[1];
	__IO U32  NMISRC;			/*!< NMI source control register */
	__I  U32  RESERVED2[8];
	__IO U32  SYSRSTSTAT;		/*!< System Reset Status register */
	__IO U32  PRESETCTRL[2];	/*!< Peripheral reset Control registers */
	__I  U32  PIOPORCAP[3];	    /*!< POR captured PIO status registers */
	__I  U32  RESERVED3[10];
	__IO U32  MAINCLKSELA;		/*!< Main clock source A select register */
	__IO U32  MAINCLKSELB;		/*!< Main clock source B select register */
	__IO U32  USBCLKSEL;		/*!< USB clock source select register */
	__IO U32  ADCASYNCCLKSEL;	/*!< ADC asynchronous clock source select register */
	__I  U32  RESERVED4[1];
	__IO U32  CLKOUTSEL[2];	    /*!< Clock out source select registers */
	__I  U32  RESERVED5[1];
	__IO U32  SYSPLLCLKSEL;   	/*!< System PLL clock source select register */
	__IO U32  USBPLLCLKSEL;	    /*!< USB PLL clock source select register */
	__IO U32  SCTPLLCLKSEL;	    /*!< SCT PLL clock source select register */
	__I  U32  RESERVED6[5];
	__IO U32  SYSAHBCLKDIV;	    /*!< System Clock divider register */
	__IO U32  SYSAHBCLKCTRL[2]; /*!< System clock control registers */
	__IO U32  SYSTICKCLKDIV;	/*!< SYSTICK clock divider */
	__IO U32  UARTCLKDIV;		/*!< UART clock divider register */
	__IO U32  IOCONCLKDIV;		/*!< programmable glitch filter divider registers for IOCON */
	__IO U32  TRACECLKDIV;		/*!< ARM trace clock divider register */
	__I  U32  RESERVED7[4];
	__IO U32  USBCLKDIV;		/*!< USB clock source divider register */
	__IO U32  ADCASYNCCLKDIV;	/*!< Asynchronous ADC clock divider */
	__I  U32  RESERVED8[1];
	__IO U32  CLKOUTDIV;		/*!< Clock out divider register */
	__I  U32  RESERVED9[9];
	__IO U32  FREQMECTRL;		/*!< Frequency measure register */
	__IO U32  FLASHCFG;		    /*!< Flash configuration register */
	__IO U32  FRGCTRL;			/*!< USART fractional baud rate generator control register */
	__IO U32  USBCLKCTRL;		/*!< USB clock control register */
	__I  U32  USBCLKST;		    /*!< USB clock status register */
	__I  U32  RESERVED10[19];
	__IO U32  BODCTRL;			/*!< Brown Out Detect register */
	__I  U32  IRCCTRL;			
	__IO U32  SYSOSCCTRL;		/*!< System Oscillator control register */
	__I  U32  RESERVED11[1];
	__IO U32  RTCOSCCTRL;		/*!< RTC Oscillator control register */
	__I  U32  RESERVED12[1];
	__IO U32  SYSPLLCTRL;		/*!< System PLL control register */
	__I  U32  SYSPLLSTAT;		/*!< System PLL status register */
	__IO U32  USBPLLCTRL;		/*!< USB PLL control register */
	__I  U32  USBPLLSTAT;		/*!< USB PLL status register */
	__IO U32  SCTPLLCTRL;		/*!< SCT PLL control register */
	__I  U32  SCTPLLSTAT;		/*!< SCT PLL status register */
	__I  U32  RESERVED13[21];
	__IO U32  PDWAKECFG;		/*!< Power down states in wake up from deep sleep register */
	__IO U32  PDRUNCFG;		    /*!< Power configuration register*/
	__I  U32  RESERVED14[3];
	__IO U32  STARTERP[2];		/*!< Start logic interrupt wake-up enable registers */
	__I  U32  RESERVED15[117];
	__I  U32  JTAG_IDCODE;		/*!< JTAG ID code register */
	__I  U32  DEVICEID[2];		/*!< Device ID registers */
} LPC_SYSCTL_T;

/**
 * Clock source selections for only the main A system clock. The main A system
 * clock is used as an input into the main B system clock selector. Main clock A
 * only needs to be setup if the main clock A input is used in the main clock
 * system selector.
 */
typedef enum SYSCTL_MAIN_A_CLKSRC {
	SYSCTL_MAIN_A_CLKSRC_IRC = 0,		/*!< Internal oscillator */
	SYSCTL_MAIN_A_CLKSRCA_MAINOSC,		/*!< Crystal (main) oscillator in */
	SYSCTL_MAIN_A_CLKSRCA_SYSOSC  = SYSCTL_MAIN_A_CLKSRCA_MAINOSC,
	SYSCTL_MAIN_A_CLKSRCA_WDTOSC,		/*!< Watchdog oscillator rate */
	SYSCTL_MAIN_A_CLKSRCA_RESERVED,
} SYSCTL_MAIN_A_CLKSRC_T;

/**
 * Clock sources for only main B system clock
 */
typedef enum SYSCTL_MAIN_B_CLKSRC {
	SYSCTL_MAIN_B_CLKSRC_MAINCLKSELA = 0,	/*!< main clock A */
	SYSCTL_MAIN_B_CLKSRC_SYSPLLIN,			/*!< System PLL input */
	SYSCTL_MAIN_B_CLKSRC_SYSPLLOUT,			/*!< System PLL output */
	SYSCTL_MAIN_B_CLKSRC_RTC,				/*!< RTC oscillator 32KHz output */
} SYSCTL_MAIN_B_CLKSRC_T;

/**
 * Clock sources for main system clock. This is a mix of both main clock A
 * and B seelctions.
 */
typedef enum SYSCTL_MAINCLKSRC {
	SYSCTL_MAINCLKSRC_IRC = 0,			/*!< Internal oscillator */
	SYSCTL_MAINCLKSRCA_MAINOSC,			/*!< Crystal (main) oscillator in */
	SYSCTL_MAINCLKSRCA_SYSOSC = SYSCTL_MAINCLKSRCA_MAINOSC,
	SYSCTL_MAINCLKSRCA_WDTOSC,			/*!< Watchdog oscillator rate */
	SYSCTL_MAINCLKSRC_SYSPLLIN = 5,		/*!< System PLL input */
	SYSCTL_MAINCLKSRC_SYSPLLOUT,		/*!< System PLL output */
	SYSCTL_MAINCLKSRC_RTC,				/*!< RTC oscillator 32KHz output */
} SYSCTL_MAINCLKSRC_T;

/* System PLL MSEL and PSEL */
#define SYSPLL_MSEL_MASK          (0x3F)
#define SYSPLL_PSEL_MASK          (0x3)
#define SYSPLL_PSEL_SHIFT         (6)

/* System PLL State Mask */
#define SYSPLL_STATE_MASK         (0x00000001)

/**
 * Clock sources for USB (usb_clk)
 */
typedef enum SYSCTL_USBCLKSRC {
	SYSCTL_USBCLKSRC_IRC = 0,		/*!< Internal oscillator */
	SYSCTL_USBCLKSRC_MAINOSC,		/*!< Crystal (main) oscillator in */
	SYSCTL_USBCLKSRC_SYSOSC = SYSCTL_USBCLKSRC_MAINOSC,
	SYSCTL_USBCLKSRC_PLLOUT,		/*!< USB PLL out */
	SYSCTL_USBCLKSRC_MAINSYSCLK,	/*!< Main system clock (B) */
} SYSCTL_USBCLKSRC_T;

/**
 * Clock sources for ADC asynchronous clock source select
 */
typedef enum SYSCTL_ADCASYNCCLKSRC {
	SYSCTL_ADCASYNCCLKSRC_IRC = 0,		/*!< Internal oscillator */
	SYSCTL_ADCASYNCCLKSRC_SYSPLLOUT,	/*!< System PLL out */
	SYSCTL_ADCASYNCCLKSRC_USBPLLOUT,	/*!< USB PLL out */
	SYSCTL_ADCASYNCCLKSRC_SCTPLLOUT		/*!< SCT PLL out */
} SYSCTL_ADCASYNCCLKSRC_T;

/**
 * Clock sources for CLKOUT
 */
typedef enum SYSCTL_CLKOUTSRC {
	SYSCTL_CLKOUTSRC_IRC = 0,		/*!< Internal oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_MAINOSC,		/*!< Main oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_SYSOSC = SYSCTL_CLKOUTSRC_MAINOSC,
	SYSCTL_CLKOUTSRC_WDTOSC,		/*!< Watchdog oscillator for CLKOUT */
	SYSCTL_CLKOUTSRC_MAINSYSCLK,	/*!< Main (B) system clock for CLKOUT */
	SYSCTL_CLKOUTSRC_USBPLLOUT = 5,	/*!< USB PLL out */
	SYSCTL_CLKOUTSRC_SCTPLLOUT,		/*!< SCT PLL out */
	SYSCTL_CLKOUTSRC_RTC32K			/*!< RTC 32 kHz output */
} SYSCTL_CLKOUTSRC_T;

/**
 * Clock sources for system, USB, and SCT PLLs
 */
typedef enum SYSCTL_PLLCLKSRC {
	SYSCTL_PLLCLKSRC_IRC = 0,		/*!< Internal oscillator in (may not work for USB) */
	SYSCTL_PLLCLKSRC_MAINOSC,		/*!< Crystal (main) oscillator in */
	SYSCTL_PLLCLKSRC_SYSOSC = SYSCTL_PLLCLKSRC_MAINOSC,
	SYSCTL_PLLCLKSRC_RESERVED1,		/*!< Reserved */
	SYSCTL_PLLCLKSRC_RESERVED2,		/*!< Reserved */
} SYSCTL_PLLCLKSRC_T;

/**
 * System and peripheral clocks
 */
typedef enum SYSCTL_CLOCK {
	/* Peripheral clock enables for SYSAHBCLKCTRL0 */
	SYSCTL_CLOCK_SYS      = 0,			/*!< System clock */
	SYSCTL_CLOCK_ROM,					/*!< ROM clock */
	SYSCTL_CLOCK_SRAM1    = 3,			/*!< SRAM1 clock */
	SYSCTL_CLOCK_SRAM2,					/*!< SRAM2 clock */
	SYSCTL_CLOCK_FLASH    = 7,			/*!< FLASH controller clock */
	SYSCTL_CLOCK_EEPROM   = 9,			/*!< EEPROM controller clock */
	SYSCTL_CLOCK_MUX      = 11,			/*!< Input mux clock */
	SYSCTL_CLOCK_SWM,					/*!< Switch matrix clock */
	SYSCTL_CLOCK_IOCON,					/*!< IOCON clock */
	SYSCTL_CLOCK_GPIO0,					/*!< GPIO0 clock */
	SYSCTL_CLOCK_GPIO1,					/*!< GPIO1 clock */
	SYSCTL_CLOCK_GPIO2,					/*!< GPIO2 clock */
	SYSCTL_CLOCK_PININT   = 18,			/*!< PININT clock */
	SYSCTL_CLOCK_GINT,					/*!< Grouped pin interrupt block clock */
	SYSCTL_CLOCK_DMA,					/*!< DMA clock */
	SYSCTL_CLOCK_CRC,					/*!< CRC clock */
	SYSCTL_CLOCK_WDT,					/*!< WDT clock */
	SYSCTL_CLOCK_RTC,					/*!< RTC clock */
	SYSCTL_CLOCK_ADC0    = 27,			/*!< ADC0 clock */
	SYSCTL_CLOCK_ADC1,					/*!< ADC1 clock */
	SYSCTL_CLOCK_DAC,					/*!< DAC clock */
	SYSCTL_CLOCK_ACMP,					/*!< ACMP clock */
	/* Peripheral clock enables for SYSAHBCLKCTRL1 */
	SYSCTL_CLOCK_MRT    = 32,			/*!< multi-rate timer clock */
	SYSCTL_CLOCK_RIT,					/*!< repetitive interrupt timer clock */
	SYSCTL_CLOCK_SCT0,					/*!< SCT0 clock */
	SYSCTL_CLOCK_SCT1,					/*!< SCT1 clock */
	SYSCTL_CLOCK_SCT2,					/*!< SCT2 clock */
	SYSCTL_CLOCK_SCT3,					/*!< SCT3 clock */
	SYSCTL_CLOCK_SCTIPU,				/*!< SCTIPU clock */
	SYSCTL_CLOCK_CAN,					/*!< CAN clock */
	SYSCTL_CLOCK_SPI0 = 32 + 9,			/*!< SPI0 clock */
	SYSCTL_CLOCK_SPI1,					/*!< SPI1 clock */
	SYSCTL_CLOCK_I2C0 = 32 + 13,		/*!< I2C0 clock */
	SYSCTL_CLOCK_UART0 = 32 + 17,		/*!< UART0 clock */
	SYSCTL_CLOCK_UART1,					/*!< UART1 clock */
	SYSCTL_CLOCK_UART2,					/*!< UART2 clock */
	SYSCTL_CLOCK_QEI = 32 + 21,			/*!< QEI clock */
	SYSCTL_CLOCK_USB = 32 + 23,			/*!< USB clock */
} SYSCTL_CLOCK_T;

/**
 * Peripheral reset identifiers, not available on all devices
 */
typedef enum {
	/* PRESETCTRL0 resets */
	RESET_FLASH = 7,		/*!< FLASH controller reset control */
	RESET_EEPROM = 9,		/*!< EEPROM controller reset control */
	RESET_MUX = 11,			/*!< Input mux reset control */
	RESET_IOCON = 13,		/*!< IOCON reset control */
	RESET_PININT = 18,		/*!< Pin interrupt (PINT) reset reset control */
	RESET_GINT,				/*!< Grouped interrupt (GINT) reset control */
	RESET_DMA,				/*!< DMA reset control */
	RESET_CRC,				/*!< CRC reset control */
	RESET_ADC0 = 27,		/*!< ADC0 reset control */
	RESET_ADC1,				/*!< ADC1 reset control */
	RESET_ACMP = 30,		/*!< Analog Comparator (all 4 ACMP) reset control */
	RESET_MRT = 32 + 0,		/*!< Multi-rate timer (MRT) reset control */
	RESET_RIT,				/*!< Repetitive interrupt timer (RIT) reset control */
	RESET_SCT0,				/*!< State configurable timer 0 (SCT0) reset control */
	RESET_SCT1,				/*!< State configurable timer 1 (SCT1) reset control */
	RESET_SCT2,				/*!< State configurable timer 2 (SCT2) reset control */
	RESET_SCT3,				/*!< State configurable timer 3 (SCT3) reset control */
	RESET_SCTIPU,			/*!< State configurable timer IPU (SCTIPU) reset control */
	RESET_CAN,				/*!< CAN reset control */
	RESET_SPI0 = 32 + 9,	/*!< SPI0 reset control */
	RESET_SPI1,				/*!< SPI1 reset control */
	RESET_I2C0 = 32 + 13,	/*!< I2C0 reset control */
	RESET_UART0 = 32 + 17,	/*!< UART0 reset control */
	RESET_UART1,			/*!< UART1 reset control */
	RESET_UART2,			/*!< UART2 reset control */
	RESET_QEI0 = 32 + 21,	/*!< QEI0 reset control */
	RESET_USB = 32 + 23		/*!< USB reset control */
} SYSCTL_PERIPH_RESET_T;

/**
 * System memory remap modes used to remap interrupt vectors
 */
typedef enum SYSCTL_BOOT_MODE_REMAP {
	REMAP_BOOT_LOADER_MODE = 0,	/*!< Interrupt vectors are re-mapped to Boot ROM */
	REMAP_USER_RAM_MODE,		/*!< Interrupt vectors are re-mapped to Static RAM */
	REMAP_USER_FLASH_MODE		/*!< Interrupt vectors are not re-mapped and reside in Flash */
} SYSCTL_BOOT_MODE_REMAP_T;

/**
 * Brown-out detector reset level
 */
typedef enum SYSCTL_BODRSTLVL {
	SYSCTL_BODRSTLVL_RESERVED0,
	SYSCTL_BODRSTLVL_RESERVED1,
	SYSCTL_BODRSTLVL_2_34V,	/*!< Brown-out reset at 2.34v */
	SYSCTL_BODRSTLVL_2_64V,	/*!< Brown-out reset at 2.64v */
} SYSCTL_BODRSTLVL_T;

/**
 * Brown-out detector interrupt level
 */
typedef enum SYSCTL_BODRINTVAL {
	SYSCTL_BODINTVAL_RESERVED0,	
	SYSCTL_BODINTVAL_RESERVED1,	
	SYSCTL_BODINTVAL_2_55V,	/*!< Brown-out interrupt at 2.55v */
	SYSCTL_BODINTVAL_2_83V,	/*!< Brown-out interrupt at 2.83v */
} SYSCTL_BODRINTVAL_T;

/**
 * @brief FLASH Access time definitions
 */
typedef enum {
	SYSCTL_FLASHTIM_25MHZ_CPU = (0 << SYSCTL_FLASHTIM_SHIFT),	/*!< Flash accesses use 1 CPU clocks. Use for up to 25 MHz CPU clock*/
	SYSCTL_FLASHTIM_55MHZ_CPU = (1 << SYSCTL_FLASHTIM_SHIFT),	/*!< Flash accesses use 2 CPU clocks. Use for up to 55 MHz CPU clock*/
	SYSCTL_FLASHTIM_72MHZ_CPU = (2 << SYSCTL_FLASHTIM_SHIFT),	/*!< Flash accesses use 3 CPU clocks. Use for up to 72 MHz CPU clock*/
} SYSCTL_FLASHTIM_T;

/**
 * @brief LPC15xx Power Management Unit register block structure
 */
typedef struct {
	__IO U32 PCON;		/*!< Offset: 0x000 Power control Register (R/W) */
	__IO U32 GPREG[5];	/*!< Offset: 0x004 General purpose Registers 0..4 (R/W) */
} LPC_PMU_T;

/**
 * @brief LPC15xx low power mode type definitions
 */
typedef enum CHIP_PMU_MCUPOWER {
	PMU_MCU_SLEEP = 0,		/*!< Sleep mode */
	PMU_MCU_DEEP_SLEEP,		/*!< Deep Sleep mode */
	PMU_MCU_POWER_DOWN,		/*!< Power down mode */
	PMU_MCU_DEEP_PWRDOWN	/*!< Deep power down mode */
} CHIP_PMU_MCUPOWER_T;
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief Initialize the System configuration of MCU including Clock and SYSCTL 
 * @param	Nothing
 * @return	Nothing
 */
void SYSTEM_vInit(void);

/**
 * @brief Reset MCU
 * @param	Nothing
 * @return	Nothing
 */
#define SYSTEM_vReset() NVIC_SystemReset()

/**
 * @brief Description: Record reset cause flag for later query, and then clear current
*              CSR. Must be called before calling SYSTEM_teResetCause
 * @param	Nothing
 * @return	Nothing
 */
void SYSTEM_vHandleResetCause(void);

/**
 * @brief Description: Record reset cause flag for later query, and then clear current
*              CSR. Must be called before calling SYSTEM_tResetCause
 * @param	Nothing
 * @return	Nothing
 */

/**
 * @brief Get reset cause. Call only after SYSTEM_vHandleResetCause
*              has been called.
 * @param	Nothing
 * @return	Nothing
 */ 
SYSTEM_RESET_CAUSE_T SYSTEM_tGetResetCause(void);

/**
 * @brief Check watchdog reset flag. Call only after SYSTEM_vHandleResetCause
 *              has been called.
 * @param	Nothing
 * @return	Nothing
 */ 
STATIC INLINE bool SYSTEM_bWatchdogReset()
{
	return (SYSTEM_tGetResetCause() == SYSTEM_WDG_RESET);
}

/**
 * @brief Check system reset flag. Call only after SYSTEM_vHandleResetCause
**              has been called.
 * @param	Nothing
 * @return	Nothing
 */ 
STATIC INLINE bool SYSTEM_bSoftwareReset() 
{
	return ((SYSTEM_tGetResetCause() == SYSTEM_SW_RESET));
}

/**
 * @brief	Return main clock rate
 * @param	Nothing 
 * @return	system clock rate (unit Hz)
 */
STATIC INLINE U32 CLOCK_nGetMainClock(void)
{
    return (CFG_MAIN_CLOCK_FREQ_HZ);
}

/**
 * @brief	Return system clock rate
 * @param	Nothing 
 * @return	system clock rate (unit Hz)
 */
STATIC INLINE U32 CLOCK_nGetSysClock(void)
{
    return (CFG_SYS_CLOCK_FREQ_HZ);
}

/**
 * @brief	Return system clock rate
 * @param	Nothing 
 * @return	system clock rate (unit MHz)
 */
STATIC INLINE U32 CLOCK_nGetSysClockMHz(void)
{
    return (CFG_SYS_CLOCK_FREQ_MHZ);
}

/**
 * @brief	Get sys tick timer value (unit tick)
 * @param	Nothing
 * @return	sys tick timer value
 */
STATIC INLINE U32 CLOCK_nGetSysTick(void)
{
   return (CFG_SYS_CLOCK_FREQ_HZ / SYSTICKRATE_HZ);
}

/**
 * @brief	Set UART base rate
 * @param	nRate : base rate
 * @param   bEnable: fractional generator enable/disable
 * @return	Nothing
 * @note	The system clock rate is the main system clock divided by this div value.
 */
void CLOCK_vSetUARTBaseClock(bool bEnable);

/**
 * @brief	Get UART base rate
 * @param	nRate : base rate
 * @param   bEnable: fractional generator enable/disable
 * @return	Nothing
 * @note	The system clock rate is the main system clock divided by this div value.
 */
STATIC INLINE U32 CLOCK_unGetUARTBaseClock()
{
    return (CFG_UART_BASE_CLOCK_HZ);
}

/**
 * @brief	Enable a system or peripheral clock
 * @param	teClk	: Clock to enable
 * @return	Nothing
 */
STATIC INLINE void CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_T teClk)
{
	SET_REG_FIELD(LPC_SYSCTL->SYSAHBCLKCTRL[SYS_PERIPH_CLK_REGIDX(teClk)], 
		          SYS_PERIPH_CLK_MASK(teClk),
		          SYS_PERIPH_CLK_MASK(teClk));
}

/**
 * @brief	Disable a system or peripheral clock
 * @param	teClk	: Clock to enable
 * @return	Nothing
 */
STATIC INLINE void CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_T teClk)
{
	SET_REG_FIELD(LPC_SYSCTL->SYSAHBCLKCTRL[SYS_PERIPH_CLK_REGIDX(teClk)], 
		          SYS_PERIPH_CLK_MASK(teClk),
		          0);
}

/**
 * @brief	Power up one or more blocks or peripherals
 * @param	unPowerupmask	: OR'ed values of SYSCTL_POWERDOWN_* values
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_vPowerUp(U32 unPowerupmask)
{
    /* Write 0 to power up */
	SET_REG_FIELD(LPC_SYSCTL->PDRUNCFG, (unPowerupmask & PDRUNCFGMASKTMP), 0); 	
	
}

/**
 * @brief	Power down one or more blocks or peripherals
 * @param	unPowerdownmask	: OR'ed values of SYSCTL_POWERDOWN_* values
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_vPowerDown(U32 unPowerdownmask)
{
    /* Write 1 to power down */
	SET_REG_FIELD(LPC_SYSCTL->PDRUNCFG, (unPowerdownmask & PDRUNCFGMASKTMP), unPowerdownmask); 
}

/**
 * @brief	Resets a peripheral
 * @param	tePeriph	:	Peripheral to reset
 * @return	Nothing
 */
void SYSCTL_vPeriphReset(SYSCTL_PERIPH_RESET_T tePeriph);

/**
 * @brief	Set FLASH access time in clocks
 * @param	teClks	: Clock cycles for FLASH access (minus 1)
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_FMC_SetFLASHAccess(SYSCTL_FLASHTIM_T teClks)
{
 	SET_REG_FIELD(LPC_SYSCTL->FLASHCFG, SYSCTL_FLASHTIM_MASK, teClks);
}

/**
 * @brief	Re-map interrupt vectors
 * @param	teRemap	: system memory map value
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_vRemap(SYSCTL_BOOT_MODE_REMAP_T teRemap)
{
	WRITE_REG(LPC_SYSCTL->SYSMEMREMAP, (U32)teRemap);
}

/**
 * @brief	Enable interrupt used for NMI source
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_EnableNMISource(void)
{
	SET_REG_FIELD(LPC_SYSCTL->NMISRC, SYSCTL_NMISRC_ENABLE, SYSCTL_NMISRC_ENABLE);
}

/**
 * @brief	Disable interrupt used for NMI source
 * @return	Nothing
 */
STATIC INLINE void SYSCTL_DisableNMISource(void)
{
	SET_REG_FIELD(LPC_SYSCTL->NMISRC, SYSCTL_NMISRC_ENABLE, 0);
}

/**
 * @brief	Set source for non-maskable interrupt (NMI)
 * @param	unIntsrc	: IRQ number(>= WDT_IRQn) to assign to the NMI (IRQn_Type)
 * @return	Nothing
 * @note	The NMI source will be disabled.
 *          Upon exiting this function, use the SYSCTL_EnableNMISource() function to enable the NMI source.
 */
STATIC INLINE void SYSCTL_SetNMISource(IRQn_Type unIntsrc)
{
	SYSCTL_DisableNMISource();/* Disable NMI firstly */

	SET_REG_FIELD(LPC_SYSCTL->NMISRC, SYSCTL_NMISRC_IRQN_MASK, unIntsrc);
}


STATIC INLINE void Clock_SetUSBPLLSource(SYSCTL_PLLCLKSRC_T src)
{
	LPC_SYSCTL->USBPLLCLKSEL  = (uint32_t) src;
}

STATIC INLINE void Clock_SetupUSBPLL(uint8_t msel, uint8_t psel)
{
	LPC_SYSCTL->USBPLLCTRL = (msel & 0x3F) | ((psel & 0x3) << 6);
}
STATIC INLINE bool Chip_Clock_IsUSBPLLLocked(void)
{
	return (bool) ((LPC_SYSCTL->USBPLLSTAT & 1) != 0);
}
STATIC INLINE void Chip_Clock_SetUSBClockSource(SYSCTL_USBCLKSRC_T src, uint32_t div)
{
	LPC_SYSCTL->USBCLKSEL = (uint32_t) src;
	LPC_SYSCTL->USBCLKDIV = div;
}


#endif

/**
 * @}
 */

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


