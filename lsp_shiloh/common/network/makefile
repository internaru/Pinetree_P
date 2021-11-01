# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_logger.h net_api.h net_debug.h
INSTALL_SRC += os_network.h os_treck_stubs.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify paths of local header files
INCFLAGS = -I./$(SRCDIR) -I./$(CFGDIR) -I./$(TESTDIR) -I./$(INCDIR) -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE = net_logger.c net_timer.c net_oids.c

include $(BUILD_ROOT)/stdtargets.mk
