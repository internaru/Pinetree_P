# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = phone.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = callerid.c 
SOURCE += dialer.c 
SOURCE += phone_main.c 
SOURCE += phonetbl.c
SOURCE += ringdet.c
SOURCE += tonedet.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Os -fno-strict-aliasing
