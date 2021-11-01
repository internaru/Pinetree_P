/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* 
 * \file aclgetversion.c
 * \brief Return the version of the firmware.
*/

#include <string.h>
#include <stdint.h>

#include "dprintf.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "acl_api.h"
#include "platform_api.h"
#include "fwVersion.h"
#include "ioutils.h"


#define ACLCMD_GET_VERSION          0x0001
#define LEN_ACL_CODEVERSION         30

#pragma pack(1)
typedef struct      // 16 bytes long, check
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    uint8_t version[LEN_ACL_CODEVERSION];
    uint16_t productID;
} getVersionCmd_t;

#pragma pack()
/* FUNCTION NAME: AclGetVersion */

/**
 * \brief Return the fw version to the host
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
error_type_t AclGetVersion(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    getVersionCmd_t *Buffer;
    uint16_t  usbProdID;

    Buffer = (getVersionCmd_t *) construct_acl_response_buffer(be16_to_cpu(AclCmd->cmd_id));
    platvars_get_usb_pid(&usbProdID);
    // date code is all 0's in bootcode since host firmware has not been downloaded yet
    // for testing, can put memcpy back in for testing, but it should NOT be in releae code.
    memcpy(&Buffer->version, FIRMWARE_VERSION_STRING, LEN_ACL_CODEVERSION);
    Buffer->productID = cpu_to_be16(usbProdID);
    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Sent Version 16 bytes written\n"));
    return OK;
}

