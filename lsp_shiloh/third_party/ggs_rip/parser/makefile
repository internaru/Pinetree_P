# ============================================================================
# Copyright (c) 2011-2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================


#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = 
INSTALL_SRC_DIR = .
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE += ggs_parser.c panda_api.c panda_paper.c

MACROS += HAVE_MARVELL_SDK

include $(BUILD_ROOT)/stdtargets.mk
