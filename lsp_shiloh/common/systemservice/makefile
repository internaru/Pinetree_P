# ============================================================================
# Copyright (c) 2009-2010 SINDOH,  All Rights Reserved
#                      
#                         SINDOH Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(OEM_PATH)/common/print_mech/eng_pinetree
INCFLAGS += -I$(OEM_PATH)/common/network/manager/config 

INSTALL_SRC = SysUI_Interface.h SysServiceMgr.h SysPosixTimer.h SysPaperDef.h SysSharedMemory.h SYS_UI_msg_interface.h
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#MODULE_LIB_TARGET = ./lib/libsystem_le_$(TYPE).a
#MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = SysServiceMgr.c SysPosixTimer.c SysSharedMemory.c WatchDogTimer.c 

include $(BUILD_ROOT)/stdtargets.mk
