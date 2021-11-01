#include <time.h>
#include <string.h>
#include <stdio.h>
#include "spdf_types.h"
#include "spdf_encrypt.h"
#include "spdf_encryptdict.h"
#ifndef SPDF_UNUSED
#define SPDF_UNUSED(a) ((void)(a))
#endif

void
SPDF_EncryptDict_CreateID  (SPDF_Encrypt attr)//, HPDF_Dict info, HPDF_Xref xref)
{
    SPDF_MD5_CTX ctx;

    /* use the result of 'time' function to get random value.
     * when debugging, 'time' value is ignored.
     */
#if 0
    time_t t = HPDF_TIME (NULL);
#endif

    SPDF_MD5Init (&ctx);

#if 0
    SPDF_MD5Update(&ctx, (SPDF_BYTE *)&t, sizeof(t));
#endif

    SPDF_MD5Final(attr->encrypt_id, &ctx);
}


SPDF_UINT8
SPDF_EncryptDict_Prepare  (SPDF_Encrypt attr)//, SPDF_Dict info, SPDF_Xref xref)
{
    SPDF_EncryptDict_CreateID (attr);	//, info, xref);
    SPDF_Encrypt_CreateOwnerKey (attr);
    SPDF_Encrypt_CreateEncryptionKey (attr);
    SPDF_Encrypt_CreateUserKey (attr);
	
    return SPDF_NO_ERROR;
}

SPDF_UINT8
SPDF_EncryptDict_SetPassword (SPDF_Encrypt dict, const char *owner_passwd, const char *user_passwd)
{
	SPDF_PTRACE((" SPDF_EncryptDict_SetPassword\n"));

	if (strlen(owner_passwd) == 0)
		return SPDF_COMMON_ERROR;

	if (owner_passwd && user_passwd && strcmp(owner_passwd, user_passwd) == 0)
		return SPDF_COMMON_ERROR;

	SPDF_PadOrTrancatePasswd (owner_passwd, dict->owner_passwd);
	SPDF_PadOrTrancatePasswd (user_passwd, dict->user_passwd);

	return SPDF_NO_ERROR;
}

