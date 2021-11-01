# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_slp_api.h net_slp_vars.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./src -I./config -I$(OEM_PATH)/common/network/apps/slp/config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE = slp.c net_slp_vars.c

# if under old build system include config locally; need to fix once all projects on new system
ifndef OEM_DIR
SOURCE += net_slp_config.c
endif

include $(BUILD_ROOT)/stdtargets.mk
