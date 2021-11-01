# ============================================================================
# Copyright (c) 2009-2010 SINDOH,  All Rights Reserved
#                      
#                         SINDOH Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INCFLAGS += -I$(PRJROOT)/sysapps/include

INSTALL_SRC = SysCardReaderMgr.h SysCardReaderMsg.h
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = SysCardReader.c 
ifdef HAVE_PAPERCUT
SOURCE += PaperCut_NW_Reader.c 
endif

include $(BUILD_ROOT)/stdtargets.mk
