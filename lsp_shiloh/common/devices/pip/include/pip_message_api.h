/*
 * ============================================================================
 * Copyright (c) 2010-2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
#ifndef PIP_MESSAGE_API_H
#define PIP_MESSAGE_API_H


/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                              T Y P E D E F S                               */
/* -------------------------------------------------------------------------- */

// Define messages for the PIP block driver thread. Force the enum to be a 32
// bit quantity.
typedef enum { 
    RASTER_ODMA_DESC_COMPLETE, 
    RASTER_ODMA_OWN_ERROR,
    RASTER_ODMA_EOI_ALIGNMENT_ERROR,
    RASTER_ODMA_EOL_ALIGNMENT_ERROR,
    RASTER_ODMA_EOI_ERROR,
    RASTER_ODMA_CL_ALIGNMENT_ERROR,
    RASTER_ODMA_DIRECTION_ERROR,
    RASTER_ODMA_ERROR,
    RASTER_ODMA_SOFT_RESET_COMPLETED,
    RASTER_ODMA_END_OF_IMAGE,
    RASTER_ODMA_ADD_INPUT_BUFFER,
    PIP_MSG_ENUM = 0xFFFFFFFF 
} pip_msg_enum_t;

#define RASTER_ODMA_INVALID_MSG  PIP_MSG_ENUM

typedef struct
{
    pip_msg_enum_t msg;
    uint32_t descriptor;
    uint32_t chan;
    uint32_t value;  // For each message to use for its own purpose, ptr, data, etc.
} pip_message_t;

/* -------------------------------------------------------------------------- */
/*                   F U N C T I O N  P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */

error_type_t pip_send_message( pip_message_t *message );

#endif
