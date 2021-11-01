/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>  /* for htonl */

#include "asp.h"

void
asp_swap_dimension_fields( struct scan_dimensions *sd )
{
    sd->top = htonl( sd->top );
    sd->left = htonl( sd->left );
    sd->bottom = htonl( sd->bottom );
    sd->right = htonl( sd->right );
}

void
asp_swap_xyscale_fields( struct xyscale *xyscale )
{
    xyscale->x_numerator = htonl( xyscale->x_numerator );
    xyscale->x_denominator = htonl( xyscale->x_denominator );
    xyscale->y_numerator = htonl( xyscale->y_numerator );
    xyscale->y_denominator = htonl( xyscale->y_denominator );
}

void
asp_swap_job_fields( struct scan_job *job )
{
    job->gamma = htonl( job->gamma );
    job->brightness = htonl( job->brightness );
    job->contrast = htonl( job->contrast );
    job->resolution = htonl( job->resolution );
    asp_swap_xyscale_fields( &job->xyscale );
    job->sharp = htonl( job->sharp );
    job->deprecated1 = htonl( job->deprecated1 );
    job->bpp = htonl( job->bpp );

    /* note I'm also being a good boy and swapping the reserved fields */
    job->reserved1 = htonl( job->reserved1 );
    job->reserved2 = htonl( job->reserved2 );
    job->reserved3 = htonl( job->reserved3 );

    job->flags = htonl( job->flags );

    job->data_type = htonl( job->data_type );

    asp_swap_dimension_fields( &job->window );
    asp_swap_dimension_fields( &job->area );

    job->scan_type = htonl( job->scan_type );
}

void
asp_swap_header_fields( struct scan_header *hdr )
{
    hdr->cookie = htonl( hdr->cookie );
    hdr->msg = htonl( hdr->msg );
    hdr->param1 = htonl( hdr->param1 );
    hdr->param2 = htonl( hdr->param2 );
    hdr->status = htonl( hdr->status );
    hdr->datalen = htonl( hdr->datalen );
    hdr->reserved1 = htonl( hdr->reserved1 );
    hdr->reserved2 = htonl( hdr->reserved2 );
}

void
asp_swap_data_header_fields( struct scan_data_header *dhdr )
{
    dhdr->data_type = htonl( dhdr->data_type );
    dhdr->rownum = htonl( dhdr->rownum );
    dhdr->numrows = htonl( dhdr->numrows );
    dhdr->bytes_per_pixel = htonl( dhdr->bytes_per_pixel );
    dhdr->pixels_per_row = htonl( dhdr->pixels_per_row );
    dhdr->pixels_per_row_padded = htonl( dhdr->pixels_per_row_padded );
}

int 
asp_scan_job_to_str( char *str, int len, struct scan_job *job )
{
    int retcode;

    /* note I'm ignoring the reserved fields and a few fields we're not using
     * yet
     */
//    retcode = snprintf( str, len, 
//                        "res=%d scale=%d/%d,%d/%d bpp=%d "
//                        "window=%d,%d,%d,%d area=%d,%d,%d,%d "
//                        "scantype=%d datatype=%d", 
//                        job->resolution, 
//                        job->xyscale.x_numerator, job->xyscale.x_denominator,
//                        job->xyscale.y_numerator, job->xyscale.y_denominator,
//                        job->bpp, 
//                        job->window.top, job->window.left, 
//                        job->window.bottom, job->window.right,
//                        job->area.top, job->area.left, 
//                        job->area.bottom, job->area.right,
//                        job->scan_type, job->data_type
//                      );

    retcode = snprintf( str, len, 
                        "res=%d gamma=%d bright=%d contrast=%d scale=%d/%d,%d/%d bpp=%d "
                        "window=%d,%d,%d,%d area=%d,%d,%d,%d "
                        "scantype=%d datatype=%d", 
                        job->resolution, 
                        job->gamma,
                        job->brightness, job->contrast,
                        job->xyscale.x_numerator, job->xyscale.x_denominator,
                        job->xyscale.y_numerator, job->xyscale.y_denominator,
                        job->bpp, 
                        job->window.top, job->window.left, 
                        job->window.bottom, job->window.right,
                        job->area.top, job->area.left, 
                        job->area.bottom, job->area.right,
                        job->scan_type, job->data_type
                      );

    return retcode;
}

int
asp_scan_header_to_str( char *str, int len, struct scan_header *hdr )
{
    int retcode;

    /* Note I'm ignoring the reserved fields. Assumes fields already
     * byteswapped.
     */

    retcode = snprintf( str, len, "cookie=%#x msg=%d param1=%#x param2=%#x "
                                  "status=%d datalen=%d",
                                  hdr->cookie, hdr->msg, hdr->param1,
                                  hdr->param2, hdr->status, hdr->datalen );

    return retcode;
}

int
asp_data_header_to_str( char *str, int len, struct scan_data_header *dhdr )
{
    int retcode;

    retcode = snprintf( str, len, "datatype=%d rownum=%d numrows=%d "
                                  "bytes_per_pixel=%d ppr=%d pprp=%d",
                                  dhdr->data_type, dhdr->rownum,
                                  dhdr->numrows, dhdr->bytes_per_pixel,
                                  dhdr->pixels_per_row,
                                  dhdr->pixels_per_row_padded );

    return retcode;
}

const char *asp_header_status_to_string( int scan_stat )
{
    size_t i;
    const struct { int stat; const char *str; } ssstr[] = {
        { SCAN_STAT_SUCCESS, "success" },
        { SCAN_STAT_FAIL,    "generic failure" },
        { SCAN_STAT_BUSY,    "busy" },
        { SCAN_STAT_CMD_NOT_RECOGNIZED, "command not recognized" },
        { SCAN_STAT_INVALID_ARG, "invalid argument" },
        { SCAN_STAT_ADF_EMPTY,   "adf is empty" },
        { SCAN_STAT_ADF_MISPICK, "adf mispick" },
        { SCAN_STAT_ADF_JAM,     "adf is jammed" },
    };

    for( i=0 ; i<sizeof(ssstr)/sizeof(ssstr[0]) ; i++ ) {
        if( scan_stat==ssstr[i].stat ) {
            return ssstr[i].str;
        }
    }

    return "unknown status code";
}

void scan_msg_page_start_decode( struct scan_header *hdr, uint8_t *page_side )
{
    *page_side = SCAN_MSG_START_PAGE_GET_PAGE_SIDE(hdr->param1);
}

void scan_msg_image_data_decode( struct scan_header *hdr, uint8_t *page_side )
{
    *page_side = SCAN_MSG_IMAGE_DATA_GET_PAGE_SIDE(hdr->param1);
}

void scan_msg_page_end_decode( struct scan_header *hdr, uint8_t *page_side )
{
    *page_side = SCAN_MSG_END_PAGE_GET_PAGE_SIDE(hdr->param1);
}

