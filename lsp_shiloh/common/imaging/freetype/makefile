# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================


#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = 
INCFLAGS += -I./src
INCFLAGS += -I./include
INCFLAGS += -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -I$(BUILD_ROOT)/third_party/freetype2/freetype-2.4.4/include

INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INSTALL_SRC       =
INSTALL_SRC      += freetype_api.h
INSTALL_SRC      += ft_services.h

SOURCE += freetype.c
SOURCE += freetype_cmd.c
SOURCE += ft_services.c

FREETYPE_DEFAULT_FONTFILE = fonts/default.ttf
MACROS += FREETYPE_DEFAULT_FONTFILE=\"$(FREETYPE_DEFAULT_FONTFILE)\"
ROMFILES = DroidSans.ttf:$(FREETYPE_DEFAULT_FONTFILE)

include $(BUILD_ROOT)/stdtargets.mk


