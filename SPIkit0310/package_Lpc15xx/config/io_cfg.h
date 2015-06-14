/******************************************************************************/
/*                                                                            */
/*    $Workfile::   IO_Cfg.h                                            $    */
/*                                                                            */
/*    $Revision:: 290                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 08 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-04 16:25:26 +0800 (周二, 04 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic configuration of IOs.                             */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef IO_CFG_H
#define IO_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/** 
    Descriptions for per IO pin's configurations:
    * IO_PX_XX_FUNC: GPIO, MovableFunc and FixedFunc
    * IO_PX_XX_INITSTAT: Init state for targer pin defined as following	                       
            GPIO Pin:[INITSTAT]: [InitLevel][InitDir]
            Movable Funcation Pin:[INITSTAT]: Moveable Func
            Fixed Funcation Pin:[INITSTAT]: Fixed Func
    */

#define INIT_GPIO_STAT(l,d)   ((U16)(((U8)l)|(((U8)d)<<8)))
#define INIT_GPIO_LEVEL(s)    ((U8)(s&0xFF))
#define INIT_GPIO_DIR(s)      ((U8)((s>>8)&0xFF))

/**
 *    Green MCU 
**/

/* IO Number in port group 0 */
#define UI_STROBE_B_PIN        IO_P0_00                               /* Alias: StrobeB  */
#define IO_P0_00_USAGE         IO_MovableFunc                                /* Pin Usage */
#define IO_P0_00_INITSTAT      SWM_SPI0_SCK_IO   /* Init State */                               
#define IO_P0_00_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_RESERVED_RXD       IO_P0_01                               /* Reserved for 2pin debug - RXD */
#define IO_P0_01_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_01_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */                               
#define IO_P0_01_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_RESERVED_TXD       IO_P0_02                               /* Reserved for 2pin debug - TXD */
#define IO_P0_02_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_02_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */                               
#define IO_P0_02_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define G12_DIM_PIN            IO_P0_03                               /* Alias: IO for output MOS gate G12DIM */
#define IO_P0_03_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_03_INITSTAT      INIT_GPIO_STAT(DIO_HIGH, GPIO_DIR_OUT) /* Init State */                              
#define IO_P0_03_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define ALIAS_IO_P0_04         IO_P0_04                               /* Alias: DEBUG pin */
#define IO_P0_04_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_04_INITSTAT      INIT_GPIO_STAT(DIO_HIGH, GPIO_DIR_OUT) /* Init State */                                     
#define IO_P0_04_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define G12_SHORT_CAP_PIN      IO_P0_05                               /* Alias: reserved */
#define IO_P0_05_USAGE         IO_MovableFunc                                /* Pin Usage */
#define IO_P0_05_INITSTAT      SWM_SPI0_MOSI_IO  /* Init State */                               
#define IO_P0_05_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define ALIAS_IO_P0_06         IO_P0_06                               /* Alias: Debug pin */
#define IO_P0_06_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_06_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */                            
#define IO_P0_06_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_R2_LOW             IO_P0_07                               /* Alias: IO for R2 Low */
#define IO_P0_07_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_07_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */
#define IO_P0_07_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_R2_UP              IO_P0_08                               /* Alias: IO for R2 Up */
#define IO_P0_08_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_08_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */                              
#define IO_P0_08_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_G2_ADC_ILED        IO_P0_09                               /* Alias: I LED ADC G2 */
#define IO_P0_09_USAGE         IO_GPIO                           /* Pin Usage */
#define IO_P0_09_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)                      /* Fixed function */
#define IO_P0_09_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */

#define PIN_G2_ADC_IMOS        IO_P0_10                               /* Alias: I MOS ADC G2 */
#define IO_P0_10_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_10_INITSTAT      SWM_FIXED_ADC1_2                       /* Fixed Function */
#define IO_P0_10_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define PIN_G1_ADC_VO          IO_P0_11                               /* Alias: VO ADC G1 */  
#define IO_P0_11_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_11_INITSTAT      SWM_FIXED_ADC1_3                       /* Fixed Function */
#define IO_P0_11_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define PIN_DAC_IPKOUT         IO_P0_12                               /* Alias: DAC_OUT for IPK */
#define IO_P0_12_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_12_INITSTAT      SWM_FIXED_DAC_OUT                      /* Fixed Function */
#define IO_P0_12_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define PIN_G1_ADC_IMOS        IO_P0_13                               /* Alias: I MOS ADC G1 */
#define IO_P0_13_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_13_INITSTAT      SWM_FIXED_ADC1_6                       /* Fixed function */
#define IO_P0_13_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */ 

#define PIN_ADC1_7             IO_P0_14                               /* Alias: ADC1_7 */
#define IO_P0_14_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_14_INITSTAT      SWM_FIXED_ADC1_7                       /* Fixed function */
#define IO_P0_14_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */ 

#define PIN_ADC_GVIN           IO_P0_15                               /* Alias: G_VIN */
#define IO_P0_15_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_15_INITSTAT      SWM_FIXED_ADC1_8                       /* Fixed function */
#define IO_P0_15_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */ 

#define PIN_G1_BOOST_PWM       IO_P0_16                               /* Alias: PWM for boost G1 (gate) sct3 output0 */
#define IO_P0_16_USAGE         IO_MovableFunc                         /* Pin Usage */
#define IO_P0_16_INITSTAT      SWM_SPI0_MISO_IO                       /* Movable Function */
#define IO_P0_16_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO COnfig */ 

#define PIN_G1_ZCD             IO_P0_17                               /* Alias: ZCD G1 via SCT0_INMUX0*/
#define IO_P0_17_USAGE         IO_MovableFunc                                /* Pin Usage */
#define IO_P0_17_INITSTAT      SWM_SCT3_OUT0_O    /* Init State */
#define IO_P0_17_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO COnfig */ 

#define PIN_G2_BOOST_PWM       IO_P0_18                               /* Alias: Pwm for boost G2 (gate)sct3 output1 */
#define IO_P0_18_USAGE         IO_MovableFunc                         /* Pin Usage */
#define IO_P0_18_INITSTAT      SWM_SCT3_OUT1_O                        /* Movable Function */
#define IO_P0_18_CFG           DIOCON_MODE_PULLUP|DIOCON_DIGMODE_EN   /* IO Config */

#define PIN_G_SWCLK            IO_P0_19                               /* Alias: SWCLK */
#define IO_P0_19_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_19_INITSTAT      SWM_FIXED_SWCLK_TCK                    /* Fixed Fcuntion */
#define IO_P0_19_CFG           DIOCON_DIGMODE_EN    /* IO Config */
 
#define PIN_G_SWDIO            IO_P0_20                               /* Alias: SWDIO 4pin programming/debug */
#define IO_P0_20_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_20_INITSTAT      SWM_FIXED_SWDIO                        /* Fixed Fcuntion */
#define IO_P0_20_CFG           DIOCON_DIGMODE_EN    /* IO Config */

//#define PIN_G_RST              IO_P0_21                               /* Alias: Reset 4pin programming/debug */
//#define IO_P0_21_USAGE         IO_FixedFunc                           /* Pin Usage */
//#define IO_P0_21_INITSTAT      SWM_FIXED_RESETN                       /* Fixed Fcuntion */
//#define IO_P0_21_CFG           DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_SSI_DRV_OEZ        IO_P0_22                               /* Alias: To TI SSI_DRV_OEZ */
#define IO_P0_22_USAGE         IO_MovableFunc                                /* Pin Usage */
#define IO_P0_22_INITSTAT      SWM_SPI0_SSELSN_0_IO/* Init State */
#define IO_P0_22_CFG            DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO Config */

#define PIN_SSI_DRV_DIRZ       IO_P0_23                               /* Alias: To TI SSI_DRV_DIRZ */
#define IO_P0_23_USAGE         IO_GPIO    /* Pin Usage */
#define IO_P0_23_INITSTAT      INIT_GPIO_STAT(DIO_HIGH, GPIO_DIR_OUT)
#define IO_P0_23_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO Config */

#define UI_TI_LITZ_PIN         IO_P0_24                               /* Alias: To TI SSI_LITZ (Debug pin for prototype) */
#define IO_P0_24_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_24_INITSTAT      INIT_GPIO_STAT(DIO_HIGH, GPIO_DIR_IN)  /* Init State */
#define IO_P0_24_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO Config */

#define PIN_COM1_IN1           IO_P0_25                               /* Alias: com1 input1 (I pk HighRef Iset)G1 to Dac */
#define IO_P0_25_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_25_INITSTAT      SWM_FIXED_ACMP0_I4                     /* Fixed Function */
#define IO_P0_25_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define UI_TI_SUBF_PORT        IO_P0_26                               /* Alias: SubFrame signal from TI */
#define IO_P0_26_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_26_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P0_26_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_COM1_IN2           IO_P0_27                               /* Alias: com1 input2 (Imos_G1)  */   
#define IO_P0_27_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_27_INITSTAT      SWM_FIXED_ACMP0_I3                     /* Fixed Function */
#define IO_P0_27_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define UI_STROBE_R_PIN        IO_P0_28                               /* Alias StrobeR */
#define IO_P0_28_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P0_28_INITSTAT      SWM_FIXED_ACMP1_I3                     /* Fixed Function */ 
#define IO_P0_28_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define PIN_STROBE_B           IO_P0_29                               /* Alias: StrobeB  */
#define IO_P0_29_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_29_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P0_29_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define UI_STROBE_G_PIN        IO_P0_30                               /* Alias: StrobeG  */
#define IO_P0_30_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_30_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P0_30_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define UI_TI_RESTZ_PIN        IO_P0_31                               /* Alias: Reset signal from TI */
#define IO_P0_31_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P0_31_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)   /* Init State */
#define IO_P0_31_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO Config */

/* IO Number in port group 1 */
#define UI_STROBE_S_PIN        IO_P1_00                               /* Alias: Strobe Sensor */
#define IO_P1_00_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_00_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P1_00_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_G2_ADC_VO          IO_P1_01                               /* Alias: Vo ADC G2 */
#define IO_P1_01_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P1_01_INITSTAT      SWM_FIXED_ADC1_0                       /* Fixed function */
#define IO_P1_01_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */

#define PIN_ADC1_4             IO_P1_02                               /* Alias: ADC1_4 */
#define IO_P1_02_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P1_02_INITSTAT      SWM_FIXED_ADC1_4                       /* Fixed function */
#define IO_P1_02_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */

#define PIN_G1_ADC_ILED        IO_P1_03                               /* Alias: I LED ADC G1 */  
#define IO_P1_03_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P1_03_INITSTAT      SWM_FIXED_ADC1_5                       /* Fixed function */
#define IO_P1_03_CFG           DIOCON_ADMODE_EN                       /* IO COnfig */                      

#define PIN_G2_ZCD             IO_P1_04                               /* Alias: ZCD G2 via SCT1_INMUX0 */ 
#define IO_P1_04_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_04_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State*/
#define IO_P1_04_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define ALIAS_IO_P1_05         IO_P1_05                               /* Alias: IO_P1_05 */ 
#define IO_P1_05_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_05_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */
#define IO_P1_05_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_SSI_DRV_CLK        IO_P1_06                               /* Alias: TI SSI_DRV_CLK */
#define IO_P1_06_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_06_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P1_06_CFG           DIOCON_MODE_PULLUP|DIOCON_DIGMODE_EN   /* IO Config */

#define UI_TI_ENZ_PIN          IO_P1_07                               /* Alias: TO TI SSI_ENZ */
#define IO_P1_07_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_07_INITSTAT      INIT_GPIO_STAT(DIO_HIGH, GPIO_DIR_OUT)   /* Init State */
#define IO_P1_07_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO Config */

#define ALIAS_IO_P1_08         IO_P1_08                               /* Alias: reserved */
#define IO_P1_08_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_08_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */
#define IO_P1_08_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define ALIAS_IO_P1_09         IO_P1_09                               /* Alias: reserved */
#define IO_P1_09_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P1_09_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */
#define IO_P1_09_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_COM2_IN1           IO_P1_10                               /* Alias: com2 input1 (I pk HighRef Iset)G2 */  
#define IO_P1_10_USAGE         IO_FixedFunc                           /* Pin Usage */
#define IO_P1_10_INITSTAT      SWM_FIXED_ACMP1_I4                     /* Fixed Function */  
#define IO_P1_10_CFG           DIOCON_ADMODE_EN                       /* IO Config */

#define ALIAS_IO_P1_11         IO_P1_11                               /* Alias: reserved */
#define IO_P1_11_USAGE         IO_MovableFunc                         /* Pin Usage */
#define IO_P1_11_INITSTAT      SWM_ADC1_PIN_TRIG0_I                   /* Init State */
#define IO_P1_11_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN   /* IO COnfig */ 

/* IO Number in port group 2 */
#define ADC_SEQA_TRIGER_INDEX  IO_P2_12                               /* Alias: To TI SSI_DRV_DIN */
#define IO_P2_12_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P2_12_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_IN)   /* Init State */
#define IO_P2_12_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */

#define PIN_SSI_DRV_DOUT       IO_P2_13                               /* Alias: To TI SSI_DRV_DOUT */
#define IO_P2_13_USAGE         IO_GPIO                                /* Pin Usage */
#define IO_P2_13_INITSTAT      INIT_GPIO_STAT(DIO_LOW, GPIO_DIR_OUT)  /* Init State */
#define IO_P2_13_CFG           DIOCON_MODE_INACT|DIOCON_DIGMODE_EN    /* IO Config */


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

