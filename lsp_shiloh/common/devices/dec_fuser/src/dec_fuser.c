/*
 * ======================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ====================================================================== */

/// \file dec_fuser.c

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "lassert.h"
#include "regAddrs.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"
#include "dec_fuser.h"
#include "dec_fuser_priv.h"
#include "dec_adc_api.h"
#include "uio_lib_api.h"
#include "logger.h"
#include "debug.h"
#include "delay_api.h"
#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#include "utils.h"
#endif

#ifndef ABS
#define ABS( x ) (x) > 0 ? (x) : -(x)
#endif

#define MAX_FUSER_MESSAGES 4

#ifdef USE_ZERO_CROSS_INT
//================================= ZERO CROSS IRQ =================
#define INTPRIORITY_FUSER 20
static uint32_t g_dec_fuser_irq_enabled = 0;

static void dec_fuser_isr( int32_t interrupt_count, void *context );

dec_fuser_t *
dec_fuser_create_gpio(dec_fuser_t *dec_fuser,
                       gpio_bank_t bank,
                       uint32_t pin,
                       gpio_logic_setting_t sense)
{
    uint32_t err ;
    gpio_handle_t *hpin = 0;

    err = gpio_open(&hpin,   // open, default and ignore
                    bank,
                    pin, // pin number
                    GPIO_DIRECTION_INPUT,
                    sense,
                    0);
    ASSERT( !err );

    dec_fuser->gpio = hpin;
    dec_fuser->callback = dec_fuser_unblock;
    dec_fuser->callback_private_data = dec_fuser;
//  dec_fuser->active = false;

    return dec_fuser;
}

static void fuser_gpio_cb( gpio_handle_t * pin, uint32_t logic_level, void * isr_callback_data )
{
	//DBG_PRINTF_DEBUG(" %s level=%d \n", __func__, logic_level);
	dec_fuser_t * fuser = (dec_fuser_t *)isr_callback_data;
    if(fuser->cb)
                (fuser->cb)(fuser->cb_private_data);
}

void dec_fuser_cb_add(dec_fuser_t * fuser,void(*cb)( void *cb_private_data ),void *cb_private_data)
{
    ASSERT(fuser);
    if ( fuser->gpio )
    {
        if (cb)
        {
            gpio_isr_attach( fuser->gpio, &fuser_gpio_cb, fuser, 0, GPIO_ISR_TYPE_EITHER_EDGE );
            gpio_isr_enable( fuser->gpio );
        }
        else
        {
            gpio_isr_disable( fuser->gpio );
            gpio_isr_detach( fuser->gpio );
        }
    }
    else
    {
        while (0 == sem_trywait(&fuser->semaphore))
        {
            //keep clearing
        }
    }
    fuser->cb= cb;
    fuser->cb_private_data = cb_private_data;
}

void dec_fuser_enable_isr( dec_fuser_t *fuser )
{
	FUSER_REGS_t* fuser_regs = dec_fuser_get_regs_base();
    ASSERT(fuser_regs);
    ASSERT(fuser);
    //DBG_PRINTF_DEBUG(" %s irq=%d \n", __func__, g_dec_fuser_irq_enabled);
    if ( g_dec_fuser_irq_enabled == 0 )
    {
        uio_int_attach( dec_fuser_get_dev(), INTPRIORITY_FUSER, dec_fuser_isr, fuser );
        uio_int_enable( dec_fuser_get_dev() );
    }

    ++g_dec_fuser_irq_enabled;

    //Enable IRQ
    //DBG_PRINTF_DEBUG(" %s Before FIER=0x%X \n", __func__, fuser_regs->FIER);
	ASSERT(FUSER_FIER_ZEROCROSS_MASK_SHIFT(fuser_regs->FIER) == 0);
	fuser_regs->FIER = FUSER_FIER_ZEROCROSS_REPLACE_VAL(fuser_regs->FIER, 1);
	//DBG_PRINTF_DEBUG(" %s After ZEROCROSS FIER=0x%X \n", __func__, fuser_regs->FIER);
	ASSERT(FUSER_FIER_TIMEOUT_MASK_SHIFT(fuser_regs->FIER) == 0);
	fuser_regs->FIER = FUSER_FIER_TIMEOUT_REPLACE_VAL(fuser_regs->FIER, 1);
	//DBG_PRINTF_DEBUG(" %s After TIMEOUT FIER=0x%X \n", __func__, fuser_regs->FIER);
	ASSERT(FUSER_FIER_DUTYCYCLECOMPLETE_MASK_SHIFT(fuser_regs->FIER) == 0);
	fuser_regs->FIER = FUSER_FIER_DUTYCYCLECOMPLETE_REPLACE_VAL(fuser_regs->FIER, 1);
	//DBG_PRINTF_DEBUG(" %s After DUTYCYCLECOMPLETE FIER=0x%X \n", __func__, fuser_regs->FIER);

	// CLEAR IRQ
	//DBG_PRINTF_DEBUG(" %s Before FIAR=0x%X \n", __func__, fuser_regs->FIAR);
	fuser_regs->FIAR = FUSER_FIAR_ZEROCROSS_REPLACE_VAL(fuser_regs->FIAR, 1);
	// DBG_PRINTF_DEBUG(" %s After ZEROCROSS Clear FIAR=0x%X \n", __func__, fuser_regs->FIAR);
	fuser_regs->FIAR = FUSER_FIAR_TIMEOUT_REPLACE_VAL(fuser_regs->FIAR, 1);
	// DBG_PRINTF_DEBUG(" %s After TIMEOUT Clear FIAR=0x%X \n", __func__, fuser_regs->FIAR);
	fuser_regs->FIAR = FUSER_FIAR_FUSEROVERTEMP_REPLACE_VAL(fuser_regs->FIAR, 1);
	// DBG_PRINTF_DEBUG(" %s After OVERTEMP Clear FIAR=0x%X \n", __func__, fuser_regs->FIAR);
	fuser_regs->FIAR = FUSER_FIAR_DUTYCYCLECOMPLETE_REPLACE_VAL(fuser_regs->FIAR, 1);
	//DBG_PRINTF_DEBUG(" %s After DUTYCYCLECOMPLETE Clear FIAR=0x%X \n", __func__, fuser_regs->FIAR);
}

void dec_fuser_disable_isr( dec_fuser_t *fuser )
{
    FUSER_REGS_t* fuser_regs = dec_fuser_get_regs_base();
    ASSERT(fuser_regs);
    ASSERT(fuser);
    //ASSERT(fuser->callback);

    --g_dec_fuser_irq_enabled;

    ASSERT(FUSER_FIER_ZEROCROSS_MASK_SHIFT(fuser_regs->FIER) == 1);
    fuser_regs->FIER = FUSER_FIER_ZEROCROSS_REPLACE_VAL(fuser_regs->FIER, 0);

    if ( g_dec_fuser_irq_enabled == 0 )
    {
        uio_int_disable( dec_fuser_get_dev() );
        uio_int_detach( dec_fuser_get_dev() );
    }
}

uint32_t dec_fuser_cnt[3] = { 0, };
static void dec_fuser_isr( int32_t interrupt_count, void *context )
{
	DBG_PRINTF_DEBUG("%s interrupt_count %d\n", __func__, interrupt_count);
    uint32_t zeroCrossbit = 1;
    zeroCrossbit <<= 7;
    dec_fuser_t *dec_fuser = (dec_fuser_t *)context;
    //DEC_SENSOR_REGS_t* sensors = dec_sensor_get_regs_base();
    FUSER_REGS_t* fuser_regs = dec_fuser_get_regs_base();
    ASSERT(fuser_regs);
    uint32_t enabled = fuser_regs->FIER;	//int_en;
    uint32_t pending = fuser_regs->FIPR;	//int_st;

    // make it a one shot for testing...
    // fuser_regs->FIER = ~(pending & enabled);
    fuser_regs->FIAR = pending & enabled;	//int_cl

    //ASSERT(FUSER_FIER_ZEROCROSS_MASK_SHIFT(fuser_regs->FIER) == 1);

	if ( ( enabled & zeroCrossbit ) && ( pending & zeroCrossbit ) && dec_fuser->callback )
	{
		++dec_fuser_cnt[0]; //use dec_fuser->instance
		(dec_fuser->callback)(dec_fuser->callback_private_data);
		if(dec_fuser->cb)
			(dec_fuser->cb)(dec_fuser->cb_private_data);
	}
}


bool dec_fuser_poll(const dec_fuser_t *fuser)
{
    ASSERT(fuser);
    if (fuser) //fuser->active)
    {
        ASSERT(fuser->gpio == 0);
        //TODO:
    }

    ASSERT(fuser->gpio != 0);
    uint32_t val;
    gpio_read(fuser->gpio, &val);

    return( val );
}

int dec_fuser_block(dec_fuser_t *fuser, uint32_t msec )
{
    ASSERT(fuser);	// && sensor->pin_number < 19);
    if (fuser->callback)
    {
        struct timespec abs_timeout;

        while (0 == sem_trywait(&fuser->semaphore))
        {
            //keep clearing
        	DBG_PRINTF_DEBUG("fuser clear semaphore %d \n",fuser->instance);
        }

        // convert from relative OS tick timeout to absolute time.
        clock_gettime( CLOCK_REALTIME, &abs_timeout );
        DBG_PRINTF_DEBUG("fuser wait semaphore %d %d %d \n", fuser->instance, abs_timeout.tv_sec, abs_timeout.tv_nsec);
        posix_calc_future_usec( &abs_timeout, msec*1000 );

        DBG_PRINTF_DEBUG("fuser wait semaphore %d %d %d \n", fuser->instance, abs_timeout.tv_sec, abs_timeout.tv_nsec);
        int err = sem_timedwait( &fuser->semaphore, &abs_timeout );
        DBG_PRINTF_DEBUG("fuser done semaphore %d err %d %d \n",fuser->instance, err, errno);
        if (err == 0)
            return 0;
        else {
	    perror("fuser semaphore \n");
            return msec;
	}
    }
    return -1; // bad call
}

void dec_fuser_unblock( void *cb_data )
{
	dec_fuser_t * fuser = (dec_fuser_t *) cb_data;
	//DBG_PRINTF_DEBUG("sensor unblock %d state %d \n", fuser->instance, dec_fuser_poll(fuser));
    sem_post(&fuser->semaphore);
}

// block
int dec_fuser_block_state(dec_fuser_t *fuser, bool fuser_state, uint32_t timeout_ticks )
{
    bool cur_fuser_state;
    uint32_t timeout = timeout_ticks;
    ASSERT(fuser && fuser->instance < 3);
    DBG_PRINTF_ERR( "this is test code don't use it.");
    cur_fuser_state = dec_fuser_poll(fuser);
    while(cur_fuser_state != fuser_state && timeout > 0)
    {
        posix_sleep_ms(1);
        if(timeout != -1)
        {
            timeout--;
        }
        cur_fuser_state = dec_fuser_poll(fuser);
    }
    return (cur_fuser_state == fuser_state) ? 0 : timeout_ticks;
}
#endif //USE_ZERO_CROSS_INT

/// serial debug
int dec_fuser_dbcallback( int argc, char *argv[] );

/// isr context, or fast timer loop if irq's are broke.
///

//static DelayTimer_t *delaytimer = 0;
#if ZX_ENABLE
void dec_fuser_set_temp( dec_fuser_t *fuser, uint32_t target_temp )
#else
void dec_fuser_set_temp( dec_fuser_t *fuser, dec_fuser_temp_t *target_temp )
#endif
{
    int err;
    //DBG_PRINTF_NOTICE("%s: fuser: %#x, temp: %#x : %d\n", __func__, fuser, target_temp, target_temp->index);
    if (fuser)
    {
        DBG_ASSERT(fuser->cookie == DEC_FUSER_DATATYPE_COOKIE);

        fuser->scmd = (uint32_t) target_temp;
//      DBG_PRINTF_ERR("%s request: %d\n", __func__, fuser->scmd);

        err = mq_send(fuser->mq, (char*)&fuser->scmd, sizeof(uint32_t), 0);
        XASSERT(err == 0, errno);
    }
}

void dec_fuser_set_paper_weight(
    dec_fuser_t* fuser,  ///< the fuser in question.
    dec_fuser_weight_t * weight  ///< the paper weight 
    )
{
    if (fuser) 
    {
        fuser->paper_weight = weight;
    }
}

#ifdef ADD_ZC
extern uint32_t fuser_mode_request;
static void dec_fuser_timer_func(void* cbdata)
{
    dec_fuser_t *dec_fuser = (dec_fuser_t *)cbdata;

    DBG_ASSERT(dec_fuser->cookie == DEC_FUSER_DATATYPE_COOKIE);

    // log dec_fuser->rcmd;
    if (0 == posix_wait_for_message(dec_fuser->mq, (char*)&dec_fuser->rcmd, sizeof(uint32_t), 0))
    {
        fuser_mode_request =  (uint32_t) dec_fuser->rcmd; 

        DBG_PRINTF_ERR("%s %d request: %d\n", __func__, __LINE__, fuser_mode_request);

        while (0 == sem_trywait(&dec_fuser->semaphore))
            ; // empty count
    }

    dec_fuser->fuser_service_callback(dec_fuser);
}
#else //ADD_ZC
static void dec_fuser_timer_func(void* cbdata)
{
    dec_fuser_t *dec_fuser = (dec_fuser_t *)cbdata;


    DBG_ASSERT(dec_fuser->cookie == DEC_FUSER_DATATYPE_COOKIE);

    // log dec_fuser->rcmd;
    if (0 == posix_wait_for_message(dec_fuser->mq, (char*)&dec_fuser->rcmd, sizeof(uint32_t), 0))
    {
        voltage_power_pair_t *target_voltages;

        dec_fuser->target_temp =  (dec_fuser_temp_t *) dec_fuser->rcmd; 

        target_voltages = &dec_fuser->paper_weight->table[dec_fuser->target_temp->index * dec_fuser->num_points_curve];   
        dec_fuser->target_voltage = (int32_t) target_voltages[dec_fuser->num_points_curve - 1].v; // only uses the target voltage.

        //DBG_PRINTF_DEBUG("%s: RxCmd fuser: %#x, temp: %#x voltage %d \n", __func__, dec_fuser, dec_fuser->target_temp, dec_fuser->target_voltage);

        while (0 == sem_trywait(&dec_fuser->semaphore))
            ; // empty count
    }

    dec_fuser->fuser_service_callback(dec_fuser);
}
#endif //ADD_ZC

void dec_fuser_assert_stop(void)
{
    const char *str;
    int i;
    for (i = 0, str = dec_fuser_name_by_index(i); 
         str ; 
         i++, str = dec_fuser_name_by_index(i) )
    {
        dec_fuser_t *fuser = dec_fuser_by_index(i);
        if (fuser != NULL)
        {
            fuser->fatal_error = true;
            dec_fuser_set_duty_cycle( fuser, 0 );
        }
    }
}

void dec_fuser_init(void)
{
    int retcode;

    dec_fuser_local_init();

    //turns off power to fuser on assert
    retcode = atassert(dec_fuser_assert_stop);
    XASSERT(retcode==0, retcode);

#ifdef HAVE_CMD
    retcode = cmd_register( "fuser", NULL, NULL, NULL, dec_fuser_dbcallback );
    ASSERT( CMD_OK == retcode );
#endif

}

void
dec_fuser_set_at_temp_callback(
    dec_fuser_t *fuser,  
    void(*callback)( void *callback_private_data ), //< on interrupt
    void *callback_private_data //< callback data on interrupt.
    )
{
    if (fuser)
    {
        fuser->at_temp_callback = callback;
        fuser->at_temp_data = callback_private_data;
    }
}

void dec_fuser_block_till_at_temp( dec_fuser_t *fuser )
{
    struct timespec ts_start, ts_end;

    if (fuser->fatal_error)
        return;

    clock_gettime( CLOCK_REALTIME, &ts_start );
    sem_wait(&fuser->semaphore);
    clock_gettime( CLOCK_REALTIME, &ts_end );

    posix_timespec_sub(&ts_end, &ts_start, &ts_end);

    DBG_PRINTF_NOTICE("%s: waited %d sec %d msec\n", __func__, ts_end.tv_sec, ts_end.tv_nsec/NANOSEC_PER_MILLISEC);
}

static void from_irq_post_semaphore_function( void *cb_data )
{
    dec_fuser_t *fuser = (dec_fuser_t *) cb_data;

    sem_post(&fuser->semaphore);
}



void 
dec_fuser_initialize( dec_fuser_t *fuser, uint32_t instance )
{
    #define MAX_NAME 64
    char name_buf[MAX_NAME];
    int err;
    if (fuser) 
    {
        fuser->cookie = DEC_FUSER_DATATYPE_COOKIE;
        fuser->instance = instance;
        fuser->fatal_error = false;
        fuser->target_temp = dec_fuser_temp_by_index(0);
	ASSERT( fuser->paper_weight ); // MUST be set.

        snprintf(name_buf, MAX_NAME, "/fuserQueue%d", instance);
        posix_create_message_queue(&fuser->mq, name_buf, MAX_FUSER_MESSAGES, sizeof(uint32_t));

        // dbg_test_delay_timer();

        // should be irq based, revisit.
        // timer needs to be serviced faster than 150ms,
        // hw will fault and stop the fuser.
#ifndef USE_ZERO_CROSS_INT
        delay_msec_timer_non_blocking(1, dec_fuser_timer_func, fuser, true); /* [KSF032701] for fuser state control *//* [KSF050201] */

        fuser->regs = dec_fuser_get_regs_base();
        fuser->regs->FCR0 = fuser->regs->FCR0; // I've been serviced.

#if 0
		//DBG_PRINTF_DEBUG(" %s Before FIER=0x%X \n", __func__, fuser->regs->FIER);
		ASSERT(FUSER_FIER_ZEROCROSS_MASK_SHIFT(fuser->regs->FIER) == 0);
		fuser->regs->FIER = FUSER_FIER_ZEROCROSS_REPLACE_VAL(fuser->regs->FIER, 1);
		//DBG_PRINTF_DEBUG(" %s After ZEROCROSS FIER=0x%X \n", __func__, fuser->regs->FIER);
#endif
        // revisit constructor
        fuser->powHistory[0] = fuser->powHistory[4] = 0;

        err = sem_init(&fuser->semaphore, 0, 0);
        XASSERT(err == 0, errno);

        // set default callback
        dec_fuser_set_at_temp_callback( fuser, from_irq_post_semaphore_function, fuser);
#else
        DBG_PRINTF_DEBUG(" %s Register %d\n", __func__, instance);
        fuser->regs = dec_fuser_get_regs_base();
        fuser->callback = dec_fuser_timer_func;	//dec_fuser_unblock;
        fuser->callback_private_data = fuser;
        fuser->regs->FCR0 = fuser->regs->FCR0; // I've been serviced.

        // revisit constructor
        fuser->powHistory[0] = fuser->powHistory[4] = 0;                

        // set default callback
        dec_fuser_set_at_temp_callback( fuser, from_irq_post_semaphore_function, fuser);

        err = sem_init(&fuser->semaphore, 0, 0);
        XASSERT(err == 0, errno);
#if 1 //GPIO working
        fuser = dec_fuser_create_gpio( fuser, GPIO_BANK_E, 9, GPIO_LOGIC_SETTING_ACTIVE_LOW);
        dec_fuser_cb_add(fuser, dec_fuser_timer_func, fuser);
#else	//not working
        dec_fuser_enable_isr(fuser);
#endif

#endif

    }
    ASSERT(fuser);
} 


void dec_fuser_enable_power( bool power_on )
{
    FUSER_REGS_t* fuser_regs = dec_fuser_get_regs_base();

    DBG_PRINTF_NOTICE("%s = %d\n", __func__, power_on);

    fuser_regs->FEPR = FUSER_FEPR_LVPOWERON_REPLACE_VAL(fuser_regs->FEPR, (power_on ? 1 : 0));

}

#ifdef HAVE_CMD
int dec_fuser_dbcallback( int argc, char *argv[] )
{
    int num; 
    int temp;
    const char *str;
    int i;
    
    if (argc == 2) {
        if (strcmp (argv[1],"weight")== 0) 
        {
            for (i = 0, str = dec_fuser_weight_name_by_index(i); 
             str ; 
             i++, str = dec_fuser_weight_name_by_index(i) )
            {
                dbg_printf(" dec_fuser_weight [%d] %s\n", i, str);
            }
            return 0;
        }
        if (strcmp (argv[1],"temp")== 0) 
        {
            for (i = 0, str = dec_fuser_temp_name_by_index(i); 
             str ; 
             i++, str = dec_fuser_temp_name_by_index(i) )
            {
                dbg_printf(" dec_fuser_temp [%d] %s\n", i, str);
            }
            return 0;
        }
    }
    if (argc < 3 )         /* not enough parameters */
    {
        dec_fuser_t * dec_fuser = dec_fuser_by_index(0); 
        voltage_power_pair_t * target_voltages;

        for (i = 0, dec_fuser = dec_fuser_by_index(i); dec_fuser != NULL; i++, dec_fuser = dec_fuser_by_index(i))
        {
            dbg_printf("debug:dec_fuser fuser_number_0->n temp->0->n \n");
            dbg_printf("debug:dec_fuser temp|weight \n");
            dbg_printf("      target_temp_table = %s, paper_weight = %s \n",
                       dec_fuser_temp_name_by_index(dec_fuser->target_temp->index),
                       dec_fuser->paper_weight->name
                      );
            dbg_printf("      dec_fuser  Measured mV = %d, applied power %d \n",
                       dec_adc_scaled_read_channel_mV( dec_adc_by_index(0) ),
                       dec_fuser->powHistory[4]
                      );
            dbg_printf("      dec_fuser FEPR = %#08x\n", dec_fuser->regs->FEPR); /* Fuser Enable and Polarity *//* [KSF050201] */
            dbg_printf("      dec_fuser FSR  = %#08x\n", dec_fuser->regs->FSR); /* Fuser Status *//* [KSF050201] */

            target_voltages = &dec_fuser->paper_weight->table[dec_fuser->target_temp->index * dec_fuser->num_points_curve];

            for (num = 0; num < dec_fuser->num_points_curve; num++)
            {
                dbg_printf("      dec_fuser ( v = %d, p = %d ) \n",(target_voltages)[num].v,(target_voltages)[num].p);
            }
        }

        return 0;
    }

    if(str_match("power", argv[1], 5))
    {
        dec_fuser_enable_power(str_match("on", argv[2], 2));
    }
    else
    {
        num = atoi(argv[1]);
        temp = atoi(argv[2]);
         
        if (temp > 255) {
            dbg_printf("debug:dec_fuser fuser_number_0->n temp->0->n \n");
            dbg_printf("debug:dec_fuser power on|off\n");
        }
        else {
            dec_fuser_t * fuser = dec_fuser_by_index(num);
    
            if (!fuser)
            {
                dbg_printf("debug:dec_fuser invalid %d, %x \n", num, fuser);
            }
            else
            {
#if ZX_ENABLE
                dec_fuser_set_temp( fuser, 1 );
#else
                dec_fuser_set_temp( fuser, dec_fuser_temp_by_index(temp) );
#endif
            }
        }
    }
    return 0;
}
#endif // #ifdef HAVE_CMD

/// Private structure is used to overlay the instance registers in the fuser
/// to make them look like and array of structures for easier programming.
typedef struct fuser_instance_regs_s
{
    volatile uint32_t FCR[MAX_FUSERS];

    volatile uint32_t FPECR[MAX_FUSERS];

    volatile uint32_t FPOCR[MAX_FUSERS];

    volatile uint32_t FDCR[MAX_FUSERS];

    volatile uint32_t FPACR[MAX_FUSERS];
} fuser_instance_regs_t;

#ifdef ADD_ZC
void dec_fuser_set_duty_cycle( 
    dec_fuser_t * dec_fuser,
    uint32_t duty_cycle //< 0 - 100 int as a duty cycle percentage
    )
{
    FUSER_REGS_t *fuser = dec_fuser->regs;
    fuser_instance_regs_t *i_regs = (fuser_instance_regs_t *)(&fuser->FCR0);
    uint32_t output_enable;
    uint32_t fuser_instance;

    ASSERT(dec_fuser->instance < MAX_FUSERS); 
    fuser_instance = dec_fuser->instance;

    if ( duty_cycle == 100 ) {
        duty_cycle = 0xff;
    }
    else {
        duty_cycle = ((255*duty_cycle+8)/100) & 0xF0;
    }
    // always drive to 0 if fuser in fatal error condition
    if (dec_fuser->fatal_error)
    {
        // NOTE: do not assert in this routine; we want to set the power to 0.
        duty_cycle = 0;
    }

    // disable fuser[1:0]
    output_enable = fuser->FEPR & FUSER_FEPR_FUSER_OE_MASK;
    output_enable = output_enable & ~(1<<fuser_instance);
    fuser->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(
            fuser->FEPR, 
            output_enable);    

    i_regs->FDCR[fuser_instance] = FUSER_FDCR0_FUSER_FULLCYCLE_REPLACE_VAL(
            i_regs->FDCR[fuser_instance],
            0); // half cycle
    i_regs->FDCR[fuser_instance] = FUSER_FDCR0_FUSER_DUTYCYCLE_REPLACE_VAL(
            i_regs->FDCR[fuser_instance],
            duty_cycle);

    // irq's are broken at the moment!
    fuser->FIAR = FUSER_FIAR_ZEROCROSS_REPLACE_VAL(
            fuser->FIAR, 
            1);        // clear IRQ
    fuser->FIAR = FUSER_FIAR_TIMEOUT_REPLACE_VAL(
            fuser->FIAR, 
            1);        // clear IRQ
    fuser->FIAR = FUSER_FIAR_FUSEROVERTEMP_REPLACE_VAL(
            fuser->FIAR, 
            1);  // clear IRQ

    if (duty_cycle != 0) {
        output_enable = (fuser->FEPR & FUSER_FEPR_FUSER_OE_MASK) | (1<<fuser_instance);
        fuser->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(
                fuser->FEPR,
                output_enable);   
    }
}

#else //ADD_ZC

void dec_fuser_set_duty_cycle( 
        dec_fuser_t * dec_fuser,
        uint32_t duty_cycle //< 0 - 100 int as a duty cycle percentage
        )
{
    FUSER_REGS_t *fuser = dec_fuser->regs;
    fuser_instance_regs_t *i_regs = (fuser_instance_regs_t *)(&fuser->FCR0);
    char DSM_dutycycle = duty_cycle == 100 ? 0xff : ((255*duty_cycle+8)/100) & 0xF0;
    uint32_t output_enable;
    uint32_t fuser_instance;

//    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("%s: power = %d\n", __func__, duty_cycle));
    ASSERT(dec_fuser->instance < MAX_FUSERS); 
    fuser_instance = dec_fuser->instance;

    // always drive to 0 if fuser in fatal error condition
    if (dec_fuser->fatal_error)
    {
        // NOTE: do not assert in this routine; we want to set the power to 0.
        duty_cycle = 0;
    }

    // disable fuser[1:0]
    output_enable = fuser->FEPR & FUSER_FEPR_FUSER_OE_MASK;
    output_enable = output_enable & ~(1<<fuser_instance);
    fuser->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(
        fuser->FEPR, 
        output_enable);    

    i_regs->FCR[fuser_instance] = FUSER_FCR0_FUSER_TIMESTEP_REPLACE_VAL(
        i_regs->FCR[fuser_instance],
        8);  // 8 ms    
    i_regs->FCR[fuser_instance] = FUSER_FCR0_FUSER_TIMEBASE_REPLACE_VAL(
        i_regs->FCR[fuser_instance],
        DEC_FUSER_Timebase_1ms);
    i_regs->FCR[fuser_instance] = FUSER_FCR0_FUSER_MODE_REPLACE_VAL(
        i_regs->FCR[fuser_instance],
        DEC_FUSER_Mode_AsyncDSM);
        //DEC_FUSER_Mode_SyncPWM);
    	//DEC_FUSER_Mode_SyncDSM);
    i_regs->FDCR[fuser_instance] = FUSER_FDCR0_FUSER_FULLCYCLE_REPLACE_VAL(
        i_regs->FDCR[fuser_instance],
        0); // half cycle
    i_regs->FDCR[fuser_instance] = FUSER_FDCR0_FUSER_DUTYCYCLE_REPLACE_VAL(
        i_regs->FDCR[fuser_instance],
        DSM_dutycycle);

    i_regs->FPACR[fuser_instance] = FUSER_FPACR0_FUSER_PULSEWIDTH_REPLACE_VAL(
        i_regs->FPACR[fuser_instance], 
        10);
    i_regs->FPACR[fuser_instance] = FUSER_FPACR0_FUSER_PULSEOFFSET_REPLACE_VAL(
        i_regs->FPACR[fuser_instance], 
        0);

    // irq's are broken at the moment!
    fuser->FIAR = FUSER_FIAR_TIMEOUT_REPLACE_VAL(
        fuser->FIAR, 
        1);        // clear IRQ
    fuser->FIAR = FUSER_FIAR_FUSEROVERTEMP_REPLACE_VAL(
        fuser->FIAR, 
        1);  // clear IRQ

    if (duty_cycle != 0) {
        output_enable = (fuser->FEPR & FUSER_FEPR_FUSER_OE_MASK) | (1<<fuser_instance);
        fuser->FEPR = FUSER_FEPR_FUSER_OE_REPLACE_VAL(
            fuser->FEPR,
            output_enable);   
    }
}
#endif //ADD_ZC
/* [KSF050201] - start */
/// set the fuser phase config only for Sync* modes currently we only support syncPWM mode 
/// (although syncDSM mode is ifdefed out and works, syncPWM gives better results)
void dec_fuser_set_phase_config( 
    dec_fuser_t *dec_fuser,
    uint32_t pulse_width_uSec,
    uint32_t pulse_offset_uSec
    )

{
    FUSER_REGS_t *fuser = dec_fuser->regs;
    fuser_instance_regs_t *i_regs = (fuser_instance_regs_t *)(&fuser->FCR0);
    uint32_t fuser_instance = dec_fuser->instance;

    i_regs->FPACR[fuser_instance] = FUSER_FPACR0_FUSER_PULSEWIDTH_REPLACE_VAL(
             i_regs->FPACR[fuser_instance], 
             pulse_width_uSec);
    i_regs->FPACR[fuser_instance] = FUSER_FPACR0_FUSER_PULSEOFFSET_REPLACE_VAL(
             i_regs->FPACR[fuser_instance], 
             pulse_offset_uSec);
}

/// set the fuser zero crossing registers
/// Affects ANY sync* fuser
void dec_fuser_set_zero_crossing( 
    dec_fuser_t *dec_fuser,//it doesn't matter which one affects all instances
    uint32_t filter, //< ZCFR
    uint32_t lockout //<ZCLR
    )
{
    FUSER_REGS_t *fuser = dec_fuser->regs;
    fuser->ZCFR = filter;
    fuser->ZCLR = lockout;
} /* [KSF050201] */

