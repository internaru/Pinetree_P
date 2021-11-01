# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

BUILD_ROOT ?= ../../../../..

#Include master environment
include $(BUILD_ROOT)/project.mk

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# install oem public header files

# specify location of local header files
INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I$(PRJROOT)/sysapps/include

# specify source files to build
SOURCE += oem_net_main.c

include $(BUILD_ROOT)/stdtargets.mk

