/**
* (c) Copyright 2006-2008 Marvell International Ltd.
*
*               Marvell Confidential
* \file aclget1284.c
 * \brief Return the 1284 string.
*/

#include "agConnectMgr.h"
#include "acl_api.h"
#include "error_types.h"
#include "platform_api.h"
#include "string.h"

#define ACLCMD_GET_ASICID          0x0004

#pragma pack(1)
typedef struct
{
    ACL_RESPONSE_BASE_CLASS_MEMBERS;
    uint32_t ASICID;
    uint8_t temp[6];
} getASICIDResp_t;

#pragma pack(0)
/* FUNCTION NAME: AclGet1284String */

/**
 * \brief Return the USB 1284 string to the host.
 * The 1284 string is defined in the usb spec for printers.  This command
 * returns the string to the host.
 *
 * \param[in] Pipe  IO Pipe for reporting results to the host
 *
 * \param[in] AclCmd The command block that caused this to be called.
 *
 * \param reserved Save for later
 *
 * \retval error_type_t  Always returns OK
 *
 **/
error_type_t AclGetASICID(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data)
{
    getASICIDResp_t *Buffer;
    uint32_t asic_id;

    Buffer = (getASICIDResp_t *) construct_acl_response_buffer(ACLCMD_GET_ASICID);

	plat_get_asic_id( &asic_id );
    memcpy(&Buffer->ASICID,&asic_id, 4); // do a 32 bit value

    Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);

    return OK;
}

