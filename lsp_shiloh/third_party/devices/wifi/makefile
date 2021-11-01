# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# builds $(MODULES) + mlan-related utilities

ifndef PRJROOT
$(error PRJROOT location?)
endif
.PHONY: install

#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

DRIVER = wifi

# *.ko bits are keyed to 'vermagic.h'
MODULES = mlan.ko usb8xxx.ko
KERNEL_VERSION = $(PRJROOT)/linux-mrvl/include/linux/vermagic.h

# sysapp $(DRIVER) location (external to app source tree)
SYSAPPS_WIFI = $(PRJROOT)/sysapps/$(DRIVER)

# wifi hw module family
HW_FAMILY = 8782

# modules in filesystem
FS_MODULES_LOC = $(PRJROOT)/newroot/lib/modules/$(HW_FAMILY)
TGT_FS_MODULES = $(foreach ko,$(MODULES),$(FS_MODULES_LOC)/$(ko))

# no $(SOURCE) required -- see build:
SOURCE =

CLEAN_TARGETS = $(TGT_FS_MODULES)
clean:
#	@rm -f $(TGT_FS_MODULES)

# rebuild kernel module(s) if $(KERNEL_VERSION) has changed
$(TGT_FS_MODULES) : $(KERNEL_VERSION)
	@echo Building $(DRIVER) kernel modules ..
	@$(foreach module,$(TGT_FS_MODULES),cd $(SYSAPPS_WIFI) && $(MAKE) all update-fs-newroot;)

# update tgt fs if modules not present
#build: $(TGT_FS_MODULES)
build:

#don't include $(BUILD_ROOT)/stdtargets.mk -- clean: & build: defined locally

#eof
