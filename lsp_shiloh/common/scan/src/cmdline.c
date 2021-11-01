/**
 * \file cmdline.c
 *
 * \brief scantask debug command line
 *
 */
/* 
 * ============================================================================
 * Copyright (c) 2006-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to makefile to prevent agMessage.h
 * from being included in the scan code.  But we need agMessage.h in this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "debug.h"
#include "list.h"
#include "utils.h"
#ifdef __KERNEL__
    #include "scos_cmd_proc.h"
#else
    #include "agMessage.h"
    #include "agRouter.h"
    #include "cmd_proc_api.h"
    #include "error_types.h"
#endif

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanvars.h"
#include "scantask.h"
#include "piehw.h"
#include "pie.h"
#include "cal.h"
#ifdef HAVE_SCAN_CALGRAPH
    #include "calgraph.h"
    #include "calgrapp.h"
#endif
#include "scanplat.h"
#include "scancont.h"
#include "scanpipe.h"
#include "scanlog.h"
#include "scanmech.h"
#include "adfsensor.h"
#include "scands.h"
#include "scandstest.h"
#ifdef HAVE_SCAN_ICEFILE_SUPPORT
    #include "icefileapp.h"
#endif
#include "pic.h"
#include "scanif.h"
#include "cisx.h"
#include "scanmem.h"

/* davep 12-Sep-2012 ; useful for test/debug of callback functions */
#define DBG_PRINT_ARGC_ARGV \
    do {\
        int i;\
        dbg2( "%s argc=%d\n", __FUNCTION__, argc );\
        for( i=0 ; i<argc ; i++ ) {\
            dbg2( "%s i=%d argv=%s\n", __FUNCTION__, i, argv[i] );\
        }\
    } while(0);

/* davep 11-Nov-2008 ; Bug 11161 ; can no longer call scanvar_push() from the
 * Debug thread task so have to use a global.  When "scan var new" is called,
 * we'll store the allocated scanvar here.  "scan var pop" will delete it.
 * "scan start" will message it over to scantask.
 */
static struct scanvars *g_scanvar;

static void cancel_job( void )
{
#if !defined __KERNEL__ && !defined LINUX
    MESSAGE sys_msg;
    error_type_t err;

    sys_msg.msgType = MSG_CANCELJOB;
    sys_msg.param1 = SYS_REQUSER;
    sys_msg.param2 = 0;
    sys_msg.param3 = 0;

    err = SYMsgSend( SJMID, &sys_msg );
    XASSERT( err==OK, err );
#endif
}

/**
 * \brief  "scan set"
 *
 * Collapse a bunch of copy/paste code into a function.
 *
 * \author David Poole
 * \date 17-Oct-2011
 *
 */
/**
 * \brief  set scan flags at run time 
 *
 * Want to be able to change between debug scan-to-network scanning, copies,
 * and scanning to host at runtime. No more recompiles!
 *
 * \author David Poole
 * \date 06-Oct-2005
 *
 */

static int scan_cmd_set_cb( int argc, char *argv[] )
{
    scan_err_t scerr;
    int i, pos;
    uint32_t num32array[8];
    uint32_t num32;

    dbg2( "%s argc=%d\n", __FUNCTION__, argc );
    for( i=0 ; i<argc ; i++ ) {
        dbg2( "%s i=%d argv=%s\n", __FUNCTION__, i, argv[i] );
    }

    /* make sure we have an active configuration we can change */
    if( g_scanvar == NULL ) {
        cmd_printf( "No active scanvar; use \"scan var new\" first.\n" );
        return CMD_ERROR;
    }

    scerr = SCANERR_NONE;

    /* davep 17-Oct-2011 ; fake out the argc to handle the bundling of
     * "scan" "set" into "scan set"
     */
    argc += 1;

    if( argc < 3 ) {
        /* silently ignore imcomplete command lines */
        return CMD_ERROR;
    }

    pos = 1;

    if( str_match( argv[pos], "scale", 5 ) ) {
        pos++;
        
        /* davep 15-Jul-2006 ; swap between linear and rapr */
        if( argc==4 ) {
            if( str_match( argv[pos], "linear", 6 ) ) {
                scanvar_set_xy_scale_mode( g_scanvar, PIE_XY_SCALER_LINEAR );
                cmd_printf( "setting xy scaler mode to linear\n" );
            }
            else if( str_match( argv[pos], "rapr", 4 ) ) {
                scanvar_set_xy_scale_mode( g_scanvar, PIE_XY_SCALER_RAPR );
                cmd_printf( "setting xy scaler mode to rapr\n" );
            }
            else {
                /* silently ignore invalid command lines */
                return CMD_ERROR;
            }
        }
        else {
            if( argc != 7 ) {
                /* silently ignore imcomplete command lines */
                return CMD_ERROR;
            }

            for( i=0 ; i<4 ; i++ ) {
                if( str_mkint( argv[pos], &num32array[i] ) != 0 ) {
                    cmd_printf("scan: bad integer \"%s\"\n", argv[pos] );
                    return CMD_ERROR;
                }
                pos++;
            }

            cmd_printf( "setting scale to x=%"PRIu32"/%"PRIu32" y=%"PRIu32"/%"PRIu32"\n", 
                                 num32array[0], num32array[1], 
                                 num32array[2], num32array[3] );

            scerr = scanvar_set_xyscale( g_scanvar, num32array[0], num32array[1], 
                                        num32array[2], num32array[3] );
        }
    }
    else if( str_match( argv[pos], "area", 4 ) ) {
        /* 6-Oct-05 ; set the scan target area */
        pos++;

        if( argc != 7 ) {
            /* silently ignore imcomplete command lines */
            return CMD_ERROR;
        }

        for( i=0 ; i<4 ; i++ ) {
            if( str_mkint( argv[pos], &num32array[i] ) != 0 ) {
                cmd_printf("scan: bad integer \"%s\"\n", argv[pos] );
                return CMD_ERROR;
            }
            pos++;
        }

        /* Scan target area should be X (upper left), Y (upper right), width,
         * and height. All fields should be in 100ths of an inch since we
         * actually use them as floating point internally.
         */
        scanvar_set_area( g_scanvar, num32array[0], num32array[1], 
                          num32array[2], num32array[3] );

        cmd_printf( "scan area set to %"PRIu32" %"PRIu32" %"PRIu32" %"PRIu32"\n", 
                    num32array[0], num32array[1], 
                    num32array[2], num32array[3] );
    }
    else if( str_match( argv[pos], "pieout", 6 ) ) {
        /* 6-Oct-05 ; set the scan PIE output (XRGB or 3 planes) */
        pos++;

        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }

        if( str_match( argv[pos], "xrgb", 4 ) ) {
            cmd_printf( "setting pie output to xrgb\n" );
            scanvar_set_pieout( g_scanvar, PIE_PIXEL_XRGB );
        }
        else if( str_match( argv[pos], "plane", 5 ) ) {
            cmd_printf( "setting pie output to planar\n" ); 
            scanvar_set_pieout( g_scanvar, PIE_PIXEL_3_PLANES );
        }
        else if( str_match( argv[pos], "mono", 4 ) ) {
            cmd_printf( "setting pie output to mono\n" ); 
            scanvar_set_pieout( g_scanvar, PIE_PIXEL_MONO );
        }
        else if( str_match( argv[pos], "rgbx", 4 ) ) {
            cmd_printf( "setting pie output to rgbx\n" );
            scanvar_set_pieout( g_scanvar, PIE_PIXEL_RGBX );
        }
        /* silently ignore incorrect commands */
    }
    else if( str_match( argv[pos], "picout", 6 ) ) {
        /* 13-Oct-05 ; set the scan PIC output. Currently only support 8 or
         * 16 bit pixels. With our hardware as of this writing, 8-bit
         * pixels requires BDR.
         */
        pos++;

        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }

        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }

        scerr = scanvar_set_pic_bpp( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_pic_bpp failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "dpi", 3 ) ) {
        pos++;

        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }

        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }

        cmd_printf( "setting DPI to %"PRIu32"\n", num32 );
        scerr = scanvar_set_dpi( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_dpi failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "bpp", 3 ) ) {
        pos++;

        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }

        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }

        cmd_printf( "setting bpp to %"PRIu32"\n", num32 );
        scerr = scanvar_set_bpp( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_bpp failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "docsrc", 6 ) ) {
        pos++;

        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }

        if( str_match( argv[pos], "flatbed", 7 ) ) {
            cmd_printf( "doc source set to flatbed\n" );
            scanvar_set_document_source( g_scanvar, SCAN_DOCUMENT_SOURCE_FLATBED );
        }
        else if( str_match( argv[pos], "adf", 3 ) ) {
            cmd_printf( "doc source set to adf\n" );
            scanvar_set_document_source( g_scanvar, SCAN_DOCUMENT_SOURCE_ADF );
        }
        else if( str_match( argv[pos], "adfduplex", 9 ) ) {
            cmd_printf( "doc source set to adfduplex\n" );
            scanvar_set_document_source( g_scanvar, SCAN_DOCUMENT_SOURCE_ADF_DUPLEX );
        }
        else { 
            cmd_printf( "unknown document source \"%s\"\n", argv[pos] );
        }
    }
    else if( str_match( argv[pos], "gamma", 5 ) ) {
        pos++;
        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        scerr = scanvar_set_gamma( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_gamma failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "contrast", 8 ) ) {
        pos++;
        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        scerr = scanvar_set_contrast( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_contrast failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "brightness", 10 ) ) {
        pos++;
        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        scerr = scanvar_set_brightness( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_brightness failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "sharpen", 7 ) ) {
        pos++;
        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        scerr = scanvar_set_sharpness( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_sharpness failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "copybrightness", 14 ) ) {
        pos++;
        if( argc != 4 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        scerr = scanvar_set_copy_brightness( g_scanvar, num32 );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_brightness failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "clipping", 8 ) ) {
        uint16_t black_clip[3], white_clip[3];
        pos++;
        if( argc != 5 ) {
            /* silently ignore incomplete command lines */
            return CMD_ERROR;
        }
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        if( num32 > 255 ) {
            return CMD_ERROR;
        }
        black_clip[0] = black_clip[1] = black_clip[2] = num32;

        pos++;
        if( str_mkint( argv[pos], &num32 ) != 0 ) {
            return CMD_ERROR;
        }
        if( num32 > 255 ) {
            return CMD_ERROR;
        }
        white_clip[0] = white_clip[1] = white_clip[2] = num32;

        scerr = scanvar_set_black_white_clip( g_scanvar, black_clip, white_clip );
        if( scerr != SCANERR_NONE ) {
            cmd_printf( "%s scanvar_set_brightness failed scerr=%d\n", __FUNCTION__, scerr );
            return CMD_ERROR;
        }
    }
    else {
        /* silently ignore unknown subcommands */
    }
    
    return CMD_OK;
}

static void scan_cmd_scanvar_delete_if_exists( void )
{
    /* if we already have an active scanvar, get rid of it */
    if( g_scanvar != NULL )

    {
        cmd_printf( "Deleting existing scanvar %p\n", g_scanvar );
        scanvar_delete( &g_scanvar );
    }
}

static int scan_cmd_var_new_cb( int argc, char *argv[] )
{
    int i;
    int pos;
    struct scanvars *sv=NULL;

    scan_cmd_scanvar_delete_if_exists();

    for( i=0 ; i<argc ; i++ ) {
        dbg2( "%s %d %s\n", __FUNCTION__, i, argv[i] );
    }

    if( argc != 3 ) {
        cmd_printf( "usage: scan var new [copy|colorcopy|scan|fax]\n" );
        return CMD_ERROR;
    }

    pos = 1;
    if( str_match( argv[pos], "copy", 4 ) ) {
        pos++;
        if( str_match( argv[pos], "draft", 5 ) ) {
            sv = scanplat_sv_draft_copy_new();
        }
        else if( str_match( argv[pos], "text", 4 ) ) {
            sv = scanplat_sv_text_copy_new();
        }
        else if( str_match( argv[pos], "mixed", 5 ) ) {
            sv = scanplat_sv_mixed_copy_new();
        }
        else if( str_match( argv[pos], "film", 4 ) ) {
            sv = scanplat_sv_filmphoto_copy_new();
        }
        else if( str_match( argv[pos], "picture", 7 ) ) {
            sv = scanplat_sv_picture_copy_new();
        }
        else {
            cmd_printf( "usage: scan var new copy [draft|text|mixed|film|picture]\n" );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "colorcopy", 9 ) ) {
        pos++;
        if( str_match( argv[pos], "draft", 5 ) ) {
            sv = scanplat_sv_draft_color_copy_new();
        }
        else if( str_match( argv[pos], "text", 4 ) ) {
            sv = scanplat_sv_text_color_copy_new();
        }
        else if( str_match( argv[pos], "mixed", 5 ) ) {
            sv = scanplat_sv_mixed_color_copy_new();
        }
        else if( str_match( argv[pos], "film", 4 ) ) {
            sv = scanplat_sv_filmphoto_color_copy_new();
        }
        else if( str_match( argv[pos], "picture", 7 ) ) {
            sv = scanplat_sv_picture_color_copy_new();
        }
        else {
            cmd_printf( "usage: scan var new colorcopy [draft|text|mixed|film|picture]\n" );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "scan", 4 ) ) {
        pos++;
        if( str_match( argv[pos], "color", 5 ) ) {
            sv = scanplat_sv_color_scan_new();
        }
        else if( str_match( argv[pos], "mono", 4 ) ) {
            sv = scanplat_sv_mono_scan_new();
        }
        else {
            cmd_printf( "usage: scan var new scan [mono|color]\n" );
            return CMD_ERROR;
        }
    }
    else if( str_match( argv[pos], "fax", 3 ) ) {
        pos++;
        if( str_match( argv[pos], "mixed", 5 ) ) {
            sv = scanplat_sv_mixed_fax_new();
        }
        else if( str_match( argv[pos], "text", 4 ) ) {
            sv = scanplat_sv_text_fax_new();
        }
        else {
            cmd_printf( "usage: scan var new fax [mixed|text]\n" );
            return CMD_ERROR;
        }
    }
    else {
        cmd_printf( "usage: scan var new [copy|colorcopy|scan|fax]\n" );
        return CMD_ERROR;
    }

    if( sv==NULL ) {
        cmd_printf( "failed to get scanvar memory\n" );
        return CMD_ERROR;
    }

    XASSERT( g_scanvar==NULL, g_scanvar->id );
    PTR_ASSIGN( g_scanvar, sv );

    return CMD_OK;
}

static const char *scan_cmd_cal_print_desc = "dump the cal cache entry";
static const char *scan_cmd_cal_print_usage = "<hres> mono|color";
static const char *scan_cmd_cal_print_notes = NULL;
static int scan_cmd_cal_print_cb( int argc, char *argv[] )
{
    int retcode;
    int hres;
    uint32_t num32;
    scan_cmode_t cmode;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }

    /* usage: scan cal dprint hres scantype 
     * where scantype is "color" or "mono"
     */

    /* parse hres */
    retcode = str_mkint( argv[1], &num32 );
    if( retcode != 0 )
    {
        cmd_printf("scan: bad integer \"%s\"\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    hres = num32;

    /* parse scan cmode */
    if( str_match( argv[2], "mono", 4 ) )
    {
        cmode = SCAN_CMODE_MONO;
    }
    else if( str_match( argv[2], "color", 5 ) )
    {
        cmode = SCAN_CMODE_COLOR;
    }
    else
    {
        return CMD_USAGE_ERROR;
    }

    /* dump cal cache entry */
    cal_cache_print( 0, hres, cmode ); // hardcode debug sensor to 0

    return CMD_OK;
}

#ifdef HAVE_SCAN_CALGRAPH
static const char *scan_cmd_cal_graph_desc = "run a cal graph job";
static const char *scan_cmd_cal_graph_usage = "<dpi> mono|color <type> <color>";
static const char *scan_cmd_cal_graph_notes = "<type> must be a valid CAL_GRAPH_T enum, <color> must be a valid CAL_GRAPH_COLOR_T enum";
static int scan_cmd_cal_graph_cb( int argc, char *argv[] )
{
    scan_err_t scerr;
    int dpi;
    scan_cmode_t cmode;
    CAL_GRAPH_T cal_graph_type;
    CAL_GRAPH_COLOR_T cal_graph_color;
    uint32_t num32;

    if ( argc != 5 )
    {
        return CMD_USAGE_ERROR;
    }

    /* scan cal graph dpi cmode gtype gcolor 
     *
     * Example:
     *  scan cal graph 300 mono 5 1 
     *
     */

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for dpi\n", argv[1] ); 
        return -1;
    }
    dpi = (int)num32;

    if( str_match( argv[2], "mono", 4 ) )
    {
        cmode = SCAN_CMODE_MONO;
    }
    else if( str_match( argv[2], "color", 5 ) )
    {
        cmode = SCAN_CMODE_COLOR;
    }
    else
    { 
        cmd_printf( "bad value \"%s\" for cmode\n", argv[2] ); 
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[3], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for graph type\n", argv[3] ); 
        return CMD_USAGE_ERROR;
    }
    cal_graph_type = (CAL_GRAPH_T)num32;

    if( str_mkint( argv[4], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for graph color\n", argv[4] ); 
        return CMD_USAGE_ERROR;
    }
    cal_graph_color = (CAL_GRAPH_COLOR_T)num32;

    scerr = scan_cal_graph_run_job( dpi,
                                    cmode,
                                    cal_graph_type,
                                    cal_graph_color, 
                                    CAL_GRAPH_PRINT_DEFAULT );

    if( scerr != 0 )
    {
        cmd_printf( "%s calgrapp_run_calgraph_job() failed with scerr=%d\n",
                    __FUNCTION__, scerr );
        return CMD_ERROR;
    }

    return CMD_OK;
}
#endif

static const char *scan_cmd_cal_flush_desc = "flush the entire cal cache";
static const char *scan_cmd_cal_flush_usage = NULL;
static const char *scan_cmd_cal_flush_notes = NULL;
static int scan_cmd_cal_flush_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_printf( "flushing cal cache\n" );
    cal_cache_flush_all();

    return CMD_OK;
}

static const char * scan_cmd_margin_desc = "set the scanner flatbed margins";
static const char * scan_cmd_margin_usage = "<dpi> mono|color <left x> <top y> <right x> <bottom y>";
static const char * scan_cmd_margin_notes = "The last four values are the margins";
static int scan_cmd_margin_cb( int argc, char *argv[] )
{
    scan_err_t scerr;
    uint32_t num32;
    struct scan_flatbed_margins fb_margins;

    if ( argc != 7 )
    {
        return CMD_USAGE_ERROR;
    }

    /* scan margin [dpi] [mono|color] num1 num2 num3
     *
     * num1 == left x margin
     * num2 == top y margin
     * num3 == right x margin
     */

    /* parse dpi */
    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }
    fb_margins.dpi = num32;

    /* parse scan cmode */
    if( str_match( argv[2], "mono", 4 ) )
    {
        fb_margins.cmode = SCAN_CMODE_MONO;
    }
    else if( str_match( argv[2], "color", 5 ) )
    {
        fb_margins.cmode = SCAN_CMODE_COLOR;
    }
    else
    {
        return CMD_USAGE_ERROR;
    }

    /* get three integers */
    if( str_mkint( argv[3], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }
    fb_margins.left_margin_hinches_x = num32;

    if( str_mkint( argv[4], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }
    fb_margins.top_margin_hinches_y = num32;

    if( str_mkint( argv[5], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }
    fb_margins.right_margin_hinches_x = num32;
    if( str_mkint( argv[6], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }
    fb_margins.bottom_margin_hinches_y = num32;

    scerr = scanmech_debug_set_fb_margins( &fb_margins );
    if( scerr != 0 )
    {
        cmd_printf( "failed to set fb margins dpi=%d cmode=%d values=%d,%d,%d,%d; scerr=%d\n",
                    fb_margins.dpi, fb_margins.cmode,
                    fb_margins.left_margin_hinches_x, 
                    fb_margins.top_margin_hinches_y, 
                    fb_margins.right_margin_hinches_x, 
                    fb_margins.bottom_margin_hinches_y,
                    scerr );
        return CMD_ERROR;
    }

    cmd_printf( "flatbed margins dpi=%d cmode=%d successfully set to %d,%d,%d,%d\n",
                fb_margins.dpi, fb_margins.cmode,
                fb_margins.left_margin_hinches_x, 
                fb_margins.top_margin_hinches_y,
                fb_margins.right_margin_hinches_x,
                fb_margins.bottom_margin_hinches_y );

    return CMD_OK;
}

static void cmd_printf_margins( const struct scan_adf_margins *m )
{
    cmd_printf( "adf margins dpi=%d cmode=%d ftof=%d tof=%d bof=%d left=%d backtof=%d backbof=%d\n",
                m->dpi, m->cmode,
                m->rows_to_first_tof,
                m->rows_to_tof, m->rows_to_bof,
                m->left_margin_hinches, 
                m->rows_to_tof_backside, 
                m->rows_to_bof_backside );
}


static const char *scan_cmd_adf_margin_desc = "adf margin debug tuning";
static const char *scan_cmd_adf_margin_usage = "<dpi> mono|color first_tof tof bof left_margin_hinches";
static int scan_cmd_adf_margin_cb( int argc, char *argv[] )
{
    scan_err_t scerr;
    uint32_t num32;
    struct scan_adf_margins adf_margins;

    /* davep 30-Jul-2013 ; adding cmdline support for duplex margins */

    if ( argc < 7 ) {
        return CMD_USAGE_ERROR;
    }

    /* scan adf margin [dpi] [mono|color] num1 num2 num3 num4 [num5 [num6]]
     *
     * num1 == adf_first_tof
     * num2 == adf_tof
     * num3 == adf_bof
     * num4 == left_margin_hinches
     *
     * (optional)
     * num5 == rows_to_tof_backside (for duplex)
     * num6 == rows_to_bof_backside (for duplex)
     */

    memset( &adf_margins, 0, sizeof(struct scan_adf_margins) );

    /* get the DPI */
    if( str_mkint( argv[1], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.dpi = num32;

    /* get the cmode */
    if( str_match( argv[2], "mono", 4 ) ) {
        adf_margins.cmode = SCAN_CMODE_MONO;
    }
    else if( str_match( argv[2], "color", 5 ) ) {
        adf_margins.cmode = SCAN_CMODE_COLOR;
    }
    else {
        return CMD_USAGE_ERROR;
    }

    /* davep 22-Nov-2011 ; get the current settings for this cmode+dpi; we're
     * overwrite some of the settings but we don't want to kill all of them 
     */
    scerr = scanmech_get_adf_margins( adf_margins.dpi, adf_margins.cmode, &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to get current margins scerr=%d\n", scerr );
        return CMD_ERROR;
    }

    /* get three integers */
    if( str_mkint( argv[3], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.rows_to_first_tof = num32;

    if( str_mkint( argv[4], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.rows_to_tof = num32;

    if( str_mkint( argv[5], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.rows_to_bof = num32;

    if( str_mkint( argv[6], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.left_margin_hinches = num32;

    if( argc > 7 ) {
        if( str_mkint( argv[7], &num32 ) != 0 ) {
            return CMD_USAGE_ERROR;
        }
        adf_margins.rows_to_tof_backside = num32;
    }
    if( argc > 8 ) {
        if( str_mkint( argv[8], &num32 ) != 0 ) {
            return CMD_USAGE_ERROR;
        }
        adf_margins.rows_to_bof_backside = num32;
    }

    scerr = scanmech_debug_set_adf_margins( &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to set adf margins; scerr=%d\n", scerr );
        cmd_printf_margins( &adf_margins );
        return CMD_ERROR;
    }

    cmd_printf( "adf margin set successful; get to verify...\n" );
    scerr = scanmech_get_adf_margins( adf_margins.dpi, adf_margins.cmode, &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to get new margins scerr=%d\n", scerr );
        return CMD_ERROR;
    }

    cmd_printf_margins( &adf_margins );

    return CMD_OK;
}

static const char *scan_cmd_adf_duplex_margin_desc = "adf duplex margin debug tuning";
static const char *scan_cmd_adf_duplex_margin_usage = "dpi mono|color duplex_tof duplex_bof";
static int scan_cmd_adf_duplex_margin_cb( int argc, char *argv[] )
{
    scan_err_t scerr;
    uint32_t num32;
    struct scan_adf_margins adf_margins;
    int i;

    for( i=0 ; i<argc ; i++ ) {
        dbg2( "%s i=%d arg=%s\n", __FUNCTION__, i, argv[i] );
    }

    if( argc != 5 ) {
        return CMD_USAGE_ERROR;
    }

    memset( &adf_margins, 0, sizeof(struct scan_adf_margins) );

    /* get the DPI */
    if( str_mkint( argv[1], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.dpi = num32;

    /* get the cmode */
    if( str_match( argv[2], "mono", 4 ) ) {
        adf_margins.cmode = SCAN_CMODE_MONO;
    }
    else if( str_match( argv[2], "color", 5 ) ) {
        adf_margins.cmode = SCAN_CMODE_COLOR;
    }
    else {
        return CMD_USAGE_ERROR;
    }

    /* davep 22-Nov-2011 ; get the current settings for this cmode+dpi; we're
     * overwrite some of the settings but we don't want to kill all of them 
     */
    scerr = scanmech_get_adf_margins( adf_margins.dpi, adf_margins.cmode, &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to get current margins scerr=%d\n", scerr );
        return CMD_ERROR;
    }

    /* get the backside tof */
    if( str_mkint( argv[3], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.rows_to_tof_backside = num32;

    /* get the backside bof */
    if( str_mkint( argv[4], &num32 ) != 0 ) {
        return CMD_USAGE_ERROR;
    }
    adf_margins.rows_to_bof_backside = num32;

    /* save the new margins */
    scerr = scanmech_debug_set_adf_margins( &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to set adf margins; scerr=%d\n", scerr );
        cmd_printf_margins( &adf_margins );
        return CMD_ERROR;
    }

    /* get the margins to verify saved correctly */
    scerr = scanmech_get_adf_margins( adf_margins.dpi, adf_margins.cmode, &adf_margins );
    if( scerr!=SCANERR_NONE ) {
        cmd_printf( "failed to get new margins; scerr=%d\n", scerr );
        return CMD_ERROR;
    }

    cmd_printf_margins( &adf_margins );

    return CMD_OK;
}


static const char *scan_cmd_adf_pip_desc = "determine if there is paper in the ADF path";
static const char *scan_cmd_adf_pip_usage = NULL;
static const char *scan_cmd_adf_pip_notes = "this command appends 1 if paper is in the ADF path, 0 otherwise";
static int scan_cmd_adf_pip_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "%d", adf_sensor_paper_in_path() );

    return CMD_OK;
}

static const char *scan_cmd_pipetap_off_desc = "set the PIPETAP_NONE flag";
static const char *scan_cmd_pipetap_off_usage = NULL;
static const char *scan_cmd_pipetap_off_notes = NULL;
static int scan_cmd_pipetap_off_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "off" );
    pipe_set_pipetap( PIPETAP_NONE );

    return CMD_OK;
}

static const char *scan_cmd_pipetap_pic_desc = "set the PIPETAP_PIC flag";
static const char *scan_cmd_pipetap_pic_usage = NULL;
static const char *scan_cmd_pipetap_pic_notes = NULL;
static int scan_cmd_pipetap_pic_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "pic" );
    pipe_set_pipetap( PIPETAP_PIC );

    return CMD_OK;
}

static const char *scan_cmd_pipetap_pie_desc = "set the PIPETAP_PIE flag";
static const char *scan_cmd_pipetap_pie_usage = NULL;
static const char *scan_cmd_pipetap_pie_notes = NULL;
static int scan_cmd_pipetap_pie_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "pie" );
    pipe_set_pipetap( PIPETAP_PIE );

    return CMD_OK;
}

static const char *scan_cmd_pipetap_hp_desc = "set the PIPETAP_HP flag";
static const char *scan_cmd_pipetap_hp_usage = NULL;
static const char *scan_cmd_pipetap_hp_notes = NULL;
static int scan_cmd_pipetap_hp_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "hp" );
    pipe_set_pipetap( PIPETAP_HP );

    return CMD_OK;
}

static const char *scan_cmd_pipetap_cutter_desc = "set the PIPETAP_CUTTER flag";
static const char *scan_cmd_pipetap_cutter_usage = NULL;
static const char *scan_cmd_pipetap_cutter_notes = NULL;
static int scan_cmd_pipetap_cutter_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_append_result( "cutter" );
    pipe_set_pipetap( PIPETAP_CUTTER );

    return CMD_OK;
}

static const char *scan_cmd_ds_dump_desc = "dump the scan data store";
static const char *scan_cmd_ds_dump_usage = NULL;
static const char *scan_cmd_ds_dump_notes = NULL;
static int scan_cmd_ds_dump_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    scands_debug_dump();

    return CMD_OK;
}

static const char *scan_cmd_ds_get_desc = "get a scan data store variable using its name";
static const char *scan_cmd_ds_get_usage = "<name>";
static const char *scan_cmd_ds_get_notes = "<name> is a scan data store variable name, the value is appended as a result";
static int scan_cmd_ds_get_cb( int argc, char *argv[] )
{
    uint32_t value;
    scan_err_t scerr;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    scerr = scands_get_integer_by_name( argv[1], &value );

    switch( scerr )
    {
        case SCANERR_NONE :
            cmd_append_result( "%"PRIu32"", value );
            break;

        case SCANERR_NO_ENTRY :
            cmd_printf( "%s get failed; no scands var with that name\n", __FUNCTION__ );
            break;

        default : 
            cmd_printf( "%s get failed err=%d\n", __FUNCTION__, scerr );
            XASSERT( 0, scerr );
            return CMD_ERROR;
            break;
    }

    return CMD_OK;
}

static const char *scan_cmd_ds_set_desc = "set a scan data store variable using its name and new value";
static const char *scan_cmd_ds_set_usage = "<name> <value>";
static const char *scan_cmd_ds_set_notes = "<name> is a scan data store variable name, <value> is the new value it should take on";
static int scan_cmd_ds_set_cb( int argc, char *argv[] )
{
    char *name_ptr;
    scan_err_t scerr;
    uint32_t value;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }

    name_ptr = argv[1];

    if( str_mkint( argv[2], &value ) != 0 )
    {
        cmd_printf( "%s invalid integer\n", __FUNCTION__ );
        return CMD_USAGE_ERROR;
    }

    scerr = scands_set_integer_by_name( name_ptr, value);
    if( scerr != SCANERR_NONE )
    {
        cmd_printf( "%s set failed err=%d\n", __FUNCTION__, scerr );
        XASSERT( 0, scerr );
        return CMD_ERROR;
    }

    return CMD_OK;
}

static const char *scan_cmd_ds_delete_desc = "delete a scan data store variable by name";
static const char *scan_cmd_ds_delete_usage = "<name>";
static const char *scan_cmd_ds_delete_notes = "<name> is a scan data store variable name";
static int scan_cmd_ds_delete_cb( int argc, char *argv[] )
{
    scan_err_t scerr;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    scerr = scands_delete_integer_by_name( argv[1] );

    switch( scerr )
    {
        case SCANERR_NONE :
            break;

        case SCANERR_NO_ENTRY :
            cmd_printf( "%s delete failed; no scands var with that name\n", __FUNCTION__ );
            return CMD_ERROR;
            break;

        default : 
            cmd_printf( "%s delete failed err=%d\n", __FUNCTION__, scerr );
            XASSERT( 0, scerr );
            return CMD_ERROR;
            break;
    }

    return CMD_OK;
}

static const char *scan_cmd_pause_desc = "set the client pause flag";
static const char *scan_cmd_pause_usage = "";
static const char *scan_cmd_pause_notes = "flag must be either 0 or 1";
static int scan_cmd_pause_cb( int argc, char *argv[] )
{
    uint32_t num32;
    scan_err_t scerr;

    if ( argc != 2 ) {
        return CMD_USAGE_ERROR;
    }

    scerr = SCANERR_NONE;

    if( str_mkint( argv[1], &num32 ) != 0 ) {
        cmd_printf( "bad integer \"%s\" for flag\n", argv[1] );
        return CMD_USAGE_ERROR;
    }

    if( num32==0 ) {
        scerr = scanmech_adf_client_clear_pause();
    }
    else if( num32==1 ) {
        scerr = scanmech_adf_client_request_pause();
    }
    else {
        /* ignore */
        return CMD_USAGE_ERROR;
    }

    if( scerr != SCANERR_NONE ) {
        cmd_printf( "%s failed scerr=%d\n", __FUNCTION__, scerr );
        return CMD_ERROR;
    }

    return CMD_OK;
}

#ifdef HAVE_PIC_PSESD
static const char *scan_cmd_esd_dir_desc = "set the esd <direction> setting";
static const char *scan_cmd_esd_dir_usage = "<direction>";
static const char *scan_cmd_esd_dir_notes = "<direction> must be either 0 or 1";
static int scan_cmd_esd_dir_cb( int argc, char *argv[] )
{
    bool esd_dir;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    esd_dir = false;
    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    if( num32 == 1 )
    {
        esd_dir = true;
    }
    else if ( 0 == num32 )
    {
        esd_dir = false;
    }
    else
    {
        return CMD_USAGE_ERROR;
    }

    scanplat_debug_set_esd_settings(&esd_dir, NULL, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_esd_size_desc = "set the esd <size> setting";
static const char *scan_cmd_esd_size_usage = "<size>";
static const char *scan_cmd_esd_size_notes = NULL;
static int scan_cmd_esd_size_cb( int argc, char *argv[] )
{
    uint32_t num32;
    uint16_t esd_accum_size;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    esd_accum_size = 0;

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    esd_accum_size = num32;

    scanplat_debug_set_esd_settings(NULL, &esd_accum_size, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_esd_start_desc = "set the esd <start> setting";
static const char *scan_cmd_esd_start_usage = "<start>";
static const char *scan_cmd_esd_start_notes = NULL;
static int scan_cmd_esd_start_cb( int argc, char *argv[] )
{
    uint16_t esd_start;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    esd_start = 0;

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    esd_start = num32;

    scanplat_debug_set_esd_settings(NULL, NULL, &esd_start, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_esd_change_desc = "set the esd <change> setting";
static const char *scan_cmd_esd_change_usage = "<change>";
static const char *scan_cmd_esd_change_notes = NULL;
static int scan_cmd_esd_change_cb( int argc, char *argv[] )
{
    uint8_t esd_change;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }
    
    esd_change = 0;

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    esd_change = num32;

    scanplat_debug_set_esd_settings(NULL, NULL, NULL, &esd_change, NULL);

    return CMD_OK;
}

static const char *scan_cmd_esd_tol_desc = "set the esd <tol> setting";
static const char *scan_cmd_esd_tol_usage = "<tol>";
static const char *scan_cmd_esd_tol_notes = NULL;
static int scan_cmd_esd_tol_cb( int argc, char *argv[] )
{
    uint8_t esd_tol;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    esd_tol = 0;

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf( "bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    esd_tol = num32;

    scanplat_debug_set_esd_settings(NULL, NULL, NULL, NULL, &esd_tol);

    return CMD_OK;
}

static const char *scan_cmd_psense_start_desc = "set the psense <start> setting";
static const char *scan_cmd_psense_start_usage = "<start>";
static const char *scan_cmd_psense_start_notes = NULL;
static int scan_cmd_psense_start_cb( int argc, char *argv[] )
{
    uint16_t ps_start;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    ps_start = 0;

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_start = (uint16_t)num32;
    scanplat_debug_set_psense_settings(&ps_start, NULL, NULL, NULL, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_tol0_desc = "set the psense tol0 setting";
static const char *scan_cmd_psense_tol0_usage = "<tol0>";
static const char *scan_cmd_psense_tol0_notes = NULL;
static int scan_cmd_psense_tol0_cb( int argc, char *argv[] )
{
    uint8_t ps_tol0;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_tol0 = num32;

    scanplat_debug_set_psense_settings(NULL, &ps_tol0, NULL, NULL, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_tol1_desc = "set the psense tol1 setting";
static const char *scan_cmd_psense_tol1_usage = "<tol1>";
static const char *scan_cmd_psense_tol1_notes = NULL;
static int scan_cmd_psense_tol1_cb( int argc, char *argv[] )
{
    uint8_t ps_tol1;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_tol1 = num32;

    scanplat_debug_set_psense_settings(NULL, NULL, &ps_tol1, NULL, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_tog01_desc = "set the psense tog01 setting";
static const char *scan_cmd_psense_tog01_usage = "<tog01>";
static const char *scan_cmd_psense_tog01_notes = NULL;
static int scan_cmd_psense_tog01_cb( int argc, char *argv[] )
{
    uint8_t ps_tog01;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_tog01 = num32;

    scanplat_debug_set_psense_settings(NULL, NULL, NULL, &ps_tog01, NULL, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_tog10_desc = "set the psense tog10 setting";
static const char *scan_cmd_psense_tog10_usage = "<tog10>";
static const char *scan_cmd_psense_tog10_notes = NULL;
static int scan_cmd_psense_tog10_cb( int argc, char *argv[] )
{
    uint8_t ps_tog10;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_tog10 = num32;

    scanplat_debug_set_psense_settings(NULL, NULL, NULL, NULL, &ps_tog10, NULL, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_cnt0_desc = "set the psense cnt0 setting";
static const char *scan_cmd_psense_cnt0_usage = "<cnt0>";
static const char *scan_cmd_psense_cnt0_notes = NULL;
static int scan_cmd_psense_cnt0_cb( int argc, char *argv[] )
{
    uint8_t ps_cnt0;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_cnt0 = num32;

    scanplat_debug_set_psense_settings(NULL, NULL, NULL, NULL, NULL, &ps_cnt0, NULL);

    return CMD_OK;
}

static const char *scan_cmd_psense_cnt1_desc = "set the psense cnt1 setting";
static const char *scan_cmd_psense_cnt1_usage = "<cnt1>";
static const char *scan_cmd_psense_cnt1_notes = NULL;
static int scan_cmd_psense_cnt1_cb( int argc, char *argv[] )
{
    uint8_t ps_cnt1;
    uint32_t num32;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        cmd_printf("bad integer \"%s\" for width\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
    ps_cnt1 = num32;

    scanplat_debug_set_psense_settings(NULL, NULL, NULL, NULL, NULL, NULL, &ps_cnt1);

    return CMD_OK;
}
#endif

static const char *scan_cmd_start_desc = "start a scan with your current scanvar settings";
static const char *scan_cmd_start_usage = NULL;
static const char *scan_cmd_start_notes = NULL;
static int scan_cmd_start_cb( int argc, char *argv[] )
{
    uint32_t scan_task_id;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    if( g_scanvar == NULL )
    {
        cmd_printf( "No active scanvar. Start ignored.\n" );
        return CMD_ERROR;
    }

    /* send a message to the scanning task telling it to start a scan */
    scantask_scan_start( g_scanvar, &scan_task_id );

    return CMD_OK;
}

static int scan_cmd_test_cb( int argc, char *argv[] )
{
    int pos;
    scan_msg_t msg;
    scan_err_t scerr;
    
    memset( &msg, 0, sizeof(scan_msg_t) );

    if ( argc < 2 ) {
        return CMD_USAGE_ERROR;
    }

    msg.msgType = SMSG_SCAN_DEBUG;
    msg.param1 = SCAN_DBGMSG_SCAN;

    pos = 1;
    if( str_match(argv[pos],"sensor",6) ) {
        msg.param2 = SCAN_DBGMSG_TEST_SENSOR; 
    }
    else if( str_match( argv[pos], "finger", 6 ) ) {
        msg.param2 = SCAN_DBGMSG_TEST_FINGER_TEST; 
    }
    else if( str_match(argv[pos],"scanlib",7) ) {
        msg.param2 = SCAN_DBGMSG_TEST_SCANLIB; 
    }
    else if( str_match(argv[pos],"scands",6) ) {
        msg.param2 = SCAN_DBGMSG_TEST_SCANDS; 
    }
    else if( str_match(argv[pos],"scantask",8) ) {
        msg.param2 = SCAN_DBGMSG_TEST_TASK; 
    }
    else if( str_match(argv[pos],"pic",3) ) {
        msg.param2 = SCAN_DBGMSG_TEST_PIC; 
    }
    else if( str_match(argv[pos],"scanvars",8) ) {
        msg.param2 = SCAN_DBGMSG_TEST_SCANVARS; 
    }
    else if( str_match(argv[pos],"cisx",4) ) {
        msg.param2 = SCAN_DBGMSG_TEST_CISX; 
    }
    else if( str_match(argv[pos],"pie",3) ) {
        msg.param2 = SCAN_DBGMSG_TEST_PIE; 
    }
    else if( str_match(argv[pos],"afe",3) ) {
        msg.param2 = SCAN_DBGMSG_TEST_AFE; 
    }
    else {
        cmd_printf( "unknown test \"%s\"\n", argv[pos] );
        return CMD_USAGE_ERROR;
    }

    scerr = scantask_debug_msg_send( &msg );
    XASSERT( scerr==0, scerr );
    return CMD_OK;
}

static const char *scan_cmd_afe_regs_desc = "dump some afe registers";
static const char *scan_cmd_afe_regs_usage = NULL;
static const char *scan_cmd_afe_regs_notes = NULL;
static int scan_cmd_afe_regs_cb( int argc, char *argv[] )
{
    scan_msg_t msg;
    scan_err_t scerr;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    msg.msgType = SMSG_SCAN_DEBUG;
    msg.param1 = SCAN_DBGMSG_DUMP_AFE;
    scerr = scantask_debug_msg_send( &msg );
    XASSERT( scerr==0, scerr );

    return CMD_OK;
}

static const char *scan_cmd_regs_desc = "dump some scan registers";
static const char *scan_cmd_regs_usage = NULL;
static const char *scan_cmd_regs_notes = NULL;
static int scan_cmd_regs_cb( int argc, char *argv[] )
{
    if ( argc != 1 ) {
        return CMD_USAGE_ERROR;
    }

    /* davep 15-Nov-2012 ; dump the regs from the debug thread (necessary debug
     * tool)
     */
    scif_dump();

    return CMD_OK;
}

static const char *scan_cmd_repost_desc = "redo the scan hardware power on self test";
static const char *scan_cmd_repost_usage = NULL;
static const char *scan_cmd_repost_notes = NULL;
static int scan_cmd_repost_cb( int argc, char *argv[] )
{
    scan_msg_t msg;
    scan_err_t scerr;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    msg.msgType = SMSG_SCAN_DEBUG;
    msg.param1 = SCAN_DBGMSG_REPOST;
    scerr = scantask_debug_msg_send( &msg );
    XASSERT( scerr==0, scerr );

    return CMD_OK;
}

static const char *scan_cmd_notch_desc = "go do the scan notch find";
static const char *scan_cmd_notch_usage = NULL;
static const char *scan_cmd_notch_notes = NULL;
static int scan_cmd_notch_cb( int argc, char *argv[] )
{
    scan_msg_t msg;
    scan_err_t scerr;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    msg.msgType = SMSG_SCAN_DEBUG;
    msg.param1 = SCAN_DBGMSG_NOTCH;
    scerr = scantask_debug_msg_send( &msg );
    XASSERT( scerr==0, scerr );

    return CMD_OK;
}

static int scan_cmd_cisx_cb( int argc, char *argv[] )
{
    if( argc==2 && str_match(argv[1],"regs",4) ) {
#ifdef HAVE_CISX
        cisx_dump();
#endif
    }
    else {
        return CMD_USAGE_ERROR;
    }

    return CMD_OK;
}

static const char *scan_cmd_pic_regs_desc = "dumps the PIC registers";
static const char *scan_cmd_pic_regs_usage = NULL;
static const char *scan_cmd_pic_regs_notes = NULL;
static int scan_cmd_pic_regs_cb( int argc, char *argv[] )
{
    if ( argc != 1 ) {
        return CMD_USAGE_ERROR;
    }

    /* davep 15-Nov-2012 ; dump the pic regs from the debug thread context */
    pic_dump();

    return CMD_OK;
}

static const char *scan_cmd_ip_desc = "sets the ip address where scan will send its data";
static const char *scan_cmd_ip_usage = "<ip>";
static const char *scan_cmd_ip_notes = "<ip> is in the format www.xxx.yyy.zzz, for example 172.31.1.34";
static int scan_cmd_ip_cb( int argc, char *argv[] )
{
    scan_msg_t msg;
    scan_err_t scerr;
    uint32_t ip;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    msg.msgType = SMSG_SCAN_DEBUG;
    msg.param1 = SCAN_DBGMSG_IP;

    /* example: "scan debug ip 172.31.1.34" */
    /* validate IP address, convert to integer, and pass to task telling
     * where to send its debug data
     */
#ifdef HAVE_NETWORK 
    if( mkip(argv[1], &ip) != 0 )
    {
        /* bad IP address */
        cmd_printf( "bad IP address \"%s\"\n", argv[1] );
        return CMD_USAGE_ERROR;
    }
#else
    cmd_printf( "no network support\n" );
    return CMD_ERROR;
#endif

    msg.param2 = ip;

    scerr = scantask_debug_msg_send( &msg );
    XASSERT( scerr==0, scerr );

    return CMD_OK;
}

static int scan_cmd_pie_cb( int argc, char *argv[] )
{
    DBG_PRINT_ARGC_ARGV 

    if( argc==2 && str_match(argv[1],"cstats",6) ) {
        pie_cstats_dump();
    }

    return CMD_OK;
}

static const char *scan_cmd_var_pop_desc = "deletes the current active scanvar";
static const char *scan_cmd_var_pop_usage = NULL;
static const char *scan_cmd_var_pop_notes = NULL;
static int scan_cmd_var_pop_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    if( g_scanvar == NULL )
    {
        cmd_printf( "No active scanvar. Pop ignored.\n" );
        return CMD_ERROR;
    }

    scanvar_delete( &g_scanvar );
    XASSERT( g_scanvar==NULL, (uint32_t)g_scanvar );

    return CMD_OK;
}

static const char *scan_cmd_var_dump_desc = "dumps the scanvar stack";
static const char *scan_cmd_var_dump_usage = NULL;
static const char *scan_cmd_var_dump_notes = NULL;
static int scan_cmd_var_dump_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    /* dump the scanvar stack */
    scanvar_stackdump();

    return CMD_OK;
}

static const char *scan_cmd_var_print_desc = "prints information about the active scanvar";
static const char *scan_cmd_var_print_usage = NULL;
static const char *scan_cmd_var_print_notes = NULL;
static int scan_cmd_var_print_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    if( g_scanvar == NULL )
    {
        cmd_printf( "No active scanvar. Print ignored.\n" );
        return CMD_ERROR;
    }

    scanvar_to_siqfile( g_scanvar );

    return CMD_OK;
}

static const char *scan_cmd_config_desc = "set 'sticky' config flags, flags that effect every scan";
static const char *scan_cmd_config_usage = "<flag 1> [<flag 2> <flag 3> ...]";
static const char *scan_cmd_config_notes = "You may specify any or all flags, each seperated by a space. flags may be any of the following: pipenet|nocal|icefile|nopie|noleds|calonly|nobpr|nojobnotch|scanfromhome|fakescan|noprnu|nodsnu|noquad|noesd|nops|nopc|ppon|ppoff|pipedrain|prtest|noxmargins|nocisx";
static int scan_cmd_config_cb( int argc, char *argv[] )
{
    int pos;

    if ( argc < 2 )
    {
        return CMD_USAGE_ERROR;
    }

    /* first turn everything off then enable only the flags we want */
    cmd_printf( "disabling all sticky config flags\n" );

    /* turn off all the debug/test flags */
    scan_set_config( SCAN_CONFIG_ALL_DISABLED );

    for( pos = 1; pos < argc ; pos++ )
    {
        if( str_match( argv[pos], "pipenet", 7 ) )
        {
            cmd_printf( "enabling sticky config network read\n" );
            scan_set_config_pipenet( true );
        }
        else if( str_match( argv[pos], "nocal", 5 ) )
        {
            cmd_printf( "enabling calibration skipping\n" );
            scan_set_config_nocal( true );
        }
        else if( str_match( argv[pos], "icefile", 7 ) )
        {
            cmd_printf( "enabling icefile\n" );
            scan_set_config_icefile( true );
        }
        else if( str_match( argv[pos], "nopie", 5 ) )
        {
            cmd_printf( "enabling PIE skipping\n" );
            scan_set_config_nopie( true );
        }
        else if( str_match( argv[pos], "noleds", 6 ) )
        {
            cmd_printf( "enabling dark scans\n" );
            scan_set_config_noleds( true );
        }
        else if( str_match( argv[pos], "calonly", 7 ) )
        {
            cmd_printf( "stopping after calibration\n" );
            scan_set_config_cal_only( true );
        }
        else if( str_match( argv[pos], "nobpr", 5 ) )
        {
            cmd_printf( "disabling bad pixel replacement\n" );
            scan_set_config_nobpr( true );
        }
        else if( str_match( argv[pos], "nojobnotch", 10 ) )
        {
            cmd_printf( "disabling job notchfind\n" );
            scan_set_config_nojobnotch( true );
        }
        else if( str_match( argv[pos], "scanfromhome", 12 ) )
        {
            cmd_printf( "enabling scan from home\n" );
            scan_set_config_scan_from_home( true );
        }
        else if( str_match( argv[pos], "fakescan", 8 ) )
        {
            cmd_printf( "enabling scanning using fake data\n" );
            scan_set_config_fakescan( true );
        }
        else if( str_match( argv[pos], "noprnu", 6 ) )
        {
            cmd_printf( "disabling PRNU correction\n" );
            scan_set_config_no_prnu( true );
        }
        else if( str_match( argv[pos], "nodsnu", 6 ) )
        {
            cmd_printf( "disabling DSNU correction\n" );
            scan_set_config_no_dsnu( true );
        }
        else if( str_match( argv[pos], "noquad", 6 ) )
        {
            cmd_printf( "disabling QUAD correction\n" );
            scan_set_config_no_quad( true );
        }
        else if( str_match( argv[pos], "noesd", 5 ) )
        {
            cmd_printf( "disabling ESD detection\n" );
            scan_set_config_no_esd( true );
        }
        else if( str_match( argv[pos], "nops", 4 ) )
        {
            cmd_printf( "disabling ESD detection\n" );
            scan_set_config_no_psense( true );
        }
        else if( str_match( argv[pos], "nopc", 4 ) )
        {
            cmd_printf( "disabling all pixel correction\n" );
            scan_set_config_no_pc( true );
        }
        else if( str_match( argv[pos], "ppon", 4 ) )
        {
            cmd_printf( "paper present forced ON\n" );
            scan_set_config_force_pp( true );
            scan_set_config_pp_position( true );
        }
        else if( str_match( argv[pos], "ppoff", 5 ) )
        {
            cmd_printf( "paper present forced OFF\n" );
            scan_set_config_force_pp( true );
            scan_set_config_pp_position( false );
        }
        else if( str_match( argv[pos], "pipedrain", 9 ) )
        {
            cmd_printf( "scanpipe will not renew memory\n" );
            scan_set_config_pipe_drain( true );
        }
        else if( str_match( argv[pos], "prtest", 6 ) )
        {
            cmd_printf( "enabling pause/resume test mode\n" );
            scan_set_config_pipe_drain( true );
            scan_set_config_pr_test( true );
        }
        else if( str_match( argv[pos], "noxmargins", 10 ) )
        {
            cmd_printf( "PIC margins will use max width\n" );
            scan_set_config_no_xmargins( true );
        }
        else if( str_match( argv[pos], "nocisx", 6 ) )
        {
            cmd_printf( "disable CISX\n" );
            scan_set_config_no_cisx( true );
        }
        else if( str_match( argv[pos], "none", 4 ) )
        {
            /* davep 25-Mar-2013 ; adding 'none' so we don't get the confusing
             * error on clearing scan config flags. Can now tell users to
             * 'scan config none' to clear all flags.
             */
            scan_set_config( SCAN_CONFIG_ALL_DISABLED );
            break;
        }
        else
        {
            cmd_printf( "unknown sticky config flag \"%s\"\n", argv[pos] );
            return CMD_USAGE_ERROR;
        }
    }

    return CMD_OK;
}

static const char *scan_cmd_cancel_desc = "cancel the current scan";
static const char *scan_cmd_cancel_usage = NULL;
static const char *scan_cmd_cancel_notes = "A scan cancel is NOT NOT NOT \"Scan Job Cancel\". This scan cancel ONLY sends an internal message to scantask telling it to cancel. Canceling an official job is elsewhere.";
static int scan_cmd_cancel_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    /* A scan cancel is NOT NOT NOT  "Scan Job Cancel". This scan cancel
     * ONLY sends an internal message to scantask telling it to cancel.
     * Canceling an official job is elsewhere.
     */
    scantask_cancel_msg();

    return CMD_OK;
}

static const char *scan_cmd_jobcancel_desc = "cancel the system job";
static const char *scan_cmd_jobcancel_usage = NULL;
static const char *scan_cmd_jobcancel_notes = "note that this may be any system job, not just scan/copy related";
static int scan_cmd_jobcancel_cb( int argc, char *argv[] )
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    /* send a user cancel message to the job manager */
    cancel_job();

    return CMD_OK;
}

#ifdef HAVE_SCAN_ICEFILE_SUPPORT
static const char *scan_cmd_icefile_start_desc = "start the icefile app";
static const char *scan_cmd_icefile_start_usage = NULL;
static const char *scan_cmd_icefile_start_notes = NULL;
static int scan_cmd_icefile_start_cb( int argc, char *argv[] )
{
    scan_err_t scerr;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    scerr = icefileapp_run_internal_page_msg();
    if ( scerr != SCANERR_NONE )
    {
        return CMD_ERROR;
    }

    return CMD_OK;
}
#endif

#ifdef HAVE_SCAN_CONTINUOUS
static const char *scan_cmd_cont_start_desc = "continous scans, a test/debug tool";
static const char *scan_cmd_cont_start_usage = "scan cont [start|stop|status]";
static const char *scan_cmd_cont_start_notes = NULL;
static int scan_cmd_cont_start_cb( int argc, char *argv[] )
{
    int pos;
    bool is_running;
    int max;
    int count;

    if ( argc != 2 ) {
        return CMD_USAGE_ERROR;
    }

    pos = 1;

    if( str_match( argv[pos], "start", 5 ) ) {
        /* pass zero to run forever */
        scan_continuous_start( 0 );
    }
    else if( str_match( argv[pos], "stop", 4 ) ) {
        scan_continuous_stop();
    }
    else if( str_match( argv[pos], "status", 6 ) ) {
        scan_continuous_get_status( &is_running, &max, &count );
        cmd_printf( "%s running=%d max=%d count=%d\n", __FUNCTION__, is_running, max, count);
    }
    else {
        cmd_printf( "usage: scan cont [start|stop|status]\n" );
    }

    return CMD_OK;
}
#endif

static const char *scan_cmd_log_desc = "set the scan log level";
static const char *scan_cmd_log_usage = "<log level>";
static const char *scan_cmd_log_notes = "typical <log level> options are 0, 1, 2, and 3. 2 is quite noisy.";
static int scan_cmd_log_cb( int argc, char *argv[] )
{
    uint32_t num32;
    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    if( str_mkint( argv[1], &num32 ) != 0 )
    {
        return CMD_USAGE_ERROR;
    }

    scanlog_set_level( num32 );

    return CMD_OK;
}

static int scan_cmd_mem_cb( int argc, char *argv[] )
{
    uint32_t size, free, delta;

    size = scanmem_get_heap_size(); 
    free = scanmem_get_free_size();
    delta = size - free;
    cmd_printf( "scan memory maxheap=%" PRIu32 " freeheap=%" PRIu32 " delta=%" PRIu32 " (Grain of salt required.)\n", 
            size, free, delta );
    return CMD_OK;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 26-Aug-2008
 *
 */

void scan_cmdline_init( void )
{
    int retcode;
    retcode = cmd_register_cmd( "scan",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    XASSERT( retcode==CMD_OK, retcode );

    /* singular cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "start",
                                   scan_cmd_start_desc,
                                   scan_cmd_start_usage,
                                   scan_cmd_start_notes,
                                   scan_cmd_start_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "regs",
                                   scan_cmd_regs_desc,
                                   scan_cmd_regs_usage,
                                   scan_cmd_regs_notes,
                                   scan_cmd_regs_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "repost",
                                   scan_cmd_repost_desc,
                                   scan_cmd_repost_usage,
                                   scan_cmd_repost_notes,
                                   scan_cmd_repost_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "notch",
                                   scan_cmd_notch_desc,
                                   scan_cmd_notch_usage,
                                   scan_cmd_notch_notes,
                                   scan_cmd_notch_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "ip",
                                   scan_cmd_ip_desc,
                                   scan_cmd_ip_usage,
                                   scan_cmd_ip_notes,
                                   scan_cmd_ip_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "config",
                                   scan_cmd_config_desc,
                                   scan_cmd_config_usage,
                                   scan_cmd_config_notes,
                                   scan_cmd_config_cb);
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "cancel",
                                   scan_cmd_cancel_desc,
                                   scan_cmd_cancel_usage,
                                   scan_cmd_cancel_notes,
                                   scan_cmd_cancel_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "log",
                                   scan_cmd_log_desc,
                                   scan_cmd_log_usage,
                                   scan_cmd_log_notes,
                                   scan_cmd_log_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "margin",
                                   scan_cmd_margin_desc,
                                   scan_cmd_margin_usage,
                                   scan_cmd_margin_notes,
                                   scan_cmd_margin_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan",
                                   "jobcancel",
                                   scan_cmd_jobcancel_desc,
                                   scan_cmd_jobcancel_usage,
                                   scan_cmd_jobcancel_notes,
                                   scan_cmd_jobcancel_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* cal cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "cal",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan cal",
                                   "print",
                                   scan_cmd_cal_print_desc,
                                   scan_cmd_cal_print_usage,
                                   scan_cmd_cal_print_notes,
                                   scan_cmd_cal_print_cb );
    XASSERT( retcode==CMD_OK, retcode );
#ifdef HAVE_SCAN_CALGRAPH
    retcode = cmd_register_subcmd( "scan cal",
                                   "graph",
                                   scan_cmd_cal_graph_desc,
                                   scan_cmd_cal_graph_usage,
                                   scan_cmd_cal_graph_notes,
                                   scan_cmd_cal_graph_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif

    retcode = cmd_register_subcmd( "scan cal",
                                   "flush",
                                   scan_cmd_cal_flush_desc,
                                   scan_cmd_cal_flush_usage,
                                   scan_cmd_cal_flush_notes,
                                   scan_cmd_cal_flush_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* set cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "set",
                                   NULL,
                                   NULL,
                                   NULL,
                                   scan_cmd_set_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* test cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "test",
                                   NULL,
                                   NULL,
                                   NULL,
                                   scan_cmd_test_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* afe cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "afe",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan afe",
                                   "regs",
                                   scan_cmd_afe_regs_desc,
                                   scan_cmd_afe_regs_usage,
                                   scan_cmd_afe_regs_notes,
                                   scan_cmd_afe_regs_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* cisx commands */
    retcode = cmd_register_subcmd( "scan",
                                   "cisx",
                                   NULL,
                                   NULL,
                                   NULL,
                                   scan_cmd_cisx_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* pic cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "pic",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pic",
                                   "regs",
                                   scan_cmd_pic_regs_desc,
                                   scan_cmd_pic_regs_usage,
                                   scan_cmd_pic_regs_notes,
                                   scan_cmd_pic_regs_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* pie cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "pie",
                                   NULL,
                                   NULL,
                                   NULL,
                                   scan_cmd_pie_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* var cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "var",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var",
                                   "pop",
                                   scan_cmd_var_pop_desc,
                                   scan_cmd_var_pop_usage,
                                   scan_cmd_var_pop_notes,
                                   scan_cmd_var_pop_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var",
                                   "dump",
                                   scan_cmd_var_dump_desc,
                                   scan_cmd_var_dump_usage,
                                   scan_cmd_var_dump_notes,
                                   scan_cmd_var_dump_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var",
                                   "print",
                                   scan_cmd_var_print_desc,
                                   scan_cmd_var_print_usage,
                                   scan_cmd_var_print_notes,
                                   scan_cmd_var_print_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* var new cmds */
    retcode = cmd_register_subcmd( "scan var",
                                   "new",
                                   NULL,
                                   NULL,
                                   NULL,
                                   scan_cmd_var_new_cb );
    XASSERT( retcode==CMD_OK, retcode );
#if 0
    /* var new copy cmds */
    retcode = cmd_register_subcmd( "scan var new",
                                   "copy",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new copy",
                                   "text",
                                   scan_cmd_var_new_copy_text_desc,
                                   scan_cmd_var_new_copy_text_usage,
                                   scan_cmd_var_new_copy_text_notes,
                                   scan_cmd_var_new_copy_text_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new copy",
                                   "draft",
                                   scan_cmd_var_new_copy_draft_desc,
                                   scan_cmd_var_new_copy_draft_usage,
                                   scan_cmd_var_new_copy_draft_notes,
                                   scan_cmd_var_new_copy_draft_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new copy",
                                   "mixed",
                                   scan_cmd_var_new_copy_mixed_desc,
                                   scan_cmd_var_new_copy_mixed_usage,
                                   scan_cmd_var_new_copy_mixed_notes,
                                   scan_cmd_var_new_copy_mixed_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new copy",
                                   "film",
                                   scan_cmd_var_new_copy_film_desc,
                                   scan_cmd_var_new_copy_film_usage,
                                   scan_cmd_var_new_copy_film_notes,
                                   scan_cmd_var_new_copy_film_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new copy",
                                   "picture",
                                   scan_cmd_var_new_copy_picture_desc,
                                   scan_cmd_var_new_copy_picture_usage,
                                   scan_cmd_var_new_copy_picture_notes,
                                   scan_cmd_var_new_copy_picture_cb );

    /* var new scan cmds */
    retcode = cmd_register_subcmd( "scan var new",
                                   "scan",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new scan",
                                   "mono",
                                   scan_cmd_var_new_scan_mono_desc,
                                   scan_cmd_var_new_scan_mono_usage,
                                   scan_cmd_var_new_scan_mono_notes,
                                   scan_cmd_var_new_scan_mono_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new scan",
                                   "color",
                                   scan_cmd_var_new_scan_color_desc,
                                   scan_cmd_var_new_scan_color_usage,
                                   scan_cmd_var_new_scan_color_notes,
                                   scan_cmd_var_new_scan_color_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* var new cal cmds */
    retcode = cmd_register_subcmd( "scan var new",
                                   "cal",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new cal",
                                   "mono",
                                   scan_cmd_var_new_cal_mono_desc,
                                   scan_cmd_var_new_cal_mono_usage,
                                   scan_cmd_var_new_cal_mono_notes,
                                   scan_cmd_var_new_cal_mono_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new cal",
                                   "color",
                                   scan_cmd_var_new_cal_color_desc,
                                   scan_cmd_var_new_cal_color_usage,
                                   scan_cmd_var_new_cal_color_notes,
                                   scan_cmd_var_new_cal_color_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* var new fax cmds */
    retcode = cmd_register_subcmd( "scan var new",
                                   "fax",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new fax",
                                   "text",
                                   scan_cmd_var_new_fax_text_desc,
                                   scan_cmd_var_new_fax_text_usage,
                                   scan_cmd_var_new_fax_text_notes,
                                   scan_cmd_var_new_fax_text_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan var new fax",
                                   "mixed",
                                   scan_cmd_var_new_fax_mixed_desc,
                                   scan_cmd_var_new_fax_mixed_usage,
                                   scan_cmd_var_new_fax_mixed_notes,
                                   scan_cmd_var_new_fax_mixed_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif

    /* icefile cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "icefile",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
#ifdef HAVE_SCAN_ICEFILE_SUPPORT
    retcode = cmd_register_subcmd( "scan icefile",
                                   "start",
                                   scan_cmd_icefile_start_desc,
                                   scan_cmd_icefile_start_usage,
                                   scan_cmd_icefile_start_notes,
                                   scan_cmd_icefile_start_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif
    /* adf cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "adf",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan adf",
                                   "margin",
                                   scan_cmd_adf_margin_desc,
                                   scan_cmd_adf_margin_usage,
                                   NULL,
                                   scan_cmd_adf_margin_cb );
    XASSERT( retcode==CMD_OK, retcode );
    /* davep 22-Nov-2011 ; adding duplex backside margin tuning */
    retcode = cmd_register_subcmd( "scan adf",
                                   "duplex_margin",
                                   scan_cmd_adf_duplex_margin_desc,
                                   scan_cmd_adf_duplex_margin_usage,
                                   NULL,
                                   scan_cmd_adf_duplex_margin_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan adf",
                                   "pip",
                                   scan_cmd_adf_pip_desc,
                                   scan_cmd_adf_pip_usage,
                                   scan_cmd_adf_pip_notes,
                                   scan_cmd_adf_pip_cb );
    XASSERT( retcode==CMD_OK, retcode );

    retcode = cmd_register_subcmd( "scan adf",
                                   "pause",
                                   scan_cmd_pause_desc,
                                   scan_cmd_pause_usage,
                                   scan_cmd_pause_notes,
                                   scan_cmd_pause_cb );
    XASSERT( retcode==CMD_OK, retcode );

#ifdef HAVE_SCAN_CONTINUOUS
    /* cont cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "cont",
                                   scan_cmd_cont_start_desc,
                                   scan_cmd_cont_start_usage,
                                   scan_cmd_cont_start_notes,
                                   scan_cmd_cont_start_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif

    /* pipetap cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "pipetap",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pipetap",
                                   "off",
                                   scan_cmd_pipetap_off_desc,
                                   scan_cmd_pipetap_off_usage,
                                   scan_cmd_pipetap_off_notes,
                                   scan_cmd_pipetap_off_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pipetap",
                                   "pic",
                                   scan_cmd_pipetap_pic_desc,
                                   scan_cmd_pipetap_pic_usage,
                                   scan_cmd_pipetap_pic_notes,
                                   scan_cmd_pipetap_pic_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pipetap",
                                   "pie",
                                   scan_cmd_pipetap_pie_desc,
                                   scan_cmd_pipetap_pie_usage,
                                   scan_cmd_pipetap_pie_notes,
                                   scan_cmd_pipetap_pie_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pipetap",
                                   "hp",
                                   scan_cmd_pipetap_hp_desc,
                                   scan_cmd_pipetap_hp_usage,
                                   scan_cmd_pipetap_hp_notes,
                                   scan_cmd_pipetap_hp_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan pipetap",
                                   "cutter",
                                   scan_cmd_pipetap_cutter_desc,
                                   scan_cmd_pipetap_cutter_usage,
                                   scan_cmd_pipetap_cutter_notes,
                                   scan_cmd_pipetap_cutter_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* ds cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "ds",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan ds",
                                   "dump",
                                   scan_cmd_ds_dump_desc,
                                   scan_cmd_ds_dump_usage,
                                   scan_cmd_ds_dump_notes,
                                   scan_cmd_ds_dump_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan ds",
                                   "get",
                                   scan_cmd_ds_get_desc,
                                   scan_cmd_ds_get_usage,
                                   scan_cmd_ds_get_notes,
                                   scan_cmd_ds_get_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan ds",
                                   "set",
                                   scan_cmd_ds_set_desc,
                                   scan_cmd_ds_set_usage,
                                   scan_cmd_ds_set_notes,
                                   scan_cmd_ds_set_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan ds",
                                   "delete",
                                   scan_cmd_ds_delete_desc,
                                   scan_cmd_ds_delete_usage,
                                   scan_cmd_ds_delete_notes,
                                   scan_cmd_ds_delete_cb );
    XASSERT( retcode==CMD_OK, retcode );

#ifdef HAVE_PIC_PSESD
    /* esd cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "esd",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan esd",
                                   "dir",
                                   scan_cmd_esd_dir_desc,
                                   scan_cmd_esd_dir_usage,
                                   scan_cmd_esd_dir_notes,
                                   scan_cmd_esd_dir_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan esd",
                                   "size",
                                   scan_cmd_esd_size_desc,
                                   scan_cmd_esd_size_usage,
                                   scan_cmd_esd_size_notes,
                                   scan_cmd_esd_size_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan esd",
                                   "start",
                                   scan_cmd_esd_start_desc,
                                   scan_cmd_esd_start_usage,
                                   scan_cmd_esd_start_notes,
                                   scan_cmd_esd_start_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan esd",
                                   "change",
                                   scan_cmd_esd_change_desc,
                                   scan_cmd_esd_change_usage,
                                   scan_cmd_esd_change_notes,
                                   scan_cmd_esd_change_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan esd",
                                   "tol",
                                   scan_cmd_esd_tol_desc,
                                   scan_cmd_esd_tol_usage,
                                   scan_cmd_esd_tol_notes,
                                   scan_cmd_esd_tol_cb );
    XASSERT( retcode==CMD_OK, retcode );

    /* psense cmds */
    retcode = cmd_register_subcmd( "scan",
                                   "psense",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "start",
                                   scan_cmd_psense_start_desc,
                                   scan_cmd_psense_start_usage,
                                   scan_cmd_psense_start_notes,
                                   scan_cmd_psense_start_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "tol0",
                                   scan_cmd_psense_tol0_desc,
                                   scan_cmd_psense_tol0_usage,
                                   scan_cmd_psense_tol0_notes,
                                   scan_cmd_psense_tol0_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "tol1",
                                   scan_cmd_psense_tol1_desc,
                                   scan_cmd_psense_tol1_usage,
                                   scan_cmd_psense_tol1_notes,
                                   scan_cmd_psense_tol1_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "tog01",
                                   scan_cmd_psense_tog01_desc,
                                   scan_cmd_psense_tog01_usage,
                                   scan_cmd_psense_tog01_notes,
                                   scan_cmd_psense_tog01_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "tog10",
                                   scan_cmd_psense_tog10_desc,
                                   scan_cmd_psense_tog10_usage,
                                   scan_cmd_psense_tog10_notes,
                                   scan_cmd_psense_tog10_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "cnt0",
                                   scan_cmd_psense_cnt0_desc,
                                   scan_cmd_psense_cnt0_usage,
                                   scan_cmd_psense_cnt0_notes,
                                   scan_cmd_psense_cnt0_cb );
    XASSERT( retcode==CMD_OK, retcode );
    retcode = cmd_register_subcmd( "scan psense",
                                   "cnt1",
                                   scan_cmd_psense_cnt1_desc,
                                   scan_cmd_psense_cnt1_usage,
                                   scan_cmd_psense_cnt1_notes,
                                   scan_cmd_psense_cnt1_cb );
    XASSERT( retcode==CMD_OK, retcode );
#endif

    /* davep 15-May-2013 ; adding mem to dump scanner memory info */
    retcode = cmd_register_subcmd( "scan",
                                   "mem",
                                   "dump scan memory info",
                                   NULL,NULL,
                                   scan_cmd_mem_cb );
    XASSERT( retcode==CMD_OK, retcode );

}

