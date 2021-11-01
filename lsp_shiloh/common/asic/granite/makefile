# ============================================================================
# Copyright (c) 2011 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

#Disable code coverage
override HAVE_CODE_COVERAGE = 0

#Include master environment
BUILD_ROOT = ../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I. -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC = asic.h jbig_compatibility.h
INSTALL_SRC_DIR = include
CONFIG_SRC = mrvl_gpio_linux_config.h
CONFIG_SRC_DIR = config
OTHER_SRC = 
OTHER_SRC_DIR = 
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

# ------------------------------------------------------------------------------
# EFUSE driver 
# ------------------------------------------------------------------------------
ifdef HAVE_EFUSE
  CONFIG_SRC += efuse_config.h
  SOURCE += efuse_config.c
endif

# ------------------------------------------------------------------------------
# HiPS PLL device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_HIPS_PLL
  CONFIG_SRC += hips_pll_config.h
  SOURCE += hips_pll_config.c
endif

# ------------------------------------------------------------------------------
# dec laser device driver 
# ------------------------------------------------------------------------------

  CONFIG_SRC += dec_laser_regs_config.h
  SOURCE += dec_laser_regs_config.c

# ------------------------------------------------------------------------------
# JBIG driver 
# ------------------------------------------------------------------------------
ifdef HAVE_JBIG
  CONFIG_SRC += mrvl_jbig_config.h
  SOURCE += mrvl_jbig_config.c
endif

include $(BUILD_ROOT)/stdtargets.mk

