/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PIPEMAP_H
#define PIPEMAP_H

/* PIC WDMA <---> PIE RDMA */
uint8_t pipemap_pic_to_pie_channel( struct ice_dma_buffer *icebuf );
uint8_t pipemap_pie_to_pic_channel( struct ice_dma_buffer *icebuf );

/* PIE WDMA <---> PIE-HP RDMA */
uint8_t pipemap_pie_to_piehp_channel( struct ice_dma_buffer *icebuf );
uint8_t pipemap_piehp_to_pie_channel( struct ice_dma_buffer *icebuf );

/* Get the PIC WDMA channels associated with a particular sensor */
void pipemap_get_pic_channels_for_sensor( uint32_t sensor_num, uint8_t channel_array[] );

#endif

