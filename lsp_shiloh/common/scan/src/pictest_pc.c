/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 * davep 06-Mar-2008; Moved PRNU/DSNU tests from pictest.c to own file.
 *
 *
 */

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"
#include "list.h"
#include "cpu_api.h"
#include "regAddrs.h"

#include "scantypes.h"
#include "scanlog.h"
#include "scancore.h"
#include "scandbg.h"
#include "scantools.h"
#include "safetylock.h"
#include "icedma.h"
#include "pichw.h"
#include "pic.h"
#include "piccbi.h"
#include "pictest.h"
#include "pictest_pc.h"
#include "scantask.h"
#include "ostools.h"
#include "lehmer.h"

#ifdef HAVE_ICE_PRNUDSNU_MUL8
/* davep 04-Apr-2008 ; the recent ASIC usurped value 3 to indicate 8x
 * gain. 
 * 
 * This is so confusing...
 *
 */
int scale_prnu_shift[4] = { -2, -1, 0, -3 };
#else
int scale_prnu_shift[4] = { -2, -1, 0, +1 };
#endif

struct pixel_stats {
    uint32_t num_tested;
    uint32_t num_duplicates;
    uint32_t num_zero;
    uint32_t num_saturated;
};

static struct pixel_stats pixel_stats;

static void print_pixel_stats( struct pixel_stats *ps )
{
    dbg1( "pixels tested=%d duplicate=%d zero=%d saturated=%d\n", 
                ps->num_tested, ps->num_duplicates, ps->num_zero, ps->num_saturated );
}

/**
 * \brief peek into a list of icebufs and get the first output pixel
 *
 * Handles both 8/16-bit pixels
 *
 * Originally written to capture the output of a single pixel value input to a
 * PRNU/DSNU test. (Send in a buffer of the same pixel value and peek at the
 * output pixel value.)
 *
 * \author David Poole
 * \date 26-Feb-2008
 *
 */
#if 0
static void
get_first_pixel_in_list( ATLISTENTRY *list, int pixel_size, uint16_t *pixel ) 
{
    ATLISTENTRY *entry;
    struct ice_dma_buffer *icebuf;

    entry = list->nextEntry;
    icebuf = CONTAINING_RECORD( entry, struct ice_dma_buffer, listnode );

    *pixel = 0;

    if( pixel_size==8 ) {
        *pixel = icebuf->data[0];
    }
    else {
        /* 16-bit is a little more complex */
        *pixel = *(uint16_t*)(icebuf->data);
    }
}
#endif

static void
prnudsnu_feeder_msg_encode( scan_msg_t *msg, uint16_t pixel_in, 
                            struct pic_dma_test *icetest )
{
    memset( msg, 0, sizeof(scan_msg_t) );

    /* param1 not yet used */
    msg->param1 = 0;

    /* send along the pixel value we want the feeder thread to send */
    msg->param2 = (uint32_t)pixel_in;

    icetest->cookie = ICETEST_COOKIE;
    msg->param3 = (void *)icetest;
}

static void
prnudsnu_feeder_msg_decode( scan_msg_t *msg, uint16_t *pixel_in, 
                            struct pic_dma_test **icetest )
{
    /* param1 not yet used */

    *pixel_in = msg->param2;

    *icetest = (struct pic_dma_test *)msg->param3;
    XASSERT( (*icetest)->cookie==ICETEST_COOKIE, (*icetest)->cookie );
}

static void
print_prnudsnu_config( struct pic_prnudsnu_config *prnudsnu_config )
{
    dbg1( "total_bits=%d extra_prnu_bits=%d,%d,%d scale_prnu_color=%d,%d,%d scale_dsnu_color=%d,%d,%d\n", 
                prnudsnu_config->total_bits,
                prnudsnu_config->extra_prnu_bits[0],
                prnudsnu_config->extra_prnu_bits[1],
                prnudsnu_config->extra_prnu_bits[2],

                prnudsnu_config->scale_prnu_color[0],
                prnudsnu_config->scale_prnu_color[1],
                prnudsnu_config->scale_prnu_color[2],

                prnudsnu_config->scale_dsnu_color[0],
                prnudsnu_config->scale_dsnu_color[1],
                prnudsnu_config->scale_dsnu_color[2]
         );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 07-Mar-2011
 *
 */

static uint32_t make_corr( uint16_t lutval, uint32_t mask, uint32_t mult, uint32_t scale, uint32_t offset )
{
    uint32_t corr;

    corr = (lutval * mult) >> scale;

    /* clamp to 'n' bits */
    if( corr & ~mask ) {
        corr = mask;
    }

    corr += offset;

    /* again with the clamping to 'n bits */
    if( corr & ~mask ) {
        corr = mask;
    }

    return corr;
}

static uint16_t quad_predictor( uint16_t pixel_in, uint8_t quad_coeff, uint32_t SF1, uint32_t SF2 )
{
    uint32_t pixel_out;
    unsigned int quad;
    unsigned int quad_neg = 0; 
    unsigned int limit;
    unsigned int quad0_neg;
    unsigned int quad0;
    int tmp;
    unsigned int quad1_neg;
    unsigned int quad1;
    unsigned int quad2;
    unsigned int quad2_neg;

//    dbg2( "%s in=%d SF1=%d SF2=%d quad_coeff=%d\n", __FUNCTION__, pixel_in, SF1, SF2, quad_coeff );

    pixel_out = pixel_in;

    /* if high bit is set, treat as negative */
    if( quad_coeff & 0x80 ) {
        quad_coeff &= ~0x80;
        quad_neg = 1;
    }
    quad = quad_coeff * SF1;

    limit = ((pixel_out >> 4)*SF2) >> 7;
    if (limit > 256) {
        limit = 256;
    }

    //            printf("WORD0:  quad=%d, quad_neg=%d, limit=%d\n", quad, quad_neg, limit);
    
    quad = (quad*limit) >> 8;

    //            printf("WORD1:  quad=%d\n", quad);
    
    // (1-Q)
    tmp = quad_neg ? (0x1<<16) + quad : (0x1<<16) - quad;
    
    if (tmp < 0) {
        quad0 = (unsigned int) -(tmp);
        quad0_neg = 1;
    }
    else {
        quad0 = (unsigned int) tmp;
        quad0_neg = 0;
    }
    quad0 = quad0 >> 2;
    
    //            printf("WORD2:  quad0=%d\n", quad0);
    
    // (Q*x)
    quad1_neg = quad_neg;
    quad1 = quad * pixel_out;
    quad1 = quad1 >> 18;

    //            printf("WORD3:  quad1=%d\n", quad1);

    // -----

    // Q*x + (1-Q)
    if (quad0_neg && quad1_neg) {
        quad2 = quad0 + quad1;
        quad2_neg = 1;
    }
    else if (quad0_neg) {
        tmp = quad1 - quad0;
        if (tmp < 0) {
            quad2 = (unsigned int) -(tmp);
            quad2_neg = 1;
        }
        else {
            quad2 = (unsigned int) tmp;
            quad2_neg = 0;
        }
    }
    else if (quad1_neg) {
        tmp = quad0 - quad1;
        if (tmp < 0) {
            quad2 = (unsigned int) -(tmp);
            quad2_neg = 1;
        }
        else {
            quad2 = (unsigned int) tmp;
            quad2_neg = 0;
        }
    }
    else {
        quad2 = quad0 + quad1;
        quad2_neg = 0;
    }

    // check...quad better be positive at this point
//    if (quad2_neg) {
//        printf("%s: ERROR:  negative quad2 value is not valid\n", args->prog_name);
//    }
    XASSERT( !quad2_neg, quad2_neg );

    //            printf("WORD4:  quad2=%d\n", quad2);
    
    // (Q*x + (1-Q)) * x
    //            printf("WORD5:  pixel_out=%d\n", pixel_out);
    
    pixel_out = pixel_out * quad2;
    pixel_out = pixel_out >> 13;
    pixel_out = (pixel_out + 1) >> 1;    // round

    if (pixel_out >= (0x1<<16)) {
        pixel_out = 0xffff;
    }

    return pixel_out;
    
    //            printf("WORD6:  pixel_out=%d\n", pixel_out);
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 01-Mar-2011
 *
 */

static uint16_t prnudsnuquad_correct_pixel( uint16_t pixel_in,  
                                       uint16_t prnu, uint16_t dsnu, uint8_t quad_coeff,
                                       struct pic_prnudsnu_config *prnudsnu_config )
{
    int prnu_bits, dsnu_bits;
    uint32_t prnu_mask, dsnu_mask;
    uint32_t dsnu32, prnu32, pixel32;
    uint16_t expected_pixel_out;
//    float prnu_float;
//    int i;

    prnu_bits = prnudsnu_config->total_bits/2 + prnudsnu_config->extra_prnu_bits[0];
    dsnu_bits = prnudsnu_config->total_bits/2 - prnudsnu_config->extra_prnu_bits[0];

    prnu_mask = (1<<prnu_bits)-1;
    dsnu_mask = (1<<dsnu_bits)-1;

    /* davep 06-Mar-2008 ; XXX temp debug (remove or the test runs will take
     * much longer...again)
     */
//    dbg1( "%s in=%d prnu=%d dsnu=%d prnu_bits=%d dsnu_bits=%d prnu_mask=%#lx dsnu_mask=%#lx\n", 
//                __FUNCTION__, pixel_in, prnu, dsnu, prnu_bits, dsnu_bits,
//                prnu_mask, dsnu_mask );

    /* Make sure I kept the prnu and dsnu nums within the configuration's
     * capabilities.
     *
     * I keep running into pixel validation problems caused by out-of-range prnu
     * values. For example, prnu=64 when only have 6 bits for prnu.
     */
    XASSERT( (prnu & ~prnu_mask) == 0, prnu );
    XASSERT( (dsnu & ~dsnu_mask) == 0, dsnu );

#define DSNU_MASK 0x7fff  /* 15 bits */
#define PRNU_MASK 0x7ffff /* 19 bits */

    dsnu32 = make_corr( dsnu, DSNU_MASK, prnudsnu_config->dmult, 
                            prnudsnu_config->dscale, prnudsnu_config->doffset );
    prnu32 = make_corr( prnu, PRNU_MASK, prnudsnu_config->pmult, 
                            prnudsnu_config->pscale, prnudsnu_config->poffset );

    /* 
     * 
     * Linear Correct
     *
     */
    if( pixel_in < dsnu32 ) {
        /* XXX I'm not 100% sure PIC clips to zero here. The PIC PC MA claims
         * "If the result of subtraction is a negative (-ve) number then replace
         * the output value with the incoming pixel value."  I'm not seeing
         * that with my tests. I'm seeing zero.
         */
        pixel32 = 0;
    }
    else {
        pixel32 = (((uint32_t)pixel_in-dsnu32) * prnu32) >> 15;
    }

    /* on 16-bit overflow, it appears PIC will replace with 0xffff */
    if( pixel32 & ~0xffff ) {
        expected_pixel_out = 0xffff;
    }
    else {
        expected_pixel_out = (uint16_t)pixel32;
    }

    /*
     *  Quadratic Correct  (11-Mar-2011)
     *
     */
    expected_pixel_out = quad_predictor( expected_pixel_out, quad_coeff, 
                                        prnudsnu_config->quad_SF1, 
                                        prnudsnu_config->quad_SF2 );

#if 0
    //# Have to scale everything into [0,1] so Doug's math will work. 
    float x, Q_f, p_f;
    int32_t p; /* NOTE SIGNED! */
    uint16_t pixin;
    uint32_t Q, L;
    uint8_t corr_factor;
    uint32_t SF1, SF2;

    SF1 = prnudsnu_config->quad_SF1;
    SF2 = prnudsnu_config->quad_SF2;
    corr_factor = 51;

    pixin = expected_pixel_out;

    L = ((pixin >> 4) * SF2 ) >> 7;
    if( L > 256 ) { 
        L = 256;
    }

    Q = (corr_factor * SF1 * L) >> 8;
    Q_f = Q / 65536.0; //(float)(2**16);

    x = pixin / 65536.0;

    p_f = (Q_f*x + (1-Q_f)) * x;

//#    print "quad x={0} L={1} Q={2} p={3}".format( x, L, Q, p )

    p = (int)roundf(p_f*65536.0);

    if( p < 0 ) {
        expected_pixel_out = 0;
    }
    else if( p > 65535 ) {
        expected_pixel_out = 65535;
    }
    else {
        expected_pixel_out = p;
    }
    
    /* davep 05-Apr-2010 ; XXX temp debug */
//    dbg2( "%s prnu=%d dsnu=%d dsnu32=%ld in=%d pixel32=%ld expected_out=%d\n", 
//                __FUNCTION__, 
//                prnu, dsnu, dsnu32, pixel_in, pixel32,
//                expected_pixel_out );
#endif
    return expected_pixel_out;
}

/**
 * \brief  mangle an incoming pixel just as PIC PRNU/DSNU does
 *
 * It's not *exactly* how PIC's Pixel Correction does it but it's as
 * functionally equivalent as I can make it.  
 *
 * (Real PIC doesn't use floating point.)
 *
 *
 * \author David Poole
 * \date 06-Mar-2008
 *
 */

static uint16_t
prnudsnu_correct_pixel( uint16_t pixel_in,  
                        uint16_t prnu, uint16_t dsnu, 
                        struct pic_prnudsnu_config *prnudsnu_config )
{
    int prnu_bits, dsnu_bits;
    uint32_t prnu_mask, dsnu_mask;
    uint32_t dsnu32, pixel32, norm_bits;
    uint16_t expected_pixel_out;
//    float prnu_float;
//    int i;

    prnu_bits = prnudsnu_config->total_bits/2 + prnudsnu_config->extra_prnu_bits[0];
    dsnu_bits = prnudsnu_config->total_bits/2 - prnudsnu_config->extra_prnu_bits[0];

    prnu_mask = (1<<prnu_bits)-1;
    dsnu_mask = (1<<dsnu_bits)-1;

    /* davep 06-Mar-2008 ; XXX temp debug (remove or the test runs will take
     * much longer...again)
     */
//    dbg1( "%s in=%d prnu=%d dsnu=%d prnu_bits=%d dsnu_bits=%d prnu_mask=%#lx dsnu_mask=%#lx\n", 
//                __FUNCTION__, pixel_in, prnu, dsnu, prnu_bits, dsnu_bits,
//                prnu_mask, dsnu_mask );

    /* Make sure I kept the prnu and dsnu nums within the configuration's
     * capabilities.
     *
     * I keep running into pixel validation problems caused by out-of-range prnu
     * values. For example, prnu=64 when only have 6 bits for prnu.
     */
    XASSERT( (prnu & ~prnu_mask) == 0, prnu );
    XASSERT( (dsnu & ~dsnu_mask) == 0, dsnu );

    /* maximum DSNU bits: 12+2  (24-bit LUT, split in half, with 2-bits stolen
     * from PRNU
     *
     * maximum DSNU shift left: 11
     *
     * 14+11 == 25 so are we safe with a 32-bit integer?
     *
     */
    dsnu32 = dsnu << prnudsnu_config->scale_dsnu_color[0];

    /* because even with all the shift shift shift, the dsnu correction factor
     * is only 15-bits in the hardware (DougK had to tell me that because it
     * sure isn't clear from the PIC PC MA)
     */
    dsnu32 &= 0x7fff;

    /* TODO get away from using floating point and mimic the normalization used
     * in the actual hardware
     *
     * (so far, the floats have been working ok)
     */
#if 0
    /*
     * Future Dave,
     *
     * Please don't delete this following switch statement.  It breaks down the
     * sets of the one prnu_float one-liner into recognizable chunks.
     *
     * Thanks,
     * Past Dave.
     *
     */
    /* davep 04-Apr-2008 ; Hello, this is Future Dave, soon to be Past Dave
     * eventually.  The recent ASIC changed the scale_prnu_color==3 case from
     * divide-by-two to multiply-by-eight. The changes gives us a bigger gain
     * range.  Added code to mimic the new case (just so I could see the math
     * again) but didn't test it. Sorry!
     */
    prnu_float = 0.0;
    switch( prnudsnu_config->scale_prnu_color[0] ) {
        case 0 :
//            prnu_float = (float)prnu / ((1<<prnu_bits)/4.0);
//            prnu_float = (float)prnu / ((1<<prnu_bits)>>2 );
            prnu_float = (float)prnu / (1<<(prnu_bits-2));
            break;
        case 1 :
//            prnu_float = (float)prnu / ((1<<prnu_bits)/2.0);
            prnu_float = (float)prnu / (1<<(prnu_bits-1));
            break;
        case 2 :
            prnu_float = (float)prnu / (1<<prnu_bits);
            break;
        case 3 :
#ifdef HAVE_ICE_PRNUDSNU_MUL8
//            prnu_float = (float)prnu / ((1<<prnu_bits)/8.0);
            prnu_float = (float)prnu / (1<<(prnu_bits-3));
#else
//            prnu_float = (float)prnu / ((1<<prnu_bits)*2.0);
            prnu_float = (float)prnu / (1<<(prnu_bits+1));
#endif
            break;
        default :
            XASSERT(0,prnudsnu_config->scale_prnu_color[0]);
    }
#endif

    norm_bits = prnu_bits + scale_prnu_shift[ prnudsnu_config->scale_prnu_color[0] ];

    if( pixel_in < dsnu32 ) {
        /* XXX I'm not 100% sure PIC clips to zero here. The PIC PC MA claims
         * "If the result of subtraction is a negative (-ve) number then replace
         * the output value with the incoming pixel value."  I'm not seeing
         * that with my tests. I'm seeing zero.
         */
        pixel32 = 0;
    }
    else {
        /* davep 13-Jan-2010 ; I think this math is more correct. This is the
         * closest I've come to reverse engineering the pixel correction
         * equation. So far, this equation has worked with everything I throw at
         * it.
         */
        pixel32 = (uint32_t)pixel_in;
        pixel32 = (pixel32-dsnu32) * (prnu+(1<<norm_bits));
        pixel32 >>= norm_bits;
    }

    /* on 16-bit overflow, it appears PIC will replace with 0xffff */
    if( pixel32 & ~0xffff ) {
        expected_pixel_out = 0xffff;
    }
    else {
        expected_pixel_out = (uint16_t)pixel32;
    }
    
    /* davep 05-Apr-2010 ; XXX temp debug */
//    dbg2( "%s prnu=%d dsnu=%d dsnu32=%ld in=%d pixel32=%ld expected_out=%d\n", 
//                __FUNCTION__, 
//                prnu, dsnu, dsnu32, pixel_in, pixel32,
//                expected_pixel_out );

    return expected_pixel_out;
}

//--------------------------------------------------------------------------
// Function     : pictest_prnudsnu_feeder
//   returns    : none
//   input      : ptr to scan_msg_t structure
// Created by   : David Poole
// Date created : Mar-05
// Description  : 
//
//  Feeds data into PIC ADC test register for a PRNU/DSNU test.
//
// Notes        : 
//
//--------------------------------------------------------------------------

thread_return_t pictest_prnudsnu_feeder( void *input )
{

    scan_msg_t *msg;
//    uint32_t num32;
    int pcnt, ccnt, rcnt;
    int color[3];
    int wordcount;
    int total_bytes;
    int sanity;
    int num_colors;
    uint16_t pixel;
    struct pic_dma_test *pictest;

    dbg2("%s start\n", __FUNCTION__ );

    /* wait until my parent thread is ready to let me run */
    pictest_feeder_wait_for_release();

    /* Input should be a pointer to a message structure containing the number
     * of rows to feed into the ADC test register and the number of pixels in
     * each row.
     *
     * ADC wants words so we need to count words.
     */ 

    msg = (scan_msg_t *)input;
    prnudsnu_feeder_msg_decode( msg, &pixel, &pictest );

    /* hardwire ourselves to 2-byte per pixels */
    wordcount = pictest->pixels_per_row;

    dbg2("sending rows=%d by pixels_per_row=%d giving words=%d\n", 
                pictest->total_rows, pictest->pixels_per_row, 
                pictest->total_rows*wordcount );

    if( pictest->cmode==SCAN_CMODE_COLOR ) {
        num_colors = 3;
        color[0] = PIC_CBI_CEVEN_0;
        color[1] = PIC_CBI_CEVEN_1;
        color[2] = PIC_CBI_CEVEN_2;
    }
    else {
        num_colors = 1;
        color[0] = PIC_CBI_MEVEN;
    }

    /* want to send in 16-bit per pixel RGB data */
    // "*2" for 2 bytes per pixel
    total_bytes = pictest->total_rows * wordcount * 2 * num_colors;

    dbg2("total bytes=%d\n", total_bytes );

    sanity = 0;

    /* davep 06-Mar-2008 ;  starting to change the pixel value during the test
     * instead of sending exactly the same pixel over and over
     *
     * note I'm allowing the pixel to roll from 2**16-1 back to zero
     */
#define NEXT_PIXEL(pixel) pixel++

    /* rcnt = row count 
     * ccnt = color count
     * pcnt = pixel count
     */
    for( rcnt=0 ; rcnt<pictest->total_rows ; rcnt++ ) {

        /* send in three color planes a line at a time like CIS does */
        for( ccnt=0 ; ccnt<num_colors ; ccnt++ ) {

            dbg2( "%s row=%d color=%d sanity=%d pixel=%d\n", __FUNCTION__, 
                        rcnt, ccnt, sanity, pixel );

            /* send in a starting pixel; PIC needs to know when it's receiving
             * the last line of data thus the _FIRPLL vs _FIRP 
             */
            if( rcnt == pictest->total_rows-1) {
                /* davep 02-Apr-2010 ; don't use FIRPLL, causes problems with
                 * descriptor DMA. 
                 */
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt],
                                    NEXT_PIXEL(pixel) );
//                /* first pixel in last line */
//                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRPLL, color[ccnt],
//                                    NEXT_PIXEL(pixel) );
            }
            else {
                /* first pixel in non-last line */
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_FIRP, color[ccnt],
                                    NEXT_PIXEL(pixel) );
            }
            sanity++;

            /* now send in a block of data;
             * -2 for the starting and ending pixels 
             */
            for( pcnt=0 ; pcnt<wordcount-2 ; pcnt++ ) {
                pic_adcnorm_ant( PIC_ADCN_TEST_DT_NORMP, color[ccnt],
                                    NEXT_PIXEL(pixel) );
                sanity++;
            }

            /* send in a last pixel */
            pic_adcnorm_ant( PIC_ADCN_TEST_DT_LASTP, color[ccnt],
                                    NEXT_PIXEL(pixel) );
            sanity++;
        }
    }

#undef NEXT_PIXEL

    XASSERT( sanity*2 == total_bytes, sanity );

    dbg2("done sending %d words of data\n", sanity );
    dbg2("%s end\n", __FUNCTION__ );

    /* release the lock I'm holding */
    pictest_feeder_release();

    return (thread_return_t)0;
}


/**
 * \brief  sorta validate the results from a run through PRNU/DSNU
 *
 * Also responsible for putting the buffers back onto the PIC ready list from
 * where they'll eventually be freed.
 *
 * \author David Poole
 * \date way back when and tortured much since then
 *
 */

static void 
pic_prnudsnu_validate( uint16_t pixel_in, 
                       uint16_t prnu_table[], uint16_t dsnu_table[], uint8_t quad,
                       struct pic_prnudsnu_config *prnudsnu_config,
                       ATLISTENTRY *results_buffers )
{
    struct ice_dma_buffer *icebuf;
    int row;
    uint16_t *ptr16;
//    uint8_t *ptr8;
    uint16_t curr_pixel;
    int bpp;
    uint16_t expected_pixel_out;
    int pixels_per_row;
    uint32_t pixel_count;
    int failcount;
    uint16_t *prnu_ptr, *dsnu_ptr;
    uint16_t prnu, dsnu;

    dbg2("%s start\n", __FUNCTION__ );

    /* XXX hardwiring to 16-bpp */
    bpp = 16;

    /* My updated feeder will send a 16-bit pixel value starting from the value
     * I was just passed in pixel_in;
     *
     * So I loop through the returning buffers, running my own correction on the
     * incrementing pixel_in and comparing against the hardware corrected
     * pixels.
     */

    pixel_count = 0;
    failcount = 0;

    while( 1 ) {
        icebuf = icebuf_pop( results_buffers );
        if( icebuf== NULL ) {
            /* end of data, leave while(1) loop */
            break;
        }

        cpu_dcache_invalidate_region( icebuf->data, icebuf->bytes_per_row*icebuf->num_rows );

//        scanlog_hex_dump( icebuf->data, MIN(64,icebuf->bytes_per_row * icebuf->num_rows) ); 
//        scanlog_hex_dump( icebuf->data, icebuf->bytes_per_row * icebuf->num_rows ); 

        /* verify every pixel in the buffer is the same value (as of this
         * writing (6-Mar-2008), my tests all feed the exact same pixel in)
         *
         * This test is for making sure I have my prnudsnu LUT alignment set up
         * correctly.
         */
        for( row=0 ; row<icebuf->num_rows ; row++ ) {

            if( bpp==16 ) {
                ptr16 = (uint16_t *)&icebuf->data[ icebuf->bytes_per_row * row ];
                pixels_per_row = icebuf->bytes_per_row / 2;

                pixel_count += pixels_per_row;

                prnu_ptr = prnu_table;
                dsnu_ptr = dsnu_table;

                while( pixels_per_row ) {
                    curr_pixel = *ptr16++;

                    prnu = *prnu_ptr++;
                    dsnu = *dsnu_ptr++;

#ifdef HAVE_PIC_PC_QUAD 
                    expected_pixel_out = prnudsnuquad_correct_pixel( pixel_in,
                                            prnu, dsnu, quad, prnudsnu_config);
/* +/- N until I get more practice with the new changes */
#define COMPARE_PIXEL(p1,p2) abs((p1)-(p2)) > 5

#else
                    expected_pixel_out = prnudsnu_correct_pixel( pixel_in,
                                            prnu, dsnu, prnudsnu_config);
/* exact */
#define COMPARE_PIXEL(p1,p2) (p1)!=(p2)

#endif

                    pixel_stats.num_tested++;
                    if( pixel_in==expected_pixel_out ) {
                        pixel_stats.num_duplicates++;
                    }
                    if( !(expected_pixel_out & 0xffff) ) {
                        pixel_stats.num_saturated++;
                    }
                    else if( !expected_pixel_out ) {
                        pixel_stats.num_zero++;
                    }

                    
                    if( COMPARE_PIXEL(curr_pixel, expected_pixel_out) ) {
                        /* extra message in case we're running at dbg1 level */
                        scanlog( 0, "%s FAILURE! in=%d prnu=%ld dsnu=%ld expect=%d actual=%d\n",
                                    __FUNCTION__, pixel_in, prnu, dsnu, 
                                    expected_pixel_out, curr_pixel );
                        scanlog( 0, "%d %d,%d,%d %d,%d,%d %d,%d,%d\n", 
                                    prnudsnu_config->total_bits,
                                    prnudsnu_config->extra_prnu_bits[0],
                                    prnudsnu_config->extra_prnu_bits[1],
                                    prnudsnu_config->extra_prnu_bits[2],

                                    prnudsnu_config->scale_prnu_color[0],
                                    prnudsnu_config->scale_prnu_color[1],
                                    prnudsnu_config->scale_prnu_color[2],

                                    prnudsnu_config->scale_dsnu_color[0],
                                    prnudsnu_config->scale_dsnu_color[1],
                                    prnudsnu_config->scale_dsnu_color[2] );
                        scanlog( 0, "%d,%d,%d %d,%d,%d\n",
                                    prnudsnu_config->pmult,
                                    prnudsnu_config->pscale,
                                    prnudsnu_config->poffset,

                                    prnudsnu_config->dmult,
                                    prnudsnu_config->dscale,
                                    prnudsnu_config->doffset
                             );
                        scanlog( 0, "PDCR1=%#lx PDCR2=%#lx\n", *PIC_PC_PDCR1, *PIC_PC_PDCR2 );
#ifdef PIC_PC_QUAD
                        scanlog( 0, "QUAD=%#lx\n", *PIC_PC_QUAD );
#endif
#ifdef PIC_PC_PMULT
                        scanlog( 0, "DOFF=%#lx DMULT=%#lx POFF=%#lx PMULT=%#lx\n", 
                                    *PIC_PC_DOFF, *PIC_PC_DMULT, *PIC_PC_POFF, *PIC_PC_PMULT );
#endif

                        /* dump the read dma register space */
//                        scanlog_hex_dump( (unsigned char *)PIC_PC_RDMA_PDCR, 0x24 );

//                        scanlog_hex_dump( (unsigned char *)&icebuf->data[ icebuf->bytes_per_row * row],
//                                icebuf->bytes_per_row );

                        if( ++failcount > 6 ) {
                            ASSERT(0);
                            XASSERT( curr_pixel == expected_pixel_out, curr_pixel );
                        }
                    }
//                    XASSERT( curr_pixel == expected_pixel_out, curr_pixel );

                    pixels_per_row--;
                    pixel_in++;
                }
            }
            else {
                /* davep 10-Mar-2008 ; XXX untested code  */
                XASSERT( 0, bpp );
#if 0
                ptr8 = &icebuf->data[ icebuf->bytes_per_row * row ];
                pixels_per_row = icebuf->bytes_per_row;

                while( pixels_per_row ) {
                    curr_pixel = (uint16_t)*ptr8++;

                    expected_pixel_out = prnudsnu_correct_pixel( pixel_in,
                            prnu, dsnu, prnudsnu_config);

                    XASSERT( curr_pixel == expected_pixel_out, curr_pixel );
                    pixels_per_row--;
                    pixel_in++;
                }
#endif
            }
        }

//        if( scanlog_get_level() >= 2 ) {
//            scanlog_hex_dump( icebuf->data, 
//                    MIN( 16, icebuf->bytes_per_row * icebuf->num_rows ) ); 
//        }

        /* put it back on the ready list */
        pic_wdma_add_ready( icebuf->channel, &icebuf );
    }
    
    print_pixel_stats( &pixel_stats );
    dbg2("%s end pixels=%ld ok\n", __FUNCTION__, pixel_count );
}

/**
 * \brief  allocate space for the unencoded PRNU, DSNU luts and for the encoded
 * prnudsnu LUT 
 *
 * \author David Poole
 * \date 29-Feb-2008
 *
 */

static int 
prnudsnu_tables_allocate( int num_pixels, int num_colors, int total_bits, 
                          uint16_t **prnu_lut, uint16_t **dsnu_lut, uint8_t **quad_lut,
                          uint8_t **dma_lut, int *num_bytes_in_lut )
{
    int total_pixels;

    /* stupid human check */
    XASSERT(   total_bits==12||total_bits==16
             ||total_bits==20||total_bits==24
             ||total_bits==28||total_bits==32, total_bits );

    /* If we're doing color, allocate one array 3x the size. We'll poke pointers
     * into it to split it into R,G,B arrays. The PRNU/DSNU LUT has to be
     * contiguous anyway and by using one array each for prnu[] and dsnu[],
     * we'll have fewer pointers to release later.
     */
    total_pixels = num_pixels * num_colors;

    *prnu_lut = *dsnu_lut = NULL;
    *quad_lut = NULL;
    *dma_lut = NULL;

    *prnu_lut = (uint16_t *)MEM_MALLOC( total_pixels * sizeof(uint16_t) );
    if( *prnu_lut==NULL ) {
        dbg2( "%s failed to alloc %d bytes for PRNU table\n", __FUNCTION__,
                    num_pixels*sizeof(uint16_t) );
        return -1;
    }

    *dsnu_lut = (uint16_t *)MEM_MALLOC( total_pixels * sizeof(uint16_t) );
    if( *dsnu_lut==NULL ) {
        dbg2( "%s failed to alloc %d bytes for DSNU table\n", __FUNCTION__,
                    num_pixels*sizeof(uint16_t) );
        PTR_FREE( *prnu_lut );
        return -1;
    }

    *quad_lut = (uint8_t *)MEM_MALLOC( total_pixels * sizeof(uint8_t) );
    if( *quad_lut==NULL ) {
        dbg2( "%s failed to alloc %d bytes for QUAD table\n", __FUNCTION__,
                    num_pixels*sizeof(uint8_t) );
        PTR_FREE( *prnu_lut );
        PTR_FREE( *dsnu_lut );
        return -1;
    }

    /* total_bits must be 12, 16, 20, or 24, the available sizes for the
     * PRNU/DSNU DMA table
     *
     * +4 to round up to an atomic number of bytes (no fractional bytes allowed)
     */
    *num_bytes_in_lut = (total_pixels * total_bits + 4) / 8;
#if ICE_DMA_BYTE_ALIGN==32
    *dma_lut = (uint8_t *)MEM_MALLOC_ALIGN( *num_bytes_in_lut, e_32_byte );
#elif ICE_DMA_BYTE_ALIGN==16
    *dma_lut = (uint8_t *)MEM_MALLOC_ALIGN( *num_bytes_in_lut, e_16_byte );
#else
#error Unknown/unsupported ICE_DMA_BYTE_ALIGN value.
#endif
    if( *dma_lut==NULL ) {
        dbg2( "%s failed to alloc %d bytes for PRNU table\n", __FUNCTION__,
                    *num_bytes_in_lut );
        PTR_FREE( *prnu_lut );
        PTR_FREE( *dsnu_lut );
        PTR_FREE( *quad_lut );
        return -1;
    }

    dbg2( "%s dma_lut=%p total_bits=%d needed_bits=%d bytes_in_lut=%d bits_in_lut=%d\n",
                __FUNCTION__, *dma_lut, total_bits, num_pixels*total_bits,
                *num_bytes_in_lut, *num_bytes_in_lut*8 );

    memset( *prnu_lut, 0, num_pixels*sizeof(uint16_t) );
    memset( *dsnu_lut, 0, num_pixels*sizeof(uint16_t) );
    memset( *quad_lut, 0, num_pixels*sizeof(uint8_t) );
    memset( *dma_lut, 0, *num_bytes_in_lut );

    /* success */
    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 21-Mar-2011
 *
 */

static void make_prnu_dsnu_quad_table( struct pic_prnudsnu_config *prnudsnu_config,
                int pixels_per_row, uint32_t prnu, uint32_t dsnu, uint8_t quad,
                uint16_t prnu_table[], uint16_t dsnu_table[], uint8_t quad_table[] )
{
    int i;
    int prnu_bits, dsnu_bits;
    uint32_t prnu_mask, dsnu_mask;

    prnu_bits = prnudsnu_config->total_bits/2 + prnudsnu_config->extra_prnu_bits[0];
    dsnu_bits = prnudsnu_config->total_bits/2 - prnudsnu_config->extra_prnu_bits[0];

    prnu_mask = (1<<prnu_bits)-1;
    dsnu_mask = (1<<dsnu_bits)-1;

    /* make a simple prnudsnu lut */
    for( i=0 ; i<pixels_per_row ; i++ )  {
//        prnu_table[i] = (uint16_t)prnu;
//        dsnu_table[i] = (uint16_t)dsnu;
        prnu_table[i] = (uint16_t)((prnu+i) & prnu_mask);
        dsnu_table[i] = (uint16_t)((dsnu+i) & dsnu_mask);
        quad_table[i] = quad;
    }
}

//--------------------------------------------------------------------------
// Function     : run_prnudsnu_test
//   returns    : none
// Created by   : David Poole
// Date created : Mar-05
// Description  : 
//
//  Run a simple test of PRNU/DSNU. As of this writing, only scanlog_hex_dumps() the
//  results so the user can see what it looks like.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

int
run_prnudsnu_test( scan_cmode_t cmode, int pixels_per_row, 
                   uint32_t prnu, uint32_t dsnu, uint8_t quad, 
                   uint16_t pixel_in, 
                   struct pic_prnudsnu_config *prnudsnu_config
                 )
{
    int i;
//    uint32_t num32;
    uint8_t *pc_lut;
    scan_msg_t *msg;
    ATLISTENTRY results_buffers;
//    int pixels_per_row;
    struct pic_dma_test pictest;
    int retcode;
    int num_colors, pc_lut_num_bytes;
    uint16_t *prnu_table, *dsnu_table;
    uint8_t *quad_table;

    /* stfu compiler */
    pc_lut_num_bytes = 0;

    pic_interrupt_disable();

    dbg1("%s start ppr=%d prnu=%ld dsnu=%ld pixel_in=%d\n", __FUNCTION__, pixels_per_row,
                prnu, dsnu, pixel_in );
    print_prnudsnu_config( prnudsnu_config );
    
    /* start PIC from a clean state */
    pic_setup_for_test();

    /* turn on margins but don't chop (note that the margins actually live in
     * the bulb monitor block thus we have to enable the bulb monitor block)
     *
     * (Margins aren't necessary for PRNU/DSNU but I wanted to throw them in
     * here to make the test a little more complex.)
     */
    pic_marg_set_left(0);
    pic_marg_set_right( pixels_per_row-1 );
//    pic_bm_set_bypass( false );
    pic_marg_set_bypass( false );

    /* set up our DMA for a data run */
    memset( &pictest, 0, sizeof(struct pic_dma_test));
    pictest.cmode = cmode;

    pictest_choose_channels( cmode, &pictest );
    pictest.rows_per_buffer = 3;
    pictest.total_rows      = 9;
    /* XXX - hardwiring to 16bpp */
    pictest.pixels_per_row = pixels_per_row;
    pictest.bytes_per_row = pictest.pixels_per_row * 2;
    pictest.buffers_per_channel = pictest.total_rows / pictest.rows_per_buffer;
    pictest.num_empties = pictest.buffers_per_channel * pictest.num_channels; 

    pictest_setup_dma( &pictest );

    dbg2( "PIC WDMA %d channel prnudsnu test of rows=%d bytes_per_row=%d total_bytes=%d\n",
                pictest.num_channels, 
                pictest.total_rows, pictest.bytes_per_row,
                pictest.bytes_per_row * pictest.total_rows );

    /* davep 25-Feb-2008 ; use the new struct pic_prnudsnu_config */
    pic_prnudsnu_set_config( prnudsnu_config );

    // Disable exposure compensation, disable bad pixel,
    // enable PRNU/DSNU
    pic_prnudsnu_set_bypass_all( false );
    pic_prnudsnu_set_bypass_prnu( false );
    pic_prnudsnu_set_bypass_dsnu( false );
    pic_prnudsnu_set_bypass_bad_pix_replace( true );
    pic_prnudsnu_set_bypass_exp_comp( true );

    pic_prnudsnu_set_bypass_quadratic( true );

#ifdef HAVE_PIC_PC_QUAD 
    //Enable quadratic correct if the quad_SF1 is valid
    if(prnudsnu_config->quad_SF1!=0)
    {
    	pic_prnudsnu_set_quadratic_config( prnudsnu_config->quad_SF1, prnudsnu_config->quad_SF2, prnudsnu_config->quad_shift);
    	pic_prnudsnu_set_bypass_quadratic( false );
    }
#endif

    dbg1( "%s %d %d,%d,%d %d,%d,%d %d,%d,%d\n", __FUNCTION__,
                prnudsnu_config->total_bits,
                prnudsnu_config->extra_prnu_bits[0],
                prnudsnu_config->extra_prnu_bits[1],
                prnudsnu_config->extra_prnu_bits[2],

                prnudsnu_config->pmult,
                prnudsnu_config->pscale,
                prnudsnu_config->poffset,

                prnudsnu_config->dmult,
                prnudsnu_config->dscale,
                prnudsnu_config->doffset
         );

    pic_prnudsnu_dump();

    /* Make our pixel correction buffers. */
    if( cmode==SCAN_CMODE_MONO ) {
        num_colors = 1;
    }
    else {
        num_colors = 3;
    }

    retcode = prnudsnu_tables_allocate( pictest.pixels_per_row,
                                        num_colors,
                                        prnudsnu_config->total_bits, 
                                        &prnu_table, 
                                        &dsnu_table, 
                                        &quad_table,
                                        &pc_lut,
                                        &pc_lut_num_bytes );
    XASSERT( retcode==0, retcode );

    XASSERT( prnudsnu_config->total_bits*pictest.pixels_per_row <= pc_lut_num_bytes*8,
            pc_lut_num_bytes );

    make_prnu_dsnu_quad_table( prnudsnu_config, pictest.pixels_per_row, 
                                prnu, dsnu, quad, 
                                prnu_table, dsnu_table, quad_table );

    /* use channel 0 for extra_prnu_bits because I'm configuring all three
     * channels the same anyway  (lazy!)
     */
    pic_prnudsnu_encode_lut( prnudsnu_config->total_bits, 
                             prnudsnu_config->extra_prnu_bits[0], 
                             pictest.pixels_per_row, 
                             prnu_table, dsnu_table, quad_table,
                             pc_lut, pc_lut_num_bytes );
    if( scanlog_get_level() >= 2 ) {
        scanlog_hex_dump( pc_lut, MIN(pc_lut_num_bytes, 16) );
    }

    /* push the PC LUT into hardware */
    pictest_prnudsnu_load_lut( pc_lut, pc_lut_num_bytes );

    /* start it */
    pic_prnudsnu_start_lut_dma();

    /* need to pass a few parameters to the thread that feeds data into ADC so
     * cheat a bit and use the AGMSG structure to do so.  I am responsible for
     * freeing this, not the thread. 
     */
    msg = (scan_msg_t *)MEM_MALLOC( sizeof(scan_msg_t) ); 
    XASSERT( msg != NULL,(uint32_t)msg );
    memset( msg, 0, sizeof(scan_msg_t) );

    /* Yes, yes, yes, I'm sending a pointer to a data structure on the stack.
     * However, this function will wait for the feeder thread to finish then
     * delete the thread. So sending something on the stack is ugly but
     * functional. So there.
     */
    prnudsnu_feeder_msg_encode( msg, pixel_in, &pictest );

    /* create feeder thread suspended; pictest_run_it() will resume */
    pictest_make_thread( pictest_prnudsnu_feeder, msg );

    /* data results will land here */
    ATInitList( &results_buffers );

    /* davep 14-Feb-2011 ; XXX temp debug */
//    pic_dump();

    /* fire in the hole! */
    pic_interrupt_enable();

    /* run the feeder thread */
    pictest_run_it( pictest_get_task_id(), &results_buffers, 
            pictest.channels, pictest.num_channels, pictest.total_rows );

    /* done getting data, clean up thread */
    pictest_wait_for_feeder_complete();

    for( i=0 ; i<pictest.num_channels ; i++ ) {
        pic_wdma_channel_disable( pictest.channels[i] );
    }

    pic_prnudsnu_validate( pixel_in, prnu_table, dsnu_table, quad, prnudsnu_config, &results_buffers );

    /* 
     * Put everything back on the ready list so we can count our buffers and find any memory leaks
     */
    pictest_dma_cleanup( &results_buffers );

   /* everything used should be back on the appropriate lists */
    pic_wdma_sanity();

    /* count PIC write */
    verify_pic_buffer_counts( &pictest );

    pic_wdma_close();

    /* free the message we passed to our feeder thread */
    PTR_FREE( msg );
    /* free our allocated LUTs */
    PTR_FREE( prnu_table );
    PTR_FREE( dsnu_table );
    PTR_FREE( pc_lut );

    /* Reset PIC to its power-on reset state */
    pic_cleanup_after_test();

    dbg2("%s end\n", __FUNCTION__ );

    /* success! */
    return 0;
}

/**
 * \brief  push the PC LUT into hardware
 *
 * wrapped in its own function so can transparently use fast memory (e.g.,
 * sram) space if available
 *
 *
 * \author David Poole
 * \date 05-Dec-2007
 *
 */

void
pictest_prnudsnu_load_lut( uint8_t *pc_lut, int datalen )
{
    /* flush our data from cache into main memory */
    cpu_dcache_writeback_region( pc_lut, datalen );

    /* make it a real DMA address by pushing in the cache bypass */
    pc_lut = CACHE_BYPASS_SET( pc_lut );

    pic_prnudsnu_reset_lut_dma();
    pic_prnudsnu_enable_lut_dma(false);
    /* davep 20-Jan-2011 ; XXX shotgun debug ; enable it early? */
    pic_prnudsnu_enable_lut_dma(true);

    pic_prnudsnu_set_lut_addr( (dma_addr_t)pc_lut );
    pic_prnudsnu_set_lut_xfer_count( datalen );

    pic_prnudsnu_enable_lut_dma(true);

    /* dump a bit of my lookup table so I know what it looks like */
//#if SDBGLEVEL > 1 
//    dbg1("PRNU/DSNU lookup table\n" ) );
    if( scanlog_get_level() >= 2 ) {
        scanlog_hex_dump( (uint8_t *)pc_lut, 16 );
    }
//#endif

    dbg2("push ptr=%p len=%d into PRNU/DSNU read DMA\n", pc_lut, datalen );

//    pic_rdma_dump();
}


static void
init_simple_prnudsnu( uint16_t *prnu, uint16_t *dsnu, int num_pixels )
{
    int i;

    /* Note I'm building this list blind to the actual max bit size of each
     * entry.
     *
     * In order to handle all possible permuations of total_bits and
     * extra_prnu_bits, the max size of any prnu/dsnu entry is 4 bits
     *
     * Most restrictive case:
     *  total_bits=12
     *  prnu / dsnu split = 6 bits
     *  extra_prnu_bits = -2 or +2
     * 
     * so prnu or dsnu can be at most 4-bits
     */
    for( i=0 ; i<num_pixels ; i++ ) {
        prnu[i] = 0xfff;
        dsnu[i] = 0xeee;
    }
}

/**
 * \brief  test the prnudsnu encode function
 *
 *
 * \author David Poole
 * \date 05-Mar-2008
 *
 */

static void
prnudsnu_encode_test( int num_pixels )
{
    uint8_t valid_total_bits[] = { 12, 16, 20, 24 };
    int8_t valid_extra_prnu_bits[] = { -2, -1, 0, 1, 2 };
    uint16_t *prnu, *dsnu;
    uint8_t *quad;
    uint8_t *prnudsnu_lut;
    int i, j;
    int retcode;
    uint8_t total_bits;
    int8_t extra_prnu_bits;
    int pc_lut_num_bytes;
    int num_colors;

    dbg2( "%s num_pixels=%d\n", __FUNCTION__, num_pixels );

    total_bits = 16;
    extra_prnu_bits = 0;
    num_colors = 1; /* mono */
    
    dbg1( "%s total_bits=%d extra_prnu_bits=%d\n", 
            __FUNCTION__, total_bits, extra_prnu_bits );

    retcode = prnudsnu_tables_allocate( num_pixels, num_colors, total_bits,
                                        &prnu, &dsnu, &quad, &prnudsnu_lut,
                                        &pc_lut_num_bytes );
    XASSERT( retcode==0, retcode );

    XASSERT( total_bits*num_pixels <= pc_lut_num_bytes*8,
            pc_lut_num_bytes );

    init_simple_prnudsnu( prnu, dsnu, num_pixels );

    pic_prnudsnu_encode_lut( total_bits, extra_prnu_bits, num_pixels, 
                                prnu, dsnu, quad, prnudsnu_lut, pc_lut_num_bytes );

    /* TODO decode and validate (happy the silly thing doesn't corrupt
     * memory at this point); have done a lot of bench tests w/ a Python
     * decoder and the encode_lut function running on my PC
     */
    if( scanlog_get_level() >= 2 ) {
//            scanlog_hex_dump( prnudsnu_lut, pc_lut_num_bytes );
        scanlog_hex_dump( prnudsnu_lut, MIN(pc_lut_num_bytes, 64) );
    }

    PTR_FREE( prnu );
    PTR_FREE( dsnu );
    PTR_FREE( quad );
    PTR_FREE( prnudsnu_lut );
//    return;

    for( i=0 ; i<sizeof(valid_total_bits)/sizeof(valid_total_bits[0]) ; i++ ) {
        for( j=0 ; j<sizeof(valid_extra_prnu_bits)/sizeof(valid_extra_prnu_bits[0]) ; j++ ) {

            total_bits = valid_total_bits[i];
            extra_prnu_bits = valid_extra_prnu_bits[j];
            
            dbg1( "%s total_bits=%d extra_prnu_bits=%d\n", 
                    __FUNCTION__, total_bits, extra_prnu_bits );

            retcode = prnudsnu_tables_allocate( num_pixels, num_colors,
                                                total_bits, &prnu, &dsnu, &quad,
                                                &prnudsnu_lut,
                                                &pc_lut_num_bytes );
            XASSERT( retcode==0, retcode );

            XASSERT( total_bits*num_pixels <= pc_lut_num_bytes*8,
                    pc_lut_num_bytes );

            init_simple_prnudsnu( prnu, dsnu, num_pixels );

            pic_prnudsnu_encode_lut( total_bits, extra_prnu_bits, num_pixels, 
                                        prnu, dsnu, quad, prnudsnu_lut, pc_lut_num_bytes );

            /* TODO decode and validate (happy the silly thing doesn't corrupt
             * memory at this point); have done a lot of bench tests w/ a Python
             * decoder and the encode_lut function running on my PC
             */
//            scanlog_hex_dump( prnudsnu_lut, pc_lut_num_bytes );
            if( scanlog_get_level() >= 2 ) {
                scanlog_hex_dump( prnudsnu_lut, MIN(pc_lut_num_bytes, 64) );
            }

            PTR_FREE( prnu );
            PTR_FREE( dsnu );
            PTR_FREE( quad );
            PTR_FREE( prnudsnu_lut );
        }
    }
    /* done! */
}

/**
 * \brief run a pixel through all possible PRNU settings ; verify result pixel
 *
 *
 * \author David Poole
 * \date 06-Mar-2008
 *
 */

static void
exhaustive_prnu_test( scan_cmode_t cmode, int pixels_per_row )
{
    int retcode;
    uint32_t prnu, dsnu;
    uint16_t pixel_in;
    struct pic_prnudsnu_config prnudsnu_config;
    int prnu_bits, dsnu_bits;
    uint32_t prnu_mask, dsnu_mask;
    uint8_t quad = 0;

    dbg2( "%s ppr=%d\n", __FUNCTION__, pixels_per_row );

    memset( &prnudsnu_config, 0, sizeof(struct pic_prnudsnu_config) );
    prnudsnu_config.total_bits = 12;
    /* XXX hardwire to channel 0 since my tests assume all three channels are
     * set the same 
     */
    prnudsnu_config.extra_prnu_bits[0] = -2;
    prnudsnu_config.scale_prnu_color[0] = 0;

    /* rest of the fields will change in the loops below or are fine at zero */

    for( prnudsnu_config.total_bits = 12 ;
         prnudsnu_config.total_bits <= 24 ;
         prnudsnu_config.total_bits += 4 ) {

        for( prnudsnu_config.extra_prnu_bits[0] = -2 ;
             prnudsnu_config.extra_prnu_bits[0] <= 2 ;
             prnudsnu_config.extra_prnu_bits[0]++ ) {

            for( prnudsnu_config.scale_prnu_color[0] = 0 ;
                 prnudsnu_config.scale_prnu_color[0] <= 3 ;
                 prnudsnu_config.scale_prnu_color[0] ++ ) {

                prnu_bits = prnudsnu_config.total_bits/2 + prnudsnu_config.extra_prnu_bits[0];
                dsnu_bits = prnudsnu_config.total_bits/2 - prnudsnu_config.extra_prnu_bits[0];
                prnu_mask = (1<<prnu_bits)-1;
                dsnu_mask = (1<<dsnu_bits)-1;

                dbg1( "%s extra_prnu_bits=%d scale_prnu_color=%d prnu_bits=%d prnu_mask=%#lx\n",
                            __FUNCTION__,
                            prnudsnu_config.extra_prnu_bits[0], 
                            prnudsnu_config.scale_prnu_color[0], prnu_bits, prnu_mask );

//                for( prnu=0 ; prnu<10 ; prnu++ )  {
                for( prnu=0 ; ; prnu++ )  {

                    /* do we still have enough bits for this prnu? */
                    if( (prnu & ~prnu_mask) != 0 ) {
                        break;
                    }

                    /* playin' */
                    dsnu = prnu & dsnu_mask;

                    /* badly chosen random pixel value */
                    pixel_in = (uint16_t)lehmer_random();
//                    pixel_in = (uint16_t)cpu_get_ccount();

                    dbg1( "%s prnu=%ld dsnu=%ld prnu_bits=%d dsnu_bits=%d scale_prnu=%d in=%d\n", 
                                    __FUNCTION__, prnu, dsnu, prnu_bits,
                                    dsnu_bits, prnudsnu_config.scale_prnu_color[0],
                                    pixel_in ); 

                    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu,
                                                    quad, pixel_in, &prnudsnu_config );

                    XASSERT( retcode==0, retcode );

                }
            }
        }
    }
}

static void pic_prnudsnu_config_init( struct pic_prnudsnu_config *prnudsnu_config, 
                                      int total_bits, 
                                      int extra_prnu_bits,
                                      int scale_prnu_color,
                                      int scale_dsnu_color )
{
    int i;

    dbg2( "%s %d %d\n", __FUNCTION__, total_bits, extra_prnu_bits );

    /* davep 11-Aug-2010 ; only do 12/12 for now until I can get the endian
     * problems figured out
     */
    XASSERT( total_bits==24, total_bits );
    XASSERT( extra_prnu_bits==0, extra_prnu_bits );

    memset( prnudsnu_config, 0, sizeof(struct pic_prnudsnu_config) );
    prnudsnu_config->total_bits = total_bits;

#ifdef HAVE_PIC_PC_QUAD 
    /* davep 11-Aug-2010 ; XXX cheap hack for trying quadratic */
    prnudsnu_config->total_bits += 8;
#endif

    for( i=0 ; i<3 ; i++ ) {
        /* valid values are -2,-1,0,1,2 */
        prnudsnu_config->extra_prnu_bits[i] = extra_prnu_bits;

        /* valid values are one of PIC_PRNUDSNU_CONFIG_SCALE_PRNU_xxx */
        prnudsnu_config->scale_prnu_color[i] = scale_prnu_color;

        /* valid values are 0..11 */
        prnudsnu_config->scale_dsnu_color[i] = scale_dsnu_color;
    }

    prnudsnu_config->pscale = 0;
    prnudsnu_config->pmult = 0;
    prnudsnu_config->poffset = 0x8000;

    prnudsnu_config->dmult = 0;
    prnudsnu_config->dscale = 0;
    prnudsnu_config->doffset = 0;
}

static void random_prnudsnu( struct pic_prnudsnu_config *prnudsnu_config, uint32_t *prnu, uint32_t *dsnu )
{
    int prnu_bits, dsnu_bits;

    prnu_bits = prnudsnu_config->total_bits/2 + prnudsnu_config->extra_prnu_bits[0];
    dsnu_bits = prnudsnu_config->total_bits/2 - prnudsnu_config->extra_prnu_bits[0];

    *prnu = (uint32_t)(lehmer_frandom() * (1<<prnu_bits));
    *dsnu = (uint32_t)(lehmer_frandom() * (1<<dsnu_bits));
}

/**
 * \brief A nesting place for simple one-shot tests for specific settings.
 *
 * Useful for those settings that failed in the past.
 *
 * \author David Poole
 * \date Mar-2010
 *
 */

static void simple_prnudnsu_tests( void )
{
    struct pic_prnudsnu_config prnudsnu_config;
    scan_cmode_t cmode;
    int retcode, pixels_per_row;
    uint32_t prnu, dsnu;
    uint16_t pixel_in;
    uint32_t pmult, pscale;
    uint32_t dmult, dscale;
    uint8_t quad;

    cmode = SCAN_CMODE_MONO;
    pixels_per_row = 1024 * 5;

//    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8, 0 );
    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 0 );
//    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL2, 0 );
//    prnu = 10;
//    dsnu = 10;
    prnu = 0x1d7;
    dsnu = 0x104;
    quad = 51;
    /* negative quad */
    quad |= 0x80;
//    prnu = 1<<10; 
//    dsnu = 0; 
//    pixel_in = 1024;
//    random_prnudsnu( &prnudsnu_config, &prnu, &dsnu );
    pixel_in = 4100;
    prnudsnu_config.pmult = 4;
    prnudsnu_config.pscale = 1;
    prnudsnu_config.poffset = 0;

//    prnudsnu_config.dmult = 0;
    prnudsnu_config.dmult = 2;
    prnudsnu_config.dscale = 0;
    prnudsnu_config.doffset = 0;

    prnudsnu_config.quad_SF1 = 111;
    prnudsnu_config.quad_SF2 = 55;

    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                    quad, pixel_in, &prnudsnu_config );
    XASSERT( retcode==0, retcode );

    /* davep 18-Mar-2011 ; bail now */
    ASSERT(0);
    return;

    for( dmult=0 ; dmult<10 ; dmult++ ) {
        for( dscale=0 ; dscale<8 ; dscale++ ) {
            pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8, 0 );
            random_prnudsnu( &prnudsnu_config, &prnu, &dsnu );
            pixel_in = 41000;
            prnudsnu_config.dmult = dmult;
            prnudsnu_config.dscale = dscale;
            retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                            quad, pixel_in, &prnudsnu_config );
            XASSERT( retcode==0, retcode );

            /* davep 09-Mar-2011 ; XXX stop here */
            ASSERT(0);

        }
    }

    for( pmult=0 ; pmult<10 ; pmult++ ) {
        pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8, 0 );
        random_prnudsnu( &prnudsnu_config, &prnu, &dsnu );
        pixel_in = 41000;
        prnudsnu_config.pmult = pmult;
        retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                        quad, pixel_in, &prnudsnu_config );
        XASSERT( retcode==0, retcode );
    }

    for( pmult=0 ; pmult<10 ; pmult++ ) {
        for( pscale=0 ; pscale<8 ; pscale++ ) {
            pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8, 0 );
            random_prnudsnu( &prnudsnu_config, &prnu, &dsnu );
            pixel_in = 41000;
            prnudsnu_config.pmult = pmult;
            prnudsnu_config.pscale = pscale;
            retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                            quad, pixel_in, &prnudsnu_config );
            XASSERT( retcode==0, retcode );
        }
    }

    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 11 );
//    pic_prnudsnu_config_init( &prnudsnu_config, 24, -2, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 11 );
    prnu = 1024;
    dsnu = 27;
    pixel_in = 1477;
    prnudsnu_config.pmult = 2;
    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                    quad, pixel_in, &prnudsnu_config );
    XASSERT( retcode==0, retcode );

    /* davep 30-Apr-2010 ; Note I'm using hardwired value for scale_prnu. The
     * scale_prnu==3 has two different meanings depending on the ASIC. Added
     * this magic number test to catch cases where HAVE_ICE_PRNUDSNU_MUL8 isn't
     * set correctly. (See scanconfig.h)
     */
    /* davep 30-Apr-2010 ; MUL8 known failure! */
//    pic_prnudsnu_config_init( &prnudsnu_config, 12, -2, 3, 0 );
//    prnu = 14;
//    dsnu = 14;
//    pixel_in = 64168;
//    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
//                                    pixel_in, &prnudsnu_config );
//    XASSERT( retcode==0, retcode );
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 26-Feb-2008
 *
 */

void pictest_prnudsnu( void )
{
    int i;
    scan_cmode_t cmode;
    int pixels_per_row;
    struct pic_prnudsnu_config prnudsnu_config;
    int scale_dsnu;
    int prnu_bits, dsnu_bits;
    uint32_t prnu_mask, dsnu_mask;
    int retcode;
    uint32_t prnu, dsnu;
    uint16_t pixel_in;
    struct timespec curr_time;
    uint8_t quad;

    /* quiet warnings while have parts of the code below commented out */
    prnu_bits = dsnu_bits = 0;
    prnu_mask = dsnu_mask = 0;
    scale_dsnu = 0;
    quad = 0;

    memset( &pixel_stats, 0, sizeof(struct pixel_stats) );

    clock_gettime( CLOCK_REALTIME, &curr_time ); 
    lehmer_srandom( curr_time.tv_nsec );

    /* temp turn off while working on new prnu/dsnu test */
#if 0
    prnudsnu_encode_test( 1024 );
    prnudsnu_encode_test( 999 );
    prnudsnu_encode_test( 10240 );
    prnudsnu_encode_test( 777 );
    prnudsnu_encode_test( 7919 );
#endif

    simple_prnudnsu_tests();

    /* davep 03-Mar-2011 ; stop here */
    print_pixel_stats( &pixel_stats );
//    ASSERT(0);

    cmode = SCAN_CMODE_MONO;
    pixels_per_row = 1024 * 5;

    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 0 );
//    pic_prnudsnu_config_init( &prnudsnu_config, 12, -2, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 0 );
    prnu = 14;
    dsnu = 14;
    pixel_in = 64168;
    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                    quad, pixel_in, &prnudsnu_config );
    XASSERT( retcode==0, retcode );

    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL2, 0 );
//    pic_prnudsnu_config_init( &prnudsnu_config, 12, -2, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL2, 0 );
    prnu = 14;
    dsnu = 14;
    pixel_in = 64168;
    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                    quad, pixel_in, &prnudsnu_config );
    XASSERT( retcode==0, retcode );

    /* davep 30-Apr-2010 ; KNOWN FAILURE! */
    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8, 0 );
    prnu = 14;
    dsnu = 14;
    pixel_in = 64168;
    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 0,
                                    pixel_in, &prnudsnu_config );
    XASSERT( retcode==0, retcode );


//    /* davep 25-Feb-2008 ; use the new struct pic_prnudsnu_config */
//    memset( &prnudsnu_config, 0, sizeof(struct pic_prnudsnu_config) );
////    prnudsnu_config.total_bits = 16;
//    prnudsnu_config.total_bits = 24;
//    for( i=0 ; i<3 ; i++ ) {
//        /* valid values are -2,-1,0,1,2 */
//        prnudsnu_config.extra_prnu_bits[i] = 0;
////        prnudsnu_config.extra_prnu_bits[i] = 2;
//
//#ifdef HAVE_ICE_PRNUDSNU_MUL8
//        prnudsnu_config.scale_prnu_color[i] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8;
//#else
//        prnudsnu_config.scale_prnu_color[i] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4;
//#endif
////        prnudsnu_config.scale_prnu_color[i] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL2;
////        prnudsnu_config.scale_prnu_color[i] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_NONE;
////        prnudsnu_config.scale_prnu_color[i] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_DIV2;
//
//        /* valid values are 0..11 */
//        prnudsnu_config.scale_dsnu_color[i] = 0;
////        prnudsnu_config.scale_dsnu_color[i] = 11;
//    }

//    cmode = SCAN_CMODE_COLOR;
    cmode = SCAN_CMODE_MONO;
    pixels_per_row = 1024 * 5;

//    prnu = 1;
//    dsnu = 1;
//    prnu = 0x111;
//    dsnu = 0xddd;
////    pixel_in = 65087;
////    pixel_in = 55087;
////    pixel_in = 45087;
//    pixel_in = 32087;
//
//    dbg1( "%s prnu=%ld dsnu=%ld in=%d\n", __FUNCTION__, 
//                prnu, dsnu, pixel_in ); 
//    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
//                                    pixel_in, &prnudsnu_config );
//    XASSERT( retcode==0, retcode );
//
//    prnudsnu_config.total_bits = 24;
//    prnudsnu_config.extra_prnu_bits[0] = -2;
//    pixel_in = 0;
//    prnu = 0;
//    dsnu = 16300;
//    dbg1( "%s prnu=%ld dsnu=%ld in=%d\n", __FUNCTION__, 
//                prnu, dsnu, pixel_in ); 
//    retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
//                                    pixel_in, &prnudsnu_config );
//    XASSERT( retcode==0, retcode );

    /* run a simple test across DSNU values */
//    pic_prnudsnu_config_init( &prnudsnu_config, 12, -2, 3, 0 );
    pic_prnudsnu_config_init( &prnudsnu_config, 24, 0, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 11 );
//    pic_prnudsnu_config_init( &prnudsnu_config, 24, -2, PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4, 11 );
    pixel_in = 1234;
    pixel_in = 40113;
    prnu = 1;
    for( i=0 ; i<=255 ; i++ )  {
        for( scale_dsnu=0 ; scale_dsnu<=11 ; scale_dsnu++ ) {
            dsnu = i;
            prnudsnu_config.scale_dsnu_color[0] = scale_dsnu;
            retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu, 
                                            quad, pixel_in, &prnudsnu_config );
            XASSERT( retcode==0, retcode );
            dbg1( "%s prnu=%ld dsnu=%ld scale_dsnu=%d in=%d\n", 
                        __FUNCTION__, prnu, dsnu, scale_dsnu, pixel_in );
        }
    }

    /* temp turn off while working on new prnu/dsnu test */
    pixel_in = 1000;
    dsnu = 0;
    prnudsnu_config.scale_prnu_color[0] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4;
    prnudsnu_config.scale_prnu_color[1] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4;
    prnudsnu_config.scale_prnu_color[2] = PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4;

    prnu_bits = prnudsnu_config.total_bits/2 + prnudsnu_config.extra_prnu_bits[0];
    dsnu_bits = prnudsnu_config.total_bits/2 - prnudsnu_config.extra_prnu_bits[0];
    prnu_mask = (1<<prnu_bits)-1;
    dsnu_mask = (1<<dsnu_bits)-1;

//    for( prnu=1 ; ; prnu++ )  {
    for( prnu=0 ; ; prnu++ )  {

        /* do we still have enough bits for this prnu? */
        if( (prnu & ~prnu_mask) != 0 ) {
            break;
        }

        /* playin' */
        dsnu = prnu & dsnu_mask;

        retcode = run_prnudsnu_test( cmode, pixels_per_row, prnu, dsnu,
                                        quad, pixel_in, &prnudsnu_config );

        XASSERT( retcode==0, retcode );
        dbg1( "%s prnu=%ld dsnu=%ld in=%d\n", __FUNCTION__, prnu, dsnu,
                    pixel_in ); 
    }

    /* davep 14-Oct-2008 ; turn on to run HUGE tests (~one hour) */
//    exhaustive_prnu_test( cmode, pixels_per_row );
}

void
pictest_prnudsnu_unused( void )
{
    /* don't call this function */
    XASSERT(0,0);
    while(1) {}

    /* call unused functions here to shut up the *&@#$@#%@!!! warnings */
    prnudsnu_encode_test(0);
    exhaustive_prnu_test(0,0);
    prnudsnu_correct_pixel( 0, 0, 0, NULL );
    prnudsnuquad_correct_pixel( 0, 0, 0, 0, NULL );
}

