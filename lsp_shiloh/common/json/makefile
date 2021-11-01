# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = json_api.h json_config_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


SOURCE += json.c json_config.c

include $(BUILD_ROOT)/stdtargets.mk
