/******************************************************************************
 * Copyright (c) 2009 - 2012  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 *
 * \file pip_test.c
 *
 * \brief Driver routines for PIP (image pipeline) HW block of ASIC.
 *
 * See the pip_api.h file for details on usage.
 *
 */

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "asic.h"
#include "pip_api.h"
#include "pip.h"
#include "pip_utils.h"
#include "zx_test_api.h"
#include "zx_idma_api.h"
#include "ued_api.h"
#include "csc_api.h"
#include "nga_api.h"
#include "davbuf_api.h"
#include "pip_stats_api.h"
#include "fhs_cfg.h"
#include "kmod_cfg.h"
#include "davbuf_cfg.h"
#include "stat_cfg.h"

#ifndef __linux__
#include "interrupt_api.h"

#include "cpu_api.h"
#include "tx_api.h"
#include "cdma_api.h"
#include "intnums.h"

#endif

#include "dma_buffer.h"
#include "memAPI.h"
#include "pip_cfg.h"
#include "print_mode.h"
#include "lassert.h"

#include "pstest_rgb32.h"
#include "pstest_bilevel.h"
#define INCLUDE_PIPTEST
#include "bvtest_mono8.h"

#include "raster_odma_api.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
//#define DEBUG_OVER_CIFS
#ifdef DEBUG_OVER_CIFS
#include <stdio.h>
#define DEBUG_OVER_CIFS_NUM_PLANES 6
#endif
#endif

zx_desc_t __attribute__ ((aligned(512))) zx_desc_cmds[MAX_ZX_DESCRIPTORS];   // Chain descs together

uint32_t __attribute__ ((aligned(512))) zx_buf_space[50];   // only 1 used, rest is buffer space

#define WIDTH_BYTES WIDTH
#define LINES HEIGHT
#define DBG_PRFX "PIP_TEST: "
#define PAD32(x) (((x) & 0x1f) ? (x) + (32-((x) % 32)) : (x))

// Configuration items
#ifdef HAVE_CMD
static uint32_t pipmode = IPC_UNUSED;  // Default to photo - setby pipmode ser console
static pip_cfg_t* pip_cfg = NULL;
static bool pippad = 0;   // Default to draft - setby pipmode ser console
static bool piptextmerge = 0; // Off by default
static uint32_t pipscale_x = 1; // 1, 2, or 4
static uint32_t pipscale_y = 1; // 1, 2, or 4
static uint32_t pipfscale = 100;
static bool pip_flip_horiz = false;
#endif

void sccsc_rd_ram(SCCSC_REGS_t *sc_csc_reg, uint32_t addr, uint32_t count,
                  uint64_t *data);

#define NDVALS 100
uint64_t pi_dvals[NDVALS];


#ifdef HAVE_CMD
static int pip_set_mode(uint32_t value)
{
    error_type_t err;
    pipmode = value;
    err = pip_cfg_reference_get(value, &pip_cfg);
    ASSERT(OK == err);

    return 1;
}

static int pip_set_fscale(uint32_t value)
{
    pipfscale = value;
    ASSERT(pipfscale >= 100);
    pip_frac_scale_set(pipfscale);

    return 1;
}

static int pip_set_hflip(uint32_t value)
{
    pip_flip_horiz = (bool)value;

    return 1;
}

static int pip_info(void)
{
    int i;
    uint32_t v1;
    uint32_t v2;

    sccsc_rd_ram(sc_csc_reg,0x00,NDVALS,pi_dvals);
    for (i = 0; i < NDVALS; i++)
    {
        v1 = (pi_dvals[i]>>32) & 0xFFFFFFFFUL;
        v2 = pi_dvals[i] & 0xFFFFFFFFUL;
        cmd_printf( "iLUT %d = %x %x\n", i, v1, v2 );
    }

    return 1;
}
#endif // HAVE_CMD

// Register callback

// Prototype
#ifdef HAVE_CMD
static error_type_t pip_davbuf_write(uint32_t addr, uint32_t value);
static error_type_t pip_davbuf_read(uint32_t addr);
static error_type_t pip_davbuf_zero(uint32_t num_entries);
static int pip_set_scale(uint32_t scale_x, uint32_t scale_y);
static int pip_set_pad(uint32_t value);
static int pip_set_mode(uint32_t value);
static int pip_set_textmerge(uint32_t value);
static int pip_info(void);
static int pip_set_fscale(uint32_t value);
static int pip_smartp_driver(void);
static int pip_strip_driver(void);
static int pip_pastore_driver(uint32_t nchans);
static int pip_paload_driver(uint32_t nchans);
static int pip_mono_driver(void);
static void pip_process_mono_strip_test(void);
static uint32_t checksum(uint8_t *dptr, uint32_t nbytes);
#ifdef DEBUG_OVER_CIFS
static void pip_test_open_debug_data_output(uint32_t num_planes);
static void pip_test_debug_data_write_raster_line(uint32_t row_stride, uint32_t num_planes, uint32_t *flocs[]);
static void pip_test_close_debug_data_output(uint32_t num_planes);
#endif

static const char *pip_cmd_wdavbuf_desc;
static const char *pip_cmd_wdavbuf_usage;
static const char *pip_cmd_wdavbuf_notes;
static int pip_cmd_wdavbuf_cb(int argc, char *argv[]);

static const char *pip_cmd_rdavbuf_desc;
static const char *pip_cmd_rdavbuf_usage;
static const char *pip_cmd_rdavbuf_notes;
static int pip_cmd_rdavbuf_cb(int argc, char *argv[]);

static const char *pip_cmd_zdavbuf_desc;
static const char *pip_cmd_zdavbuf_usage;
static const char *pip_cmd_zdavbuf_notes;
static int pip_cmd_zdavbuf_cb(int argc, char *argv[]);

static const char *pip_cmd_scale_desc;
static const char *pip_cmd_scale_usage;
static const char *pip_cmd_scale_notes;
static int pip_cmd_scale_cb(int argc, char *argv[]);

static const char *pip_cmd_pad_desc;
static const char *pip_cmd_pad_usage;
static const char *pip_cmd_pad_notes;
static int pip_cmd_pad_cb(int argc, char *argv[]);

static const char *pip_cmd_mode_desc;
static const char *pip_cmd_mode_usage;
static const char *pip_cmd_mode_notes;
static int pip_cmd_mode_cb(int argc, char *argv[]);

static const char *pip_cmd_textmerge_desc;
static const char *pip_cmd_textmerge_usage;
static const char *pip_cmd_textmerge_notes;
static int pip_cmd_textmerge_cb(int argc, char *argv[]);

static const char *pip_cmd_sclut_desc;
static const char *pip_cmd_sclut_usage;
static const char *pip_cmd_sclut_notes;
static int pip_cmd_sclut_cb(int argc, char *argv[]);

static const char *pip_cmd_fscl_desc;
static const char *pip_cmd_fscl_usage;
static const char *pip_cmd_fscl_notes;
static int pip_cmd_fscl_cb(int argc, char *argv[]);

static const char *pip_cmd_hflip_desc;
static const char *pip_cmd_hflip_usage;
static const char *pip_cmd_hflip_notes;
static int pip_cmd_hflip_cb(int argc, char *argv[]);

static const char *pip_cmd_smart_desc;
static const char *pip_cmd_smart_usage;
static const char *pip_cmd_smart_notes;
static int pip_cmd_smart_cb(int argc, char *argv[]);

static const char *pip_cmd_strip_desc;
static const char *pip_cmd_strip_usage;
static const char *pip_cmd_strip_notes;
static int pip_cmd_strip_cb(int argc, char *argv[]);

static const char *pip_cmd_past_desc;
static const char *pip_cmd_past_usage;
static const char *pip_cmd_past_notes;
static int pip_cmd_past_cb(int argc, char *argv[]);

static const char *pip_cmd_pald_desc;
static const char *pip_cmd_pald_usage;
static const char *pip_cmd_pald_notes;
static int pip_cmd_pald_cb(int argc, char *argv[]);

static const char *pip_cmd_mono_desc;
static const char *pip_cmd_mono_usage;
static const char *pip_cmd_mono_notes;
static int pip_cmd_mono_cb(int argc, char *argv[]);

static const char *pip_cmd_mstrip_desc;
static const char *pip_cmd_mstrip_usage;
static const char *pip_cmd_mstrip_notes;
static int pip_cmd_mstrip_cb(int argc, char *argv[]);

static const char *pip_cmd_routing_8x8_desc;
static const char *pip_cmd_routing_8x8_usage;
static const char *pip_cmd_routing_8x8_notes;
static int pip_cmd_routing_8x8_cb(int argc, char *argv[]);

#ifdef HAVE_ZLIB_PIP
static const char *pip_cmd_ztest_desc;
static const char *pip_cmd_ztest_usage;
static const char *pip_cmd_ztest_notes;
static int pip_cmd_ztest_cb(int argc, char *argv[]);
#endif
#endif // HAVE_CMD


error_type_t pip_test_init(void)
{
    #ifdef HAVE_CMD
    int retcode;

    // Register cmds
    retcode = cmd_register_cmd( "pip",
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "wdavbuf",
                                   pip_cmd_wdavbuf_desc,
                                   pip_cmd_wdavbuf_usage,
                                   pip_cmd_wdavbuf_notes,
                                   pip_cmd_wdavbuf_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "rdavbuf",
                                   pip_cmd_rdavbuf_desc,
                                   pip_cmd_rdavbuf_usage,
                                   pip_cmd_rdavbuf_notes,
                                   pip_cmd_rdavbuf_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "zdavbuf",
                                   pip_cmd_zdavbuf_desc,
                                   pip_cmd_zdavbuf_usage,
                                   pip_cmd_zdavbuf_notes,
                                   pip_cmd_zdavbuf_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "scale",
                                   pip_cmd_scale_desc,
                                   pip_cmd_scale_usage,
                                   pip_cmd_scale_notes,
                                   pip_cmd_scale_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "pad",
                                   pip_cmd_pad_desc,
                                   pip_cmd_pad_usage,
                                   pip_cmd_pad_notes,
                                   pip_cmd_pad_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "mode",
                                   pip_cmd_mode_desc,
                                   pip_cmd_mode_usage,
                                   pip_cmd_mode_notes,
                                   pip_cmd_mode_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "textmerge",
                                   pip_cmd_textmerge_desc,
                                   pip_cmd_textmerge_usage,
                                   pip_cmd_textmerge_notes,
                                   pip_cmd_textmerge_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "sclut",
                                   pip_cmd_sclut_desc,
                                   pip_cmd_sclut_usage,
                                   pip_cmd_sclut_notes,
                                   pip_cmd_sclut_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "fscl",
                                   pip_cmd_fscl_desc,
                                   pip_cmd_fscl_usage,
                                   pip_cmd_fscl_notes,
                                   pip_cmd_fscl_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "hflip",
                                   pip_cmd_hflip_desc,
                                   pip_cmd_hflip_usage,
                                   pip_cmd_hflip_notes,
                                   pip_cmd_hflip_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "smart",
                                   pip_cmd_smart_desc,
                                   pip_cmd_smart_usage,
                                   pip_cmd_smart_notes,
                                   pip_cmd_smart_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "strip",
                                   pip_cmd_strip_desc,
                                   pip_cmd_strip_usage,
                                   pip_cmd_strip_notes,
                                   pip_cmd_strip_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "past",
                                   pip_cmd_past_desc,
                                   pip_cmd_past_usage,
                                   pip_cmd_past_notes,
                                   pip_cmd_past_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "pald",
                                   pip_cmd_pald_desc,
                                   pip_cmd_pald_usage,
                                   pip_cmd_pald_notes,
                                   pip_cmd_pald_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "mono",
                                   pip_cmd_mono_desc,
                                   pip_cmd_mono_usage,
                                   pip_cmd_mono_notes,
                                   pip_cmd_mono_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "mstrip",
                                   pip_cmd_mstrip_desc,
                                   pip_cmd_mstrip_usage,
                                   pip_cmd_mstrip_notes,
                                   pip_cmd_mstrip_cb );
    ASSERT( CMD_OK == retcode );

    retcode = cmd_register_subcmd( "pip",
                                   "routing_8x8",
                                   pip_cmd_routing_8x8_desc,
                                   pip_cmd_routing_8x8_usage,
                                   pip_cmd_routing_8x8_notes,
                                   pip_cmd_routing_8x8_cb );
    ASSERT( CMD_OK == retcode );
#ifdef HAVE_ZLIB_PIP
    retcode = cmd_register_subcmd( "pip",
                                   "ztest",
                                   pip_cmd_ztest_desc,
                                   pip_cmd_ztest_usage,
                                   pip_cmd_ztest_notes,
                                   pip_cmd_ztest_cb );
    ASSERT( CMD_OK == retcode );
#endif
    #endif

    return OK;
}

DAVBUFF_REGS_t *davbp = (DAVBUFF_REGS_t *)PIP_DB_HT_BASE;

#define PIP_DB_HT_NUM_ENTRIES 10240
#define PIP_DB_HT_MAX_VAL 65535
#define TO 1000000 // Timeout count

#ifdef HAVE_CMD
static error_type_t pip_davbuf_write(uint32_t addr, uint32_t value)
{
    error_type_t ret = OK;
    int timeout;

    if ((addr<PIP_DB_HT_NUM_ENTRIES) && (value<PIP_DB_HT_MAX_VAL))
    {
        davbp->CFG = 0x03;
        davbp->DEBUG_PIX_ADDR = addr;
        davbp->DEBUG_DATA_IDX = 0;
        davbp->DEBUG_DATA_WR = value;
        davbp->DEBUG_GO = 1;
        for (timeout = TO; (davbp->DEBUG_STAT & DAVBUFF_SR_STAT_SRBUSY_MASK && timeout); timeout--);

        if (!timeout)
        {
            cmd_printf( "pip_davbuf_write Timeout\n" );
            ret = FAIL;
        }
        else
        {
            cmd_printf( "pip_davbuf_write: wrote 0x%08x to address 0x%08x\n", value, addr );
        }
    }
    else
    {
        cmd_printf( "pip_davbuf_write failed, input params out of range: address 0x%08x value 0x%08x\n", addr, value );
        ret = FAIL;
    }

    return ret;
}

static error_type_t pip_davbuf_read(uint32_t addr)
{
    error_type_t ret = OK;
    int timeout;

    if (addr < PIP_DB_HT_NUM_ENTRIES)
    {
        davbp->CFG = 0x03;
        davbp->DEBUG_PIX_ADDR = addr;
        davbp->DEBUG_DATA_IDX = 0;
        davbp->DEBUG_GO = 0;
        for (timeout = TO; (davbp->DEBUG_STAT & DAVBUFF_SR_STAT_SRBUSY_MASK && timeout); timeout--);

        if (!timeout)
        {
            cmd_printf( "pip_davbuf_read Timeout\n" );
            ret = FAIL;
        }
        else
        {
            cmd_printf( "pip_davbuf_read Address %u = 0x%08x\n",
                        addr,
                        davbp->DEBUG_DATA_RD );
        }
    }
    else
    {
        cmd_printf( "pip_davbuf_read failed, address out of range: address 0x%08x\n", addr );
        ret = FAIL;
    }

    return ret;
}

static error_type_t pip_davbuf_zero(uint32_t num_entries)
{
    error_type_t ret = OK;
    int timeout;
    int i;

    if (num_entries < PIP_DB_HT_NUM_ENTRIES)
    {
        davbp->CFG = 0x03;
        for (i = 0; i < num_entries; i++)
        {
            davbp->DEBUG_PIX_ADDR = i;
            davbp->DEBUG_DATA_IDX = 0;
            davbp->DEBUG_DATA_WR = 0;
            davbp->DEBUG_DATA_IDX = 1;
            davbp->DEBUG_DATA_WR = 0;
            davbp->DEBUG_DATA_IDX = 2;
            davbp->DEBUG_DATA_WR = 0;
            davbp->DEBUG_GO = 1;

            for (timeout = TO; (davbp->DEBUG_STAT & DAVBUFF_SR_STAT_SRBUSY_MASK && timeout); timeout--);

            if (!timeout)
            {
                cmd_printf( "pip_davbuf_zero IDX 0 Timeout\n" );
                ret = FAIL;
            }
        }
        davbp->CFG = 0x0;
    }
    else
    {
        cmd_printf( "pip_davbuf_zero failed, num_entries out of range: num_entries %u\n", num_entries );
        ret = FAIL;
    }

    return(1);
}
#endif // HAVE_CMD


#ifdef HAVE_CMD
static int pip_set_pad(uint32_t value)
{
    // Set a new value
    pippad = value;

    return(1);
}

static int pip_set_scale(uint32_t scale_x, uint32_t scale_y)
{
    pipscale_x = scale_x;
    pipscale_y = scale_y;

    return(1);
}

static int pip_set_textmerge(uint32_t value)
{
    if ( value == 0 )
    {
        piptextmerge = false;
    }
    else
    {
        piptextmerge = true;
    }

    return(1);
}
#endif // HAVE_CMD

#ifdef HAVE_CMD
/**
 *==============================================================================
 * \brief Test driver routine to exercise the PIP HW block for smart passing.
 *   Will process a passed in image (image is read out of absolute memory bufs
 *   defined by DABS_ZX). Invoked via ser dbg console "pip smart" cmd
 *
 *==============================================================================
 */

#define PID_WIDTH_BYTES (*((uint32_t *) (pstestrgb32_bin+24)))
#define PID_LINES       (*((uint32_t *) (pstestrgb32_bin+28)))
#define REPORT_CHECKSUM 1

#define NUM_COLOR_PLANES 6
uint32_t *pid_planep[NUM_COLOR_PLANES];

//#define PSCALE 2
#define PSCALE_X pipscale_x
#define PSCALE_Y pipscale_y
#define PMODE  pipmode
#define PPAD   pippad

#ifdef INCLUDE_PIPTEST
#define MAX_LINE_LENGTH_PS 1024
#define MAX_OUT_LENGTH_PS 1024*563
#else
#define MAX_LINE_LENGTH_PS 1
#define MAX_OUT_LENGTH_PS 1
#endif

void *outbuf_c_ps = NULL;
void *outbuf_m_ps = NULL;
void *outbuf_y_ps = NULL;
void *outbuf_k_ps = NULL;
void *outbuf_p_ps = NULL;
void *outbuf_g_ps = NULL;


static int pip_smartp_driver(void)
{
    uint32_t zx_data;
    uint32_t inc_lines, width_pix_in, width_pix_out;
    uint32_t rows_generated;
    int line_len, rows;
    int i;
#ifdef REPORT_CHECKSUM
    uint32_t dchecksum;
#endif
    uint32_t start_time, end_time;
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    BigBuffer_t *bb[6] = {0};

    cmd_printf( "pip_smartp_driver - size of image = %d\n", sizeof(pstestrgb32_bin));

    if (0 == sizeof(pstestrgb32_bin))
    {
        cmd_printf( "pip_smartp_driver - No image data available, abort smartp test.\n");
        return 0;
    }
    start_time = posix_gettime_ms(); // tx_time_get();

    inc_lines = 1;  // Ouput scaled lines
    width_pix_in = PID_WIDTH_BYTES/4;

#ifdef DEBUG_OVER_CIFS
    pip_test_open_debug_data_output(DEBUG_OVER_CIFS_NUM_PLANES);
#endif

    pip_prepare_page(PMODE,
                     PSCALE_X,
                     PSCALE_Y,
                     width_pix_in,
                     width_pix_in,
                     PID_LINES,    // Input, non-scaled lines
                     inc_lines,    // Output, scale applied lines
                     PPAD,
                     piptextmerge,
                     pip_flip_horiz,
                     &width_pix_out);

    pip_notify_rows_available(PID_LINES, true);

    line_len = pip_cfg->bpp * width_pix_out/8;
    XASSERT(line_len <= MAX_LINE_LENGTH_PS, line_len);
    rows = (PID_LINES*PSCALE_Y)/inc_lines;
    XASSERT((line_len*rows*inc_lines) <= MAX_OUT_LENGTH_PS, (line_len*rows*inc_lines));

    cmd_printf( "pip_smartp_driver - line_len=%u rows=%u\n", line_len, rows);

    zx_data = ((uint32_t) pstestrgb32_bin)+32;  // Skip past sbin header

    // Setup Zx to drive data through
    zx_setup_single(zx_reg, zx_desc_cmds, (uint32_t)zx_data,
                  width_pix_in, PID_LINES);
//lsptodo: descriptor cpu_dcache_writeback_region(zx_desc_cmds, sizeof(zx_desc_cmds));

    // Process nlines at a time
    zx_start(zx_reg, (uint32_t) &zx_desc_cmds[0]);

    // Start the JBIG transaction if necessary
    if (0 != piptextmerge)
    {
        pip_start_jbig(pstestbilevel_bin, (width_pix_out/8)*rows);
    }

    // Setup output line buffers
    mlimiter_t *lim = mlimiter_by_name("print");
    for (i = 0; i < 6; i++) {
        bb[i] = dma_buffer_malloc(lim, MAX_OUT_LENGTH_PS );
    }
    pid_planep[0] = outbuf_c_ps = dma_buffer_map_single( bb[0], DMA_FROM_DEVICE );
    pid_planep[1] = outbuf_m_ps = dma_buffer_map_single( bb[1], DMA_FROM_DEVICE );
    pid_planep[2] = outbuf_y_ps = dma_buffer_map_single( bb[2], DMA_FROM_DEVICE );
    pid_planep[3] = outbuf_k_ps = dma_buffer_map_single( bb[3], DMA_FROM_DEVICE );
    pid_planep[4] = outbuf_p_ps = dma_buffer_map_single( bb[4], DMA_FROM_DEVICE );
    pid_planep[5] = outbuf_g_ps = dma_buffer_map_single( bb[5], DMA_FROM_DEVICE );

    // Run the data through the pipe and generate all output rows
#ifdef REPORT_CHECKSUM
    dchecksum = 0L;
#endif
    for (i = 0; i < rows; i++)
    {
        pid_planep[0] = (outbuf_c_ps + (i*line_len));
        pid_planep[1] = (outbuf_m_ps + (i*line_len));
        pid_planep[2] = (outbuf_y_ps + (i*line_len));
        pid_planep[3] = (outbuf_k_ps + (i*line_len));
        pid_planep[4] = (outbuf_p_ps + (i*line_len));
        pid_planep[5] = (outbuf_g_ps + (i*line_len));

        pip_generate_rows(pid_planep,inc_lines,&combined_stats,chan_stats,1, 1, rows_generated);
    }

    pip_close_page();

#ifdef REPORT_CHECKSUM
    for (i = 0; i < 6; i++) {
        dma_buffer_unmap_single(bb[i], DMA_FROM_DEVICE );
    }
    outbuf_c_ps = dma_buffer_mmap_forcpu( bb[0] );
    outbuf_m_ps = dma_buffer_mmap_forcpu( bb[1] );
    outbuf_y_ps = dma_buffer_mmap_forcpu( bb[2] );
    outbuf_k_ps = dma_buffer_mmap_forcpu( bb[3] );
    outbuf_p_ps = dma_buffer_mmap_forcpu( bb[4] );
    outbuf_g_ps = dma_buffer_mmap_forcpu( bb[5] );

    if(pip_cfg->raster_odma_cfg[0] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_c_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[1] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_m_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[2] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_y_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[3] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_k_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[4] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_p_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[5] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_g_ps, line_len * rows);

    cmd_printf( "pip_smartp_driver - data checksum = %u\n",dchecksum);

#ifdef DEBUG_OVER_CIFS
    pid_planep[0] = outbuf_c_ps;
    pid_planep[1] = outbuf_m_ps;
    pid_planep[2] = outbuf_y_ps;
    pid_planep[3] = outbuf_k_ps;
    pid_planep[4] = outbuf_p_ps;
    pid_planep[5] = outbuf_g_ps;

    pip_test_debug_data_write_raster_line(line_len*rows, DEBUG_OVER_CIFS_NUM_PLANES, pid_planep);

    pip_test_close_debug_data_output(DEBUG_OVER_CIFS_NUM_PLANES);
#endif

/*
    uint32_t j, pix_index;

    if(dchecksum == 80620864)
    {
        for (i = 0; i < ((PID_LINES*PSCALE_Y)/inc_lines); i++)
        {
            for (j = 0; j < (width_pix_out*(pip_cfg->bpp)/8); j++)
            {
                pix_index = (MAX_LINE_LENGTH_PS * i) + j;
                if(pip_cfg->odma0_cfg != NULL)
                    cmd_printf( "%02x ",outbuf_c_ps[pix_index]);
                if(pip_cfg->odma1_cfg != NULL)
                    cmd_printf( "%02x ",outbuf_m_ps[pix_index]);
                if(pip_cfg->odma2_cfg != NULL)
                    cmd_printf( "%02x ",outbuf_y_ps[pix_index]);
                if(pip_cfg->odma3_cfg != NULL)
                    cmd_printf( "%02x ",outbuf_k_ps[pix_index]);
                if(pip_cfg->odma4_cfg != NULL)
                    cmd_printf( "%02x ",outbuf_p_ps[pix_index]);
                if(pip_cfg->odma5_cfg != NULL)
                    cmd_printf( "%02x",outbuf_g_ps[pix_index]);
                cmd_printf( "\n");
            }
        }
    }
*/

#endif

    for (i = 0; i < 6; i++) {
        dma_buffer_unmmap_forcpu( bb[i] );
        dma_buffer_free( bb[i] );
    }
    end_time = posix_gettime_ms(); // tx_time_get();
    cmd_printf( "pip_smartp_driver - time (ms): %u\n", (end_time-start_time));

    return(1);
}
#endif

#ifdef HAVE_CMD
/**
 *==============================================================================
 * \brief Test driver routine to exercise the PIP HW block with the zx_idma
 * front end. Invoked via ser dbg console "pip strip" cmd.
 * 
 *==============================================================================
 */

#define PID_WIDTH_BYTES (*((uint32_t *) (pstestrgb32_bin+24)))
#define PID_LINES       (*((uint32_t *) (pstestrgb32_bin+28)))
#define REPORT_CHECKSUM 1
#define MAX_STRIP_HEIGHT 8

#define NUM_COLOR_PLANES 6
uint32_t *pid_planep[NUM_COLOR_PLANES];

#define PSCALE_X pipscale_x
#define PSCALE_Y pipscale_y
#define PMODE  pipmode 
#define PPAD   pippad

#ifdef INCLUDE_PIPTEST
#define MAX_LINE_LENGTH_PS 1024
#define MAX_OUT_LENGTH_PS 1024*563
#else
#define MAX_LINE_LENGTH_PS 1
#define MAX_OUT_LENGTH_PS 1
#endif

void *outbuf_strip_c_ps = 0;
void *outbuf_strip_m_ps = 0;
void *outbuf_strip_y_ps = 0;
void *outbuf_strip_k_ps = 0;
void *outbuf_strip_p_ps = 0;
void *outbuf_strip_g_ps = 0;


static int pip_strip_driver(void)
{
    uint32_t zx_data;
    uint32_t inc_lines, width_pix_in, width_pix_out;
    uint32_t rows_generated;
    uint32_t strip_height;
    int line_len, rows;
    int i;
#ifdef REPORT_CHECKSUM
    uint32_t dchecksum;
#endif
    uint32_t start_time, end_time;
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    error_type_t ret;
    BigBuffer_t *bb[6];

    cmd_printf( "pip_strip_driver - size of image = %d\n", sizeof(pstestrgb32_bin)); 

    if (0 == sizeof(pstestrgb32_bin))
    {
        cmd_printf( "pip_strip_driver - No image data available, abort strip test.\n"); 
        return 0;
    }
    start_time = posix_gettime_ms();

    inc_lines = 1;  // Ouput scaled lines 
    width_pix_in = PID_WIDTH_BYTES/4;

#ifdef DEBUG_OVER_CIFS
    pip_test_open_debug_data_output(DEBUG_OVER_CIFS_NUM_PLANES);
#endif

    pip_prepare_page(PMODE,
                     PSCALE_X,
                     PSCALE_Y,
                     width_pix_in,
                     width_pix_in,
                     PID_LINES,    // Input, non-scaled lines
                     inc_lines,    // Output, scale applied lines
                     PPAD,
                     piptextmerge,
                     pip_flip_horiz,
                     &width_pix_out);

    pip_notify_rows_available(PID_LINES, true);

    line_len = pip_cfg->bpp * width_pix_out/8;
    XASSERT(line_len <= MAX_LINE_LENGTH_PS, line_len);
    rows = (PID_LINES*PSCALE_Y)/inc_lines;
    XASSERT((line_len*rows*inc_lines) <= MAX_OUT_LENGTH_PS, (line_len*rows*inc_lines));

    cmd_printf( "pip_strip_driver - line_len=%u rows=%u\n", line_len, rows); 

    zx_data = ((uint32_t) pstestrgb32_bin)+32;  // Skip past sbin header

    // Setup Zx IDMA to drive data through
    ret = zx_idma_open();
    ASSERT(OK == ret);

    // Add the first strip of input data
    ret = zx_idma_add_buffer((uint8_t *)zx_data, width_pix_in, MAX_STRIP_HEIGHT);
    ASSERT(OK == ret);

    // Process nlines at a time
    ret = zx_idma_start(width_pix_in, PID_LINES);
    ASSERT(OK == ret);

    // Start the JBIG transaction if necessary
    if (0 != piptextmerge) 
    {
        pip_start_jbig(pstestbilevel_bin, (width_pix_out/8)*rows);
    }

    // Setup output line buffers
    mlimiter_t *lim = mlimiter_by_name("print");
    for (i = 0; i < 6; i++) {
        bb[i] = dma_buffer_malloc(lim, MAX_OUT_LENGTH_PS );
    }
    pid_planep[0] = outbuf_strip_c_ps = dma_buffer_map_single( bb[0], DMA_FROM_DEVICE );
    pid_planep[1] = outbuf_strip_m_ps = dma_buffer_map_single( bb[1], DMA_FROM_DEVICE );
    pid_planep[2] = outbuf_strip_y_ps = dma_buffer_map_single( bb[2], DMA_FROM_DEVICE );
    pid_planep[3] = outbuf_strip_k_ps = dma_buffer_map_single( bb[3], DMA_FROM_DEVICE );
    pid_planep[4] = outbuf_strip_p_ps = dma_buffer_map_single( bb[4], DMA_FROM_DEVICE );
    pid_planep[5] = outbuf_strip_g_ps = dma_buffer_map_single( bb[5], DMA_FROM_DEVICE );

    // Run the data through the pipe and generate all output rows
#ifdef REPORT_CHECKSUM
    dchecksum = 0L;
#endif
    for (i = 0; i < rows; i++) 
    {
        // Add another strip of input data if necessary
        if (0 == (i%MAX_STRIP_HEIGHT) &&
            (i+MAX_STRIP_HEIGHT) < rows)
        {
            // Calculate the height of this strip
            strip_height = rows - (i+MAX_STRIP_HEIGHT);
            strip_height = strip_height > MAX_STRIP_HEIGHT ? MAX_STRIP_HEIGHT : strip_height;

            // Offset into the input buffer and add the strip.
            ret = zx_idma_add_buffer((uint8_t *)zx_data + (i+MAX_STRIP_HEIGHT)*width_pix_in*4,
                                     width_pix_in,
                                     strip_height);
            ASSERT(OK == ret);        
        }

        // Generate one row of output data
        pid_planep[0] = (outbuf_strip_c_ps + (i*line_len)); 
        pid_planep[1] = (outbuf_strip_m_ps + (i*line_len)); 
        pid_planep[2] = (outbuf_strip_y_ps + (i*line_len)); 
        pid_planep[3] = (outbuf_strip_k_ps + (i*line_len)); 
        pid_planep[4] = (outbuf_strip_p_ps + (i*line_len)); 
        pid_planep[5] = (outbuf_strip_g_ps + (i*line_len)); 

        pip_generate_rows(pid_planep,inc_lines,&combined_stats,chan_stats,1, 1, rows_generated);
    }

    zx_idma_close();
    pip_close_page();

#ifdef REPORT_CHECKSUM
    for (i = 0; i < 6; i++) {
        dma_buffer_unmap_single(bb[i], DMA_FROM_DEVICE );
    }
    outbuf_strip_c_ps = dma_buffer_mmap_forcpu( bb[0] );
    outbuf_strip_m_ps = dma_buffer_mmap_forcpu( bb[1] );
    outbuf_strip_y_ps = dma_buffer_mmap_forcpu( bb[2] );
    outbuf_strip_k_ps = dma_buffer_mmap_forcpu( bb[3] );
    outbuf_strip_p_ps = dma_buffer_mmap_forcpu( bb[4] );
    outbuf_strip_g_ps = dma_buffer_mmap_forcpu( bb[5] );

    if(pip_cfg->raster_odma_cfg[0] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_c_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[1] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_m_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[2] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_y_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[3] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_k_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[4] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_p_ps, line_len * rows);
    if(pip_cfg->raster_odma_cfg[5] != NULL)
        dchecksum += checksum((uint8_t *) outbuf_strip_g_ps, line_len * rows);

    cmd_printf( "pip_strip_driver - data checksum = %u\n",dchecksum); 

#ifdef DEBUG_OVER_CIFS
    pid_planep[0] = outbuf_strip_c_ps; 
    pid_planep[1] = outbuf_strip_m_ps;
    pid_planep[2] = outbuf_strip_y_ps;
    pid_planep[3] = outbuf_strip_k_ps;
    pid_planep[4] = outbuf_strip_p_ps;
    pid_planep[5] = outbuf_strip_g_ps;

    pip_test_debug_data_write_raster_line(line_len*rows, DEBUG_OVER_CIFS_NUM_PLANES, pid_planep);

    pip_test_close_debug_data_output(DEBUG_OVER_CIFS_NUM_PLANES);
#endif
#endif
    for (i = 0; i < 6; i++) {
        dma_buffer_unmmap_forcpu( bb[i] );
        dma_buffer_free( bb[i] );
    }
    end_time = posix_gettime_ms(); // tx_time_get();
    cmd_printf( "pip_strip_driver - time (ms): %u\n", (end_time-start_time));

    return(1);
}
#endif

#ifdef HAVE_CMD
/**
 * \brief Test the mono strip processing service.
 *
 * \retval none.
 *
 */
static void pip_process_mono_strip_test(void)
{
#if 0 // fixme
    BigBuffer_t *bb_in;
    BigBuffer_t *bb_out;
    uint8_t *data_in;
    uint8_t *data_out = 0;
    uint32_t width_pix_in;
    uint32_t lines_in;
    uint32_t data_out_size;
    uint32_t outbuf_size;
    uint32_t dchecksum = 0;
    uint32_t mode;
#ifdef DEBUG_OVER_CIFS
    uint32_t *debug_output[1];
#endif

    if (0 == sizeof(bvtest_mono8))
    {
        cmd_printf("pip_process_mono_strip_test - No image data available, abort test.\n");
        return;
    }

    mode = IPC_PRINT_MONO8_DETAIL;
    pip_set_mode(mode);
    mlimiter_start( mlimiter_by_name("print") );
    data_in = (uint8_t *) (bvtest_mono8+32);  // Skip past sbin header
    bb_in = dma_buffer_adopt(data_in, sizeof(bvtest_mono8) - 32);
    width_pix_in = *((uint32_t *)(bvtest_mono8+24));
    lines_in = *((uint32_t *)(bvtest_mono8+28));

    data_out_size = ((pip_cfg->bpp*width_pix_in)/8)*lines_in;
    outbuf_size = PAD32(data_out_size);
 
    bb_out = dma_buffer_malloc(mlimiter_by_name("print"), outbuf_size);

    cmd_printf("pip_process_mono_strip_test - width:%u height:%u\n", width_pix_in, lines_in);

#ifdef DEBUG_OVER_CIFS
    pip_test_open_debug_data_output(1);
#endif

    pip_process_mono_strip(mode,
                           bb_in,
                           width_pix_in,
                           lines_in,
                           lines_in,
                           lines_in,
                           1,
                           1, 
                           true,
                           true,
                           bb_out );
 
    if(data_out)
    {
        dchecksum = checksum((uint8_t *)data_out, data_out_size);
    }

    cmd_printf("pip_process_mono_strip_test - data checksum = %u\n",dchecksum);

#ifdef DEBUG_OVER_CIFS
    debug_output[0] = (uint32_t *) dma_buffer_mmap_forcpu( bb_out ); //data_out;

    pip_test_debug_data_write_raster_line(data_out_size, 1, debug_output);

    pip_test_close_debug_data_output(1);
#endif

    BigBuffer_Free( bb_out );
    BigBuffer_Free( bb_in ); 
#endif
}





/**
 * \brief Test the mono strip processing service.
 *
 * \retval none.
 *
 */
static void pip_routing_8x8_test(void)
{
    BigBuffer_t *bb_in;
    BigBuffer_t *bb_out;
    uint8_t *data_in;
    uint8_t *data_out;
    uint32_t width_pix_in;
    uint32_t lines_in;
    uint32_t data_out_size;
    uint32_t outbuf_size;
    //uint32_t dchecksum;
    uint32_t mode;
#ifdef DEBUG_OVER_CIFS
    uint32_t *debug_output[1];
#endif

    if (0 == sizeof(bvtest_mono8))
    {
        cmd_printf("pip_process_mono_strip_test - No image data available, abort test.\n");
        return;
    }

    mode = IPC_PRINT_MONO8_DETAIL;
    pip_set_mode(mode);
    mlimiter_start( mlimiter_by_name("print") );
    data_in = (uint8_t *) (bvtest_mono8+32);  // Skip past sbin header
    bb_in = dma_buffer_adopt(data_in, sizeof(bvtest_mono8) - 32);
    width_pix_in = *((uint32_t *)(bvtest_mono8+24));
    lines_in = *((uint32_t *)(bvtest_mono8+28));

    data_out_size = ((pip_cfg->bpp*width_pix_in))*lines_in;
    outbuf_size = PAD32(data_out_size);
    cmd_printf("pip_routing_8x8_test - width:%u height:%u\n", width_pix_in, lines_in);
 
    bb_out = dma_buffer_malloc(mlimiter_by_name("print"), outbuf_size);

    data_in = dma_buffer_map_single(bb_in, DMA_TO_DEVICE);
    data_out = dma_buffer_map_single( bb_out, DMA_FROM_DEVICE );

    pip_setupregs();

    pip_reg_u->CFG = 0x01; // softreset
    pip_reg_u->CFG = (1<<30) | 0x02 | ( 1 << 5); // pip interrupt enable, lpc sideband
    pip_reg_u->DMA_CFG = (1 << 24) | (1 << 9);  // KTti output to dma 4

    void *idesc_hw;
    // configure idma 
    lpc_idma_descriptor_t *idesc_v = (lpc_idma_descriptor_t*) MEM_MALLOC_UNCACHED(&idesc_hw, sizeof(lpc_idma_descriptor_t), 32 );
    idesc_v->config = 0x00010000; // enable, 4 word  
    idesc_v->src_hw = (uint32_t)data_in;
    idesc_v->len = bb_in->datalen;
    idesc_v->next_hw = 0;
    pip_lpc_idma3_reg->line_width = width_pix_in;
    pip_lpc_idma3_reg->int_cl = 0x3;
    pip_lpc_idma3_reg->int_en = 0x3;
    pip_lpc_idma3_reg->cfg = 0x00010300;
    pip_lpc_idma3_reg->desc_write = (uint32_t)idesc_hw;  // 

    // configure odma
#if 1
    static odma_handle_t *odma_handle;
    static dma_config_data_t user_data;
    int ret = raster_odma_open(&odma_handle, 4);
    XASSERT(OK == ret, ret);
    ret = raster_odma_register_notifcation_callback(odma_handle, 0);
    XASSERT(OK == ret, ret);
    ret = raster_odma_input_width_in_bpp(odma_handle, 8);
    XASSERT(OK == ret, ret);
    ret = raster_odma_set_line_width_in_bytes(odma_handle, width_pix_in);
    XASSERT(OK == ret, ret);

    ret = raster_odma_set_cfg(odma_handle, &raster_odma_8_bpp_cfg);
    XASSERT(OK == ret, ret);
    ret = raster_odma_add_input_buffer(odma_handle,
                                       (char *)data_out,
                                       width_pix_in,
                                       lines_in,
                                       true, // SOI
                                       true,  // EOI
                                       true,  // stop_on_finish
                                       &user_data);
    XASSERT(OK == ret, ret);

    ret = raster_odma_start_xfer(odma_handle);
    XASSERT(OK == ret, ret);

    raster_odma_dump_regs( );
    // Wait for PA ODMA to complete
    ODMA_WAIT( odma_handle );
#endif
    BigBuffer_Free( bb_out );
    BigBuffer_Free( bb_in ); 
}
#endif

#ifdef HAVE_CMD
/*----------------------------------------------------------------------
 * pip_pastore_driver() - Test routine to drive data through the
 *   PIP pipeline but output to the PIPE_ACCESS store function.
 *
 *----------------------------------------------------------------------
 */

#define PID_WIDTH_BYTES (*((uint32_t *) (pstestrgb32_bin+24)))
#define PID_LINES       (*((uint32_t *) (pstestrgb32_bin+28)))
#define REPORT_CHECKSUM 1

#define NUM_COLOR_PLANES 6
uint32_t *pid_planep[NUM_COLOR_PLANES];

#define PSCALE_X pipscale_x
#define PSCALE_Y pipscale_y
#define PMODE  pipmode
#define PPAD   pippad

#ifdef INCLUDE_PIPTEST
#define MAX_OUT_LENGTH_PAST 288*8*563
#else
#define MAX_OUT_LENGTH_PAST 1
#endif

unsigned char __attribute__ ((aligned(256))) outbuf_past[MAX_OUT_LENGTH_PAST];

static int pip_pastore_driver(uint32_t nchans)
{
    uint32_t zx_data;
    uint32_t inc_lines, width_pix_in, width_pix_out, width_pix_pa;
    int i;
    uint32_t *outp;
    uint32_t chancount;
    raster_odma_cfg_t *pa_odma_cfg_saved;
    pipe_access_cfg_t *pa_cfg_saved;
    stat_cfg_t *stat_cfg_saved;
#ifdef REPORT_CHECKSUM
    uint32_t dchecksum = 0;
#endif

    if (0==sizeof(pstestrgb32_bin) || 1==MAX_OUT_LENGTH_PAST)
    {
        cmd_printf( "pip_past_driver - No image data available, abort past test.\n");
        return 0;
    }

    chancount = nchans;

    ASSERT((chancount == 6) || (chancount == 4));

    // Save current PIPE_ACCESS, PA_DMA, and PIP_STAT config structures
    pa_cfg_saved = pip_cfg->pipe_access_cfg;
    pa_odma_cfg_saved = pip_cfg->pa_odma_cfg;
    stat_cfg_saved = pip_cfg->stat_cfg;

    // Set PIPE_ACCESS and PA_DMA config structures to do a store
    if (6 == chancount)
    {
        pip_cfg->pipe_access_cfg = &pipe_access_store_6chan_cfg;
    }
    else if (4 == chancount)
    {
        pip_cfg->pipe_access_cfg = &pipe_access_store_4chan_cfg;
    }
    else
    {
        // unrecognized channel count!
        ASSERT(0);
    }
    pip_cfg->pa_odma_cfg = &raster_odma_32_bpp_cfg;

    // Turn off pip stats for this test
    pip_cfg->stat_cfg = NULL;

    inc_lines = 1;  // Ouput scaled lines
    width_pix_in = PID_WIDTH_BYTES/4;

    pip_prepare_page(PMODE,
                     PSCALE_X,
                     PSCALE_Y,
                     width_pix_in,
                     width_pix_in,
                     PID_LINES,    // Input, non-scaled lines
                     inc_lines,    // Output, scale applied lines
                     PPAD,
                     piptextmerge,
                     pip_flip_horiz,
                     &width_pix_out);

    pip_notify_rows_available(PID_LINES, true);

    width_pix_pa = width_pix_out / PSCALE_X;

    zx_data = ((uint32_t) pstestrgb32_bin)+32;  // Skip past sbin header

    // Setup Zx to drive data through
    zx_setup_single(zx_reg, zx_desc_cmds, (uint32_t)zx_data,
                    width_pix_in, PID_LINES);
    //lsptodo: descriptor and data :cpu_dcache_writeback_region(zx_desc_cmds, sizeof(zx_desc_cmds));

    // Process nlines at a time
    zx_start(zx_reg, (uint32_t) &zx_desc_cmds[0]);

    // Setup output line buffers
    ASSERT((width_pix_pa*8*PID_LINES) <= MAX_OUT_LENGTH_PAST);
    outp = (uint32_t *) outbuf_past;

    // Store the rows out in DRAM
    for (i = 0; i < (PID_LINES/inc_lines); i++)
    {
        //lsptodo: data        cpu_dcache_invalidate_region(outp,inc_lines*width_pix_pa*8);

        pip_pastore_rows(outp,inc_lines,width_pix_pa,chancount);
#ifdef REPORT_CHECKSUM
        dchecksum += checksum((uint8_t *) outp,inc_lines*width_pix_pa*8);
#endif
        outp += inc_lines*width_pix_pa*2;    // only multiply by 2 because outp is a uint32_t ptr
    }
#ifdef REPORT_CHECKSUM
    cmd_printf( "pip_pastore_driver - data checksum = %u\n",dchecksum);
#endif

    pip_close_page();

    // Restore current PIPE_ACCESS, PA_ODMA, and PIP_STAT config structures
    pip_cfg->pipe_access_cfg = pa_cfg_saved;
    pip_cfg->pa_odma_cfg = pa_odma_cfg_saved;
    pip_cfg->stat_cfg = stat_cfg_saved;

    return(1);
}
#endif

#ifdef HAVE_CMD
/*----------------------------------------------------------------------
 * pip_paload_driver() - Test routine to drive data through the
 *   PIP pipeline while reading from memory to the PIPE_ACCESS block
 *
 *----------------------------------------------------------------------
 */

#define PID_WIDTH_BYTES (*((uint32_t *) (pstestrgb32_bin+24)))
#define PID_LINES       (*((uint32_t *) (pstestrgb32_bin+28)))
#define REPORT_CHECKSUM 1

#define NUM_COLOR_PLANES 6
uint32_t *pid_planep[NUM_COLOR_PLANES];

//#define PSCALE 2
#define PSCALE_X pipscale_x
#define PSCALE_Y pipscale_y
#define PMODE  pipmode
#define PPAD   pippad

#ifdef INCLUDE_PIPTEST
#define MAX_OUT_LENGTH_PALD 96         // cache line multiple
#else
#define MAX_OUT_LENGTH_PALD 1
#endif


void *outbuf_pald_c;
void *outbuf_pald_m;
void *outbuf_pald_y;
void *outbuf_pald_k;
void *outbuf_pald_p;
void *outbuf_pald_g;

static int pip_paload_driver(uint32_t nchans)
{
    uint32_t *inbuf;    // data to be loaded into the pipe;
    uint32_t inc_lines, width_pix_in, width_pix_out, width_pix_pa;
    uint32_t rows_generated;
    int i;
    int j;
    uint32_t chancount;
    uint32_t width_pixels;
    pa_idma_cfg_t *pa_idma_cfg_saved;
    pipe_access_cfg_t *pa_cfg_saved;
#ifdef REPORT_CHECKSUM
    uint32_t dchecksum = 0;
#endif
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    BigBuffer_t *bb[6];

    if (0 == sizeof(pstestrgb32_bin))
    {
        cmd_printf( "pip_pald_driver - No image data available, abort pald test.\n");
        return 0;
    }

    chancount = nchans;

    ASSERT((chancount == 6) || (chancount == 4));

    // Save current PIPE_ACCESS and PA_DMA config structures
    pa_cfg_saved = pip_cfg->pipe_access_cfg;
    pa_idma_cfg_saved = pip_cfg->pa_idma_cfg;

    // Set PIPE_ACCESS and PA_DMA config structures to do a load
    if (6 == chancount)
    {
        pip_cfg->pipe_access_cfg = &pipe_access_load_6chan_cfg;
//        width_pixels = PID_WIDTH_BYTES/8;
        width_pixels = PID_WIDTH_BYTES/4;
    }
    else if (4 == chancount)
    {
        pip_cfg->pipe_access_cfg = &pipe_access_load_4chan_cfg;
        width_pixels = PID_WIDTH_BYTES/4;
    }
    else
    {
        // unrecognized channel count!
        ASSERT(0);
    }
    pip_cfg->pa_idma_cfg = &pa_idma_load_cfg;

    //  inc_lines = PID_LINES;  // Ouput scaled lines
    inc_lines = 1;  // Ouput scaled lines
    width_pix_in = PID_WIDTH_BYTES/4;

    pip_prepare_page(PMODE,
                     PSCALE_X,
                     PSCALE_Y,
                     width_pix_in,
                     width_pix_in,
                     PID_LINES,    // Input, non-scaled lines
                     inc_lines,    // Output, scale applied lines
                     PPAD,
                     piptextmerge,
                     pip_flip_horiz,
                     &width_pix_out);

    pip_notify_rows_available(PID_LINES, true);

    width_pix_pa = width_pix_out / PSCALE_X;

    inbuf = (uint32_t *) outbuf_past;    // Output of PA store operation

    // Setup output line buffers
    // Setup output line buffers
    mlimiter_t *lim = mlimiter_by_name("print");
    for (i = 0; i < 6; i++) {
        bb[i] = dma_buffer_malloc(lim, MAX_OUT_LENGTH_PALD );
    }

    // Load the data and run it through the pipe
    pip_paload_rows(inbuf,PID_LINES,width_pix_pa,chancount);
    for (i = 0; i < ((PID_LINES*PSCALE_Y)/inc_lines); i++)
    {
        for (j=0; j < 6; j++) {
            dma_buffer_map_single(bb[i], DMA_FROM_DEVICE);
        }
        pip_generate_rows(pid_planep,inc_lines,&combined_stats,chan_stats, 1, 1, rows_generated);
        for (j=0; j < 6; j++) {
            pid_planep[i] = dma_buffer_unmap_single( bb[0], DMA_FROM_DEVICE );
        }
#ifdef REPORT_CHECKSUM
        for (j=0; j < 6; j++) {
            void* temp = dma_buffer_mmap_forcpu( bb[i] );
            dchecksum += checksum((uint8_t *) temp,inc_lines*width_pix_out/4);
            dma_buffer_unmmap_forcpu( bb[i] );
        }
#endif
    }
    for (j=0; j < 6; j++) {
        dma_buffer_free( bb[i] );
    }
#ifdef REPORT_CHECKSUM
    cmd_printf("pip_paload_driver - data checksum = %u\n",dchecksum);
#endif

    // Restore current PIPE_ACCESS and PA_DMA config structures
    pip_cfg->pipe_access_cfg = pa_cfg_saved;
    pip_cfg->pa_idma_cfg = pa_idma_cfg_saved;

    pip_close_page();

    return(1);
}
#endif

#ifdef HAVE_CMD
/*----------------------------------------------------------------------
 *   Test driver routine to exercise the PIP HW block for mono modes
 *   Will process a passed in image (image is read out of absolute memory
 *   bufs). Invoked via ser dbg console "pip mono" cmd.
 *----------------------------------------------------------------------
 */
#define PIDM_WIDTH_BYTES (*((uint32_t *) (bvtest_mono8+24)))
#define PIDM_LINES       (*((uint32_t *) (bvtest_mono8+28)))
#define REPORT_CHECKSUM 1

#define NUM_COLOR_PLANES 6
uint32_t *pid_planep[NUM_COLOR_PLANES];

//#define PSCALE 2
#define PSCALE_X pipscale_x
#define PSCALE_Y pipscale_y
#define PMODE  pipmode
#define PPAD   pippad

#define MAX_OUT_LENGTH_MONO 4096
void *outbuf_mono_cmykpg[6];
/*
unsigned char __attribute__ ((aligned(256))) outbuf_mono_c[MAX_OUT_LENGTH_MONO];
unsigned char __attribute__ ((aligned(256))) outbuf_mono_m[MAX_OUT_LENGTH_MONO];
unsigned char __attribute__ ((aligned(256))) outbuf_mono_y[MAX_OUT_LENGTH_MONO];
unsigned char __attribute__ ((aligned(256))) outbuf_mono_k[MAX_OUT_LENGTH_MONO];
unsigned char __attribute__ ((aligned(256))) outbuf_mono_p[MAX_OUT_LENGTH_MONO];
unsigned char __attribute__ ((aligned(256))) outbuf_mono_g[MAX_OUT_LENGTH_MONO];
*/
//unsigned char __attribute__ ((aligned(256))) mono_outbuf[128*1024];
unsigned char mono_outbuf[128*1024]; //cpu only
unsigned char *saved;

static int pip_mono_driver(void)
{
    uint32_t zx_data;
    uint32_t width_pix_in, width_pix_out;
    uint32_t inc_lines, line_len;
    uint32_t i;
    uint32_t j;
#ifdef REPORT_CHECKSUM
    uint32_t dchecksum;
#endif
    uint32_t rows_generated;
    pip_stats_entry_t combined_stats;
    pip_stats_entry_t chan_stats[PIP_MAX_OUTPUT_CHANS];
    BigBuffer_t *bb[6];

    if (0 == sizeof(bvtest_mono8))
    {
        cmd_printf( "pip_mono_driver - No image data available, abort mono test.\n" );
        return 0;
    }

    inc_lines = 1;  // Ouput scaled lines
    width_pix_in = PIDM_WIDTH_BYTES;

    // Setup output line buffers
    ASSERT((width_pix_out*4) < MAX_OUT_LENGTH_MONO);
    mlimiter_t *lim = mlimiter_by_name("print");
    mlimiter_start( lim );
    for (i = 0; i < 6; i++) {
        bb[i] = dma_buffer_malloc(lim, MAX_OUT_LENGTH_MONO );
        outbuf_mono_cmykpg[i] = dma_buffer_mmap_forcpu( bb[i] );
        memset(outbuf_mono_cmykpg[i], 0x0A5, MAX_OUT_LENGTH_MONO );
        pid_planep[i] = dma_buffer_map_single( bb[i], DMA_FROM_DEVICE );
    }


    memset(mono_outbuf, 0x55, sizeof(mono_outbuf));

    pip_prepare_page(PMODE,
                     PSCALE_X,
                     PSCALE_Y,
                     PIDM_WIDTH_BYTES,
                     PIDM_WIDTH_BYTES,
                     PIDM_LINES,    // Input, non-scaled lines
                     inc_lines,     // Output, scale applied lines
                     PPAD,
                     piptextmerge,
                     pip_flip_horiz,
                     &width_pix_out);

    pip_notify_rows_available(PIDM_LINES, true);

    zx_data = ((uint32_t) bvtest_mono8)+32;  // Skip past sbin header

    // Setup Zx to drive data through
    zx_setup_mono(zx_reg, zx_desc_cmds, 0/* lsptodo */, (uint32_t)zx_data,
                  width_pix_in, PIDM_LINES, 0);
    //lsptodo: descriptor cpu_dcache_writeback_region(zx_desc_cmds, sizeof(zx_desc_cmds));

    // Process nlines at a time
    zx_start(zx_reg, (uint32_t) &zx_desc_cmds[0]);


    line_len = pip_cfg->bpp * width_pix_out/8;

    // The following for loop is for demonstration purposes - each
    //   strip overwrites the previous strip each time through the
    //   loop.
    //
    // In real code, you would want to call pip_generate_rows(), and
    //   then some function to process the generated rows before you
    //   overwrote the data the next time through the loop.
#ifdef REPORT_CHECKSUM
    dchecksum = 0L;
#endif
    saved = mono_outbuf;

    for (i = 0; i < ((PIDM_LINES*PSCALE_Y)/inc_lines); i++)
    {
        for (j=0; j < 6; j++) {
            pid_planep[i] = dma_buffer_map_single(bb[i], DMA_FROM_DEVICE);
        }
	rows_generated += inc_lines;
        pip_generate_rows(pid_planep,inc_lines,&combined_stats,chan_stats,1,1, rows_generated);
        for (j=0; j < 6; j++) {
            dma_buffer_unmap_single(bb[i], DMA_FROM_DEVICE);
        }
        dma_buffer_mmap_forcpu(bb[i]);
        memcpy(saved,pid_planep[1],line_len);
        saved+=line_len;
        dma_buffer_unmmap_forcpu(bb[1]);

#ifdef REPORT_CHECKSUM
        for (j=0; j < 6; j++) {
            void *temp = dma_buffer_mmap_forcpu(bb[i]);
            dchecksum += checksum((uint8_t *) temp,line_len);
            dma_buffer_unmmap_forcpu(bb[i]);
        }
#endif

        cmd_printf( "i: %u pip_generate_rows(): smin = %u smax = %u\n",
                    i,
                    combined_stats.smin,
                    combined_stats.smax );
    }
    for (j=0; j < 6; j++) {
        BigBuffer_Free( bb[i] );
    }
    pip_close_page();

#ifdef REPORT_CHECKSUM
    cmd_printf( "pip_mono_driver - data checksum = %u\n", dchecksum);
#endif

    return 0;
}
#endif

#ifdef HAVE_CMD
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef HAVE_ZLIB_PIP
#include <zlib.h>

#define CHUNK 16384
int pip_ztest_inf(FILE *source, FILE *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    static unsigned char in[CHUNK];
    static unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            ASSERT(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

static int pip_zlib_test(char *filename_in, char *filename_out)
{
    FILE *file_in;
    FILE *file_out;
    int fd;
    struct stat fd_stats;
    uint32_t size;
    uint32_t start_time;
    uint32_t end_time; 

    cmd_printf("Starting zlib test...\n");

    file_in = fopen(filename_in, "r+");
    if (NULL == file_in)
    { 
        cmd_printf("Failed to open input file.\n");
        return FAIL;
    }

    file_out = fopen(filename_out, "w+");
    if (NULL == file_out)
    {
        cmd_printf("Failed to open output file.\n");
        return FAIL;
    }

    fd = fileno(file_in);
    fstat(fd, &fd_stats);
    size = fd_stats.st_size;
    cmd_printf("input file size %u bytes\n", size);

    start_time = tx_time_get();
    pip_ztest_inf(file_in, file_out);
    end_time = tx_time_get();
    cmd_printf("Total zlib inflate time = %u ms\n", (end_time - start_time)*10);

    fd = fileno(file_out);
    fstat(fd, &fd_stats);
    size = fd_stats.st_size;
    cmd_printf("output file size %u bytes\n", size);

    fclose(file_in);
    fclose(file_out);

    cmd_printf("...zlib test finished.\n");

    return 0;
}
#endif
#endif


#ifdef HAVE_CMD
static const char *pip_cmd_wdavbuf_desc = "write value to address";
static const char *pip_cmd_wdavbuf_usage = "<addr> <value>";
static const char *pip_cmd_wdavbuf_notes = NULL;
static int pip_cmd_wdavbuf_cb(int argc, char *argv[])
{
    error_type_t etype;

    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }
    etype = pip_davbuf_write(atoi(argv[1]), atoi(argv[2]));
    if (FAIL == etype)
    {
         return CMD_ERROR;
    }

    return CMD_OK;
}

static const char *pip_cmd_rdavbuf_desc = "read from specified address";
static const char *pip_cmd_rdavbuf_usage = "<addr>";
static const char *pip_cmd_rdavbuf_notes = NULL;
static int pip_cmd_rdavbuf_cb(int argc, char *argv[])
{
    error_type_t etype;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    etype = pip_davbuf_read(atoi(argv[1]));
    if (FAIL == etype)
    {
         return CMD_ERROR;
    }

    return CMD_OK;
}

static const char *pip_cmd_zdavbuf_desc = "write specified number of zeros";
static const char *pip_cmd_zdavbuf_usage = "<num_entries>";
static const char *pip_cmd_zdavbuf_notes = NULL;
static int pip_cmd_zdavbuf_cb(int argc, char *argv[])
{
    error_type_t etype;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    etype = pip_davbuf_zero(atoi(argv[1]));
    if (FAIL == etype)
    {
         return CMD_ERROR;
    }

    return CMD_OK;
}

static const char *pip_cmd_scale_desc = "get or set the pip scale";
static const char *pip_cmd_scale_usage = "[<scale_x>] [<scale_y>]";
static const char *pip_cmd_scale_notes = "if no parameters are given the current scale will be given.  If only <scale_x> is given it will be used for by x and y scale";
static int pip_cmd_scale_cb(int argc, char *argv[])
{
    if ( ( argc != 1 ) && ( argc != 2 ) && ( argc != 3 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        pip_set_scale(atoi(argv[1]), atoi(argv[1]));
    }

    if (3 == argc)
    {
        pip_set_scale(atoi(argv[1]), atoi(argv[2]));
    }

    cmd_printf( "current pip scale_x is %u\n", pipscale_x );
    cmd_printf( "            scale_y is %u\n", pipscale_y );

    return CMD_OK;
}

static const char *pip_cmd_pad_desc = "get or set the pip pad";
static const char *pip_cmd_pad_usage = "[<value>]";
static const char *pip_cmd_pad_notes = "if <value> is not specified the current value will be displayed";
static int pip_cmd_pad_cb(int argc, char *argv[])
{
    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        pip_set_pad(atoi(argv[1]));
    }

    cmd_printf( "current pip pad is %d\n", pippad );

    return CMD_OK;
}

static const char *pip_cmd_mode_desc = "get or set the pip mode";
static const char *pip_cmd_mode_usage = "[<mode>]";
static const char *pip_cmd_mode_notes = "if <mode> is not specified the current value will be displayed";
static int pip_cmd_mode_cb(int argc, char *argv[])
{
    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        pip_set_mode(atoi(argv[1]));
    }

    cmd_printf( "current pip mode is %u which is %s\n", pipmode, pip_mode_name_get(pipmode) );

    return CMD_OK;
}

static const char *pip_cmd_textmerge_desc = "get or set the textmerge enable";
static const char *pip_cmd_textmerge_usage = "[1|0]";
static const char *pip_cmd_textmerge_notes = "if either 1 or 0 is not specified the current value will be displayed";
static int pip_cmd_textmerge_cb(int argc, char *argv[])
{
    int value;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        value = atoi( argv[1] );
        if ( ( value != 0 ) && ( value != 1 ) )
        {
            return CMD_USAGE_ERROR;
        }
        pip_set_textmerge( value );
    }

    cmd_printf( "current pip textmerge is %d\n", piptextmerge );

    return CMD_OK;
}

static const char *pip_cmd_sclut_desc = "dump pip info";
static const char *pip_cmd_sclut_usage = NULL;
static const char *pip_cmd_sclut_notes = NULL;
static int pip_cmd_sclut_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }
    pip_info();

    return CMD_OK;
}

static const char *pip_cmd_fscl_desc = "get or set the fscale";
static const char *pip_cmd_fscl_usage = "[<scale>]";
static const char *pip_cmd_fscl_notes = "if <scale> is not specified the current value will be displayed.  <scale> must be greater than or equal to 100";
static int pip_cmd_fscl_cb(int argc, char *argv[])
{
    int value;
    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        value = atoi( argv[1] );
        if ( value < 100 )
        {
            return CMD_USAGE_ERROR;
        }
        pip_set_fscale( value );
    }

    cmd_printf( "pipfscale = %u which gives %u%% upscale\n", pipfscale, pipfscale);

    return CMD_OK;
}

static const char *pip_cmd_hflip_desc = "get or set the horizontal flip enable";
static const char *pip_cmd_hflip_usage = "[1|0]";
static const char *pip_cmd_hflip_notes = "if either 1 or 0 is not specified the current value will be displayed";
static int pip_cmd_hflip_cb(int argc, char *argv[])
{
    int value;
    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (2 == argc)
    {
        value = atoi( argv[1] );
        if ( ( value != 0 ) && ( value != 1 ) )
        {
            return CMD_USAGE_ERROR;
        }
        pip_set_hflip( value );
    }

    cmd_printf( "current hflip is %d\n", pip_flip_horiz );

    return CMD_OK;
}

static const char *pip_cmd_smart_desc = "run the smartp test";
static const char *pip_cmd_smart_usage = NULL;
static const char *pip_cmd_smart_notes = NULL;
static int pip_cmd_smart_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_smartp_driver();

    return CMD_OK;
}

static const char *pip_cmd_strip_desc = "run the strip test";
static const char *pip_cmd_strip_usage = NULL;
static const char *pip_cmd_strip_notes = NULL;
static int pip_cmd_strip_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_strip_driver();

    return CMD_OK;
}

static const char *pip_cmd_past_desc = "run the pastore test with the specified number of channels";
static const char *pip_cmd_past_usage = "[<num channels>=6]";
static const char *pip_cmd_past_notes = "Only values 4 and 6 are allowable, if no values are given, 6 will be used by default.";
static int pip_cmd_past_cb(int argc, char *argv[])
{
    int value;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (1 == argc)
    {
        pip_pastore_driver(6);  // default to 6 chans
    }
    else
    {
        value = atoi( argv[1] );
        if ( ( value != 4 ) && ( value != 6 ) )
        {
            return CMD_USAGE_ERROR;
        }
        pip_pastore_driver(atoi(argv[1]));
    }

    return CMD_OK;
}

static const char *pip_cmd_pald_desc = "run the paload test with specified number of channels";
static const char *pip_cmd_pald_usage = "[<num channels>=6]";
static const char *pip_cmd_pald_notes = "Only values 4 and 6 are allowable, if no values are given, 6 will be used by default.";
static int pip_cmd_pald_cb(int argc, char *argv[])
{
    int value;

    if ( ( argc != 1 ) && ( argc != 2 ) )
    {
        return CMD_USAGE_ERROR;
    }

    if (1 == argc)
    {
        pip_paload_driver(6);  // default to 6 chans
    }
    else
    {
        value = atoi( argv[1] );
        if ( ( value != 4 ) && ( value != 6 ) )
        {
            return CMD_USAGE_ERROR;
        }
        pip_paload_driver( value );
    }

    return CMD_OK;
}


static const char *pip_cmd_mono_desc = "run the mono driver";
static const char *pip_cmd_mono_usage = NULL;
static const char *pip_cmd_mono_notes = NULL;
static int pip_cmd_mono_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_mono_driver();

    return CMD_OK;
}

static const char *pip_cmd_mstrip_desc = "run the mono strip test";
static const char *pip_cmd_mstrip_usage = NULL;
static const char *pip_cmd_mstrip_notes = NULL;
static int pip_cmd_mstrip_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_process_mono_strip_test();

    return CMD_OK;
}

static const char *pip_cmd_routing_8x8_desc = "run the routing 8in 8out test";
static const char *pip_cmd_routing_8x8_usage = NULL;
static const char *pip_cmd_routing_8x8_notes = NULL;
static int pip_cmd_routing_8x8_cb(int argc, char *argv[])
{
    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_routing_8x8_test();

    return CMD_OK;
}

#ifdef HAVE_ZLIB_PIP
static const char *pip_cmd_ztest_desc = "run the mono strip test";
static const char *pip_cmd_ztest_usage = NULL;
static const char *pip_cmd_ztest_notes = NULL;
static int pip_cmd_ztest_cb(int argc, char *argv[])
{
    if ( argc != 3 )
    {
        return CMD_USAGE_ERROR;
    }

    pip_zlib_test(argv[1], argv[2]);

    return CMD_OK;
}
#endif
/**
 *==============================================================================
 * \brief Simple checksum over specified number of bytes at a specified address.
 *
 * \param dptr pointer to buffer to calculate checksum over
 *
 * \param nbytes number of bytes to add to checksum
 *
 * \retval 32 bit simple checksum of passed in buffer
 *==============================================================================
 */

static uint32_t checksum(uint8_t *dptr, uint32_t nbytes)
{
    int i;
    uint32_t retval;

    for (retval = 0, i = 0; i < nbytes; i++)
        retval += *dptr++;

    return(retval);
}


#ifdef DEBUG_OVER_CIFS

#define DEBUG_OVER_CIFS_SHARE_NAME "shared"
#define DEBUG_OVER_CIFS_FILE_PATH_MAX_LEN 100
#define DEBUG_OVER_CIFS_SHARE_NAME_MAX_LEN 80

static FILE *_file[DEBUG_OVER_CIFS_NUM_PLANES];

static void pip_test_open_debug_data_output(uint32_t num_planes)
{
    unsigned int i;
    char file_path[DEBUG_OVER_CIFS_FILE_PATH_MAX_LEN];

    XASSERT(strlen(DEBUG_OVER_CIFS_SHARE_NAME) < DEBUG_OVER_CIFS_SHARE_NAME_MAX_LEN, strlen(DEBUG_OVER_CIFS_SHARE_NAME));

    for (i=0; i<num_planes; i++)
    {
        snprintf(file_path, DEBUG_OVER_CIFS_FILE_PATH_MAX_LEN, "/%s/pip_test_out_%u.bin", DEBUG_OVER_CIFS_SHARE_NAME, i);
        _file[i] = fopen(file_path, "w+");
        ASSERT(NULL != _file[i]);
    }
}

static void pip_test_debug_data_write_raster_line(uint32_t row_stride, uint32_t num_planes, uint32_t *flocs[])
{
    uint32_t ret;
    uint32_t i;

    for (i=0; i<num_planes; i++)
    {
        ret = fwrite(flocs[i], 1, row_stride, _file[i]);
        XASSERT(row_stride == ret, ret);
    }
}

static void pip_test_close_debug_data_output(uint32_t num_planes)
{
    uint32_t ret;
    uint32_t i;

    for (i=0; i<num_planes; i++)
    {
        ret = fclose(_file[i]);
        XASSERT(0==ret, ret);
    }
}

#endif // DEBUG_OVER_CIFS
#endif // HAVE_CMD

