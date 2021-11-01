# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(BUILD_ROOT)/common/nvram/src/

ifdef HAVE_NVRAM_FILE
INSTALL_SRC = sqlite3.h db_api.h nvram_db_api.h
endif
CONFIG_SRC = nvram_config.h
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
ifdef HAVE_NVRAM_FILE
SOURCE = nvram_db_shim.c nvram.c
endif

CONFIG_SOURCE = nvram_config.c

include $(BUILD_ROOT)/stdtargets.mk
