# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = cdma_api.h
INSTALL_SRC_DIR = ../include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = mrvl_cdma.c

ifdef HAVE_UNIT_TEST
VPATH := ../tests
SOURCE += cdma_test.c
endif

MACROS += HAVE_CDMA

include $(BUILD_ROOT)/stdtargets.mk
