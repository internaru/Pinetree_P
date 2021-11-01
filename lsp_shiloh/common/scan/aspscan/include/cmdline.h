/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef CMDLINE_H
#define CMDLINE_H

/* The command line is parsed into an array of key/value pairs. Any fields that
 * were specified on the command line have an entry in the list. 
 */

#define OPT_RESOLUTION   1
#define OPT_X_SCALE      2
#define OPT_Y_SCALE      3
#define OPT_DATA_TYPE    4
#define OPT_SCAN_TYPE    5
#define OPT_OUTFILENAME  6
#define OPT_AREA         7
#define OPT_STAT_HANDLE  8
#define OPT_CONTRAST     9
#define OPT_BRIGHTNESS  10
#define OPT_GAMMA       11
#define OPT_SHARPNESS   12
#define OPT_COPY_QUALITY 13
#define OPT_SCAN_TIMEOUT 14
#define OPT_USB_VID      15
#define OPT_USB_PID      16
#define OPT_CAL_TO_HOST  17
#define OPT_SERIAL_NUM   18
#define OPT_PAGE_NUMBERS 19
#define OPT_DUMP_SIQFILE 20
#define OPT_USB_INTERFACE     21
#define OPT_USB_OUT_ENDPOINT  22
#define OPT_USB_IN_ENDPOINT   23
#define OPT_BPP 24
#define OPT_FORCE_HTONL_DATA 25
#define OPT_FORCE_HTONS_DATA 26
#define OPT_DOCUMENT_SOURCE  27
#define OPT_NO_CAL 28
#define OPT_NO_PIE 29
#define OPT_ORIGINAL_CONTENT 30
#define OPT_IMPORT_SIQFILE 31

/* because I'm appending things to the end of the filename, enforce a shorter
 * filename than the system max
 */
#define MAX_OUTPUT_FILENAME (FILENAME_MAX/2)
 
struct cmdline_option {
    int key;

    /* not using a union here so I can know unequivicably when buf needs to be
     * free()'d; i.e., if( buf!=NULL ) free(buf)
     */

    /* generic value */
    uint32_t num32;

    /* e.g., xyscale */
    uint32_t numerator, denominator;

//    char *buf;
    void *buf;
    int buflen;
};

/* go nuts */
#define MAX_CMDLINE_OPTIONS 128

struct cmdline_option_list {
    struct cmdline_option options[MAX_CMDLINE_OPTIONS];
    int num_options; /* amount of used elements in our 'options' list */
};

struct cmdline_option * lookup_option( struct cmdline_option_list *option_list, int key );

void free_options( struct cmdline_option_list *option_list );

int parse_args( int argc, char *argv[], 
                struct cmdline_option_list *option_list );

void usage( const char *progname );

#endif

