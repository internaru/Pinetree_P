/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/*
 * Stub containing functions that run tests for code we haven't yet ported into
 * linux kernel.
 *
 */

#include "scos.h"

#include "scantypes.h"
#include "scanvars.h"
#include "scantask.h"
#include "scantasktest.h"
#include "scands.h"
#include "scandstest.h"

scan_err_t start_scantask_test( void )
{
    /* TODO - integrate scantasktest.c into linux kernel code */
    return SCANERR_NOT_IMPLEMENTED;
}

scan_err_t scands_test( void )
{
    /* TODO - integrate scands.c into linux kernel code */
    return SCANERR_NOT_IMPLEMENTED;
}

void scanplat_run_test(uint32_t param2, void *param3)
{
    /* empty */
}

