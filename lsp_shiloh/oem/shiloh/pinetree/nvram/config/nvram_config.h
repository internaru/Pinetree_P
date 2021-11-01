/*
 * ============================================================================
 * Copyright (c) 2009-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *  \file nvram_config.h
 *
 *  \brief Non-Volatile Memory Manager Configuration
 *
 *  The defines in this file set the search order used to locate NV entities
 *  on physical memory devices.  We currently have two distinct search paths:
 *   - NV data store:    'dynamic' variable blocks that live in a container 
 *                       partition.
 *   - ROM Mgr entities: 'static' blocks of data that must be found someplace 
 *                       in the search path to be read or written (writing a 
 *                       ROM entity is intended to be used in field replacement
 *                       use cases)
 *
 *  Each search path must be arranged from highest to lowest priority and be
 *  terminated by 'MEM_DEVICE_NOT_FOUND' (see deviceid.h).  The search list
 *  may contain devices that are not installed; they will be ignored at run
 *  time.  Simply omit devices that should not be used in your products
 *  search path.
 *
 **/

#ifndef INC_NVM_CONFIG_H
#define INC_NVM_CONFIG_H

#include "deviceid.h"
#include "error_types.h"
#include "ATypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \brief Select the image map (code table) version.  
 *
 * Note that we use a define here so that we can compile out code that supports unused
 * versions.  Currently supported versions:
 *  - IMAGEMAP_VER_0_0:  Original code table format
 *  - IMAGEMAP_VER_1_0:  Enhanced Image Map
 */
 #define IMAGEMAP_VER_1_0


/**
 *  \brief Get the Rommgr device search list
 *
 *  This routine will return the Rommgr device search list.  The first item
 *  in the list has the highest priority, terminated with MEM_DEVICE_NOT_FOUND.
 *
 *  \return MEMORY_DEVICE *
 *  \retval Pointer to returned search list
 **/
const MEMORY_DEVICE *getRommgrSearchList(void);


/**
 *  \brief Get the NV store device search list
 *
 *  This routine will return the NV store device search list.  The first item
 *  in the list has the highest priority, terminated with MEM_DEVICE_NOT_FOUND.
 *
 *  \return MEMORY_DEVICE *
 *  \retval Pointer to returned search list
 **/
const MEMORY_DEVICE *getNvStoreSearchList(void);


#if defined(HAVE_NAND)

#define NVRAM_SUPPORTS_AUTO_FORMAT

// eeprom and bspi autoformat not supported
#define nvconfig_eeprom_format NULL
#define nvconfig_eeprom_verify_format NULL
#define nvconfig_bspi_format NULL
#define nvconfig_bspi_verify_format NULL

error_type_t nvconfig_nand_verify_format(uint8_t* ctbuf);
error_type_t nvconfig_nand_format(DEVICE_HANDLE memDev);

// codetable contains a layout format/version entry.  Not version of codetable, but version of NAND layout entries..
#define IM_CODETABLE_LAYOUT 0xF0
#define CODETABLE_VERSION   0x20111205  // used to define a minimum set of codetable entries

// NAND MAP - assumption of 128KB block size
// boot blocks will contain only 1 valid block
//      other blocks are for: 
//      -possible MFG bad block
//      -possible RUNTIME block gone bad - we valid contents to next block in range.
//      will contain:  HW NV page(s) for pll /dram...; nand loader; 
//      MBBT (mfg bad block table)
//      boot blocks are NOT MEANT to be erase once deployed!!!!!!!
// codetable block
// NV blocks (static=1)(dynamic=2)(silentboot=1)(misc=x)
// RBBT (run time bad block table) block - points to (spare pool location --> end of nand) resides in FIRST SPARE POOL block
// LOGICAL ORDER#  (I = image @ MFG;  F = in Field Added;   + = in field modified)
// I+0       boot block                          <<-- will not be erased in field.  MBBT is ADDED in last page
//                                               <<-- MBBT will be created (in field) in boot block, Never updated
// I1        empty - boot block backup
// I2        empty - boot block backup
// I3        empty - boot block backup
// I+4       codetable                           <<-- a burnflash will update f/w length field, not much else would change....
// I+5       filesystem                          <<-- will be erased in field.  This can grow/shrink
//           pad(x)                              <<-- enough pad to allow filesys to grow and not destroy next section
// F         spare pool                          <<-- last valid x blocks -- this section shrinks as MFG BB grows... 
//                                                  blocks gone bad in field are spared, copied into available spare pool block, now RBBT table
//                                                  will handle mapping.
//  ..


#define NAND_NUM_BLKS_PER_MB    8       // assuming 128K blocks

#define NAND_MBBT_BOOT_BLOCK    1       // MBBT is contained in boot block - don't erase it. maintain for life... and longer
#define NAND_MBBT_DEFAULT_ENABLE    true;
#define NAND_RBBT_DEFAULT_ENABLE    true;

#define NAND_RBBT_VIRTUAL_BLOCK_START       0xFF00          // dynamic block allocation from spare pool START
#define NAND_RBBT_VIRTUAL_BLOCK_RBBT        0xFF00          // dynamic block allocation from spare pool

// The number of blocks for various block types.
#define NUM_BLKS_BOOT_BLOCK    4
#define NUM_BLKS_CODETABLE     1
//-jrs#define NUM_BLKS_RBBT          0        // no longer is table in normal block, instead it will come directly from spare pool
#define NUM_BLKS_PRE_FW_PAD    7
#define NUM_BLKS_SPARING_GB    (5 *NAND_NUM_BLKS_PER_MB)  // 5 MB = 40 blocks/MB device, assuming 128K blocks.

// The number of blocks depending upon the NAND part size.
#if defined(HAVE_2Gb_NAND)
#define NUM_BLKS_TOTAL       (256*NAND_NUM_BLKS_PER_MB)  // assuming 128K blocks.
#define NUM_BLKS_FILE_SYS    (120*NAND_NUM_BLKS_PER_MB)  // 120 MB, assuming 128K blocks.
#define NUM_BLKS_PAD         (75 *NAND_NUM_BLKS_PER_MB)  // 75 MB assuming 128K blocks.
#define NUM_BLKS_SPARING     (2  *NUM_BLKS_SPARING_GB )  // 10 MB = 80 blocks, assuming 128K blocks.
#elif defined(HAVE_1Gb_NAND)
#define NUM_BLKS_TOTAL       (128*NAND_NUM_BLKS_PER_MB)  // assuming 128K blocks.
#define NUM_BLKS_FILE_SYS    (80 *NAND_NUM_BLKS_PER_MB)  // 80 MB, assuming 128K blocks.
#define NUM_BLKS_PAD         (4  *NAND_NUM_BLKS_PER_MB)  // 4 MB assuming 128K blocks.
#define NUM_BLKS_SPARING     (1  *NUM_BLKS_SPARING_GB )  // 5 MB = 40 blocks, assuming 128K blocks.
#else 
#error Must define a NAND part size of 1 or 2 Gb in project file. 
#endif

// The logical address layout for the NAND part.

// The boot block staring location.
#define NAND_MAP_BOOT_BLOCK_START   0

// The starting location of the code table.
#define NAND_MAP_CODETABLE_BLOCK_START (NAND_MAP_BOOT_BLOCK_START + NUM_BLKS_BOOT_BLOCK)
// following addresses are relative to codetable placement
#define NAND_MAP_CODETABLE_BLOCK_START0 0       
#define NAND_MAP_FILE_SYS_START         (NAND_MAP_CODETABLE_BLOCK_START0 + NUM_BLKS_CODETABLE)
#define NAND_MAP_SPARE_POOL_START       (NAND_MAP_FILE_SYS_START + NUM_BLKS_FILE_SYS + NUM_BLKS_PAD)
#define NAND_MAP_END                    (NAND_MAP_SPARE_POOL_START + NUM_BLKS_SPARING)
#define NAND_MAP_PHYS_END               (NAND_MAP_END + NAND_MAP_CODETABLE_BLOCK_START)

#if NAND_MAP_PHYS_END > NUM_BLKS_TOTAL
#error NAND map exceeded max blocks available
#endif


#endif // HAVE_NAND

#ifdef __cplusplus
}
#endif

#endif /*INC_NVM_CONFIG_H */
