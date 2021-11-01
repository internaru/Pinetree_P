# ============================================================================
# Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INSTALL_SRC = zj_parser_config.h
INSTALL_SRC_DIR = config
CONFIG_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = zj_parser_config.c

include $(BUILD_ROOT)/stdtargets.mk
