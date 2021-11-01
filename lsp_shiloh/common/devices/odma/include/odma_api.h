/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef ODMA_API_H
#define ODMA_API_H

#include <stdint.h>
#include <stdbool.h>

#include "error_types.h"
#include "raster_odma_func_table.h"
#include "dma_api.h"
#include "BigBuffer.h"

/**
 *
 *  \file odma_api.h
 *
 *  \brief Function prototypes and defines for accessing the image pipe output
 *  DMA hardware.
 *
 * 
*/

/* **************************** T Y P E D E F S ***************************** */

/**
 *
 * \brief Output DMA channel type to define all possible DMA channels by name.
 *
 **/
typedef enum {
    ODMA_CYAN = 0,
    ODMA_MAGENTA = 1,
    ODMA_YELLOW = 2,
    ODMA_GRAY = 3,
    ODMA_BLACK = 4,
    ODMA_MIP = 5
} odma_channel_num_t;



// Function pointer prototype for DMA complete callback.
typedef void (*p_fn_dma_complete)(uint8_t* buffer);

// DMA configuration for the DMA session. A session is a series of DMA 
// transactions; e.g. the output DMA's for a printed page.
typedef struct ODMA_cfg_s {
    bool serpentine;
    bool line_reverse;
    bool transfer_reverse;
    bool input_on_upper_half;
    bool input_on_msb;
    bool enable_dma_block;
    uint8_t input_data_width;
    uint8_t burst_length;
} odma_cfg_t;


/**
 *
 * \brief Output DMA handle type
 *
 **/
typedef struct odma_handle_s odma_handle_t;


/* ****************************  D E F I N E S ****************************** */

#define BIT_WIDTH_1 0
#define BIT_WIDTH_2 1
#define BIT_WIDTH_4 2
#define BIT_WIDTH_8 3
#define BIT_WIDTH_16 4
#define BIT_WIDTH_24 6
#define BIT_WIDTH_32 5


/* ***************** F U N C T I O N  P R O T O T Y P E S ******************* */

/** 
 * \brief Initialize Output DMA driver.
 *
 * This function will initialize the output DMA driver. This must be called
 * before using any other driver function. It will initialize the output DMA
 * hardware and interface.
 **/
void odma_init(void);

/**
 * \brief ODMA callback function type
 *
 * This is the function type for the Output DMA callback function.
 **/
typedef void (*odma_callback_t)(dma_config_data_t *user_data); // uint8_t *data_buffer);


/**
 * 
 * \brief Create a descriptor chain for an output DMA channel.
 * 
 * This function creates a descriptor, attaches the specified data buffer to
 * it and inserts it in the descriptor chain for the channel specified by the
 * handle.
 * 
 * @param handle
 * @param data_buffer
 * 
 * @return error_type_t
 */
error_type_t odma_add_descriptor(odma_handle_t *handle, 
                                 uint8_t *data_buffer, 
                                 dma_config_data_t *user_data);

error_type_t odma_add_bigbuffer(odma_handle_t *handle, 
                                 BigBuffer_t *bigbuffer, 
                                 dma_config_data_t *user_data);


/**
 * \brief Start a new ODMA session.
 * 
 * @param handle
 */
error_type_t odma_start_new_session(odma_handle_t *handle,
                                    uint32_t bits_per_pixel, 
                                    uint32_t line_width_in_bytes); 


/**
 * \brief Specify a callback function to be called when an output DMA
 * descriptor completes.
 * 
 * @param handle
 * @param function
 */
void odma_callback_function(odma_handle_t *handle,
                            odma_callback_t function);


/**
 * \brief A data buffer for the channel specified by the handle has become
 * available.
 * 
 * @param handle
 */
void odma_data_buffer_available( void *handle,
                                 dma_config_data_t *data );


/**
 * \brief Specify the line width in bytes for the output DMA session.
 * 
 * @param handle
 * @param line_width_in_bytes
 * 
 * @return error_type_t
 */
// REVISIT DAB error_type_t odma_set_line_width(odma_handle_t *handle,  uint32_t line_width_in_bytes);


/**
 * \brief Setup an output DMA transaction.
 * 
 * @param handle
 * @param buffer_size
 * @param start_of_image
 * @param end_of_image
 * @param stop_on_completion
 * @param start_dma_chain
 */
void odma_setup_dma( odma_handle_t *handle,
                     uint8_t *p_buffer_data,
                     uint32_t buffer_size,
                     bool start_of_image,
                     bool end_of_image,
                     bool stop_on_completion,
                     bool start_dma_chain);


/**
 * \brief Open an ODMA channel
 * 
 * @param channel
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t odma_open(int32_t channel, odma_handle_t **handle);


/**
 * \brief Close an ODMA channel
 * 
 * The ODMA channel will be closed and all descriptors created for it will be
 * freed. The client must free all data buffers associated with the descriptors,
 * however.
 * 
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t odma_close(odma_handle_t *handle);

/**
 * \brief Abort any ODMA transaction in progress.
 * 
 * @param handle
 * 
 * @return error_type_t
 */
error_type_t odma_abort_transaction( odma_handle_t *handle );

error_type_t odma_query_is_dma_in_progress(odma_handle_t *handle, bool *in_progress);



/**
 * \brief Specify a callback function to be called when an output DMA
 * descriptor completes.
 * 
 * @param handle
 * @param function
 */
error_type_t pipr_odma_register_notifcation_callback( odma_handle_t *handle,
                                                      odma_event_callback_t function );


#endif
