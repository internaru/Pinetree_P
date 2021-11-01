# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../../..

include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./inc

# specify paths of local header files
INCFLAGS += -I../mlan/src -I../mthreadx/src -I../mlaninterface/src 
INCFLAGS += -I../wps/src -I../supplicant/src -I../src -I../include 
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)


# specify source files to build
SOURCE = $(notdir $(wildcard src/*.c))

include $(BUILD_ROOT)/stdtargets.mk
