# ============================================================================
# Copyright (c) 2009-2010,2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./config -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC  = agLinkedList.h
# third-party private header needed by map.c
INCFLAGS += -I$(BUILD_ROOT)/third_party/uthash/
#CCPARAM += -E

INSTALL_SRC += base64_api.h
INSTALL_SRC += BigBuffer.h
INSTALL_SRC += dma_buffer.h
INSTALL_SRC += dma_alloc_api.h
INSTALL_SRC += byteQ_api.h
INSTALL_SRC += checksum.h
INSTALL_SRC += cppenumops.h
INSTALL_SRC += deviceid.h
INSTALL_SRC += event_observer.h
ifdef HAVE_SYSTEM
INSTALL_SRC += file_to_pipe_api.h
INSTALL_SRC += pipe_io_device.h
endif
INSTALL_SRC += ioutils.h
INSTALL_SRC += key_table.h
INSTALL_SRC += list.h
INSTALL_SRC += mlist.h
INSTALL_SRC += fifolist.h
INSTALL_SRC += readfile_into_ram.h
INSTALL_SRC += writefile_from_ram.h
INSTALL_SRC += regutils.h
INSTALL_SRC += response_subject.h
# INSTALL_SRC += threads_api.h
INSTALL_SRC += tree_unordered.h
INSTALL_SRC += utf8.h
INSTALL_SRC += utils.h
INSTALL_SRC += cppdefs.h
INSTALL_SRC += lehmer.h
INSTALL_SRC += map_mem.h
ifdef HAVE_SYSTEM
INSTALL_SRC += uuid.h
INSTALL_SRC += model_info.h
endif
INSTALL_SRC += map_api.h

INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

ifdef HAVE_WIRELESS
INCFLAGS += -I $(BUILD_ROOT)/third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/wps/src/
INCFLAGS += -I $(BUILD_ROOT)/third_party/devices/wifi/$(WIRELESS_MODULE_TYPE)/src/

# openssl headers
INCFLAGS += -I $(PRJROOT)/sysapps/include
endif

#  objects to build:
SOURCE += base64.c
SOURCE += BigBuffer.c
SOURCE += dma_buffer.c
SOURCE += dma_alloc_app.c
SOURCE += map_mem.c
ifdef HAVE_BYTEQ
SOURCE += byteQ.c
endif
SOURCE += checksum.c
SOURCE += deviceid.c
SOURCE += endian.c
SOURCE += event_observer.c
ifdef HAVE_SYSTEM
SOURCE += file_to_pipe.c
SOURCE += pipe_io_device.c
endif
SOURCE += funcpoll.c
SOURCE += key_table.c
SOURCE += list.c
SOURCE += mlist.c
SOURCE += fifolist.c
SOURCE += readfile_into_ram.c
SOURCE += writefile_from_ram.c
SOURCE += regpoll.c
SOURCE += str.c
LSPTODO_SOURCE += threads.c
SOURCE += tree_unordered.c
SOURCE += utf8.c
SOURCE += utilstest.c
SOURCE += lehmer.c
ifdef HAVE_SYSTEM
SOURCE += uuid.c
SOURCE += model_info.c
endif
SOURCE += map.c

include $(BUILD_ROOT)/stdtargets.mk


