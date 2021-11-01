# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../../..

include $(BUILD_ROOT)/project.mk

INSTALL_SRC_DIR = include
INSTALL_SRC = encrypt.h
INSTALL_SRC_DIR = wps/src
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# openssl headers
INCFLAGS += -I$(PRJROOT)/sysapps/include

# source file(s)
VPATH += wps/src
SOURCE += encrypt.c

include $(BUILD_ROOT)/stdtargets.mk

