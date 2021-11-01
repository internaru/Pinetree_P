# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#

#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(OEM_PATH)/common/print/colorpipe/config -I./tests -I./include  \
	   -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# headers to install
INSTALL_SRC = ColorPipe.h ColorPipeTables.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
ifdef HAVE_ZX_COLOR_PIPE
SOURCE =  ColorPipeZxDefault.c ColorPipeZx.c ColorPipeTableStub.c
else
ifdef HAVE_LASER_HW_STRIP_COLOR_PIPE
#SOURCE =  ColorPipeLzHWDefault.c ColorPipeLzHW.c ColorPipeTableStub.c
SOURCE =  ColorPipeRGBDefault.c ColorPipeRGB.c ColorPipeTableStub.c
else
SOURCE =  ColorPipeStub.c ColorPipeTableStub.c
endif
endif

include $(BUILD_ROOT)/stdtargets.mk
