# ============================================================================
# Copyright (c) 2013 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT ?= ../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC  = net_services.h
INSTALL_SRC += net_services_api.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS = -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

SOURCE  = net_services_task.c
SOURCE += net_services_backend.c
SOURCE += net_services_health.c
SOURCE += net_services_cmds.c

include $(BUILD_ROOT)/stdtargets.mk

#eof
