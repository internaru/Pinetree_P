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

INCFLAGS = -I./src -I$(OEM_PATH)/common/parsers/common/config -I./tests -I./include  \
	   -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# headers to install
INSTALL_SRC = bstream.h cp_strip.h 
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = bstream.c cp_strip.c 
# SOURCE += mpdf_strip.c

ifdef HAVE_BSTREAM_FILTER_RLE
INSTALL_SRC += bstream_filter_rle.h
SOURCE += bstream_filter_rle.c 
endif 

# force test
# HAVE_BSTREAM_FILTER_JPEG = 1  

ifdef HAVE_BSTREAM_FILTER_JPEG
INSTALL_SRC += bstream_filter_jpeg.h
SOURCE += bstream_filter_jpeg.c
endif 

ifdef HAVE_BSTREAM_FILTER_FLATE
INSTALL_SRC += bstream_filter_flate.h
SOURCE += bstream_filter_flate.c 
endif 

ifdef HAVE_JBIGHW
SOURCE += JbigHW.c
INSTALL_SRC += JbigHW.h
MACROS += HAVE_JBIGHW
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

ifdef HAVE_ZX_CREEK_STRIP_COLOR_PIPE
MACROS += LASER_ZX_CREEK_STRIP_PAGE
endif

include $(BUILD_ROOT)/stdtargets.mk
