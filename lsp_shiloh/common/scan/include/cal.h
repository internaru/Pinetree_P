/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006-2011 Marvell Semiconductor Inc. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan calibration APIs
 *
 *****************************************************************************/

#ifndef CAL_H
#define CAL_H

/* davep 06-Jun-2008 ; as of this writing, we're very hardwired to use 16-bit
 * pixels for cal. But want to start moving toward being able to cal 8-bit
 * pixels, too.
 */
#define CAL_BYTES_PER_PIXEL   2
typedef uint16_t cal_pixel_t;

#define CAL_MM_PER_INCH                   25.4
#define CAL_PRNU_MAX_DUST_SIZE_MM         0.35
#define CAL_DSNU_MAX_DUST_SIZE_MM         0.35


/* davep 14-Jan-2009 ; three uint32_t encoded as TLD (Type+Length+Data) metadata
 * sent in the SMSG_SCAN_BLOB.
 *
 *  0123456789abcef0123456789abcef0123...
 * [tttttttttttttttllllllllllllllldddd...]
 * t = type (CAL_METADATA_xxx)
 * l = length in bytes
 * d = data 
 *
 * All integers are sent in network byte order.
 *
 * The host tools depend on these values to always be constant. Don't move them
 * around. Need a key? Add one. Don't change.
 */

#define CAL_METADATA_BASE 100
#define CAL_METADATA_VERT_RES           (CAL_METADATA_BASE+1)
#define CAL_METADATA_HORIZ_RES          (CAL_METADATA_BASE+2)
#define CAL_METADATA_CMODE              (CAL_METADATA_BASE+3)
#define CAL_METADATA_RED_ENABLED        (CAL_METADATA_BASE+4)
#define CAL_METADATA_GREEN_ENABLED      (CAL_METADATA_BASE+5)
#define CAL_METADATA_BLUE_ENABLED       (CAL_METADATA_BASE+6)
#define CAL_METADATA_RED_PWM            (CAL_METADATA_BASE+7)
#define CAL_METADATA_GREEN_PWM          (CAL_METADATA_BASE+8)
#define CAL_METADATA_BLUE_PWM           (CAL_METADATA_BASE+9)
#define CAL_METADATA_RED_EXP            (CAL_METADATA_BASE+10)
#define CAL_METADATA_GREEN_EXP          (CAL_METADATA_BASE+11)
#define CAL_METADATA_BLUE_EXP           (CAL_METADATA_BASE+12)
#define CAL_METADATA_RED_ANALOG_GAIN    (CAL_METADATA_BASE+13)
#define CAL_METADATA_GREEN_ANALOG_GAIN  (CAL_METADATA_BASE+14)
#define CAL_METADATA_BLUE_ANALOG_GAIN   (CAL_METADATA_BASE+15)
#define CAL_METADATA_RED_ANALOG_OFFSET  (CAL_METADATA_BASE+16)
#define CAL_METADATA_GREEN_ANALOG_OFFSET (CAL_METADATA_BASE+17)
#define CAL_METADATA_BLUE_ANALOG_OFFSET (CAL_METADATA_BASE+18)
#define CAL_METADATA_BITS_PER_PIXEL     (CAL_METADATA_BASE+19)
#define CAL_METADATA_ROWS_PER_STRIP     (CAL_METADATA_BASE+20)
#define CAL_METADATA_PIXELS_PER_ROW     (CAL_METADATA_BASE+21)
#define CAL_METADATA_CAL_CAP            (CAL_METADATA_BASE+22) 
#define CAL_METADATA_PIXELBAND_MONO     (CAL_METADATA_BASE+23)
#define CAL_METADATA_PIXELBAND_RED      (CAL_METADATA_BASE+24)
#define CAL_METADATA_PIXELBAND_GREEN    (CAL_METADATA_BASE+25)
#define CAL_METADATA_PIXELBAND_BLUE     (CAL_METADATA_BASE+26)
#define CAL_METADATA_PRNU_WHITE_AVG     (CAL_METADATA_BASE+27)
#define CAL_METADATA_PRNU_BLACK_AVG     (CAL_METADATA_BASE+28)
#define CAL_METADATA_DEVICE_ENDIAN      (CAL_METADATA_BASE+29)
#define CAL_METADATA_CAL_STRIP          (CAL_METADATA_BASE+30)
// *NOW* is sent down when we start calibrating a new side
#define CAL_METADATA_NOW_CALIBRATING_SENSOR_NUM (CAL_METADATA_BASE+31)

/* Values for the CAL_METADATA_CAL_CAP; reasons cal is doing an image capture .
 * Useful for a downstream client to interpret our data and make cal graphs.
 * (For the life of me, I couldn't come up with a better name.)
 *
 * Note I've spread the numbers apart a bit so we can add new caps later without
 * breaking a lot of existing code on the host.
 *
 * We need to have enough tags to match what the calgraph application needs.
 */

#define CAL_CAP_NONE  0  // reserved for NULL/NIL/uninitialized
#define CAL_CAP_DEBUG 42 // a capture purely for debug (can happen multiple times)
#define CAL_CAP_DESAT 50 /* desaturate the sensor */
#define CAL_CAP_ANALOG_OFFSET       100
#define CAL_CAP_AFTER_ANALOG_OFFSET 101
#define CAL_CAP_ANALOG_GAIN         110
#define CAL_CAP_AFTER_ANALOG_GAIN   111
#define CAL_CAP_LEDS          200
#define CAL_CAP_AFTER_LEDS    201
#define CAL_CAP_DSNU        300
#define CAL_CAP_AFTER_DSNU  301
#define CAL_CAP_PRNU        310
#define CAL_CAP_AFTER_PRNU  311

/**
  \brief Set up hardware blocks for Calibration
  Call this to go back to a 100% startup state.
*/
scan_err_t cal_onetime_init(void);

/**
   \brief Main calibration entry
*/
scan_err_t calibrate(uint32_t sensor_bitmask, int resolution, 
                     int scanType, 
                     bool force );

/**
 * \brief Clean up hardware blocks responsible for calibration
 *
 */
scan_err_t cal_done( void );

/**
  \brief Dump calibration cache
    
*/
void cal_cache_print( uint32_t sensor_num, int hres, scan_cmode_t cmode);

/**
 * \brief flush all entries from the cal cache
 *
 * Marks all the entries in the cal cache as too old.
 *
 */
void cal_cache_flush_all( void );

/**
  \brief Check to see whether a calibration has been done with the given scan mode and 
          reolution on the requested sensors
    
  \return TRUE    Calibration is done with usable cahced settings
          FALSE   No calibration hass been done
*/
bool cal_check_cal_done(int res, 
                        scan_cmode_t cmode, uint32_t sensor_mask);

/**
   \brief Load cached PIC settings  

*/
scan_err_t cal_load_from_cache( uint32_t     dpi, 
                                scan_cmode_t cmode,
                                bool update_hit_count,
                                scanvar_cal_t cal_mode,
                                uint32_t sensor_bitmask);

/**
 * \brief  Decode a pixel array received in a SMSG_DATA_BLOB
 *
 *
 */
scan_err_t cal_decode_pixel_array_msg( uint32_t *buffer, 
                                       scan_data_type *dtype,
                                       int *num_pixels,
                                       cal_pixel_t **pixel_buf_p );



#endif

