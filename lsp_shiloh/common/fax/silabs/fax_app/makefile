# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = fax_app_api.h fax_app_status.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = fax_app_print.c 
SOURCE += fax_app_scan.c 
SOURCE += fax_app_faxer.c 
SOURCE += fax_app_main.c
SOURCE += fax_app_status.c
SOURCE += fax_app_context.c
SOURCE += fax_app_jpeg.c
SOURCE += fax_app_raw.c

# This may be best somewhere else, but for now it is here
SOURCE += fax_header.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Os -fno-strict-aliasing
