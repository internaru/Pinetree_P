# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_wifi_vars.h
INSTALL_SRC += net_wifi_api.h
INSTALL_SRC += wifi_intf.h
INSTALL_SRC += wifi_intf_vars.h
INSTALL_SRC += wifi_intf_api.h
INSTALL_SRC += wifi_intf_link_api.h
INSTALL_SRC += wifi_intf_event_api.h
INSTALL_SRC += wirelessQueue.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./config -I$(MODULES_PATH)/common/network/apps/wifi/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I$(OEM_PATH)/common/network/manager/config 

# specify source files to build
SOURCE  = net_wifi_main.c
SOURCE += net_wifi_config.c
SOURCE += net_wifi_cmds.c
SOURCE += wifi_intf_vars.c

include $(BUILD_ROOT)/stdtargets.mk
