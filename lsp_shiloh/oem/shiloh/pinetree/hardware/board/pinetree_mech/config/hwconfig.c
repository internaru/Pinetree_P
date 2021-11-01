/* 
 *
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
 */

/** 
 *
 * \brief This file is where we execute the board/asic specific stuff for
 * a given product.  Mapping of i/o lines etc should be here.
 * gpio bypass should also be here.
 *
 **/
#include <stdint.h>
#include <string.h>
#include "logger.h"
#include "debug.h"
#include "error_types.h"
#include "lassert.h"
#include "axi_bus_api.h"

#include "asic.h"

typedef struct
{
    uint32_t bus_speed_mhz;     
    uint32_t proc_speed_mhz;     
    uint32_t platform_mem_size;
    uint32_t scan_clock_mhz;
} HW_CONFIG_TABLE;

static HW_CONFIG_TABLE hw_config_table;

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int hwConfig_callback(int argc, char *argv[]);
#endif

#if 0
#define ASIC_ID_RegMask *( (uint32_t *)0xF8000000 )

#define CPUMCCR1        *( (volatile uint32_t *)0xF8000284 )

/* Powerdown Configuration Register */
#define PWCR_REG         0xF8000200

/* timebase configuration */
#define TIMEBASE_TCR *((uint32_t *)0xF8030000)

// dec timebase configuration
#define DEC_TIMEBASE_TCR *((uint32_t *)0xF9308100)
#endif

/* FUNCTION NAME: hwConfigInit */
 
/** 
 * \brief Initialize the hw specific functions.
 * 
 * \retval None
 * 
 **/
void hwConfigInit(void)
{
    /* Create a copy of the hardware configuration table stored in the LCM
     * because the LCM will eventually be destroyed. */
//-jrs    HW_CONFIG_TABLE * temp_config_table;

//-jrs    MC_REGS_t *mc_registers = (MC_REGS_t *)MC_BASE;

    // TODO: read these values from the OS; for now hardcode so we can make progress on other items.
    hw_config_table.proc_speed_mhz = 800;
    hw_config_table.platform_mem_size = 512 * 1024 * 1024;
    hw_config_table.bus_speed_mhz = 200;
    if (hw_config_table.bus_speed_mhz == 200)
    {
        hw_config_table.bus_speed_mhz = 198; /// hack foo revisit remove BUG!: dithered bus is 1% slower than max bus speed.
    }

    hw_config_table.scan_clock_mhz = 250; // 250 MHz undithered clock

    // boost bus priorities for critical blocks; needed to allow 600dpi duplex copy -jrs
    axi_bus_set_qos(AXI_BUS_BLOCK_MVDO, 1, 1);
    axi_bus_set_qos(AXI_BUS_BLOCK_PIC, 1, 1);
    axi_bus_set_qos(AXI_BUS_BLOCK_CISX, 1, 1);

    /* initialize the timebase configuration with the bus frequency */
    // TIMEBASE_TCR = hw_config_table.bus_speed_mhz;
    {
//-jrs These are initialized by u-boot on the LSP
//-jrs        APB_TIMEBASE_REG_REGS_t *timebase = 
//-jrs            (APB_TIMEBASE_REG_REGS_t *)DEC_DEC_TIMEBASE_BASE;
//-jrs        timebase->TCR = 240; 
//-jrs        
//-jrs        timebase = (APB_TIMEBASE_REG_REGS_t *)APB_TIMEBASE_BASE;
//-jrs        timebase->TCR = hw_config_table.bus_speed_mhz;
    }
                
    /* Allow the JTAG debugger to connect when WFI is used */
//-jrs    CPUMCCR1 |= ( 1 << 16 );

    // Clear RTC power down
//-jrs    *(volatile uint32_t *)( PWCR_REG ) &= ~( 1 << APB_CONFIG_PWCR_POR_PDWN_SHIFT );

    // Set memory bus priority
//-jrs    mc_registers->SDRAMControl5 = MC_SDRAMCONTROL5_MSTRWT1_REPLACE_VAL( mc_registers->SDRAMControl5, 0x1 );
}

void hwConfigDebugInit()
{
#ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register("hwconfig",
                         NULL,
                         NULL,
                         NULL,
                         hwConfig_callback);
    ASSERT( CMD_OK == i_res );
#endif
}
 


/**
 * \brief get the ASIC id and version numbers 
 *
 */
void hwGetASICId( uint32_t *platform, uint8_t *major_rev, uint8_t *minor_rev )
{
    // TODO
    ASSERT(0);
//-jrs    *platform = (ASIC_ID_RegMask & 0xff00) >> 8;
//-jrs    *major_rev = (ASIC_ID_RegMask & 0x00f0) >> 4;
//-jrs    *minor_rev = (ASIC_ID_RegMask & 0x000f);
}


/** 
 * \brief return processor clock speed in megahertz.
 * 
 * \retval Processor speed in megahertz
 * 
 **/
uint32_t hwGetProcSpeed(void)
{
    return hw_config_table.proc_speed_mhz;
}


uint32_t hw_get_scan_clk_speed(void)
{
    return hw_config_table.scan_clock_mhz;
}

/** 
 * \brief return bus speed in megahertz.
 * 
 * 
 * 
 * \retval return bus speed in megahertz
 * 
 **/
uint32_t hwGetBusSpeed(void)
{
    return hw_config_table.bus_speed_mhz;
}

uint32_t hwGetDebugUARTNumber(void)
{
    return 0;
}

uint32_t hwGetRamStartAddress(void)
{
    //RAM starts at 0x0
    return 0;
}


/** 
 * \brief return the size of ram in bytes
 * 
 * \retval None
 * 
**/
uint32_t hwGetRamSize(void)
{
    return hw_config_table.platform_mem_size;
}

uint32_t hwGetUncachedRamSize(void)
{
    /// 1 MEG at top of memory for now coupled to tlb implementation
    return 0x100000; /// revisit when tlb can handle subblocks
}
uint32_t hwGetUncachedRamStartAddress(void)
{
    return hwGetRamStartAddress() + hwGetRamSize() - hwGetUncachedRamSize();
}
bool hwAddressIsCached( void *startAddr )
{
    return ((uint32_t)startAddr < hwGetUncachedRamStartAddress());
}

/* FUNCTION NAME: hwResetPlatform */
 
/** 
 * \brief Hardware specific complete platform reset/reboot.
 * 
 * \retval None
 * 
 **/
void hwPlatformReset(void)
{
    ASSERT(0);
/* TODO: need to call OS and request reboot.
    volatile TIMERS_REGS_t * timer_regs = (TIMERS_REGS_t *)APB_TMR_BASE;

    // Reset the board
    // turn on the timer
    
    // put it in continuous mode
    timer_regs->TCR0 = TIMERS_TCR0_CONTMODE_REPLACE_VAL( timer_regs->TCR0, 1 );
    // enable the timer
    timer_regs->TCR0 = TIMERS_TCR0_ENABLE_REPLACE_VAL( timer_regs->TCR0, 1 );
    // enable the watch dog
    timer_regs->TWR0 = TIMERS_TWR0_ENABLE_REPLACE_VAL( timer_regs->TWR0, 1 );

    // this should reset the thing.
    timer_regs->TWR0 = 0;
*/
}


#ifdef HAVE_CMD
static int hwConfig_callback(int argc, char *argv[])
{
    uint32_t platform;
    uint8_t majorRev, minorRev;
    int32_t pos;

    pos = 1;

    if ( strncmp( (char *)argv[pos], "reset", 5 ) == 0 )
    {
        hwPlatformReset();
    }
    else if (strncmp((char *)argv[pos], "assert", 6) == 0)
    {
        ASSERT(0);
    }
    else if (strncmp((char *)argv[pos], "ram",3) == 0)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Ram Size %d MB\n",hwGetRamSize()/0x100000));
    } else if (strncmp((char *)argv[pos], "busspeed", 8) == 0)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Bus Speed %d MHz\n", hwGetBusSpeed()));
    } else if (strncmp((char *)argv[pos], "procspeed",9) == 0)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Processor Speed %d MHz\n",hwGetProcSpeed()));
    } else if (strncmp((char *)argv[pos], "asicid", 6) == 0)
    {
        hwGetASICId(&platform, &majorRev, &minorRev);
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Asicid Plat 0x%x, Major Rev 0x%x, Minor Rev 0x%x\n",platform,
                                     majorRev, minorRev));
    } else if (strncmp((char *)argv[pos], "scanclk",7) == 0)
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Scan clock freq %d MHz\n", hw_get_scan_clk_speed()));
    }
    else
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Supported cmds,\n\treset\n\tram\n\tbusspeed\n\tprocspeed\n\tasicid\n\tscanclk\n"));
    }
    return OK;
}
#endif

