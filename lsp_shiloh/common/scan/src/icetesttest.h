/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ICETESTTEST_H
#define ICETESTTEST_H

scan_err_t icetest_test( void );
void icetest_test_buffer_init( scan_cmode_t cmode, uint8_t *buf, int buflen_bytes, int pixels_per_row, int num_rows );

#endif

