/**
 * \file scansen.h
 *
 * \brief  Scan Sensor driver
 *
 * Header File for the ScanSensor class (scansen.c)
 *
 */
/*
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */


#ifndef SCANSEN_H
#define SCANSEN_H


/**
 * \brief Power up initialization of the scan sensor 
 *
 * Initializes the scan sensor and any associated internal structures, variables, etc.
 * This function will be called once at system startup.  
 *
 * \return scan_err_t any scan error that occured (if all is well, SCANERR_NONE)
 *
 * \remarks If there's nothing that needs to be done, just return SCANERR_NONE.
 **/
scan_err_t scansen_onetime_init( void );

/**
 * \brief Run a selftest on the sensor / AFE subsystem
 *
 * Run selftests (chat up the AFE, capture some data, perhaps attempt to
 * diagnose any hardware faults).  
 *
 * The selftest is separated from the onetime_init because other parts of the
 * hardware subsystem (e.g., PIC) must be initialized before we can do an image
 * capture. 
 *
 * \return scan_err_t any scan error that occured (if hw is happy, return SCANERR_NONE)
 *
 * See also scan_set_hwerr() and scanhwerr.h
 *
 */
scan_err_t scansen_selftest( void );

/**
 * \brief Sets up the scan sensor for the requested dpi and color mode
 *
 * A scan sensor can support a variety of resolutions and modes (color, mono).
 * Based on these values, the sensor may require different settings
 * (e.g. the sensor needs a signal asserted to indicate 600 dpi).
 *
 * \param[in] cmode  the requested color mode
 *
 * \param[in] dpi  the requested dpi
 *
 * \param[in] scanx ; the sensor's pixels per row 
 *
 * \return scan_err_t any scan error that occured
 * \retval SCANERR_NONE - dpi was found, and scan sensor set up
 * \retval SCANERR_INVALID_PARAM - requested dpi is not supported
 *
 **/
scan_err_t scansen_setup( scan_cmode_t cmode, uint32_t dpi, uint32_t scanx );


/**
 * \brief Ask scansen for pixels per row. 
 *
 * \author David Poole
 * \date 24-Aug-2011
 *
 * Need to ask the scansen layer for scanx (pixels per row) to handle the case
 * where the scansen driver needs to change scanx to meet different padding
 * requirements.  (SCANX is the scan block's pixels per row register.)
 *
 * Initially added for a sensor sending 10-bpp over a serial interface. The
 * 10-bpp pixels to memory had to be DMA aligned by changing scanx.  If the
 * scansen driver doesn't need to change, don't touch the value.
 *
 * 24-Aug-2011 ; Updated to handle multi-channel parallel sensors (CISX).
 *
 */

scan_err_t scansen_calc_scanx( scan_cmode_t cmode, uint32_t dpi, uint32_t *scanx );

/**
 * \brief Returns the margin table
 *
 * Uses a platform-specific call to get the margin table, then searches
 * through it to find the appropriate entry for the requested dpi, then
 * returns just that entry.
 *
 * \param[in] dpi  the dpi of the margin entry to be returned
 *
 * \param[out] margins  the margin entry that matches the requested dpi
 *
 * \return scan_err_t any scan error that occured.
 * \retval SCANERR_NONE - entry was found and returned
 * \retval SCANERR_INVALID_PARAM - margin table has invalid entry
 * \retval SCANERR_NO_ENTRY - the requested dpi was not found in the margin table
 *
 **/
scan_err_t scansen_get_margins( uint32_t dpi, scan_sensor_margin_t *margins );

/**
 * \brief Setup the illumination time for each LED 
 *  
 *  Normally, the 3 LEDs (RGB) are on in sequence for a given amount
 *  of time.  The gate values for each color specify how much time
 *  each of the LEDs should be on.  If the gate values are all set to
 *  the same value, then each LED will be on for the same amount of time
 *  (when the time comes in the sequence for the LED to be on). 
 *
 * \param[in] cmode  the requested color mode
 * \param[in] pixels_per_line  the number of pixels on a scan line
 * \param[in] gateR - the percentage of time the red LED should be on
 * \param[in] gateG - the percentage of time the green LED should be on
 * \param[in] gateB - the percentage of time the blue LED should be on 
 *
 * davep 04-Jun-2012 ; Removing floats. The gate value is an integer in [0,100]
 *                     for 0,100%.  Note there is no decimal precision (like
 *                     with the analog gain).
 **/
void scansen_set_leds( uint32_t sensor_num, scan_cmode_t cmode, uint32_t pixels_per_line, 
                       uint32_t gateR, uint32_t gateG, uint32_t gateB );

/**
 * \brief Enable the LEDs individually
 *
 * Each LED (RGB) can be individually enabled or disabled.
 *
 * \param[in] enable_red  enable or disable the red LED:
 *            true=enable, false=disable
 * \param[in] enable_green  enable or disable the green LED:
 *            true=enable, false=disable
 * \param[in] enable_blue  enable or disable the blue LED:
 *            true=enable, false=disable
 *
 **/
void scansen_led_enable( uint32_t sensor_num, bool enable_red, bool enable_green, bool enable_blue );

/**
 * \brief Send the LED percentile to the hardware
 *
 * Each LED (RGB) can be individually set for strength.  Range is 0-100% x100
 * so we don't use floating point. For example, 42.69% = 4269.
 *
 * This function can be used to set power, pwm duty cycle, voltage, current, etc.
 *
 * \param[in] dutyR  strength for the red LED
 * \param[in] dutyG  strength for the green LED
 * \param[in] dutyB  strength for the blue LED
 *
 *
 * davep 04-Jun-2012 ; Removing floats. The gate value is an integer in [0,100]
 *                     for 0,100%.  Note there is no decimal precision (like
 *                     with the analog gain).
 **/
void scansen_set_led_percentile( uint32_t sensor_num, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB);

/**
 * \brief Send the analog offsets for the 3 colors to the AFE
 *
 * Take platform independent analog offset values, convert them to the
 * values needed for our AFE, and write them to the AFE.  This function
 * is normally used by calibration which uses this function to set the offset,
 * take measurements, and compute the final offset.  This function assures
 * that the values written to the afe stay within the values the AFE supports.
 *
 * \param[in] red_offset  platform-independent, red analog offset value
 * \param[in] green_offset  platform-independent, green analog offset value
 * \param[in] blue_offset  platform-independent, blue analog offset value 
 *
 * The range of the input parms is 0-65535
 *
 *  \remark Because we compute cugs (dac_unit_gain_output_change_step) as the number
 *  of changes in the pixel offset for a unit change in the dac offset, computing
 *  the dac offset is a simple matter of dividing the pixel offset by cugs, and
 *  adding in the zero offset
 *
 *
 */
void scansen_set_analog_offset(uint32_t sensor_num, int red_offset, int green_offset, int blue_offset);

/**
 * \brief Send the analog gain for the 3 colors to the AFE
 *
 * Take platform independent analog gain values, convert them to the
 * values needed for our AFE, and write them to the AFE.  This function
 * is normally used by calibration, which uses this function to set the gain,
 * take measurements, and compute the final gain.  This function assures
 * that the values written to the afe are appropriate for the AFEs gain function
 *
 * \param[in] red_gain  platform-independent, red analog gain value
 * \param[in] green_gain  platform-independent, green analog gain value
 * \param[in] blue_gain  platform-independent, blue analog gain value 
 *
 * There is no absolute range, as the gain parameter is the ratio of the
 * desired output pixel to the un-gained output pixel.
 *
 * davep 04-Jun-2012 ; Removing floats. The gate value is an integer in
 *                      [0,10000] for 0,100.00%.  Using float*100 to have two
 *                      decimal digits of precision.
 */
void scansen_set_analog_gain(uint32_t sensor_num, uint32_t red_gain, uint32_t green_gain, uint32_t blue_gain);

#ifdef SCANIF_H
/**
 * \brief Fill out the SENSOR_CONF structure with the sensor and AFE clocks for
 * our platform
 *
 * \param[out] sensor  the structure containing all the sensor and AFE values
 *
 */
void scansen_get_sensor_conf(uint32_t sensor_num, scan_cmode_t cmode, uint32_t sensor_dpi, SENSOR_CONF *sensor );
#endif

/**
 * \brief Return the multi-segment sensor configuration.
 *
 * If you don't know what CISX means, return SCANERR_NO_ENTRY.
 *
 * See also cisx.h
 *
 * \date 26-Jul-2011
 *
 */

#ifdef CISX_H
scan_err_t scansen_get_cisx_conf(uint32_t sensor_dpi, struct cisx_sensor_conf *sensor );
#endif

uint32_t scansen_get_num_sensors(void);
#define SCANSEN_MAX_SENSORS 16

scan_err_t scansen_run( void );
scan_err_t scansen_stop( void );
scan_err_t scansen_pause( void );
scan_err_t scansen_resume( void );

#endif

