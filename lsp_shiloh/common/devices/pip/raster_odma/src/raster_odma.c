/*
 * ============================================================================
 * Copyright (c) 2010, 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file raster_odma.c
 * 
 * \brief This is the source code file for the Print Image Pipe 
 *        raster output DMA.
 *  
 * Notes:
 *      Only set Start of Image (SOI) in the very first descriptor in the image.
 *      Likewise, only set End of Image (EOI) in the very last descriptor in the
 *      image. These are tags carried through the hardware image pipe to signal
 *      start and end of image.
 * 
 *      For use with laser video, configure 2 or more descriptors to run as a
 *      chain and then stop. When this data is rendered by video, the video
 *      interrupt will signal that the shared data buffer is available and will
 *      allow a subsequent ODMA descriptor to run. At this point the video and
 *      ODMA will be in lockstep, one transaction at a time - each ODMA
 *      descriptor will stop on finish and need to be restarted.
 *  
 *      Configuration settings:
 *  
 *      The following tables illustrate the relationship between the settings
 *      "Input on Upper Half", "Input on MSB" and the bits that are selected
 *      from the input byte. This assumes that the ODMA hardware block is built
 *      with a maximum input data width of 8.
 *  
 *      Output Data Width = 2
 *  
 *      Input on MSB | Input on Upper Half | Output bits
 *          0               0                   1:0
 *          0               1                   5:4
 *          1               0                   3:2
 *          1               1                   7:6
 *  
 *  
 *      Output Data Width = 4
 *  
 *      Input on MSB | Input on Upper Half | Output bits
 *          0               0                   3:0
 *          0               1                   7:4
 *          1               0                   3:0
 *          1               1                   7:4
 * 
 **/

#include <stdint.h>
#include <string.h>
// lsptodo: #include <tx_api.h>

#include "asic.h"
#include "error_types.h"
#include "ATypes.h"
#include "raster_odma_api.h"
#include "memAPI.h"
#include "pip_message_api.h"
#include "mlist.h"
#include "lassert.h"
#include "logger.h"
#include "regAddrs.h"
#include "raster_odma_func_table.h"
#include "dma_buffer.h"
#include "map_mem.h"
#include "SYS_UI_msg_interface.h"
/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define DBG_PRFX "RASTER_ODMA: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )


#define DBG_SLOG_ENTRY DBG_MEMLOG_DEBUG("%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG_DEBUG("%s EXIT\n", __func__);

#define ODMA_HANDLE_ID 0xBBBBFEDC

#define DESC_CFG_OWNERSHIP 0x10000
#define DESC_CFG_EOI 0x80  // Descriptor config word end of image bit
#define DESC_CFG_SOI 0x40  // Descriptor config word start of image bit
#define DESC_CFG_INT_ON_FINISH  0x2
#define DESC_CFG_STOP_ON_FINISH 0x1

#define RASTER_ODMA_BIT_WIDTH_1 0
#define RASTER_ODMA_BIT_WIDTH_2 1
#define RASTER_ODMA_BIT_WIDTH_4 2
#define RASTER_ODMA_BIT_WIDTH_8 3
#define RASTER_ODMA_BIT_WIDTH_16 4
#define RASTER_ODMA_BIT_WIDTH_24 6
#define RASTER_ODMA_BIT_WIDTH_32 5


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

// This is the structure defining the Output DMA descriptor. The info field is 
// not used by hardware, but is attached to the descriptor for use in the
// callback function that is called when descriptors complete. The info field
// provides necessary information for use by the callback; e.g. keeping track of
// how many lines of the image have been processed.
typedef struct odma_desc_s
{
    uint32_t config;
    uint32_t src_data_address;
    uint32_t length;
    uint32_t next; // hw
    void *next_v;
    void *this_v;
    void *this_hw;
    
    dma_config_data_t *info;
} odma_desc_t; 


typedef struct odma_context_s
{
    odma_desc_t *start_descriptor;
    odma_desc_t *last_descriptor;
    bool odma_session_in_progress;  
} odma_context_t;


struct odma_handle_s
{
    uint32_t  channel;
    odma_event_callback_t callback;  // user callback for non-blocking transfer
    void *context;             // user data passed to callback
    bool descriptor_in_flight; // Is a descriptor chain active?
    bool reset_in_progress;    // Keep track of soft reset of the DMA block
    bool open;                 // channel is open
    uint32_t handle_id;
	//semt_t done_semaphore; 
};

typedef struct odma_list_node_s
{
    ATLISTENTRY list_node;
    odma_desc_t *desc;

} odma_list_node_t;



/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */

static ODMA_REGS_t *odma_regs[NUM_RASTER_ODMA_CHANNELS];
static odma_context_t odma_context[NUM_RASTER_ODMA_CHANNELS];
static sem_t odma_open_semaphore; // really a mutex 
static sem_t odma_done_semaphore[NUM_RASTER_ODMA_CHANNELS]; // user done semaphore.
static odma_handle_t odma_handle[NUM_RASTER_ODMA_CHANNELS];

static MLISTENTRY odma_available_descriptors[NUM_RASTER_ODMA_CHANNELS];
static MLISTENTRY odma_in_use_descriptors[NUM_RASTER_ODMA_CHANNELS];

static odma_func_ptr_t odma_func;

 
/* -------------------------------------------------------------------------- */
/*                   F O R W A R D  D E C L A R A T I O N S                   */
/* -------------------------------------------------------------------------- */
static void odma_write_start_descriptor_to_hardware( odma_desc_t *descriptor,
                                                     odma_channel_num_t chan );

static odma_desc_t* odma_get_descriptor(uint32_t chan);

static error_type_t odma_configure_dma_descriptor( odma_desc_t *desc,
                                                   char *p_buffer_data,
                                                   uint32_t size_in_bytes,  // Size of requested transfer
                                                   uint32_t num_lines,      // Number of lines in the buffer
                                                   uint32_t channel,        // DMA channel
                                                   bool SOI,                // Is this the SOI DMA in the session?
                                                   bool EOI,                // Is this the EOI DMA in the session?
                                                   bool stop );             // Stop on finish?
                                                   
static bool raster_odma_is_handle_valid( odma_handle_t *handle );
static void raster_odma_free_descriptors(uint32_t chan);


/* -------------------------------------------------------------------------- */
/*                     E X P O R T E D  F U N C T I O N S                     */
/* -------------------------------------------------------------------------- */

void raster_odma_init( void )
{
    uint32_t i;
    uint32_t status;
    error_type_t result;
    static bool raster_init = false;

    DBG_PRINTF_NOTICE("%s ENTRY\n", __func__);

    if (!raster_init)
    {

#ifdef __linux__
        int fd_mem = -1;
        void *baseaddr;

        if((fd_mem = open("/dev/mem", (O_RDWR | O_SYNC))) < 0) {
            ASSERT(fd_mem != -1);
        }
#endif


        // Assign base hardware address to register variables
        for (i=0; i<NUM_RASTER_ODMA_CHANNELS; i++)
        {
            result = raster_odma_get_base_addr(i, &odma_regs[i]);
#ifdef __linux__
            odma_regs[i] = mapMem( (uint32_t)odma_regs[i], sizeof(ODMA_REGS_t), &baseaddr, fd_mem);
#endif
            XASSERT(OK == result, result);
	    odma_regs[i]->int_en = 0; 
	    odma_regs[i]->cfg = ODMA_CFG_ENABLE_REPLACE_VAL(odma_regs[i]->cfg, 0);

            ASSERT(NULL != odma_regs[i]);
            DBG_PRINTF_NOTICE("i: %d, odma base address 0x%x\n", i, odma_regs[i]);
            status = sem_init(&odma_done_semaphore[i], 0, 0);
            XASSERT(status == 0, status);
        }
        
        // Initialize the function pointer table
        // odma_func.add_data_buffer = raster_odma_add_input_buffer;  
        // odma_func.register_event_callback =  raster_odma_register_notifcation_callback;
        // odma_func.set_output_dma_line_width_in_bytes = raster_odma_set_line_width_in_bytes;
        status = sem_init(&odma_open_semaphore, 0, 1);
        XASSERT(status == 0, status);
        raster_init = true;
    }
    DBG_PRINTF_NOTICE("%s EXIT\n", __func__);
}


error_type_t raster_odma_add_input_buffer( odma_handle_t *handle, 
					   char *data_buffer,
                                           uint32_t size_in_bytes,
                                           uint16_t num_lines,
                                           bool SOI,
                                           bool EOI,
                                           bool stop_on_finish,
                                           dma_config_data_t *user_data )
{
    odma_desc_t *my_desc = NULL;
    uint32_t chan;

    DBG_MEMLOG_DEBUG("%s ENTRY SOI: %d, EOI: %d, num lines: %d, size of transfer: %d, data 0x%x\n", __func__, SOI, EOI, num_lines, size_in_bytes, data_buffer);
   // DBG_SLOG_ENTRY

    ASSERT(NULL != user_data);

    if ( false == raster_odma_is_handle_valid( handle ) )
    {
        DBG_PRINTF_NOTICE("RASTER ODMA: %s ERROR - no handle!!!\n", __func__);
        ASSERT(0);
        return FAIL;
    }

    // This is a legal case - if the size of the request is zero, return FAIL.
    if (size_in_bytes == 0)
    {
        DBG_MEMLOG_DEBUG("RASTER ODMA: %s size in bytes is zero.\n", __func__);
        DBG_SLOG_EXIT
        return FAIL;
    }

    chan = handle->channel;

    my_desc = odma_get_descriptor(chan);

    my_desc->info = user_data;
    // DPRINTF(RASTER_ODMA_DBG_LVL,("RASTER ODMA: attaching user data 0x%x to desc 0x%x\n", user_data, my_desc));

    // If there isn't a descriptor in progress, simply append this descriptor 
    // to the end of it. Otherwise create a new chain.
    if (handle->descriptor_in_flight == false)
    {
        odma_desc_t *desc;

        if (odma_context[chan].start_descriptor == 0)
        {
            odma_context[chan].start_descriptor = my_desc;
            DBG_MEMLOG_DEBUG("%s new chain start descriptor 0x%x\n", __func__, odma_context[chan].start_descriptor);
        }
        else
        {
            desc = odma_context[chan].start_descriptor;
    
            while (desc->next_v != 0)
            {
                desc = (odma_desc_t *)desc->next_v;
            }
            desc->next_v = my_desc;
            desc->next = (uint32_t)my_desc->this_hw;
            DBG_MEMLOG_DEBUG("%s append to OLD chain start descriptor 0x%x\n", __func__, odma_context[chan].start_descriptor);
            ASSERT(my_desc->next_v == 0);
        }
    }
    else
    {
        DBG_MEMLOG_DEBUG("RASTER ODMA: %s desc in flight, can't start!\n", __func__);
    }

    // DBG_PRINTF_NOTICE("RASTER ODMA: %s desc: 0x%x, data: 0x%x, chan: %d\n", __func__, my_desc, data_buffer, chan);
    DBG_MEMLOG_DEBUG("RASTER ODMA: %s desc: 0x%x, data: 0x%x, chan: %d\n", __func__, my_desc, data_buffer, chan);
 
    odma_configure_dma_descriptor( my_desc,
                                   data_buffer,
                                   size_in_bytes,
                                   num_lines,
                                   chan,
                                   SOI,  
                                   EOI,
                                   stop_on_finish);  


    // If there isn't a start descriptor already designated, this must be the
    // first descriptor in a new chain. Set the start descriptor value.
    if (odma_context[chan].start_descriptor == 0)
    {
        odma_context[chan].start_descriptor = my_desc;
        DBG_MEMLOG_DEBUG("%s start descriptor is now 0x%x\n", __func__, odma_context[chan].start_descriptor);
        ASSERT(my_desc);
    }

    DBG_SLOG_EXIT

    return OK;
}


error_type_t raster_odma_query_is_dma_in_progress(odma_handle_t *handle, bool *in_progress)
{
    odma_channel_num_t chan;

    if (handle == NULL)
    {
        return FAIL;
    }
    chan = handle->channel;

    *in_progress = odma_context[chan].odma_session_in_progress;

//    DBG_PRINTF_NOTICE("%s in_progress %d\n", __func__, *in_progress);

    return OK;
}


error_type_t raster_odma_set_cfg(odma_handle_t *handle, raster_odma_cfg_t *cfg)
{
    uint32_t channel;
    uint32_t cfg_reg;
    bool line_reverse;

    DBG_SLOG_ENTRY

    if ( false == raster_odma_is_handle_valid( handle ) )
    {
        DBG_PRINTF_NOTICE("RASTER ODMA: %s ERROR - no handle!!!\n", __func__);
        ASSERT(0);
        return FAIL;
    }

    channel = handle->channel;
    XASSERT(channel < NUM_RASTER_ODMA_CHANNELS, channel);

    cfg_reg = odma_regs[channel]->cfg;

    DBG_MEMLOG_DEBUG("%s ENTRY chan %d\n", __func__, channel);

    // Set up the configuration register bit values.
    if (cfg->serpentine == true) 
    {
        cfg_reg = ODMA_CFG_SERPENTINE_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_SERPENTINE_REPLACE_VAL(cfg_reg, 0);
    }

    line_reverse = (cfg->line_reverse && !cfg->flip_horizontal) || 
                   (!cfg->line_reverse && cfg->flip_horizontal);
    if (line_reverse == true)
    {
        cfg_reg = ODMA_CFG_LINE_REV_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_LINE_REV_REPLACE_VAL(cfg_reg, 0);
    }

    if (cfg->transfer_reverse == true)
    {
        cfg_reg = ODMA_CFG_TRAN_REV_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_TRAN_REV_REPLACE_VAL(cfg_reg, 0);
    }

    if (cfg->input_on_upper_half)
    {
        cfg_reg = ODMA_CFG_UPPER_HALF_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_UPPER_HALF_REPLACE_VAL(cfg_reg, 0);
    }

    if (cfg->input_on_msb)
    {
        cfg_reg = ODMA_CFG_MSB_IN_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_MSB_IN_REPLACE_VAL(cfg_reg, 0);
    }

    if (cfg->enable_dma_block)
    {
        cfg_reg = ODMA_CFG_ENABLE_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_CFG_ENABLE_REPLACE_VAL(cfg_reg, 0);  // was always enabled.
    }

    switch (cfg->input_data_width)
    {
    case RASTER_ODMA_BIT_WIDTH_1:
    case RASTER_ODMA_BIT_WIDTH_2:
    case RASTER_ODMA_BIT_WIDTH_4:
    case RASTER_ODMA_BIT_WIDTH_8:
    case RASTER_ODMA_BIT_WIDTH_16:
    case RASTER_ODMA_BIT_WIDTH_24:
    case RASTER_ODMA_BIT_WIDTH_32:
        cfg_reg = ODMA_CFG_IN_WIDTH_REPLACE_VAL(cfg_reg, cfg->input_data_width);
        break;

    default : 
        DBG_PRINTF_NOTICE("ERROR: Data size of %0d is invalid!\n\r", cfg->input_data_width);
        DBG_PRINTF_NOTICE("Defaulting to 2 bit per pixel...\n\r");
        cfg_reg = ODMA_CFG_IN_WIDTH_REPLACE_VAL(cfg_reg, RASTER_ODMA_BIT_WIDTH_2);
        break;
    }

    cfg_reg = ODMA_CFG_BURST_LEN_REPLACE_VAL(cfg_reg, cfg->burst_length);
    
    // Now that the config register shadow is setup, write it to hardware.
    odma_regs[channel]->cfg = cfg_reg;

    DBG_MEMLOG_DEBUG("ODMA: channel %d set cfg register 0x%x to 0x%x\n", channel, &odma_regs[channel]->cfg, odma_regs[channel]->cfg);

    // Disable the direction error interrupt if a horizontal flip was requested.
    if (true == cfg->flip_horizontal)
    {
        odma_regs[channel]->int_en &= ~ODMA_INT_PEND_DIR_INT_PEND_MASK;
    }
    else 
    {
        odma_regs[channel]->int_en |= ODMA_INT_PEND_DIR_INT_PEND_MASK;
    }

    return OK;
}



error_type_t raster_odma_register_notifcation_callback( odma_handle_t *handle, 
                                                      odma_event_callback_t function )
{
    odma_channel_num_t chan;

    if (handle == NULL)
    {
        return FAIL;
    }

    chan = handle->channel;

    handle->callback = function;

    return OK;
}


error_type_t raster_odma_set_line_width_in_bytes( odma_handle_t *handle, uint32_t width )
{
    uint32_t chan;

    DBG_SLOG_ENTRY

    DBG_MEMLOG_DEBUG("%s line width: %d\n", __func__, width);
    // DBG_PRINTF_NOTICE("%s line width: %d\n", __func__, width);

    if (handle == NULL)
    {
        return FAIL;
    }

    chan = handle->channel;

    odma_regs[chan]->line_size = width;

    return OK;
}


error_type_t raster_odma_get_odma_fn_ptr_table( odma_handle_t *handle,
                                                odma_func_ptr_t **table )
{
    if (raster_odma_is_handle_valid( handle ) == false)
    {
        *table = 0;
        return FAIL;
    }

    *table = &odma_func;

    return OK;
}



/* -------------------------------------------------------------------------- */
/*            D R I V E R  O P E N / C L O S E  F U N C T I O N S             */
/* -------------------------------------------------------------------------- */

error_type_t raster_odma_open(odma_handle_t **handle, int32_t channel)
{
    uint32_t status;
    uint32_t int_mask;

    error_type_t result = FAIL;

    // is channel valid?
    if (!((channel >= 0) && (channel < NUM_RASTER_ODMA_CHANNELS)))
    {
        DBG_PRINTF_NOTICE("ODMA: open channel failed! %d\n", channel);
        return FAIL;
    }

    // Confirm that the driver has been initialized.

    // is odma_open() already in use?
    status = sem_wait(&odma_open_semaphore);
    XASSERT((status == 0), status);

    // open the channel if it's available
    if(odma_handle[channel].open == false) 
    {                                      
        DBG_MEMLOG_DEBUG("ODMA: open channel %d\n", channel);
        odma_handle[channel].open = true;                                         // open the channel
        odma_handle[channel].channel = channel;
        odma_handle[channel].handle_id = ODMA_HANDLE_ID;
        *handle = &odma_handle[channel];

        // Initialize the  descriptor "available"  and "in use" lists
        MInitList(&odma_available_descriptors[channel]);
        MInitList(&odma_in_use_descriptors[channel]);

	// constructor would be nice :)
        memset( (void *)&odma_context[channel], 0, sizeof(odma_context_t) );

        // Enable interrupts 
        int_mask =  ODMA_INT_PEND_WHO_INT_PEND_MASK |
                    ODMA_INT_PEND_FIN_INT_PEND_MASK | 
                    ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK |
                    ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK |
                    ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK |
                    ODMA_INT_PEND_CL_ALI_INT_PEND_MASK |
                    ODMA_INT_PEND_DIR_INT_PEND_MASK |
                    ODMA_INT_PEND_EOI_INT_PEND_MASK |
                    ODMA_INT_PEND_RST_INT_PEND_MASK;
                    // Not supported by SLI HW 
                    // ODMA_INT_PEND_BRESP_INT_PEND_MASK |
                    // ODMA_INT_PEND_RRESP_INT_PEND_MASK;

        odma_regs[channel]->int_en = int_mask;

        DBG_MEMLOG_DEBUG("ODMA: channel %d, enabled ints: 0x%x\n", channel, odma_regs[channel]->int_en);

        raster_odma_soft_reset(channel);
        raster_odma_soft_reset_complete(channel);

        result = OK;
    }

    // return odma_open() to not in use
    status = sem_post(&odma_open_semaphore);
    XASSERT(status == 0, status);
    return result;
}

void ODMA_WAIT( odma_handle_t *handle )
{
	int s;

	if (handle /* && handle->channel */) {
                DBG_MEMLOG_WARNING("RASTER ODMA: sem_wait %d\n", handle->channel);
#if 0
        s = 0;
		sem_wait(&odma_done_semaphore[handle->channel]);
#else
		while ( -EINTR == (s = posix_sem_timedwait_ms(&odma_done_semaphore[handle->channel], 5000))) {
			DBG_MEMLOG_WARNING("RASTER ODMA: sem_wait restart %d\n", handle->channel);
			continue; // restart if interrupted by handler.
		}

		if ( -ETIMEDOUT == s ) {
			DBG_PRINTF_CRIT( "ODMA wait timeout failure %x %x \n", 
					 handle, handle ? handle->channel : 0xdeadbeef );
			raster_odma_dump_regs();
			//To get more information, start with enabled pip log(logger set print pip log_debug)
			{
				system("/app/tinydebug cmd \"slog dump\"");
extern void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);
                SendSCStatusToUI(SC_SYSTEM_ODMA_WAIT_FAIL, SYSTEM_MODULE_SCID);
				while(1) {
					sleep(2);
				}
			}
		} else {
			DBG_PRINTF_NOTICE( "ODMA completed semaphore\n" );
		}
#endif
		DBG_MEMLOG_WARNING("RASTER ODMA: sem_wait return %d\n", s);
	} else {
		DBG_PRINTF_ERR( "ODMA wait failure %x %x \n", handle, handle ? handle->channel : 0xdeadbeef );
	}
}

error_type_t raster_odma_close(odma_handle_t *handle)
{
    uint32_t channel; 
    error_type_t result = FAIL;

    if (handle == NULL)
    {
        DBG_PRINTF_NOTICE("ODMA: attempt to close a NULL HANDLE\n");
        return FAIL;
    }

    channel = handle->channel;

    // is channel valid?
    XASSERT(channel < NUM_RASTER_ODMA_CHANNELS, channel);
    if (channel < NUM_RASTER_ODMA_CHANNELS) 
    {
        // Only close it if it is open, otherwise return fail.
        if(odma_handle[channel].open == true) 
        {
            odma_handle[channel].open = false;
            odma_handle[channel].handle_id = 0;
            
            // Disable interrupts 
            odma_regs[channel]->int_en = 0;
            result = OK;
        }
    } 
    else 
    {
        ASSERT(NULL == &"odma_close: close failed channel in use");
    }

    raster_odma_free_descriptors(channel);
    MList_DeleteLock(&odma_available_descriptors[channel]);
    MList_DeleteLock(&odma_in_use_descriptors[channel]);
    return result;
}


/**
 * \brief Initiate a soft reset of the output DMA hardware and set the flag in 
 *        the handle indicating a soft reset is in progress. The soft reset is
 *        necessary if the descriptor chain terminates with an ownership error.
 *        No manipulation of block registers should occur until the soft reset
 *        is completed.
 * 
 * @param chan 
 */
void raster_odma_soft_reset(odma_channel_num_t chan)
{
    uint32_t cfg_reg;

    DBG_SLOG_ENTRY

    DBG_MEMLOG_DEBUG("%s %d\n", __func__, chan);

    odma_handle[chan].reset_in_progress = true;

  //  DBG_MEMLOG_DEBUG(" ODMA: chan: %d reset: 0x%x\n", chan, odma_regs[chan]->cfg);
    cfg_reg = odma_regs[chan]->cfg;
    cfg_reg = ODMA_CFG_ENABLE_REPLACE_VAL(cfg_reg, 0);
    odma_regs[chan]->cfg = cfg_reg;

  //  DBG_MEMLOG_DEBUG(" ODMA reset: 0x%x\n", odma_regs[chan]->cfg);
    cfg_reg = ODMA_CFG_ENABLE_REPLACE_VAL(cfg_reg, 1);
    odma_regs[chan]->cfg = cfg_reg;
 //   DBG_MEMLOG_DEBUG(" ODMA reset: 0x%x\n", odma_regs[chan]->cfg);
}


void raster_odma_soft_reset_complete(odma_channel_num_t chan)
{
    odma_handle[chan].reset_in_progress = false;
    DBG_MEMLOG_DEBUG("%s %d\n", __func__, chan);
}


error_type_t raster_odma_start_xfer(odma_handle_t *handle)
{
    uint32_t chan;

    DBG_SLOG_ENTRY

    if (handle == NULL)
    {
        return FAIL;
    }

    chan = handle->channel;

    if (handle->descriptor_in_flight)
    {
        DBG_MEMLOG_DEBUG("%s early EXIT in flight\n", __func__);
        return OK;
    }

    if (odma_context[chan].start_descriptor != 0)
    {
        DBG_MEMLOG_DEBUG("%s chan %d restarting raster ODMA desc 0x%x in flight %d\n", __func__, chan, odma_context[chan].start_descriptor, handle->descriptor_in_flight);
        handle->descriptor_in_flight = true;
        odma_write_start_descriptor_to_hardware(odma_context[chan].start_descriptor, chan);
    }
    
    return OK;
}


/* -------------------------------------------------------------------------- */
/* The following functions are used to set the configuration values for the   */
/* UDMA and IDMA core configuration registers.                                */
/* -------------------------------------------------------------------------- */

error_type_t raster_odma_input_width_in_bpp(odma_handle_t *handle, uint8_t input_data_width)
{
    uint32_t chan;
    uint32_t bpp = RASTER_ODMA_BIT_WIDTH_2;
    uint32_t reg_val;

    if (handle == NULL)
    {
        return FAIL;
    }

    DBG_SLOG_ENTRY

    chan = handle->channel;

    switch (input_data_width)
    {
    case 1:     
        bpp = RASTER_ODMA_BIT_WIDTH_1;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 1\n");
        break;

    case 2: 
        bpp = RASTER_ODMA_BIT_WIDTH_2;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 2\n");
        break;

    case 4:
        bpp = RASTER_ODMA_BIT_WIDTH_4;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 4\n");
        break;

    case 8:
        bpp = RASTER_ODMA_BIT_WIDTH_8;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 8\n");
        break;

    case 16:
        bpp = RASTER_ODMA_BIT_WIDTH_16;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 16\n");
        break;

    case 24:
        bpp = RASTER_ODMA_BIT_WIDTH_24;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 24\n");
        break;

    case 32:
        bpp = RASTER_ODMA_BIT_WIDTH_32;
        DBG_MEMLOG_DEBUG("RASTER ODMA: input data width is 32\n");
        break;

    default:
        ASSERT(0);
        break;
    }

    reg_val = odma_regs[chan]->cfg;
    reg_val = ODMA_CFG_IN_WIDTH_REPLACE_VAL(reg_val, bpp);
    odma_regs[chan]->cfg = reg_val;

    return OK;
}


void raster_odma_dump_regs( void )
{
    uint32_t chan;

    for (chan = 0; chan < NUM_RASTER_ODMA_CHANNELS; chan++ )
    {
        DBG_PRINTF_ERR(">>>>>> ODMA registers channel: %d <<<<<<\n\n", chan);
        DBG_PRINTF_ERR("ODMA CFG         0x%x\n", odma_regs[chan]->cfg);

        switch( ODMA_CFG_IN_WIDTH_MASK_SHIFT(odma_regs[chan]->cfg) )
        {
        case RASTER_ODMA_BIT_WIDTH_1:
            DBG_PRINTF_ERR("   BPP 1\n"); 
            break;

        case RASTER_ODMA_BIT_WIDTH_2:
            DBG_PRINTF_ERR("   BPP 2\n"); 
            break;

        case RASTER_ODMA_BIT_WIDTH_4:
            DBG_PRINTF_ERR("   BPP 4\n"); 
            break;

        case RASTER_ODMA_BIT_WIDTH_8:
            DBG_PRINTF_ERR("   BPP 8\n"); 
            break;

        default: 
            DBG_PRINTF_ERR("   *** BPP OUT OF RANGE ***\n"); 
            break;
        }

        DBG_PRINTF_ERR("ODMA DESC_READ   0x%x\n", odma_regs[chan]->desc_read);
        DBG_PRINTF_ERR("ODMA DESC_WRITE  0x%x\n", odma_regs[chan]->desc_write);
        DBG_PRINTF_ERR("ODMA INT_ACK     0x%x\n", odma_regs[chan]->int_ack);
        DBG_PRINTF_ERR("ODMA INT_ENABLE  0x%x\n", odma_regs[chan]->int_en);
        DBG_PRINTF_ERR("ODMA INT_FORCE   0x%x\n", odma_regs[chan]->int_fo);
        DBG_PRINTF_ERR("ODMA INT_PEND    0x%x\n", odma_regs[chan]->int_pend);
        DBG_PRINTF_ERR("ODMA LINE_SIZE   0x%x\n", odma_regs[chan]->line_size);
        DBG_PRINTF_ERR("ODMA RESET       0x%x\n", odma_regs[chan]->reset);
        DBG_PRINTF_ERR("ODMA STATUS      0x%x\n", odma_regs[chan]->status);
        DBG_PRINTF_ERR("ODMA XFER_ADDR   0x%x\n", odma_regs[chan]->xfer_addr);
        DBG_PRINTF_ERR("ODMA XFER_BURST  0x%x\n", odma_regs[chan]->xfer_burst);
        DBG_PRINTF_ERR("ODMA XFER_LENGTH 0x%x\n", odma_regs[chan]->xfer_length);
        DBG_PRINTF_ERR("\n");
    }
}


/**
 * \brief This function is called when a descriptor completes. The descriptor 
 *        complete event is detected in interrupt context. This function should
 *        be called from the thread of the print image pipe containing this
 *        output DMA. This function searches the list for all descriptors in the
 *        "in use" list up to and including the descriptor in the function
 *        parameter list and moves them to the "available" list. Note that the
 *        hardware can not notify of every complete descriptor because of
 *        latency.
 * 
 * @param desc 
 * @param chan 
 */
void raster_odma_release_descriptors_to_available_list(uint32_t desc, uint32_t chan)
{
    odma_desc_t *last_desc;
    odma_list_node_t *desc_node;
    ATLISTENTRY *node;
    bool done = false;

    DBG_SLOG_ENTRY;

    last_desc = (odma_desc_t *)desc;

    // The RASTER ODMA hardware is not UDMA based. It does not follow the next
    // desriptor link if it is zero and subsequently post an ownership error.
    // If the next descriptor link is zero, the DMA chain is completed and the
    // DMA is no longer in progress.
    if ( last_desc == 0 )
    {
        odma_context[chan].odma_session_in_progress = false;
        odma_handle[chan].descriptor_in_flight = false;
        DBG_MEMLOG_DEBUG("RASTER ODMA: %s in flight = false, in progress = false\n", __func__);
    }

    if (last_desc == 0)
    {
        return;
    }



    // Move descriptor from "in use" list to "available" list. Nodes are always 
    // added to the tail of the "in use" list, so start at the head node of the
    // "in use" list and remove nodes until the node containing the specified
    // descriptor is found. Remove it and exit!

    while (!done)
    {
        // It is possible that the in use list is empty, if so then return.
        node = MListHead(&odma_in_use_descriptors[chan]);
        if (node == 0)
        {
            break;
        }
        desc_node = CONTAINING_RECORD(node, odma_list_node_t, list_node);

        if (desc_node->desc->this_hw == last_desc)
        {
            done = true;
        }

	DBG_MEMLOG_DEBUG("RASTER ODMA: release descriptor last: 0x%x last next 0x%x\n", last_desc, desc_node->desc->next);
        // If the descriptor is owned by the CPU and therefore not "in flight",
        // move it from the "in use" to the "available" list.
        if ( (desc_node->desc->config & DESC_CFG_OWNERSHIP) == 0)
        {
            // The descriptor is complete. Execute the callback function 
            // associated with this event.
            if (odma_handle[chan].callback)
            {
                DBG_MEMLOG_DEBUG("RASTER ODMA: callback desc 0x%x 0x%x with info 0x%x\n", desc_node->desc, desc_node->desc->config, desc_node->desc->info);
                odma_handle[chan].callback((void *)desc_node->desc->info);
            }
            // Now move it from the "in use" to the "available" list.
            DBG_MEMLOG_DEBUG("RASTER ODMA: move node 0x%x 0x%x from in use to available\n", node, desc_node->desc);
            DBG_MEMLOG_DEBUG("RASTER ODMA: data buffer 0x%x COMPLETE. SOI: %d, EOI %d\n", desc_node->desc->src_data_address, desc_node->desc->info->SOI, desc_node->desc->info->EOI); 

            desc_node->desc->src_data_address = 0;

            MRemoveEntryList(&odma_in_use_descriptors[chan], node);
            MInsertTailList(&odma_available_descriptors[chan], node);

            // If the "last descriptor" was released, then null it out.
            if (odma_context[chan].last_descriptor == desc_node->desc)
            {
                // DBG_PRINTF_NOTICE("RASTER ODMA: NULLING last descriptor!\n");
                DBG_MEMLOG_DEBUG("RASTER ODMA: NULLING last descriptor!\n");
                odma_context[chan].last_descriptor = 0;
            }

            if (desc_node->desc->config & DESC_CFG_EOI)
            {
                ASSERT(done==true);
                odma_handle[chan].descriptor_in_flight = false;
            }

            // Set the start descriptor to the next descriptor in the chain.
            // This may be a null value.
            odma_context[chan].start_descriptor =  (odma_desc_t *)desc_node->desc->next_v;
            DBG_MEMLOG_DEBUG("RASTER ODMA: %s start descriptor is 0x%x\n", __func__, odma_context[chan].start_descriptor);
            if (odma_context[chan].start_descriptor == 0)
            {
                // If the next descriptor in the chain is null, then this 
                // descriptor is the last in the chain.
                DBG_MEMLOG_DEBUG("RASTER ODMA: %s in flight = false\n", __func__);
                odma_handle[chan].descriptor_in_flight = false;
            }

            DBG_MEMLOG_DEBUG("RASTER ODMA: start descriptor is now 0x%x\n", odma_context[chan].start_descriptor);
            if (!odma_handle[chan].callback) 
                sem_post(&odma_done_semaphore[chan]);  // complete list manipulation, then post.
        }
    }
    
    DBG_SLOG_EXIT
    return;
}


void raster_odma_handle_ownership_error(uint32_t chan)
{
     DBG_MEMLOG_DEBUG("RASTER ODMA: %s in flight = false\n", __func__);

    // When an ownership error occurs, the descriptor chain must be restarted, 
    // set the descriptor in flight flag to indicate the chain has stalled.
    odma_handle[chan].descriptor_in_flight = false;
}


/**
 * \brief Handle raster output DMA interrupt events. This function runs in 
 *        interrupt context and notifies the print image pipe that contains it
 *        about the event(s) that have occurred.
 * 
 */
void raster_odma_handle_interrupt_event(void)
{
    uint32_t chan;
    uint32_t pending;
    pip_message_t msg;
    // odma_desc_t *descriptor;
    uint32_t descriptor;

    DBG_MEMLOG_DEBUG("RASTER ODMA: %s  ENTRY\n", __func__);

    // Filter through the active ODMA channels and handle any pending interrupts
    for (chan=0; chan<NUM_RASTER_ODMA_CHANNELS; chan++)
    {
        // Only process ODMA channels that have been configured
        if (odma_regs[chan] == 0)
        {
            continue;
        }

        pending = odma_regs[chan]->int_pend & odma_regs[chan]->int_en;
        descriptor = odma_regs[chan]->desc_read;
        DBG_MEMLOG_DEBUG("ODMA: chan %d, ISR 0x%x, descriptor 0x%x\n", chan, pending, descriptor);

        msg.chan = chan;
        msg.value = (uint32_t)&odma_handle[chan];
        msg.msg = 0xFFFFFFFF; // Set to invalid here, should be set below.
        msg.descriptor = (uint32_t)descriptor;

        // Check for ownership error. This error is posted when the next 
        // descriptor in the chain does not have the ownership bit set to
        // indicate DMA hardware owns the descriptor. In this case the DMA
        // descriptor chain will have to be restarted.
        if (pending & ODMA_INT_PEND_WHO_INT_PEND_MASK)
        {
            DBG_MEMLOG_DEBUG("ODMA: ownership error! Descriptor 0x%x\n", descriptor);

            msg.msg = RASTER_ODMA_OWN_ERROR;
            msg.descriptor = (uint32_t)descriptor;
            pip_send_message(&msg);
        }

        // Descriptor completed. 
        if (pending & ODMA_INT_PEND_FIN_INT_PEND_MASK)  
        {
            // Read the value for the descriptor that just finished. Invalidate
            // data cache because the CPU isn't aware of the changes to the
            // descriptor made by the DMA hardware.
            // descriptor = odma_regs[chan]->desc_read;
            if (descriptor == 0)
            {
                DBG_PRINTF_ERR("ERROR: ODMA NULL DESCRIPTOR chan=%d, 0x%X 0x%X 0x%X 0x%X 0x%X\n",chan, odma_regs[0]->desc_read,
                       odma_regs[1]->desc_read,odma_regs[2]->desc_read,odma_regs[3]->desc_read,
                       odma_regs[4]->desc_read);
                return;
            }
//            ASSERT(descriptor != 0);

            if (0) // lsptodo: hwaddr deref: //descriptor->config & DESC_CFG_STOP_ON_FINISH)
            {
                DBG_MEMLOG_DEBUG("ODMA: stopping on descriptor! 0x%x\n", descriptor);
                // odma_context[chan].odma_session_in_progress = false;
            }

            DBG_MEMLOG_DEBUG("Sending message ODMA desc complete chan %d\n", chan);
            msg.msg = RASTER_ODMA_DESC_COMPLETE;
            msg.descriptor = (uint32_t)descriptor;
            pip_send_message(&msg);
        }
        // End Of Image (EOI) occurred without End Of Data.
        if (pending & ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK)
        {
            DBG_MEMLOG_DEBUG_M("ODMA: *** EOI ERROR!!!! ***\n");
            //msg.msg = RASTER_ODMA_EOI_ERROR;
            //pip_send_message(&msg);
        }

        // EOI alignment error
        if (pending & ODMA_INT_PEND_EOI_ALI_INT_PEND_MASK)
        {
#if defined(DEBUG)
		uint32_t data_valid = odma_regs[chan]->reserved0;
		DBG_MEMLOG_DEBUG_M("ODMA: *** End of IMAGE alignment error *** %d  \n", data_valid );
#endif
		odma_regs[chan]->reset = 1;
            //msg.msg = RASTER_ODMA_EOI_ALIGNMENT_ERROR;
            //pip_send_message(&msg);
        }

        // End of Line alignment error
        if (pending & ODMA_INT_PEND_EOL_ALI_INT_PEND_MASK)
        {
		DBG_MEMLOG_DEBUG_M("ODMA: eol error %d len %d addr %x burst%d\n", 
				   odma_regs[chan]->reserved0, 
				   odma_regs[chan]->xfer_length,
				   odma_regs[chan]->xfer_addr,
				   odma_regs[chan]->xfer_burst
			);

		//msg.msg = RASTER_ODMA_DESC_COMPLETE;
		//msg.descriptor = (uint32_t)descriptor;
		//pip_send_message(&msg);
		//odma_regs[chan]->reset = 1;
            //msg.msg = RASTER_ODMA_EOL_ALIGNMENT_ERROR;

        }

        // Change Line alignment error
        if (pending & ODMA_INT_PEND_CL_ALI_INT_PEND_MASK)
        {
            msg.msg = RASTER_ODMA_CL_ALIGNMENT_ERROR;
            pip_send_message(&msg);
        }

        // DMA Direction error
        if (pending & ODMA_INT_PEND_DIR_INT_PEND_MASK)
        {
             msg.msg = RASTER_ODMA_DIRECTION_ERROR;
             pip_send_message(&msg);
        }

        // EOI 
        if (pending & ODMA_INT_PEND_EOI_INT_PEND_MASK)
        {
            msg.msg = RASTER_ODMA_END_OF_IMAGE;
            DBG_MEMLOG_DEBUG("ODMA: *** EOI interrupt ***\n");
            pip_send_message(&msg);
        }

        if (pending & ODMA_INT_PEND_RST_INT_PEND_MASK)
        {
            msg.msg = RASTER_ODMA_SOFT_RESET_COMPLETED;
            pip_send_message(&msg);
        }

        /* NOT SUPPORTED BY SLI HARDWARE
        if (pending & ODMA_INT_PEND_BRESP_INT_PEND_MASK)
        {
            msg.msg = RASTER_ODMA_ERROR;
            pip_send_message(&msg);
        }

        if (pending & ODMA_INT_PEND_RRESP_INT_PEND_MASK)
        {
            msg.msg = RASTER_ODMA_ERROR;
            pip_send_message(&msg);
        } 
        */ 


        odma_regs[chan]->int_ack = pending;
    } // for

    DBG_MEMLOG_DEBUG("RASTER ODMA: %s  EXIT\n", __func__);
}


/* -------------------------------------------------------------------------- */
/*                      P R I V A T E  F U N C T I O N S                      */
/* -------------------------------------------------------------------------- */

// Must be called after at least one transaction is configured. This allows for
// multiple transactions to be chained together before the DMA is started.
static void odma_write_start_descriptor_to_hardware(odma_desc_t *descriptor,
                                                    odma_channel_num_t chan)
{
    DBG_MEMLOG_DEBUG("start transfer chan: %d, desc: 0x%x, config 0x%x length 0x%x, next 0x%x, this_hw 0x%x \n", 
                       chan, descriptor, descriptor->config, descriptor->length, 
                       descriptor->next, descriptor->this_hw);
    odma_context[chan].odma_session_in_progress = true;

    // Write the descriptor write register to start the DMA
    odma_regs[chan]->desc_write = (uint32_t)descriptor->this_hw; 
}


/**
 * \brief Provide a DMA descriptor from uncached memory. If the available 
 *        descriptor list is empty, then allocate a new one.
 * 
 * @param chan 
 * 
 * @return odma_desc_t* 
 */
static odma_desc_t* odma_get_descriptor(uint32_t chan)
{
    odma_list_node_t *desc_node;
    odma_desc_t *desc;
    ATLISTENTRY *node;
    void *p = 0;

    // If the descriptor list is empty, then allocate a new node and descriptor
    // and add to the list. Otherwise grab a descriptor off the list and use it.
    if (MIsListEmpty(&odma_available_descriptors[chan]))
    {
        desc_node = MEM_MALLOC(sizeof(odma_list_node_t));
        desc = (odma_desc_t *)MEM_MALLOC_UNCACHED(&p, sizeof(odma_desc_t), e_32_byte);
	memset(desc, 0, sizeof(odma_desc_t));
        desc_node->desc = desc;
        desc->this_hw = p;
	desc->this_v = desc;
        MInsertTailList(&odma_in_use_descriptors[chan], &desc_node->list_node);

        // DBG_PRINTF_NOTICE("RASTER ODMA create node: 0x%x, descriptor: 0x%x\n", desc_node, desc);
        DBG_MEMLOG_DEBUG("RASTER ODMA create node: 0x%x, descriptor: 0x%x\n", desc_node, desc);
    }
    else
    {
        node = MRemoveHeadList(&odma_available_descriptors[chan]);
        desc_node = CONTAINING_RECORD(node, odma_list_node_t, list_node);
        desc = desc_node->desc;
        MInsertTailList(&odma_in_use_descriptors[chan], &desc_node->list_node);

        // DBG_PRINTF_NOTICE("RASTER ODMA move node: 0x%x, descriptor: 0x%x to descriptor in use list\n", desc_node, desc);
        DBG_MEMLOG_DEBUG("RASTER ODMA move node: 0x%x, descriptor: 0x%x to descriptor in use list\n", desc_node, desc);
	memset(desc, 0, 20);  //warning ! clear up to this_v
        ASSERT(desc->this_v == desc);
    }


    return desc;
}


static error_type_t odma_configure_dma_descriptor( odma_desc_t *desc,
                                                   char *p_buffer_data,
                                                   uint32_t size_in_bytes,  // Size of requested transfer
                                                   uint32_t num_lines,      // Number of lines in the buffer
                                                   uint32_t channel,        // DMA channel
                                                   bool SOI,                // Is this the SOI DMA in the session?
                                                   bool EOI,                // Is this the EOI DMA in the session?
                                                   bool stop )              // Stop on finish?
{
    uint32_t config;

    desc->info->SOI = SOI;
    desc->info->EOI = EOI;
    //lsptodo: bigbuffer ?
    //desc->info->data_buffer = p_buffer_data;

    //desc->info->size_in_bytes = size_in_bytes;
    //desc->info->data = p_buffer_data;  //hw ??? lsptodo: do I need the virtual here for a callback free?
    desc->info->num_lines = num_lines;
    desc->info->channel = channel;

    // Always interrupt on finish
    config = DESC_CFG_INT_ON_FINISH;

    if (stop)
    {
        config |= DESC_CFG_STOP_ON_FINISH;
        DBG_MEMLOG_DEBUG("ODMA: *** Descriptor stop on finish ***\n");
    }
      
    if (SOI)
    {
        DBG_MEMLOG_DEBUG("ODMA: *** SOI configured ***\n");
        config |= DESC_CFG_SOI;
    }
    
    if (EOI)
    {
        DBG_MEMLOG_DEBUG("ODMA: *** EOI configured ***\n");
        config |= DESC_CFG_EOI;
        config |= DESC_CFG_STOP_ON_FINISH;
    }

    ASSERT( (config & DESC_CFG_OWNERSHIP) == 0);
    
    config |= DESC_CFG_OWNERSHIP;
   
    desc->config = config;
    desc->length = size_in_bytes;
    desc->src_data_address = (uint32_t)p_buffer_data;

    DBG_MEMLOG_DEBUG("ODMA: desc 0x%x, data 0x%x, cfg: 0x%x, size: %d\n", 
              desc, desc->src_data_address, desc->config, size_in_bytes);

    return OK;
}


static void raster_odma_free_descriptors(uint32_t chan)
{
    odma_list_node_t *desc_node;
    ATLISTENTRY *node;

    DBG_MEMLOG_DEBUG("%s ENTRY\n", __func__);
    DBG_MEMLOG_DEBUG("RASTER ODMA: num available nodes: %d, num in use nodes %d\n", 
                           MNumListNodes(&odma_available_descriptors[chan]), 
                           MNumListNodes(&odma_in_use_descriptors[chan]) );

    while ( NULL != (node = MRemoveHeadList(&odma_available_descriptors[chan])))
    {
        desc_node = CONTAINING_RECORD(node, odma_list_node_t, list_node);
        DBG_MEMLOG_DEBUG("RASTER ODMA: freeing descriptor: 0x%x, list node 0x%x\n", desc_node->desc, desc_node);
        // MEM_FREE_AND_NULL(desc_node->desc);
	memFreePhysical( desc_node->desc->this_hw, desc_node->desc );
        MEM_FREE_AND_NULL(desc_node);
    }

    node = MRemoveHeadList(&odma_in_use_descriptors[chan]);
    if (node != NULL)
    {
        DBG_PRINTF_NOTICE("%s in use list is NOT EMPTY! 0x%x\n", __func__, node);
        // ASSERT(NULL == node);
    }

    DBG_MEMLOG_DEBUG("%s EXIT\n", __func__);
}


static bool raster_odma_is_handle_valid( odma_handle_t *handle )
{
    if (handle != 0)
    {
        if (handle->handle_id == ODMA_HANDLE_ID)
        {
            return true;
        }
    }

    return false;
}

