# ============================================================================
# Copyright (c) 2011-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I../src -I$(OEM_PATH)/common/print/pip/config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pip_stats_api.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#lsptodo add stats 
ifeq (($ASIC), gemstone)
#  objects to build:
# SOURCE = pip_stats.c
endif
include $(BUILD_ROOT)/stdtargets.mk
