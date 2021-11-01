# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I../CommonHeader -I../CommonHeader/ra

INSTALL_SRC = faxapp_print.h faxapp_scan.h faxapp_notify.h faxapp_param.h faxapp_make2pdf.h faxapp_make2raw.h faxapp_make2tif.h faxapp_sc_code.h faxapp_solution.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = faxapp_main.c
SOURCE += faxapp_job.c
SOURCE += faxapp_param.c
SOURCE += faxapp_print.c
SOURCE += faxapp_scan.c
SOURCE += faxapp_notify.c
SOURCE += faxapp_version.c
SOURCE += faxapp_db.c
SOURCE += faxapp_report.c
SOURCE += faxapp_performance.c
SOURCE += faxapp_make2pdf.c
SOURCE += faxapp_make2raw.c
SOURCE += faxapp_make2tif.c
SOURCE += faxapp_solution.c
SOURCE += faxapp_network.c

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Os -fno-strict-aliasing
