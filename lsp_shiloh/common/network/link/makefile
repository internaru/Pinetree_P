# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

# public headers
INSTALL_SRC  = net_link_api.h
INSTALL_SRC += net_link_enum.h
INSTALL_SRC += net_link_config.h
INSTALL_SRC += net_wlan_api.h
INSTALL_SRC_DIR = .
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INCFLAGS = -I$(BUILD_ROOT)/common/network/link/$(INSTALL_SRC_DIR) -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

##  objects to build:
SOURCE  = net_link.c
SOURCE += net_link_config_dflt.c
SOURCE += net_link_inotify.c
SOURCE += net_link_cmds.c
SOURCE += net_link_events.c

ifdef HAVE_ETHERNET
VPATH += eth
SOURCE += net_eth.c
endif

ifdef HAVE_WLAN
VPATH += wlan
SOURCE += net_wlan.c
endif

ifdef HAVE_UAP
VPATH += uap
SOURCE += net_uap.c
endif

include $(BUILD_ROOT)/stdtargets.mk
