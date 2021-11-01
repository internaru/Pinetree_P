/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file picdma_descrip.h
 *
 * \brief PIC Write DMA driver for chained descriptor based DMA hardware
 *
 * davep 28-Mar-2010 ; Added for new descriptor-based DMAs
 *
 */

#ifndef PICDMA_DESCRIP_H
#define PICDMA_DESCRIP_H

struct pic_cwdma_regs
{
    volatile uint32_t cfg;  ///< 0x0 [R/W]: Configuration register
    volatile uint32_t status;  ///< 0x4 [R]: Status register
    volatile uint32_t line_size;  ///< 0x8 [R/W]: Line Size Register
    volatile uint32_t int_en;  ///< 0xc [R/W]: Interrupt Enable Register
    volatile uint32_t int_pend;  ///< 0x10 [R]: Interrupt Pending Register
    volatile uint32_t int_ack;  ///< 0x14 [W]: Interrupt Acknowledge Register
    volatile uint32_t int_fo;  ///< 0x18 [W]: Interrupt Force Register
    volatile uint32_t desc_write;  ///< 0x1c [W]: Descriptor Write Register
    volatile uint32_t desc_read;  ///< 0x20 [R]: Descriptor Read Register
    volatile uint32_t xfer_length;  ///< 0x24 [R]: Descriptor Transfer Length Register
    volatile uint32_t xfer_addr;  ///< 0x28 [R]: Descriptor Transfer Address Register
    volatile uint32_t xfer_burst;  ///< 0x2c [R]: Descriptor Transfer Burst Register
    volatile uint32_t data_val;  ///< 0x30 [R]: Data Valid Register
    volatile uint32_t reset;  ///< 0x34 [W]: Soft Reset register
};

void pic_cwdma_set_config( uint32_t disable_mode );
struct pic_cwdma_regs * pic_cwdma_get_regs( uint8_t channel );
uint32_t pic_cwdma_status_get( uint8_t channel );

scan_err_t pic_cwdma_channel_open( uint8_t channel );
void pic_cwdma_channel_close( uint8_t channel );

void pic_cwdma_interrupt_stats_dump( void );

void pic_cwdma_late_launch( void );

#ifdef DDMA_H
void pic_ddma_desc_write( uint8_t channel, struct ddma_descriptor *desc, uint32_t line_width_bytes );
#endif

#endif

