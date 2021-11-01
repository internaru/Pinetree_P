# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC =
INSTALL_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = acl_register_cmds.c

VPATH := config

include $(BUILD_ROOT)/stdtargets.mk
