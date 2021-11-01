# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(MODULES_PATH)/common/print/mode/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INSTALL_SRC = print_mode.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


MODULE_LIB_TARGET = ./lib/libprint_le_$(TYPE).a
MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = print_mode.c

include $(BUILD_ROOT)/stdtargets.mk
