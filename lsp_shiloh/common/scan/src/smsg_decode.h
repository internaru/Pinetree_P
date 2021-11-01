/*
 * ============================================================================
 * Copyright (c) 2013 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/* 
 * Use these functions for decoding scan_msg_t messages. Do NOT use these
 * functions for decoding agMessage.h MESSAGE structures. (Use scanmsg.h
 * instead.)
 *
 * davep 23-May-2013 ; moved from scancore.h to own file
 */

#ifndef SMSG_DECODE_H
#define SMSG_DECODE_H

/* Decode SMSG_SCAN_SIZE
 *
 * See scanlib_send_scan_size()-scanlib.c for encode.
 *
 * Do NOT use scanmsg.h to decode. The scanmsg.h is public for decoding
 * agMessage.h MESSAGE structures.
 */

static inline void scan_msg_size_decode( scan_msg_t *msg, uint32_t *pixels_per_row_padded, 
                                         uint32_t *pixels_per_row, uint32_t *total_rows, 
                                         uint32_t *bits_per_pixel )
{
    uint32_t n;

    *pixels_per_row_padded = msg->param1;
    *pixels_per_row = msg->param2;

    n = (uint32_t)msg->param3;
    *total_rows = SMSG_SIZE_GET_NUM_ROWS( n );
    *bits_per_pixel = SMSG_SIZE_GET_PIXEL_SIZE( n );
}

/* Decode SMSG_PAGE_DATA (scan's internal data payload message)
 *
 * See scanlib_send_page_data()-scanlib.c for encode.
 *
 * Do NOT use scanmsg.h to decode. The scanmsg.h is public for decoding
 * agMessage.h MESSAGE structures.
 */

static inline void scan_msg_page_data_decode( scan_msg_t *msg, 
                                              uint16_t *num_rows, scan_data_type *dtype, 
                                              uint8_t **data_ptr, bool *last_buffer )
{
    *num_rows = SMSG_DATA_GET_NUM_ROWS( msg->param1 );
    *dtype = SMSG_DATA_GET_DATATYPE( msg->param2 );

    *last_buffer = msg->param2 & SMSG_DATA_LAST_BUFFER_FLAG;

    *data_ptr = msg->param3;
}

/* Decode SMSG_PAGE_DATA page side field. Page side is used to indicate which
 * side of a page a buffer belongs to. Used with mechs that support sensors on
 * both sides of the paper. Buffers of either side of the page may arrive
 * interleaved.
 *
 * davep 04-Apr-2013 
 */
static inline void scan_msg_page_data_get_page_side( scan_msg_t *msg, uint8_t *page_side )
{
    *page_side = SMSG_DATA_GET_PAGE_SIDE(msg->param2);
}

/* Decode SMSG_PAGE_DATA maxrows field. The max_rows is used to indicate the size of
 * the attached data buffer. The num_rows will be <= max_rows. The num_rows
 * indicates the amount of actual image data in the buffer. 
 *
 * davep 15-May-2013 
 */
static inline void scan_msg_page_data_get_max_rows( scan_msg_t *msg, uint16_t *max_rows )
{
    *max_rows = SMSG_DATA_GET_MAX_ROWS(msg->param1);
}

/* Decode SMSG_DATA_BLOB. A data blob is an opaque data structure usually used
 * during cal-to-host. The host must know how to decode the blob.
 *
 * See cal_common.c for encode examples.
 *
 * Do NOT use scanmsg.h to decode. The scanmsg.h is public for decoding
 * agMessage.h MESSAGE structures.
 */

static inline void scan_msg_data_blob_decode( scan_msg_t *msg, 
                                              uint8_t **data_ptr, uint32_t *num_bytes )
{
    *num_bytes = msg->param1;
    *data_ptr = msg->param3;
}

static inline void scan_msg_page_start_decode( scan_msg_t *msg, uint8_t *page_side )
{
    *page_side = SMSG_PAGE_START_GET_PAGE_SIDE(msg->param1);
}

static inline void scan_msg_page_end_decode( scan_msg_t *msg, uint8_t *page_side )
{
    *page_side = SMSG_PAGE_END_GET_PAGE_SIDE(msg->param1);
}

#endif
