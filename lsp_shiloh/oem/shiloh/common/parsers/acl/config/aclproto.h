/*
 *
 * ============================================================================
 * Copyright (c) 2006-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief 
 *
 */

#ifndef __ACLPROTO_H_
#define __ACLPROTO_H_

#include "ATypes.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ACLCMD_GET_VERSION         0x0001
#define ACLCMD_STATUS              0x0002
#define ACLCMD_READ_EEPROM         0x0003
#define ACLCMD_GET_ASICID          0x0004
#define ACLCMD_GET_1284_STRING     0x0006
#define ACLCMD_GET_BUILD_INFO      0x0008
#define ACLCMD_READ_TABLE          0x0009
#define ACLCMD_WRITE_TABLE         0x000A
#define ACLCMD_READ_LOG            0x000B
#define ACLCMD_CONFIG_TIMEBASE     0x000C
#define ACLCMD_CONFIG_LOG          0x000D
#define ACLCMD_SET_VIDEO_TABLE     0x000E
#define ACLCMD_BURN_FLASH          0x000F
#define ACLCMD_GET_ENG_VERSION     0x0010
#define ACLCMD_GET_RADIO_VER       0x0011
#define ACLCMD_READ_FLASH          0x0012
#define ACLCMD_GET_CONFIGURATION   0x0013
#define ACLCMD_DOWNLOAD_SRAM       0x0014  ///< bootcode command - download ELF into SRAM not DRAM
#define ACLCMD_GET_VERSION2        0x0015  ///< will return the 'codeVersion' AKA ASIC version.
#define ACLCMD_GET_COMP_VERSION    0x0016  ///< will return Nand CodeTable component versions (app, smartinstall, binary, loader, bootbackup, ...)
#define ACLCMD_HW_CMD              0x0018  ///< bootcode command for hardware configuration

#define ACLCMD_PRINT_CONF_PAGE     0x0021
#define ACLCMD_READ_NVRAM          0x0023 ///< Read nvram using string parm
#define ACLCMD_WRITE_NVRAM         0x0024 ///< Write nvram using string parm
#define ACLCMD_SET_LED             0x0025 ///< Write nvram using string parm
#define ACLCMD_GET_BUTTONS_PRESSED 0x0026
#define ACLCMD_RESTORE_NVRAM       0x0027 ///< Restore dynamic NVRAM to factory defaults
#define ACLCMD_GET_NVRAM_VAR_LIST  0x0028
#define ACLCMD_READ_MEM_CARD_STATE 0x0029 ///< Read memory card connection state (card/disk connected or not)
#define ACLCMD_MOVE_SCANNER        0x0101 ///< Move scanner
#define ACLCMD_CAL_SCANNER         0x0103 ///< Calibrate scanner
#define ACLCMD_GET_CAL_STATS       0x0104 ///< Retrieve cal result
#define ACLCMD_GET_CAL_STATS_V2    0x0105 ///< Retrieve cal result with better protocol
#define ACLCMD_GET_CAL_NOTCH_DATA  0x0106 ///< debug only command to get notch data

#define ACLCMD_SET_TIMER           0x0110 ///< set system timer

#define ACLCMD_SET_TTCP            0x1001

#define ACLCMD_DOWNLOAD            0xC0DE
#define ACLCMD_WRITE_EEPROM        0xEC0D
#define ACLCMD_UPDATE_EEPROM       0xEC1D
#define ACLCMD_RESET               0xD1EE

#define ACLCMD_GET_FILE            0xf11e
void * agiACLGetDownloadAddr(unsigned int size);
void * agiACLGetDownloadThreadStkStart(void);
void * agiACLHandleDownload(void* codeLoc);
int CodeCheckStart(UINT32 *Code, UINT32 Length);
ATSTATUS SPIMemRead(UINT32 devMemAddr, UINT8* memBuf, UINT32 bytesToRead); // in spiBus.c
ATSTATUS SPIMemOverwriteDev(UINT8* memBuf, UINT32 numBytes, BOOL fErase);  // in spiBus.c
ATSTATUS VerifyPlatformID(UINT8* pImgStart, UINT32 imgSize);
error_type_t AclGetVersion(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
//LSPTODO error_type_t AclDoDownload(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclRetStatus(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclResetBoard(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclGet1284String(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclGetBuildInfo(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclReadTable(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclWriteTable(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclReadLog(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclReadNVRAM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclWriteNVRAM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclRestoreDynamicNVRAM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
ATSTATUS ACLBurnFlash(UINT8* , UINT32 , UINT32 , UINT16 , UINT16 checkSum);
error_type_t AclBurnFlash(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
ATSTATUS VerifyFlash(UINT8* flashBuf, UINT8* srcBuf, UINT32 size);
error_type_t AclWriteEEPROM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclReadEEPROM(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
ATSTATUS WriteMems( unsigned char *downBuf, UINT32 dataLength, UINT8 busType,BOOL fErase );
ATSTATUS ACLBurnFlashCodePart(UINT8* dataBuf, UINT32 dataLen, UINT16 destination);
ATSTATUS ACLBurnFlashFullPart(UINT8* dataBuf, UINT32 dataLen, UINT16 destination);
error_type_t AclEEPROMUpdate(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);
error_type_t AclGetASICID(IOReg *Pipe, acl_cmd_base_struct_t *AclCmd, void *cmd_data);


#ifdef __cplusplus
}
#endif

#endif   // __ACLPROTO_H_

