# ============================================================================
# Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = 
CONFIG_SRC =
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR =
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = generators.c

ifdef HAVE_GENERATOR_SKELETON
SOURCE += generator_skeleton.c
endif

ifdef HAVE_GENERATOR_PIP
SOURCE += generator_pip.c
endif

ifdef HAVE_GENERATOR_PIP_ADAPTOR
SOURCE += generator_pip_adaptor.c
endif

ifdef HAVE_GENERATOR_COMPRESS
SOURCE += generator_compress.c
endif

ifdef HAVE_GENERATOR_PIPP_PIPR
SOURCE += generator_pipp_pipr.c  generator_decompress.c
endif

ifdef HAVE_GENERATOR_NUP
SOURCE += generator_nup.c
SOURCE += nup_rotate.c
endif

include $(BUILD_ROOT)/stdtargets.mk
