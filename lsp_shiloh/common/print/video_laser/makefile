# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) \
	   -I$(BUILD_ROOT)/common/devices/video/config

#-I./$(BUILD_ROOT)/oem/print/engine -I./$(BUILD_ROOT)/oem/print/video \
	    -I./$(BUILD_ROOT)/oem/print/engine/include

INSTALL_SRC = video_laser_api.h
CONFIG_SRC = 
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


MODULE_LIB_TARGET = ./lib/lib_laser_video_le_$(TYPE).a
MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = video_laser_linux.c 
CONFIG_SOURCE = 
ifdef HAVE_POWER_MGR
SOURCE += 
endif

include $(BUILD_ROOT)/stdtargets.mk
