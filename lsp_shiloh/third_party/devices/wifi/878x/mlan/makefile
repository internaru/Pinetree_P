# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../../..

include $(BUILD_ROOT)/project.mk

# specify paths of local header files
INCFLAGS = -I../mlan/src -I../mthreadx/src -I../mlaninterface/src 
INCFLAGS += -I../wps/src -I../supplicant/src -I../src -I../include 
INCFLAGS += -I../../../network/treck/include
ifeq ($(HAVE_WIFI_CERT_TEST), 1)
INCFLAGS += -I../sigma/inc
endif
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE  = mlan_11d.c mlan_11n.c mlan_11n_aggr.c mlan_11n_rxreorder.c mlan_cfp.c
SOURCE += mlan_cmdevt.c mlan_init.c mlan_join.c mlan_misc.c mlan_module.c mlan_scan.c
SOURCE += mlan_shim.c mlan_sta_cmd.c mlan_sta_cmdresp.c mlan_sta_event.c mlan_sta_ioctl.c
SOURCE += mlan_sta_rx.c mlan_sta_tx.c mlan_txrx.c mlan_uap_cmdevent.c mlan_uap_ioctl.c
SOURCE += mlan_uap_txrx.c mlan_usb.c mlan_wmm.c mlan_11h.c mlan_meas.c

include $(BUILD_ROOT)/stdtargets.mk
