//#include "stm8s.h"
//#include "global.h"
#include "encryption.h"
#include "farch.h"

// AES encryption size
// 1558 bytes code
// 145 bytes const
// 268 bytes data
// AES encryption + decryption size
// 2726 bytes code
// 145 bytes const
// 524 bytes data
#define BC 4

u8 ucC[16],ucK[16];

struct stBitType
{
	unsigned int Bit0 : 1;
	unsigned int Bit1 : 1;
	unsigned int Bit2 : 1;
	unsigned int Bit3 : 1;
	unsigned int Bit4 : 1;
	unsigned int Bit5 : 1;
	unsigned int Bit6 : 1;
	unsigned int Bit7 : 1;
};
union unBitConvert
{
	struct stBitType BitValue;
	u8 ucValue;
};

const u8 S_Box[256] = {
	99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118,
	202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192,
	183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21,
	4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117,
	9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132,
	83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207,
	208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168,
	81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210,
	205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115,
	96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219,
	224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121,
	231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8,
	186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138,
	112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158,
	225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223,
	140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22,
};

const u8 Inv_S_Box[256] = {
	82, 9, 106, 213, 48, 54, 165, 56, 191, 64, 163, 158, 129, 243, 215, 251,
	124, 227, 57, 130, 155, 47, 255, 135, 52, 142, 67, 68, 196, 222, 233, 203,
	84, 123, 148, 50, 166, 194, 35, 61, 238, 76, 149, 11, 66, 250, 195, 78,
	8, 46, 161, 102, 40, 217, 36, 178, 118, 91, 162, 73, 109, 139, 209, 37,
	114, 248, 246, 100, 134, 104, 152, 22, 212, 164, 92, 204, 93, 101, 182, 146,
	108, 112, 72, 80, 253, 237, 185, 218, 94, 21, 70, 87, 167, 141, 157, 132,
	144, 216, 171, 0, 140, 188, 211, 10, 247, 228, 88, 5, 184, 179, 69, 6,
	208, 44, 30, 143, 202, 63, 15, 2, 193, 175, 189, 3, 1, 19, 138, 107,
	58, 145, 17, 65, 79, 103, 220, 234, 151, 242, 207, 206, 240, 180, 230, 115,
	150, 172, 116, 34, 231, 173, 53, 133, 226, 249, 55, 232, 28, 117, 223, 110,
	71, 241, 26, 113, 29, 41, 197, 137, 111, 183, 98, 14, 170, 24, 190, 27,
	252, 86, 62, 75, 198, 210, 121, 32, 154, 219, 192, 254, 120, 205, 90, 244,
	31, 221, 168, 51, 136, 7, 199, 49, 177, 18, 16, 89, 39, 128, 236, 95,
	96, 81, 127, 169, 25, 181, 74, 13, 45, 229, 122, 159, 147, 201, 156, 239,
	160, 224, 59, 77, 174, 42, 245, 176, 200, 235, 187, 60, 131, 83, 153, 97,
	23, 43, 4, 126, 186, 119, 214, 38, 225, 105, 20, 99, 85, 33, 12, 125
};
const u8 Inv_S_Box2[256] = {
	96, 81, 127, 169, 25, 181, 74, 13, 45, 229, 122, 159, 147, 201, 156, 239,
	71, 241, 26, 113, 29, 41, 197, 137, 111, 183, 98, 14, 170, 24, 190, 27,
	114, 248, 246, 100, 134, 104, 152, 22, 212, 164, 92, 204, 93, 101, 182, 146,
	82, 9, 106, 213, 48, 54, 165, 56, 191, 64, 163, 158, 129, 243, 215, 251,
	124, 227, 57, 130, 155, 47, 255, 135, 52, 142, 67, 68, 196, 222, 233, 203,
	84, 123, 148, 50, 166, 194, 35, 61, 238, 76, 149, 11, 66, 250, 195, 78,
	8, 46, 161, 102, 40, 217, 36, 178, 118, 91, 162, 73, 109, 139, 209, 37,
	108, 112, 72, 80, 253, 237, 185, 218, 94, 21, 70, 87, 167, 141, 157, 132,
	144, 216, 171, 0, 140, 188, 211, 10, 247, 228, 88, 5, 184, 179, 69, 6,
	208, 44, 30, 143, 202, 63, 15, 2, 193, 175, 189, 3, 1, 19, 138, 107,
	58, 145, 17, 65, 79, 103, 220, 234, 151, 242, 207, 206, 240, 180, 230, 115,
	150, 172, 116, 34, 231, 173, 53, 133, 226, 249, 55, 232, 28, 117, 223, 110,
	252, 86, 62, 75, 198, 210, 121, 32, 154, 219, 192, 254, 120, 205, 90, 244,
	31, 221, 168, 51, 136, 7, 199, 49, 177, 18, 16, 89, 39, 128, 236, 95,
	160, 224, 59, 77, 174, 42, 245, 176, 200, 235, 187, 60, 131, 83, 153, 97,
	23, 43, 4, 126, 186, 119, 214, 38, 225, 105, 20, 99, 85, 33, 12, 125
};
const u8 cu8_KeyTable[]={
	0xCD, 0xCD, 0x90, 0x2A, 0xBE, 0x3B, 0xA3, 0x02, 0x58, 0x25, 0xB0, 0xCD, 0x8F, 0xF3, 0x45, 0x69,																																																																																			
	0x31, 0x45, 0x6A, 0x32, 0x45, 0x6B, 0x33, 0x45, 0x6C, 0x34, 0x45, 0x65, 0x08, 0x45, 0x66, 0x09,																																																																																			
	0x45, 0x67, 0x0A, 0xAE, 0x00, 0x2D, 0x45, 0x68, 0x0B, 0x89, 0xA6, 0x01, 0xCD, 0x87, 0xA6, 0x45,																																																																																			
	0x08, 0x65, 0x45, 0x09, 0x66, 0x45, 0x0A, 0x67, 0x85, 0x45, 0x0B, 0x68, 0xCC, 0x8D, 0x27, 0xA3,																																																																																			
	0x01, 0xE3, 0x25, 0x03, 0xCC, 0x8C, 0xFC, 0xCD, 0x8F, 0xD1, 0x6B, 0x18, 0xCD, 0x8F, 0xEB, 0x6B,																																																																																			
	0x19, 0xCD, 0x8F, 0xE3, 0x6B, 0x1A, 0xCD, 0x8F, 0xDB, 0x6B, 0x1B, 0x7B, 0x18, 0xB7, 0x91, 0xCD,																																																																																			
	0x90, 0x1F, 0x96, 0x1C, 0x00, 0x09, 0xCD, 0x97, 0x2E, 0x7B, 0x19, 0xB7, 0x91, 0xCD, 0x90, 0x14,																																																																																			
	0x96, 0x1C, 0x00, 0x05, 0xCD, 0x97, 0x2E, 0x7B, 0x1A, 0xCD, 0x90, 0x7D, 0x96, 0x5C, 0xCD, 0x97,																																																																																			
	0x2E, 0x7B, 0x1B, 0xB7, 0x91, 0x3F, 0x90, 0x3F, 0x8F, 0x3F, 0x8E, 0x96, 0x5C, 0xCD, 0x94, 0xFA,																																																																																			
	0x96, 0x1C, 0x00, 0x05, 0xCD, 0x94, 0xFA, 0x96, 0x1C, 0x00, 0x09, 0xCD, 0x94, 0xFA, 0x96, 0xCD,																																																																																			
	0x90, 0x73, 0xCD, 0x90, 0x1F, 0x96, 0xCD, 0x90, 0x69, 0xCD, 0x90, 0x14, 0x96, 0xCD, 0x90, 0x4E,																																																																																			
	0x96, 0xCD, 0x90, 0x5B, 0x96, 0x5C, 0xCD, 0x94, 0xFA, 0x96, 0x1C, 0x00, 0x05, 0xCD, 0x94, 0xFA,																																																																																			
	0x96, 0x1C, 0x00, 0x09, 0xCD, 0x94, 0xFA, 0x96, 0x1C, 0x00, 0x10, 0xCD, 0x97, 0x2E, 0x7B, 0x14,																																																																																			
	0xA4, 0x7F, 0x6B, 0x14, 0x7B, 0x10, 0xA4, 0x7F, 0x6B, 0x10, 0x96, 0x1C, 0x00, 0x14, 0xCD, 0x95,																																																																																			
	0xE2, 0xAE, 0x81, 0x88, 0xCD, 0x95, 0x12, 0x25, 0x3E, 0x96, 0x1C, 0x00, 0x10, 0xCD, 0x95, 0xE2,																																																																																			
	0x96, 0x1C, 0x00, 0x14, 0xCD, 0x95, 0x12, 0x22, 0x2E, 0xCC, 0x8A, 0xE0, 0xA3, 0x01, 0xE4, 0x25,																																																																																			
	0xF8, 0xA3, 0x01, 0xF4, 0x24, 0x1E, 0xCD, 0x8F, 0xD1, 0xBA, 0x65, 0xB7, 0x65, 0xCD, 0x8F, 0xEB 																																																																																			
};
const u8 Shifts128[4][2] =
{
	{0, 0},
	{1, 3},
	{2, 2},
	{3, 1}
};
const u8 RCON[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
u8 ucChallenge[4][4];
u8 ucKey[4][4],RCON_Count;


#define rom_tbl const
//static rom_tbl u8	tbl_key[]={'U','C','F','O','B','0','0','0'};
//static rom_tbl u8	tbl_key[]={'U','C','F','O','B','0','0','0'};
static rom_tbl u8	eNLF[]=
{
	0,1,1,1,0,1,0,0,
	0,0,1,0,1,1,1,0,
	0,0,1,1,1,0,1,0,
	0,1,0,1,1,1,0,0
};



void AddRoundKey(void)
{ // Exor corresponding text input and round key input bytes
	u8 i, j;
	for (i=0; i<4; i++)
	for (j= 0; j<BC; j++)
	ucChallenge[i][j] ^= ucKey[i][j];
}
void SubBytes(void)
{ // Replace every byte of the input by the byte at that place
	// in the nonlinear S-box.
	// This routine implements SubBytes and InvSubBytes
	u8 i, j;
	for (i=0; i<4; i++)
	for (j=0; j<BC; j++)
	ucChallenge[i][j] = S_Box[ucChallenge[i][j]];
}

void ShiftRows(void)
{ // Row 0 remains unchanged
	// The other three rows are shifted a variable amount
	u8 ucTemp;
	// rotate over 1 byte
	ucTemp = ucChallenge[0][1]; // set temp value
	ucChallenge[0][1] = ucChallenge[1][1];
	ucChallenge[1][1] = ucChallenge[2][1];
	ucChallenge[2][1] = ucChallenge[3][1];
	ucChallenge[3][1] = ucTemp;
	// rotate over 2 bytes
	ucTemp = ucChallenge[0][2]; // set temp value
	ucChallenge[0][2] = ucChallenge[2][2];
	ucChallenge[2][2] = ucTemp;
	ucTemp = ucChallenge[1][2]; // set temp value
	ucChallenge[1][2] = ucChallenge[3][2];
	ucChallenge[3][2] = ucTemp;
	// rotate over 3 bytes
	ucTemp = ucChallenge[1][3]; // set temp value
	ucChallenge[1][3] = ucChallenge[0][3];
	ucChallenge[0][3] = ucChallenge[3][3];
	ucChallenge[3][3] = ucChallenge[2][3];
	ucChallenge[2][3] = ucTemp;
}
u8 modMult02(u8 x)
{
	union unBitConvert Input,Output;
	Input.ucValue = x;
	Output.BitValue.Bit0 = Input.BitValue.Bit7;
	Output.BitValue.Bit7 = Input.BitValue.Bit6;
	Output.BitValue.Bit6 = Input.BitValue.Bit5;
	Output.BitValue.Bit5 = Input.BitValue.Bit4;
	Output.BitValue.Bit2 = Input.BitValue.Bit1;
	Output.BitValue.Bit1 = Input.BitValue.Bit0 ^ Input.BitValue.Bit7;
	Output.BitValue.Bit3 = Input.BitValue.Bit2 ^ Input.BitValue.Bit7;
	Output.BitValue.Bit4 = Input.BitValue.Bit3 ^ Input.BitValue.Bit7;
	return Output.ucValue;
}
u8 modMult03(u8 x)
{
	return (x ^ modMult02(x));
}

void MixColumns(void)
{ // Mix the four bytes of every column in a linear way
	u8 MixResult[4][4],i,j;
	for (i=0; i<4; i++)
	{
	MixResult[i][0] = modMult02(ucChallenge[i][0]) ^ modMult03(ucChallenge[i][1]) ^ ucChallenge[i][2] ^
	ucChallenge[i][3];
	MixResult[i][1] = modMult02(ucChallenge[i][1]) ^ modMult03(ucChallenge[i][2]) ^ ucChallenge[i][3] ^
	ucChallenge[i][0];
	MixResult[i][2] = modMult02(ucChallenge[i][2]) ^ modMult03(ucChallenge[i][3]) ^ ucChallenge[i][0] ^
	ucChallenge[i][1];
	MixResult[i][3] = modMult02(ucChallenge[i][3]) ^ modMult03(ucChallenge[i][0]) ^ ucChallenge[i][1] ^
	ucChallenge[i][2];
	}
	for (i=0 ; i<4 ; i++)
	for (j=0 ; j<4 ; j++)
	ucChallenge[j][i] = MixResult[j][i];
}
void KeySchedule(void)
{u8 CipherResult[4][4],i,j;
	CipherResult[3][0] = ucKey[3][1]; // Rotate last col of Key
	CipherResult[3][1] = ucKey[3][2];
	CipherResult[3][2] = ucKey[3][3];
	CipherResult[3][3] = ucKey[3][0];
	for (i=0; i<4; i++) // Add Key and RCON to last col
	{
	CipherResult[3][i] = S_Box[CipherResult[3][i]]; // substitute last col with sbox
	if (i)
	CipherResult[0][i] = ucKey[0][i] ^ CipherResult[3][i];
	else
	CipherResult[0][i] = ucKey[0][i] ^ CipherResult[3][i] ^ RCON[RCON_Count];
	CipherResult[1][i] = ucKey[1][i] ^ CipherResult[0][i]; // Add second col = Key 2 col + Result 1 col
	CipherResult[2][i] = ucKey[2][i] ^ CipherResult[1][i];
	CipherResult[3][i] = ucKey[3][i] ^ CipherResult[2][i];
	}
	for (i=0; i<4; i++)
	for (j=0; j<4; j++)
	{
	ucKey[i][j] = CipherResult[i][j];
	}
	RCON_Count++;
}

void AES128_Encrypt(u8 *ucInput_Chal, u8 *ucInputKey)
{	
	u8 i,k;
	RCON_Count = 0;
	for (i=0; i<4;i++)
	for (k=0; k<4;k++)
	{
	ucChallenge[i][k] = ucInput_Chal[i*4+k];
	ucKey[i][k] = ucInputKey[i*4+k];
	}
	AddRoundKey();
	for (i=0; i<9 ; i++)
	{
	SubBytes();
	ShiftRows();
	MixColumns();
	KeySchedule();
	AddRoundKey();
	}
	SubBytes();
	ShiftRows();
	//Not at last round MixColumns();
	KeySchedule();
	AddRoundKey();
	for (i=0; i<4;i++)
	for (k=0; k<4;k++)
	{
	ucInput_Chal[i*4+k] = ucChallenge[i][k];
	}
}

void InvShiftRows(void)
{// Row 0 remains unchanged
// The other three rows are shifted a variable amount
	u8 ucTemp;
	// rotate over 1 byte
	ucTemp = ucChallenge[0][1]; // set temp value
	ucChallenge[0][1] = ucChallenge[3][1];
	ucChallenge[3][1] = ucChallenge[2][1];
	ucChallenge[2][1] = ucChallenge[1][1];
	ucChallenge[1][1] = ucTemp;
	// rotate over 2 bytes
	ucTemp = ucChallenge[0][2]; // set temp value
	ucChallenge[0][2] = ucChallenge[2][2];
	ucChallenge[2][2] = ucTemp;
	ucTemp = ucChallenge[1][2]; // set temp value
	ucChallenge[1][2] = ucChallenge[3][2];
	ucChallenge[3][2] = ucTemp;
	// rotate over 3 bytes
	ucTemp = ucChallenge[0][3]; // set temp value
	ucChallenge[0][3] = ucChallenge[1][3];
	ucChallenge[1][3] = ucChallenge[2][3];
	ucChallenge[2][3] = ucChallenge[3][3];
	ucChallenge[3][3] = ucTemp;
}
void InvSubBytes(void)
{ // Replace every byte of the input by the byte at that place
	// in the nonlinear S-box.
	// This routine implements SubBytes and InvSubBytes
	u8 i, j;
	for (i=0; i<4; i++)
	for (j=0; j<BC; j++)
	ucChallenge[i][j] = Inv_S_Box[ucChallenge[i][j]];
}
u8 modMult04(u8 x)
{
	return (modMult02(modMult02(x)));
}
u8 modMult08(u8 x)
{
	return (modMult02(modMult02(modMult02(x))));
}
u8 modMult09(u8 x)
{	// 9 = 1 + 8
	return (x ^ modMult08(x));
}
u8 modMult0B(u8 x)
{	// 11 = 8 + 2 + 1
	return (modMult08(x) ^ modMult02(x) ^ x);
}
u8 modMult0D(u8 x)
{	// 13 = 8 + 4 + 1
	return (modMult08(x) ^ modMult04(x) ^ x);
}
u8 modMult0E(u8 x)
{	// 14 = 8 + 4 + 2
	return (modMult08(x) ^ modMult04(x) ^ modMult02(x));
}

void InvMixColumns(void)
{ // Mix the four bytes of every column in a linear way
	u8 MixResult[4][4],i,j;
	for (i=0; i<4; i++)
	{
	MixResult[i][0] = modMult0E(ucChallenge[i][0]) ^ modMult09(ucChallenge[i][3]) ^
	modMult0D(ucChallenge[i][2]) ^ modMult0B(ucChallenge[i][1]);
	MixResult[i][1] = modMult0E(ucChallenge[i][1]) ^ modMult09(ucChallenge[i][0]) ^
	modMult0D(ucChallenge[i][3]) ^ modMult0B(ucChallenge[i][2]);
	MixResult[i][2] = modMult0E(ucChallenge[i][2]) ^ modMult09(ucChallenge[i][1]) ^
	modMult0D(ucChallenge[i][0]) ^ modMult0B(ucChallenge[i][3]);
	MixResult[i][3] = modMult0E(ucChallenge[i][3]) ^ modMult09(ucChallenge[i][2]) ^
	modMult0D(ucChallenge[i][1]) ^ modMult0B(ucChallenge[i][0]);
	}
	for ( i=0 ; i<4 ; i++)
	for (j=0 ; j<4 ; j++)
	ucChallenge[j][i] = MixResult[j][i];
}

void AES128_Decrypt(u8 *ucInput_Chal, u8 *ucInputKey)
{
	u8 i,k,m;
	RCON_Count = 0;
	for (i=0; i<4;i++)
	for (k=0; k<4;k++)
	{
	ucChallenge[i][k] = ucInput_Chal[i*4+k];
	ucKey[i][k] = ucInputKey[i*4+k];
	}
	for (i=0; i<10;i++)
	{
	KeySchedule();
	}
	AddRoundKey();
	for (i=0; i<9 ; i++)
	{
	InvShiftRows();
	InvSubBytes();
	RCON_Count = 0;
	for (m=0; m<4;m++)
	for (k=0; k<4;k++)
	{
	ucKey[m][k] = ucInputKey[m*4+k];
	}
	for (k=i+1; k<10;k++)
	{
	KeySchedule();
	}
	AddRoundKey();
	InvMixColumns();
	}
	InvShiftRows();
	InvSubBytes();
	for (m=0; m<4;m++)
	for (k=0; k<4;k++)
	{
	ucKey[m][k] = ucInputKey[m*4+k];
	}
	AddRoundKey();
	for (i=0; i<4;i++)
	for (k=0; k<4;k++)
	{
	ucInput_Chal[i*4+k] = ucChallenge[i][k];
	}
}


/*
void main(void)
{
	u8 i;
	for (i=0;i<16;i++)
	{
		ucC[i] = ucK[i] = 0x0F - (0x01 * i);
	}
	AES128_Encrypt(ucC,ucK); //ucC writes back the calculated signature
	AES128_Decrypt(ucC,ucK); //writes back the calculated signature
	ucC[0] = ucK[0];
}
*/





void EncryptionPassiveRcv(u8 *EncData, u16 random)
{
	u8	gen_key[8];
	
	u8	KeyIndex;
	u8	NlfIndex;
	u8	XorIndex;
	u8	KeyBuff;
	u8	Count1,Count2;
	
	gen_key[0] = gen_key[2] = random & 0xff;
	gen_key[1] = gen_key[3] = random >> 8;
	gen_key[4] = EncData[0];
	gen_key[5] = EncData[1];
	gen_key[6] = EncData[2];
	gen_key[7] = EncData[3];
	
	Count2=(144/8);
	KeyIndex=0;
	
	do{
		KeyBuff = gen_key[KeyIndex];
		Count1 = 8;
		do{
			NlfIndex = (EncData[0]&2) ? 1 : 0;
			if( EncData[1] & 2 )	NlfIndex |= 2;
			if( EncData[2] & 16 )	NlfIndex |= 4;
			if( EncData[3] & 4 )	NlfIndex |= 8;
			if( EncData[3] & 128 )	NlfIndex |= 16;
		
			XorIndex = eNLF[NlfIndex] ^ EncData[0] ^ EncData[2] ^ KeyBuff;
			
			KeyBuff >>= 1;
			if(EncData[0]&1)	{KeyBuff|=0x80u;}
			EncData[0] >>= 1;
			if(EncData[1]&1)	{EncData[0]|=0x80u;}
			EncData[1] >>= 1;
			if(EncData[2]&1)	{EncData[1]|=0x80u;}
			EncData[2] >>= 1;
			if(EncData[3]&1)	{EncData[2]|=0x80u;}
			EncData[3] >>= 1;
			if(XorIndex&1)		{EncData[3]|=0x80u;}
		}while(--Count1);
		KeyIndex++;
		KeyIndex &= 7;
	}while(--Count2);
}
#if 0
void EncryptionPassiveTrs(u8 *EncData)
{
	u8	KeyIndex;
	u8	NlfIndex;
	u8	XorIndex;
	u8	KeyBuff;
	u8	Count1,Count2;
	
	Count2=(144/8);
	KeyIndex=0;
	GetENCRYPTION_KEY();
	do{
		KeyBuff = ENCRYPTION_KEY[KeyIndex];
		Count1 = 8;
		do{
			NlfIndex = (EncData[0]&2) ? 1 : 0;
			if( EncData[1] & 2 )	NlfIndex |= 2;
			if( EncData[2] & 16 )	NlfIndex |= 4;
			if( EncData[3] & 4 )	NlfIndex |= 8;
			if( EncData[3] & 128 )	NlfIndex |= 16;
		
			XorIndex = eNLF[NlfIndex] ^ EncData[0] ^ EncData[2] ^ KeyBuff;
			
			KeyBuff >>= 1;
			if(EncData[0]&1)	{KeyBuff|=0x80u;}
			EncData[0] >>= 1;
			if(EncData[1]&1)	{EncData[0]|=0x80u;}
			EncData[1] >>= 1;
			if(EncData[2]&1)	{EncData[1]|=0x80u;}
			EncData[2] >>= 1;
			if(EncData[3]&1)	{EncData[2]|=0x80u;}
			EncData[3] >>= 1;
			if(XorIndex&1)		{EncData[3]|=0x80u;}
		}while(--Count1);
		KeyIndex++;
		KeyIndex &= 7;
	}while(--Count2);
}
#endif
static rom_tbl u8	dNLF[]=
{
	0x00,0x80,0x80,0x80,0x00,0x80,0x00,0x00,
	0x00,0x00,0x80,0x00,0x80,0x80,0x80,0x00,
	0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x00,
	0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x00
};
#if 0
void DecryptionRKE(u8 *RcvData)
{
	u8	KeyIndex;
	u8	NlfIndex;
	u8	XorIndex;
	u8	KeyBuff;
	u8	Count1,Count2;

	Count2 = (144/8);
	KeyIndex = 1;
	GetENCRYPTION_KEY();

	do{
		/*
		gcDecryptionStep	: 0 1 2 3 4 ....
		Index				: 1 0 7 6 5 ....
		start index = 1
		*/
		KeyBuff = ENCRYPTION_KEY[KeyIndex];
		Count1 = 8;
		do{
			NlfIndex = RcvData[0] & 1;
			if( RcvData[1] & 1 )	NlfIndex |= 2;
			if( RcvData[2] & 8 )	NlfIndex |= 4;
			if( RcvData[3] & 2 )	NlfIndex |= 8;
			if( RcvData[3] & 64 )	NlfIndex |= 16;	
			
			XorIndex = dNLF[NlfIndex] ^ RcvData[1] ^ RcvData[3] ^ KeyBuff;	
			
			KeyBuff <<= 1;
			if(RcvData[3]&0x80u)	{KeyBuff|=1;}
			RcvData[3] <<= 1;
			if(RcvData[2]&0x80u)	{RcvData[3]|=1;}
			RcvData[2] <<= 1;
			if(RcvData[1]&0x80u)	{RcvData[2]|=1;}
			RcvData[1] <<= 1;
			if(RcvData[0]&0x80u)	{RcvData[1]|=1;}
			RcvData[0] <<= 1;
			if(XorIndex&0x80u)		{RcvData[0]|=1;}
		}while( --Count1 );
		KeyIndex--;
		KeyIndex &= 7;
	}while( --Count2 );
}
#endif

