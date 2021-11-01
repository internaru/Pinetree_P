/** 
 *
 * ============================================================================
 * Copyright (c) 2004-2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 **/

#ifndef AFE_H
#define AFE_H

struct afe_hardware_config_t
{
    int bits_per_pixel;

    /* davep 21-Jan-2010 ; this field is used to set the PIC ADC Normalizer
     * 'Left Justify' bit.  Not sure how well this will map to other AFEs. 
     */
    bool left_justify;

    /* more later? */
};

/**
 * \brief  the type of AFE
 *
 * AFE types 
 *
 *
 */
typedef enum {
    AFE_TYPE_UNKNOWN=0,
    AFE_TYPE_WM8152=1,
    AFE_TYPE_WM8259=2,
    AFE_TYPE_INTERNAL=3
} afe_type_t;

#define AFE_INFO_VERSION_0  0
#define AFE_INFO_VERSION_2  2

/**
 * \brief Hardware specific information about the AFE
 *
 * Information to set AFE registers back to their documented power-on values.
 *
 */
struct scan_afe_info_t {
    /* one of AFE_INFO_VERSION_x */
    uint32_t version;

    float afe_full_scale_input_range; 
    int dac_unit_gain_output_change_step; 
    uint8_t cal_analog_offset_zero; 
    uint8_t cal_analog_offset_max;  

    /* davep 19-Dec-2012 ; Strange/odd/weird settings about the AFE. There is
     * one particular AFE vendor recommends using a gain != 1.0 during parts of
     * analog calibration. 
     */
    bool analog_cal_default_gain_not_null;
    float analog_cal_default_gain;

    /* In general while doing offset calibration calculation, unity gain should
       be used (gain = 1.0), so set this value to 1.0. But, if an AFE needs a different
       gain during offset calibration calculations, then put that here. */
    float gain_value_for_offset_cal;
    uint8_t cal_analog_offset_min;
    
};

/**
 * \brief Get the AFE-specific structure
 *
 * Returns the AFE-specific info structure to the caller
 *
 *
 * \param[out] scan_afe_info  the afe_info structure
 *
 */
void afe_get_afe_info( struct scan_afe_info_t **scan_afe_info );

/**
 * \brief  Convert a gain to an AFE PGA gain value for the AFE
 *
 * Compute the PGA 8 bit gain value.
 * The AFE PGA gain is the gain value written to the AFE to produce the desired
 * output gain from the InputPixel to the OutputPixel, loosely:
 * AFE PGA = [(OutputPixel/InputPixel +/- some_AFE_specific_value) * another_AFE_specific_value]
 *
 * To find the analog gain formula for a specific AFE, consult the AFE-specific
 * documentation.
 *
 * \retval uint8_t the pga gain
 *
 */
uint8_t afe_gain_to_pga( uint32_t gain );

scan_err_t afe_onetime_init( void );
//void afe_setup(void);
scan_err_t afe_reset(void);
void afe_sleep( bool bsleep );
unsigned int afe_read(uint32_t afe_num, uint32_t reg);
void afe_write(uint32_t afe_num, uint32_t regnum, uint32_t data);

void afe_write_pga( uint32_t afe_num, uint8_t red_gain, uint8_t green_gain, uint8_t blue_gain );
void afe_write_dac( uint32_t afe_num, uint8_t red_dac, uint8_t green_dac, uint8_t blue_dac );

void afe_auto_setup(void);
void afe_auto_enable(bool on);

/* debug print all registers */
void afe_dump(void);

/* run read/write tests */
scan_err_t afe_test( void );

void afe_get_hardware_config( struct afe_hardware_config_t *afe_hw );

scan_err_t afe_soft_setup(void);

#endif

