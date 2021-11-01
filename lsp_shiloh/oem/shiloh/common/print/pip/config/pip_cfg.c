/****************************************************************************** 
 * Copyright (c) 2009 - 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
  * 
  *This file contains functions for the PIP config.
  * 
 **/

#include "ATypes.h"
#include "pip_api.h"
#include "pip_cfg.h"
#include "print_mode.h"
#include "asic.h"

// Basic 1D LUTs
#include "pa_ident_lut.h"
#include "pa_zero_lut.h"
#include "pa_invert_lut.h"
#include "pa_inv1d_ued_1bpp.h"
#include "pa_inv1d_ued_2bpp.h"
#include "pa_inv1d_th_smooth.h"
#include "pa_inv1d_th_detail.h"
#include "pa_inv1d_8b_lin.h"
#include "color_reg_enhance_cfg.h"
// Basic Colormaps
#include "sccsc_identity_3d.h"
#ifdef ASIC_granite
#include "sccsc_6ch_10bit_colormap_bgr.h"
#include "sccsc_6ch_10bit_colormap_linear_rgb.h" // linear rgb
#include "sccsc_6ch_10bit_colormap_linear_5bgr.h" // linear rgb
#include "sccsc_6ch_8bit_colormap_linear_5bgr.h" // linear rgb
#define sccsc_6ch_10bit_colormap sccsc_6ch_10bit_colormap_bgr 
// #include "sccsc_6ch_10bit_colormap_colorz.h"
extern unsigned char *th_stoc_c_1bit_data;
#else
#include "sccsc_6ch_10bit_colormap.h"
#endif

static pip_cfg_t pip_cfg_table[] =
{
#include "ipc_print_color24_1bpp.c"  // not used rgb->cmyk threshold 
#include "ipc_print_rgb24_1bpp.c" // convert rgb -> 1bitk threshold.
#include "ipc_print_mono8_detail.c"
#include "ipc_print_mono8_smooth.c"
#include "ipc_print_additive_gray_smooth.c"
#include "ipc_print_additive_gray_detail.c"
#include "ipc_copy_mono8_detail.c" 
#include "ipc_copy_mono8_film_detail.c" 

};


static pip_cfg_t *pip_cfg_find_match(uint32_t mode)
{
    uint32_t i;
    uint32_t num_pip_cfg_entries;
    pip_cfg_t *match = NULL;

    num_pip_cfg_entries = sizeof(pip_cfg_table) / sizeof(pip_cfg_t);

    for (i=0; i<num_pip_cfg_entries; i++)
    {
        if (mode == pip_cfg_table[i].cfg_mode)
        {
            match = &pip_cfg_table[i];
            break;
        }
    }

    return match;
}

bool pip_cfg_is_mode_supported(uint32_t mode)
{
    bool supported;
    pip_cfg_t *pip_cfg_match;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        supported = true;
    }
    else
    {
        supported = false;
    }

    return supported;
}

error_type_t pip_cfg_copy(uint32_t mode, pip_cfg_t *cfg)
{
    error_type_t ret;
    pip_cfg_t *pip_cfg_match;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        *cfg = *pip_cfg_match;
        ret = OK;
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}

error_type_t pip_cfg_reference_get(uint32_t mode, pip_cfg_t **cfg)
{
    error_type_t ret;
    pip_cfg_t *pip_cfg_match;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        *cfg = pip_cfg_match;
        ret = OK;
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}

error_type_t pip_cfg_mode_bpp_get(uint32_t mode, uint32_t *bpp)
{
    error_type_t ret;
    pip_cfg_t *pip_cfg_match;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        *bpp = pip_cfg_match->bpp;
        ret = OK;
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}

const char *pip_cfg_mode_name_get(uint32_t mode)
{
    pip_cfg_t *pip_cfg_match;
    const char *name;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        name = pip_cfg_match->cfg_name;
    }
    else
    {
        name = "";
    }

    return name;
}

#if 0
error_type_t pip_cfg_out_chan_map_get(uint32_t mode,
                                      pip_odma_chan_map_entry_t out_chan_map[])
{
    uint32_t chan; 
    error_type_t ret;
    pip_cfg_t *pip_cfg_match;

    pip_cfg_match = pip_cfg_find_match(mode);

    if (NULL != pip_cfg_match)
    {
        for (chan=0; chan<PIP_MAX_OUTPUT_CHANS; chan++)
        {
            if (NULL == pip_cfg_match->raster_odma_cfg[chan])
            {
                ASSERT(false == pip_cfg_match->out_chan_map[chan].enabled);
            }
            else
            {
                ASSERT(true == pip_cfg_match->out_chan_map[chan].enabled);
            }
            out_chan_map[chan] = pip_cfg_match->out_chan_map[chan];
        }
        ret = OK;
    }
    else
    {
        ret = FAIL;
    }

    return ret;
}
#endif
