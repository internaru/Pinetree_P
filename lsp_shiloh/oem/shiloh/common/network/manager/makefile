# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = NetworkIFModule.h
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
OTHER_SRC_DIR = config/wpaSupport

# specify location of local header files
INCFLAGS = -I./config -I$(MODULES_PATH)/common/network/manager/src -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -Iconfig/wpaSupport

# specify source files to build
SOURCE = NetworkIFModule.c
SOURCE += NetworkUI_Interface.c
SOURCE += md5.c
SOURCE += sha1.c
ifdef HAVE_WIFI_SETTER
SOURCE += WifiSetter.c
endif
include $(BUILD_ROOT)/stdtargets.mk
