# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS  = -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -Isrc

INSTALL_SRC  = cmd_sys_api.h
INSTALL_SRC += cmd_proc_api.h
INSTALL_SRC += cmd_wrapper_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE += cmd.c
SOURCE += cmd_cmd.c
SOURCE += cmd_cntxt.c
SOURCE += cmd_wrapper.c
SOURCE += cmd_memory_dump.c

ifdef HAVE_CMD_VARS
INSTALL_SRC += cmd_var_api.h
SOURCE      += cmd_var.c
endif

# This can be overridden on the command line but the default is given here
CMD_STACK_SIZE=20000
MACROS += CMD_STACK_SIZE=$(CMD_STACK_SIZE)

include $(BUILD_ROOT)/stdtargets.mk


