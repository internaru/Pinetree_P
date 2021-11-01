# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(PRODUCT_PATH)/paper/config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = paper_types_api.h paper_types_config.h paper_size_api.h \
	      paper_size_config.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = paper_size.c paper_types.c

ifdef HAVE_UNIT_TEST
SOURCE += paper_tests.c
endif

MACROS += HAVE_PAPER

include $(BUILD_ROOT)/stdtargets.mk
