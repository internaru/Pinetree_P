# ============================================================================
# Copyright (c) 2010, 2012 Marvell Semiconductor, Inc. All Rights Reserved
#                      
#                         Marvell Confidential
# ============================================================================

 
#Include master environment
BUILD_ROOT = ../../../../../..
include $(BUILD_ROOT)/project.mk

INCFLAGS = -I./src -I./include -I$(BUILD_ROOT)/$(GLOBAL_INCLUDE)

INSTALL_SRC =
INSTALL_SRC_DIR =
CONFIG_SRC = 
CONFIG_SRC_DIR = config
INSTALL_DEST_DIR = $(BUILD_ROOT)/$(GLOBAL_INCLUDE)

MODULE_LIBPATH := $(foreach LIB, $(LIBPATH), $(BUILD_ROOT)/$(LIB))

# ------------------------------------------------------------------------------
# bootspi device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_BOOTSPI
  CONFIG_SRC += bootspi_config.h
  SOURCE += bootspi_config.c
endif

# ------------------------------------------------------------------------------
# display device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DISPLAY
  CONFIG_SRC += display_config.h
endif

# ------------------------------------------------------------------------------
# gpio led device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_GPIO_LED
  ifdef HAVE_ENGINE_STUB
    SOURCE += gpio_led_engine_stub.c
  else
    SOURCE += gpio_led_config.c
  endif
endif

# ------------------------------------------------------------------------------
# hwconfig device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_HWCONFIG
  SOURCE += hwconfig.c
endif

# ------------------------------------------------------------------------------
# pwm device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_PWM
  SOURCE += pwm_config.c 
endif


# ------------------------------------------------------------------------------
# stepper device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_STEPPER
#  SOURCE += 
endif


# ------------------------------------------------------------------------------
# dec sensor device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_SENSOR
  SOURCE += dec_sensor_config.c
endif


# ------------------------------------------------------------------------------
# dec adc device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_ADC
  SOURCE += dec_adc_config.c
endif


# ------------------------------------------------------------------------------
# dec fuser device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_FUSER
  SOURCE += dec_fuser_config.c
endif


# ------------------------------------------------------------------------------
# dec laser device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DEC_LASER
  SOURCE += dec_laser_config.c
endif

# ------------------------------------------------------------------------------
# dc motor device driver 
# ------------------------------------------------------------------------------
ifdef HAVE_DCMOTOR
  INCFLAGS += -I$(BUILD_ROOT)/common/devices/dcmotor/src
  SOURCE += dcmotor_config.c
endif

# ------------------------------------------------------------------------------
# dc motor device driver - open loop
# ------------------------------------------------------------------------------
ifdef HAVE_DCMOTOR_OL_OEM
  INCFLAGS += -I$(OEM_PATH)/common/devices/dcmotor_ol_oem/src
  SOURCE += dcmotor_ol_config.c
endif

# ------------------------------------------------------------------------------
# Print Engine configuration
# ------------------------------------------------------------------------------
ifdef PRINT_MECH
  CONFIG_SRC += engine_config.h
endif

CONFIG_SRC += raster_odma_cfg.h
SOURCE += raster_odma_cfg.c


include $(BUILD_ROOT)/stdtargets.mk
