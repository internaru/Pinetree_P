# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = agConnectMgr.h agMessage.h agResourceMgr.h agRouter.h \
	      io_device.h sjm_observers_api.h SJMgr.h
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


MODULE_LIB_TARGET = ./lib/libsystem_le_$(TYPE).a
MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = agRouter.c connect_mgr.c resource_mgr.c SysJobMgr.c

include $(BUILD_ROOT)/stdtargets.mk
