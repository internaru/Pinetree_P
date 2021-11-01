# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS  = -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)
INCFLAGS += -Isrc

INSTALL_SRC  = cmd_interp_api.h
INSTALL_SRC_DIR = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)


SOURCE_tcl    = cmd_interp_tcl.c
SOURCE_tcl   += cmd_interp_tcl_procs.c
SOURCE_simple = cmd_interp_simple.c

SOURCE += $(SOURCE_$(CMD_INTERP_TYPE))

CLEAN_SRC += cmd_interp_tcl_procs.c

.PRECIOUS : %.c
%.c : %.tcl
	echo "Converting $(notdir $<):"
	echo "const char $(basename $(notdir $<))[] = {" > $@
	od -t x1 --width=16 < $< | sed -e 's/^[^ ]*//' \
                                  -e 's/ /0x/g' \
                                  -e 's/0x../&,/g' >> $@
	echo "0};" >> $@
vpath %.tcl $(SRCDIR)


include $(BUILD_ROOT)/stdtargets.mk


