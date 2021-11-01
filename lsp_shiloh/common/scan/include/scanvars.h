/**
 * \file scanvars.h
 *
 * \brief  Scanvars tells Scantask how a user wants a scan performed. 
 *
 * Scanvar contains all the information about the user's scan, in terms of what
 * the user wants. For example, scanvar contains the DPI the user expects, e.g,
 * 800 DPI.  If the hardware doesn't support 800 DPI, the lower layers translate
 * that into a supported horizontal and vertical DPI (in this case, 1200 DPI).
 * The scanvar will still say 800 DPI.
 *
 * Another example is the scan area. The scanvar area is the area the user
 * wanted but for various buffer management and hardware reasons, the device may
 * have to scan a slightly larger area. But the original is still in scanvars.
 * 
 * There's a fine, fuzzy line about what goes into scanvars and what doesn't.
 *
 *   - all of PIE and HalfPack goes into scanvars.
 *   - all Image/Copy Quality settings go into scanvars (most of those are in 
 *     PIE/HalfPack anyway)
 *   - all the user's settings (color/mono, DPI, area, scaling) go into scanvars
 *
 * Scanvars contains the "holistic" view of the scan. Scanvars describes the
 * final result of the scan. The low level, nitty-gritty details of actually
 * implementing the scan (e.g., how fast to run the motor, how much gain to put
 * into the AFE) lie elsewhere.
 *
 * By process of elimination, what is left? PIC, with only very few exceptions,
 * belongs entirely to calibration. The Scan block separation is, as of this
 * writing, still under reconstruction (sensor/afe control, motor control, scan
 * cmdq) but definitely doesn't belong to any of the above categories.
 *
 * Note there are some calibration variables in scanvars. The calibration
 * white/black points are actually IQ settings and so belong in scanvars. 
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 **/
 
#ifndef SCANVAR_H
#define SCANVAR_H

/** \brief The scan area.  Upper left of the page is coordinate 0,0.
 *
 * This structure is unitless.  Units are imposed by the variable
 * implementation.  For example, below we have user_area_hinch which is the
 * user's desired scan area measured in 1/100 inches.
 *
 * Note these are deliberately declared as signed integers. We've found use in
 * the past of moving the scan area negative. For example, when scaling copy
 * data down far enough, we have to move the user margins out beyond the platen
 * in order to keep the image centered.
 *
 * Legitimate uses of negative values are rare and will mostly cause the
 * firmware to choke and die in horrible, unspeakable ways.
 */
struct scan_area {
    /* 
     * note: signed int! 
     */

    /** Upper Left coordinate of scan area; measured from the left of origin. */
    long int x;

    /** Upper Left coordinate of scan area; measured from the top of the
     * origin. 
     */
    long int y;

    /** Width of the scan area. Measured as a distance from X. */
    long int width;

    /** Height of the scan area. Measured as a distance from Y. */
    long int height;
};

/** \brief The scan scale.  Scales are measured in integer fractions. 
 *
 * For example, a valid scale might be 1/2, 1/3, 8/1, 876/3450, 22/7, 355/113, etc.
 *
 * The maximum value for any of these integers is a [0..32767]  Negative
 * values are not allowed. 
 */
struct scan_scale {
    int x_numer, x_denom, y_numer, y_denom;
};

/* davep 12-Oct-2011 ; adding full page operators, stuff that requires a full
 * page in memory (e.g., rotate180 for duplex adf or histogram equalization)
 *
 * I'm using a free-form string so I can add/remove page operators without
 * breaking a lot of existing code. The string values are used to set up the
 * scan_pager.
 */
#define SCANVAR_PAGE_OP_NAME_MAX 32
#define SCANVAR_PAGE_OP_MAX 12
struct scanvar_page_op {
    char name[SCANVAR_PAGE_OP_NAME_MAX+1];
    scan_page_range page_range;
};

#define SCANVAR_MAX_SCALE_TERM 32767

/** \brief Dynamically allocate array of uint32_t 
 *
 * Encapsulate a malloc'd array with its size.
 */
struct uint32_array {
    uint32_t *data;
    int num_entries;
};

/* array indices for digital targets and clip points, etc */
#define SV_COLOR_RED   0
#define SV_COLOR_GREEN 1
#define SV_COLOR_BLUE  2
#define SV_COLOR_MONO  0
#define SV_COLOR_NUM_ENTRIES  3

/* possible values for use_sw_smooth in scanvars */
#define SV_SW_FILTER_DISABLED   0  /**< filter is disabled */
#define SV_SW_FILTER_AFTER_PIC  1  /**< filter data after PIC; not yet implemented in icefilter.c */
#define SV_SW_FILTER_AFTER_PIE  2  /**< filter data after PIE */

/* davep 20-Dec-2011 ; max number of coefficients we can program into
 * multifilters custom coefficient array (could be less; depends on the asic)
 */
#define MF_COEF_MAX_COEFFICIENTS 15

/* davep 01-Oct-2006 ; (This wasn't documented back when this structure and
 * these settings were created to I'm doing it now.) Copy "brightness" means
 * something different than Scan "brightness".  Copy brightness is the front
 * panel "lightness/darkness" setting and hits all three of gamma, contrast,
 * and image brightness. 
 *
 * We don't really use copy gamma or copy contrast externally. Only brightness.
 */
struct copy_brightness_settings {
    int copy_brightness; /* used as a key for tables */
    int gamma_times_10;
    int white_clip_point;
    int black_clip_point;

    /* TODO add smooth/sharpen */
};

/* SDH Copy BW Offset Setting */
struct copy_offset_settings {	// mod.lsh.for SDH tuning
    uint32_t id; /* used as a key for tables */
    uint32_t white_offset;
    uint32_t black_offset;
    uint32_t white_guard;
    uint32_t black_guard;

    /* TODO add smooth/sharpen */
};
/** scanvar can be passed in a message so we want to be able to pointer verify
 * on receive
 */
#define SCANVAR_COOKIE 0x82DEC2E6 

/** 
 * \brief  Scanvars tells Scantask how a user wants a scan performed. 
 */
struct scanvars {
    /** scanvar can be passed in a message so we want to be able to pointer
     * verify on receive
     */
    uint32_t cookie;

    /** If you write code using this id, you're uncool and I'll make fun of you. */
    int id;

    /* 
     * General Settings 
     */

    /* This is the user dpi, not the hardware dpi. This dpi can be any weird
     * number [75,9600]. This user dpi is converted into a hardware dpi and
     * stored in hw_dpi_vert, hw_dpi_horiz.
     *
     * TL;DR if you're talking to hardware, don't use this field.
     */
    uint32_t dpi;

    scan_cmode_t cmode;

    /** area user wants to scan meansured in 1/100" */
    struct scan_area user_area_hinch;

    /* davep 04-Jun-2009 ; adding manual document source selection */
    scan_document_source_t doc_src;

    /* PIE XYScaler scale; this is the user's scale setting (might not be the
     * actual scale after we've scaled to support a weird DPI)
     */
    struct scan_scale scale;

    bool use_gamma;
    /** gamma multiplied by 10 so we can store as in integer */
    uint32_t gamma_times_10; // valid values are 10-100

    /* davep 19-Oct-2011 ; New contrast equation.
     * color: floating point number * 100. Multiplied by the CbCr channels. 
     *
     * In float:     
     *      :    0  -> gray
     *      : (0,1) -> reduce contrast
     *      :    1  -> no contrast change
     *      : (1,N) -> go nuts but (1,2) gives nicest results
     *
     * mono: TBD
     */
    uint32_t contrast; 

    /* 'brightness' is the image brightness */
    /* davep 19-Oct-2011 ; New brightness equation. 
     *
     * In float:     
     *      :    0  -> darkest
     *      : (0,1) -> reduce brightness
     *      :    1  -> no brightness change
     *      : (1,N) -> go nuts but (1,2) gives nicest results
     */
    uint32_t brightness; 

    /* copy_brightness is an index into a table in scanvars.c.  The table
     * contains all the IQ settings for the 11 copy modes (black/white clips,
     * gamma, contrast, sharp/smooth, etc). For use by control panel code,
     * mostly.
     */
    uint8_t copy_brightness; // valid values are 1-11 (index into scanvar.c table)

    uint32_t sharpness; // TBD
    
    /** (Bug 13964) if non-null, scantask will send data, etc, messages here
     * when we're done with it (formerly "use_scanman") 
     */
    scan_callback_fn_t scan_callback_fn;

    /** send data through the software filter */
    int sw_filter_position;

    /** support for backwards sensors; we have to reverse the row data in
     * firmware (slow!)
     */
    bool sensor_flip_horizontal;

    /* davep 08-Dec-2010 ; support users' margins in firmware (instead of
     * entirely in PIC). Adding this feature to support whacky sensors. Note
     * PIC margins will still be used to clip left/right dead sensor pixels.
     */
    bool use_fw_margins;

    /*
     * Calibration
     *
     * Most of calibration actually belongs to cal.c but some image settings
     * have to be tuned for the scan target (text copy, color scan to host,
     * etc).
     *
     * These are indexed using the SV_TARGET_xxx defines
     */
    bool use_cal_white_digital_target;
    bool use_cal_black_digital_target;
    bool use_cal_white_analog_target;
    bool use_cal_black_analog_target;
    uint16_t cal_white_digital_target[ SV_COLOR_NUM_ENTRIES ];
    uint16_t cal_black_digital_target[ SV_COLOR_NUM_ENTRIES ];
    uint16_t cal_white_analog_target[ SV_COLOR_NUM_ENTRIES ];
    uint16_t cal_black_analog_target[ SV_COLOR_NUM_ENTRIES ];

    bool use_white_clip_point;
    bool use_black_clip_point;
    uint16_t white_clip_point[ SV_COLOR_NUM_ENTRIES ];
    uint16_t black_clip_point[ SV_COLOR_NUM_ENTRIES ];

    scanvar_cal_t cal_mode;

    /* davep 12-Oct-2006 ; Adding cal-to-host. Cal-to-host lets us set up
     * calibration for a specific type of scan (e.g., mono 300 dpi) but instead
     * of doing the actual scan, we send the calibration data to the host
     * instead.
     */
    bool capture_cal;

    /* davep 01-Feb-2007 ; adding ability to tweak/tune calibration (afe
     * gain/offset, leds) from host; made them all uint16_t so I can reuse a lot
     * of the same siqfile code as the analog targets
     */
    bool use_cal_analog_offset;
    int cal_analog_offset[ SV_COLOR_NUM_ENTRIES ]; ///< analog offset

    bool use_cal_analog_gain;  
    uint16_t cal_analog_gain[ SV_COLOR_NUM_ENTRIES ];  ///<  analog gain; float stored as *100

    bool use_cal_led_pwm;
    uint16_t cal_led_pwm[ SV_COLOR_NUM_ENTRIES ]; ///< LED PWM; float stored as *100 

    bool use_cal_led_exp;
    uint16_t cal_led_exp[ SV_COLOR_NUM_ENTRIES ]; ///< LED exposure percentags; float stored as *100 

    /* davep 23-Feb-2009 ; adding cheap hack gray calibration */
    bool use_gray_cal;

    /*
     * PIC settings 
     */

    /** almost all PIC is owned by cal.c; however, we do need to know how PIC
     * sends us pixels
     */
    int pic_bpp;

    /* davep 31-Mar-2009 ; some ASIC's XYScale can't scale color so we have to
     * kludge it with PIC hscale
     */
    struct scan_scale pic_scale;
    bool use_pic_scale;

    /** davep 24-Dec-2008 ; ICE Lite PIE has no LUTs so we have to do image
     * enhacement in the PIC BDR LUT instead. Normally PIC entirely belongs to
     * calibration. However, if we're using PIC BDR for image enhancement, PIC
     * BDR belongs to scanvars. 
     */
    bool use_pic_bdr;

    /** enable using pic_bdr_lut */
    bool use_pic_bdr_lut;
    /** PIC BDR (Bit Depth Reduce) 10-bit to 8-bit lut */
    struct uint32_array pic_bdr_lut; 

    /* 
     * PIE settings 
     */
    bool use_pie;
    pie_pixel_t pieout; ///< PIE write dma output format 
    int pie_bpp; 

    /** e.g., ink needs row boundries at 96 pixels */
    int pie_row_align_bytes;  

    /** Force extra padding to make output fill page (i.e., copies will be in
     * upper left of page). PIE will pad to at least this value (might be more
     * due to DMA alignment).
     */
    uint32_t pie_pad_to_pixel_width;

    /** davep 11-July-06 ; since we pad the bejezus out of the input, truncate
     * output to this point
     */
    int pie_truncate_rows;

    /* davep 09-May-2013 ; if set, the number of rows per PIE WDMA buffer will
     * be at least this number (could be more).
     * Originally created for color copy which needed large buffers. 
     */
    int pie_min_rows_per_buffer;

    /* davep 30-Apr-2014 ; if set, the number of rows per PIE WDMA buffer will
     * be at MOST this number (could be less). Added because print kept hurting
     * itself.
     */
    int pie_max_rows_per_buffer;

    /* davep 25-May-2011; adding ColorShift, a new block to clean up CIS color
     * fringing (not available in all asics). 
     */
    bool use_colorshift;
    uint32_t cis_colorshift_round_mode; /* PIE_COLORSHIFT_CONFIG_ROUND_MODE_xxx */
    uint32_t cis_colorshift_mmode[3]; /* PIE_COLORSHIFT_MMODE_LINE_xxx in r,g,b order */
    uint32_t cis_colorshift_mult[3]; /* r,g,b order */

    /** enable csc (color space conversion) blocks */
    bool use_csc;

    /* bypass individual parts of CSC for debugging/testing */
    bool csc_rgb_srgb_bypass;
    bool csc_srgb_ycc_bypass;
    bool csc_ycc_rgb_bypass;

    /** enable using csc_rgb_lut */
    bool use_csc_rgb_lut;
    /** CSC RGB->esRGB 10-bit lut */
    struct uint32_array csc_rgb_lut; 

    /* davep 18-May-2012 ; feature request: red/green/blue planar shift
     * (increase/decrease red/green/blue tint directly). This value added
     * directly into each R/G/B CSC LUT. (Obviously only works with separate
     * LUTs)
     */
    int rgb_tint[SV_COLOR_NUM_ENTRIES];

    /* davep 29-Oct-2007 ; adding LUT for each plane (new asic feature) */
    bool use_csc_r_lut, use_csc_g_lut, use_csc_b_lut;
    struct uint32_array csc_r_lut; 
    struct uint32_array csc_g_lut; 
    struct uint32_array csc_b_lut; 

    /** CSC RGB->esRGB 3x3 */
    uint32_t csc_rgb_matrix[9]; 
    /** CSC RGB->YCC 3x3 */
    uint32_t csc_rgbycc_matrix[9]; 
    /** CSC YCC->RGB 3x3 */
    uint32_t csc_yccrgb_matrix[9]; 

    /** enable tcns */
    bool use_tcns;

    /** tcns YCbCr LUT */
    bool use_tcns_lut;
    struct uint32_array tcns_lut;    


    /** enable multifilter */
    bool use_mf; 

    /** enable the MultiFilter Activity Index LUT */
    bool use_mf_ai_lut;
    struct uint32_array mf_ai_lut;
    /** sculptor config */
    int mf_y_radius;    /* PIE_FILTER_SCR1_Y_xx */
    int mf_cbcr_radius; /* PIE_FILTER_SCR1_R_xx */
    int mf_scale_10;    /* PIE_FILTER_SCR1_S_xx */
    int mf_scale_15;    /* PIE_FILTER_SCR1_M_xx */
    int mf_scale_20;    /* PIE_FILTER_SCR1_P_xx */
    int mf_lut_output_scale;    /* PIE_FILTER_SCR1_N_xx */
    int mf_offset;      /* just the numerical offset */

    bool use_mf_erosion;
    bool use_mf_dilation;
    bool use_mf_varcoefficients;

    /* variable coefficients: can have a 5x5 or 5x9 depending on which ASIC
     * we're on.
     */
    uint32_t mf_varcoef_y[MF_COEF_MAX_COEFFICIENTS]; 
    uint32_t mf_varcoef_chroma[MF_COEF_MAX_COEFFICIENTS]; 
    uint32_t mf_varcoef_y_shift;
    uint32_t mf_varcoef_y_sf;
    uint32_t mf_varcoef_chroma_shift;
    uint32_t mf_varcoef_chroma_sf;

    /** enable MultiFilter's text enhance feature */
    bool use_mf_te;

    /** text enhance params */
    uint32_t te_count_black_min;
    uint32_t te_count_white_min;
    uint32_t te_count_white_plus_black_min;
    uint32_t te_center_color_max;
    uint32_t te_intense_color_max;
    uint32_t te_black_y_max;
    uint32_t te_black_CbCr_max;
    uint32_t te_white_y_min;
    uint32_t te_white_CbCr_max;
    bool   te_use_5x5_matrix;

    /** scaler */
    int xy_scaler_mode;
    

    /*
     * HalfPack settings 
     */

    /** enable HalfPack */
    bool use_hp;

    /** bits per pixel; should be 8, 4, or 2 */
    int hp_bpp;

    bool use_hp_horiz_rep;  /**< Horizontal replication */

    bool use_hp_size_lut;
    bool use_hp_sbias_lut;
    bool use_hp_density_lut;
    bool use_hp_diffus_lut;
    struct uint32_array hp_size_lut;
    struct uint32_array hp_sbias_lut;
    struct uint32_array hp_density_lut;
    struct uint32_array hp_diffus_lut;

    bool hp_alg_invert_data;  /**< HalfPack Algorithm register, Data Inversion flag */
    bool hp_alg_accum_err; /**< HalfPack Algorithm register, Accum Error flag */
    bool hp_alg_sbias_idx; /**< HalfPack Algorithm register, Signed Bias (Index) flag */
    bool hp_alg_sbias_dot; /**< HalfPack Algorithm register, Signed Bias (DoADot) flag */
    bool hp_alg_doadot;  /**< HalfPack Algorithm register, DoADot Override */

    bool use_hp_alg_shift; 
    uint8_t hp_alg_shift; /**< HalfPack Algorithm register, 2-bit shift field */

    uint8_t hp_alg_errweight1; /**< HalfPack Algorithm register, error weight 1 */
    uint8_t hp_alg_errweight2; /**< HalfPack Algorithm register, error weight 2 */
    uint8_t hp_alg_errweight3; /**< HalfPack Algorithm register, error weight 3 */

    uint8_t hp_bias_sram_cfg; /**< signed bias sram config: 1=64x32x1, 0=16x16x8 */
    uint8_t hp_bias_reg0; /**< signed bias programmable bias value 0 */
    uint8_t hp_bias_reg1; /**< signed bias programmable bias value 1 */

    /* davep 01-Oct-2006 ; break up color adjustments into class methods */
    /* BIG FAT NOTE! These function pointers could be NULL! Don't call
     * except from scanvars.c itself. 
     */
    void (*set_gamma_curve)( struct scanvars *sv );
    void (*set_contrast_curve)( struct scanvars *sv );
    void (*set_brightness_curve)( struct scanvars *sv );

    /* davep 18-Apr-2012 ; adding virtual method for the copy_brightness. Idea
     * is store some braindead simple default copy_brightness in scanvars
     * (core) code. Using this function ptr platform code can override the
     * table.  scanvar_set_copy_brightness() will call this function to get the
     * copy settings. If this function is NULL, scanvar_set_copy_brightness()
     * will use the defaults in scanvars.c.
     *
     * Example:
     *
     * static scan_err_t my_get_copy_brightness_settings(struct copy_brightness_settings *settings)
     * {
     *     # copyapp calls scanvar_set_copy_brightness().
     *     # scanvar_set_copy_brightness() will call this function. 
     *
     *     switch( settings->copy_brightness ) {
     *        # only showing one case here
     *        case 3 : 
     *            settings->gamma_times_10 = 22;
     *            settings->white_clip_point = 30;
     *            settings->black_clip-point = 200;
     *            break;
     *     }  
     *     return SCANERR_NONE;
     * }
     *
     * struct scanplat_sv_text_copy_new( void )
     * {
     *     struct scanvars *sv;
     *     sv = scanvars_text_copy_new();
     *     if( !sv ) return SCANERR_OUT_OF_MEMORY;
     *     sv->get_copy_brightness_settings = my_copy_brightness_settings;
     *
     *     # must call set_copy_brightness to force new settings into scanvar
     *     # (my_copy_brightness_settings() will be called from here)
     *     scanvar_set_copy_brightness( sv, 6 );
     *
     *     return sv;
     * }    
     *
     * Color copies' constructors would override get_copy_brightness settings
     * to load color appropriate values, etc.
     */
    scan_err_t (*get_copy_brightness_settings)( struct copy_brightness_settings *settings );

    /**********************************************************************
     *
     * PRIVATE class members for internal use only ; don't mess with these
     * unless you're an idiot.
     */

    /* absolute scan area in pixels; measured in terms of absolute 0,0 (home
     * position); calculated from the user_area_hinches, dpi, and document notch
     * location
     */
    struct scan_area scan_area_pixels;

    /* hardware DPI ; scanvar.dpi is the user dpi and could be some whacky
     * number like 500 or 19200.  Using a scavar method that talks to the
     * hardware motor and sensor layers, we convert that user dpi to hardware
     * capable dpi.  The scan code should be using these DPIs throughout.
     */
    uint32_t hw_dpi_horiz, hw_dpi_vert;
    
    /* davep 17-Dec-2010 ; actual scale; this is the scale we calculate using
     * the user's scale factor and the scale to convert for a dpi. For example,
     * the user wants 75 dpi scaled 1/2 we'd actually scan at 300 dpi, scale
     * down 1/4 (to 75 dpi) then 1/2 again.  1/4 * 1/2 = 1/8 scale.
     */
    struct scan_scale pie_scale;

    /* davep 12-Oct-2011 ; adding full page operators (e.g., rotate180 for
     * duplex adf). Do not fiddle these directly. Use scanvar_add_page_op().
     * Values here are parsed and used to set up the scan_pager.
     */
    struct scanvar_page_op page_op_list[SCANVAR_PAGE_OP_MAX];
    
     /*
     * Background Remove	// add.lsh.for background removal
     */
    uint32_t bgr_sensitivity;
    
};

void scanvar_onetime_init( void );
void scanvar_push( struct scanvars *sv );
struct scanvars * scanvar_pop( void );
struct scanvars * scanvar_get( void );
const struct scanvars * scanvar_peek( void );
void scanvar_delete( struct scanvars **sv );
void scanvar_print( const struct scanvars *sv );
void scanvar_stackdump( void );
void scanvar_sanity( const struct scanvars *sv );

/* davep 31-Oct-2012 ; add clumsy mechanism to capture siqfile to string rather
 * than just squirting out serial port. Requires stdarg.h to be included ahead
 * of this header so the #ifdef allows old code to still build.
 */
#ifdef USE_SIQFILE_PRINT_FN
typedef void (*siqfile_print_fn)(const char *, va_list );
siqfile_print_fn set_siqfile_print_fn( siqfile_print_fn new_siqfile_print_fn );
//siqfile_print_fn set_siqfile_print_fn( void (*new_siqfile_print)(const char *fmt, va_list ap ) );
#endif

/* davep 01-Jan-2006 ; utility functions for uint32_array */
scan_err_t uint32_array_new( struct uint32_array *new_array, int num_entries );
void uint32_array_delete( struct uint32_array *array );
scan_err_t uint32_array_copy( struct uint32_array *dst, const uint32_t *src, int num_entries);
scan_err_t uint32_array_clone( struct uint32_array *dst, const struct uint32_array *src );

struct scanvars * scanvar_new( void );

struct scanvars * scanvar_mono_cal_new( void );
struct scanvars * scanvar_color_cal_new( void );
struct scanvars * scanvar_cal_newcopy( const struct scanvars *src_sv );

struct scanvars * scanvar_draft_copy_new( void );
struct scanvars * scanvar_text_copy_new( void );
struct scanvars * scanvar_picture_copy_new( void );
struct scanvars * scanvar_filmphoto_copy_new( void );
struct scanvars * scanvar_mixed_copy_new( void );

struct scanvars * scanvar_draft_color_copy_new( void );
struct scanvars * scanvar_text_color_copy_new( void );
struct scanvars * scanvar_picture_color_copy_new( void );
struct scanvars * scanvar_filmphoto_color_copy_new( void );
struct scanvars * scanvar_mixed_color_copy_new( void );

struct scanvars * scanvar_mono_scan_new( void );
struct scanvars * scanvar_color_scan_new( void );

struct scanvars * scanvar_text_fax_new( void );
struct scanvars * scanvar_mixed_fax_new( void );

/* davep 18-Apr-2008 ; moving the leaf nodes of the scanvar class hierarchy into
 * platform specific code so we can move all the platform tuning (sensor
 * frequence, IQ, HalfPack, etc etc etc etc etc into platform specific code.
 */
struct scanvars *scanplat_sv_draft_copy_new( void );
struct scanvars *scanplat_sv_text_copy_new( void );
struct scanvars *scanplat_sv_mixed_copy_new( void );
struct scanvars *scanplat_sv_filmphoto_copy_new( void );
struct scanvars *scanplat_sv_picture_copy_new( void );

struct scanvars *scanplat_sv_draft_color_copy_new( void );
struct scanvars *scanplat_sv_text_color_copy_new( void );
struct scanvars *scanplat_sv_mixed_color_copy_new( void );
struct scanvars *scanplat_sv_filmphoto_color_copy_new( void );
struct scanvars *scanplat_sv_picture_color_copy_new( void );

/* davep 02-Dec-2011 ; mono_copy and color_copy deprecated as of this date; use
 * the "xxx_color_copy_new() functions instead 
 */
//struct scanvars *scanplat_sv_mono_copy_new( void );
//struct scanvars *scanplat_sv_color_copy_new( void );

struct scanvars *scanplat_sv_mono_scan_new( void );
struct scanvars *scanplat_sv_color_scan_new( void );
struct scanvars *scanplat_sv_mono_cal_new( void );
struct scanvars *scanplat_sv_color_cal_new( void );
struct scanvars *scanplat_sv_text_fax_new( void );
struct scanvars *scanplat_sv_mixed_fax_new( void );

scan_err_t scanvar_set_black_white_clip( struct scanvars *sv, uint16_t black_clip[3], uint16_t white_clip[3] );
scan_err_t scanvar_set_all_black_white_clip( struct scanvars *sv, uint16_t black_clip_all, uint16_t white_clip_all );
scan_err_t scanvar_set_gamma( struct scanvars *sv, uint32_t gamma );
scan_err_t scanvar_set_copy_contrast( struct scanvars *sv, uint32_t copy_contrast );
scan_err_t scanvar_set_contrast( struct scanvars *sv, uint32_t contrast );
scan_err_t scanvar_set_brightness( struct scanvars *sv, uint32_t brightness );
scan_err_t scanvar_set_copy_brightness( struct scanvars *sv, uint8_t brightness );
// add.lsh.for background removal
scan_err_t scanvar_set_background( struct scanvars *sv, uint32_t sensitivity );
scan_err_t scanvar_set_sharpness( struct scanvars *sv, uint32_t sharpness );
scan_err_t scanvar_set_rgb_tint( struct scanvars *sv, int r_tint, int g_tint, int b_tint );
scan_err_t scanvar_set_dpi( struct scanvars *sv, uint32_t dpi );
scan_err_t scanvar_set_bpp( struct scanvars *sv, uint8_t bpp );
uint8_t scanvar_get_bpp( const struct scanvars *sv );
void scanvar_set_use_hp( struct scanvars *sv, bool enable );
void scanvar_set_use_pie( struct scanvars *sv, bool enable );
void scanvar_set_scan_callback( struct scanvars *sv, scan_callback_fn_t scan_callback_fn );
void scanvar_get_area( const struct scanvars *sv, long int *x, long int *y, long int *width, long int *height );
scan_err_t scanvar_set_area( struct scanvars *sv, int x, int y, int width, int height ); 
scan_err_t scanvar_reduce_and_verify_xyscale( int *x_numer, int *x_denom, 
                                       int *y_numer, int *y_denom );
scan_err_t scanvar_verify_xyscale( int x_numer, int x_denom, 
                                   int y_numer, int y_denom );
scan_err_t scanvar_set_xyscale( struct scanvars *sv, int x_numer, int x_denom, int y_numer, int y_denom );
void scanvar_get_xyscale( const struct scanvars *sv, int *x_numer, int *x_denom, int *y_numer, int *y_denom );
scan_cmode_t scanvar_get_cmode( void );
scan_err_t scanvar_set_pic_bpp( struct scanvars *sv, int pic_bpp );
scan_err_t scanvar_set_pic_scale( struct scanvars *sv, 
                            int x_numer, int x_denom );
int scanvar_get_pic_bpp( void );
void scanvar_set_pie_row_align( struct scanvars *sv, int align );
void scanvar_set_xy_scale_mode( struct scanvars *sv, int xy_scaler_mode );
void scanvar_set_capture_cal( struct scanvars *sv, bool flag );
void scanvar_disable_image_enhancements( struct scanvars *sv );
void scanvar_set_flip_horizontal( struct scanvars *sv, bool flag );
scan_err_t scanvar_set_document_source( struct scanvars *sv, 
                                    scan_document_source_t doc_src );
pie_pixel_t scanvar_get_pieout(void);
void scanvar_set_pieout( struct scanvars *sv, pie_pixel_t dtype);
void scanvar_get_hw_dpi( const struct scanvars *sv, uint32_t *horiz_dpi, uint32_t *vert_dpi );

scan_err_t scanvar_add_page_op( struct scanvars *sv, const char *opname, scan_page_range page_range );
scan_err_t scanvar_get_page_op( struct scanvars *sv, const char *opname, scan_page_range *page_range );

scan_data_type scanvar_get_final_data_type( const struct scanvars *sv );
const char *scanvar_get_scan_data_type_name( scan_data_type dtype );

scan_err_t scanvar_set_mf_ai_lut( struct scanvars *sv, const uint8_t *ai_lut_y, const uint8_t *ai_lut_cbcr );
scan_err_t scanvar_set_mf_coef_lut( struct scanvars *sv, 
                                     const uint32_t *varcoef_y, 
                                     const uint32_t *varcoef_chroma );

/* only the Scan IQ parser should call this function */
scan_err_t scanvar_classid_lookup( const char *name, int namelen, int *id );

/* If we have the ability to send Scan IQ info from the host, we include these
 * functions.
 */
#ifdef HAVE_SCANHIQ_SUPPORT
void scanvar_parse_file( char *buf, int buflen, int bufmaxlen );
const char * scanvar_name_lookup( int classid );
#endif

/* this function will be empty if we are building without siqfile support */
void scanvar_to_siqfile( const struct scanvars *sv );
scan_err_t scanvar_from_siqfile( struct scanvars **psv, char *buf, int buflen );

/* Don't use the following functions unless you really really really know what
 * you're doing. No, seriously. You'll shoot your eye out.
 *
 * They're intended for "private" use of scanlib.
 */
void scanvar_get_pixel_area( const struct scanvars *sv, struct scan_area *pixel_area );
void scanvar_set_pixel_area( struct scanvars *sv, struct scan_area *pixel_area );
void scanvar_calc_pixel_area( struct scanvars *sv );
void scanvar_set_cal_mode( struct scanvars *sv, scanvar_cal_t cal_mode);
void scanvar_get_pie_scale( const struct scanvars *sv, int *x_numer, int *x_denom, int *y_numer, int *y_denom );
// add.lsh.for background removal
void scanvar_set_csc_rgb_lut( uint32_t *src_lut, struct uint32_array *dst_lut, uint8_t BG_x );

#endif

