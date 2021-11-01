/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief 
 *
 */

#ifndef FWAPPHEADER_H
#define FWAPPHEADER_H

#ifdef __cplusplus
extern "C" {
#endif


#define SIG_LENGTH_BYTES 256
typedef struct
{
    UINT32  LengthKey;
    UINT32 ProgInfo;
    UINT32 Version;
    UINT32 AuthLength;  ///< The length of the elf file in words (32 bits)
    UINT32 PayloadTag;  ///< Defined by the customer.
    UINT32  RandSeedHi; ///< Random seed for the signature.
    UINT32  RandSeedLo; ///< Random seed low
    char    DigitalSignature[SIG_LENGTH_BYTES];  ///< The signature itself.
    UINT32 *ExecAddr;   ///< execution override value
} FW_APP_HEADER;

#define HEAD_KEY_VALUE 0xc0de
#define AUTH_MASK 0x80000000
#define HEADER_LEN_MASK 0x7fff0000;
#define HEADER_LEN_SHIFT 16
#define KEY_MASK 0xffff
#define KEY_SHIFT 0
#define AUTH_KEY_MASK 0x1f000000
#define AUTH_KEY_SHIFT 24
#define PROG_ID_MASK 0xfff000
#define PROG_ID_SHIFT 12
#define VERSION_ID_MASK 0xfff
#define VERSION_ID_SHIFT 0
#define VERSION_YR_MASK 0xffe00000
#define VERSION_YR_SHIFT 21
#define VERSION_MO_MASK 0x1e0000
#define VERSION_MO_SHIFT 17
#define VERSION_DA_MASK 0x1f000
#define VERSION_DA_SHIFT 12
#define PLAT_ID_MASK 0xfff
#define PLAT_ID_SHIFT 0
#define EXEC_ADD_USEELF 1
#define EXEC_ADD_NORUN 3
#define CODE_TYPE_MASK 0x60000000
#define CODE_TYPE_SHIFT 29
#define CODE_COMPRESSION_MASK 0x80000000
#define CODE_COMPRESSION_SHIFT 31
 

#ifdef __cplusplus
}
#endif

#endif
