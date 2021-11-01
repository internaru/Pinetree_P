# ============================================================================
# Copyright (c) 2014 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

#INCFLAGS = -I./src -I./include -I./config -I./config/$(BOARD) -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE =

include $(BUILD_ROOT)/stdtargets.mk

#override stdtargets build and clean rules to build the scan apps
build:
	@echo Building scan apps
	$(MAKE) -f apps.mk

clean:
	$(MAKE) -f apps.mk clean

