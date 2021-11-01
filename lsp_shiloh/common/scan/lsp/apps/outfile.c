/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "outfile.h"

void output_file_list_init( struct output_file_list *outfile_list )
{
    memset( outfile_list, 0, sizeof(struct output_file_list) );
}

struct output_file * output_file_list_lookup( struct output_file_list *outfile_list, 
                                              scan_data_type dtype, int page_side )
{
    int i;
    struct output_file *outfile;

    for( i=0 ; i<MAX_OUTPUT_FILE_LIST ; i++ ) {
        outfile = &outfile_list->list[i];

        if( outfile->dtype==dtype && outfile->page_side==page_side ) {
            return outfile;
        }
    }

    return NULL;
}

static struct output_file * find_empty_slot( struct output_file_list *outfile_list )
{
    int i;
    struct output_file *outfile;

    for( i=0 ; i<MAX_OUTPUT_FILE_LIST ; i++ ) {
        outfile = &outfile_list->list[i];

        if( ! outfile->filep ) {
            memset( outfile, 0, sizeof(struct output_file) );
            return outfile;
        }
    }

    return NULL;
}

struct output_file * output_file_list_fopen( struct output_file_list *outfile_list,
                                             const char *filename,
                                             scan_data_type dtype, 
                                             int page_side )
{
    int retcode;
    struct output_file *outfile;

    outfile = find_empty_slot( outfile_list );
    if( outfile ==NULL ) {
        return NULL;
    }

    retcode = strnlen( filename, FILENAME_MAX );
    XASSERT( retcode<FILENAME_MAX-1, retcode );

    strncpy( outfile->filename, filename, FILENAME_MAX-1 );

    outfile->filep = fopen( outfile->filename, "wb" );
    if( !outfile->filep ) {
        fprintf( stderr, "unable to open \"%s\" for writing: (%d) %s\n",
                outfile->filename, errno, strerror(errno) );
        return NULL;
    }
    outfile->dtype = dtype;
    outfile->page_side = page_side;
    
    return outfile;
}

scan_err_t output_file_list_fclose( struct output_file_list *outfile_list,
                                             scan_data_type dtype, 
                                             int page_side )
{
    struct output_file *outfile;

    outfile = output_file_list_lookup( outfile_list, dtype, page_side );
    if( outfile==NULL ) {
        dbg1( "%s output file dtype=%d page_side=%d not found\n", __FUNCTION__,
                dtype, page_side );
        return SCANERR_NO_ENTRY;
    }

    XASSERT( outfile->filep != NULL, outfile->dtype );

    dbg1( "%s close \"%s\"\n", __FUNCTION__, outfile->filename );
	fflush( outfile->filep );
	fsync(fileno( outfile->filep ));		
    fclose( outfile->filep );
    memset( outfile, 0, sizeof(struct output_file) );

    return SCANERR_NONE;
}

void output_file_list_close_all( struct output_file_list *outfile_list )
{
    int i;
    struct output_file *outfile;

    for( i=0 ; i<MAX_OUTPUT_FILE_LIST ; i++ ) {
        outfile = &outfile_list->list[i];
        if( outfile->filep) {
            fclose( outfile->filep );
            memset( outfile, 0, sizeof(struct output_file) );
        }
    }
}

