/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/
/**
 * \file base64_api.h
 *
 * \brief This module performs base64 conversions. A primary use case is with
 *        the CMD system which cannot, without encoding, handle binary data
 **/

#ifndef __INC_BASE64_API_H__
#define __INC_BASE64_API_H__


/**
 * \brief Type of a function that supplies data to the base64 routines.
 *
 * \param[in] arg  The getarg parameter supplied by a client of this library
 * \retval         The next byte of data from an input stream which would be
 *                 either binary data or an encoded base64 character. The value
 *                 is cast to an int so that a special value of (int)-1 can
 *                 signal the end of the stream.
 *
 * This function is intended to act like getc() or fgetc() from libc
 */
typedef int (*base64_get_t)(void* arg);

/**
 * \brief Type of a function that receives data from the base64 routines.
 *
 * \param[in] arg  The putarg parameter supplied by a client of this library
 * \param[in] c    The byte to be emitted to the output stream
 * \retval         Is not currently used but convention is that if the put fails
 *                 for any reason to return -1. 0 would be returned in the case
 *                 of success.
 *
 * This function is intended to act like putc() or fputc() from libc
 */
typedef int (*base64_put_t)(void* arg, unsigned char c);

/**
 * \brief Initialize the base64 library
 *
 * Currently this only causes base64 to register cmd handlers with cmd
 */
void base64_init(void);

/**
 * \brief Encode a stream of binary data into a stream of base64 data
 *
 * \param[in] get     A function called to retrieve input data
 * \param[in] getarg  The argument to provide to invocations of get
 * \param[in] put     A function called to produce output data
 * \param[in] putarg  The argument to provide to invocations of put
 * \retval            The number of output bytes produced
 *
 * This will encode a stream of binary data, provided by get(getarg), into an
 * ASCII stream via put(putarg, <data>) until the get function returns -1.
 * Note that the output stream is NOT 0 terminated.
 */
int  base64_encode(base64_get_t get, void* getarg,
                   base64_put_t put, void* putarg);

/**
 * \brief Decode a stream of base64 data into a stream of binary data
 *
 * \param[in] get     A function called to retrieve input data
 * \param[in] getarg  The argument to provide to invocations of get
 * \param[in] put     A function called to produce output data
 * \param[in] putarg  The argument to provide to invocations of put
 * \retval            The number of output bytes produced
 *
 * This will decode a stream of base64 data, provided by get(getarg), into a
 * binary stream via put(putarg, <data>) until the get function returns -1.
 */
int  base64_decode(base64_get_t get, void* getarg,
                   base64_put_t put, void* putarg);

/**
 * \brief Decode a "string" of base64 data into memory
 *
 * \param[in] dst  A pointer to memory where the output shall be stored
 * \param[in] src  A pointer to memory where a NULL terminated string of
 *                 base64 data is stored.
 * \retval         The number of output bytes produced
 *
 * The caller should make sure that dst is large enough to hold all of the
 * decoded data.
 */
int base64_dec(void* dst, const char* src);

/**
 * \brief Encode a block of memory into a base64 encoded "string"
 *
 * \param[in] dst  A pointer to memory where the output shall be stored
 * \param[in] src  A pointer to memory where the source is located
 * \param[in] len  The number of bytes to use as source data
 * \retval         The number of output bytes produced
 *
 * The caller should make sure that dst is large enough to hold all of the
 * decoded data. This is, generally, (len*4+2)/3+2 bytes.
 * Note that the output "string" is NOT 0 terminated.
 */
int base64_enc(void* dst, void* src, unsigned int len);

#endif /* __INC_BASE64_API_H__ */
