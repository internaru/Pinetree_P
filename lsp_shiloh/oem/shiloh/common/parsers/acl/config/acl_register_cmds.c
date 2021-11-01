/*
 * ============================================================================
 * Copyright (c) 2006-2008   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
 /**
 * Description:
 *
 *
 * Description:  Register acl cmds..
 *
 *****************************************************************************/

 #include "acl_api.h"
 #include "aclproto.h"

void register_acl_cmds(void)
{

     register_acl_cmd(ACLCMD_GET_VERSION,AclGetVersion,0);
     register_acl_cmd(ACLCMD_STATUS,AclRetStatus, 0);
	 //LSPTODO register_acl_cmd(ACLCMD_DOWNLOAD, AclDoDownload, 0);
	 register_acl_cmd(ACLCMD_RESET, AclResetBoard, 0);
	 register_acl_cmd(ACLCMD_GET_1284_STRING, AclGet1284String, 0);
	 register_acl_cmd(ACLCMD_GET_BUILD_INFO, AclGetBuildInfo, 0);
	 register_acl_cmd(ACLCMD_READ_TABLE, AclReadTable, 0);
	 register_acl_cmd(ACLCMD_WRITE_TABLE, AclWriteTable, 0);
	 //register_acl_cmd(ACLCMD_READ_NVRAM, AclReadNVRAM, 0);
	 //register_acl_cmd(ACLCMD_WRITE_NVRAM, AclWriteNVRAM, 0);
	 //register_acl_cmd(ACLCMD_RESTORE_NVRAM, AclRestoreDynamicNVRAM, 0);
#ifdef HAVE_EEPROM
	 register_acl_cmd(ACLCMD_READ_EEPROM, AclReadEEPROM, 0);
	 register_acl_cmd(ACLCMD_WRITE_EEPROM, AclWriteEEPROM, 0);
#endif
	 register_acl_cmd(ACLCMD_GET_ASICID, AclGetASICID,0);
#ifdef HAVE_BOOTSPI
	 register_acl_cmd(ACLCMD_BURN_FLASH, AclBurnFlash, 0);
#endif
}
