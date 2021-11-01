/****************************************************************************** 
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains header definitions for the Zx IDMA.
  * 
 **/

#ifndef __ZX_IDMA_H
#define __ZX_IDMA_H

#include <stdint.h>
#include "error_types.h"
#include "dma_buffer.h"

#define ZX_CREEK_STRETCHBLT8_CMD 0x0A
#define ZX_CREEK_STRETCHBLT24_CMD 0x0C
#define ZX_CREEK_STRETCHBLT32_CMD 0x0E

/// initialize once to create descriptor chains
void zx_idma_init(void);

/// open will cleanup unfreed buffers, call once per page.
error_type_t zx_idma_open(void);  

/// add an input buffer all buffers on a page should be the same type (pip config restriction)
/// height should be less than or equal to the video buffer strip height.
#define zx_idma_add_bigbuffer_mono(bb_in, w, h)  zx_idma_add_bigbuffer( bb_in, w, h, ZX_CREEK_STRETCHBLT8_CMD )
#define zx_idma_add_bigbuffer_rgb(bb_in, w, h)   zx_idma_add_bigbuffer( bb_in, w, h, ZX_CREEK_STRETCHBLT24_CMD )
#define zx_idma_add_bigbuffer_rgbx(bb_in, w, h)  zx_idma_add_bigbuffer( bb_in, w, h, ZX_CREEK_STRETCHBLT32_CMD )
error_type_t zx_idma_add_bigbuffer(BigBuffer_t *bb_in, uint32_t width, uint32_t height, uint8_t zx_blt_cmd );

/// called once per page to start processing.
error_type_t zx_idma_start(uint32_t image_width_in_pixels,
                           uint32_t image_height_in_lines);
/// disable 
error_type_t zx_idma_close(void);
int zx_idma_free_input( int lines_to_free, bool start, bool end );
void zx_dump_descriptor_list( char *label );

/// not supported...
error_type_t zx_idma_add_buffer(uint8_t *buffer, uint32_t width, uint32_t height);
#endif


