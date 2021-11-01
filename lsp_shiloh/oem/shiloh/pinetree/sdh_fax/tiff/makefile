# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = tiff.h tiffconf.h tiffio.h tiffvers.h tiffiop.h tif_config.h tif_dir.h tif_predict.h uvcode.h tif_fax3.h t4.h tiff_main.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE += tiff_main.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -O3 -fno-strict-aliasing
