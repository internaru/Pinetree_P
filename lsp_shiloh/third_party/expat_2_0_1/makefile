# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = expat.h expat_external.h
INSTALL_SRC_DIR = .
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = xmlparse.c xmltok.c xmlrole.c

include $(BUILD_ROOT)/stdtargets.mk
