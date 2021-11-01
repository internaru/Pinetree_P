# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = engPaperHandlingAPI.h laser_engine_config_api.h
INSTALL_SRC_DIR = ./include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

SOURCE += laser_engine_config_utils.c

#  objects to build:
ifdef HAVE_ENGINE_STUB
SOURCE += laser_engine_config.c
SOURCE += engine_stub.c 
endif

ifdef HAVE_ENGINE_PAPERLESS_MODE
MACROS += HAVE_ENGINE_PAPERLESS_MODE
endif


include $(BUILD_ROOT)/stdtargets.mk
