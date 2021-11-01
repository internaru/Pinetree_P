# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = consumables_common_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#only install config and build stub if we are using stub implemntation
ifdef HAVE_COMMON_CONSUMABLES_STUB
#  CONFIG_SRC = consumables_config.h
  CONFIG_SRC_DIR = config
  SOURCE = consumables_common_stub.c
endif

include $(BUILD_ROOT)/stdtargets.mk
