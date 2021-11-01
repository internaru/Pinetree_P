# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_eth_vars.h
INSTALL_SRC += net_eth_api.h
INSTALL_SRC += net_eth.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INCFLAGS += -I$(INSTALL_DEST_DIR)

SOURCE += net_eth.c
SOURCE += net_eth_vars.c

include $(BUILD_ROOT)/stdtargets.mk

