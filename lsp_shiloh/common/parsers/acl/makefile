# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(OEM_PATH)/common/parsers/acl/config -I./tests -I./include -I./acl_sys_cmds \
	   -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = acl_api.h ACL_burn_flash_event_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = ACL.c aclburnflash.c aclget1284.c aclgetasicid.c \
         aclgetbuildinfo.c aclgetversion.c aclreadstatus.c \
	 aclreadtable.c aclresetboard.c aclwritetable.c globburnflash.c \
	 verifyflash.c writemems.c verifyflash.c acldownload.c \
	 dwnldcoderam.c loadELF.c

ifdef HAVE_EEPROM 
    SOURCE += writeeeprom.c readeeprom.c
else ifdef HAVE_STATIC_NV
    SOURCE += writeeeprom.c readeeprom.c
endif

VPATH := config acl_sys_cmds

include $(BUILD_ROOT)/stdtargets.mk
