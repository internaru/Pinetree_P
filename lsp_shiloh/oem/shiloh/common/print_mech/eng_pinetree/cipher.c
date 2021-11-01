/******************** (C) COPYRIGHT 2009 PLATO CO., LTD. ***********************
* Project			 : Smart IC test project
* Customer			 : SindoH
* Device			 : STM32F10x
* Compiler			 :
* File Name          : main.h
* Author             : 1 team, BR GROUP, PLATO CO., LTD.
*
*
* Description        :
*
* Revision history -------------------------------------------------------------
Version 		Issued date					Changes
--------------------------------------------------------------------------------
preliminary     2009-09-01

*******************************************************************************/


/* Global variable */
#include <stdlib.h>
#include <string.h>

#define extern
#include "cipher.h"
#undef	extern

typedef	unsigned char u8;
typedef	unsigned short u16;
typedef	unsigned int u32;

//#define	DST80


/* Serial Number 2byte */
#if 0	/* @@2013.01.11. By CSPark */
const u8	u8_SerialNr[2] = { 'p','l' };
#else	/* @@2013.01.11. By CSPark */
extern const u8	u8_SerialNr[];
#endif	/* @@2013.01.11. By CSPark */

/* 카운터는 하위 24비트만 사용 */
u32	u32_Counter = 0x00000000;
/* 128비트 Key 테이블 */

/* Security chip에 보낼 128비트 데이터 버퍼 */
u8	u8_I2CSendData[16];
/* Security chip으로부터 받을 ACK 버퍼 */
u8	u8_ACK[3];
/* 테스트 목적으로 사용된 버퍼임. Printer에서는 사용안됨. */
u8	u8_DecData1[4], u8_DecData2[4];

#if 0		/* @@2013.01.11. By CSPark */
const u8	u8_Key[16]=
{
	'S','I','N','D','O','S','m','a',
	'r','t','I','C',0xae,0x25,0x44,0x55
};
#else	/* @@2013.01.11. By CSPark */
extern const u8 u8_key[16];
#endif	/* @@2013.01.11. By CSPark */

#if defined(DST80)
	u8 brByte(u8 x);
	u8 lower4upper4(u8 x1, u8 x2);
	u8 upper4upper4(u8 x1, u8 x2);
#endif

#define	kBit0			0x0001U
#define	kBit1			0x0002U
#define	kBit2			0x0004U
#define	kBit3			0x0008U
#define	kBit4			0x0010U
#define	kBit5			0x0020U
#define	kBit6			0x0040U
#define	kBit7			0x0080U
#define	kBit8			0x0100U
#define	kBit9			0x0200U
#define	kBitA			0x0400U
#define	kBitB			0x0800U
#define	kBitC			0x1000U
#define	kBitD			0x2000U
#define	kBitE			0x4000U
#define	kBitF			0x8000U



static const u8 f1a[]=
{
	8,0,8,0,0,0,8,8,
	0,0,8,8,0,8,0,8,
	8,0,8,0,8,8,0,0,
	0,8,0,8,8,8,0,0
};
static const u8 f1b[]=
{
	0,4,4,4,0,4,0,0,
	0,0,4,0,4,4,4,0,
	4,0,4,0,4,4,0,0,
	0,0,4,4,0,4,0,4
};
static const u8 f1c[]=
{
	2,0,0,0,2,0,2,2,
	2,2,0,2,0,0,0,2,
	2,0,2,2,2,0,0,0,
	0,0,0,2,2,2,0,2
};
static const u8 f1d[]=
{
	0,1,0,1,1,0,1,0,
	1,1,0,0,1,1,0,0,
	0,0,1,1,0,0,1,1,
	0,1,0,1,1,0,1,0
};
static const u8 f1e1[]=
{
	8,8,8,0,
	0,0,8,0,
	0,8,0,0,
	0,8,8,8
};
static const u8 f1e2[]=
{
	2,2,2,0,
	0,0,2,0,
	0,2,0,0,
	0,2,2,2
};
static const u8 f2[]=
{
	0,1,0,0,
	1,1,1,0,
	0,1,1,1,
	0,0,1,0
};
static const u8 f3[]=
{
	0,1,3,3,2,1,2,0,
	0,2,1,2,3,3,1,0,
	0,1,1,1,0,0,1,0
};

#if	defined(DST80)

u8 brByte(u8 x)
{
	u8	y = x & 0xa5u;

	if( (x & kBit6)!=0 )	y |= kBit3;
	if( (x & kBit4)!=0 )	y |= kBit1;
	if( (x & kBit3)!=0 )	y |= kBit6;
	if( (x & kBit1)!=0 )	y |= kBit4;

	return	y;
}

u8 lower4upper4(u8 x1, u8 x2)
{
	u8	y;

	y =  (x2>>4) & 0x0fu;
	y |= (x1<<4) & 0xf0u;

	return	y;
}

u8 upper4upper4(u8 x1, u8 x2)
{
	u8	y;

	y =  (x2>>4) & 0x0fu;
	y |= x1 & 0xf0u;

	return	y;
}
#endif

static	u8	Dst_Count,Dst_Round;
static	u8	Dst_X[5],Dst_K[5],Dst_K0[5],Dst_K1[5];

void EncMachine2(u8 * Key, u8 * Challange)
{
	u8	Data[4];
	u8	i,loop_cnt;
#if	defined(DST80)
	u8	q0,q1,q2,q3,q4, q2r0,q2r1,q2r2,q2r3,q2r4;
#endif
	Dst_Count = 2;
	Dst_Round = 200;

	Dst_K0[0] = Key[0];
	Dst_K0[1] = Key[1];
	Dst_K0[2] = Key[2];
	Dst_K0[3] = Key[3];
	Dst_K0[4] = Key[4];
	Dst_K1[0] = Key[5];
	Dst_K1[1] = Key[6];
	Dst_K1[2] = Key[7];
	Dst_K1[3] = Key[8];
	Dst_K1[4] = Key[9];

	Dst_X[0] = Challange[0];
	Dst_X[1] = Challange[1];
	Dst_X[2] = Challange[2];
	Dst_X[3] = Challange[3];
	Dst_X[4] = Challange[4];

	loop_cnt = 10;
	do{
	#if	defined(DST80)
		q0 = Dst_K0[0];
		q1 = Dst_K0[1];
		q2 = Dst_K0[2];
		q3 = Dst_K0[3];
		q4 = Dst_K0[4];

		if( q4 & 0x80 )
		{
			q4 = ~q4;
			q3 = ~q3;
			q2 = ~q2;
			q1 = ~q1;
			q0 = ~q0;
			q4 =  q4 | 0x80;
		}

		q4 = brByte(q4);
		q3 = brByte(q3);
		q2 = brByte(q2);
		q1 = brByte(q1);
		q0 = brByte(q0);

		q2r0 = Dst_K1[0];
		q2r1 = Dst_K1[1];
		q2r2 = Dst_K1[2];
		q2r3 = Dst_K1[3];
		q2r4 = Dst_K1[4];
		if( q2r4 & 0x80 )
		{
			q2r4 = ~q2r4;
			q2r3 = ~q2r3;
			q2r2 = ~q2r2;
			q2r1 = ~q2r1;
			q2r0 = ~q2r0;
			q2r4 =  q2r4 | 0x80;
		}

		q2r4 = brByte(q2r4);
		q2r3 = brByte(q2r3);
		q2r2 = brByte(q2r2);
		q2r1 = brByte(q2r1);
		q2r0 = brByte(q2r0);
		Dst_K[0] = lower4upper4(  q3,  q2);
		Dst_K[1] = lower4upper4(  q4,  q3);
		Dst_K[2] = upper4upper4(q2r2,  q4);
		Dst_K[3] = q2r3;
		Dst_K[4] = q2r4;
	#else	/* DST40 */

		Dst_K[0] = Dst_K0[0];
		Dst_K[1] = Dst_K0[1];
		Dst_K[2] = Dst_K0[2];
		Dst_K[3] = Dst_K0[3];
		Dst_K[4] = Dst_K0[4];

	#endif

		i=0;

		Data[0]=0;
		Data[1]=0;
		Data[2]=0;
		Data[3]=0;
		if( Dst_X[4]&kBit0 )	Data[3] |= kBit4;
		if( Dst_K[4]&kBit0 )	Data[3] |= kBit3;
		if( Dst_X[3]&kBit0 )	Data[3] |= kBit2;
		if( Dst_K[3]&kBit0 )	Data[3] |= kBit1;
		if( Dst_X[2]&kBit0 )	Data[3] |= kBit0;
		if( Dst_K[2]&kBit0 )	Data[2] |= kBit3;
		if( Dst_X[1]&kBit0 )	Data[2] |= kBit2;
		if( Dst_K[1]&kBit0 )	Data[2] |= kBit1;
		if( Dst_K[0]&kBit0 )	Data[2] |= kBit0;
		if( Dst_X[4]&kBit1 )	Data[1] |= kBit4;
		if( Dst_K[4]&kBit1 )	Data[1] |= kBit3;
		if( Dst_X[3]&kBit1 )	Data[1] |= kBit2;
		if( Dst_K[3]&kBit1 )	Data[1] |= kBit1;
		if( Dst_X[2]&kBit1 )	Data[1] |= kBit0;
		if( Dst_K[2]&kBit1 )	Data[0] |= kBit3;
		if( Dst_X[1]&kBit1 )	Data[0] |= kBit2;
		if( Dst_K[1]&kBit1 )	Data[0] |= kBit1;
		if( Dst_K[0]&kBit1 )	Data[0] |= kBit0;

		if( f2[ f1e1[Data[0]]|f1b[Data[1]]|f1e2[Data[2]]|f1d[Data[3]] ])
		i|=kBit3;

		Data[0]=0;
		Data[1]=0;
		Data[2]=0;
		Data[3]=0;
		if( Dst_X[4]&kBit2 )	Data[3] |= kBit4;
		if( Dst_K[4]&kBit2 )	Data[3] |= kBit3;
		if( Dst_X[3]&kBit2 )	Data[3] |= kBit2;
		if( Dst_K[3]&kBit2 )	Data[3] |= kBit1;
		if( Dst_X[2]&kBit2 )	Data[3] |= kBit0;
		if( Dst_K[2]&kBit2 )	Data[2] |= kBit4;
		if( Dst_X[1]&kBit2 )	Data[2] |= kBit3;
		if( Dst_K[1]&kBit2 )	Data[2] |= kBit2;
		if( Dst_X[0]&kBit2 )	Data[2] |= kBit1;
		if( Dst_K[0]&kBit2 )	Data[2] |= kBit0;
		if( Dst_X[4]&kBit3 )	Data[1] |= kBit4;
		if( Dst_K[4]&kBit3 )	Data[1] |= kBit3;
		if( Dst_X[3]&kBit3 )	Data[1] |= kBit2;
		if( Dst_K[3]&kBit3 )	Data[1] |= kBit1;
		if( Dst_X[2]&kBit3 )	Data[1] |= kBit0;
		if( Dst_K[2]&kBit3 )	Data[0] |= kBit4;
		if( Dst_X[1]&kBit3 )	Data[0] |= kBit3;
		if( Dst_K[1]&kBit3 )	Data[0] |= kBit2;
		if( Dst_X[0]&kBit3 )	Data[0] |= kBit1;
		if( Dst_K[0]&kBit3 )	Data[0] |= kBit0;

		if( f2[ f1a[Data[0]]|f1b[Data[1]]|f1c[Data[2]]|f1d[Data[3]] ])
		i|=kBit2;

		Data[0]=0;
		Data[1]=0;
		Data[2]=0;
		Data[3]=0;
		if( Dst_X[4]&kBit4 )	Data[3] |= kBit4;
		if( Dst_K[4]&kBit4 )	Data[3] |= kBit3;
		if( Dst_X[3]&kBit4 )	Data[3] |= kBit2;
		if( Dst_K[3]&kBit4 )	Data[3] |= kBit1;
		if( Dst_X[2]&kBit4 )	Data[3] |= kBit0;
		if( Dst_K[2]&kBit4 )	Data[2] |= kBit4;
		if( Dst_X[1]&kBit4 )	Data[2] |= kBit3;
		if( Dst_K[1]&kBit4 )	Data[2] |= kBit2;
		if( Dst_X[0]&kBit4 )	Data[2] |= kBit1;
		if( Dst_K[0]&kBit4 )	Data[2] |= kBit0;
		if( Dst_X[4]&kBit5 )	Data[1] |= kBit4;
		if( Dst_K[4]&kBit5 )	Data[1] |= kBit3;
		if( Dst_X[3]&kBit5 )	Data[1] |= kBit2;
		if( Dst_K[3]&kBit5 )	Data[1] |= kBit1;
		if( Dst_X[2]&kBit5 )	Data[1] |= kBit0;
		if( Dst_K[2]&kBit5 )	Data[0] |= kBit4;
		if( Dst_X[1]&kBit5 )	Data[0] |= kBit3;
		if( Dst_K[1]&kBit5 )	Data[0] |= kBit2;
		if( Dst_X[0]&kBit5 )	Data[0] |= kBit1;
		if( Dst_K[0]&kBit5 )	Data[0] |= kBit0;

		if( f2[ f1a[Data[0]]|f1b[Data[1]]|f1c[Data[2]]|f1d[Data[3]] ])
		i|=kBit1;

		Data[0]=0;
		Data[1]=0;
		Data[2]=0;
		Data[3]=0;
		if( Dst_X[4]&kBit6 )	Data[3] |= kBit4;
		if( Dst_K[4]&kBit6 )	Data[3] |= kBit3;
		if( Dst_X[3]&kBit6 )	Data[3] |= kBit2;
		if( Dst_K[3]&kBit6 )	Data[3] |= kBit1;
		if( Dst_X[2]&kBit6 )	Data[3] |= kBit0;
		if( Dst_K[2]&kBit6 )	Data[2] |= kBit4;
		if( Dst_X[1]&kBit6 )	Data[2] |= kBit3;
		if( Dst_K[1]&kBit6 )	Data[2] |= kBit2;
		if( Dst_X[0]&kBit6 )	Data[2] |= kBit1;
		if( Dst_K[0]&kBit6 )	Data[2] |= kBit0;
		if( Dst_X[4]&kBit7 )	Data[1] |= kBit4;
		if( Dst_K[4]&kBit7 )	Data[1] |= kBit3;
		if( Dst_X[3]&kBit7 )	Data[1] |= kBit2;
		if( Dst_K[3]&kBit7 )	Data[1] |= kBit1;
		if( Dst_X[2]&kBit7 )	Data[1] |= kBit0;
		if( Dst_K[2]&kBit7 )	Data[0] |= kBit4;
		if( Dst_X[1]&kBit7 )	Data[0] |= kBit3;
		if( Dst_K[1]&kBit7 )	Data[0] |= kBit2;
		if( Dst_X[0]&kBit7 )	Data[0] |= kBit1;
		if( Dst_K[0]&kBit7 )	Data[0] |= kBit0;

		if( f2[ f1a[Data[0]]|f1b[Data[1]]|f1c[Data[2]]|f1d[Data[3]] ])
		i|=kBit0;

		i = f3[i] ^ Dst_X[0];

		Dst_X[0] >>= 2;
		Dst_X[0] |= Dst_X[1]<<6;
		Dst_X[1] >>= 2;
		Dst_X[1] |= Dst_X[2]<<6;
		Dst_X[2] >>= 2;
		Dst_X[2] |= Dst_X[3]<<6;
		Dst_X[3] >>= 2;
		Dst_X[3] |= Dst_X[4]<<6;
		Dst_X[4] >>= 2;
		Dst_X[4] |= i<<6;

	#if	defined(DST80)

		Dst_Count = 0;
		i=0;
		if( Dst_K0[0]&kBit0 )	i++;
		if( Dst_K0[0]&kBit2 )	i++;
		if( Dst_K0[2]&kBit3 )	i++;
		if( Dst_K0[2]&kBit5 )	i++;

		Dst_K0[0] >>= 1;
		Dst_K0[0] |= Dst_K0[1]<<7;
		Dst_K0[1] >>= 1;
		Dst_K0[1] |= Dst_K0[2]<<7;
		Dst_K0[2] >>= 1;
		Dst_K0[2] |= Dst_K0[3]<<7;
		Dst_K0[3] >>= 1;
		Dst_K0[3] |= Dst_K0[4]<<7;
		Dst_K0[4] >>= 1;
		Dst_K0[4] |= i<<7;

		i=0;
		if( Dst_K1[0]&kBit0 )	i++;
		if( Dst_K1[0]&kBit2 )	i++;
		if( Dst_K1[2]&kBit3 )	i++;
		if( Dst_K1[2]&kBit5 )	i++;

		Dst_K1[0] >>= 1;
		Dst_K1[0] |= Dst_K1[1]<<7;
		Dst_K1[1] >>= 1;
		Dst_K1[1] |= Dst_K1[2]<<7;
		Dst_K1[2] >>= 1;
		Dst_K1[2] |= Dst_K1[3]<<7;
		Dst_K1[3] >>= 1;
		Dst_K1[3] |= Dst_K1[4]<<7;
		Dst_K1[4] >>= 1;
		Dst_K1[4] |= i<<7;

	#else	/* DST40 */

		Dst_Count--;
		if( Dst_Count == 0 )
		{
			i=0;
			if( Dst_K0[0]&kBit0 )	i++;
			if( Dst_K0[0]&kBit2 )	i++;
			if( Dst_K0[2]&kBit3 )	i++;
			if( Dst_K0[2]&kBit5 )	i++;

			Dst_K0[0] >>= 1;
			Dst_K0[0] |= Dst_K0[1]<<7;
			Dst_K0[1] >>= 1;
			Dst_K0[1] |= Dst_K0[2]<<7;
			Dst_K0[2] >>= 1;
			Dst_K0[2] |= Dst_K0[3]<<7;
			Dst_K0[3] >>= 1;
			Dst_K0[3] |= Dst_K0[4]<<7;
			Dst_K0[4] >>= 1;
			Dst_K0[4] |= i<<7;

			Dst_Count=3;
		}
	#endif

		Dst_Round--;
		loop_cnt--;
	}while(Dst_Round);

	Challange[0] = Dst_X[0];
	Challange[1] = Dst_X[1];
	Challange[2] = Dst_X[2];
	Challange[3] = Dst_X[3];
	Challange[4] = Dst_X[4];
}




const u8	eNLF[]=
{
	0,1,1,1,0,1,0,0,
	0,0,1,0,1,1,1,0,
	0,0,1,1,1,0,1,0,
	0,1,0,1,1,1,0,0
};


void EncMachine1(u8 *Key, u8 *EncData)
{
	u8	KeyIndex;
	u8	NlfIndex;
	u8	XorIndex;
	u8	KeyBuff;
	u8	Count1;
	u16 Count2;

	Count2=(144/8);
	KeyIndex=0;

	do{
		KeyBuff = Key[KeyIndex];
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



const u8	dNLF[]=
{
	0x00,0x80,0x80,0x80,0x00,0x80,0x00,0x00,
	0x00,0x00,0x80,0x00,0x80,0x80,0x80,0x00,
	0x00,0x00,0x80,0x80,0x80,0x00,0x80,0x00,
	0x00,0x80,0x00,0x80,0x80,0x80,0x00,0x00
};

void DecMachine1(u8 *Key, u8 *DecData)
{
	u8	KeyIndex;
	u8	NlfIndex;
	u8	XorIndex;
	u8	KeyBuff;
	u8	Count1;
	u16 Count2;

	 Count2 = (144/8);
	KeyIndex = 1;

	do{
		KeyBuff = Key[KeyIndex];
		Count1 = 8;
		do{
			NlfIndex = DecData[0] & 1;
			if( DecData[1] & 1 )	NlfIndex |= 2;
			if( DecData[2] & 8 )	NlfIndex |= 4;
			if( DecData[3] & 2 )	NlfIndex |= 8;
			if( DecData[3] & 64 )	NlfIndex |= 16;

			XorIndex = dNLF[NlfIndex] ^ DecData[1] ^ DecData[3] ^ KeyBuff;

			KeyBuff <<= 1;
			if(DecData[3]&0x80u)	{KeyBuff|=1;}
			DecData[3] <<= 1;
			if(DecData[2]&0x80u)	{DecData[3]|=1;}
			DecData[2] <<= 1;
			if(DecData[1]&0x80u)	{DecData[2]|=1;}
			DecData[1] <<= 1;
			if(DecData[0]&0x80u)	{DecData[1]|=1;}
			DecData[0] <<= 1;
			if(XorIndex&0x80u)		{DecData[0]|=1;}
		}while( --Count1 );
		KeyIndex--;
		KeyIndex &= 7;
	}while( --Count2 );
}

