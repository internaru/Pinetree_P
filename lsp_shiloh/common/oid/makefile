# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I./src/include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I$(OEM_PATH)/common/network/manager/config 

INSTALL_SRC = oid_api.h
INSTALL_SRC += oid_producer_api.h
ifdef HAVE_SM_JOB
INSTALL_SRC += oid_sm_api.h
endif
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = oid_controller.c oid_producer.c oid_module_memory.c oid_module_feature_support.c

ifdef HAVE_UNIT_TEST
SOURCE += oid_unit_test.c
endif

ifdef HAVE_COMMON_CONSUMABLES_API
SOURCE += oid_module_consumable.c
endif

ifdef HAVE_PLATFORM
SOURCE += oid_module_platform.c
endif

ifdef HAVE_PRINT_SUPPORT
SOURCE += oid_module_status.c
SOURCE += oid_module_print.c
endif

ifdef HAVE_STRING_MGR
SOURCE += oid_module_string_mgr.c
endif

ifdef HAVE_HTTP
SOURCE += oid_module_http.c
endif

ifeq ($(HAVE_USB_MASS_STORAGE), 1)
SOURCE += oid_module_rem_storage.c
endif

ifdef HAVE_SCAN_SUPPORT
SOURCE += oid_module_scan.c
endif

ifdef HAVE_USB_DEVICE
SOURCE += oid_module_usb_device.c
endif

ifdef HAVE_NETWORK
SOURCE += oid_module_network.c
endif

ifdef HAVE_SM_JOB
SOURCE += oid_module_sm.c
endif

ifdef HAVE_WLAN
SOURCE += oid_module_wifi.c
endif

ifdef HAVE_GCPP
SOURCE += oid_module_gcpp.c
endif

ifdef HAVE_FAX
ifneq ($(HAVE_FAX), silabs)
SOURCE += oid_module_fax.c
endif
endif

ifdef HAVE_UNIT_TEST
SOURCE += oid_unit_test.c
endif

include $(BUILD_ROOT)/stdtargets.mk
