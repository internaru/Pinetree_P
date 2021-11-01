/*
 * ============================================================================
 * Copyright (c) 2010, 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef RASTER_ODMA_API_H
#define RASTER_ODMA_API_H

/**
 *
 *  \file raster_odma_api.h
 *
 *  \brief Function prototypes and defines for accessing the print image pipe
 *  raster output DMA hardware.
 *
 * 
*/

#include <stdint.h>
#include <stdbool.h>

#include "error_types.h"
#include "raster_odma_cfg.h"
#include "raster_odma_func_table.h"
#include "dma_api.h"


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

/**
 *
 * \brief Output DMA handle type
 *
 **/
typedef struct odma_handle_s odma_handle_t;


/* -------------------------------------------------------------------------- */
/*                   F U N C T I O N  P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */

/** 
 * \brief Initialize the raster output DMA driver. This function will initialize 
 * the raster output DMA driver. This must be called before using any other 
 * driver function. It will initialize the output DMA hardware and interface.
 **/
void raster_odma_init(void);


/**
 * \brief Handle DMA descriptor ownership errors
 * 
 * @param chan 
 */
void raster_odma_handle_ownership_error( uint32_t chan );
 

/**
 * \brief Respond to raster DMA interrupt events. For each DMA channel in the 
 * hardware, filter through each possible interrupt event and take appropriate 
 * action. 
 * 
 */
void raster_odma_handle_interrupt_event( void );


/**
 * \brief Specify a callback function to be called when an output DMA descriptor
 *        completes.
 * 
 * @param handle
 * @param function
 */
error_type_t raster_odma_register_notifcation_callback( odma_handle_t *handle,
                                                        odma_event_callback_t function );
  
/** blocking wait on done is operational with a null callback function
 */
void ODMA_WAIT( odma_handle_t *handle );

/**
 * \brief Open an ODMA channel
 * 
 * @param channel
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t raster_odma_open(odma_handle_t **handle, int32_t channel);


/**
 * \brief Close an ODMA channel. The ODMA channel will be closed and all 
 * descriptors created for it will be freed. The client must free all data 
 * buffers associated with the descriptors, however. 
 * 
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t raster_odma_close( odma_handle_t *handle );

/**
 * \brief Return a pointer to the function table interface. This interface is 
 *        intended to provide a seamless API for both the JBIG output DMA and
 *        the raster output DMA.
 * 
 * @param handle 
 * @param table 
 * 
 * @return error_type_t 
 */
error_type_t odma_get_odma_fn_ptr_table( odma_handle_t *handle,
                                         odma_func_ptr_t **table );


/**
 * \brief Query if an ODMA transaction is in progress. Primarily used by self 
 *        test software.
 * 
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t odma_query_in_progress(odma_handle_t *handle, bool *in_progress);


/**
 * \brief Perform a soft reset of the ODMA hardware, for the specified channel.
 * 
 * @param chan 
 */
void raster_odma_soft_reset( odma_channel_num_t chan );


/**
 * \brief Used to notify the DMA driver that a pending soft reset has completed.
 * 
 * @param chan 
 */
void raster_odma_soft_reset_complete( odma_channel_num_t chan );


/**
 * \brief Set the line width of the Print Image Pipe Raster output DMA. Note the 
 *        value is in bytes.
 * 
 * @param handle 
 * @param width 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_set_line_width_in_bytes( odma_handle_t *handle, 
                                                  uint32_t width );


/**
 * \brief Specify the input data width in bits per pixel. This must be 
 *        consistent with the settings for the entire Print Image Pipe. See
 *        hardware documentation for legal values. This directly manipulates the
 *        configuration register for the DMA channel specified in the handle.
 * 
 * @param handle 
 * @param input_data_width 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_input_width_in_bpp( odma_handle_t *handle, 
                                             uint8_t input_data_width);


/**
 * \brief Apply the raster output DMA configuration values to the hardware. 
 *        Typically called once at initialization as these values are product
 *        and hardware specific and do not change.
 * 
 * @param handle 
 * @param cfg 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_set_cfg(odma_handle_t *handle, raster_odma_cfg_t *cfg);


/**
 * \brief Release completed descriptors to the available descriptor list. The 
 *        descriptor parameter indicates the
 * 
 * @author dbartle (10/31/2011)
 * 
 * @param desc 
 * @param chan 
 */
void raster_odma_release_descriptors_to_available_list(uint32_t desc, uint32_t chan);


/**
 * \brief Add an input data buffer to the raster output DMA descriptor chain. 
 * 
 * @param handle 
 * @param data_buffer 
 * @param size_in_bytes 
 * @param num_lines 
 * @param SOI 
 * @param EOI 
 * @param user_data 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_add_input_buffer( odma_handle_t *handle, 
                                           char *data_buffer, //hw address
                                           uint32_t size_in_bytes,
                                           uint16_t num_lines,
                                           bool SOI,
                                           bool EOI,
                                           bool stop_on_finish,
                                           dma_config_data_t *user_data );


/**
 * \brief Start the DMA descriptor chain. If a transfer isn't already underway, 
 *        the next pending descriptor chain will start.
 * 
 * @param handle 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_start_xfer(odma_handle_t *handle);


/**
 * \brief Query if the DMA descriptor chain is active. Typically used for self 
 *        test where interrupts may not be used.
 * 
 * @param handle 
 * @param in_progress 
 * 
 * @return error_type_t 
 */
error_type_t raster_odma_query_is_dma_in_progress( odma_handle_t *handle, 
                                                   bool *in_progress);


/**
 * \brief Used for debug. Dump the contents of the hardware registers to the 
 *        serial output.
 * 
 */
void raster_odma_dump_regs( void );

#endif
