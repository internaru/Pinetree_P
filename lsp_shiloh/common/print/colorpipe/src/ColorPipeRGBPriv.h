#ifndef __COLORPIPERGBPRIV_H__
#define __COLORPIPERGBPRIV_H__
/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /**
 * \file ColorPipeZxPriv.h
 *
 *
 *****************************************************************************/
#include <stdint.h>

#define CP_MAX_LINE_LEN (600 * 9)

typedef enum
{
    e_between_lines,
    e_idle,
    e_long_straight,
    e_long_run
}comp_state_t;

#define MAXPIXELBYTES 3

typedef struct cp_comp_s
{
    comp_state_t state;
    uint32_t num_pixels;
    uint8_t * len_pos;
    uint8_t * curr_pos;
    uint32_t num_bytes_in_line;
    uint8_t *pixel;
} cp_comp_t;

struct color_pipe_s
{
    uint32_t curr_line;
    cmyk_byte_array_t *color_planes_out;
    uint32_t output_strip_pitch;
    uint32_t lines_per_strip;
    uint32_t bytes_per_pixel;
    void(*strip_halftoned_callback)( void *callback_private_data, cmyk_byte_array_t *color_planes_out, uint32_t size ); 
    void *strip_callback_private_data;
    /* is something needed to hook up to resource manager?*/
    uint8_t * line_data;
    uint32_t num_pixels;
    uint8_t * curr_strip_pos;
    uint32_t num_bytes_written;
    uint32_t num_bytes_not_written;
    mlimiter_t * mlimiter;
    cp_comp_t comp;
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
