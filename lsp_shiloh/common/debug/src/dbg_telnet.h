/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef DBG_TELNET_H
#define DBG_TELNET_H

#ifdef HAVE_DBG_TELNET
void dbg_telnet_init(void);

/* Return <0 if telnet is not open or in the case of other errors */
int dbg_telnet_getchar(void);

/* Return 0 unless telnet wishes to have local console output inhibitted */
int dbg_telnet_inhibit_local_console(void);
#else
#define dbg_telnet_init() 
#define dbg_telnet_getchar() -1
#define dbg_telnet_inhibit_local_console() 0 

#endif
#endif /* DBG_TELNET_H */
