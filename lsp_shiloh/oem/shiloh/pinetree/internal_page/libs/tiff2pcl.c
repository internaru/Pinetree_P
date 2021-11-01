/*
 * $Id: t2pcl.c,v 1.1 1996/05/03 14:32:12 alberto Exp alberto $
 * 
 * TIFF to PCL3 translator.
 *
 * Profuse apologies for writing such a spagetti-code program,
 * but speed is here the issue, so resampling and compression is
 * done all in one big loop in main instead of being modularized.
 *
 * This program translates input TIFF bitmap files into PCL level 3
 * files using PCL mode 2 row compression, which as far as I can 
 * tell is the more general compression supported by a large 
 * variety of laserjets and deskjets.  Better compression schemes
 * are supported under both PCL3 and PCL5, but not all printers 
 * will accept them.  For more information check Ghostscript's 
 * source code where several PCL drivers are available.
 *
 *
 * Standard disclaimer:
 *
 * This program is part of the NASA Astrophysics Data System
 * article service data reduction/conversion software.
 *
 * Copyright (C): 1994, 1995 Smithsonian Astrophysical Observatory.
 * You may do anything you like with this file except remove
 * this copyright.  The Smithsonian Astrophysical Observatory
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.  It may not be incorporated into
 * commercial products without permission of the Smithsonian
 * Astrophysical Observatory.
 *
 * Written by Alberto Accomazzi <alberto@cfa.harvard.edu>
 *                              http://cfa-www.harvard.edu/~alberto/
 *
 *
 * $Log: t2pcl.c,v $
 * Revision 1.1  1996/05/03  14:32:12  alberto
 * Initial revision
 *
 *
 */

//from
//https://www.cfa.harvard.edu/~alberto/tiff/tiff2pcl.c
//ftp://ftp.sgi.com/graphics/tiff/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

char *progname;
int debug = 0;

#define TOP_MARGIN 	 0.050	/* top margin for ADS docs */
#define BOTTOM_MARGIN 	 0.025	/* bottom margin for ADS docs */
#define PGSIZEX 	 8.270	/* standard A4 size */
#define LEFT_MARGIN	 0.250	/* PCL printers skip this margin */
#define PGSIZEY 	11.690
#define MARGIN 		 0.250	/* used to compute resolution for scans */

#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))
#define match(string, option, lmin) \
        (strncmp((string), (option), max((lmin),strlen(string))) == 0)

/* returns number of lower 4 bits set to 1 in entry */
static unsigned char lut4lo[] = {
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4,
     0,  1,  1,  2,  1,  2,  2,  3,  1,  2,  2,  3,  2,  3,  3,  4
};

/* returns number of upper 4 bits set to 1 in entry */
static unsigned char lut4hi[] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4
};

static unsigned char lut2hi1[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2
};

static unsigned char lut2hi2[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2
};

static unsigned char lut2lo1[] = {
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2
};

static unsigned char lut2lo2[] = {
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,
  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2,  0,  1,  1,  2
};

/* elements have single bits set if index is greater than 6 */
static unsigned char bit1ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80 
};
static unsigned char bit2ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40 
};
static unsigned char bit3ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
    0x20, 0x20, 0x20, 0x20, 0x20 
};
static unsigned char bit4ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10 
};
static unsigned char bit5ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08 
};
static unsigned char bit6ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04 
};
static unsigned char bit7ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02
};
static unsigned char bit8ge6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01
};

static unsigned char bit1ge2[] = { 0x00, 0x00, 0x80, 0x80, 0x80, 0x80 };
static unsigned char bit2ge2[] = { 0x00, 0x00, 0x40, 0x40, 0x40, 0x40 };
static unsigned char bit3ge2[] = { 0x00, 0x00, 0x20, 0x20, 0x20, 0x20 };
static unsigned char bit4ge2[] = { 0x00, 0x00, 0x10, 0x10, 0x10, 0x10 };
static unsigned char bit5ge2[] = { 0x00, 0x00, 0x08, 0x08, 0x08, 0x08 };
static unsigned char bit6ge2[] = { 0x00, 0x00, 0x04, 0x04, 0x04, 0x04 };
static unsigned char bit7ge2[] = { 0x00, 0x00, 0x02, 0x02, 0x02, 0x02 };
static unsigned char bit8ge2[] = { 0x00, 0x00, 0x01, 0x01, 0x01, 0x01 };


int
tiff2pcl (FILE *prn_stream, const char *tiff_filename)
{
    int compression = 2, rskip = 0, factor, sum, out_line_size,
        topx, topy;
    unsigned char *image = NULL, *pclb = NULL, *outb = NULL, 
        *im[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    register unsigned char *i1, *i2, *i3, *i4, *i5, *i6, *i7, *i8, *o;
    register int x;
    TIFF *tif;
    tsize_t scanline;
    //FILE *prn_stream = stdout;
    int xdpi = 0, ydpi = 0, resolution = 0, line_size_words = 0, dpi = 300;
    uint16 config, bitspersample, samplesperpixel, bitsperpixel, 
           bytesperrow;
    uint32 row, w, h, pad = 0, *data_words, *eim[8];
    static uint32 buffersize = 0;
    int debug = 1;
    //const char *tiff_filename = "image.tiff";
    
    progname = "progname";

    //debug = 1;
    if (dpi != 75 && dpi != 150 && dpi != 300) {
	fprintf(stderr, 
		"%s: error: output PCL resolution of %ddpi not supported!\n",
		progname, dpi);
	return -1;
    }
  
	if (NULL == (tif = TIFFOpen(tiff_filename, "r"))) {
	    fprintf(stderr, "%s: %s: error: cannot open file for reading!\n",
		    progname, tiff_filename);
	    return -1;	
	}
	/* add 4 bytes to scanline size so that fast G4 decoding
	 * can take place (this should really be done by the library...) */
	scanline = sizeof(long) * (TIFFScanlineSize(tif) / sizeof(long) + 1);
	if (!TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h)) {
	    fprintf(stderr, "%s: %s: error: cannot get imagelength!\n", 
		    progname, tiff_filename);
	    return -1;
	}
	if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w)) {
	    fprintf(stderr, "%s: %s: error: cannot get imagewidth!\n", 
		    progname, tiff_filename);
	    return -1;
	}
	if (TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config) &&
	    config != PLANARCONFIG_CONTIG) {
	    fprintf(stderr, 
		    "%s: %s: error: file has non-planar configuration!\n",
		progname, tiff_filename);
	    return -1;
	}
    if (debug) {
        fprintf(stderr, 
                "tiff width %lu, height %lu, config %d\n", w, h, config);
    }

	if (!TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample))
	    bitspersample = 1;
	if (!TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel))
	    samplesperpixel = 1;
	bitsperpixel = bitspersample * samplesperpixel;

	/* calculate resolution based on pixel size rather than
	 * what's in the TIFF header (assume it's a multiple of 75 dpi) */
	for (xdpi = 75; ((double)w/(double)xdpi) > PGSIZEX + MARGIN; xdpi *= 2)
	    {}
	for (ydpi = 75; ((double)h/(double)ydpi) > PGSIZEY; ydpi *= 2)
	    {}
	resolution = max(xdpi,ydpi);

	if (debug)
	    fprintf(stderr, "%s: input file resolution set to %ddpi\n",
		    progname, resolution);

	/* make sure width is at least a multiple of 8 pixels, possibly 32 */
	if (w % 32) {
	    uint32 ow = w;
	    if (debug)
		fprintf(stderr, 
			"%s: %s: # of rows (%ld) is not a multiple of 32!\n", 
			progname, tiff_filename, w);
	    w = 32 * (w / 32 + (w % 32 > 0));
	    pad = ~(unsigned long)0 << (w-ow);
        }

	factor = resolution / dpi;
	if (factor != 1 && factor != 2 && factor != 4) {
	    fprintf(stderr, "%s: %s: cannot reduce image by a factor of %d\n",
		    progname, tiff_filename, factor);
	    return -1;
	}
	if (debug)
	    fprintf(stderr, "%s: reducing image by a factor of %d\n",
		    progname, factor);

	bytesperrow = (w / 8) * bitsperpixel;
	if (image == NULL) {
	    buffersize = scanline;
	    image = (unsigned char *) malloc (factor * buffersize);
	    outb = (unsigned char *) malloc (4 * buffersize / factor);
	    if (factor > 1)
		pclb = (unsigned char *) malloc (buffersize / factor);
	} else if (buffersize < scanline) {
	    if (debug) 
		fprintf(stderr, 
			"%s: increasing output buffer size from %ld to %ld\n",
			progname, buffersize, (long)((h + 8) * scanline));
	    buffersize = scanline;
	    image = (unsigned char *) 
		realloc ((void *)image, factor * buffersize);
	    outb = (unsigned char *) 
		realloc ((void *)outb, 4 * buffersize / factor);
	    if (factor > 1)
		pclb = (unsigned char *) 
		    realloc ((void *)pclb, buffersize / factor);
	}
	if (image == NULL || outb == NULL) { 
	    fprintf(stderr, 
		    "%s: %s: error: cannot allocate image buffer\n", 
		    progname, tiff_filename);
	    return -1;
	} else if (pclb == NULL && factor > 1) {
	    fprintf(stderr, 
		    "%s: %s: error: cannot allocate image buffer\n", 
		    progname, tiff_filename);
	    return -1;
	}

	/* trim document to US letter size */
	if (h > resolution * (PGSIZEY - TOP_MARGIN - BOTTOM_MARGIN)) {
	    rskip = min(0, ((h - resolution * PGSIZEY) / 2));
	    /* shift page up to avoid bottom unprintable margin */
	    rskip += TOP_MARGIN * resolution;
	    h = min(h, resolution * (PGSIZEY - TOP_MARGIN - BOTTOM_MARGIN));
	}
	if (h % factor) 
	    h = factor * (h / factor);

	topx = ((PGSIZEX - LEFT_MARGIN) * dpi - w / factor) / 2.0;
	topy = ((PGSIZEY - TOP_MARGIN - BOTTOM_MARGIN) * dpi - h / factor) / 2.0;
	topx = max(0,topx);
	topy = max(0,topy);

	if (debug) {
	    fprintf(stderr, 
		    "%s: %s: skipping %d pixels (%f inches) from top\n",
		    progname, tiff_filename, rskip, 
		    (double)rskip / (double)resolution);
	    fprintf(stderr, 
		    "%s: %s: PCL page length is %d pixels (%f inches)\n",
		    progname, tiff_filename, (int)(h / factor), 
		    (double)h / (double)resolution);
	    fprintf(stderr,
		    "%s: %s: ULC of PCL image set to x=%d, y=%d (%f,%f inches)\n",
		    progname, tiff_filename, topx, topy,
		    (double)topx / (double)dpi, 
		    (double)topy / (double)dpi);
	    fprintf(stderr,
		    "%s: %s: LRC of PCL image is at x=%lu, y=%lu (%f,%f inches)\n",
		    progname, tiff_filename, topx + w / factor, topy + h / factor,
		    (double)(topx + w / factor) / (double)dpi, 
		    (double)(topy + h / factor) / (double)dpi);
            }

	/* Initialize PCL page. */
	//fputs("\033E\033*rB", prn_stream);
	/* set resolution */
	//fprintf(prn_stream, "\033*t%dR", dpi);
	/* set paper size to letter, portrait orientation, top margin to 0 */
	//fputs("\033&l2A\033&l0o0e0L", prn_stream);
	/* set cursor to (topx,topy) position, 
	 * left margin to current X position */
	fprintf(prn_stream, "\033*p%dx%dY\033*r1A", topx, topy);
	/* set compression mode */
	fprintf(prn_stream, "\033*b%dM", compression);

	/* initialize pointers, variables used in loop */
	out_line_size = bytesperrow / factor;
	line_size_words = out_line_size / sizeof(unsigned long);
	for (x = 0; x < factor; x++) {
	    im[x] = &image[x * scanline];
	    eim[x] = (unsigned long *)im[x] + bytesperrow / factor - 1;
	}

	/* skip top margin */
	for (row = 0; row < rskip; row++) {
	    if (TIFFReadScanline(tif, image, row, 0) < 0) {
	        fprintf(stderr, "%s: %s: error reading scanline %ld\n",
			progname, tiff_filename, row);
		continue;
	    }
	}

	/* read line by line */
        while (row < h + rskip ) {
	    register unsigned long *exam;
	    unsigned long *end_data;
	    register unsigned char *cptr = outb; 
	    int count = 0;

	    for (x = 0; x < factor; x++) {
		if (TIFFReadScanline(tif, im[x], row++, 0) < 0) {
		    fprintf(stderr, "%s: %s: error reading scanline %ld\n",
			    progname, tiff_filename, row - 1);
		    continue;
		}
		*(eim[x]) &= pad;
	    }

	    switch (factor) {

	    case 4:	
		/* fast 4x4 bitmap reduction */
		exam = data_words = (unsigned long *)pclb;
		end_data = data_words + line_size_words - 1;
		i1 = im[0]; i2 = im[2]; i3 = im[2]; i4 = im[3];
		i5 = im[4]; i6 = im[5]; i7 = im[6]; i8 = im[7];
		o = pclb;
		for (x = 0; x < out_line_size; x++, o++) {
		    *o = 0;
		    sum = lut4hi[*i1]   + lut4hi[*i2]   + lut4hi[*i3]   + lut4hi[*i4];
		    *o |= bit1ge6[sum];
		    sum = lut4lo[*i1++] + lut4lo[*i2++] + lut4lo[*i3++] + lut4lo[*i4++];
		    *o |= bit2ge6[sum];
		    sum = lut4hi[*i1]   + lut4hi[*i2]   + lut4hi[*i3]   + lut4hi[*i4];
		    *o |= bit3ge6[sum];
		    sum = lut4lo[*i1++] + lut4lo[*i2++] + lut4lo[*i3++] + lut4lo[*i4++];
		    *o |= bit4ge6[sum];
		    sum = lut4hi[*i1]   + lut4hi[*i2]   + lut4hi[*i3]   + lut4hi[*i4];
		    *o |= bit5ge6[sum];
		    sum = lut4lo[*i1++] + lut4lo[*i2++] + lut4lo[*i3++] + lut4lo[*i4++];
		    *o |= bit6ge6[sum];
		    sum = lut4hi[*i1]   + lut4hi[*i2]   + lut4hi[*i3]   + lut4hi[*i4];
		    *o |= bit7ge6[sum];
		    sum = lut4lo[*i1++] + lut4lo[*i2++] + lut4lo[*i3++] + lut4lo[*i4++];
		    *o |= bit8ge6[sum];
		}
		break;

	    case 2:
		/* fast 2x2 bitmap reduction */
		exam = data_words = (unsigned long *)pclb;
		end_data = data_words + line_size_words - 1;
		i1 = im[0]; i2 = im[1];
		o = pclb;
		for (x = 0; x < out_line_size; x++, o++) {
		    *o = 0;
		    sum = lut2hi1[*i1]   + lut2hi1[*i2];
		    *o |= bit1ge2[sum];
		    sum = lut2hi2[*i1]   + lut2hi2[*i2];
		    *o |= bit2ge2[sum];
		    sum = lut2lo1[*i1]   + lut2lo1[*i2];
		    *o |= bit3ge2[sum];
		    sum = lut2lo2[*i1++] + lut2lo2[*i2++];
		    *o |= bit4ge2[sum];
		    sum = lut2hi1[*i1]   + lut2hi1[*i2];
		    *o |= bit5ge2[sum];
		    sum = lut2hi2[*i1]   + lut2hi2[*i2];
		    *o |= bit6ge2[sum];
		    sum = lut2lo1[*i1]   + lut2lo1[*i2];
		    *o |= bit7ge2[sum];
		    sum = lut2lo2[*i1++] + lut2lo2[*i2++];
		    *o |= bit8ge2[sum];
		}
		break;

	    case 1:
		/* no reduction necessary */
		exam = data_words = (unsigned long *)im[0];
		end_data = data_words + line_size_words - 1;
		break;
		
	    default:
		fprintf(stderr, 
			"%s: %s: cannot reduce image by a factor of %d\n",
			progname, tiff_filename, factor);
		return -1;
		break;
	    }

	    /* Remove trailing 0s. */
	    while (end_data >= data_words && *end_data == 0) 
		 end_data--;
	    if (++end_data == data_words) {
		fputs("\033*bW", prn_stream);
		continue;
	    }

	    while (exam < end_data) {
		/* Search ahead in the input looking for a run 
		 * of at least 4 identical bytes. */
		unsigned char *compr = (unsigned char *)exam;
		unsigned char *end_dis;
		unsigned long *next;
		register unsigned long test = *exam;
		while (((test << 8) ^ test) > 0xff) {
		    if (++exam >= end_data)
			break;
		    test = *exam;
		}

		/* Find out how long the run is */
		end_dis = (unsigned char *)exam;
		if (exam == end_data) {
		    /* no run */
		    /* See if any of the last 3 "dissimilar" bytes are 0. */
		    if (end_dis > compr && end_dis[-1] == 0) {
			if (end_dis[-2] != 0) 
			    end_dis--;
			else if (end_dis[-3] != 0) 
			    end_dis -= 2;
			else 
			    end_dis -= 3;
		    }
		    next = --end_data;
		}
		else {
		    next = exam + 1;
		    while (next < end_data && *next == test)
			next++;
		    /* See if any of the last 3 "dissimilar" bytes */
		    /* are the same as the repeated byte. */
		    if (end_dis > compr && 
			end_dis[-1] == (unsigned char)test) {
			if (end_dis[-2] != (unsigned char)test) 
			    end_dis--;
			else if (end_dis[-3] != (unsigned char)test) 
			    end_dis -= 2;
			else 
			    end_dis -= 3;
		    }
		}

	        /* Now [compr..end_dis) should be encoded as dissimilar, */
		/* and [end_dis..next) should be encoded as similar. */
		/* Note that either of these ranges may be empty. */
		for ( ; ; ) {
		    /* Encode up to 127 dissimilar bytes */
		    count = end_dis - compr;
		    count = (count > 0) ? count : 0;
		    switch (count) {
			/* Use memcpy only if it's worthwhile. */
		    case 6: cptr[6] = compr[5];
		    case 5: cptr[5] = compr[4];
		    case 4: cptr[4] = compr[3];
		    case 3: cptr[3] = compr[2];
		    case 2: cptr[2] = compr[1];
		    case 1: cptr[1] = compr[0];
			*cptr = count - 1;
			cptr += count + 1;
		    case 0: /* all done */
			break;
		    default:
			if (count > 127) 
			    count = 127;
			*cptr++ = count - 1;
			memcpy(cptr, compr, count);
			cptr += count;
			compr += count;
			continue;
		    }
		    break;
		}
        
		/* Encode up to 127 similar bytes. */
		/* Note that count may be < 0 at end of row. */
		count = (const unsigned char *)next - end_dis;
		while (count > 0) {
		    int this = (count > 127 ? 127 : count);
		    *cptr++ = 257 - this;
		    *cptr++ = (unsigned char)test;
		    count -= this;
		}
		exam = next;
	    }
	    
	    /* now write out compressed row */
	    count = cptr - outb;
	    fprintf(prn_stream, "\033*b%dW", count);
	    fwrite(outb, 1, count, prn_stream);
	}

	/* end raster graphics and eject page */
	fputs("\033*rB\f", prn_stream);
	fflush(prn_stream);

	TIFFClose(tif);

    return 0;
}



