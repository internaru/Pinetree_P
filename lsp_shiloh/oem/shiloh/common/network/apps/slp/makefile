# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC =
INSTALL_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./config -I$(MODULES_PATH)/common/network/apps/slp/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE = net_slp_config.c

include $(BUILD_ROOT)/stdtargets.mk
