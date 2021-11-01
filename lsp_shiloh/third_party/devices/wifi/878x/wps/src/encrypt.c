/** @file  encrypt.c 
  * @brief This file contains interface function for encrypt library
  * 
  * Copyright (C) 2003-2013, Marvell International Ltd.
  * All Rights Reserved
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#ifdef __linux__

#include "os_network.h"
#define DBG_PRFX "encrypt: "
#define DBG_VERBOSE_ON
#include "net_debug.h"

// lib headers
#include <openssl/sha.h>

int Mrv_SHA256(unsigned char* tmp_uuid, uint32_t len, uint8_t* hashDigest)
{
   SHA256_CTX sha256;
   SHA256_Init(&sha256);
   SHA256_Update(&sha256, tmp_uuid, len);
   SHA256_Final(hashDigest, &sha256);
   return 0;
}

#else // ! __linux__

//#include <stdio.h>
#include "common.h"
#include "encrypt.h"
#include "sha256.h"
#include "aes_wrap.h"
#include "dh.h"
#include "wps_util.h"

void sha256_vector(size_t num_elem, const u8 * addr[], const size_t * len,
                   u8 * mac);

/**********************************************************
 ***   HMAC-SHA256
 **********************************************************/
/** 
 *  @brief  Wrapper function for SHA256 hash
 *
 *  @param data         Pointers to the data areas
 *  @param data_len     Lengths of the data blocks
 *  @param digest       Buffer for the hash
 *  @return             0 on success, -1 on failure
 */
int
Mrv_SHA256(u8 * data, u32 data_len, u8 * digest)
{
    const u8 *addr[2];
    size_t len[2];

    addr[0] = (u8 *) data;
    len[0] = (size_t) data_len;
    sha256_vector(1, addr, len, digest);

    return 0;
}

/** 
 *  @brief  Wrapper function for HMAC-SHA256 (RFC 2104)
 *
 *  @param Key          Key for HMAC operations
 *  @param Key_size     Length of the key in bytes
 *  @param Message      Pointers to the data areas
 *  @param Message_len  Lengths of the data blocks
 *  @param Mac          Buffer for the hash (32 bytes)
 *  @param MacSize      Length of hash buffer
 *  @return             0 on success, -1 on failure
 */
int
MrvHMAC_SHA256(u8 * Key, u32 Key_size, u8 * Message, u32 Message_len, u8 * Mac,
               u32 MacSize)
{
    const u8 *addr[2];
    size_t len[2];

    addr[0] = Message;
    len[0] = (size_t) Message_len;
    hmac_sha256_vector(Key, Key_size, 1, addr, len, Mac);

    return 0;
}

/**********************************************************
 *****     Diffie-Hellman Shared Key Generation       *****
 **********************************************************/
/** 
 *  @brief  Sets up Diffie-Hellman key agreement.
 *
 *  @param public_key       Pointers to public key generated
 *  @param public_len       Length of public key
 *  @param private_key      Pointers to private key generated randomly
 *  @param private_len      Length of private key
 *  @param dh_params        Parameters for DH algorithm
 *  @return                 0 on success, -1 on failure
 */
int
MrvDH_Setup(u8 * public_key, u32 public_len,
            u8 * private_key, u32 private_len, DH_PG_PARAMS * dh_params)
{
    return (setup_dh_agreement(public_key, public_len,
                               private_key, private_len, dh_params));
}

/** 
 *  @brief  Computes agreed shared key from the public value,
 *          private value, and Diffie-Hellman parameters.
 *
 *  @param shared_key       Pointer to agreed shared key generated
 *  @param public_key       Pointer to public key generated
 *  @param public_len       Length of public key
 *  @param private_key      Pointer to private key generated randomly
 *  @param private_len      Length of private key
 *  @param dh_params        Parameters for DH algorithm
 *  @return                 0 on success, -1 on failure
 */
int
MrvDH_Compute(u8 * shared_key,
              u8 * public_key, u32 public_len,
              u8 * private_key, u32 private_len, DH_PG_PARAMS * dh_params)
{
    return (compute_dh_agreed_key(shared_key,
                                  public_key, public_len,
                                  private_key, private_len, dh_params));
}

/**********************************************************
 ***   AES Key Wrap Key
 **********************************************************/
/** 
 *  @brief  Wrap keys with AES Key Wrap Algorithm (128-bit KEK)
 *
 *  @param pPlainTxt    Plaintext key to be wrapped
 *  @param TextLen      Length of the plain key in bytes (16 bytes)
 *  @param pCipTxt      Wrapped key
 *  @param pKEK         Key encryption key (KEK)
 *  @param KeyLen       Length of KEK in bytes (must be divisible by 16)
 *  @param IV           Encryption IV for CBC mode (16 bytes)
 *  @return             0 on success, -1 on failure
 */
int
MrvAES_Wrap(u8 * pPlainTxt, u32 TextLen, u8 * pCipTxt, u8 * pKEK, u32 KeyLen,
            u8 * IV)
{
    u8 *buf;

    if ((TextLen / 16) % 2) {
        buf = (u8 *) os_malloc(TextLen);
        if (buf == NULL) {
            wps_printf(MSG_INFO, "MrvAES_Wrap malloc failed!\n");
            return -1;
        }

        memcpy(buf, pPlainTxt, TextLen);
        aes_128_cbc_encrypt(pKEK, IV, buf, TextLen);
        memcpy(pCipTxt, buf, TextLen);
    } else {
        buf = (u8 *) os_malloc(TextLen + 16);
        if (buf == NULL) {
            wps_printf(MSG_INFO, "MrvAES_Wrap malloc failed!\n");
            return -1;
        }

        memcpy(buf, pPlainTxt, TextLen);
        memset(&buf[TextLen], 0x10, 16);
        aes_128_cbc_encrypt(pKEK, IV, buf, TextLen + 16);
        memcpy(pCipTxt, buf, TextLen + 16);
    }

    os_free(buf);
    return 0;
}

/** 
 *  @brief  Unwrap key with AES Key Wrap Algorithm (128-bit KEK)
 *
 *  @param pCipTxt      Wrapped key to be unwrapped
 *  @param TextLen      Length of the wrapped key in bytes (16 bytes)
 *  @param pPlainTxt    Plaintext key
 *  @param pKEK         Key encryption key (KEK)
 *  @param KeyLen       Length of KEK in bytes (must be divisible by 16)
 *  @param IV           Encryption IV for CBC mode (16 bytes)
 *  @return             0 on success, -1 on failure
 */
int
MrvAES_UnWrap(u8 * pCipTxt, u32 TextLen, u8 * pPlainTxt, u8 * pKEK, u32 KeyLen,
              u8 * IV)
{
    u8 *buf;

    if ((TextLen / 16) % 2) {
        buf = (u8 *) os_malloc(TextLen);
        if (buf == NULL) {
            wps_printf(MSG_INFO, "MrvAES_UnWrap malloc failed!\n");
            return -1;
        }

        memcpy(buf, pCipTxt, TextLen);
        aes_128_cbc_decrypt(pKEK, IV, buf, TextLen - 16);
        memcpy(pPlainTxt, buf, TextLen - 16);
    } else {
        buf = (u8 *) os_malloc(TextLen);
        if (buf == NULL) {
            wps_printf(MSG_INFO, "MrvAES_UnWrap malloc failed!\n");
            return -1;
        }

        memcpy(buf, pCipTxt, TextLen);
        aes_128_cbc_decrypt(pKEK, IV, buf, TextLen);
        memcpy(pPlainTxt, buf, TextLen);
    }

    return 0;
}

#endif // !__linux__

#ifdef __cplusplus
}
#endif

//eof
