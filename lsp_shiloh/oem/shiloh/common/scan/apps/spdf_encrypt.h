#ifndef SPDF_ENCRYPT_H
#define SPDF_ENCRYPT_H

/*----------------------------------------------------------------------------*/
/*----- encrypt-dict ---------------------------------------------------------*/

#define SPDF_ID_LEN              16
#define SPDF_PASSWD_LEN          32
#define SPDF_ENCRYPT_KEY_MAX     16
#define SPDF_MD5_KEY_LEN         16
#define SPDF_PERMISSION_PAD      0xFFFFFFC0
#define SPDF_ARC4_BUF_SIZE       256


typedef struct SPDF_MD5Context
{
    SPDF_UINT32 buf[4];
    SPDF_UINT32 bits[2];
    SPDF_BYTE in[64];
} SPDF_MD5_CTX;


typedef struct _SPDF_ARC4_Ctx_Rec {
    SPDF_BYTE    idx1;
    SPDF_BYTE    idx2;
    SPDF_BYTE    state[SPDF_ARC4_BUF_SIZE];
} SPDF_ARC4_Ctx_Rec;


typedef struct _SPDF_Encrypt_Rec  *SPDF_Encrypt;

typedef struct _SPDF_Encrypt_Rec {
    /* key_len must be a multiple of 8, and between 40 to 128 */
    SPDF_UINT          key_len;

    /* owner-password (not encrypted) */
    SPDF_BYTE          owner_passwd[SPDF_PASSWD_LEN];

    /* user-password (not encrypted) */
    SPDF_BYTE          user_passwd[SPDF_PASSWD_LEN];

    /* owner-password (encrypted) */
    SPDF_BYTE          owner_key[SPDF_PASSWD_LEN];

    /* user-password (encrypted) */
    SPDF_BYTE          user_key[SPDF_PASSWD_LEN];

    SPDF_INT           permission;
    SPDF_BYTE          encrypt_id[SPDF_ID_LEN];
    SPDF_BYTE          encryption_key[SPDF_MD5_KEY_LEN + 5];
    SPDF_BYTE          md5_encryption_key[SPDF_MD5_KEY_LEN];
    SPDF_ARC4_Ctx_Rec  arc4ctx;
} SPDF_Encrypt_Rec;


void
SPDF_MD5Init  (struct SPDF_MD5Context  *ctx);


void
SPDF_MD5Update  (struct SPDF_MD5Context *ctx,
                 const SPDF_BYTE        *buf,
                 SPDF_UINT32            len);


void
SPDF_MD5Final  (SPDF_BYTE              digest[16],
                struct SPDF_MD5Context *ctx);

void
SPDF_PadOrTrancatePasswd  (const char  *pwd,
                           SPDF_BYTE        *new_pwd);


void
SPDF_Encrypt_Init  (SPDF_Encrypt  attr);


void
SPDF_Encrypt_CreateUserKey  (SPDF_Encrypt  attr);


void
SPDF_Encrypt_CreateOwnerKey  (SPDF_Encrypt  attr);


void
SPDF_Encrypt_CreateEncryptionKey  (SPDF_Encrypt  attr);


void
SPDF_Encrypt_InitKey  (SPDF_Encrypt  attr,
                       SPDF_UINT32       object_id,
                       SPDF_UINT16       gen_no);


void
SPDF_Encrypt_Reset  (SPDF_Encrypt  attr);


void
SPDF_Encrypt_CryptBuf  (SPDF_Encrypt  attr,
                        const SPDF_BYTE   *src,
                        SPDF_BYTE         *dst,
                        SPDF_UINT         len);

#endif /* _SPDF_ENCRYPT_H */


