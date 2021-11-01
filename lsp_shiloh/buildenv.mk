# ============================================================================
# Copyright (c) 2006-2011   Marvell International, Ltd. All Rights Reserved
#
#                         Marvell Confidential
#                      
# ============================================================================


# ARM toolchain
# define ARM_ROOT in your environment or specify the path below
# best practice for cygwin or linux is:
# armgcc is common softlink name pointing to the gcc toolchain for your platform
# fully describing the version of the toolchain with date_version_endian_host etc is helpful
# ln -s /opt/mrvlcompiler/2009q3-18-4.2.0_little-endian_host-i686-linux /opt/armgcc 
ifndef ARM_ROOT
ARM_ROOT := /opt/armgcc
#ARM_ROOT := /opt/marvellarm
#ARM_ROOT := /opt/armcc
endif

RM := rm -f
RM_R := rm -rf
CP	:= cp -f
MKPATH	:= mkdir -p
CMP     := cmp
ECHO    := echo
PERL	:= perl
export PERL

ifeq ($(CPU_TYPE), X86)
  AS := as
  CC := gcc
  CPP := g++
#  LD := ld
  LD := g++
  OBJDUMP := objdump
  ELFSTRIP := strip
  SPLIT := split
  AR := ar
  ARPARAM := rs
  COVCC  = $(CC)
  COVCPP = $(CPP)

CCPARAM += -g -Wall -Werror -Wno-unused-but-set-variable -D _GNU_SOURCE

ifeq ($(TYPE), debug)
    MACROS += DEBUG
    MACROS += HAVE_DBG_PRINTF
    CCPARAM += -O0
else ifeq ($(TYPE), release)
    CCPARAM += -Os
else
	$(error Unsupported build type '$(TYPE)' - supported build types are: 'debug' 'release')
endif

# Get the path to the C Compiler in a variable
CC_EXE := $(shell readlink -f `which $(CC)`)
CC_EXE_PATH := $(shell dirname $(CC_EXE))

# One dir (bin) up from the CC is the base of the Tools package
TOOLS_BASE := $(CC_EXE_PATH)/..

# Libraries are always found in this path relative to the tools base
#LIB_BASE := $(TOOLS_BASE)/lib/gcc/arm-marvell-eabi/$(CC_VERSION)
LIB_BASE := /usr/lib

# Set a Libs search var for the CC line
export LIBPATH= $(LIB_BASE)
# -L /usr/lib/gcc/x86_64-linux-gnu/4.4.5 -L /usr/lib/gcc/x86_64-linux-gnu/4.4.5/../lib

STD_LIBS = pthread rt m c gcc gcc_eh

MACROS += CPU_LITTLE_ENDIAN

else 
#ARM toolchain
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

ifeq ($(HAVE_CODE_COVERAGE), bullseye)
  ifndef BULLSEYE_TOOLS_ROOT
    BULLSEYE_TOOLS_ROOT = /opt/bullseye/bin
  endif
  COVFILE := $(realpath $(BUILD_ROOT))/$(PRODUCT)-$(TYPE)-$(HAVE_CODE_COVERAGE).cov
  COVCC   := $(BULLSEYE_TOOLS_ROOT)/covc -f$(COVFILE) -q $(CC)
  COVCPP  := $(BULLSEYE_TOOLS_ROOT)/covc -f$(COVFILE) -q $(CPP)
else
  COVCC  := $(CC)
  COVCPP := $(CPP)
endif

# Determine the compiler version. Use this information to determine the 
# correct path for the libraries.
export CC_VERSION := $(shell  $(CC) -v 2>&1 | grep "gcc version" | cut -d' ' -f3)
export CC_FULL_VERSION := $(shell $(CC) -v 2>&1 | grep "gcc version" | cut -d' ' -f3,9)


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

ifdef CPU_LITTLE_ENDIAN
    ENDIAN_CC_PARAM = -mlittle-endian
    ENDIAN_AS_PARAM = -EL
    MACROS += CPU_LITTLE_ENDIAN
endif



ifeq ($(TYPE), debug)
    MACROS += DEBUG
    MACROS += HAVE_DBG_PRINTF
    CCPARAM = -O0 
else ifeq ($(TYPE), release)
    CCPARAM = -Os
else
	$(error Unsupported build type '$(TYPE)' - supported build types are: 'debug' 'release')
endif

# Add stack frame generation (default 1)
# Build with, for example, "make .... STACK_FRAMES=0" to override this default
STACK_FRAMES ?= 1
STACK_FRAMES_1 = -mapcs-frame
CCPARAM += $(STACK_FRAMES_$(STACK_FRAMES))

# The following are unconditional
ASPARAM = -g $(ENDIAN_AS_PARAM) 
CCPARAM += -g -Wall $(ENDIAN_CC_PARAM) -D _GNU_SOURCE

ifeq ($(CC_VERSION), 4.6.4)
    CCPARAM += -Wno-unused-but-set-variable -Wno-switch
endif

# your project must specify ERROR_ON_WARNING=1|0.
# When you specify ERROR_ON_WARNING=1 then warnings
# will be treated as errors by the compiler.
# You may override this at the make command line by issuing:
# make ... ERROR_ON_WARNING=1|0
ERROR_ON_WARNING_1=-Werror
ERROR_ON_WARNING_0=
CCPARAM += $(ERROR_ON_WARNING_$(ERROR_ON_WARNING))
$(if $(filter 0 1,$(ERROR_ON_WARNING)),,$(error ERROR_ON_WARNING must be 0 or 1))

ifeq ($(CPU_TYPE), ARM_R4)
    CCPARAM += -Wno-unknown-pragmas -fno-strict-aliasing
    CCPARAM += -march=armv7-r -mtune=marvell-f
    ASPARAM += -march=armv7-r -mfloat-abi=soft
else ifeq ($(CPU_TYPE), ARM_PJ1)
    CCPARAM += -Wno-unknown-pragmas
    #CCPARAM += -mmarvell-div
    CCPARAM += -mcpu=marvell-f

  ifdef HAVE_WMMX
    ASPARAM += -mcpu=marvell-f+iwmmxt+iwmmxt2
    CCPARAM += -mmrvl-use-iwmmxt
  else
    ASPARAM += -mcpu=marvell-f
  endif
else ifeq ($(CPU_TYPE), ARM_PJ4)
    CCPARAM += -Wno-unknown-pragmas -fno-strict-aliasing
    CCPARAM += -mmarvell-div
    CCPARAM += -mcpu=marvell-fv7

  ifdef HAVE_WMMX
    ASPARAM += -mcpu=marvell-fv7+iwmmxt+iwmmxt2
    CCPARAM += -mmrvl-use-iwmmxt
  else
    ASPARAM += -mcpu=marvell-fv7
  endif
else ifeq ($(CPU_TYPE), ARM_A7)
    CCPARAM += -Wno-unknown-pragmas -fno-strict-aliasing	
  
else
    $(error Unsupported CPU type '$(CPU_TYPE)' - supported CPU types are: ARM_R4 ARM_PJ1 ARM_PJ4)
endif

ifdef FLOAT_ABI
   CCPARAM += -mfloat-abi=$(FLOAT_ABI)
   ASPARAM += -mfloat-abi=$(FLOAT_ABI)
else
   CCPARAM += -mfloat-abi=soft
   ASPARAM += -mfloat-abi=soft
endif

endif # CPU_TYPE X86 vs ARM

CPPPARAM = $(CCPARAM) 

ifeq ($(TYPE), debug)
BLDCFGSTR=Build Configuration: GCC Debug Build
else 
ifeq ($(TYPE), release)
BLDCFGSTR=Build Configuration: GCC Release Build
else
BLDCFGSTR=!!!Build Configuration not specified.  Defaulting to GCC Debug Build!!!
endif
endif  

SRCDIR=src
LIBDIR=lib
CFGDIR=config
TESTDIR=tests

