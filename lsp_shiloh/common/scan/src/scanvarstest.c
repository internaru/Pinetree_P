/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file scanvarstest.c
 *
 * \brief regression test for scanvars.c
 *
 * davep 17-Dec-2010 
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanvars.h"

static scan_err_t test_dpi_and_scale( int order ) 
{
    scan_err_t scerr;
    struct scanvars *sv;
    int x_numer, x_denom, y_numer, y_denom;

    sv = scanvar_mono_scan_new();
    if( sv==NULL ) {
        return SCANERR_OUT_OF_MEMORY;
    }
    scanvar_sanity( sv );

    /* Assume we're running on a system that can handle 300 dpi. We'll set to
     * 150 dpi which should set a scale of 1/2,1/2.  And we're doing a user
     * scale of 1/2,1/2.
     *
     * Both set_dpi() and set_xyscale() need to cooperate to keep the dpi
     * scaling and user scaling coordinating.
     *
     * FIXME this test will fail on a system that doesn't support 300 dpi.
     * Should write a smarter test
     */
    if( order==0 ) {
        scerr = scanvar_set_xyscale( sv, 1, 2, 1, 2 );
        XASSERT( scerr==0, scerr ); 

        scerr = scanvar_set_dpi( sv, 150 );
        XASSERT( scerr==0, scerr ); 
    }
    else {
        scerr = scanvar_set_dpi( sv, 150 );
        XASSERT( scerr==0, scerr ); 

        scerr = scanvar_set_xyscale( sv, 1, 2, 1, 2 );
        XASSERT( scerr==0, scerr ); 
    }


    scanvar_get_pie_scale( sv, &x_numer, &x_denom, &y_numer, &y_denom );
    XASSERT( x_numer==1, x_numer );
    XASSERT( x_denom==4, x_denom );
    XASSERT( y_numer==1, y_numer );
    XASSERT( y_denom==4, y_denom );

    scanvar_sanity( sv );

    scanvar_delete( &sv );
    XASSERT( sv==NULL, sv->id );

    return SCANERR_NONE;
}

static struct scanvars * (*scanvar_constructor_list [])(void) = {
    scanplat_sv_mono_scan_new,
    scanplat_sv_color_scan_new,

    scanplat_sv_mono_cal_new,
    scanplat_sv_color_cal_new,

    scanplat_sv_text_fax_new,
    scanplat_sv_mixed_fax_new,

    scanplat_sv_draft_copy_new,
    scanplat_sv_text_copy_new,
    scanplat_sv_mixed_copy_new,
    scanplat_sv_filmphoto_copy_new,
    scanplat_sv_picture_copy_new,
                                                
    scanplat_sv_draft_color_copy_new,
    scanplat_sv_text_color_copy_new,
    scanplat_sv_mixed_color_copy_new,
    scanplat_sv_filmphoto_color_copy_new,
    scanplat_sv_picture_color_copy_new,

    NULL /* end of list */
};

static scan_err_t test_all_constructors( void )
{
    scan_err_t scerr;
    struct scanvars *sv;
    int i;

    i=0;
    while( scanvar_constructor_list[i]!=NULL ) {

        sv = scanvar_constructor_list[i]();
        if( sv==NULL ) {
            return SCANERR_OUT_OF_MEMORY;
        }

        /* test the functions that generate LUTs */
        scerr = scanvar_set_gamma( sv, 22 );
        XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );

        scerr = scanvar_set_contrast( sv, 120 );
        XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );

        scerr = scanvar_set_brightness( sv, 120 );
        XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );

        scerr = scanvar_set_sharpness( sv, 80 );
        XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );

        scerr = scanvar_set_all_black_white_clip( sv, 10, 200 );
        XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );

        if( sv->cmode==SCAN_CMODE_COLOR ){
            scerr = scanvar_set_rgb_tint( sv, 10, 20, 30 );
            XASSERT( scerr==SCANERR_NONE || scerr==SCANERR_NOT_IMPLEMENTED, scerr );
        }

        scanvar_sanity( sv );
        scanvar_delete( &sv );

        i += 1;
    }

    return SCANERR_NONE;
}

scan_err_t scanvars_test( void )
{
    scan_err_t scerr;
    struct scanvars *sv;

    sv = scanvar_mono_scan_new();
    if( sv==NULL ) {
        ASSERT(0);
        return SCANERR_OUT_OF_MEMORY;
    }
    scanvar_sanity( sv );
    scanvar_print( sv );
    scanvar_to_siqfile( sv );
    scanvar_delete( &sv );
    XASSERT( sv==NULL, sv->id );

    sv = scanvar_color_scan_new();
    if( sv==NULL ) {
        ASSERT(0);
        return SCANERR_OUT_OF_MEMORY;
    }
    scanvar_sanity( sv );
    scanvar_delete( &sv );
    XASSERT( sv==NULL, sv->id );

    scerr = test_dpi_and_scale(0);
    XASSERT( scerr==SCANERR_NONE, scerr );
    scerr = test_dpi_and_scale(1);
    XASSERT( scerr==SCANERR_NONE, scerr );

    scerr = test_all_constructors();

    return SCANERR_NONE;
}

