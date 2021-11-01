# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = ATTRIB.H BUFFERS.H Config.h configmo.h CONTROLX.H cxshell.h dc_common_includes.h dc_fwcxdxx_lnx_api.h ECM.H ECMX.H ERROR.H fshs.h fxcb.h HDLC.H homolgat.h MACROS.H modem_buff.h MODEMX.H mosi.h mosi_linux.h PHONECPX.H PHONEX.H ref_fipp.h T30X.H
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = fipp_app.c
SOURCE += mosi.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Os -fno-strict-aliasing
