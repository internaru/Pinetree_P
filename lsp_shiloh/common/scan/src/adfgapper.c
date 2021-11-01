/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file adfgapper.c
 *
 * \brief Functions used by scanpipe.c for handling adf page gap tasks.
 *
 *  We can't pause while scanning ADF. So there are a variety of tasks we
 *  schedule for a page gap.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "list.h"
#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanlog.h"
#include "scanalyzer.h"
#include "safetylock.h"
#include "icedma.h"
#include "scancmdq.h"
#include "scanvars.h"
#include "adfpath.h"
#include "adfgapper.h"
#include "scanpipe.h"
#include "pipecut.h"
#include "scanmech.h"
#include "adfsensor.h"
#include "scanmem.h"

/* davep 09-Jan-2013 ; experimenting with client pause; this is temporary until
 * I can integrate it into adf_page_gapper structure
 */
static int client_pause;

static scan_err_t duplex_dance( int *page_side ) 
{
    scan_err_t flip_scerr, eject_scerr, stage_scerr;

    /* davep 01-Sep-2011 ; ZOMG! Duplex !!!!!!!!! */

    flip_scerr = eject_scerr = stage_scerr = SCANERR_GENERIC_FAIL;

	dbg1( "%s called\n", __FUNCTION__ );
	
    if( *page_side==1 ) {
        /* flip to back side of page */

        dbg2( "%s flip start\n", __FUNCTION__ );
        SCANALYZER_LOG( LOG_ADF_DUPLEX_FLIPPING, 1 );

        flip_scerr = smech_adf_duplex_flip_page();

        SCANALYZER_LOG( LOG_ADF_DUPLEX_FLIPPING, 0 );
        dbg2( "%s flip done scerr=%d\n", __FUNCTION__, flip_scerr );

        *page_side=2;

        return flip_scerr;
    }

    /* as of this writing (08-Sep-2011) there are only two sides to any given
     * sheet of paper
     */
    XASSERT( *page_side==2, *page_side );

    /* At this point, we're finished scanning this particular page. Eject
     * current page, attempt to load next.
     */

    /* turn off the adf sensor callback or we'll get a LOT of tof/bof as the
     * paper is ejected and new page is staged
     */
    adf_sensor_pip_callback_disable();

    dbg2( "%s eject start\n", __FUNCTION__ );
    SCANALYZER_LOG( LOG_ADF_DUPLEX_EJECTING, 1 );

    eject_scerr = smech_adf_duplex_eject();

    SCANALYZER_LOG( LOG_ADF_DUPLEX_EJECTING, 0 );
    dbg2( "%s eject done scerr=%d\n", __FUNCTION__, eject_scerr );

    if( eject_scerr!=SCANERR_NONE ) {
        dbg2( "%s eject failed scr==%d\n", __FUNCTION__, eject_scerr );
        return eject_scerr;
    }

    if( !adf_sensor_paper_present() ) {

        /* buh bye */
        return SCANERR_NO_PAPER_LOADED;
    }

    /* we still have paper in the hopper, load another page */
    dbg2( "%s stage start\n", __FUNCTION__ );
    SCANALYZER_LOG( LOG_ADF_DUPLEX_STAGING, 1 );

    stage_scerr = smech_adf_duplex_stage_input_sheet();

    SCANALYZER_LOG( LOG_ADF_DUPLEX_STAGING, 0 );
    dbg2( "%s stage done scerr=%d\n", __FUNCTION__, stage_scerr );

    *page_side=1;

    /* now that the new page is successfully staged (and weird bouncing
     * hopefully done) re-enable the tof/bof callback 
     */
    adf_sensor_pip_callback_enable();

    /* throw fake TOF event if we did indeed find the paper leading edge */
    if( stage_scerr==SCANERR_NONE && adf_sensor_paper_in_path() ) {
        smech_adf_paper_event_callback( SCAN_ADF_PAPER_TOF, 1, 0 );
    }

    return stage_scerr;
}

void adf_page_gapper_init( struct adf_page_gapper *adfgapper, 
                            uint32_t low_memory_trigger, uint32_t max_rows_trigger )
{
    memset( adfgapper, 0, sizeof(struct adf_page_gapper) );

    adfgapper->max_distance_traveled_rows = max_rows_trigger;
    adfgapper->required_memory_for_next_page = low_memory_trigger;

    /* davep 25-Jan-2013 ; clear client pause request flag */
    client_pause = 0;

//    SCANALYZER_ENABLE_LOG( LOG_ADF_DUPLEX_FLIPPING ); 
//    SCANALYZER_ENABLE_LOG( LOG_ADF_DUPLEX_EJECTING ); 
//    SCANALYZER_ENABLE_LOG( LOG_ADF_DUPLEX_STAGING ); 

}

scan_err_t adf_page_gapper_resume( struct adf_page_gapper *adfgapper, struct adf_paper_path *adfpath )
{
    scan_err_t scerr;

    dbg2( "%s\n", __FUNCTION__ );

    scerr = SCANERR_NONE;

    /* davep 01-Sep-2011 ; adding duplex */
    if( adfpath->use_duplex ) {
        scerr = duplex_dance( &adfpath->duplex_page_side );

        if( scerr != SCANERR_NONE ) {
            if( scerr==SCANERR_NO_PAPER_LOADED ) {
                dbg2( "%s no more paper; ending scan\n", __FUNCTION__ );
                /* successful end of our duplex scan */
                pipe_cutter_flush();
            }
            return scerr;
        }
    }

    /* If not enough free memory to resume, come back later. */
    if( scanmem_get_free_size() < adfgapper->required_memory_for_next_page ) {
        /* Return success; returning a failure triggers end of the scan. Low
         * memory is transitory. We'll try again later (see pipe_poll()).
         */

        dbg2( "%s free=%d require=%d\n", __FUNCTION__, 
                    scanmem_get_free_size(), adfgapper->required_memory_for_next_page );

        return SCANERR_NONE;
    }

    /* davep 09-Jan-2013 ; experiment with client pause */
    if( client_pause ) {
        dbg2( "%s waiting for client to clear\n", __FUNCTION__ );
#if 0	// mod.lsh for USER CANCEL
        return SCANERR_NONE;
#else
		return SCANERR_SCAN_CANCELLED;	// mod.lsh pinetree not support pause/resume. should stop immediately.
#endif
    }

    /* we are ready to resume */

    /* burtp 01-Oct-2009; Looks like everyone else is ready to go, slow
     *  things down a bit if we are in meltdown mode.
     */
    if (smech_is_motor_overheating()) {
    	dbg1( "[S_G] Warning : Overheating Waiting!!\n");
        posix_sleep_seconds(5);
    }

    ASSERT( !adfgapper->stop_at_next_page_gap );

    adfgapper->resume_on_mech_ready = false;
    scan_cmdq_resume( SCAN_CMDQ_PAUSE_CAUSE_SCANPIPE );
    dbg1( "[S_G] Gapper resume!!\n");

    return SCANERR_NONE;
}

void adf_page_gapper_update_distance( struct adf_page_gapper *adfgapper, 
                                      struct adf_paper_path *adfpath, 
                                      uint32_t num_rows ) 
{
    /* davep 16-Jul-2009 ; adding pause between pages on ADF so can
     * reset the motor move to prevent overflow
     */
    adfgapper->distance_traveled_rows += num_rows;

    /* if we haven't already decided to pause and
     * we're not trying to resume
     */
    if( !adfgapper->stop_at_next_page_gap && !adfgapper->resume_on_mech_ready ) {
        /* davep 17-Jul-2009 ; stop after a certain distance because our
         * motor driver can't tolerate rollover
         */
        if( adfgapper->distance_traveled_rows > adfgapper->max_distance_traveled_rows  ) 
        {
            dbg1( "%s will stop at page gap for rollover\n", __FUNCTION__ );
            ASSERT( !adfgapper->stop_at_next_page_gap );
            adfgapper->stop_at_next_page_gap = true;
        }
    }
}

scan_err_t adf_page_gapper_poll( struct adf_page_gapper *adfgapper, 
                                 struct adf_paper_path *adfpath )
{
    /* davep 09-Sep-2011 ; XXX temp debug */
    dbg2( "[S_G] free=%d required=%d\n",  
                scanmem_get_free_size(), adfgapper->required_memory_for_next_page );

    /* if we have already scheduled a stop
     * or we're trying to resume
     *      return now
     *
     * otherwise, run our tests to see if we need to stop
     */
    if( adfgapper->stop_at_next_page_gap || adfgapper->resume_on_mech_ready ) {
        return SCANERR_NONE;
    }

    /* davep 17-Jul-2009 ; stop after a certain distance because our
     * motor driver can't tolerate rollover
     */
    if( adfgapper->distance_traveled_rows > adfgapper->max_distance_traveled_rows  ) 
    {
        dbg1( "[S_G] Warning : %s will stop at page gap for rollover\n", __FUNCTION__ );
        adfgapper->stop_at_next_page_gap = true;
    }

    /* davep 05-Aug-2009 ; stop when getting low on memory, too */
    if( scanmem_get_free_size() < adfgapper->required_memory_for_next_page ) 
    {
        dbg1( "[S_G] Warning : %s will stop at page gap for memory (f_m:%d, r_m:%d)\n", __FUNCTION__, scanmem_get_free_size(), adfgapper->required_memory_for_next_page );
        adfgapper->stop_at_next_page_gap = true;
    }

    /* burtp 01-Oct-2009; check to see of we need to stop at the next page gap
     * due to a motor meltdown.
     */
    if( smech_is_motor_overheating() ) 
    {
        dbg1( "[S_G] Warning : %s will stop at page gap for overheat\n", __FUNCTION__ );
        adfgapper->stop_at_next_page_gap = true;
    }

    /* davep 09-Jan-2013 ; experimenting with client pause */
    if( client_pause ) {
        dbg1( "[S_G] Warning : %s will stop at page gap at client request\n", __FUNCTION__ );
        dbg1("[S_CANCEL] step4. pause by client!!\n");
        adfgapper->stop_at_next_page_gap = true;
    }

    return SCANERR_NONE;
}

scan_err_t adf_page_gapper_test_for_pause( struct adf_page_gapper *adfgapper, struct adf_paper_path *adfpath )
{
    scan_err_t scerr;
    int slop;
    const struct scanvars *sv = scanvar_get();

    /* davep 16-Sep-2011 ; need to make sure we capture PAST the bottom of
     * page. We have to capture the strip with the last marker. 
     *
     * This code is a h@ck. I'm under deadline pressure. Bummer, huh?
     */
    slop=0;
#if 0
    if( adfgapper->stop_at_next_page_gap && 
        adfpath->bof_marker && 
        adfpath->use_duplex && adfpath->duplex_page_side==2 ) {
//        dbg2( "%s adding slop for back side pause\n", __FUNCTION__ );
        slop += 66;
    }
#endif
    /* davep 18-Nov-2011 ; XXX temp hack for pipecutter flush on pause (need to
     * capture past bottom of page so the flush will eject the last buffer so
     * we have a page_end)
     */
    if(sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF) {   //mod.kks
		slop += 66;
	}
	else if(sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_NSENSOR){
		if( sv->hw_dpi_vert == 300)
			slop += 1000; 
		else
			slop += 1000*2; 
	}
	else{
		slop += 1;
	}

    /* check if we need to make a pit stop and
     *       if we've recently seen a BOF and
     *       if we're enough past the BOF we should be in the interpage gap.
     */
    if( adfgapper->stop_at_next_page_gap && 
        adfpath->bof_marker  &&
        adfpath->row_counter > adfpath->bof_marker + slop ) 
    {
 //       dbg2( "%s recent=%d row_counter=%d tobof=%d\n", __FUNCTION__, adfpath->bof_marker, 
 //                   adfpath->row_counter, adfpath->rows_to_bof );
	
        XASSERT( !adfgapper->resume_on_mech_ready, adfgapper->resume_on_mech_ready );

        scerr = scan_cmdq_start_pause( SCAN_CMDQ_PAUSE_CAUSE_SCANPIPE );
        if( scerr==0 ) {
            adfgapper->stop_at_next_page_gap = false;
            adfgapper->resume_on_mech_ready = true;
            adfgapper->distance_traveled_rows = 0;
        }
        else { 
            /* scan_cmdq_start_pause() could ignore us but anything
             * else is a fatal error
             */
            XASSERT( scerr==SCANERR_NOT_PERMITTED, scerr );
        }
    }
    
    return SCANERR_NONE;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 10-Jan-2013
 */

scan_err_t adf_page_gapper_client_pause( void )
{
    /* TODO add semaphore */
    /* TODO ignore if not doing ADF scan */
    client_pause = 1;
    dbg1("[S_CANCEL] step3. pause!! adf_page_gapper_client_pause!!\n");

    return SCANERR_NONE;
}

scan_err_t adf_page_gapper_client_resume( void )
{
    /* TODO add semaphore */
    /* TODO ignore if not doing ADF scan */
    client_pause = 0;
    dbg1("[S_CANCEL] step3. resume!! adf_page_gapper_client_pause!!\n");

    return SCANERR_NONE;
}

