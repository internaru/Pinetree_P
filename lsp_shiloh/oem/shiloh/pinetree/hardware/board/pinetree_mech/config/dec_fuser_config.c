/*
 * ======================================================================
 * Copyright (c) 2010-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ======================================================================
 */

/// dec fuser configuration

#include <string.h> // for memset
#include <sys/mman.h>
#include <stdint.h>
#include "lassert.h"
#include "dec_fuser.h"
#include "dec_fuser_priv.h"
#include "uio_lib_api.h"
#include "logger.h"
#include "debug.h"
#include "stdlib.h"
#include "asic.h"
#include "dec_adc_api.h"

//#define DBG_PRFX "dec_fuser: "
//#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | LOGGER_SUBMODULE_BIT( 5 )
 


static dec_fuser_config_t fuser_config = 
{
    .output_mux = 0x03,                 // power on pin ctrl[2], fuser 1  // ctrl[0] : LVPwr
    .power_polarity = 1,                // active high
    .power_output_enable = 0x01,           // fuser 1 enable
    .power_output = 0x01,        	   // on
    .fuser_over_temp_polarity = 0,      // active low
    .fuser_over_temp_enable = 0,
    .fuser_output_polarity = 0x02      // fuser 1 active high, 
};

// fuser service callbacks
static void dec_fuser_servicefuser( dec_fuser_t *dec_fuser );


#define MAX_FUSERS_USED
static dec_fuser_t my_fusers[ MAX_FUSERS_USED + 1]; 
static dec_fuser_t *myfusers[] = { 
    &my_fusers[0], 
    0
};
const const char * dec_fuser_key_table[] = 
{
    "main_fuser",
    0
};
KEY_TABLE_DEFINE_PRIVATE_3( dec, fuser, dec_fuser_t, dec_fuser_key_table, myfusers );


#define MAX_TEMPS 92  // number of different target temperatures. /* [KSF050201] */
#define MAX_POINTS_PER_CURVE 1 // current algorithm only needs target temperature point.

static const  voltage_power_pair_t 
normal_target_voltages[MAX_TEMPS][MAX_POINTS_PER_CURVE] = 
{
	/* [KSF032606] DEC temp table */
    {{ .v = 2000, .p = 0 },}, //0
    
    {{ 1980 , 100 },}, //23
    {{ 1970 , 100 },}, //42
    {{ 1960 , 100 },}, //55
    {{ 1950 , 100 },}, //64
    {{ 1940 , 100 },}, //72
    {{ 1930 , 100 },}, //79
    {{ 1920 , 100 },}, //84
    {{ 1910 , 100 },}, //90  
    {{ 1900 , 100 },}, //94
    {{ 1890 , 100 },}, //99

    {{ 1880 , 100 },}, //103
    {{ 1870 , 100 },}, //106
    {{ 1860 , 100 },}, //110
    {{ 1850 , 100 },}, //113
    {{ 1840 , 100 },}, //117
    {{ 1830 , 100 },}, //121
    {{ 1820 , 100 },}, //123
    {{ 1810 , 100 },}, //125  
    {{ 1800 , 100 },}, //128
    {{ 1790 , 100 },}, //131
    
    {{ 1780 , 100 },}, //133
    {{ 1770 , 100 },}, //136
    {{ 1760 , 100 },}, //138
    {{ 1750 , 100 },}, //140
    {{ 1740 , 100 },}, //143
    {{ 1730 , 100 },}, //145
    {{ 1720 , 100 },}, //147
    {{ 1710 , 100 },}, //149  
    {{ 1700 , 100 },}, //151
    {{ 1690 , 100 },}, //153
    
    {{ 1680 , 100 },}, //155
    {{ 1670 , 100 },}, //157
    {{ 1660 , 100 },}, //159
    {{ 1650 , 100 },}, //161
    {{ 1640 , 100 },}, //163
    {{ 1630 , 100 },}, //165
    {{ 1620 , 100 },}, //166
    {{ 1610 , 100 },}, //168  
    {{ 1600 , 100 },}, //170
    {{ 1590 , 100 },}, //172
    
    {{ 1580 , 100 },}, //173
    {{ 1570 , 100 },}, //175
    {{ 1560 , 100 },}, //177
    {{ 1550 , 100 },}, //178
    {{ 1540 , 100 },}, //180
    {{ 1530 , 100 },}, //182
    {{ 1520 , 100 },}, //183
    {{ 1510 , 100 },}, //185  
    {{ 1500 , 100 },}, //186
    {{ 1490 , 100 },}, //188
    
    {{ 1480 , 100 },}, //190
    {{ 1470 , 100 },}, //191
    {{ 1460 , 100 },}, //193
    {{ 1450 , 100 },}, //194
    {{ 1440 , 100 },}, //196
    {{ 1430 , 100 },}, //198
    {{ 1420 , 100 },}, //199
    {{ 1410 , 100 },}, //200  
    {{ 1400 , 100 },}, //202
    {{ 1390 , 100 },}, //203
 
    {{ 1380 , 100 },}, //205
    {{ 1370 , 100 },}, //206
    {{ 1360 , 100 },}, //208
    {{ 1350 , 100 },}, //209
    {{ 1340 , 100 },}, //211
    {{ 1330 , 100 },}, //212
    {{ 1320 , 100 },}, //214
    {{ 1310 , 100 },}, //215  
    {{ 1300 , 100 },}, //217
    {{ 1290 , 100 },}, //218 
    
    {{ 1280 , 100 },}, //220
    {{ 1270 , 100 },}, //221
    {{ 1260 , 100 },}, //223
    {{ 1250 , 100 },}, //224
    {{ 1240 , 100 },}, //225
    {{ 1230 , 100 },}, //227
    {{ 1220 , 100 },}, //228
    {{ 1210 , 100 },}, //230  
    {{ 1200 , 100 },}, //231
    {{ 1190 , 100 },}, //233
    
    {{ 1180 , 100 },}, //234
    {{ 1170 , 100 },}, //236
    {{ 1160 , 100 },}, //237
    {{ 1150 , 100 },}, //239
    {{ 1140 , 100 },}, //240
    {{ 1130 , 100 },}, //242
    {{ 1120 , 100 },}, //243
    {{ 1110 , 100 },}, //245  
    {{ 1100 , 100 },}, //246
    {{ 1090 , 100 },}, //248                            

    {{ 1080 , 100 },}  //249
};

static const voltage_power_pair_t
heavy_target_voltages[MAX_TEMPS][MAX_POINTS_PER_CURVE] = 
{
    { // OFF
    { .v = 2000, .p = 0 }, 
    },


    {  // pre heat
        { 2000, 100 },   
    },

    {  //  warm
        { 2000, 100 }, 
    },

    {  // full heat
      { .v = 2000, .p = 100 }, 
    },

};

static const voltage_power_pair_t
light_target_voltages[MAX_TEMPS][MAX_POINTS_PER_CURVE] = 
{
    { // OFF
    { .v = 2000, .p = 0 }, 
    },

   {  // pre heat
    { 2000, 100 },   
    },

    {  //  warm
        { 2000, 100 }, 
    },

    {  // full heat
    { .v = 2000, .p = 100 }, 
    },

};
static dec_fuser_temp_t my_fusers_temp[MAX_TEMPS];
static dec_fuser_temp_t *myfusers_temps[MAX_TEMPS] = {0};
const const char * dec_fuser_temp_key_table[] = 
{
    "temp_off",     
    "temp_warm",   
    "temp_hot",    
    "temp_hotter", 
    0
};
KEY_TABLE_DEFINE_PRIVATE_2( dec_fuser_temp, dec_fuser_temp_t, dec_fuser_temp_key_table, myfusers_temps );




#define MAX_WEIGHTS 3  // different paper weights might use different target temperatures.
static dec_fuser_weight_t my_fusers_weights[MAX_WEIGHTS];
static dec_fuser_weight_t *myfusers_weights[MAX_WEIGHTS] = {0};

const const char * dec_fuser_weights_key_table[] = 
{
   "normal_paper",
   "heavy_paper",
   "light_paper",
   // "normal_paper_fuser2",
   // "heavy_paper_fuser2",
   // "light_paper_fuser2",
    0
};

KEY_TABLE_DEFINE_PRIVATE_2( dec_fuser_weight, dec_fuser_weight_t, dec_fuser_weights_key_table, myfusers_weights );

#ifdef ADD_ZC
uint32_t fuser_mode_request;
uint32_t fuser_mode_current;
uint8_t  fuser_step;
enum {
    FUSER_OFF = 0,
    FUSER_ON_WARMUP = 1,
    FUSER_ON_READY = 2,
    FUSER_ON_PRINT = 3,
    FUSER_ON = 4
};

enum {
    TBL_WARMUP,
    TBL_READY,
    TBL_PRINT,
    TBL_MAX
};

const uint32_t fuser_pulse_width = 1000;
const uint8_t fuser_step_max[TBL_MAX] = { 50, 50, 50 };
const uint32_t fuser_pulse_offset[TBL_MAX][50] ={
    /* warmup */
    { 6500, 6500, 6500, 6500, 6500,  6500, 6500, 6500, 6500, 6500,
      6000, 6000, 6000, 6000, 6000,  6000, 6000, 6000, 6000, 6000,
      5500, 5500, 5500, 5500, 5500,  5500, 5500, 5500, 5500, 5500,
      5000, 5000, 5000, 5000, 5000,  5000, 5000, 5000, 5000, 5000,
      4500, 4500, 4500, 4500, 4500,  4500, 4500, 4500, 4500, 4500 },
    /* ready */
    { 6500, 6500, 6500, 6500, 6500,  6500, 6500, 6500, 6500, 6500,
      6000, 6000, 6000, 6000, 6000,  6000, 6000, 6000, 6000, 6000,
      5500, 5500, 5500, 5500, 5500,  5500, 5500, 5500, 5500, 5500,
      5000, 5000, 5000, 5000, 5000,  5000, 5000, 5000, 5000, 5000,
      4500, 4500, 4500, 4500, 4500,  4500, 4500, 4500, 4500, 4500 },
    /* printing */
    { 6500, 6500, 6500, 6500, 6500,  6500, 6500, 6500, 6500, 6500,
      6000, 6000, 6000, 6000, 6000,  6000, 6000, 6000, 6000, 6000,
      5500, 5500, 5500, 5500, 5500,  5500, 5500, 5500, 5500, 5500,
      5000, 5000, 5000, 5000, 5000,  5000, 5000, 5000, 5000, 5000,
      4500, 4500, 4500, 4500, 4500,  4500, 4500, 4500, 4500, 4500 }
};
#endif

void *dec_fuser_get_dev()
{
    static void *fuser_dev = NULL;

    if(fuser_dev == NULL)
    {
    	fuser_dev = uio_open_dev("dec_fuser");
        XASSERT(fuser_dev != NULL, errno);
    }

    return fuser_dev;
}



/**
 * \brief Get dec fuser registers memory mapped to user space using dec_fuser UIO kernel driver.
 * 
 */
FUSER_REGS_t* dec_fuser_get_regs_base()
{
    static uio_dev_t *fuser_dev = NULL;
    static FUSER_REGS_t* fuser_regs = NULL;

    if(fuser_dev == NULL)
    {
        fuser_dev = uio_open_dev("dec_fuser");
        XASSERT(fuser_dev != NULL, errno);
    }

    if(fuser_regs == NULL)
    {
        // get dec_fuser map
        uio_map_t *map = uio_get_map(fuser_dev, 0);

        if (map != NULL)
        {
            fuser_regs = map->virt_addr;
        }
    }

    DBG_PRINTF_NOTICE("%s = %#x\n", __func__, fuser_regs);

    return fuser_regs;
}

void dec_fuser_local_init(void)
{
    int i;
    uint32_t fepr_reg_val = 0;
    uint32_t filter = 0; /* [KSF050201] */
    uint32_t lockout = 0; /* [KSF050201] */
    
    FUSER_REGS_t* fuser_regs = dec_fuser_get_regs_base();

    memset(&my_fusers[0], 0, sizeof(dec_fuser_t));

    for (i = 0; i < MAX_TEMPS;i ++) 
    {
        my_fusers_temp[i].index = i;
        myfusers_temps[i] = &my_fusers_temp[i];
    }

    for (i = 0; i < MAX_WEIGHTS;i ++) 
    {
        myfusers_weights[i] = &my_fusers_weights[i];  
        myfusers_weights[i]->name = dec_fuser_weights_key_table[i];

    }
    // determine fuser tables for current fuser & engine

    // fuser 0 
    myfusers_weights[0]->table = (voltage_power_pair_t *)normal_target_voltages;
    myfusers_weights[1]->table = (voltage_power_pair_t *)heavy_target_voltages;
    myfusers_weights[2]->table = (voltage_power_pair_t *)light_target_voltages;
    // fuser 1 example uses the same temperature targets for the second fuser.
    // myfusers_weights[3]->table = (voltage_power_pair_t *)normal_target_voltages;
    // myfusers_weights[4]->table = (voltage_power_pair_t *)heavy_target_voltages;
    // myfusers_weights[5]->table = (voltage_power_pair_t *)light_target_voltages;
    my_fusers[0].num_points_curve = MAX_POINTS_PER_CURVE;
    my_fusers[0].fuser_service_callback = dec_fuser_servicefuser;

#ifdef ADD_ZC
    fuser_mode_request = FUSER_OFF;
    fuser_mode_current = FUSER_OFF;
    fuser_step = 0;
#endif //ADD_ZC

    // configure fuser pinouts and polarity
    fepr_reg_val = FUSER_FEPR_OUTPUT_MUX_REPLACE_VAL(fepr_reg_val, fuser_config.output_mux);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_POLARITY_REPLACE_VAL(fepr_reg_val, fuser_config.power_polarity);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_OE_REPLACE_VAL(fepr_reg_val, fuser_config.power_output_enable);
    fepr_reg_val = FUSER_FEPR_LVPOWERON_REPLACE_VAL(fepr_reg_val, fuser_config.power_output);
    fepr_reg_val = FUSER_FEPR_FUSEROVERTEMP_POLARITY_REPLACE_VAL(fepr_reg_val, fuser_config.fuser_over_temp_polarity);    
    fepr_reg_val = FUSER_FEPR_FUSEROVERTEMP_IE_REPLACE_VAL(fepr_reg_val, fuser_config.fuser_over_temp_enable);
    fepr_reg_val = FUSER_FEPR_FUSER_POLARITY_REPLACE_VAL(fepr_reg_val, fuser_config.fuser_output_polarity);
    
    fuser_regs->FEPR = fepr_reg_val;

    my_fusers[0].paper_weight = dec_fuser_weight_by_name("normal_paper"); 
    // default temperature is OFF at index 0 ALWAYS.

    //the rest of the initialization is fuser independent.
//    dec_fuser_initialize( &my_fusers[0], 1 /* hw fuser block number*/ );
#if 1 /* [KSF050201] - start */
    filter = FUSER_ZCFR_FALL_LO_WEIGHT_REPLACE_VAL(0,0xa);
    filter = FUSER_ZCFR_FALL_HI_WEIGHT_REPLACE_VAL(filter,0x2);
    filter = FUSER_ZCFR_RISE_LO_WEIGHT_REPLACE_VAL(filter,0x2);
    filter = FUSER_ZCFR_RISE_HI_WEIGHT_REPLACE_VAL(filter,0xa);
    filter = FUSER_ZCFR_THRESHOLD_REPLACE_VAL(filter, 100);
    filter = FUSER_ZCFR_TIMEBASE_SEL_REPLACE_VAL(filter,0x2);//1usec timebase
    filter = FUSER_ZCFR_OUTSENSE_REPLACE_VAL(filter, 0x0);
#if 1
    filter = FUSER_ZCFR_OUTTYPE_REPLACE_VAL(filter,0x2); //one clock pulse on rising edge
    lockout = FUSER_ZCLR_ZC_INT_TYPE_REPLACE_VAL(0,0x1);//falling edge ??used??
    lockout = FUSER_ZCLR_LOCKOUT_TIMEBASE_REPLACE_VAL(lockout,0x3);//100usec
    //lockout = FUSER_ZCLR_CLR_LOCKOUT_REPLACE_VAL(lockout,0x1);//clr lock
    lockout = FUSER_ZCLR_LOCKOUT_COUNT_REPLACE_VAL(lockout,50);//50 ticks 5000uSec total
#else
    filter = FUSER_ZCFR_OUTTYPE_REPLACE_VAL(filter,0x2);
    lockout = FUSER_ZCLR_ZC_INT_TYPE_REPLACE_VAL(0,0x1);//falling edge ??used??
	lockout = FUSER_ZCLR_LOCKOUT_TIMEBASE_REPLACE_VAL(lockout,0x3);
	lockout = FUSER_ZCLR_CLR_LOCKOUT_REPLACE_VAL(lockout,0x1);//clr lock
	lockout = FUSER_ZCLR_LOCKOUT_COUNT_REPLACE_VAL(lockout,50);//50 ticks 5000uSec total
#endif
    my_fusers[0].syncPWMMode = true;
//    dec_fuser_initialize( &my_fusers[0], 0 );
    dec_fuser_initialize( &my_fusers[0], 1 );
    
    dec_fuser_set_phase_config( &my_fusers[0], 600, 600);
    dec_fuser_set_zero_crossing(&my_fusers[0],
                                filter, //< ZCFR
                                lockout //<ZCLR
                                );    
#endif /* [KSF050201] - end */
}


#define GAIN 1
#define GAIN_DIV 2
#define I_GAIN  4 // 12
#define I_GAIN_DIV 100

uint32_t dec_fuser_get_power( dec_fuser_t *dec_fuser, uint32_t current_voltage )
{
    static int32_t integrate = 1;
    int32_t error, ctrl;

    if (dec_fuser->target_voltage == 0) 
      return 0;  // this really is an error in the table.

    error = (int32_t) current_voltage - dec_fuser->target_voltage;

    ctrl = (error * GAIN)/GAIN_DIV;


    if(ctrl < -100)
    {
       ctrl = 0; 
       integrate = 0;
    }
    else if(ctrl > 100)
    {
        ctrl = 100;
        integrate = 0;
    }
    else if(ctrl < 0)
    {
        integrate += ctrl * 4;
        ctrl += (integrate * I_GAIN)/I_GAIN_DIV;
    }
    else 
    {
        integrate += ctrl;
        ctrl += (integrate * I_GAIN)/I_GAIN_DIV;
    } 

    // clamp to 0 - 100 percent power range.
    ctrl = ctrl < 0 ? 0 : ctrl > 100 ? 100 : ctrl;
    if ( ctrl ) {
      DBG_MEMLOG_NOTICE(" voltage %d error %d integrate %d target_voltage %d ctrl %d\n", 
		       current_voltage, error, integrate, dec_fuser->target_voltage, ctrl);
    }
    

    return ctrl;
}

#ifdef ADD_ZC

static void dec_fuser_servicefuser( dec_fuser_t *dec_fuser )
{
    uint8_t changed = 0;
    uint8_t index = 0;

    if ( (fuser_mode_current == FUSER_OFF && fuser_mode_request != FUSER_OFF) ||
            (fuser_mode_current != FUSER_OFF && fuser_mode_request == FUSER_OFF) ) {
        changed = 1;
        fuser_mode_current = fuser_mode_request;
        fuser_step = 0;
        DBG_PRINTF_ERR("bal L%d\n", __LINE__);

    }
    else if ( fuser_mode_current == FUSER_ON_WARMUP ||
            fuser_mode_current == FUSER_ON_READY  ||
            fuser_mode_current == FUSER_ON_PRINT ) {
        index = fuser_mode_current-FUSER_ON_WARMUP;
        if ( fuser_step >= fuser_step_max[index] ) {
            changed = 1;
            fuser_mode_current = FUSER_ON;
            fuser_step = 0;
            DBG_PRINTF_ERR("bal L%d\n", __LINE__);
        }
    }

    switch ( fuser_mode_current ) {
        case FUSER_OFF:
            if (changed) {
                DBG_PRINTF_ERR("bal L%d:OFF\n", __LINE__);
                dec_fuser_set_duty_cycle( dec_fuser, 0 );
            }
            break;
        case FUSER_ON:
            if (changed) {
                DBG_PRINTF_ERR("bal L%d:ON\n", __LINE__);
                dec_fuser_set_phase_config( dec_fuser, fuser_pulse_width, 0 );
                dec_fuser_set_duty_cycle( dec_fuser, 100 );
            }
            break;
        case FUSER_ON_WARMUP:
        case FUSER_ON_READY:
        case FUSER_ON_PRINT:
            DBG_PRINTF_ERR("bal L%d: %d, step: %d\n", __LINE__, fuser_mode_current, fuser_step);
            index = fuser_mode_current-FUSER_ON_WARMUP;
            dec_fuser_set_phase_config( dec_fuser, fuser_pulse_width, fuser_pulse_offset[index][fuser_step]);
            fuser_step ++;
            if (changed) {
                dec_fuser_set_duty_cycle( dec_fuser, 100 );
            }
            break;
        default:
            DBG_PRINTF_ERR("bal L%d:default\n", __LINE__);
            dec_fuser_set_duty_cycle( dec_fuser, 0 );
            break;
    }

    if ( dec_fuser->fatal_error ) {
        DBG_PRINTF_ERR("bal L%d\n", __LINE__);
        dec_fuser_set_duty_cycle( dec_fuser, 0 );
    }
        DBG_PRINTF_DEBUG("bal L%d\n", __LINE__);
}

#else //ADD_ZC

static void dec_fuser_servicefuser( dec_fuser_t *dec_fuser )
{
    uint32_t voltage;
    uint32_t power = 0; // 0 - 100 percentage of fuser power to deliver for next interval
    static uint32_t cnt = 0;

    

    // insure that we service the fuser fast enough
    if (FUSER_FIPR_TIMEOUT_MASK_SHIFT(dec_fuser->regs->FIPR) != 0) {
        DBG_PRINTF_WARNING("SLOW service_fuser %x\n", 
			   FUSER_FIPR_TIMEOUT_MASK_SHIFT(dec_fuser->regs->FIPR));
    }

    dec_fuser->tempHistory[0] = dec_fuser->tempHistory[1];
    dec_fuser->tempHistory[1] = dec_fuser->tempHistory[2];
    dec_fuser->tempHistory[3] = dec_fuser->tempHistory[4];

    // read voltage as 0-1023 integer scaling of Max voltage: i = v * 1023/3.3v
    voltage = dec_adc_scaled_read_channel_mV( dec_adc_by_index(0) );

    dec_fuser->tempHistory[4] = voltage;

    // convert fuser temperature history and fuser command to a fuser power level.
    power = dec_fuser_get_power( dec_fuser, voltage );

    dec_fuser->powHistory[0] += power;
    dec_fuser->powHistory[1] = dec_fuser->powHistory[2];
    dec_fuser->powHistory[2] = dec_fuser->powHistory[3];
    dec_fuser->powHistory[3] = dec_fuser->powHistory[4];

    ++cnt;
    
    if ( dec_fuser->powHistory[4] == power && power)
    {     
        // don't change the fuser settings unless you have to as it will be slower
        // to turn it off and back on, than to run it continuously.
        dec_fuser->regs->FCR0 = dec_fuser->regs->FCR0; // I've been serviced.   
    }
    else 
    {
        if (dec_fuser->fatal_error) 
            power = 0;
    
        //change them.
        if (power) { // don't spew if off.
            DBG_PRINTF_NOTICE("service_fuser %d hv[%d] volt[%d] power[%d] ticks %d p[%d:%d:%d:%d:%d]\n", 
			      dec_fuser->target_temp->index,
			      0, voltage, power, cnt,
			      dec_fuser->powHistory[0],                 
			      dec_fuser->powHistory[1],                 
			      dec_fuser->powHistory[2],                 
			      dec_fuser->powHistory[3],                 
			      dec_fuser->powHistory[4]
			      ); 
        }
        dec_fuser_set_duty_cycle( dec_fuser, power );
    }    

    // if( voltage < dec_fuser->target_voltage && dec_fuser->target_temp->index != 0 ) 
    if( power <= 25 && dec_fuser->target_temp->index != 0 ) 
    {
        // DBG_PRINTF_DEBUG("at temp power %d voltage %d \n", power, voltage);
        if( dec_fuser->at_temp_callback ) {
            (dec_fuser->at_temp_callback)(dec_fuser->at_temp_data);
        }
    }

    dec_fuser->powHistory[4] = power;
}

#endif //ADD_ZC
