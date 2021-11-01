/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef OUTFILE_H
#define OUTFILE_H

struct output_file {
    /* key */
    scan_data_type dtype;
    int page_side;

    /* value */
    FILE *filep;

    char filename[FILENAME_MAX+1];
};

#define MAX_OUTPUT_FILE_LIST 8

struct output_file_list {
    int list_count;

    struct output_file list[MAX_OUTPUT_FILE_LIST];
};

void output_file_list_init( struct output_file_list *outfile_list );
struct output_file * output_file_list_lookup( struct output_file_list *outfile_list, 
                                              scan_data_type dtype, int page_side );
struct output_file * output_file_list_fopen( struct output_file_list *outfile_list, 
                                             const char *filename,
                                              scan_data_type dtype, int page_side );
scan_err_t output_file_list_fclose( struct output_file_list *outfile_list, 
                                              scan_data_type dtype, int page_side );
void output_file_list_close_all( struct output_file_list *outfile_list );

#endif

