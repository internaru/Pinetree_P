# ============================================================================
# Copyright (c) 2008-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(OEM_PATH)/common/print/pip/config -I$(OEM_PATH)/common/print/pip/config/luts -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = pip_api.h pip_dma_api.h 
INSTALL_SRC += pip_page_adaptor.h 
INSTALL_SRC += pip_PAGE_page_adaptor.h
INSTALL_SRC += pip_strip_adaptor.h 
INSTALL_SRC += pip_PLANE_strip_adaptor.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
SOURCE =  pip_utils.c pip_dma.c
SOURCE += pip.c 
SOURCE += pip_page_adaptor.c pip_PAGE_page_adaptor.c 
SOURCE += pip_strip_adaptor.c pip_PLANE_strip_adaptor.c 
SOURCE += file_pip_adaptor.c

# pip_test.c

ifdef HAVE_GENERATOR_PIP 
SOURCE += pip_pqcmd.c
else 
ifdef HAVE_GENERATOR_PIP_ADAPTOR
SOURCE += pip_pqcmd.c
endif
endif

include $(BUILD_ROOT)/stdtargets.mk

