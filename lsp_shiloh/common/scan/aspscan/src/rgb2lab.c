/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* Convert a raw RGB file to CIELAB via RGB -> XYZ -> Lab.  
 *
 * Uses simple clipping (absolute colorimetric I think what it's called).
 *
 * The output Lab values are normalized to [0-255] to be able to convert to
 * grayscale images for your viewing pleasure.
 *
 * Converted from color.py. Heavily annotated as to the source of the math.
 *
 * davep 12-Apr-2007
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define VERSION "1.0.0"

#ifndef MIN
#define MIN(x,y) ( (x)<(y)?(x):(y))
#endif

#ifndef MAX
#define MAX(x,y) ( (x)>(y)?(x):(y))
#endif

#define PTR_FREE(ptr)  do { free(ptr); (ptr)=NULL; } while(0);

/*
 * Observer = 2 degrees, Illuminant = D65  from Table 3-3
 * _Understanding Color Management_, Sharma. ISBN 1401814476
 *
 * Also http://www.easyrgb.com/math.php?MATH=M15#text15
 */
const double D65_X = 95.047;
const double D65_Y = 100.0;
const double D65_Z = 108.883;

inline void 
RGB_to_XYZ( double *P1, double *P2, double *P3 ) 
{
//    double R, G, B;
    double var_R, var_G, var_B;
    double X, Y, Z;

//    # http://www.easyrgb.com/math.php?MATH=M2#text2

//    # Observer = 2 degrees, Illuminant = D65
     
    var_R = ( *P1 / 255.0 ); //       # Where R = 0 / 255
    var_G = ( *P2 / 255.0 ); //       # Where G = 0 / 255
    var_B = ( *P3 / 255.0 ); //       # Where B = 0 / 255

    if( var_R > 0.04045 ) {
        var_R = pow( ( ( var_R + 0.055 ) / 1.055 ), 2.4) ;
    }
    else {
        var_R = var_R / 12.92;
    }

    if( var_G > 0.04045 ) {
        var_G = pow( ( ( var_G + 0.055 ) / 1.055 ), 2.4 );
    }
    else {
        var_G = var_G / 12.92;
    }

    if( var_B > 0.04045 ) {
        var_B = pow( ( ( var_B + 0.055 ) / 1.055 ), 2.4 );
    }
    else {
        var_B = var_B / 12.92;
    }

    var_R = var_R * 100;
    var_G = var_G * 100;
    var_B = var_B * 100;

//    # //Observer = 2 degrees, Illuminant = D65
    X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
    Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
    Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;
    
    /* Note: no gamut checking! */

    *P1 = X;
    *P2 = Y;
    *P3 = Z;
}

inline void 
XYZ_to_LAB( double *P1, double *P2, double *P3 ) 
{ 
//    double X, Y, Z;
    double X_term, Y_term, Z_term;
    double L_star, a_star, b_star;

//    # Can't remember where I originally found this formula but have confirmed
//    # it in R.W.G.Hunt's _The Reproduction of Color_, 5e, p777.
//    # 
//    # Note that I'm hardwiring to D65 illumination here.
//    #
//    # davep 16-Aug-06 

//    X = *P1;
//    Y = *P2;
//    Z = *P3;

//    # cube root of ratio
    X_term = pow( ( *P1 / D65_X), (1.0/3.0));
    Y_term = pow( ( *P2 / D65_Y), (1.0/3.0));
    Z_term = pow( ( *P3 / D65_Z), (1.0/3.0));

    L_star = 116.0 * Y_term - 16;
    a_star = 500.0 * ( X_term - Y_term );
    b_star = 200.0 * ( Y_term - Z_term );

//    # davep 16-Aug-06 ; enforce clipping
//    # L = Luminance [0,100]
//    # a = [-128 (green), 127 (red)]
//    # b = [-128 (blue), 127 (yellow)]

//    L_star = MIN( 100, MAX( 0, L_star ));
//    a_star = MIN( 127, MAX( -128, a_star ));
//    b_star = MIN( 127, MAX( -128, b_star ));

    /* Push up into [0-255] range then clip.  The 2.55 is to convert L [0-100]
     * to [0-255].
     *
     * Note I'm normalizing before doing the absolute colormetric clipping.
     * That probably makes some sort of difference which I'm not smart enough
     * to understand.
     */
    L_star = MIN( 255, MAX( 0, L_star*2.55 ));
    a_star = MIN( 255, MAX( 0, (a_star+127) ));
    b_star = MIN( 255, MAX( 0, (b_star+127) ));
      
    *P1 = L_star;
    *P2 = a_star;
    *P3 = b_star;
}

void
close_outfiles( FILE *outfiles[3] )
{
    int i;
    for( i=0 ; i<3 ; i++ ) {
        if( outfiles[i] != NULL ) {
            fclose( outfiles[i] );
            outfiles[i] = NULL;
        }
    }
}

int 
open_outfiles( const char *outfilename, FILE *outfiles[3] )
{
    int i;
    char tmp[FILENAME_MAX];
    const char *extensions[] = { ".L", ".a", ".b" };

    if( strlen( outfilename ) > FILENAME_MAX-10 ) {
        fprintf( stderr, "output filename too long\n" );
        return -1;
    }

    for( i=0 ; i<3 ; i++ ) {
        outfiles[i] = NULL;
    }

    for( i=0 ; i<3 ; i++ ) {
        strcpy( tmp, outfilename );
        strcat( tmp, extensions[i] );

        outfiles[i] = fopen( tmp, "wb" );
        if( outfiles[i] == NULL ) {
            fprintf( stderr, "unable to open %s for writing : %s\n", tmp,
                    strerror(errno));
            goto fail;
        }

    }

    return 0;
fail:
    close_outfiles( outfiles );
    return -1;
}

void
free_outbufs( uint8_t *outbufs[3] )
{
    int i;

    for( i=0 ; i<3 ; i++ ) {
        if( outbufs[i] ) {
            PTR_FREE( outbufs[i] );
        }
    }
}

int
allocate_outbufs( uint8_t *outbufs[3], size_t width )
{
    int i;

    for( i=0 ; i<3 ; i++ ) {
        outbufs[i] = NULL;
    }

    for( i=0 ; i<3 ; i++ ) {
        outbufs[i] = (uint8_t *)malloc( width );
        if( outbufs[i] == NULL ) {
            goto fail;
        }
    }

    return 0;
fail:
    free_outbufs( outbufs );
    return -1;
}

int
convert( const char *infilename, const char *outfilename )
{
    FILE *infile;
    FILE *out_files[3];
    uint8_t *out_bufs[3], *out_ptrs[3];
    int retcode, function_retcode;
    size_t bytes_read, pixels_read;
    uint8_t *buf, *ptr, *end_ptr;
    double p1, p2, p3;
    size_t chunk_size;

    function_retcode = -1;

    chunk_size = 65536;

    /* multiply by three assuming RGB pixel input */
    buf = (uint8_t *)malloc( chunk_size * 3 * sizeof(uint8_t) );
    if( buf==NULL ) {
        fprintf( stderr, "out of memory\n" );
        return -1;
    }

    infile = fopen( infilename, "rb" );
    if( infile==NULL ) {
        fprintf( stderr, "unable to open %s : %s\n", infilename,
                strerror(errno));

        free( buf );
        return -1;
    }

    retcode = open_outfiles( outfilename, out_files );
    if( retcode < 0 ) {
        /* open_outfiles() will log error */
        goto leave;
    }

    retcode = allocate_outbufs( out_bufs, chunk_size );
    if( retcode < 0 ) {
        goto leave;
    }

    while( 1 ) {
        /* multiply by three assuming RGB pixel input */
        bytes_read = fread( buf, 1, chunk_size*3, infile );
        if( bytes_read <= 0 ) {
            /* end of file or error */
            break;
        }
        
        pixels_read = bytes_read / 3;

        /* using pixels_read*3 instead of bytes_read so we'll round off any
         * weird non-mod-3 sized files rather than overrunning one of our
         * buffers (in other words, if I have a buffer where bytes_read%3 != 0
         * I don't want to walk off the end of one of the pixel buffers)
         */
        ptr = buf;
        end_ptr = buf + pixels_read*3;

        out_ptrs[0] = out_bufs[0];
        out_ptrs[1] = out_bufs[1];
        out_ptrs[2] = out_bufs[2];

        while( ptr < end_ptr ) {
            p1 = (double) *ptr++;
            p2 = (double) *ptr++;
            p3 = (double) *ptr++;

            RGB_to_XYZ( &p1, &p2, &p3 );
            XYZ_to_LAB( &p1, &p2, &p3 );

            *out_ptrs[0]++ = (uint8_t)(p1+0.5);
            *out_ptrs[1]++ = (uint8_t)(p2+0.5);
            *out_ptrs[2]++ = (uint8_t)(p3+0.5);
        }

        fwrite( out_bufs[0], 1, pixels_read, out_files[0] );
        fwrite( out_bufs[1], 1, pixels_read, out_files[1] );
        fwrite( out_bufs[2], 1, pixels_read, out_files[2] );
    }

    /* success! */
    function_retcode = 0;

leave:
    free( buf );
    fclose( infile );
    close_outfiles( out_files );
    return function_retcode;
}

void
usage( void )
{
    fprintf( stderr, "rgb2lab : convert raw RGB file to CIELAB\n" );
    fprintf( stderr, "usage: rgb2lab infile.rgb outfile-basename\n" );
    fprintf( stderr, "creates outfile-basename.L outfile-basename.a outfile-basename.b\n" );
    fprintf( stderr, "example: rgb2lab foo.rgb bar\n" );
    fprintf( stderr, "   will create bar.L, bar.a, and bar.b\n" );
}

int main( int argc, char *argv[] )
{
    const char *src_filename;
    const char *dst_filename;
    int retcode;

    if( argc != 3 ) {
        usage();
        exit(EXIT_FAILURE);
    }

    src_filename = argv[1];
    dst_filename = argv[2];

//    convert( "kazi_says_hi.rgb", 640, "kazi_says_hi" );
//    convert( "img_0510_copy.rgb", 620, "img_0510_copy" );

    retcode = convert( src_filename, dst_filename );
    if( retcode != 0 ) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

