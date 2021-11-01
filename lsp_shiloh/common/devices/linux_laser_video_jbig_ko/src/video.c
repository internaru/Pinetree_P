/*
 * ============================================================================
 * Copyright (c) 2010-2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_video.c
 * 
 * \brief
 * 
 * Notes:
 *  1. Galvo functionality is not currently supported. This includes features
 *     like "right side bender default".
 * 
 * To do:
 * 
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "dros.h"
#include "list.h"
#include "lassert.h"
#include "video_api.h"
#include "asic.h"
#include "video_config.h"
#include "error_types.h"
#include "interrupt_api.h"
#include "hips_pll_api.h"
#include "memAPI.h"


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */

#define DBG_PRFX "VIDDRV: "
#ifdef __KERNEL__
#define ERR(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__)
#define MSG(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_ERR(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__)
#define SLOG_MSG(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_WRN(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define SLOG_INFO(...) //printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define dbg_printf(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__)
#else
#define LOGGER_SUBMOD LOGGER_SUBMODULE_BIT( 1 )
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_VIDEO | LOGGER_SUBMOD
#if 1
#define ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define MSG(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#else
#define ERR(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#define MSG(...) DPRINTF(DBG_LOUD|DBG_OUTPUT, (DBG_PRFX __VA_ARGS__))
#endif
//#define MSG(...) DBG_MEMLOG(LOG_INFO, DBG_PRFX __VA_ARGS__)

#define SLOG_ERR DBG_MEMLOG_ERR
#define SLOG_MSG DBG_MEMLOG_INFO
#define SLOG_WRN DBG_MEMLOG_WARNING
#define SLOG_INFO DBG_MEMLOG_INFO
#endif // __KERNEL__

#define DBG_SLOG_ENTRY /*logString("VIDEO: %s ENTRY\n", __func__);*/
#define DBG_SLOG_EXIT /*logString("VIDEO: %s EXIT\n\n", __func__);*/


// Video descriptor configuration word masks
#define STOP_ON_FINISH_BIT  (1<<0)
#define INT_ON_FINISH_BIT   (1<<1)
#define BLANK_BUFFER_BIT    (1<<4)
#define RIGHT_TO_LEFT_BIT   (1<<5)
#define START_OF_IMAGE_BIT  (1<<6)
#define END_OF_IMAGE_BIT    (1<<7)
#define OWNERSHIP_BIT       (1<<16)

#define VIDEO_HANDLE_ID 0xAAAAFEDC


// Value used to calculate the data buffer start address for right to left DMA.
// It is documented in the MVDO DRS document in figure 3
#define VDO_WORD_SIZE_IN_BYTES 4 
                                
#define TEST_BUS_CLOCK_FREQ_MHZ 198
#define TEST_LINE_PERIOD_US 400     // equates to ~45ppm for dual beam mono engine
#define TEST_HSYNC_PERIOD (TEST_LINE_PERIOD_US * TEST_BUS_CLOCK_FREQ_MHZ)

#define VIDEO_ISR_EPOLL_FLAG_EVENT_ATTACHED	    (1<<0)

#define NUM_VIDEO_DMA_DESCS_PER_CHAN 16

/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

/**
 * \brief This structure defines the layout of the video DMA descriptor. DMA
 *  descriptors reside in memory, and must NOT be cached. Descriptors are
 *  "chained", and the DMA hardware "follows the chain", as long as the next
 *  descriptor in the chain is marked as ready (owned by CPU).
 */
typedef struct video_desc_s
{
    volatile uint32_t dma_config;
    volatile uint32_t dma_src_data_address;
    volatile uint32_t dma_line_count;
    volatile uint32_t dma_next_address;
    void* hw_address;
    void* user_data;
} video_desc_t; 

/**
 * \brief Define the pulse code array type.
 */
typedef  const uint32_t pcode_array_t[NUM_PCODE_REGS];

struct video_handle_s
{
    uint32_t handle_id;
    uint32_t channel;
    bool open;
    bool descriptor_in_flight;
    bool page_in_progress;    // Set when descriptor with SOI true is started, cleared at EOI
    viddrv_dma_callback_t dma_callback_function; // Optional callback function to exercise when video DMA completes.
    void *dma_callback_handle;            // Optional driver handle to use with callback function.
    viddrv_event_callback_t event_callback; // Optional callback function to notify of video events
    void* event_context;                  // Optional context to use with event callback
    bool top_to_bottom;                   ///< false only on autoduplex backside rotated pages.

    video_desc_t* video_dma_descs[NUM_VIDEO_DMA_DESCS_PER_CHAN];
    uint32_t next_descriptor;
    uint32_t last_descriptor;
    uint32_t num_descriptors_in_use;
};


/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static video_config_t *video_config = NULL;
static laser_engine_video_config_t *laser_engine_video_config = NULL;
static SVDO_REGS_t   *video_reg[NUM_VIDEO_CHAN];
static HIPWM_REGS_t  *hipwm_reg[NUM_VIDEO_CHAN];  
static uint32_t video_int_num[NUM_VIDEO_CHAN];
static HVSYNC_REGS_t *hvsync_reg;  

static dros_sem_t video_open_semaphore;
static video_handle_t video_handle[NUM_VIDEO_CHAN];
static dros_smp_lock_t viddrv_smplock;

/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */
static void viddrv_init_hardware( void );
static void viddrv_acknowledge_pending_ints(uint32_t chan, uint32_t pending_ints);
//static error_type_t viddrv_program_pcode_registers(uint32_t chan, 
//                                                 uint32_t bits_per_pixel);
static void viddrv_apply_configuration( video_handle_t *handle, bool top_to_bottom );
void viddrv_set_hsync_mirrors(laser_engine_video_config_t *config);

static error_type_t viddrv_configure_descriptor( video_desc_t *desc,
                                                char *p_buffer_data,
                                                uint32_t size_in_bytes,  // Size of requested transfer
                                                uint32_t line_width_in_bytes,
                                                uint32_t num_lines_per_beam, // Number of lines in the transfer
                                                uint32_t beam,  // 0 to number of beams per plane -1
                                                bool SOI,                // Is this the SOI DMA in the session?
                                                bool EOI,                // Is this the EOI DMA in the session?
                                                bool blank,              // Set if the buffer is blank or white space
                                                bool right_to_left,      // True if the data is backward order, or right to left
                                                bool top_to_bottom,      // true if page is not reversed 
                                                bool stop );              // Stop on finish?


static void viddrv_write_start_descriptor_to_hardware( video_desc_t *descriptor,
                                                      uint32_t chan );
static video_desc_t *viddrv_get_descriptor( uint32_t chan );
static void viddrv_complete_descriptors( uint32_t chan );
static void viddrv_reset_descriptors( uint32_t chan );

static void hvsync_ack_pending_interrupts( void );

static inline uint32_t viddrv_current_descriptor_address( uint32_t chan );

// Event notification functions
static void viddrv_notify_event( uint32_t event_code, uint32_t chan );
static error_type_t viddrv_detach_interrupt( video_handle_t *handle );
static error_type_t viddrv_attach_interrupt(video_handle_t *handle);

// debug routines
bool viddrv_is_handle_valid( video_handle_t *handle );
void dump_video_desc(video_desc_t* desc);
void viddrv_dump_svd0_reg(uint32_t reg_bank_num);
void viddrv_dump_hipwm_reg(uint32_t reg_bank_num);
void viddrv_dump_hvsync_reg( void );

#ifdef HAVE_POWER_MGR
#define PIAPB_PI_CONFIG_REG ((PI_APB_CONFIG_REGS_t *)PIAPB_PI_CONFIG_BASE)
#define AOAPB_AO_CONFIG_REG ((AO_APB_CONFIG_REGS_t *)AOAPB_AO_CONFIG_BASE)

#define PWR_MGR_MY_NAME  "video"

static uint32_t pwr_mgr_id;

static void pwr_down_lvdo(void)
{
    PIAPB_PI_CONFIG_REG->PINCR = PI_APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL(PIAPB_PI_CONFIG_REG->PINCR, 0x0); // turn off laser video outputs
}

static void pwr_up_lvdo(void)
{
    PIAPB_PI_CONFIG_REG->PINCR = PI_APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL(PIAPB_PI_CONFIG_REG->PINCR, 0x3); // turn off laser video outputs
}

static void pwr_on(void)
{
    PIAPB_PI_CONFIG_REG->PINCR = PI_APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL(PIAPB_PI_CONFIG_REG->PINCR, 0x3); // turn on laser video outputs
    PIAPB_PI_CONFIG_REG->PDCR1 = PI_APB_CONFIG_PDCR1_OLVD_SSR_REPLACE_VAL(PIAPB_PI_CONFIG_REG->PDCR1, 0x3); // fast slew for VDO outputs
    AOAPB_AO_CONFIG_REG->PWCR = AO_APB_CONFIG_PWCR_LVDS_PAD_PDWN_REPLACE_VAL(AOAPB_AO_CONFIG_REG->PWCR, 0x0);   // power up VDO outputs
#ifdef HAVE_SERENITY
    FPGA3_CONFIG_REGS_t *fpga3_config = (FPGA3_CONFIG_REGS_t *)FPGA3_CONFIG_BASE;
    // test code for causing automatic generation of stripes on the page by hardware
//    fpga3_config->Peripheral = FPGA3_CONFIG_PERIPHERAL_PRINTSTRIPEMASK_REPLACE_VAL(fpga3_config->Peripheral, 0xFF00);
//    fpga3_config->Peripheral = FPGA3_CONFIG_PERIPHERAL_PRINTSTRIPES_REPLACE_VAL(fpga3_config->Peripheral, 1);

    // It is currently not understood why this is necessary, but it seems to be needed to get SS_1000 to print
    // on the serenity emulation systems.  This code causes the code on the FPGA on the video board to be reloaded.
    // It is automatically loaded when the system comes up so this should not be necessary, but the system
    // produces only blank pages unless this is done.
    fpga3_config->Peripheral = FPGA3_CONFIG_PERIPHERAL_VDO_PROGB_NOE_REPLACE_VAL(fpga3_config->Peripheral, 0);
    posix_sleep_ticks(3);
    fpga3_config->Peripheral = FPGA3_CONFIG_PERIPHERAL_VDO_PROGB_NOE_REPLACE_VAL(fpga3_config->Peripheral, 1);
    posix_sleep_ticks(3);
#endif //HAVE_SERENITY
    viddrv_init_hardware();
}
static void pwr_off(void)
{
    PIAPB_PI_CONFIG_REG->PINCR = PI_APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL(PIAPB_PI_CONFIG_REG->PINCR, 0x0); // turn off laser video outputs
}
static bool viddrv_power_save(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    static pwr_mgr_level_t pwr = pwr_mgr_on_e;
    switch(level)
    {
    case pwr_mgr_on_e:
        if (pwr == pwr_mgr_off_e)
        {
            pwr_on();
        }
        pwr_mgr_powered_up(pwr_mgr_id);
        pwr = level;
        break;
    case pwr_mgr_reduced_power_e:
    case pwr_mgr_lowest_power_e:
        pwr_mgr_powered_down(pwr_mgr_id);
        pwr = level;
        break;
    case pwr_mgr_off_e:
        pwr_off();
        pwr_mgr_powered_down(pwr_mgr_id);
        pwr = level;
        break;
    default:
        break;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// video_pwr_init
//
//    Inputs:  
//    Returns: 
//
////////////////////////////////////////////////////////////////////////////////
static void viddrv_pwr_init(void)
{
    MSG("%s()\r\n",__FUNCTION__);

    pwr_on();

    pwr_mgr_id = pwr_mgr_reg_callback(PWR_MGR_MY_NAME, video_power_save, PWRMGR_VIDEO_PRIORITY);

    // temp set pwr level here till unity get AOAO schema mgr
    pwr_mgr_set_module_pwr_level(PWR_MGR_MY_NAME, PWRMGR_VIDEO_LEVEL);   

    return;
} 
#else
static void pwr_down_lvdo(void)
{
}

static void pwr_up_lvdo(void)
{
}

#endif


error_type_t viddrv_paperless_setup(uint32_t chan)
{
    // Set HiPWM Pulse Width...
    hipwm_reg[chan]->HSPWR = 0x10;

    // Set HipWM Quiescent Time...
    hipwm_reg[chan]->HSQR = TEST_HSYNC_PERIOD;

    MSG("  HiPWM HSPWR: 0x%x HSQR: %u\n", (uint32_t)hipwm_reg[chan]->HSPWR, (uint32_t)hipwm_reg[chan]->HSQR);

    return(OK);
}

error_type_t viddrv_paperless_enable(uint32_t chan, bool enable)
{
    // Enable/Disable test mode and synthetic beam detect.
    // When in paperless mode we also disable the PTUNDRRUN interrupt because
    // it can be spuriously generated as documented in the Test Modes table of
    // the HiPWM programming guide.
    if(enable) 
    {
        viddrv_clear_interrupt_mask(&video_handle[chan], SVDO_VIER_PTUNDERRUNINT_MASK);
        hipwm_reg[chan]->HCR |= (HIPWM_HCR_TESTMODE_MASK | HIPWM_HCR_SYNTHHSYNCEN_MASK);
    } 
    else 
    {
        hipwm_reg[chan]->HCR &= ~(HIPWM_HCR_TESTMODE_MASK | HIPWM_HCR_SYNTHHSYNCEN_MASK);
        viddrv_acknowledge_pending_ints(chan, SVDO_VIER_PTUNDERRUNINT_MASK);
        viddrv_set_interrupt_mask(&video_handle[chan], SVDO_VIER_PTUNDERRUNINT_MASK);
    }

    MSG("  HIPWM HCR: 0x%x\n", (uint32_t)hipwm_reg[chan]->HCR);

    return(OK);
}

error_type_t viddrv_paperless_clear_CRC(uint32_t chan)
{
    hipwm_reg[chan]->HCR |= (HIPWM_HCR_CRCRST_MASK);
    hipwm_reg[chan]->HCR &= ~(HIPWM_HCR_CRCRST_MASK);

    return(OK);
}

uint32_t viddrv_paperless_get_CRC(uint32_t chan)
{
    return(hipwm_reg[chan]->HCRC);
}

error_type_t viddrv_init_handle(uint32_t chan)
{
    int i;
    void* hw_addr;

    memset(&video_handle[chan], 0, sizeof(video_handle_t));
    video_handle[chan].open = false;
    video_handle[chan].channel = chan;
    video_handle[chan].handle_id = VIDEO_HANDLE_ID;

    // allocate and init all descriptors for this channel; the dma_next_address field
    // is initialized to form a circular list of descriptors
    for(i = 0; i < NUM_VIDEO_DMA_DESCS_PER_CHAN; i++)
    {
        video_handle[chan].video_dma_descs[i] = (video_desc_t *)MEM_MALLOC_UNCACHED((void*)&hw_addr, sizeof(video_desc_t), e_32_byte);
        if(video_handle[chan].video_dma_descs[i] == NULL)
        {
            goto ERROR;
        }
        memset(video_handle[chan].video_dma_descs[i], 0, sizeof(video_desc_t));
        video_handle[chan].video_dma_descs[i]->hw_address = hw_addr;
        if(i > 0)
        {
            video_handle[chan].video_dma_descs[i-1]->dma_next_address = (uint32_t)video_handle[chan].video_dma_descs[i]->hw_address;
        }
    }
    video_handle[chan].video_dma_descs[i-1]->dma_next_address = (uint32_t)video_handle[chan].video_dma_descs[0]->hw_address;  // link the last desc back to first desc

    return OK;

ERROR:
    for(i = 0; i < NUM_VIDEO_DMA_DESCS_PER_CHAN; i++)
    {
        if(video_handle[chan].video_dma_descs[i] != NULL)
        {
            MEM_FREE_UNCACHED(video_handle[chan].video_dma_descs[i]->hw_address, video_handle[chan].video_dma_descs[i]);
            video_handle[chan].video_dma_descs[i] = NULL;
        }
    }
    return FAIL;
}

/* -------------------------------------------------------------------------- */
/*                     E X P O R T E D  F U N C T I O N S                     */
/* -------------------------------------------------------------------------- */

void viddrv_init(laser_engine_video_config_t* eng_video_config)
{
    error_type_t result;
    uint32_t i;
    dros_err_t status;

    MSG("video_init entry\n");

    // Get the platform configuration
    ASSERT(video_config == NULL);       // has viddrv_init() already been called?
    result = video_get_config(&video_config); 
    ASSERT(result == OK);
    ASSERT(video_config != NULL);
    
#ifdef HAVE_CMD
    status = cmd_register( "video", NULL, NULL, NULL, video_dbcallback );
    ASSERT( CMD_OK == status );
#endif

    // Now setup all the local variables that contain the hardware addresses
    // for each of the pieces of hardware in the video block.
    for (i=0; i < NUM_VIDEO_CHAN; i++)
    {
        result = viddrv_init_handle(i);
        ASSERT(result == OK);

        // LSPTODO: why not put these into channel
        video_reg[i] = (SVDO_REGS_t *)video_config->video_base_addr[i];  
        hipwm_reg[i] = (HIPWM_REGS_t *)video_config->hipwm_base_addr[i];
        video_int_num[i] = video_config->video_int_num[i];
        MSG("i: %d, video base addr: 0x%x, hipwm base addr: 0x%x\n", i, (uint32_t)video_reg[i], (uint32_t)hipwm_reg[i]);
    }

    hvsync_reg = (HVSYNC_REGS_t *)video_config->hvsync_base_addr;

    laser_engine_video_config = eng_video_config;

    // Perform basic video hardware initialization that is only done once
    viddrv_init_hardware();

    // debug tool:  will cause more interrupt than normal caveat emptor!
    // viddrv_enable_hvsync_int();
    // intEnable(INTNUM_BDMAN); 

    status = dros_sem_init(&video_open_semaphore, "video_sem", 1);
    XASSERT(status == 0, status);

    dros_smplock_init(&viddrv_smplock);

    // Acknowledge any pending video ints
    for (i=0; i < NUM_VIDEO_CHAN; i++)
    {
        video_reg[i]->VIAR = 0xffffffff;
    }

 //   viddrv_init_pcode_LUTs(); // Setup default pulse code tables

#ifdef HAVE_POWER_MGR
    viddrv_pwr_init();
#endif

    MSG("video_init exit\n");
}


error_type_t viddrv_enable_video(video_handle_t *handle, bool enable)
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    hipwm_reg[chan]->HCR = HIPWM_HCR_ENABLE_REPLACE_VAL(hipwm_reg[chan]->HCR, (enable) ? 1 : 0);
    video_reg[chan]->SCFG = SVDO_SCFG_SVDOENABLE_REPLACE_VAL(video_reg[chan]->SCFG, (enable) ? 1 : 0);

    SLOG_MSG("video enable chan %d, enable %d, reg = %#x\n", chan, enable, video_reg[chan]->SCFG);

    return OK;
}


error_type_t viddrv_reset_hardware_block(video_handle_t *handle)
{
    uint32_t chan, counter;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    // Disable video interrupt for this channel
    viddrv_enable_interrupt(handle, false);

    // Acknowledge all outstanding interrupts
    viddrv_acknowledge_pending_ints(chan, 0xFF);
    hvsync_ack_pending_interrupts();
   

    // Stop all DMA's in progress and reset the hardware - all that is required
    // is to disable the video block. If there is a DMA in progress, the current
    // DMA bus transaction will complete and then stop. Use the DMA status
    // register to determine when the DMA has stopped. No other descriptor will
    // be started.
    viddrv_enable_video(handle, false);

    counter = 0;
    while (video_reg[chan]->STAT & SVDO_STAT_DMAACTIVE_MASK)
    {
        counter++;
        if (counter == 1000)
        {
            return FAIL;
        }
    }

    viddrv_enable_video(handle, true);
    viddrv_enable_interrupt(handle, true);

    return OK;
}

static inline void viddrv_reset_hvsync(void)
{
    hvsync_reg->CFG = HVSYNC_CFG_HVSYNCENABLE_REPLACE_VAL(hvsync_reg->CFG, 0);
    hvsync_reg->CFG = HVSYNC_CFG_HVSYNCENABLE_REPLACE_VAL(hvsync_reg->CFG, 1);
}

// cancel all open video channels.  Also reset hvsync to clear the topqueue.
error_type_t viddrv_cancel( void )
{
    int i;

    MSG("==> %s\n", __func__);

    for (i = 0; i < NUM_VIDEO_CHAN; i++)
    {
        if (video_handle[i].open)
        {
            viddrv_reset_hardware_block(&video_handle[i]);
            viddrv_reset_descriptors(video_handle[i].channel);
            video_handle[i].page_in_progress = false;
        }
    }

    // reset hvsync; This is needed to clear the top queue so we don't have a stale
    // top indication hanging around.
    viddrv_reset_hvsync();

    MSG("<== %s\n", __func__);
    return OK;
}

error_type_t viddrv_set_line_width( video_handle_t *handle,  
                                   uint32_t line_width_in_bytes )
{
    uint32_t chan;

    MSG("set line width %d, handle: 0x%x\n", line_width_in_bytes, (uint32_t)handle);
     
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    SLOG_MSG("chan %d, line width %d\n", chan, line_width_in_bytes);

    video_reg[chan]->VLWR = line_width_in_bytes;

    return OK;
}


error_type_t viddrv_set_page_bpp(video_handle_t *handle, uint32_t output_bpp)
{
    uint32_t chan;

    MSG("set bpp %d, handle: 0x%x\n", output_bpp, (uint32_t)handle);
     
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    if(1 == output_bpp)
    {
        // Set bitsPerPix field to 0 to denote 1 bit depth
        video_reg[chan]->SCFG = SVDO_SCFG_BPP_REPLACE_VAL(video_reg[chan]->SCFG, 0);
    }
    else if(2 == output_bpp)
    {
        // Set bitsPerPix field to 1 to denote 2 bit depth
        video_reg[chan]->SCFG = SVDO_SCFG_BPP_REPLACE_VAL(video_reg[chan]->SCFG, 1);
    }
    else if(4 == output_bpp)
    {
        // Set bitsPerPix field to 2 to denote 4 bit depth
        video_reg[chan]->SCFG = SVDO_SCFG_BPP_REPLACE_VAL(video_reg[chan]->SCFG, 2);
    }

    //video_program_pcode_registers(chan, output_bpp);

    return OK;
}


/* -------------------------------------------------------------------------- */
/*          V I D E O  D M A  D E S C R I P T O R  F U N C T I O N S          */
/* -------------------------------------------------------------------------- */

static error_type_t viddrv_configure_descriptor( video_desc_t *desc,
                                                char *p_buffer_data,
                                                uint32_t size_in_bytes,  // Size of requested transfer
                                                uint32_t line_width_in_bytes,
                                                uint32_t num_lines_per_beam, // Number of lines in the transfer
                                                uint32_t beam,  // 0 to number of beams per plane -1
                                                bool SOI,                // Is this the SOI DMA in the session?
                                                bool EOI,                // Is this the EOI DMA in the session?
                                                bool blank,              // Set if the buffer is blank or white space
                                                bool right_to_left,      // True if the data is backward order, or right to left
                                                bool top_to_bottom,      // true if page is not reversed 
                                                bool stop )              // Stop on finish?
{
    uint32_t config = 0;

    ASSERT( (desc->dma_config & OWNERSHIP_BIT) == 0);

    desc->dma_line_count = num_lines_per_beam;

    // Always interrupt on finish
    config = INT_ON_FINISH_BIT | OWNERSHIP_BIT;

    if (stop)
    {
        config |= STOP_ON_FINISH_BIT;
        SLOG_MSG("*** Descriptor stop on finish ***\n");
    }

    if (blank)
    {
        config |= BLANK_BUFFER_BIT;
    }

    if (right_to_left)
    {
        config |= RIGHT_TO_LEFT_BIT;
    }
      
    if (SOI)
    {
        SLOG_MSG("*** SOI configured ***\n");
        config |= START_OF_IMAGE_BIT;
    }
    
    if (EOI)
    {
        SLOG_MSG("*** EOI configured ***\n");
        config |= END_OF_IMAGE_BIT;
        config |= STOP_ON_FINISH_BIT;
    }
       
    if (right_to_left)
    {
	    if ( top_to_bottom ) {
		    // For right to left DMA operations, the starting data pointer must be 
		    // offset the width of one line in bytes minus the word width in bytes.
		    desc->dma_src_data_address = (uint32_t)( p_buffer_data 
                                                     + (line_width_in_bytes * (beam + 1))
                                                     - VDO_WORD_SIZE_IN_BYTES );

	    } else {
		    // decrement bytes and decrement lines for backside of duplex 180 rotation/mirror
		    // 
		    desc->dma_src_data_address = (uint32_t) (p_buffer_data + size_in_bytes 
                                                     - VDO_WORD_SIZE_IN_BYTES
                                                     - (line_width_in_bytes * beam));
	    }
    }
    else
    {
	    if ( top_to_bottom ) {
		    /// normal case increment bytes and lines
		    desc->dma_src_data_address = (uint32_t)p_buffer_data + (line_width_in_bytes * beam);
	    } else {
		    /// left to right, bottom to top 
		    desc->dma_src_data_address = (uint32_t)( p_buffer_data + size_in_bytes 
                                                     - (line_width_in_bytes * (beam + 1)));
        }
    }

    desc->dma_config = config;
#if 0	// del.lsh too much amount of kernel log 
    SLOG_MSG("desc 0x%x, data 0x%x, cfg: 0x%x, size: %d\n", 
              (uint32_t)desc, desc->dma_src_data_address, desc->dma_config, size_in_bytes);
#endif
    return OK;
}


static video_desc_t *viddrv_get_descriptor(uint32_t chan)
{
    video_desc_t *desc = NULL;
    uint32_t lockflags;

    XASSERT(video_handle[chan].num_descriptors_in_use <= NUM_VIDEO_DMA_DESCS_PER_CHAN, video_handle[chan].num_descriptors_in_use);

    dros_smplock_acquire_isr(&viddrv_smplock, &lockflags);
    if(video_handle[chan].num_descriptors_in_use < NUM_VIDEO_DMA_DESCS_PER_CHAN)
    {
        desc = video_handle[chan].video_dma_descs[video_handle[chan].next_descriptor];
        XASSERT(!(desc->dma_config & OWNERSHIP_BIT), desc->dma_config);

        if(++video_handle[chan].next_descriptor == NUM_VIDEO_DMA_DESCS_PER_CHAN)
        {
            video_handle[chan].next_descriptor = 0;
        }
        video_handle[chan].num_descriptors_in_use++;
    }
    dros_smplock_release_isr(&viddrv_smplock, lockflags);

    return desc;
}

static void viddrv_reset_descriptors( uint32_t chan )
{
    int i;

    video_handle[chan].descriptor_in_flight = 0;
    video_handle[chan].num_descriptors_in_use = 0;
    video_handle[chan].next_descriptor = 0;
    video_handle[chan].last_descriptor = 0;

    for (i = 0; i < NUM_VIDEO_DMA_DESCS_PER_CHAN; i++)
    {
        // on underrun descriptors can be left with the ownership bit set; clean it up here
        video_handle[chan].video_dma_descs[i]->dma_config = 0;
        video_handle[chan].video_dma_descs[i]->dma_src_data_address = 0;
        video_handle[chan].video_dma_descs[i]->dma_line_count = 0;
    }
}

static inline uint32_t viddrv_current_descriptor_address( uint32_t chan )
{
    return (uint32_t)video_reg[chan]->VDAR;
}

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
                                     bool right_to_left )
{
    video_desc_t *my_desc = NULL;
    uint32_t chan;

    DBG_SLOG_ENTRY

    if (false == viddrv_is_handle_valid( handle ))
    {
        SLOG_ERR("handle is invalid\n");
        return FAIL;
    }

    chan = handle->channel;

    my_desc = viddrv_get_descriptor(chan);
    if(my_desc == NULL)
    {
        return FAIL;
    }

    if (num_lines_per_beam == 0)
    {
        // on ThreadX should probably be an assert, but use a debug message on linux to not crash the kernel.
        SLOG_ERR("%s: WARNING! num_lines_per_beam == 0.  VIDEO UNDERRUN WILL OCCUR\n", __func__);
    }

    my_desc->user_data = user_data;

    viddrv_configure_descriptor( my_desc,
                                data_buffer,
                                buffer_size_in_bytes,
                                line_width_in_bytes,
                                num_lines_per_beam,
                                beam,
                                SOI,  
                                EOI,
                                false, // REVISIT DAB
                                right_to_left,
                                handle->top_to_bottom, 
                                false);  

    DBG_SLOG_EXIT

    return OK;
}



error_type_t viddrv_register_dma_complete_callback( video_handle_t *handle,
                                                   viddrv_dma_callback_t callback_func,
                                                   void *callback_handle )
{
    if (handle == NULL)
    {
        MSG("%s ERROR - no handle!!!\n", __func__);
        ASSERT(0);
        return FAIL;
    }

    MSG("%s handle: 0x%x, callback_func: 0x%x, dma_callback_handle 0x%x\n", __func__, (uint32_t)handle, (uint32_t)callback_func, (uint32_t)callback_handle);

    handle->dma_callback_function = callback_func;
    handle->dma_callback_handle = callback_handle;
    return OK;
}

error_type_t viddrv_register_event_callback(video_handle_t *handle, viddrv_event_callback_t event_callback, void* context)
{
    if (handle == NULL)
    {
        MSG("%s ERROR - no handle!!!\n", __func__);
        ASSERT(0);
        return FAIL;
    }

    MSG("%s handle: 0x%x, callback_func: 0x%x, context 0x%x\n", __func__, (uint32_t)handle, (uint32_t)event_callback, (uint32_t)context);

    handle->event_callback = event_callback;
    handle->event_context = context;
    return OK;
}

static inline bool is_video_descriptor_complete(video_desc_t *desc)
{
    return !(desc->dma_config & OWNERSHIP_BIT);
}

// 
static void viddrv_complete_descriptors( uint32_t chan )
{
    video_desc_t *desc = NULL;
    void* context;

//    SLOG_INFO("%s: chan %d, num in use: %d\n", __func__, chan, video_handle[chan].num_descriptors_in_use);
    // LSPTODO: NEED to SMPISR lock this access
    XASSERT(video_handle[chan].num_descriptors_in_use <= NUM_VIDEO_DMA_DESCS_PER_CHAN, video_handle[chan].num_descriptors_in_use);
    dros_smplock_acquire(&viddrv_smplock);
    while(video_handle[chan].num_descriptors_in_use > 0)
    {
        desc = video_handle[chan].video_dma_descs[video_handle[chan].last_descriptor];
//        SLOG_INFO("%s: desc %d @ %#x\n", __func__, video_handle[chan].last_descriptor, (uint32_t)desc);
        if(is_video_descriptor_complete(desc))
        {
            if(++video_handle[chan].last_descriptor == NUM_VIDEO_DMA_DESCS_PER_CHAN)
            {
                video_handle[chan].last_descriptor = 0;
            }
            video_handle[chan].num_descriptors_in_use--;
            context = desc->user_data;
            desc->dma_src_data_address = 0;
            desc->dma_line_count = 0;

            // notify upper level driver of descriptor completion
            if(video_handle[chan].dma_callback_function)
            {
                dros_smplock_release(&viddrv_smplock);
                video_handle[chan].dma_callback_function( video_handle[chan].dma_callback_handle, context );
                dros_smplock_acquire(&viddrv_smplock);
            }
        }
        else
        {
            // we're done once we reach the first descriptor that is owned by the hw
            break;
        }
    }
    dros_smplock_release(&viddrv_smplock);
}


error_type_t viddrv_set_line_skip(video_handle_t *handle, uint32_t num_lines_to_skip)
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    if (num_lines_to_skip >=0 && num_lines_to_skip <= 7)
    {
        video_reg[chan]->SCFG = SVDO_SCFG_LINESKIP_REPLACE_VAL(video_reg[chan]->SCFG, num_lines_to_skip);
        return OK;
    }
    else
    {
        ASSERT(0);
        return FAIL;
    }
}


error_type_t viddrv_set_line_replication(video_handle_t *handle, uint32_t replicate)
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    if (replicate >=0 && replicate <= 3)
    {
        video_reg[chan]->SCFG = SVDO_SCFG_LINEREPL_REPLACE_VAL(video_reg[chan]->SCFG, replicate);
        return OK;
    }
    else
    {
        ASSERT(0);
        return FAIL;
    }
}


error_type_t viddrv_set_resolution(video_handle_t *handle,
                                  uint32_t pixel_freq_in_hz,
                                  resolution_t engine_resolution, 
                                  resolution_t print_resolution_x,
                                  resolution_t print_resolution_y)
{
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    // The configuration of the video hardware to support a given print
    // resolution depends upon the native resolution of the print engine. In the
    // scan or horizontal direction the resolution is determined by the nominal
    // pixel frequency. For laser printers, the pixel frequency is usually 
    // specified for 600 or 1200 dpi resolution. If the horizontal resolution 
    // is desired to be 300 dpi, the pixel frequency for a 600 dpi engine will 
    // need to be divided by 2, and for a 1200 dpi engine it will need to be 
    // divided by 4. 

    if (engine_resolution == DPI_600)
    {
        switch (print_resolution_x)
        {
        case DPI_300:
            // Cut the pixel frequency in half for 300 dpi
            viddrv_program_pixel_freq_default(handle, pixel_freq_in_hz/2); 
            break;

        case DPI_600:
            // For 600 dpi engine resolution.
            viddrv_program_pixel_freq_default(handle, pixel_freq_in_hz); 
            break;

        case DPI_1200 :
            // dbg_printf("VIDEO DOUBLE THE PIXEL FREQUENCY!!!!!! 1200 DPI\n");
            // For 1200 dpi print resolution on a 600 dpi engine, leave the 
            // pixel frequency the same as for 600 dpi. The image data will be 
            // converted from 1200 dpi to 600 dpi, 4 bit per pixel.
            viddrv_program_pixel_freq_default(handle, 2*pixel_freq_in_hz); 
            break;

        default:
            XASSERT(0, print_resolution_x);
            break;
        }

        switch (print_resolution_y)
        {
            case DPI_300:
            // Set the video hardware to replicate each 300 dpi line
            // REVISIT
            viddrv_set_line_replication(handle, 1);
            break;

        case DPI_400:
        case DPI_600:
        case DPI_1200 :
            // No line replication!
            viddrv_set_line_replication(handle, 0);
            break;

        default:
            XASSERT(0, print_resolution_y);
            break;
        }
    }
    else
    {
        // Other engine resolutions are currently not supported
        ASSERT(0);
        return FAIL;
    }

    return OK;
}


error_type_t viddrv_set_pulse_justification_regs(video_handle_t *handle, 
                                                const uint32_t *regArray)
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    video_reg[chan]->JUST0 = regArray[0];
    video_reg[chan]->JUST1 = regArray[1];
    video_reg[chan]->JUST2 = regArray[2];
    video_reg[chan]->JUST3 = regArray[3];
    video_reg[chan]->JUST4 = regArray[4];
    video_reg[chan]->JUST5 = regArray[5];
    video_reg[chan]->JUST6 = regArray[6];
    video_reg[chan]->JUST7 = regArray[7];
    video_reg[chan]->JUST8 = regArray[8];
    video_reg[chan]->JUST9 = regArray[9];
    video_reg[chan]->JUST10 = regArray[10];
    video_reg[chan]->JUST11 = regArray[11];
    video_reg[chan]->JUST12 = regArray[12];
    video_reg[chan]->JUST13 = regArray[13];
    video_reg[chan]->JUST14 = regArray[14];
    video_reg[chan]->JUST15 = regArray[15];

    return OK;
}


error_type_t viddrv_set_pulse_code_regs( video_handle_t *handle, 
                                        const uint32_t *array)
{
    uint32_t chan;
    uint32_t *ptr = (uint32_t *)array;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    MSG("%s pcode array: 0x%x\n", __func__, (uint32_t)array);

    video_reg[chan]->PCODE0 = *ptr++; // array[0];
    video_reg[chan]->PCODE1 = *ptr++; // array[1];
    video_reg[chan]->PCODE2 = *ptr++; // array[2];
    video_reg[chan]->PCODE3 = *ptr++; // array[3];
    video_reg[chan]->PCODE4 = *ptr++; // array[4];
    video_reg[chan]->PCODE5 = *ptr++; // array[5];
    video_reg[chan]->PCODE6 = *ptr++; // array[6];
    video_reg[chan]->PCODE7 = *ptr++; // array[7];
    video_reg[chan]->PCODE8 = *ptr++; // array[8];
    video_reg[chan]->PCODE9 = *ptr++; // array[9];
    video_reg[chan]->PCODE10 = *ptr++; // array[10];
    video_reg[chan]->PCODE11 = *ptr++; // array[11];
    video_reg[chan]->PCODE12 = *ptr++; // array[12];
    video_reg[chan]->PCODE13 = *ptr++; // array[13];
    video_reg[chan]->PCODE14 = *ptr++; // array[14];
    video_reg[chan]->PCODE15 = *ptr++; // array[15];

    return OK;
}


// Driver open/close functions

error_type_t viddrv_open(video_handle_t **handle, uint32_t chan, bool top_to_bottom)
{
    dros_err_t status;
    error_type_t result = FAIL;

    DBG_SLOG_ENTRY
    ASSERT(video_config != NULL);  // has the driver been initialized?

    // is viddrv_open() already in use?
    status = dros_sem_wait(&video_open_semaphore);
    XASSERT(status == 0, status);

    // is channel valid?
    ASSERT(chan >= 0 && chan < NUM_VIDEO_CHAN);
    ASSERT(video_handle[chan].channel == chan);
    ASSERT(video_handle[chan].handle_id == VIDEO_HANDLE_ID);

    // open the channel if it's available
    if ( video_handle[chan].open == false ) 
    {                                      
        char dev_name[32];
        MSG("open channel %d  handle 0x%x\n", chan, (uint32_t)&video_handle[chan]);

        sprintf(dev_name, "video_laser_irq_%d", chan);

        viddrv_reset_descriptors(chan);
        video_handle[chan].page_in_progress = false;

        video_handle[chan].open = true;                                         
        *handle = &video_handle[chan];

        viddrv_attach_interrupt(&video_handle[chan]);

        viddrv_reset_hardware_block(*handle);
        viddrv_apply_configuration(*handle, top_to_bottom);

        if( laser_engine_video_config && laser_engine_video_config->power_down_lvdo_between_pages)
        {
            viddrv_enable_hvsync_interrupt(&video_handle[chan], true);
        }
// UNCOMMENT FOR HVSYNC DEBUG  
//        viddrv_enable_hvsync_interrupt(&video_handle[chan], true);
        
//-jrs        viddrv_set_interrupt_mask( &video_handle[chan], 0xFF);
        viddrv_set_interrupt_mask( &video_handle[chan], (0xFF & ~SVDO_VIER_HSYNCERRINT_MASK));
        viddrv_enable_interrupt( &video_handle[chan], true);

        result = OK;
    }

    // return viddrv_open() to not in use
    status = dros_sem_post(&video_open_semaphore);
    XASSERT(status == 0, status);
    
    DBG_SLOG_EXIT
    return result;
}


error_type_t viddrv_close(video_handle_t *handle)
{
    uint32_t channel = handle->channel;
    error_type_t result = FAIL;
    dros_err_t status;

 //   DBG_SLOG_ENTRY

    ASSERT(video_config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < NUM_VIDEO_CHAN);
    ASSERT(video_handle[channel].channel == channel);
    ASSERT(video_handle[channel].handle_id == VIDEO_HANDLE_ID);

    status = dros_sem_wait(&video_open_semaphore);
    XASSERT(status == 0, status);

    if (channel < NUM_VIDEO_CHAN) 
    {
        // is this channel is use?
        // has this channel been opened?
        ASSERT(video_handle[channel].open == true);
        if(video_handle[channel].open == true) 
        {
            viddrv_enable_interrupt(handle, false);
            viddrv_detach_interrupt(handle);
            video_handle[channel].open = false;
            result = OK;
        }
    } 
    else 
    {
        ASSERT(NULL == &"video_close: close failed channel in use");
    }

    status = dros_sem_post(&video_open_semaphore);
    XASSERT(status == 0, status);

    return result;
}

bool viddrv_is_handle_valid( video_handle_t *handle )
{
    if (handle != 0)
    {
        if (handle->handle_id == VIDEO_HANDLE_ID)
        {
            return true;
        }
    }

    SLOG_MSG("************************** PUT ASSERT BACK IN HERE *****************************\n");
   // ASSERT(0);
    return false;
}


error_type_t viddrv_dma_start_xfer(video_handle_t *handle)
{
    uint32_t chan;
    uint32_t lockflags;

    if (false == viddrv_is_handle_valid( handle ))
    {
        ASSERT(0);
        return FAIL;
    }

    chan = handle->channel;

    SLOG_MSG("%s ENTRY handle 0x%x, chan %d\n", __func__, (uint32_t)handle, chan);

    dros_smplock_acquire_isr(&viddrv_smplock, &lockflags);
    if (handle->descriptor_in_flight)
    {
        dros_smplock_release_isr(&viddrv_smplock, lockflags);
        SLOG_INFO("%s early EXIT in flight\n", __func__);
        DBG_SLOG_EXIT
        return OK;
    }

    if (handle->num_descriptors_in_use > 0)
    {
        handle->descriptor_in_flight = true;
        // we always start the transfer w/ descriptor 0
        viddrv_write_start_descriptor_to_hardware(handle->video_dma_descs[0], chan);
    }
    else
    {
        SLOG_WRN("%s chain EMPTY!\n", __func__);
    }
    dros_smplock_release_isr(&viddrv_smplock, lockflags);    
    return OK;
}


/* -------------------------------------------------------------------------- */
/*                       H V S Y N C  F U N C T I O N S                       */
/* -------------------------------------------------------------------------- */

uint32_t hsync_count[NUM_VIDEO_CHAN];
uint32_t top=0;

static void isr_hvsync_common(uint32_t context)
{
    uint32_t my_ints;
    uint32_t i;
    uint32_t chan;
   
    my_ints = hvsync_reg->HVSIPR & hvsync_reg->HVSIER;    

    chan = (uint32_t)context;

    // logString("%s 0x%x\n", __func__, my_ints); 
    SLOG_ERR("isr_hvsync: ints = %#x\n", my_ints);
    
    // Track horizontal sync signals for each video channel. Used exclusively
    // for hardware debug to ensure hsync signals are present.
    if (my_ints & HVSYNC_HVSIAR_HSYNCINT_MASK)
    {
        for (i=0; i < NUM_VIDEO_CHAN; i++)
        {
            if (my_ints & 1<<(i+HVSYNC_HVSIAR_HSYNCINT_SHIFT))
            {
                hsync_count[i]++;
            }
        }
    }


    if (my_ints & HVSYNC_HVSIAR_BOTINT_MASK)
    {
        SLOG_ERR("%s  chan: %d bottom of page\n", __func__, chan);
        // for some engines we need to turn power of to the LVDO pad to conserver power
        if( laser_engine_video_config && laser_engine_video_config->power_down_lvdo_between_pages)
        {
            pwr_down_lvdo();
        }
    }

    if (my_ints & HVSYNC_HVSIAR_TOPINT_MASK)
    {
        // for some engines we need to turn power of to the LVDO pad to conserver power so we need to make sure we turn it on here
        if( laser_engine_video_config && laser_engine_video_config->power_down_lvdo_between_pages)
        {
            pwr_up_lvdo();
        }

    	for (i=0; i < NUM_VIDEO_CHAN; i++)
        {
            if (my_ints & 1<<(i+HVSYNC_HVSIAR_TOPINT_SHIFT))
            {
                top++;
                SLOG_ERR("isr_hvsync: top for chan %d\n", i);
            }
        }
    }

    if (my_ints & HVSYNC_HVSIAR_TOPQFULLINT_MASK)
    {
        SLOG_ERR("isr_hvsync: top queue full ints = %#x\n", my_ints);
    }

    if (my_ints & HVSYNC_HVSIAR_MEASDONEINT_MASK)
    {
    	SLOG_ERR( "isr_hvsync: meas done\n");
    }

    if (my_ints & HVSYNC_HVSIAR_TOPMEASDONEINT_MASK )
    {
        SLOG_ERR( "isr_hvsync: top meas done\n");
    }

    if (my_ints & HVSYNC_HVSIAR_TOPDLYINT_MASK)
    {
        for (i=0; i < NUM_VIDEO_CHAN; i++)
        {
            if (my_ints & 1<<(i+HVSYNC_HVSIAR_TOPDLYINT_SHIFT))
            {
                SLOG_ERR("isr_hvsync: top delay for chan %d\n", i);
            }
        }
    }

    // Acknowledge all pending interrupts that were serviced
    hvsync_reg->HVSIAR = my_ints;
}

#ifdef __KERNEL__
static irqreturn_t isr_hvsync(int32_t interrupt_count, void *context)
{
    isr_hvsync_common((uint32_t)context);
    return IRQ_HANDLED;
}
#else
static void isr_hvsync(uint32_t context)
{
    isr_hvsync_common(context);
}
#endif


static void viddrv_attach_hvsync_interrupt(void)
{
    static uint32_t hvsync_attached = 0;

    if (hvsync_attached == 0)
    {
        intAttach(video_config->hvsync_int_num, 0, isr_hvsync, "laser_video_hvsync", 0);
        hvsync_attached = 1;
    }
}

static void hvsync_ack_pending_interrupts( void )
{
    // Acknowledge all pending interrupts
    hvsync_reg->HVSIAR = 0xffffffff;
}


// This function sets up the hsync lockout period for each video channel. 
// Depending on the product configuration, there could be 1, 2 or 4 hsync inputs
// and 1, 2, 4 or 8 video channels. The mapping of hsync inputs to video
// channels is taken care of in the product configuration file.
error_type_t viddrv_set_hsync_lockout(uint32_t lockout_in_hipwm_clocks)
{
    uint32_t input;

    for (input=0; input < NUM_HSYNC_INPUTS; input++)
    {
        switch (input)
        {
            case 0:
                hvsync_reg->HLOR0 = HVSYNC_HLOR0_HSYNCLOCKOUT_REPLACE_VAL(hvsync_reg->HLOR0, lockout_in_hipwm_clocks);
                break;

#if (NUM_HSYNC_INPUTS >= 2)
            case 1:
                hvsync_reg->HLOR1 = HVSYNC_HLOR1_HSYNCLOCKOUT_REPLACE_VAL(hvsync_reg->HLOR1, lockout_in_hipwm_clocks);
                break;

#if (NUM_HSYNC_INPUTS == 4)
            case 2:
                hvsync_reg->HLOR2 = HVSYNC_HLOR2_HSYNCLOCKOUT_REPLACE_VAL(hvsync_reg->HLOR2, lockout_in_hipwm_clocks);
                break;

            case 3: 
                hvsync_reg->HLOR3 = HVSYNC_HLOR3_HSYNCLOCKOUT_REPLACE_VAL(hvsync_reg->HLOR3, lockout_in_hipwm_clocks);
                break;
#endif
#endif
            default:
                ASSERT(0);
                break;
        }
    }

    return OK;
}

void viddrv_set_hsync_mirrors(laser_engine_video_config_t *config)
{
    uint32_t i;
    
    // TODO: need to program mirror select register
    // for now all video channels map to mirror 0

    // configure number of mirror facets
    for(i = 0; i < config->num_mirrors ; i++)
    {
        switch(i)
        {
            case 0:
                hvsync_reg->MIR_CFG0 = HVSYNC_MIR_CFG0_NUMFACETS_REPLACE_VAL(hvsync_reg->MIR_CFG0, config->num_mirror_facets[i]-1);
                break;
            case 1:
                hvsync_reg->MIR_CFG1 = HVSYNC_MIR_CFG1_NUMFACETS_REPLACE_VAL(hvsync_reg->MIR_CFG1, config->num_mirror_facets[i]-1);
                break;
            default:
                ASSERT(0);
                break;
        }
    }
}

error_type_t viddrv_program_output_select_registers(void)
{
#if defined(HVSYNC_OUT_SEL0_TOPOUT_REPLACE_VAL)
// WARNING: currrently the HVSYNC_OUT_SEL0_TOPOUT_REPLACE_VAL is defined only on gemstone and granite
    uint32_t top, chan, hsync_src;
    laser_engine_video_config_t *config = laser_engine_video_config;

    // Use the top signal and video channel mapping information to set up the
    // output select registers.

    for (chan=0; chan<NUM_VIDEO_CHAN; chan++)
    {
        if (config->active_video_channels[chan])
        {
            top = config->top_to_print_plane[chan];
            hsync_src = config->video_chan_hsync_input[chan];
            MSG("%s: chan %d top out sel = %d hsync_src = %x\n", __func__, chan, top, hsync_src);

            switch (chan)
            {
                case 0:
                    hvsync_reg->OUT_SEL0 = HVSYNC_OUT_SEL0_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL0, top);
                    hvsync_reg->OUT_SEL0 = HVSYNC_OUT_SEL0_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL0, hsync_src);
//                dbg_printf("VDO: HVSYNC OUTSEL0 = %#x\n", hvsync_reg->OUT_SEL0);
                    break;
                case 1:
                    hvsync_reg->OUT_SEL1 = HVSYNC_OUT_SEL1_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL1, top);
                    hvsync_reg->OUT_SEL1 = HVSYNC_OUT_SEL1_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL1, hsync_src);
//                dbg_printf("VDO: HVSYNC OUTSEL1 = %#x\n", hvsync_reg->OUT_SEL0);
                    break;

                case 2:
                    hvsync_reg->OUT_SEL2 = HVSYNC_OUT_SEL2_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL2, top);
                    hvsync_reg->OUT_SEL2 = HVSYNC_OUT_SEL2_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL2, hsync_src);
                    break;

                case 3:
                    hvsync_reg->OUT_SEL3 = HVSYNC_OUT_SEL3_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL3, top);
                    hvsync_reg->OUT_SEL3 = HVSYNC_OUT_SEL3_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL3, hsync_src);
                    break;

#if (NUM_VIDEO_CHAN == 8)
                case 4:
                    hvsync_reg->OUT_SEL4 = HVSYNC_OUT_SEL4_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL4, top);
                    hvsync_reg->OUT_SEL4 = HVSYNC_OUT_SEL4_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL4, hsync_src);
                    break;

                case 5:
                    hvsync_reg->OUT_SEL5 = HVSYNC_OUT_SEL5_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL5, top);
                    hvsync_reg->OUT_SEL5 = HVSYNC_OUT_SEL5_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL5, hsync_src);
                    break;

                case 6:
                    hvsync_reg->OUT_SEL6 = HVSYNC_OUT_SEL6_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL6, top);
                    hvsync_reg->OUT_SEL6 = HVSYNC_OUT_SEL6_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL6, hsync_src);
                    break;

                case 7:
                    hvsync_reg->OUT_SEL7 = HVSYNC_OUT_SEL7_TOPOUT_REPLACE_VAL(hvsync_reg->OUT_SEL7, top);
                    hvsync_reg->OUT_SEL7 = HVSYNC_OUT_SEL7_HSYNCOUT_REPLACE_VAL(hvsync_reg->OUT_SEL7, hsync_src);
                    break;
#endif // (NUM_VIDEO_CHAN == 8)

                default:
                    MSG("ERROR in videoProgramOutputSelectRegisters()! Out of range value!\n");
                    ASSERT(0);
                    break;
            }
        }
    }
#endif // defined(HVSYNC_OUT_SEL0_TOPOUT_REPLACE_VAL) 
    return OK;
}

error_type_t viddrv_program_top_select_registers(void)
{
    // check that this asic supports the Top Queue HSYNC Select (TQSEL) register
#if defined(HVSYNC_TQSEL_TOPQHSYNCSEL0_MASK)
    uint32_t topq_sel_reg_val;
    laser_engine_video_config_t *config = laser_engine_video_config;

    topq_sel_reg_val = hvsync_reg->TQSEL;
    topq_sel_reg_val = HVSYNC_TQSEL_TOPQHSYNCSEL0_REPLACE_VAL(topq_sel_reg_val, config->topq_hsync_sel[0]);
    topq_sel_reg_val = HVSYNC_TQSEL_TOPQHSYNCSEL1_REPLACE_VAL(topq_sel_reg_val, config->topq_hsync_sel[1]);
    topq_sel_reg_val = HVSYNC_TQSEL_TOPQHSYNCSEL2_REPLACE_VAL(topq_sel_reg_val, config->topq_hsync_sel[2]);
    topq_sel_reg_val = HVSYNC_TQSEL_TOPQHSYNCSEL3_REPLACE_VAL(topq_sel_reg_val, config->topq_hsync_sel[3]);
    hvsync_reg->TQSEL = topq_sel_reg_val;
#endif
    return OK;
}

/* -------------------------------------------------------------------------- */
/*              V I D E O  I N T E R R U P T  F U N C T I O N S               */
/* -------------------------------------------------------------------------- */

static uint32_t viddrv_query_pending_ints(uint32_t chan)
{
    // Filter the pending interrupt bits with the enabled interrupts.
    return video_reg[chan]->VIPR & video_reg[chan]->VIER;
}

static void viddrv_acknowledge_pending_ints(uint32_t chan, uint32_t pending_ints)
{
    // Acknowledge the interrupts.
    video_reg[chan]->VIAR |= pending_ints;
}

static void viddrv_ISR_common(uint32_t context)
{ 
    uint32_t pending_ints;  
    uint32_t chan;

    chan = (uint32_t)context;
    
    // Query the pending interrupt bits.
    pending_ints = viddrv_query_pending_ints(chan);

//    SLOG_MSG("%s  channel = %d, pending_ints = %#x\n", __func__, chan, pending_ints);

    // Acknowledge the interrupts.
    viddrv_acknowledge_pending_ints(chan, pending_ints);

    // Check to see if the "end of image" interrupt occurred. This occurs after
    // all the DMA'ed data makes its way through the video pipe.
    if (pending_ints & SVDO_VIER_EOIINT_MASK)
    {
        SLOG_MSG("%s EOI chan %d\n", __func__, chan);
        viddrv_complete_descriptors(chan);

        video_handle[chan].page_in_progress = false;

        viddrv_notify_event( VIDEO_EVENT_END_OF_IMAGE, chan );
    } 
    else if (pending_ints & SVDO_VIER_DESCINT_MASK)
    {
//       SLOG_MSG("%s  DESCINT chan: %d\n", __func__, chan);
       viddrv_complete_descriptors(chan);
    }

    // Process any errors. No need to look for all errors, bail out if any error
    // is found. The following error search is in priority order.
    if ( (pending_ints & SVDO_VIER_PTUNDERRUNINT_MASK) || (pending_ints & SVDO_VIER_OWNINT_MASK) )
    {
        if (pending_ints & SVDO_VIER_OWNINT_MASK)
        {
            SLOG_ERR(">>>>>>> LINK UNDERRUN ERROR! video channel: %d\n", chan);
        }
        else
        {
            SLOG_ERR(">>>>>>> VIDEO UNDERRUN ERROR! video channel: %d\n", chan);
        }
        // remove comments on the lines below for full register dump underrun
//        viddrv_dump_svd0_reg(chan);
//        viddrv_dump_hipwm_reg(chan);
//        viddrv_dump_hvsync_reg();
//        dump_video_desc(video_handle[chan].video_dma_descs[video_handle[chan].last_descriptor]);

        video_handle[chan].page_in_progress = false;
        viddrv_notify_event( VIDEO_EVENT_LINK_OR_BUS_UNDERRUN, chan );
    }

    // Premature bottom detected. Notify the video thread.
    if (pending_ints & SVDO_VIER_BOTERRINT_MASK)
    {
        video_handle[chan].page_in_progress = false;
        viddrv_notify_event( VIDEO_EVENT_PREMATURE_END_OF_PAGE, chan );
        SLOG_MSG(">>>>>>> BOTTOM ERROR!\n");
    }

    // Horizontal sync detect error. Used for debug.
    else if (pending_ints & SVDO_VIER_HSYNCERRINT_MASK)
    {
        #ifdef HSYNC_DEBUG
        if (beam_err < 10) // Debug
        {
            SLOG_MSG(">>>>>>>> BEAM DETECT ERROR! chan: %d\n", chan);
        }
        beam_err++;
        #endif
    }

    // Bender underrun - hardware problem!
    else if (pending_ints & SVDO_VIER_BENDERUNDERRUNINT_MASK)
    {
        SLOG_MSG(">>>>>>>> BENDER UNDERRUN ERROR!!! chan: %d\n", chan);
    }

    else if (pending_ints & SVDO_VIER_TOPERRINT_MASK)
    {
        SLOG_MSG(">>>>>>>> PREMATURE TOP ERROR!!! chan: %d\n", chan);
    }
}

#ifdef __KERNEL__
static irqreturn_t viddrv_ISR(int32_t interrupt_count, void *context)
{
    viddrv_ISR_common((uint32_t)context);
    return IRQ_HANDLED;
}
#else
static void viddrv_isr(uint32_t context)
{
    viddrv_ISR_common(context);
}
#endif

// REVISIT DAB what about the mask setting for various int conditions???
error_type_t viddrv_enable_hvsync_interrupt(video_handle_t *handle, bool enable)
{
   // uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

  //  chan = handle->channel;

    // Acknowledge all pending interrupts
    hvsync_ack_pending_interrupts();
    
    viddrv_attach_hvsync_interrupt();

    if (enable)
    {
        SLOG_MSG("enable hvsync int 0x%x\n",  hvsync_reg->HVSIER);
        hvsync_reg->HVSIER |=  (HVSYNC_HVSIER_BOTINT_MASK | HVSYNC_HVSIER_TOPINT_MASK | HVSYNC_HVSIER_TOPDLYINT_MASK | HVSYNC_HVSIER_TOPQFULLINT_MASK);
        SLOG_MSG("* enable hvsync int 0x%x\n", hvsync_reg->HVSIER);
        intEnable( video_config->hvsync_int_num ); 
    }
    else
    {
        hvsync_reg->HVSIER = 0;
        intDisable( video_config->hvsync_int_num ); 
    }

    return OK;
}


error_type_t viddrv_enable_interrupt(video_handle_t *handle, bool enable)
{
    uint32_t chan;

    SLOG_MSG("%s enable=%d\n", __func__, enable);    
    
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    if( enable)
    {
        intEnable( video_int_num[chan] );
    }
    else
    { 
        intDisable(video_int_num[chan]);
    }

    return OK;
}


static error_type_t viddrv_attach_interrupt( video_handle_t *handle)
{
    uint32_t chan;

    SLOG_MSG("%s handle=0x%x\n", __func__, (uint32_t)handle);    
    
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

#ifdef __KERNEL__
    intAttach(video_int_num[chan], 0, viddrv_ISR, "laser_video", (void*)chan);
#else
    intAttach (video_int_num[chan], 0, (PFN_ISR)&viddrv_ISR, chan);
#endif

    return OK;
}


static error_type_t viddrv_detach_interrupt( video_handle_t *handle )
{
    uint32_t chan;

    SLOG_MSG("%s handle=0x%x\n", __func__, (uint32_t)handle);    
    
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    // BUGBUG: jrs should assert that int is disabled here
#ifdef __KERNEL__
    intDetach (video_int_num[chan], (void*)chan);
#else
    intDetach (video_int_num[chan]);
#endif
    return OK;
}


error_type_t viddrv_set_interrupt_mask(video_handle_t *handle,
                                      uint8_t interrupt_mask)
{
    uint32_t chan;

    SLOG_MSG("%s handle=0x%x, mask=0x%x\n", __func__, (uint32_t)handle, interrupt_mask);    
    
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;
    video_reg[chan]->VIER = video_reg[chan]->VIER | interrupt_mask;

    MSG("%s chan %d, reg 0x%x\n", __func__, chan, video_reg[chan]->VIER);
    return OK;
}


error_type_t viddrv_clear_interrupt_mask(video_handle_t *handle,
                                        uint8_t interrupt_mask)
{
    uint32_t chan;

    SLOG_MSG("%s handle=0x%x, mask=0x%x\n", __func__, (uint32_t)handle, interrupt_mask);    
    
    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    video_reg[chan]->VIER = video_reg[chan]->VIER & ~interrupt_mask;

    MSG("%s chan %d, reg 0x%x\n", __func__, chan, video_reg[chan]->VIER);

    return OK;
}



/* -------------------------------------------------------------------------- */
/*                        H I P W M  F U N C T I O N S                        */
/* -------------------------------------------------------------------------- */

/* ---------------- V I D E O  M A R G I N  F U N C T I O N S --------------- */

error_type_t viddrv_program_top_margin(video_handle_t *handle, uint32_t margin_in_lines)
{
    uint32_t chan;
    uint32_t top_margin_reg_num;
    laser_engine_video_config_t *config = laser_engine_video_config;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;
    top_margin_reg_num = config->top_to_print_plane[chan];

    MSG("%s chan: %d, hsync %d, margin: %d\n", __func__, chan, top_margin_reg_num, margin_in_lines);
    SLOG_MSG("%s chan: %d, hsync %d, margin: %d\n", __func__, chan, top_margin_reg_num, margin_in_lines);

    // program the top skew for the channel.
    hipwm_reg[chan]->HTSR = config->top_of_page_skew_in_lines[chan];

    // Configure the top margin for the HSYNC signal used to initiate the DMA on 
    // the specified channel; e.g. one HSYNC can map to multiple channels.
    switch (top_margin_reg_num)
    {
        case 0:
            hvsync_reg->HVSTPMR0 = margin_in_lines;
            MSG("%s chan: %d, margin: %d\n", __func__, chan, hvsync_reg->HVSTPMR0);
            SLOG_MSG("top margin 0: 0x%x\n", hvsync_reg->HVSTPMR0);
            break;

#if (NUM_HSYNC_INPUTS >= 2)
        case 1: 
            hvsync_reg->HVSTPMR1 = margin_in_lines;
            break;

#if (NUM_HSYNC_INPUTS == 4)
        case 2:
            hvsync_reg->HVSTPMR2 = margin_in_lines;
            break;

        case 3: 
            hvsync_reg->HVSTPMR3 = margin_in_lines;
            break;
#endif
#endif

        default:
            ASSERT(0 == "Illegal channel for top margin!!!\n");
            return FAIL;
    }


    return OK;
}


error_type_t viddrv_program_side_margin(video_handle_t *handle, uint32_t margin_in_pixels, uint32_t sub_margin)
{
    uint32_t chan, facet;
    laser_engine_video_config_t *config = laser_engine_video_config;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    // The mirror controls are subtractive margins, so if we have a sub-pixel
    // margin, we need to add one to the pixel margin and back off the
    // sub-pixel amount.
    if (sub_margin != 0)
    {
      // REVISIT: The right thing to do here is to find out the hardware
      // sub-pixel steps, and convert their sub-margin into this.  See the
      // code changing PTWIDTH for more information.  Right now we don't have
      // their denominator, so are dealing with some absolute sub-margin
      // without any idea what it actually means.  So.... this code is
      // ASSUMING the denominator is 64 (given that it isn't 1 since we have
      // a non-zero submargin value) and the HMIC registers are setup for 0-63.
      // Neither of these are correct in the general case.
      //
      // I would recommend either standardizing on a denominator (e.g. 64), or
      // passing in the denominator if we want to keep it dynamic.
      ASSERT( (sub_margin >= 0) && (sub_margin <= 63) );
      margin_in_pixels += 1;
      sub_margin = 64 - sub_margin;
    }

    SLOG_MSG("%s chan: %d, side: %d, sub margin: %d\n", __func__, chan, margin_in_pixels, sub_margin);
    MSG("%s chan: %d, side: %d, sub margin: %d\n", __func__, chan, margin_in_pixels, sub_margin);

    hipwm_reg[chan]->HSMR = HIPWM_HSMR_SIDEMARGIN_REPLACE_VAL(hipwm_reg[chan]->HSMR, margin_in_pixels);

    // REVISIT: Issues with this sub-pixel margin code:
    //   1. We program the registers based on the number of mirror facets.
    //   2. we're overwriting the mirror facet values, not adding to them.  At the
    //      moment nobody else is setting these so we're ok.  If someone does
    //      set them, we have to make sure we add in our margin only once.
    //      Currently these are not being reset between firmware downloads,
    //      so we could end up with incorrect margins if we blindly added to
    //      the current value.  This also means we should make sure we update
    //      them so they don't have old values.
    //   3. These are only supported in granite, gemstone b0, SS_1000 z1 and b0.  This
    //      code is looking for Granite or Gemstone only.

#if defined(ASIC_granite) || (defined(ASIC_gemstone) && (ASIC_REV >= ASIC_REV_B0))
    // assume facet registers are in sequential order and just increment though
    // BUGBUG: assuming single mirror.  Need to restructure to deal w/ multiple mirrors
    {
        volatile uint32_t* hipwm_reg_ptr = &hipwm_reg[chan]->HMIC0;
        for (facet = 0; facet < config->num_mirror_facets[0]; facet++)
        {
            hipwm_reg_ptr[facet] = HIPWM_HMIC0_MICFRACPIX_REPLACE_VAL(hipwm_reg_ptr[facet], sub_margin);
        }
    }
#endif

    return OK;
}

/* ------- V I D E O  P I X E L  F R E Q U E N C Y  F U N C T I O N S --------*/

// Pixel Frequency Adjust Register functions. Also known as "bender" in the
// hardware documentation. Set when the pixel frequency default register is 
// configured. Used for pixel frequency adjust segment register setup.


// The default pixel frequency register has 19 bits of precision for the 
// fractional part. The pixel frequency increment registers have 26 bits of
// fractional precision, but only 14 significant bits are programmable in the
// acceleration registers. There are 4 bits available for shifting the 14 bits
// to provide 29 total bits of range. The 29 bits consist of 3 bits of whole
// part and 26 bits of fractional part.
#define NUM_BITS_PFA_DEFAULT_FRAC_PRECISION 19
#define NUM_BITS_PFA_INCREMENT_FRAC_PRECISION 26
#define NUM_BITS_PFA_PRECISION 33


/*
 *  The pixel frequency default register contains 6 whole bits and 19 fractional
 *  bits:
 *  
 *    wwwwww.fffffffffffffffffff
 *
 *  The pixel frequency adjust registers contain 14 bits of fractional value, 
 *  with 4 bits of shift value to give a representation of 29 bits of place
 *  value. The place value with respect to the default register is as follows:
 *
 *    0.xxxofffffffffffffffffffffffffffff 
 *
 *  In the above binary fraction, the first three bits are not available, the
 *  next bit is an internal overflow bit, and the following 29 bits are the
 *  available locations for the 14 significant bits. To program the pixel
 *  frequency adjust registers, the location of the most significant bit must
 *  be determined, if it is outside the lower 14 bits, the value must be right
 *  shifted to place the msb in the 13 bit location (13..0).
 */

/* *************** H I P W M  *************** */
#ifdef HAVE_SERENITY
#define HIPWM_HIPS_WIDTH    32u  
#else
#define HIPWM_HIPS_WIDTH    16u
#endif
 
#define HIPS_FREQUENCY      2500000000u  // 2.5 GHz = 2.5 x 10^9 Hz
#define TAPS_FREQ           36000000u
#define NUM_TAPS_MAX        64
#define NUM_TAPS_MIN        32


static uint64_t viddrv_calc_and_normalize_fraction(uint64_t numerator, 
                                                  uint64_t denominator,
                                                  uint32_t numBitsPrecision)
{
    uint32_t bit;
    uint64_t x = denominator>>1; // same as denominator/2;
    uint64_t frac = 0;

    MSG("numerator: %u, denominator: %u, precision: %d\n", (uint32_t)numerator, (uint32_t)denominator, numBitsPrecision);

    while (numBitsPrecision-- > 0)
    {
        bit = 0;
        
        if (numerator > x)
        {
            numerator -=x;
            bit = 1;
        }
      
        x = x>>1; // same as x/2;
        frac = (frac<<1) + bit;
    }

    //MSG("\nFractional part: 0x%x\n", (uint32_t)frac);

    return frac;
}


static uint32_t viddrv_calc_pixel_freq_reg_value(uint32_t pixel_freq_in_hz, uint32_t num_taps)
{
    uint32_t frac_part;
    uint32_t whole_part;
    uint32_t fraction;

    MSG("    number taps %d pix freq %d\n", num_taps, pixel_freq_in_hz);

#ifdef __KERNEL__
    whole_part = (uint32_t)div_u64_rem((HIPWM_HIPS_WIDTH * (uint64_t)pixel_freq_in_hz * num_taps), HIPS_FREQUENCY, &frac_part);
#else
    whole_part = (uint32_t)((HIPWM_HIPS_WIDTH * (uint64_t)pixel_freq_in_hz * num_taps)/HIPS_FREQUENCY);
    frac_part = (uint32_t)((HIPWM_HIPS_WIDTH * (uint64_t)pixel_freq_in_hz * num_taps) % HIPS_FREQUENCY);
#endif
    MSG("    whole_part: %d  frac_part: 0x%x\n", whole_part, frac_part);

    fraction = viddrv_calc_and_normalize_fraction((uint64_t)frac_part, (uint64_t)HIPS_FREQUENCY, NUM_BITS_PFA_DEFAULT_FRAC_PRECISION);
    MSG("    whole_part: %d  fraction: 0x%x\n", whole_part, fraction);

    whole_part = (whole_part << NUM_BITS_PFA_DEFAULT_FRAC_PRECISION) + (uint32_t)fraction;

    MSG("register value: 0x%x\n", whole_part);

    return whole_part;
}

#ifdef BENDER_ERROR_ADJUST
// Calculate the value for the pixel frequency adjust register, given the 
// desired frequency. There are 64 pixel frequency adjust registers.
static uint32_t viddrv_calc_pixel_freq_adjust_reg(int32_t pixelFreq, 
                                                 uint32_t numTaps,
                                                 uint32_t samples_per_segment)
{
    uint32_t reg_value,  shift, i, result;
    uint64_t fraction, frac_part;

    if ( (numTaps != 32) && (numTaps != 64) )
    {
        MSG("\n*** ERROR - number taps not set! *** %d\n\n", numTaps);
    }

    // Set the sign bit in the register
    reg_value = pixelFreq < 0 ? 0x40000 : 0;

    // Because the pixel frequency calculation should ALWAYS be a fraction, the 
    // mod operation to find the remainder of the division can be eliminated.
    // The div and mod operations have caused problems within the Linux kernel.
    // Simplify!
    // frac_part =((((uint64_t)HIPWM_HIPS_WIDTH * (uint64_t)abs(pixelFreq) * (uint64_t)numTaps)) % ((uint64_t)HIPS_FREQUENCY * (uint64_t)samples_per_segment));
    frac_part = (uint64_t)HIPWM_HIPS_WIDTH * (uint64_t)abs(pixelFreq) * (uint64_t)numTaps;

    MSG("MVDO: video_calc_pixel_freq_adjust_reg: pixelFreq: %d, frac_part: %d\n", pixelFreq, (uint32_t)frac_part);

    // Get the full precision fractional value. Note that the registers only
    // provide 14 significant bits.
    fraction = viddrv_calc_and_normalize_fraction(frac_part, (uint64_t)HIPS_FREQUENCY * (uint64_t)samples_per_segment, NUM_BITS_PFA_PRECISION);

    MSG("MVDO: pfa fraction 0x%llx\n", fraction);

    // Now determine shift value - shift the fractional part until out of shift
    // range or until the most signficant bit of the word is 1. The variable
    // "shift" keeps track of how far the fractional part has been left shifted.
    // This becomes the "exponent" for the fractional value.
    shift = 15;
    result = fraction;
    for (i=1; i<=15; i++)
    {
        // dbg_printk((DBG_ANY|LOG_DEBUG),"result: 0x%x shift %d 0x%x\n", result, shift, 1<<(shift+14));
        if (result & (1<<(shift+13)))
        {
            break;
        }
        shift--;
    }
    result = result >> shift;

    MSG("MVDO: shift: %d 0x%x\n", shift, result);


    // Construct the register value. Shift the exponent value, represented by
    // "shift" to the upper part of the word.
    reg_value = reg_value | (shift << 14) | result;

    MSG("MVDO: videoCalculatePixelFreqRegister:  0x%x\n", reg_value);
     
    return reg_value;
}
#endif


uint32_t viddrv_program_pixel_freq_default(video_handle_t *handle,
                                          uint32_t pixel_freq_in_hz) 
{
    uint32_t chan;
    uint32_t reg_value, num_taps;

    if (false == viddrv_is_handle_valid( handle ))
    {
        ASSERT(0);
        return FAIL;
    }

    chan = handle->channel;

    // Disable pixel frequency adjust
    hipwm_reg[chan]->HCR = HIPWM_HCR_BENDEREN_REPLACE_VAL(hipwm_reg[chan]->HCR, 0);

    // Calculate the number of "taps", or input pixel pulse train width
    num_taps = (TAPS_FREQ < pixel_freq_in_hz) ? NUM_TAPS_MIN : NUM_TAPS_MAX;

    reg_value = viddrv_calc_pixel_freq_reg_value(pixel_freq_in_hz, num_taps);

    MSG("MVDO:  chan: %d, number taps: %d, pix freq: %d, register: 0x%x\n", chan, num_taps, pixel_freq_in_hz, reg_value);

    // Finally write the register value to hardware. Only set the left side
    // "bender" default register. 
    hipwm_reg[chan]->HLBDR = reg_value;
   
    MSG("MVDO: register value: 0x%x actual: 0x%x\n\n", reg_value, hipwm_reg[chan]->HLBDR);

    // Set the Pulse Train bit width in the HiPWM configuration register.
    // If NUM_TAPS is 64, set the bit to 0, otherwise set the bit to 1.
    if (NUM_TAPS_MIN == num_taps)
    {
        hipwm_reg[chan]->HCR = HIPWM_HCR_PTWIDTH_REPLACE_VAL(hipwm_reg[chan]->HCR, 1);
    }
    else
    {
        hipwm_reg[chan]->HCR = HIPWM_HCR_PTWIDTH_REPLACE_VAL(hipwm_reg[chan]->HCR, 0);
    }

    return num_taps;
}

 
#ifdef BENDER_ERROR_ADJUST
void viddrv_program_pixel_freq_adjust_registers(linear_error_t *p_linear_error)
{
    int32_t error;
    uint32_t i;
    uint32_t chan = p_linear_error->chan;
    uint32_t scan_correction_time, 
             scan_correction_time_in_picosec,
             hipwm_samples_per_segment,
             time,
             reference_freq;

    linear_error_array_int_t my_array;

    for (i=0; i<NUM_X_POINTS; i++)
    {
        my_array[i].x = p_linear_error->error_array[i].x;
        my_array[i].y = p_linear_error->error_array[i].y;
        MSG("MVDO: linearity error array values %d: %d %d\n", i, (uint32_t)my_array[i].x, (uint32_t)my_array[i].y);
    }

   //  printk("scan time total: %d\n", (uint32_t)p_linear_error->error_array[NUM_X_POINTS-1].x);

    // Correct for the entire time of the scan
    scan_correction_time = (uint32_t)p_linear_error->error_array[NUM_X_POINTS-1].x;
    MSG("MVDO: scan correction time %d\n\n", scan_correction_time);

    // Calculate the HiPWM samples per segment 
    scan_correction_time_in_picosec = scan_correction_time * 1000;
    hipwm_samples_per_segment = scan_correction_time_in_picosec/(HIPWM_CLOCK_CYCLE_IN_PICOSEC *  NUM_PIXEL_FREQ_ADJUST_REGS);
    MSG("MVDO: samples per segment %d\n", hipwm_samples_per_segment);

    // Program configuration register with samples per segment value. Program
    // with one less than desired value per documentation.
    pHIPWM_REGS[chan]->HBCR = hipwm_samples_per_segment - 1; 

    // Enable the CPU access bit to allow the registers to be written.
    REG_SET_BITS((uint32_t *)&pHIPWM_REGS[chan]->HCR,  HIPWM_HCR_CPUACCESS_MASK);

    // Reference frequency is used to calculate the frequency error correction
    // for the pixel frequency adjust register. The value for the adjust
    // register is based on the frequency at the end of the last segment; i.e.
    // the "reference" frequency. All the values in the error array are 
    // referenced to the default pixel frequency, so the reference for the 
    // initial calculation is zero.
    reference_freq = 0;

    for (i=0; i<NUM_PIXEL_FREQ_ADJUST_REGS; i++)
    {
        uint32_t reg_value,
                 freq;
       
        time = (i+1)*scan_correction_time/NUM_PIXEL_FREQ_ADJUST_REGS;
        freq = viddrv_interpolate_int_error(&my_array, time);
        error = freq - reference_freq;

        // New reference frequency is the frequency calculated for the end of
        // pixel frequency adjust segment.
        reference_freq = freq;

        MSG("MVDO: time %2d: %8d, freq: %8u, error: %8d\n", i, time, freq, error);

        reg_value = viddrv_calc_pixel_freq_adjust_reg(error, 
                                                     g_vid_num_taps[chan], 
                                                     hipwm_samples_per_segment);

        // Set the "last" bit 
        if (( NUM_PIXEL_FREQ_ADJUST_REGS-1) == i)
        {
            reg_value |= HIPWM_HBAT0_BATLAST_MASK;
        }

        // REVISIT DAB set the register value!
        *(uint32_t *)(&(pHIPWM_REGS[chan]->HBAT0) + i) = reg_value;

        MSG("MVDO: addr: 0x%x, value: 0x%x, actual: 0x%x\n\n", (uint32_t)(&(pHIPWM_REGS[chan]->HBAT0) + i)), 
               reg_value, *(uint32_t *)(&(pHIPWM_REGS[chan]->HBAT0) + i) ;
    }

    // Disable the CPU access bit to allow the registers to be written.
    REG_CLEAR_BITS(&pHIPWM_REGS[chan]->HCR,  HIPWM_HCR_CPUACCESS_MASK);

    // Enable pixel frequency adjust
    REG_SET_BITS(&pHIPWM_REGS[chan]->HCR, HIPWM_HCR_BENDEREN_MASK);

    MSG("MVDO: HCR register 0x%x\n", *(uint32_t *)&pHIPWM_REGS[chan]->HCR);
    // Enable bender
}
#endif

#ifdef BENDER_TEST
void viddrv_test_pixel_freq_adjust(int32_t freq_in_hz)
{
    int32_t error;
    uint32_t i;
    uint32_t chan = 0;
    uint32_t reference_freq;

    // Calculate the HiPWM samples per segment 
    MSG("MVDO: samples per segment %d\n", 500);

    // Program configuration register with samples per segment value. Program
    // with one less than desired value per documentation.
    pHIPWM_REGS[chan]->HBCR = 500 - 1; 

    // Enable the CPU access bit to allow the registers to be written.
    REG_SET_BITS((uint32_t *)&pHIPWM_REGS[chan]->HCR,  HIPWM_HCR_CPUACCESS_MASK);

    // Reference frequency is used to calculate the frequency error correction
    // for the pixel frequency adjust register. The value for the adjust
    // register is based on the frequency at the end of the last segment; i.e.
    // the "reference" frequency. All the values in the error array are 
    // referenced to the default pixel frequency, so the reference for the 
    // initial calculation is zero.
    reference_freq = 0;

    for (i=0; i<NUM_PIXEL_FREQ_ADJUST_REGS; i++)
    {
        uint32_t reg_value;
                
        error = freq_in_hz;

        // New reference frequency is the frequency calculated for the end of
        // pixel frequency adjust segment.
        reference_freq = 0;


        reg_value = viddrv_calc_pixel_freq_adjust_reg(error, g_vid_num_taps[chan], 500);

        // Set the "last" bit 
        if (( NUM_PIXEL_FREQ_ADJUST_REGS-1) == i)
        {
            reg_value |= HIPWM_HBAT0_BATLAST_MASK;
        }

        // REVISIT DAB set the register value!
        *(uint32_t *)(&(pHIPWM_REGS[chan]->HBAT0) + i) = reg_value;

        dbg_printk((DBG_ANY|LOG_DEBUG),"MVDO: addr: 0x%x, value: 0x%x, actual: 0x%x\n\n", (uint32_t)(&(pHIPWM_REGS[chan]->HBAT0) + i), 
               reg_value, *(uint32_t *)(&(pHIPWM_REGS[chan]->HBAT0) + i) );
    }

    // Disable the CPU access bit to allow the registers to be written.
    REG_CLEAR_BITS(&pHIPWM_REGS[chan]->HCR,  HIPWM_HCR_CPUACCESS_MASK);

    // Enable pixel frequency adjust
    REG_SET_BITS(&pHIPWM_REGS[chan]->HCR, HIPWM_HCR_BENDEREN_MASK);

    dbg_printk((DBG_ANY|LOG_DEBUG),"MVDO: HCR register 0x%x\n", *(uint32_t *)&pHIPWM_REGS[chan]->HCR);
    
}
#endif


// REVISIT should this count pixels for a given color? Or require the app
// to do that? All driver function access is by handle - so app should know 
// what channels comprise a color. Keep track of that in the config layer!!!
// And how many beams per print plane (color)!!!

/* FUNCTION NAME: video_query_pixel_count */

/** 
 * \brief Calculate pixel counts for this channel as specified by the handle.
 * 
 * \param
 * 
 * \retval None
 * 
 **/
error_type_t viddrv_query_pixel_count( video_handle_t *handle, 
                                      uint32_t *non_white_pixels,
                                      uint32_t *weighted_pixels )
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;
    
    // count the off and on pixels for pixel counting. 
    non_white_pixels[chan] = video_reg[chan]->VTCP; 
    if(weighted_pixels != NULL) // if not used this value, pass it.
        *weighted_pixels = video_reg[chan]->VTCA;
    #if 0
    // for dual beam engine add in the other beam
    // if(DUAL_BEAM == myVideoRegs.numLaserBeams)
    // REVISIT - assume that if the beam isn't used the value will be zero!
    if ( myVideoConfigVals->numLaserBeamsPerPrintPlane == DUAL_BEAM )
    {
        nonWhitePixels += *SVDO_VTCP_REG(1) ;  //on pixel count is
        weightedPixels += *SVDO_VTCA_REG(1);
    }   

    // weighted pixel count must be further normalized by 4 to account for 64 pulse levels
    weightedPixels >>= 2;

    return;

    #endif

    return OK;
}
/* -------------------------------------------------------------------------- */
/*      D E F A U L T  P U L S E  J U S T I F I C A T I O N  T A B L E S      */
/* -------------------------------------------------------------------------- */

#define CENTER_JUST 0
#define EARLY_JUST  2
#define LATE_JUST  1
#define SPLIT_JUST  3


// JustIndex = {LeftPix[3:2],CenterPix[3:0],RightPix[3:2]}
error_type_t viddrv_set_centered_justification(video_handle_t *handle, uint32_t bpp)
{
    uint32_t reg_num;
    uint32_t justification[NUM_JUSTIFY_REGS];

    if ( false == viddrv_is_handle_valid(handle) )
        return FAIL;

    for (reg_num = 0; reg_num < NUM_JUSTIFY_REGS; reg_num++)
        justification[reg_num] = 0;

    viddrv_set_pulse_justification_regs(handle, justification);

    MSG("center justification\n");
    return OK;
}


error_type_t viddrv_set_auto_justification(video_handle_t *handle, uint32_t bpp)
{
    uint32_t l,c,r;
    uint32_t reg_num, bit_loc, value;
    uint32_t nvalues = 1 << bpp;
    uint32_t justification[NUM_JUSTIFY_REGS];

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    for (reg_num = 0; reg_num < NUM_JUSTIFY_REGS; reg_num++)
    {
        justification[reg_num] = 0;
    }

    // Loop through all values for left, center and right pixels.
    // TODO: This has not been tested for 1-bpp and 4-bpp.
    for (c = 0; c < nvalues; c++)
    {
        value = 0;

        for (l = 0; l < nvalues; l++)
        {
            for (r = 0; r < nvalues; r++)
            {
                // Center pixel determines which justification register. For 
                // 2 bpp only one fourth of the registers will be filled.
                reg_num = c << (4-bpp);

                // Location in the register of the justification value is 
                // determined by the left and right pixels.
                bit_loc = (l<<2 | r)<<1;

//-jrs                MSG("index 0x%x %d %d %d ", bit_loc, l, c, r);
                if ( (l>c) && (c>=r) )
                {
                    // early justify
                    value = value | EARLY_JUST<<bit_loc;
//-jrs                    MSG("early \n");
                }
                else if ( (l<=c) && (c<r) )
                {
                    // late justify
                    value = value | LATE_JUST<<bit_loc;
//-jrs                    MSG("late\n");
                }
                else if ( (l>c) && (c<r) )
                {
                    // split justfiy
                    value = value | SPLIT_JUST<<bit_loc;
//-jrs                    MSG(" split\n");
                }
                else if ( (l<=c) && (c>=r) )
                {
                    if (l>r)
                    {
                        // early justify
                        value = value | EARLY_JUST<<bit_loc;
//-jrs                        MSG("early \n");
                    }
                    else if (r>l)
                    {
                        // late justify
                        value = value | LATE_JUST<<bit_loc;
//-jrs                        MSG("late\n");
                    }
                    else
                    {
                        // center justify - leave table at default value
//-jrs                        MSG("center\n");
                    }
                }
                else
                {
                    MSG("UNHANDLED CASE! %d %d %d\n", l,c,r);
                }
            } // right
        } // left

        justification[reg_num] = value;
       
        MSG("wrote 0x%x to register %d\n", value, reg_num);
    } // center

    viddrv_set_pulse_justification_regs(handle, justification);

    MSG("auto justification\n");
    return OK;
}


error_type_t viddrv_clear_forced_justification( video_handle_t *handle )
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    #if defined(ASIC_gemstone) && (ASIC_REV < ASIC_REV_B0)
    ASSERT(0);
    #else
    video_reg[chan]->SCFG = SVDO_SCFG_FORCEDJUST_REPLACE_VAL( video_reg[chan]->SCFG, 0 );
    #endif

    return OK;
}


error_type_t viddrv_set_forced_justification( video_handle_t *handle, 
                                             uint32_t init_just, 
                                             uint32_t toggle_just )
{
    uint32_t chan;

    if (false == viddrv_is_handle_valid( handle ))
    {
        return FAIL;
    }

    chan = handle->channel;

    // This hardware is supported in SS100 Z1, B0; Gemstone B0; and Granite C0
    #if defined(ASIC_gemstone) && (ASIC_REV < ASIC_REV_B0)
    ASSERT(0);
    #else
    video_reg[chan]->SCFG = SVDO_SCFG_FORCEDJUST_REPLACE_VAL(video_reg[chan]->SCFG, 1 );
    video_reg[chan]->SCFG = SVDO_SCFG_INITJUST_REPLACE_VAL(video_reg[chan]->SCFG, init_just );
    video_reg[chan]->SCFG = SVDO_SCFG_TOGGLEJUST_REPLACE_VAL(video_reg[chan]->SCFG, toggle_just );
    #endif

    MSG("paired-mode justification\n");
    return OK;
}


/* -------------------------------------------------------------------------- */
/*                      P R I V A T E  F U N C T I O N S                      */
/* -------------------------------------------------------------------------- */


// Must be called after at least one transaction is configured. This allows for
// multiple transactions to be chained together before the DMA is started.
static void viddrv_write_start_descriptor_to_hardware( video_desc_t *descriptor,
                                                      uint32_t chan)
{
    SLOG_MSG("start new transfer chan: %d, desc: 0x%x, config 0x%x\n", chan, (uint32_t)descriptor, (uint32_t)descriptor->dma_config);

    if (descriptor->dma_config & START_OF_IMAGE_BIT)
    {
        video_handle[chan].page_in_progress = true;
    }

    // Write the descriptor write register to start the DMA
    SLOG_MSG("set VDWR=0x%x\n", (uint32_t)descriptor->hw_address);
    video_reg[chan]->VDWR = (uint32_t)descriptor->hw_address; 
    
}

// LSPTODO: CLEAN UP WHERE APB CONFIG HAPPENS
static void viddrv_init_hardware( void )
{
    APB_CONFIG_REGS_t *apbconfig = NULL;
    hips_pll_device_t *hips_handle;
    uint32_t chan;
    error_type_t status;
    laser_engine_video_config_t *config = laser_engine_video_config;

    apbconfig = video_config->apb_config_base_addr;

 #if defined(ASIC_gemstone)
    apbconfig->PINCR = APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL( apbconfig->PINCR, 0xF );
    apbconfig->PINCR = APB_CONFIG_PINCR_PENLVDSSEL_REPLACE_VAL( apbconfig->PINCR, 0x0 );
    //apbconfig->PLL0_CR1 = APB_CONFIG_PLL0_CR1_SEL_VCO_CLK_SE_REPLACE_VAL( apbconfig->PLL0_CR1, 0x1 );
    apbconfig->PDCR1 = APB_CONFIG_PDCR1_LVDSSR_REPLACE_VAL(apbconfig->PDCR1, 0x3c);      // fast slew for VDO outputs
    apbconfig->PWCR = APB_CONFIG_PWCR_LVDS_PAD_PDWN_REPLACE_VAL(apbconfig->PWCR, 0x0 );  // power up VDO outputs
 #elif defined(ASIC_granite)
    apbconfig->PINCR = APB_CONFIG_PINCR_LVDO_OE_REPLACE_VAL( apbconfig->PINCR, 0xFF );
    apbconfig->PINCR = APB_CONFIG_PINCR_LVDS_PENNVID_SEL_REPLACE_VAL( apbconfig->PINCR, 0x0 );
    //apbconfig->PLL0_CR1 = APB_CONFIG_PLL0_CR1_SEL_VCO_CLK_SE_REPLACE_VAL( apbconfig->PLL0_CR1, 0x1 );
    apbconfig->PDCR1 = APB_CONFIG_PDCR1_OLVDSSR_REPLACE_VAL(apbconfig->PDCR1, 0x1FF); // fast slew for VDO outputs
    apbconfig->PDCR2 = APB_CONFIG_PDCR2_ILVDS_PD_REPLACE_VAL(apbconfig->PDCR2, 0x0);  // power up VDO outputs
 #else
 #error "Unknown ASIC"
 #endif

    // The video configuration should have the information that is required to
    // configure and route the signals for top of page and horizontal sync
    // detect.
    viddrv_program_output_select_registers();

    viddrv_program_top_select_registers();

    viddrv_set_hsync_lockout(config->hsync_lockout_period_in_hipwm_clocks);

    viddrv_set_hsync_mirrors(config);

    // Enable the Horizontal/Vertical sync block and leave it enabled. It should
    // never be necessary to disable this hardware block.
    hvsync_reg->CFG = HVSYNC_CFG_HVSYNCENABLE_REPLACE_VAL(hvsync_reg->CFG, 1);

    // Enable all the HIPS channels
    for (chan=0; chan < NUM_VIDEO_CHAN; chan++) // REVISIT DAB -jrs do we only want to enable the ones marked as active?
    {
        status = hips_pll_open(&hips_handle, chan);
        //ASSERT(status == OK);
        hipwm_reg[chan]->HCR = HIPWM_HCR_ENABLE_REPLACE_VAL(hipwm_reg[chan]->HCR, 1);
    }

    if(laser_engine_video_config && laser_engine_video_config->power_down_lvdo_between_pages)
    {
        // don't power up the pads until we get a beam detect.
        pwr_down_lvdo();
    }

    //MSG("*** HIPS reg vals 0x%x 0x%x 0x%x\n", hips_reg->Chan_Enable, hips_reg->FWBIST_Control, hips_reg->FWBIST_Diagnostic_Table0);
}



// Expected to be called only when video channel is opened.
static void viddrv_apply_configuration( video_handle_t *handle, bool top_to_bottom )
{
    uint32_t chan;
    uint32_t tmp;
    laser_engine_video_config_t *config = laser_engine_video_config;

    if (handle == NULL)
    {
    ASSERT(handle != NULL);
    }

    chan = handle->channel;

    MSG("apply configuration for channel %d\n", chan);

    // Set the default pixel frequency.
    viddrv_program_pixel_freq_default(handle, config->pixel_freq_in_hz[chan]);

    // Setup the video burst size for DMA.
    switch (config->burst_size)
    {
    case BYTES_16:
        tmp=0;
        break;

    case BYTES_32:
        tmp = 1;
        break;

    case BYTES_64:
        tmp = 2;
        break;

    default:
        tmp=0;
        break;

    }
    video_reg[chan]->SCFG = SVDO_SCFG_BURSTSZ_REPLACE_VAL(video_reg[chan]->SCFG, tmp);
 
    // Setup the serpentine value
    video_reg[chan]->SCFG = SVDO_SCFG_SERP_REPLACE_VAL(video_reg[chan]->SCFG, config->serpentine);

    // per page 180 rotation for backside of longedge duplex, false is Bottom_to_Top
    handle->top_to_bottom = top_to_bottom;  // Save this since each strip needs to know about the rotation.
    video_reg[chan]->SCFG = SVDO_SCFG_BOT2TOP_REPLACE_VAL(video_reg[chan]->SCFG, top_to_bottom ? 0 : 1 );


    // Setup channel data source value
    video_reg[chan]->SCFG = SVDO_SCFG_DATASOURCE_REPLACE_VAL(video_reg[chan]->SCFG, 0);
 
    // Setup the video invert value
    hipwm_reg[chan]->HCR = HIPWM_HCR_INVPT_REPLACE_VAL(hipwm_reg[chan]->HCR, config->invert_video);

    viddrv_set_line_skip( handle, config->num_beams_per_print_plane-1);
}


static void viddrv_notify_event( uint32_t event_code, uint32_t chan )
{
    DBG_SLOG_ENTRY

    if(!viddrv_is_handle_valid(&video_handle[chan]) || !video_handle[chan].open)
    {
        ERR("%s: invalid or closed channel. chan = %d, event = %#x\n", __func__, chan, event_code);
    }

    if(video_handle[chan].event_callback != NULL)
    {
        video_handle[chan].event_callback(&video_handle[chan], event_code, video_handle[chan].event_context);
    }

    DBG_SLOG_EXIT
}

void dump_video_desc(video_desc_t* desc)
{
    SLOG_ERR("%s:\n", __func__);
    SLOG_ERR("\tdma_config           = %#x\n", desc->dma_config);
    SLOG_ERR("\tdma_src_data_address = %#x\n", desc->dma_src_data_address);
    SLOG_ERR("\tdma_line_count       = %d\n", desc->dma_line_count);
    SLOG_ERR("\tdma_next_address     = %#x\n", desc->dma_next_address);
    SLOG_ERR("\thw_address           = %#x\n", (uint32_t)desc->hw_address);
    SLOG_ERR("\tuser_data            = %#x\n", (uint32_t)desc->user_data);
}

void viddrv_dump_svd0_reg(uint32_t reg_bank_num)
{
    SVDO_REGS_t *reg = video_reg[reg_bank_num];
    dbg_printf("\nvideo bank %d, base addr: 0x%x\n", reg_bank_num, (uint32_t)reg);
    dbg_printf("\t SCFG     \t: 0x%08x \n", reg->SCFG);
    dbg_printf("\t STAT     \t: 0x%08x \n", reg->STAT);
    dbg_printf("\t VDWR     \t: 0x%08x \n", reg->VDWR);
    dbg_printf("\t VDAR     \t: 0x%08x \n", reg->VDAR);
    dbg_printf("\t VDBR     \t: 0x%08x \n", reg->VDBR);
    dbg_printf("\t VDLR     \t: 0x%08x \n", reg->VDLR);
    dbg_printf("\t VDNR     \t: 0x%08x \n", reg->VDNR);
    dbg_printf("\t VIER     \t: 0x%08x \n", reg->VIER);
    dbg_printf("\t VIPR     \t: 0x%08x \n", reg->VIPR);
    dbg_printf("\t VIAR     \t: 0x%08x \n", reg->VIAR);
    dbg_printf("\t VIFR     \t: 0x%08x \n", reg->VIFR);
    dbg_printf("\t VLWR     \t: 0x%08x \n", reg->VLWR);
    dbg_printf("\t VTCA     \t: 0x%08x \n", reg->VTCA);
    dbg_printf("\t VTCP     \t: 0x%08x \n", reg->VTCP);
    dbg_printf("\t VSAR     \t: 0x%08x \n", reg->VSAR);
    dbg_printf("\t VPKR     \t: 0x%08x \n", reg->VPKR);
    dbg_printf("\t JUST0    \t: 0x%08x \n", reg->JUST0);
    dbg_printf("\t JUST1    \t: 0x%08x \n", reg->JUST1);
    dbg_printf("\t JUST2    \t: 0x%08x \n", reg->JUST2);
    dbg_printf("\t JUST3    \t: 0x%08x \n", reg->JUST3);
    dbg_printf("\t JUST4    \t: 0x%08x \n", reg->JUST4);
    dbg_printf("\t JUST5    \t: 0x%08x \n", reg->JUST5);
    dbg_printf("\t JUST6    \t: 0x%08x \n", reg->JUST6);
    dbg_printf("\t JUST7    \t: 0x%08x \n", reg->JUST7);
    dbg_printf("\t JUST8    \t: 0x%08x \n", reg->JUST8);
    dbg_printf("\t JUST9    \t: 0x%08x \n", reg->JUST9);
    dbg_printf("\t JUST10   \t: 0x%08x \n", reg->JUST10);
    dbg_printf("\t JUST11   \t: 0x%08x \n", reg->JUST11);
    dbg_printf("\t JUST12   \t: 0x%08x \n", reg->JUST12);
    dbg_printf("\t JUST13   \t: 0x%08x \n", reg->JUST13);
    dbg_printf("\t JUST14   \t: 0x%08x \n", reg->JUST14);
    dbg_printf("\t JUST15   \t: 0x%08x \n", reg->JUST15);
    dbg_printf("\t PCODE0   \t: 0x%08x \n", reg->PCODE0);
    dbg_printf("\t PCODE1   \t: 0x%08x \n", reg->PCODE1);
    dbg_printf("\t PCODE2   \t: 0x%08x \n", reg->PCODE2);
    dbg_printf("\t PCODE3   \t: 0x%08x \n", reg->PCODE3);
    dbg_printf("\t PCODE4   \t: 0x%08x \n", reg->PCODE4);
    dbg_printf("\t PCODE5   \t: 0x%08x \n", reg->PCODE5);
    dbg_printf("\t PCODE6   \t: 0x%08x \n", reg->PCODE6);
    dbg_printf("\t PCODE7   \t: 0x%08x \n", reg->PCODE7);
    dbg_printf("\t PCODE8   \t: 0x%08x \n", reg->PCODE8);
    dbg_printf("\t PCODE9   \t: 0x%08x \n", reg->PCODE9);
    dbg_printf("\t PCODE10  \t: 0x%08x \n", reg->PCODE10);
    dbg_printf("\t PCODE11  \t: 0x%08x \n", reg->PCODE11);
    dbg_printf("\t PCODE12  \t: 0x%08x \n", reg->PCODE12);
    dbg_printf("\t PCODE13  \t: 0x%08x \n", reg->PCODE13);
    dbg_printf("\t PCODE14  \t: 0x%08x \n", reg->PCODE14);
    dbg_printf("\t PCODE15  \t: 0x%08x \n", reg->PCODE15);
}

void viddrv_dump_hipwm_reg(uint32_t reg_bank_num)
{
    HIPWM_REGS_t *reg = hipwm_reg[reg_bank_num];
    dbg_printf("\nhipwm bank %d, base addr: 0x%x\n", reg_bank_num, (uint32_t)reg);
    dbg_printf("\t HCR      \t: 0x%08x \n", reg->HCR);
    dbg_printf("\t HSMR     \t: 0x%08x \n", reg->HSMR);
    dbg_printf("\t HTSR     \t: 0x%08x \n", reg->HTSR);
    dbg_printf("\t HSR      \t: 0x%08x \n", reg->HSR);
    dbg_printf("\t HSPWR    \t: 0x%08x \n", reg->HSPWR);
    dbg_printf("\t HSQR     \t: 0x%08x \n", reg->HSQR);
    dbg_printf("\t PRRA     \t: 0x%08x \n", reg->PRRA);
    dbg_printf("\t HPPVR    \t: 0x%08x \n", reg->HPPVR);
    dbg_printf("\t HPSR     \t: 0x%08x \n", reg->HPSR);
    dbg_printf("\t HPCR     \t: 0x%08x \n", reg->HPCR);
    dbg_printf("\t HPPSR    \t: 0x%08x \n", reg->HPPSR);
    dbg_printf("\t HPLSR    \t: 0x%08x \n", reg->HPLSR);
    dbg_printf("\t HPPOER   \t: 0x%08x \n", reg->HPPOER);
    dbg_printf("\t HPPOOR   \t: 0x%08x \n", reg->HPPOOR);
    dbg_printf("\t HPPLOR   \t: 0x%08x \n", reg->HPPLOR);
    dbg_printf("\t HCRC     \t: 0x%08x \n", reg->HCRC);
    dbg_printf("\t HBCR     \t: 0x%08x \n", reg->HBCR);
    dbg_printf("\t HLBDR    \t: 0x%08x \n", reg->HLBDR);
    dbg_printf("\t HRBDR    \t: 0x%08x \n", reg->HRBDR);
    dbg_printf("\t HBNCR    \t: 0x%08x \n", reg->HBNCR);
    dbg_printf("\t HBNINIT  \t: 0x%08x \n", reg->HBNINIT);
    dbg_printf("\t HBNCV    \t: 0x%08x \n", reg->HBNCV);
    dbg_printf("\t HBAT0    \t: 0x%08x \n", reg->HBAT0);
    dbg_printf("\t HBAT1    \t: 0x%08x \n", reg->HBAT1);
    dbg_printf("\t HBAT2    \t: 0x%08x \n", reg->HBAT2);
    dbg_printf("\t HBAT3    \t: 0x%08x \n", reg->HBAT3);
    dbg_printf("\t HBAT4    \t: 0x%08x \n", reg->HBAT4);
    dbg_printf("\t HBAT5    \t: 0x%08x \n", reg->HBAT5);
    dbg_printf("\t HBAT6    \t: 0x%08x \n", reg->HBAT6);
    dbg_printf("\t HBAT7    \t: 0x%08x \n", reg->HBAT7);
    dbg_printf("\t HBAT8    \t: 0x%08x \n", reg->HBAT8);
    dbg_printf("\t HBAT9    \t: 0x%08x \n", reg->HBAT9);
    dbg_printf("\t HBAT10   \t: 0x%08x \n", reg->HBAT10);
    dbg_printf("\t HBAT11   \t: 0x%08x \n", reg->HBAT11);
    dbg_printf("\t HBAT12   \t: 0x%08x \n", reg->HBAT12);
    dbg_printf("\t HBAT13   \t: 0x%08x \n", reg->HBAT13);
    dbg_printf("\t HBAT14   \t: 0x%08x \n", reg->HBAT14);
    dbg_printf("\t HBAT15   \t: 0x%08x \n", reg->HBAT15);
    dbg_printf("\t HBAT16   \t: 0x%08x \n", reg->HBAT16);
    dbg_printf("\t HBAT17   \t: 0x%08x \n", reg->HBAT17);
    dbg_printf("\t HBAT18   \t: 0x%08x \n", reg->HBAT18);
    dbg_printf("\t HBAT19   \t: 0x%08x \n", reg->HBAT19);
    dbg_printf("\t HBAT20   \t: 0x%08x \n", reg->HBAT20);
    dbg_printf("\t HBAT21   \t: 0x%08x \n", reg->HBAT21);
    dbg_printf("\t HBAT22   \t: 0x%08x \n", reg->HBAT22);
    dbg_printf("\t HBAT23   \t: 0x%08x \n", reg->HBAT23);
    dbg_printf("\t HBAT24   \t: 0x%08x \n", reg->HBAT24);
    dbg_printf("\t HBAT25   \t: 0x%08x \n", reg->HBAT25);
    dbg_printf("\t HBAT26   \t: 0x%08x \n", reg->HBAT26);
    dbg_printf("\t HBAT27   \t: 0x%08x \n", reg->HBAT27);
    dbg_printf("\t HBAT28   \t: 0x%08x \n", reg->HBAT28);
    dbg_printf("\t HBAT29   \t: 0x%08x \n", reg->HBAT29);
    dbg_printf("\t HBAT30   \t: 0x%08x \n", reg->HBAT30);
    dbg_printf("\t HBAT31   \t: 0x%08x \n", reg->HBAT31);
    dbg_printf("\t HBAT32   \t: 0x%08x \n", reg->HBAT32);
    dbg_printf("\t HBAT33   \t: 0x%08x \n", reg->HBAT33);
    dbg_printf("\t HBAT34   \t: 0x%08x \n", reg->HBAT34);
    dbg_printf("\t HBAT35   \t: 0x%08x \n", reg->HBAT35);
    dbg_printf("\t HBAT36   \t: 0x%08x \n", reg->HBAT36);
    dbg_printf("\t HBAT37   \t: 0x%08x \n", reg->HBAT37);
    dbg_printf("\t HBAT38   \t: 0x%08x \n", reg->HBAT38);
    dbg_printf("\t HBAT39   \t: 0x%08x \n", reg->HBAT39);
    dbg_printf("\t HBAT40   \t: 0x%08x \n", reg->HBAT40);
    dbg_printf("\t HBAT41   \t: 0x%08x \n", reg->HBAT41);
    dbg_printf("\t HBAT42   \t: 0x%08x \n", reg->HBAT42);
    dbg_printf("\t HBAT43   \t: 0x%08x \n", reg->HBAT43);
    dbg_printf("\t HBAT44   \t: 0x%08x \n", reg->HBAT44);
    dbg_printf("\t HBAT45   \t: 0x%08x \n", reg->HBAT45);
    dbg_printf("\t HBAT46   \t: 0x%08x \n", reg->HBAT46);
    dbg_printf("\t HBAT47   \t: 0x%08x \n", reg->HBAT47);
    dbg_printf("\t HBAT48   \t: 0x%08x \n", reg->HBAT48);
    dbg_printf("\t HBAT49   \t: 0x%08x \n", reg->HBAT49);
    dbg_printf("\t HBAT50   \t: 0x%08x \n", reg->HBAT50);
    dbg_printf("\t HBAT51   \t: 0x%08x \n", reg->HBAT51);
    dbg_printf("\t HBAT52   \t: 0x%08x \n", reg->HBAT52);
    dbg_printf("\t HBAT53   \t: 0x%08x \n", reg->HBAT53);
    dbg_printf("\t HBAT54   \t: 0x%08x \n", reg->HBAT54);
    dbg_printf("\t HBAT55   \t: 0x%08x \n", reg->HBAT55);
    dbg_printf("\t HBAT56   \t: 0x%08x \n", reg->HBAT56);
    dbg_printf("\t HBAT57   \t: 0x%08x \n", reg->HBAT57);
    dbg_printf("\t HBAT58   \t: 0x%08x \n", reg->HBAT58);
    dbg_printf("\t HBAT59   \t: 0x%08x \n", reg->HBAT59);
    dbg_printf("\t HBAT60   \t: 0x%08x \n", reg->HBAT60);
    dbg_printf("\t HBAT61   \t: 0x%08x \n", reg->HBAT61);
    dbg_printf("\t HBAT62   \t: 0x%08x \n", reg->HBAT62);
    dbg_printf("\t HBAT63   \t: 0x%08x \n", reg->HBAT63);
    dbg_printf("\t HMIC0    \t: 0x%08x \n", reg->HMIC0);
    dbg_printf("\t HMIC1    \t: 0x%08x \n", reg->HMIC1);
    dbg_printf("\t HMIC2    \t: 0x%08x \n", reg->HMIC2);
    dbg_printf("\t HMIC3    \t: 0x%08x \n", reg->HMIC3);
    dbg_printf("\t HMIC4    \t: 0x%08x \n", reg->HMIC4);
    dbg_printf("\t HMIC5    \t: 0x%08x \n", reg->HMIC5);
    dbg_printf("\t HMIC6    \t: 0x%08x \n", reg->HMIC6);
    dbg_printf("\t HMIC7    \t: 0x%08x \n", reg->HMIC7);
}


void viddrv_dump_hvsync_reg()
{
    HVSYNC_REGS_t *reg = hvsync_reg;
    dbg_printf("\nhvsync base addr: 0x%x\n", (uint32_t)reg);
    dbg_printf("\t CFG      \t: 0x%08x \n", reg->CFG);
    dbg_printf("\t MIR_CFG0 \t: 0x%08x \n", reg->MIR_CFG0);
    dbg_printf("\t MIR_CFG1 \t: 0x%08x \n", reg->MIR_CFG1);
    dbg_printf("\t MIR_STA0 \t: 0x%08x \n", reg->MIR_STA0);
    dbg_printf("\t MIR_STA1 \t: 0x%08x \n", reg->MIR_STA1);
    dbg_printf("\t FT_RST0  \t: 0x%08x \n", reg->FT_RST0);
    dbg_printf("\t FT_RST1  \t: 0x%08x \n", reg->FT_RST1);
    dbg_printf("\t OUT_SEL0 \t: 0x%08x \n", reg->OUT_SEL0);
    dbg_printf("\t OUT_SEL1 \t: 0x%08x \n", reg->OUT_SEL1);
    dbg_printf("\t OUT_SEL2 \t: 0x%08x \n", reg->OUT_SEL2);
    dbg_printf("\t OUT_SEL3 \t: 0x%08x \n", reg->OUT_SEL3);
#if (NUM_VIDEO_CHAN == 8)
    dbg_printf("\t OUT_SEL4 \t: 0x%08x \n", reg->OUT_SEL4);
    dbg_printf("\t OUT_SEL5 \t: 0x%08x \n", reg->OUT_SEL5);
    dbg_printf("\t OUT_SEL6 \t: 0x%08x \n", reg->OUT_SEL6);
    dbg_printf("\t OUT_SEL7 \t: 0x%08x \n", reg->OUT_SEL7);
#endif
    dbg_printf("\t TQSTAT   \t: 0x%08x \n", reg->TQSTAT);
    dbg_printf("\t TQSEL    \t: 0x%08x \n", reg->TQSEL);
    dbg_printf("\t HVSIER   \t: 0x%08x \n", reg->HVSIER);
    dbg_printf("\t HVSIPR   \t: 0x%08x \n", reg->HVSIPR);
    dbg_printf("\t HVSIAR   \t: 0x%08x \n", reg->HVSIAR);
    dbg_printf("\t HVSIFR   \t: 0x%08x \n", reg->HVSIFR);
    dbg_printf("\t HVSTPMR0 \t: 0x%08x \n", reg->HVSTPMR0);
    dbg_printf("\t HVSTPMR1 \t: 0x%08x \n", reg->HVSTPMR1);
    dbg_printf("\t HVSTPMR2 \t: 0x%08x \n", reg->HVSTPMR2);
    dbg_printf("\t HVSTPMR3 \t: 0x%08x \n", reg->HVSTPMR3);
    dbg_printf("\t HFCR0    \t: 0x%08x \n", reg->HFCR0);
    dbg_printf("\t HFCR1    \t: 0x%08x \n", reg->HFCR1);
    dbg_printf("\t HFCR2    \t: 0x%08x \n", reg->HFCR2);
    dbg_printf("\t HFCR3    \t: 0x%08x \n", reg->HFCR3);
    dbg_printf("\t HFFCR0   \t: 0x%08x \n", reg->HFFCR0);
    dbg_printf("\t HFFCR1   \t: 0x%08x \n", reg->HFFCR1);
    dbg_printf("\t HFFCR2   \t: 0x%08x \n", reg->HFFCR2);
    dbg_printf("\t HFFCR3   \t: 0x%08x \n", reg->HFFCR3);
    dbg_printf("\t HLOR0    \t: 0x%08x \n", reg->HLOR0);
    dbg_printf("\t HLOR1    \t: 0x%08x \n", reg->HLOR1);
    dbg_printf("\t HLOR2    \t: 0x%08x \n", reg->HLOR2);
    dbg_printf("\t HLOR3    \t: 0x%08x \n", reg->HLOR3);
    dbg_printf("\t HCCR0    \t: 0x%08x \n", reg->HCCR0);
    dbg_printf("\t HCCR1    \t: 0x%08x \n", reg->HCCR1);
    dbg_printf("\t HCCR2    \t: 0x%08x \n", reg->HCCR2);
    dbg_printf("\t HCCR3    \t: 0x%08x \n", reg->HCCR3);
    dbg_printf("\t HECR0    \t: 0x%08x \n", reg->HECR0);
    dbg_printf("\t HECR1    \t: 0x%08x \n", reg->HECR1);
    dbg_printf("\t HECR2    \t: 0x%08x \n", reg->HECR2);
    dbg_printf("\t HECR3    \t: 0x%08x \n", reg->HECR3);
    dbg_printf("\t HOCR0    \t: 0x%08x \n", reg->HOCR0);
    dbg_printf("\t HOCR1    \t: 0x%08x \n", reg->HOCR1);
    dbg_printf("\t HOCR2    \t: 0x%08x \n", reg->HOCR2);
    dbg_printf("\t HOCR3    \t: 0x%08x \n", reg->HOCR3);
    dbg_printf("\t HFSR0    \t: 0x%08x \n", reg->HFSR0);
    dbg_printf("\t HFSR1    \t: 0x%08x \n", reg->HFSR1);
    dbg_printf("\t HFSR2    \t: 0x%08x \n", reg->HFSR2);
    dbg_printf("\t HFSR3    \t: 0x%08x \n", reg->HFSR3);
    dbg_printf("\t HSET0    \t: 0x%08x \n", reg->HSET0);
    dbg_printf("\t HSET1    \t: 0x%08x \n", reg->HSET1);
    dbg_printf("\t HSET2    \t: 0x%08x \n", reg->HSET2);
    dbg_printf("\t HSET3    \t: 0x%08x \n", reg->HSET3);
    dbg_printf("\t MASK_CFG0\t: 0x%08x \n", reg->MASK_CFG0);
    dbg_printf("\t MASK_CFG1\t: 0x%08x \n", reg->MASK_CFG1);
    dbg_printf("\t MASK_CFG2\t: 0x%08x \n", reg->MASK_CFG2);
    dbg_printf("\t MASK_CFG3\t: 0x%08x \n", reg->MASK_CFG3);
    dbg_printf("\t TOP_TM0  \t: 0x%08x \n", reg->TOP_TM0);
    dbg_printf("\t TOP_TM1  \t: 0x%08x \n", reg->TOP_TM1);
    dbg_printf("\t HVSTAT   \t: 0x%08x \n", reg->HVSTAT);
    dbg_printf("\t TM0_CNT0 \t: 0x%08x \n", reg->TM0_CNT0);
    dbg_printf("\t TM0_CNT1 \t: 0x%08x \n", reg->TM0_CNT1);
    dbg_printf("\t TM0_CNT2 \t: 0x%08x \n", reg->TM0_CNT2);
    dbg_printf("\t TM0_CNT3 \t: 0x%08x \n", reg->TM0_CNT3);
    dbg_printf("\t TM0_CNT4 \t: 0x%08x \n", reg->TM0_CNT4);
    dbg_printf("\t TM0_CNT5 \t: 0x%08x \n", reg->TM0_CNT5);
    dbg_printf("\t TM0_CNT6 \t: 0x%08x \n", reg->TM0_CNT6);
    dbg_printf("\t TM0_CNT7 \t: 0x%08x \n", reg->TM0_CNT7);
    dbg_printf("\t TM1_CNT0 \t: 0x%08x \n", reg->TM1_CNT0);
    dbg_printf("\t TM1_CNT1 \t: 0x%08x \n", reg->TM1_CNT1);
    dbg_printf("\t TM1_CNT2 \t: 0x%08x \n", reg->TM1_CNT2);
    dbg_printf("\t TM1_CNT3 \t: 0x%08x \n", reg->TM1_CNT3);
    dbg_printf("\t TM1_CNT4 \t: 0x%08x \n", reg->TM1_CNT4);
    dbg_printf("\t TM1_CNT5 \t: 0x%08x \n", reg->TM1_CNT5);
    dbg_printf("\t TM1_CNT6 \t: 0x%08x \n", reg->TM1_CNT6);
    dbg_printf("\t TM1_CNT7 \t: 0x%08x \n", reg->TM1_CNT7);
    dbg_printf("\t HDLR0    \t: 0x%08x \n", reg->HDLR0);
    dbg_printf("\t HDLR1    \t: 0x%08x \n", reg->HDLR1);
    dbg_printf("\t HWCR0    \t: 0x%08x \n", reg->HWCR0);
    dbg_printf("\t HWCR1    \t: 0x%08x \n", reg->HWCR1);
    dbg_printf("\t HWCR2    \t: 0x%08x \n", reg->HWCR2);
    dbg_printf("\t HWCR3    \t: 0x%08x \n", reg->HWCR3);
}

#ifdef HAVE_CMD
int viddrv_dbcallback( int argc, char *argv[] )
{
    int i;
    
    if (argc >= 2) 
    {
        if (strcmp (argv[1],"dump")== 0) 
        {
            int start=0;
            int end=NUM_VIDEO_CHAN;
            
            cmd_printf("VIDEO dump information \n");
            
            if (argc >= 3) 
                start = atoi(argv[2]);
                
            if (argc >= 4) 
                end = atoi(argv[3]);
                
            if (start < 0 || end < 0)
                return 0;
            
            for (i=start; i<=end; i++)
            {
                cmd_printf("\nvideo channel %u \n", i);
                viddrv_dump_svd0_reg(i);
                viddrv_dump_hipwm_reg(i);
            }

            viddrv_dump_hvsync_reg();
    
            return 0;
        }
    }
    else
    {
        dbg_printf("debug:video dump [channel start] [channel end]\n");
    }
    
    return 0;
}
#endif // #ifdef HAVE_CMD
