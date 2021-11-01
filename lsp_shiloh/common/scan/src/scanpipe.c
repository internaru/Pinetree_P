/**
 * \file scanpipe.c
 *
 * \brief Plumbing that connects PIC->PIE->HalfPack and all steps between.
 *
 * ============================================================================
 * Copyright (c) 2004-2011, Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* davep 23-Jul-2008 ; We cannot have a dependency on agMessage.h because we
 * can't give a BSDK customer that code. So we must not include agMessage in our
 * source. But many of the system headers include it. So we #define the header
 * file guard block in the Makefile to guarantee we won't get agMessage.h in our
 * source.  
 *
 * This #ifdef catches a build without the agMessage.h prevention mechanism.
 *
 * I'm only trapping on agMessage.h in scanpipe.c because if agMessage.h is in
 * one file, it's probably in many more as well.
 */
#ifndef __AGMESSAGE_H__
    #error cannot include agMessage.h in the scan code! 
#endif

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>  /* for snprintf */
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "scos.h"

/* need ceil() for xy scaler linear mode */
#include <math.h>

#include "list.h"           
#include "lassert.h"
#include "memAPI.h"
#include "regAddrs.h"
#include "cpu_api.h"

/* I'm testing PIC and PIE DMA so I need to see some of the internal parts for
 * sanity checks, etc.
 */
#define PICDMA_FRIEND
#define PIEDMA_FRIEND

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scanpipe.h"
#include "scanreset.h"
#include "scan.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "piccbi.h"
#include "piehw.h"
#include "pie.h"
#include "scanif.h"
#include "piehw.h"
#include "scanvars.h"
#include "pipenet.h"
#include "scanmech.h"
#include "scancmdq.h"
#include "strippr.h"
#include "icefilter.h"
#include "icefile.h"
#include "scantask.h"
#include "ostools.h"
#include "scanplat.h"
#include "pipecut.h"
#include "safeint.h"
#include "adfsensor.h"
#include "fwmargin.h"
#include "piecs.h"
#include "icetest.h"
#include "scanalyzer.h"
#include "adfpath.h"
#include "adfgapper.h"
#include "scanmem.h"
#include "scanlib.h"
#include "scands.h"
#include "cisx.h"
#include "scansen.h"
#include "scanimg.h"
#include "piemux.h"
#include "pipemap.h"
#include "piereset.h"
#include "picdma_descrip.h"
/* enables many, many verbose debugging messages (better to cherry pick the
 * messages you want, convert to dbg2 to debug the problem, then convert back
 * to pipe_dbg2() on checkin)
 */
//#define PIPE_MSG_DEBUG

/* davep 10-Nov-2005 ; added pipe_dbg2 to try to declutter the debug code a bit */
#ifdef PIPE_MSG_DEBUG
#define pipe_dbg2 dbg2
#else
#define pipe_dbg2(...)
#endif

//=========================================================================================
#define BG_REM_ADVANCED		1
#define BGR_REMOVE 			1	// add.lsh for background removal
#define BGR_REMOVE_FOR_SCAN	0			// background removal for scan (for debugging)

#define BG_REM_TOP_OFFSET	(50*dpi_factor_v)		// 300dpi 100 = 8.46mm
#define BG_REM_HEIGHT		(200*dpi_factor_v)	// height of detecting area for BGR level (300dpi lines)
#define BG_REM_LEFT_OFFSET	(200*dpi_factor_h)
#define BG_REM_RIGHT_OFFSET	(200*dpi_factor_h)
#if BG_REM_ADVANCED
	#define BG_REM_COMPENSATE	10			// compensate the differnece between pic and pie
#else
	#define BG_REM_COMPENSATE	4			// compensate the differnece between pic and pie
#endif
	#define BG_REM_LIMIT_B			50			// Background Working Limit (White) => PIC background level of SJ-1 chart
	//#define BG_REM_LIMIT_W			205	// Background Working Limit (White) => PIC background level of S-5 chart
static bool	bg_enable = false;			// 0 : disable, 1 : enable
static bool	bg_log_enable = false;		// 0 : disable, 1 : enable
static uint32_t dpi_factor_h;			// 300dpi : 1, 600dpi : 2
static uint32_t dpi_factor_v;			// 300dpi : 1, 600dpi : 2
static uint32_t histogram[256] = {0,};
static uint32_t peak_lvl;
static uint32_t peak_index;
static uint32_t current_rows = 0;	// 0 : top of the page
static bool	bgr_detecting = false;	// true : top of the page ~ end of detecting
static uint32_t p_bgr_lut_org[PIE_CSC_RGB_LUT_SIZE];
//=========================================================================================

/* davep 02-Nov-2006 ; if we're scaling, we scale up the number of buffers,
 * too. This is the ceiling on the number of buffers
 */
#define MAX_BUFFERS_IF_SCALING  32

/* how long to wait for scan messages from interrupts, other threads, etc,
 * while inside pipe_wait()  (measured in the quantums required by
 * scos_queue_receive())
 *
 * Using two different numbers based on the scan debug level because we use
 * this timeout to dump interesting debug info.
 *
 * SDBGLEVEL is #def'd in scandbg.h
 */
#if 1 /* add. fix the timeout to wait for scan messages */
#define PIPE_WAIT_SECONDS  1
#else
#define PIPE_WAIT_SECONDS  5
#endif

/* We'll end the scan after this many rows of seeing no new TOF from the ADF.
 * Scale the returned value to the specified vertical resolution.
 */
//#define PIPECUT_END_OF_INPUT_ROW_COUNT(hw_dpi_vert) ( (2*2600 * ((int)hw_dpi_vert)) / 300 ) //mod.kks Request by Mechanical TEAM. 398mm --> 440mm
#define PIPECUT_END_OF_INPUT_ROW_COUNT(hw_dpi_vert) ( (2*2300 * ((int)hw_dpi_vert)) / 300 )

/* If we're in a page and we haven't seen a BOF in this many rows, consider it a
 * paper jam (~15" at 300dpi). Scale the returned value to the specified vertical
 * resolution.
 */
#define PIPECUT_JAM_ROW_COUNT(hw_dpi_vert) ( (4800 * ((int)hw_dpi_vert)) / 300 )
//#define PIPECUT_JAM_ROW_COUNT(user_area_hinch, hw_dpi_vert) ((user_area_hinch+100)*((int) hw_dpi_vert) /100) //kks TBD

#define SUPPORTED_PAPER_MIN_SIZE(hw_dpi_vert) ((1250 * ((int)hw_dpi_vert)) / 300 )  // mod.kks

 // user_area_hinch + margine * dpi / 100 => 1/100"

/* davep 17-Jul-2009 ; stop after a certain distance because our motor driver
 * can't tolerate rollover (FIXME we should probably fix scanmot_dc so it
 * can handle rollover)
 * 
 * Notes (from BurtP):
 *  - 37500 is 125 inches at 300dpi, so about 11 letter pages (except
 *    this constant also include inter page gap, which is variable).
 *  - when changing this number make sure you stay WELL under
 *    (like half of) the ADF_SCAN_DISTANCE value in the mech code.
 */
#define DEFAULT_PIPECUT_MOVE_DISTANCE_ROWS 37500

/* must have this much memory free before we start scanning the next page on
 * the ADF  (can't pause while scanning from adf so if there is low memory, we
 * pause between pages, allow memory to recover)
 *
 * There is no need to change this compile-time number directly. Can change the
 * scands "required_memory_for_next_page" at run-time or in your scanmech
 * starup code instead. For example:
 *      scands_set_integer( "required_memory_for_next_page", 48*1024*1024 )
 */
//#define DEFAULT_REQUIRED_MEMORY_FOR_NEXT_PAGE  (48*1024*1024)
#define DEFAULT_REQUIRED_MEMORY_FOR_NEXT_PAGE  (72*1024*1024)	// mod.lsh to prevent EC221(PIC overflow, 600dpi BW photo Dual Scan)
//#define DEFAULT_REQUIRED_MEMORY_FOR_NEXT_PAGE  (12*1024*1024)

/* davep 11-Apr-2013 ; adding page_side to several functions. However, many
 * functions don't usually require a page side so '0' is passed. Adding this
 * #def to identify the parameter so we don't have a bunch of calls with magic
 * numbers.
 */
#define PAGE_SIDE_0 0
#define PAGE_SIDE_1 1

static uint8_t pic_channels[ PIC_WDMA_NUM_CHANNELS ];
static uint8_t fw_margin_channels[ PIC_WDMA_NUM_CHANNELS ];
static uint8_t pie_rchannels[ PIE_RDMA_NUM_CHANNELS ];
static uint8_t pie_wchannels[ PIE_WDMA_NUM_CHANNELS ];
static uint8_t piehp_wchannels[ PIEHP_WDMA_NUM_CHANNELS ];
static uint8_t piehp_rchannels[ PIEHP_RDMA_NUM_CHANNELS ];
static uint8_t num_pic_channels;
static uint8_t num_fw_margin_channels;
static uint8_t num_pie_rchannels;
static uint8_t num_pie_wchannels;
static uint8_t num_hp_rchannels;
static uint8_t num_hp_wchannels;

/* davep 07-Mar-2006 ; adding software filter */
static uint8_t filter_channels[ ICE_FILTER_NUM_CHANNELS ];
static uint8_t num_filter_channels;

static bool use_scanman = false; // results sent to ScanMan
static bool use_results; // pipe results get saved in "pipe_results"
static bool use_pipenet; // reading "scan data" from network into PIE/HalfPack

/* davep 16-Mar-2006 ; adding icefile feature; read scan data in from USB or
 * TCP or whatever
 */
static bool use_icefile; 

/* davep 07-Mar-2006 ; adding software filter */
static int filter_position=SV_SW_FILTER_DISABLED; 

/* davep 01-Jun-2009 ; adding pipe cutter */
static bool use_pipecut;

/* davep 02-Apr-2013 ; adding nsensor support (sensor on both sides of paper) */
static bool use_nsensor;

/* davep 22-May-2013 ; adding piemux, a way to multiplex dual scan (sensor on
 * both sides of the paper) through our single PIE
 */
static bool use_piemux;

static int pipetap = PIPETAP_NONE; 

static ATLISTENTRY pipe_results;

/** Convenience variable holding total bytes per row of resulting data from the
 * PIC/PIE/HalfPack chain. Used to print the size of the final data after the scan is
 * complete.
 */
static int g_final_bytes_per_row;

/** Convenience variable holding total rows of resulting data from the
 * PIC/PIE/HalfPack chain. Used to print the size of the final data after the scan is
 * complete.
 */
static int g_final_total_rows;

extern bool ADF_mech_running;

/* davep 21-Nov-2005 ; we've had this structure bouncing around for a while and
 * I keep starting to use bits of it and stop using other bits. Be careful
 * about using certain fields; they may not be initialized correctly. This
 * structure started out to count the number of buffers in the system to check
 * for leaks and sort of grew from there.
 */
struct pipe_stats {
    /* davep 22-Jan-2009 ; Add scan_cmdq_rows. Because of ICE Lite have to split
     * the linkage between PIC DMA rows and the Scan block's SCMD (Scan Command)
     * queue.  
     *
     * In color mode, the scan block will send three start sensor pulses for
     * every row in the SCMD. So we will get three rows of data into memory for
     * every "row" we capture. 
     *
     * In an ASIC with color channels, we would have three DMA buffers for
     * color: one for every output row (R,G,B) of the SCMD row (start pulse). We
     * told Scan CMDQ the number of rows in a single channel's buffer. 
     *
     * But in ICE Lite we don't have color channels. All the DMA goes out one
     * channel. So we have to triple the number of rows in the DMA buffer. We
     * can't tell Scan CMDQ the number of rows in the channel anymore. The
     * coupling is broken.
     *
     * Instead, we preserve a number (scan_cmdq_rows) which is the number of
     * rows to capture. Now, cmdq_rows != PIC WDMA buffer rows.
     */
    int scan_cmdq_rows;

    /* for pipe_wait debug */
    int scan_expected_rows;

    int picw_rows_per_buffer;
    int picw_num_buffers;
    int picw_tot_rows;
    int picw_bytes_per_row; 

    int fw_wmargin_rows_per_buffer;

    int pier_num_buffers;
    int pier_tot_rows;
    
    int piew_rows_per_buffer;
    int piew_num_buffers;
    int piew_tot_rows;

    int hpr_tot_rows;

    int hpw_rows_per_buffer;
    int hpw_num_buffers;
    int hpw_tot_rows;

    /* davep 26-Apr-2013 ; add pipe cutter's requirements */
    int pipe_cutter_num_rows_cached;
    
    /* add.lsh.to support adf jam */
    int page_starts;
    int page_ends;
    bool pp_status;
};

static struct pipe_stats pstats;

/* davep 07-Jun-2009 ; adding more sane ADF support by moving adf responsiblity
 * into scanpipe. But we have to know when to send the new SMSG_PAGE_START and
 * SMSG_PAGE_END messages.
 */
/* davep 03-Feb-2011 ; changing to multi-channel ; this is a bit ugly */
struct pipecut_page_state {
    int num_channels_in_page;
    int num_channels;
    bool is_channel_in_page[MAX_PIPE_CUTTER_CHANNELS];
};

static struct pipecut_page_state pipecut_page_state_list[2];

/* davep 22-May-2013 ; Explicit is better than implicit. Track exactly where
 * pipecutter lives so we can adjust the buffer counts of the previous WDMA
 * system.
 */
static int pipe_cutter_location;
#define PIPE_CUTTER_LOCATION_NONE 0
#define PIPE_CUTTER_LOCATION_AFTER_PIC 1
#define PIPE_CUTTER_LOCATION_AFTER_PIE 2
#define PIPE_CUTTER_LOCATION_AFTER_PIEHP 3

/* davep 22-Apr-2013 ; adding pipecut 2.0 */
static void return_icebuf_to_pic_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf );
static void return_icebuf_to_pie_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf );
static void return_icebuf_to_piehp_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf );

#if 1	//add.lsh.for pipe debug
    int dbg_cnt_pic_interrupt = 0;
    int dbg_cnt_pic_wdma_interrupt = 0;
    int dbg_cnt_channel_isr = 0;
    int dbg_cnt_pie_interrupt = 0;
    int dbg_cnt_pie_dma_interrupt = 0;
    int dbg_cnt_piems_rdma_isr = 0;
    int dbg_cnt_piems_wdma_isr = 0;
    int dbg_cnt_pie_rdma_isr = 0;
    int dbg_cnt_pie_wdma_isr = 0;
    int dbg_cnt_ice_buffer_callback = 0;
    int dbg_cnt_piems_dma_isr = 0;
    int dbg_cnt_ice_dma_isr = 0; 
    
    int dbg_cnt_SMSG_PIC_WDMA_DATA = 0;
	int dbg_cnt_SMSG_PIE_RDMA_DATA = 0;
	int dbg_cnt_SMSG_PIE_WDMA_DATA = 0;
	int dbg_cnt_SMSG_PIE_RDMA_INT = 0;
	int dbg_cnt_SMSG_PIE_WDMA_INT = 0;   
#endif


int pipe_set_pipetap( int pipetap_location )
{
    int old_pipetap;

    /* XXX note no semaphore/mutex protection used! (scary) */

    old_pipetap = pipetap;
    pipetap = pipetap_location;

    return old_pipetap;
}

ATLISTENTRY *pipe_get_pipe_results( void )
{
    /* davep 08-Sep-2011 ; originally pipe_results was accessed by cal through
     * an extern. The ugly finally overwhelmed me and I converted to an
     * accessor.
     */
    return &pipe_results;
}

/**
 * \brief  Send the image metadata so we can decode the buf on the receiving
 *         host.
 *
 *      I'm tired of trying to decode these mystery buffers of goo on my disk.
 *      Let's send some HTTP header style metadata.
 *
 * \author David Poole
 * \date 06-Apr-2012
 */

static int send_data( uint8_t channels[], int num_channels, 
                      uint8_t *buf, int buflen )
{
    int retcode;
    int i;

    retcode = 0;

    for( i=0 ; i<num_channels ; i++ ) {
        retcode = scan_debug_net_send( channels[i], buf, buflen );
        if( retcode != 0 ) {
            return retcode;
        }
    }

    return retcode;
}

static scan_err_t net_send_metadata( uint8_t channels[], int num_channels,
                                     scan_data_type dtype, 
                                     uint32_t pixels_per_row_padded, uint32_t pixels_per_row,
                                     uint32_t total_rows, uint32_t bits_per_pixel )
{
    int i, retcode;
    char str[64];
    scan_data_type planar_type_list[] = {
        SCAN_DATA_TYPE_RED,
        SCAN_DATA_TYPE_GREEN,
        SCAN_DATA_TYPE_BLUE,
        SCAN_DATA_TYPE_RED,
        SCAN_DATA_TYPE_GREEN,
        SCAN_DATA_TYPE_BLUE };

    /* if network debug not active, bail before we spend a lot of time
     * sprintf'ing strings
     */
    if( scan_debug_get_ip()==-1 ) {
        return SCANERR_NOT_PERMITTED;
    }

    memset( str, 0, sizeof(str) );
    
#define SENDSTR\
    do { \
        retcode = send_data( channels, num_channels, (uint8_t *)str, strlen(str) );\
        if( retcode != 0 ) {\
            return SCANERR_MSG_FAIL;\
        }\
    } while(0);

    /* send a line to to identify header */
    snprintf( str, 63, "pipetap/2.0\r\n" );
    SENDSTR;

    snprintf( str, 63, "pixels_per_row: %" PRId32 "\r\n", pixels_per_row );
    SENDSTR;

    snprintf( str, 63, "pixels_per_row_padded: %" PRId32 "\r\n", pixels_per_row_padded );
    SENDSTR;

    snprintf( str, 63, "total_rows: %" PRId32 "\r\n", total_rows );
    SENDSTR;

    snprintf( str, 63, "bits_per_pixel: %" PRId32 "\r\n", bits_per_pixel );
    SENDSTR;

    if( dtype==SCAN_DATA_TYPE_PLANE ) {
        /* planar is a pain: it's not a type in itself but rather an indication
         * that we will be getting three planes of data: red,green,blue. Need
         * to send the proper data type to the proper channel (Note! This is a
         * big fat assumption that the red channel is the first in the
         * channels[], the green is the 2nd, the blue is the 3rd. 
         */
        /* davep 26-Mar-2013 ; adding support for 6 channels (dual scan) */
        XASSERT( num_channels==3||num_channels==6, num_channels );

        for( i=0 ; i<num_channels ; i++ ) {
            snprintf( str, 63, "data_type: %s\r\n",     
                        scanvar_get_scan_data_type_name(planar_type_list[i]) );
            retcode = scan_debug_net_send( channels[i], (uint8_t *)str, strlen(str) );
            if( retcode != 0 ) {
                return SCANERR_MSG_FAIL;
            }
        }
    }
    else {
        snprintf( str, 63, "data_type: %s\r\n", 
                scanvar_get_scan_data_type_name( dtype ) );
        SENDSTR;
    }

    /* send a blank line to terminate the header */
    retcode = send_data( channels, num_channels, (uint8_t *)"\r\n", 2 );

    return SCANERR_NONE;

#undef SENDSTR
}

/**
 * \brief  Calculate how many extra buffers we will need for pipecutter.
 *
 * Increase our buffer counts to support pipecutter because pipecutter will
 * hold buffers while waiting for ADF events. This value will be added to the
 * last link of the DMA chain (i.e., pic or pie or piehp) before pipecutter.
 *
 * For example, if using PIE, we will need extra PIE WDMA buffers because the
 * pipecutter will cache several. Otherwise, we'll pause on low memory and not
 * resume.
 *
 * If we're not using pipecutter, this should wind up being zero.
 *
 * \author David Poole
 * \date 22-May-2013
 *
 */

static uint32_t calc_extra_buffers_for_pipecut( uint32_t rows_per_buffer )
{
    uint32_t pipe_cutter_buffers;

    if( !pstats.pipe_cutter_num_rows_cached ) {
        return 0;
    }

    pipe_cutter_buffers = safeint_divide_with_ceil( pstats.pipe_cutter_num_rows_cached, 
                                                    rows_per_buffer );
    dbg2( "%s pipecutter requires rows=%d so increase buffers of rows=%d by buffers=%d\n", 
            __FUNCTION__, 
            pstats.pipe_cutter_num_rows_cached, rows_per_buffer, 
            pipe_cutter_buffers );


    return pipe_cutter_buffers;
}

/**
 * \brief  tune the memory requirements per the DPI
 *
 * Bigger scans are slower so need more memory per buffer but fewer buffers.
 * Smaller DPI scans need less memory but a lot more buffers because they blow
 * through them so quickly.
 *
 * This function is just a starting point. There's a lot more numerical tuning
 * we can do here. 
 *
 * \author David Poole
 * \date 21-Nov-2005
 *
 * davep 21-Jul-2008 ; add 'cmode' parameter due to ICE Lite's whacky lack of
 *                     color DMA
 */

static void setup_buffer_counts( struct scanvars *sv, struct pipe_stats *ps )
{
    scan_err_t scerr;
    int x_numer, x_denom, y_numer, y_denom;
    int x_scale, y_scale;
    uint32_t pic_pie_buffers, pad_buffers, max_buffers;

    /* davep 02-Apr-2012 ; changing from compile time memory config to scands. 
     *
     * Be very careful changing these numbers!
     *
     * For tuning pause/resume problems, pipe_pad_buffers is best option.
     */
    scerr = scands_get_integer_with_default( "pipe_pic_pie_buffers", &pic_pie_buffers, 10 );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = scands_get_integer_with_default( "pipe_pad_buffers", &pad_buffers, 10);
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = scands_get_integer_with_default( "pipe_max_buffers", &max_buffers, 
                        pic_pie_buffers+pad_buffers );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* populate the num_buffers fields of a pipe_stats structure */

    /* TODO tune these numbers for the various DPIs */

    switch( sv->dpi ) {
        case 1200 :
            /* davep 30-Oct-2006 ; make everyone the same but leave in the
             * original code in case I have to tune later. If not, delete this.
             */
#if 0
            ps->picw_num_buffers = 12;
            ps->pier_num_buffers = 8; 
            ps->piew_num_buffers = 8;
            ps->hpr_num_buffers = 16;
            ps->hpw_num_buffers = 16;
#else
            ps->picw_num_buffers = max_buffers;
            ps->pier_num_buffers = max_buffers; 
            ps->piew_num_buffers = max_buffers;
            ps->hpw_num_buffers = max_buffers;
#endif
            break;

        case 600 : 
            ps->picw_num_buffers = max_buffers;
            ps->pier_num_buffers = max_buffers;
            ps->piew_num_buffers = max_buffers;
            ps->hpw_num_buffers = max_buffers;
            break;

        case 300 :
            ps->picw_num_buffers = max_buffers;
            ps->pier_num_buffers = max_buffers;
            ps->piew_num_buffers = max_buffers;
            ps->hpw_num_buffers = max_buffers;
            break;

        default :
            ps->picw_num_buffers = max_buffers;
            ps->pier_num_buffers = max_buffers;
            ps->piew_num_buffers = max_buffers;
            ps->hpw_num_buffers = max_buffers;
            break;
    }

    /* davep 26-Apr-2013 ; add pipecutter's memory requirements into the
     * calculation (PIC/PIE WDMA will allocate these extra if necessary)
     */
    if( use_pipecut ) {
        ps->pipe_cutter_num_rows_cached = MAX_PIPE_CUTTER_RUNNING_ROWS;
    }

    /* davep 02-Nov-2006 ; I shrank the buffer counts so now have to tune
     * upwards to take scaler into account
     */
    scanvar_get_pie_scale( sv, &x_numer, &x_denom, &y_numer, &y_denom );

    /* davep 08-Jun-2012 ; change to integer math; previous code should have
     * restricted numer and denom can be at most 15-bits so the scaled math in
     * safeint should be fine. Only need a rough estimate here.
     */
    x_scale = safeint_divide_with_round( x_numer, x_denom );
    y_scale = safeint_divide_with_round( y_numer, y_denom );
    if( x_scale > 3 || y_scale > 3 ) {
        ps->picw_num_buffers *= MAX( x_scale, y_scale );
        ps->piew_num_buffers *= MAX( x_scale, y_scale );

        ps->picw_num_buffers = MIN( MAX_BUFFERS_IF_SCALING, ps->picw_num_buffers );
        ps->piew_num_buffers = MIN( MAX_BUFFERS_IF_SCALING, ps->piew_num_buffers );

        dbg2( "bump buffers to picw=%d piew=%d\n", ps->picw_num_buffers,
                    ps->piew_num_buffers );
    }
}

/**
 * \brief Start the ICE software filter. 
 *
 *  Sets up a few globals we'll use later in pipe_wait() then calls the
 *  ice_filter_open() function. 
 *
 * \param[in] channels
 * List of channel numbers to use. Should be either PIC Write DMA or PIE Write
 * DMA channel numbers.
 *
 * \param[in] num_channels
 * Number of entries in channels array.
 *
 * \param[in] total_rows
 * Total number of rows of data each channel should see. After this many rows
 * passes through a channel, the channel is done.
 *
 * \param[in] bytes_per_row
 * Number of bytes per row. Might not be exactly the number of pixels per row
 * (e.g., PIC could use 16-bit pixels, PIE xRGB uses 32-bit pixels).
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static scan_err_t
filter_start( uint8_t channels[], uint8_t num_channels, 
              int total_rows, int bytes_per_row  )
{
    int i;

    XASSERT( num_channels <= ICE_FILTER_NUM_CHANNELS, num_channels );
    XASSERT( filter_position!=SV_SW_FILTER_DISABLED, filter_position );

    num_filter_channels = num_channels;

    /* The ICE filter takes its channels from either PIC or PIE so we need to
     * start clean every time.
     */
    memset( filter_channels, 0, sizeof(filter_channels) );

    for( i=0 ; i<num_channels ; i++ ) {
        filter_channels[i] = channels[i];
    }

    ice_filter_open( channels, num_channels, total_rows, bytes_per_row );

    /* success! */
    return SCANERR_NONE;
}


/**
 * \brief  Add a curve to PIC BDR
 *
 * PIC BDR is used with ICE Lite for image/copy quality tuning. BDR is the only
 * place we can put a curve to the 
 *
 * In full ICE we don't use BDR but use PIE's TCNS instead.
 *
 * Note that normally PIC belongs entirely to calibration with the exception of
 * WDMA.  But we're using PIC BDR as an image tuning mechanism and thus it
 * properly belongs in scanpipe and scanvars.
 *
 * \author David Poole
 * \date 24-Dec-2008
 *
 */

static void pic_bdr_setup( void )
{
    const struct scanvars *sv;

    sv = scanvar_get();
    if( !sv->use_pic_bdr || !sv->use_pic_bdr_lut ) {
        return;
    }

    ASSERT( sv->pic_bdr_lut.data != NULL );
    XASSERT( sv->pic_bdr_lut.num_entries==PIC_BDR_LUT_SIZE,
             sv->pic_bdr_lut.num_entries );

    pic_bdr_enable_fw_access( true );
    pic_bdr_set_lut( sv->pic_bdr_lut.data );
    pic_bdr_enable_fw_access( false );

    /* take bdr out of bypass */
    pic_bdr_set_bypass( false );
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 18-Mar-2013
 */

static void pixels_per_row_sanity_check( const struct scanvars *sv )
{
    scan_err_t scerr;
    int i;
    uint32_t scanx;
    uint32_t pic_pixels_per_row, pic_bytes_per_row;
    uint32_t pic_left_margin, pic_right_margin;
    uint32_t cisx_ppr;
    struct cisx_sensor_conf cisxsensor;
    uint32_t pixel_sum;
    uint32_t active_pixel_sum;

    scanimg_get_scanx( &scanx );
    
    /* davep 24-Oct-2008 ; check if PIC margins are in bypass */
    if( pic_marg_get_bypass() ) {
        /* pic will send to memory whatever scan sends */
        pic_pixels_per_row = scanx;

        /* margins not used  */
        pic_left_margin = pic_right_margin = 0;
    }
    else {
        pic_left_margin = pic_marg_get_left();
        pic_right_margin = pic_marg_get_right();
        /* +1 because, well, just because it's necessary, ok? PIC margins are
         * confusing 
         */
        pic_pixels_per_row = pic_right_margin - pic_left_margin + 1;

        dbg2( "%s pic left=%d right=%d ppr=%d\n", __FUNCTION__,
                    pic_left_margin, pic_right_margin, pic_pixels_per_row );

        /* davep 04-Apr-2009 ; make sure our margins don't exceed the actual
         * pixels we'll be receiving otherwise rows will wrap onto each other.
         * The right margin is the last pixel the margin block will pass.
         *
         * scanx is a count.  [0,scanx)
         * pic_right_margin is an index.
         * 
         */
        XASSERT( pic_right_margin < scanx, pic_right_margin );
    }

    if( pic_wdma_get_bitpack_mode() == PIC_BITPACK_8BIT ) {
        dbg2( "%s pic wdma bpp=8\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row;
    }
    else {
        dbg2( "%s pic wdma bpp=16\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row*2;
    }

    dbg2( "%s scanx=%ld pic ppr=%ld bpr=%ld\n", __FUNCTION__, 
            scanx, pic_pixels_per_row, pic_bytes_per_row );

    /* davep 25-Feb-2013 ; if we're in dicard mode, don't fail on PIC
     * DMA misalignment.
     */
    if( *PIC_WDMA_DWOCR & PIC_WDMA_DWOCR_DISABLEMODE(PIC_WDMA_DWOCR_DM_DISCARD)) {
        dbg2( "%s PIC WDMA in discard\n", __FUNCTION__ );
    }
    else { 
        XASSERT( pic_bytes_per_row % ICE_DMA_BYTE_ALIGN == 0, pic_bytes_per_row );
        XASSERT( scanx >= pic_right_margin, scanx );
    }

    /* check CISX if we have CISX */
    scerr = scansen_get_cisx_conf( sv->hw_dpi_horiz, &cisxsensor );
    if( scerr == SCANERR_NONE && !(scan_get_config() & SCAN_CONFIG_NO_CISX) ) {
        /* this is a CISX sensor so test CISX configuration */

        cisx_get_pixels_per_row( &cisx_ppr );

        /* stupid human check */
        XASSERT( cisx_ppr>0 && cisx_ppr < 65536, cisx_ppr );

        pixel_sum = 0;
        active_pixel_sum = 0;
        for( i=0 ; i<cisxsensor.num_segments ; i++ ) {
            pixel_sum += cisxsensor.segments[i].num_dummy + cisxsensor.segments[i].num_data;
            active_pixel_sum += cisxsensor.segments[i].num_data;
        }

        dbg2( "%s cisx_ppr=%d pixel_sum=%d active_pixel_sum=%d\n", __FUNCTION__, 
                cisx_ppr, pixel_sum, active_pixel_sum );

        XASSERT( pixel_sum>0 && pixel_sum < 65536, pixel_sum );
        XASSERT( active_pixel_sum>0 && active_pixel_sum < 65536, active_pixel_sum );

        XASSERT( pixel_sum==cisx_ppr, pixel_sum );
        XASSERT( active_pixel_sum >= pic_right_margin, active_pixel_sum );
    }

}

/**
 * \brief  Configure the PIC margins
 *
 * Moved margins from cal's responsibility to scanpipe's responsibility.
 *
 * \author David Poole
 * \date 01-Mar-2013
 */

static void pic_margin_setup( uint32_t margin_pixels_per_row_in, 
                              uint32_t margin_pixels_per_row_out )
{
    const struct scanvars *sv;
    const struct scan_area *pixel_area;
    uint32_t pic_pixels_per_row, pic_bytes_per_row;

    sv = scanvar_get();

    dbg2( "%s svid=%d\n", __FUNCTION__, sv->id );

    /* we don't currently use bulb monitor */
    pic_bm_set_bypass( true );

    /* setup phony bulb monitor to allow margins to work (this is copy/paste
     * code from way/way back)
     */
    pic_bm_set_color_counter(1);
//    pic_bm_set_lines_to_avg(1);
//    pic_bm_set_window_size(16);
//    pic_bm_set_startpix(0);

    /* use a shortcut */
    pixel_area = &sv->scan_area_pixels;

    /* set margins to bypass before configuring */
    pic_marg_set_bypass( true );

    /* now set the margins */
    pic_marg_set_left( pixel_area->x );
    pic_marg_set_right( pixel_area->x + pixel_area->width -1 );
    pic_marg_set_bypass( false );

    dbg2( "%s left=%d right=%d\n", __FUNCTION__, pic_marg_get_left(),
            pic_marg_get_right() );

    /* Sanity check the margins. If the margins go wrong, everything falls apart
     * further down the pipe.
     *
     * +1 because, well, just because it's necessary, ok? PIC margins are
     * confusing 
     */
    pic_pixels_per_row = pic_marg_get_right() - pic_marg_get_left() + 1;

    if( sv->pic_bpp==8 ) {
//        dbg2( "%s pic wdma bpp=8\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row;

        pic_wdma_set_bitpack_mode( PIC_BITPACK_8BIT );
    }
    else {
//        dbg2( "%s pic wdma bpp=16\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row*2;

        pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
    }

//    dbg2( "%s pic ppr=%ld bpr=%ld\n", __FUNCTION__, 
//                pic_pixels_per_row, pic_bytes_per_row );

    XASSERT( pic_bytes_per_row % ICE_DMA_BYTE_ALIGN == 0, pic_bytes_per_row );

    /* davep 18-Mar-2013 ; check our pixels per row from top of pipe to bottom
     * of pipe
     */
    pixels_per_row_sanity_check(sv);
}

static scan_err_t
pic_start( int total_rows_per_channel,
           int pixels_per_row_in,
           int rows_per_buffer,
           int pic_buffer_cnt, 
           struct pic_pixels_per_row *pic_ppr )
{
    scan_err_t scerr;
    int retcode;
    int pic_bytes_per_row;
    int pic_bpp;  
    int i, channel_idx, buffers_accepted;
    int rdma_rows_per_channel;
    int scan_cmdq_rows;
    const struct scanvars *sv;
    uint32_t hs_scale;
    uint32_t flag;
    int min_pic_rows;
    scan_data_type dtype;
    SENSOR_CONF sc;
    struct scan_sensor_chipgap sensor_gaps;
    uint32_t sensor_bitmask;

    /* davep 18-Oct-2005 ; if pretending scan data came from network, don't try
     * start since we're not starting the scanner so we'll never get data
     * anyway
     */
    if( use_pipenet ) {
        return SCANERR_NONE;
    }

    dbg2( "%s trp=%d ppr=%d rpb=%d pbc=%d\n", __FUNCTION__, 
                total_rows_per_channel, pixels_per_row_in,
                rows_per_buffer, pic_buffer_cnt );

    XASSERT( pic_buffer_cnt>0, pic_buffer_cnt );

    sv = scanvar_get();

    /* davep 13-Jul-2011 ; changing scansen_get_sensor_conf to need cmode,dpi
     * so can have resolution select in register settings
     */
    /* check if we need to invert the incoming data */
    // for the future, we need to call scansen_get_sensor_conf for each sensor, and set up
    // each of them - for now, just grab the configuration for the 1st sensor
    scansen_get_sensor_conf(0, sv->cmode, sv->hw_dpi_horiz, &sc );
    if( !sc.positive_going_video ) {
        pic_adcnorm_set_invert( true );
    }

    /* davep 13-Jun-2011 ; if our buffers are too small, we start seeing pic
     * overflows (firmware can't keep up with hardware). We also hit problems
     * with high/low water.
     *
     * Note: PIC rows per buffer MUST be a factor of PIE strip size.
     */
    if( rows_per_buffer < 6 ) {
       rows_per_buffer *= 4;
       //rows_per_buffer *= 2; //KKS Test
    }

    /* davep 22-Mar-2013 ; tinkering with LARGE buffers for fast platforms */
    /* davep 09-Apr-2013 ; FIXME quick&dirty hack -- use an #ifdef */
#ifdef HAVE_NSENSOR_SUPPORT
    rows_per_buffer *= 4;
    //rows_per_buffer *= 2; //KKS test
#endif

    if( pic_ppr->use_hscale ) {
        pic_hs_set_bypass( true );
        hs_scale = pic_hs_calc_factor( pic_ppr->hscale_ppr_in,
                                        pic_ppr->hscale_ppr_out );
        pic_hs_set_hscale_factor( hs_scale );
        pic_hs_set_bypass( false );
    }
    else {
        pic_hs_set_bypass( true );
    }

    /* davep 28-Feb-2013 ; adding hardware chipgap correction */
    if( pic_ppr->use_chipgap ) {
        scerr = pic_chipgap_get_sensor_chipgap( sv->hw_dpi_horiz, &sensor_gaps );
        XASSERT( scerr==SCANERR_NONE, scerr );
        pic_chipgap_set_bypass(false);
        pic_chipgap_set_location( sensor_gaps.first_gap_offset,
                                  sensor_gaps.gap_to_gap_width );
        pic_chipgap_set_gaps( sensor_gaps.chipgap_list, sensor_gaps.num_chipgap_list );
        pic_chipgap_dump();
    }

    /* davep 01-Mar-2013 ; move PIC margins here from cal */
    pic_margin_setup(pic_ppr->lrmargins0_ppr_in,pic_ppr->lrmargins0_ppr_out);

    /* davep 17-Feb-2012 ; adding line reverse */
    if( sv->sensor_flip_horizontal ) {
        pic_wdma_enable_line_reverse(true);
    }
    /* add a backdoor way to force line_rev on/off */
    scerr = scands_get_integer("pic_line_rev",&flag);
    if( scerr==SCANERR_NONE ) {
        pic_wdma_enable_line_reverse( flag?true:false );
    }

    /* davep 29-May-2009 ; tune our memory usage down if we're scanning a tiny
     * tiny area
     */
    if( total_rows_per_channel < pic_buffer_cnt*rows_per_buffer ) {
        /* funny integer math so we round up */
        pic_buffer_cnt = (total_rows_per_channel+rows_per_buffer-1) / rows_per_buffer;

        dbg2( "%s dropping buffer count to pbc=%d\n", __FUNCTION__, pic_buffer_cnt );
    }

    /* davep 10-Oct-2007 ; (Bug 7116); make sure we're above the high water mark
     * or we won't even get off the ground
     * XXX this is experimental ; need to remove the hardwired numbers and
     * instead query the motor layer to find the real high water mark
     */
    min_pic_rows = scan_get_scan_mech()->pending_low_water;
//    const int min_pic_rows = 90;

    if( pic_buffer_cnt > 1 && rows_per_buffer * pic_buffer_cnt < min_pic_rows ) {
        /* +1 to round up one buffer */
        pic_buffer_cnt = (min_pic_rows / rows_per_buffer) + 1;
        dbg2( "%s change buffer count to pbc=%d for high water\n", 
                    __FUNCTION__, pic_buffer_cnt );

        /* check my math */
        XASSERT( (rows_per_buffer*pic_buffer_cnt)>=min_pic_rows, pic_buffer_cnt );
    }

    /* davep 24-Dec-2008 ; add a curve to BDR */
    pic_bdr_setup();

    /*Eric 7-Jun-11 Add PSESD setup*/
#ifdef HAVE_PIC_PSESD
    if(!(scan_get_config()& SCAN_CONFIG_NO_ESD))
    {
        pic_esd_setup(pixels_per_row_in);
    }

    if(!(scan_get_config()& SCAN_CONFIG_NO_PSENSE))
    {
        if(scanvar_get_cmode() == SCAN_CMODE_COLOR)
        {
            pic_ps_setup(true, true, 0x07, sv->hw_dpi_horiz);
        }
        else
        {
            pic_ps_setup(true, false, 0x01, sv->hw_dpi_horiz);
        }
        /*
         * To start searching of TOF
         *
         */
        pic_ps_go(true);
    }
#endif

    /* davep 07-Oct-2005 ; start playing with BDR so we have 8-bit pixels out
     * of PIC 
     */
    pic_bpp = scanvar_get_pic_bpp();

    if( pic_bpp==8 ) {
        pic_bytes_per_row = pic_ppr->pic_pixels_per_row_out;
    }
    else {
        /* Amount of data we're pushing through in this test.
         * 16 bits per pixels gives us the "* 2"
         */
        /* davep 05-Oct-2005 ; XXX - shouldn't be hardcoding pixel size! */
        pic_bytes_per_row = pic_ppr->pic_pixels_per_row_out * 2;
    }

    /* davep 06-Jun-2008 ; support for ICE Lite (no color DMA channels) */
    rdma_rows_per_channel = total_rows_per_channel;

    /* davep 22-Jan-2009 ; another ice lite fix (add more details)  */
    scan_cmdq_rows = rows_per_buffer;

    /* davep 28-Jan-2013 ; turn on dual scan */
    scanimg_get_sensor_bitmask(&sensor_bitmask);
    if (!SCANIMG_SINGLE_SENSOR(sensor_bitmask)) {
        scif_cbi_enable_dual_channel(true);
        scerr = pic_enable_dual_channel(true);
        XASSERT( scerr==SCANERR_NONE, scerr );

    }

    /* how we want the channels set up; just using 3 channels (non-staggered
     * RGB) using the default routing.
     */
    if (scanvar_get_cmode() == SCAN_CMODE_COLOR) {
        /* davep 18-Mar-2013 ; duplex color */
        if (!SCANIMG_SINGLE_SENSOR(sensor_bitmask)) {
            num_pic_channels = 6;
            pic_channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
            pic_channels[1] = PIC_DMA_CHANNEL_CODD_0;
            pic_channels[2] = PIC_DMA_CHANNEL_CEVEN_1;
            pic_channels[3] = PIC_DMA_CHANNEL_CODD_1;
            pic_channels[4] = PIC_DMA_CHANNEL_CEVEN_2;
            pic_channels[5] = PIC_DMA_CHANNEL_CODD_2;

            pic_wdma_enable_line_reverse_by_channel( PIC_DMA_CHANNEL_CODD_0, false );
            pic_wdma_enable_line_reverse_by_channel( PIC_DMA_CHANNEL_CODD_1, false );
            pic_wdma_enable_line_reverse_by_channel( PIC_DMA_CHANNEL_CODD_2, false );
        } 
        else { 
            num_pic_channels = 3;
            pic_channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
            pic_channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
            pic_channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
        }
        dtype = SCAN_DATA_TYPE_PLANE;
    } else {
        /* davep 28-Jan-2013 ; duplex mono */
        if (!SCANIMG_SINGLE_SENSOR(sensor_bitmask)) {
            num_pic_channels = 2;
            pic_channels[0] = PIC_DMA_CHANNEL_MEVEN;
            pic_channels[1] = PIC_DMA_CHANNEL_MODD;

            pic_wdma_enable_line_reverse_by_channel( PIC_DMA_CHANNEL_MODD, false );
        }
        else {
            num_pic_channels = 1;
            pic_channels[0] = PIC_DMA_CHANNEL_MEVEN;
        }
        dtype = SCAN_DATA_TYPE_MONO;
    }

    /* davep 16-Nov-2005 ; start the scan command queue at the same time */
    /* davep 02-Jun-2011 ; moving icefile to icetest block */
    if( !use_icefile ) {
        scerr = scan_cmdq_open( pic_channels, num_pic_channels,
                                  total_rows_per_channel );
        if( scerr != SCANERR_NONE ) {
            /* returns fail if we've run out of memory in the cmdq_open() function */
            return scerr;
        }
    }

    /* davep 22-May-2013 ; add the extra buffers for pipecutter (could be zero
     * if PIC not the last link before pipecutter)
     */
    if( pipe_cutter_location==PIPE_CUTTER_LOCATION_AFTER_PIC ) {
        pic_buffer_cnt += calc_extra_buffers_for_pipecut(rows_per_buffer);
    }
    
    retcode = pic_wdma_open( pic_channels, num_pic_channels, pic_buffer_cnt,
                   rdma_rows_per_channel, pic_bytes_per_row );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* save so we know how to allocate new buffers later */
    pstats.picw_rows_per_buffer = rows_per_buffer;
    pstats.scan_cmdq_rows = scan_cmdq_rows;

    /* davep 21-Oct-2009 ; adding picw_rows_per_buffer so we can capture PIC
     * position during ADF paper events
     */
    pstats.picw_bytes_per_row = pic_bytes_per_row;

    for( channel_idx=0 ; channel_idx<num_pic_channels ; channel_idx++ ) {

        /* feed in some buffers to hold the data we expect back */
        buffers_accepted = pic_wdma_setup_buffers( pic_channels[channel_idx],
                                pic_buffer_cnt, rows_per_buffer, pic_bytes_per_row );

        if( buffers_accepted <= 0 ) {
            /* setup returns fail if we've run out of memory for data structures */
            return SCANERR_OUT_OF_MEMORY;
        }

        /* davep 16-Nov-2005 ; add scan commands to match those buffers */
        /* davep 02-Jun-2011 ; moving icefile to icetest block */
        if( !use_icefile ) {
            for( i=0 ; i<buffers_accepted ; i++ ) { 
                scan_cmdq_channel_add_rows( pic_channels[channel_idx],
                                            scan_cmdq_rows );
            }
        }
    }

    /* davep 29-Mar-2013 ; run forever must be BEFORE launch */
    if( use_pipecut ) {
        /* davep 15-Jun-2009 ; adding run forever */
        scan_cmdq_run_forever();
        pic_wdma_run_forever();
    }

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_enable( pic_channels[i] );
    }

    /* feed initial buffers into the channels */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_launch( pic_channels[i] );
    }

    /* clear out the statistics so we can count interrupts, etc, for just this
     * PIC run
     */
    pic_clear_interrupt_stats();

    if( pipetap==PIPETAP_PIC && scan_debug_get_ip() != -1) {
        scan_debug_net_open( scan_debug_get_ip(), pic_channels, num_pic_channels );
        /* ignore error */

        /* davep 06-Apr-2012 ; adding pipetap net send metadata header */
        net_send_metadata( pic_channels, num_pic_channels, dtype,
                    pic_ppr->pic_pixels_per_row_out, /* pixels per row padded */
                    pic_ppr->pic_pixels_per_row_out, /* pixels per row (XXX sending the padded width) */
                    total_rows_per_channel,  /* total rows */
                    pic_bpp /* bits per pixel */
                );
    }

    /* dump starting state that might help looking problems */
//    pic_wdma_debug_log();

    /* success! */
    return SCANERR_NONE;
}

static void pic_done( void )
{
    int i, cnt;
    struct ice_dma_mm *picmm;
    const struct scanvars *sv;

    /* davep 18-Oct-2005 ; if pretending scan data came from network, don't try
     * to shut down since we never started
     */
    if( use_pipenet ) {
        return;
    }

    /* davep 24-Dec-2008 ;put BDR back into bypass if we're using it */
    sv = scanvar_get();
    if( sv->use_pic_bdr_lut ) {
        pic_bdr_set_bypass( true );
    }

    /* shut everything down */
    for( i=0 ; i<num_pic_channels ; i++ ) {
        pic_wdma_channel_disable( pic_channels[i] );
    }

    /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* Since we're going to be testing the PIC write DMA, we're going to need
     * to peek at its insides. Regular code will not and should not get
     * pointers to this structures.
     */
    picmm = pic_wdma_get_mm();

    /* count PIC write */
    cnt = 0;
    for( i=0 ; i<picmm->num_channels ; i++ ) {
        cnt += picmm->channels[i].num_readies;
    }

    /* we're done so clean up after ourselves */
    pic_wdma_close();
    num_pic_channels = 0;
}

/**
 * \brief  Start firmware margins
 *
 * \author David Poole
 * \date 08-Dec-2010
 *
 */

static scan_err_t 
fw_margin_start( int total_rows_per_channel,
                 int margin_pixels_per_row_in,
                 int margin_pixels_per_row_out,
                 int rows_per_buffer )
{
    int i, retcode;
    int pic_bpp;  
    int bytes_per_row_in, bytes_per_row_out;

    dbg2( "%s pin=%d pout=%d rpb=%d\n", __FUNCTION__, 
                margin_pixels_per_row_in, margin_pixels_per_row_out,
                rows_per_buffer );

    /* Use the PIC write channels for both margin read and write */
    num_fw_margin_channels = num_pic_channels;
    for( i=0 ; i<num_pic_channels ; i++ ) {
        fw_margin_channels[i] = pic_channels[i];
    }

    pic_bpp = scanvar_get_pic_bpp();
    if( pic_bpp==8 ) {
        bytes_per_row_in = margin_pixels_per_row_in;
        bytes_per_row_out = margin_pixels_per_row_out;
    }
    else {
        /* assuming 16bpp */
        bytes_per_row_in = margin_pixels_per_row_in * 2;
        bytes_per_row_out = margin_pixels_per_row_out * 2;
    }

    retcode = fw_rmargin_open( fw_margin_channels, num_fw_margin_channels,
                        pstats.picw_num_buffers, total_rows_per_channel,
                        bytes_per_row_in );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    retcode = fw_wmargin_open( fw_margin_channels, num_fw_margin_channels,
                        pstats.picw_num_buffers, total_rows_per_channel,
                        bytes_per_row_out );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* save so we know how to allocate new buffers later */
    pstats.fw_wmargin_rows_per_buffer = rows_per_buffer;

    /* add buffers to write margin */
    for( i=0 ; i<num_fw_margin_channels ; i++ ) {
        retcode = fw_margin_setup_write_buffers( fw_margin_channels[i],
                                pstats.picw_num_buffers, 
                                rows_per_buffer, bytes_per_row_out );
        if( retcode <= 0 ) {
            /* setup returns fail if we've run out of memory for data structures */
            return SCANERR_OUT_OF_MEMORY;
        }

    }

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_fw_margin_channels ; i++ ) {
        fw_rmargin_channel_enable( fw_margin_channels[i] );
        fw_wmargin_channel_enable( fw_margin_channels[i] );
    }

    /* feed initial buffers into the channels */
    for( i=0 ; i<num_fw_margin_channels ; i++ ) {
        fw_rmargin_channel_launch( fw_margin_channels[i] );
        fw_wmargin_channel_launch( fw_margin_channels[i] );
    }

    return SCANERR_NONE;
}

/**
 * \brief  Shut down firmware margins
 *
 * \author David Poole
 * \date 08-Dec-2010
 *
 */

static void fw_margin_done( void )
{
    int i;

    /* shut everything down; note firmware margins sponge the channel numbers
     * from PIC
     */
    for( i=0 ; i<num_fw_margin_channels ; i++ ) {
        fw_rmargin_channel_disable( fw_margin_channels[i] );
        fw_wmargin_channel_disable( fw_margin_channels[i] );
    }

    fw_rmargin_sanity();
    fw_wmargin_sanity();
    fw_rmargin_close();
    fw_wmargin_close();

    fw_margin_close();

    num_fw_margin_channels = 0;
}

static scan_err_t
pie_start( int total_rows_per_channel_in,
           struct pie_dma_row *row,
           struct pie_strip *strip,
           uint32_t pixel_size,
           int extra_pad,
           struct pie_scale *scale
           )
{
    scan_err_t scerr;
    int i, retcode;
    int read_rows_per_buffer, write_rows_per_buffer;
    int total_rows_per_channel_out;
    int pixels_out;
    bool use_scaler = true; /* davep 11-Sep-05 ; unless necessary, don't enable
                               the scaler! I'm hardwiring it enabled for now */
    int image_offset[PIE_RDMA_NUM_CHANNELS]; /* davep 12-Jan-2006 */
    const struct scanvars *sv;
    int rdma_rows_per_channel;
    pie_pixel_t piein;
//    uint32_t scale_rpb;

    dbg2( "%s\n", __FUNCTION__ );

    pie_dbg2_strip( strip ); 

    XASSERT( strip->rdma_in>0 && strip->rdma_in <= PIE_RDMA_MAX_ROWS, strip->rdma_in );
    XASSERT( strip->wdma_out>0 && strip->wdma_out <= PIE_WDMA_MAX_ROWS, strip->wdma_out );

    sv = scanvar_get();

    /* unless we're scaling, we'll get the same out as in */
    total_rows_per_channel_out = total_rows_per_channel_in;

    /* davep 31-Aug-2005 ; XXX adding in the scaler just for HalfPack data alignment for
     * now so we're hardwired to scale 1:1
     */
    pie_xy_scale( scale,  /* scale factors */
                  row->pixels_in,
                  strip->xy_in,
                  total_rows_per_channel_in,
                  extra_pad,
                  sv->xy_scaler_mode,
                  &pixels_out,
                  strip->wdma_out,
                  &total_rows_per_channel_out );

    /* davep 13-Sep-05 ; Make sure the numbers we calculated before match what
     * PIE is telling me now. I hate having to calculate some of these values
     * twice (e.g., pixels_out) but we have a chicken-and-egg problem with
     * needing the values so we can adjust them for the next stage in the DMA
     * pipe.
     */
    XASSERT( pixels_out==row->pixels_out, pixels_out );

    dbg2( "PIE rows in=%d rows out=%d\n", 
                total_rows_per_channel_in, total_rows_per_channel_out );

    /* set up the strips */
    read_rows_per_buffer = strip->rdma_in;
    write_rows_per_buffer = strip->wdma_out;

    /* davep 05-Jun-2008 ; adding support for ICE Lite (no color DMA channels)
     * but I have to be >VERY< careful about the row counts with PIE DMA (strips
     * vs buffers and all that confusion). So I don't muck with the rows per
     * channel number but use something different to pass in for DMA's
     * expected_rows counting.
     */
    rdma_rows_per_channel = total_rows_per_channel_in;

    /* set up read */
    if (sv->cmode == SCAN_CMODE_COLOR) {
        num_pie_rchannels = 3;
        pie_rchannels[0] = PIE_RDMA_RED_EVEN;
        pie_rchannels[1] = PIE_RDMA_GREEN_EVEN;
        pie_rchannels[2] = PIE_RDMA_BLUE_EVEN;
        /* davep 09-May-2011 ;  TODO add support for RGB, etc, input; for now
         * leave as planar 
         */
        piein = PIE_PIXEL_3_PLANES;
    } else {
        num_pie_rchannels = 1;
        pie_rchannels[0] = PIE_RDMA_MONO_EVEN;
        piein = PIE_PIXEL_MONO;
    }

    /* davep 24-May-2013 ; enable the soft bank switching if we're using piemux
     * (multiplexing multiple images through PIE)
     */
    scerr = piems_dma_open_bank(0);
    XASSERT( scerr==SCANERR_NONE, scerr );
    if( use_piemux ) {
        ASSERT(use_pipecut);

        scerr = piems_dma_open_bank(1);
        XASSERT( scerr==SCANERR_NONE, scerr );

        piemux_open();
    }

    retcode = piems_rdma_open( pie_rchannels, num_pie_rchannels, pstats.pier_num_buffers,
                     rdma_rows_per_channel, row->bytes_in, strip->rdma_in );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* davep 09-May-2011 ; convert to pie_rmda_setup_config() */
    /* set up the PIE read DMA controller. */
    pie_rdma_setup_config( pixel_size, sv->cmode, piein, strip->rdma_in );

    if( use_scaler ) {
        piems_rdma_init_scaler( strip->xy_in, scale->y_numerator,
                scale->y_denominator, scale->y_scale );
    }

    if( sv->use_colorshift ) {
        /* davep 25-May-2011 ; tell the multistrip driver we're running (eek!)
         * ColorShift. piems needs to adjust strip sizes to compensate for CS
         * eating two lines from each strip
         */
        piems_rdma_init_colorshift();
        piems_wdma_init_colorshift();
    }

    if( sv->use_mf ) {
        /* Tell the MultiStrip driver we're running MultiFilter. It is absolutely
         * critical this be called AFTER piems_rdma_init_scaler()!
         */
        piems_rdma_init_mf();

        /* davep 12-Jan-2006 ; set all channels' image offsets to -2 for
         * MultiFilter context 
         *
         * >> BIG FAT NOTE! <<
         *
         * Assumes whoever is setting up the scanning is capturing +4 extra
         * rows of data!
         */
        if (sv->cmode == SCAN_CMODE_COLOR) {
            image_offset[PIE_RDMA_RED_EVEN] = -2;
            image_offset[PIE_RDMA_GREEN_EVEN] = -2;
            image_offset[PIE_RDMA_BLUE_EVEN] = -2;
        } else {
            image_offset[PIE_RDMA_MONO_EVEN] = -2;
        }
        piems_rdma_set_image_offsets( image_offset );
    }

    /* davep 29-Mar-2013 ; run forever must be BEFORE launch */
    if( use_pipecut ) {
        piems_rdma_run_forever();
    }

    /* need to explicitly turn on our channels */
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_enable( pie_rchannels[i] );
    }

    /* feed initial buffers into the channels */
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        piems_rdma_channel_launch( pie_rchannels[i] );
    }

    /* set up write */
    switch( sv->pieout ) {
        case PIE_PIXEL_XRGB :
            XASSERT(sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_xRGB;
            break;

        case PIE_PIXEL_RGBX :
            XASSERT(sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RGBx;
            break;

        case PIE_PIXEL_3_PLANES :
            XASSERT(sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
            num_pie_wchannels = 3;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RED;
            pie_wchannels[1] = PIE_WDMA_CHANNEL_GREEN;
            pie_wchannels[2] = PIE_WDMA_CHANNEL_BLUE;
            break;

        case PIE_PIXEL_MONO :
            XASSERT(sv->cmode==SCAN_CMODE_MONO, sv->cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_MONO;
            break;

        case PIE_PIXEL_RGB :
            XASSERT(sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
            num_pie_wchannels = 1;
            pie_wchannels[0] = PIE_WDMA_CHANNEL_RGB;
            break;

        default :
            /* unknown/unsupported PIE output format */
            XASSERT( 0, sv->pieout );
    }

    /* davep 16-Jan-2012 ; tinkering with minimum buffer size */
    /* davep 09-May-2013 ; add pie_min_rows_per_buffer for color scan-to-host
     * heap fragmentation 
     */
//    if( sv->pie_min_rows_per_buffer && write_rows_per_buffer < sv->pie_min_rows_per_buffer ) {
//
//        scale_rpb = safeint_divide_with_ceil( sv->pie_min_rows_per_buffer, 
//                                            write_rows_per_buffer );
//        dbg2( "%s rows_per_buffer old=%d new=%d\n", __FUNCTION__, write_rows_per_buffer,
//                write_rows_per_buffer*scale_rpb );
//
//        write_rows_per_buffer *= scale_rpb;
//    }

    /* davep 30-Apr-2014 ; adding pie_max_rows_per_buffer so print doesn't hurt
     * itself. Iteratively find a rows_per_buffer that is still modulo strip
     * size but is between the user's min/max window. (Min and/or max can be
     * zero and this code will still work.)
     */
    while( write_rows_per_buffer < sv->pie_min_rows_per_buffer ||
           write_rows_per_buffer+strip->wdma_out <= sv->pie_max_rows_per_buffer ) {
        write_rows_per_buffer += strip->wdma_out;
    }
    dbg2( "%s min=%d max=%d write_rows_per_buffer=%d\n", __FUNCTION__, 
            sv->pie_min_rows_per_buffer, sv->pie_max_rows_per_buffer,
            write_rows_per_buffer );
    /* If these asserts fail, your min/max range is too restrictive and the
     * code can't find a value to match the requirement.
     */
    if( sv->pie_min_rows_per_buffer ) {
        XASSERT( write_rows_per_buffer >= sv->pie_min_rows_per_buffer,
                    write_rows_per_buffer );
    }
    if( sv->pie_max_rows_per_buffer ) {
        XASSERT( write_rows_per_buffer <= sv->pie_max_rows_per_buffer,
                    write_rows_per_buffer );
    }

    /* save so we know how to allocate new buffers later */
    pstats.piew_rows_per_buffer = write_rows_per_buffer;

    /* davep 22-May-2013 ; add the extra buffers for pipecutter (could be zero
     * if PIC not the last link before pipecutter)
     */
    if( pipe_cutter_location==PIPE_CUTTER_LOCATION_AFTER_PIE ) {
        pstats.piew_num_buffers += calc_extra_buffers_for_pipecut(write_rows_per_buffer);
    }

    /* davep 11-Sep-2005 ; Note I'm passing total_rows_per_channel_in in as the
     * total rows expected on the write channel. Yes, this is very weird but
     * required since, when the scaler is enabled, we cannot know how many rows
     * will be coming out of XYScale for certain scale factors.
     */
    /* davep 05-Jun-2008 ; using rdma_rows_per_channel now (changes for ICE
     * Lite) 
     */
    retcode = piems_wdma_open( pie_wchannels, 
                     num_pie_wchannels, 
                     pstats.piew_num_buffers,  /* num_empties */
                     //total_rows_per_channel_out,  NO, DON'T PASS THIS!
                     rdma_rows_per_channel,  // <-- weird but necessary
                     row->bytes_out,
                     strip->wdma_out );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    pie_wdma_setup_config( sv->cmode, sv->pieout, strip->wdma_out, pixels_out );

    /* turn on dynamic row counter if we're scaling in the Y direction */
    /* davep 06-Jul-2006 ; always turn on DRC for Linear */
    if( scale->y_numerator != scale->y_denominator ||
            sv->xy_scaler_mode==PIE_XY_SCALER_LINEAR  ) {
        pie_wdma_dynamic_rows( true );
    }

    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        retcode = piems_wdma_setup_buffers( pie_wchannels[i], 
                                    pstats.piew_num_buffers, 
                                    write_rows_per_buffer,
                                    row->bytes_out );

        if( retcode <= 0 ) {
            /* returns fail if we've run out of memory in the setup function */
            return SCANERR_OUT_OF_MEMORY;
        }
    }

    if( use_scaler ) {
        piems_wdma_init_scaler( strip->xy_in, scale->y_numerator,
                                scale->y_denominator, scale->y_scale );
    }

    /* davep 29-Mar-2013 ; run forever must be BEFORE launch */
    if( use_pipecut ) {
        piems_wdma_run_forever();
    }

    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_enable( pie_wchannels[i] );
    }

    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        piems_wdma_channel_launch( pie_wchannels[i] );
    }

    /* dump starting state that might help looking problems */
    //piems_rdma_debug_log();
    //piems_wdma_debug_log();

    /* davep 18-Oct-2005 ; feed canned data in from network to run through PIE
     * and HalfPack as a print/scan job
     */
    if( use_pipenet ) {
        pipenet_init();

        retcode = pipenet_multichannel_open( 
                                   pie_rchannels, 
                                   num_pie_rchannels,
                
                                   /* for now, want just read, no write */
                                   /* pie_wchannels, */
                                   /* num_pie_wchannels, */
                                   NULL,
                                   0,

                                   /* davep 03-Jul-2006 ; I need to get this
                                    * file from home! Has good comments on this
                                    * problem
                                    */
                                    pstats.scan_expected_rows,
//                                   total_rows_per_channel_in,
                                   total_rows_per_channel_out,

                                   /* read buffer allocation */
                                   pstats.pier_num_buffers,  /* buffers per channel */ 
                                   read_rows_per_buffer,  /* rows per buffer */
                                   row->bytes_in     /* bytes per row */

                                 );
        /* ignore error, hope for the best (pipenet a test/debug feature for
         * now) 
         */
    }

    /* davep 07-Mar-2006 ; adding software filter */
    if( filter_position==SV_SW_FILTER_AFTER_PIE ) {
        scerr = filter_start( pie_wchannels, 
                      num_pie_wchannels,
                      total_rows_per_channel_out, 
                      row->bytes_out   /* bytes per row */
                    );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }

    /* clear out the statistics so we can count interrupts, etc, for just this
     * PIE run
     */
    pie_clear_interrupt_stats();

    if( pipetap==PIPETAP_PIE && scan_debug_get_ip() != -1) {
        scan_data_type dtype;

        pie_pixel_type_to_scan_data_type( sv->pieout, &dtype );

        scan_debug_net_open( scan_debug_get_ip(), pie_wchannels, num_pie_wchannels );
        /* ignore error */

        /* davep 06-Apr-2012 ; adding pipetap net send metadata header */
        net_send_metadata( pie_wchannels, num_pie_wchannels, dtype,
                    row->pixels_out,  /* pixels per row padded */
                    row->pixels_out,  /* pixels per row (XXX sending the padded width) */
                    total_rows_per_channel_out, 
                    pie_get_bits_per_pixel( scanvar_get_pieout() ) );
    }

    /* davep 09-Aug-2013 ; PIE should be completely set up at this point. Tell
     * piemux to grab a snapshot of the registers that will be used in context
     * switches.
     */
    piemux_first_schedule();

    /* success! */
    return SCANERR_NONE;
}

static void
pie_done( void )
{
    int i;

    dbg2( "%s\n", __FUNCTION__ );

    /* shut everything down */
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        pie_rdma_channel_disable( pie_rchannels[i] );
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        pie_wdma_channel_disable( pie_wchannels[i] );
    }

    /* everything used should be back on the appropriate lists */
    piems_rdma_sanity();
    piems_wdma_sanity();

    /* we're done so clean up after ourselves */
    piems_rdma_close();
    piems_wdma_close();

    num_pie_rchannels = 0;
    num_pie_wchannels = 0;

    if( use_pipenet ) {
        pipenet_multichannel_close();
    }

    /* davep 24-May-2013 ; enable the soft bank switching if we're using piemux
     * (multiplexing multiple images through PIE)
     */
    piems_dma_close_bank(0);
    if( use_piemux ) {
        piems_dma_close_bank(1);
    }
}

static scan_err_t
hp_start( int total_rows_per_channel,
          struct pie_dma_row *row,
          int rows_per_buffer,
          int bpp )
{
    int retcode;
    int read_rows_per_buffer, write_rows_per_buffer;
    int buffer_cnt;
    const struct scanvars *sv;

    dbg2( "%s total_rows=%d pin=%d pout=%d bin=%d bout=%d ss=%d bpp=%d\n", 
                __FUNCTION__, total_rows_per_channel, 
                row->pixels_in, row->pixels_out, row->bytes_in, row->bytes_out,
                rows_per_buffer, bpp );

    buffer_cnt = pstats.hpw_num_buffers;

    read_rows_per_buffer = rows_per_buffer;
    write_rows_per_buffer = read_rows_per_buffer;

    piehp_set_bypass(false);
    switch (bpp)
    {
        case 1:
            piehp_set_bit_depth(1);
            break;
        case 2:
            piehp_set_bit_depth(2);
            break;
        case 4:
            piehp_set_bit_depth(4);
            break;
        case 8:
            piehp_set_bypass(true);
            break;

        default:
            /* should always be passed a valid bpp */
            XASSERT(0,bpp);
            break;
    }
    XASSERT(!( row->bytes_out & 0xf), row->bytes_out );

    /* davep 22-May-2013 ; add the extra buffers for pipecutter (could be zero
     * if PIC not the last link before pipecutter)
     */
    if( pipe_cutter_location==PIPE_CUTTER_LOCATION_AFTER_PIEHP ) {
        buffer_cnt += calc_extra_buffers_for_pipecut(write_rows_per_buffer);
    }
    
    /* set up write */
    num_hp_wchannels = 1;
    piehp_wchannels[0] = 0;
    retcode = piehp_wdma_open( piehp_wchannels, num_hp_wchannels, buffer_cnt, 
                     total_rows_per_channel, row->bytes_out );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* feed in some buffers, exactly enough to hold all the data we expect back
     */
    retcode = piehp_wdma_setup_buffers( piehp_wchannels[0], 
                          buffer_cnt, write_rows_per_buffer, 
                          row->bytes_out );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the setup function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* save so we know how to allocate new buffers later */
    pstats.hpw_rows_per_buffer = write_rows_per_buffer;

    piehp_wdma_enable( piehp_wchannels[0] );

    /* set up read */
    num_hp_rchannels = 1;
    piehp_rchannels[0] = 0;
    retcode = piehp_rdma_open( piehp_rchannels, num_hp_rchannels, buffer_cnt,
                     total_rows_per_channel, row->bytes_in );
    if( retcode <= 0 ) {
        /* returns fail if we've run out of memory in the dma_open() function */
        return SCANERR_OUT_OF_MEMORY;
    }

    /* HalfPack read will have no data of its own. All its data will come from PIE
     * write.
     */

    /* davep 29-Mar-2013 ; run forever must be BEFORE launch */
    if( use_pipecut ) {
        /* davep 15-Jun-2009 ; adding run forever */
        piehp_rdma_run_forever();
        piehp_wdma_run_forever();
    }

    /* need to explicitly turn on our channels */
    piehp_rdma_enable( piehp_rchannels[0] );
    piehp_rdma_channel_launch( piehp_rchannels[0] );

    /* davep 26-Apr-2006 ; (Bug 619) the read launch above will also cause
     * write to lauch because of halfpack's read/write sync behavior. But
     * launch write anyway just in case we're running HalfPack unsync'd.
     */
    piehp_wdma_channel_launch( piehp_wchannels[0] );

    /* set up data massaging */
    sv = scanvar_get();

    /* davep 24-Nov-2007 ; XXX temp hack ; flip the error diffusion and see what
     * happens (need to move into scanvars)
     */
#ifdef HAVE_HALFPACK_FULL_PIXEL_DIFFUSION
    if( sv->hp_bpp==4 ) {
        /* davep 03-Oct-2009 ; normal error diffusion is 4-bpp only */
        piehp_set_error_diffusion_mode( PIE_HP_CFG_NORMAL_ERRDIFF );
    }
    else { 
        /* pixels come in pairs! (headbang) */
        piehp_set_error_diffusion_mode( PIE_HP_CFG_PAIRED_ERRDIFF );
    }
#endif

    /* load any luts we might want to use */
#ifdef PIE_HP_SZDD_OFFSET 
    /* davep 23-Nov-2007 ; XXX temp hack to use the SZDD LUT instead of the
     * individual LUTs (get rid of the #ifdef!) 
     */
    if( sv->use_hp_size_lut ) {
        ASSERT( sv->hp_size_lut.data != NULL );
        ASSERT( sv->hp_diffus_lut.data != NULL );
        ASSERT( sv->hp_density_lut.data != NULL );

        piehp_set_szdd_sram( sv->hp_size_lut.data,
                             sv->hp_diffus_lut.data,
                             sv->hp_density_lut.data,
                             sv->hp_size_lut.num_entries );

    }
#else
    if( sv->use_hp_size_lut ) {
        piehp_set_size_sram( sv->hp_size_lut.data, 
                             sv->hp_size_lut.num_entries );
    }
    if( sv->use_hp_density_lut ) {
        piehp_set_density_sram(sv->hp_density_lut.data,
                               sv->hp_density_lut.num_entries );
    }
    if( sv->use_hp_diffus_lut ) {
        piehp_set_error_diffusion_sram(sv->hp_diffus_lut.data,
                                       sv->hp_diffus_lut.num_entries );
    }
#endif
    if( sv->use_hp_sbias_lut ) {
        piehp_set_signed_bias_sram(sv->hp_sbias_lut.data,
                                   sv->hp_sbias_lut.num_entries );
    }

    /* set up algorithm register (the first fields are simply true/false flags) */
    piehp_set_invert_data( sv->hp_alg_invert_data );
    piehp_set_include_accumerror_idx( sv->hp_alg_accum_err );
    piehp_set_include_signed_bias_idx( sv->hp_alg_sbias_idx );
    piehp_set_include_signed_bias_do_a_dot( sv->hp_alg_sbias_dot );
    piehp_set_do_a_dot_override( sv->hp_alg_doadot );

    /* zero is an invalid hp_alg_shift value so we have a 'use' flag on it as
     * well 
     */
    if( sv->use_hp_alg_shift ) {
        piehp_set_shift( sv->hp_alg_shift );
    }

    /* XXX should these have a 'use_' flag with them? They're not flags,
     * they're valued 
     */
    piehp_set_error_weight_1( sv->hp_alg_errweight1 );
    piehp_set_error_weight_2( sv->hp_alg_errweight2 );
    piehp_set_error_weight_3( sv->hp_alg_errweight3 );


    /* davep 01-Jan-2006 ; moving to scanvars */
#if 0
    /* Doug said to turn this on so I turn it on */
    piehp_set_do_a_dot_override(true);

    /* davep 21-Oct-2005 ; play with error weights with DougK (the magic
     * numbers come from DougK)
     */
#if 1
    piehp_set_include_accumerror_idx( true );
    piehp_set_error_weight_1( 1 );
    piehp_set_error_weight_2( 5 );
    piehp_set_error_weight_3( 3 );
#endif

    // Need to invert all scanned data going to the laser engine.
    // What about ink?
    piehp_set_invert_data(true);
#endif

    /* davep 12-Oct-2005 ; test horizontal replication */
    if( sv->use_hp_horiz_rep ) {
        piehp_set_replicate_horiz_data( true );
    }

    if ( pipetap==PIPETAP_HP && scan_debug_get_ip() != -1) {
        scan_debug_net_open( scan_debug_get_ip(), piehp_wchannels, num_hp_wchannels );
        /* ignore error */

        /* davep 07-Sep-2012 ; adding pipetap net send metadata header */
        net_send_metadata( piehp_wchannels, num_hp_wchannels,
                    SCAN_DATA_TYPE_MONO,
                    row->pixels_out,  /* pixels per row padded */
                    row->pixels_out,  /* pixels per row (XXX sending the padded width) */
                    total_rows_per_channel, 
                     bpp );
    }

//    piehp_dump();

    /* success! */
    return SCANERR_NONE;
}

static void
hp_done( void )
{
//    piehp_dump();

    /* shut everything down */
    piehp_rdma_disable( piehp_rchannels[0] );
    piehp_wdma_disable( piehp_wchannels[0] );

    /* everything used should be back on the appropriate lists */
    piehp_rdma_sanity();
    piehp_wdma_sanity();

    /* we're done so clean up after ourselves */
    piehp_rdma_close();
    piehp_wdma_close();

    num_hp_rchannels = 0;
    num_hp_wchannels = 0;
}

/**
 * \brief Shut down and clean up the software filter.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static void
filter_done( void )
{
    int i;

    for( i=0 ; i<num_filter_channels ; i++ ) {
        ice_filter_channel_disable( filter_channels[i] );
    }

    ice_filter_sanity();
    ice_filter_close();

    num_filter_channels = 0;
}

scan_err_t pipe_onetime_init( void ) // one time cold init
{
    dbg2( "%s\n", __FUNCTION__ );

    pic_wdma_init();

    pie_rdma_init();
    pie_wdma_init();
    piems_rdma_init();
    piems_wdma_init();
    piehp_wdma_init();
    piehp_rdma_init();

    /* davep 07-Mar-2006 ; adding software image filter(s) */
    ice_filter_init();

    /* data results will land here */
    ATInitList( &pipe_results );

    /* davep 01-Jun-2009 ; adding pipe cutter */
    pipe_cutter_onetime_init();

    /* davep 08-Dec-2010 ; adding firmware margins */
    fw_margin_onetime_init();

#ifdef HAVE_ICETEST
    /* davep 30-Mar-2011 ; adding icetest */
    icetest_idma_init();
#endif

    /* davep 28-May-2013 ; adding piemux */
    piemux_onetime_init();

    return SCANERR_NONE;
}

/**
 * \brief  Run the strip predictor clear through 
 *
 * Runs the strip predictor all the way to last row of the expected rows.
 * Returns the strip that contains the last row.
 *
 * Created due to Bug 397.
 * 
 * \param[in] st 
 * already initialized strip predictor state structure
 *
 * \param[in] expected_rows
 * Number of rows we plan to run through scan. We look for the strip containing
 * the last row.
 *
 * \param[out] strip
 * Boundries of the read and write strip containing the last row of the scan.
 *
 * \author David Poole
 * \date 10-Jan-2006
 *
 */

static void
find_last_strip( struct predict_state *st, int expected_rows, int strip[4] )
{
    int num_strips;
    int last_row_index;

//    dbg2( "%s %d\n", __FUNCTION__, expected_rows );

    /* run the predictor until we hit a state where the expected_rows'th row
     * is included
     */

    strip[0] = strip[1] = strip[2] = strip[3] = 0;

    /* -1 to convert from count to index */
    last_row_index = expected_rows-1;
    
    num_strips = 0;

    while( 1 ) {
        strip_next( st, strip );

#if 0
    dbg2( "strip %d=[%d,%d][%d,%d]\n",  
                num_strips,
                strip[STRIP_READ_FIRST], strip[STRIP_READ_LAST], 
                strip[STRIP_WRITE_FIRST], strip[STRIP_WRITE_LAST] );
#endif
        
        num_strips += 1;

        /* beware of stupid mistakes */
        XASSERT( num_strips<100000, num_strips );

        /* does this strip contain the last row? */
        if( strip[STRIP_READ_FIRST] <= last_row_index 
                && strip[STRIP_READ_LAST] >= last_row_index ) {
            break;
        }

    }

    dbg2( "%s done strip=[%d,%d][%d,%d] num_strips=%d\n", __FUNCTION__, 
                strip[STRIP_READ_FIRST], strip[STRIP_READ_LAST], 
                strip[STRIP_WRITE_FIRST], strip[STRIP_WRITE_LAST],
                num_strips );
}

static void pipe_get_pie_scale( const struct scanvars *sv, 
                                struct pie_scale *scale )
{
    memset( scale, 0, sizeof(struct pie_scale) );

    scanvar_get_pie_scale( sv, &scale->x_numerator, &scale->x_denominator,
                               &scale->y_numerator, &scale->y_denominator );

    /* calculate the integer representation of the scale factors */
    pie_xy_calc_xyscale( scale, sv->xy_scaler_mode );

    pie_dbg2_scale( scale );
}

/**
 * \brief calculate how many rows necessary to do a stripsize correct scan
 *
 * Based on an original number of rows, peek into the future and, using
 * the PIE scaling factor and stripsize, figure out how many rows we'd really
 * have to scan in order to correctly fit PIE's stripsize requirements.
 *
 * \author David Poole
 * \date 21-Sep-2005
 *
 * \note This function is a bit of a hack. We have to tweak the scan layer's
 * number of rows captured to keep PIE happy. However, we don't want the scan
 * layer to even know PIE really exists. Also, we're calculating a lot of these
 * numbers again down in pipe_start() so the double calculation is a little
 * annoying.
 */

void pipe_calc_rows( int expected_rows, 
                     int *scan_required_rows, 
                     int *pie_required_rows )
{
    scan_err_t scerr;
    struct pie_scale scale;
    int pie_total_rows_out;
    int needed_rows_in;
    int client_rows_out;
    struct predict_state strip_state;
    int last_row_of_last_strip;
    int last_strip[4];
    const struct scanvars *sv;
    struct pie_strip strip;
    uint64_t numer64, denom64, quotient64;

    sv = scanvar_get();

    if( sv->use_colorshift ) {
        /* colorshift only useful when scanning color */
        XASSERT( sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
    }
        
    /* get PIE's scaling factor so we can figure out how many rows we expect */
    pipe_get_pie_scale( sv, &scale );

    // Determine how many rows pie will produce after scaling.
    pie_xy_calc_expected_rows( expected_rows, scale.y_scale, 
                          sv->xy_scaler_mode, &pie_total_rows_out );
		
    memset( &strip, 0, sizeof(struct pie_strip) );
    strip.use_cs = sv->use_colorshift;
    strip.use_mf = sv->use_mf;
    strip.use_xy = !(sv->xy_scaler_mode==PIE_XY_SCALER_BYPASS && true);

    scerr = pie_calc_stripsize( &strip, scale.y_scale, sv->xy_scaler_mode );
    XASSERT( scerr==SCANERR_NONE, scerr );
	
    pie_dbg2_scale( &scale );
    pie_dbg2_strip( &strip );

    /* davep 10-Jan-2006 ; Bug 397. Have to run the predictor clear through to
     * find the last strip so we know how to stop PIE. 
     *
     * Only run the predictor if we're doing a scan using pie.
     */
    /* -1 to convert from count to index */
    last_row_of_last_strip = expected_rows-1;
    if( sv->use_pie ) {
//        dbg2( "running strip predictor for strip size=%d y=%d/%d fracy=%d\n", 
//                    pie_strip_size_in-total_extra_rows, 
//                    scale.y_numerator, scale.y_denominator, pie_fracY );

        if( sv->xy_scaler_mode==PIE_XY_SCALER_LINEAR ) {
            strip_start( &strip_state, strip.xy_in,
                    scale.y_numerator, scale.y_denominator, scale.y_scale, STRIPPR_MODE_LINEAR );
        }
        else {
            XASSERT( sv->xy_scaler_mode==PIE_XY_SCALER_RAPR, sv->xy_scaler_mode );
            strip_start( &strip_state, strip.xy_in,
                    scale.y_numerator, scale.y_denominator, scale.y_scale, STRIPPR_MODE_RAPR );
        }

        /* run the strip predictor clear through to find the last strip we need
         * for this many rows
         */
        find_last_strip( &strip_state, expected_rows, last_strip );

        last_row_of_last_strip = last_strip[STRIP_READ_LAST];
    }

    /* davep 10-Jan-2006 ; changed to tweak based on the last row of last
     * predicted strip, with additional data for multifilter. We need to
     * capture at LEAST as much data as we expect. I'm leveraging knowledge
     * that I always replicate the top two rows since we always start capturing
     * data at row 0 but we need rows -1,-2 to get row 0 out of MultiFilter.
     */
	
    /* +1 to convert from index to count */
    needed_rows_in = last_row_of_last_strip + strip.total_extra_rows + 1;
    XASSERT( needed_rows_in >= expected_rows, needed_rows_in );
		
    /* round up the row count to the next % stripsize */
    needed_rows_in = ( (needed_rows_in + (strip.rdma_in-1))
                    / strip.rdma_in) * strip.rdma_in;

	    /* davep 11-Jul-2006 ; now with our new PIE input, determine how many rows
     * pie will produce after scaling.
     */
    pie_xy_calc_expected_rows( needed_rows_in, scale.y_scale, 
                          sv->xy_scaler_mode, &pie_total_rows_out );

    /* +1 to convert from index to count */
    dbg2( "%s needed_rows_in rounded up from %d to %d giving %d rows out\n", __FUNCTION__, 
                last_row_of_last_strip + strip.total_extra_rows + 1,
                needed_rows_in, pie_total_rows_out );
    
    // brads
    // The XY scaler does not always produce the number of lines that one would
    // intuitively expect based on the rows in and the scale factor. However,
    // for scans-to-host the host expects the "intuitive" number of lines, not
    // the XY scaler number of lines. If after calculating how many rows the
    // scaler will produce (and rounding up to the row count to the next %
    // stripsize) we discover that we'll send less than what the host
    // is expecting, add on another pie_strip_size_in's worth of rows.

    /* davep 10-Jan-2006 ; for example, 1500 scaled by 1/3 is 499 rows.
     * The XYScale factors are normalized by 65536. And remember this is all
     * 100% integer math.
     *
     * xyscale of 1/3 = ((1 * 65536)/3) = 21845
     * 1500 rows scaled by 21845 = (1500 * 21845) / 65536 = 499
     *  
     *  scale of a/b = ((a * 65536)/b)
     *  'N' scaled = ((N * scaler)/65536)
     */

    /* davep 10-Aug-2012 ; get rid of floats for Linux kernel. Note we _must_
     * do this calculation in base-10 so we get the human expected (intuitive)
     * length out. The *1000 will give us a decent level of precision. 
     */
    numer64 = (uint64_t)expected_rows * (uint64_t)scale.y_numerator;
    /* multiply by 1000 ~= 1024 so need 10-bits of headroom for this to work */
    XASSERT( (numer64 & ((uint64_t)0x3ff<<54))==0, expected_rows );
    numer64 *= 1000;
    denom64 = (uint64_t)scale.y_denominator*1000;
    quotient64 = safeint_divide_uint64( numer64, denom64 );
    /* make sure we can cram the result into a 32-bit */
    XASSERT( (quotient64 >> 32)==0, (uint32_t)(quotient64>>32));
    client_rows_out = (uint32_t)quotient64;

    dbg2( "%s client_rows_out=%d\n", __FUNCTION__, client_rows_out );

    if (client_rows_out > pie_total_rows_out)
    {
        dbg2("Padding rows, client_rows_out=%d and pie_total_rows_out=%d\n",
                    client_rows_out,pie_total_rows_out);
        needed_rows_in += strip.rdma_in;
        pie_xy_calc_expected_rows( needed_rows_in, scale.y_scale, 
                              sv->xy_scaler_mode, &pie_total_rows_out );
        /* davep 12-Jan-2006 ; now that we need two numbers of rows, also need
         * to calculate a new last strip so we make sure we always send at
         * least what the user expects.
         */
        if( sv->use_pie ) {
            /* if our last write output already meets the requirement, we don't
             * need another strip
             */
            /* -1 to convert from count to index */
            if( last_strip[STRIP_WRITE_LAST] < (pie_total_rows_out-1) ) {
                strip_next( &strip_state, last_strip );
                last_row_of_last_strip = last_strip[STRIP_READ_LAST];
                dbg2( "run another strip to meet expected rows\n" );
                dbg2( "new last strip=[%d,%d][%d,%d]\n", 
                            last_strip[STRIP_READ_FIRST], 
                            last_strip[STRIP_READ_LAST], 
                            last_strip[STRIP_WRITE_FIRST], 
                            last_strip[STRIP_WRITE_LAST] );
            }
        }
    }
    dbg2( "%s in=%d out=%d expected=%d needed=%d lrls=%d\n", __FUNCTION__, 
                strip.rdma_in, strip.wdma_out,
                expected_rows, needed_rows_in,
                last_row_of_last_strip );

    /* check for math mistakes */
    XASSERT(pie_total_rows_out >= client_rows_out, pie_total_rows_out );

    XASSERT( needed_rows_in % strip.rdma_in==0, strip.rdma_in );
    
    /* strictly > because we're comparing count to index */
    XASSERT( needed_rows_in > last_row_of_last_strip, needed_rows_in );

    /* davep 18-Jan-2006 ; make sure our future calculations come out
     * correctly.  I don't want to have to carry the strip prediction
     * throughout the rest of the code. (Running the strip predictor all the
     * way through is a new necessity; I originally assumed I could calculate
     * all the numbers I needed based on the scan expected_rows.) I need the
     * number of rows *out* of PIE in order to properly set up HalfPack.
     * However, I don't want to have to pass another number up from this
     * function then back down into the scanpipe functions.
     * The pie setup functions call pie_xy_calc_expected_rows() to set up
     * HalfPack but I don't know if this will still work. In this function,
     * while I still have the last predicted strip, run a few functions and
     * test to make sure they come out the same. If not, then you'll never see
     * this comment because I'll have to rewrite a pile of code.
     */
    if( sv->use_pie ) {

        /* +1 to convert from index to count */
        pie_xy_calc_expected_rows( last_row_of_last_strip+1, scale.y_scale, 
                              sv->xy_scaler_mode, &pie_total_rows_out );
                                                        
        dbg2( "%s strip_write_last=%d pie_total_rows_out=%d\n", __FUNCTION__,
                    last_strip[STRIP_WRITE_LAST], pie_total_rows_out );

        /* davep 15-Jun-2006 ; TEMPORARILY remove this check for Linear mode */
        /* -1 to convert from count to index ; note I'm doing an exactly equal
         * check here
         */
        /* davep 15-Jul-2006 ; exactly equal doesn't work because linear mode
         * is a pain in the neck. Use <= so at least we know we won't stall
         */
        XASSERT( pie_total_rows_out-1<=last_strip[STRIP_WRITE_LAST], pie_total_rows_out );
    }

    *scan_required_rows = needed_rows_in;
    /* +1 to convert from index to count */
    *pie_required_rows = last_row_of_last_strip+1;
}

/**
 * \brief Start up the Scan DMA pipe with only PIC in the mix.
 *
 * The DMA "pipe" is the connections of PIC, PIE, and HalfPack. In order to keep the
 * permutations from getting out of hand and creating an ugly pile of
 * unmaintainable, undebuggable code, I'm splitting PIC, PIC->PIE, and
 * PIC->PIE->HalfPack pipe start into three functions.
 *
 * \param pixels_per_row 
 *  the number of PIC pixels (8 or 16bpp) in a row; should already be DMA aligned
 *
 * \param expected_rows 
 *  the number of total rows we should expect to receive; should already be DMA
 *  aligned
 *
 * \param one_buffer 
 *  return all the results in a single buffer; used by Cal
 *
 * \author David Poole
 * \date 09-Sep-2005
 *
 */

static scan_err_t
pipe_start_pic_only( int scan_pixels_per_row, 
                     int scan_expected_rows,
                     bool one_buffer )
{
    scan_err_t scerr;
    const struct scanvars *sv;
    int pic_bpp;  
    int margin_pixels_per_row_out, margin_pixels_per_row_in;
    int final_pixels_per_row_out;
    int pic_strip_size;
    struct pic_pixels_per_row pic_ppr;
    uint8_t channels[3];

    dbg2( "%s %d %d\n", __FUNCTION__, scan_pixels_per_row, scan_expected_rows );

    sv = scanvar_get();

    /* use a small strip size since we're only using PIC */
    pic_strip_size = 15;

    /* davep 01-Mar-2013 ; consolidate pic pixels calculations. Calculate this
     * now so we know how many pixels will hit PIE
     */
    memset( &pic_ppr, 0, sizeof(struct pic_pixels_per_row) );
    pic_ppr.use_lrmargins0 = true;
    scerr = pic_calc_pixels_per_row( sv, &pic_ppr, scan_pixels_per_row );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* fire up the image processing (ICE) blocks */
    if (one_buffer) {
        scerr = pic_start( scan_expected_rows, scan_pixels_per_row, scan_expected_rows, 
                            1, &pic_ppr );
        pic_strip_size = scan_expected_rows;
    }
    else {
        scerr = pic_start( scan_expected_rows, scan_pixels_per_row, pic_strip_size,
                           pstats.picw_num_buffers, &pic_ppr );
    }
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* davep 21-Feb-2012 ; adding firmware margins to pic_only so can handle
     * 16-bpp scan-to-host
     */
    if( sv->use_fw_margins ) {
        scerr = fw_margin_open();
        XASSERT( scerr==0, scerr );

        ASSERT( !one_buffer );

        margin_pixels_per_row_in = pic_ppr.pic_pixels_per_row_out;

        scerr = fw_margin_setup_margins( sv, 
                        margin_pixels_per_row_in, &margin_pixels_per_row_out );

        if( scerr != SCANERR_NONE ) {
            return scerr;
        }

        scerr = fw_margin_start( scan_expected_rows, 
                                margin_pixels_per_row_in, margin_pixels_per_row_out, 
                                pic_strip_size );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }

        final_pixels_per_row_out = margin_pixels_per_row_out;
    }
    else {
        /* fix a compiler warning */
        margin_pixels_per_row_in = margin_pixels_per_row_out = 0;

        final_pixels_per_row_out = pic_ppr.pic_pixels_per_row_out;
    }

    pic_bpp = scanvar_get_pic_bpp();

    /* At this point we know how big the resulting scan will be. Pass it on in
     * the message.
     */
    if( use_scanman ) {
        /* send along the X and Y dimensions of the upcoming data */

        scanlib_send_scan_size( final_pixels_per_row_out,  /* pixels per row padded */
                        final_pixels_per_row_out,  /* pixels per row */
                        scan_expected_rows, pic_bpp );
    }

    if( use_pipecut ) {
        /* don't have piconly + pipecutter + margin support */
        XASSERT( !sv->use_fw_margins, sv->id );

        /* davep 10-Apr-2013 ; adding dual pipecutters */
        if( use_nsensor ) {
            memset( channels, 0, sizeof(channels) );
            if( num_pic_channels==6 ) {
                /* color dual scan */
                channels[0] = PIC_DMA_CHANNEL_CEVEN_0;
                channels[1] = PIC_DMA_CHANNEL_CEVEN_1;
                channels[2] = PIC_DMA_CHANNEL_CEVEN_2;
                scerr = pipe_cutter_open( PAGE_SIDE_0, channels, 3);
                if( scerr != SCANERR_NONE ) {
                    return scerr;
                }
                channels[0] = PIC_DMA_CHANNEL_CODD_0;
                channels[1] = PIC_DMA_CHANNEL_CODD_1;
                channels[2] = PIC_DMA_CHANNEL_CODD_2;
                scerr = pipe_cutter_open( PAGE_SIDE_1, channels, 3);
                if( scerr != SCANERR_NONE ) {
                    return scerr;
                }
            }
            else {
                /* mono dual scan */
                XASSERT( num_pic_channels==2, num_pic_channels );
                channels[0] = PIC_DMA_CHANNEL_MEVEN;
                scerr = pipe_cutter_open( PAGE_SIDE_0, channels, 1);
                if( scerr != SCANERR_NONE ) {
                    return scerr;
                }
                channels[0] = PIC_DMA_CHANNEL_MODD;
                scerr = pipe_cutter_open( PAGE_SIDE_1, channels, 1);
                if( scerr != SCANERR_NONE ) {
                    return scerr;
                }
            }
        }
        else {
            scerr = pipe_cutter_open( PAGE_SIDE_0, pic_channels, num_pic_channels);
            if( scerr != SCANERR_NONE ) {
                return scerr;
            }
        }
        /* davep 02-Jun-2009 ; if memory fails, use less memory. Duh. But fail
         * hard on unexpected errors.
         */
        XASSERT( scerr==0||scerr==SCANERR_OUT_OF_MEMORY, scerr );

        if( pipetap==PIPETAP_CUTTER && scan_debug_get_ip() != -1) {
            scan_debug_net_open( scan_debug_get_ip(), pic_channels,
                                            num_pic_channels );
            /* ignore error */
        }

        /* davep 22-Apr-2013 ; pipecut 2.0 */
        pipe_cutter_set_recycler( return_icebuf_to_pic_wdma );
    } /* end if use_pipecut */

    pic_interrupt_enable();

    /* success! */
    return SCANERR_NONE;
}

/**
 * \brief fire up MultiFilter 
 *
 * Start the PIE MultiFilter block. Change strip size calculates to take into
 * account the extra 4 (two above, two below) rows of context each row needs to
 * make it through MultiFilter.
 *
 * Can completely change input and output strip size if the +4 pushes the sizes
 * above the max.
 *
 * \author David Poole
 * \date 16-Oct-2005
 *
 * \param[in] pie_strip_size_in
 * \param[in] pie_fracY
 * \param[in] pie_row 
 *
 */

static void
start_pie_multifilter( int pie_strip_size_in, int pixels_per_row_in )
{
    /* adjust down the input and output strip size if we're now too big 
     * davep 16-Oct-2005 ; TODO pie_filter_setup() hardwires WAY too many
     * things; need to split it out into several functions.
     */
    pie_filter_setup( pie_strip_size_in, pixels_per_row_in );
}

static void
start_pie_tcns( void )
{
    const struct scanvars *sv;

//    dbg2( "%s\n", __FUNCTION__ );

    pie_tcns_set_bypass( true );
    pie_tcns_enable_fw_access( true );

    sv = scanvar_get();
    if( sv->use_tcns_lut ) {
        pie_tcns_set_lut( sv->tcns_lut.data );
    }
    pie_tcns_set_bypass( false );
    pie_tcns_enable_fw_access( false );

//    pie_tcns_dump();
}

/**
 * \brief Start the Scan DMA plumbing with just PIC and PIE.
 *
 * See also pipe_start_pic_only() 
 *
 * \param pixels_per_row 
 *  the number of PIC pixels (8 or 16bpp) in a row; should already be DMA
 *  aligned. Need to calculate PIE output pixels from this value.
 *
 * \param scan_expected_rows 
 *  The number of total rows we should expect to receive; should already be DMA
 *  aligned. 
 *
 * \param pie_expected_rows
 *  The number of total rows we should expect to run through PIE. Computed
 *  based on running the strip predictor. (added davep 10-Jan-06 for Bug 397)
 *
 * \author David Poole
 * \date 09-Sep-2005
 *
 */

static scan_err_t
pipe_start_pic_and_pie_only( int scan_pixels_per_row, 
                             int scan_expected_rows,
                             int pie_expected_rows )
{
    scan_err_t scerr;
    struct pie_dma_row pie_row;
    uint32_t pie_pixel_size;
    int pie_xy_pad;
    int pie_total_rows_out;
    struct pie_scale scale;
    const struct scanvars *sv;
    int pie_pixels_per_row_in;
    uint32_t pixels_per_row_out_unpadded;
    int margin_pixels_per_row_out, margin_pixels_per_row_in;
    struct pie_strip strip;
    struct pic_pixels_per_row pic_ppr;

    dbg2( "%s ppr=%d ser=%d per=%d\n", __FUNCTION__, scan_pixels_per_row, scan_expected_rows,
                pie_expected_rows );

    /** Make sure to call pie_reset() or something like that before this
     * function is done
     */
    pie_soft_setup();

    sv = scanvar_get();

    /* how big are the pixels coming to PIE RDMA going to be (being silly
     * verbose so can catch numbers not in 8,16) 
     */
    if( sv->pic_bpp == 8 ) {
        pie_pixel_size = 8;
    }
    else {
        XASSERT( sv->pic_bpp==16, sv->pic_bpp );
        pie_pixel_size = 16;
    }

    /* davep 30-Aug-2005 ; run some calculations to pad with the current BPP
     * and output of PIE
     */

    /* pie_xy_pad is measured in pixels, not bytes */
    pie_xy_pad = 0;
    
    /* davep 07-Sep-2005 ; add in some debugging pad; MUST BE DMA ALIGNED */
//    pie_xy_pad += 32;

    /* get PIE's scaling factor so we can figure out how many rows we expect */
    pipe_get_pie_scale( sv, &scale );

    /* davep 31-Mar-2009 ; ICE Lite XYScale can't scale color so scale using PIC */
//    pic_pixels_per_row_out = scan_pixels_per_row;

    /* davep 01-Mar-2013 ; consolidate pic pixels calculations. Calculate this
     * now so we know how many pixels will hit PIE
     */
    memset( &pic_ppr, 0, sizeof(struct pic_pixels_per_row) );
    pic_ppr.use_lrmargins0 = true;
    pic_ppr.use_chipgap = false; /* TODO add run-time choice of hardware chipgap */
    pic_ppr.use_hscale = sv->use_pic_scale;
    scerr = pic_calc_pixels_per_row( sv, &pic_ppr, scan_pixels_per_row );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* pixels_per_row_out_unpadded is the pixels out scaled by a base-10. It's
     * the "intuitive" scale. If the user says 1500 pixels scaled by 1/3, the
     * user expects 500 pixels out, not 1499. (True story.)
     */
    pixels_per_row_out_unpadded = 0;

    /* davep 08-Dec-2010 ; adding firmware margins (sit between PIC and PIE) */
    if( sv->use_fw_margins ) {
        scerr = fw_margin_open();
        XASSERT( scerr==0, scerr );

        margin_pixels_per_row_in = pic_ppr.pic_pixels_per_row_out;

        scerr = fw_margin_setup_margins( sv, 
                        margin_pixels_per_row_in, &margin_pixels_per_row_out );

        if( scerr != SCANERR_NONE ) {
            return scerr;
        }

        pie_pixels_per_row_in = margin_pixels_per_row_out;
    }
    else {
        pie_pixels_per_row_in = pic_ppr.pic_pixels_per_row_out;

        /* fix a compiler warning */
        margin_pixels_per_row_in = margin_pixels_per_row_out = 0;
    }

    pixels_per_row_out_unpadded = safeint_scale_uint32( pie_pixels_per_row_in, 
                                        scale.x_numerator, 
                                        scale.x_denominator );

    /* determine PIE's output rows and columns based on the current settings */
    pie_xy_calc_row( pie_pixels_per_row_in, 
                  pie_pixel_size,
                  sv->pieout, 
                  &scale, 
                  pie_xy_pad,
                  sv->xy_scaler_mode,
                  &pie_row );

    pie_dbg2_row( &pie_row );

    if( pixels_per_row_out_unpadded > pie_row.pixels_out ) {
        pixels_per_row_out_unpadded = pie_row.pixels_out;
    }

    if(sv->pie_pad_to_pixel_width) {

        /* davep 25-Apr-2013 ; if we need to pad to page width, pad the
         * output to the same size as the input
         */

        if( sv->pie_pad_to_pixel_width > pie_row.pixels_out)
        {
            pie_xy_pad = sv->pie_pad_to_pixel_width - pie_row.pixels_out;
            dbg2("pie padding output of %d by %d to fit scaled into upper left\n",
                        pie_row.pixels_out, pie_xy_pad );
            pie_xy_calc_row( pie_pixels_per_row_in, 
              pie_pixel_size,
              sv->pieout, 
              &scale, 
              pie_xy_pad,
              sv->xy_scaler_mode,
              &pie_row );

            /* in the case of pie_pad_to_pixel_width, the padding is meaningful data
             * (for example, we're trying to push a copy into the upper left) so
             * tell downstream our unpadded size is our padded size
             */
            pixels_per_row_out_unpadded = pie_row.pixels_out;
        }
    }

    /* davep 07-Jul-2006 ; calculate xy output based on what xy will see; we
     * have to pad scan to fill up a buffer but that won't necessarily match a
     * strip (strip != buffer)
     */
    pie_xy_calc_expected_rows( pie_expected_rows, scale.y_scale, 
                          sv->xy_scaler_mode, &pie_total_rows_out );

    /* davep 14-Nov-2005 ; some hardware platforms (e.g., Ink) need rowsize to
     * be bigger than our usual 16-byte aligned
     */
    if( sv->pie_row_align_bytes && (pie_row.bytes_out % sv->pie_row_align_bytes) != 0 ) {

        /* davep 14-Jan-2013 ; pad pixels_out to modulo pie_row_align_bytes */
        pie_xy_pad += ((pie_row.pixels_out+(sv->pie_row_align_bytes-1)) 
                                    & ~(sv->pie_row_align_bytes-1)) -
                        pie_row.pixels_out;

        dbg1( "adjusting pie row extra pad to modulo=%d pad=%d pout=%d\n", 
                    sv->pie_row_align_bytes, pie_xy_pad,
                    pie_row.pixels_out+pie_xy_pad );

        /* recalculate row sizes */
        pie_xy_calc_row( pie_row.pixels_in, 
                      pie_pixel_size,
                      sv->pieout, 
                      &scale, 
                      pie_xy_pad,
                      sv->xy_scaler_mode,
                      &pie_row );

        pie_dbg2_row( &pie_row );

        XASSERT( pie_row.bytes_out % sv->pie_row_align_bytes==0, pie_row.bytes_out);

        if( pixels_per_row_out_unpadded > pie_row.pixels_out ) {
            pixels_per_row_out_unpadded = pie_row.pixels_out;
        }
    }

    dbg2( "pie_expected_rows=%d pie_total_rows_out=%d\n", 
                pie_expected_rows, pie_total_rows_out );

    /* davep 24-May-2011 ; integrate Color Shift into this mess */
    if( sv->use_colorshift ) {
        /* colorshift only useful when scanning color */
        XASSERT( sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
    }

    memset( &strip, 0, sizeof(struct pie_strip) );
    strip.use_cs = sv->use_colorshift;
    strip.use_mf = sv->use_mf;
    strip.use_xy = !(sv->xy_scaler_mode==PIE_XY_SCALER_BYPASS && true);

    scerr = pie_calc_stripsize( &strip, scale.y_scale, sv->xy_scaler_mode );
    XASSERT( scerr==SCANERR_NONE, scerr );

    pie_dbg2_strip( &strip );

    if( sv->use_mf ) {
        start_pie_multifilter( strip.mf_in, pie_row.pixels_in );
    }

    XASSERT( scan_expected_rows % strip.rdma_in==0, strip.rdma_in );

    /* davep 23-May-2011 ; add color shift */
    if( sv->use_colorshift ) {
        /* colorshift only useful when scanning color */
        XASSERT( sv->cmode==SCAN_CMODE_COLOR, sv->cmode );
        pie_cs_setup( strip.cs_in, 
                        sv->cis_colorshift_round_mode, 
                        sv->cis_colorshift_mmode,
                        sv->cis_colorshift_mult );
        pie_cs_set_bypass( false );

        pie_cs_dump();
    }

    if( sv->use_csc ) {
        /* turn on all three color conversion blocks */
        pie_csc_setup();
    }

    if( sv->use_tcns ) {
        start_pie_tcns();
    }

    /* fire up the image processing (ICE) blocks */
    scerr = pic_start( scan_expected_rows, scan_pixels_per_row, strip.rdma_in, 
                       pstats.picw_num_buffers, &pic_ppr );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* davep 08-Dec-2010 ; adding firmware margins */
    if( sv->use_fw_margins ) {

        /* davep 18-Jun-2013 ; use the PIC rows_per_buffer as the fw margins
         * rows_per_buffer. FW margins must have identical input/output buffer
         * sizes. (Was using the PIE RDMA strip size.)
         */
        scerr = fw_margin_start( scan_expected_rows, 
                                margin_pixels_per_row_in, margin_pixels_per_row_out, 
                                pstats.picw_rows_per_buffer );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }

    /* davep 10-Jan-2006 ; (bug 397) make sure we feed in the rows PIE is
     * expecting for its predicted strippage
     */
    XASSERT( scan_expected_rows>=pie_expected_rows, pie_expected_rows );
    scerr = pie_start( pie_expected_rows, 
                       &pie_row, 
                       &strip,
                       pie_pixel_size, 
                       pie_xy_pad, 
                       &scale  );

    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* completed PIE read buffers send back to PIC read */
    /* The PIEMS driver runs in threadspace. Handle buffers in the message
     * loop so we can properly lock out interrupts.
     */

    /* At this point we know how big the resulting scan will be. Pass it on to
     * ScanMan in a message. Store in a global so we can dump it out in a debug
     * message after the scan is complete.
     */
    if( use_scanman ) {
        /* send along the X and Y dimensions of the upcoming data */

        scanlib_send_scan_size( pie_row.pixels_out,  /* pixels per row padded */
                        pixels_per_row_out_unpadded,  /* pixels per row */
                        pie_total_rows_out, 
                        pie_get_bits_per_pixel( scanvar_get_pieout() ) );
    }

    if( use_pipecut ) {
        scan_data_type dtype;

        scerr = pipe_cutter_open( PAGE_SIDE_0, pie_wchannels, num_pie_wchannels);
        if( scerr != SCANERR_NONE ) {
            /* davep 02-Jun-2009 ; if memory fails, use less memory. Duh. But fail
             * hard on unexpected errors.
             */
            XASSERT( scerr==SCANERR_OUT_OF_MEMORY, scerr );
            return scerr;
        }

        /* davep 31-May-2013 ; adding PIE multiplexing */
        if( use_piemux ) {
            scerr = pipe_cutter_open( PAGE_SIDE_1, pie_wchannels, num_pie_wchannels);
            if( scerr != SCANERR_NONE ) {
                /* davep 02-Jun-2009 ; if memory fails, use less memory. Duh. But fail
                 * hard on unexpected errors.
                 */
                XASSERT( scerr==SCANERR_OUT_OF_MEMORY, scerr );
                return scerr;
            }
        }

        /* davep 07-Sep-2012 ; we're using PIE. So PIE is the last link in the
         * chain before we hit pipecut. So we need to find the pipetap data
         * type using PIE's data type
         */
        pie_pixel_type_to_scan_data_type( sv->pieout, &dtype );

        if( pipetap==PIPETAP_CUTTER && scan_debug_get_ip() != -1) {
            scan_debug_net_open( scan_debug_get_ip(), pie_wchannels,
                                            num_pie_wchannels );
            /* ignore error */

            net_send_metadata( pie_wchannels, num_pie_wchannels, dtype,
                        pie_row.pixels_out,  /* pixels per row padded */
                        pixels_per_row_out_unpadded,  /* pixels per row */
                        pie_total_rows_out, 
                        pie_get_bits_per_pixel( scanvar_get_pieout() ) );
        }
        pipe_cutter_set_recycler( return_icebuf_to_pie_wdma );
    } /* end if use_pipecut */

    g_final_bytes_per_row = pie_row.bytes_out;
    g_final_total_rows = pie_total_rows_out;

    pic_interrupt_enable();
    pie_interrupt_enable();

    /* success! */
    return SCANERR_NONE;
}

/**
 * \brief get the min/max pixel sizes for print 
 *
 * In order to not confuse the laser engine, we need to meet a minimum and
 * maximum number of output pixels. Otherwise we'll corrupt the output as the
 * last engine tries to wrap the pixels (max) or the laser will underrun (min).
 *
 * This function is a bit of a cheap hack. I'm not sure these min/max sizes are
 * correct.
 *
 * \author David Poole
 * \date 01-Nov-2005
 *
 */

static void
get_hp_minmax_pixels( int dpi, int *min, int *max )
{
     /* XXX I'm flat out guessing on the the min/max number of pixels */
#define HP_MIN_PIXELS 1200 /* 2" * 600 DPI */
#define HP_MAX_PIXELS 4992 /* sorta 8.5" * 600 DPI */
//#define HP_MAX_PIXELS 5120 /* sorta 8.53" * 600 DPI */

    switch( dpi ) {
        case 300 :
            *min = HP_MIN_PIXELS/2;
            *max = HP_MAX_PIXELS/2;
            break;

        case 600 :
            *min = HP_MIN_PIXELS;
            *max = HP_MAX_PIXELS;
            break;

        default: 
            /* as of this writing (1-Nov-05), we only support 300 and 600 DPI
             * printing 
             */
            XASSERT(0,dpi);
            *min = 0;
            *max = 0;
            break;
    }
#undef HP_MIN_PIXELS
#undef HP_MAX_PIXELS
}

/**
 * \brief  Start the scan DMA plumbing with all the toys: PIC, PIE, and
 * HalfPack.
 *
 * See also pipe_start_pic_only() and pipe_start_pic_and_pie_only()
 *
 * \author David Poole
 * \date 09-Sep-2005
 *
 */

static scan_err_t
pipe_start_pic_pie_hp( int scan_pixels_per_row, 
                       int scan_expected_rows,
                       int pie_expected_rows )
{
    scan_err_t scerr;
    struct pie_dma_row pie_row, piehp_row;
    uint32_t pie_pixel_size;
    int pie_xy_pad;
    int pie_total_rows_out;
    struct pie_scale scale;
    struct pie_strip strip;
    int hp_total_rows;
    int hp_min_pixels, hp_max_pixels;
    const struct scanvars *sv;
    int pic_rpb;
    struct pic_pixels_per_row pic_ppr;

    dbg2( "%s %d %d %d\n", __FUNCTION__, scan_pixels_per_row, 
                scan_expected_rows, pie_expected_rows );

    /* davep 02-Jan-2006 ; start using a lot more of scanvar */
    sv = scanvar_get();

    /* Reset PIE back to clean power-on values. Don't touch PIC since
     * calibration has PIC already set up for us
     */
    pie_soft_setup();

    /* how big are the pixels coming to PIE RDMA going to be (being silly
     * verbose so can catch numbers not in 8,16) 
     */
    if( sv->pic_bpp == 8 ) {
        pie_pixel_size = 8;
    }
    else {
        XASSERT( sv->pic_bpp==16, sv->pic_bpp );
        pie_pixel_size = 16;
    }

    /* davep 30-Aug-2005 ; run some calculations to pad HalfPack with the current BPP
     * and output of PIE
     */
    pie_xy_pad = 0;

    /* davep 07-Sep-2005 ; add in some debugging pad; MUST BE DMA ALIGNED 
     * (Note: if we're scaling, this could get changed and/or ignored)
     */
//    pie_xy_pad += 32;

    /* get PIE's scaling factor so we can figure out how many rows we expect */
    pipe_get_pie_scale( sv, &scale );

    /* davep 01-Mar-2013 ; consolidate pic pixels calculations. Calculate this
     * now so we know how many pixels will hit PIE
     */
    memset( &pic_ppr, 0, sizeof(struct pic_pixels_per_row) );
    pic_ppr.use_lrmargins0 = true;
    pic_ppr.use_chipgap = false; /* TODO add run-time choice of chipgap */
    pic_ppr.use_hscale = sv->use_pic_scale;
    scerr = pic_calc_pixels_per_row( sv, &pic_ppr, scan_pixels_per_row );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* determine PIE's output rows and columns based on the current settings */
    pie_xy_calc_row( pic_ppr.pic_pixels_per_row_out, 
                     pie_pixel_size,
                     sv->pieout, 
                     &scale, 
                     pie_xy_pad,
                     sv->xy_scaler_mode,
                     &pie_row );

    /* If we're too small for the printer to support, pad our way up again.
     * If we're too big, truncate.
     *
     * XXX - Only check if we're actually trying to print? But for now if I'm
     * sending out the network, I want to clip exactly the same way so I can
     * see exactly what I'd be sending to the printer.
     */
    get_hp_minmax_pixels( sv->dpi, &hp_min_pixels, &hp_max_pixels );

    /* davep 16-Apr-2006 ; (Bug 1158) support pushing the width clear across
     * the page via the OUTX register so we can fit scaled down copies in the
     * upper left
     */
    /* davep 16-May-2013 ; pie_pad_to_page_width replaced by
     * pie_pad_to_pixel_width  
     */
#if 0
    if( sv->pie_pad_to_page_width && pie_row.pixels_out < hp_max_pixels ) {
        pie_xy_pad = hp_max_pixels - pie_row.pixels_out;
        dbg1( "padding output of %d by %d to fit scaled into upper left\n",
                    pie_row.pixels_out, pie_xy_pad );

        /* run it again given the new OUTX padding */
        pie_xy_calc_row( pie_row.pixels_in, 
                      pie_pixel_size,
                      sv->pieout, 
                      &scale, 
                      pie_xy_pad,
                      sv->xy_scaler_mode,
                      &pie_row );
    }
#endif

    /* davep 18-Jan-2006 ; calculate based on predicted pie input */
    pie_xy_calc_expected_rows( pie_expected_rows, scale.y_scale, 
                            sv->xy_scaler_mode, &pie_total_rows_out );

    dbg2( "PIE row: pin=%d pout=%d bin=%d bout=%d\n", pie_row.pixels_in,
                pie_row.pixels_out, pie_row.bytes_in, pie_row.bytes_out );

    dbg2( "pie_expected_rows=%d pie_total_rows_out=%d\n", 
                pie_expected_rows,
                pie_total_rows_out );

    /* calculate how we have to adjust PIE to fit HalfPack's requirements */
    hp_total_rows = pie_total_rows_out;

    dbg2( "PIEHP total_rows=%d\n", hp_total_rows );
    
    /* calculate how wide HalfPack's output needs to be */
    piehp_calc_row( pie_row.pixels_out, sv->hp_bpp, sv->use_hp_horiz_rep, &piehp_row );

    dbg2( "PIE row: pin=%d pout=%d bin=%d bout=%d\n", pie_row.pixels_in,
                pie_row.pixels_out, pie_row.bytes_in, pie_row.bytes_out );
    dbg2( "PIEHP row: pin=%d pout=%d bin=%d bout=%d\n", piehp_row.pixels_in,
                piehp_row.pixels_out, piehp_row.bytes_in, piehp_row.bytes_out);

    /* davep ; XXX having to divide/multiply the pixel counts here by
     * two if horizontal replication is really ugly.  Need to put these
     * min/max values into a table somewhere.
     */

    if( piehp_row.pixels_out < hp_min_pixels ) {

        if( sv->use_hp_horiz_rep ) {
            piehp_calc_row( hp_min_pixels/2, sv->hp_bpp, sv->use_hp_horiz_rep, &piehp_row );
        }
        else {
            piehp_calc_row( hp_min_pixels, sv->hp_bpp, sv->use_hp_horiz_rep, &piehp_row );
        }

        dbg2( "PIEHP pad row: pin=%d pout=%d bin=%d bout=%d\n", piehp_row.pixels_in,
                    piehp_row.pixels_out, piehp_row.bytes_in, piehp_row.bytes_out);
    }
    else if( piehp_row.pixels_out > hp_max_pixels ) {

        if( sv->use_hp_horiz_rep ) {
            piehp_calc_row( hp_max_pixels/2, sv->hp_bpp, sv->use_hp_horiz_rep, &piehp_row );
        }
        else {
            piehp_calc_row( hp_max_pixels, sv->hp_bpp, sv->use_hp_horiz_rep, &piehp_row );
        }

        dbg2( "PIEHP trunc row: pin=%d pout=%d bin=%d bout=%d\n", piehp_row.pixels_in,
                    piehp_row.pixels_out, piehp_row.bytes_in, piehp_row.bytes_out);
    }

    /* If the PIE output is smaller than HalfPack's input needs, we will use
     * PIE's XYScale block to give us some pad pixels.
     */
    if( pie_row.pixels_out != piehp_row.pixels_in ) {

        /* note: pie_xy_pad could very well be negative */
        pie_xy_pad += piehp_row.bytes_in - pie_row.bytes_out;

        dbg2( "adjusting PIE->HalfPack bytes from %d to %d (%d bytes)\n", 
                    pie_row.bytes_out, piehp_row.bytes_in, 
                    piehp_row.bytes_in - pie_row.bytes_out);

        dbg3( "pie_xy_pad=%d\n", pie_xy_pad );

        /* readjust the pie row data to account for extra padding */
        pie_xy_calc_row( pie_row.pixels_in, 
                         pie_pixel_size,
                         sv->pieout,
                         &scale, 
                         pie_xy_pad, 
                         sv->xy_scaler_mode,
                         &pie_row );
        pie_dbg2_row( &pie_row );
    }

    /* check for math mistakes */
    XASSERT( pie_row.pixels_out==piehp_row.pixels_in, pie_row.pixels_out );
    XASSERT( pie_row.bytes_out==piehp_row.bytes_in, pie_row.bytes_out );

    memset( &strip, 0, sizeof(struct pie_strip) );
    strip.use_cs = sv->use_colorshift;
    strip.use_mf = sv->use_mf;
    strip.use_xy = !(sv->xy_scaler_mode==PIE_XY_SCALER_BYPASS && true);

    scerr = pie_calc_stripsize( &strip, scale.y_scale, sv->xy_scaler_mode );
    XASSERT( scerr==SCANERR_NONE, scerr );

    pie_dbg2_scale( &scale );
    pie_dbg2_strip( &strip );

    /* 21-Sep-05 davep; PIE stripsize is fixed across all buffers in a run.
     * Therefore, we have to make sure PIC is sending us exactly the same sized
     * buffers. In order for PIC to send us correctly sized buffers clear
     * across the scan, the number of lines we scan will have to be padded to
     * exactly fit the PIE stripsize. As of this writing,
     * scan_setup()-pager.c will call into scanpipe.c to get the necessary
     * numbers to pad the rows padded. That padded could should be what we're
     * passed as "expected_rows". 
     *
     * 14-Oct-05 ; I'm doing this same assert up in the function that starts
     * just PIC and PIE, pipe_start_pic_and_pie_only().
     */
    XASSERT( scan_expected_rows % strip.rdma_in==0, strip.rdma_in );

    /* davep 14-Oct-2005 ; integrating multifilter into the plumbing */

    /* the xy strip size is the strip size that hits the top of XYScale, the
     * size that comes out of MultiFilter
     */
    if( sv->use_mf ) {
        start_pie_multifilter( strip.mf_in, pie_row.pixels_in );
    }
    
    /* davep 07-Sep-2012 ; we could be doing mono IQ curves in CSC so 
     * call CSC, too (also, someday, color fax?)
     */
    if( sv->use_csc ) {
        /* turn on all three color conversion blocks */
        pie_csc_setup();
    }

    /* davep 20-Apr-2009 ; XXX should this be in pie_start()? I think this
     * should be in PIE start but I don't have time to test it right now.
     */
    if( sv->use_tcns ) {
        start_pie_tcns();
    }

    /* fire up the image processing (ICE) blocks */
    /* davep 20-Sep-2005 ; push in a different rows per buffer just to
     * experiment
     */

    /* davep 13-Mar-2006 ; if using the software filter, we need to make
     * larger PIC buffers. Otherwise, between flipping buffers and the
     * software filter, we will slow us down to the point we have to stop
     * the scan motor.
     *
     * We only have to bump up the size if we're scaling up since we only
     * need to worry about this if we having tiny PIC buffers.
     *
     * Note: only doing this if sw_filter_position is enabled because the
     * software filter is arriving late in the development cycle. I want to
     * disturb as little existing code as possible.
     *
     * Note: PIC rows per buffer MUST be a factor of PIE strip size.
     */
    pic_rpb = strip.rdma_in * 2;
    if( sv->sw_filter_position != SV_SW_FILTER_DISABLED ) {

        if( strip.rdma_in < 6 ) {
            pic_rpb = strip.rdma_in * 8;
        }
        else if( strip.rdma_in < 10 ) {
            pic_rpb = strip.rdma_in * 6;
        }
        else if( strip.rdma_in < 12 ) {
            pic_rpb = strip.rdma_in * 4;
        }
    }

    scerr = pic_start( scan_expected_rows, scan_pixels_per_row, pic_rpb,
                        pstats.picw_num_buffers, &pic_ppr );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* davep 18-Jan-2006 ; (bug 397) feed PIE the number of rows we
     * predicted earlier
     */
    XASSERT( scan_expected_rows>=pie_expected_rows, pie_expected_rows );
    scerr = pie_start( pie_expected_rows, &pie_row, &strip,
            pie_pixel_size, pie_xy_pad, &scale );

    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* HalfPack's input strip size must match PIE's ouptut strip size */
    scerr = hp_start( hp_total_rows, &piehp_row, strip.wdma_out, sv->hp_bpp );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* At this point we know how big the resulting scan will be. Pass it on to
     * ScanMan in a message. Store in a global so we can dump it out in a debug
     * message after the scan is complete.
     */
    if( use_scanman ) {
        /* send along the X and Y dimensions of the upcoming data */
        /* davep 23-Jul-2008 ; TODO send along real pixels_per_row_padded! */
        scanlib_send_scan_size( piehp_row.pixels_out,  /* pixels per row padded */
                        piehp_row.pixels_out,  /* pixels per row */
                        hp_total_rows, sv->hp_bpp );
    }

    if( use_pipecut ) {
        scerr = pipe_cutter_open( 0, piehp_wchannels, num_hp_wchannels);
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
        /* davep 02-Jun-2009 ; if memory fails, use less memory. Duh. But fail
         * hard on unexpected errors.
         */
        XASSERT( scerr==0||scerr==SCANERR_OUT_OF_MEMORY, scerr );

        if ( pipetap==PIPETAP_CUTTER && scan_debug_get_ip() != -1) {
            scan_debug_net_open( scan_debug_get_ip(), 
                                        piehp_wchannels, num_hp_wchannels );
            /* ignore error */
        }

        /* davep 22-Apr-2013 ; pipecut 2.0 */
        pipe_cutter_set_recycler( return_icebuf_to_piehp_wdma );
    }

    g_final_bytes_per_row = piehp_row.bytes_out;
    g_final_total_rows = hp_total_rows;

    pic_interrupt_enable();
    pie_interrupt_enable();
    INT_DISABLE_HALFPACK;
    /* davep 28-Nov-2007 ; note: INT_DISABLE_HALFPACK is intDisable(INTNUM_PIE)
     * sometimes (earliest ASICs) so we'e enabling it twice here. No harm in
     * this case but something to be aware of.
     */

    /* success! */
    return SCANERR_NONE;
}

/**
 * \brief set up the ICE DMA plumbing and PIE/HalfPack data tables
 *
 * \author Scott Lloyd, David Poole, Brad Smith
 * \date ??-2005
 * 
 * \param pixels_per_row  number of pixels per row (duh)
 * \param scan_expected_rows  number of rows coming out of scan
 * \param pie_expected_rows  number of rows PIE needs to get through all its
 *                           strips
 * \param flags  debugging flags; see PIPE_FLAG_xxx def's in scanpipe.h
 *
 * davep 09-Jan-2006 ; added doxygen header
 *
 * davep 09-Jan-2006 ; added user_rows parameter
 * Added the user_rows parameter because there are cases where the final strip
 * (the last STRIP, not the last BUFFER) being fed to PIE doesn't contain the
 * final row being fed to PIE. I originally assumed that given 'expected_rows'
 * of input, pipe_wait() would run to the point where the last PIE strip would
 * contain the last row. For mathematical reasons I can't quite understand,
 * this isn't the case. Need to preserve the original number of rows we were
 * going to scan before we padded it so we can run the strip predictor clear
 * through and find the last strip PIE expects to see.
 */

scan_err_t
pipe_start( int pixels_per_row, int scan_expected_rows, int pie_expected_rows, uint32_t flags )
{
    scan_err_t scerr;
    bool one_buffer;
    struct scanvars *sv;

    dbg2( "%s ppr=%d ser=%d per=%d flags=0x%x\n", __FUNCTION__, pixels_per_row, 
                scan_expected_rows, pie_expected_rows, flags );

    /* zero pipe statistics so we can check for leaks later */
    memset( &pstats, 0, sizeof(struct pipe_stats) );

    pstats.scan_expected_rows = scan_expected_rows; // save for pipe_wait debug

    sv = scanvar_get();

    /* global flags via scanvars */
    use_scanman = (sv->scan_callback_fn != NULL) ? true : false;
    filter_position = sv->sw_filter_position;

    /* the following global flags are passed in to this function and are used
     * internally for debugging
     */
    use_results = (flags & PIPE_FLAG_LINK_RESULTS) && true; 
    use_pipenet = (flags & PIPE_FLAG_DATA_FROM_NET) && true;
    use_icefile = (flags & PIPE_FLAG_DATA_FROM_ICEFILE) && true;

    /* local flags, still more internal use only stuff (cal uses one_buffer a
     * lot)
     */
    one_buffer = (flags & PIPE_FLAG_ONE_BUFFER) && true;

    /* adf: it's complicated */
    use_pipecut = false;
    pipe_cutter_location = PIPE_CUTTER_LOCATION_NONE;
    if( sv->doc_src!=SCAN_DOCUMENT_SOURCE_FLATBED ) {
        /* all adf => use pipe cutter */
        use_pipecut = true;
        if( !sv->use_pie && !sv->use_hp ) {
            /* PIC only */
            pipe_cutter_location = PIPE_CUTTER_LOCATION_AFTER_PIC;
        }
        else if( !sv->use_hp ) {
            /* PIC and PIE */
            pipe_cutter_location = PIPE_CUTTER_LOCATION_AFTER_PIE;
        }
        else {
            /* PIC, PIE, HalfPack */
            pipe_cutter_location = PIPE_CUTTER_LOCATION_AFTER_PIEHP;
        }
    }

    /* davep 02-Apr-2013 ; Oh, yeah, ADF gets even more complicated. Add
     * support for Duplex scanning via sensors on both sides of paper 
     * (aka, dual scan). 
     *
     * NOTE! SCAN_DOCUMENT_SOURCE_ADF_DUPLEX is reserved for mechanical duplex
     * scanning. 
     */
    use_nsensor = false;
    if( sv->doc_src==SCAN_DOCUMENT_SOURCE_ADF_NSENSOR ) {
        use_nsensor = true;
    }

    /* davep 22-May-2013 ; decide if we're using piemux */
    use_piemux = false;
    if( sv->use_pie && use_nsensor ) {
        use_piemux = true;

        ASSERT( use_pipecut );
    }

    /* davep 21-Nov-2005 ; set buffer numbers in pstats so we can tune each
     * layer for each DPI
     */
    setup_buffer_counts( sv, &pstats );

    pic_interrupt_disable();
    pie_interrupt_disable();
    INT_DISABLE_HALFPACK;
    /* davep 28-Nov-2007 ; note: INT_DISABLE_HALFPACK is intDisable(INTNUM_PIE)
     * sometimes (earliest ASICs) so we'e disabling it twice here. No harm in
     * this case but something to be aware of.
     */

    /* Let's simplify this thing a bit.
     *
     * If we're only using PIC, go to a function that knows we're only using
     * PIC.
     *
     */
    if( !sv->use_pie && !sv->use_hp ) {
        scerr = pipe_start_pic_only( pixels_per_row, scan_expected_rows,
                                     one_buffer );
        return scerr;
    }
    /* if we're using PIC and PIE, go to a function that knows we're only using
     * PIC and PIE.
     */
    if( !sv->use_hp ) {
        scerr = pipe_start_pic_and_pie_only( pixels_per_row, 
                                             scan_expected_rows,
                                             pie_expected_rows );
        return scerr;
    }

    scerr = pipe_start_pic_pie_hp( pixels_per_row, scan_expected_rows,
                                   pie_expected_rows );
    return scerr;
}

void
pipe_done( void )
{
    const struct scanvars *sv;

    sv = scanvar_get();

//    dbg2( "%s\n", __FUNCTION__ );

    /* close the debug net socket if it was open */
    if( pipetap != PIPETAP_NONE ) {
        scan_debug_net_close();
    }

    pic_interrupt_disable();
    pie_interrupt_disable();
    INT_DISABLE_HALFPACK;
    /* davep 28-Nov-2007 ; note: INT_DISABLE_HALFPACK is intDisable(INTNUM_PIE)
     * sometimes (earliest ASICs) so we'e enabling it twice here. No harm in
     * this case but something to be aware of.
     */

    pic_done();
    if( sv->use_fw_margins ) fw_margin_done();
    if (sv->use_pie) pie_done();
    if (sv->use_hp) hp_done();

    /* davep 07-Mar-2006 ; adding software filter */
    if( filter_position != SV_SW_FILTER_DISABLED ) {
        filter_done();
    }

    /* davep 01-Jun-2009 ; adding the pipe cutter */
    if( use_pipecut ) {
        pipe_cutter_close();
    }

    /* davep 22-May-2013 ; adding piemux */
    piemux_close();
}

static void
make_wait_flags( uint32_t *scan_wait, uint32_t *pic_wait, uint32_t *pie_wait, 
                 uint32_t *hp_wait, uint32_t *filter_wait, uint32_t *fw_margin_wait )
{
    int i;

    /* set up our wait conditions with a bitmask we can use to
     * watch for all the channels finishing
     */
    *scan_wait = 1;

    *pic_wait = 0;
    for( i=0 ; i<num_pic_channels ; i++ ) {
        XASSERT( pic_channels[i] < PIC_WDMA_NUM_CHANNELS, pic_channels[i] );
        SET_BIT( *pic_wait, pic_channels[i] );
    }

    *pie_wait = 0;
    for( i=0 ; i<num_pie_rchannels ; i++ ) {
        XASSERT( pie_rchannels[i] < PIE_RDMA_NUM_CHANNELS, pie_rchannels[i] );
        SET_BIT( *pie_wait, pie_rchannels[i] + 8);
    }
    for( i=0 ; i<num_pie_wchannels ; i++ ) {
        XASSERT( pie_wchannels[i] < PIE_WDMA_NUM_CHANNELS, pie_wchannels[i] );
        SET_BIT( *pie_wait, pie_wchannels[i] );
    }

    *hp_wait = 0;
    if ( num_hp_rchannels ) {
        SET_BIT( *hp_wait, 0 + 8);
    }
    if ( num_hp_wchannels ) {
        SET_BIT( *hp_wait, 0);
    }

    /* davep 07-Mar-2006 ; adding software filter */
    *filter_wait = 0;
    for( i=0 ; i<num_filter_channels ; i++ ) {
        XASSERT( filter_channels[i] < ICE_FILTER_NUM_CHANNELS,
                filter_channels[i] );
        SET_BIT( *filter_wait, filter_channels[i] );
    }

    /* davep 17-Mar-2006 ; icefile uses the pic_wait flags to signal end but we
     * need to clear the scan_wait since we won't have ANY scan data coming in
     */
    if( use_icefile ) {
        *scan_wait = 0;
    }

    /* davep 09-Dec-2010 ; adding firmware margins */
    *fw_margin_wait = 0;
    for( i=0 ; i<num_fw_margin_channels; i++ ) {
        /* read channels' bits offset by 8 */
        SET_BIT( *fw_margin_wait, fw_margin_channels[i] + 8);

        SET_BIT( *fw_margin_wait, fw_margin_channels[i]);
    }
}

/**
 * \brief  "Top off" the memory in the final destination buffers of scan data.
 * Should be used to fill the destination write DMA buffers for the final
 * destination of the data.  For example, in a laser copy, we should call this
 * function on HalfPack Write DMA data to replace the HP data buffer we've sent off
 * to the laser printer.
 *
 * \author David Poole, Bradley Smith
 * \date 19-Aug-2005
 *
 */

static scan_err_t
pipe_topoff( const char *name, 
             ice_add_ready_f add_ready, 
             ice_free_empty_f free_empty,
             struct ice_dma_buffer **p_icebuf )
{
    uint8_t *data;
    scan_err_t scerr;

    /*
     *  NOTE: modifies DMA data structures. Make sure proper interrupt (usually
     *  PIE) is disabled before calling this function.
     *
     */

    pipe_dbg2("%s heapfree=%d\n", __FUNCTION__, scanmem_get_free_size() );

    /* davep 21-Jan-2009 ; test/debug code for pause/resume  */
    if( scan_get_config() & SCAN_CONFIG_PIPE_DRAIN ) {
        dbg2( "%s allowing the pipe to drain\n", __FUNCTION__ );

        /* act as though we didn't get a buffer */
        free_empty( (*p_icebuf)->channel, p_icebuf );

        return SCANERR_OUT_OF_MEMORY;
    }

    data = ice_dma_alloc_buffer( (*p_icebuf)->datalen );
    if( data==NULL ) {
        dbg1("%s no memory for DMA buffer topoff channel=%d heapfree=%d\n", 
                    name, (*p_icebuf)->channel, scanmem_get_free_size() );

        free_empty( (*p_icebuf)->channel, p_icebuf );
        scerr = SCANERR_OUT_OF_MEMORY;
    }
    else {
        /* be paranoid about memory leaks */
        XASSERT( (*p_icebuf)->data==NULL, (uint32_t)(*p_icebuf)->data );
        XASSERT( (*p_icebuf)->dma_handle==(dma_addr_t)NULL, (*p_icebuf)->tag );

        PTR_ASSIGN( (*p_icebuf)->data, data );

        /* davep 28-Mar-2013 ; linux kernel memory mapping */
        icebuf_dma_map(*p_icebuf);

        add_ready( (*p_icebuf)->channel, p_icebuf );
        scerr = SCANERR_NONE;
    }

    return scerr;
}


/**
 * \brief  gently cancel the currently running scan. 
 *
 * Do a pipe cancel without touching the motors or the scan block
 *
 * Gather up all outstanding data from the message queue, putting it back onto
 * the ready lists to be freed later. Do not pass any more data anywhere.
 *
 * \author David Poole
 * \date 09-Nov-2005 (original)
 * \date 04-Mar-2007 (remove motor code)
 *
 */

void pipe_cancel( void )
{
    scan_msg_t msg;
    struct scos_mq * msgq;
    struct ice_dma_buffer *icebuf;
    scan_err_t scerr;
    struct scanvars *sv;

    dbg2( "%s canceling scan\n", __FUNCTION__ );

    sv = scanvar_get();
    msgq = scantask_get_msgq();

#ifdef HAVE_SCAN_ICEFILE_SUPPORT
    /* davep 16-Nov-2010 ; ask icefile to cancel via a cancel message. Need to
     * do this first so icefile will stop sending us buffers.
     */
    if( use_icefile ) {
        /* davep 02-Jun-2011 ; TODO  */
//        ASSERT( 0 );
       // icefile_cancel_msg();
    }
#endif

    /* Note I'm not shutting off PIC and PIE interrupts so we can gather up any
     * outstanding data that might bouncing around in the blocks
     */

    /* Drop into message loop gathering up all the buffer messages. send
     * NOTHING on from this point. Just release back to a safe place.
     */
    while( 1 ) {

        /* sleep 1/2 second waiting for messages */
        scerr = scos_mq_receive( msgq, &msg, USEC_PER_SECOND/2 );

        if( scerr==SCANERR_TIMEDOUT ) {

            dbg1("%s no messages; hope and pray we're done\n", __FUNCTION__ );
			            
            /* leave message loop */
            break;
        }

        /* 30-Mar-05 ; at this stage of the game, we're only expecting
         * something from an interrupt
         */

        switch ( msg.msgType ) {
            case SMSG_PIC_WDMA_DATA: 
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("picw msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                pic_interrupt_disable();
                pic_wdma_add_ready( icebuf->channel, &icebuf );
                pic_interrupt_enable();
                break;

            case SMSG_PIE_RDMA_DATA: 
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("pier msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                pie_interrupt_disable();
                piems_rdma_add_ready( icebuf->channel, &icebuf );
                pie_interrupt_enable();
                break;

            case SMSG_PIE_WDMA_DATA: 
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("piew msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                pie_interrupt_disable();
                piems_wdma_add_ready( icebuf->channel, &icebuf );
                pie_interrupt_enable();
                break;

            case SMSG_PIEHP_RDMA_DATA: 
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("hpr  msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                INT_DISABLE_HALFPACK;
                piehp_rdma_add_ready( icebuf->channel, &icebuf );
                INT_ENABLE_HALFPACK;
                break;

            case SMSG_PIEHP_WDMA_DATA: 
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("hpw  msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                INT_DISABLE_HALFPACK;
                piehp_wdma_add_ready( icebuf->channel, &icebuf );
                INT_ENABLE_HALFPACK;
                break;

            case SMSG_ICE_FILTER_DATA :
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                pipe_dbg2("icef msg=%p channel=%d rows=%d data=%p\n", 
                    icebuf, icebuf->channel, icebuf->num_rows, icebuf->data );

                ice_filter_add_ready( icebuf->channel, &icebuf );
                break;

            /* ignore the following messages because they simply tell us an
             * interrupt has occurred. Unless we have data we need to gather
             * up, we don't care about the messages
             */
            case SMSG_PIE_RDMA_INT: 
                pipe_dbg2("pier dma intmsg channel %d\n", msg.param2 );
                break;

            case SMSG_PIE_WDMA_INT: 
                pipe_dbg2("piew dma intmsg channel %d\n", msg.param2 );
                break;

            case SMSG_SCAN_SCMDCOMP: 
                dbg1( "[S_P] %s scan command complete\n", __FUNCTION__ );
                break;

            case SMSG_SCAN_CMDQSTOP :
                dbg1( "[S_P] Error : %s scan command queue empty\n", __FUNCTION__ );
                break;

            case SMSG_SCAN_MECH_STATUS:
                scan_cmdq_mech_status_change((t_ScanMechStatus)msg.param1);
                break;

            case SMSG_SCAN_MECH_FAULT :
                /* We have no idea what's in this message or what it mean. (It's
                 * a message from the mech driver to the mech driver.) Pass
                 * directly down to the mech driver.
                 */
				dbg1( "[S_P] Error : %s (Mech Fault) t_r:%d\n", __FUNCTION__, pstats.pier_tot_rows);
                scerr = smech_fault_msg( &msg );
                /* we're already cancelling so no need to handle error */
                break;

            case SMSG_SCAN_MECH_IOCTL :
                /* davep 24-May-2012 ; adding ioctl handler so mech driver can
                 * communicate with itself.  We have no idea what's in this
                 * message or what it mean. (It's a message from the mech
                 * driver to the mech driver.) Pass directly down to the mech
                 * driver.
                 */
                scerr = smech_ioctl( msg.param1, msg.param2, msg.param3 );
                break;

            case SMSG_AFE_HW_FAILURE : 
                /* davep 13-Jun-2012 ; catch AFE hardware failure; can happen in
                 * interrupt context so ISR will send the thread this message
                 */
                dbg1( "[S_P] Error : %s afe failure param1=0x%x, t_r:%d\n", __FUNCTION__, msg.param1, pstats.pier_tot_rows );
                /* ignore since we're cancelling anyway */
                break;

            case SMSG_SCAN_CANCEL:
                //ignore the cancel messages
                break;

            case SMSG_SCAN_SCMDEND :
                /* Ignore because we're already in the process of canceling so
                 * we don't really care the mech has hit the paper sensor. So
                 * there!
                 */
                /* davep 23-Mar-2012 ; this message is deprecated and should never
                 * be seen. Will be removed ASAP.
                 */
                ASSERT( !"deprecated code! you should never see this!" );
                break;

            case SMSG_SMECH_POLL :
                /* We should shut off the polling before we run a scan but gently
                 * ignore this message anyway. No sense risking a race condition.
                 */
                break;

            case SMSG_SCAN_ADF_PAPER_EVENT : 
                dbg1( "[S_P] %s ignoring adf paper event=%d value=%d, t_r:%d\n", 
                            __FUNCTION__, (scan_adf_paper_event_t)msg.param1,
                            (uint32_t)msg.param2 , pstats.pier_tot_rows);
                break;

            /* davep 05-Oct-2011 ; adding firmware margin cancel */
            case SMSG_FW_READ_MARGIN_DATA :
                ASSERT( sv->use_fw_margins );
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                /* assume fwmargins hardwired between PIC->PIE; send it back to PIC */
                pic_interrupt_disable();
                pic_wdma_add_ready( icebuf->channel, &icebuf );
                pic_interrupt_enable();
                break;

            case SMSG_FW_WRITE_MARGIN_DATA :
                ASSERT( sv->use_fw_margins );
                PTR_ASSIGN( icebuf, msg.param3 );
                icebuf_sanity( icebuf );

                /* pass straight to PIE (margins use PIC's channel numbers) */
                ASSERT( sv->use_pie );
                pie_interrupt_disable();
                piems_rdma_add_ready( pipemap_pic_to_pie_channel( icebuf ), &icebuf );
                pie_interrupt_enable();
                break;

            case SMSG_PIC_ESD:
            case SMSG_PIC_ESD_FLUSH:
                /* davep 24-Jul-2012 ; ignore stray ESD messages. Comes from an
                 * ISR. Contains nothing to be concerned about during a cancel.
                 */
                dbg2( "%s ignore ESD event\n", __FUNCTION__ );
                break;

            default:
                /* freak out on unknown messages so we can make sure to write
                 * cancel code in the future
                 */
                XASSERT( 0, msg.msgType );
                break;
        }
    }

    /* shut off PIC and PIE interrupts so we can prevent any remote possibility
     * more data might arrive
     */
    pic_interrupt_disable();
    pie_interrupt_disable();
    INT_DISABLE_HALFPACK;

    /* cancel any running data */
    pic_wdma_cancel();

    /* davep 08-Dec-2010 ; adding firmware margins */
    if( sv->use_fw_margins ) {
        fw_rmargin_cancel();
        fw_wmargin_cancel();
    }

    if( sv->use_pie ) {
        piems_rdma_cancel();
        piems_wdma_cancel();
    }

    /* davep 07-Mar-2006 ; adding software filter */
    if( filter_position != SV_SW_FILTER_DISABLED ) {
        ice_filter_cancel();
    }

    if( sv->use_hp ) {
        piehp_rdma_cancel();
        piehp_wdma_cancel();
    }

    if( use_pipecut ) {
        /* hmmm... interesting my pipecutter doesn't have a cancel method */
        pipe_cutter_close();
    }

    /* davep 22-May-2013 ; adding piemux */
    piemux_cancel();
    
    /* release anything that may be in our pipe_results list */
    ice_free_list( &pipe_results );
}

/**
 * \brief queue up a buffer from PIE for HalfPack
 *
 * Written to be called from pipe_wait(). Assumes the channel number is a PIE
 * channel number since we will map that channel number to a HalfPack channel.
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 */

static void
send_pie_icebuf_to_hp( struct ice_dma_buffer **pp_icebuf )
{
    uint8_t channel;
    struct ice_dma_buffer *icebuf;

    PTR_ASSIGN( icebuf, *pp_icebuf );

    INT_DISABLE_HALFPACK;

    /* if we're using HalfPack, we must be running in mono so we'd better
     * only have the one channel coming out of PIE
     */
    /* davep 07-Mar-2006 ; this still applies to the software filter since the
     * software filter uses PIE's channels
     */
    XASSERT( icebuf->channel==PIE_WDMA_CHANNEL_MONO, icebuf->channel );

    /* use a temporary because icebuf is NULL after calls to _add_ready() */
    channel = pipemap_pie_to_piehp_channel( icebuf );

    /* add to HalfPack */
    piehp_rdma_add_ready( channel, &icebuf );
    piehp_rdma_channel_launch( channel );

    INT_ENABLE_HALFPACK;
}

/**
 * \brief send a chunk of completed PIE data to scanman
 *
 * Builds up the message necessary to send data to ScanMan. Written to be
 * called from pipe_wait() in the case of PIE Write DMA data or Filter data
 * that needs to be sent on to scanman (e.g., scan-to-host).
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 * davep 17-Oct-2006 ; added PIC data for cal-to-host
 * 
 */

static void send_data_to_scanman( uint8_t page_side, struct ice_dma_buffer *icebuf, bool last_buffer )
{
    /* davep 08-Apr-2013 ; adding scan_data_type to icebuf itself (should have
     * done this long, long ago). This assert will catch code not yet updated
     * with the dtype.
     */
    XASSERT( icebuf->dtype > 0, icebuf->tag );

    /* davep 14-Aug-2012 ; make sure we are 100% cpu accessible */
    icebuf_dma_unmap( icebuf );

    scanlib_send_page_data( &icebuf->data, icebuf->dtype,
            icebuf->num_rows, icebuf->max_rows, 
            page_side, last_buffer?true:false );

    /* icebuf->data will be NULL here */
    XASSERT( icebuf->data==NULL, (uint32_t)icebuf->data );
    XASSERT( icebuf->dma_handle==(dma_addr_t)NULL, (uint32_t)icebuf->data );
}

/**
 * \brief return a naked (i.e., no data attached) icebuf to PIE Write DMA
 *
 * Written to be called from pipe_wait() to handle PIE Write DMA and Filter
 * write data when the two of them are sending data to ScanMan.
 *
 * When PIE Write or Filter finishes with a chunk of data and HalfPack isn't
 * involved, the data is detached from the icebuf and sent to ScanMan. The now
 * naked icebuf is sent back to PIE via pipe_topoff().
 *
 * \author David Poole
 * \date 07-Mar-2006
 *
 * davep 22-Apr-2013 ; updating for pipecutter 2.0; now must handle an icebuf
 *                     that might still contain data
 */

static void return_icebuf_to_pie_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf )
{
    struct ice_dma_buffer *icebuf;

    /* use a convenience pointer */
    PTR_ASSIGN( icebuf, *pp_icebuf );

    /* Send the icebuf header back to PIE write DMA. pipe_topoff() will try to
     * allocate a new data buffer from the heap. If we get a buffer, the data
     * will be attached to the now free icebuf and put back into PIE's ready
     * list with _add_ready(). If we can't get a buffer, the empty icebuf will
     * be put back on the free list with _free_empty().
     */

    pie_interrupt_disable();

//    dbg2( "%s ch=%d data=%p\n", __FUNCTION__, channel, icebuf->data );

    if( icebuf->data ) {
        piems_wdma_add_ready( channel, &icebuf );
    }
    else {
        /* should NOT have any data attached! this function designed to put a naked
         * icebuf back to PIE; the icebuf's data should have been sent elsewhere
         * (e.g., scanman)
         */
        XASSERT( icebuf->data==NULL, (uint32_t)icebuf );
        XASSERT( icebuf->dma_handle==(dma_addr_t)NULL, (uint32_t)icebuf->dma_handle );

        pipe_topoff( "pie", piems_wdma_add_ready, 
                     piems_wdma_free_empty,
                     &icebuf );
    }

    piems_wdma_channel_launch( channel );
    pie_interrupt_enable();
}

static void return_icebuf_to_piehp_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf )
{
    struct ice_dma_buffer *icebuf;

    /* use a convenience pointer */
    PTR_ASSIGN( icebuf, *pp_icebuf );

    /* Send it back to HalfPack write DMA. pipe_topoff() will try to
     * allocate a new data buffer from the heap. If we get a
     * buffer, the data will be attached to the now free icebuf and
     * put back into HalfPack's ready list with _add_ready(). If we can't
     * get a buffer, the empty icebuf will be put back on the free
     * list with _free_empty().
     */

    INT_DISABLE_HALFPACK;

    if( icebuf->data ) {
        piehp_wdma_add_ready( channel, &icebuf );
    }
    else {
        pipe_topoff( "piehp", piehp_wdma_add_ready, 
                     piehp_wdma_free_empty,
                     &icebuf );
    }

    piehp_wdma_channel_launch( channel );
    INT_ENABLE_HALFPACK;
}

static void return_icebuf_to_pic_wdma( uint8_t channel, struct ice_dma_buffer **pp_icebuf )
{
    scan_err_t scerr;
    uint8_t pic_channel;
    struct ice_dma_buffer *icebuf;
    bool update_row_count;

    /* use a convenience pointer */
    PTR_ASSIGN( icebuf, *pp_icebuf );

//    dbg2( "%s ch=%d data=%p\n", __FUNCTION__, icebuf->channel, icebuf->data );

    /* PIC is the most complex case because we have to update the scan cmdq row
     * counters at the same time
     */

    /* davep 02-Jul-2013 ; caller required to remap the channel before we get
     * here
     */
    pic_channel = channel;

    XASSERT( icebuf->max_rows==pstats.scan_cmdq_rows, icebuf->max_rows );

    /* replace the buffer we just sent downstream */
    pic_interrupt_disable();

    update_row_count = false;
    if( icebuf->data ) {
        pic_wdma_add_ready( pic_channel, &icebuf );
        update_row_count = true;
    }
    else {
        scerr = pipe_topoff( "pic", pic_wdma_add_ready, 
                     pic_wdma_free_empty,
                     &icebuf );
        if( scerr==SCANERR_NONE ) {
            update_row_count = true;
        }
    }
    XASSERT( icebuf==NULL, icebuf->tag );

    pic_wdma_channel_launch( pic_channel );
    pic_interrupt_enable();
    /* if we were able to get memory, scan some more lines */
    /* davep 06-Jun-2011 ; adding icetest+icefile */
    if( update_row_count && !use_icefile ) {
        bool was_scan_enabled;
        was_scan_enabled = scan_interrupt_disable();
        scan_cmdq_channel_add_rows( pic_channel, 
                                   pstats.scan_cmdq_rows );
        if (was_scan_enabled) {
            scan_interrupt_enable();
        }
    }
}


/**
 * \brief  Send a copy of an icebuf's data to scanman
 *
 * Created to support cal-to-host. Calibration data is needed cal so we can't
 * simply send it off to scanman. We make a copy of the data to send to scanman
 * instead.
 *
 * \author David Poole
 * \date 12-Oct-2006
 *
 */

static scan_err_t
dup_data_to_scanman( struct ice_dma_buffer *icebuf, bool last_buffer )
{
    uint8_t *copy_of_data;
    int datalen;
    int num_rows;
    struct ice_dma_buffer tmp_icebuf;

    /* davep 22-Jun-2010 ; no more free pass with CACHE_BYPASS :-( */
    cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );

    /* quiet compiler */
    datalen = 0;

    /* davep 29-May-2009 ; adding a little recovery ability. If we can't get the
     * buffer, try sending less until success or we're completely hosed.
     * Originally created for MFPs with tiny memory and 1200 DPI color cal
     * graphs.
     */
    num_rows = icebuf->num_rows;
    copy_of_data=NULL;
    while( num_rows > 0 && copy_of_data==NULL ) {

        datalen = num_rows * icebuf->bytes_per_row;

        /* since we're sending to scanman, we need to have a data (not a scratch)
         * buffer
         */
        copy_of_data = ice_dma_alloc_buffer( datalen );

        if( copy_of_data==NULL ) {
            /* too bad, so sad */
            dbg2("%s unable to clone buffer num_rows=%d datalen=%d freemem=%d\n", 
                        __FUNCTION__, icebuf->num_rows, icebuf->datalen,
                        scanmem_get_free_size() );

            /* davep 29-May-2009 ;  have to be very careful about shrinking the
             * buffer so we don't break the ICE Lite BRG nonsense. No memory is
             * a corner case so optimize for the most general solution.  By
             * subtracting six rows, I'm shrinking the buffer by three lines of
             * color and also shrinks mono.  (Originally I was dividing by two
             * but that broke ICE Lite data.)
             *
             * As of this writing, incoming buffer sizes are usually 45 rows or
             * some other modulo 15.
             */
            num_rows -= 6;
        }
    }

    if( copy_of_data==NULL ) {
        /* too bad, so sad */
        dbg2("%s giving up; unable to clone buffer num_rows=%d datalen=%d freemem=%d\n", 
                    __FUNCTION__, icebuf->num_rows, icebuf->datalen,
                    scanmem_get_free_size() );
        return SCANERR_OUT_OF_MEMORY;
    }

    copy_of_data = CACHE_BYPASS_CLEAR(copy_of_data);

    /* eek! memcpy()! */
    memcpy( copy_of_data, icebuf->data, datalen );

    /* flush our memcpy data from cache into main memory */
    cpu_dcache_writeback_region( copy_of_data, datalen );
    copy_of_data = CACHE_BYPASS_SET(copy_of_data);

    /* This is a bit of a kludge; send_data_to_scanman() wants an icebuf so make
     * a carbon copy of our original
     */
    memcpy( &tmp_icebuf, icebuf, sizeof(struct ice_dma_buffer) );
    PTR_ASSIGN( tmp_icebuf.data, copy_of_data );
    tmp_icebuf.num_rows = num_rows;
    tmp_icebuf.max_rows = num_rows;
    tmp_icebuf.datalen = datalen;

    send_data_to_scanman( icebuf->sensor_num, &tmp_icebuf, last_buffer );

    return SCANERR_NONE;
}

static scan_err_t scanpipe_pic_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                                            uint32_t rows, uint32_t bytes_per_row )
{
    scan_err_t scerr;
    bool was_scan_enabled;

//    dbg2( "%s ch=%d rows=%d scan_cmdq_rows=%d\n", __FUNCTION__, channel, rows,
//            pstats.scan_cmdq_rows );

    scerr = pic_wdma_add_buffer( channel, data, datalen, rows, bytes_per_row );
    if( scerr != SCANERR_NONE ) {
        XASSERT(0,scerr);
        return scerr;
    }
    pic_wdma_channel_launch(channel);

    /* davep 06-Jun-2011 ; adding icetest+icefile */
    if( !use_icefile ) {
        was_scan_enabled = scan_interrupt_disable();
        scan_cmdq_channel_add_rows( channel, 
                                   pstats.scan_cmdq_rows );
        if (was_scan_enabled) {
            scan_interrupt_enable();
        }
    }

    return SCANERR_NONE;
}

static void pipecut_page_state_init( void )
{
    memset( &pipecut_page_state_list, 0, sizeof(pipecut_page_state_list) );

    pipe_cutter_get_num_channels( 0, &pipecut_page_state_list[0].num_channels );
    pipe_cutter_get_num_channels( 1, &pipecut_page_state_list[1].num_channels );
}

scan_err_t pipecut_callback_send_to_scanman( uint8_t page_side, 
                                struct ice_dma_buffer *icebuf, 
                                bool last_buffer )
{
    struct pipecut_page_state *page_state;

    /* DO NOT MODIFY FIELDS OF ICEBUF! ONLY take the data pointer. The rest of
     * the icebuf has been passed to indicate the nature of the data. The
     * icebuf belongs to pipe_cutter.
     */

//    dbg2( "base_row=%d ch=%d dtype=%d last_buffer=%d\n",
//                icebuf->base_row, icebuf->channel, dtype, last_buffer );
//    dbg2( "%s base_row=%d data=%p ud=%d dtype=%d last_buffer=%d\n",
//                __FUNCTION__, icebuf->base_row, icebuf->data, user_data, dtype,
//                last_buffer );

    XASSERT( icebuf->channel<MAX_PIPE_CUTTER_CHANNELS, icebuf->channel );

    XASSERT( page_side<=1, page_side );
    page_state = &pipecut_page_state_list[page_side];

    /* The following 'if' then 'send_data' then 'if' again looks a little funny.
     * But need to (possibly) send the pagestart before we send the data. And
     * also need to send the data before we send the pageend.
     */ 
    if( ! page_state->is_channel_in_page[icebuf->channel] ) {
        ASSERT( !last_buffer );

        page_state->num_channels_in_page += 1;
        XASSERT( page_state->num_channels_in_page <= page_state->num_channels, 
                icebuf->channel );

        /* first buffer of a new page! */
        /* davep 03-Feb-2011 ; the ==1 is not 100% reliable due to planar
         * fields possible arriving /very/ out-of-order. This will fail if we
         * ever get multiples of a plane before receiving the other planes.
         * R,G,B -> ok
         * G,B,R -> ok (and other permutations thereof)
         * R,R,G,G,B -> FAIL!  (shouldn't happen but...)
         * */
        if( use_scanman && page_state->num_channels_in_page==1 ) {
            scanlib_send_page_start(page_side);
            pstats.page_starts++;
            dbg1( "[S_P] Send SMSG_PAGE_START (Top), %d, t_r:%d\n", page_side, pstats.pier_tot_rows);
        }

        page_state->is_channel_in_page[icebuf->channel] = true;
    }

    if( pipetap==PIPETAP_CUTTER ) {
        scan_debug_net_send( icebuf->channel, icebuf->data,
                     icebuf->num_rows * icebuf->bytes_per_row );
        /* ignore error */
    }

    send_data_to_scanman( page_side, icebuf, last_buffer );

    if( last_buffer ) {
        XASSERT( page_state->is_channel_in_page[icebuf->channel], icebuf->channel );

        page_state->num_channels_in_page -= 1;
        XASSERT( page_state->num_channels_in_page >=0, icebuf->channel );

        /* last buffer in a page; send the page end downstream */
        if( use_scanman && page_state->num_channels_in_page==0 ) {
            scanlib_send_page_end(page_side);
            pstats.page_ends++;
			dbg1( "[S_P] Send SMSG_PAGE_END (Bottom), %d, t_r:%d\n", page_side, pstats.pier_tot_rows);
        }

        page_state->is_channel_in_page[icebuf->channel] = false;
    }

    return SCANERR_NONE;
}

//=========================================================================================
void background_init(void)
{
	struct scanvars *sv; 
	
	sv = scanvar_get();
	
	dpi_factor_h = (int)sv->hw_dpi_horiz/300;	// 300dpi : 1, 600dpi : 2
	dpi_factor_v = (int)sv->hw_dpi_vert/300;	// 300dpi : 1, 600dpi : 2
	
	/* Backup Original LUT */
	memcpy( p_bgr_lut_org, sv->csc_r_lut.data, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE) );
	
	/* Debug Code */
	{
	    struct file *filp1;
	    struct file *filp2;
	    	    
	    /* kernel memory access setting */
	    mm_segment_t old_fs = get_fs();
	    
	    set_fs(get_ds());
	 
	    /* open a file */
	    //filp1 = filp_open("/data/bg_enable", O_CREAT|O_RDWR, 0);
	    filp1 = filp_open("/data/bg_disable", O_RDWR, 0);
	    filp2 = filp_open("/data/bg_log", O_RDWR, 0);

	    if (IS_ERR(filp1)) {
	        bg_enable = true;
	    }
	    else {
	    	bg_enable = false;
			filp_close(filp1, NULL);
	    }

	    if (IS_ERR(filp2)) {
	        bg_log_enable = false;
	    }
	    else {
	    	bg_log_enable = true;
	    	filp_close(filp2, NULL);
	    }
		
	    dbg1("[BG] BG Init Complete (id:%d, b_s:%d, d_f_h:%d, d_f_v:%d, enable:%d, log:%d)\n", 
			sv->id, sv->bgr_sensitivity, dpi_factor_h, dpi_factor_v, bg_enable, bg_log_enable);
		
	    /* restore kernel memory setting */
	    set_fs(old_fs);
	}
}

void background_open(void)
{
	dbg1( "[BG] BGR Open!!\n");
	
	current_rows = 0;
	
	memset( histogram, 0, 256*sizeof(uint32_t) );
}

void background_close(void)
{
	struct scanvars *sv; 
	
	dbg1( "[BG] BGR Close!!\n");
	
	sv = scanvar_get();
	
	bgr_detecting = false;
	pie_interrupt_enable_unlock();
	
	/* Dump LUT into file */
	{
	    struct file *filp1;
	    uint8_t *data;
	    int i;
	    	    
	    /* kernel memory access setting */
	    mm_segment_t old_fs = get_fs();
	    
	    set_fs(get_ds());
	 
	    /* open a file */
	    filp1 = filp_open("/data/bg_lut_dump", O_RDWR, 0);
	    
		/* Dump CSC LUT */
	    if (IS_ERR(filp1)) {
	        ;
	    }
	    else {
#if 0
			/* Memory Alloc */
			data = scanmem_alloc_aligned(sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE));
			if( data == NULL ) {
				dbg1("[BG] Error : Dump Mem Alloc Fail!!\n");
				filp_close(filp1, NULL);
				set_fs(old_fs);
        		return;
    		}
    		
			/* Copy to Local */
			memcpy( data, p_bgr_lut_org, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE) );
			
			/* Read example */
			//vfs_read(file, data, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE-1), &filp1->f_pos);

			/* write example */
			vfs_write(filp1, data, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE), &filp1->f_pos);
			
			/* Memory Free */
			memFree(data);
#else
			dbg1("[BG] CSC LUT Dump (Org, New)\n");
			
			for( i=0 ; i<sv->csc_r_lut.num_entries ; i++ ) {
				dbg1("%d	%d\n", p_bgr_lut_org[i], sv->csc_r_lut.data[i]);
			}
#endif

			filp_close(filp1, NULL);
	    }

	    /* restore kernel memory setting */
	    set_fs(old_fs);
	}
}

void background_detect( struct ice_dma_buffer *icebuf, uint32_t valid_rows)
{
	uint32_t ma;					// moving avg with neighbor pixels in histogram
	int i, j, k;
	uint8_t *pData;
	struct scanvars *sv; 
	uint32_t target_index =0;
	
	if(icebuf->channel == 0 && current_rows <= (BG_REM_TOP_OFFSET + BG_REM_HEIGHT) )
	{
		/***** Initialize (every page) *****/
		if(current_rows == 0){
			dbg1( "[BG] BGR Start!! ( ch:%d, c_r:%d, v_r:%d, rpb:%d, bpr:%d, [h:%d, t_o:%d, l_o:%d, r_o:%d] )\n", 
					icebuf->channel, current_rows, valid_rows, icebuf->num_rows, icebuf->bytes_per_row,
					BG_REM_HEIGHT, BG_REM_TOP_OFFSET, BG_REM_LEFT_OFFSET, BG_REM_RIGHT_OFFSET);
			bgr_detecting = true;
			
			pie_interrupt_disable();
			pie_interrupt_enable_lock();
#if 0
			scanimg_stop();
			dbg1( "[BG] LED ALL OFF!!\n");
#endif	
		}

		/***** Detecting *****/
		for( i=0; i<icebuf->num_rows; i++)
		{
			/***** Update Line Pointer *****/
			pData = icebuf->data + i*icebuf->bytes_per_row;
			
			/***** Skip Invalid Rows  *****/
			if( i < icebuf->num_rows - valid_rows )
				continue;
			
			/***** Skip Top Area *****/
			if(current_rows < BG_REM_TOP_OFFSET){
				XASSERT( BG_REM_HEIGHT > BG_REM_TOP_OFFSET, BG_REM_TOP_OFFSET-BG_REM_HEIGHT );
				current_rows++;
				continue;
			}
			/***** Detecting Area *****/
			else if(current_rows < BG_REM_TOP_OFFSET + BG_REM_HEIGHT){
				
				/* One Line Histogram */
				for(j=BG_REM_LEFT_OFFSET; j<icebuf->bytes_per_row-BG_REM_RIGHT_OFFSET; j++){
					histogram[pData[j]]++;
					
					/* Dump Line Image */
					if(bg_log_enable == true){
        				if( (current_rows == BG_REM_TOP_OFFSET && j < BG_REM_LEFT_OFFSET+300) || 
							(current_rows == BG_REM_TOP_OFFSET+BG_REM_HEIGHT-1 && j < BG_REM_LEFT_OFFSET+300) ){
					        dbg2( "%d	%d\n", j, pData[j]);
        				}
					}
						
					/* Check Saturation */
					if(histogram[pData[j]] == 0xffffffff)
						dbg1( "[BG] Error : Histogram Saturated (%d, %d, %d)\n", pData[j], histogram[pData[j]], current_rows);
				}
				
				/* Marking Detecting Area */
				if( bg_log_enable == true && ((current_rows == BG_REM_TOP_OFFSET+1) || 
					(current_rows == BG_REM_TOP_OFFSET+BG_REM_HEIGHT-1)) ){

		        	memset( pData+BG_REM_LEFT_OFFSET, 0x00, icebuf->bytes_per_row-2*BG_REM_LEFT_OFFSET );
		        	cpu_dcache_writeback_region( pData, icebuf->bytes_per_row );
				}
				
				current_rows++;
			}
			/***** Detecting Peak  *****/
			else if(current_rows == BG_REM_TOP_OFFSET + BG_REM_HEIGHT){

				peak_lvl = 0;
				peak_index = 255;
				
				for(k=0; k<256; k++){
					
					if(k >= BG_REM_LIMIT_B){
						/* Moving Avg */
						ma = (histogram[k]+histogram[k-1]+histogram[k-2])/3;					// moving avg with neighbor pixels in histogram
						
						/* Check Peak */
						if(peak_lvl < ma){	// moving avg size : 3 pixel
							peak_lvl = histogram[k];
							peak_index = k;
						}
					}						
				}
				
				/* Update CSC LUT */
				sv = scanvar_get();
				
				if( bg_enable == true && sv->use_csc_r_lut ){
#if BG_REM_ADVANCED
					target_index = MAX(0, MIN(255, peak_index + BG_REM_COMPENSATE*(5-sv->bgr_sensitivity)));	// bgr_sensitivity : 1 ~ 9, 5 : center
#else
					target_index = MIN(255, peak_index + BG_REM_COMPENSATE*(10-sv->bgr_sensitivity));	// bgr_sensitivity : 1 ~ 9, 5 : center
#endif
					*PIE_CSC_RGB_CCR = PIE_CSC_RGB_CCR_CPU;
					
					scanvar_set_csc_rgb_lut( p_bgr_lut_org, &sv->csc_r_lut, target_index);
					pie_csc_rgb_set_lut( sv->csc_r_lut.data, PIE_CSC_RED_LUT );
					
					*PIE_CSC_RGB_CCR = 0;
				}
				
				dbg1( "[BG] BGR Complete!!, peak(sv->id:%d, p_i:%d, t_i:%d, cnt:%d, c_r:%d)\n", sv->id, peak_index, target_index,
					 peak_lvl, current_rows);
					 
				bgr_detecting = false;
				pie_interrupt_enable_unlock();
				pie_interrupt_enable();
				
				current_rows++;
				
				break;
			}
		}
	}
}
//=========================================================================================

/**
 * \brief  calculate the next pipecutter marker 
 *
 * Very complicated because duplex requires separate margins.
 *
 * \author David Poole
 * \date 29-Nov-2011
 */

static void calc_marker_row( uint32_t pic_rows_scanned,
                             struct adf_paper_path *adfpath, 
                             uint32_t *marker_row_out
                           )
{
    struct scan_adf_margins *adf_margins = &adfpath->adf_margins;
    uint32_t marker_row = 0;

    marker_row = pic_rows_scanned + adfpath->row_counter;

    /* davep 19-Sep-2011 ; backside of duplex is staged differently than the
     * front side so need separate field for the back
     */

    if( ! adfpath->in_page ) {
        /* paper is leaving -> BOF approaches */

        if( adfpath->use_duplex && adfpath->duplex_page_side==2 ) {
            marker_row += adf_margins->rows_to_bof_backside;
        }
        else {
            marker_row += adf_margins->rows_to_bof;
        }
    }
    else {
        /* paper is arriving -> TOF approaches */

        if( adfpath->use_duplex && adfpath->duplex_page_side==2 ) {
            marker_row += adf_margins->rows_to_tof_backside;
        }
        else {
            marker_row += adf_margins->rows_to_tof;
        }
    }

    *marker_row_out = marker_row;
}

/**
 * \brief  Handle an ADF paper event message.
 *
 * Also tries to absorb the extra mechanical debounce.
 *
 * \author David Poole
 * \date 27-Aug-2009
 *
 */

static void adf_paper_event( scan_adf_paper_event_t adfevt,
                             uint32_t adfevt_flag_value,
                             uint32_t pic_dwcsr,
                             struct adf_paper_path *adfpath )
{
    bool previous_flag_state;
    uint32_t pic_rows_scanned;
    uint32_t marker_row, marker_row_scaled;

    /* davep 27-Aug-2009 ; XXX temp debug */
    dbg2( "%s event=%d value=%d pic=%d\n", __FUNCTION__, adfevt, adfevt_flag_value, pic_dwcsr );

    /* davep 21-Oct-2009 ; pic_dwcsr == PIC DMA Write Count Status Register */
    pic_rows_scanned = pstats.picw_rows_per_buffer - 
                      ((pic_dwcsr + pstats.picw_bytes_per_row - 1) / pstats.picw_bytes_per_row);

    dbg2( "%s rpb=%d bpr=%d scanned=%d\n", __FUNCTION__,
                pstats.picw_rows_per_buffer, pstats.picw_bytes_per_row,
                pic_rows_scanned );

    /* davep 22-Oct-2009 ; this is such new code, I put in safety. I'm worried
     * about unsigned integer underflow 
     */
//    if( pic_rows_scanned > 69 ) {
//        dbg2( "%s scanned=%d too big; safety switch engaged scanned=0\n", 
//                    __FUNCTION__, pic_rows_scanned );
//        pic_rows_scanned = 0;
//    }

    previous_flag_state = adfpath->in_page ;

    if( adfevt_flag_value ) {
        dbg2( "%s adf switch asserted at PIC received rows=%d\n", __FUNCTION__,
                    adfpath->row_counter );
        /* paper is in path */
        adfpath->in_page = true;
#if	BGR_REMOVE
        background_open();
#endif	
    }
    else {
        dbg2( "%s adf switch de-asserted at PIC received rows=%d\n", __FUNCTION__,
                    adfpath->row_counter );
        /* paper is not in path */
        adfpath->in_page = false;
    }

    /* No change from last time so we should not have received this event.
     * Probably a de-bounce issue.
     */
    if( previous_flag_state==adfpath->in_page ) {
        dbg1( "%s ignoring consecutive adf paper event of %d->%d\n", __FUNCTION__,
                    previous_flag_state, adfpath->in_page);
        return;
    }

    dbg1( "[S_P] %s to %s (adf_r=%d), picw_t_r:%d, pier_t_r:%d, piew_t_r:%d\n",
                (previous_flag_state?"TOP_ON":"TOP_OFF"), 
                (adfpath->in_page?"TOP_ON":"TOP_OFF"),
                 adfpath->row_counter,
                 pstats.picw_tot_rows,
                 pstats.pier_tot_rows,
                 pstats.piew_tot_rows);

    if( ! adfpath->in_page ) {
        /* paper is leaving -> BOF approaches */
        calc_marker_row( pic_rows_scanned, adfpath, &marker_row );

        /* davep 18-Jul-2009 ; adding scale */
        marker_row_scaled = safeint_scale_uint32( marker_row,
                                        adfpath->scale.y_numer, adfpath->scale.y_denom );
		
		dbg1("[S_P]Paper Length %d \n", marker_row - adfpath->tof_marker);
			
        /* save where we've seen our newest BOF */
        adfpath->tof_marker = 0;
        adfpath->bof_marker = marker_row;
        
        dbg2( "%s adf bof will be at row=%d\n", __FUNCTION__, marker_row );

        /* drop a marker where we'll cut the final output */
        pipe_cutter_add_marker( marker_row_scaled );
    }
    else {
        /* paper is arriving -> TOF approaches */
        calc_marker_row( pic_rows_scanned, adfpath, &marker_row );

        /* davep 18-Jul-2009 ; adding scale */
        marker_row_scaled = safeint_scale_uint32( marker_row,
                                        adfpath->scale.y_numer, adfpath->scale.y_denom );

		dbg1("[S_P]Paper GAP %d \n",  marker_row - adfpath->bof_marker);
		
        /* save where we've seen our newest TOF */
        adfpath->tof_marker = marker_row;
        adfpath->bof_marker = 0;
			
        dbg2( "%s adf tof will be at row=%d\n", __FUNCTION__, marker_row );

        /* drop a marker where we'll cut the final output */
        pipe_cutter_add_marker( marker_row_scaled );
    }
}

#ifdef HAVE_PIC_PSESD
static void pic_esd_process(uint32_t esd_row_in_page, uint32_t picw_buf_start_row, 
                            struct ice_dma_buffer *icebuf)
{
    uint32_t esd_row;
    uint8_t *p_row;
    uint8_t *p_row2;

    /*calculate the row buffer address that has ESD */
    esd_row = esd_row_in_page - picw_buf_start_row;
    p_row = (uint8_t *)( ((uint32_t)icebuf->data) + (esd_row * icebuf->bytes_per_row) );
    dbg1("ES in buf ch %d row start%d line %d!\n", icebuf->channel, picw_buf_start_row, esd_row);

   /*Use the row before or after for replacement*/
    if(esd_row > 0)
    {
        p_row2 = (uint8_t *)( ((uint32_t)icebuf->data) + ( (esd_row - 1) * icebuf->bytes_per_row) );
    }else
    {
        p_row2 = (uint8_t *)( ((uint32_t)icebuf->data) + ( (esd_row + 1) * icebuf->bytes_per_row) );
    }
    memcpy(p_row, p_row2, icebuf->bytes_per_row);

    //DEBUG: to mark the detection
    //memset(p_row + icebuf->bytes_per_row - 250, 0, 150);

    /*Have to flush the changes */
    cpu_dcache_writeback_region( p_row, icebuf->bytes_per_row );
}

static void pic_esd_flush_buffers( struct ice_dma_buffer *icebuf_holds[3])
{
     scan_msg_t msg;
     scan_err_t scerr;
#define FLUSH_ESD_HOLD_BUF(ch)  do{msg.msgType = SMSG_PIC_WDMA_DATA;\
                                   msg.param3 = icebuf_holds[ch];\
                                   msg.param2 = 0;\
                                   scerr = scantask_msg_send_nowait( &msg );\
                                   icebuf_holds[ch] = 0;}while(0)

       if( icebuf_holds[0])FLUSH_ESD_HOLD_BUF(0);\
       if( icebuf_holds[1])FLUSH_ESD_HOLD_BUF(1);\
       if( icebuf_holds[2])FLUSH_ESD_HOLD_BUF(2);\
}
#endif

static scan_err_t pipe_poll( t_ScanMechStatus mech_status, 
                             struct adf_paper_path *adfpath,
                             struct adf_page_gapper *adfgapper  )
{
    scan_err_t scerr;
    const struct scanvars *sv;

    sv = scanvar_get();

    /* davep 21-Jan-2009 ; test/debug code for pause/resume  */
    if( scan_get_config() & SCAN_CONFIG_PIPE_DRAIN ) {
        dbg2( "%s allowing the pipe to drain\n", __FUNCTION__ );
        return SCANERR_NONE;
    }

    /* top off any buffers that might be empty */
    if( !sv->use_pie && !sv->use_hp ) {
        /* davep 23-Feb-2012 ; add fwmargin as last link in chain */
        if( sv->use_fw_margins ) {
            /* fwmargin in threadspace so no interrupts to disable */
            fw_wmargin_refill( pstats.fw_wmargin_rows_per_buffer );
        }
        else {
            pic_interrupt_disable();
            /* davep 02-Mar-2009 ;  need to call the ice dma refill with our
             * add buffer callback. Our scanpipe callback will add rows to
             * scancmdq, something the pic dma refill function cannot do.
             * When we add rows to PIC, we need to tell scancmdq about it as
             * well.
             */
            ice_dma_refill( pic_wdma_get_mm(), pstats.picw_rows_per_buffer,
                    scanpipe_pic_wdma_add_buffer, pic_wdma_channel_launch );
            pic_interrupt_enable();
        }
    }
    else if( !sv->use_hp ) {
        pie_interrupt_disable();
        piems_wdma_refill( pstats.piew_rows_per_buffer );
        pie_interrupt_enable();
    }
    else {
        INT_DISABLE_HALFPACK;
        piehp_wdma_refill( pstats.piew_rows_per_buffer );
        INT_ENABLE_HALFPACK;
    }

    /* dump some useful information */
    dbg2("total rows exp:%d picw:%d pier:%d piew:%d hpr:%d hpw:%d\n",
        pstats.scan_expected_rows, pstats.picw_tot_rows,
        pstats.pier_tot_rows, pstats.piew_tot_rows,
        pstats.hpr_tot_rows, pstats.hpw_tot_rows );

    dbg2("heapfree=%d\n", scanmem_get_free_size() );

    /* davep 05-Aug-2009 ; we might pause between pages on ADF:
     *  - to reset the motor move to prevent overflow
     *  - if there is limited memory remaining
     *  - if the motor gets too hot
     *
     * davep 06-Sep-2011 ; to flip a page during duplex scanning 
     */
    if( use_pipecut ) {
        if( adfgapper->resume_on_mech_ready && mech_status==SCAN_MECH_READY )
        {
            ASSERT( !adfgapper->stop_at_next_page_gap );

            /* Call into the ADF gapper. If we have enough memory for the next
             * page, this function will resume us.
             */
            scerr = adf_page_gapper_resume( adfgapper, adfpath );
            if( scerr != SCANERR_NONE ) {
                /* oh noes! */
                return scerr;
            }
        }
    }

    /* davep 28-May-2013 ; poll the piemux in case something new arrived */
//    if( use_piemux ) {
//        piemux_scheduler();
//    }

    /* davep 27-Mar-2008 ; pick and choose useful debug */
//    scan_cmdq_dump();
//    pic_dump();
//    pic_wdma_dump();
//    pic_wdma_debug_log();
//    scanlog_set_level( 2 );
//    scif_dump();
//    pluto_dump();
//    cisx_dump();
//    cisx_ddma_dump();
//    pic_dump();
//    scanman_dump_stats();
//    pic_cwdma_interrupt_stats_dump();
//    scanalyzer_dump();
//    scan_cmdq_dump();
//    scan_interrupt_stats_dump();
//    scan_cmdq_dump_queue_status();
//    pic_wdma_debug_log();
//    piemux_dump();
//    pie_dump();
//    pie_xy_dump();
//    piems_rdma_debug_log();
//    piems_wdma_debug_log();
//    pie_rdma_dump();
//    pie_wdma_dump();
//    pie_interrupt_stats_dump();
//    fw_rmargin_debug_log();
//    fw_wmargin_debug_log();
//    adf_paper_path_debug_log( adfpath );
//    pipe_cutter_dump();

    /* davep 21-Jun-2013 ; here? */
    piemux_scheduler();

    return SCANERR_NONE;
}

#define GET_WAIT_WRITE_FLAGS(x)  ( (x)&0x00ff)
#define GET_WAIT_READ_FLAGS(x)   ( (x)&0xff00)


scan_err_t pipe_wait( void )
{
    scan_msg_t msg;
    struct scos_mq * msgq;
    uint32_t scan_wait, pic_wait, pie_wait, hp_wait, filter_wait, fw_margin_wait;
    struct ice_dma_buffer *icebuf;
    uint8_t channel;
    scan_err_t pipe_wait_retcode;
#if	BGR_REMOVE
	struct scanvars *sv;	// mod.lsh for background removal (sv should be updated at every page)
#else
    const struct scanvars *sv; 
#endif    
    scan_err_t scerr;
    struct adf_paper_path adfpath;
    struct adf_page_gapper adfgapper;
    t_ScanMechStatus mech_status;
#ifdef HAVE_PIC_PSESD
    int picw_rows[3]={0,0,0};
    int last_esd_rows[3] = {-1,-1,-1};
    struct ice_dma_buffer *icebuf_holds[3]={0,0,0};
    struct ice_dma_buffer *icebuf_tmp;
    bool TOF_detected = false;
#endif
    int scan_pic_row_ending;
    scan_adf_paper_event_t adfevt;
    bool previous_flag_state;
    uint8_t sensor_num;
	int timeout_log_cnt = 0;	//add.lsh.for Unexpected Timeout Debug
	/* useful debug */
//    afe_dump();
//    scif_dump();
//    pic_dump();

    sv = scanvar_get();
    
#if	BGR_REMOVE
	if(sv->use_pie && sv->bgr_sensitivity != 0){	// add.lsh for background removal
		background_init();
		background_open();
	}
#endif

	if(sv->use_pie)
		dbg1("[S_P] Step6. %s (d_s:%d(f0,a_s1,a_d3), dpi:%d, mode(m1,c2):%d) mem:%d\n", __FUNCTION__, sv->doc_src, sv->dpi, sv->cmode, scanmem_get_free_size());
    	
    msgq = scantask_get_msgq();

    make_wait_flags( &scan_wait, &pic_wait, &pie_wait, &hp_wait, &filter_wait, &fw_margin_wait );

    /* Assume we'll succeed. If we don't succeed, we run the risk of failure! */
    pipe_wait_retcode = SCANERR_NONE; 

    mech_status = SCAN_MECH_ERROR;

    /* davep 01-Jun-2009 ; XXX start sending data as soon as it arrives
     * (experimental) 
     */
    if( use_pipecut ) {
        uint32_t pipecut_move_distance_rows; 
        uint32_t required_memory_for_next_page;

        adf_paper_path_init( sv, &adfpath );

        scands_get_integer_with_default( "pipecut_move_distance_rows", 
                                         &pipecut_move_distance_rows, 
                                         DEFAULT_PIPECUT_MOVE_DISTANCE_ROWS  );

        /* davep 08-Sep-2011 ; creating a new 'adf page gapper' to handle all
         * the cases we may need to handle at page gaps
         */
        scands_get_integer_with_default( "required_memory_for_next_page", 
                                         &required_memory_for_next_page,
                                         DEFAULT_REQUIRED_MEMORY_FOR_NEXT_PAGE );
        adf_page_gapper_init( &adfgapper, required_memory_for_next_page, pipecut_move_distance_rows );

        /* whether or not we're staged, we start the pipecutter callback outside
         * the page so we can track the edges (Yes, it's confusing). The
         * pipecutter callback has to carefully maintain state (inpage/!inpage)
         * so can know when to send pagestart/pageend.
         */
        pipecut_page_state_init();

        /* davep 09-Oct-2013 ; if the mech driver staged the ADF page right
         * into the TOF, we need a marker to indicate we are in a page
         */
        if( adf_sensor_paper_in_path() ) {
            pipe_cutter_add_marker( 0 );
        }
        /* davep 09-Jul-2013 ; if we're using >1 sensor, we need to tell
         * pipecut how far apart the sensors are
         */
        if( use_nsensor ) {
            uint32_t page_side_offset;

            page_side_offset = safeint_scale_uint32(
                                adfpath.adf_margins.rows_to_tof_backside,
                                adfpath.scale.y_numer, adfpath.scale.y_denom ),
            pipe_cutter_set_page_side_offset( PAGE_SIDE_1, page_side_offset );
        }
    }
    else {
        /* The pipecutter is responsible for sending pagestart/pageend when the
         * tof/bof are found. But we're not using the pipecutter, we'll send the
         * pagestart here. We'll send a pageend at the bottom of the while loop.
         */
        if( use_scanman ) {
            /* davep 12-Apr-2013 ; adding page_side to page start/end messages */
            scanlib_send_page_start(PAGE_SIDE_0);
            pstats.page_starts++;
            dbg2( "[S_P] Invalid : Send SMSG_PAGE_START (No PipeCut)\n");
        }
    }
    
#ifdef HAVE_PIC_PSESD
#define PIC_ESD_FLUSH_PICW_DMA do{pic_esd_flush_buffers(icebuf_holds);}while(0)
#else
#define PIC_ESD_FLUSH_PICW_DMA
#endif

    /* BEWARE!  This code does NOT NOT NOT break out of the message handling
     * switch statement, does NOT break out of the while loop!
     *
     * This code calls a few functions to cancel the scan then CONTINUES ON
     * after it finishes.
     *
     * If we put a 'continue' at the bottom of the escape hatch, we could leak
     * memory in an unhandled message.
     *
     * I should probably fix this somehow.  davep 08-Sep-2011
     */
#define ESCAPE_HATCH(pipe_retcode) \
            do { \
                dbg2( "%s escape retcode=%d\n", __FUNCTION__, pipe_retcode );\
                if( !use_icefile ) scan_cmdq_cancel();\
                PIC_ESD_FLUSH_PICW_DMA;\
                pipe_cancel();\
                scan_wait=pic_wait=fw_margin_wait=pie_wait=filter_wait=hp_wait = 0;\
                pipe_wait_retcode = pipe_retcode;\
                dbg2( "%s be aware: continuing loop\n", __FUNCTION__ );\
            } while(0);
    while( scan_wait || pic_wait || pie_wait || hp_wait || filter_wait || fw_margin_wait ) {

//        dbg1("waiting for scan message...\n");
		
        /* sleep waiting for messages */
        scerr = scos_mq_receive( msgq, &msg, USEC_PER_SECOND*PIPE_WAIT_SECONDS ); 
        
        if( scerr==SCANERR_TIMEDOUT ) {
            dbg1("%s no messages; wait some more\n", __FUNCTION__ );
            dbg2("scan_wait=0x%x pic_wait=0x%x pie_wait=0x%x hp_wait=0x%x\n", 
                        scan_wait, pic_wait, pie_wait, hp_wait );
			
            scerr = pipe_poll( mech_status, &adfpath, &adfgapper ); 
            if( scerr != SCANERR_NONE ) {
#if 1
                /* User Cancel */	// add.lsh for USER CANCEL
                if( scerr==SCANERR_SCAN_CANCELLED  ) {
                	dbg1( "[S_CANCEL] %s Cancel!!, t_r:%d\n", __FUNCTION__, pstats.pier_tot_rows);
                }
#endif
            	dbg1( "[S_P] Escape 1(Timeout & Error) t_r:%d\n", pstats.pier_tot_rows);
                ESCAPE_HATCH( scerr );
            }
#if 1
			extern bool bScanEC;
            /* Unexpected Timeout Debug */
            if(timeout_log_cnt++ >= 30){
            	dbg1("scan_wait=0x%x pic_wait=0x%x pie_wait=0x%x hp_wait=0x%x BG_lock:%d\n", 
                    scan_wait, pic_wait, pie_wait, hp_wait, pie_interrupt_lock_status_get() );
            	dbg1("total rows exp:%d picw:%d pier:%d piew:%d hpr:%d hpw:%d\n",
        			pstats.scan_expected_rows, pstats.picw_tot_rows,
        			pstats.pier_tot_rows, pstats.piew_tot_rows,
        			pstats.hpr_tot_rows, pstats.hpw_tot_rows );
        			
        		print_pipe_dgb_cnt();
        		
        		scan_cmdq_dump();				//add.lsh for EC223
    			scan_interrupt_stats_dump();	//add.lsh for EC223
				scif_dump();					//add.lsh for EC223
				if(pic_wait){					//add.lsh for EC228
					pic_cwdma_interrupt_stats_dump();
					pic_wdma_debug_log();
				}
				
				dbg1( "[S_P] Escape (30 sec Timeout) t_r:%d\n", pstats.pier_tot_rows);
    			if( !use_icefile ) scan_cmdq_cancel();
	            pipe_cancel();
    			scanlib_send_sc(SMSG_SC_PIPE_WAIT_TIMEOUT);
    			scan_wait=pic_wait=fw_margin_wait=pie_wait=filter_wait=0;
	            pipe_wait_retcode = SCANERR_NONE;
            }
            // add.lsh KB reset problem after EC221
            else if( bScanEC == true ){
        		dbg1( "[S_P] Escape 1(EC) t_r:%d\n", pstats.pier_tot_rows);
        		ESCAPE_HATCH( SCANERR_SCAN_CANCELLED );
        	}
#endif
            continue;
        }
        else {
        	timeout_log_cnt = 0;
    	}

        if( use_pipenet ) {
            /* pass in scan_wait so we can pretend the network is the "scan"
             * subsystem giving us data
             */
            if( pipenet_message( &msg, &scan_wait ) ) {
                continue;
            }
        }
        switch ( msg.msgType ) {
        case SMSG_PIC_WDMA_DATA: 
        	dbg_cnt_SMSG_PIC_WDMA_DATA++;
        case SMSG_PIC_ESD_FLUSH:
            /* PIC write DMA complete interrupt */
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < PIC_WDMA_NUM_CHANNELS, icebuf->channel );

#ifdef HAVE_PIC_PSESD //EricH; Add to try ESD detection

            if(msg.msgType != SMSG_PIC_ESD_FLUSH )
            {
                if(msg.param2 == ICE_DMA_LAST_BUFFER)
                {
                    /* Modify the message type and send the buffer to myself again
                     *
                     **/
                    msg.msgType = SMSG_PIC_ESD_FLUSH;

                    /*Have to reload the param3 for it has been cleared at above read*/
                    msg.param3 = icebuf;
                    scerr = scantask_msg_send_nowait( &msg );
                }
                /*Buffer the latest pic_wdma */
                icebuf_tmp = icebuf_holds[icebuf->channel];
                icebuf_holds[icebuf->channel] = icebuf;
                /*Do not do anything if we are still buffering*/
                if(!icebuf_tmp)
                    break;

                /*Modify to use buffered data*/
                icebuf = icebuf_tmp;
                /*Clear the last buffer since we are buffering*/
                msg.param2 = 0;
            }else
            {
                /*Clear the a pending bufffer for it is no longer pending*/
                icebuf_holds[icebuf->channel] = 0;
            }

            if(last_esd_rows[icebuf->channel] >=0  &&
               picw_rows[icebuf->channel] <= last_esd_rows[icebuf->channel] &&
               (picw_rows[icebuf->channel] + icebuf->num_rows) > last_esd_rows[icebuf->channel])
            {
                pic_esd_process(last_esd_rows[icebuf->channel], picw_rows[icebuf->channel], icebuf);
                last_esd_rows[icebuf->channel] = -1;
            }

            /* Update next row number for the channel */
            picw_rows[icebuf->channel]+= icebuf->num_rows;
            //*Check whether pending ESD has pass the starting row number of next buffer*/
            if( (last_esd_rows[icebuf->channel] >=0) && 
                    (picw_rows[icebuf->channel] > last_esd_rows[icebuf->channel]))
            {
                dbg1("[S_P] Invalid : BAD! Miss buff for esd color %d, next buf start row is %d t_r:%d\n", 
                            icebuf->channel, picw_rows[icebuf->channel], pstats.pier_tot_rows);
                last_esd_rows[icebuf->channel]=-1;
            }

#endif
            pstats.picw_tot_rows += icebuf->num_rows;

            if( pipetap==PIPETAP_PIC ) {
                scan_debug_net_send( icebuf->channel, icebuf->data,
                             icebuf->num_rows * icebuf->bytes_per_row );
                /* ignore error */
            }

            /* davep 01-Jun-2009 ; adding pipe cutter */
            /* davep 01-Feb-2011 ; adding check for channel==0 */
            if( icebuf->channel==0 && use_pipecut ) {
                adfpath.row_counter += icebuf->num_rows;
                
                /* davep 06-Sep-2011 ; XXX temp debug */
                dbg2( "%s in_page=%d counter=%d tof=%d bof=%d end=%d\n", __FUNCTION__, 
                            adfpath.in_page, adfpath.row_counter, 
                            adfpath.tof_marker, adfpath.bof_marker, 
                            PIPECUT_END_OF_INPUT_ROW_COUNT(sv->hw_dpi_vert) );

                /* look for end of input by waiting for several rows to pass while
                 * we're not in a page
                 */                 
                if( (!adfpath.in_page &&
                     adfpath.row_counter-adfpath.bof_marker > PIPECUT_END_OF_INPUT_ROW_COUNT(sv->hw_dpi_vert) ))
                {
                    /* normal end */
                    dbg2( "%s end of input at row=%d bofrow=%d\n", __FUNCTION__,
                                adfpath.row_counter, adfpath.bof_marker );
                    pipe_cutter_flush();
					if(adf_sensor_paper_present() && (ADF_mech_running == 1)){ //Mod.kks   Set the Present Sensor detecting point on MECH.
	                   // smech_force_failure( SMECH_FAILURE_ADF_PAPER_JAM );
						smech_force_failure(SMECH_FAILURE_ADF_NO_PICKUP_JAM); //Add the J201 Paper Jam define. mod.kks 2014.06.16
						dbg1( "[S_P] Jam!! : Escape 2_1 (Pickup Jam), in:%d, r:%d, b:%d t_r:%d\n", adfpath.in_page, adfpath.row_counter, adfpath.bof_marker, pstats.pier_tot_rows);
						ESCAPE_HATCH(SCANERR_PAPER_JAM);
					}
					else
					{
						dbg1( "[S_P] Escape 2_2 (Normal, in:%d, r:%d, b:%d) t_r:%d, n_s:%d, a_m_r:%d\n", adfpath.in_page, adfpath.row_counter, adfpath.bof_marker, pstats.pier_tot_rows, adf_sensor_paper_present(), ADF_mech_running);
						ESCAPE_HATCH(SCANERR_NONE);
					}
                }

                /* davep 15-Sep-2009 ; assume we jammed if we haven't seen a BOF
                 * after a certain distance
                 */
                if( adfpath.in_page && 
                    adfpath.row_counter-adfpath.tof_marker > PIPECUT_JAM_ROW_COUNT(sv->hw_dpi_vert) ) 
                    //adfpath.row_counter-adfpath.tof_marker > PIPECUT_JAM_ROW_COUNT(sv->user_area_hinch.height , sv->hw_dpi_vert) ) // kks TBD
                     
                {
                    /* this is an abnormal end to our scan so don't call
                     * pipe_cutter_flush() 
                     */ 

                    /* call mech driver to signal the jam */
                    smech_force_failure( SMECH_FAILURE_ADF_PAPER_JAM );

                    /* leave with an error */
                    dbg1( "[S_P] Jam!! : Escape 3 (Scanning Jam), in:%d, r:%d, t:%d) t_r:%d\n", adfpath.in_page, adfpath.row_counter, adfpath.tof_marker, pstats.pier_tot_rows);
                    ESCAPE_HATCH(SCANERR_PAPER_JAM);
                }

                /* davep 16-Jul-2009 ; adding pause between pages on ADF so can
                 * handle motor encoder rollover, low memory, etc.
                 */
                adf_page_gapper_update_distance( &adfgapper, &adfpath, icebuf->num_rows );
                /* check if we need to pause */
                scerr = adf_page_gapper_test_for_pause( &adfgapper, &adfpath );
                if( scerr != SCANERR_NONE ) {
                	dbg1( "[S_P] Invalid : Escape 4 (err:%d, in:%d, r:%d, t:%d) t_r:%d\n", scerr, adfpath.in_page, adfpath.row_counter, adfpath.tof_marker, pstats.pier_tot_rows);
                    ESCAPE_HATCH( scerr );
                }
            } /* end if use_pipecut */
#if	BGR_REMOVE
	#if BGR_REMOVE_FOR_SCAN	// for debugging
			if( sv->use_pie && sv->bgr_sensitivity != 0 && bg_enable == true)
	#else
			if( sv->use_pie && sv->bgr_sensitivity != 0 && bg_enable == true && 
				((sv->id > 200/*SVCID_COPY*/ && sv->id < 300/*SVCID_SCAN*/) || sv->id == 402/*SVCID_MIXED_FAX*/) )
	#endif
			{
				// FB Background Detect
				if( sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED ) // simplex adf 
				{
					dbg2( "[BG] FB background_detect()!!\n");
					background_detect( icebuf, icebuf->num_rows);
				}
				// ADF Background Detect (front)
				else if( sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF || sv->doc_src == SCAN_DOCUMENT_SOURCE_ADF_NSENSOR)
				{
					if( adfpath.in_page && adfpath.row_counter > adfpath.tof_marker ) 
	                {
	                	dbg2( "[BG] ADF background_detect()!!(c_r:%d, r_c:%d, t_m:%d, row:%d)\n", current_rows, adfpath.row_counter, adfpath.tof_marker,
	                				 MIN( adfpath.row_counter - adfpath.tof_marker, icebuf->num_rows ));
	                	background_detect( icebuf, MIN( adfpath.row_counter - adfpath.tof_marker, icebuf->num_rows ) );
					}
				}
				// ADF Background Detect (back)
				else
				{
					;
				}
			}
#endif
            /* davep 04-Mar-2007 ; if we're in an ADF end-of-page state, send
             * the buffer back immediately (sort-of a cancel)
             */
            if( !(GET_BIT( pic_wait, icebuf->channel )) ) {
                dbg1("[S_P] ignore picw icebuf=%p channel=%d rows=%d data=%p total=%d\n", 
                    icebuf, icebuf->channel, icebuf->num_rows,
                    icebuf->data, pstats.picw_tot_rows );
                pic_interrupt_disable();
                pic_wdma_add_ready( icebuf->channel, &icebuf );
                pic_interrupt_enable();
                break;
            }

            pipe_dbg2("[S_P] picw msg=%p channel=%d rows=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->data, pstats.picw_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {	// cmt.lsh ICE_DMA_LAST_BUFFER is used for FB or ADF(no pipecut)
                if(sv->use_pie)
                	dbg1("[S_P] picw dma channel %d done, t_r:%d\n", icebuf->channel, pstats.pier_tot_rows );
                	
                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( pic_wait, icebuf->channel )), icebuf->channel );
                CLR_BIT( pic_wait, icebuf->channel );

                /* davep 04-Apr-2013 ; pipe cutter runs forever so should never
                 * see a channel finish
                 */
                ASSERT( !use_pipecut );
            }

            channel = icebuf->channel;

            //----- do something with the data -----
            if( sv->use_fw_margins ) {
                /* firmware margins use PIC's channel numbers */
                fw_rmargin_add_ready( channel, &icebuf );
                fw_rmargin_channel_launch( channel );
            }
            else if (sv->use_pie) {
                /* davep 24-May-2013 ; adding piemux */
                piemux_add_ready( icebuf->channel, &icebuf );
                XASSERT( icebuf==NULL, icebuf->tag );
            } 
            else if (use_results) {
                if( use_scanman ) {
                    /* davep 09-Oct-2006 ; send calibration data to host; have to
                     * clone the data since we have use_results set
                     */
                    dup_data_to_scanman( icebuf, msg.param2==ICE_DMA_LAST_BUFFER );
                }
                ATInsertTailList( &pipe_results, &icebuf->listnode );
                icebuf = NULL;
            } 
            else if( use_scanman ) {

                channel = icebuf->channel;

                if( use_pipecut ) {
                    /* davep 10-Apr-2013 ; adding page_side */
                    /* davep 23-May-2013 ; adding sensor_num to DMA data
                     * messages */
                    dbg2( "%s ch=%d sensor=%d page=%d\n", __FUNCTION__,
                            icebuf->channel, icebuf->sensor_num, icebuf->channel & 1 );

                    /* davep 23-May-2013 ; so far, we only have max two sensors */
                    XASSERT( icebuf->sensor_num<2, icebuf->sensor_num );

                    /* sensor_num now becomes page_side */
                    pipe_cutter_add_ready( icebuf->sensor_num, &icebuf );
                    XASSERT( icebuf==NULL, icebuf->channel );
                }
                else {
                    /* send data to scan manager for further processing. Note
                     * using page_side==0 since we're scanning from the flatbed
                     * (not using the pipecutter)
                     */
                    send_data_to_scanman( PAGE_SIDE_0, icebuf, msg.param2==ICE_DMA_LAST_BUFFER );
                    return_icebuf_to_pic_wdma( icebuf->channel, &icebuf );
                }
            }
            else { 
                /* send it back to PIC */
                return_icebuf_to_pic_wdma( icebuf->channel, &icebuf );
            } /* end if/else */
            break;
        

        case SMSG_PIE_RDMA_DATA: 
        	dbg_cnt_SMSG_PIE_RDMA_DATA++;
            /* PIE read DMA complete interrupt */
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < PIE_RDMA_NUM_CHANNELS, icebuf->channel );

            pstats.pier_tot_rows += icebuf->num_rows;

            pipe_dbg2("pier msg=%p channel=%d rows=%d sensor=%d data=%p total=%d\n", 
                    icebuf, icebuf->channel, icebuf->num_rows,
                    icebuf->sensor_num, icebuf->data, pstats.pier_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {	// cmt.lsh ICE_DMA_LAST_BUFFER is used for FB or ADF(no pipecut)
                dbg1("[S_P] pier dma channel %d done, t_r:%d\n", icebuf->channel, pstats.pier_tot_rows );
                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( pie_wait, icebuf->channel + 8 )), icebuf->channel );
                CLR_BIT( pie_wait, icebuf->channel + 8 );
            }

            if( use_pipenet ) {
                pipenet_rdma_buffer( &icebuf );
                /* icebuf will be NULL at this point */
            }
            /* davep 09-Dec-2010 ; adding firmware margins */
            else if( sv->use_fw_margins ) {
                /* fwmargins use PIC's channel numbers */
                channel = pipemap_pie_to_pic_channel( icebuf );
                /* fwmargins run entirely in scantask threadspace so nothing to
                 * disable 
                 */
                fw_wmargin_add_ready( channel, &icebuf );
                fw_wmargin_channel_launch( channel );
            }
            else {
                /* send it back to PIC */
                return_icebuf_to_pic_wdma( pipemap_pie_to_pic_channel(icebuf), &icebuf );
            }
            break;


        case SMSG_PIE_WDMA_DATA: 
        	dbg_cnt_SMSG_PIE_WDMA_DATA++;
            /* PIE write DMA complete interrupt */
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < PIE_WDMA_NUM_CHANNELS, icebuf->channel );

            pstats.piew_tot_rows += icebuf->num_rows;

            if( pipetap==PIPETAP_PIE ) {
                scan_debug_net_send( icebuf->channel, icebuf->data,
                             icebuf->num_rows * icebuf->bytes_per_row );
                /* ignore error */
            }
#if 0
            /* davep 12-Sep-2005 ; debugging code for asserting on memory
             * corruption. Add in extra padding into XYScale's OUTX and check
             * for it here.
             */
            {
                uint8_t last_byte;
                int i;

                /* check the last byte of the last row */
//                last_byte = icebuf->data[ icebuf->num_rows * icebuf->bytes_per_row - 1];
//                XASSERT( last_byte==0xbb, last_byte );

                /* Check each row in the buffer. The last byte of each data row
                 * should be 0xbb because of the extra DMA padding I'm adding.
                 */
                for( i=0 ; i<icebuf->num_rows ; i++ ) {
                    last_byte = icebuf->data[ (i+1) * icebuf->bytes_per_row - 1];
                    XASSERT( last_byte==0xbb, last_byte );
                }
            }
#endif

            pipe_dbg2("piew msg=%p channel=%d rows=%d bpr=%d sensor=%d data=%p total=%d\n", 
                    icebuf, icebuf->channel, icebuf->num_rows,
                    icebuf->bytes_per_row, icebuf->sensor_num,
                    icebuf->data, pstats.piew_tot_rows );

            /* stupid human check */
            XASSERT( icebuf->num_rows > 0, (uint32_t)icebuf );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {	// cmt.lsh ICE_DMA_LAST_BUFFER is used for FB or ADF(no pipecut)
                dbg1("[S_P] piew dma channel %d done, t_r:%d\n", icebuf->channel, pstats.pier_tot_rows );
                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( pie_wait, icebuf->channel )), icebuf->channel );
                CLR_BIT( pie_wait, icebuf->channel );
            }

            //----- do something with the data -----
            if( filter_position==SV_SW_FILTER_AFTER_PIE ) {
                /* Send the data to the software filter. The software filter is
                 * responsible for the next step in the chain.
                 *
                 * We can add buffers into the software filter because the scan
                 * code mostly runs from one thread and the software filter has
                 * no chance of getting hit by interrupts.
                 */

                /* grab a copy of the channel because icebuf will be NULL after
                 * we come back from _add_ready()
                 */
                channel = icebuf->channel;

                ice_filter_add_ready( channel, &icebuf );
                ice_filter_channel_launch( channel );
            }
            else if (sv->use_hp) {
                send_pie_icebuf_to_hp( &icebuf );
            } 
            else if (use_scanman) {
                if( use_pipecut ) {
                    /* davep 10-Apr-2013 ; adding page_side */
                    /* davep 23-May-2013 ; adding sensor_num to DMA data
                     * messages */
//                    dbg2( "%s ch=%d sensor=%d\n", __FUNCTION__,
//                            icebuf->channel, sensor_num );
                    pipe_cutter_add_ready( icebuf->sensor_num, &icebuf );
                    XASSERT( icebuf==NULL, icebuf->channel );
                }
                else { 
                    XASSERT( icebuf->sensor_num==0, icebuf->sensor_num );
                    send_data_to_scanman( PAGE_SIDE_0, icebuf, msg.param2==ICE_DMA_LAST_BUFFER );
                    XASSERT( icebuf->data==NULL, icebuf->channel );
                    /* now send the icebuf header itself back to PIE */
                    return_icebuf_to_pie_wdma( icebuf->channel, &icebuf );
                }
            } 
            else {
                /* Send it back to PIE */
                return_icebuf_to_pie_wdma( icebuf->channel, &icebuf );
            }
            break;

        case SMSG_PIEHP_RDMA_DATA: 
            /* HalfPack read DMA complete interrupt */
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < PIEHP_RDMA_NUM_CHANNELS, icebuf->channel );

            pstats.hpr_tot_rows += icebuf->num_rows;

            pipe_dbg2("hpr  msg=%p channel=%d rows=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->data, pstats.hpr_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                dbg1("hpr  dma channel %d done\n", icebuf->channel );
                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( hp_wait, icebuf->channel + 8 )), icebuf->channel );
                CLR_BIT( hp_wait, icebuf->channel + 8 );
            }

            /* Send it back to PIE */
            return_icebuf_to_pie_wdma( pipemap_piehp_to_pie_channel(icebuf), &icebuf );
            break;

        case SMSG_PIEHP_WDMA_DATA: 
            /* HalfPack write DMA complete interrupt */
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < PIEHP_WDMA_NUM_CHANNELS, icebuf->channel );

            pstats.hpw_tot_rows += icebuf->num_rows;

            if( pipetap==PIPETAP_HP ) {
                scan_debug_net_send( icebuf->channel, icebuf->data,
                             icebuf->num_rows * icebuf->bytes_per_row );
                /* ignore error */
            }

            pipe_dbg2("hpw  msg=%p channel=%d rows=%d bpr=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->bytes_per_row,
                icebuf->data, pstats.hpw_tot_rows );

            /* stupid human check */
            XASSERT( icebuf->num_rows > 0, (uint32_t)icebuf );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                dbg1("hpw  dma channel %d done\n", icebuf->channel );
                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( hp_wait, icebuf->channel )), icebuf->channel );
                CLR_BIT( hp_wait, icebuf->channel );
            }

            //----- do something with the data -----
            if (use_scanman) {

                /* davep 01-Jun-2009 ; adding the pipe cutter */
                if( use_pipecut ) {
                    pipe_cutter_add_ready( PAGE_SIDE_0, &icebuf );
                    XASSERT( icebuf==NULL, icebuf->channel );
                }
                else { 
                    /* send data to scan manager for further processing */
                    send_data_to_scanman( PAGE_SIDE_0, icebuf, msg.param2==ICE_DMA_LAST_BUFFER );
                    return_icebuf_to_piehp_wdma( icebuf->channel, &icebuf );
                }
            } 
            else {
                return_icebuf_to_piehp_wdma( icebuf->channel, &icebuf );
            }
            break;
        

        /* 19-June-05 davep ; added thread-level support for the complex
         * MultiStrip DMA driver
         */
        case SMSG_PIE_RDMA_INT: 
        	dbg_cnt_SMSG_PIE_RDMA_INT++;
            sensor_num = SMSG_PIEMS_INT_GET_SENSOR_NUM(msg.param1);
            pipe_dbg2("pier dma intmsg ch=%d sensor=%d\n", msg.param2, sensor_num );

            /* davep 22-May-2013 ; adding piemux */
            piemux_rdma_intmsg( sensor_num, msg.param2 );
            break;
        
        case SMSG_PIE_WDMA_INT: 
        	dbg_cnt_SMSG_PIE_WDMA_INT++;
            sensor_num = SMSG_PIEMS_INT_GET_SENSOR_NUM(msg.param1);
            pipe_dbg2("piew dma intmsg ch=%d sensor=%d\n", msg.param2, sensor_num );

            /* davep 22-May-2013 ; adding piemux */
            piemux_wdma_intmsg( sensor_num, msg.param2 );
            break;

        case SMSG_SCAN_SCMDCOMP: 
            /* We've received a scan interrupt and SSTAT.SCMDCOMP was set
             * therefore our scan command has completed.
             */
            //dbg1("[S_P] scan command complete\n" );
            /* Scan's part in this nickelodeon is now done */
            scan_wait = 0;
            break;

        case SMSG_AFE_HW_FAILURE : 
            /* davep 13-Jun-2012 ; catch AFE hardware failure; can happen in
             * interrupt context so ISR will send the thread this message
             */
            dbg1( "[S_P] Error : Escape 5 (afe failure param1=0x%x), t_r:%d\n", msg.param1, pstats.pier_tot_rows);
            ESCAPE_HATCH( SCANERR_SCAN_CANCELLED );
            break;

        case SMSG_SCAN_CANCEL :
        	dbg1( "[S_P] Warning : Escape 6 (Canceled), t_r:%d\n", pstats.pier_tot_rows);
            ESCAPE_HATCH( SCANERR_SCAN_CANCELLED );
            break;

        case SMSG_SCAN_CMDQSTOP :
            dbg1( "[S_P] scan paused: 0x%x 0x%x, t_r:%d\n", msg.param2, (uint32_t)msg.param3, pstats.pier_tot_rows );
            break;

        case SMSG_SCAN_SCMDEND :

            /* davep 23-Mar-2012 ; this message is deprecated and should never
             * be seen. Will be removed ASAP.
             */
            ASSERT( !"deprecated code! you should never see this!" );

            // param2 = row scan is now ending
            scan_pic_row_ending = msg.param2;  

            dbg2("%s ADF end of page at %d\n", __FUNCTION__,
                        scan_pic_row_ending );

            if( use_pipecut ) {
                pipe_cutter_flush();
            }
            break;

        case SMSG_ICE_FILTER_DATA :
            /* ICE_FILTER_DATA is data coming out the other end of the software
             * filter. 
             *
             * param2 = true if last buffer, false otherwise
             * param3 = pointer to icebuf
             *
             * If we're positioned after PIE 
             *      If we're using HalfPack, 
             *            send data to HalfPack.
             *      Else 
             *            send data to ScanMan
             *
             * If we're positioned after PIC 
             *     If we're using PIE,
             *            send data to PIE.
             */

            XASSERT( filter_position!=SV_SW_FILTER_DISABLED, filter_position );

            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );
            XASSERT( icebuf->channel < ICE_FILTER_NUM_CHANNELS, icebuf->channel );

            pipe_dbg2("icef msg=%p channel=%d rows=%d bpr=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->bytes_per_row,
                icebuf->data, pstats.hpw_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                dbg1("[S_P] Invalid : icef channel %d done, t_r:%d\n", icebuf->channel, pstats.pier_tot_rows );

                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( filter_wait, icebuf->channel )), icebuf->channel );
                CLR_BIT( filter_wait, icebuf->channel );
            }

            if( filter_position==SV_SW_FILTER_AFTER_PIE ) {
                if( sv->use_hp ) {
                    send_pie_icebuf_to_hp( &icebuf );
                }
                else if( use_scanman ) {
                    send_data_to_scanman( PAGE_SIDE_0, icebuf, msg.param2==ICE_DMA_LAST_BUFFER );

                    /* now send the icebuf header itself back to PIE */
                    return_icebuf_to_pie_wdma( icebuf->channel, &icebuf );
                }
                else { 
                    /* TODO */
                    ASSERT(0);
                }
            }
            else if( filter_position==SV_SW_FILTER_AFTER_PIC ) {
                /* TODO */
            }
            else {
                XASSERT( 0, filter_position );
            }
            break;

        case SMSG_SCAN_MECH_STATUS:
            //Tell cmdq that the mech has changed status. --Theron
            mech_status = (t_ScanMechStatus)msg.param1;
            if( use_pipecut )
            	dbg1( "[S_P] %s mech status change param1=%d, resume:%d\n", __FUNCTION__, mech_status, adfgapper.resume_on_mech_ready );
            scan_cmdq_mech_status_change(mech_status);

            /* davep 16-Jul-2009 ; adding pause between pages on ADF so can
             * reset the motor move to prevent overflow
             */
            if( use_pipecut ) {
                if( adfgapper.resume_on_mech_ready && mech_status==SCAN_MECH_READY )
                {
                    /* davep 17-Nov-2011 ; flush the pipecuter to send the
                     * pageend downstream (helps with downstreams holding onto
                     * memory, e.g., ADF copies)
                     */
                    pipe_cutter_flush();

                    /* we successfully finished pausing. Now attempt to resume. */
                    scerr = adf_page_gapper_resume( &adfgapper, &adfpath );
                    if( scerr != SCANERR_NONE ) {
                        /* sneaky trick. An 'out of paper' error simply means
                         * our ADF has run dry. No big deal. Change to a normal
                         * error
                         */
                        if( scerr==SCANERR_NO_PAPER_LOADED  ) {
                            dbg2( "[S_P] adf empty so ending scan, t_r:%d\n", __FUNCTION__, pstats.pier_tot_rows );
                            scerr = SCANERR_NONE;
                        }
                        dbg1( "[S_P] Error : Escape 7 (Not Resume:%d), t_r:%d\n", scerr, pstats.pier_tot_rows);
                        ESCAPE_HATCH( scerr );
                    }
                }
            }
            break;

        case SMSG_SCAN_MECH_FAULT :
            /* We have no idea what's in this message or what it mean. (It's a
             * message from the mech driver to the mech driver.) Pass directly
             * down to the mech driver.  If smech_fault_msg() returns an error,
             * we will cancel the scan with that error code.
             */
            scerr = smech_fault_msg( &msg );
            
            /* call mech driver to signal the jam */
            //smech_force_failure( SMECH_FAILURE_ADF_PAPER_JAM );	// add.lsh
            
            //if(msg.param1 == SMECH_ADF_ERROR_CIO) TBD mod.kks
			smech_force_failure( SMECH_FAILURE_ADF_TOPCOVER_OPEN_JAM );	//Top Cover Open Error mod.kks 2014.06.13
            
            if( scerr != SCANERR_NONE ) {
                /* Holy hardware, Batman! Our mech says things have gone all
                 * pear shaped. Stop work and clean up.
                 */
				dbg1( "[S_P] Jam!! : Escape 9 (Cover Open), %d\n", pstats.pier_tot_rows);
                ESCAPE_HATCH( scerr );
            }
            break;

        case SMSG_SCAN_MECH_IOCTL :
            /* davep 24-May-2012 ; adding ioctl handler so mech driver can
             * communicate with itself.  We have no idea what's in this
             * message or what it mean. (It's a message from the mech
             * driver to the mech driver.) Pass directly down to the mech
             * driver.
             */
            dbg1( "[S_P] IOCTL\n");
            scerr = smech_ioctl( msg.param1, msg.param2, msg.param3 );
            break;

        case SMSG_SMECH_POLL :
            /* We should shut off the polling before we run a scan but gently
             * ignore this message anyway. No sense risking a race condition.
             */
            break;

        case SMSG_SCAN_ADF_PAPER_EVENT :
            /* davep 27-Aug-2009 ; adding adf paper event messages. Currently we
             * only expect the paper-in-path switch events
             */

            if( !use_pipecut ) {
                /* if we're not using the pipecutter, shouldn't see this so
                 * ignore it even if it does happen 
                 */
                break;
            }
            
            /*mod.kks detect the short paper*/
			if(adfpath.in_page&& adfpath.row_counter-adfpath.tof_marker <  SUPPORTED_PAPER_MIN_SIZE(sv->hw_dpi_vert) ) {
				 /* call mech driver to signal the jam */
                    smech_force_failure( SMECH_FAILURE_SHORT_PAPER );

                    /* leave with an error */
                    ESCAPE_HATCH(SMECH_FAILURE_SHORT_PAPER);
					dbg1("SHORT PAPER JAM!! %d  \n",adfpath.row_counter-adfpath.tof_marker );
				break;
			}
			
            adfevt = (scan_adf_paper_event_t)msg.param1;
			
            /* currently only care about the paper tof/bof event */
            if( adfevt != SCAN_ADF_PAPER_TOF ) {
                dbg1( "[S_P] Warning : %s ignoring adf paper event=%d value=%d\n", 
                            __FUNCTION__, adfevt, (uint32_t)msg.param2 );
                break;
            }

            dbg2( "[S_P] adf paper event=%d value=%d t_r=%d\n", 
                        adfevt, (uint32_t)msg.param2, pstats.pier_tot_rows );

            /* davep 01-Sep-2011 ; Duplex. At BOF, pause so we can flip */
            previous_flag_state = adfpath.in_page;
			//dbg1("kks_test1 adfpath.in_page %d  sensor state %d  xfer_length %d \n",adfpath.in_page,msg.param2,msg.param3);
            adf_paper_event( adfevt, (uint32_t)msg.param2, (uint32_t)msg.param3, &adfpath );

            /* davep 01-Sep-2011 ; duplex; at BOF, pause so we can flip */
            if( adfpath.use_duplex ) {	// cmt.lsh.invalid in case of pinetree
                if( previous_flag_state != adfpath.in_page && !adfpath.in_page ) {
                    dbg1( "[S_P] Invalid : will stop at page gap for duplex\n", __FUNCTION__ );
                    adfgapper.stop_at_next_page_gap = true;
                }
            }
            /* Check Pause Condition */
            scerr = adf_page_gapper_poll( &adfgapper, &adfpath );
            if( scerr != SCANERR_NONE ) {
            	dbg1( "[S_P] Invalid : Escape 10 (Gapper Poll), t_r:%d\n", pstats.pier_tot_rows);
                ESCAPE_HATCH( scerr );
            }
            break;

        /* davep 09-Dec-2010 ; adding firmware margins */
        case SMSG_FW_READ_MARGIN_DATA :
            ASSERT( sv->use_fw_margins );
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );

            channel = icebuf->channel;

            pipe_dbg2("fw_rmarg msg=%p channel=%d rows=%d bpr=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->bytes_per_row,
                icebuf->data, pstats.piew_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                dbg1("fwmargin read channel=%d done\n", icebuf->channel );

                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( fw_margin_wait, icebuf->channel + 8 )), icebuf->channel );
                CLR_BIT( fw_margin_wait, icebuf->channel + 8 );
            }
            /* assume fwmargins hardwired between PIC->PIE; send it back to PIC
             * at exactly the same channel
             */
            return_icebuf_to_pic_wdma( icebuf->channel, &icebuf );
            break;

        case SMSG_FW_WRITE_MARGIN_DATA :
            ASSERT( sv->use_fw_margins );
            PTR_ASSIGN( icebuf, msg.param3 );
            icebuf_sanity( icebuf );

            /* fwmargins use PIC's channel numbers; assume this buffer came
             * from PIC (fwmargins hardwired between PIC->PIE)
             */
            channel = icebuf->channel;

            pipe_dbg2("fw_wmarg msg=%p channel=%d rows=%d bpr=%d data=%p total=%d\n", 
                icebuf, icebuf->channel, icebuf->num_rows,
                icebuf->bytes_per_row,
                icebuf->data, pstats.piew_tot_rows );

            /* last buffer in this channel? */
            if ( msg.param2 == ICE_DMA_LAST_BUFFER ) {
                dbg1("fwmargin write channel=%d done\n", icebuf->channel );

                /* make sure we don't get more than one done message */
                XASSERT( (GET_BIT( fw_margin_wait, icebuf->channel )), icebuf->channel );
                CLR_BIT( fw_margin_wait, icebuf->channel );
            }

            /* let's be lazy and always assume margins hardwired between
             * PIC->PIE 
             */
            if( sv->use_pie ) {
#ifdef __KERNEL__
                /* davep 18-Jun-2013 ; kick the data out of cache before
                 * sending to PIE (this is a shotgun debug )
                 */
                icebuf->dma_handle = dma_map_single( NULL, (void *)icebuf->data, 
                                        icebuf->datalen, DMA_BIDIRECTIONAL );
#endif

                /* pass straight to PIE (fwmargins use PIC's channel numbers) */
                channel = pipemap_pic_to_pie_channel( icebuf );
                pie_interrupt_disable();
                piems_rdma_add_ready( channel, &icebuf );
                piems_rdma_channel_launch( channel );
                pie_interrupt_enable();
            }
            else if( use_scanman ) {
                send_data_to_scanman( PAGE_SIDE_0, icebuf, msg.param2==ICE_DMA_LAST_BUFFER );
                scerr = pipe_topoff( "fwmargin", fw_wmargin_add_ready, 
                             fw_wmargin_free_empty,
                             &icebuf );
                /* icebuf will be NULL here */
                fw_wmargin_channel_launch( channel );
            }
            else {
                /* fwmargins run entirely in scantask threadspace so nothing to
                 * disable 
                 */
                fw_wmargin_add_ready( channel, &icebuf );
                fw_wmargin_channel_launch( channel );
            }
            break;

#ifdef HAVE_PIC_PSESD
        case SMSG_PIC_ESD:
            /*EricH: Note we need to tTraslate row cnt to row number starting from zero
             */
            last_esd_rows[msg.param1] = msg.param2 - 1;
            dbg1("[S_P] Got ESD, color=%d row cnt=%d, buf row cnt=%d\n", msg.param1, msg.param2, picw_rows[msg.param1]);
            break;

        case SMSG_PIC_PSENSE:
            if(!TOF_detected)
            {
                TOF_detected = true;
                dbg1("PSENSE: Page top at row %d\n", msg.param2);
                /*Start page end detection*/
                pic_ps_go(false);
            }
            else
            {
                TOF_detected = false;
                dbg1("PSENSE: Page end at row %d\n", msg.param2);
                /*Start page top detection*/
                pic_ps_go(true);
            }
            break;
#endif
        default :
            /* davep 23-Jul-2008 ; add this default case; I think there was a
             * reason we didn't have it before but I can't remember what it
             * might be. I'm moving from MESSAGE to scan_msg_t, I'm not sure
             * what bugs might be introduced with the change, so I'm putting the
             * assert back.
             */
            XASSERT(0,msg.msgType );
            break;

        } /* switch ( msg.param1 ) */

    } /* while (*_wait != 0) */

    /* davep 06-Oct-2011 ; add check for pipe_wait_retcode so we don't do
     * anything stupid on a cancel or failure
     */
    if( pipe_wait_retcode == SCANERR_NONE ) {
        if( use_pipecut ) {
            pipe_cutter_flush();
        }
        else {
            /* Not using the pipecutter so tell the downstream we're done with a
             * page. (pipecutter responsible for pagestart/pageend)
             */
            if( use_scanman ) {
                /* davep 12-Apr-2013 ; adding page_side to page start/end messages */
                scanlib_send_page_end(PAGE_SIDE_0);
                pstats.page_ends++;
            }
        }
    }
    else {
	    if( use_scanman ) {
	    	if(pstats.page_starts > pstats.page_ends){
            	scanlib_send_page_end(PAGE_SIDE_0);
            	pstats.page_ends++;
            }
        }
	}
	
	if( sv->use_pie )
		dbg1( "[S_P] pipe_wait complete!! %d, p_s=%d, p_e=%d, t_r:%d, mem:%d\n", pipe_wait_retcode,
								pstats.page_starts, pstats.page_ends, pstats.pier_tot_rows, scanmem_get_free_size());
#if	BGR_REMOVE	
	if(sv->use_pie && sv->bgr_sensitivity != 0){	// add.lsh for background removal
		background_close();
#if 1
		if(bg_log_enable == true){
			int k;
			
			dbg1( "[BG] Hisgogram Dump!!\n");
			for(k=0; k<256; k++)
				dbg1( "\n[BG]%d	%d\n", k, histogram[k]);
		
			dbg1( "[BG] LUT Dump!!\n");
			for(k=0; k<256; k++)
				dbg1( "\n[BG]%d	%d\n", k, sv->csc_r_lut.data[k]);
		}		
#endif
	}
#endif

    dbg2( "final data width=%d height=%d\n", 
                g_final_bytes_per_row, g_final_total_rows );

    dbg2("total rows exp:%d picw:%d pier:%d piew:%d hpr:%d hpw:%d\n",
        pstats.scan_expected_rows, pstats.picw_tot_rows,
        pstats.pier_tot_rows, pstats.piew_tot_rows,
        pstats.hpr_tot_rows, pstats.hpw_tot_rows );

//    if( use_pipecut ) {
//        /* davep 22-Jun-2009 ; XXX temp debug to quickly trap on jams */
//        XASSERT( pipe_wait_retcode==0||pipe_wait_retcode==SCANERR_SCAN_CANCELLED , pipe_wait_retcode );
//    }

    return pipe_wait_retcode;
}

void init_pipe_dgb_cnt( void )
{
	dbg_cnt_pic_interrupt=0;
    dbg_cnt_pic_wdma_interrupt=0;
    dbg_cnt_channel_isr=0;
    dbg_cnt_pie_interrupt=0;
    dbg_cnt_pie_dma_interrupt=0;
    dbg_cnt_piems_rdma_isr=0;
    dbg_cnt_piems_wdma_isr=0;
    dbg_cnt_pie_rdma_isr=0;
    dbg_cnt_pie_wdma_isr=0;
    dbg_cnt_ice_buffer_callback=0;
    dbg_cnt_piems_dma_isr=0;
    dbg_cnt_ice_dma_isr=0;
    
    dbg_cnt_SMSG_PIC_WDMA_DATA=0;
    dbg_cnt_SMSG_PIE_RDMA_DATA=0;
    dbg_cnt_SMSG_PIE_WDMA_DATA=0;
    dbg_cnt_SMSG_PIE_RDMA_INT=0;
    dbg_cnt_SMSG_PIE_WDMA_INT=0;
}

void print_pipe_dgb_cnt( void )
{
	dbg1("pic_interrupt : %d\n", dbg_cnt_pic_interrupt );
    //dbg1("pic_wdma_interrupt : %d\n", dbg_cnt_pic_wdma_interrupt ); // same as dbg_cnt_pic_interrupt
    dbg1("channel_isr : %d\n", dbg_cnt_channel_isr );
    dbg1("pie_interrupt : %d\n", dbg_cnt_pie_interrupt );
    //dbg1("pie_dma_interrupt : %d\n", dbg_cnt_pie_dma_interrupt );	// same as dbg_cnt_pie_interrupt
    dbg1("piems_rdma_isr : %d\n", dbg_cnt_piems_rdma_isr );
    dbg1("piems_wdma_isr : %d\n", dbg_cnt_piems_wdma_isr );
    //dbg1("pie_rdma_isr : %d\n", dbg_cnt_pie_rdma_isr );	// not used
    //dbg1("pie_wdma_isr : %d\n", dbg_cnt_pie_wdma_isr );	// not used
    dbg1("piems_dma_isr : %d\n", dbg_cnt_piems_dma_isr );
    dbg1("ice_dma_isr : %d\n", dbg_cnt_ice_dma_isr );
    dbg1("ice_buffer_callback : %d\n", dbg_cnt_ice_buffer_callback );

    dbg1("SMSG_PIC_WDMA_DATA : %d\n", dbg_cnt_SMSG_PIC_WDMA_DATA );
    dbg1("SMSG_PIE_RDMA_DATA : %d\n", dbg_cnt_SMSG_PIE_RDMA_DATA );
    dbg1("SMSG_PIE_WDMA_DATA : %d\n", dbg_cnt_SMSG_PIE_WDMA_DATA );
    dbg1("SMSG_PIE_RDMA_INT : %d\n", dbg_cnt_SMSG_PIE_RDMA_INT );
    dbg1("SMSG_PIE_WDMA_INT : %d\n", dbg_cnt_SMSG_PIE_WDMA_INT );
}
