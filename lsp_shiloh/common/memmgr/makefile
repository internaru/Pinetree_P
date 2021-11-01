# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = memAPI.h mlimiter.h memAPInew.h




INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
SOURCE = memAPI.c mlimiter.c

ifdef HAVE_CMD
# SOURCE += mem_peek_poke.c
endif
ifndef HAVE_MLIMITER_CONFIG
SOURCE += mlimiter_config.c
endif

# this macro only needs local definition
MACROS += HAVE_MEMMGR_NEW
SOURCE += memAPInew.cpp



include $(BUILD_ROOT)/stdtargets.mk


