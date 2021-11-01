/****************************************************************************** 
 * Copyright (c) 2009 - 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the PIP Stats block.
  * 
 **/

#include "asic.h"
#include "pip_stats_api.h"
#include "pip.h"

#include "string.h"
#include "error_types.h"
#include "cdma_api.h"
#include "logger.h"

#define DBG_PRFX "PIP_STATS: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT( 18 )

cdma_handle_t *stata_cdma_handle = NULL;
cdma_handle_t *statb_cdma_handle = NULL;
cdma_handle_t *statc_cdma_handle = NULL;

#ifndef CDMA_AHS_0
#define CDMA_AHS_0 CDMA_STATS_AHS_0_RX
#define CDMA_AHS_1 CDMA_STATS_AHS_1_RX
#define CDMA_AHS_2 CDMA_STATS_AHS_2_RX
#endif

//********************************
// stat_cfg_driver - setup the STAT block
//********************************
void stat_cfg_driver(PIP_STATS_REGS_t *stat_reg, stat_cfg_t *stat_cfg)
{
    uint32_t reg_val;

    DBG_PRINTF_NOTICE("Called stat_cfg_driver() \n\r");

    // STAT configuration register (ACR)
    reg_val = stat_reg->ACR;
    reg_val = PIP_STATS_ACR_IMAGE_WIDTH_M1_REPLACE_VAL(reg_val,stat_cfg->image_width);
    reg_val = PIP_STATS_ACR_ACCUM_ENABLE_REPLACE_VAL(reg_val,stat_cfg->accumulate);
    reg_val = PIP_STATS_ACR_STATS_ENABLE_REPLACE_VAL(reg_val,stat_cfg->stats_enable);
    reg_val = PIP_STATS_ACR_BYPASS_ENABLE_REPLACE_VAL(reg_val,
                !(stat_cfg->accumulate || stat_cfg->stats_enable));
    stat_reg->ACR = reg_val;

    // STAT A configuration register (ASACR)
    reg_val = stat_reg->ASACR;
    reg_val = PIP_STATS_ASACR_CH0_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch0_thresh);
    reg_val = PIP_STATS_ASACR_CH1_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch1_thresh);
    reg_val = PIP_STATS_ASACR_CH4_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch4_thresh);
    reg_val = PIP_STATS_ASACR_STATS_A_SWATH_HEIGHT_M1_REPLACE_VAL(reg_val,stat_cfg->a_swath_height);
    stat_reg->ASACR = reg_val;

    // STAT B configuration register (ASBCR)
    reg_val = stat_reg->ASBCR;
    reg_val = PIP_STATS_ASBCR_CH2_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch2_thresh);
    reg_val = PIP_STATS_ASBCR_CH3_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch3_thresh);
    reg_val = PIP_STATS_ASBCR_STATS_B_SWATH_HEIGHT_M1_REPLACE_VAL(reg_val,stat_cfg->b_swath_height);
    stat_reg->ASBCR = reg_val;

    // STAT C configuration register (ASCCR)
    reg_val = stat_reg->ASCCR;
    reg_val = PIP_STATS_ASCCR_CH5_THRESH_REPLACE_VAL(reg_val,stat_cfg->ch5_thresh);
    reg_val = PIP_STATS_ASCCR_STATS_C_SWATH_HEIGHT_M1_REPLACE_VAL(reg_val,stat_cfg->c_swath_height);
    stat_reg->ASCCR = reg_val;
}

void stat_cdma_callback(void *user_data, uint32_t int_status)
{
    pip_notify_stat_cdma_done((uint32_t)user_data);
}

error_type_t stat_cfg_buffer(uint32_t perfid)
{
    // Allocate CDMA channel to output STAT data to specified buffer
    cdma_handle_t *handle = NULL;
    CDMA_CONFIG MyCfg;

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

    DBG_PRINTF_INFO("stat_cfg__buffers() called\n");

    if(CdmaOpen(CDMA_FIND_CHANNEL, &handle) == OK) 
    {
        if(CdmaSetConfig(handle, &MyCfg) == FAIL) 
        {
            DBG_PRINTF_NOTICE("Could not set CDMA config.\n");
            return FAIL;
        }

        memset(&MyCfg,0,sizeof(MyCfg));
        if(CdmaGetConfig(handle, &MyCfg) == FAIL) 
        {
            DBG_PRINTF_NOTICE("Could not get CDMA config.\n");
            return FAIL;
        }

        // Save the handle for use in the callback 
        if (CDMA_AHS_0 == perfid)
        {
            stata_cdma_handle = handle;
        }
        else if (CDMA_AHS_1 == perfid)
        {
            statb_cdma_handle = handle;
        }
        else
        {
            XASSERT(CDMA_AHS_2 == perfid, perfid);
            statc_cdma_handle = handle;
        }
    } 
    else 
    { 
        DBG_PRINTF_NOTICE("Could not find a CDMA channel.\n");
        return FAIL;
    }

    return OK;
}

error_type_t stat_dma_start(uint32_t *buf, uint32_t bufsize, uint32_t *srcaddr, uint32_t perfid)
{
    uint32_t tempres;
    cdma_handle_t *handle = NULL;

    // Get the handle 
    if (CDMA_AHS_0 == perfid)
    {
        handle = stata_cdma_handle;
    }
    else if (CDMA_AHS_1 == perfid)
    {
        handle = statb_cdma_handle;
    }
    else
    {
        XASSERT(CDMA_AHS_2 == perfid, perfid);
        handle = statc_cdma_handle;
    }
    ASSERT(NULL != handle);

    // Perform the CDMA transfer (non-blocking transfer, 32 bytes or less per line)
    tempres = CdmaTransfer(handle,
                           (uint8_t *)srcaddr,
                           (uint8_t *)buf,
                           bufsize,
                           0,
                           stat_cdma_callback,
                           (void *)perfid);    // Use the perfid as the context 
    if (tempres != bufsize)
    {
        DBG_PRINTF_NOTICE( "CDMA transfer failed to complete.\n"); 
        return FAIL;
    }

    return OK;
}

void stat_cdma_close(void)
{
    // Close CDMA handles 
    if (NULL != stata_cdma_handle)
    {
        if(CdmaClose(stata_cdma_handle) == FAIL) 
        {
            DBG_PRINTF_NOTICE("Could not close the CDMA handle.\n");
            ASSERT(0);
        }
    }
    if (NULL != statb_cdma_handle)
    {
        if(CdmaClose(statb_cdma_handle) == FAIL) 
        {
            DBG_PRINTF_NOTICE("Could not close the CDMA handle.\n");
            ASSERT(0);
        }
    }
    if (NULL != statc_cdma_handle)
    {
        if(CdmaClose(statc_cdma_handle) == FAIL) 
        {
            DBG_PRINTF_NOTICE("Could not close the CDMA handle.\n");
            ASSERT(0);
        }
    }

    stata_cdma_handle = NULL;
    statb_cdma_handle = NULL;
    statc_cdma_handle = NULL;
}

//********************************
// stat_read_stats
// Read stat registers
//********************************
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))

void stat_read_stats(uint32_t *bufa, 
                     uint32_t *bufb, 
                     uint32_t *bufc, 
                     pip_stats_entry_t *combined_stats,
                     pip_stats_entry_t chan_stats[])
{
    uint32_t reg_val;
    uint32_t minval,maxval;

    // Channel 0 
    reg_val = *bufa;
    chan_stats[0].smin = reg_val & 0xFFFF;
    chan_stats[0].smax = (reg_val >> 16) & 0xFFFF;
    minval = chan_stats[0].smin;
    maxval = chan_stats[0].smax;
    bufa += 1;

    // Channel 1 
    reg_val = *bufa;
    chan_stats[1].smin = reg_val & 0xFFFF;
    chan_stats[1].smax = (reg_val >> 16) & 0xFFFF;
    minval = min(minval,chan_stats[1].smin);
    maxval = max(maxval,chan_stats[1].smax);
    bufa += 1;

    // Channel 4 
    reg_val = *bufa;
    chan_stats[4].smin = reg_val & 0xFFFF;
    chan_stats[4].smax = (reg_val >> 16) & 0xFFFF;
    minval = min(minval,chan_stats[4].smin);
    maxval = max(maxval,chan_stats[4].smax);
    bufa += 1;

    chan_stats[0].accum = *bufa;     // accumulated value	
    bufa += 1;
    chan_stats[1].accum = *bufa;     // accumulated value	
    bufa += 1;
    chan_stats[4].accum = *bufa;     // accumulated value	

    // Channel 2 
    reg_val = *bufb;
    chan_stats[2].smin = reg_val & 0xFFFF;
    chan_stats[2].smax = (reg_val >> 16) & 0xFFFF;
    minval = min(minval,chan_stats[2].smin);
    maxval = max(maxval,chan_stats[2].smax);
    bufb += 1;

    // Channel 3
    reg_val = *bufb;
    chan_stats[3].smin = reg_val & 0xFFFF;
    chan_stats[3].smax = (reg_val >> 16) & 0xFFFF;
    minval = min(minval,chan_stats[3].smin);
    maxval = max(maxval,chan_stats[3].smax);
    bufb += 1;

    chan_stats[2].accum = *bufb;     // accumulated value	
    bufb += 1;
    chan_stats[3].accum = *bufb;     // accumulated value	

    // Channel 5 
    reg_val = *bufc;
    chan_stats[5].smin = reg_val & 0xFFFF;
    chan_stats[5].smax = (reg_val >> 16) & 0xFFFF;
#ifdef INCLUDE_CH5
    minval = min(minval,chan_stats[5].smin);
    maxval = max(maxval,chan_stats[5].smax);
#endif
    bufc += 1;
    chan_stats[5].accum = *bufc;     // accumulated value	

    // populated combined stats output values
    combined_stats->smin = minval;
    combined_stats->smax = maxval;
    combined_stats->accum = chan_stats[0].accum +
                            chan_stats[1].accum +
                            chan_stats[2].accum +
                            chan_stats[3].accum +
                            chan_stats[4].accum +
                            chan_stats[5].accum;
}
