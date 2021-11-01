# ============================================================================
# Copyright (c) 2014 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
BUILD_ROOT ?= ../../../..
include $(BUILD_ROOT)/project.mk

# install public header files
INSTALL_SRC = net_wsd_api.h net_wsd_vars.h net_wsd_config.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify location of local header files
INCFLAGS =  -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I./include 

INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/src/dpws/include
INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/platform/gnu/linux/include
INCFLAGS += -I$(PRJROOT)/sysapps/dpws/dpwscore/src/common/include

LIBS  = pthread xmltools dpws common dcpl
LIBS += ssl crypto
LIBS += al

ifdef HAVE_WSD_PRINT
INCFLAGS += -I./src/wsprint/gen
endif

# specify source files to build
SOURCE = wsd.c wsd_vars.c wsd_directed_probe.c net_wsd_config.c

ifdef HAVE_WSD_PRINT
SOURCE += wsd_print.c wsd_print_events.c net_wsd_print_config.c plat_print_config.c
endif

include $(BUILD_ROOT)/stdtargets.mk

