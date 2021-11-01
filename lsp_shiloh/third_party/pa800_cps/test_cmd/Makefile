# ============================================================================
# Copyright (c) 2009-2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./ -I./src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC += SwAuthComplete_set.bin
CONFIG_SRC = 
INSTALL_SRC_DIR = $(BUILD_ROOT)/third_party/pa800_cps/bin/
CONFIG_SRC_DIR = 
INSTALL_DEST_DIR = $(PRJROOT)/newroot/cps

SOURCE = cps_cmd.c

MACROS += CLUSTERFILEPATH=\"$(CLUSTERFILEPATH)\"

include $(BUILD_ROOT)/stdtargets.mk
