/*
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file pic_common.c
 *
 * \brief Hardware independent functions of PIC
 *
 * Originally separated from pic.c 30-Oct-2012. 
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "pic.h"
#include "scanvars.h"

/**
 * \brief  calculate how many margin pixels necessary to meet PIC WDMA burst
 * requirements
 *
 * If fitting_strategy is TRUNCATE, we'll subtract pixels from the right side
 * until we're DMA aligned.  If fitting strategy is PD, we'll add pixels to the
 * right side until we're DMA aligned.
 *
 * Why have both strategies?  We need to handle two cases: the sensor number of
 * pixels isn't DMA aligned and user scan area requests aren't DMA aligned. 
 *
 * If the sensor pixel count isn't DMA aligned, we have to shrink the number of
 * pixels. Otherwise, we'll get garbage. 
 *
 * If the user wants an area that's not DMA aligned, we'll pad out to DMA
 * aligned and send the user slightly more than she asked for. (We already have
 * that accounted for in the ASP protocol; we send pixels_per_row and
 * pixels_per_row_padded. The host software can trim the data down to match
 * exactly what the user asked.
 *
 * Why only adjust the right side?  It's easier. 
 *
 * \author David Poole
 * \date 20-Feb-2008
 *
 */

scan_err_t pic_marg_calc_fit( uint32_t left_margin_pixel_pos, 
                   uint32_t *right_margin_pixel_pos,
                   int bits_per_pixel, 
                   pic_marg_fit_t fitting_strategy )
{
    uint32_t bytes_per_pixel, pixels_per_burst, num_pixels_out;

    dbg2( "%s left=%d right=%d bpp=%d fit=%d\n", __FUNCTION__, 
                left_margin_pixel_pos, *right_margin_pixel_pos, bits_per_pixel,
                fitting_strategy );

    /* stupid human checks; right must be at least 4 pixels away from the left
     * (based on the current docs)
     *
     * will also catch goofs of sending right < left
     */
    if( left_margin_pixel_pos+4 >= *right_margin_pixel_pos ) {
        dbg1( "%s width too small\n", __FUNCTION__ );
        return SCANERR_INVALID_PARAM;
    }

    /* left_margin_pixel_pos and right_margin_pixel_pos are indexes NOT count.
     * 
     * For example, here's a sample sensor:
     *
     * 0         1         2         3         4         5         6         7
     * 01234567890123456789012345678901234567890123456789012345678901234567890
     *           L                                                   R
     *
     * left_margin_pixel_pos = 10
     * right_margin_pixel_pos = 62
     * num_pixels_out = 62 - 10 = 52
     *
     * num_pixels_out must be DMA aligned when BPP and DMA burst size are taken
     * into consideration.
     *
     * So we shrink or grow right_margin_pixel_pos until num_pixels_out is DMA
     * aligned.
     *
     */

    bytes_per_pixel = bits_per_pixel / 8;
    pixels_per_burst = ICE_DMA_BYTE_ALIGN / bytes_per_pixel;
    num_pixels_out = *right_margin_pixel_pos - left_margin_pixel_pos;

    if( fitting_strategy == PIC_MARG_PIXELFIT_PAD_RIGHT ) {
        /* pad (ceiling) 
         *
         * Example:
         *   32 byte alignment, 2 byte pixel = 16 pixel aligned
         *   
         *   Given 2553 pixels:
         *
         *   pixels_padded = ((2553+15)/16)*16 = 2560
         *
         */
        num_pixels_out = ((num_pixels_out+(pixels_per_burst-1))/pixels_per_burst)*pixels_per_burst;
    }
    else {
        /* truncate math is more straight forward */
        num_pixels_out = (num_pixels_out/pixels_per_burst)*pixels_per_burst;
    }

    *right_margin_pixel_pos = left_margin_pixel_pos + num_pixels_out;

    dbg2( "%s new right=%d pixels_out=%d bytes_out=%#x\n", __FUNCTION__, 
                *right_margin_pixel_pos, num_pixels_out, 
                (num_pixels_out*bits_per_pixel)/8 );

    /* make sure we're working with an atomic number of bytes */
    XASSERT( (bits_per_pixel*num_pixels_out) % 8 == 0, num_pixels_out );

    /* make sure my math works; make sure we really do have a number of pixels
     * when converted into bytes that really is exactly modulo our burst size
     */
    XASSERT( ((bits_per_pixel * num_pixels_out)/8)%ICE_DMA_BYTE_ALIGN==0, num_pixels_out );

    /* success! */
    return 0;
}


