/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef __INC_DBG_OUTBUF_H__
#define __INC_DBG_OUTBUF_H__

void dbg_outbuf_init(void* uart_hdl);
void dbg_outbuf_shutdown(void);
void dbg_outbuf_sync(void);
void dbg_outbuf_mutex(int acquire_nRelease);
void dbg_outbuf_add_char(char c);
char dbg_outbuf_get_char(void);

#endif /* __INC_DBG_OUTBUF_H__ */
