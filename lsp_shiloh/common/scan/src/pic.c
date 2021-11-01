/*
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file pic.c
 *
 * \brief Interface functions to PIC
 *
 * PIC - Pipelined Image Correction.  PIC's goal in life is to make an image as
 * accurate as possible.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "list.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "lassert.h"
#include "hwconfig_api.h"
#include "cpu_api.h"
#include "regAddrs.h"
#include "io.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "safetylock.h"
#include "scantask.h"
#include "scanhw.h"
#include "pichw.h"
#include "icedma.h"
#include "pic.h"
#include "picreset.h"
#include "piccbi.h"
#include "afe.h"
#include "scanalyzer.h"
#include "safeint.h"
#include "PRNUDSNU_CORR_regmasks.h"
#include "PRNUDSNU_CORR_regstructs.h"
#include "PIC_IntCtrl_regmasks.h"
#include "PIC_IntCtrl_regstructs.h"
#include "scanvars.h"
#include "scansen.h"
#include "scanlib.h"
#include "scanif.h"

/* Define to turn on more verbose debugging */
//#define PIC_DEBUG  

#ifdef PIC_DEBUG
  #define pic_dbg2 dbg2 
#else
  #define pic_dbg2(...) 
#endif

/* Fields for PDCR2 COEFFWIDTH. Keep private to this file. Used inside
 * pic_prnudsnu_set_coeffwidth()  
 */
#define PIC_PC_PDCR2_COEFFWIDTH_12BITS 0
#define PIC_PC_PDCR2_COEFFWIDTH_16BITS 1
#define PIC_PC_PDCR2_COEFFWIDTH_20BITS 2
#define PIC_PC_PDCR2_COEFFWIDTH_24BITS 3
#define PIC_PC_PDCR2_COEFFWIDTH_28BITS 4
#define PIC_PC_PDCR2_COEFFWIDTH_32BITS 5

/* davep 31-Jan-2013 ; they changed the names again */
#ifdef ICE_PIC_PRNUDSNU_S_BASE 
#define ICE_PIC_PRNUDSNU_BASE ICE_PIC_PRNUDSNU_S_BASE 
#endif

/* davep 08-Jan-2013 ; porting to new regbuild register headers */
static volatile PRNUDSNU_CORR_REGS_t * const prnudsnu_regs_array[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (volatile PRNUDSNU_CORR_REGS_t *)(ICE_PIC_PRNUDSNU_PRNUDSNU_CORR0_BASE ),
    (volatile PRNUDSNU_CORR_REGS_t *)(ICE_PIC_PRNUDSNU_PRNUDSNU_CORR1_BASE ),
#else
    (volatile PRNUDSNU_CORR_REGS_t *)(ICE_PIC_PRNUDSNU_BASE ),
#endif
    0 /* end of list */
};

/* davep 15-Jan-2013 ; make sure the dual scan ASIC's first (default)
 * PRNU/DSNU block is at the same address as single scan ASIC's PRNU/DSNU (will
 * make the code simpler--we can initialize prnudsnu_regs to a sane default at
 * compile time)
 */
#ifdef HAVE_NSENSOR_SUPPORT
    #if ICE_PIC_PRNUDSNU_PRNUDSNU_CORR0_BASE != ICE_PIC_PRNUDSNU_BASE
        #error prnudsnu header file address error
    #endif
#endif

static volatile PRNUDSNU_CORR_REGS_t * prnudsnu_regs = (volatile PRNUDSNU_CORR_REGS_t *)(ICE_PIC_PRNUDSNU_BASE);

static volatile PIC_INTCTRL_REGS_t * const pic_ci_regs = (volatile PIC_INTCTRL_REGS_t *)(ICE_PIC_PIC_INTCTRL_BASE);
static int current_block_num=0;

extern int dbg_cnt_pic_interrupt;	//add.lsh.for pipe debug
extern void scan_cmdq_emergency_halt( void );
extern void print_pipe_dgb_cnt( void );
struct pic_interrupt_stats pic_interrupt_stats;

/*
 * Start of PIC Interrupt code
 *
 */

#ifdef __KERNEL__
static irqreturn_t pic_interrupt( int irq, void *dev_id ) 
#else
static void pic_interrupt( uint32_t param )
#endif
{
//    int i;
    uint32_t pic_ipend, adc_ipend;

    /* 
     * BIG FAT NOTE!  This is an interrupt handler
     */
	dbg_cnt_pic_interrupt++;
    pic_interrupt_stats.count++;

    /* read PIC's pending interrupts */
    pic_ipend = pic_ci_regs->IPR;

    SCANALYZER_LOG( LOG_PIC_INTERRUPT, pic_ipend );

    /* read the ADC's pending interrupts */
#ifdef PIC_ADCN_ANIP
    adc_ipend = *PIC_ADCN_ANIP;
#else
    adc_ipend = *PIC_ADCN_ANI;
#endif

//    dbg2( "%s %#x %#x\n", __FUNCTION__, pic_ipend, adc_ipend );

    /* track overruns (usually an error condition) */
    if( pic_ipend & PIC_CI_COMBUSOI_BIT ) {
        dbg1( "ERROR! %s OVERFLOW pic_ipend=%#x adc_ipend=%#x overflow:%d\n", 
                    __FUNCTION__, pic_ipend, adc_ipend, pic_interrupt_stats.combus_overrun );

        /* as of 23-dec-04, the only overrun is from ADC */
        pic_interrupt_stats.combus_overrun++;

//        scif_dump();
//        cisx_dump();

        /* davep 28-Mar-2013 ; let's try to extract ourselves to userspace so
         * we can get more debugging without crashing 
         */
        scif_interrupt_disable();
        pic_interrupt_disable();
        scif_clock(0);
        scif_control(0,0);
        scan_cmdq_emergency_halt();
#if 0
        XASSERT( 0, pic_ipend );
#else
		extern bool bScanEC;
		if( bScanEC == false )
			scanlib_send_sc(SMSG_SC_PIC_OVF_FAIL);
		return IRQ_HANDLED;
#endif
    }

    /* davep 04-Oct-2010 ; moving to a platform neutral PIC WDMA interrupt
     * handler */
    /* TODO check ipend for DMA interrupt, only call if WDMA interrupt pending.
     * For now, unconditionally call since good PIC interrupts are always DMA
     * anyway. ("Bad" interrupts would be overflows.)
     */
    pic_wdma_interrupt();

    /* ADC Norm block overrun interrupt? */
    if( adc_ipend & PIC_ADCN_ANI_OVERRUN) {
        pic_interrupt_stats.adc_overrun++;
        /* ACK it to make it go away */
        *PIC_ADCN_ANA = PIC_ADCN_ANA_OVERRUN;

        /* davep 19-Feb-2008 ; print out the pending instead of my stupid
         * constant 
         */
        XASSERT( 0, adc_ipend );
    }

#ifdef HAVE_PIC_PSESD
    if( pic_ipend & PIC_CI_PS_ESD ) {
    	pic_pgesd_interrupt();
    }
#endif

    /* time stamp our exit */
    SCANALYZER_LOG( LOG_PIC_INTERRUPT, 0xffff0000 );

#ifdef __KERNEL__
    return IRQ_HANDLED;
#endif
}

uint32_t pic_interrupt_disable( void )
{
    uint32_t was_enabled;

    was_enabled = pic_ci_regs->IENR && 1;

    /* disable everything */
    pic_ci_regs->IENR = 0;
    return was_enabled;
}

void pic_interrupt_enable( void )
{
    /* re-enable everything */
    /* davep 02-Apr-2013 ; enable only overflow & WDMA_Common (combine WDMA
     * interrupts together to reduce ISR calls)
     */
    pic_ci_regs->IENR = PIC_INTCTRL_IENR_WDMA_COMMON_MASK 
                      | PIC_INTCTRL_IENR_OVERFLOW_MASK 
                      ;
//    pic_ci_regs->IENR = ~0;
}

void pic_setup_interrupt( void )
{
    /* setup PIC interrupts; enable everything */
    pic_interrupt_enable();

    /* clear all interrupts */
    *PIC_ADCN_ANA = ~0;

    *PIC_ADCN_ANI = PIC_ADCN_ANI_OVERRUN;
}

/*
 * End of PIC Interrupt code
 *
 */

/**
 * \brief the PIC INTCTRL struct has the same register under two different names
 *
 * Grrr...
 *
 * \author David Poole
 * \date 10-Jan-2013
 */

volatile uint32_t *pic_ci_cbir( void )
{
#ifdef PIC_INTCTRL_CCBIR_SOFTRESET_MASK
    return &pic_ci_regs->CCBIR;
#else
    return &pic_ci_regs->PCR;
#endif
}

uint32_t pic_prnudsnu_get_block(void)
{
    return current_block_num;
}

/**
 * \brief swap global PRNU/DSNU registers between blocks
 *
 * Original idea is similar to Bank Switching.
 * http://en.wikipedia.org/wiki/Bank_switching
 *
 * Adding dual-sensor support without breaking all existing code. We will
 * swap the global pointer back/forth depending on which sensor we're
 * configuring.
 *
 *  pic_prnudsnu_set_block(0);
 *  calibrate sensor 0
 *  pic_prnudsnu_set_block(1);
 *  calibrate sensor 1
 *
 * \author David Poole
 * \date 15-Jan-2013
 */

void pic_prnudsnu_set_block( uint32_t block ) 
{
    uint32_t sensor_num;

    sensor_num = scansen_get_num_sensors();

    XASSERT(block < sensor_num, block );

    prnudsnu_regs = prnudsnu_regs_array[block];
    dbg2("%s prnudsnu_regs is %p\n",__FUNCTION__,prnudsnu_regs);
    current_block_num = block;
}

/**
 * \brief  Enable/disable dual scan mode
 *
 * \author David Poole
 * \date 15-Jan-2013
 */

scan_err_t pic_enable_dual_channel( bool enable )
{
#ifdef HAVE_NSENSOR_SUPPORT
    if( enable ) {
        /* turn on dual channel */
        pic_ci_regs->PCR |= PIC_INTCTRL_PCR_DUALCH_MASK; 
    }
    else {
        /* turn off dual channel */
        pic_ci_regs->PCR &= ~PIC_INTCTRL_PCR_DUALCH_MASK; 
    }
    return SCANERR_NONE;
#else
    return SCANERR_NOT_IMPLEMENTED;
#endif
}

/**
 * \brief  Get PIC Dual Channel enable/disable state
 *
 * Created for sanity checking. All of Scan block, CISX, and PIC must be in
 * agreement for dual channel mode.
 *
 * \author David Poole
 * \date 03-Apr-2013
 */

bool pic_get_dual_channel_enabled( void )
{
#ifdef HAVE_NSENSOR_SUPPORT
    return (pic_ci_regs->PCR & PIC_INTCTRL_PCR_DUALCH_MASK)==PIC_INTCTRL_PCR_DUALCH_MASK;
#else
    return false;
#endif
}

/*
 * Start of PIC hardware/software setup code.
 *
 */
//--------------------------------------------------------------------------
// Function     : pic_reset
//   returns    : 
//   arg1       : 
//   arg2       : 
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Resets the PIC block to its power-on default values.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void 
pic_reset( void )
{
    uint32_t num32;
    uint32_t pd_save_block_num, pd_dma_save_block_num;
    uint32_t i;

    /* restore PIC to power-on default settings */
    pic_dbg2( "%s\n", __FUNCTION__) ;

    /* 
     * Reset PIC Common Block
     */
    /* Clear PIC common block */
    *pic_ci_cbir() = PIC_CI_CBIR_SOFTRESET;
    cpu_spin_delay( 1 ); /* hold it for "a little while" (XXX-necessary?) */
    /* XXX 13-Jan-05 ; I'm still not sure how long to hold.  */
    /* davep 10-Jan-2013 ; To this day, I'm still still not sure how long to hold. */
    *pic_ci_cbir() = 0;

    pic_ci_regs->IENR = PIC_CI_IENR_R;

    /* 
     * Reset ADC
     */
    *PIC_ADCN_ANR = PIC_ADCN_ANR_R;
    /* 19-Feb-05 ; verify PIC is at least alive */
    num32 = *PIC_ADCN_ANR;
    XASSERT( num32 == PIC_ADCN_ANR_R, num32 );
    /* davep 15-Dec-2011 ; ack any and all pending interrupts */
    *PIC_ADCN_ANA = ~0;
//    *PIC_ADCN_ANT = PIC_ADCN_ANT_R;

    /* 
     * Reset Bulb Monitor block
     */
    pic_bm_reset();

    /* 
     * Reset Pixel Correction block
     */
    // we may have multiple pixel correction blocks nowadays.  Reset all of them
    
    // store away current block for the prnudsnu functions, reset all blocks, and restore when done
    pd_save_block_num = pic_prnudsnu_get_block();
    pd_dma_save_block_num = pic_prnudsnu_dma_get_block();    

    for (i=0;i<scansen_get_num_sensors();i++)
    {
        pic_prnudsnu_set_block(i);   // switch to the block to reset
        pic_prnudsnu_dma_set_block(i);    
        
        pic_prnudsnu_enable_lut_dma( false );
        pic_prnudsnu_reset_lut_dma();
        prnudsnu_regs->PDCR1 = PIC_PC_PDCR1_R;
        prnudsnu_regs->PDCR2 = PIC_PC_PDCR2_R;
        prnudsnu_regs->PONCR = PIC_PONC_R;  
    }
    pic_prnudsnu_set_block(pd_save_block_num);  // restoring both now....
    pic_prnudsnu_dma_set_block(pd_dma_save_block_num);    
    
    /* 
     * Reset H Scale block
     */
    *PIC_HS_HSCR = PIC_HS_HSCR_R;
    *PIC_HS_HSSR = PIC_HS_HSSR_R;

    /*
     * Margin block(s)
     *
     * Older ICE blocks lumped the margins in with Bulb Monitor.
     * New in 2011 is an ICE block with standalong margin blocks. 
     */
    pic_marg_reset();

    /* 
     * Reset Bit-Depth Reduction block
     */
    pic_bdr_reset();

    // Reset Bulb Shadow registers
    // FIXME - there really are n sensor bulb shadow registers - but since not currently using....
    *PIC_PC_BSRLSGCR = PIC_BS_RLSGC_R;
    *PIC_PC_BSLSVCR  = PIC_BS_LSVC_R;
    *PIC_PC_BSLMPCR0 = PIC_BS_LMPCR0_R;
    *PIC_PC_BSLMPCR1 = PIC_BS_LMPCR1_R;
    *PIC_PC_BSLMPCR2 = PIC_BS_LMPCR2_R;
    *PIC_PC_BSRSVCR  = PIC_BS_RSVC_R;
    *PIC_PC_BSRSPCR0 = PIC_BS_RSPCR0_R;
    *PIC_PC_BSRSPCR1 = PIC_BS_RSPCR1_R;
    *PIC_PC_BSRSPCR2 = PIC_BS_RSPCR2_R;

    // Reset additional gain & PONCR registers
#ifdef PIC_PC_AGCR0
    *PIC_PC_AGCR0 = PIC_AGCR0_R;
    *PIC_PC_AGCR1 = PIC_AGCR1_R;
    *PIC_PC_AGCR2 = PIC_AGCR2_R;
#endif


    /* 
     * Reset PIC WDMA
     */
    pic_wdma_reset();

#ifdef HAVE_PIC_PSESD
    pic_psesd_reset();
#endif

    /* davep 27-Feb-2013 ; adding chipgap repair */
    pic_chipgap_reset();
}

/**
 * \brief PIC hardware setup.
 *
 *  Restores PIC to as close to power-on values as possible. Doesn't interfere
 *  with operating system interface (interrupts, extra debugging, etc).
 *
 *  Compare to pic_onetime_init().
 *
 *  pic_onetime_init() does first-time intialization of PIC (turn on the
 *  hardware, hook up operating system interrupts, add debug commands, one-time
 *  data structure set-up).
 *
 *  This function does things that need to be done to reset PIC completely to
 *  a predictable, clean, runnable state.
 *
 *  Originally this code was all part of pic_onetime_init() but we now want to
 *  be able to restore PIC to power-on defaults after each cal (Bug 10870).
 *
 * \retval 0 success
 * \retval !0 detectable hardware failure
 *
 * \author David Poole
 * \date 21-Oct-2008
 *
 */

scan_err_t pic_soft_setup( void )
{
    bool was_enabled;
    struct afe_hardware_config_t afe_hw;

    pic_dbg2("%s\n", __FUNCTION__);

    /* add PIC interrupt into OS but don't enable until everything is set up */
    was_enabled = pic_interrupt_disable();

    // Reset all PIC registers to their power-on values.
    pic_reset();

    // Setup PIC common block
    pic_setup_interrupt();

    /* reset ADC by setting / clearing bypass bit */
    pic_adcnorm_set_bypass( true );
    pic_adcnorm_set_bypass( false );

    /* davep 21-Jan-2010; set up the interface between PIC-ADC and AFE based
     * on the new AFE hardware descriptor structure. (Was hardwired to 16-bpp,
     * right justify)
     */
    afe_get_hardware_config( &afe_hw );
    pic_adcnorm_set_abits( afe_hw.bits_per_pixel );
    pic_adcnorm_set_left_justify( afe_hw.left_justify );

    /* set BWM to bypass, rest defaults */
    pic_bm_set_bypass( true );

    /* by default, set bypass to disable hscale block */
    pic_hs_set_bypass(true);

    pic_wdma_init_routing();

    /* davep 04-Oct-2010 ; default to 16-bpp. Adding this because PICDMA2005
     * defaults to 16-bpp and a lot of code is assuming 16-bpp on reset. The
     * descriptor PIC DMA defaults to 8-bpp.
     */
    pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );

    /* davep 21-Feb-2012 ; set the burst size */
#if ICE_DMA_BYTE_ALIGN==16
    pic_wdma_set_burst_size( ICE_DMA_BURST_16 );
#elif ICE_DMA_BYTE_ALIGN==32
    pic_wdma_set_burst_size( ICE_DMA_BURST_32 );
#else
    #error Unknown ICE_DMA_BYTE_ALIGN
#endif

    if (was_enabled) {
        pic_interrupt_enable();
    }

    return SCANERR_NONE;
}

#ifdef __KERNEL__
scan_err_t pic_capture_interrupt( void )
{
    int retcode;

    /* wait until explicitly enabled */
    pic_interrupt_disable();

    retcode = request_irq( INTNUM_PIC, pic_interrupt, 0, "icepic", NULL );
    dbg2( "%s request_irq retcode=%d\n", __FUNCTION__, retcode );
    XASSERT(retcode==0, retcode );

    return SCANERR_NONE;
}

void pic_release_interrupt( void )
{
    pic_interrupt_disable();
    /* disable at the OS level as well */
    disable_irq( INTNUM_PIC );
    free_irq( INTNUM_PIC, NULL );
}
#else
scan_err_t pic_capture_interrupt( void )
{
    intAttach( INTNUM_PIC, 1, pic_interrupt, INTNUM_PIC );
    intEnable( INTNUM_PIC );
    return SCANERR_NONE;
}

void pic_release_interrupt( void )
{
    pic_interrupt_disable();
    /* disable at the OS level as well */
    intDisable( INTNUM_PIC );
    intDetach( INTNUM_PIC );
}
#endif

//--------------------------------------------------------------------------
// Function     : pic_onetime_init
//   returns    : integer status
//   arg1       :
//   arg2       :
// Created by   : David Poole
// Date created : ????
// Description  : Sets up the PIC block with some default values.
//
// 
//--------------------------------------------------------------------------

scan_err_t pic_onetime_init( void )
{
    scan_err_t scerr;

    pic_dbg2("%s\n", __FUNCTION__);

    /* add PIC interrupt into OS but don't enable until everything is set up */
    pic_interrupt_disable();

    pic_capture_interrupt( );

    scerr = pic_soft_setup();
    if( scerr != 0 ) {
        /* pic_soft_setup() hardwired to return 0 at this time but just in
         * case... 
         */
        return scerr;
    }

    /* note I'm unconditionally enabling the interrupt */
    pic_interrupt_enable();

    SCANALYZER_ENABLE_LOG( LOG_PIC_INTERRUPT );
    SCANALYZER_ENABLE_LOG( LOG_PIC_WDMA_IPEND );

    return SCANERR_NONE;
}

/**
 * \brief  Release all PIC resources. 
 *
 * Originally created for Linux kernel module __exit
 *
 * \author David Poole
 * \date 23-Apr-2012
 */

scan_err_t pic_cleanup_module( void )
{
    pic_release_interrupt();

    return SCANERR_NONE;
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_num_correction_bits
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the total number of correction bits used
//                for PRNU and DSNU.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_num_correction_bits( unsigned int numbits )
{
    /* davep 09-Mar-2011 ; XXX temp hack */
//    numbits= 24;
//    dbg2( "%s numbits=%d\n", __FUNCTION__, numbits );

    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_TOTAL_BITS(-1);
    switch (numbits) {
    case 12:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(0);
        break;
    case 16:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(1);
        break;
    case 20:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(2);
        break;
    case 24:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(3);
        break;
    case 28:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(4);
        break;
    case 32:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_BITS(5);
        break;
    default:
        dbg1( "Unsupported number of bits: %d",numbits);
        XASSERT(0,numbits);
        break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_num_extra_prnu_bits_col0_mono_even
//   returns    : 
//   arg1       : numbits (can be negative)
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the number of extra bits used for PRNU
//                compensation for color 0 or mono or the even row of mono,
//                depending on the attached sensor. Note that the number of "extra"
//                bits can be negative, in which case we will use more bits
//                for DSNU correction than for PRNU correction.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_num_extra_prnu_bits_col0_mono_even( int numbits )
{
    XASSERT(numbits >= -2 && numbits <= 2,numbits);
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_TOTAL_COLOR0(-1);
    switch (numbits) {
    case -2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR0(4);
        break;
    case -1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR0(3);
        break;
    case 0:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR0(0);
        break;
    case 1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR0(1);
        break;
    case 2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR0(2);
        break;
    default:
        dbg1( "Invalid numbits: %d",numbits);
        XASSERT(0,numbits);
        break;
    }
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Mar-2011
 *
 */

void pic_prnudsnu_set_prnu_muloff( uint32_t prnu_mult, uint8_t prnu_scale, uint32_t prnu_offset )
{
#ifdef PRNUDSNU_CORR_PMULT_SCALE_MASK 
    prnudsnu_regs->PMULT = PRNUDSNU_CORR_PMULT_SCALE_REPLACE_VAL( prnudsnu_regs->PMULT, prnu_scale );
    prnudsnu_regs->PMULT = PRNUDSNU_CORR_PMULT_MULT_REPLACE_VAL( prnudsnu_regs->PMULT, prnu_mult );
    prnudsnu_regs->POFF = PRNUDSNU_CORR_POFF_OFFSET_REPLACE_VAL( prnudsnu_regs->POFF, prnu_offset );
#endif
}

void pic_prnudsnu_set_dsnu_muloff( uint32_t dsnu_mult, uint8_t dsnu_scale, uint32_t dsnu_offset )
{
#ifdef PRNUDSNU_CORR_DMULT_SCALE_MASK 
    prnudsnu_regs->DMULT = PRNUDSNU_CORR_DMULT_SCALE_REPLACE_VAL( prnudsnu_regs->DMULT, dsnu_scale );
    prnudsnu_regs->DMULT = PRNUDSNU_CORR_DMULT_MULT_REPLACE_VAL( prnudsnu_regs->DMULT, dsnu_mult );
    prnudsnu_regs->DOFF = PRNUDSNU_CORR_DOFF_OFFSET_REPLACE_VAL( prnudsnu_regs->DOFF, dsnu_offset );
#endif
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_max_prnu_mult_col0_mono_even
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the PRNU compensation range for color 0
//                or mono or the even row of mono, depending on the
//                attached sensor. Note that a wider compensation range will
//                result in less compensation resolution.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_prnudsnu_set_max_prnu_mult_col0_mono_even( pic_prnu_multiplier_t multiplier )
{

    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_PRNU_COLOR0(-1);

    switch (multiplier) {
        case PIC_PRNU_MULTIPLIER_15 : //1.5x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR0(3);
            break;
        case PIC_PRNU_MULTIPLIER_20 : //2.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR0(2);
            break;
        case PIC_PRNU_MULTIPLIER_30 : //3.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR0(1);
            break;
        case PIC_PRNU_MULTIPLIER_50 : //5.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR0(0);
            break;
        default:
            dbg1( "invalid multiplier %d",multiplier );
            XASSERT(0,multiplier);
            break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_dsnu_mult_col0_mono_even
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the DSNU multiplier for color 0
//                or mono or the even row of mono, depending on the
//                attached sensor.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_dsnu_mult_col0_mono_even( uint8_t multiplier )
{
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_DSNU_COLOR0(-1);
    prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_DSNU_COLOR0(multiplier);
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_num_extra_prnu_bits_col1_odd
//   returns    : 
//   arg1       : numbits (can be negative)
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the number of extra bits used for PRNU
//                compensation for color 1 or the odd row of mono, depending
//                on the attached sensor. Note that the number of "extra"
//                bits can be negative, in which case we will use more bits
//                for DSNU correction than for PRNU correction.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_num_extra_prnu_bits_col1_odd( int numbits )
{
    XASSERT(numbits >= -2 && numbits <= 2,numbits);
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_TOTAL_COLOR1(-1);
    switch (numbits) {
    case -2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR1(4);
        break;
    case -1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR1(3);
        break;
    case 0:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR1(0);
        break;
    case 1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR1(1);
        break;
    case 2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR1(2);
        break;
    default:
        dbg1( "Invalid numbits: %d",numbits);
        XASSERT(0,numbits);
        break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_max_prnu_mult_col1_odd
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the PRNU compensation range for color 1
//                or the odd row of mono, depending on the attached sensor.
//                Note that a wider compensation range will result in less
//                compensation resolution.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_max_prnu_mult_col1_odd( pic_prnu_multiplier_t multiplier )
{
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_PRNU_COLOR1(-1);
    switch (multiplier) {
        case PIC_PRNU_MULTIPLIER_15 : //1.5x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR1(3);
            break;
        case PIC_PRNU_MULTIPLIER_20 : //2.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR1(2);
            break;
        case PIC_PRNU_MULTIPLIER_30 : //3.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR1(1);
            break;
        case PIC_PRNU_MULTIPLIER_50 : //5.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR1(0);
            break;
        default:
            dbg1( "invalid multiplier %d",multiplier );
            XASSERT(0,multiplier);
            break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_dsnu_mult_col1_odd
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the DSNU multiplier for color 1
//                or the odd row of mono, depending on the
//                attached sensor.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_dsnu_mult_col1_odd( uint8_t multiplier )
{
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_DSNU_COLOR1(-1);
    prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_DSNU_COLOR1(multiplier);
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_num_extra_prnu_bits_col2
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the number of extra bits used for PRNU
//                compensation for color 2. Note that the number of "extra"
//                bits can be negative, in which case we will use more bits
//                for DSNU correction than for PRNU correction.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_num_extra_prnu_bits_col2( int numbits )
{
    XASSERT(numbits >= -2 && numbits <= 2,numbits);
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_TOTAL_COLOR2(-1);
    switch (numbits) {
    case -2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR2(4);
        break;
    case -1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR2(3);
        break;
    case 0:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR2(0);
        break;
    case 1:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR2(1);
        break;
    case 2:
        prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_TOTAL_COLOR2(2);
        break;
    default:
        dbg1( "Invalid numbits: %d",numbits);
        XASSERT(0,numbits);
        break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_max_prnu_mult_col2
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the PRNU compensation range for color 2.
//                Note that a wider compensation range will result in less
//                compensation resolution.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_max_prnu_mult_col2( pic_prnu_multiplier_t multiplier )
{
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_PRNU_COLOR2(-1);
    switch (multiplier) {
        case PIC_PRNU_MULTIPLIER_15 : //1.5x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR2(3);
            break;
        case PIC_PRNU_MULTIPLIER_20 : //2.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR2(2);
            break;
        case PIC_PRNU_MULTIPLIER_30 : //3.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR2(1);
            break;
        case PIC_PRNU_MULTIPLIER_50 : //5.0x
            prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_PRNU_COLOR2(0);
            break;
        default:
            dbg1( "invalid multiplier %d",multiplier );
            XASSERT(0,multiplier);
            break;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_dsnu_mult_col1_odd
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine sets the DSNU multiplier for color 2.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_dsnu_mult_col2( uint8_t multiplier )
{
    prnudsnu_regs->PDCR1 &= ~PIC_PC_PDCR1_DSNU_COLOR2(-1);
    prnudsnu_regs->PDCR1 |= PIC_PC_PDCR1_DSNU_COLOR2(multiplier);
}

/**
 * \brief  Set the PRNU/DSNU configuration.
 *
 * The PIC PRNU/DSNU config register (PDCR1) is a complex beast. However, it
 * only has to be set up once so let's make it as easy as possible.
 *
 * Originally created this function so I could tweak with the PRNU/DSNU settings
 * easily in test code. Plan to deprecate the original pic_prnudsnu_set_xxx
 * functions and move cal to use this function+structure instead.
 *
 * \author David Poole
 * \date 27-Feb-2008
 *
 */

void pic_prnudsnu_set_config( struct pic_prnudsnu_config *pdc )
{
    uint32_t num32;

    pic_prnudsnu_set_num_correction_bits( pdc->total_bits );

    /* davep 27-Feb-2008 ; I'm not sure the way BradS did the
     * _set_max_prnu_mult_ function accurately reflects what these PDCR1 bits
     * actually mean. 
     *
     * I'm planning to deprecate his very long name functions and move code to
     * this function+structure instead. In the meantime, I'm going to poke the
     * PRNU shift value directly based on the integer field in struct
     * pic_prnudsnu_config.
     */

    /* clear the PRNU scale fields in PDCR1 */
    num32 = ( PIC_PC_PDCR1_PRNU_COLOR0(~0) |
              PIC_PC_PDCR1_PRNU_COLOR1(~0) |
              PIC_PC_PDCR1_PRNU_COLOR2(~0) );
    prnudsnu_regs->PDCR1 &= ~num32;

    /* set the new values */
    num32 = ( PIC_PC_PDCR1_PRNU_COLOR0(pdc->scale_prnu_color[0]) |
              PIC_PC_PDCR1_PRNU_COLOR1(pdc->scale_prnu_color[1]) |
              PIC_PC_PDCR1_PRNU_COLOR2(pdc->scale_prnu_color[2]) );
    prnudsnu_regs->PDCR1 |= num32;

    pic_prnudsnu_set_prnu_muloff( pdc->pmult, pdc->pscale, pdc->poffset );
    pic_prnudsnu_set_dsnu_muloff( pdc->dmult, pdc->dscale, pdc->doffset );

    pic_prnudsnu_set_num_extra_prnu_bits_col0_mono_even( pdc->extra_prnu_bits[0] );
    pic_prnudsnu_set_num_extra_prnu_bits_col1_odd( pdc->extra_prnu_bits[1] );
    pic_prnudsnu_set_num_extra_prnu_bits_col2( pdc->extra_prnu_bits[2] );

    pic_prnudsnu_set_dsnu_mult_col0_mono_even( pdc->scale_dsnu_color[0] );
    pic_prnudsnu_set_dsnu_mult_col1_odd( pdc->scale_dsnu_color[1] );
    pic_prnudsnu_set_dsnu_mult_col2( pdc->scale_dsnu_color[2] );

//    pic_prnudsnu_set_dsnu_scale( pdc->scale_dsnu_color[0] );

    /* davep 10-Aug-2010 ; new field in PDCR2 ; hardcode for now until I can
     * get the header files sorted out
     */
#ifdef HAVE_PIC_PC_QUAD
    /* hardwire to 12/12/8 (quad/prnu/dsnu) */
    pic_prnudsnu_set_coeffwidth(32);
#else
    /* hardwire to 12/12/0 (quad/prnu/dsnu) */
    pic_prnudsnu_set_coeffwidth(24);
#endif
}

/**
 * \brief  htonl the pixel correction LUT
 *
 *
 * \author David Poole
 * \date 06-Apr-2010
 *
 */

void pd_lut_swap32( uint8_t *lut, int lutsize_bytes )
{
    int lutsize_uint32;
    uint32_t *ptr32;

    /* note I'm not trying to handle a lut that doesn't align exactly on a
     * 4 byte boundary. Given our DMA alignment restrictions, we should be
     * getting a 16- or 32-byte aligned buffer anyway.
     */
    lutsize_uint32 = lutsize_bytes / 4;

    if( lutsize_bytes%4 != 0 ) {
        dbg1( "%s not 4-byte aligned!\n", __FUNCTION__ );
    }

    ptr32 = (uint32_t *)lut;
    while( lutsize_uint32 ) {

        *ptr32 = asp_htonl( *ptr32 );
        ptr32++;
        lutsize_uint32--;
    }

//    scanlog_hex_dump( lut, MIN(lutsize_bytes,64) );
}

/**
 * \brief  Pack the PRNU/DSNU correction factors into a LUT suitable for input
 * into the PIC PC RDMA.
 *
 * \param total_bits ; total bits from PDCR1 ; valid values are 12, 16, 20, 24
 *
 * \param extra_prnu_bits ; extra prnu bits from PDCR1 ; valid values are -2,-1,0,1,2
 *      
 * \param num_pixels ; number of pixels in the tables
 *
 * \param prnu ; prnu correction factors to encode into the hardware LUT
 *
 * \param dsnu ; dsnu correction factors to encode into the hardware LUT
 *
 * \param prnudsnu_lut ; prnu[] and dsnu[] will be encoded into this byte array;
 *
 * It is the caller's responsibility to make sure prnudsnu_lut[] is large enough
 * for all the pixels in the input.
 *
 * \author David Poole
 * \date 04-Mar-2008
 *
 */

void
pic_prnudsnu_encode_lut( int total_bits, int extra_prnu_bits,
                         int num_pixels,
                         uint16_t prnu[], uint16_t dsnu[], uint8_t quad[],
                         uint8_t prnudsnu_lut[], int prnudsnu_lut_num_entries )
{
    int i;
    uint32_t num32;
    uint8_t bit;
    int bitpos, curr_bit, curr_byte, curr_bit_of_curr_byte;
    int prnu_bits, dsnu_bits;

    /* davep 10-Aug-2010 ; asic changed. I have no f'ing clue how to pack the
     * bits. Restrict all LUTs to 12+12.  
     */
#ifdef HAVE_PIC_PC_QUAD
    /* davep 11-Aug-2010 ; hardwire to 8+12+12 (8 is for quad) */
    XASSERT( total_bits==32, total_bits );
#else
    XASSERT( total_bits==24, total_bits );
#endif
    XASSERT( extra_prnu_bits==0, extra_prnu_bits );


#ifdef HAVE_PIC_PC_QUAD
    prnu_bits = (total_bits-8)/2 + extra_prnu_bits;
    dsnu_bits = (total_bits-8)/2 - extra_prnu_bits;
#else
    prnu_bits = total_bits/2 + extra_prnu_bits;
    dsnu_bits = total_bits/2 - extra_prnu_bits;
#endif

    pic_dbg2( "%s prnu_bits=%d dsnu_bits=%d num_pixels=%d\n", __FUNCTION__,
            prnu_bits, dsnu_bits, num_pixels );

    /* quiet a warning */
    prnu_bits = prnu_bits;

    curr_bit = 0;
    for( i=0 ; i<num_pixels ; i++ ) {

//        num32 = encode_prnudsnu( prnu[i], dsnu[i], prnu_bits, dsnu_bits );
        /* typecast to uint32_t to avoid losing bits if we shift up past 16 */
        num32 = (((uint32_t)prnu[i])<<dsnu_bits) | dsnu[i];
//        num32 = prnu[i];
//        num32 <<= dsnu_bits;
//        num32 |= dsnu[i];

        /* davep 10-Aug-2010 ; stupid endian crap */
        num32 = (num32&0x0000ff)<<16
              | (num32&0x00ff00)
              | (num32&0xff0000)>>16
              ;
#ifdef HAVE_PIC_PC_QUAD
        num32 <<= 8;  /* start with quad of zero */
//        num32 |= 127;  /* add a quad value */
//        uint8_t quad = 0;
//        uint8_t quad = 0x33;
//        quad |= 0x80; /* negative */
        num32 |= quad[i];  /* add a quad value */
#endif

        for( bitpos = total_bits-1 ; bitpos >=0 ; bitpos-- ) {
            bit = (num32 & (1<<bitpos)) && 1;

            curr_byte = curr_bit / 8;
            curr_bit_of_curr_byte = 7 - (curr_bit % 8);

            XASSERT( curr_byte < prnudsnu_lut_num_entries, curr_byte );

            prnudsnu_lut[curr_byte] |= (bit << curr_bit_of_curr_byte);

//            dbg2( "%#lx bitpos=%d bit=%d cb=%d cB=%d cbcB=%d %#x\n", num32,
//                    bitpos, bit, curr_bit, curr_byte, curr_bit_of_curr_byte,
//                    prnudsnu_lut[curr_byte] );

            curr_bit++;
        }

    }

    /* davep 20-Jan-2011 ; XXX temp debug */
    scanlog_hex_dump( (unsigned char *)prnudsnu_lut, 128 );
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_color_counter
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is part of the bulb shadow correction logic.
//                From the HLD document: "When color_counter=1, then color0,
//                color1 and color2 register settings are used for color0/mono,
//                color1 and color2, respectively. When color_counter=0, color0
//                register settings are used for all even pixels and color 1 register
//                settings are used for all odd pixels."
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_color_counter( bool odd_even )
{
    if (odd_even)
        prnudsnu_regs->PDCR2 &= ~PIC_PC_PDCR2_CC;
    else
        prnudsnu_regs->PDCR2 |= PIC_PC_PDCR2_CC;
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_bypass_all
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is used to set or clear the bypass_all bit.
//                When the bypass_all bit is set, PRNU, DSNU, Bad Pixel and
//                Exposure Compensation are all bypassed.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_bypass_all( bool bypass_all )
{
    if (bypass_all)
        prnudsnu_regs->PDCR2 |= PIC_PC_PDCR2_BYALL;
    else
        prnudsnu_regs->PDCR2 &= ~PIC_PC_PDCR2_BYALL;
}

bool pic_prnudsnu_get_bypass_all( void )
{
    return (prnudsnu_regs->PDCR2 & PIC_PC_PDCR2_BYALL) && 1;
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_bypass_prnu
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is used to bypass PRNU compensation.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_bypass_prnu( bool bypass_prnu )
{
    if (bypass_prnu)
        prnudsnu_regs->PDCR2 |= PIC_PC_PDCR2_BYPRNU;
    else
        prnudsnu_regs->PDCR2 &= ~PIC_PC_PDCR2_BYPRNU;
}

bool pic_prnudsnu_get_bypass_prnu( void )
{
    return (prnudsnu_regs->PDCR2 & PIC_PC_PDCR2_BYPRNU) && 1;
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_bypass_dsnu
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is used to bypass DSNU compensation.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_bypass_dsnu( bool bypass_dsnu )
{
    if (bypass_dsnu)
        prnudsnu_regs->PDCR2 |= PIC_PC_PDCR2_BYDSNU;
    else
        prnudsnu_regs->PDCR2 &= ~PIC_PC_PDCR2_BYDSNU;
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_bypass_bad_pix_replace
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is used to bypass bad pixel compensation.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_bypass_bad_pix_replace( bool bypass_badpix )
{
    if (bypass_badpix) {
        prnudsnu_regs->PDCR2 |= PRNUDSNU_CORR_PDCR2_BYPASSBADPIX_MASK ; 
    }
    else {
        prnudsnu_regs->PDCR2 &= ~PRNUDSNU_CORR_PDCR2_BYPASSBADPIX_MASK; 
    }
}

//--------------------------------------------------------------------------
// Function     : pic_prnudsnu_set_bypass_exp_comp
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : This routine is used to bypass exposure compensation multiplication.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_prnudsnu_set_bypass_exp_comp( bool bypass_exp )
{
    if (bypass_exp)
        prnudsnu_regs->PDCR2 |= PIC_PC_PDCR2_BYEXP;
    else
        prnudsnu_regs->PDCR2 &= ~PIC_PC_PDCR2_BYEXP;
}

/**
 * \brief  Set quadratic correction bypass enable/disable
 *
 * \author David Poole
 * \date 09-Aug-2010
 *
 */

void pic_prnudsnu_set_bypass_quadratic( bool bypass )
{
#ifdef PRNUDSNU_CORR_PDCR2_BYPASSQUAD_MASK 
    if( bypass ) {
        prnudsnu_regs->PDCR2 |= PRNUDSNU_CORR_PDCR2_BYPASSQUAD_MASK; 
    }
    else {
        prnudsnu_regs->PDCR2 &= ~PRNUDSNU_CORR_PDCR2_BYPASSQUAD_MASK; 
    }
#endif
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 11-Aug-2010
 *
 */

void pic_prnudsnu_set_quadratic_config( uint32_t scale_factor, uint32_t limit_factor, uint32_t shift_factor )
{
#ifdef HAVE_PIC_PC_QUAD
    pic_dbg2( "%s SF1=%d SF2=%d shift=%d\n", __FUNCTION__, scale_factor, limit_factor, shift_factor );

    /* set the scale factor, hardwire the rest */
    prnudsnu_regs->QUAD = 0; /* default value */

    prnudsnu_regs->QUAD = PRNUDSNU_CORR_QUAD_SF1_REPLACE_VAL( prnudsnu_regs->QUAD, scale_factor );
    prnudsnu_regs->QUAD = PRNUDSNU_CORR_QUAD_SF2_REPLACE_VAL( prnudsnu_regs->QUAD, limit_factor);
    prnudsnu_regs->QUAD = PRNUDSNU_CORR_QUAD_Q_SHIFT_REPLACE_VAL( prnudsnu_regs->QUAD, (shift_factor&0x07));
#endif
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 09-Jan-2013
 */

void pic_prnudsnu_set_coeffwidth( uint8_t coeffwidth ) 
{
    uint8_t n;

    n = 0;
    switch( coeffwidth ) {
        case 12 : 
            n = PIC_PC_PDCR2_COEFFWIDTH_12BITS; 
            break;
        case 16 : 
            n = PIC_PC_PDCR2_COEFFWIDTH_16BITS; 
            break;
        case 20 : 
            n = PIC_PC_PDCR2_COEFFWIDTH_20BITS; 
            break;
        case 24 : 
            n = PIC_PC_PDCR2_COEFFWIDTH_24BITS; 
            break;
        case 32 : 
            n = PIC_PC_PDCR2_COEFFWIDTH_32BITS; 
            break;
        default:
            XASSERT(0,coeffwidth);
            return;
    }

    prnudsnu_regs->PDCR2 = PRNUDSNU_CORR_PDCR2_COEFFWIDTH_REPLACE_VAL(prnudsnu_regs->PDCR2, n );
}

/**
 * \brief dump just the prnu/dsnu stuff
 *
 * \author David Poole
 * \date 25-Jul-2011
 *
 */

void pic_prnudsnu_dump( void )
{
    dbg1( "PDCR1=0x%08lx PDCR2=0x%08lx\n", prnudsnu_regs->PDCR1, prnudsnu_regs->PDCR2 );

#ifdef HAVE_PIC_PC_QUAD
    dbg1("QUAD=0x%08lx\n", prnudsnu_regs->QUAD );
#endif

    /* some ASICs added new fields */
#ifdef PRNUDSNU_CORR_DOFF_OFFSET_MASK 
    dbg1( "DOFF=0x%08lx DMULT=0x%08lx POFF=0x%08lx PMULT=0x%08lx\n", 
                prnudsnu_regs->DOFF, prnudsnu_regs->DMULT, 
                prnudsnu_regs->POFF, prnudsnu_regs->PMULT );
#endif
}

//--------------------------------------------------------------------------
// Function     : pic_set_PonN
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the PonN value. (Refer to the PIC Micro Arch. doc for more info.)
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_set_PonN( unsigned int PonN )
{
    prnudsnu_regs->PONCR &= ~PIC_PC_PONCR_PON_N(-1);
    prnudsnu_regs->PONCR |= PIC_PC_PONCR_PON_N(PonN);
}

//--------------------------------------------------------------------------
// Function     : pic_set_PonNorm
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the PonNorm value. (Refer to the PIC Micro Arch. doc for more info.)
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_set_PonNorm( unsigned char PonNorm )
{
    prnudsnu_regs->PONCR &= ~PIC_PC_PONCR_PON_NORM(-1);
    prnudsnu_regs->PONCR |= PIC_PC_PONCR_PON_NORM(PonNorm);
}

/* hardware level functions for hscale */
//--------------------------------------------------------------------------
// Function     : pic_hs_set_bypass
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to bypass (or not) the PIC horizontal scaler.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_hs_set_bypass( bool bypass )
{
    if (bypass)
        *PIC_HS_HSCR |= PIC_HS_HSCR_BYPASS;
    else
        *PIC_HS_HSCR &= ~PIC_HS_HSCR_BYPASS;
}

//--------------------------------------------------------------------------
// Function     : pic_hs_set_hscale_factor
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to set the hscale factor. Factors above 65536
//                will cause the data to be scaled up, and factors below
//                65536 will scale the data down. The values 0 and 65536 will
//                place the block in bypass mode.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_hs_set_hscale_factor( unsigned int sfactor )
{
    *PIC_HS_HSSR = PIC_HS_HSSR_SF(sfactor);
}

//--------------------------------------------------------------------------
// Function     : pic_hs_get_hscale_factor
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the hscale factor. Factors above 65536
//                will cause the data to be scaled up, and factors below
//                65536 will scale the data down. The values 0 and 65536 will
//                place the block in bypass mode.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_hs_get_hscale_factor( void )
//{
//    return *PIC_HS_HSSR & 0x1ffff;
//}

/**
 * \brief  calculate and return a PIC Hscale factor 
 *
 * Given pixels_in and how many pixels we want out, calculate the HScale factor
 * necessary.
 *
 * \author David Poole
 * \date 12-Nov-2007
 *
 */

uint32_t pic_hs_calc_factor( uint32_t pixels_in, uint32_t pixels_out )
{
    uint32_t hscale_factor;
    int extra_pixels;
    int numer, denom;

    /* from the PIC Hscale MA:
     * "P = number of pixels to add or subtrace (positive to add, negative to
     * subtract"
     *
     * I'm using extra_pixels as "P"
     *
     * Note extra_pixels can be negative!!!!
     */
    extra_pixels = pixels_out - pixels_in;

    XASSERT( pixels_in + extra_pixels > 0, extra_pixels );

//    hscale_factor = (int)( 65536 * (f_line_length + extra_pixels)/f_line_length );

    /* can't use safeint because we want need an accurate intermediate result
     * in order to get the fractional 65536
     */
    numer = (pixels_in + extra_pixels) << 8;
    denom = pixels_in << 8;
    hscale_factor = ((numer*(65536<<8))/denom)>>8;

//    error = abs( (f_line_length + extra_pixels)/f_line_length -
//                  hscale_factor/65536 )

    return hscale_factor;
}

/**
 * \brief  Calculate a DMA aligned output pixel size based on scale and input
 * pixel size
 *
 * Originally created to handle hacking a 75/150 DPI into ICE Lite based
 * products. ICE Lite can't XY scale color.
 *
 *
 * \author David Poole
 * \date 01-Apr-2009
 *
 */

void pic_hs_calc_row( int pixels_per_row_in, int x_numer, int x_denom, 
                      int bits_per_pixel, 
                      int *pixels_per_row_out )
{
    int ppr, bytes_per_row;

    XASSERT( bits_per_pixel==8||bits_per_pixel==16, bits_per_pixel );

//    /* add 0.5 to round up */
//    ppr = (uint32_t)(((float)(pixels_per_row_in * x_numer) / (float)x_denom)+0.5);
    ppr = safeint_scale_int( pixels_per_row_in, x_numer, x_denom );

    /* convert to bytes per row, then round up for DMA alignment */
    bytes_per_row = ICE_DMA_ALIGN_ME( ppr * (bits_per_pixel/8) );

    /* convert back to pixels */
    ppr = bytes_per_row / (bits_per_pixel/8);

    *pixels_per_row_out = ppr;

    dbg2( "%s in=%d scale=%d/%d out=%d\n", __FUNCTION__, 
                pixels_per_row_in, x_numer, x_denom, *pixels_per_row_out );
}

/* hardware level functions for bit depth reduction */
//--------------------------------------------------------------------------
// Function     : pic_bdr_set_bypass
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to bypass (or not) the Bit Depth Reduction block.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_bdr_set_bypass( bool bypass )
{
    if (bypass)
        *PIC_BDR_BDRCR |= PIC_BDR_BDRCR_BYPASS;
    else
        *PIC_BDR_BDRCR &= ~PIC_BDR_BDRCR_BYPASS;
}

//--------------------------------------------------------------------------
// Function     : pic_bdr_enable_fw_access
//   returns    : 
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to enable CPU access to the Bit Depth Reduction LUT.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_bdr_enable_fw_access( bool enable )
{
    if (enable)
        *PIC_BDR_BDRCR |= PIC_BDR_BDRCR_CPU;
    else
        *PIC_BDR_BDRCR &= ~PIC_BDR_BDRCR_CPU;
}

//--------------------------------------------------------------------------
// Function     : pic_bdr_set_lut
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to write the Bit Depth Reduction LUT.
//
// pic_bdr_enable_fw_access() must be called before using this function.
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_bdr_set_lut( uint32_t *lut )
{
    int i;
    volatile uint32_t *lutreg = PIC_BDR_LUT;

    /* davep 24-Mar-2010 ; if on a platform that supports the 3-lut BDR, the
     * proper LUT must have been set before this function is called
     */
    
    // Note that the correction table ("LUT") is word addressed
    // but filled with 8-bit data (i.e. there's one register for
    // each entry).
    for (i=0;i<PIC_BDR_LUT_SIZE;i++) {
        *lutreg++ = *lut++;
    }
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 26-Mar-2010
 *
 */

#ifdef HAVE_THREE_BDR_LUT
void pic_bdr_set_num_luts( int num_luts )
{
    /* clear the old value and set the new value */
    if( num_luts==1 ) {
        *PIC_BDR_BDRCR = (*PIC_BDR_BDRCR & ~PIC_BDR_BDRCR_NUMLUTS(~0))
                       | PIC_BDR_BDRCR_NUMLUTS( PIC_BDR_BDRCR_NUMLUTS_ONE );
    }
    else if( num_luts==3 ) {
        *PIC_BDR_BDRCR = ((*PIC_BDR_BDRCR & ~PIC_BDR_BDRCR_NUMLUTS(~0))
                       | PIC_BDR_BDRCR_NUMLUTS( PIC_BDR_BDRCR_NUMLUTS_THREE ));
    }
    else {
        XASSERT( 0, num_luts );
    }
}
#endif

/**
 * \brief  Select which LUT to control 
 *
 * Select the LUT to program.  
 *
 * Normal use is something like:
 *   pic_bdr_set_config( PIC_BDR_BDRCR_NUMLUTS_THREE );
 *   pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR2 );
 *   pic_bdr_enable_cpu_access( true );
 *   pic_bdr_set_lut( lut );
 *   pic_bdr_enable_cpu_access( false );
 *
 * \author David Poole
 * \date 26-Mar-2010
 *
 */

#ifdef HAVE_THREE_BDR_LUT
void pic_bdr_select_lut( int lutsel )
{
    XASSERT( lutsel==PIC_BDR_BDRCR_LUTSEL_COLOR0MONO
          || lutsel==PIC_BDR_BDRCR_LUTSEL_COLOR1
          || lutsel==PIC_BDR_BDRCR_LUTSEL_COLOR2, lutsel );

    *PIC_BDR_BDRCR = (*PIC_BDR_BDRCR & ~PIC_BDR_BDRCR_LUTSEL(~0))
                   | PIC_BDR_BDRCR_LUTSEL( lutsel );
}
#endif

/**
 * \brief  Dump the PIC BDR
 *
 *
 * \author David Poole
 * \date 26-Mar-2010
 *
 */

void pic_bdr_dump( bool dump_lut )
{
    dbg2( "BDRCR=%#x\n", *PIC_BDR_BDRCR );

    if( dump_lut ) {
        pic_bdr_enable_fw_access(true);

        /* The 'lut_size' is number of entries in the table, not bytes. */
//        scanlog_hex_dump( (unsigned char *)PIC_BDR_LUT, 64 );
        scanlog_hex_dump( (unsigned char *)PIC_BDR_LUT, PIC_BDR_LUT_SIZE*sizeof(uint32_t));

        pic_bdr_enable_fw_access(false);
    }
}

/**
 * \brief  Reset the BDR to power-on defaults
 *
 *
 * \author David Poole
 * \date 26-Mar-2010
 *
 */

void pic_bdr_reset( void )
{
    uint32_t i;
    volatile uint32_t *bdr_lut;

    *PIC_BDR_BDRCR = PIC_BDR_BDRC_R;

    // Reset the BDR LUT
    *PIC_BDR_BDRCR |= PIC_BDR_BDRCR_CPU;
    bdr_lut = PIC_BDR_LUT;
    for (i=0 ; i<PIC_BDR_LUT_SIZE; i++ ) {
        *bdr_lut++=0;
    }

#ifdef HAVE_THREE_BDR_LUT
    /* davep 24-Mar-2010 ; Adding support for new three LUT BDR;
     * initialize the other two luts 
     */
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR1 );
    bdr_lut = PIC_BDR_LUT;
    for (i=0 ; i<PIC_BDR_LUT_SIZE; i++ ) {
        *bdr_lut++=0;
    }
    pic_bdr_select_lut( PIC_BDR_BDRCR_LUTSEL_COLOR2 );
    bdr_lut = PIC_BDR_LUT;
    for (i=0 ; i<PIC_BDR_LUT_SIZE; i++ ) {
        *bdr_lut++=0;
    }
#endif

    // Reset BDR block again
    *PIC_BDR_BDRCR = PIC_BDR_BDRC_R;
}

/**
 * \brief Enable/Disable the ADC Normalization block data inversion 
 *
 * When enabled, ADCNorm will invert the incoming ADC (aka AFE) data. When
 * disabled, the data is passed through as-is.
 *
 * \author David Poole
 * \date 16-Jan-2008
 *
 */

void
pic_adcnorm_set_invert( bool enable ) 
{
    if( enable )  {
        /* turn on ADC inverting the incoming data */
        *PIC_ADCN_ANR |= PIC_ADCN_ANR_INV;
    }
    else {
        /* turn off ADC inverting the incoming data */
        *PIC_ADCN_ANR &= ~PIC_ADCN_ANR_INV;
    }
}

/**
 * \brief  Set PIC ADC Normalizer block bypass mode
 *
 *
 * \author David Poole
 * \date 21-Oct-2008
 *
 */

void pic_adcnorm_set_bypass( bool bypass )
{
    if( bypass ) {
        *PIC_ADCN_ANR |= PIC_ADCN_ANR_BYPASS;
    }
    else {
        *PIC_ADCN_ANR &= ~PIC_ADCN_ANR_BYPASS;
    }
}

/**
 * \brief Set the PIC ADC Normalizer to left justify the incoming pixels. 
 *
 * From the ADC Norm MA: "Set to left justify (left shift) ADC Data so that
 * Dataout [15:m] = DataIn[ABITS:0], where m = 15-ABITS."
 *
 * \author David Poole
 * \date 10-Jan-2010
 *
 */

void pic_adcnorm_set_left_justify( bool left_justify )
{
    uint32_t num32;

    num32 = *PIC_ADCN_ANR;
    
    /* clear old setting */
    num32 &= ~PIC_ADCN_ANR_JUSTIFY;

    /* set new */
    if( left_justify ) {
        num32 |= PIC_ADCN_ANR_JUSTIFY;
    }

    *PIC_ADCN_ANR = num32;
}

/**
 * \brief  Set the PIC ADC Normalizer ABITS field
 *
 * ABITS field is the number of bits in the ADC (aka AFE)
 *
 * Number of ADC bits = ABITS + 1
 *
 * \author David Poole
 * \date 21-Oct-2008
 *
 */

void pic_adcnorm_set_abits( uint8_t abits )
{
    uint32_t num32;

    /* stupid human check */
    ASSERT( abits > 0 );

    num32 = *PIC_ADCN_ANR;

    /* clear old setting */
    num32 &= ~PIC_ADCN_ANR_ABITS(~0);

    /* set new */
    num32 |= PIC_ADCN_ANR_ABITS(abits-1);

    *PIC_ADCN_ANR = num32;
}

/**
 * \brief Put the ADCNorm block into test mode.
 *
 * Test mode lets us write data into the top of PIC via the ANT register. (See
 * also pic_adcnorm_ant().)
 *
 *
 * \author David Poole
 * \date 03-Mar-2010
 *
 */

void pic_adcnorm_set_test_mode( bool enable )
{
    if( enable ) {
        *PIC_ADCN_ANR |= PIC_ADCN_ANR_TEST; 
    }
    else {
        *PIC_ADCN_ANR &= ~PIC_ADCN_ANR_TEST; 
    }
}

/**
 * \brief  Write into the ADC Norm Test Register (ANT) 
 *
 * Used when testing PIC. Writes data into the top of PIC as if it came from the
 * scan block.
 *
 * Note: has no support for sending Exposure data. We're not using CCFL right
 * now so we don't need it. 
 *
 * \author David Poole
 * \date 27-Feb-2008
 *
 */

void
pic_adcnorm_ant( uint32_t adc_type, uint32_t adc_color, uint32_t adc_data )
{
    uint32_t num32;

    /* note I'm leaving the exposure as zero */
    num32 = PIC_ADCN_ANT_DATATYPE( adc_type ) 
          | PIC_ADCN_ANT_COLOR( adc_color ) 
          | PIC_ADCN_ANT_DATA( adc_data )
          ;
    *PIC_ADCN_ANT = num32;
}

/**
 * \brief  Calculate the pixels-per-row flowing through PIC.
 *
 * \author David Poole
 * \date 01-Mar-2013
 */

scan_err_t pic_calc_pixels_per_row( const struct scanvars *sv, 
                                    struct pic_pixels_per_row *p,
                                    uint32_t scan_pixels_per_row_in  )
{
    scan_err_t scerr;
    int num32;
    struct scan_sensor_chipgap sensor_gaps;

    dbg2( "%s scan_ppr_in=%d\n", __FUNCTION__, scan_pixels_per_row_in );

#if !defined HAVE_PIC_CHIPGAP
    if( p->use_chipgap ) {
        /* no chipgap support in this ASIC */
        ASSERT(0);
        /* use scerr here to quiet a warning */
        scerr = SCANERR_NOT_IMPLEMENTED;
        return scerr;
    }
#else
    memset( &sensor_gaps, 0, sizeof(struct scan_sensor_chipgap) );
    scerr = pic_chipgap_get_sensor_chipgap( sv->hw_dpi_horiz, &sensor_gaps );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s no chipgap entry for dpi=%d\n", __FUNCTION__, sv->hw_dpi_horiz );
        p->use_chipgap = false;
    }
#endif

    p->pic_pixels_per_row_in = scan_pixels_per_row_in;

    p->lrmargins0_ppr_in = scan_pixels_per_row_in;
    if( p->use_lrmargins0 ) {
        /* margin's output is the scan area */
        p->lrmargins0_ppr_out = sv->scan_area_pixels.width;
    }
    else {
        p->lrmargins0_ppr_out = p->lrmargins0_ppr_in;
    }

    p->chipgap_ppr_in = p->lrmargins0_ppr_out;
    if( p->use_chipgap ) {
        /* pixels out will be pixels in plus all the gap repair pixels we'll
         * add 
         */
        p->chipgap_ppr_out = p->chipgap_ppr_in + 
                    pic_chipgap_sum_list( sensor_gaps.chipgap_list,
                                          sensor_gaps.num_chipgap_list );
    }
    else {
        p->chipgap_ppr_out = p->chipgap_ppr_in;
    }

    p->lrmargins1_ppr_in = p->chipgap_ppr_out;
    if( p->use_lrmargins1 ) {
        /* davep 01-Mar-2013 ; no support for 2nd LRMargin block yet */
        ASSERT(0); 
    }
    else {
        p->lrmargins1_ppr_out = p->lrmargins1_ppr_in;
    }

    p->hscale_ppr_in = p->lrmargins1_ppr_in;
    if( p->use_hscale ) {
        pic_hs_calc_row( p->lrmargins1_ppr_out, 
                        sv->pic_scale.x_numer, sv->pic_scale.x_denom, 
                        sv->pic_bpp, &num32 );
        p->hscale_ppr_out = (uint32_t)num32;
    }
    else {
        p->hscale_ppr_out = p->hscale_ppr_in;
    }

    p->pic_pixels_per_row_out = p->hscale_ppr_out;

    dbg2( "%s pic_ppr_out=%d\n", __FUNCTION__, p->pic_pixels_per_row_out );

    /* stupid human check */
    XASSERT( p->pic_pixels_per_row_out < 65536, p->pic_pixels_per_row_out );

    return SCANERR_NONE;
}
/**
 * \brief clear the PIC interrupt statistics structure 
 *
 * Call before starting PIC to clear out any old stats floating around.
 *
 * \author David Poole
 * \date 19-Oct-2005
 *
 * Don't be an idiot--don't do this while PIC is running.
 *
 */
void
pic_clear_interrupt_stats( void )
{
    memset( &pic_interrupt_stats, 0, sizeof(pic_interrupt_stats) );
}


void pic_dump( void )
{
    int i;
    int num_pic_blocks=1;
    uint32_t pd_save_block_num, pd_dma_save_block_num;

#ifdef HAVE_NSENSOR_SUPPORT
    num_pic_blocks = scansen_get_num_sensors();
#endif

    dbg2("PIC DUMP:\n");
    dbg2("------------------------------------------------------------------------\n");
    dbg2("IENR   (Interrupt Enable Register)................................. 0x%08x\n", pic_ci_regs->IENR );
    dbg2("IPR    (Interrupt Pending Register)................................ 0x%08x\n", pic_ci_regs->IPR );
#ifdef PIC_INTCTRL_CCBIR_SOFTRESET_MASK
    dbg2("CBIR   (Common Block Interface Register)........................... 0x%08x\n", pic_ci_regs->CCBIR );
#else
    dbg2("PCR    (PIC Control Register)...................................... 0x%08x\n", pic_ci_regs->PCR );
#endif
    dbg2("ANR    (ADC Normalization Register)................................ 0x%08x\n", *PIC_ADCN_ANR);
    dbg2("ANI    (ADC Normalization Interrupt Register)...................... 0x%08x\n", *PIC_ADCN_ANI);
#ifdef PIC_ADCN_ANIP
    dbg2("ANIP   (ADC Normalization Interrupt Pending Register).............. 0x%08x\n", *PIC_ADCN_ANIP);
#endif

    pic_bm_dump();
    pic_marg_dump();

    /* davep 27-Feb-2013 ; adding chipgap repair */
    pic_chipgap_dump();

    /* davep 27-Mar-2008 ; we don't use these currently so no point in printing
     * them
     */
#if 0
    dbg2( ("RLSCR  (Bulb Shadow Right-Left Starting Gain Config Register)...... 0x%08x\n", *PIC_PC_BSRLSGCR) );
    dbg2( ("LSVCR  (Bulb Shadow Left Slope Value Config Register).............. 0x%08x\n", *PIC_PC_BSLSVCR) );
    dbg2( ("LMPCR0 (Bulb Shadow Left Max Pixel Config Register Color0/Even).... 0x%08x\n", *PIC_PC_BSLMPCR0) );
    dbg2( ("LMPCR1 (Bulb Shadow Left Max Pixel Config Register Color1/Odd)..... 0x%08x\n", *PIC_PC_BSLMPCR1) );
    dbg2( ("LMPCR2 (Bulb Shadow Left Max Pixel Config Register Color2)......... 0x%08x\n", *PIC_PC_BSLMPCR2) );
    dbg2( ("RSVCR  (Bulb Shadow Right Slope Value Config Register)............. 0x%08x\n", *PIC_PC_BSRSVCR) );
    dbg2( ("RSPCR0 (Bulb Shadow Right Start Pixel Config Register Color0/Even). 0x%08x\n", *PIC_PC_BSRSPCR0) );
    dbg2( ("RSPCR1 (Bulb Shadow Right Start Pixel Config Register Color1/Odd).. 0x%08x\n", *PIC_PC_BSRSPCR1) );
    dbg2( ("RSPCR2 (Bulb Shadow Right Start Pixel Config Register Color2)...... 0x%08x\n", *PIC_PC_BSRSPCR2) );
#endif

//    dbg2( ("PDCR1  (PRNU/DSNU Config Register 1)............................... 0x%08x\n", *PIC_PC_PDCR1) );
//    dbg2( ("PDCR2  (PRNU/DSNU Config Register 2)............................... 0x%08x\n", *PIC_PC_PDCR2) );
//
//#ifdef PIC_PC_QUAD
//    dbg2( ("QUAD   (PRNU/DSNU Quadratic Config )............................... 0x%08x\n", *PIC_PC_QUAD ) );
//#endif

    /* davep 27-Mar-2008 ; we don't use these currently so no point in printing
     * them
     */
#if 0
    dbg2( ("AGCR0  (Additional Gain Config Register Color0).................... 0x%08x\n", *PIC_PC_AGCR0) );
    dbg2( ("AGCR1  (Additional Gain Config Register Color1).................... 0x%08x\n", *PIC_PC_AGCR1) );
    dbg2( ("AGCR2  (Additional Gain Config Register Color2).................... 0x%08x\n", *PIC_PC_AGCR2) );
    dbg2( ("PONCR  (Percentage of Nominal Configuration Register).............. 0x%08x\n", *PIC_PC_PONCR) );
#endif

    // dual scan - pic may have multiple sets of registers for dual scan support
    // store away current block for the prnudsnu functions, reset all blocks, and restore when done
    pd_save_block_num = pic_prnudsnu_get_block();
    pd_dma_save_block_num = pic_prnudsnu_dma_get_block();    
    
    for( i=0 ; i<num_pic_blocks; i++ ) {
        pic_prnudsnu_set_block(i);
        pic_prnudsnu_dump();
        pic_prnudsnu_dma_dump();
    }
    pic_prnudsnu_set_block(pd_save_block_num);  // restoring both now....
    pic_prnudsnu_dma_set_block(pd_dma_save_block_num);    

    dbg2( "HSCR   (Horizontal Scaler Config Register)......................... 0x%08x\n", *PIC_HS_HSCR) ;
    dbg2( "HSSR   (Horizontal Scaler Scale Register).......................... 0x%08x\n", *PIC_HS_HSSR) ;
    dbg2( "BDRCR  (Bit Depth Reduction Config Register)....................... 0x%08x\n", *PIC_BDR_BDRCR) ;

    /* davep 25-Mar-2010 ; PIC WDMA changed */
    dbg2( "DWOCR  (PIC Write DMA Overall Config Register)..................... 0x%08x\n", *PIC_WDMA_DWOCR) ;
    dbg2( "DWRTBL (PIC Write DMA Router Table Register)....................... 0x%08x\n", *PIC_WDMA_DWRTBL) ;
    dbg2( "DWCIS  (PIC Write DMA Common Interrupt Select Register)............ 0x%08x\n", *PIC_WDMA_DWCIS) ;

    /* davep 26-Mar-2010 ; changing PIC WDMA to the chained, descriptor based
     * DMA. Older chips need the individual register block WDMA.
     */
    for( i=0 ; i<PIC_WDMA_NUM_CHANNELS ; i++ ) {
        pic_wdma_channel_dump(i);
    }
}

int pic_debug_msg( uint32_t param2, void *param3 )
{
    /* parse the rest of the message fields */

    switch( param2 ) {
        case SCAN_DBGMSG_PIC_DUMP :
            pic_dump();
            break;

        default :
            XASSERT( 0, param2 );
            /* ignore */
    }

    return 0;
}

