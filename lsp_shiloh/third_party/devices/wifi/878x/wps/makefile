# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../../..

include $(BUILD_ROOT)/project.mk

MACROS += WPS_CHECK_AP_PBC

# specify paths of local header files
INCFLAGS = -I../mlan/src -I../mthreadx/src -I../mlaninterface/src 
INCFLAGS += -I../wps/src -I../supplicant/src -I../src -I../include
INCFLAGS += -I$(BUILD_ROOT)/network/apps/wsd/include 
ifeq ($(HAVE_WIFI_CERT_TEST), 1)
INCFLAGS += -I../sigma/inc
endif
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE = wps_state.c wps_msg.c wps_os_threadx.c wps_eapol.c wps_l2.c \
         wps_util.c wps_wlan.c wps_main.c encrypt.c dh.c key_algorim.c \
         wps_thread.c wps_start.c wfd_action.c wfd_main.c wps_events.c

include $(BUILD_ROOT)/stdtargets.mk
