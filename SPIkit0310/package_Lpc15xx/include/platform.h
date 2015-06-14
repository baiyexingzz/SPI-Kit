/******************************************************************************/
/*                                                                            */
/*    $Workfile::   platform.h                                           $    */
/*                                                                            */
/*    $Revision:: 277                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 07 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-31 14:49:29 +0800 (周五, 31 十月 2014)       $    */
/*                                                                            */
/*      $Author:: simon.shi@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Generic definitions LPC15XX platform.                     */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef PLATFORM_H
#define PLATFORM_H

/* Include platform_cfg file for target processor definition */
#include <stdint.h>
#include <stdbool.h>

/* LPC15XX family */
#define LPC1517                   (0)
#define LPC1518                   (1)
#define LPC1519                   (2)
#define LPC1547                   (3)
#define LPC1548                   (4)
#define LPC1549                   (5)

/* LPC15XX chip package */
#define LQFP48                    (0)
#define LQFP64                    (1)
#define LQFP100                   (2)

/* Endianness of the processor */
/*lint -esym(755,__BIG_ENDIAN,__LITTLE_ENDIAN) */
#define      BIG_ENDIAN           (0)
#define      LITTLE_ENDIAN        (1)

#include "platform_cfg.h"
#include "cmsis.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/* Preprocessor directive for inline functions */
#ifdef __CC_ARM
#define INLINE  __inline
/*lint -esym(755,TARGET_INLINE) */
#define TARGET_INLINE __inline
#else
#define INLINE inline
#define TARGET_INLINE inline
#endif

/* Static data/function define */
#define STATIC static

/* External data/function define */
#define EXTERN extern

#ifndef CORE_M3
#error CORE_M3 is not defined for the LPC15xx architecture
#error CORE_M3 should be defined as part of your compiler define list
#endif

/* Preprocessor directive for interrupt service routine functions */
/*lint -esym(755,TARGET_INTERRUPT) */
#define TARGET_INTERRUPT __irq

/* Preprocessor directive for packed (no padding) structures and fields */
/*lint -esym(755,TARGET_PACKED) */
#define TARGET_PACKED __packed

/* Preprocessor directive for memory specifiers */
/*                 ST7     8051    ARM7 */
/* Direct Addr.:   @tiny   _dat         */
/* Indirect Addr.: @near   _idat        */
/* External:       @near   _xdat        */
/* Read only:      @near   _rom         */
/*lint -esym(755,TARGET_MEM_*) */
#define TARGET_MEM_DIA
#define TARGET_MEM_IDA
#define TARGET_MEM_EXT
#define TARGET_MEM_ROM

#if (TARGET_BYTE_ORDER) == (BIG_ENDIAN)
#elif (TARGET_BYTE_ORDER) == (LITTLE_ENDIAN)
#else
  #error "platform_cfg.h: TARGET_BYTE_ORDER: select one option out of BIG_ENDIAN or LITTLE_ENDIAN"
#endif

#if defined(CHIP_LPC15XX)
#if (LPC1517 == TARGET_PROCESSOR)
#include "platform_lpc15xx.h"
#else
#error TARGET_PROCESSOR is not supported yet!
#endif 
#else 
#error LPC15XX familiy is not defined!
#endif

#if (TARGET_ICPACKAGE) == (LQFP100)
#error LQFP100 is not supported yet!
#endif

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#undef  BIT
/* Set bit macro */
#define BIT(n) (1UL << (n))

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#undef  SBF
/* Set bit field macro */
#define SBF(f, v) ((v) << (f))

/* _BITMASK constructs a symbol with 'field_width' least significant
 * bits set.
 * e.g., _BITMASK(5) constructs '0x1F', _BITMASK(16) == 0xFFFF
 * The symbol is intended to be used to limit the bit field width
 * thusly:
 * <a_register> = (any_expression) & _BITMASK(x), where 0 < x <= 32.
 * If "any_expression" results in a value that is larger than can be
 * contained in 'x' bits, the bits above 'x - 1' are masked off.  When
 * used with the _SBF example above, the example would be written:
 * a_reg = ((_SBF(5,7) | _SBF(12,0xF)) & _BITMASK(16))
 * This ensures that the value written to a_reg is no wider than
 * 16 bits, and makes the code easier to read and understand.
 */
#undef   BITMASK
/* Bitmask creation macro */
#define  BITMASK(field_width) ( BIT(field_width) - 1)

/* NULL pointer */
#ifndef  NULL
#define  NULL ((void *) 0)
#endif

/* Number of elements in an array */
#define NELEMENTS(array)  (sizeof(array) / sizeof(array[0]))

#if !defined(ABS)
#define  ABS(a)   (((a) <0)   ?   -(a)   :(a)) 
#endif
#if !defined(MAX)
#define  MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#if !defined(MIN)
#define  MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#if !defined(RSHIFT)
#define  RSHIFT(a, bits) ((a) >> (bits))
#endif
#if !defined(LSHIFT)
#define  LSHIFT(a, bits) ((a) << (bits))
#endif

/* Conbine two U16 - H and L to a U32 */
#define  TOU32(H,L) ((U32)(LSHIFT(H,16))|(U32)(L))

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/* Old Type Definition compatibility */
/** @addtogroup LPC_Types_Public_Types
 * @{
 */

/** LPC type for character type */
typedef char        CHAR;

/** LPC type for 8 bit unsigned value */
typedef uint8_t     UNS_8;

/** LPC type for 8 bit signed value */
typedef int8_t      INT_8;

/** LPC type for 16 bit unsigned value */
typedef uint16_t    UNS_16;

/** LPC type for 16 bit signed value */
typedef int16_t     INT_16;

/** LPC type for 32 bit unsigned value */
typedef uint32_t    UNS_32;

/** LPC type for 32 bit signed value */
typedef int32_t     INT_32;

/** LPC type for 64 bit signed value */
typedef int64_t     INT_64;

/** LPC type for 64 bit unsigned value */
typedef uint64_t    UNS_64;

/*>RELAX<CodingConvention_20010_Naming> */
typedef bool        B1;
/*>RELAX<CodingConvention_20010_Naming> */
typedef uint8_t     U8;
/*>RELAX<CodingConvention_20010_Naming> */
typedef uint16_t    U16;
/*>RELAX<CodingConvention_20010_Naming> */
typedef uint32_t    U32;
/*>RELAX<CodingConvention_20010_Naming> */
typedef uint64_t    U64;
/*>RELAX<CodingConvention_20010_Naming> */
typedef int8_t      S8;
/*>RELAX<CodingConvention_20010_Naming> */
typedef int16_t     S16;
/*>RELAX<CodingConvention_20010_Naming> */
typedef int32_t     S32;
/*>RELAX<CodingConvention_20010_Naming> */
typedef float       F32;
/*>RELAX<CodingConvention_20010_Naming> */
typedef double      F64;
/*>RELAX<CodingConvention_20010_Naming> */

/**
 * @brief bool type definition
 */
typedef bool         BOOL;
#ifndef TRUE
#define TRUE         (!0)
#endif
#ifndef FALSE
#define FALSE        (0)
#endif

#ifndef ENABLE
#define ENABLE       TRUE
#endif
#ifndef DISABLE
#define DISABLE      FALSE
#endif

#define PRESENT      (1)  
#define UNPRESENT    (0)

/**
 * @brief Flag Status and Interrupt Flag Status type definition
 */
typedef enum {
    STAT_RESET = 0, 
    STAT_SET =   !0
} FlagStatus, IntStatus, SetState;
#define PARAM_SETSTATE(State) (((State) == SetState.STAT_RESET) || ((State) == SetState.STAT_SET))

/**
 * @brief Functional State Definition
 */
typedef enum {
    FS_DISABLE = ENABLE, 
    FS_ENABLE =  DISABLE
} FunctionalState;
#define PARAM_FUNCTIONALSTATE(State) (((State) == FunctionalState.FS_DISABLE) || ((State) == FunctionalState.FS_ENABLE))

/**
 * @ Status type definition
 */
typedef enum {
    ERROR = 0, 
    SUCCESS = !0
} Status;

/**
 * Read/Write transfer type mode (Block or non-block)
 */
typedef enum {
    NONE_BLOCKING = 0,		/**< None Blocking type */
    BLOCKING,				/**< Blocking type */
} TRANSFER_BLOCK_T;

/** Pointer to Function returning Void (any number of parameters) */
typedef void (*PFV)();

/** Pointer to general function as timer or other callback */
typedef void (*PCALLBACK)(void);

/** Pointer to Function returning int32_t (any number of parameters) */
typedef S32 (*PFI)();

/**
 *  Reg access macros
 */
/* Clear all bits of a regsiter as zero */
#define CLEAR_REG(REG)                               ((REG) = ((U32)0x0))
/* Write all bits of a regsiter as the expected values */
#define WRITE_REG(REG, VAL)                          ((REG) = ((U32)(VAL)))
/* Read all bits of a regsiter */
#define READ_REG(REG)                                (REG)

/* Write all bits of a half regsiter as the expected values */
#define WRITE_HALFREG(REG, VAL)                      ((REG) = ((U16)(VAL)))
/* Read all bits of a half regsiter */
#define READ_HALFREG(REG)                            ((U16)(REG))

/* Set a field of regsiter with a field-value */
#define SET_REG_FIELD(REG, MASK, FVAL)               ((REG) = (((REG)&(~(MASK)))|((FVAL)&(MASK))))
/* Set a field of half regsiter with a field-value */
#define SET_HALFREG_FIELD(REG, MASK, FVAL)           ((REG) = (((U16)(REG)&(~((U16)(MASK))))|((U16)(FVAL)&(U16)(MASK))))

/* Read and return a field-value of resgister */
#define GET_REG_FIELD(REG, MASK)                     ((REG)&(MASK))
/* Read and return a field-value of half resgister */
#define GET_HALFREG_FIELD(REG, MASK)                 ((U16)(((U16)(REG))&((U16)(MASK))))

/* Write a value into a field of regsiter*/
#define SET_REG_FIELD_VALUE(REG, MASK, SHIFT, VAL)   ((REG) = (((REG)&(~(MASK)))|(((VAL)<<(SHIFT))&(MASK))))  
/* Read a value from a field of regsiter*/
#define GET_REG_FIELD_VALUE(REG, MASK, SHIFT)        (((REG)&(MASK)) >> (SHIFT))
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

