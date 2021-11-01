/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 *  PIE ColorShift (CS) - CIS color fringing removal via shifting rows together. 
 *
 *  Just enough to turn it on/off.
 *  davep 22-Feb-2011
 *
 *  Up and running, I hope.
 *  davep 20-May-2011
 *
 */
 
#include "scos.h"

#include "regAddrs.h"
#include "debug.h"
#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "piehw.h"
#include "piecs.h"
#include "COLOR_SHIFT_regstructs.h"
#include "COLOR_SHIFT_regmasks.h"

static volatile COLOR_SHIFT_REGS_t* const cs_regs = (volatile COLOR_SHIFT_REGS_t *)(ICE_PIE_COLORSHIFT_BASE);

void pie_cs_dump( void )
{
    dbg2( "CCSCR=%#lx CCSMMR=%#lx CCSMWR=%#lx CCSLSSR=%#lx CCSLSR=%#lx CCSRSR=%#lx\n", 
                cs_regs->CCSCR, cs_regs->CCSMMR, cs_regs->CCSMWR,
                cs_regs->CCSLSSR, cs_regs->CCSLSR, cs_regs->CCSRSR );
}

void pie_cs_set_bypass( bool bypass ) 
{
    if( bypass ) {
        cs_regs->CCSCR |= COLOR_SHIFT_CCSCR_BYPASS_MASK;
    }
    else {
        cs_regs->CCSCR &= ~COLOR_SHIFT_CCSCR_BYPASS_MASK;
    }
}

void pie_cs_setup( uint8_t strip_size, uint32_t round_mode, 
                    const uint32_t mmode[3], const uint32_t mult[3]  )
{
    int i;

    /* sanity checks (using magic numbers, for shame!)  */
    XASSERT( round_mode < 3, round_mode );
    for( i=0 ; i<3 ; i++ ) {
        XASSERT( mmode[i] < 3, mmode[i] );
//        XASSERT( mult[i] < 3, mult[i] );
    }

    cs_regs->CCSCR = COLOR_SHIFT_CCSCR_COLHEIGHT_REPLACE_VAL( cs_regs->CCSCR, strip_size );

    /* davep 05-Nov-2012 ; adding round_mode, mmode, mult as parameters instead
     * of hardwiring into the code
     */

    /* Doug says use three line mode. So we use three line mode */
    cs_regs->CCSCR = COLOR_SHIFT_CCSCR_TWOLINE_REPLACE_VAL(cs_regs->CCSCR,PIE_COLORSHIFT_CONFIG_LINE_MODE_THREE);
    
    cs_regs->CCSCR = COLOR_SHIFT_CCSCR_ROUND_REPLACE_VAL( cs_regs->CCSCR, round_mode );
//    /* use fixed rounding for now */
//    cs_regs->CCSCR = COLOR_SHIFT_CCSCR_ROUND_REPLACE_VAL( cs_regs->CCSCR,PIE_COLORSHIFT_CONFIG_ROUND_MODE_FIXED );

    cs_regs->CCSMMR = COLOR_SHIFT_CCSMMR_RED_REPLACE_VAL( 0, mmode[0] )
                    | COLOR_SHIFT_CCSMMR_GREEN_REPLACE_VAL( 0, mmode[1] )
                    | COLOR_SHIFT_CCSMMR_BLUE_REPLACE_VAL( 0, mmode[2] )
                    ;
//    /* red below, green both, blue above */
//    cs_regs->CCSMMR = COLOR_SHIFT_CCSMMR_RED_REPLACE_VAL( 0, PIE_COLORSHIFT_MMODE_LINE_BELOW )
//                    | COLOR_SHIFT_CCSMMR_GREEN_REPLACE_VAL( 0, PIE_COLORSHIFT_MMODE_LINE_BOTH )
//                    | COLOR_SHIFT_CCSMMR_BLUE_REPLACE_VAL( 0, PIE_COLORSHIFT_MMODE_LINE_ABOVE )
//                    ;

    cs_regs->CCSMWR = COLOR_SHIFT_CCSMWR_RED_REPLACE_VAL( 0, mult[0] )
                    | COLOR_SHIFT_CCSMWR_GREEN_REPLACE_VAL( 0, mult[1] )
                    | COLOR_SHIFT_CCSMWR_BLUE_REPLACE_VAL( 0, mult[2] )
                    ;
//    /* set multipler to 11 on all three channels (1/3 each channel or 32/3~=11) */
//    cs_regs->CCSMWR = COLOR_SHIFT_CCSMWR_RED_REPLACE_VAL( 0, 11 )
//                    | COLOR_SHIFT_CCSMWR_GREEN_REPLACE_VAL( 0, 11 )
//                    | COLOR_SHIFT_CCSMWR_BLUE_REPLACE_VAL( 0, 11 )
//                    ;
}

void pie_cs_reset( void )
{
    pie_cs_set_bypass( true );

    /* restore to documented power-on defaults (values as of 20may2011) */
    cs_regs->CCSCR = COLOR_SHIFT_CCSCR_COLHEIGHT_REPLACE_VAL(0,8)
                   | COLOR_SHIFT_CCSCR_ROUND_REPLACE_VAL(0,PIE_COLORSHIFT_CONFIG_ROUND_MODE_TRUNC)
                   | COLOR_SHIFT_CCSCR_TWOLINE_REPLACE_VAL(0,PIE_COLORSHIFT_CONFIG_LINE_MODE_TWO)
                   | COLOR_SHIFT_CCSCR_BYPASS_REPLACE_VAL(0,1)
                   ;
    cs_regs->CCSMMR = COLOR_SHIFT_CCSMMR_BLUE_REPLACE_VAL(0,PIE_COLORSHIFT_MMODE_LINE_ABOVE)
                    | COLOR_SHIFT_CCSMMR_GREEN_REPLACE_VAL(0,PIE_COLORSHIFT_MMODE_LINE_ABOVE)
                    | COLOR_SHIFT_CCSMMR_RED_REPLACE_VAL(0,PIE_COLORSHIFT_MMODE_LINE_ABOVE)
                    ; 
    cs_regs->CCSMWR = 0;

    /* XXX write only register; should I be touching this? need to read the
     * documentation some more
     */
    cs_regs->CCSLSSR = 0;
}

