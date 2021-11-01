/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_odma.c
 * 
 * \brief
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
 * To do:
 *      > Determine if channel is in use, don't allow it to be closed it it is
 *      in use. Perhaps using the EOI flag would tell when the last descriptor
 *      has been processed.
 *      > Use the configuration value for number of channels as input to memory
 *      allocation to dynamically allocate structures now based on hardcoded
 *      values for the number of ODMA channels (determined by hardware
 *      implementation).
 **/

#include <stdint.h>
#include <string.h>

#include <tx_api.h>

#include "cpu_api.h"

#include "error_types.h"
#include "ATypes.h"
#include "odma_api.h"
#include "mrvl_odma_config.h"
#include "lassert.h"
#include "memAPI.h"
#include "interrupt_api.h"
#include "logger.h"
#include "odma_compatibility.h"
#include "odma_int_attach.h"

// REVISIT - these are here for now until the PIP configuration gets put in place.
// Used for Serenity. See what is required for the real ASIC.
//PIP_FPGA_REGS_t *pip_fpga_regs = (PIP_FPGA_REGS_t *)PIP_PIP_FPGA_BASE;  // TEMP FOR TEST
//#include "PIP_CORE_regmasks.h"
//#include "PIP_CORE_regstructs.h"
//#include "regAddrs.h"
//PIP_REGS_t *pip_regs = (PIP_REGS_t *)PIP_PIP_BASE;


// Defines
//#define ODMA_DBG_LVL (DBG_LOUD | DBG_OUTPUT)
#define ODMA_DBG_LVL 0
#define ODMA_END_TRANSFER_INT 2
#define ODMA_OWNERSHIP_ERROR_INT 1

#define DESC_CFG_OWNERSHIP 0x10000
#define DESC_CFG_EOI 0x80  // Descriptor configuration word end of image bit
#define DESC_CFG_SOI 0x40  // Descriptor configuration word start of image bit
#define DESC_CFG_INT_ON_FINISH 0x2
#define DESC_CFG_STOP_ON_FINISH 0x1
                                   
// Typedefs
typedef struct odma_desc_s
{
    uint32_t config;
    uint32_t src_data_address;
    uint32_t length;
    uint32_t next;
    dma_config_data_t *user_data;
} odma_desc_t; 

typedef struct odma_context_s
{
    uint32_t desc_available;
    uint32_t data_buffer_available;
    uint32_t num_descriptors;
    uint32_t desc_complete;
    odma_desc_t *p_current_descriptor;
    odma_desc_t *odma_start_descriptor;
    bool odma_in_progress;
} odma_context_t;


/**
 * Channel info
 */

typedef struct odma_channel_s
{
    void *callback;             // user callback for non-blocking transfer
    void *context;              // user data passed to callback
    bool open;                  // channel is open
    //uint16_t num_descriptors;   // number of descriptors configured 
    odma_cfg_t config;          // channel configuration
} odma_channel_t;


struct odma_handle_s
{
    uint32_t    channel;
    odma_event_callback_t callback;
};

/**
 * Local variables
 **/
static odma_platform_config_t *p_config = NULL;
/* static */ odma_regs_t *odma_reg[ODMA_NUM_CHANNELS];
static odma_desc_t *p_desc[ODMA_NUM_CHANNELS];
static odma_desc_t *p_next_desc[ODMA_NUM_CHANNELS];
static odma_desc_t *p_start_desc[ODMA_NUM_CHANNELS];
static odma_context_t odma_context[ODMA_NUM_CHANNELS];
static TX_SEMAPHORE odma_open_semaphore;
static odma_channel_t odma_channel[ODMA_NUM_CHANNELS];
static odma_handle_t odma_handles[ODMA_NUM_CHANNELS];


/**
 * Local function forward declarations
 */
static void odma_soft_reset_odma(odma_channel_num_t chan);
static void odma_start_transfer(odma_channel_num_t chan,
                                odma_desc_t *descriptor);
static void isr_pip(uint32_t input); 


void odma_init()
{
    error_type_t result;
    uint32_t i;
    uint32_t status;

    ASSERT(p_config == NULL);       // has odma_init() already been called?
    result = odma_platform_get_config(&p_config); 
    ASSERT(result == OK);
    ASSERT(p_config != NULL);

    for (i=0; i<ODMA_NUM_CHANNELS; i++)
    {
        odma_reg[i] = (odma_regs_t *)p_config->base_address[i];  
        DPRINTF(ODMA_DBG_LVL, ("i: %d, odma base address 0x%x\n", i, odma_reg[i]));

        p_desc[i] = NULL;
        p_start_desc[i] = NULL;
        p_next_desc[i] = NULL;
    }

    status = tx_semaphore_create(&odma_open_semaphore, "ODMA", 1);
    ASSERT(status == TX_SUCCESS);

    // Disable the color pipe interrupt and acknowledge any pending ODMA ints
    intDisable(INTNUM_PIP_POGO_INTERRUPT);
    
    for (i=0; i<ODMA_NUM_CHANNELS; i++)
    {
        odma_reg[i]->int_ack = 0xffffffff;
        DPRINTF(ODMA_DBG_LVL, ("i: %d reg: 0x%x\n", i, odma_reg[i]->int_ack));
    }

    intAttach(INTNUM_PIP_POGO_INTERRUPT, 16, isr_pip, 0);

    intEnable(INTNUM_PIP_POGO_INTERRUPT);
}



error_type_t odma_add_descriptor(odma_handle_t *handle, 
                                 uint8_t *data_buffer,
                                 dma_config_data_t *user_data)
{
    odma_desc_t *my_desc = NULL;
    odma_channel_num_t chan;
    void *p = 0;

    if (handle == NULL)
    {
        DPRINTF(ODMA_DBG_LVL, ("ODMA: odma_add_descriptor ERROR - no handle!!!\n"));
        ASSERT(0);
        return FAIL;
    }

    chan = handle->channel;

    
    my_desc = (odma_desc_t *)MEM_MALLOC_UNCACHED(p, sizeof(odma_desc_t), e_32_byte);
    memset(my_desc, 0, sizeof(odma_desc_t));

    DPRINTF(ODMA_DBG_LVL, ("ODMA: odma_add_descriptor chan: %d, desc: 0x%x, data: 0x%x\n", chan, my_desc, data_buffer));
    if (my_desc == NULL)
    {
        DPRINTF(ODMA_DBG_LVL, ("ODMA: allocation failed!!!\n"));
        return FAIL;
    }

    odma_context[chan].num_descriptors++;                                         
    my_desc->src_data_address = (uint32_t)data_buffer;
    my_desc->user_data = user_data;

    // If this is the first descriptor, initialize the chain.
    if (p_desc[chan] == NULL)
    {
        p_desc[chan] = my_desc;
        p_start_desc[chan] = my_desc;
        p_desc[chan]->next = NULL;
        p_next_desc[chan] = p_desc[chan];
        DPRINTF(ODMA_DBG_LVL, ("ODMA: chan %d, next descriptor is: 0x%x\n", chan, p_next_desc[chan]));
    }
    else
    {
        p_desc[chan]->next = (uint32_t)my_desc;
        my_desc->next = (uint32_t)p_start_desc[chan];
        p_desc[chan] = my_desc;
    }

    return OK;
}


error_type_t odma_query_is_dma_in_progress(odma_handle_t *handle, bool *in_progress)
{
    odma_channel_num_t chan;

    if (handle == NULL)
    {
        return FAIL;
    }
    chan = handle->channel;

    *in_progress = odma_context[chan].odma_in_progress;

    return OK;
}


static error_type_t odma_set_config(uint32_t channel, const odma_cfg_t *p_odma_config)
{
    uint32_t cfg_reg = 0;

    ASSERT((channel >= 0) && (channel < ODMA_NUM_CHANNELS));

    // Set up the configuration register
    if (p_odma_config->input_on_upper_half)
    {
        cfg_reg = ODMA_SLI_CFG_UPPER_HALF_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_SLI_CFG_UPPER_HALF_REPLACE_VAL(cfg_reg, 0);
    }

    if (p_odma_config->input_on_msb)
    {
        cfg_reg = ODMA_SLI_CFG_MSB_IN_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_SLI_CFG_MSB_IN_REPLACE_VAL(cfg_reg, 0);
    }

    if (p_odma_config->enable_dma_block)
    {
        cfg_reg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(cfg_reg, 1);
    }
    else
    {
        cfg_reg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(cfg_reg, 1);
    }

    switch (p_odma_config->input_data_width)
    {
    case BIT_WIDTH_1:
    case BIT_WIDTH_2:
    case BIT_WIDTH_4:
    case BIT_WIDTH_8:
    case BIT_WIDTH_16:
    case BIT_WIDTH_24:
    case BIT_WIDTH_32:
        cfg_reg = ODMA_SLI_CFG_IN_WIDTH_REPLACE_VAL(cfg_reg, p_odma_config->input_data_width);
        break;

    default : 
        DPRINTF(ODMA_DBG_LVL, ("ERROR: Data size of %0d is invalid!\n\r", p_odma_config->input_data_width));
        DPRINTF(ODMA_DBG_LVL, ("Defaulting to 2 bit per pixel...\n\r"));
        cfg_reg = ODMA_SLI_CFG_IN_WIDTH_REPLACE_VAL(cfg_reg, BIT_WIDTH_2);
        break;
    }

    // Now that the config register shadow is setup, write it to hardware.
    odma_reg[channel]->cfg = cfg_reg;

    // Enable interrupts - REVISIT where should this be done at?
    odma_reg[channel]->int_en = ODMA_END_TRANSFER_INT | ODMA_OWNERSHIP_ERROR_INT;
    odma_reg[channel]->int_en = 0xff;

    DPRINTF(ODMA_DBG_LVL, ("ODMA: channel %d set cfg register 0x%x to 0x%x\n", channel, &odma_reg[channel]->cfg, odma_reg[channel]->cfg));

    return OK;
}




/*
static error_type_t odma_set_line_width(odma_channel_num_t chan, uint32_t line_width_in_bytes)
{
    


    if (odma_reg[chan] != 0)
    {
        logString("ODMA: setting line width for chan %d to %d\n", chan, line_width_in_bytes);
        odma_reg[chan]->line_size = line_width_in_bytes;
        return OK;
    }

    return FAIL;
}
*/

error_type_t odma_abort_transaction( odma_handle_t *handle )
{
    odma_channel_num_t chan;

    if (handle == NULL)
    {
        return FAIL;
    }

    chan = handle->channel;

    if (odma_reg[chan] != 0)
    {
        odma_soft_reset_odma(chan);
        return OK;
    }

    return FAIL;
}


static error_type_t odma_configure_dma_transaction( odma_channel_num_t chan,
                                                    odma_desc_t *desc,
                                                    uint8_t *p_buffer_data,
                                                    uint32_t size_in_bytes,  // Size of requested transfer
                                                    bool SOI,                // Is this the SOI DMA in the session?
                                                    bool EOI,                // Is this the EOI DMA in the session?
                                                    bool stop )              // Stop on finish?
                                                   
{
    uint32_t config;

    config = DESC_CFG_INT_ON_FINISH;

    ASSERT(chan == ODMA_MIP);

    if (stop)
    {
        config |= DESC_CFG_STOP_ON_FINISH;
        logString("ODMA: *** Descriptor stop on finish ***\n");
    }
      
    if (SOI)
    {
        logString("ODMA: *** SOI configured ***\n");
        config |= DESC_CFG_SOI;
    }
    
    if (EOI)
    {
        logString("ODMA: *** EOI configured ***\n");
        config |= DESC_CFG_EOI;
        config |= DESC_CFG_STOP_ON_FINISH;
    }

    ASSERT( (config & DESC_CFG_OWNERSHIP) == 0);
    
    config |= DESC_CFG_OWNERSHIP;
    p_next_desc[chan]->config = config;
    p_next_desc[chan]->length = size_in_bytes;
    // p_next_desc[chan]->src_data_address = (uint32_t)p_buffer_data;

    logString("ODMA: desc 0x%x, data 0x%x, cfg: 0x%x, size: %d\n", 
              p_next_desc[chan], 
              p_next_desc[chan]->src_data_address, 
              p_next_desc[chan]->config,  
              size_in_bytes);

    p_next_desc[chan] = (odma_desc_t *)p_next_desc[chan]->next;
    logString("ODMA: chan %d, next descriptor is: 0x%x\n", chan, p_next_desc[chan]);

    odma_context[chan].desc_available--;
    odma_context[chan].data_buffer_available--;
  

    logString("ODMA configure dma: avail %d, next 0x%x\n", 
              odma_context[chan].desc_available, 
              p_next_desc[chan]);

    return OK;
}



// Must be called after at least one transaction is configured. This allows for
// multiple transactions to be chained together before the DMA is started.
static void odma_start_transfer(odma_channel_num_t chan,
                                odma_desc_t *descriptor)
{
    logString("ODMA: start new transfer chan: %d, desc: 0x%x, config 0x%x\n", chan, descriptor, descriptor->config);

    odma_context[chan].p_current_descriptor = descriptor;
    odma_context[chan].odma_in_progress = true;

    // Write the descriptor write register to start the DMA
    cpu_dcache_writeback_region( (void *)descriptor, sizeof(descriptor) );
    odma_reg[chan]->desc_write = (uint32_t)descriptor; 
}




static void odma_free_descriptors(odma_channel_num_t chan )
{
    odma_desc_t *desc, *next_desc;

    if(p_start_desc[chan] != NULL)
    {
        desc = (odma_desc_t *)p_start_desc[chan]->next;
    
        while (desc != p_start_desc[chan])
        {
            next_desc = (odma_desc_t *)desc->next;
            DPRINTF(ODMA_DBG_LVL, ("ODMA: freeing descriptor 0x%x\n", desc));
            MEM_FREE_AND_NULL(desc);
            desc = next_desc;
        }
    
        DPRINTF(ODMA_DBG_LVL, ("ODMA: freeing start descriptor 0x%x\n", p_start_desc[chan]));
        MEM_FREE_AND_NULL(p_start_desc[chan]);
        p_desc[chan] = NULL;
        p_next_desc[chan] = NULL;
    }
}


error_type_t odma_start_new_session(odma_handle_t *handle, 
                                    uint32_t bits_per_pixel, 
                                    uint32_t line_width_in_bytes)
{
    uint32_t channel;
    odma_cfg_t *odma_cfg;

    logString("ODMA: start new session\n");

    if (handle == NULL)
    {
        return FAIL;
    }
    channel = handle->channel;

    logString("ODMA: setting line width for chan %d to %d\n", channel, line_width_in_bytes);
    odma_reg[channel]->line_size = line_width_in_bytes;

    odma_cfg = &odma_channel[channel].config;

    // Set the last descriptor in the ODMA chain to not chain back to the start.
    // This allows the first set of descriptors to run and stall while waiting
    // for video without getting an ownership error (loss of data occurs).
    // When video finishes it will kick off the next ODMA and the chain can
    // be reconfigured as it should be able to run without interruption.

    odma_context[channel].desc_available = odma_context[channel].num_descriptors;  
    odma_context[channel].data_buffer_available = odma_context[channel].num_descriptors; 
    odma_context[channel].desc_complete = 0;

    memset(odma_cfg, 0, sizeof(odma_cfg_t));

    switch (bits_per_pixel)
    {
    case 1:      
        odma_cfg->input_data_width = BIT_WIDTH_1;
        DPRINTF(ODMA_DBG_LVL, ("ODMA: bits per pixel input width is 1\n"));
        break;

    case 2: 
        odma_cfg->input_data_width = BIT_WIDTH_2;
        DPRINTF(ODMA_DBG_LVL, ("ODMA: bits per pixel input width is 2\n"));
        break;

    case 4:
        odma_cfg->input_data_width = BIT_WIDTH_4;
        DPRINTF(ODMA_DBG_LVL, ("ODMA: bits per pixel input width is 4\n"));
        break;

    default:
        ASSERT(0);
        break;
    }

    odma_cfg->enable_dma_block = true;
    odma_cfg->input_on_msb = false; // true;
    odma_cfg->input_on_upper_half = false;
    odma_cfg->serpentine = false;

    odma_set_config(channel, odma_cfg);

    // REVISIT DAB pip_regs->CFG = 0x003F001C;         // REVISIT TEMP
    //pip_fpga_regs->CFG = 0x003F001C;    // REVISIT TEMP

    // REVISIT DAB DPRINTF(ODMA_DBG_LVL, ("ODMA TEST CFG REGS 0x%x 0x%x\n", &pip_regs->CFG, pip_regs->CFG));

    return OK;
}


void odma_callback_function(odma_handle_t *handle, 
                            odma_callback_t function)
{
    odma_channel_num_t chan;

    DPRINTF(ODMA_DBG_LVL, ("%s 0x%x\n",__func__, function));

    chan = handle->channel;

    odma_channel[chan].callback = (void *)function;
}

/*
error_type_t pipr_odma_register_notifcation_callback( odma_handle_t *handle, 
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
*/


static void isr_pip(uint32_t input)
{
    uint32_t chan, pending;

    logString("PIP: %s  ENTRY\n", __func__);

    // Filter through the active ODMA channels and handle any pending interrupts
    for (chan=0; chan<ODMA_NUM_CHANNELS; chan++)
    {
        // Only process ODMA channels that have been configured
        if (odma_reg[chan] == 0)
        {
            continue;
        }

        pending = odma_reg[chan]->int_pend;
        odma_reg[chan]->int_ack = pending;

        // logString("ODMA: ISR 0x%x\n", pending);
        if (pending & ODMA_SLI_INT_PEND_WHO_INT_PEND_MASK)
        {
            // An ownership error has occurred. Next descriptor in the transfer must
            // be marked as "first" for the DMA to restart.
            //odma_context.first_descriptor = true;
            logString("ODMA: ownership error!!!!\n");
            DPRINTF(ODMA_DBG_LVL, ("GOT TO ISR FOR PIP ***** OWN ERROR!\n"));

            // Toggle the enable bit when the ownership error occurs, per hardware
            // documentation.
            odma_soft_reset_odma(chan);

        }

        // If this descriptor is finished see if there is more work to do.
        if ( (pending & ODMA_SLI_INT_PEND_FIN_INT_PEND_MASK)  || (pending & ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK) )
        {
            uint32_t data_buffer_address;

            // If this descriptor is set to stop on finish, then the next descriptor
            // must be written to the 'descriptor write' register.
            odma_desc_t *my_descriptor = (odma_desc_t *)odma_reg[chan]->desc_read;
            cpu_dcache_invalidate_region( (void *)my_descriptor, sizeof(odma_desc_t) );

            odma_context[chan].desc_available++;
            odma_context[chan].desc_complete++;

            if (my_descriptor->config & DESC_CFG_STOP_ON_FINISH)
            {
                logString("ODMA: stopping on descriptor! 0x%x\n", my_descriptor);
                odma_context[chan].odma_start_descriptor = 0;
                odma_context[chan].odma_in_progress = false;
            }

            // Determine the data buffer attached to the descriptor that just
            // completed.
          //  data_buffer_address = odma_context[chan].p_current_descriptor->src_data_address;
            data_buffer_address = my_descriptor->src_data_address;

            if (odma_channel[chan].callback)
            {
                logString("ODMA: exec callback desc: 0x%x data buffer: 0x%x\n", my_descriptor, data_buffer_address);
             //   ((odma_callback_t)odma_channel[chan].callback)((uint8_t *)data_buffer_address);
                ((odma_callback_t)odma_channel[chan].callback)(my_descriptor->user_data);
            }

            logString("ODMA: ISR chan %d done!  current desc 0x%x, complete: %d\n", 
                      chan, 
                      odma_context[chan].p_current_descriptor, 
                      odma_context[chan].desc_complete);

            odma_context[chan].p_current_descriptor = 
                (odma_desc_t *)odma_context[chan].p_current_descriptor->next;
        }

        if (pending & ODMA_SLI_INT_PEND_EOI_INT_PEND_MASK)
        {
            logString("ODMA: *** EOI interrupt ***\n");
        }

        if (pending & ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK)
        {
            logString("ODMA: *** EOI ERROR!!!! ***\n");
            DPRINTF(ODMA_DBG_LVL, ("GOT TO ISR FOR PIP ***** EOI ERROR!\n"));
        }

        if (pending & ODMA_SLI_INT_PEND_EOI_ALI_INT_PEND_MASK)
        {
        }

        if (pending & ODMA_SLI_INT_PEND_EOL_ALI_INT_PEND_MASK)
        {
        }

        if (pending & ODMA_SLI_INT_PEND_CL_ALI_INT_PEND_MASK)
        {
        }

        if (pending & ODMA_SLI_INT_PEND_DIR_INT_PEND_MASK)
        {
        }

       
    } // for

    logString("PIP: %s  EXIT\n", __func__);
}

void odma_setup_dma(odma_handle_t *handle,
                    uint8_t *p_buffer_data,
                    uint32_t buffer_size, 
                    bool start_of_image,
                    bool end_of_image,
                    bool stop_on_completion,
                    bool start_dma_chain)
{
    odma_channel_num_t channel;

    channel = handle->channel;

    logString("ODMA: setup next dma - size: %d, soi: %d, eoi: %d, stop: %d\n", 
              buffer_size, 
              start_of_image, 
              end_of_image,
              stop_on_completion);

  //  logString("ODMA: next %d, desc avail %d, data buf avail %d\n", 
  //            odma_context[channel].next_descriptor, 
  //            odma_context[channel].desc_available, 
  //            odma_context[channel].data_buffer_available); 

    if ( (odma_context[channel].desc_available != 0) && 
         (odma_context[channel].data_buffer_available != 0) )
    {

        // If the descriptor chain start descriptor hasn't been set, then
        // set it now. It will be used later to start the descriptor chain.
        if (odma_context[channel].odma_start_descriptor == 0)
        {
            // logString("ODMA: setting start descriptor to 0x%x\n", p_next_desc[channel]);
            odma_context[channel].odma_start_descriptor = p_next_desc[channel];
        }

        odma_configure_dma_transaction( channel,
                                        p_next_desc[channel],
                                        p_buffer_data,
                                        buffer_size,
                                        start_of_image,  
                                        end_of_image,
                                        stop_on_completion);  

        logString("ODMA: start %d, desc 0x%x, in progress %d\n", start_dma_chain, odma_context[channel].odma_start_descriptor, odma_context[channel].odma_in_progress);
        if ( (start_dma_chain == true) && 
             (odma_context[channel].odma_in_progress == false) )
        {
            odma_start_transfer(channel, odma_context[channel].odma_start_descriptor);
        }
    }
}


void odma_data_buffer_available( void *handle,
                                 dma_config_data_t *data )
{
    odma_channel_num_t channel;

    channel = ((odma_handle_t *)handle)->channel;

    odma_context[channel].data_buffer_available++;
    logString("ODMA: chan %d data buffer %d\n", 
              channel,
              odma_context[channel].data_buffer_available);
}


static void odma_soft_reset_odma(odma_channel_num_t chan)
{
    uint32_t cfg_reg;

    logString(" ODMA: chan: %d reset: 0x%x\n", chan, odma_reg[chan]->cfg);
    cfg_reg = odma_reg[chan]->cfg;
    cfg_reg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(cfg_reg, 0);
    odma_reg[chan]->cfg = cfg_reg;
    logString(" ODMA reset: 0x%x\n", odma_reg[chan]->cfg);
    cfg_reg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(cfg_reg, 1);
    odma_reg[chan]->cfg = cfg_reg;
    logString(" ODMA reset: 0x%x\n", odma_reg[chan]->cfg);
}


// Driver open/close functions

error_type_t odma_get_number_of_channels(uint32_t *number_of_channels)
{
    ASSERT(p_config != NULL);                         // has the driver been initialized?

    *number_of_channels = p_config->number_of_channels;
    return OK;
}


error_type_t odma_open(int32_t channel, odma_handle_t **handle)
{
    uint32_t status;
    error_type_t result = FAIL;

    ASSERT(p_config != NULL);  // has the driver been initialized?

    // is odma_open() already in use?
    status = tx_semaphore_get(&odma_open_semaphore, TX_WAIT_FOREVER);
    ASSERT(status == TX_SUCCESS);

    // is channel valid?
    ASSERT(channel >= 0 && channel < (int32_t)p_config->number_of_channels);

    // open the channel if it's available
    if(odma_channel[channel].open == false) 
    {                                      
        DPRINTF(ODMA_DBG_LVL, ("ODMA: open channel %d\n", channel));
        odma_channel[channel].open = true;                                         // open the channel
        odma_handles[channel].channel = channel;
        *handle = &odma_handles[channel];
        result = OK;
    }

    // return odma_open() to not in use
    status = tx_semaphore_put(&odma_open_semaphore);
    ASSERT(status == TX_SUCCESS);
    return result;
}


error_type_t odma_close(odma_handle_t *handle)
{
    uint32_t channel = handle->channel;
  //  uint32_t status; //, flags;
    error_type_t result = FAIL;

    ASSERT(p_config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < p_config->number_of_channels);
    if (channel < p_config->number_of_channels) 
    {
        // is this channel is use?
        // has this channel been opened?
        ASSERT(odma_channel[channel].open == true);
        if(odma_channel[channel].open == true) 
        {
            odma_channel[channel].open = false;
            //         odma_handles[channel].channel = HANDLE_NOT_USED;
            result = OK;
        }
    } 
    else 
    {
        ASSERT(NULL == &"odma_close: close failed channel in use");
    }

    odma_free_descriptors(channel);
    return result;
}

/** 
 * Attach the image pipe interrupt to this module's isr. This is provided
 * to allow this driver to coexist with the current PIP code.
**/
void odma_int_attach(void)
{
    uint32_t i;

    // Disable the color pipe interrupt and acknowledge any pending ODMA ints
    intDisable(INTNUM_PIP_POGO_INTERRUPT);
    
    for (i=0; i<ODMA_NUM_CHANNELS; i++)
    {
        odma_reg[i]->int_ack = 0xffffffff;
    }

    intDetach(INTNUM_PIP_POGO_INTERRUPT);
    intAttach(INTNUM_PIP_POGO_INTERRUPT, 16, isr_pip, 0);

    intEnable(INTNUM_PIP_POGO_INTERRUPT);
}

