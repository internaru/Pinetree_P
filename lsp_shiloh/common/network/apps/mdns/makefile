# ============================================================================
# Copyright (c) 2009-2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_mdns_api.h
INSTALL_SRC += net_mdns_vars.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./config -I$(MODULES_PATH)/common/network/apps/mdns/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE  = net_mdns_main.c
SOURCE += net_mdns_config.c
SOURCE += net_mdns_vars.c
SOURCE += net_mdns_oids.c
SOURCE += net_mdns_cmds.c

include $(BUILD_ROOT)/stdtargets.mk
