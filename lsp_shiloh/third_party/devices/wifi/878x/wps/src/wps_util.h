/** @file wps_util.h
 *  @brief This file contains definitions for debugging print functions.
 *  
 *  Copyright (C) 2003-2010,2012 Marvell International Ltd.
 *  All Rights Reserved
 */

#ifndef _WPS_UTIL_H_
#define _WPS_UTIL_H_

#include "wps_msg.h"

/** MAC to string */
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
/** MAC string */
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

/* 
 * Debugging function - conditional printf and hex dump.
 * Driver wrappers can use these for debugging purposes.
 */
/** Debug Message : None */
#define MSG_NONE            0x00000000
/** Debug Message : All */
#define DEBUG_ALL           0xFFFFFFFF

/** Debug Message : Message Dump */
#define MSG_MSGDUMP         0x00000001
/** Debug Message : Info */
#define MSG_INFO            0x00000002
/** Debug Message : Warning */
#define MSG_WARNING         0x00000004
/** Debug Message : Error */
#define MSG_ERROR           0x00000008

/** Debug Message : Init */
#define DEBUG_INIT          0x00000010
/** Debug Message : EAPOL */
#define DEBUG_EAPOL         0x00000020
/** Debug Message : WLAN */
#define DEBUG_WLAN          0x00000040
/** Debug Message : Call Flow  */
#define DEBUG_CALL_FLOW     0x00000080

/** Debug Message : WPS Message */
#define DEBUG_WPS_MSG       0x00000100
/** Debug Message : WPS State */
#define DEBUG_WPS_STATE     0x00000200
/** Debug Message : Post Run */
#define DEBUG_POST_RUN      0x00000400
/** Debug Message : Walk Time */
#define DEBUG_WALK_TIME     0x00000800
/** Debug Message : Event */
#define DEBUG_EVENT         0x00001000
/** Debug Message : Vendor TLV */
#define DEBUG_VENDOR_TLV    0x00002000
/** Debug Message : WFD Discovery */
#define DEBUG_WFD_DISCOVERY  0x00008000
/** Debug Message : Input */
#define DEBUG_INPUT          0x00010000

/** Default Debug Message */
#define DEFAULT_MSG     (MSG_NONE   \
|   MSG_INFO    \
|   MSG_WARNING \
|   MSG_ERROR   \
|   DEBUG_WPS_STATE   \
|   DEBUG_WPS_MSG   \
|   DEBUG_POST_RUN   \
|   MSG_NONE)

#ifdef DEBUG// STDOUT_DEBUG

/** 
 *  @brief Timestamp debug print function
 *
 *  @return         None
*/
void wps_debug_print_timestamp(void);

/** 
 *  @brief Debug print function
 *         Note: New line '\n' is added to the end of the text when printing to stdout.
 *
 *  @param level    Debugging flag
 *  @param fmt      Printf format string, followed by optional arguments
 *  @return         None
 */
void wps_printf(int level, char *fmt, ...);

/** 
 *  @brief Debug buffer dump function
 *         Note: New line '\n' is added to the end of the text when printing to stdout.
 *
 *  @param level    Debugging flag
 *  @param title    Title of for the message
 *  @param buf      Data buffer to be dumped
 *  @param len      Length of the buf
 *  @return         None
 */
void wps_hexdump(int level, const char *title, const unsigned char *buf,
                 size_t len);
#ifndef ENTER
#if 0
/** ENTER print */
#define ENTER()         wps_printf(DEBUG_CALL_FLOW, "Enter: %s, %s:%i", __FUNCTION__, \
                            __FILE__, __LINE__)
/** LEAVE print */
#define LEAVE()         wps_printf(DEBUG_CALL_FLOW, "Leave: %s, %s:%i", __FUNCTION__, \
                            __FILE__, __LINE__)
#else
/** ENTER print */
#define ENTER()
/** LEAVE print */
#define LEAVE()
#endif
#endif // ENTER

#else /* STDOUT_DEBUG */

/** WPS debug print timestamp */
#define wps_debug_print_timestamp() do { } while (0)
/** WPS printf */
#define wps_printf(args...) do { } while (0)
/** WPS hexdump */
#define wps_hexdump(args...) do { } while (0)

#ifndef ENTER
/** ENTER print */
#define ENTER()
/** LEAVE print */
#define LEAVE()
#endif // ENTER

#endif /* STDOUT_DEBUG */

#endif /* _WPS_UTIL_H_ */
