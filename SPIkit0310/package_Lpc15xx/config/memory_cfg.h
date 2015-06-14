/******************************************************************************/
/*                                                                            */
/*    $Workfile::   Memory_cfg.h                                                  $ */
/*                                                                            */
/*    $Revision:: 246                                                    $    */
/*                                                                            */
/*     $Archive::                                                             */
/*                                                                            */
/*     $Modtime::   Jul 15 2008 11:18:12                                 $    */
/*                                                                            */
/*      $Date:: 2014-10-24 10:15:21 +0800 (周五, 24 十月 2014)       $    */
/*                                                                            */
/*      $Author:: qingwu.lu@philips.com                                    $  */
/*                                                                            */
/*      Owner::     Tim Lu                                                    */
/*                                                                            */
/*  Description::   Memory configurations                                     */
/*                                                                            */
/*   Department::   Philips Lighting Sources and Electronics                  */
/*                                                                            */
/*                  Philips Company Confidential.                             */
/*                  Copyright (C) 2014 Koninklijke Philips Electronics N.V.   */
/*                  All rights reserved.                                      */
/*                                                                            */
/******************************************************************************/
#ifndef MEMORY_CFG_H
#define MEMORY_CFG_H

/******************************************************************************/
/*                            CONSTANT DEFINITIONS                            */
/******************************************************************************/
/* ROM space */
#define CFG_ROM_BASE_ADDR         (0x03000000)
#define CFG_ROM_END_ADDR          (0x03007FFF)
#define CFG_ROM_SIZE              (CFG_ROM_MAX_ADDR - CFG_ROM_BASE_ADDR + 1)

/* SRAM space */
#define CFG_SRAM_BASE_ADDR        (0x02000000)
#define CFG_SRAM_END_ADDR         (0x02002FFF)
#define CFG_SRAM_SIZE             (CFG_SRAM_MAX_ADDR - CFG_SRAM_BASE_ADDR + 1)

/* Flash space */
/* 0x00000000  - 0x000000BF for active interrupt vectors */
#define CFG_FLASH_BASE_ADDR       (0x00000000)
#define CFG_FLASH_END_ADDR        (0x0003FFFF)
#define CFG_FLASH_SIZE            (CFG_FLASH_MAX_ADDR - CFG_FLASH_BASE_ADDR + 1)

/* EEPROM space 
 * The top 64 bytes of the EEPROM memory are reserved
 */
#define CFG_EEPROM_BASE_ADDR      (0x032000F0)
#define CFG_EEPROM_END_ADDR       (0x03200FBF)
#define CFG_EEPROM_SIZE           (CFG_EEPROM_MAX_ADDR - CFG_EEPROM_BASE_ADDR + 1)
#endif

/******************************************************************************/
/*                                 END OF FILE                                */
/******************************************************************************/

