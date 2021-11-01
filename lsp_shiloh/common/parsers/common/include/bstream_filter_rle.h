#ifndef BSTREAM_FILTER_RLE_H
#define BSTREAM_FILTER_RLE_H
/******************************************************************************
 * Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <stdint.h>
#include "bstream.h"

/// RunLengthDecode filter used by PDF
/// wraps a base bstream with a decode filter.
///


/// attach a rle filter to the base stream
///
bstream_t *
bstream_filter_rle_open( bstream_t *s );

/// detach rle filter from the base stream
///
void
bstream_filter_rle_close( bstream_t *s );

/// negative on error, premature end of stream
/// positive is output byte count placed at dest
///
int32_t
bstream_filter_rle_decode( bstream_t *s, //< base stream pointer post bstream_filter_rle_open call.
			   uint8_t *dest, //< decoded raster destination. 
			   uint32_t dest_len, //< size of destination buffer, might read less.
			   int32_t max
    );

#endif // BSTREAM_FILTER_RLE_H
