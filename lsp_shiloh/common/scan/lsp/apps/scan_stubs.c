/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 **/
#include <stdint.h>

#include "scos.h"

#include "lassert.h"
#include "memAPI.h"

#include "scantypes.h"
#include "scancore.h"
#include "scanmech.h"
#include "scanhw.h"
#include "scanif.h"
#include "scanplat.h"
#include "scancmdq.h"

//-jrsscan_err_t scanmem_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t cal_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t icetest_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t pic_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t pie_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t cisx_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t scanimg_onetime_init( void ) { return SCANERR_NONE; }
scan_err_t pipe_onetime_init( void ) { return SCANERR_NONE; }

scan_err_t scan_onetime_init( void ) 
{ 
    scan_err_t scerr;

    /* call scanplat to initialize our userspace mech driver */
    scanplat_init_mech();

    /* wait for mech driver ready */
    scerr = scan_cmdq_wait_for_ready();
    XASSERT( scerr==0, scerr );

    return SCANERR_NONE; 
}

void scan_cmdline_init( void ){}

void afe_dump( void ) {}
void scif_dump( void ) {}
void pic_dump( void ) {}

void scan_cmdq_mech_status_change(t_ScanMechStatus mech_status)
{
}

scan_err_t scan_cmdq_wait_for_ready(void)
{
    return SCANERR_NONE;
}

void registerScanACLCmds( void )
{
    /* scanman wants this */
}

void scif_motor_setup(MOTOR_SEL motor_data_enable, MOTOR_SYNC lsmode, 
                      MOTOR_TYPE m1type, MOTOR_TYPE m2type)
{
    ASSERT(0);
}

int pic_debug_msg( uint32_t param2, void *param3 )
{
    ASSERT(0);
    return 0;
}

void scan_debug_set_ip( uint32_t destip )
{
    ASSERT(0);
}

uint32_t memGetHeapFreeMemorySize(mlimiter_t *heap)
{
    /* used by copyapp */
    return ~0;
}

