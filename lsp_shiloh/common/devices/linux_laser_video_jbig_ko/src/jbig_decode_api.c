/* ============================================================================
 * Copyright (c) 2010, 2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file jbig_decode_api.c
 * 
 * \brief See jbig_decode_api.h.
 * 
 **/

#include <stdint.h>
#include <stdbool.h>

#include "dros.h"
#include "lassert.h"
#include "asic.h"
#include "error_types.h"
#include "jbig_decode_api.h"
#include "jbig_decode_utils.h"


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */


//=========================================================================================================================
// Debug configuration
//=========================================================================================================================
#ifdef __KERNEL__
#define DBG_PRFX "JBIGD: "
//#define DBG_SLOG_ENTRY printk(KERN_DEBUG DBG_PRFX "==> %s\n", __func__);
//#define DBG_SLOG_EXIT printk(KERN_DEBUG DBG_PRFX "<== %s\n", __func__);
//#define MEMLOG(...) printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)

#define DBG_SLOG_ENTRY
#define DBG_SLOG_EXIT
#define MEMLOG(...)
#define ERRLOG(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__)

#else
#include "devices_logger.h"
#define DBG_PRFX "JBIGD: "

#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_JBIG
#define DBG_SLOG_ENTRY DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__); // DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);   // DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);
#define MEMLOG(...) DBG_MEMLOG(LOG_DEBUG_H, DBG_PRFX __VA_ARGS__);   // DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);
#endif 




/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */

static const jbig_decode_block_config_t *jbig_decode_block_config_ptr = NULL;     // platform configuration parameters
static dros_sem_t jbig_decode_semaphore;        // JbigDecodeOpen is in use
static jbig_decode_handle_t * currentTransactionHandle;
static bool jbigHasBeenInitialized = false;

/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */


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
 
void jbig_decode_init()
{
    uint32_t status;
#ifdef HAVE_CMD
    int result;
#endif

    ASSERT(jbig_decode_block_config_ptr == NULL);                         // has CdmaInit() already been called?
    jbig_decode_block_config_ptr = jbig_decode_get_block_config();
    ASSERT(jbig_decode_block_config_ptr != NULL);

    jbigInit(jbig_decode_block_config_ptr);

    status = dros_sem_init(&jbig_decode_semaphore, "jbig_decode_sem", 1);
    ASSERT(status == 0);

#ifdef HAVE_CMD
    result = cmd_register( "jbigapi",
                           NULL,
                           NULL,
                           NULL,
                           jbig_decode_debug_callback);
	ASSERT( CMD_OK == result );
#endif

    jbigHasBeenInitialized = true;
}


error_type_t jbig_decode_set_parms( jbig_decode_handle_t *handle,
                                      uint32_t Xd,  
                                      uint32_t Yd,
                                      uint32_t L0, 
                                      uint8_t option_flags )
{

    if (handle == NULL)
    {
        return FAIL;
    }

    if (JBIG_TPBON & option_flags)
    {
        jbig_decode_enable_tpbon(handle);
    } 
    else
    {
        jbig_decode_disable_tpbon(handle);
    }

    jbig_decode_enable_atmov(handle);
    jbig_decode_set_atmov_value(handle, 0);

    jbig_decode_set_x_dimension(handle, Xd);
    jbig_decode_set_y_dimension(handle, Yd);

    // Note: for inline host based color, the setting of L0 may use the
    // pPlane->Yd value instead of L0.
    jbig_decode_set_stripe_length(handle, L0); 

    jbig_decode_set_output_dma_line_width( handle, Xd/8 );

    return OK;
}

/**
 * \brief Open handle to jbig decode driver instance
 **/
error_type_t jbig_decode_open( jbig_decode_handle_t **handle )
{
    error_type_t return_result = FAIL;
    dros_err_t status;

    DBG_SLOG_ENTRY

    ASSERT(jbig_decode_block_config_ptr != NULL);                         // has the driver been initialized?

    if( jbigHasBeenInitialized )
    {
        // is JbigDecodeOpen already in use?
        status = dros_sem_wait(&jbig_decode_semaphore);
        ASSERT(status == DROSERR_NONE);

        *handle = jbig_decode_assign_handle();

        if( *handle )
        {            
            jbigResetHardware( *handle );
            return_result = OK;
        }
        else
        {
            status = dros_sem_post(&jbig_decode_semaphore);
        }
    }

    DBG_SLOG_EXIT
    return return_result;
}

/**
 *
 **/
error_type_t jbig_decode_close( jbig_decode_handle_t * handle )
{
    error_type_t return_result = FAIL;

    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL && handle->inUse )
    {    
        handle->jbigCallback = NULL;
        handle->odma_desc_block.dma_callback = NULL;
        handle->idma_desc_block.dma_callback = NULL;
            
        if ( handle->started == true)
        {
            // someone called close without calling stop transaction
            jbig_decode_stop_transaction( handle );
        }

        // clear up the handle now in case someone is waiting on the semaphore they will run immediately
        // after the release of the semaphore
        handle->inUse = false;
        
        // return JbigDecodeOpen to not in use       
        dros_sem_post(&jbig_decode_semaphore);

        return_result = OK;
    }

    DBG_SLOG_EXIT
    return return_result;
}

/**
 * \brief Set the JBIG event callback
 * 
 **/
error_type_t jbig_decode_set_event_callback( jbig_decode_handle_t *handle, JBIG_CALLBACK jbig_callback, void* context)
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
error_type_t jbig_decode_set_idma_callback( jbig_decode_handle_t *handle, JBIG_DMA_CALLBACK idma_callback)
{
    if(handle != NULL && handle->inUse)
    {
        handle->idma_desc_block.dma_callback = idma_callback;
        return OK;
    }
    return FAIL;
}

/**
 * \brief Set the ODMA callback
 * 
 **/
error_type_t jbig_decode_set_odma_callback( jbig_decode_handle_t *handle, JBIG_DMA_CALLBACK odma_callback)
{
    if(handle != NULL && handle->inUse)
    {
        handle->odma_desc_block.dma_callback = odma_callback;
        return OK;
    }
    return FAIL;
}

/**
 * 
 *
 *
 **/

error_type_t jbig_decode_set_config(jbig_decode_handle_t *handle, const JBIG_CONFIG *MyCfg)
{
    error_type_t retunValue = FAIL;

    if( jbigHasBeenInitialized )
    {
        if( handle && handle->inUse && ( MyCfg != NULL ))
        {
            memcpy( &handle->config, MyCfg, sizeof( JBIG_CONFIG ) );
            retunValue = OK;
        }
    }
    return retunValue;
} 

/**
 * 
 *
 *
 **/

error_type_t jbig_decode_get_config(jbig_decode_handle_t *handle, JBIG_CONFIG *MyCfg)
{

    error_type_t retunValue = FAIL;

    ASSERT(jbig_decode_block_config_ptr != NULL);                         // has the driver been initialized?

    if( jbigHasBeenInitialized && handle && handle->inUse && ( MyCfg != NULL ))
    {
    	memcpy( MyCfg, &handle->config, sizeof( JBIG_CONFIG ) );

		retunValue = OK;
    }

    return retunValue;
}


/**
 * 
 *
 *
 **/

error_type_t jbig_decode_get_last_error_code( jbig_decode_handle_t * handle, uint32_t *errorCodePtr )
{
	uint32_t errorCode = 0;
	error_type_t returnValue = FAIL;

	if( jbigHasBeenInitialized && ( handle != NULL ))
    {
        errorCode = handle->lastError;
    
        *errorCodePtr = errorCode;
        returnValue = OK;
	}
	return returnValue;

}

/**
 * 
 *
 *
 **/

void jbig_decode_set_last_error_code( uint32_t errorCode )
{
	if ( currentTransactionHandle )
	{
		currentTransactionHandle->lastError = errorCode;
	}
}


/**
 * 
 *
 *
 **/

error_type_t
jbig_decode_start_transaction( jbig_decode_handle_t * handle)
{
	error_type_t return_result = FAIL;

    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL )
    {
        if( handle->started == false )
        {
            jbigResetHardware( handle );

            // Look  up the configuration for this handle and set up the JBIG registers accordingly
            if( jbigCommitConfig( handle ) == OK )
            {
                currentTransactionHandle = handle;
                jbigSetCurrentTransaction( handle );
    
                handle->started = true;
                
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
jbig_decode_read_bytes_left_to_transfer_output_dma( jbig_decode_handle_t *handle,
                                             uint32_t *bytes_left )
{
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs;
    uint32_t loopCount = 0;

    if( handle == NULL )
    {
        return FAIL;
    }

    p_JBIG_ODMA_regs = handle->jbig_block_interface->jbig_odma_regs;

    // Disable the output DMA, wait for it to not be busy and then read the 
    // bytes left to transfer register.
    p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(p_JBIG_ODMA_regs->cfg, 0);


    // Wait for the DMA to flush any pending data out. Seems unlikely...
    while(1 == (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK) && 
               (loopCount++ <1000) );

    *bytes_left = p_JBIG_ODMA_regs->xfer_length;

    return OK;
}

/**
 * 
 *
 *
 **/

error_type_t
jbig_decode_stop_transaction( jbig_decode_handle_t * handle )
{
	error_type_t return_result = FAIL;
	
    DBG_SLOG_ENTRY

    if( jbigHasBeenInitialized && handle != NULL && handle->inUse)
    {
        //jbigResetWriteDMA( handle );
        //jbigResetReadDMA( handle );
        jbigStop( handle );

        jbigClearCurrentTransaction( handle );
        
        handle->started = false;
        jbigFreeHandleDescriptors( handle );

        currentTransactionHandle = NULL;
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
jbig_decode_add_input_buffer(
		jbig_decode_handle_t * handle,
		void* buffer,
		uint32_t length,
		void * userData)
{
	error_type_t return_result = FAIL;

    MEMLOG("%s data buffer 0x%x\n", __func__, (uint32_t)buffer);

    if( jbigHasBeenInitialized && handle != NULL )
    {
        return_result = jbigAddReadDMABuffer( handle, buffer, length, userData );        
    }
	return return_result;
}


error_type_t
jbig_decode_add_output_buffer(
		jbig_decode_handle_t * handle,
		uint8_t* buffer,
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

#ifdef HAVE_UNIT_TEST
/**
 * 
 *
 *
 **/
static int jbig_decode_unit_test( void )
{
    int i_res;

    /* run both for the plain unit test */
    i_res = jbig_decode_test();

    return i_res;
}
#endif

#ifdef HAVE_CMD

/**
 * 
 *
 *
 **/

static void jbig_decode_chained_test( void )
{
#ifdef TODO
    DBG_PRINTF_INFO("cdma chain test enter\n");

    if (0 == chain_test_channel)
    {
        tx_semaphore_create( &chain_test_semaphore,
                             "chain_test_semaphore",
                         0 );
        REL_ASSERT(OK == 
                   CdmaOpen(CDMA_FIND_CHANNEL, &chain_test_channel));

    }
    cdma_chain_cmds_t *c = cdma_chain_array_allocate_coherent( 500, 10);
    ASSERT(c);


    int i = 0;

    uint32_t src[120];
    void *dest_p = 0;
    uint32_t *dest = MEM_MALLOC_UNCACHED( &dest_p, 500 * 10 * 4, 8);
    ASSERT(dest);
    int t = 0;
    int cmds = 0;
    int cmd_len = 3;

    for (cmds = 0; cmds < 500;  cmds++)
    {
        for (i=0; i<cmd_len; i++, t++)
            src[i] = t;
        cdma_chain_add_cmd_copy(c, &dest[t-cmd_len], src, cmd_len);
    }

    int loop_cnt = 3;
    int fail = 0;
    do 
    {
        memset(dest, 500*cmd_len, 0xcd);
        cdma_chain_transfer(chain_test_channel, 
                            c, 
                            1234134, 
                            from_irq_post_semaphore_function_status, 
                            chain_test_channel);
    
        i = 0;
        while (TX_SUCCESS != tx_semaphore_get(&chain_test_semaphore, 2)) {
            i++;
        }
        DBG_PRINTF_INFO("cdma chain done %d\n", 2 * i);
    

        for (i=0; i < 500*cmd_len ; i++)
        {
            if (dest[i] != i)
            {
                DBG_PRINTF_ERR("chain dma fail %x %x\n", dest[i], i);
                fail++;
            }
        }
    } while (!fail && loop_cnt--);

    CdmaClose(chain_test_channel); 
    chain_test_channel = 0;  
    tx_semaphore_delete( &chain_test_semaphore );
    c = cdma_chain_array_free(c);
    memFree(dest);

    DBG_PRINTF_INFO("cdma chain test exit fail cnt = %d\n", fail);
#else
    DBG_PRINTF_INFO("jbig chain test NOT implemented yet\n");
#endif
}

/**
 * 
 *
 *
 **/

int jbig_decode_debug_callback(int argc, char *argv[])
{
    int pos;
    char *char_ptr;

    if( argc < 2 ) 
    {
		cmd_printf("Supported commands:\n");
        cmd_printf("'jbigapi chain' - runs jbig chain tests\n");
#ifdef HAVE_UNIT_TEST
        cmd_printf("'jbigapi test' - runs all jbig tests\n");
#endif
        return 0;
    }

    pos = 1;

    char_ptr = argv[pos];
    while(*char_ptr) {
        *char_ptr = tolower(*char_ptr);
        char_ptr++;
    }
    if((strncmp( argv[pos],"chain", 5 ) == 0 ) && ( argc == 2 ) ) 
    {
        jbig_decode_chained_test();
    }
#ifdef HAVE_UNIT_TEST
    else if((strncmp( argv[pos],"test",5 ) == 0 ) && ( argc == 2 ) ) 
    {
        int line;
        line = jbig_decode_test();
        if(line) 
        {
            cmd_printf("ERROR in mrvl_jbig.c line %d\n", line);
        }
    }
#endif // HAVE_UNIT_TEST
    else
    {
        cmd_printf("Unsupported command\n");
    }
    return 0;
}
#endif
