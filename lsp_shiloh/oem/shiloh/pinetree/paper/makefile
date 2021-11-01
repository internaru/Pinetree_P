# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS =-I./config -I$(MODULES_PATH)/common/paper/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = 
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = paper_config.c

include $(BUILD_ROOT)/stdtargets.mk
