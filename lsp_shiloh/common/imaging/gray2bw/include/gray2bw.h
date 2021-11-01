/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file gray2bw.h
 *
 * \brief This module performs conversion of 8 bit 2 dimensional "images" into
 *        1 bit 2 dimensional images of the same size.
 *
 * Suggested use model (other models will work):
 *
 * int r;
 * int m = 16;                            // Maximum num of rows for each loop
 * int w = image_width;                   // Width of the image to be processed
 * int s = gray2bw_memreq(w);             // Bytes needed for gray2bw context
 * void* data = malloc(m*w);              // Storage to hold input (and output)
 * void* mem  = malloc(s);                // Storage for gray2bw context
 * void* hdl;                             // gray2bw context pointer
 * 
 * if ((hdl = gray2bw_create(w, mem, s))) // Create context
 * {
 *   while ((r = read(infd, data, m*w)))  // Read data
 *   {
 *     r /= w;                            // Convert bytes read into rows read
 *     gray2bw_convert(hdl, data, r);     // Convert a number of rows
 *     write(outfd, data, r*((w+7)/8));   // Write results
 *   }
 * }
 * else
 * {
 *   // handle error
 * }
 *                                        // Note that gray2bw context does not
 *                                        //   require any sort of free or close
 * free(mem);                             // Free context memory
 * free(data);                            // Free input/output data storage
 *
 **/

#ifndef __GRAY2BW_H__
#define __GRAY2BW_H__
#include <stdint.h>

/**
 * \brief Determine how much memory is needed for a gray2bw conversion
 *
 * \param[in] width  How many pixels (bytes) wide is the input row
 * \retval           The number of bytes needed for this operation
 *
 * This is intened to be used to determine how much memory to provide to
 * gray2bw_create.
**/
uint32_t gray2bw_memreq(int width);

/**
 * \brief Initialize a gray2bw conversion (and obtain a handle)
 *
 * \param[in] width      How many pixels (bytes) wide is the input row
 * \param[in] mem        Usable memory from which to create the context
 * \param[in] mem_bytes  The number of bytes provided. This should be at
 *                       least gray2bw_memreq(width) and larger values serve
 *                       no purpose.
 * \retval               A handle to be provied to gray2bw_convert or NULL in
 *                       the case of an error (e.g. memory too small)
 *
 * Note this context does not need to be closed or freed. When the client is
 * done processing rows it can simply reclaim/recycle the provided memory
**/
void* gray2bw_create(int width, uint32_t* mem, int mem_bytes);

/**
 * \brief Convert a specified number of rows from grayscale to black and white
 *
 * \param[in]     hdl      A handle as returned from gray2bw_create
 * \param[in/out] data     A pointer to grayscale data (should point to numrows
 *                         of data). Upon function return the first 1/8 of the
 *                         data will be overwritten with black and white output.
 * \param[in]     numrows  The number of rows to convert
 * \retval                 0 upon success
**/
int gray2bw_convert(void* hdl, uint8_t* data, int numrows);

#endif /* __GRAY2BW_H__ */
