# ============================================================================
# Copyright (c) 2009-2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Include master environment
BUILD_ROOT = ../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./config -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

CLEAN_SRC = $(PRODUCT_PATH)/string_mgr/config/blob*.blb \
            $(PRODUCT_PATH)/string_mgr/config/blob*.c \
            $(PRODUCT_PATH)/string_mgr/config/fwEnglishStrings.txt \
            $(PRODUCT_PATH)/string_mgr/config/log*.txt \

INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = blobEnglish.c string_mgr_config.c

VPATH += ./config

include $(BUILD_ROOT)/stdtargets.mk

# override install rule with rule to run the 
# rules for the string manager
install: $(PRODUCT_PATH)/string_mgr/config/blobEnglish.c $(INSTALL_DEST_DIR)/string_mgr_config.h

$(PRODUCT_PATH)/string_mgr/config/blobEnglish.c: $(PRODUCT_PATH)/string_mgr/config/string_mgr_config.h $(MODULES_PATH)/common/string_mgr/version_$(HAVE_STRING_MGR)/tools/string_blobber.pl
	@echo --------------------------------------
	@echo Running String Blobber
	@echo --------------------------------------
	@-rm -f $(PRODUCT_PATH)/string_mgr/config/blob*.blb
	@-rm -f $(PRODUCT_PATH)/string_mgr/config/blob*.c
	@-rm -f $(PRODUCT_PATH)/string_mgr/config/fwEnglishStrings.txt
	@-rm -f $(PRODUCT_PATH)/string_mgr/config/log*.txt
	@perl $(MODULES_PATH)/common/string_mgr/version_$(HAVE_STRING_MGR)/tools/string_blobber.pl --path $(PRODUCT_PATH)/string_mgr/config

$(INSTALL_DEST_DIR)/string_mgr_config.h: ./config/string_mgr_config.h
	@echo INSTALLING string_mgr_config.h
	@test -d $(INSTALL_DEST_DIR) || $(MKPATH) $(INSTALL_DEST_DIR)
	@$(CP) ./config/string_mgr_config.h $(INSTALL_DEST_DIR)
