/*
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#include "scos.h" 

#include "list.h"           
#include "lassert.h"
#include "interrupt_api.h"
#include "intnums.h"
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

#define BAD_PIXEL_DUMP			0

/** capture statistics on pixel recovery */
struct pixel_recovery_stats {
    /** number of pixels with bad PRNU */
    int bad_prnu;

    /** number of pixels with bad DSNU */
    int bad_dsnu;

    /** pixels set to max gain in hope of recovery */
    int max_gains;  

    /** number of pixels set to be replaced */
    int replaced; 

    /** number of clusters of N consecutive pixels replaced */
    int num_replaced_clusters;
};

static struct pixel_recovery_stats g_pixel_recovery_stats;

extern uint32_t sensor_num_cal;
int color_cnt = 0;

//static bool (pcal_config->BYPASS_REPLACE_BAD_PIXEL) = false; 
//
//bool bypass_badpixel_replacement()
//{
//    return (pcal_config->BYPASS_REPLACE_BAD_PIXEL);
//}

/**
 * \brief  pack PRNU and DSNU correction numbers into the PIC hardware LUT format PRNU[23:12] DSNU[11:0]
 *
 * Moved into a function from other code.
 *
 * \author David Poole
 * \date 17-Dec-2006
 *
 */
static void
encode_correction_bytes_24bit( uint16_t prnu_corr, uint16_t dsnu_corr, uint8_t corr_bytes[] )
{
    // Set both PRNU and DSNU correction.
    // We're currently using 12 bits for PRNU and 12 bits for DSNU, for a total
    // of 24 bits or 3 bytes.  The 8 msb of the PRNU correction value go into
    // the first byte, with the 4 lsb going into the top half of the second
    // byte. The 4 msb of the DSNU correction value go in the bottom half of the
    // second byte, with the lsb going into the third byte.

    corr_bytes[0] = (uint8_t)((prnu_corr >> 4) & 0xff);
    corr_bytes[1] = (uint8_t)(((prnu_corr & 0x0f) << 4) & 0xf0);
    corr_bytes[1] |= (uint8_t)((dsnu_corr >> 8)& 0x0f);
    corr_bytes[2] = (uint8_t)(dsnu_corr & 0xff);
}
/*
 * \brief  pack PRNU and DSNU correction numbers into the PIC hardware LUT format PRNU[31:16] DSNU[15:0]

*/
static void
encode_correction_bytes_32bit( uint16_t prnu_corr, uint16_t dsnu_corr, uint8_t corr_bytes[] )
{
    // Set both PRNU and DSNU correction.
    // We're currently using 12 bits for PRNU and 12 bits for DSNU, for a total
    // of 24 bits or 3 bytes.  The 8 msb of the PRNU correction value go into
    // the first byte, with the 4 lsb going into the top half of the second
    // byte. The 4 msb of the DSNU correction value go in the bottom half of the
    // second byte, with the lsb going into the third byte.

    corr_bytes[0] = (uint8_t)((prnu_corr >> 8) & 0xff); //PRNU MSB
    corr_bytes[1] = (uint8_t)(prnu_corr & 0xff);        //PRNU LSB
    corr_bytes[2] = (uint8_t)((dsnu_corr >> 8)& 0xff);  //DSNU MSB
    corr_bytes[3] = (uint8_t)(dsnu_corr & 0xff);        //DSNU LSB
}

/**
 * \brief  decide whether or not to set a pixel to be replaced
 *
 * Pixel replacement is a last resort because, if used too often, can lead to
 * disturbing smears on the image. 
 *
 * If we already have N replaced pixels in a row and pixel N=1 is
 * uncorrectable, don't replace.
 *
 * If we had a cluster of replaced pixels within a certain distance of another
 * uncorrectable pixel, don't replace.  (Avoid too many clusters too close
 * together.)
 *
 * \author David Poole
 * \date 14-Feb-2007
 *
 */

static void
maybe_replace_pixel( int current_pixel_idx, uint16_t *prnu_corr, uint16_t *dsnu_corr )
{
    int replacement_distance;
    static int last_replaced_pixel_idx;
    static int adjacent_pixel_replaced_cnt;
    static bool in_cluster;
    const CAL_CONFIG *pcal_config=cal_get_config();

    /* XXX move these to somewhere else (set at runtime? datastore?) */
#define MAX_ADJACENT_REPLACED_PIXELS 3
#define MIN_REPLACEMENT_CLUSTER_DISTANCE 50
//#define MAX_ADJACENT_REPLACED_PIXELS 99  // for testing
//#define MIN_REPLACEMENT_CLUSTER_DISTANCE 500 // for testing

    /* XXX this is silly ; need to move these persistent variables onto
     * something passed in the stack 
     */
    if( current_pixel_idx < 0 ) {
        last_replaced_pixel_idx = -MIN_REPLACEMENT_CLUSTER_DISTANCE-1;
        adjacent_pixel_replaced_cnt = 0;
        in_cluster = false;
        return;
    }

//    dbg2("%s enter i=%d p=%d d=%d\n", __FUNCTION__, current_pixel_idx,
//                *prnu_corr, *dsnu_corr );

    /* XXX temp debug */
//    dbg2( "last=%d adj=%d\n", last_replaced_pixel_idx,
//                adjacent_pixel_replaced_cnt );


    replacement_distance = current_pixel_idx - last_replaced_pixel_idx;

//    dbg2( "rd=%d ic=%d\n", replacement_distance, in_cluster );

    if( replacement_distance == 1 ) {
        /* previous pixel was replaced */
        if( adjacent_pixel_replaced_cnt < MAX_ADJACENT_REPLACED_PIXELS ) {
            /* not too many adjacent replaced pixels yet so set to replace */
            *prnu_corr = pcal_config->REPLACE_BAD_PIXEL;
            *dsnu_corr = pcal_config->REPLACE_BAD_PIXEL;

            last_replaced_pixel_idx = current_pixel_idx;
            adjacent_pixel_replaced_cnt += 1;
            g_pixel_recovery_stats.replaced += 1;
        }
        else {
            /* too many adjacent replaced pixels ; set to max gain. */

            /* XXX assuming only uncorrectable PRNU ; how to handle
             * uncorrectable DSNU? 
             */
            *prnu_corr = pcal_config->MAX_DIGITAL_CORRECTION-1;
            *dsnu_corr = 0;

            /* Want to count clusters so we need increment this count only on a
             * transition from a bad cluster to another bad cluster. Since this
             * function is only called on bad pixels, this test gets a little
             * weird.
             *
             * if we're in a new bad pixel cluster we'll assume we
             */
            if( !in_cluster ) {
                g_pixel_recovery_stats.num_replaced_clusters += 1;
                in_cluster = true;
            }
            g_pixel_recovery_stats.max_gains += 1;
        }
    }
    else if( replacement_distance < MIN_REPLACEMENT_CLUSTER_DISTANCE &&
             adjacent_pixel_replaced_cnt >= MAX_ADJACENT_REPLACED_PIXELS ) {
        /* Not enough space between last cluster of bad pixels ; set to max
         * gain. Want clusters of replaced pixels to be a little ways apart.
         */

         /* XXX I'm only assuming uncorrectable PRNU; how to handle
          * uncorrectable DSNU? 
          */
        *prnu_corr = pcal_config->MAX_DIGITAL_CORRECTION-1;
        *dsnu_corr = 0;
        g_pixel_recovery_stats.max_gains += 1;
    }
    else {
        /* set to replace */
        *prnu_corr = pcal_config -> REPLACE_BAD_PIXEL;
        *dsnu_corr = pcal_config -> REPLACE_BAD_PIXEL;
        
        last_replaced_pixel_idx = current_pixel_idx;
        adjacent_pixel_replaced_cnt = 1;

        g_pixel_recovery_stats.replaced += 1;

        in_cluster = false;
    }

//    dbg2("%s leave p=%d d=%d\n", __FUNCTION__, *prnu_corr, *dsnu_corr );
}


/**
 * \brief  push pixel correction into prnu/dsnu hardware lut
 *
 * Does some quick and dirty checks along the way. If there are NN dead
 * pixels in a row, change to use maximum gain instead (avoids smears in the
 * output image). 
 *
 * TODO need to figure out how to handle uncorrectable DSNU. We need to
 * differentiate between a pixel stuck "low" (uncorrectable PRNU) and a pixel
 * stuck "high" (uncorrectable DSNU). Currently we're only handling
 * uncorrectable PRNU.
 *
 * \author David Poole
 * \date 17-Dec-2006
 *
 * 12-Feb-2007 David Poole ; adding gain "window" (if pixel bad but not too bad
 * (e.g., gain < 3.5) then don't even try bad pixel replacement (Bug 3456)
 *
 *
 */

static void
push_correction_into_lut( uint8_t *prnu_dsnu_lut, 
                              int *lut_idx,
                              int pixel_idx,
                              uint16_t prnu_corr, 
                              uint16_t dsnu_corr,
                              uint32_t d_gain_100 )
{
    uint8_t corr_bytes[4]={0,0,0,0};
    const CAL_CONFIG *pcal_config=cal_get_config();

//    dbg2( "%s idx=%d pixel_idx=%d prnu=%d dsnu=%d gain=%d\n", __FUNCTION__, 
//            *lut_idx, pixel_idx, prnu_corr, dsnu_corr, d_gain_100  );

    /* davep 30-Nov-2009 ; turning on bad pixel rep */
    /* XXX move CAL_MODERATELY_BAD_PIXEL_GAIN somewhere else */
#define CAL_MODERATELY_BAD_PIXEL_GAIN 350 /* 3.5 * 100 */

    if( prnu_corr < pcal_config->MAX_DIGITAL_CORRECTION && 
        dsnu_corr < pcal_config->MAX_DIGITAL_CORRECTION ) {
        /* trust the caller and assume correctable (in other words, do nothing) */
    }
    /* the *10 is because d_gain_100 is *100 */
    else if( d_gain_100 >= pcal_config->MAX_DIGITAL_GAIN_TIMES_10*10 &&
             d_gain_100 <= CAL_MODERATELY_BAD_PIXEL_GAIN ) {
        /* set to max gain and hope for the best */
        prnu_corr = pcal_config->MAX_DIGITAL_CORRECTION-1;
//        dsnu_corr = 0;
        g_pixel_recovery_stats.max_gains += 1;
    }
    else {
        /* pixel is uncorrectable so it's a candidate for replacement */
        maybe_replace_pixel( pixel_idx, &prnu_corr, &dsnu_corr ) ;
    }

    /* encode into format for LUT */
    if(!pcal_config->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU) {
        encode_correction_bytes_24bit( prnu_corr, dsnu_corr, corr_bytes );
    }
    else {
        encode_correction_bytes_32bit( prnu_corr, dsnu_corr, corr_bytes );
    }

//    if( pixel_idx<20 ) {
//        dbg2( "%s %d %d %d %02x%02x%02x%02x\n", __FUNCTION__, pixel_idx, prnu_corr,
//                dsnu_corr, corr_bytes[0], corr_bytes[1], corr_bytes[2], corr_bytes[3] );
//    }

    if(!pcal_config->USE_QUAD)
    {
#ifdef HAVE_PIC_PRNUDSNU_BIG_ENDIAN
        if(!cal_get_config()->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
        {
            prnu_dsnu_lut[*lut_idx] = corr_bytes[0];
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[2];
        }
        else
        {
            prnu_dsnu_lut[*lut_idx] = corr_bytes[0];
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[2];
            prnu_dsnu_lut[*lut_idx+3] = corr_bytes[3];
            *lut_idx += 4;
        }
#else
        /* davep 21-Mar-2011 ; XXX shotgun debug; swap the order.
         * I don't know why this works. I don't care why this works. It just works
         * and I'm sick of fiddling with it.
        */
        if(!pcal_config->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
        {
            prnu_dsnu_lut[*lut_idx] = corr_bytes[2];
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[0];

            *lut_idx += 3;
        }
        else 
        {
            prnu_dsnu_lut[*lut_idx] = corr_bytes[3];
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[2];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+3] = corr_bytes[0];
            *lut_idx += 4;
        }
#endif
    }
    else //->USE_QUAD
    {
        if(!pcal_config->PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU)
        {
#ifdef HAVE_PIC_PRNUDSNU_BIG_ENDIAN
            prnu_dsnu_lut[*lut_idx] = 0;// leave quad zero
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[0];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+3] = corr_bytes[2];
#else
            /* davep 21-Mar-2011 ; XXX shotgun debug; swap the order.
             * I don't know why this works. I don't care why this works. It just works
             * and I'm sick of fiddling with it.
             */
            prnu_dsnu_lut[*lut_idx] = corr_bytes[2];
            prnu_dsnu_lut[*lut_idx+1] = corr_bytes[1];
            prnu_dsnu_lut[*lut_idx+2] = corr_bytes[0];
            prnu_dsnu_lut[*lut_idx+3] = 0; //leave quad zero
#endif
        *lut_idx += 4;
        }
    }
}

/**
 * Function name: correct_prnu_dsnu
 *
 * \brief Calculates the PRNU and (optionally) DSNU correction value for each pixel.
 *
 * Description: Calculates PRNU and DSNU correction values simultaneously.  See
 * extensive comments throughout the function for more details.
 *
 * \author Eric Huang, Brad Smith
 *
 * \author davep 16-Dec-2006 ; adding the param headers as I learning the code;
 * more comments in the header. This is a very complex function.
 *
 * \author davep 14-Feb-2007 ; eliminated the ability to only correct PRNU and
 * skip DSNU. We now always do both PRNU and DSNU.  (We weren't using the
 * capability to only correct PRNU anyway.)
 *
 * \param[out] lut  fill with the PRNU/DSNU correction table
 * \param[in] prnu_target
 * \param[in,out] white_data  pixels captured with LEDs turned on
 * \param[in,out] black_data  pixels captured with LEDs turned off
 * \param[out] badPixelLogBuf
 * \param[in] bandsToWatch
 * \param[in] bandDefines
 * \param[out] pixelsInBand
 *
 * white_data and black_data are modified in-place to be able to get an
 * approximation of the data after DSNU but before PRNU. DSNU & PRNU happen
 * inside the ASIC so we can't see them otherwise.
 *
 **/
//#define USE_FLOAT 1 //Eric: Keep this!
static cal_err_t
correct_prnu_dsnu(uint8_t *lut, 
                  int pixels_per_row,
                  uint16_t prnu_target, 
                  uint16_t dsnu_target, 
                  uint16_t *white_data, 
                  uint16_t *gray_data, 
                  uint16_t *black_data, 
                  uint8_t *badPixelLogBuf,
                  int bandsToWatch,
                  uint32_t *bandDefines,
                  int *pixelsInBand
                 )
{
    scan_err_t scerr;
    int lut_idx,i;
    int band_cnt;
//    int lut_idx,i,k;
    uint16_t prnu_corr, dsnu_corr;
    uint32_t d_gain_100;
    int offset;
    uint16_t  bytePos;
    uint8_t   bitPos;
    bool badPRNU = false;
    uint32_t fake_gain_100;
    uint32_t bad_consecutive_pixels_count;
    cal_err_t calerr;
    int fail_CIS;
    bool stop_on_bad_pixels;
    uint32_t max_consecutive_digital_failure_pixels;
    uint32_t num32;
    const struct scanvars *sv;
    uint16_t *dark_pixel_data;
    const CAL_CONFIG *pcal_config = cal_get_config();
#ifdef USE_FLOAT
    float gain_f;
#endif

    dbg2("%s ppr=%ld pt=%d dt=%d\n", __FUNCTION__, 
                pixels_per_row, prnu_target, dsnu_target );

    /* If we fail this many consecutive pixels during digital correction, we'll
     * give up trying to cal.
     *
     * (I pulled 50 out of my ear; it's a nice number that screams "I pulled
     * this number out of my ear")
     *
     * davep 27-May-2008 
     */
    scerr = scands_get_integer_with_default( "cal_max_cons_digital_fail_pixels",
                                        &max_consecutive_digital_failure_pixels, 100 );
    XASSERT( scerr==0, scerr );

    scerr = scands_get_integer_with_default( "cal_stop_on_bad_pixels", &num32, 1 );
//    scerr = scands_get_integer_with_default( "cal_stop_on_bad_pixels", &num32, 0 );
    XASSERT( scerr==0, scerr );
    /* it's a flag */
    stop_on_bad_pixels = num32 && true;

    //Initialize the band log buffer
    if(bandsToWatch) {
        ASSERT(pixelsInBand);
        ASSERT(bandDefines);
        for(band_cnt=0;band_cnt<bandsToWatch; band_cnt++) {
            pixelsInBand[band_cnt]= 0;
        }
    }
    
    //Clear bad pixel log buffer
    ASSERT( badPixelLogBuf != NULL );
    memset(badPixelLogBuf, 0, (pixels_per_row + 7)/8);
    
    lut_idx = 0;

    // Iterate through all the pixels we scanned, correcting for PRNU and, if
    // requested, DSNU.  The "* 3" is because we use 24 total bits or 3 bytes of
    // prnu/dsnu correction for each pixel.  "*ptr" holds the GRAY value for the
    // pixel, and "black_data[i]" holds the black value for the pixel.  Each
    // pixel is a 16-bit value.

    /* davep 14-Feb-2007 ; quiet down the bad pixel messages */
#if 0
#define badpixel_dbg1  dbg1
#define badpixel_dbg2  dbg2
#else
#define badpixel_dbg1(...)  
#define badpixel_dbg2(...)
#endif

    /* davep 27-May-2008 ; if we have too many bad consecutive pixels, consider
     * the cal a failure and raise an error
     */
    bad_consecutive_pixels_count = 0;

    /* XXX this is dumb; get rid of it and put stats on the stack */
    maybe_replace_pixel( -1, NULL, NULL );

    calerr = 0;

    sv = scanvar_get();

    /* davep 18-Feb-2009 ; tinkering with gray calibration */
    if( sv->use_gray_cal ) {
        dbg2( "%s gray calibration\n", __FUNCTION__ );
//        prnu_target /= GRAY_CAL_CONST;
//        prnu_target /= 2;
    }

    dbg2( "%s max_digital_gain_times_10=%d max_digital_correction=%d\n", __FUNCTION__,
                pcal_config->MAX_DIGITAL_GAIN_TIMES_10, 
                pcal_config->MAX_DIGITAL_CORRECTION );

    fail_CIS = 0;

    if( sv->use_gray_cal ) {
        dark_pixel_data = gray_data;
        /* davep 14-Feb-2011 ; hardwire hack dsnu target for gray cal */
        dsnu_target = 31000;
    }
    else {
        dark_pixel_data = black_data;
    }


    for( i=0 ; i<pixels_per_row ; i++)
    {
        // First we do PRNU

        // Calculate the gain required to bring this pixel up to the target. Unfortunately we
        // can't just plug this floating point number into the ASIC, so we have to convert it
        // into a "correction factor" below.
     
        if(white_data[i]<=dark_pixel_data[i]) { //In case there is a bad pixel !
            d_gain_100 = 99*100; // Report a laaaaaaarge gain of 99
        }
        else {
            int target_diff;
            int pixel_diff;

            target_diff = (int)(prnu_target - dsnu_target);
            XASSERT( target_diff > 0, target_diff );

            /* white_data > dark_pixel_data (if check above) */
            pixel_diff = (int)(white_data[i] - dark_pixel_data[i]);

            /* davep 20-Dec-2012 ; original code; changed to use gain*100 to
             * eliminate some floating point calculations
             */
#ifdef USE_FLOAT
            gain_f = (float)(prnu_target - dsnu_target)/(float)(white_data[i] - dark_pixel_data[i]);
#else
            d_gain_100 = ((target_diff*1000) / (pixel_diff*10));
#endif

//            if( i<50 ) {
//                dbg2( "i=%d black=%d white=%d diff=%d d_gain=%d\n", 
//                        i, dark_pixel_data[i], white_data[i], pixel_diff,
//                        d_gain_100 );
//            }
        }

        /* davep 21-Mar-2011 ; handy temp debug */
//        if( i>100 && i<120 ){
//            dbg1( "i=%d d_gain=%d white_data[i]=%d dark_data[i]=%d delta=%d\n", 
//                        i, d_gain_100, 
//                        white_data[i], dark_pixel_data[i], 
//                        white_data[i] - dark_pixel_data[i] );
//        }

#ifndef USE_FLOAT
        //Log the pixels in each gain band
        if( d_gain_100 > 99*100 ) //Need this to make extreme gain fall in one of the >3.0 bands
        {
            badpixel_dbg2(">100 gain=%d\n", d_gain_100 );
            d_gain_100 = 99*100; /* report a large gain of 99 */
        }
#endif
        /* We want the pixel bands to reflect a slightly different gain value.
         * We were getting complaints that the calgraph's math didn't match. The
         * calgraph reports gain, target, and white_pixel but the gain is
         * calculated slightly differently. Report the "intuitive" gain for the
         * calgraphs.
         */
        fake_gain_100 = (prnu_target*1000)/(white_data[i]*10);

        /* linear search for this "gain" value in the bands we're supposed to
         * record 
         */
        for(band_cnt=0;band_cnt<bandsToWatch; band_cnt++) {
            /* if this "gain" is within this band... */
            if(fake_gain_100>bandDefines[band_cnt*2] && fake_gain_100<=bandDefines[band_cnt*2+1]) {
                /* add to the total */
                pixelsInBand[band_cnt] += 1;
            }
        }

        /* davep 17-Nov-2009 ; XXX temp debug */
//        if( i<50 ) {
//            dbg1( "white=%d black=%d d_gain=%d band0=%d band1=%d band2=%d\n", 
//                        white_data[i], black_data[i],
//                        d_gain_100,
//                        pixelsInBand[0], pixelsInBand[1], pixelsInBand[2] );
//        }

        /* davep 20-Dec-2012 ; original code; changed to use gain*100 to
         * eliminate some floating point calculations
         */
#ifdef USE_FLOAT  //Eric use float
        prnu_corr = (uint16_t)((gain_f - 1.0f) * (1<<(pcal_config->DIGITAL_GAIN_PRECISION))) ;
#else
        prnu_corr = (uint16_t)(((d_gain_100 - 100) *
                              (1<<(pcal_config->DIGITAL_GAIN_PRECISION)))/100) ;
#endif

        //We no longer compare the prnu_corr to find the bad pixel, we check the original gain
        //  to avoid any rounding-error, and by doing this, we can make sure the pixel count
        //  in a >=3.0 band will match the bad pixels count
        if(prnu_corr >= pcal_config->MAX_DIGITAL_CORRECTION) {
            badpixel_dbg2( "prnu_corr %d; clipped to %d\n", prnu_corr, pcal_config->MAX_DIGITAL_CORRECTION );
            prnu_corr=pcal_config->MAX_DIGITAL_CORRECTION;
        }
           
        badPRNU = false;
#ifdef USE_FLOAT
        /* the *10 is because d_gain_100 is *100 */
        if( (uint16_t)(gain_f*100) <= pcal_config->MAX_DIGITAL_GAIN_TIMES_10*10 ) {
            /* good gain! we can correct this pixel */
            bad_consecutive_pixels_count = 0;
        }
#else
        /* the *10 is because d_gain_100 is *100 */
		if( d_gain_100 <= pcal_config->MAX_DIGITAL_GAIN_TIMES_10*10 ) {
			/* good gain! we can correct this pixel */
//      dbg1("[BAD PIXEL] Pixel# = %d Value = %d\n", i,white_data[i] );//for test

			bad_consecutive_pixels_count = 0;
		}
#endif
        else {
            // We can't correct this pixel. Setting both the prnu and dsnu correction values
            // to the max value tells the ASIC that this pixel is uncorrectable. If "bad pixel
            // correction" is enabled in the ASIC, this pixel's value will be replaced with
            // that of the previous pixel (or with white if it's the first pixel).
            prnu_corr = pcal_config->MAX_DIGITAL_CORRECTION;
            if(!((pcal_config->BYPASS_REPLACE_BAD_PIXEL)))
                dsnu_corr = pcal_config->MAX_DIGITAL_CORRECTION;

              //badpixel_dbg1("%d uncorrectable PRNU\n", i );
            dbg1("[BAD PIXEL] Pixel# = %d Value = %d\n", i,white_data[i] );              
            
            bad_consecutive_pixels_count += 1;

            /* davep 27-May-2008 ; if we have too many bad pixels in a row, consider
             * the calibration a failure  (only do this if scands value requests a stop,
             * since manufacturing people will want the full details)
             */
            if (bad_consecutive_pixels_count > max_consecutive_digital_failure_pixels ) {
                fail_CIS = bad_consecutive_pixels_count;  // keep track of failure
                if (stop_on_bad_pixels) {
                    // if we want, stop looking for more bad pixels now
                    calerr = CALERR_PRNU_TOO_MANY_UNGAINABLE_PIXELS;
                }
            }
            if(badPixelLogBuf)
            {   //Log them into   
                bytePos = (i)/8;
                bitPos = (i)%8;
                badPixelLogBuf[bytePos] = badPixelLogBuf[bytePos]|(0x01<<bitPos);
            }
            badPRNU = true;

            g_pixel_recovery_stats.bad_prnu += 1;
        }

        // Correct for DSNU.
        //
        // Compute the difference between the pixel's value and what we want its
        // value to be. DSNU correction is a subtractive operation, so hopefully
        // the pixel's value is greater than the target.
#ifdef USE_FLOAT  //Eric H try with float gain
        offset = black_data[i] - (uint16_t)(((float)dsnu_target)/gain_f);
#else
        offset = black_data[i] - (( ((dsnu_target*100)/d_gain_100))/100);
#endif
        if (offset < 0) {
            offset = 0;
        }

        // The ASIC will convert this to a 16-bit number internally, so we need
        // to divide by the proper amount to make the number correct when the
        // ASIC multiplies it. Currently we're using 12 bits for the DSNU
        // correction value, so we need to right-shift by 4 bits.
        dsnu_corr = offset >> (pcal_config->DSNU_SHIFT);

        /* 
         *   CAL_MAX_DIGITAL_CORRECTION is based on 12bit/16bit 
         */
        if (dsnu_corr >= pcal_config->MAX_DIGITAL_CORRECTION)
        {
            // We can't correct this pixel. Setting both the prnu and dsnu correction values
            // to the max value tells the ASIC that this pixel is uncorrectable. If "bad pixel
            // correction" is enabled in the ASIC, this pixel's value will be replaced with
            // that of the previous pixel (or with white if it's the first pixel).
            badpixel_dbg1("%d uncorrectable DSNU!\n", i );

            g_pixel_recovery_stats.bad_dsnu += 1;

            if(!pcal_config->BYPASS_REPLACE_BAD_PIXEL) {
                prnu_corr = pcal_config->MAX_DIGITAL_CORRECTION; 
            }
                
            if(badPixelLogBuf)
            {   //Log them into     
                 bytePos = (i)/8;
                 bitPos = (i)%8;
                 badPixelLogBuf[bytePos] = badPixelLogBuf[bytePos]|(0x01<<bitPos);
            }
        }
        else if(badPRNU && !((pcal_config->BYPASS_REPLACE_BAD_PIXEL)))
        {
            /* set pixel to be replaced */
            /* davep 14-Feb-2007 ; I'm finding good results with bad PRNU
             * recovery if I don't disturb the calculated DSNU
             */
//            dsnu_corr = CAL_MAX_DIGITAL_CORRECTION;
        }
        
        offset= dsnu_corr<<pcal_config->DSNU_SHIFT;
           
        /* run our previously scanned data through a phony PRNU/DSNU correction
         * so we can make calgraphs that show what would happen with our
         * calculated correction values
         */
        white_data[i]= white_data[i] - offset; //Get the data back as before PRNU;
        black_data[i]= black_data[i] - offset; //Get black data back as after DSNU
#ifdef USE_FLOAT
        push_correction_into_lut( lut, &lut_idx, i, prnu_corr, dsnu_corr, (uint16_t)(gain_f*100));
#else
        push_correction_into_lut( lut, &lut_idx, i, prnu_corr, dsnu_corr, d_gain_100 );
#endif

        /* davep 21-Mar-2011 ; handy temp debug */
//        if( i<20 ){
//            dbg2( "i=%d gain=%d offset=%d p=%d d=%d\n",  
//                        i, FLOAT_TO_DECIMAL(gain), offset, prnu_corr, dsnu_corr );
//        }

        /* davep 27-May-2008 ; if something went wrong up there, consider the
         * calibration a failure and leave
         */
        if( calerr != 0 ) {
            break;
        }

    } /* end for( i=0 ; i<pixels_per_row ; i++) */

    dbg2("prnu_dsnu_correction: band1=%d band2=%d band3=%d\n",
                pixelsInBand[0], pixelsInBand[1], pixelsInBand[2]);
    if (fail_CIS > 0)
    {
        // we don't set calerr until here so customer can see all failed pixels on serial port
        calerr = CALERR_PRNU_TOO_MANY_UNGAINABLE_PIXELS;


#if BAD_PIXEL_DUMP //dump_test
		struct file *filp1; 
		char *file_name;  

		dbg1("[CAL_DUMP] sensor_num_cal = %d \n",sensor_num_cal);			
//		file_name = "/data/bad_pixel_dump.raw";
		if(sv->dpi == 300){
			if(sv->cmode == SCAN_CMODE_MONO){
				if(sensor_num_cal){//300,mono,back
					file_name = "/data/BP_300M_BACK.raw";
				}
				else{//300,mono,front			    	
					file_name = "/data/BP_300M_FRONT.raw";
				}
			}
			else{
				if(sensor_num_cal){//300,color,back
					color_cnt++;
					if(color_cnt==1){
						file_name = "/data/BP_300CR_BACK.raw";
					}
					if(color_cnt==2){
						file_name = "/data/BP_300CG_BACK.raw";
					}
					if(color_cnt==3){
						file_name = "/data/BP_300CB_BACK.raw";
					}					
				}
				else{//300,color,front
					color_cnt++;
					if(color_cnt==1){
						file_name = "/data/BP_300CR_FRONT.raw";
					}
					if(color_cnt==2){
						file_name = "/data/BP_300CG_FRONT.raw";
					}
					if(color_cnt==3){
						file_name = "/data/BP_300CB_FRONT.raw";
					}
				}
			}
		}
		else
		{
			if(sv->cmode == SCAN_CMODE_MONO){
				if(sensor_num_cal){//600,mono,back
					file_name = "/data/BP_600M_BACK.raw";
				}
				else{//600,mono,front
					file_name = "/data/BP_600M_FRONT.raw";	
				}
			}
			else{
				if(sensor_num_cal){//600,color,back
					color_cnt++;
					if(color_cnt==1){
						file_name = "/data/BP_600CR_BACK.raw";
					}
					if(color_cnt==2){
						file_name = "/data/BP_600CG_BACK.raw";
					}
					if(color_cnt==3){
						file_name = "/data/BP_600CB_BACK.raw";
					}			
				}
				else{//600,color,front
					color_cnt++;
					if(color_cnt==1){
						file_name = "/data/BP_600CR_FRONT.raw";
					}
					if(color_cnt==2){
						file_name = "/data/BP_600CG_FRONT.raw";
					}
					if(color_cnt==3){
						file_name = "/data/BP_600CB_FRONT.raw";
					}			
				}
			}			
		}	
				
		dbg1("[CAL_DUMP] CAL_DUMP = %s \n",file_name);	
		
		/* kernel memory access setting */
		mm_segment_t old_fs = get_fs();
		
		set_fs(get_ds());
	 
		/* open a file */
		filp1 = filp_open(file_name, O_CREAT|O_RDWR, 0);

		if (IS_ERR(filp1)) {
			dbg1("[CAL_DUMP] Error : Open sortdump Fail!!\n");
		}
		else {
			dbg1("[CAL_DUMP] Success : Open sortdump Ok!!\n");

			/* write example */
			vfs_write(filp1, white_data, sizeof(uint16_t)*pixels_per_row, &filp1->f_pos);
		}
		
		//dbg1("[KMG_DUMP] num_rows = %d  pixels_per_row = %d \n",num_rows,pixels_per_row);

		filp_close(filp1, NULL);
		
		/* restore kernel memory setting */
	   set_fs(old_fs);		
#endif //dump_test

		if(sensor_num_cal == 0)
		{
			dbg1("%s too many consecutive bad pixels - %d - Front CIS Failure!\n",
				__FUNCTION__,fail_CIS);					
        }
        else
        {
			dbg1("%s too many consecutive bad pixels - %d - Back CIS Failure!\n",
				__FUNCTION__,fail_CIS);					        	
        }

    }
                        
    return calerr;
}
#ifdef HAVE_PIC_PC_QUAD
/*
   Quatric calibration
*/
static cal_err_t  cal_quad(CalibrationCase *pCCase,  cal_cache_t *cached_cal,  uint8_t *luts[3], uint16_t *gray_datas[3])
{
    int i;
    int num_colors;
    //uint16_t *gray_datas[3]={0,0,0};
    cal_err_t calerr;
    #define MAX_QPRIME 127
    uint32_t q_sum;
    uint16_t q_cal_target;
    float q_m, q_M, q_k, q_Q;
    int32_t q_SF1[3], q_shift;
    uint16_t *q_Ms;
    int32_t *q_Qs[3];
    int32_t q_Q_max, q_Q_min;
    int32_t SF1_a, SF1_b;
    int k;
    uint8_t *prnu_dsnu_lut;
    uint8_t qcoef;

//    dbg1("QUAD PDCR1=0x%08x, PDCR2=0x%08x\n" ,*PIC_PC_PDCR1, *PIC_PC_PDCR2);

    num_colors = 3;
    if(pCCase->cmode==SCAN_CMODE_MONO) {
        num_colors = 1;
    }

    /*The quadratic correction should not have been set here*/
    ASSERT(cached_cal->quad_SF1 == 0);

    /* Collect gray scan data, must turn DSNU/PRNU */
    cal_debug_scan_setup_pdlut( cached_cal, pCCase->cmode );

#define GRAY_EXP_FACTOR 5

    cal_send_scan_id(40);

    calerr = get_data_and_average( pCCase, num_colors, GRAY_EXP_FACTOR, gray_datas );
    if(calerr!=0)
    {
        return calerr;
    }

    q_k = 65536;

    /*Initial value for q_shift*/
    q_shift = 0;
    for(k = 0;k < num_colors; k++)
    {

        q_Ms = gray_datas[k];

        q_Qs[k] = cal_get_memory(pCCase->pixels_to_scan*sizeof(int32_t));

        ASSERT( q_Qs[k]);

        q_sum = 0;

        for(i = 0; i < pCCase->pixels_to_scan; i++)
        {
            q_sum += q_Ms[i];
        }

        q_cal_target = q_sum/pCCase->pixels_to_scan;

        dbg2("quad: gray taret is %d\n", q_cal_target);

        q_m = ((float)q_cal_target)/q_k;

        q_Q_max = 0;
        q_Q_min = 0;

        for(i = 0;i < pCCase->pixels_to_scan; i++)
        {
            q_M = ((float)(q_Ms[i]))/q_k;

            q_Q =(q_m - q_M)/(q_M*q_M - q_M);
     
            //convert to INT and save it
            q_Qs[k][i] = (int32_t)( (q_Q*q_k));  //S.16 for Q

            if(q_Qs[k][i] > q_Q_max) q_Q_max = q_Qs[k][i];
            if(q_Qs[k][i] < q_Q_min) q_Q_min = q_Qs[k][i];

        }

        //Take absolute values
        if(q_Q_max < 0)q_Q_max = -q_Q_max;
        if(q_Q_min < 0)q_Q_min = -q_Q_min;

        /*
         * the 1.1 factor seem to be used for adding some room in SF1
         */
        SF1_a = (int32_t)( 0.999 + 1.1* ((float)q_Q_max)/((float)MAX_QPRIME));
        SF1_b = (int32_t)( 0.999 + 1.1* ((float)q_Q_min)/((float)MAX_QPRIME));

        if(SF1_a > SF1_b)
           q_SF1[k] = SF1_a;
        else
           q_SF1[k] = SF1_b;

        dbg1("SF1=%d (%d %d %d %d)\n", q_SF1[k], SF1_a, SF1_b, q_Q_max, q_Q_min);

        /*Need to find out q_shift factor, which is used to scale down the range of q_SF1 to
         * the range of [-MAX_QPRIME, MAX_QPRIME] if it is not in it already
         */

        /* For now, require q_SF1[k] always in range*/
#if 0
        ASSERT(q_SF1[k]<= MAX_QPRIME);
#endif
        if(q_SF1[k] > MAX_QPRIME)
        {
            while( (q_SF1[k]>>q_shift) > MAX_QPRIME)
            {
                q_shift++;
            }
        }

        dbg2("SF1=%d q_shift=%d\n", q_SF1[k], q_shift);
    }
    /* The q_shift now has the maximum shift factor needed for all colors*/
    cached_cal->quad_shift = q_shift;

    /*Need to find the maximum SF1 from all colors*/
    cached_cal->quad_SF1 = q_SF1[0];

    for(k = 0;k < num_colors; k++)
    {
        if(cached_cal->quad_SF1 < q_SF1[k])
            cached_cal->quad_SF1 = q_SF1[k];
    }

    /*Adjust for the shifting*/
    cached_cal->quad_SF1 = cached_cal->quad_SF1 >> q_shift;

    ASSERT( cached_cal->quad_SF1 <= MAX_QPRIME);

    ASSERT(cached_cal->quad_SF1 >= 0);

    /*For now, use a default to do correction all pixels*/
    cached_cal->quad_SF2 = 255;

    for(k = 0;k < num_colors; k++)
    {
        /*Load the Q to up 8bits of LUT
         *  ToDo: 4bits ?
         */
        prnu_dsnu_lut = luts[k];

        /*
         * At the moment, we always use 8 bits located at bit [31:24] for quadratic
         * correction.
         *
         * ToDo: Use any 8/4 bits based on PDCR1.CorrBits and PDCR2.CoeffWidth
         */
        for(i = 0; i< pCCase->pixels_to_scan;i++)
        {
            /*  From spec, the qcoef goes into LUT must be in signed magnitude notatin, Not signed int
            */
            if(q_Qs[k][i]>=0)
                qcoef = (q_Qs[k][i]/cached_cal->quad_SF1) &0xFF;
            else
                qcoef = 0x80 + (((0-q_Qs[k][i])/cached_cal->quad_SF1) &0xFF);

    #ifdef HAVE_PIC_PRNUDSNU_BIG_ENDIAN
            prnu_dsnu_lut[i*4] =  qcoef;
    #else
            prnu_dsnu_lut[i*4+3] = qcoef;
    #endif
        }

        PTR_FREE(q_Qs[k]);
    }

    dbg1("Done with quadratic correction. SF1=%d SF2=%d\n", cached_cal->quad_SF1, cached_cal->quad_SF2);

    return 0;
}
#endif

/**
 * Function name: cal_calc_digital_correction
 *
 * \brief Digital calibration on WHITE, scans the white calibration label and 
 * calculates PRNU and DSNU compensation.
 * Description: Performs a scan of the white calibration label. Computes the
 * PRNU and DSNU correction for each pixel using an average of each pixel's value across
 * several lines (to account for dust on the label).
 *
 * \author Eric Huang
 * \author David Poole
 *
 */

cal_err_t cal_calc_digital_correction(CalibrationCase *pCCase )
{
    uint16_t *black_datas[3]={0,0,0}; //[mono/red, green, blue]
    uint16_t *white_datas[3]={0,0,0};
    uint16_t *gray_datas[3]={0,0,0};
    int i;
    int num_colors;
    uint8_t *luts[3];
    int total_pixels_replaced;
    cal_cache_t *cached_cal;
    cal_err_t calerr;
    int bytes_needed;
    const struct scanvars *sv;

    dbg2( "%s\n", __FUNCTION__ );

    sv = scanvar_get();

    calerr = CALERR_NONE;

    //Get black data, we don't use the average data here
    pic_prnudsnu_set_bypass_all(true);
    
    num_colors = 3;
    if(pCCase->cmode==SCAN_CMODE_MONO) {
        num_colors = 1;
    }

    /* We need to capture a dark scan (no LEDs) but we'll need to keep the data
     * around to compare it to a light scan (with LEDs). Allocate buffers for
     * the the pixel data. 
     *
     * We store pixels that are approximated to mimic the pixel values after
     * PRNU has had its way with them. Used for cal graphs.
     */
    bytes_needed = pCCase->pixels_to_scan * CAL_BYTES_PER_PIXEL;

    for( i=0 ; i<num_colors ; i++ ) {
        black_datas[i] = (uint16_t *)cal_get_memory( bytes_needed );
        if( !black_datas[i] ) {
            dbg1("%s failed to get black buffer idx=%d bytes=%d\n",
                        __FUNCTION__, i, bytes_needed );
            calerr = CALERR_OUT_OF_MEMORY;
            goto fail;
        }
        white_datas[i] = (uint16_t *)cal_get_memory( bytes_needed );
        if( !white_datas[i] ) {
            dbg1("%s failed to get white buffer idx=%d bytes=%d\n",
                        __FUNCTION__, i, bytes_needed );
            calerr = CALERR_OUT_OF_MEMORY;
            goto fail;
        }
        gray_datas[i] = (uint16_t *)cal_get_memory( bytes_needed );
        if( !gray_datas[i] ) {
            dbg1("%s failed to get gray buffer idx=%d bytes=%d\n",
                        __FUNCTION__, i, bytes_needed );
            calerr = CALERR_OUT_OF_MEMORY;
            goto fail;
        }
        memset( black_datas[i], 0, bytes_needed );
        memset( white_datas[i], 0, bytes_needed );
        memset( gray_datas[i], 0, bytes_needed );
    }

    cal_send_uint32_metadata( 4,
            CAL_METADATA_CAL_CAP, CAL_CAP_DSNU,
            CAL_METADATA_RED_ENABLED, 0, 
            CAL_METADATA_GREEN_ENABLED, 0, 
            CAL_METADATA_BLUE_ENABLED, 0 );

    //Get black data (LEDs off) and its average
    scan_enable_motor_motion(false);
    calerr = get_data_and_average( pCCase, num_colors, 0, black_datas );

    if( calerr != 0 ) {
        /* get_data_and_average() logs error message */
        scanlib_send_sc(SMSG_SC_BLACK_STRIP_READ_FAIL);    
        goto fail;
    }

    cal_send_uint32_metadata( 4,
            CAL_METADATA_CAL_CAP, CAL_CAP_PRNU,
            CAL_METADATA_RED_ENABLED, 1, 
            CAL_METADATA_GREEN_ENABLED, 1, 
            CAL_METADATA_BLUE_ENABLED, 1 );

    //Get white data (LEDs on) and its average
    scan_enable_motor_motion(true);
    calerr = get_data_and_average( pCCase, num_colors, 10, white_datas );

    if( calerr != 0 ) {
        /* get_data_and_average() logs error message */
		scanlib_send_sc(SMSG_SC_WHITE_STRIP_READ_FAIL);    
        goto fail;
    }

    cal_send_uint32_metadata( 4,
            CAL_METADATA_CAL_CAP, CAL_CAP_PRNU,
            CAL_METADATA_RED_ENABLED, 1, 
            CAL_METADATA_GREEN_ENABLED, 1, 
            CAL_METADATA_BLUE_ENABLED, 1 );

    //Get gray data (LEDs sorta on) and its average
    if( sv->use_gray_cal ) {
        calerr = get_data_and_average( pCCase, num_colors, 5, gray_datas );
        if( calerr != 0 ) {
            /* get_data_and_average() logs error message */
            goto fail;
        }
    }

    /* davep 18-Nov-2009 ; not sure if this will help or not but send the
     * pixel averages downstream, too.
     */
    if (pCCase->cmode==SCAN_CMODE_MONO) {
        calerr = cal_send_pixel_array( CAL_METADATA_PRNU_WHITE_AVG, 
                                        SCAN_DATA_TYPE_MONO,
                                        pCCase->pixels_to_scan, white_datas[0] );
        if( calerr != 0 ) {
            goto fail;
        }

        calerr = cal_send_pixel_array( CAL_METADATA_PRNU_BLACK_AVG, 
                                        SCAN_DATA_TYPE_MONO,
                                        pCCase->pixels_to_scan, black_datas[0] );
        if( calerr != 0 ) {
            goto fail;
        }
    }
    else {
        for (i = 0; i<3; i++) {
            calerr = cal_send_pixel_array( CAL_METADATA_PRNU_WHITE_AVG, 
                                           SCAN_DATA_TYPE_RED+i,
                                           pCCase->pixels_to_scan, white_datas[i] );
            if ( calerr != 0 ) {
                goto fail;
            }

            calerr = cal_send_pixel_array( CAL_METADATA_PRNU_BLACK_AVG, 
                                           SCAN_DATA_TYPE_RED+i,
                                           pCCase->pixels_to_scan, black_datas[i] );
            if ( calerr != 0 ) {
                goto fail;
            }
        }
    }

    cached_cal = getCalCache(pCCase);
#ifdef HAVE_PIC_PC_QUAD
    /*Make sure the quad_SF1 is invalid*/
    cached_cal->quad_SF1 = 0;
#endif

    if(pCCase->cmode==SCAN_CMODE_MONO) {
        ASSERT( cached_cal->prnudsnu_lut != NULL );
        luts[0] = cached_cal->prnudsnu_lut;
    }
    else {
        ASSERT( cached_cal->pc_lut_ptr_red != NULL );
        ASSERT( cached_cal->pc_lut_ptr_green != NULL );
        ASSERT( cached_cal->pc_lut_ptr_blue != NULL );
        luts[0] = cached_cal->pc_lut_ptr_red;
        luts[1] = cached_cal->pc_lut_ptr_green;
        luts[2] = cached_cal->pc_lut_ptr_blue;
    }

    /* assuming red index == mono index in the code below */
    XASSERT( SV_COLOR_RED==SV_COLOR_MONO, SV_COLOR_RED );

    total_pixels_replaced = 0;
    
    for( i=0; i<num_colors ; i++) {
        
        /* davep 14-Feb-2007 ; start with clean pixel recovery statistics */
        memset( &g_pixel_recovery_stats, 0, sizeof(g_pixel_recovery_stats));

        XASSERT( pCCase->pixels_uncorrectable[i] != NULL,  i );

        calerr = correct_prnu_dsnu( luts[i], 
                               pCCase->pixels_to_scan, 
                               sv->cal_white_digital_target[i - SV_COLOR_RED],
                               sv->cal_black_digital_target[i - SV_COLOR_RED],
                               white_datas[i],   
                               gray_datas[i], 
                               black_datas[i], 
                               pCCase->pixels_uncorrectable[i], //Buffer for bad pixel log
                               3, //3 gain bands to watch
                               pCCase->bandsDefined, // definitions of the gain bands
                               pCCase->pixelsInBand[i] //buf to store the pixel count in bands
                             );

        if( calerr != 0 ) {
            dbg2( "%s correct_prnu_dsnu failed with calerr=%d\n", __FUNCTION__, calerr );
            goto fail;
        }

        /* capture total pixels replaced so we don't have to call
         * reconcile_pixel_replacements (see below) unless we absolutely have
         * to
         */
        total_pixels_replaced += g_pixel_recovery_stats.replaced;

        dbg2( "color=%d bad_prnu=%d bad_dsnu=%d max_gains=%d replaced=%d num_clusters=%d\n",
                    i,
                    g_pixel_recovery_stats.bad_prnu,
                    g_pixel_recovery_stats.bad_dsnu,
                    g_pixel_recovery_stats.max_gains,
                    g_pixel_recovery_stats.replaced,
                    g_pixel_recovery_stats.num_replaced_clusters );
    }
	color_cnt = 0;
    /* davep 06-Oct-2009 ; send the pixel band calculations */
    cal_send_pixel_band_counts( pCCase );

    /* davep 12-Feb-2007 ; if a pixel in any color plane is set to replace,
     * make sure the same pixel in the other color planes are also set to
     * replace.
     *
     * XXX this is *extremely* slow and would better be done at the moment
     * we're calculating prnu/dsnu. But that would require rewriting
     * correct_prnu_dsnu
     */
//    if( num_colors>1 && !(pcal_config->BYPASS_REPLACE_BAD_PIXEL) && total_pixels_replaced > 0 ) {
//        reconcile_pixel_replacements( num_colors, luts );
//    }

    /* sanity check ; make sure we're obeying the bad pixel replacement bypass */
    /* davep 16-Feb-2007 ; not yet; have run out of time */
//    if( (pcal_config->BYPASS_REPLACE_BAD_PIXEL) ) {
//        XASSERT( total_pixels_replaced==0, total_pixels_replaced );
//    }

#ifdef HAVE_PIC_PC_QUAD
    if(cal_get_config()->USE_QUAD)
    {
        calerr = cal_quad(pCCase, cached_cal, luts, gray_datas);
        if(calerr)
        {
            scanlog(0, "%s Quadratic calibration failed %d\n", __FUNCTION__, calerr );
            goto fail;
        }   
    }
#endif   

    for( i = 0 ; i < 3 ; i++ ) {
       if( black_datas[i] ) {
           PTR_FREE( black_datas[i] );
       }
       if( white_datas[i] ) {
           PTR_FREE( white_datas[i] );
       }
       if( gray_datas[i] ) {
           PTR_FREE( gray_datas[i] );
       }
    }

    /* success! */
    return 0;

fail:
    /* shouldn't get here unless we can report an error */
    ASSERT( calerr != 0 );

    dbg1("%s failed calerr=%d\n", __FUNCTION__, calerr );
    for( i=0 ; i<3 ; i++ ) {
        if( black_datas[i] ) {
            PTR_FREE( black_datas[i] );
        }
        if( white_datas[i] ) {
            PTR_FREE( white_datas[i] );
        }
        if( gray_datas[i] ) {
            PTR_FREE( gray_datas[i] );
        }
    }
    return calerr;
}

