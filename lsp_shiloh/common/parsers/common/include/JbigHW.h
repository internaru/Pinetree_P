/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/// Single pass complete image encode or decode using the asic hw block.
/// returns true on OK false on error, and the comressed decompressed size in pnTargetBytes.
///
/// realloc can be used to shrink the comressed buffer to its "used" size this will 
/// shrink in place.
///
/// shrunk = MEM_REALLOC_LIMITED(lim, comressed_data, compressed_size);
///
bool JBIGConvert( bool  bEncodeNotDecode,    ///< true to compress, false to decompress
                  const uint8_t * pjSourceBuf, ///< source buffer 
                  uint32_t nSourceBytes,       ///< size of source
                  uint8_t * pjTargetBuf,       ///< dest buffer
                  uint32_t uiTargetBufSize,    ///< size of dest buffer
                  uint32_t * pnTargetBytes,    ///< returns used bytes in destination buffer
                  uint32_t nRows,            ///< lines 
                  uint32_t nCols,            ///< number of bits must be 128bit aligned for video dma later on. 
                  uint32_t nPadRows,         ///< 0 
                  bool Rotate_Bottom_to_Top  ///< used for backside duplex printing
    );
