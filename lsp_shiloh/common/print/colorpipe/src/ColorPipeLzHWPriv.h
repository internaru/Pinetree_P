#ifndef __COLORPIPELZHWPRIV_H__
#define __COLORPIPELZHWPRIV_H__
/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /**
 * \file ColorPipeLzHWPriv.h
 *
 *
 *****************************************************************************/
#include <stdint.h>

#define CP_MAX_LINE_LEN (600 * 9)
#define CP_MAX_LINES_STRIP 128
#define CP_STRIP_ALIGN 8

typedef enum cp_state_e
{
    IDLE,
    FILLING,
    CSC,
    HT_PLANE1,
    HT_PLANE2,
    HT_PLANE3,
    HT_PLANE4,
    HT_COMPLETE,
    ABORTING //I don't know if this is needed.
}cp_state_t;

typedef struct cp_strip_s
{
    cp_state_t state;
    color_space_conv_t color_space_conv;
    uint8_t * contone_strip_data;
    uint8_t * curr_line_ptr;
    uint8_t * curr_line_pos;
    uint8_t * last_line_ptr;//Must be NULL befor strip is reused.
    cmyk_byte_array_t * halftoned_strip_data;
}cp_strip_t;
struct color_pipe_s
{
    uint32_t pixel_per_line;
    uint32_t csc_pitch;
    uint32_t curr_line;
    cmyk_byte_array_t *color_planes_out;
    cp_strip_t strip[2];
    cp_strip_t *curr_strip_ptr;
    cp_strip_t *last_line_strip;
    uint32_t num_pixels;
    color_space_conv_t color_space_conv;
    uint32_t output_strip_pitch;
    uint32_t lines_per_strip;
    uint32_t bytes_per_pixel;
    void(*strip_halftoned_callback)( void *callback_private_data, cmyk_byte_array_t *color_planes_out, uint32_t size ); 
    void *strip_callback_private_data;
    mlimiter_t * mlimiter;
    bool page_init;
};

/* 8 bit pixel only */
#define PIX_COUNT_LUT_SIZE 256
typedef struct
{
    uint32_t C[PIX_COUNT_LUT_SIZE];
    uint32_t M[PIX_COUNT_LUT_SIZE];
    uint32_t Y[PIX_COUNT_LUT_SIZE];
    uint32_t K[PIX_COUNT_LUT_SIZE];
} pixel_count_data_t;

#endif // __COLORPIPEPRIV_H__
