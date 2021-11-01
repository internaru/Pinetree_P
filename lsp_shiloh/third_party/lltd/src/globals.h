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

/* This is both the definition and the declaration of all global variables */

#include <stdio.h>
#include <inttypes.h>   /* for uint8_t, uint16_t etc */
#include <sys/time.h>	/* for timeval */
#include <sys/types.h>  /* for size_t */
#include <arpa/inet.h>  /* for in6addr, htons(), and friends */
#include <limits.h>     /* for UINT_MAX */

//#include "net_logger.h"

#define CAN_FOPEN_IN_SELECT_LOOP 1
#define USING_UNAME

#ifdef __ARM_PIKA__
// std function memcpy() fails on earliest ARM architectures (as used in Pika reference platform),
// whenever the span is divisible by 4 and the source or destination address is not aligned to 4-bytes...
#define memcpy(pDest, pSrc, cnt) {int i; for (i=0;i<(int)cnt;i++) ((uint8_t*)(pDest))[i] = ((uint8_t*)(pSrc))[i];}
#define FMT_SIZET "%ld"
#else
#define FMT_SIZET "%d"
#endif

#ifdef __ARM_PIKA_PAL__
// Microsoft Pika Platform Abstraction Layer (PAL) definitions
#include "../rmpal/include/rmpaltypes.h"
#include "../rmpal/include/rmpalexec.h"
#include "../rmpal/include/rmpalsocket.h"
#define FMT_UINT32 "%lu"
#define FMT_UINT16 "%u"
#else
typedef unsigned long bool_t;
#define FMT_UINT32 "%u"
#define FMT_UINT16 "%u"
#endif

#include "lld2d_types.h"
#include "band.h"
#include "seeslist.h"
#include "osl.h"


#ifndef	GLOBALS_H
#define GLOBALS_H

#ifdef DECLARING_GLOBALS
#define GLOBAL
char	releaseVersion[] = {"RELEASE 1.2"};
#else
#define GLOBAL extern
#endif

#define TRUE 1
#define FALSE 0

GLOBAL  char                   *g_icon_path;
GLOBAL  char                   *g_jumbo_icon_path;

/* useful info about the interface and machine (TLV data) */
GLOBAL tlv_info_t               g_tlv_info;

#ifndef __MARVELL__
GLOBAL  char                    g_buf[160];
/* packet pointers and information, hoisted here for fast access */
/* pointer to ethernet header in rxbuf */
GLOBAL  topo_ether_header_t    *g_ethernet_hdr;
/* pointer to base header in rxbuf */
GLOBAL  topo_base_header_t     *g_base_hdr;
/* pointer to discover header in rxbuf */
GLOBAL  topo_discover_header_t *g_discover_hdr;
/* pointer to hello header in rxbuf */
GLOBAL	topo_hello_header_t    *g_hello_hdr;
/* pointer to query-large-tlv header in rxbuf */
GLOBAL  topo_qltlv_header_t    *g_qltlv_hdr;
#endif // __MARVELL__

/* OS-Layer state */
GLOBAL  osl_t	*g_osl;

GLOBAL	uint    g_trace_flags;

/* flags for subsystems to trace: */
#define TRC_BAND    0x01
#define TRC_PACKET  0x02
#define TRC_CHARGE  0x04
#define TRC_TLVINFO 0x08
#define TRC_STATE   0x10
#define TRC_QOS     0x20
#define TRC_SELECT  0x40

#ifdef DEBUG

#define TRACE(x) (g_trace_flags & x)
#define IF_TRACED(f) if (g_trace_flags & f) {
#define DBG_PRFX "lltd: "
#define LOGGER_MODULE_MASK \
                DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_LLTD
#define DBG_ERR(...) DBG_PRINTF_ERR(__VA_ARGS__)
#define DBG_CRIT(...) DBG_PRINTF_CRIT(__VA_ARGS__)
#define DBG_NOTICE(...) DBG_PRINTF_NOTICE(__VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF_DEBUG(__VA_ARGS__)
#define IF_DEBUG {

#else // DEBUG

#define TRACE(x) (0)
#define IF_TRACED(f) if (0) {
#define DBG_ERR(...)
#define DBG_CRIT(...)
#define DBG_NOTICE(...)
#define DBG_VERBOSE(...)
#define IF_DEBUG if (0){

#endif // DEBUG

#define END_DEBUG }
#define END_TRACE }

/**************************************  Q O S   G l o b a l s  **************************************/
#include "qosglobals.h"

#endif /*** GLOBALS_H ***/
