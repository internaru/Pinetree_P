#ifndef BSTREAM_FILTER_JPEG_H
#define BSTREAM_FILTER_JPEG_H
/******************************************************************************
 * Copyright (c) 2011-2013 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
#include <stdint.h>
#include "bstream.h"

/// DCT / jpeg filter used by PDF
/// wraps a base bstream with a decode filter.
///


/// attach a jpeg filter to the base stream
///
bstream_t *
bstream_filter_jpeg_open( bstream_t *s );

/// detach jpeg filter from the base stream
///
void
bstream_filter_jpeg_close( bstream_t *s );

/// clean up resourses
///
void
bstream_filter_jpeg_cleanup( );

/// negative on error, premature end of stream
/// positive is output byte count placed at dest
///
int32_t
bstream_filter_jpeg_decode( bstream_t *s, //< base stream pointer post bstream_filter_jpeg_open call.
			    uint8_t *dest, //< decoded raster destination. 
			    uint32_t dest_len, //< size of destination buffer, might read less.
			    int32_t max //< max input bytes to read 
    );

#endif // BSTREAM_FILTER_JPEG_H
