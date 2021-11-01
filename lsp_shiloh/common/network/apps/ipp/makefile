# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC      = ipp_api.h

SOURCE += ipp.c
SOURCE += ipp_attribute.c
SOURCE += ipp_const.c
SOURCE += ipp_main.c
SOURCE += ipp_request.c
SOURCE += ipp_handle_attr_reqs.c
SOURCE += ipp_printer_requested_attr.c
SOURCE += ipp_job_requested_attr.c
SOURCE += ipp_validate_request.c
SOURCE += ipp_config_attr.c

include $(BUILD_ROOT)/stdtargets.mk
