/** 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *   This file used as a header for our low power setup
 *
 **/

#ifndef SCANPOWER_H
#define SCANPOWER_H

scan_err_t scanpower_onetime_init(void);

scan_err_t scanpower_up(void);
scan_err_t scanpower_down(void);
void scanpower_scanning_is_active( bool is_active );
bool scanpower_sleeping(void);
#endif // SCANPOWER_H

