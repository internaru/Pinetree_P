
/******************************************************************************
 * Copyright (c) 2009-2014  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip.c
 *
 * \brief Driver routines for PIP (image pipeline) HW block of ASIC.
 *
 * See the pip_api.h file for details on usage.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "pip_api.h"
#include "pip_message_api.h"
#include "pip.h"
#include "pip_utils.h"
#include "asic.h"
#include "zx_test_api.h"
#include "zx_idma_api.h"
#include "raster_odma_api.h"
#include "odma_int_attach.h"
#include "ued_api.h"
#include "th_api.h"
#include "cp_api.h"
#include "csc_api.h"
#include "nga_api.h"
#include "davbuf_api.h"
#include "pip_stats_api.h"
#include "fhs_cfg.h"
#include "kmod_cfg.h"
#include "davbuf_cfg.h"
#include "stat_cfg.h"
#include "tp_api.h"

#ifndef ASIC_granite 
#include "sharpen_api.h"
#include "ctwin_api.h"
#include "tems_api.h"
#endif
#include "color_reg_enhance_api.h"

#include "logger.h"
#include "dma_buffer.h"

#ifdef __linux__
#include "uio_lib_api.h"
#else // must be threadx
#include "interrupt_api.h"
#include "cpu_api.h"
#include "tx_api.h"
#endif

#include "debug.h"
#include "cdma_peripheral_ids.h"
#ifdef HAVE_CDMA
#include "cdma_api.h"
#endif
#include "intnums.h"
#include "memAPI.h"

#if 0
#include "jbig_api.h"
#endif

#include "print_mode.h"
#include "pip_cfg.h"
#include "lassert.h"

// KMOD dither and pattern header files
#include "kmod_api.h"
#include "kmod_dither_20_c.h"
#include "kmod_dither_20_k.h"
#include "kmod_dither_20_m.h"

#include "kmod_pattern_edgeK_0.h"
#include "kmod_pattern_edgeK_1.h"
#include "kmod_pattern_edgeK_2.h"
#include "kmod_pattern_edgeK_3.h"

// MIP LUTs
#include "mip_tielut_test.h"


#ifndef CDMA_AHS_0  // gemstone <- granite compatible for dma peripheral id names
#define CDMA_AHS_0 CDMA_STATS_AHS_0_RX
#define CDMA_AHS_1 CDMA_STATS_AHS_1_RX
#define CDMA_AHS_2 CDMA_STATS_AHS_2_RX
#define CDMA_PIPE_ACCESS CDMA_STATS_PIPE_ACCESS_RX
#endif

#ifdef ASIC_Gemstone

#define PIP_NUM_ODMA_CHANNELS 6
#define PIP_IP_ODMA_IRQ_MASK (PIP_IP_ODMA0_IRQ_MASK | \
                              PIP_IP_ODMA1_IRQ_MASK | \
                              PIP_IP_ODMA2_IRQ_MASK | \
                              PIP_IP_ODMA3_IRQ_MASK | \
                              PIP_IP_ODMA4_IRQ_MASK | \
                              PIP_IP_ODMA5_IRQ_MASK)

#else

#define PIP_NUM_ODMA_CHANNELS 8 
#define PIP_IP_ODMA_IRQ_MASK (PIP_IP_ODMA0_IRQ_MASK | \
                              PIP_IP_ODMA1_IRQ_MASK | \
                              PIP_IP_ODMA2_IRQ_MASK | \
                              PIP_IP_ODMA3_IRQ_MASK | \
                              PIP_IP_ODMA4_IRQ_MASK | \
                              PIP_IP_ODMA5_IRQ_MASK | \
                              PIP_IP_ODMA6_IRQ_MASK | \
                              PIP_IP_ODMA7_IRQ_MASK)

#endif

#define PIP_BLANK_LINE_SMIN 0xffff
#define PIP_BLANK_LINE_SMAX 0
#define PIP_STAT_EXTENT_BYTES 4
#define PIP_STAT_ACCUM_BYTES 4
#define PIP_STAT_GROUPA_CHANS 3
#define PIP_STAT_GROUPB_CHANS 2
#define PIP_STAT_GROUPC_CHANS 1
#define DBG_PRFX "PIP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

#define PIP_MESSAGE_SIZE_IN_32_BIT_WORDS sizeof(pip_message_t)/sizeof(uint32_t)

typedef enum
{
    PIP_IDLE = 1,
    PIP_PREPARED,
    PIP_GENERATING_ROWS
} pip_state_t;

// IDMA and ODMA descriptors
typedef struct pip_dma_desc_s
{
    uint32_t config;
    uint32_t src;
    uint32_t len;
    uint32_t next;
    uint32_t lines;
    uint32_t width;
    uint32_t lastline;
    uint32_t src2;
} pip_dma_desc_t;

// virtual and hw address of a single idma descriptor.
static pip_dma_desc_t *idma_descriptor_v;
static void *idma_descriptor_hw;

static pip_state_t pip_current_state = PIP_IDLE;
static uint32_t pipmode = IPC_UNUSED;
static pip_cfg_t* pip_cfg = NULL;
static int internal_buffering_lines_needed = 0;
static bool pippad = 0;   // Default to draft - setby pipmode ser console
static bool piptextmerge = false; // Off by default
static uint32_t pipscale_x = 1; // 1, 2, or 4
static uint32_t pipscale_y = 1; // 1, 2, or 4
static uint32_t pipfScale = 100;
static odma_handle_t *odma_handle[PIP_NUM_ODMA_CHANNELS];
static odma_handle_t *pa_odma_handle;
static dma_config_data_t pip_raster_odma_user_data[PIP_NUM_ODMA_CHANNELS];
static dma_config_data_t pip_pa_raster_odma_user_data;

//static bool uniform_strips = false; // true; /// uniform strip output.


#ifndef INTNUM_DAVINCI
// must be granite alias the interrupt number
#define INTNUM_DAVINCI INTNUM_PIP
#endif

sem_t pip_rows_availiable_semaphore;  // input data notification.

//  thread variables and defines
static pthread_t pip_thread_id;
#define STACK_SIZE POSIX_MIN_STACK_SIZE
static uint8_t pip_stack[STACK_SIZE] __attribute__ ((aligned (8)));
static mqd_t pip_inbox;
#define PIP_QUEUE_ENTRIES 100  // lsptodo: why so deep?

static pthread_mutex_t pip_isr_mutex;

// Local function definitions
static int pip_reset(void);
static void pip_set_default_top_cfg(void);

#ifdef __linux
static void pip_uio_isr(int32_t interrupt_count, void *context);
#else
static int pip_isr(uint32_t intpos);
#endif

static void pip_setup_davbuffers(uint32_t padded_width, uint32_t scaled_width);
static void pip_free_davbuffers(void);
#if 0
// NOT SHARING PIP WITH JBIG on laser platforms LSPTODO
static void pip_setup_jbig(uint32_t Xd,
                           uint32_t Yd,
                           uint32_t L0,
                           uint32_t bpp);

static error_type_t pip_stop_jbig(void);
#endif
static void pip_disable_odma_channel(uint32_t channel);
static void *pip_thread_func( void *input );

#ifdef __linux__
static void *pip_get_dev()
{
    static void *pip_dev = 0;

    if (!pip_dev)
    {
        pip_dev = uio_open_dev("pip_irq");
        XASSERT(pip_dev != 0, errno);
    }
    return pip_dev;
}
#define pip_int_attach( priority, isr, cb_data ) uio_int_attach( pip_get_dev(), priority, isr, cb_data )
#define pip_int_detach( ) uio_int_detach( pip_get_dev() )
#define pip_int_enable() uio_int_enable( pip_get_dev() )
#define pip_int_disable() uio_int_disable( pip_get_dev() )
#else
#define pip_get_dev void
#define pip_int_attach( priority, isr, cb_data ) intAttach( INTNUM_DAVINCI, cb_data, isr, priority )
#define pip_int_detach( ) intDetach( INTNUM_DAVINCI )
#define uio_int_disable( ) intDisable(INTNUM_DAVINCI)
#define uio_int_enable( ) intEnable(INTNUM_DAVINCI)
#endif  // __linux__

uint8_t alt_lut[256];

void th_cfg_alc_altlut_contents(uint32_t bpp)
{
    uint32_t level;
    double output;
    uint32_t output_rnd;
    uint32_t i;
    uint8_t out_array[8];
    uint8_t nib1;
    uint8_t nib2;
    uint8_t nib3;
    uint8_t nib4;
 
    // For 2bpp there are 8 2 bit values available for every input level. The
    // input is constrained to 6 bits, therefore there are 64 input levels. The
    // ALT LUT SRAM provides 8 2 bit outputs for each level, which are selected
    // in a pseudorandom sequence automatically by the hardware. The average of
    // the sum of the 8 2 bit outputs must reflect the relative magnitude of the
    // input level. The maximum available output is 3 for each 2 bit value, and
    // there are 8 of them so the available maximum value is 24.
    //
    // For example:
    // Input  0: Output: 0 0 0 0 0 0 0 0
    // Input 31: Output: 1 2 1 2 1 2 1 2 - average for these values is 1.5
    // Input 63: Output: 3 3 3 3 3 3 3 3
 
    // Only support 2 bpp at the moment.
    ASSERT(bpp == 2);
 
    for (level = 0; level < 64; level++)
    {

        memset(out_array, 0, sizeof(out_array));
 
        output = level/63.0 * 24.0;
        output_rnd = output + 0.5;
 
        for (i=0; i<output_rnd; i++)
        {
            out_array[i%8] += 1;
        }
 
        nib1 = (out_array[0]<<2) + out_array[1];
        nib2 = (out_array[2]<<2) + out_array[3];
        nib3 = (out_array[4]<<2) + out_array[5];
        nib4 = (out_array[6]<<2) + out_array[7];
 
        alt_lut[level*4]    = nib4;
        alt_lut[level*4 +1] = nib3;
        alt_lut[level*4 +2] = nib2;
        alt_lut[level*4 +3] = nib1;
    }
}



void pip_init(void)
{
    int status;

    status = posix_mutex_init(&pip_isr_mutex);
    XASSERT(status == 0, status);

    pip_int_attach( 0 /* INTPRI_PIP */, pip_uio_isr, 0 /* cb_data */);

    th_cfg_alc_altlut_contents(2);

    // Init HW submodules
    pip_setupregs();

    raster_odma_init();

#if ASIC_REV >= ASIC_REV_B0  && ASIC_gemstone
    color_reg_enhance_init();
#endif

    zx_idma_init();
    zx_idma_open();

    status = sem_init(&pip_rows_availiable_semaphore, 0, 1);
    XASSERT(status == 0, status);
    // Create message queue
    posix_create_message_queue( &pip_inbox, "/pip_mq", PIP_QUEUE_ENTRIES, sizeof(pip_message_t) );

    // Now start the PIP thread.
    posix_create_thread(&pip_thread_id, pip_thread_func, 0, "PipThread",
                         pip_stack, STACK_SIZE, POSIX_THR_PRI_REALTIME);
    // lsptodo: thread priority, queue depth is excessive,
}

#ifndef __linux__
int pip_isr(uint32_t intpos)
{
#else
void pip_uio_isr( int32_t interrupt_count, void *context )
{
#endif
    uint32_t pip_ipend;
    uint32_t pa_ipend;
    static int32_t prev_interrupt_count = 0;

    // Disable PIP Interrupt handler
    //pip_int_disable( );
    pthread_mutex_lock(&pip_isr_mutex);
    if (interrupt_count - prev_interrupt_count > 1) {
        DBG_MEMLOG_WARNING( "%s dropping interrupts %d\n", __FUNCTION__, interrupt_count - prev_interrupt_count );
    }
    prev_interrupt_count = interrupt_count;
    pip_ipend = pip_reg_u->IP;  // interrupt pending

    DBG_PRINTF_DEBUG_H( "%s --- %x\n", __FUNCTION__, pip_ipend);

    if (pip_ipend & PIP_IP_ODMA_IRQ_MASK)
    {
        raster_odma_handle_interrupt_event();
    }

    if ( pip_ipend & 0x00010000 ) //lsptodo granite idma_lpc interrrupt 
    {
        idma_isr(pip_lpc_idma3_reg);
    }        

    if (pip_ipend & PIP_IP_PA_IRQ_MASK)
    {
        // Check for pending PA IDMA interrupts
        pa_ipend = (pa_idma_reg->int_en & pa_idma_reg->int_st);      // PA IDMA
        if (pa_ipend & ~PA_IDMA_INT_EN_FIN_INT_EN_MASK)
        {   // Anything other than the finish interrupt
            // lsptodo: this just prints out status info.
            idma_isr(pa_idma_reg);   // pa_idma is an instance of IDMA in HW
        }
        if (pa_ipend & PA_IDMA_INT_EN_FIN_INT_EN_MASK)
        {   // Finish interrupt
            // Set PA Flag
            //lsptodo: this is a semaphore that no one listens to.
            // tx_res = tx_event_flags_set(&pip_idma_flags,PIP_IP_PA_IRQ_MASK,TX_OR);
            //ASSERT(TX_SUCCESS == tx_res);
        }
        // Ack the PA IDMA interrupts
        pa_idma_reg->int_cl = pa_idma_reg->int_st;

        // Check for pending PA ODMA interrupts
        pa_ipend = (pa_odma_reg->int_en & pa_odma_reg->int_pend);    // PA ODMA
        if (0 != pa_ipend)
        {
            raster_odma_handle_interrupt_event(); // this sends messages to pip's inbox
        }
    }
    pthread_mutex_unlock(&pip_isr_mutex);

    // Enable PIP Interrupt handler
    //pip_int_enable();
}


/**
 *==============================================================================
 * \brief Reset PIP hardware to power up state.
 *
 *==============================================================================
 */
static int pip_reset(void)
{
    int i;

    // Disable PIP Interrupt handler
    //pip_int_disable();
    // Reset Zx
    DBG_PRINTF_DEBUG("%s \n",__FUNCTION__); 

    zx_reg->ZCR = ZX_ZCR_ENABLE_REPLACE_VAL(zx_reg->ZCR,0);
    zx_idma_open();
    zx_idma_free_input(0, 1, 0);

    // Clear the enable bit in each ODMA and wait for busy bit to go away
    for (i = 0; i < PIP_NUM_ODMA_CHANNELS; i++)
    {
        odma_dav_reg[i]->cfg =
        ODMA_SLI_CFG_ENABLE_REPLACE_VAL(odma_dav_reg[i]->cfg,0);
        while (ODMA_SLI_STATUS_DMA_BUSY_MASK_SHIFT(odma_dav_reg[i]->status));
    }

    // Write a 1 to each ODMA0 int acknowledge
    for (i = 0; i < PIP_NUM_ODMA_CHANNELS; i++)
    {
        uint32_t regval;

        regval = odma_dav_reg[i]->int_ack;
        regval = ODMA_SLI_INT_ACK_RST_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_EOI_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_DIR_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_CL_ALI_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_EOL_ALI_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_EOI_ALI_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_EOI_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_FIN_INT_ACK_REPLACE_VAL(regval,1);
        regval = ODMA_SLI_INT_ACK_WHO_INT_ACK_REPLACE_VAL(regval,1);
        odma_dav_reg[i]->int_ack = regval;
    }
    /* lsptodo gemstone only 
    // Restore the ODMA Enables to the reset condition
    pip_reg_u->CFG = PIP_CFG_DMA0_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    pip_reg_u->CFG = PIP_CFG_DMA1_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    pip_reg_u->CFG = PIP_CFG_DMA2_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    pip_reg_u->CFG = PIP_CFG_DMA3_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    pip_reg_u->CFG = PIP_CFG_DMA4_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    pip_reg_u->CFG = PIP_CFG_DMA5_EN_REPLACE_VAL(pip_reg_u->CFG,1);
    */
    for (i = 0; i < PIP_NUM_ODMA_CHANNELS; i++)
    {
	odma_dav_reg[i]->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(odma_dav_reg[i]->cfg, 0);
    }

    // Hit pipeline soft reset
    pip_reg_u->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_u->CFG,0);
// lsptodo: pip_reg_l is believed to be related to serenity and not the pip_ued
//    pip_reg_l->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_l->CFG,0);
    pip_reg_u->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_u->CFG,1);
//    pip_reg_l->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_l->CFG,1);
    pip_reg_u->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_u->CFG,0);
//    pip_reg_l->CFG = PIP_CFG_SOFTRESET_REPLACE_VAL(pip_reg_l->CFG,0);

    // Delay to give everyone a chance to settle down.
    //lsptodo: cpu_spin_delay(1);

    // Set the top-level PIP config register to defaults.
    pip_set_default_top_cfg();

    // Attach the PIP interrupt channel
    // pip_int_detach();
    //pip_int_attach( 0 /* INTPRI_PIP */, pip_uio_isr, 0 /* cb_data */ );
    //pip_int_enable();

    return(0);
}

/**
 * \brief Local utility function to put the top-level PIP config register into
 * the default state for the SERP/Ink UED data path.
 *
 * \retval none.
 *
 */
static void pip_set_default_top_cfg(void)
{
    uint32_t regval;

    // Enable PIP interrupts, mask FHS, DB interrupts.
    regval = pip_reg_u->CFG;

    regval = PIP_CFG_IE_REPLACE_VAL(regval,1); // interrupt enable

    // Set MIP to output to SERP.
    regval = PIP_CFG_MS_REPLACE_VAL(regval,0);
#if ASIC_REV >= ASIC_REV_B0 && defined(ASIC_gemstone)
    // Enable CTWIN/TEMS path, set SRAMs to InkED/CTWIN.  Mux SRAMs to SERP and not
    // TEMS (this may be changed below).
    regval = PIP_CFG_IME_REPLACE_VAL(regval,0x780);
    regval = PIP_CFG_CRENCTWIN_REPLACE_VAL(regval, 0);
#else
    // granite laser 
    regval |= 0x00000010; // CRE select
#endif

#ifdef HAVE_DEC_LASER
    regval = PIP_CFG_RM_REPLACE_VAL(regval, 4);  // laser
#else
    regval = PIP_CFG_RM_REPLACE_VAL(regval, 0x0); // ink    
#endif
    DBG_PRINTF_DEBUG("pip config %x\n", regval);
    pip_reg_u->CFG = regval;

    internal_buffering_lines_needed = 0;     // reset per page
}



static int pip_page_rows = 0; /// TODO: Remove when pip_dma_api is ready.

/**
 * \brief API accessor to get the number of bits per pixel for a given
 *   print mode.
 *
 * \param[in] mode Print mode that we want to obtain the bpp for.
 *
 * \retval uint32_t Bits per pixel of the specified print mode.
 *
 */
uint32_t pip_mode_bpp_get(uint32_t mode)
{
    uint32_t bpp;
    error_type_t err;
	err = pip_cfg_mode_bpp_get(mode, &bpp);
    ASSERT(OK == err);
    return bpp;
}

/**
 * \brief API accessor to get the text name for a given print mode.
 *
 * \param[in] mode Print mode that we want to obtain the name for.
 *
 * \retval const char* String of the specified print mode.
 *
 */
const char* pip_mode_name_get(uint32_t mode)
{
    return pip_cfg_mode_name_get(mode);
}

/**
 * \brief API accessor to determine the video justification setting for a print mode.
 *
 * \param[in] mode Print mode that we want to obtain the justification setting for.
 *
 * \retval pip_justification_mode_t  The enumerated justification setting.
 *
 */
justification_mode_t pip_mode_justification_get(uint32_t mode)
{
    pip_cfg_t* pip_cfg;
    error_type_t err;

    err = pip_cfg_reference_get(mode, &pip_cfg);
    ASSERT(OK == err);

    if (pip_cfg->ued_cfg != NULL) {
      // Paired mode UED needs paired justification
      if (pip_cfg->ued_cfg->mode == 1)
          return VIDEO_JUSTIFY_PAIRED;
      // Regular UED wanted centered.
      return VIDEO_JUSTIFY_CENTERED;
    }
    // Non-UED uses auto.
    return VIDEO_JUSTIFY_AUTO;
}


/**
 *==============================================================================
 * \brief API to determine if a given page uses the image pipe.
 *
 * \param[in] local_page Page that we want to check.
 *
 * \retval bool True if the page uses the image pipe, false otherwise.
 *
 *==============================================================================
 */
bool pip_does_page_use_image_pipe(PAGE_DATA *local_page)
{
    bool ret;

    if((e_CONFIGPAGE!=local_page->DocType) &&
       (e_ALIGNMENTPAGE!=local_page->DocType) &&
       (e_CALPAGE!=local_page->DocType) &&
       (e_DDPAGE!=local_page->DocType))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool pip_is_mode_supported(uint32_t mode)
{
    return pip_cfg_is_mode_supported(mode);
}

void pip_frac_scale_set(uint32_t value)
{
    pipfScale = value;
}


/*----------------------------------------------------------------------
 * pip_prepare_page() - Prepare the PIP pipeline to generate output
 *   rasters by calling pip_generate_rows() successively.
 *
 * Parameters: pipmode
 *             pippad
 *             pipscale
 *----------------------------------------------------------------------
 */
static uint32_t adjust_bytecount = 0;

// All of the below variables are in pixels and correspond to the
//  [i]mage [w]idth and [i]mage [h]eight after the named block.
static uint32_t iw_in;
static uint32_t ih_in;

static uint32_t iw_fhs;
static uint32_t ih_fhs;

static uint32_t iw_pad;
static uint32_t ih_pad;

static uint32_t iw_scale;
static uint32_t ih_scale;

static uint32_t iw_out;
static uint32_t ih_out;

static uint32_t iw_out_bytes;

static uint32_t pip_genrow_last;
static uint32_t pip_genrow_total_lines;
static uint32_t pip_genrow_linecount; // track total lines generated so far
static uint32_t pip_genrow_lines_available; // lines that have been input into the pipeline
static uint32_t pip_pastore_last;
static uint32_t pip_pastore_total_lines;
static uint32_t pip_pastore_linecount; // track total lines generated so far
static uint32_t pip_paload_last;
static uint32_t pip_paload_total_lines;
static uint32_t pip_paload_linecount; // track total lines loaded so far
static bool pip_flip_horiz = false;
static uint32_t __attribute__ ((aligned(64))) cachelut[10464];

#if 0
JBIG_DECODE_IDMA_REGS_t idma_desc __attribute__ ((aligned(128)));

void jbig_dav_setup(JBIG_DECODE_REGS_t *decode_reg,
                    JBIG_DECODE_IDMA_REGS_t *decode_rdma_reg,
                    uint32_t rdma_addr,
                    uint32_t idma_data_addr,
                    uint32_t width,
                    uint32_t height,
                    uint32_t bpp);

void jbig_dec_disable(JBIG_DECODE_REGS_t *decode_reg,
                      JBIG_DECODE_IDMA_REGS_t *rdma_reg,
                      JBIG_DECODE_ODMA_REGS_t *wdma_reg);
#endif

// Used for plut generation
#define SCALE 255
#define SCALE2 64

#define STAT_BUF_WORDS 8         // Up to 3 colors * 2 words per color (round up to cache line size)
#define PA_STAT_BUF_WORDS 16     // 16 words per PA stats swath
uint32_t stat_buf_a[STAT_BUF_WORDS] ALIGN(32);
uint32_t stat_buf_b[STAT_BUF_WORDS] ALIGN(32);
uint32_t stat_buf_c[STAT_BUF_WORDS] ALIGN(32);
uint32_t pa_stat_buf[PA_STAT_BUF_WORDS] ALIGN(32);


void pip_setbypass()
{
    cr_reg->CFG = 0x0;
    //    cr_reg->CFG |= CR_CFG_ZXEN_MASK;
    cr_reg->CFG |= CR_CFG_BYPASS_MASK | (1<<9); // contone enable
    csc_dav_reg->CCR |= CSC_DAVINCI_CCR_BYPASSALL_MASK;
    nga_reg->Config |= NGADJUST_DAVINCI_CONFIG_BYPASS_MASK;

    // color ?
    pipe_access_cfg_driver(pa_reg, &pipe_access_mono8_cfg);

    sc_csc_reg->CNTRL |= SCCSC_CNTRL_BYPASSALL_MASK;
    serp_cfg_driver(serp_reg, &serp_bypass_cfg);
#ifdef ASIC_gemstone
    ued_reg->CFGA |= UED_BASIC_CFGA_BYPASS_MASK;
#else
    ued_reg->CFGA |= UED_ADVANCED_CFGA_BYPASS_MASK;
#endif
    yipi_cfg_driver(yipi_reg, &yipi_bypass_cfg);
    kmod_reg->KMDC1R |= KMOD_KMDC1R_BYPASS_MASK;
    stat_reg->ACR |= PIP_STATS_ACR_BYPASS_ENABLE_MASK;
    fhs_reg->control |= FHS_CONTROL_BYPASS_MASK;
#if ASIC_granite || (ASIC_REV >= ASIC_REV_B0 && ASIC_gemstone)

    th0_reg->TCR |= CORE_TCR_BYPASS_MASK;
    th1_reg->TCR |= CORE_TCR_BYPASS_MASK;
    th2_reg->TCR |= CORE_TCR_BYPASS_MASK;
#endif

    th3_reg->TCR |= CORE_TCR_BYPASS_MASK;
}

/// really prepare_image maybe page or strip/band.
error_type_t pip_prepare_page(uint32_t mode,
                              uint32_t int_scale_x,
                              uint32_t int_scale_y,
                              uint32_t input_width_in_pixels,
                              uint32_t frac_scale_width_in_pixels,
                              uint32_t input_height_in_lines,
                              uint32_t incremental_lines,
                              bool enable_pad,
                              bool enable_textmerge,
                              bool flip_horiz,
                              uint32_t *output_width_in_pixels)
{
    uint64_t *sccsc_cache;
    uint32_t i;
#if ASIC_REV >= ASIC_REV_B0  && defined(ASIC_gemstone)
    uint32_t regval;
#endif
    error_type_t ret;

    ret = pip_cfg_reference_get(mode, &pip_cfg);
    ASSERT(OK == ret);

    ret = FAIL;
    XASSERT(PIP_IDLE == pip_current_state, pip_current_state);

    pip_genrow_last = 0;
    pip_genrow_total_lines = input_height_in_lines * int_scale_y;
    pip_genrow_linecount = 0;
    pip_genrow_lines_available = 0;
    pip_pastore_last = 0;
    pip_pastore_total_lines = input_height_in_lines;  // before scaling
    pip_pastore_linecount = 0;
    pip_paload_last = 0;
    pip_paload_total_lines = input_height_in_lines;   // before scaling
    pip_paload_linecount = 0;
    pip_flip_horiz = flip_horiz;
    pip_page_rows = 0;

    pip_reset();

    pipmode = mode;
    pippad = enable_pad;
    piptextmerge = enable_textmerge;
    pipscale_x = int_scale_x;
    pipscale_y = int_scale_y;
    
    iw_in = input_width_in_pixels;
    ih_in = incremental_lines;

    // lsptodo : what is this and why is it in the config?
    pip_setbypass();


    sccsc_cache = (uint64_t *)0x0; // not used
    // lsptodo: 
    // why isn't fractional scaling based on the fhs_cfg ?
    iw_fhs = iw_in;
    ih_fhs = ih_in;
    ASSERT(NULL != pip_cfg->fhs_cfg);
    pip_cfg->fhs_cfg->imageinwidth = iw_in;
    
    if (frac_scale_width_in_pixels != iw_in )
    {
	// Use fractional scale output width if it is provided.
	pip_cfg->fhs_cfg->imageoutwidth = frac_scale_width_in_pixels;
    }
    else if (pipfScale != 100)
    {
	// Else use a scale factor if provided.
	pip_cfg->fhs_cfg->imageoutwidth = (iw_in * pipfScale)/100;
    }
    else
    {
	// Else do not perform fractional scaling.
	pip_cfg->fhs_cfg->imageoutwidth = iw_in;
    }
    fhs_setup(fhs_reg,pip_cfg->fhs_cfg);
    DBG_PRINTF_NOTICE("FHS - pipfScale = %d x %d y %d inWidth = %d outWidth = %d\n",
		      pipfScale,
		      int_scale_x, int_scale_y,
		      pip_cfg->fhs_cfg->imageinwidth,
		      pip_cfg->fhs_cfg->imageoutwidth);
    iw_fhs = pip_cfg->fhs_cfg->imageoutwidth;
    
	    
    
    
    iw_pad = iw_fhs;
    ih_pad = ih_fhs;
    
    // Contone pad
    if (true == pippad)
    {
	uint32_t hpad; 
	iw_pad = pip_padded_image_width( iw_fhs, 1 );  // NO scaling yet 
	hpad = iw_pad - iw_fhs;
	
	cp_cfg.rightlen = hpad/2; // hpad/2;
	cp_cfg.leftlen = hpad - (hpad/2); // hpad - (hpad/2);
	DBG_PRINTF_NOTICE("pippad=%d image_width=%d iw_pad=%d hpad=%d rl=%d ll=%d\n",pippad, iw_fhs, iw_pad, hpad, cp_cfg.rightlen, cp_cfg.leftlen);
	//cp_cfg.leftedgefill = 1; // fill with first pixel of line.
	//cp_cfg.rightedgefill = 1; // fill with last pixel of line.
	// color space based padding value
	extern uint32_t PA_inverse[]; // header file data, included ONCE.
	if (pip_cfg->onedlut4 == (uint8_t *)PA_inverse) { 
	    cp_cfg.rightval = -1;  // additive white.
	    cp_cfg.leftval = -1;  
	} else {
	    cp_cfg.rightval =  0;   // subtractive white.
	    cp_cfg.leftval =  0;   
	}
    }
    else
    {
	cp_cfg.rightlen = 0;
	cp_cfg.leftlen = 0;
    }
    pip_cfg->cp_cfg = &cp_cfg;
    ASSERT(NULL != pip_cfg->cp_cfg);
    cp_setup(cp_reg, pip_cfg->cp_cfg);
    
    // Calculate scaled image size output from SERP
    iw_scale = iw_pad;
    ih_scale = ih_pad;
    iw_scale *= pipscale_x;
    ih_scale *= pipscale_y;
    DBG_ASSERT( iw_scale == pip_padded_image_width( input_width_in_pixels ,pipscale_x )); // Fails on fhs scaling

    // Configure SERP or TEMS SRAM muxing and integer scaling
#if ASIC_REV >= ASIC_REV_B0  && defined(ASIC_gemstone)
    if ((NULL!=pip_cfg->tems_cfg) &&
	(&tems_bypass_cfg!=pip_cfg->tems_cfg))
    {
	// Make sure SERP is in bypass (SERP and TEMS are mutually exclusive)
	pip_cfg->serp_cfg = &serp_bypass_cfg;
		    
	// Set the TEMS scale integer scaling
	if (0 == pip_cfg->tems_cfg->text_enable)
	{
	    pip_cfg->tems_cfg->hscale_m1 = pipscale_x-1;
	    pip_cfg->tems_cfg->vscale_m1 = pipscale_y-1;
	}
	else if (0 == pip_cfg->tems_cfg->text_res)
	{
	    // 2x2 super pixel
	    XASSERT(0 == pipscale_x%2, pipscale_x);
	    XASSERT(0 == pipscale_y%2, pipscale_y);
	    pip_cfg->tems_cfg->hscale_m1 = pipscale_x/2 -1;
	    pip_cfg->tems_cfg->vscale_m1 = pipscale_y/2 -1;
	}
	else if (1 == pip_cfg->tems_cfg->text_res)
	{
	    // 1x2 super pixel
	    XASSERT(0 == pipscale_y%2, pipscale_y);
	    pip_cfg->tems_cfg->hscale_m1 = pipscale_x-1;
	    pip_cfg->tems_cfg->vscale_m1 = pipscale_y/2 -1;
	}
	else
	{
	    // 2x1 super pixel
	    XASSERT(2 == pip_cfg->tems_cfg->text_res, pip_cfg->tems_cfg->text_res);
	    XASSERT(0 == pipscale_x%2, pipscale_x);
	    pip_cfg->tems_cfg->hscale_m1 = pipscale_x/2 -1;
	    pip_cfg->tems_cfg->vscale_m1 = pipscale_y-1;
	}
	
	// Mux the SRAMs into TEMS mode
	regval = pip_reg_u->CFG;
	regval = PIP_CFG_CRENCTWIN_REPLACE_VAL(regval, 0);
	regval = PIP_CFG_RM_REPLACE_VAL(regval, 0x1);
	pip_reg_u->CFG = regval;
	DPRINTF((DBG_OUTPUT|DBG_LOUD), ("     pip tems ued %d-bit  %d rows\n", pip_cfg->bpp, pip_genrow_total_lines));
    }
    else
    {
#endif
	// Set the SERP integer scaling
	if (NULL != pip_cfg->serp_cfg)
	{
	    pip_cfg->serp_cfg->contone_h_scale = pipscale_x-1;
	    pip_cfg->serp_cfg->contone_v_scale = pipscale_y-1;
	    
	    // linear scaling can only be enabled when scaling by 2; since linear
	    // overrides the contone setting disable linear if scale factor != 2 -jrs
	    if (pip_cfg->serp_cfg->h_bilinear && (pipscale_x != 2))
	    {
		pip_cfg->serp_cfg->h_bilinear = 0;
	    }
	    if (pip_cfg->serp_cfg->v_bilinear && (pipscale_y != 2))
	    {
		pip_cfg->serp_cfg->v_bilinear = 0;
	    }
	    
	}
#if ASIC_REV >= ASIC_REV_B0  && defined(ASIC_gemstone)
	// Mux the SRAMs into SERP mode
	if (NULL != pip_cfg->ued_cfg)  {
	    regval = pip_reg_u->CFG;
	    regval = PIP_CFG_CRENCTWIN_REPLACE_VAL(regval, 0);
	    regval = PIP_CFG_RM_REPLACE_VAL(regval, 0x0);
	    pip_reg_u->CFG = regval;
	    DPRINTF((DBG_OUTPUT|DBG_LOUD), 
		    ("     pip serp error-diffusion %d-bit  %d rows\n", pip_cfg->bpp, pip_genrow_total_lines));
	} else {
	    // Setup the CRE block
	    if (NULL != pip_cfg->cre_cfg)
	    {
		color_reg_enhance_set_cfg(pip_cfg->cre_cfg, &internal_buffering_lines_needed);
	    }
	    
	    regval = pip_reg_u->CFG;
	    regval = PIP_CFG_CRENCTWIN_REPLACE_VAL(regval, 1);
	    regval = PIP_CFG_RM_REPLACE_VAL(regval, 0x7);
	    pip_reg_u->CFG = regval;
	    DPRINTF((DBG_OUTPUT|DBG_LOUD), 
		    ("     pip serp threshold %d-bit  %d rows\n", pip_cfg->bpp, pip_genrow_total_lines));
	}
    }
#else // granite
    const bool INK_MODE = false;  // lsptodo move mode to config file. 
    if ( INK_MODE ) {
	pip_reg_u->CFG = 0x00000012; // ink mode 0
    } else if(pip_cfg->ued_cfg) { // ued
	pip_reg_u->CFG = 0x10000012; // laser mode 1 cre enable even if we disable later for laser
    } else { // threshold
	pip_reg_u->CFG = 0x40000012; // laser mode 4 cre enable even if we disable later for laser
    }
    if (pip_cfg->raster_odma_cfg[0] == NULL ) {
	pip_reg_u->DMA_CFG = 0x01150200;  // gray 8bit input -> K only output 
    } else {
	pip_reg_u->DMA_CFG = 0x011503c0; // color 24 -> color 4 plane out
    }
    DBG_PRINTF_INFO("DMA_CFG %x CFG %x \n", 	pip_reg_u->DMA_CFG, pip_reg_u->CFG ); 
    
    // Setup the CRE block
    if (NULL != pip_cfg->cre_cfg) {
	color_reg_enhance_set_cfg(pip_cfg->cre_cfg, &internal_buffering_lines_needed );
    } 
    else { 
	cre_reg->CNTRL = 0x8000004f; // bypass CRE 
    }
    
#endif

    // Configure the text merge path if it is enabled
    if ((true==piptextmerge) &&
	(NULL!=pip_cfg->serp_cfg) &&
	(&serp_bypass_cfg!=pip_cfg->serp_cfg))
    {
	// Configure Text Pad with text merge cfg
	tp_setup(tp_reg,&tp_jbig_txtmerge_cfg);
	
	// Configure SERP for text merge
	pip_cfg->serp_cfg->text_h_scale = pipscale_x-1;  // Default is contone = text scale
	pip_cfg->serp_cfg->text_v_scale = pipscale_y-1;
	pip_cfg->serp_cfg->text_source = 10;
	pip_cfg->serp_cfg->text_bits = 1;
	pip_cfg->serp_cfg->text_merge_enable = 1;
	// Start JBIG
#if 1		    
	ASSERT(0);  // lsptodo: port jbig input to pip
#else
	pip_setup_jbig(iw_pad,
		       input_height_in_lines,
		       8,
		       pip_cfg->serp_cfg->text_bits);
#endif
    }
    else
    {
	// No text merge - clear out serp_cfg values
	if (NULL != pip_cfg->serp_cfg)
	{
	    pip_cfg->serp_cfg->text_merge_enable = 0;
			    pip_cfg->serp_cfg->text_source = 10;  // 1bpp
			    pip_cfg->serp_cfg->text_bits = 1;
	}
    }
    
#if ASIC_REV >= ASIC_REV_B0  && defined(ASIC_gemstone)
    if (NULL != pip_cfg->sharpen_cfg)
    {
	pip_cfg->sharpen_cfg->image_width = iw_pad;
	sharpen_cfg_driver(sharpen_reg, pip_cfg->sharpen_cfg);
    }
#endif

    if (NULL != pip_cfg->pipe_access_cfg)
    {
	if (1 == pip_cfg->pipe_access_cfg->stats_en)
	{
	    pip_cfg->pipe_access_cfg->stats_image_width = iw_pad;
	}
	
	pipe_access_cfg_driver(pa_reg, pip_cfg->pipe_access_cfg);
	
        // Make sure we do nothing for bypass mode configurations
	if (!pip_cfg->pipe_access_cfg->bypass)
	{
	    pipe_access_setup_luts(pa_reg,
				   (uint32_t*)pip_cfg->onedlut0,
				   (uint32_t*)pip_cfg->onedlut1,
				   (uint32_t*)pip_cfg->onedlut2,
				   (uint32_t*)pip_cfg->onedlut3,
				   (uint32_t*)pip_cfg->onedlut4,
				   (uint32_t*)pip_cfg->onedlut5);
	}
    }
    
    if (NULL != pip_cfg->sccsc_cfg)
    {
	sccsc_setup(sc_csc_reg,
		    pip_cfg->sccsc_cfg,
		    (uint64_t *) pip_cfg->sccsc_table,
		    (uint64_t *) pip_cfg->sccsc_table,
		    0,
		    0,
		    (uint32_t) (((uint32_t)cachelut)>>3));  // pip_cfg->sccsc_table
    }
    
    if (NULL != pip_cfg->serp_cfg) 
    {
	ASSERT( ! (pip_cfg->serp_cfg->bypass && pipscale_y > 1) ); 
	serp_cfg_driver(serp_reg, pip_cfg->serp_cfg);
	
	if ( internal_buffering_lines_needed )
	    internal_buffering_lines_needed *= pipscale_y;
	else if ( pip_cfg->serp_cfg->edge_strength_en )
	    internal_buffering_lines_needed = pipscale_y;
	else if ( pip_cfg->serp_cfg->bypass || pip_cfg->serp_cfg->high_res_text_enable )
	    internal_buffering_lines_needed = 0;
	else 
	    internal_buffering_lines_needed = 2;
    }
    
#if ASIC_REV >= ASIC_REV_B0  && defined(ASIC_gemstone)
    if (NULL != pip_cfg->ctwin_cfg)
    {
	pip_cfg->ctwin_cfg->line_width = iw_pad;
	ctwin_cfg_driver(ctwin_reg, pip_cfg->ctwin_cfg);
    }
    
    if (NULL != pip_cfg->tems_cfg)
    {
	tems_cfg_driver(tems_reg, pip_cfg->tems_cfg);
    }
#endif

    if (NULL != pip_cfg->ued_cfg)
    {
	pip_cfg->ued_cfg->up0_height = ih_scale;    // image_height;
	pip_cfg->ued_cfg->up0_width  = iw_scale;    // image_width;
	pip_cfg->ued_cfg->dn_height  = ih_scale;    // image_height;
	pip_cfg->ued_cfg->dn_width   = iw_scale;    // image_width;
	// Modify first line values to account for per-strip hardware setup
	if (pip_page_rows == 0) {
	    pip_cfg->ued_cfg->first_line_error_mode = 2;
	} else {
	    pip_cfg->ued_cfg->first_line_error_mode = 1;
	}
	ued_cfg_driver(ued_reg, pip_cfg->ued_cfg);
	if (pip_cfg->ued_cfg->weight8_en)
	{
	    ued_setup_luts(ued_reg, pip_cfg->ued_cfg->lut_data);
	}
	else
	{
	    ued_setup_luts_wlut4(ued_reg, pip_cfg->ued_cfg->lut_data);
	}
    }
    if (NULL != pip_cfg->th_cfg[0])
    {
	th_cfg_driver(th0_reg, th0_sram_reg, pip_cfg->th_cfg[0], pip_page_rows);		    
    }
    if (NULL != pip_cfg->th_cfg[1])
    {
	th_cfg_driver(th1_reg, th1_sram_reg, pip_cfg->th_cfg[1], pip_page_rows);
    }
    if (NULL != pip_cfg->th_cfg[2])
    {	   
	th_cfg_driver(th2_reg, th2_sram_reg, pip_cfg->th_cfg[2], pip_page_rows);
    }
    if (NULL != pip_cfg->th_cfg[3])
    {
	th_cfg_driver(th3_reg, th3_sram_reg, pip_cfg->th_cfg[3], pip_page_rows);
    }
    
    if (NULL != pip_cfg->mip_cfg)
    {
	mip_load_tielut(TIELUT_test,sizeof(TIELUT_test)/4,mip_reg);
	mip_check_tielut(TIELUT_test,sizeof(TIELUT_test)/4,mip_reg);
	mip_cfg_driver(mip_reg, pip_cfg->mip_cfg);
    }
    
    if (NULL != pip_cfg->yipi_cfg)
    {
	yipi_cfg_driver(yipi_reg, pip_cfg->yipi_cfg);
    }
    
    if (NULL != pip_cfg->kmod_cfg)
    {
	kmod_load_dither(kmod_reg,dither_20_c,dither_20_m,dither_20_k);
	kmod_load_pattern(kmod_reg,pattern_edgeK_0,pattern_edgeK_1,pattern_edgeK_2,pattern_edgeK_3);
	kmod_cfg_driver(kmod_reg, pip_cfg->kmod_cfg);
    }
	    
#if 0 // lsptodo no stats
    if (NULL != pip_cfg->stat_cfg &&
	(pip_cfg->stat_cfg->stats_enable || pip_cfg->stat_cfg->accumulate))
    {
	// Configure STAT block
	pip_cfg->stat_cfg->image_width = iw_scale-1;
	stat_cfg_driver(stat_reg, pip_cfg->stat_cfg);
	
	ret = stat_cfg_buffer(CDMA_AHS_0);
	ASSERT(OK == ret);
	ret = stat_cfg_buffer(CDMA_AHS_1);
	ASSERT(OK == ret);
	ret = stat_cfg_buffer(CDMA_AHS_2);
	ASSERT(OK == ret);
    }
#endif// lsptodo no stats
    iw_out = iw_scale;
    ih_out = ih_scale;
    if (2 == pip_cfg->bpp)
    {
	iw_out_bytes = iw_scale / 4;  // 2bpp 4 pixels per byte
    }
    else if (1 == pip_cfg->bpp)
    {
	iw_out_bytes = iw_scale / 8;  // 1bpp 8 pixels per byte
    }
    else if (4 == pip_cfg->bpp)
    {
	iw_out_bytes = iw_scale / 2;
    }
    else if (8 == pip_cfg->bpp)
    {
	iw_out_bytes = iw_scale;
    }
    else if (16 == pip_cfg->bpp)
    {
	iw_out_bytes = iw_scale * 2;
    }
    else
    {
	ASSERT(0);  // Don't recognize bits per pixel
    }
    
    // Used as 1 time trigger to pip_generate_rows() to adjust image dimension values
    adjust_bytecount = 1;
    
    DBG_PRINTF_NOTICE(
	"page_setup iw_in = %d ih_in = %d\n        iw_pad = %d ih_pad = %d\n",
	iw_in, ih_in, iw_pad, ih_pad);
    DBG_PRINTF_NOTICE(
	"        iw_scale = %d ih_scale = %d\n        iw_out = %d ih_out = %d\n",
	iw_scale, ih_scale,iw_out,ih_out);
    
    *output_width_in_pixels = iw_out;
    
    // Now setup the output DMAs
    for (i=0; i<PIP_NUM_ODMA_CHANNELS; i++)
    {
        if (NULL != pip_cfg->raster_odma_cfg[i])
        {
            pip_cfg->raster_odma_cfg[i]->flip_horizontal = flip_horiz;
            ret = raster_odma_open(&odma_handle[i], i);
            XASSERT(OK == ret, ret);

            ret = raster_odma_register_notifcation_callback(odma_handle[i], 0); // lsptodo: uses semaphore instead: &pip_odma_data_buf_complete);
            XASSERT(OK == ret, ret);

            ret = raster_odma_input_width_in_bpp(odma_handle[i], pip_cfg->bpp);
            XASSERT(OK == ret, ret);

            ret = raster_odma_set_line_width_in_bytes(odma_handle[i], iw_out_bytes);
            XASSERT(OK == ret, ret);
        }
        else
        {
		odma_handle[i] = 0;
        }
    }
    
    if (NULL != pip_cfg->pipe_access_cfg &&
        NULL != pip_cfg->pa_odma_cfg)
    {
        pip_cfg->pa_odma_cfg->flip_horizontal = false;
        ret = raster_odma_open(&pa_odma_handle, ODMA_PIPE_ACCESS);
        XASSERT(OK == ret, ret);

        ret = raster_odma_register_notifcation_callback(pa_odma_handle, 0); // &pip_odma_data_buf_complete);
        XASSERT(OK == ret, ret);

        ret = raster_odma_input_width_in_bpp(pa_odma_handle, pip_cfg->bpp);
        XASSERT(OK == ret, ret);

        if (PA_STORE_6_CHANNEL == pip_cfg->pipe_access_cfg->pipeline_store_en)
        {
            ret = raster_odma_set_line_width_in_bytes(pa_odma_handle, iw_out*8);
            XASSERT(OK == ret, ret);
        }
        else
        {
            ret = raster_odma_set_line_width_in_bytes(pa_odma_handle, iw_out*4);
            XASSERT(OK == ret, ret);
        }
    }

    pip_setup_davbuffers(iw_pad, iw_out);
    pip_current_state = PIP_PREPARED;

    pip_page_rows += pip_genrow_total_lines;

    return ret;
}



/**
 *  \brief Return the number of output lines expected for a given input strip size
 *  and pip configuration.  For some pip configurations PIP will buffer some number of
 *  lines internally and not flush the lines through until the end-of-image indication
 *  is sent through the pipe.  Thus the first output DMA strip will be the buffered
 *  number of lines shorter than expected and the last strip will be the buffered
 *  number of lines longer than expected.  This function returns the number of lines that
 *  pip will output for a given configuration allowing the output DMAs for the
 *  first and last strips to be configured to the correct number of lines.
 *  
 * \param input_strip_height Input strip height in lines. 
 *  
 * \param start_of_image - Boolean indication that this is the first strip in a multi-strip image. 
 *  
 * \param end_of_image - Boolean indication that this is the last strip in a multi-strip image. 
 *  
 * \return Number of lines in the output strip.
 *  
 * \remarks NOTE that in order to obtain the correct strip height value this routine 
 * must be called after pip_prepare_page.   If called early, the previous page's 
 * strip height settings will be returned. 
 */
uint32_t pip_calculate_output_strip_height(uint32_t input_strip_height, bool start_of_image, bool end_of_image)
{
    uint32_t output_strip_height = input_strip_height;

    if ( internal_buffering_lines_needed > 0 ) {
	output_strip_height *= pipscale_y;
	if (start_of_image) {
	    output_strip_height -= 0; // internal_buffering_lines_needed;
	}
	if (end_of_image) {
	    output_strip_height += internal_buffering_lines_needed;
	}           
    } 
    DBG_ASSERT( !(internal_buffering_lines_needed == 0 && pipscale_y > 1 ) );

    return output_strip_height;
}

/*----------------------------------------------------------------------
 * pip_generate_rows() - Generate the next nlines output rows before
 *   returning.
 *
 *----------------------------------------------------------------------
 */
error_type_t pip_generate_rows(uint32_t *flocs[],
                               uint32_t nlines_out,
                               pip_stats_entry_t *combined_stats,
                               pip_stats_entry_t chan_stats[],
			       bool first,
			       bool last,
			       uint32_t total_linecount // even odd && serpentine flag.
    )
{
    // uint32_t first, last;

    uint32_t i;
    bool line_reverse;
//    bool evenodd = total_linecount % 2;
    error_type_t ret = FAIL;

    // Setup the ODMA Channels

    for (i=0; i<PIP_NUM_ODMA_CHANNELS; i++)
    {
        if (NULL != pip_cfg->raster_odma_cfg[i])
        {
            line_reverse = 0; // pip_cfg->raster_odma_cfg[i]->serpentine ? evenodd : 0;
            pip_cfg->raster_odma_cfg[i]->line_reverse = line_reverse;
            ret = raster_odma_set_cfg(odma_handle[i], pip_cfg->raster_odma_cfg[i]);
            XASSERT(OK == ret, ret);
            ret = raster_odma_add_input_buffer(odma_handle[i],
                                               (char *)flocs[i],
                                               iw_out_bytes * nlines_out, 
                                               nlines_out, 
                                               first, // SOI
                                               last,  // EOI
                                               true,  // tsop_on_finish
                                               &pip_raster_odma_user_data[i]);
            XASSERT(OK == ret, ret);
        }
        else
        {
            pip_disable_odma_channel(i);
        }
    }

    // Enable KMOD if necessary
    if (NULL != pip_cfg->kmod_cfg)
    {
        kmod_enable(kmod_reg);
    }
#if 0 //lsptodo no stats yet
    // Start STAT CDMA transfers
    if (NULL != pip_cfg->stat_cfg &&
        (pip_cfg->stat_cfg->stats_enable || pip_cfg->stat_cfg->accumulate))
    {
        // Let's configure CDMA to point to fixed buffers
        ASSERT(pip_cfg->stat_cfg->a_swath_height == (nlines_out-1) &&
               pip_cfg->stat_cfg->b_swath_height == (nlines_out-1) &&
               pip_cfg->stat_cfg->c_swath_height == (nlines_out-1));

        stat_bytes_per_channel = 0;
        if (pip_cfg->stat_cfg->stats_enable)
        {
            stat_bytes_per_channel += PIP_STAT_EXTENT_BYTES;
        }
        if (pip_cfg->stat_cfg->accumulate)
        {
            stat_bytes_per_channel += PIP_STAT_ACCUM_BYTES;
        }

        // STATS0
        ret = stat_dma_start(stat_buf_a,
                             (uint32_t)PIP_STAT_GROUPA_CHANS*stat_bytes_per_channel,
                             (uint32_t *) &stat_cdma0->DATA,
                             CDMA_AHS_0);
        ASSERT(OK == ret);

        // STATS1
        ret = stat_dma_start(stat_buf_b,
                             (uint32_t)PIP_STAT_GROUPB_CHANS*stat_bytes_per_channel,
                             (uint32_t *) &stat_cdma1->DATA,
                             CDMA_AHS_1);
        ASSERT(OK == ret);

        // STATS2
        ret = stat_dma_start(stat_buf_c,
                             (uint32_t)PIP_STAT_GROUPC_CHANS*stat_bytes_per_channel,
                             (uint32_t *) &stat_cdma2->DATA,
                             CDMA_AHS_2);
        ASSERT(OK == ret);
    }
#endif // lsptodo no stats
    // Start ODMA transfers
    for (i=0; i<PIP_NUM_ODMA_CHANNELS; i++)
    {
        if (NULL != pip_cfg->raster_odma_cfg[i])
        {
            ret = raster_odma_start_xfer(odma_handle[i]);
            XASSERT(OK == ret, ret);
        }
    }
    return ret;
}

void pip_block_output()
{
    int i;
// out wait
// */
    //raster_odma_dump_regs();
    /*DBG_PRINTF_DEBUG("%s: ODMA_WAIT: evenodd = %d, lines_out = %d, SOI = %d, EOI = %d\n", __func__, evenodd, nlines_out, first, last);
     */
	// Wait for the ODMA flags indicating interrupts have fired
//	if (nlines_out < 10)
	    DBG_PRINTF_DEBUG("%s: ODMA_WAIT start \n", __func__);
    for (i=0; i<PIP_NUM_ODMA_CHANNELS; i++)
    {
	if (NULL != pip_cfg->raster_odma_cfg[i])
	{
	    // sleep(1);
	    ODMA_WAIT( odma_handle[i] ); 
	    //            tx_res = tx_event_flags_get(&pip_odma_flags,
	    //                            1<<i,
	    //                            TX_AND_CLEAR,
	    //                            &flags,TX_WAIT_FOREVER);
	    //ASSERT(TX_SUCCESS == tx_res);
	}
    }

//	if (nlines_out < 10)
	    DBG_PRINTF_DEBUG("%s: ODMA_WAIT DONE\n", __func__);
}
    //raster_odma_dump_regs();

#if 0 //lsptodo add back stats
    // Wait for STAT CDMA transfers to complete
    if (NULL != pip_cfg->stat_cfg &&
        (pip_cfg->stat_cfg->stats_enable || pip_cfg->stat_cfg->accumulate))
    {
        tx_res = tx_event_flags_get(&pip_stat_flags,
                                    (PIP_STATA_FLAG | PIP_STATB_FLAG | PIP_STATC_FLAG),
                                    TX_AND_CLEAR,
                                    &flags,
                                    TX_WAIT_FOREVER);
        ASSERT(TX_SUCCESS == tx_res);

        cpu_dcache_invalidate_region(stat_buf_a, STAT_BUF_WORDS*sizeof(uint32_t));
        cpu_dcache_invalidate_region(stat_buf_b, STAT_BUF_WORDS*sizeof(uint32_t));
        cpu_dcache_invalidate_region(stat_buf_c, STAT_BUF_WORDS*sizeof(uint32_t));

        stat_read_stats(stat_buf_a, stat_buf_b, stat_buf_c, combined_stats, chan_stats);

        // flip the statistics min/max if we are doing horizontal mirroring
        if (true == pip_flip_horiz)
        {
            // flip smin/smax if this line is not entirely composed of whitespace
            if ((PIP_BLANK_LINE_SMIN!=combined_stats->smin) && (PIP_BLANK_LINE_SMAX!=combined_stats->smax))
            {
                temp = iw_out - combined_stats->smax;
                combined_stats->smax = iw_out - combined_stats->smin;
                combined_stats->smin = temp;
                for (i=0; i<PIP_MAX_OUTPUT_CHANS; i++)
                {
                    temp = iw_out - chan_stats[i].smax;
                    chan_stats[i].smax = iw_out - chan_stats[i].smin;
                    chan_stats[i].smin = temp;
                }
            }
        }
    }
    else
    {
        // If stats are disabled, assume no whitespace.
        combined_stats->smin = 0;
        combined_stats->smax = iw_out;
        combined_stats->accum = 0xffffffff;
        for (i=0; i<PIP_MAX_OUTPUT_CHANS; i++)
        {
            chan_stats[i].smin = 0;
            chan_stats[i].smax = iw_out;
            chan_stats[i].accum = 0xffffffff;
        }
    }

    DBG_PRINTF_DEBUG_H(
               "combined stats[%u]: smin:%u smax:%u accum:%u\n",
               pip_genrow_linecount,
               combined_stats->smin,
               combined_stats->smax,
               combined_stats->accum);
    for (i=0; i<PIP_MAX_OUTPUT_CHANS; i++)
    {
        DBG_PRINTF_DEBUG_H(
                   "chan%u: smin:%u smax:%u accum:%u\n",
                   i,
                   chan_stats[i].smin,
                   chan_stats[i].smax,
                   chan_stats[i].accum);
    }
    kmod_disable(kmod_reg);



    pip_genrow_linecount += nlines_out;

    pip_current_state = PIP_GENERATING_ROWS;

    *rows_generated = nlines_out;

    DBG_PRINTF_DEBUG("<==%s\n", __func__);

    return ret;
}
#endif
#if 0
void pip_notify_rows_available(uint32_t num_rows, bool last_rows)   // these calls need to occur after pip_prepare_page()...
{
    pip_genrow_lines_available += num_rows * pipscale_y;

    if (true == last_rows)
    {
        pip_genrow_total_lines = pip_genrow_lines_available;
    }

    // lsptodo : mutex or semaphore
    sem_post( &pip_rows_availiable_semaphore );
}
#endif
/*----------------------------------------------------------------------
 * pip_close_page() - Close the current PIP page and clean up.
 *
 *----------------------------------------------------------------------
 */
error_type_t pip_close_page(void)
{
    uint32_t i;
    error_type_t ret;

    ret = FAIL;

    if ((PIP_GENERATING_ROWS==pip_current_state) ||
        (PIP_PREPARED==pip_current_state))
    {
#if 0 // lsptodo no stats
        // Close the PIP Stats CDMAs
        stat_cdma_close();
#endif
        // Free Davinci Buffers
        //pip_free_davbuffers();

        // Stop the jbig block if textmerge is enabled
        ret = OK;
        if (true == piptextmerge)
        {
#if 0
            ret = pip_stop_jbig();
#endif
        }

        // Close the odma driver handles
        for (i=0; i<PIP_NUM_ODMA_CHANNELS; i++)
        {
            if (NULL != pip_cfg->raster_odma_cfg[i])
            {
                ret = raster_odma_close(odma_handle[i]);
                XASSERT(OK == ret, ret);

            }
        }
        if (NULL != pip_cfg->pa_odma_cfg)
        {
            ret = raster_odma_close(pa_odma_handle);
            XASSERT(OK == ret, ret);
        }

        // Reset the SRAM muxing mode
        // pip_set_default_top_cfg();

        pip_current_state = PIP_IDLE;
    }

    // Give the PIP interrupt back to the odma driver
    // lsptodo test odma and pip transfer of the isr.
    //pip_int_detach();
    // lsptodo: odma port: odma_int_attach();

    return ret;
}


void pip_notify_stat_cdma_done(uint32_t cdma_perfid)
{
	//uint32_t stat_cdma_flag;

    // Determine which stat CDMA completed based on the callback context
    if (CDMA_AHS_0 == cdma_perfid)
    {
        //        stat_cdma_flag = PIP_STATA_FLAG;
    }
    else if (CDMA_AHS_1 == cdma_perfid)
    {
        // stat_cdma_flag = PIP_STATB_FLAG;
    }
    else
    {
        XASSERT(CDMA_AHS_2 == cdma_perfid, cdma_perfid);
        // stat_cdma_flag = PIP_STATC_FLAG;
    }

    // Set the event flag for the stat CDMA that completed
    // lsptodo: tx_res = tx_event_flags_set(&pip_stat_flags,stat_cdma_flag,TX_OR);
    //    ASSERT(TX_SUCCESS == tx_res);
}

void pip_notify_pa_stat_cdma_done(uint32_t cdma_perfid)
{
    XASSERT(CDMA_PIPE_ACCESS == cdma_perfid, cdma_perfid);

    // Set the event flag for the stat CDMA that completed
    //lsptodo    tx_res = tx_event_flags_set(&pip_stat_flags,PIP_PA_STAT_FLAG,TX_OR);
    // ASSERT(TX_SUCCESS == tx_res);
}

void pip_enable_pa_store(uint32_t mode)
{
    XASSERT(pip_cfg->cfg_mode == mode, mode);
    // Set PIPE_ACCESS and PA_DMA config structures to do a store
    pip_cfg->pipe_access_cfg = &pipe_access_store_6chan_cfg;

    pip_cfg->pa_odma_cfg = &raster_odma_8_bpp_cfg;

    // Turn off pip stats
    pip_cfg->stat_cfg = NULL;
}


/*----------------------------------------------------------------------
 * pip_pastore_rows() - Generate the next nlines output rows for
 *   PIPE_ACCESS store operation.
 *
 *----------------------------------------------------------------------
 */
uint32_t pip_pastore_rows(uint32_t *outbuf, uint32_t nlines, uint32_t width, uint32_t channels)
{
    uint32_t first, last;
    //uint32_t flags;
    //uint32_t tx_res;
    uint32_t store_width_in_bytes;
    error_type_t ret;

    first = !pip_pastore_linecount;

    pip_pastore_last += nlines;
    last = (pip_pastore_last >= pip_pastore_total_lines);

    DBG_PRINTF_DEBUG_M("pip_pastore_rows(): lines %d, width %d\n",nlines,width);

    // Setup the PA_DMA Channel
    ASSERT(NULL != pip_cfg->pipe_access_cfg);
    ASSERT(NULL != pip_cfg->pa_odma_cfg);  // Can't run without config info

    // The generic ODMA routines we're using for PA_ODMA processing
    //   are setup for max 32 bits per pixel. We end up with 60 bits
    //   per pixel for PA_STORE mode, so we're multiplying image_width
    //   by 2 to get 64 bpp.
    if (channels == 6)
    {
        store_width_in_bytes = width * 8;
    }
    else
    {
        store_width_in_bytes = width * 4;
    }

    pip_cfg->pa_odma_cfg->line_reverse = false;
    ret = raster_odma_set_cfg(pa_odma_handle, pip_cfg->pa_odma_cfg);
    XASSERT(OK == ret, ret);
    ret = raster_odma_add_input_buffer(pa_odma_handle,
                                       (char *)outbuf,
                                       store_width_in_bytes,
                                       nlines,
                                       first, // SOI
                                       last,  // EOI
                                       true,  // stop_on_finish
                                       &pip_pa_raster_odma_user_data);
    XASSERT(OK == ret, ret);

    ret = raster_odma_start_xfer(pa_odma_handle);
    XASSERT(OK == ret, ret);

    // Wait for PA ODMA to complete
    ODMA_WAIT( pa_odma_handle );
    //    tx_res = tx_event_flags_get(&pip_odma_flags,
    //                            1<<ODMA_PIPE_ACCESS,
    //                            TX_AND_CLEAR,
    //                            &flags,
    //                            TX_WAIT_FOREVER);
    //    ASSERT(TX_SUCCESS == tx_res);

#if 0 //lsptodo stats
    // Start PA stat CDMA
    if (1 == pip_cfg->pipe_access_cfg->stats_en)
    {
        ASSERT(pip_cfg->pipe_access_cfg->stats_patch_height == nlines);     // make sure the pa stat swath height is equal to nlines
        ret = pa_stat_cfg_buffer(pa_stat_buf, 16*4,
                                 (uint32_t *) &pa_cdma_reg->DATA, CDMA_PIPE_ACCESS);
        ASSERT(OK == ret);
    }

    // Wait for PA stat CDMA to complete
    if (1 == pip_cfg->pipe_access_cfg->stats_en)
    {
        CDMA_WAIT( lsp_todo_get_cdma_handle() );
        //tx_res = tx_event_flags_get(&pip_stat_flags,
        //                             PIP_PA_STAT_FLAG,
        //                           TX_AND_CLEAR,
        //                            &flags,
        //                            TX_WAIT_FOREVER);
        ASSERT(TX_SUCCESS == tx_res);
        pa_stat_cdma_close();

        // lsptodo: bigbuffer
        //        dma_buffer_unmap_single( pa_stat_bb, DMA_FROM_DEVICE);
        // cpu_dcache_invalidate_region(pa_stat_buf, 16*4);
#if 0
        int i;
        DBG_PRINTF_NOTICE("pastore stats line:%u ", pip_pastore_linecount);
        for (i=0; i<16; i++)
        {
            DBG_PRINTF_NOTICE("%08x ", pa_stat_buf[i]);
        }
        DBG_PRINTF_NOTICE("\n");
#endif

//        pa_read_stats(pa_stat_buf, smin, smax);
    }
    else
    {
    // If stats are disabled, assume no whitespace.
//        *smin = 0;
//        *smax = iw_out;
    }
#endif // lsptodo
    pip_pastore_linecount += nlines;

    return(1);
}


/*----------------------------------------------------------------------
 * pip_paload_rows() - Read the next nlines output rows for
 *   PIPE_ACCESS load operation.
 *
 *----------------------------------------------------------------------
 */
uint32_t pip_paload_rows(uint32_t *inbuf, //< hwaddress
                         uint32_t nlines, uint32_t width, uint32_t channels)
{
    uint32_t first, last;

    first = !pip_paload_linecount;

    pip_paload_last += nlines;
    last = (pip_paload_last >= pip_paload_total_lines);

    DBG_PRINTF_DEBUG_M(" pip_paload_rows(): lines %d, width %d\n",nlines,width);

    // Setup the PA_DMA Channel
    ASSERT(NULL != pip_cfg->pa_idma_cfg);  // Can't run without config info
    pip_cfg->pa_idma_cfg->image_width = width;
    pip_cfg->pa_idma_cfg->image_height = nlines;


    if (6 == channels)
    {
        idma_setup_strip(pa_idma_reg, idma_descriptor_v, idma_descriptor_hw, (uint32_t) inbuf,
                         pip_cfg->pa_idma_cfg->image_width * pip_cfg->pa_idma_cfg->image_height * 8,
                         pip_cfg->pa_idma_cfg->image_width*2,first,last);
    }
    else if (4 == channels)
    {
        idma_setup_strip(pa_idma_reg, idma_descriptor_v, idma_descriptor_hw, (uint32_t) inbuf,
                         pip_cfg->pa_idma_cfg->image_width * pip_cfg->pa_idma_cfg->image_height * 4,
                         pip_cfg->pa_idma_cfg->image_width,first,last);
    }
    else
    {
        ASSERT(0);  // Unrecognized channels
    }

    idma_start(pa_idma_reg, (uint32_t)idma_descriptor_hw);

    // Wait for PA IDMA to complete
    //    tx_res = tx_event_flags_get(&pip_idma_flags,PIP_IP_PA_IRQ_MASK,TX_AND_CLEAR,&flags,TX_WAIT_FOREVER);
    //    ASSERT(TX_SUCCESS == tx_res);

    pip_paload_linecount += nlines;

    return(1);
}

error_type_t pip_send_message( pip_message_t *message)
{
    // This will likely be called from interrupt context, so use TX_NO_WAIT
    // lsptodo: priority ?
    posix_message_send(pip_inbox, (void*)message, sizeof(pip_message_t), MQ_DEFAULT_PRIORITY, POSIX_WAIT_FOREVER);

    return OK;
}

static BigBuffer_t *pip_davbuf_serp_bigbuffer = NULL;
static BigBuffer_t *pip_davbuf_ued_bigbuffer = NULL;
static BigBuffer_t *pip_davbuf_kmod_bigbuffer = NULL;

static void pip_setup_davbuffers(uint32_t padded_width, uint32_t scaled_width)
{
    uint32_t pix_bursts = 0;
    uint32_t beats_per_pix_burst = 0;
    uint32_t pip_davbuf_serp_alloc_size;
    uint32_t pip_davbuf_ued_alloc_size;
    uint32_t pip_davbuf_kmod_alloc_size;
    void *hwaddr;

    pip_free_davbuffers();

    if (NULL != pip_cfg->serp_cfg)
    {
        if (padded_width > DAVBUF_PIXWIDTH_SERP)
        {
            ASSERT(NULL != pip_cfg->db_serp_cfg);
            pix_bursts = (uint32_t)ceil((float)(padded_width) / DAVBUF_PIXBURST);
            beats_per_pix_burst = (uint32_t)ceil(((float)DAVBUF_PIXBURST*DAVBUF_BITS_PER_PIXEL) / DAVBUF_BUSWIDTH);
            pip_davbuf_serp_alloc_size = pix_bursts * beats_per_pix_burst * 4;
            pip_davbuf_serp_bigbuffer = dma_buffer_malloc(0, pip_davbuf_serp_alloc_size);
            ASSERT(NULL != pip_davbuf_serp_bigbuffer);
            //memset(pip_davbuf_serp_alloc, 0x0, pip_davbuf_serp_alloc_size);

            pip_cfg->db_serp_cfg->imagewidth = padded_width;
            pip_cfg->db_serp_cfg->pixbursts = pix_bursts;
            pip_cfg->db_serp_cfg->beatsperpixburst = beats_per_pix_burst;
            pip_cfg->db_serp_cfg->mode = DAVBUF_NORMAL;
            hwaddr = dma_buffer_map_single(pip_davbuf_serp_bigbuffer, DMA_FROM_DEVICE);
            pip_cfg->db_serp_cfg->buffer = (uint32_t)hwaddr;
            pip_cfg->db_serp_cfg->buffer_end = (uint32_t)(hwaddr + pip_davbuf_serp_alloc_size);

            davbuf_cfg_driver((DB_REGS_t *) db_serp_reg, pip_cfg->db_serp_cfg);
        }
    }

    if ( (NULL != pip_cfg->ued_cfg) ||
         (NULL != pip_cfg->th_cfg) )
    {
        if (padded_width > DAVBUF_PIXWIDTH_UED)
        {
            ASSERT(NULL != pip_cfg->db_ued_cfg);
            pix_bursts = (uint32_t)ceil((float)(scaled_width) / DAVBUF_PIXBURST);
            beats_per_pix_burst = (uint32_t)ceil(((float)DAVBUF_PIXBURST*DAVBUF_BITS_PER_PIXEL) / DAVBUF_BUSWIDTH);
            pip_davbuf_ued_alloc_size = pix_bursts * beats_per_pix_burst * 4;
            pip_davbuf_ued_bigbuffer = dma_buffer_malloc(0, pip_davbuf_ued_alloc_size);
            ASSERT(NULL != pip_davbuf_ued_bigbuffer);
            // do I really need to memset this?
            //memset(pip_davbuf_ued_alloc, 0x0, pip_davbuf_ued_alloc_size);

            pip_cfg->db_ued_cfg->imagewidth = scaled_width;
            pip_cfg->db_ued_cfg->pixbursts = pix_bursts;
            pip_cfg->db_ued_cfg->beatsperpixburst = beats_per_pix_burst;
            pip_cfg->db_ued_cfg->mode = DAVBUF_NORMAL;
            hwaddr = dma_buffer_map_single( pip_davbuf_ued_bigbuffer, DMA_FROM_DEVICE );
            pip_cfg->db_ued_cfg->buffer = (uint32_t)hwaddr;
            pip_cfg->db_ued_cfg->buffer_end = (uint32_t)(hwaddr + pip_davbuf_ued_alloc_size);

            davbuf_cfg_driver((DB_REGS_t *) db_ued_reg, pip_cfg->db_ued_cfg);
        }
    }

    if (NULL != pip_cfg->kmod_cfg)
    {
        if (padded_width > DAVBUF_PIXWIDTH_KMOD)
        {
            ASSERT(NULL != pip_cfg->db_kmod_cfg);
            pix_bursts = (uint32_t)ceil((float)(scaled_width) / DAVBUF_PIXBURST);
            beats_per_pix_burst = (uint32_t)ceil(((float)DAVBUF_PIXBURST*DAVBUF_BITS_PER_PIXEL) / DAVBUF_BUSWIDTH);
            pip_davbuf_kmod_alloc_size = pix_bursts * beats_per_pix_burst * 4;
            pip_davbuf_kmod_bigbuffer = dma_buffer_malloc(0, pip_davbuf_kmod_alloc_size);
            ASSERT(NULL != pip_davbuf_kmod_bigbuffer);
            //memset(pip_davbuf_kmod_alloc, 0x0, pip_davbuf_kmod_alloc_size);

            pip_cfg->db_kmod_cfg->imagewidth = scaled_width;
            pip_cfg->db_kmod_cfg->pixbursts = pix_bursts;
            pip_cfg->db_kmod_cfg->beatsperpixburst = beats_per_pix_burst;
            pip_cfg->db_kmod_cfg->mode = DAVBUF_NORMAL;
            hwaddr = dma_buffer_map_single( pip_davbuf_ued_bigbuffer, DMA_FROM_DEVICE );
            pip_cfg->db_kmod_cfg->buffer = (uint32_t)hwaddr;
            pip_cfg->db_kmod_cfg->buffer_end = (uint32_t)(hwaddr + pip_davbuf_kmod_alloc_size);

            davbuf_cfg_driver((DB_REGS_t *) db_kmod_reg, pip_cfg->db_kmod_cfg);
        }
    }
}

static void pip_free_davbuffers(void)
{
    if (NULL != pip_davbuf_serp_bigbuffer)
    {
        BigBuffer_Free( pip_davbuf_serp_bigbuffer );
    }
    if (NULL != pip_davbuf_ued_bigbuffer)
    {
        BigBuffer_Free( pip_davbuf_ued_bigbuffer );
    }
    if (NULL != pip_davbuf_kmod_bigbuffer)
    {
        BigBuffer_Free( pip_davbuf_kmod_bigbuffer );
    }
}



void *pip_bb_output_strip_alloc( uint32_t i, 
				 uint32_t input_line_size_in_pixels,
				 uint32_t x_scale,
				 uint32_t out_height,
				 uint32_t output_bpp,
				 BigBuffer_t *bb[6]
    )
{
    static mlimiter_t* limiter = NULL;
    uint32_t output_buffer_size;	
    uint32_t strip_size_in_pixels;
    uint32_t out_line_size_in_pixels;
    BigBuffer_t* output_buffer;	
    out_line_size_in_pixels = pip_padded_image_width(input_line_size_in_pixels, x_scale);
    strip_size_in_pixels = out_line_size_in_pixels * out_height;
    output_buffer_size = ((strip_size_in_pixels * output_bpp)/8);

    if (!limiter) 
	limiter = mlimiter_by_name("print");

    DBG_PRINTF_DEBUG("pip alloc before %p i%d\n", bb[i], i);
    output_buffer = dma_buffer_malloc(limiter, output_buffer_size);
    if (!output_buffer) 
	return 0;
    static int odd = 0;
    if (0) {   // debug fill of output buffer

       	   dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
           dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
           void *v = dma_buffer_mmap_forcpu(output_buffer);
           if ( odd++ & 1 ) {
               memset( v, 0x00, output_buffer_size );
           } else {
               memset( v, 0xff, output_buffer_size );
           }
           dma_buffer_unmmap_forcpu( output_buffer);
    }
    
    bb[i] = output_buffer;
    DBG_PRINTF_DEBUG("pip alloc after %p\n", bb[i]);
    return dma_buffer_map_single( bb[i], DMA_FROM_DEVICE );				
}

/**
 *
 * See pip_api.h for details.
 *
**/
int pip_process_mono_strip(uint32_t mode,
                            BigBuffer_t *bb_in,
                            uint32_t width_pix_in,
                            uint32_t lines_in,
                            uint32_t lines_out,
			   uint32_t output_bpp,
                            uint32_t page_height,
                            uint32_t scale_x,
                            uint32_t scale_y,
                            bool start_of_image,
                            bool end_of_image,
                            BigBuffer_t *bb_out)
{
    BigBuffer_t *bb[6] = {0,0,0,0,0,0};
    bb[4] = bb_out;

    return pip_process_strip(mode, bb_in, width_pix_in, lines_in, lines_out, output_bpp, 
			     page_height, scale_x, scale_y, start_of_image, end_of_image, bb );
}



int pip_process_strip(uint32_t mode,
                       BigBuffer_t *bb_in,
                       uint32_t width_pix_in,
                       uint32_t lines_in,
                       uint32_t lines_out,
		       uint32_t output_bpp,
                       uint32_t page_height,
                       uint32_t scale_x,
                       uint32_t scale_y,
                       bool start_of_image,
                       bool end_of_image,
                       BigBuffer_t *bb_out[6])

{
    int i;
    static uint32_t *output_planes[6];  // hwaddress for bb_out allocated on demand here.
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    static uint32_t page_lines_received = 0; // unscaled
    static uint32_t strip_lines_in = 0;   // scaled
    static uint32_t strip_lines_out = 0;  // scaled
    static int start_next = 0;
    BigBuffer_t *dummy_bb = 0;  // extra pad data used to flush pip in the absence of a EOI from zx.
    int dummy_size;
    int freed = 0;
    
    if ( start_of_image ) {
	strip_lines_in = 0;
	strip_lines_out = 0;
        page_lines_received = 0;
	start_next = 0;
	DBG_PRINTF_NOTICE("pip_process_strip - line_len=%u rows=%u\n", width_pix_in, lines_out);
    }
    if ( bb_in ) {
	page_lines_received += lines_in;
	if (end_of_image)
	{
	    // When doing a copy we often get an initial page size that is much longer than
	    // the true page size.  When re-scaling the block buffers a single line
	    // and does not flush until it receives the last input line of the image.
	    // So, on the last strip we reset the page height to be the actual number
	    // of lines in the image.  This causes zx to issue the EOI and flush the
	    // pipe and all is well in the pip imaging world.
	    page_height = page_lines_received;
	}
	if (!pip_cfg->mono_nColor) { // color not K only.
	    zx_idma_add_bigbuffer_rgb( bb_in, width_pix_in, lines_in );
	} else {
	    zx_idma_add_bigbuffer_mono( bb_in, width_pix_in, lines_in );
	}
	if ( start_next == 1 ) {
	    zx_idma_start( width_pix_in, page_height ); // delay idma till second input buffer
	}

	start_next++;
	strip_lines_in += lines_in * scale_y;
    }
    internal_buffering_lines_needed = 16;
    DBG_PRINTF_DEBUG("pip after add %d in %d out %d scale %d ibneeded %d\n", 
		     strip_lines_in, lines_in, lines_out, scale_y, internal_buffering_lines_needed);
    if ( strip_lines_in > (lines_out + internal_buffering_lines_needed) || end_of_image ) {
	if ( end_of_image ) {
	    if ( bb_in ) {
		int extra = 16 ; // internal_buffering_lines_needed; // todo size optimization
		dummy_size = bb_in->datalen / lines_in * extra;
		dummy_bb = dma_buffer_malloc(mlimiter_by_name("print"), dummy_size ); 
		dma_buffer_map_single(dummy_bb, DMA_FROM_DEVICE);
		dma_buffer_unmap_single(dummy_bb, DMA_FROM_DEVICE);
		char * p = dma_buffer_mmap_forcpu(dummy_bb);
		memset( p, 0x80, dummy_size );
		dma_buffer_unmmap_forcpu(dummy_bb);
		zx_idma_add_bigbuffer_mono( dummy_bb, width_pix_in, extra ); // add extra to flush.
		strip_lines_in += extra; 
		lines_out = page_height * scale_y - strip_lines_out;  // compute size of last buffer
		lines_out = lines_out > 128 ? 128 : lines_out;
		DBG_PRINTF_DEBUG("extra %d linesout %d size %d internal_needed %d\n", 
				 extra, lines_out, dummy_size, internal_buffering_lines_needed);
	    } else {
		DBG_PRINTF_DEBUG("pip eoi %d %d \n", 
				 strip_lines_out, page_lines_received );

		if ( strip_lines_out >= page_lines_received * scale_y ) {
		    DBG_PRINTF_WARNING("pip eoi %d %d out ZERO %d \n", 
				       strip_lines_out, page_lines_received * scale_y, lines_out );
		    return 0; // eoi done
		} else {
		    lines_out = strip_lines_in - 16;  // eoi flush 
		    lines_out = lines_out > 128 ? 128 : lines_out;  // video max strip height
		    lines_out = lines_out < 2 ? 2 : lines_out;      // video min function of number of beams
		    DBG_PRINTF_WARNING("pip eoi %d %d out %d \n", 
				       strip_lines_out, page_lines_received * scale_y, lines_out );
		}
	    }
	}
	if (pip_cfg->mono_nColor) { 
	    output_planes[4] = pip_bb_output_strip_alloc( 4, width_pix_in, scale_x, lines_out, output_bpp, bb_out );
	    output_planes[0] = 0; 
	    output_planes[1] = 0; 
	    output_planes[2] = 0; 
	} else {	
	    output_planes[0] = pip_bb_output_strip_alloc( 0, width_pix_in, scale_x, lines_out, output_bpp, bb_out );
	    output_planes[1] = pip_bb_output_strip_alloc( 1, width_pix_in, scale_x, lines_out, output_bpp, bb_out );
	    output_planes[2] = pip_bb_output_strip_alloc( 2, width_pix_in, scale_x, lines_out, output_bpp, bb_out );
	    output_planes[4] = pip_bb_output_strip_alloc( 4, width_pix_in, scale_x, lines_out, output_bpp, bb_out );
	}
	start_of_image = (strip_lines_out == 0); // move SOI from first input strip to first output strip. 
	strip_lines_out += lines_out;
	pip_generate_rows(output_planes, lines_out, &combined_stats, chan_stats, 
			  start_of_image, end_of_image, strip_lines_out);
	pip_block_output( );
        freed = zx_idma_free_input( lines_out / scale_y, start_of_image, end_of_image );  // free dummy_bb on next page, 
	strip_lines_in -= lines_out;  
	DBG_PRINTF_DEBUG("pip after free input %d out %d %d freed totalout %d\n", 
			 strip_lines_in, lines_out, freed, strip_lines_out );
	for (i = 0; i<6; i++) {
	    if (bb_out[i]) {
		dma_buffer_unmap_single(bb_out[i], DMA_FROM_DEVICE);
	    }
	}
	return lines_out;
    } 
    DBG_PRINTF_NOTICE("%s waiting for input strip_lines_in %d total received %d \n", 
		      __FUNCTION__, strip_lines_in, page_lines_received);
    return 0;
}
    
#if 0
static jbig_handle_t * jbig_decode_handle;
static void pip_setup_jbig(uint32_t Xd,
                           uint32_t Yd,
                           uint32_t L0,
                           uint32_t bpp)
{
    uint32_t bits_per_byte;


    XASSERT(0 == (Xd%32), Xd);              // Xd must be a multiple of 32
    XASSERT(1==bpp || 2==bpp || 4==bpp, bpp);
    bits_per_byte = 8 / bpp;

    if(jbig_open(&jbig_decode_handle, JBIG_DECODE_BLOCK) == FAIL)
    {
        DPRINTF((DBG_OUTPUT|DBG_LOUD),("JBIG TEST: FAILED DECODE block already in use.\n"));
        return;
    }

    jbig_set_decode(jbig_decode_handle);
    jbig_enable_cbi(jbig_decode_handle);
    jbig_set_x_dimension(jbig_decode_handle, Xd);
    jbig_set_y_dimension(jbig_decode_handle, Yd);
    jbig_enable_bypass(jbig_decode_handle);
}


error_type_t pip_start_jbig(uint8_t *data,
                            uint32_t source_buffer_size)
{
    error_type_t ret = FAIL;

    ret = jbig_add_input_buffer(jbig_decode_handle, data, source_buffer_size, NULL);

    if (OK == ret)
    {
        ret = jbig_start_transaction(jbig_decode_handle, jbig_decode_cb);
        //cpu_spin_delay(10); //lsptodo: why waste cpu cycles.
    }

    return ret;
}

error_type_t pip_add_jbig_buffer(uint8_t *data,
                                 uint32_t source_buffer_size)
{
    error_type_t ret = FAIL;

    //    XASSERT(0==(((uint32_t)data)&0x1f), (uint32_t)data); // already hw mapped can't do this

    ret = jbig_add_input_buffer(jbig_decode_handle, data, source_buffer_size, NULL);

    // lsptodo why? // cpu_spin_delay(10);

    return ret;
}

static error_type_t pip_stop_jbig(void)
{
    error_type_t ret = FAIL;

    ret = jbig_stop_transaction( jbig_decode_handle );

    if (OK == ret)
    {
        ret = jbig_close(jbig_decode_handle);
    }

    return ret;
}

static void pip_jbig_callback(void *user_data, uint32_t int_status)
{
    uint32_t status = int_status & JBIG_EVENT_STATUS_MASK;
    UINT result;

    if( int_status & JBIG_ODMA_COMPLETE_FLAG )
    {
        //DPRINTF((DBG_OUTPUT|DBG_LOUD),("jbig test: Pix callback data = %x status = %x\n", user_data, status));
    }

    if( int_status & JBIG_IDMA_COMPLETE_FLAG )
    {
        //DPRINTF((DBG_OUTPUT|DBG_LOUD),("jbig test: IDMA callback status = %x\n", status));
    }

    if( int_status & JBIG_CONTROL_FLAG )
    {
        //DPRINTF((DBG_OUTPUT|DBG_LOUD),("jbig test: Control callback status = %x\n", status));

        // signal the client thread that the operation is complete
        result = tx_event_flags_set(&pip_jbig_codec_events,
                                   JBIG_CODEC_OPERATION_COMPLETE,
                                   TX_OR);
        XASSERT(result == TX_SUCCESS, result);

        if( status & JBIG_JSTAT_DD_MASK)
        {
           DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_DD_MASK\n"));
        }

        if( status & JBIG_JSTAT_ED_MASK)
        {
           DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_ED_MASK\n"));
        }

        if( status & JBIG_JSTAT_V_MASK)
        {
           //DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_V_MASK\n"));
        }

        if( status & JBIG_JSTAT_AR_MASK)
        {
           //DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_AR_MASK %x\n", status & JBIG_JSTAT_AR_MASK));
        }

        if( status & JBIG_JSTAT_I_MASK)
        {
           DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_I_MASK %x\n", status & JBIG_JSTAT_I_MASK));
        }

        if( status & JBIG_JSTAT_D_MASK)
        {
           DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_D_MASK\n"));
        }

        if( status & JBIG_JSTAT_E_MASK)
        {
           DPRINTF((DBG_OUTPUT|DBG_LOUD),("\t JBIG_JSTAT_E_MASK = %x \n", status & JBIG_JSTAT_E_MASK));
        }
    }
}
#endif
static void pip_disable_odma_channel(uint32_t channel)
{
    return ; // lsptodo gemstone only....

    switch (channel)
    {
        case (0):
            pip_reg_u->CFG = PIP_CFG_DMA0_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (1):
            pip_reg_u->CFG = PIP_CFG_DMA1_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (2):
            pip_reg_u->CFG = PIP_CFG_DMA2_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (3):
            pip_reg_u->CFG = PIP_CFG_DMA3_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (4):
            pip_reg_u->CFG = PIP_CFG_DMA4_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (5):
            pip_reg_u->CFG = PIP_CFG_DMA5_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (6):
            pip_reg_u->CFG = PIP_CFG_DMA6_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
        case (7):
            pip_reg_u->CFG = PIP_CFG_DMA7_EN_REPLACE_VAL(pip_reg_u->CFG,0);
            break;
    }
}

#if 0 // use block on semaphore instead
static void pip_odma_data_buf_complete(void *user_data)
{
    dma_config_data_t *data;

    ASSERT(NULL != user_data);
    data = (dma_config_data_t *)user_data;
    DBG_PRINTF_DEBUG_H("%s 0x%x, chan:%u\n", __func__, user_data, data->channel);



    // Set ODMA Flag
    // lsptodo: semaphore
    //tx_res = tx_event_flags_set(&pip_odma_flags, 1<<data->channel, TX_OR);
    //XASSERT(TX_SUCCESS == tx_res, tx_res);
}
#endif

static void *pip_thread_func( void *input )
{
    pip_message_t msg;
    error_type_t ret;
    int iret;

    while (1)
    {
        // Wait for a message, then call the registered callback function.
        iret = posix_wait_for_message(pip_inbox, (char*)&msg, sizeof(pip_message_t), POSIX_WAIT_FOREVER);
        if (iret != 0)
            continue;  // signal interrupt

        DBG_PRINTF_DEBUG("*** pip thread MSG %d\n", msg.msg);
        switch (msg.msg)
        {
        case RASTER_ODMA_DESC_COMPLETE:
            DBG_PRINTF_DEBUG("PIP ODMA DESC COMPLETE desc 0x%x\n",  msg.descriptor);
            DBG_MEMLOG_DEBUG("PIP ODMA DESC COMPLETE desc 0x%x\n",  msg.descriptor);
            raster_odma_release_descriptors_to_available_list(msg.descriptor, msg.chan);
            break;

        case RASTER_ODMA_OWN_ERROR:
            DBG_MEMLOG_CRIT("*** PIP ODMA ownership ERROR!  desc 0x%x\n",  msg.descriptor);
            DBG_PRINTF_CRIT("*** PIP ODMA ownership ERROR!  desc 0x%x\n",  msg.descriptor);
            raster_odma_handle_ownership_error(msg.chan);
            raster_odma_release_descriptors_to_available_list(msg.descriptor, msg.chan);

            // Toggle the enable bit when the ownership error occurs to cause a
            // soft reset of the block, per hardware documentation.
            // raster_odma_soft_reset(msg.chan);

            ret = raster_odma_start_xfer( (odma_handle_t *)msg.value );
            XASSERT(OK == ret, ret);
            break;

        case RASTER_ODMA_EOI_ALIGNMENT_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA END OF IMAGE ALIGNMENT ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA EOI ALIGNMENT ERROR!!!!\n");
            //ASSERT(0);
            break;

        case RASTER_ODMA_EOL_ALIGNMENT_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA END OF LINE ALIGNMENT ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA END OF LINE ALIGNMENT ERROR!!!!\n");
            //ASSERT(0);
            break;

        case RASTER_ODMA_EOI_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA END OF IMAGE ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA END OF IMAGE ERROR!!!!\n");
            //ASSERT(0);
            break;

        case RASTER_ODMA_CL_ALIGNMENT_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA CHANGE LINE ALIGNMENT ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA CHANGE LINE ALIGNMENT ERROR!!!!\n");
            //ASSERT(0);
            break;

        case RASTER_ODMA_DIRECTION_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA DIRECTION ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA DIRECTION ERROR!!!!\n");
            //ASSERT(0);
            break;

        case RASTER_ODMA_ERROR:
            DBG_MEMLOG_WARNING("*** PIP ODMA GENERIC ERROR!!!!\n");
            DBG_PRINTF_WARNING("*** PIP ODMA GENERIC ERROR!!!!\n");
            ASSERT(0);
            break;

        case RASTER_ODMA_SOFT_RESET_COMPLETED:
            raster_odma_soft_reset_complete(msg.chan);
            DBG_PRINTF_DEBUG("*** PIP ODMA Soft reset completed\n");
            break;

        case RASTER_ODMA_END_OF_IMAGE:
            DBG_PRINTF_DEBUG("*** PIP ODMA END OF IMAGE\n");
            break;

        case RASTER_ODMA_ADD_INPUT_BUFFER:
            // Currently shouldn't be getting here.
            ASSERT(0);
            break;

        default:
            DBG_PRINTF_WARNING("*** DEFAULT HANDLER. MSG %d\n", msg.msg);
            ASSERT(0);
            break;

        } // switch
    } //while
    return 0; // won't get here.
}

