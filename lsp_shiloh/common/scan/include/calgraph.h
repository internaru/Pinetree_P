/**
 * \file calgraph.h
 *
 * \brief Scan sensor cal graphs.
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef CALGRAPH_H_
#define CALGRAPH_H_

/* max number of graphs we can print on one sheet */
#define CAL_GRAPH_MAX_SPECS 8

typedef enum
{
    CAL_GRAPH_NO_GRAPH = 0,
    CAL_GRAPH_BEFORE_ANALOG =1,
    CAL_GRAPH_AFTER_ANALOG = 2,
    CAL_GRAPH_BEFORE_DSNU = 3,
    CAL_GRAPH_AFTER_DSNU = 4,
    CAL_GRAPH_BEFORE_PRNU = 5,
    CAL_GRAPH_BEFORE_PRNU_GRAY = 6,
    CAL_GRAPH_AFTER_PRNU = 7,
    CAL_GRAPH_AFTER_PRNU_GRAY = 8,

    CAL_GRAPH_MULTIPLE_UNCORRECTED_MONO = 9,
    CAL_GRAPH_MULTIPLE_CORRECTED_MONO = 10,
    CAL_GRAPH_MULTIPLE_UNCORRECTED_COLOR = 11,
    CAL_GRAPH_MULTIPLE_CORRECTED_COLOR = 12,

} CAL_GRAPH_T;

typedef enum
{
    CAL_GRAPH_COLOR_NONE=0, /* NULL/NIL, aka uninitialized value */
    CAL_GRAPH_COLOR_MONO = 1,
    CAL_GRAPH_COLOR_RED,
    CAL_GRAPH_COLOR_GREEN,
    CAL_GRAPH_COLOR_BLUE
} CAL_GRAPH_COLOR_T;

typedef enum {
    CAL_GRAPH_PRINT_NO_PRINT=0,  ///< Nope, don't want a printed page
    CAL_GRAPH_PRINT_DEFAULT,     ///< Default calgraph print 
    /* TODO more print styles later? */
} cal_graph_print_t;

typedef struct 
{
    CAL_GRAPH_T type;
    CAL_GRAPH_COLOR_T color;

    cal_pixel_t *pixel_buf;
    int num_pixels; ///< number of pixels in pixel_buf

    /* We keep a copy of the calibration data in our pixel_buf but we have to
     * allocate the data before the calibration starts. Instead of possibly
     * graphing garbage if pixel_buf was allocated but not memcpy'd, we set this
     * flag once we capture good data.
     */
    bool initialized;

} CalibrationData;

/* We break up the pixel values into "bands". Each pixel value falls
 * somewhere in one of the bands. We count the pixels in each band and
 * report the value on the printed cal graph.
 */
#define CAL_MAX_PIXEL_BANDS_TO_WATCH 5

#define CAL_PIXEL_BAND_IDX_MONO 0
#define CAL_PIXEL_BAND_IDX_RED  0
#define CAL_PIXEL_BAND_IDX_GREEN 1
#define CAL_PIXEL_BAND_IDX_BLUE  2

struct cal_pixel_band {
    /* if pixel in [min_value,max_value), it is in this band 
     *
     * if( pixel >= min_value && pixel < max_value ) {
     *    pixel_count += 1;
     * }
     *
     */
    float min_value; 
    float max_value;

    uint32_t pixel_count;
};

struct scan_cal_graph {
    int horiz_res, vert_res;

    /** SCAN_CMODE_MONO or SCAN_CMODE_COLOR */
    scan_cmode_t cmode; 
    
    /* if true, we're graphing multiple colors to same print */
    bool multi_graph_mode;

    CAL_GRAPH_T cal_graph_type; 
    CAL_GRAPH_COLOR_T requst_color;
    
    /* if true, we print a page with the calibration */
    cal_graph_print_t cal_graph_print;

    /** Num of graph specs included in this case, NUM<=8. This is the number of
     * elements in the CalibrationData data[] and GraphSpec specs[] arrays
     */
    int number_of_specs;
    
    /** The array of 'number_of_specs' calibration result data. This is the
     * data to match the entries in graph_specs[].
     */
    CalibrationData cal_data[CAL_GRAPH_MAX_SPECS];
    
    /* The scan subsystem tells us the width of the upcoming data in the
     * MSG_SCAN_SIZE.  We use these when data arrives.
     */
    uint32_t scan_pix_per_row;
    uint32_t scan_pix_per_row_padded;

    /* some settings in use when the data was captured; pwm, exp, gain stored as
     * value*100 so we don't need floating point
     */
    uint32_t red_pwm,green_pwm,blue_pwm; 
    uint32_t red_exp,green_exp,blue_exp;
    uint32_t red_gain,green_gain,blue_gain;
    /* can be negative numbers! */
    int32_t red_offset,green_offset,blue_offset;

    /**A bit array to record uncorrectable pixels*/
    uint8_t *pixels_uncorrectable[3];

    /* The targets used in PRNU correction */
    int PRNUTargets[3];

    int bandsToWatch;       
    /* [3] for 3 colors (mono & red share slot 0) */
    struct cal_pixel_band pixel_band[3][CAL_MAX_PIXEL_BANDS_TO_WATCH];
};

/* 
 * If you want a calgraph, use scan_cal_graph_run_job() 
 *
 */

scan_err_t scan_cal_graph_run_job( int dpi, scan_cmode_t cmode, 
                             CAL_GRAPH_T cal_graph_type,
                             CAL_GRAPH_COLOR_T cal_graph_color,
                             cal_graph_print_t cal_graph_print );

/* 
 * The following functions are For Internal Use Only.
 *
 * Don't use them. You'll shoot your eye out.
 *
 */

struct scan_cal_graph *scan_cal_graph_new( void );

void scan_cal_graph_delete( struct scan_cal_graph **p_calgraph );

scan_err_t scan_cal_graph_add_spec( struct scan_cal_graph *calgraph, 
                                    CAL_GRAPH_T cal_graph_type, 
                                    CAL_GRAPH_COLOR_T cal_graph_color);

scan_err_t calgraph_get_cal_data_ptr( struct scan_cal_graph *calgraph, 
                                      CAL_GRAPH_COLOR_T cal_graph_color,
                                      CalibrationData **p_cal_data );

scan_err_t scan_cal_graph_add_data( struct scan_cal_graph *calgraph, 
                                    CAL_GRAPH_COLOR_T cal_graph_color,
                                    cal_pixel_t *pixel_data_ptr,
                                    int num_pixels );

scan_err_t scan_cal_graph_free_data( struct scan_cal_graph *calgraph, 
                                      CAL_GRAPH_COLOR_T cal_graph_color );

scan_err_t scan_cal_graph_print(struct scan_cal_graph *calgraph);

void scan_cal_graph_debug_dump( struct scan_cal_graph *calgraph );

#endif /*CALGRAPH_H_*/

