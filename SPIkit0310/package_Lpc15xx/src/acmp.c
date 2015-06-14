/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ACMP.c                                                 $  */
/*                                                                            */
/*    $Revision:: 308                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-07 14:25:58 +0800 (周五, 07 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - ACMP controller */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "acmp.h"

#ifdef CFG_ACMP_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Default Ring Oscillator Control Settings and Reset Source */
/* If undefined, it means that MCU reset values are used */
#define ACMP_ROSCCTL_DEFAULT             ACMP_ROSCCTL_SET0_RST1                
#define ACMP_ROSCRST_DEFAULT             ACMP_EXTRESET_INT     

/* Default Comparator Filter Settings - Sample Mode and Divider Clock */
/* If undefined, it means MCU reset default values are used */
#define ACMP_FILTER_SMODE_DEFAULT        ACMP_SMODE_BYPASS 
#define ACMP_FILTER_CLKDIV_DEFAULT       ACMP_CLKDIV_1 

/* Default Propagation Delay - Sample Mode and Divider Clock */
#define ACMP_PROPDLY_DEFAULT             ACMP_PROPDLY_SHORTEST

/* Maximum ACMP reference voltage */
#define ACMP_LADSEL_MAX                  (ACMP_LADSEL_MASK>>ACMP_LADSEL_SHIFT)

#if (ENABLE == CFG_ACMP0_ENABLE)
#if (ACMP0_LADSEL > ACMP_LADSEL_MAX)
  #error "Error: ACMP0 reference voltage cannot be bigger than ACMP_LADSEL_MAX - 0x1F"
#endif
#endif
#if (ENABLE == CFG_ACMP1_ENABLE)
#if (ACMP1_LADSEL > ACMP_LADSEL_MAX)
  #error "Error: ACMP1 reference voltage cannot be bigger than ACMP_LADSEL_MAX - 0x1F"
#endif
#endif
#if (ENABLE == CFG_ACMP2_ENABLE)
#if (ACMP2_LADSEL > ACMP_LADSEL_MAX)
  #error "Error: ACMP2 reference voltage cannot be bigger than ACMP_LADSEL_MAX - 0x1F"
#endif
#endif
#if (ENABLE == CFG_ACMP3_ENABLE)
#if (ACMP3_LADSEL > ACMP_LADSEL_MAX)
  #error "Error: ACMP3 reference voltage cannot be bigger than ACMP_LADSEL_MAX - 0x1F"
#endif
#endif

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/* Declare macro for callback */
#define DECLARE_ACMP_CALLBACK(_Func_)  extern void _Func_(void);

/* ACMP setup macro  including voltage settings,, intrrupt and voltage ladder */
#define ACMP_CFG_SETUP_VOLTREFS(ch)    ACMP_vSetupACMPRefs(ch, ch##_POS_INPUT, ch##_NEG_INPUT, ch##_HYS)
#define ACMP_CFG_SETUP_INTEDGE(ch)     ACMP_vSetupACMPEdgeInt(ch, ch##_INT_EDGE)
#define ACMP_CFG_SETUP_INTLEVEL(ch)    ACMP_vSetupACMPLevelInt(ch, ch##_INT_POL)
#define ACMP_CFG_SETUP_VOLTLADDER(ch)  ACMP_vSetupVoltLadder(ch, ch##_LADSEL, ch##_LADREF)

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/
/* Declare ACMP callbacks if exist */
#if (ENABLE == CFG_ACMP0_ENABLE)
#ifdef ACMP0_CB
DECLARE_ACMP_CALLBACK(ACMP0_CB);
#endif
#endif
#if (ENABLE == CFG_ACMP1_ENABLE)
#ifdef ACMP1_CB
DECLARE_ACMP_CALLBACK(ACMP1_CB);
#endif
#endif
#if (ENABLE == CFG_ACMP2_ENABLE)
#ifdef ACMP2_CB
DECLARE_ACMP_CALLBACK(ACMP2_CB);
#endif
#endif
#if (ENABLE == CFG_ACMP3_ENABLE)
#ifdef ACMP3_CB
DECLARE_ACMP_CALLBACK(ACMP3_CB);
#endif
#endif
/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
/**
 * @brief	ACMP Enable configuration
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE U32 ACMP_nPowerupConfig(void)
{
	U32 temp = 0;
	
    #if (ENABLE == CFG_ACMP0_ENABLE)
	temp |= SYSCTL_POWERDOWN_ACMP0_PD;
	#endif
    #if (ENABLE == CFG_ACMP1_ENABLE)
	temp |= SYSCTL_POWERDOWN_ACMP1_PD;
	#endif	
    #if (ENABLE == CFG_ACMP2_ENABLE)
	temp |= SYSCTL_POWERDOWN_ACMP2_PD;
	#endif		
    #if (ENABLE == CFG_ACMP3_ENABLE)
	temp |= SYSCTL_POWERDOWN_ACMP3_PD;
	#endif	

	return temp;
}

/**
 * @brief	Get the ACMP interrupt flag bit(INTFLAG bit)
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	TRUE if ACMP_INTFLAG_BIT is set else returns FALSE
 */
STATIC INLINE bool ACMP_bGetIntStatus(ACMP_NUM_T teCH)
{
	return (GET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_INTFLAG_BIT) != 0);
}

/**
 * @brief	Clears the ACMP interrupt flag bit (INTFLAG bit)
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vClearIntStatus(ACMP_NUM_T teCH)
{
	/* Writing a 1 to this bit clears the flag */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_INTFLAG_BIT, ACMP_INTFLAG_BIT);
}

#ifdef CFG_ACMP_INT_USED
/**
 * @brief	Enable the interrupt for the comparator
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vEnableCompInt(ACMP_NUM_T teCH)
{
	/* Make sure interrupt flag is not set during read write operation */
	ACMP_vClearIntStatus(teCH);
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_INTEN_BIT, ACMP_INTEN_BIT);
}

#else
/**
 * @brief	Disable the interrupt for the comparator
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vDisableCompInt(ACMP_NUM_T teCH)
{
	/* Make sure interrupt flag is not set during read write operation */
	ACMP_vClearIntStatus(teCH);	
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_INTEN_BIT, 0);	
}
#endif

/**
 * @brief	Disable all ACMP Ints used (configured) 
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vDisableUsedACMPInt(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
	#ifdef ACMP0_INT_TYPE
    ACMP_vDisableCompInt(ACMP0);
	#endif
    #endif
	#if (ENABLE == CFG_ACMP1_ENABLE)
	#ifdef ACMP1_INT_TYPE
	ACMP_vDisableCompInt(ACMP1);
	#endif
	#endif
    #if (ENABLE == CFG_ACMP2_ENABLE)
	#ifdef ACMP2_INT_TYPE
	ACMP_vDisableCompInt(ACMP2);
	#endif
	#endif
    #if (ENABLE == CFG_ACMP3_ENABLE)
	#ifdef ACMP3_INT_TYPE
	ACMP_vDisableCompInt(ACMP3);
	#endif
	#endif
}

/**
 * @brief	Disable all ACMP Ints used (configured)
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vEnableUsedACMPInt(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
		#ifdef ACMP0_INT_TYPE
		ACMP_vEnableCompInt(ACMP0);
		#else
		ACMP_vDisableCompInt(ACMP0);
		#endif
	#endif
	
    #if (ENABLE == CFG_ACMP1_ENABLE)
		#ifdef ACMP1_INT_TYPE
		ACMP_vEnableCompInt(ACMP1);
		#else
		ACMP_vDisableCompInt(ACMP1);
		#endif
	#endif
	
    #if (ENABLE == CFG_ACMP2_ENABLE)
		#ifdef ACMP2_INT_TYPE
		ACMP_vEnableCompInt(ACMP2);
		#else
		ACMP_vDisableCompInt(ACMP2);
		#endif
	#endif
	
    #if (ENABLE == CFG_ACMP3_ENABLE)
		#ifdef ACMP3_INT_TYPE
		ACMP_vEnableCompInt(ACMP3);
		#else
		ACMP_vDisableCompInt(ACMP3);
		#endif
	#endif
}

/**
 * @brief	Enable the comparator
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vEnableComp(ACMP_NUM_T teCH)
{
	/* Make sure interrupt flag is not set during read write operation */
	ACMP_vClearIntStatus(teCH);
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_CMPEN_BIT, ACMP_CMPEN_BIT);
}

/**
 * @brief	Disable the comparator
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vDisableComp(ACMP_NUM_T teCH)
{
	/* Make sure interrupt flag is not set during read write operation */
	ACMP_vClearIntStatus(teCH);
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP,  ACMP_CMPEN_BIT, 0);
}

/**
 * @brief	Clear the current comparator status as zero 
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @return  Nothing
 */
STATIC INLINE void ACMP_vClearCompStatus(ACMP_NUM_T teCH)
{
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_STATUS_BIT, 0);
}

/**
 * @brief	Disable all ACMPs used 
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vDisableUsedACMP(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
	ACMP_vDisableComp(ACMP0);
	#endif
    #if (ENABLE == CFG_ACMP1_ENABLE)
	ACMP_vDisableComp(ACMP1);
	#endif	
    #if (ENABLE == CFG_ACMP2_ENABLE)
	ACMP_vDisableComp(ACMP2);
	#endif		
    #if (ENABLE == CFG_ACMP3_ENABLE)
	ACMP_vDisableComp(ACMP3);
	#endif	
}

/**
 * @brief	Enable all ACMPs used 
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vEnableUsedACMP(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)	
	ACMP_vEnableComp(ACMP0);
	ACMP_vClearCompStatus(ACMP0);
	#endif
    #if (ENABLE == CFG_ACMP1_ENABLE)
	ACMP_vClearCompStatus(ACMP1);
	ACMP_vEnableComp(ACMP1);
	#endif	
    #if (ENABLE == CFG_ACMP2_ENABLE)
	ACMP_vClearCompStatus(ACMP2);
	ACMP_vEnableComp(ACMP2);
	#endif		
    #if (ENABLE == CFG_ACMP3_ENABLE)
	ACMP_vClearCompStatus(ACMP3);
	ACMP_vEnableComp(ACMP3);
	#endif	
}

/**
 * @brief	Setting up ACMP voltage settings 
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	unPosinput	: enum value of ACMP_POS_INPUT_T
 * @param	unNeginput	: enum value of ACMP_NEG_INPUT
 * @param	unHys	: enum value of ACMP_HYS_T
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetupACMPRefs(ACMP_NUM_T       teCH,
                                       U32              unPosinput,
					                   U32              unNeginput, 
					                   U32              unHys)
{
	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);

    /* ACMP voltage settings */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, 
		          (ACMP_COMPVMSEL_MASK | ACMP_COMPVPSEL_MASK | ACMP_HYSTERESIS_MASK),  
		          (unPosinput|unNeginput|unHys));
}

#ifdef CFG_ACMP_INTEDGE_USED
/**
 * @brief	Setting up ACMP edge int settings 
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	unEdgeSel	: ACMP_EDGESEL_FALLING, ACMP_EDGESEL_RISING or ACMP_EDGESEL_BOTH
 * @return	Nothing
 */
void STATIC INLINE ACMP_vSetupACMPEdgeInt(ACMP_NUM_T     teCH,
					                      U32            unEdgeSel)
{
	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);

	/* For Level triggered interrupt, invert sets the polarity
	     For edge triggered interrupt edgeSel sets the edge type */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, 
		          (ACMP_INTTYPE_BIT|ACMP_INTEDGE_MASK),
		          (ACMP_INTTYPE_EDGE|unEdgeSel));
}
#endif

#ifdef CFG_ACMP_INTLEVEL_USED 
/**
 * @brief	Setting up ACMP level int settings 
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	unPol	: ACMP_INTPOL_NOTINVERTED or ACMP_INTPOL_NOTINVERTED
 * @return	Nothing
 */
void STATIC INLINE ACMP_vSetupACMPLevelInt(ACMP_NUM_T     teCH,
					                       U32            unPol)
{
	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);

	/* For Level triggered interrupt, invert sets the polarity
	   For edge triggered interrupt   edgeSel sets the edge type */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, 
		          (ACMP_INTTYPE_BIT|ACMP_INTPOL_BIT),
		          (ACMP_INTTYPE_LEVEL|unPol));
}
#endif

/**
 * @brief	Set clock divider and mode of Comparator filter
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	teDiv	: SysClk divider enum value
 * @param	teMode	: sample mode enum value
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetCompFilt(ACMP_NUM_T     teCH,                                      
                                     ACMP_SMODE_T   teMode,
                                     ACMP_CLKDIV_T  teDiv)
{
	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);
	
    SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMPFILTR, 
		          ACMP_CLKDIV_MASK, 
		          teDiv);
	
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMPFILTR, 
		          ACMP_SMODE_MASK, 
		          teMode);
}

/**
 * @brief	Enables or disables voltage ladder
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	bEnable	: enable flag
 * @return	Nothing
 */
STATIC INLINE void ACMP_vEnableVoltLadder(ACMP_NUM_T     teCH,
                                          bool           bEnable)
{
	U32  temp = ((bEnable)? ACMP_LADENAB_BIT : 0);

	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);
	
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, 
		          ACMP_LADENAB_BIT, 
		          temp);
}

#ifdef CFG_ACMP_LADREF_USED
/**
 * @brief	Setting up voltage ladder 
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	unLadSel	: ACMP_LADREF_BIT or ~ACMP_LADREF_BIT
 * @param	unLadRef	: Voltage Ladder Seletion (0x00~0x1F)
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetupVoltLadder(ACMP_NUM_T    teCH, 
                                         U32           unLadSel, 
                                         U32           unLadRef)
{
	/* Make sure interrupt flag is not set during read OR/AND and write operation */
    ACMP_vClearIntStatus(teCH);
	/* Setup voltage ladder and ladder reference */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, 
		          (ACMP_LADREF_BIT | ACMP_LADSEL_MASK), 
		          (unLadRef|(unLadSel << ACMP_LADSEL_SHIFT)));
}
#endif

/**
 * @brief	Set propogation delay for comparator output
 * @param	teCH	: index to the comparator (ACMP0 - 3)
 * @param	ubDelay	: propogation delay (0 - 2), 0 is short delay more power consumption
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetPropDelay(ACMP_NUM_T     teCH,
                                      ACMP_PROPDLY_T teDelay)
{
	/* Make sure interrupt flag is not set during read write operation */
	SET_REG_FIELD(LPC_CMP->ACMP[teCH].CMP, ACMP_PROPDLY_MASK, teDelay);	
}

/**
 * @brief	Set Ring Oscillator control bit, ROSC output is set by ACMP1 and reset by ACMP0
 * @param	unCtrl	: ACMP_ROSCCTL_SET1_RST0 or ACMP_ROSCCTL_SET0_RST1
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetRingOscCtl(U32 unCtrl)
{
	SET_REG_FIELD(LPC_CMP->CTRL, ACMP_ROSCCTL_BIT, unCtrl);		
}

/**
 * @brief	Set Ring Oscillator Reset Source
 * @param	unRst	: ACMP_EXTRESET_EXT or ACMP_EXTRESET_INT
 * @return	Nothing
 */
STATIC INLINE void ACMP_vSetRingOscRstSrc(U32 unRst)
{
	SET_REG_FIELD(LPC_CMP->CTRL, ACMP_EXTRESET_BIT, unRst);	
}

/******************************************************************************/
/*                      ACMP SETUP FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/**
 *   ACMP config setup inline functions
 */
/**
 * @brief	Setting up voltage reference according to acmp_cfg
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupVoltRefs(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
    ACMP_CFG_SETUP_VOLTREFS(ACMP0);
    #endif

    #if (ENABLE == CFG_ACMP1_ENABLE)
    ACMP_CFG_SETUP_VOLTREFS(ACMP1);
    #endif

    #if (ENABLE == CFG_ACMP2_ENABLE)
    ACMP_CFG_SETUP_VOLTREFS(ACMP2);
    #endif

    #if (ENABLE == CFG_ACMP3_ENABLE)
    ACMP_CFG_SETUP_VOLTREFS(ACMP3);
    #endif
}

/**
 * @brief	Setting up acmp inputs according to acmp_cfg
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupInt(void)
{
    #if(ENABLE == CFG_ACMP0_ENABLE)
	#ifdef ACMP0_INT_TYPE
	   #if(ACMP0_INT_TYPE == ACMP_INTTYPE_EDGE)
           ACMP_CFG_SETUP_INTEDGE(ACMP0);
	   #elif(ACMP0_INT_TYPE == ACMP_INTTYPE_LEVEL)
	       ACMP_CFG_SETUP_INTLEVEL(ACMP0);
	   #else
           #error "Error: ACMP0 INT Type should be ACMP_INTTYPE_LEVEL or ACMP_INTTYPE_EDGE"
	   #endif
	#endif
	#endif

    #if (ENABLE == CFG_ACMP1_ENABLE)
	#ifdef ACMP1_INT_TYPE
	   #if(ACMP1_INT_TYPE == ACMP_INTTYPE_EDGE)
           ACMP_CFG_SETUP_INTEDGE(ACMP1);
	   #elif(ACMP1_INT_TYPE == ACMP_INTTYPE_LEVEL)
	       ACMP_CFG_SETUP_INTLEVEL(ACMP1);
	   #else
           #error "Error: ACMP1 INT Type should be ACMP_INTTYPE_LEVEL or ACMP_INTTYPE_EDGE"
	   #endif
	#endif   
	#endif

    #if (ENABLE == CFG_ACMP2_ENABLE)
	#ifdef ACMP2_INT_TYPE
	   #if(ACMP2_INT_TYPE == ACMP_INTTYPE_EDGE)
           ACMP_CFG_SETUP_INTEDGE(ACMP2);
	   #elif(ACMP2_INT_TYPE == ACMP_INTTYPE_LEVEL)
	       ACMP_CFG_SETUP_INTLEVEL(ACMP2);
	   #else
           #error "Error: ACMP2 INT Type should be ACMP_INTTYPE_LEVEL or ACMP_INTTYPE_EDGE"
	   #endif
	#endif   
	#endif

    #if (ENABLE == CFG_ACMP3_ENABLE)
	#ifdef ACMP3_INT_TYPE
	   #if(ACMP3_INT_TYPE == ACMP_INTTYPE_EDGE)
           ACMP_CFG_SETUP_INTEDGE(ACMP3);
	   #elif(ACMP3_INT_TYPE == ACMP_INTTYPE_LEVEL)
	       ACMP_CFG_SETUP_INTLEVEL(ACMP3);
	   #else
           #error "Error: ACMP3 INT Type should be ACMP_INTTYPE_LEVEL or ACMP_INTTYPE_EDGE"
	   #endif
	#endif   
	#endif
}

/**
 * @brief	Setting up voltage ladder according to acmp_cfg
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupVoltLadder(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
		#ifdef ACMP0_LADREF
			ACMP_vEnableVoltLadder(ACMP0, true);
		    ACMP_CFG_SETUP_VOLTLADDER(ACMP0);
		#else
			ACMP_vEnableVoltLadder(ACMP0, false);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP1_ENABLE)
        #ifdef ACMP1_LADREF		
			ACMP_vEnableVoltLadder(ACMP1, true);
		    ACMP_CFG_SETUP_VOLTLADDER(ACMP1);
		#else
			ACMP_vEnableVoltLadder(ACMP1, false);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP2_ENABLE)
        #ifdef ACMP2_LADREF		
			ACMP_vEnableVoltLadder(ACMP2, true);
		    ACMP_CFG_SETUP_VOLTLADDER(ACMP2);
		#else
			ACMP_vEnableVoltLadder(ACMP2, false);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP3_ENABLE)
        #ifdef ACMP3_LADREF		
			ACMP_vEnableVoltLadder(ACMP3, true);
		    ACMP_CFG_SETUP_VOLTLADDER(ACMP3);
		#else
			ACMP_vEnableVoltLadder(ACMP3, false);
		#endif
    #endif
}

/**
 * @brief	Setting up voltage comparator filter
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupFilter(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
		#ifdef ACMP_FILTER_CLKDIV_DEFAULT
			ACMP_vSetCompFilt(ACMP0, ACMP_FILTER_SMODE_DEFAULT, ACMP_FILTER_CLKDIV_DEFAULT);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP1_ENABLE)
	    #ifdef ACMP_FILTER_CLKDIV_DEFAULT
	    	ACMP_vSetCompFilt(ACMP1, ACMP_FILTER_SMODE_DEFAULT, ACMP_FILTER_CLKDIV_DEFAULT);
        #endif
	#endif

    #if (ENABLE == CFG_ACMP2_ENABLE)
	    #ifdef ACMP_FILTER_CLKDIV_DEFAULT
        	ACMP_vSetCompFilt(ACMP2, ACMP_FILTER_SMODE_DEFAULT, ACMP_FILTER_CLKDIV_DEFAULT);
        #endif
	#endif

    #if (ENABLE == CFG_ACMP3_ENABLE)
	    #ifdef ACMP_FILTER_CLKDIV_DEFAULT	
	    	ACMP_vSetCompFilt(ACMP3, ACMP_FILTER_SMODE_DEFAULT, ACMP_FILTER_CLKDIV_DEFAULT);
        #endif
	#endif
}

/**
 * @brief	Setting up propagation delay
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupPropDelay(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
	    #ifdef ACMP_PROPDLY_DEFAULT
	    	ACMP_vSetPropDelay(ACMP0, ACMP_PROPDLY_DEFAULT);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP1_ENABLE)
	    #ifdef ACMP_PROPDLY_DEFAULT
	    	ACMP_vSetPropDelay(ACMP1, ACMP_PROPDLY_DEFAULT);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP2_ENABLE)
	    #ifdef ACMP_PROPDLY_DEFAULT	
	    	ACMP_vSetPropDelay(ACMP2, ACMP_PROPDLY_DEFAULT);
		#endif
    #endif

    #if (ENABLE == CFG_ACMP3_ENABLE)
	    #ifdef ACMP_PROPDLY_DEFAULT
	    	ACMP_vSetPropDelay(ACMP3, ACMP_PROPDLY_DEFAULT);
		#endif
    #endif
}

/**
 * @brief	Setup Ring Oscillator control
 * @param	Nothing
 * @return	Nothing
 */
STATIC INLINE void ACMP_vCfgSetupRingOsc(void)
{
	#ifdef ACMP_ROSCCTL_DEFAULT
    	ACMP_vSetRingOscCtl(ACMP_ROSCCTL_DEFAULT);
	#endif
	#ifdef ACMP_ROSCRST_DEFAULT
		ACMP_vSetRingOscRstSrc(ACMP_ROSCRST_DEFAULT);
	#endif
}
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initializes and start the ACMP */
void ACMP_vInit(void)
{
	/* Enable clock and power up ACMP */
	SYSCTL_vPowerUp(ACMP_nPowerupConfig());
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_IREF_PD);
	
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_ACMP);
	SYSCTL_vPeriphReset(RESET_ACMP);

    /* Disable ACMP before setuping */
	ACMP_vDisableUsedACMPInt();
	ACMP_vDisableUsedACMP();

	/* Setup Ring Oscillator control - Ctrl and reset source */
	ACMP_vCfgSetupRingOsc();

    /* Setup Comparator Filter */
	ACMP_vCfgSetupFilter();

	/* Setup Propagation Delay */
	ACMP_vCfgSetupPropDelay();

    /* Setup ACMP according to configurations */
	ACMP_vCfgSetupVoltRefs();
	ACMP_vCfgSetupVoltLadder();
	ACMP_vCfgSetupInt();

    /* Enable ACMP */
	ACMP_vEnableUsedACMP();
	ACMP_vEnableUsedACMPInt();
}

/* Shutdown the ACMP */
void ACMP_vShutdown(void)
{
	CLOCK_vDisablePeriphClock(SYSCTL_CLOCK_ACMP);
	
	SYSCTL_vPowerDown(ACMP_nPowerupConfig());
	SYSCTL_vPowerDown(SYSCTL_POWERDOWN_IREF_PD);
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
#endif/* CFG_ACMP_USED */

/******************************************************************************/
/*                      ISR IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for ACMP0/1/2/3
 *   Each ACMP has an independent NVIC slot
 **/
void ACMP0_vISR(void)
{
    #if (ENABLE == CFG_ACMP0_ENABLE)
	if(ACMP_bGetIntStatus(ACMP0)){
		ACMP_vClearIntStatus(ACMP0);
		
        /* CallbackBody */
		#ifdef ACMP0_CB
        ACMP0_CB();
		#endif
	}	
	#endif
}

void ACMP1_vISR(void)
{
    #if (ENABLE == CFG_ACMP1_ENABLE)
	if(ACMP_bGetIntStatus(ACMP1)){
		ACMP_vClearIntStatus(ACMP1);
		
        /* CallbackBody */
		#ifdef ACMP1_CB
        ACMP1_CB();
		#endif
	}	
	#endif
}

void ACMP2_vISR(void)
{
    #if (ENABLE == CFG_ACMP2_ENABLE)
	if(ACMP_bGetIntStatus(ACMP2)){
		ACMP_vClearIntStatus(ACMP2);
		
        /* CallbackBody */
		#ifdef ACMP2_CB	
        ACMP2_CB();
		#endif
	}	
	#endif
}

void ACMP3_vISR(void)
{
    #if (ENABLE == CFG_ACMP3_ENABLE)
	if(ACMP_bGetIntStatus(ACMP3)){
		ACMP_vClearIntStatus(ACMP3);
		
        /* CallbackBody */
		#ifdef ACMP3_CB	
        ACMP3_CB();
		#endif
	}	
	#endif
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

