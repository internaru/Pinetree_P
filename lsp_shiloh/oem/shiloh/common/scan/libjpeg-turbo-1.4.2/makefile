# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC +=  jconfig.h\
				jerror.h\
				jmorecfg.h\
				jpeglib.h\
				cdjpeg.h\
				jinclude.h\
				cderror.h

CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#MODULE_LIB_TARGET = ./lib/libsystem_le_$(TYPE).a
#MODULE_LIB_DIR = ./lib

#  objects to build:
#SOURCE = 			

include $(BUILD_ROOT)/stdtargets.mk
