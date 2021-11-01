/****************************************************************************
*	Copyright (c) 2012 SINDOH.Crp.  All right reserved.
*****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "pthread.h"
#include "posix_ostools.h"
#include "sys_init_api.h"
#include "ATypes.h"
#include "lassert.h"

#define UART_H_MAIN
#include "uart.h"
#undef UART_H_MAIN


#define UART_READ_SIZE	2048
#define UART_STACK_SIZE     POSIX_MIN_STACK_SIZE

extern char fOptionTray1Set;
extern char fOptionTray2Set;
extern char fOptionTray1Sync;
extern char fOptionTray2Sync;
extern char fOptionTray1Ver;
extern char fOptionTray2Ver;
extern char fOptionTray1Pro;
extern char fOptionTray2Pro;

EXTERN	UART_RECV_HANDLER	UartRecvHandler[UCH_MAX];

static int	fd_ttys[UCH_MAX] = {0};
//int ufd;
static pthread_t	uart_thd_id;
static unsigned char	uart_stack[UART_STACK_SIZE]ALIGN8;
static void *uart_recv_thread(void *);
static int	get_fd(int);
struct termios options;
static int ret;
//static int STOP=FALSE;
char fUartReadEnable;
void	uart_init(unsigned char v_time, unsigned char v_min) {
	int px_status;


#define	DEV_CH			UCH_OPTION
#define	DEV_TTYS		"/dev/ttyS3"
#define	BAUDRATE		 B38400
	fd_ttys[DEV_CH] = open( DEV_TTYS , O_RDWR | O_NOCTTY  );
//	ufd = fd_ttys[DEV_CH];
	if (fd_ttys[DEV_CH] < 0) {
		perror ( DEV_TTYS );
	}
	printf("step1: %d\n", fd_ttys[DEV_CH] );
#if 0
	tcgetattr(fd_ttys[DEV_CH], &options);
//	printf("(eng)ret0: %d\n", ret);
	int i;
	printf("c_iflag : %xh\n", options.c_iflag );
	printf("c_oflag : %xh\n", options.c_oflag );
	printf("c_cflag : %xh\n", options.c_cflag );
	printf("c_lflag : %xh\n", options.c_lflag );
	printf("c_ispeed : %xh\n", options.c_ispeed );
	printf("c_ospeed : %xh\n", options.c_ospeed );
	for ( i = 0; i < NCCS ; i++ ) {
		printf("c_cc[%d]: %xh\n", i, options.c_cc[i] );
	}
#endif
	bzero(&options, sizeof(options)); /* clear struct for new port settings */
	options.c_iflag = IGNPAR;
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD | PARENB | PARODD;
	options.c_oflag = 0;
	options.c_cc[VTIME] = v_time;
	options.c_cc[VMIN]  = v_min;
//	tcsetattr(fd_ttys[DEV_CH], TCSANOW, &options);
//	cfsetispeed(&options,  BAUDRATE  );
//	cfsetospeed(&options,  BAUDRATE  );
	ret = tcflush(fd_ttys[DEV_CH], TCIFLUSH);
//	printf("(eng)ret1: %d\n", ret);
	ret = tcsetattr(fd_ttys[DEV_CH], TCSANOW, &options);
//	printf("(eng)ret2: %d\n", ret);
	/* Create thread for receiving serial data */
	px_status = posix_create_thread( &uart_thd_id, 
									uart_recv_thread, 
									0, 
									"uart_recv_thread",
									uart_stack, 
									UART_STACK_SIZE, 
									POSIX_THR_PRI_ENGINE);
	XASSERT( px_status==0, px_status );
}

void *uart_recv_thread(void *unused) {
	int fd;
	char buf[UART_READ_SIZE];
	int recv_length;

	fd = get_fd( UCH_OPTION );
	fUartReadEnable = FALSE;
	printf("option : uart_recv_thread fd: %d\n", fd);
	while (TRUE) {
//		if(fOptionTray1Set == TRUE) options.c_cc[VMIN] = 1;
//		else if(fOptionTray1Ver == TRUE) options.c_cc[VMIN] = 11;
//		else if(fOptionTray1Pro == TRUE) options.c_cc[VMIN] = 6;
//		else{}
//		ret = tcflush(fd_ttys[DEV_CH], TCIFLUSH);
//		ret = tcsetattr(fd_ttys[DEV_CH], TCSANOW, &options);
			
		recv_length = read(fd, buf, UART_READ_SIZE); 
//		if(recv_length >= 1) printf("(eng) recv_length = %d\n",recv_length);
		if(fUartReadEnable == TRUE) {
			(*UartRecvHandler[UCH_OPTION])(recv_length, buf);
		}
	}
	return 0;
}

void uart_exit(void) {
	int i;
	for ( i = UCH_OPTION ; i < UCH_MAX ; i++ ) {
		if ( fd_ttys[i] > 0 ) close( fd_ttys[i] );
	}
}

void uart_recv_handler_register(int ch, UART_RECV_HANDLER *pFunc) {
	if ( ch < UCH_MAX ) {
		UartRecvHandler[ch] = (void *)pFunc;
		printf("uart_ch%d's recv_handler is registed\n", ch);
	}
	else {
		printf("error at uart_recv_handler_register([%d],)\n", ch);
	}
}


int get_fd( int channel ) {
//	if ( channel < UCH_ENG || channel > UCH_ETC ) {
//		return -1;
//	}
	
//	if ( fd_ttys[channel] <= 0 ) {
//		return -1;
//	}
	return  fd_ttys[channel];
}

void send_to_eng(int length, char *ptr) {
	char res;
	res = write(fd_ttys[UCH_OPTION], ptr, length );
//	printf("(eng) %d, fd_ttys: %d\n", res, fd_ttys[0]);
}
