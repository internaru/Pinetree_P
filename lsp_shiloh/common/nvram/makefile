# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./tests -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

ifdef HAVE_NVRAM_FILE
INSTALL_SRC = nvram_api.h
else
INSTALL_SRC = nvram_api.h partitionMgrAPI.h rom_id_mapping.h
INSTALL_SRC += nvram_fwupdate.h
endif

ifdef HAVE_NVRAM_FILE
CONFIG_SRC = 
OTHER_SRC = 
else
CONFIG_SRC = 
OTHER_SRC = rommgrapi.h imageMapUtil.h nodeCache.h code_table.h
OTHER_SRC += memdevice.h
endif

INSTALL_SRC_DIR = include
CONFIG_SRC_DIR = config
OTHER_SRC_DIR = src
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


#  objects to build:
ifdef HAVE_NVRAM_FILE
SOURCE =
else
SOURCE = imageMapUtil.c memdevice.c nodeCache.c \
	 nvram_dev_eeprom.c nvram_dev_shim.c nvram_dev_spi.c partitionMgr.c \
	 regmemdev.c rommgrapi.c code_table.c crc32.c nvram.c nvram_dev_nand.c

ifdef HAVE_CMD
SOURCE += bootspi_cmd.c
endif
endif

include $(BUILD_ROOT)/stdtargets.mk
