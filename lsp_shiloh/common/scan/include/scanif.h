/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006-2008   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Interface - See HLD Document, Scan Interface
 *
 *****************************************************************************/

#ifndef SCANIF_H
#define SCANIF_H


/* * * * * * * * * * Motor * * * * * * * * * */
typedef enum {SYNC_FIRST, SYNC_EVERY, SYNC_DISABLE} MOTOR_SYNC; // Line Sync Mode
typedef enum {MOT_STEPPER,   //Stepper motor driver
              MOT_DC         //Dc motor
    } MOTOR_TYPE; // Motor Type
typedef enum {MOTOR1, MOTOR2} MOTOR_SEL; // Select which motor
                                          
void scif_motor_setup(MOTOR_SEL motor_data_enable, MOTOR_SYNC lsmode, 
                      MOTOR_TYPE m1type, MOTOR_TYPE m2type);
void scif_motor_lsmode(MOTOR_SYNC lsmode);
void scif_motor_data_enable(MOTOR_SEL motor_data_enable);



/* 
 * Scan Block's AFE control 
 */

typedef struct {
    enum {AFE_LOW, AFE_HIGH} SENINIT; // Serial Enable Initial Value
    enum {AFE_PULSE, AFE_LEVEL} SENCTRL; // Serial Enable Control
    enum {UNIDATA, AFE_BIDI, UNISDO} SERDCFG; // Serial Data Configuration
    unsigned SCLKDIV; // Serial Clock Divider
    // R   = read/write bit
    // A   = address bits, includes dummy bits at start
    // x   = dummy bits between address and data bits
    // D   = data bits
    // e.g. "RAAAxxxDDDDDDDDD" AD9822
    // e.g. "ARAAAADDDDDDDD" WM8152
    char *protocol;
} SCIF_AFE_CONF;

typedef struct {
    enum {BIN2, OHC3, BIN3} CCODE; // Color Code
    unsigned CLOC; // Color Location (MSB)
    unsigned reg; // register address
    unsigned data; // write data template
} SCIF_AFE_AUTO;

void scif_afe_setup(SCIF_AFE_CONF ac);
unsigned int scif_afe_read(unsigned int reg);
void scif_afe_write(unsigned int reg, unsigned int data);
void scif_afe_auto_setup(SCIF_AFE_AUTO aa);
void scif_afe_auto_enable(int on);

/* * * * * * * * * * Sensor * * * * * * * * * */

struct scan_config1_register {
    bool p1clken; /**< sensor phase 1 clocks enable */
    bool p2clken; /**< sensor phase 2 clocks enable */
    bool rsclken; /**< sensor reset clocks enable */
    bool cpclken; /**< sensor CP clocks enable */
    bool mclken;  /**< AFE master clock enable */
    bool vsclken; /**< AFE VS (video sample?) clock enable */
    bool clclken; /**< AFE clamp clock enable */
    bool lclpen;  /**< AFE line clamp clock enable */
    uint8_t leden;  /**< enable LEDR, LEDG, LEDB (3 bits) */
    bool bulben;  /**< enable bulb PBM generator on the LEDB pin */
    uint8_t ssmen;  /**< sensor clock start signal mask enable (4 bits)
                        1000=P1Clk 0100=P2Clk 0010=RSClk 0001=SPClk */
    uint8_t smpol;  /**< sensor clock start signal mask polarities (4 bits)
                        1000=P1Clk 0100=P2Clk 0010=RSClk 0001=SPClk */
    uint8_t p1apol; /**< P1Clk polarity; 0=initial low, asserts high; 1=initial high, asserts low */
    uint8_t sppol;  /**< start pulse polarity */
    uint8_t scpol;  /**< sensor clock polarities (4 bits)
                        1000=P1Clk 0100=P2Clk 0010=RSClk 0001=SPClk */
    uint8_t acpol;  /**< AFE clock polarities (4 bits)
                        1000=MClk 0100=VSClk 0010=CD1Clk 0001=LCClk */
};

struct scan_config3_register {
    /* Each register field is 2 bits:
     * 00: delay by 1/4 system clock period
     * 01: delay by 1/2 system clock period
     * 10: delay by 3/4 system clock period
     * 11: delay by 1 system clock period
     */
    uint8_t p1rise;
    uint8_t p1fall;
    uint8_t p2rise;
    uint8_t p2fall;
    uint8_t rsrise;
    uint8_t rsfall;
    uint8_t cprise;
    uint8_t cpfall;
    uint8_t mcrise;
    uint8_t mcfall;
    uint8_t vsrise;
    uint8_t vsfall;
    uint8_t cd1rise;
    uint8_t cd1fall;
    uint8_t sprise;
    uint8_t spfall;
};

/* we hardwire SCYCLE to this value */
#define SCAN_SCYCLE_DEFAULT  32

struct afe_ldata {
    unsigned int clock, code;
};

typedef struct {
    enum {SEN_CCD, SEN_CIS} STYPE;    // Sensor Type
    unsigned SSEL;       // Used to select DPI modes for sensor
    unsigned AFEWIDTH;   // AFE Data Width
    unsigned PIXPER;     // Pixel Period = PIXPER * Sys Clk Period

    unsigned int sensor_freq_khz;

    bool is_staggered;

    /* if positive going, analog signal black < white */
    bool positive_going_video;
    
    /* peak voltages in millivolts, per DPI */
    int peak_millivolts_300;
    int peak_millivolts_600;
    int peak_millivolts_1200;

    /* SCFG1,SCFG3 have a lot of fields so they're conglomerated together into
     * their own structures
     */
    struct scan_config1_register SCFG1;
    struct scan_config3_register SCFG3;

    /* Start Config Clocks are generated from the Pixel or System Clock; SPGEN
     * field of STCFG1 
     */
    enum SPGEN { SPGEN_PIXEL_CLOCK, SPGEN_SYSTEM_CLOCK } SPGEN; 
    uint32_t sensor_ssm_edges[2]; /* start pulse signal mask edges, STCFG1 */

    uint32_t sensor_sp_edges[2];     /* the legendary Start Pulse, STCFG2 */

    uint32_t sensor_aph1clk_edges[8];/* Additional phase 1, STCFG[3-4] */

    /* clamp config fields, CCFG */
    int ccd_clamp_mode;
    int afe_clamp_mode;
    uint32_t sensor_clamp_edges[3]; 
    
    uint32_t sensor_p1clk_edges[8];  /* Phase 1 clocks, SCLK[1-2] */
    uint32_t sensor_p2clk_edges[8];  /* Phase 2 clocks, SCLK[3-4] */
    uint32_t sensor_rsclk_edges[4];  /* Reset clocks, SCLK5 */
    uint32_t sensor_cpclk_edges[4];  /* CP clocks, SCLK6 */
    
    uint32_t afe_mclk_edges[16];      /* AFE ACLK[1-3], plus ACLK6 on some platforms */
    uint32_t afe_vsclk_edges[4];      /* AFE ACLK4 */
    uint32_t afe_cd1clk_edges[4];     /* AFE ACLK5 */
    struct afe_ldata afe_ldata[12]; /* AFE LDATA[1-6] registers' fields for AFE 1 */
    struct afe_ldata afe_ldata_a2[12]; /* AFE LDATA[1-6] registers' fields for AFE 2 */
    uint32_t afe_pixel_seq[6];        /* AFE PSEQ[1-2] registers' fields */

    /* CBI Bus Arbitration Config; new in Spring, 2011 */
    bool cbi_enable_external;
    bool cbi_enable_parallel;

} SENSOR_CONF;

// Number of system clocks per pixel (PIXPER)
unsigned scif_sys_clocks_pixel(void);

/* * * * * * * * * * Illumination * * * * * * * * * */

void scif_led_duty(uint32_t sensor_num, unsigned busClkDiv, uint32_t dutyR, uint32_t dutyG, uint32_t dutyB);
void scif_ledR_gate(uint32_t sensor_num, unsigned pos, unsigned neg);
void scif_ledG_gate(uint32_t sensor_num, unsigned pos, unsigned neg);
void scif_ledB_gate(uint32_t sensor_num, unsigned pos, unsigned neg);
void scif_led_enable(uint32_t sensor_num, int onR, int onG, int onB);
void scif_led_reset( void );

/* * * * * * * * * * Control & Status * * * * * * * * * */

void scif_reset(void);
void scif_control(int run, int interrupt);
void scif_clock(int enable);

uint32_t scif_command( bool sdata, unsigned int sclkper, uint16_t scany);

uint32_t scif_status(void);

void scif_set_clamp_clock(uint32_t ccd_clamp_mode, uint32_t afe_clamp_mode,
                          uint32_t clamp_mode_polarity, uint8_t *sensor_clamp_edges);

void scif_interrupt_wait(unsigned bits);
void scif_interrupt_ack(unsigned bits);
uint32_t scif_interrupt_disable( void );
void scif_interrupt_enable( void );

bool scif_command_executing( void );

void scif_dump(void);

void scif_set_cmode( int cmode );

/* davep 17-Feb-2008 ; new stuff for better API; cleanup of scan.c and
 * scanlib.c; make a real interface to the sensor/afe not just hackery
 */
void scif_reset(void);
void scif_sensor_setup( SENSOR_CONF *sc );
void scif_set_scanx( uint32_t scanx );
void scif_set_pixel_period( uint32_t pixel_period );
uint32_t scif_calc_pixel_period( uint32_t sensor_freq_khz );
void scif_send_simple_scmd( bool sdata, uint32_t sclkper, uint16_t num_lines );
void scif_set_ssel( uint8_t ssel );
uint32_t scif_calc_sclkper( scan_cmode_t cmode, uint32_t dpi, uint32_t pixeltimes_per_line );
void scif_set_internal_scanline_mode( scan_cmode_t, uint32_t dpi, uint32_t scanx, uint32_t *sclkper );

void scif_cbi_arbitration_config( bool enable_external, bool enable_parallel  );
void scif_cbi_enable_dual_channel( bool enable );
bool scif_get_dual_channel_enabled( void );

void scif_led_dump( void );

uint32_t scif_pseq( uint32_t S, uint32_t P, uint32_t V, uint32_t color );

#endif // SCANIF_H

