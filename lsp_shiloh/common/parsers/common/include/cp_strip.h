#ifndef CP_STRIP_H
#define CP_STRIP_H

/******************************************************************************
 * Copyright (c) 2010-2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * common parser strip handling functions.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "ColorPipe.h"
#include "print_job_api.h"

// extra halftone overwrite protection shouldn't be needed anymore.
#define DBG_EXTRA 0

// enable profiling
// #define PROFILE_RENDERING( x ) x
// disable profiling
#define PROFILE_RENDERING( x ) 

typedef struct cp_strip_s cp_strip_t; 
struct cp_strip_s
{
    bool inuse;
    cmyk_byte_array_t *cpo; // owned, constructed then memory passed to print pipe  
    //unirast_page_t
    void *page;  // not owned
    print_page_t *p_page;  // not owned
    uint32_t rawsize;
    uint32_t pitch; // output plane pitch
    uint32_t lines; // strip height MAX 128 or so
    uint32_t actual_lines; // actual strip height used on last strip
    uint32_t page_height;  // 
    uint32_t cols;
    uint32_t res;
    uint32_t num_input_colors;
    uint32_t num_output_colors;
    bool endpage;
    MODULE_ID dest;
    uint8_t *buf; // not owned
    mlimiter_t *mlimiter;
    uint8_t *static_compression_buffer; // if null do realloc shrinking, else do copy of compressed data.
    void (*end_page_cb)(cp_strip_t *cp_strip);
};


void  cp_compress_strip( 
    mlimiter_t *mlimiter,
    uint32_t rawsize, 
    uint8_t *raw_compression_buf,
    uint32_t lines,
    uint32_t cols,
    uint8_t **compressed_buf,
    uint32_t *comressed_size,
    uint8_t *static_compression_buffer
    );


void cp_send_strip( 
    print_page_t *page, MODULE_ID modid, uint8_t *ptr, uint32_t datalen,
    uint32_t bits_per_row, uint32_t num_rows, 
    uint32_t LinesPerPage, uint32_t xres, uint32_t yres,
    bool endPage,  COLOR color, PLANE_TYPE comp_type 
    );

/// color pipe will call this callback on end of strip.
///
void cp_strip_done_cb( void *data, cmyk_byte_array_t *planes, uint32_t size);


/// return null after freeing.
cp_strip_t * cp_free_rawstrips( cp_strip_t *strip );

/// terminiate a page early by sending and end page marked strip.
/// 
void cp_send_empty_endpage_strip( cp_strip_t *strip );

/// return pointer to pixel counts, these are only used on color systems (X)
/// pointer is allocated and freed down below after much message passing in
/// param3 
#ifdef PIXELCNT_BYFW
PIXELCNT *cp_pixel_count( mlimiter_t *lim, color_pipe_t *cp, bool reset_counts );
#else
#define cp_pixel_count( m, a, b ) 0
#endif

#if (NUM_MARKING_COLOR_CHANNELS == 1) 
#define cp_mono_only() (true)
#else
//
#define cp_mono_only() (false)
#endif

#ifdef DEBUG
void dbg_print_cp_strip_t( cp_strip_t *strip );
#endif



#endif // CP_STRIP_H
