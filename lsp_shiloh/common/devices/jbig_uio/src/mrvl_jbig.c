/* ============================================================================
 * Copyright (c) 2010, 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_jbig.c
 * 
 * \brief See jbig_api.h.
 * 
 **/

#include <stdint.h>
#include <stdbool.h>
#include <string.h> // used for memcpy and memset
#include "ATypes.h"
#include "posix_ostools.h"
#include "ctype.h"
#include "lassert.h"
#include "memAPI.h"
#include "logger.h"
#include "error_types.h"
#include "jbigUtils.h"

#include "asic.h"
#include "jbig_api.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
#endif

#if defined (HAVE_UNIT_TEST) || defined (HAVE_CMD)
#include "jbig_test.h"
#endif

/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

#define JBIG_DBG_LVL (DBG_LOUD|DBG_OUTPUT)

#define DBG_PRFX "JBIG: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | LOGGER_SUBMODULE_BIT( 14 )

#define DBG_SLOG_ENTRY DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__); // DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);   // DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);
 

/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */

static const jbig_block_config_t *jbig_block_config_ptr = NULL;     // platform configuration parameters
static bool jbigHasBeenInitialized = FALSE;

/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */

#ifdef HAVE_UNIT_TEST
static int jbig_unit_test( void );
#endif

#ifdef HAVE_CMD
static int jbig_cmd_callback(int argc, char *argv[]);
#endif


/**
 * 
 * FUNCTIONS
 * 
 **/

/** 
 * \brief Initialize the JBIG driver
 *
 *  This function is called to initialize the JBIG thread, handles and stack.
 *  It has to be called before any other function.
 * 
 **/
 
void jbig_init()
{
#ifdef HAVE_CMD
    int result;
#endif
#ifdef HAVE_UNIT_TEST
    int unit_test_res;
#endif


    ASSERT(jbig_block_config_ptr == NULL);                         // has CdmaInit() already been called?
    jbig_block_config_ptr = jbig_platform_get_config();
    ASSERT(jbig_block_config_ptr != NULL);

    jbigInit(jbig_block_config_ptr);

#ifdef HAVE_CMD
    result = cmd_register( "jbig",
                           NULL,
                           NULL,
                           NULL,
                           jbig_cmd_callback);
	ASSERT( CMD_OK == result );
#endif

#ifdef HAVE_UNIT_TEST
    /* also register with the unit test manager */
    unit_test_res = unit_test_register_testcase("jbig", jbig_unit_test);
    ASSERT( 0 == unit_test_res );
#endif

    jbigHasBeenInitialized = TRUE;
}


error_type_t jbig_setup_decode_parms( jbig_handle_t *handle,
                                      bool enable_cbi,
                                      uint32_t Xd,  
                                      uint32_t Yd,
                                      uint32_t L0, 
                                      uint8_t option_flags )
{

    if (handle == NULL)
    {
        return FAIL;
    }

    // Setup the routing of the output of JBIG. CBI is common block interface 
    // and will route to the downstream hardware block. If CBI is not enabled,
    // JBIG output DMA will need to be configured to write the decompressed
    // image to memory.
    if( enable_cbi == true )
    {
        jbig_enable_cbi(handle);
    }
    else
    {
        jbig_disable_cbi(handle);
    }

    if (JBIG_LRLTWO & option_flags)
    {
        jbig_disable_3_line(handle);
    } 
    else
    {
        jbig_enable_3_line(handle);
    }

    if (JBIG_TPBON & option_flags)
    {
        jbig_enable_tpbon(handle);
    } 
    else
    {
        jbig_disable_tpbon(handle);
    }

    jbig_enable_atmov(handle);
       
    jbig_set_x_dimension(handle, Xd);
    jbig_set_y_dimension(handle, Yd);

    // Note: for inline host based color, the setting of L0 may use the
    // pPlane->Yd value instead of L0.
    jbig_set_stripe_length(handle, L0); 

    jbig_set_output_dma_line_width( handle, Xd/8 );

    return OK;
}

/**
 *
 **/

error_type_t jbig_open( jbig_handle_t **handle, JBIG_BLOCK block )
{
    error_type_t return_result = FAIL;

    DBG_SLOG_ENTRY

    if ( jbigHasBeenInitialized )
    {
        *handle = jbigAssignHandle( block );
        if ( *handle )
        {
            jbigResetHardware( *handle );
            return_result = OK;
        }
    }     

    DBG_SLOG_EXIT
    return return_result;
}

/**
 *
 **/
error_type_t jbig_close( jbig_handle_t * handle )
{
    error_type_t return_result = FAIL;
    int err;

    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL && handle->inUse )
    {
        
        if ( handle->started == 1)
        {
            // someone called close without calling stop transaction
            jbig_stop_transaction( handle );
        }

        handle->inUse = 0;
        
        // clear up the handle now in case someone is waiting on the semaphore they will run immediately
        // after the release of the semaphore
        err = sem_post(&handle->open_sem);
        ASSERT(err == 0);

        DBG_SLOG_EXIT
        return_result = OK;
    }

    DBG_SLOG_EXIT
    return return_result;
}

/**
 * 
 *
 *
 **/

error_type_t jbig_set_config(jbig_handle_t *handle, const JBIG_CONFIG *MyCfg)
{
    error_type_t returnValue = FAIL;

    if( jbigHasBeenInitialized )
    {
        if( handle && (handle->inUse == 1) && ( MyCfg != NULL ))
        {
            memcpy( &handle->config, MyCfg, sizeof( JBIG_CONFIG ) );
            returnValue = OK;
        }
    }
    return returnValue;
} 


static void jbig_blocking_op_callback(jbig_handle_t* handle, void* context, uint32_t status)
{
    uint32_t* out_status = context;

    ASSERT(out_status != NULL);
    ASSERT(handle != NULL);

    *out_status = status;
    sem_post(&handle->completion_sem);
}

/**
 * 
 *
 *
 **/

error_type_t jbig_encode_strip(jbig_handle_t *handle, BigBuffer_t* src_buf, BigBuffer_t* dest_buf, 
                               uint32_t Xd, uint32_t Yd, uint32_t L0, uint8_t option_flags, uint32_t* bytes_output)
{
    error_type_t ret_status = FAIL;
    uint32_t op_status = -1;
    uint32_t bytes_per_line = Xd/8;

    if(!jbigHasBeenInitialized || !handle || !handle->inUse)
    {
        return FAIL;
    }

    // callback will set op status pointer to operation completion value prior to signaling completion via semaphore
    jbig_set_event_callback(handle, jbig_blocking_op_callback, &op_status);
    jbig_set_idma_callback(handle, NULL );
    jbig_set_odma_callback(handle, NULL );

    // configure
    handle->config.Operation = JBIG_ENCODE;
    handle->config.CBI = JBIG_CBI_DISABLED;
    handle->config.Xd = Xd;
    handle->config.Yd = Yd;
    handle->config.Ld = L0;
    XASSERT(Xd > 0 && Xd%16 == 0, Xd);  // must be a mod16 value as per documentation of JX register of jbig codec
    handle->config.LineWidth = bytes_per_line;
    handle->config.AtmovState = ATMOV_DISABLED;
    handle->config.TpbonState = (option_flags & JBIG_TPBON) ? JBIG_TPBON_ENABLED : JBIG_TPBON_DISABLED;

    // add buffers
    ASSERT(bytes_per_line*Yd <= src_buf->datalen);
    jbig_add_input_buffer(handle, src_buf, bytes_per_line*Yd, NULL);
    jbig_add_output_buffer(handle, dest_buf, dest_buf->datalen, NULL);

    // start transaction
    ASSERT(sem_trywait(&handle->completion_sem) == -1);  // semaphore count should be at 0
    *bytes_output = 0;
    ret_status = jbig_start_transaction(handle);
    if (ret_status == OK)
    {
        ret_status = FAIL;
        // wait for completion; timeout of 5 seconds should be more than enough for any reasonable
        // buffer size (enough time to encode at least 125Mbytes)
        if(posix_sem_timedwait_ms(&handle->completion_sem, 5*MILLISEC_PER_SECOND) == 0)
        {
            if(op_status == 0)
            {
                jbig_read_bytes_output_last_transaction(handle, bytes_output);
                ret_status = OK;
            }
            else
            {
                DBG_PRINTF_ERR("%s: FAILED with block status %#x\n", op_status);
                jbig_dump_all_regs(handle);
                ASSERT(0);
            }
        }
        else
        {
            jbig_dump_all_regs(handle);
            ASSERT(0);
        }

        jbig_stop_transaction(handle);
    }

    return ret_status;
}

/**
 * 
 *
 *
 **/

error_type_t jbig_decode_strip(jbig_handle_t *handle, BigBuffer_t* src_buf, uint32_t src_buf_len_bytes, BigBuffer_t* dest_buf, 
                               uint32_t Xd, uint32_t Yd, uint32_t L0, uint8_t option_flags, uint32_t* bytes_output)
{
    error_type_t ret_status = FAIL;
    uint32_t op_status = -1;
    uint32_t bytes_per_line = Xd/8;

    if(!jbigHasBeenInitialized || !handle || !handle->inUse)
    {
        return FAIL;
    }

    // callback will set op status pointer to operation completion value prior to signaling completion via semaphore
    jbig_set_event_callback(handle, jbig_blocking_op_callback, &op_status);
    jbig_set_idma_callback(handle, NULL );
    jbig_set_odma_callback(handle, NULL );

    // configure
    handle->config.Operation = JBIG_DECODE;
    handle->config.CBI = JBIG_CBI_DISABLED;
    handle->config.Xd = Xd;
    handle->config.Yd = Yd;
    handle->config.Ld = L0;
    XASSERT(Xd > 0 && Xd%16 == 0, Xd);          // must be a mod16 value as per documentation of JX register of jbig codec
    handle->config.LineWidth = bytes_per_line;
    handle->config.AtmovState = ATMOV_ENABLED;
    handle->config.TpbonState = (option_flags & JBIG_TPBON) ? JBIG_TPBON_ENABLED : JBIG_TPBON_DISABLED;

    // add buffers
    ASSERT(bytes_per_line*Yd <= dest_buf->datalen);
    jbig_add_input_buffer(handle, src_buf, src_buf_len_bytes, NULL);
    jbig_add_output_buffer(handle, dest_buf, dest_buf->datalen, NULL);

    // start transaction
    ASSERT(sem_trywait(&handle->completion_sem) == -1);  // semaphore count should be at 0
    *bytes_output = 0;
    ret_status = jbig_start_transaction(handle);
    if (ret_status == OK)
    {
        ret_status = FAIL;
        // wait for completion; timeout of 5 seconds should be more than enough for any reasonable
        // buffer size (enough time to decode at least 125Mbytes)
        if(posix_sem_timedwait_ms(&handle->completion_sem, 5*MILLISEC_PER_SECOND) == 0)
        {
            if(op_status == 0)
            {
                jbig_read_bytes_output_last_transaction(handle, bytes_output);
                ret_status = OK;
            }
            else
            {
                DBG_PRINTF_ERR("%s: FAILED with block status %#x\n", __func__, op_status);
                jbig_dump_all_regs(handle);
                ASSERT(0);
            }
        }
        else
        {
            DBG_PRINTF_ERR("%s: Timeout\n", __func__);
            jbig_dump_all_regs(handle);
            ASSERT(0);
        }

        jbig_stop_transaction(handle);
    }

    return ret_status;
}


/**
 * 
 *
 *
 **/

error_type_t
jbig_started( jbig_handle_t * handle, uint8_t * started )
{
    error_type_t returnValue = FAIL;

    if( jbigHasBeenInitialized )
    {
        if( handle &&  (handle->inUse == 1))
        {
            if( handle->started == 1 )
            {
                *started = handle->started;
            }
            else
            {
                *started = 0;
            }
    
            returnValue = OK;
        }
    }
    return returnValue;
}

/**
 * 
 *
 *
 **/

error_type_t jbig_get_config(jbig_handle_t *handle, JBIG_CONFIG *MyCfg)
{

    error_type_t returnValue = FAIL;

    ASSERT(jbig_block_config_ptr != NULL);                         // has the driver been initialized?

    if( jbigHasBeenInitialized && handle &&  (handle->inUse == 1) && ( MyCfg != NULL ))
    {
    	memcpy( MyCfg, &handle->config, sizeof( JBIG_CONFIG ) );

		returnValue = OK;
    }

    return returnValue;
}

/**
 * \brief Set the JBIG event callback
 * 
 **/
error_type_t jbig_set_event_callback( jbig_handle_t *handle, JBIG_CALLBACK jbig_callback, void* context)
{
    if(handle != NULL && handle->inUse)
    {
        handle->jbigCallback = jbig_callback;
        handle->jbig_callback_context = context;
        return OK;
    }
    return FAIL;
}

/**
 * \brief Set the IDMA callback
 * 
 **/
error_type_t jbig_set_idma_callback( jbig_handle_t *handle, JBIG_DMA_CALLBACK idma_callback)
{
    if(handle != NULL && handle->inUse)
    {
        handle->idma_callback = idma_callback;
        return OK;
    }
    return FAIL;
}

/**
 * \brief Set the ODMA callback
 * 
 **/
error_type_t jbig_set_odma_callback( jbig_handle_t *handle, JBIG_DMA_CALLBACK odma_callback)
{
    if(handle != NULL && handle->inUse)
    {
        handle->odma_callback = odma_callback;
        return OK;
    }
    return FAIL;
}


/**
 * 
 *
 *
 **/

error_type_t
jbig_start_transaction( jbig_handle_t * handle )
{
	error_type_t return_result = FAIL;

    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL )
    {
        if( handle->started != 1 )
        {
            jbigResetHardware( handle );

            // Look  up the configuration for this handle and set up the JBIG registers accordingly
            if( jbigCommitConfig( handle ) == OK )
            {    
                handle->started = 1;
                handle->bytes_output = 0;
                handle->complete = false;
                handle->odma_complete = false;
                
                jbigStartWriteDMA( handle );
                jbigStartReadDMA( handle );
                jbigStart( handle );
    
                return_result = OK;
            }
        }
    }

    DBG_SLOG_EXIT
	return return_result;
}

error_type_t
jbig_read_bytes_output_last_transaction( jbig_handle_t *handle, uint32_t *bytes_output )
{
    if( handle == NULL )
    {
        return FAIL;
    }

    *bytes_output = handle->bytes_output;

    return OK;
}


/**
 * 
 *
 *
 **/

error_type_t
jbig_stop_transaction( jbig_handle_t * handle )
{
	error_type_t return_result = FAIL;
	
    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL && handle->inUse)
    {
        jbigStop( handle );

        handle->started = 0;
        
        jbigFreeHandleDescriptors( handle );

    	return_result = OK;
    }
    DBG_SLOG_EXIT
	return return_result;
}

/**
 * 
 *
 *
 **/

error_type_t
jbig_add_input_buffer(
		jbig_handle_t * handle,
		BigBuffer_t* buffer,
		uint32_t length,
		void * userData)
{
	error_type_t return_result = FAIL;

    DBG_MEMLOG(LOG_DEBUG_H, "%s data buffer 0x%x\n", __func__, buffer);

    if( jbigHasBeenInitialized && handle != NULL )
    {
        return_result = jbigAddReadDMABuffer( handle, buffer, length, userData );
    }
	return return_result;
}

error_type_t
jbig_add_output_buffer(
		jbig_handle_t * handle,
		BigBuffer_t* buffer,
		uint32_t length,
		void * userData)
{
	error_type_t return_result = FAIL;

    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL )
    {
        // Get a descriptor
        // Set descriptors buffer address and size
        // If this is first descriptor mark descriptor config bits accordingly
        // If this is last descriptor mark the descriptor config bits accordingly
        // If this is not the first descriptor set the previous descriptors "nextDescriptor" field to point
        // to this descriptor

        if( jbigAddWriteDMABuffer( handle, buffer, length, userData ) == OK )
        {
           return_result = OK;
        }
    }

    DBG_SLOG_EXIT

	return return_result;
}

/**
 * 
 *
 *
 **/

void jbig_enable_tpbon( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.TpbonState = JBIG_TPBON_ENABLED;
    }
}

/**
 * 
 *
 *
 **/

void jbig_disable_tpbon( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.TpbonState = JBIG_TPBON_DISABLED;
    }
}


/**
 * 
 *
 *
 **/

//-----------------------------------------
//	EN_ATMOV
//  Enable ATMOV for decode, encode operations 0 - ATMOV disabled 1 - ATMOV enabled - used with non-zero ATMOV value
//-----------------------------------------

void jbig_enable_atmov( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.AtmovState = ATMOV_ENABLED;
    }
}

/**
 * 
 *
 *
 **/

void jbig_disable_atmov( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.AtmovState = ATMOV_DISABLED;
    }
}

/**
 * 
 *
 *
 **/
//	ATMov value used during encode (if EN_ATMOV=1). Maximum Value is -x7F = 127 (decimal). The decoder
//	looks at ATMOV register and no the EN_ATMOV. If ATMOV is programmed to 0, then it looks for ATMOV
//	markers in the input data stream and if those markers are absent, then it starts decoding with no
//	ATMOV till it encounters a ATMOV marker in the input data stream. But if it is programmed with a
//	value between 1 - 127 it starts decoding the first stripe of the image with this ATMOV value. For
//	the next stripe it looks for a ATMOV marker in the input data stream, which it uses if present.
//	Otherwise it continues with the value programmed in the ATMOV register.
void jbig_clear_atmov_value( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.ATMOVvalue = 0;
    }
}

/**
 * 
 *
 *
 **/
void jbig_set_atmov_value( jbig_handle_t * handle, uint8_t value)
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.ATMOVvalue = value;
    }
}

/**
 * 
 *
 *
 **/
//-----------------------------------------
//	3 Line Template Enabled
//	3 Line Template enabled 0 - disabled: 2 line Template is used 1 - enabled: 3 line Template is used Note: Use 2 line template only
//-----------------------------------------
void jbig_enable_3_line( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.LineTemplate = JBIG_3_LINE_TEMPLATE;
    }
}

/**
 * 
 *
 *
 **/
void jbig_disable_3_line( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.LineTemplate = JBIG_LINE_TEMPLATE_DISABLED;
    }
}
/**
 * 
 *
 *
 **/
//-----------------------------------------
//	BPEND
//	Bypass Endian- This field determines how the the 32 bit value from the read DMA is broken into 16 bit words
//	in bypass mode. The 16 bit words are then passed to another unpack which ALWAYS unpacks in big endian
//	format. 0 - word0 = bits [15:0] 1 - word0 = bits [31:16]
//-----------------------------------------

void jbig_enable_bpend( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Endian = JBIG_BIG_ENDIAN;
    }
}

/**
 * 
 *
 *
 **/
void jbig_disable_bpend( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Endian = JBIG_LITTLE_ENDIAN;
    }
}

/**
 * 
 *
 *
 **/
//-----------------------------------------
//  Bypass Core
//	Bypass JBIG Core (Decode Only) 0 - Data Passes through JBIG core and is decompressed 1 - Data Bypasses
//	JBIG core and is NOT decompressed NOTE: Bypass always uses CBI as the output regardless of the state of
//	the enable CBI bit.
//-----------------------------------------

void jbig_enable_bypass( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.BypassCore = JBIG_BYPASS_CORE_ENABLED;
    }
}

/**
 * 
 *
 *
 **/
void jbig_disable_bypass( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.BypassCore = JBIG_BYPASS_CORE_DISABLED;
    }
}

/**
 * 
 *
 *
 **/
//-----------------------------------------
//  Enable CBI
//	Enable CBI Output 0 - Output of JBIG goes to the DMA 1 - Output of JBIG goes to the CBI
//-----------------------------------------

void jbig_enable_cbi( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.CBI = JBIG_CBI_ENABLED;
    }
}

/**
 * 
 *
 *
 **/
void jbig_disable_cbi( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.CBI = JBIG_CBI_DISABLED;
    }
}

/**
 * 
 *
 *
 **/
void jbig_set_decode( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Operation = JBIG_DECODE;
    }
}
/**
 * 
 *
 *
 **/
void jbig_set_encode( jbig_handle_t * handle )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Operation = JBIG_ENCODE;
    }
}

/**
 * 
 *
 *
 **/
void jbig_set_x_dimension( jbig_handle_t * handle, uint16_t xDimension )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Xd = xDimension;
    }
}

/**
 * 
 *
 *
 **/
void jbig_set_y_dimension( jbig_handle_t * handle, uint16_t yDimension )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Yd = yDimension;
    }
}

/**
 * 
 *
 *
 **/
void jbig_set_stripe_length( jbig_handle_t * handle, uint16_t stripLength )
{
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.Ld = stripLength;
    }
}


void jbig_set_output_dma_line_width( jbig_handle_t *handle, uint32_t line_width_in_bytes )
{
    DBG_MEMLOG(LOG_INFO,"jbig: jbig_set_write_line_width numBytes = %d\n", line_width_in_bytes );
    if( jbigHasBeenInitialized && handle && (handle->inUse == 1))
    {
        handle->config.LineWidth = line_width_in_bytes;
    }
}


#ifdef HAVE_UNIT_TEST
/**
 * 
 *
 *
 **/
static int jbig_unit_test( void )
{
    int i_res;

    /* run both for the plain unit test */
    i_res = jbig_full_test();

    return i_res;
}
#endif

#ifdef HAVE_CMD

static void jbig_display_dbg_usage()
{
        cmd_printf("Supported cmds,\n\treset\n");
#ifdef HAVE_UNIT_TEST
        cmd_printf("\ttest 1\n\ttest 2\n");
#endif
}
/**
 * jbig_cmd_callback
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
static int jbig_cmd_callback(int argc, char *argv[])
{
    int32_t pos;

    if (argc < 2)
    {
        jbig_display_dbg_usage();
        return 0;
    }

    pos = 1;

    if ( strncmp( (char *)argv[pos], "reset", 5 ) == 0 )
    {
        //hwPlatformReset();
    }
#ifdef HAVE_UNIT_TEST
    else if (strncmp((char *)argv[pos], "test",4) == 0)
    {
        if (argc < 3)
        {
            jbig_display_dbg_usage();
            return 0;
        }
    	if (strncmp((char *)argv[pos+1], "1",1) == 0)
    	{
            cmd_printf("Start JBIG Test 1\n");
            jbigTest();
    	}
    	else if (strncmp((char *)argv[pos+1], "2",1) == 0)
    	{
    		cmd_printf("Start JBIG Test 2\n");
            jbig_test_blocking_encode_decode();
    	}
    	else if (strncmp((char *)argv[pos+1], "3",1) == 0)
    	{
    		cmd_printf("Start JBIG Test 3\n");
            jbigSimpleTest();
//    		jbigDelayedOutputTest();
    	}
    }
#endif // HAVE_UNIT_TEST
    else
    {
        jbig_display_dbg_usage();
    }
    return OK;
}
#endif

