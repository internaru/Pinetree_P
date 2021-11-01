/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file readfile_into_ram.h
 *
 * \brief Reads a file from the file system into allocated system memory
 *
 **/
#ifndef READFILE_INTO_RAM_H
#define READFILE_INTO_RAM_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define READFILE_INTO_RAM_USAGE_ERROR   (-11)
#define READFILE_INTO_RAM_FAIL_TO_OPEN  (-12)
#define READFILE_INTO_RAM_FAIL_TO_STAT  (-13)
#define READFILE_INTO_RAM_FAIL_TO_READ  (-14)
#define READFILE_INTO_RAM_FAIL_TO_ALLOC (-15)

/**
 * \brief Read a file from the filesystem into system memory
 *
 * \param[in]     filepath     This is the filename of the file to read
 * \param[in/out] results      A pointer to variable that will be updated to
 *                             point to the allocated memory.
 * \param[in/out] results_size If provided, this routine will fill in the size
 *                             of the file here. May be NULL.

 * \retval OK - All is well, memory is allocated, file is read
 *         READFILE_INTO_RAM_USAGE_ERROR   - parameter error.
 *                                           Filepath is NULL?
 *                                           Results is NULL?
 *         READFILE_INTO_RAM_FAIL_TO_OPEN  - File could not be open()'ed.
 *         READFILE_INTO_RAM_FAIL_TO_STAT  - File could not be stat()'ed.
 *         READFILE_INTO_RAM_FAIL_TO_READ  - File could not be read()'ed.
 *         READFILE_INTO_RAM_FAIL_TO_ALLOC - Memory allocation failed
 *
 * It is the responsibility of the caller to eventually free the results.
 * This should be done with MEM_FREE_AND_NULL(*results).
 *
 **/
error_type_t readfile_into_ram(const char* filepath,
                               void** results, uint64_t* results_size);

#ifdef __cplusplus
}
#endif

#endif  /* READFILE_INTO_RAM_H */
