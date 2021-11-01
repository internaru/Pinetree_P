
/*
 *
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */        
/**
      \file intnums.h

      Defines for the interrupt numbers for the main processor
      */

#ifndef INTNUMS_H
#define INTNUMS_H


#ifdef __cplusplus
extern "C" {
#endif


/*
	Interrupts on the GIC
*/

#define INTNUM_DEC_SECONDARY_IC_1_FIQ			136
#define INTNUM_DEC_SECONDARY_IC_1_IRQ			135
#define INTNUM_DEC_SECONDARY_IC_0_FIQ			134
#define INTNUM_DEC_SECONDARY_IC_0_IRQ			133
#define INTNUM_ANTICOUNTERFEITING			132
#define INTNUM_ASELA			131
#define INTNUM_AGATE			130
#define INTNUM_VCF			129
#define INTNUM_PIP			128
#define INTNUM_UPC_NORM_1			127
#define INTNUM_UPC_NORM_0			126
#define INTNUM_LASER_VIDEO_7			125
#define INTNUM_LASER_VIDEO_6			124
#define INTNUM_LASER_VIDEO_5			123
#define INTNUM_LASER_VIDEO_4			122
#define INTNUM_LASER_VIDEO_3			121
#define INTNUM_LASER_VIDEO_2			120
#define INTNUM_LASER_VIDEO_1			119
#define INTNUM_LASER_VIDEO_0			118
#define INTNUM_CISX			117
#define INTNUM_ICE_TEST			114
#define INTNUM_SCAN			113
#define INTNUM_SIZAR			112
#define INTNUM_PIC			111
#define INTNUM_PIE			110
#define INTNUM_CDMA			109
#define INTNUM_MOTORS_6			108
#define INTNUM_MOTORS_5			107
#define INTNUM_MOTORS_4			106
#define INTNUM_MOTORS_3			105
#define INTNUM_MOTORS_2			104
#define INTNUM_MOTORS_1			103
#define INTNUM_MOTORS_0			102
#define INTNUM_M2M			101
#define INTNUM_PCIE			100
#define INTNUM_USB2_DUAL_MODE			99
#define INTNUM_USB2_HOST			98
#define INTNUM_USB2_DEVICE			97
#define INTNUM_LAN			96
#define INTNUM_SILABS_VOICE_CODEC			95
#define INTNUM_SILABS_DAA			94
#define INTNUM_SPI_2			93
#define INTNUM_SPI_1			92
#define INTNUM_SPI_0			91
#define INTNUM_JPEGW			90
#define INTNUM_JPEGR			89
#define INTNUM_JPEGO			88
#define INTNUM_JBIG_DECODER_JPX			87
#define INTNUM_JBIG_DECODER_JBD			86
#define INTNUM_JBIG_DECODER_OVER			85
#define INTNUM_JBIG_CODEC_JPX			84
#define INTNUM_JBIG_CODEC_JBD			83
#define INTNUM_JBIG_CODEC_OVER			82
#define INTNUM_ZX			81
#define INTNUM_LASER_ENGINE_3			80
#define INTNUM_LASER_ENGINE_2			79
#define INTNUM_LASER_ENGINE_1			78
#define INTNUM_LASER_ENGINE_0			77
#define INTNUM_HVSYNC			76
#define INTNUM_LCD			75
#define INTNUM_I2S			74
#define INTNUM_I2C_3			73
#define INTNUM_I2C_2			72
#define INTNUM_I2C_1			71
#define INTNUM_I2C_0			70
#define INTNUM_AUDIO_DDAC			69
#define INTNUM_ONEWIRE			68
#define INTNUM_IEEE1284_PARALLEL_PORT			67
#define INTNUM_SDMMC			66
#define INTNUM_NAND			65
#define INTNUM_RTC			64
#define INTNUM_UPC_ERROR_1			63
#define INTNUM_UPC_ERROR_0			62
#define INTNUM_CPU_CTI_1			61
#define INTNUM_CPU_CTI_0			60
#define INTNUM_CPU_ERROR			59
#define INTNUM_L2_ERROR			58
#define INTNUM_MEMORY_CONTROLLER			57
#define INTNUM_EXTERNAL_INTERRUPT			56
#define INTNUM_TIMER_3			55
#define INTNUM_TIMER_2			54
#define INTNUM_TIMER_1			53
#define INTNUM_TIMER_0			52
#define INTNUM_TIMEBASE_100MS			51
#define INTNUM_TIMEBASE_10MS			50
#define INTNUM_TIMEBASE_1MS			49
#define INTNUM_TIMEBASE_100US			48
#define INTNUM_TIMEBASE_10US			47
#define INTNUM_TIMEBASE_1US			46
#define INTNUM_GPIOH			45
#define INTNUM_GPIOG			44
#define INTNUM_GPIOF			43
#define INTNUM_GPIOE			42
#define INTNUM_GPIOD			41
#define INTNUM_GPIOC			40
#define INTNUM_GPIOB			39
#define INTNUM_GPIOA			38
#define INTNUM_SCCP1			37
#define INTNUM_SCCP0			36
#define INTNUM_UART3			35
#define INTNUM_UART2			34
#define INTNUM_UART1			33
#define INTNUM_UART0			32
#define INTNUM_GALVO			217
#define INTNUM_SENSOR			216
#define INTNUM_DPWM			215
#define INTNUM_ADC			214
#define INTNUM_FUSER			213
#define INTNUM_CREG			212
#define INTNUM_LSCAN_2			209
#define INTNUM_LSCAN_1			208
#define INTNUM_TIMER_11			199
#define INTNUM_TIMER_10			198
#define INTNUM_TIMER_9			197
#define INTNUM_TIMER_8			196
#define INTNUM_TIMER_7			195
#define INTNUM_TIMER_6			194
#define INTNUM_TIMER_5			193
#define INTNUM_TIMER_4			192

/*
	This section specifies if the above interrupts are level or
	edge triggered. This is a bit mask with interrupt id's as the index
	a 1 means edge triggered, a 0 means level triggered
*/
#define INT_TYPE4 0x0
#define INT_TYPE3 0x0
#define INT_TYPE2 0x0
#define INT_TYPE1 0xfc000
#define INT_TYPE6 0x0
#define INT_TYPE0 0
#define INT_TYPE5 0
#define INT_TYPE7 0
#define INT_TYPE8 0
#define INT_TYPE9 0
#define INT_TYPE10 0
#define INT_TYPE11 0
#define INT_TYPE12 0
#define INT_TYPE13 0
#define INT_TYPE14 0
#define INT_TYPE15 0
#define INT_TYPE16 0
#define INT_TYPE17 0
#define INT_TYPE18 0
#define INT_TYPE19 0
#define INT_TYPE20 0
#define INT_TYPE21 0
#define INT_TYPE22 0
#define INT_TYPE23 0
#define INT_TYPE24 0
#define INT_TYPE25 0
#define INT_TYPE26 0
#define INT_TYPE27 0
#define INT_TYPE28 0
#define INT_TYPE29 0
#define INT_TYPE30 0
#define INT_TYPE31 0

#ifdef __cplusplus
}
#endif
#endif
