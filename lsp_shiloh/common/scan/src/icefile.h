/**
 * \file icefile.h
 *
 * \brief header file for icefile.c
 *
 * Icefile is a debug tool that reads copy data from USB. The data is read
 * through the scan endpoint and injected into the scanpipe as if it came from
 * PIC.
 *
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef ICEFILE_H
#define ICEFILE_H

/* This is legacy from the days of PIC WDMA. Icetest has one and only one dma
 * channel. TODO remove.
 */
#define ICEFILE_NUM_CHANNELS  6

struct icefile_packet
{
    uint32_t cookie;
    uint32_t opcode; 
    uint32_t seqnum; /* USB has problems with packet replay */
    uint32_t flags;  /* future proofing */
    uint32_t length; /* header+data length, like UDP */
};

#define ICEFILE_PACKET_COOKIE 0x1b5b356d

#define ICEFILE_OP_OPEN  1
#define ICEFILE_OP_CLOSE 2
#define ICEFILE_OP_DATA  3
#define ICEFILE_OP_ERROR 4
#define ICEFILE_OP_ACK   5

struct icefile_open
{
    uint32_t num_channels;
    uint32_t channels[ICEFILE_NUM_CHANNELS];
};

struct icefile_data
{
    uint32_t channel;
    uint32_t num_rows;
    uint32_t bytes_per_row;
    uint32_t datalen_bytes;
};

/** size info of icebuf data sent by icefile to the scanpipe */
struct icefile_image_info 
{
    uint32_t total_rows;
    uint32_t pixels_per_row;
    uint32_t rows_per_buffer;
};

/* TODO */
//void icefile_cancel_msg( void );

/* general icefile functions */
scan_err_t icefile_set_image_info( struct icefile_image_info *image_info ); 
scan_err_t icefile_get_image_info( struct icefile_image_info *image_info );

#endif

