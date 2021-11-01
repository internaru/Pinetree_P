/**
 * \file scancap.c
 *
 * \brief utility functions for scan hardware capabilities (scancaps)
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scancap.h"
#include "scanplat.h"

bool scancap_has_sensor_dpi( scan_document_source_t docsrc, uint32_t dpi )
{
    int i;
    struct scan_platform_capabilities scan_capabilities;
    uint32_t *dpilist;

    scancap_get_capabilities( &scan_capabilities );

    switch( docsrc ) {
        case SCAN_DOCUMENT_SOURCE_FLATBED :
            if( !scan_capabilities.has_flatbed ) {
                return false;
            }
            dpilist = scan_capabilities.fb_supported_horiz_dpi;
            break;

        case SCAN_DOCUMENT_SOURCE_ADF :
            if( !scan_capabilities.has_adf ) {
                return false;
            }
            dpilist = scan_capabilities.adf_supported_horiz_dpi;
            break;

        default :
            dpilist = NULL;
            XASSERT( 0, docsrc );
    }

    if( !dpilist ) {
        return false;
    }

    /* dpi is a zero terminated list */
    for( i=0 ; ; i++ ) {
        /* sanity check our array size */
        XASSERT( i<SCAN_CAP_DPI_MAX, i );

        if( dpi==dpilist[i] ) {
            return true;
        }
        /* end-of-list marker */
        if( dpilist[i]==0 ) {
            break;
        }
    }

    return false;
}

static scan_err_t numlist_to_str( uint32_t numlist[], int numlist_len, 
                                    char buf[], int buflen )
{
    int i, len, retcode;
    char *ptr;

    memset( buf, 0, buflen );
    ptr = buf;
    len = buflen-1; /* -1 for NULL */

    /* require a zero-terminated list of a specified length */
    for( i=0 ; i<numlist_len && numlist[i]!=0 ; i++ ) {

        /* don't want an ugly trailing comma */
        if( i<numlist_len-1 && numlist[i+1]!=0 ) {
            retcode = snprintf( ptr, len, "%"PRIu32",", numlist[i] );
        }
        else {
            retcode = snprintf( ptr, len, "%"PRIu32, numlist[i] );
        }

        ptr += retcode;
        len -= retcode;
        if( len <= 0 ) {
            break;
        }
    }

    return SCANERR_NONE;
}

void scancap_debug_dump( struct scan_platform_capabilities *scancap )
{
    char tmpstr[64];
    int i;
    uint32_t num32_list[SCAN_CAP_DATA_TYPE_MAX];

    dbg1( "scancap has_adf=%d has_flatbed=%d has_duplex=%d has_nsensor=%d\n", 
            scancap->has_adf,
            scancap->has_flatbed,
            scancap->has_duplex,
            scancap->has_nsensor );

    numlist_to_str( scancap->fb_supported_horiz_dpi, SCAN_CAP_DPI_MAX, 
                    tmpstr, sizeof(tmpstr) );
    dbg1( "scancap fb_horiz_dpi=%s\n", tmpstr );

    numlist_to_str( scancap->fb_supported_vert_dpi, SCAN_CAP_DPI_MAX, 
                    tmpstr, sizeof(tmpstr) );
    dbg1( "scancap fb_vert_dpi=%s\n", tmpstr );

    /* convert enum to integer so can use my convenience function */
    memset( num32_list, 0, sizeof(num32_list) );
    for( i=0 ; i<SCAN_CAP_DATA_TYPE_MAX && scancap->scan_data_type_list[i]!=0 ; i++ ) {
        num32_list[i] = scancap->scan_data_type_list[i];
    }
    numlist_to_str( num32_list, SCAN_CAP_DATA_TYPE_MAX, 
                    tmpstr, sizeof(tmpstr) );
    dbg1( "scancap scan_data_type=%s\n", tmpstr );
}

bool scancap_has_data_type( scan_data_type dtype )
{
    int i;
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );

    /* scan_data_type_list[] is a 0 terminated list */
    for( i=0 ; ; i++ ) {
        /* sanity check our array size */
        XASSERT( i<SCAN_CAP_DATA_TYPE_MAX, i );

        if( dtype==scan_capabilities.scan_data_type_list[i] ) {
            return true;
        }
        if( scan_capabilities.scan_data_type_list[i]==SCAN_DATA_NULL_TYPE ) {
            break;
        }
    }

    return false;
}

static scan_err_t get_nearest_dpi( uint32_t *dpi_list, uint32_t dpi_in, uint32_t *dpi_out )
{
    int i;

    *dpi_out = 0;
    
    for( i=0 ; i<SCAN_CAP_DPI_MAX ; i++ ) {
        /* 0 is the end-of-list marker */
        if( dpi_list[i]==0 ) {
            break;
        }
        *dpi_out = dpi_list[i];
        if( dpi_list[i] >= dpi_in ) {
            break;
        }
    }
    XASSERT( i<SCAN_CAP_DPI_MAX, i );

    if( *dpi_out == 0 ) {
        return SCANERR_NO_ENTRY;
    }

    return SCANERR_NONE;
}

scan_err_t scancap_get_nearest_horiz_dpi( scan_document_source_t docsrc, uint32_t dpi_in, uint32_t *dpi_out )
{
    scan_err_t scerr;
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );

    *dpi_out = 0;

    /* davep 08-Sep-2011 ; adding adf duplex; assuming duplex adf same dpis as
     * simplex adf
     */

    switch( docsrc ) {
        case SCAN_DOCUMENT_SOURCE_FLATBED :
            scerr = get_nearest_dpi( scan_capabilities.fb_supported_horiz_dpi, dpi_in, dpi_out ); 
            break;

        case SCAN_DOCUMENT_SOURCE_ADF :
        case SCAN_DOCUMENT_SOURCE_ADF_DUPLEX :
            scerr = get_nearest_dpi( scan_capabilities.adf_supported_horiz_dpi, dpi_in, dpi_out ); 
            break;

        default :
            XASSERT( 0, docsrc );
            scerr = SCANERR_INVALID_PARAM;
            break;
    }

    return scerr;
}

scan_err_t scancap_get_nearest_vert_dpi( scan_document_source_t docsrc, uint32_t dpi_in, uint32_t *dpi_out )
{
    scan_err_t scerr;
    struct scan_platform_capabilities scan_capabilities;

    scancap_get_capabilities( &scan_capabilities );

    *dpi_out = 0;

    /* davep 08-Sep-2011 ; adding adf duplex; assuming duplex adf same dpis as
     * simplex adf
     */

    switch( docsrc ) {
        case SCAN_DOCUMENT_SOURCE_FLATBED :
            scerr = get_nearest_dpi( scan_capabilities.fb_supported_vert_dpi, dpi_in, dpi_out ); 
            break;

        case SCAN_DOCUMENT_SOURCE_ADF :
        case SCAN_DOCUMENT_SOURCE_ADF_DUPLEX :
            scerr = get_nearest_dpi( scan_capabilities.adf_supported_vert_dpi, dpi_in, dpi_out ); 
            break;

        default :
            XASSERT( 0, docsrc );
            scerr = SCANERR_INVALID_PARAM;
            break;
    }

    return scerr;
}

void scancap_get_capabilities( struct scan_platform_capabilities *caps )
{
    scanplat_get_capabilities( caps );
}

/**
 * \brief  Run sanity checks on the scanplat capabilities. 
 *
 * \author David Poole
 * \date 16-Dec-2010
 *
 */

void scancap_sanity_check( void )
{
    int i;

    /* dpi list(s) must be sorted smallest to largest and be zero terminated */
    for( i=0 ; i<SCAN_CAP_DPI_MAX ; i++ ) {
        /* TODO  */
    }

}

