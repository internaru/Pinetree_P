#ifndef DEC_ADC_API_H
#define DEC_ADC_API_H

/* 
 *
 * ============================================================================
 * Copyright (c) 2010-2010  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

/**
 *
 * \file dec_adc_api.h
 *
 */

#include <stdint.h>
#include "key_table.h"

typedef struct dec_adc_s dec_adc_t;

/**
 *
 *     \brief initializes the dec adc device internal structures
 *      and driver interface.
 *
 */

void dec_adc_init(void);

/**
 *
 *   \brief returns the raw adc value of the adc requested
 *   channel.
 *   
 *
 *   \param adc[in] adc channel to read current value.
 *
 *   \returns uint32_t last read adc value (raw) of adc
 *   location. how often it is updated is configurable at
 *   compile time.
 */


uint32_t dec_adc_raw_read_channel(const dec_adc_t *adc);
/**
 * 
 *   \brief returns the scaled value of the requested adc
 *   channel.
 * 
 *   \param adc[in] adc channel to read current value.
 * 
 *    \returns uint32_t last read adc value (scaled) of adc
 *    location. how often it is updated is configurable at
 *    compile time.
 * 
 */

uint32_t dec_adc_scaled_read_channel_mV(const dec_adc_t *adc);

/**
 * 
 *   \brief returns the pin number of the requested adc
 *   channel.
 * 
 *   \param adc[in] adc channel to read current value.
 * 
 *    \returns uint32_t pin number of adc
 *    location. 
 * 
 */
uint32_t dec_adc_channel_pin_number(dec_adc_t *adc); 

/**
 * 
 *                  (external_voltage_max_mV)(adc_ref_max_voltage_mV)
 * scaleVoltage(mV)=------------------------------------------*adc_reading
 *                  (adc_max_reading)(pin_input_max_mV)
 * 
 *  Care should be taken that neither
 *  (pin_max_voltage_mV)*(adc_ref_max_voltage_mV)*(adc_reading)
 *  or (adc_max_reading)(signal_max_voltage_mV) is greater than
 *  MAX_INT.
 * 
 * 
 */
/**
 *  \brief returns the ADCs internal values.
 * 
 *  \param [out] uint32 * adc_max_reading maximum raw reading
 *  from an adc channel.
 * 
 *  \param [out] uint32_t * adc_ref_max_voltage_mV Reference
 *  voltage (in mV) used to do the a->d coversion.
 *
 */

void dec_adc_get_adc_chip_parameters(uint32_t * adc_max_reading, 
                                     uint32_t * adc_ref_max_voltage_mV);

/**
 * \brief gets the currently set external & pin input voltage
 * for a adc channel.  Which is used to calculate scaled value.
 * 
 * \param adc[in] adc channel to read current value.
 * 
 * \param external_voltage_max_mV [out] Currently set
 * external_voltage_max_mV.  (See above).
 * 
 * \param pin_input_max_mV [out]  Currently set pin_input_max_mV
 * (See above).
 */
void dec_adc_get_scale_factor(const dec_adc_t * adc, 
                              uint32_t * pin_input_max_mV,
                              uint32_t * external_voltage_max_mV);

/**
 * \brief Sets scale factor values for adc channel.
 * 
 * \param adc[in/out] adc channel to set current values.
 * 
 * \param external_voltage_max_mV [in] Sets
 * external_voltage_max_mV for adc channel.  (See above).
 * 
 * \param pin_input_max_mV [out]  Sets pin_input_max_mV for adc
 * channel (See above). 
 *  
 * \param pin_input_max_mV [out]  Specifys the adc reference 
 *        voltage.
 *  
 */
void dec_adc_set_scale_factor(dec_adc_t * adc,
                              uint32_t pin_input_max_mV,
                              uint32_t external_voltage_max_mV,
                              uint32_t reference_voltage);


/// look in adc_XXXXX_config.[ch] for implementation.
/// int key - string - object mapping, from key_table.h
KEY_TABLE_DECLARE_PUBLIC_2( dec_adc, dec_adc_t );

#endif // DEC_ADC_API_H
