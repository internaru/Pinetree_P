/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file grayline.c
 *
 * \brief functions for linear brightness/contrast 
 *
 * davep 24-Jan-2012
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "grayline.h"

struct polar 
{
    float rho, theta;
};

static void to_polar( struct rectangular *r, struct polar *p )
{
    memset( p, 0, sizeof(struct polar) );
    p->rho = sqrtf( powf( r->x, 2 ) + powf( r->y, 2 ) );
    p->theta = atanf( r->y / r->x );
}

static void to_rect( struct polar *p, struct rectangular *r )
{
    memset( r, 0, sizeof(struct rectangular) );
    r->x = p->rho * cosf( p->theta );
    r->y = p->rho * sinf( p->theta );
}

static void line_midpoint( struct line *line,
                    struct rectangular *midpoint )
{
    memset( midpoint, 0, sizeof(struct rectangular) );
    midpoint->x = (line->p2.x - line->p1.x )/2 + line->p1.x;
    midpoint->y = (line->p2.y - line->p1.y )/2 + line->p1.y;
}

static void point_new( struct rectangular *r, float x, float y )
{
    memset( r, 0, sizeof(struct rectangular) );
    r->x = x;
    r->y = y;
}

static void point_add( struct rectangular *point1, struct rectangular *point2,
                struct rectangular *result )
{
    memset( result, 0, sizeof(struct rectangular) );
    result->x = point1->x + point2->x;
    result->y = point1->y + point2->y;
}

static void point_subtract( struct rectangular *point1, struct rectangular *point2,
                     struct rectangular *result )
{
    memset( result, 0, sizeof(struct rectangular) );
    result->x = point1->x - point2->x;
    result->y = point1->y - point2->y;
}

static void calc_contrast_theta( float float_contrast, float theta_in, float *contrast_theta ) 
{
    float theta2;

    if( float_contrast > 1.0 ) {
        /* increase contrast
         * rotate counterclockwise
         * increase theta (angle) toward vertical
         */
        theta2 = (M_PI_2 - theta_in);
        *contrast_theta = theta_in + theta2 * (float_contrast-1.0);
    }
    else if( float_contrast < 1.0 ) {
        /* decrease contrast
         * rotate clockwise
         * decrease theta (angle) toward horizontal
         */
        theta2 = theta_in;
        *contrast_theta = theta_in - theta2 * (1.0 - float_contrast);
    }
    else {
        *contrast_theta = theta_in;
    }
}

void grayline_new( struct line *line, 
                   float p1_x, float p1_y,
                   float p2_x, float p2_y  )
{
    memset( line, 0, sizeof(struct line) );
    point_new( &line->p1, p1_x, p1_y );
    point_new( &line->p2, p2_x, p2_y );
}

/**
 * \brief Contrast enhance a linear transfer function based on two endpoints.
 *
 * Currently rotates the line around a midpoint. Rotation amount is determined
 * by a fraction of the current line's theta. The contrast is used to determine
 * the rotation amount.
 *
 * \author David Poole
 * \date 24-Jan-2012
 */

void grayline_enhance_contrast( float float_contrast, 
                                struct line *line_in,
                                struct line *line_out )
{
    struct rectangular r_midpoint;
    struct rectangular new_p1, new_p2;
    struct polar pol1, pol2;

    /* There are probably smarter math ways to do this. I wanted to leave the
     * math/code as simple as possible so I can understand it in the future.
     */

    /* find midpoint */
    line_midpoint( line_in, &r_midpoint );

//    dbg2( "midpoint=%f,%f\n", r_midpoint.x, r_midpoint.y );

    /* move midpoint to origin */
    point_subtract( &line_in->p1, &r_midpoint, &new_p1 );
    point_subtract( &line_in->p2, &r_midpoint, &new_p2 );

//    dbg2( "normed=%f,%f %f,%f\n", new_p1.x, new_p1.y, new_p2.x, new_p2.y );

    /* convert to polar notation */
    to_polar( &new_p1, &pol1 );
    to_polar( &new_p2, &pol2 );

//    dbg2( "polar=%f,%f %f,%f\n", pol1.rho, pol1.theta, pol2.rho, pol2.theta);

    float contrast_theta;
    calc_contrast_theta( float_contrast, pol2.theta, &contrast_theta );

    /* rotate line */
    pol1.theta = M_PI + contrast_theta;
    pol2.theta = contrast_theta;

//    dbg2( "polar rotate=%f,%f %f,%f\n", pol1.rho, pol1.theta, pol2.rho, pol2.theta);

    /* 3pi/2 (270 degrees) */
    const float max_p1 = 3*M_PI_2;

    /* clip to vertical / horizontal 
     * XXX the /1000 is to prevent fully vertical or horizontal lines so we can
     * avoid division by zero later
     */
    if( pol1.theta > max_p1 ) {
        pol1.theta = max_p1;
        pol1.theta -= M_PI/1000;
    }
    else if( pol1.theta < M_PI ) {
        pol1.theta = M_PI;
        pol1.theta += M_PI/1000;
    } 
    if( pol2.theta > M_PI_2 ) {
        pol2.theta = M_PI_2;
        pol2.theta -= M_PI/1000;
    }
    else if( pol2.theta < 0 ) {
        pol2.theta = 0;
        pol2.theta += M_PI/1000;
    }

//    dbg2( "polar clipped=%f,%f %f,%f\n", pol1.rho, pol1.theta, pol2.rho, pol2.theta);

    /* convert to rectangular */
    to_rect( &pol1, &new_p1 );
    to_rect( &pol2, &new_p2 );

    /* move new line back to original position */
    point_add( &new_p1, &r_midpoint, &line_out->p1 );
    point_add( &new_p2, &r_midpoint, &line_out->p2 );
}

static void line_equation( float x1, float y1, float x2, float y2, float *line_slope, float *line_b )
{
    XASSERT( y2-y1 != 0, y2 );
    XASSERT( x2-x1 != 0, x2 );

    *line_slope = ((float)(y2-y1))/((float)(x2-x1));
    *line_b = y1 - x1 * (*line_slope);
}

void grayline_to_lut( int num_entries, uint32_t lut[], struct line *line ) 
{
    int i;
    float line_slope, line_b, y;

    line_equation( line->p1.x, line->p1.y, line->p2.x, line->p2.y, 
                    &line_slope, &line_b );

//    dbg2( "%s line m=%f b=%f\n", __FUNCTION__, line_slope, line_b );

    /* XXX hardwired to a 10-bit to 8-bit LUT (TCNS) */

    for( i=0 ; i<num_entries; i++ ) {
        y = line_slope * i + line_b;
        if( y < 0 ) {
            lut[i] = 0;
        }
        else if( y >= 1024 ) {
            lut[i] = 255;
        }
        else {
            lut[i] = (uint32_t)(y/4.0);
        }
    }
}

