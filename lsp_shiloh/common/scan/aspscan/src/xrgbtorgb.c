/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#define VERSION "1.5.0" 

/* davep 30-May-05 ; short cheap program to convert from xRGB to RGB.
 */

/* davep 30-Nov-05 ; added -i and -o to try to speed up the program a bit (stop
 * going through pipes)
 */

/* must be evenly divisible by four; we make a read buffer of
 * exactly this size and a write buffer of 3/4 this size.
 */
#define BUFSIZE 0x100000  

FILE *infile, *outfile;
char infilename[FILENAME_MAX+1], outfilename[FILENAME_MAX+1];

void usage( const char *progname )
{
    fprintf( stderr, "%s %s %s: convert xRGB to RGB\n", 
            progname, VERSION, __DATE__ );
    fprintf( stderr, "usage:\n" );
    fprintf( stderr, "%s -i infile -o outfile\n", progname );
    fprintf( stderr, "With no arguments, reads from stdin and writes to stdout.\n" );
}

int parse_args( int argc, char *argv[] )
{
    int c;

    memset( infilename, 0, sizeof(infilename) );
    memset( outfilename, 0, sizeof(outfilename) );

    while( 1 ) {
        c = getopt( argc, argv, "hi:o:" );
        if( c==-1 ) {
            break;
        }

        switch( c ) {
            case 'h' :
                usage( argv[0] );
                goto fail;

            case 'i' :
                strncpy( infilename, optarg, FILENAME_MAX );
                break;

            case 'o' :
                strncpy( outfilename, optarg, FILENAME_MAX );
                break;

            default:
                fprintf( stderr, "Unknown command line option \"%s.\n", optarg);
                fprintf( stderr, "Run with \"-h\" to get usage.\n" );
                goto fail;
        }
    }

    if( infilename[0]==0 && outfilename[0]==0 ) {
        /* hardcode to read from stdin, output to stdout */
        infile = stdin;
        outfile = stdout;
    }
    else if( infilename[0]==0 ) {
        fprintf( stderr, "missing input filename\n" );
        usage( argv[0] );
        goto fail;
    }
    else if( outfilename[0]==0 ) {
        fprintf( stderr, "missing output filename\n" );
        usage( argv[0] );
        goto fail;
    }
    else {
        infile = fopen( infilename, "rb" );
        if( infile==NULL ) {
            fprintf( stderr, "Unable to open \"%s\" for reading : %s\n", 
                    infilename, strerror(errno) );
            goto fail;
        }
        outfile = fopen( outfilename, "wb" );
        if( outfile==NULL ) {
            fprintf( stderr, "Unable to open \"%s\" for writing : %s\n", 
                    outfilename, strerror(errno) );
            fclose( infile );
            goto fail;
        }
    }

    return 0;
fail:
    return -1;
}

int main( int argc, char *argv[] ) 
{
    uint8_t *inbuf, *outbuf;
    uint8_t *inptr, *outptr;
    int bytesread, byteswritten;
    int offset;
    int cnt;
    int retcode;

    retcode = parse_args( argc, argv );
    if( retcode < 0 ) {
        /* parse_args() reports error */
        exit(1);
    }


    inbuf = (uint8_t *)malloc( BUFSIZE );
    if( inbuf==NULL ) {
        fprintf( stderr, "Unable to allocate input buffer of size %d\n", BUFSIZE);
        return -1;
    }
    outbuf = (uint8_t *)malloc( (BUFSIZE * 3)/4 );
    if( outbuf==NULL ) {
        fprintf( stderr, "Unable to allocate output buffer of size %d\n", BUFSIZE/2 );
        free( inbuf );
        return -1;
    }

    while( 1 ) {

        bytesread = fread( inbuf, 1, BUFSIZE, infile );
        
        if( bytesread < 0 ) {
            fprintf( stderr, "failed to read from \"%s\" : %s\n", 
                    infilename, strerror(errno)  );
            break;
        }
        if( bytesread==0 )  {
            /* end of file */
            break;
        }

        if( bytesread % 4 != 0 ) {
            /* be tolerant of error; any extra data is ignored */
            fprintf( stderr, "warning: short read of %d (%d remaining bytes dropped)\n", 
                    bytesread, bytesread%4 );
            bytesread -= bytesread % 4;
        }

        /* copy 2nd, 3rd, 4th byte of every 4 bytes */
        offset = 0;
        cnt = 0;
        inptr = inbuf;
        outptr = outbuf;
        while( offset < bytesread ) {
            offset++;
            outptr[cnt++] = inptr[offset++];
            outptr[cnt++] = inptr[offset++];
            outptr[cnt++] = inptr[offset++];
        }

        byteswritten = 0;
        while( byteswritten < cnt ) { 
            retcode = fwrite( &outbuf[byteswritten], 1, cnt-byteswritten, outfile );
            if( retcode < 0 ) {
                fprintf( stderr, "failed to write to \"%s\" : %s\n",
                        outfilename, strerror(errno) );
                break;
            }
            else {
                byteswritten += retcode;
            }
        }
    }

    fclose( infile );
    fclose( outfile );

    return 0;
}

