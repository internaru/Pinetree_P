# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

# Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pwm_api.h pwm_priv.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# Objects to build:
SOURCE = pwm.c 

MACROS += HAVE_PWM

include $(BUILD_ROOT)/stdtargets.mk
