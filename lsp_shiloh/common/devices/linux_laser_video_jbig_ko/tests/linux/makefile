# ============================================================================
# Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

ifndef ARM_ROOT
ARM_ROOT := /opt/armgcc
endif

RM := rm -f
RM_R := rm -rf
CP	:= cp -f
MKPATH	:= mkdir -p
CMP     := cmp
ECHO    := echo
PERL	:= perl
export PERL

CCPREFIX := arm-marvell-linux-gnueabi-
AS := $(CCPREFIX)as
CC := $(CCPREFIX)gcc
CPP := $(CCPREFIX)g++
LD := $(CCPREFIX)g++
#  LD := $(CCPREFIX)ld
OBJDUMP := $(CCPREFIX)objdump
ELFSTRIP := $(CCPREFIX)strip
SPLIT := $(CCPREFIX)split
AR := $(CCPREFIX)ar
ARPARAM := rs

# Get the path to the C Compiler in a variable
CC_EXE := $(shell readlink -f `which $(CC)`)
CC_EXE_PATH := $(shell dirname $(CC_EXE))

# One dir (bin) up from the CC is the base of the Tools package
TOOLS_BASE := $(CC_EXE_PATH)/..

# Libraries are always found in this path relative to the tools base
LIB_BASE := $(TOOLS_BASE)/lib/gcc/arm-marvell-linux-gnueabi/$(CC_VERSION)

# Set a Libs search var for the CC line
export LIBPATH= -L $(LIB_BASE)


# *** DO NOT MODIFY *** 
# use TOOLS_BASE to specify the platform-specific lib path
# export LIBPATH = 
STANDARD_LIBRARY_LOCATION := $(TOOLS_BASE)/arm-marvell-linux-gnueabi/lib
LIBPATH += $(STANDARD_LIBRARY_LOCATION)
# add our build root to the library path because we will put
# the modified libc library there.
LIBPATH += $(BUILD_ROOT) 

STD_LIBS = pthread rt m c gcc gcc_eh

ENDIAN_CC_PARAM = -mlittle-endian
ENDIAN_AS_PARAM = -EL

# The following are unconditional
ASPARAM = -g $(ENDIAN_AS_PARAM) 
CCPARAM += -g -Wall $(ENDIAN_CC_PARAM) -D _GNU_SOURCE

ifeq ($(CC_VERSION), 4.6.4)
    CCPARAM += -Wno-unused-but-set-variable -Wno-switch
endif

LDPARAM = -Wl,--warn-common $(ENDIAN_CC_PARAM)
ifeq ($(TYPE), debug)
#allow the backtrace() library routine to provide full symbol info -jrs
LDPARAM += -rdynamic
endif

CFLAGS += -I. -I../../include -I../../src
define link_rules
	@echo Linking $@
	$(LD) $(LDPARAM) -o $@ $(filter %.o,$^)\
	   $(foreach l,$(STD_LIBS),-l $(l)) $(foreach l,$(LIBPATH),-L $(l))
	@echo done.
	@echo ------------------------------------------
endef

TARGETS = vid_test

.PHONY: all $(TARGETS)

all: $(TARGETS)

vid_test: dma_alloc_app.o list.o vid_test.o
	$(call link_rules)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCPARAM) $(CFLAGS) -I.

clean:
	rm -f *.o $(TARGETS)

