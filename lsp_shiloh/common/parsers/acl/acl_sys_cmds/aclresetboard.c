/*
 *
 * ============================================================================
 * Copyright (c) 2007-2009   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file aclresetboard.c
 *
 * \brief Reset the board
 *
 */

#include <stdint.h>
#include "acl_api.h"
#include "hwconfig_api.h"
#include "agConnectMgr.h"

/**
 * \brief Reset the board
 * \param[in] Pipe The io pipe for getting data.
 * \param[in] AclCmd The acl command block
 * \retval If this returns then panic does not work!
 */

int32_t AclResetBoard(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *reserved)
{
    /* Reset the board */
#ifdef __linux__
    //??????? hwPlatformReset();  // reset the entire block
#else
    hwPlatformReset();  // reset the entire block
#endif        

    return 0;
}
