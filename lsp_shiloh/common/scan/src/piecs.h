/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/*
 *  PIE ColorShift (CS) - CIS color fringing removal via shifting rows together. 
 *
 */

#ifndef PIECS_H
#define PIECS_H

#define PIE_COLORSHIFT_CONFIG_ROUND_MODE_TRUNC  0
#define PIE_COLORSHIFT_CONFIG_ROUND_MODE_FIXED  1
#define PIE_COLORSHIFT_CONFIG_ROUND_MODE_RANDOM 2

#define PIE_COLORSHIFT_CONFIG_LINE_MODE_TWO   1
#define PIE_COLORSHIFT_CONFIG_LINE_MODE_THREE 0

#define PIE_COLORSHIFT_MMODE_LINE_BELOW 0
#define PIE_COLORSHIFT_MMODE_LINE_ABOVE 1
#define PIE_COLORSHIFT_MMODE_LINE_BOTH  2

#ifdef HAVE_PIE_COLORSHIFT
void pie_cs_reset( void );
void pie_cs_setup( uint8_t strip_size, uint32_t round_mode, 
                    const uint32_t mmode[3], const uint32_t mult[3] );
void pie_cs_set_bypass( bool bypass );
void pie_cs_dump( void );
#else
/* empty functions so we don't have to ugly up a lot of code */
static inline void pie_cs_reset(void )  {}
static inline void pie_cs_setup( uint8_t strip_size, uint32_t round_mode, 
                                    const uint32_t mmode[3], const uint32_t mult[3] )
{
    /* davep 12-Apr-2013 ; die if we we don't have colorshift in this hardware */
    ASSERT(0);
}
static inline void pie_cs_set_bypass( bool bypass ) {}
static inline void pie_cs_dump( void ) {}
#endif

#endif

