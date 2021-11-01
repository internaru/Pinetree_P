# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = lltd_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS  = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# enable Marvell-specific source modifications
CCPARAM += -D__MARVELL__ 

# suppress warnings
CCPARAM += -Wno-attributes -Wno-unused-variable -Wno-unused-function

# specify source files to build
SOURCE  =  osl-linux.c lltd_main.c event.c util.c packetio.c band.c \
	state.c sessionmgr.c enumeration.c mapping.c seeslist.c \
	tlv.c qospktio.c lltd_cmd.c

include $(BUILD_ROOT)/stdtargets.mk
