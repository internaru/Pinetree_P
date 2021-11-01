# ============================================================================
# Copyright (c) SINDOH,  All Rights Reserved
#                      
# SINDOH Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = TinyDebugger.h
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = TinyDebugger.c 

include $(BUILD_ROOT)/stdtargets.mk
