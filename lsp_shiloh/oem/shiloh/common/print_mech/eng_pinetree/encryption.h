#ifndef __ENCRYPTION_H__
#define __ENCRYPTION_H__
typedef unsigned char u8;
typedef unsigned int u16; 
void AES128_Encrypt(u8 *ucInput_Chal, u8 *ucInputKey);
void AES128_Decrypt(u8 *ucInput_Chal, u8 *ucInputKey);

void EncryptionPassiveRcv(u8 *EncData, u16 random);
void EncryptionPassiveTrs(u8 *EncData);
void DecryptionRKE(u8 *RcvData);
#endif /* end of __ENCRYPTION_H__ */

