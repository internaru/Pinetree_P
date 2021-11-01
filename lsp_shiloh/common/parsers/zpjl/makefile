# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I$(OEM_PATH)/common/parsers/zpjl/config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pjl_api.h pjl_enum_objs_api.h pjl_extra_api.h pjl_variables_common.h 
INSTALL_SRC_DIR = include
OTHER_SRC = zPJL.h
OTHER_SRC_DIR = src
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)



#  objects to build:
SOURCE = pjl_enum_objs.c PjlStatus.c zPJL.c zPJL_env.c zPJL_put.c zpjlparse.c \
	 zStr.c zPJL_test_env.c zPJL_test_grammar.c pjl_ustatus.c pjl_variable_common.c
SOURCE += oid_module_pjl_common.c


include $(BUILD_ROOT)/stdtargets.mk
