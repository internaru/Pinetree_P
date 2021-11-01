/*
 * ============================================================================
 * Copyright (c) 2010,2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file jbig_decode_api.h
 * 
 * \brief JBIG decode only driver.
 * 
 * This driver provides functionality to execute JBIG decoding only.  It is 
 * intended for use by the laser video control routines to decode JBIG 
 * compressed images prior to sending the image data to the laser video. 
 *
 * To perform a JBIG decode:
 * - Open with jbig_open().
 * - Set the decode configuration parameters with
 * jbig_set_config().
 * - Execute the transfer with JbigTransfer().
 * - Close JBIG driver with jbig_close().
 *
 **/

#ifndef _JBIG_DECODE_API_H
#define _JBIG_DECODE_API_H

#include <stdint.h>
#include "error_types.h"
#include "jbig_decode_config.h"

#ifdef __cplusplus
extern "C" {
#endif



/** 
 * \brief definitions for event handling
 * 
 * This defines the bitmasks used in the JBIG event callback
 **/ 

// definitions for event handling
#define JBIG_IDMA_COMPLETE_FLAG 0x10000000      // input (read) DMA is complete
#define JBIG_ODMA_COMPLETE_FLAG 0x20000000      // output (write) DMA is complete
#define JBIG_CONTROL_FLAG 0x40000000            // jbig control event; typically encode/decode complete
#define JBIG_STOP_FLAG 0x80000000               // unsupported
#define JBIG_EVENT_FLAG_MASK 0xF0000000
#define JBIG_EVENT_STATUS_MASK 0x0FFFFFFF
#define JBIG_ODMA_LINES_LEFT_MASK 0x0FFFFFFF

/*	Order flags  */

#define JBIG_HITOLO     0x08
#define JBIG_SEQ        0x04
#define JBIG_ILEAVE     0x02
#define JBIG_SMID       0x01

/*	Option flags  */

#define JBIG_LRLTWO     0x40
#define JBIG_VLENGTH    0x20
#define JBIG_TPDON      0x10
#define JBIG_TPBON      0x08
#define JBIG_DPON       0x04
#define JBIG_DPPRIV     0x02
#define JBIG_DPLAST     0x01


/** 
 * \brief Transfer width in bits.
 * 
 * This defines the size of the word transfer.
 **/ 
typedef enum
{
    JBIG_8_BITS,            //!< 8 bit.
    JBIG_16_BITS,           //!< 16 bit.
    JBIG_32_BITS,           //!< 32 bit.
} JBIG_XFER_WIDTH;

/** 
 * \brief Burst transfer size.
 * 
 * This defines throughput performance of the device.  Consult
 * hardware specifications when selecting a size.  If the
 * transfer width is less than 32 bits or the device is
 * non-bursting, then the size must be #CDMA_BURST_1.
 * 
 * Typically a transfer to/from memory can use the maximum size.
 **/
typedef enum
{
    JBIG_BURST_1,           //!< Size 1.
    JBIG_BURST_2,           //!< Size 2.
    JBIG_BURST_4,           //!< Size 4.
    JBIG_BURST_8            //!< Size 8.
} JBIG_BURST_SIZE;


typedef enum
{
	ATMOV_DISABLED,
	ATMOV_ENABLED
}JBIG_ATMOV_STATE;

typedef enum
{
	JBIG_BPP_1,		//!< 1 bit per pixel.
	JBIG_BPP_2,		//!< 2 bit per pixel.
	JBIG_BPP_4		//!< 4 bit per pixel.
}JBIG_BPP;

/**
 * \brief Line Template
 *
 * This defines which line template to use during JBIG
 * decode/encode.
 *
 * \warning Currently only 2 Line template should be used
 **/

typedef enum
{
	JBIG_LINE_TEMPLATE_DISABLED,
	JBIG_2_LINE_TEMPLATE,
	JBIG_3_LINE_TEMPLATE
} JBIG_LINE_TEMPLATE;

/**
 * \brief Typical Line Detection ( prediction )
 *
 * This defines whether type line detection ( prediction )
 * will be enabled for JBIG decoding/encoding.
 *
 **/

typedef enum
{
	JBIG_TPBON_DISABLED,
	JBIG_TPBON_ENABLED
} JBIG_TPBON_STATE;

/**
 * \brief Bypass Endian
 *
 * This field determines how the the 32 bit value from the read DMA
 * is broken into 16 bit words in bypass mode. The 16 bit words are
 * then passed to another unpack which ALWAYS unpacks in big endian
 * format.
 *
 **/
// TODO: is this still needed?
typedef enum
{
	JBIG_LITTLE_ENDIAN,
	JBIG_BIG_ENDIAN
} JBIG_BYPASS_ENDIAN;


/** 
 * \brief JBIG configuration.
 **/
typedef struct
{
    JBIG_BURST_SIZE         SrcBurst;       //!< Source burst transfer size.
    JBIG_BPP				JbigBpp;		//!< Bits Per Pixel used to calculate data types
    JBIG_ATMOV_STATE		AtmovState;
    JBIG_LINE_TEMPLATE		LineTemplate;
    JBIG_TPBON_STATE		TpbonState;
    JBIG_BYPASS_ENDIAN		Endian;			//!< determines in bypass mode how 32 bit value is handled
    uint8_t					ATMOVvalue;
    uint32_t				Xd;
    uint32_t				Yd;
    uint32_t				Ld;
    uint32_t				LineWidth;
} JBIG_CONFIG;

typedef struct jbig_decode_handle_s jbig_decode_handle_t;

/**
 * \brief JBIG callback function type
 *
 * \param user_data User defined data.
 * \param int_status JBIG interrupt status.
 * 
 **/
typedef void (*JBIG_CALLBACK)(jbig_decode_handle_t* handle, void* context, uint32_t status);

/**
 * \brief JBIG DMA callback function type. Used for both IDMA 
 *        and ODMA.
 *  
 * \param handle pointer to jbig handle on which the ODMA event 
 *        occurred.
 * \param context Pointer to context data supplied by user when 
 *        the buffer was submitted to the DMA.
 * \param status DMA completion status for this buffer.
 * 
 **/
typedef void (*JBIG_DMA_CALLBACK)(jbig_decode_handle_t* handle, void* context);

#define NUM_JBIG_DMA_DESCS_PER_BLOCK 16
typedef struct jbig_dma_desc_s jbig_dma_desc_t;

typedef struct _jbig_dma_descriptor_block
{
    jbig_dma_desc_t* jbig_descs[NUM_JBIG_DMA_DESCS_PER_BLOCK];
    uint32_t next_descriptor;
    uint32_t last_descriptor;
    uint32_t num_descriptors_in_use;
    JBIG_DMA_CALLBACK dma_callback;
}jbig_dma_descriptor_block_t;


/**
 *
 * \brief JBIG handle type
 *
 */
struct jbig_decode_handle_s
{
    uint32_t    channel;
    JBIG_CONFIG config;
    bool        inUse;
    uint32_t    lastError;
    bool		started;
    uint32_t    linesRemaining;
    
    jbig_dma_descriptor_block_t idma_desc_block;
    jbig_dma_descriptor_block_t odma_desc_block;
    
    // pointer to JBIG event callback given by user
    JBIG_CALLBACK  jbigCallback;
    void* jbig_callback_context;

    // JBIG block data including block type, register interface pointers, and int nums.
    const jbig_decode_block_config_t* jbig_block_interface;
};


/** 
 * \brief Initialize JBIG driver.
 *
 * This function will initialize the driver. This must be called before using any other driver function.
 **/
void jbig_decode_init( void );


/**
 * \brief Open the given JBIG block.
 * 
 * \param handle Return pointer handle of opened decoder.
 *        block signify JBIG_DECODE or JBIG_CODEC
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will attempt to open the JBIG block given in the block parameter.
 * After the jbig block is opened it will be reserved
 * until jbig_decode_close() is called.
 **/
error_type_t jbig_decode_open( jbig_decode_handle_t **handle );

/**
 * \brief Close the JBIG block.
 * 
 * \param handle Return pointer handle of opened decoder.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will attempt to open the JBIG block given in the block parameter.
 * After the jbig block is opened it will be reserved
 * until jbig_decode_close() is called.
 **/
error_type_t jbig_decode_close( jbig_decode_handle_t *handle );

/**
 * \brief Set the JBIG event callback
 * 
 * \param handle handle to jbig channel 
 * \param jbig_callback JBIG callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_decode_set_event_callback( jbig_decode_handle_t *handle, JBIG_CALLBACK jbig_callback, void* context);

/**
 * \brief Set the IDMA callback
 * 
 * \param handle handle to jbig channel 
 * \param idma_callback IDMA callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_decode_set_idma_callback( jbig_decode_handle_t *handle, JBIG_DMA_CALLBACK idma_callback);

/**
 * \brief Set the ODMA callback
 * 
 * \param handle handle to jbig channel 
 * \param idma_callback ODMA callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_decode_set_odma_callback( jbig_decode_handle_t *handle, JBIG_DMA_CALLBACK odma_callback);


/** 
 * \brief Set the channel configuration.
 * 
 * \param handle The handle of the channel to configure.
 * \param config The address of the configuration.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will set the configuration for an opened channel.
 * 
 * \warning Do not set the channel configuration while waiting
 * for a CdmaTransfer() call to complete.
 **/

error_type_t jbig_decode_set_config(jbig_decode_handle_t *handle, const JBIG_CONFIG *config);

/** 
 * \brief Get the channel configuration
 * 
 * \param handle The handle of the channel.
 * \param config Return configuration to the address of this location.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will get the configuration for an opened channel.
 **/

error_type_t jbig_decode_get_config(jbig_decode_handle_t *handle, JBIG_CONFIG *config);


/**
 * \brief Setup the decode parameters for JBIG host based print data.
 * 
 * @param handle 
 * @param Xd - represents the padded data width, in bits, of each line in the video buffer
 * @param Yd 
 * @param L0 - JBIG lines per stripe
 * @param option_flags - JBIG option flags
 * 
 * @return error_type_t 
 */
error_type_t jbig_decode_set_parms( jbig_decode_handle_t *handle,
                                      uint32_t Xd,  
                                      uint32_t Yd,
                                      uint32_t L0, 
                                      uint8_t option_flags ); 

/**
 * \brief Add a buffer to JBIG input
 *
 * \param handle The handle of the decoder/encoder.
 *
 * \return OK if successful, FAIL if not.
 *
 * This function will attempt to add an additional buffer
 * to the current JBIG input process.
 *
 **/
error_type_t
jbig_decode_add_input_buffer(
		jbig_decode_handle_t * handle,
		void* buffer,
		uint32_t length,
		void * userData );


/**
 * \brief Add a buffer to JBIG output
 *
 * \param handle The handle of the decoder/encoder.
 *
 * \return OK if successful, FAIL if not.
 *
 * This function will attempt to add an additional buffer
 * to receive the results of the current JBIG output process.
 *
 **/
error_type_t
jbig_decode_add_output_buffer(
		jbig_decode_handle_t * handle,
		uint8_t* buffer,
		uint32_t length,
		void * userData);

/**
 * \brief start a JBIG process
 *
 * \param handle The handle of the decoder/encoder.
 *
 * \return OK if successful, FAIL if not.
 *
 * This function will attempt to start the JBIG
 * process associated with the given handle.
 *
 **/

error_type_t
jbig_decode_start_transaction( jbig_decode_handle_t * handle );

/**
 * \brief stop a JBIG process
 *
 * \param handle The handle of the decoder/encoder.
 *
 * \return OK if successful, FAIL if not.
 *
 * This function will stope the JBIG
 * process associated with the given handle.
 *
 **/

error_type_t
jbig_decode_stop_transaction( jbig_decode_handle_t * handle );

error_type_t
jbig_decode_read_bytes_left_to_transfer_output_dma( jbig_decode_handle_t *handle,
                                             uint32_t *bytes_left );


static inline void jbig_decode_set_output_dma_line_width( jbig_decode_handle_t *handle,
                                     uint32_t line_width_in_bytes )
{
    ASSERT(handle != NULL);
	handle->config.LineWidth = line_width_in_bytes;
}

// functions to configue the DECODE::JCTL register
static inline void jbig_decode_enable_tpbon( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
	handle->config.TpbonState = JBIG_TPBON_ENABLED;
}

static inline void jbig_decode_disable_tpbon( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
	handle->config.TpbonState = JBIG_TPBON_DISABLED;
}

static inline void jbig_decode_enable_atmov( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
    handle->config.AtmovState = ATMOV_ENABLED;
}

static inline void jbig_decode_disable_atmov( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
    handle->config.AtmovState = ATMOV_DISABLED;
}

/**
*   \brief Set ATMov value used during encode (if EN_ATMOV=1). Maximum Value is -x7F = 127 (decimal).
*   The decoder looks at the ATMOV register and not the EN_ATMOV. If ATMOV is programmed to 0, then
*   it looks for ATMOV markers in the input data stream and if those markers are absent, then it starts
*   decoding with no ATMOV till it encounters a ATMOV marker in the input data stream. But if it is
*   programmed with a 	value between 1 - 127 it starts decoding the first stripe of the image with
*   this ATMOV value. For the next stripe it looks for a ATMOV marker in the input data stream,
*   which it uses if present.  Otherwise it continues with the value programmed in the ATMOV register.
*/
static inline void jbig_decode_set_atmov_value( jbig_decode_handle_t * handle, uint8_t value)
{
    ASSERT(handle != NULL);
    handle->config.ATMOVvalue = value;
}

static inline void jbig_decode_clear_atmov_value( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
    handle->config.ATMOVvalue = 0;
}

/**
*	\brief	Bypass Endian-  BPEND This field determines how the the 32 bit value from the read DMA is broken into 16 bit words
*	in bypass mode. The 16 bit words are then passed to another unpack which ALWAYS unpacks in big endian
*	format. 0 - word0 = bits [15:0] 1 - word0 = bits [31:16]
*/
static inline void jbig_decode_enable_bpend( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
    handle->config.Endian = JBIG_BIG_ENDIAN;
}

static inline void jbig_decode_disable_bpend( jbig_decode_handle_t * handle )
{
    ASSERT(handle != NULL);
    handle->config.Endian = JBIG_LITTLE_ENDIAN;
}

static inline void jbig_decode_set_x_dimension( jbig_decode_handle_t * handle, uint16_t xDimension )
{
    ASSERT(handle != NULL);
	handle->config.Xd = xDimension;
}

static inline void jbig_decode_set_y_dimension( jbig_decode_handle_t * handle, uint16_t yDimension )
{
    ASSERT(handle != NULL);
	handle->config.Yd = yDimension;
}

static inline void jbig_decode_set_stripe_length( jbig_decode_handle_t * handle, uint16_t stripLength)
{
    ASSERT(handle != NULL);
    handle->config.Ld = stripLength;
}


static inline bool jbig_is_decode_started( jbig_decode_handle_t * handle)
{
    ASSERT(handle != NULL);
    return handle->started;
}


/**
* \brief
* 
* \param handle The handle of the decoder/encoder.
*
* \return OK if successful, FAIL if not.
*
* This function will stope the JBIG
* process associated with the given handle.
**/

error_type_t
jbig_decode_get_last_error_code(
	jbig_decode_handle_t * handle,
    uint32_t *ptr ///< pointer to uint32_t to store last error code
    );

/**
 * \brief Debug routine to dump all jbig registers for the blocks associated with the given handle
 */
void jbig_dump_all_regs(jbig_decode_handle_t * handle);

#ifdef __cplusplus
}
#endif
#endif

