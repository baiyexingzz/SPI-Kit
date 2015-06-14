/******************************************************************************/
/*                                                                            */
/*    $Workfile::   SCT.c                                            $   */
/*                                                                            */
/*    $Revision:: 286                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-04 09:59:57 +0800 (周二, 04 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - SCT/PWM         */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "platform.h"

#include "sct.h"

#ifdef CFG_SCT_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/** Pointer to User-Specific SCT receiving indicate callback  */
#define DECLARE_SCT_CALLBACK(_Func_)     extern void _Func_(void);

#define MATHCH_MODE(reg, n)    (SCT_REGMODE_MATCH(n) == (SCT_REGMODE_MASK(n)&(reg)))
#define CAPTURE_MODE(reg, n)   (SCT_REGMODE_CAPTRUE(n) == (SCT_REGMODE_MASK(n)&(reg)))
/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
#ifdef SCT0_CB
DECLARE_SCT_CALLBACK(SCT0_CB);
#endif

#ifdef SCT1_CB
DECLARE_SCT_CALLBACK(SCT1_CB);
#endif

#ifdef SCT2_CB
DECLARE_SCT_CALLBACK(SCT2_CB);
#endif

#ifdef SCT3_CB
DECLARE_SCT_CALLBACK(SCT3_CB);
#endif
/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* SCT Initialization */
void SCT_vInit(void)
{
     #if (ENABLE == CFG_SCT0_ENABLE)
	 CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT0);
	 SYSCTL_vPeriphReset(RESET_SCT0);
     #endif
	 
     #if (ENABLE == CFG_SCT1_ENABLE)
	 CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT1);
	 SYSCTL_vPeriphReset(RESET_SCT1);
     #endif	
	 
     #if (ENABLE == CFG_SCT2_ENABLE)
	 CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT2);
	 SYSCTL_vPeriphReset(RESET_SCT2);
     #endif	
	 
     #if (ENABLE == CFG_SCT3_ENABLE)
	 CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT3);
	 SYSCTL_vPeriphReset(RESET_SCT3);
     #endif		 
}

/* Config SCT */
void SCT_vConfig(SCT_GROUP_T teSCT, SCT_CONFIG_T *pTeConfig)
{
	U16 i = 0;	

    /* Config SCT global operation mode */
    SET_REG_FIELD(LPC_SCT[teSCT]->CONFIG, SCT_CFG_OP_MASK, pTeConfig->CFG_OP_MODE);

	/* Event configurations */
    for(i=0; i<CONFIG_SCT_nEVENT; i++){
	    LPC_SCT[teSCT]->EVENT[i].CTRL = pTeConfig->CFG_EVENT[i].CFG_CTRL;
		LPC_SCT[teSCT]->EVENT[i].STATE = pTeConfig->CFG_EVENT[i].CFG_STATE;
    }

	/* OUT configurations */
    WRITE_REG(LPC_SCT[teSCT]->OUTPUT , pTeConfig->CFG_OUTPUT);	
	for(i=0; i<CONFIG_SCT_nOUTPUT; i++){
        LPC_SCT[teSCT]->OUT[i].SET = pTeConfig->CFG_OUT[i].CFG_SET;
	    LPC_SCT[teSCT]->OUT[i].CLR = pTeConfig->CFG_OUT[i].CFG_CLR;
	}

    /* Match/Capture configurations */
    if(SCT_OP_UNIFIED == pTeConfig->CFG_OP_MODE){
		/* Unified SCT */
        WRITE_REG(LPC_SCT[teSCT]->LIMIT_U, pTeConfig->CFG_LIMIT_U);
		WRITE_REG(LPC_SCT[teSCT]->COUNT_U, 0);
		WRITE_REG(LPC_SCT[teSCT]->STATE_U, pTeConfig->CFG_STATE_U); 
		SET_REG_FIELD(LPC_SCT[teSCT]->CTRL_U, SCT_CTRL_BIDIR_MASK, pTeConfig->CFG_DIRSEL_U);

        for(i=0; i<CONFIG_SCT_nMATCAP; i++){
            if(MATHCH_MODE(pTeConfig->CFG_REGMODE_U, i)){
			    /* Match/Reload mode */
				if(0 != pTeConfig->CFG_MATCH[i].CFG_U){
					WRITE_REG(LPC_SCT[teSCT]->MATCH[i].U, pTeConfig->CFG_MATCH[i].CFG_U);
					WRITE_REG(LPC_SCT[teSCT]->MATCHREL[i].U, pTeConfig->CFG_MATCH[i].CFG_U);
				}
			}	
			else{
				/* Capture/Capture Control mode */
				if(0 != pTeConfig->CFG_CAPCTRL[i].CFG_U)
				{
				    WRITE_REG(LPC_SCT[teSCT]->CAPCTRL[i].U, pTeConfig->CFG_CAPCTRL[i].CFG_U);
				}
			}				
        }
	}
	else
	{ 
        /* Splitted SCT */
        WRITE_HALFREG(LPC_SCT[teSCT]->LIMIT_L , pTeConfig->CFG_LIMIT_L);
		WRITE_HALFREG(LPC_SCT[teSCT]->COUNT_L, 0);
		WRITE_HALFREG(LPC_SCT[teSCT]->STATE_L, pTeConfig->CFG_STATE_L);
		SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_L, SCT_CTRL_BIDIR_MASK, pTeConfig->CFG_DIRSEL_L);

        WRITE_HALFREG(LPC_SCT[teSCT]->LIMIT_H , pTeConfig->CFG_LIMIT_H);
		WRITE_HALFREG(LPC_SCT[teSCT]->COUNT_H, 0);
		WRITE_HALFREG(LPC_SCT[teSCT]->STATE_H, pTeConfig->CFG_STATE_H);
		SET_HALFREG_FIELD(LPC_SCT[teSCT]->CTRL_H, SCT_CTRL_BIDIR_MASK, pTeConfig->CFG_DIRSEL_H);

        for(i=0; i<CONFIG_SCT_nMATCAP; i++){
	   	    /* High Half SCT */
            if(MATHCH_MODE(pTeConfig->CFG_REGMODE_H, i)){
			    /* Match/Reload mode */
				if(0 != pTeConfig->CFG_MATCH[i].CFG_H){
					WRITE_HALFREG(LPC_SCT[teSCT]->MATCH[i].H, pTeConfig->CFG_MATCH[i].CFG_H);
					WRITE_HALFREG(LPC_SCT[teSCT]->MATCHREL[i].H, pTeConfig->CFG_MATCH[i].CFG_H);
				}				
			}	
			else{
				/* Capture/Capture Control mode */
				if(0 != pTeConfig->CFG_CAPCTRL[i].CFG_H)
				{
				    WRITE_HALFREG(LPC_SCT[teSCT]->CAPCTRL[i].H, pTeConfig->CFG_CAPCTRL[i].CFG_H);
				}				
			}

			/* Low Half SCT */
            if(MATHCH_MODE(pTeConfig->CFG_REGMODE_L, i)){
			    /* Match/Reload mode */
				if(0 != pTeConfig->CFG_MATCH[i].CFG_L){
					WRITE_HALFREG(LPC_SCT[teSCT]->MATCH[i].L, pTeConfig->CFG_MATCH[i].CFG_L);
					WRITE_HALFREG(LPC_SCT[teSCT]->MATCHREL[i].L, pTeConfig->CFG_MATCH[i].CFG_L);
				}				
			}	
			else{
				/* Capture/Capture Control mode */
				if(0 != pTeConfig->CFG_CAPCTRL[i].CFG_L)
				{
				    WRITE_HALFREG(LPC_SCT[teSCT]->CAPCTRL[i].L, pTeConfig->CFG_CAPCTRL[i].CFG_L);
				}
			}
        }
	}
}

/* SCT Initialization */
void SCT_vShutdown(void)
{
     #if (ENABLE == CFG_SCT0_ENABLE)
     CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT0);
	 SYSCTL_vPeriphReset(RESET_SCT0);
     #endif
	 
     #if (ENABLE == CFG_SCT1_ENABLE)
     CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT1);
	 SYSCTL_vPeriphReset(RESET_SCT1);
     #endif	

	 #if (ENABLE == CFG_SCT2_ENABLE)
     CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT2);
	 SYSCTL_vPeriphReset(RESET_SCT2);
     #endif	

	 #if (ENABLE == CFG_SCT3_ENABLE)
     CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_SCT3);
	 SYSCTL_vPeriphReset(RESET_SCT0);
     #endif		 
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                      ISR HANDLER FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for SCT0/1/2/3
 **/
void SCT0_vISR()
{
	#if(ENABLE == CFG_SCT0_ENABLE)
		/* ISR body */
		#ifdef SCT0_CB
		SCT0_CB();
		#endif
	#endif
}

void SCT1_vISR()
{
    #if(ENABLE == CFG_SCT1_ENABLE)	
		/* ISR body */
		#ifdef SCT1_CB
		SCT1_CB();
		#endif
	#endif
}

void SCT2_vISR()
{
	#if(ENABLE == CFG_SCT2_ENABLE)	
		/* ISR body */
		#ifdef SCT2_CB
		SCT2_CB();
		#endif
	#endif
}

void SCT3_vISR()
{
	#if(ENABLE == CFG_SCT3_ENABLE)
		/* ISR body */
		#ifdef SCT3_CB
		SCT3_CB();
		#endif
	#endif
}
#endif /* CFG_SCT_USED */

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

