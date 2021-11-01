# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE) -I$(MODULES_PATH)/common/http/src/include

INSTALL_SRC =
INSTALL_SRC_DIR =
OTHER_SRC =
OTHER_SRC_DIR =
CLEAN_SRC = $(PRODUCT_PATH)/http/config/http_file_mappings.h.preproced \
            $(PRODUCT_PATH)/http/config/http_rom.c $(PRODUCT_PATH)/http/config/http_rom.h
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = http_translate_oid_to_xml_midas.c http_translate_oid_to_html.c http_config.c http_rom.c http_methods_support.c
SOURCE += http_oid_lut_config.c

ifdef WEB_SERVER_PORT
    MACROS += WEB_SERVER_PORT=$(WEB_SERVER_PORT)
endif
ifdef HTTPS_SERVER_PORT
    MACROS += HTTPS_SERVER_PORT=$(HTTPS_SERVER_PORT)
endif

VPATH += ./config

include $(BUILD_ROOT)/stdtargets.mk

# override install rule with rule to run the HTML preprocessor
# rule for the http files
install: $(PRODUCT_PATH)/http/config/http_rom.h

$(PRODUCT_PATH)/http/config/http_rom.h: $(PRODUCT_PATH)/http/config/http_file_mappings.h $(PRODUCT_PATH)/http/config/html/input/* $(PRODUCT_PATH)/http/config/html/input/images/* $(PRODUCT_PATH)/http/config/html/input/js/* $(PRODUCT_PATH)/http/config/xml/input/* $(PRODUCT_PATH)/http/config/xml/input/supplies/* $(PRODUCT_PATH)/http/config/xml/input/functions/* $(PRODUCT_PATH)/http/config/xml/input/io-connections/* $(MODULES_PATH)/common/http/tools/*.pl $(PRODUCT_PATH)/http/config/*.pl
	@echo --------------------------------------
	@echo Processing XML/HTML files
	@echo --------------------------------------
	@-rm -rf $(PRODUCT_PATH)/http/config/tmpin
	@-mkdir $(PRODUCT_PATH)/http/config/tmpin
	@cp -R $(PRODUCT_PATH)/http/config/html/input/* $(PRODUCT_PATH)/http/config/tmpin
	@cp -R $(PRODUCT_PATH)/http/config/xml/input/* $(PRODUCT_PATH)/http/config/tmpin
	@$(CC) -E $(CCPARAM) -c $(INCPATH_MACROS) $(PRODUCT_PATH)/http/config/http_file_mappings.h -o $(PRODUCT_PATH)/http/config/http_file_mappings.h.preproced
	@perl $(MODULES_PATH)/common/http/tools/http_preprocessor.pl --common_path $(MODULES_PATH)/common --product_path $(PRODUCT_PATH)
	@-rm -rf $(PRODUCT_PATH)/http/config/http_file_mappings.h.preproced
	@-rm -rf $(PRODUCT_PATH)/http/config/tmpin
	@-rm -rf $(PRODUCT_PATH)/http/config/tmpout

