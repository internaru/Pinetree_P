/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the Pipeline Access block.
  * 
 **/

#include "string.h"
#include "lassert.h"
//#include "cdma_api.h"
//#include "pip.h"
#include "pipe_access_api.h"
#include "logger.h"

#define DBG_PRFX "PIPE_ACCESS: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

#ifndef CDMA_PIPE_ACCESS
#define CDMA_PIPE_ACCESS CDMA_STATS_PIPE_ACCESS_RX
#endif

//********************************
// pipe_access_cfg_driver - setup the majority of the block
//********************************
void pipe_access_cfg_driver(PIPE_ACCESS_REGS_t *pa_reg, pipe_access_cfg_t *pa_cfg)
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE( "Called pipe_access_cfg_driver() \n\r");

    // Configuration Register
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_SEL_REPLACE_VAL(reg_val, 0);
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_EN_REPLACE_VAL(reg_val, 0);
    reg_val     = PIPE_ACCESS_CFG_LUT_CBI_REPLACE_VAL(reg_val,pa_cfg->lut_cbi);
    reg_val     = PIPE_ACCESS_CFG_PIPELINE_LOAD_EN_REPLACE_VAL(reg_val,pa_cfg->pipeline_load_en);
    reg_val     = PIPE_ACCESS_CFG_PIPELINE_STORE_EN_REPLACE_VAL(reg_val,pa_cfg->pipeline_store_en);
    reg_val     = PIPE_ACCESS_CFG_BYPASS_REPLACE_VAL(reg_val,pa_cfg->bypass);
    DBG_PRINTF_INFO(" pipe_access cfg %x\n", reg_val);
    pa_reg->cfg = reg_val;

    // Packing Configuration Register
    reg_val          = pa_reg->pack_cfg;
    reg_val          = PIPE_ACCESS_PACK_CFG_PACK_8_CHAN_SEL_REPLACE_VAL(reg_val, pa_cfg->pack_8_chan_sel);
    reg_val          = PIPE_ACCESS_PACK_CFG_PACK_32_CHAN_SEL_REPLACE_VAL(reg_val, pa_cfg->pack_32_chan_sel);
    reg_val          = PIPE_ACCESS_PACK_CFG_PACK_SIDEBAND_MSB_REPLACE_VAL(reg_val, pa_cfg->pack_sideband_msb);
    reg_val          = PIPE_ACCESS_PACK_CFG_PACK_SHIFT_2_REPLACE_VAL(reg_val, pa_cfg->pack_shift_2);
    reg_val          = PIPE_ACCESS_PACK_CFG_PACK_MODE_REPLACE_VAL(reg_val, pa_cfg->pack_mode);
    pa_reg->pack_cfg = reg_val;

    // Unpacking Configuration Register
    reg_val            = pa_reg->unpack_cfg;
    reg_val            = PIPE_ACCESS_UNPACK_CFG_SIDEBAND_FW_VAL_REPLACE_VAL(reg_val, pa_cfg->unpack_sideband_fw_val);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_8_CHAN_SEL_REPLACE_VAL(reg_val, pa_cfg->unpack_8_chan_sel);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_32_CHAN_SEL_REPLACE_VAL(reg_val, pa_cfg->unpack_32_chan_sel);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_USE_SIDEBAND_FW_REPLACE_VAL(reg_val, pa_cfg->unpack_use_sideband_fw);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_SIDEBAND_MSB_REPLACE_VAL(reg_val, pa_cfg->unpack_sideband_msb);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_SHIFT_2_REPLACE_VAL(reg_val, pa_cfg->unpack_shift_2);
    reg_val            = PIPE_ACCESS_UNPACK_CFG_UNPACK_MODE_REPLACE_VAL(reg_val, pa_cfg->unpack_mode);
    pa_reg->unpack_cfg = reg_val;

    // LUT Input Configuration Register
    reg_val               = pa_reg->lut_input_sel;
    reg_val               = PIPE_ACCESS_LUT_INPUT_SEL_LUT_5_INPUT_SEL_REPLACE_VAL(reg_val, pa_cfg->lut_5_input_sel);
    reg_val               = PIPE_ACCESS_LUT_INPUT_SEL_LUT_4_INPUT_SEL_REPLACE_VAL(reg_val, pa_cfg->lut_4_input_sel);
    reg_val               = PIPE_ACCESS_LUT_INPUT_SEL_LUT_3_INPUT_SEL_REPLACE_VAL(reg_val, pa_cfg->lut_3_input_sel);
    pa_reg->lut_input_sel = reg_val;

    // Per-Pixel Enable Configuration Register
    reg_val              = pa_reg->per_pixel_en;
    reg_val              = PIPE_ACCESS_PER_PIXEL_EN_PER_PIXEL_ENABLE_REPLACE_VAL(reg_val, pa_cfg->per_pixel_enable);
    reg_val              = PIPE_ACCESS_PER_PIXEL_EN_PER_PIXEL_ENABLE_REPLACE_VAL(reg_val, pa_cfg->per_pixel_polarity);
    reg_val              = PIPE_ACCESS_PER_PIXEL_EN_PER_PIXEL_ENABLE_REPLACE_VAL(reg_val, pa_cfg->per_pixel_sel);
    pa_reg->per_pixel_en = reg_val;

    // Stats 0 Configuration Register
    reg_val            = pa_reg->stats_cfg0;
    reg_val            = PIPE_ACCESS_STATS_CFG0_IMAGE_WIDTH_REPLACE_VAL(reg_val, pa_cfg->stats_image_width);
    reg_val            = PIPE_ACCESS_STATS_CFG0_PATCH_HEIGHT_REPLACE_VAL(reg_val, pa_cfg->stats_patch_height);
    reg_val            = PIPE_ACCESS_STATS_CFG0_STATS_EN_REPLACE_VAL(reg_val, pa_cfg->stats_en);
    pa_reg->stats_cfg0 = reg_val;

    // Stats 1 Configuration Register
    reg_val            = pa_reg->stats_cfg1;
    reg_val            = PIPE_ACCESS_STATS_CFG1_THRESH0_REPLACE_VAL(reg_val, pa_cfg->stats_thresh0);
    reg_val            = PIPE_ACCESS_STATS_CFG1_THRESH1_REPLACE_VAL(reg_val, pa_cfg->stats_thresh1);
    pa_reg->stats_cfg1 = reg_val;

    // Stats 2 Configuration Register
    reg_val            = pa_reg->stats_cfg3;
    reg_val            = PIPE_ACCESS_STATS_CFG2_THRESH2_REPLACE_VAL(reg_val, pa_cfg->stats_thresh2);
    reg_val            = PIPE_ACCESS_STATS_CFG2_THRESH3_REPLACE_VAL(reg_val, pa_cfg->stats_thresh3);
    pa_reg->stats_cfg2 = reg_val;

    // Stats 3 Configuration Register
    reg_val            = pa_reg->stats_cfg3;
    reg_val            = PIPE_ACCESS_STATS_CFG3_THRESH4_REPLACE_VAL(reg_val, pa_cfg->stats_thresh4);
    reg_val            = PIPE_ACCESS_STATS_CFG3_THRESH5_REPLACE_VAL(reg_val, pa_cfg->stats_thresh5);
    pa_reg->stats_cfg3 = reg_val;
}

//********************************
// pipe_access_setup_luts - program the LUTs
//********************************
void pipe_access_setup_luts(PIPE_ACCESS_REGS_t *pa_reg,
                            uint32_t* lut0_data,
                            uint32_t* lut1_data,
                            uint32_t* lut2_data,
                            uint32_t* lut3_data,
                            uint32_t* lut4_data,
                            uint32_t* lut5_data)
{
    // load 'em
    pipe_access_lut_load(pa_reg, 0, lut0_data);
    pipe_access_lut_load(pa_reg, 1, lut1_data);
    pipe_access_lut_load(pa_reg, 2, lut2_data);
    pipe_access_lut_load(pa_reg, 3, lut3_data);
    pipe_access_lut_load(pa_reg, 4, lut4_data);
    pipe_access_lut_load(pa_reg, 5, lut5_data);

    // check 'em
    pipe_access_lut_check(pa_reg, 0, lut0_data);
    pipe_access_lut_check(pa_reg, 1, lut1_data);
    pipe_access_lut_check(pa_reg, 2, lut2_data);
    pipe_access_lut_check(pa_reg, 3, lut3_data);
    pipe_access_lut_check(pa_reg, 4, lut4_data);
    pipe_access_lut_check(pa_reg, 5, lut5_data);
}

//********************************
// pipe_access_bypass - Puts Pipe Access in bypass
//********************************
void pipe_access_bypass(PIPE_ACCESS_REGS_t *pa_reg)
{
    pa_reg->cfg |= PIPE_ACCESS_CFG_BYPASS_MASK;
}

//********************************
// pipe_access_inv_lut
// Load inverse tables.
//********************************
void pipe_access_inv_lut(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t lut_data[PIPE_ACCESS_LUT_DEPTH];
    uint32_t i;

    // create inverse LUT data
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = (~i) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }

    // write data to each of the LUTs
    for(i=0; i<6; i++)
    {
        DBG_PRINTF_NOTICE( "%s : Loading LUT %d \n", __FUNCTION__, i);
        pipe_access_lut_load(pa_reg, i, lut_data);
    }

    // check each of the LUTs
    for(i=0; i<6; i++)
    {
        DBG_PRINTF_NOTICE( "%s : Checking LUT %d \n", __FUNCTION__, i);
        pipe_access_lut_check(pa_reg, i, lut_data);
    }
}

//********************************
// pipe_access_lut_load
// Load the specified LUT with data at data_ptr
//********************************
void pipe_access_lut_load(PIPE_ACCESS_REGS_t *pa_reg, 
                 uint32_t  lut_num, 
                 uint32_t* data_ptr)
{
    uint32_t reg_val;
    uint32_t i;

    // first set the LUT access bits
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_SEL_REPLACE_VAL(reg_val, lut_num);
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_EN_REPLACE_VAL(reg_val, 1);
    pa_reg->cfg = reg_val;

    // blast the data into the LUT
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        pa_reg->cpu_lut[i] = data_ptr[i];
    }

    // disable LUT access
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_SEL_REPLACE_VAL(reg_val, 0);
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_EN_REPLACE_VAL(reg_val, 0);
    pa_reg->cfg = reg_val;
}

//********************************
// pipe_access_lut_check
// Compare the specified LUT with data at data_ptr
//********************************
void pipe_access_lut_check(PIPE_ACCESS_REGS_t *pa_reg, 
                           uint32_t  lut_num, 
                           uint32_t* data_ptr)
{
    uint32_t reg_val;
    uint32_t i;

    // first set the LUT access bits
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_SEL_REPLACE_VAL(reg_val, lut_num);
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_EN_REPLACE_VAL(reg_val, 1);
    pa_reg->cfg = reg_val;

    DBG_PRINTF_NOTICE( "%s : Before LUT read config = 0x%08x\n\r",
        __FUNCTION__, pa_reg->cfg);

    // read and compare the LUT data
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        reg_val = pa_reg->cpu_lut[i];
        if(reg_val != data_ptr[i])
        {
            DBG_PRINTF_NOTICE( "ERROR: %s : Incorrect LUT Data LUT #%d Addr=0x%08x, Act=0x%08x, Exp=0x%08x\n\r",
                __FUNCTION__, lut_num, &pa_reg->cpu_lut[i], reg_val, data_ptr[i]);
        }
    }

    // disable LUT access
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_SEL_REPLACE_VAL(reg_val, 0);
    reg_val     = PIPE_ACCESS_CFG_CPU_LUT_ACCESS_EN_REPLACE_VAL(reg_val, 0);
    pa_reg->cfg = reg_val;
}

//********************************
// pipe_access_store_4
// Configure block to do a pipeline store on
// the 4-channel interface.
//********************************
void pipe_access_store_4(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // set the pipe_store bits
    reg_val = pa_reg->cfg;
    reg_val = PIPE_ACCESS_CFG_PIPELINE_STORE_EN_REPLACE_VAL(reg_val, 1);
    pa_reg->cfg = reg_val;

    // set the pack 32 bits
    reg_val = pa_reg->pack_cfg;
    reg_val = PIPE_ACCESS_PACK_CFG_PACK_MODE_REPLACE_VAL(reg_val,1);
    pa_reg->pack_cfg = reg_val;
}

//********************************
// pipe_access_store_6
// Configure block to do a pipeline store on
// the 6-channel interface.
//********************************
void pipe_access_store_6(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // set the pipe_store bits
    reg_val = pa_reg->cfg;
    reg_val = PIPE_ACCESS_CFG_PIPELINE_STORE_EN_REPLACE_VAL(reg_val, 2);
    pa_reg->cfg = reg_val;

    // set the pack 32 bits
    reg_val = pa_reg->pack_cfg;
    reg_val = PIPE_ACCESS_PACK_CFG_PACK_MODE_REPLACE_VAL(reg_val,0);
    pa_reg->pack_cfg = reg_val;
}

//********************************
// pipe_access_load_4
// Configure block to do a pipeline store on
// the 4-channel interface.
//********************************
void pipe_access_load_4(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // set the pipe_store bits
    reg_val = pa_reg->cfg;
    reg_val = PIPE_ACCESS_CFG_PIPELINE_LOAD_EN_REPLACE_VAL(reg_val, 1);
    pa_reg->cfg = reg_val;

    // set the unpack 32 bits
    reg_val = pa_reg->unpack_cfg;
    reg_val = PIPE_ACCESS_UNPACK_CFG_UNPACK_MODE_REPLACE_VAL(reg_val,1);
    pa_reg->unpack_cfg = reg_val;
}

//********************************
// pipe_access_load_6
// Configure block to do a pipeline store on
// the 6-channel interface.
//********************************
void pipe_access_load_6(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // set the pipe_store bits
    reg_val = pa_reg->cfg;
    reg_val = PIPE_ACCESS_CFG_PIPELINE_LOAD_EN_REPLACE_VAL(reg_val, 2);
    pa_reg->cfg = reg_val;

    // set the unpack 64 bits
    reg_val = pa_reg->unpack_cfg;
    reg_val = PIPE_ACCESS_UNPACK_CFG_UNPACK_MODE_REPLACE_VAL(reg_val,0);
    pa_reg->unpack_cfg = reg_val;
}

//********************************
// pipe_access_lut_en
// Configure block to use 1-D LUTS
// on the output of the 6 channel interface.
//********************************
void pipe_access_lut_en(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // set the interface to 4 channel interface
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_LUT_CBI_REPLACE_VAL(reg_val, 2);
    reg_val     = PIPE_ACCESS_CFG_BYPASS_REPLACE_VAL(reg_val, 0); // make sure not bypass 
    pa_reg->cfg = reg_val;
}

//********************************
// pipe_access_stats_en
// Enable stats and setup the thresholds
// Pipeline store must be setup outside of this function.
//********************************
void pipe_access_stats_en(PIPE_ACCESS_REGS_t *pa_reg,
                          uint32_t image_width,
                          uint32_t stats_patch_height,
                          uint32_t thresh0,
                          uint32_t thresh1,
                          uint32_t thresh2,
                          uint32_t thresh3,
                          uint32_t thresh4,
                          uint32_t thresh5)
{
    uint32_t reg_val;

    // program thresholds
    reg_val            = pa_reg->stats_cfg1;
    reg_val            = PIPE_ACCESS_STATS_CFG1_THRESH0_REPLACE_VAL(reg_val, thresh0);
    reg_val            = PIPE_ACCESS_STATS_CFG1_THRESH1_REPLACE_VAL(reg_val, thresh1);
    pa_reg->stats_cfg1 = reg_val;

    reg_val            = pa_reg->stats_cfg2;
    reg_val            = PIPE_ACCESS_STATS_CFG2_THRESH2_REPLACE_VAL(reg_val, thresh2);
    reg_val            = PIPE_ACCESS_STATS_CFG2_THRESH3_REPLACE_VAL(reg_val, thresh3);
    pa_reg->stats_cfg2 = reg_val;

    reg_val            = pa_reg->stats_cfg3;
    reg_val            = PIPE_ACCESS_STATS_CFG3_THRESH4_REPLACE_VAL(reg_val, thresh4);
    reg_val            = PIPE_ACCESS_STATS_CFG3_THRESH5_REPLACE_VAL(reg_val, thresh5);
    pa_reg->stats_cfg3 = reg_val;

    // stats_cfg0 register
    reg_val            = pa_reg->stats_cfg0;
    reg_val            = PIPE_ACCESS_STATS_CFG0_IMAGE_WIDTH_REPLACE_VAL(reg_val, image_width);
    reg_val            = PIPE_ACCESS_STATS_CFG0_PATCH_HEIGHT_REPLACE_VAL(reg_val, stats_patch_height);
    reg_val            = PIPE_ACCESS_STATS_CFG0_STATS_EN_REPLACE_VAL(reg_val, 1);
    pa_reg->stats_cfg0 = reg_val;
}

//********************************
// pipe_access_lut_disable
// Disable the 1-D LUT in Pipe Access.
//********************************
void pipe_access_lut_disable(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t reg_val;

    // disable the 1-D LUTs in PIPE Access
    reg_val     = pa_reg->cfg;
    reg_val     = PIPE_ACCESS_CFG_LUT_CBI_REPLACE_VAL(reg_val, 0);
    pa_reg->cfg = reg_val;
}

//********************************
// pipe_access_regtest
// Perform a simple regtest
//********************************
uint32_t pipe_access_regtest(PIPE_ACCESS_REGS_t *pa_reg)
{
    uint32_t i;
    uint32_t reg_val;
    uint32_t fail = 0;

    uint32_t lut_data[PIPE_ACCESS_LUT_DEPTH];

    // test the stats_cfg0 register
    reg_val = pa_reg->stats_cfg0;
    reg_val = PIPE_ACCESS_STATS_CFG0_IMAGE_WIDTH_REPLACE_VAL(reg_val, 0x1234);
    pa_reg->stats_cfg0 = reg_val;

    if(pa_reg->stats_cfg0 != 0x12340000)
    {
        DBG_PRINTF_NOTICE( "ERROR: %s : Incorrect stats_cfg0 Addr=0x%08x, Act=0x%08x, Exp=0x%08x\n\r",
            __FUNCTION__, &pa_reg->stats_cfg0, reg_val, 0x12340000);
        fail = 1;
    }
    else
    {
        DBG_PRINTF_NOTICE( "I can read my registers: %s : stats_cfg0 Addr=0x%08x, Act=0x%08x, Exp=0x%08x\n\r",
            __FUNCTION__, &pa_reg->stats_cfg0, pa_reg->stats_cfg0, 0x12340000);
    }
 
    // do a simple LUT test
    // load the luts
    DBG_PRINTF_NOTICE( "%s : Loading LUT 0 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(0)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 0, lut_data);

    DBG_PRINTF_NOTICE( "%s : Loading LUT 1 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(1)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 1, lut_data);

    DBG_PRINTF_NOTICE( "%s : Loading LUT 2 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(2)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 2, lut_data);

    DBG_PRINTF_NOTICE( "%s : Loading LUT 3 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(3)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 3, lut_data);

    DBG_PRINTF_NOTICE( "%s : Loading LUT 4 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(4)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 4, lut_data);

    DBG_PRINTF_NOTICE( "%s : Loading LUT 5 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(5)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_load(pa_reg, 5, lut_data);

    // check the LUT data
    DBG_PRINTF_NOTICE( "%s : Checking LUT 0 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(0)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 0, lut_data);

    DBG_PRINTF_NOTICE( "%s : Checking LUT 1 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(1)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 1, lut_data);

    DBG_PRINTF_NOTICE( "%s : Checking LUT 2 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(2)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 2, lut_data);

    DBG_PRINTF_NOTICE( "%s : Checking LUT 3 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(3)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 3, lut_data);

    DBG_PRINTF_NOTICE( "%s : Checking LUT 4 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(4)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 4, lut_data);

    DBG_PRINTF_NOTICE( "%s : Checking LUT 5 \n\r", __FUNCTION__);
    for(i=0; i<PIPE_ACCESS_LUT_DEPTH; i++)
    {
        lut_data[i] = ((i<<4)|(5)) & PIPE_ACCESS_CPU_LUT_DATA_MASK;
    }
    pipe_access_lut_check(pa_reg, 5, lut_data);

    return(fail);
}

void pa_odma_setup(PA_ODMA_REGS_t *odma_reg, uint32_t desc_addr, uint32_t data_addr, uint32_t xferCnt)
{
    // Setup Descriptor
    *(volatile uint32_t*)desc_addr = 0x000100C3;
    *(volatile uint32_t*)(desc_addr+4) = data_addr;
    *(volatile uint32_t*)(desc_addr+8) = xferCnt;
    *(volatile uint32_t*)(desc_addr+12) = 0x0;

    odma_reg->int_en = 0xFFFFFFFF;
    odma_reg->cfg = 0x00010500;
}

#ifdef lsptodo_cdma
cdma_handle_t *pa_cdma_handle = NULL;

void pa_stat_cdma_callback(void *user_data, uint32_t int_status)
{
    pip_notify_pa_stat_cdma_done((uint32_t)user_data);
}

error_type_t pa_stat_cfg_buffer(uint32_t *buf, uint32_t bufsize, uint32_t *srcaddr, uint32_t perfid)
{
    // Allocate CDMA channel to output PA stat data to specified buffer
        
    uint32_t tempres;
    cdma_handle_t *handle = NULL;
    CDMA_CONFIG MyCfg;
    uint8_t *destbuf, *srcbuf;
    int result = 0;

    XASSERT(CDMA_PIPE_ACCESS == perfid, perfid);

    MyCfg.SourceEndian = CDMA_LITTLE_ENDIAN;
    MyCfg.DestEndian = CDMA_LITTLE_ENDIAN;
    MyCfg.SrcBurst = CDMA_BURST_1;
    MyCfg.DestBurst = CDMA_BURST_8;
    MyCfg.SrcXferWidth = CDMA_32_BITS;
    MyCfg.DestXferWidth = CDMA_32_BITS;
    MyCfg.FlowControl = CDMA_DEV_TO_MEM;
    MyCfg.SrcIncrement = CDMA_NO_INC;
    MyCfg.DestIncrement = CDMA_INC;
    MyCfg.DestModify = CDMA_NO_MOD_DEST;
    MyCfg.TimeBase = CDMA_1_US;
    MyCfg.SourcePerfID = perfid;
    destbuf = (uint8_t *) buf;
    srcbuf = (uint8_t *) srcaddr;

    DBG_PRINTF_INFO( "STAT_cfg_buffers() called\n");

    memset(buf,0, bufsize);

    if(CdmaOpen(CDMA_FIND_CHANNEL, &handle) == OK) 
    {
        if(CdmaSetConfig(handle, &MyCfg) == FAIL) 
        {
            result = __LINE__;
            DBG_PRINTF_NOTICE( "Could not set CDMA config.\n");
            return FAIL;
        }

        memset(&MyCfg,0,sizeof(MyCfg));
        if(CdmaGetConfig(handle, &MyCfg) == FAIL) 
        {
            result = __LINE__;
            DBG_PRINTF_NOTICE( "Could not get CDMA config.\n");
            return FAIL;
        }

        // Save the handle for use in the callback 
        pa_cdma_handle = handle;

        // Perform the CDMA transfer (blocking transfer, 32 bytes or less per line)
        tempres = CdmaTransfer(handle,
                               (uint8_t *)srcbuf,
                               (uint8_t *)destbuf,
                               bufsize,
                               0,
                               pa_stat_cdma_callback,
                               (void *)perfid);    // Use the perfid as the context 
        if (tempres != bufsize)
        {
            DBG_PRINTF_NOTICE( "CDMA transfer failed to complete.\n"); 
            return FAIL;
        }
    } 
    else 
    { 
        DBG_PRINTF_NOTICE( "Could not find a CDMA channel.\n");
        result = __LINE__;
        return FAIL;
    }

    return OK;
}

void pa_stat_cdma_close(void)
{
    // Close CDMA handle 
    if(CdmaClose(pa_cdma_handle) == FAIL) 
    {
        DBG_PRINTF_NOTICE( "Could not close the CDMA handle.\n");
        ASSERT(0);
    }
}
#endif
