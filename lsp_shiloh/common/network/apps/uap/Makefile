# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_uap_api.h
INSTALL_SRC += net_uap_vars.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./config -I$(MODULES_PATH)/common/network/apps/uap/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE  = net_uap_main.c
SOURCE += net_uap_config.c
SOURCE += net_uap_vars.c
SOURCE += net_uap_oids.c
SOURCE += net_uap_cmds.c

include $(BUILD_ROOT)/stdtargets.mk
