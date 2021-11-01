# ============================================================================
# Copyright (c) 2009-2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = eth_phy_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./src  -I./config -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

include $(BUILD_ROOT)/stdtargets.mk

