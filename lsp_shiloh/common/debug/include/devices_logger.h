/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef DEVICES_LOGGER_H
#define DEVICES_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * register DEBUG_LOGGER_MODULE_DEVICES submodules here 
 *  
 * NOTE: bits 0-5 (0x0000003f) are reserved 
 *  
 */



#define DEVICES_LOGGER_SUBMOD_GPIO_LED      LOGGER_SUBMODULE_BIT( 1 )
#define DEVICES_LOGGER_SUBMOD_GPIO_LOGGER   LOGGER_SUBMODULE_BIT( 2 )
#define DEVICES_LOGGER_SUBMOD_DEC_ADC       LOGGER_SUBMODULE_BIT( 3 )
#define DEVICES_LOGGER_SUBMOD_DEC_LASER     LOGGER_SUBMODULE_BIT( 4 )
#define DEVICES_LOGGER_SUBMOD_DEC_FUSER     LOGGER_SUBMODULE_BIT( 5 )
#define DEVICES_LOGGER_SUBMOD_DEC_SENSOR    LOGGER_SUBMODULE_BIT( 6 )
#define DEVICES_LOGGER_SUBMOD_DCMOTOR       LOGGER_SUBMODULE_BIT( 7 )
#define DEVICES_LOGGER_SUBMOD_STEPPER       LOGGER_SUBMODULE_BIT( 8 )
#define DEVICES_LOGGER_SUBMOD_PWM           LOGGER_SUBMODULE_BIT( 9 )
#define DEVICES_LOGGER_SUBMOD_DELAY         LOGGER_SUBMODULE_BIT( 10 )
#define DEVICES_LOGGER_SUBMOD_ETH_MAC       LOGGER_SUBMODULE_BIT( 11 )
#define DEVICES_LOGGER_SUBMOD_ETH_PHY       LOGGER_SUBMODULE_BIT( 12 )
#define DEVICES_LOGGER_SUBMOD_CDMA          LOGGER_SUBMODULE_BIT( 13 )
#define DEVICES_LOGGER_SUBMOD_CRYPTO        LOGGER_SUBMODULE_BIT( 14 )
#define DEVICES_LOGGER_SUBMOD_BOOTSPI       LOGGER_SUBMODULE_BIT( 15 )

#ifdef __cplusplus
}
#endif

#endif // DEVICES_LOGGER_H


