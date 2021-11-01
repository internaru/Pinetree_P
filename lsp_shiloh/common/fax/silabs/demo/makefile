# ============================================================================
# Copyright (c) 2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================


#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = fax_dbg_net.h faxtest.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

#  objects to build:
SOURCE = fax_demo.c 
ifdef HAVE_NETWORK
SOURCE += fax_dbg_net.c
endif
SOURCE += fax_dbg_test.c

FAXTEST_SRC  = ../fax_app/include/fax_app_status.h
FAXTEST_SRC += ./include/faxtest.h
FAXTEST_GENERATED  = $(INSTALL_DEST_DIR)/faxtest_msgs.h
FAXTEST_GENERATED += $(INSTALL_DEST_DIR)/faxtest_msgs_str.h

# It is VERY important that bld_faxtest_msgs be in unix format but it is also
# important to not try to modify files that are under revision control. For this
# reason we make a mutable copy in the TARGETDIR and dos2unix it there.
$(TARGETDIR)/bld_faxtest_msgs : tools/bld_faxtest_msgs
	@echo "Dos2Unix'ing and Chmod'ing $(notdir $<)"
	@test -d $(dir $@) || $(MKPATH) $(dir $@)
	@$(CP) $< $@
	@dos2unix $@
	@chmod +x $@

$(FAXTEST_GENERATED) : $(TARGETDIR)/bld_faxtest_msgs $(FAXTEST_SRC)
	bash $(TARGETDIR)/bld_faxtest_msgs $(BUILD_ROOT) $(INSTALL_DEST_DIR)

$(SOURCE) : $(FAXTEST_GENERATED)

include $(BUILD_ROOT)/stdtargets.mk

CCPARAM += -Wall -Werror -Os -fno-strict-aliasing
