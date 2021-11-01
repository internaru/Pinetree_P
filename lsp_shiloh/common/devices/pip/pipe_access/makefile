# ============================================================================
# Copyright (c) 2011-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I../src -I$(OEM_PATH)/common/print/pip/config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pipe_access_api.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

SOURCE = pipe_access.c
include $(BUILD_ROOT)/stdtargets.mk
