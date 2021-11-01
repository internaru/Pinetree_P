# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = t30.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = t30_ecm.c 
SOURCE += t30_main.c 
SOURCE += t30_misc.c 
SOURCE += t30_rx.c
SOURCE += t30_table.c
SOURCE += t30_tx.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Os -fno-strict-aliasing
