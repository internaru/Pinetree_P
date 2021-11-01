/**
 * \file afe_wm8213.c
 *
 * \brief  This is for reading and writing the external AFE registers, WM8213
 *
 *   All reading and writing of the AFE registers is done in this file. 
 *   (AFE - Analog Front End - the h/w that reads the analog data from the
 *    scan sensor and returns it to the scan block of the ASIC).
 *
 */
/*
 * 
 *
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

#include <stdbool.h>
#include <string.h>

#include "scos.h"

#include "lassert.h"
#include "regAddrs.h"
#include "utils.h"
#include "interrupt_api.h"
#include "intnums.h"
#ifdef HAVE_DBG_CALLBACK_CMDS
#include "scos_cmd_proc.h"
#endif

#include "SCCP_regstructs.h"
#include "SCCP_regmasks.h"
#include "scantypes.h"
#include "scanconfig.h"
#include "scandbg.h"
#include "scanif.h"
#include "scan.h"
#include "scanplat.h"
#include "scansen.h"
#include "afe.h"
#include "scands.h"
#include "scanimg.h"
#include "debug.h"
#include "afe_wm8213.h"
#include "scif_afe_wm8213.h"

#include "scanlib.h"
#include "scancore.h"

#include "../../../../../common/include/spec.h"

static const struct afe_hardware_config_t internal_mas_afe_config = {
    .bits_per_pixel = 16,
    .left_justify =false 
};

//#define AFE_DEBUG 1
#ifdef AFE_DEBUG
  #define afe_dbg2 dbg2 
#else
  #define afe_dbg2(...) 
#endif

#if 0
#define wm8213_read          scif_afe_wm8213_read
#define wm8213_write         scif_afe_wm8213_write
#define wm8213_setup         scif_afe_wm8213_setup
#else
#define wm8213_read          sccp_wm8213_read
#define wm8213_write         sccp_wm8213_write
#define wm8213_setup(...)    sccp_wm8213_setup()
#endif


#ifdef HAVE_DBG_CALLBACK_CMDS
#define           WM8213_REG_BANK      0x2C
static int        afe_debug_callback(int argc, char *argv[]);
static uint8_t    afe_debug_regs[WM8213_REG_BANK];
static bool       afe_write_dac_enable = true;
static bool       afe_write_pga_enable = true;
#endif


/* davep 09-Jul-2012 ; as of this writing, we have two sccp blocks. This is the
 * block we're using for the AFE. But we change our minds often. We're flighty. 
 */
//#define SCAN_AFE_SCCP_NUM 0
#define SCAN_AFE_SCCP_NUM 1


static volatile SCCP_REG_REGS_t * const sccp_regs_list[] = {
    (volatile SCCP_REG_REGS_t*)(APB_SCCP0_BASE),
    (volatile SCCP_REG_REGS_t*)(APB_SCCP1_BASE)
};


#define SCCP_IPTR_START 0x24
#define SCCP_WM8213_CMD_START_WRITE (1<<31)
#define SCCP_WM8213_CMD_START_READ  ((1<<31) | (1<<0))

/* wait this many for the write to successfully complete */
#define SCCP_WRITE_COMPLETE_WAIT  10
#define SCCP_READ_COMPLETE_WAIT  10
#define SCCP_INT_CMD_SUCCESS 1
#define SCCP_INT_BAD_CMD 2


struct sccp_wm8213_regs {
    volatile uint32_t start;

    /* davep 10-Dec-2012 ; update to SCCP microcode to handle multiple AFEs */
    volatile uint32_t afe_num;

    volatile uint32_t regnum;
    volatile uint32_t data;
};


static void sccp_wm8213_dump( void )
{
    volatile SCCP_REG_REGS_t *sccp_regs = sccp_regs_list[SCAN_AFE_SCCP_NUM];
    bool debug_enable = false;
    
    if (debug_enable)
    {
       dbg1("%8s=0x%08x %8s=0x%08x %8s=0x%08x %8s=0x%08x\n", 
              "IPTR", sccp_regs->IPTR,
              "CINST", sccp_regs->CINST,
              "INT", sccp_regs->INT,
              "CPU_CTRL", sccp_regs->CPU_CTRL);
    }
}


#if 0
static scan_err_t sccp_wm8213_read_or_write(unsigned int reg, unsigned int *data, uint32_t op )
{
    scan_err_t final_scerr;
    int wait_counter;
    volatile SCCP_REG_REGS_t *sccp_regs = sccp_regs_list[SCAN_AFE_SCCP_NUM];
    volatile struct sccp_wm8213_regs *sccp_afe_regs;

    dbg2( "%s regnum=0x%x\n", __FUNCTION__, reg );

    /* blindly clear all the pending SCCP interrupts */
    sccp_regs->INT = 0;

    /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

    /* The afe read/write register space is at the top of the SCCP SRAM */
    sccp_afe_regs = (struct sccp_wm8213_regs *)&sccp_regs->SRAM;
    sccp_afe_regs->regnum = reg;

    /* start */
//    sccp_afe_regs->start = SCCP_WM8213_CMD_START_READ;
    sccp_afe_regs->start = op;
    final_scerr = SCANERR_NONE;

    /* if we don't succeed, we run the risk of failure */
    wait_counter = 0;
    while( 1 ) {
        scos_sleep_milliseconds(10);

        /* poll for interrupt pending */
        if( sccp_regs->INT & SCCP_INT_CMD_SUCCESS ) {
            /* success! */
            break;
        }

        /* treat as failure any pending interrupts that aren't explicitly success */
        if( sccp_regs->INT & ~SCCP_INT_CMD_SUCCESS ) {
            /* failure. bummer! */
            dbg2( "%s error on afe read INT=0x%x\n", sccp_regs->INT );
            final_scerr = SCANERR_HARDWARE_FAIL;
            break;
        }

        if( wait_counter==10 ) {
            dbg2( "%s timeout waiting for sccp afe read\n", __FUNCTION__ );
            final_scerr = SCANERR_HARDWARE_FAIL;
            break;
        }

        dbg2( "%s waiting count=%d of %d\n", __FUNCTION__, 
                    wait_counter, SCCP_READ_COMPLETE_WAIT );
        wait_counter++;
    }

    /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

    /* blindly ack the interrupt */
    sccp_regs->INT = 0;
    if( op==SCCP_WM8213_CMD_START_READ ) {
        *data = sccp_regs->GEN7;
    }

    return final_scerr;
}
#endif


static unsigned int sccp_wm8213_read(uint8_t afe_num, unsigned int reg)
{
    scan_err_t final_scerr;
    int wait_counter;
    volatile SCCP_REG_REGS_t *sccp_regs = sccp_regs_list[SCAN_AFE_SCCP_NUM];
    volatile struct sccp_wm8213_regs *sccp_afe_regs;

//    /* davep 12-Jul-2012 ; don't try to read */
//    return 0;

    afe_dbg2( "%s afe_num=%d regnum=0x%x\n", __FUNCTION__, afe_num, reg );

    /* currently only two AFEs */
    XASSERT( afe_num==0 || afe_num==1, afe_num );

    /* blindly clear all the pending SCCP interrupts */
    sccp_regs->INT = 0;

    /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

    /* The afe read/write register space is at the top of the SCCP SRAM */
    sccp_afe_regs = (struct sccp_wm8213_regs *)&sccp_regs->SRAM;
    sccp_afe_regs->afe_num = afe_num;
    sccp_afe_regs->regnum = reg;
    
    /* start */
    sccp_afe_regs->start = SCCP_WM8213_CMD_START_READ;
    final_scerr = SCANERR_NONE;

   /* if we don't succeed, we run the risk of failure */
    wait_counter = 0;
    scos_sleep_microseconds(10);
    while( 1 ) {
//        scos_sleep_milliseconds(10);//mod.kmg 2013.08.30
        /* poll for interrupt pending */
        if( sccp_regs->INT & SCCP_INT_CMD_SUCCESS ) {
            /* success! */
            break;
        }
        scos_sleep_milliseconds(5);
       /* treat as failure any pending interrupts that aren't explicitly success */
        if( sccp_regs->INT & ~SCCP_INT_CMD_SUCCESS ) {
            /* failure. bummer! */
            dbg2( "%s error on afe read INT=0x%x\n", sccp_regs->INT );
            final_scerr = SCANERR_HARDWARE_FAIL;
            break;
        }

       if( wait_counter==10 ) {
            dbg1( "%s timeout waiting for sccp afe read\n", __FUNCTION__ );
            final_scerr = SCANERR_HARDWARE_FAIL;
            break;
        }

       dbg2( "%s waiting count=%d of %d\n", __FUNCTION__, 
                    wait_counter, SCCP_READ_COMPLETE_WAIT );
        wait_counter++;
    }

   /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

   /* blindly ack the interrupt */
    sccp_regs->INT = 0;
    return sccp_afe_regs->data;
//    return sccp_regs->GEN7;
}


static scan_err_t sccp_wm8213_write( uint8_t afe_num, unsigned int reg, unsigned int data)
{
    scan_err_t final_scerr;
    int wait_counter;
    volatile SCCP_REG_REGS_t *sccp_regs = sccp_regs_list[SCAN_AFE_SCCP_NUM];
    volatile struct sccp_wm8213_regs *sccp_afe_regs;
#ifdef HAVE_DBG_CALLBACK_CMDS
    unsigned int  index;
#endif

//    /* davep 12-Jul-2012 ; disable afe writes, use BrianM's script */
//    return SCANERR_NONE;

    /* blindly clear all the pending SCCP interrupts */
    sccp_regs->INT = 0;

    /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

    /* The afe read/write register space is at the top of the SCCP SRAM */
    sccp_afe_regs = (struct sccp_wm8213_regs *)&sccp_regs->SRAM;
    sccp_afe_regs->afe_num = afe_num;
    sccp_afe_regs->regnum = reg;
    sccp_afe_regs->data = data;

    /* start */
    sccp_afe_regs->start = SCCP_WM8213_CMD_START_WRITE;
    final_scerr = SCANERR_NONE;

    /* if we don't succeed, we run the risk of failure */
    wait_counter = 0;
    scos_sleep_microseconds(10);
    while( 1 ) {
//        scos_sleep_milliseconds(10);//mod.kmg 2013.08.30
        /* poll for interrupt pending */
        if( sccp_regs->INT & SCCP_INT_CMD_SUCCESS ) {
            /* success! */
            break;
        }
        scos_sleep_milliseconds(5);
        /* treat as failure any pending interrupts that aren't explicitly success */
        if( sccp_regs->INT & ~SCCP_INT_CMD_SUCCESS ) {
            /* failure. bummer! */
            dbg2( "%s error on afe write INT=0x%x\n", sccp_regs->INT );
            final_scerr = SCANERR_HARDWARE_FAIL;
			scanlib_send_sc(SMSG_SC_AFE_SETTING_FAIL);             
            break;
        }

        if( wait_counter==SCCP_WRITE_COMPLETE_WAIT ) {
            dbg1( "%s timeout waiting for sccp afe write\n", __FUNCTION__ );
//            final_scerr = SCANERR_HARDWARE_FAIL;
//            break;
        }

//debug only
//        dbg2( "%s waiting count=%d of %d\n", __FUNCTION__, 
//                    wait_counter, SCCP_WRITE_COMPLETE_WAIT );
        wait_counter++;
    }

    /* davep 05-Jul-2012 ; XXX temp debug */
    sccp_wm8213_dump();

    /* blindly ack the interrupt */
    sccp_regs->INT = 0;

    /* davep 10-Jul-2012 ; XXX temp debug ; read it back */
    if( final_scerr==SCANERR_NONE && reg != WM8213_REG_SWRESET)  {
        uint32_t num32;
        num32 = wm8213_read( afe_num, reg );
        afe_dbg2( "%s readback reg=%d val=0x%x\n", __FUNCTION__, reg, num32 );
    }

#ifdef HAVE_DBG_CALLBACK_CMDS
    for (index=0; index<WM8213_REG_BANK; index++)
    {
        if (index == reg)
        {
            if (reg == WM8213_REG_DAC_RGB)
            {
                afe_debug_regs[WM8213_REG_DAC_RGB] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_DAC_RED] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_DAC_GRN] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_DAC_BLU] = (uint8_t)data;
            }
            else if (reg == WM8213_REG_PGA_GAIN_MSB_RGB)
            {
                afe_debug_regs[WM8213_REG_PGA_GAIN_MSB_RGB] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_PGA_GAIN_MSB_RED] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_PGA_GAIN_MSB_GRN] = (uint8_t)data;
                afe_debug_regs[WM8213_REG_PGA_GAIN_MSB_BLU] = (uint8_t)data;
            }
            else
            {
                afe_debug_regs[index] = (uint8_t)data;
            }
            break;
        }
    }
#endif

    return final_scerr;
}


static void sccp_wm8213_setup( void )
{
    volatile SCCP_REG_REGS_t *sccp_regs = sccp_regs_list[SCAN_AFE_SCCP_NUM];

    /* set the starting instruction pointer */
    sccp_regs->IPTR = SCCP_REG_IPTR_INSTRPTR_REPLACE_VAL(sccp_regs->IPTR,
                            SCCP_IPTR_START );

    /* poke the CPU control with magic number from Brian Mahaffey that I need
     * to eventually decode into proper control bits
     */
    sccp_regs->CPU_CTRL = 0x5;
}

/**
 * \brief  
 *
 * \author David Poole
 * \date 28-Jan-2013
 */

static scan_err_t single_afe_soft_setup( uint8_t afe_num ) 
{
    scan_err_t scerr;
    uint32_t num32;

#ifdef HAVE_PINETREE_FORM_FACTOR
    // make sure the write is good since it's after a soft reset of the afe
    do
    {
        scerr = wm8213_write(afe_num,WM8213_REG_SETUP1, 0x31);
        if (scerr != SCANERR_NONE)
            return scerr;
        num32 = wm8213_read(afe_num, WM8213_REG_SETUP1);
        if (num32 != 0x31)
            dbg1("ERROR!!! WE DIDN'T GET 0x31 BACK FROM AFE SETUP REG 1\n");
    } while (num32 != 0x31);

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP2, 0x32);
    if( scerr != SCANERR_NONE ) return scerr;

//    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x07));
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x03));//tes.kmg
//    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x00));//tes.kmg
    
    if( scerr != SCANERR_NONE ) return scerr;
#else
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP1, 0x81);
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP2, WM8213_REG_SETUP2_OPFORM_4bit );
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x06));
    if( scerr != SCANERR_NONE ) return scerr;
#endif

#if 0
#ifdef HAVE_PINETREE_FORM_FACTOR
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP1, 0x31);
#else
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP1, 0x81);
#endif
    if( scerr != SCANERR_NONE ) return scerr;

#ifdef HAVE_PINETREE_FORM_FACTOR
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP2, 0x32);
#else
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP2, WM8213_REG_SETUP2_OPFORM_4bit );
#endif
    if( scerr != SCANERR_NONE ) return scerr;

#ifdef HAVE_PINETREE_FORM_FACTOR
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x07));
#else
    scerr = wm8213_write(afe_num,WM8213_REG_SETUP3, WM8213_REG_SETUP3_RLCDAC(0x06));
#endif
    if( scerr != SCANERR_NONE ) return scerr;
#endif

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP4, 0x00);
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP5, 0x00);
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write(afe_num,WM8213_REG_SETUP6, 
                            WM8213_REG_SETUP6_VSMPDET | 
                            WM8213_REG_SETUP6_VDEL(2) | 
                            WM8213_REG_SETUP6_POSNEG_POS );
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write( afe_num,WM8213_REG_DAC_RGB, 0x80 );
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write( afe_num,WM8213_REG_PGA_GAIN_MSB_RGB, 0x0c );
    if( scerr != SCANERR_NONE ) return scerr;

    scerr = wm8213_write( afe_num,WM8213_REG_PGA_GAIN_LSB_RGB, 0x00 );
    if( scerr != SCANERR_NONE ) return scerr;

    return SCANERR_NONE;
}


/**
 * \brief Reset the internal AFE 
 *
 * This is not resetting to power up values so we don't have to run ADC
 * calibration again.
 * This function may be called multiple times by the scan system.  For
 * setup that needs to occur only at first time powerup, see the function
 * afe_onetime_init().
 *
 * \retval scan_err_t any scan error that occured (if all is well, SCANERR_NONE)
 *
 * \remarks This is a good place to call the scanplat function to determine
 * which AFE is defined, and assert if the AFE is not the expected type.
 *
 **/

static scan_err_t single_afe_reset( int8_t afe_num )
{
    scan_err_t scerr;

    scerr = wm8213_write(afe_num,WM8213_REG_SWRESET, 0x00);
    if( scerr != SCANERR_NONE ) return scerr;

//    scos_sleep_milliseconds(100); //mod.kmg 2013.08.30

    return SCANERR_NONE;
}

scan_err_t afe_reset( void )
{
    scan_err_t scerr;
    uint8_t afe_num;


#if PINETREE_L

#if 1 //to avoid the error of setting AFE
    scerr = single_afe_reset(0);

    /* davep 28-Jan-2013 ; XXX temp disable 2nd AFE */
//    wm8213_write(1,WM8213_REG_SETUP1,0);

    single_afe_soft_setup(0);
#endif    

#else    
    for( afe_num=0 ; afe_num<2 ; afe_num++ ) {
        scerr = single_afe_reset( afe_num );
        if( scerr != SCANERR_NONE ) {
            return scerr;
        }
    }

    /* davep 28-Jan-2013 ; XXX temp disable 2nd AFE */
//    wm8213_write(1,WM8213_REG_SETUP1,0);

    single_afe_soft_setup(0);
    single_afe_soft_setup(1);
#endif    

    return SCANERR_NONE;
}

/**
 * \brief  Read/write/read an AFE register. Verify the result.
 *
 * \author David Poole
 * \date 28-Jan-2013
 */

static scan_err_t test_read_write( uint8_t afe_num, unsigned int reg, unsigned int write_value)  
{
    scan_err_t scerr;
    unsigned int value;

    value = wm8213_read( afe_num, reg );
    dbg2( "%s reg=0x%x value=0x%x\n", __FUNCTION__, reg, value );

    scerr = wm8213_write( afe_num, reg, write_value );
    if( scerr != SCANERR_NONE ) {
        dbg2( "%s write failed scerr=%d\n", __FUNCTION__, scerr );
        return scerr;
    }

    value = wm8213_read( afe_num, reg );
    dbg2( "%s afe_num=%d reg=0x%x value=0x%x\n", __FUNCTION__, afe_num, reg, value );

    /* The (uint8_t) because register values are actually 8-bit even though the
     * function interface is 'unsigned int'
     */
    if( (uint8_t)value != (uint8_t)write_value ) {
        dbg1( "%s ERROR! afe register readback afe_num=%d regnum=%d failed; value=0x%x expected=0x%x\n", 
                __FUNCTION__, afe_num, reg, value, write_value );
        return SCANERR_GENERIC_FAIL;
    }

    return SCANERR_NONE;
}

/**
 * \brief  Run simple read/write tests on the AFE.
 *
 * \author David Poole
 * \date 25-Jan-2013
 */

scan_err_t afe_test( void )
{
    scan_err_t scerr, final_scerr;
    uint8_t afe_num;
    unsigned int reg;
    unsigned int write_value;
    int list_idx;
    unsigned int reg_to_test_list[] = { 
        /* the gain/offset registers are pretty harmless to read/write */
        WM8213_REG_DAC_RED, WM8213_REG_DAC_GRN, WM8213_REG_DAC_BLU, 
        /* read/write the MSB which has 8 live bits (LSB has only 1 bit) */
        WM8213_REG_PGA_GAIN_MSB_RED, WM8213_REG_PGA_GAIN_MSB_GRN, WM8213_REG_PGA_GAIN_MSB_BLU, 
        0, /* end of list */
    };

    /* read/write safe registers (registers that won't break functionality) */
    list_idx = 0;
    reg = reg_to_test_list[list_idx];
    write_value = ~0;
    while( reg != 0 ) {
        for( afe_num=0 ; afe_num<2 ; afe_num++ ) {

            /* write different value to each AFE (to verify independence) */
            write_value = 0xf0 | afe_num;

            scerr = test_read_write( afe_num, reg, write_value );
            if( scerr != SCANERR_NONE ) {
                final_scerr = scerr;
                goto leave;
            }
        }
        list_idx += 1;
        reg = reg_to_test_list[list_idx];
    }

    afe_dump();

leave:
    afe_reset();
    return SCANERR_NONE;
}

/**
 * \brief Dump all AFE register values
 *
 * Read the value of each AFE register, format it, and output it,
 * often to a debug window.  But it could be used to dump any AFE
 * information to any useful location.
 *
 **/
void afe_dump(void)
{
    uint8_t afe_num;
    uint32_t     reg;
    uint32_t     data;

    sccp_wm8213_dump();

    for( afe_num=0 ; afe_num<2 ; afe_num++ ) {
        for (reg=1; reg<=8; reg++) {
            data = wm8213_read(afe_num,reg);
            dbg1("afe_num=%d reg 0x%08lX=0x%08lX\n", afe_num, reg, data);
        }
        for (reg=0x20; reg<=0x2b; reg++) {
            data = wm8213_read(afe_num,reg);
            dbg1("afe_num=%d reg 0x%08lX=0x%08lX\n", afe_num, reg, data);
        }
    }
}


void afe_get_hardware_config( struct afe_hardware_config_t *afe_hw )
{
    memcpy( afe_hw, &internal_mas_afe_config, sizeof(struct afe_hardware_config_t) );
}


/**
 * \brief Read the requested AFE register
 *
 * Read the value of the requested AFE register.
 *
 * \param[in] reg the register to read
 *
 * \retval unsigned int the value read from the requested AFE register
 *
 *
 **/
//unsigned int afe_read( uint32_t reg )
//{
//    return (wm8213_read(reg));
//}


void afe_write_pga(uint32_t afe_num, uint8_t red_gain, uint8_t green_gain, uint8_t blue_gain)
{
#ifdef HAVE_DBG_CALLBACK_CMDS
    if (afe_write_pga_enable)
#endif        
    {
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_LSB_RED, 0x00);
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_LSB_GRN, 0x00);
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_LSB_BLU, 0x00);
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_MSB_RED, red_gain);
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_MSB_GRN, green_gain);
        wm8213_write(afe_num,WM8213_REG_PGA_GAIN_MSB_BLU, blue_gain);    
    }
}


void afe_write_dac(uint32_t afe_num, uint8_t red_dac, uint8_t green_dac, uint8_t blue_dac)
{
#ifdef HAVE_DBG_CALLBACK_CMDS
    if (afe_write_dac_enable)
#endif        
    {
        wm8213_write(afe_num,WM8213_REG_DAC_RED, red_dac);
        wm8213_write(afe_num,WM8213_REG_DAC_GRN, green_dac);
        wm8213_write(afe_num,WM8213_REG_DAC_BLU, blue_dac);
    }
}


/**
 * \brief Write the value of the requested AFE register.
 *
 * \param[in] reg the index of the register to write
 * \param[in] data the value to write to the register 
 *
 * \remark If the ASIC's AFE registers are used to access the AFE, the
 * ASIC's interface function scif_afe_write() is used.
 *
 * \remark Marvell recommends checking that the register to be written
 * is actually a valid register before attempting to write.
 *
 **/

//void afe_write( uint32_t reg, uint32_t data)
//{
//    scan_err_t scerr;
//    scerr = wm8213_write(0,reg, data);
//}


/**
 * \brief Power up initialization of the AFE 
 *
 * Initializes the AFE and any associated internal structures, variables, etc.
 * This function will be called once at system startup.  For a function
 * that can be called multiple times to reset the AFE registers, see
 * afe_reset().
 *
 * \retval scan_err_t any scan error that occured (if all is well, SCANERR_NONE)
 **/

scan_err_t afe_onetime_init(void)
{
    scan_err_t scerr;
    SCIF_AFE_CONF afetemp;
#ifdef HAVE_DBG_CALLBACK_CMDS
    int retcode;
#endif    
    dbg1("%s\n", __FUNCTION__);

#ifdef HAVE_DBG_CALLBACK_CMDS
    afe_debug_regs[0x00] = 0xE1; //write only register
    afe_debug_regs[0x01] = 0x03;
    afe_debug_regs[0x02] = 0x20;
    afe_debug_regs[0x03] = 0x1F;
    afe_debug_regs[0x04] = 0xE2; //write only register
    afe_debug_regs[0x05] = 0xE3; //write only register
    afe_debug_regs[0x06] = 0x00;
    afe_debug_regs[0x07] = 0x00;
    afe_debug_regs[0x08] = 0x20;
    afe_debug_regs[0x09] = 0xE2; //reserved
    afe_debug_regs[0x0A] = 0xE2; //reserved
    afe_debug_regs[0x0B] = 0xE2; //reserved
    afe_debug_regs[0x0C] = 0xE2; //reserved
    afe_debug_regs[0x0D] = 0xE3; //blank
    afe_debug_regs[0x0E] = 0xE3; //blank
    afe_debug_regs[0x0F] = 0xE3; //blank
    afe_debug_regs[0x10] = 0xE3; //blank
    afe_debug_regs[0x11] = 0xE3; //blank
    afe_debug_regs[0x12] = 0xE3; //blank
    afe_debug_regs[0x13] = 0xE3; //blank
    afe_debug_regs[0x14] = 0xE3; //blank
    afe_debug_regs[0x15] = 0xE3; //blank
    afe_debug_regs[0x16] = 0xE3; //blank
    afe_debug_regs[0x17] = 0xE3; //blank
    afe_debug_regs[0x18] = 0xE3; //blank
    afe_debug_regs[0x19] = 0xE3; //blank
    afe_debug_regs[0x1A] = 0xE3; //blank
    afe_debug_regs[0x1B] = 0xE3; //blank
    afe_debug_regs[0x1C] = 0xE3; //blank
    afe_debug_regs[0x1D] = 0xE3; //blank
    afe_debug_regs[0x1E] = 0xE3; //blank
    afe_debug_regs[0x1F] = 0xE3; //blank
    afe_debug_regs[0x20] = 0x80;
    afe_debug_regs[0x21] = 0x80;
    afe_debug_regs[0x22] = 0x80;
    afe_debug_regs[0x23] = 0x80;
    afe_debug_regs[0x24] = 0x00;
    afe_debug_regs[0x25] = 0x00;
    afe_debug_regs[0x26] = 0x00;
    afe_debug_regs[0x27] = 0x00;
    afe_debug_regs[0x28] = 0x0C;
    afe_debug_regs[0x29] = 0x0C;
    afe_debug_regs[0x2A] = 0x0C;
    afe_debug_regs[0x2B] = 0x0C;
#endif

   //MRVL-Harris {
    //setup for WM8213
    afetemp.SENINIT = AFE_LOW;
    afetemp.SENCTRL = AFE_PULSE;
    afetemp.SERDCFG = UNIDATA;
    afetemp.SCLKDIV = 32;
    afetemp.protocol = "ARAAAADDDDDDDD";
    wm8213_setup(afetemp);
    //MRVL-Harris }

    scerr = SCANERR_NONE;

    afe_reset();

    /* davep 07-Jul-2012 ; XXX temp code ; loop writing a harmless register so
     * we can see data on scope
     */
#if 0
    uint32_t num32;

   while( 1 ) {
//        scerr = wm8213_write(WM8213_REG_PGA_GAIN_MSB_RGB, 0xa5);
//        if( scerr != SCANERR_NONE ) return scerr;
        num32 = wm8213_read(WM8213_REG_SETUP1);
//        num32 = wm8213_read(WM8213_REG_PGA_GAIN_MSB_RGB );
        if( scerr != SCANERR_NONE ) return scerr;
        dbg2( "%s num32=0x%x\n", __FUNCTION__, num32 );
    }
#endif

#ifdef HAVE_DBG_CALLBACK_CMDS
    retcode = cmd_register_cmd("afe", NULL, NULL, NULL, NULL, afe_debug_callback);
    XASSERT(retcode==0, retcode);
#endif    

    return scerr;
}


/**
 * \brief Turn AFE Auto Transfer Mode on or off
 *
 * Auto Transfer Mode is for CIS Color scans to switch the AFE's internal
 * Programmable Gain and Offset registers to match the illumination control at
 * the sensor (i.e. have the AFE apply the Red Offset/Gain pair when reading the
 * red scanline, the Green Offset/Gain pair when reading the green scanline, and the
 * Blue Offset/Gain pair when reading the blue scanline).  
 *
 * For the internal AFE (starting with ASIC2 rev Z1), this is automatically done
 * by the ASIC - so this function is empty.  
 *
 **/
void afe_auto_setup(void)
{
    //wm8213 bypasses this function
}


/**
 * \brief Enable or Disable AFE Auto Transfer Mode
 *
 * For the internal AFE (starting with ASIC2 rev Z1), this is automatically done
 * by the ASIC - so this function doesn't do anything for enable or disable.
 *
 **/
void afe_auto_enable(bool on)
{
    //wm8213 bypasses this function
}

/**
 * \brief  Convert a gain to an AFE PGA value for the 8213 AFE
 * Note that the gain parm that is passed in is already multiplied by 100 to eliminate
 * floating point
 */
uint8_t afe_gain_to_pga(uint32_t gain)
{
    int intgain;

    /* davep 24-May-2012 ; gain should be an integer representing float_gain * 100 */
#ifdef __KERNEL__
    // formula from the 8213 is A = 0.66+PGA*7.34/511.  (where A is the floating point gain value)
    // (our gain parm = 100*A)  So multiplying to eliminate floating
    // point and solving for PGA (since we need to send PGA to the AFE), we have:
    // PGA = (gain - 66) * 511 / 734.  But also note that this AFE has a 9 bit value for the
    // PGA.  8 bits MSB and 1 bit LSB.  So, we can just 0 out the LSB bit, right shift our PGA
    // value by 1 bit, and write that value into the 8 bits of MSB.  So we now have:
    // value_to_write_to_MSB = [(gain - 66) * 511 / 734] >> 1 
    gain = gain - 66;
    gain *= 511/2; // right shifting by 1 here - preprocessor creates a constant for us
    // so now gain = gain/734, but remember to round up first: ((int + n-1)/n
    gain = (gain + 734-1) / 734;
    // clamp gain if it got too big
    if (gain > 0xFF) 
        gain = 0xFF;
    intgain = (uint8_t) gain;
    return intgain;
#else
    float f_gain;
    float temp1;
    float temp2;
    if (gain < 1)
    {
        XASSERT(0, gain);
    }

    f_gain = gain/100.0;
    temp1 = 5.85f * 63.0f * (f_gain - 1.0f);
    temp2 = 4.85f * f_gain;
    f_gain = temp1 / temp2;
    f_gain = f_gain + 0.5;
    intgain = (uint8_t) f_gain;

    if (intgain > 63)
    {
        intgain = 63;
    }
#endif
    return intgain;
}

/**
 * hardware specific information about the WM8213 external AFE
 * all comments based on the Wolfson 8213 spec
 */

/* afe_full_scale_input_range - Vfs on page 7 is 1.2V since LOWREFS=1 in WM8213_REG_SETUP2
 * Vfs would have been  2.0V if LOWREFS=0.
 */
/* dac_unit_gain_output_change_step - this is also known as cugs -  change per unit gain.
 * This is calculated from the Offset DAC formula in Figure 16
 *   260mV*(DAC - 127.5)/127.5
 * Assume a unit gain (gain = 1), so multiplying by 1 doesn't impact the formula.
 * Then notice the ADC BLOCK formula (V3 * 65535/Vfs) = D1
 * (D1 is the digital output value).  This makes the cugs formula:
 * D1 = .26*(1)/127.5 * 65535/1.2 = 111 (rounding down)
 */
/* cal_analog_offset_zero - Looking at the table on page 7 has "Offset DAC, Monotonicity Guaranteed".
 * The entry for "Output voltage" shows DAC value 0x0 is -260 mV, and 0xFF is +260mV.
 * This implies that 0x7F or 0x80 is the 0 value.  Seeing that the default value on Table 8
 * for all offset DACs is 0x80, that implies that 0x80 is the correct 0 value
 * IN short, if you program 0x80 into the DAC, you should have 0 offset
 */
/* cal_analog_offset_max - The maximum value for the DAC is 0xFF - giving +260 mV (same comment as
   cal_analog_offset_zero
*/
/* cal_analog_offset_min - The minimum value for the DAC is 0x00 - giving -260 mv (same comment as
   cal_analog_offset_zero
*/
/* analog_cal_default_gain - when computing the gain (now that offset is calculated), we assume we
   want to start higher than 1.0 to give more signal to play with.
 *  
 */
/* gain_value_for_offset_cal - when we calibrate offset, based on the formula above, we must have unity gain
   (gain = 1.0 - see comment above "Assume a unit gain")
 */
static struct scan_afe_info_t wm8213_info = {
    .afe_full_scale_input_range = 1.2,
    .dac_unit_gain_output_change_step = 111,
    .cal_analog_offset_zero = 0x80,
    .cal_analog_offset_max = 0xFF,
    .version = AFE_INFO_VERSION_2,
    .analog_cal_default_gain_not_null = true,
    .analog_cal_default_gain = 1.3,
    .gain_value_for_offset_cal = 1.0,
    .cal_analog_offset_min = 0x00,
};


/**
 * \brief Setup the AFE for a scan
 * 
 * Whatever setup for the AFE before each scan
 *
 * \retval float  the top margin
 *
 */
scan_err_t afe_soft_setup( void )
{
    //wm8213 bypasses this function
    return SCANERR_NONE;
}


/**
 * \brief Get the AFE specific structure
 * 
 * Returns the AFE specific info structure to the caller
 * 
 *
 * \param[out] scan_afe_info  the afe_info structure
 *
 */
void afe_get_afe_info( struct scan_afe_info_t **scan_afe_info )
{
    *scan_afe_info = &wm8213_info; 
}


#ifdef HAVE_DBG_CALLBACK_CMDS
int afe_debug_callback(int argc, char *argv[])
{
    int          i, pos;
    uint32_t     temp;
    uint32_t     reg;
    uint32_t     data;
    uint32_t     index;

    for (i=0; i<argc; i++)
    {
        dbg1("%s %d %s\n", __FUNCTION__, i, argv[i]);
    }

    if ( argc < 2 ) {
        return -1; 
    }
    pos = 1;

    if (strncmp(argv[pos], "dacwrite", 9) == 0)
    {
        if(argc != 3)
        {
            dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
            return 0;
        }

        str_mkint(argv[pos+1], &temp);

        if (temp != 0)
        {
            afe_write_dac_enable = true;
        }
        else
        {
            afe_write_dac_enable = false;
        }
    }
    else
    if (strncmp(argv[pos], "pgawrite", 9) == 0)
    {
        if(argc != 3)
        {
            dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
            return 0;
        }

        str_mkint(argv[pos+1], &temp);

        if (temp != 0)
        {
            afe_write_pga_enable = true;
        }
        else
        {
            afe_write_pga_enable = false;
        }
    }
    else
    if (strncmp(argv[pos], "write", 6) == 0)
    {
        if(argc != 4)
        {
            dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
            return 0;
        }

        //get addr
        str_mkint(argv[pos+1], &temp);
        reg = temp;

        //get data
        str_mkint(argv[pos+2], &temp);
        data = temp;

        dbg1("AFE write: addr=0x%08lX, data=0x%08lX\n", reg, data);
        wm8213_write(0,reg, data);
            
    }
    else if (strncmp(argv[pos], "read", 5) == 0)
    {
        if(argc != 3)
        {
            dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
            return 0;
        }
    
        //get reg
        str_mkint(argv[pos+1], &temp);
        reg = temp;

        //get data
        //data = wm8213_read(reg);
        data = afe_debug_regs[reg];
        dbg1("AFE read: addr=0x%08lX, data=0x%08lX\n", reg, data);
    }
    else if (strncmp(argv[pos], "dump", 5) == 0)
    {
        if(argc != 2)
        {
            dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
            return 0;
        }

        #if 0
        reg = 0x01; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x02; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x03; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x06; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x07; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x08; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x20; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x21; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x22; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x23; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x28; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x29; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x2A; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        reg = 0x2B; data = wm8213_read(reg); dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
        #else
        for (index=0; index<WM8213_REG_BANK; index++)
        {
            if ((index != 0x00) && (index != 0x04) && (index != 0x05) && 
                (!((index >= 0x09) && (index <= 0x1F)))
               )
            {
                reg = index;
                data = afe_debug_regs[index];
                dbg1("AFE dump: addr=0x%08lX, data=0x%08lX\n", reg, data);
            }
        }
        #endif
    }
    else
    {
        dbg1("error@%s@%s.%d\n", __FUNCTION__, __FILE__, __LINE__);
    }
    return 0;
}
#endif
