/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file picbm2005.c
 *
 * \brief  PIC Bulb Monitor, circa 2005 design.
 *
 * Margins belong to BM in ASICs using this file. Other ASICs have a standalone
 * margin block(s).
 *
 * davep 20-Mar-2013 ; updated to RegBuild headers so can support dual scan
 *                      capable ASICs
 *
 **/

#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "pichw.h"
#include "pic.h"
#include "picreset.h"
#include "BMLR_regmasks.h"
#include "BMLR_regstructs.h"

/* davep 20-Mar-2013 ; At some point, the ASIC team renamed the bulb
 * monitor/margin block. Map new name back to the old name. This is why we
 * can't have nice things.
 */
#ifdef BULBMONLRMARGIN_BMCR_BYPASS_MASK 
    #define BMLR_REGS_t BULBMONLRMARGIN_REGS_t
    #define BMLR_BMCR_BYPASS_REPLACE_VAL BULBMONLRMARGIN_BMCR_BYPASS_REPLACE_VAL
    #define BMLR_BMCR_COLORCOUNT_REPLACE_VAL BULBMONLRMARGIN_BMCR_COLORCOUNT_REPLACE_VAL
    #define BMLR_BMCR_LINESTOAVE_REPLACE_VAL BULBMONLRMARGIN_BMCR_LINESTOAVE_REPLACE_VAL
    #define BMLR_BMCR_WINLEN_REPLACE_VAL BULBMONLRMARGIN_BMCR_WINLEN_REPLACE_VAL
    #define BMLR_BMCR_STARTPIX_REPLACE_VAL BULBMONLRMARGIN_BMCR_STARTPIX_REPLACE_VAL
    #define BMLR_LRMR0_RIGHTMARGINC0_REPLACE_VAL BULBMONLRMARGIN_LRMR0_RIGHTMARGINC0_REPLACE_VAL
    #define BMLR_LRMR0_LEFTMARGINC0_REPLACE_VAL BULBMONLRMARGIN_LRMR0_LEFTMARGINC0_REPLACE_VAL
#endif

static volatile BMLR_REGS_t * const bmlr_regs_list[] = {
#ifdef HAVE_NSENSOR_SUPPORT
    (volatile BMLR_REGS_t *)(ICE_PIC_BULBMONLRMARGIN0_BASE),
    (volatile BMLR_REGS_t *)(ICE_PIC_BULBMONLRMARGIN1_BASE),
#else
    (volatile BMLR_REGS_t *)ICE_PIC_BULBMONLRMARGIN_BASE,
#endif
    0 /* end of list */
};

static void reset( volatile BMLR_REGS_t *regs ) 
{
    regs->BMCR  = PIC_BM_BMCR_R; 
    regs->LRMR0 = PIC_BM_LRMR0_R;   
    regs->LRMR1 = PIC_BM_LRMR1_R; 
    regs->LRMR2 = PIC_BM_LRMR2_R; 
    regs->BMR0 = PIC_BM_BMR0_R;
    regs->BMR1 = PIC_BM_BMR1_R;
    regs->BMR2 = PIC_BM_BMR2_R;
    regs->BMR3 = PIC_BM_BMR3_R;
}

static void dump( volatile BMLR_REGS_t *regs )
{
    dbg2( "bmcr=0x%08x lrmr0=0x%08x lrmr1=0x%08x lrmr2=0x%08x\n", 
                regs->BMCR, regs->LRMR0, regs->LRMR1, regs->LRMR2 );
    dbg2( "bmr0=0x%08x  bmr1=0x%08x  bmr2=0x%08x  bmr3=0x%08x\n",
                regs->BMR0, regs->BMR1, regs->BMR2, regs->BMR3 );
}

void pic_bm_reset( void )
{
    int i;

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        reset(bmlr_regs_list[i]);
    }
}

void pic_bm_dump( void )
{
    int i;

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        dump(bmlr_regs_list[i]);
    }
}

//--------------------------------------------------------------------------
// Function     : pic_bm_set_bypass
//   returns    : 
//   arg1       : boolean indicating whether the bulb monitor should be bypassed
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Enables or disables bypass of the Bulb Monitor.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_bm_set_bypass( bool bypass )
{
    int i;
    int flag;
    volatile BMLR_REGS_t *regs;

    /* I have typecast trust issues */
    flag = bypass?1:0;

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->BMCR = BMLR_BMCR_BYPASS_REPLACE_VAL( regs->BMCR, flag );
    }
}


//--------------------------------------------------------------------------
// Function     : pic_bm_get_bypass
//   returns    : boolean indicating whether bulb monitor is bypassed
//   arg1       :
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the status of bulb monitor bypass.
//
// 
// Notes        : 
//
//
//--------------------------------------------------------------------------

bool pic_bm_get_bypass( void )
{
    int i;
    bool any_bypass;
    volatile BMLR_REGS_t *regs;

    any_bypass = false;

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        any_bypass = any_bypass || ((regs->BMCR & PIC_BM_BMCR_BYPASS)==1);
    }

    return any_bypass;
}

//--------------------------------------------------------------------------
// Function     : pic_bm_set_color_counter
//   returns    : 
//   arg1       : boolean indicating color or odd/even
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the Bulb Monitor for 3 colors or odd/even mono data.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
void pic_bm_set_color_counter( bool not_odd_even )
{
    int i;
    volatile BMLR_REGS_t *regs;
    int flag;

    flag = not_odd_even ? 1 : 0;

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->BMCR = BMLR_BMCR_COLORCOUNT_REPLACE_VAL( regs->BMCR, flag );
    }
}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_color_counter
//   returns    : Returns whether Bulb monitor is set up for color or odd/even 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : returns true if the Bulb Monitor is set up for color and false
//                if it's set up for odd/even mono data
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

//bool pic_bm_get_color_counter( void )
//{
//    if (*PIC_BM_BMCR & PIC_BM_BMCR_CC) {
//        return true;
//    }
//    return false;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_set_lines_to_avg
//   returns    : 
//   arg1       : number of lines over which the Bulb Monitor should average
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the number of lines over which the Bulb Monitor should
//                average when it computes the light monitor window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_bm_set_lines_to_avg( unsigned int lines )
{
    int i;
    volatile BMLR_REGS_t *regs;
    
    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        switch (lines) {
        case 1:
            regs->BMCR = BMLR_BMCR_LINESTOAVE_REPLACE_VAL( regs->BMCR, 0 );
            break;
        case 2:
            regs->BMCR = BMLR_BMCR_LINESTOAVE_REPLACE_VAL( regs->BMCR, 1 );
            break;
        case 4:
            regs->BMCR = BMLR_BMCR_LINESTOAVE_REPLACE_VAL( regs->BMCR, 2 );
            break;
        case 8:
            regs->BMCR = BMLR_BMCR_LINESTOAVE_REPLACE_VAL( regs->BMCR, 3 );
            break;
        case 16:
            regs->BMCR = BMLR_BMCR_LINESTOAVE_REPLACE_VAL( regs->BMCR, 4 );
            break;
        default:
            dbg1( "Unsupported number of lines %d", lines);
            XASSERT(0, lines);
            break;
        }

        regs++;
    }
}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_lines_to_avg
//   returns    : number of lines over which the Bulb Monitor is averaging
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the number of lines over which the Bulb Monitor
//                averages when it computes the light monitor window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
unsigned int pic_bm_get_lines_to_avg( void )
{
    int retval;

    retval = -1;
    switch (*PIC_BM_BMCR & PIC_BM_BMCR_NUML(7)) {
    case 0:
        retval = 1;
        break;
    case 1:
        retval = 2;
        break;
    case 2:
        retval = 4;
        break;
    case 3:
        retval = 8;
        break;
    case 4:
        retval = 16;
        break;
    default:
        dbg1( "Unsupported number of lines to average");
        XASSERT(0, *PIC_BM_BMCR & PIC_BM_BMCR_NUML(7));
        break;
    }
    return retval;
}
#endif

//--------------------------------------------------------------------------
// Function     : pic_bm_set_window_size
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the number of pixels (window size) that the Bulb
//                Monitor adds together to compute the light monitor
//                window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_bm_set_window_size( unsigned int numpix )
{
    int i;
    volatile BMLR_REGS_t *regs;


    XASSERT(numpix<=256 && numpix >= 2,numpix);

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->BMCR = BMLR_BMCR_WINLEN_REPLACE_VAL( regs->BMCR, numpix );
    }
}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_window_size
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the number of pixels (window size) that the Bulb
//                Monitor adds together to compute the light monitor
//                window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_window_size( void )
//{
//    return (((*PIC_BM_BMCR & (0xff<<16)) >> 16) & 0xff) - 1;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_set_startpix
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Sets the first pixel used by the Bulb Monitor when it
//                determines the light monitor window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

void pic_bm_set_startpix( unsigned int startpix )
{
    int i;
    volatile BMLR_REGS_t *regs;

    XASSERT(startpix<=65535,startpix);

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->BMCR = BMLR_BMCR_STARTPIX_REPLACE_VAL(regs->BMCR, startpix );
    }
}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_startpix
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the first pixel used by the Bulb Monitor when it
//                determines the light monitor window sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_startpix( void )
//{
//    return *PIC_BM_BMCR & 0xffff;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_sum_col0_mono_even
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the light monitor window sum for the first color
//                or monochrome or the even pixel, depending on what type
//                of imaging sensor is attached.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_sum_col0_mono_even( void )
//{
//    return *PIC_BM_BMR0;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_sum_col1_odd
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the light monitor window sum for the second color
//                or the even mono pixel, depending on what type
//                of imaging sensor is attached.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_sum_col1_odd( void )
//{
//    return *PIC_BM_BMR1;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_sum_col2
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the light monitor window sum for the third color.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_sum_col2( void )
//{
//    return *PIC_BM_BMR2;
//}

//--------------------------------------------------------------------------
// Function     : pic_bm_get_exposure_data
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Returns the exposure correction data. Exposure correction
//                is a measure of how far off the previous exposure was from
//                the target value. This allows us to take the exposure compensation
//                into account when we interpret the bulb monitor sum.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------
//unsigned int pic_bm_get_exposure_data( void )
//{
//    return *PIC_BM_BMR3;
//}


//--------------------------------------------------------------------------
// Function     : pic_marg_set_right_mar_col0_mono_even
//   returns    : 
//   arg1       : 
//   arg2       :
// Created by   : Brad Smith
// Date created : 5/13/05
// Description  : Called to set the right margin for color 0 or mono or the
//                even mono row, depending on the attached sensor.
//
// 
// Notes        : 
//
//--------------------------------------------------------------------------

#if 0
void pic_marg_set_right_mar_col0_mono_even( unsigned int rmar )
{
    XASSERT(rmar >= 4 && rmar <= 65535,rmar);
    *PIC_BM_LRMR0 &= ~PIC_BM_LRMR_RIGHT(-1);
    *PIC_BM_LRMR0 |= PIC_BM_LRMR_RIGHT(rmar);
}
#endif

uint32_t pic_marg_get_right( void )
{
    uint32_t num32;

    /* note: assuming all three margins set to the same values (see
     * pic_marg_set_xxx())
     */
    XASSERT( *PIC_BM_LRMR0==*PIC_BM_LRMR1, *PIC_BM_LRMR1 );
    XASSERT( *PIC_BM_LRMR1==*PIC_BM_LRMR2, *PIC_BM_LRMR2 );

    num32 = *PIC_BM_LRMR0;

    /* TODO add margin masks to pichw.h */
    num32 = (num32>>16) & 0xffff;

    return num32;
}
    
uint32_t pic_marg_get_left( void )
{
    uint32_t num32;

    /* note: assuming all three margins set to the same values (see
     * pic_marg_set_xxx())
     */
    XASSERT( *PIC_BM_LRMR0==*PIC_BM_LRMR1, *PIC_BM_LRMR1 );
    XASSERT( *PIC_BM_LRMR1==*PIC_BM_LRMR2, *PIC_BM_LRMR2 );

    num32 = *PIC_BM_LRMR0;

    /* TODO add margin masks to pichw.h */
    num32 = num32 & 0xffff;

    return num32;
}

/**
 * \brief  set all PIC margins' left position
 *
 * Sets all channels' margins to the same value.
 *
 * Made this to wrap Brad's original functions.  I'm deprecating the original
 * functions since their names are a bit confusing. 
 *
 *
 * NOTE! PIC Margins work in PIXELS, not BYTES!  If you're using 16-bit pixels,
 * the margins will be 2x the size of the PIC WDMA settings. 
 *
 * Remember: Margins work on PIXELS.  PIC WDMA works on BYTES.
 *
 *
 * The Left margin is the first pixel BM will pass, counting from zero.  So if
 * you want to trim the first 8 pixels, set the left margin to 8.  When the
 * pixel counter hits 8, the pixels will pass.
 *
 * The Right margin is the LAST pixel BM will pass, counting from zero. 
 *
 * (I'm pretty sure this is how it works. The PIC BM (Bulb Monitor) document
 * isn't entirely clear.)
 *
 * \author David Poole
 * \date 23-Jan-2008
 *
 */

void pic_marg_set_left( uint32_t left )
{
    int i;
    volatile BMLR_REGS_t *regs;
    
    XASSERT(left<=65535, left);

    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->LRMR0 = BMLR_LRMR0_LEFTMARGINC0_REPLACE_VAL( regs->LRMR0, left );
        regs->LRMR1 = BMLR_LRMR0_LEFTMARGINC0_REPLACE_VAL( regs->LRMR1, left );
        regs->LRMR2 = BMLR_LRMR0_LEFTMARGINC0_REPLACE_VAL( regs->LRMR2, left );
    }
}

/**
 * \brief  set all PIC margins' right position
 *
 * Sets all channels' margins to the same value.
 *
 * Made this to replace Brad's original functions.  I'm deprecating the original
 * functions since their names are a bit confusing. 
 *
 *
 *  For in-depth docs, see pic_marg_set_left().
 *
 * \author David Poole
 * \date 23-Jan-2008
 *
 */

void pic_marg_set_right( uint32_t right )
{
    int i;
    volatile BMLR_REGS_t *regs;
    
    for( i=0 ; bmlr_regs_list[i] ; i++ ) {
        regs = bmlr_regs_list[i];
        regs->LRMR0 = BMLR_LRMR0_RIGHTMARGINC0_REPLACE_VAL( regs->LRMR0, right );
        regs->LRMR1 = BMLR_LRMR0_RIGHTMARGINC0_REPLACE_VAL( regs->LRMR1, right );
        regs->LRMR2 = BMLR_LRMR0_RIGHTMARGINC0_REPLACE_VAL( regs->LRMR2, right );
    }
}

void pic_marg_set_bypass( bool bypass )
{
    pic_bm_set_bypass( bypass );
}

bool pic_marg_get_bypass( void )
{
    return pic_bm_get_bypass();
}

void pic_marg_dump( void )
{
    /* NO-OP ; margins part of Bulb Monitor */
}

void pic_marg_reset( void )
{
    /* NO-OP ; margins part of Bulb Monitor */
}

