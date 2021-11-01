# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(BUILD_ROOT)/fax/silab/include

INSTALL_SRC  = faxer_api.h fax_modem_status.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

SOURCE  = faxer_main.c faxer_config.c
SOURCE  += faxer_log.c
#SOURCE  = faxer_main.c faxer_job_control.c faxer_config.c faxer_device_evt_handler.c
#SOURCE += faxer_timer.c faxer_acl.c faxer_log.c faxer_test.c  
#SOURCE += fax_svc_io_funcs.c
#SOURCE += fax_svc_driver_shim.c
SOURCE += fax_svc_modem_status.c
#ifdef HAVE_POWER_MGR
#SOURCE += fax_svc_pwr_drv.c
#endif

#SOURCE += faxer_debug.c

# Todo: set up config dir for generic file system mappings, etc
#CONFIG_SRC = 
#CONFIG_SRC_DIR = config
#CONFIG_SOURCE = 

include $(BUILD_ROOT)/stdtargets.mk
CCPARAM += -Wall -Werror -Os -fno-strict-aliasing

