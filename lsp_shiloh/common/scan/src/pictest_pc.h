/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PICTEST_PC_H
#define PICTEST_PC_H

void pictest_prnudsnu( void );
thread_return_t pictest_prnudsnu_feeder( void *input );
void pictest_prnudsnu_load_lut( uint8_t *pc_lut, int datalen );

#endif

