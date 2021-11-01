# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC      = rgb2labi.h
INSTALL_SRC     += lab2rgbi.h
INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE  = rgb2labi.c
SOURCE += lab2rgbi.c

include $(BUILD_ROOT)/stdtargets.mk


