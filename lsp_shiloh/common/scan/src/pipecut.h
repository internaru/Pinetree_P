/**
 * \file pipecut.h
 *
 * \brief The pipe cutter cuts tof/bof in ADF scans.
 *
 */
/*
 * ============================================================================
 * Copyright (c) 2009 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PIPECUT_H
#define PIPECUT_H

#define MAX_PIPE_CUTTER_CHANNELS 6

#define MAX_PIPE_CUTTER_RUNNING_ROWS 300

typedef void (*scanpipe_recycler_callback)( uint8_t channel, struct ice_dma_buffer **pp_icebuf );

void pipe_cutter_onetime_init( void );

scan_err_t pipe_cutter_open( uint8_t page_side, uint8_t channels[], int num_channels) ;
void pipe_cutter_close( void );
void pipe_cutter_flush( void );

void pipe_cutter_set_recycler( scanpipe_recycler_callback fn );

void pipe_cutter_add_marker( uint32_t row_number );
void pipe_cutter_print_markers( void );

void pipe_cutter_add_ready( uint8_t page_side, struct ice_dma_buffer **pp_icebuf );

void pipe_cutter_get_num_channels( uint8_t page_side, int *num_channels );

void pipe_cutter_set_page_side_offset( uint8_t page_side, uint32_t offset );

void pipe_cutter_dump( void );

#endif

