# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../../..

include $(BUILD_ROOT)/project.mk

ifdef CONFIG_EAPOL_TEST
	DEFINES += -Werror -DEAPOL_TEST
endif

ifdef CONFIG_WIRELESS_EXTENSION
	DEFINES += -DCONFIG_WIRELESS_EXTENSION
endif

# specify paths of local header files
INCFLAGS = -I../supplicant/src -I../mlan/src -I../mthreadx/src  
INCFLAGS += -I../mlaninterface/src -I../wps/src  -I../src -I../include 
ifeq ($(HAVE_WIFI_CERT_TEST), 1)
INCFLAGS += -I../sigma/inc
endif
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# specify source files to build
SOURCE = config.c eloop.c common.c aes.c md5.c rc4.c sha1.c sha256.c \
         wpa.c config_datastore.c l2_packet_raw.c tls_none.c preauth.c \
         aes_wrap.c wpa_supplicant.c events.c drivers.c \
         os_threadx.c driver_marvell.c driver_wext.c main_threadx.c

include $(BUILD_ROOT)/stdtargets.mk
