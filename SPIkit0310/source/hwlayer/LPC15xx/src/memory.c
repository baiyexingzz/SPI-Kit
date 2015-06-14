/******************************************************************************/
/*                                                                            */
/*    $Workfile::   memory.c                                             $    */
/*                                                                            */
/*    $Revision:: 4805                                                   $    */
/*                                                                            */
/*     $Archive::                                                        $    */
/*                                                                            */
/*     $Modtime::                                                        $    */
/*                                                                            */
/*        $Date:: 2013-11-28 14:16:45 +0800 (周四, 28 十一月 2013)  $    */
/*                                                                            */
/*      $Author:: ricky.lu@philips.com                                   $    */
/*                                                                            */
/*        Owner::   Kyle Ye                                                   */
/*                                                                            */
/*  Description::   Memory handler for the STM32 platform                     */
/*                                                                            */
/*   Department::   B.G. Lighting Systems & Controls                          */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2008 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include <assert.h>
#include <string.h>

#include "platform.h"
#include "stdtypes.h"
#include "memory.h"
#include "memory_cfg.h"
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
#define WRITTEN_FLAG  0x11

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((U32)0x00000001)
#define CR_PG_Reset              ((U32)0x00001FFE) 
#define CR_PER_Set               ((U32)0x00000002)
#define CR_PER_Reset             ((U32)0x00001FFD)
#define CR_MER_Set               ((U32)0x00000004)
#define CR_MER_Reset             ((U32)0x00001FFB)
#define CR_OPTPG_Set             ((U32)0x00000010)
#define CR_OPTPG_Reset           ((U32)0x00001FEF)
#define CR_OPTER_Set             ((U32)0x00000020)
#define CR_OPTER_Reset           ((U32)0x00001FDF)
#define CR_STRT_Set              ((U32)0x00000040)
#define CR_LOCK_Set              ((U32)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((U32)0x00000002)
#define WRP0_Mask                ((U32)0x000000FF)
#define WRP1_Mask                ((U32)0x0000FF00)
#define WRP2_Mask                ((U32)0x00FF0000)
#define WRP3_Mask                ((U32)0xFF000000)
#define OB_USER_BFB2             ((U16)0x0008)

/* FLASH Keys */
#define RDP_Key                  ((U16)0x00A5)
#define FLASH_KEY1               ((U32)0x45670123)
#define FLASH_KEY2               ((U32)0xCDEF89AB)

/* Delay definition */   
#define ERASE_TIMEOUT            ((U32)0x000B0000)
#define PROGRAM_TIMEOUT          ((U32)0x00002000)
#define UNLOCK_TIMEOUT           ((U32)0x00002000) 

#define FLASH_FLAG_BSY           ((ULONG)0x00000001)  /*!< FLASH Busy flag */
#define FLASH_FLAG_EOP           ((ULONG)0x00000020)  /*!< FLASH End of Operation flag */
#define FLASH_FLAG_PGERR         ((ULONG)0x00000004)  /*!< FLASH Program error flag */
#define FLASH_FLAG_WRPRTERR      ((ULONG)0x00000010)  /*!< FLASH Write protected error flag */
#define FLASH_FLAG_OPTERR        ((ULONG)0x00000001)  /*!< FLASH Option Byte error flag */

#define FLASH_END_ADDRESS              (0x08010000)
#define APPLICATION_ADDRESS            (0x08001000)
#define PAGE_SIZE                      (0x400)

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
/******************************************************************************/
#define IS_FLASH_ADDRESS(ADDRESS) (((ADDRESS) >= APPLICATION_ADDRESS) && ((ADDRESS) < FLASH_END_ADDRESS))

/******************************************************************************/
/*                              TYPE DEFINITIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                          PUBLIC DATA DEFINITIONS                           */
/******************************************************************************/

/******************************************************************************/
/*                          PRIVATE DATA DEFINITIONS                          */
/******************************************************************************/
/******************************************************************************/
/*                        PRIVATE FUNCTION PROTOTYPES                         */
/******************************************************************************/

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/

void MEMORY_vInit(void)
{
  M24C16_vClearBusyFlag();
}
////////////////////////////////////////////////////////////////////////////////
FLASH_Status FLASH_Unlock(U32 Timeout)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;


  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while(((FLASH->CR & CR_LOCK_Set) == CR_LOCK_Set) && (Timeout != 0x00))
  {
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
FLASH_Status FLASH_GetStatus(void)
  /**
   * @brief  Returns the FLASH Status.
   * @note   This function can be used for all STM32F10x devices, it is equivalent
   *         to FLASH_GetBank1Status function.
   * @param  None
   * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
   *                                                  FLASH_ERROR_WRP or FLASH_COMPLETE
   */
{
  FLASH_Status flashstatus = FLASH_COMPLETE;

  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
FLASH_Status FLASH_WaitForLastOperation(U32 Timeout)
  /**
   * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
   * @note   This function can be used for all STM32F10x devices, 
   *         it is equivalent to FLASH_WaitForLastBank1Operation.
   *         - For STM32F10X_XL devices this function waits for a Bank1 Flash operation
   *           to complete or a TIMEOUT to occur.
   *         - For all other devices it waits for a Flash operation to complete 
   *           or a TIMEOUT to occur.
   * @param  Timeout: FLASH progamming Timeout
   * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
   *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
   */
{ 
  FLASH_Status status = FLASH_COMPLETE;

  /* Check for the Flash Status */
  status = FLASH_GetStatus();

  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
FLASH_Status FLASH_ProgramHalfWord(U32 Address, U16 Data)
  /**
   * @brief  Programs a half word at a specified address.
   * @note   This function can be used for all STM32F10x devices.
   * @param  Address: specifies the address to be programmed.
   * @param  Data: specifies the data to be programmed.
   * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
   *                                                  FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
   */
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  if (!IS_FLASH_ADDRESS(Address))
  {
    status = FLASH_BAD_ADDRESS;
    return status;
  }

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);

  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;

    *(volatile U16*)Address = Data;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(PROGRAM_TIMEOUT);

    /* Disable the PG Bit */
    FLASH->CR &= CR_PG_Reset;
  } 

  /* Return the Program Status */
  return status;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
FLASH_Status FLASH_ErasePage(U32 Page_Address)
  /**
   * @brief  Erases a specified FLASH page.
   * @note   This function can be used for all STM32F10x devices.
   * @param  Page_Address: The page address to be erased.
   * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
   *   FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
   */
{
  FLASH_Status status = FLASH_COMPLETE;

  ///* Check the parameters */
  if (!IS_FLASH_ADDRESS(Page_Address))
  {
    status = FLASH_BAD_ADDRESS;
    return status;
  }

  if ((FLASH->CR & CR_LOCK_Set) == CR_LOCK_Set)
  {
    status = FLASH_LOCKED;
    return status;
  }

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);

  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ERASE_TIMEOUT);

    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }

  /* Return the Erase Status */
  return status;
}

////////////////////////////////////////////////////////////////////////////////
TE_RESULT MEMORY_teWrite(const void *pvSrc,
    T_ADDRESS_SPACE tOffset,
    UBYTE ubNrOfBytes,
    TE_MEMTYPES teMemType)
{
  UBYTE i = 0;
  TE_RESULT teResult = RESULT_SUCCESS;
  USHORT u16Temp1 = 0, u16Temp2 = 0;
  ULONG u32FlashAddress = 0;
  FLASH_Status FlashStatus = FLASH_COMPLETE;

  switch (teMemType)
  {
    case MEMTYPE_NVRAM:
      if(tOffset > NVRAM_LOGIC_ADDR_END)
      {
        return RESULT_ERROR;
      }

      //Case 1: whole range in E2PROM 
      if((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) < NVRAM_DRIVER_PARAMETERS)
      {
        teResult = M24C16_teWrite(pvSrc,(USHORT)tOffset,ubNrOfBytes);
        break;
      }
      //Notice: Remove the case one part in E2PROM and one part in Flash. 
      //Because from 0x0A00 to 0x0FFF is reversed area, 1536 bytes is enough use for gap.
      //Case 2: One part in E2PROM and one part in Flash, return error
      if((tOffset < NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) > NVRAM_DRIVER_PARAMETERS))
      {
        return RESULT_ERROR;
      }

      //Notice : Should Erase flash before write it on STM32 platform, the Driver and Lamp table should be written together.
      //And the first UART command's addr should be 'NVRAM_DRIVER_PARAMETERS'. Otherwise we can't determine when to erase flash.
      if(tOffset == NVRAM_DRIVER_PARAMETERS)
      {
        FLASH_Unlock(0x00002000);
        FlashStatus = FLASH_ErasePage(FLASH_END_ADDRESS - PAGE_SIZE); 
        if(FLASH_COMPLETE != FlashStatus)
          return RESULT_ERROR;
      }

      //Case 2: Whole in Flash
      //Notice : the ubNrOfBytes should be evennumber.
      if((tOffset >= NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) <= NVRAM_LOGIC_ADDR_END))
      {
        UBYTE* pubSrc = (UBYTE*)pvSrc;
        if (ubNrOfBytes%2 != 0)
          return RESULT_ERROR;

        u32FlashAddress = tOffset - NVRAM_DRIVER_PARAMETERS + FLASH_DRIVER_PARAMETERS;
        for (i=0; (i*2) < ubNrOfBytes; i++)
        {          
          u16Temp1 = (USHORT)(*pubSrc++);
          u16Temp2 = (USHORT)(*pubSrc++);
          u16Temp1 = (u16Temp2<<8) + u16Temp1;
          FlashStatus = FLASH_ProgramHalfWord(u32FlashAddress, u16Temp1);

          if (FlashStatus != FLASH_COMPLETE)
          {
            return RESULT_ERROR;
          }
          u32FlashAddress += 0x00000002;
        }
        break;
      }

      //Case 3: Part in Flash and part out of memory logic addr //This case is not exist, consider as the error inut
      if((tOffset >= NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) > NVRAM_LOGIC_ADDR_END))
      {
        return RESULT_ERROR;
      }
      break;
    case MEMTYPE_RAM:
      (void)memcpy((void *)(tOffset),pvSrc,ubNrOfBytes);
      teResult = RESULT_SUCCESS;
      break;
    default:
      // Unsupported memory type
      teResult = RESULT_ERROR;
      break;
  }
  return teResult;
}

///////////////////////////////////////////////////
//Read the whole table from EEPROM from HW concept
TE_RESULT MEMORY_teReadTable(void *pvDst,
    T_ADDRESS_SPACE tOffset,
    UBYTE ubNrOfBytes,
    TE_MEMTYPES teMemType)
{
  TE_RESULT teResult = RESULT_SUCCESS;

  if(tOffset == NVRAM_DRIVER_PARAMETERS)
  {
    tOffset = FLASH_DRIVER_PARAMETERS;
    (void)memcpy(pvDst,(void *)(tOffset),ubNrOfBytes);
  }
  else if(tOffset == NVRAM_LAMP_PARAMETERS)
  {
    tOffset = FLASH_LAMP_PARAMETERS;
    (void)memcpy(pvDst,(void *)(tOffset),ubNrOfBytes);
  }
  else
  {
    teResult = M24C16_teRead(pvDst, (USHORT)tOffset, ubNrOfBytes);
  }
  return teResult;
}

////////////////////////////////////////////////////////////////////////////////
TE_RESULT MEMORY_teRead(void *pvDst,
    T_ADDRESS_SPACE tOffset,
    UBYTE ubNrOfBytes,
    TE_MEMTYPES teMemType)
{
  TE_RESULT teResult = RESULT_SUCCESS;

  switch (teMemType)
  {
    case MEMTYPE_NVRAM:
      if(tOffset > NVRAM_LOGIC_ADDR_END)
      {
        return RESULT_ERROR;
      }

      //Case 1: whole range in E2PROM 
      if((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) < NVRAM_DRIVER_PARAMETERS)
      {
        teResult = M24C16_teRead(pvDst, (USHORT)tOffset, ubNrOfBytes);
        break;
      }

      //Case 2: One part in E2PROM and one part in Flash, return error
      //Notice: Remove the case one part in E2PROM and one part in Flash. 
      //Because from 0x0A00 to 0x0FFF is reversed area, 1536 bytes is enough use for gap.
      if((tOffset < NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) > NVRAM_DRIVER_PARAMETERS))
      {
        return RESULT_ERROR;
      }

      //Case 3: Whole in Flash
      if((tOffset >= NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) <= NVRAM_LOGIC_ADDR_END))
      {
        tOffset = tOffset - NVRAM_DRIVER_PARAMETERS + FLASH_DRIVER_PARAMETERS;
        (void)memcpy(pvDst,(void *)(tOffset),ubNrOfBytes);
        break;
      }

      //Case 4: Part in Flash and part out of memory logic addr 
      //Notice : Consider this case is error input.
      if((tOffset >= NVRAM_DRIVER_PARAMETERS) && ((T_ADDRESS_SPACE)(tOffset + ubNrOfBytes) > NVRAM_LOGIC_ADDR_END))
      {
        return RESULT_ERROR;
      }
      break;
    case MEMTYPE_RAM:
    case MEMTYPE_ROM:
    case MEMTYPE_FLASH:
      (void)memcpy(pvDst,(void *)(tOffset),ubNrOfBytes);
      teResult = RESULT_SUCCESS;
      break;

    default:
      // Unsupported memory type
      teResult = RESULT_ERROR;
  }
  return teResult;
}

////////////////////////////////////////////////////////////////////////////////
void MEMORY_vWriteBusyTaskHandler(TE_EVENT_ID teEventId)
{
  M24C16_vClearBusyFlag();
}

/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

