/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/*
 * Find the cal strip in a scanned image.
 *
 * davep 23-Oct-2012
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "memAPI.h"

//#define FCS_DEBUG 

#include "scantypes.h"
#include "scancore.h"
#include "scanvars.h"
#include "scandbg.h"
#include "cal.h"
#include "cal_common.h"
#include "findcalstrip.h"
#include "scands.h"

#define FIND_CAL_STRIP_MAX_ROWS 600

/* number or rows we want to find in the cal strip @300 dpi
 * (scaled up for 600, 1200)
 */
#define CAL_STRIP_ROWS 60

#ifdef FCS_DEBUG
static scan_err_t save_buffer_to_pbm( const char *outfilename, uint8_t *image_data8,
                                uint32_t width, uint32_t height )
{
    FILE *outfile;

    dbg2( "%s outfilename=\"%s\" data=%p width=%d height=%d\n", __FUNCTION__, outfilename,
            image_data8, width, height );

    outfile = fopen( outfilename, "wb" );
    if( outfile==NULL ) {
        dbg2( "%s failed to open \"%s\" for writing\n", __FUNCTION__, outfilename );
        return SCANERR_GENERIC_FAIL;
    }

    /* write PGM (gray) header */
    fprintf( outfile, "P5\n" );
    fprintf( outfile, "%ld %ld\n", width, height);
    fprintf( outfile, "255\n" );
    fwrite( image_data8, sizeof(uint8_t), width*height, outfile );

    fclose(outfile);

    printf( "wrote %s\n", outfilename );

    return SCANERR_NONE;
}

static scan_err_t bit_depth_reduce( uint16_t *buffer16, uint8_t **p_buffer8, uint32_t width, uint32_t height )
{
    int num_bytes;
    uint8_t *buffer8, *ptr8;
    uint16_t *ptr16;
    uint16_t *endptr16;

    *p_buffer8 = NULL;

    num_bytes = width*height*sizeof(uint8_t);
    buffer8 = MEM_MALLOC(num_bytes);
    if( !buffer8 ) {
        dbg2( "%s unable to allocate numbytes=%d\n", __FUNCTION__, num_bytes );
        return SCANERR_OUT_OF_MEMORY;
    }

    dbg2( "%s allocated num_bytes=%d\n", __FUNCTION__, num_bytes );

    ptr8 = buffer8;
    ptr16 = buffer16;
    endptr16 = buffer16 + num_bytes;
    while( ptr16 < endptr16 ) {
        /* take high byte, truncate down to 8-bpp */
        *ptr8 = (uint8_t)((*ptr16 & 0xff00)>>8);
        ptr8++;
        ptr16++;
    }

    /* send the allocated buffer back */
    PTR_ASSIGN( *p_buffer8, buffer8 ); 

    return SCANERR_NONE;
}

/* For test/debug, I save a set of files with various parts of data during the
 * image processing. The filenames will have this prefix string so I can have
 * multiple calstrip images at the same time.
 */
static char outfilename_prefix[MAX_PREFIX_FILENAME+1];

void fcs_set_outfilename_prefix( const char *filename_base )
{
    if( strnlen(filename_base,FILENAME_MAX) > MAX_PREFIX_FILENAME ) {
        /* ignore */
        return;
    }

    memset( outfilename_prefix, 0, MAX_PREFIX_FILENAME );
    strncpy( outfilename_prefix, filename_base, MAX_PREFIX_FILENAME );
}

static void make_outfilename( const char *filename, char *dst, int dstlen )
{
    /* if our prefix name is empty, just copy the filename */
    if( outfilename_prefix[0]==0 ) {
        strncpy( dst, filename, dstlen );
    }
    else { 
        /* we have a prefix (we're running in debug code) */
        strncpy( dst, outfilename_prefix, dstlen );
        strncat( dst, "_", dstlen );
        strncat( dst, filename, dstlen );
    }
}

static void savedat( const char *dat_filename, uint32_t dat[], int num_dat )
{
    int i;
    FILE *outfile;
    char outfilename[MAX_PREFIX_FILENAME*2+1]; 

    memset( outfilename, 0, sizeof(outfilename) );
    /* -1 to leave space for NULL */
    make_outfilename( dat_filename, outfilename, sizeof(outfilename)-1 );

    outfile = fopen( outfilename, "w" );
    if( outfile ==NULL ) {
        fprintf( stderr, "%s failed to open \"%s\" for writing : %s\n", 
                __FUNCTION__, outfilename, strerror(errno));
        return;
    }

    for( i=0 ; i<num_dat ; i++ ) {
        fprintf( outfile, "%lu\n", dat[i] );
    }

    fclose( outfile );
    printf( "wrote %s\n", outfilename );
}
#else
/* no file system on this platform :-( too bad, so sad */
static void savedat( const char *dat_filename, uint32_t dat[], int num_dat ) {}
static scan_err_t bit_depth_reduce( uint16_t *buffer16, uint8_t **p_buffer8, 
                                    uint32_t width, uint32_t height )
                                    { return SCANERR_NOT_IMPLEMENTED; }
static scan_err_t save_buffer_to_pbm( const char *outfilename, uint8_t *image_data8,
                                      uint32_t width, uint32_t height )
                                      { return SCANERR_NOT_IMPLEMENTED; }
#endif

cal_err_t find_cal_strip( cal_pixel_t *pixbuf, uint32_t dpi, uint32_t pixels_per_row, uint32_t num_rows,
                     uint32_t *starting_row, uint32_t *ending_row )
{
    scan_err_t scerr;
    int i, j;
    cal_pixel_t *ptr;
    int starting_edge, ending_edge, cal_strip_size,cal_cut_rows;
    cal_pixel_t max_pixel, min_pixel, threshold;
    uint32_t *pixel_row_means;
    uint8_t *buffer8;
    uint32_t num32;

    dbg2( "%s dpi=%lu ppr=%lu nr=%lu\n", __FUNCTION__, dpi, pixels_per_row, num_rows );

    /* davep 05-Mar-2013 ; add run-time ability to save the occasional cal
     * strip image to the file system (assuming we have a file system).
     */
    scerr = scands_get_integer_with_default( "save_calstrip", &num32, 0 );
    if( num32 ) {
        scerr = bit_depth_reduce( pixbuf, &buffer8, pixels_per_row, num_rows );
        if( scerr==SCANERR_NONE ) {
            save_buffer_to_pbm( "/Kinoma/calstrip.pgm", buffer8, pixels_per_row, num_rows );
            PTR_FREE(buffer8);
        }
    }

    *starting_row = 0;
    *ending_row = 0;

    if( num_rows > FIND_CAL_STRIP_MAX_ROWS ) {
        dbg1( "%s num_rows=%ld too big\n", num_rows );
        return CALERR_INVALID_PARAMETER;
    }

    /* we want this many rows for our cal strip calibration. Could require tuning! */
    cal_strip_size = CAL_STRIP_ROWS * (dpi/300);
    if( num_rows < cal_strip_size ) {
        *ending_row = num_rows;
        dbg1( "%s num_rows=%ld too small\n", num_rows );
        return CALERR_NONE;
    }
    ptr = pixbuf;


    /* allocate memory for our math */
    pixel_row_means = (uint32_t *)MEM_MALLOC( sizeof(uint32_t)*FIND_CAL_STRIP_MAX_ROWS);
    if( !pixel_row_means ) {
        dbg1( "%s out of memory for cal strip analysis\n", __FUNCTION__ );
        return CALERR_OUT_OF_MEMORY;
    }
    memset( pixel_row_means, 0, sizeof(uint32_t)*FIND_CAL_STRIP_MAX_ROWS );

    min_pixel = ~0;
    max_pixel = 0;

    /* calculate sum of each row */
    for( i=0 ; i<num_rows ; i++ ) {
        ptr = pixbuf + (i*pixels_per_row);
        for( j=0 ; j<pixels_per_row ; j++ ) {
            pixel_row_means[i] += ptr[j];
        }
    }
    savedat( "sums.dat", pixel_row_means, num_rows );

    /* calculate the mean and find min/max */
    for( i=0 ; i<num_rows ; i++ ) {
        pixel_row_means[i] /= pixels_per_row;  
        if(pixel_row_means[i] > 65000)// high  value must meanu that we get some data out of the cal strip.
        {
            continue;
        }  
        if( pixel_row_means[i] > max_pixel ) {
            max_pixel = pixel_row_means[i];
        }
        else if( pixel_row_means[i] < min_pixel ) {
            min_pixel = pixel_row_means[i];
        }
    }
    savedat( "means.dat", pixel_row_means, num_rows );

    /* this threshold value might need to be tuned */
    threshold = (max_pixel-min_pixel)/2;
    dbg2( "%s min=%d max=%d threshold=%d\n", __FUNCTION__, min_pixel, max_pixel, threshold );

    /* threshold the data */
    for( i=0 ; i<num_rows ; i++ ) {
        //dbg1("i=%d:%d ",i,pixel_row_means[i]);
        if( pixel_row_means[i] > threshold ) {
            /* white */
            pixel_row_means[i] = ~0;
        }
        else {
            /* black */
            pixel_row_means[i] = 0;
        }
    }
    savedat( "binarized.dat", pixel_row_means, num_rows );

    /* search forwards looking for a leading and trailing edge */
    starting_edge = -1;
    ending_edge = -1;
    for( i=0 ; i<num_rows ; i++ ) {
//        dbg2( "i=%d thresh=%d value=%u\n", i, threshold, pixel_row_means[i]);
        if( starting_edge==-1 && pixel_row_means[i] ) {
            dbg2( "%s starting_edge=%d\n", __FUNCTION__, starting_edge );
            starting_edge = i;
        }
        else if( starting_edge!=-1 && !pixel_row_means[i] ) {
            ending_edge = i;
            dbg2( "%s starting_edge=%u ending_edge=%u\n", 
                     __FUNCTION__, starting_edge, ending_edge );
            break;
        }
    }

    PTR_FREE( pixel_row_means );

    dbg2( "starting_edge=%u ending_edge=%u\n", starting_edge, ending_edge );

    if( starting_edge==-1 ) {
        dbg2( "%s failed to find leading edge\n", __FUNCTION__ );
        return CALERR_CAL_STRIP_NO_LEADING_EDGE; 
    }

    cal_cut_rows = 10 * (dpi/300);

    if( ending_edge==-1 ) {
        dbg1( "%s failed to find trailing edge,start_edge=%d,min=%d max=%d threshold=%d\n", 
              __FUNCTION__ ,starting_edge,min_pixel, max_pixel, threshold);
        starting_edge += cal_cut_rows;
        ending_edge = starting_edge + cal_strip_size;
        if(ending_edge > num_rows)
        {
            //return some rows back even it is not the best data. 
            ending_edge = num_rows;
            starting_edge = ending_edge - cal_strip_size;
            dbg1("failed to get best data, return start=%d,end=%d\n",starting_edge,ending_edge);
            //return CALERR_CAL_STRIP_NO_TRAILING_EDGE; 
        }
    }
    else
    {
        /* back edge up 10 more rows (scaled to dpi) to be safer */
        if((ending_edge - starting_edge) > (CAL_STRIP_ROWS+cal_cut_rows))
        {
            ending_edge -= cal_cut_rows;
            starting_edge = ending_edge - cal_strip_size;
        }
        else
        {
            starting_edge += cal_cut_rows;
            ending_edge = starting_edge + cal_strip_size;
        }
    }

    *starting_row = starting_edge; 
    *ending_row = ending_edge;

    return CALERR_NONE;
}

