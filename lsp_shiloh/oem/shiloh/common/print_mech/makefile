# ==========================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#
# This makefile assumes GNU make features
#
 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

# The configuration directory depends on the print mechanism for this product.
# The global configuration directory should not be used.
ENG_CFG_DIR = eng_$(PRINT_MECH)

INCFLAGS = -I./$(ENG_CFG_DIR) -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC =
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
CONFIG_SRC_DIR = $(ENG_CFG_DIR)

#  objects to build:
CONFIG_SOURCE = $(PRINT_MECH)_laser_engine_config.c


# marvell pinetree engine code
# CONFIG_SOURCE += engine_$(PRINT_MECH)_mv.c 

# shiloh pinetree engine code
CONFIG_SOURCE += engine_$(PRINT_MECH).c 
CONFIG_SOURCE += fusermanager.c enginemanager.c ep_control.c page_control.c pagesupervisor.c out_port.c ipc.c

ifeq ($(PRINT_MECH), pinetree)
CONFIG_SOURCE += option.c uart.c
CONFIG_SOURCE += i2c_thread.c
CONFIG_SOURCE += cipher.c security.c
CONFIG_SOURCE += encryption.c
endif

ifeq ($(PRINT_MECH), engine_z)
CONFIG_SOURCE += engine_z_engine_if.c uart.c
endif
SOURCE = 

# monochrome source files
ifeq ($(PRINT_MECH_TYPE), monochrome)
	SOURCE += 
endif

# color source files
ifeq ($(PRINT_MECH_TYPE), color)
	SOURCE +=
endif

include $(BUILD_ROOT)/stdtargets.mk
