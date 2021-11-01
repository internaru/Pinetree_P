/****************************************************************************** 
 *
 * ============================================================================
 * Copyright (c) 2006   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description: Scan Interface - See HLD Document, Scan Interface
 * Author: Scott Lloyd
 *         Brad Smith
 *         David Poole
 *         Eric Huang
 *         Theron Luttman
 *
 *****************************************************************************/

// Things to remember (some lessons painfully learned - e.g. by smoking a motor):
//  PIC left/right margins need to be the same for each RGB channel or set to bypass.
//  SCANX needs to be div 2 for staggered color CCD data; data goes to 6 PIC channels.
//  SCANX must be a multiple of 16 for PIC DMA, or margins need to trim to multiple.
//  Don't set DataType bit in PSEQ after first RGB triplet on staggered CCD data.
//  Set CCD VBulb to 11V (Aruba JP20) and leave on, don't cycle on off.
//  Red is the first color in the CCD sensor to see the page.
//  One flatbed stepper motor full step is 1/300 of inch.
//  8 microsteps = 1 full step on flatbed.
//  MnHOLD in L6219 Control LATCHES the last VRef Motor PWM (may not be low current).
//  MnSCALE in L6219 Control scales 0-93.75% in 6.25% increments.
//  NDATA in AFE Control should not account for dummy bits.
//  SENINIT in AFE Control is the initial state of SLOAD before it is asserted.
//  SENINIT in AFE Control matches signal pin polarity 1=high, 0=low.
//  SSTAT_QEMPTY might glitch high for 1 clock at end of a scan command.
//  SSTAT_QEMPTY will be stable when SSTAT_SCMDCOMP is set.
//  afe_auto_* must be called after any afe_read/write functions.
//  The LED Gate edges appear to be one pixel clock behind in numbering
//  The LED Gate neg edge must be <= pixclks-2 or the next LED cycle is missed

#include <stdint.h>

#include "scos.h"

#include "regAddrs.h" 
#include "lassert.h"
#include "hwconfig_api.h"
#include "cpu_api.h"
#include "hwconfig_api.h"
#include "regAddrs.h"
#include "SCAN_regstructs.h"
#include "SCAN_regmasks.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "scanif.h"
#include "scanreset.h"
#include "scanhw.h"
#include "scan.h"
#include "scands.h"
#include "scanplat.h"
#include "safeint.h"

/* davep 12-Jul-2012 ; start using new regbuild structures 
 * TODO port all code to scif_regs
 */
static volatile SCAN_REGS_t * const scif_regs = (volatile SCAN_REGS_t*)(ICE_SCAN_BASE);

/* davep 13-Nov-2012 ; The interrupt flags are in funny places so need to do
 * some work to build a mask of them all. Some chips support certain features,
 * some don't.
 */
static const uint32_t sctrl_interrupt_flags =
                  /* everyone has this one */
                  SCAN_SCTRL_SCMDIE_MASK
#ifdef SCAN_SCTRL_UMSEN_MASK 
                | SCAN_SCTRL_UMSEN_MASK 
#endif
#ifdef SCAN_SCTRL_AFEPIE_MASK
                | SCAN_SCTRL_AFEPIE_MASK
#endif
                ;

/* * * * * * * * * * Sensor * * * * * * * * * */

unsigned int scif_sys_clocks_pixel(void)
{
    return((*SCIF_SCFG2 & SCIF_SCFG2_PIXPER(-1)) + 1);
}

/* * * * * * * * * * Control & Status * * * * * * * * * */

void scif_control(int run, int interrupt)
{
    if( run ) {
        scif_regs->SCTRL = SCAN_SCTRL_RUN_REPLACE_VAL(scif_regs->SCTRL,1);
    }
    else {
        scif_regs->SCTRL = SCAN_SCTRL_RUN_REPLACE_VAL(scif_regs->SCTRL,0);
    }
    if( interrupt ) {
        scif_regs->SCTRL = SCAN_SCTRL_SCMDIE_REPLACE_VAL(scif_regs->SCTRL,1);
    }
    else {
        scif_regs->SCTRL = SCAN_SCTRL_SCMDIE_REPLACE_VAL(scif_regs->SCTRL,0);
    }

//    *SCIF_SCTRL =
//        (*SCIF_SCTRL & ~(SCIF_SCTRL_SCMDIE(1)|SCIF_SCTRL_RUN(1))) |
//        SCIF_SCTRL_SCMDIE(interrupt) |
//        SCIF_SCTRL_RUN(run);
}

uint32_t scif_interrupt_disable( void )
{
    uint32_t was_enabled;

    was_enabled = (scif_regs->SCTRL & sctrl_interrupt_flags) && 1;

    /* turn them all off */
    scif_regs->SCTRL = scif_regs->SCTRL & ~sctrl_interrupt_flags;

    return was_enabled;
}

void scif_interrupt_enable( void )
{
    /* turn them all on */
    scif_regs->SCTRL = scif_regs->SCTRL | sctrl_interrupt_flags;
}

void scif_clock(int enable)
{
    *SCIF_SCTRL =
        (*SCIF_SCTRL & ~SCIF_SCTRL_SCANEN(1)) |
        SCIF_SCTRL_SCANEN(enable);
}

uint32_t scif_command( bool sdata, unsigned int sclkper, uint16_t scany)
{
    /* davep 17-Mar-2008 ; XXX temp debug */
    ASSERT( sclkper>0 );
    
    /* burtp 03-Aug-2009; Catch evil doers that would seek to roll us over */
    ASSERT( scany>0 );

    return(
        SCIF_SCMD_SDATA(sdata) |
        SCIF_SCMD_SCLKPER(sclkper-1) |
        SCIF_SCMD_SCANY(scany-1));
}

uint32_t scif_status(void)
{
    return *SCIF_SSTAT;
}

bool scif_command_executing(void)
{
    return (*SCIF_SSTAT & SCIF_SSTAT_CMDSTAT) && true;
}

void scif_interrupt_wait(unsigned int bits)
{
    while ((*SCIF_SSTAT & bits) == 0) ; // wait for condition
}

void scif_interrupt_ack(unsigned int sstat_bits)
{
    // acknowledge and clear status bits

    /* burtp 11-Oct-2010: Starting in Z1, some of the status and IRQ Ack bits don't 
     * line up, so we need to map each status bit to a specific ack bit.
     */ 
   *SCIF_SIACK = (
#ifdef SCIF_SIACK_UMSACK
        SCIF_SIACK_UMSACK( (sstat_bits & SCIF_SSTAT_UNEXPSYNC) && 1 ) |
#endif
        SCIF_SIACK_QOVRACK( (sstat_bits & SCIF_SSTAT_QOVR) && 1 )     |
        SCIF_SIACK_CIOVRACK( (sstat_bits & SCIF_SSTAT_CIOVR) && 1 )   |
#ifdef SCIF_SIACK_AFEPACK
        SCIF_SIACK_AFEPACK( (sstat_bits & SCIF_SSTAT_AFEPCOMP) && 1 ) |
#endif
        SCIF_SIACK_SCMDACK( (sstat_bits & SCIF_SSTAT_SCMDCOMP) && 1 ) );
}

/**
 * \brief Setup the scan block CMODE (Color Mode) bit
 *
 * SCFG2.CMODE==0 => monochrome scan
 * SCFG2.CMODE==1 => color scan
 *
 * \author David Poole
 * \date 26-Feb-2008
 *
 */

void
scif_set_cmode( int cmode )
{
    if( cmode==SCAN_CMODE_MONO ) {
        /* clear the cmode bit indicating mono mode */
        *SCIF_SCFG2 &= ~SCIF_SCFG2_CMODE(-1);
    }
    else {
        /* set the cmode bit indicating color mode */
        *SCIF_SCFG2 |= SCIF_SCFG2_CMODE(-1);
    }
}


/**
 * \brief Setup the scan block clamp config register (CCFG) register
 * and turn on the clamp clock enable in scan config 1 register
 *
 * Note, when you use the clamp clock to generate edges for scanner resolution,
 * set ccd_clamp_mode = 1 (line mode), and set sensor_clamp_edges[0] and [1] to
 * any edges you need for changing the clamp line.
 *
 * If youi don't want any edges for scanner resolution (you only want to have the
 * line asserted or deasserted), then set the ccd_camp_mode = 0 (pixel mode) which
 * seems to make the ASIC ignore the sensor_clamp_edges.  Then be sure to set
 * the clamp_mode_polarity = 0 for deassert, or = for assert.
 *
 * It looks odd, but we use CPCLK as the ccd clamp clock as per Doug Keithly
 *
 * \author Sandra Capri
 * \date 21-Apr-2008
 *
 */

void scif_set_clamp_clock(uint32_t ccd_clamp_mode, uint32_t afe_clamp_mode,
                          uint32_t clamp_mode_polarity, uint8_t *sensor_clamp_edges)
{
    uint32_t scfg1_val;

    *SCIF_CCFG =
        SCIF_CCFG_CPMODE(ccd_clamp_mode) |
        SCIF_CCFG_CLMODE(afe_clamp_mode) | 
        SCIF_CCFG_CLE1(sensor_clamp_edges[0]) |
        SCIF_CCFG_CCLE2(sensor_clamp_edges[1]) |
        SCIF_CCFG_ACLE2(sensor_clamp_edges[2]); 

    scfg1_val = *SCIF_SCFG1;
    
    // clear out fields we will be setting    
    scfg1_val = scfg1_val & ~((SCIF_SCFG1_CPCLKEN(0xF) | SCIF_SCFG1_SCPOL_CPCLK(0xF))); 

    // enable ccd clamp line and set requested polarity bits
    *SCIF_SCFG1 = scfg1_val | SCIF_SCFG1_CPCLKEN(1) | SCIF_SCFG1_SCPOL_CPCLK(clamp_mode_polarity);
}

static struct scan_register {
    const char *name;
    volatile uint32_t *addr;
} regs[] = {
    { "SCFG1",    SCIF_SCFG1, },
    { "SCFG2",    SCIF_SCFG2, },
    { "SCFG3",    SCIF_SCFG3, },
    { "SCTRL",    SCIF_SCTRL, },
    { "SCMD",    SCIF_SCMD, },
    { "SSTAT",    SCIF_SSTAT, },
    { "SIACK",    SCIF_SIACK, },
    { "SCANX",    SCIF_SCANX, },
    { "SCYCLE",    SCIF_SCYCLE, },
    { "STCFG1",    SCIF_STCFG1, },
    { "STCFG2",    SCIF_STCFG2, },
    { "STCFG3",    SCIF_STCFG3, },
    { "STCFG4",    SCIF_STCFG4, },
    { "CCFG",    SCIF_CCFG, },
    { "SCLK1",    SCIF_SCLK1, },
    { "SCLK2",    SCIF_SCLK2, },
    { "SCLK3",    SCIF_SCLK3, },
    { "SCLK4",    SCIF_SCLK4, },
    { "SCLK5",    SCIF_SCLK5, },
    { "SCLK6",    SCIF_SCLK6, },
    { "ACLK1",    SCIF_ACLK1, },
    { "ACLK2",    SCIF_ACLK2, },
    { "ACLK3",    SCIF_ACLK3, },
    { "ACLK4",    SCIF_ACLK4, },
    { "ACLK5",    SCIF_ACLK5, },
#ifdef HAVE_SCIF_ACLK6
    { "ACLK6",    SCIF_ACLK6, },
#endif
    { "BPWM",    SCIF_BPWM, },
#ifdef HAVE_SCAN_BLOCK_2005
    { "LPWM",    SCIF_LPWM, },
#endif
#ifdef SCIF_CFGARB 
    { "CFGARB",    SCIF_CFGARB, },
#endif
    { "LED0",    SCIF_LED0, },
    { "LED1",    SCIF_LED1, },
    { "LED2",    SCIF_LED2, },
    { "MCFG",    SCIF_MCFG, },
#ifdef HAVE_SCAN_BLOCK_2005
    { "L6219",    SCIF_6219MCTRL, },
//    { "A3967",    SCIF_3967MCTRL, },
//    { "T62209",    SCIF_62209MCTRL, },
    { "MPWMCFG",SCIF_MPWMCFG, },
    { "PCTRLA",    SCIF_PCTRLA, },
    { "PCTRLB",    SCIF_PCTRLB, },
    { "PCTRLC",    SCIF_PCTRLC, },
    { "PCTRLD",    SCIF_PCTRLD, },
#endif
    { "AFEPC",    SCIF_AFEPC, },
//    { "AFEPD",    SCIF_AFEPD, },

    /* davep 12-Oct-2010 ; if we have LPWM0, assume we have both */
#ifdef SCIF_LPWM0
    { "LPWM0",    SCIF_LPWM0, },
    { "LPWM1",    SCIF_LPWM1, },
#endif
};

void scif_dump(void)
{
    int i;
    uint32_t scan_clock_mhz;

    scan_clock_mhz = hw_get_scan_clk_speed();
    dbg1( "%s scan_clock_mhz=%d\n", __FUNCTION__, scan_clock_mhz );

    for (i = 0; i < sizeof(regs)/sizeof(struct scan_register); i++) {
        dbg1( "%8s=0x%08x", regs[i].name, *regs[i].addr ); 
        if (i%4 == 3) {
            dbg1("\n");
        }
    }
    if (i%4 != 0) {
        dbg1("\n");
    }

    /* davep 16-Jul-2012 ; as of this writing, our current platforms have three
     * separate LDATA settings
     */
#ifdef SCAN_LDATA1_DS1_MASK
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LDATA1", scif_regs->LDATA1,
            "LDATA2", scif_regs->LDATA2,
            "LDATA3", scif_regs->LDATA3,
            "LDATA4", scif_regs->LDATA4,
            "LDATA5", scif_regs->LDATA5,
            "LDATA6", scif_regs->LDATA6 );
#endif

#ifdef SCAN_LDATA10_DS1_MASK
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LDATA10", scif_regs->LDATA10,
            "LDATA20", scif_regs->LDATA20,
            "LDATA30", scif_regs->LDATA30,
            "LDATA40", scif_regs->LDATA40,
            "LDATA50", scif_regs->LDATA50,
            "LDATA60", scif_regs->LDATA60 );
#endif

#ifdef SCAN_LDATA11_DS1_MASK
    dbg1( "%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
            "LDATA11", scif_regs->LDATA11,
            "LDATA21", scif_regs->LDATA21,
            "LDATA31", scif_regs->LDATA31,
            "LDATA41", scif_regs->LDATA41,
            "LDATA51", scif_regs->LDATA51,
            "LDATA61", scif_regs->LDATA61 );
#endif

    dbg1( "%8s=0x%08x %8s=0x%08x\n", 
            "PSEQ1", scif_regs->PSEQ1,
            "PSEQ2", scif_regs->PSEQ2 );

    /* davep 16-Jul-2012 ; LEDs continue to diverge so pushed into separate
     * function
     */
    scif_led_dump();
}

void scif_reset(void)
{
    dbg2( "%s\n", __FUNCTION__ );

    /* shut down all clocks, interrupts, and commands first */
    *SCIF_SCTRL = 0; 

    /* ack any and all pending interrupts */
    *SCIF_SIACK = ~0;

    /* disable all clocks */
    *SCIF_SCFG1 = 0; 

    *SCIF_SCTRL |= SCIF_SCTRL_SRESET(1); // Scan Block soft reset
    cpu_spin_delay( 1 );
    *SCIF_SCTRL &= ~SCIF_SCTRL_SRESET(1);

    /* davep 30-Sep-2005 ; set all registers to their default power-on
     * values. 
     */
    *SCIF_SCFG1 = SCAN_CFG1_R;
    *SCIF_SCFG2 = SCAN_CFG2_R;
    *SCIF_SCFG3 = SCAN_CFG3_R;
    *SCIF_SCTRL = SCAN_CTRL_R;
    *SCIF_SCANX = SCAN_X_R;
    *SCIF_SCYCLE = SCAN_CYCLE_R;
    *SCIF_STCFG1 = SCAN_START_CFG1_R;
    *SCIF_STCFG2 = SCAN_START_CFG2_R;
    *SCIF_STCFG3 = SCAN_START_CFG3_R;
    *SCIF_STCFG4 = SCAN_START_CFG4_R;
    *SCIF_CCFG = SCAN_CLMP_CFG_R;
    *SCIF_SCLK1 = SCAN_SENS_CLK1_R;
    *SCIF_SCLK2 = SCAN_SENS_CLK2_R;
    *SCIF_SCLK3 = SCAN_SENS_CLK3_R;
    *SCIF_SCLK4 = SCAN_SENS_CLK4_R;
    *SCIF_SCLK5 = SCAN_SENS_CLK5_R;
    *SCIF_SCLK6 = SCAN_SENS_CLK6_R;
    *SCIF_ACLK1 = SCAN_AFE_CLK1_R;
    *SCIF_ACLK2 = SCAN_AFE_CLK2_R;
    *SCIF_ACLK3 = SCAN_AFE_CLK3_R;
    *SCIF_ACLK4 = SCAN_AFE_CLK4_R;
    *SCIF_ACLK5 = SCAN_AFE_CLK5_R;
    *SCIF_LDATA1 = SCAN_AFE_LDATA1_R;
    *SCIF_LDATA2 = SCAN_AFE_LDATA2_R;
    *SCIF_LDATA3 = SCAN_AFE_LDATA3_R;
    *SCIF_LDATA4 = SCAN_AFE_LDATA4_R;
    *SCIF_LDATA5 = SCAN_AFE_LDATA5_R;
    *SCIF_LDATA6 = SCAN_AFE_LDATA6_R;

    /* davep 29-Mar-2013 ; adding sandrac's 2nd AFE LDATA changes */
#ifdef SCAN_LDATA11_DS1_MASK
    scif_regs->LDATA11 = SCAN_AFE_LDATA1_R;
    scif_regs->LDATA21 = SCAN_AFE_LDATA2_R;
    scif_regs->LDATA31 = SCAN_AFE_LDATA3_R;
    scif_regs->LDATA41 = SCAN_AFE_LDATA4_R;
    scif_regs->LDATA51 = SCAN_AFE_LDATA5_R;
    scif_regs->LDATA61 = SCAN_AFE_LDATA6_R;
#endif    
    *SCIF_PSEQ1 = SCAN_AFE_PSEQ1_R;
    *SCIF_PSEQ2 = SCAN_AFE_PSEQ2_R;
    *SCIF_BPWM = SCAN_BULB_PWM_R;
//#ifdef HAVE_SCAN_BLOCK_2005
//    *SCIF_LPWM = SCAN_LED_PWM_R;
//#endif
//
//    *SCIF_LED0 = SCAN_LED0_GATE_R;
//    *SCIF_LED1 = SCAN_LED1_GATE_R;
//    *SCIF_LED2 = SCAN_LED2_GATE_R;
    scif_led_reset();

#ifdef HAVE_SCAN_BLOCK_2005
    *SCIF_6219MCTRL = SCAN_L6219_MCTRL_R; 
    *SCIF_3967MCTRL = SCAN_A3967_MCTRL_R; 
    *SCIF_62209MCTRL = SCAN_T62209_MCTRL_R;  
    *SCIF_MPWMCFG = SCAN_M_PWMCFG_R; 
    *SCIF_PCTRLA = SCAN_PWM_CTRLA_R; 
    *SCIF_PCTRLB = SCAN_PWM_CTRLB_R; 
    *SCIF_PCTRLC = SCAN_PWM_CTRLC_R; 
#endif

//    /* davep 12-Oct-2010 ; if we have LPWM0, assume we have both */
//#ifdef SCIF_LPWM0
//    *SCIF_LPWM0 = SCAN_LPWM0_R;
//    *SCIF_LPWM1 = SCAN_LPWM1_R;
//#endif

    /* davep 04-May-2010 ; sanity check our registers are in the correct
     * location. Check some regs that are not 0 or ~0 at power on. Check
     * read-write and read-only regs.
     */
//    scanlog_hex_dump( (unsigned char *)SCIF_SCFG1, 64 );
    XASSERT( *SCIF_SCFG1==SCAN_CFG1_R, (uint32_t)SCIF_SCFG1 );

    /*  SCIF_SSTAT_UNEXPSYNC is set if a motor_sync comes in when the block wasn't 
     *  expecting one (this was intended for debug use only).  This bit is flapping around,
     *  which is strange given DC motor is only doing sync first. Ignore it for now.
     */
#ifdef HAVE_SCIF_UNEXPSYNC
    XASSERT( ((*SCIF_SSTAT) & ~(SCIF_SSTAT_SCANYR|SCIF_SSTAT_UNEXPSYNC))==SCAN_STAT_R, (uint32_t)*SCIF_SSTAT );
#else
    XASSERT( ((*SCIF_SSTAT) & ~SCIF_SSTAT_SCANYR)==SCAN_STAT_R, (uint32_t)*SCIF_SSTAT );
#endif

    /* davep 28-Jan-2013 ; belatedly adding reset of CFGARB */
#ifdef SCIF_CFGARB 
    scif_regs->CFGARB = SCAN_CFGARB_R;
#endif
}

/**
 * \brief If scan block register exists in scands, use that value 
 *
 * Overrides the named register's value. Allows us to change some of the scan
 * block registers while tuning clock edges.
 *
 * Originally part of merge_register_override() but moved to own function so
 * could add the LDATA registers
 *
 * \author David Poole
 * \date 24-Oct-2013
 */

static void register_override( const char *name, volatile uint32_t *regaddr )
{
    scan_err_t scerr;
    uint32_t value;

    scerr = scands_get_integer_by_name( name, &value );
    if( scerr == SCANERR_NONE ) {
        dbg2( "%s using value=0x%08x for %s\n", __FUNCTION__, value, name );
        *regaddr = value;
    }
}

/**
 * \brief merge command line set register values 
 *
 * Created this code to allow command line get/set of register values before we
 * start a scan.
 *
 * Feel free to shoot yourself in the foot.
 *
 * \author David Poole
 * \date 27-Sep-2010
 *
 */

static void merge_register_override( void )
{
    int i;

    for( i=0 ; i < sizeof(regs)/sizeof(struct scan_register) ; i++) {
        register_override(regs[i].name, regs[i].addr );
    }

    /* davep 01-Nov-2013 ; adding LDATA override. LDATA registers not in the
     * scan_register list. LDATA registers are very different across ASICs.
     * Plus the new "scif_regs" style of code doesn't work in the compile-time
     * scan-register list.
     */
#ifdef SCAN_LDATA1_DS1_MASK
    register_override( "LDATA1", &scif_regs->LDATA1 );
    register_override( "LDATA2", &scif_regs->LDATA2 );
    register_override( "LDATA3", &scif_regs->LDATA3 );
    register_override( "LDATA4", &scif_regs->LDATA4 );
    register_override( "LDATA5", &scif_regs->LDATA5 );
    register_override( "LDATA6", &scif_regs->LDATA6 );
#endif

#ifdef SCAN_LDATA10_DS1_MASK
    register_override("LDATA10", &scif_regs->LDATA10 );
    register_override("LDATA20", &scif_regs->LDATA20 );
    register_override("LDATA30", &scif_regs->LDATA30 );
    register_override("LDATA40", &scif_regs->LDATA40 );
    register_override("LDATA50", &scif_regs->LDATA50 );
    register_override("LDATA60", &scif_regs->LDATA60 );
#endif

#ifdef SCAN_LDATA11_DS1_MASK
    register_override( "LDATA11", &scif_regs->LDATA11 );
    register_override( "LDATA21", &scif_regs->LDATA21 );
    register_override( "LDATA31", &scif_regs->LDATA31 );
    register_override( "LDATA41", &scif_regs->LDATA41 );
    register_override( "LDATA51", &scif_regs->LDATA51 );
    register_override( "LDATA61", &scif_regs->LDATA61 );
#endif
}

void scif_sensor_setup(SENSOR_CONF *sc)
{
    //*********************************************************************
    // NOTE: DO NOT change the LED gate times or PWM here. They've already been
    // set by calibration.
    //*********************************************************************

    /* davep 22-Jul-2008 ; sanity checks on how we use the enum since we're
     * mapping it into the register setting
     */
    XASSERT( SCAN_CMODE_MONO==1, SCAN_CMODE_MONO );
    
    /* davep 17-Feb-2008 ; set SCIF_SCIFG2_CMODE to mono for now; must be
     * changed later as we're setting up the per-scan 
     *
     * Set SSEL to zero since it depends on our DPI.  (Will set later.)
     * 
     */
    *SCIF_SCFG2 =
        SCIF_SCFG2_CMODE(SCAN_CMODE_MONO) |
        SCIF_SCFG2_STYPE(sc->STYPE) |
        SCIF_SCFG2_SSEL(0) | 
        SCIF_SCFG2_AFEWIDTH(sc->AFEWIDTH) |
        SCIF_SCFG2_PIXPER(sc->PIXPER-1);

    /* Note I'm enabling the scan command queue but leaving the other fields
     * disabled. 
     *
     * See scif_control() to enable/disable SCTRL.RUN and SCTRL.SCMDIE fields.
     * See scif_run() to enable/disable SCTRL.SCANEN field.
     */
    *SCIF_SCTRL = SCIF_SCTRL_CMDQEN(1); 

    /* 
     * SCANX is set up later since it depends on our DPI
     */

    /* davep 26-Mar-2010 ; asic team replaced the MSTEPP field with the EXPPER
     * (Exposure Field Data)
     */
#ifdef SCIF_SCYCLE_EXPPER
    *SCIF_SCYCLE =
        SCIF_SCYCLE_EXPPER(SCAN_SCYCLE_DEFAULT-1) |
        SCIF_SCYCLE_SCYCLE(SCAN_SCYCLE_DEFAULT-1);
#else
    *SCIF_SCYCLE =
        SCIF_SCYCLE_MSTEPP(SCAN_SCYCLE_DEFAULT-1) |
        SCIF_SCYCLE_SCYCLE(SCAN_SCYCLE_DEFAULT-1);
#endif

    *SCIF_STCFG1 =
        SCIF_STCFG1_SPGEN(sc->SPGEN == SPGEN_SYSTEM_CLOCK) |
        SCIF_STCFG1_SSME1(sc->sensor_ssm_edges[0]) |
        SCIF_STCFG1_SSME2(sc->sensor_ssm_edges[1]);

    *SCIF_STCFG2 =
        SCIF_STCFG2_SPE1(sc->sensor_sp_edges[0]) |
        SCIF_STCFG2_SPE2(sc->sensor_sp_edges[1]);

    *SCIF_STCFG3 =
        SCIF_STCFG3_APH1E1(sc->sensor_aph1clk_edges[0]) |
        SCIF_STCFG3_APH1E2(sc->sensor_aph1clk_edges[1]) |
        SCIF_STCFG3_APH1E3(sc->sensor_aph1clk_edges[2]) |
        SCIF_STCFG3_APH1E4(sc->sensor_aph1clk_edges[3]);

    *SCIF_STCFG4 =
        SCIF_STCFG4_APH1E5(sc->sensor_aph1clk_edges[4]) |
        SCIF_STCFG4_APH1E6(sc->sensor_aph1clk_edges[5]) |
        SCIF_STCFG4_APH1E7(sc->sensor_aph1clk_edges[6]) |
        SCIF_STCFG4_APH1E8(sc->sensor_aph1clk_edges[7]);

    *SCIF_CCFG =
        SCIF_CCFG_CPMODE(sc->ccd_clamp_mode) |
        SCIF_CCFG_CLMODE(sc->afe_clamp_mode) |
        SCIF_CCFG_CLE1(sc->sensor_clamp_edges[0]) |
        SCIF_CCFG_CCLE2(sc->sensor_clamp_edges[1]) |
        SCIF_CCFG_ACLE2(sc->sensor_clamp_edges[2]);

    *SCIF_SCLK1 =
        SCIF_SCLK1_P1E1(sc->sensor_p1clk_edges[0]) |
        SCIF_SCLK1_P1E2(sc->sensor_p1clk_edges[1]) |
        SCIF_SCLK1_P1E3(sc->sensor_p1clk_edges[2]) |
        SCIF_SCLK1_P1E4(sc->sensor_p1clk_edges[3]);
    *SCIF_SCLK2 =
        SCIF_SCLK2_P1E5(sc->sensor_p1clk_edges[4]) |
        SCIF_SCLK2_P1E6(sc->sensor_p1clk_edges[5]) |
        SCIF_SCLK2_P1E7(sc->sensor_p1clk_edges[6]) |
        SCIF_SCLK2_P1E8(sc->sensor_p1clk_edges[7]);

    *SCIF_SCLK3 =
        SCIF_SCLK3_P2E1(sc->sensor_p2clk_edges[0]) |
        SCIF_SCLK3_P2E2(sc->sensor_p2clk_edges[1]) |
        SCIF_SCLK3_P2E3(sc->sensor_p2clk_edges[2]) |
        SCIF_SCLK3_P2E4(sc->sensor_p2clk_edges[3]);
    *SCIF_SCLK4 =
        SCIF_SCLK4_P2E5(sc->sensor_p2clk_edges[4]) |
        SCIF_SCLK4_P2E6(sc->sensor_p2clk_edges[5]) |
        SCIF_SCLK4_P2E7(sc->sensor_p2clk_edges[6]) |
        SCIF_SCLK4_P2E8(sc->sensor_p2clk_edges[7]);

    *SCIF_SCLK5 =
        SCIF_SCLK5_RSE1(sc->sensor_rsclk_edges[0]) |
        SCIF_SCLK5_RSE2(sc->sensor_rsclk_edges[1]) |
        SCIF_SCLK5_RSE3(sc->sensor_rsclk_edges[2]) |
        SCIF_SCLK5_RSE4(sc->sensor_rsclk_edges[3]);

    *SCIF_SCLK6 =
        SCIF_SCLK6_CPE1(sc->sensor_cpclk_edges[0]) |
        SCIF_SCLK6_CPE2(sc->sensor_cpclk_edges[1]) |
        SCIF_SCLK6_CPE3(sc->sensor_cpclk_edges[2]) |
        SCIF_SCLK6_CPE4(sc->sensor_cpclk_edges[3]);

    *SCIF_ACLK1 =
        SCIF_ACLK1_MCE1(sc->afe_mclk_edges[0]) |
        SCIF_ACLK1_MCE2(sc->afe_mclk_edges[1]) |
        SCIF_ACLK1_MCE3(sc->afe_mclk_edges[2]) |
        SCIF_ACLK1_MCE4(sc->afe_mclk_edges[3]);
    *SCIF_ACLK2 =
        SCIF_ACLK2_MCE5(sc->afe_mclk_edges[4]) |
        SCIF_ACLK2_MCE6(sc->afe_mclk_edges[5]) |
        SCIF_ACLK2_MCE7(sc->afe_mclk_edges[6]) |
        SCIF_ACLK2_MCE8(sc->afe_mclk_edges[7]);
    *SCIF_ACLK3 =
        SCIF_ACLK3_MCE9(sc->afe_mclk_edges[8]) |
        SCIF_ACLK3_MCE10(sc->afe_mclk_edges[9]) |
        SCIF_ACLK3_MCE11(sc->afe_mclk_edges[10]) |
        SCIF_ACLK3_MCE12(sc->afe_mclk_edges[11]);

#ifdef HAVE_SCIF_ACLK6
    *SCIF_ACLK6 =
        SCIF_ACLK6_MCE13(sc->afe_mclk_edges[12]) |
        SCIF_ACLK6_MCE14(sc->afe_mclk_edges[13]) |
        SCIF_ACLK6_MCE15(sc->afe_mclk_edges[14]) |
        SCIF_ACLK6_MCE16(sc->afe_mclk_edges[15]);
#endif

    *SCIF_ACLK4 =
        SCIF_ACLK4_VSE1(sc->afe_vsclk_edges[0]) |
        SCIF_ACLK4_VSE2(sc->afe_vsclk_edges[1]) |
        SCIF_ACLK4_VSE3(sc->afe_vsclk_edges[2]) |
        SCIF_ACLK4_VSE4(sc->afe_vsclk_edges[3]);

    *SCIF_ACLK5 =
        SCIF_ACLK5_CD1E1(sc->afe_cd1clk_edges[0]) |
        SCIF_ACLK5_CD1E2(sc->afe_cd1clk_edges[1]) |
        SCIF_ACLK5_CD1E3(sc->afe_cd1clk_edges[2]) |
        SCIF_ACLK5_CD1E4(sc->afe_cd1clk_edges[3]);

    *SCIF_LDATA1 =
        SCIF_LDATA_DS_A(sc->afe_ldata[0].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[0].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[1].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[1].code);
    *SCIF_LDATA2 =
        SCIF_LDATA_DS_A(sc->afe_ldata[2].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[2].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[3].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[3].code);
    *SCIF_LDATA3 =
        SCIF_LDATA_DS_A(sc->afe_ldata[4].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[4].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[5].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[5].code);
    *SCIF_LDATA4 =
        SCIF_LDATA_DS_A(sc->afe_ldata[6].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[6].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[7].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[7].code);
    *SCIF_LDATA5 =
        SCIF_LDATA_DS_A(sc->afe_ldata[8].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[8].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[9].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[9].code);
    *SCIF_LDATA6 =
        SCIF_LDATA_DS_A(sc->afe_ldata[10].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata[10].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata[11].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata[11].code);

    /* davep 29-Mar-2013 ; adding sandrac's 2nd AFE LDATA changes */
#ifdef SCAN_LDATA11_DS1_MASK
    scif_regs->LDATA11  =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[0].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[0].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[1].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[1].code);
    scif_regs->LDATA21 =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[2].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[2].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[3].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[3].code);
    scif_regs->LDATA31 =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[4].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[4].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[5].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[5].code);
    scif_regs->LDATA41 =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[6].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[6].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[7].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[7].code);
    scif_regs->LDATA51 =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[8].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[8].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[9].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[9].code);
    scif_regs->LDATA61 =
        SCIF_LDATA_DS_A(sc->afe_ldata_a2[10].clock) |
        SCIF_LDATA_DC_A(sc->afe_ldata_a2[10].code) |
        SCIF_LDATA_DS_B(sc->afe_ldata_a2[11].clock) |
        SCIF_LDATA_DC_B(sc->afe_ldata_a2[11].code);
#endif
    *SCIF_PSEQ1 =
        SCIF_PSEQ1_PSEQ1(sc->afe_pixel_seq[0]) |
        SCIF_PSEQ1_PSEQ2(sc->afe_pixel_seq[1]) |
        SCIF_PSEQ1_PSEQ3(sc->afe_pixel_seq[2]) |
        SCIF_PSEQ1_PSEQ4(sc->afe_pixel_seq[3]);
    *SCIF_PSEQ2 =
        SCIF_PSEQ2_PSEQ5(sc->afe_pixel_seq[4]) |
        SCIF_PSEQ2_PSEQ6(sc->afe_pixel_seq[5]);
    
    //*********************************************************************
    // NOTE: DO NOT change the LED gate times or PWM here. They've already been
    // set by calibration.
    //*********************************************************************
    
    *SCIF_SCFG1 =
        (*SCIF_SCFG1 & // preserve LED & Bulb enables
            (SCIF_SCFG1_LEDEN(-1) |
             SCIF_SCFG1_BULBEN(1))) |
        SCIF_SCFG1_P1CLKEN(sc->SCFG1.p1clken) |
        SCIF_SCFG1_P2CLKEN(sc->SCFG1.p2clken) |
        SCIF_SCFG1_RSCLKEN(sc->SCFG1.rsclken) |
        SCIF_SCFG1_CPCLKEN(sc->SCFG1.cpclken) |
        SCIF_SCFG1_MCLKEN(sc->SCFG1.mclken)   |
        SCIF_SCFG1_VSCLKEN(sc->SCFG1.vsclken) |
        SCIF_SCFG1_CLCKEN(sc->SCFG1.clclken)  |
        SCIF_SCFG1_LCLMPEN(sc->SCFG1.lclpen)  |
        SCIF_SCFG1_SSMEN(sc->SCFG1.ssmen)  |
        SCIF_SCFG1_SMPOL(sc->SCFG1.smpol)  |
        SCIF_SCFG1_P1APOL(sc->SCFG1.p1apol)|
        SCIF_SCFG1_SPPOL(sc->SCFG1.sppol)  |
        SCIF_SCFG1_SCPOL(sc->SCFG1.scpol)  |
        SCIF_SCFG1_ACPOL(sc->SCFG1.acpol) ;

    *SCIF_SCFG3 =
        SCIF_SCFG3_P1RISE(sc->SCFG3.p1rise) |
        SCIF_SCFG3_P1FALL(sc->SCFG3.p1fall) |
        SCIF_SCFG3_P2RISE(sc->SCFG3.p2rise) |
        SCIF_SCFG3_P2FALL(sc->SCFG3.p2fall) |
        SCIF_SCFG3_RSRISE(sc->SCFG3.rsrise) |
        SCIF_SCFG3_RSFALL(sc->SCFG3.rsfall) |
        SCIF_SCFG3_CPRISE(sc->SCFG3.cprise) |
        SCIF_SCFG3_CPFALL(sc->SCFG3.cpfall) |
        SCIF_SCFG3_MCRISE(sc->SCFG3.mcrise) |
        SCIF_SCFG3_MCFALL(sc->SCFG3.mcfall) |
        SCIF_SCFG3_VSRISE(sc->SCFG3.vsrise) |
        SCIF_SCFG3_VSFALL(sc->SCFG3.vsfall) |
        SCIF_SCFG3_CD1RISE(sc->SCFG3.cd1rise) |
        SCIF_SCFG3_CD1FALL(sc->SCFG3.cd1fall) |
        SCIF_SCFG3_SPRISE(sc->SCFG3.sprise) |
        SCIF_SCFG3_SPFALL(sc->SCFG3.spfall);

    /* davep 27-Jun-2011 ; poke the new ASIC CFGARB register */
    scif_cbi_arbitration_config( sc->cbi_enable_external, sc->cbi_enable_parallel );


    /* davep 27-Sep-2010 ; adding command line feature to set override values
     * for registers 
     */
    merge_register_override();
}

void
scif_set_scanx( uint32_t scanx )
{
    /* -1 because the SCANX register counts from zero */
    scif_regs->SCANX = scanx-1;
}

void
scif_set_pixel_period( uint32_t pixel_period )
{
//    dbg2( "%s pixel_period=%d\n", __FUNCTION__, pixel_period );

    /* clear previous value */
    *SCIF_SCFG2 &= ~SCIF_SCFG2_PIXPER(-1);

    /* set new value */
    *SCIF_SCFG2 |= SCIF_SCFG2_PIXPER(pixel_period-1);
}

uint32_t scif_calc_pixel_period( uint32_t sensor_freq_khz )
{
#if 0
    float sensor_mhz;

    sensor_mhz = sensor_freq_khz / 1000.0;

    /* check for initialized variable (have some new code I want to make sure
     * doesn't cause a divide-by-zero)
     */
    ASSERT( sensor_mhz > 0 );

    /* adding 0.5 to round up */
    return (uint32_t)(((float)hw_get_scan_clk_speed() / sensor_mhz) + 0.5 );
#else
    uint32_t scan_clk_khz;

    /* get scan block clock in Mhz, convert to khz */
    scan_clk_khz = hw_get_scan_clk_speed()*1000;

//    dbg2( "%s scan_clk_khz=%d sensor_freq_khz=%d pixper=%d\n", __FUNCTION__, scan_clk_khz, sensor_freq_khz,
//        safeint_divide_with_round( scan_clk_khz, sensor_freq_khz ));

    return safeint_divide_with_round( scan_clk_khz, sensor_freq_khz );
#endif
}

void
scif_set_ssel( uint8_t ssel )
{
    *SCIF_SCFG2 = (*SCIF_SCFG2 & ~SCIF_SCFG2_SSEL(-1)) // clear the bits
                |   SCIF_SCFG2_SSEL(ssel); // set the bits
}

void
scif_send_simple_scmd( bool sdata, uint32_t sclkper, uint16_t num_lines )
{
    /* make sure the scan block can accept another command (otherwise we'll get
     * a command queue overflow interrupt)
     */
    XASSERT( !(*SCIF_SSTAT & SCIF_SSTAT_QFULL), *SCIF_SSTAT );

    *SCIF_SCMD = scif_command( sdata, sclkper, num_lines );

    XASSERT( !(*SCIF_SSTAT & SCIF_SSTAT_QEMPTY), *SCIF_SSTAT );
}

uint32_t scif_calc_sclkper( scan_cmode_t cmode, uint32_t dpi, uint32_t pixeltimes_per_line ) 
{
    /* davep 18-Nov-2011 ; h@ck to allow platform specific calculation of
     * sclkper. Very weird, very specific special case.
     *
     * Originally created to allow HarrisH to handle three channel external
     * AFE with 3-segment sensor on high-speed Quartz hardware.
     *
     * TODO make scanimg and afe into full blooded drivers (structure with
     * function pointers); maybe scanplat, too. Need an ioctl to handle weird
     * stuff like this.
     */
#ifdef HAVE_SCANPLAT_SCLKPER
    return scanplat_calc_sclkper( cmode, dpi, pixeltimes_per_line );
#endif

    /* pixeltimes_per_line is the number of pixel periods we want in our
     * scanline (the time between start pulses)
     * 
     * NOTE: pixelstimes_per_line may or may not be the number of pixels in the
     * sensor. (WTF!?) Let me explain. In this function, the
     * pixeltimes_per_line is the number of pixels between Start Pulses. When
     * scanning with a motor, the sensor can capture data faster than we can
     * move the motor. So our Start Pulses need to be far enough apart to take
     * into account the time it takes to move to the next line. For example,
     * suppose the sensor has a 500ns pixel.  The sensor could deliver 3200
     * pixels in 1.6ms.  But suppose the motor takes us 5ms to get to the next
     * line. Thus we need our Start Pulses 5ms apart.  Using d=rt, we have:
     *
     *   num_pixels = (1pixel/500ns) * 5ms
     *   num_pixels = 5ms / 500ns
     *   num_pixels = 5e-3 / 500e-9
     *   num_pixels = 10000
     */
    
    /* Assuming we're hardwiring SCYCLE to 32 (the same as it's always been
     * since we have yet to see a need to change the damn thing).
     */

    /* +0.5 to round up */
#if 0
    return  (uint32_t)( (float)pixeltimes_per_line / SCAN_SCYCLE_DEFAULT + 0.5 );
#else
    return safeint_divide_with_round( pixeltimes_per_line, SCAN_SCYCLE_DEFAULT );
#endif
}


void scif_set_internal_scanline_mode( scan_cmode_t cmode, uint32_t dpi, uint32_t scanx, uint32_t *sclkper )
{
    scif_motor_setup(MOTOR1, SYNC_DISABLE, MOT_DC, MOT_DC);

    /* we need to send sclkper back for SCMD */
    *sclkper = scif_calc_sclkper( cmode, dpi, scanx );

    /* add 10% to keep our lines from colliding with each other */
    *sclkper += *sclkper/10;

//    dbg2( "%s cmode=%d dpi=%d scanx=%d sclkper=%d\n", __FUNCTION__, 
//            cmode, dpi, scanx, *sclkper );
}

/**
 * \brief CBI Arbitration Configuration 
 *
 * enable_external : "Enables the CBI arbitration block to accept data from the
 * respective external interface and multiplex it into the output datastream. "
 *
 * enable_parallel : "Enables the CBI arbitration block to accept data from the
 * respective parallel (sdata) interface and multiplex it into the output
 * datastream." 
 *
 * \author David Poole
 * \date 29-Jun-2011
 *
 */

void scif_cbi_arbitration_config( bool enable_external, bool enable_parallel  )
{
#ifdef SCIF_CFGARB 
    /* One or the other must be enabled. Hopefully trap old code which needs to
     * update scansen_get_sensor_conf()
     */
    XASSERT( enable_external || enable_parallel, enable_external );

    /* clear the fields */
    scif_regs->CFGARB &= ~(SCAN_CFGARB_EXTENABLE_MASK|SCAN_CFGARB_PARENABLE_MASK);

    /* set the fields */
    if( enable_external ) {
        scif_regs->CFGARB = SCAN_CFGARB_EXTENABLE_REPLACE_VAL( scif_regs->CFGARB, 1 );
    }

    if( enable_parallel ) {
        scif_regs->CFGARB = SCAN_CFGARB_PARENABLE_REPLACE_VAL( scif_regs->CFGARB, 1 );
    }
#endif
}

/**
 * \brief  Tentative function to enable/disable dual scan
 *
 *  Not final code! See note below.
 *
 * \author David Poole
 * \date 23-Jan-2013
 */

void scif_cbi_enable_dual_channel( bool enable )
{
#ifdef SCIF_CFGARB 
    /* davep 21-Jan-2013 ; XXX this function is just a temporary placehold
     * while turning on dual scan. Need something that will allow us to change
     * the parenable flags to 1, 2, or 3.
     */
    if( enable ) {
//        scif_regs->CFGARB = SCAN_CFGARB_PARENABLE_REPLACE_VAL( scif_regs->CFGARB, 1 );
//        scif_regs->CFGARB = SCAN_CFGARB_PARENABLE_REPLACE_VAL( scif_regs->CFGARB, 2 );
        scif_regs->CFGARB = SCAN_CFGARB_PARENABLE_REPLACE_VAL( scif_regs->CFGARB, 3 );
    }
    else {
        scif_regs->CFGARB = SCAN_CFGARB_PARENABLE_REPLACE_VAL( scif_regs->CFGARB, 1 );
    }
#endif
}

/**
 * \brief  Get Scan Block Dual Channel enable/disable state
 *
 * Created for sanity checking. All of Scan block, CISX, and PIC must be in
 * agreement for dual channel mode.
 *
 * \author David Poole
 * \date 03-Apr-2013
 */

bool scif_get_dual_channel_enabled( void )
{
#ifdef SCIF_CFGARB 
    /* Note! I'm only checking the parallel sources. */
    return (scif_regs->CFGARB & SCAN_CFGARB_PARENABLE_MASK)==3;
#else
    return FALSE;
#endif
}

/**
 * \brief Build a PSEQ pixel register field value.
 *
 * \author David Poole
 * \date 15-Mar-2013
 *
 * From the programmer's guide:
 *
 * S = Sequence Complete
 * P = DataType Complete; also decrements the SCANX data count when P = 1.
 * V = Output Valid
 *
 * Color[2:0] = Specified by the PIC Common Bus Interface: 
 * 000 = Color 0 non-staggered
 * 001 = Color 1 non-staggered
 * 010 = Color 2 non-staggered
 * 011 = Monochrome non-staggered 100 = Color 0 staggered
 * 101 = Color 1 staggered
 * 110 = Color 2 staggered
 * 111 = Monochrome staggered
 *
 */

#define SCIF_PSEQ_S(x) (((x)&1)<<5)
#define SCIF_PSEQ_P(x) (((x)&1)<<4)
#define SCIF_PSEQ_V(x) (((x)&1)<<3)
#define SCIF_PSEQ_COLOR(x) ((x)&0x07)  /* three bits */

uint32_t scif_pseq( uint32_t S, uint32_t P, uint32_t V, uint32_t color )
{
    return SCIF_PSEQ_S(S) 
            | SCIF_PSEQ_P(P) 
            | SCIF_PSEQ_V(V) 
            | SCIF_PSEQ_COLOR(color);
}

