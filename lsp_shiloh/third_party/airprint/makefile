# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#

#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(OEM_PATH)/common/parsers/urf/config -I./tests -I./include  \
	   -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(BUILD_ROOT)/common/parsers/urf/src

# headers to install
INSTALL_SRC = airprint_api.h oid_airprint_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


# objects to build:
SOURCE = oid_module_airprint.c airprint.c

ifdef HAVE_PARSER_UNI10
  # UNIRAST 1.0 is a 3rd party parser.
  SOURCE += uni1_parser.c
endif


include $(BUILD_ROOT)/stdtargets.mk
