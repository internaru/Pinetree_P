/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file chipgap.c
 *
 * \brief functions for fixing chipgap with memcpy and linear interpolation
 *
 * davep 01-Feb-2012
 */

#include <stdint.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"

#if 0
    /* davep 21-Feb-2012 ; stamp with white so we can see the boundaries */
    #define NEW_PIXEL (~0)
#else
    /* linear interpolation */
    #define NEW_PIXEL ((p1+p2)/2)
#endif

#define MAX_SEGMENTS 20

/* davep 25-Jul-2013 ; defaults updated to match a mech that will be shipping
 * with fw chipgap correction enabled 
 */
#define DEFAULT_SEGMENT_SIZE_PIXELS  432
#define DEFAULT_FIRST_GAP_PIXELS    429

struct sensor_segment {
    int pixel_pos;
    int num_pixels;

    int byte_pos;
    int num_bytes;
};

static struct sensor_segment src_segment[MAX_SEGMENTS];
static struct sensor_segment dst_segment[MAX_SEGMENTS];
static int num_segments;

static uint32_t first_gap_pixels = DEFAULT_FIRST_GAP_PIXELS;
static uint32_t segment_size_pixels = DEFAULT_SEGMENT_SIZE_PIXELS;

void chipgap_debug_print_segments( void )
{
    int i;

    for( i=0 ; i<num_segments ; i++ ) {
        dbg1( "%d %d %d %d, ", src_segment[i].pixel_pos, src_segment[i].num_pixels, 
                             src_segment[i].byte_pos, src_segment[i].num_bytes );
    }
    dbg1("\n" );
    for( i=0 ; i<num_segments ; i++ ) {
        dbg1( "%d %d %d %d, ", dst_segment[i].pixel_pos, dst_segment[i].num_pixels, 
                             dst_segment[i].byte_pos, dst_segment[i].num_bytes );
    }
    dbg1("\n");
}

scan_err_t chipgap_set_positions( uint32_t first_gap, uint32_t gap_distance )
{
    /* sanity checks */
    if( first_gap > 65536 || gap_distance > 65536 ) {
        return SCANERR_INVALID_PARAM;
    }

    first_gap_pixels = first_gap;
    segment_size_pixels = gap_distance;
    return SCANERR_NONE;
}

void chipgap_get_positions( uint32_t *first_gap, uint32_t *gap_distance )
{
    *first_gap = first_gap_pixels;
    *gap_distance = segment_size_pixels;
}

void chipgap_precalculate_segment_positions( uint32_t pic_left_margin, 
                                             uint32_t pic_right_margin, 
                                             int bits_per_pixel )
{
    uint32_t pixels_per_row;
    uint32_t pixel_pos;
    uint32_t src_pixel_pos, dst_pixel_pos;
    int bytes_per_pixel;
    int i;

    dbg2( "%s left=%d right=%d bpp=%d first_gap=%d segsize=%d\n", __FUNCTION__, pic_left_margin,
            pic_right_margin, bits_per_pixel, first_gap_pixels,
            segment_size_pixels );

    /* sanity check for improper/uninitialized pic margins */
    XASSERT( pic_right_margin > pic_left_margin+4, pic_right_margin );

    memset( src_segment, 0, sizeof(src_segment) );
    memset( dst_segment, 0, sizeof(dst_segment) );

    num_segments = 0;

    /* so far, have only tested with 1-bpp and 2-bpp */
    XASSERT( bits_per_pixel==8||bits_per_pixel==16, bits_per_pixel );
    bytes_per_pixel = bits_per_pixel/8;

    /* +1 to convert from index to count */
    pixels_per_row = pic_right_margin - pic_left_margin + 1;

    if( pic_left_margin <= first_gap_pixels ) {
        pixel_pos = first_gap_pixels - pic_left_margin;
    }
    else {
        /* skip ahead looking for first segment boundary that contains our pic
         * margin
         */
        pixel_pos = first_gap_pixels;
        while( pixel_pos < pic_left_margin ) {
            pixel_pos += segment_size_pixels;
        }
        pixel_pos -= pic_left_margin;
    }

    dbg2( "%s pixel_pos=%d\n", __FUNCTION__, pixel_pos );

    src_segment[0].pixel_pos = 0;
    src_segment[0].num_pixels = pixel_pos;
    dst_segment[0].pixel_pos = 0;
    dst_segment[0].num_pixels = pixel_pos;

    num_segments = 1;

    src_pixel_pos = pixel_pos;
    dst_pixel_pos = pixel_pos+1;

    while( dst_pixel_pos < pixels_per_row ) {
        XASSERT( num_segments < MAX_SEGMENTS, num_segments );

        src_segment[num_segments].pixel_pos = src_pixel_pos;
        src_segment[num_segments].num_pixels = segment_size_pixels;

        dst_segment[num_segments].pixel_pos = dst_pixel_pos;
        dst_segment[num_segments].num_pixels = segment_size_pixels;

        src_pixel_pos += segment_size_pixels;
        dst_pixel_pos += segment_size_pixels + 1;

        num_segments++;
    }

    /* truncate last entry to max width of row */
    src_segment[num_segments-1].num_pixels = 
                    pixels_per_row - src_segment[num_segments-1].pixel_pos; 
    dst_segment[num_segments-1].num_pixels = 
                    pixels_per_row - dst_segment[num_segments-1].pixel_pos; 

    /* now that all the pixels are settled, compute everything in bytes */
    for( i=0 ; i<num_segments ; i++ ) {
        src_segment[i].byte_pos = src_segment[i].pixel_pos * bytes_per_pixel;
        src_segment[i].num_bytes = src_segment[i].num_pixels * bytes_per_pixel;

        dst_segment[i].byte_pos = dst_segment[i].pixel_pos * bytes_per_pixel;
        dst_segment[i].num_bytes = dst_segment[i].num_pixels * bytes_per_pixel;
    }
}

static void chipgap_interpolate_row( uint8_t *src_row, uint8_t *dst_row, int bytes_per_row  )
{
    int i;
    int ds, ss;
    uint8_t *endptr;

    /* temporary for debugging! */
//    memset( dst_row, 0, bytes_per_row );

    endptr = dst_row + bytes_per_row; 

//    dbg2( "%p %p %d end=%p\n", src_row, dst_row, bytes_per_row, endptr );

    /* copy the real data */
    for( i=0 ; i<num_segments ; i++ ) {
        ss = src_segment[i].byte_pos;
        ds = dst_segment[i].byte_pos;

        XASSERT( dst_row+ds+dst_segment[i].num_bytes <= endptr, 
                (uint32_t)(dst_row+ds+dst_segment[i].num_bytes) );

        memcpy( dst_row+ds, src_row+ss, dst_segment[i].num_bytes );
    }
}

void chipgap_interpolate_row_8bpp( uint8_t *src_row, uint8_t *dst_row, int bytes_per_row  )
{
    int i, pos; 
    uint32_t p1, p2;

    chipgap_interpolate_row( src_row, dst_row, bytes_per_row );

    /* now interpolate between the pixels at the gaps (note this loop starts at
     * i=1 rather than i=0 because the i=0 element starts at pixel zero)
     */
    for( i=1 ; i<num_segments ; i++ ) {
        pos = dst_segment[i].pixel_pos - 1;
        /* cast to uint32 to avoid overflow on the 8bpp pixel */
        p1 = (uint32_t)dst_row[pos-1];
        p2 = (uint32_t)dst_row[pos+1];
        dst_row[pos] = (uint8_t)NEW_PIXEL;
    }
}

void chipgap_interpolate_row_16bpp( uint8_t *src_row, uint8_t *dst_row, int bytes_per_row  )
{
    int i, pos; 
    uint32_t p1, p2;
    uint16_t *ptr16 = (uint16_t *)dst_row;

    chipgap_interpolate_row( src_row, dst_row, bytes_per_row );

    /* now interpolate between the pixels at the gaps (note this loop starts at
     * i=1 rather than i=0 because the i=0 element starts at pixel zero)
     */
    for( i=1 ; i<num_segments ; i++ ) {
        pos = dst_segment[i].pixel_pos - 1;
        /* cast to uint32 to avoid overflow on the 16-bpp pixel */
        p1 = (uint32_t)ptr16[pos-1];
        p2 = (uint32_t)ptr16[pos+1];
        ptr16[pos] = (uint16_t)NEW_PIXEL;
    }
}

