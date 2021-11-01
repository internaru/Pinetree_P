# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#INSTALL_SRC = image_converter.h fax_tiff_io.h
INSTALL_SRC = fax_tiff_io.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

SOURCE	= fax_tiff_io_stub.c

#MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))
#MODULE_LIB_TARGET = ./lib/libimgcnvtrt_le_$(TYPE).a
#MODULE_LIB_DIR = ./lib


# Todo: set up config dir for generic file system mappings, etc
#CONFIG_SRC = 
#CONFIG_SRC_DIR = config
#CONFIG_SOURCE = 

include $(BUILD_ROOT)/stdtargets.mk
