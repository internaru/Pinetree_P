# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I../include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
# only one source here or devices/odma ?
INSTALL_SRC = raster_odma_api.h #raster_odma_func_table.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
SOURCE = raster_odma.c

VPATH := config

include $(BUILD_ROOT)/stdtargets.mk
