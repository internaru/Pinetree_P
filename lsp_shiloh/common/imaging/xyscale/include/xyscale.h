/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * \file xyscale.h
 *
 * \brief This module performs scaling of 8 bit 2 dimensional "images".
 *        The data input and output must be 8 bits per plane but can contain
 *        as many planes as is convenient (typical values are 1, 3 and 4)
 *
 *        The data is arranged in plane-interleaved format and processed in a
 *        row at a time fashion. i.e. a single row of 3 plane data will contain
 *        ordered triples of data for a single row (RGB for pixel 0 followed by
 *        RGB for pixel1, and so on across an entire row).
 *
 * Suggested use model (other models will work):
 *
 * outrow = 0;
 *
 * xyscale_init(&hdl, planes, incols, inrows, outcols, outrows, mem, memsize);
 *
 * acquire_input_datarow(datain)
 *
 * while (outrow < outrows)
 * {
 *   if (XYSCALE_OKAY == xyscale_indata(&hdl, datain))
 *   {
 *     acquire_input_datarow(datain);
 *   }
 *
 *   if (XYSCALE_OKAY == xyscale(&hdl, outdata))
 *   {
 *     produce_output_datarow(outdata);
 *     outrow += 1;
 *   }
 * }
 *
 **/

#ifndef __INC_XYSCALE_H__
#define __INC_XYSCALE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Return values for all functions in the xyscale library
 */
#define XYSCALE_DATA_EMPTY   ( 2)
#define XYSCALE_DATA_FULL    ( 1)
#define XYSCALE_OKAY          (0)
#define XYSCALE_HANDLE_ERR   (-1)
#define XYSCALE_YSCALE_ERR   (-2)
#define XYSCALE_XSCALE_ERR   (-3)
#define XYSCALE_MEMORY_ERR   (-4)

/**
 * \brief The opaque handle for a single instance of an xyscale operation.
 */
typedef void* xyscale_hdl_t;

/**
 * \brief A control for the speed/memory use/quality of the scale operation
 */
typedef enum
{
  XYSCALE_SPEED_NORMAL = 0, /* Will do normal averaging and interpolating */
  XYSCALE_SPEED_FAST   = 1, /* Resorts to nearest neighbor algorithm */
} xyscale_speed_t;

/**
 * \brief Initialize an xyscale operation
 *
 * \param[out] hdl     A reference to an xyscale_hdl_t. This will be returned
 *                     to the caller to be used with future operations
 * \param[in] planes   The number of planes of data in both input and output
 * \param[in] incols   The 'width' of the input data
 * \param[in] inrows   The 'height' of the input data
 * \param[in] outcols  The 'width' of the output data
 * \param[in] outrows  The 'height' of the output data
 * \param[in] mem      Provided memory (used for storing working data)
 * \param[in] memsize  The amount of memory provided (see below)
 * \param[in] speed    A speed/memory/quality setting
 * \retval             One of XYSCALE_??? return values defined earlier
 *                     XYSCALE_OKAY indicates success.
 *
 * The provided memory must be at least the size indicated by a call to
 * xyscale_mem_size with the same parameters. This memory should be aligned to
 * a 4 byte boundary. If the memory is not aligned the library will align it
 * but this will consume up to 3 bytes and so the provided memory will have to
 * be slightly larger in some cases.
 *
 * Note that xyscale are not specified by input and output resolutions or scale
 * factors but rather by specifying input and output dimensions. It is expected
 * that this will be more useful. However, it would not be difficult to allow
 * other initialization routines that used different semantics.
 *
 * Note that there is no mutual exclusion protection provided by this library.
 * If calls are made against the same hdl then it is recommended that the
 * caller provide mutex locks on the use hdl.
**/
int xyscale_init(xyscale_hdl_t* hdl,
                 int planes, int incols, int inrows, int outcols, int outrows,
                 uint8_t* mem, uint32_t memsize, xyscale_speed_t speed);

/**
 * \brief Determine how much memory will be needed by an xyscale operation
 *
 * \param[in] planes   The number of planes of data in both input and output
 * \param[in] incols   The 'width' of the input data
 * \param[in] inrows   The 'height' of the input data
 * \param[in] outcols  The 'width' of the output data
 * \param[in] outrows  The 'height' of the output data
 * \param[in] speed    A speed/memory/quality setting
 * \retval             The number of bytes needed for the operation
 *
 * The purpose for this routine is to allow the use of the xyscale_init()
 * routine to know how much memory is necessary for a given scale operation.
**/
uint32_t xyscale_mem_size(int planes, int incols, int inrows,
                          int outcols, int outrows, xyscale_speed_t speed); 


/**
 * \brief Provide a row of input data to the scaler
 *
 * \param[in] hdl     A previously initialized xyscale_hdl_t.
 * \param[in] data    A pointer to input data (must be planes * incols wide)
 * \retval            XYSCALE_OKAY or XYSCALE_DATA_FULL (or XYSCALE_HANDLE_ERR)
 *
 * If this function returns XYSCALE_OKAY then the process has cached this data
 * successfully and the next row of input data should be provided on a
 * subsequent call. If XYSCALE_DATA_FULL is returned this is an indication that
 * there was not space to cache this input data and that it must be re-provided
 * in a subsequent call. In this latter case at least one call to
 * xyscale_outdata should be made to make room for additional input data.
**/
int xyscale_indata(xyscale_hdl_t hdl, uint8_t *data);


/**
 * \brief Retrieve a row of output data for the caller
 *
 * \param[in] hdl     A previously initialized xyscale_hdl_t.
 * \param[in] data    A pointer to output data (will be planes * outcols wide)
 * \retval            XYSCALE_OKAY or XYSCALE_DATA_EMPTY (or XYSCALE_HANDLE_ERR)
 *
 * This function will write a row of output data into the memory pointed to by
 * data. The memory should be large enough to hold planes * outcols bytes of
 * data.
 *
 * If XYSCALE_DATA_EMPTY is returned this is an indication that the scaler needs
 * more input data before it can produce an output row.
**/
int xyscale_outdata(xyscale_hdl_t hdl, uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif /* __INC_XYSCALE_H__ */
