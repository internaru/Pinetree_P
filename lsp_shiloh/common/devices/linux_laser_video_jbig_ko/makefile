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

INCFLAGS = -I./src -I./include -I./config -I./config/$(BOARD) -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = laser_video_chardev_api.h laser_video_types.h laser_video_defs.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE =

MACROS += HAVE_VIDEO

include $(BUILD_ROOT)/stdtargets.mk

#override stdtargets build and clean rules to build the kernel modules
build:
	@echo Building video kernel drivers
	@echo $(PRJROOT)
	export PRJROOT
	cd src && $(MAKE) ARCH=arm HOSTCC=/usr/bin/gcc CROSS_COMPILE=arm-marvell-linux-gnueabi- all
	cp src/*.ko $(BUILD_ROOT)

clean:
	@export PRJROOT
	cd src && $(MAKE) ARCH=arm HOSTCC=/usr/bin/gcc CROSS_COMPILE=arm-marvell-linux-gnueabi- -s clean
	@-$(RM) $(BUILD_ROOT)/*.ko

