/** @file  encrypt.h 
  * @brief This file contains definition for encrypt library
  * 
  * Copyright (C) 2003-2008, Marvell International Ltd.
  * All Rights Reserved
  */

#ifndef _ENCRYPT_H
#define _ENCRYPT_H

#include "type_def.h"

/** Digest size	*/
#define SHA256_DIGEST_SIZE (256 / 8)
/** Block size	*/
#define SHA256_BLOCK_SIZE  (512 / 8)

/**
 * Diffie-Hellman parameters.
 */
typedef struct
{
    /** prime */
    unsigned char *prime;
    /** length of prime */
    unsigned int primeLen;
    /** generator */
    unsigned char *generator;
    /** length of generator */
    unsigned int generatorLen;
} DH_PG_PARAMS;

int Mrv_SHA256(u8 * message, u32 len, u8 * digest);
int MrvHMAC_SHA256(u8 * Key, u32 Key_size, u8 * Message, u32 Message_len,
                   u8 * Mac, u32 MacSize);

int MrvDH_Setup(u8 * public_key, u32 public_len,
                u8 * private_key, u32 private_len, DH_PG_PARAMS * dh_params);
int MrvDH_Compute(u8 * shared_key,
                  u8 * public_key, u32 public_len,
                  u8 * private_key, u32 private_len, DH_PG_PARAMS * dh_params);

int MrvAES_Wrap(u8 * pPlainTxt, u32 PlainTxtLen, u8 * pCipTxt, u8 * pKEK,
                u32 KeyLen, u8 * IV);
int MrvAES_UnWrap(u8 * pCipTxt, u32 CipTxtLen, u8 * pPlainTxt, u8 * pKEK,
                  u32 KeyLen, u8 * IV);

int MrvKDF(u8 * Key, u32 Key_size, u32 TotalKeyLen, u8 * OutKey);
unsigned int N8_Digits(u8 * a, u32 digits);

#ifdef EMBEDDED_SUPPLICANT
void pbkdf2_sha1(const char *passphrase, const char *ssid, size_t ssid_len,
                 int iterations, u8 * buf, size_t buflen);
#endif

#endif /* ENCRYPT_H */
