# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I../include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = gpio_api.h
INSTALL_SRC_DIR = ../include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = mrvl_gpio.c

MACROS += HAVE_GPIO

include $(BUILD_ROOT)/stdtargets.mk
