
/*
-------------------------------------------------------------------------------
One of the macros `BIGENDIAN' or `LITTLEENDIAN' must be defined.
-------------------------------------------------------------------------------
*/
#define LITTLEENDIAN

/*
-------------------------------------------------------------------------------
The macro `BITS64' can be defined to indicate that 64-bit integer types are
supported by the compiler.
-------------------------------------------------------------------------------
*/
#define BITS64

/*
-------------------------------------------------------------------------------
Each of the following `typedef's defines the most convenient type that holds
integers of at least as many bits as specified.  For example, `uint8' should
be the most convenient type that can hold unsigned integers of as many as
8 bits.  The `flag' type must be able to hold either a 0 or 1.  For most
implementations of C, `flag', `uint8', and `int8' should all be `typedef'ed
to the same as `int'.
-------------------------------------------------------------------------------
*/
typedef char flag;
typedef unsigned char uint8;
typedef signed char int8;
typedef int uint16;
typedef int int16;
typedef unsigned int uint32;
typedef signed int int32;
#ifdef BITS64
typedef unsigned long long int bits64;
typedef signed long long int sbits64;
#endif

/*
-------------------------------------------------------------------------------
Each of the following `typedef's defines a type that holds integers
of _exactly_ the number of bits specified.  For instance, for most
implementation of C, `bits16' and `sbits16' should be `typedef'ed to
`unsigned short int' and `signed short int' (or `short int'), respectively.
-------------------------------------------------------------------------------
*/
typedef unsigned char bits8;
typedef signed char sbits8;
typedef unsigned short int bits16;
typedef signed short int sbits16;
typedef unsigned int bits32;
typedef signed int sbits32;
#ifdef BITS64
typedef unsigned long long int uint64;
typedef signed long long int int64;
#endif

#ifdef BITS64
/*
-------------------------------------------------------------------------------
The `LIT64' macro takes as its argument a textual integer literal and if
necessary ``marks'' the literal as having a 64-bit integer type.  For
example, the Gnu C Compiler (`gcc') requires that 64-bit literals be
appended with the letters `LL' standing for `long long', which is `gcc's
name for the 64-bit integer type.  Some compilers may allow `LIT64' to be
defined as the identity macro:  `#define LIT64( a ) a'.
-------------------------------------------------------------------------------
*/
#define LIT64( a ) a##LL
#endif

/*
-------------------------------------------------------------------------------
The macro `INLINE' can be used before functions that should be inlined.  If
a compiler does not support explicit inlining, this macro should be defined
to be `static'.
-------------------------------------------------------------------------------
*/
#define INLINE extern __inline__


/* For use as a GCC soft-float library we need some special function names. */

#ifdef __LIBFLOAT__

/* Some 32-bit ops can be mapped straight across by just changing the name. */
#define float32_add			__addsf3
#define float32_sub			__subsf3
#define float32_mul			__mulsf3
#define float32_div			__divsf3
#define int32_to_float32		__floatsisf
#define uint32_to_float32		__floatunsisf
#define float32_to_int32_round_to_zero	__fixsfsi
#define float32_to_uint32_round_to_zero	__fixunssfsi

/* These ones go through the glue code.  To avoid namespace pollution
   we rename the internal functions too.  */
#define float32_eq			___float32_eq
#define float32_le			___float32_le
#define float32_lt			___float32_lt

/* All the 64-bit ops have to go through the glue, so we pull the same
   trick.  */
#if 0
#define float64_add			___float64_add
#define float64_sub			___float64_sub
#define float64_mul			___float64_mul
#define float64_div			___float64_div
#define int32_to_float64		___int32_to_float64
#define float64_to_int32_round_to_zero	___float64_to_int32_round_to_zero
#define float64_to_uint32_round_to_zero	___float64_to_uint32_round_to_zero
#define float64_to_float32		___float64_to_float32
#define float32_to_float64		___float32_to_float64
#define float64_eq			___float64_eq
#define float64_le			___float64_le
#define float64_lt			___float64_lt

#else

#define float64_add			__adddf3
#define float64_sub			__subdf3
#define float64_mul			__muldf3
#define float64_div			__divdf3
#define int32_to_float64		__floatsidf
#define float64_to_int32_round_to_zero	__fixdfsi
#define float64_to_uint32_round_to_zero	__fixunsdfsi
#define float64_to_float32		__truncdfsf2
#define float32_to_float64		__extendsfdf2
#endif

#ifdef __ARM_EABI__
/* Rename functions to their EABI names.  */
/* The comparison functions need wrappers for EABI semantics, so
   leave them unmolested.  */
#define __negsf2		__aeabi_fneg
#define __subsf3		__aeabi_fsub
#define __addsf3		__aeabi_fadd
#define __floatunsisf	__aeabi_ui2f
#define __floatsisf		__aeabi_i2f
#define __floatundisf	__aeabi_ul2f
#define __floatdisf		__aeabi_l2f
#define __mulsf3		__aeabi_fmul
#define __divsf3		__aeabi_fdiv
#define __unordsf2		__aeabi_fcmpun
#define __fixsfsi		__aeabi_f2iz
#define __fixunssfsi	__aeabi_f2uiz
#define __fixsfdi		__aeabi_f2lz
#define __fixunssfdi	__aeabi_f2ulz
#define __floatdisf		__aeabi_l2f

#define __negdf2        __aeabi_dneg
#define __subdf3        __aeabi_dsub
#define __adddf3        __aeabi_dadd
//#define __floatunsidf        __aeabi_ui2d
#define __floatsidf        __aeabi_i2d
#define __extendsfdf2        __aeabi_f2d
#define __truncdfsf2        __aeabi_d2f
#define __floatundidf        __aeabi_ul2d
#define __floatdidf        __aeabi_l2d
#define __muldf3        __aeabi_dmul
#define __divdf3        __aeabi_ddiv
#define __unorddf2        __aeabi_dcmpun
#define __fixdfsi        __aeabi_d2iz
#define __fixunsdfsi        __aeabi_d2uiz
#define __fixdfdi        __aeabi_d2lz
#define __fixunsdfdi        __aeabi_d2ulz
#define __floatdidf        __aeabi_l2d

#define	___float32_le		__aeabi_fcmpgt

#define __extendhfsf2        __gnu_h2f_ieee

#define __truncsfhf2        __gnu_f2h_ieee

#endif /* __ARM_EABI__ */

#endif

