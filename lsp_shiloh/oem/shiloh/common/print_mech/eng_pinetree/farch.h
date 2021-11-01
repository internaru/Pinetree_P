#ifdef __CPU_16LX__	/* for 16 bit C Compiler System */

/*************************************
 **    FFMC C compailer mean that,  **
 **    char ----->  8 bit           **
 **    int  -----> 16 bit           **
 **    long -----> 32 bit           **
 *************************************
 *  Integer types
 */

/* 8 bit */
typedef char Sint8;             /* -128 .. 127 */
typedef unsigned char Uint8;    /* 0 .. 255 */

#define MINSINT8  ((char) 0x80)
#define MAXSINT8  ((char) 0x7f)
#define MINUINT8  ((unsigned char) 0x00)
#define MAXUINT8  ((unsigned char) 0xff)

/* 16 bit */
typedef int Sint16;           /* -32768 .. 32767 */
typedef unsigned int Uint16;  /* 0 .. 65535 */

#define MINSINT16  ((int) 0x8000)
#define MAXSINT16  ((int) 0x7fff)
#define MINUINT16  ((unsigned int) 0x0000)
#define MAXUINT16  ((unsigned int) 0xffff)

/* 32 bit */
typedef long Sint32;            /* -2147483648 .. 2147483647 */
typedef unsigned long Uint32;   /* 0 .. 4294967295 */

#define MINSINT32  ((long) 0x80000000)
#define MAXSINT32  ((long) 0x7fffffff)
#define MINUINT32  ((unsigned long) 0x00000000)
#define MAXUINT32  ((unsigned long) 0xffffffff)

/* Boolean */
typedef char Boolean;

# else	/* for 32 bit C Compiler System */


/* ************************************
 **    FR C compailer mean that,    **
 **    char   ----->  8 bit         **
 **    short  -----> 16 bit         **
 **    int    -----> 32 bit         **
 *************************************
 *  Integer types
 */

/* 8 bit */
typedef char Sint8;             /* -128 .. 127 */
typedef unsigned char Uint8;    /* 0 .. 255 */

#define MINSINT8  ((char) 0x80)
#define MAXSINT8  ((char) 0x7f)
#define MINUINT8  ((unsigned char) 0x00)
#define MAXUINT8  ((unsigned char) 0xff)

/* 16 bit */
typedef short Sint16;           /* -32768 .. 32767 */
typedef unsigned short Uint16;  /* 0 .. 65535 */

#define MINSINT16  ((short) 0x8000)
#define MAXSINT16  ((short) 0x7fff)
#define MINUINT16  ((unsigned short) 0x0000)
#define MAXUINT16  ((unsigned short) 0xffff)

/* 32 bit */
typedef int Sint32;            /* -2147483648 .. 2147483647 */
typedef unsigned int Uint32;   /* 0 .. 4294967295 */

#define MINSINT32  ((int) 0x80000000)
#define MAXSINT32  ((int) 0x7fffffff)
#define MINUINT32  ((unsigned int) 0x00000000)
#define MAXUINT32  ((unsigned int) 0xffffffff)

/* Boolean */
#ifdef __CPU_FR__	/* for Compact System */
typedef char Boolean;
#else		/* for Workstation or PC System */
typedef int Boolean;
#endif

# endif   /*****    TargetMachFlag    *****/


typedef	int	SIX8;
typedef	unsigned int	UIX8;
typedef	int	SIX16;
typedef	unsigned int	UIX16;

#ifdef __CPU_FR__
typedef	int	SIX32;
typedef	unsigned int	UIX32;
#else
typedef	long	SIX32;
typedef	unsigned long	UIX32;
#endif

#if !defined(TRUE)
#define TRUE (1)
#endif

#if !defined(FALSE)
#define FALSE (0)
#endif

/*
 *   Floating Point Types
 */

typedef float Float32;

#define MINFLOAT32 ((float) 0xffffffff) 
#define MAXFLOAT32 ((float) 0x7fffffff) 

typedef double Float64;

#define MINFLOAT64 ((float) 0xffffffffffffffff) 
#define MAXFLOAT64 ((float) 0x7fffffffffffffff) 

/* For those with an 80 bit IEEE type accessible from C...
 * typedef iee80 Float80;
 * #define MINFLOAT80 ((iee80) 0xffffffffffffffffffff)
 * #define MAXFLOAT80 ((iee80) 0x7fffffffffffffffffff)
 */
/* $Id: farch.h,v 1.1 2002/08/13 13:27:07 dbrown (null) $ */
