# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = fsu.h fsu_api.h fsu_call.h fsu_pcfax.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE += fsu.c
SOURCE += fsu_api.c
SOURCE += fsu_call.c
SOURCE += fsu_cmd.c
SOURCE += fsu_mdm.c
SOURCE += fsu_mem.c
SOURCE += fsu_moif.c
SOURCE += fsu_entry.c
SOURCE += fsu_pcfax.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Os -fno-strict-aliasing
