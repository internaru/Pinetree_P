/*
 * ============================================================================
 * Copyright (c) 2010, 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file jbig_api.h
 * 
 * \brief JBIG driver.
 * 
 * This driver provides functionality to execute JBIG encoding and decoding.
 *  This file contains all driver api functions and data types.
 *
 * Call jbig_init() at system initialization before using any driver functions.
 *
 * To perform a JBIG decode:
 * - Open with jbig_open().
 * - Set the decode configuration parameters with
 * jbig_set_config().
 * - Execute the transfer with JbigTransfer().
 * - Close JBIG driver with jbig_close().
 *
 **/

#ifndef _JBIG_H
#define _JBIG_H

#include <stdint.h>
#include <stdbool.h>

#include "jbig.h"
#include "error_types.h"
#include "BigBuffer.h"


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
 * \brief JBIG Coded/Decode
 *
 * This defines which JBIG block will be used CODEC or DECODE. CODEC block
 * support both decode and encode and will DMA results to memory only. The
 * DECODE block will decode only but will allow the DMA to write directly to
 * the Davinci block.
 *
 **/

typedef enum
{
	JBIG_CODEC_BLOCK,
	JBIG_DECODE_BLOCK
} JBIG_BLOCK;

/**
 * \brief JBIG Operation
 *
 * This defines whether the JBIG block will be used to encode or decode 
 * using the JBIG compression algorithm.
 *
 **/

typedef enum
{
	JBIG_ENCODE,
	JBIG_DECODE
} JBIG_OPERATION;

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

typedef enum
{
	JBIG_LITTLE_ENDIAN,
	JBIG_BIG_ENDIAN
} JBIG_BYPASS_ENDIAN;

/**
 * \brief Bypass JBIG Decompression
 *
 * This defines whether the JBIG core will decompress incoming
 * data or simply passed it onto the CBI output regardless of the
 * state of the enable CBI setting.
 *
 **/

typedef enum
{
	JBIG_BYPASS_CORE_DISABLED,  //!< Data Passes through JBIG core and is decompressed
	JBIG_BYPASS_CORE_ENABLED	//!< Data passes through JBIG core and is NOT decompressed
} JBIG_BYPASS_CORE;

/**
 * \brief Enable CBI
 *
 * This defines whether the JBIG will go to DMA or
 * to CBI.
 *
 **/
typedef enum
{
	JBIG_CBI_DISABLED,  //!< Output of JBIG goes to DMA 1
	JBIG_CBI_ENABLED	//!< Output of JBIG goes to the CBI
} JBIG_CBI_STATE;

/** 
 * \brief JBIG configuration.
 **/
typedef struct
{
	JBIG_BLOCK				Block;          //!< Which JBIG block will be used CODEC or DECODE
	JBIG_OPERATION          Operation;       //!< Signifies whether JBIG will encode or decode incoming data
    JBIG_BURST_SIZE         SrcBurst;       //!< Source burst transfer size.
    JBIG_BPP				JbigBpp;		//!< Bits Per Pixel used to calculate data types
    JBIG_ATMOV_STATE		AtmovState;
    JBIG_LINE_TEMPLATE		LineTemplate;
    JBIG_TPBON_STATE		TpbonState;
    JBIG_BYPASS_ENDIAN		Endian;			//!< determines in bypass mode how 32 bit value is handled
    JBIG_BYPASS_CORE		BypassCore;
    JBIG_CBI_STATE			CBI;
    uint8_t					ATMOVvalue;
    uint32_t				Xd;
    uint32_t				Yd;
    uint32_t				Ld;
    uint32_t				LineWidth;      //!< line width in bytes

} JBIG_CONFIG;

/**
 *
 * \brief JBIG handle type
 *
 */
typedef struct jbig_handle_s jbig_handle_t;

/**
 * \brief JBIG callback function type
 *
 * \param user_data User defined data.
 * \param int_status JBIG interrupt status.
 * 
 **/
typedef void (*JBIG_CALLBACK)(jbig_handle_t* handle, void* context, uint32_t status);

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
typedef void (*JBIG_DMA_CALLBACK)(jbig_handle_t* handle, void* context);

/** 
 * \brief Initialize JBIG driver.
 *
 * This function will initialize the driver. This must be called before using any other driver function.
 **/
void jbig_init( void );


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
 * until jbig_close() is called.
 **/
error_type_t jbig_open( jbig_handle_t **handle, JBIG_BLOCK block );

/**
 * \brief Close the JBIG block.
 * 
 * \param handle Return pointer handle of opened decoder.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will attempt to open the JBIG block given in the block parameter.
 * After the jbig block is opened it will be reserved
 * until jbig_close() is called.
 **/
error_type_t jbig_close( jbig_handle_t *handle );

/**
 * \brief Set the JBIG event callback
 * 
 * \param handle handle to jbig channel 
 * \param jbig_callback JBIG callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_set_event_callback( jbig_handle_t *handle, JBIG_CALLBACK jbig_callback, void* context);

/**
 * \brief Set the IDMA callback
 * 
 * \param handle handle to jbig channel 
 * \param idma_callback IDMA callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_set_idma_callback( jbig_handle_t *handle, JBIG_DMA_CALLBACK idma_callback);

/**
 * \brief Set the ODMA callback
 * 
 * \param handle handle to jbig channel 
 * \param idma_callback ODMA callback function pointer 
 * 
 * \return OK if successful, FAIL if not.
 * 
 **/
error_type_t jbig_set_odma_callback( jbig_handle_t *handle, JBIG_DMA_CALLBACK odma_callback);

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

error_type_t jbig_set_config(jbig_handle_t *handle, const JBIG_CONFIG *config);

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
error_type_t jbig_get_config(jbig_handle_t *handle, JBIG_CONFIG *config);


/**
 * \brief Blocking API to encode a strip of data.  The strip will be encoded as a single 
 * jbig image.  On successful return the encode is complete.  The resulting number of bytes 
 * is contained in the bytes_output output parameter.  Note that this routine will reset 
 * the callback routines to internal versions and modify the config information. Thus if the block 
 * is being used in both blocking and asynchronous mode the callbacks and config will need to be reset 
 * after calling this API. 
 *  
 * If the dest_buffer is not large enough to contain the output data the operation will fail.  It is 
 * recommended that a buffer equal to the size of src_buf be provided for the output data and then 
 * resized as needed. 
 * 
 * @param handle 
 * @param src_buf - BigBuffer_t containing the input data; will be converted to a dma buffer 
 * @param dest_buf - BigBuffer_t output data buffer; will be converted to a dma buffer.
 * @param Xd - represents the padded data width, in bits, of each line in the image; must be a multiple of 8
 * @param Yd - represents the length in lines of the image.
 * @param L0 - JBIG lines per stripe; Typically the same as Yd.  Required to be identical to Yd if the image 
 * will be sent on the video driver. 
 * @param option_flags - JBIG option flags 
 * @param bytes_output - number of data bytes output by the encode operation.  This will be the number of output 
 * bytes contained in the dest_buf.  May be used to resize the dest_buf. 
 * 
 * @return error_type_t OK on success
 */
error_type_t jbig_encode_strip(jbig_handle_t *handle, BigBuffer_t* src_buf, BigBuffer_t* dest_buf, 
                               uint32_t Xd, uint32_t Yd, uint32_t L0, uint8_t option_flags, uint32_t* bytes_output);

/**
 * \brief Blocking API to decode a strip of data.  The strip to be decoded must be a single 
 * jbig image.  On successful return the decode is complete.  The resulting number of bytes 
 * is contained in the bytes_output output parameter.  Note that this routine will reset 
 * the callback routines to internal versions and modify the config information. Thus if the block 
 * is being used in both blocking and asynchronous mode the callbacks and config will need to be reset 
 * after calling this API. 
 *  
 * If the dest_buffer is not large enough to contain the output data the operation will fail.
 * 
 * @param handle 
 * @param src_buf - BigBuffer_t containing the input data; will be converted to a dma buffer 
 * @param dest_buf - BigBuffer_t output data buffer; will be converted to a dma buffer. 
 * @param src_buf_len_bytes - length in bytes of the data in the src_buf 
 * @param Xd - represents the padded data width, in bits, of each line in the image; must be a multiple of 8
 * @param Yd - represents the length in lines of the image.
 * @param L0 - JBIG lines per stripe; Typically the same as Yd.  Required to be identical to Yd if the image 
 * will be sent on the video driver. 
 * @param option_flags - JBIG option flags 
 * @param bytes_output - number of data bytes output by the encode operation.  This will be the number of output 
 * bytes contained in the dest_buf.  May be used to resize the dest_buf. 
 * 
 * @return error_type_t OK on success
 */
error_type_t jbig_decode_strip(jbig_handle_t *handle, BigBuffer_t* src_buf, uint32_t src_buf_len_bytes, BigBuffer_t* dest_buf, 
                               uint32_t Xd, uint32_t Yd, uint32_t L0, uint8_t option_flags, uint32_t* bytes_output);


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
error_type_t jbig_setup_decode_parms( jbig_handle_t *handle,
                                      bool enable_cbi,
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
jbig_add_input_buffer(
		jbig_handle_t * handle,
		BigBuffer_t* buffer,
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
jbig_add_output_buffer(
		jbig_handle_t * handle,
		BigBuffer_t* buffer,
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

error_type_t jbig_start_transaction( jbig_handle_t * handle );

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
jbig_stop_transaction( jbig_handle_t * handle );

error_type_t jbig_read_bytes_output_last_transaction( jbig_handle_t *handle, uint32_t *bytes_output );

void jbig_set_output_dma_line_width( jbig_handle_t *handle,
                                     uint32_t line_width_in_bytes );

// functions to configue the DECODE::JCTL register
void jbig_enable_tpbon( jbig_handle_t * handle );
void jbig_disable_tpbon( jbig_handle_t * handle );
void jbig_enable_atmov( jbig_handle_t * handle );
void jbig_disable_atmov( jbig_handle_t * handle );
void jbig_set_atmov_value( jbig_handle_t * handle, uint8_t value);
void jbig_clear_atmov_value( jbig_handle_t * handle );
void jbig_enable_3_line( jbig_handle_t * handle );
void jbig_disable_3_line( jbig_handle_t * handle );
void jbig_enable_bpend( jbig_handle_t * handle );
void jbig_disable_bpend( jbig_handle_t * handle );
void jbig_enable_bypass( jbig_handle_t * handle );
void jbig_disable_bypass( jbig_handle_t * handle );
void jbig_enable_cbi( jbig_handle_t * handle );
void jbig_disable_cbi( jbig_handle_t * handle );
void jbig_set_x_dimension( jbig_handle_t * handle, uint16_t xDimension );
void jbig_set_y_dimension( jbig_handle_t * handle, uint16_t yDimension );
void jbig_set_stripe_length( jbig_handle_t * handle, uint16_t stripLength);
void jbig_set_decode( jbig_handle_t * handle );
void jbig_set_encode( jbig_handle_t * handle );



error_type_t
jbig_started( jbig_handle_t * handle, uint8_t * started );


/** EXAMPLE DISABLED /example jbig_example.c
 * This is an example of a JBIG transfer.
 */

#ifdef __cplusplus
}
#endif
#endif

