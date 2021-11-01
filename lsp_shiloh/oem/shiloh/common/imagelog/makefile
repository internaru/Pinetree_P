# ============================================================================
# Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = imagelog.h imagelogMonitor.h
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
ifdef HAVE_IMAGELOG
SOURCE = imagelog.c imagelogMonitor.c aria.c
endif

include $(BUILD_ROOT)/stdtargets.mk
