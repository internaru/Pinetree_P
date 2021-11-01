# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS  = -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -Isrc

INSTALL_SRC  = 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE += cmd_usb.c

include $(BUILD_ROOT)/stdtargets.mk

# Always optimize and check for errors
CCPARAM += -Wall -Werror -Os -fno-strict-aliasing
