/*
 *
 * ============================================================================
 * Copyright (c) 2012   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file mrvl_gpio.c
 *
 * \brief This file implements the gpio_api.h file for the Marvell
 * GPIO HW block. The driver doesn't use banks. Instead it uses a flat
 * numbering scheme (0 - 127) to select a specific GPIO.
 *
 * See the API for more information on usage.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include "error_types.h"
#include "gpio_api.h"
#include "GPIO_regstructs.h"
#include "GPIO_regmasks.h"
#include "mrvl_gpio_linux_config.h"
#include "memAPI.h"
#include "lassert.h"
#include "dprintf.h"
#include "utils.h"
//#include "interrupt_api.h"
#include "logger.h"
#include "devices_logger.h"
#include "intnums.h"
#include "pthread.h"
#include "posix_ostools.h"
#include "ATypes.h"


/* Module level portion (specific to each module) */
#define DBG_PRFX "GPIO: "                      // simple string prepend to the front of all strings.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | DEVICES_LOGGER_SUBMOD_GPIO_LOGGER

/* Not using any config for now*/

#if defined(HAVE_CMD)
#include "cmd_proc_api.h"
#endif

/** \brief A unique identifier for each GPIO handle to validate external
 * handle input */
#define GPIO_INTERNAL_HANDLE_COOKIE 0x4750494f

/* Uncomment this to enable gpio commands */
#define GPIO_DEBUG_ENABLE 1

/** \brief Information passed along related to each pin's state */
struct gpio_handle_s
{
    uint32_t cookie;                                  /*!< A unique identifier for each GPIO handle to validate external handle input */

    int gpio_pin_fd;                                  /*!< File Descriptor for the GPIO pin                                           */

    gpio_isr_type_t isr_type;                         /*!< Edge-trigger setting for this pin                                          */

    uint32_t index;                                   /*!< The pin's index location in the group                                       */

    gpio_isr_t isr_callback;                          /*!< The pin's ISR callback, when registered for interrupts                     */

    void * isr_callback_data;                         /*!< The pin's callback_data ISR*/
};

/** \brief The GPIO platform configuration data */
static gpio_handle_t * pin_handle_list[NUM_GPIO_PINS] = {0};

pthread_mutex_t gpio_lock = PTHREAD_MUTEX_INITIALIZER;

static bool gpio_drv_is_initialized = false;

/**
 *
 * \brief Drive the gpio output register based on the pin's
 * configuration and the logic_level desired
 *
 * \param pin A valid, open gpio_handle_t
 *
 * \param logic_level a valid logic level, 1==asserted or 0==deasserted
 *
 * \return OK upon success, otherwise FAIL
 *
 */
static error_type_t gpio_internal_pin_output_set( gpio_handle_t * pin,
        uint32_t logic_level );


/**
 *
 * \brief Configure the gpio direction register to the desired level for
 * the pin specified
 *
 * \param pin A valid, open gpio_handle_t
 *
 * \param direction a valid gpio_direction_t, input or output
 *
 * \return OK upon success, otherwise FAIL
 *
 */
static error_type_t gpio_internal_pin_direction_set( gpio_handle_t * pin,
                                                     gpio_direction_t direction
                                                   );

/**
 *
 * \brief Configure the gpio logic setting (active high / active low) to the desired level for
 * the pin specified
 *
 * \param pin A valid, open gpio_handle_t
 *
 * \param logic a valid gpio_logic_setting_t
 *
 * \return OK upon success, otherwise FAIL
 *
 */
static error_type_t gpio_internal_pin_logic_set( gpio_handle_t * pin,
                                                     gpio_logic_setting_t logic
                                                   );


/**
 *
 * \brief Internally, get the pin's current logical level by reading the
 * pin's physical level
 *
 * \param pin A valid, open gpio_handle_t
 *
 * \param logic_level A valid pointer to a memory space large enough to hold
 * a uint32_t value
 *
 * \return OK upon success, otherwise FAIL
 *
 */
static error_type_t gpio_internal_pin_input_get( gpio_handle_t * pin,
       uint32_t * logic_level );


static error_type_t gpio_internal_pin_isr_trigger_set( gpio_handle_t * pin, const char* isr_trigger);


#if 0
/**
 *
 * \brief ISR handler for all interrupts
 *
 * \param input An internal bank pointer represented by a uint32_t
 *
 */
void gpio_internal_isr( uint32_t input );
#endif

#if defined(HAVE_CMD) && defined(GPIO_DEBUG_ENABLE)
static void gpio_debug_init ( void );
#endif

#define GPIO_STACK_SIZE     POSIX_MIN_STACK_SIZE
static pthread_t            gpio_thd_id;
static unsigned char        GPIOStack[GPIO_STACK_SIZE]ALIGN8;
static int gpio_epfd = -1;
#define MAX_EVENTS 10
#define MAX_BUF 64
void *GPIOIsrThread(void *unused)
{
    struct epoll_event events[MAX_EVENTS];
    char buf[MAX_BUF];

    while (1)
    {
        int nfds = epoll_wait(gpio_epfd, events, MAX_EVENTS, -1);
        if (nfds < 0)
        {
            if (errno != EINTR)
            {
                posix_sleep_ms(500);
            }
        }
        else
        {
            int i;
            for (i = 0; i < nfds; i++) 
            {
                gpio_handle_t *pin = events[i].data.ptr;
                
                ASSERT(pin);
                
                if (events[i].events & EPOLLPRI)
                {
                    read(pin->gpio_pin_fd, &buf, MAX_BUF);
                    
                    /* Make sure pin is open and a callback is registered */
                    if ( NULL != pin->isr_callback )
                    {
                        error_type_t e_res = 0; UNUSED_VAR( e_res ); // lol: read register failed in release
                        uint32_t logic_level;
                        
                        /* get the pin's logical voltage level */
                        e_res = gpio_internal_pin_input_get( pin,
                                &logic_level );
                        DBG_ASSERT( OK == e_res );

                        /* make the callback, passing the logical voltage level as a
                         * parameter */
                        pin->isr_callback( pin, logic_level, pin->isr_callback_data );
                    }
                }
            }
        }
    }
    return 0;
}


/*
 * PUBLIC API FUNCTION IMPLEMENTATION
 */

void gpio_init( void )
{
    int px_status;
    
    DBG_PRINTF_NOTICE("%s\n", __func__);

    // make sure init only called once
    ASSERT(!gpio_drv_is_initialized);
    if(gpio_drv_is_initialized)
    {
        return;
    }

    gpio_drv_is_initialized = true;

    // create pin ISR thread
    gpio_epfd = epoll_create(1);
    XASSERT(gpio_epfd != -1, errno);
    
	px_status = posix_create_thread( &gpio_thd_id, 
                                  GPIOIsrThread, 
                                  0, 
                                  "GPIO_isr_thread",
                                  GPIOStack, 
                                  GPIO_STACK_SIZE, 
                                  (POSIX_THR_PRI_ISR));
    XASSERT( px_status==0, px_status );

#if defined(HAVE_CMD) && defined(GPIO_DEBUG_ENABLE)
    gpio_debug_init();
#endif
}

#define GPIO_SYS_PATH "/sys/class/gpio"
#define GPIO_SYS_PIN_PATH GPIO_SYS_PATH "/gpio%d"
#define MAX_GPIO_BUF 64

static void assert_pin_valid(uint32_t pin_index)
{
    ASSERT(pin_index < NUM_GPIO_PINS);
    ASSERT(pin_handle_list[pin_index] != NULL);
    ASSERT(pin_handle_list[pin_index]->index == pin_index);
    ASSERT(pin_handle_list[pin_index]->cookie == GPIO_INTERNAL_HANDLE_COOKIE);
}

static int gpio_set_export(uint32_t pin_index)
{
    int fd;
    char buf[MAX_GPIO_BUF];
    int len;

    fd = open(GPIO_SYS_PATH "/export", O_WRONLY);
    DBG_ASSERT(fd >= 0);
    len = snprintf(buf, MAX_GPIO_BUF, "%d", pin_index);
    write(fd, buf, len);
    close(fd);

    // verify success
    len = snprintf(buf, MAX_GPIO_BUF, GPIO_SYS_PIN_PATH "/value", pin_index);
    fd = open(buf, O_RDWR);
    if(fd < 0)
    {
        DBG_PRINTF_ERR("%s: failed\n", __func__);
        DBG_ASSERT(!(fd < 0));
        return -errno;
    }
    close(fd);
    return 0;
}

static int gpio_clear_export(uint32_t pin_index)
{
    int fd;
    char buf[MAX_GPIO_BUF];
    int len;

    assert_pin_valid(pin_index);

    fd = open(GPIO_SYS_PATH "/unexport", O_WRONLY);
    DBG_ASSERT(fd >= 0);
    len = snprintf(buf, MAX_GPIO_BUF, "%d", pin_index);
    write(fd, buf, len);
    close(fd);
    return 0;
}

static int gpio_open_pin_value(pin_index)
{
    char buf[MAX_GPIO_BUF];
    // int len;

    assert_pin_valid(pin_index);
    DBG_ASSERT(pin_handle_list[pin_index]->gpio_pin_fd == -1);

    // len = 
    snprintf(buf, MAX_GPIO_BUF, GPIO_SYS_PIN_PATH "/value", pin_index);
    pin_handle_list[pin_index]->gpio_pin_fd = open(buf, O_RDWR | O_CLOEXEC);
    DBG_ASSERT(pin_handle_list[pin_index]->gpio_pin_fd >= 0);

    return (pin_handle_list[pin_index]->gpio_pin_fd >= 0) ? 0 : -errno;
}

static __inline__ char bank_to_letter(gpio_bank_t bank)
{
    return 'A'+(int)bank;
}

static __inline__ uint32_t bank_pin_to_index(gpio_bank_t bank, uint32_t pin)
{
    return (((uint32_t)bank * NUM_GPIO_PINS_PER_BANK) + pin);
}

error_type_t gpio_open( gpio_handle_t ** handle,
        gpio_bank_t bank_id,
        uint32_t number,
        gpio_direction_t initial_direction,
        gpio_logic_setting_t initial_logic_setting,
        uint32_t initial_logic_level )
{
    error_type_t e_res;
    uint32_t index = bank_pin_to_index(bank_id, number);
    int lock_result;

    DBG_PRINTF_NOTICE("%s Bank%c num %d -> index %d\n", __func__, bank_to_letter(bank_id), number, index);

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    ASSERT(index < NUM_GPIO_PINS);
    
    /* verify handle */
    DBG_ASSERT( NULL != handle );

    /* verify other parameters */
    DBG_ASSERT( ( GPIO_DIRECTION_INPUT == initial_direction ) ||
            ( GPIO_DIRECTION_OUTPUT == initial_direction ) );

    DBG_ASSERT( ( GPIO_LOGIC_SETTING_ACTIVE_HIGH == initial_logic_setting ) ||
            ( GPIO_LOGIC_SETTING_ACTIVE_LOW == initial_logic_setting ) );

    DBG_ASSERT( ( 0 == initial_logic_level ) ||
            ( 1 == initial_logic_level ) );

    e_res = OK;
    *handle = NULL;

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    if(pin_handle_list[index] != NULL)
    {
        // pin already open; fail
        DBG_PRINTF_ERR("%s Pin index %d already open\n", index);
        e_res = -EINVAL;
        goto EXIT;
    }

    pin_handle_list[index] = (gpio_handle_t*)MEM_CALLOC(sizeof(gpio_handle_t), 1);
    DBG_ASSERT(pin_handle_list[index]);
    if(pin_handle_list[index] == NULL)
    {
        // pin already open; fail
        DBG_PRINTF_ERR("%s ERROR: no memory\n", index);
        e_res = -ENOMEM;
        goto EXIT;
    }

    pin_handle_list[index]->cookie = GPIO_INTERNAL_HANDLE_COOKIE;
    pin_handle_list[index]->gpio_pin_fd = -1;
    pin_handle_list[index]->index = index;

    // export pin from kernel
    if((e_res = gpio_set_export(index)) != 0)
    {
        //free memory
        pin_handle_list[index]->cookie = 0;
        MEM_FREE_AND_NULL(pin_handle_list[index]);
        goto EXIT;
    }

    // open the pin
    if((e_res = gpio_open_pin_value(index)) != 0)
    {
        //unexport and free memory
        pin_handle_list[index]->cookie = 0;
        gpio_clear_export(index);
        MEM_FREE_AND_NULL(pin_handle_list[index]);
        goto EXIT;
    }

    /* Set the pin direction */
    gpio_internal_pin_direction_set( pin_handle_list[index], initial_direction );

    /* Set the logic setting */
    gpio_internal_pin_logic_set( pin_handle_list[index], initial_logic_setting);

    /* drive the pin to the desired initial state */
    gpio_internal_pin_output_set( pin_handle_list[index], initial_logic_level );

    /* disable isr */
    gpio_internal_pin_isr_trigger_set(pin_handle_list[index], "none");

    /* Assign the handle */
    *handle = pin_handle_list[index];

EXIT:
    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}


error_type_t gpio_close( gpio_handle_t * pin )
{
    error_type_t e_res = OK;
    int lock_result;

    /* verify pin handle, make sure it was opened */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    // TODO: disable isr if any
    close(pin->gpio_pin_fd);                            // close the handle to the pin value
    gpio_clear_export(pin->index);                      // pin no longer exported to user space
    pin->cookie = 0;
    MEM_FREE_AND_NULL(pin_handle_list[pin->index]);     // free the pin handle struct

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}


error_type_t gpio_write( gpio_handle_t * pin, uint32_t logic_level )
{
    error_type_t e_res;

    /* verify pin */
    DBG_ASSERT( NULL != pin );

    e_res = gpio_internal_pin_output_set( pin, logic_level );

    return e_res;
}


error_type_t gpio_read( gpio_handle_t * pin,
        uint32_t * logic_level )
{
    error_type_t e_res = FAIL;

    /* verify pin */
    DBG_ASSERT( NULL != pin );

    e_res = gpio_internal_pin_input_get( pin, logic_level );

    return e_res;
}


error_type_t gpio_set_direction( gpio_handle_t * pin,
        gpio_direction_t direction
                                 )
{
    error_type_t e_res = FAIL;

    /* verify pin */
    DBG_ASSERT( NULL != pin );

    e_res = gpio_internal_pin_direction_set( pin, direction );

    return e_res;
}


error_type_t gpio_isr_attach( gpio_handle_t * pin,
                              gpio_isr_t isr_callback,
                              void * isr_callback_data,
                              uint32_t isr_time_ns,
                              gpio_isr_type_t isr_type )
{
    error_type_t e_res = OK;
    int lock_result;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT( NULL != pin );
    assert_pin_valid(pin->index);

    /* verify other parameters */
    DBG_ASSERT( NULL != isr_callback );
    DBG_ASSERT( ( GPIO_ISR_TYPE_FALLING_EDGE == isr_type ) ||
            ( GPIO_ISR_TYPE_RISING_EDGE == isr_type ) ||
            ( GPIO_ISR_TYPE_EITHER_EDGE == isr_type ) );

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    /* Make sure no ISR is attached */
    DBG_ASSERT( NULL == pin->isr_callback );

    /* Assign the edge sensitivity setting, to be used when enabling interrupt */
    pin->isr_type = isr_type;

    /* Set pin direction to input */
    if ( OK == e_res )
    {
        e_res = gpio_internal_pin_direction_set( pin, GPIO_DIRECTION_INPUT );
    }

    if ( OK == e_res )
    {
        struct epoll_event ev;
        
        /* save the callback */
        pin->isr_callback = isr_callback;
        pin->isr_callback_data = isr_callback_data;
        
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLPRI;
        ev.data.ptr = pin;
        
        if (epoll_ctl(gpio_epfd, EPOLL_CTL_ADD, pin->gpio_pin_fd, &ev) != 0)
        {
            e_res = FAIL;
        }
    }

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}


error_type_t gpio_isr_detach( gpio_handle_t * pin )
{
    error_type_t e_res;
    int lock_result;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT( NULL != pin );
    assert_pin_valid(pin->index);

    /* Make sure an ISR was actually attached */
    DBG_ASSERT( NULL != pin->isr_callback );

    /* interrupt should be disabled */
    gpio_internal_pin_isr_trigger_set(pin_handle_list[pin->index], "none");

    /* Set to input - is this needed? */
    e_res = gpio_internal_pin_direction_set( pin, GPIO_DIRECTION_INPUT );

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    if ( OK == e_res )
    {
        struct epoll_event ev;
        
        memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLPRI;
        ev.data.ptr = pin;
        
        if (epoll_ctl(gpio_epfd, EPOLL_CTL_DEL, pin->gpio_pin_fd, &ev) == 0)
        {
            e_res = FAIL;
        }
    }
    
    /* Clear the ISR callback and data */
    pin->isr_callback = NULL;
    pin->isr_callback_data = NULL;

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}


error_type_t gpio_isr_enable( gpio_handle_t * pin )
{
    error_type_t e_res = FAIL;
    int lock_result;
    char* isr_trigger;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT( NULL != pin );
    assert_pin_valid(pin->index);

    /* attach before calling enable */
    DBG_ASSERT( NULL != pin->isr_callback );

    switch (pin->isr_type)
    {
        case GPIO_ISR_TYPE_FALLING_EDGE: 
            isr_trigger = "falling";
            break;

        case GPIO_ISR_TYPE_RISING_EDGE:
            isr_trigger = "rising";
            break;

        case GPIO_ISR_TYPE_EITHER_EDGE:
            isr_trigger = "both";
            break;

        case GPIO_ISR_TYPE_LEVEL_LOW:
        case GPIO_ISR_TYPE_LEVEL_HIGH:
        default:
            DBG_PRINTF_ERR("%s Unsupported ISR Type\n", __func__);
            XASSERT(0, pin->isr_type);
            return FAIL;
    }

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    /* Set interrupt polarity */
    e_res = gpio_internal_pin_isr_trigger_set( pin, isr_trigger );

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}


error_type_t gpio_isr_disable( gpio_handle_t * pin )
{
    error_type_t e_res = FAIL;
    int lock_result;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT( NULL != pin );
    assert_pin_valid(pin->index);

    /* attach before calling disable */
    DBG_ASSERT( NULL != pin->isr_callback );

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    e_res = gpio_internal_pin_isr_trigger_set( pin, "none" );

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    return e_res;
}

error_type_t gpio_isr_ack( gpio_handle_t * pin )
{
    int lock_result;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT( NULL != pin );
    assert_pin_valid(pin->index);

    lock_result = pthread_mutex_lock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    /* ack interrupt */

    lock_result = pthread_mutex_unlock(&gpio_lock);
    XASSERT(lock_result == 0, lock_result);

    // -jrs I believe that driver automatically acks, but asserting and returning fail until we confirm.
    ASSERT(0);

    return FAIL;
}

/*
 * PRIVATE FUNCTIONS
 */


static error_type_t gpio_internal_pin_input_get( gpio_handle_t * pin,
       uint32_t * logic_level )
{
    char buf;
    int len;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);
    
    /* verify pin */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);
    DBG_ASSERT(pin->gpio_pin_fd >= 0);

    /* verify other parameters */
    DBG_ASSERT( NULL != logic_level );

    lseek(pin->gpio_pin_fd, 0, SEEK_SET);
    len = read(pin->gpio_pin_fd, &buf, 1);
    XASSERT(len == 1, len);

    *logic_level = (buf == '0') ? 0 : 1;

    return OK;
}

static error_type_t gpio_internal_pin_output_set( gpio_handle_t * pin,
        uint32_t logic_level )
{
    error_type_t e_res = OK;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);
    
    /* verify pin */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);
    DBG_ASSERT(pin->gpio_pin_fd >= 0);

    /* verify other parameters */
    DBG_ASSERT( ( 0 == logic_level ) ||
            ( 1 == logic_level ) );

    lseek(pin->gpio_pin_fd, 0, SEEK_SET);
    write(pin->gpio_pin_fd, (logic_level == 1) ? "1" : "0", 2);

    return e_res;
}


#if 0
void gpio_internal_isr( uint32_t input )
{
    gpio_handle_t * pin;
    uint32_t original_int_status;
    uint32_t current_int_status;
    uint32_t logic_level;
    uint32_t pin_index;
    uint32_t group_index;
    error_type_t e_res;
    GPIO_REGS_t * curr_gpio_reg = NULL;

    /* Check each GPIO block */
    for ( group_index = 0; group_index < NUM_GPIO_REG_GROUPS; group_index++ )
    {
        curr_gpio_reg = ( GPIO_REGS_t *) gpio_base_addrs[group_index];

        /* Read GPIO_EDR (Edge detect status register) for the block */
        current_int_status = original_int_status = ( curr_gpio_reg->GPIO_EDR & curr_gpio_reg->GPIO_APMASK_REG );

        /* get the pin index from the status register register
         * ffs returns > 0 for the the set bit (minus 1), 0 if non*/
        pin_index = ffs( current_int_status );
        while( pin_index != 0 )
        {
            pin_index = pin_index - 1;

            /* Get the pin handle from the index */
            pin = &pin_handle_list[( group_index * NUM_GPIO_PINS_PER_GRP ) + pin_index] ;

            /* Make sure pin is open and a callback is registered */
            if ( ( true == pin->is_open ) &&
                    ( NULL != pin->isr_callback ) )
            {
                /* get the pin's logical voltage level */
                e_res = gpio_internal_pin_input_get( pin,
                        &logic_level );
                DBG_ASSERT( OK == e_res );

                /* make the callback, passing the logical voltage level as a
                 * parameter */
                pin->isr_callback( pin, logic_level, pin->isr_callback_data );
            }

            /* clear this pin's int */
            current_int_status &= ~( 1 << pin_index );
            pin_index = ffs( current_int_status );
        }

        /* Ack the interrupt - clear the status register bit */
        curr_gpio_reg->GPIO_EDR = original_int_status;
    }
}
#endif


static error_type_t gpio_internal_pin_direction_set( gpio_handle_t * pin,
                                                     gpio_direction_t direction
                                                   )
{
    error_type_t e_res = OK;
    int fd;
    char buf[MAX_GPIO_BUF];
    int len;

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);

    /* verify other parameters */
    DBG_ASSERT( ( GPIO_DIRECTION_INPUT == direction ) ||
            ( GPIO_DIRECTION_OUTPUT == direction ) );


    len = snprintf(buf, MAX_GPIO_BUF, GPIO_SYS_PIN_PATH "/direction", pin->index);
    fd = open(buf, O_WRONLY);
    DBG_ASSERT(fd >= 0);

    len = snprintf(buf, MAX_GPIO_BUF, "%s", ( GPIO_DIRECTION_INPUT == direction ) ? "in" : "out");
    write(fd, buf, len);
    close(fd);

    return e_res;
}

static error_type_t gpio_internal_pin_logic_set( gpio_handle_t * pin,
                                                     gpio_logic_setting_t logic
                                                   )
{
    error_type_t e_res = OK;
    int fd;
    char buf[MAX_GPIO_BUF];

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);

    /* verify other parameters */
    DBG_ASSERT( ( GPIO_LOGIC_SETTING_ACTIVE_HIGH == logic ) ||
            ( GPIO_LOGIC_SETTING_ACTIVE_LOW == logic ) );


    snprintf(buf, MAX_GPIO_BUF, GPIO_SYS_PIN_PATH "/active_low", pin->index);
    fd = open(buf, O_WRONLY);
    DBG_ASSERT(fd >= 0);

    write(fd, ( GPIO_LOGIC_SETTING_ACTIVE_LOW == logic ) ? "1" : "0", 2);
    close(fd);

    return e_res;
}


/**
 *
 * \brief Set the GPIO's isr trigger.
 *
 * \param pin A valid, open gpio_handle_t
 *
 * \param isr_trigger - must be one of "none", "falling", "rising", or "both".
 *
 * \return OK upon success, otherwise FAIL
 *
 */
static error_type_t gpio_internal_pin_isr_trigger_set( gpio_handle_t * pin, const char* isr_trigger)
{
    error_type_t e_res = OK;
    int fd;
    char buf[MAX_GPIO_BUF];

    /* verify init has completed */
    DBG_ASSERT(gpio_drv_is_initialized);

    /* verify pin */
    DBG_ASSERT(pin);
    assert_pin_valid(pin->index);

    /* verify other parameters */
    DBG_ASSERT(isr_trigger);

    snprintf(buf, MAX_GPIO_BUF, GPIO_SYS_PIN_PATH "/edge", pin->index);
    fd = open(buf, O_WRONLY);
    DBG_ASSERT(fd >= 0);

    write(fd, isr_trigger, strnlen(isr_trigger, 32)+1);
    close(fd);

    return e_res;
}

#if defined(HAVE_CMD) && defined(GPIO_DEBUG_ENABLE)
gpio_handle_t * mygpios[NUM_GPIO_PINS];
volatile char trigger_flag[NUM_GPIO_PINS];

#define xstr(s) str(s)
#define str(s) #s

static void isr_callback ( gpio_handle_t * pin , uint32_t logic_level, void * isr_callback_data )
{
    uint32_t int_num = (uint32_t) isr_callback_data;

    if ( int_num < NUM_GPIO_PINS )
    {
        if ( logic_level == 0 )
        {
            trigger_flag[int_num] = 'l';
        }
        else if ( logic_level == 1 )
        {
            trigger_flag[int_num] = 'h';
        }
    }
}

static const char *gpio_cmd_open_desc = "Open a gpio pin";
static const char *gpio_cmd_open_usage = "<pin> <dir> <logic> <level>";
static const char *gpio_cmd_open_notes = "pin : 0 - " xstr(NUM_GPIO_PINS) ", dir: 0 (in), 1 (out), logic : 0 (active-low), 1 (active-high), level : 0 (inactive) - 1 (active)";
static int gpio_cmd_open_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    uint32_t dir;
    uint32_t level;
    uint32_t logic;
    gpio_logic_setting_t logic_setting;
    gpio_direction_t dir_setting;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 5 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( (retval == CMD_OK) && ( mygpios[pin_num] != NULL ) )
    {
        cmd_printf( "GPIO_CMD: pin %u is already open \n", pin_num );
        retval = CMD_ERROR;
    }

    /* get the direction setting */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[2], &dir );
        if ( ( 0 != i_res ) || ( dir > 1 ) )
        {
            retval = CMD_USAGE_ERROR;
        }
        else
        {
            dir_setting = ( dir == 1 ) ? GPIO_DIRECTION_OUTPUT : GPIO_DIRECTION_INPUT;
        }

    }

    /* get the logic setting */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[3], &logic );
        if ( ( 0 != i_res ) || ( logic > 1 ) )
        {
            retval = CMD_USAGE_ERROR;
        }

    }

    /* get the logic level */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[4], &level );
        if ( ( 0 != i_res ) || ( level > 1 ) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( retval == CMD_OK )
    {
        logic_setting = ( logic == 1 ) ? GPIO_LOGIC_SETTING_ACTIVE_HIGH : GPIO_LOGIC_SETTING_ACTIVE_LOW;

        /* Open the gpio */
        e_res = gpio_open ( &mygpios[pin_num],
                GPIO_BANK_A,
                pin_num,
                dir_setting,
                logic_setting,
                level );
        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot open gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}

static const char *gpio_cmd_close_desc = "Close an open gpio pin";
static const char *gpio_cmd_close_usage = "";
static const char *gpio_cmd_close_notes = "";
static int gpio_cmd_close_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    uint32_t pin_num;
    int32_t i_res;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 2 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( (retval == CMD_OK) && ( mygpios[pin_num] == NULL ) )
    {
        cmd_printf( "GPIO_CMD: pin %u is already closed \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        /* Close the gpio */
        e_res = gpio_close ( mygpios[pin_num] );
        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot close gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
        else
        {
            mygpios[pin_num] = NULL;
        }
    }

    return retval;
}

static const char *gpio_cmd_write_desc = "Write to a gpio pin";
static const char *gpio_cmd_write_usage = "<pin> <level>";
static const char *gpio_cmd_write_notes = "pin : 0 - " xstr(NUM_GPIO_PINS) ", level : 0 (inactive) - 1 (active)";
static int gpio_cmd_write_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    uint32_t level;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 3 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    /* get the logic level */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[2], &level );
        if ( ( 0 != i_res ) || ( level > 1 ) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( ( retval == CMD_OK ) && ( NULL == mygpios[pin_num] ) )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_write ( mygpios[pin_num],
                             level );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot write to gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}


static const char *gpio_cmd_read_desc = "Read from a gpio pin";
static const char *gpio_cmd_read_usage = "<pin>";
static const char *gpio_cmd_read_notes = "pin : 0 - "xstr(NUM_GPIO_PINS);
static int gpio_cmd_read_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    uint32_t level;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 2 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( ( retval == CMD_OK ) && ( NULL == mygpios[pin_num] ) )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_read ( mygpios[pin_num],
                            &level );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot read from gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
        else
        {
            if ( level == 0 )
            {
                cmd_printf( "GPIO_CMD: gpio %u - logic level 0 (NOT ASSERTED) \n", pin_num );
            }
            else
            {
                cmd_printf( "GPIO_CMD: gpio %u - logic level 1 (ASSERTED) \n", pin_num );
            }

        }
    }

    return retval;
}

static const char *gpio_cmd_int_attach_desc = "Set the GPIO into interrupt mode, attach an ISR";
static const char *gpio_cmd_int_attach_usage = "<pin> <edge-trigger>";
static const char *gpio_cmd_int_attach_notes = "pin : 0 - " xstr(NUM_GPIO_PINS) ", trigger - 0 (fall), 1 (rise), 2 (both)";
static int gpio_cmd_int_attach_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    uint32_t trigger;
    gpio_isr_type_t edge_trigger;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 3 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    /* Get the edge trigger */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[2], &trigger );
        if ( ( 0 != i_res ) || ( trigger > 2 ) )
        {
            retval = CMD_USAGE_ERROR;
        }
        else
        {
            switch( trigger )
            {
                case 0:
                {
                    edge_trigger = GPIO_ISR_TYPE_FALLING_EDGE;
                    break;
                }
                case 1:
                {
                    edge_trigger = GPIO_ISR_TYPE_RISING_EDGE;
                    break;
                }
                case 2:
                {
                    edge_trigger = GPIO_ISR_TYPE_EITHER_EDGE;
                    break;
                }
                default:
                {
                    edge_trigger = GPIO_ISR_TYPE_EITHER_EDGE;
                    break;
                }
            }
        }
    }

    if ( ( retval == CMD_OK ) && ( NULL == mygpios[pin_num] ) )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_isr_attach ( mygpios[pin_num],
                                  isr_callback,
                                  ( void * ) pin_num,
                                  0,
                                  edge_trigger );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot attach ISR to to gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}

static const char *gpio_cmd_int_detach_desc = "Detach an attached ISR ";
static const char *gpio_cmd_int_detach_usage = "<pin> ";
static const char *gpio_cmd_int_detach_notes = "pin : 0 - " xstr(NUM_GPIO_PINS);
static int gpio_cmd_int_detach_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 2 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( ( retval == CMD_OK ) && ( NULL == mygpios[pin_num] ) )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_isr_detach ( mygpios[pin_num] );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot detach ISR from gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}

static const char *gpio_cmd_int_enable_desc = "Enable a GPIO interrupt";
static const char *gpio_cmd_int_enable_usage = "<pin> ";
static const char *gpio_cmd_int_enable_notes = "pin = 0 - " xstr(NUM_GPIO_PINS);
static int gpio_cmd_int_enable_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 2 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( ( retval == CMD_OK ) && NULL == mygpios[pin_num] )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_isr_enable ( mygpios[pin_num] );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot enable interrupt on gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}

static const char *gpio_cmd_int_disable_desc = "Disable a GPIO interrupt";
static const char *gpio_cmd_int_disable_usage = "<pin>";
static const char *gpio_cmd_int_disable_notes = "pin = 0 - " xstr(NUM_GPIO_PINS);
static int gpio_cmd_int_disable_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    error_type_t e_res;

    /* Check the args */
    if ( argc != 2 )
    {
        retval = CMD_USAGE_ERROR;
    }

    /* Get the pin number */
    if ( retval == CMD_OK )
    {
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num >= NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }
    }

    if ( ( retval == CMD_OK ) && ( NULL == mygpios[pin_num] ) )
    {
        cmd_printf( "GPIO_CMD: gpio %u is not open! \n", pin_num );
        retval = CMD_ERROR;
    }

    if ( retval == CMD_OK )
    {
        e_res = gpio_isr_disable ( mygpios[pin_num] );

        if ( e_res != OK )
        {
            cmd_printf( "GPIO_CMD: Cannot disable interrupt on gpio %u \n", pin_num );
            retval = CMD_ERROR;
        }
    }

    return retval;
}

#if 0
static const char *gpio_cmd_dump_desc = "Dump important information to the screen";
static const char *gpio_cmd_dump_usage = "";
static const char *gpio_cmd_dump_notes = "";
static int gpio_cmd_dump_cb( int argc, char *argv[] )
{
    int32_t retval = CMD_OK;
    int32_t i_res;
    uint32_t pin_num;
    uint32_t group_index = 0;
    uint32_t pin_index = 0;
    volatile GPIO_REGS_t * curr_gpio_reg = NULL;
    gpio_handle_t * curr_gpio = NULL;

    /* Dump all */
    if ( argc == 1 )
    {
        cmd_printf("GPIO dump information \n");

        for ( group_index = 0; group_index < NUM_GPIO_REG_GROUPS; group_index++ )
        {
            curr_gpio_reg = ( GPIO_REGS_t *) gpio_base_addrs[group_index];

            cmd_printf("GPIO group %u \n", group_index);
            cmd_printf("\t GPIO_PLR \t: 0x%08lx \n", curr_gpio_reg->GPIO_PLR);
            cmd_printf("\t GPIO_PDR \t: 0x%08lx \n", curr_gpio_reg->GPIO_PDR);
            cmd_printf("\t GPIO_PSR \t: 0x%08lx \n", curr_gpio_reg->GPIO_PSR);
            cmd_printf("\t GPIO_PCR \t: 0x%08lx \n", curr_gpio_reg->GPIO_PCR);
            cmd_printf("\t GPIO_RER \t: 0x%08lx \n", curr_gpio_reg->GPIO_RER);
            cmd_printf("\t GPIO_FER \t: 0x%08lx \n", curr_gpio_reg->GPIO_FER);
            cmd_printf("\t GPIO_EDR \t: 0x%08lx \n", curr_gpio_reg->GPIO_EDR);
            cmd_printf("\t GPIO_SDR \t: 0x%08lx \n", curr_gpio_reg->GPIO_SDR);
            cmd_printf("\t GPIO_CDR \t: 0x%08lx \n", curr_gpio_reg->GPIO_CDR);
            cmd_printf("\t GPIO_SRER \t: 0x%08lx \n", curr_gpio_reg->GPIO_SRER);
            cmd_printf("\t GPIO_CRER \t: 0x%08lx \n", curr_gpio_reg->GPIO_CRER);
            cmd_printf("\t GPIO_SFER \t: 0x%08lx \n", curr_gpio_reg->GPIO_SFER);
            cmd_printf("\t GPIO_CFER \t: 0x%08lx \n", curr_gpio_reg->GPIO_CFER);
            cmd_printf("\t GPIO_APMASK_REG: 0x%08lx \n", curr_gpio_reg->GPIO_APMASK_REG);
            cmd_printf("\t GPIO_CPMASK_REG: 0x%08lx \n", curr_gpio_reg->GPIO_CPMASK_REG);

            cmd_printf("Pin open/close: ");
            /* initialize the pin handles from each group */
            for ( pin_index = NUM_GPIO_PINS_PER_GRP;
                    pin_index > 0;
                    pin_index-- )
            {
                curr_gpio = &pin_handle_list[( group_index * NUM_GPIO_PINS_PER_GRP ) + pin_index - 1];

                if ( true == curr_gpio->is_open )
                {
                    cmd_printf("o");
                }
                else
                {
                    cmd_printf("c");
                }
            }

            cmd_printf("\nPin trigger: \t");
            /* initialize the pin handles from each group */
            for ( pin_index = NUM_GPIO_PINS_PER_GRP;
                    pin_index > 0;
                    pin_index-- )
            {
                cmd_printf("%c", trigger_flag[( group_index * NUM_GPIO_PINS_PER_GRP ) + pin_index - 1] );
            }

            cmd_printf("\n: ");
        }


    }
    else if ( argc == 2 )
    {
        /* Get the pin number */
        i_res = str_mkint( argv[1], &pin_num );
        if ( ( 0 != i_res ) || ( pin_num > NUM_GPIO_PINS) )
        {
            retval = CMD_USAGE_ERROR;
        }

        if ( NULL == pin_handle_list )
        {
            cmd_printf( "GPIO_CMD: pin handle list is not initialized \n");
            retval = CMD_ERROR;
        }

        if ( retval == CMD_OK )
        {
            curr_gpio = &pin_handle_list[pin_num];
            curr_gpio_reg = curr_gpio->regs;

            cmd_printf("GPIO %u info :\n", pin_num);
            cmd_printf("\t Open \t\t\t: ");
            if ( true == curr_gpio->is_open )
            {
                cmd_printf("YES\n");
            }
            else
            {
                cmd_printf("NO \n");
            }

            cmd_printf("\t Level \t\t\t: ");
            if ( curr_gpio_reg->GPIO_PLR & (1 << curr_gpio->index) )
            {
                cmd_printf("HIGH \n");
            }
            else
            {
                cmd_printf("LOW \n");
            }

            cmd_printf("\t Direction \t\t: ");
            if ( curr_gpio_reg->GPIO_PDR & (1 << curr_gpio->index) )
            {
                cmd_printf("OUTPUT \n");
            }
            else
            {
                cmd_printf("INPUT \n");
            }

            cmd_printf("\t Rise-Edge Detect \t: ");
            if ( curr_gpio_reg->GPIO_RER & (1 << curr_gpio->index) )
            {
                cmd_printf("ENABLED \n");
            }
            else
            {
                cmd_printf("DISABLED \n");
            }

            cmd_printf("\t Fall-Edge Detect \t: ");
            if ( curr_gpio_reg->GPIO_FER & (1 << curr_gpio->index) )
            {
                cmd_printf("ENABLED \n");
            }
            else
            {
                cmd_printf("DISABLED \n");
            }

            cmd_printf("\t Edge Detected \t\t: ");
            if ( curr_gpio_reg->GPIO_EDR & (1 << curr_gpio->index) )
            {
                cmd_printf("YES \n");
            }
            else
            {
                cmd_printf("NO \n");
            }

            cmd_printf("\t Masked \t\t: ");
            if ( curr_gpio_reg->GPIO_APMASK_REG & (1 << curr_gpio->index) )
            {
                cmd_printf("NO \n");
            }
            else
            {
                cmd_printf("YES \n");
            }

            cmd_printf("\t Last trigger \t\t: ");
            switch ( trigger_flag[pin_num] )
            {
                case 'n':
                {
                    cmd_printf("NONE \n");
                    break;
                }
                case 'l':
                {
                    cmd_printf("FALL EDGE \n");
                    break;
                }
                case 'h':
                {
                    cmd_printf("RISE EDGE \n");
                    break;
                }
                default:
                {
                    cmd_printf("NONE \n");
                    break;
                }

            }
        }
    }
    else
    {
        retval = CMD_USAGE_ERROR;
    }


    return retval;
}
#endif

static void gpio_debug_init ( void )
{
    DBG_PRINTF_ERR("%s\n", __func__);

    /* Clear the tables */
    memset ( mygpios, 0, sizeof( gpio_handle_t * ) * NUM_GPIO_PINS );
    memset ( (void*) trigger_flag, 'n', sizeof(char) * NUM_GPIO_PINS );

    int cmd_res;
    cmd_res = cmd_register_cmd( "gpio",
                                NULL,
                                "gpio driver test commands",
                                NULL,
                                NULL,
                                NULL );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio",
                                   "open",
                                   gpio_cmd_open_desc,
                                   gpio_cmd_open_usage,
                                   gpio_cmd_open_notes,
                                   gpio_cmd_open_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio",
                                   "close",
                                   gpio_cmd_close_desc,
                                   gpio_cmd_close_usage,
                                   gpio_cmd_close_notes,
                                   gpio_cmd_close_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio",
                                   "write",
                                   gpio_cmd_write_desc,
                                   gpio_cmd_write_usage,
                                   gpio_cmd_write_notes,
                                   gpio_cmd_write_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio",
                                   "read",
                                   gpio_cmd_read_desc,
                                   gpio_cmd_read_usage,
                                   gpio_cmd_read_notes,
                                   gpio_cmd_read_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio",
                                   "int",
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio int",
                                   "attach",
                                   gpio_cmd_int_attach_desc,
                                   gpio_cmd_int_attach_usage,
                                   gpio_cmd_int_attach_notes,
                                   gpio_cmd_int_attach_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio int",
                                   "detach",
                                   gpio_cmd_int_detach_desc,
                                   gpio_cmd_int_detach_usage,
                                   gpio_cmd_int_detach_notes,
                                   gpio_cmd_int_detach_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio int",
                                   "enable",
                                   gpio_cmd_int_enable_desc,
                                   gpio_cmd_int_enable_usage,
                                   gpio_cmd_int_enable_notes,
                                   gpio_cmd_int_enable_cb );
    ASSERT( CMD_OK == cmd_res);

    cmd_res = cmd_register_subcmd( "gpio int",
                                   "disable",
                                   gpio_cmd_int_disable_desc,
                                   gpio_cmd_int_disable_usage,
                                   gpio_cmd_int_disable_notes,
                                   gpio_cmd_int_disable_cb );
    ASSERT( CMD_OK == cmd_res);

#if 0
    cmd_res = cmd_register_subcmd( "gpio",
                                   "dump",
                                   gpio_cmd_dump_desc,
                                   gpio_cmd_dump_usage,
                                   gpio_cmd_dump_notes,
                                   gpio_cmd_dump_cb );
    ASSERT( CMD_OK == cmd_res);
#endif
}

#endif



