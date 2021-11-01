/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/// \file dec_sensor.c

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "lassert.h"
#include "logger.h"
#include "debug.h"

#include "regAddrs.h"
#include "uio_lib_api.h"
#include "DEC_CONFIG_regmasks.h"
#include "DEC_CONFIG_regstructs.h"

#include "dec_sensor.h"
#include "dec_sensor_priv.h"

#if defined( HAVE_CMD ) && defined ( DEBUG )
#include "cmd_proc_api.h"
#include "delay_api.h" // msec
#endif


#ifndef INTNUM_CDMA 
// #warning failure to define interrupt numbers
#endif

// one per sensor. 
static dec_sensor_t g_dec_sensors[19];

static uint32_t g_dec_irq_enabled = 0;

#define INTPRI_SENSOR 20

#if defined( HAVE_CMD ) && defined ( DEBUG )
/// serial debug callback function.
static int dec_sensor_cmd_init( void );
#else
#define dec_sensor_cmd_init()
#endif

static void dec_sensor_isr( int32_t interrupt_count, void *context );

void dec_sensor_init()
{
    uint32_t offset;
    uint32_t i;
    DEC_SENSOR_REGS_t* sensors = dec_sensor_get_regs_base();
    ASSERT(sensors);

    // validate hw registers haven't moved.
    DBG_ASSERT( ((18 * 4 * 4) + (uint32_t)&sensors->cfg0) == (uint32_t)&sensors->cfg18 );

    for (i=0; i < 19; i++)
    {
        g_dec_sensors[i].active = false;
        g_dec_sensors[i].pin_number = i;
        g_dec_sensors[i].callback = 0;
        g_dec_sensors[i].callback_private_data = 0;
        g_dec_sensors[i].cb = 0;
        g_dec_sensors[i].cb_private_data = 0;
        
        offset = i * 4 * 4;

        g_dec_sensors[i].cfg = (volatile uint32_t*)((uint32_t)&sensors->cfg0 + offset);
        g_dec_sensors[i].fthresh = (volatile uint32_t*)((uint32_t)&sensors->thresh0 + offset);
        g_dec_sensors[i].frise = (volatile uint32_t*)((uint32_t)&sensors->risew0 + offset);
        g_dec_sensors[i].ffall = (volatile uint32_t*)((uint32_t)&sensors->fallw0 + offset);


        g_dec_sensors[i].sensors = sensors;
    }
    // customer initialization
    {
        extern void dec_sensor_local_init(void);
        dec_sensor_local_init();
    }

    dec_sensor_cmd_init();
}

dec_sensor_t * 
dec_sensor_create_gpio(uint32_t num,
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
    g_dec_sensors[num].active = false;
    g_dec_sensors[num].gpio = hpin;
    g_dec_sensors[num].callback = dec_sensor_unblock; // 0; // callback;
    g_dec_sensors[num].callback_private_data = &g_dec_sensors[num]; // was : 0;

    return &g_dec_sensors[num]; 
}  



dec_sensor_t *
dec_sensor_create(uint32_t num, 
                  uint32_t fcfg,
                  uint32_t fthresh,
                  uint32_t cfg,
                  void (*callback)(void *callback_private_data)
    )
{
    ASSERT(num < 19);
    bool use_interrupts = (cfg & SENSOR_REGS_CFG0_INTTYPE_MASK);

    if (use_interrupts)
    {
        REL_ASSERT(callback);

    }   
    if (num >= 19 || g_dec_sensors[num].active )
        return 0; // failure 

    g_dec_sensors[num].active = true;
    g_dec_sensors[num].callback = callback;
    g_dec_sensors[num].callback_private_data = &g_dec_sensors[num];
    g_dec_sensors[num].gpio = 0;

    *g_dec_sensors[num].cfg = cfg;
    *g_dec_sensors[num].fthresh = SENSOR_REGS_THRESH0_THRESHOLD_REPLACE_VAL(*g_dec_sensors[num].fthresh, fthresh);
    *g_dec_sensors[num].frise = SENSOR_REGS_RISEW0_RISE_LO_WEIGHT_REPLACE_VAL(*g_dec_sensors[num].frise, ((fcfg >> (8 * 1)) & 0xff));
    *g_dec_sensors[num].frise = SENSOR_REGS_RISEW0_RISE_HI_WEIGHT_REPLACE_VAL(*g_dec_sensors[num].frise, ((fcfg >> (8 * 0)) & 0xff));
    *g_dec_sensors[num].ffall = SENSOR_REGS_FALLW0_FALL_LO_WEIGHT_REPLACE_VAL(*g_dec_sensors[num].ffall, ((fcfg >> (8 * 3)) & 0xff));
    *g_dec_sensors[num].ffall = SENSOR_REGS_FALLW0_FALL_HI_WEIGHT_REPLACE_VAL(*g_dec_sensors[num].ffall, ((fcfg >> (8 * 2)) & 0xff));

    if (use_interrupts) 
    {
        int err;
        err = sem_init(&g_dec_sensors[num].semaphore, 0, 0);
        XASSERT(err == 0, errno);

        dbg_printf("enable sensors isr \n");
        dec_sensor_enable_isr(&g_dec_sensors[num]);
    }
    return &g_dec_sensors[num];
}

uint32_t
dec_sensor_pin_number( dec_sensor_t *sensor )
{
    ASSERT(sensor && sensor->pin_number < 19 && !sensor->gpio);
    return(sensor->pin_number);
}

static void sensor_gpio_cb( gpio_handle_t * pin, uint32_t logic_level, void * isr_callback_data )
{
    dec_sensor_t * sensor = (dec_sensor_t *)isr_callback_data;
    if(sensor->cb)
                (sensor->cb)(sensor->cb_private_data);
}
void dec_sensor_cb_add(dec_sensor_t * sensor,void(*cb)( void *cb_private_data ),void *cb_private_data)
{
    ASSERT(sensor && sensor->pin_number < 19);
    if ( sensor->gpio ) 
    {
        if (cb) 
        {
            gpio_isr_attach( sensor->gpio, &sensor_gpio_cb, sensor, 0, GPIO_ISR_TYPE_EITHER_EDGE );
            gpio_isr_enable( sensor->gpio );
        }
        else
        {
            gpio_isr_disable( sensor->gpio );
            gpio_isr_detach( sensor->gpio );
        }
    }
    else
    {
        while (0 == sem_trywait(&sensor->semaphore))
        {
            //keep clearing
        }
    }
    sensor->cb= cb;
    sensor->cb_private_data = cb_private_data;
}

void
dec_sensor_enable_isr( dec_sensor_t *sensor )
{
    DEC_SENSOR_REGS_t* sensors = dec_sensor_get_regs_base();
    ASSERT(sensors);
    ASSERT(sensor && sensor->pin_number < 19);
    //ASSERT(sensor->callback);
    if ( g_dec_irq_enabled == 0 )
    {
        uio_int_attach( dec_sensor_get_dev(), INTPRI_SENSOR, dec_sensor_isr, 0 );
        uio_int_enable( dec_sensor_get_dev() );
    }
    
    ++g_dec_irq_enabled;

    ASSERT(SENSOR_REGS_INT_EN_EDGE_INT_EN_SHIFT == 0);
    sensors->int_en = sensors->int_en | (1 << sensor->pin_number);
}

void
dec_sensor_disable_isr( dec_sensor_t *sensor )
{
    DEC_SENSOR_REGS_t* sensors = dec_sensor_get_regs_base();
    ASSERT(sensors);
    ASSERT(sensor && sensor->pin_number < 19);
    //ASSERT(sensor->callback);

    --g_dec_irq_enabled;

    ASSERT(SENSOR_REGS_INT_EN_EDGE_INT_EN_SHIFT == 0);  // 
    sensors->int_en = sensors->int_en & ~(1 << sensor->pin_number);

    if ( g_dec_irq_enabled == 0 )
    {
        uio_int_disable( dec_sensor_get_dev() );
        uio_int_detach( dec_sensor_get_dev() );
    }
}


uint32_t dec_sensor_cnt[19] = { 0, };
static void 
dec_sensor_isr( int32_t interrupt_count, void *context )
{
    uint32_t n;
    uint32_t bit;
    DEC_SENSOR_REGS_t* sensors = dec_sensor_get_regs_base();
    ASSERT(sensors);
    uint32_t enabled = sensors->int_en;
    uint32_t pending = sensors->int_st;

    // make it a one shot for testing...
    // sensors->int_en = ~(pending & enabled);
    sensors->int_cl = pending & enabled;

    for ( n = 0, bit = 1; n < 19; n++, bit <<= 1 )
    {
        if ( g_dec_sensors[n].active && 
             ( enabled & bit ) && 
             ( pending & bit ) && 
             g_dec_sensors[n].callback )
        {
            ++dec_sensor_cnt[n];
            (g_dec_sensors[n].callback)(g_dec_sensors[n].callback_private_data);
            if(g_dec_sensors[n].cb)
                (g_dec_sensors[n].cb)(g_dec_sensors[n].cb_private_data);
        }
    }
}


bool dec_sensor_poll(const dec_sensor_t *sensor)
{
    ASSERT(sensor);
    ASSERT(sensor->pin_number < 19);
    if (sensor->active)
    {
        ASSERT(sensor->gpio == 0);
        // if the outsense invert bit is set we also invert the register read so
        // that we get the same sense that would be passed along on the output line
        // it would be nice if the hardware did this for us, but...
        if(*sensor->cfg & SENSOR_REGS_CFG0_OUTSENSE_MASK)
        {
            return( (sensor->sensors->sensor_out & (1 << sensor->pin_number)) == 0 ); // true low; false high
        }
        else
        {
            return( (sensor->sensors->sensor_out & (1 << sensor->pin_number)) != 0 ); // true high; false low
        }
    }

    ASSERT(sensor->gpio != 0);
    uint32_t val;
    gpio_read(sensor->gpio, &val);
    
    return( val ); 
}


#include "stdio.h"
int dec_sensor_block(dec_sensor_t *sensor, uint32_t msec )
{
    //ASSERT(sensor && sensor->active && sensor->pin_number < 19);
    ASSERT(sensor && sensor->pin_number < 19);
    if (sensor->callback)
    {
        struct timespec abs_timeout;
        
        while (0 == sem_trywait(&sensor->semaphore))
        {
            //keep clearing
	  DBG_LOG_ERR("sensor clear semaphore %d \n",sensor->pin_number);
        }
        
        // convert from relative OS tick timeout to absolute time.
        clock_gettime( CLOCK_REALTIME, &abs_timeout );
	DBG_LOG_ERR("sensor wait semaphore %d %d %d \n", sensor->pin_number, abs_timeout.tv_sec, abs_timeout.tv_nsec);
        posix_calc_future_usec( &abs_timeout, msec*1000 );

	DBG_LOG_ERR("sensor wait semaphore %d %d %d \n", sensor->pin_number, abs_timeout.tv_sec, abs_timeout.tv_nsec);
        int err = sem_timedwait( &sensor->semaphore, &abs_timeout );
	DBG_LOG_ERR("sensor done semaphore %d err %d %d \n",sensor->pin_number, err, errno);
        if (err == 0)
            return 0;
        else {
	    perror("sensor semaphore \n");
            return msec;
	}
    }
    return -1; // bad call 
}

void dec_sensor_unblock( void *cb_data )
{
    dec_sensor_t * sensor = (dec_sensor_t *) cb_data;
    DBG_LOG_ERR("sensor unblock %d state %d \n", sensor->pin_number, dec_sensor_poll(sensor));
    sem_post(&sensor->semaphore);
}

// block 
int dec_sensor_block_state(dec_sensor_t *sensor, bool sensor_state, uint32_t timeout_ticks )
{
    bool cur_sensor_state;
    uint32_t timeout = timeout_ticks;
    ASSERT(sensor && sensor->pin_number < 19);
    dbg_printf( "this is test code don't use it.");
    cur_sensor_state = dec_sensor_poll(sensor);
    while(cur_sensor_state != sensor_state && timeout > 0)
    {
        posix_sleep_ms(1);
        if(timeout != -1)
        {
            timeout--;
        }
        cur_sensor_state = dec_sensor_poll(sensor);
    }
    return (cur_sensor_state == sensor_state) ? 0 : timeout_ticks;
}


uint32_t 
sensor_filter_config(uint32_t fall_low_weight, 
                     uint32_t fall_hi_weight,
                     uint32_t rise_low_weight,
                     uint32_t rise_hi_weight
    )
{
    uint32_t fcfg = 0;    
    //ASSERT( fall_low_weight < 16 );
    fcfg |= fall_low_weight << (8 * 3);
    fcfg |= fall_hi_weight << (8 * 2);
    fcfg |= rise_low_weight << (8 * 1);
    fcfg |= rise_hi_weight << (8 * 0);

    return fcfg;
}


uint32_t
sensor_config(  uint32_t alt_in,
                uint32_t out_sense,
                uint32_t time_base,
                uint32_t interupt_type,
                uint32_t out_type
    )
{
    uint32_t cfg = 0;
    cfg = SENSOR_REGS_CFG0_ALT_IN_REPLACE_VAL(cfg,alt_in);
    cfg = SENSOR_REGS_CFG0_OUTSENSE_REPLACE_VAL(cfg,out_sense);
    cfg = SENSOR_REGS_CFG0_TIMEBASE_SEL_REPLACE_VAL(cfg,time_base); 
    cfg = SENSOR_REGS_CFG0_INTTYPE_REPLACE_VAL(cfg,interupt_type); //SENSOR_INTTYPE_Fall); // fall?
    cfg = SENSOR_REGS_CFG0_OUTTYPE_REPLACE_VAL(cfg,out_type);

    return cfg;
}

#if defined( HAVE_CMD ) && defined ( DEBUG )
static int dec_sensor_block_test_cmdcb( int argc, char *argv[] )
{
    int msec;
    dec_sensor_t *sensor;
    int t, t2;

    if ( argc != 3 || argv[1] == 0 )
    {
        return CMD_USAGE_ERROR;
    }
    
    sensor = dec_sensor_by_name( argv[1] );
    msec = atoi(argv[2]);
    
    if ( !sensor || msec < 0 || msec > 50000 )
    {
        cmd_printf(" sensor %s msec %d not found \n", argv[1], msec );
        return  CMD_USAGE_ERROR;
    }
    t = timer_get_time_usec();
    cmd_printf(" blocking on sensor %s  for %d msec val %d @ %d \n", 
	       argv[1], msec, dec_sensor_poll( sensor), t );
    dec_sensor_enable_isr( sensor );
    dec_sensor_block(sensor, msec);
    dec_sensor_disable_isr( sensor );
    t2 = timer_get_time_usec() ;
    cmd_printf(" after blocking on sensor %s  for %d msec val %d @ %d %d\n", 
	       argv[1], msec, dec_sensor_poll( sensor), t, t2 - t);
    return CMD_OK;
}

static int dec_sensor_looping_test_cmdcb( int argc, char *argv[] )
{
    int i;
    dec_sensor_t *sensor;
    
    if ( argc != 2 || argv[1] == 0 )
    {
        return CMD_USAGE_ERROR;
    }
    
    sensor = dec_sensor_by_name( argv[1] );
    if ( !sensor )
    {
        cmd_printf(" sensor %s not found \n", argv[1] );
        return  CMD_USAGE_ERROR;
    }
    
    dec_sensor_enable_isr( sensor );
    for (i = 10 * 10; i > 0; --i )
    {
        cmd_printf("sensor isr %d val %d sensor %d \n", 
                   dec_sensor_cnt[0], 
                   i, 
                   dec_sensor_poll( sensor));
        posix_sleep_ticks(10);
    }

    cmd_printf("block sensor isr %d val %d sensor %d \n", 
               dec_sensor_cnt[0], 
               i, 
               dec_sensor_poll(sensor));

    dec_sensor_block(sensor, 5000);

    cmd_printf("block sensor isr %d val %d sensor %d \n", 
               dec_sensor_cnt[0], 
               i, 
               dec_sensor_poll(sensor));


    dec_sensor_block(sensor, 5000);

    cmd_printf("block sensor isr %d val %d sensor %d \n", 
               dec_sensor_cnt[0], 
               i, 
               dec_sensor_poll(sensor));
    dec_sensor_disable_isr( sensor );
    return CMD_OK;
}

static int dec_sensor_poll_all_cmdcb( int argc, char *argv[] )
{
    int i;
    const char *str;

    for (i = 0, str = dec_sensor_name_by_index(i); 
	 str ; 
	 i++, str = dec_sensor_name_by_index(i) )
    {
        cmd_printf(" dec_sensor [%d] %s:%d\n", i, str, dec_sensor_poll(dec_sensor_by_index(i)));
    }
    return CMD_OK;
}

static int dec_sensor_cmd_init( void )
{
    int cmd_res;
    cmd_res = cmd_register_cmd("sensor",
			       NULL,
			       NULL,
			       NULL,
			       NULL,
			       NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "sensor",
				   "poll",
				   "poll all configured sensors",
				   NULL,
				   NULL,
				   dec_sensor_poll_all_cmdcb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "sensor",
				   "loop",
				   "test a user modify-able sensor by name ",
				   "<sensor name>",
				   NULL,
				   dec_sensor_looping_test_cmdcb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "sensor",
				   "block",
				   "test a user modify-able sensor by name ",
				   "<sensor name> <msec timeout>",
				   NULL,
				   dec_sensor_block_test_cmdcb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    return 0;
}




#endif
