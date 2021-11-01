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

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = jbig_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE =  jbigUtils.c mrvl_jbig.c

ifdef HAVE_UNIT_TEST
SOURCE += jbig_test.c
endif

include $(BUILD_ROOT)/stdtargets.mk
