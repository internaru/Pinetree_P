# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
#BUILD_ROOT = ../../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS =  -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(BUILD_ROOT)/common/network/apps/gcpp/include
INCFLAGS += -I$(PRJROOT)/sysapps/include

INSTALL_SRC = gcpp_oem_config.h
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


# specify source files to build
SOURCE = gcpp_oem_config.c
SOURCE += gcpp_oem_vars.c

include $(BUILD_ROOT)/stdtargets.mk
