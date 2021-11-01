/**
 * \file scanimg.c
 *
 * \brief ScanImg "class".
 *
 * scanimg == Scan Imaging Layer.  A Singleton Facade [1] object sitting above
 * the sensor, AFE, and LEDs.
 *
 * Why an object? We have a lot of settings dependent on other parts of the
 * system. For example, the LED exposure depends on the sensor width and the
 * Start Pulse width. Want to keep that knowledge encapsulated.
 *
 * Also, want to be able to put different sensor/afe combos into the hardware
 * but keep the layer between them as clean to the rest of the code as possible.
 * Any horrendous upheaval in sensor/afe hardware should be kept to scansen.c
 * and afe.c (respectively). 
 *
 * David Poole
 * Feb/Mar 2008
 *
 * [1] Gamma, Erich; Richard Helm, Ralph Johnson, and John Vlissides (1995).
 * Design Patterns: Elements of Reusable Object-Oriented Software.
 * Addison-Wesley. ISBN 0-201-63361-2. 
 */
/* 
 * ============================================================================
 * Copyright (c) 2007, 2008 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "interrupt_api.h"
#include "intnums.h"
#include "regAddrs.h"
#include "hwconfig_api.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanhw.h"
#include "scanif.h"
#include "scan.h"
#include "scanimg.h"
#include "scansen.h"
#include "afe.h"
#include "scancmdq.h"
#include "safeint.h"

#define DEF_LED_EXP      100  /* The default exposure values for all LEDs */
#define DEF_LED_PWM      100  /* The default LED PWM  */

/* define to turn on more verbose debugging */
//#define SCANIMG_DEBUG 

#ifdef SCANIMG_DEBUG
#define scanimg_dbg2 dbg2 
#else
#define scanimg_dbg2(...) 
#endif

/* davep 18-Feb-2008 ; we've always had horrible ordering problems with setting
 * up a scan.  Later functions would peek into the Scan block registers to find
 * certain information. If a particular function hadn't been called yet, that
 * part of the scan block would be uninitialized. But there was no way to tell
 * uninitialized vs initialized with the scan block registers because several of
 * the fields are perfectly fine at their uninitialized defaults (e.g.,
 * SCFG2.CMODE).
 *
 * So the scan code below is a layer above the Scan block and maintains the
 * steps necessary to set up a scan using the Scan block. This layer will
 * maintain "is_null" states for certain variables so we'll know if we try to
 * get a value before it's been set. 
 *
 * Think of it as a record in a database. That record can have several fields
 * whose values are "IS NULL".  
 *
 * As a specific example, before turning on Internal ScanLine Mode, we need to
 * have set scanx and cmode. In order to set the LED gates, also need to have
 * scanx, cmode, and the start pulse width.
 *
 * This structure will only contain information about the sensor/afe that other
 * parts of the code might need, the per-scan stuff that is order dependent. 
 *
 */

struct scanimg_members {

    /* Each public member has a getter/setter function.  The setter function
     * will be "private" (declared 'static' in C terms) and is responsible for
     * setting
     *
     * Private members have no externally available getter function.
     *
     * getters will assert if the field is uninitialized.
     *
     * Note: the "setter" functions are named "save" instead of "set" because I
     * didn't want any confusion that the setter function was actually writing
     * to hardware.
     */

    /* 
     * Public members (getter available outside this source file)
     */
    scan_cmode_t cmode;
    bool cmode_not_null;

    uint32_t scanx;
    bool scanx_not_null;

    /* pixel period; number of system bus clock ticks in a pixel */
    uint32_t pixper;
    bool pixper_not_null;

    uint32_t dpi;
    bool dpi_not_null;

    uint32_t sensor_bitmask;
    bool sensor_bitmask_not_null;
    
    /* 
     * Private members 
     */

    /* The scanimg (scan imaging) layer is open for business  */
    bool is_open;

    /* led enable/disable */
    bool led_enable_red, led_enable_green, led_enable_blue;

    /* led pwm */
    uint32_t led_pwm_red, led_pwm_green, led_pwm_blue;

    /* led exposures as percentiles */
    uint32_t led_exp_red, led_exp_green, led_exp_blue;

    /* analog offset */
    int afe_red_offset, afe_green_offset, afe_blue_offset; 

    /* analog gain */
    uint32_t afe_red_gain, afe_green_gain, afe_blue_gain;
};

/* The data members of the ScanImg class.
 *
 * Cleared in scanimg_open() (the "constructor") and filled as we set up the
 * scan.
 *
 * I'd have named this structure "self" or "this" but that probably have
 * collided with a keyword eventually. :-)
 */

static struct scanimg_members scanimg_self[SCANSEN_MAX_SENSORS];

static void
current_init( void ) 
{
    int i;
    /* Sort of like a C++ Constructor.
     *
     * note: always go through this constructor so we can alloc any 
     * necessary memory (if at some point we do have malloc'd memory in
     * scanimg_members) and set any constants; 
     */
    memset( &scanimg_self, 0, sizeof(scanimg_self));

    /* default to 100% illumination */
    for (i=0;i<SCANSEN_MAX_SENSORS;i++)
    {
        scanimg_self[i].led_enable_red = scanimg_self[i].led_enable_green =
            scanimg_self[i].led_enable_blue = true;
        scanimg_self[i].led_pwm_red = scanimg_self[i].led_pwm_green =
            scanimg_self[i].led_pwm_blue = DEF_LED_PWM;
        scanimg_self[i].led_exp_red = scanimg_self[i].led_exp_green =
            scanimg_self[i].led_exp_blue = DEF_LED_EXP;
        
        /* default to zero offset, no gain */
        scanimg_self[i].afe_red_offset = 0;
        scanimg_self[i].afe_green_offset = 0;
        scanimg_self[i].afe_blue_offset = 0;
        scanimg_self[i].afe_red_gain = 100; /* 1.0; */
        scanimg_self[i].afe_green_gain = 100; /* 1.0; */
        scanimg_self[i].afe_blue_gain = 100;  /* 1.0; */
    }
}

static void
current_delete( void )
{
    /* Sort of like a C++ Destructor.
     *
     * note: always go through this destructor so we can free any malloc'd
     * memory (if at some point we do have malloc'd memory in scanimg_members)
     */

    memset( &scanimg_self, 0, sizeof(scanimg_self));
}

static void
current_save_cmode( scan_cmode_t cmode )
{
    int i;
    // cmode is shared across all sensors
    for (i=0;i<SCANSEN_MAX_SENSORS;i++)
    {
        scanimg_self[i].cmode = cmode;
        scanimg_self[i].cmode_not_null = true;
    }
}

static void
current_save_scanx( uint32_t scanx )
{
    int i;
    // scanx is shared across all sensors
    for (i=0;i<SCANSEN_MAX_SENSORS;i++)
    {
        scanimg_self[i].scanx = scanx;
        scanimg_self[i].scanx_not_null = true;
    }
}

static void
current_save_pixper( uint32_t pixper )
{
    int i;
    // pixper is shared across all sensors
    for (i=0;i<SCANSEN_MAX_SENSORS;i++)
    {
        scanimg_self[i].pixper = pixper;
        scanimg_self[i].pixper_not_null = true;
    }
}

static void
current_save_dpi( uint32_t dpi )
{
    int i;
    // dpi is shared across all sensors
    for (i=0;i<SCANSEN_MAX_SENSORS;i++)
    {
        scanimg_self[i].dpi = dpi;
        scanimg_self[i].dpi_not_null = true;
    }
}

void
scanimg_get_cmode( scan_cmode_t *cmode )
{
    // since it is shared across all sensors, grab any
    ASSERT( scanimg_self[0].cmode_not_null );
    *cmode = scanimg_self[0].cmode;
}

void
scanimg_get_scanx( uint32_t *scanx ) 
{
    // since it is shared across all sensors, grab any
    ASSERT( scanimg_self[0].scanx_not_null );
    *scanx = scanimg_self[0].scanx;
}

/**
 * \brief  Set the scanx, the scan block pixels per row.
 *
 * Normally scanimg will calculate scanx on his own.  However, there are some
 * cases where higher level code needs to adjust scanx for padding reasons.
 *
 * \author David Poole
 * \date 21-Jan-2010
 *
 */

void scanimg_set_scanx( uint32_t new_scanx ) 
{
    /* set our hardware to the new value */
    scif_set_scanx( new_scanx );

    /* remember the value */
    current_save_scanx( new_scanx );
}

void
scanimg_get_pixper( uint32_t *pixper )
{
    // since it is shared across all sensors, grab any
    ASSERT( scanimg_self[0].pixper_not_null );
    *pixper = scanimg_self[0].pixper;
}

void
scanimg_get_dpi( uint32_t *dpi )
{
    // since it is shared across all sensors, grab any
    ASSERT( scanimg_self[0].dpi_not_null );
    *dpi = scanimg_self[0].dpi;
}


static void
current_save_sensor_bitmask( uint32_t sensor_bitmask )
{
    int i;
    // sensor_bitmask is shared across all sensors
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        scanimg_self[i].sensor_bitmask = sensor_bitmask;
        scanimg_self[i].sensor_bitmask_not_null = true;
    }
}

void
scanimg_set_sensor_bitmask( uint32_t sensor_bitmask )
{
    current_save_sensor_bitmask( sensor_bitmask );
}

void
scanimg_get_sensor_bitmask( uint32_t *sensor_bitmask )
{
    // since it is shared across all sensors, grab any
    ASSERT( scanimg_self[0].sensor_bitmask_not_null );
    *sensor_bitmask = scanimg_self[0].sensor_bitmask;
}

/**
 * \brief  Set the platform independent analog offset.
 *
 * Input range of the offsets should be in [-65536,65535] (signed 17-bit
 * number). The funny range is a 16-bit pixel offset distance.
 *
 * The scansen layer will convert the platform independent offset into the
 * appropriate platform dependent value.
 *
 * \author David Poole
 * \date Mar-2008
 *
 */

void scanimg_set_analog_offset( uint32_t sensor_num, int red_offset, int green_offset, int blue_offset )
{
    ASSERT( scanimg_self[sensor_num].is_open );

    scanimg_dbg2( "%s red=%d green=%d blue=%d\n", __FUNCTION__, 
                red_offset, green_offset, blue_offset );

    scanimg_self[sensor_num].afe_red_offset = red_offset;
    scanimg_self[sensor_num].afe_green_offset = green_offset;
    scanimg_self[sensor_num].afe_blue_offset = blue_offset;
}

void scanimg_get_analog_offset( uint32_t sensor_num, int *p_red_offset, int *p_green_offset, int *p_blue_offset )
{
    ASSERT( scanimg_self[sensor_num].is_open );

    *p_red_offset = scanimg_self[sensor_num].afe_red_offset;
    *p_green_offset = scanimg_self[sensor_num].afe_green_offset;
    *p_blue_offset = scanimg_self[sensor_num].afe_blue_offset;
}

/**
 * \brief  Set the platform independent analog gain.
 *
 *  Input value will be something like desired_pixel_value/actual_pixel_value.
 *
 *  The scansen layer will convert the platform independent gain into the
 *  appropriate platform dependent value.
 *
 * \author David Poole
 * \date Mar-2008
 *
 * davep 31-May-2012 ; removing floats for Linux kernel and breaking current
 *                     API. Caller must send in float*100 to represent the gain.
 *
 */

void scanimg_set_analog_gain( uint32_t sensor_num, uint32_t red_gain, uint32_t green_gain, uint32_t blue_gain )
{
    ASSERT( scanimg_self[sensor_num].is_open );

    scanimg_dbg2("%s sensor=%d red=%d green=%d blue=%d\n", __FUNCTION__, 
          sensor_num, red_gain, green_gain, blue_gain);
    
    /* davep 31-May-2012 ; removing floats so try to catch old code. The Gain
     * should be (0,1000) for 0.0 to 10.00  (gain of 10 seemed a safe sane max)
     */
    XASSERT( red_gain>0 && red_gain<=1000, red_gain );
    XASSERT( green_gain>0 && green_gain<=1000, green_gain );
    XASSERT( blue_gain>0 && blue_gain<=1000, blue_gain );

    scanimg_self[sensor_num].afe_red_gain = red_gain;
    scanimg_self[sensor_num].afe_green_gain = green_gain;
    scanimg_self[sensor_num].afe_blue_gain = blue_gain;
}

void scanimg_get_analog_gain( uint32_t sensor_num, uint32_t *p_red_gain, uint32_t *p_green_gain, uint32_t *p_blue_gain )
{
    ASSERT( scanimg_self[sensor_num].is_open );

    *p_red_gain = scanimg_self[sensor_num].afe_red_gain;
    *p_green_gain = scanimg_self[sensor_num].afe_green_gain;
    *p_blue_gain = scanimg_self[sensor_num].afe_blue_gain;
}

/**
 * \brief  power up initialization of scan imaging subsystem
 *
 *
 *
 * \author David Poole
 * \date 26-Mar-2008
 *
 */

scan_err_t scanimg_onetime_init( void )
{
    scan_err_t scerr;

    /* set up the scan block so the AFE can talk to it */
    scif_reset();

    scerr = afe_onetime_init();
    if( scerr != 0 ) {
        return scerr;
    }

    scerr = scansen_onetime_init();

    return scerr;
}

/**
 * \brief  ScanImg class "constructor"
 *
 * ScanImg is a singleton wrapping itself around the sensor, AFE, and
 * illumination.
 *
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

scan_err_t scanimg_open( scan_cmode_t cmode, uint32_t dpi, uint32_t sensor_bitmask )
{
    SENSOR_CONF sc;
    uint32_t pixper;
    scan_err_t scerr;
    int i;

    scanimg_dbg2( "%s\n", __FUNCTION__ );

    /* stupid human check */
    XASSERT(sensor_bitmask != 0, sensor_bitmask);
    for (i=0;i<SCANSEN_MAX_SENSORS;i++) {
        ASSERT( scanimg_self[i].is_open==false );
    }

    /* start our scan run with a clean slate */
    current_init();

    /* set up the static parts of the sensor/afe, the stuff that is the same for
     * each and every scan
     */
    scif_reset();

    memset( &sc, 0, sizeof(SENSOR_CONF) );
    // for the future, we need to call scansen_get_sensor_conf for each sensor, and set up
    // each of them - for now, just grab the configuration for the 1st sensor
    scansen_get_sensor_conf( 0, cmode, dpi, &sc );
    scif_sensor_setup( &sc );

    current_save_dpi( dpi );
    current_save_cmode( cmode );
    current_save_sensor_bitmask( sensor_bitmask );

    scerr = afe_reset();
    if( scerr != 0 ) {
        dbg2( "%s afe_reset failed err=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    /* davep 27-Sep-2010 ; catch drivers that haven't been updated to the new
     * SENSOR_CONF structure
     */
    ASSERT( sc.sensor_freq_khz != 0 );

    /* Pixel period can change depending on our scan so it needs its own
     * interface function.
     *
     * (For example, the original stepper motor control is intricately tied into
     * the scan block. So in order to drive the stepper motor at different
     * speeds, we had to adjust the pixel period.)
     */
    pixper = scif_calc_pixel_period( sc.sensor_freq_khz );

    scif_set_pixel_period( pixper );
    current_save_pixper( pixper );

    /* we're now open for business */
    for (i=0;i<scansen_get_num_sensors();i++)
        scanimg_self[i].is_open = true;

    /* success! */
    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

scan_err_t scanimg_setup( void )
{
    uint32_t scanx;
    scan_err_t scerr;
    scan_cmode_t cmode;
    uint32_t dpi;
    int i;

    scanimg_get_cmode( &cmode );
    scanimg_get_dpi( &dpi );

    scanimg_dbg2( "%s cmode=%d dpi=%d\n", __FUNCTION__, cmode, dpi );

    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
        ASSERT( scanimg_self[i].is_open );

    scif_set_cmode( cmode );

    /* davep 24-Aug-2011 ; move the scanx (total pixels per row) calculation
     * down into the scansen layer. We use this number to set the scan block's
     * SCANX register, the number of pixels per line the scan block will clock
     * out.
     */
    scerr = scansen_calc_scanx( cmode, dpi, &scanx );
    XASSERT( scerr==SCANERR_NONE, scerr );

    scanimg_dbg2( "%s scanx=%d\n", __FUNCTION__, scanx );

    /* tell the sensor and AFE what we're doing */
    /* davep 13-Jul-2010 ; adding scanx as in/out param */
    scerr = scansen_setup( cmode, dpi, scanx );
    if( scerr != 0 ) {
        dbg1( "%s scansen_setup failed scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    scerr = afe_soft_setup();
    if( scerr != 0 ) {
        /* davep 24-Feb-2008 ; cannot assert on this afe setup call because it
         * can fail if we're having a hardware problem. I only assert on
         * firmware problems.
         */
        dbg1( "%s afe setup failed! scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    /* save the settings and mark them initialized */
    current_save_cmode( cmode );
    current_save_dpi( dpi );

    /* pushes into scanblock, saves a local copy */
    scanimg_set_scanx( scanx );

    /* XXX for debugging */
//    scif_dump();

    return 0;
}

/**
 * \brief  set LED exposure percentiles
 *
 * Doesn't write the LED values into hardware. Stores until the scan is up and
 * running.
 *
 * \author David Poole
 * \date 12-Mar-2008
 *
 */

void scanimg_set_led_exposure(uint32_t sensor_num, uint32_t expR, uint32_t expG, uint32_t expB )
{
    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    scanimg_dbg2( "%s num=%d r=%d g=%d b=%d\n", __FUNCTION__, sensor_num, expR, expG, expB );

    /* davep 16-Oct-2012 ; exposure functions are strictly [0,100] with no
     * percentile
     */
    XASSERT( expR>=0 && expR<=100, expR );
    XASSERT( expG>=0 && expG<=100, expG );
    XASSERT( expB>=0 && expB<=100, expB );

    scanimg_self[sensor_num].led_exp_red = expR;
    scanimg_self[sensor_num].led_exp_green = expG;
    scanimg_self[sensor_num].led_exp_blue = expB;
}

void scanimg_get_led_exposure( uint32_t sensor_num, uint32_t *p_expR, uint32_t *p_expG, uint32_t *p_expB )
{
    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    *p_expR = scanimg_self[sensor_num].led_exp_red;
    *p_expG = scanimg_self[sensor_num].led_exp_green;
    *p_expB = scanimg_self[sensor_num].led_exp_blue;
}

void scanimg_set_led_enable( uint32_t sensor_num, bool red, bool green, bool blue )
{  
    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    scanimg_dbg2( "%s num=%d r=%d g=%d b=%d\n", __FUNCTION__, sensor_num, red, green, blue );

    scanimg_self[sensor_num].led_enable_red = red;
    scanimg_self[sensor_num].led_enable_green = green;
    scanimg_self[sensor_num].led_enable_blue = blue;
}

void scanimg_get_led_enable( uint32_t sensor_num, bool *p_red, bool *p_green, bool *p_blue )
{
    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    *p_red = scanimg_self[sensor_num].led_enable_red;
    *p_green = scanimg_self[sensor_num].led_enable_green;
    *p_blue = scanimg_self[sensor_num].led_enable_blue;
}

void scanimg_set_led_duty( uint32_t sensor_num, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB)
{
    scanimg_dbg2( "%s num=%d r=%d g=%d b=%d\n", __FUNCTION__, sensor_num, dutyR, dutyG, dutyB ); 

    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    /* davep 16-Oct-2012 ; duty functions are strictly [0,100] with no
     * percentile
     */
    XASSERT( dutyR>0 && dutyR<=100, dutyR );
    XASSERT( dutyG>0 && dutyG<=100, dutyG );
    XASSERT( dutyB>0 && dutyB<=100, dutyB );

    scanimg_self[sensor_num].led_pwm_red = dutyR;
    scanimg_self[sensor_num].led_pwm_green = dutyG;
    scanimg_self[sensor_num].led_pwm_blue = dutyB;
}

void scanimg_get_led_duty( uint32_t sensor_num, uint32_t *p_dutyR, uint32_t *p_dutyG, uint32_t *p_dutyB)
{
    /* stupid human check */
    ASSERT( scanimg_self[sensor_num].is_open );

    *p_dutyR = scanimg_self[sensor_num].led_pwm_red;
    *p_dutyG = scanimg_self[sensor_num].led_pwm_green;
    *p_dutyB = scanimg_self[sensor_num].led_pwm_blue;
}

/**
 * \brief Align scanx to a boundary
 *
 * Created so we can tweak scanx to match PIC WDMA requirements when we don't
 * want to use PIC margins.
 *
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

scan_err_t scanimg_align_scanx( uint32_t *p_scanx, uint32_t align_to, scanx_align_t align_how )
{
    uint32_t scanx, new_scanx;

    scanx = *p_scanx;

    /* if not pad, then truncate */
    if( align_how==SCANX_ALIGN_PAD ) {
        /* round up to match alignment value */
        new_scanx = ((scanx + (align_to-1)) / align_to) * align_to;
        dbg2( "%s pad scanx from 0x%x to 0x%x\n", 
                    __FUNCTION__, scanx, new_scanx );
    }
    else {
        /* truncate down to match alignment value */
        new_scanx = (scanx / align_to) * align_to;
        dbg2( "%s truncate scanx from 0x%x to 0x%x\n", 
                    __FUNCTION__, scanx, new_scanx );
    }

    /* sanity check */
    XASSERT( new_scanx%align_to==0, new_scanx );

    *p_scanx = new_scanx;

    /* success! */
    return 0;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 12-Mar-2008
 *
 */

static void
finalize_led_setup( void )
{
    scan_cmode_t cmode;
    uint32_t scanx;
    int i;

    scanimg_get_scanx( &scanx );
    scanimg_get_cmode( &cmode );

    scanimg_dbg2( "%s cmode=%d scanx=%d\n", __FUNCTION__, cmode, scanx );

    // All of the values we care about have been set in the scanimg_self array.
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        scansen_led_enable(i, scanimg_self[i].led_enable_red,
                           scanimg_self[i].led_enable_green,
                           scanimg_self[i].led_enable_blue );

        scansen_set_leds(i, cmode, scanx, 
                         scanimg_self[i].led_exp_red, 
                         scanimg_self[i].led_exp_green, 
                         scanimg_self[i].led_exp_blue );

        scansen_set_led_percentile(i, scanimg_self[i].led_pwm_red,
                                   scanimg_self[i].led_pwm_green,
                                   scanimg_self[i].led_pwm_blue );
        /* check our back doors */
        if( scan_get_config() & SCAN_CONFIG_NOLEDS ) {
            scansen_led_enable(i,false, false, false);
        }
    }
}
/**
 * \brief  Write the analog offset/gain settings into the AFE.
 *
 * Originally we wrote the offset and gain directly into the AFE inside the
 * scanimg_set_xxx() functions. However, I needed to expose the ability to
 * get/set the offset and gain before we started the scan.
 *
 * We wanted the ability to manually tweak the analog settings before we scan.
 *
 * \author David Poole
 * \date 08-Jan-2009
 *
 */

static void finalize_afe_setup( void )
{
    int i;
    // All of the values we care about have been set in the scanimg_self array.
    // so it is safe to write all
    // of the scanimg_self array to the hardware.

    for (i=0;i<scansen_get_num_sensors();i++)
    {
        scansen_set_analog_offset(i,scanimg_self[i].afe_red_offset, 
                                  scanimg_self[i].afe_green_offset, 
                                  scanimg_self[i].afe_blue_offset);

        dbg2( "%s offset=%d %d %d gains %d %d %d\n", __FUNCTION__, scanimg_self[i].afe_red_offset,
              scanimg_self[i].afe_green_offset, scanimg_self[i].afe_blue_offset,
              scanimg_self[i].afe_red_gain, 
              scanimg_self[i].afe_green_gain, 
              scanimg_self[i].afe_blue_gain);

        scansen_set_analog_gain(i,scanimg_self[i].afe_red_gain, 
                                scanimg_self[i].afe_green_gain, 
                                scanimg_self[i].afe_blue_gain);
    }
}

/**
 * \brief  Finalize any remaining hardware settings. Start up sensor and AFE. 
 *
 *  Some hardware settings are modifiable after ScanImg is instantiated.  For
 *  example, Cal needs to be able to tweak the AFE and LEDs before the scan
 *  actually starts. 
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

scan_err_t scanimg_run( void )
{
    int i;
    scan_err_t scerr;
//    scan_cmode_t cmode;

    scanimg_dbg2( "%s\n", __FUNCTION__ );

    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        ASSERT( scanimg_self[i].is_open );
    }
    /* now that we have all our goodies poked into our class, set up the final
     * bits and pieces that we need
     */
    finalize_led_setup();
    finalize_afe_setup();

    /* davep 12-Mar-2008 ; XXX for debugging */
//    scif_dump();
//    afe_dump();

    /* davep 21-Dec-2010 ; pulling plug on hardwired #ifdefs around specific
     * afe/sensor combos 
     */
    scerr = scansen_run();
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

//    /* If we are doing a color scan: the AFE will need to be switched to the
//     * appropriate gain/offset settings for each color scan line.
//     * NOTE:  We can't do the auto mode setup until all the afe reads/writes
//     *   are done, otherwise our settings get zapped!  So this code can't live
//     *   in scanimg_setup ...
//     */
//    scanimg_get_cmode( &cmode );
//    if( cmode==SCAN_CMODE_COLOR) {
//        afe_auto_setup();
//        afe_auto_enable( true );
//    }

    return 0;
}

/**
 * \brief  Shut down sensor and AFE.
 *
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

scan_err_t scanimg_stop( void )
{
    scan_err_t scerr;
    int i;
    
    scanimg_dbg2( "%s\n", __FUNCTION__ );

    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        if( !scanimg_self[i].is_open ) {
            /* gently ignore duplicate close */
            return SCANERR_NONE;
        }
    }

    /* davep 21-Dec-2010 ; pulling plug on hardwired #ifdefs around specific
     * afe/sensor combos 
     */
    scerr = scansen_stop();
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

//    /* Disable auto AFE mode (doesn't hurt when in mono scan mode) */
//    afe_auto_enable( false );

    /* davep 12-Mar-2008 ; XXX for debugging */
//    scif_dump();
//    afe_dump();

    return 0;
}

/**
 * \brief  ScanImg class "destructor"
 *
 *
 * \author David Poole
 * \date 18-Feb-2008
 *
 */

void scanimg_close( void )
{
    int i;
    
    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        if( !scanimg_self[i].is_open ) {
            /* gently ignore duplicate close */
            return;
        }
    }

    /* clean up our settings */
    current_delete();

    /* be explicit about being closed */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        scanimg_self[i].is_open = false;
    }
}

/**
 * \brief calculate sclkper from a desired line time in usec
 *
 * \param scan_line_time_usec ; desired scan line time in
 * microseconds
 *
 *
 * \author Theron Luttmann
 * \date 16-Jan-2009
 *
 */

uint32_t scanimg_calc_sclkper_from_line_time( uint32_t scan_line_time_usec )
{
    uint32_t scanx_in_usec;
    uint32_t scanx, pixper, sclkper;
    scan_cmode_t cmode;
    uint32_t dpi;

    dbg2( "%s scan_line_time=%d\n", __FUNCTION__, scan_line_time_usec );

    scanimg_get_scanx( &scanx );
    scanimg_get_pixper( &pixper );
    scanimg_get_cmode( &cmode );
    scanimg_get_dpi( &dpi );
    sclkper = scif_calc_sclkper( cmode, dpi, scanx );

    dbg2( "%s scanx=%d pixper=%d cmode=%d dpi=%d sclkper=%d\n", 
                __FUNCTION__, scanx, pixper, cmode, dpi, sclkper );

    /* (sclkper * scycle * pixper) / bus_freq_mhz = scan_line_period_usec
     *
     * Based on the Scan Block MA and emperical studies using the logic
     * analyzer.
     *
     * Adding 0.5 to round up
     */

#ifdef __KERNEL__
    /* TODO get rid of float */
    BUG();
    scanx_in_usec = 0;
#else
    scanx_in_usec = (uint32_t)(
                              ((float)( sclkper * SCAN_SCYCLE_DEFAULT * pixper ) / 
                               (float)hw_get_scan_clk_speed()
                              ) 
                              + 0.5 
                            );

    dbg2( "%s scanx_in_usec=%d\n", __FUNCTION__, scanx_in_usec );

    if( scan_line_time_usec < scanx_in_usec ) {
        dbg2( "%s scan line should be increased from %d to %d\n", 
                    __FUNCTION__, scan_line_time_usec, scanx_in_usec );
    }

    //now calculate the sclkper value for the desired line time
    sclkper = ((float)(hw_get_scan_clk_speed() * scan_line_time_usec)) / 
              ((float)( SCAN_SCYCLE_DEFAULT * pixper )); 
#endif

    return sclkper;
}

/**
 * \brief  
 *
 *
 * \author David Poole
 * \date 22-Jun-2009
 *
 */

uint32_t scanimg_calc_line_time_from_sclkper( uint32_t sclkper )
{
    uint32_t pixper;
    uint32_t scan_line_time_usec;

    scanimg_get_pixper( &pixper );

//    dbg2( "%s pixper=%d sclkper=%d scanclk_mhz=%d\n", 
//            __FUNCTION__, pixper, sclkper, hw_get_scan_clk_speed() );

    scan_line_time_usec = safeint_divide_with_round( 
                                    (sclkper * SCAN_SCYCLE_DEFAULT * pixper), 
                                    hw_get_scan_clk_speed() );
#if 0
    scan_line_time_usec = (uint32_t)( 
                                    ( (sclkper * SCAN_SCYCLE_DEFAULT * pixper) /
                                      (float)hw_get_scan_clk_speed()
                                    )
                                    + 0.5
                                  );
#endif

    return scan_line_time_usec;
}

scan_err_t scanimg_scanning_pause( void )
{
    scan_err_t scerr;
    int i;
    
    dbg2( "%s\n", __FUNCTION__ );

    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        ASSERT( scanimg_self[i].is_open );
    }

    scerr = scansen_pause();
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

    /* more? */

    return SCANERR_NONE;
}

scan_err_t scanimg_scanning_resume( void )
{
    scan_err_t scerr;
    int i;
    
    dbg2( "%s\n", __FUNCTION__ );

    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        ASSERT( scanimg_self[i].is_open );
    }

    scerr = scansen_resume();
    if( scerr!=SCANERR_NONE ) {
        return scerr;
    }

    /* more? */

    return SCANERR_NONE;
}

uint32_t scanimg_get_line_time_usec( void )
{
    uint32_t sclkper;
    int i;
    
    /* stupid human check */
    for (i=0;i<scansen_get_num_sensors();i++)
    {
        ASSERT( scanimg_self[i].is_open );
    }

    /* ballen 12/2011 We need to use this call (rather than scif_calc_sclkper)
     * because it factors in any additional buffer (currently 10%) and
     * any slowdown needed by the mech code */
    sclkper = scan_cmdq_get_sclkper();
    return scanimg_calc_line_time_from_sclkper( sclkper );
}
