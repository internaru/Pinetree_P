/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef DBG_RAW_SOCKET_H
#define DBG_RAW_SOCKET_H
#ifdef HAVE_DBG_RAW_SOCKET
void dbg_raw_socket_init(void);
int dbg_socket_receive(unsigned char *data, int len, int wait_secs);
#else
#define dbg_raw_socket_init()
#define dbg_socket_receive(a, b, c) 0
#endif
#endif /* DBG_RAW_SOCKET_H */
