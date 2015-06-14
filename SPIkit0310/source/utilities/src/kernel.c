/******************************************************************************/
/*                                                                            */
/*    $Workfile::   kernel.c                                             $    */
/*                                                                            */
/*    $Revision:: 3419                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2012-04-25 19:38:13 +0800 (周三, 25 四月 2012)     $    */
/*                                                                            */
/*      $Author:: kelvin.zhang@philips.com                               $    */
/*                                                                            */
/*        Owner::   PHS                                                       */
/*                                                                            */
/*  Description::   Handle kernel functions                                   */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2007 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include <string.h>
#include <assert.h>

#include "platform.h"
#include "stdtypes.h"

#include "hal.h"
#include "logging.h"
#include "kernel.h"

#ifdef KERNEL_STATISTICS
  #include "print.h"
#endif

/* It's not needed to include modules where the (task handlers) prototypes are
   declared. These prototypes will be generated automatically from the
   kernel_cfg file.
*/  

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Timer trigger time is set to this value if a timer is stopped */
#define KERNEL_TIMER_STOPPED        ((T_SYSTEM_TIME)(0))
/* Timer trigger time is ORed with this value if a timer is started */
#define KERNEL_TIMER_RUNS           ((T_SYSTEM_TIME)(1))

/* Pending event count is set to this value if a event is not pending (anymore)
*/
#define KERNEL_EVENT_IS_NOT_PENDING (0)
/* Pending event count is set to this value if a event is pending */
#define KERNEL_EVENT_IS_PENDING     (1)

/* This value is used to shift out (or in) bit 0 (bit 0 indicates if the timer
  is running */
#define KERNEL_TIMER_SHIFT_FACTOR   (1)

/* This value is used to increase the system time inside the kernel callback
   function */
#define KERNEL_TIMER_INCREASE_COUNT (1<<KERNEL_TIMER_SHIFT_FACTOR)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
/*lint -esym(750,KERNEL_PROTO) */
/*
 Suppress PC-Lint Info 750: Local macro not referenced
 When no kernel tasks are present, this macro will not be referenced...

 Helper macro for kernel module, generates automatic prototypes for
 the kernel function handlers.
*/ 
#ifdef KERNEL_DATA_PARAM
  #define KERNEL_PROTO(_FUNC_) extern void _FUNC_(TE_EVENT_ID teEventId, \
                                                    ULONG ulData)
#else
  #define KERNEL_PROTO(_FUNC_) extern void _FUNC_(TE_EVENT_ID teEventId)
#endif

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

typedef struct
{
  TE_EVENT_ID teEventID;
  TE_FUNCTION_ID teFunctionID;
}TS_SUBSCRIBED_EVENT_LIST;

typedef struct
{
  UBYTE           ubCount;
  #ifdef KERNEL_DATA_PARAM
    ULONG         ulData;
  #endif
  #ifdef KERNEL_STATISTICS
    ULONG         ulScheduleCount;
    ULONG         ulExecuteCount;
    T_SYSTEM_TIME tExecuteTime;
    T_SYSTEM_TIME tStartTime;
  #endif
}TS_PENDING_EVENT;



/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
//be very careful with this varialbe, always keep it as the first one
#if ((TARGET_DEVICE == DEVICE_BASICVALUE)||(TARGET_DEVICE == DEVICE_SUPERVALUE))

/*lint -esym(551,ubStoredCCValue) */
/* ubStoredCCValue is only accessed by assembly code */
static UBYTE ubStoredCCValue;
static BOOL blKernelUpdateBlocked;
#endif
static T_SYSTEM_TIME  tSystemTime;
static BOOL           blFlagSystemTimeUpdated;

#ifdef KERNEL_STATISTICS
/*lint -esym(750,KERNEL_PROTO) */

/*
 Suppress PC-Lint Info 750: Local macro not referenced
 When no kernel tasks are present, this macro will not be referenced...

 Create labels for the timer and event lists
*/
 static const char * tsLabels[ ] =
 {
   #define KERNEL_CFG_TIMER_DEFINITIONS
     #define KERNEL_ADD_TIMER(_TIMER_ID_) #_TIMER_ID_,

/* Coding Convetion tool requires that an #include is left aligned!! */     
 /*>RELAX<CodingConvention_20040_FileSection> */
 #include "kernel_cfg.h"
 
   #undef KERNEL_ADD_TIMER
   #undef KERNEL_CFG_TIMER_DEFINITIONS

   #define KERNEL_CFG_EVENT_DEFINITIONS
     #define KERNEL_ADD_EVENT(_EVENT_ID_) #_EVENT_ID_,

/* Coding Convetion tool requires that an #include is left aligned!! */
/*>RELAX<CodingConvention_20040_FileSection> */
#include "kernel_cfg.h"

   #undef KERNEL_ADD_EVENT
   #undef KERNEL_CFG_EVENT_DEFINITIONS
 };
#endif
/* List containing the trigger times for each timer */
static T_SYSTEM_TIME atTimerTriggerTimes[ TIMERID_LAST ];

/* Create prototypes for the function list */
  /*>RELAX<CodingConvention_20040_FileSection> */
  #define KERNEL_CFG_FUNCTION_DEFINITIONS
  /*>RELAX<CodingConvention_20040_FileSection> */
  #define KERNEL_ADD_FUNCTION(_FUNCTION_ID_, _FUNCTION_HANDLER_) \
                                KERNEL_PROTO(_FUNCTION_HANDLER_);

/* Coding Convetion tool requires that an #include is left aligned!! */         
/*>RELAX<CodingConvention_20040_FileSection> */
/*lint -e(451) disable repeatedly included and no standard include guard */
#include "kernel_cfg.h"

  #undef KERNEL_ADD_FUNCTION

  /* Create function list for the kernel functions */
  static const T_PFVOID apfnKernelFunctionList[FUNCTIONID_LAST] =
  {
    /*>RELAX<CodingConvention_20040_FileSection> */
    #define KERNEL_ADD_FUNCTION(_FUNCTION_ID_, _FUNCTION_HANDLER_) \
                                  _FUNCTION_HANDLER_,

/* Coding Convetion tool requires that an #include is left aligned!! */
/*>RELAX<CodingConvention_20040_FileSection> */
/*lint -e(451) disable repeatedly included and no standard include guard */
#include "kernel_cfg.h"

    #undef KERNEL_ADD_FUNCTION
  };
#undef KERNEL_CFG_FUNCTION_DEFINITIONS

/*
  Create a list where events can be subscribed, each member has an eventID with
  a corresponding functionID
*/
static TS_SUBSCRIBED_EVENT_LIST atsSubcribedEventList[KERNEL_SUB_SCR_LIST_SIZE];

/* Create a pending event list, each element has a ubCount member, which
   indicates that event is pending, it also contains statistics.
*/   
static TS_PENDING_EVENT atsPendingEventList[EVENTID_LAST];

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/
static void KERNEL_vScheduler(void);
/*
** Description: Kernel scheduler. The scheduler is executed if the systemtime 
**              is increased. A timer event ID is posted if the timer is
**              elapsed.
** Parameters:  void
** Returns:     void
*/

static void KERNEL_vCallback(void);
/*
** Description: Function which is called if the kernel timer ISR occured.
** Parameters:  void
** Returns:     void
*/

static UBYTE KERNEL_ubSearchSubscribedEvent(const TE_EVENT_ID teEventID,
                                             TE_FUNCTION_ID teFunctionID);
/*
** Description: Searches for a subscribed event in the Subcribed Event List.
** Parameters:  teEventID:  Event index to search (created by kernel_cfg.h)
**              teFunctionID: Function pointer to search (kernel_cfg.h creates
**              a list of function pointers)
** Returns:     Position (index) of event inside the Subcribed Event List OR
**              "KERNEL_SUB_SCR_LIST_SIZE" if event is not found in list.
*/

static UBYTE KERNEL_ubFindFreeEventSlot(void);
/*
** Description: Searches for a free slot in the Subcribed Event List.
** Parameters:  void
** Returns:     Position (index) of free slot inside the Subcribed Event List 
**              OR "KERNEL_SUB_SCR_LIST_SIZE" if list is FULL.
*/


#if ((TARGET_DEVICE == DEVICE_BASICVALUE)||(TARGET_DEVICE == DEVICE_SUPERVALUE))
static TARGET_INLINE void KERNEL_vEnterCriticalCode(void)
{
   ubStoredCCValue = (UBYTE)_asm("PUSH CC\nPOP A\nPUSH #0\nPOP CC");
   blKernelUpdateBlocked=TRUE;
}

static TARGET_INLINE void KERNEL_vLeaveCriticalCode(void)
{
  blKernelUpdateBlocked=FALSE;
  _asm("PUSH A\nPOP CC\n", ubStoredCCValue);
}

#else
#define KERNEL_vEnterCriticalCode()  
#define KERNEL_vLeaveCriticalCode()  
#endif
/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/

void KERNEL_vInit(void)
/*
** Description: Initializes the kernel. The complete Timer-List is cleared.
**              The SubcribedEventList is set empty and the kernel timer
**              driver
**              is initialised (with kernel callback function)
** Parameters:  void
** Returns:     void
*/
{
  UBYTE ubIndex;

  tSystemTime = KERNEL_TIMER_RUNS;
  blFlagSystemTimeUpdated = FALSE;
#if ((TARGET_DEVICE == DEVICE_BASICVALUE)||(TARGET_DEVICE == DEVICE_SUPERVALUE))
  blKernelUpdateBlocked=FALSE;
#endif

  /* Clear timer trigger time list */
  (void)memset((void *)atTimerTriggerTimes, KERNEL_TIMER_STOPPED,
                                     sizeof(atTimerTriggerTimes));

  /* Clear Subscribed event list --> Set all events to 'EVENTID_LAST' */
  for (ubIndex=0; ubIndex<KERNEL_SUB_SCR_LIST_SIZE; ubIndex++)
  {
    atsSubcribedEventList[ubIndex].teEventID = EVENTID_LAST;
  }

  /* Clear Pending events list */
  (void)memset(atsPendingEventList, KERNEL_EVENT_IS_NOT_PENDING,
                                      sizeof(atsPendingEventList));

  /* Register for the KERNEL statistics event (if enabled) */
  #ifdef KERNEL_STATISTICS
    KERNEL_vSubscribeForEvent(EVENTID_KERNEL_STATICS,
                              FUNCID_KERNEL_STATS_TASKHANDLER);
    KERNEL_vPostEvent(EVENTID_KERNEL_STATICS);
  #endif

  /* Init KernelTimer driver (Set KernelCallback function) */
  HAL_vInitKernelTimer(KERNEL_vCallback);
}

void KERNEL_vStartTimer(const TE_TIMER_ID teTimerID,
                        const T_SYSTEM_TIME tPeriodTime,
                        const TE_FUNCTION_ID teFunctionID)
/*
** Description: Start a kernel timer. The triggertime from this timer in the
**              Timer-List is filled.
**              The timer event is added to the SubcribedEventList.
** Parameters:  teTimerID:    Timer index of timer to start (created by
**                            kernel_cfg.h)
**              tPeriodTime:  Requested periodtime
**              teFunctionID: Function pointer index which will be
**                            unregistered from the SubcribedEventList.
**                            (kernel_cfg.h creates a list of function
**                            pointers)
** Returns:     void
*/
{
  LOG_FUNCTION4(COMPID_KERNEL, KERNEL_LOG_IDS,
                 "vStartTimer %d time:%d func:%d\n", teTimerID, tPeriodTime, 
                 teFunctionID);
  assert(teTimerID < TIMERID_LAST);
  assert(teFunctionID < FUNCTIONID_LAST);

  if (teTimerID < TIMERID_LAST)
  {
    /* Adds timer event ID to the SubcribedEventList */
    KERNEL_vSubscribeForEvent((TE_EVENT_ID)teTimerID, teFunctionID);
    /* Shift period time (tSystemTime.0 indicates if the timer is running) and 
        set tSystemTime.0
    */

    KERNEL_vEnterCriticalCode();

    atTimerTriggerTimes[teTimerID] = ((tPeriodTime << KERNEL_TIMER_SHIFT_FACTOR)
                                             + tSystemTime) | KERNEL_TIMER_RUNS;

    KERNEL_vLeaveCriticalCode();
  }
}

void KERNEL_vStopTimer(const TE_TIMER_ID teTimerID,
                       const TE_FUNCTION_ID teFunctionID)
/*
** Description: Stops a pending kernel timer. The triggertime from this timer
**              is set to KERNEL_TIMER_STOPPED.
**              The timer event is removed from the SubcribedEventList.
** Parameters:  teTimerID:  Timer index of timer to stop (created by
**              kernel_cfg.h)
**              teFunctionID: Function pointer index which will be
**              unregistered from the SubcribedEventList.
**              (kernel_cfg.h creates a list of function pointers)
** Returns:     void
*/
{
  LOG_FUNCTION3(COMPID_KERNEL, KERNEL_LOG_IDS, "vStopTimer %d func:%d\n",
                   teTimerID, teFunctionID);
  assert(teTimerID < TIMERID_LAST);
  assert(teFunctionID < FUNCTIONID_LAST);

  if (teTimerID < TIMERID_LAST)
  {
    /* Removes timer event ID from the SubcribedEventList */
    KERNEL_vUnsubscribeForEvent((TE_EVENT_ID)teTimerID, teFunctionID);
    atTimerTriggerTimes[teTimerID] = KERNEL_TIMER_STOPPED;
  }
}

void KERNEL_vSubscribeForEvent(const TE_EVENT_ID teEventID,
                               const TE_FUNCTION_ID teFunctionID)
/*
** Description: An event ID is subcribed to the SubcribedEventList.
**              A free slot inside the list is searched. The event is
**              subscribed to the list if an empty slot is found.
** Parameters:  teEventID:    Event index to subscribe (created by
**                            kernel_cfg.h)
**              teFunctionID: Function pointer index which will be registered 
**                            in theSubcribedEventList. (kernel_cfg.h
**                            creates a list of function pointers) 
** Returns:     void
*/
{
  UBYTE ubIndex;

  LOG_FUNCTION3(COMPID_KERNEL, KERNEL_LOG_IDS, "vSubscribeForEvent %d func:%d\n",
                 teEventID, teFunctionID);
  assert(teEventID < EVENTID_LAST);
  assert(teFunctionID < FUNCTIONID_LAST);

  if (teEventID < EVENTID_LAST)
  {
    /* First remove 'old' event (same Event ID and Function ID) before adding
       the 'new' one */
    KERNEL_vUnsubscribeForEvent(teEventID, teFunctionID);

    /* Find a free slot inside the subscribed EventList */
    ubIndex = KERNEL_ubFindFreeEventSlot();
    if (ubIndex < KERNEL_SUB_SCR_LIST_SIZE)
    { 
      /* Empty slot is found --> add this event */
      atsSubcribedEventList[ubIndex].teEventID = teEventID;
      atsSubcribedEventList[ubIndex].teFunctionID = teFunctionID;
    }
    else
    {
      /* All slots are full */
      assert(FALSE);
    }
  }
}

void KERNEL_vUnsubscribeForEvent(const TE_EVENT_ID teEventID,
                                 const TE_FUNCTION_ID teFunctionID)
/*
** Description: An event ID is unsubcribed from the SubcribedEventList.
**              The event is searched in the list. The event is unsubscribed
**              from the list if the event is found in the list.
** Parameters:  teEventID:   Event index to subscribe (created by kernel_cfg.h)
**              teFunctionID: Function pointer index which will be registered 
**                            in the SubcribedEventList. (kernel_cfg.h creates
**                            a list of function pointers)
** Returns:     void
*/
{
  UBYTE ubIndex;

  LOG_FUNCTION3(COMPID_KERNEL, KERNEL_LOG_IDS,
                 "vUnsubscribeForEvent %d func:%d\n", teEventID, teFunctionID);
  assert(teEventID < EVENTID_LAST);
  assert(teFunctionID < FUNCTIONID_LAST);

  if (teEventID < EVENTID_LAST)
  {
    /* Search if event is in the list */
    ubIndex = KERNEL_ubSearchSubscribedEvent(teEventID, teFunctionID);
    if (ubIndex < KERNEL_SUB_SCR_LIST_SIZE)
    {
      /* Remove event from list */
      atsSubcribedEventList[ubIndex].teEventID = EVENTID_LAST;
    }
    else
    {
      /* Event is not in the list. Do not use an ASSERT here, this is not
         an error situation! */
    }
  }
}

#ifdef KERNEL_DATA_PARAM
  void KERNEL_vPostEvent(const TE_EVENT_ID teEventID, ULONG ulData)
#else
  void KERNEL_vPostEvent(const TE_EVENT_ID teEventID)
#endif
/*
** Description: Post an event. The event counter in the atsPendingEventList
**              is increased (the event ID can be pending more than one time) 
**              or set to '1' (eventID is pending once).
** Parameters:  teEventID:  Event index to subscribe (created by kernel_cfg.h)
** Returns:     void
*/
{
  LOG_FUNCTION2(COMPID_KERNEL, KERNEL_LOG_IDS, "vPostEvent %d\n", teEventID);
  assert(teEventID < EVENTID_LAST);

  if (teEventID < EVENTID_LAST)
  {
    /*TODO: SHARED RESOURCE
      atsPendingEventList is a SHARED RESOURCE
      EventID is pending more times (if not handled on-time)
      atsPendingEventList[(UBYTE)teEventID].ubCount++;
    
      EventID is pending only once
    */      
    atsPendingEventList[(UBYTE)teEventID].ubCount = KERNEL_EVENT_IS_PENDING;
    
    #ifdef KERNEL_DATA_PARAM
      atsPendingEventList[(UBYTE)teEventID].ulData = ulData;
    #endif
    
    /* Keep record of some statistics (if enabled) */
    #ifdef KERNEL_STATISTICS
      atsPendingEventList[(UBYTE)teEventID].tStartTime = 
                      (tSystemTime >> KERNEL_TIMER_SHIFT_FACTOR);
      atsPendingEventList[(UBYTE)teEventID].ulScheduleCount++;
    #endif
    /* atsPendingEventList is a SHARED RESOURCE */
  }
}

void KERNEL_vDispatcher(void)
/*
** Description: Kernel dispatcher. The dispatcher checks for pending events.
**              The event is searched in the Subcribed Event List if an event 
**              is pending. The function is executed if the event is found in 
**              the list. Each complete cycle inside the KERNEL_vDispatcher
** Parameters:  void
** Returns:     *** THIS FUNCTION NEVER RETURNS ***
*/
{
  UBYTE         ubIndex;
  TE_EVENT_ID   teEventIndex;

  /* end-less loop */
  for (;;)
  {
    
    HAL_vSetCPUTestPin(CPULOAD_KERNEL_FREERUN_TIME);
    /* Check if systemtime is updated (inside ISR) */
    if (blFlagSystemTimeUpdated)
    {
      blFlagSystemTimeUpdated = FALSE;
    
      /* Call the scheduler, which checks if timers are elapsed */
      KERNEL_vScheduler();
    }
  
    /* Find first event which is pending --> itterate through complete pending
       EventList */
    for (teEventIndex=(TE_EVENT_ID)0; teEventIndex<EVENTID_LAST; teEventIndex++)
    {
      /* atsPendingEventList is a SHARED RESOURCE */
      if (atsPendingEventList[teEventIndex].ubCount > KERNEL_EVENT_IS_NOT_PENDING)
      /* atsPendingEventList is a SHARED RESOURCE */
      {
        /*Decrease pending event counter
          atsPendingEventList[teEventIndex].ubCount--;
        */
        atsPendingEventList[teEventIndex].ubCount = KERNEL_EVENT_IS_NOT_PENDING;
  
        /* Search if this 'pending Event' is subscribed */
        for(ubIndex=0; ubIndex<KERNEL_SUB_SCR_LIST_SIZE; ubIndex++)
        {
          if (atsSubcribedEventList[ubIndex].teEventID == teEventIndex)
          {
            /* Event is subscribed --> check function pointer ID */
            assert(atsSubcribedEventList[ubIndex].teFunctionID < FUNCTIONID_LAST);
            if (atsSubcribedEventList[ubIndex].teFunctionID < FUNCTIONID_LAST)
            {
              /* Keep record of some statistics (if enabled) */
              #ifdef KERNEL_STATISTICS
                T_SYSTEM_TIME   tStartExecuteTime;
                
                tStartExecuteTime = (tSystemTime >> KERNEL_TIMER_SHIFT_FACTOR);
                atsPendingEventList[teEventIndex].ulExecuteCount++;
                atsPendingEventList[(UBYTE)teEventIndex].tStartTime =
                                                         tStartExecuteTime;
              #endif

              /* Remove event from Subscribed list first
                 atsSubcribedEventList[ubIndex].teEventID = EVENTID_LAST;
  
                Execute the function pointer, the event ID is passed as parameter
                indicate which event was pending.
              */
               HAL_vRestoreCPUTestPin();              
              #ifdef KERNEL_DATA_PARAM
               apfnKernelFunctionList[atsSubcribedEventList[ubIndex].teFunctionID]
                                (teEventIndex,
                                 atsPendingEventList[teEventIndex].ulData);
              #else
               apfnKernelFunctionList[atsSubcribedEventList[ubIndex].teFunctionID]
                                (teEventIndex);
              #endif
  
              /* Keep record of some statistics (if enabled) */
              #ifdef KERNEL_STATISTICS
                atsPendingEventList[teEventIndex].tExecuteTime =
                  (tSystemTime >> KERNEL_TIMER_SHIFT_FACTOR) - tStartExecuteTime;
              #endif
              
            }
          }
        }
      }
    }
  }
}

/*
** Description: Returns the current system-time
** Parameters:  void
** Returns:     T_SYSTEM_TIME: the current system-time
*/
T_SYSTEM_TIME KERNEL_tGetSystemTime(void)
{
  /* System time is increased with 2 (for each timer tick)
     Return the "real" time here by dividing t first by '2'
  */
  T_SYSTEM_TIME tSystemTimeCopy;

  KERNEL_vEnterCriticalCode();

  tSystemTimeCopy=tSystemTime;

  KERNEL_vLeaveCriticalCode();

  return (tSystemTimeCopy >> KERNEL_TIMER_SHIFT_FACTOR);
}


/*
** Description: Returns the lowest byte of the current system-time 
**                    As only the LSB is needed, no overflow risk so no CC change needed and safe to be called inside interrupt
** Parameters:  void
** Returns:     UBYTE: LSB of the current system-time
*/
UBYTE KERNEL_ubGetSystemTimeLSB(void)
{
  /* System time is increased with 2 (for each timer tick)
     Return the "real" time here by dividing t first by '2'
  */
  return ((UBYTE)(((USHORT)tSystemTime) >> KERNEL_TIMER_SHIFT_FACTOR));
}


#if (TARGET_DEVICE == DEVICE_X86)
  BOOL KERNEL_blGetFlagSystemTimeUpdated(void)
  /*
  ** Description: Returns the Flag if the system time is updated, this
  **              is used in mock based testing only
  ** Parameters:  void
  ** Returns:     BOOL: Flag system time is updates
  */
  {
    return (blFlagSystemTimeUpdated);
  }
#endif

#ifdef KERNEL_STATISTICS
  #ifdef KERNEL_DATA_PARAM
    void KERNEL_vPrintTaskList(TE_EVENT_ID teEventId, ULONG ulData)    
  #else
    void KERNEL_vPrintTaskList(TE_EVENT_ID teEventId)
  #endif
  /*
  ** Description: Kernel statistics funtion. This function (if enabled) prints
  **              the kernel status.
  ** Parameters:  teEventId: This function is called by the kernel, this 
  **              parameter is the event which causes this function to be
  **              executed.
  ** Returns:     void
  */
  {
    TE_EVENT_ID       teEventIndex;
    TS_PENDING_EVENT  *ptsEvent;

    print("Systemtime: %d\n", (tSystemTime >> KERNEL_TIMER_SHIFT_FACTOR));

    /* Print complete Timer list */
    print("Timer:                Sche,  Exec ExeTime= tStart= Count= T=\n");
    for ( teEventIndex=(TE_EVENT_ID)0; teEventIndex<TIMERID_LAST;
                                                      teEventIndex++)
    {
      ptsEvent = &atsPendingEventList[(UBYTE)teEventIndex];

      print("%2d %-15s %5d,%5d   %3d, %8d%5d      %2d\n",
        (UBYTE)teEventIndex,
        tsLabels[(UBYTE)teEventIndex],
        ptsEvent->ulScheduleCount,
        ptsEvent->ulExecuteCount,
        ptsEvent->tExecuteTime,
        atsPendingEventList[(UBYTE)teEventIndex].tStartTime,
        atsPendingEventList[(UBYTE)teEventIndex].ubCount,
        atTimerTriggerTimes[(UBYTE)teEventIndex]);
    }

    /* Print Event list */
    print("Event:                Sche,  Exec ExeTime= tStart= Count=\n");
    for ( teEventIndex=(TE_EVENT_ID)TIMERID_LAST; teEventIndex<EVENTID_LAST;
                                                       teEventIndex++)
    {
      ptsEvent = &atsPendingEventList[(UBYTE)teEventIndex];
      print("%2d %-15s %5d,%5d   %3d, %8d%5d\n",
        (UBYTE)teEventIndex,
        tsLabels[(UBYTE)teEventIndex],
        ptsEvent->ulScheduleCount,
        ptsEvent->ulExecuteCount,
        ptsEvent->tExecuteTime,
        atsPendingEventList[(UBYTE)teEventIndex].tStartTime,
        atsPendingEventList[(UBYTE)teEventIndex].ubCount);
    }
  }
#endif

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

static void KERNEL_vScheduler(void)
/*
** Description: Kernel scheduler. The scheduler is executed if the systemtime 
**              is increased. A timer event ID is posted if the timer is
**              elapsed.
** Parameters:  void
** Returns:     void
*/
{
  TE_TIMER_ID   teTimerIndex;
  T_SYSTEM_TIME tCopySystemTime, tTriggerTime;

  /* Make a copy of the SystemTime (SystemTime is used in IRQ)
     Mask out the running bit (tSystemTime.0)
     tSystemTime.0 = 0 --> timer is stopped
     tSystemTime.0 = 1 --> timer is running
  */
  KERNEL_vEnterCriticalCode();

  tCopySystemTime = tSystemTime;

  KERNEL_vLeaveCriticalCode();
  /* Find first timer which is elapsed --> itterate through complete TimerList*/
  for (teTimerIndex=(TE_TIMER_ID)0; teTimerIndex<TIMERID_LAST; teTimerIndex++)
  {
    tTriggerTime = atTimerTriggerTimes[(UBYTE)teTimerIndex];

    /* Is timer running ? */
    if (tTriggerTime & KERNEL_TIMER_RUNS)
    {
      /* Is timer TriggerTime elapsed ? 
         --> typecast to signed value of T_SYSTEM_TIME */
      if ((T_SYSTEM_TIME_SIGNED)(tTriggerTime - tCopySystemTime) <= 0)
      {
        /* TriggerTime is elapsed --> Stop the timer */
        atTimerTriggerTimes[(UBYTE)teTimerIndex] = KERNEL_TIMER_STOPPED;
          
        /* Post timer event (timer event is added to pending event list) */
        #ifdef KERNEL_DATA_PARAM
          KERNEL_vPostEvent((TE_EVENT_ID)teTimerIndex, 0);
        #else
          KERNEL_vPostEvent((TE_EVENT_ID)teTimerIndex);
        #endif
      }
    }
  }
}

static void KERNEL_vCallback(void)
/*
** Description: Callback function for the kernel driver. This function is  
**              called if the kernel timer interrupts.
** Parameters:  The Systemtime is increased with 1 tick, a flag is set to
**              indicate that the Systemtime is updated.
** Returns:     void
*/
{
  
  /* Increment systemtimer with 'KERNEL_TIMER_INCREASE_COUNT', tSystemTime.0
     is used to indicate if timer is running
  */
#if ((TARGET_DEVICE == DEVICE_BASICVALUE)||(TARGET_DEVICE == DEVICE_SUPERVALUE))
  if(!blKernelUpdateBlocked)
#endif
  {
    tSystemTime += KERNEL_TIMER_INCREASE_COUNT;

    /* Flag to indicate the scheduler that the systemtime is updated */
    blFlagSystemTimeUpdated = TRUE;
  }

}

static UBYTE KERNEL_ubSearchSubscribedEvent(const TE_EVENT_ID teEventID,
                                     const TE_FUNCTION_ID teFunctionID)
/*
** Description: Searches for a subscribed event in the Subcribed Event List.
** Parameters:  teEventID:  Event index to search (created by kernel_cfg.h)
**              teFunctionID: Function pointer to search (kernel_cfg.h creates
**              a list of function pointers)
** Returns:     Position (index) of event inside the Subcribed Event List OR
**              "KERNEL_SUB_SCR_LIST_SIZE" if event is not found in list.
*/
{
  UBYTE ubIndex;

  assert(teEventID < EVENTID_LAST);

  for(ubIndex=0; ubIndex<KERNEL_SUB_SCR_LIST_SIZE; ubIndex++)
  {
    if (atsSubcribedEventList[ubIndex].teEventID == teEventID)
    {
      if (atsSubcribedEventList[ubIndex].teFunctionID == teFunctionID)
      {
        /* Event found in list, so exit for loop --> return current index in
           list */
        break;
      }
    }
  }

  return (ubIndex);
}

static UBYTE KERNEL_ubFindFreeEventSlot(void)
/*
** Description: Searches for a free slot in the Subcribed Event List.
** Parameters:  void
** Returns:     Position (index) of free slot inside the Subcribed Event List 
**              OR "KERNEL_SUB_SCR_LIST_SIZE" if list is FULL.
*/
{
  UBYTE ubIndex;

  for(ubIndex=0; ubIndex<KERNEL_SUB_SCR_LIST_SIZE; ubIndex++)
  {
    if (atsSubcribedEventList[ubIndex].teEventID == EVENTID_LAST)
    {
      /* Empty slot found in list, so exit for loop --> return current index in
         list */
      break;
    }
  }

  return (ubIndex);
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

