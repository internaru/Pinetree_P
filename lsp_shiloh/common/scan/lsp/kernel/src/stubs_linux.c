/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "lassert.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "afe.h"
#include "scanvars.h"
#include "smirb.h"

//struct scanvars sv;

long int get_queue_pending( mqd_t msgq )
{
    /* TODO */
    BUG();
    return 0;
}

scan_err_t scanman_dev_request( uint32_t request, int arg, void *ptr, uint32_t *request_id )
{
    /* TODO */
    BUG();
    return SCANERR_NOT_IMPLEMENTED;
}

void scanman_smirb_sanity( struct scanman_ioctl_request_block *smirb )
{
    /* TODO */
    BUG();
}

