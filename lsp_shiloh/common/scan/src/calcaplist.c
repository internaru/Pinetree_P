/* 
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */
#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"
#include "cal.h"
#include "safetylock.h"
#include "calcaplist.h"

#define CAL_CAP_LIST_MAX 32

static struct safety_lock lock;
static uint32_t cal_cap_list[CAL_CAP_LIST_MAX];
static int cal_cap_list_count;

static void cal_cap_list_lock_init( void )
{
    safety_lock_init( &lock );
}

static void cal_cap_list_lock( void )
{
    safety_lock_lock( &lock );
}

static void cal_cap_list_unlock( void )
{
    safety_lock_unlock( &lock );
}

scan_err_t cal_cap_list_add( uint32_t cal_cap_id )
{
    int i;

    dbg2( "%s id=%d\n", __FUNCTION__, cal_cap_id );

    cal_cap_list_lock();

    /* if already in the list, don't consume another slot */
    for( i=0 ; i<cal_cap_list_count ; i++ ) {
        if( cal_cap_list[i]==cal_cap_id ) {
            cal_cap_list_unlock();
            return SCANERR_NONE;
        }
    }

    /* any space left? */
    if( cal_cap_list_count >= CAL_CAP_LIST_MAX ) {
        cal_cap_list_unlock();
        return SCANERR_OUT_OF_MEMORY;
    }

    cal_cap_list[cal_cap_list_count++] = cal_cap_id;
    
    cal_cap_list_unlock();

    return SCANERR_NONE;
}

void cal_cap_list_flush( void )
{
    cal_cap_list_lock();
    cal_cap_list_count = 0;
    memset( cal_cap_list, 0, sizeof(cal_cap_list) );
    cal_cap_list_unlock();
}

bool cal_cap_list_contains( uint32_t cal_cap_id )
{
    int i;

    cal_cap_list_lock();

    for( i=0 ; i<cal_cap_list_count ; i++ ) {
        if( cal_cap_list[i]==cal_cap_id ) {
            cal_cap_list_unlock();
            dbg2( "%s id=%d true\n", __FUNCTION__, cal_cap_id );
            return true;
        }
    }

    cal_cap_list_unlock();

    dbg2( "%s id=%d false\n", __FUNCTION__, cal_cap_id );
    return false;
}

scan_err_t cal_cap_list_onetime_init( void )
{
    cal_cap_list_lock_init();

    cal_cap_list_flush();

    return SCANERR_NONE;
}

