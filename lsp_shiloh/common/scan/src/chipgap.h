/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file chipgap.h
 *
 * \brief functions for fixing chipgap with memcpy and linear interpolation
 *
 * davep 01-Feb-2012
 */

#ifndef CHIPGAP_H
#define CHIPGAP_H

void chipgap_debug_print_segments( void );

void chipgap_precalculate_segment_positions( uint32_t pic_left_margin, 
                                             uint32_t pic_right_margin, 
                                             int bits_per_pixel );

void chipgap_interpolate_row_8bpp( uint8_t *p_src_row, uint8_t *p_dst_row, int bytes_per_row  );
void chipgap_interpolate_row_16bpp( uint8_t *p_src_row, uint8_t *p_dst_row, int bytes_per_row  );

scan_err_t chipgap_set_positions( uint32_t first_gap, uint32_t gap_distance );
void chipgap_get_positions( uint32_t *first_gap, uint32_t *gap_distance );

#endif

