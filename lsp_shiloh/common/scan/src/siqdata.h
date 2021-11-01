/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2004, 2005 Avago Technologies, Inc. All Rights Reserved
 *                      
 *                         Avago Confidential
 * ============================================================================
 *
 * Description:
 *
 * davep 29-Oct-2007 
 *  Renumbering because CSC nums ran out of space. I didn't put enough gaps
 *  between the blocks' numbers to begin with. Same on me.
 *
 *  Putting all of CSC into the 300 range because CSC is the 3rd block on the
 *  PIE chart (I'm so clever). Yes, yes, there are three CSC blocks (RGB2esRGB,
 *  RGB2YCC, YCC2RGB) so I took the first. Picky, picky.
 *
 *  TCNS is in the 600 range because it's the 6th block. HalfPack is 2000 because it's
 *  out of the way of the PIE numbers.
 *
 *  Stuff below the hundreds is purely firmware stuff and isn't directly plugged
 *  into a hardware block. For example, SIQ_GAMMA isn't in PIE itself but rather
 *  is used to create the a curve that will be plugged into RGB2esRGB (color) or
 *  TCNS (mono). 
 *
 *
 *****************************************************************************/

#ifndef SIQDATA_H
#define SIQDATA_H

/* SIQ (Scan Image Quality) key values */
#define SIQ_NAME         1 /**< Name of siqfile; used to connect to scanvars */

/* Calibration targets (3 values (color) or 1 (mono)) */
#define SIQ_CAL_WHITE_DIGITAL 10 /**< Calibration white digital target */
#define SIQ_CAL_BLACK_DIGITAL 11 /**< Calibration black digital target */
#define SIQ_CAL_WHITE_ANALOG  12 /**< Calibration white analog target */
#define SIQ_CAL_BLACK_ANALOG  13 /**< Calibration black analog target */
#define SIQ_CAL_ANALOG_OFFSET 14 /**< Calibration analog offset */
#define SIQ_CAL_ANALOG_GAIN   15 /**< Calibration analog gain */
#define SIQ_CAL_LED_PWM       16 /**< Calibration LED PWM (percetile; stored x100) */
#define SIQ_CAL_LED_EXP       17 /**< Calibration LED Exposure (percentile; stored x100) */
#define SIQ_CAL_USE_GRAY_ALG  18 /**< Calibration uses the gray cal algorithm */

/* white/black clip responsibility is probably going to move around between
 * different modules (for example, tcns/csc or perhaps bde/bdr)
 */
#define SIQ_WHITE_CLIP_POINT    20 /**< white clip point */
#define SIQ_BLACK_CLIP_POINT    21 /**< black clip point */

/* The following are settable by the host on a per-job basis. */

#define SIQ_GAMMA             22 /**< gamma*10 e.g., a gamma of 2.2 would come down as 22 */
#define SIQ_CONTRAST          23 /**< Contrast. Values from 1-11, with nominal = 6 */
#define SIQ_BRIGHTNESS        24 /**< Image brightness. Values from 1-11 with nominal = 6 */
#define SIQ_COPY_BRIGHTNESS   26 /**< Control panel brightness for copy. Values from 1-11, with nominal = 6 */
#define SIQ_SHARPNESS         27 /**< Sharpness. Values from 0-4, with 0 = no sharpening */
#define SIQ_RGB_TINT          28 /**< RGB tinting. Values from [-255,255] with 0 = no tinting (added to CSC LUT) */
// add.lsh.for background removal
#define SIQ_BACKGROUND        29 /**< Background Removal. Values from 1-9, with nominal = 5 */

#define SIQ_SW_FILTER    80 /**< Use the software filter (e.g., smoother),
                                 values: 0=disabled, 1=after PIC (not implemented), 2=after PIE */
#define SIQ_FW_MARGINS   90 /**< firmware margins for sensors PIC can't handle;
                              also used for chipgap correction */

/* PIC BDR (Bit Depth Reduction); use the range [110..119] */
#define SIQ_PIC_BDR_ENABLE 110 /**< PIC BDR enable/disable */
#define SIQ_PIC_BDR_LUT    111 /**< PIC BDR LUT */

/* CSC has three blocks; all three are in the 300 range */
#define SIQ_CSC_ENABLE   330 /**< CSC enable/disable */
#define SIQ_CSC_RGB_LUT  331 /**< CSC RGB2esRGB LUT */
#define SIQ_CSC_RGB_MATRIX  332 /**< CSC RGB2esRGB matrix */
#define SIQ_CSC_RGBYCC_MATRIX  333 /**< CSC YCC2RGB matrix */
#define SIQ_CSC_YCCRGB_MATRIX  334 /**< CSC RGB2YCC matrix */
#define SIQ_CSC_RGBSRGB_BYPASS 335 /**< bypass CSC RGB->sRGB */
#define SIQ_CSC_RGBYCC_BYPASS 336 /**< bypass CSC sRGB->YCC */
#define SIQ_CSC_YCCRGB_BYPASS 337 /**< bypass CSC YCC->RGB */
#define SIQ_CSC_R_LUT  338 /**< CSC RGB2esRGB LUT, Red channel */
#define SIQ_CSC_G_LUT  339 /**< CSC RGB2esRGB LUT, Green channel */
#define SIQ_CSC_B_LUT  340 /**< CSC RGB2esRGB LUT, Blue channel */

/* TODO add CPA here; 500 range */

/* TCNS ; 600 range */
#define SIQ_TCNS_ENABLE  640 /**< TCNS enable/disable */
#define SIQ_TCNS_LUT_Y   641 /**< TCNS lookup table, Y part, PIE_TCNS_LUT_SIZE entries */
#define SIQ_TCNS_LUT_CB  642 /**< TCNS lookup table, Cb part, PIE_TCNS_LUT_SIZE entries */
#define SIQ_TCNS_LUT_CR  643 /**< TCNS lookup table, Cr part, PIE_TCNS_LUT_SIZE entries */

/* davep 05-Nov-2012 ; add PIE CIS ColorShift */
#define SIQ_CIS_COLORSHIFT_ENABLE 675   /**< CIS Colorshift enable/disable */
#define SIQ_CIS_COLORSHIFT_ROUND_MODE 676  
#define SIQ_CIS_COLORSHIFT_MMODE 677  
#define SIQ_CIS_COLORSHIFT_MULT  678   

/* MultiFilter and its subblocks ; 700 range */
#define SIQ_MF_ENABLE    750 /**< MultiFilter enable/disable */
#define SIQ_MF_AI_LUT_Y  751 /**< MultiFilter Activity Index (AI) LUT, Y channel */
#define SIQ_MF_AI_LUT_CBCR 752 /**< MultiFilter Activity Index (AI) LUT, CbCr channel */
#define SIQ_MF_Y_RADIUS    753 /**< MultiFilter Sculptor AI Y radius */
#define SIQ_MF_CBCR_RADIUS 754   /**< MultiFilter Sculptor AI CbCr radius */
#define SIQ_MF_SCALE_10    755   /**< MultiFilter Sculptor AI scale 1.0 radius */
#define SIQ_MF_SCALE_15    756   /**< MultiFilter Sculptor AI scale 1.5 radius */
#define SIQ_MF_SCALE_20    757   /**< MultiFilter Sculptor AI scale 2.0 radius */
#define SIQ_MF_OUTPUT_SCALE 758  /**< MultiFilter Sculptor LUT output scale */
#define SIQ_MF_OFFSET      759   /**< MultiFilter Sculptor AI offset */
#define SIQ_MF_TE          760   /**< MultiFilter Text Enhance enable/disable */

/* davep 20-Dec-2011 ; adding variable filter coefficients, erosion, dilation */
#define SIQ_MF_ENABLE_EROSION 761
#define SIQ_MF_ENABLE_DILATION 762
#define SIQ_MF_ENABLE_VAR_COEFF 763
#define SIQ_MF_VAR_COEFF_Y 764
#define SIQ_MF_VAR_COEFF_Y_SF 765
#define SIQ_MF_VAR_COEFF_Y_SHIFT 766
#define SIQ_MF_VAR_COEFF_CHROMA 767
#define SIQ_MF_VAR_COEFF_CHROMA_SF 768
#define SIQ_MF_VAR_COEFF_CHROMA_SHIFT 769

/* davep 20-Dec-2011 ; move TE to 900 */
#define SIQ_TE_COUNT_BLACK_MIN             900 /**< MultiFilter Text Enhance min number of black pixels required in the matrix */
#define SIQ_TE_COUNT_WHITE_MIN             901 /**< MultiFilter Text Enhance min number of white pixels required in the matrix */
#define SIQ_TE_COUNT_WHITE_PLUS_BLACK_MIN  902 /**< MultiFilter Text Enhance min number of white+black pixels required in the matrix */
#define SIQ_TE_CENTER_COLOR_MAX            903 /**< MultiFilter Text Enhance max color value allowed for the matrix's center pixel
                                                   (e.g. to make sure it's not a halftone dot) */
#define SIQ_TE_INTENSE_COLOR_MAX           904 /**< MultiFilter Text Enhance max color value allowed before pixel is considered to
                                                   be intense (e.g. halftone dot) */
#define SIQ_TE_BLACK_Y_MAX                 905 /**< MultiFilter Text Enhance max value allowed for Y component for pixel to still
                                                   be considered black */
#define SIQ_TE_BLACK_CbCr_MAX              906 /**< MultiFilter Text Enhance max value allowed for color components for pixel to still
                                                   be considered black */
#define SIQ_TE_WHITE_Y_MIN                 907 /**< MultiFilter Text Enhance min value allowed for Y component for pixel to still
                                                   be considered white */
#define SIQ_TE_WHITE_CbCr_MAX              908 /**< MultiFilter Text Enhance max value allowed for color components for pixel to still
                                                   be considered white */
#define SIQ_TE_USE_5X5_MATRIX              909 /**< MultiFilter Text Enhance use 5x5 or 3x3 matrix */


/* HalfPack ; put in the 2000 range so out of everyone else's way */
#define SIQ_HP_BPP         2100 /**<  Bits per Pixel, values=(8,4,2) */
#define SIQ_HP_HORIZ_REP   2101 /**<  enable/disable horizontal replication */
#define SIQ_HP_SIZE_LUT    2102 /**<  Size LUT, 256x32 */
#define SIQ_HP_SBIAS_LUT   2103 /**<  Signed bias LUT, 256x32 */
#define SIQ_HP_DENSITY_LUT 2104 /**<  Density LUT, 256x32 */
#define SIQ_HP_DIFFUS_LUT  2105 /**<  Error Diffusion LUT, 256x32 */
#define SIQ_HP_ALG_INVERT_DATA 2106 /**<  Algorithm Switch - Invert Incoming Data */
#define SIQ_HP_ALG_ACCERR  2107 /**<  Algorithm Switch - AccumError Inclusion */
#define SIQ_HP_ALG_SBIAS_I 2108 /**<  Algorithm Switch - SignedBias Inclusion (Index) */
#define SIQ_HP_ALG_SBIAS_D 2109 /**<  Algorithm Switch - SignedBias Inclusion (DoADot) */
#define SIQ_HP_ALG_DOADOT  2110 /**<  Algorithm Switch - DoADot Override */
#define SIQ_HP_ALG_SHIFT   2111 /**<  Algorithm Shift Value */
#define SIQ_HP_ALG_ERRW1   2112 /**<  Algorithm error weight 1 */
#define SIQ_HP_ALG_ERRW2   2113 /**<  Algorithm error weight 2 */
#define SIQ_HP_ALG_ERRW3   2114 /**<  Algorithm error weight 3 */
#define SIQ_HP_BIAS_SRAM_CFG 2115 /**<  Bias register, sram config field */
#define SIQ_HP_BIAS_REG0   2116 /**<  Bias register, programmable bias value 0 */
#define SIQ_HP_BIAS_REG1   2117 /**<  Bias register, programmable bias value 1 */

/* davep 01-Nov-2012 ; settings that are for a single particular scan. I'm
 * using a siqfile to communicate scan settings from linux userspace to
 * kscantask. Must have all the user's settings in the siqfile as well.
 *
 * These values are not kept in the global siqdata cache which is used for IQ
 * tuning. These are only used for scanvar pickle/unpickle.
 */
#define SIQ_USER_BASE 6000
#define SIQ_USER_DPI   (SIQ_USER_BASE+1)
#define SIQ_USER_SCALE (SIQ_USER_BASE+2)
#define SIQ_USER_AREA  (SIQ_USER_BASE+3)
#define SIQ_CAPTURE_CAL (SIQ_USER_BASE+4)
#define SIQ_PIEOUT     (SIQ_USER_BASE+5)
#define SIQ_BPP        (SIQ_USER_BASE+6)
#define SIQ_DOC_SRC    (SIQ_USER_BASE+7)

struct siqdata_name {
    /** null terminated name of the token, e.g., "hp_size_lut" */
    const char *str;

    /** string length of the token */
    int len;

    /** key used internally, using the "SIQ_" defines */
    int key;
};

struct siqdata {
    /** key used internally, using the "SIQ_" defines */
    int key;

    /* not using a union here so I can always know when dynamic memory needs to
     * be free()'d
     */

    /** generic value */
    uint32_t num32;

    /** dynamically allocated array of 32-bit integers from statements like:
     * tcns = { 1,2,3,3,4,5,6 }
     * 
     * Eventually I could have other arrays in here too (e.g., 
     * "char *uint8_values" and still use max_values and num_values. Simply
     * choose one array to use per option type.
     */
    uint32_t *num32_values;

    /** null terminated human readable string; num_values acts as strlen */
    char *str;

    /** maximum size of array */
    int max_values;

    /** currently used size of array/string; array will be realloc'd when hits
     * max_values
     */
    int num_values;
};

#define MAX_HASH_ENTRIES 128

struct siqdata_table {
    /** id is used to link the data to a classid in scanvars */
    int id;

    /** array of values for this id */
    struct siqdata data[MAX_HASH_ENTRIES];

    /** max size of data[] 
       \todo TODO realloc data[] instead of using a static array */
    int max_entries;

    /** used size of data[] */
    int num_entries;
};


const struct siqdata_table * siqdata_lookup( int classid );
const char * siqdata_find_name_for_key( int key );
const struct siqdata_name * siqdata_get_first_name( void );
const struct siqdata_name * siqdata_get_next_name( const struct siqdata_name *name );

scan_err_t siqdata_parse_siqfile( char *buf, int buflen, struct siqdata_table *siqdata );
scan_err_t siqdata_add_siqfile( char *buf, int buflen );
int siqdata_find_key( const struct siqdata_table *siqdata, int key );

scan_err_t siqdata_read_from_net( void );

struct siqdata_table *siqdata_table_new( void );
void siqdata_table_free( struct siqdata_table **siqdata );

#endif

