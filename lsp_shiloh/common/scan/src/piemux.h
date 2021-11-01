/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file piemux.c 
 *
 * \brief Multiplex PIE (multiple separate images through PIE simultaneously)
 *
 * davep 23-May-2013
 *
 * Originally created to handle nsensor support (sensors on both sides of page)
 *
 */

#ifndef PIEMUX_H
#define PIEMUX_H

void piemux_onetime_init( void );
void piemux_dump( void );
void piemux_cancel( void );
scan_err_t piemux_open( void );
void piemux_close( void );

void piemux_add_ready( uint8_t page_side, struct ice_dma_buffer **p_icebuf );

scan_err_t piemux_scheduler( void );
void piemux_first_schedule( void );

void piemux_rdma_intmsg( uint8_t sensor_num, uint8_t pie_channel );
void piemux_wdma_intmsg( uint8_t sensor_num, uint8_t pie_channel );

#endif

