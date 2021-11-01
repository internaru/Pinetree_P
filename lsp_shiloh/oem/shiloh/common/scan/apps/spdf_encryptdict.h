
#ifndef _SPDF_ENCRYPTDICT_H
#define _SPDF_ENCRYPTDICT_H

/*---------------------------------------------------------------------------*/
/*------ SPDF_EncryptDict ---------------------------------------------------*/

void
SPDF_EncryptDict_CreateID(SPDF_Encrypt dict);//, SPDF_Dict info, SPDF_Xref xref);

SPDF_UINT8
SPDF_EncryptDict_SetPassword(SPDF_Encrypt dict, const char *owner_passwd, const char *user_passwd);

SPDF_UINT8
SPDF_EncryptDict_Prepare(SPDF_Encrypt dict);// SPDF_Dict info, SPDF_Xref xref);

#endif /* _SPDF_ENCRYPTDICT_H */

