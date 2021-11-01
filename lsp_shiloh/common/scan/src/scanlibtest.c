/**
 * \file scanlibtest.c
 *
 * \brief scanlib.c test routines.
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdlib.h>
#include <string.h>

#include "scos.h"

#include "list.h" 
#include "lassert.h" 
#include "memAPI.h"

#include "scancore.h" 
#include "scantypes.h"
#include "scanvars.h" 
#include "scandbg.h" 
#include "scanlib.h"
#include "scanlibtest.h"
#include "scanmech.h"

#define NUM_TEST_DPI  3

typedef struct scanvars *(*scanvar_create_fn)( void );

/**
 * \brief test directly calling scanlib2_setup()/run()/done() 
 *
 *
 * \author David Poole
 * \date 15-Oct-2008
 *
 */

static scan_err_t scanlib2_test_direct( scan_cmode_t cmode, int dpi )
{
    scan_err_t scerr;
    struct scanvars *sv, *tmp_sv;

    /* davep 09-Oct-2008 ; directly grab the knobs so we can do scans with more
     * control (like what cal and the notchfind code needs)
     */
    dbg1( "%s cmode=%d dpi=%d\n", __FUNCTION__, cmode, dpi );

    if( cmode==SCAN_CMODE_MONO ) {
        sv = scanplat_sv_mono_scan_new();
    }
    else {
        sv = scanplat_sv_color_scan_new();
    }

    if( sv==NULL ) {
        dbg2( "%s unable to alloc memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    scanvar_push( sv );
    scanvar_set_dpi( sv, dpi );
    scanvar_set_area( sv, 0, 0, 850, 300 );

    scerr = scanmech_prepare(SCAN_TARGET_TYPE_DOC);
    if( scerr != 0 ) {
        dbg1( "%s scanmech_prepare() failed with scerr=%d\n", 
                    __FUNCTION__, scerr );
        
        goto fail;
    }

    scerr = scanlib2_setup();
    if( scerr != 0 ) {
        dbg1( "%s scanlib2_setup() failed with scerr=%d\n", 
                    __FUNCTION__, scerr);
        
        goto fail;
    }

    scerr = scanlib2_run(0);
    if( scerr != 0 ) {
        /* scanlib2_run() will clean up anything that needs cleaning up inside
         * the scanlib layers and below
         */
        dbg1( "%s scanlib2_run() failed with scerr=%d\n", 
                    __FUNCTION__, scerr);
        
        goto fail;
    }

    /* scanlib2_done() is a void function so no return code to test */
    scanlib2_done();

    scanmech_done(SMECH_DONE_FLAG_SUCCESS);

    /* clean up my mess */
    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==sv, (uint32_t)tmp_sv );
    scanvar_delete( &sv );


    dbg1( "%s direct scanlib2 knobs test ran successfully\n", __FUNCTION__ );

    /* success! */
    return SCANERR_NONE;

fail:
    tmp_sv = scanvar_pop();
    XASSERT( tmp_sv==sv, tmp_sv->id );
    scanvar_delete( &sv );

    return scerr;
}

/**
 * \brief  test scanlib2_setup_and_run()
 *
 *
 * \author David Poole
 * \date 15-Oct-2008
 *
 */

static scan_err_t ssar_run( struct scanvars **psv )
{
    scan_err_t scerr;
    struct scanvars *sv;

    dbg1( "%s\n", __FUNCTION__ );

    /* use a local to avoid splat hell */
    PTR_ASSIGN( sv, *psv );

    /* set to a smaller area so test will run faster */
    scanvar_set_area( sv, 100, 100, 300, 300 );
//    scanvar_set_area( sv, 0, 0, 850, 1100 );

    scerr = scanlib2_setup_and_run( &sv );

    /* scanlib2_setup_and_run() shouldn't have taken the scanvar from us */
    ASSERT( sv!=NULL );

    /* we send the scanvar back to the caller */
    PTR_ASSIGN( *psv, sv );

    if( scerr != 0 ) {
        dbg2( "%s line=%d scanlib2_setup_and_run() failed with scerr=%d\n",
                    __FUNCTION__, __LINE__, scerr );

        return scerr;
    }

    dbg1( "%s ran successfully\n", __FUNCTION__ );

    /* success! */
    return SCANERR_NONE;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 16-Nov-2008
 *
 */

static scan_err_t scanlib2_test_simple( scan_cmode_t cmode, int dpi )
{
    struct scanvars *sv;
    scan_err_t scerr, final_scerr;
    
    dbg2( "%s cmode=%d dpi=%d\n", __FUNCTION__, cmode, dpi );

    final_scerr = SCANERR_NONE;

    if( cmode==SCAN_CMODE_MONO ) {
        sv = scanplat_sv_mono_scan_new();
    }
    else {
        sv = scanplat_sv_color_scan_new();
    }
    if( sv==NULL ) {
        dbg2( "%s unable to alloc memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }

    scanvar_set_dpi( sv, dpi );

    scerr = ssar_run( &sv );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
    }

    ASSERT( sv!=NULL );

    /* clean up and move on to the next test */
    scanvar_delete( &sv );

    return final_scerr;
}

#ifdef HAVE_ICE_HALFPACK
/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 16-Nov-2008
 *
 */

static scan_err_t mono_laser_copy_mode_tests( void )
{
    scanvar_create_fn scanvar_create_fn_list[] = { 
        scanplat_sv_draft_copy_new,
        scanplat_sv_text_copy_new,
        scanplat_sv_mixed_copy_new,
        scanplat_sv_filmphoto_copy_new,
        scanplat_sv_picture_copy_new,
        NULL,
    };
    struct scanvars *sv;
    int i;
    scan_err_t scerr;

    dbg2( "%s\n", __FUNCTION__ );

    for( i=0 ; ; i++ ) {
        if( scanvar_create_fn_list[i] == NULL ) {
            break;
        }

        sv = scanvar_create_fn_list[i]();
        if( sv==NULL ) {
            return SCANERR_OUT_OF_MEMORY;
        }

        scerr = ssar_run( &sv );

        ASSERT( sv!=NULL );
        scanvar_delete( &sv );

        if( scerr != 0 ) {
            return scerr;
        }
    }

    return SCANERR_NONE;
}
#endif

/**
 * \brief  Run tests on the scanlib2 API.
 *
 * Even though this is test code, want to properly cleanup and propagate
 * the error upwards so others can use this code as an example.
 *
 * \author David Poole
 * \date ??-Mar-2008
 *
 */

scan_err_t scanlib2_test( void )
{
    scan_err_t scerr;
    int dpi_cnt, cmode_cnt;
    int dpi;
    int dpi_list[NUM_TEST_DPI] = { 300, 600, 1200 };
    scan_cmode_t cmode;
    scan_cmode_t cmode_list[2] = { SCAN_CMODE_MONO, SCAN_CMODE_COLOR };

    dbg1( "%s start\n", __FUNCTION__ );

    for( cmode_cnt=0 ; cmode_cnt<2 ; cmode_cnt++ ) {

        cmode = cmode_list[cmode_cnt];

        for( dpi_cnt=0 ; dpi_cnt<NUM_TEST_DPI ; dpi_cnt++ ) {
            dpi = dpi_list[dpi_cnt];

            scerr = scanlib2_test_simple( cmode, dpi );
            if( scerr != 0 ) {
                dbg2( "%s scanlib2_test_simple failed scerr=%d\n", __FUNCTION__, scerr );
                return scerr;
            }

            scerr = scanlib2_test_direct( cmode, dpi );
            if( scerr != 0 ) {
                dbg2( "%s scanlib2_test_direct failed scerr=%d\n", __FUNCTION__, scerr );
                return scerr;
            }
        }
    }

    /* because some platforms don't have HalfPack, run these tests separately */
#ifdef HAVE_ICE_HALFPACK
    scerr = mono_laser_copy_mode_tests();
    if( scerr != 0 ) {
        return scerr;
    }
#endif

    dbg1( "%s ran successfully\n", __FUNCTION__ );

    /* success! */
    return SCANERR_NONE;
}

