# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#

#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(OEM_PATH)/common/parsers/urf/config -I./tests -I./include  \
	   -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(BUILD_ROOT)/common/devices/jpeg/include

INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC =
INSTALL_SRC_DIR = include

# headers to install
ifdef HAVE_PARSER_INTERNAL_URF
INSTALL_SRC += internal_urf_parser.h
endif

ifdef HAVE_ZX_COLOR_PIPE
MACROS += ZX_STREAM_PAGE
endif

ifdef HAVE_LASER_SW_COLOR_PIPE
MACROS += LASER_CMYK_STRIP_PAGE
endif

ifdef HAVE_LASER_HW_STRIP_COLOR_PIPE
MACROS += LASER_RGB_STRIP_PAGE
endif
#  objects to build:
SOURCE = unirast.c  

ifdef HAVE_PARSER_CUPS20
SOURCE += cups2_parser.c
MACROS += HAVE_PARSER_CUPS20
# cups3.0 parser is dependent on cups2.0 parser
ifdef HAVE_PARSER_CUPS30
SOURCE += cups3_parser.c
MACROS += HAVE_PARSER_CUPS30
endif
endif

ifdef HAVE_PARSER_INTERNAL_URF
SOURCE += internal_urf_parser.c
MACROS += HAVE_PARSER_INTERNAL_URF
endif

ifdef HAVE_PARSER_UNI10
# UNIRAST 1.0 is a 3rd party parser.
MACROS += HAVE_PARSER_UNI10
endif

ifdef HAVE_JPEG_PARSER
SOURCE += jpeg_parser.c process_strip_data.c
INSTALL_SRC += jpeg_parser.h
ifdef HAVE_JPEG_HW_ASSIST
SOURCE += jpeg_decompress_hw.c
else
SOURCE += jpeg_decompress_sw.c
endif
MACROS += HAVE_PARSER_JPEG
endif

ifdef HAVE_JBIGHW 
MACROS += HAVE_JBIGHW
endif

include $(BUILD_ROOT)/stdtargets.mk
