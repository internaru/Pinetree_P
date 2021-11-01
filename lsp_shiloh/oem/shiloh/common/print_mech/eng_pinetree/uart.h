/****************************************************************************
*	Copyright (c) 2012 SINDOH.Crp.  All right reserved.
*****************************************************************************/
#ifndef		UART_H
#define		UART_H

#undef	EXTERN 
#ifdef UART_H_MAIN
 #define	EXTERN 
#else
 #define	EXTERN extern
#endif

#define	VTIME_OUT		 1
#define	VMIN_READ_CHAR	 64
#define	VTIME_OUT_ForNoti		 1
#define	VMIN_READ_CHAR_ForNoti	 0

enum	UART_CH {
//	UCH_CONSOLE,
//	UCH_ENG,
//	UCH_SDC,
//	UCH_ETC,
	UCH_OPTION,
	UCH_MAX
};
typedef void (*UART_RECV_HANDLER)(int length, char *buf);
EXTERN	void	uart_recv_handler_register(int ch, UART_RECV_HANDLER *pFunc);
EXTERN	void	uart_init(unsigned char v_time, unsigned char v_min);
EXTERN	void	uart_exit(void);
EXTERN	void	send_to_eng(int length, char *ptr);
#endif
