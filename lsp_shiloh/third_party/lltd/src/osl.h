/*
 * LICENSE NOTICE.
 *
 * Use of the Microsoft Windows Rally Development Kit is covered under
 * the Microsoft Windows Rally Development Kit License Agreement,
 * which is provided within the Microsoft Windows Rally Development
 * Kit or at http://www.microsoft.com/whdc/rally/rallykit.mspx. If you
 * want a license from Microsoft to use the software in the Microsoft
 * Windows Rally Development Kit, you must (1) complete the designated
 * "licensee" information in the Windows Rally Development Kit License
 * Agreement, and (2) sign and return the Agreement AS IS to Microsoft
 * at the address provided in the Agreement.
 */

/*
 * Copyright (c) Microsoft Corporation 2005.  All rights reserved.
 * This software is provided with NO WARRANTY.
 */

#ifndef OSL_H
#define OSL_H

/* Operating-System Layer.
 * Hides details of underlying OS details beneath this consistent API.
 */

#include <semaphore.h>
#include "posix_ostools.h"

#include "tlv.h"
#include "util.h"

#include "os_network.h"
#include "net_logger.h"
#include "net_iface_api.h"

#define NETLOG_SUBMOD_ID_LLTD   21 /* TODO: move to <net_logger.h>? */
#define NET_LOGGER_SUBMOD_LLTD  LOGGER_SUBMODULE_BIT(NETLOG_SUBMOD_ID_LLTD)

typedef enum
{
    INTF_INVALID    = -1,
    INTF_WIRED      = 0,
    INTF_WIRELESS   = 1,
} INTF_TYPE;

typedef struct
{
    char *name;
    INTF_TYPE type;
} supported_app_interface_t;

extern sem_t osl_listening_sem;

#define MAX_APPIFS 8
#define IFNAME_MAXLEN 16

#define MAX_NUM_SESSIONS 11
#define RXBUFSZ 2048
#define TXBUFSZ 2048

typedef struct
{
    pthread_mutex_t lock;
    pthread_mutex_t det_lock;
} osl_icon_t;

typedef struct
{
    char *name;
    etheraddr_t	hw_addr;

    pthread_t thread_id;
    pthread_mutex_t mutex;
    pthread_cond_t cond;    

    /* LLTD internal interface index */
    int index;

    /* 0-based marvell platform interface index */
    int app_index;
    /* listener socket file descriptor */ 
    int	listener_sock_fd;
    /* wired/wireless type descriptor */
    INTF_TYPE type;

    smE_state_t             smE_state;
    smT_state_t             smT_state;

    /* input to state machines, to drive transitions */
    protocol_event_t        this_event;
    /* sessions started by Discover msgs (either quick or topo) */
    session_t               sessions[MAX_NUM_SESSIONS];
    /* the unique session that can do emits, etc... */
    session_t               *topo_session;
    /* BAND algorthm's state */
    band_t		            band;

    /* network receive / transmit context -
     *  many of these are macro'd for brevity */

    /* fixed buffer we receive packets into */
    uint8_t		            rxbuf[RXBUFSZ];
    /* fixed buffer we transmit packets from */
    uint8_t		            txbuf[TXBUFSZ];
    /* alternate tx buffer for retransmission from */
    uint8_t		            re_txbuf[TXBUFSZ];
    /* how many bytes of rxbuf are valid */
    size_t		            rcvd_pkt_len;
    /* which sequence number rtxbuf holds (or 0) */
    uint16_t	            rtxseqnum;
    /* how many bytes of txbuf were sent, sequenced or not */
    size_t		            tx_len;
    /* how many bytes of rtxbuf are valid - sequenced, only */
    size_t		            re_tx_len;
    /* last sequenced-request opcode */
    uint8_t		            re_tx_opcode;

    /* Emit state */
    uint8_t		            emitbuf[RXBUFSZ];
    uint16_t	            emit_remaining;
    uint16_t	            emit_seqnum;
    topo_emitee_desc_t      *emitdesc;

    /* generation we've sent in Hello, or learnt */
    uint16_t	            generation;
    /* sequence number from base hdr in rxbuf */
    uint16_t	            sequencenum;
    /* opcode from base header in rxbuf, expanded to uint */
    uint		            opcode;

    /* Current Transmit Credit (CTC), and needs for this event's response */
    uint32_t	            ctc_packets;
    uint32_t	            ctc_bytes;
    uint32_t                totalPause;
    uint32_t                neededPackets;
    uint32_t                neededBytes;

    /* circular buffer of recvee_desc_t */
    topo_seeslist_t         *sees;

    /* timers; NULL if not running */
    event_t		            *block_timer;
    event_t		            *charge_timer;
    event_t		            *emit_timer;
    event_t		            *hello_timer;

    /* packet pointers and information, hoisted here for fast access */
    /* pointer to ethernet header in rxbuf */
    topo_ether_header_t     *ethernet_hdr;
    /* pointer to base header in rxbuf */
    topo_base_header_t      *base_hdr;
    /* pointer to discover header in rxbuf */
    topo_discover_header_t  *discover_hdr;
    /* pointer to hello header in rxbuf */
    topo_hello_header_t     *hello_hdr;
    /* pointer to query-large-tlv header in rxbuf */
    topo_qltlv_header_t     *qltlv_hdr;

    /* general use buffer */
    char                    gbuf[160];
} osl_interface_info_t;

struct osl_st
{
    /* number of responder interfaces */
    unsigned responder_if_num;
    /* table of responder interface info */
    osl_interface_info_t *responder_ifs;

    /* icons info */
    osl_icon_t icon_info;
};

/* Private state needed to handle the OS. */
typedef struct osl_st osl_t;

#define IF_NUM      (g_osl->responder_if_num)

/* get a specific info about an interface */
#define IF_NAME(i)      (g_osl->responder_ifs[i].name)
#define IF_ADDR(i)      (g_osl->responder_ifs[i].hw_addr)
#define IF_SOCK(i)      (g_osl->responder_ifs[i].listener_sock_fd)
#define IF_INDEX(i)     (g_osl->responder_ifs[i].app_index)
#define IF_TYPE(i)      (g_osl->responder_ifs[i].type)
#define IF_THREAD(i)    (g_osl->responder_ifs[i].thread_id)
#define IF_MUTEX(i)     (g_osl->responder_ifs[i].mutex)
#define IF_COND(i)      (g_osl->responder_ifs[i].cond)

/* get the info set of an interface */
#define IF_INFO(i)      ((osl_interface_info_t *) &(g_osl->responder_ifs[i]))

/* get the index of the current thread's responder interface */
#define RIF (osl_interface_id_get(pthread_self()))

/* Do any start-of-day initialistation, and return private state. */
extern osl_t *osl_init(void);

/* Do anything required to become a long-running background process. */
extern void osl_become_daemon(osl_t *osl);

/* Open "interface", and add packetio_recv_handler(state) as the IO
 * event handler for its packets (or die on failure).  If possible,
 * the OSL should try to set the OS to filter packets so just frames
 * with ethertype == topology protocol are received, but if not the
 * packetio_recv_handler will filter appropriately, so providing more
 * frames than necessary is safe. */
extern void osl_interface_open(int index, void *state);

/* pidfile maintenance: this is not locking (there's plenty of scope
 * for races here!) but it should stop most accidental runs of two
 * lltdd instances on the same interface.  We open the pidfile for
 * read: if it exists and the named pid is running we abort ourselves.
 * Otherwise we reopen the pidfile for write and log our pid to it. */
extern void osl_write_pidfile(osl_t *osl);

/* Permanently drop elevated privilleges, eg by using POSIX.1e
 * capabilities, or by setuid(nobody) */
extern void osl_drop_privs(osl_t *osl);

/* Turn promiscuous mode on or off. */
extern void osl_set_promisc(int ifndx, bool_t promisc);

/* Return the Ethernet address for the interface, or die. */
extern void osl_get_hwaddr(osl_t *osl, /*OUT*/ etheraddr_t *addr);


/* Usual read/write functions; here if they need wrapping */

/* Read from "fd" argument passed into the IO event handler. */
extern ssize_t osl_read(int fd, void *buf, size_t count);

/* Write to opened interface. */
extern ssize_t osl_write(osl_t *osl, const void *buf, size_t count);

/* Map an LLTD responder interface to an application-registered interface. */
extern int osl_interface_map(char *responder_if_name);

/* Determine the active responder interface for a thread */
extern int osl_interface_id_get(pthread_t thread_id);

/* Close a responder interface. */
extern void osl_interface_close(int index);

/* Prepare responder interfaces. */
extern void osl_interface_init(void);

/* Process interface state change notification. */
extern void osl_interface_handle_state_change(
                net_iface_event_flags_t events, void *context);

/* Register for notifications whenever a link changes state. */
extern void osl_interface_register_notification(char *rif_name);

#endif /* OSL_H */
