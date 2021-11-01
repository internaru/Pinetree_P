# ============================================================================
# Copyright (c) 2009-2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_iface.h
INSTALL_SRC += net_iface_api.h
INSTALL_SRC += net_iface_vars.h
INSTALL_SRC += net_iface_enum.h
INSTALL_SRC += net_iface_config.h
INSTALL_SRC += net_intf_api.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE  = net_iface.c
SOURCE += net_iface_vars.c
SOURCE += net_iface_config.c
SOURCE += net_iface_config_dflt.c
SOURCE += net_iface_cmds.c

include $(BUILD_ROOT)/stdtargets.mk

