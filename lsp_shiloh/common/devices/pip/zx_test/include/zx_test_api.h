/****************************************************************************** 
 * Copyright (c) 2009 - 2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains definitions for the Zx block.
  * 
 **/

#ifndef __ZX_FUNC_H_INCLUDED
#define __ZX_FUNC_H_INCLUDED

#include <stdint.h>
#include "Zx_regstructs.h"
#include "Zx_regmasks.h"
#include "logger.h"

//#define MAX_ZX_DESCRIPTORS 2   // Zx descs are in pixels - 2 = 32 Megapixels
//#define MAX_ZX_DESCRIPTORS 8
#define MAX_ZX_DESCRIPTORS 4

typedef struct zx_creek 
{
    uint32_t cmd;
    uint32_t data_address;
    uint32_t scale;
    uint32_t error;
} zx_creek_t;

typedef struct zx_desc zx_desc_t;

struct zx_desc 
{
    volatile uint16_t ctrl;
    volatile uint16_t own;
    volatile uint8_t *src;
    volatile uint32_t length;
    volatile zx_desc_t *next;
    volatile uint8_t pad[48]; // Round out entire record to multiple of 64 bytes
    volatile zx_creek_t zx_creek_cmd;
    volatile uint8_t pad2[48];
};


void zx_setup(ZX_REGS_t *zx_reg, zx_desc_t descs[], uint32_t data_addr, uint32_t width, uint32_t height);
void zx_setup_single(ZX_REGS_t *zx_reg, zx_desc_t descs[], uint32_t data_addr, uint32_t width, uint32_t height);
void zx_setup_mono(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_cmds_hw, uint32_t data_addr, 
                   uint32_t width, uint32_t height, /**< strip height */ 
                   uint32_t page_height /**< on first strip, zero on subsequent calls */);
void zx_setup_rgb24(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_cmds_hw, uint32_t data_addr, 
                    uint32_t width, uint32_t height,
                    uint32_t page_height /**< on first strip, zero on subsequent calls */);
void zx_setup_raster(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t data_addr, 
                     uint32_t width, uint32_t height, uint8_t zx_blt_cmd,
                     uint32_t page_height /**< on first strip, zero on subsequent calls */);
void zx_setup_fill32(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t width, uint32_t height, uint32_t color);
void zx_start(ZX_REGS_t *zx_reg, uint32_t desc_addr);
void zx_disable(ZX_REGS_t *zx_reg);
#endif


