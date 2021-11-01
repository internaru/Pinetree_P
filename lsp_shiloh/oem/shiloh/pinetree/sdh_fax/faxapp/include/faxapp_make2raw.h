#ifndef __FAXAPP_MAKE_2_RAW__
#define __FAXAPP_MAKE_2_RAW__

#include <stdio.h>

#define TRUE		1
#define FALSE		0

#define	EOL	0x001	/* EOL code value - 0000 0000 0000 1 */
#define	isAligned(p,t)	((((unsigned long)(p)) & (sizeof (t)-1)) == 0)


typedef struct {
    unsigned int	rowbytes;		/* bytes in a decoded scanline */
    unsigned int	rowpixels;		/* pixels in a scanline */
    unsigned int	imagesize;

/* Decoder state info */
    int	bit;						/* current i/o bit in byte */
    unsigned int	data;			/* current i/o byte/word */

/* input/output buffering */
    int				tif_rawcc;		/* bytes unread from raw buffer */
    unsigned char*	tif_rawbase;	/* current spot in raw buffer */
    unsigned char*	tif_rawcp;		/* current spot in raw buffer */
} Fax3CodecState;

/*
 * CCITT Group 3 FAX Encoding.
 */

#define	_FlushBits(tif) {					\
    *(tif)->tif_rawcp++ = (unsigned char) data;		\
    (tif)->tif_rawcc++;					\
    data = 0, bit = 8;					\
}


static const int _msbmask[9] =
    { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
#define	_PutBits(tif, bits, length) {				\
    while (length > bit) {					\
        data |= bits >> (length - bit);			\
        length -= bit;					\
        _FlushBits(tif);				\
    }							\
    data |= (bits & _msbmask[length]) << (bit - length);	\
    bit -= length;						\
    if (bit == 0)						\
        _FlushBits(tif);				\
}

int Fax3SetupState(Fax3CodecState* sp);
int Fax3PreEncode(Fax3CodecState* sp);
int Fax3Encode(Fax3CodecState* sp, unsigned char* bp);
int Fax3Encode_UIScan(Fax3CodecState* sp, unsigned char* bp);
int Fax3PostEncode(Fax3CodecState* sp);

typedef struct {
	unsigned int	rowbytes;		/* bytes in a decoded scanline */
	unsigned int	rowpixels;		/* pixels in a scanline */

	/* Decoder state info */
	unsigned int	data;			/* current i/o byte/word */
	int				bit;			/* current i/o bit in byte */
	int				EOLcnt;			/* count of EOL codes recognized */
	unsigned int*	runs;			/* b&w runs for current/previous row */

	unsigned char*	refline;	/* reference line for 2d decoding */

	int				tif_rawcc;		/* bytes unread from raw buffer */
	unsigned char*	tif_rawcp;		/* current spot in raw buffer */
	unsigned char*	tif_rawbase;	/* raw data buffer */	
	unsigned int	imagesize;
} Fax4CodecState;

int Fax4SetupState(Fax4CodecState *esp);
int Fax4PreEncode(Fax4CodecState *esp);
int Fax4Encode_booklet( Fax4CodecState *data, const unsigned char *buf);
void Fax4PostEncode(Fax4CodecState *esp);
#endif // #ifndef __FAXAPP_MAKE_2_RAW__
