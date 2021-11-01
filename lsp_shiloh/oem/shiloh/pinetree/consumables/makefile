# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT ?= ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = consumables_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#only install config and build stub if we are using stub implemtation
ifdef HAVE_OEM_CONSUMABLES_API
  CONFIG_SRC = consumables_config.h
  CONFIG_SRC_DIR = config
endif

include $(BUILD_ROOT)/stdtargets.mk
