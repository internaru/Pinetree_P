/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file grayline.h
 *
 * \brief functions for linear brightness/contrast 
 *
 */

#ifndef GRAYLINE_H
#define GRAYLINE_H

struct rectangular
{
    float x, y;
};

struct line 
{
    struct rectangular p1, p2;
};

void grayline_new( struct line *line, 
                   float p1_x, float p1_y,
                   float p2_x, float p2_y );

void grayline_to_lut( int num_entries, uint32_t lut[], struct line *line );

void grayline_enhance_contrast( float float_contrast, 
                                struct line *line_in,
                                struct line *line_out );

#endif

