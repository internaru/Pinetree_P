# ============================================================================
# Copyright (c) 2009-2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) 

INSTALL_SRC = agJobUtils.h JobMgr.h print_logger.h PrintJobInfo.h PrintMgr.h \
	      PrintTools.h printvars_api.h agprint.h print_job_api.h print_job_api_restricted.h \
	      print_job_types.h print_events_common_api.h dbg_pnm.h virtual_control_panel.h
INSTALL_SRC += jbig.h
CONFIG_SRC = 
INSTALL_SRC_DIR = include
CONFIG_SRC_DIR = 
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIB_TARGET = ./lib/libprint_le_$(TYPE).a
MODULE_LIB_DIR = ./lib

#  objects to build:
SOURCE = PrintJobInfo.c
SOURCE += print_job_utils.c JobMgr.c PrintJobInfo.c PrintMgr.c print_job_api.c \
	PrintTools.c print_events_common.c printVars.c print_status_observers.c

ifdef HAVE_DBG_PNM
SOURCE += dbg_pnm_linux.c virtual_control_panel.c
else
SOURCE += dbg_pnm_default.c
endif

include $(BUILD_ROOT)/stdtargets.mk
