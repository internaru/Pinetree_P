#ifndef __COLORPIPETABLES_H__
#define __COLORPIPETABLES_H__
/******************************************************************************
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file ColorPipe.h
 *
 *
 *****************************************************************************/
#include <stdint.h>
#include "agJobUtils.h"



typedef enum {
    e_ht_default,
    e_smooth,
    e_detail
}halftone_type_t;

/* Move to print_job_types.h
typedef enum {
    //e_linear_RGB, //may need to support
    e_sRGB,
    e_adobe_RGB,
    e_additive_grey,
    e_subtractive_grey
} input_color_space_t; 
*/
 
/**
 * @brief this sets the default parameters for determining which
 *        CSC and HT table to choose.
 * @WARNING The last parser to call this wins.  So, if parser A
 *          calls these calls and then does a
 *          color_pipe_page_open then finishes and parser B
 *          wakes up and calls these calls, parser B's values
 *          are now the color tables' values.  So EVERY time,
 *          before parser does a color_pipe_page_open it should
 *          re-set the values.  Any less often is at your own
 *          risk.
 */
void color_table_set_defaults( void );
/**
 * @brief this sets the %s parameter for determining which CSC
 *        and HT table to choose.
 * @WARNING See above
 * 
 * @param input_space
 */
void color_table_set_input_space(input_color_space_t input_space);

/**
 * @brief this sets the %s parameter for determining which CSC
 *        and HT table to choose.
 * 
 * @WARNING See above
 * 
 * @param output_bits_per_pixel
 */
void color_table_set_output_bits_per_pixel( uint32_t output_bits_per_pixel );

/**
 * @brief this sets the %s parameter for determining which CSC
 *        and HT table to choose.
 * @WARNING See above
 * 
 * @param halftone_type
 */
void color_table_set_halftone_type( halftone_type_t halftone_type );

/**
 * @brief this sets the %s parameter for determining which CSC
 *        and HT table to choose.
 * @WARNING See above
 * 
 * @param page_type
 */
void color_table_set_page_type( mediatype_t page_type );

/**
 * @brief this sets the %s parameter for determining which CSC
 *        and HT table to choose.
 * 
 * @WARNING See above
 * 
 * @param page_size
 */
void color_table_set_page_size( mediasize_t page_size );

void color_table_init_ACL(void);


#endif // __COLORPIPETABLES_H__
