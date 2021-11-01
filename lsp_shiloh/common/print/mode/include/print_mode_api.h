/*
 * ============================================================================
 * Copyright (c) 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PRINT_MODE_API_H
#define PRINT_MODE_API_H

#include <stdint.h>
#include <stdio.h>

#include "print_job_types.h"
#include "print_mode_typedef.h"     // This is a product or OEM specific header file
                            // residing in the oem common/include directory
#include "print_mode.h"
/**
 *
 *  \file print_mode_api.h
 *
 *  \brief Function prototypes and defines for determining the print image pipe
 *         mode and subsequent hardware configuration.
 * 
 *  Note that the include file print_mode.h is a product or OEM specific file
 *  that defines print_mode_t across the entire set of files defining the
 *  product. It will be referenced in API files residing in the common code but
 *  it will be implemented and utilized in OEM specific source files.
 *
 */

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

#define THRESHOLD_HALFTONE 1
#define ERR_DIFFUSION_HALFTONE 2


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */


/**
 * 
 * \brief Used to obtain image pipe configuration information for the print 
 *  pipe. Uses information supplied in the page description to determine the
 *  configuration required to process the raw image data.
 * 
 * @param page 
 * 
 * @return print_mode_t* 
 */
print_mode_t *print_mode_get( PAGE_DATA *page);

uint32_t print_mode_get_bpp(print_mode_t *print_mode);

uint32_t print_mode_get_image_pipe_cfg( print_mode_t *print_mode );

uint32_t print_mode_get_input_bpp( print_mode_t * mode  );

uint32_t print_mode_get_output_bpp( print_mode_t * mode );

uint32_t print_mode_get_sizeof( void );

uint32_t print_mode_get_image_pipe_configuration(print_mode_t * print_mode );

uint32_t print_mode_get_image_width_in_pixels( print_mode_t *print_mode );

uint32_t print_mode_get_halftone( print_mode_t *print_mode );

void print_mode_get_color_space( print_mode_t *print_mode, input_color_space_t *color_space );
void print_mode_set_color_space( print_mode_t *print_mode, input_color_space_t color_space );


void print_mode_set_rendering_intent( print_mode_t *print_mode, uint32_t intent );
void print_mode_get_rendering_intent( print_mode_t *print_mode, uint32_t *intent );

void print_mode_get_color_table( print_mode_t *print_mode, uint32_t *color_table_index );


#endif

