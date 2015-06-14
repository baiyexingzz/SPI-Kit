/******************************************************************************/
/*                                                                            */
/*    $Workfile::   ExtInt.c                                            $   */
/*                                                                            */
/*    $Revision:: 316                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-10 16:23:00 +0800 (周一, 10 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - Pin Interrupt   */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include "extint.h"

#include "io.h"
#include "inmux.h"

#ifdef CFG_EXTINT_USED
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
typedef void (*P_EXTINT_CALLBACK)(PININT_STATE_T tState);

/* Declare macro for callback */
#define DECLARE_EXTINT_CALLBACK(_Func_)  extern void _Func_(PININT_STATE_T tState);

/* Declare EXINT callbacks if exist */
#if (ENABLE == CFG_PININTCH0_ENABLE)
#ifdef PININTCH0_CB
DECLARE_EXTINT_CALLBACK(PININTCH0_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH1_ENABLE)
#ifdef PININTCH1_CB
DECLARE_EXTINT_CALLBACK(PININTCH1_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH2_ENABLE)
#ifdef PININTCH2_CB
DECLARE_EXTINT_CALLBACK(PININTCH2_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH3_ENABLE)
#ifdef PININTCH3_CB
DECLARE_EXTINT_CALLBACK(PININTCH3_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH4_ENABLE)
#ifdef PININTCH4_CB
DECLARE_EXTINT_CALLBACK(PININTCH3_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH5_ENABLE)
#ifdef PININTCH5_CB
DECLARE_EXTINT_CALLBACK(PININTCH5_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH6_ENABLE)
#ifdef PININTCH6_CB
DECLARE_EXTINT_CALLBACK(PININTCH6_CB);
#endif
#endif

#if (ENABLE == CFG_PININTCH7_ENABLE)
#ifdef PININTCH7_CB
DECLARE_EXTINT_CALLBACK(PININTCH7_CB);
#endif
#endif

/* Alias macro for enable/disable rising and falling */
#define EXTINT_vEnableIntRising    EXTINT_vEnableIntHigh 
#define EXTINT_vEnableIntFalling   EXTINT_vEnableIntLow
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
 * @brief	Configure the pins as edge sensitive in Pin interrupt block
 * @param	ubPin	: Pins (value of PININTCH*)
  * @param	unMode	: PININT_MODE_EDGE or PININT_MODE_LEVEL
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vSetPinMode(PININT_CH_T unPin, PININT_MODE_T tMode)
{
    U32 temp = ((PININT_MODE_LEVEL == tMode)? PININTCH(unPin) : 0);
	SET_REG_FIELD(LPC_PIN_INT->ISEL, PININTCH_MASK&(PININTCH(unPin)), temp);
}

/**
 * @brief	Enable/Disable high level/rising edge PININT interrupts for pins
 * @param	ubPin	: Pins (value of PININTCH*)
 * @param	ubPin	: true: enable high and disable low ; false: disable high and enable low
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vEnableIntHigh(PININT_CH_T unPin, bool bEnable)
{
	if(bEnable){
		SET_REG_FIELD(LPC_PIN_INT->SIENR, PININTCH_MASK&(PININTCH(unPin)), (PININTCH(unPin)));
	}
	else{
        SET_REG_FIELD(LPC_PIN_INT->CIENR, PININTCH_MASK&(PININTCH(unPin)), (PININTCH(unPin)));
	}
}

/**
 * @brief	Enable/Disable low level/ falling edge PININT interrupts for pins
 * @param	ubPin	: Pins (value of PININTCH*)
 * @param	ubPin	: true: enable; false: disable 
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vEnableIntLow(PININT_CH_T unPin, bool bEnable)
{
	if(bEnable){
		SET_REG_FIELD(LPC_PIN_INT->SIENF, PININTCH_MASK&(PININTCH(unPin)), (PININTCH(unPin)));
	}
	else{
		SET_REG_FIELD(LPC_PIN_INT->CIENF, PININTCH_MASK&(PININTCH(unPin)), (PININTCH(unPin)));
	}
}

/**
 * @brief	Return pin states that have a detected latched high edge (RISE) state
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	true: a latched rise state detected; false - not detected
 */
STATIC INLINE bool EXTINT_bGetRiseState(PININT_CH_T unPin)
{
	return (GET_REG_FIELD(LPC_PIN_INT->RISE, PININTCH_MASK&(PININTCH(unPin))) != 0);
}

/**
 * @brief	Clears pin states that had a latched high edge (RISE) state
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vClearRiseState(PININT_CH_T unPin)
{
	/* Write 1 to clear */
	WRITE_REG(LPC_PIN_INT->RISE, PININTCH(unPin));
}

/**
 * @brief	Return pin states that have a detected latched falling edge (FALL) state
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	true: a latched fall state detected; false - not detected

 */
STATIC INLINE bool EXTINT_bGetFallState(PININT_CH_T unPin)
{
	return (GET_REG_FIELD(LPC_PIN_INT->FALL, (PININTCH_MASK&(PININTCH(unPin)))) != 0);
}

/**
 * @brief	Clears pin states that had a latched falling edge (FALL) state
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vClearFallState(PININT_CH_T unPin)
{
	/* Write 1 to clear */
	WRITE_REG(LPC_PIN_INT->FALL, PININTCH(unPin));
}

#ifdef CFG_EXTINT_LEVEL_USED
/**
 * @brief	Get interrupt status from Pin interrupt block
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	Interrupt status (bit n for PININTn = high means interrupt ie pending)
 */
STATIC INLINE bool EXTINT_bGetIntStatus(PININT_CH_T unPin)
{
	return (GET_REG_FIELD(LPC_PIN_INT->IST, (PININTCH_MASK&(PININTCH(unPin)))) != 0);	
}

/**
 * @brief	Clear interrupt status in Pin interrupt block
 * @param	ubPin	: Pins (value of PININTCH*)
 * @return	Nothing
 */
STATIC INLINE void EXTINT_vClearIntStatus(PININT_CH_T unPin)
{
	/* Write 1 to clear */
	WRITE_REG(LPC_PIN_INT->IST, (PININTCH(unPin)));
}
#endif

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Initialize Pin interrupt block */
#define EXTINT_SETUP_EDGE(ch)           EXTINT_vSetPinMode(ch, PININT_MODE_EDGE);\
	                                    EXTINT_vClearRiseState(ch);\
	                                    EXTINT_vEnableIntRising(ch, CFG_##ch##_EDGE_RISING);\
                                        EXTINT_vClearFallState(ch);\
	                                    EXTINT_vEnableIntFalling(ch, CFG_##ch##_EDGE_FALLING);
#define EXTINT_SETUP_LEVEL(ch)          EXTINT_vSetPinMode(ch, PININT_MODE_LEVEL);\
	                                    EXTINT_vClearIntStatus(ch);\
	                                    EXTINT_vEnableIntHigh(ch, CFG_##ch##_LEVEL_HIGH);\
	                                    EXTINT_vEnableIntLow(ch, !(CFG_##ch##_LEVEL_HIGH))\

void EXTINT_vInit(void)
{
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_PININT);
	SYSCTL_vPeriphReset(RESET_PININT);

	#if(ENABLE == CFG_PININTCH0_ENABLE)		
		INMUX_vExtPinIntSel(0, CFG_PININTCH0_PIN_SEL);
	
	    #if (ENABLE == CFG_PININTCH0_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH0);
		#elif (DISABLE == CFG_PININTCH0_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH0);	
		#endif
    #endif 

	#if(ENABLE == CFG_PININTCH1_ENABLE)
		INMUX_vExtPinIntSel(1, CFG_PININTCH1_PIN_SEL);	
	
		#if (ENABLE == CFG_PININTCH1_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH1);
		#elif (DISABLE == CFG_PININTCH1_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH1);	
		#endif
    #endif 

	#if(ENABLE == CFG_PININTCH2_ENABLE)
		INMUX_vExtPinIntSel(2, CFG_PININTCH2_PIN_SEL);		
	
	    #if (ENABLE == CFG_PININTCH2_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH2);
		#elif (DISABLE == CFG_PININTCH2_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH2);	
		#endif
    #endif 	

	#if(ENABLE == CFG_PININTCH3_ENABLE)	
		INMUX_vExtPinIntSel(3, CFG_PININTCH3_PIN_SEL);
	
	    #if (ENABLE == CFG_PININTCH3_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH3);
		#elif (DISABLE == CFG_PININTCH3_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH3);	
		#endif
    #endif 	

	#if(ENABLE == CFG_PININTCH4_ENABLE)	
		INMUX_vExtPinIntSel(4, CFG_PININTCH4_PIN_SEL);		
	
	    #if (ENABLE == CFG_PININTCH4_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH4);
		#elif (DISABLE == CFG_PININTCH4_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH4);		
		#endif
		
    #endif 		

	#if(ENABLE == CFG_PININTCH5_ENABLE)	
    	INMUX_vExtPinIntSel(5, CFG_PININTCH5_PIN_SEL);		
	
	    #if (ENABLE == CFG_PININTCH5_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH5);
		#elif (DISABLE == CFG_PININTCH5_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH5);	
		#endif
    #endif 

	#if(ENABLE == CFG_PININTCH6_ENABLE)	
        INMUX_vExtPinIntSel(6, CFG_PININTCH6_PIN_SEL);	
	
	    #if (ENABLE == CFG_PININTCH6_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH6);
		#elif (DISABLE == CFG_PININTCH6_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH6);	
		#endif
    #endif 	

	#if(ENABLE == CFG_PININTCH7_ENABLE)	
        INMUX_vExtPinIntSel(7, CFG_PININTCH7_PIN_SEL);
   
		#if (ENABLE == CFG_PININTCH7_MODE_LEVEL) 
	    EXTINT_SETUP_LEVEL(PININTCH7);
		#elif (DISABLE == CFG_PININTCH7_MODE_LEVEL)
	    EXTINT_SETUP_EDGE(PININTCH7);	
		#endif
    #endif 	
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                      ISR IMPLEMENTATIONS                      */
/******************************************************************************/
/** 
 *   ISR callbacks for EXTINT0/1/2/3/4/5/6/7
 *   Each EXTINT has an independent NVIC slot
 **/
void PININT0_vISR(void)
{
    #if (ENABLE == CFG_PININTCH0_ENABLE)        
        #if(ENABLE == CFG_PININTCH0_MODE_LEVEL)
		/* CallbackBody for level extint */
        if(EXTINT_bGetIntStatus(PININTCH0)){
			EXTINT_vClearIntStatus(PININTCH0);
			#ifdef PININTCH0_CB
			    #if (ENABLE == CFG_PININTCH0_LEVEL_HIGH)
		        PININTCH0_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH0_LEVEL_HIGH)
		        PININTCH0_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	    #elif(DISABLE == CFG_PININTCH0_MODE_LEVEL)
		/* CallbackBody for edge extint */
	    #if (ENABLE == CFG_PININTCH0_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH0)){
			EXTINT_vClearRiseState(PININTCH0);
			#ifdef PININTCH0_CB
	        	PININTCH0_CB(PININT_STATE_RISING);
			#endif
        }
		#endif
        #if (ENABLE == CFG_PININTCH0_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH0)){
			EXTINT_vClearFallState(PININTCH0);			
			#ifdef PININTCH0_CB
	        	PININTCH0_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT1_vISR(void)
{
    #if (ENABLE == CFG_PININTCH1_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH1_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH1)){
			EXTINT_vClearIntStatus(PININTCH1);
			#ifdef PININTCH1_CB
			    #if (ENABLE == CFG_PININTCH1_LEVEL_HIGH)
		        PININTCH1_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH1_LEVEL_HIGH)
		        PININTCH1_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH1_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH1_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH1)){
			EXTINT_vClearRiseState(PININTCH1);
			#ifdef PININTCH1_CB
	        	PININTCH1_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH1_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH1)){
			EXTINT_vClearFallState(PININTCH1);			
			#ifdef PININTCH1_CB
	        	PININTCH1_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT2_vISR(void)
{
    #if (ENABLE == CFG_PININTCH2_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH2_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH2)){
			EXTINT_vClearIntStatus(PININTCH2);
			#ifdef PININTCH2_CB
			    #if (ENABLE == CFG_PININTCH2_LEVEL_HIGH)
		        PININTCH2_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH2_LEVEL_HIGH)
		        PININTCH2_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH2_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH2_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH2)){
			EXTINT_vClearRiseState(PININTCH2);
			#ifdef PININTCH2_CB
	        	PININTCH2_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH2_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH2)){
			EXTINT_vClearFallState(PININTCH2);			
			#ifdef PININTCH2_CB
	        	PININTCH2_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT3_vISR(void)
{
    #if (ENABLE == CFG_PININTCH3_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH3_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH3)){
			EXTINT_vClearIntStatus(PININTCH3);
			#ifdef PININTCH3_CB
			    #if (ENABLE == CFG_PININTCH3_LEVEL_HIGH)
		        PININTCH3_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH3_LEVEL_HIGH)
		        PININTCH3_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH3_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH3_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH3)){
			EXTINT_vClearRiseState(PININTCH3);
			#ifdef PININTCH3_CB
	        	PININTCH3_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH3_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH3)){
			EXTINT_vClearFallState(PININTCH3);			
			#ifdef PININTCH3_CB
	        	PININTCH3_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif

}

void PININT4_vISR(void)
{
    #if (ENABLE == CFG_PININTCH4_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH4_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH4)){
			EXTINT_vClearIntStatus(PININTCH4);
			#ifdef PININTCH4_CB
			    #if (ENABLE == CFG_PININTCH4_LEVEL_HIGH)
		        PININTCH4_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH4_LEVEL_HIGH)
		        PININTCH4_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH4_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH4_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH4)){
			EXTINT_vClearRiseState(PININTCH4);
			#ifdef PININTCH4_CB
	        	PININTCH4_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH4_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH4)){
			EXTINT_vClearFallState(PININTCH4);			
			#ifdef PININTCH4_CB
	        	PININTCH4_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT5_vISR(void)
{
    #if (ENABLE == CFG_PININTCH5_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH5_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH5)){
			EXTINT_vClearIntStatus(PININTCH5);
			#ifdef PININTCH5_CB
			    #if (ENABLE == CFG_PININTCH5_LEVEL_HIGH)
		        PININTCH5_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH5_LEVEL_HIGH)
		        PININTCH5_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH5_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH5_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH5)){
			EXTINT_vClearRiseState(PININTCH5);
			#ifdef PININTCH5_CB
	        	PININTCH5_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH5_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH5)){
			EXTINT_vClearFallState(PININTCH5);			
			#ifdef PININTCH5_CB
	        	PININTCH5_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT6_vISR(void)
{
    #if (ENABLE == CFG_PININTCH6_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH6_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH6)){
			EXTINT_vClearIntStatus(PININTCH6);
			#ifdef PININTCH6_CB
			    #if (ENABLE == CFG_PININTCH6_LEVEL_HIGH)
		        PININTCH6_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH6_LEVEL_HIGH)
		        PININTCH6_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH6_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH6_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH6)){
			EXTINT_vClearRiseState(PININTCH6);
			#ifdef PININTCH6_CB
	        	PININTCH6_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH6_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH6)){
			EXTINT_vClearFallState(PININTCH6);			
			#ifdef PININTCH6_CB
	        	PININTCH6_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}

void PININT7_vISR(void)
{
    #if (ENABLE == CFG_PININTCH7_ENABLE)
    /* CallbackBody */
    #if(ENABLE == CFG_PININTCH7_MODE_LEVEL)
        if(EXTINT_bGetIntStatus(PININTCH7)){
			EXTINT_vClearIntStatus(PININTCH7);
			#ifdef PININTCH7_CB
			    #if (ENABLE == CFG_PININTCH7_LEVEL_HIGH)
		        PININTCH7_CB(PININT_STATE_HIGH);
				#elif (DISABLE == CFG_PININTCH7_LEVEL_HIGH)
		        PININTCH7_CB(PININT_STATE_LOW);
				#endif
			#endif
        }
	#elif(DISABLE == CFG_PININTCH7_MODE_LEVEL)
	    #if (ENABLE == CFG_PININTCH7_EDGE_RISING)
        if(EXTINT_bGetRiseState(PININTCH7)){
			EXTINT_vClearRiseState(PININTCH7);
			#ifdef PININTCH7_CB
	        	PININTCH7_CB(PININT_STATE_RISING);
			#endif
        }
		#endif

        #if (ENABLE == CFG_PININTCH7_EDGE_FALLING)
        if(EXTINT_bGetFallState(PININTCH7)){
			EXTINT_vClearFallState(PININTCH7);		
			#ifdef PININTCH7_CB
	        	PININTCH7_CB(PININT_STATE_FALLING);
			#endif
        }
		#endif
	#endif
	#endif
}
#endif
/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

