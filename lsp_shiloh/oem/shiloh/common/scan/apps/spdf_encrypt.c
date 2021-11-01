#include <stdio.h>

#include "spdf_types.h"
#include "spdf_encrypt.h"
#include "string.h"

static const SPDF_BYTE SPDF_PADDING_STRING[] = {
    0x28, 0xBF, 0x4E, 0x5E, 0x4E, 0x75, 0x8A, 0x41,
    0x64, 0x00, 0x4E, 0x56, 0xFF, 0xFA, 0x01, 0x08,
    0x2E, 0x2E, 0x00, 0xB6, 0xD0, 0x68, 0x3E, 0x80,
    0x2F, 0x0C, 0xA9, 0xFE, 0x64, 0x53, 0x69, 0x7A
};


/*---------------------------------------------------------------------------*/
/*------ MD5 message-digest algorithm ---------------------------------------*/

static void
MD5Transform(SPDF_UINT32 buf[4], const SPDF_UINT32 in[16]);


static void
MD5ByteReverse(SPDF_BYTE *buf, SPDF_UINT32 longs);


void
SPDF_MD5Init  (struct SPDF_MD5Context  *ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bits[0] = 0;
    ctx->bits[1] = 0;
}


void
SPDF_MD5Update  (struct SPDF_MD5Context *ctx, const SPDF_BYTE *buf, SPDF_UINT32 len)
{
    SPDF_UINT32 t;

    /* Update bitcount */

    t = ctx->bits[0];
    if ((ctx->bits[0] = t + ((SPDF_UINT32) len << 3)) < t)
        ctx->bits[1]++;     /* Carry from low to high */
    ctx->bits[1] += len >> 29;

    t = (t >> 3) & 0x3f; /* Bytes already in shsInfo->data */

    /* Handle any leading odd-sized chunks */

    if (t) {
        SPDF_BYTE *p = (SPDF_BYTE *) ctx->in + t;

        t = 64 - t;
        if (len < t)
        {
            memcpy (p, buf, len);
            return;
        }
        memcpy (p, buf, t);
        MD5ByteReverse (ctx->in, 16);
        MD5Transform (ctx->buf, (SPDF_UINT32 *) ctx->in);
        buf += t;
        len -= t;
    }
    /* Process data in 64-byte chunks */

    while (len >= 64) {
        memcpy (ctx->in, buf, 64);
        MD5ByteReverse (ctx->in, 16);
        MD5Transform (ctx->buf, (SPDF_UINT32 *) ctx->in);
        buf += 64;
        len -= 64;
    }

    /* Handle any remaining bytes of data. */

    memcpy (ctx->in, buf, len);
}


/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void
SPDF_MD5Final  (SPDF_BYTE digest[16], struct SPDF_MD5Context *ctx)
{
    SPDF_UINT32 count;
    SPDF_BYTE *p;

    /* Compute number of bytes mod 64 */
    count = (ctx->bits[0] >> 3) & 0x3F;

    /* Set the first char of padding to 0x80.  This is safe since there is
       always at least one byte free */
    p = ctx->in + count;
    *p++ = 0x80;

    /* Bytes of padding needed to make 64 bytes */
    count = 64 - 1 - count;

    /* Pad out to 56 mod 64 */
    if (count < 8) {
        /* Two lots of padding:  Pad the first block to 64 bytes */
        memset (p, 0, count);
        MD5ByteReverse (ctx->in, 16);
        MD5Transform (ctx->buf, (SPDF_UINT32 *) ctx->in);

        /* Now fill the next block with 56 bytes */
        memset (ctx->in, 0, 56);
    } else {
        /* Pad block to 56 bytes */
        memset (p, 0, count - 8);
    }
    MD5ByteReverse (ctx->in, 14);

    /* Append length in bits and transform */
    ((SPDF_UINT32 *) ctx->in)[14] = ctx->bits[0];
    ((SPDF_UINT32 *) ctx->in)[15] = ctx->bits[1];

    MD5Transform (ctx->buf, (SPDF_UINT32 *) ctx->in);
    MD5ByteReverse ((SPDF_BYTE *) ctx->buf, 4);
    memcpy((SPDF_BYTE *)digest, (SPDF_BYTE *)ctx->buf, 16);
    memset((SPDF_BYTE *)ctx, 0, sizeof (ctx));   /* In case it's sensitive */
}

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the SPDF_MD5 algorithm. */
#define SPDF_MD5STEP(f, w, x, y, z, data, s) \
 ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )


/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
static void
MD5Transform  (SPDF_UINT32       buf[4],
               const SPDF_UINT32 in[16])
{
    register SPDF_UINT32 a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    SPDF_MD5STEP (F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    SPDF_MD5STEP (F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    SPDF_MD5STEP (F1, c, d, a, b, in[2] + 0x242070db, 17);
    SPDF_MD5STEP (F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    SPDF_MD5STEP (F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    SPDF_MD5STEP (F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    SPDF_MD5STEP (F1, c, d, a, b, in[6] + 0xa8304613, 17);
    SPDF_MD5STEP (F1, b, c, d, a, in[7] + 0xfd469501, 22);
    SPDF_MD5STEP (F1, a, b, c, d, in[8] + 0x698098d8, 7);
    SPDF_MD5STEP (F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    SPDF_MD5STEP (F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    SPDF_MD5STEP (F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    SPDF_MD5STEP (F1, a, b, c, d, in[12] + 0x6b901122, 7);
    SPDF_MD5STEP (F1, d, a, b, c, in[13] + 0xfd987193, 12);
    SPDF_MD5STEP (F1, c, d, a, b, in[14] + 0xa679438e, 17);
    SPDF_MD5STEP (F1, b, c, d, a, in[15] + 0x49b40821, 22);

    SPDF_MD5STEP (F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    SPDF_MD5STEP (F2, d, a, b, c, in[6] + 0xc040b340, 9);
    SPDF_MD5STEP (F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    SPDF_MD5STEP (F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    SPDF_MD5STEP (F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    SPDF_MD5STEP (F2, d, a, b, c, in[10] + 0x02441453, 9);
    SPDF_MD5STEP (F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    SPDF_MD5STEP (F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    SPDF_MD5STEP (F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    SPDF_MD5STEP (F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    SPDF_MD5STEP (F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    SPDF_MD5STEP (F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    SPDF_MD5STEP (F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    SPDF_MD5STEP (F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    SPDF_MD5STEP (F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    SPDF_MD5STEP (F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    SPDF_MD5STEP (F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    SPDF_MD5STEP (F3, d, a, b, c, in[8] + 0x8771f681, 11);
    SPDF_MD5STEP (F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    SPDF_MD5STEP (F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    SPDF_MD5STEP (F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    SPDF_MD5STEP (F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    SPDF_MD5STEP (F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    SPDF_MD5STEP (F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    SPDF_MD5STEP (F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    SPDF_MD5STEP (F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    SPDF_MD5STEP (F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    SPDF_MD5STEP (F3, b, c, d, a, in[6] + 0x04881d05, 23);
    SPDF_MD5STEP (F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    SPDF_MD5STEP (F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    SPDF_MD5STEP (F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    SPDF_MD5STEP (F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    SPDF_MD5STEP (F4, a, b, c, d, in[0] + 0xf4292244, 6);
    SPDF_MD5STEP (F4, d, a, b, c, in[7] + 0x432aff97, 10);
    SPDF_MD5STEP (F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    SPDF_MD5STEP (F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    SPDF_MD5STEP (F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    SPDF_MD5STEP (F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    SPDF_MD5STEP (F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    SPDF_MD5STEP (F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    SPDF_MD5STEP (F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    SPDF_MD5STEP (F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    SPDF_MD5STEP (F4, c, d, a, b, in[6] + 0xa3014314, 15);
    SPDF_MD5STEP (F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    SPDF_MD5STEP (F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    SPDF_MD5STEP (F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    SPDF_MD5STEP (F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    SPDF_MD5STEP (F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}


static void
MD5ByteReverse  (SPDF_BYTE    *buf,
                 SPDF_UINT32  longs)
{
    SPDF_UINT32 t;
    do
    {
        t = (SPDF_UINT32) ((SPDF_UINT32) buf[3] << 8 | buf[2]) << 16 |
        ((SPDF_UINT32) buf[1] << 8 | buf[0]);
        *(SPDF_UINT32 *) buf = t;
        buf += 4;
    }
    while (--longs);
}

/*----- encrypt-obj ---------------------------------------------------------*/

static void
ARC4Init(SPDF_ARC4_Ctx_Rec *ctx, const SPDF_BYTE *key, SPDF_UINT key_len);


static void
ARC4CryptBuf(SPDF_ARC4_Ctx_Rec *ctx, const SPDF_BYTE *in, SPDF_BYTE *out, SPDF_UINT len);


/*---------------------------------------------------------------------------*/

void
SPDF_PadOrTrancatePasswd  (const char *pwd, SPDF_BYTE *new_pwd)
{
	SPDF_UINT32 len = strlen(pwd);

	memset(new_pwd, 0x00, SPDF_PASSWD_LEN);

	if (len >= SPDF_PASSWD_LEN) {
		memcpy(new_pwd, (SPDF_BYTE *)pwd, SPDF_PASSWD_LEN);
	}
	else {
		if (len > 0) {
			memcpy(new_pwd, (SPDF_BYTE *)pwd, len);
		}
			memcpy(new_pwd + len, SPDF_PADDING_STRING, SPDF_PASSWD_LEN - len);
	}
}


void
SPDF_Encrypt_Init(SPDF_Encrypt  attr)
{
    memset(attr, 0, sizeof(SPDF_Encrypt_Rec));
    attr->key_len = 16;
    memcpy(attr->owner_passwd, SPDF_PADDING_STRING, SPDF_PASSWD_LEN);
    memcpy(attr->user_passwd, SPDF_PADDING_STRING, SPDF_PASSWD_LEN);
    attr->permission = SPDF_ENABLE_READ | SPDF_ENABLE_PRINT | SPDF_ENABLE_EDIT_ALL | SPDF_ENABLE_COPY | SPDF_ENABLE_EDIT;
}

void
SPDF_Encrypt_CreateOwnerKey  (SPDF_Encrypt  attr)
{
    SPDF_ARC4_Ctx_Rec rc4_ctx;
    SPDF_MD5_CTX md5_ctx;
    SPDF_BYTE digest[SPDF_MD5_KEY_LEN];
    SPDF_BYTE tmppwd[SPDF_PASSWD_LEN];
	
    SPDF_PTRACE((" SPDF_Encrypt_CreateOwnerKey\n"));

    /* create md5-digest using the value of owner_passwd */

    /* Algorithm 3.3 step 2 */
    SPDF_MD5Init(&md5_ctx);
    
    SPDF_MD5Update(&md5_ctx, attr->owner_passwd, SPDF_PASSWD_LEN);

    SPDF_PTRACE(("@ Algorithm 3.3 step 2\n"));

    SPDF_MD5Final(digest, &md5_ctx);
	
    /* Algorithm 3.3 step 3 (Revision 3 only) */
    /*if (attr->mode == SPDF_ENCRYPT_R3)*/ 
    {
        SPDF_UINT i;

        for (i = 0; i < 50; i++) {
            SPDF_MD5Init(&md5_ctx);

            /* SPDF_MD5Update (&md5_ctx, digest, SPDF_MD5_KEY_LEN); */
            SPDF_MD5Update (&md5_ctx, digest, attr->key_len);
            SPDF_MD5Final(digest, &md5_ctx);

            SPDF_PTRACE(("@ Algorithm 3.3 step 3 loop %u\n", i));
        }
    }

    /* Algorithm 3.3 step 4 */
    SPDF_PTRACE(("@ Algorithm 3.3 step 7 loop 0\n"));

    ARC4Init (&rc4_ctx, digest, attr->key_len);

    SPDF_PTRACE(("@ Algorithm 3.3 step 5\n"));

    /* Algorithm 3.3 step 6 */
    SPDF_PTRACE(("@ Algorithm 3.3 step 6\n"));
    ARC4CryptBuf (&rc4_ctx, attr->user_passwd, tmppwd, SPDF_PASSWD_LEN);

    /* Algorithm 3.3 step 7 */
    SPDF_PTRACE(("@ Algorithm 3.3 step 7\n"));
    /*if (attr->mode == SPDF_ENCRYPT_R3)*/
    {
        SPDF_BYTE tmppwd2[SPDF_PASSWD_LEN];
        SPDF_UINT i;

        for (i = 1; i <= 19; i++) {
            SPDF_UINT j;
            SPDF_BYTE new_key[SPDF_MD5_KEY_LEN];

            for (j = 0; j < attr->key_len; j++)
                new_key[j] = (SPDF_BYTE)(digest[j] ^ i);

            SPDF_PTRACE(("@ Algorithm 3.3 step 7 loop %u\n", i));

            memcpy (tmppwd2, tmppwd, SPDF_PASSWD_LEN);
            ARC4Init(&rc4_ctx, new_key, attr->key_len);
            ARC4CryptBuf(&rc4_ctx, tmppwd2, tmppwd, SPDF_PASSWD_LEN);
        }
    }

    /* Algorithm 3.3 step 8 */
    SPDF_PTRACE(("@ Algorithm 3.3 step 8\n"));
    memcpy(attr->owner_key, tmppwd, SPDF_PASSWD_LEN);
}


void
SPDF_Encrypt_CreateEncryptionKey (SPDF_Encrypt  attr)
{
    SPDF_MD5_CTX md5_ctx;
    SPDF_BYTE tmp_flg[4];

    SPDF_PTRACE((" SPDF_Encrypt_CreateEncryptionKey\n"));

    /* Algorithm3.2 step2 */
    SPDF_MD5Init(&md5_ctx);
    SPDF_MD5Update(&md5_ctx, attr->user_passwd, SPDF_PASSWD_LEN);

    /* Algorithm3.2 step3 */
    SPDF_MD5Update(&md5_ctx, attr->owner_key, SPDF_PASSWD_LEN);


    /* Algorithm3.2 step4 */
    SPDF_PTRACE(("@@@ permission =%d\n", attr->permission));
    tmp_flg[0] = (SPDF_BYTE)(attr->permission);
    tmp_flg[1] = (SPDF_BYTE)(attr->permission >> 8);
    tmp_flg[2] = (SPDF_BYTE)(attr->permission >> 16);
    tmp_flg[3] = (SPDF_BYTE)(attr->permission >> 24);

    SPDF_MD5Update(&md5_ctx, tmp_flg, 4);

    /* Algorithm3.2 step5 */
    SPDF_PTRACE(("@ Algorithm 3.2 step 5\n"));

    SPDF_MD5Update(&md5_ctx, attr->encrypt_id, SPDF_ID_LEN);
    SPDF_MD5Final(attr->encryption_key, &md5_ctx);

    /* Algorithm 3.2 step6 (Revision 3 only) */
    /*if (attr->mode == SPDF_ENCRYPT_R3)*/
    {
        SPDF_UINT i;

        for (i = 0; i < 50; i++) {
            SPDF_PTRACE(("@ Algorithm 3.3 step 6 loop %u\n", i));
            SPDF_MD5Init(&md5_ctx);
            SPDF_MD5Update (&md5_ctx, attr->encryption_key, attr->key_len);
            SPDF_MD5Final(attr->encryption_key, &md5_ctx);
        }
    }
}


void
SPDF_Encrypt_CreateUserKey (SPDF_Encrypt  attr)
{
    SPDF_ARC4_Ctx_Rec ctx;

    SPDF_PTRACE((" SPDF_Encrypt_CreateUserKey\n"));

    /* Algorithm 3.4/5 step1 */

    /* Algorithm 3.4 step2 */
    ARC4Init(&ctx, attr->encryption_key, attr->key_len);
    ARC4CryptBuf(&ctx, SPDF_PADDING_STRING, attr->user_key, SPDF_PASSWD_LEN);

    /*if (attr->mode == SPDF_ENCRYPT_R3) */
    {
        SPDF_MD5_CTX md5_ctx;
        SPDF_BYTE digest[SPDF_MD5_KEY_LEN];
        SPDF_BYTE digest2[SPDF_MD5_KEY_LEN];
        SPDF_UINT i;

        /* Algorithm 3.5 step2 (same as Algorithm3.2 step2) */
        SPDF_MD5Init(&md5_ctx);
        SPDF_MD5Update(&md5_ctx, SPDF_PADDING_STRING, SPDF_PASSWD_LEN);

        /* Algorithm 3.5 step3 */
        SPDF_MD5Update(&md5_ctx, attr->encrypt_id, SPDF_ID_LEN);
        SPDF_MD5Final(digest, &md5_ctx);

        SPDF_PTRACE(("@ Algorithm 3.5 step 3\n"));

        /* Algorithm 3.5 step4 */
        ARC4Init(&ctx, attr->encryption_key, attr->key_len);
        ARC4CryptBuf(&ctx, digest, digest2, SPDF_MD5_KEY_LEN);

        SPDF_PTRACE(("@ Algorithm 3.5 step 4\n"));

        /* Algorithm 3.5 step5 */
        for (i = 1; i <= 19; i++) {
            SPDF_UINT j;
            SPDF_BYTE new_key[SPDF_MD5_KEY_LEN];

            SPDF_PTRACE(("@ Algorithm 3.5 step 5 loop %u\n", i));

            for (j = 0; j < attr->key_len; j++)
                new_key[j] = (SPDF_BYTE)(attr->encryption_key[j] ^ i);

            memcpy(digest, digest2, SPDF_MD5_KEY_LEN);

            ARC4Init(&ctx, new_key, attr->key_len);
            ARC4CryptBuf(&ctx, digest, digest2, SPDF_MD5_KEY_LEN);
        }

        /* use the result of Algorithm 3.4 as 'arbitrary padding' */
        memset(attr->user_key, 0, SPDF_PASSWD_LEN);
        memcpy(attr->user_key, digest2, SPDF_MD5_KEY_LEN);
    }
}


void
ARC4Init(SPDF_ARC4_Ctx_Rec *ctx, const SPDF_BYTE  *key, SPDF_UINT key_len)
{
    SPDF_BYTE tmp_array[SPDF_ARC4_BUF_SIZE];
    SPDF_UINT i;
    SPDF_UINT j = 0;

    SPDF_PTRACE((" ARC4Init\n"));

    for (i = 0; i < SPDF_ARC4_BUF_SIZE; i++)
        ctx->state[i] = (SPDF_BYTE)i;

    for (i = 0; i < SPDF_ARC4_BUF_SIZE; i++)
        tmp_array[i] = key[i % key_len];

    for (i = 0; i < SPDF_ARC4_BUF_SIZE; i++) {
        SPDF_BYTE tmp;

        j = (j + ctx->state[i] + tmp_array[i]) % SPDF_ARC4_BUF_SIZE;

        tmp = ctx->state[i];
        ctx->state[i] = ctx->state[j];
        ctx->state[j] = tmp;
    }

    ctx->idx1 = 0;
    ctx->idx2 = 0;
}


void
ARC4CryptBuf (SPDF_ARC4_Ctx_Rec *ctx, const SPDF_BYTE *in, SPDF_BYTE *out, SPDF_UINT len)
{
    SPDF_UINT i;
    SPDF_UINT t;
    SPDF_BYTE K;

    SPDF_PTRACE((" ARC4CryptBuf\n"));

    for (i = 0; i < len; i++) {
        SPDF_BYTE tmp;

        ctx->idx1 = (SPDF_BYTE)((ctx->idx1 + 1) % 256);
        ctx->idx2 = (SPDF_BYTE)((ctx->idx2 +  ctx->state[ctx->idx1]) % 256);

        tmp = ctx->state[ctx->idx1];
        ctx->state[ctx->idx1] = ctx->state[ctx->idx2];
        ctx->state[ctx->idx2] = tmp;

        t = (ctx->state[ctx->idx1] + ctx->state[ctx->idx2]) % 256;
        K = ctx->state[t];

        out[i] = (SPDF_BYTE)(in[i] ^ K);
    }
}


void
SPDF_Encrypt_InitKey(SPDF_Encrypt attr, SPDF_UINT32 object_id, SPDF_UINT16 gen_no)
{
    SPDF_MD5_CTX ctx;
    SPDF_UINT key_len;

    SPDF_PTRACE((" SPDF_Encrypt_Init\n"));

    attr->encryption_key[attr->key_len] = (SPDF_BYTE)object_id;
    attr->encryption_key[attr->key_len + 1] = (SPDF_BYTE)(object_id >> 8);
    attr->encryption_key[attr->key_len + 2] = (SPDF_BYTE)(object_id >> 16);
    attr->encryption_key[attr->key_len + 3] = (SPDF_BYTE)gen_no;
    attr->encryption_key[attr->key_len + 4] = (SPDF_BYTE)(gen_no >> 8);

    SPDF_PTRACE(("@@@ OID=%u, gen_no=%u\n", (SPDF_INT)object_id, gen_no));

    SPDF_MD5Init(&ctx);
    SPDF_MD5Update(&ctx, attr->encryption_key, attr->key_len + 5);
    SPDF_MD5Final(attr->md5_encryption_key, &ctx);

    key_len = (attr->key_len + 5 > SPDF_ENCRYPT_KEY_MAX) ?
                    SPDF_ENCRYPT_KEY_MAX : attr->key_len + 5;

    ARC4Init(&attr->arc4ctx, attr->md5_encryption_key, key_len);
}


void
SPDF_Encrypt_Reset(SPDF_Encrypt attr)
{
    SPDF_UINT key_len = (attr->key_len + 5 > SPDF_ENCRYPT_KEY_MAX) ?
                    SPDF_ENCRYPT_KEY_MAX : attr->key_len + 5;

    SPDF_PTRACE((" SPDF_Encrypt_Reset\n"));

    ARC4Init(&attr->arc4ctx, attr->md5_encryption_key, key_len);
}


void
SPDF_Encrypt_CryptBuf(SPDF_Encrypt attr, const SPDF_BYTE *src, SPDF_BYTE *dst, SPDF_UINT len)
{
    ARC4CryptBuf(&attr->arc4ctx, src, dst, len);
}


/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

