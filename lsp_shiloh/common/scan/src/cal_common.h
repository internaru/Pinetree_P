/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 * \file cal_common.h
 *
 * \brief Declarations for internally used scanner calibration routines .
 *
 */

#ifndef CAL_COMMON_H
#define CAL_COMMON_H

typedef enum {
    CALERR_NONE = 0,

    /* general errors */
    CALERR_OUT_OF_MEMORY     = -1,
    CALERR_INVALID_PARAMETER = -2,
    CALERR_SCAN_FAILED       = -3, 
    CALERR_NO_ENTRY          = -4,
    CALERR_HARDWARE_FAIL     = -5,

    /* davep 25-Oct-2012 ; we now search for the cal strip inside a scanned
     * image 
     */
    CALERR_CAL_STRIP_NO_LEADING_EDGE = -6,
    CALERR_CAL_STRIP_NO_TRAILING_EDGE = -7,

    /* analog offset errors */
    CALERR_ANALOG_OFFSET_BAD_FLOOR = -100,

    /* LED errors */
    /* TODO */

    /* analog gain errors */
    CALERR_ANALOG_GAIN_INVALID_CH0 =-300,
    CALERR_ANALOG_GAIN_INVALID_CH1 =-301,
    CALERR_ANALOG_GAIN_INVALID_CH2 =-302,
    /* TODO */

    /* PRNU/DSNU errors */
    CALERR_PRNU_TOO_MANY_UNGAINABLE_PIXELS = -400,
    /* TODO -- add more prnu and dsnu errors */

    /* more? */

} cal_err_t;


typedef struct {
    /* 
     * Primary keys into cal cache table
     */
    int hres;
    scan_cmode_t cmode;
    uint32_t sensor_num;
	
    /* cache entry is currently initialized and in use */
    bool in_use;

    /*
     *  Values stored in cal cache
     */
    uint32_t led_exp_red;
    uint32_t led_exp_green;
    uint32_t led_exp_blue;
    uint32_t led_pwm_red;
    uint32_t led_pwm_green;
    uint32_t led_pwm_blue;
    int offset_red;
    int offset_green;
    int offset_blue;
    float gain_red;
    float gain_green;
    float gain_blue;

    /** a malloc'd buffer holding all our correction data */
    uint8_t *prnudsnu_lut;

    /** a malloc'd buffer holding the slices of correction data for the current
     * scan.  It's all about how the PRNU/DSNU RDMA, PIC margins, and pixel
     * correction interact.
     *
     * We calibrate the full width of the sensor and store the correction data
     * into prnudsu_lut. The cur_prnudsnu_lut is exactly the same size as
     * prnudsnu_lut but is made up of chunks of memcpy'd prnudsnu_lut.
     *
     * Suppose a user wants to scan a 3" strip in the middle of the page. We'll
     * calibrate the full width of the sensor and store the correction data in
     * prnudsnu_lut. Then we'll memcpy the appropriate 3" of correction data
     * into the front of cur_prnudsnu_lut. If we're scanning RGB, the three
     * R,G,B correction luts are appended to each other. 
     *
     * davep 24-Apr-2008 ; added this comment long after code was originally
     * written
     */
    uint8_t *cur_prnudsnu_lut;

    /* pointers into cur_prnudsnu_lut ("pc" stands for "pixel correction") */
    uint8_t *pc_lut_ptr_red;
    uint8_t *pc_lut_ptr_green;
    uint8_t *pc_lut_ptr_blue;

    /** size of the prnu/dsnu luts */
    int num_pixels_in_lut;

    /** how many times this entry has been used so far (for cache aging) */
    int use_count;

    /** when the entry was created in seconds since boot (for cache aging) */
    uint32_t creation_time;

#ifdef HAVE_PIC_PC_QUAD
    /** Quadratic Scaling Factor SF1*/
    int32_t quad_SF1; //SF1

    /** Quadratic Limit Scaling Factor SF2*/
    int32_t quad_SF2;

    /** Quadratic Shift Amount*/
    uint32_t quad_shift;
#endif

} cal_cache_t;


#define NUM_PIXEL_BANDS 3

typedef struct CalibrationCaseTag
{
    int horiz_res, vert_res;
    uint32_t sensor_num;
    
    /** SCAN_CMODE_MONO or SCAN_CMODE_COLOR */
    scan_cmode_t cmode; 

    /**Forced calibrationon/off flag*/
    bool forceCalOn;
    
    /**A bit array to record uncorrectable pixels*/
    uint8_t *pixels_uncorrectable[3];
    
    /**Cached value for pixels to scan */
    uint32_t pixels_to_scan;
    
    /** discard this many pixels from top and bottom of sorted list during
     * digital calibration (we want to discard outlying pixel values)
     */
    uint32_t pixels_to_discard;

    uint32_t red_pwm,green_pwm,blue_pwm;
    uint32_t red_exp,green_exp,blue_exp;
    float red_gain,green_gain,blue_gain;
    int32_t red_offset,green_offset,blue_offset;

    /** enable/disable the LEDs */
    bool red_enable, green_enable, blue_enable;
    
    int bandsToWatch;
    // *2 for upper and lower bounds */
    uint32_t bandsDefined[NUM_PIXEL_BANDS*2];  
    // [3] for mono,red,green,blue (mono/red share [0])
    int pixelsInBand[3][NUM_PIXEL_BANDS];  
    
    int ClippedHigh[3];

    /* davep 20-Jan-2009 ; cal graphs are separated from cal but cal needs to do
     * a few extra scans at calgraph's request. The extra scans are stored in
     * this list, zero terminated.
     */
#define MAX_EXTRA_CAL_CAPS 12
    int extra_cal_caps[MAX_EXTRA_CAL_CAPS];

} CalibrationCase;


typedef struct CAL_CONFIG_tag
{
    uint8_t   PRNUDSNU_LUT_ENTRY_SIZE;
    bool      USE_QUAD;
    bool      PRNUDSNU_LUT_USE_16BIT_PRNU_DSNU;
    uint8_t   DSNU_SHIFT;
    uint32_t  ANALOG_GAIN_CAP_TIMES_10;
    uint32_t  MAX_DIGITAL_GAIN_TIMES_10; 
    uint32_t  COLOR_SAT_SLOPE_THRESHOLD;
    uint32_t  MONO_SAT_SLOPE_THRESHOLD;
    uint16_t  MAX_DIGITAL_CORRECTION;
    uint8_t   DIGITAL_GAIN_PRECISION; //Bits cnt for fractional for PRNU
    uint16_t  REPLACE_BAD_PIXEL;
    bool      BYPASS_REPLACE_BAD_PIXEL; 
    int (*hw_setup)(const struct CAL_CONFIG_tag *);
    cal_err_t (*build_case)( CalibrationCase **pp_cal_case, uint32_t sensor_num, int resolution, int scanType, bool force);
    cal_err_t (*process_case)(CalibrationCase *pCCase);

}CAL_CONFIG;

const CAL_CONFIG  *cal_get_config(void);

/* params for 'scan_defines' of cal_run_debug_scans */
#define CAL_DEBUG_SCAN_WHITE_DIGITAL_OFF (1<<0)
#define CAL_DEBUG_SCAN_GRAY_DIGITAL_OFF (1<<1)
#define CAL_DEBUG_SCAN_GRAY_DIGITAL_ON  (1<<2)
#define CAL_DEBUG_SCAN_WHITE_DIGITAL_ON (1<<3)
#define CAL_DEBUG_SCAN_GRAY_WITH_STEPS  (1<<4)

scan_err_t cal_err_to_scan_err( cal_err_t calerr );
void cal_set_scan_error(scan_err_t err);
scan_err_t cal_get_scan_error(void);
void cal_send_all_calcase_metadata( CalibrationCase *pCCase );
void cal_send_scan_id( uint32_t id);
void cal_run_debug_scans( CalibrationCase *pCCase, cal_cache_t *cached_cal, uint32_t scan_defines );
cal_err_t cal_dummy_scan(CalibrationCase *pCCase);
scan_err_t cal_soft_setup( void);
scan_err_t cal_debug_scan( CalibrationCase *pCCase );
const cal_cache_t *cal_get_cache_settings( uint32_t sensor_num, int cal_hres, scan_cmode_t cmode);
void store_to_cal_cache( cal_cache_t *ccache, CalibrationCase *pCCase );
int get_needed_rows_per_buffer( int dpi );
void cal_horizontal_setup( CalibrationCase *pCCase );
scan_err_t cal_decode_pixel_array_msg( uint32_t *buffer, 
                                       scan_data_type *dtype ,
                                       int *num_pixels,
                                       cal_pixel_t **pixel_buf_p );

cal_err_t cal_send_pixel_array( int tag, 
                                  scan_data_type dtype,
                                  int num_pixels, 
                                  cal_pixel_t *pixel_array );
void cal_send_pixel_band_counts( CalibrationCase *pCCase );
void cal_cache_flush_all( void );
void cal_cache_flush_mode( int mode );
void closeCalibrationCase(CalibrationCase *pCCase);
void prnudsnu_sanity_check( scan_cmode_t cmode );
void init_cal_using_cached( cal_cache_t *cached_cal);
cal_err_t retrieveCachedIndex( uint32_t sensor_num, int hres, scan_cmode_t cmode, int *cache_idx);
void cal_open_cisx( const struct scanvars *sv );
void cal_close_cisx( void );
bool do_cal(CalibrationCase *pCCase, cal_cache_t *cached_cal);
cal_err_t allocate_pixel_logs( CalibrationCase *pCCase );
scan_err_t cal_set_cal_cache_timeout( uint32_t timeout_seconds, uint32_t cache_hits );
uint16_t findCurveFloor(uint16_t *buff, int numPixels, int numToDrop, int numToAvg);
uint16_t findCurveCeiling(uint16_t *buff, int numPixels,  int numToDrop, int numToAvg);
cal_err_t cal_scan_lines_open( scan_target_type_t cal_target_type );
cal_err_t cal_scan_lines_run( void );
cal_err_t cal_scan_lines_close(void);
void cal_send_uint32_metadata( int num_values, ... );
void ice_getScanData( scan_cmode_t cmode, cal_pixel_t *datas[], 
                      uint32_t *num_rows, uint32_t *pixels_per_row,uint8_t channel_array[] );
cal_err_t get_data_and_average( CalibrationCase *pCCase, int num_colors, int led_enable, uint16_t *pixel_avgs[] );
void *cal_get_memory( int datalen_bytes );
cal_cache_t *getCalCache(CalibrationCase *pCCase);
void cal_debug_scan_setup_pdlut( const cal_cache_t *cached_cal, scan_cmode_t cmode);

void cal_debug_print_calcase( const char *file, int line, CalibrationCase *pCCase );

cal_err_t create_calibration_case( CalibrationCase **pp_cal_case,
                                   uint32_t sensor_num, int resolution, int scanType, bool force);
cal_err_t cal_setup(CalibrationCase *pCCase);
//cal_err_t processCalibrationCase(CalibrationCase *pCCase);
cal_err_t cal_calc_analog_offset( CalibrationCase *pCCase );
cal_err_t cal_calc_analog_gain(CalibrationCase *pCCase); 
cal_err_t cal_calc_pwm_gain(CalibrationCase *pCCase); 
cal_err_t cal_calc_digital_correction(CalibrationCase *pCCase );
cal_err_t cal_calc_led_pwm(CalibrationCase *pCCase); 
void cal_set_max_digital_gain( int intmult );
void turn_off_all_leds(void);

#endif

