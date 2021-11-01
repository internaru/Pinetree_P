/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef CALCAPLIST
#define CALCAPLIST

scan_err_t cal_cap_list_onetime_init( void );

scan_err_t cal_cap_list_add( uint32_t cal_cap_id );

void cal_cap_list_flush( void );

bool cal_cap_list_contains( uint32_t cal_cap_id );

#endif

