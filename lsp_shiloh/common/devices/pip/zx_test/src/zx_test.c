/****************************************************************************** 
 * Copyright (c) 2009 - 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for Zx.
  * 
 **/

#ifndef __ZX_FUNC_C_INCLUDED
#define __ZX_FUNC_C_INCLUDED

#include <stdint.h>
#include "lassert.h"
#include "zx_test_api.h"


void zx_setup(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], uint32_t data_addr, uint32_t width, uint32_t height)
{
    uint32_t npixels, descpixels, i;
    //uint8_t color_cmd = 0x0c;
    uint8_t color_cmd = 0x0e;
    // Check for 32B alignment
    ASSERT((((uint32_t) desc_cmds) & 0x01F) == 0x0);

    npixels = width * height;  // Total pixels
    ASSERT(npixels < (MAX_ZX_DESCRIPTORS*0xFFFFFFUL)); // 24 bits of pixels per desc
    descpixels = (npixels / MAX_ZX_DESCRIPTORS) & 0xFFFFFFE0UL;  // pixels per descriptor

    for (i = 0; i < MAX_ZX_DESCRIPTORS; i++) 
    {
        // Fill in each descriptor
        desc_cmds[i].ctrl = 0x0000UL;  // not intOnFinish and not stopOnFinish
        desc_cmds[i].own = 0x0001;   // Owned by block
        desc_cmds[i].src = (uint8_t *) &desc_cmds[i].zx_creek_cmd;
        desc_cmds[i].length = 16; // With padding
        desc_cmds[i].next = ((MAX_ZX_DESCRIPTORS-1) == i ? 0L : &desc_cmds[i+1]);
        // Now fill in the zx_creek_cmd
        
        desc_cmds[i].zx_creek_cmd.cmd = ((MAX_ZX_DESCRIPTORS-1) == i) ?
            ((((npixels - (descpixels*(MAX_ZX_DESCRIPTORS-1)))-1) << 8) | color_cmd) :
            ((descpixels-1) << 8) | color_cmd;
        desc_cmds[i].zx_creek_cmd.data_address = data_addr + ((descpixels*i)*4); 
        ASSERT((desc_cmds[i].zx_creek_cmd.data_address & 0x1F) == 0);
        desc_cmds[i].zx_creek_cmd.scale = 1 << 24;
        desc_cmds[i].zx_creek_cmd.error = 1 << 24; 
    }

    desc_cmds[MAX_ZX_DESCRIPTORS-1].ctrl = 0x03;  // int and stop on finish
    zx_reg->ZIER = 0;       // Disable ints
    zx_reg->ZICR = 0x1F;    // Clear ints
    zx_reg->ZLLR = width - 1;   // In pixels
    zx_reg->ZIHR = height - 1;  // In lines
    // zx_reg->ZCR = 0x00000003; // LCM On
    zx_reg->ZCR = 0x00000001; // LCM Off
}

void zx_setup_single(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], uint32_t data_addr, uint32_t width, uint32_t height)
{
    uint32_t npixels;

    // Check for 32B alignment
    ASSERT((((uint32_t) desc_cmds) & 0x01F) == 0x0);

    npixels = width * height;  // Total pixels
    ASSERT(npixels < (0xFFFFFFUL)); // 24 bits of pixels per desc

    // Fill in each descriptor
    desc_cmds[0].ctrl = 0x0003UL;  // intOnFinish and stopOnFinish
    desc_cmds[0].own = 0x0001;   // Owned by block
    desc_cmds[0].src = (uint8_t *) &desc_cmds[0].zx_creek_cmd;
    desc_cmds[0].length = 16; // With padding
    desc_cmds[0].next = 0L;
    // Now fill in the zx_creek_cmd
    desc_cmds[0].zx_creek_cmd.cmd = ((npixels-1) << 8) | 0x0E;
    desc_cmds[0].zx_creek_cmd.data_address = data_addr;
    ASSERT((desc_cmds[0].zx_creek_cmd.data_address & 0x1F) == 0);
    desc_cmds[0].zx_creek_cmd.scale = 1 << 24;
    desc_cmds[0].zx_creek_cmd.error = 1 << 24; 

    zx_reg->ZIER = 0;       // Disable ints
    zx_reg->ZICR = 0x1F;    // Clear ints
    zx_reg->ZLLR = width - 1;   // In pixels
    zx_reg->ZIHR = height - 1;  // In lines
    // zx_reg->ZCR = 0x00000003; // LCM On
    zx_reg->ZCR = 0x00000001; // LCM Off
}

#define ZX_CREEK_STRETCHBLT8_CMD 0x0A
#define ZX_CREEK_STRETCHBLT24_CMD 0x0C
#define ZX_CREEK_STRETCHBLT32_CMD 0x0E

void zx_setup_mono(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t data_addr, uint32_t width, uint32_t height, uint32_t page_height)
{  
    uint8_t zx_blt_cmd = ZX_CREEK_STRETCHBLT8_CMD;

    zx_setup_raster(zx_reg, desc_cmds, desc_hw, data_addr, width, height, zx_blt_cmd, page_height);
}

void zx_setup_rgb24(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t data_addr, uint32_t width, uint32_t height, uint32_t page_height)
{  
    uint8_t zx_blt_cmd = ZX_CREEK_STRETCHBLT24_CMD;

    zx_setup_raster(zx_reg, desc_cmds, desc_hw, data_addr, width, height, zx_blt_cmd, page_height);
}

void zx_setup_raster(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t data_addr, uint32_t width, uint32_t height, uint8_t zx_blt_cmd, uint32_t page_height)
{
    uint32_t npixels;

#if 0  // turn back on for debug
    static uint32_t page_lines_remain = 0;

    if (page_height)
    {
        dbg_printf("%s: set initial height = %d\n", __func__, page_height);
        page_lines_remain = page_height;
    }
    page_lines_remain -= height;
    dbg_printf("%s: lines = %d, remain = %d\n", __func__, height, page_lines_remain);
#endif

    // Check for 32B alignment07
    ASSERT((((uint32_t) desc_cmds) & 0x01F) == 0x0);

    npixels = width * height;  // Total pixels
    ASSERT(npixels < (0xFFFFFFUL)); // 24 bits of pixels per desc

    // Fill in each descriptor
    desc_cmds[0].ctrl = 0x0001UL; //no stop no int; 3UL;  // intOnFinish and stopOnFinish
    desc_cmds[0].own = 0x0001;   // Owned by block
    desc_cmds[0].src = desc_hw + 64; // lsptodo offset hack. (&desc_cmds[0].zx_creek_cmd - &desc_cmds[0]);
    desc_cmds[0].length = 16; // With padding
    desc_cmds[0].next = 0L;
    // Now fill in the zx_creek_cmd
    desc_cmds[0].zx_creek_cmd.cmd = ((npixels-1) << 8) | zx_blt_cmd;
    desc_cmds[0].zx_creek_cmd.data_address = data_addr;
    ASSERT((desc_cmds[0].zx_creek_cmd.data_address & 0x1F) == 0);
    desc_cmds[0].zx_creek_cmd.scale = 1 << 24;
    desc_cmds[0].zx_creek_cmd.error = 1 << 24; 
    
    if (page_height) {
        zx_reg->ZIER = 0;       // Disable ints
        zx_reg->ZICR = 0x1F;    // Clear ints
        zx_reg->ZLLR = width - 1;   // In pixels
        
        zx_reg->ZIHR = page_height - 1; // height - 1;  // In lines
    // zx_reg->ZCR = 0x00000003; // LCM On
        zx_reg->ZCR = 0x00000001; // LCM Off
    }
}

void zx_setup_fill32(ZX_REGS_t *zx_reg, zx_desc_t desc_cmds[], void *desc_hw, uint32_t width, uint32_t height, uint32_t color)
{
    uint32_t npixels;

    // Check for 32B alignment07
    ASSERT((((uint32_t) desc_cmds) & 0x01F) == 0x0);

    npixels = width * height;  // Total pixels
    ASSERT(npixels < (0xFFFFFFUL)); // 24 bits of pixels per desc

    // Fill in each descriptor
    desc_cmds[0].ctrl = 0x0003UL;  // intOnFinish and stopOnFinish
    desc_cmds[0].own = 0x0001;   // Owned by block
    desc_cmds[0].src = desc_hw + 64; // lsptodo offset hack. (&desc_cmds[0].zx_creek_cmd - &desc_cmds[0]);
    desc_cmds[0].length = 16; // With padding
    desc_cmds[0].next = 0L;
    // Now fill in the zx_creek_cmd
    desc_cmds[0].zx_creek_cmd.cmd = ((npixels-1) << 8) | 0x02;
    desc_cmds[0].zx_creek_cmd.data_address = color; // data_addr;
    desc_cmds[0].zx_creek_cmd.scale = 0; // 1 << 24;
    desc_cmds[0].zx_creek_cmd.error = 0; //1 << 24; 

    zx_reg->ZIER = 0;       // Disable ints
    zx_reg->ZICR = 0x1F;    // Clear ints
    zx_reg->ZLLR = width - 1;   // In pixels
    zx_reg->ZIHR = height - 1;  // In lines
    // zx_reg->ZCR = 0x00000003; // LCM On
    zx_reg->ZCR = 0x00000001; // LCM Off
}


void zx_start(ZX_REGS_t *zx_reg, uint32_t desc_addr)
{
    zx_reg->ZDWR = desc_addr;
}

void zx_disable(ZX_REGS_t *zx_reg)
{
    zx_reg->ZCR = 0;
}

#endif
