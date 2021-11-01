# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pjl_variables.h
INSTALL_SRC_DIR = ./config
OTHER_SRC =
OTHER_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = zPJL_sym.c pjlStatusMap.c 
SOURCE += pjl_variables.c
SOURCE += oid_module_pjl_oem.c

include $(BUILD_ROOT)/stdtargets.mk
