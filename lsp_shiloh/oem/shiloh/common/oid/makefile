# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 
INCFLAGS += -I$(OEM_PATH)/common/network/manager/config

INSTALL_SRC = oid_vendor_api.h
INSTALL_SRC_DIR = include
OTHER_SRC =
OTHER_SRC_DIR =
CLEAN_SRC = 
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE  = oem_snmp_printermib.c
SOURCE += oem_snmp_markers.c
SOURCE = oid_vendor.c

# VPATH += ./config

include $(BUILD_ROOT)/stdtargets.mk


