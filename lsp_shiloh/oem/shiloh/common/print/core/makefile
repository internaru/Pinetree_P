# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(MODULES_PATH)/common/print/core/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INSTALL_SRC = TrayPaperResolve.h print_status_codes.h
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


MODULE_LIB_TARGET = ./lib/libprint_le_$(TYPE).a
MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = PaperHandling.c TrayPaperResolve.c

ifdef HAVE_POWER_MGR
SOURCE += print_pwr_mgr.c
endif

include $(BUILD_ROOT)/stdtargets.mk
