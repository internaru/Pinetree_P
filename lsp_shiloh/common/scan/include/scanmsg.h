/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International. Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Decode MSG_SCAN_xxx messages. 
 *
 * Do NOT use these functions to decode scan_msg_t messages. Use these
 * functions to decode agMessage.h MESSAGE structures. The scan_msg_t message
 * is internal to scan and can change indiscriminately. These functions are for
 * decoding scanman's public messaging protocol.
 *
 * davep 22-Apr-2011
 */

#ifndef SCANMSG_H
#define SCANMSG_H

/* MSG_SCAN_SIZE
 *
 * see SMSG_SCAN_SIZE-scantypes.h and scanlib_send_scan_size()-scanlib.c
 */

inline static void msg_scan_size_decode( MESSAGE *msg, uint32_t *pixels_per_row_padded, 
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

/* MSG_SCAN_PLANEDATA 
 *
 * see SMSG_SCAN_DATA-scantypes.h and scanlib_send_page_data()-scanlib.c
 */

inline static void msg_scan_planedata_decode( MESSAGE *msg, 
                                              uint16_t *num_rows, scan_data_type *dtype, 
                                              uint8_t **data_ptr, bool *last_buffer )
{

    *num_rows = SMSG_DATA_GET_NUM_ROWS( msg->param1 );
    *dtype = SMSG_DATA_GET_DATATYPE( msg->param2 );

    *last_buffer = msg->param2 & SMSG_DATA_LAST_BUFFER_FLAG;

    *data_ptr = msg->param3;
}

static inline void msg_scan_planedata_get_page_side( MESSAGE *msg, uint8_t *page_side )
{
    *page_side = SMSG_DATA_GET_PAGE_SIDE(msg->param2);
}

static inline void msg_scan_page_start_decode( MESSAGE *msg, uint8_t *page_side )
{
    *page_side = SMSG_PAGE_START_GET_PAGE_SIDE(msg->param1);
}

static inline void msg_scan_page_end_decode( MESSAGE *msg, uint8_t *page_side )
{
    *page_side = SMSG_PAGE_END_GET_PAGE_SIDE(msg->param1);
}

#endif

