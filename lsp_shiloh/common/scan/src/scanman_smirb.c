/* 
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *                      
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * \file scanman_smirb.c
 *
 * \brief 
 *
 */

/* davep 23-Jul-2008 ; added -D__AGMESSAGE_H_ to the makefile to prevent
 * agMessage.h from being included in the scan code.  But we need agMessage.h in
 * this file.
 */
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "posix_ostools.h"
#include "scos.h"

#include "ATypes.h"
#include "lassert.h"
#include "agMessage.h"
#include "agRouter.h"

#include "scancore.h"
#include "scantypes.h"
#include "scandbg.h"
#include "scantask.h"
#include "scanman.h"
#include "smirb.h"

#define MAX_REQUEST_LIST 12
static struct scanman_ioctl_request_block request_list[MAX_REQUEST_LIST];

static pthread_mutex_t      list_lock;

static void request_list_lock( void )
{
    UINT pxretcode;

    pxretcode = pthread_mutex_lock( &list_lock ); 
    XASSERT(pxretcode == 0, pxretcode); 
}

static void request_list_unlock( void )
{
    UINT pxretcode;

    pxretcode = pthread_mutex_unlock( &list_lock ); 
    XASSERT(pxretcode == 0, pxretcode); 
}

static scan_err_t request_list_claim_entry( int *idx )
{
    int i;

    request_list_lock();
    for( i=0 ; i<MAX_REQUEST_LIST ; i++ ) {
        if( request_list[i].cookie==0 ) {
            /* clean it */
            memset( &request_list[i], 0, sizeof(struct scanman_ioctl_request_block) );

            /* claim it */
            request_list[i].cookie = SCANMAN_IOCTL_REQUEST_COOKIE;

            /* return index to this entry */
            *idx = i;

            request_list_unlock();
            return SCANERR_NONE;
        }
    }
    request_list_unlock();
    return SCANERR_NO_ENTRY;
}

static scan_err_t request_list_lookup_entry( uint32_t id, int *idx )
{
    int i;

    request_list_lock();
    *idx = -1;
    for( i=0 ; i<MAX_REQUEST_LIST ; i++ ) {
        if( request_list[i].id==id && request_list[i].cookie==SCANMAN_IOCTL_REQUEST_COOKIE) {
            *idx = i;
            request_list_unlock();
            return SCANERR_NONE;
        }
    }
    request_list_unlock();
    return SCANERR_NO_ENTRY;
}

static scan_err_t request_list_release_entry( uint32_t id )
{
    int i;

    request_list_lock();
    for( i=0 ; i<MAX_REQUEST_LIST ; i++ ) {
        if( request_list[i].id==id ) {
            /* clean it (also marks it free) */
            memset( &request_list[i], 0, sizeof(struct scanman_ioctl_request_block) );
            request_list_unlock();
            return SCANERR_NONE;
        }
    }
    request_list_unlock();
    return SCANERR_NO_ENTRY;
}

static uint32_t request_list_next_id( void )
{
    static uint32_t id;
    uint32_t next_id;

    request_list_lock();
    /* beware of rollover; 0 is a reserved value */
    if( (uint32_t)(id+1)==0 ) {
        id = 0;
    }
    next_id = ++id;
    request_list_unlock();

    return next_id;
}

void scanman_smirb_sanity( struct scanman_ioctl_request_block *smirb )
{
    scan_err_t scerr;
    int irb_idx;

    /* is this a valid smirb? */
    XASSERT( smirb->cookie==SCANMAN_IOCTL_REQUEST_COOKIE, (uint32_t)smirb );

    /* is this smirb from my own list of goodies? */
    scerr = request_list_lookup_entry( smirb->id, &irb_idx );
    XASSERT( scerr==SCANERR_NONE, smirb->id );
    XASSERT( request_list[irb_idx].id==smirb->id, smirb->id );
    XASSERT( &request_list[irb_idx]==smirb, (uint32_t)smirb );
}

void scanman_smirb_free( struct scanman_ioctl_request_block **smirb )
{
    request_list_release_entry( (*smirb)->id );
    *smirb = NULL;
}

scan_err_t scanman_smirb_alloc( struct scanman_ioctl_request_block **psmirb )
{
    scan_err_t scerr;
    uint32_t new_request_id;
    int idx = 0;

    new_request_id = request_list_next_id();

    scerr = request_list_claim_entry( &idx );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    request_list[idx].id = new_request_id;

    *psmirb = &request_list[idx];

    return SCANERR_NONE;
}

/**
 * 
 * \brief  
 *
 * Called from an outside thread to queue a smirb for Scanman.
 *
 *
 * \author David Poole
 * \date 27-Sep-2011
 *
 */

scan_err_t scanman_dev_request( uint32_t request, int arg, void *ptr, uint32_t *request_id )
{
    scan_err_t scerr;
    error_type_t err;
    MESSAGE msg;
    struct scanman_ioctl_request_block *smirb;

//    dbg2( "%s req=%d arg=%d ptr=%p\n", __FUNCTION__, request, arg, ptr );

    *request_id = 0;

    scerr = scanman_smirb_alloc( &smirb );
    if( scerr != SCANERR_NONE ) {
        return scerr;
    }

    scanman_smirb_sanity( smirb );

    /* we'll pass back this smirb's id */
    *request_id = smirb->id;

    /* fill the smirb with the requested parameters */
    smirb->request = request;
    smirb->arg = arg;
    smirb->ptr = ptr;

    /* send the smirb to the scanman thread for processing */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_DEV_REQUEST;
    msg.param3 = (void *)smirb;

    err = SYMsgSend( SCANMANID, &msg );
    if( err != OK ) {
        XASSERT( err==OK, err );

        scanman_smirb_free( &smirb );

        return SCANERR_MSG_FAIL;
    }

    dbg2( "%s req=%d arg=%d ptr=%p id=%d\n", __FUNCTION__, request, arg, ptr, *request_id);

    return SCANERR_NONE;
}

scan_err_t scanman_smirb_onetime_init( void )
{
    UINT pxretcode;

    /* Create lock  */
    pxretcode = posix_mutex_init( &list_lock );
    XASSERT( pxretcode==0, pxretcode );

    return SCANERR_NONE;
}

