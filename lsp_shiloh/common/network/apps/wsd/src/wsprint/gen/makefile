# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
include $(BUILD_ROOT)/project.mk

# Allow warnings in this makefile since it is generated 3rd party code
CCPARAM := $(filter-out -Werror,$(CCPARAM)) -Wno-unused-function


# install public header files
INSTALL_SRC = 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS =  -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I../../../include
INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/src/dpws/include
INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/platform/gnu/linux/include
INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/src/common/include

# specify source files to build
SOURCE = wprtServerLib.c wprtClientLib.c

include $(BUILD_ROOT)/stdtargets.mk

