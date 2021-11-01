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

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)


INSTALL_SRC = dma_api.h odma_api.h odma_int_attach.h
INSTALL_SRC += raster_odma_func_table.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
#SOURCE = mrvl_odma.c

#MACROS += HAVE_ODMA

include $(BUILD_ROOT)/stdtargets.mk
