/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/* 
 * Simple userspace scan. Talks to kscantask through /dev/scanman
 *
 * davep 19-Jun-2012 
 *
 * davep 18-Dec-2012 ; add cal-to-host
 */

#include <stdio.h>
#include <stdlib.h>
/* need strnlen() and current rev of compiler requires __USE_GNU */
#define __USE_GNU  
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "lassert.h"
#include "memAPI.h"
#include "utils.h"
#include "aspscan/include/cmdline.h"

/* need the siqfile_print_fn capabilities in scanvars.h */
#define USE_SIQFILE_PRINT_FN 1

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanmech.h"
#include "scanvars.h"
#include "scantask.h"
#include "scanmech_null.h"
#include "scanlog.h"
#include "mbuffer.h"
#include "siqdata.h"
#include "outfile.h"
#include "smsg_decode.h"
#include "scands.h"
#include "scanvarsiq.h"

#define MODULE_NAME "scanman" 
#define DEV_NAME "/dev/"MODULE_NAME

#define BUFSIZE 65536

/* because I'm appending things to the end of the filename, enforce a shorter
 * filename than the system max
 */
#define MAX_OUTPUT_FILENAME (FILENAME_MAX/2)
 
#define ERR_FAIL -1

/* Global pointer to hold malloc'd buffer to hold data read during
 * SMSG_PAGE_DATA. Allocated (and possibly reallocated) during incoming
 * buffers. Freed at end of program.
 */
static struct {
    uint8_t *buf;
    uint8_t bufsize_bytes;
} data_msg_buffer;

/* We need to preserve some data during the scan. We get the SMSG_SCAN_SIZE
 * message which contains the scan size (width, height, etc). The data messages
 * only contain the number of rows, not the buffer size. We have to infer the
 * buffer size based on the previous SMSG_SCAN_SIZE message.
 */

struct runscan_members {
    uint32_t pixels_per_row_padded;
    uint32_t pixels_per_row;
    uint32_t total_rows;
    uint32_t bits_per_pixel;

    /* calculated from the above fields */
    uint32_t bytes_per_row;

    scan_cmode_t cmode;
};

struct runscan_stats {
    /* count incoming messages */
    uint32_t sizes;
    uint32_t scan_starts;
    uint32_t scan_ends;
    uint32_t page_starts;
    uint32_t page_ends;
    uint32_t page_datas;
    uint32_t readies;

    /* total data rows received (across all channels) */
    uint32_t total_rows;
};

static struct runscan_members runscan_self;
static struct runscan_stats runscan_stats;

static struct mbuffer siqfile_buf;

/* default settings for our scan */
//static scan_cmode_t cmode = SCAN_CMODE_COLOR;
//static int dpi=600;

static uint32_t debug_level=1;

static char outfilename_base[MAX_OUTPUT_FILENAME+1] = "out";

/* add a page number to each file name (useful when scanning from ADF); set by
 * the -n cmdline option
 */
static bool add_page_number = false;

/* davep 04-Apr-2013 ; adding page sides for dual scan */
static bool add_page_side = false;

/* davep 01-Nov-2012 ; quit scanning when this is true (can be synchronously
 * set but eventually hook to a signal)
 */
static bool runscan_quit_flag=false;

/* davep 18-Dec-2012 ; adding cal-to-host */
//static bool cal_to_host_flag=false;

/* output file for cal-to-host requires special handling compared to regular
 * scan-to-host
 */
static FILE *cal_to_host_outfile;
struct output_file_list outfile_list;

/* options from the command line */
struct cmdline_option_list g_option_list;

/* The following functions are for compatibility with the core scan code.
 * Rather than having to build in a larger chunk of the core code than
 * necessary, some of the smaller functions are put here.
 */
pthread_t scantask_get_id( void )
{
    /* Sanity checks in scanvars and other locations make sure we're touching
     * data structures from only one thread.
     */
    return pthread_self();
}

void smech_null_init( t_ScanMech *scan_mech )
{
    /* need this function to link; don't call */
    ASSERT(0);
}

bool pie_csc_has_unified_rgb_luts( void )
{
    /* Only very, very early ASICs had combined LUTs. Add this function as a
     * stub so we don't have to pull in all of PIE. 
     */
    return false;
}

scan_err_t scansen_get_margins(uint32_t dpi, scan_sensor_margin_t *margins)
{
    /* Required by scanvars and implemented in scansen driver. Not sure how I'm
     * going to solve this problem yet. :-/
     */
    ASSERT(0);

    return SCANERR_NOT_IMPLEMENTED;
}

uint32_t scan_get_config( void )
{
    return 0;
}

/*
 *  End of core scan code stub functions.
 */

static bool cal_to_host( void )
{
    static bool is_cal_to_host = false;
    static bool once = false;
    struct cmdline_option *opt;

    if( once ) {
        return is_cal_to_host;
    }

    opt = lookup_option( &g_option_list, OPT_CAL_TO_HOST );
    if( opt && opt->num32 ) {
        is_cal_to_host = true;
    }
    return is_cal_to_host;
}


static void runscan_quit( void )
{
    runscan_quit_flag = true;
}

static void scanfw_siqfile_print( const char *fmt, va_list ap )
{
    scan_err_t scerr;
    int retcode;
    char *ptr;
    int avail;

    avail = siqfile_buf.max - siqfile_buf.len;
    ptr = siqfile_buf.buf + siqfile_buf.len;

    while( 1 )  {
//        dbg2( "%s buf=%p ptr=%p avail=%d\n", __FUNCTION__, siqfile_buf.buf, ptr, avail );

        retcode = vsnprintf( ptr, avail, (char *)fmt, ap );
        if( retcode<0 ) {
            /* error */
            dbg2( "%s error retcode=%d\n", __FUNCTION__, retcode );
            return;
        }

//        dbg2( "%s avail=%d retcode=%d\n", __FUNCTION__, avail, retcode );

        if( retcode < avail ) {
            /* success! */
            siqfile_buf.len += retcode;
            break;
        }

//        dbg1( "%s overflow; realloc and try again\n", __FUNCTION__ );

        scerr = mbuffer_realloc( &siqfile_buf );
        if( scerr != SCANERR_NONE ) {
            /* ignore this string */
            dbg2( "%s cannot print this string\n", __FUNCTION__ );
            return;
        }

        /* realloc() might have changed the buffer so reinitialize our local
         * ptr/len 
         */
        avail = siqfile_buf.max - siqfile_buf.len;
        ptr = siqfile_buf.buf + siqfile_buf.len;
    }
}

static scan_err_t buffer_read( FILE *f, uint8_t* buf, uint32_t buflen_bytes )
{
    int retcode;
    uint8_t *ptr;
    uint32_t bytes_remain;

    ASSERT( buflen_bytes );

    if( debug_level>2 ) {
        dbg2( "%s buf=%p buflen_bytes=%d\n", __FUNCTION__, buf, buflen_bytes );
    }

    ptr = buf;
    bytes_remain = buflen_bytes;

    while( bytes_remain > 0 ) {
        retcode = fread( ptr, 1, bytes_remain, f );
        if( retcode <= 0 ) {
            dbg1( "%s read failed errno=%d\n", __FUNCTION__, errno );
            return SCANERR_MSG_FAIL; 
        }
        if( debug_level > 2 ) {
            dbg2( "%s read bytes=%d returned bytes=%d\n", 
                        __FUNCTION__, bytes_remain, retcode );
        }

        ptr += retcode;
        bytes_remain -= retcode;
    }

    return SCANERR_NONE;
}

static scan_err_t buffer_write( FILE *f, uint8_t* buf, uint32_t buflen_bytes )
{
    int retcode;
    uint8_t *ptr;
    uint32_t bytes_remain;

    ASSERT( buflen_bytes );
    ASSERT(f);

    if( debug_level > 2 ) {
        dbg2( "%s buf=%p buflen_bytes=%d\n", __FUNCTION__, buf, buflen_bytes );
    }

    ptr = buf;
    bytes_remain = buflen_bytes;

    while( bytes_remain > 0 ) {
        retcode = fwrite( ptr, 1, bytes_remain, f );
        if( retcode <= 0 ) {
            dbg1( "%s write failed errno=%d\n", __FUNCTION__, errno );
            return SCANERR_MSG_FAIL; 
        }

        if( debug_level > 2 ) {
            dbg2( "%s write bytes=%d returned bytes=%d\n", 
                        __FUNCTION__, bytes_remain, retcode );
        }

        ptr += retcode;
        bytes_remain -= retcode;
    }

    return SCANERR_NONE;
}

static void htons_data( unsigned char *buf, uint32_t buf_num_bytes )
{
    uint16_t *ptr16, *endptr16;

    ptr16 = (uint16_t*)buf;
    endptr16 = (uint16_t *)(buf + buf_num_bytes);

    while( ptr16 < endptr16 ) {
        *ptr16 = htons(*ptr16);
        ptr16++;
    }
}

static void make_output_filename( char outfilename[], int page_number, scan_data_type dtype, int page_side  ) 
{
    int retcode;
    char *extension;

    if( dtype==SCAN_DATA_TYPE_RGB ) {
        extension = "ppm";
    }
    else {
        extension ="pgm";
    }

    if( add_page_number ) {
        if( add_page_side ) {
            retcode = snprintf( outfilename, FILENAME_MAX, "%s_side%d_%d_%s.%s", 
                            outfilename_base, page_side, page_number,
                            scanvar_get_scan_data_type_name(dtype), extension );
        }
        else {
            retcode = snprintf( outfilename, FILENAME_MAX, "%s_%d_%s.%s", 
                            outfilename_base, page_number, 
                            scanvar_get_scan_data_type_name(dtype),
                            extension );
        }
    }
    else {
        if( add_page_side ) {
            retcode = snprintf( outfilename, FILENAME_MAX, "%s_side%d_%s.%s", 
                            outfilename_base, page_side, 
                            scanvar_get_scan_data_type_name(dtype), 
                            extension );
        }
        else {
            retcode = snprintf( outfilename, FILENAME_MAX, "%s_%s.%s", 
                            outfilename_base, 
                            scanvar_get_scan_data_type_name(dtype), 
                            extension );
        }
    }

    XASSERT( retcode<FILENAME_MAX, retcode );
}

static scan_err_t handle_data_message( FILE *devfile, int page_number, scan_msg_t *msg)
{
    scan_err_t scerr;
    uint16_t num_rows;
    scan_data_type dtype;
    uint8_t *ptr;
    bool last_buffer;
    uint32_t num_bytes;
    uint8_t page_side;
    char outfilename[FILENAME_MAX+1];
    struct output_file *outfile;
    char pbm_header[64];
    struct output_file cal_stub;

    /* decode the message because we need to know the buffer size */
    scan_msg_page_data_decode( msg, &num_rows, &dtype, &ptr, &last_buffer);
    
    runscan_stats.total_rows += num_rows;

    /* davep 04-Apr-2013 ; adding page side support for dual scan */
    scan_msg_page_data_get_page_side( msg, &page_side );

    dbg2( "%s num_rows=%d total_rows=%d dtype=%d ptr=%p side=%d last=%d\n", __FUNCTION__, 
            num_rows, runscan_stats.total_rows, dtype, ptr, page_side, last_buffer );

    /* davep 05-Apr-2013 ; automatically turn on page side in file names if we
     * get a buffer from the back side (will also be enabled when
     * --docsrc=adfduplex is on command line)
     */
    if( page_side > 0 ) {
        add_page_side = true;
    }

    num_bytes = num_rows * runscan_self.bytes_per_row;

    /* if we have a previous buffer but that buffer is smaller than what we
     * want, resize it to fit what we need now
     */
    if( data_msg_buffer.buf && data_msg_buffer.bufsize_bytes < num_bytes ) {
        data_msg_buffer.buf = realloc( data_msg_buffer.buf, num_bytes );
        if( data_msg_buffer.buf != NULL ) {
            data_msg_buffer.bufsize_bytes = num_bytes;
        }
    }

    if( data_msg_buffer.buf==NULL ) {
        data_msg_buffer.bufsize_bytes = num_bytes;
        data_msg_buffer.buf = malloc( num_bytes );
        if( data_msg_buffer.buf==NULL ) {
            dbg1( "%s out of memory num_bytes=%d\n", __FUNCTION__, num_bytes );
            return SCANERR_OUT_OF_MEMORY;
        }
    }

    scerr = buffer_read( devfile, data_msg_buffer.buf, num_bytes );
    if( scerr != SCANERR_NONE ) {
        /* buffer_read() logs the error */
        return scerr;
    }

    /* davep 17-Jul-2013 ; if we're doing cal-to-host, we already have an open
     * file. Set to our cal output here and we'll fall through code later to
     * write to that open file.
     */
    if( cal_to_host() ) {
        memset( &cal_stub, 0, sizeof(struct output_file) );
        cal_stub.filep = cal_to_host_outfile;
        outfile = &cal_stub;
    }
    else {
        outfile = output_file_list_lookup( &outfile_list, dtype, page_side );
    }
    if( outfile==NULL ) {
        make_output_filename( outfilename, page_number, dtype, page_side );
        outfile = output_file_list_fopen( &outfile_list, outfilename, 
                                            dtype, page_side );
        if( !outfile ) {
            dbg2( "failed to open \"%s\" for scanned image!\n", outfilename );
        }
        else {
            dbg2( "open \"%s\" for scanned image\n", outfilename );
            
            /* write a PBM header */
            if( dtype==SCAN_DATA_TYPE_RGB ) {
                fwrite( "P6\n", 1, 3, outfile->filep );
            } 
            else {
                fwrite( "P5\n", 1, 3, outfile->filep );
            }
            memset( pbm_header, 0, sizeof(pbm_header) );
            sprintf( pbm_header, "%d %d\n", runscan_self.pixels_per_row_padded,
                                        runscan_self.total_rows );
            fwrite( pbm_header, 1, strlen(pbm_header), outfile->filep );
            if( runscan_self.bits_per_pixel==16 ) {
                fwrite( "65535\n", 1, 6, outfile->filep );
            }
            else {
                fwrite( "255\n", 1, 4, outfile->filep );
            }
        }
    }

    if( !outfile ) {
        dbg1( "%s output file not open; data dropped\n", __FUNCTION__ );
    }
    else {
        /* davep 18-Dec-2012 ; if we're doing cal-to-host, stamp the
         * Type+Length header into the file so we can parse the cal data
         * properly (see calparse.py)
         */
        if( cal_to_host() ) {
            uint32_t num32;

            num32 = htonl( dtype );
            fwrite( &num32, 1, sizeof(uint32_t), outfile->filep );

            /* length */
            num32 = htonl( num_bytes );
            fwrite( &num32, 1, sizeof(uint32_t), outfile->filep );
        }

        /* davep 25-Mar-2013 ; need to swap 16-bit pixels (firmware is
         * little-endian, Netpbm is big-endian) 
         */
        /* davep 11-Apr-2013 ; SOME Netpbm is big-endian. This is not
         * foolproof. 
         *
         * TODO stop using Netpbm and port to libtiff.
         */
        /* davep 18-Jul-2013 ; don't do this when doing cal-to-host
         * because this swap will confuse calparse.py
         */
        if( runscan_self.bits_per_pixel==16 && !cal_to_host() ) {
            htons_data( data_msg_buffer.buf, num_bytes );
        }

        scerr = buffer_write( outfile->filep, data_msg_buffer.buf, num_bytes );
        if( scerr != SCANERR_NONE ) {
            /* buffer_write() logs error */
            return scerr;
        }

        fflush( outfile->filep );
    }

    /* davep 17-Jul-2013 ; cal-to-host files closed later, after all data
     * received
     */
    if( last_buffer && !cal_to_host() ) {
        scerr = output_file_list_fclose( &outfile_list, dtype, page_side );
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s failed to close file for dtype=%d page_side=%d\n", 
                    __FUNCTION__, dtype, page_side );
        }
        XASSERT(scerr==SCANERR_NONE, scerr );
    }

    return SCANERR_NONE;
}

static scan_err_t handle_data_blob_message( FILE *devfile, scan_msg_t *msg)
{
    scan_err_t scerr;
    uint32_t num_bytes;
    uint8_t *ptr;
    uint8_t *buf;

    /* decode the message because we need to know the buffer size */
    scan_msg_data_blob_decode( msg, &ptr, &num_bytes );
    
    dbg2( "%s num_bytes=%d ptr=%p\n", __FUNCTION__, num_bytes, ptr );

    buf = malloc( num_bytes );
    if( buf==NULL ) {
        dbg1( "%s out of memory num_bytes=%d\n", __FUNCTION__, num_bytes );
        return SCANERR_OUT_OF_MEMORY;
    }

    scerr = buffer_read( devfile, buf, num_bytes );
    if( scerr != SCANERR_NONE ) {
        /* buffer_read() logs the error */
        PTR_FREE( buf );
        return scerr;
    }

    if( debug_level > 1 ) {
        scanlog_hex_dump( buf, 64 );
    }

    if( !cal_to_host_outfile ) {
        dbg1( "%s output file not open; data dropped\n", __FUNCTION__ );
    }
    else {
        scerr = buffer_write( cal_to_host_outfile, buf, num_bytes );
        if( scerr != SCANERR_NONE ) {
            /* buffer_write() logs error */
            PTR_FREE(buf);
            return scerr;
        }

        fflush( cal_to_host_outfile );
    }

    PTR_FREE(buf);

    return SCANERR_NONE;
}

static void handle_paper_event_msg( scan_adf_paper_event_t adfevt, uint32_t value  )
{
    dbg2( "scanman received adf paper event evt=%d value=%d\n", adfevt, value );
}

static scan_err_t runscan_dispatch_message( FILE *devfile, 
                                            const struct scanvars *sv, 
                                            scan_msg_t *msg )
{
    static int page_number=0;
    uint8_t page_side;

//    printf( "%s msgtype=%d p1=%d p2=%d p3=%p\n", __FUNCTION__, msg.msgType,
//            msg.param1, msg.param2, msg.param3 );

    switch( msg->msgType ) {
        case SMSG_SCAN_SIZE :
            dbg1( "runscan received scan_size\n");
            scan_msg_size_decode( msg, 
                        &runscan_self.pixels_per_row_padded, &runscan_self.pixels_per_row, 
                        &runscan_self.total_rows, &runscan_self.bits_per_pixel );
            dbg2( "%s ppr=%d pprp=%d tr=%d bpp=%d\n", __FUNCTION__,
                    runscan_self.pixels_per_row, 
                    runscan_self.pixels_per_row_padded,
                    runscan_self.total_rows, runscan_self.bits_per_pixel );

            /* pre-calculate the bytes_per_row (will be constant across this scan) */
            runscan_self.bytes_per_row = runscan_self.pixels_per_row_padded * 
                                         (runscan_self.bits_per_pixel/8);
            runscan_stats.sizes++;

            /* write the file size into the PBM header.
             *
             * THIS WILL ONLY WORK WITH FLATBED SCANS!
             *
             */
//            memset( pbm_header, 0, sizeof(pbm_header) );
//            sprintf( pbm_header, "%d %d\n", runscan_self.pixels_per_row_padded,
//                                        runscan_self.total_rows );
            break;

        case SMSG_PAGE_START :
            scan_msg_page_start_decode(msg,&page_side);
            dbg1( "runscan received page_start page_side=%d\n", page_side );
            runscan_stats.page_starts++;
            break;

        case SMSG_PAGE_DATA :
//            dbg1( "runscan received page_data\n" );
            runscan_stats.page_datas++;
            handle_data_message( devfile, page_number, msg );
            break;

        case SMSG_PAGE_END :
            scan_msg_page_end_decode(msg,&page_side);
            dbg1( "runscan received page_end page_side=%d\n", page_side );
            runscan_stats.page_ends++;

            /* only update the page_number for a single side */
            if( page_side==0 ) {
                page_number += 1;
            }
            break;

        case SMSG_SCAN_END :
            dbg1( "runscan received scan_end\n" );
            runscan_stats.scan_ends++;
            break;

        case SMSG_SCAN_READY :
            /* scan_ready is the "ack" from scantask telling us he's done */
            dbg1( "runscan received scan_ready\n");
            runscan_stats.readies++;

            /* davep 01-Nov-2012 ; quit when we get this message (as of this
             * writing, our application is a single shot scan)
             */
            runscan_quit();
            break;

        case SMSG_DATA_BLOB : 
            /* data blobs are usually from cal-to-host */
            dbg1( "runscan received data_blob\n" );
            handle_data_blob_message( devfile, msg);
            break;

        case SMSG_CAL_CALIBRATION_IN_PROGRESS :
        case SMSG_CAL_CALIBRATION_DONE :
            dbg2( "%s cal status message (ignored)\n", __FUNCTION__ );
            break;
    
        case SMSG_SCAN_ADF_PAPER_EVENT :
            handle_paper_event_msg( msg->param1, msg->param2 );
            break;

        case SMSG_SCAN_FAILED : 
            dbg1( "runscan received scanfailed\n", __FUNCTION__ );
            /* TODO decode the message, report failure reason */
            break;

        default :
            /* epic fail! */
            dbg1("%s unknown message 0x%x param1=0x%x param2=0x%x param3=%p\n",
                  __FUNCTION__, msg->msgType, msg->param1, msg->param2, msg->param3 );
            XASSERT( 0, msg->msgType );
            break;
    }

    return SCANERR_NONE;
}

static scan_err_t send_siqfile_to_kscanman( FILE *devfile, char *buf, int buflen )
{
    char *ptr;
    int bytes_written, bytes_remain;

    bytes_written = 0;

    ptr = buf;
    bytes_remain = buflen;

    while(bytes_remain) {
        bytes_written = fwrite( ptr, 1, bytes_remain, devfile );

        dbg2( "%s fwrite() siqfile bytes_written=%d\n", __FUNCTION__, bytes_written );
        if( bytes_written < 0 ) {
            perror("fwrite() to dev\n" );
            break;
        }

        bytes_remain -= bytes_written;
        ptr += bytes_written;
    }

    bytes_written = fwrite( "#END-OF-FILE-FOR-USB", 1, 20, devfile );

    fflush( devfile );

    return SCANERR_NONE;
}

static scan_err_t make_scanvar_from_cmdline( struct scanvars **p_sv ) 
{
    scan_err_t scerr, final_scerr;
    struct scanvars *sv;
    scan_cmode_t cmode;
    struct cmdline_option *opt;
    scan_document_source_t docsrc;
    int x_numer, x_denom, y_numer, y_denom;
    struct scan_area *area;

    *p_sv = NULL;
    final_scerr = SCANERR_GENERIC_FAIL;

    opt = lookup_option( &g_option_list, OPT_SCAN_TYPE );
    if( !opt ) {
        dbg1( "internal error; no cmode specified\n" );
        /* no scanvar to delete so just return here */
        return SCANERR_INVALID_PARAM;
    }
    cmode = (scan_cmode_t)opt->num32;

    if( cmode==SCAN_CMODE_COLOR ) {
        sv = scanplat_sv_color_scan_new();
    } 
    else {
        XASSERT( cmode==SCAN_CMODE_MONO, cmode );
        sv = scanplat_sv_mono_scan_new();
    }
    if( !sv ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    opt = lookup_option( &g_option_list, OPT_DATA_TYPE );
    if( opt ) {
        switch( opt->num32 ) {
            case SCAN_DATA_TYPE_MONO :
                if( sv->cmode!=SCAN_CMODE_MONO ) {
                    dbg1( "%s internal error; data type must be mono if scan type is mono\n", __FUNCTION__ );
                    final_scerr = SCANERR_INVALID_PARAM;
                    goto fail;
                }
                break;

            case SCAN_DATA_TYPE_PLANE : 
                if( sv->cmode!=SCAN_CMODE_COLOR ) {
                    dbg1( "%s internal error; data type must be color if scan type is planar\n", __FUNCTION__ );
                    final_scerr = SCANERR_INVALID_PARAM;
                    goto fail;
                }
                scanvar_set_pieout( sv, PIE_PIXEL_3_PLANES );
                break;

            case SCAN_DATA_TYPE_XRGB : 
                if( sv->cmode!=SCAN_CMODE_COLOR ) {
                    dbg1( "%s internal error; data type must be color if scan type is xrgb\n", __FUNCTION__ );
                    final_scerr = SCANERR_INVALID_PARAM;
                    goto fail;
                }
                scanvar_set_pieout( sv, PIE_PIXEL_XRGB );
                break;

            case SCAN_DATA_TYPE_RGBX : 
                if( sv->cmode!=SCAN_CMODE_COLOR ) {
                    dbg1( "%s internal error; data type must be color if scan type is rgbx\n", __FUNCTION__ );
                    final_scerr = SCANERR_INVALID_PARAM;
                    goto fail;
                }
                scanvar_set_pieout( sv, PIE_PIXEL_RGBX );
                break;

            case SCAN_DATA_TYPE_RGB : 
                if( sv->cmode!=SCAN_CMODE_COLOR ) {
                    dbg1( "%s internal error; data type must be color if scan type is rgb\n", __FUNCTION__ );
                    final_scerr = SCANERR_INVALID_PARAM;
                    goto fail;
                }
                scanvar_set_pieout( sv, PIE_PIXEL_RGB );
                break;

            default:
                XASSERT(0,opt->num32);
                break;
        }
    }

    opt = lookup_option( &g_option_list, OPT_RESOLUTION );
    if( opt ) {
        scerr = scanvar_set_dpi( sv, opt->num32 );
        if( scerr != SCANERR_NONE ) {
            dbg2( "%s invalid dpi=%d scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            scanvar_delete( &sv );
            return SCANERR_INVALID_PARAM;
        }
    }

    opt = lookup_option( &g_option_list, OPT_X_SCALE );
    if( opt ) {
        scanvar_get_xyscale( sv, &x_numer, &x_denom, &y_numer, &y_denom );
        scerr = scanvar_set_xyscale( sv, opt->numerator, opt->denominator,
                                     y_numer, y_denom );
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid x scale %d/%d; scerr=%d\n", __FUNCTION__,
                    opt->numerator, opt->denominator );
            return scerr;
        }
    }

    opt = lookup_option( &g_option_list, OPT_Y_SCALE );
    if( opt ) {
        scanvar_get_xyscale( sv, &x_numer, &x_denom, &y_numer, &y_denom );
        scerr = scanvar_set_xyscale( sv, x_numer, x_denom, opt->numerator, opt->denominator );
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid y scale %d/%d; scerr=%d\n", __FUNCTION__,
                    opt->numerator, opt->denominator );
            return scerr;
        }
    }

    opt = lookup_option( &g_option_list, OPT_AREA );
    if( opt ) {
        area = (struct scan_area *)opt->buf;
        ASSERT( area );
        XASSERT(opt->buflen==sizeof(struct scan_area), sizeof(struct scan_area));
        scerr = scanvar_set_area( sv, area->x, area->y, area->width, area->height );
        if( scerr != SCANERR_NONE ) { 
            dbg1( "%s invalid scan area %d,%d,%d,%d\n", __FUNCTION__,
                        area->x, area->y, area->width, area->height );
            return scerr;
        }
    }

    opt = lookup_option( &g_option_list, OPT_BPP );
    if( opt ) {
        scerr = scanvar_set_bpp( sv, opt->num32 );
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid bpp=%d scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            scanvar_delete( &sv );
            return SCANERR_INVALID_PARAM;
        }
    }

    opt = lookup_option( &g_option_list, OPT_DOCUMENT_SOURCE );
    if( opt ) { 
        docsrc = (scan_document_source_t)opt->num32;
        scerr = scanvar_set_document_source( sv, docsrc );
        XASSERT( scerr==SCANERR_NONE, scerr );

        /* davep 05-Apr-2013 ; if we're scanning from adfduplex, automatically
         * add page sides. This is not 100% portable (duplex normally means
         * mechanical duplex) but on this particular platform as of this
         * writing, this code is only running on a dual scan (2x sensors, one
         * each side of paper).
         */
        if( docsrc==SCAN_DOCUMENT_SOURCE_ADF_NSENSOR || docsrc==SCAN_DOCUMENT_SOURCE_ADF_DUPLEX ) {
            add_page_side = true;
        }
    }

    opt = lookup_option( &g_option_list, OPT_CONTRAST );
    if( opt ) {
        scerr = scanvar_set_contrast( sv, opt->num32 ); 
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid contrast \"%d\": scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            return scerr;
        }
    }
    opt = lookup_option( &g_option_list, OPT_BRIGHTNESS );
    if( opt ) {
        scerr = scanvar_set_brightness( sv, opt->num32 ); 
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid brightness\"%d\": scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            return scerr;
        }
    }
    opt = lookup_option( &g_option_list, OPT_GAMMA );
    if( opt ) {
        scerr = scanvar_set_gamma( sv, opt->num32 ); 
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid gamma \"%d\": scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            return scerr;
        }
    }
    opt = lookup_option( &g_option_list, OPT_SHARPNESS );
    if( opt ) {
        scerr = scanvar_set_sharpness( sv, opt->num32 ); 
        if( scerr != SCANERR_NONE ) {
            dbg1( "%s invalid sharpen/smooth \"%d\": scerr=%d\n", __FUNCTION__, opt->num32, scerr );
            return scerr;
        }
    }

    opt = lookup_option( &g_option_list, OPT_CAL_TO_HOST );
    if( opt && opt->num32 ) {
        sv->capture_cal = true;
    }

    /* make sure the "--nopie" check is last so will override any other
     * settings before this code
     */
    opt = lookup_option( &g_option_list, OPT_NO_PIE );
    if( opt && opt->num32 ) {
        scanvar_disable_image_enhancements( sv );
    }

    /* merge the scanvar with any siqfile from the --siqdata command line
     * arg 
     */
    scanvar_siq_merge( sv );

    PTR_ASSIGN( *p_sv, sv );

    return SCANERR_NONE;

fail:
    scanvar_delete(&sv);
    return final_scerr;
}

static scan_err_t file_to_buffer( const char *filename, struct mbuffer *buf )
{
    scan_err_t scerr, final_scerr;
    int retcode;
    struct stat statbuf;
    FILE *f;

#ifdef DEBUG
    printf( "%s %s\n", __FUNCTION__, filename );
#endif

    f = NULL;
    ASSERT( buf->buf==NULL );

    retcode = stat( filename, &statbuf );
    if( retcode != 0 ) {
        fprintf( stderr, "unable to stat \"%s\" : %s\n", 
                filename, strerror(errno) );
        return SCANERR_INVALID_PARAM;
    }

#ifdef DEBUG
    printf( "%s %s is %d bytes\n", __FUNCTION__, filename, (int)statbuf.st_size );
#endif

#define BUFFER_SIZE_SANITY_CHECK_MEGABYTES 1

    /* sanity check for really large files; cap for my own peace of mind */
    if( statbuf.st_size > (BUFFER_SIZE_SANITY_CHECK_MEGABYTES*1024*1024) ) {
        fprintf( stderr, "cowardly refusing to allocate more than %dM bytes of memory\n", 
                BUFFER_SIZE_SANITY_CHECK_MEGABYTES );
        return SCANERR_NOT_PERMITTED;
    }

    scerr = mbuffer_new( buf, statbuf.st_size );
    if( scerr != SCANERR_NONE ) {
        fprintf( stderr, "out of memory for buffer\n" );
        goto fail;
    }

    /* read the entire file into a buffer */
    f = fopen( filename, "rb" );
    if( f==NULL ) {
        fprintf( stderr, "unable to open \"%s\" for reading : %s\n", 
                filename, strerror(errno) );
        final_scerr = SCANERR_NO_ENTRY;
        goto fail;
    }

    retcode = fread( buf->buf, 1, buf->max, f );
    if( retcode < 0 ) {
        fprintf( stderr, "failed to read \"%s\" : %s\n", 
                filename, strerror(errno));
        final_scerr = SCANERR_NO_ENTRY;
        goto fail;
    }

    buf->len = retcode;

    fclose( f );

#ifdef DEBUG
    printf( "%s read %d bytes of %s into buffer\n", __FUNCTION__,
                buf->len, filename );
#endif

    return SCANERR_NONE;
fail:
    if( f ) {
        fclose(f);
    }
    if( buf->buf ) {
        mbuffer_free( buf );
    }

    return final_scerr;
}

static scan_err_t load_siqfile( const char *filename )
{
    scan_err_t scerr;
    struct mbuffer buf;

    dbg2( "%s %s\n", __FUNCTION__, filename );

    memset( &buf, 0, sizeof(struct mbuffer) );
    scerr = file_to_buffer( filename, &buf );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    scerr = siqdata_add_siqfile( buf.buf, buf.len );

    mbuffer_free(&buf);

    return scerr;
}

int main( int argc, char *argv[] )
{
    scan_err_t scerr;
    FILE *devfile;
    int retcode;
    int bytes_read;
    int no_data_counter;
    scan_msg_t msg;
    struct scanvars *sv;
    siqfile_print_fn old_p;
    struct cmdline_option *opt;

    scanlog_set_level(2);
    scands_onetime_init();
    scanvar_onetime_init();

    output_file_list_init( &outfile_list );

    retcode = parse_args(argc,argv,&g_option_list);
    if( retcode != EXIT_SUCCESS ) {
        return retcode;
    }

    /* davep 04-Apr-2013 ; adding option to specify a siqfile from cmdline */
    opt = lookup_option( &g_option_list, OPT_IMPORT_SIQFILE );
    if( opt ) {
        scerr = load_siqfile( opt->buf );
        if( scerr != SCANERR_NONE ) {
            /* load_siqfile() logs error */
            return EXIT_FAILURE;
        }
    }

    opt = lookup_option( &g_option_list, OPT_OUTFILENAME );
    if( opt ) {
        strncpy( outfilename_base, opt->buf, MAX_OUTPUT_FILENAME );
    }

    opt = lookup_option( &g_option_list, OPT_PAGE_NUMBERS );
    if( opt ) {
        add_page_number = true;
    }

    scerr = make_scanvar_from_cmdline( &sv );
    if( scerr != SCANERR_NONE ) {
        /* make_scanvar_from_cmdline() should log error */
        ASSERT(sv==NULL);
        return EXIT_FAILURE;
    }

    /* davep 26-Mar-2013 ; add option to just print the scanvar */
    opt = lookup_option( &g_option_list, OPT_DUMP_SIQFILE );
    if( opt && opt->num32 ) {
        scanvar_to_siqfile(sv);
        return EXIT_SUCCESS;
    }
   
    devfile = fopen( DEV_NAME, "r+" );
    if( devfile==NULL ) {
        perror( "fopen devfile" );
        scanvar_delete( &sv );
        return EXIT_FAILURE;
    }

    mbuffer_new( &siqfile_buf, 1024*4 );
    old_p = set_siqfile_print_fn( scanfw_siqfile_print );
    scanvar_to_siqfile( sv );
    set_siqfile_print_fn( old_p );
//    printf( "%s", siqfile_buf.buf );

//    /* davep 20-Mar-2013 ; print to stdout for debugging */
//    scanvar_to_siqfile(sv);

    scerr = send_siqfile_to_kscanman( devfile, siqfile_buf.buf, siqfile_buf.len );
    if( scerr != SCANERR_NONE ) {
        mbuffer_free( &siqfile_buf );
        scanvar_delete( &sv );
        fclose(devfile);
        return EXIT_FAILURE;
    }

    mbuffer_free( &siqfile_buf );

    /* davep 18-Dec-2012 ; adding cal-to-host. Need to open the output file at
     * scan size (first message), not at page start. We will receive cal
     * metadata before the image data.
     */
    if( cal_to_host() ) {
        char outfilename[FILENAME_MAX+1];

        retcode = snprintf( outfilename, FILENAME_MAX, "%s.%s", 
                        outfilename_base, "gray");
        XASSERT( retcode<FILENAME_MAX, retcode );
        ASSERT(cal_to_host_outfile==NULL);
        dbg2( "%s open cal-to-host file %s\n", __FUNCTION__, outfilename );

        cal_to_host_outfile = fopen( outfilename, "wb" );
        if( cal_to_host_outfile==NULL ) {
            fprintf( stderr, "Unable to option output file \"%s\" for writing: %s", 
                        outfilename, strerror(errno) ); 
            /* continue the scan but the the data will be dropped */
        }
    }

    no_data_counter = 0;
    while( !runscan_quit_flag) {
        memset( &msg, 0, sizeof(msg) );
        bytes_read = fread( &msg, 1, sizeof(msg), devfile );

//        printf( "bytes_read=%d\n", bytes_read );

        if( bytes_read==0 ) {
            sleep(1);
            no_data_counter++;
            continue;
        }
        no_data_counter = 0;

        runscan_dispatch_message( devfile, sv, &msg );
    }

    dbg1( "strips=%d bits_per_pixel=%d pixels_per_row=%d pixels_per_row_padded=%d total_rows=%d\n", 
            runscan_stats.page_datas,
            runscan_self.bits_per_pixel,
            runscan_self.pixels_per_row,
            runscan_self.pixels_per_row_padded,
            runscan_self.total_rows );
    
    scanvar_delete( &sv );

    fclose( devfile );

    if( cal_to_host_outfile != NULL ) {
        fclose(cal_to_host_outfile);
    }

    /* free our global buffer used for reading buffers from the data messages
     * (global so won't have to alloc/free it each time)
     */
    if( data_msg_buffer.buf ) {
        PTR_FREE(data_msg_buffer.buf);
    }

    return EXIT_SUCCESS;
}

