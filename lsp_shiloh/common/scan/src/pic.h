/**
 * \file pic.h
 *
 * \brief Header file for PIC interface functions (DMA, etc).
 */
/*
 * ============================================================================
 * Copyright (c) 2006 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef PIC_H
#define PIC_H

#ifdef HAVE_STAGGERED_SENSOR_SUPPORT
  #define PIC_WDMA_NUM_CHANNELS 6
#elif defined HAVE_NSENSOR_SUPPORT
  /* dual scan (two sensors, one each side of paper) */
  #define PIC_WDMA_NUM_CHANNELS 6
#else
  #define PIC_WDMA_NUM_CHANNELS 3
#endif

struct pic_interrupt_stats {
    uint32_t count;           /* increment every time we enter pic_interrupt() */
    uint32_t combus_overrun;  /* common block overrun */
    uint32_t adc_overrun;     /* ADC overrun */
    uint32_t dma[PIC_WDMA_NUM_CHANNELS];
    /* more? */
};

/* PIC's Pixel Correction block's PDCR1 register is very complex. This structure
 * wraps around the complexity and gives a one stop shop for setting the
 * register.
 *
 * The fields of this structure are designed to make it easier for someone who
 * knows what they're doing to get done what they want to get done. 
 */
struct pic_prnudsnu_config {

    /* valid values are 12, 16, 20, 24 */
    uint8_t total_bits;

    /* valid values are -2, -1, 0, +1, +2 */
    int8_t extra_prnu_bits[3];

    /* Valid values are one of PIC_PRNUDSNU_CONFIG_SCALE_PRNU_xxx. These fields in
     * PDCR1 reflect a floating point operation and don't map exactly to a C
     * value very well.
     */
    uint8_t scale_prnu_color[3];

    /* valid values are 0..11 which are <<0 .. <<11  (shift left) */
    uint8_t scale_dsnu_color[3];

    /* davep 11-Mar-2011 ; quadratic (not supported in all ASICs) */
    uint32_t quad_SF1, quad_SF2, quad_shift;

    /* davep 03-Mar-2011 ; finer grained prnu/dsnu (not supported in all ASICs) */
    uint32_t pmult, pscale, poffset;
    uint32_t dmult, dscale, doffset;
};

#define PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL4  0
#define PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL2  1
#define PIC_PRNUDSNU_CONFIG_SCALE_PRNU_NONE  2
/* davep 04-Apr-2008 ; the reset asic introduced a PRNU/DSNU max gain of shift
 * left three by stealing the config bits from the shift right 1.
 */
#ifdef HAVE_ICE_PRNUDSNU_MUL8
#define PIC_PRNUDSNU_CONFIG_SCALE_PRNU_MUL8  3
#else
#define PIC_PRNUDSNU_CONFIG_SCALE_PRNU_DIV2  3
#endif

/* pic_marg_calc_fit()'s fitting_strategy parameter */
typedef enum {
    PIC_MARG_PIXELFIT_TRUNCATE_RIGHT = 1,
    PIC_MARG_PIXELFIT_PAD_RIGHT
} pic_marg_fit_t;

/* davep 05-Mar-2013 ; This structure represents pixels-per-row flowing through
 * PIC. Not all ASICs have all features. The scanpipe function
 * pic_calc_pixels_per_row() calculates the PPR based on scan settings, current
 * mech's sensor chipgaps, and ASIC capabilities.
 */
struct pic_pixels_per_row {
    bool use_lrmargins0;
    bool use_chipgap;
    bool use_lrmargins1;
    bool use_hscale;

    uint32_t pic_pixels_per_row_in;

    uint32_t lrmargins0_ppr_in;
    uint32_t lrmargins0_ppr_out;

    uint32_t chipgap_ppr_in;
    uint32_t chipgap_ppr_out;

    uint32_t lrmargins1_ppr_in;
    uint32_t lrmargins1_ppr_out;

    uint32_t hscale_ppr_in;
    uint32_t hscale_ppr_out;

    uint32_t pic_pixels_per_row_out;
};

#define PIC_CHIPGAP_MAX_GAPS 16

struct scan_sensor_chipgap {
    uint32_t first_gap_offset;
    uint32_t gap_to_gap_width;

    uint8_t chipgap_list[PIC_CHIPGAP_MAX_GAPS];
    uint32_t num_chipgap_list;
};

/* function prototypes */
scan_err_t pic_onetime_init( void );
scan_err_t pic_cleanup_module( void );
scan_err_t pic_soft_setup( void );
void pic_reset( void );
scan_err_t pic_capture_interrupt( void );
void pic_release_interrupt( void );
void pic_setup_interrupt( void );
void pic_clear_interrupt_stats( void );

void pic_channel_setup( uint8_t channel );

/* functions for 2+ sensors */
uint32_t pic_prnudsnu_get_block( void );
uint32_t pic_prnudsnu_dma_get_block( void );
void pic_prnudsnu_set_block( uint32_t block );
void pic_prnudsnu_dma_set_block( uint32_t block );
scan_err_t pic_enable_dual_channel( bool enable );
bool pic_get_dual_channel_enabled( void );

/* PIC ADCNorm - Analog Digital Conversion Normalization interface */
void pic_adcnorm_set_bypass( bool bypass );
void pic_adcnorm_set_invert( bool enable );
void pic_adcnorm_set_left_justify( bool left_justify );
void pic_adcnorm_set_abits( uint8_t abits );
void pic_adcnorm_set_test_mode( bool enable );
void pic_adcnorm_ant( uint32_t adc_type, uint32_t adc_color, uint32_t adc_data );

/* hardware level functions for bulb monitor */
void pic_bm_set_bypass( bool bypass );
bool pic_bm_get_bypass( void );

void pic_bm_set_color_counter( bool not_odd_even );
//bool pic_bm_get_color_counter( void );

void pic_bm_set_lines_to_avg( unsigned int lines );
//unsigned int pic_bm_get_lines_to_avg( void );

void pic_bm_set_window_size( unsigned int numpix );
//unsigned int pic_bm_get_window_size( void );

void pic_bm_set_startpix( unsigned int startpix );
//unsigned int pic_bm_get_startpix( void );

//unsigned int pic_bm_get_sum_col0_mono_even( void );
//unsigned int pic_bm_get_sum_col1_odd( void );
//unsigned int pic_bm_get_sum_col2( void );
//unsigned int pic_bm_get_exposure_data( void );

void pic_bm_dump( void );
void pic_bm_reset( void );

/* hardware level functions for left/right margins */
void pic_marg_set_right( uint32_t right_pixels );
void pic_marg_set_left( uint32_t left );
uint32_t pic_marg_get_right( void );
uint32_t pic_marg_get_left( void );

void pic_marg_reset( void );
void pic_marg_dump( void );

scan_err_t pic_marg_calc_fit( uint32_t left_margin_pixels, 
                       uint32_t *right_margin_pixels,
                       int bits_per_pixel, 
                       pic_marg_fit_t fitting_strategy );

void pic_marg_set_bypass( bool bypass );
void pic_marg_set_bypass( bool bypass );
bool pic_marg_get_bypass( void );

/* davep 13-Apr-2012 ; moved these into pic_bs.c which isn't compiled into
 * mainline code (bulb shadow not used in firmware)
 */
#if 0
/* hardware level functions for bulb shadow config */
void pic_bs_set_right_min_gain_col0_mono_even( float gain );
float pic_bs_get_right_min_gain_col0_mono_even( void );
void pic_bs_set_left_max_gain_col0_mono_even( float gain );
float pic_bs_get_left_max_gain_col0_mono_even( void );
void pic_bs_set_right_min_gain_col1_odd( float gain );
float pic_bs_get_right_min_gain_col1_odd( void );
void pic_bs_set_left_max_gain_col1_odd( float gain );
float pic_bs_get_left_max_gain_col1_odd( void );
void pic_bs_set_right_min_gain_col2( float gain );
float pic_bs_get_right_min_gain_col2( void );
void pic_bs_set_left_max_gain_col2( float gain );
float pic_bs_get_left_max_gain_col2( void );

void pic_bs_set_left_slope_col0_mono_even( unsigned char lslope );
unsigned char pic_bs_get_left_slope_col0_mono_even( void );
void pic_bs_set_left_slope_col1_odd( unsigned char lslope );
unsigned char pic_bs_get_left_slope_col1_odd( void );
void pic_bs_set_left_slope_col2( unsigned char lslope );
unsigned char pic_bs_get_left_slope_col2( void );

void pic_bs_set_left_max_pixel_col0_mono_even( unsigned int lmaxpix );
unsigned int pic_bs_get_left_max_pixel_col0_mono_even( void );
void pic_bs_set_left_max_pixel_col1_odd( unsigned int lmaxpix );
unsigned int pic_bs_get_left_max_pixel_col1_odd( void );
void pic_bs_set_left_max_pixel_col2( unsigned int lmaxpix );
unsigned int pic_bs_get_left_max_pixel_col2( void );

void pic_bs_set_right_slope_col0_mono_even( unsigned char rslope );
unsigned char pic_bs_get_right_slope_col0_mono_even( void );
void pic_bs_set_right_slope_col1_odd( unsigned char rslope );
unsigned char pic_bs_get_right_slope_col1_odd( void );
void pic_bs_set_right_slope_col2( unsigned char rslope );
unsigned char pic_bs_get_right_slope_col2( void );

void pic_bs_set_right_start_pixel_col0_mono_even( unsigned int rmaxpix );
unsigned int pic_bs_get_right_start_pixel_col0_mono_even( void );
void pic_bs_set_right_start_pixel_col1_odd( unsigned int rmaxpix );
unsigned int pic_bs_get_right_start_pixel_col1_odd( void );
void pic_bs_set_right_start_pixel_col2( unsigned int rmaxpix );
unsigned int pic_bs_get_right_start_pixel_col2( void );
#endif

/* hardware level functions for PRNU/DSNU compensation */
void pic_prnudsnu_dma_dump( void );
void pic_prnudsnu_dump( void );
void pic_prnudsnu_set_num_correction_bits( unsigned int numbits );

typedef enum {
    /* note I'm using whacky numbers here to catch old code using the previous
     * float parameters
     */
    PIC_PRNU_MULTIPLIER_15=42, /* 1.5x */
    PIC_PRNU_MULTIPLIER_20=43, /* 2.0x */
    PIC_PRNU_MULTIPLIER_30=44, /* 3.0x */
    PIC_PRNU_MULTIPLIER_50=45, /* 5.0x */
} pic_prnu_multiplier_t;

void pic_prnudsnu_set_num_extra_prnu_bits_col0_mono_even( int numbits );
void pic_prnudsnu_set_max_prnu_mult_col0_mono_even( pic_prnu_multiplier_t multiplier );
void pic_prnudsnu_set_dsnu_mult_col0_mono_even( uint8_t multiplier );

void pic_prnudsnu_set_num_extra_prnu_bits_col1_odd( int numbits );
void pic_prnudsnu_set_max_prnu_mult_col1_odd( pic_prnu_multiplier_t multiplier );
void pic_prnudsnu_set_dsnu_mult_col1_odd( uint8_t multiplier );

void pic_prnudsnu_set_num_extra_prnu_bits_col2( int numbits );
void pic_prnudsnu_set_max_prnu_mult_col2( pic_prnu_multiplier_t multiplier );
void pic_prnudsnu_set_dsnu_mult_col2( uint8_t multiplier );

void pic_prnudsnu_set_config( struct pic_prnudsnu_config *p );
void pic_prnudsnu_encode_lut( int total_bits, int extra_prnu_bits,
                              int num_pixels,
                              uint16_t prnu[], uint16_t dsnu[], uint8_t quad[],
                              uint8_t prnudsnu_lut[],
                              int prnudsnu_lut_num_entries );

void pic_prnudsnu_set_color_counter( bool odd_even );
void pic_prnudsnu_set_bypass_all( bool bypass_all );
void pic_prnudsnu_set_bypass_prnu( bool bypass_prnu );
void pic_prnudsnu_set_bypass_dsnu( bool bypass_dsnu );
void pic_prnudsnu_set_bypass_bad_pix_replace( bool bypass_badpix );
void pic_prnudsnu_set_bypass_exp_comp( bool bypass_exp );

bool pic_prnudsnu_get_bypass_all( void );
bool pic_prnudsnu_get_bypass_prnu( void );

void pic_prnudsnu_enable_lut_dma( bool enable );
bool pic_prnudsnu_lut_dma_enabled( void );
void pic_prnudsnu_reset_lut_dma( void );
void pic_prnudsnu_enable_lut_dma_complete_int( bool enable );
bool pic_prnudsnu_lut_dma_complete_int_enabled( void );
#ifdef ICEDMA_H
void pic_prnudsnu_set_lut_addr( dma_addr_t lut_addr );
#endif
void pic_prnudsnu_set_lut_xfer_count( unsigned long count );
unsigned long pic_prnudsnu_get_lut_xfer_count( void );
void pic_prnudsnu_start_lut_dma( void );
bool pic_prnudsnu_lut_dma_active( void );
bool pic_prnudsnu_lut_dma_queue_avail( void );
bool pic_prnudsnu_lut_dma_complete( void );
void pic_prnudsnu_set_bypass_quadratic( bool bypass );
void pic_prnudsnu_set_quadratic_config( uint32_t scale_factor, uint32_t limit_factor, uint32_t shift_factor);
void pic_prnudsnu_set_prnu_muloff( uint32_t prnu_mult, uint8_t prnu_scale, uint32_t prnu_offset );
void pic_prnudsnu_set_dsnu_muloff( uint32_t dsnu_mult, uint8_t dsnu_scale, uint32_t dsnu_offset );
void pic_prnudsnu_set_coeffwidth( uint8_t coeffwidth );

/* hardware level functions for additional gain */
void pic_set_PonN( unsigned int PonN );
//unsigned int pic_get_PonN( void );
void pic_set_PonNorm( unsigned char PonNorm );
//unsigned char pic_get_PonNorm( void );

/* hardware level functions for hscale */
void pic_hs_set_bypass( bool bypass );
void pic_hs_set_hscale_factor( unsigned int sfactor );
//unsigned int pic_hs_get_hscale_factor( void );
uint32_t pic_hs_calc_factor( uint32_t pixels_in, uint32_t pixels_out );
void pic_hs_calc_row( int pixels_per_row_in, int x_numer, int x_denom, 
                      int bits_per_pixel, int *pixels_per_row_out );

/* hardware level functions for bit depth reduction */
void pic_bdr_reset( void );
void pic_bdr_set_bypass( bool bypass );
void pic_bdr_enable_fw_access( bool enable );
void pic_bdr_set_lut( uint32_t *lut );
#ifdef HAVE_THREE_BDR_LUT
void pic_bdr_set_num_luts( int num_luts );
void pic_bdr_select_lut( int lutsel );
#endif
void pic_bdr_dump( bool dump_lut );


/* 
 * 7-Apr-05; new functions compatible with generic ICE DMA driver 
 */

/* forward declaration of the buffer management structure */
struct ice_dma_buffer;

typedef enum { PIC_BITPACK_16BIT, PIC_BITPACK_8BIT } pic_bitpack_mode;

#ifdef ICEDMA_H
void pic_wdma_set_burst_size( ice_dma_burst_size burst_size );
#endif
void pic_wdma_set_bitpack_mode( pic_bitpack_mode mode );
pic_bitpack_mode pic_wdma_get_bitpack_mode( void );
void pic_wdma_init_routing( void );
void pic_wdma_enable_line_reverse( bool enable );
void pic_wdma_enable_line_reverse_by_channel( uint8_t channel, bool enable );
scan_err_t pic_wdma_set_bpp( int pic_bits_per_pixel );

/* hardware level functions for PIC write DMA */
void pic_wdma_reset( void );
void pic_wdma_channel_reset( uint8_t channel );
void pic_wdma_channel_enable( uint8_t channel );
bool pic_wdma_channel_is_enabled( uint8_t channel );
void pic_wdma_channel_disable( uint8_t channel );
void pic_wdma_channel_start( uint8_t channel );
void pic_wdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t rows, uint32_t bytes_per_row );
void pic_wdma_channel_status( uint8_t channel, 
                              uint32_t *cnt_status, 
                              uint32_t *addr_status);
void pic_wdma_channel_dump( uint8_t channel );
void pic_wdma_dump( void );

void pic_wdma_channel_launch( uint8_t channel );

/* software driver level functions for PIC write dma (icedma.c methods) */
void pic_wdma_init( void );

int pic_wdma_open( uint8_t channels[],
                   uint8_t num_channels,
                   int num_empties,
                   int total_rows,
                   int bytes_per_row );

scan_err_t pic_wdma_add_buffer( uint8_t channel, 
                                uint8_t *data, 
                                uint32_t datalen,
                                uint32_t rows, 
                                uint32_t bytes_per_row );

void pic_wdma_add_ready( uint8_t channel, 
                                struct ice_dma_buffer **addme );

//int pic_wdma_channel_num_readies(uint8_t channel);

void pic_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );

void pic_wdma_isr( uint8_t channel );

void pic_wdma_sanity( void );

void pic_wdma_debug_log( void );

void pic_wdma_close( void );
void pic_wdma_refill( int rows_per_buffer );
void pic_wdma_cancel( void );
void pic_wdma_run_forever( void );

/* testing code needs to be able to see the insides of picdma.c so we don't
 * have a ridiculous proliferation of small utility functions
 */
#ifdef PICDMA_FRIEND
struct ice_dma_mm *pic_wdma_get_mm( void );
#endif

void pic_dump( void );

int pic_wdma_setup_buffers( uint8_t channel, 
                            int num_buffers, 
                            int rows_per_buffer, 
                            int bytes_per_row );

/* called from pic_interrupt() to handle WDMA interrupts */
void pic_wdma_interrupt( void );

uint32_t pic_interrupt_disable( void );
void pic_interrupt_enable( void );

void pd_lut_swap32( uint8_t *lut, int lutsize_bytes );

struct scanvars; /* forward decl */
scan_err_t pic_calc_pixels_per_row( const struct scanvars *sv, 
                                    struct pic_pixels_per_row *p,
                                    uint32_t scan_pixels_per_row_in  );
#ifdef HAVE_PIC_PSESD
void pic_psesd_reset( void );
void pic_psesd_dump( void );
void pic_pgesd_interrupt();
void pic_esd_setup(int pix_per_row);
void pic_esd_color_reset(uint8_t color);
void pic_esd_enable( bool enable );
void pic_ps_go(bool tof_nbof);
void pic_ps_setup(bool start_bit, bool is_color, uint8_t chan_en, uint32_t res);
#endif

#ifdef HAVE_PIC_CHIPGAP
void pic_chipgap_reset( void );
void pic_chipgap_dump( void );
void pic_chipgap_set_bypass( bool bypass );
uint32_t pic_chipgap_sum_list( uint8_t gap_list[], int num_gap_list );
scan_err_t pic_chipgap_get_sensor_chipgap( uint32_t horiz_dpi, 
                                            struct scan_sensor_chipgap *sensor_gaps );
scan_err_t pic_chipgap_set_location( uint32_t first_gap_offset, uint32_t gap_to_gap_width );
scan_err_t pic_chipgap_set_gaps( uint8_t gap_list[], int num_gap_list ) ;
#else
static inline void pic_chipgap_reset( void ) {}
static inline void pic_chipgap_dump( void ) {}
static inline void pic_chipgap_set_bypass( bool bypass ) {}
static inline uint32_t pic_chipgap_sum_list( uint8_t gap_list[], int num_gap_list ) { return 0; }
static inline scan_err_t pic_chipgap_get_sensor_chipgap( uint32_t horiz_dpi, 
                                                    struct scan_sensor_chipgap *sensor_gaps )
                                                    { return SCANERR_NOT_IMPLEMENTED; }
static inline scan_err_t pic_chipgap_set_location( uint32_t first_gap_offset, uint32_t gap_to_gap_width )
                                                    { return SCANERR_NOT_IMPLEMENTED; }
static inline scan_err_t pic_chipgap_set_gaps( uint8_t gap_list[], int num_gap_list ) 
                                                    { return SCANERR_NOT_IMPLEMENTED; }
#endif

#endif

