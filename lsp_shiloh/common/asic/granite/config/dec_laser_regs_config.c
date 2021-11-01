/*
 * ======================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

#include <stddef.h>
#include "posix_ostools.h"
#include "logger.h"
#include "lassert.h"
#include "dec_laser_regs_config.h"
#include "uio_lib_api.h"
#include "asic.h"

/**
 * \brief Get dec config registers memory mapped to user space using UIO kernel driver.
 * 
 */
DEC_CONFIG_REGS_REGS_t* dec_laser_get_dec_config_regs_base()
{
    static void *dev = NULL;
    static DEC_CONFIG_REGS_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_config");
        XASSERT(dev != NULL, errno);
    }

    if(regs == NULL)
    {
        // get device's map
        uio_map_t *map = uio_get_map(dev, 0);

        if (map != NULL)
        {
            regs = map->virt_addr;
        }
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, regs));
    }

    return regs;
}

/**
 * \brief Get apb config registers memory mapped to user space using UIO kernel driver.
 * 
 */
static uio_dev_t* dec_laser_apbconfig_dev = NULL;
static APB_CONFIG_REGS_t* dec_laser_apbconfig_regs = NULL;
APB_CONFIG_REGS_t* dec_laser_open_apb_config_regs()
{
    if(dec_laser_apbconfig_dev == NULL)
    {
        dec_laser_apbconfig_dev = uio_open_dev("apb_config");
        XASSERT(dec_laser_apbconfig_dev != NULL, errno);
    }

    if(dec_laser_apbconfig_regs == NULL)
    {
        // get device's map
        uio_map_t *map = uio_get_map(dec_laser_apbconfig_dev, 0);

        if (map != NULL)
        {
            dec_laser_apbconfig_regs = map->virt_addr;
        }
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, dec_laser_apbconfig_regs));
    }

    return dec_laser_apbconfig_regs;
}

/**
 * \brief Close apb config device and unmap memory from UIO kernel driver.
 * 
 */
void dec_laser_close_apb_config_regs()
{
    if(dec_laser_apbconfig_dev != NULL)
    {
        uio_close_dev(dec_laser_apbconfig_dev);
        dec_laser_apbconfig_dev = NULL;
        dec_laser_apbconfig_regs = NULL;
    }
}

/**
 * \brief Get instance of wavegen registers specified by index 
 * memory mapped to user space using UIO kernel driver.
 * 
 */
WAVEGEN_REGS_t* dec_laser_get_wavegen_regs(uint32_t index)
{
    static void *dev = NULL;
    WAVEGEN_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_wavegen");
        XASSERT(dev != NULL, errno);
    }

    // get device's map
    uio_map_t *map = uio_get_map(dev, index);
    ASSERT(map != NULL);
    if (map != NULL)
    {
        regs = map->virt_addr;
    }
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s %d = %#x\n", __func__, index, regs));

    return regs;
}

/**
 * \brief Get instance of Waveform Generator Control (wfg) registers 
 * specified by index memory mapped to user space using UIO kernel driver.
 * 
 */
WFG0_REGS_t* dec_laser_get_lscan_wfg_regs(uint32_t index)
{
    static void *dev = NULL;
    WFG0_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_lscan_wfg");
        XASSERT(dev != NULL, errno);
    }

    // get device's map
    uio_map_t *map = uio_get_map(dev, index);
    ASSERT(map != NULL);
    if (map != NULL)
    {
        regs = map->virt_addr;
    }
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s %d = %#x\n", __func__, index, regs));

    return regs;
}

/**
 * \brief Get instance of Mirror Control registers 
 * specified by index memory mapped to user space using UIO kernel driver.
 * 
 */
MIRROR0_REGS_t* dec_laser_get_lscan_mirror_regs(uint32_t index)
{
    static void *dev = NULL;
    MIRROR0_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_lscan_mirror");
        XASSERT(dev != NULL, errno);
    }

    // get device's map
    uio_map_t *map = uio_get_map(dev, index);
    ASSERT(map != NULL);
    if (map != NULL)
    {
        regs = map->virt_addr;
    }
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s %d = %#x\n", __func__, index, regs));

    return regs;
}

/**
 * \brief Get Waveform Selector Control (wfs) registers memory mapped to user space using UIO kernel driver.
 * 
 */
WFS_REGS_t* dec_laser_get_lscan_wfs_regs()
{
    static void *dev = NULL;
    static WFS_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_lscan_wfs");
        XASSERT(dev != NULL, errno);
    }

    if(regs == NULL)
    {
        // get device's map
        uio_map_t *map = uio_get_map(dev, 0);

        if (map != NULL)
        {
            regs = map->virt_addr;
        }
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, regs));
    }

    return regs;
}

/**
 * \brief Get HSync Control (wfs) registers memory mapped to user space using UIO kernel driver.
 * 
 */
HSYNC_REGS_t* dec_laser_get_lscan_hsync_regs()
{
    static void *dev = NULL;
    static HSYNC_REGS_t* regs = NULL;

    if(dev == NULL)
    {
        dev = uio_open_dev("dec_lscan_hsync");
        XASSERT(dev != NULL, errno);
    }

    if(regs == NULL)
    {
        // get device's map
        uio_map_t *map = uio_get_map(dev, 0);

        if (map != NULL)
        {
            regs = map->virt_addr;
        }
        DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, regs));
    }

    return regs;
}

/**
 * \brief Get video HIPWM registers memory mapped to user space using video UIO kernel driver.
 * 
 */
uint32_t dec_laser_hipwm_regs_opened = 0; ///< bitfield tracking the indicies of which HIPWM regs are currently open
static uio_dev_t *dec_laser_video_dev = NULL;
static void* dec_laser_video_regs_base = NULL;
HIPWM0_REGS_t* dec_laser_open_hipwm_regs(uint32_t index)
{
    #define VIDEO_HIPWM0_OFFSET (MVDO_HIPWM0_BASE - MVDO_BASE)
    #define HIPWM_REG_INSTANCE_OFFSET (MVDO_HIPWM1_BASE - MVDO_HIPWM0_BASE)

    HIPWM0_REGS_t* hipwm_regs_base = NULL;
    
    // open video device since HIPWM is a subset of the video register block
    if(dec_laser_video_dev == NULL)
    {
        dec_laser_video_dev = uio_open_dev("video");
        XASSERT(dec_laser_video_dev != NULL, errno);
    }

    if (dec_laser_video_regs_base == NULL)
    {
        // get video map
        uio_map_t *map = uio_get_map(dec_laser_video_dev, 0);

        if (map != NULL)
        {
            dec_laser_video_regs_base = map->virt_addr;
        }
    }
    ASSERT(dec_laser_video_regs_base != NULL);

    hipwm_regs_base = (HIPWM0_REGS_t*) ((char*)dec_laser_video_regs_base + 
                                        VIDEO_HIPWM0_OFFSET +                   // offset from base of video to HIPWM regs base
                                        (HIPWM_REG_INSTANCE_OFFSET*index));     // offset from HIPWM regs base to each reg instance

    dec_laser_hipwm_regs_opened |= 1<<index;
    
    DPRINTF(DBG_LOUD|DBG_OUTPUT, ("%s = %#x\n", __func__, hipwm_regs_base));

    return hipwm_regs_base;
}

/**
 * \brief Close an index of HIPWM registers memory mapped to user space using video UIO kernel driver. 
 * When the last index is close the handle to the video driver is closed.
 * 
 */
void dec_laser_close_hipwm_regs(uint32_t index)
{
    dec_laser_hipwm_regs_opened &= ~(1<<index);
    if(dec_laser_hipwm_regs_opened == 0)
    {
        uio_close_dev(dec_laser_video_dev);
        dec_laser_video_dev = NULL;
        dec_laser_video_regs_base = NULL;
    }
}

