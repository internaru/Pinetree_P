/**
 * \file scansen-target.c
 *
 * \brief  Scan Sensor driver
 *
 * scan sensor driver - implements functions to use with scan sensor
 *
 */
/*
 * 
 *
 * ============================================================================
 * Copyright (c) 2008 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <string.h>

#include "lassert.h"
#include "regAddrs.h"
#include "hwconfig_api.h"
#include "cpu_api.h"
#include "gpio_api.h"
#include "gpio_config_api.h"

#ifdef HAVE_DBG_CALLBACK_CMDS
#include "scos_cmd_proc.h"
#endif
#include "scantypes.h"
#include "scandbg.h"
#include "scanhw.h"
#include "scanif.h"
#include "scan.h"
#include "cisx.h"
#include "scansen.h"
#include "afe.h"
#include "scansen-target.h"
#include "scif_afe_wm8213.h"
#include "utils.h"
#include "debug.h"
#include "../../../../../common/include/spec.h"


/**
 * \brief The clock frequency of the CIS for scan mode
 *
 * This is the return value of get_default_sensor_freq()
 * which is used (along with the scan block's clock) to set the pixel period.
 * NOTE: changing the sensor clock frequency will more than likely
 *  change the various clock edges - be ready to re-tune those.
 */

#define SENSOR_CLOCK_FREQ_KHZ 4167

static scan_cmode_t        scansen_cmode;
static uint32_t            scansen_dpi;


extern int cis_select_state;

#ifdef HAVE_DBG_CALLBACK_CMDS
static bool          scansen_p1clk_debug_enable = false;
static uint32_t      scansen_p1clk_edges[2];
static bool          scansen_clamp_debug_enable = false;
static uint32_t      scansen_clamp_edges[2];
static bool          scansen_ldata_debug_enable = false;
static uint32_t      scansen_ldata_edges[12];
static uint32_t      scansen_ldata_edges_a2[12];
static bool          scansen_vsclk_debug_enable = false;
static uint32_t      scansen_vsclk_edges[2];
#endif



/**
 * \brief product specific sensor margins
 *
 */

// note that for sensors that require cisx (like this one), the inactive pixel field must
// be set to 0
#if 0
static const scan_sensor_margin_t scansentype1_sensor_margin_table[] = {
    //collect all pixels, including inactive pixels
    {300,  0, 2592,  4+1},
    {600,  0, 5184-2*3,  4+1},
    {1200, 0, 10368, 4+1},    
    /* end of list marker */
    { -1, 0, 0, 0 }
};
#endif

static const scan_sensor_margin_t scansentype1_sensor_margin_table[] = {
    //collect all pixels, including inactive pixels
    {300,  0, 2592, 4+1},
    {600,  0, 5184-2*3, 4+1},
    {1200, 0, 10368, 4+1},    
    /* end of list marker */
    { -1, 0, 0, 0 }
};

static const scan_sensor_margin_t scansentype2_sensor_margin_table[] = {
    /* davep 18-Jul-2011 ; tuned ~14-Jul-2011 */
    {300,  120, 2580,  0},   
    {600,  120, 5160,  0},  
    {1200, 120, 10320, 0}, 
    {2400, 120, 20640, 0}, 
    /* end of list marker */
    { -1, 0, 0, 0 }
};


// note that the rising edge in the table needs to have an offset
// based on the rising edge of the start pulse.  See sensor_sp_edges[0]
// and offset based on that.
// Note that the edgeF isn't the actual time of the falling edge, it is
// the amount of time after the edgeR that the LED can remain on.
// (i.e. scansen_set_leds adds edgeF to edgeR)
#define START_PULSE_RISING_EDGE 0
static const scan_led_timing_t scansen_led_timing_table[] = {
    {SCAN_CMODE_MONO,  300,  72 + START_PULSE_RISING_EDGE,  989  },
    {SCAN_CMODE_MONO,  600,  72 + START_PULSE_RISING_EDGE,  1902 }, 
    {SCAN_CMODE_MONO,  1200, 72 + START_PULSE_RISING_EDGE,  4364 }, //don't care
    {SCAN_CMODE_COLOR, 300,  72 + START_PULSE_RISING_EDGE,  989  }, 
    {SCAN_CMODE_COLOR, 600,  72 + START_PULSE_RISING_EDGE,  2114 }, 
    {SCAN_CMODE_COLOR, 1200, 72 + START_PULSE_RISING_EDGE,  4364 }, //don't care
    /* end of list marker */
    {0, -1, 0, 0}
};

//------------------------------- old CIS -------------------------------------------
//#if 0
// scanx includes leading inactive and active pixels, not trailing inactive
// it includes all pixels from the 3 segments (leading inactive and active)
static const scan_scanx_conf_t scansen_scanx_table_old[] = {
    {SCAN_CMODE_MONO,  300,  90*3+2592  },  //2862
    {SCAN_CMODE_MONO,  600,  88*3+5184  }, 	//5448
    {SCAN_CMODE_MONO,  1200, 91*3+10368 }, 

    {SCAN_CMODE_COLOR, 300,  90*3+2592  }, 
    {SCAN_CMODE_COLOR, 600,  88*3+5184  }, 
    {SCAN_CMODE_COLOR, 1200, 91*3+10368 },
    /* end of list marker */
    {0, -1, 0}
};

static const struct scan_cisx_conf scansen_cisx_conf_table_old[] = {
    {300,   {3, {{90, 864, 0},
                 {90, 864, 0},
                 {90, 864, 0},
                },
             CISX_3CHAN_ORDER_012,
            },
    },
    {600,   {3, {{88, 1728, 0},
                 {88, 1728, 0},
                 {88, 1728, 0},
                },
             CISX_3CHAN_ORDER_012,
            },
    },
    {1200,  {3, {{91, 3456, 0},
                 {91, 3456, 0},
                 {91, 3456, 0},
                },
             CISX_3CHAN_ORDER_012,
            },
    },
    /* end of list marker */
    {-1,   {0, {{0, 0, 0},
                {0, 0, 0},
                {0, 0, 0},
               },
            0,
           },
    }    
}; 
//#endif
//-----------------------------------------------------------------------------------
//---------------------------- New CIS ----------------------------------------------
// scanx includes leading inactive and active pixels, not trailing inactive
// it includes all pixels from the 3 segments (leading inactive and active)
static const scan_scanx_conf_t scansen_scanx_table_new[] = {
    {SCAN_CMODE_MONO,  300,  82*3+2592  }, //82, 2838
    {SCAN_CMODE_MONO,  600,  80*3+5184  }, //80, 5424
    {SCAN_CMODE_MONO,  1200, 91*3+10368 }, 

    {SCAN_CMODE_COLOR, 300,  82*3+2592  }, //82
    {SCAN_CMODE_COLOR, 600,  80*3+5184  }, //80
    {SCAN_CMODE_COLOR, 1200, 91*3+10368 },
    /* end of list marker */
    {0, -1, 0}
};

static const struct scan_cisx_conf scansen_cisx_conf_table_new[] = {
    {300,   {3, {{82, 864, 0},
                 {82, 864, 0},
                 {82, 864, 0},
                },
#if PINETREE_L
             CISX_3CHAN_ORDER_120,
#else
             CISX_3CHAN_ORDER_012,
#endif
            },
    },
    {600,   {3, {{80, 1728, 0},
                 {80, 1728, 0},
                 {80, 1728, 0},
                },
#if PINETREE_L
             CISX_3CHAN_ORDER_120,
#else
             CISX_3CHAN_ORDER_012,
#endif
            },
    },
    {1200,  {3, {{91, 3456, 0},
                 {91, 3456, 0},
                 {91, 3456, 0},
                },
             CISX_3CHAN_ORDER_012,
            },
    },
    /* end of list marker */
    {-1,   {0, {{0, 0, 0},
                {0, 0, 0},
                {0, 0, 0},
               },
            0,
           },
    }    
}; 
//-----------------------------------------------------------------------------------


#define FLOAT_TO_DECIMAL(f)   ( (int)( (f)*100 ) )

/**
 * \brief  Q&D function to allow us to build with two different image sensors
 * and yet remain backward compatible with previous builds.
 *
 * \author David Poole
 * \date 25-May-2011
 *
 */
int scansen_get_sensor_id( void )
{
    return SCAN_SENSOR_TYPE1;
}



#ifdef HAVE_DBG_CALLBACK_CMDS
int scif_debug_callback(int argc, char *argv[])
{
    int        i, pos;
    uint32_t   temp;

    for (i=0; i<argc; i++)
    {
        dbg1("%s %d %s\n", __FUNCTION__, i, argv[i]);
    }

    if ( argc < 2 ) {
        return -1; 
    }
    pos = 1;

    if (strncmp(argv[pos], "ledexp", 7) == 0)
    {
        uint32_t tmp_pixels_per_line;
        uint32_t tmp_gateR;
        uint32_t tmp_gateG;
        uint32_t tmp_gateB;       
        uint32_t sensor_num=0;
    
        if (argc < 6)
        {
            dbg1("ledexp command error!!!!\n");  
            return 0;
        }
        else
        {
            if (argc == 7)
                sensor_num = str_mkint(argv[++pos],&sensor_num);
            
            str_mkint(argv[pos+1], &tmp_pixels_per_line);
            str_mkint(argv[pos+2], &tmp_gateR);
            str_mkint(argv[pos+3], &tmp_gateG);
            str_mkint(argv[pos+4], &tmp_gateB);
            scansen_set_leds(sensor_num, scansen_cmode, tmp_pixels_per_line, 
                             tmp_gateR,tmp_gateG,tmp_gateB);
            
        }
    }
    else
    if (strncmp(argv[pos], "p1clk", 6) == 0)
    {
        if (argc != 4)
        {
            dbg1("p1clk command error!!!!\n");  
            scansen_p1clk_debug_enable = false;
            return 0;
        }
        else
        {
            for (i=0; i<2; i++)
            {
                str_mkint(argv[pos+1+i], &temp);
                scansen_p1clk_edges[i] = temp;
                dbg1("scansen_p1clk_edges[%d]=%d\n", i, scansen_p1clk_edges[i]);
            }
            scansen_p1clk_debug_enable = true;
        }
    }
    else if (strncmp(argv[pos], "clamp", 6) == 0)
    {
        if (argc != 4)
        {
            dbg1("clamp command error!!!!\n");  
            scansen_clamp_debug_enable = false;
            return 0;
        }
        else
        {
            for (i=0; i<2; i++)
            {
                str_mkint(argv[pos+1+i], &temp);
                scansen_clamp_edges[i] = temp;
                dbg1("scansen_clamp_edges[%d]=%d\n", i, scansen_clamp_edges[i]);
            }
            scansen_clamp_debug_enable = true;
        }
    }
    else if (strncmp(argv[pos], "vsclk", 6) == 0)
    {
        if (argc != 4)
        {
            dbg1("vsclk command error!!!!\n");  
            scansen_vsclk_debug_enable = false;
            return 0;
        }
        else
        {
            for (i=0; i<2; i++)
            {
                str_mkint(argv[pos+1+i], &temp);
                scansen_vsclk_edges[i] = temp;
                dbg1("scansen_vsclk_edges[%d]=%d\n", i, scansen_vsclk_edges[i]);
            }
            scansen_vsclk_debug_enable = true;
        }
    }
    else if (strncmp(argv[pos], "ldata", 6) == 0)
    {
        if (argc != 14)
        {
            dbg1("ldata command error!!!! must have 12 edges specified\n");  
            scansen_ldata_debug_enable = false;
            return 0;
        }
        else
        {
            for (i=0; i<12; i++)
            {
                str_mkint(argv[pos+1+i], &temp);
                if (i<6)
                {
                    scansen_ldata_edges[i] = temp;
                    dbg1("scansen_ldata_edges[%d]=%d\n", i, scansen_ldata_edges[i]);
                }
                else
                {
                    scansen_ldata_edges_a2[i-6] = temp;
                    dbg1("scansen_ldata_edges_a2[%d]=%d\n", i-6, scansen_ldata_edges_a2[i-6]);
                }
            }
            scansen_ldata_debug_enable = true;
        }
    }
    else if (strncmp(argv[pos], "dump", 5) == 0)
    {
        for (i=0; i<2; i++)
        {
            dbg1("scansen_p1clk_edges[%d]=%d\n", i, scansen_p1clk_edges[i]);
        }
        for (i=0; i<2; i++)
        {
            dbg1("scansen_clamp_edges[%d]=%d\n", i, scansen_clamp_edges[i]);
        }
        for (i=0; i<2; i++)
        {
            dbg1("scansen_vsclk_edges[%d]=%d\n", i, scansen_vsclk_edges[i]);
        }
        for (i=0; i<12; i++)
        {
            dbg1("scansen_ldata_edges[%d]=%d\n", i, scansen_ldata_edges[i]);
            dbg1("scansen_ldata_edges_a2[%d]=%d\n", i, scansen_ldata_edges_a2[i]);            
        }
    }
    return 0;
}
#endif


/**
 * \brief Power up initialization of the scan sensor 
 *
 * Initializes the scan sensor and any associated internal structures, variables, etc.
 * This function will be called one time at system startup.  
 *
 * \retval scan_err_t any scan error that occured (if all is well, SCANERR_NONE)
 *
 * \remarks If there's nothing that needs to be done, just return SCANERR_NONE.
 **/
scan_err_t scansen_onetime_init(void)
{
#ifdef HAVE_DBG_CALLBACK_CMDS
    int retcode;
#endif

#ifdef HAVE_DBG_CALLBACK_CMDS
    scansen_p1clk_debug_enable = false;
    scansen_clamp_debug_enable = false;
    scansen_vsclk_debug_enable = false;
    scansen_ldata_debug_enable = false;
    retcode = cmd_register_cmd("scif", NULL, NULL, NULL, NULL, scif_debug_callback);
    XASSERT(retcode==0, retcode);
#endif    

    return SCANERR_NONE;
}


/**
 * \brief Return the margin table
 *
 * Uses a platform-specific call to get the margin table, then searches
 * through it to find the appropriate entry for the requested dpi, then
 * returns just that entry.
 *
 * \param[in] dpi  the dpi of the margin entry to be returned
 *
 * \param[out] margins  the margin entry that matches the requested dpi
 *
 * \retval scan_err_t any scan error that occured
 *  SCANERR_NONE - entry was found and returned
 *  SCANERR_INVALID_PARAM - margin table has invalid entry
 *  SCANERR_NO_ENTRY - the requested dpi was not found in the margin table
 *
 **/
scan_err_t scansen_get_margins(uint32_t dpi, scan_sensor_margin_t *margins)
{
    int   sensor_id;
    int   i;
    const scan_sensor_margin_t *sensor_margin_table;

    memset( margins, 0, sizeof(margins) );

    /* davep 18-Jul-2011 ; choose margins at run-ish time */
    sensor_id = scansen_get_sensor_id();
    if ( sensor_id==SCAN_SENSOR_TYPE1 ) {
        sensor_margin_table = scansentype1_sensor_margin_table;
    }
    else {
        XASSERT( sensor_id==SCAN_SENSOR_TYPE2, sensor_id );
        sensor_margin_table = scansentype2_sensor_margin_table;
    }

    for ( i=0 ; ; i++ ) {
        if ( sensor_margin_table[i].dpi==-1 ) {
            dbg1("%s invalid/unknown DPI %ld\n", __FUNCTION__, dpi);
            return SCANERR_INVALID_PARAM;
        }
        else if ( sensor_margin_table[i].dpi == dpi ) {
            memcpy( margins, &sensor_margin_table[i], sizeof(scan_sensor_margin_t) );

            /* davep 06-Dec-2010 ; XXX temp debug */
            dbg2("%s dpi=%d left=%d active=%d right=%d\n", __FUNCTION__,
                  margins->dpi, margins->left, margins->active, margins->right);

            return SCANERR_NONE;
        }
    }

    /* should never get here - the requested dpi was not found in the margin table */
    return SCANERR_NO_ENTRY;
}


/**
 * \brief Setup the scan sensor for the requested dpi and color mode
 *
 * A scan sensor can support a variety of resolutions and modes (color, mono).
 * Based on these values, the sensor may require different settings
 * (e.g. the sensor needs a signal asserted to indicate 600 dpi).
 *
 * \param[in] cmode  the requested color mode
 *
 * \param[in] dpi  the requested dpi
 *
 * \retval scan_err_t any scan error that occured
 *  SCANERR_NONE - dpi was found, and scan sensor set up
 *  SCANERR_INVALID_PARAM - requested dpi is not supported
 *
 * \remark - an even number of edges is required by the ASIC
 **/
scan_err_t scansen_setup(scan_cmode_t cmode, uint32_t dpi, uint32_t scanx)
{
    dbg2("%s: cmode=%d dpi=%d\n", __FUNCTION__, cmode, dpi);
    scansen_cmode = cmode;
    scansen_dpi = dpi;

    return SCANERR_NONE;
}


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
 **/
void scansen_set_leds(uint32_t sensor_num,
                      scan_cmode_t cmode, 
                      uint32_t pixels_per_line, 
                      uint32_t gateR,
                      uint32_t gateG,
                      uint32_t gateB)
{
    unsigned int pos;
    unsigned int rng;
    scan_led_timing_t led_timing;
    int i;
    uint8_t           mono_led_method = 2;
	unsigned int pos_r, pos_g, pos_b;    

    dbg2("%s s#=%d ppr=%d r=%d g=%d b=%d\n", __FUNCTION__, sensor_num, pixels_per_line, gateR, gateG, gateB);
    if (cmode != scansen_cmode)
    {
        dbg1("%s: scansen_set_leds error, cmode=%d, scansen_cmode=%d\n", __FUNCTION__, 
                                                                         cmode,
                                                                         scansen_cmode);  
        XASSERT(0, cmode);
    }

    for (i=0; ; i++) 
    {
        if (scansen_led_timing_table[i].dpi == -1) 
        {
            dbg1("%s: invalid/unknown DPI %ld\n", __FUNCTION__, scansen_dpi);
            XASSERT(0, scansen_dpi);
        }
        else if ((scansen_led_timing_table[i].cmode == cmode) && (scansen_led_timing_table[i].dpi == scansen_dpi)) 
        {
            memcpy(&led_timing, &scansen_led_timing_table[i], sizeof(scan_led_timing_t));
            dbg1("%s: cmode=%d dpi=%d edgeR=%d edgeF=%d\n", __FUNCTION__,
                                                            led_timing.cmode,
                                                            led_timing.dpi,
                                                            led_timing.edgeR,
                                                            led_timing.edgeF);
            break;
        }
    }

    pos = led_timing.edgeR; 
    if (cmode == SCAN_CMODE_COLOR)
    {
        rng = led_timing.edgeF;
        if (gateR == 0) gateR = 1;
        if (gateG == 0) gateG = 1;
        if (gateB == 0) gateB = 1;        
        scif_ledR_gate(sensor_num, pos, pos + (unsigned)(rng*gateR/100)); //gateR% of range
        scif_ledG_gate(sensor_num, pos, pos + (unsigned)(rng*gateG/100)); //gateG% of range
        scif_ledB_gate(sensor_num, pos, pos + (unsigned)(rng*gateB/100)); //gateB% of range
    } 
    else
    {
//        rng = led_timing.edgeF / 3;
        rng = led_timing.edgeF;
        if (gateR == 0) gateR = 1;
        if (gateG == 0) gateG = 1;
        if (gateB == 0) gateB = 1;

        if (mono_led_method == 1)
        {
            //method 1
            //LEDs were lightened at the same time - not supported on pinetree board
            scif_ledR_gate(sensor_num, pos, pos + (unsigned)(rng*gateR/100)); //gateR% of range
            scif_ledG_gate(sensor_num, pos, pos + (unsigned)(rng*gateG/100)); //gateG% of range
            scif_ledB_gate(sensor_num, pos, pos + (unsigned)(rng*gateB/100)); //gateB% of range
        }
        else
        {
            //method 2
            //LEDs were lightened by one by
			pos_r = (23*rng)/100;
			pos_g = (67*rng)/100;
			pos_b = (10*rng)/100;
	
			dbg2("%s s#=%d ppr=%d r=%d g=%d b=%d pos = %d rng = %d\n", __FUNCTION__, sensor_num, pixels_per_line, pos_r, pos_g, pos_b, pos, rng);
			scif_ledR_gate(sensor_num, pos, pos + (unsigned)(pos_r*gateR/100)); pos += pos_r + 1; //gateR% of range
            scif_ledG_gate(sensor_num, pos, pos + (unsigned)(pos_g*gateG/100)); pos += pos_g + 1; //gateG% of range
            scif_ledB_gate(sensor_num, pos, pos + (unsigned)(pos_b*gateB/100));                   //gateB% of range        		
        }
    }
}

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
void scansen_led_enable( uint32_t sensor_num, bool enable_red, bool enable_green, bool enable_blue )
{
    scif_led_enable( sensor_num, enable_red, enable_green, enable_blue );
}


/**
 * \brief Send the LED percentile to the hardware
 *
 * Each LED (RGB) can be individually set for strength.  Range is 0-100%
 * This can be used to set power, pwm duty cycle, voltage, current, etc.
 *
 * \param[in] dutyR  strength for the red LED
 * \param[in] dutyG  strength for the green LED
 * \param[in] dutyB  strength for the blue LED
 *
 * davep 16-Oct-2012 ; Removing floats. The gate value is an integer in [0,100]
 *                     for 0,100%.  Note there is no decimal precision (like
 *                     with the analog gain).
 */
void scansen_set_led_percentile( uint32_t sensor_num, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB)
{
    uint16_t led_pwm_period = 128; // nice round value, works out nicely in h/w
    
    dbg2( "%s s#=%d %d %d %d\n", __FUNCTION__, sensor_num, dutyR, dutyG, dutyB );
    XASSERT( dutyR>0 && dutyR<=100, dutyR );
    XASSERT( dutyG>0 && dutyG<=100, dutyG );
    XASSERT( dutyB>0 && dutyB<=100, dutyB );

    if (scansen_cmode == SCAN_CMODE_COLOR)
    {
        scif_led_duty(sensor_num, led_pwm_period, dutyR, dutyG, dutyB);
    }
    else
    {
        //scif_led_duty(1*128, dutyR/3, dutyG/3, dutyB/3);
        scif_led_duty(sensor_num, led_pwm_period, dutyR, dutyG, dutyB);
    }
}


/**
 * \brief Send the analog offsets for the 3 colors to the AFE
  * The values passed in are platform independent, and can be negative
 * 0 means no offset
 * 1 means set whatever AFE value is necessary to create a change of 1
 *    on the output
 * etc....
 */
void scansen_set_analog_offset(uint32_t sensor_num, int red_offset, int green_offset, int blue_offset)
{
    struct   scan_afe_info_t *afe_info;
    int      cugs, analog_offset_zero, analog_offset_max, analog_offset_min;
    int      i;
    int      pixel_offset[3];
    int16_t  dac_offset[3];

    afe_get_afe_info(&afe_info);
    cugs = afe_info->dac_unit_gain_output_change_step;
    analog_offset_zero = afe_info->cal_analog_offset_zero;
    analog_offset_max = afe_info->cal_analog_offset_max;
    analog_offset_min = afe_info->cal_analog_offset_min;

//    dbg1("%s: red_offset=%d green_offset=%d blue_offset=%d\n", __FUNCTION__,
//                                                               red_offset,
//                                                               green_offset,
//                                                               blue_offset);

    pixel_offset[0] = red_offset;
    pixel_offset[1] = green_offset;
    pixel_offset[2] = blue_offset;

    for ( i=0 ; i<3 ; i++) 
    {
        dac_offset[i] = pixel_offset[i] / cugs + analog_offset_zero;
        // clip numbers smaller than the min, or larger than the max
        if (dac_offset[i] < analog_offset_min)
            dac_offset[i] = analog_offset_min;
        
        if (dac_offset[i] >= analog_offset_max)
            dac_offset[i] = analog_offset_max;

    }

    dbg2("%s: s#=%d,Roffset=0x%X Goffset=0x%X Boffset=0x%X\n", __FUNCTION__,
                                                         sensor_num,
                                                         dac_offset[0],
                                                         dac_offset[1],
                                                         dac_offset[2]);

    afe_write_dac(sensor_num,dac_offset[0], dac_offset[1], dac_offset[2]);
}


/**
 * \brief Send the analog gain for the 3 colors to the AFE
 */
void scansen_set_analog_gain(uint32_t sensor_num, uint32_t red_gain, uint32_t green_gain, uint32_t blue_gain)
{
    uint8_t pga_red_gain, pga_green_gain, pga_blue_gain;

//    dbg1("%s: red_gain=%d green_gain=%d blue_gain=%d\n", __FUNCTION__,
//                                                         red_gain,
//                                                         green_gain,
//                                                         blue_gain);

    pga_red_gain = afe_gain_to_pga(red_gain);
    pga_green_gain = afe_gain_to_pga(green_gain);
    pga_blue_gain = afe_gain_to_pga(blue_gain);    

    dbg2("%s: s#=%d Rgain=%d Ggain=%d Bgain=%d\n", __FUNCTION__, sensor_num,
                                             pga_red_gain,
                                             pga_green_gain,
                                             pga_blue_gain);

    afe_write_pga(sensor_num, pga_red_gain, pga_green_gain, pga_blue_gain);
}

scan_err_t scansen_run( void )
{
    return SCANERR_NONE;
}


scan_err_t scansen_stop( void )
{
    scansen_led_enable(0, false, false, false);
    scansen_led_enable(1, false, false, false);    
    return SCANERR_NONE;
}


scan_err_t scansen_pause( void )
{
    /* NO OP */
    return SCANERR_NONE;
}


scan_err_t scansen_resume( void )
{
    /* NO OP */
    return SCANERR_NONE;
}

static void scansentype1_get_sensor_conf( scan_cmode_t cmode, uint32_t dpi, SENSOR_CONF *sensor)
{
    int i;

    dbg2("%s cmode=%d, dpi=%d\n", __FUNCTION__, cmode, dpi);
    /* clear a warning */
    i = 0;

    // Please note, SSEL can not be set during init_sensor - The appropriate signal for a sensor,
    // should be set during scansen_setup(), when the DPI is known

    sensor->is_staggered = false;        // always false for CIS
    sensor->positive_going_video = true; //true  if analog black signal < white (positive going video)
                                         //false if analog black signal > white (negative going video)

    /*
     * NOTE: the edges in this routine are tuned assuming:
     *   - Scan clock of 264 (Z3 dithered)
     *   - Sensor freq of 4.889
     *   Which gives us a pixel period of 54 scan clocks.
     */
    sensor->sensor_freq_khz = SENSOR_CLOCK_FREQ_KHZ;

    //Currently used during calibration only.  Units are in millivolts
    sensor->peak_millivolts_300 = 1100;  //highest voltage the sensor outputs @ 300dpi
    sensor->peak_millivolts_600 = 1100;  //highest voltage the sensor outputs @ 600dpi
    sensor->peak_millivolts_1200 = 1100; //highest voltage the sensor outputs @ 1200dpi

    sensor->STYPE = SEN_CIS; //SEN_CIS = using a CIS sensor, SEN_CCD = using CCD sensor

    sensor->AFEWIDTH = SCIF_SCFG2_AFEWIDTH_8; //MRVL-Harris for external AFE
//    sensor->AFEWIDTH = SCIF_SCFG2_AFEWIDTH_4; //MRVL-Harris for external AFE


    /* SCFG1 */
    // set all clock enables to false
    memset(&sensor->SCFG1, 0, sizeof(struct scan_config1_register));

    // now set the clock enables we use
    sensor->SCFG1.p1clken = true;  //enable CISCLK to the CIS
    sensor->SCFG1.p2clken = true; 
    sensor->SCFG1.mclken  = true;  //enable MCLK to the AFE
    sensor->SCFG1.vsclken = true;  //enable VSMP to the AFE

    //MRVL-Harris for external AFE start
    //SP setting
    sensor->SCFG1.sppol = false;
    sensor->SCFG1.scpol = false;
    sensor->SPGEN  = SPGEN_PIXEL_CLOCK;

#if 1 //Lite on sensor
    if (dpi <= 300) 
    {
        // dpi selected by length of SP for liteon CIS   
        sensor->sensor_sp_edges[0] = START_PULSE_RISING_EDGE; 
        sensor->sensor_sp_edges[1] = 4;
    }
    else
    { 	
        sensor->sensor_sp_edges[0] = START_PULSE_RISING_EDGE; 
        sensor->sensor_sp_edges[1] = 6;
    }
    
    // ID pin pull-up
    sensor->ccd_clamp_mode = 0;
    sensor->SCFG1.scpol = true;
    sensor->sensor_clamp_edges[0] = 0xFF;
    sensor->sensor_clamp_edges[1] = 0xFF;
    
#else // Canon sensor
    sensor->sensor_sp_edges[0] = 1; 
    sensor->sensor_sp_edges[1] = 2; 
    if (dpi <= 300) 
    {
        //300 DPI
        sensor->ccd_clamp_mode = 1;
        sensor->SCFG1.scpol = false;
      #ifdef HAVE_DBG_CALLBACK_CMDS
        if (scansen_clamp_debug_enable == true)
        {
            sensor->sensor_clamp_edges[0] = scansen_clamp_edges[0];
            sensor->sensor_clamp_edges[1] = scansen_clamp_edges[1];
        }
        else
      #endif
        {
            sensor->sensor_clamp_edges[0] = 0;
            sensor->sensor_clamp_edges[1] = 57;
          #ifdef HAVE_DBG_CALLBACK_CMDS
            scansen_clamp_edges[0] = sensor->sensor_clamp_edges[0];
            scansen_clamp_edges[1] = sensor->sensor_clamp_edges[1];
          #endif
        }
    }
    else if (dpi <= 600) 
    {
        //600 DPI
        sensor->ccd_clamp_mode = 0;
        sensor->SCFG1.scpol = false;
        sensor->sensor_clamp_edges[0] = 0xFF;
        sensor->sensor_clamp_edges[1] = 0xFF;
    } 
    else 
    {
        //1200 DPI 
        sensor->ccd_clamp_mode = 0;
        sensor->SCFG1.scpol = true;
        sensor->sensor_clamp_edges[0] = 0xFF;
        sensor->sensor_clamp_edges[1] = 0xFF;
    }
#endif
/* davep 05-Jul-2012 ; adjust clocks for Granite (250Mhz) from Gemstone
 * (264Mhz) 
 */

    //P1CLK setting
  #ifdef HAVE_DBG_CALLBACK_CMDS
    if (scansen_p1clk_debug_enable == true)
    {
        sensor->sensor_p1clk_edges[0] = scansen_p1clk_edges[0];
        sensor->sensor_p1clk_edges[1] = scansen_p1clk_edges[1];
    }
    else
  #endif
    {
#if PINETREE_L
//        sensor->sensor_p1clk_edges[0] = 8;
//        sensor->sensor_p1clk_edges[1] = 40;

        sensor->sensor_p1clk_edges[0] = 6; //final
        sensor->sensor_p1clk_edges[1] = 38;

//        sensor->sensor_p1clk_edges[0] = 12; //final
//        sensor->sensor_p1clk_edges[1] = 44;

#else
        sensor->sensor_p1clk_edges[0] = 19;
        sensor->sensor_p1clk_edges[1] = 47;
#endif
      #ifdef HAVE_DBG_CALLBACK_CMDS
        scansen_p1clk_edges[0] = sensor->sensor_p1clk_edges[0];
        scansen_p1clk_edges[1] = sensor->sensor_p1clk_edges[1];
      #endif
    }

    //MCLK setting
#if PINETREE_L
//    sensor->sensor_p2clk_edges[0] = 1;
//    sensor->sensor_p2clk_edges[1] = 11;
//    sensor->sensor_p2clk_edges[2] = 21;
//    sensor->sensor_p2clk_edges[3] = 31,
//    sensor->sensor_p2clk_edges[4] = 41;
//    sensor->sensor_p2clk_edges[5] = 51;
    
    sensor->sensor_p2clk_edges[0] = 3;
    sensor->sensor_p2clk_edges[1] = 13;
    sensor->sensor_p2clk_edges[2] = 23;
    sensor->sensor_p2clk_edges[3] = 33,
    sensor->sensor_p2clk_edges[4] = 43;
    sensor->sensor_p2clk_edges[5] = 53;    
#else
    sensor->sensor_p2clk_edges[0] = 6;
    sensor->sensor_p2clk_edges[1] = 16;
    sensor->sensor_p2clk_edges[2] = 26;
    sensor->sensor_p2clk_edges[3] = 36,
    sensor->sensor_p2clk_edges[4] = 46;
    sensor->sensor_p2clk_edges[5] = 56;
#endif
    
    sensor->SCFG1.acpol = 0x08; // polarity for mclk - select FB first

    // Turn on the mux for dual scan.
    // We need to mux between the outputs of each AFE.  Each AFE is
    // outputting data between each of p2clk's edges, so the mux needs
    // to read that data 2x p2clk's speed to read the output while the
    // data is still there for the same pixel on both AFEs.
    // Note that Shiloh's board uses our mclk signal for the mux signal
    // (which is why we use p2clk for the AFE mclk)
    sensor->afe_mclk_edges[ 0] = 1;
    sensor->afe_mclk_edges[ 1] = 5;
    sensor->afe_mclk_edges[ 2] = 11;
    sensor->afe_mclk_edges[ 3] = 15;
    sensor->afe_mclk_edges[ 4] = 21;
    sensor->afe_mclk_edges[ 5] = 25;
    sensor->afe_mclk_edges[ 6] = 31;
    sensor->afe_mclk_edges[ 7] = 35;
    sensor->afe_mclk_edges[ 8] = 41;
    sensor->afe_mclk_edges[ 9] = 45;
    sensor->afe_mclk_edges[10] = 51;
    sensor->afe_mclk_edges[11] = 55;
    //VSMP setting
  #ifdef HAVE_DBG_CALLBACK_CMDS
    if (scansen_vsclk_debug_enable == true)
    {
        sensor->afe_vsclk_edges[0] = scansen_vsclk_edges[0];
        sensor->afe_vsclk_edges[1] = scansen_vsclk_edges[1];
    }
    else
  #endif
    {
//test.kmg
#if PINETREE_L
//        sensor->afe_vsclk_edges[0] = 38; 
//        sensor->afe_vsclk_edges[1] = 56;

        sensor->afe_vsclk_edges[0] = 40; 
        sensor->afe_vsclk_edges[1] = 58;
#else
        sensor->afe_vsclk_edges[0] =  2; 
        sensor->afe_vsclk_edges[1] = 20;
#endif

        
      #ifdef HAVE_DBG_CALLBACK_CMDS
        scansen_vsclk_edges[0] = sensor->afe_vsclk_edges[0];
        scansen_vsclk_edges[1] = sensor->afe_vsclk_edges[1];
      #endif
    }

    //CLCLK setting =) disable, using SHA mode
    sensor->SCFG1.clclken = false; 
    sensor->afe_cd1clk_edges[0] = 23;  //don't care, using SHA mode
    sensor->afe_cd1clk_edges[1] = 38;  //don't care, using SHA mode

    //LDATA setting
  #ifdef HAVE_DBG_CALLBACK_CMDS
    if (scansen_ldata_debug_enable == true)
    {
        sensor->afe_ldata[ 0].clock =  scansen_ldata_edges[ 0];
        sensor->afe_ldata_a2[ 0].clock =  scansen_ldata_edges_a2[ 0];        
        sensor->afe_ldata[ 1].clock =  scansen_ldata_edges[ 1];
        sensor->afe_ldata_a2[ 1].clock =  scansen_ldata_edges_a2[ 1];        
        sensor->afe_ldata[ 2].clock =  scansen_ldata_edges[ 2];
        sensor->afe_ldata_a2[ 2].clock =  scansen_ldata_edges_a2[ 2];        
        sensor->afe_ldata[ 3].clock =  scansen_ldata_edges[ 3];
        sensor->afe_ldata_a2[ 3].clock =  scansen_ldata_edges_a2[ 3];        
        sensor->afe_ldata[ 4].clock =  scansen_ldata_edges[ 4];
        sensor->afe_ldata_a2[ 4].clock =  scansen_ldata_edges_a2[ 4];        
        sensor->afe_ldata[ 5].clock =  scansen_ldata_edges[ 5];
        sensor->afe_ldata_a2[ 5].clock =  scansen_ldata_edges_a2[ 5];        
    }
    else
  #endif
    {
        sensor->afe_ldata[ 0].clock = 6;
        sensor->afe_ldata[ 1].clock = 16;
        sensor->afe_ldata[ 2].clock = 26;
        sensor->afe_ldata[ 3].clock = 36;
        sensor->afe_ldata[ 4].clock = 46;
        sensor->afe_ldata[ 5].clock = 56;
        sensor->afe_ldata_a2[ 0].clock = 0;
        sensor->afe_ldata_a2[ 1].clock = 10;
        sensor->afe_ldata_a2[ 2].clock = 20;
        sensor->afe_ldata_a2[ 3].clock = 30;
        sensor->afe_ldata_a2[ 4].clock = 40;
        sensor->afe_ldata_a2[ 5].clock = 50;
      #ifdef HAVE_DBG_CALLBACK_CMDS
        scansen_ldata_edges[ 0] = sensor->afe_ldata[ 0].clock;
        scansen_ldata_edges_a2[ 0] = sensor->afe_ldata_a2[ 0].clock;        
        scansen_ldata_edges[ 1] = sensor->afe_ldata[ 1].clock;
        scansen_ldata_edges_a2[ 1] = sensor->afe_ldata_a2[ 1].clock;        
        scansen_ldata_edges[ 2] = sensor->afe_ldata[ 2].clock;
        scansen_ldata_edges_a2[ 2] = sensor->afe_ldata_a2[ 2].clock;        
        scansen_ldata_edges[ 3] = sensor->afe_ldata[ 3].clock;
        scansen_ldata_edges_a2[ 3] = sensor->afe_ldata_a2[ 3].clock;        
        scansen_ldata_edges[ 4] = sensor->afe_ldata[ 4].clock;
        scansen_ldata_edges_a2[ 4] = sensor->afe_ldata_a2[ 4].clock;        
        scansen_ldata_edges[ 5] = sensor->afe_ldata[ 5].clock;
        scansen_ldata_edges_a2[ 5] = sensor->afe_ldata_a2[ 5].clock;        
      #endif
    }  

    sensor->afe_ldata[ 0].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL;
    sensor->afe_ldata[ 1].code =  SCIF_LDATA_CODE_AB_BITS;
    sensor->afe_ldata[ 2].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL;
    sensor->afe_ldata[ 3].code =  SCIF_LDATA_CODE_AB_BITS;
    sensor->afe_ldata[ 4].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL;
    sensor->afe_ldata[ 5].code =  SCIF_LDATA_CODE_AB_BITS | SCIF_LDATA_CODE_SEQ_COMPL;
    sensor->afe_ldata_a2[ 0].code =  SCIF_LDATA_CODE_AB_BITS;    
    sensor->afe_ldata_a2[ 1].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL;    
    sensor->afe_ldata_a2[ 2].code =  SCIF_LDATA_CODE_AB_BITS;    
    sensor->afe_ldata_a2[ 3].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL;    
    sensor->afe_ldata_a2[ 4].code =  SCIF_LDATA_CODE_AB_BITS;    
    sensor->afe_ldata_a2[ 5].code =  SCIF_LDATA_CODE_CD_BITS | SCIF_LDATA_CODE_LOAD_COMPL | SCIF_LDATA_CODE_SEQ_COMPL;

    //PSEQ setting
    /* davep 25-Mar-2013 ; I created new fct scif_pseq() to make building PSEQ
     * easier. Testing with Harris' code.
     */
    if (cmode == SCAN_CMODE_COLOR)
    {
//        sensor->afe_pixel_seq[0] = 0x00 + 0x08 + 0x10;
//        sensor->afe_pixel_seq[1] = 0x01 + 0x08 + 0x10;
//        sensor->afe_pixel_seq[2] = 0x02 + 0x08 + 0x10 + 0x20;
        sensor->afe_pixel_seq[0] = scif_pseq(0,1,1,0);
        sensor->afe_pixel_seq[1] = scif_pseq(0,1,1,1);
        sensor->afe_pixel_seq[2] = scif_pseq(1,1,1,2);
    }
    else
    {
//        sensor->afe_pixel_seq[0] = 0x03 + 0x08 + 0x10;
//        sensor->afe_pixel_seq[1] = 0x03 + 0x08 + 0x10;
//        sensor->afe_pixel_seq[2] = 0x03 + 0x08 + 0x10 + 0x20;
        sensor->afe_pixel_seq[0] = scif_pseq(0,1,1,3);
        sensor->afe_pixel_seq[1] = scif_pseq(0,1,1,3);
        sensor->afe_pixel_seq[2] = scif_pseq(1,1,1,3);
    }
    //MRVL-Harris for external AFE end
} 


static void scansentype2_get_sensor_conf( scan_cmode_t cmode, uint32_t dpi, SENSOR_CONF *sensor)
{
    int i;

    /* clear a warning */
    i = 0;

    // Please note, SSEL can not be set during init_sensor - The appropriate signal for a sensor,
    // should be set during scansen_setup(), when the DPI is known

    sensor->is_staggered = false; // always false for CIS
    sensor->positive_going_video = false;// true if analog black signal < white (positive going video)
                                         // false if analog black signal > white (negative going video)

    /*
     * NOTE: the edges in this routine are tuned assuming:
     *   - Scan clock of 264 (Z3 dithered)
     *   - Sensor freq of 4
     *   Which gives us a pixel period of 66 scan clocks.
     */
    sensor->sensor_freq_khz = SENSOR_CLOCK_FREQ_KHZ;

    //Currently used during calibration only.  Units are in millivolts
    sensor->peak_millivolts_300 = 1100; // highest voltage the sensor outputs @ 300dpi
    sensor->peak_millivolts_600 = 1100; // highest voltage the sensor outputs @ 600dpi
    sensor->peak_millivolts_1200 = 1100; // highest voltage the sensor outputs @ 1200dpi

    sensor->STYPE = SEN_CIS; // SEN_CIS = using a CIS sensor, SEN_CCD = using CCD sensor

    sensor->AFEWIDTH = SCIF_SCFG2_AFEWIDTH_16; // Marvell's internal AFE has a 16 bit wide bus

    /* SCFG1 set the clock enables we use */
    sensor->SCFG1.p1clken = true; // enable main pixel clock
    sensor->SCFG1.mclken = true; // enable MCLK to the AFE
    sensor->SCFG1.vsclken = true;  // enable VSMP to the AFE

    // Polarity used, if needed, when tuning clock edges
    // see scanif.h for details (scan_config1_register).
    // Normally, all clocks are lo at pixel time 0.
    // if acpol is set to 0x8, then MClk will be high
    // at pixel time 0.
    sensor->SCFG1.sppol = true;

    /* SCFG3 : not using */

    // SPGEN - determines the units to use for the SP
    // here we use the system clock.
    //  SPGEN_SYSTEM_CLOCK = Scan system clock units
    //  SPGEN_PIXEL_CLOCK  = Pixel clock units (Pixel period)
    sensor->SPGEN  = SPGEN_PIXEL_CLOCK;    // Clock edges are in pixel clock units

    /* sensor_ssm_edges : not using */

    /* Sensor Start Pulse; (transfer SP signal to the CIS); */
    sensor->sensor_sp_edges[0] = 0x7; // assert start pulse on this pixel clock after start of line

    /* davep 12-Jul-2011 ; resolution select uses sensor SP width */
    switch( dpi ) {
        case 300 : 
            sensor->sensor_sp_edges[1] = sensor->sensor_sp_edges[0] + 10; /* 300 */
            break;
        case 600 : 
            sensor->sensor_sp_edges[1] = sensor->sensor_sp_edges[0] + 15; /* 600  */
            break;
        case 1200 : 
            sensor->sensor_sp_edges[1] = sensor->sensor_sp_edges[0] + 20; /* 1200 */
            break;
        case 2400 : 
            sensor->sensor_sp_edges[1] = sensor->sensor_sp_edges[0] + 25; /* 2400 */
            break;
        default:
            /* wtf? */
            XASSERT(0,dpi);
            break;
    }

    /* Sensor P1Clk edges; 0xff is disabled (scan bus clock units) */
    // The pixel period is set by return value of get_default_sensor_freq()
    // but here we set the rising and falling edges (scan bus clock units).
    // example: if pixel time is 58, 0x0 and 0x1d gives ~50% duty cycle.
    // The rising edge of p1clk is at 0, and falling edge is at 0x1d
    /* davep 12-Jul-2011 ; RogerS */
    sensor->sensor_p1clk_edges[0] = 3; 
    sensor->sensor_p1clk_edges[1] = 32;

    /* The internal AFE requires a nice periodic MCLK over the entire pixel period,
     * it can't vary by more than 5%. So you can jiggle the clocks a little bit
     * to get better sample points or fit an uneven pixel period, but don't go nuts.
     * Note:
     *   OSR4: 4 complete mclk cycles, needs 8 edges
     *   OSR8: 8 complete mclk cycles, needs 16 edges
     *
     * Unused edges MUST be set to 0xff (see the memset above)!
     */
    /* Note: pixel period (66/16=4.125) leaves things a bit strange for OSR8.  Doing
     * two things to the mclk:
     *  - alternate delta between edges from 4 and 5 to smooth things out where 
     *    possible
     *  - group some of the 4's and 5's to get better RSMP placement
     */ 
    /* davep 12-Jul-2011 ; RogerS */
    sensor->afe_mclk_edges[0]  = 0;
    sensor->afe_mclk_edges[1]  = 4; 
    sensor->afe_mclk_edges[2]  = 8;   // <--- VSMP Falling Edge (polarity fliped)
    sensor->afe_mclk_edges[3]  = 14;
    sensor->afe_mclk_edges[4]  = 18;
    sensor->afe_mclk_edges[5]  = 22;
    sensor->afe_mclk_edges[6]  = 26;  // <+++ RMSP Rising Edge
    sensor->afe_mclk_edges[7]  = 30;
    sensor->afe_mclk_edges[8]  = 34;
    sensor->afe_mclk_edges[9]  = 38;
    sensor->afe_mclk_edges[10] = 42;  // <--- RSMP Falling Edge
    sensor->afe_mclk_edges[11] = 46;
    sensor->afe_mclk_edges[12] = 50;
    sensor->afe_mclk_edges[13] = 54;
    sensor->afe_mclk_edges[14] = 58;  // <+++ VSMP Rising Edge
    sensor->afe_mclk_edges[15] = 62;

    /* NOTE: going to flip over the vsclk clock polarity 
     * AFE clock polarities (4 bits)
     *    1000=MClk 0100=VSClk 0010=CD1Clk 0001=LCClk 
     *
     * FIXME! magic number needs to be in asic specific header file.
     */
    sensor->SCFG1.acpol = 0x4;

    /* Video sample point.  The internal AFE requires the VSMP edges to occur
     * only on rising edges of MCLK. The VSMP pulse width should be one complete
     * MCLK period.
     *
     * Unused edges MUST be set to 0xff (see the memset above)!
     */
    sensor->afe_vsclk_edges[0] = 10; 
    sensor->afe_vsclk_edges[1] = 60;

    /* Enable CDS */
    sensor->SCFG1.clclken = true; 

    /* CDS needs a reference level sample point. The internal AFE requires the RSMP
     * edges to occur only on rising edges of MCLK. The RSMP pulse width should be
     * one complete MCLK period.
     *
     * Unused edges MUST be set to 0xff (see the memset above)!
     */
    /* davep 12-Jul-2011 ; RogerS */
    sensor->afe_cd1clk_edges[0] = 28;
    sensor->afe_cd1clk_edges[1] = 44;

    /* rest disabled */

    // .clock is in scan bus clock units - identifies the clock on which to sample the 
    // data coming out of the AFE.  Internal AFE has 16 bits, so ldata[0].clock
    // identifies the sample for the entire pixel
    // .code identifies bit order - ABCD is all 4 nibbles at once

    /* AFE Load Data clocks; LDATA1-LDATA6; 0xff=>disabled */
    // internal AFE - 16 bits - 1 load
    sensor->afe_ldata[0].clock = 56;
    sensor->afe_ldata[0].code =  SCIF_LDATA_CODE_ABCD_BITS |
                                 SCIF_LDATA_CODE_LOAD_COMPL |
                                 SCIF_LDATA_CODE_SEQ_COMPL;

    /* pixel sequence */
    /* Scan block defines 111_011 for PSEQ1 for Color CIS */
    // this is the only value ever used for Color CIS
    sensor->afe_pixel_seq[0] = 0x3b; /* 6-bit field: 111_011 */
    /* rest are disabled (zero) */
} 


/**
 * \brief  SENSOR_CONF 'constructor'
 *
 * Initializes a SENSOR_CONF structure with clean defaults, mostly disabled.
 *
 * Subsequent setup code should initialize just the fields necessary.
 *
 * Created this function in an attempt to reduce the copy/paste code among the
 * plethora of supported sensors.
 *
 * \author David Poole
 * \date 18-Jul-2011
 *
 */
static void sensor_conf_constructor( SENSOR_CONF *sensor)
{
    int i;

    /* Put sensor config struct in a known state */
    memset( sensor, 0, sizeof(SENSOR_CONF) );

    // Please note, SSEL can not be set during init_sensor - The appropriate signal for a sensor,
    // should be set during scansen_setup(), when the DPI is known

    sensor->is_staggered = false; // always false for CIS

    sensor->STYPE = SEN_CIS; // SEN_CIS = using a CIS sensor, SEN_CCD = using CCD sensor

    /* SCFG1 */
    // set all clock enables to false
    memset( &sensor->SCFG1, 0, sizeof(struct scan_config1_register) );

    /* SCFG3 - delay clocks by up to 1 clock period. Not normally used. see scanif.h
       for details. */
    /* leave all zero (default) if you don't use */
    memset( &sensor->SCFG3, 0, sizeof(struct scan_config3_register) );

    // SPGEN - determines the units to use for the SP
    // here we use the system clock.
    //  SPGEN_SYSTEM_CLOCK = Scan system clock units
    //  SPGEN_PIXEL_CLOCK  = Pixel clock units (Pixel period)
    sensor->SPGEN  = SPGEN_PIXEL_CLOCK;    // Clock edges are in pixel clock units

    /* Sensor start pulse signal mask edges; 0xff is disabled */
    /* Used for masking off certain clocks after the start pulse */
    /* The clocks that can be masked are set in the ssmen field (see scanif.h), and are
       P1Clk, P2Clk, RSClk, SPClk */
    /* ssm_edges[0] is the time from the beginning of the line until masking starts
       and ssm_edges[1] is the time from the beginning of the line until masking stops */
    sensor->sensor_ssm_edges[0] = 0xff; // no masking
    sensor->sensor_ssm_edges[1] = 0xff; // no masking

    /* Sensor Start Pulse; (transfer SP signal to the CIS); */
    sensor->sensor_sp_edges[0] = 0xff; 
    sensor->sensor_sp_edges[1] = 0xff;
    
    /* Sensor APH1 edges; STCFG3,STCFG4; 0xff is disabled */
    /* this is only used for additional Phase 1 edges for P1 clk */
    memset( sensor->sensor_aph1clk_edges, 0xff, 
            sizeof(sensor->sensor_aph1clk_edges) );
    /* disabled */

    /* Sensor Clamp Config; CCFG; 0xff edges are disabled */
    memset( sensor->sensor_clamp_edges, 0xff,
            sizeof(sensor->sensor_clamp_edges) );
    sensor->ccd_clamp_mode = 0; /* default */
    sensor->afe_clamp_mode = 0; /* default */

    /* Sensor P1Clk edges; 0xff is disabled (scan bus clock units) */
    memset( sensor->sensor_p1clk_edges, 0xff, 
            sizeof(sensor->sensor_p1clk_edges) );

    /* Disable all edges */

    /* Sensor P2Clk edges - for sensors that need a 2nd phase pixel clock; 0xff is disabled
     (scan bus clock units) */
    memset( sensor->sensor_p2clk_edges, 0xff, 
            sizeof(sensor->sensor_p2clk_edges) );

    /* Sensor RSClk edges; 0xff is disabled (scan bus clock units) */
    memset( sensor->sensor_rsclk_edges, 0xff, 
            sizeof(sensor->sensor_rsclk_edges) );

    /* Sensor CPClk edges; 0xff is disabled (scan bus clock units) */
    memset( sensor->sensor_cpclk_edges, 0xff, 
            sizeof(sensor->sensor_cpclk_edges) );

    /* Reset AFE MCLK edges; 0xff is disabled */
    memset( sensor->afe_mclk_edges, 0xff, sizeof(sensor->afe_mclk_edges) );

    /* Reset AFE VSCLK (vsmp) edges; 0xff is disabled */
    memset( sensor->afe_vsclk_edges, 0xff, sizeof(sensor->afe_vsclk_edges) );

    /* Reset AFE CD1CLK edges; (scan bus clock units) 0xff is disabled */
    memset( sensor->afe_cd1clk_edges, 0xff, sizeof(sensor->afe_cd1clk_edges) );

    /* AFE Load Data clocks; LDATA1-LDATA6; 0xff=>disabled */
    memset( sensor->afe_ldata, 0xff, sizeof(sensor->afe_ldata) );
    memset( sensor->afe_ldata_a2, 0xff, sizeof(sensor->afe_ldata) );    

    /* AFE ldata disabled */
    for( i=0 ; i<12 ; i++ ) {
        sensor->afe_ldata[i].clock = 0xff;
        sensor->afe_ldata_a2[i].clock = 0xff;        
        sensor->afe_ldata[i].code = 0;
        sensor->afe_ldata_a2[i].code = 0;        
    }

    /* pixel sequence */
    memset( sensor->afe_pixel_seq, 0, sizeof(sensor->afe_pixel_seq) );

    /* davep 03-Jul-2012 ; add Granite CFGARB */
    sensor->cbi_enable_parallel = true;
} 

uint32_t scansen_get_num_sensors(void)
{
    return 2;
}

// currently sensor_num is ignored - future-proofing
void scansen_get_sensor_conf(uint32_t sensor_num, scan_cmode_t cmode, uint32_t dpi, SENSOR_CONF *sensor)
{
    int sensor_id;

    /* davep 18-Jul-2011 ; initialize to nice clean defaults */
    sensor_conf_constructor( sensor );

    /* davep 18-Jul-2011 ; choose sensor config at run-ish time */
    sensor_id = scansen_get_sensor_id();
    if ( sensor_id==SCAN_SENSOR_TYPE1 ) {
        scansentype1_get_sensor_conf( cmode, dpi, sensor );
    }
    else {
        XASSERT( sensor_id==SCAN_SENSOR_TYPE2, sensor_id );
        scansentype2_get_sensor_conf( cmode, dpi, sensor );
    }
}


/**
 * \brief  
 *
 * \author David Poole
 * \date 15-Aug-2011
 *
 * TODO need to push latest cisx.h to branch for this function to work 
 */
scan_err_t scansen_get_cisx_conf( uint32_t dpi, struct cisx_sensor_conf *cisxsensor )
{
    /* davep 27-Jul-2011 ; added this function */
    int i;
    
    if(cis_select_state == 1) //by JJW (for new CIS)
	{ //new CIS
		for (i=0; ; i++) 
		{
			if (scansen_cisx_conf_table_new[i].dpi == -1) 
			{
				dbg1("%s: invalid/unknown DPI %ld\n", __FUNCTION__, dpi);
				XASSERT(0, dpi);
			}
			else if (scansen_cisx_conf_table_new[i].dpi == dpi) 
			{
				memcpy(cisxsensor, &scansen_cisx_conf_table_new[i].cisx_conf, sizeof(struct cisx_sensor_conf));
				/* davep 25-Mar-2013 ; hardwire everyone to cbiout_pace=10 (from
				 * DougK)
				 */
				cisxsensor->cbiout_pace = 10;
				break;
			}
		}
		
	}
	else
	{ //old CIS
	for (i=0; ; i++) 
		{
			if (scansen_cisx_conf_table_old[i].dpi == -1) 
			{
				dbg1("%s: invalid/unknown DPI %ld\n", __FUNCTION__, dpi);
				XASSERT(0, dpi);
			}
			else if (scansen_cisx_conf_table_old[i].dpi == dpi) 
			{
				memcpy(cisxsensor, &scansen_cisx_conf_table_old[i].cisx_conf, sizeof(struct cisx_sensor_conf));

				/* davep 25-Mar-2013 ; hardwire everyone to cbiout_pace=10 (from
				 * DougK)
				 */
				cisxsensor->cbiout_pace = 10;

				break;
			}
		}
		
	}
	
    return SCANERR_NONE;
}


/**
 * \brief  Calculate the scanx necessary for this sensor.
 *
 *  The scan block needs to know the number of pixels in the sensor.
 *
 * \author David Poole
 * \date 25-Aug-2011
 *
 */
scan_err_t scansen_calc_scanx( scan_cmode_t cmode, uint32_t dpi, uint32_t *scanx )
{
    scan_scanx_conf_t scanx_conf;
    int i;

	if(cis_select_state == 1) //by jjw (for new CIS)
	{ //new CIS
		//dbg1("[JJW] new CIS - ");
		for (i=0; ; i++) 
		{
			if (scansen_scanx_table_new[i].dpi == -1) 
			{
				dbg1("%s: invalid/unknown DPI %ld\n", __FUNCTION__, dpi);
				XASSERT(0, dpi);
			}
			else if ((scansen_scanx_table_new[i].cmode == cmode) && (scansen_scanx_table_new[i].dpi == dpi)) 
			{
				memcpy(&scanx_conf, &scansen_scanx_table_new[i], sizeof(scan_scanx_conf_t));
				dbg2("%s: cmode=%d dpi=%d scanx=%d\n", __FUNCTION__,
													   scanx_conf.cmode,
													   scanx_conf.dpi,
													   scanx_conf.scanx);
				*scanx = scanx_conf.scanx;
				break;
			}
		}
	}
	else
	{ //old CIS
		//dbg1("[JJW] old CIS - ");
		for (i=0; ; i++) 
		{
			if (scansen_scanx_table_old[i].dpi == -1) 
			{
				dbg1("%s: invalid/unknown DPI %ld\n", __FUNCTION__, dpi);
				XASSERT(0, dpi);
			}
			else if ((scansen_scanx_table_old[i].cmode == cmode) && (scansen_scanx_table_old[i].dpi == dpi)) 
			{
				memcpy(&scanx_conf, &scansen_scanx_table_old[i], sizeof(scan_scanx_conf_t));
				dbg2("%s: cmode=%d dpi=%d scanx=%d\n", __FUNCTION__,
													   scanx_conf.cmode,
													   scanx_conf.dpi,
													   scanx_conf.scanx);
				*scanx = scanx_conf.scanx;
				break;
			}
		}
	
	}

    return SCANERR_NONE;
}
