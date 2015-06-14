/******************************************************************************/
/*                                                                            */
/*    $Workfile::   queue.c                                              $    */
/*                                                                            */
/*    $Revision:: 2234                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2010-11-11 16:05:58 +0800 (周四, 11 十一月 2010)  $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
/*                                                                            */
/*        Owner::                                                             */
/*                                                                            */
/*  Description::   This queue module provides FIFO functionality.            */
/*                  Bytes can be added to the queue using the QUEUE_teWrite() */
/*                  function and be read with the QUEUE_teRead() function.    */
/*                  The QUEUE_vFree() can be used to delete bytes from the    */
/*                  queue. The effective queue size is the buffer size - 1,   */
/*                  since one byte is used for head & tail logic.             */
/*                                                                            */
/*   Department::   B.G. Lighting Electronics                                 */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2006 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

/*! \file
 * \brief Queue utility.
 *
 * This queue module provides FIFO functionality.
 * Bytes can be added to the queue using the QUEUE_teWrite()
 * function and be read with the QUEUE_teRead() function, note that
 * the read function also removes data from the queue. In order
 * to leave the data on the queue use the QUEUE_tePeek() function.
 * The QUEUE_vFree() can be used to delete bytes from the
 * queue. The effective queue size is the buffer size - 1,
 * since one byte is used for head & tail logic.
 *
 * Example:
 * \code
 * QUEUE_vInit();
 *
 * //Retrieve data using teRead:
 * if (QUEUE_teRead(QUEUEID_0, (UBYTE*)&m_ubBuffer, 1) == RESULT_SUCCESS)
 * {
 *   (void)QUEUE_teWrite(QUEUEID_1, &m_ubBuffer, 1);
 * }
 *
 * //Retrieve data using tePeek/vFree:
 * if (QUEUE_tePeek(QUEUEID_0, (UBYTE*)&m_ubBuffer, 1) == RESULT_SUCCESS)
 * {
 *   (void)QUEUE_teWrite(QUEUEID_1, &m_ubBuffer, 1);
 *   QUEUE_vFree(QUEUEID_0, 1);
 * }
 * \endcode
 */

/*! \defgroup Utilities_Queue Queue utility module
 *  Queue utility.
 *  \par Introduction
 *  The Queue is an RSA module providing a standard queue for other modules/
 *  drivers. It's possible to create multiple Queues, with different buffer
 *  sizes per queue. The module has a small ROM and RAM footprint. Queues are
 *  created at design time inside the queue configuration file 'queue_cfg.h'.
 *  The module creates a new instance of each queue (head and tail indexes, an
 *  "used space" counter and a buffer in RAM). The module is platform
 *  independent. No hardware related peripherals are used in this module.
 *  \par Context
 *  \image html queue_context.png "Figure 1 - Queue module context diagram"
 *  \par Detailed design
 *  The Queue buffer is implemented as an array, with each element storing one
 *  byte of data. The 'head' (=write) and 'tail' (=read) indexes keep track of
 *  buffer occupancy. The 'ring' is formed by allowing the indexes to 'wrap'
 *  around the end of the array back to the beginning. This is useful since
 *  these buffers are typically written to and read from at somewhat random
 *  times. Figure 2 shows an example of a queue.
 *  \par Writing to the queue:
 *  The head index points to the current write location (the next byte is
 *  written to this location). The head index is increased for each byte after
 *  the byte is written to the queue buffer; see Figure 2 for an example of a
 *  write action. The head index wraps if the end of the array is. The free
 *  space inside the buffer is calculated before writing the data into the
 *  buffer. The number of bytes written is added to the used space counter after
 *  a successful write action. Error "RESULT_BAD_PARAM" is returned if the data
 *  doesn't fit in the buffer (NO data is written). "RESULT_SUCCESS" is returned
 *  if the write action succeeds.
 *  The free space in the buffer is calculated with following calculation:
 *  \code
 *  Free space = size of buffer - used space
 *  \endcode
 *  \image html queue_design_write.png "Figure 2 - Queue write action"
 *  \par Peeking the queue
 *  The tail index points to the current read location (the next byte is peeked
 *  from this location). A temporary version of the tail index is used to peek
 *  data from the buffer. This temporary version is increased for each byte that
 *  is peeked from the buffer; see Figure 3 for an example of a peek action. The
 *  temporary version of the tail index wraps if the end of the array is reached
 *  (see Figure 5). The temporary version of the tail index cannot cross the
 *  head index (there is no more data in buffer). The tail position doesn't move
 *  with a peek action, so when a 2nd peek is performed, the same data is
 *  returned. "RESULT_ERROR" is returned if there are more bytes requested to
 *  peek than there are actual in the buffer. "RESULT_SUCCESS" is returned if
 *  the peek action succeeds.
 *  \image html queue_design_peek.png "Figure 3 - Queue peek action"
 *  \par Freeing the queue:
 *  The tail index is increased for each byte that is freed from the buffer; see
 *  Figure 4 for an example of a free action. The tail index wraps if the end of
 *  the array is reached  (see Figure 5). The data is not removed from the
 *  buffer, only the tail index is increased. A peek action gets "new" data from
 *  the buffer (if there is any data located in the buffer). The complete buffer
 *  is marked as empty if more bytes are requested to free than actually are
 *  inside the buffer; no ERROR is returned in this situation. The number of
 *  bytes freed is subtracted from the used space counter.
 *  \image html queue_design_free.png "Figure 4 - Queue free action"
 *  \par Reading the queue:
 *  A Queue read action first copies the number of specified bytes from the
 *  queue into the allocated buffer (same as the peeking the queue function).
 *  The number of bytes is freed afterwards, when the read was successful
 *  (same as the freeing the queue function).
 *  \image html queue_design_read.png "Figure 5 - Wrapping of tail and head indexes"
 *  \par Defining Queues:
 *  Queues are defined (design time) inside the configuration file
 *  "queue_cfg.h". The user defines the queue size per Queue (QUEUE_SIZE_XXX)
 *  inside this file. Using a queue size of 0 for all not used queues is
 *  mandatory. See the following piece of code (only QueueIDs 0, 1, 2 and 5 are
 *  in use):
 *  \code
 *  #define QUEUE_SIZE_0        (16)
 *  #define QUEUE_SIZE_1        (16)
 *  #define QUEUE_SIZE_2        (64)
 *  #define QUEUE_SIZE_3        (0)
 *  #define QUEUE_SIZE_4        (0)
 *  #define QUEUE_SIZE_5        (32)
 *  #define QUEUE_SIZE_6        (0)
 *  #define QUEUE_SIZE_7        (0)
 *  \endcode
 *  An enumeration list of valid QueueIDs is generated inside the file queue.h.
 *  (The enumeration only consists of QueueIDs which has a size > 0). The
 *  following enum is generated (with 4 queues in use):
 *  \code
 *  typedef enum
 *  {
 *    QUEUEID_0,
 *    QUEUEID_1,
 *    QUEUEID_2,
 *    QUEUEID_5,
 *  } TE_QUEUE_ID;
 *  \endcode
 *  \par The queue buffer:
 *  1 buffer (1 big array) is used for all queues. Each individual queue buffer
 *  gets a start index and a queue size. The start index per queue indicates the
 *  start inside the buffer for that queue; see Figure 6 for an example of 3
 *  queue buffers located in 1 buffer. The advantage of placing all queue
 *  buffers inside 1 buffer is code-size. Only a list of start indexes is needed
 *  to read/write data from/to the specific queue.
 *  \image html queue_design_buffer.png "Figure 6 - Queue buffer"
 *  Start index list:
 *  The queue module uses 1 buffer for all defined queues. Each queue has a
 *  buffer size and a start index. This index indicates the location where the
 *  buffer for this queue starts (inside the complete queue buffer). A list of
 *  index is created in queue.c with the following line:
 *  \code
 *  const T_BUFFER_INDEX atStartIndex[ACTUAL_NR_OF_QUEUES + 1] = START_INDEXES;
 *  \endcode
 *  The array 'atStartIndex' contains a list of start indexes for each queue.
 *  The last value of this list should always contain value 'QUEUE_BUF_SIZE'.
 *  \code
 *  #define START_INDEX_0       (0)
 *  #define START_INDEX_1       (START_INDEX_0 + QUEUE_SIZE_0)
 *  #define START_INDEX_2       (START_INDEX_1 + QUEUE_SIZE_1)
 *  #define START_INDEX_3       (START_INDEX_2 + QUEUE_SIZE_2)
 *  #define START_INDEX_4       (START_INDEX_3 + QUEUE_SIZE_3)
 *  #define START_INDEX_5       (START_INDEX_4 + QUEUE_SIZE_4)
 *  #define START_INDEX_6       (START_INDEX_5 + QUEUE_SIZE_5)
 *  #define START_INDEX_7       (START_INDEX_6 + QUEUE_SIZE_6)
 *  #define QUEUE_BUF_SIZE   (START_INDEX_7 + QUEUE_SIZE_7)
 *
 *  // Create an constant array with the start indexes of each queue buffer
 *  static TARGET_MEM_ROM const T_BUFFER_INDEX atStartIndex[] =
 *  {
 *  #if QUEUESIZE_0 > 0
 *    START_INDEX_0,
 *  #endif
 *  #if QUEUESIZE_1 > 0
 *    START_INDEX_1,
 *  #endif
 *  #if QUEUESIZE_2 > 0
 *    START_INDEX_2,
 *  #endif
 *  #if QUEUESIZE_3 > 0
 *    START_INDEX_3,
 *  #endif
 *  #if QUEUESIZE_4 > 0
 *    START_INDEX_4,
 *  #endif
 *  #if QUEUESIZE_5 > 0
 *    START_INDEX_5,
 *  #endif
 *  #if QUEUESIZE_6 > 0
 *    START_INDEX_6,
 *  #endif
 *  #if QUEUESIZE_7 > 0
 *    START_INDEX_7,
 *  #endif
 *
 *    //Always end with the last index (= size of buffer)
 *    QUEUE_BUF_SIZE
 *  };
 *  \endcode
 *  All indexes used for the separate queue buffers are 8-bit if the total Queue
 *  buffer size (the buffer used for all queues together) doesn't exceed 255
 *  bytes. 16-bit indexes are used (increasing the code size) if the total queue
 *  size exceeds 255.
 *  \code
 *  #if (QUEUE_BUF_SIZE <= 255)
 *    // Create 8-bit index (if buffer size <= 255)
 *    typedef UBYTE T_BUFFER_INDEX;
 *  #else
 *    // Create 16-bit index (if buffer size > 255)
 *    typedef USHORT T_BUFFER_INDEX;
 *  #endif
 *  \endcode
 *  \par Buffer sizes:
 *  A list of buffer sizes is not used in the code (code optimization). Each
 *  buffer size is calculated in the code by using the subtracting the start
 *  index of the next queue with the start index of the used queue. (Therefore
 *  the element TOTAL_QUEUE_SIZE is appended to the list, see Start index list.)
 *  \code
 *  ubSize = (UBYTE)(atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID]);
 *  \endcode
 *  \par Maximum pre-defined queues in code:
 *  The code supports a maximum of 8 queues at the moment of writing. It's
 *  possible to extend this number easily. Extra entries must be added to the
 *  array 'atStartIndex' (queue.c) and entries must be added to the enum
 *  TE_QUEUE_ID (queue.h).
 *  \par Multiple Queue Interface:
 *  Each interface function contains a teQueueID parameter of type TE_QUEUE_ID.
 *  This parameter is only in use if more than 1 QueueID is defined. The
 *  parameter teQueueID is not used if only 1 queue is defined (only queue
 *  QUEUEID_0 is accessed). This setting must be done with the define
 *  'QUEUE_INTERFACE' in file 'queue_cfg.h'.
 *
 *  @{
 */

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/

#include <string.h>

#include "platform.h"
#include "stdtypes.h"

#include "queue.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#if QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/* Each interface function of the queue module uses 'teQueueID' as parameter.
   This parameter is forced to use queue '0' if a single queue is used.
*/
 #define teQueueID    (QUEUEID_SINGLE)
#endif

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define NR_OF_QUEUES  ((sizeof(atStartIndex) / sizeof(T_BUFFER_INDEX)) - 1)

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/
#if (QUEUE_BUF_SIZE <= 255)
  /* Create 8-bit index (if buffer size <= 255) */
  typedef UBYTE T_BUFFER_INDEX;
#else
  /* Create 16-bit index (if buffer size > 255) */
  typedef USHORT T_BUFFER_INDEX;
#endif

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
/* Create an constant array with the start indexes of each queue buffer */
static TARGET_MEM_ROM const T_BUFFER_INDEX atStartIndex[] =
{
#if QUEUESIZE_0 > 0
  START_INDEX_0,
#endif
#if QUEUESIZE_1 > 0
  START_INDEX_1,
#endif
#if QUEUESIZE_2 > 0
  START_INDEX_2,
#endif
#if QUEUESIZE_3 > 0
  START_INDEX_3,
#endif
#if QUEUESIZE_4 > 0
  START_INDEX_4,
#endif
#if QUEUESIZE_5 > 0
  START_INDEX_5,
#endif
#if QUEUESIZE_6 > 0
  START_INDEX_6,
#endif
#if QUEUESIZE_7 > 0
  START_INDEX_7,
#endif

  //Always end with the last index (= size of buffer)
  QUEUE_BUF_SIZE
};

/* Create 1 buffer for all Queues */
static UBYTE aubQueueBuf[QUEUE_BUF_SIZE];
/* Create an array of Head indexes for each Queue */
static UBYTE aubHead[NR_OF_QUEUES];
/* Create an array of Tail indexes for each Queue */
static UBYTE aubTail[NR_OF_QUEUES];
/* Create an array of Bytes in buffer counters for each Queue */

/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
void QUEUE_vInit(void)
/*
** Description: Initialises the Queue module
** Parameters:  void
** Returns:     void
*/
/* This function not needed for a normal application, everything is initialized
    by the compiler !! */
{
  /* Clears all internal Queue variables (head, tail and BytesinBuffer),
     This is only used for testing with the "Test Suite project"
  */
  (void)memset(aubHead,       0, sizeof(aubHead));
  (void)memset(aubTail,       0, sizeof(aubTail));
}



#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_teWrite_Multi(const TE_QUEUE_ID teQueueID,
                           const void *pvData,
                           UBYTE ubNrOfBytes)
/*
** Description: Adds the specified number of bytes to the specified queue
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
**              pvData: Pointer to the data to be added to the queue
**              ubNrOfBytes: Number of bytes to be added to the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**               "RESULT_SUCCESS" if bytes are added to the queue successfully
**               "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the
**               bytes are added to the queue.
*/
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_teWrite_Single(const void *pvData,
                           UBYTE ubNrOfBytes)
/*
** Description: Adds the specified number of bytes to the queue (QUEUEID_0)
** Parameters:  pvData: Pointer to the data to be added to the queue
**              ubNrOfBytes: Number of bytes to be added to the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**               "RESULT_SUCCESS" if bytes are added to the queue successfully
**               "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the
**               bytes are added to the queue.
*/
#endif
{
  const UBYTE *pubData = pvData;
  UBYTE       ubSize;

  QUEUE_ASSERT(teQueueID < QUEUEID_LAST);
  QUEUE_ASSERT(pubData);

  /* Get Size of this Queue buffer (Use index of this queue and next queue) */
  ubSize = (UBYTE)((atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID])-1);

  /* Check for free space in buffer */
  if ((ubSize-((aubHead[teQueueID]-aubTail[teQueueID]) & ubSize))< ubNrOfBytes)
  {
    /* Not enough space left in FIFO buffer */
    return (RESULT_BAD_PARAM);
  }

  while (ubNrOfBytes--)
  {
    /* Wrap? */
    aubHead[teQueueID] &= ubSize;

    /* Write byte to head of buffer */
    aubQueueBuf[atStartIndex[teQueueID] + aubHead[teQueueID]] = *pubData;
    pubData++;

    /* Increment head AFTER adding bytes */
    aubHead[teQueueID]++;
  }

  /* Bytes added successfully */
  return (RESULT_SUCCESS);
}

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_tePeek_Multi(const TE_QUEUE_ID teQueueID,
                          void *pvData,
                          UBYTE ubNrOfBytes)
/*
** Description: Reads the specified number of bytes from the specified queue.
**               Bytes are not deleted from the queue during a read.
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
**              pvData: Pointer to a buffer where read data is to be stored
**              ubNrOfBytes: Number of bytes to read from the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**                "RESULT_SUCCESS" if bytes are read from the queue successfully
**                "RESULT_ERROR" if the specified number of bytes cannot be read
**                from the queue.
*/
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_tePeek_Single(void *pvData,
                          UBYTE ubNrOfBytes)
/*
** Description: Reads the specified number of bytes from the queue (QUEUEID_0)
**              Bytes are not deleted from the queue during a read.
** Parameters:  pvData: Pointer to a buffer where read data is to be stored
**              ubNrOfBytes: Number of bytes to read from the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**                "RESULT_SUCCESS" if bytes are read from the queue successfully
**                "RESULT_ERROR" if the specified number of bytes cannot be read
**                from the queue.
*/
#endif
{
  UBYTE *pubData = pvData;
  UBYTE ubReadTail;
  UBYTE ubSize;

  QUEUE_ASSERT(teQueueID < QUEUEID_LAST);
  QUEUE_ASSERT(pubData);

  /* Get Size of this Queue buffer (Use index of this queue and next queue) */
  ubSize = (UBYTE)((atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID])-1);
  /* Copy tail location */
  ubReadTail = aubTail[teQueueID] ;
   
  if (ubNrOfBytes > ((aubHead[teQueueID]-ubReadTail) & ubSize))
  {
    /* Amount of bytes not in buffer */
    return (RESULT_ERROR);
  }

  /* Get a temp. tail location */
  while (ubNrOfBytes--)
  {
    /* Copy data from buffer */
    ubReadTail &= ubSize;

    *pubData = aubQueueBuf[atStartIndex[teQueueID] + ubReadTail];
    pubData++;
    ubReadTail++;
  }
  return (RESULT_SUCCESS);
}

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_teRead_Multi(const TE_QUEUE_ID teQueueID,
                                 void *pvData,
                                 UBYTE ubNrOfBytes)
/*
** Description: Reads the specified number of bytes from the specified queue
**               and deletes them from the queue.
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
**              pvData: Pointer to a buffer where read data is to be stored
**              ubNrOfBytes: Number of bytes to read and free from the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**                "RESULT_SUCCESS" if bytes are read and deleted from the queue
**                successfully
**                "RESULT_ERROR" if the specified number of bytes cannot be read
**                from the queue.
*/
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
 TE_RESULT QUEUE_teRead_Single(void *pvData, UBYTE ubNrOfBytes)
/*
** Description: Reads the specified number of bytes from the queue (QUEUEID_0)
**               and deletes them from the queue.
** Parameters:  pvData: Pointer to a buffer where read data is to be stored
**              ubNrOfBytes: Number of bytes to read and free from the queue
** Returns:     TE_RESULT code from enumerated type TE_RESULT:
**                "RESULT_SUCCESS" if bytes are read and deleted from the queue
**                successfully
**                "RESULT_ERROR" if the specified number of bytes cannot be read
**                and freed from the queue.
*/
#endif
{
  UBYTE *pubData = pvData;
  UBYTE ubReadTail;
  UBYTE ubSize;

  QUEUE_ASSERT(teQueueID < QUEUEID_LAST);
  QUEUE_ASSERT(pubData);

  /* Get Size of this Queue buffer (Use index of this queue and next queue) */
  ubSize = (UBYTE)((atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID])-1);

  /* Get temp. copy of Tail index */
  ubReadTail = aubTail[teQueueID];

  if (ubNrOfBytes > ((aubHead[teQueueID]-ubReadTail) & ubSize))
  {
    /* Amount of bytes not in buffer */
    return (RESULT_ERROR);
  }


  while (ubNrOfBytes--)
  {
    /* Wrap Tail? */
    ubReadTail &= ubSize;
    /* Copy data from buffer */
    *pubData = aubQueueBuf[atStartIndex[teQueueID] + ubReadTail];
    pubData++;
    ubReadTail++;
  }
  aubTail[teQueueID] = ubReadTail;

  return (RESULT_SUCCESS);
}

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
void QUEUE_vFree_Multi(const TE_QUEUE_ID teQueueID, UBYTE ubNrOfBytes)
/*
** Description: Frees the specified number of bytes from the specified queue.
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
**              ubNrOfBytes: Number of bytes to free from the queue
** Returns:     void
*/
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
 void QUEUE_vFree_Single(UBYTE ubNrOfBytes)
/*
** Description: Frees the specified number of bytes from the queue (QUEUEID_0)
** Parameters:  ubNrOfBytes: Number of bytes to free from the queue
** Returns:     void
*/
#endif
{
  UBYTE ubSize;
  UBYTE ubBytesInQueue;

  QUEUE_ASSERT(teQueueID < QUEUEID_LAST);

  /* Get Size of this Queue buffer (Use index of this queue and next queue) */
  ubSize = (UBYTE)((atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID])-1);


  ubBytesInQueue = (UBYTE)((aubHead[teQueueID]-aubTail[teQueueID]) & ubSize);

  if (ubNrOfBytes > ubBytesInQueue)
  {
    /* Amount of bytes not in buffer --> clip number of bytes */
    ubNrOfBytes = ubBytesInQueue;
  }

  /* Free specified number of bytes from queue */
  aubTail[teQueueID] = (UBYTE)((aubTail[teQueueID] + ubNrOfBytes) & ubSize);

}

#if 0
UBYTE QUEUE_ubGetFreeSpace(const TE_QUEUE_ID teQueueID)
{
  UBYTE ubSize;
  /* Get Size of this Queue buffer (Use index of this queue and next queue) */
  ubSize = (UBYTE)(atStartIndex[(UBYTE)teQueueID+1] - atStartIndex[teQueueID])-1;

  return (aubHead[teQueueID]-aubTail[teQueueID]) & ubSize);
}
#endif

/* Functionality for the Test Suite */
#if (TARGET_DEVICE == DEVICE_X86)
 #if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
  UBYTE* QUEUE_pubGetQueueBufferPointer(const TE_QUEUE_ID teQueueID,
                                    UBYTE *pubBufSize)
/*
** Description: Returns the pointer and the size of the requested (teQueueID)
                 Queue buffer
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
** Returns:     **pubQueueBuf: pointer to requested Queue buffer
                *pubBufSize: size of requested Queue buffer
*/
 #elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
  UBYTE* QUEUE_pubGetQueueBufferPointer(UBYTE *pubBufSize)
/*
** Description: Returns the pointer and the size of the requested (teQueueID)
                 Queue buffer
** Parameters:  teQueueID: Id (type enum TE_QUEUE_ID) of used queue
** Returns:     **pubQueueBuf: pointer to requested Queue buffer
                *pubBufSize: size of requested Queue buffer
*/
 #endif
{
  QUEUE_ASSERT(teQueueID < QUEUEID_LAST);

  *pubBufSize = atStartIndex[teQueueID+1] - atStartIndex[teQueueID];
  return &(aubQueueBuf[atStartIndex[teQueueID]]);
}
#endif

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/*! @} */

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

