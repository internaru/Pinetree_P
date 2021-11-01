/**
 * \file pie.h
 *
 * \brief Header file for PIE and HalfPack interface functions (DMA, etc).
 *
 * ============================================================================
 * Copyright (c) 2006-2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PIE_H
#define PIE_H

/* 6-Mar-05 ; these might more properly live in piehw.h but I need them here in
* pie.h for a lot of data structures.
*/
//#define PIE_RDMA_NUM_CHANNELS  6
//#define PIE_WDMA_NUM_CHANNELS  3

/* 13-Apr-05 ; PIE Mono HalfPack doesn't really have the concept of channels
 * so it only has one.
 */
#define PIEHP_RDMA_NUM_CHANNELS  1
#define PIEHP_WDMA_NUM_CHANNELS  1

typedef enum {
    HP_DMA_DIR_FORWARD,
    HP_DMA_DIR_REVERSE
} HP_DMA_DIR;

typedef enum {
    HP_BIAS_CONFIG_1,
    HP_BIAS_CONFIG_8
} HP_BIAS_CONFIG;

/* 26-July-05; Bit Depth Enhance (BDE) info */
typedef enum {
    PIE_BDE_MODE_MONO=1,
    PIE_BDE_MODE_COLOR
} pie_bde_mode;

/* The PIE DMA (both read and write) registers are actually a 2-deep
 * queue so we can load a next transaction during a current transaction. 
 */
#define PIE_DMA_REG_DEPTH   2

struct pie_interrupt_stats {
    uint32_t count;

    /* davep 18-Feb-2011 ; changed these arrays from using #def to a hard wired
     * big magic number. needed to move the PIE_[RW]_NUM_CHANNELS into piehw.h
     */
    /* PIE read/write DMA */
    uint32_t rdma[32]; /* +=1 for every interrupt on this channel */
    uint32_t wdma[32]; /* +=1 for every interrupt on this channel */

    uint32_t rdma_last_drr[32];
    uint32_t wdma_last_drr[32];

    /* Mono HalfPack read/write DMA */
    uint32_t hp_rdma;
    uint32_t hp_wdma;

    uint32_t hp_rdma_last_drr;
    uint32_t hp_wdma_last_drr;
};

/** Convenience structure holding row information. Used to balance the DMA
 * requirements of PIE and HalfPack.
 *
 *
 * See also pie_calc_row() and piehp_calc_row() 
 */

struct pie_dma_row {
    int pixels_in;
    int pixels_out;
    int bytes_in;
    int bytes_out;
};

/** convenience structure holding PIE XYScale scaling information. The scaler
 * values are passed as two fractions, an X scale and a Y scale. For example,
 * to scale down 50%, XYScale needs X=1/2 and Y=1/2.
 */
struct pie_scale {
    int x_numerator;
    int x_denominator;
    int y_numerator;
    int y_denominator;

    /* xyscaler hardware values */
    int x_scale;
    int y_scale;
};

/* davep 09-Jun-2011 ; centralizing a collection of hodge-podge strip size
 * calculations
 */
struct pie_strip {
    bool use_cs;
    bool use_mf;
    bool use_xy;

    int rdma_in; /* input dma strip size */

    int cs_in;   /* colorshift; eats two lines */
    int cs_out;

    int mf_in;   /* multifilter; eats two or four lines */
    int mf_out; 

    int xy_in;   /* very complicated */
    int xy_out;   

    int wdma_out; /* output dma strip size */

    int total_extra_rows; /* cs+mf */
};

/* Structure containing fields used to context switching PIE. */
struct pie_context {
    /* XYScale registers' save/restore. Note we only save/restore the Y
     * direction.
     *
     * "Interleaving of strips from different images will require the saving
     * and re-initialization of registers, but particularly the Vertical
     * Current / End Percentage should be stored and written back to the
     * Vertical Start Percentage Register in RAPR mode, and the Current Y
     * Transpose register value should be stored and written back to the Y
     * Transpose Register in linear mode."
     *
     * cyr  -> ytr
     * vcpr -> vspr
     *
     */
    uint32_t cyr;   /* Current Y Transpose:       0x...030 save -> 0x...028 restore */
    uint32_t cxr;   
    uint32_t yper;  /* Vertical Start Percentage: 0x...03c save -> 0x...034 restore */

};

/* function prototypes */

/* functions in pie.c */
void pie_reset( void );
void pie_block_reset( void );
scan_err_t pie_onetime_init( void );
scan_err_t pie_cleanup_module( void );
scan_err_t pie_soft_setup( void );
struct pie_interrupt_stats * get_pie_interrupt_stats( void );
void pie_clear_interrupt_stats( void );
void pie_interrupt_stats_dump( void );
int pie_get_bits_per_pixel( pie_pixel_t ppt );
int pie_get_bytes_per_pixel( pie_pixel_t ppt );
scan_err_t pie_calc_stripsize( struct pie_strip *strip, int fracY, int scaler_mode );
void pie_pixel_type_to_scan_data_type( pie_pixel_t ppt, scan_data_type *sdt );

void pie_dbg2_strip( struct pie_strip *strip );
void pie_dbg2_scale( struct pie_scale *scale );
void pie_dbg2_row( struct pie_dma_row *row );
void pie_dump( void );

void pie_get_context( struct pie_context *ctx );
void pie_set_context( struct pie_context *ctx );

/* Bit Depth Enhance (BDE) */
void pie_bde_reset( void );
void pie_bde_setup( pie_bde_mode mode, uint32_t lut[] );
void pie_bde_enable_fw_access( bool access );
void pie_bde_set_bypass( bool bypass );
void pie_bde_dump( void );

/* davep 06-Mar-2013 ; adding PIE CStats (Contone Statistics--histograms) */
/* number of uint32_t elements in the SRAM 'RAM[012]' fields */
#define PIE_CSTATS_SRAM_SIZE 128
/* number of uint32_t elements in the SRAM 'RAM3' field */
#define PIE_CSTATS_NONWHITE_SRAM_SIZE 32
typedef enum {
    PIE_CSTATS_HISTOGRAM_DISABLED=0,

    PIE_CSTATS_HISTOGRAM_3PLANE_1D=1,

    /* Plane1 as a 1-D and Planes 2-3 as a 2-D  */
    PIE_CSTATS_HISTOGRAM_PLANE1_1D_PLANE23_2D=2, 

    /* 3 planes as a 2-D */
    PIE_CSTATS_HISTOGRAM_3PLANE_2D=3,

} pie_cstats_histogram_t;

typedef enum {
    /* output of BDE (RGB) */
    PIE_CSTATS_INPUT_MUX_BDE = 0,

    /* output of esRGB2YCC (usually YCC) */
    PIE_CSTATS_INPUT_MUX_RGB2YCC = 1,
    
    /* input to PIE WDMA (RGB or YCC; usually RGB */
    PIE_CSTATS_INPUT_MUX_WDMA = 2

} pie_cstats_input_mux_t;

#ifdef HAVE_PIE_CSTATS
void pie_cstats_enable( bool enable );
void pie_cstats_reset( void );
void pie_cstats_dump( void );
void pie_cstats_set_sram( int sram_idx, uint32_t sram[], int sram_size );
void pie_cstats_get_sram( int sram_idx, uint32_t sram[], int sram_size );
void pie_cstats_enable_eoc_tag( bool enable );
void pie_cstats_set_cmode( scan_cmode_t cmode );
void pie_cstats_set_margins( uint32_t left, uint32_t right );
void pie_cstats_set_strip_height( uint32_t height_in_pixels );
void pie_cstats_set_strip_width( uint32_t width_in_pixels );
void pie_cstats_set_rgb_threshold( uint32_t value );
void pie_cstats_set_white_threshold( uint32_t value );
void pie_cstats_set_black_threshold( uint32_t value );
void pie_cstats_histogram( pie_cstats_histogram_t which );
void pie_cstats_set_input_mux( pie_cstats_input_mux_t which );
#else
#define pie_cstats_dump()
#define pie_cstats_reset()
#endif

/* Color Space Convert (CSC) */
void pie_csc_reset( void );
void pie_csc_setup( void );
void pie_csc_dump( void );
void pie_csc_rgb_set_lut( uint32_t *data, volatile uint32_t *lut );
bool pie_csc_has_unified_rgb_luts( void );
void pie_csc_rgb_enable_fw_access( bool access );
void pie_csc_set_bypass_all( bool bypass );

/* Color Plane Adjust (CPA) */
void pie_cpa_reset( void );
void pie_cpa_setup( uint32_t mode_bit, uint32_t lut[] );
void pie_cpa_set_bypass( bool bypass );

/* Tone Contrast and Neutral Sensitivity (TCNS) */
void pie_tcns_reset( void );
void pie_tcns_dump( void );
void pie_tcns_enable_fw_access( bool access );
void pie_tcns_set_bypass( bool bypass );
void pie_tcns_set_lut( uint32_t *lut );

/* MultiFilter (MF) */
typedef enum { PIE_MF_MONO=1, PIE_MF_COLOR } pie_mf_color;
typedef enum { PIE_MF_TE_MATRIX_3x3=1, PIE_MF_TE_MATRIX_5x5 } pie_mf_te_matrix;
void pie_filter_reset( void );
void pie_filter_dump( void );
void pie_filter_dump_lut( void );
void pie_filter_set_bypass_te( bool bypass );
void pie_filter_set_bypass( bool bypass );
void pie_filter_enable_fw_access( bool enable );
void pie_filter_set_lut( uint32_t *lut );
void pie_filter_set_monocolor( pie_mf_color monocolor );
void pie_filter_set_te_matrix( pie_mf_te_matrix te_matrix );
void pie_filter_set_pogo( int num_rows, int pixels_per_row );
void pie_filter_setup( int stripsize, int pixels_per_row );

/* davep 01-Apr-2013 ; PIE variable coefficient fields are all 17 bits max */
#define PIE_SCULPTOR_VARCOEF_MASK 0x1ff

/* XYScale */
#define PIE_XY_SCALER_BYPASS 0
#define PIE_XY_SCALER_RAPR   1
#define PIE_XY_SCALER_LINEAR 2

/* smallest our current HW can support is 1/16. Largest is 8/1 */
#define PIE_XY_SMALLEST_SCALE  0.0625
#define PIE_XY_LARGEST_SCALE    8

void pie_xy_reset( void );
void pie_xy_dump( void );
void pie_xy_set_bypass( bool bypass );
void pie_xy_calc_row( int pixels_per_row, 
                      uint32_t pixel_size, 
                      pie_pixel_t pie_output_type,
                      struct pie_scale *scale,
                      int extra_pad,
                      int scaler_mode,
                      struct pie_dma_row *row );
void pie_xy_scale(  struct pie_scale *scale,
               int pixels_in,
               int strip_size_in,
               int total_rows_in,
               int extra_pad,
               int scaler_mode,
               int *pixels_out,
               int strip_size_out,
               int *total_rows_out );
void pie_xy_calc_expected_rows( int rows_in, int fracY, int scaler_mode, int *rows_out );
void pie_xy_calc_expected_pixels( int pixels_in, int fracY, int scaler_mode, int *pixels_out );
void pie_xy_calc_xyscale( struct pie_scale *scale, int scaler_mode );
scan_err_t pie_xy_calc_stripsize( int *read_stripsize, int fracY, 
                           int scaler_mode, int *write_stripsize);
uint32_t pie_xy_calc_linear_mode_edge_pixels( uint32_t numerator, 
                                           uint32_t denominator );

uint32_t pie_interrupt_disable( void );
void pie_interrupt_enable( void );
void pie_interrupt_enable_lock( void );		// add.lsh for background removal
void pie_interrupt_enable_unlock( void );	// add.lsh for background removal
bool pie_interrupt_lock_status_get( void );	// add.lsh for background removal

/* PIE DMA interrupt handler, called from pie_interrupt().
 * Separated from pie_interrupt() so pie.c and piedma.c are independent.
 */
void pie_dma_interrupt( uint32_t ipend );

/* 11-June-05 davep ; run-time swappable DMA driver */
#ifdef ICEDMA_H
/* include icedma.h before pie.h to get access to these functions 
 * XXX - this is a kludge; need to split up pie.h into pie.h and piedma.h
 */
const struct ice_dma_driver * pie_rdma_get_driver( void );
void pie_rdma_set_icebuf_isr( iceisr_f icebuf_isr_f );

const struct ice_dma_driver * pie_wdma_get_driver( void );
void pie_wdma_set_icebuf_isr( iceisr_f icebuf_isr_f );

void pie_dma_set_burst_size( ice_dma_burst_size burst_size );
#endif

/* read DMA - piedma.c */
void pie_rdma_setup_config( uint8_t pie_bpp, scan_cmode_t cmode, pie_pixel_t piein, uint32_t strip_size_in );
void pie_rdma_reset( void );
bool pie_rdma_is_idle( void );
scan_err_t pie_rdma_channel_open( uint8_t channel );
void pie_rdma_channel_close( uint8_t channel );
void pie_rdma_channel_reset( uint8_t channel );
void pie_rdma_channel_start( uint8_t channel );
void pie_rdma_channel_enable( uint8_t channel );
bool pie_rdma_channel_is_enabled( uint8_t channel );
void pie_rdma_channel_disable( uint8_t channel );
void pie_rdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t rows, uint32_t bytes_per_row );
void pie_rdma_channel_load2( uint8_t channel, uint32_t dar1, uint32_t drcr1, 
                            uint32_t dar2, uint32_t drcr2, uint32_t bytes_per_row );
void pie_rdma_channel_dump( uint8_t channel );
void pie_rdma_dump( void );

/* Read DMA channel assignments */

/* From the PIE Read DMA MA:
 *
 * "In monochrome mode, only data in channel 0 is used in non-staggered mode
 * and the channel 0 and channel 1 in staggered mode. The other channels are
 * ignored and bogus data is handed to the pipe for the upper bits of the
 * output data."
 */
#define PIE_RDMA_MONO_EVEN 0
#define PIE_RDMA_MONO_ODD  1

/* "In staggered color mode, the data from Channels 0, 2 and 4 are interleaved
 * with the data on channels 1, 3 and 5 on a column basis. In monochrome mode,
 * the data for channels 0 and 1 are interleaved.  In non-staggered color mode,
 * the data is fetched from channels 0, 2 and 4. For non-staggered monochrome
 * mode, the data is taken from channel 0 only."
 */
#if defined HAVE_STAGGERED_SENSOR_SUPPORT
    #define PIE_RDMA_RED_EVEN   0
    #define PIE_RDMA_GREEN_EVEN 2
    #define PIE_RDMA_BLUE_EVEN  4
    #define PIE_RDMA_RED_ODD    1
    #define PIE_RDMA_GREEN_ODD  3 
    #define PIE_RDMA_BLUE_ODD   5
#else
    #if PIE_RDMA_NUM_CHANNELS==6
        /* davep 29-Mar-2010 ; as of this writing, PIE channels are still 0,2,4
         * in non-staggered ASICs. We'll need "holes" in our map.
         */
        #define PIE_RDMA_RED_EVEN   0
        #define PIE_RDMA_GREEN_EVEN 2
        #define PIE_RDMA_BLUE_EVEN  4
    #else 
        /* davep 22-Mar-2011 ; finally a PIE with channels 1,2,3 */
        #define PIE_RDMA_RED_EVEN   0
        #define PIE_RDMA_GREEN_EVEN 1
        #define PIE_RDMA_BLUE_EVEN  2
    #endif
#endif

/* maximum number of rows PIE read DMA allows (hardware limit) */
#define PIE_RDMA_MAX_ROWS  21

/* write DMA - piedma.c */
void pie_wdma_setup_config( scan_cmode_t cmode, pie_pixel_t pieout, 
                            uint32_t strip_size_out, uint32_t pixels_per_row );
void pie_wdma_dynamic_rows( bool flag );
bool pie_wdma_is_idle( void );
void pie_wdma_reset( void );
scan_err_t pie_wdma_channel_open( uint8_t channel );
void pie_wdma_channel_close( uint8_t channel );
void pie_wdma_channel_reset( uint8_t channel );
void pie_wdma_channel_start( uint8_t channel );
void pie_wdma_channel_enable( uint8_t channel );
bool pie_wdma_channel_is_enabled( uint8_t channel );
void pie_wdma_channel_disable( uint8_t channel );
void pie_wdma_channel_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                            uint32_t rows, uint32_t bytes_per_row );
void pie_wdma_channel_dump( uint8_t channel );
void pie_wdma_dump( void );

void pie_wdma_debug_log( void );

/* Write DMA channel assignments */

/* xRGB output all comes out channel 0 */
#define PIE_WDMA_CHANNEL_xRGB  0

/* RGBx output also all comes out channel 0 */
#define PIE_WDMA_CHANNEL_RGBx  0

/* As you might have guessed, RGB output also all comes out channel 0 */
#define PIE_WDMA_CHANNEL_RGB   0

/* mono output comes out channel 0 */
#define PIE_WDMA_CHANNEL_MONO  0

/* red,green,blue come out separate channels */
#define PIE_WDMA_CHANNEL_RED   0
#define PIE_WDMA_CHANNEL_GREEN 1
#define PIE_WDMA_CHANNEL_BLUE  2

/* maximum number of rows PIE Write DMA allows (hardware limit) */
#define PIE_WDMA_MAX_ROWS 16

/* Mono HalfPack block - piehp.c */
void piehp_reset( void );
void piehp_dump( void );

/* davep 27-Nov-2007 ; HalfPack now has his own interrupt vectors */
#ifdef HAVE_INDEPENDENT_HALFPACK
void piehp_rdma_interrupt( uint32_t intRegMask );
void piehp_wdma_interrupt( uint32_t intRegMask );
#define INT_DISABLE_HALFPACK  do { intDisable( INTNUM_HALFPACK_READ ); intDisable( INTNUM_HALFPACK_WRITE );} while(0);
#define INT_ENABLE_HALFPACK  do { intEnable( INTNUM_HALFPACK_READ ); intEnable( INTNUM_HALFPACK_WRITE );} while(0);
#else
/* In this code, HalfPack share's PIE's interrupt.
 *
 * NOTE! These can be very dangerous if you don't know what you're doing. Be
 * very careful about assuming PIE and HalfPack are different interrupts. 
 * For example, turning off INTNUM_PIE then doing INT_ENABLE_HALFPACK and
 * assuming INTNUM_PIE is still disabled. Danger! Danger! Danger!
 *
 * As of this writing, the only place I enable/disable PIE (and now HalfPack)
 * interrupts where we have to be very careful is doing xxx_add_ready() in
 * threadspace which shares data structures with icedma.c which lives in
 * interrupt space.
 *
 * davep 28-Nov-2007
 */
/* davep 08-Nov-2012 ; HalfPack long gone. Only survives as a firmware
 * halftoner used with Fax. Time to remove.  
 */
#define INT_DISABLE_HALFPACK  
#define INT_ENABLE_HALFPACK   
//#define INT_DISABLE_HALFPACK  intDisable(INTNUM_PIE)
//#define INT_ENABLE_HALFPACK   intEnable(INTNUM_PIE)
#endif

void piehp_rdma_reset( uint8_t channel );
void piehp_rdma_start( uint8_t channel );
void piehp_rdma_enable( uint8_t channel );
void piehp_rdma_disable( uint8_t channel );
void piehp_rdma_dump( void );
void piehp_rdma_load( uint8_t channnel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                        uint32_t lines, uint32_t bytes_per_line );

void piehp_wdma_reset( uint8_t channel );
void piehp_wdma_start( uint8_t channel );
void piehp_wdma_enable( uint8_t channel );
void piehp_wdma_disable( uint8_t channel );
void piehp_wdma_dump( void );
void piehp_wdma_load( uint8_t channel, uint8_t *data_ptr, dma_addr_t dma_dest, 
                        uint32_t lines, uint32_t bytes_per_line );

void piehp_set_size_sram(uint32_t *size_values, int num_values );
void piehp_set_signed_bias_sram(uint32_t *signed_bias_values, int num_values );
void piehp_set_density_sram(uint32_t *density_values, int num_values);
void piehp_set_error_diffusion_sram(uint32_t *error_diff_values, int num_values );
void piehp_set_szdd_sram( uint32_t *size_values, uint32_t *error_diff_values, 
                          uint32_t *density_values, int num_values );
void piehp_set_error_diffusion_mode( uint32_t mode );
void piehp_set_wdma_direction(HP_DMA_DIR dir);
void piehp_set_rdma_direction(HP_DMA_DIR dir);
void piehp_set_start_of_image( bool enable );
void piehp_set_replicate_vert_data(bool replicate);
void piehp_set_replicate_horiz_data(bool replicate);
void piehp_set_bit_depth(int depth);
void piehp_set_bypass(bool bypass);
void piehp_set_invert_data(bool invert);
void piehp_set_include_accumerror_idx(bool include);
void piehp_set_include_signed_bias_idx(bool include);
void piehp_set_include_signed_bias_do_a_dot(bool include);
void piehp_set_do_a_dot_override(bool do_a_dot);
void piehp_set_shift(uint8_t shift);
void piehp_set_error_weight_1(uint8_t weight);
void piehp_set_error_weight_2(uint8_t weight);
void piehp_set_error_weight_3(uint8_t weight);
void piehp_set_bias_sram_config(HP_BIAS_CONFIG config);
void piehp_set_bias_reg0(uint8_t value);
void piehp_set_bias_reg1(uint8_t value);
void piehp_calc_row( int pixels_per_row_in,
                     int bpp, bool horiz_rep,
                     struct pie_dma_row *row );

/* 8-Apr-05 ; new ICE DMA driver */

/* forward declaration for some prototypes below */
struct ice_dma_buffer;

/* 
 * PIE read DMA 
 */
void pie_rdma_init( void );
int pie_rdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t pie_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row);
void pie_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme);
void pie_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void pie_rdma_channel_launch( uint8_t channel );
void pie_rdma_sanity( void );
void pie_rdma_debug_log( void );
void pie_rdma_isr( uint8_t channel );
void pie_rdma_close( void );
void pie_rdma_cancel( void );

/* 
 * PIE write DMA
 */
void pie_wdma_init( void );
int pie_wdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t pie_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row);
void pie_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme);
void pie_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void pie_wdma_channel_launch( uint8_t channel );
void pie_wdma_sanity( void );
void pie_wdma_debug_log( void );
void pie_wdma_isr( uint8_t channel );
void pie_wdma_close( void );
void pie_wdma_refill( int rows_per_buffer );
void pie_wdma_cancel( void );

int pie_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                           int rows_per_buffer, int bytes_per_row );


/* PIEDMA_FRIEND is for testing code which needs to be able to see the insides
 * of the structures; we don't have a ridiculous proliferation of small utility
 * functions
 */
#ifdef PIEDMA_FRIEND
struct ice_dma_mm *pie_rdma_get_mm( void );
struct ice_dma_mm *pie_wdma_get_mm( void );
#endif

/** PIE Mono HalfPack prototypes **/

/* 
 * PIE Mono HalfPack read DMA 
 */
void piehp_rdma_init( void );
int piehp_rdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t piehp_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row);
void piehp_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme);
void piehp_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void piehp_rdma_channel_launch( uint8_t channel );
void piehp_rdma_sanity( void );
void piehp_rdma_debug_log( void );
void piehp_rdma_isr( uint8_t channel );
void piehp_rdma_close( void );
void piehp_rdma_cancel( void );
void piehp_rdma_run_forever( void );

/* 
 * PIE Mono HalfPack write DMA
 */
void piehp_wdma_init( void );
int piehp_wdma_open( uint8_t channels[], uint8_t num_channels,
               int num_empties, int total_rows,
               int bytes_per_row );
scan_err_t piehp_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row);
void piehp_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme);
void piehp_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void piehp_wdma_channel_launch( uint8_t channel );
void piehp_wdma_sanity( void );
void piehp_wdma_debug_log( void );
void piehp_wdma_isr( uint8_t channel );
void piehp_wdma_close( void );
void piehp_wdma_refill( int rows_per_buffer );
void piehp_wdma_cancel( void );
void piehp_wdma_run_forever( void );

int piehp_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                               int rows_per_buffer, int bytes_per_row );

/* PIEDMA_FRIEND is for testing code which needs to be able to see the insides
 * of the structures; we don't have a ridiculous proliferation of small utility
 * functions
 */
#ifdef PIEDMA_FRIEND
struct ice_dma_mm *piehp_rdma_get_mm( void );
struct ice_dma_mm *piehp_wdma_get_mm( void );
#endif

/* davep 24-May-2013 ; adding capability to multiplex pie */
void piems_dma_set_bank( uint8_t banknum );
scan_err_t piems_dma_open_bank( uint8_t banknum );
void piems_dma_close_bank( uint8_t banknum );
void piems_dma_stall( void );
void piems_dma_reload( void );

/* 10-June-05 ; PIE MultiStrip Read DMA (when in doubt, use the single strip
 * version above)
 */
void piems_rdma_init( void );
int piems_rdma_open( uint8_t channels[], 
                            uint8_t num_channels,
                            int num_empties, 
                            int total_rows,
                            int bytes_per_row,
                            int strip_size );
scan_err_t piems_rdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row );
void piems_rdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme );
void piems_rdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void piems_rdma_channel_launch( uint8_t channel );
void piems_rdma_launch( void );
void piems_rdma_sanity( void );
void piems_rdma_debug_log( void );
void piems_rdma_isr( uint8_t channel );
void piems_rdma_close( void );
void piems_rdma_cancel( void );
/* the following functions are unique to the PIE multistrip DMA driver */
void piems_rdma_init_scaler( int scaler_strip_size, 
                                    int y_numerator, 
                                    int y_denominator, 
                                    int fracY);
void piems_rdma_intmsg( uint8_t channel );
void piems_rdma_set_image_offsets( int image_offsets[] );
void piems_rdma_init_mf( void );
void piems_rdma_init_colorshift( void );
void piems_rdma_run_forever( void );
bool piems_rdma_is_idle( void );
bool piems_rdma_is_empty( void );


/* 
 * PIE MultiStrip write DMA
 */
void piems_wdma_init( void );
int piems_wdma_open( uint8_t channels[], uint8_t num_channels,
                            int num_empties, 
                            int total_rows,
                            int bytes_per_row,
                            int strip_size );
scan_err_t piems_wdma_add_buffer( uint8_t channel, uint8_t *data, uint32_t datalen,
                     uint32_t rows, uint32_t bytes_per_row );
void piems_wdma_add_ready( uint8_t channel, struct ice_dma_buffer **addme );
void piems_wdma_free_empty( uint8_t channel, struct ice_dma_buffer **freeme );
void piems_wdma_channel_launch( uint8_t channel );
void piems_wdma_launch( void );
void piems_wdma_sanity( void );
void piems_wdma_debug_log( void );
void piems_wdma_isr( uint8_t channel );
void piems_wdma_close( void );
void piems_wdma_cancel( void );
/* the following functions are unique to the PIE multistrip DMA driver */
void piems_wdma_init_scaler( int scaler_strip_size, 
                                    int y_numerator, 
                                    int y_denominator, 
                                    int fracY );
void piems_wdma_intmsg( uint8_t channel );
void piems_wdma_refill( int rows_per_buffer );
void piems_wdma_init_colorshift( void );
void piems_wdma_run_forever( void );

int piems_wdma_setup_buffers( uint8_t channel, int num_buffers, 
                           int rows_per_buffer, int bytes_per_row );
bool piems_wdma_is_idle( void );
bool piems_wdma_is_empty( void );

/* PIEDMA_FRIEND is for testing code which needs to be able to see the insides
 * of the structures; we don't have a ridiculous proliferation of small utility
 * functions
 */
#ifdef PIEDMA_FRIEND
struct ice_dma_mm *piems_rdma_get_mm( void );
struct ice_dma_mm *piems_wdma_get_mm( void );
#endif

#endif /* PIE_H */

