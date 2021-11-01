/*
 * ============================================================================
 * Copyright (c) 2010 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file icetest_threadx.c
 *
 * \brief ICETest code to work portably (ha ha) across RTOS selections.
 *
 * ICETest has an interrupt. We must wait on the interrupt. In order to work
 * across RTOS differences, put the interrupt and OS specific bits in own file.
 *
 */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/completion.h>
#include <linux/interrupt.h>

#include "scos.h"

#include "lassert.h"

#include "scantypes.h"
#include "scancore.h"
#include "icetest.h"

/* forward decl */
static void icetest_tasklet( unsigned long arg );

static DECLARE_TASKLET(icetest_isr_tasklet,icetest_tasklet,0);
static DECLARE_COMPLETION(icetest_isr_complete);

static void icetest_tasklet( unsigned long arg )
{
    complete_all( &icetest_isr_complete );
}

void icetest_interrupt_rtos_wait( void )
{
    wait_for_completion( &icetest_isr_complete );

    /* reset to allow it to be re-used */
    INIT_COMPLETION( icetest_isr_complete );
}

void icetest_interrupt_rtos_signal( void )
{
    /* 
     * BIG FAT NOTE! This is called from interrupt context! 
     */
    tasklet_schedule( &icetest_isr_tasklet );
}

void icetest_rtos_init( void )
{
    INIT_COMPLETION( icetest_isr_complete );
}

