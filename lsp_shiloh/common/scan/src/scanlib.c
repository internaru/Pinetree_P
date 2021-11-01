/****************************************************************************** 
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Application Lib
 * Author: Scott Lloyd
 *         Brad Smith
 *         David Poole
 *         Eric Huang 
 *         Theron Luttman
 *         Sandra Capri
 *         Owen Zhang
 *         Burt Poppenga
 *         Bryan Allen
 *         Jun Wang
 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "list.h" 
#include "lassert.h" 
#include "memAPI.h"

#include "scancore.h" 
#include "scantypes.h"
#include "piehw.h"
#include "scanvars.h" 
#include "scantask.h"
#include "scandbg.h" 
#include "scanif.h" 
#include "scan.h" 
#include "cal.h" 
#include "scanpipe.h" 
#include "scanlib.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "pic.h"
#include "scanimg.h"
#include "scansen.h"
#include "fakescan.h"
#include "scancap.h"
#include "icefile.h"
#include "picdma_descrip.h"  /* ugh */
#include "scands.h"
#include "cal_common.h"
#include "scanman.h"
#include "safetylock.h"
#include "cisx.h"

/* Define to turn on more verbose debugging */
//#define SCANLIB_DEBUG  

#ifdef SCANLIB_DEBUG
  #define scanlib_dbg2 dbg2 
#else
  #define scanlib_dbg2(...) 
#endif

struct scanlib_scan_info {
    int pie_required_rows;
    int scan_required_rows;
};

static struct scanlib_scan_info scanlib_self;

static struct safety_lock scanlib_global_lock;

static scan_callback_fn_t scan_callback_fn;

/* 
 * We have a funny dance around sensor_bitmask. He who sets sensor_mask must be
 * he who clears it. scanlib has two main entry points: scanlib2_setup_run and
 * scanlib2_setup.  The setup_and_run is an outer layer that involves
 * calibration and will eventually call scanlib2_setup. scanlib2_setup is an
 * inner layer that is called by calibration and stuff like notchfind. The
 * sensor_bitmask must be preserved if we're called from the upper layer but
 * must set if we're called from the lower layer.  If setup_and_run is called,
 * that mask is used for inner calls through scanlib2_setup. If scanlib2_setup
 * is called directly, his sensor_mask is used.  No matter how scanlib is
 * called, the bitmask must be cleared properly on exit. 
 * davep 17-Jul-2013 
*/

//#define SENSOR_BITMASK_OWNER_NONE  0    /* uninitialized */
//#define SENSOR_BITMASK_OWNER_OUTER 1    /* scanlib2_setup_and_run */
//#define SENSOR_BITMASK_OWNER_INNER 2    /* scanlib2_setup / scanlib2_done */

static int scanlib_sensor_bitmask_owner;
static uint32_t scanlib_sensor_bitmask; 

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 02-Sep-2009
 *
 */

static void scanlib_set_callback( scan_callback_fn_t new_callback )
{
    scanlib_dbg2( "%s %p\n", __FUNCTION__, new_callback );

    scan_callback_fn = new_callback;
}

static scan_callback_fn_t scanlib_get_callback( void )
{
    return scan_callback_fn;
}

scan_err_t scanlib_call_msg_callback( scan_msg_t *msg )
{
    ASSERT( scan_callback_fn!=NULL );
    return scan_callback_fn( msg );
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 16-Jul-2013
 */

//static void set_scanlib_sensor_bitmask( int owner, const struct scanvars *sv )
void set_scanlib_sensor_bitmask( int owner, const struct scanvars *sv ) 
{
    struct scan_platform_capabilities scancap;

    if( scanlib_sensor_bitmask_owner != SENSOR_BITMASK_OWNER_NONE ) {
        /* Someone else got here first. Only that owner can change the bitmask. */
        ASSERT( scanlib_sensor_bitmask );
        return;
    }

    scanlib_sensor_bitmask = 1; // by default, select the main scan sensor only

    scancap_get_capabilities( &scancap );

    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_NSENSOR ) {
        scanlib_sensor_bitmask = 3; // select the main scan sensor and the upper sensor
        XASSERT( scancap.has_nsensor, sv->doc_src );
    }
    scanlib_sensor_bitmask_owner = owner;
}

//static void clear_scanlib_sensor_bitmask( int owner )
void clear_scanlib_sensor_bitmask( int owner )
{
    XASSERT( scanlib_sensor_bitmask_owner == SENSOR_BITMASK_OWNER_OUTER ||
             scanlib_sensor_bitmask_owner == SENSOR_BITMASK_OWNER_INNER,
             scanlib_sensor_bitmask_owner );

    if( scanlib_sensor_bitmask_owner != owner ) {
        /* Caller is not the owner. Only the owner can clear the bitmask. */
        return;
    }

    scanlib_sensor_bitmask_owner = SENSOR_BITMASK_OWNER_NONE;
    scanlib_sensor_bitmask = 0;
}

/**
 * \brief  Send simple status messages to scanman.
 *
 * Scanman is responsible for calling scan observers (e.g., the control panel).
 * We want to update the user as to what the scan subsystem is doing but
 * calibration is deep, deep within Scantask. So Scantask must be responsible
 * and tell Scanman about calibration. Scanman then tells observers who could
 * tell the user.
 *
 * \author David Poole
 * \date 01-Jun-2012
 */
#if 0
static void calibration_is_running( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_IN_PROGRESS;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void calibration_is_finished( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_DONE;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}
#else
void calibration_is_running( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_IN_PROGRESS;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}

void calibration_is_finished( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_DONE;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}
#endif

/**
 * \brief rearrange pixel area if necessary
 *
 * Rolled a ball of copy/paste code between scanlib2_setup() and icefile into
 * this function
 *
 * \author David Poole
 * \date 07-Jun-2011
 */

static void wrangle_pixel_area( struct scanvars *sv )
{
    struct scan_area pixel_area;
    uint32_t bytes_per_row;

    ASSERT( sv->hw_dpi_horiz > 0 );
    ASSERT( sv->hw_dpi_vert > 0 );

    /* davep 21-Apr-2008 ; we might need to modify the area depending on what
     * scanpipe says so we'll do our work with a copy 
     */
    scanvar_get_pixel_area( sv, &pixel_area );

    dbg2( "%s area x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__, 
                pixel_area.x, pixel_area.y,
                pixel_area.width, pixel_area.height );

    XASSERT( pixel_area.width > 0, (uint32_t)pixel_area.width );
    XASSERT( pixel_area.height > 0, (uint32_t)pixel_area.height );
    
    if( sv->use_pie ) {
        pipe_calc_rows( pixel_area.height, 
                        &scanlib_self.scan_required_rows, 
                        &scanlib_self.pie_required_rows );

        /* we have to scan more to completely fill PIE's last strip */
        if( scanlib_self.scan_required_rows != pixel_area.height ) {
            dbg2( "changing user_height_pixels from %ld to %d\n", 
                        pixel_area.height, scanlib_self.scan_required_rows );
            pixel_area.height = scanlib_self.scan_required_rows;
            scanvar_set_pixel_area( sv, &pixel_area );
        }
    }
    else {
        scanlib_self.scan_required_rows = pixel_area.height;

        /* we're not using PIE so this number will be ignored by scanpipe */
        scanlib_self.pie_required_rows = pixel_area.height;
    }

    /* sanity check: pixels should be happy at this point; check for PIC WDMA
     * aligned (fail the first time I notice something wrong)
     *
     * sv->pic_bpp should be 8 or 16
     */
    bytes_per_row = pixel_area.width*(sv->pic_bpp/8);
    XASSERT( (bytes_per_row % ICE_DMA_BYTE_ALIGN)==0, bytes_per_row );
}

/**
 * \brief Run scantask with data coming in from ICEFILE.
 *
 * Written as part of ICEFILE. We want to send known good images down to be
 * sent through PIE and HalfPack. ICEFILE reads from USB/TCP/Whatever, chunks
 * into icebufs, and sends to scanpipe.
 *
 * \author David Poole
 * \date 16-Mar-2006
 *
 * davep 07-Jun-2011 ; heavy rewrite to use icetest hw block
 */

static scan_err_t scanlib_run_from_icefile(void) 
{
    scan_err_t scerr;
    struct scanvars *sv;
    uint32_t scan_config;
    struct icefile_image_info image_info;
    uint32_t flags;
    uint32_t scanx;

#if !defined HAVE_SCAN_ICEFILE_SUPPORT
    dbg1( "%s icefile not in this build. No icefile for you.\n", __FUNCTION__ );
    return SCANERR_NOT_IMPLEMENTED;
#endif

    sv = scanvar_get();

    scan_config = scan_get_config();

    /* we shouldn't have gotten into this function unless our config told us to */
    XASSERT( scan_config & SCAN_CONFIG_ICEFILE, scan_config );

    // 08-Apr-2014 ; PIC wants to run sanity tests on some of the sensor data (scanx, etc)
    // set up some minimal configuration from the sensor without actually using it.
    set_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_INNER,sv);
    scanimg_set_sensor_bitmask(scanlib_sensor_bitmask);
    scerr = scansen_calc_scanx( sv->cmode, sv->dpi, &scanx );
    scanimg_set_scanx( scanx );

    /* davep 08-Jun-2011 ; we have to reset PIC here because we're not going
     * through calibrate() (which would normally reset pic)
     */
    pic_soft_setup();
    cal_soft_setup();

    /* davep 18-Sep-2007; adding ability to disable PIE (good for IQ testing) */
    if( scan_config & SCAN_CONFIG_NOPIE ) {
        dbg2("%s disabling image enhancements\n", __FUNCTION__ );
        scanvar_disable_image_enhancements( sv );
    }

    /* calculate scanvar's internal pixel area */
    scanvar_calc_pixel_area( sv ); 

    /* push pixels around for hardware limitations */
    wrangle_pixel_area( sv );

    memset( &image_info, 0, sizeof(struct icefile_image_info) );
    image_info.total_rows = sv->scan_area_pixels.height;
    /* Send along the margin pixels + the pixel width (we have to send enough
     * data to get past PIC's margins). Throw in a few extra to give PIC right
     * margin something to do.
     */
    image_info.pixels_per_row = sv->scan_area_pixels.x + sv->scan_area_pixels.width + 20;
    icefile_set_image_info( &image_info );

    /* davep 01-Mar-2013 ; PIC margin setup moved to scanpipe. Only thing left
     * from cal_finalize_setup() is cisx
     */
    cal_open_cisx( sv );

    /* davep 06-Feb-2012 ; adding cal to icefile */
//    scerr = icefile_cal_setup( sv );
//    if( scerr != SCANERR_NONE ) {
//        dbg1( "%s icefile_cal_setup failed scerr=%d\n", __FUNCTION__, scerr );
//        return scerr;
//    }

    /* don't support one_buffer or link_results here because we're not doing
     * the extra work to clean up afterwards
     */
    flags = PIPE_FLAG_DATA_FROM_ICEFILE ;
    scerr = pipe_start( sv->scan_area_pixels.width, 
                scanlib_self.scan_required_rows, 
                scanlib_self.pie_required_rows, 
                flags );

    if( scerr != SCANERR_NONE ) {
        dbg1( "%s pipe_start failed scerr=%d\n", __FUNCTION__, scerr );
//        icefile_cal_done();
        pipe_done();
        return scerr;
    }

    /* don't invert no matter what pic_start()-scanpipe.c did */
    pic_adcnorm_set_invert( false );

    /* davep 03-Jun-2011 ; gross beyond words */
#ifdef HAVE_PIC_DESCRIPTOR_DMA 
    pic_cwdma_late_launch();
#endif

    scerr = pipe_wait();

    pipe_done();

    clear_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_INNER);

//    icefile_cal_done();
    cal_done();

    return SCANERR_NONE;
}

/**
 * \brief pretend scan data comes from the network
 *
 * When pipenet is used, we receive buffers through icenet that look like they
 * came from PIC. They're fed into PIE and (maybe) HalfPack looking just as if we're
 * doing a real scan/copy job.
 *
 * Used for testing scan (PIE) and print (HalfPack) quality.
 *
 * \author David Poole
 * \date 18-Oct-2005
 *
 */

static scan_err_t scanlib_run_from_net(void) 
{
    /* davep 07-Jun-2011 ; removed for now; icefile changes mean
     * scan_get/set_icefile_size() gone
     */
//    scan_err_t scerr;
//    int scan_required_rows;
//    int width, height;
//    struct scanvars *sv;
//
//    sv = scanvar_get();
//
//    /* assume the user will send us 8-bit pixels; we will set up PIE to expect
//     * 8-bit pixels by pretending PIC is sending 8-bit pixels 
//     */
//    scanvar_set_pic_bpp( sv, 8 );
//
//    /* davep 13-Jun-2006 ; subvert icefile's command line until I can
//     * integrate TCP into icefile and get rid of this function completely
//     */
//    scan_get_icefile_size( &width, &height );
//    if( width==0 || height==0 ) {
//        dbg2( "icefile not set up\n" );
//        return SCANERR_INVALID_PARAM;
//    }
//    /* end davep 13-Jun-2006 */
//
//    /* davep 7-May-2007; adding ability to disable PIE (good for IQ testing) */
//    if( scan_get_config() & SCAN_CONFIG_NOPIE ) {
//        dbg2("%s disabling image enhancements\n", __FUNCTION__ );
//        scanvar_disable_image_enhancements( sv );
//    }
//
//    /* davep 21-Sep-2005 ; add in extra rows to pad for stripsize */
//    pipe_calc_rows( (int)height, &scan_required_rows, &scanlib_self.pie_required_rows );
//    if( scan_required_rows != height ) {
//        dbg2( "changing height from %ld to %d\n", height, scan_required_rows );
//        height = scan_required_rows;
//    }
//    
//    /* don't support one_buffer or link_results here because we're not doing
//     * the extra work to clean up afterwards
//     */
//    scerr = pipe_start( width, height, scanlib_self.pie_required_rows, PIPE_FLAG_DATA_FROM_NET );
//
//    if( scerr != SCANERR_NONE ) {
//        dbg2( "%s pipe_start failed scerr=%d\n", __FUNCTION__, scerr );
//        pipe_done();
//        return scerr;
//    }
//
//    scerr = pipe_wait();
//    pipe_done();
//
    return(0);
}

static void scanlib_lock( void )
{
    scan_err_t scerr;
//    int retcode;

    /* davep 22-Oct-2008 ;  if this assertion fails, someone is trying to call
     * scanlib2_setup() while a scanlib2_setup/run/done call are already
     * active.  
     *
     * scanlib2 is not recursive.  We cannot do a scan while we're doing a
     * scan.
     */
    scerr = safety_lock_try_lock( &scanlib_global_lock );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scanlib_unlock( void )
{
    safety_lock_unlock( &scanlib_global_lock );
}

bool scanlib_is_locked( void )
{
    scan_err_t scerr;

    /* Try to grab the lock. If we don't get the lock, assume scanlib has it.
     * If we get the lock, scanlib isn't running so release the lock
     * immediately.
     *
     * This is a poor excuse for a busy flag. Used as a crude mechnanism to
     * tell if there is a running scan.
     */
    scerr = safety_lock_try_lock( &scanlib_global_lock );
    if( scerr != SCANERR_NONE ) {
        return true;
    }
    safety_lock_unlock( &scanlib_global_lock );
    return false;
}

scan_err_t scanlib2_setup( void )
{
    scan_err_t scerr;
    struct scanvars *sv;

    scanlib_dbg2( "%s\n", __FUNCTION__ );
    
	init_pipe_dgb_cnt();	// add.lsh for debug
	
    /* prevent recursive calls into scanlib */
    scanlib_lock();

    /*
     *
     *   STARTUP / SETUP
     *
     */

    sv = scanvar_get();

    /* davep 02-Sep-2009 ; (Bug 13964) moving scan callback into
     * scanvar/scanlib. Assert is to make sure we cleaned up properly when we
     * left previously. If we're not running, no callback should be set.
     */
    XASSERT( scanlib_get_callback()==NULL, (uint32_t)scanlib_get_callback() );
    if( sv->scan_callback_fn != NULL ) {
        scanlib_set_callback( sv->scan_callback_fn );
    }

    /* Convert user area from 1/100" into pixels (might be calling this again
     * if we're coming from scanlib2_setup_and_run())
     *
     * Normal scans will go through scanlib2_setup_and_run() which will calc 
     * the pixel area.  Calibration needs to have enough brains to carefully
     * set up pixel area.
     *
     * Like Calibration, Notchfind doesn't go through scanlib2_setup_and_run().
     *
     * Peeking at width is a horrible kludge. :-(
     */
    if( sv->scan_area_pixels.width==0 ) {
        scanvar_calc_pixel_area( sv ); 
    }

    ASSERT( sv->hw_dpi_horiz > 0 );
    ASSERT( sv->hw_dpi_vert > 0 );

    /* davep 17-Jul-2013 ; maybe set up global sensor bitmask */
    set_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_INNER,sv);

    /* davep 13-Jul-2011 ; changing scansen_get_sensor_conf to need cmode,dpi
     * so can have resolution select in register settings
     */
    /* Construct the ScanImg object. If scanimg_open() is successful we must be
     * sure to call scanimg_close() again.
     */
    ASSERT( scanlib_sensor_bitmask );
    scerr = scanimg_open( sv->cmode, sv->hw_dpi_horiz, scanlib_sensor_bitmask);
    if( scerr != 0 ) {
        /* open can fail on hardware error */

        /* get rid of callback */
        scanlib_set_callback( NULL );

        scanlib_unlock();
        return scerr;
    }

    scerr = scanimg_setup();
    if( scerr!=0 ) {
        /* setup can fail on hardware error */
        scanimg_close();

        /* get rid of callback */
        scanlib_set_callback( NULL );

        scanlib_unlock();
        return scerr;
    }

    wrangle_pixel_area( sv );

    /* davep 01-Mar-2013 ; PIC margin setup moved to scanpipe. Only thing left
     * from cal_finalize_setup() is cisx
     */
    cal_open_cisx( sv );

    scerr = scan_cmdq_setup();
    if( scerr != 0 ) {
        XASSERT( 0, scerr );
        scanimg_close();

        /* get rid of callback */
        scanlib_set_callback( NULL );

        scanlib_unlock();
        return scerr;
    }

//    scif_dump();
//    pic_dump();

    scanlib_dbg2( "%s finish\n", __FUNCTION__ );

    return SCANERR_NONE;
}

scan_err_t scanlib2_run( uint32_t pipe_flags )
{
    scan_err_t scerr;
    const struct scanvars *sv;
    scan_err_t pipe_wait_scerr;
    scan_err_t cmdq_wait_scerr;
    uint32_t temp_sensor_mask, sens_num;

    scanlib_dbg2( "%s flags=0x%x\n", __FUNCTION__, pipe_flags );

    sv = scanvar_get();

    scerr = scanimg_run();
    if( scerr != 0 ) {
        return scerr;
    }

    /* pipe_start() will call scan_cmdq_open() and feed a set of initial
     * commands into scan command queue
     */

    scerr = pipe_start( sv->scan_area_pixels.width, 
                scanlib_self.scan_required_rows, 
                scanlib_self.pie_required_rows, 
                pipe_flags
                );

    if( scerr != SCANERR_NONE ) {
        dbg1( "%s pipe_start failed scerr=%d\n", __FUNCTION__, scerr );

        pipe_cancel();

        /* davep 04-Oct-2009 ; scan_cmdq_open() and scan_cmdq_close() are a bit
         * of a special obnoxious case.  To call open, we need to know the pic
         * buffer counts and sizes. So you'd think we should call close inside
         * pic_done().  But scan_cmdq is also responsible for start/stop the
         * sensor clocks. We want to turn off the sensor as fast as possible
         * when the scan completes.  So scanlib calls scan_cmdq_close() when
         * pipe_wait() finishes. 
         */
        scan_cmdq_close();

        /* Simply return the error. If scanlib2_run() fails, we will still call
         * scanlib2_done() to perform cleanup.
         */
        return scerr;
    }

    /* davep 03-Apr-2013 ; sanity test dual scan.  All of Scan block, CISX, and
     * PIC must be in agreement for dual channel mode.
     */
    ASSERT( scif_get_dual_channel_enabled()==pic_get_dual_channel_enabled() );
    ASSERT( pic_get_dual_channel_enabled()==cisx_get_dual_channel_enabled() );

    /* davep 27-Mar-2013 ; XXX temp debug */
//    dbg2( "%s %d\n", __FUNCTION__, __LINE__ );
//    scif_dump();
//    cisx_dump();
//    pic_dump();

    /* davep 01-Mar-2013 ; sanity check PIC's calibration LUT configuration */
    temp_sensor_mask = scanlib_sensor_bitmask;
    for (sens_num=0;sens_num < scansen_get_num_sensors();sens_num++) {
        if (temp_sensor_mask & 1) {
            pic_prnudsnu_set_block(sens_num);
            pic_prnudsnu_dma_set_block(sens_num); 
            prnudsnu_sanity_check(sv->cmode);
        }
        temp_sensor_mask = temp_sensor_mask >> 1;
    }

    scan_cmdq_launch();

    pipe_wait_scerr = pipe_wait();
    dbg2( "%s pipe_wait_scerr=%d\n", __FUNCTION__, pipe_wait_scerr );

    /* Ignore pipe_wait's return code and do normal cleanup. NOTE: if anything
     * after this point changes and we start assuming pipe_wait() always
     * succeeds, make sure to check the return code!
     */

    //Waiting for scan_cmdq telling us it is in the ready state.
    // This function will block until the scan_cmdq is ready. We
    // need this because we must wait until the motor is done and
    // stopped before we close the scan_cmdq.
    cmdq_wait_scerr = scan_cmdq_wait_for_ready();
    dbg2( "%s cmdq_wait_scerr=%d\n", __FUNCTION__, cmdq_wait_scerr );

    scan_cmdq_close();

    scanlib_dbg2( "%s done\n", __FUNCTION__ );

    if( pipe_wait_scerr!=SCANERR_NONE || cmdq_wait_scerr!=SCANERR_NONE ) {
        /* awww, failure! if both bad, return the cmdq wait since it's probably
         * worse (e.g., paper jam)
         */
        if( cmdq_wait_scerr!=SCANERR_NONE ){
            return cmdq_wait_scerr;            
        }
        return pipe_wait_scerr;
    }

    /* success! */
    return SCANERR_NONE;
}

void scanlib2_done( void )
{
    scanlib_dbg2( "%s start\n", __FUNCTION__ );

    scanimg_stop();
    scanimg_close();

    pipe_done();

    cal_done();

    /* clean up our globals ("destructor") */
    memset( &scanlib_self, 0, sizeof(scanlib_self));

    /* davep 02-Sep-2009 ; moving callback into scanlib */
    scanlib_set_callback( NULL );

    /* davep 17-Jul-2013 ; maybe clear the bitmask */
    clear_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_INNER);
    
    scanlib_unlock();

    scanlib_dbg2( "%s finish\n", __FUNCTION__ );
}

static void scanlib_scan_end( struct scanvars **psv )
{
    const struct scanvars *tmp_sv;
    struct scanvars *sv;

    PTR_ASSIGN( sv, *psv );

    /* make sure our stack is clean */
    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==sv, tmp_sv->id );
    tmp_sv = NULL;

    /* Scanlib no longer has control of the scanvar. If we're using ScanMan,
     * the scanvar has been sent back in a SMSG_PAGE_END. Then sv will be NULL
     * after the call to scantask_scan_end().
     *
     * But if we're not using ScanMan (such as with internal scans or test/debug
     * scans), sv will not be NULL after the call to scantask_scan_end(). We
     * have to return control of the scanvar back to the caller.
     *
     * For example, cmdline.c handles the "scan var new"/"scan start" and is
     * also responsible for allocating/freeing his own scanvar.
     */
    scantask_scan_end( &sv );
    if( sv != NULL ) {
        /* we'll pass the scanvar back to the caller */
        PTR_ASSIGN( *psv, sv );
    }
}

static scan_err_t scanlib_run_debug_scan( uint32_t scan_config )
{
    scan_err_t scerr;
    struct scanvars *sv;

    scanlib_lock();

    sv = scanvar_get();

    /* davep 02-Sep-2009 ; (Bug 13964) moving scan callback into
     * scanvar/scanlib. Assert is to make sure we cleaned up properly when we
     * left previously. If we're not running, no callback should be set.
     */
    XASSERT( scanlib_get_callback()==NULL, (uint32_t)scanlib_get_callback() );
    if( sv->scan_callback_fn != NULL ) {
        scanlib_set_callback( sv->scan_callback_fn );
    }

    scerr = SCANERR_NONE;

    /* what to do today what to do today... */

    if( scan_config & SCAN_CONFIG_ICEFILE ) {
        scerr = scanlib_run_from_icefile();
    }
    else if( scan_config & SCAN_CONFIG_PIPENET ) {
        scerr = scanlib_run_from_net();
    }
    else if( scan_config & SCAN_CONFIG_FAKESCAN ) {
        scerr = fakescan_run();
    }
    else {
        /* um... why are we here? */
        XASSERT(0, scan_config );
    }

    if( scerr==SCANERR_NONE ) {
        scanlib_scan_end( &sv );
    }

    scanlib_set_callback( NULL );

    scanlib_unlock();

    return scerr;
}

scan_err_t scanlib2_setup_and_run( struct scanvars **psv )
{
    scan_err_t scerr, cal_scerr, final_scerr;
    struct scanvars *sv;
    struct scanvars *cal_sv;
    struct scanvars *tmp_sv;
    bool force_cal;
    struct scan_platform_capabilities scancap;
    uint32_t scan_config;
    uint32_t flag;

    dbg1( "[S_L] Step2. %s\n", __FUNCTION__ );

    /* initialize our globals to defaults ("constructor") */
    memset( &scanlib_self, 0, sizeof(scanlib_self));

    /* take control of the passed scanvar */
    PTR_ASSIGN( sv, *psv );
    scanvar_push( sv );

    /* davep 26-Mar-2013 ; add scands debug hook to dump the scanvar */
    scerr = scands_get_integer_with_default( "dump_siqfile", &flag, 0 );
    if( scerr==SCANERR_NONE && flag ) {
        scanvar_to_siqfile( scanvar_peek() );
    }

    /* davep 01-Feb-2012 ; adding chipgap correction */
    scands_get_integer_with_default("chipgap", &flag, sv->use_fw_margins );
    sv->use_fw_margins = flag;

    /* as of this writing, chipgap interpolation only works with 600-dpi */
    if( sv->use_fw_margins && sv->hw_dpi_horiz != 600 ) {
        sv->use_fw_margins = false;
    }
    /* davep 16-Jul-2013 ; disable FW chipgap correction when scanning from ADF */
    if( sv->doc_src != SCAN_DOCUMENT_SOURCE_FLATBED ) {
        sv->use_fw_margins = false;
    }

    scan_config = scan_get_config();

    /* davep 02-Nov-2005 ; if we're using PIENET, jump to a different
     * scanlib_run so we don't start the sensor
     */
    /* davep 17-Mar-2006 ; if we're using ICEFILE, jump to a different
     * scanlib_run so we don't start the sensor
     */
    /* davep 17-Aug-2007 ; add sending myself off to do a fake scan */
    if( (scan_config & SCAN_CONFIG_ICEFILE) ||
        (scan_config & SCAN_CONFIG_PIPENET) ||
        (scan_config & SCAN_CONFIG_FAKESCAN) ) {

        scerr = scanlib_run_debug_scan( scan_config );
        if( scerr == SCANERR_NONE ) {
            return scerr;
        }
        final_scerr = scerr;
        goto leave;
    }

    /* davep 26-Jul-2009 ; add sanity check if our platform indeed has an
     * ADF (require caller to be smart enough to know not to scan from adf
     * when we don't have an adf)
     */
    /* davep 08-Sep-2011 ; adding duplex support */
    scancap_get_capabilities( &scancap );
    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF ) {
        XASSERT( scancap.has_adf, sv->doc_src );
    }
    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
        XASSERT( scancap.has_adf, sv->doc_src );
        XASSERT( scancap.has_duplex, sv->doc_src );
    }
    /* davep 03-Apr-2013 ; adding duplex with multiple sensors. */
    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_NSENSOR ) {
        XASSERT( scancap.has_nsensor, sv->doc_src );
    }
    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_DUPLEX) {
        if( scancap.has_nsensor ) {
            /* change the docsrc to plain ADF+NSENSOR. "ADF_DUPLEX" implies
             * mechanical duplex. Instead, the duplex for nsensor enables a 2nd
             * set of data paths.
             */
            sv->doc_src = SCAN_DOCUMENT_SOURCE_ADF_NSENSOR;
        }
    }

    /* davep 17-Jul-2013 ; set up global sensor bitmask if necessary */
    set_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_OUTER,sv);

    /* convert user area from 1/100" into pixels */
    scanvar_calc_pixel_area( sv ); 

    dbg2( "%s user area in pixels x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__,
                sv->scan_area_pixels.x,
                sv->scan_area_pixels.y,
                sv->scan_area_pixels.width,
                sv->scan_area_pixels.height );

    /* make a calibration scanvar that is optimized for our scan destination */
    cal_sv = scanvar_cal_newcopy( sv );
    if( cal_sv==NULL ) {
        dbg1( "scantask failed to get cal scanvars\n" );

        /* clean up stack, send the scanvar back to the caller */
        tmp_sv = scanvar_pop();
        XASSERT( tmp_sv==sv, tmp_sv->id );
        PTR_ASSIGN( *psv, sv );

        scanlib_set_callback( NULL );

        /* Note I'm not calling scanlib_scan_end() here because our scan failed
         * to even start. 
         */
        clear_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_OUTER);
        return SCANERR_OUT_OF_MEMORY;
    }

    /* make our calibration configuration the active configuration */
    scanvar_push( cal_sv );

    /* davep 12-Oct-2006 ; adding ability to cal-to-host */
    force_cal = false;
    if( sv->scan_callback_fn && sv->capture_cal ) {
        dbg2("doing calibration capture\n" );
        cal_sv->scan_callback_fn = sv->scan_callback_fn ;
        cal_sv->capture_cal = true;
        force_cal = true;
    }

    /* davep 29-Jan-2007 ; adding cal-only */
    if( scan_config & SCAN_CONFIG_CAL_ONLY ) {
        dbg2( "stopping after calibration\n" );
        force_cal = true;
    }

    if ( sv->cal_mode == SCANVAR_CAL_ALWAYS )
    {
        dbg2( "scanvar->cal_mode requires cal\n" );
        force_cal = true;
    }
    
    /* davep 01-Jun-2012 ; add calibrating start/stop messages so scanman can
     * update observers
     */
    calibration_is_running();

    /* davep 04-Jun-2012 ; XXX temp debug ; EFT failure seems to happen during
     * cal --force it to always cal
     */
//    force_cal = true;
	
    cal_scerr = calibrate(scanlib_sensor_bitmask, cal_sv->hw_dpi_horiz, cal_sv->cmode, force_cal );
        
    if( cal_scerr != 0 ) {
        dbg1("calibration failed; not scanning\n" );
        /* keep going so we can cleanup the cal scanvar */
    }

    /* davep 01-Jun-2012 ; add calibrating start/stop messages so scanman can
     * update observers
     */
    calibration_is_finished();

    /* take the cal configuration off the stack */
    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==cal_sv, tmp_sv->id );
    tmp_sv = NULL;

    /* davep 01-Jan-2006 ; TODO merge BDR/BDE information from the cal scanvar
     * into the tip scanvar. BDR/BDE need to compliment each other and are the
     * only place where data between cal/pic needs to cross over to
     * scanpipe/pie.
     */
    
    scanvar_delete( &cal_sv );

    if( cal_scerr != 0 ) {
        /* we already reported the error; since cal failed we can't scan */
        final_scerr = cal_scerr;
        goto leave;
    }

    /* davep 12-Oct-2006 ; if we're doing a cal-to-host, we need to stop here
     * since we've already capture the data we're interested in
     */
    if( sv->capture_cal ) {
        dbg2("only capturing calibration; leave now\n" );

        scanlib_scan_end( &sv );
        if( sv != NULL ) {
            /* we'll pass the scanvar back to the caller */
            PTR_ASSIGN( *psv, sv );
        }

        final_scerr = SCANERR_NONE;
        goto leave;
    }

    /* davep 26-Jan-2007 ; for testing calibration from command line */
    if( scan_config & SCAN_CONFIG_CAL_ONLY ) {
        dbg2( "only doing calibration; leave now\n" );
        final_scerr = SCANERR_NONE;
        goto leave;
    }

    /* davep 12-Dec-2006 ; adding ability to disable PIE (good for IQ testing) */
    if( scan_config & SCAN_CONFIG_NOPIE ) {
        dbg2("%s disabling image enhancements\n", __FUNCTION__ );
        scanvar_disable_image_enhancements( sv );

        /* davep 18-Jan-2013 ; inject a bug */
        if( scan_config & SCAN_CONFIG_CAFFEINE ) {
            if( sv->cmode==SCAN_CMODE_COLOR ) {
                scanvar_set_all_black_white_clip( sv, 0, 255 );
                sv->use_pic_bdr = true;
                sv->use_pic_bdr_lut = true;
                scanvar_set_gamma(sv,12);
            }
        }
    }

    //Allow the scan mech prepare the page for scanning; stage the page
    //We should always be doing a document scan here. --Theron 080730
    dbg1( "[S_L] Step3. scanmech_prepare()\n");
    
    scerr = scanmech_prepare(SCAN_TARGET_TYPE_DOC);
    if( scerr != 0 ) {
        dbg1( "[S_L] Error : %s scanmech_prepare failed scerr=%d\n", __FUNCTION__, scerr );
        final_scerr = scerr;

        /* davep 04-Sep-2009 ; if we had a "clean" failure, we need to cleanup
         * the paper.  Added this to handle the case of cancelling while
         * staging (we don't want to leave paper in the mech).
         */
        if( scerr==SCANERR_SCAN_CANCELLED ) {
            scerr = scanmech_done( SMECH_DONE_FLAG_CANCEL );
            /* ignore error */
        }

        goto leave;
    }

    /* If scanlib2_setup() fails, cannot call scanlib2_run() or scanlib2_done().
     * We're hosed.
     */
    dbg1( "[S_L] Step4. scanlib2_setup()\n");
    
    scerr = scanlib2_setup();   
    if( scerr != 0 ) {
        dbg1( "[S_L] Error : %s scanlib2_setup failed scerr=%d\n", __FUNCTION__, scerr );
        final_scerr = scerr;

        /* davep 05-Jan-2011 ; call scanmech_done() on setup failure*/
        scerr = scanmech_done( SMECH_DONE_FLAG_CANCEL );
        /* ignore error; want to return the scanlib2_setup() failure */

        goto leave;
    }

    /* davep 23-Mar-2008 ; scanlib2_setup() starts the scanimg subsystem (sensor
     * and afe) from scratch. Ask cal to load the led and afe settings into the
     * scanimg subsystem
     *
     * BIG FAT WARNING! Calibration was run with values from calvar.  Now I'm
     * passing in values from sv instead.  The members should be the same due to
     * the copy constructor (which created calvar based on sv) but we'll have to
     * be careful.
     */
    scerr = cal_load_from_cache( sv->hw_dpi_horiz, sv->cmode, true,
                                 sv->cal_mode, scanlib_sensor_bitmask );
    XASSERT( scerr==0, scerr );

	dbg1( "[S_L] Step5. scanlib2_run(h_dpi:%d, v_dpi:%d, pie_h_s:%d/%d, pie_v_s:%d/%d, h_s:%d/%d, v_s:%d/%d)\n", sv->hw_dpi_horiz, sv->hw_dpi_vert, 
												sv->pie_scale.x_numer, sv->pie_scale.x_denom, sv->pie_scale.y_numer, sv->pie_scale.y_denom,
												sv->scale.x_numer, sv->scale.x_denom, sv->scale.y_numer, sv->scale.y_denom);
	
    scerr = scanlib2_run( PIPE_FLAG_NULL );

    if( scerr != 0 ) {
        dbg2( "%s scanlib2_run failed scerr=%d\n", __FUNCTION__, scerr );
    }

    /* at this point, we'll return what scanlib2_run() gave us */
    final_scerr = scerr;

    /* NOTE! even if scanlib2_run() fails, must call scanlib2_done() */
    scanlib2_done();
    /* davep 04-Mar-2009 ; Oops. We shouldn't send PAGEEND if we failed with an
     * error. We send SMSG_SCAN_FAILED (in scantask.c). So even though we have
     * to call scanlib2_done() even on error, we should skip over the
     * scanlib_scan_end(). 
     */
    if( final_scerr != 0 ) {
        
        /* scanlib2_done() doesn't call scanmech_done() since we split BOF handling
         * from PAGEND a couple months ago. But need to call scanmech_done() anyway
         * to handle page eject (I think?)
         */
		dbg1( "[S_L] Step7. scanlib2_run() Fail(%d)!!\n", final_scerr);
		
        scerr = scanmech_done( SMECH_DONE_FLAG_CANCEL );
        /* ignore error because we're leaving with an error already */

        goto leave;
    }
    else{
    	dbg1( "[S_L] Step7. scanlib2_run() Success(%d)!!\n", final_scerr);
    }

    /* keep going on error because we still must call scan_end() and
     * scanmech_done()
     */

    scanlib_scan_end( &sv );
    if( sv != NULL ) {
        /* we'll pass the scanvar back to the caller */
        PTR_ASSIGN( *psv, sv );
    }

	scerr = scanmech_done( SMECH_DONE_FLAG_SUCCESS );
    if( scerr != 0 ) {
        dbg1( "[S_L] %s scanmech_done() failed scerr=%d\n", __FUNCTION__, scerr );
        final_scerr = scerr;
    }

    /* Note: if we add more code here (between the scanmech_done() and the 'leave'
     * target), perhaps we need a 'goto leave' on catching the scanmech_done()
     * error?
     */

leave:
    clear_scanlib_sensor_bitmask(SENSOR_BITMASK_OWNER_OUTER);

    /* We can get here when handling an error so make sure our stack is clean.
     * But don't free the scanvar; whoever allocated the scanvar is responsible
     * for freeing it.
     */
    if( scanvar_peek() ) {
        tmp_sv = scanvar_pop();
        XASSERT( tmp_sv==sv, tmp_sv->id );
        tmp_sv = NULL;

        /* we'll pass the scanvar back to the caller */
        PTR_ASSIGN( *psv, sv );
    }

    /* stack should be clean before we leave */
    XASSERT( scanvar_peek()==NULL, scanvar_peek()->id );

    return final_scerr;
}


/**
 * \brief  create a global lock to prevent more than one active
 * scanlib2_setup/run/done
 *
 * We can't scan within a scan. Sorry. :-P
 *
 * \author David Poole
 * \date 22-Oct-2008
 *
 */

scan_err_t scanlib_onetime_init( void )
{
    safety_lock_init( &scanlib_global_lock );

    return 0;

    /* call this function so compiler won't complain */
    scanlib_get_callback();
}

/**
 * \brief Cancel a scan from within the scantask thread itself.
 *
 * \author David Poole
 * \date 02-Mar-2011
 *
 */

scan_err_t scanlib_cancel_self( void )
{
    scan_err_t scerr;
    scan_msg_t msg;

    /* Only Scantask may cancel Scantask. Everyone else, use scantask_scan_cancel(). */
#ifdef __KERNEL__
    /* TODO */
#else
    XASSERT( pthread_equal( pthread_self(), scantask_get_id()), (uint32_t)pthread_self() );
#endif

    /* For now, just send a cancel message to myself */

    memset( &msg, 0, sizeof(scan_msg_t) );
    msg.msgType = SMSG_SCAN_CANCEL;

    /* We're sending a message to ourself. Use a timeout with an assfail so we
     * avoid deadlocks. (Don't block forever in case message queue is full.)
     */
    scerr = scantask_msg_send( &msg, 1*USEC_PER_SECOND );
    XASSERT( scerr==SCANERR_NONE, scerr );

    return SCANERR_NONE;
}
