
   /*
    *
    * ============================================================================
    * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
    *
    *                         Marvell Confidential
    * ============================================================================
    */        
/**
         CDMA channel defines
         */

#ifndef CDMA1_H
#define CDMA1_H

#ifdef __KERNEL__
    #include <linux/types.h>
#else
    #include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define CDMA_A_PPUART_TX                           58
#define CDMA_A_PPUART_RX                           57
#define CDMA_A_PENSIVE_UART_TX                     56
#define CDMA_A_PENSIVE_UART_RX                     55
#define CDMA_A_PARALLEL_PORT_RX                    54
#define CDMA_A_SHA_TX                              53
#define CDMA_MVDO_EDGE_RX                          52
#define CDMA_MVDO_MIRROR_2_RX                      51
#define CDMA_MVDO_MIRROR_1_RX                      50
#define CDMA_PLANAR_DATA_2_TX                      49
#define CDMA_PLANAR_DATA_1_TX                      48
#define CDMA_PLANAR_DATA_0_TX                      47
#define CDMA_STATS_PIPE_ACCESS_RX                  46
#define CDMA_STATS_AHS_2_RX                        45
#define CDMA_STATS_AHS_1_RX                        44
#define CDMA_STATS_AHS_0_RX                        43
#define CDMA_ADC_2_RX                              42
#define CDMA_ADC_1_RX                              41
#define CDMA_STEPPER_MOTOR_4PIN_1_TX               40
#define CDMA_STEPPER_MOTOR_4PIN_0_TX               39
#define CDMA_STEPPER_MOTOR_6PIN_3_TX               38
#define CDMA_STEPPER_MOTOR_6PIN_2_TX               37
#define CDMA_STEPPER_MOTOR_6PIN_1_TX               36
#define CDMA_STEPPER_MOTOR_6PIN_0_TX               35
#define CDMA_SCCP_1_TX                             34
#define CDMA_SCCP_1_RX                             33
#define CDMA_SCCP_0_TX                             32
#define CDMA_SCCP_0_RX                             31
#define CDMA_IEEE1284_PARALLEL_RX                  30
#define CDMA_EXTERNAL_DMA                          29
#define CDMA_SILABS_VOICE_CODEC_TX                 28
#define CDMA_SILABS_VOICE_CODEC_RX                 27
#define CDMA_SILABS_DAA_TX                         26
#define CDMA_SILABS_DAA_RX                         25
#define CDMA_AUDIO_DDAC_TX                         24
#define CDMA_I2S_TX                                23
#define CDMA_I2S_RX                                22
#define CDMA_I2C_3_TX                              21
#define CDMA_I2C_3_RX                              20
#define CDMA_I2C_2_TX                              19
#define CDMA_I2C_2_RX                              18
#define CDMA_I2C_1_TX                              17
#define CDMA_I2C_1_RX                              16
#define CDMA_I2C_0_TX                              15
#define CDMA_I2C_0_RX                              14
#define CDMA_SPI_2_TX                              13
#define CDMA_SPI_2_RX                              12
#define CDMA_SPI_1_TX                              11
#define CDMA_SPI_1_RX                              10
#define CDMA_SPI_0_TX                              9
#define CDMA_SPI_0_RX                              8
#define CDMA_UART_3_TX                             7
#define CDMA_UART_3_RX                             6
#define CDMA_UART_2_TX                             5
#define CDMA_UART_2_RX                             4
#define CDMA_UART_1_TX                             3
#define CDMA_UART_1_RX                             2
#define CDMA_UART_0_TX                             1
#define CDMA_UART_0_RX                             0

#ifdef __cplusplus
}
#endif
#endif
