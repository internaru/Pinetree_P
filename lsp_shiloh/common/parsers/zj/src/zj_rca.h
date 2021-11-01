/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved
*
*               Marvell Confidential
 * ============================================================================
 *
*/

/* ZjRCA.h */

/* Binary stream definitions for Zenographics J series controllers */
/* (C) 1998-2003 Zenographics, Inc.  All rights reserved. */


#ifndef __ZJRCA_H__
#define __ZJRCA_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ZJRCA_SIGNATURE_LE 0x4A5A4A5A   /* "ZJZJ" little-endian */
#define ZJRCA_SIGNATURE_BE 0x5A4A5A4A   /* byte swapped */

#define ZJRCA_SIGNATURE ZJRCA_SIGNATURE_LE
#define ZJRCA_SIGNATURE_SWAPPED ZJRCA_SIGNATURE_BE

#define ZJRCA_ITEM_SIGNATURE  0x5A5A  /* "ZZ" */
#define SDH_TONERMODE			1 /* GDI Toner save & darkness	@@ 2013.11.12		by Juny */

/* Any reader (big or little endian) that matches ZJRCA_SIGNATURE uses 
        its native endian for stream uint32_t (except BIH contents); 
        any reader that matches ZJRCA_SIGNATURE_SWAPPED must byte-swap.
 */

/* Stream sequence:

    [ZJRCA_SIGNATURE] (or ZJRCA_SIGNATURE_SWAPPED)

    [START_DOC]
        header:(key=ZJT_START_DOC, type=ZJIT_ZJT/ZJIT_COLLECTION)
        // numElements items follow, in no specific order...
        item:(ZJI_PAGECOUNT, type=ZJIT_NUMBER)
        item:(ZJI_DMCOLLATE, type=ZJIT_NUMBER)
        item:(ZJI_COLLATE, type=ZJIT_NUMBER)

        -- for each page ...

        [START_PAGE]
            header:(key=ZJT_START_PAGE, type=ZJIT_ZJT/ZJIT_COLLECTION)
            // numElements items follow, in no specific order...
            item:(ZJI_DMPAPER, type=ZJIT_NUMBER)
            item:(ZJI_DMCOPIES, type=ZJIT_NUMBER)
            item:(ZJI_COPIES, type=ZJIT_NUMBER)
            item:(ZJI_DMDEFAULTSOURCE, type=ZJIT_NUMBER)
            item:(ZJI_DMDEFAULTSOURCE, type=ZJIT_XY)
            item:(ZJI_DMDEFAULTSOURCE, type=ZJIT_XY)
            item:(ZJI_RESOLUTION, type=ZJIT_XY)
            item:(ZJI_OFFSET, type=ZJIT_XY)
            item:(ZJI_RASTER, type=ZJIT_XY)


            -- for each BIE, a BIH, then one or more BID blocks ...
            -- i.e., for each color plane

            [JBIG_BIH]
                header:(key=ZJT_JBIG_BIH, type=ZJIT_BUFFER)
                // length bytes follow, padded out to least uint32_t
                ...

            [JBIG_BID]
                header:(key=ZJT_JBIG_BIH, type=ZJIT_BUFFER)
                // length bytes follow, padded out to least uint32_t
                ...

            (more items... most are [JBIG_BID] ZJIT_BUFFER blocks...)

            [END_JBIG]
                header:(key=ZJT_END_JBIG, type=ZJIT_MARKER)
                // if ZJIT_MARKER: no additional data

        [END_PAGE]
            header:(key=ZJT_END_JBIG, type=ZJIT_MARKER)
            // if ZJIT_MARKER: no additional data

    [END_DOC]
        header:(key=ZJT_END_DOC, type=ZJIT_MARKER)
        // if ZJIT_MARKER: no additional data

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

        ZJT_END_DOC, ZJT_END_PAGE, and ZJT_END_JBIG have no additional data.

        All record sizes are an even multiple of uint32_t (uint32_t
        alignment), including ZJ_JBIG_BID records.

        All but the last BID block record contain valid JBIG data
        (no padding at the end of a BID block to fulfill the uint32_t 
        alignment requirement).  The last BID block of a plane is 
        padded with at least 4 trailing zero bytes.

        Each plane contains 1 or more BID records.  A plane ends
        with a ZJ_END_JBIG record.
*/

typedef enum { 
        ZJT_START_DOC, ZJT_END_DOC,
        ZJT_START_PAGE, ZJT_END_PAGE,
        ZJT_JBIG_BIH,           /*  Bi-level Image Header */
        ZJT_JBIG_BID,           /*  Bi-level Image Data blocks */
        ZJT_END_JBIG,
        ZJT_SIGNATURE,
        ZJT_RAW_IMAGE,              /*  full uncompressed plane follows */
        ZJT_START_PLANE, ZJT_END_PLANE,
        ZJT_PAUSE,
        ZJT_BITMAP,         /* self-contained image record */
	ZJT_ZX_START,
	ZJT_ZX_DATA,
	ZJT_ZX_END,
	ZJT_OPAQUE_DATA
        } ZJ_TYPE;                      /*  record types */
# if defined( HAVE_DBG_PRINTF )
static const char* DecodeZjBufType( ZJ_TYPE zjType );
# endif // defined( HAVE_DBG_PRINTF )

typedef struct _ZJ_HEADER {
        uint32_t cbSize;           /*  total record size, includes sizeof(ZJ_HEADER) */
        uint32_t Type;                     /*  ZJ_TYPE */
        uint32_t dwParam;          /*  use varies by Type, e.g. item count */
        uint16_t wParam;            /*  use varies by Type */
        uint16_t Signature;         /*  'ZZ' */
        } ZJ_HEADER;

typedef enum {
        /* for START_DOC */
        ZJI_PAGECOUNT,          /* number of ZJT_START_PAGE / ZJT_END_PAGE pairs, if known */
        ZJI_DMCOLLATE,          /* from DEVMODE */
        ZJI_DMDUPLEX,           /* from DEVMODE */

        /* for START_PAGE */
        ZJI_DMPAPER,            /* from DEVMODE */
        ZJI_DMCOPIES,           /* from DEVMODE */
        ZJI_DMDEFAULTSOURCE,    /* from DEVMODE */
        ZJI_DMMEDIATYPE,        /* from DEVMODE */
        ZJI_NBIE,                       /* number of Bi-level Image Entities, */
                                                /*  e.g. 1 for monochrome, 4 for color */
        ZJI_RESOLUTION_X, ZJI_RESOLUTION_Y,     /* dots per inch */
        ZJI_OFFSET_X, ZJI_OFFSET_Y,             /* upper left corner */
        ZJI_RASTER_X, ZJI_RASTER_Y,             /* raster dimensions */

        ZJI_COLLATE,            /* asks for collated copies */
        ZJI_QUANTITY,           /* copy count */

        ZJI_VIDEO_BPP,          /* video bits per pixel */
        ZJI_VIDEO_X, ZJI_VIDEO_Y,               /* video dimensions (if different than raster) */
        ZJI_INTERLACE,          /* 0 or 1 */
        ZJI_PLANE,                      /* enum PLANE */
        ZJI_PALETTE,            /* translation table (dimensions in item type) */

        ZJI_RET,                        /* ZJIT_UINT32, RET_xxx enum */
        ZJI_TONER_SAVE,         /* 0 == disable, !0 == enable */

        /* ZJI_MEDIA_SIZE_xxx *MUST* be supplied when ZJI_DMPAPER is set to a
         *                    value that indicates custom media size.
         * All three ZJI_MEDIA_SIZE_xxx tags must be present to be valid.
         */
        ZJI_MEDIA_SIZE_X,       /* in ZJI_MEDIA_SIZE_UNITS, along fast-scan direction */
        ZJI_MEDIA_SIZE_Y,       /* in ZJI_MEDIA_SIZE_UNITS, along slow-scan direction */
        ZJI_MEDIA_SIZE_UNITS,   /* 1 == 1/1000 inch, 2 == 1/10 mm */

        ZJI_CHROMATIC,      /* 0 == monochrome page, !0 = color page */

        ZJI_PAD = 99, /* bogus item type for padding stream */

        /*  for PAUSE */
        ZJI_PROMPT,         /* ZJIT_STRING */

        /*      for ZJT_BITMAP */
        ZJI_BITMAP_TYPE,                /* (required) see BITMAP_TYPE enum */
        ZJI_ENCODING_DATA,              /* (optional), ZJIT_BYTELUT, encoding-specific data, 
                                                         *  e.g. BIH 
                                                         */
        ZJI_END_PLANE,                  /* 1 == last bitmap for this plane this page */

        /*      for ZJT_BITMAP of type BITMAP_RAW */
        ZJI_BITMAP_PIXELS,
        ZJI_BITMAP_LINES,
        ZJI_BITMAP_BPP,
        ZJI_BITMAP_STRIDE,
        /*  General page color information.
            These values may be specified in ZJT_START_PAGE but may be
            overridden by an image entity's own parameters. */
        ZJI_COLOR_FORMAT,       /* Each byte holds a color ID; Order of values
                                   indicates color order in the color data.
                                   Default is [R,G,B,x] (where 'R' is the
                                   first appearing byte) if omitted.
                                   See ZJ_COLOR enum below for defined colors. */
        ZJI_COLOR_COMPONENT_SIZE,   /* Bit depth of each color component, in bits.
                                       Default is 8 if omitted. */
        ZJI_COLOR_SIZE,         /* Color value size, in bytes.
                                    Default is 4 if omitted. */

        /*  for ZJT_ZX_START */
        ZJI_ZxBITS,             /* Zx options bits (see ZxStream.h) used during
                                   creation of Zx data.
                                   Default is ZxBITS_DEFAULT if ommitted. */

	ZJI_OUTBIN,				/* output tray */

	ZJI_QUALITY = 113,             
	ZJI_BORDERLESS,
	ZJI_PRINTGRAY,

        /* for instrumentation */
        ZJI_TIMESTAMP = 0x7FFF, /* incrementing 32-bit counter for benchmarking */

        /* 0x8200-0x82FF : Item tags for the next OEM specific features. */
        ZJI_SW_PIXELCOUNT_C = 0x8200,

        ZJI_SW_PIXELCOUNT_M,

        ZJI_SW_PIXELCOUNT_Y,

        ZJI_SW_PIXELCOUNT_K,

        ZJI_SW_PIXELCOUNT_NW_C, /* non-whites */
        ZJI_SW_PIXELCOUNT_NW_M, /* non-whites */
        ZJI_SW_PIXELCOUNT_NW_Y, /* non-whites */
        ZJI_SW_PIXELCOUNT_NW_K, /* non-whites */
#ifdef SDH_TONERMODE		/* 2013.10.28		by Juny */
        ZJI_TONERSAVE = 0x8400,
        ZJI_TONERDARKNESS,
#endif
        } ZJ_ITEM;
# if defined( HAVE_DBG_PRINTF )
static const char* ToString_ZJ_ITEM( uint16_t zjItem );
# endif // defined( HAVE_DBG_PRINTF )

/*  Color component identifiers */
typedef enum {
    ZJC_NULL,
    ZJC_CYAN,
    ZJC_MAGENTA,
    ZJC_YELLOW,
    ZJC_BLACK,
    ZJC_CYAN2,
    ZJC_MAGENTA2,
    ZJC_YELLOW2,
    ZJC_BLACK2,
    ZJC_RED,
    ZJC_GREEN,
    ZJC_BLUE,
    ZJC_WHITE,
    } ZJ_COLOR;

/*      ZJI_BITMAP_TYPE enums */
typedef enum {
        ZJBT_RAW,
	ZJBT_JBIG,
	ZJBT_DORY
        } ZJ_BITMAP_TYPE;

typedef enum {
        ZJIT_UINT32=1,          /* unsigned integer */
        ZJIT_INT32,                     /* signed integer */
        ZJIT_STRING,            /* byte string, NUL-terminated, uint32_t-aligned */
        ZJIT_BYTELUT            /* uint32_t count followed by that many byte entries */
        } ZJ_ITEM_TYPE;

typedef struct _ZJ_ITEM_HEADER {
        uint32_t cbSize;           /*  total record size, includes sizeof(ZJ_ITEM_HEADER) */
    uint16_t  Item;                 /*  ZJ_ITEM */
    uint8_t  Type;                 /*  ZJ_ITEM_TYPE */
    uint8_t  bParam;               /*  general use */
    } ZJ_ITEM_HEADER;


typedef struct _ZJ_ITEM_UINT32 {
        ZJ_ITEM_HEADER header;
        uint32_t value;
        } ZJ_ITEM_UINT32;

typedef struct _ZJ_ITEM_INT32 {
        ZJ_ITEM_HEADER header;
        int32_t value;
        } ZJ_ITEM_INT32;

/* 
typedef struct _ZJ_ITEM_BYTELUT {
        ZJ_ITEM_HEADER header;
        uint8_t  lut[2];
        // actual count goes in header.bParam
        } ZJ_ITEM_BYTELUT;
*/
// I updated this structure to reflect what appears to be the actual usage
typedef struct _ZJ_ITEM_BYTELUT {
        ZJ_ITEM_HEADER header;
        uint32_t cbSize;           /* actual number of bytes in lut */
        uint8_t  lut[2];
        } ZJ_ITEM_BYTELUT;

typedef enum {
    RET_OFF = 0, RET_ON, RET_AUTO, RET_LIGHT, RET_MEDIUM, RET_DARK
    } RET_VALUE;

#ifdef __cplusplus
}
#endif
#endif /* __ZJRCA_H__*/
