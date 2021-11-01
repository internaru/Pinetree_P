/** @file  dh.h 
  * @brief This file contains definition for Diffie-Hellman Key
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */

#ifndef _DH_H
#define _DH_H

#include "type_def.h"
#include "encrypt.h"

/** Get lower 16 bits */
#define LOW_16_BITS(x)          (x & 0x0000FFFF)
/** Get higher 16 bits */
#define HIGH_16_BITS(x)         ((x & 0xFFFF0000) >> 16)
/** Right shift 16 bits */
#define RIGHT_SHIFT_16_BITS(x)  ((x << 16) & 0xFFFF0000)
/** U32 bits length */
#define U32_BITS_LENGTH         32
/** Get 2 MSB*/
#define GET_2MSB(x)             ((x >> 30) & 0x03)
/** Maximum digit size */
#define MAX_DIGIT_SIZE          128     /* 256 */

int setup_dh_agreement(u8 *, u32, u8 *, u32, DH_PG_PARAMS *);
int compute_dh_agreed_key(u8 *, u8 *, u32, u8 *, u32, DH_PG_PARAMS *);

#endif /* _DH_H */
