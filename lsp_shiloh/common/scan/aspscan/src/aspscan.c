/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/* David Poole 26-Nov-05
 *
 * Capture ASP (ASP Scan Protocol) data using Linux libusb.
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>  /* for htonl */
#include <unistd.h>
#include <libusb.h>
#define __USE_GNU  /* for sighandler_t */
#include <signal.h>

#include "log.h"
#include "err.h"
#include "hdump.h"
#include "asp.h"
#include "aspusb.h"
#include "cmdline.h"
#include "usbtools.h"
#include "prod.h"
#include "xassert.h"

#define MAX_MSG  256

#define MIN(x,y)  ( (x)<(y)?(x):(y) )

#define PTR_FREE(ptr)  do { free(ptr); (ptr)=NULL; } while(0);

/* This global file list is losely indexed by the type of image data coming
 * back (see datatype_to_fileidx()).
 *
 * 3 files
 *  [0] - mono or xrgb output
 *  [0],[1],[2] - red, green, blue output for planar output
 */
#define FILE_OUT_MONO  0
#define FILE_OUT_XRGB  0
#define FILE_OUT_RGBX  0
#define FILE_OUT_RGB   0
#define FILE_OUT_RED   0
#define FILE_OUT_GREEN 1
#define FILE_OUT_BLUE  2
#define FILE_OUT_MAX   3

/* davep 17-Jun-2011 ; roll a pile of globals into page structure */
struct page_info {
    /* total pixels per row of real data and pixels per row of padded output we
     * have; incremented in
     */
    uint32_t pixels_per_row, pixels_per_row_padded;

    /* take bits per pixel from the data messages */
    int bits_per_pixel;

    /* rows in this page */
    uint32_t total_rows;

    /* SCAN_DATA_xxx from asp.h */
    uint32_t data_type;

    /* if >=0 , write a page number. otherwise, assume single page. */
    int page_number;

    /* The base filename. The extension can change depending on the file type;
     * there could be multiples of this file if we're doing a planar scan
     * (r,g,b).
     */
    char filename_base[FILENAME_MAX+1];

    /* davep 16-Apr-2013 ; adding page_side */
    uint8_t page_side;

    FILE *datafile[FILE_OUT_MAX];
};

/* davep 16-Apr-2013 ; adding page sides */
#define MAX_PAGE_SIDES 2

//FILE *g_datafile[FILE_OUT_MAX];

/* total number of rows we received; incremented in get_scan_data() */
int g_total_rows;

/* total number of pages we received (>1 when scanning with ADF) */
int g_total_pages;

/* options from the command line */
struct cmdline_option_list g_option_list;

/* see separate_and_save_icelite_data() for description of this global */
int g_icelite_rowcolor_idx;

bool g_add_page_side = false;
bool g_add_page_numbers = false;

// keep track of which sensor we are currently calibrating (blob sent from scanner)
uint8_t currently_calibrating_sensor_num = 0xFF; // initialize to invalid value

static void save_data( struct scan_header *hdr,
                       struct scan_data_header *datahdr, 
                       unsigned char *buf, int bufsize, 
                       struct page_info *page );

/* davep 23-Dec-2010 ; adding a simple ASP protocol state machine to catch the
 * firmware messing up
 */
typedef enum {
    SCAN_STATE_NULL=0,
    SCAN_STATE_IDLE=1,
    SCAN_STATE_STARTING=2,
    SCAN_STATE_START_JOB=3,
    SCAN_STATE_START_PAGE=4,
    SCAN_STATE_SCANNING=5,
    SCAN_STATE_END_PAGE=6,
    SCAN_STATE_END_JOB=7,
    SCAN_STATE_CLOSING=8,
} scan_state;

#define STATE_TRANSITION(new_state)  (state_transition( (new_state), __LINE__ ))

static scan_state g_curr_state = SCAN_STATE_NULL;

static void state_transition( scan_state new_state, int line_number )
{
    dbg2( "scan state from %d to %d at %d\n", 
                g_curr_state, new_state, line_number );

#define STATE_WARN(curstate,newstate)\
                dbg1( "%s WARNING! invalid protocol state transition %d -> %d\n", \
                            __FUNCTION__, (curstate), (newstate) );

    /* Sanity checks on the state to catch missing protocol elements. We're
     * simply trying to catch the firmware screwing up.
     */
    switch( g_curr_state ) {
        case SCAN_STATE_START_JOB : 
            /* davep 05-Jan-2011 ; add START->IDLE which happens on an error */
            if( new_state != SCAN_STATE_START_PAGE && new_state != SCAN_STATE_IDLE ) {
                STATE_WARN( g_curr_state, new_state );
            }
            break;

        case SCAN_STATE_START_PAGE : 
            if( new_state != SCAN_STATE_SCANNING ) {
                STATE_WARN( g_curr_state, new_state );
            }
            break;

        case SCAN_STATE_SCANNING : 
            if( new_state != SCAN_STATE_SCANNING && new_state != SCAN_STATE_END_PAGE ) {
                STATE_WARN( g_curr_state, new_state );
            }
            break;

        case SCAN_STATE_END_PAGE : 
            if( new_state != SCAN_STATE_END_JOB && new_state != SCAN_STATE_START_PAGE ) {
                STATE_WARN( g_curr_state, new_state );
            }
            break;

        default : 
            /* ignore */
            break;
    }

    g_curr_state = new_state;
}

static int openfile( struct page_info *page, int index, const char *filename )
{
    dbg2( "%s index=%d filename=%s\n", __FUNCTION__, index, filename );

    if( index < 0 || index >= FILE_OUT_MAX ) {
        errorlog( "internal error; bad file index\n" );
        return ERR_INTERNAL;
    }

    page->datafile[index] = fopen( filename, "w" );
    if( page->datafile[index] == NULL ) {
        errorlog( "unable to open datafile \"%s\" for writing : %s\n", 
                filename, strerror(errno) );
        return ERR_FAIL;
    }
    
    return 0;
}

static void closefile( struct page_info *page, int index ) 
{
    if( index < FILE_OUT_MAX && page->datafile[index] ) {
        fclose( page->datafile[index] );
        page->datafile[index] = NULL;
    }
}

static void close_output_files( struct page_info *page )
{
    int i;

    dbg1( "%s page_side=%d\n", __FUNCTION__, page->page_side );

    for( i=0 ; i<FILE_OUT_MAX ; i++ ) {
        if( page->datafile[i] != NULL ) {
            fclose( page->datafile[i] );
            page->datafile[i] = NULL;
        }
    }
}

static int setup_output_files( struct page_info *page )
{
    int retcode;
    struct cmdline_option *opt_outfile;
    char filename[FILENAME_MAX+1];
    int len;

    /* davep 16-Apr-2013 ; adding page_side */

    dbg1( "%s data_type=%d page_number=%d page_side=%d\n", __FUNCTION__, 
                page->data_type, page->page_number, page->page_side );

    opt_outfile = lookup_option( &g_option_list, OPT_OUTFILENAME );

    /* we should have required the output filename option in parse_args() */
    if( opt_outfile==NULL ) {
        errorlog( "internal error; no output filename specified\n" );
        return ERR_INTERNAL;
    }

    /* opt_outfile->buf should have been verified to be much smaller than
     * FILENAME_MAX in parse_cmdline()-cmdline.c
     */
    if( opt_outfile->buflen+30 >= FILENAME_MAX ) {
        errorlog( "internal error; filename too long to append extensions\n" );
        return ERR_INTERNAL;
    }

    /* don't add page side if we are doing cal to host */
    if (lookup_option(&g_option_list, OPT_CAL_TO_HOST) != NULL)
    {
        strncpy(filename, opt_outfile->buf, FILENAME_MAX);
    }
    /* davep 18-Apr-2013 ; TODO how to only add page side only when dual scan? */
    else if (g_add_page_side)
    {
        
        /* add page_side */
        /* don't append page number unless we're given a positive number */
        if( page->page_number >=0  ) {
            snprintf( filename, FILENAME_MAX, "%s_s%d_p%d", 
                        (char *)opt_outfile->buf, page->page_side, page->page_number );
        }
        else {
            snprintf( filename, FILENAME_MAX, "%s_s%d", 
                        (char *)opt_outfile->buf, page->page_side );
        }
    }
    else
    {
        /* don't add page_side */
        /* don't append page number unless we're given a positive number */
        if( page->page_number >=0  ) {
            snprintf( filename, FILENAME_MAX, "%s_p%d", (char *)opt_outfile->buf, page->page_number );
        }
        else {
            strncpy( filename, opt_outfile->buf, FILENAME_MAX );
        }
    }

    /* davep 20-Jun-2011 ; save a copy of the filename base into my new page structure */
    strncpy( page->filename_base, filename, FILENAME_MAX );

    /* add an extension, open the output file */
    switch( page->data_type ) {
        case SCAN_DATA_RGB_PACKED :
            strcat( filename, ".rgb" );
            retcode = openfile( page, FILE_OUT_RGB, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                return retcode;
            }
            break;

        case SCAN_DATA_XRGB_PACKED :
            strcat( filename, ".xrgb" );
            retcode = openfile( page, FILE_OUT_XRGB, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                return retcode;
            }
            break;

        case SCAN_DATA_RGBX_PACKED : 
            strcat( filename, ".rgbx" );
            retcode = openfile( page, FILE_OUT_RGBX, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                return retcode;
            }
            break;

        case SCAN_DATA_PLANAR :
            strcat( filename, ".r" );
            len = strlen( filename );
            retcode = openfile( page, FILE_OUT_RED, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                return retcode;
            }
           
            /* note: fall through! */

            /* change from '.r' to to a '.g' file */
            filename[len-1] = 'g';
            retcode = openfile( page, FILE_OUT_GREEN, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                closefile( page, FILE_OUT_RED );
                return retcode;
            }

            /* note: fall through! */

            /* change from '.g' to to a '.b' file */
            filename[len-1] = 'b';
            retcode = openfile( page, FILE_OUT_BLUE, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                closefile( page, FILE_OUT_RED );
                closefile( page, FILE_OUT_GREEN );
                return retcode;
            }
            break;

        case SCAN_DATA_MONO :
            strcat( filename, ".gray" );
            retcode = openfile( page, FILE_OUT_MONO, filename );
            if( retcode < 0 ) {
                /* openfile() logs error */
                return retcode;
            }
            break;

        default :
            errorlog( "internal error; unexpected data_type %d\n", page->data_type );
            return ERR_INTERNAL;
    }

    return 0;
}

/**
 * \brief  htonl() the scan data packet's data
 *
 * As of this writing, PIE and PIC can be big or little endian but we're stuck
 * with big for now. Currently we're getting PIE big endian bytes. 
 *
 * TODO add PIC and PIE endian to platform string, decode the data bytes based
 * on the platform string
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

static void htonl_data( struct scan_data_header *datahdr, unsigned char *buf )
{
    uint32_t i;
    uint32_t *ptr32, *endptr32;
    int bytes_per_row;
    struct cmdline_option *force;

    /* davep 03-May-2011 ; adding force htonl option to command line to help
     * with debugging goofy images
     */
    force = lookup_option( &g_option_list, OPT_FORCE_HTONL_DATA );
    if( force && force->num32==0 ) { 
        /* command line option force to skip htonl */
        return;
    }

    dbg2( "%s buf=%p numrows=%d\n", __FUNCTION__, buf, datahdr->numrows );

    /* sanity check! for now, only do this on xrgb or rgbx or rgb packed */
    XASSERT( datahdr->bytes_per_pixel==3 || datahdr->bytes_per_pixel==4, datahdr->bytes_per_pixel );

    bytes_per_row = datahdr->pixels_per_row_padded * datahdr->bytes_per_pixel;

    for( i=0 ; i<datahdr->numrows ; i++ ) {

        ptr32 = (uint32_t*)(buf + (i*bytes_per_row));
        endptr32 = (uint32_t *)(buf + ((i+1)*bytes_per_row));

        while( ptr32 < endptr32 ) {
            *ptr32 = htonl(*ptr32);
            ptr32++;
        }
    }
}

static void htons_data( struct scan_data_header *datahdr, unsigned char *buf )
{
    uint32_t i;
    uint16_t *ptr16, *endptr16;
    int bytes_per_row;
    struct cmdline_option *force;

    /* davep 03-May-2011 ; adding force htonl option to command line to help
     * with debugging goofy images
     */
    force = lookup_option( &g_option_list, OPT_FORCE_HTONS_DATA );
    if( force && force->num32==0 ) { 
        /* command line option force to skip htons */
        return;
    }

    dbg2( "%s buf=%p numrows=%d\n", __FUNCTION__, buf, datahdr->numrows );

    bytes_per_row = datahdr->pixels_per_row_padded * datahdr->bytes_per_pixel;

    for( i=0 ; i<datahdr->numrows ; i++ ) {

        ptr16 = (uint16_t*)(buf + (i*bytes_per_row));
        endptr16 = (uint16_t *)(buf + ((i+1)*bytes_per_row));

        while( ptr16 < endptr16 ) {
            *ptr16 = htons(*ptr16);
            ptr16++;
        }
    }
}

static int datatype_to_fileidx( uint32_t datatype )
{
    switch( datatype ) {
        case SCAN_DATA_RGB_PACKED :
            return FILE_OUT_RGB;

        case SCAN_DATA_XRGB_PACKED :
            return FILE_OUT_XRGB;

        case SCAN_DATA_RGBX_PACKED :
            return FILE_OUT_RGBX;

        case SCAN_DATA_RED :
            return FILE_OUT_RED;

        case SCAN_DATA_GREEN :
            return FILE_OUT_GREEN;

        case SCAN_DATA_BLUE :
            return FILE_OUT_BLUE;

        case SCAN_DATA_MONO :
        case SCAN_DATA_MONO_MIRRORED :
            return FILE_OUT_MONO;

        case SCAN_DATA_MONO_HALFPACK_1BPP :
        case SCAN_DATA_MONO_HALFPACK_2BPP :
        case SCAN_DATA_MONO_HALFPACK_4BPP :
            return FILE_OUT_MONO;

        case SCAN_DATA_BRG_INTERLACED :
        case SCAN_DATA_BRG_INTERLACED_MIRRORED :
            return FILE_OUT_MONO;

        default :
            XASSERT( 0, datatype );
            return -1;
    }
}

// user passes down a ptr to a TLD, we return the T (type)
static int get_blob_type(uint8_t *buf)
{
    uint32_t *ptr;
    int retval;

    ptr = (uint32_t *) buf;
    retval = htonl(ptr[0]); // type is the 1st 4 byte value in the tld
    return retval;
}

// user passes down a ptr to a TLD, we return the D (type)
// (only return a 4 byte value)
static uint32_t get_blob_int_data(uint8_t *buf)
{
    uint32_t *ptr;
    uint32_t retval;

    ptr = (uint32_t *) buf;
    retval = htonl(ptr[2]); // type is the 3rd 4 byte value in the TLD
    return retval;
}

static void separate_and_save_icelite_data( struct scan_data_header *datahdr, 
                                            unsigned char *buf, int bufsize,
                                            struct page_info *page )
{
    int i;
    unsigned int row_idx;
    struct scan_data_header datahdr_list[3];
    unsigned char *ptr, *endptr, *buf_list[3];
    int numrows_list[3];
    int bytes_per_row;

    dbg2( "%s bufsize=%d\n", __FUNCTION__, bufsize );

    /* We'll separate the BRG data into three buffers then call save_data()
     * with a fake scan_data_header for each buffer. We pretend we're getting
     * multiple individual buffers of Blue, Red, Green.
     */
    for( i=0 ; i<3 ; i++ ) {
        memcpy( &datahdr_list[i], datahdr, sizeof(struct scan_data_header) );
    }
    /* note the f'd up color order compliments of ICE Lite */
    datahdr_list[0].data_type = SCAN_DATA_BLUE;
    datahdr_list[1].data_type = SCAN_DATA_RED;
    datahdr_list[2].data_type = SCAN_DATA_GREEN;

    memset( buf_list, 0, sizeof(buf_list) );
    memset( numrows_list, 0, sizeof(numrows_list) );

    /* save one row of data at a time */
    bytes_per_row = datahdr->bytes_per_pixel * datahdr->pixels_per_row_padded;

    XASSERT( (uint32_t)bufsize==bytes_per_row*datahdr->numrows, bufsize );

    /* So, about g_icelite_rowcolor_idx.
     *
     * We need to round robin through the fake data headers in the correct
     * order we can keep the data order staight. Because the ICELite data can
     * cross buffers, we won't know where the incoming 'buf' starts in the BRG
     * order.
     *
     * Thus the global to track the color index across incoming buffers.
     *
     * For example, 10 rows per buffer gives:
     *
     * BRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRGBRG...
     * 012345678901234567890123456789012345678901234567890123456789012345...
     * |--------||--------||--------||--------||--------||--------||-----...
     * B         R         G         B         R         G         B
     * 012012012012012012012012012012012012012012012012012012012012012012...
     *
     */

    ptr = buf;
    endptr = buf + bufsize;
    for( row_idx=0 ; row_idx<datahdr->numrows ; row_idx++ ) {
        save_data( NULL, &datahdr_list[g_icelite_rowcolor_idx], ptr, bytes_per_row, page );
        ptr += bytes_per_row;
        XASSERT( ptr <= endptr, (unsigned long int)ptr );
        g_icelite_rowcolor_idx = (g_icelite_rowcolor_idx+1) % 3;
    }

}

static void save_data( struct scan_header *hdr,
                       struct scan_data_header *datahdr, 
                       unsigned char *buf, int bufsize, 
                       struct page_info *page )
{
    int retcode, datalen;
    unsigned char *ptr;
    int fileidx;
    uint32_t num32;
    uint8_t page_side;

    fileidx = datatype_to_fileidx( datahdr->data_type );

    dbg2( "%s data_type=%d fileidx=%d buf=%p bufsize=%d\n", __FUNCTION__,
                datahdr->data_type, fileidx, buf, bufsize );

    if( fileidx < 0 || page->datafile[fileidx]==NULL ) {
        /* Note we could flood the logs if we get a lot of bad messages coming
         * back but I didn't want to create a global var for a relatively
         * unlikely problem. I want to know about it as soon as it happens and
         * fix the problem.
         */
        errorlog( "ignore all unknown/unexpected data type %d\n", datahdr->data_type );
        return;
    }

    if( datahdr->data_type==SCAN_DATA_BRG_INTERLACED ||
            datahdr->data_type==SCAN_DATA_BRG_INTERLACED_MIRRORED ) {
        dbg1( "icelite BRG interlaced data encountered\n");
        separate_and_save_icelite_data( datahdr, buf, bufsize, page);
        return;
    }

    // hdr should only be NULL for separate_and_save_icelite_data() calls
    page_side = 0;
    if (hdr != NULL)
        scan_msg_image_data_decode( hdr, &page_side );
    
    /* davep 01-Dec-2008 ; if we're doing cal-to-host, stamp the
     * Type+Length header into the file so we can parse the cal data
     * properly (see calparse.py)
     */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) != NULL ) {
        if (page_side != currently_calibrating_sensor_num)
        {
            printf("page_side %d != current sensor %d, throwing away data\n",page_side, currently_calibrating_sensor_num);
            return; // this is data from the other (non-calibrating side), bit bucket it
        }

        // write a TLD for the data itself
        /* type */
        num32 = htonl( datahdr->data_type );
        fwrite( &num32, 1, sizeof(uint32_t), page->datafile[fileidx] );
        /* length */
        num32 = htonl( bufsize );
        fwrite( &num32, 1, sizeof(uint32_t), page->datafile[fileidx] );
    }

    ptr = buf;
    datalen = bufsize;

    /* davep 17-Dec-2010 ; if we're doing XRGB, RGBX, or RGB packed, we'll need
     * to htonl the entire buffer
     * TODO - read big/little endianness of PIE via the platform string
     */
    /* davep 14-Jun-2011 ; don't swap RGB packed */
//    if( datahdr->data_type==SCAN_DATA_XRGB_PACKED ||
//        datahdr->data_type==SCAN_DATA_RGBX_PACKED ) 
    if( datahdr->bytes_per_pixel==4 ) {
        htonl_data( datahdr, ptr );
    }

    /* davep 20-Jun-2011 ; XXX fiddle with big-endian data */
    if( 0 ) {
//    if( datahdr->bytes_per_pixel==2 ) {
        htons_data( datahdr, ptr );
    }

    while( datalen > 0 ) {
        retcode = fwrite( ptr, 1, datalen, page->datafile[fileidx] );
        if( retcode < 0 ) {
            errorlog( "write to data file failed : %s\n", strerror(errno) );
            fclose( page->datafile[fileidx] );
            page->datafile[fileidx] = NULL;
            break;
        }
        ptr += retcode;
        datalen -= retcode;
    }
}

/**
 * \brief write ONLY a PNM header.
 *
 * Write JUST the header to a file. 
 *
 * \author David Poole
 * \date 17-Jun-2011
 *
 */

static void write_pnm_header( const struct page_info *page )
{
    int retcode;
    FILE *outfile;
    char outfilename[FILENAME_MAX+1];
    uint32_t total_rows;
    const char *extension;
    const char *magic_number;
    uint32_t max_pixel;

    /* davep 27-Jun-2011 ; don't write PNM if we're doing cal-to-host */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) ) {
        return;
    }

    /* stupid human checks */
    ASSERT( page->filename_base[0] != 0 );
    ASSERT( page->bits_per_pixel != 0 );
    ASSERT( page->pixels_per_row_padded != 0 );

    /* note total_rows can be zero! I'm using this function to write a
     * temporary header when the first data packet arrives. If we crash partway
     * through the scan, at least we'll have the pixels_per_row_padded and the
     * bpp 
     */

    if( page->data_type==SCAN_DATA_MONO ) {
        extension = "p5";
        magic_number = "P5";
    }
    else {
        extension = "p6";
        magic_number = "P6";
    }

    retcode = snprintf( outfilename, FILENAME_MAX, "%s.%s", 
                    page->filename_base, extension );
    XASSERT( retcode<FILENAME_MAX, retcode );

    dbg1( "%s writing \"%s\"\n", __FUNCTION__, outfilename );

    total_rows = page->total_rows;
    if( page->data_type==SCAN_DATA_PLANAR ) {
        /* we received planar data */
        total_rows /= 3;
    }

    /* This max_pixel could be wrong if we're scanning copy-to-host (1,2,4-bpp)
     * or xrgb/rgbx (32-bpp). Make our best guess and let the user sort out any
     * problems.
     */
    max_pixel = 255;
    if( page->bits_per_pixel==16 ) {
        max_pixel = 65535;
    }

    outfile = fopen( outfilename, "wb" );
    fprintf( outfile, "%s\n", magic_number );
    fprintf( outfile, "%d %d\n", page->pixels_per_row_padded, total_rows );
    fprintf( outfile, "%d\n", max_pixel );
    fclose( outfile );
}


int start_job( libusb_device_handle *udev, struct scan_header *hdr, 
                struct scan_job *job_settings, struct page_info *page )
{
    int retcode;
    char msg[MAX_MSG];

    dbg2( "%s\n", __FUNCTION__ );

    /* quiet a warning */
    hdr = hdr;

    /* the startjob response contains a scan_job_settings */
    memset( job_settings, 0, sizeof(struct scan_job) );
    retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(),
                        (unsigned char *)job_settings, 
                        sizeof(struct scan_job), 5*USB_SECONDS );
    dbg2( "aspusb_read returned %d\n", retcode );

    if( retcode < 0 ) {
        errorlog( "aspusb_read failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    hex_dump( (unsigned char *)job_settings, sizeof(struct scan_job) );

    asp_swap_job_fields( job_settings );

    asp_scan_job_to_str( msg, MAX_MSG, job_settings); 
    dbg1( "%s\n", msg );

    /* davep 22-Jul-2013 ; if firmware is sending us dithered, just say we're
     * getting monochrome. My dithered FW is 8-bpp mono so we'll follow the
     * 8-bpp mono path
     */
    if( job_settings->data_type==SCAN_DATA_BILEVEL ) {
        job_settings->data_type = SCAN_DATA_MONO;
    }
    
    /* davep 08-Jul-2009 ; blob data comes before the start_page so we have to
     * open our files in the start_job (see also comments in start_page())
     */
    /* davep 20-Jun-2011 ; pass our new page_info structure to
     * setup_output_files()
     */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) != NULL ) {
        memset( page, 0, sizeof(struct page_info) );
        /* pass -1 to not use page numbers on the filename */
        page->page_number = -1;
        page->data_type = job_settings->data_type;
        retcode = setup_output_files( page );
        if( retcode != 0 ) {
            return retcode;
        }
    }

    return 0;
}

int end_job( struct page_info *page )
{
    dbg2( "%s\n", __FUNCTION__ );

    /* davep 08-Jul-2009 ; if we're doing cal-to-host, the start_job/end_job
     * messages open/close the files (see also comments in start_page())
     */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) != NULL ) {
        close_output_files(page);
    }

    return 0;
}

int start_page( uint32_t job_data_type, int page_number, uint8_t page_side, struct page_info *page )
{
    int retcode;

    dbg2( "%s page_number=%d page_side=%d\n", __FUNCTION__, page_number, page_side );

    retcode = 0;

    /* davep 08-Jul-2009 ; recent protocol changes mean we're going to get
     * multiple start_page/end_page during a cal-to-host. Plus, we'll be
     * getting the blob data after the start_job but before the start_page.
     * We'll need to open our cal-to-host files in start_job() and our other
     * scan files here in start_page().
     */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) == NULL ) {
        /* davep 17-Jun-2011 ; roll globals into page structure */
        memset( page, 0, sizeof(struct page_info) );
        /* preserve a few fields */
        page->data_type = job_data_type;
        page->page_number = page_number;

        /* davep 16-Apr-2013 ; adding page_side */
        page->page_side = page_side;

        /* We're not doing a cal-to-host.  
         *
         * Set the global (ack!) list of filenames we'll write data to. We
         * could get multiple files in a single scan (think ADF) so we have to
         * open a new file whenever a start_page arrives.
         */
        retcode = setup_output_files( page );
        /* setup_output_files() logs error if something goes wrong */

    }

    return retcode;
}

int end_page( struct page_info *page )
{
    dbg2( "%s page_side=%d\n", __FUNCTION__, page->page_side );

    /* davep 08-Jul-2009 ; if we're doing cal-to-host, the start_job/end_job
     * messages open/close the files (see also comments in start_page())
     */
    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) == NULL ) {
        close_output_files(page);
    }

    return 0;
}

/* Global pointer to hold malloc'd buffer to hold data read during
 * SMSG_PAGE_DATA. Allocated (and possibly reallocated) during incoming
 * buffers. Freed at end of program.
 */
static struct {
    uint8_t *buf;
    uint8_t bufsize_bytes;
} data_msg_buffer;

int get_scan_data( libusb_device_handle *udev, struct scan_header *hdr, 
                        struct page_info page_list[] )
{
    int retcode, datalen;
    struct scan_data_header *datahdr;
    unsigned char *ptr;
    char msg[MAX_MSG];
    int save_errno;
    uint8_t page_side;
    struct page_info *page;

    /* davep 9-dec-2006 ; sanity check against a really broken protocol (8M
     * max which should handle most stuff ) 
     */
    if( hdr->datalen > 0x800000 ) {
        errorlog( "header datalen %d too large! (max expected=%d)\n",
                hdr->datalen, 0x800000 );
        return ERR_BAD_PROTOCOL;
    }

    /* davep 16-Apr-2013 ; adding page_side */
    scan_msg_image_data_decode( hdr, &page_side );
    XASSERT( page_side<MAX_PAGE_SIDES, page_side );


    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) )
    {
        // we only output to 1 file for cal to host (don't use page side)
        page = &page_list[0];
    }
    else
    {
        page = &page_list[page_side];
    }

    /* if we have a previous buffer but that buffer is smaller than what we
     * want, resize it to fit what we need now
     */
    if( data_msg_buffer.buf && data_msg_buffer.bufsize_bytes < hdr->datalen ) {
        data_msg_buffer.buf = realloc( data_msg_buffer.buf, hdr->datalen );
        if( data_msg_buffer.buf != NULL ) {
            data_msg_buffer.bufsize_bytes = hdr->datalen;
        }
    }

    if( data_msg_buffer.buf==NULL ) {
        data_msg_buffer.bufsize_bytes = hdr->datalen;
        data_msg_buffer.buf = malloc( hdr->datalen );
        if( data_msg_buffer.buf==NULL ) {
            dbg1( "%s out of memory numbytes=%d\n", __FUNCTION__, hdr->datalen );
            return ERR_OUT_OF_MEMORY;
        }
    }

    ptr = data_msg_buffer.buf;
    datalen = hdr->datalen;
    while( datalen > 0 ) {

        retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(), ptr, 
                                 MIN(datalen,16384), 5*USB_SECONDS );

//        dbg2( "%s aspusb_read retcode=%d\n", __FUNCTION__, retcode );

        /* capture errno immediately (before any debugging statements) */
        save_errno = errno;

        if( retcode==0 ) {
            dbg2( "%s no data; wait some more\n", __FUNCTION__ );
        }
        else if( retcode < 0 ) {
            errorlog( "%s aspusb_read failed : retcode=\"%s\" errno=\"%s\"\n", 
                    __FUNCTION__, get_libusb_error_string(retcode), strerror(save_errno) );

            return ERR_USB_FAIL;
        }
        else { 
            datalen -= retcode;
            ptr += retcode;
        }
    }

    datahdr = (struct scan_data_header *)data_msg_buffer.buf;
    asp_swap_data_header_fields( datahdr );

    /* XXX get rid of these debugging message once we get this program working.
     * They'll slow down the data capture way too much.
     */
    asp_data_header_to_str( msg, MAX_MSG, datahdr );
    dbg2( "%s\n", msg );

    /* davep 21-Apr-2011 ; capture bits-per-pixel so we can accurately report
     * to scan.py (adding copy-to-host decode to scan.py)
     *
     * We designed the protocol BADLY. The DataHeader uses 'bytes per pixel'
     * instead of BITS per pixel. When I later added copy-to-host, I hacked in
     * a kludge. If bytes_per_pixel < 0, then bytes_per_pixel => bits_per_pixel.
     * See the bytes_per_pixel_map[] in scanapp.c
     */
    int n, bits_per_pixel;
    n = (int)datahdr->bytes_per_pixel;
    if( n < 0 ) {
        bits_per_pixel = -n;
    }
    else {
        bits_per_pixel = datahdr->bytes_per_pixel * 8; 
    }
    if( page->bits_per_pixel==0 ) {
        page->bits_per_pixel = bits_per_pixel;
    }
    else {
        XASSERT( page->bits_per_pixel==bits_per_pixel, page->bits_per_pixel );
    }

    /* ppr and pprp should be constant across the entire scan; save here so we
     * can dump out at the end of the program
     */
    if( page->pixels_per_row==0 ) {
        /* save the ppr, pprp */
        page->pixels_per_row = datahdr->pixels_per_row;
        page->pixels_per_row_padded = datahdr->pixels_per_row_padded;

        /* this should be our first data packet in this page; write a pnm
         * header with just the pixels_per_row_unpadded; if we crash, we can
         * at least have a hint at the data size. The total_rows of zero forces
         * us humans to know the header is bad.
         */
        XASSERT( page->total_rows==0, page->total_rows );
        write_pnm_header( page );
    }
    else {
        XASSERT( page->pixels_per_row==datahdr->pixels_per_row, page->pixels_per_row );
        XASSERT( page->pixels_per_row_padded==datahdr->pixels_per_row_padded, 
                 page->pixels_per_row_padded );
    }

    /* track the total number of rows we receive to make decoding the data
     * easier when we're done
     */
    page->total_rows += datahdr->numrows;
    g_total_rows += datahdr->numrows;
    
    /* davep 17-Jun-2011 ; don't write the data packet data type into the page
     * structure because the data type can change if we're scanning planar
     * (r,g,b) data. I'm currently using the data type to decode how many rows
     * to stamp into the PPM header. If planar, then divide by three before
     * writing the row count.
     */

    ptr = data_msg_buffer.buf + sizeof(struct scan_data_header);
    datalen = hdr->datalen - sizeof(struct scan_data_header);
    save_data( hdr, datahdr, ptr, datalen, page );

    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 07-Sep-2010
 *
 */

static int read_data( libusb_device_handle *udev, unsigned char *buf, int buflen )
{
    unsigned char *ptr;
    int retcode, datalen;

    ptr = buf;
    datalen = buflen;
    while( datalen > 0 ) {
        retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(), ptr, 
                                 datalen, 5*USB_SECONDS );

        dbg2( "aspusb_read of %d retcode=%d\n",
                    datalen, retcode);

        if( retcode < 0 ) {
            errorlog( "aspusb_read failed : %s\n", strerror(errno) );
            return ERR_USB_FAIL;
        }
        datalen -= retcode;
        ptr += retcode;
    }

    return ERR_NONE;
}

int get_data_blob( libusb_device_handle *udev, 
                   struct scan_header *hdr, 
                   struct page_info *page )
{
    int i, retcode, datalen;
    unsigned char *buf, *ptr;
    bool data_written;

    dbg2( "%s\n", __FUNCTION__ );

    /* davep 08-Jul-2009 ; XXX temp debug */
    char debug[1024];
    asp_scan_header_to_str( debug, 1024, hdr );
    dbg1( "%s %s\n", __FUNCTION__, debug );

    /* davep 9-dec-2006 ; sanity check against a really broken protocol (8M
     * max which should handle most stuff ) 
     */
    if( hdr->datalen > 0x800000 ) {
        errorlog( "header datalen %d too large! (max expected=%d)\n",
                hdr->datalen, 0x800000 );
        return ERR_BAD_PROTOCOL;
    }

    buf = malloc( hdr->datalen );
    if( buf==NULL ) {
        errorlog( "out of memory\n" );
        return ERR_OUT_OF_MEMORY;
    }

    retcode = read_data( udev, buf, hdr->datalen );
    if( retcode < 0 ) {
        PTR_FREE( buf );
        errorlog( "read_data() failed : retcode=%d\n", retcode );
        return retcode;
    }

//    hex_dump( (unsigned char *)buf, hdr->datalen );

    if( lookup_option( &g_option_list, OPT_CAL_TO_HOST ) == NULL ) {
        /* if we're not doing cal-to-host, ignore the blob */
        dbg2( "not doing cal-to-host so ignoring the blob\n" );
        PTR_FREE( buf );
        return 0;
    }

    // Because we only wish to keep cal data for the sensor we are currently calibrating, it is
    // important to now parse this blob to track what sensor we care about for data. 
    if (get_blob_type(buf) == CAL_METADATA_NOW_CALIBRATING_SENSOR_NUM)
    {
        currently_calibrating_sensor_num = (uint8_t) get_blob_int_data(buf);
    }
         
    /* Write the blob to each and every datafile.
     *
     * Blobs were originally created to support calibration meta data in
     * cal-to-host. The cal metadata will apply to each data capture: mono or
     * R,G,B files.
     *
     * It's simpler to write the data to the output file and let calparse
     * decode.
     */
    data_written = false;
    for( i=0 ; i<FILE_OUT_MAX ; i++ ) {
        if( page->datafile[i]==NULL ) {
            continue;
        }

        ptr = buf;
        datalen = hdr->datalen;

        while( datalen > 0 ) {
            retcode = fwrite( ptr, 1, datalen, page->datafile[i] );
            if( retcode < 0 ) {
                errorlog( "write to data file failed : %s\n", strerror(errno) );
                fclose( page->datafile[i] );
                page->datafile[i] = NULL;
                break;
            }
            ptr += retcode;
            datalen -= retcode;
        }
        dbg2( "%s wrote bytes=%d to file=%p\n", __FUNCTION__, 
                    hdr->datalen, page->datafile[i] );
        data_written = true;
    }

    if( !data_written ) {
        errorlog( "%s no file to write blob!\n", __FUNCTION__ );
    }

    /* yes, it's pretty inefficient to malloc a buffer each time */
    PTR_FREE( buf );

    return 0;
}

int
aspscan_query_adf( libusb_device_handle *udev, int *adf_status )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    char msg[MAX_MSG];

    dbg2( "%s\n", __FUNCTION__ );

    *adf_status = SCAN_ADF_STATUS_UNKNOWN;

    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_ADF_IS_PAPER_PRESENT; 

    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        /* aspusb_get_header() logs error */
        return retcode;
    }

    asp_scan_header_to_str( msg, MAX_MSG, &rcvpkt ); 
    dbg2( "%s\n", msg );

    /* make sure we got back at least what we were expecting */
    if( rcvpkt.msg != SCAN_MSG_ADF_IS_PAPER_PRESENT) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_ADF_IS_PAPER_PRESENT, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    /* firmware will respond with a fail message if it doesn't have an ADF
     * (e.g., it's a flatbad scanner)
     */
    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        *adf_status = SCAN_ADF_STATUS_NOT_ADF;
    }
    else if( rcvpkt.param1 ) {
        /* param1 is a boolean indicating paper/nopaper */
        *adf_status = SCAN_ADF_STATUS_PAPER_PRESENT;
    }
    else {
        *adf_status = SCAN_ADF_STATUS_NO_PAPER_PRESENT;
    }

    return 0;
}

static int
aspscan_lock( libusb_device_handle *udev, uint32_t timeout )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    char msg[MAX_MSG];

    dbg2( "%s timeout=%d\n", __FUNCTION__, timeout );

    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_LOCK_SCAN_RESOURCE; 
    pkt.param1 = timeout;

    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        /* aspusb_get_header() logs error */
        return retcode;
    }

    asp_scan_header_to_str( msg, MAX_MSG, &rcvpkt ); 
    dbg2( "%s\n", msg );

    /* make sure we were able to lock it */
    if( rcvpkt.msg != SCAN_MSG_LOCK_SCAN_RESOURCE ) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_LOCK_SCAN_RESOURCE, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        errorlog( "!!! Scan failed: scanner is busy!          !!!\n" );
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        return ERR_DEV_BUSY;
    }

    return 0;
}

int 
aspscan_unlock( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    int function_retcode;

    dbg2( "%s\n", __FUNCTION__ );

    memset( &pkt, 0, sizeof(struct scan_header) );

    pkt.msg = SCAN_MSG_RELEASE_SCAN_RESOURCE; 
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    function_retcode = 0;

    /* davep 18-Sep-2007 ; could receive an ABORT here so have to loop around
     * the read 
     */
    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        /* aspusb_get_header() logs error */
        return retcode;
    }

    /* davep 18-Sep-2007 ; we can receive an ABORT here if we get a paper
     * jam on eject 
     */
    if( rcvpkt.msg == SCAN_MSG_ABORT_SCAN_JOB ) {
        errorlog( "received ABORT during unlock\n" );
        /* I reuse retcode whenever I read from usb so have to have a second
         * variable so we can return the fact that we got an ABORT
         */
        function_retcode = ERR_DEV_FAILED;

        /* read again so we can try to get the response to the unlock */
        retcode = aspusb_get_header( udev, &rcvpkt );
        if( retcode < 0 ) {
            /* aspusb_get_header() logs error */
            return retcode;
        }
    }

    /* make sure we were able to unlock it */
    if( rcvpkt.msg != SCAN_MSG_RELEASE_SCAN_RESOURCE ) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_RELEASE_SCAN_RESOURCE, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    /* anything else go wrong? */
    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        errorlog( "scanner failed to unlock\n" );
        return ERR_DEV_BUSY;
    }

    return function_retcode;
}

static void 
aspscan_cancel( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;

    dbg2( "%s\n", __FUNCTION__ );

    dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
    dbg1( "!!!!!!    USER CANCEL!     !!!!!!\n" );
    dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );

    dbg1( "sending cancel message to device\n" );
    /* send a cancel message */
    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_CANCEL_SCAN_JOB; 

    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */

        /* ignore error? */
    }
}

static void write_stats( struct page_info *page )
{
    struct cmdline_option *op;
    char msg[FILENAME_MAX+1];
    int fd;
    int dpi;
    int data_type;
    int retcode;

    dbg2( "%s\n", __FUNCTION__ );

    op = lookup_option( &g_option_list, OPT_RESOLUTION );
    if( op==NULL ) {
        /* shouldn't happen! */
        XASSERT( 0, OPT_RESOLUTION );
        dpi = 0;
    }
    else {
        dpi = op->num32;
    }

    /* make sure we have a filename */
    ASSERT( page->filename_base[0] != 0 );

    data_type = page->data_type;

    /* davep 16-Apr-2013 ; adding page_side */
    if( page->page_number >= 0 ) {
        retcode = snprintf( msg, FILENAME_MAX, 
                    "dpi=%d bpp=%d rows=%d ppr=%d pprp=%d file=\"%s\" datatype=%d page_number=%d page_side=%d\n", 
                    dpi, page->bits_per_pixel, page->total_rows, page->pixels_per_row,
                    page->pixels_per_row_padded, page->filename_base,
                    data_type, page->page_number, page->page_side );
    }
    else {
        /* we don't have page numbers */
        retcode = snprintf( msg, FILENAME_MAX, 
                    "dpi=%d bpp=%d rows=%d ppr=%d pprp=%d file=\"%s\" datatype=%d page_side=%d\n", 
                    dpi, page->bits_per_pixel, page->total_rows, page->pixels_per_row,
                    page->pixels_per_row_padded, page->filename_base,
                    data_type, page->page_side );
    }
    XASSERT( retcode < FILENAME_MAX, retcode );

    /* davep 26-aug-2009 ; write to debug output, too */
    dbg1( "%s %s", __FUNCTION__, msg );

    /* davep 15-dec-05 ; check if user wants me to write final image info to a
     * file handle; if so, do so
     */
    op = lookup_option( &g_option_list, OPT_STAT_HANDLE );
    if( op != NULL ) {
        fd = (int)op->num32;

        dbg2( "%s to handle %d\n", __FUNCTION__, fd );

        write( fd, msg, strnlen(msg,FILENAME_MAX) );
    }

    /* davep 17-Jun-2011 ; write a PPM header */
    write_pnm_header( page );
}

/**
 * \brief  Send packet telling firmware to reset its timeout.
 *
 * The timeout reset acts a bit like a keepalive. If the firmware doesn't hear
 * from the host, eventually the FW will thing the host has Gone Away.
 *
 * \author David Poole
 * \date 25-Jun-2013
 */

static int reset_timeout( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;

    dbg1( "%s\n", __FUNCTION__ );

    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_RESET_TIMEOUT; 
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }
    return 0;
}

int 
aspscan_scan( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    struct scan_job job_settings;
    char msg[MAX_MSG];
    bool end_job_flag;
    int page_number;
    int cancel_sent;
    int scan_retcode;
    struct page_info page_list[MAX_PAGE_SIDES];
    uint8_t page_side;

    dbg2( "%s\n", __FUNCTION__ );

    // first initialize page_list with an obvious pattern to catch uninitalized problems
    memset( page_list, 0xaa, sizeof(page_list));
    
    STATE_TRANSITION( SCAN_STATE_STARTING );

    /* check for user cancel (often) */
    if( aspusb_is_cancelled() ) { 
        aspscan_cancel( udev );
        return ERR_USER_CANCEL;
    }

    /* send the start job message to kick off capturing data */
    dbg1( "starting scan...\n" );
    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_START_SCAN_JOB; 
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    /* read the response */
    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        /* aspusb_get_header() logs error */
        return retcode;
    }

    if( rcvpkt.msg != SCAN_MSG_START_SCAN_JOB ) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_START_SCAN_JOB, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        errorlog( "!!!   SCANNER FAILED TO START!   !!!\n" );
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        errorlog( "scanner reports status=%d (\"%s\")\n", 
                rcvpkt.status, asp_header_status_to_string(rcvpkt.status) );
        return ERR_DEV_FAILED;
    }

    /* stay in the loop below collecting messages and data until the scanner
     * sends us an "end job" telling it's done
     */
    end_job_flag = false;

    /* start numbering pages here; check a command line option to leave off
     * page numbers
     */
    if( g_add_page_numbers ){
        page_number = 0;
    }
    else {
        /* -1 means no page numbers */
        page_number = -1;
    }

    cancel_sent = 0;

    /* at this point, when we return from this function, we'll return this
     * value
     */
    scan_retcode = ERR_NONE;

    /* start capturing data */
    while( !end_job_flag ) {

        /* check for user cancel (often); only send the cancel once */
        if( aspusb_is_cancelled() && ! cancel_sent ) { 
            aspscan_cancel( udev );
            cancel_sent = 1;
//            return ERR_USER_CANCEL;
            /* we continue to take data from the host until we see the
             * SCAN_MSG_ABORT_SCAN_JOB come back
             */
        }

        retcode = aspusb_get_header( udev, &rcvpkt );
        if( retcode < 0 ) {
            if( retcode==ERR_USB_TIMEOUT ) {
                /* wait for things that could take a while to come back, e.g.,
                 * while we wait for calibration
                 */
                dbg1( "read timeout; retrying...\n" );
                continue;
            }

            errorlog( "error trying to read; halting scan\n" );

            /* leave the !end_job_flag loop */
            scan_retcode = retcode;
            goto fail;
        }

        asp_scan_header_to_str( msg, MAX_MSG, &rcvpkt );
        dbg2( "%s\n", msg );

        retcode = 0;
        switch( rcvpkt.msg ) {
            case SCAN_MSG_START_JOB :
                dbg1( "received start job message\n" );
                STATE_TRANSITION( SCAN_STATE_START_JOB );

                /* If we're doing a cal-to-host, start_job() will open the
                 * set up the struct page_info and the output file. So pass in
                 * the [0]th element of the page_list.
                 * If we're not doing a cal-to-host, start_job() doesn't use
                 * the page_info parameter and we'll open files in the
                 * start_page handler below.
                 */
                retcode = start_job(udev, &rcvpkt, &job_settings, &page_list[0]);
                break;

            case SCAN_MSG_START_SHEET :
                dbg1( "received start sheet message\n" );
                break;

            case SCAN_MSG_START_PAGE :
                scan_msg_page_start_decode( &rcvpkt, &page_side );
                dbg1( "received start page message page_side=%d\n", page_side );
                XASSERT( page_side<MAX_PAGE_SIDES, page_side );

                STATE_TRANSITION( SCAN_STATE_START_PAGE );

                g_icelite_rowcolor_idx = 0;

                retcode = reset_timeout( udev );

                retcode = start_page( job_settings.data_type, page_number, page_side,
                                        &page_list[page_side] );
                break;

            case SCAN_MSG_SCAN_IMAGE_DATA :
                scan_msg_image_data_decode( &rcvpkt, &page_side );
                dbg1( "received image data message page_side=%d\n", page_side );
                STATE_TRANSITION( SCAN_STATE_SCANNING );
                retcode = get_scan_data( udev, &rcvpkt, page_list );
                break;

            case SCAN_MSG_END_PAGE :
                scan_msg_page_start_decode( &rcvpkt, &page_side );
                dbg1( "received end page message page_side=%d\n", page_side );
                XASSERT( page_side<MAX_PAGE_SIDES, page_side );

                STATE_TRANSITION( SCAN_STATE_END_PAGE );

                retcode = end_page(&page_list[page_side]);

                /* write all the vitals of the page we just scanned */
                write_stats( &page_list[page_side] );

                /* page_number < 0 means we don't want to increment the page
                 * number no matter how many pages we receive (we write to the
                 * same file)
                 */
                /* davep 18-Apr-2013 ; only update the page_number once, for a
                 * single side of the page
                 */
                if( page_number >= 0 && page_side==0 ) {
                   page_number++;
                }
                
                g_total_pages++;
                break;

            case SCAN_MSG_END_SHEET :
                dbg1( "received end sheet message\n" );
                break;

            case SCAN_MSG_END_JOB :
                dbg1( "received end job message\n" );
                STATE_TRANSITION( SCAN_STATE_END_JOB );
                /* successful finish */
                end_job_flag = true;
                end_job(&page_list[0]);
                break;
                
            case SCAN_MSG_ABORT_SCAN_JOB :
                dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
                dbg1( "!!!    ABORT JOB RECEIVED!    !!!\n" );
                dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
                dbg1( "prematurely ending the scan\n" );
                scan_retcode = ERR_ABORT_RECEIVED;
                end_job_flag = true;
                break;

            case SCAN_MSG_NEW_PAGE :
                /* ADF tells us there's another page ; send the message back to
                 * tell the scanner to start the next page
                 */
                dbg1( "received new page message\n" );
                dbg1( "sending new page message to start next page\n" );

                /* XXX for testing ; delay newpage for a bit */
//                sleep( 5 );

                memset( &pkt, 0, sizeof(struct scan_header) );
                pkt.msg = SCAN_MSG_NEW_PAGE; 
                retcode = aspusb_send_header( udev, &pkt );
                break;

            case SCAN_MSG_DATA_BLOB :
                dbg1("received data blob message len=%d\n", rcvpkt.datalen );
                retcode = get_data_blob( udev, &rcvpkt, &page_list[0] );
                break;
            
            default :
                /* ignore but complain loudly */
                dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
                dbg1( "!!! received unknown scan_header.msg=%d !!!\n", rcvpkt.msg );
                dbg1( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
                break;
        }

        /* something went wrong ; bail out now and let the  */
        if( retcode < 0 ) {
            errorlog( "error during scan. giving up.\n" );
            scan_retcode = retcode;
            break;
        }
    }

    STATE_TRANSITION( SCAN_STATE_CLOSING );

    dbg2( "%s returning %d\n", __FUNCTION__, scan_retcode );
    return scan_retcode;
fail:
    dbg2( "%s returning %d\n", __FUNCTION__, scan_retcode );
    return scan_retcode;
}

int 
aspscan_get_settings( libusb_device_handle *udev, struct scan_job *settings )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    char msg[MAX_MSG];

    dbg2( "%s\n", __FUNCTION__ );

    /* send down the get scanjob settings request */
    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_GET_SCAN_JOB_SETTINGS; 
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        return retcode;
    }

    dbg2( "rcvpkt.datalen=%d\n", rcvpkt.datalen );

    /* now get the settings packet */
    retcode = aspusb_read( udev, aspusb_get_scan_in_endpoint(),
            (unsigned char *)settings, 
                    sizeof(struct scan_job), 5*USB_SECONDS );
    dbg2( "aspusb_read returned %d\n", retcode );

    if( retcode < 0 ) {
        errorlog( "aspusb_read failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    hex_dump( (unsigned char *)settings, sizeof(struct scan_job) );

    asp_swap_job_fields( settings );

    asp_scan_job_to_str( msg, MAX_MSG, settings); 
    dbg1( "%s\n", msg );

    return 0;
}

int
aspscan_set_default_settings( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;

    dbg2( "%s\n", __FUNCTION__ );

    /* tell scanner to reset all scan settings to defaults so we will have a
     * clean set between scans
     */
    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_SET_DEFAULT_SCAN_JOB_SETTINGS; 
    pkt.datalen = sizeof(struct scan_job);
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    /* read back the "ack" */
    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        return retcode;
    }

    /* not sure what could cause a failure but check for it anyway */
    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        errorlog( "Set default scan settings failed, error status=%d\n",
                rcvpkt.status );
        return ERR_FAIL;
    }

    return 0;
}

int 
aspscan_set_settings( libusb_device_handle *udev, struct scan_job *settings )
{
    int retcode, transferred;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    char msg[MAX_MSG];

    dbg2( "%s\n", __FUNCTION__ );

    asp_scan_job_to_str( msg, MAX_MSG, settings ); 
    dbg1( "%s\n", msg );

    asp_swap_job_fields( settings );

    /* send down the set scanjob settings request */
    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_SET_SCAN_JOB_SETTINGS; 
    pkt.datalen = sizeof(struct scan_job);
    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    /* now send the settings */
    retcode = libusb_bulk_transfer( udev, aspusb_get_scan_out_endpoint(),
            (unsigned char *)settings, 
                    sizeof(struct scan_job), &transferred, 5*USB_SECONDS );
    dbg2( "libusb_bulk_transfer returned %d\n", retcode );

    if( retcode < 0 ) {
        errorlog( "libusb_bulk_transfer failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    /* get the response */
    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        return retcode;
    }

    /* check it was ack'd ok */
    if( rcvpkt.msg != SCAN_MSG_SET_SCAN_JOB_SETTINGS) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_SET_SCAN_JOB_SETTINGS, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    if( rcvpkt.status != SCAN_STAT_SUCCESS ) {
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        errorlog( "!!!  SCANNER REJECTED REQUESTED SETTINGS!  !!!\n" );
        errorlog( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
        return ERR_SETTINGS_FAIL;
    }

    return 0;
}

#if 0
int 
open_scanner( struct usb_device *dev, libusb_device_handle **scanner_dev_handle )
{
    libusb_device_handle *udev;
    int retcode;

    dbg1( "%s %p %s/%s\n", __FUNCTION__, dev, dev->bus->dirname, dev->filename );

    *scanner_dev_handle = NULL;

    udev = usb_open(dev);
    if( !udev ) {
        errorlog( "usb_open failed : %s\n", strerror(errno) );
        return ERR_USB_FAIL;
    }

    dbg2( "device opened\n" );

    /* claim the scanner interface so we can talk to it; libusb requires this
     * before we can send any bulk in/out  
     */
    retcode = usb_claim_interface( udev, SCAN_INTERFACE );
    if( retcode < 0 ) {
        errorlog( "usb_claim_interface failed : %s\n", strerror(errno) );
        goto usb_fail;
    }
    
    /* I don't think we need to call usb_set_configuration() because we only
     * have one config.
     */

    *scanner_dev_handle = udev;

    return 0;

usb_fail:
    retcode = usb_close( udev );
    /* ignore error */
    return ERR_USB_FAIL;
}
#endif

static void 
reconcile_settings( struct scan_job *curr_settings )
{
    int i;
    struct scan_dimensions *area;
    struct cmdline_option *opt;
    struct cmdline_option *scan_type_opt;

    for( i=0 ; i<g_option_list.num_options ; i++ ) {
        opt = &g_option_list.options[i];

        switch( opt->key ) {
            case OPT_RESOLUTION :
                curr_settings->resolution = opt->num32;
                break;

            case OPT_DATA_TYPE :
                curr_settings->data_type = opt->num32;
                break;

            case OPT_SCAN_TYPE :
                curr_settings->scan_type = opt->num32;
                break;

            case OPT_X_SCALE :
                curr_settings->xyscale.x_numerator = opt->numerator;
                curr_settings->xyscale.x_denominator = opt->denominator;
                break;

            case OPT_Y_SCALE :
                curr_settings->xyscale.y_numerator = opt->numerator;
                curr_settings->xyscale.y_denominator = opt->denominator;
                break;

            case OPT_AREA :
                area = (struct scan_dimensions *)opt->buf;
                if( !area || 
                        (opt->buflen != sizeof(struct scan_dimensions)) ) {
                    dbg1( "ignored bad scan dimensions structure\n" );
                }
                else {
                    curr_settings->window.top = area->top;
                    curr_settings->window.left = area->left;
                    curr_settings->window.bottom = area->bottom;
                    curr_settings->window.right = area->right;
                }
                break;

            case OPT_CONTRAST :
                curr_settings->contrast = opt->num32;
                break;

            case OPT_BRIGHTNESS :
                curr_settings->brightness = opt->num32;
                break;

            case OPT_GAMMA :
                curr_settings->gamma = opt->num32;
                break;

            case OPT_SHARPNESS :
                curr_settings->sharp = opt->num32;
                break;

            case OPT_COPY_QUALITY :
                /* make sure we're doing a copy-to-host first; if so, we'll
                 * set the copy quality using the reserved1 field
                 */
                scan_type_opt = lookup_option( &g_option_list, OPT_SCAN_TYPE);
                if( scan_type_opt && scan_type_opt->num32==SCAN_TYPE_SCAN_THRU_COPY ) {
                    curr_settings->reserved1 = opt->num32;
                }
                break;

            case OPT_BPP :
                curr_settings->bpp = opt->num32;
                break;

            default :
                /* silently ignore any options we don't care about */
                break;
        }
    }
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 07-Sep-2010
 *
 */

static int get_platform_string( libusb_device_handle *udev )
{
    int retcode;
    struct scan_header pkt;
    struct scan_header rcvpkt;
    char msg[MAX_MSG];
    unsigned char *buf;

    memset( &pkt, 0, sizeof(struct scan_header) );
    pkt.msg = SCAN_MSG_GET_PLATFORM_STRING; 

    retcode = aspusb_send_header( udev, &pkt );
    if( retcode < 0 ) {
        /* aspusb_send_header() logs error */
        return retcode;
    }

    retcode = aspusb_get_header( udev, &rcvpkt );
    if( retcode < 0 ) {
        /* aspusb_get_header() logs error */
        return retcode;
    }

    asp_scan_header_to_str( msg, MAX_MSG, &rcvpkt ); 
    dbg2( "%s\n", msg );

    /* make sure we got back at least what we were expecting */
    if( rcvpkt.msg != SCAN_MSG_GET_PLATFORM_STRING ) {
        errorlog( "protocol error: expected msg %d got %d\n",
                SCAN_MSG_GET_PLATFORM_STRING, rcvpkt.msg );
        return ERR_BAD_PROTOCOL;
    }

    /* davep 9-dec-2006 ; sanity check against a really broken protocol (8M
     * max which should handle most stuff ) 
     */
    if( rcvpkt.datalen > 0x800000 ) {
        errorlog( "header datalen %d too large! (max expected=%d)\n",
                rcvpkt.datalen, 0x800000 );
        return ERR_BAD_PROTOCOL;
    }

    buf = malloc( rcvpkt.datalen );
    if( buf==NULL ) {
        errorlog( "out of memory\n" );
        return ERR_OUT_OF_MEMORY;
    }
    memset( buf, 0, rcvpkt.datalen );

    retcode = read_data( udev, buf, rcvpkt.datalen );
    if( retcode < 0 ) {
        PTR_FREE( buf );
        errorlog( "read_data() failed : retcode=%d\n", retcode );
        return retcode;
    }

    /* davep 08-Sep-2010 ; XXX temp ; just dump the data; eventually need to
     * parse and do something with this 
     */
    hex_dump( (unsigned char *)buf, rcvpkt.datalen );

    PTR_FREE( buf );

    return 0;
}


static void
stop_scan_on_signal( int signum )
{
    /* davep 18-Sep-2007 ; XXX It seems UNIX signals don't break MacOSX out of
     * its USB read. This code doesn't work on my Mac.
     */

    /* quiet the warning */
    signum = signum;

    /* set a global flag and bail */
    aspusb_set_cancel();

    /* write() is listed as signal handler callable safe */
#if LOGLEVEL >=1 
    const char *msg;

    msg = "signal received; starting scan cancel\n";
    write( fileno(stderr), msg, strlen(msg) );
#endif
}

int scan( libusb_device_handle *udev )
{
    int retcode;
    struct scan_job settings;
    struct cmdline_option *opt;
    uint32_t scan_timeout;
    struct sigaction sa;
    int scan_retcode;
    int unlock_retcode;

    STATE_TRANSITION( SCAN_STATE_IDLE );

    scan_timeout = 60; /* seconds */
    opt = lookup_option( &g_option_list, OPT_SCAN_TIMEOUT );
    if( opt ) {
        scan_timeout = opt->num32;
    }

    /* lock the scanner so we have exclusive access to it (e.g., prevent a copy
     * job from trying to access the scanner while we're scanning)
     */
    retcode = aspscan_lock( udev, scan_timeout );
    if( retcode < 0 ) {
        /* aspscan_lock() logs error */
        /* save the failure so we can return it */
        scan_retcode = retcode;
        return retcode;
    }

    /* davep 07-Sep-2010 ; get the platform capabilities (new firmware feature
     * as of this date)
     */
    retcode = get_platform_string( udev );
    if( retcode < 0 ) {
        /* save the failure so we can return it */
        scan_retcode = retcode;
        goto leave;
    }

    /* set the remote scan settings to defaults first so we don't have any
     * leftovers from any previous scan
     */
    retcode = aspscan_set_default_settings( udev );
    if( retcode < 0 ) {
        /* aspscan_get_settings() logs error */
        /* save the failure so we can return it */
        scan_retcode = retcode;
        goto leave;
    }

    /* modify the settings to the way we want based on the current settings */
    retcode = aspscan_get_settings( udev, &settings );
    if( retcode < 0 ) {
        /* aspscan_get_settings() logs error */
        /* save the failure so we can return it */
        scan_retcode = retcode;
        goto leave;
    }

    opt = lookup_option( &g_option_list, OPT_CAL_TO_HOST );
    if( opt && opt->num32 ) {
        dbg2( "setting to do cal-to-host\n" );
        settings.flags |= SCAN_JOB_FLAGS_CAL_TO_HOST;
    }

    /* davep 26-Jul-2011 ; adding nocal and nopie */
    opt = lookup_option( &g_option_list, OPT_NO_CAL );
    if( opt && opt->num32 ) {
        dbg2( "setting nocal\n" );
        settings.flags |= SCAN_JOB_FLAGS_NO_CAL;
    }
    opt = lookup_option( &g_option_list, OPT_NO_PIE );
    if( opt && opt->num32 ) {
        dbg2( "setting nopie\n" );
        settings.flags |= SCAN_JOB_FLAGS_NO_PIE;
    }
    
    /* copy settings from user into the default settings we get from the
     * scanner
     */
    reconcile_settings( &settings );

    /* davep 15-Jul-2009 ; force document source if requested (otherwise,
     * firmware will choose for us)
     */
    opt = lookup_option( &g_option_list, OPT_DOCUMENT_SOURCE );
    if( opt ) {
        settings.flags |= SCAN_JOB_FLAGS_SET_DOCUMENT_SOURCE(opt->num32);
    }

    /* davep 19-Dec-2011 ; adding original content */
    opt = lookup_option( &g_option_list, OPT_ORIGINAL_CONTENT );
    if( opt ) {
        settings.flags |= SCAN_JOB_FLAGS_SET_ORIGINAL_CONTENT(opt->num32);
    }

    /* now push the new settings back onto the device */
    retcode = aspscan_set_settings( udev, &settings );
    if( retcode < 0 ) {
        /* aspscan_set_settings() logs error */

        /* save the failure so we can return it */
        scan_retcode = retcode;
        goto leave;
    }

    /* set up ctrl-c, etc, to stop the scan */
    sa.sa_handler = stop_scan_on_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESETHAND;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        errorlog( "unable to install SIGINT handler\n" );
        goto leave;
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        errorlog( "unable to install SIGTERM handler\n" );
        goto leave;
    }

    scan_retcode = aspscan_scan( udev );

    /* REVISIT: does sigaction require us to replace the old handlers?
     *   Seems like it doesn't ...
     */

#if 0
    close_output_files( htonl( settings.data_type ) );
#endif

leave:
    /* we're done scanning so unlock it */
    unlock_retcode = aspscan_unlock( udev );

    STATE_TRANSITION( SCAN_STATE_IDLE );

    if( unlock_retcode < 0 ) {
        /* aspscan_unlock() logs error */
        return unlock_retcode;
    }

    return scan_retcode;
}

static void get_usb_cmdline_options( int *vid, int *pid, unsigned char **usb_serial_number, 
                         int *interface, int *out_endpoint, int *in_endpoint )
{
    struct cmdline_option *opt;

    /* start with the compiled defaults */
    *vid = SCANNER_VENDOR_ID;
    *pid = SCANNER_PRODUCT_ID;
    *usb_serial_number = NULL;
    *interface    = SCAN_INTERFACE;
    *out_endpoint = SCAN_OUT_ENDPOINT;
    *in_endpoint  = SCAN_IN_ENDPOINT;

    /* see if user specified on command line */
    opt = lookup_option( &g_option_list, OPT_USB_VID );
    if( opt!=NULL ) {
        *vid = opt->num32;
    }
    opt = lookup_option( &g_option_list, OPT_USB_PID );
    if( opt!=NULL ) {
        *pid = opt->num32;
    }
    opt = lookup_option( &g_option_list, OPT_SERIAL_NUM );
    if( opt!= NULL ) {
        *usb_serial_number = opt->buf;
    }
    opt = lookup_option( &g_option_list, OPT_USB_INTERFACE );
    if( opt!=NULL ) {
        *interface = opt->num32;
    }
    opt = lookup_option( &g_option_list, OPT_USB_OUT_ENDPOINT );
    if( opt!=NULL ) {
        *out_endpoint = opt->num32;

        /* Setting an out endpoint should cause the in endpoint to follow the out
         * address with the direction bit set.  If the user supplied a specific
         * in endpoint it will be set below.
         */
        *in_endpoint = (0x80 | opt->num32);
    }
    opt = lookup_option( &g_option_list, OPT_USB_IN_ENDPOINT );
    if( opt!=NULL ) {
        *in_endpoint = opt->num32;
    }
}

/**
 * \brief  Tweak file naming options depending on cmdline options
 *
 * If we're scanning from ADF, want page numbers on output file names. If we're
 * scanning from ADF Duplex, want page side on the output file names.
 *
 * \author David Poole
 * \date 19-Jul-2013
 */

static void check_file_numbering_options( void )
{
    struct cmdline_option *opt;

    g_add_page_side = false;
    g_add_page_numbers = false;

    if( lookup_option( &g_option_list, OPT_PAGE_NUMBERS ) ){
        g_add_page_numbers = true;
    }

    opt = lookup_option( &g_option_list, OPT_DOCUMENT_SOURCE );
    if( opt ) {
        /* If we're explicitly scanning from adf in some way, automatically turn on
         * page numbers so files are written to their own file. Note if we're
         * scanning from ADF based on firmware decision (paper present in ADF means
         * we usually scan from ADF), the -n option will still be necessary. This
         * code isn't smart enough (yet?) to turn on page numbers in filenames
         * after the scan has already started.
         */
        if( opt->num32 == SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX ||
            opt->num32 == SCAN_JOB_SETTINGS_DOC_SRC_ADF ) {
            g_add_page_numbers = true;
        }

        /* If we're scanning ADF Duplex, turn on the page side (p0/p1) in output 
         * filenames. I'd like to someday update aspscan to be smart enough to
         * automatically add the page side / page numbers depending on what the
         * firmware sends. 
         */
        if( opt->num32 == SCAN_JOB_SETTINGS_DOC_SRC_ADF_DUPLEX ) {
            g_add_page_side = true;
        }
    }
}

void
print_descriptor( struct libusb_device_descriptor *dev_des )
{
    dbg2( "bLength=%d\n", dev_des->bLength );
    dbg2( "bDescriptorType=%d\n", dev_des->bDescriptorType );
    dbg2( "bcdUSB=%d\n", dev_des->bcdUSB );
    dbg2( "bDeviceClass=%d\n", dev_des->bDeviceClass );
    dbg2( "bDeviceSubClass=%d\n", dev_des->bDeviceSubClass );
    dbg2( "bDeviceProtocol=%d\n", dev_des->bDeviceProtocol );
    dbg2( "bMaxPacketSize0=%d\n", dev_des->bMaxPacketSize0 );
    dbg2( "idVendor=%#x\n", dev_des->idVendor );
    dbg2( "idProduct=%#x\n", dev_des->idProduct );
    dbg2( "bcdDevice=%d\n", dev_des->bcdDevice );
    dbg2( "iManufacturer=%d\n", dev_des->iManufacturer );
    dbg2( "iProduct=%d\n", dev_des->iProduct );
    dbg2( "iSerialNumber=%d\n", dev_des->iSerialNumber );
    dbg2( "bNumConfigurations=%d\n", dev_des->bNumConfigurations );
}

int dump_descriptors( libusb_device *dev )
{
    int retcode;
    struct libusb_device_descriptor dev_des;
#define BUF_SIZE 1024
    unsigned char buf[BUF_SIZE];

//    memset( &dev_des, 0, sizeof(dev_des) );
    retcode = libusb_get_device_descriptor( dev, &dev_des );
    if( retcode < 0 ) {
        errorlog( "failed to get usb device descriptor\n" );
        return -1;
    }

    print_descriptor( &dev_des );

    memset( buf, 0, sizeof(buf));

#if 0
    /* get a list of supported languages */
    retcode = usb_get_string( dev, 0, 0, buf, BUF_SIZE );
    if( retcode < 0 ) {
        errorlog( "failed to get usb LANGID list\n" );
        return -1;
    }
    hex_dump( (unsigned char *)buf, MIN(retcode,BUF_SIZE) );
#endif

    /* davep 28-Jun-2009 ; TODO */
#if 0
    memset( buf, 0, sizeof(buf));
    retcode = usb_get_string_simple( udev, dev_des.iManufacturer, buf, BUF_SIZE );
    if( retcode < 0 ) {
        errorlog( "failed to get usb manufacturer string\n" );
        return -1;
    }
    dbg1( "manufacturer=\"%s\"\n", buf);

    memset( buf, 0, sizeof(buf));
    retcode = usb_get_string_simple( udev, dev_des.iSerialNumber, buf, BUF_SIZE );
    if( retcode < 0 ) {
        errorlog( "failed to get usb serial number string\n" );
        return -1;
    }
    dbg1( "serial number=\"%s\"\n", buf);
#endif

    return 0;    
#undef BUF_SIZE
}

static int query_adf( libusb_device_handle *udev )
{
    int retcode;
    int adf_status;

    retcode = aspscan_query_adf( udev, &adf_status );
    if( retcode < 0 ) {
        /* aspscan_query_adf() logs error */
        return retcode;
    }

    switch( adf_status ) {
        case SCAN_ADF_STATUS_UNKNOWN          :
            /* shouldn't happen */
            dbg1( "Error! Couldn't get ADF status from scanner!\n" );
            break;

        case SCAN_ADF_STATUS_NOT_ADF          :
            dbg1( "Scanner claims to not have an ADF.\n" );
            break;

        case SCAN_ADF_STATUS_PAPER_PRESENT    :
            dbg1( "Scanner ADF claims paper present.\n" );
            break;

        case SCAN_ADF_STATUS_NO_PAPER_PRESENT :
            dbg1( "Scanner ADF claims no paper present.\n" );
            break;

        default :
            XASSERT(0,adf_status);
    }

    return ERR_NONE;
}

static int query( libusb_device_handle *udev, int query_msg )
{
    int query_retcode;

    query_retcode = ERR_NONE;

    /* davep 14-Jun-2011 ; don't lock the scanner; locking starts a job */

    switch( query_msg ) {
        case SCAN_MSG_ADF_IS_PAPER_PRESENT :
            query_retcode = query_adf( udev );
            break;

        case SCAN_MSG_GET_PLATFORM_STRING :
            query_retcode = get_platform_string( udev );
            break;

        default : 
            XASSERT( 0, query_msg );
            break;
    }
    
    /* If the query function failed, return that error  */
    if( query_retcode < 0 ) {
        return query_retcode;
    }

    return ERR_NONE;
}


int main( int argc, char *argv[] )
{
    int retcode;
    libusb_device *dev;
    libusb_device_handle *udev;
    int iface;
    int vid, pid;
    unsigned char *usb_serial_number;
    int scan_retcode;
    int scan_out_endpoint;
    int scan_in_endpoint;
//    int num_busses;
//    int num_devices;

    retcode = parse_args( argc, argv, &g_option_list );
    if( retcode < 0 ) {
        /* parse_args() will log error */
        exit(EXIT_FAILURE);
    }
    /* Make sure at least one setting was set on the command line; I hate
     * programs that run off and do something crazy without any real input.
     */
    if( g_option_list.num_options == 0 ) {
        usage( argv[0] );
        exit(EXIT_FAILURE);
    }

    /* davep 18-Jul-2013 ; tweak the filename creation rules (page numbers and
     * page sides) based on our command line arguments 
     */
    check_file_numbering_options();

    /* Initalize libusb */
    libusb_init(NULL);

//    libusb_set_debug( NULL, 3 );

    /* Find all available USB busses (sets libusb usb_busses global variable) */
//    num_busses = usb_find_busses();
//    dbg2( "Number of busses found/changed: %d\n", num_busses );

    /* Find all available USB devices (sets device entries in libusb usb_busses structure) */
//    num_devices = usb_find_devices();
//    dbg2( "Number of devices found/changed: %d\n", num_devices );

    get_usb_cmdline_options( &vid, &pid, 
                             &usb_serial_number,
                             &iface, &scan_out_endpoint, &scan_in_endpoint );

    /* See if we can find the requested vid/pid */
    retcode = find_device( &dev, vid, pid, usb_serial_number );
    if( retcode < 0 ) {
        errorlog( "unable to find scanner\n" );
        exit(EXIT_FAILURE);
    }

    /* Each libusb_bulk_transfer()/usb_bulk_write() call needs to know the endpoint
     * address.  Note that we should always take care to get the direction bit
     * set properly as some platforms will fix things up (Linux/Mac) ... others 
     * won't (Windows).  The user may select out/in endpoint addresses and 
     * interface using command line options or we may use default compile-time 
     * constants. This call is placed after the find_device call just in case 
     * set_scan_endpoints needs to traverse the device structure.
     */
    aspusb_set_scan_endpoints( dev, iface, scan_out_endpoint, scan_in_endpoint );

    /* Try and open the device ... */
    retcode = open_device( dev, &udev, iface );
    if( retcode != ERR_NONE ) {
        errorlog( "failed to connect to USB device\n" );
        exit(EXIT_FAILURE);
    }

    /* davep 4-Dec-2006 ; adding support for picking device by serial number */
//    dump_descriptors( udev );

    scan_retcode = ERR_NONE;

    /* davep 11-Dec-2007 ; test querying the ADF (XXX add support to command
     * line at some point)
     */
#if 1
    scan_retcode = query( udev, SCAN_MSG_ADF_IS_PAPER_PRESENT );
    if( scan_retcode != ERR_NONE ) {
        errorlog( "query() adf failed with retcode=%d\n", scan_retcode );
        goto leave;
    }

    scan_retcode = query( udev, SCAN_MSG_GET_PLATFORM_STRING );
    if( scan_retcode != ERR_NONE ) {
        errorlog( "query() platform string failed with retcode=%d\n", scan_retcode );
        goto leave;
    }
#endif

    scan_retcode = scan( udev );
    if( scan_retcode != ERR_NONE ) {
        goto leave;
    }

    dbg1( "received total rows=%d pages=%d\n", g_total_rows, g_total_pages );

leave:
    /* free our global buffer used for reading buffers from the data messages
     * (global so won't have to alloc/free it each time)
     */
    if( data_msg_buffer.buf ) {
        PTR_FREE(data_msg_buffer.buf);
    }

    if( scan_retcode != 0 ) {
        errorlog( "scanning failed with error=%d\n", scan_retcode );
    }

    /* free the memory from the options */
    free_options( &g_option_list );

    retcode = close_device( udev, iface );
    /* ignore return code */

    libusb_exit(NULL);

    if( scan_retcode == 0 ) {
        retcode = EXIT_SUCCESS;
    }
    else {
        retcode = EXIT_FAILURE;
    }
    dbg1( "aspscan (pid=%d) exiting with code=%d\n", getpid(), retcode );

    return retcode;
}

