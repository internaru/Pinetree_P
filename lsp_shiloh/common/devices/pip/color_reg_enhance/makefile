# ============================================================================
# Copyright (c) 2010-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(OEM_PATH)/common/print/pip/config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = color_reg_enhance_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = color_reg_enhance.c 

VPATH := config

include $(BUILD_ROOT)/stdtargets.mk
