/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/*
 * scannet.c, linux kernel edition.
 *
 * Handles pipetap. Writes scanned data out TCP socket.
 *
 * davep 16-Oct-2012
 */

#include <stdint.h>
#include <stdbool.h>

#include "scos.h"

#include "utils.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/tcp.h>
#include <linux/in.h>
#include <linux/file.h>
#include <linux/socket.h>

#include "scantypes.h"
#include "scancore.h"
#include "scandbg.h"

/*
 * TODO handle multiple channels (r,g,b).
 *
 * TODO handle short writes in scan_debug_net_send(). Need to support
 *      sock_sendmsg() sending < full length.
 *
 *
 */

//struct sockaddr_in saddr, daddr;
//struct socket *control= NULL;

/* in case you're wondering, 050213 == 13-Feb-05 */
/* 24-May-05 davep ; changed to 50210 so the channel offsets start from zero */
#define SCAN_DEBUG_BASE_PORT 50210

/* must use command line "scan ip nn.nn.nn.nn" to set this value */
static uint32_t g_destip = -1;

/* 13-May-05 davep ; currently both PIC and PIE have a max of 6 DMA channels
 * each and we want to be able to use channel_sockets[] for both PIC and PIE.
 */
#define NUM_CHANNEL_SOCKS  6

/* Initialize all sockets to NULL to indicate a closed state. */
static struct socket * channel_sockets[NUM_CHANNEL_SOCKS] = { NULL,NULL,NULL,NULL,NULL,NULL };

char *ipstr( uint32_t s_addr, char buf[], int bufsize )
{
    /* TODO */
    return NULL;
}

int mkip( char *str, uint32_t *ipaddr )
{
    int i, len, num_fields, shift;
    char scratch[20];
    char *fields[10], *errptr;
    uint32_t num32;

    *ipaddr = 0;
    len = strnlen(str,16);
    if(len>15) {
//        dbg2( "%s too long len=%d\n", __FUNCTION__, len );
        return -1;
    }

    /* str_split() is destructive so use a copy (on the stack) */
    memset( scratch, 0, sizeof(scratch) );
    strncpy( scratch, str, 15 );

    memset( fields, 0, sizeof(fields));

    num_fields = str_split( scratch, len, fields, 5, "." );
    if( num_fields != 4 ) {
//        dbg2( "%s found fields=%d want fields=4\n", __FUNCTION__, num_fields );
        return -1;
    }

    shift = 24;
    for( i=0 ; i<4 ; i++ ) {
//        dbg2( "%s %s\n", __FUNCTION__, fields[i] );
        errptr = NULL;
        num32 = simple_strtol( fields[i], &errptr, 10 );
        if( *errptr ) {
//            dbg2( "%s invalid integer field=%s\n", __FUNCTION__, fields[i] );
            return -1;
        }
        if( num32 > 255 ) {
//            dbg2( "%s integer=%d too large\n", __FUNCTION__, num32 );
            return -1;
        }

        *ipaddr |= num32<<shift;
        shift -= 8;
    }

    /* success! */
    return 0;
}

void report_net_err( const char *msg, int sock, const char *fct )
{
    printk( "%s %s sock=%d fct=%p\n", __FUNCTION__, msg, sock, fct );
}

void scan_debug_net_close( void )
{
    int i;
    int retcode;
    struct socket *s;

    for( i=0 ; i<NUM_CHANNEL_SOCKS ; i++ ) {
        s = channel_sockets[i];
        if( s==NULL ) {
            dbg2( "%s socket idx=%d not open\n", __FUNCTION__, i );
            continue;
        }

        retcode = s->ops->shutdown( s, 0 );
        dbg2( "%s shutdown retcode=%d\n", __FUNCTION__, retcode );

        sock_release( channel_sockets[i] );
        channel_sockets[i] = NULL;
    }
}

void scan_debug_set_ip( uint32_t destip )
{
    g_destip = destip;
}

uint32_t scan_debug_get_ip( void )
{
    return g_destip;
}

void scan_debug_clear_ip( void )
{
    g_destip = -1;
}

int scan_debug_net_open( uint32_t destip, uint8_t channels[], int num_channels )
{
    int i;
    int retcode;
    struct sockaddr_in daddr;
    struct socket *s;
    int ch;

    /* http://www.linuxjournal.com/article/7660?page=0,2 */

    for( i=0 ; i<num_channels ; i++ ) {
        ch = channels[i];

        /* beware memory leaks */
        BUG_ON(channel_sockets[ch] != NULL );

        retcode = sock_create( PF_INET, SOCK_STREAM, IPPROTO_TCP, &channel_sockets[ch] );
        dbg2( "%s sock_create channel=%d retcode=%d sock=%p\n", __FUNCTION__, 
                ch, retcode, channel_sockets[ch] );
        
        if( retcode != 0 ) {
            dbg2( "%s failed to create socket retcode=%d\n", __FUNCTION__, retcode );
            scan_debug_net_close();
            return -1;
        }

        s = channel_sockets[ch];

        memset( &daddr, 0, sizeof(struct sockaddr) );
        daddr.sin_family = AF_INET;
        daddr.sin_port = htons( SCAN_DEBUG_BASE_PORT + ch );
        daddr.sin_addr.s_addr = htonl( g_destip );

        retcode = s->ops->connect( s, (struct sockaddr *)&daddr, 
                                    sizeof(struct sockaddr), O_RDWR );

        dbg2( "%s connect port=%d addr=0x%x retcode=%d\n", __FUNCTION__,
                htons(daddr.sin_port), htonl(daddr.sin_addr.s_addr), retcode );
    }

    return 0;
}

int scan_debug_net_send( uint8_t channel, uint8_t *ptr, uint32_t len )
{
    int retcode;
    struct socket *s;
    struct msghdr msg;
    struct iovec iov;
    mm_segment_t old_fs;

//    dbg2( "%s ch=%d ptr=%p len=%d\n", __FUNCTION__, channel, ptr, len );

    s = channel_sockets[channel];

//    dbg2( "%s sock=%p\n", __FUNCTION__, s );

    /* socket not open, ignore */
    if( !s ) {
        dbg2( "%s channel=%d not open\n", __FUNCTION__, channel );
        return -1;
    }

    memset( &msg, 0, sizeof(msg) );
    memset( &iov, 0, sizeof(struct iovec) );

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_iov->iov_len = len;
    msg.msg_iov->iov_base = ptr;

    old_fs = get_fs();
    set_fs( KERNEL_DS );

    retcode = sock_sendmsg( s, &msg, len );
//    dbg2( "%s sock_sendmsg retcode=%d\n", __FUNCTION__, retcode );

    set_fs( old_fs );

    /* davep 16-Oct-2012 ; I stupidly designed this function to return '0' on
     * success
     */
    return 0;
}

bool scan_debug_net_is_open( uint8_t channel )
{
    return channel_sockets[channel] != NULL;
}

