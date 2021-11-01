# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS =  -I./$(SRCDIR) -I./include -I./$(CFGDIR) -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

ifdef HAVE_SCAN_SUPPORT
INCFLAGS += -I$(BUILD_ROOT)/common/scan/include/
endif

INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC      = sm_job_api.h
INSTALL_SRC     += sm_job_support_api.h

SOURCE = sm_job_mgr.c
SOURCE += sm_job_pif.c
SOURCE += sm_job_support.c
SOURCE += sm_job_core.c

ifdef HAVE_SCAN_SUPPORT
SOURCE += sm_job_sif.c
endif

ifdef HAVE_CMD
SOURCE += sm_job_cmd.c
endif

# weak linkage config
SOURCE += sm_job_dflt_config.c

include $(BUILD_ROOT)/stdtargets.mk
