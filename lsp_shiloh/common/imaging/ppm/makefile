# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC      = ppm.h
INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE  = ppm.c

include $(BUILD_ROOT)/stdtargets.mk

# Always optimize and check for errors
CCPARAM += -Wall -Werror -Os -fno-strict-aliasing
