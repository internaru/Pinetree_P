# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS =  -I./src -I$(PRODUCT_PATH)/http/config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I./src/include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)/openssl

INSTALL_SRC = http_api.h
INSTALL_SRC_DIR = include
OTHER_SRC = http_vars.h
OTHER_SRC_DIR = src/include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE = http_oid_lut.c
SOURCE += http_vars.c
SOURCE += htmlssi.c
SOURCE += http.c 

ifdef HAVE_HTTP_TEST
    MACROS += HAVE_HTTP_TEST
    SOURCE += http_registered_callback_test.c
endif

ifdef HAVE_USB_DEVICE
    SOURCE += http_usbd_parser.c 
endif

ifdef HAVE_NETWORK
    SOURCE += http_network_parser.c 
endif

include $(BUILD_ROOT)/stdtargets.mk
