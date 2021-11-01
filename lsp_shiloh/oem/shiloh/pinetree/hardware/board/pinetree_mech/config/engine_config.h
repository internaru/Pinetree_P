/*
 * ============================================================================
 * Copyright (c) 2012   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef _ENGINE_CONFIG_H
#define _ENGINE_CONFIG_H

#define GPIO_BANK_REGI_CLUTCH   GPIO_BANK_G
#define GPIO_PIN_REGI_CLUTCH    30


// main tray pick
#define GPIO_BANK_PICK          GPIO_BANK_G
#define GPIO_PIN_PICK           27
#define GPIO_PIN_MPT_PICK       26
#define GPIO_PIN_DUP_SOL        25
#define GPIO_PIN_TX_SEN			0

#define GPIO_PIN_MOTOR_LOCK		13		// GPIO_D
#define GPIO_WAKE_KEY			14		// GPIO_F
#define GPIO_UI_NRESET			16		// GPIO_F
#define GPIO_SENSOR_PWR			17		// GPIO_F
#define GPIO_PIN_MAINFAN_LOCK	18		// GPIO_F
#define GPIO_PIN_SUBFAN_LOCK	19		// GPIO_F
#define GPIO_PIN_DEVFAN_LOCK	20		// GPIO_F

#define GPIO_BANK_THV_EN        GPIO_BANK_F
#define GPIO_PIN_THV_EN         31

#endif // _ENGINE_CONFIG_H

