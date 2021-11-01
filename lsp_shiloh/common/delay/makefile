# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = delay_api.h delay_msg_api.h
ifdef PROCESSOR_ARM 
ifndef HAVE_RTOS
INSTALL_SRC += timerfd.h
endif
endif

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build based on OS threadx or linux
ifdef HAVE_RTOS 
SOURCE = delay_mgr.c timerdelay.c
else
SOURCE = delay_linux.c
ifdef PROCESSOR_ARM 
SOURCE += timerfd.c
endif
endif
include $(BUILD_ROOT)/stdtargets.mk
