/**
 * \file scantest.c
 *
 * \brief Code for running simple tests on the sensor.
 *
 * Do NOT NOT call these functions unless you know what you're doing! Don't
 * make me hurt you.
 *
 */
/** 
 * ============================================================================
 * Copyright (c) 2007, 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 **/
 
#include <string.h>
#include <limits.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"
#include "regAddrs.h"
#include "cpu_api.h"
#include "list.h"
#include "regAddrs.h" 

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scan.h"
#include "scanimg.h"
#include "scantest.h"
#include "scanhw.h"
#include "scanif.h"
#include "scancmdq.h"
#include "pichw.h"
#include "pic.h"
#include "scanvars.h"
#include "safetylock.h"
#include "icedma.h"
#include "scantask.h"
#include "ostools.h"
#include "afe.h" /* for afe_dump() */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
#include "picdma_descrip.h"
#include "picdma_descrip_test.h"
#endif
#include "scands.h"

// set this constant to the actual sensor you want to debug.
// If you only have 1 sensor, the value 1 works nicely.
// If you have 2 sensors, set to 2 for the 2nd sensor, set to 1 for the 1st sensor
// If you have 3 sensors, set to 4 for the 3rd sensor, etc, std bitmask
#define SCANTEST_SENSOR 0x1

/* define SCANTEST_DEBUG to turn on extra verbose messages; help for debugging
 * the tests themselves
 */
//#define SCANTEST_DEBUG

static bool scmd_done;

static int scantest_scancmdq_isr( void )
{
    /*
     * BIG FAT NOTE!  THIS IS AN INTERRUPT HANDLER! 
     */

    scmd_done = true;

    /* fake out scan_interrupt() */
    return SCAN_CMDQ_SUCCESS;
}

static scan_err_t check_for_messages( struct scos_mq * msgq )
{
    scan_msg_t msg;
    scan_err_t scerr;

    /* wait for N usec for message; also serves as a handy delay */
    scerr = scos_mq_receive( msgq, (void *)&msg, 1000 );
    XASSERT( scerr==SCANERR_NONE||scerr==SCANERR_TIMEDOUT, scerr );

    if( scerr==0 ) {
        switch( msg.msgType ) {
            case SMSG_SCAN_CANCEL :
                scerr = SCANERR_SCAN_CANCELLED;
                break;

            case SCAN_DBGMSG_DUMP_AFE :
                afe_dump();
                break;

            case SCAN_DBGMSG_DUMP_SCAN :
                scif_dump();
                break;

            case SMSG_AFE_HW_FAILURE :
                /* davep 19-Jul-2012 ; catch AFE hardware failure; can happen in
                 * interrupt context so ISR will send the thread this message
                 */
                dbg1( "%s afe failure param1=0x%x\n", __FUNCTION__, msg.param1 );
                scerr = SCANERR_SCAN_CANCELLED;
                break;

            default :
                /* at this stage of the game, we're only expecting something from
                 * the scan subsystem
                 */
                XASSERT( 0, msg.msgType );
        }
    }

    return scerr;
}

static void blocking_cancel_scmd( void )
{
    /* davep 24-Jul-2012 ; simpler, brute force cancel. Shut everything down. */
    scan_interrupt_disable();
    scif_control(false, false); // stop scan, no interrupt
    scif_clock(false);          // disable scan clocks
}

scan_err_t scantest_sensor( scan_cmode_t cmode, uint32_t dpi, uint32_t num_lines )
{
    scan_err_t scerr, test_scerr;
    scan_cmdq_isr_t old_cmdq_isr;
    uint32_t scanx, sclkper;
    uint32_t pixper;
    struct scos_mq * msgq;
    uint16_t scany;
    uint32_t sensor_bitmask;

    dbg2( "%s cmode=%d dpi=%d num_lines=%d\n", __FUNCTION__, 
                cmode, dpi, num_lines );

    /* we're going to run SCMD so grab the cmdq interrupt from the official
     * owner (we'll restore it before we leave this function)
     */
    old_cmdq_isr  = scan_set_cmdq_interrupt( scantest_scancmdq_isr );
    scan_interrupt_enable();

    scerr = scands_get_integer_with_default( "scantest_sensor", &sensor_bitmask, SCANTEST_SENSOR);
    XASSERT( scerr==0, scerr );
    
    /* start the scan block and sensor/afe from a clean slate */
    scerr = scanimg_open( cmode, dpi, sensor_bitmask );
    XASSERT( scerr==0, scerr );

    /* tell sensor/afe how we're scanning */
    scerr = scanimg_setup();
    XASSERT( scerr==0, scerr );

    scanimg_get_scanx( &scanx );
    scanimg_get_pixper( &pixper );

    /* we'll need internal scanline mode so we don't need a motor */
    scif_set_internal_scanline_mode( cmode, dpi, scanx, &sclkper );
    dbg2( "%s scanx=%d sclkper=%d pixper=%d\n", 
                __FUNCTION__, scanx, sclkper, pixper );

    /* davep 25-Feb-2013 ; explicitly put PIC into bypass */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_soft_setup();
    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_DISCARD);
#endif

    /* away we go! */
    scanimg_run();

    /* Turn on scan block. After this call, we're ready to run. All we need is
     * some stuff pushed into SCMD
     */
    scif_control(true,true);
    scif_clock(true);

    /* is this why I'm not getting my PIC overflow? */
    pic_interrupt_enable();
//    pic_interrupt_disable();

    dbg2( "%s waiting for num_lines=%d to complete\n", __FUNCTION__, num_lines );

    /* get the scantask message queue so we can check for cancel and debug
     * messages 
     */
    msgq = scantask_get_msgq();

    test_scerr = SCANERR_NONE;

    /* davep 18-Feb-2008 ; Interesting note: I found doing a scif_dump()
     * here caused the command to never start. Hypothesis is reading the
     * registers at the moment I pushed in an SCMD caused some sort of
     * confusion in the Scan block state machines.
     *
     * After some further tinkering, decided even peeking at SSTAT.SCMDSTAT
     * might be causing problems. I'd plug in an SCMD and it just wouldn't
     * start. 
     *
     * So I think I'm going to avoid hassling the scan block while it's
     * working (i.e, don't poll the scan block). Switched to using the
     * global scmd_done flag.
     */
    scmd_done = false;

    /* davep 09-Nov-2010 ; I've changed how we send in scan commands. Previous
     * code would send one row at a time, waiting for scmd_done. We would
     * brutally spin on scmd_done. Something changed so the serial terminal
     * locks up while we spin loop. I added a pthread_yield() but I'm worried
     * that would change the timing of the SCMDs. 
     *
     * I changed the algorithm. Now we push in as many lines as we can then
     * politely spin waiting for the command to complete.
     */

    while( num_lines ) {
        scany = MIN( SCIF_SCMD_SCANY(~0), num_lines );

        num_lines -= scany;

        /* scan command to get some lines with no data */
        scif_send_simple_scmd( false, sclkper, scany );

        while( !scmd_done ) {
            /* davep 30-Sep-2010 ;  peak into msgq looking for a cancel */
            scerr = check_for_messages( msgq );
            if( scerr==SCANERR_SCAN_CANCELLED ) {
                blocking_cancel_scmd();

                /* leave our loop */
                dbg2( "%s test cancelled\n", __FUNCTION__ );
                num_lines = 0;
                test_scerr = scerr;
                break;
            }
            else { 
                XASSERT( scerr==SCANERR_NONE||scerr==SCANERR_TIMEDOUT, scerr );
                pthread_yield();
            }
        }

        scmd_done = false;
    }

//    XASSERT( !scif_command_executing(), *SCIF_SSTAT );

    scanimg_stop();
    scanimg_close();

    /* davep 24-Jul-2012 ; a certain digital afe control block seems to throw
     * an error during cancel so need to clean out any pending messages
     */
    while( 1 ) {
        scerr = check_for_messages( msgq );
        if( scerr==SCANERR_NONE||scerr==SCANERR_TIMEDOUT ) {
            break;
        }
    }

    /* put back the previous handler (clean up after ourselves) */
    scan_set_cmdq_interrupt( old_cmdq_isr );

    dbg2( "%s done\n", __FUNCTION__ );

    return test_scerr;
}

scan_err_t scantest_simple_sensor( void )
{
    scan_err_t scerr;

    dbg2( "%s start\n", __FUNCTION__ );

    scerr = scantest_sensor( SCAN_CMODE_MONO, 300, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor( SCAN_CMODE_COLOR, 300, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor( SCAN_CMODE_MONO, 600, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor( SCAN_CMODE_COLOR, 600, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    /* MORE HERE */

    dbg2( "%s done\n", __FUNCTION__ );

    /* success! */
    return 0;
}


scan_err_t scantest_sensor_with_data( scan_cmode_t cmode, uint32_t dpi, uint32_t num_lines )
{
    scan_err_t scerr;
    int i;
    scan_cmdq_isr_t old_cmdq_isr;
    uint32_t sanity_count;
    uint32_t scanx, sclkper, pic_bytes_per_row;
    uint8_t *picbuf, channel, pic_num_channels;
    uint32_t num_rows;
    uint8_t *pic_buffers[ PIC_WDMA_NUM_CHANNELS ];
    uint8_t pic_channels[ PIC_WDMA_NUM_CHANNELS ];
    bool test_success;
    pic_bitpack_mode bitpack_mode;
    uint32_t sensor_bitmask;

    dbg2( "%s cmode=%d dpi=%d num_lines=%d\n", __FUNCTION__, 
                cmode, dpi, num_lines );

    /* we're going to run SCMD so grab the cmdq interrupt from the official
     * owner (we'll restore it before we leave this function)
     */
    scan_interrupt_disable();
    old_cmdq_isr  = scan_set_cmdq_interrupt( scantest_scancmdq_isr );
    scan_interrupt_enable();

    /* need to grab the PIC WDMA handler, too. The default handler wants to use
     * a lot of RTOS message queue stuff which we don't want to handle in our
     * simple test
     */
    pic_interrupt_disable();

    /*
     *
     *   Static/Const fields steps
     *
     */

    scerr = scands_get_integer_with_default( "scantest_sensor", &sensor_bitmask, SCANTEST_SENSOR);
    XASSERT( scerr==0, scerr );
    
    /* start the scan block and sensor/afe from a clean slate */
    scerr = scanimg_open( cmode, dpi, sensor_bitmask );
    XASSERT( scerr==0, scerr );

    /* set pic to a clean slate (all blocks will be in bypass after this call) */
//    pic_reset();
//    pic_setup_interrupt();
    pic_soft_setup();

    /* 
     *
     *   Per Scan Setup steps
     *
     */

    /* tell sensor/afe how we're scanning */
    scerr = scanimg_setup();
    XASSERT( scerr==0, scerr );

    /* we'll need internal scanline mode so we don't need a motor */
    scanimg_get_scanx( &scanx );
    scif_set_internal_scanline_mode( cmode, dpi, scanx, &sclkper );

    dbg2( "%s scanx=%d sclkper=%d\n", __FUNCTION__, scanx, sclkper );


    /* 
     *
     *  Set up PIC
     *
     */

    bitpack_mode = pic_wdma_get_bitpack_mode();

    /* how many bytes will we need? we'll leave PIC margins in bypass for this
     * test so PIC WDMA will get all the pixels from the scan block
     */
    if( bitpack_mode == PIC_BITPACK_16BIT ) {
        pic_bytes_per_row = scanx * 2;
    }
    else {
        pic_bytes_per_row = scanx;
    }

    /* we're not using PIC margins (all PIC blocks will be in bypass for this
     * test) so we need to make sure Scan has set up SCANX to be modulo our DMA
     * width
     */
    if( pic_bytes_per_row % ICE_DMA_BYTE_ALIGN != 0 ) {
        /* we'll have to ask the scan block to pad up so we'll be able to
         * survive DMA 
         */

        if( bitpack_mode == PIC_BITPACK_16BIT ) {
            /* we'll get 16-bit pixels so we'll need to make sure scanx number
             * of pixels is aligned to 1/2 our burst size
             *
             * e.g.,  16-bit pixels and 16-byte DMA burst size means we'll need
             * 8 pixels per burst.
             */
            scanimg_align_scanx( &scanx, ICE_DMA_BYTE_ALIGN/2, SCANX_ALIGN_PAD );
            scanimg_set_scanx( scanx );
            pic_bytes_per_row = scanx * 2;
        }
        else {
            /* we'll get 8-bit pixels so align PIXELs to our byte burst size */
            scanimg_align_scanx( &scanx, ICE_DMA_BYTE_ALIGN, SCANX_ALIGN_PAD );
            scanimg_set_scanx( scanx );
            pic_bytes_per_row = scanx;
        }
    }

    XASSERT( (pic_bytes_per_row % ICE_DMA_BYTE_ALIGN)==0, pic_bytes_per_row );

    if( cmode==SCAN_CMODE_MONO ) {
        num_rows = num_lines;
        pic_num_channels = 1;
        pic_channels[0] = PIC_DMA_CHANNEL_MEVEN;
    }
    else {
        num_rows = num_lines;
        pic_num_channels = 3;
        pic_channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
        pic_channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
        pic_channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
    }

    for( i=0 ; i<pic_num_channels ; i++ ) {
        channel = pic_channels[i];

        picbuf = ice_dma_alloc_buffer( pic_bytes_per_row * num_rows );
        if( picbuf == NULL ) {
            dbg2( "%s failed to get %d bytes of memory for PIC buffer for channel %d\n",
                        __FUNCTION__, pic_bytes_per_row * num_rows, channel );

            /* clean up after ourselves before we leave */
            do {
                i--;
                pic_buffers[i] = CACHE_BYPASS_CLEAR( pic_buffers[i] );
                PTR_FREE( pic_buffers[i] );
            }
            while( i>0 );

            scanimg_close();
            scan_set_cmdq_interrupt( old_cmdq_isr );
            return SCANERR_OUT_OF_MEMORY;
        }

        dbg2( "%s channel=%d ptr=%p datalen=%d\n", __FUNCTION__, channel,
                    picbuf, pic_bytes_per_row*num_rows );

        /* put into the DMA-able memory area */
        picbuf = CACHE_BYPASS_SET( picbuf );

        /* initialize to a value so we can debug the buffer if something goes
         * wrong 
         */
        memset( picbuf, 0xee, pic_bytes_per_row * num_rows );
        cpu_dcache_writeback_region( picbuf, pic_bytes_per_row * num_rows );

        /* davep 04-Oct-2010 ; need to call channel specific open/close
         * functions because descriptor DMA needs extra initalization 
         */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
        scerr = pic_cwdma_channel_open( channel );
        XASSERT( scerr==SCANERR_NONE, scerr );
#endif

        pic_wdma_channel_reset( channel );
        pic_wdma_channel_enable( channel );
        /* davep 01-Jul-2013 ; passing in parameters that will crash in Linux
         * buf should work in non-virtual memory environments (this function
         * not updated to Linux)
         */
        pic_wdma_channel_load( channel, picbuf, (dma_addr_t)picbuf, num_rows, pic_bytes_per_row );
        pic_wdma_channel_start( channel );

        /* save the pointer so we can free it later */
        PTR_ASSIGN( pic_buffers[i], picbuf );
    }

    /* davep 04-Oct-2010 ; have to re-write the bitpack mode because recent
     * changes put the bitpack mode into the individual DMA channels.  The
     * above pic_wdma_channel_reset() wipes out the previous setting. 
     *
     * In earlier ASICs, the bitpack mode was stored in an overall config
     * register as it was in earlier ASICs. This code will cause no harm in
     * older ASICs.
     */
    pic_wdma_set_bitpack_mode( bitpack_mode );

    XASSERT( pic_wdma_get_bitpack_mode()==bitpack_mode, bitpack_mode );

    /* "PIC ain't done 'til the interrupt is run."  PIC DMA busy flag won't
     * clear until the interrupt is fired.
     */
    pic_interrupt_enable();

    /*
     *
     *  Fire in the hole!
     *
     */

    scerr = scanimg_run();
    XASSERT( scerr==0, scerr );

    /* Turn on scan block. After this call, we're ready to run. All we need is
     * some stuff pushed into SCMD.
     */
    scif_control(true,true);
    scif_clock(true);

    /* see scantest_sensor() to understand why I'm polling on scmd_done instead
     * of polling on the scan hardware itself
     */
    scmd_done = false;

    scif_dump();

    /* scan command to get our lines */
    scif_send_simple_scmd( true, sclkper, num_lines );

    /* wait for the command to finish */
    sanity_count = 0;
    while( !scmd_done )  {
        sanity_count++;

        /* if it's taking longer than 1 seconds something has gone very wrong */
        posix_sleep_seconds( 1 );
        XASSERT( sanity_count < 4, i );
    }

#ifdef SCANTEST_DEBUG
    dbg2( "sanity=%d\n", sanity_count );
#endif

    /* Wait for PIC to finish.
     *
     * This is a very stupid, brute force, ugly way to wait.
     */
    dbg2( "%s waiting for PIC to finish...\n", __FUNCTION__ );
    i = 0;
    sanity_count = 0;
    while( i<pic_num_channels ) {
        channel = pic_channels[i];

        /* davep 29-Mar-2010 ; we added chained descriptor DMA and removed the
         * old register-based DMA
         */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
        if( 1 ) {
            struct pic_cwdma_regs *regs;
            regs = pic_cwdma_get_regs( channel );
            if( !(regs->status & PIC_WDMA_STATUS_DMA_BUSY) ) {
                /* this channel is done; move to next */
                sanity_count = 0;
                i++;
            }
            else {
                /* wait a bit */
                posix_sleep_seconds( 5 );
                sanity_count++;
            }
        }
#else
        dbg2( "%s channel=%d status=%#010lx\n", __FUNCTION__, 
                    channel, *PIC_WDMA_DWSR(channel) );

        if( !(*PIC_WDMA_DWSR(channel) & PIC_WDMA_DWSR_A) ) {
            /* this channel is done; move to next */
            sanity_count = 0;
            i++;
        }
        else {
            /* wait a bit */
            posix_sleep_seconds( 1 );
            sanity_count++;
        }
#endif

#ifdef SCANTEST_DEBUG
        dbg2( "sanity=%d\n", sanity_count );
#endif

        /* check our escape hatch */
        if( sanity_count > 3 ) {
            break;
        }
    }

    /* Note: I'm not asserting on PIC failing to finish so I'll have a working
     * debug console. From there, I can better debug the problem(s).
     */
    if( i>=pic_num_channels ) {
        dbg2( "%s PIC finished successfully\n", __FUNCTION__ );
        test_success = true;
    }
    else {
        dbg2( "%s gave up waiting for PIC\n", __FUNCTION__ );
        test_success = false;
    }

#ifdef SCANTEST_DEBUG
    pic_dump();
#endif

    /* turn things off */
    scerr = scanimg_stop();
    XASSERT( scerr==0, scerr );

    for( i=0 ; i<pic_num_channels ; i++ ) {
        pic_wdma_channel_disable( pic_channels[i] );

        /* davep 04-Oct-2010 ; need to call channel specific open/close
         * functions because descriptor DMA needs extra initalization 
         */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
        pic_cwdma_channel_close( pic_channels[i] );
#endif
    }
    pic_reset();

    /* (clean up after ourselves) */
    for( i=0 ; i<pic_num_channels ; i++ ) {
#ifdef SCANTEST_DEBUG
        scanlog_hex_dump( pic_buffers[i], 256 );
#endif
        pic_buffers[i] = CACHE_BYPASS_CLEAR( pic_buffers[i] );
        PTR_FREE( pic_buffers[i] );
    }
    scanimg_close();
    scif_control(false,false);
    scif_clock(false);

    /* put back the previous handlers */
    scan_set_cmdq_interrupt( old_cmdq_isr );

    dbg2( "%s done\n", __FUNCTION__ );

    /* success? */
    return test_success ? SCANERR_NONE : SCANERR_GENERIC_FAIL;
}

scan_err_t scantest_with_data( void )
{
    scan_err_t scerr;

    dbg2( "%s start\n", __FUNCTION__ );

    scerr = scantest_sensor_with_data( SCAN_CMODE_MONO, 300, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor_with_data( SCAN_CMODE_MONO, 600, 25 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor_with_data( SCAN_CMODE_MONO, 300, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scantest_sensor_with_data( SCAN_CMODE_COLOR, 300, 100 );
    if( scerr != 0 ) {
        return scerr;
    }

    /* success! */
    return 0;
}

scan_err_t scantest_run_finger_test( void )
{
    scan_err_t scerr;
    uint32_t num32;
    int dpi;
    scan_cmode_t cmode;

    dpi = 300;
    cmode = SCAN_CMODE_MONO;

    /* davep 09-Feb-2011 ; adding scands */
    scerr = scands_get_integer_with_default( "fingertest_dpi", &num32, 300 );
    XASSERT( scerr==0, scerr );

    /* Normally I'd return SCANERR_INVALID_PARAM on the following sanity
     * checks.  However, by design, the scandbg caller will assert fail on a
     * return value != SCANERR_NONE. So, I return SCANERR_NONE.  Smile and
     * wave, boys. Smile and wave.
     */

    if( num32 != 300 && num32 != 600 && num32 != 1200 ) {
        /* finger test gives you the finger */
        dbg2( "%s bad scands dpi value %d; finger test not started\n", __FUNCTION__, num32 );
        return SCANERR_NONE;
    }
    dpi = num32;

    scerr = scands_get_integer_with_default( "fingertest_cmode", &num32, (uint32_t)SCAN_CMODE_MONO );
    XASSERT( scerr==0, scerr );

    if( (scan_cmode_t)num32 != SCAN_CMODE_MONO && (scan_cmode_t)num32 != SCAN_CMODE_COLOR ) {
        /* finger test gives you the finger */
        dbg2( "%s ignoring bad scands cmode value %d; finger test not started\n", __FUNCTION__, num32 );
        return SCANERR_NONE;
    }
    cmode = (scan_cmode_t)num32;

    /* davep 30-Mar-2008 ; nifty idea from SandraC's; bump to insane numlines to
     * do a finger test 
     */
    /* davep 30-Sep-2010 ; making cancelable */
    while( 1 ) { 
        scerr = scantest_sensor( cmode, dpi, INT_MAX );
        if( scerr==SCANERR_SCAN_CANCELLED ) {
            dbg2( "%s finger test canceled\n", __FUNCTION__ );
            break;
        }

        XASSERT( scerr==0, scerr );
    }

    return SCANERR_NONE;
}

scan_err_t scantest_run_all_tests( void )
{
    scan_err_t scerr;

    dbg1( "%s start\n", __FUNCTION__ );

    /* davep 04-Oct-2010 ; hacking on pic to see if I can run without leaving a
     * PIC overflow dangling
     */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_soft_setup();
    pic_cwdma_set_config(PIC_WDMA_DWOCR_DM_DISCARD);
#endif

    scerr = scantest_simple_sensor();
    XASSERT( scerr==0, scerr );

    /* davep 10-Nov-2010 ; disable this test for now (hard to get working, no
     * time)
     */
//    scerr = scantest_with_data();
//    XASSERT( scerr==0, scerr );

    dbg1( "%s done\n", __FUNCTION__ );

    return SCANERR_NONE;
}

