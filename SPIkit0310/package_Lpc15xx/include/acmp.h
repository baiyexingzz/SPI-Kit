/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ACMP.h                                                  $ */
/*                                                                            */
/*    $Revision:: 265                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-29 16:03:26 +0800 (周三, 29 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - ACMP Controller */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef ACMP_H
#define ACMP_H

#include "platform.h"
#include "system.h"

#include "acmp_cfg.h"

#ifdef CFG_ACMP_USED

/** @defgroup ACMP_15XX CHIP: LPC15xx ACMP Controller driver
 *  @ingroup 15XX_Drivers
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define ACMP_MAX_NUMBER        (4)	
	
/* Bit definitions for block control register */
#define ACMP_ROSCCTL_SHIFT     (8)		    /* Ring Oscillator control bit - shift */
#define ACMP_ROSCCTL_BIT       (1 << 8)		/* Ring Oscillator control bit */
#define ACMP_ROSCCTL_SET1_RST0 (1 << 8)     /* ROSC output is set by ACMP1 and reset by ACMP0 */
#define ACMP_ROSCCTL_SET0_RST1 (0 << 8)     /* ROSC output is set by ACMP0 and reset by ACMP1 */

#define ACMP_EXTRESET_SHIFT    (9)		    /* Reset source for ring oscillator 0 - shift */
#define ACMP_EXTRESET_BIT      (1 << 9)		/* Reset source for ring oscillator */
#define ACMP_EXTRESET_EXT      (1 << 9)     /* Reset source for ring oscillator 1 - External pin */
#define ACMP_EXTRESET_INT      (0 << 9)     /* Reset source for ring oscillator 0 - Internal chip reset */

/* Bit definitions for compare register */
#define ACMP_CMPEN_BIT         (1 << 0)		/* Comparator enable bit */
#define ACMP_INTEN_BIT         (1 << 2)		/* Comparator Interrupt enable bit */
#define ACMP_STATUS_BIT        (1 << 3)		/* Comparator status, reflects the state of the comparator output */
#define ACMP_COMPVMSEL_SHIFT   (4)          /* Shift for VM Input selection */
#define ACMP_COMPVMSEL_MASK    (0x7 << 4)   /* Mask for VM Input selection */
#define ACMP_COMPVPSEL_SHIFT   (8)          /* Shift for VP Input selection */
#define ACMP_COMPVPSEL_MASK    (0x7 << 8)	/* Mask for VP Input selection */
#define ACMP_HYSTERESIS_SHIFT  (13)	        /* Shiftfor Hysterisis Control */
#define ACMP_HYSTERESIS_MASK   (0x3 << 13)	/* Mask for Hysterisis Control */
#define ACMP_INTPOL_SHIFT      (15)		    /* Polarity shift of CMP output for interrupt 0 - Not Inverted, 1 - Inverted */
#define ACMP_INTPOL_BIT        (1 << 15)	/* Polarity of CMP output for interrupt 0 - Not Inverted, 1 - Inverted */
#define ACMP_INTTYPE_SHIFT     (16)		    /* Interrupt Type 0 - Edge, 1 - Level */
#define ACMP_INTTYPE_BIT       (1 << 16)	/* Interrupt Type 0 - Edge, 1 - Level */
#define ACMP_INTEDGE_SHIFT     (17)	        /* Mask for Interrupt edge selection */
#define ACMP_INTEDGE_MASK      (0x3 << 17)	/* Mask for Interrupt edge selection */
#define ACMP_INTFLAG_BIT       (1 << 19)	/* Interrupt Flag bit */
#define ACMP_LADENAB_BIT       (1 << 20)	/* Voltage ladder enable bit */
#define ACMP_LADREF_SHIFT      (22)		    /* Voltage reference select bit for voltage ladder */
#define ACMP_LADREF_BIT        (1 << 22)	/* Voltage reference select bit for voltage ladder */
#define ACMP_LADSEL_SHIFT      (24)	        /* Reference voltage selection mask for ladder */
#define ACMP_LADSEL_MASK       (0x1F << 24)	/* Reference voltage selection mask for ladder */
#define ACMP_PROPDLY_SHIFT     (29)	        /* Propogation delay shift */
#define ACMP_PROPDLY_MASK      (0x3 << 29)	/* Propogation delay mask */

/* Bit definitions for comparator filter register */
#define ACMP_SMODE_MASK        (0x3 << 0)   /* Mask for digital filter sample mode */
#define ACMP_CLKDIV_SHIFT      (2)	        /* Mask for comparator clock */
#define ACMP_CLKDIV_MASK       (0x7 << 2)	/* Mask for comparator clock */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/**
 * @brief Enum definition of ACMP channel
 */ 
typedef enum {
  #if (ENABLE == CFG_ACMP0_ENABLE)
  ACMP0        =  0,
  #endif 
  #if (ENABLE == CFG_ACMP1_ENABLE)
  ACMP1        =  1,
  #endif  
  #if (ENABLE == CFG_ACMP2_ENABLE)
  ACMP2        =  2,
  #endif
  #if (ENABLE == CFG_ACMP3_ENABLE)
  ACMP3        =  3,
  #endif  
} ACMP_NUM_T;

/**
 * @brief Analog Comparator channel register block structure
 */
typedef struct {
	__IO U32  CMP;			/*!< Individual Comparator control register */
	__IO U32  CMPFILTR;	    /*!< Individual Comparator Filter registers */
} CMP_REG_T;

/**
 * @brief Analog Comparator register block structure
 */
typedef struct {				                /*!< ACMP Structure */
	__IO U32  CTRL;		                        /*!< Comparator block control register */
	__IO CMP_REG_T ACMP[ACMP_MAX_NUMBER];		/*!< Individual Comparator registers */
} LPC_CMP_T;

/** Interrupt Polarity for comparator */
#define ACMP_INTPOL_NOTINVERTED    (0 << ACMP_INTPOL_SHIFT)   
#define ACMP_INTPOL_INVERTED       (1 << ACMP_INTPOL_SHIFT)   

/** Interrupt Level for comparator */
#define ACMP_INTTYPE_EDGE          (0 << ACMP_INTTYPE_SHIFT)
#define ACMP_INTTYPE_LEVEL         (1 << ACMP_INTTYPE_SHIFT)

/** Edge selection for comparator */
#define ACMP_EDGESEL_FALLING       (0 << ACMP_INTEDGE_SHIFT)
#define ACMP_EDGESEL_RISING        (1 << ACMP_INTTYPE_SHIFT)
#define ACMP_EDGESEL_BOTH          (2 << ACMP_INTEDGE_SHIFT)

/** Interrupt Level for comparator */
#define ACMP_LADREF_VDDCMP         (0 << ACMP_LADREF_SHIFT)
#define ACMP_LADREF_VDDA           (1 << ACMP_LADREF_SHIFT)

/** Hysteresis selection for comparator */
typedef enum {
	ACMP_HYS_NONE           =  (0 << ACMP_HYSTERESIS_SHIFT),	/* No hysteresis (the output will switch as the voltages cross) */
	ACMP_HYS_5MV            =  (1 << ACMP_HYSTERESIS_SHIFT),	/* 5mV hysteresis */
	ACMP_HYS_10MV           =  (2 << ACMP_HYSTERESIS_SHIFT),	/* 10mV hysteresis */
	ACMP_HYS_15MV           =  (3 << ACMP_HYSTERESIS_SHIFT)	    /* 20mV hysteresis */
} ACMP_HYS_T;

/**
 * Analog Comparator positive input values
 */
typedef enum ACMP_POS_INPUT {
	ACMP_POSIN_VREF_DIV  = (0 << ACMP_COMPVPSEL_SHIFT),	/*!< Voltage ladder output */
	ACMP_POSIN_ACMP_I1   = (1 << ACMP_COMPVPSEL_SHIFT),	/*!< ACMP_I1 pin */
	ACMP_POSIN_ACMP_I2   = (2 << ACMP_COMPVPSEL_SHIFT),	/*!< ACMP_I2 pin */
	ACMP_POSIN_ACMP_I3   = (3 << ACMP_COMPVPSEL_SHIFT),	/*!< ACMP_I3 pin */
	ACMP_POSIN_ACMP_I4   = (4 << ACMP_COMPVPSEL_SHIFT),	/*!< ACMP_I4 pin */
	ACMP_POSIN_INT_REF   = (5 << ACMP_COMPVPSEL_SHIFT),	/*!< Internal reference voltage */
	ACMP_POSIN_ADCIN_1   = (6 << ACMP_COMPVPSEL_SHIFT),	/*!< ADC Input or Temperature sensor varies with comparator */
	ACMP_POSIN_ADCIN_2   = (7 << ACMP_COMPVPSEL_SHIFT)	/*!< ADC Input varies with comparator */
} ACMP_POS_INPUT_T;

/**
 * Analog Comparator negative input values
 */
typedef enum ACMP_NEG_INPUT {
	ACMP_NEGIN_VREF_DIV  = (0 << ACMP_COMPVMSEL_SHIFT),	/*!< Voltage ladder output */
	ACMP_NEGIN_ACMP_I1   = (1 << ACMP_COMPVMSEL_SHIFT),	/*!< ACMP_I1 pin */
	ACMP_NEGIN_ACMP_I2   = (2 << ACMP_COMPVMSEL_SHIFT),	/*!< ACMP_I2 pin */
	ACMP_NEGIN_ACMP_I3   = (3 << ACMP_COMPVMSEL_SHIFT),	/*!< ACMP_I3 pin */
	ACMP_NEGIN_ACMP_I4   = (4 << ACMP_COMPVMSEL_SHIFT),	/*!< ACMP_I4 pin */
	ACMP_NEGIN_INT_REF   = (5 << ACMP_COMPVMSEL_SHIFT),	/*!< Internal reference voltage */
	ACMP_NEGIN_ADCIN_1   = (6 << ACMP_COMPVMSEL_SHIFT),	/*!< ADC Input or Temperature sensor varies with comparator */
	ACMP_NEGIN_ADCIN_2   = (7 << ACMP_COMPVMSEL_SHIFT)	/*!< ADC Input varies with comparator */
} ACMP_NEG_INPUT_T;

/**
 * Analog Comparator sample mode values
 */
typedef enum {
	ACMP_SMODE_0 = 0,	/*!< Bypass filter */
	ACMP_SMODE_BYPASS = ACMP_SMODE_0, 
	ACMP_SMODE_1,		/*!< Reject pulses shorter than 1 filter clock cycle */
	ACMP_SMODE_2,		/*!< Reject pulses shorter than 2 filter clock cycle */
	ACMP_SMODE_3		/*!< Reject pulses shorter than 3 filter clock cycle */
} ACMP_SMODE_T;

/**
 * Analog Comparator clock divider values
 */
typedef enum {
	ACMP_CLKDIV_1  =    (0 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK */
	ACMP_CLKDIV_2  =    (1 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK/2 */
	ACMP_CLKDIV_4  =    (2 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK/4 */
	ACMP_CLKDIV_8  =    (3 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK/8 */
	ACMP_CLKDIV_16 =    (4 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK/16 */
	ACMP_CLKDIV_32 =    (5 << ACMP_CLKDIV_SHIFT),	/*!< Use CMP_PCLK/32 */
	ACMP_CLKDIV_64 =    (6 << ACMP_CLKDIV_SHIFT)	/*!< Use CMP_PCLK/64 */
} ACMP_CLKDIV_T;

/**
 * Analog Comparator propagation delay
 */
typedef enum {
	ACMP_PROPDLY_0             =    (0 << ACMP_PROPDLY_SHIFT),	/* 0x00 delay */
	ACMP_PROPDLY_SHORTEST      =     ACMP_PROPDLY_0,            /* Shorest propagation delay */
	ACMP_PROPDLY_HIGHEST_PWR   =     ACMP_PROPDLY_0,	        /* Highest power consumption */
	ACMP_PROPDLY_1             =    (1 << ACMP_PROPDLY_SHIFT),	/* 0x01 delay */
	ACMP_PROPDLY_2             =    (2 << ACMP_PROPDLY_SHIFT),	/* 0x02 delay */
	ACMP_PROPDLY_LONGEST       =     ACMP_PROPDLY_2,            /* Longest propagation delay */
	ACMP_PROPDLY_LOWEST_PWR    =     ACMP_PROPDLY_2,	        /* Lowest power consumption */
} ACMP_PROPDLY_T;
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	Initializes and start the ACMP
 * @param	Nothing
 * @return	Nothing
 */
void ACMP_vInit(void);


/**
 * @brief	Shutdown the ACMP
 * @param	Nothing
 * @return	Nothing
 */
void ACMP_vShutdown(void);

/**
 * @brief	Returns the current comparator status
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return  Comparator status, reflects the state of the comparator output
 */
STATIC INLINE bool ACMP_bGetCompStatus(ACMP_NUM_T teCH)
{
	return (GET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_STATUS_BIT) != 0);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif/* CFG_ACMP_USED */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

