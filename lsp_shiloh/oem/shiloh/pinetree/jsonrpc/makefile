# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) #-Xlinker --start-group -L./src/ -ljson_marvell-4.6.4_libmt -Xlinker --end-group
INCFLAGS += -I$(OEM_PATH)/common/network/manager/config


INSTALL_SRC = jsonrpc.h #json/json.h json/value.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

SOURCE += jsonrpc.cpp jsonrpc_wrapper.cpp jsonrpc_network.cpp
#ADDITIONAL_LIBRARIES = ./src/libjson_marvell-4.6.4_libmt.a
#MODULE_LIBPATH := ./src/libjson_marvell-4.6.4_libmt.a
#CCPARAM += -L./src/ -ljson_marvell-4.6.4_libmt #./src/libjson_marvell-4.6.4_libmt.a

include $(BUILD_ROOT)/stdtargets.mk
