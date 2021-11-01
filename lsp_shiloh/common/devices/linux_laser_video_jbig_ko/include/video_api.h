/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef VIDEO_API_H
#define VIDEO_API_H

#include <stdint.h>
#include <stdbool.h>

#include "error_types.h"
#include "laser_engine_config_types.h"

/**
 *
 *  \file video_api.h
 *
 *  \brief Function prototypes and defines for accessing the video hardware.
 *  This includes the HiPWM block, the HVSync block and the Video block.
 *
 */

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

// Event notification defines that the outside world might care about
#define VIDEO_EVENT_END_OF_IMAGE           0x100
#define VIDEO_EVENT_LINK_OR_BUS_UNDERRUN   0x101
#define VIDEO_EVENT_OWNERSHIP_ERROR        0x102
#define VIDEO_EVENT_PREMATURE_END_OF_PAGE  0x103 // Bottom of page occurred before video data completed
#define VIDEO_LAST_EVENT                   0x104 // Last event tag.  Used for overloading the events by incrementing past this tag.


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */



/**
 * \brief Video handle type
 */
typedef struct video_handle_s video_handle_t;

/* -------------------------------------------------------------------------- */
/*                   F U N C T I O N  P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */

/** 
 * \brief Initialize the video driver.
 *
 * This function will initialize the video driver. This must be called before
 * using any other driver function. It will initialize the video hardware and
 * interface.
 **/
void viddrv_init(laser_engine_video_config_t* eng_video_config);

error_type_t viddrv_enable_video(video_handle_t *handle, bool enable);

/**
 * \brief This function cancels all outstanding video output on all open channels. 
 * It also resets the hvsync block.   The single cancel for all channels is necessary 
 * because the reset of the single hvsync block will affect all channels.  The 
 * hvsync block must be reset in order to reset the topqueues. 
 * 
 * @return error_type_t
 */
error_type_t viddrv_cancel( void );

error_type_t viddrv_reset_hardware_block(video_handle_t *handle);

uint32_t viddrv_get_channel_value(video_handle_t *handle);

// REVISIT should this be exposed or just done with video enable?
// error_type_t viddrv_enable_hipwm(video_handle_t *handle, bool enable);


error_type_t viddrv_add_input_buffer( video_handle_t *handle, 
                                     char *data_buffer,
                                     uint32_t buffer_size_in_bytes,
                                     uint32_t line_width_in_bytes,
                                     uint32_t num_lines_per_beam,
                                     uint32_t beam,
                                     void *user_data,
                                     bool SOI,
                                     bool EOI,
                                     bool blank, 
                                     bool right_to_left );


error_type_t viddrv_dma_start_xfer( video_handle_t *handle );


/**
 * \brief This function sets the lockout period for horizontal sync. The units
 *  are in HIPWM clock cycles.
 * 
 *  This function causes hsync pulses to be ignored for the specified time after
 *  a valid hsync pulse is received. This improves noise immunity.
 * 
 * @param lockout_in_hipwm_clocks
 * 
 * @return error_type_t
 */
error_type_t viddrv_set_hsync_lockout(uint32_t lockout_in_hipwm_clocks);

/**
 * \brief Program the top margin in lines.
 * 
 * @param handle
 * @param margin_in_lines
 * 
 * @return error_type_t
 */
error_type_t viddrv_program_top_margin(video_handle_t *handle, uint32_t margin_in_lines);

/**
 * \brief Program the side margin and side sub margin.
 * 
 * @param handle
 * @param margin_in_pixels
 * @param sub_margin
 * 
 * @return error_type_t
 */
error_type_t viddrv_program_side_margin(video_handle_t *handle, uint32_t margin_in_pixels, uint32_t sub_margin);


error_type_t viddrv_program_output_select_registers(void);


uint32_t viddrv_program_pixel_freq_default(video_handle_t *handle,
                                          uint32_t pixel_freq_in_hz);

error_type_t  viddrv_set_page_bpp(video_handle_t *handle, uint32_t output_bpp);

error_type_t viddrv_set_line_replication(video_handle_t *handle, uint32_t replicate);

error_type_t viddrv_set_line_skip(video_handle_t *handle, uint32_t num_lines_to_skip);

error_type_t viddrv_set_pulse_code_regs(video_handle_t *handle, 
                                       const uint32_t *array);

error_type_t viddrv_set_pulse_justification_regs(video_handle_t *handle, 
                                                const uint32_t *regArray);

error_type_t viddrv_set_auto_justification(video_handle_t *handle, uint32_t bpp);
error_type_t viddrv_set_centered_justification(video_handle_t *handle, uint32_t bpp);
error_type_t viddrv_clear_forced_justification( video_handle_t *handle );
error_type_t viddrv_set_forced_justification( video_handle_t *handle,  
                                             uint32_t init_just, 
                                             uint32_t toggle_just );

error_type_t viddrv_set_interrupt_mask(video_handle_t *handle,
                                      uint8_t interrupt_mask);

error_type_t viddrv_clear_interrupt_mask(video_handle_t *handle,
                                        uint8_t interrupt_mask);

error_type_t viddrv_enable_hvsync_interrupt( video_handle_t *handle, 
                                          bool enable );

error_type_t viddrv_enable_interrupt( video_handle_t *handle,
                                     bool enable );

// Adjust video hardware to allow the given print resolution data to be rendered
// correctly on a print engine with the specified native resolution.
// REVISIT move to application layer!!!
error_type_t viddrv_set_resolution(video_handle_t *handle,
                                  uint32_t pixel_freq_in_hz,
                                  resolution_t engine_resolution, 
                                  resolution_t print_resolution_x,
                                  resolution_t print_resolution_y);

/**
 * \brief Specify the line width in bytes for the video DMA session.
 * 
 * @param handle
 * @param line_width_in_bytes
 * 
 * @return error_type_t
 */
error_type_t viddrv_set_line_width(video_handle_t *handle,  uint32_t line_width_in_bytes);

/**
 * \brief Open a video channel
 * 
 * @param channel
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t viddrv_open( 
	video_handle_t **handle,
	uint32_t channel,
	bool top_to_bottom ///< false on backside of duplex longedge to rotate 180 the image.
	);

/**
 * \brief Close a video channel
 * 
 * The video channel will be closed and all descriptors created for it will be
 * freed. The client must free all data buffers associated with the descriptors,
 * however.
 * 
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t viddrv_close(video_handle_t *handle);

// Define the function pointer for the notification callback. 

typedef void (* viddrv_dma_callback_t)(void *handle, void *context);

error_type_t viddrv_register_dma_complete_callback( video_handle_t *handle,
                                                   viddrv_dma_callback_t func,
                                                   void *callback_handle );

// Define the function pointer for the video event callback. 
typedef void (* viddrv_event_callback_t)(video_handle_t *handle, uint32_t video_event, void* context);

// register the callback routine for the channel; note that the callback is called in ISR context.
error_type_t viddrv_register_event_callback(video_handle_t *handle, viddrv_event_callback_t event_callback, void* context);

error_type_t viddrv_paperless_setup(uint32_t chan);
error_type_t viddrv_paperless_enable(uint32_t chan, bool enable);
error_type_t viddrv_paperless_clear_CRC(uint32_t chan);
uint32_t viddrv_paperless_get_CRC(uint32_t chan);

error_type_t viddrv_query_pixel_count( video_handle_t *handle, 
                                      uint32_t *non_white_pixels,
                                      uint32_t *weighted_pixels );

#endif
