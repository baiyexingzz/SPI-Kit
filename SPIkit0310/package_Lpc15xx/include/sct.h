/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SCT.h                                                  $ */
/*                                                                            */
/*    $Revision:: 294                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-11-05 12:20:36 +0800 (周三, 05 十一月 2014)    $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Low-level macros, definition, functions - SCT/PWM         */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef SCT_H
#define SCT_H

#include "platform.h"
#include "system.h"

#include "sct_cfg.h"

#ifdef CFG_SCT_USED

/** @defgroup SCT_15XX CHIP: LPC15xx SCT/PWM (Input Processing Unit) Registers and Driver
 * @ingroup 15XX_Drivers
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Numbere of SCT0/12/3 */
#define  MAX_SCT_NUMBER                 (4)
	
/* Number of events */
#define  CONFIG_SCT_nEVENT              (16)
/* Number of match/compare registers */
#define  CONFIG_SCT_nMATCAP             (16)
/* Number of fractional match registers */
#define  CONFIG_SCT_nFRACMAT            (6)
/* Number of outputs */
#define  CONFIG_SCT_nOUTPUT             (10) 

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define SCT_REG_HL(valh, vall)      ((U32)((((U16)valh) << 16)|((U16)vall)))
#define SCT_REG_U(val)              ((U16)(val))
#define SCT_REG_L(val)              ((U16)(val))
#define SCT_REG_H(val)              (((U16)val) << 16)

/* Bit definitions for SCT config register */
#define SCT_CFG_OP_SHIFT          (0)		    	/* SCT operation shift */
#define SCT_CFG_OP_MASK           (1 << 0)		    /* SCT operation mask */
#define SCT_CFG_OP_UNIFIED        (1 << 0)			/* SCT unified operation */
#define SCT_CFG_OP_SPLITTED       (0 << 0)			/* SCT splitted operation */
#define SCT_CFG_CLKMODE_SHIFT     (1)		    	/* SCT clock mode shift */
#define SCT_CFG_CLKMODE_0         (0 << 1)			/* SCT clock mode - system clock */
#define SCT_CFG_CLKMODE_1         (1 << 1)      	/* SCT clock mode - prescaled system clock */
#define SCT_CFG_CLKMODE_2         (2 << 1)			/* SCT clock mode - SCT input */
#define SCT_CFG_CLKMODE_3         (3 << 1)			/* SCT clock mode - prescaled SCT input */
#define SCT_CFG_CLKSEL_MASK       (0x00000078)   	/* SCT clock selection mask */
#define SCT_CFG_CLKSEL_SHIFT      (3)		    	/* SCT clock selection shift */
#define SCT_CFG_CLKSEL(n)         ((n) << 3)		/* SCT clock selection 0x0 - 0xF */
#define SCT_CFG_NORELAOD_L_SHIFT  (7)		        /* SCT prevents the lower match and fractional match registers from being reloaded */
#define SCT_CFG_NORELAOD_L        (1 << 7)          /* SCT prevents the lower match and fractional match registers from being reloaded */
#define SCT_CFG_RELAOD_L          (0 << 7)          /* SCT allows the lower match and fractional match registers to be reloaded */
#define SCT_CFG_NORELAOD_H_SHIFT  (8)		        /* SCT prevents the higher match and fractional match registers from being reloaded */
#define SCT_CFG_NORELAOD_H        (1 << 8)          /* SCT prevents the higher match and fractional match registers from being reloaded */
#define SCT_CFG_RELAOD_H          (0 << 8)          /* SCT allows the higher match and fractional match registers to be reloaded */
#define SCT_CFG_INSYNC_MASK       (0x0001FE00)      /* SCT Synchronization for input mask */
#define SCT_CFG_INSYNC_SHIFT      (9)		        /* SCT Synchronization for input shift */
#define SCT_CFG_INSYNC(n)         ((n) << 9)        /* SCT Synchronization for input n  */
#define SCT_CFG_AUTOLIMIT_L_SHIFT (17)		        /* SCT match on lower match register - a de-facto LIMIT condition without the need to define an associated event */
#define SCT_CFG_AUTOLIMIT_L       (1 << 17)         /* SCT match on lower match register - a de-facto LIMIT condition without the need to define an associated event */
#define SCT_CFG_EVENTLIMIT_L      (0 << 17)         /* SCT match on lower match register - a de-facto LIMIT condition with the need to define an associated event */
#define SCT_CFG_AUTOLIMIT_H_SHIFT (18)		        /* SCT match on higher match register - a de-facto LIMIT condition without the need to define an associated event */
#define SCT_CFG_AUTOLIMIT_H       (1 << 18)         /* SCT match on higher match register - a de-facto LIMIT condition without the need to define an associated event */
#define SCT_CFG_EVENTLIMIT_H      (0 << 18)         /* SCT match on higher match register - a de-facto LIMIT condition with the need to define an associated event */
#define SCT_CFG_MASK              (0x0007FFFF)   	/* SCT config register mask */

/* Bit definitions for SCT control register (UNIFIED or SPLITTED) */
#define SCT_CTRL_DOWN_SHIFT       (0)		    	/* SCT control shift - counting down */
#define SCT_CTRL_DOWN             (1 << 0)          /* SCT control - counting down */
#define SCT_CTRL_STOP_SHIFT       (1)		    	/* SCT control shift - stop */
#define SCT_CTRL_STOP             (1 << 1)          /* SCT control - stop */
#define SCT_CTRL_HALT_SHIFT       (2)		    	/* SCT control shift - halt */
#define SCT_CTRL_HALT             (1 << 2)          /* SCT control - halt */
#define SCT_CTRL_CLRCTR_SHIFT     (3)		    	/* SCT control shift - clear counter */
#define SCT_CTRL_CLRCTR           (1 << 3)          /* SCT control - clear counter */
#define SCT_CTRL_BIDIR_SHIFT      (4)		    	/* SCT control shift - bidirection */
#define SCT_CTRL_BIDIR_MASK       (1 << 4)          /* SCT control - mask */
#define SCT_CTRL_BIDIR            (1 << 4)          /* SCT control - bidirection */
#define SCT_CTRL_UPDIR            (0 << 4)          /* SCT control - one direction */
#define SCT_CTRL_PRE_SHIFT        (5)               /* SCT control shift - SCT clock prescaled factor */
#define SCT_CTRL_PRE_MASK         (0x1FE0)          /* SCT control mask - SCT clock prescaled factor */
#define SCT_CTRL_PRE(n)           ((n) << 5)        /* SCT control - SCT clock prescaled factor */
#define SCT_CTRL_MASK             (0x1FFF)   	    /* SCT control register mask */

/* Bit definitions for limit control register (UNIFIED or SPLITTED) */
#define SCT_LIMIT_MASK            (0xFFFF)   	    /* SCT limit register mask */
#define SCT_LIMIT(n)              (1 << (n))   	    /* SCT limit register mask */

/* Bit definitions for halt control register (UNIFIED or SPLITTED) */
#define SCT_HALT_MASK             (0xFFFF)   	    /* SCT halt register mask */
#define SCT_HALT(n)               (1 << (n))   	    /* SCT halt register mask */

/* Bit definitions for stop control register (UNIFIED or SPLITTED) */
#define SCT_STOP_MASK             (0xFFFF)   	    /* SCT stop register mask */
#define SCT_STOP(n)               (1 << (n))   	    /* SCT stop register mask */

/* Bit definitions for stop control register (UNIFIED or SPLITTED) */
#define SCT_START_MASK            (0xFFFF)   	    /* SCT start register mask */
#define SCT_START(n)              (1 << (n))   	    /* SCT start register mask */

/* Bit definitions for dither control register (UNIFIED or SPLITTED) */
#define SCT_DITHER_MASK           (0xFFFF)   	    /* SCT dither register mask */
#define SCT_DITHER(n)             (1 << (n))   	    /* SCT dither register mask */

/* Bit definitions for count register (UNIFIED or SPLITTED) */
#define SCT_COUNT_MASK            (0xFFFF)   	    /* SCT count register mask */

/* Bit definitions for state register (UNIFIED or SPLITTED) */
#define SCT_STATE_MASK            (0x001F)   	    /* SCT state register mask */

/* Bit definitions for input register */
#define SCT_INPUT_ASYNC_MASK      (0x000000FF)   	/* SCT async input register mask */
#define SCT_INPUT_ASYNC(n)        (1 << (n))        /* SCT async input register - input n */
#define SCT_INPUT_SYNC_MASK       (0x00FF0000)   	/* SCT sync input register mask */
#define SCT_INPUT_SYNC(n)         (1 << (n+16))     /* SCT sync input register - input n */

/* Bit definitions for regmode register (UNIFIED or SPLITTED) */
#define SCT_REGMODE_MASK(n)       (1 << (n))        /* SCT regmode register - match/capture mask n */
#define SCT_REGMODE_MATCH(n)      (0 << (n))        /* SCT regmode register - match n */
#define SCT_REGMODE_CAPTRUE(n)    (1 << (n))        /* SCT regmode register - capture n */

/* Bit definitions for output register */
#define SCT_ALLOUTPUT_MASK        (0x000003FF)      /* SCT output register mask */
#define SCT_OUTPUT_MASK(n)        (1 << (n))   	    /* SCT output register mask */
#define SCT_OUTPUT(n, val)        (val << (n))   	/* SCT output register mask */

/* Bit definitions for bidirectional output control register */
#define SCT_OUTPUTDIRCTRL_MASK(n) (0x3 << (2*n))    /* SCT bidirectional output control register mask */
#define SCT_OUTPUTDIRCTRL_NONE(n) (0x0 << (2*n))   	/* SCT bidirectional output control - Set and clear do not depend on any counte */
#define SCT_OUTPUTDIRCTRL_L(n)    (0x1 << (2*n))   	/* SCT bidirectional output control - Set and clear are reversed when counter L or the unified counter is counting down. */
#define SCT_OUTPUTDIRCTRL_H(n)    (0x2 << (2*n))   	/* SCT bidirectional output control - Set and clear are reversed when counter H is counting down. */

/* Bit definitions for conflict resolution register */
#define SCT_RES_MASK(n)           (0x3 << (2*n))    /* SCT conflict resolution register mask */
#define SCT_RES_NOCHANGE(n)       (0x0 << (2*n))   	/* SCT conflict resolution - No change */
#define SCT_RES_SET(n)            (0x1 << (2*n))   	/* SCT conflict resolution - Set output (or clear based on the SETCLRn field). */
#define SCT_RES_CLEAR(n)          (0x2 << (2*n))   	/* SCT conflict resolution - Clear output (or set based on the SETCLRn field). */
#define SCT_RES_TOGGLE(n)         (0x3 << (2*n))   	/* SCT conflict resolution - Toggle output. */

/* Bit definitions for DMA0 request register */
#define SCT_DMA0REQ_DEV0_MASK     (0xFFFF))         /* set DMA request 0 when it loads the Match_L/Unified registers */
#define SCT_DMA0REQ_DEV0(n)       (1 << (n))        /* set DMA request 0 when it loads the Match_L/Unified registers */
#define SCT_DMA0REQ_DRL_SHIFT     (30)              /* event n sets DMA request 0 - shift*/
#define SCT_DMA0REQ_DRL           (1 << 30)         /* event n sets DMA request 0 */
#define SCT_DMA0REQ_DRQ_SHIFT     (31)              /* This read-only bit indicates the state of DMA Request 0 - shift */
#define SCT_DMA0REQ_DRQ           (1 << 31)         /* This read-only bit indicates the state of DMA Request 0 */

/* Bit definitions for DMA1 request register */
#define SCT_DMA1REQ_DEV0_MASK     (0xFFFF))         /* set DMA request 1 when it loads the Match_L/Unified registers */
#define SCT_DMA1REQ_DEV0(n)       (1 << (n))        /* set DMA request 1 when it loads the Match_L/Unified registers */
#define SCT_DMA1REQ_DRL_SHIFT     (30)              /* event n sets DMA request 1 - shift*/
#define SCT_DMA1REQ_DRL           (1 << 30)         /* event n sets DMA request 1 */
#define SCT_DMA1REQ_DRQ_SHIFT     (31)              /* This read-only bit indicates the state of DMA Request 1 - shift */
#define SCT_DMA1REQ_DRQ           (1 << 31)         /* This read-only bit indicates the state of DMA Request 1 */

/* Bit definitions for event enable register */
#define SCT_EVEN_MASK             (0xFFFF))         /* Enables flags to request an interrupt if the FLAGn - mask */
#define SCT_EVEN(n)               (1 << (n))        /* Enables flags to request an interrupt if the FLAGn */

/* Bit definitions for event flag register */
#define SCT_EVFLAG_MASK           (0xFFFF))         /* Event flags to request an interrupt if the FLAGn - mask */
#define SCT_EVFLAG(n)             (1 << (n))        /* Event flags to request an interrupt if the FLAGn */

/* Bit definitions for conflict enable register */
#define SCT_CONEN_MASK            (0xFFFF))         /* Enables conflict flags to request an interrupt if the FLAGn - mask */
#define SCT_CONEN(n)              (1 << (n))        /* Enables conflict flags to request an interrupt if the FLAGn */

/* Bit definitions for conflict flag register */
#define SCT_CONFLAG_MASK          (0xFFFF))         /* Conflict flags to request an interrupt if the FLAGn - mask */
#define SCT_CONFLAG(n)            (1 << (n))        /* Conflict flags to request an interrupt if the FLAGn */

/* Bit definitions for match register (UNIFIED or SPLITTED) */
#define SCT_MATCH_MASK            (0xFFFF)   	    /* SCT match register mask */

/* Bit definitions for fractional match register (UNIFIED or SPLITTED) */
#define SCT_FRAMATCH_MASK         (0xF)   	        /* SCT fractional match register mask */

/* Bit definitions for capture register (UNIFIED or SPLITTED) */
#define SCT_CAP_MASK              (0xFFFF)   	    /* SCT capture register mask */

/* Bit definitions for match reload register (UNIFIED or SPLITTED) */
#define SCT_MATCHREL_MASK         (0xFFFF)   	    /* SCT match reload register mask */

/* Bit definitions for match reload register (UNIFIED or SPLITTED) */
#define SCT_FRACMATCHREL_MASK     (0xF)   	        /* SCT fractional match reload register mask */

/* Bit definitions for capture control register */
#define SCT_CAPCTRL_MASK          (0xFFFF)   	    /* SCT capture control register mask */
#define SCT_CAPCTRL(n)            (1 << (n))   	    /* SCT capture control register */

/* Bit definitions for event state mask register */
#define SCT_EVSTAT_MASK           (0xFFFF)   	    /* SCT event state mask register mask */
#define SCT_EVSTAT(n)             (1 << (n))   	    /* SCT event state mask register */

/* Bit definitions for event control register */
#define SCT_EVCTRL_MATCHSEL_SHIFT   (0)   	        /* Match register selection shift */
#define SCT_EVCTRL_MATCHSEL_MASK    (0xF)   	    /* Match register selection mask */
#define SCT_EVCTRL_MATCHSEL(n)      ((n) << 0)   	/* Match register selection */
#define SCT_EVCTRL_HEVENT_SHIFT     (4)   	        /* Select L/H counter shift */
#define SCT_EVCTRL_HEVENT_MASK      (0x10)   	    /* Select L/H counter mask */
#define SCT_EVCTRL_HEVENT_H         (1 << 4)   	    /* Select H counter */
#define SCT_EVCTRL_HEVENT_L         (0 << 4)   	    /* Select L counter */
#define SCT_EVCTRL_OUTSEL_SHIFT     (5)   	        /* Input/output select shift */
#define SCT_EVCTRL_OUTSEL_MASK      (0x20)   	    /* Input/output select mask */
#define SCT_EVCTRL_OUTSEL_OUT       (1 << 5)   	    /* Selects the output selected by IOSEL */
#define SCT_EVCTRL_OUTSEL_IN        (0 << 5)   	    /* Selects the input selected by IOSEL */
#define SCT_EVCTRL_IOSEL_SHIFT      (6)   	        /* Selects the input or output signal number associated with this event */
#define SCT_EVCTRL_IOSEL_MASK       (0x3C0)   	    /* Selects the input or output signal number associated with this event */
#define SCT_EVCTRL_IOSEL(n)         ((n) << 6)   	/* Selects the input or output signal number associated with this event */
#define SCT_EVCTRL_IOCON_SHIFT      (10)   	        /* Selects the I/O condition for event n. */
#define SCT_EVCTRL_IOCON_MASK       (0xC00)   	    /* Selects the I/O condition for event n. */
#define SCT_EVCTRL_IOCON_LOW        (0 << 10)   	/* LOW */
#define SCT_EVCTRL_IOCON_RISE       (1 << 10)   	/* RISE */
#define SCT_EVCTRL_IOCON_FALL       (2 << 10)   	/* FALL */
#define SCT_EVCTRL_IOCON_HIGH       (3 << 10)   	/* HIGH */
#define SCT_EVCTRL_COMBMODE_SHIFT   (12)   	        /* Selects how the specified match and I/O condition are used and combined */
#define SCT_EVCTRL_COMBMODE_MASK    (0x3000)   	    /* Selects how the specified match and I/O condition are used and combined */
#define SCT_EVCTRL_COMBMODE_OR      (0 << 12)   	/* OR The event occurs when either the specified match or I/O condition occurs */
#define SCT_EVCTRL_COMBMODE_MATCH   (1 << 12)   	/* MATCH Uses the specified match only */
#define SCT_EVCTRL_COMBMODE_IO      (2 << 12)   	/* IO Uses the specified I/O condition only */
#define SCT_EVCTRL_COMBMODE_AND     (3 << 12)   	/* AND The event occurs when the specified match and I/O condition occur simultaneously. */
#define SCT_EVCTRL_STATELD_SHIFT    (14)   	        /* This bit controls how the STATEV value modifies the state */
#define SCT_EVCTRL_STATELD_MASK     (0x4000)   	    /* This bit controls how the STATEV value modifies the state */
#define SCT_EVCTRL_STATELD_ADDED    (0 << 14)   	/* STATEV value is added into STATE (the carry-out is ignored). */
#define SCT_EVCTRL_STATELD_LOADED   (1 << 14)   	/* STATEV value is loaded into STATE */
#define SCT_EVCTRL_STATEV_SHIFT     (15)   	        /* This value is loaded into or added to the state selected by HEVENT */
#define SCT_EVCTRL_STATEV_MASK      (0xF8000)   	/* This value is loaded into or added to the state selected by HEVENT */
#define SCT_EVCTRL_STATEV(n)        ((n) << 15)  	/* Selects the input or output signal number associated with this event */
#define SCT_EVCTRL_MATCHMEM_SHIFT   (20)   	        /* If this bit is one and the COMBMODE field specifies a match component to the triggering of this event */
#define SCT_EVCTRL_MATCHMEM_MASK    (0x100000)   	/* If this bit is one and the COMBMODE field specifies a match component to the triggering of this event */
#define SCT_EVCTRL_MATCHMEM         (1 << 20)       /* If this bit is one and the COMBMODE field specifies a match component to the triggering of this event */
#define SCT_EVCTRL_DIRECTION_SHIFT  (21)   	        /* This bit controls how the STATEV value modifies the state */
#define SCT_EVCTRL_DIRECTION_MASK   (0x600000)   	/* This bit controls how the STATEV value modifies the state */
#define SCT_EVCTRL_DIRECTION_INDEP  (0 << 21)   	/* Direction independent. */
#define SCT_EVCTRL_DIRECTION_UP     (1 << 21)   	/* Counting up */
#define SCT_EVCTRL_DIRECTION_DOWN   (2 << 21)   	/* Counting down */
#define SCT_EVCTRL_MASK             (0x007FFFFF)   	/* SCT event control register mask */

/* Bit definitions for output set register */
#define SCT_OUTSET_MASK             (0xFFFF)   	    /* SCT output set register mask */
#define SCT_OUTSET(n)               (1 << (n))   	/* SCT output set register */

/* Bit definitions for output clear register */
#define SCT_OUTCLR_MASK             (0xFFFF)   	    /* SCT output clear register mask */
#define SCT_OUTCLR(n)               (1 << (n))   	/* SCT output clear register */

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
/* Logic numbered SCT0/12/3 */
typedef enum {
     #if (ENABLE == CFG_SCT0_ENABLE)
     SCT0       = 0,
     #endif
	 #if (ENABLE == CFG_SCT1_ENABLE)
     SCT1       = 1,
     #endif
	 #if (ENABLE == CFG_SCT2_ENABLE)
     SCT2       = 2,
     #endif	 
	 #if (ENABLE == CFG_SCT3_ENABLE)
     SCT3       = 3,
     #endif	 
}SCT_GROUP_T;

/* SCT operation mode for SCT - two 16-bit couters or one 32-bit counter */
typedef enum {
     SCT_OP_UNIFIED      = SCT_CFG_OP_UNIFIED,
	 SCT_OP_SPLITTED     = SCT_CFG_OP_SPLITTED,
}SCT_OP_T;

/* SCT counting direction mode */
typedef enum {
     SCT_BIDIR_SEL       = SCT_CTRL_BIDIR,
	 SCT_UPDIR_SEL       = SCT_CTRL_UPDIR,
}SCT_DIRSEL_T;

/* SCT init configuation structure for SCT */
/* Currently just a simple mapped setup structure, 
   which needs to be redeigned for flexible config target  */
typedef struct
{
    SCT_OP_T      CFG_OP_MODE;               /* Mapped to Operation Mode */

	union {                                  /* Mapped to Counter Direction Register */
	    SCT_DIRSEL_T CFG_DIRSEL_U;                              
        struct {
            SCT_DIRSEL_T CFG_DIRSEL_L;              
            SCT_DIRSEL_T CFG_DIRSEL_H;               
        };
    };
	
	union {                                /* Mapped to LIMIT register */
        U32 CFG_LIMIT_U;                  
        struct {
            U16 CFG_LIMIT_L;              
            U16 CFG_LIMIT_H;              
        };
    }; 

	union {
        U32 CFG_STATE_U;                  /* Mapped to STATE register */
        struct {
            U16 CFG_STATE_L;              
            U16 CFG_STATE_H;              
        };
    };

	union {
        U32 CFG_REGMODE_U;                 /* Mapped to REGMODE register */
        struct {
            U16 CFG_REGMODE_L;            
            U16 CFG_REGMODE_H;            
        };
    };

	U32 CFG_OUTPUT;                        /* Mapped to output register */

    union {
        union {                            /* Mapped to Match / Capture value */
            U32 CFG_U;                         
            struct {
                U16 CFG_L;                     
                U16 CFG_H;                    
            };
        } CFG_MATCH[CONFIG_SCT_nMATCAP];
    };

    union {                               /* Mapped to Capture Control value */
        union {
            U32 CFG_U;                 		  
            struct {
                U16 CFG_L;             		   
                U16 CFG_H;             		   
            };
        } CFG_CAPCTRL[CONFIG_SCT_nMATCAP];
    };

    struct {                               /* Mapped to SCTEVENT[i].STATE / SCTEVENT[i].CTRL*/
        U32 CFG_STATE;                     /* Mapped to Event State Register */
        U32 CFG_CTRL;                      /* Mapped to Event Control Register */
    } CFG_EVENT[CONFIG_SCT_nEVENT];

    struct {                               /* Mapped to SCTOUT[i].SET / SCTOUT[i].CLR */
        U32 CFG_SET;                       /* Mapped to Output n Set Register */
        U32 CFG_CLR;                       /* Mapped to Output n Clear Register */
    } CFG_OUT[CONFIG_SCT_nOUTPUT];	
} SCT_CONFIG_T;

/* SCT register map */
typedef struct
{
    __IO  U32 CONFIG;                      /* 0x000 Configuration Register */

	union {
        __IO U32 CTRL_U;                   /* 0x004 Control Register */
        struct {
            __IO U16 CTRL_L;               /* 0x004 low control register */
            __IO U16 CTRL_H;               /* 0x006 high control register */
        };
    };

	union {
        __IO U32 LIMIT_U;                  /* LIMIT register */
        struct {
            __IO U16 LIMIT_L;              /* 0x008 limit register for counter L */
            __IO U16 LIMIT_H;              /* 0x00A limit register for counter H */
        };
    };

	union {
        __IO U32 HALT_U;                   /* HALT register */
        struct {
            __IO U16 HALT_L;               /* 0x00C halt register for counter L */
            __IO U16 HALT_H;               /* 0x00E halt register for counter H */
        };
    };

	union {
        __IO U32 STOP_U;                  /* STOP register */
        struct {
            __IO U16 STOP_L;              /* 0x010 stop register for counter L */
            __IO U16 STOP_H;              /* 0x012 stop register for counter H */
        };
    };

	union {
        __IO U32 START_U;                  /* START register */
        struct {
            __IO U16 START_L;              /* 0x010 start register for counter L */
            __IO U16 START_H;              /* 0x012 start register for counter H */
        };
    };

	union {
        __IO U32 DITHER_U;                  /* DITHER register */
        struct {
            __IO U16 DITHER_L;              /* 0x018 dither register for counter L */
            __IO U16 DITHER_H;              /* 0x01A dither register for counter H */
        };
    };

    U32 RESERVED1[9];                 /* 0x01C-0x03C reserved */

	union {
        __IO U32 COUNT_U;                  /* 0x040 counter register */
        struct {
            __IO U16 COUNT_L;              /* 0x040 counter register for counter L */
            __IO U16 COUNT_H;              /* 0x042 counter register for counter H */
        };
    };
	
	union {
        __IO U32 STATE_U;                  /* STATE register */
        struct {
            __IO U16 STATE_L;              /* 0x044 state register for counter L */
            __IO U16 STATE_H;              /* 0x046 state register for counter H */
        };
    };

	__I  U32 INPUT;                        /* 0x048 input register */

	union {
        __IO U32 REGMODE_U;                /* REGMODE register */
        struct {
            __IO U16 REGMODE_L;            /* 0x04C regmode register for counter L */
            __IO U16 REGMODE_H;            /* 0x04E regmode register for counter H */
        };
    };

    __IO U32 OUTPUT;                       /* 0x050 output register */
    __IO U32 OUTPUTDIRCTRL;                /* 0x054 Output counter direction Control Register */
    __IO U32 RES;                          /* 0x058 conflict resolution register */
    __IO U32 DMA0REQUEST;                  /* 0x05C DMA0 Request Register */
    __IO U32 DMA1REQUEST;                  /* 0x060 DMA1 Request Register */
         U32 RESERVED2[35];                /* 0x064-0x0EC reserved */
    __IO U32 EVEN;                         /* 0x0F0 event enable register */
    __IO U32 EVFLAG;                       /* 0x0F4 event flag register */
    __IO U32 CONEN;                        /* 0x0F8 conflict enable register */
    __IO U32 CONFLAG;                      /* 0x0FC conflict flag register */

    union {
        __IO union {                       /* 0x100-... Match / Capture value */
            U32 U;                         /*       SCTMATCH[i].U  Unified 32-bit register */
            struct {
                U16 L;                     /*       SCTMATCH[i].L  Access to L value */
                U16 H;                     /*       SCTMATCH[i].H  Access to H value */
            };
        } MATCH[CONFIG_SCT_nMATCAP];
        __I union {
            U32 U;                         /*       SCTCAP[i].U  Unified 32-bit register */
            struct {
                U16 L;                     /*       SCTCAP[i].L  Access to H value */
                U16 H;                     /*       SCTCAP[i].H  Access to H value */
            };
        } CAP[CONFIG_SCT_nMATCAP];
    };

	union {                                /* 0x140-... FRAMatch value */
     __IO U32 U;                           /*       FRASCTMATCH[i].U  Unified 32-bit register */
        struct {
         __IO U16 L;                       /*       FRASCTFRAMATCH[i].L  Access to L value */
         __IO U16 H;                       /*       FRASCTFRAMATCH[i].H  Access to H value */
        };
    } FRAMATCH[CONFIG_SCT_nFRACMAT];
				
	U32 RESERVED3[42];   		
   		
    union {
        __IO union {                       /* 0x200-... Match Reload / Capture Control value */
            U32 U;                 		   /*       SCTMATCHREL[i].U  Unified 32-bit register */
            struct {
                U16 L;            		   /*       SCTMATCHREL[i].L  Access to L value */
                U16 H;             		   /*       SCTMATCHREL[i].H  Access to H value */
            };
        } MATCHREL[CONFIG_SCT_nMATCAP];
        __IO union {
            U32 U;                 		   /*       SCTCAPCTRL[i].U  Unified 32-bit register */
            struct {
                U16 L;             		   /*       SCTCAPCTRL[i].L  Access to H value */
                U16 H;             		   /*       SCTCAPCTRL[i].H  Access to H value */
            };
        } CAPCTRL[CONFIG_SCT_nMATCAP];
    };

	union {                    			   /* 0x240-... FRAMatch Reload value */
     __IO U32 U;                   		   /*       RELFRAMATCH[i].U  Unified 32-bit register */
        struct {
         __IO U16 L;               		   /*       RELFRAMATCH[i].L  Access to L value */
         __IO U16 H;                       /*       RELFRAMATCH[i].H  Access to H value */
        };  
    }RELFRAMATCH[CONFIG_SCT_nFRACMAT];
				
	U32 RESERVED6[42]; 	
		
    __IO struct {                          /* 0x300-0x3FC  SCTEVENT[i].STATE / SCTEVENT[i].CTRL*/
        U32 STATE;                         /* Event State Register */
        U32 CTRL;                          /* Event Control Register */
    } EVENT[CONFIG_SCT_nEVENT];

    U32 RESERVED9[128-2*CONFIG_SCT_nEVENT];   /* ...-0x4FC reserved */

    __IO struct {                          /* 0x500-0x57C  SCTOUT[i].SET / SCTOUT[i].CLR */
        U32 SET;                           /* Output n Set Register */
        U32 CLR;                           /* Output n Clear Register */
    } OUT[CONFIG_SCT_nOUTPUT];

    U32 RESERVED10[191-2*CONFIG_SCT_nOUTPUT];  /* ...-0x7F8 reserved */

    __I  U32 MODULECONTENT;                /* 0x7FC Module Content */
} LPC_SCT_T;

/******************************************************************************/
/*                          Private DATA DEFINITIONS                           */
/******************************************************************************/
/* Reg map table for SCT0/1/2/3 */
STATIC LPC_SCT_T* const LPC_SCT[MAX_SCT_NUMBER] = {
     LPC_SCT0,
     LPC_SCT1,
     LPC_SCT2,
     LPC_SCT3
};

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief SCT Initialization
 * @param	Nothing
 * @return	Nothing
 */
void SCT_vInit(void);

/**
 * @brief SCT Shutdown
 * @param	Nothing
 * @return	Nothing
 */
void SCT_vShutdown(void);

/**
 * @brief Config SCT
 * @param	teSCT : SCT group number
 * @param	pTeConfig: pointer to config structure 
 * @return	Nothing
 */
void SCT_vConfig(SCT_GROUP_T teSCT, SCT_CONFIG_T *pTeConfig);

/**
 * @brief Start SCT Low
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStartL(SCT_GROUP_T teSCT, U32 nInitOut, U32 nInitMask)
{
    /* Reset status */
	SET_REG_FIELD(LPC_SCT[teSCT]->OUTPUT, nInitMask, nInitOut);
	WRITE_HALFREG(LPC_SCT[teSCT]->COUNT_L, 0);
	
    /* Start */
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_L, SCT_CTRL_HALT, 0);
}

/**
 * @brief Start SCT High
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStartH(SCT_GROUP_T teSCT, U32 nInitOut, U32 nInitMask)
{
    /* Reset status */
	SET_REG_FIELD(LPC_SCT[teSCT]->OUTPUT, nInitMask, nInitOut);
	WRITE_HALFREG(LPC_SCT[teSCT]->COUNT_H, 0);
	
    /* Start */
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_H, SCT_CTRL_HALT, 0);
}

/**
 * @brief Start SCT
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStart(SCT_GROUP_T teSCT, U32 nInitOut, U32 nInitMask)
{
    /* Reset status */
	SET_REG_FIELD(LPC_SCT[teSCT]->OUTPUT, nInitMask, nInitOut);
	WRITE_REG(LPC_SCT[teSCT]->COUNT_U, 0);
	
    /* Start */
	SET_REG_FIELD(LPC_SCT[teSCT]->CTRL_U, SCT_CTRL_HALT, 0);
}

/**
 * @brief Stop(halt) SCT Low
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStopL(SCT_GROUP_T teSCT)
{
	/* Halt sct */
    SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_L, SCT_CTRL_HALT, SCT_CTRL_HALT);

    /* Reset status */
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->COUNT_L, SCT_COUNT_MASK, 0);	
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->STATE_L, SCT_STATE_MASK, 0);	
}

/**
 * @brief Stop(halt) SCT High
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStopH(SCT_GROUP_T teSCT)
{	
	/* Halt sct */
    SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_H, SCT_CTRL_HALT, SCT_CTRL_HALT);

    /* Reset status */
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->COUNT_H, SCT_COUNT_MASK, 0);	
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->STATE_H, SCT_STATE_MASK, 0);	
}

/**
 * @brief Stop(halt) SCT
 * @param	teSCT : SCT group number
 * @return	Nothing
 */
STATIC INLINE void SCT_vStop(SCT_GROUP_T teSCT)
{
	/* Halt sct */
    SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_U, SCT_CTRL_HALT, SCT_CTRL_HALT);

    /* Reset status */
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->COUNT_U, SCT_COUNT_MASK, 0);	
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->STATE_U, SCT_STATE_MASK, 0);	
}

/**
 * @brief SCT Shutdown
 * @param	teSCT : SCT group number
 * @param	nCH : Output channel
 * @param	nL :  16-bit Relad Match low half of value
 * @return	Nothing
 */
STATIC INLINE void SCT_vReloadMatchL(SCT_GROUP_T teSCT, U32 nCH, U16 nL)
{
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCH[nCH].L, SCT_MATCH_MASK, nL);
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCHREL[nCH].L, SCT_MATCHREL_MASK, nL);
}

/**
 * @brief SCT Shutdown
 * @param	teSCT : SCT group number
 * @param	nCH : Output channel
 * @param	nH :  16-bit Relad Match high half of value
 * @return	Nothing
 */
STATIC INLINE void SCT_vReloadMatchH(SCT_GROUP_T teSCT, U32 nCH, U16 nH)
{
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCH[nCH].H, SCT_MATCH_MASK, nH);
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCHREL[nCH].H, SCT_MATCHREL_MASK, nH);
}

/**
 * @brief SCT Shutdown
 * @param	teSCT : SCT group number
 * @param	nCH : Output channel
 * @param	nVal : 32-bit Relad Match value
 * @return	Nothing
 */
STATIC INLINE void SCT_vReloadMatch(SCT_GROUP_T teSCT, U32 nCH, U32 nVal)
{
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCH[nCH].U, SCT_MATCH_MASK, nVal);
	SET_HALFREG_FIELD(LPC_SCT[teSCT]->MATCHREL[nCH].U, SCT_MATCHREL_MASK, nVal);
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif/* CFG_SCT_USED */

#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/


