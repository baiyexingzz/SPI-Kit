/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Memory.c                                                 $   */
/*                                                                            */
/*    $Revision:: 357                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*     $Date:: 2014-11-26 13:12:09 +0800 (周三, 26 十一月 2014)    $     */
/*                                                                            */
/*     $Author:: qingwu.lu@philips.com                                    $   */
/*                                                                            */
/*     Owner::     Tim Lu                                                     */
/*                                                                            */
/*   Description::  Low-level macros, definition, functions - Flash, etc      */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#include <assert.h>
#include <string.h>

#include "system.h"
#include "memory.h"
/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
STATIC const U32 FLASH_CRP_COED[FLASH_CRP_3 + 1] = 
{
     0xFFFFFFFF,
	 0x4E697370,
	 0x12345678,
     0x87654321,
     0x43218765,
};

/******************************************************************************/
/*                              MACRO DEFINITIONS                             */
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
/**
 * @brief	Initialize EEPROM controller
 * @param	Nothing
 * @return	Nothing
 * @note    Not provide shutdown funtion of EEPROM
 */
STATIC INLINE void EEPROM_vInit()
{
	/* EEPROM controller is power up default */
	SYSCTL_vPowerUp(SYSCTL_POWERDOWN_EEPROM_PD);	
	
	CLOCK_vEnablePeriphClock(SYSCTL_CLOCK_EEPROM);
	SYSCTL_vPeriphReset(RESET_EEPROM);
}

/**
 * @brief	Write data to EEPROM
 * @param	unDstAdd : Destination address 
 * @param	pData    : Pointer to data buffer 
 * @param	unBytes  : Length of data buffer - byte unit 
 * @return	Nothing
 * @note    Not provide shutdown funtion of EEPROM
 */
STATIC INLINE IAP_STATUS_T EEPROM_tWrite(U32 unDstAdd, U8 *pData, U32 unBytes)
{
	U32 command[5], result[4];

	command[0] = IAP_EEPROM_WRITE;
	command[1] = (unDstAdd - CFG_EEPROM_BASE_ADDR);
	command[2] = (U32) pData;
	command[3] = unBytes;
	command[4] = CLOCK_nGetSysClock()/ 1000;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/**
 * @brief	Read data from EEPROM
 * @param	unSrcAdd : Source address 
 * @param	pData    : Pointer to data buffer 
 * @param	unBytes  : Length of data buffer - byte unit 
 * @return	Nothing
 * @note    Not provide shutdown funtion of EEPROM
 */
STATIC INLINE IAP_STATUS_T EEPROM_tRead(U32 unSrcAdd, U8 *pData, U32 unBytes)
{
	U32 command[5], result[4];

	command[0] = IAP_EEPROM_READ;
	command[1] = (unSrcAdd - CFG_EEPROM_BASE_ADDR);
	command[2] = (U32) pData;
	command[3] = unBytes;
	command[4] = CLOCK_nGetSysClock() / 1000;
	IAP_vEntry(command, result);

	return (IAP_STATUS_T)result[0];
}

/**
 * @brief	Remap IAP_STATUS_T to MEM_RESULT_T
 * @param	tStatus       : IAP_STATUS_T
 * @return	MEM_RESULT_T 
 */
MEM_RESULT_T MEMORY_tRemapStatus(IAP_STATUS_T tStatus);

/**
 * @brief	Write data into flash
 * @param	pvSrc       : Source data buffer
 * @param	tOffset     : Address offset to flash base address 
 * @param	ubNrOfBytes : Size of data
 * @return	bool 
 */
MEM_RESULT_T MEMORY_tFlashWrite(const void      *pSrc,
                                T_ADDRESS_SPACE tAddress,
                                U16             unNrOfBytes);

/**
 * @brief	Write data into EEPROM
 * @param	pvSrc       : Source data buffer
 * @param	tOffset     : Address offset to flash base address 
 * @param	ubNrOfBytes : Size of data
 * @return	bool 
 */
STATIC INLINE MEM_RESULT_T MEMORY_tEEPROMWrite(const void      *pSrc,
                                               T_ADDRESS_SPACE tAddress,
                                               U16              unNrOfBytes)
{
    IAP_STATUS_T status = IAP_CMD_SUCCESS;
	
    if(NULL == pSrc){
		return MEM_BAD_PARAM;
	}
	 
	status = EEPROM_tWrite(tAddress, (U8*) pSrc, (U32)unNrOfBytes);

	return MEMORY_tRemapStatus(status);
}

/**
 * @brief	Read data from EEPROM
 * @param	pvSrc       : Source data buffer
 * @param	tOffset     : Address offset to flash base address 
 * @param	ubNrOfBytes : Size of data
 * @return	bool 
 */
STATIC INLINE MEM_RESULT_T MEMORY_tEEPROMRead(void            *pDst,
                                              T_ADDRESS_SPACE tAddress,
                                              U16             unNrOfBytes)
{
    IAP_STATUS_T status = IAP_CMD_SUCCESS;

	if(NULL == pDst){
		return MEM_BAD_PARAM;
	}
	 
	status = EEPROM_tRead(tAddress, (U8*) pDst, (U32)unNrOfBytes);

	return MEMORY_tRemapStatus(status);
}

/******************************************************************************/
/*                      PUBLIC FUNCTION IMPLEMENTATIONS                       */
/******************************************************************************/
/* Init memory - Flash and EEPROM */
MEM_RESULT_T MEMORY_tInit(void)
{
    /* SRAM0, SRAM1 and SRAM2 is default enabled */	
	
	/* Flash is default enabled*/	
	SYSCTL_FMC_SetFLASHAccess(SYSCTL_FLASHTIM_DEFAULT);

    /* Init EEPROM controller */
	EEPROM_vInit();

	return MEM_SUCCESS;
}

/* Write data into memory */
MEM_RESULT_T MEMORY_tWrite(const void      *pSrc,
                           T_ADDRESS_SPACE tAddress,
                           U16             unNrOfBytes,
                           LPC_MEMTYPES_T  teMemType)
{
    MEM_RESULT_T ret = MEM_SUCCESS;    

    if(NULL == pSrc){
		return MEM_BAD_PARAM;
	}

    switch (teMemType){     
		case MEMTYPE_FLASH:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_FLASH_BASE_ADDR, CFG_FLASH_END_ADDR)){
				return MEM_ERROR;
			}

			ret = MEMORY_tFlashWrite(pSrc, tAddress, unNrOfBytes);			
			break;
        case MEMTYPE_SRAM:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_SRAM_BASE_ADDR, CFG_SRAM_END_ADDR)){
				return MEM_ERROR;
			}
			
            (void)memcpy((void *)(tAddress),pSrc,unNrOfBytes);
            break;
        case MEMTYPE_EEPROM:			
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_EEPROM_BASE_ADDR, CFG_EEPROM_END_ADDR)){
				return MEM_ERROR;
			}
			
            ret = MEMORY_tEEPROMWrite(pSrc, tAddress, unNrOfBytes);			
			break;			
        default:
            // Unsupported memory type
            ret = MEM_ERROR;
        break;
    }
	
    return ret;
}

/* Read data from memory */
MEM_RESULT_T MEMORY_tRead(void            *pDst,
                          T_ADDRESS_SPACE tAddress,
                          U16             unNrOfBytes,
                          LPC_MEMTYPES_T  teMemType)
{
    MEM_RESULT_T ret = MEM_SUCCESS;    

	if(NULL == pDst){
		return MEM_BAD_PARAM;
	}

    switch (teMemType){      
		case MEMTYPE_FLASH:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_FLASH_BASE_ADDR, CFG_FLASH_END_ADDR)){
				return MEM_BAD_PARAM;
			}
			
			(void)memcpy(pDst, (void *)(tAddress), unNrOfBytes);
	        break;
		case MEMTYPE_SRAM:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_SRAM_BASE_ADDR, CFG_SRAM_END_ADDR)){
				return MEM_BAD_PARAM;
			}
			
	        (void)memcpy(pDst,(void *)(tAddress), unNrOfBytes);
	        break;	
        case MEMTYPE_ROM:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_ROM_BASE_ADDR, CFG_ROM_END_ADDR)){
				return MEM_BAD_PARAM;
			}
			
	        (void)memcpy(pDst,(void *)(tAddress), unNrOfBytes);
	        break;
		case MEMTYPE_EEPROM:
			if(!MEMORY_VALIDATE_ADDR(tAddress, CFG_EEPROM_BASE_ADDR, CFG_EEPROM_END_ADDR)){
				return MEM_BAD_PARAM;
			}

		    ret = MEMORY_tEEPROMRead(pDst, tAddress, unNrOfBytes);
			break;			
        default:
	        // Unsupported memory type
	        ret = MEM_BAD_PARAM;
  }
	
  return ret;
}

/* Erase Flash Page(s) */
MEM_RESULT_T MEMORY_tEraseFlashPage(U32 unStrPage, U32 unEndPage)
{
	IAP_STATUS_T status = IAP_CMD_SUCCESS;
	
	if(unStrPage < unEndPage){
		return MEM_BAD_PARAM;
	}

    /* Erase sector(s) */
	status = IAP_tErasePage(unStrPage, unEndPage);
    if(IAP_CMD_SUCCESS != status){
        return MEMORY_tRemapStatus(status);
	}

	return MEM_SUCCESS;
}

/* Erase Flash Sector(s) */
MEM_RESULT_T MEMORY_tEraseFlashSector(U32 unStrSector, U32 unEndSector)
{
	IAP_STATUS_T status = IAP_CMD_SUCCESS;

	if(unStrSector < unEndSector){
		return MEM_BAD_PARAM;
	}
	
    /* Prepare sector range for next writting Op */
	status = IAP_tPreSectorForWrite(unStrSector, unEndSector);
    if(IAP_CMD_SUCCESS != status){
        return MEMORY_tRemapStatus(status);
	}

    /* Erase sector(s) */
	status = IAP_tEraseSector(unStrSector, unEndSector);
    if(IAP_CMD_SUCCESS != status){
        return MEMORY_tRemapStatus(status);
	}

	return MEM_SUCCESS;
}

/* Refresh Flash CRP code */
MEM_RESULT_T MEMORY_tRefreshFlashCRP(LPC_FLASH_CRP_T teCRP)
{
	 return MEMORY_tWrite((void*)&FLASH_CRP_COED[teCRP], (T_ADDRESS_SPACE)(FLASH_CRP_ADDR), 4, MEMTYPE_FLASH);
}
/******************************************************************************/
/*                      PRIVATE FUNCTION IMPLEMENTATIONS                      */
/******************************************************************************/
/* Remap IAP_STATUS_T to MEM_RESULT_T */
MEM_RESULT_T MEMORY_tRemapStatus(IAP_STATUS_T tStatus)
{  
	switch(tStatus){
	    case IAP_CMD_SUCCESS:
			return MEM_SUCCESS;
		case IAP_INVALID_COMMAND:
		case IAP_COMPARE_ERROR:	
			return MEM_ERROR;
		case IAP_SRC_ADDR_NOT_MAPPED:
		case IAP_DST_ADDR_NOT_MAPPED:
		case IAP_ADDR_NOT_MAPPED:
		case IAP_ADDR_ERROR:	
		case IAP_SRC_ADDR_ERROR:  
		case IAP_DST_ADDR_ERROR:			
			return MEM_INVAIID_ADDR;	
		case IAP_COUNT_ERROR:
		case IAP_INVALID_SECTOR:
		case IAP_PARAM_ERROR:
			return MEM_BAD_PARAM;
		case IAP_SECTOR_NOT_BLANK:
		case IAP_SECTOR_NOT_PREPARED:
		case IAP_CMD_LOCKED:	
			return MEM_BAD_STATE;
		case IAP_BUSY: 
			return MEM_BUSY;
		default:
			return MEM_ERROR;	
	}	
}

/* Write data into memory */
MEM_RESULT_T MEMORY_tFlashWrite(const void       *pSrc,
                                T_ADDRESS_SPACE  tAddress,
                                U16              unNrOfBytes)
{
	IAP_STATUS_T status = IAP_CMD_SUCCESS;

	U32 srcoffset  = 0;
	U32 dstoffset  = 0;
	U32 curlen     = 0;

	U32 curpage    = 0;
	U32 startpage  = 0;
	U32 endpage = 0;
	U32 pagestartaddr = 0;
	U32 pageendaddr = 0;
	U32 cursector = 0;

    #if defined(__CC_ARM)
	__align(4) U8 MemFlashPageBuf[FLASH_PAGE_BYTES]; 
    #endif
    #if defined(__ICCARM__)
    #pragma data_alignment=4
	static U8 MemFlashPageBuf[FLASH_PAGE_BYTES];
    #endif 
    #if defined( __GNUC__ )
	static U8 MemFlashPageBuf[FLASH_PAGE_BYTES] __attribute__ ((aligned(4)));
    #endif 	

	startpage = FLASH_PAGE(tAddress);
	endpage   = FLASH_PAGE(tAddress+unNrOfBytes-1);
    for(curpage = startpage; curpage <= endpage; curpage++){		
        pagestartaddr = FLASH_PAGE_STARTADDRESS(curpage);
		pageendaddr   = FLASH_PAGE_ENDADDRESS(curpage);
		cursector     = FLASH_SECTOR(pagestartaddr);
    
		/* Dst data offset for current page */
        dstoffset = (tAddress + srcoffset - pagestartaddr);

		/* Src data length for current page */		
		curlen = MIN(tAddress + unNrOfBytes, pageendaddr) - (dstoffset + pagestartaddr) + 1;
		
		/* Combine a temp page aligned src data buffer with target flash */
	    (void)memcpy((void *)MemFlashPageBuf, (void *)pagestartaddr, FLASH_PAGE_BYTES);
        (void)memcpy((void *)((U32)MemFlashPageBuf + dstoffset), (void *)((U32)pSrc + srcoffset), curlen);

		/* Prepare sector range for next erasing Op */
		status = IAP_tPreSectorForWrite(cursector, cursector);
		if(IAP_CMD_SUCCESS != status){
		    return MEMORY_tRemapStatus(status);
		}

		status = IAP_tErasePage(curpage, curpage);
		if(IAP_CMD_SUCCESS != status){
		    return MEMORY_tRemapStatus(status);
		}
 
		/* Prepare sector range for next writting Op */
		status = IAP_tPreSectorForWrite(cursector, cursector);
		if(IAP_CMD_SUCCESS != status){
		    return MEMORY_tRemapStatus(status);
		}
		
		status = IAP_tCopyRamToFlash(pagestartaddr, (U32)(MemFlashPageBuf), FLASH_PAGE_BYTES);
		if(IAP_CMD_SUCCESS != status){
	        return MEMORY_tRemapStatus(status);
		}

        /* next page */
		srcoffset += curlen;
	}

	return MEM_SUCCESS;   
}

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

