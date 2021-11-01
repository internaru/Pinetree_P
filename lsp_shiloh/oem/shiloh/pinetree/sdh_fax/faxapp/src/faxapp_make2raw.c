#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "faxapp_make2raw.h"
#include "t4.h"
#include "../../fax_memmgr/include/fax_memAPI.h"

/*
 * Write the sequence of codes that describes
 * the specified span of zero's or one's.  The
 * appropriate table that holds the make-up and
 * terminating codes is supplied.
 */
static void putspan(Fax3CodecState* sp, int span, const tableentry* tab)
{
    unsigned int bit = sp->bit;
    int data = sp->data;
    unsigned int code, length;

    while (span >= 2624) {
        const tableentry* te = &tab[63 + (2560>>6)];
        code = te->code, length = te->length;

        _PutBits(sp, code, length);
        span -= te->runlen;
    }
    if (span >= 64) {
        const tableentry* te = &tab[63 + (span>>6)];
        code = te->code, length = te->length;

        _PutBits(sp, code, length);
        span -= te->runlen;
    }
    code = tab[span].code, length = tab[span].length;

    _PutBits(sp, code, length);

    sp->data = data;
    sp->bit = bit;
}

/*
 * Write an EOL code to the output stream.  The zero-fill
 * logic for byte-aligning encoded scanlines is handled
 * here.  We also handle writing the tag bit for the next
 * scanline when doing 2d encoding.
 */
static void Fax3PutEOL(Fax3CodecState* sp)
{
    unsigned int bit = sp->bit;
    int data = sp->data;
    unsigned short code, length;

//	printf("Fax3PutEOL\n");

    code = EOL;
    length = 12;

    _PutBits(sp, code, length);

    sp->data = data;
    sp->bit = bit;
}

/*
 * Reset encoding state at the start of a strip.
 */
int Fax3PreEncode(Fax3CodecState* sp)
{
    sp->bit = 8;
    sp->data = 0;
    sp->tif_rawcc = 0;

    return (1);
}

int Fax3PostEncode(Fax3CodecState* sp)
{

    MEM_FREE_AND_NULL(sp->tif_rawbase);
    return (1);
}

static const unsigned char zeroruns[256] = {
    8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,	/* 0x00 - 0x0f */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	/* 0x10 - 0x1f */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0x20 - 0x2f */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0x30 - 0x3f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x40 - 0x4f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x50 - 0x5f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x60 - 0x6f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x70 - 0x7f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x80 - 0x8f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x90 - 0x9f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xa0 - 0xaf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xb0 - 0xbf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xc0 - 0xcf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xd0 - 0xdf */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xe0 - 0xef */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xf0 - 0xff */
};
static const unsigned char oneruns[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x00 - 0x0f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x10 - 0x1f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x20 - 0x2f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x30 - 0x3f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x40 - 0x4f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x50 - 0x5f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x60 - 0x6f */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x70 - 0x7f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x80 - 0x8f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0x90 - 0x9f */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0xa0 - 0xaf */
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,	/* 0xb0 - 0xbf */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0xc0 - 0xcf */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,	/* 0xd0 - 0xdf */
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,	/* 0xe0 - 0xef */
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 7, 8,	/* 0xf0 - 0xff */
};

/*
 * Find a span of ones or zeros using the supplied
 * table.  The ``base'' of the bit string is supplied
 * along with the start+end bit indices.
 */
static int find0span(unsigned char* bp, int bs, int be)
{
    int bits = be - bs;
    int n, span;

    bp += bs>>3;
    /*
     * Check partial byte on lhs.
     */
    if (bits > 0 && (n = (bs & 7))) {
        span = zeroruns[(*bp << n) & 0xff];
        if (span > 8-n)		/* table value too generous */
            span = 8-n;
        if (span > bits)	/* constrain span to bit range */
            span = bits;
        if (n+span < 8)		/* doesn't extend to edge of byte */
            return (span);
        bits -= span;
        bp++;
    } else
        span = 0;
    if (bits >= (int)(2 * 8 * sizeof(long))) {
        long* lp;
        /*
         * Align to longword boundary and check longwords.
         */
        while (!isAligned(bp, long)) {
            if (*bp != 0x00)
                return (span + zeroruns[*bp]);
            span += 8, bits -= 8;
            bp++;
        }
        lp = (long*) bp;
        while ((bits >= (int)(8 * sizeof(long))) && (0 == *lp)) {
            span += 8*sizeof (long), bits -= 8*sizeof (long);
            lp++;
        }
        bp = (unsigned char*) lp;
    }
    /*
     * Scan full bytes for all 0's.
     */
    while (bits >= 8) {
        if (*bp != 0x00)	/* end of run */
            return (span + zeroruns[*bp]);
        span += 8, bits -= 8;
        bp++;
    }
    /*
     * Check partial byte on rhs.
     */
    if (bits > 0) {
        n = zeroruns[*bp];
        span += (n > bits ? bits : n);
    }
    return (span);
}

static int find1span(unsigned char* bp, int bs, int be)
{
    int bits = be - bs;
    int n, span;

    bp += bs>>3;
    /*
     * Check partial byte on lhs.
     */
    if (bits > 0 && (n = (bs & 7))) {
        span = oneruns[(*bp << n) & 0xff];
        if (span > 8-n)		/* table value too generous */
            span = 8-n;
        if (span > bits)	/* constrain span to bit range */
            span = bits;
        if (n+span < 8)		/* doesn't extend to edge of byte */
            return (span);
        bits -= span;
        bp++;
    } else
        span = 0;
    if (bits >= (int)(2 * 8 * sizeof(long))) {
        long* lp;
        /*
         * Align to longword boundary and check longwords.
         */
        while (!isAligned(bp, long)) {
            if (*bp != 0xff)
                return (span + oneruns[*bp]);
            span += 8, bits -= 8;
            bp++;
        }
        lp = (long*) bp;
        while ((bits >= (int)(8 * sizeof(long))) && (~0 == *lp)) {
            span += 8*sizeof (long), bits -= 8*sizeof (long);
            lp++;
        }
        bp = (unsigned char*) lp;
    }
    /*
     * Scan full bytes for all 1's.
     */
    while (bits >= 8) {
        if (*bp != 0xff)	/* end of run */
            return (span + oneruns[*bp]);
        span += 8, bits -= 8;
        bp++;
    }
    /*
     * Check partial byte on rhs.
     */
    if (bits > 0) {
        n = oneruns[*bp];
        span += (n > bits ? bits : n);
    }
    return (span);
}

/*
 * 1d-encode a row of pixels.  The encoding is
 * a sequence of all-white or all-black spans
 * of pixels encoded wit Huffman codes.
 */
static int Fax3Encode1DRow(Fax3CodecState* sp, unsigned char* bp, unsigned int bits)
{
    int span;
    unsigned int bs = 0;

// Write whitecodes and then blackcodes. -> must
    for (;;) {
        span = find0span(bp, bs, bits);		/* black span */
        putspan(sp, span, TIFFFaxWhiteCodes);
        bs += span;
        if (bs >= bits)
            break;
        span = find1span(bp, bs, bits);		/* white span */
        putspan(sp, span, TIFFFaxBlackCodes);
        bs += span;
        if (bs >= bits)
            break;
    }
    return (1);
}

/*
 * Encode a buffer of pixels.
 */
int Fax3Encode(Fax3CodecState* sp, unsigned char* bp)
{
    int cc = sp->imagesize;
    int cnt = 0;

    while ((long)cc > 0) {

        Fax3PutEOL(sp);
        cnt++;
        if (!Fax3Encode1DRow(sp, bp, sp->rowpixels))
        {
            printf("===> Encode Error\n");
            return (0);
        }

        bp += sp->rowbytes;
        cc -= sp->rowbytes;
#if 0
        if (sp->tif_rawcc > sp->imagesize)
        {
			 printf("===> Encode Error 2 (%d)(%d)\n", sp->tif_rawcc, cc);
            return (0);
    }
#endif
    }
    printf("Encode Complete!! cnt : %d\n",cnt);
    Fax3PutEOL(sp);
    return (1);
}

static int Fax3Encode1DRow_UIScan(Fax3CodecState* sp, unsigned char* bp, unsigned int bits)
{
    int span;
    unsigned int bs = 0;

// Write whitecodes and then blackcodes. -> must
    for (;;) {
        span = find1span(bp, bs, bits);		/* white span */
        putspan(sp, span, TIFFFaxWhiteCodes);
        bs += span;
        if (bs >= bits)
            break;
        span = find0span(bp, bs, bits);		/* black span */
        putspan(sp, span, TIFFFaxBlackCodes);
        bs += span;
        if (bs >= bits)
            break;
    }
    return (1);
}

int Fax3Encode_UIScan(Fax3CodecState* sp, unsigned char* bp)
{
	int cc = sp->imagesize;
	int cnt = 0;

	while ((long)cc > 0)
	{

		Fax3PutEOL(sp);
		cnt++;
		if (!Fax3Encode1DRow_UIScan(sp, bp, sp->rowpixels))
		{
			printf("===> Encode Error\n");
			return (0);
		}

		bp += sp->rowbytes;
		cc -= sp->rowbytes;

		if (sp->tif_rawcc > sp->imagesize)
		{
			return (0);
		}
	}
	printf("Encode Complete!! cnt : %d\n",cnt);
	Fax3PutEOL(sp);
	return (1);
}

/*
 * Setup G3/G4-related compression/decompression state
 * before data is processed.  This routine is called once
 * per image -- it sets up different state based on whether
 * or not decoding or encoding is being done and whether
 * 1D- or 2D-encoded data is involved.
 */
int Fax3SetupState(Fax3CodecState* sp)
{
    unsigned int rowbytes;

    //printf("Fax3SetupState\n");

    /*
     * Calculate the scanline/tile widths.
     */
    rowbytes = (unsigned int)((sp->rowpixels + 7) >> 3);
//	printf(" b:%d, p:%d\n",rowbytes,rowpixels);

    sp->rowbytes = (unsigned int) rowbytes;

    sp->tif_rawbase = (unsigned char *) MEM_MALLOC((sp->imagesize * 2) * sizeof(unsigned char));
    sp->tif_rawcp = sp->tif_rawbase;

    return (1);
}

/* NB: the uint32 casts are to silence certain ANSI-C compilers */
#define TIFFhowmany(x, y) ((((unsigned int)(x))+(((unsigned int)(y))-1))/((unsigned int)(y)))
#define	TIFFroundup(x, y) (TIFFhowmany(x,y)*(y))

void Fax4PostEncode(Fax4CodecState *esp)
{
	if(esp->refline!=NULL) {
		free(esp->refline);
		esp->refline=NULL;
	}
	if(esp->runs!=NULL) {
		free(esp->runs);
		esp->runs=NULL;
	}

	if(esp->tif_rawbase!=NULL) {
		free(esp->tif_rawbase);
		esp->tif_rawbase=NULL;
	}
}

/*
 * Setup G3/G4-related compression/decompression state
 * before data is processed.  This routine is called once
 * per image -- it sets up different state based on whether
 * or not decoding or encoding is being done and whether
 * 1D- or 2D-encoded data is involved.
 */
int Fax4SetupState(Fax4CodecState *esp)
{
	unsigned int nruns;

	esp->rowbytes = (unsigned int) (esp->rowpixels/8);

	nruns = 2*TIFFroundup(esp->rowpixels,32);
	nruns += 3;
	esp->runs = (unsigned int*) malloc(2*nruns * sizeof (unsigned int));
	if (esp->runs == NULL)
		return 1;

	/*
	 * 2d encoding requires a scanline
	 * buffer for the ``reference line''; the
	 * scanline against which delta encoding
	 * is referenced.  The reference line must
	 * be initialized to be ``white'' (done elsewhere).
	 */
	esp->refline = (unsigned char*) malloc(esp->rowbytes);
	if (esp->refline == NULL) {
		return 0;
	}

	return 1;
}

/*
 * Reset encoding state at the start of a strip.
 */
int Fax4PreEncode(Fax4CodecState *esp)
{
	/* assert(sp != NULL); */
	esp->bit = 8;
	esp->data = 0;
	
	/*
	 * This is necessary for Group 4; otherwise it isn't
	 * needed because the first scanline of each strip ends
	 * up being copied into the refline.
	 */
	if (esp->refline)
		memset(esp->refline, 0x00, esp->rowbytes);

	return 1;
}

/*
 * Write a variable-length bit-value to
 * the output stream.  Values are
 * assumed to be at most 16 bits.
 */
static void Fax4PutBits(Fax4CodecState *esp, unsigned int bits, unsigned int length)
{
	unsigned int bit = esp->bit;
	int data = esp->data;

	_PutBits(esp, bits, length);

	esp->data = data;
	esp->bit = bit;
}

/*
 * Write a code to the output stream.
 */
#define putcode(tif, te)	Fax4PutBits(tif, (te)->code, (te)->length)

/*
 * Write the sequence of codes that describes
 * the specified span of zero's or one's.  The
 * appropriate table that holds the make-up and
 * terminating codes is supplied.
 */
static void putspan_Fax4(Fax4CodecState *esp, int span, const tableentry* tab)
{
	unsigned int bit = esp->bit;
	int data = esp->data;
	unsigned int code, length;

	while (span >= 2624) {
		const tableentry* te = &tab[63 + (2560>>6)];
		code = te->code, length = te->length;
		_PutBits(esp, code, length);
		span -= te->runlen;
	}
	if (span >= 64) {
		const tableentry* te = &tab[63 + (span>>6)];
		code = te->code, length = te->length;
		_PutBits(esp, code, length);
		span -= te->runlen;
	}
	code = tab[span].code, length = tab[span].length;
	_PutBits(esp, code, length);

	esp->data = data;
	esp->bit = bit;
}

/*
 * Return the offset of the next bit in the range
 * [bs..be] that is different from the specified
 * color.  The end, be, is returned if no such bit
 * exists.
 */
#define	finddiff(_cp, _bs, _be, _color)	\
	(_bs + (_color ? find1span(_cp,_bs,_be) : find0span(_cp,_bs,_be)))
/*
 * Like finddiff, but also check the starting bit
 * against the end in case start > end.
 */
#define	finddiff2(_cp, _bs, _be, _color) \
	(_bs < _be ? finddiff(_cp,_bs,_be,_color) : _be)


static const tableentry horizcode =
    { 3, 0x1, 0 };	/* 001 */
static const tableentry passcode =
    { 4, 0x1, 0 };	/* 0001 */
static const tableentry vcodes[7] = {
    { 7, 0x03, 0 },	/* 0000 011 */
    { 6, 0x03, 0 },	/* 0000 11 */
    { 3, 0x03, 0 },	/* 011 */
    { 1, 0x1, 0 },	/* 1 */
    { 3, 0x2, 0 },	/* 010 */
    { 6, 0x02, 0 },	/* 0000 10 */
    { 7, 0x02, 0 }	/* 0000 010 */
};

/*
 * 2d-encode a row of pixels.  Consult the CCITT
 * documentation for the algorithm.
 */
static void Fax4Encode2DRow(Fax4CodecState *esp, unsigned char* bp, unsigned char* rp, unsigned int bits)
{
#define	PIXEL(buf,ix)	((((buf)[(ix)>>3]) >> (7-((ix)&7))) & 1)
	unsigned int a0 = 0;
	unsigned int a1 = (PIXEL(bp, 0) != 0 ? 0 : finddiff(bp, 0, bits, 0));
	unsigned int b1 = (PIXEL(rp, 0) != 0 ? 0 : finddiff(rp, 0, bits, 0));
	unsigned int a2, b2;

	for (;;) {
		b2 = finddiff2(rp, b1, bits, PIXEL(rp,b1));
		if (b2 >= a1) {
			int d = b1 - a1;
			if (!(-3 <= d && d <= 3)) {	/* horizontal mode */
				a2 = finddiff2(bp, a1, bits, PIXEL(bp,a1));
				putcode(esp, &horizcode);
				if (a0+a1 == 0 || PIXEL(bp, a0) == 0) {
					putspan_Fax4(esp, a1-a0, TIFFFaxWhiteCodes);
					putspan_Fax4(esp, a2-a1, TIFFFaxBlackCodes);
				} else {
					putspan_Fax4(esp, a1-a0, TIFFFaxBlackCodes);
					putspan_Fax4(esp, a2-a1, TIFFFaxWhiteCodes);
				}
				a0 = a2;
			} else {			/* vertical mode */
				putcode(esp, &vcodes[d+3]);
				a0 = a1;
			}
		} else {				/* pass mode */
			putcode(esp, &passcode);
			a0 = b2;
		}
		if (a0 >= bits)
			break;
		a1 = finddiff(bp, a0, bits, PIXEL(bp,a0));
		b1 = finddiff(rp, a0, bits, !PIXEL(bp,a0));
		b1 = finddiff(rp, b1, bits, PIXEL(bp,a0));
	}
#undef PIXEL
}

/*
 * Encode the requested amount of data.
 */
static void Fax4Encode(Fax4CodecState *esp, unsigned char* bp, int cc)
{
	while ((long)cc > 0) {
		Fax4Encode2DRow(esp, bp, esp->refline, esp->rowpixels);
		memcpy(esp->refline, bp, esp->rowbytes);
		bp += esp->rowbytes;
		cc -= esp->rowbytes;
	}
}

int Fax4Encode_booklet( Fax4CodecState *esp, const unsigned char *buf)
{
	const unsigned char *pBufPos;
	const unsigned char *pBufEnd; /* end marker */

	pBufPos = buf;
	pBufEnd = buf+(esp->imagesize);

	esp->tif_rawbase = (unsigned char*) malloc( 2* esp->imagesize );
	esp->tif_rawcc = 0;
	esp->tif_rawcp = esp->tif_rawbase;

	//printf("Encode Start!! %d, %d, %d\n", esp->rowbytes, esp->rowpixels, esp->imagesize);
	
	/*  encode data */
	while(pBufEnd!=pBufPos)
	{
		Fax4Encode(esp, (unsigned char*)pBufPos, esp->rowbytes);
		pBufPos+=esp->rowbytes;
		//printf("tif_rawcc %d %p %p\n", esp->tif_rawcc, pBufPos, pBufEnd );
		if (esp->tif_rawcc > esp->imagesize)
			return 0;
	}

	/* terminate strip w/ EOFB */
	Fax4PutBits(esp, EOL, 12);
	Fax4PutBits(esp, EOL, 12);

	if (esp->tif_rawcc > esp->imagesize)
		return 0;
	else		
		return 1;
}