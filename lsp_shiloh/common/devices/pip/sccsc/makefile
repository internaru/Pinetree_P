# ============================================================================
# Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(OEM_PATH)/common/print/pip/config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = sccsc_api.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
SOURCE = sccsc.c

include $(BUILD_ROOT)/stdtargets.mk
