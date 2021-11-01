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

#ifndef STRIP_PREDICT_H
#define STRIP_PREDICT_H

struct predict_state {
    /* state machine fields */
    int R;      /* remainder */
    int iPtr;   /* input strip pointer */
    int oPtr;   /* output strip pointer */
    int lastInputShipped;

    /* user parameters; constant across an image */
    int strip_height;
    int fracY;

    /* davep 27-june-06 ; 1==linear, 0==RAPR */
    int linear;
};

/* indices of values in strip[] array as returned by strip_next() */
#define STRIP_READ_FIRST  0
#define STRIP_READ_LAST   1
#define STRIP_WRITE_FIRST 2
#define STRIP_WRITE_LAST  3

#define STRIPPR_MODE_RAPR   1
#define STRIPPR_MODE_LINEAR 2

void strip_start( struct predict_state *s, 
                  int strip_height, 
                  int y_numerator,
                  int y_denominator, 
                  int fracY,
                  int mode );
void strip_next( struct predict_state *s, int strip[] );

#endif

