/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file mrvl_gpio_linux_config.h 
 *
 * \brief This file contains an API for the GPIO device
 * driver configuration layer
 *
 */
#ifndef INC_MRVL_GPIO_CONFIG_H
#define INC_MRVL_GPIO_CONFIG_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio_api.h"

/* ASIC GPIO config info */
#define NUM_GPIO_BANKS 15
#define NUM_GPIO_PINS_PER_BANK 32
#define NUM_GPIO_PINS (NUM_GPIO_BANKS*NUM_GPIO_PINS_PER_BANK)

#endif

