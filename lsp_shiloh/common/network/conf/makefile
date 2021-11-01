# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================
 
#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_conf_api.h net_conf_files.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INCFLAGS = -I$(BUILD_ROOT)/common/network/conf/src/ -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

##  objects to build:
SOURCE  = net_conf_api.c
SOURCE += net_conf_cmds.c

VPATH += ./src

include $(BUILD_ROOT)/stdtargets.mk
