# ============================================================================
# Copyright (c) 2009-2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I./tests -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

OTHER_SRC =
INSTALL_SRC =
INSTALL_SRC_DIR += ./include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE =

INSTALL_SRC += posix_ostools.h
SOURCE += posix_ostools.c

# if linux build
ifndef HAVE_RTOS  
INSTALL_SRC += os_system.h
INSTALL_SRC += os_debug.h
SOURCE += os_system.c
SOURCE += os_shell.c
endif

include $(BUILD_ROOT)/stdtargets.mk

# if linux build we want to build the DrOS layer for linux to a loadable kernel module
ifndef HAVE_RTOS
# override stdtargets build and clean rules to build the kernel modules
# note that we invoke the default base rule to build the src directory then
# we build the kernel modules.
build: build_base_rule
	@echo Building DrOS Kernel Shim
	@echo $(PRJROOT)
	export PRJROOT
	cd linux-kernel/src && $(MAKE) ARCH=arm HOSTCC=/usr/bin/gcc CROSS_COMPILE=arm-marvell-linux-gnueabi- all
	cp linux-kernel/src/*.ko $(BUILD_ROOT)

#clean rule 
clean: clean_base_rule
	@export PRJROOT
	cd linux-kernel/src && $(MAKE) ARCH=arm HOSTCC=/usr/bin/gcc CROSS_COMPILE=arm-marvell-linux-gnueabi- clean
	@-$(RM) $(BUILD_ROOT)/*.ko
endif
