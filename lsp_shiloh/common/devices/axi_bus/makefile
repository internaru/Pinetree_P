# ============================================================================
# Copyright (c) 2014 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I../include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = axi_bus_api.h
INSTALL_SRC_DIR = ./include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = axi_bus_api.c

include $(BUILD_ROOT)/stdtargets.mk
