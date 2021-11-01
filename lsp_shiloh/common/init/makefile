# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT ?= ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

ifdef HAVE_SYSAPP_CURL
endif
INCFLAGS += -I$(PRJROOT)/sysapps/include

ifdef HAVE_IFAX
INCFLAGS += -I$(BUILD_ROOT)/common/network/apps/ports/include
endif
INSTALL_SRC = sys_init_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = SysInit.c main.c

include $(BUILD_ROOT)/stdtargets.mk
