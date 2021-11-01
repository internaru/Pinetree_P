# ============================================================================
# Copyright (c) 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I./config/luts -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INSTALL_SRC =
INSTALL_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
SOURCE = ahs_cfg.c cp_cfg.c cr_cfg.c csc_cfg.c \
         davbuf_cfg.c fhs_cfg.c kmod_cfg.c mip_cfg.c nga_cfg.c \
         pa_idma_cfg.c pip_cfg.c pipe_access_cfg.c sccsc_cfg.c \
         serp_cfg.c stat_cfg.c th_cfg.c tp_cfg.c ued_cfg.c yipi_cfg.c \
	color_reg_enhance_cfg.c
ifeq ($(ASIC_REV), b0)
SOURCE += ctwin_cfg.c sharpen_cfg.c tems_cfg.c
endif

include $(BUILD_ROOT)/stdtargets.mk
