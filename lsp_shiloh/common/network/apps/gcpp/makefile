# ============================================================================
# Copyright (c) 2011-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_gcpp_api.h
INSTALL_SRC += net_gcpp_vars.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS =  -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I$(PRJROOT)/sysapps/include
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)/libstrophe
INCFLAGS += -I$(BUILD_ROOT)/third_party/curl/include
INCFLAGS += -I$(BUILD_ROOT)/common/network/apps/net_sm/include
INCFLAGS += -I./include 
INCFLAGS += -I$(OEM_PATH)/common/network/apps/gcpp/config
INCFLAGS += -I./src

MACROS += USE_THREADX_SOCKET_H GCPP_ACCEPT_COMPRESSION

# specify source files to build
SOURCE = gcpp.c
SOURCE += gcpp_helper.c
SOURCE += gcpp_vars.c

# if under old build system include config locally; need to fix once all projects on new system
ifndef OEM_DIR
SOURCE += gcpp_oem_config.c
endif

include $(BUILD_ROOT)/stdtargets.mk

