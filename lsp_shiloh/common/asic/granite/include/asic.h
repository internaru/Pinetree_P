/*
 *
 * ============================================================================
 * Copyright (c) 2011,2013  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

 /**
  * \file asic.h 
  *  
  * \brief Master Granite ASIC include file. 
  *  
  */

#ifndef ASIC_H
#define ASIC_H

#ifndef ASIC_granite
#error "WRONG ASIC"
#endif

#if ASIC_REV == ASIC_REV_C0

#include "ADCNorm_regmasks.h"
#include "ADCNorm_regstructs.h"
#include "antic_top_regmasks.h"
#include "antic_top_regstructs.h"
#include "adc_regmasks.h"
#include "adc_regstructs.h"
#include "apb_config_regmasks.h"
#include "apb_config_regstructs.h"
#include "apb_ddac_regmasks.h"
#include "apb_ddac_regstructs.h"
#include "APB2_cfg_regmasks.h"
#include "APB2_cfg_regstructs.h"
#include "asela_regmasks.h"
#include "asela_regstructs.h"
#include "AUTO_HT_regmasks.h"
#include "AUTO_HT_regstructs.h"
#include "BDEnhance_regmasks.h"
#include "BDEnhance_regstructs.h"
#include "BitReduct_regmasks.h"
#include "BitReduct_regstructs.h"
#include "BLOOM_regmasks.h"
#include "BLOOM_regstructs.h"
#include "BMLR_regmasks.h"
#include "BMLR_regstructs.h"
#include "BOOTSPI_regmasks.h"
#include "BOOTSPI_regstructs.h"
#include "BUS_regmasks.h"
#include "BUS_regstructs.h"
#include "CBI2CDMA_regmasks.h"
#include "CBI2CDMA_regstructs.h"
#include "CDMA_regmasks.h"
#include "CDMA_regstructs.h"
#include "CDMA_Top_Int_regmasks.h"
#include "CDMA_Top_Int_regstructs.h"
#include "CDMA_Top_regmasks.h"
#include "CDMA_Top_regstructs.h"
#include "CHG_OWI_regmasks.h"
#include "CHG_OWI_regstructs.h"
#include "CISX_IDMA_AXI_regmasks.h"
#include "CISX_IDMA_AXI_regstructs.h"
#include "CISX_int_regmasks.h"
#include "CISX_int_regstructs.h"
#include "CISX_ODMA_AXI_regmasks.h"
#include "CISX_ODMA_AXI_regstructs.h"
#include "COLOR_SHIFT_regmasks.h"
#include "COLOR_SHIFT_regstructs.h"
#include "CREG_regmasks.h"
#include "CREG_regstructs.h"
#include "CRE_regmasks.h"
#include "CRE_regstructs.h"
#include "CR_regmasks.h"
#include "CR_regstructs.h"
#include "CSC_DAVINCI_regmasks.h"
#include "CSC_DAVINCI_regstructs.h"
#include "CSC_ES2Y_regmasks.h"
#include "CSC_ES2Y_regstructs.h"
#include "CSC_RGB2ES_3L_regmasks.h"
#include "CSC_RGB2ES_3L_regstructs.h"
#include "CSC_Y2ES_regmasks.h"
#include "CSC_Y2ES_regstructs.h"
#include "CSSCG_regmasks.h"
#include "CSSCG_regstructs.h"
#include "CStats_regmasks.h"
#include "CStats_regstructs.h"
#include "DavBuff_regmasks.h"
#include "DavBuff_regstructs.h"
#include "DCMotor_BASE_regmasks.h"
#include "DCMotor_BASE_regstructs.h"
#include "DCMotor_S2_regmasks.h"
#include "DCMotor_S2_regstructs.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"
#include "DNAND_regmasks.h"
#include "DNAND_regstructs.h"
#include "DPWM_regmasks.h"
#include "DPWM_regstructs.h"
#include "dro_regmasks.h"
#include "dro_regstructs.h"
#include "efuse_regmasks.h"
#include "efuse_regstructs.h"
#include "engctl_regmasks.h"
#include "engctl_regstructs.h"
#include "EXP_regmasks.h"
#include "EXP_regstructs.h"
#include "FHS_regmasks.h"
#include "FHS_regstructs.h"
#include "fuser_regmasks.h"
#include "fuser_regstructs.h"
#include "GALVO_regmasks.h"
#include "GALVO_regstructs.h"
#include "gic_regstructs.h"
#include "GPIO_regmasks.h"
#include "GPIO_regstructs.h"
#include "HFIR_regmasks.h"
#include "HFIR_regstructs.h"
#include "HiPS_regmasks.h"
#include "HiPS_regstructs.h"
#include "HiPWM_regmasks.h"
#include "HiPWM_regstructs.h"
#include "HScale_regmasks.h"
#include "HScale_regstructs.h"
#include "HVSync_regmasks.h"
#include "HVSync_regstructs.h"
#include "I2C_regmasks.h"
#include "I2C_regstructs.h"
#include "I2S_regmasks.h"
#include "I2S_regstructs.h"
#include "ICE_test_regmasks.h"
#include "ICE_test_regstructs.h"
#include "IDMA_2D_regmasks.h"
#include "IDMA_2D_regstructs.h"
#include "idma_regmasks.h"
#include "idma_regstructs.h"
#include "ieee1284c_regmasks.h"
#include "ieee1284c_regstructs.h"
#include "intnums.h"
#include "istone_regmasks.h"
#include "istone_regstructs.h"
#include "JBIG_regmasks.h"
#include "JBIG_regstructs.h"
#include "JEDI_regmasks.h"
#include "JEDI_regstructs.h"
#include "KMOD_regmasks.h"
#include "KMOD_regstructs.h"
#include "l2_cache_regstructs.h"
#include "LAN_regmasks.h"
#include "LAN_regstructs.h"
#include "LCD2_regmasks.h"
#include "LCD2_regstructs.h"
#include "LCM_regmasks.h"
#include "LCM_regstructs.h"
#include "LPC_regmasks.h"
#include "LPC_regstructs.h"
#include "LPT_regmasks.h"
#include "LPT_regstructs.h"
#include "LSCAN_regmasks.h"
#include "LSCAN_regstructs.h"
#include "m2m_top_regmasks.h"
#include "m2m_top_regstructs.h"
#include "MC_regmasks.h"
#include "MC_regstructs.h"
#include "MFilter_regmasks.h"
#include "MFilter_regstructs.h"
#include "MIP_regmasks.h"
#include "MIP_regstructs.h"
#include "NGAdjust_PIE_regmasks.h"
#include "NGAdjust_PIE_regstructs.h"

#include "ODMA_AXI_regmasks.h"
#include "ODMA_AXI_regstructs.h"

#include "PCIE_regmasks.h"
#include "PCIE_regstructs.h"
#include "PIC_IntCtrl_regmasks.h"
#include "PIC_IntCtrl_regstructs.h"
#include "PIC_WDMA_6_regmasks.h"
#include "PIC_WDMA_6_regstructs.h"
#include "PIE_ICE_Common_regmasks.h"
#include "PIE_ICE_Common_regstructs.h"
#include "PIE_RDMA_ICE_regmasks.h"
#include "PIE_RDMA_ICE_regstructs.h"
#include "PIE_WDMA_ICE_regmasks.h"
#include "PIE_WDMA_ICE_regstructs.h"
#include "PIP_regmasks.h"
#include "PIP_regstructs.h"
#include "PIPE_ACCESS_regmasks.h"
#include "PIPE_ACCESS_regstructs.h"
#include "PIP_STATS_regmasks.h"
#include "PIP_STATS_regstructs.h"
#include "PLNR_regmasks.h"
#include "PLNR_regstructs.h"
#include "PNU2D_regmasks.h"
#include "PNU2D_regstructs.h"
#include "PRNUDSNU_CORR_regmasks.h"
#include "PRNUDSNU_CORR_regstructs.h"
#include "regAddrs.h"
#include "RTC_SZ_regmasks.h"
#include "RTC_SZ_regstructs.h"
#include "SCAN_regmasks.h"
#include "SCAN_regstructs.h"
#include "SCCP_regmasks.h"
#include "SCCP_regstructs.h"
#include "ScCSC_regmasks.h"
#include "ScCSC_regstructs.h"
#include "SDMMC2_regmasks.h"
#include "SDMMC2_regstructs.h"
#include "SENSOR_regmasks.h"
#include "SENSOR_regstructs.h"
#include "SERP_regmasks.h"
#include "SERP_regstructs.h"
#include "SIC_regmasks.h"
#include "SIC_regstructs.h"
#include "SILABS_regmasks.h"
#include "SILABS_regstructs.h"
#include "SIZAR_regmasks.h"
#include "SIZAR_regstructs.h"
#include "SPI_MASTER_regmasks.h"
#include "SPI_MASTER_regstructs.h"
#include "STMotor_regmasks.h"
#include "STMotor_regstructs.h"
#include "SVDO_regmasks.h"
#include "SVDO_regstructs.h"
#include "SWITCHER_regmasks.h"
#include "SWITCHER_regstructs.h"
#include "TCNSense_regmasks.h"
#include "TCNSense_regstructs.h"
#include "TH_regmasks.h"
#include "TH_regstructs.h"
#include "TIMEBASE_regmasks.h"
#include "TIMEBASE_regstructs.h"
#include "TIMERS_regmasks.h"
#include "TIMERS_regstructs.h"
#include "TP_CONTONE_regmasks.h"
#include "TP_CONTONE_regstructs.h"
#include "TP_TEXT_regmasks.h"
#include "TP_TEXT_regstructs.h"
#include "tsen_regmasks.h"
#include "tsen_regstructs.h"
#include "UART_regmasks.h"
#include "UART_regstructs.h"
#include "UED_advanced_regmasks.h"
#include "UED_advanced_regstructs.h"
#include "USB2TOP_regmasks.h"
#include "USB2TOP_regstructs.h"
#include "WaveGenLUT_regmasks.h"
#include "WaveGenLUT_regstructs.h"
#include "Xena_regmasks.h"
#include "Xena_regstructs.h"
#include "XYCScale_regmasks.h"
#include "XYCScale_regstructs.h"
#include "XYScale_regmasks.h"
#include "XYScale_regstructs.h"
#include "YIPI_regmasks.h"
#include "YIPI_regstructs.h"
#include "Zx_regmasks.h"
#include "Zx_regstructs.h"

#else
#error "Unsupported ASIC revision "
#endif // ASIC_REV

// ASIC register compatibility defines to allow certain drivers to run on multiple asics
//#if ASIC_REV < ASIC_REV_xx
//#include "xxx_compatibility.h"
//#endif
//#include "xxx_compatibility.h"

// I2C compatibility for later version (IC_COMP_VERSION = 0x3131352A)
#define I2C_IC_DATA_CMD_RESTART_SHIFT 10
#define I2C_IC_DATA_CMD_STOP_SHIFT 9
#define I2C_IC_CON_IC_RESTART_EN_MASK 0x20

#define PA_IDMA_REGS_t IDMA_REGS_t
#define PA_IDMA_INT_EN_RST_INT_EN_REPLACE_VAL IDMA_INT_EN_RST_INT_EN_REPLACE_VAL
#define PA_IDMA_INT_EN_EOI_INT_EN_REPLACE_VAL IDMA_INT_EN_EOI_INT_EN_REPLACE_VAL
#define PA_IDMA_INT_EN_FIN_INT_EN_REPLACE_VAL IDMA_INT_EN_FIN_INT_EN_REPLACE_VAL
#define PA_IDMA_INT_EN_WHO_INT_EN_REPLACE_VAL IDMA_INT_EN_WHO_INT_EN_REPLACE_VAL
#define PA_IDMA_INT_EN_RST_INT_EN_MASK IDMA_INT_EN_RST_INT_EN_MASK
#define PA_IDMA_INT_EN_EOI_INT_EN_MASK IDMA_INT_EN_EOI_INT_EN_MASK
#define PA_IDMA_INT_EN_FIN_INT_EN_MASK IDMA_INT_EN_FIN_INT_EN_MASK
#define PA_IDMA_INT_EN_WHO_INT_EN_MASK IDMA_INT_EN_WHO_INT_EN_MASK
#define ODMA_SLI_REGS_t ODMA_REGS_t
#define PA_ODMA_REGS_t ODMA_SLI_REGS_t
#define PA_ODMA_INT_EN_FIN_INT_EN_MASK ODMA_SLI_INT_EN_FIN_INT_EN_MASK
#define DEC_CONFIG_REGS_t DEC_CONFIG_REGS_REGS_t
#define HIPWM0_REGS_t HIPWM_REGS_t
#define HIPWM0_HCR_CFGDECFORCELASERONEN_REPLACE_VAL HIPWM_HCR_CFGDECFORCELASERONEN_REPLACE_VAL
#define HIPWM0_HCR_CFGDECVIDEODATAEN_REPLACE_VAL HIPWM_HCR_CFGDECVIDEODATAEN_REPLACE_VAL
#define HIPWM0_HCR_ENABLE_REPLACE_VAL HIPWM_HCR_ENABLE_REPLACE_VAL

#define WAVEGEN_REGS_t WAVEGEN_LUT_REGS_t
#define WAVEGEN_WCR_HSYNCSEL_REPLACE_VAL WAVEGEN_LUT_WCR_HSYNCSEL_REPLACE_VAL
#define WAVEGEN_WCR_ENABLE_REPLACE_VAL WAVEGEN_LUT_WCR_ENABLE_REPLACE_VAL
#define WAVEGEN_WTR_TIMESTEP_REPLACE_VAL WAVEGEN_LUT_WTR_TIMESTEP_REPLACE_VAL
#define WAVEGEN_WSLR_SEGLENGTH_REPLACE_VAL WAVEGEN_LUT_WSLR_SEGLENGTH_REPLACE_VAL
#define WAVEGEN_WGLUT0_DUTYCYCLE_REPLACE_VAL WAVEGEN_LUT_WGLUT0_DUTYCYCLE_REPLACE_VAL

// Video block configuration values.
#define NUM_VIDEO_CHAN   8
#define NUM_HSYNC_INPUTS 4
#define NUM_PCODE_REGS 16
#define NUM_JUSTIFY_REGS 16
#define NUM_MIRRORS_MAX 2
#define MAX_CHANNELS_PER_COLOR 2

// Monochrome Image Pipe 

// REVISIT #define MIP_LUT_SIZE 5120
#define MIP_LUT_SIZE 1536 // This is the default size - can be modified by 
                          // setting the RAM MUXing mode in the PIP 
                          // configuration register.
#define MIP_LUT_WORDS_PER_ENTRY 2 // 64 bits for each LUT entry  ??? -jrs VERIFY; asic docs say 84 bits
#define MIP_LUT_SIZE_IN_32_BIT_WORDS (MIP_LUT_SIZE*MIP_LUT_WORDS_PER_ENTRY)

#define MIP_BASE_ADDR PIP_MIP_BASE

// Raster ODMA compatibility
#define RASTER_ODMA_BASE PIP_ODMA0_BASE

// CR register name compatibility
#define CR_REP0_REP0CH0_REPLACE_VAL CR_REP0_CH0_REPLACE_VAL
#define CR_REP0_REP0CH1_REPLACE_VAL CR_REP0_CH1_REPLACE_VAL
#define CR_REP0_REP0CH2_REPLACE_VAL CR_REP0_CH2_REPLACE_VAL
#define CR_REP1_REP1CH0_REPLACE_VAL CR_REP1_CH0_REPLACE_VAL
#define CR_REP1_REP1CH1_REPLACE_VAL CR_REP1_CH1_REPLACE_VAL
#define CR_REP1_REP1CH2_REPLACE_VAL CR_REP1_CH2_REPLACE_VAL
#define CR_UPPER0_UP0CH0_REPLACE_VAL CR_UPPER0_CH0_REPLACE_VAL
#define CR_UPPER0_UP0CH1_REPLACE_VAL CR_UPPER0_CH1_REPLACE_VAL
#define CR_UPPER0_UP0CH2_REPLACE_VAL CR_UPPER0_CH2_REPLACE_VAL
#define CR_UPPER1_UP1CH0_REPLACE_VAL CR_UPPER1_CH0_REPLACE_VAL
#define CR_UPPER1_UP1CH1_REPLACE_VAL CR_UPPER1_CH1_REPLACE_VAL
#define CR_UPPER1_UP1CH2_REPLACE_VAL CR_UPPER1_CH2_REPLACE_VAL
#define CR_UPPER2_UP2CH0_REPLACE_VAL CR_UPPER2_CH0_REPLACE_VAL
#define CR_UPPER2_UP2CH1_REPLACE_VAL CR_UPPER2_CH1_REPLACE_VAL
#define CR_UPPER2_UP2CH2_REPLACE_VAL CR_UPPER2_CH2_REPLACE_VAL

#define CR_LOWER0_LO0CH0_REPLACE_VAL CR_LOWER0_CH0_REPLACE_VAL
#define CR_LOWER0_LO0CH1_REPLACE_VAL CR_LOWER0_CH1_REPLACE_VAL
#define CR_LOWER0_LO0CH2_REPLACE_VAL CR_LOWER0_CH2_REPLACE_VAL
#define CR_LOWER1_LO1CH0_REPLACE_VAL CR_LOWER1_CH0_REPLACE_VAL
#define CR_LOWER1_LO1CH1_REPLACE_VAL CR_LOWER1_CH1_REPLACE_VAL
#define CR_LOWER1_LO1CH2_REPLACE_VAL CR_LOWER1_CH2_REPLACE_VAL
#define CR_LOWER2_LO2CH0_REPLACE_VAL CR_LOWER2_CH0_REPLACE_VAL
#define CR_LOWER2_LO2CH1_REPLACE_VAL CR_LOWER2_CH1_REPLACE_VAL
#define CR_LOWER2_LO2CH2_REPLACE_VAL CR_LOWER2_CH2_REPLACE_VAL

//ODMA regmask compatibility
#define ODMA_SLI_CFG_ENABLE_REPLACE_VAL  ODMA_CFG_ENABLE_REPLACE_VAL 
#define ODMA_SLI_STATUS_DMA_BUSY_MASK_SHIFT ODMA_STATUS_DMA_BUSY_MASK_SHIFT
#define ODMA_SLI_STATUS_DMA_BUSY_MASK ODMA_STATUS_DMA_BUSY_MASK
#define ODMA_SLI_STATUS_SOFTRESET_MASK ODMA_STATUS_SOFTRESET_MASK
#define ODMA_SLI_INT_ACK_RST_INT_ACK_REPLACE_VAL ODMA_INT_ACK_RST_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_EOI_INT_ACK_REPLACE_VAL ODMA_INT_ACK_EOI_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_DIR_INT_ACK_REPLACE_VAL ODMA_INT_ACK_DIR_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_CL_ALI_INT_ACK_REPLACE_VAL ODMA_INT_ACK_CL_ALI_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_EOL_ALI_INT_ACK_REPLACE_VAL ODMA_INT_ACK_EOL_ALI_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_EOI_ALI_INT_ACK_REPLACE_VAL ODMA_INT_ACK_EOI_ALI_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_EOI_INT_ACK_REPLACE_VAL ODMA_INT_ACK_EOI_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_FIN_INT_ACK_REPLACE_VAL ODMA_INT_ACK_FIN_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_ACK_WHO_INT_ACK_REPLACE_VAL ODMA_INT_ACK_WHO_INT_ACK_REPLACE_VAL
#define ODMA_SLI_INT_PEND_WHO_INT_PEND_MASK ODMA_INT_PEND_WHO_INT_PEND_MASK
#define ODMA_SLI_INT_PEND_RST_INT_PEND_MASK ODMA_INT_PEND_RST_INT_PEND_MASK
#define ODMA_SLI_INT_PEND_FIN_INT_PEND_MASK ODMA_INT_PEND_FIN_INT_PEND_MASK
#define ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK ODMA_INT_PEND_EOI_ERR_INT_PEND_MASK
#define ODMA_SLI_INT_PEND_EOI_INT_PEND_MASK ODMA_INT_PEND_EOI_INT_PEND_MASK
#define ODMA_SLI_CFG_ENABLE_REPLACE_VAL ODMA_CFG_ENABLE_REPLACE_VAL
#define ODMA_SLI_CFG_IN_WIDTH_REPLACE_VAL ODMA_CFG_IN_WIDTH_REPLACE_VAL   
#define ODMA_SLI_CFG_BURST_LEN_REPLACE_VAL ODMA_CFG_BURST_LEN_REPLACE_VAL
#define ODMA_SLI_STATUS_EMPTY_CBUF_MASK ODMA_STATUS_EMPTY_CBUF_MASK
#define ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL ODMA_RESET_SOFT_RESET_REPLACE_VAL
#define ODMA_SLI_CFG_UPPER_HALF_REPLACE_VAL ODMA_CFG_UPPER_HALF_REPLACE_VAL

// PIP regmask compatibility
#define PIP_CFG_DMA0_EN_REPLACE_VAL PIP_DMA_CFG_DMA0_MODE_REPLACE_VAL
#define PIP_CFG_DMA1_EN_REPLACE_VAL PIP_DMA_CFG_DMA1_MODE_REPLACE_VAL
#define PIP_CFG_DMA2_EN_REPLACE_VAL PIP_DMA_CFG_DMA2_MODE_REPLACE_VAL
#define PIP_CFG_DMA3_EN_REPLACE_VAL PIP_DMA_CFG_DMA3_MODE_REPLACE_VAL
#define PIP_CFG_DMA4_EN_REPLACE_VAL PIP_DMA_CFG_DMA4_MODE_REPLACE_VAL
#define PIP_CFG_DMA5_EN_REPLACE_VAL PIP_DMA_CFG_DMA5_MODE_REPLACE_VAL
#define PIP_CFG_DMA6_EN_REPLACE_VAL PIP_DMA_CFG_DMA6_MODE_REPLACE_VAL
#define PIP_CFG_DMA7_EN_REPLACE_VAL PIP_DMA_CFG_DMA7_MODE_REPLACE_VAL

#include "jbig_compatibility.h"


#define CRE_RASTER_REGS_t CRE_REGS_t
///#define CRE CRE

/// concatination macro with 6 arguments
#define CAT_6(a,b,c,d,e,f) a ## b ## c ## d ## e ## f
/// REPLACE_VAL( ever_changing_structure_hierarchy_name, 
///              field_name,
///              bit(s)_name,
///              register,
///              new_value 
///              )
/// this uses the asic supplied *_masks and *_structs files but allows the structure name to be
/// easily replaced, this is needed since the structure location/name seems to change with each asic.
///
#define REPLACE_VAL( STRUCTN, FIELDN, BITN, reg, val ) \
  CAT_6( STRUCTN, _, FIELDN, _, BITN, _REPLACE_VAL )((reg), (val))

/// same as REPLACE_VAL but just reads the bits instead of replacing them.
#define MASK_SHIFT( STRUCTN, FIELDN, BITN, reg ) \
  CAT_6( STRUCTN, _, FIELDN, _, BITN, _MASK_SHIFT )((reg))

#define MASK( STRUCTN, FIELDN, BITN ) \
  CAT_6( STRUCTN, _, FIELDN, _, BITN, _MASK )

//#define 
//#define 
#endif // ASIC_H


