/** 
 *
 * ============================================================================
 * Copyright (c) 2006-2008   Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan application header file. This file defines the data structures shared
 * with the host software for scan-to-host.
 *
 **/

#ifndef SCANAPP_H
#define SCANAPP_H

#include "scanapp_observers_api.h"

extern void scanapp_log_event( scanapp_event_t evt );
void dither_buffer( uint8_t *buf, uint32_t pixels_per_row, uint32_t rows_per_buffer );
void scanapp_merge_adf_roller_move(int direction);

typedef enum {
	PC_SCAN_MERGEJOB_NONE = 0,
	PC_SCAN_MERGEJOB_A3,
	PC_SCAN_MERGEJOB_11X17,
	PC_SCAN_MERGEJOB_LEGAL_1ST_PAGE,
	PC_SCAN_MERGEJOB_LEGAL_2ND_PAGE
} PC_SCAN_ACTION_JOB_TYPE;

#endif //SCANAPP_H
