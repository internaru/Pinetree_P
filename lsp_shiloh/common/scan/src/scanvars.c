/**
 * \file scanvars.c
 *
 * \brief  Data controlling the current scan.
 *
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
 
#include <stddef.h>
#include <string.h>
#include <math.h> //for powf()
#include <stdio.h>
// NOTE!!!  Not using pow() which is double, using powf() which is float
// we don't need the double precision, and the pow() implementation seems flakey

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"
#include "utils.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "pichw.h"
#include "pic.h"
#include "piehw.h"
#include "piehphw.h"
#include "pie.h"
#include "scanvars.h"
#include "siqdata.h"
#include "siqparse.h"
#include "scanvarsiq.h"
#include "scan.h"
#include "scansen.h"
#include "cal.h"
#include "scantask.h"
#include "ostools.h"
#include "fraction.h"
#include "scancap.h"
#include "grayline.h"
#include "safetylock.h"
#include "scands.h"
#include "scanplat.h"

//#ifdef SV_MSG_DEBUG
#if 0
#define sv_dbg2 dbg1
#else
#define sv_dbg2(...)
#endif

#define SDH_SIQ_TUNING	0
#define SDH_COPY_TUNING	1
#define SDH_FAX_TUNING	1
#define SDH_SCAN_TUNING	1
#define SDH_MOIRE_TUNING_AI_LUT 1
#define SDH_MOIRE_TUNING_600x300 1	// add.lsh for moire test
#define BG_REM_ADVANCED	1
#define BG_REM_LIMIT_W	215	// Background Working Limit (White) => PIC background level of old S-5 chart
#define SDH_PINETREE_YANGSAN_IC 1

/** Every scanvar "class" is stamped with a unique ID; don't change it once set
 * or things will get mighty confusing while debugging.
 *
 *
 * davep 19-Dec-05
 *
 * >>>>>> BIG FAT NOTE! <<<<<<
 *
 * Do NOT NOT NOT NOT NOT if or switch or whatever to look at the classid.
 * The classID is ONLY a debugging tool or for SIQ to convert human strings
 * "Copy Mono" to a classid. 
 *
 * if( sv->id==SVCID_COPY ) {
 *    piehp_set_size_sram( sv->size_sram );
 * }
 * else if( sv->id==SVCID_SCAN ) {
 *    pie_set_tcns( sv->tcns_table );
 * }
 *
 * It will break future compatibility.  We won't be able to create descendants
 * of a class if we do something dumb and hardwire code behavior to a
 * particular id.
 *
 * Think data-driven. If we need to do something differently based on scan or
 * copy or cal or fax or handwriting recognition or whatever, put it into the
 * scanvar as data.
 *
 * if( sv->has_hp_size_sram ) {
 *    piehp_set_size_sram( sv->hp_size_sram );
 * }
 * if( sv->use_tcns ) {
 *    pie_set_tcns( sv->tcns_table );
 * }
 *
 */

#define    SVCID_ZERO  0  ///< reserved for error conditions

#define    SVCID_BASE 1 ///< virtual base class; should never see 

/* Calibration */
#define    SVCID_CAL        100 ///< virtual base class of calibration ; should never see 
#define    SVCID_MONOCAL    101
#define    SVCID_COLORCAL   102

/* Copy */
#define    SVCID_COPY         200 ///< virtual base class of copies ; should never see 
#define    SVCID_DRAFT_COPY   201
#define    SVCID_TEXT_COPY    202
#define    SVCID_PICTURE_COPY 203
#define    SVCID_FILMPHOTO_COPY  204
#define    SVCID_MIXED_COPY   205
#define    SVCID_COLOR_COPY   206
#define    SVCID_DRAFT_COLOR_COPY   207
#define    SVCID_TEXT_COLOR_COPY    208
#define    SVCID_PICTURE_COLOR_COPY 209
#define    SVCID_FILMPHOTO_COLOR_COPY  210
#define    SVCID_MIXED_COLOR_COPY   211

/* Scan */
#define    SVCID_SCAN       300 ///< virtual base class of scans ; should never see 
#define    SVCID_MONO_SCAN  301
#define    SVCID_COLOR_SCAN 302

/* Fax */
#define    SVCID_FAX        400 ///< virtual base class of fax ; should never see
#define    SVCID_TEXT_FAX   401 ///< optimzied for text
#define    SVCID_MIXED_FAX  402 ///< text and graphics

/* internal function prototypes */
static void mono_scan_set_enhance_curve( struct scanvars *sv );
static void mono_scan_set_gamma_curve( struct scanvars *sv );
//static void mono_scan_set_contrast_curve( struct scanvars *sv );
//static void mono_scan_set_brightness_curve( struct scanvars *sv );
static void color_scan_set_gamma_curve( struct scanvars *sv );
static void color_scan_set_enhance_curve( struct scanvars *sv );
static void mono_copy_set_gamma_curve( struct scanvars *sv );
static void mono_copy_set_enhance_curve( struct scanvars *sv );
#if SDH_FAX_TUNING
#else
static void mono_fax_mixed_set_brightness_curve( struct scanvars *sv );
#endif
static void color_copy_set_gamma_curve( struct scanvars *sv );
static void color_copy_set_enhance_curve( struct scanvars *sv );
static scan_err_t copy_load_lightdark_settings( struct scanvars *sv );

static scan_err_t opalloc_mf_ai_lut( struct scanvars *sv );
static scan_err_t opalloc_pic_bdr_lut( struct scanvars *sv );
static scan_err_t opalloc_tcns_lut( struct scanvars *sv );
static scan_err_t opalloc_csc_rgb_lut( struct scanvars *sv );

/** lookup table to convert name from siqfile into class id so we can find our
 * data 
 */
static const struct siqdata_name scanvar_siq_names[] = {
    { "Mono Copy Draft", 15, SVCID_DRAFT_COPY },
    { "Mono Copy Text",  14, SVCID_TEXT_COPY },
    { "Mono Copy Picture", 17, SVCID_PICTURE_COPY },
    { "Mono Copy Film",  14, SVCID_FILMPHOTO_COPY },
    { "Mono Copy Mixed", 15, SVCID_MIXED_COPY },

    { "Color Copy Draft", 16, SVCID_DRAFT_COLOR_COPY },
    { "Color Copy Text",  15, SVCID_TEXT_COLOR_COPY },
    { "Color Copy Picture", 18, SVCID_PICTURE_COLOR_COPY },
    { "Color Copy Film",  15, SVCID_FILMPHOTO_COLOR_COPY },
    { "Color Copy Mixed", 16, SVCID_MIXED_COLOR_COPY },

    { "Color Scan Host", 15, SVCID_COLOR_SCAN },
    { "Mono Scan Host",  14, SVCID_MONO_SCAN  },

    { "Mono Fax Text",   13, SVCID_TEXT_FAX },
    { "Mono Fax Mixed",  14, SVCID_MIXED_FAX },

    { NULL, 0, 0 },
};

/*
 * Standard sRGB to Ycc conversion coefficients:
 *    0.2990  0.5870  0.1140
 *   -0.1687 -0.3313  0.5000
 *    0.5000 -0.4187 -0.0813
 * 
 * Standard YCC to sRGB Coefficients:
 *    1.0000  0.0000  1.4020 //Y  --> R
 *    1.0000 -0.3441 -0.7141 //Cr --> G
 *    1.0000  1.7720 -0.0001 //Cb --> B
 */

/* The actual matrix programmed into the ASIC is the matrix above multiplied
 * by a normalization factor of 2^(Coefficient_Width - 2) (12 bits--4096).
 * Negative values are represented as a 2's-complement hex number.
 */
static const uint32_t esrgb_to_ycc[9] = { 

    // Standard RGB-->Ycc coefficients
   /*0.2990  0.5870  0.1140
    -0.1687 -0.3313  0.5000
     0.5000 -0.4187 -0.0813*/

    // Register values for standard coefficients.
    /*
    1225, 2404, 467,
    0x3d5e, 0x3ab3, 2048,
    2048, 0x3943, 0x3ebd
    */

    // These coefficients map sRGB to Ycc and also perform a color correction
    // to match our color scans more closely to the LJ3050 series.
    // The color correction coefficients were determined by using MatLab to
    // perform a least-squares fit of the scanned output for the Kodak Q60
    // target to the LJ3050 output for the Q60 target.
    //
    // The resulting MatLab-generated matrix was then transposed and multiplied
    // by the sRGB to Ycc coefficients above. --btsmith 3/22/06

//    1283,   2958,   0x3f94,
//    0x3d5e, 0x33fe, 3741,
//    2482,   0x37c6, 0x3e9b

    /* davep 05-Sep-2006 ; switch back to textbook values */
    0x4c9, 0x964, 0x1d3, 
    0x3d4d, 0x3ab3, 0x800, 
    0x800, 0x394d, 0x3eb3
};

/* The actual matrix programmed into the ASIC is the matrix above multiplied
 * by a normalization factor of 2^(Coefficient_Width - 2) (12 bits--4096).
 * Negative values are represented as a 2's-complement hex number.
 */
static const uint32_t ycc_to_esrgb[9] = {
    0x1000, 0x0, 0x166f, 
    0x1000, 0x3a7f, 0x3493, 
    0x1000, 0x1c5a, 0x0
};

/* davep 09-Jan-2013 ; workaround for ASICs that don't support PIE depogo RGB
 * format (output is BGR). These coefficients will swap the R/B planes. 
 */
static const uint32_t ycc_to_bgr[9] = {
    0x1000, 0x1c5a, 0x0,
    0x1000, 0x3a7f, 0x3493, 
    0x1000, 0x0, 0x166f, 
};

static const uint8_t
mono_copy_text_ai_lut_y[PIE_FILTER_LUT_SIZE] = {	// mod.lsh.for SDH tuning
    0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

static const uint8_t
mono_fax_ai_lut_y[PIE_FILTER_LUT_SIZE] = {	// mod.lsh.for SDH tuning
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint8_t
mono_copy_mixed_ai_lut_y[PIE_FILTER_LUT_SIZE] = {	// mod.lsh.for SDH tuning
#if SDH_MOIRE_TUNING_AI_LUT	//mod.lsh for moire test
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32
#else
    0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
#endif
};

static const uint8_t
mono_copy_film_ai_lut_y[PIE_FILTER_LUT_SIZE] = {	// mod.lsh.for SDH tuning
#if 0	// mod.lsh for moire
    -6, -6, -6, -6, -6, -6, -6, -6,
    -4, -4, -4, -4, -4, -4, -4, -4,
    -2, -2, -2, -2, -2, -2, -2, -2,
    0, 0, 0, 0, 0, 0, 0, 0,
    8, 8, 8, 8, 8, 8, 8, 8,
    16, 16, 16, 16, 16, 16, 16, 16,
    32, 32, 32, 32, 32, 32, 32, 32,
    64, 64, 64, 64, 64, 64, 64, 64
#else
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
#endif
};

static const uint8_t
mono_copy_picture_ai_lut_y[PIE_FILTER_LUT_SIZE] = {
    -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
    -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
    -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
    -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
    -12, -12, -12, -12,
};

uint32_t mono_copy_text_var_coefs_y[15] = {			// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
uint32_t mono_copy_text_var_coefs_chroma[15] = {	// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};

uint32_t mono_copy_mixed_var_coefs_y[15] = {		// mod.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
uint32_t mono_copy_mixed_var_coefs_chroma[15] = {	// mod.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
    
uint32_t mono_copy_film_var_coefs_y[15] = {			// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
uint32_t mono_copy_film_var_coefs_chroma[15] = {	// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
    
uint32_t mono_copy_picture_var_coefs_y[15] = {		// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
uint32_t mono_copy_picture_var_coefs_chroma[15] = {	// add.lsh.for SDH tuning
    0, 0, 1,  3,  6,
    0, 0, 3, 15, 25,
    0, 0, 6, 25, 44};
/** Three is enough for 1 debug, 1 scan or copy, and 1 calibrate. Feel free to
 * bump up if not enough
 */
/* davep 31-Dec-2008 ; bump to five per discussion with OwenZ; need to handle
 * several nested scans with the software based paper-present sensor
 */
//#define SCANVAR_STACK_MAX 5
#define SCANVAR_STACK_MAX 10


static struct scanvars * scanvar_stack[SCANVAR_STACK_MAX];
static int stack_top;

/* if you're hitting this assert, odds are you're trying to call
 * scanvar_set_XXX() before doing a scanvar_push()
 */
#define STACKTOP(sv)  do { ASSERT(stack_top>0); \
                           sv = scanvar_stack[stack_top-1]; } while( 0 );

static struct safety_lock scanvar_protection_semaphore;

/* davep 11-Nov-2008 ; Bug 11161 ; keep a count of the number of scanvars we've
 * allocated/freed.  Will help track leaked scanvars.  scanvar_new() call be
 * called from outside Scantask thread context so this variable must be
 * protected by the scanvar_protection_semaphore.
 */
static int num_allocated_scanvars;

static const struct copy_brightness_settings film_copy_brightness[11] = {	// mod.lsh.for SDH tuning
	/* index, gamma, whiteclip, blackclip */
	{ 1, 18, 220, 5 },  /* darkest */
    { 2, 18, 215, 5 },
    { 3, 18, 210, 5 },
    { 4, 18, 205, 5 },
    { 5, 18, 200, 5 },
	{ 6, 18, 195, 5 },
    { 7, 18, 190, 5 },
    { 8, 18, 185, 5 },
    { 9, 18, 180, 5 },
    // not used (Pinetree)
    {10, 18, 175, 5 },
    {11, 18, 170, 5 },  /* lightest */
};

static const struct copy_brightness_settings mixed_copy_brightness[11] = {	// mod.lsh.for SDH tuning
	/* index, gamma, whiteclip, blackclip */
#if SDH_PINETREE_YANGSAN_IC
	{ 1, 20-4, 240, 0 },  /* darkest */
    { 2, 20, 250, 0 },
    { 3, 20+2, 260, 0 },
    { 4, 20+4, 265, 0 },
    { 5, 20+6, 270, 0 },
	{ 6, 20+12, 270, 0 },
    { 7, 20+16, 270, 0 },
    { 8, 20+20, 270, 0 },
    { 9, 20+24, 270, 0 },
#else
	{ 1, 20, 390, 0 },  /* darkest */
    { 2, 20, 360, 0 },
    { 3, 20, 330, 0 }, 
    { 4, 20, 300, 0 },
    { 5, 20, 270, 0 },
	{ 6, 20, 240, 0 },
    { 7, 20, 210, 0 },
    { 8, 20, 190, 0 },
    { 9, 20, 160, 0 },
#endif
    // not used (Pinetree)
    {10, 20, 210, 0 },
    {11, 20, 205, 0 },  /* lightest */
};

static const struct copy_brightness_settings text_copy_brightness[11] = {	// mod.lsh.for SDH tuning
    /* index, gamma, whiteclip, blackclip */
	{ 1, 20-8-3, 240-5, 0 },  /* darkest */
    { 2, 20-2-3, 250-5, 0 },
    { 3, 20-3, 260-5, 0 },
    { 4, 20+4-3, 265-5, 0 },
    { 5, 20+8-3, 270-5, 0 },
	{ 6, 20+12-3, 270-5, 0 },
    { 7, 20+16-3, 270-5, 0 },
    { 8, 20+20-3, 270-5, 0 },
    { 9, 20+24-3, 270-5, 0 },
    // not used (Pinetree)
    {10, 20, 210, 0 },
    {11, 20, 205, 0 },  /* lightest */
};

/************ SDH IMAGE TUNING *********************/
#if SDH_FAX_TUNING
static const struct copy_brightness_settings mixed_fax_brightness[11] = {	// mod.lsh.for SDH tuning
	/* index, gamma, whiteclip, blackclip */
#if 0	// mod.lsh for second fax tunning
	{ 1, 22, 240, 0 },  /* darkest */
    { 2, 22, 230, 0 },
    { 3, 22, 220, 0 }, 
    { 4, 22, 210, 0 },
    { 5, 22, 200, 0 },
	{ 6, 22, 190, 0 },
    { 7, 22, 180, 0 },
    { 8, 22, 170, 0 },
    { 9, 22, 160, 0 },
#else
	{ 1, 20, 235, 30 },  /* darkest */
    { 2, 20, 225, 30 },
    { 3, 20, 215, 30 }, 
    { 4, 20, 205, 30 },
    { 5, 20, 200, 30 },
	{ 6, 20, 195, 30 },
    { 7, 20, 190, 30 },
    { 8, 20, 185, 30 },
    { 9, 20, 180, 30 },
#endif
    // not used (Pinetree)
    {10, 22, 150, 0 },
    {11, 22, 140, 0 },  /* lightest */
};

static const struct copy_brightness_settings text_fax_brightness[11] = {	// mod.lsh.for SDH tuning
    /* index, gamma, whiteclip, blackclip */
#if 0
	{ 1, 18, 160, 75 },  /* darkest */
    { 2, 18, 150, 65 },
    { 3, 18, 140, 55 },
    { 4, 18, 130, 45 },
    { 5, 18, 120, 35 },
	{ 6, 18, 110, 25 },
    { 7, 18, 100, 15 },
    { 8, 18, 90, 5 },
    { 9, 18, 80, 0 },
#else
	{ 1, 11, 160, 90 },  /* darkest */
    { 2, 15, 170, 70 },
    { 3, 17, 180, 50 },
    { 4, 19, 190, 40 },
    { 5, 21, 200, 35 },
	{ 6, 23, 190, 30 },
    { 7, 26, 180, 25 },
    { 8, 29, 170, 20 },
    { 9, 32, 150, 15 },
#endif
    // not used (Pinetree)
    {10, 18, 70, 0 },
    {11, 18, 60, 0 },  /* lightest */
};
#endif
typedef enum{
    TEXT_M = 0,
    DRAFT_M = 1,
    MIXED_M = 2,
    FILM_PHOTO_M = 3,
    PICTURE_M = 4,
    FAX_TEXT_M = 7, 
    FAX_MIXED_M = 8,
    COPYQUALITY_MAX_M
}COPYQUALITY_M; ///< Copy Quality

static const struct copy_offset_settings sdh_film_copy_offset[11] = {	// add.lsh.for SDH tuning (lighten dark tone)
	/* index, white offset, black offset, white guard, black guard */
	{ 1, 0, 0,	 0, 0},  /* darkest */
    { 2, 0, 30,	 0, 0},
    { 3, 0, 60,	 0, 0},
    { 4, 0, 100, 0, 0},
    { 5, 0, 130, 0, 0},
	{ 6, 0, 160, 0, 0},
    { 7, 0, 190, 0, 0},
    { 8, 0, 220, 0, 0},
    { 9, 0, 250, 0, 0},
    // not used (Pinetree)
    { 10, 0, 280, 0, 0},
    { 11, 0, 310, 0, 0},  /* lightest */
};

static const struct copy_offset_settings sdh_text_copy_offset[11] = {	// add.lsh.for SDH tuning (lighten dark tone)
	/* index, white offset, black offset, white guard, black guard */
	{ 1, 0, 210-40, 70, 0},  /* darkest */
    { 2, 0, 210-40, 80, 0},
    { 3, 0, 210-40, 80, 0},
    { 4, 0, 210-40, 80, 0},
    { 5, 0, 210-40, 80, 0},
	{ 6, 0, 210-40, 80, 0},
    { 7, 0, 210-40, 80, 0},
    { 8, 0, 210-40, 80, 0},
    { 9, 0, 210-40, 80, 0},
    // not used (Pinetree)
    { 10, 0, 460, 140, 0},
    { 11, 0, 510, 150, 0},  /* lightest */
};

static const struct copy_offset_settings sdh_mixed_copy_offset[11] = {	// add.lsh.for SDH tuning (lighten dark tone)
	/* index, white offset, black offset, white guard, black guard */
#if SDH_PINETREE_YANGSAN_IC
	{ 1, 0, 210-40, 80, 0},  /* darkest */
    { 2, 0, 210-40, 90, 0},
    { 3, 0, 210-40, 90, 0},
    { 4, 0, 210-40, 90, 0},
    { 5, 0, 210-40, 90, 0},
	{ 6, 0, 210-40, 90, 0},
    { 7, 0, 210-40, 90, 0},
    { 8, 0, 210-40, 90, 0},
    { 9, 0, 210-40, 90, 0},
#else
	{ 1, 0, 10,  20, 0},  /* darkest */
    { 2, 0, 60,  40, 0},
    { 3, 0, 110, 60, 0},
    { 4, 0, 160, 75, 0},
    { 5, 0, 210, 90, 0},
	{ 6, 0, 260, 95, 0},
    { 7, 0, 310, 100, 0},
    { 8, 0, 360, 105, 0},
    { 9, 0, 410, 110, 0},
#endif
    // not used (Pinetree)
    { 10, 0, 460, 140, 0},
    { 11, 0, 510, 150, 0},  /* lightest */
};

#if SDH_FAX_TUNING
static const struct copy_offset_settings sdh_text_fax_offset[11] = {	// add.lsh.for SDH tuning (lighten dark tone)
	/* index, white offset, black offset, white guard, black guard */
	{ 1, 0, 0, 0, 0},  /* darkest */
    { 2, 0, 0, 0, 0},
    { 3, 0, 0, 0, 0},
    { 4, 0, 0, 0, 0},
    { 5, 0, 0, 0, 0},
	{ 6, 0, 0, 0, 0},
    { 7, 0, 0, 0, 0},
    { 8, 0, 0, 0, 0},
    { 9, 0, 0, 0, 0},
    // not used (Pinetree)
    { 10, 0, 0, 0, 0},
    { 11, 0, 0, 0, 0},  /* lightest */
};

static const struct copy_offset_settings sdh_mixed_fax_offset[11] = {	// add.lsh.for SDH tuning (lighten dark tone)
	/* index, white offset, black offset, white guard, black guard */
#if 0	// mod.lsh for second fax tunning
	{ 1, 0, 10,  20, 0},  /* darkest */
    { 2, 0, 60,  40, 0},
    { 3, 0, 110, 60, 0},
    { 4, 0, 160, 80, 0},
    { 5, 0, 210, 100, 0},
	{ 6, 0, 260, 115, 0},
    { 7, 0, 310, 110, 0},
    { 8, 0, 360, 115, 0},
    { 9, 0, 410, 120, 0},
#else
	{ 1, 0, 10,  0, 0},  /* darkest */
    { 2, 0, 60,  30, 0},
    { 3, 0, 110, 50, 0},
    { 4, 0, 160, 70, 0},
    { 5, 0, 210, 90, 0},
	{ 6, 0, 260, 95, 0},
    { 7, 0, 310, 100, 0},
    { 8, 0, 360, 105, 0},
    { 9, 0, 410, 110, 0},
#endif
    // not used (Pinetree)
    { 10, 0, 460, 130, 0},
    { 11, 0, 510, 140, 0},  /* lightest */
};
#endif

#if	SDH_COPY_TUNING
static uint32_t sdh_white_offset = 0;	// add.lsh.for SDH tuning (Range : 0~1023)
static uint32_t sdh_black_offset = 210;	// add.lsh.for SDH tuning (Range : 0~1023)
static uint32_t sdh_white_guard = 90;	// add.lsh.for SDH tuning (Range : 0~1023)
static uint32_t sdh_black_guard = 0;	// add.lsh.for SDH tuning (Range : 0~1023)

static int g_id = SVCID_BASE;
#endif

/**************************************************/
static struct {
    int id;
    const struct copy_brightness_settings *settings;
} all_copy_brightness[] = {
    { SVCID_TEXT_COPY,  text_copy_brightness },		// for Text Mode (Pinetree)
    { SVCID_DRAFT_COPY, text_copy_brightness },
    { SVCID_MIXED_COPY, mixed_copy_brightness },	// for Mixed Mode (Pinetree)
    { SVCID_FILMPHOTO_COPY, film_copy_brightness },	// for Photo Mode (Pinetree)
    { SVCID_PICTURE_COPY, film_copy_brightness },
//    { SVCID_PICTURE_COPY, picture_copy_brightness },
#if SDH_FAX_TUNING
    { SVCID_TEXT_FAX, text_fax_brightness },
    { SVCID_MIXED_FAX, mixed_fax_brightness },
#else
    { SVCID_TEXT_FAX, text_copy_brightness },
    { SVCID_MIXED_FAX, mixed_copy_brightness },
#endif
    { 0, NULL }
};

/**
 * \brief initialize the global scanvar stack semaphore 
 *
 *
 * \author David Poole
 * \date 30-Nov-2005
 *
 */

void scanvar_onetime_init( void )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    safety_lock_init( &scanvar_protection_semaphore );
}

static void scanvar_lock( void )
{
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* wait 1 second; using a timeout to catch scofflaws */
    scerr = safety_lock_lock_with_timeout( &scanvar_protection_semaphore, USEC_PER_SECOND );
    XASSERT( scerr==SCANERR_NONE, scerr );
}

static void scanvar_unlock( void )
{
    safety_lock_unlock( &scanvar_protection_semaphore );
}

/**
 * \brief allocate and initialize an uint32_array 
 *
 *
 * \author David Poole
 * \date 23-Dec-2005
 *
 */

scan_err_t uint32_array_new( struct uint32_array *new_array, int num_entries )
{
    int datalen;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* trap memory leaks */
    XASSERT( new_array->data==NULL, num_entries );

    memset( new_array, 0, sizeof(struct uint32_array) );
    
    datalen = num_entries * sizeof(uint32_t);

    new_array->data = MEM_MALLOC( datalen );
    if( new_array->data == NULL ) {
        dbg1( "%s out of memory\n", __FUNCTION__ );
        return SCANERR_OUT_OF_MEMORY;
    }
    memset( new_array->data, 0, datalen );
    new_array->num_entries = num_entries;
    return 0;
}

void
uint32_array_delete( struct uint32_array *array )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 26-Jul-2006 ; check to make sure we actually have some data before
     * we memset it
     */
    XASSERT( array->data != NULL, array->num_entries );

    /* poison the memory before we free it */
    memset( array->data, 0xcc, array->num_entries*sizeof(uint32_t) );

    PTR_FREE( array->data );
    array->num_entries = 0;
}

/**
 * \brief  Copy a raw array of uint32_t into a uint32_array object.
 *
 * If the destination's buffer done exist, they will be created.
 *
 * If the destination's buffer does exist, it must be exactly the same size as
 * the source data.
 *
 * \author David Poole
 * \date 25-Dec-2005
 *
 */

scan_err_t 
uint32_array_copy( struct uint32_array *dst, const uint32_t *src, int num_entries )
{
    int datalen;
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
	
    datalen = num_entries * sizeof(uint32_t);

    /* cheerfully copy empty arrays */
    if( datalen == 0 ) {
        XASSERT( src==NULL, (uint32_t)src );

        dst->data = NULL;
        dst->num_entries = 0;
        return 0;
    }
    else { 
        ASSERT( src!=NULL );
    }

    /* if we don't have data to begin with, allocate it */
    if( dst->data == NULL ) {
        scerr = uint32_array_new( dst, num_entries );
        if( scerr!=0 ) {
            return scerr;
        }
    }
    else { 
        /* Our destination must be exactly the same size otherwise a copy makes
         * no sense. Yes, we could support copying smaller into larger but
         * let's be ultra paranoid here, eh?
         */
        XASSERT( dst->num_entries==num_entries, dst->num_entries );
    }

    memcpy( dst->data, src, datalen );

    return 0;
}

/**
 * \brief copy one uint32_array into another
 *
 * Simply a wrapper around uint32_array_copy() to hide the internals of the
 * uint32_array structure a bit.
 *
 * \author David Poole
 * \date 01-Jan-2006
 *
 */

scan_err_t
uint32_array_clone( struct uint32_array *dst, const struct uint32_array *src )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    return uint32_array_copy( dst, src->data, src->num_entries );
}


/**
 * \brief put a scanvar object onto the config stack 
 *
 * After calling scanvar_new_xxx(), push the object onto the scanvar stack. Any
 * calls to scanvar_get_xxx() or scanvar_() get return the top of the stack.
 * The stack concept allows us to have several active scan configurations
 * simultaneously.
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

void scanvar_push( struct scanvars *sv )
{
    dbg2( "%s %d %p\n", __FUNCTION__, sv->id, sv );
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 11-Nov-2008 ; Bug 11161 ; only Scantask may modify the scanvar
     * stack
     */
#ifdef __KERNEL__
    /* TODO */
#else
    XASSERT( pthread_equal( pthread_self(), scantask_get_id()), (uint32_t)pthread_self() );
#endif

    /* davep 17-Dec-2010 ; add sanity check (will assert on insane scanvar) */
    scanvar_sanity(sv);

    scanvar_lock();

    ASSERT( sv->id > 0 );
    XASSERT( stack_top < SCANVAR_STACK_MAX, stack_top );
    PTR_ASSIGN( scanvar_stack[stack_top], sv );
    stack_top++;

    /* release my just received lock */
    scanvar_unlock();
}

/**
 * \brief pop a scanvar object from the config stack 
 *
 * See also scanvar_push()
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_pop( void )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 11-Nov-2008 ; Bug 11161 ; only Scantask may modify the scanvar
     * stack
     */
#ifdef __KERNEL__
    /* TODO */
#else
    XASSERT( pthread_equal( pthread_self(), scantask_get_id()), (uint32_t)pthread_self() );
#endif

    scanvar_lock();

    XASSERT( stack_top > 0, stack_top );
    stack_top--;
    PTR_ASSIGN( sv, scanvar_stack[stack_top] );
    ASSERT( sv->id > 0 );

    scanvar_unlock();

    /* davep 17-Dec-2010 ; add sanity check (will assert on insane scanvar) */
    scanvar_sanity(sv);

    dbg2( "%s %d %p\n", __FUNCTION__, sv->id, sv );

    return sv;
}

/**
 * \brief get a pointer to the top scanvar from the stack 
 *
 * Will assert if there is no stack top. Use this call when we're expecting a
 * scanvar to be ready and waiting and we don't want to have to test for its
 * existance.
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_get( void )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    STACKTOP(sv);

    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    return sv;
}

/**
 * \brief get a pointer to the top scanvar of the stack 
 *
 * \retval pointer to scanvar
 * \retval NULL if stack is empty
 *
 * Only call this function to test if there is something on the scanvar stack.
 * scanvar_get() is probably the function you're looking for.
 *
 * \code
 * if( scanvar_peek() == NULL ) {
 *    error( "no scanvar active" );
 *    return SCANERR_NO_ENTRY;
 * }
 * scanvar_set_dpi( 300 );
 * \endcode
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

const struct scanvars *
scanvar_peek( void )
{
    const struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( stack_top > 0 ) {

        /* we're sharing this memory between threads so lets make sure no one
         * broke us
         */
        sv = scanvar_stack[stack_top-1];
        XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

        return sv;
    }
    return NULL;
}

void
scanvar_print( const struct scanvars *sv )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%p id=%d dpi=%d type=%d area={%ld,%ld,%ld,%ld} scale=%d/%d,%d/%d\n",
                sv, sv->id, sv->dpi, sv->cmode, 
                sv->user_area_hinch.x, 
                sv->user_area_hinch.y,
                sv->user_area_hinch.height, 
                sv->user_area_hinch.width,
                sv->scale.x_numer, sv->scale.x_denom, 
                sv->scale.y_numer, sv->scale.y_denom );
               
}

void 
scanvar_stackdump( void )
{
    int i;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    i = stack_top - 1;
    while( i>=0 ) {
        scanvar_print( scanvar_stack[i] );
        i--;
    }
}

/*************************************
 * Base "Class"                      *
 *************************************/

/**
 * \brief "Root" class of the scanvar class hierarchy
 *
 * scanvar_new() allocates heap memory for the scanvar "class" instance and
 * initializes a few members to safe values. 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * 
scanvar_new( void )
{
    scan_err_t scerr;
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv = MEM_MALLOC( sizeof(struct scanvars) );
    if( sv==NULL ) {
        dbg1( "no memory for scanvar!\n" );
        return NULL;
    }

    /* davep 11-Nov-2008 ; now that we're passing scanvars around in messages,
     * need to be much more careful about memory leaks
     */
    scanvar_lock();
    num_allocated_scanvars += 1;
    XASSERT( num_allocated_scanvars <= SCANVAR_STACK_MAX, num_allocated_scanvars );
    /* davep 03-Mar-2010 ; report the allocated so we can watch for leaks over
     * time
     */
    dbg1( "%s num_allocated=%d\n", __FUNCTION__, num_allocated_scanvars );
    scanvar_unlock();

    /* set up the "virtual base class" */
    memset( sv, 0, sizeof( struct scanvars ) );

    /* to verify structure pointers in messages */
    sv->cookie = SCANVAR_COOKIE;

    g_id = sv->id = SVCID_BASE;

    /* TODO pick more sane defaults */
    sv->scale.x_numer = 1;
    sv->scale.x_denom = 1;
    sv->scale.y_numer = 1;
    sv->scale.y_denom = 1;

    /* default to Letter size */
    sv->user_area_hinch.x = 0;
    sv->user_area_hinch.y = 0;
    sv->user_area_hinch.width = 850;
    sv->user_area_hinch.height = 1100;

    /* davep 23-Jun-2010 ; PIC will DMA 8-bit pixels to memory */
//    sv->pic_bpp = 8;
    /* PIC will DMA 16-bit pixels to memory */
    sv->pic_bpp = 16;

    /* davep 17-Dec-2010 ; use scanvar_set_dpi() so I can get the automatic
     * calculation of the hardware dpi
     */
    scerr = scanvar_set_dpi( sv, 300 );
    XASSERT( scerr==0, scerr );

    /* davep 15-Jul-2006 ; as of this writing, we're always enabling the scaler
     * when we run through PIE so we always have to set scaler to one or the
     * other or we'll assert. Eventually I want to get away from always
     * enabling the scaler.
     */
    sv->xy_scaler_mode = PIE_XY_SCALER_RAPR;

    return sv;
}

/**
 * \brief  Sanity test to catch mismatches of "use_xxx" and the scanvar_delete()
 * destructor.
 *
 * Created when I ran into a case of allocating hp_diffus_lut but not setting
 * "use_hp_diffus_lut" so it wasn't freed.
 *
 * uint32_array_delete() will set the lut.data pointer to NULL. Make sure
 * there's nothing allocated that wasn't freed.
 *
 * Note: this function will of course have to be updated when scanvars struct
 * changes.
 *
 * \author David Poole
 * \date 16-Apr-2008
 *
 */

static void
scanvar_delete_sanity_check( struct scanvars *sv )
{
    int i;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* regardless of whether we used them or not, make sure all pointers are
     * clean
     */
    XASSERT( sv->mf_ai_lut.data==NULL, (uint32_t)sv->mf_ai_lut.data );
    XASSERT( sv->tcns_lut.data==NULL, (uint32_t)sv->tcns_lut.data );
    XASSERT( sv->hp_size_lut.data==NULL, (uint32_t)sv->hp_size_lut.data );
    XASSERT( sv->hp_sbias_lut.data==NULL, (uint32_t)sv->hp_sbias_lut.data );
    XASSERT( sv->hp_density_lut.data==NULL, (uint32_t)sv->hp_density_lut.data );
    XASSERT( sv->hp_diffus_lut.data==NULL, (uint32_t)sv->hp_diffus_lut.data );
    XASSERT( sv->pic_bdr_lut.data==NULL, (uint32_t)sv->pic_bdr_lut.data );
    XASSERT( sv->csc_rgb_lut.data==NULL, (uint32_t)sv->csc_rgb_lut.data );
    XASSERT( sv->csc_r_lut.data==NULL, (uint32_t)sv->csc_r_lut.data );
    XASSERT( sv->csc_g_lut.data==NULL, (uint32_t)sv->csc_g_lut.data );
    XASSERT( sv->csc_b_lut.data==NULL, (uint32_t)sv->csc_b_lut.data );

    /* davep 15-Dec-2008 ; make sure it's not still on the stack */
    for( i=0 ; i<SCANVAR_STACK_MAX ; i++ ) {
        XASSERT( sv!=scanvar_stack[i], (uint32_t)sv );
    }
}

/**
 * \brief delete a ScanVar structure
 *
 * The C++ "class destructor" concept fails here.  I didn't want to force
 * people to match the exact "class" new and delete calls.
 *
 * For example, call scanvar_mixed_copy_new() or scanvar_draft_copy_new() based
 * on settings. Don't want to have to remember the settings and call
 * scanvar_mixed_copy_delete() or scanvar_draft_copy_delete(). Just call
 * scanvar_delete() and all will be well.
 *
 * This "base class destructor" will have to test for any additional memory
 * inside the structure, no matter who down the "class" hierarchy might have
 * allocated it. It's a complete breakage of OOP but it will keep the outside
 * code simple.
 *
 * \author David Poole
 * \date 27-Oct-2005
 *
 */

void
scanvar_delete( struct scanvars **psv )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* stupid human check (double free) */
    ASSERT( psv != NULL );
    ASSERT( *psv != NULL );

    PTR_ASSIGN( sv, *psv );

    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    dbg2( "%s %p %d\n", __FUNCTION__, sv, sv->id );

    /* as we add more members to the structure, test for them here and free
     * the allocated memory BEFORE freeing the structure itself
     */

    if( sv->mf_ai_lut.data ) {
        uint32_array_delete( &sv->mf_ai_lut );
    }
    if( sv->tcns_lut.data ) {
        uint32_array_delete( &sv->tcns_lut );
    }
    if( sv->hp_size_lut.data ) {
        uint32_array_delete( &sv->hp_size_lut );
    }
    if( sv->hp_sbias_lut.data ) {
        uint32_array_delete( &sv->hp_sbias_lut );
    }
    if( sv->hp_density_lut.data ) {
        uint32_array_delete( &sv->hp_density_lut );
    }
    if( sv->hp_diffus_lut.data ) {
        uint32_array_delete( &sv->hp_diffus_lut );
    }
    if( sv->pic_bdr_lut.data ) {
        uint32_array_delete( &sv->pic_bdr_lut );
    }
    if( sv->csc_rgb_lut.data ) {
        uint32_array_delete( &sv->csc_rgb_lut );
    }
    if( sv->csc_r_lut.data ) {
        uint32_array_delete( &sv->csc_r_lut );
    }
    if( sv->csc_g_lut.data ) {
        uint32_array_delete( &sv->csc_g_lut );
    }
    if( sv->csc_b_lut.data ) {
        uint32_array_delete( &sv->csc_b_lut );
    }

    scanvar_delete_sanity_check( sv );

    /* davep 15-Dec-2008 ; poison the memory (more thread safety) */
    memset( sv, 'q', sizeof(struct scanvars) );

    PTR_FREE( sv );

    /* davep 11-Nov-2008 ; now that we're passing scanvars around in messages,
     * need to be much more careful about memory leaks
     */
    scanvar_lock();
    XASSERT( num_allocated_scanvars > 0, num_allocated_scanvars );
    num_allocated_scanvars -= 1;
    /* davep 03-Mar-2010 ; report the allocated so we can watch for leaks over
     * time
     */
    dbg1( "%s num_allocated=%d\n", __FUNCTION__, num_allocated_scanvars );
    scanvar_unlock();
}

/**
 * \brief  run a set of sanity tests on a scanvar
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

void scanvar_sanity( const struct scanvars *sv )
{
    scan_err_t scerr;
    struct scan_scale pie_scale, user_scale;
    struct scan_area area;
    uint32_t horiz_dpi, vert_dpi;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    ASSERT( sv!=NULL );

    dbg2( "%s %p %d\n", __FUNCTION__, sv, sv->id );

    /* make sure the scanvar has a nice set of usable defaults */
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    memset( &area, 0, sizeof(area) );
    memset( &user_scale, 0, sizeof(struct scan_scale) );
    memset( &pie_scale, 0, sizeof(struct scan_scale) );

    scanvar_get_area( sv, &area.x, &area.y, &area.width, &area.height );
    scanvar_get_pie_scale( sv, &pie_scale.x_numer, &pie_scale.x_denom, 
                               &pie_scale.y_numer, &pie_scale.y_denom );
    scanvar_get_pie_scale( sv, &user_scale.x_numer, &user_scale.x_denom, 
                               &user_scale.y_numer, &user_scale.y_denom );

    XASSERT( area.x >= 0, (uint32_t)area.x );
    XASSERT( area.y >= 0, (uint32_t)area.y );
    XASSERT( area.width > 0, (uint32_t)area.width );
    XASSERT( area.height > 0, (uint32_t)area.height );

    scerr = scanvar_verify_xyscale( pie_scale.x_numer, pie_scale.x_denom,
                                    pie_scale.y_numer, pie_scale.y_denom );
    XASSERT( scerr==SCANERR_NONE, scerr );
    scerr = scanvar_verify_xyscale( user_scale.x_numer, user_scale.x_denom,
                                    user_scale.y_numer, user_scale.y_denom );
    XASSERT( scerr==SCANERR_NONE, scerr );

    XASSERT( sv->dpi >= 75 && sv->dpi <= 19200, sv->dpi );

    scanvar_get_hw_dpi( sv, &horiz_dpi, &vert_dpi );
    XASSERT( horiz_dpi > 0 && horiz_dpi <= 2400, horiz_dpi );
    XASSERT( vert_dpi > 0 && horiz_dpi <= 2400, vert_dpi );

    XASSERT( sv->pic_bpp==8 || sv->pic_bpp==16, sv->pic_bpp );
}

/*************************************
 * Calibration "Classes"             *
 *************************************/

/**
 * \brief Calibration scanvar "virtual base class" constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

static struct scanvars * 
scanvar_cal_new( void )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* call parent "constructor" */
    sv = scanvar_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_CAL;

    return sv;
}

/**
 * \brief Mono Scan Calibration white/black targets initializer
 *
 * \author Brad Smith
 * \date 11-Jan-2006
 *
 */

static void
scanvar_mono_scan_cal_white_black_targets( struct scanvars *sv)
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_cal_white_digital_target = true;
    //sv->cal_white_digital_target[SV_COLOR_MONO] = 65535;
    sv->cal_white_digital_target[SV_COLOR_MONO] = 63504 + 1980;

    sv->use_cal_black_digital_target = true;
    sv->cal_black_digital_target[SV_COLOR_MONO] = 1028;

    sv->use_cal_white_analog_target = true;
    
    /* Brad S.*/
    sv->cal_white_analog_target[SV_COLOR_MONO] = 52429;
    sv->cal_white_analog_target[SV_COLOR_GREEN] = 52429;
    sv->cal_white_analog_target[SV_COLOR_BLUE] = 52429;
    
    
    /*Eric H. 90% of Full
    sv->cal_white_analog_target[SV_COLOR_MONO] = 58981;
    sv->cal_white_analog_target[SV_COLOR_GREEN] = 58981;
    sv->cal_white_analog_target[SV_COLOR_BLUE] = 58981;
    */
    

    sv->use_cal_black_analog_target = true;
    sv->cal_black_analog_target[SV_COLOR_MONO] = 2560;
    sv->cal_black_analog_target[SV_COLOR_GREEN] = 2560;
    sv->cal_black_analog_target[SV_COLOR_BLUE] = 2560;
}
/**
 * \brief Color Scan Calibration white/black targets initializer
 *
 * \author Brad Smith
 * \date 11-Jan-2006
 *
 */

static void
scanvar_color_scan_cal_white_black_targets( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_cal_white_digital_target = true;
    sv->cal_white_digital_target[SV_COLOR_RED] = 63504 + 1980;//61000;  
//    sv->cal_white_digital_target[SV_COLOR_RED] = 59000;
//    sv->cal_white_digital_target[SV_COLOR_RED] = 61153;
    sv->cal_white_digital_target[SV_COLOR_GREEN] = 63504 + 1980;//61000; 
    sv->cal_white_digital_target[SV_COLOR_BLUE] = 63504 + 1980;//61000;  

    sv->use_cal_black_digital_target = true;
    sv->cal_black_digital_target[SV_COLOR_RED] = 1028;
    sv->cal_black_digital_target[SV_COLOR_GREEN] = 1028;
    sv->cal_black_digital_target[SV_COLOR_BLUE] = 1028;
    
    /* Brad S.*/
    sv->use_cal_white_analog_target = true;
    sv->cal_white_analog_target[SV_COLOR_RED] = 52429;
    sv->cal_white_analog_target[SV_COLOR_GREEN] = 52429;
    sv->cal_white_analog_target[SV_COLOR_BLUE] = 52429;
    
   
    /*Eric H. 90% of Full
    sv->cal_white_analog_target[SV_COLOR_RED] = 58981;
    sv->cal_white_analog_target[SV_COLOR_GREEN] = 58981;
    sv->cal_white_analog_target[SV_COLOR_BLUE] = 58981;
    */
    
    sv->use_cal_black_analog_target = true;
    sv->cal_black_analog_target[SV_COLOR_RED] = 2560;
    sv->cal_black_analog_target[SV_COLOR_GREEN] = 2560;
    sv->cal_black_analog_target[SV_COLOR_BLUE] = 2560;
}

/**
 * \brief Mono Calibration scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_mono_cal_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* call parent "constructor" */
    sv = scanvar_cal_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_MONOCAL;

    /* set up a chunk of defaults */
    sv->cmode = SCAN_CMODE_MONO;
    scanvar_mono_scan_cal_white_black_targets(sv);

    return sv;
}

/**
 * \brief Color Calibration scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_color_cal_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* call parent "constructor" */
    sv = scanvar_cal_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_COLORCAL;

    /* set up a chunk of defaults */
    sv->cmode = SCAN_CMODE_COLOR;
    scanvar_color_scan_cal_white_black_targets(sv);

    return sv;
}

struct scanvars * scanvar_cal_newcopy( const struct scanvars *src_sv )
{
    struct scanvars *sv;
    int i;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    ASSERT( src_sv != NULL );

    /* call parent "constructor" to make a new scanvar for cal */
    sv = scanvar_cal_new();
    if( sv==NULL ) {
        return NULL;
    }

    /* At this point we have a spanking new calibration scanvar. We override
     * common calibration variables here by taking the bits and pieces from the
     * destination scan's fields.  This sort of thing requires an incredibly
     * detailed knowledge of the internals of scanpipe. Don't go fiddling with
     * this code unless you know what you're doing!
     *
     * Also note that siq merge must have occurred on the src_sv prior
     * to calling this function!
     *
     * Almost all the fields will be left as empty/null/disabled because
     * calibration doesn't use a lot of scanpipe.
     */

    /* base */
    sv->dpi = src_sv->dpi;
    sv->hw_dpi_vert = src_sv->hw_dpi_vert;
    sv->hw_dpi_horiz = src_sv->hw_dpi_horiz;
    sv->cmode = src_sv->cmode;
    sv->user_area_hinch = src_sv->user_area_hinch;
    sv->scan_area_pixels = src_sv->scan_area_pixels;
    sv->scale = src_sv->scale;

    /* PIC */
    sv->pic_bpp = src_sv->pic_bpp;

    /* Cal */
#if 0
    sv->use_cal_white_digital_target = src_sv->use_cal_white_digital_target;
    sv->cal_white_digital_target[SV_COLOR_MONO] = src_sv->cal_white_digital_target[SV_COLOR_MONO];
    sv->cal_white_digital_target[SV_COLOR_RED] = src_sv->cal_white_digital_target[SV_COLOR_RED];
    sv->cal_white_digital_target[SV_COLOR_GREEN] = src_sv->cal_white_digital_target[SV_COLOR_GREEN];
    sv->cal_white_digital_target[SV_COLOR_BLUE] = src_sv->cal_white_digital_target[SV_COLOR_BLUE];
#else //test.kmg
    sv->use_cal_white_digital_target = src_sv->use_cal_white_digital_target;
//    sv->cal_white_digital_target[SV_COLOR_MONO] = src_sv->cal_white_digital_target[SV_COLOR_MONO];
    sv->cal_white_digital_target[SV_COLOR_MONO] = 63504 + 1980; //mono
    sv->cal_white_digital_target[SV_COLOR_RED] = 63504 + 1980;
    sv->cal_white_digital_target[SV_COLOR_GREEN] = 63504 + 1980;
    sv->cal_white_digital_target[SV_COLOR_BLUE] = 63504 + 1980;

#endif    

    sv->use_cal_black_digital_target = src_sv->use_cal_black_digital_target;
    sv->cal_black_digital_target[SV_COLOR_MONO] = src_sv->cal_black_digital_target[SV_COLOR_MONO];
    sv->cal_black_digital_target[SV_COLOR_RED] = src_sv->cal_black_digital_target[SV_COLOR_RED];
    sv->cal_black_digital_target[SV_COLOR_GREEN] = src_sv->cal_black_digital_target[SV_COLOR_GREEN];
    sv->cal_black_digital_target[SV_COLOR_BLUE] = src_sv->cal_black_digital_target[SV_COLOR_BLUE];

    sv->use_cal_white_analog_target = src_sv->use_cal_white_analog_target;
    sv->cal_white_analog_target[SV_COLOR_MONO] = src_sv->cal_white_analog_target[SV_COLOR_MONO];
    sv->cal_white_analog_target[SV_COLOR_RED] = src_sv->cal_white_analog_target[SV_COLOR_RED];
    sv->cal_white_analog_target[SV_COLOR_GREEN] = src_sv->cal_white_analog_target[SV_COLOR_GREEN];
    sv->cal_white_analog_target[SV_COLOR_BLUE] = src_sv->cal_white_analog_target[SV_COLOR_BLUE];

    sv->use_cal_black_analog_target = src_sv->use_cal_black_analog_target;
    sv->cal_black_analog_target[SV_COLOR_MONO] = src_sv->cal_black_analog_target[SV_COLOR_MONO];
    sv->cal_black_analog_target[SV_COLOR_RED] = src_sv->cal_black_analog_target[SV_COLOR_RED];
    sv->cal_black_analog_target[SV_COLOR_GREEN] = src_sv->cal_black_analog_target[SV_COLOR_GREEN];
    sv->cal_black_analog_target[SV_COLOR_BLUE] = src_sv->cal_black_analog_target[SV_COLOR_BLUE];

    // allow the user to disable or force a cal for any given scan
    sv->cal_mode = src_sv->cal_mode;
    /* davep 01-Feb-2007 ; adding ability to override analog offset/gain and
     * led pwm/exp
     */
    sv->use_cal_analog_offset = src_sv->use_cal_analog_offset;
    for( i=0 ; i<SV_COLOR_NUM_ENTRIES ; i++ ) {
        sv->cal_analog_offset[i] = src_sv->cal_analog_offset[i];
    }
    sv->use_cal_analog_gain = src_sv->use_cal_analog_gain;
    for( i=0 ; i<SV_COLOR_NUM_ENTRIES ; i++ ) {
        sv->cal_analog_gain[i] = src_sv->cal_analog_gain[i];
    }
    sv->use_cal_led_pwm = src_sv->use_cal_led_pwm;
    for( i=0 ; i<SV_COLOR_NUM_ENTRIES ; i++ ) {
        sv->cal_led_pwm[i] = src_sv->cal_led_pwm[i];
    }
    sv->use_cal_led_exp = src_sv->use_cal_led_exp;
    for( i=0 ; i<SV_COLOR_NUM_ENTRIES ; i++ ) {
        sv->cal_led_exp[i] = src_sv->cal_led_exp[i];
    }

    sv->use_gray_cal = src_sv->use_gray_cal;

    return sv;
}


/*************************************
 * Copy Job "Classes"                *
 *************************************/


/**
 * \brief  initialize default HalfPack LUTs for 1bpp
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */
#if SDH_FAX_TUNING
#else
static void
init_hp_1bpp_luts( struct uint32_array *size_sram, 
                   struct uint32_array *density_sram )
{
    int cnt; 
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( size_sram->num_entries==PIE_HP_SIZE_LUT_SIZE,
             size_sram->num_entries );
    XASSERT( density_sram->num_entries==PIE_HP_DENSITY_LUT_SIZE,
             density_sram->num_entries );

    // 1bpp has three levels (pixels come in pairs but why aren't there four
    // levels?
    //
    // DougK suggested a 1/6 + 1/3 + 1/2 split.

    // size and density want the same gradient (I think) so do both at once
    memset( size_sram->data, 0, sizeof(uint32_t)*PIE_HP_SIZE_LUT_SIZE );
    memset( density_sram->data, 0, sizeof(uint32_t)*PIE_HP_DENSITY_LUT_SIZE );

    cnt = 0;
    while( cnt < 256/6 ) {
        size_sram->data[cnt] = 0;
        density_sram->data[cnt] = 0;
        cnt += 1;
    }

    // filled up 1/6 of the lut; now fill up the next 1/3 of the lut
    // (1/6+1/3=1/2) which will take us to the halfway point
    while( cnt < 256/2 ) {
        size_sram->data[cnt] = 1;
        density_sram->data[cnt] = 127;
        cnt += 1;
    }

    while( cnt < 256 ) {
        size_sram->data[cnt] = 2;
        density_sram->data[cnt] = 255;
        cnt += 1;
    }
}
#endif
/**
 * \brief initialize default HalfPack LUTs for 2bpp
 *
 * Numbers originally from DougK. The code is hardwired assuming a 256-entry
 * LUT which shouldn't be a problem for a while.  "640k should be enough for
 * everyone." Right? 
 *
 * \author David Poole
 * \date 29-Dec-2005
 *
 */

static void
init_hp_2bpp_luts( struct uint32_array *size_sram, 
                   struct uint32_array *density_sram )
{
    const int levels = 6; /* 2-bpp */
    int i, j, idx; 
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( size_sram->num_entries==PIE_HP_SIZE_LUT_SIZE,
             size_sram->num_entries );
    XASSERT( density_sram->num_entries==PIE_HP_DENSITY_LUT_SIZE,
             density_sram->num_entries );

    /* davep 29-Dec-2005 ; emergency check for future hardware changes which
     * will cause problems with the 256-entry hardwired assumptions below
     * XXX these are stupid but I don't have the guts to change Doug's code.
     */
    XASSERT( PIE_HP_SIZE_LUT_SIZE==256, PIE_HP_SIZE_LUT_SIZE );
    XASSERT( PIE_HP_DENSITY_LUT_SIZE==256, PIE_HP_DENSITY_LUT_SIZE );
    
    memset( size_sram->data, 0, PIE_HP_SIZE_LUT_SIZE );
    memset( density_sram->data, 0, PIE_HP_DENSITY_LUT_SIZE );

    idx = 0;
    for( i=0 ; i<128/levels ; i++ ) {
        size_sram->data[idx++] = 0;
    }
    for( j=1 ; j<levels ; j++ ) {
        for( i=0 ; i<256/levels ; i++ ) {
            size_sram->data[idx++] = j;
        }
    }
    while( idx<256 ) {
        size_sram->data[idx++] = levels;
    }

    idx = 0;
    for( i=0 ; i<128/levels ; i++ ) {
        density_sram->data[idx++] = 0;
    }
    for( j=1 ; j<levels ; j++ ) {
        for( i=0 ; i<256/levels ; i++ ) {
            density_sram->data[idx++] = (j*256)/levels;
        }
    }
    while( idx<256 ) {
        density_sram->data[idx++] = 255;
    }
}

/**
 * \brief Build a matching size lut by walking the values in the density lut 
 *
 * Size and Density LUTs need to have matching step values or Weird Things (tm)
 * happen.
 *
 * \author David Poole
 * \date 05-Oct-2009
 *
 */

#if 0
static void sync_density_lut_to_size_lut( struct uint32_array *size_sram, 
                                          struct uint32_array *density_sram )
{
    int i;
    uint8_t size;
    uint8_t last_density;

    last_density = density_sram->data[0];
    size = 0;

    memset( size_sram->data, 0, PIE_HP_SIZE_LUT_SIZE*sizeof(*size_sram->data) );

    /* walk the density list, building a matching size lut */
    for( i=0 ; i<density_sram->num_entries ; i++ ) {
        if( density_sram->data[i] != last_density ) {
            last_density = density_sram->data[i];
            size += 1;

            /* stupid human check; can only have 16 levels in the size LUT so we
             * can only have 16 levels in the density lut
             */
            XASSERT( size <= 15, size );
        }
        size_sram->data[i] = size;
    }
}
#endif

/**
 * \brief initialize default HalfPack LUTs for 4bpp
 *
 * see comments for init_hp_2bpp_luts()
 *
 * \author David Poole
 * \date 29-Dec-2005
 *
 */
#if 0
static void
init_hp_4bpp_luts( struct uint32_array *size_sram, 
                   struct uint32_array *density_sram )
{
//    const int levels = 16; /* 4bpp */
    int i; 
//    int j, idx; 
    const uint8_t *hp_4bpp_density_lut;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( size_sram->num_entries==PIE_HP_SIZE_LUT_SIZE,
             size_sram->num_entries );
    XASSERT( density_sram->num_entries==PIE_HP_DENSITY_LUT_SIZE,
             density_sram->num_entries );

    /* davep 29-Dec-2005 ; emergency check for future hardware changes which
     * will cause problems with the 256-entry hardwired assumptions below
     * XXX these are stupid but I don't have the guts to change Doug's code.
     */
    XASSERT( PIE_HP_SIZE_LUT_SIZE==256, PIE_HP_SIZE_LUT_SIZE );
    XASSERT( PIE_HP_DENSITY_LUT_SIZE==256, PIE_HP_DENSITY_LUT_SIZE );
    
    memset( size_sram->data, 0, PIE_HP_SIZE_LUT_SIZE*sizeof(*size_sram->data) );
    memset( density_sram->data, 0,
            PIE_HP_DENSITY_LUT_SIZE*sizeof(*density_sram->data) );

#if 0
    /* linear(ish) size lut */
    idx = 0;
    for( i=0 ; i<128/(levels-1) ; i++ ) {
        size_sram->data[idx++] = 0;
    }
    for( j=1 ; j<(levels-1) ; j++ ) {
        for( i=0 ; i<256/(levels-1) ; i++ ) {
            size_sram->data[idx++] = j;
        }
    }
    while( idx<256 ) {
        size_sram->data[idx++] = levels-1;
    }
#endif

    /* davep 25-Apr-2006 ; through much pain and misery, we at last have a
     * density LUT 
     */
//    hp_4bpp_density_lut = scanplat_get_hp_4bpp_density();
    hp_4bpp_density_lut = NULL;
    if( hp_4bpp_density_lut!=NULL ) {
        for( i=0 ; i<PIE_HP_DENSITY_LUT_SIZE ; i++ ) {
            density_sram->data[i] = hp_4bpp_density_lut[i];
        }

        /* davep 05-Oct-2009 ; auto build size lut based on density lut */
        sync_density_lut_to_size_lut( size_sram, density_sram );
    }

    /* davep 25-Apr-2006 ; SAVE! nice linear(ish) LUT */
#if 0
    idx = 0;
    for( i=0 ; i<128/(levels-1) ; i++ ) {
        density_sram->data[idx++] = 0;
    }
    for( j=1 ; j<(levels-1) ; j++ ) {
        for( i=0 ; i<256/(levels-1) ; i++ ) {
            density_sram->data[idx++] = ((j+1)*256)/levels;
        }
    }
    while( idx<256 ) {
        density_sram->data[idx++] = 255;
    }
#endif
}
#endif

/**
 * \brief Create a gamma and contrast corrected pixel 10-bit to 8-bit LUT 
 *
 * \param[out] lut values as will be plugged into hardware LUT
 * \param gamma_times_10 gamma correction factor * 10 to avoid some floating
 *                          point
 * \param black_clip  any pixels below this value will be set to black
 * \param white_clip  any pixels above this value will be set to white
 *
 * The gamma correction curve is calculated between the black and white clip
 * points. Afterwards, a sinusoidal contrast value, also calculated between the
 * black and white clips, is subtracted from the gamma value.
 *
 * \todo TODO Note the TCNS LUT is actually three terms: Y, Cb, Cr. We're only
 * setting the Y value here because we're assuming Mono copies. When we start
 * to support corrected color copies, we'll need a different function.
 *
 * \author David Poole
 * \date 19-Apr-2006
 *
 * davep 27-Dec-2008 ; converted to a generic 10-bit to 8-bit LUT creation so we
 * can support both PIE TCNS and PIC BDR in the same function.
 */

#if 0
static void
init_mono_contrast_lut( struct uint32_array *lut, int gamma_times_10, 
                        uint16_t black_clip, uint16_t white_clip)
{
    int i, lutval;
    float gamma_float, x_black;
    float gamma_val;
//    float sine_val;
    uint32_t bw_diff;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT((gamma_times_10 > 0),gamma_times_10);
    ASSERT( lut->data != NULL );

    dbg2( "%s gamma=%d black_clip=%d white_clip=%d\n",
                __FUNCTION__,
            gamma_times_10, black_clip, white_clip); 

    XASSERT( white_clip > black_clip, white_clip );

    gamma_float = gamma_times_10 / 10.0;

    bw_diff = (white_clip - black_clip) * 4;

    /* all the *4 below assume the PIE TCNS lut size is 1024 and we're scaling
     * it into 256 values
     *
     * davep 27-Dec-2008 ; ditto for PIC BDR
     */

    for( i=0 ; i<lut->num_entries ; i++ ) {
        if( i <= black_clip*4 ) {
            lut->data[i] = 0;
        }
        else if( i >= (white_clip+1)*4 ) {
            lut->data[i] = 255;
        }
        else {
            x_black = (float)( i - black_clip*4 );

            gamma_val = powf( (x_black/bw_diff), (1.0/gamma_float) ) * 255.0;            

            /* davep 13-Sep-2007 ; only do a half phase sine wave */
//            sine_val = sin( (M_PI) * (x_black / bw_diff) ) * contrast_float * 255.0;
//            sine_val = sin( (2*M_PI) * (x_black / bw_diff) ) * contrast_float * 255.0;

//            lutval = (int)( gamma_val - sine_val );
            lutval = (int)( gamma_val);

//            dbg2( "black/diff=%d gamma_val=%d sine_val=%d lutval=%ld\n",
//                    (int)((x_black/bw_diff)*1000), (int)(gamma_val*1000),
//                    (int)(sine_val*1000), lutval );


            if( lutval < 0 ) {
                lutval = 0;
            }
            else if( lutval > 255 ) {
                lutval = 255;
            }

            lut->data[i] = lutval; 
        }
    }
}
#endif

/**
 * \brief  
 *
 * \author David Poole
 * \date 24-Jan-2012
 */

void init_mono_tcns_lut( struct uint32_array *lut, 
                         int gamma_times_10, 
                         int contrast, int brightness,
                         uint16_t black_clip, uint16_t white_clip)
{
    int i;
    float float_brightness;
    struct line line_in;
    struct line line_out;

	sv_dbg2("[SV] %s (c:%d, b:%d), g:%d, bc:%d, wc:%d)\n", __FUNCTION__, contrast, brightness, gamma_times_10, black_clip, white_clip);
	
    /* davep 31-Oct-2012 ; TODO remove floats from this function for linux
     * kernel 
     */
#ifdef __KERNEL__
    return;
#endif

    /* davep 24-Jan-2012 ; XXX tinkering ; push gamma/bc/wc into CSC rgb2esrgb
     * LUT. Do only enhancement here in TCNS
     */
    black_clip = 0;
    white_clip = 255;

    dbg2( "%s gamma=%d contrast=%d brightness=%d bc=%d wc=%d\n", __FUNCTION__, 
                gamma_times_10, contrast, brightness, 
                black_clip, white_clip );

    float_brightness = brightness/100.0;

    if( brightness < 100 ) {
        //white_clip += ( 1-float_brightness ) * 127;
        black_clip += ( 1-float_brightness ) * 127;	//mod.lsh to remove dark background
    }
    else if( brightness > 100 ) {
        white_clip -= ( float_brightness-1 ) * 127;
    }

    sv_dbg2( "[SV] %s new bright=%f wc=%d bc=%d\n", __FUNCTION__, float_brightness, white_clip, black_clip );

    grayline_new( &line_in, black_clip<<2, 0, white_clip<<2, PIE_TCNS_LUT_SIZE );
    if( contrast != 100 ) {
        grayline_enhance_contrast( contrast/100.0, &line_in, &line_out );
    }
    else {
        /* avoid any floating point foofaraw */
        memcpy( &line_out, &line_in, sizeof(struct line) );
    }

//    dbg2( "%s (%f,%f) (%f,%f) rotated (%f,%f) (%f,%f)\n", __FUNCTION__, 
//                line_in.p1.x, line_in.p1.y,
//                line_in.p2.x, line_in.p2.y,
//                line_out.p1.x, line_out.p1.y,
//                line_out.p2.x, line_out.p2.y );

    XASSERT( lut->num_entries==PIE_TCNS_LUT_SIZE, lut->num_entries );
    grayline_to_lut( lut->num_entries, lut->data, &line_out );

    /* encode the values to our hardware LUT */
    for( i=0 ; i<lut->num_entries; i++ ) {
        lut->data[i] = PIE_TCNS_LUT_Y(lut->data[i]);
    }
}

static int 
clip8bit( float num )
{
//    return MIN( 255, MAX( 0, (int)(num+0.5) ) );
    int num2;

    num2 = (int)(num+0.5);
    if( num2 < 0 ) {
        return 0;
    }
    if( num2 > 255 ) {
        return 255;
    }
    return num2;
}

static void line_equation( int x1, int y1, int x2, int y2, float *line_slope, float *line_b )
{
    *line_slope = ((float)(y2-y1))/((float)(x2-x1));
    *line_b = y1 - x1 * (*line_slope);
}

static void
init_color_tcns_lut( struct uint32_array *lut, int contrast, uint32_t brightness )
{
#if 0	//===========================================================================
    int i;
    float contrast_mult;
    float brightness_addr;
    int num; /* SIGNED integer */
    uint32_t y_val, cb_val, cr_val;

	sv_dbg2("[SV] %s\n", __FUNCTION__);
	
    /* davep 31-Oct-2012 ; TODO remove floats from this function for linux
     * kernel 
     */
#ifdef __KERNEL__
    return;
#endif

    ASSERT( lut->data!=NULL );
    XASSERT( lut->num_entries==PIE_TCNS_LUT_SIZE, lut->num_entries );

    dbg2("%s contrast=%d brightness=%d\n", __FUNCTION__, contrast, brightness ); 

    /* davep 18-Oct-2011 ; new contrast equation (color only so far) */
    contrast_mult = contrast / 100.0;

    brightness_addr = brightness / 100.0;

    // Color's gamma and black/white clip is performed in the CSC, not here.

    /* davep 30-Aug-2006 ; greatly revamped the TCNS curve. Color black/white
     * clip as well as contrast/brightness are handled in TCNS. I left Mono
     * alone.
     */

#define MAX_TCNS (PIE_TCNS_LUT_SIZE-1)
#define CLIP_TCNS(n) ((n)>MAX_TCNS?MAX_TCNS:( (n)<0?0:(n) ) )

    /* davep 24-Oct-2011 ; new brightness/contrast equations. Black/white
     * clip are now done in CSC RGB2esRGB
     */
    for( i=0 ; i<PIE_TCNS_LUT_SIZE ; i++ ) {
        num = (int)(i + (brightness_addr-1) * PIE_TCNS_LUT_SIZE);
        y_val = CLIP_TCNS( num );
        y_val /= 4;

        num = (int)(((i-512) * contrast_mult)+512);
        cb_val = CLIP_TCNS( num );
        cb_val /= 4;
        cr_val = cb_val;

        lut->data[i] = PIE_TCNS_TUPLE( y_val, cb_val, cr_val );
    }
#else	//===========================================================================
    int i;
    float float_brightness;
    struct line line_in;
    struct line line_out;
	uint16_t black_clip;
	uint16_t white_clip;
	uint32_t y_val, cb_val, cr_val;
	
	sv_dbg2("[SV] %s (c:%d, b:%d), g:%d, bc:%d, wc:%d)\n", __FUNCTION__, contrast, brightness, black_clip, white_clip);
	
    /* davep 31-Oct-2012 ; TODO remove floats from this function for linux
     * kernel 
     */
#ifdef __KERNEL__
    return;
#endif

    /* davep 24-Jan-2012 ; XXX tinkering ; push gamma/bc/wc into CSC rgb2esrgb
     * LUT. Do only enhancement here in TCNS
     */
    black_clip = 0;
    white_clip = 255;

    dbg2( "%s contrast=%d brightness=%d\n", __FUNCTION__, 
                contrast, brightness );

    float_brightness = brightness/100.0;

    if( brightness < 100 ) {
        //white_clip += ( 1-float_brightness ) * 127;
        black_clip += ( 1-float_brightness ) * 127;
    }
    else if( brightness > 100 ) {
        white_clip -= ( float_brightness-1 ) * 127;
    }

    sv_dbg2( "[SV] %s new bright=%f wc=%d bc=%d\n", __FUNCTION__, float_brightness, white_clip, black_clip );

    grayline_new( &line_in, black_clip<<2, 0, white_clip<<2, PIE_TCNS_LUT_SIZE );
    if( contrast != 100 ) {
        grayline_enhance_contrast( contrast/100.0, &line_in, &line_out );
    }
    else {
        /* avoid any floating point foofaraw */
        memcpy( &line_out, &line_in, sizeof(struct line) );
    }

//    dbg2( "%s (%f,%f) (%f,%f) rotated (%f,%f) (%f,%f)\n", __FUNCTION__, 
//                line_in.p1.x, line_in.p1.y,
//                line_in.p2.x, line_in.p2.y,
//                line_out.p1.x, line_out.p1.y,
//                line_out.p2.x, line_out.p2.y );

    XASSERT( lut->num_entries==PIE_TCNS_LUT_SIZE, lut->num_entries );
    grayline_to_lut( lut->num_entries, lut->data, &line_out );

    /* encode the values to our hardware LUT */
    for( i=0 ; i<lut->num_entries; i++ ) {
    	y_val = lut->data[i];
		cb_val = i/4;
		cr_val = cb_val;
        
        lut->data[i] = PIE_TCNS_TUPLE( y_val, cb_val, cr_val );
    }
    
#endif	//===========================================================================
}

static void init_scan_pic_bdr_lut( struct uint32_array *lut, 
                                  int gamma_times_10, 
                                  uint16_t black_clip, uint16_t white_clip )
{
    int i;
    float gamma_f;
    float i_float, lutsize_f, lutval_f;
    uint32_t val_gamma_corrected;
    int x1,y1,x2,y2;
    float line_slope, line_b;
    sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%s gamma=%d bc=%d wc=%d\n", __FUNCTION__, 
                gamma_times_10, black_clip, white_clip );

    /* Note. As of this writing (29-Sep-2009), contrast is ignored (for now?) */

    gamma_f = gamma_times_10/10.0f;
    lutsize_f = (float)PIC_BDR_LUT_SIZE;

    /* TODO add brightness (no clue how, tho) */

    /* The *4 is to convert from 8-bit to 10-bit values */
    x1 = black_clip*4;
    y1 = 0;
    x2 = white_clip*4;
    y2 = PIC_BDR_LUT_SIZE;

    line_equation( x1, y1, x2, y2, &line_slope, &line_b );

    for( i=0 ; i<lut->num_entries ; i++ ) {

        /* gamma function */
        i_float = (float)i;
        val_gamma_corrected = (uint32_t)(lutsize_f * powf( (i_float/lutsize_f), (1.0/gamma_f) ));

        /* y = m(gamma(x))+b */
        lutval_f = line_slope * val_gamma_corrected + line_b;

        /* 10-to-8 LUT */
        lutval_f = lutval_f / 4.0;

        lut->data[i] = clip8bit( lutval_f );
    }

}

/**
 * \brief Set up the PIE MultiFilter Activity Index LUT.
 *
 * \author David Poole
 * \date 04-Sep-2006
 *
 * \date 21-Nov-2011 ; new implementation 
 */

static void init_filter_ai_lut( uint32_t *lut, uint32_t sharpness )
{
    int i;
//    int y, cbcr;
    int num;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%s sharp=%d\n", __FUNCTION__, sharpness);

//    y = 0;
//    cbcr = 0;

    /* davep 21-Nov-2011 ; want to make 0->max smooth, 100 to no change, 200 to
     * nice sharpen.  [0,100,200] -> [-32,0,32]
     *
     * Using the [0,200] scale (100 no change) so smooth/sharpen uses same
     * scale as brightness/contrast.
     */
    num = (sharpness * 32) / 100 - 32;
    if( num>127) {
        /* crazy high sharpen (introduces artifacts) */
        num = 127;
    }
    else if( num<-32 ) {
        /* min smooth before image corruption */
        num = -32;
    }
    
    /* for now, set both Y and CbCr planes to same value (might change later?) */
    for( i=0 ; i<PIE_FILTER_LUT_SIZE ; i++ ) {
        lut[i] = PIE_FILTER_LUT_Y(num) | PIE_FILTER_LUT_C(num);
    }

    ASSERT( lut != NULL );
}

/**
 * \brief  Set up a scanvar's MultiFilter Activity Index.
 *
 * If the AI LUT doesn't exist, create it. Once created, initialize it based on
 * the scanvar's settings.
 *
 * Make sure to handle siqfile overrides as well.
 *
 *
 * \author David Poole
 * \date 04-Sep-2006
 *
 */

static scan_err_t scanvar_setup_filter_ai( struct scanvars *sv )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* if we don't already of an AI LUT allocated, do it now */
    scerr = opalloc_mf_ai_lut( sv );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    if( sv->use_mf_ai_lut ) {
        ASSERT( sv->mf_ai_lut.data != NULL );
        XASSERT( sv->mf_ai_lut.num_entries==PIE_FILTER_LUT_SIZE, sv->mf_ai_lut.num_entries );
        init_filter_ai_lut( sv->mf_ai_lut.data, sv->sharpness );
    }

    return SCANERR_NONE;
}

static void
init_csc_rgb_lut( struct uint32_array *lut, int gamma_times_10,
                  uint16_t black_clip, uint16_t white_clip, int rgb_tint )
{
    int i;
    float gamma_f;
    uint8_t value;

	sv_dbg2("[SV] %s (id=%d g=%d bc=%d wc=%d t=%d)\n", __FUNCTION__, 
                g_id, gamma_times_10, black_clip, white_clip, rgb_tint );
    dbg2( "%s gamma=%d bc=%d wc=%d rgb_tint=%d\n", __FUNCTION__, 
                gamma_times_10, black_clip, white_clip, rgb_tint );

    XASSERT( lut->num_entries==PIE_CSC_RGB_LUT_SIZE, lut->num_entries );
    ASSERT( lut->data != NULL );

    /* davep 02-Aug-2010 ; use same formula as PIC BDR */
    gamma_f = gamma_times_10/10.0f;

    for( i=0 ; i<lut->num_entries ; i++ ) {

        /*davep 08-Mar-2010 ; equation from DougK. gamma and black/white clip
         * in one shot
         */
        value = MIN(255,
                    MAX(0,
                         (i - black_clip) * (255./(white_clip-black_clip))
                       )
                   );

        lut->data[i] = (1023.0 * (powf((value/255.0),(1.0/gamma_f))));

        /* davep 18-May-2012 ; feature request: rgb tint (add/subtract fixed
         * value from r/g/b curve). rgb_tint in [-255,255] (8-bit signed). Cast
         * the existing lut value to 'int' so negatives will be properly
         * clipped to zero.
         */
        lut->data[i] = MIN(1023,MAX(0, (int)lut->data[i]+rgb_tint ));
        
#if	SDH_COPY_TUNING
		if( (g_id > SVCID_COPY && g_id < SVCID_SCAN) || g_id == SVCID_TEXT_FAX || g_id == SVCID_MIXED_FAX )
		{
			uint32_t	org;
			org = lut->data[i];
			
			/* SDH White Offset */
			lut->data[i] = MIN(1023,MAX(0, lut->data[i]-sdh_white_offset*i/(lut->num_entries) ));
			
			/* SDH Black Offset */
			lut->data[i] = MIN(1023,MAX(0, lut->data[i]+sdh_black_offset*(lut->num_entries-i)/lut->num_entries ));
			
			/* SDH White Guard (Background Remove Effect) */
	        if(i>lut->num_entries-sdh_white_guard)
	        	lut->data[i] = 1023;
	        	
	        /* SDH Black Guard (Pure Black Effect) */
	        if(i<sdh_black_guard)
	        	lut->data[i] = 0;
/*
			sv_dbg2("[SV] b_o=%d org=%d new=%d)\n",	sdh_black_offset*(lut->num_entries-i)/lut->num_entries,
															org,
															lut->data[i]
			 );
*/
		}
#endif
    }
}
#if BG_REM_ADVANCED //==========================================================
void
scanvar_set_csc_rgb_lut( uint32_t *src_lut, struct uint32_array *dst_lut, 
						uint8_t BG_x )	// add.lsh.for background removal
{
    int i;
	uint32_t WP_x = PIE_CSC_RGB_LUT_SIZE-1;		// x-axis coordinate filled with white in CSC LUT (White Padding)
	uint32_t WP_y = 1023;						// y-axis coordinate filled with white in CSC LUT (1023 : Max Value)
	uint32_t BG_y_org = 1023;					// y-axis coordinate of background in CSC LUT (Org)
	uint32_t WP_y_tar = 1023;					// y-axis coordinate of background in CSC LUT (Target)
		
	sv_dbg2("%s (level=%d)\n", __FUNCTION__, BG_x );

    XASSERT( dst_lut->num_entries==PIE_CSC_RGB_LUT_SIZE, dst_lut->num_entries );
    ASSERT( dst_lut->data != NULL );

	// Searching white starting position
	for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
		if(src_lut[i] == 1023){
			WP_x = i-1;
			WP_y = src_lut[WP_x];
			break;
		}
	}
	// Searching LUT y value matched with background level
	BG_y_org = src_lut[BG_x];
				
	/* Background in White Padding */
	if(BG_x >= WP_x)	
	{
		// Use org LUT
		if(BG_x > BG_REM_LIMIT_W){
			memcpy( dst_lut->data, src_lut, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE) );			
			dbg1("[BG] Set Original LUT (BG_x:%d, WP_x:%d, WP_y:%d, BG_y_org:%d, WP_y_tar:%d), LIMIT_W:%d\n", BG_x, WP_x, WP_y, BG_y_org, WP_y_tar, BG_REM_LIMIT_W);
		}
		// Apply restrictively (base on distance between BG_x and BG_REM_LIMIT_W)
		else{				
			// Deside target y level
			WP_y_tar = WP_y + (1023 - WP_y)*(BG_REM_LIMIT_W - BG_x)/(BG_REM_LIMIT_W - WP_x);
			
			for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
		    	
		    	if(i>WP_x)
		    		;	// do nothing
				else
					dst_lut->data[i] = MIN(WP_y_tar, src_lut[i] + (WP_y_tar-WP_y)*i/(WP_x));
		    }
		    dbg1("[BG] Set Restrict LUT (BG_x:%d, WP_x:%d, WP_y:%d, BG_y_org:%d, WP_y_tar:%d), LIMIT_W:%d\n", BG_x, WP_x, WP_y, BG_y_org, WP_y_tar, BG_REM_LIMIT_W);
		}	
	}
	/* Background out of White Padding */
	else
	{
		for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
	    	
	    	if(i>BG_x)
	    		dst_lut->data[i] = 1023;
			else
				dst_lut->data[i] = MIN(1023, src_lut[i] + (1023-BG_y_org)*i/(BG_x));
	    }
	    dbg1("[BG] Set New LUT (BG_x:%d, WP_x : %d, WP_y:%d, BG_y_org:%d, WP_y_tar:%d), LIMIT_W:%d\n", BG_x, WP_x, WP_y, BG_y_org, WP_y_tar, BG_REM_LIMIT_W);
	}
}
#else //==========================================================
void
scanvar_set_csc_rgb_lut( uint32_t *src_lut, struct uint32_array *dst_lut, 
						uint8_t background_level )	// add.lsh.for background removal
{
    int i;
	uint32_t src_target;
	
	sv_dbg2("%s (level=%d)\n", __FUNCTION__, background_level );

    XASSERT( dst_lut->num_entries==PIE_CSC_RGB_LUT_SIZE, dst_lut->num_entries );
    ASSERT( dst_lut->data != NULL );

	src_target = src_data[background_level];
	
	/* Original LUT */
	if(src_target == 1023)
	{
		memcpy( dst_lut->data, src_data, sizeof(uint32_t)*(PIE_CSC_RGB_LUT_SIZE) );
		dbg1("[BG] Set Original LUT (target : %d)\n", src_target);
	}
	/* Resampling LUT */
	else
	{
		for( i=0 ; i<PIE_CSC_RGB_LUT_SIZE ; i++ ) {
	    	
	    	if(i>background_level)
	    		dst_lut->data[i] = 1023;
			else
				dst_lut->data[i] = MIN(1023, src_data[i] + (1023-src_target)*i/(background_level));
	    }
	    dbg1("[BG] Set New LUT (target : %d)\n", src_target);
	}
}
#endif //==========================================================
/**
 * \brief initialize any/all the CSC RGB->esRGB LUTs 
 *
 * \author David Poole
 * \date 02-Aug-2010
 *
 */

static void init_all_csc_rgb_luts( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if (sv->use_csc_rgb_lut) {
        init_csc_rgb_lut( &sv->csc_rgb_lut, sv->gamma_times_10,
                              sv->black_clip_point[0],
                              sv->white_clip_point[0], 0 );
    }
    if( sv->use_csc_r_lut ) {
        init_csc_rgb_lut( &sv->csc_r_lut, sv->gamma_times_10,
                sv->black_clip_point[0], sv->white_clip_point[0], 
                sv->rgb_tint[SV_COLOR_RED] );
    }
    if( sv->use_csc_g_lut ) {
        init_csc_rgb_lut( &sv->csc_g_lut, sv->gamma_times_10,
                sv->black_clip_point[SV_COLOR_GREEN], sv->white_clip_point[SV_COLOR_GREEN],
                sv->rgb_tint[SV_COLOR_GREEN] );
    }
    if( sv->use_csc_b_lut ) {
        init_csc_rgb_lut( &sv->csc_b_lut, sv->gamma_times_10,
                sv->black_clip_point[SV_COLOR_BLUE], sv->white_clip_point[SV_COLOR_BLUE],
                sv->rgb_tint[SV_COLOR_BLUE] );
    }
}

static void
init_csc_rgb_matrix(uint32_t *matrix)
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 28-Aug-2006 ; linear RGB -> sRGB */
    matrix[0]=16384; matrix[1]=0;     matrix[2]=0;
    matrix[3]=0;     matrix[4]=16384; matrix[5]=0;
    matrix[6]=0;     matrix[7]=0;     matrix[8]=16384;
}

static void
init_csc_rgbycc_matrix(uint32_t *matrix)
{
    int i;
	
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    for( i=0 ; i<9 ; i++ ) {
        matrix[i] = esrgb_to_ycc[i];
    }
}

static void
init_csc_yccrgb_matrix(uint32_t *matrix)
{
    int i;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    for( i=0 ; i<9 ; i++ ) {
#ifdef HAVE_PIE_POGO_BGR
        /* davep 09-Jan-2013 ; swap R/B planes on ASICS that don't have the
         * proper PIE DEPOGO capability (PIE RGB mode output is actually BGR
         * (endian-ness issue))
         */
        matrix[i] = ycc_to_bgr[i];
#else
        /* output of PIE is truly RGB packed */
        matrix[i] = ycc_to_esrgb[i];
#endif
    }
}

/**
 * \brief  Initialize the MultiFilter Activity Index LUT
 *
 * \author David Poole
 * \date 05-Jun-2007
 *
 */

static scan_err_t 
init_mf_ai_lut( struct scanvars *sv, const uint8_t *ai_lut_y, const uint8_t *ai_lut_cbcr )
{
    scan_err_t scerr;
    int i;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( !sv->use_mf_ai_lut ) {
        return SCANERR_INVALID_PARAM;
    }

    scerr = opalloc_mf_ai_lut( sv );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    if( ai_lut_y && ai_lut_cbcr ) {
        for( i=0 ; i<PIE_FILTER_LUT_SIZE ; i++ ) {
            /* put the Y and CbCr values together into the LUT */
            sv->mf_ai_lut.data[i] = (ai_lut_y[i] << PIE_FILTER_LUT_Y_SHIFT) |
                                    (ai_lut_cbcr[i] << PIE_FILTER_LUT_CBCR_SHIFT);
        }
    }
    else if( ai_lut_y ) {
        for( i=0 ; i<PIE_FILTER_LUT_SIZE ; i++ ) {
            /* put just the Y value into the LUT (mono only uses the Y
             * channel)
             * XXX could someday do a merge instead of an overwrite? 
             */
            sv->mf_ai_lut.data[i] = (ai_lut_y[i] << PIE_FILTER_LUT_Y_SHIFT); 
        }
    }
    else { 
        /* davep 01-Apr-2013 ; need Y&CbCr (color) OR Y (mono); anything else
         * is an error.
         */
        return SCANERR_INVALID_PARAM;
    }

    return SCANERR_NONE;
}

/**
 * \brief Set the calibration white/black targets for monochrome (laser)
 * copies. 
 *
 * As of this writing (26-Oct-2006), all five copy modes are identical. We
 * could change in the calling class method, though.
 *
 * \author David Poole
 * \date 26-Oct-2006
 *
 */

static void
scanvar_mono_copy_cal_white_black_targets( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_cal_white_digital_target = true;
//    sv->cal_white_digital_target[SV_COLOR_MONO] = 65535;
    sv->cal_white_digital_target[SV_COLOR_MONO] = 61153;

    sv->use_cal_black_digital_target = true;
    sv->cal_black_digital_target[SV_COLOR_MONO] = 1028;

    sv->use_cal_white_analog_target = true;
    sv->cal_white_analog_target[SV_COLOR_MONO] = 52429;
    sv->cal_white_analog_target[SV_COLOR_GREEN] = 52429;
    sv->cal_white_analog_target[SV_COLOR_BLUE] = 52429;

    sv->use_cal_black_analog_target = true;
    sv->cal_black_analog_target[SV_COLOR_MONO] = 2560;
    sv->cal_black_analog_target[SV_COLOR_GREEN] = 2560;
    sv->cal_black_analog_target[SV_COLOR_BLUE] = 2560;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 24-Jan-2012
 *
 * An ASIC change introduced three separate CSC RGB2esRGB LUTS, one for each of
 * R,G,B. Previous platforms used one for all three channels.
 */

static void csc_rgb_choose_luts( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
#ifdef HAVE_THREE_RGB2ESRGB_LUT 
    sv->use_csc_r_lut = true;
    sv->use_csc_g_lut = true;
    sv->use_csc_b_lut = true;

    sv->use_csc_rgb_lut = false;
#else
    sv->use_csc_rgb_lut = true;

    sv->use_csc_r_lut = false;
    sv->use_csc_g_lut = false;
    sv->use_csc_b_lut = false;
#endif
}

/**
 * \brief Mono Copy "base class" constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 * davep 08-Nov-2011 ; removed all HalfPack
 */

struct scanvars * scanvar_mono_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);

    /* call parent "constructor" */
    sv = scanvar_mono_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    /* turn on the blocks we'll use */
    sv->use_mf = true;
    sv->use_mf_ai_lut = true;
    sv->use_tcns = true;

    opalloc_tcns_lut( sv );
    opalloc_mf_ai_lut( sv );

    /* default to middle of the copy settings block */
    sv->copy_brightness = 6;

    /* TE only useful with color */
    sv->use_mf_te = false;

    /* davep 01-Oct-2006 ; break up color adjustments into class methods */
    sv->set_gamma_curve = mono_copy_set_gamma_curve;
    sv->set_contrast_curve = mono_copy_set_enhance_curve;
    sv->set_brightness_curve = mono_copy_set_enhance_curve;
	
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
	
    return sv;
}

/**
 * \brief  Initialize the mono copy LUTs based on current scanvar IQ settings
 *
 * Gathered together a collection of copy/paste code in the various copy
 * scanvar constructors.
 *
 * \author David Poole
 * \date 09-Nov-2011
 *
 */

static scan_err_t scanvar_mono_copy_init_iq( struct scanvars *sv ) 
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%s %d\n", __FUNCTION__, sv->id );

    scanvar_mono_copy_cal_white_black_targets( sv );

    scerr = copy_load_lightdark_settings( sv );
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

    if( sv->use_mf_ai_lut ) {
        init_filter_ai_lut( sv->mf_ai_lut.data, sv->sharpness );
    }

    /* davep 29-Dec-2005 ; integrating SIQ */
    scanvar_siq_merge( sv );

    // Must do this after the siq_merge so we pick up any changes from that.
    mono_copy_set_gamma_curve( sv );
    mono_copy_set_enhance_curve( sv );

    // Merge again to pick up LUT changes. (LUT values are calculated by the init_*
    // functions above).
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

/**
 * \brief Mono Draft Copy scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_draft_copy_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_mono_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_DRAFT_COPY;

    /* 
     *  change the copy tuning params here 
     *
     */

    /* call "parent method" to initialize the copy IQ settings */
    scerr = scanvar_mono_copy_init_iq( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Mono Text Copy scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_text_copy_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
    sv = scanvar_mono_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_TEXT_COPY;

    /* 
     *  change the copy tuning params here 
     *
     */

    /* call "parent method" to initialize the copy IQ settings */
    scerr = scanvar_mono_copy_init_iq( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }

#if 1
	/* settings tuning */
    init_mf_ai_lut( sv, mono_copy_text_ai_lut_y, NULL );

    /* additional multifilter settings */
    sv->mf_y_radius    = PIE_FILTER_SCR1_Y_1;//PIE_FILTER_SCR1_Y_1, PIE_FILTER_SCR1_Y_1_5, PIE_FILTER_SCR1_Y_2
    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;//PIE_FILTER_SCR1_R_1, PIE_FILTER_SCR1_R_1_5, PIE_FILTER_SCR1_R_2
    sv->mf_scale_10    = PIE_FILTER_SCR1_S_1;//PIE_FILTER_SCR1_S_1, PIE_FILTER_SCR1_S_0_5, PIE_FILTER_SCR1_S_0_25;
    sv->mf_scale_15    = PIE_FILTER_SCR1_M_1;//PIE_FILTER_SCR1_M_1, PIE_FILTER_SCR1_M_0_5, PIE_FILTER_SCR1_M_0_25, PIE_FILTER_SCR1_M_0;
    sv->mf_scale_20    = PIE_FILTER_SCR1_P_0_5;//PIE_FILTER_SCR1_P_0_5, PIE_FILTER_SCR1_P_0_25, PIE_FILTER_SCR1_P_0_125, PIE_FILTER_SCR1_P_0;
    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_16;//PIE_FILTER_SCR1_N_16, PIE_FILTER_SCR1_N_32, PIE_FILTER_SCR1_N_64, PIE_FILTER_SCR1_N_128;
    sv->mf_offset      = 0;

    sv->use_mf_erosion      = false;
    sv->use_mf_dilation = false;
    sv->use_mf_varcoefficients = true;

    memcpy(sv->mf_varcoef_y, mono_copy_text_var_coefs_y, MF_COEF_MAX_COEFFICIENTS*sizeof(uint32_t));
    memcpy(sv->mf_varcoef_chroma, mono_copy_text_var_coefs_chroma, MF_COEF_MAX_COEFFICIENTS*sizeof(uint32_t));
    sv->mf_varcoef_y_shift      = 8;
    sv->mf_varcoef_y_sf         = 2048;
    sv->mf_varcoef_chroma_shift = 8;
    sv->mf_varcoef_chroma_sf    = 2048;
#endif
    // Merge again to pick up LUT changes. (LUT values are calculated by the
    // init_* functions above).
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Mono Picture Copy scanvar constructor 
 *
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_picture_copy_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_mono_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_PICTURE_COPY;

    /* 
     *  change the copy tuning params here 
     *
     */
    /* I want the biggest smooth I can have so switch up to the biggest radius */
    sv->mf_y_radius = PIE_FILTER_SCR1_Y_2;

    /* call "parent method" to initialize the copy IQ settings */
    scerr = scanvar_mono_copy_init_iq( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }

    /* "Picture" specific settings tuning */
    init_mf_ai_lut( sv, mono_copy_picture_ai_lut_y, NULL );

    // Merge again to pick up LUT changes. (LUT values are calculated by the
    // init_* functions above).
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Mono Film/Photo Copy scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_filmphoto_copy_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
    sv = scanvar_mono_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_FILMPHOTO_COPY;

    /* 
     *  change the copy tuning params here 
     *
     */

    /* call "parent method" to initialize the copy IQ settings */
    scerr = scanvar_mono_copy_init_iq( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }

    /* settings tuning */
    init_mf_ai_lut( sv, mono_copy_film_ai_lut_y, NULL );

    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
	
    return sv;
}

/**
 * \brief Mono Mixed Copy scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_mixed_copy_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
    sv = scanvar_mono_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_MIXED_COPY;

    /* 
     *  change the copy tuning params here 
     *
     */

    /* call "parent method" to initialize the copy IQ settings */
    scerr = scanvar_mono_copy_init_iq( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }
#if 1
    /* settings tuning */
    init_mf_ai_lut( sv, mono_copy_mixed_ai_lut_y, NULL );

    /* additional multifilter settings */
#if SDH_PINETREE_YANGSAN_IC
    sv->mf_y_radius    = PIE_FILTER_SCR1_Y_1;//PIE_FILTER_SCR1_Y_1, PIE_FILTER_SCR1_Y_1_5, PIE_FILTER_SCR1_Y_2
    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;//PIE_FILTER_SCR1_R_1, PIE_FILTER_SCR1_R_1_5, PIE_FILTER_SCR1_R_2
    sv->mf_scale_10    = PIE_FILTER_SCR1_S_1;//PIE_FILTER_SCR1_S_1, PIE_FILTER_SCR1_S_0_5, PIE_FILTER_SCR1_S_0_25;
    sv->mf_scale_15    = PIE_FILTER_SCR1_M_0_5;//PIE_FILTER_SCR1_M_1, PIE_FILTER_SCR1_M_0_5, PIE_FILTER_SCR1_M_0_25, PIE_FILTER_SCR1_M_0;
    sv->mf_scale_20    = PIE_FILTER_SCR1_P_0_25;//PIE_FILTER_SCR1_P_0_5, PIE_FILTER_SCR1_P_0_25, PIE_FILTER_SCR1_P_0_125, PIE_FILTER_SCR1_P_0;
    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_32;//PIE_FILTER_SCR1_N_16, PIE_FILTER_SCR1_N_32, PIE_FILTER_SCR1_N_64, PIE_FILTER_SCR1_N_128;
    sv->mf_offset      = 0;
#else
    sv->mf_y_radius    = PIE_FILTER_SCR1_Y_1;//PIE_FILTER_SCR1_Y_1, PIE_FILTER_SCR1_Y_1_5, PIE_FILTER_SCR1_Y_2
    sv->mf_cbcr_radius = PIE_FILTER_SCR1_R_1;//PIE_FILTER_SCR1_R_1, PIE_FILTER_SCR1_R_1_5, PIE_FILTER_SCR1_R_2
    sv->mf_scale_10    = PIE_FILTER_SCR1_S_1;//PIE_FILTER_SCR1_S_1, PIE_FILTER_SCR1_S_0_5, PIE_FILTER_SCR1_S_0_25;
    sv->mf_scale_15    = PIE_FILTER_SCR1_M_1;//PIE_FILTER_SCR1_M_1, PIE_FILTER_SCR1_M_0_5, PIE_FILTER_SCR1_M_0_25, PIE_FILTER_SCR1_M_0;
    sv->mf_scale_20    = PIE_FILTER_SCR1_P_0_5;//PIE_FILTER_SCR1_P_0_5, PIE_FILTER_SCR1_P_0_25, PIE_FILTER_SCR1_P_0_125, PIE_FILTER_SCR1_P_0;
    sv->mf_lut_output_scale = PIE_FILTER_SCR1_N_16;//PIE_FILTER_SCR1_N_16, PIE_FILTER_SCR1_N_32, PIE_FILTER_SCR1_N_64, PIE_FILTER_SCR1_N_128;
    sv->mf_offset      = 0;
#endif
    sv->use_mf_erosion      = false;
    sv->use_mf_dilation = false;
    sv->use_mf_varcoefficients = true;

    memcpy(sv->mf_varcoef_y, mono_copy_mixed_var_coefs_y, MF_COEF_MAX_COEFFICIENTS*sizeof(uint32_t));
    memcpy(sv->mf_varcoef_chroma, mono_copy_mixed_var_coefs_chroma, MF_COEF_MAX_COEFFICIENTS*sizeof(uint32_t));
    sv->mf_varcoef_y_shift      = 8;
    sv->mf_varcoef_y_sf         = 2048;
    sv->mf_varcoef_chroma_shift = 8;
    sv->mf_varcoef_chroma_sf    = 2048;
#endif
    // Merge again to pick up LUT changes. (LUT values are calculated by the
    // init_* functions above).
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Color Copy scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

static struct scanvars * scanvar_color_copy_new( void )
{
    struct scanvars *sv;

	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
    sv = scanvar_color_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_COLOR_COPY;

    sv->copy_brightness = 6;

    /* davep 01-Oct-2006 ; break up color adjustments into class methods;
     * note I'm overriding the parent (color scan) methods 
     */
    sv->set_gamma_curve = color_copy_set_gamma_curve;
    sv->set_contrast_curve = color_copy_set_enhance_curve;
    sv->set_brightness_curve = color_copy_set_enhance_curve;

    /* PIE */
    sv->pieout = PIE_PIXEL_XRGB;
    
    /* davep 14-Nov-2005 ; ink needs row boundries at 96 pixels (we have 32bpp
     * in xRGB mode)
     */
//    sv->pie_row_align_bytes = 96 * 4;

    sv->sharpness = 100;
    scanvar_setup_filter_ai( sv );

    sv->white_clip_point[0] = 230;
    sv->white_clip_point[1] = 230;
    sv->white_clip_point[2] = 230;
    sv->black_clip_point[0] = 10;
    sv->black_clip_point[1] = 10;
    sv->black_clip_point[2] = 10;

    /* davep 20-Sep-2006 ; change to a nicer gamma that leaves the colors
     * richer (on white paper--mixed mode, remember?) 
     */
    sv->gamma_times_10 = 22;
    init_all_csc_rgb_luts( sv );

    /* now that white/black clip and contrast have changed, have to redo TCNS
     * curve. Not using black/white clip in tcns for color so pass zero.
     */
    if( sv->use_tcns ) {
        init_color_tcns_lut(&sv->tcns_lut, sv->contrast, sv->brightness);
    }

    /* preserve siqfile overrides */
    scanvar_siq_merge( sv );
    
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

struct scanvars *scanvar_draft_color_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_color_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_DRAFT_COLOR_COPY;
    sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

struct scanvars *scanvar_text_color_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_color_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_TEXT_COLOR_COPY;
    sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

struct scanvars *scanvar_mixed_color_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_color_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_MIXED_COLOR_COPY;
    sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

struct scanvars *scanvar_filmphoto_color_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_color_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_FILMPHOTO_COLOR_COPY;
    sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

struct scanvars *scanvar_picture_color_copy_new( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_color_copy_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_PICTURE_COLOR_COPY;
    sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}


/*************************************
 * Fax Job "Classes"                 *
 *************************************/

/**
 * \brief  Base class for fax
 *
 * \author David Poole
 * \date 09-Nov-2011
 *
 */

struct scanvars * scanvar_fax_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    /* call parent "constructor" */
    sv = scanvar_mono_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    /* pick some nice halfpack defaults */
    sv->use_hp = true;
    sv->hp_bpp = 2;

    /* davep 29-Dec-2005 ; set up the default HalfPack LUTs (can be overridden
     * by a descendent)
     */
#ifdef PIE_HP_SZDD_OFFSET 
    /* davep 23-Nov-2007 ; hack to use the SZDD LUT instead of the individual
     * LUTs (have to use all three simultaneously) 
     * XXX get rid of the #ifdef!
     */
    scerr = uint32_array_new( &sv->hp_size_lut, PIE_HP_SIZE_LUT_SIZE );
    if( scerr == 0 ) {
        scerr = uint32_array_new( &sv->hp_density_lut, PIE_HP_DENSITY_LUT_SIZE );
        if( scerr != 0 ) {
            scanvar_delete( &sv );
            return NULL;
        }

        scerr = uint32_array_new( &sv->hp_diffus_lut, PIE_HP_DIFFUS_LUT_SIZE );
        if( scerr != 0 ) {
            scanvar_delete( &sv );
            return NULL;
        }

        init_hp_2bpp_luts( &sv->hp_size_lut, &sv->hp_density_lut );
        sv->use_hp_size_lut = true;
        sv->use_hp_density_lut = true;
        sv->use_hp_diffus_lut = true;
    }
#else
    scerr = uint32_array_new( &sv->hp_size_lut, PIE_HP_SIZE_LUT_SIZE );
    if( scerr == 0 ) {
        scerr = uint32_array_new( &sv->hp_density_lut, PIE_HP_DENSITY_LUT_SIZE );
        if( scerr==0 ) {
            init_hp_2bpp_luts( &sv->hp_size_lut, &sv->hp_density_lut );
            sv->use_hp_size_lut = true;
            sv->use_hp_density_lut = true;
        }
    }
#endif

    /* davep 07-Sep-2012 ; we are now using CSC for even the mono LUTs */
    sv->use_tcns = false;

    /* davep 01-Jan-2006 ; moving hardwired code from hp_start()-scanpipe.c
     * into scanvars. 
     */
    sv->hp_alg_invert_data = false;

    // Enable error diffusion
    sv->hp_alg_accum_err = true;

    // Simple error diffusion for now, so force DoADot on
    // rather than allow halfpack to calculate whether it
    // should be on for a particular pixel.
    sv->hp_alg_doadot = true;

    // Floyd-Steinberg error weights
    sv->hp_alg_errweight1 = 1;
    sv->hp_alg_errweight2 = 5;
    sv->hp_alg_errweight3 = 3;

    /* davep 01-Oct-2006 ; break up color adjustments into class methods */
    sv->set_gamma_curve = mono_copy_set_gamma_curve;
    sv->set_contrast_curve = mono_copy_set_enhance_curve;
    sv->set_brightness_curve = mono_copy_set_enhance_curve;

	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Constructor for a text mode fax scanvars 
 *
 * TODO needs IQ work
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

struct scanvars * scanvar_text_fax_new( void )
{
#if SDH_FAX_TUNING
	struct scanvars     *sv = NULL;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);

	sv = scanvar_text_copy_new();
	
	init_mf_ai_lut( sv, mono_fax_ai_lut_y, NULL );
	g_id = sv->id = SVCID_TEXT_FAX;
	
	return sv;
#else 
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_fax_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_TEXT_FAX;

    sv->hp_bpp = 1;

    /* reset to our 1bpp luts */
    if( sv->use_hp_size_lut && sv->use_hp_density_lut ) {
        init_hp_1bpp_luts( &sv->hp_size_lut, &sv->hp_density_lut );
    }

    /* davep 02-Apr-2009 ; must flip for fax (works in cooperation with code in
     * the mech driver)
     */
    sv->sensor_flip_horizontal = true;

    /* merge again to pick up any changes (scanvar_text_copy_new() does a merge
     * which gets ignored because the scanvar id doesn't match ) 
     */
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
#endif
}

/**
 * \brief Constructor for a mixed mode fax scanvars 
 *
 * TODO needs IQ work
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

struct scanvars * scanvar_mixed_fax_new( void )
{
#if SDH_FAX_TUNING
	struct scanvars     *sv = NULL;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
	sv = scanvar_mixed_copy_new();
	
	init_mf_ai_lut( sv, mono_fax_ai_lut_y, NULL );
	g_id = sv->id = SVCID_MIXED_FAX;
	
	return sv;
#else
    struct scanvars *sv;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_fax_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_MIXED_FAX;

    sv->hp_bpp = 1;

    /* reset to our 1bpp luts */
    if( sv->use_hp_size_lut && sv->use_hp_density_lut ) {
        init_hp_1bpp_luts( &sv->hp_size_lut, &sv->hp_density_lut );
    }

    /* davep 01-Oct-2007 ; see the notes aroud
     * mono_fax_mixed_set_brightness_curve() to understand why I'm overriding
     * the parent's set_brightness_curve() method 
     */
    sv->set_brightness_curve = mono_fax_mixed_set_brightness_curve;

    /* davep 02-Apr-2009 ; must flip for fax (works in cooperation with code in
     * the mech driver)
     */
    sv->sensor_flip_horizontal = true;

    /* merge again to pick up any changes (scanvar_mixed_copy_new() does a merge
     * which gets ignored because the scanvar id doesn't match ) 
     */
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
#endif
}

/*************************************
 * Scan Job "Classes"                *
 *************************************/

/**
 * \brief Scan-to-Host scanvar "virtual base class" constructor 
 *
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

static struct scanvars * scanvar_scan_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;

	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
	
    sv = scanvar_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_SCAN;

    /* PIE defaults */
    sv->use_pie = true;

    if( PIE_WDMA_NUM_CHANNELS==1 ) {
        sv->pieout = PIE_PIXEL_RGB;
    } 
    else {
        sv->pieout = PIE_PIXEL_3_PLANES;
    }

    sv->use_tcns = true;

    if (sv->use_tcns) {
        scerr = uint32_array_new( &sv->tcns_lut, PIE_TCNS_LUT_SIZE );
        if (scerr == 0) {
            sv->use_tcns_lut = true;
        }
        else {
            sv->use_tcns_lut = false;
        }
    }

    /* davep 09-Jan-2009 ; support PIC BDR for image enhancement */
    if( sv->use_pic_bdr ) {
        scerr = uint32_array_new( &sv->pic_bdr_lut, PIC_BDR_LUT_SIZE );
        if (scerr == 0) {
            sv->use_pic_bdr_lut = true;
        }
        else {
            sv->use_pic_bdr_lut = false;
        }
    }

    /* davep 25-Oct-2011 ; new contrast/brightness equations */
    sv->brightness = 100;
    sv->contrast = 100;

    /* davep 21-Nov-2011 ; new sharpen/smooth */
    sv->sharpness = 100;
    sv->bgr_sensitivity = 0;	// add.lsh.for background removal
    
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
    return sv;
}

/**
 * \brief Mono Scan-to-Host scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_mono_scan_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_MONO_SCAN;

    sv->cmode = SCAN_CMODE_MONO;

    sv->pieout = PIE_PIXEL_MONO;

    /* davep 24-Jan-2012 ; use CSC for gamma, black/white clips */
    sv->use_csc = true;
    sv->use_csc_rgb_lut = true;
    sv->csc_srgb_ycc_bypass = true;
    sv->csc_ycc_rgb_bypass = true;
    /* not using the rgbycc or yccrgb matrices but we need the 
     * rgb2esrgb matrix
     */
    init_csc_rgb_matrix(sv->csc_rgb_matrix);

    csc_rgb_choose_luts(sv);
    opalloc_csc_rgb_lut(sv);

    sv->use_mf = true;
    sv->use_mf_ai_lut = true;
    sv->use_mf_te = false;

    if (sv->use_mf_ai_lut)
    {
        scerr = uint32_array_new( &sv->mf_ai_lut, PIE_FILTER_LUT_SIZE );
        if( scerr != 0 ) {
            sv->use_mf_ai_lut = false;
        }
    }

    sv->use_gamma = true;
    if (sv->use_gamma) {
        sv->gamma_times_10 = 24;
    }
    else {
        sv->gamma_times_10 = 10;
    }

    sv->contrast = 100;
    sv->brightness = 100;
    sv->sharpness = 100;

    sv->use_white_clip_point = true;
    sv->use_black_clip_point = true;

    if (sv->use_white_clip_point) {
#ifdef SDH_SCAN_TUNING
		//sv->white_clip_point[0] = 230;	// mod.lsh for complaint (republic of south africa)
		//sv->white_clip_point[0] = 200;		// mod.lsh for complaint (dubai user), 210 -> 200
		sv->white_clip_point[0] = 190;		// mod.lsh for complaint (dubai user 2nd), 200 -> 180
#else
        sv->white_clip_point[0] = 240;
#endif
		
    }
    else {
        sv->white_clip_point[0] = 255;
    }

    if (sv->use_black_clip_point) {
#ifdef SDH_SCAN_TUNING
		sv->black_clip_point[0] = 1;
#else
        sv->black_clip_point[0] = 10;
#endif
    }
    else {
        sv->black_clip_point[0] = 0;
    }

    scanvar_mono_scan_cal_white_black_targets(sv);

    /* davep 01-Oct-2006 ; break up color adjustments into class methods */
    /* davep 07-Jan-2009 ; point all three at the same method since all three of
     * gamma, contrast, brightness are done simultaneously in either TCNS-Y or
     * PIC BDR
     */
    /* davep 24-Jan-2012 ; move gamma and black/white clip into CSC */
    sv->set_gamma_curve = mono_scan_set_gamma_curve;
    sv->set_contrast_curve = mono_scan_set_enhance_curve;
    sv->set_brightness_curve = mono_scan_set_enhance_curve;

    /* davep 30-Dec-2005 ; integrating SIQ */
    scanvar_siq_merge( sv );

    // Must do this after the siq_merge so we pick up any changes from that.
    init_all_csc_rgb_luts( sv );

    if (sv->use_tcns_lut) {
        init_mono_tcns_lut( &sv->tcns_lut, sv->gamma_times_10,
                sv->contrast, sv->brightness,
                sv->black_clip_point[0],
                sv->white_clip_point[0] );
    }
    /* davep 09-Jan-2009 ; support PIC BDR for image enhancement */
    if( sv->use_pic_bdr_lut ) {
        init_scan_pic_bdr_lut(&sv->pic_bdr_lut, 
                              sv->gamma_times_10,
                              sv->black_clip_point[0],
                              sv->white_clip_point[0] );
    }

    // Merge again to pick up LUT changes. (LUT values are calculated by the init_*
    // functions above).
    scanvar_siq_merge( sv );
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
	
    return sv;
}

/**
 * \brief Color Scan-to-Host scanvar constructor 
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

struct scanvars * scanvar_color_scan_new( void )
{
    struct scanvars *sv;
    scan_err_t scerr;
	sv_dbg2("[SV] %s Enter =====\n", __FUNCTION__);
    sv = scanvar_scan_new();
    if( sv==NULL ) {
        return NULL;
    }

    g_id = sv->id = SVCID_COLOR_SCAN;

    /* davep 31-Oct-2012 ; FIXME need to eliminate float in making the TCNS lut */
#ifdef __KERNEL__
    sv->use_tcns = false;
    sv->use_tcns_lut = false;
#endif

    sv->cmode = SCAN_CMODE_COLOR;

    if( PIE_WDMA_NUM_CHANNELS==1 ) {
        sv->pieout = PIE_PIXEL_RGB;
    } 
    else {
        sv->pieout = PIE_PIXEL_3_PLANES;
    }

    sv->use_mf = true;
    sv->use_mf_ai_lut = true;
    sv->use_mf_te = true;
    if (sv->use_mf_ai_lut)
    {
        scerr = uint32_array_new( &sv->mf_ai_lut, PIE_FILTER_LUT_SIZE );
        if( scerr != 0 ) {
            sv->use_mf_ai_lut = false;
        }
    }
    if (sv->use_mf_te)
    {
        sv->te_count_black_min = 1;
        sv->te_count_white_min = 3;
        sv->te_count_white_plus_black_min = 7;
        sv->te_center_color_max = 96;
        sv->te_intense_color_max = 60;
        sv->te_black_y_max = 68;
        sv->te_black_CbCr_max = 70;
        sv->te_white_y_min = 107;
        sv->te_white_CbCr_max = 70;
        sv->te_use_5x5_matrix = true;
    }

    sv->use_gamma = true;
    if (sv->use_gamma) {
        sv->gamma_times_10 = 22;
    }
    else {
        sv->gamma_times_10 = 10;
    }

    sv->sharpness = 0;

    sv->use_white_clip_point = true;
    sv->use_black_clip_point = true;

    if (sv->use_white_clip_point)
    {
#ifdef SDH_SCAN_TUNING	// mod.lsh for complaint (republic of south africa)
        //sv->white_clip_point[0] = 220;	// mod.lsh for complaint (dubai user), 235 -> 220
        //sv->white_clip_point[1] = 220;
        //sv->white_clip_point[2] = 220;
        sv->white_clip_point[0] = 200;	// mod.lsh for complaint (dubai user), 235 -> 220
        sv->white_clip_point[1] = 200;
        sv->white_clip_point[2] = 200;
#else	
        sv->white_clip_point[0] = 245;
        sv->white_clip_point[1] = 245;
        sv->white_clip_point[2] = 245;
#endif
    }
    else
    {
        sv->white_clip_point[0] = 255;
        sv->white_clip_point[1] = 255;
        sv->white_clip_point[2] = 255;
    }

    if (sv->use_black_clip_point)
    {
#ifdef SDH_SCAN_TUNING	// to reduce wide dark range
        sv->black_clip_point[0] = 1;
        sv->black_clip_point[1] = 1;
        sv->black_clip_point[2] = 1;
#else
        sv->black_clip_point[0] = 10;
        sv->black_clip_point[1] = 10;
        sv->black_clip_point[2] = 10;
#endif
    }
    else
    {
        sv->black_clip_point[0] = 0;
        sv->black_clip_point[1] = 0;
        sv->black_clip_point[2] = 0;
    }

    /* davep 30-Dec-2005 ; turn on CSC by default */
    sv->use_csc = true;
    csc_rgb_choose_luts(sv);
    scerr = opalloc_csc_rgb_lut( sv );
    if( scerr != SCANERR_NONE ) {
        scanvar_delete( &sv );
        return NULL;
    }

    scanvar_color_scan_cal_white_black_targets(sv);

    /* davep 01-Oct-2006 ; break up color adjustments into class methods */
    sv->set_gamma_curve = color_scan_set_gamma_curve;
    sv->set_contrast_curve = color_scan_set_enhance_curve;
    sv->set_brightness_curve = color_scan_set_enhance_curve;

    /* davep 22-Dec-2005 ; integrating SIQ */
    scanvar_siq_merge( sv );

    // Must do this after the siq_merge so we pick up any changes from that.

    // NOTE: Gamma correction for color scans is performed in the CSC, not TCNS.
    // We're only using TCNS for contrast enhancement, so gamma, black/white
    // clip parameters are ignored.
    if (sv->use_tcns_lut) {
        init_color_tcns_lut(&sv->tcns_lut, sv->contrast, sv->brightness);
    }

    init_all_csc_rgb_luts( sv );

    init_csc_rgb_matrix(sv->csc_rgb_matrix);
    init_csc_rgbycc_matrix( sv->csc_rgbycc_matrix );
    init_csc_yccrgb_matrix( sv->csc_yccrgb_matrix );

    // Merge again to override anything the init() functions created on their own.
    scanvar_siq_merge( sv );
	
	sv_dbg2("[SV] %s Exit =====\n", __FUNCTION__);
	
    return sv;
}

/*************************************
 * Start of "Class Methods"          *
 *************************************/

/* davep 15-Aug-2009 ; create the "opalloc" methods to handle the case where a
 * descendent class changes a 'use_lut' but doesn't know to allocate data. 
 *
 * When/if the IQ class member functions [1] are called, we will do an
 * "opportunistic allocate" of the memory.
 *
 * Note if the memory alloc fails, the lut itself is disabled for safety.
 *
 * [1] set_gamma_curve, set_brightness_curve, set_contrast_curve
 */

static scan_err_t opalloc_array( bool *use_flag, struct uint32_array *array, int num_entries ) 
{
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( ! *use_flag ) {
        XASSERT( array->data==NULL, (uint32_t)array->data );
        XASSERT( array->num_entries==0, (uint32_t)array->num_entries);
        return SCANERR_NONE;
    }

    if( array->data==NULL ) {
        scerr = uint32_array_new( array, num_entries );
        if( scerr != 0 ) {
            *use_flag = false;
            return scerr;
        }
    }
    else {
        /* sanity check while we're here */
        XASSERT( array->num_entries==num_entries, array->num_entries );
    }

    return SCANERR_NONE;
}

static scan_err_t opalloc_mf_ai_lut( struct scanvars *sv )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    return opalloc_array( &sv->use_mf_ai_lut, &sv->mf_ai_lut, PIE_FILTER_LUT_SIZE );
}


static scan_err_t opalloc_pic_bdr_lut( struct scanvars *sv )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    return opalloc_array( &sv->use_pic_bdr_lut, &sv->pic_bdr_lut, PIC_BDR_LUT_SIZE );
}

static scan_err_t opalloc_tcns_lut( struct scanvars *sv )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    return opalloc_array( &sv->use_tcns_lut, &sv->tcns_lut, PIE_TCNS_LUT_SIZE );
}

static scan_err_t opalloc_csc_rgb_lut( struct scanvars *sv )
{
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( sv->use_csc_rgb_lut && sv->csc_rgb_lut.data==NULL ) {
        scerr = uint32_array_new( &sv->csc_rgb_lut, PIE_CSC_RGB_LUT_SIZE );
        if( scerr != 0) {
            sv->use_csc_rgb_lut = false;
            return scerr;
        }
    }
    if( sv->use_csc_r_lut && sv->csc_r_lut.data==NULL ) {
        scerr = uint32_array_new( &sv->csc_r_lut, PIE_CSC_RGB_LUT_SIZE );
        if( scerr != 0) {
            sv->use_csc_r_lut = false;
            return scerr;
        }
    }
    if( sv->use_csc_g_lut && sv->csc_g_lut.data==NULL ) {
        scerr = uint32_array_new( &sv->csc_g_lut, PIE_CSC_RGB_LUT_SIZE );
        if( scerr != 0) {
            sv->use_csc_g_lut = false;
            return scerr;
        }
    }
    if( sv->use_csc_b_lut && sv->csc_b_lut.data==NULL ) {
        scerr = uint32_array_new( &sv->csc_b_lut, PIE_CSC_RGB_LUT_SIZE );
        if( scerr != 0) {
            sv->use_csc_b_lut = false;
            return scerr;
        }
    }

    return 0;
}

/**
 * \brief Create a mono scan-to-host IQ curve. 
 *
 * As of this writing, the mono scan-to-host gamma, contrast, and brightness are
 * all built into either TCNS or PIC BDR (depending on ASIC). 
 *
 * We have three separate class methods: set_gamma_curve, set_contrast_curve,
 * and set_brightness_curve.  Currently all three point into this function.
 * When we have spiffier ASIC hardware, we could split the functionality among
 * different sub-blocks.
 *
 * Color scans in ASIC with full PIE support split the enhacement
 * responsibilities between CSC and TCNS.
 *
 * \author David Poole
 * \date 07-Jan-2009
 *
 * davep 24-Jan-2012 ; move gamma, black/white clip into CSC.
 * Contrast+Brightness now in TCNS Y channel.
 */

static void mono_scan_set_enhance_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    opalloc_tcns_lut(sv);
    opalloc_pic_bdr_lut(sv);

    if (sv->use_tcns_lut) {
        init_mono_tcns_lut(&sv->tcns_lut, 
                sv->gamma_times_10, sv->contrast, sv->brightness,
                sv->black_clip_point[0], sv->white_clip_point[0] );
    }

    /* davep 07-Jan-2009 ; support PIC BDR for image enhancement */
    if( sv->use_pic_bdr_lut ) {
        init_scan_pic_bdr_lut(&sv->pic_bdr_lut, 
                sv->gamma_times_10,
                sv->black_clip_point[0],
                sv->white_clip_point[0]);
    }
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 24-Jan-2012
 */

static void mono_scan_set_gamma_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    opalloc_csc_rgb_lut(sv);
    opalloc_pic_bdr_lut(sv);

    if( sv->use_csc ) {
        init_all_csc_rgb_luts( sv );
    }

    /* davep 14-Aug-2009 ; support PIC BDR for image enhancement */
    if( sv->use_pic_bdr_lut ) {
        init_scan_pic_bdr_lut(&sv->pic_bdr_lut, 
                sv->gamma_times_10,
                sv->black_clip_point[0],
                sv->white_clip_point[0]);
    }
}

/**
 * \brief  Create a color scan-to-host gamma curve.
 *
 * The color scan-to-host gamma curve is in the CSC RGB-to-sRGB block.
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

static void color_scan_set_gamma_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    opalloc_csc_rgb_lut(sv);
    opalloc_pic_bdr_lut(sv);

    if( sv->use_csc ) {
        init_all_csc_rgb_luts( sv );
    }

    /* davep 14-Aug-2009 ; support PIC BDR for image enhancement */
    if( sv->use_pic_bdr_lut ) {
        init_scan_pic_bdr_lut(&sv->pic_bdr_lut, 
                sv->gamma_times_10,
                sv->black_clip_point[0],
                sv->white_clip_point[0]);
    }
}

/**
 * \brief  Create the color scan-to-host contrast curve.
 *
 * As of this writing, contrast is in TCNS. If we find a better way to do it,
 * here's the place. Brightness is also in TCNS.
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

//static void color_scan_set_enhance_curve( struct scanvars *sv )
//{
//    opalloc_tcns_lut(sv);
//
//    if (sv->use_tcns_lut) {
//        /* NOTE: Gamma correction for color scans is performed in the CSC, not TCNS.
//         * We're only using TCNS for contrast and brightness so
//         * gamma,black/white clip are ignored.
//         */
//        init_color_tcns_lut(&sv->tcns_lut, 
//                10, sv->contrast, sv->brightness, 0, 0 );
//    }
//}

/**
 * \brief  Create the color scan brightness curve.
 *
 * As of this writing, brightness is in TCNS. If we find a better way to do it,
 * here's the place. Contrast is also in TCNS.
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

static void color_scan_set_enhance_curve( struct scanvars *sv )
//static void color_scan_set_brightness_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    opalloc_tcns_lut(sv);
    
    /* NOTE: Gamma correction and black/white clipping for color scans is
     * performed in the CSC, not TCNS.  We're only using TCNS for contrast and
     * brightness.
     */
    if (sv->use_tcns_lut) {
        init_color_tcns_lut(&sv->tcns_lut, sv->contrast, sv->brightness );
    }
}

/**
 * \brief Search the global copy brightness table. Return the default copy brightness settings. 
 *
 * Now we can have default local copy settings and platform specific copy
 * settings. Platform code needs to override sv->get_copy_brightness_settings
 * to their own function. If sv->get_copy_brightness_settings==NULL, we will
 * call this function to get some nice bland defaults.
 *
 * \author David Poole
 * \date 18-Apr-2012
 */

static scan_err_t get_default_copy_brightness_settings( const struct scanvars *sv,  
                                        struct copy_brightness_settings *settings )
{
    int i, idx;
    const struct copy_brightness_settings *ps;

	sv_dbg2("[SV] %s\n", __FUNCTION__);
    ps = NULL;
    for( i=0 ; all_copy_brightness[i].id != 0 ; i++ ) {
        if( all_copy_brightness[i].id == sv->id ) {
            ps = all_copy_brightness[i].settings;
        }
    }
    if( ps==NULL ) {
        /* not found so leave as default */
        return SCANERR_NO_ENTRY;
    }

    /* use the copy_brightness as an index into our table */
    idx = settings->copy_brightness-1;
    XASSERT( ps[idx].copy_brightness==settings->copy_brightness, 
            ps[idx].copy_brightness );

    memcpy( settings, &ps[idx], sizeof(struct copy_brightness_settings) );

    return SCANERR_NONE;
}

/**
 * \brief  Load the lightness/darkness mono laser copy settings into scanvars.
 *
 * Search the global table of mono laser copy scanvar settings for our current
 * copy type. Load those settings into the scanvar.
 *
 * See mono_copy_set_brightness_curve() for more details.
 *
 *
 * \author David Poole
 * \date 01-Oct-2006
 *
 */

static scan_err_t copy_load_lightdark_settings( struct scanvars *sv )
{
    scan_err_t scerr;
    struct copy_brightness_settings settings;
#if SDH_SIQ_TUNING
#else
    int i;
#endif

	// sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* call around, find the settings we want to load */
    memset( &settings, 0, sizeof(struct copy_brightness_settings) );
    settings.copy_brightness = sv->copy_brightness;

    /* davep 18-Apr-2012 ; add platform specific ability to get the
     * copy_brightness_settings 
     */
    if( sv->get_copy_brightness_settings ) {
        scerr = sv->get_copy_brightness_settings( &settings );
    }
    else {
        /* load the defaults */
        scerr = get_default_copy_brightness_settings( sv, &settings );
    }
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }
#if	SDH_SIQ_TUNING
#else
    /* TODO add smooth/sharpen? */
    sv->gamma_times_10 = settings.gamma_times_10;

    /* davep 15-May-2012 ; copy same value into all three clippings so we'll
     * cover color as well  TODO add separate r/g/b clips for white/black?
     */
    for( i=0 ; i<3 ; i++ ) {
        sv->white_clip_point[i] = settings.white_clip_point;
        sv->black_clip_point[i] = settings.black_clip_point;
    }
#endif
    sv_dbg2( "[SV] %s c_b=%d c_g=%d wcp=%d bcp=%d\n", __FUNCTION__,
                sv->copy_brightness, sv->gamma_times_10, 
                sv->white_clip_point[0], sv->black_clip_point[0] );

    return SCANERR_NONE;
}

static void mono_copy_set_enhance_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    mono_scan_set_enhance_curve( sv );
}

static void mono_copy_set_gamma_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    mono_scan_set_gamma_curve( sv );
}

/**
 * \brief Set the mixed mode fax brightness curve
 *
 *
 * \author David Poole
 * \date 01-Oct-2007
 *
 *    THIS IS A CHEAP HACK!
 *
 *    I'm *****ing sick of working on image tuning. Let's get this OVER WITH
 *
 * See the copious notes around and init_mono_tcns_lut().  
 *  
 * Fax+Text uses the Copy+Text mode settings and the old TCNS equation.
 * Fax+FilmPhoto uses the Copy+Mixed settings and the new equation. If I
 * continued to use mono_copy_set_brightness_curve(), I can have either Text or
 * Mixed mode. Not both simultaneously. 
 *
 * So force Fax+FilmPhoto to use the new equation by overriding the parent
 * class's set_brightness_curve method (see scanvar_mixed_fax_new()).
 *
 */
#if SDH_FAX_TUNING
#else
static void 
mono_fax_mixed_set_brightness_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    opalloc_tcns_lut(sv);

    if (sv->use_tcns_lut) {
        init_mono_tcns_lut(&sv->tcns_lut, 
                sv->gamma_times_10,
                sv->contrast, sv->brightness,
                sv->black_clip_point[0],
                sv->white_clip_point[0]);
    }
}
#endif
/**
 * \brief  Create the color copy gamma curve.
 *
 * \author David Poole
 * \date 06-Dec-2011
 *
 */

static void color_copy_set_gamma_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    color_scan_set_gamma_curve( sv );
}

/**
 * \brief  Create the color copy brightness curve.
 *
 *  As of this writing (06-Dec-2011), this function is a starting point. For
 *  now I'm only using the TCNS Y-channel for copy brightness.
 *
 *  The mono copy uses one function for all of gamma, contrast, and brightness
 *  but color copy is using an individual function for each. Color has more
 *  opportunities for enhancement so I'm keeping my options open.
 *
 * \author David Poole
 * \date 06-Dec-2011
 *
 */

static void color_copy_set_enhance_curve( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
	
    XASSERT( sv->cmode==SCAN_CMODE_COLOR, sv->cmode );

    /* davep 08-Dec-2011 ; TCNS Y brightness knob is [0,200] or [least,most]
     * brightness. Map copy's 11 to 0 (darkest) and 1 to 200 (lightest).
     */
    sv->brightness = (11-sv->copy_brightness) * 20;

    if( sv->use_tcns ) {
	color_scan_set_enhance_curve( sv );
    }

    /* davep 08-Dec-2011 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );
}

scan_err_t scanvar_set_copy_contrast( struct scanvars *sv, uint32_t copy_contrast )
{
    dbg2("%s contrast %d\n", __FUNCTION__, copy_contrast );
    sv_dbg2("[SV] %s (%d)\n", __FUNCTION__, copy_contrast);

    if( !sv->set_contrast_curve ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    if( !(copy_contrast >= 1 && copy_contrast <= 5) ){
        return SCANERR_INVALID_PARAM;
    }
    
    if( scanvar_siq_has_key( sv, SIQ_CONTRAST ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }
#if 0
    sv->contrast = 70+(copy_contrast*20);	// del.lsh for copy tuning
#else
    sv->contrast = 40+(copy_contrast*20);	// add.lsh for 1st copy tuning
#endif
    sv->set_contrast_curve( sv );

    /* davep 20-Jul-2006 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}


scan_err_t scanvar_set_contrast( struct scanvars *sv, uint32_t contrast )
{
    dbg2("%s contrast %d\n", __FUNCTION__, contrast );
	sv_dbg2("[SV] %s (%d)\n", __FUNCTION__, contrast);
    if( !sv->set_contrast_curve ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    if( scanvar_siq_has_key( sv, SIQ_CONTRAST ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }

    sv->contrast = contrast;
    sv->set_contrast_curve( sv );

    /* davep 20-Jul-2006 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

/**
 * \brief  Set the copy brightness
 *
 * The brightness is an index into a table containing various settings that
 * make up the copy quality. 
 *
 * \author David Poole
 * \date 
 */

scan_err_t scanvar_set_copy_brightness( struct scanvars *sv, uint8_t copy_brightness )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s (c_b:%d, id:%d)\n", __FUNCTION__, copy_brightness, sv->id);
    dbg2("%s %d\n", __FUNCTION__, copy_brightness );
    
#if SDH_COPY_TUNING
	
	copy_brightness = 10 - copy_brightness;	// add.lsh.for pinetree UI setting (1:brightest ~ 9:darkest)
			
	/* SDH Black/White Offset for Copy */
	switch(sv->id)
	{
		case SVCID_TEXT_COPY:
			sdh_white_offset = sdh_text_copy_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_text_copy_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_text_copy_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_text_copy_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] text mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset, 
																		sdh_white_guard, sdh_black_guard);
			break;
		case SVCID_MIXED_COPY:
			sdh_white_offset = sdh_mixed_copy_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_mixed_copy_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_mixed_copy_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_mixed_copy_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] mixed mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset, 
																		sdh_white_guard, sdh_black_guard);
			break;
		case SVCID_FILMPHOTO_COPY:
			sdh_white_offset = sdh_film_copy_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_film_copy_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_film_copy_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_film_copy_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] film mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset, 
																		sdh_white_guard, sdh_black_guard);
			break;
#if SDH_FAX_TUNING
		case SVCID_TEXT_FAX:
			sdh_white_offset = sdh_text_fax_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_text_fax_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_text_fax_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_text_fax_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] fax text mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset, 
																		sdh_white_guard, sdh_black_guard);
			break;
		case SVCID_MIXED_FAX:
			sdh_white_offset = sdh_mixed_fax_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_mixed_fax_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_mixed_fax_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_mixed_fax_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] fax mixed mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset, 
																		sdh_white_guard, sdh_black_guard);
			break;
#endif
		default:
			sdh_white_offset = sdh_mixed_copy_offset[copy_brightness-1].white_offset;
			sdh_black_offset = sdh_mixed_copy_offset[copy_brightness-1].black_offset;
			sdh_white_guard = sdh_mixed_copy_offset[copy_brightness-1].white_guard;
			sdh_black_guard = sdh_mixed_copy_offset[copy_brightness-1].black_guard;
			sv_dbg2("[SV] default mode (w_o:%d, b_o:%d, w_g:%d, b_g:%d)\n", sdh_white_offset, sdh_black_offset,
																		sdh_white_guard, sdh_black_guard);
			break;
	}
#endif	

    if( !sv->set_brightness_curve ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    if( !(copy_brightness >= 1 && copy_brightness <= 11) ){
    	dbg1("[SV] ERROR %s Inavlid Brightness Param\n", __FUNCTION__);
        return SCANERR_INVALID_PARAM;
    }

    if( scanvar_siq_has_key( sv, SIQ_COPY_BRIGHTNESS ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }

    sv->copy_brightness = copy_brightness;

    scerr = copy_load_lightdark_settings( sv );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* davep 24-Jan-2012 ; "copy brightness" is a lump sum of several tuning
     * parameters. Need to call each of our three polymorphic member functions
     * to rewrite our IQ curves with the settings for the new copy_brightness.
     */
    if( sv->set_gamma_curve ) {
        sv->set_gamma_curve( sv );
    }

    if( sv->set_brightness_curve ) {
        sv->set_brightness_curve( sv );
    }

    if( sv->set_contrast_curve ) {
        sv->set_contrast_curve( sv );
    }

    /* davep 1-Sep-2006 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

/**
 * \brief  Set the copy background
 *
 * 
 * This is for background algorithm using histogram on top of image (Pinetree)
 * 
 * \author Seunghwan Lee
 * \date 
 */
 
scan_err_t scanvar_set_background( struct scanvars *sv, uint32_t sensitivity )
{
    dbg2("%s sensitivity %d\n", __FUNCTION__, sensitivity );
	sv_dbg2("[SV] %s (%d)\n", __FUNCTION__, sensitivity);

	if( !(sensitivity >= 0 && sensitivity <= 9) ){	// 0 : off, 1 ~ 9 : on
		sv_dbg2("[SV] Error : Invalid Background Param (%d)\n", sensitivity);
        return SCANERR_INVALID_PARAM;
    }
    
    if( scanvar_siq_has_key( sv, SIQ_BACKGROUND ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }

    sv->bgr_sensitivity = sensitivity;

    /* davep 20-Jul-2006 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

scan_err_t scanvar_set_gamma( struct scanvars *sv, uint32_t gamma )
{
    dbg2("%s gamma=%d\n", __FUNCTION__, gamma );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( !sv->set_gamma_curve ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    if( !(gamma >= 10 && gamma <= 100) ) {
        return SCANERR_INVALID_PARAM;
    }
    
    if( scanvar_siq_has_key( sv, SIQ_GAMMA ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }

    sv->gamma_times_10 = gamma;
    sv->set_gamma_curve( sv );

    /* davep 28-Aug-2006 ; prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

scan_err_t scanvar_set_brightness( struct scanvars *sv, uint32_t brightness )
{
    dbg2( "%s brightness=%d\n", __FUNCTION__, brightness );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( !sv->set_brightness_curve ) {
        return SCANERR_NOT_IMPLEMENTED;
    }

    if( scanvar_siq_has_key( sv, SIQ_BRIGHTNESS ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }
    sv->brightness = brightness;
    sv->set_brightness_curve( sv );

    /* prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

scan_err_t scanvar_set_black_white_clip( struct scanvars *sv, uint16_t black_clip[3], uint16_t white_clip[3] )
{
    /* davep 02-Aug-2010 ; finally adding api function to set black/white clip.
     * It's only taken me, what, three years?  >.< 
     */
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2("%s bc=%d,%d,%d wc=%d,%d,%d\n", 
                __FUNCTION__, 
                black_clip[0], black_clip[1], black_clip[2], 
                white_clip[0], white_clip[1], white_clip[2] );

    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    /* only change if we didn't get a value from siqfile; siqdata overrides
     * everyone else 
     */
    if( ! scanvar_siq_has_key( sv, SIQ_BLACK_CLIP_POINT ) ) {
        sv->black_clip_point[0] = black_clip[0];
        sv->black_clip_point[1] = black_clip[1];
        sv->black_clip_point[2] = black_clip[2];

        sv->use_black_clip_point = true;
    }
    if( ! scanvar_siq_has_key( sv, SIQ_WHITE_CLIP_POINT ) ) {
        sv->white_clip_point[0] = white_clip[0];
        sv->white_clip_point[1] = white_clip[1];
        sv->white_clip_point[2] = white_clip[2];

        sv->use_white_clip_point = true;
    }

    /* davep 24-Jan-2012 ; gamma and black/white clip now 100% in CSC for both
     * mono and color
     */
    if( sv->use_csc ) {
        init_all_csc_rgb_luts( sv );
    }

    if( sv->use_pic_bdr_lut ) {
        /* We use PIC BDR when CSC and TCNS are not available. */
        init_scan_pic_bdr_lut(&sv->pic_bdr_lut, 
                sv->gamma_times_10,
                sv->black_clip_point[0],
                sv->white_clip_point[0]);
    }

    /* Merge again to keep siqfile as primary source of IQ data */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

/**
 * \brief Convenience function to set all 3 channels to same black/white clip
 *          values.
 *
 * \author David Poole
 * \date 04-Oct-2012
 */

scan_err_t scanvar_set_all_black_white_clip( struct scanvars *sv, 
                                             uint16_t black_clip_all, 
                                             uint16_t white_clip_all )
{
    int i;
    uint16_t black_clip[ SV_COLOR_NUM_ENTRIES ];
    uint16_t white_clip[ SV_COLOR_NUM_ENTRIES ];
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    for( i=0 ; i<SV_COLOR_NUM_ENTRIES ; i++ ) {
        black_clip[i] = black_clip_all;
        white_clip[i] = white_clip_all;
    }

    return scanvar_set_black_white_clip( sv, black_clip, white_clip );
}

scan_err_t scanvar_set_sharpness( struct scanvars *sv, uint32_t sharpness )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2("%s %d\n", __FUNCTION__, sharpness );

    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    if( scanvar_siq_has_key( sv, SIQ_SHARPNESS ) ) {
        /* don't change; siqdata overrides everyone else */
        return SCANERR_NONE;
    }

    sv->sharpness = sharpness;

    /* davep 04-Sep-2006 ; set up scanvar sharpness */
    scerr = scanvar_setup_filter_ai( sv );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    /* prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}

/**
 * \brief  Set an add/subtract factor for the red/green/blue planes. 
 *
 * Factored into the CSC RGB LUT which we currently use for gamma, black/white
 * clipping.
 *
 * \author David Poole
 * \date 18-May-2012
 */

scan_err_t scanvar_set_rgb_tint( struct scanvars *sv, int r_tint, int g_tint, int b_tint )
{
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->rgb_tint[SV_COLOR_RED] = r_tint;
    sv->rgb_tint[SV_COLOR_GREEN] = g_tint;
    sv->rgb_tint[SV_COLOR_BLUE] = b_tint;

    /* as of this writing rgb tint implemented in CSC LUT so re-generate the
     * LUT 
     */
    init_all_csc_rgb_luts( sv );

    /* prevent siqfile from being overwritten */
    scanvar_siq_merge( sv );

    return SCANERR_NONE;
}


/**
 * \brief  Calculate best hardware dpi to support a weird user dpi
 *
 * The call to scanvar_set_dpi() will convert a user dpi (such as 400, 1000,
 * 1234, 19200, 75, 4242, etc etc) into a dpi the sensor and motor hardware can
 * support.
 *
 * Algorithm is to find the next highest supported dpi then scale down. If no
 * higher dpi is available, the highest is chosen and we scale up.
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

static scan_err_t scanvar_calc_hardware_dpi( struct scanvars *sv )
{
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* stupid human check ; shouldn't be hardwiring the max dpi but rather
     * should be using scan_capabilities instead! 
     *
     * (these are mostly to catch uninitialized variables)
     */
    XASSERT( sv->dpi > 0 && sv->dpi <= 19200, sv->dpi );

    scerr = scancap_get_nearest_horiz_dpi( sv->doc_src, sv->dpi, &sv->hw_dpi_horiz );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    scerr = scancap_get_nearest_vert_dpi( sv->doc_src, sv->dpi, &sv->hw_dpi_vert );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    dbg2( "%s dpi user=%d horiz=%d vert=%d\n", __FUNCTION__, sv->dpi,
                sv->hw_dpi_horiz, sv->hw_dpi_vert );

    return SCANERR_NONE;
}

/**
 * \brief  Choose XYScale Linear mode or RAPR mode based on scale factors
 *
 * \author David Poole
 * \date 07-Oct-2013
 */

static void scanvar_calc_scaler_mode( struct scanvars *sv )
{
    scan_err_t scerr;
    uint32_t force_scaler;
    int x_numer, x_denom, y_numer, y_denom;

    /* davep 17-Nov-2006 ; do some smart switching between scaler modes ; might
     * want to revisit this later depending on how the quality comes out
     *
     * remember we can't use Linear mode on earlier asics
     *
     * if we're scaling up, use linear; if down use RAPR 
     */

    /* Use the PIE scale to decide. User scale should have been factored into
     * this by now.
     */
    scanvar_get_pie_scale(sv,&x_numer,&x_denom,&y_numer,&y_denom);

    if( x_numer < x_denom || y_numer < y_denom ) {
        /* if either X or Y are scaling down, must use RAPR */
        dbg2( "%s scaling down so use RAPR\n", __FUNCTION__ );
        sv->xy_scaler_mode = PIE_XY_SCALER_RAPR;
    }
    else if( x_numer==1 && x_denom==1 && y_numer==1 && y_denom==1 ) {
        /* Note I'm always enabling the scaler. I want only two paths
         * through the code: linear mode or rapr mode.  The scaler is
         * complicated enough without having to worry about
         * without adding bypass and having three paths.
         *
         * RAPR doesn't change the data on the way through. Linear mode
         * will eat a pixel each row on the way out.
         *
         * davep 04-Apr-2007 
         */
        dbg2( "%s not scaling so use RAPR\n", __FUNCTION__ );
        sv->xy_scaler_mode = PIE_XY_SCALER_RAPR;
    }
    else {
        dbg2( "%s scaling up so use Linear\n", __FUNCTION__ );
        sv->xy_scaler_mode = PIE_XY_SCALER_LINEAR;
    }

    /* davep 09-Jul-2013 ; add test/debug to force the scalar to a specific
     * mode. Feel free to shoot yourself in the foot.
     */
    scerr = scands_get_integer( "force_scaler", &force_scaler );
    if( scerr==SCANERR_NONE ) {
        switch( force_scaler ) {
            case 0:
                sv->xy_scaler_mode = PIE_XY_SCALER_BYPASS;
                break;
            case 1 :
                sv->xy_scaler_mode = PIE_XY_SCALER_RAPR;
                break;
            case 2:
                sv->xy_scaler_mode = PIE_XY_SCALER_LINEAR;
                break;

            default:
                /* invalid so ignore */
                dbg2( "%s ignore invalid force_scaler=%d\n", __FUNCTION__, force_scaler );
                break;
        }
    }

}

static scan_err_t scanvar_calc_scale( struct scanvars *sv )
{
    scan_err_t scerr;
    struct fraction f1, f2, fout;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%s dpi=%d h=%d v=%d\n", __FUNCTION__, 
                sv->dpi, sv->hw_dpi_horiz, sv->hw_dpi_vert );

    ASSERT( sv->hw_dpi_horiz > 0 );
    ASSERT( sv->hw_dpi_vert > 0 );

    if( sv->dpi == sv->hw_dpi_horiz ) {
        /* go back to user's scale */
        sv->pie_scale.x_numer = sv->scale.x_numer;
        sv->pie_scale.x_denom = sv->scale.x_denom;
    }
    else {
        /* scale to match the dpi */
        f1.numer = sv->scale.x_numer;
        f1.denom = sv->scale.x_denom;
        f2.numer = sv->dpi;
        f2.denom = sv->hw_dpi_horiz;
        fraction_multiply( &f1, &f2, &fout );

        dbg2( "%s change scale from x=%d/%d to x=%d/%d\n", __FUNCTION__, 
                    sv->pie_scale.x_numer, sv->pie_scale.x_denom,
                    fout.numer, fout.denom );

        sv->pie_scale.x_numer = fout.numer;
        sv->pie_scale.x_denom = fout.denom;
    }

    if( sv->dpi == sv->hw_dpi_vert ) {
        /* go back to user's scale */
        sv->pie_scale.y_numer = sv->scale.y_numer;
        sv->pie_scale.y_denom = sv->scale.y_denom;
    }
    else {
        /* scale to match the dpi */
        f1.numer = sv->scale.y_numer;
        f1.denom = sv->scale.y_denom;
        f2.numer = sv->dpi;
        f2.denom = sv->hw_dpi_vert;
        fraction_multiply( &f1, &f2, &fout );

        dbg2( "%s change scale from y=%d/%d to y=%d/%d\n", __FUNCTION__, 
                    sv->pie_scale.y_numer, sv->pie_scale.y_denom,
                    fout.numer, fout.denom );

        sv->pie_scale.y_numer = fout.numer;
        sv->pie_scale.y_denom = fout.denom;
    }

    scerr = scanvar_reduce_and_verify_xyscale( 
                &sv->pie_scale.x_numer,
                &sv->pie_scale.x_denom,
                &sv->pie_scale.y_numer,
                &sv->pie_scale.y_denom );

    return scerr;
}

/**
 * \brief  Set the user dpi. Calculate best hardware dpi from user dpi.
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

scan_err_t scanvar_set_dpi( struct scanvars *sv, uint32_t dpi )
{
    scan_err_t scerr;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    /* davep 06-Jan-2012 ; add simple brute force limits on dpi */
    if( dpi < 75 || dpi > 9600 ) {
        return SCANERR_INVALID_PARAM;
    }

    sv->dpi = dpi;

    /* davep 22-Aug-2009 ; we've changed the scan's DPI so recalculate 
     * hardware DPIs. 
     *
     * These are *PRIVATE* variables and should NOT be used by anything except
     * scantask code!  You'll blow your lips off, trust me.
     */
    sv->hw_dpi_horiz = sv->hw_dpi_vert = 0;

    scerr = scanvar_calc_hardware_dpi(sv);
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

#if SDH_MOIRE_TUNING_600x300	// add.lsh for moire test
	if ( ((sv->id >= 200) && (sv->id < 300)) && 					// Copy Only
		sv->dpi == 300 && 											// 300dpi Only
		sv->doc_src == SCAN_DOCUMENT_SOURCE_FLATBED)				// FB Only
	{				
		sv->hw_dpi_horiz = 600;
		dbg1( "[MOIRE] %s : 600x300x2 (id:%d, %d, %d, %d)\n", __FUNCTION__, sv->id, sv->dpi, sv->hw_dpi_horiz, sv->hw_dpi_vert);
	}
	else
	{
		dbg1( "[MOIRE] %s : other (id:%d, %d, %d, %d)\n", __FUNCTION__, sv->id, sv->dpi, sv->hw_dpi_horiz, sv->hw_dpi_vert);
	}
#endif	

    scerr = scanvar_calc_scale( sv );

    /* setting these back to zero is an attempt to cause an assert in scanlib
     * if our caller doesn't check the return value of scanvar_set_dpi()
     */
    if( scerr != 0 ) {
        sv->hw_dpi_horiz = sv->hw_dpi_vert = 0;
    }

    /* davep 07-Oct-2013 ; XYScale factors are set, at least for now. Choose
     * our scale mode based on scaling up or down
     */
    scanvar_calc_scaler_mode(sv);

    return scerr;
}

/**
 * \brief set the scan output BPP 
 *
 * The BPP (Bits Per Pixel) depends on what you actually want out of the scan. 
 *
 * Valid values are 1,2,4,8,16 
 *  1,2,4 - HalfPack
 *  8     - PIC, PIE
 *  16    - PIC
 * 
 * PIE in xRGB mode will send 32-bit pixels.
 *
 * \author David Poole
 * \date 27-Oct-2005
 *
 * davep 24-Nov-2008 ; adding support for 16-bpp scan-to-host
 *
 * davep 22-Mar-2013 ; attempt to make smart enough to switch PIE to the proper
 *                     mode on 32, 24 BPP.
 */

scan_err_t scanvar_set_bpp( struct scanvars *sv, uint8_t bpp )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( sv->use_hp ) {
        if( !(bpp==1 || bpp==2 || bpp==4 ) ) {
            return SCANERR_INVALID_PARAM;
        }
        sv->hp_bpp = bpp;
    }
    else if( sv->use_pie ) {
        switch( bpp ) {
            case 32 : 
                /* if we're not already XRGB or RGBX, default to XRGB */
                if( sv->pieout != PIE_PIXEL_XRGB && sv->pieout != PIE_PIXEL_RGBX ) {
                    scanvar_set_pieout( sv, PIE_PIXEL_XRGB );
                }
                break;
            case 24 :
                scanvar_set_pieout( sv, PIE_PIXEL_RGB );
                break;
            case 16 :
                sv->use_pie = false;
                sv->pic_bpp = 16;
                break;
            case 8:
                if( sv->cmode==SCAN_CMODE_MONO ) {
                    scanvar_set_pieout( sv, PIE_PIXEL_MONO );
                }
                else {
                    scanvar_set_pieout( sv, PIE_PIXEL_3_PLANES );
                }
                break;
            default:
                dbg1( "%s invalid bpp=%d\n", __FUNCTION__, bpp );
                return SCANERR_INVALID_PARAM;
        }
    }
    else {
        if( !(bpp==8 || bpp==16) ) {
            return SCANERR_INVALID_PARAM;
        }

        sv->pic_bpp = bpp;
    }

    return SCANERR_NONE;
}

/**
 * \brief  Calculate the bits-per-pixel
 *
 * The actual bits-per-pixel depends on the configuration of the scanvar. See
 * also scanvar_set_bpp()
 *
 * \author David Poole
 * \date 21-Mar-2013
 */

uint8_t scanvar_get_bpp( const struct scanvars *sv ) 
{
    uint8_t bpp;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    bpp = 8;
    if( sv->use_hp ) {
        bpp = sv->hp_bpp;
    }
    else if( sv->use_pie ) {
        switch( sv->pieout ) {
            case PIE_PIXEL_MONO :
            case PIE_PIXEL_3_PLANES :
                bpp = 8;
                break;

            case PIE_PIXEL_XRGB :
            case PIE_PIXEL_RGBX :
                bpp = 32;
                break;

            case PIE_PIXEL_RGB :
                bpp = 24;
                break;

            default:
                /* wtf? */
                XASSERT( 0, sv->pieout);
                break;
        }
    }
    else {
        bpp = sv->pic_bpp;
    }

    return bpp;
}

void scanvar_set_use_hp( struct scanvars *sv, bool enable )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_hp = enable;
}

void scanvar_set_use_pie( struct scanvars *sv, bool enable )
{
    /* davep 12-Dec-2006 ; adding ability to disable PIE's image enhancement
     * features. Added this note to explain the difference between
     * scanvar_set_use_pie() and scanvar_set_image_enhancments()
     *
     * scanpipe decides how to put together the DMA modules (PIC, PIE, HalfPack,
     * scanman; which data gets routed where) based on use_pie, use_hp, etc.
     * This use_pie flag means all data coming from PIC is next routed into PIE. 
     *
     * scanvar_set_image_enhancement() (see below) means data will still be
     * fed into PIE but CSC, TCNS, CPA, MF are all disabled. Scaler can still
     * be used. 
     */
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_pie = enable;
}

/**
 * \brief  Set the scan callback function pointer.
 *
 * When set, scantask will send outbound SMSG_* messages to this function. 
 *
 * Threads wanting a scan should set this member if they want to get data from
 * the scan.
 *
 * See also Bug 13964.
 *
 * \author David Poole
 * \date 02-Sep-2009
 *
 */

void scanvar_set_scan_callback( struct scanvars *sv, scan_callback_fn_t scan_callback_fn )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->scan_callback_fn = scan_callback_fn;
}

/**
 * \brief convenience function to get the scan area through params instead of a
 * structure
 *
 * Returns the user scan area measured in 1/100".
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

void scanvar_get_area( const struct scanvars *sv, long int *x, long int *y, long int *width, long int *height )
{
    /* davep 22-Apr-2008 ; this seems like a good place to add some sanity
     * checks
     */
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( sv->user_area_hinch.width > 0, (uint32_t)sv->user_area_hinch.width );
    XASSERT( sv->user_area_hinch.height > 0, (uint32_t)sv->user_area_hinch.height );

    *x = sv->user_area_hinch.x;
    *y = sv->user_area_hinch.y;
    *width = sv->user_area_hinch.width;
    *height = sv->user_area_hinch.height;
}

/**
 * \brief set the scannable area in 100ths of an inch 
 *
 * \param x upper left 
 * \param y upper right 
 * \param width width of scan 
 * \param height height of scan
 *
 * The parameters are taken in 1/100ths of an inch.
 *
 * Example:
 * \code
 * scanvar_set_area( 0, 0, 850, 1100 );
 * \endcode
 * will scan a full Letter sized page.
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

scan_err_t scanvar_set_area( struct scanvars *sv, int x, int y, int width, int height )
{
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
    XASSERT( width > 0, width );
    XASSERT( height > 0, height );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 03-Mar-2008 ; XXX temp debug */
    dbg1( "%s x=%d y=%d width=%d height=%d\n", __FUNCTION__, x, y, width, height );

    sv->user_area_hinch.x = x;
    sv->user_area_hinch.y = y;
    sv->user_area_hinch.width = width;
    sv->user_area_hinch.height = height;

    /* TODO verify area against platform capabilties */
    return SCANERR_NONE;
}

/**
 * \brief set the scanvar calibration field
 *
 *  see comments on enum of scanvar_cal_t for meaning
 *  of each.
 *
 */

void scanvar_set_cal_mode( struct scanvars *sv, scanvar_cal_t cal_mode)
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->cal_mode = cal_mode;
}

/** \brief  reduce and verify the scaler fractions 
 *
 * Verify against the limitations of the current scaler hardware and a few bugs
 * in the scaler firmware.
 *
 * \retval 0 success
 * \retval -1 bad scale factors
 *
 *
 * \author David Poole
 * \date 15-Mar-2007
 *
 */

scan_err_t scanvar_reduce_and_verify_xyscale( int *x_numer, int *x_denom, 
                                   int *y_numer, int *y_denom )
{
    struct fraction x_frac;
    struct fraction y_frac;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* use temporaries to make code a little less confusing */
    x_frac.numer = *x_numer;
    x_frac.denom = *x_denom;
    y_frac.numer = *y_numer;
    y_frac.denom = *y_denom;

    XASSERT( x_frac.numer>0, x_frac.numer );
    XASSERT( x_frac.denom>0, x_frac.denom );
    XASSERT( y_frac.numer>0, y_frac.numer );
    XASSERT( y_frac.denom>0, y_frac.denom );

//    dbg2("%s x=%d/%d y=%d/%d\n", __FUNCTION__, 
//                x_frac.numer, x_frac.denom, y_frac.numer, y_frac.denom );
    
    fraction_reduce( &x_frac );
    fraction_reduce( &y_frac );

//    dbg2("%s reduced to x=%d/%d y=%d/%d\n", __FUNCTION__, 
//                x_frac.numer, x_frac.denom, y_frac.numer, y_frac.denom );

    /* There are a lot of places deeper in the code that depend on 'int'.
     * That's my (davep) own dumb fault. Additionally, the way the PIE scaler
     * works is to normalize by (1<<16). Consequently, I have to be careful not
     * to overflow scale*(1<<16) in a signed int. So the largest I'm going to
     * allow a scale factor to be is 15 bits.
     */
    if( x_frac.numer > SCANVAR_MAX_SCALE_TERM || x_frac.denom > SCANVAR_MAX_SCALE_TERM || 
        y_frac.numer > SCANVAR_MAX_SCALE_TERM || y_frac.denom > SCANVAR_MAX_SCALE_TERM ) 
    {

        dbg1( "%s scale factor(s) out of range\n", __FUNCTION__ );
        return SCANERR_INVALID_PARAM;
    }

    /* check for scaler capability. As of this writing, the smallest we can
     * scale is 1/16 (0.625) and the largest we can scale up is 8/1 
     */
#ifdef __KERNEL__
    /* FIXME */
#else
    if( (float)x_frac.numer/(float)x_frac.denom < PIE_XY_SMALLEST_SCALE ||
        (float)x_frac.numer/(float)x_frac.denom > PIE_XY_LARGEST_SCALE  ||
        (float)y_frac.numer/(float)y_frac.denom < PIE_XY_SMALLEST_SCALE ||
        (float)y_frac.numer/(float)y_frac.denom > PIE_XY_LARGEST_SCALE ) 
    {

        /* bad settings */
        dbg2( "%s scale capability exceeded\n", __FUNCTION__ );
        return SCANERR_INVALID_PARAM;
    }
#endif

    *x_numer = x_frac.numer;
    *x_denom = x_frac.denom;
    *y_numer = y_frac.numer;
    *y_denom = y_frac.denom;

    return 0;
}

/**
 * \brief  simple wrapper around the reduce and verify function
 *
 * The reduce/verify function will modify the parameters. Wrote this one as a
 * simple wrapper to be able to pass in values I want to preserve.
 *
 * \author David Poole
 * \date 15-Mar-2007
 *
 */

scan_err_t scanvar_verify_xyscale( int x_numer, int x_denom, 
                        int y_numer, int y_denom )
{
    return scanvar_reduce_and_verify_xyscale( &x_numer, &x_denom, &y_numer, &y_denom );
}

/**
 * \brief set the XYScale factors 
 *
 * \param x_numer
 * \param x_denom
 * \param y_numer
 * \param y_denom
 * 
 * Set the XYScale scale factors using a horizontal (X) and vertical (Y) scale
 * fractions.
 *
 * For example:
 * \code
 * scanvar_set_xyscale( 1, 2, 1, 2 );
 * \endcode
 * will scale the image by 50% is both directions.
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

scan_err_t scanvar_set_xyscale( struct scanvars *sv, int x_numer, int x_denom, int y_numer, int y_denom )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );

    scerr = scanvar_reduce_and_verify_xyscale( &x_numer, &x_denom, &y_numer, &y_denom );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    sv->scale.x_numer = x_numer;
    sv->scale.x_denom = x_denom;
    sv->scale.y_numer = y_numer;
    sv->scale.y_denom = y_denom;

    /* davep 17-Dec-2010 ; given a new user scale, recalculate the scale factor
     * necessary if the user wants a weird DPI 
     */
    scerr = scanvar_calc_scale( sv );

    /* setting these back to zero is an attempt to cause an assert in scanlib
     * if our caller doesn't check the return value of scanvar_set_dpi()
     */
    if( scerr != 0 ) {
        sv->hw_dpi_horiz = sv->hw_dpi_vert = 0;
    }

    /* davep 07-Oct-2013 ; XYScale factors are set, at least for now. Choose
     * our scale mode based on scaling up or down
     */
    scanvar_calc_scaler_mode(sv);

    return scerr;
}

/**
 * \brief  Set a PIC scale factor
 *
 * Originally created to handle hacking a 75/150 DPI into ICE Lite based
 * products. ICE Lite can't XY scale color.
 *
 * Note: PIC can only scale horizontally.  The 'y' scaling has to be handled
 * elsewhere.
 *
 * \author David Poole
 * \date 01-Apr-2009
 *
 */

scan_err_t scanvar_set_pic_scale( struct scanvars *sv, 
                            int x_numer, int x_denom )
{
    struct fraction f;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* TODO add sanity checking */

    dbg2( "%s x=%d/%d\n", __FUNCTION__, x_numer, x_denom );

    f.numer = x_numer;
    f.denom = x_denom;
    fraction_reduce( &f );

    sv->use_pic_scale = true;

    /* PIC hscale can only scale horizontally so use 1/1 for Y */
    sv->pic_scale.x_numer = f.numer;
    sv->pic_scale.x_denom = f.denom;
    sv->pic_scale.y_numer = 1;
    sv->pic_scale.y_denom = 1;

    return SCANERR_NONE;
}


/**
 * \brief convenience function to get the XYScale factors as individual
 * parameters instead of a structure 
 *
 * Note: This function returns the *USER* scale factors NOT the actual scale
 * factors applied to the image.  
 *
 * The actual image scale factor could include additional scaling to scale a
 * hardware DPI down to a user DPI.  For example, a user requests 150 DPI we
 * scan at 300 DPI and scale down 1/2.
 *
 * \author David Poole
 * \date 28-Oct-2005
 *
 */

void scanvar_get_xyscale( const struct scanvars *sv, 
                          int *x_numer, int *x_denom, 
                          int *y_numer, int *y_denom )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( sv->scale.x_numer>0 && sv->scale.x_numer<=SCANVAR_MAX_SCALE_TERM, sv->scale.x_numer );
    XASSERT( sv->scale.x_denom>0 && sv->scale.x_denom<=SCANVAR_MAX_SCALE_TERM, sv->scale.x_denom );
    XASSERT( sv->scale.y_numer>0 && sv->scale.y_numer<=SCANVAR_MAX_SCALE_TERM, sv->scale.y_numer );
    XASSERT( sv->scale.y_denom>0 && sv->scale.y_denom<=SCANVAR_MAX_SCALE_TERM, sv->scale.y_denom );

    *x_numer = sv->scale.x_numer;
    *x_denom = sv->scale.x_denom;
    *y_numer = sv->scale.y_numer;
    *y_denom = sv->scale.y_denom;
}

/**
 * \brief Get the Mono/Color scan type 
 *
 * Pull from the active scanvar what type of scan we're doing: Color or Mono.
 *
 * \author David Poole
 * \date 17-Aug-2005
 *
 */

scan_cmode_t scanvar_get_cmode( void )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    STACKTOP( sv );
    return sv->cmode;
}

/**
 * \brief Set the PIE output data type
 *
 * Set into the active scanvar what data format we're getting from PIE (mono, XRGB, planar, etc.)
 *
 * \author Brad Smith
 * \date 30-Aug-2005
 *
 */

void scanvar_set_pieout( struct scanvars *sv, pie_pixel_t dtype )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
    sv->pieout = dtype;
}

/**
 * \brief Get the PIE output data type
 *
 * Pull from the active scanvar what data format we're getting from PIE (mono, XRGB, planar, etc.)
 *
 * \author Brad Smith
 * \date 30-Aug-2005
 *
 */

pie_pixel_t scanvar_get_pieout( void )
{
    struct scanvars *sv;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    STACKTOP( sv );
    return sv->pieout;
}

/**
 * \brief get/set the PIC BPP 
 *
 * \author David Poole
 * \date 14-Oct-2005
 */

int 
scanvar_get_pic_bpp( void )
{
    struct scanvars *sv;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    STACKTOP( sv );
    XASSERT( sv->pic_bpp==8||sv->pic_bpp==16, sv->pic_bpp );
    return sv->pic_bpp;
}

/**
 * \brief get/set the PIC BPP 
 *
 * \author David Poole
 * \date 14-Oct-2005
 */

scan_err_t scanvar_set_pic_bpp( struct scanvars *sv, int pic_bpp ) 
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( !(pic_bpp==8||pic_bpp==16) ) {
        return SCANERR_INVALID_PARAM;
    }
    sv->pic_bpp = pic_bpp;
    return SCANERR_NONE;
}

/**
 * \brief set the PIE extra row size restriction modulo 
 *
 * Some hardware platforms (e.g., inkjet) need a rowsize that is larger than
 * our minimum 16-byte DMA alignment size. We add in more XYScale padding to
 * make the rowsize come out correctly.
 *
 * \author David Poole
 * \date 14-Nov-2005
 *
 * davep 31-Oct-2007 ; moving from hardwired 16-byte assumption to #def 
 */

void scanvar_set_pie_row_align( struct scanvars *sv, int align )
{
    XASSERT( align % ICE_DMA_BYTE_ALIGN == 0, align );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->pie_row_align_bytes = align;
}

/**
 * \brief Set the amount we'll truncate PIE output to.
 *
 * Needed a way to store the number of rows PIE should appear to output after
 * calculating the input rows against the "intuitive" scale factor. Remember
 * 1500/3=499 so we do all sorts of extra padding. The PIE layer will call this
 * function once all those calculations are done so we can know how much the
 * user might *really* expect (e.g., 1500/3=500).
 *
 * This should only be called from the internal PIE code once all the XY scale
 * calculations are done. It's a bit a kludge to put the truncate rows into
 * scanvars but the scaler is driving me crazy and I just want to be done with
 * it.
 *
 * \author David Poole
 * \date 11-Jul-2006
 *
 */

void
scanvar_set_pie_truncate_rows( struct scanvars *sv, int rows )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->pie_truncate_rows = rows;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 15-Jul-2006
 *
 */

void
scanvar_set_xy_scale_mode( struct scanvars *sv, int xy_scaler_mode )
{
    XASSERT( xy_scaler_mode==PIE_XY_SCALER_LINEAR ||
             xy_scaler_mode==PIE_XY_SCALER_RAPR, 
             xy_scaler_mode );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* Warning: earlier ASIC only does RAPR but we're not checking for that
     * here
     */
    sv->xy_scaler_mode = xy_scaler_mode;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 12-Oct-2006
 *
 */

void
scanvar_set_capture_cal( struct scanvars *sv, bool flag )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->capture_cal = flag;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 12-Dec-2006
 *
 */

void scanvar_disable_image_enhancements( struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_csc = false;    
    sv->use_tcns = false;
    sv->use_mf = false;
    sv->use_mf_te = false;

    /* davep 12-Dec-2006 ; TODO don't forget to add CPA if/when we start using
     * that block
     */

    /* davep 09-Jan-2009 ; adding PIC BDR */
    sv->use_pic_bdr = false;

    /* davep 04-Apr-2013 ; adding PIE's variable coefficients block */
    sv->use_mf_varcoefficients = false;
}

/**
 * \brief  Enable the software data horizontal flip
 *
 * When enabled, PIE WDMA data will be fliped left-to-right before sending to
 * HalfPack (copies) or ScanMan (scan-to-host).
 *
 * Originally created to support an MFP which has to scan ADF pages
 * backwards.
 *
 * \author David Poole
 * \date 02-Jan-2009
 *
 */

void scanvar_set_flip_horizontal( struct scanvars *sv, bool flag )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* trust no one */
    flag = flag?true:false;
    
    sv->sensor_flip_horizontal = flag;
}

/**
 * \brief Request a scan from a given document source.
 *
 *  Default scanvar->doc_src is auto which leaves the scan source up to the mech
 *  driver. For example, in an adf+flatbed product, always scan from ADF if
 *  paper loaded in the ADF.
 *
 *  However, if the doc source is set, the mech will make an effort to scan from
 *  that document source. To continue the previous example, if the doc src is
 *  set to flatbed, the adf+flatbed will scan from flatbed even if a document is
 *  loaded in the adf.
 *
 * \author David Poole
 * \date 04-Jun-2009
 *
 */

scan_err_t scanvar_set_document_source( struct scanvars *sv, 
                                    scan_document_source_t doc_src )
{
    XASSERT( sv->cookie==SCANVAR_COOKIE, sv->cookie );
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    if( doc_src > SCAN_DOCUMENT_SOURCE_ADF_DUPLEX ) {
        return SCANERR_INVALID_PARAM;
    }

    sv->doc_src = doc_src;
    return SCANERR_NONE;
}

/**
 * \brief  convert user area from 1/100 inch ("hinches") into pixels
 *
 * Takes into account just about everything about the scan up to and including
 * the phase of the moon.
 *
 *
 * \author David Poole
 * \date 13-Mar-2008
 *
 */

void scanvar_calc_pixel_area( struct scanvars *sv )
{
    long int int_x, int_y, int_width, int_height;
    uint32_t right_margin_pixel, max_right_pixel;
    int bytes_per_row;
    scan_sensor_margin_t sensor_margins;
    scan_err_t scerr;
    unsigned long int pixels_per_hinch_horiz, pixels_per_hinch_vert;
    uint32_t max_length_hinches;
    
	sv_dbg2("[SV] %s\n", __FUNCTION__);
	
    ASSERT( sv->hw_dpi_horiz != 0 );
    ASSERT( sv->hw_dpi_vert != 0 );

    scanvar_get_area( sv, &int_x, &int_y, &int_width, &int_height );

    dbg2( "%s user area hinches x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__, 
                int_x, int_y, int_width, int_height );

    /* davep 08-Jun-2012 ; remove some floating point, do everything in
     * hinches. Require all hardware DPIs are multiples of 100. Hope this
     * doesn't bite me in the future. 
     */
    XASSERT( sv->hw_dpi_horiz%100==0, sv->hw_dpi_horiz );
    XASSERT( sv->hw_dpi_vert%100==0, sv->hw_dpi_vert );

    pixels_per_hinch_horiz = sv->hw_dpi_horiz / 100;
    pixels_per_hinch_vert = sv->hw_dpi_vert / 100;

    scerr = scansen_get_margins( sv->hw_dpi_horiz, &sensor_margins );
    XASSERT( scerr==0, scerr );
    max_right_pixel = sensor_margins.left + sensor_margins.active;

    dbg2( "%s sensor left=%d active=%d\n", __FUNCTION__, sensor_margins.left, sensor_margins.active );

    /* davep 25-Jul-2013 ; restrict length to max reported for this docsrc */
    max_length_hinches = scanplat_get_max_scan_length(sv->doc_src);
    dbg2( "%s user height=%d max_height=%d\n", __FUNCTION__, 
            int_height, max_length_hinches );
    if( int_height > max_length_hinches ) {
        dbg2( "%s truncating length=%d to max length=%d\n", __FUNCTION__, 
                int_height, max_length_hinches );
        int_height = max_length_hinches;
    }

    memset( &sv->scan_area_pixels, 0, sizeof( sv->scan_area_pixels) );

    /* davep 08-Jun-2012 ; convert from hinches to pixels in integer.
     * The scan_area fields are signed long int so assume 31 bits.
     * The pixels per inch horiz/vert are, say, 19200 max (crazy high) so
     * that's 15 bits. The x,y,width,height are hinches. We have 16-bits worth
     * of space for hinches before the scan_area overflows. ~65" is a lot of
     * wiggle room.
     */
    sv->scan_area_pixels.x = int_x * pixels_per_hinch_horiz;
    sv->scan_area_pixels.y = int_y * pixels_per_hinch_vert;
    sv->scan_area_pixels.width  = int_width * pixels_per_hinch_horiz;
    sv->scan_area_pixels.height = int_height * pixels_per_hinch_vert;

    /* the user is asking for this area of pixels to be captured */
    dbg2( "%s user area pixels x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__, 
                sv->scan_area_pixels.x, sv->scan_area_pixels.y,
                sv->scan_area_pixels.width, sv->scan_area_pixels.height );

    /* adjust left side pixel to avoid the sensor's dead pixels */
    sv->scan_area_pixels.x += sensor_margins.left;

    right_margin_pixel = sv->scan_area_pixels.x + sv->scan_area_pixels.width;

    /* davep 06-Dec-2010 ; tinkering; ignore user margins, use sensor margins.
     * We'll still need to use PIC margins to clip sensor's dead left/right
     * pixels and for PIC WDMA alignment issues.
     */
    /* davep 01-Feb-2012 ; disable this code so we can use fwmargins for
     * chipgap correction
     */
#if 0
    if( sv->use_fw_margins ) {
        dbg2( "%s ignoring user margins\n", __FUNCTION__ );
        sv->scan_area_pixels.x = sensor_margins.left;
        right_margin_pixel = max_right_pixel;
        sv->scan_area_pixels.width = right_margin_pixel - sv->scan_area_pixels.x;

        /* davep 16-Dec-2010 ; user margins live between PIC->PIE and currently
         * only support 8bpp 
         */
        XASSERT( sv->pic_bpp==8, sv->pic_bpp );
    }
#endif

    /* davep 08-Dec-2010 ; experimental feature; set margins to max width
     * (needs more testing!) 
     */
    if( scan_get_config() & SCAN_CONFIG_NO_XMARGINS ) {
        sv->scan_area_pixels.x = 0;
        sv->scan_area_pixels.width = sensor_margins.left + sensor_margins.active + sensor_margins.right;
        right_margin_pixel = max_right_pixel;
        dbg1( "%s setting horizontal pixel margins to max x=%d width=%d\n", __FUNCTION__, 
                sv->scan_area_pixels.x, sv->scan_area_pixels.width );
    }

    /* davep 30-Sep-2011 ; instead of always truncating, we grab as much as we
     * can (padding) until we exceed the max. Then we'll truncate.
     */
    scerr = pic_marg_calc_fit( sv->scan_area_pixels.x, 
                                 &right_margin_pixel, sv->pic_bpp,
                                 PIC_MARG_PIXELFIT_PAD_RIGHT );
    XASSERT( scerr==0, scerr );

    /* davep 30-Sep-2011 ; we padded up to DMA boundaries. Did we pad too far?  */
    if( right_margin_pixel > max_right_pixel ) {
        dbg1( "%s right_margin_pixel=%d exceeds max=%d so truncating instead\n", 
                    __FUNCTION__, right_margin_pixel, max_right_pixel );

        /* Set right margin to max. Try try again. */
        right_margin_pixel = max_right_pixel;

        /* still need to be dma aligned so use truncation instead */
        scerr = pic_marg_calc_fit( sv->scan_area_pixels.x, 
                                     &right_margin_pixel, sv->pic_bpp,
                                     PIC_MARG_PIXELFIT_TRUNCATE_RIGHT );
        XASSERT( scerr==0, scerr );
    }

    /* adjust the scanned area width with our (maybe) adjusted right pixel */
    if( sv->scan_area_pixels.width != right_margin_pixel - sv->scan_area_pixels.x ) {
        sv->scan_area_pixels.width = right_margin_pixel - sv->scan_area_pixels.x;        
        dbg2( "%s changed pixel width to %ld for PIC WDMA\n", __FUNCTION__,
                    sv->scan_area_pixels.width );
    }

    /* sanity checks */
    bytes_per_row = sv->scan_area_pixels.width * (sv->pic_bpp/8);
    XASSERT( bytes_per_row%ICE_DMA_BYTE_ALIGN==0, bytes_per_row );

    dbg1( "%s final scan area x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__,
                sv->scan_area_pixels.x, sv->scan_area_pixels.y, 
                sv->scan_area_pixels.width, sv->scan_area_pixels.height );

    XASSERT( sv->scan_area_pixels.x+sv->scan_area_pixels.width <= max_right_pixel,
                max_right_pixel );
}

/**
 * \brief get a copy of the scan area in pixels
 *
 * Note: pixel area could be zero if scanvar_set_pixel_area() hasn't been
 * called.
 *
 * \author David Poole
 * \date 21-Apr-2008
 *
 */

void
scanvar_get_pixel_area( const struct scanvars *sv, struct scan_area *pixel_area )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    pixel_area->x = sv->scan_area_pixels.x;
    pixel_area->y = sv->scan_area_pixels.y;
    pixel_area->width = sv->scan_area_pixels.width;
    pixel_area->height = sv->scan_area_pixels.height;
}

/**
 * \brief This is not the function you are looking for. You're probably
 * looking for scanvar_set_area().
 *
 * Directly set the pixel area to scan.  Explicitly created for use only by
 * calibration.
 *
 * \author David Poole
 * \date 13-Mar-2008
 *
 */

void
scanvar_set_pixel_area( struct scanvars *sv, struct scan_area *pixel_area ) 
{
    int bytes_per_row;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 03-Mar-2008 ; XXX temp debug */
    dbg2( "%s x=%ld y=%ld width=%ld height=%ld\n", __FUNCTION__, pixel_area->x,
                pixel_area->y, pixel_area->width, pixel_area->height );

    /* stupid human check */
    XASSERT( pixel_area->width > 0, (uint32_t)pixel_area->width );
    XASSERT( pixel_area->height > 0, (uint32_t)pixel_area->height );

    /* there is an implicit linkage between scanvar's user_area_hinch and
     * scan_area_pixels.  The scan_area_pixels can be calculated from the
     * user_area_hinch and dpi. But as of this function's creation
     * (13-Mar-2008), there is nothing to do the other way (pixels to hinches).
     * 
     * So since we're directly mucking with the pixel area, we can't trust that
     * the hinches area is identical.
     *
     * It's not as much of a problem because the pixel area is only meant to be
     * used by internal code.
     *
     * Note: We must be sure not change the scanvar hinches area because that's
     * the area the user specified. We need to preserve it in case we're doing
     * an ADF scan (we use the scanvar multiple times in a row) and so we can
     * report the correct pixels-per-row downstream. (These are kind of weak
     * reasons but don't change scanvar hiches regardless.)
     */

    sv->scan_area_pixels.x = pixel_area->x;
    sv->scan_area_pixels.y = pixel_area->y;
    sv->scan_area_pixels.width = pixel_area->width;
    sv->scan_area_pixels.height = pixel_area->height;

    /* stupid human check; pic_bpp should be 8 or 16 ; bytes_per_row must be PIC
     * WDMA aligned
     */
    XASSERT( sv->pic_bpp==8 || sv->pic_bpp==16, sv->pic_bpp );
    bytes_per_row = sv->scan_area_pixels.width * (sv->pic_bpp/8);
    XASSERT( bytes_per_row%ICE_DMA_BYTE_ALIGN==0, bytes_per_row );        
}

/**
 * \brief  return the hardware (sensor+motor) dpis calculated from the user dpi
 *
 * See scanvar_set_dpi()
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

void scanvar_get_hw_dpi( const struct scanvars *sv, uint32_t *horiz_dpi, uint32_t *vert_dpi )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    *horiz_dpi = sv->hw_dpi_horiz;
    *vert_dpi = sv->hw_dpi_vert;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 17-Dec-2010
 *
 */

void scanvar_get_pie_scale( const struct scanvars *sv, 
                            int *x_numer, int *x_denom, 
                            int *y_numer, int *y_denom )
{
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* davep 14-Jun-2012 ; XXX temp debug  */
//    dbg2( "%s %d %d %d %d\n", __FUNCTION__, 
//            sv->pie_scale.x_numer, 
//            sv->pie_scale.x_denom, 
//            sv->pie_scale.y_numer, 
//            sv->pie_scale.y_denom );

    /* these asserts are here to catch upstream code that might have bypassed
     * error checks
     */
    XASSERT( sv->pie_scale.x_numer>0 && sv->pie_scale.x_numer<=SCANVAR_MAX_SCALE_TERM, sv->pie_scale.x_numer );
    XASSERT( sv->pie_scale.x_denom>0 && sv->pie_scale.x_denom<=SCANVAR_MAX_SCALE_TERM, sv->pie_scale.x_denom );
    XASSERT( sv->pie_scale.y_numer>0 && sv->pie_scale.y_numer<=SCANVAR_MAX_SCALE_TERM, sv->pie_scale.y_numer );
    XASSERT( sv->pie_scale.y_denom>0 && sv->pie_scale.y_denom<=SCANVAR_MAX_SCALE_TERM, sv->pie_scale.y_denom );

    *x_numer = sv->pie_scale.x_numer;
    *x_denom = sv->pie_scale.x_denom;
    *y_numer = sv->pie_scale.y_numer;
    *y_denom = sv->pie_scale.y_denom;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 12-Oct-2011
 *
 */

scan_err_t scanvar_add_page_op( struct scanvars *sv, const char *opname, scan_page_range page_range )
{
    int i;
    int len;
    struct scanvar_page_op *op_list;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    len = strnlen( opname, SCANVAR_PAGE_OP_NAME_MAX );
    if( len >= SCANVAR_PAGE_OP_NAME_MAX ) {
        return SCANERR_INVALID_PARAM; 
    }

    op_list = sv->page_op_list;

    /* linear search; bleah */
    for( i=0 ; i<SCANVAR_PAGE_OP_MAX ; i++ ) {
        if( op_list[i].name[0]==0 ) {
            /* found an empty slot! */
            strncpy( op_list[i].name, opname, SCANVAR_PAGE_OP_NAME_MAX );
            op_list[i].page_range = page_range;
            return SCANERR_NONE;
        }

        if( str_match( op_list[i].name, opname, len ) ) {
            /* replace the existing value */
            op_list[i].page_range = page_range;
        }
    }

    return SCANERR_OUT_OF_MEMORY;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 12-Oct-2011
 *
 */

scan_err_t scanvar_get_page_op( struct scanvars *sv, const char *opname, scan_page_range *page_range )
{
    int i;
    int len;
    struct scanvar_page_op *op_list;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    len = strnlen( opname, SCANVAR_PAGE_OP_NAME_MAX );
    if( len >= SCANVAR_PAGE_OP_NAME_MAX ) {
        return SCANERR_INVALID_PARAM; 
    }

    op_list = sv->page_op_list;

    /* linear search; bleah */
    for( i=0 ; i<SCANVAR_PAGE_OP_MAX ; i++ ) {
        if( str_match( op_list[i].name, opname, len ) ) {
            /* found it! */
            *page_range = op_list[i].page_range;
            return SCANERR_NONE;
        }
    }

    return SCANERR_NO_ENTRY;
}

/**
 * \brief  Peek into settings and determine what final data type
 *
 * Need to know final data type so can send in each data message. 
 * 
 *
 * \author David Poole
 * \date 13-Jan-2012
 */

scan_data_type scanvar_get_final_data_type( const struct scanvars *sv )
{
    scan_data_type dtype;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    dtype = SCAN_DATA_NULL_TYPE;

    if( sv->use_hp ) {
        switch( sv->hp_bpp ) {
            case 1 :
                dtype = SCAN_DATA_TYPE_HP1BPP;
                break;
            case 2 :
                dtype = SCAN_DATA_TYPE_HP2BPP ;
                break;
            case 4 :
                dtype = SCAN_DATA_TYPE_HP4BPP ;
                break;
            default :
                XASSERT(0, sv->hp_bpp);
        }
    }
    else if( sv->use_pie ) {
        switch( sv->pieout ) {
            case PIE_PIXEL_MONO:
                dtype = SCAN_DATA_TYPE_MONO;
                break;
            case PIE_PIXEL_3_PLANES:
                /* we'll have to demux between the red, green, blue planes
                 * while we're scanning
                 */
                dtype = SCAN_DATA_TYPE_PLANE;
                break;
            case PIE_PIXEL_XRGB:
                dtype = SCAN_DATA_TYPE_XRGB;
                break;
            case PIE_PIXEL_RGBX:
                dtype = SCAN_DATA_TYPE_RGBX;
                break;
            case PIE_PIXEL_RGB:
                dtype = SCAN_DATA_TYPE_RGB;
                break;
            default :
                XASSERT( 0, sv->pieout );
        }
    }
    else {
        /* only PIC */
        if( sv->cmode == SCAN_CMODE_COLOR) {
            dtype = SCAN_DATA_TYPE_PLANE;
        }
        else {
            dtype = SCAN_DATA_TYPE_MONO;
        }
    }

    return dtype;
}

static const struct {
    scan_data_type dtype;
    const char *name;
} dtype_to_name[] = {
    { SCAN_DATA_TYPE_XRGB, "xrgb" },
    { SCAN_DATA_TYPE_PLANE, "planar" },
    { SCAN_DATA_TYPE_RED, "red" },
    { SCAN_DATA_TYPE_GREEN, "green" },
    { SCAN_DATA_TYPE_BLUE, "blue" },
    { SCAN_DATA_TYPE_MONO, "mono" },
    { SCAN_DATA_TYPE_HP1BPP, "hp1bpp" },
    { SCAN_DATA_TYPE_HP2BPP, "hp2bpp" },
    { SCAN_DATA_TYPE_HP4BPP, "hp4bpp" },
    { SCAN_DATA_TYPE_RGBX, "rgbx" },
    { SCAN_DATA_TYPE_RGB, "rgb" },

    /* end of list marker */
    {SCAN_DATA_NULL_TYPE, "" }
};

const char *scanvar_get_scan_data_type_name( scan_data_type dtype )
{
    int i;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    for( i=0 ; ; i++ ) {
        if( dtype_to_name[i].dtype==dtype ) {
            return dtype_to_name[i].name;
        }

        if( dtype_to_name[i].dtype==SCAN_DATA_NULL_TYPE ) {
            /* end of list */
            break;
        }
    }

    return "(unknown)";
}

/**
 * \brief  Populate the scanvar mf_ai_lut 
 *
 * Expert mode.
 *
 * \author David Poole
 * \date 01-Apr-2013
 */

scan_err_t scanvar_set_mf_ai_lut( struct scanvars *sv, const uint8_t *ai_lut_y, const uint8_t *ai_lut_cbcr )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    sv->use_mf_ai_lut = true;
    scerr = init_mf_ai_lut( sv, ai_lut_y, ai_lut_cbcr );
    return scerr;
}

static scan_err_t init_mf_coeff_lut( uint32_t *lut, const uint32_t *new_lut )
{
    int i;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    for( i=0 ; i<MF_COEF_MAX_COEFFICIENTS ; i++ ) {
        if( new_lut[i] & ~PIE_SCULPTOR_VARCOEF_MASK ) {
            return SCANERR_INVALID_PARAM;
        }
    }
    for( i=0 ; i<MF_COEF_MAX_COEFFICIENTS ; i++ ) {
        lut[i] = new_lut[i];
    }
    return SCANERR_NONE;
}

scan_err_t scanvar_set_mf_coef_lut( struct scanvars *sv, 
                                     const uint32_t *varcoef_y, 
                                     const uint32_t *varcoef_chroma )
{
    scan_err_t scerr;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
#if !defined HAVE_PIE_SCULPTOR_5X5 && !defined HAVE_PIE_SCULPTOR_5X9
    sv->use_mf_varcoefficients = false;
    return SCANERR_NOT_IMPLEMENTED;
#endif
    sv->use_mf_varcoefficients = true;

    if( varcoef_y ) {
        memset( sv->mf_varcoef_y, 0, sizeof(sv->mf_varcoef_y) );
        scerr = init_mf_coeff_lut( sv->mf_varcoef_y, varcoef_y );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }
    if( varcoef_chroma ) {
        memset( sv->mf_varcoef_chroma, 0, sizeof(sv->mf_varcoef_chroma) );
        scerr = init_mf_coeff_lut( sv->mf_varcoef_chroma, varcoef_chroma );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }

    return SCANERR_NONE;
}

/**
 * \brief  Read a siqfile from a buffer.
 *
 * Compiled empty unless we have siqfile support built in.  
 *
 * For example, we read the siqfile into a buffer over USB then call this
 * function.
 *
 * \author David Poole
 * \date 23-Dec-2005
 *
 */

void scanvar_parse_file( char *buf, int buflen, int bufmaxlen )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
#ifdef HAVE_SCANHIQ_SUPPORT
    siqdata_add_siqfile( buf, buflen );
#endif
}

/**
 * \brief  Create a fully populated scanvar from a siqfile parse.
 *
 * Originally created to unpickle a scanvar from a siqfile.
 *
 * See scanvar_to_siqfile() for the pickle operation.
 *
 * \author David Poole
 * \date 01-Nov-2012
 */

struct scanvar_constructor {
    /* turn a scanvar id into a constructor so we can build a default scanvar */
    int id;
    /* pointer to the scanvar constructor for a given id */
    struct scanvars * (*scanvar_constructor)(void);
};

static const struct scanvar_constructor scanvar_constructor_list[] = {
    { SVCID_DRAFT_COPY, scanplat_sv_draft_copy_new },
    { SVCID_TEXT_COPY, scanplat_sv_text_copy_new },
    { SVCID_PICTURE_COPY, scanplat_sv_picture_copy_new },
    { SVCID_FILMPHOTO_COPY, scanplat_sv_filmphoto_copy_new },
    { SVCID_MIXED_COPY, scanplat_sv_mixed_copy_new },

    { SVCID_MONO_SCAN, scanplat_sv_mono_scan_new },
    { SVCID_COLOR_SCAN, scanplat_sv_color_scan_new },
#if SDH_FAX_TUNING
	{ SVCID_TEXT_FAX, scanplat_sv_text_fax_new },
    { SVCID_MIXED_FAX, scanplat_sv_mixed_fax_new },
#endif
    { SVCID_ZERO, NULL },
};

scan_err_t scanvar_from_siqfile( struct scanvars **psv, char *buf, int buflen )
{
    scan_err_t scerr, final_scerr;
    int i;
    struct scanvars *sv=NULL;
    const struct siqdata *sd=NULL;
    struct siqdata_table *siqdata=NULL;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    dbg2( "%s\n", __FUNCTION__ );

    /* beware of memory leaks */
    XASSERT( *psv==NULL, (*psv)->id );

    /* allocate memory for the parser to hold the symbols in the siqfile */
    siqdata = siqdata_table_new();
    if( !siqdata ) {
        return SCANERR_OUT_OF_MEMORY;
    }

    /* parse the siqfile into the data table */
    scerr = siqdata_parse_siqfile( buf, buflen, siqdata );
    if( scerr != SCANERR_NONE ) {
        siqdata_table_free( &siqdata );
        return scerr;
    }

    dbg2( "%s id=%d\n", __FUNCTION__, siqdata->id );

    for( i=0 ; ; i++ ) {
        if( scanvar_constructor_list[i].id==SVCID_ZERO ) {
            dbg1( "%s no constructor for id=%d\n", __FUNCTION__, siqdata->id );
            final_scerr = SCANERR_INVALID_PARAM;
            goto fail;
        }

        if( siqdata->id==scanvar_constructor_list[i].id ) {
            sv = scanvar_constructor_list[i].scanvar_constructor();
            break;
        }
    }
    if( !sv ) {
        final_scerr = SCANERR_OUT_OF_MEMORY;
        goto fail;
    }

    /* pull the siqdata into the scanvar */
    for( i=0 ; i<siqdata->num_entries ; i++ ) {
        sd = (const struct siqdata *)&siqdata->data[i];
        scanvar_siq_merge_data( sd, sv );
    }

    /* There are some fields we want to merge but we don't want in the global
     * siqdata table. (This is a bit weird.)
     */
    scerr = scanvar_siq_merge_all_user_settings( siqdata, sv );
    if( scerr != SCANERR_NONE ) {
        final_scerr = scerr;
        goto fail;
    }

    siqdata_table_free( &siqdata );

    /* return the scanvar we just built */
    PTR_ASSIGN( *psv, sv );

    return SCANERR_NONE;

fail:
    if( siqdata ) {
        siqdata_table_free( &siqdata );
    }
    if( sv ) {
        scanvar_delete( &sv );
    }

    return final_scerr;
}

/**
 * \brief print a scanvar in a format suitable to be read as a siqfile 
 *
 * Useful to dump one scanvar, fiddle the variables, then feed back in. Also
 * great for debugging so can see all of a particular scanvar in an easily
 * readable format.
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 */

void scanvar_to_siqfile( const struct scanvars *sv )
{
	sv_dbg2("[SV] %s\n", __FUNCTION__);
#ifdef HAVE_SCANHIQ_SUPPORT
    scanvar_siq_print( sv );
#endif
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 25-Jun-2006
 *
 */

const char *
scanvar_name_lookup( int classid )
{
    int i;
	sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* linear search the list of human class names to find the human name for
     * the classid number
     */
    for( i=0 ; ; i++ ) {

        if( scanvar_siq_names[i].len==0 ) {
            /* end of list */
            break;
        }
        if( scanvar_siq_names[i].key == classid ) {
            return scanvar_siq_names[i].str;
        }
    }

    return "(no name)";
}

/**
 * \brief  Convert a human name (e.g., "Color Scan Host") into an internal
 * classid. 
 *
 * \author David Poole
 * \date 22-Dec-2005
 *
 * Don't call this except for siqfile support!
 *
 */

scan_err_t 
scanvar_classid_lookup( const char *name, int namelen, int *id )
{
    int i;

    *id = 0;
	//sv_dbg2("[SV] %s\n", __FUNCTION__);
    /* should only be called from siqdata.c */

    /* linear search the list of human class names to convert to an internal
     * use classid number
     */
    for( i=0 ; ; i++ ) {

        if( scanvar_siq_names[i].len==0 ) {
            /* end of list */
            break;
        }

        dbg2( "%s %d==%d && %s==%s?\n", __FUNCTION__,
            namelen, scanvar_siq_names[i].len,
            scanvar_siq_names[i].str, name );

        if( namelen==scanvar_siq_names[i].len 
                && strncasecmp( scanvar_siq_names[i].str, name, namelen )==0 ) {

            /* found it */
            *id = scanvar_siq_names[i].key;
            return 0;
        }
    }

    return SCANERR_NO_ENTRY;
}

#ifndef HAVE_SCANHIQ_SUPPORT

/* Some empty siqfile functions so we can build without siqfile support.
 * Actual functions are in the scanvarsiq.c source file. 
 */

void
scanvar_siq_merge( struct scanvars *sv )
{
    /* nothing */
}

bool 
scanvar_siq_has_key( struct scanvars *sv, int key )
{
    return false;
}

#endif

