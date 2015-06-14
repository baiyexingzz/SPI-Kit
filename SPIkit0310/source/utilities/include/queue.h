/******************************************************************************/
/*                                                                            */
/*    $Workfile::   queue.h                                              $    */
/*                                                                            */
/*    $Revision:: 2906                                                   $    */
/*                                                                            */
/*     $Archive::   P:/ARCLE_SW/Reusable SW Architecture/archives/Utiliti$    */
/*                                                                            */
/*     $Modtime::   Sep 11 2007 11:06:36                                 $    */
/*                                                                            */
/*        $Date:: 2011-08-01 14:24:43 +0800 (周一, 01 八月 2011)     $    */
/*                                                                            */
/*      $Author:: kyleye                                                 $    */
/*                                                                            */
/*        Owner::   Wijnand Rietman                                           */
/*                                                                            */
/*  Description::   This queue module provides circular buffer functionality. */
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
 *  \brief Queue utility.
 */

/*! \addtogroup Utilities_Queue
 *  @{
 */

#ifndef QUEUE_H
#define QUEUE_H

#ifdef __cplusplus
  extern "C" {
#endif

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
/*>RELAX<CodingConvention_10120_HeaderInclude> */
#include "queue_cfg.h"

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* Calculate the start indexes for each Queue buffer */
#define START_INDEX_0       (0)
#define START_INDEX_1       (START_INDEX_0 + QUEUESIZE_0)
#define START_INDEX_2       (START_INDEX_1 + QUEUESIZE_1)
#define START_INDEX_3       (START_INDEX_2 + QUEUESIZE_2)
#define START_INDEX_4       (START_INDEX_3 + QUEUESIZE_3)
#define START_INDEX_5       (START_INDEX_4 + QUEUESIZE_4)
#define START_INDEX_6       (START_INDEX_5 + QUEUESIZE_5)
#define START_INDEX_7       (START_INDEX_6 + QUEUESIZE_6)
#define QUEUE_BUF_SIZE      (START_INDEX_7 + QUEUESIZE_7)

#if START_INDEX_0 == QUEUE_BUF_SIZE
  #error "0 queues are defined !!!"
#endif

#define SINGLE_QUEUE_INTERFACE    (1)
#define MULTIPLE_QUEUE_INTERFACE  (2)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
 /*! Function like macro that adds the specified number of bytes to the specified queue.
     \a teQueueID Id (type enum TE_QUEUE_ID) of used queue. \a pubData Pointer to the data
     to be added to the queue. \a ubNrOfBytes Number of bytes to be added to the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are added to the queue successfully.
     "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the.
     bytes are added to the queue.
  */
  #define QUEUE_teWrite(teQueueID, pubData, ubNrOfBytes)  \
                      QUEUE_teWrite_Multi(teQueueID, pubData, ubNrOfBytes)
 /*! Function like macro that reads the specified number of bytes from the specified queue.
     Bytes are not deleted from the queue during a read.
     \a teQueueID Id (type enum TE_QUEUE_ID) of used queue.
     \a pubData Pointer to a buffer where read data is to be stored.
     \a ubNrOfBytes Number of bytes to read from the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
      "RESULT_SUCCESS" if bytes are read from the queue successfully
      "RESULT_ERROR" if the specified number of bytes cannot be read
      from the queue.
  */
  #define QUEUE_tePeek(teQueueID, pubData, ubNrOfBytes) \
                      QUEUE_tePeek_Multi(teQueueID, pubData, ubNrOfBytes)
 /*! Function like macro that reads the specified number of bytes from the specified queue
      and deletes them from the queue.
     \a teQueueID Id (type enum TE_QUEUE_ID) of used queue.
     \a pubData Pointer to a buffer where read data is to be stored.
     \a ubNrOfBytes Number of bytes to read and free from the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
      "RESULT_SUCCESS" if bytes are read and deleted from the queue
      successfully
      "RESULT_ERROR" if the specified number of bytes cannot be read
      from the queue.
  */
  #define QUEUE_teRead(teQueueID, pubData, ubNrOfBytes)  \
                      QUEUE_teRead_Multi(teQueueID, pubData, ubNrOfBytes)
 /*! Function like macro that frees the specified number of bytes from the specified queue.
     \a teQueueID Id (type enum TE_QUEUE_ID) of used queue.
     \a ubNrOfBytes Number of bytes to free from the queue.
     Returns void.
  */
  #define QUEUE_vFree(teQueueID, ubNrOfBytes)  \
                      QUEUE_vFree_Multi(teQueueID, ubNrOfBytes)

#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
 /*! Function like macro that adds the specified number of bytes to QUEUEID_0 queue.
     \a teQueueID will be ignored, must be QUEUEID_0. \a pubData Pointer to the data
     to be added to the queue. \a ubNrOfBytes Number of bytes to be added to the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are added to the queue successfully.
     "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the.
     bytes are added to the queue.
  */
  #define QUEUE_teWrite(teQueueID, pubData, ubNrOfBytes)  \
                      QUEUE_teWrite_Single(pubData, ubNrOfBytes)
 /*! Function like macro that reads the specified number of bytes from the QUEUEID_0 queue.
     Bytes are not deleted from the queue during a read.
     \a teQueueID will be ignored, must be QUEUEID_0.
     \a pubData Pointer to a buffer where read data is to be stored.
     \a ubNrOfBytes Number of bytes to read from the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
      "RESULT_SUCCESS" if bytes are read from the queue successfully
      "RESULT_ERROR" if the specified number of bytes cannot be read
      from the queue.
  */
  #define QUEUE_tePeek(teQueueID, pubData, ubNrOfBytes) \
                      QUEUE_tePeek_Single(pubData, ubNrOfBytes)
 /*! Function like macro that reads the specified number of bytes from the QUEUEID_0 queue
      and deletes them from the queue.
     \a teQueueID will be ignored, must be QUEUEID_0.
     \a pubData Pointer to a buffer where read data is to be stored.
     \a ubNrOfBytes Number of bytes to read and free from the queue.
     Returns TE_RESULT code from enumerated type TE_RESULT:
      "RESULT_SUCCESS" if bytes are read and deleted from the queue
      successfully
      "RESULT_ERROR" if the specified number of bytes cannot be read
      from the queue.
  */
  #define QUEUE_teRead(teQueueID, pubData, ubNrOfBytes)  \
                      QUEUE_teRead_Single(pubData, ubNrOfBytes)
 /*! Function like macro that frees the specified number of bytes from the QUEUEID_0 queue.
     \a teQueueID will be ignored, must be QUEUEID_0.
     \a ubNrOfBytes Number of bytes to free from the queue.
     Returns void.
  */
  #define QUEUE_vFree(teQueueID, ubNrOfBytes)  \
                      QUEUE_vFree_Single(ubNrOfBytes)
#endif

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/* Build an enum with QueueIDs (depending on the configured Queue sizes
   in queue_cfg.h) */
typedef enum
{
#if QUEUESIZE_0 > 0
  QUEUEID_0,
#endif
#if QUEUESIZE_1 > 0
  QUEUEID_1,
#endif
#if QUEUESIZE_2 > 0
  QUEUEID_2,
#endif
#if QUEUESIZE_3 > 0
  QUEUEID_3,
#endif
#if QUEUESIZE_4 > 0
  QUEUEID_4,
#endif
#if QUEUESIZE_5 > 0
  QUEUEID_5,
#endif
#if QUEUESIZE_6 > 0
  QUEUEID_6,
#endif
#if QUEUESIZE_7 > 0
  QUEUEID_7,
#endif

  //This dummy ID is added for the Cosmic compiler (complaining about the last ','
  // if less then 7 items are used
  QUEUEID_DUMMY
} TE_QUEUE_ID;

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                         PUBLIC FUNCTION PROTOTYPES                         */
/******************************************************************************/

/*! \brief Initialises the Queue module
    \return void

    This init function is only needed to re-init the queue module.
    All internal Queue variables (head / tail) are initialised
    automatically by the compiler (according ANSI C).
 */
void QUEUE_vInit(void);

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
/*! \brief Adds the specified number of bytes to the specified queue
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \param pvData Pointer to the data to be added to the queue
    \param ubNrOfBytes Number of bytes to be added to the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are added to the queue successfully
     "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the
     bytes are added to the queue.
 */
 TE_RESULT QUEUE_teWrite_Multi(const TE_QUEUE_ID teQueueID,
                                  const void *pvData,
                                  UBYTE ubNrOfBytes);
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/*! \brief Adds the specified number of bytes to the queue (using QUEUEID_0)
    \param pvData Pointer to the data to be added to the queue
    \param ubNrOfBytes Number of bytes to be added to the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are added to the queue successfully
     "RESULT_BAD_PARAM" if bytes do not fit in queue. None of the
     bytes are added to the queue.
 */
 TE_RESULT QUEUE_teWrite_Single(const void *pvData,
                                UBYTE ubNrOfBytes);
#endif

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
/*! \brief Reads the specified number of bytes from the specified queue.
     Bytes are not deleted from the queue during a read.
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \param pvData Pointer to a buffer where read data is to be stored
    \param ubNrOfBytes Number of bytes to read from the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are read from the queue successfully
     "RESULT_ERROR" if the specified number of bytes cannot be read
     from the queue.
 */
 TE_RESULT QUEUE_tePeek_Multi(const TE_QUEUE_ID teQueueID,
                                 void *pvData,
                                 UBYTE ubNrOfBytes);
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/*! \brief Reads the specified number of bytes from the queue (QUEUEID_0)
     Bytes are not deleted from the queue during a read.
    \param pvData Pointer to a buffer where read data is to be stored
    \param ubNrOfBytes Number of bytes to read from the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are read from the queue successfully
     "RESULT_ERROR" if the specified number of bytes cannot be read
     from the queue.
 */
 TE_RESULT QUEUE_tePeek_Single(void *pvData, UBYTE ubNrOfBytes);
#endif

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
/*! \brief Reads the specified number of bytes from the specified queue
     and deletes them from the queue.
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \param pvData Pointer to a buffer where read data is to be stored
    \param ubNrOfBytes Number of bytes to read and free from the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are read and deleted from the queue
     successfully
     "RESULT_ERROR" if the specified number of bytes cannot be read
     from the queue.
 */
 TE_RESULT QUEUE_teRead_Multi(const TE_QUEUE_ID teQueueID,
                                 void *pvData,
                                 UBYTE ubNrOfBytes);
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/*! \brief Reads the specified number of bytes from the queue (QUEUEID_0)
     and deletes them from the queue.
    \brief pvData Pointer to a buffer where read data is to be stored
    \brief ubNrOfBytes Number of bytes to read and free from the queue
    \return TE_RESULT code from enumerated type TE_RESULT:
     "RESULT_SUCCESS" if bytes are read and deleted from the queue
     successfully
    "RESULT_ERROR" if the specified number of bytes cannot be read
    and freed from the queue.
 */
 TE_RESULT QUEUE_teRead_Single(void *pvData,
                                 UBYTE ubNrOfBytes);
#endif

#if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
/*! \brief Frees the specified number of bytes from the specified queue.
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \param ubNrOfBytes Number of bytes to free from the queue
    \return void
 */
 void QUEUE_vFree_Multi(const TE_QUEUE_ID teQueueID, UBYTE ubNrOfBytes);
#elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/*! \brief Frees the specified number of bytes from the queue (QUEUEID_0)
    \param ubNrOfBytes Number of bytes to free from the queue
    \return void
 */
 void QUEUE_vFree_Single(UBYTE ubNrOfBytes);
#endif

//extern UBYTE QUEUE_ubGetFreeSpace(const TE_QUEUE_ID teQueueID);

/* Functionality for the Test Suite */
#if (TARGET_DEVICE == DEVICE_X86)
 #if QUEUE_INTERFACE == MULTIPLE_QUEUE_INTERFACE
/*! \brief Returns the pointer and the size of the requested (teQueueID)
     Queue buffer
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \return **pubQueueBuf: pointer to requested Queue buffer
 */
  UBYTE* QUEUE_pubGetQueueBufferPointer(const TE_QUEUE_ID teQueueID,
                                               UBYTE *pubBufSize);
 #elif QUEUE_INTERFACE == SINGLE_QUEUE_INTERFACE
/*! \brief Returns the pointer and the size of the requested (teQueueID)
     Queue buffer
    \param teQueueID Id (type enum TE_QUEUE_ID) of used queue
    \param *pubBufSize: size of requested Queue buffer
    \return **pubQueueBuf: pointer to requested Queue buffer

 */
  UBYTE* QUEUE_pubGetQueueBufferPointer(UBYTE *pubBufSize);
 #endif
#endif

#ifdef __cplusplus
  }
#endif

#endif

/*! @} */

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

