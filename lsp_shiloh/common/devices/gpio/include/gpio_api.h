/*
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file gpio_api.h
 *
 * \brief Function prototypes and defines for the public accessing and manipulation 
 * of _G_eneral _P_urpose _I_nput/_O_utputs (GPIOs). 
 * 
 * The gpio driver makes the gpio pins each appear as a single entity.  Those
 * pins can then be used as an input or output. This API also allows for
 * setting up an interrupt on a GPIO, changing the output value on the
 * physical pin and reading pins that are configured for input.
 *
 */

#ifndef __GPIO_API_H
#define __GPIO_API_H

#include <stdint.h>
#include "error_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/* 
 *
 * API Data Structures
 *
 */


/** \brief A GPIO device handle.  The handle is used to identify a particular
 * pin within the system and should be used as a parameter to each GPIO
 * function.  * See note below regarding how to acquire a handle pointer */
typedef struct gpio_handle_s gpio_handle_t;

/**
 * \brief This enumeration is a listing for all possible banks that the API
 * can handle.
 *
 * The number of banks is platform-specific and care should be taken to
 * make sure the user of this API does not use a bank value above the system's
 * limit.  If a non-existent bank on a platform is attempted to be used the
 * caller will cause an ASSERTION failure.  Typically a "bank" configuration
 * for a pin or signal is located in a module or device driver's
 * "configuration layer"
 *
 * If a platform has no banks or a single bank, then use GPIO_BANK_A for
 * all calls to gpio_open()
 */
typedef enum gpio_bank_e
{
    GPIO_BANK_A,
    GPIO_BANK_B,
    GPIO_BANK_C,
    GPIO_BANK_D,
    GPIO_BANK_E,
    GPIO_BANK_F,
    GPIO_BANK_G,
    GPIO_BANK_H,
    GPIO_BANK_I,
    GPIO_BANK_J,
    GPIO_BANK_K,
    GPIO_BANK_L,
    GPIO_BANK_M,
    GPIO_BANK_N,
    GPIO_BANK_O,
    GPIO_BANK_P,
    GPIO_BANK_Q,
    GPIO_BANK_R,
    GPIO_BANK_S,
    GPIO_BANK_T,
    GPIO_BANK_U,
    GPIO_BANK_V,
    GPIO_BANK_W,
    GPIO_BANK_X,
    GPIO_BANK_Y,
    GPIO_BANK_Z,
} gpio_bank_t;

/**
 *
 * \brief Enumeration used to set and get the direction of a pin
 * Pins configured for input can be used to read signals from a device
 * external to the system.  Pins configured for output can be used to
 * drive signals to a device external to the system.
 *
 */
typedef enum gpio_direction_e
{
    GPIO_DIRECTION_INPUT,  /*!< The GPIO pin will be configured as an input */

    GPIO_DIRECTION_OUTPUT, /*!< The GPIO pin will be configured as an output */

} gpio_direction_t;

/**
 *
 * \brief This enumeration allows the user of a GPIO pin to invert the meaning
 * of the physical voltage level of either the input or output. 
 *
 * Using the table below and the "gpio_logic_setting_t" enumeration in the
 * configuration layer of various modules we can ensure that user-level
 * firmware never has to worry about the physical level of a pin, but only
 * the "logical" level (asserted or deasserted, on or off).
 *
 * |------------------------|--------------------------------|--------------------------|
 * | physical voltage level | gpio_logic_setting_t           | logical voltage          |
 * |------------------------|--------------------------------|--------------------------|
 * | high (1,8v, 3v, etc.)  | GPIO_LOGIC_SETTING_ACTIVE_HIGH | 1 - logically asserted   |
 * | high (1,8v, 3v, etc.)  | GPIO_LOGIC_SETTING_ACTIVE_LOW  | 0 - logically deasserted |
 * | low (0v)               | GPIO_LOGIC_SETTING_ACTIVE_HIGH | 0 - logically deasserted |
 * | low (0v)               | GPIO_LOGIC_SETTING_ACTIVE_LOW  | 1 - logically asserted   |
 * |------------------------|--------------------------------|--------------------------|
 *
 */
typedef enum gpio_logic_setting_e
{
    GPIO_LOGIC_SETTING_ACTIVE_HIGH=0x0, /*!< A high physical voltage corresponds to
                                      a _HIGH_ logical voltage */

    GPIO_LOGIC_SETTING_ACTIVE_LOW=0x1 /*!< A high physical voltage corresponds to
                                    a _LOW_ logical voltage */

} gpio_logic_setting_t;

/**
 *
 * \brief This enumeration defines the interrupt types that this API supports
 * This enumeration will be used as a parameter to the gpio_isr_atach()
 * function
 *
 */
typedef enum gpio_isr_type_e
{
    GPIO_ISR_TYPE_FALLING_EDGE, /*!< A falling edge interrupt is desired */

    GPIO_ISR_TYPE_RISING_EDGE,  /*!< A rising edge interrupt is desired */

    GPIO_ISR_TYPE_EITHER_EDGE,  /*!< An interrupt on both falling and rising
                                  edge is desired */

    GPIO_ISR_TYPE_LEVEL_LOW,    /*!< An interrupt whenever the level is low */

    GPIO_ISR_TYPE_LEVEL_HIGH,   /*!< An interrupt whenever the level is high */

} gpio_isr_type_t;


/**
 *
 * \brief typedef defines a callback to be executed when the desired ISR is 
 * executed.  The callback parameter used for the gpio_isr_attach function 
 * must be of this type.  When the ISR fires, the logical level that the pin
 * was at during the interrupt is passed to the handler.
 *
 * \note you may not call gpio_write() within the ISR callback
 *
 * \param[in] pin A valid gpio_handle_t structure pointer that represents the
 * pin whose input triggered the interrupt
 *
 * \param[in] logic_level The state of the pin when the interrupt occured, either
 * logically deasserted (0) or logically asserted (1)
 *
 */
typedef void (*gpio_isr_t)( gpio_handle_t * pin, uint32_t logic_level, void * isr_callback_data );


/* 
 *
 * API Functions
 *
 */


/**
 *
 * \brief Initializes the GPIO internal structures and driver interface. 
 *
 */
void gpio_init( void );

/**
 *
 * \brief Acquires, configures and reserves a GPIO pin/bit for further use.
 *
 * This should be the first function call used when attempting to manipulate
 * GPIO pins in the system.
 * 
 * In general, users of this function call should
 * place the configuration information related to the bank, pin number, and
 * other initial settings in its configuration layer.  In this way
 * code using a GPIO will not have to change, except to specify changes
 * in GPIO locations or initial settings.
 *
 * \param[out] pin A pointer to a location that receives a valid
 * gpio_handle_t pointer
 *
 * \param[in] bank The gpio_bank_t associated with the pin desired to open
 * (A valid gpio_bank_t enum value)
 *
 * \param[in] number An integer value representing the pin within the bank.
 * This value has an index of 0, so the first pin in a system or bank is pin 0
 *
 * \param[in] initial_direction The initial direction setting, a valid
 * gpio_direction_t value
 *
 * \param[in] initial_logic_setting The initial logic setting, a valid
 * gpio_logic_setting_t value
 *
 * \param[in] initial_logic_level The initial logic level
 * The only valid values are:
 * 0 == logicaly deasserted
 * 1 == logically asserted
 *
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL upon failure
 *
 **/
error_type_t gpio_open( gpio_handle_t ** pin,
                        gpio_bank_t bank,
                        uint32_t number,
                        gpio_direction_t initial_direction,
                        gpio_logic_setting_t initial_logic_setting,
                        uint32_t initial_logic_level );


/**
 *
 * \brief Closes the gpio pin and frees it so that another module may open it
 *
 * \param[in] pin A pointer to a location that contains a handle representing a
 * valid GPIO
 * 
 * \return error_type_t
 * \retval OK upon success 
 * \retval FAIL upon failure 
 *
 * \par These situations will cause \a gpio_close() to return \e FAIL
 * \n The Handle is invalid
 */
error_type_t gpio_close( gpio_handle_t * pin );


/**
 *
 * \brief Asserts or deasserts the pin logically, sets the pin's output
 * logically  either "high" or "low". Please see the table related to the
 * gpio_logic_setting_t enum for more information
 *
 * \param[in] pin A pointer to a location that contains a handle
 * representing a valid GPIO (* see note above)
 *
 * \param[in] logic_level The logic level to drive the GPIO: 
 * logically asserted (1) or logically deasserted (0)
 *
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL upon failure
 *
 * \par These situations will cause \a gpio_write() to return \e FAIL
 * \n The Handle is invalid
 * \n The GPIO indicated by the handle is not configured for output.
 * 
 */
error_type_t gpio_write( gpio_handle_t * pin,
                         uint32_t logic_level );


/**
 *
 * \brief Reads the GPIO's logical value
 *
 * Please see the table related to the gpio_logic_setting_t enum for more
 * information
 *
 * \param[in] pin A pointer to a location that contains a handle representing a
 * valid GPIO (* see note above)
 *
 * \param[out] logic_level A pointer to a location that receives a valid
 * uint32_t value representing the logical state of the input pin:
 * 0 == logically deasserted
 * 1 == logically asserted
 *
 * \return error_type_t
 * \retval OK upon succes
 * \retval FAIL otherwise
 *
 * \note This function may only be called by pins that have been configured
 * for input
 *
 */
error_type_t gpio_read( gpio_handle_t * pin,
                        uint32_t * logic_level );


/**
 *
 * \brief Configures the GPIO for either input or output depending on the
 * direction desired
 * 
 * \param[in] pin A pointer to a location that contains a handle representing a
 * valid GPIO (* see note above)
 *
 * \param[in] direction indicates the desired direction, a valid gpio_direction_t
 * value either input or output
 *
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL upon failure 
 *
 * \par These situations will cause \a gpio_set_direction() to return \e FAIL
 * \n The Handle is invalid
 * \n The Direction selected is not valid.
 * 
 */
error_type_t gpio_set_direction( gpio_handle_t * pin,
                                 gpio_direction_t direction);


/**
 *
 * \brief attaches an ISR to a GPIO
 *
 * Allows calling systems to attach a ISR to the GPIO. Note this ISR is
 * disabled by default until gpio_isr_enable() is called.
 *
 * \param[in] pin A pointer to a location that contains the gpio handle value
 * (* see note above)
 *
 * \param[in] isr_callback callback to be executed when the ISR occurs.
 *
 * \param[in] isr_time_ns This parameter represents the amount of time in
 * nanoseconds used to "debounce" or "deglitch" the input signal before
 * issuing a callback to the isr_callback.  The two terms, "debounce" and 
 * "deglitch" are used synonymously here.  The specific driver implementation
 * will determine the best fit based on the hardware underneat the API.
 *
 *   - 0 indicates the interrupt is synchronous (or instantaneous)
 *
 *   - The deglitch time is platform specific.  The value also may not match
 *   the actual deglitch time used in practice.  The driver may adjust the
 *   time up if the hardware does not support the smaller time.
 * 
 * \param[in] isr_type A valid gpio_isr_type_t enum value to indicate what type
 * of ISR needed
 * 
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL otherwise
 *
 * \note In some cases not all interrupt types are supported on every pin
 * For those cases, FAIL will be returned and you should refer to the
 * configuration layer or the HW documentation.
 * 
 * \par These situations will cause \a gpio_isr_attach() to return \e FAIL
 * \n The Handle is invalid
 * \n The hardware does not support an interrupt of the type specified on
 * the pin
 * 
 */
error_type_t gpio_isr_attach( gpio_handle_t * pin,
                              gpio_isr_t isr_callback,
                              void * isr_callback_data,
                              uint32_t isr_time_ns,
                              gpio_isr_type_t isr_type );


/**
 * 
 * \brief detach an ISR from a GPIO
 * 
 * \param[in] pin A pointer to a location that contains the handle value
 * (* see note above)
 * 
 * \return error_type_t
 * \retval OK upon success
 * \retval FAIL upon failure 
 * 
 * \par These situations will cause \a gpio_isr_detach() to return \e FAIL
 * \n The Handle is invalid
 * 
 * \note the ISR is disabled after calling this function.
 *
 */
error_type_t gpio_isr_detach( gpio_handle_t * pin );


/**
 * 
 * \brief Enables the previously attached ISR for a GPIO
 * 
 * \param[in] pin A pointer to a location that contains the handle value
 * (* see note above)
 * 
 * \return error_type_t
 * \retval OK upon success 
 * \retval FAIL upon failure
 * 
 * \par These situations will cause \a gpio_isr_enable() to return \e FAIL
 * \n The Handle is invalid
 *
 * \warning you must first call gpio_isr_attach() before attempting to use this
 * function 
 * 
 */
error_type_t gpio_isr_enable( gpio_handle_t * pin );


/**
 * 
 * \brief Disables the previously attached ISR for a GPIO
 * 
 * \param[in] pin A pointer to a location that contains the handle value
 * (* see note above)
 * 
 * \return error_type_t
 * \retval OK upon success 
 * \retval FAIL upon failure
 * 
 * \par These situations will cause \a gpio_isr_disable() to return \e FAIL
 * \n The Handle is invalid
 * 
 * \warning you must first call gpio_isr_attach() before attempting to use this
 * function 
 *
 */
error_type_t gpio_isr_disable( gpio_handle_t * pin );

/**
 * @brief ack pin interupt
 * 
 * @param[in] pin A pointer to a location that contains the handle value
 * 
 * @return error_type_t OK upon success, FAIL upon failure
 */

error_type_t gpio_isr_ack( gpio_handle_t * pin );

#ifdef __cplusplus
}
#endif

#endif

