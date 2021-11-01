/******************************************************************************
 * Copyright (c) 2009-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_api.h
 *
 * \brief This file defines the API for the PIP module.
 *
 */

#ifndef __PIP_API_H_INCLUDED
#define __PIP_API_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>
#include "print_job_types.h"
#include "lassert.h"

///< lsptodo: redefine stats to match new HW.
typedef struct pip_stats_entry_s  
{
    uint32_t smin;
    uint32_t smax;
    uint32_t accum;
} pip_stats_entry_t;


/** power on initialization.
 */
void pip_init(void);

/** legacy interface lsptodo: remove/deprecate
 */
bool pip_does_page_use_image_pipe(PAGE_DATA *page);


error_type_t pip_prepare_page(uint32_t mode,
                              uint32_t int_scale_x,
                              uint32_t int_scale_y,
                              uint32_t input_width_in_pixels,
                              uint32_t frac_scale_width_in_pixels,
                              uint32_t input_height_in_lines,
                              uint32_t incremental_lines,
                              bool enable_pad,
                              bool enable_textmerge,
                              bool flip_horiz,
                              uint32_t *output_width_in_pixels);

/** lsptodo: deprecate
 *  was used to try to match input and output sizes so 1 input strip matched 1 output strip.
 *  no longer used.
 */
uint32_t pip_calculate_output_strip_height(uint32_t input_strip_height, bool start_of_image, bool end_of_image);

/** lsptodo: deprecate remove from public interface:
 */
error_type_t pip_generate_rows(uint32_t *flocs[],
                               uint32_t nlines,
                               pip_stats_entry_t *combined_stats,
                               pip_stats_entry_t chan_stats[],
			       bool soi,
			       bool eoi,
                               uint32_t total_lines);
/** lsptodo: deprecate remove from public interface:
 */
void pip_notify_rows_available(uint32_t num_rows, bool last_rows);

/** Used both to close a correctly imaged page and to abort/cancel a page in progress
 */
error_type_t pip_close_page(void);

/**
 * \brief Process a strip of 8bpp mono data and produce halftoned output.
 *
 * \param[in] mode PIP mode to use for processing.
 *
 * \param[in] data_in Reference to the input data buffer.
 *
 * \param[in] width_pix_in Input width in pixels.
 *
 * \param[in] lines_in Input height in lines.
 *
 * \param[in] scale_x Horizontal scale factor.
 *
 * \param[in] scale_y Vertical scale factor.
 *
 * \param[in] data_out_size Size of the output data buffer.
 *
 * \param[in/out] data_out Reference to the output data buffer (caller allocates memory).
 *
 * \retval positive if data_out contains strip data, 0 on OK continue, negative on fatal error.
 */
int pip_process_strip(uint32_t mode,
                       BigBuffer_t *bb_in,
                       uint32_t width_pix_in,
                       uint32_t lines_in,
                       uint32_t lines_out,
		       uint32_t output_bpp,
                       uint32_t page_height,
                       uint32_t scale_x,
                       uint32_t scale_y,
                       bool start_of_image,
                       bool end_of_image,
                       BigBuffer_t *data_out[6]);
/** lsptod: deprecate */
int pip_process_mono_strip(uint32_t mode,
                       BigBuffer_t *bb_in,
                       uint32_t width_pix_in,
                       uint32_t lines_in,
                       uint32_t lines_out,
   	 	       uint32_t output_bpp,
                       uint32_t page_height,
                       uint32_t scale_x,
                       uint32_t scale_y,
                       bool start_of_image,
                       bool end_of_image,
                       BigBuffer_t *data_out);

/** lsptodo: jbig mono interface
 */
error_type_t pip_start_jbig(uint8_t *data,
                            uint32_t source_buffer_size);
/** lsptodo: jbig mono interface
 */
error_type_t pip_add_jbig_buffer(uint8_t *data,
                                 uint32_t source_buffer_size);

/**
 * \brief Get the output bit depth of the specified print mode.
 *
 * \param[in] mode The print mode of interest.
 *
 * \retval bpp The output bit depth in bits per pixel.
 */
uint32_t pip_mode_bpp_get(uint32_t mode);

/**
 * \brief Get the name of the specified print mode.
 *
 * \param[in] mode The print mode of interest.
 *
 * \retval char * The name of the specified print mode. This is a string literal
 *                defined in OEM-specific pip_cfg.c. The caller is not expected
 *                to allocate memory for the string.
 */
const char *pip_mode_name_get(uint32_t mode);

/**
 * \brief Get the justification mode of the specified print mode.
 *
 * \param[in] mode The print mode of interest.
 *
 * \retval justification_mode_t The justification mode of the specified print mode.
 *         The definition of this enum can be found in print_job_types.h.
 */
justification_mode_t pip_mode_justification_get(uint32_t mode);

/**
 * \brief Find out if a given print mode is supported in this project.
 *
 * \param[in] mode The print mode that we want to check on.
 *
 * \retval bool True if the print mode is supported, false otherwise.
 */
bool pip_is_mode_supported(uint32_t mode);


/**
 * Video dma has 128bit alignment requirement. Input unpadded width may have other alignment 
 * requirements like ending on a cache boundary.  Compute the video required padded with base on 
 * input pixel width and output integer scaling.
 * 
 */
static inline uint32_t pip_padded_image_width( 
    uint32_t unpadded_width_in_pixels, //< PIXELS width of input raster
    uint32_t scale_x //< integer scale factor in X dimension: 1,2,4 
    )
{
    DBG_ASSERT( 128 == 128 ); //< HW VIDEO DMA bit alignment requirement 
    DBG_ASSERT( scale_x == 1 || scale_x == 2 || scale_x == 4 );
    return (scale_x * ((unpadded_width_in_pixels + 127 ) & ~127)) ;
}
#endif

