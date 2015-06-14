/******************************************************************************/
/*                                                                            */
/*    $Workfile::   platform_lpc15xx.h                                           $    */
/*                                                                            */
/*    $Revision:: 182                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 07 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-09-25 11:02:18 +0800 (周四, 25 九月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic definitions of LPC1517                            */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
 #ifndef PLATFORM_1517_H
 #define PLATFORM_1517_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define LPC_AHB_BASE              0x10000000

#define LPC_ADC0_BASE             0x40000000
#define LPC_DAC_BASE              0x40004000
#define LPC_CMP_BASE              0x40008000
#define LPC_INMUX_BASE            0x40014000
#define LPC_RTC_BASE              0x40028000
#define LPC_WWDT_BASE             0x4002C000
#define LPC_SWM_BASE              0x40038000
#define LPC_PMU_BASE              0x4003C000
#define LPC_USART0_BASE           0x40040000
#define LPC_USART1_BASE           0x40044000
#define LPC_SPI0_BASE             0x40048000
#define LPC_SPI1_BASE             0x4004C000
#define LPC_I2C_BASE              0x40050000
#define LPC_QEI_BASE              0x40058000
#define LPC_SYSCTL_BASE           0x40074000
#define LPC_ADC1_BASE             0x40080000
#define LPC_MRT_BASE              0x400A0000
#define LPC_PIN_INT_BASE          0x400A4000
#define LPC_GPIO_GROUP_INT0_BASE  0x400A8000
#define LPC_GPIO_GROUP_INT1_BASE  0x400AC000
#define LPC_RITIMER_BASE          0x400B4000
#define LPC_SCTIPU_BASE           0x400B8000
#define LPC_FLASH_BASE            0x400BC000
#define LPC_USART2_BASE           0x400C0000
#define TBD_BASE                  0x400E8000
#define LPC_C_CAN0_BASE           0x400F0000
#define LPC_IOCON_BASE            0x400F8000
#define LPC_EEPROM_BASE           0x400FC000
#define LPC_GPIO_PIN_INT_BASE     0x1C000000
#define LPC_DMA_BASE              0x1C004000
#define LPC_USB0_BASE             0x1C00C000
#define LPC_CRC_BASE              0x1C010000
#define LPC_SCT0_BASE             (LPC_AHB_BASE + 0x0C018000)
#define LPC_SCT1_BASE             (LPC_AHB_BASE + 0x0C01C000)
#define LPC_SCT2_BASE             (LPC_AHB_BASE + 0x0C020000)
#define LPC_SCT3_BASE             (LPC_AHB_BASE + 0x0C024000)
  
#define LPC_PMU                   ((LPC_PMU_T              *) LPC_PMU_BASE)
#define LPC_DIOCON                ((LPC_DIOCON_T           *) LPC_IOCON_BASE)
#define LPC_SYSCTL                ((LPC_SYSCTL_T           *) LPC_SYSCTL_BASE)
#define LPC_SYSCON                ((LPC_SYSCTL_T           *) LPC_SYSCTL_BASE)	/* Alias for LPC_SYSCTL */
#define LPC_GPIO                  ((LPC_GPIO_T             *) LPC_GPIO_PIN_INT_BASE)
#define LPC_GPIOGROUP             ((LPC_GPIOGROUPINT_T     *) LPC_GPIO_GROUP_INT0_BASE)
#define LPC_PIN_INT               ((LPC_PIN_INT_T          *) LPC_PIN_INT_BASE)
#define LPC_USART0                ((LPC_USART_T            *) LPC_USART0_BASE)
#define LPC_USART1                ((LPC_USART_T            *) LPC_USART1_BASE)
#define LPC_USART2                ((LPC_USART_T            *) LPC_USART2_BASE)
#define LPC_I2C0                  ((LPC_I2C_T              *) LPC_I2C_BASE)
// #define LPC_I2C1                  ((LPC_I2C_T              *) LPC_I2C1_BASE)
// #define LPC_SSP0                  ((LPC_SSP_T              *) LPC_SSP0_BASE)
// #define LPC_SSP1                  ((LPC_SSP_T              *) LPC_SSP1_BASE)
#define LPC_USB                   ((LPC_USB_T              *) LPC_USB0_BASE)
#define LPC_ADC0                  ((LPC_ADC_T              *) LPC_ADC0_BASE)
#define LPC_ADC1                  ((LPC_ADC_T              *) LPC_ADC1_BASE)
#define LPC_SCT0                  ((LPC_SCT_T              *) LPC_SCT0_BASE)
#define LPC_SCT1                  ((LPC_SCT_T              *) LPC_SCT1_BASE)
#define LPC_SCT2                  ((LPC_SCT_T              *) LPC_SCT2_BASE)
#define LPC_SCT3                  ((LPC_SCT_T              *) LPC_SCT3_BASE)
// #define LPC_TIMER16_0          ((LPC_TIMER_T            *) LPC_TIMER16_0_BASE)
// #define LPC_TIMER16_1          ((LPC_TIMER_T            *) LPC_TIMER16_1_BASE)
// #define LPC_TIMER32_0          ((LPC_TIMER_T            *) LPC_TIMER32_0_BASE)
// #define LPC_TIMER32_1          ((LPC_TIMER_T            *) LPC_TIMER32_1_BASE)
#define LPC_RTC                   ((LPC_RTC_T              *) LPC_RTC_BASE)
#define LPC_WWDT                  ((LPC_WWDT_T             *) LPC_WWDT_BASE)
#define LPC_DMA                   ((LPC_DMA_T              *) LPC_DMA_BASE)
#define LPC_CRC                   ((LPC_CRC_T              *) LPC_CRC_BASE)
#define LPC_FMC                   ((LPC_FMC_T              *) LPC_FLASH_BASE)
#define LPC_MRT                   ((LPC_MRT_T              *) LPC_MRT_BASE)
#define LPC_SWM                   ((LPC_SWM_T              *) LPC_SWM_BASE)
#define LPC_RITIMER               ((LPC_RITIMER_T          *) LPC_RITIMER_BASE)
#define LPC_INMUX                 ((LPC_INMUX_T            *) LPC_INMUX_BASE)
#define LPC_SCTIPU                ((LPC_SCTIPU_T           *) LPC_SCTIPU_BASE)
#define LPC_CMP                   ((LPC_CMP_T              *) LPC_CMP_BASE)
#define LPC_DAC                   ((LPC_DAC_T              *) LPC_DAC_BASE)
#define LPC_SPI0                  ((LPC_SPI_T              *) LPC_SPI0_BASE)
#define LPC_SPI1                  ((LPC_SPI_T              *) LPC_SPI1_BASE)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/* AHB/APB Clock 1 enable bits for peripherals on H2 */
#define     EN1_MRT         (1<<0)
#define     EN1_RIT         (1<<1)
#define     EN1_SCT0        (1<<2)
#define     EN1_SCT1        (1<<3)
#define     EN1_SCT2        (1<<4)
#define     EN1_SCT3        (1<<5)
#define     EN1_SCT_IPU     (1<<6)
#define     EN1_CAN         (1<<7)
#define     EN1_SPI0        (1<<9)
#define     EN1_SPI1        (1<<10)
#define     EN1_I2C         (1<<13)
#define     EN1_UART0       (1<<17)
#define     EN1_UART1       (1<<18)
#define     EN1_UART2       (1<<19)
#define     EN1_QEI         (1<<21)
#define     EN1_USB         (1<<23)
#define     EN1_PVT         (1<<28)
#define     EN1_BODY_BIAS   (1<<29)
#define     EN1_EZH         (1<<31)

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

