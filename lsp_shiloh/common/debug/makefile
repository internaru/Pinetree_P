# ============================================================================
# Copyright (c) 2009-2010 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================


#Include master environment
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC      =
INSTALL_SRC     += debug.h
INSTALL_SRC     += dprintf.h
INSTALL_SRC     += logger.h
INSTALL_SRC     += dbg_api.h
INSTALL_SRC     += devices_logger.h
INSTALL_SRC     += dbg_raw_socket.h
INSTALL_SRC_DIR  = include
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

#  objects to build:
SOURCE  =
SOURCE += logger.c
SOURCE += logger_common.c
SOURCE += debug.c
SOURCE += dprintf.c
ifdef HAVE_RTOS
SOURCE += cpp_support.cpp
endif
SOURCE += dbg.c
SOURCE += dbg_assert.c
SOURCE += dbg_context.c
SOURCE += dbg_outbuf.c

ifdef HAVE_CMD
  SOURCE += dbg_cmd.c
  ifdef HAVE_NETWORK
    ifdef HAVE_DBG_TELNET
      SOURCE += dbg_telnet.c
    endif
    ifdef HAVE_DBG_RAW_SOCKET
      SOURCE += dbg_raw_socket.c
    endif
  endif
endif

ifdef HAVE_SYSTEM
SOURCE += debug_sys_msg.c
endif

ifdef HAVE_HTTP
  ifdef HAVE_DEBUG_OVER_HTTP
    SOURCE += dbghttp.c
  endif
endif

ifdef CPU_USAGE_PROFILER
  SOURCE += profile.c
endif

//SOURCE += dt_ether_drv.c
# ------------------------------------------------------------------------------
# RESET_ON_ASSERT
# When the system asserts (or catches a processor exception, etc.) it will
# display error information on the system console and then take one of two
# actions. It will either reset itself or it will enter into a tight CPU loop
# and await additional user interaction. The setting of RESET_ON_ASSERT controls
# this behavior. JTAG users, in particular, may want to make sure that
# RESET_ON_ASSERT is 0 so that they can do post-mortem debugging of the system.
# Release builds, for the field, should almost certainly have this set to 1.
# This logic is placed here because this setting is local to the debug system
# alone and need not affect recompilation of any other modules.
DEFAULT_RESET_ON_ASSERT_for_TYPE_debug=0
DEFAULT_RESET_ON_ASSERT_for_TYPE_release=1
RESET_ON_ASSERT ?= $(DEFAULT_RESET_ON_ASSERT_for_TYPE_$(TYPE))
MACROS += RESET_ON_ASSERT=$(RESET_ON_ASSERT)
$(if $(filter 0 1,$(RESET_ON_ASSERT)),,$(error RESET_ON_ASSERT must be 0 or 1))

# ------------------------------------------------------------------------------
# DEBUG_ENABLE
# By default debug messages are enabled. In some cases this produces a
# ridiculous amount of debug activity and some users may want to have a more
# sane console to use for issuing commands and receiving responses. Users may
# choose to build with DEBUG_ENABLE=0 in this case. Note that this is only the
# power on default. Debug messages may always be enabled/disabled by using the
# "dbg enable <setting>" cmd.
DEBUG_ENABLE ?= 1
MACROS += DEBUG_ENABLE=$(DEBUG_ENABLE)
$(if $(filter 0 1,$(DEBUG_ENABLE)),,$(error DEBUG_ENABLE must be 0 or 1))

# ------------------------------------------------------------------------------
# DEBUG_BLOCKING
# By default console output is blocking (unless generated from interrupt
# context). This has an affect on system performance. Non blocking behavior
# causes all console output to be flushed from a low priority background task.
# In both cases all output is flushed in te case of an assertion and shutdown.
# In the non-blocking case it is possible for messages to come so fast that the
# buffer overflows and messages are lost. Note that this is only the power on
# default. Debug blocking may always be enabled/disabled by using the
# "dbg blocking <setting>" cmd.
DEBUG_BLOCKING_release=0
DEBUG_BLOCKING_debug=0
DEBUG_BLOCKING ?= $(DEBUG_BLOCKING_$(TYPE))
MACROS += DEBUG_BLOCKING=$(DEBUG_BLOCKING)
$(if $(filter 0 1,$(DEBUG_BLOCKING)),,$(error DEBUG_BLOCKING must be 0 or 1))

# ------------------------------------------------------------------------------
# DEBUG_OUTBUF_SIZE
# By default there is a relatively small debug output buffer. If DEBUG_BLOCKING
# is on this can be very small (1 or 2k) but remember that debug blocking can be
# controlled at runtime and the buffer can easily overrun if it is small and
# non-blocking since the buffer is only flushed from a low priority background
# task. The size of the debug output buffer cannot be changed (at this time)
# at run time but is a fixed memory allocation. The size MUST be a power of 2
# and this is checked, by dbg_outbuf.c, at build time.
DEBUG_OUTBUF_SIZE ?= 8192
MACROS += DEBUG_OUTBUF_SIZE=$(DEBUG_OUTBUF_SIZE)

include $(BUILD_ROOT)/stdtargets.mk

