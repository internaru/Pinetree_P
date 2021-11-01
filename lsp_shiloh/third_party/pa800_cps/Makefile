# ============================================================================
# Copyright (c) 2014 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

#PATH := $(PATH):$(ARM_ROOT)/bin
#export CROSS_COMPILE=arm-marvell-eabi-

# export paths
# contains shared headers
export CPS_INCDIR=$(shell pwd)/include
# contains shared archive libraries
export CPS_LIBDIR=$(shell pwd)/lib
# contains misc binaries
export CPS_BINDIR=$(shell pwd)/bin
# CRI uses CRI_WORKDIR in their makefiles
export CRI_WORKDIR=$(shell pwd)/src
# path to abstraction layer code
export OEM_AL_PATH=$(shell pwd)/threadx_al

include cps-mrvl-thrdx.mk.include

ifeq ($(TYPE),debug)
BUILD_DEBUG=YES
else
BUILD_DEBUG=NO
endif

# Uncomment this to enable logger on debug mode.
BUILD_MSGLOGGER=$(BUILD_DEBUG)
MAKEARGS = BUILD_MSGLOGGER=$(BUILD_MSGLOGGER) BUILD_DEBUG=$(BUILD_DEBUG)

# install shared headers
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
ifdef DYNAMIC_PA800_CPS_BUILD
# install headers listed in CPS_SHARED_HEADERS
INSTALL_SRC = $(CPS_SHARED_HEADERS)
else
# install headers in include/
INSTALL_SRC = $(wildcard $(CPS_INCDIR)/*.h)
endif

#INSTALL_LIBS = $(wildcard $(CPS_LIBDIR)/*.so)

.PHONY: build

#$(INSTALL_LIBS):
#	cp $@ $(PRJROOT)/newroot/lib

build: $(INSTALL_LIBS)
#	@echo CPS BUILD INVOKED for install libs: $(INSTALL_LIBS)
	@-cp $(CPS_LIBDIR)/*.so $(PRJROOT)/newroot/lib

# Call the source makefile if building from source
ifdef DYNAMIC_PA800_CPS_BUILD
.PHONY: install build clean 
lib_no_headers: build

install:

build: 

clean:
	
endif	

include $(BUILD_ROOT)/stdtargets.mk
