/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file aclreadstatus.c
 * \brief Read the status
*/

#include <string.h>

#include "dprintf.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "acl_api.h"
#include "error_types.h"

#define ACLCMD_STATUS              0x0002

#pragma pack(1)
typedef struct  // 16 bytes
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;    // length of 6
    uint32_t status;
    uint8_t temp[6];
} getStatusCmd_t;

#pragma pack(0)
static UINT32 BootStatus=0;

/* FUNCTION NAME: AclRetStatus */

/**
 * \brief Return the status of system, currently returns 0
 *
 * \param[in] Pipe  IO Pipe for reporting results to the host
 *
 * \param[in] AclCmd The command block that caused this to be called.
 *
 * \param reserved Save for later
 *
 * \retval SINT32 0 success, 1 failure.
 *
 * \date 2/27/2006
 *
 **/
error_type_t AclRetStatus(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    getStatusCmd_t *Buffer;

    Buffer = (getStatusCmd_t *) construct_acl_response_buffer(ACLCMD_STATUS);

    Buffer->status = BootStatus;
    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);
    return OK;
}

error_type_t AclSetStatus(UINT32 Status)
{
    BootStatus = Status;
    return OK;
}

