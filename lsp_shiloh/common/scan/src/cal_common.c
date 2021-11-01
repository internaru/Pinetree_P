/**
 * \file cal_common.c
 *
 * \brief Scanner calibration common routines.
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <linux/uaccess.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#include "scos.h" 

#include "list.h"           
#include "lassert.h"
#include "cpu_api.h"
#include "memAPI.h"
#include "regAddrs.h"
#include "io.h"

#include "scancore.h"
#include "scantypes.h"
#include "scanif.h"
#include "scantools.h"
#include "scandbg.h"
#include "scanhwerr.h"
#include "scan.h"
#include "scanlib.h"
#include "scanvars.h"
#include "scanpipe.h"
#include "scanmech.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "cal.h"
#include "scancmdq.h"
#include "calstats.h"
#include "scanimg.h"
#include "cisx.h"
#include "scansen.h"
#include "scantask.h"
#include "afe.h"
#include "scands.h"
#include "scancap.h"
#include "cal_common.h"
#include "scanmem.h"
#include "calcaplist.h"
#include "findcalstrip.h"

#ifdef __KERNEL__
extern void qsort(void *, size_t, size_t, int (*)(const void *,const void *));
#endif

/**  The average_lines() function uses an array on the stack.
 * CAL_MAX_LINES_TO_AVG sets the max size of the array and the max size of the
 * number of pixels we can average.
 *
 * Not using a malloc() because want to reduce the risk of failure and the calls
 * to average_lines() aren't set up to handle failure. The original code assumed
 * we'd have the number of lines scanned fixed at compile time but with our
 * upcoming 8M product, we have to reduce the number of lines scanned based on
 * specific product. Lower memory products need lower number of lines scanned. 
 *
 * davep 23-Jun-2008
 */
#define CAL_MAX_LINES_TO_AVG  60

/* davep 26-Sep-2007 ; Bug 7220 ; adding an additional key to the search so we
 * can support different cal caches for both copy and 300 DPI scan-to-host.
 */

static cal_cache_t Cal_cache[]=
{
    /* mono copy -- put the most common one first */
    {.hres=300,  .cmode=SCAN_CMODE_MONO,  .sensor_num=0, },
    {.hres=300,  .cmode=SCAN_CMODE_MONO,  .sensor_num=1, },
    {.hres=300,  .cmode=SCAN_CMODE_COLOR, .sensor_num=0, },
    {.hres=300,  .cmode=SCAN_CMODE_COLOR, .sensor_num=1, },        
    {.hres=600,  .cmode=SCAN_CMODE_MONO,  .sensor_num=0, },
    {.hres=600,  .cmode=SCAN_CMODE_MONO,  .sensor_num=1, },
    {.hres=600,  .cmode=SCAN_CMODE_COLOR, .sensor_num=0, },
    {.hres=600,  .cmode=SCAN_CMODE_COLOR, .sensor_num=1, },        
    {.hres=1200, .cmode=SCAN_CMODE_MONO,  .sensor_num=0,},
    {.hres=1200, .cmode=SCAN_CMODE_COLOR, .sensor_num=0,},
    {.hres=2400, .cmode=SCAN_CMODE_MONO,  .sensor_num=0,},
    {.hres=2400, .cmode=SCAN_CMODE_COLOR, .sensor_num=0,},
    /* end of list */
    { .hres=0, .cmode=0 }
};

/* cal cache aging; if we haven't used a cache entry in this amount of time,
 * we'll discard the cached values and force a new calibration
 */
#define CAL_CACHE_TIMEOUT (3600*4) /* four hours in seconds */
//#define CAL_CACHE_TIMEOUT (180*SECONDS) /* short timeout for testing */
//#define CAL_CACHE_TIMEOUT (180) /* short timeout for testing */
//#define CAL_CACHE_TIMEOUT (5*60) /* short timeout for testing */
//#define CAL_CACHE_TIMEOUT (-1) /* dont used*/

static uint32_t g_cal_cache_timeout = CAL_CACHE_TIMEOUT;

/* cal cache aging; once the use_count hits this number, we'll discard the
 * cached values and force a new calibration
 */
#define CAL_CACHE_MAX_USE_COUNT  100
//#define CAL_CACHE_MAX_USE_COUNT  5 /* for testing */
//#define CAL_CACHE_MAX_USE_COUNT  0 //dont use

static uint32_t g_cal_cache_max_use_count = CAL_CACHE_MAX_USE_COUNT;

/* Uncomment to turn on extra checks. Increase the value for more debugging.
 *
 * Note: calgrapp.c, calgraphs, and cal-to-host need information from
 * CAL_DEBUG>=1. This is currently a bug(ish) and needs to be fixed.
 */
static uint32_t cal_debug = 1;

/* davep 08-Jul-2011 ; adding global cisx state so can shut it down only when
 * started
 */
static bool cisx_started;

uint32_t sensor_num_cal = 0;

/* davep 23-Apr-2013 ; we're doing all sorts of new tricks with the PRNU/DSNU
 * pixels correction LUT. This structure will store info about the PRNU/DSNU
 * memory pointer so we can clean it up after scan is complete (in cal_done()).
 *
 * Do NOT PTR_FREE() the buf. If the pdlut is in DRAM, the buffer space itself
 * is cached_cal->cur_prnudsnu_lut. But if the pdlut is in fast memory (e.g.,
 * sram), we need to release it back to the fastmem "heap".
 */
struct prnudsnu_memory_alloc {
    uint8_t *ptr;
    int num_bytes;
    dma_addr_t dma_handle;

    /* requires scanmem_free_fast_memory() */
    bool is_fast_memory;

    /* requires dma_unmap_single() when done */
    bool is_dma_map_single;
};
static struct prnudsnu_memory_alloc pdlut_memory[SCANSEN_MAX_SENSORS];

/* Version cookie for the pixel array blob (so can detect protocol changes in
 * future).
 * 
 * Sent in a SMSG_DATA_BLOB along with the tag+length field.
 *
 * Packet Version 1 is:
 *  version cookie (4-bytes)
 *  scan_data_type  (4-bytes)
 *  num_pixels (4-bytes)
 *  array of pixels (cal_pixel_t)
 */
#define CAL_METADATA_PIXEL_ARRAY_V1 0xBE85ECEF 

/* values for CAL_METADATA_DEVICE_ENDIAN */
#define CAL_METADATA_DEVICE_ENDIAN_MSB 1
#define CAL_METADATA_DEVICE_ENDIAN_LSB 2

/* davep 24-Oct-2012 ; adding global to capture capability of the attached mech
 * driver to handle the new cal strip overscan feature (we scan 1" over the cal
 * strip then find cal strip in the image
 */
#define CALSTRIP_FULLSCAN_NULL 0
#define CALSTRIP_FULLSCAN_SUPPORTED 1
#define CALSTRIP_FULLSCAN_NOT_SUPPORTED 2

/**
 * \brief  Debug print (some of) a calcase
 *
 * \author David Poole
 * \date 06-Aug-2012
 */

void cal_debug_print_calcase( const char *file, int line, CalibrationCase *pCCase ) 
{
    dbg1( "%s @%s:%d %dx%d cmode=%d pwm=[%d,%d,%d] exp=[%d,%d,%d] gain=[%d,%d,%d] offset=[%d,%d,%d]\n", 
            __FUNCTION__, file, line, pCCase->horiz_res,
            pCCase->vert_res, pCCase->cmode,
            pCCase->red_pwm,
            pCCase->green_pwm,
            pCCase->blue_pwm,
                
            pCCase->red_exp,
            pCCase->green_exp,
            pCCase->blue_exp,
                
            FLOAT_TO_DECIMAL( pCCase->red_gain ),
            FLOAT_TO_DECIMAL( pCCase->green_gain ),
            FLOAT_TO_DECIMAL( pCCase->blue_gain ),
                
            pCCase->red_offset,
            pCCase->green_offset,
            pCCase->blue_offset
    );
}

/**
 * \brief Debug print a cal cache entry 
 *
 * \param hres horizontal resolution to search for in cal cache
 * \param cmode SCAN_CMODE_MONO or SCAN_CMODE_COLOR
 *
 * \author sw1 
 * \date 29-Oct-2006
 *
 */

void cal_cache_print( uint32_t sensor_num, int hres, scan_cmode_t cmode)
{
    const cal_cache_t *cached_cal;
    int num_bytes_in_lut;

    dbg1("%s %d %d\n", __FUNCTION__, hres, cmode );

    cached_cal = cal_get_cache_settings( sensor_num, hres, cmode);
    if( cached_cal==NULL ) {
        /* silently fail */
        return;
    }
    dbg2( "hres=%d cmode=%d\n", hres, cmode );

    dbg2( "led exps: red=%d green=%d blue=%d\n", 
                cached_cal->led_exp_red,
                cached_cal->led_exp_green, 
                cached_cal->led_exp_blue );
    dbg2( "led pwms: red=%d green=%d blue=%d\n", 
                cached_cal->led_pwm_red,
                cached_cal->led_pwm_green, 
                cached_cal->led_pwm_blue );
    dbg2( "offsets: red=%d green=%d blue=%d\n", 
                cached_cal->offset_red, 
                cached_cal->offset_green,
                cached_cal->offset_blue );
    dbg2( "gains: red=%d green=%d blue=%d\n", 
                FLOAT_TO_DECIMAL(cached_cal->gain_red), 
                FLOAT_TO_DECIMAL(cached_cal->gain_green),
                FLOAT_TO_DECIMAL(cached_cal->gain_blue) );

    num_bytes_in_lut = cached_cal->num_pixels_in_lut*cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE;

    dbg2( "pixels=%d numbytes=%d prnudsnu_lut=%p cur_prnudsnu_lut=%p\n",
                cached_cal->num_pixels_in_lut, 
                num_bytes_in_lut,
                cached_cal->prnudsnu_lut, 
                cached_cal->cur_prnudsnu_lut );
    dbg2( "use_count=%d creation_time=%ld\n", 
                cached_cal->use_count, 
                cached_cal->creation_time );

    if( cmode == SCAN_CMODE_MONO ) {
        dbg2( "mono=%p\n", cached_cal->prnudsnu_lut );
    }
    else {
        dbg2( "red=%p green=%p blue=%p\n", cached_cal->pc_lut_ptr_red, 
                    cached_cal->pc_lut_ptr_green, cached_cal->pc_lut_ptr_blue );
    }
}

/**\brief Get the cached calibration data
 */
cal_err_t retrieveCachedIndex( uint32_t sensor_num, int hres, scan_cmode_t cmode, int *cache_idx )
{
    int i;

    /* davep 26-Sep-2007 ; XXX temp debug */
//    dbg2( "%s hres=%d type=%d\n", 
//                __FUNCTION__, hres, cmode );

    *cache_idx = -1;

    for( i=0; ; i++) {
        /* end of list marker */
        if( Cal_cache[i].hres==0 ) {
            break;
        }

        /* davep 26-Sep-2007 ; XXX temp debug */
//        dbg2( "%s i=%d h=%d v=%d c=%d\n", __FUNCTION__, i, 
//              Cal_cache[i].hres, Cal_cache[i].cmode );

        if (Cal_cache[i].hres == hres &&
            Cal_cache[i].cmode == cmode &&
            Cal_cache[i].sensor_num == sensor_num)
        {
            /* found it! */
            *cache_idx = i;
            return 0;
        }
    }
    return CALERR_NO_ENTRY;
}

/**
 * \brief verify margins and prnudsnu read dma line up correctly
 *
 * \author David Poole
 * \date 28-Apr-2008
 *
 */

void prnudsnu_sanity_check( scan_cmode_t cmode )
{
    uint32_t pic_pixels_per_row, pic_bytes_per_row;
    uint32_t pic_prnudsnu_xfer_pixels, pic_prnudsnu_xfer_bytes;

    /* davep 01-Mar-2013 ; now calling this function from scanlib because pic
     * margin setup moving into scanpipe. If cal isn't running, don't bother to
     * run sanity checks.
     */
    if( pic_prnudsnu_get_bypass_all()  ||
        pic_prnudsnu_get_bypass_prnu() || 
        pic_marg_get_bypass() ) {
        return;
    }

    /* +1 because, well, just because it's necessary, ok? pic margins are
     * confusing 
     */
    pic_pixels_per_row = pic_marg_get_right() - pic_marg_get_left() + 1;

    /* PIC pixel size */
    if( pic_wdma_get_bitpack_mode() == PIC_BITPACK_8BIT ) {
        dbg2( "%s pic wdma bpp=8\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row;
    }
    else {
        dbg2( "%s pic wdma bpp=16\n", __FUNCTION__ );
        pic_bytes_per_row = pic_pixels_per_row*2;
    }

    dbg2( "%s pic ppr=%ld bpr=%ld\n", __FUNCTION__, pic_pixels_per_row,
                pic_bytes_per_row );

    XASSERT( pic_bytes_per_row % ICE_DMA_BYTE_ALIGN == 0, pic_bytes_per_row );

    /* get the PIC prnudsnu byte xfer count; +1 because the register counts from
     * zero 
     */
    pic_prnudsnu_xfer_bytes = pic_prnudsnu_get_lut_xfer_count();

    pic_prnudsnu_xfer_pixels = pic_prnudsnu_xfer_bytes / cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE;

    dbg2( "%s pdxfer pixels=%ld bytes=%ld\n", __FUNCTION__,
                pic_prnudsnu_xfer_pixels, pic_prnudsnu_xfer_bytes );

    /* the xfer bytes must line up with the margins */
    if( cmode==SCAN_CMODE_MONO ) {
        XASSERT( pic_prnudsnu_xfer_pixels==pic_pixels_per_row,
                 pic_prnudsnu_xfer_bytes );
    }
    else {
        XASSERT( pic_prnudsnu_xfer_pixels==pic_pixels_per_row*3,
                 pic_prnudsnu_xfer_bytes );
    }
}

/**
 * \brief
 *
 * This is debug/test code.
 *
 * \author David Poole
 * \date 06-Dec-2008
 *
 */

#if 0
static void break_pdlut( uint8_t *lut_ptr, int pixel_start, int num_pixels )
{
    int i;

    dbg2( "%s lut=0x%x\n", __FUNCTION__, lut_ptr );

    /* we're using 3 bytes per pixel (12 bits DSNU, 12 bits PRNU) */
    for( i=0 ; i<pixel_start ; i++ ) {
        lut_ptr += cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE;
    }

    dbg2( "%s lut=0x%x\n", __FUNCTION__, lut_ptr );

    /* write our three bytes of entry to 0xff which indicates a bad pixel
     * replacement pixel
     */
    for( i=pixel_start ; i<pixel_start+num_pixels ; i++ ) {
        *lut_ptr++ = 0xff;
        *lut_ptr++ = 0xff;
        *lut_ptr++ = 0xff;
        dbg2( "%s 0x%x\n", __FUNCTION__, *(uint32_t*)(lut_ptr-4));
    }

    dbg2( "%s lut=0x%x\n", __FUNCTION__, lut_ptr );
}
#endif

/**
 * \brief  Test/debug dump the pdlut memory cache
 *
 * \author David Poole
 * \date 15-Jul-2013
 */

static void pdlut_memory_dump( void )
{
    int i;

    for( i=0 ; i<SCANSEN_MAX_SENSORS ; i++ ) {
        dbg1( "pdlut idx=%d ptr=%p num_bytes=%d is_fast_memory=%d is_dma_map_single=%d\n", 
                i,
                pdlut_memory[i].ptr, 
                pdlut_memory[i].num_bytes, 
                pdlut_memory[i].is_fast_memory, 
                pdlut_memory[i].is_dma_map_single
            );
    }
}

/**
 * \brief  Initialize and load the PRNU/DSNU Correction LUT
 *
 * Init of the PD Lut is a fairly complex operation.  I rewrote the original
 * code from scratch and we now have this slightly more readable version.
 * 
 * Here's a quick description:
 *
 * We cal the full width of the sensor (or at least as much as we can given DMA
 * restrictions and the sensor margins). We store that calibration info in the
 * cal cache.
 *
 * The user can scan a smaller area than the full sensor width.  The Pixel
 * Correction happens after the PIC margins so we have to set up the PD Lut to
 * match the user's margins. 
 *
 * So we have two PD Luts in RAM: the cached (original) lut and the current lut,
 * the lut specifically for the current scan.  The PIC PD hardware will be
 * pointed at the current lut.  Both LUTs are allocated when the cal cache entry
 * is initialized. Both LUTs are exactly the same byte size BUT the current lut
 * will only contain pieces of the original lut. 
 *
 * For example, given a sensor width like:
 * [ppppppppppppppppppppp]   ('p'==pixel)
 *
 * Here's a full width LUT for an RGB scan:
 * [RRRRRRRRRRRRRRRRRRRRRGGGGGGGGGGGGGGGGGGGGGBBBBBBBBBBBBBBBBBBBBB]
 * [0123456789abcdef012340123456789abcdef012340123456789abcdef01234]
 *
 * Suppose the user wants to scan with margins like:  
 * [xxxppppppppppxxxxxxxx]   ('x'==pixels thrown away; stuff outside the margin)
 *     |         |           ('|'==PIC margins; yes, the second '|' is on an 'x')
 *
 * The pixels from the full width LUT need to be:
 * [   RRRRRRRRRR           GGGGGGGGGG           BBBBBBBBBB        ]
 * [   3456789abc           3456789abc           3456789abc        ]
 *
 * We need to have all the pixels contiguous for the PIC Pixel Correction RDMA:
 * [RRRRRRRRRRGGGGGGGGGGBBBBBBBBBB]
 * [3456789abc3456789abc3456789abc]
 *
 * \author David Poole
 * \date 20-Jun-2008
 *
 */

static void
init_pdlut_using_cached( cal_cache_t *cached_cal )
{
    scan_sensor_margin_t sensor_margins;
    int i, src_width, dst_width, retcode;
    int num_colors, start_pix_diff;
    uint8_t const *src_ptr[3];
    uint8_t *dst_ptr[3];
    const struct scanvars *sv;
    uint8_t const *orig_lut_addr;  /** the full width sensor pdlut */
    uint8_t *cur_lut_addr; /** the sensor pdlut for this user scan */
    const CAL_CONFIG *cal_config = cal_get_config();
    uint32_t cur_lut_addr_numbytes;
    uint8_t *fastmem;
    dma_addr_t dma_handle;

    sv = scanvar_get();

    if( cached_cal->cmode == SCAN_CMODE_MONO ) {
        num_colors = 1;
    }
    else {
        num_colors = 3;
    }

    pic_prnudsnu_set_bypass_prnu(false);
    pic_prnudsnu_set_bypass_dsnu(false);
    pic_prnudsnu_set_bypass_all(false);

#ifdef HAVE_PIC_PC_QUAD
     if(cal_config->USE_QUAD && cached_cal->quad_SF1!= 0)
     {
         pic_prnudsnu_set_quadratic_config( cached_cal->quad_SF1, cached_cal->quad_SF2, cached_cal->quad_shift);
         pic_prnudsnu_set_bypass_quadratic(false);
     }else
     {
         pic_prnudsnu_set_bypass_quadratic(true);
     }
#else
     pic_prnudsnu_set_bypass_quadratic(true);
#endif

    pic_prnudsnu_enable_lut_dma(false);
    pic_prnudsnu_reset_lut_dma();

    /* cal always calibrates from pixel zero (not including the dead left
     * pixels, of course)
     */
    retcode = scansen_get_margins( sv->hw_dpi_horiz, &sensor_margins );
    XASSERT( retcode==0, retcode );

    start_pix_diff = sv->scan_area_pixels.x - sensor_margins.left;

    dbg2( "%s sensor margins left=%ld active=%ld right=%ld\n",  __FUNCTION__,
                sensor_margins.left, 
                sensor_margins.active, 
                sensor_margins.right );

    /* sanity check */
    ASSERT( cached_cal->prnudsnu_lut != NULL );
    ASSERT( cached_cal->cur_prnudsnu_lut != NULL );

    orig_lut_addr = cached_cal->prnudsnu_lut;
    cur_lut_addr = cached_cal->cur_prnudsnu_lut;

    /* davep 06-Dec-2008 ; XXX break the LUT so I can track margins  */
//    break_pdlut( cur_lut_addr, 100, 25 );
//    break_pdlut( orig_lut_addr, 200, 25 );
//    break_pdlut( orig_lut_addr, 300, 25 );

    if( cal_debug > 1 ) {
        /* this stuff is very complicated so add lots of debugging messages */
        dbg2( "%s cached orig=%p cur=%p width=%d\n", __FUNCTION__, cached_cal->prnudsnu_lut,
                    cached_cal->cur_prnudsnu_lut, cached_cal->num_pixels_in_lut );

        dbg2( "%s hinch x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__,
                    sv->user_area_hinch.x,
                    sv->user_area_hinch.y,
                    sv->user_area_hinch.width,
                    sv->user_area_hinch.height );
        dbg2( "%s pixel x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__,
                    sv->scan_area_pixels.x,
                    sv->scan_area_pixels.y,
                    sv->scan_area_pixels.width,
                    sv->scan_area_pixels.height );
    }

    src_width = cached_cal->num_pixels_in_lut * cal_config->PRNUDSNU_LUT_ENTRY_SIZE;
    dst_width = sv->scan_area_pixels.width * cal_config->PRNUDSNU_LUT_ENTRY_SIZE;

    XASSERT( src_width >= dst_width, dst_width );

    src_ptr[0] = orig_lut_addr + cal_config->PRNUDSNU_LUT_ENTRY_SIZE*start_pix_diff;
    for( i=1 ; i<num_colors ; i++ ) {
        src_ptr[i] = src_ptr[i-1] + src_width;
    }

    dst_ptr[0] = cur_lut_addr;
    for( i=1 ; i<num_colors ; i++ ) {
        dst_ptr[i] = dst_ptr[i-1] + dst_width;
    }

    memcpy( dst_ptr[0], src_ptr[0], dst_width );
    for( i=1 ; i<num_colors ; i++ ) {
        memcpy( dst_ptr[i], src_ptr[i], dst_width );
    }

    if( cal_debug > 1 ) {
        if( num_colors==1 ) {
            dbg2( "%s src width=%d ptr=%p\n", __FUNCTION__, 
                        src_width, src_ptr[0] );
            dbg2( "%s dst width=%d ptr=%p\n", __FUNCTION__,
                        dst_width, dst_ptr[0]);
        }
        else {
            dbg2( "%s src %d %p %p %p\n", __FUNCTION__, src_width,
                        src_ptr[0], src_ptr[1], src_ptr[2] );
            dbg2( "%s dst %d %p %p %p\n", __FUNCTION__, dst_width,
                        dst_ptr[0], dst_ptr[1], dst_ptr[2] );
        }
    }
    
    /* davep 18-Jun-2008 ; XXX temp debug; print the LUT as HW will see it */
//    print_prnu_dsnu( (uint8_t*)cur_lut_addr, 120*PRNUDSNU_LUT_ENTRY_SIZE );
//    print_prnu_dsnu( (uint8_t*)cached_cal->prnudsnu_lut, 120*PRNUDSNU_LUT_ENTRY_SIZE );
//    print_prnu_dsnu( (uint8_t*)cur_lut_addr, dst_width*num_colors );
//    print_prnu_dsnu( (uint8_t*)cached_cal->prnudsnu_lut, cached_cal->num_pixels_in_lut );

    cur_lut_addr_numbytes = dst_width * num_colors; 
    pic_prnudsnu_set_lut_xfer_count( cur_lut_addr_numbytes );

    /* davep 06-Apr-2010 ; PIC P/D uses a big-endian 32-bit somewhere in his
     * pipe so need to make sure our LUT is 32-bit big endian.
     */
    /* davep 30-Nov-2010 ; swap the data at the last possible moment instead of
     * once the LUT is built
     */
#ifdef HAVE_PIC_PRNUDSNU_BIG_ENDIAN
    pd_lut_swap32( cur_lut_addr, cur_lut_addr_numbytes );
#endif

    /* flush our data from cache into main memory */
    cpu_dcache_writeback_region( cur_lut_addr, cur_lut_addr_numbytes );

    /* davep 25-Jun-2012 ; use fast memory, if available (e.g., sram).
     * If available, this is just a pointer into an area set aside
     * specifically for scan's use. I don't explicitly free this pointer later.
     * That will probably come back to bite me.
     */
    /* davep 23-Apr-2013 ; switching to memory-by-tag to be able to share
     * scan's compile-time allocated block between cisx and prnu/dsnu
     */
    fastmem = scanmem_get_fast_memory( SCANMEM_TAG_PDLUT, cur_lut_addr_numbytes, cached_cal->sensor_num );
    if( fastmem ) {
        dbg2( "%s using numbytes=%d of fast memory\n", __FUNCTION__, cur_lut_addr_numbytes );
        memcpy( fastmem, cur_lut_addr, cur_lut_addr_numbytes );
        dma_handle = (dma_addr_t)fastmem;

        pdlut_memory[cached_cal->sensor_num].ptr = fastmem;
        pdlut_memory[cached_cal->sensor_num].num_bytes = cur_lut_addr_numbytes;
        pdlut_memory[cached_cal->sensor_num].is_fast_memory = true;
    }
    else {
        /* use regular dram */
        dbg2( "%s using numbytes=%d of regular memory\n", __FUNCTION__, cur_lut_addr_numbytes );

        /* davep 08-Jan-2013 ; convert to linux kernel DMA */
#ifdef __KERNEL__
        /* davep 28-Mar-2013 ; let's try DMA_TO_DEVICE */
        dma_handle = dma_map_single( NULL, (void *)cur_lut_addr,
                                     cur_lut_addr_numbytes, DMA_TO_DEVICE);
//        dma_handle = dma_map_single( NULL, (void *)cur_lut_addr, cur_lut_addr_numbytes, DMA_BIDIRECTIONAL );
        XASSERT( dma_handle, (uint32_t)cur_lut_addr );
        pdlut_memory[cached_cal->sensor_num].is_dma_map_single = true;
        pdlut_memory[cached_cal->sensor_num].dma_handle = dma_handle;
#else
        dma_handle = (dma_addr_t)cur_lut_addr;
#endif
        pdlut_memory[cached_cal->sensor_num].ptr = cur_lut_addr;
        pdlut_memory[cached_cal->sensor_num].num_bytes = cur_lut_addr_numbytes;
    }
    pic_prnudsnu_set_lut_addr(dma_handle);
}

/**
 * Function name: init_cal_using_cached 
 *
 * \brief Loads the settings from a previous calibration
 *
 * Description: Called when we are asked to calibrate a type of scan
 * that we've already calibrated and saved the settings for. We load
 * the settings from that previous calibration rather than performing
 * another calibration.
 *
 * \author Brad Smith
 * \author David Poole
 *
 * Notes: 
 *
 * davep 01-Feb-2007 ; adding scanvars/siqfile overrides
 *
 **/

void 
init_cal_using_cached( cal_cache_t *cached_cal)
{
    const struct scanvars *sv;
    uint32_t sensor_num = cached_cal->sensor_num;

    sv = scanvar_get();

    // loading analog cal data
    if( sv->use_cal_analog_offset ) {
        dbg2( "overriding cal analog offset\n" );
        scanimg_set_analog_offset( sensor_num, sv->cal_analog_offset[SV_COLOR_RED], 
                                   sv->cal_analog_offset[SV_COLOR_GREEN],
                                   sv->cal_analog_offset[SV_COLOR_BLUE] );
    }
    else {
        scanimg_set_analog_offset(sensor_num, cached_cal->offset_red,
                                  cached_cal->offset_green,
                                  cached_cal->offset_blue);
    }

    if( sv->use_cal_analog_gain ) {
        dbg2( "overriding cal analog gain\n" );
        /* davep 03-Jun-2008 ; gain now stored as *100 "float" in the scanvar */
        scanimg_set_analog_gain( sensor_num, sv->cal_analog_gain[SV_COLOR_RED],
                                 sv->cal_analog_gain[SV_COLOR_GREEN],
                                 sv->cal_analog_gain[SV_COLOR_BLUE] );
    }
    else {
        scanimg_set_analog_gain(sensor_num, FLOAT_TO_DECIMAL(cached_cal->gain_red),
                                FLOAT_TO_DECIMAL(cached_cal->gain_green),
                                FLOAT_TO_DECIMAL(cached_cal->gain_blue));
    }

    scanimg_set_led_enable(sensor_num, 1, 1, 1);

    if( sv->use_cal_led_exp ) {
        /* scanvars stored as x100 */
        dbg2( "overriding cal led exp\n" );
        scanimg_set_led_exposure( sensor_num, sv->cal_led_exp[SV_COLOR_RED],
                                  sv->cal_led_exp[SV_COLOR_GREEN],
                                  sv->cal_led_exp[SV_COLOR_BLUE] );
    }
    else {
        scanimg_set_led_exposure(sensor_num, cached_cal->led_exp_red,
                                 cached_cal->led_exp_green,
                                 cached_cal->led_exp_blue );
    }
                  
    if( sv->use_cal_led_pwm ) {
        /* scanvars stored as x100 */
        dbg2( "overriding cal led pwm\n" );
        scanimg_set_led_duty( sensor_num, sv->cal_led_pwm[SV_COLOR_RED],
                              sv->cal_led_pwm[SV_COLOR_GREEN],
                              sv->cal_led_pwm[SV_COLOR_BLUE] );
    }
    else {
        scanimg_set_led_duty( sensor_num, cached_cal->led_pwm_red,
                              cached_cal->led_pwm_green,
                              cached_cal->led_pwm_blue);
    }

    // loading digital cal data

    // tell pic and picdma which sensor/block we want
    pic_prnudsnu_dma_set_block(cached_cal->sensor_num);
    pic_prnudsnu_set_block(cached_cal->sensor_num);
    
    // Setup PRNU/DSNU correction
    pic_prnudsnu_set_bypass_all(true);
    pic_prnudsnu_set_bypass_exp_comp(true);
    pic_prnudsnu_set_bypass_bad_pix_replace(cal_get_config()->BYPASS_REPLACE_BAD_PIXEL);

    /* davep 07-Feb-2007 ; adding command line BPR disable  */
    if( scan_get_config() & SCAN_CONFIG_NOBPR ) {
        pic_prnudsnu_set_bypass_bad_pix_replace(true);
    }

    /* davep 25-Apr-2008 ; adding command line Pixel Correction (prnudsnu)
     * disable  
     */
    if( scan_get_config() & SCAN_CONFIG_NOPC ) {
        pic_prnudsnu_set_bypass_all(true);

        pic_prnudsnu_enable_lut_dma(false);
        pic_prnudsnu_reset_lut_dma();
    }
    else {
        init_pdlut_using_cached( cached_cal );

        /* davep 10-Feb-2011 ; add prnu and/or dsnu disable  */
        if( scan_get_config() & SCAN_CONFIG_NOPRNU ) {
            pic_prnudsnu_set_bypass_prnu(true); 
        }
        if( scan_get_config() & SCAN_CONFIG_NODSNU ) {
            pic_prnudsnu_set_bypass_dsnu(true);
        }
        if( scan_get_config() & SCAN_CONFIG_NOQUAD ) {
            pic_prnudsnu_set_bypass_quadratic(true);
        }

        /* and away we go */
        pic_prnudsnu_enable_lut_dma(true);
        pic_prnudsnu_start_lut_dma();
    }
}

/**
 * \brief  External function telling cal to load its cached settings into
 * hardware
 *
 * The new (new as of this writing) scanlib/scanimg/scansen/afe archicture
 * splits responsibility of the hardware into new areas.  The scanimg class
 * (which owns scansen, the sensor class, and the afe class) is responsible for
 * (most of) the scan block registers and the AFE.  Being responsible for the
 * scan block registers also means scanimg is responsible for the LED PWMs. 
 *
 * Thus cal no longer can directly poke the LEDs and AFE.  Cal must work through
 * scanimg to get that information down into the hardware.
 *
 * This scanimg layer (sort of a "Facade" design pattern [1] ) will reset the
 * scan block and afe upon instantiation.  A scanimg is instantiated by
 * the scanlib setup function then used in the scanlib run function.  So cal
 * must poke its LED and AFE settings into scanimg between the instantiation and
 * running of scanimg.
 *
 * [1] Gamma, Erich; Richard Helm, Ralph Johnson, and John Vlissides (1995).
 * Design Patterns: Elements of Reusable Object-Oriented Software.
 * Addison-Wesley. ISBN 0-201-63361-2. 
 *
 *
 * \author David Poole
 * \date 23-Mar-2008
 *
 */

scan_err_t
cal_load_from_cache( uint32_t dpi, scan_cmode_t cmode, bool update_hit_count, scanvar_cal_t cal_mode, uint32_t sensor_bitmask)
{ 
    cal_err_t calerr;
    int cache_idx;
    cal_cache_t *cached_cal;
    uint32_t sensor_num;

    dbg2( "%s dpi=%ld cmode=%d\n", __FUNCTION__, dpi, cmode );

    XASSERT( cmode==SCAN_CMODE_COLOR||cmode==SCAN_CMODE_MONO, cmode );

    if( scan_get_config()&SCAN_CONFIG_NOCAL ) {
        dbg1("nocal is set so not loading cached cal\n");
        return 0;
    }

    if( cal_mode == SCANVAR_CAL_DISABLED ) {
        dbg1("cal scanvar is disabled so not loading cached cal\n");
        return 0;
    }

    /* davep 23-Apr-2013 ; global to store info about the PRNU/DSNU memory
     * pointer so we can clean it up after scan is complete (in cal_done())
     */
    memset( &pdlut_memory, 0, sizeof(pdlut_memory) );

    sensor_num=0;
    // for every sensor that this scan requires, load up cached cal
    while (sensor_bitmask != 0)
    {
        if (sensor_bitmask & 1)
        {
            calerr = retrieveCachedIndex( sensor_num, dpi, cmode, &cache_idx);
            if( calerr != 0 ) {
                /* we're callable from outside cal.c so need to return a general purpose
                 * failure code
                 */
                return SCANERR_NO_ENTRY;
            }
            cached_cal = &Cal_cache[cache_idx];
            
            if( !cached_cal->in_use ) {
                scanlog(0, "%s cannot use uninitialied cached entry=%d\n", __FUNCTION__,
                        cache_idx );
                return SCANERR_NO_ENTRY;
            }
            
            init_cal_using_cached( cached_cal );
            
            /* davep 10-Oct-2009 ; (Bug 14467) adding a way to load from cache but not
             * update the hit count. The paper present scans will not update the cache
             * hit counter. Regular scans (e.g., copy, scan-to-host) will update the
             * cache hit count.
             */
            if( update_hit_count ) {
                /* davep 03-Apr-2007 ; cache aging */
                // scapri 31-jul-2009 - short circuit cache aging
                if (cached_cal->use_count != -1)
                    cached_cal->use_count++;  
            }
        }
        sensor_num++;
        sensor_bitmask = sensor_bitmask >> 1;
    }

    /* success! */
    return 0;
}

/**
 * \brief look up a cal cache entry 
 *
 *  Note this function is called from outside the scan/copy firmware subsystem
 *  (specifically the ACL code).  The function returns a const pointer to try to
 *  keep kids out of our yard.
 *
 * \author David Poole
 *
 */

const cal_cache_t *
cal_get_cache_settings( uint32_t sensor_num, int cal_hres, scan_cmode_t cmode)
{
    cal_err_t calerr;
    int idx;

    calerr = retrieveCachedIndex( sensor_num, cal_hres, cmode, &idx);
    if( calerr != 0 ) {
        return NULL;
    }

    return &Cal_cache[idx];
}

void
store_to_cal_cache( cal_cache_t *ccache, CalibrationCase *pCCase )
{
    struct timespec curr_ts;

    /* NOTE! We're not saving LUTs here! The LUTs are allocated and put into
     * the cal_cache_t entry in cal_setup() which happens before we start the
     * calibration process (see processCalibrationCase()). The LUT is filled
     * during cal_calc_digital_correction which pokes things into the globals.
     * davep 03-Apr-2007 
     */

    /* save what we've learned */
    ccache->gain_red = pCCase->red_gain;
    ccache->gain_green = pCCase->green_gain;
    ccache->gain_blue = pCCase->blue_gain;
   
    ccache->led_pwm_red = pCCase->red_pwm;
    ccache->led_pwm_green = pCCase->green_pwm;
    ccache->led_pwm_blue = pCCase->blue_pwm;
     
    ccache->led_exp_red = pCCase->red_exp;
    ccache->led_exp_green = pCCase->green_exp;
    ccache->led_exp_blue = pCCase->blue_exp;
    
    ccache->offset_red = pCCase->red_offset;
    ccache->offset_green = pCCase->green_offset;
    ccache->offset_blue = pCCase->blue_offset;

    /* davep 17-Oct-2007 ; better cache aging */
    ccache->use_count = 0;
    /* davep 17-Oct-2007 ; XXX always timeout (good for testing) */
//    ccache->creation_time = 0;
    clock_gettime( CLOCK_REALTIME, &curr_ts );
    ccache->creation_time = curr_ts.tv_sec;

    ccache->in_use = true;
}

/**\brief Routine to get cached calibration result
 */
cal_cache_t *getCalCache(CalibrationCase *pCCase)
{
    cal_err_t calerr;
    int idx;

    calerr = retrieveCachedIndex( pCCase->sensor_num, pCCase->horiz_res, pCCase->cmode, &idx);
    XASSERT( calerr==0, calerr );

    return &(Cal_cache[idx]);
}



/**
 * \brief best effort to covert a cal_err_t to a matching scan_err_t 
 *
 * cal_err_t has a lot more possible states that we don't want in scan_err_t
 * (since scan_err_t can be seen outside the scan code). 
 *
 * If the cal failed, do our best to convert the cal_err_t to something like a
 * scan_err_t.  
 *
 * Note we also peek at the cal_scan_failed_scerr global.
 *
 * \author David Poole
 * \date 18-Jul-2008
 *
 */
static scan_err_t cal_scan_failed_scerr;

void cal_set_scan_error(scan_err_t err)
{
    cal_scan_failed_scerr = err;
}

scan_err_t cal_get_scan_error()
{
    return  cal_scan_failed_scerr;
}


scan_err_t cal_err_to_scan_err( cal_err_t calerr )
{
    /* not using a switch() statement so the compiler won't kvetch about missing
     * cases in switch statement (cal_err_t is an enum)
     */
    if( calerr==CALERR_NONE ) {
        return SCANERR_NONE;
    }
    else if( calerr==CALERR_SCAN_FAILED ) {
        /* the calls to scanlib failed so return that failure if it exist*/
        if(cal_scan_failed_scerr)
            return cal_scan_failed_scerr;
        else
            return SCANERR_CAL_FAILED;
    }
    else if( calerr==CALERR_OUT_OF_MEMORY ) {
        return SCANERR_OUT_OF_MEMORY;
    }
    else if( calerr==CALERR_HARDWARE_FAIL ) {
        return SCANERR_HARDWARE_FAIL;
    }
    /* punt; return a general error which will probably require more detailed
     * debugging 
     */
    return SCANERR_CAL_FAILED;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date Mar-2008
 *
 */

cal_err_t cal_scan_lines_open( scan_target_type_t cal_target_type )
{
    scan_err_t scerr;
    struct scan_area pixel_area;
    struct scanvars *sv;
    uint32_t num32;
    struct scanmech_capabilities mech_cap;
    static int supports_calstrip_fullscan=0;
    
    sv = scanvar_get();

    /* davep 24-Oct-2012 ; add backwards compatible support for mech drivers
     * that don't support the two new cal target types (moving and stationary)
     *
     * Use a three-state value so only have to query the ioctl once.
     */
    if( supports_calstrip_fullscan==CALSTRIP_FULLSCAN_NULL ) {
        /* can this mech driver handle the new types? */
        scerr = scanmech_get_capabilities( &mech_cap );
        if( scerr==SCANERR_NOT_IMPLEMENTED) {
            /* mech driver doesn't have an ioctl */
            supports_calstrip_fullscan = CALSTRIP_FULLSCAN_NOT_SUPPORTED;
        }
        else if( !scanmech_capability_has_target( &mech_cap, cal_target_type )) {
            /* mech driver knows not these mysterious new cal types */
            supports_calstrip_fullscan = CALSTRIP_FULLSCAN_NOT_SUPPORTED;
        }
        else {
            XASSERT( scerr==SCANERR_NONE, scerr );
            supports_calstrip_fullscan = CALSTRIP_FULLSCAN_SUPPORTED;
        }
    }

    /* davep 23-Oct-2012 ; secret sauce to tweak target type used in
     * calibration (useful for testing)
     *
     */
    if( scands_get_integer( "cal_target_type", &num32 ) == SCANERR_NONE ){
        if( num32==SCAN_TARGET_TYPE_CAL ||
            num32==SCAN_TARGET_TYPE_STATIONARY_CAL ||
            num32==SCAN_TARGET_TYPE_MOVING_CAL ) 
        {
            cal_target_type = (scan_target_type_t)num32;
        }
    }

    /* If we're doing a stationary cal, only get a small amount of data. If
     * we're doing a moving cal, capture 1" so we can see the cal strip in all
     * his glory.
     */
    scanvar_get_pixel_area( sv, &pixel_area );
    if( cal_target_type==SCAN_TARGET_TYPE_STATIONARY_CAL ||
        supports_calstrip_fullscan==CALSTRIP_FULLSCAN_NOT_SUPPORTED ) 
    {
        pixel_area.height = get_needed_rows_per_buffer( sv->hw_dpi_horiz);
    }
    else {
        /* scan 1" and we'll look for the cal strip inside the image */
//        pixel_area.height = 1 * sv->hw_dpi_vert;

        /* davep 25-Oct-2012 ; all calibrations run @ 300 dpi to save
         * memory/time
         */
        pixel_area.height = 1 * 300;
    }
    scanvar_set_pixel_area( sv, &pixel_area );

    // Allow the scan mech prepare the page for scanning; stage the page
    scerr = scanmech_prepare(cal_target_type);
    if( scerr != 0 ) {
        dbg2( "%s scanmech prepare failed with scerr=%d\n", __FUNCTION__, scerr );

        /* save the reason for the failure in the global so other code can get
         * it to help diagnose the problem
         */
        cal_set_scan_error(scerr);//cal_scan_failed_scerr = scerr;

        if( scerr==SCANERR_SCAN_CANCELLED ) {
            scerr = scanmech_done( SMECH_DONE_FLAG_CANCEL );
            /* ignore error */
        }

        return CALERR_SCAN_FAILED;
    }

    scerr = scanlib2_setup();

    /* TODO ; need to fix the scanlib functions to return better error codes so
     * we can propagate the actual cause of the failure 
     */
    if( scerr != 0 ) {
        dbg2( "%s scanlib_setup failed with scerr=%d\n", __FUNCTION__, scerr );

        /* save the reason for the failure in the global so other code can get
         * it to help diagnose the problem
         */
         cal_set_scan_error(scerr);//cal_scan_failed_scerr = scerr;

        /* davep 05-Jan-2011 ; call scanmech_done() on setup failure*/
        scanmech_done( SMECH_DONE_FLAG_CANCEL );
        /* ignore error; want the scanlib2_setup() failure */

        return CALERR_SCAN_FAILED;
    }

    /* success */
    return 0;
}

static void clean_pipe_results( void )
{
    struct ice_dma_buffer *icebuf;

    /* davep 28-Sep-2007 ; if any data is still out there on the results list,
     * send it back to PIC. 
     */
    while( 1 ) {
        icebuf = icebuf_pop( pipe_get_pipe_results() );
        if( icebuf==NULL ) {
            break;
        }
        pic_wdma_add_ready( icebuf->channel, &icebuf );   
    }
}

cal_err_t cal_scan_lines_run( void )
{
    scan_err_t scerr;

//    dbg2( "%s count=%d\n", __FUNCTION__, cal_scan_lines_count );

    /* davep 18-Mar-2008 ; XXX want to get rid of these PIPE_FLAGs  */
    /* davep 06-Jun-2008 ; let's see if I can get cal to work without ONE_BUFFER */
    scerr = scanlib2_run( PIPE_FLAG_LINK_RESULTS | PIPE_FLAG_ONE_BUFFER  );

    /* TODO ; need to fix the scanlib functions to return better error codes so
     * we can propagate the actual cause of the failure 
     */
    if( scerr != 0 ) {
        dbg2( "%s scanlib run failed with scerr=%d\n", __FUNCTION__, scerr );

        clean_pipe_results();

        /* NOTE! even if scanlib2_run() fails, must call scanlib2_done() */
        scanlib2_done();

        scanmech_done( SMECH_DONE_FLAG_CANCEL );
        /* ignore error because we're leaving with an error already */

        /* save the reason for the failure in the global so other code can get
         * it to help diagnose the problem
         */
         cal_set_scan_error(scerr);//cal_scan_failed_scerr = scerr;

        return CALERR_SCAN_FAILED;
    }

    /* success */
    return 0;
}

cal_err_t cal_scan_lines_close(void)
{
    cal_err_t calerr;

//    dbg2( "%s count=%d\n", __FUNCTION__, cal_scan_lines_count );

    clean_pipe_results();

    scanlib2_done();

    //cal_scan_lines_count++;
    calerr = scanmech_done( SMECH_DONE_FLAG_SUCCESS );
    if( calerr != 0 ) {
        dbg2( "%s scanmech_done failed calerr=%d\n", __FUNCTION__, calerr );
    }
    return calerr;
}

/**
 * Function name: sortPixels
 *
 * \brief Qsort sort routine.
 *
 * Description: Called by qsort to sort pixels 
 *
 * \param arg1
 * \param arg2
 *
 * \author Brad Smith
 *
 * Notes: 
 *
 **/
static int sortPixels(const void *arg1, const void *arg2)
{
    if ( *((cal_pixel_t *)arg1) < *((cal_pixel_t *)arg2) ) {
        return -1;
    }
    else if ( *((cal_pixel_t *)arg1) > *((cal_pixel_t *)arg2) ) {
        return 1;
    }
    return 0;
}

/** Find the ceiling of a curve.
 *  The function take top 5% valued pixels and take an average to get 
 *  the top boundary value of the curve.
 *  Note: The function will re-order the data buffer.
 * 
 *  numPixels     Number of total pixels
 *  numToDiscard  Number of top values to drop before averaging
 *  numToAvg      Number of pixels to average
 *  
 */
uint16_t findCurveCeiling(uint16_t *buff, int numPixels,  int numToDrop, int numToAvg)
{
    int i;
    uint32_t sum = 0 ;

    dbg2("%s numPixels=%d numtoDrop=%d numToAvg=%d\n", __FUNCTION__, numPixels,
                numToDrop, numToAvg );

    ASSERT(numPixels >= (numToAvg + numToDrop) );
    
    qsort(buff, numPixels, sizeof(uint16_t), sortPixels);

    for(i=numPixels - numToDrop; i > numPixels - numToAvg - numToDrop; i--) {
        sum = sum + buff[i];
    }
      
    dbg2("%s sum=%d numToAvg=%d ceiling=%ld\n", __FUNCTION__, sum, numToAvg, sum/numToAvg );

    return sum/numToAvg;
}

/** Find the floor value of a curve.
 *  The function find bottom valued pixels and take an average to get 
 *  the floor value of the curve.
 *  Note: The function will re-order the data buffer.
 * 
 *  numPixels     Number of total pixels
 *  numToDiscard  Number of top values to drop before averaging
 *  numToAvg      Number of pixels to average
 *  
 */
uint16_t findCurveFloor(uint16_t *buff, int numPixels, int numToDrop, int numToAvg)
{
    int i;
    uint32_t sum = 0 ;
    
    XASSERT(numPixels >= (numToAvg + numToDrop), numPixels );

    qsort(buff, numPixels, sizeof(uint16_t), sortPixels);

    for(i = numToDrop; i < numToAvg + numToDrop; i++) {
        sum = sum + buff[i];
    }

    dbg2( "%s sum=%ld numToAvg=%d floor=%d\n", __FUNCTION__, sum, numToAvg, sum/numToAvg );
      
    return sum/numToAvg;
}
 
/**
 * Function name: average_lines
 *
 * \brief Averages lines.
 *
 * Description: Averages "label_lines" pixel columns and writes the results
 * into the first line of the same buffer that's passed in. The pixels are
 * sorted by value prior to averaging.
 *
 * \param[in] buff    The buffer for the pixels
 * \param[in] pixelsInLine The length of the bufer
 *
 * \author Brad Smith
 * \author David Poole
 *
 * Notes: "num_discards" min and max values are tossed out prior to
 * computing the average.
 *
 * Note: This function isn't used with ICE Lite because of its interlaced BRG
 * data.
 *
 **/
#if 0
static void average_lines( cal_pixel_t *buff, int num_rows, int pixels_per_row, int num_discards )
{
    int pixel;
    int i;
    cal_pixel_t sorted_vals[CAL_MAX_LINES_TO_AVG];
    uint32_t sum;
    int clipped_num_rows;

    dbg2( "%s num_rows=%d ppr==%d num_discards=%d\n", 
                __FUNCTION__, num_rows, pixels_per_row, num_discards );

    ASSERT( num_rows>0 );

    if( num_rows > CAL_MAX_LINES_TO_AVG ) {
        dbg2( "%s ppr=%d max=%d so will only average rows=%d\n", __FUNCTION__,
                    pixels_per_row, CAL_MAX_LINES_TO_AVG, CAL_MAX_LINES_TO_AVG );
        clipped_num_rows = CAL_MAX_LINES_TO_AVG;
    }
    else
    {
        clipped_num_rows = num_rows;
    }


    // Compute the average value for each pixel.

    // for each pixel...
    for( pixel=0 ; pixel<pixels_per_row ; pixel++) {
 
        memset( sorted_vals, 0, sizeof(sorted_vals) );

        // for each line...
        /* davep 20-Jun-2010 ; skip the first two rows of crap */
#if 1
        for (i=2 ; i<clipped_num_rows ;i++) {
            sorted_vals[i-2] = buff[pixel + (pixels_per_row * i)];
        }

//        scanlog_hex_dump( sorted_vals, sizeof(sorted_vals);

        // Sort this pixel's values
        qsort(sorted_vals, clipped_num_rows-2, sizeof(cal_pixel_t), sortPixels);
#else
        for (i=0 ; i<clipped_num_rows ;i++) {
            sorted_vals[i] = buff[pixel + (pixels_per_row * i)];
   }

//        scanlog_hex_dump( sorted_vals, sizeof(sorted_vals);

        // Sort this pixel's values
        qsort(sorted_vals, clipped_num_rows, sizeof(cal_pixel_t), sortPixels);
#endif
        // Throw out min and max values, then compute the average of the remaining values.

        // First compute the sum of the pixels we're keeping.
        sum = 0;
        for (i=0+num_discards;i<clipped_num_rows-num_discards;i++) {
            sum += sorted_vals[i];
        }

        // Now compute the average of those pixels.
        // (We throw away num_discards min values and num_discards max values.)
        buff[pixel] = sum / (clipped_num_rows - (num_discards * 2);
    }
}
#else
/*
    1017/2011: Eric this is another way to average lines by taking a median value
*/
static void average_lines( cal_pixel_t *buff, int num_rows, int pixels_per_row, int num_discards )
{
    int pixel;
    int i;
    cal_pixel_t sorted_vals[CAL_MAX_LINES_TO_AVG];
    //uint32_t sum;
    int clipped_num_rows;

    dbg2( "%s num_rows=%d ppr==%d num_discards=%d\n", 
                __FUNCTION__, num_rows, pixels_per_row, num_discards );

    ASSERT( num_rows>0 );

    if( num_rows > CAL_MAX_LINES_TO_AVG ) {
        dbg2( "%s ppr=%d max=%d so will only average rows=%d\n", __FUNCTION__,
                    pixels_per_row, CAL_MAX_LINES_TO_AVG, CAL_MAX_LINES_TO_AVG );
        clipped_num_rows = CAL_MAX_LINES_TO_AVG;
    }
    else
    {
        clipped_num_rows = num_rows;
    }


    // Compute the average value for each pixel.

    // for each pixel...
    for( pixel=0 ; pixel<pixels_per_row ; pixel++) {
 
        memset( sorted_vals, 0, sizeof(sorted_vals) );

        // for each line...

        for (i=0 ; i<clipped_num_rows ;i++) {
            sorted_vals[i] = buff[pixel + (pixels_per_row * i)];
        }

//        scanlog_hex_dump( sorted_vals, sizeof(sorted_vals);

        // Sort this pixel's values
        qsort(sorted_vals, clipped_num_rows, sizeof(cal_pixel_t), sortPixels);

        //Find a medium value
        buff[pixel] = (sorted_vals[clipped_num_rows/2]+ sorted_vals[clipped_num_rows/2+1])/2;

    }
/**********************	Strip Dump	**********************/
	{
	    struct file *filp;
    	uint32_t sum;
		uint32_t ave = 0;      
		
	    /* Kernel memory access setting */
	    mm_segment_t old_fs = get_fs();
	    
	    set_fs(get_ds());
	 
	 	/* Check Dump or not */
	 	filp = filp_open("/data/strip_dump", O_RDWR, 0);
	 	
	 	/* Skip Dump */
	 	if (IS_ERR(filp)) {
			;
	    }
	    /* Do Dump */
	    else{
	    	dbg1( "[CAL_DUMP] Avg of Every Row\n");
			
			for(i=0 ; i<clipped_num_rows ;i++) {
				sum = 0;
				for( pixel=0 ; pixel<pixels_per_row ; pixel++){
					sum=sum+buff[pixel+(i*pixels_per_row)];
				}
				ave=sum/pixels_per_row;
				dbg1("%d\n", ave >> 8);	// only MSB
			}
			
			filp_close(filp, NULL);
    	}
    	/* Restore Kernel Memory Setting */
		set_fs(old_fs);
	}
/*********************************************************/
}
#endif

/**
 * \brief  Do a cal scan. Return the data with the average of the data.
 *
 *  Simply rolled some previous cut'n'paste code into a function.
 *
 * \param led_enable : 0 LED Disable; 0-9: exposure factor as in x/10; >=10 100% exposure
 *
 * \author David Poole
 * \date Dec-2006
 *
 */

cal_err_t
get_data_and_average( CalibrationCase *pCCase, 
                      int num_colors, 
                      int led_enable,
                      uint16_t *pixel_avgs[] )
{
    cal_err_t calerr, close_calerr;
    int i;
    cal_pixel_t *data_ptrs[3];
    uint32_t num_rows_to_average, num_rows_in_buffer, pixels_per_row;
    uint32_t starting_row, ending_row;
    scan_data_type dtype;
    scan_target_type_t cal_target_type;
    scan_data_type dtype_list[3] = { 
            SCAN_DATA_TYPE_RED, SCAN_DATA_TYPE_GREEN, SCAN_DATA_TYPE_BLUE };
    int num_cis;
    uint8_t channel_array[3];

    /* If we're doing a dark scan, don't move. Otherwise, move.
     *
     * This function is called during the digital scans and is used for both
     * the dark, light, and the gray scans.
     */
    if( led_enable ) {
        cal_target_type = SCAN_TARGET_TYPE_MOVING_CAL;
    }
    else { 
        cal_target_type = SCAN_TARGET_TYPE_STATIONARY_CAL;
    }
    calerr = cal_scan_lines_open(cal_target_type); 
    if( calerr != 0 ) { 
        dbg2( "%s scan open failed calerr=%d\n", __FUNCTION__, calerr );
        return calerr; 
    }

    /* want a dark scan but with previous analog offset/gain still applied */
    scanimg_set_analog_offset(pCCase->sensor_num,
                              pCCase->red_offset, 
                              pCCase->green_offset, 
                              pCCase->blue_offset);
    
    scanimg_set_analog_gain(pCCase->sensor_num,
                            FLOAT_TO_DECIMAL(pCCase->red_gain), 
                            FLOAT_TO_DECIMAL(pCCase->green_gain),
                            FLOAT_TO_DECIMAL(pCCase->blue_gain));

    /* Turn off illumination - don't want any light from another sensor's leaking over */
    turn_off_all_leds();
    
    scanimg_set_led_enable( pCCase->sensor_num,
                            led_enable&&true, led_enable&&true, led_enable&&true );

    /*Eric May 2011: Add support for fractional LED exposure*/
    if(led_enable > 10) led_enable = 10;

    scanimg_set_led_exposure(pCCase->sensor_num,
                             (pCCase->red_exp*led_enable)/10,
                             (pCCase->green_exp*led_enable)/10,
                             (pCCase->blue_exp*led_enable)/10);

    cal_send_uint32_metadata( 3,
                    CAL_METADATA_RED_EXP, (pCCase->red_exp*led_enable)/10,
                    CAL_METADATA_GREEN_EXP, (pCCase->green_exp*led_enable)/10,
                    CAL_METADATA_BLUE_EXP, (pCCase->blue_exp*led_enable)/10
                    );
    scanimg_set_led_duty( pCCase->sensor_num,
                          pCCase->red_pwm, 
                          pCCase->green_pwm, 
                          pCCase->blue_pwm );

    calerr = cal_scan_lines_run(); 
    if (calerr) {
        /* on error, cal_scan_lines_run() will clean up */
        dbg2( "%s scan run failed calerr=%d\n", __FUNCTION__, calerr );
        return calerr; 
    }
    
    for (i=0;i<num_colors;i++)
    {
        // code to calculate array for staggered sensors would go here
        num_cis = PIC_WDMA_NUM_CHANNELS/3;
        // sensor 0 gets channels 0, 2, 4.  sensor 1 gets 1, 3, 5
        channel_array[i] = num_cis*i + pCCase->sensor_num;
    }
    
    ice_getScanData( pCCase->cmode, data_ptrs, &num_rows_in_buffer, &pixels_per_row, channel_array );

    dbg2( "%s num_rows_in_buffer=%d pixels_per_row=%d\n", __FUNCTION__,
            num_rows_in_buffer, pixels_per_row );


    for( i=0 ; i<num_colors ; i++ ) {

        /* we'll have to guess at the dtype */
        if( num_colors==1 ) {
            dtype = SCAN_DATA_TYPE_MONO;
        }
        else {
            dtype = dtype_list[i];
        }

/**********************	Strip Dump	**********************/
		{
		    struct file *filp, *filp1;
		    cal_pixel_t *pData;
		    int row;
		    int pixel;
		    	    
		    /* Kernel memory access setting */
		    mm_segment_t old_fs = get_fs();
		    
		    set_fs(get_ds());
		 
		 	/* Check Dump or not */
		 	filp = filp_open("/data/strip_dump", O_RDWR, 0);
		 	
		 	/* Skip Dump */
		 	if (IS_ERR(filp)) {
				;
		    }
		    /* Do Dump */
		    else{
		    	filp_close(filp, NULL);
		    	/* Open Dump File */
			    if(pCCase->sensor_num == 0)
			    {
			    	if(num_rows_in_buffer == 30){	// 300dpi
						if( num_colors==1 ) {	// M
			            	filp1 = filp_open("/data/shading_f_m_300(2592x30).raw", O_CREAT|O_RDWR, 0);
			        	}
				        else {
							if(i == 0)			// R
					    		filp1 = filp_open("/data/shading_f_r_300(2592x30).raw", O_CREAT|O_RDWR, 0);
							else if(i == 1)		// G
					    		filp1 = filp_open("/data/shading_f_g_300(2592x30).raw", O_CREAT|O_RDWR, 0);
					    	else				// B
					    		filp1 = filp_open("/data/shading_f_b_300(2592x30).raw", O_CREAT|O_RDWR, 0);
				        }
				    }
				    else{							// 600dpi
						if( num_colors==1 ) {	// M
			            	filp1 = filp_open("/data/shading_f_m_600(5168x60).raw", O_CREAT|O_RDWR, 0);
			        	}
				        else {
							if(i == 0)			// R
					    		filp1 = filp_open("/data/shading_f_r_600(5168x60).raw", O_CREAT|O_RDWR, 0);
							else if(i == 1)		// G
					    		filp1 = filp_open("/data/shading_f_g_600(5168x60).raw", O_CREAT|O_RDWR, 0);
					    	else				// B
					    		filp1 = filp_open("/data/shading_f_b_600(5168x60).raw", O_CREAT|O_RDWR, 0);
				        }
					}
				}
				else
				{
					if(num_rows_in_buffer == 30){	// 300dpi
						if( num_colors==1 ) {	// M
			            	filp1 = filp_open("/data/shading_b_m_300(2592x30).raw", O_CREAT|O_RDWR, 0);
			        	}
				        else {
							if(i == 0)			// R
					    		filp1 = filp_open("/data/shading_b_r_300(2592x30).raw", O_CREAT|O_RDWR, 0);
							else if(i == 1)		// G
					    		filp1 = filp_open("/data/shading_b_g_300(2592x30).raw", O_CREAT|O_RDWR, 0);
					    	else				// B
					    		filp1 = filp_open("/data/shading_b_b_300(2592x30).raw", O_CREAT|O_RDWR, 0);
				        }
				    }
				    else{							// 600dpi
						if( num_colors==1 ) {	// M
			            	filp1 = filp_open("/data/shading_b_m_600(5168x60).raw", O_CREAT|O_RDWR, 0);
			        	}
				        else {
							if(i == 0)			// R
					    		filp1 = filp_open("/data/shading_b_r_600(5168x60).raw", O_CREAT|O_RDWR, 0);
							else if(i == 1)		// G
					    		filp1 = filp_open("/data/shading_b_g_600(5168x60).raw", O_CREAT|O_RDWR, 0);
					    	else				// B
					    		filp1 = filp_open("/data/shading_b_b_600(5168x60).raw", O_CREAT|O_RDWR, 0);
				        }
					}
				}
				/* Dump Strip Image */
			    if (IS_ERR(filp1)) {
			        ;
			    }
			    else {
			    	dbg1( "[CAL_DUMP] Shading Cal Dump Start(cis:%d, cmode:%d, ch:%d, ppr:%d, bytes:%d)\n", pCCase->sensor_num, num_colors, i,
			    								pixels_per_row, sizeof(cal_pixel_t) * pixels_per_row * num_rows_in_buffer);
			    	pData = data_ptrs[i];

					vfs_write(filp1, pData, sizeof(cal_pixel_t) * pixels_per_row * num_rows_in_buffer, &filp1->f_pos);
					
					// 1st line dump
					for(pixel=0 ; pixel<pixels_per_row ;pixel++){
						if(pixel == 0)
							dbg1( "[CAL_DUMP] 1st Line Dump\n");
						if(pixel%20 == 0)	// display at every 20 pixel
							dbg1( "%ld\n", pData[pixel] >> 8);	// only MSB
					}
									
					filp_close(filp1, NULL);
			    }
		    }
			/* Restore Kernel Memory Setting */
			set_fs(old_fs);
		}
/*********************************************************/
    
        /* davep 23-Oct-2012 ; highly experimental code! If we have a large
         * buffer, assume we're doing a calstrip overscan
         */
        if( num_rows_in_buffer > 100 ) {
            /* davep 22-Oct-2012 ; digital scan now overscanning the cal strip. Search
             * for the cal strip in the image
             */

            calerr = find_cal_strip( data_ptrs[i], 300, 
                                    pixels_per_row, num_rows_in_buffer, 
                                    &starting_row, &ending_row );
            if( calerr != CALERR_NONE ) {
                /* use a different error so we can return the previous */
                close_calerr = cal_scan_lines_close();

                dbg1( "%s find_cal_strip failed: calerr=%d\n", __FUNCTION__, calerr );
                return calerr;
            }

            num_rows_to_average = ending_row-starting_row;

            dbg2( "%s nr=%d sr=%u er=%u\n", __FUNCTION__, num_rows_to_average,
                    starting_row, ending_row );

            /* adjust the starting point of the data */
            dbg2( "%s ptr=%p\n", __FUNCTION__, data_ptrs[i] );
            data_ptrs[i] += (pixels_per_row*starting_row);
            dbg2( "%s ptr=%p\n", __FUNCTION__, data_ptrs[i] );
        }
        else {
            num_rows_to_average = num_rows_in_buffer;
        }

        calerr = cal_send_pixel_array( CAL_METADATA_CAL_STRIP, 
                                        dtype,
                                        pCCase->pixels_to_scan*num_rows_to_average, 
                                        data_ptrs[i] );

        /* ignore error */

        average_lines( data_ptrs[i], num_rows_to_average, pCCase->pixels_to_scan,
                pCCase->pixels_to_discard );

        /* copy the data we just scanned into the buffer we were given */
        XASSERT( pixel_avgs[i] != NULL, i );
        memcpy( pixel_avgs[i], data_ptrs[i], pCCase->pixels_to_scan*2 );
    }

    close_calerr = cal_scan_lines_close();
    if( close_calerr != 0 ) {
        /* cal_scan_lines_close() logs error */
        return close_calerr;
    }

    return 0;
}


/**
 * \brief  allocate memory for parts of cal
 *
 * Originally added to isolate places that need to worry about dma alignment. 
 *
 * \author David Poole
 * \date 12-Feb-2010
 *
 */

void *cal_get_memory( int datalen_bytes )
{
    return (void *)MEM_MALLOC_ALIGN( datalen_bytes, cpu_get_dcache_line_size() );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Dec-2008
 *
 */

void cal_send_all_calcase_metadata( CalibrationCase *pCCase )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t *blob, *ptr, *endptr;
    int blob_bytes_used;
    const struct scanvars *sv;

    sv = scanvar_get();

    if( !sv->scan_callback_fn || !sv->capture_cal ) {
        return;
    }

#define BLOB_BYTE_SIZE 256*sizeof(uint32_t)

    blob = (uint32_t *)cal_get_memory( BLOB_BYTE_SIZE );
    if( blob==NULL ) {
        dbg1( "%s unable to allocate blob for metadata\n", __FUNCTION__ );
        return;
    }
    dbg2( "%s blob=%p\n", __FUNCTION__, blob );
    XASSERT( ((uint32_t)blob % cpu_get_dcache_line_size())==0, (uint32_t)blob );


    memset( blob, 0, BLOB_BYTE_SIZE );
    ptr = blob;
    endptr = blob + BLOB_BYTE_SIZE/sizeof(uint32_t);

    blob_bytes_used = 0;

    /* encode TLD (Type Length Data) */
#define ENCODE_uint32_t(tag,value) \
    do {\
        *ptr++ = asp_htonl( tag );\
        *ptr++ = asp_htonl( sizeof(uint32_t) );\
        *ptr++ = asp_htonl( value );\
        blob_bytes_used += sizeof(uint32_t)*3;\
        XASSERT( ptr<endptr, (uint32_t)ptr );\
    } while(0);

    ENCODE_uint32_t( CAL_METADATA_HORIZ_RES, pCCase->horiz_res);
    ENCODE_uint32_t( CAL_METADATA_VERT_RES, pCCase->vert_res);
    ENCODE_uint32_t( CAL_METADATA_CMODE, pCCase->cmode );
    ENCODE_uint32_t( CAL_METADATA_PIXELS_PER_ROW, pCCase->pixels_to_scan );
    ENCODE_uint32_t( CAL_METADATA_RED_PWM, pCCase->red_pwm );
    ENCODE_uint32_t( CAL_METADATA_GREEN_PWM, pCCase->green_pwm );
    ENCODE_uint32_t( CAL_METADATA_BLUE_PWM, pCCase->blue_pwm );
    ENCODE_uint32_t( CAL_METADATA_RED_EXP, pCCase->red_exp );
    ENCODE_uint32_t( CAL_METADATA_GREEN_EXP, pCCase->green_exp );
    ENCODE_uint32_t( CAL_METADATA_BLUE_EXP, pCCase->blue_exp );
    ENCODE_uint32_t( CAL_METADATA_RED_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->red_gain) );
    ENCODE_uint32_t( CAL_METADATA_GREEN_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->green_gain) );
    ENCODE_uint32_t( CAL_METADATA_BLUE_ANALOG_GAIN, FLOAT_TO_DECIMAL(pCCase->blue_gain) );
    ENCODE_uint32_t( CAL_METADATA_RED_ANALOG_OFFSET, pCCase->red_offset );
    ENCODE_uint32_t( CAL_METADATA_GREEN_ANALOG_OFFSET, pCCase->green_offset );
    ENCODE_uint32_t( CAL_METADATA_BLUE_ANALOG_OFFSET, pCCase->blue_offset );

    /* davep 08-Sep-2010 ; adding our endian-ness */
#ifdef ASP_ON_LITTLE_ENDIAN
    ENCODE_uint32_t( CAL_METADATA_DEVICE_ENDIAN, CAL_METADATA_DEVICE_ENDIAN_LSB );
#else
    ENCODE_uint32_t( CAL_METADATA_DEVICE_ENDIAN, CAL_METADATA_DEVICE_ENDIAN_MSB );
#endif
    
#undef ENCODE_uint32_t

    memset( &send_msg, 0, sizeof(scan_msg_t) );
    send_msg.msgType = SMSG_DATA_BLOB;
    send_msg.param1 = blob_bytes_used;
    /* param2 unused */
    PTR_ASSIGN( send_msg.param3, blob );

    scerr = sv->scan_callback_fn( &send_msg );
    XASSERT( scerr==0, scerr );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Dec-2008
 *
 */

void cal_send_uint32_metadata( int num_values, ... )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t *blob, *ptr;
    uint32_t tag, value;
    va_list varg;
    int i, num_bytes;
    const struct scanvars *sv;

    sv = scanvar_get();

    if( !sv->scan_callback_fn || !sv->capture_cal ) {
        return;
    }

    va_start( varg, num_values );

    num_bytes = num_values * 3 * sizeof(uint32_t);

    blob = (uint32_t *)cal_get_memory( num_bytes );
    if( blob==NULL ) {
        dbg1( "%s unable to allocate blob for metadata\n", __FUNCTION__ );
        va_end( varg );
        return;
    }

    memset( blob, 0, num_bytes );
    ptr = blob;

    for( i=0 ; i<num_values ; i++ ) {
        tag = va_arg( varg, uint32_t); 
        value = va_arg( varg, uint32_t );

        *ptr++ = asp_htonl( tag );
        *ptr++ = asp_htonl( sizeof(uint32_t) );
        *ptr++ = asp_htonl( value );
    }

    va_end( varg );

    memset( &send_msg, 0, sizeof(scan_msg_t) );
    send_msg.msgType = SMSG_DATA_BLOB;
    send_msg.param1 = num_bytes;
    /* param2 unused */
    PTR_ASSIGN( send_msg.param3, blob );

    scerr = sv->scan_callback_fn( &send_msg );
    XASSERT( scerr==0, scerr );
}

static void cal_send_uint32_array( int tag, int num_values, ... )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t *blob, *ptr;
    uint32_t value;
    va_list varg;
    int i, num_bytes;
    const struct scanvars *sv;

//    dbg2( "%s num_values=%d\n", __FUNCTION__, num_values );

    sv = scanvar_get();

    if( !sv->scan_callback_fn || !sv->capture_cal ) {
        return;
    }

    va_start( varg, num_values );

    /* +2 for the tag and datalen */
    num_bytes = (num_values+2) * sizeof(uint32_t);

    blob = (uint32_t *)cal_get_memory( num_bytes );
    if( blob==NULL ) {
        dbg1( "%s unable to allocate blob for metadata\n", __FUNCTION__ );
        va_end( varg );
        return;
    }

    memset( blob, 0, num_bytes );
    ptr = blob;

    *ptr++ = asp_htonl( tag );
    *ptr++ = asp_htonl( sizeof(uint32_t)*num_values );

    for( i=0 ; i<num_values ; i++ ) {
        value = va_arg( varg, uint32_t );
        *ptr++ = asp_htonl( value );
    }

    va_end( varg );

    memset( &send_msg, 0, sizeof(scan_msg_t) );
    send_msg.msgType = SMSG_DATA_BLOB;
    send_msg.param1 = num_bytes;
    /* param2 unused */
    PTR_ASSIGN( send_msg.param3, blob );

    scerr = sv->scan_callback_fn( &send_msg );
    XASSERT( scerr==0, scerr );
}

void cal_send_pixel_band_counts( CalibrationCase *pCCase )
{
    /* format is simply all three values sent with the corresponding
     * CAL_METADATA_PIXELBAND_color 
     */

//    dbg2( "%s\n", __FUNCTION__ );

    /* hardwiring to three bands */
    if(pCCase->cmode==SCAN_CMODE_MONO) {
        cal_send_uint32_array( CAL_METADATA_PIXELBAND_MONO, 3,  
                        pCCase->pixelsInBand[0][0],
                        pCCase->pixelsInBand[0][1],
                        pCCase->pixelsInBand[0][2] );
    }
    else {
        cal_send_uint32_array( CAL_METADATA_PIXELBAND_RED, 3,
                        pCCase->pixelsInBand[0][0],
                        pCCase->pixelsInBand[0][1],
                        pCCase->pixelsInBand[0][2] );
        cal_send_uint32_array( CAL_METADATA_PIXELBAND_GREEN, 3,
                        pCCase->pixelsInBand[1][0],
                        pCCase->pixelsInBand[1][1],
                        pCCase->pixelsInBand[1][2] );
        cal_send_uint32_array( CAL_METADATA_PIXELBAND_BLUE, 3,
                        pCCase->pixelsInBand[2][0],
                        pCCase->pixelsInBand[2][1],
                        pCCase->pixelsInBand[2][2] );
    }
}

/**
 * \brief  if a pixel in one color plane is set to replace, set the
 * corresponding pixel in the other planes to replace as well
 *
 * THIS FUNCTION SUCKS.
 *
 * Need to rework how we're doing digital correction. Current mechanism
 * immediately encodes the correction values into the PRNU/DSNU hardware LUT.
 * This is DUMB. Need to have the correction values in a native integer format
 * so I can run some tests on them without having to decode/encode them every
 * stinking time.
 *
 * I'll fix the stupid thing as soon as I have some time and my hands aren't
 * hurting so badly.
 *
 * \author David Poole
 * \date 12-Feb-2007
 *
 */

/* davep 12-Jun-2008 ; XXX temp remove until I can get it working again */
#if 0
static void
reconcile_pixel_replacements( int num_colors, uint8_t *luts[] )
{
    int pixel_cnt, i;
    uint8_t *ptr[3];
    uint16_t prnu[3];
    uint16_t dsnu[3];

    XASSERT( num_colors <= 3, num_colors );
    for( i=0 ; i<num_colors ; i++ ) {
        ptr[i] = luts[i];
    }

    for( pixel_cnt=0 ; pixel_cnt<pCCase->pixels_to_scan ; pixel_cnt++ ) {
        /* if this pixel is set to replace, make sure all planes are set to
         * replace
         */

        for( i=0 ; i<num_colors ; i++ ) {
            decode_correction_bytes( &prnu[i], &dsnu[i], ptr[i] );

            if( prnu[i] == CAL_MAX_DIGITAL_CORRECTION ) {
                /* found a bad one ; set all pixels to pixel replace (yes, I'm
                 * using 'i' twice in the same loop and yes, I'm suitably
                 * ashamed of myself.
                 */
                for( i=0 ; i<num_colors ; i++ ) {
                    dbg2( "%s setting pixel %d plane %d to replace\n",
                                __FUNCTION__, pixel_cnt, i );

                    /* set prnu and dsnu to 0xff */
                    encode_correction_bytes( CAL_MAX_DIGITAL_CORRECTION,
                            CAL_MAX_DIGITAL_CORRECTION, ptr[i] );
                }
                break;
            }
        }

        /* skip to next correction value ; 
         * XXX assuming 12-bits for PRNU, 12-bits for DSNU 
         */
        for( i=0 ; i<num_colors ; i++ ) {
            ptr[i] += PRNUDSNU_LUT_ENTRY_SIZE;
        }

    }
}
#endif

/** Utility to retrieve the scan data
 *  scanTy = 0    Mono scan data
 *  scanType = 1  Color scan data
 *  length: the number of data
 */

void ice_getScanData( scan_cmode_t cmode, cal_pixel_t *datas[], 
                      uint32_t *num_rows, uint32_t *pixels_per_row, uint8_t channel_array[] )
{
    struct ice_dma_buffer *icebuf;
    uint16_t *ptr;
    int i,j; 
    int num_colors;
    ATLISTENTRY *results;
    int num_buffers;
    uint32_t sensor_bitmask, current_sensor;

    /* note: datas[] must be an array of at least [3] (three colors, max) */

    if(cmode == SCAN_CMODE_COLOR) {
        num_colors = 3;
    }
    else {
        XASSERT( cmode==SCAN_CMODE_MONO, cmode );
        num_colors = 1;
    }

    *num_rows = 0;

    results = pipe_get_pipe_results(); 

    num_buffers = count_list( results );
//    dbg2( "%s num_buffers=%d\n", __FUNCTION__, num_buffers );

    scanimg_get_sensor_bitmask(&sensor_bitmask);

    for(i=0;i<num_colors;i++)
    {
        current_sensor = sensor_bitmask; // set up bitmask
        while (current_sensor != 0)
        {
            // if this sensor is on, grab the data, put it in the datas array
            if (current_sensor & 0x1)
            {
                icebuf = icebuf_pop(results);
                ASSERT(icebuf!= NULL ); 
                
                /* take our num_rows from the first buffer; we'll verify each subsequent
                 * icebufs have the same num_rows
                 */
                if( *num_rows==0 ) {
                    *num_rows = icebuf->num_rows;
                }
                else {
                    XASSERT( icebuf->num_rows == *num_rows, *num_rows );
                }
                
                /* davep 28-Mar-2013 ; linux kernel memory mapping */
                icebuf_dma_unmap(icebuf);
                
                *pixels_per_row = icebuf->bytes_per_row / CAL_BYTES_PER_PIXEL;
                
//        dbg2( "%s icebuf=%p numrows=%u pixels_per_row=%u\n", __FUNCTION__, 
//                icebuf, *num_rows, *pixels_per_row );
                
                /* davep 22-Jun-2010 ; no more free pass with CACHE_BYPASS :-( */
                cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );
                
                /* davep 05-Jun-2008 ; sanity check the buffer ; make sure we're
                 * getting everything we expect in ONE buffer! or we'll start
                 * calibrating against garbage (can you guess why I put this check in
                 * here?)
                 */
                /* davep 16-Jun-2008 ; TODO put this check back once I figure out how
                 * to get the vert res and/or needed rows per buffer into this
                 * function
                 */
//        needed_num_rows = get_needed_rows_per_buffer( pCCase->horiz_res );
//        XASSERT( icebuf->num_rows>=needed_num_rows, icebuf->num_rows );
                
                ptr = (uint16_t *)icebuf->data;
                
                /* note we're returning a pointer into the buffer, not the buffer
                 * itself!
                 */
                /* davep 23-Mar-2011 ; PIC descriptor DMA can arrive out-of-order
                 * (known bug but I haven't chased it; maybe something in the driver?)
                 * so we can't count on the icebufs arriving in R==0 G==1 B==2 order.
                 */
                XASSERT( icebuf->channel < PIC_WDMA_NUM_CHANNELS, icebuf->channel );

                for (j=0;j<num_colors;j++)
                {
                    if (icebuf->channel == channel_array[j])
                    {
//                        XASSERT( datas[j]==NULL, j );
                        datas[j] = ptr;
                    }
                }
                
                pic_wdma_add_ready( icebuf->channel, &icebuf );   
                
#ifdef CAL_DEBUG
//        scanlog_hex_dump( (unsigned char *)datas[i], 64 );
#endif
            }
            current_sensor = current_sensor >> 1;
        }
    }
}

/**
 * \brief Calculate and return the maximum possible number of pixels we're
 * scanning.
 *
 * Created to handle places allocating buffers based on how many pixels we're
 * scanning.
 *
 * Note: I'm assuming all the code is obeying the sensor margins. There's no
 * guarantee. 
 *
 * \author David Poole
 * \date 05-Jun-2008
 *
 */

int cal_get_max_possible_pixel_width( int horizontal_resolution )
{
    scan_err_t scerr;
    int max_pixel_count;
    scan_sensor_margin_t sensor_margins;

    scerr = scansen_get_margins( horizontal_resolution, &sensor_margins );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* grab all the margins to be extra buffer safe */
    max_pixel_count = sensor_margins.left + 
                      sensor_margins.active +
                      sensor_margins.right;

    return max_pixel_count;
}

/*
 * Send a scan id to host
 */

void cal_send_scan_id( uint32_t id)
{
    /* davep 06-Aug-2012 ; this code was originally using the red analog which
     * confused the host calparse.py graphs.  Changed to 'cal_cap'.
     */
    cal_send_uint32_metadata( 1, CAL_METADATA_CAL_CAP, id);
}

/**
 * \brief roll a ball of code into a function
 *
 * \param scan_defines  Definitions for debug scan to perform:
 *                       : set to do a white scan with digital correction off
 *                       : set to do a gray (half-white) scan with digital correction OFF
 *                       : set to do a gray (half-white) scan with digital correction ON
 *                       : set to do a White scan with digital correction ON
 *                       : set to do a multiple level gray scan (0.2, 0.4, 0.6, 0.8 of White) scan 
 *                             with digital correction ON
 *
 * \author Eric Huang David Poole
 *
 */

void cal_run_debug_scans( CalibrationCase *pCCase, cal_cache_t *cached_cal, uint32_t scan_defines )
{
    cal_err_t calerr;
    uint32_t red_pwm,green_pwm,blue_pwm;

    dbg2( "%s 0x%x\n", __FUNCTION__, scan_defines );

    /* Save the original pwm*/
    red_pwm = pCCase->red_pwm;
    green_pwm = pCCase->green_pwm;
    blue_pwm = pCCase->blue_pwm;

    pic_prnudsnu_dma_set_block(cached_cal->sensor_num);
    pic_prnudsnu_set_block(cached_cal->sensor_num);
    
    if( cal_cap_list_contains( CAL_CAP_PRNU ))
    {
        //White with digital correction off
        pCCase->red_pwm = red_pwm;
        pCCase->green_pwm = green_pwm;
        pCCase->blue_pwm = blue_pwm;

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        pic_prnudsnu_set_bypass_all(true);

        cal_send_scan_id(CAL_CAP_PRNU);

        calerr = cal_debug_scan( pCCase );
        dbg2("%s Done White scan, PRNU by-passed\n", __FUNCTION__ );
    }

    if(scan_defines & CAL_DEBUG_SCAN_GRAY_DIGITAL_OFF )
    {   //Gray(half pwm) scan with digital correctioon off

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        pic_prnudsnu_set_bypass_all(true);

        pCCase->red_pwm = red_pwm/2;
        pCCase->green_pwm = green_pwm/2;
        pCCase->blue_pwm = blue_pwm/2;

        cal_send_uint32_metadata( 3,
                CAL_METADATA_RED_PWM, pCCase->red_pwm,
                CAL_METADATA_GREEN_PWM, pCCase->green_pwm,
                CAL_METADATA_BLUE_PWM, pCCase->blue_pwm );

        //Gray with digital correction off
        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        pic_prnudsnu_set_bypass_all(true);
        cal_send_scan_id(51);
        calerr = cal_debug_scan( pCCase );

        dbg2("%s Done Gray scan, PRNU by-passed\n", __FUNCTION__ );
    }

    if(scan_defines & CAL_DEBUG_SCAN_GRAY_DIGITAL_ON )
    {
        //Gray with digital correction on
        pCCase->red_pwm = red_pwm/2;
        pCCase->green_pwm = green_pwm/2;
        pCCase->blue_pwm = blue_pwm/2;

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        pic_prnudsnu_set_bypass_all(false);
        cal_send_scan_id(52);
        calerr = cal_debug_scan( pCCase );
        dbg2("%s Done Gray scan, PRNU enabled\n", __FUNCTION__ );
    }

    if( cal_cap_list_contains( CAL_CAP_AFTER_PRNU ))
    { //White with digital correction on
        pCCase->red_pwm = red_pwm;
        pCCase->green_pwm = green_pwm;
        pCCase->blue_pwm = blue_pwm;

        cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
        pic_prnudsnu_set_bypass_all(false);
        /* davep 24-Sep-2012 ; change to CAL_CAP_AFTER_PRNU which is what the
         * downstream calgrapp seems to be expecting 
         */
        cal_send_scan_id(CAL_CAP_AFTER_PRNU);
//        cal_send_scan_id(53);
        calerr = cal_debug_scan( pCCase );
        dbg2("%s Done White scan, PRNU enabled\n", __FUNCTION__ );
    }

    if(scan_defines & CAL_DEBUG_SCAN_GRAY_WITH_STEPS)
    {   //Multiple levels gray scan  with digital correction on
        int p;
        for (p=1;p<=4;p++)
        {
            pCCase->red_pwm = red_pwm*0.2*p;
            pCCase->green_pwm = green_pwm*0.2*p;
            pCCase->blue_pwm = blue_pwm*0.2*p;

            cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );
            pic_prnudsnu_set_bypass_all(false);
            cal_send_scan_id(53 + p);
            calerr = cal_debug_scan( pCCase );
        }
        dbg2("%s Done multi-level gray scan, PRNU enabled\n", __FUNCTION__ );
    }

    /*Restore the original pwms*/
    pCCase->red_pwm = red_pwm;
    pCCase->green_pwm = green_pwm;
    pCCase->blue_pwm = blue_pwm;
}

/*
    Do a dummy scan and throw away the data
*/
cal_err_t cal_dummy_scan( CalibrationCase *pCCase )
{
    cal_err_t calerr;
    calerr = cal_scan_lines_open(SCAN_TARGET_TYPE_STATIONARY_CAL); 
    if( calerr != 0 ) { 
        /* cal_scan_lines_open() logs error */
       return calerr;
    }
    cal_send_scan_id(CAL_CAP_DESAT);

    /*Nominal setup*/
    scanimg_set_analog_offset(pCCase->sensor_num,0,0,0);
    scanimg_set_analog_gain(pCCase->sensor_num,FLOAT_TO_DECIMAL(2.18),
                            FLOAT_TO_DECIMAL(2.18),FLOAT_TO_DECIMAL(2.18));
    scanimg_set_led_duty(pCCase->sensor_num,30, 30, 30);
    scanimg_set_led_exposure(pCCase->sensor_num,100, 100, 100);
    scanimg_set_led_enable(pCCase->sensor_num, true, true, true);
    calerr = cal_scan_lines_run(); 
    if (calerr ) {
         return calerr;
    }
    calerr = cal_scan_lines_close();
    if (calerr ) {
         return calerr;
    }

    return 0;
}

/**
 * \brief Run a scan with all the current correction.
 *
 * Doesn't do anything with the data.  Simply runs the scan.  Useful when we're
 * doing cal-to-host so we can see before/after the correction values are
 * applied.
 *
 * \author David Poole
 * \date 14-Mar-2008
 *
 */

scan_err_t cal_debug_scan( CalibrationCase *pCCase )
{
    cal_err_t calerr, final_calerr;
    uint16_t *datas[3];
    uint32_t num_rows, pixels_per_row;
    uint8_t channel_array[3];

    final_calerr = CALERR_NONE;

    calerr = cal_scan_lines_open(SCAN_TARGET_TYPE_MOVING_CAL); 
    if( calerr != 0 ) { 
        /* cal_scan_lines_open() logs error */
        final_calerr = calerr;
    }
    else { 
        scanimg_set_analog_offset(pCCase->sensor_num,
                          pCCase->red_offset, 
                          pCCase->green_offset, 
                          pCCase->blue_offset);

        scanimg_set_analog_gain(pCCase->sensor_num,
                                FLOAT_TO_DECIMAL(pCCase->red_gain), 
                                FLOAT_TO_DECIMAL(pCCase->green_gain ),
                                FLOAT_TO_DECIMAL(pCCase->blue_gain));

        scanimg_set_led_duty( pCCase->sensor_num,
                              pCCase->red_pwm, 
                              pCCase->green_pwm, 
                              pCCase->blue_pwm );

        scanimg_set_led_exposure( pCCase->sensor_num,
                                  pCCase->red_exp, 
                                  pCCase->green_exp, 
                                  pCCase->blue_exp );

        scanimg_set_led_enable( pCCase->sensor_num,
                                pCCase->red_enable, 
                                pCCase->green_enable,
                                pCCase->blue_enable );

        calerr = cal_scan_lines_run(); 
        if (calerr ) {
            /* on error, cal_scan_lines_run() will clean up */
            final_calerr = calerr;
        }
        else {
            /* This call is necessary to free the buffers back to the
             * appropriate data structure. We're not actually looking at the
             * data here.
             */

            // since we don't care about the data, just pick any array
            channel_array[0]=0;channel_array[1]=1;channel_array[2]=2;
            ice_getScanData(pCCase->cmode, datas, &num_rows, &pixels_per_row,channel_array );
        }

        calerr = cal_scan_lines_close();
        if (calerr ) {
            /* cal_scan_lines_close() logs error */
            final_calerr = calerr;
        }
    }

    return final_calerr;
}

/**
 * \brief  Convenience function to handle new PIC float-less functions
 *
 * \author David Poole
 * \date 25-Sep-2012
 */

#if 0
void cal_set_max_digital_gain_by_float( float max_multiplier )
{
    pic_prnu_multiplier_t multiplier;
    int intmult = max_multiplier*10;

    /* davep 16-Apr-2012 ; to build in linux kernel, had to remove float from
     * pic_prnudsnu_set_max_prnu_mult so this function will interface from old
     * float to new hawtness enum
     */

    switch( intmult ) {
        case 15: //1.5x
            multiplier = PIC_PRNU_MULTIPLIER_15;
            break;

        case 20: //2.0x
            multiplier = PIC_PRNU_MULTIPLIER_20;
            break;

        case 30: //3.0x
            multiplier = PIC_PRNU_MULTIPLIER_30;
            break;

        case 50: //5.0x
            multiplier = PIC_PRNU_MULTIPLIER_50;
            break;

        default : 
            dbg1( "invalid multiplier, integer representation %d",intmult );
            XASSERT(0,intmult);
            return;
    }

    pic_prnudsnu_set_max_prnu_mult_col0_mono_even( multiplier );
    pic_prnudsnu_set_max_prnu_mult_col1_odd( multiplier );
    pic_prnudsnu_set_max_prnu_mult_col2( multiplier );
}
#endif

/**
 * \brief Convenience function to set PIC PRNU multiplier based on float*10 
 *
 * \author David Poole
 * \date 16-Nov-2012
 */

void cal_set_max_digital_gain( int intmult )
{
    pic_prnu_multiplier_t multiplier;

    /* davep 16-Apr-2012 ; to build in linux kernel, had to remove float from
     * pic_prnudsnu_set_max_prnu_mult so this function will interface from old
     * float to new hawtness enum
     */

    switch( intmult ) {
        case 15: //1.5x
            multiplier = PIC_PRNU_MULTIPLIER_15;
            break;

        case 20: //2.0x
            multiplier = PIC_PRNU_MULTIPLIER_20;
            break;

        case 30: //3.0x
            multiplier = PIC_PRNU_MULTIPLIER_30;
            break;

        case 50: //5.0x
            multiplier = PIC_PRNU_MULTIPLIER_50;
            break;

        default : 
            dbg1( "invalid multiplier, integer representation %d",intmult );
            XASSERT(0,intmult);
            return;
    }

    pic_prnudsnu_set_max_prnu_mult_col0_mono_even( multiplier );
    pic_prnudsnu_set_max_prnu_mult_col1_odd( multiplier );
    pic_prnudsnu_set_max_prnu_mult_col2( multiplier );
}

/**
 * \brief  Set up hardware blocks for Calibration
 *
 * Originally cal_onetime_init().  Changed to cal_soft_setup() so can call
 * whenever we need CAL to go back to a 100% startup state.
 *
 * Mostly configures PIC.
 *
 * \author David Poole
 * \date 22-Oct-2008
 *
 */

scan_err_t cal_soft_setup( void)
{
   int ret;
   const CAL_CONFIG *cal_config;
   uint32_t pd_save_block_num, pd_dma_save_block_num;
   int i;

   cal_config = cal_get_config();

    // store away current block for the prnudsnu functions, reset all blocks, and restore when done
    pd_save_block_num = pic_prnudsnu_get_block();
    pd_dma_save_block_num = pic_prnudsnu_dma_get_block();    

    // Bypass everything until we calibrate.
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        pic_prnudsnu_set_block(i);   // switch to the block to setup
        pic_prnudsnu_dma_set_block(i);
        
        pic_prnudsnu_set_bypass_all(true);
        pic_prnudsnu_set_bypass_exp_comp(true);
        pic_prnudsnu_set_bypass_bad_pix_replace(true);
        pic_prnudsnu_set_bypass_prnu(true);
        pic_prnudsnu_set_bypass_dsnu(true);
        pic_prnudsnu_set_bypass_quadratic(true);
        pic_prnudsnu_reset_lut_dma();
        pic_prnudsnu_enable_lut_dma(false);

        ret = cal_config->hw_setup(cal_config);
        if(ret) {
            XASSERT(0, ret);
            return SCANERR_INVALID_PARAM;
        }
    }
    pic_prnudsnu_set_block(pd_save_block_num);  // restoring both now....
    pic_prnudsnu_dma_set_block(pd_dma_save_block_num);    
    
    return SCANERR_NONE;
}

/**
 * Function name: cal_onetime_init
 *
 * \brief Initializes the calibration hardware.
 *
 * Description: Called during boot-up to configure the calibration portion
 * of the ASIC.
 *
 * \author Brad Smith
 *
 * Notes: 
 *
 **/

scan_err_t cal_onetime_init( void )
{
    scan_err_t scerr;

    scerr = cal_soft_setup();
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
    
    scerr = cal_cap_list_onetime_init();
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    return SCANERR_NONE;
}

/**
 * \brief  based on a resolution, return the number of rows we need for cal
 *
 *  Cal wants all the rows in one buffer so make mucking with the data easier.
 *  Depending on our resolution, we want more data (to better bury the
 *  occasional sensor artifact or dust or whatnot).
 *
 * Higher buffer sizes give better results but require more memory.
 *
 * \author David Poole
 * \date 05-Jun-2008
 *
 */

int get_needed_rows_per_buffer( int dpi )
{
    scan_err_t scerr;
    uint32_t rows_per_buffer;

    /* stfu compiler */
    rows_per_buffer = 0; 

    switch( dpi ) {
        case 150 :
        case 300 :
            scerr = scands_get_integer( "cal_max_lines_avg_300", &rows_per_buffer );
            if( scerr != SCANERR_NONE ) {
                rows_per_buffer = 30;
            }
            break;

        case 600 :
            scerr = scands_get_integer( "cal_max_lines_avg_600", &rows_per_buffer );
            if( scerr != SCANERR_NONE ) {
                rows_per_buffer = 60;
            }
            break;

        case 1200 :
            scerr = scands_get_integer( "cal_max_lines_avg_1200", &rows_per_buffer );
            if( scerr != SCANERR_NONE ) {
                /* 120 would be too big (too much memory) so default smaller */
                rows_per_buffer = 50;
            }
            break;

        case 2400 :
            /* davep 18-Nov-2010 ; adding 2400 dpi support */
            scerr = scands_get_integer( "cal_max_lines_avg_2400", &rows_per_buffer );
            if( scerr != SCANERR_NONE ) {
                /* default way way smaller */
                rows_per_buffer = 20;
            }
            break;

        default :
            XASSERT( 0, dpi );
    }

    /* davep 18-Nov-2010 ; add sanity check to scale down the row count so
     * we'll stay under PIC WDMA's memory limit
     */

    return (int)rows_per_buffer;
}

/**
 * \brief  set up scanning for calibration 
 *
 * 1) Capture the user's scan area (so we know how to load the PRNU/DSNU
 * LUT).
 *
 * 2) get the sensor's hardware margins so we can cal full width
 *
 * The user's scan area will be in the scanvar. The sensor's margin will be
 * pulled from the sensor driver.
 *
 * \author David Poole
 * \date 13-Mar-2008
 *
 */

void
cal_horizontal_setup( CalibrationCase *pCCase )
{
    int retcode;
    struct scanvars *sv;
    scan_sensor_margin_t sensor_margins;
    uint32_t left_pixel, right_pixel, pixel_width, pixel_height;
    struct scan_area pixel_area;

    sv = scanvar_get();

    /* davep 21-Feb-2012 ; should never have fwmargins enabled during cal
     * (stupid human check)
     */
    XASSERT( !sv->use_fw_margins, sv->id );

    pCCase->horiz_res = sv->hw_dpi_horiz;
    pCCase->vert_res = sv->hw_dpi_vert;

    pixel_height = get_needed_rows_per_buffer( pCCase->horiz_res );

    retcode = scansen_get_margins( sv->hw_dpi_horiz, &sensor_margins );
    XASSERT( retcode==0, retcode );

    /* calculate how much we can capture of what the sensor will offer us; note
     * I'm truncating the available pixels so I err on the side of caution and
     * never try to calibrate crap (as I might if I round up)
     */
    left_pixel = sensor_margins.left;
    right_pixel = sensor_margins.left + sensor_margins.active;

    dbg2( "%s left_pixel=%d right_pixel=%d\n", __FUNCTION__, left_pixel, right_pixel );

    retcode = pic_marg_calc_fit( left_pixel, &right_pixel, sv->pic_bpp,
                                    PIC_MARG_PIXELFIT_TRUNCATE_RIGHT );
    XASSERT( retcode==0, retcode );

    pixel_width = right_pixel - left_pixel;

    /* XXX temp hardwire debug ; I know I have 16-bit pixels */
    XASSERT( pixel_width%8==0, pixel_width );

    pCCase->pixels_to_scan = pixel_width;

    /* Note: this is also the maximum pixel area we can ever usably scan. By
     * "usably", I mean we can freely capture the left/right inactive pixels of
     * the sensor but we certainly can't calibrate them. They're not usable
     * pixels and will cause calibration to barf.
     */
    dbg2( "%s calibrating left=%ld right=%ld pixel_width=%ld vdpi=%d hdpi=%d\n", 
                __FUNCTION__,
                left_pixel, right_pixel, pixel_width, pCCase->vert_res, pCCase->horiz_res );

    /* Hope the above code was correct because scanvar_set_pixel_area() has no
     * mercy with respect to making sure the pixels line up correctly with PIC
     * WDMA alignment!
     */
    memset( &pixel_area, 0, sizeof(pixel_area) );
    pixel_area.x = left_pixel;
    pixel_area.y = 0;
    pixel_area.width = pixel_width;
    pixel_area.height = pixel_height;

    scanvar_set_pixel_area( sv, &pixel_area );

    /* davep 12-Jun-2008 ; XXX temp debug ; removing some globals */
    ASSERT( pCCase->pixels_to_scan > 0 );

    /* now wasn't that simple and straightforward? */
}


/**
 * \brief  Decode a pixel array received in a SMSG_DATA_BLOB
 *
 *
 * \author David Poole
 * \date 18-Nov-2009
 *
 */

scan_err_t cal_decode_pixel_array_msg( uint32_t *buffer, 
                                       scan_data_type *dtype ,
                                       int *num_pixels,
                                       cal_pixel_t **pixel_buf_p )
{
    uint32_t *ptr32;
    uint32_t cookie;

    ptr32 = buffer;

//    scanlog_hex_dump( (unsigned char *)buffer, 64 );

    /* verify cookie */
    cookie = asp_htonl( *ptr32++ );
    if( cookie != CAL_METADATA_PIXEL_ARRAY_V1 ) {
        dbg1( "%s unknown pixel array format=%#lx\n", __FUNCTION__, cookie );
        return SCANERR_INVALID_PARAM;
    }

    /* data type */
    *dtype = (scan_data_type)( asp_htonl(*ptr32++) );

    /* num_pixels */
    *num_pixels = (int)( asp_htonl(*ptr32++) );

    /* pointer to array of pixels */
    *pixel_buf_p = (cal_pixel_t *)ptr32;

    return 0;
}


cal_err_t cal_send_pixel_array( int tag, 
                                  scan_data_type dtype,
                                  int num_pixels, 
                                  cal_pixel_t *pixel_array )
{
    scan_msg_t send_msg;
    scan_err_t scerr;
    uint32_t *blob, *ptr32;
    cal_pixel_t *pixel_ptr;
    int num_bytes;
    const struct scanvars *sv;

//    dbg2( "%s num_values=%d data_type=%d\n", 
//                __FUNCTION__, num_pixels, dtype  );

    /* Blob format:
     *  tag (4-bytes)
     *  length in bytes (4-bytes)
     *
     * payload: 
     *  cookie (4-bytes)
     *  scan_data_type  (4-bytes)
     *  num_pixels (4-bytes)
     *  pixels (array of cal_pixel_t)
     */

    sv = scanvar_get();

    if( !sv->scan_callback_fn || !sv->capture_cal ) {
        return 0;
    }

    /* 5 for the blob's tag and datalen and the payload's cookie, data_type, and
     * num_pixels
     */
    num_bytes = 5 * sizeof(uint32_t);
    /* add in space for the pixels */
    num_bytes += sizeof(cal_pixel_t) * num_pixels;

    blob = (uint32_t *)cal_get_memory( num_bytes );
    if( blob==NULL ) {
        dbg1( "%s unable to allocate blob for metadata\n", __FUNCTION__ );
        return CALERR_OUT_OF_MEMORY;
    }

    memset( blob, 0, num_bytes );
    ptr32 = blob;

    /* tag */
    *ptr32++ = asp_htonl( tag );

    /* length == 3 uint32_t for cookie+scan_data_type+num_pixels followed by our pixels */
    *ptr32++ = asp_htonl( 3*sizeof(uint32_t)+num_pixels*sizeof(cal_pixel_t) );

    /* now encode the blob's payload */

    /* version cookie (so can change format later if we want) */
    *ptr32++ = asp_htonl( CAL_METADATA_PIXEL_ARRAY_V1 );

    /* stamp in the pixel data type */
    *ptr32++ = asp_htonl( (uint32_t)dtype );

    /* number of pixels */
    *ptr32++ = asp_htonl( (uint32_t)num_pixels );

    /* now copy in the pixels */
    pixel_ptr = (cal_pixel_t *)ptr32;
    memcpy( pixel_ptr, pixel_array, sizeof(cal_pixel_t)*num_pixels );

//    scanlog_hex_dump( (unsigned char *)blob, 64 );

    /* off you go! */
    memset( &send_msg, 0, sizeof(scan_msg_t) );
    send_msg.msgType = SMSG_DATA_BLOB;
    send_msg.param1 = num_bytes;
    /* param2 unused */
    PTR_ASSIGN( send_msg.param3, blob );

    scerr = sv->scan_callback_fn( &send_msg );
    XASSERT( scerr==0, scerr );

    return 0;
}

/**
 * \brief flush all entries from the cal cache
 *
 * Marks all the entries in the cal cache as too old.
 *
 * \author David Poole
 * \date 27-Mar-2009
 *
 */

void cal_cache_flush_all( void )
{
    int i;
    cal_cache_t *cached_cal;

    /*
     * Not thread safe!
     *
     * I need to add a lock around the cal cache.
     */

    for( i=0; ; i++) {
        /* end of list marker */
        if( Cal_cache[i].hres==0 ) {
            break;
        }

        cached_cal = &Cal_cache[i];

        /* Mark the entry as old. Next time we use it, we'll force a cal. */
        cached_cal->creation_time = 0;

        /* davep 04-Aug-2009 ; throw another monkey wrench into the flush. Get
         * rid of the memory, too
         */
        if( cached_cal->prnudsnu_lut != NULL ) {
            PTR_FREE( cached_cal->prnudsnu_lut );
        }
        if( cached_cal->cur_prnudsnu_lut ) {
            PTR_FREE( cached_cal->cur_prnudsnu_lut ); 
        }
        /* mark the pointers NULL to be extra safe */
        cached_cal->pc_lut_ptr_red = NULL;
        cached_cal->pc_lut_ptr_green = NULL;
        cached_cal->pc_lut_ptr_blue = NULL;

        cached_cal->in_use = false;
    }
}

void cal_cache_flush_mode( int mode )
{
    int i;
    cal_cache_t *cached_cal;

    /*
     * Not thread safe!
     *
     * I need to add a lock around the cal cache.
     */
	i = mode;

//    for( i=0; ; i++) {
        /* end of list marker */
//        if( Cal_cache[i].hres==0 ) {
//            break;
//        }

        cached_cal = &Cal_cache[i];

        /* Mark the entry as old. Next time we use it, we'll force a cal. */
        cached_cal->creation_time = 0;

        /* davep 04-Aug-2009 ; throw another monkey wrench into the flush. Get
         * rid of the memory, too
         */
        if( cached_cal->prnudsnu_lut != NULL ) {
            PTR_FREE( cached_cal->prnudsnu_lut );
        }
        if( cached_cal->cur_prnudsnu_lut ) {
            PTR_FREE( cached_cal->cur_prnudsnu_lut ); 
        }
        /* mark the pointers NULL to be extra safe */
        cached_cal->pc_lut_ptr_red = NULL;
        cached_cal->pc_lut_ptr_green = NULL;
        cached_cal->pc_lut_ptr_blue = NULL;

        cached_cal->in_use = false;
//    }
}
/**\brief Cleanup routine for calibration case 
 * 
 * The routine free all the resources (hope so) used by the calibration
 * case. It should be called when the case is done.
 * 
 * \param[in] pCCase  The calibration case to be cleaned
 * 
 * \author Eric Huang
*/
void closeCalibrationCase(CalibrationCase *pCCase)
{
    ASSERT(pCCase);

    if(pCCase->cmode==SCAN_CMODE_MONO) {
       PTR_FREE(pCCase->pixels_uncorrectable[0]);
    }
    else
    {
        PTR_FREE(pCCase->pixels_uncorrectable[0]);
        PTR_FREE(pCCase->pixels_uncorrectable[1]);
        PTR_FREE(pCCase->pixels_uncorrectable[2]);
    }
       
    /* davep 29-Nov-2006 ; poison the memory to catch anyone using it */
    memset( pCCase, 0xee, sizeof(CalibrationCase) );

    PTR_FREE(pCCase);

    /* davep 23-Oct-2008 ; leave PIC/Cal spankin' clean */
    cal_done();
}

/* if we we're being forced to cal OR if we've used this entry a lot
 * (implying we're doing a lot of scans in a short amount of time) OR it's
 * been too long since we've done a cal on these settings, force a cal
 */
bool do_cal(CalibrationCase *pCCase, cal_cache_t *cached_cal)
{
    struct timespec curr_ts;

    if (pCCase->forceCalOn)
    {
        dbg2("%s=true: calibration case forceCalOn\n",__FUNCTION__);
        return true;
    }
    if (g_cal_cache_timeout == 0)
    {
        dbg2("%s=true: g_cal_cache_timeout == 0\n",__FUNCTION__);
        return true; // cache always times out
    }
    if (g_cal_cache_timeout == -1)
    {
        dbg2("%s=false: g_cal_cache_timeout == -1\n",__FUNCTION__);
        return false; // cache never times out
    }
    if (cached_cal->creation_time == 0)
    {
        dbg2("%s=true: cached_cal creation time == 0\n",__FUNCTION__);
        return true; // cache always times out
    }
    if (cached_cal->creation_time == -1)
    {
        dbg2("%s=false: cached_cal creation time == -1\n",__FUNCTION__);
        return false; // cache never times out
    }
    if (cached_cal->use_count > g_cal_cache_max_use_count)
    {
        dbg2("%s=true: use_count = %d, g_cal_cache_max_use_count=%d\n",
              __FUNCTION__,cached_cal->use_count,g_cal_cache_max_use_count);
        return true; // used cache too much, recalibrate
    }

    clock_gettime( CLOCK_REALTIME, &curr_ts );
    
    if (TIMER_EXPIRED(curr_ts.tv_sec,cached_cal->creation_time,g_cal_cache_timeout))
    {
        dbg2("%s=true: timer expired - curr=%d, created=%d, timeout=%d\n", __FUNCTION__,
              curr_ts.tv_sec, cached_cal->creation_time, g_cal_cache_timeout);
        return true;
    }
    dbg2("%s: use_count=%d max_use_count=%d, curr=%d, created=%d, timeout=%d\n", __FUNCTION__,
          cached_cal->use_count, g_cal_cache_max_use_count, 
          curr_ts.tv_sec, cached_cal->creation_time, g_cal_cache_timeout);

//    dbg2("%S: No reason to cal - use the cache, luke\n",__FUNCTION__);
    return false;
}

/**
 * \brief Start CISX.
 *
 * \author David Poole
 * \date 28-Jun-2011
 *
 */

void cal_open_cisx( const struct scanvars *sv ) 
{
    scan_err_t scerr;
    uint32_t cisx_ppr, scanx;
    struct cisx_sensor_conf cisxsensor;
    int i;
    uint32_t pixel_sum;
    uint32_t active_pixel_sum;
    uint32_t sensor_bitmask;

    ASSERT( !cisx_started );
    ASSERT( sv );

    /* don't start cisx (useful for debugging/testing) */
    if( scan_get_config() & SCAN_CONFIG_NO_CISX ) {
        return;
    }

    scerr = scansen_get_cisx_conf( sv->hw_dpi_horiz, &cisxsensor );
    if( scerr != SCANERR_NONE ) {
        /* this is a non-CISX sensor so bail */
        return;
    }

    scerr = cisx_soft_setup();
    XASSERT( scerr==SCANERR_NONE, scerr );

    scanimg_get_sensor_bitmask(&sensor_bitmask);
    scerr = cisx_open( sv->cmode, &cisxsensor, sensor_bitmask );

    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 29-Jun-2011 ; sanity checks */
    cisx_get_pixels_per_row( &cisx_ppr );
    scanimg_get_scanx( &scanx );

    XASSERT( scanx >= cisx_ppr, cisx_ppr );

    /* cheap check the data structure is set up correctly (XXX hardwired to
     * three segment sensor for now)
     */
    XASSERT( cisxsensor.num_segments==3, cisxsensor.num_segments );

    pixel_sum = 0;
    active_pixel_sum = 0;
    for( i=0 ; i<cisxsensor.num_segments ; i++ ) {
        pixel_sum += cisxsensor.segments[i].num_dummy + cisxsensor.segments[i].num_data;
        active_pixel_sum += cisxsensor.segments[i].num_data;
    }

    XASSERT( pixel_sum==cisx_ppr, pixel_sum );

    cisx_launch();

    cisx_started = true;
}

/**
 * \brief  Shut down CISX.
 *
 * \author David Poole
 * \date 28-Jun-2011
 *
 */

void cal_close_cisx( void )
{
    if( !cisx_started ) {
        /* gently ignore un-started cisx */
        return;
    }

    /* close and clean up */
    cisx_sanity();
    cisx_close();

    cisx_started = false;
}

/**
 * \brief  allocate bad pixel logging memory, set up the digital gain ranges we
 * want to watch
 *
 * Pulled into function from other code
 *
 * \author David Poole
 * \date 29-Nov-2006
 *
 */

cal_err_t allocate_pixel_logs( CalibrationCase *pCCase )
{
    cal_err_t final_calerr;
    int max_pixels_count;
    int num_colors;
    int i;

    max_pixels_count = cal_get_max_possible_pixel_width( pCCase->horiz_res );
    
    /* davep 04-Jun-2008 ; stupid global variables */
    XASSERT( pCCase->pixels_to_scan <= max_pixels_count, max_pixels_count );

    //Allocate memory for bad-pixel log    
    num_colors = 3;
    if(pCCase->cmode==SCAN_CMODE_MONO) {
        num_colors = 1;
    }

    for( i=0 ; i<num_colors ; i++ ) {
        /* paranoid about memory leaks */
        XASSERT( pCCase->pixels_uncorrectable[i]==NULL, i );

       pCCase->pixels_uncorrectable[i] = cal_get_memory((max_pixels_count + 7)/8);

        if( pCCase->pixels_uncorrectable[i] == NULL ) {
            final_calerr = CALERR_OUT_OF_MEMORY;
            goto fail;
        }

       memset(pCCase->pixels_uncorrectable[i], 0, (max_pixels_count + 7)/8);
    }
    
    /* success! */
    return 0;
fail:
    for( i=0 ; i<num_colors ; i++ ) {
        if( pCCase->pixels_uncorrectable != NULL ) {
            PTR_FREE( pCCase->pixels_uncorrectable[i] );
        }
    }
    return final_calerr;
}

/* calibration values are cached.  After the cached value has been in the cache
 * for a certain number of seconds OR how many times the cached data has been
 * used.
 *
 * The 'cache_hits'  is a way to measure how often the scanner is used. Sensor
 * characteristics can change as the sensor is used (due to temperature).
 *
 * set to 0 to calibrate every scan (cache times out immediately)
 * set to -1 to never timeout any cached cal
 * otherwise, cal cache entries timeout after 'seconds' elapsed after creation
 * OR
 * the entry has been used 'cache_hits' number of times.
 */

scan_err_t cal_set_cal_cache_timeout( uint32_t timeout_seconds, uint32_t cache_hits )
{
   int i;

   // first set the global cal cache timeout to requested seconds, then the creation
   // time in the whole cal cache to the requested seconds, and the use_count to the
   // cache_hits in the whole cal cache as well

   g_cal_cache_timeout = timeout_seconds;
   g_cal_cache_max_use_count = cache_hits;

   /*
    * Not thread safe!
    *
    * I need to add a lock around the cal cache.
    */

   for( i=0; ; i++) {
       /* end of list marker */
       if( Cal_cache[i].hres==0 ) {
           break;
       }

       /* Mark the entry as requested */
       Cal_cache[i].creation_time = timeout_seconds;
       Cal_cache[i].use_count = cache_hits;
   }
   return SCANERR_NONE;
}

/**
 * Function name: cal_setup
 *
 * \brief Prepare for calibration.
 *
 * Description:  Allocates memory (if necessary) and performs all setup necessary
 * prior to calibration.
 *
 * \author Brad Smith
 *
 * Notes: This routine assumes that the BIOS-level ("scan_") horizontal configuration
 * has already been performed.
 *
 **/

cal_err_t cal_setup(CalibrationCase *pCCase)
{
    int num_colors;
    cal_cache_t *cached_cal; 
    int num_bytes_in_lut, num_bytes_in_lut_rounded_up;
    uint8_t *lut, *cur_lut;
    uint8_t *red_ptr, *green_ptr, *blue_ptr;

    // Allocate the PRNU/DSNU correction table.
    // 16bpp, RGB
    /* if we're scanning color, we'll need space for all three of RGB */
    num_colors = pCCase->cmode==SCAN_CMODE_MONO ? 1 : 3 ;

    /* davep 12-Jun-2008 ; XXX temp debug ; removing some globals */
    ASSERT( pCCase->pixels_to_scan > 0 );

    num_bytes_in_lut = pCCase->pixels_to_scan*cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE*num_colors; 

    /* davep 04-Dec-2008 ; round up to make certain cache functions happy */
    num_bytes_in_lut_rounded_up = ICE_DMA_ALIGN_ME( num_bytes_in_lut );

    /* allocate the LUTs from the system heap so they'll persist when the job
     * finishes (we may or may not be in a job when we calibrate so we'll err on
     * the side of caution)
     */
    lut = ice_dma_alloc_persistent_buffer( num_bytes_in_lut_rounded_up );
    if( lut==NULL ) {
        dbg2( "%s failed to get bytes=%d for prnudsnu lut\n", 
                    __FUNCTION__, num_bytes_in_lut_rounded_up );
        return CALERR_OUT_OF_MEMORY;
    }

    cur_lut = ice_dma_alloc_persistent_buffer( num_bytes_in_lut_rounded_up );
    if( cur_lut==NULL ) {
        PTR_FREE( lut );
        dbg2( "%s failed to get bytes=%d for current prnudsnu lut\n", 
                    __FUNCTION__, num_bytes_in_lut_rounded_up );
        return CALERR_OUT_OF_MEMORY;
    }

    // Zero out the LUT so we don't have to worry about PRNU values when calculating DSNU.
    memset( lut, 0, num_bytes_in_lut_rounded_up );

    cached_cal = getCalCache(pCCase);

    /* be paranoid about memory leaks */
    XASSERT( cached_cal->prnudsnu_lut==NULL,
            (uint32_t)cached_cal->prnudsnu_lut );
    XASSERT( cached_cal->cur_prnudsnu_lut==NULL,
            (uint32_t)cached_cal->cur_prnudsnu_lut );

    PTR_ASSIGN( cached_cal->prnudsnu_lut, lut );
    PTR_ASSIGN( cached_cal->cur_prnudsnu_lut, cur_lut );
    cached_cal->num_pixels_in_lut = pCCase->pixels_to_scan;

    // CIS data is received in planar form (i.e. entire row of red followed
    // by entire row of green followed by entire row of blue), and each
    // pixel is 2 bytes.
    if( pCCase->cmode == SCAN_CMODE_COLOR )
    {
        /* work with temporaries to  keep the expressions a little less hairy;
         * don't use PTR_ASSIGN since the red/green/blue lut pointers are just
         * pointers into the real PRNU/DSNU lut
         */
        red_ptr = cached_cal->prnudsnu_lut;
        green_ptr = red_ptr + (pCCase->pixels_to_scan * cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE);
        blue_ptr = green_ptr + (pCCase->pixels_to_scan * cal_get_config()->PRNUDSNU_LUT_ENTRY_SIZE);

        /* now we can use PTR_ASSIGN to point the pointers into their final
         * home
         */
        PTR_ASSIGN( cached_cal->pc_lut_ptr_red, red_ptr );
        PTR_ASSIGN( cached_cal->pc_lut_ptr_green, green_ptr );
        PTR_ASSIGN( cached_cal->pc_lut_ptr_blue, blue_ptr );
    }

    pic_prnudsnu_set_bypass_all(true);
    pic_prnudsnu_set_bypass_exp_comp(true);
    pic_prnudsnu_set_bypass_bad_pix_replace(true);
    pic_prnudsnu_set_bypass_prnu(true);
    pic_prnudsnu_set_bypass_dsnu(true);
    pic_prnudsnu_set_bypass_quadratic(true);
    pic_prnudsnu_reset_lut_dma();
    pic_prnudsnu_enable_lut_dma(false);
  
    return 0;
}

static void init_pixel_bands( CalibrationCase *pCCase ) 
{
    //Setup the gain bands we want to watch and log, no need to clear the counts in band!
    pCCase->bandsToWatch = NUM_PIXEL_BANDS;
    //band 1
    pCCase->bandsDefined[0] = 200; /* 2.0 * 100 (no floats) */
    pCCase->bandsDefined[1] = 250; /* 2.5 * 100 */
    //band 2
    pCCase->bandsDefined[2] = 250; /* 2.5 * 100 */
    pCCase->bandsDefined[3] = 300; /* 3.0 * 100 */
    //band 3
    pCCase->bandsDefined[4] = 300; /* 30 * 100 */
    pCCase->bandsDefined[5] = 20000; /* 200 * 100 */

    pCCase->ClippedHigh[0] = 0;
    pCCase->ClippedHigh[1] = 0;
    pCCase->ClippedHigh[2] = 0;
}

/**
 * \brief  Malloc and initialize a usable CalibrationCase
 *
 * \author David Poole
 * \date 13-May-2008
 *
 */

cal_err_t
create_calibration_case( CalibrationCase **pp_cal_case,
                         uint32_t sensor_num,
                         int resolution, int scanType, 
                         bool force)
{
    cal_err_t calerr;
    CalibrationCase *pCCase;

    *pp_cal_case = NULL;

    pCCase = cal_get_memory(sizeof(CalibrationCase));
    if(!pCCase) {
        scanlog(0, "BAD BAD!! No memory for calibration case, Abor!\n");
        return CALERR_OUT_OF_MEMORY;
    }
    memset( pCCase, 0, sizeof(CalibrationCase) );

    /* build a simple calibration case */
    pCCase->horiz_res = resolution;
    pCCase->vert_res = resolution;
    pCCase->cmode = scanType;
    pCCase->forceCalOn = force; 

    pCCase->red_offset = 0;
    pCCase->green_offset = 0; 
    pCCase->blue_offset = 0; 

    pCCase->red_gain = 1.0;
    pCCase->green_gain = 1.0;
    pCCase->blue_gain = 1.0;
      
    pCCase->red_pwm = 100;
    pCCase->green_pwm = 100;
    pCCase->blue_pwm = 100;

    pCCase->red_exp = 100;
    pCCase->green_exp = 100;
    pCCase->blue_exp = 100;

    pCCase->sensor_num = sensor_num;

    // Calculate the number of high and low pixels we'll discard for each column.
    // Hack -- I'm hard-coding this to 300 until I decide what the best thing
    // to do is for different resolutions. We should be discarding more lines
    // for higher resolutions.
    pCCase->pixels_to_discard = ((300 * CAL_PRNU_MAX_DUST_SIZE_MM) / CAL_MM_PER_INCH) + 1;

    calerr = allocate_pixel_logs( pCCase );
    if( calerr != 0 ) {
        PTR_FREE(pCCase);
        return calerr;
    }

    init_pixel_bands( pCCase );

    *pp_cal_case = pCCase;

    /* success! */
    return 0;
}

#if 0
static void calibration_is_running( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_IN_PROGRESS;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void calibration_is_finished( void )
{
    scan_msg_t msg;
    scan_err_t scerr;

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = SMSG_CAL_CALIBRATION_DONE;

    scerr = scantask_msg_callback(&msg);
    XASSERT( scerr==SCANERR_NONE, scerr );
}
#endif

/** \brief A wrapper routine for scantask to start a calibration.
 * 
 *  This rountine assumes that scan settings have been pushed into the scanvar stack. 
 * 
 * \param[in] resolution  Resolution of the scan
 * \param[in] scanType     Type of the scan
 * \param[in] force       force a calibration to occur (ignore cal cache)
 * 
 * \author Eric Huang
 * \author Brad Smith
 * 
 * davep 12-Oct-2006 ; added force parameter
 *
 */

scan_err_t calibrate( uint32_t sensor_bitmask, int resolution, int scanType, bool force )
{
    scan_err_t scerr;
    CalibrationCase *pCCase;
    cal_err_t calerr;
    const CAL_CONFIG *cal_config; 
    struct scanvars *sv;
    cal_cache_t *cached_cal;
    uint32_t sensor_num;

    dbg1("%s bitmask=0x%X res=%d type=%d force=%d\n", __FUNCTION__, sensor_bitmask,
         resolution, scanType, force);

    cal_config = cal_get_config(); 

    scerr = scands_get_integer_with_default( "cal_debug", &cal_debug, 0 );
    XASSERT( scerr==SCANERR_NONE, scerr );

    /* davep 07-Dec-2010 ; TODO need to detect flatbed vs adf here */
    if( !scancap_has_sensor_dpi( SCAN_DOCUMENT_SOURCE_FLATBED, resolution ) ) {
        /* should have been caught long before we got this far! */
        dbg2( "%s unsupported sensor dpi=%d\n", __FUNCTION__, resolution );
        return SCANERR_INVALID_PARAM;        
    }

    XASSERT( scanType==SCAN_CMODE_MONO||scanType==SCAN_CMODE_COLOR, scanType );

    calerr = 0;
    sensor_num=0;
	sensor_num_cal=0;    
    // for every sensor, create a cal case
    while (sensor_bitmask != 0)
    {
        if (sensor_bitmask & 1)
        {
            // tell cal to host which sensor we are now calibrating
            cal_send_uint32_metadata( 1, CAL_METADATA_NOW_CALIBRATING_SENSOR_NUM, sensor_num); 
            calerr = cal_config->build_case( &pCCase, sensor_num, resolution, scanType, force);
            if( calerr != 0 ) {
                dbg1("fail to create cal case; calerr=%d\n", calerr );
                
                /* calibrate() called outside cal code so must return a right and proper
                 * scan_err_t 
                 */
                return cal_err_to_scan_err( calerr );
            }
            
            calerr = 0;

            // for the many many functions that run after this point, set up the proper prnudsnu
            // and prnudsnu_dma lut block
            pic_prnudsnu_dma_set_block(sensor_num);
            pic_prnudsnu_set_block(sensor_num);

            pic_soft_setup();
            cal_soft_setup();
            
            sv = scanvar_get();
            
            if( sv->cal_mode != SCANVAR_CAL_DISABLED && 
                !( (scan_get_config())& SCAN_CONFIG_NOCAL)) 
            {
                scanvar_set_pic_bpp( sv, 16 );
                pic_wdma_set_bitpack_mode( PIC_BITPACK_16BIT );
                
                cal_horizontal_setup( pCCase );
                
                cached_cal = getCalCache(pCCase);
                /* getCalCache() will make sure we get a valid pointer */

                if (do_cal(pCCase, cached_cal) == true)
                {
                    /* discard previous cal if it exists */
                    /* davep 17-Oct-2007 ; if no calcache timeout is set but if we have a
                     * previous cal, use it (see scan_set_config_no_calcache_timeout() for
                     * more comments)
                     */
                    if( cached_cal->prnudsnu_lut && 
                        !(scan_get_config()&SCAN_CONFIG_NO_CALCACHE_TIMEOUT) ) {
                        PTR_FREE( cached_cal->prnudsnu_lut );
                        PTR_FREE( cached_cal->cur_prnudsnu_lut );
                        /* mark the pointers NULL to be extra safe */
                        cached_cal->pc_lut_ptr_red = NULL;
                        cached_cal->pc_lut_ptr_green = NULL;
                        cached_cal->pc_lut_ptr_blue = NULL;
                        cached_cal->in_use = false;
                    }
                }
                /* unused cache entries have prnudsnu_lut==NULL */
                if(!cached_cal->prnudsnu_lut ) {

//					calibration_is_running(); ////send message(calibration_running)
                    /* davep 23-Mar-2008 ; can't call load from cache anymore since we're
                     * calibrating outside the context of an open ScanLib/ScanImg
                     */
                    scerr = smech_ioctl( SCANMECH_IOCTL_MOVE_TO_HOME, 0, NULL ); //Add the Home Sensor detecting. mod.kks 2014.06.10
                     
                    calerr = cal_config->process_case(pCCase); 
                    
                    if(calerr == SCANERR_NONE)
						scerr = smech_ioctl( SCANMECH_IOCTL_MOVE_TO_HS_ON, 0, NULL );	//Change the Home Sensor ON State. mod.kks 2014.06.10 
                     
                    if(calerr)
                    {
                        /* clean up after ourselves */
                        scan_enable_motor_motion(true);
                        /* invalidate the current cal cache entry (unused cache entries have
                         * prnudsnu_lut==NULL)
                         */
                        cached_cal = getCalCache( pCCase );
                        if( cached_cal->prnudsnu_lut != NULL ) {
                            PTR_FREE( cached_cal->prnudsnu_lut );
                        }
                        if( cached_cal->cur_prnudsnu_lut ) {
                            PTR_FREE( cached_cal->cur_prnudsnu_lut );
                        }
                        /* mark the pointers NULL to be extra safe */
                        cached_cal->pc_lut_ptr_red = NULL;
                        cached_cal->pc_lut_ptr_green = NULL;
                        cached_cal->pc_lut_ptr_blue = NULL;
                        cached_cal->in_use = false;
                        
                        /* we need to output a different message here so we can tell a
                         * failure. However I have some scripts that depend on this. Need to fix
                         * those scripts.
                         */
                        scanlog(0, "WARNING: CALIBRATION FAILED! err=%d\n", calerr);
                    }
//							calibration_is_finished(); //send message(calibration_is_finished)
                }
            }
            
            /* we use 'calerr' to decide what to return from this function so don't
             * touch it from here on! 
             */

			//send message(calibration_is_finished)
//			calibration_is_finished();            

            /* clean up */
            closeCalibrationCase(pCCase);  
            
            /* calibrate() called outside cal code so must return a right and proper
             * scan_err_t 
             */
            if( calerr != 0 ) {
                return cal_err_to_scan_err( calerr );
            }
        }
        
        sensor_num++;
        sensor_bitmask = sensor_bitmask >> 1;
        sensor_num_cal = sensor_num;
    }
    return SCANERR_NONE;
}


/**
 * \brief clean up hardware blocks responsible for calibration
 *
 * Called by scanlib when scanning is complete.  Because cal owns PIC, cal is
 * responsible for cleaning up PIC after a scan is complete.
 *
 * See also Bug 10870.
 *
 * \author David Poole
 * \date 21-Oct-2008
 *
 */

scan_err_t cal_done( void )
{
    bool was_enabled;
    int i;
	
    dbg2( "%s\n", __FUNCTION__ );

    /* stop any running DMA */
    was_enabled = pic_interrupt_disable();

    /* reset pic and cal back to power-on defaults */
    pic_soft_setup();
    cal_soft_setup();

    /* shortcut -- bypass everything in PIC? */
    pic_bm_set_bypass( true );
    pic_prnudsnu_set_bypass_all(true);
    pic_prnudsnu_enable_lut_dma(false);
    pic_prnudsnu_reset_lut_dma();
    pic_hs_set_bypass( true );
    pic_bdr_set_bypass( true );

    /* davep 07-Jul-2011 ; close up CISX */
    cal_close_cisx();

    /* ready to roll */
    if( was_enabled ) {
        pic_interrupt_enable();
    }

    /* davep 23-Apr-2013 ; if we did anything clever with specialized memory
     * for the pdlut, clean it up now
     */

    /* davep 15-Jul-2013 ; handy function for dumping state of pdlut_memory[] */
    if( 0 ) {
        pdlut_memory_dump();
    }

    for (i=0;i<scansen_get_num_sensors();i++)
    {
        if( pdlut_memory[i].is_fast_memory ) {
            scanmem_free_fast_memory( SCANMEM_TAG_PDLUT, pdlut_memory[i].ptr,
                                      pdlut_memory[i].num_bytes, i );
        }
        if( pdlut_memory[i].is_dma_map_single ) {
#ifdef __KERNEL__
            dma_unmap_single( NULL, pdlut_memory[i].dma_handle, 
                              pdlut_memory[i].num_bytes, DMA_TO_DEVICE );
#else
            /* yeah, this shouldn't happen */
            ASSERT(0);
#endif
        }
    }

    memset( &pdlut_memory, 0, sizeof(pdlut_memory) );
    return SCANERR_NONE;
}


/**
 * \brief  Load the freshly created PDLUT back into PRNU/DSNU DMA
 *
 * I'm adding a cal_done() function to shut down cal/PIC once a scan is done.
 * The original code would leave PIC in a partially configured state which could
 * cause all sorts of when doing an uncalibrated scan following a calibrated
 * scan.
 *
 * In my debug/test code, I run cal_debug_scan() several times after completing
 * digital correction. Before, the PDLUT would be untouched and ready to use
 * again. Now with the cal_done() being called after each scan, I have to
 * reinitialize the PDLUT.
 *
 * \author David Poole
 * \date 22-Oct-2008
 *
 */

void cal_debug_scan_setup_pdlut( const cal_cache_t *cached_cal, scan_cmode_t cmode)
{
    int num_bytes_in_lut;
    uint8_t *cur_lut_addr; 
    const CAL_CONFIG *cal_config;
    dma_addr_t dma_handle;

    dbg2( "%s\n", __FUNCTION__ );

    cal_config = cal_get_config();

    pic_prnudsnu_dma_set_block(cached_cal->sensor_num);
    pic_prnudsnu_set_block(cached_cal->sensor_num);    
    
    pic_prnudsnu_enable_lut_dma(false);

    if( cmode == SCAN_CMODE_MONO ) {
        num_bytes_in_lut = cached_cal->num_pixels_in_lut*cal_config->PRNUDSNU_LUT_ENTRY_SIZE;
        pic_prnudsnu_set_lut_xfer_count( num_bytes_in_lut );
    }
    else {
        /* the *3 is for the three planes of RGB */
        num_bytes_in_lut = cached_cal->num_pixels_in_lut*cal_config->PRNUDSNU_LUT_ENTRY_SIZE * 3;
        pic_prnudsnu_set_lut_xfer_count(num_bytes_in_lut);
    }
        
    /* davep 01-Dec-2010 ; use a temp pointer for my own sanity */
    cur_lut_addr = cached_cal->cur_prnudsnu_lut;

    /* copy our spankin' fresh pdlut to our working lut; we'll byteswap this
     * lut, not the original lut
     */
    memcpy( cur_lut_addr, cached_cal->prnudsnu_lut, num_bytes_in_lut );

    /* davep 06-Dec-2008 ; XXX break the LUT so I can track margins  */
//    break_pdlut( cur_lut_addr, 100, 25 );
//    break_pdlut( cur_lut_addr, 200, 25 );
//    break_pdlut( cur_lut_addr, 300, 25 );

    /* davep 06-Apr-2010 ; PIC P/D uses a big-endian 32-bit somewhere in his
     * pipe so need to make sure our LUT is 32-bit big endian.
     */
    /* davep 30-Nov-2010 ; swap the data at the last possible moment instead of
     * once the LUT is built
     */
#ifdef HAVE_PIC_PRNUDSNU_BIG_ENDIAN
    pd_lut_swap32( cur_lut_addr, num_bytes_in_lut );
#endif

    /* flush our data from cache into main memory */
    cpu_dcache_writeback_region( cur_lut_addr, num_bytes_in_lut );

    /* davep 08-Jan-2013 ; convert to linux kernel DMA */
#ifdef __KERNEL__
    dma_handle = dma_map_single( NULL, (void *)cur_lut_addr, num_bytes_in_lut, DMA_BIDIRECTIONAL );
    XASSERT( dma_handle, (uint32_t)cur_lut_addr );
#else
    dma_handle = (dma_addr_t)cur_lut_addr;
#endif
    pic_prnudsnu_set_lut_addr(dma_handle);

    pic_prnudsnu_set_bypass_bad_pix_replace(cal_config->BYPASS_REPLACE_BAD_PIXEL); 
    pic_prnudsnu_set_bypass_prnu(false); 
    pic_prnudsnu_set_bypass_dsnu(false);

#ifdef HAVE_PIC_PC_QUAD
    /*Enable quadratic correction if we the setting is valid!*/
    if(cal_config->USE_QUAD && cached_cal->quad_SF1 != 0 )
    {  
        dbg1("%s: Enable quadratic correction, %d %d %d\n", __FUNCTION__, 
                cached_cal->quad_SF1, cached_cal->quad_SF2, cached_cal->quad_shift);
        pic_prnudsnu_set_quadratic_config( cached_cal->quad_SF1, cached_cal->quad_SF2, cached_cal->quad_shift);
        pic_prnudsnu_set_bypass_quadratic(false);
    }
    else
    {
        dbg1("%s: Disable quadratic correction use_quad=%d SF1=%d\n", 
                __FUNCTION__, cal_config->USE_QUAD, cached_cal->quad_SF1 );
        pic_prnudsnu_set_bypass_quadratic(true);
    }
#else
    pic_prnudsnu_set_bypass_quadratic(true);
#endif

    pic_prnudsnu_set_bypass_all(false);
  
    pic_prnudsnu_enable_lut_dma(true);
    pic_prnudsnu_start_lut_dma();
}

/*
Check to see if calibration has been done with certain resolution on all sensors requested
*/
bool cal_check_cal_done(int res, scan_cmode_t cmode, uint32_t sensor_bitmask)
{ 
    const cal_cache_t *cached_cal;
    uint32_t sensor_num;

    sensor_num=0;
    while (sensor_bitmask != 0)
    {
        if (sensor_bitmask & 1)
        {
            cached_cal = cal_get_cache_settings( sensor_num, res, cmode);

            ASSERT( cached_cal!=NULL );

            if( !cached_cal->in_use ) 
                return false;
        }
        sensor_num++;
        sensor_bitmask = sensor_bitmask >> 1;
    }
    return true;
}

void turn_off_all_leds()
{
    int i;
    int num_sensors;
    
    /* turn off illumination for all sensors */
    num_sensors = scansen_get_num_sensors();
    for (i=0;i<num_sensors;i++)
    {
        scanimg_set_led_enable(i, false, false, false);
    }
}
