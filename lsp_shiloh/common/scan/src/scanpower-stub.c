/** 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *   Low power stub
 *
 **/

#include <stdint.h>
#include <stdbool.h>


#include "scantypes.h"
#include "scanpower.h"

scan_err_t scanpower_onetime_init(void)
{
    /* no-op */
    return SCANERR_NONE;
}

scan_err_t scanpower_up(void)
{
    return SCANERR_NONE;
}

scan_err_t scanpower_down(void)
{
    return SCANERR_NONE;
}

bool scanpower_sleeping(void)
{
    return false;
}

void scanpower_scanning_is_active( bool is_active ) 
{
    /* no-op */
}

