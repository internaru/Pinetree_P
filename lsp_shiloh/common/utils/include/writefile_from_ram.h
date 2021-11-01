/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file writefile_from_ram.h
 *
 * \brief Writes a file from system memory to the filesystem.
 *
 **/
#ifndef WRITEFILE_FROM_RAM_H
#define WRITEFILE_FROM_RAM_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WRITEFILE_FROM_RAM_USAGE_ERROR   (-11)
#define WRITEFILE_FROM_RAM_FAIL_TO_OPEN  (-12)
#define WRITEFILE_FROM_RAM_FAIL_TO_WRITE (-13)

/**
 * \brief Write a file from system memory to the filesystem 
 *
 * \param[in]     filepath     This is the filename of the file to write
 * \param[in/out] source       A pointer to the memory containing the
 *                             desired file content.
 * \param[in/out] source_size  Length (in bytes) of the desired 
 *                             file content.
 *
 * \retval OK - All is well, non-zero length file was written
 *         WRITEFILE_FROM_RAM_USAGE_ERROR   - parameter error.
 *                                            filepath is NULL?
 *                                            source_size is 0?
 *         WRITEFILE_FROM_RAM_FAIL_TO_OPEN  - File could not be open()'ed
 *         WRITEFILE_FROM_RAM_FAIL_TO_WRITE - File could not be write()'ed
 *
 * This function blocks until the operation attempt is complete 
 * so the caller can free the source data if desired when this 
 * function returns. 
 *
 **/
error_type_t writefile_from_ram(const char* filepath,
                                void* source, uint64_t source_size);

#ifdef __cplusplus
}
#endif

#endif  /* WRITEFILE_FROM_RAM_H */
