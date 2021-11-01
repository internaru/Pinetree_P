/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Agilent Technologies, Inc. All Rights Reserved
 *                      
 *                         Agilent Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/
/* ========================================================================
 * Portions of the materials and information (Materials) provided herein 
 * are distributed by Marvell under license from Agilent Technologies.
 * Use of the Materials is subject to the terms of the license agreement
 * which accompanied the Materials.
 * ========================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                       Marvell Confidential
 * ========================================================================
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef STANDALONE
/* build as part of command line utility */
#include <stdio.h>
#define dbg1 printf 
#define dbg2 printf 
#else
/* build as part of firmware */
#include "scos.h"
#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "piehw.h"
#include "pie.h"
#endif

#include "strippr.h"

/* 21-May-05 davep ;  translated from Won Rhee's Perl model, turned into a
 *                    reentrant state machine
 */
/* 27-June-06 davep ; merging in ErickP's linear mode changes */

/* Note: this file also must be buildable as a standalone program for running
 * strip predictions from the command line.
 */

#define NORMAL (1<<16)

static int STRIPDEBUG=0;

void strip_start( struct predict_state *s, 
                  int strip_height, 
                  int y_numerator,
                  int y_denominator, 
                  int fracY,
                  int mode ) 
{
    int edge_pixels; 

    memset( s, 0, sizeof(struct predict_state) );

    if( mode==STRIPPR_MODE_LINEAR ) {
        s->linear = 1;

        edge_pixels = pie_xy_calc_linear_mode_edge_pixels( y_numerator, y_denominator );

//        s->R = 0;
        s->R = edge_pixels * fracY - 32768 + fracY/2;

        dbg2( "strip predictor remainder initialized to %d\n", s->R );
    }
    else {
        s->R = NORMAL;
    }

    s->strip_height = strip_height;
    s->fracY = fracY;
}

void strip_next( struct predict_state *s, int strip[] )
{
    int i;
    int R;
    int nextR;
    int iPtr;
    int oPtr;
    int lastInputShipped;
    /* convenience variables; these never change */
    const int fracY = s->fracY;
    const int strip_height = s->strip_height;
    int start_i;

    /* restore state */
    iPtr = s->iPtr;
    oPtr = s->oPtr;
    lastInputShipped = s->lastInputShipped;
    R = s->R;

    /* convenience variables; these never change */
//    fracY = s->fracY;
//    strip_height = s->strip_height;

    /* we would iterate over strips starting here */

        // [0] - input strip start row
        // [1] - input strip end row
        // [2] - output strip start row
        // [3] - output strip end row
        strip[0] = iPtr;
        strip[1] = iPtr + strip_height - 1;
        strip[2] = -1;
        strip[3] = -1;

        if( s->linear ) {
            start_i = 1;
        }
        else {
            start_i = 0;
        }

        // #-- iterate down the input pixels in this strip
        for( i=start_i ; i<strip_height ; ) { 

            if( STRIPDEBUG ) {
                dbg1( "..input row %d+%d  R=%d\n", iPtr, i, R );
            }

            nextR = R;

            if( !s->linear ) {
                if( R-fracY <= 0 ) { 
                    // #- ship.
                    if( STRIPDEBUG ) {
                        dbg1( " ..ship output line %d: input row = %d: R=%d",
                                    oPtr, i, R );
                        if( R-fracY == 0 ) {
                            dbg1( " (perfect)" );
                        }
                        dbg1( "\n" );
                    }

                    if( strip[2] == -1 ) {
                        // this is the first time we've seen this output strip;
                        // mark its start
                        strip[2]=oPtr;
                    }

                    strip[3]=oPtr;
                    oPtr++;
                    nextR += NORMAL;
                    lastInputShipped = i + (R-fracY == 0);
                }

                if( R-fracY >= 0 ) {  
                    // #- next input line
                    if( STRIPDEBUG ) {
                        dbg1( "..retire input line %d+%d\n", iPtr, i );
                    }
                    nextR -= fracY; 
                    /* new line in new predictor is actually:
                     *  $i+= 1/(1<<$prescale);
                     * but I'm not using prescale so leave alone.
                     */
                    i++;
                }
            }
            else {
                /* linear mode */
                if (fracY < NORMAL) { // # enlarging
                    if( STRIPDEBUG ) {
                        dbg1( " ..ship output line %d: input row = %d: R=%d\n",
                                    oPtr, i, R ); 
                    }
                    if( strip[2]==-1 ) {
                        strip[2]=oPtr;
                    }
                    strip[3]=oPtr;
                    oPtr++;
                    nextR += fracY; 
                    if (nextR > NORMAL) {
                        nextR -= NORMAL;
                        if( STRIPDEBUG ) {
                            dbg1( "..retire input line %d\n", iPtr+i );
                        }
                        i++;
                    }
                } else { // # reducing
                    if (R <= NORMAL) {  // # output a line
                        if( STRIPDEBUG ) {
                            dbg1( " ..ship output line %d: input row = %d: R=%d\n", oPtr, i, R );
                        }
                        if( strip[2]==-1 ) {
                            strip[2]=oPtr;
                        }
                        strip[3]=oPtr;
                        oPtr++;
                        nextR += (fracY - NORMAL);
                    } else {
                        nextR -= NORMAL;
                    }
                    if( STRIPDEBUG ) {
                        dbg1(  "..retire input line %d+%d\n", iPtr, i );
                    }
                    i++;
                }
            }

            R = nextR;
        }

        /* don't need to save R in strip[] (leave commented out to keep
         * matching the perl predictor)
         */
//        strip[4] = R;
        if (s->linear) {
            iPtr += (strip_height - 1);
        } else {
            iPtr += lastInputShipped;
            R += (fracY * (i-lastInputShipped));
        }

    /* end of iterating over strips */

    /* save state */
    s->iPtr = iPtr;
    s->oPtr = oPtr;
    s->lastInputShipped = lastInputShipped;
    s->R = R;
}

#if 0
void strip_predict_sm( int num_steps, int strip_height, int fracY, int mode )
{
    struct predict_state state;
    struct predict_state state2;
    struct predict_state state3;
    int sPtr;
    int strip[4];
    int strip2[4];
    int strip3[4];

    strip_start( &state, strip_height, fracY, mode );
//    strip_start( &state2, 16, 65536 );
    strip_start( &state2, strip_height+1, fracY, mode );
    strip_start( &state3, strip_height+2, fracY, mode );

    for( sPtr=0 ; sPtr<num_steps ; sPtr++ ) {
        strip_next( &state, strip );
        strip_next( &state2, strip2 );
        strip_next( &state3, strip3 );

        dbg1( "Strip %d: Input Rows: [%d,%d]", sPtr, strip[0], strip[1] );
        dbg1( "  Output Rows: [%d,%d]", strip[2], strip[3] );
        dbg1( "=%d\n", strip[3]-strip[2]+1 );

//        dbg1( "Strip %d: Input Rows: [%d,%d]", sPtr, strip2[0], strip2[1] );
//        dbg1( "  Output Rows: [%d,%d]", strip2[2], strip2[3] );
//        dbg1( "=%d\n", strip2[3]-strip2[2]+1 );
//
//        dbg1( "Strip %d: Input Rows: [%d,%d]", sPtr, strip3[0], strip3[1] );
//        dbg1( "  Output Rows: [%d,%d]", strip3[2], strip3[3] );
//        dbg1( "=%d\n", strip3[3]-strip3[2]+1 );
    }
    
}
#endif

#if 0
void strip_predict( int num_steps, int strip_height, int fracY )
{
    int i;
    int R;
    int nextR;
    int sPtr;
    int iPtr;
    int oPtr;
    int lastInputShipped;
    int strip[4];

//    dbg1( "%s -ns=%d -sh=%d -fracY=%d\n", __FUNCTION__,
//            num_steps, strip_height, fracY );

    sPtr = 0;  
    iPtr = 0; 
    oPtr = 0;
    lastInputShipped = 0;

    R = initY;

    // #-- iterate down the rows of the image to compute strip boundaries
    for (sPtr =0; sPtr < num_steps; sPtr++) {
        if( STRIPDEBUG ) {
            dbg1( "Working on Strip %d -- iPtr=%d\n", sPtr, iPtr ); 
        }
        
        // [0] - input strip start row
        // [1] - input strip end row
        // [2] - output strip start row
        // [3] - output strip end row
        strip[0] = iPtr;
        strip[1] = iPtr+strip_height-1;
        strip[2] = -1;
        strip[3] = -1;

        // #-- iterate down the input pixels in this stripe
        for( i=0 ; i<strip_height ; ) { 

            if( STRIPDEBUG ) {
                dbg1( "..input row %d+%d  R=%d\n", iPtr, i, R );
            }

            nextR = R;

            if( R-fracY <= 0 ) { 
                // #- ship.
                if( STRIPDEBUG ) {
                    dbg1 (( " ..ship output line %d: input row = %d: R=%d",
                                oPtr, i, R );
                    if (R-fracY == 0 ) {
                        dbg1( " (perfect)" );
                    }
                    dbg1( "\n" );
                }

                if( strip[2] == -1 ) {
                    // this is the first time we've seen this output strip;
                    // mark its start
                    strip[2]=oPtr;
                }

                strip[3]=oPtr;
                oPtr++;
                nextR += (1<<16);
                lastInputShipped = i + (R-fracY == 0);
            }

            if( R-fracY >= 0 ) {  
                // #- next input line
                if( STRIPDEBUG ) {
                    dbg1( "..retire input line %d+%d\n", iPtr, i );
                }
                nextR -= fracY; 
                i++;
            }

            R = nextR;
        }

        iPtr += lastInputShipped;
        R += fracY * (i-lastInputShipped);

        dbg1( "Strip %d: Input Rows: [%d,%d]", sPtr, strip[0], strip[1] );
        dbg1( "  Output Rows: [%d,%d]", strip[2], strip[3] );
        dbg1( "=%d\n", strip[3]-strip[2]+1 );
    }

}
#endif

