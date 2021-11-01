// =============================================================================
//	Copyright (C) 2009-2012, Heartland Data Co, Ltd.  All Rights Reserved.
//	
//	Title:	Ethernet Driver(for Winodws)
//	
// =============================================================================

// =============================================================================
// 	Please customize the code for your environment.
// =============================================================================

//------------------------------------------------------------------------------
//	Desc:	Header for Ethernet Controll
//------------------------------------------------------------------------------
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "logger.h"

#define	DBG_PRFX "DT10:"
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMODULE_BIT(12)
//------------------------------------------------------------------------------
//  Macro:	DT_UINT
//	Desc:	Please chage Test Point argument type for DT10 Project setting.
//------------------------------------------------------------------------------
#define	DT_UINT unsigned int

//------------------------------------------------------------------------------
//  Macro:	DT_ETHER_XXX
//	Desc:	Please select Ethernet mode
//------------------------------------------------------------------------------
#define	DT_ETHER_TCPIP_SERVER	0
#define DT_ETHER_TCPIP_CLIENT	0
#define DT_ETHER_UDP			1

//------------------------------------------------------------------------------
//	Macro:	DT_INLINE
//	Desc: 	Please use "static" instead of "inline" if "inline" cannot be used.
//------------------------------------------------------------------------------
#define	DT_INLINE	inline
//#define	DT_INLINE	static

//------------------------------------------------------------------------------
//	Func:	_TP_BusEthernetInit
//	Desc: 	Please describe the code to initializes Ethernet.
//------------------------------------------------------------------------------
static int sock = -1;
static struct sockaddr_in ipaddr;
#if DT_ETHER_TCPIP_SERVER
static int ssock = -1;
static struct sockaddr_in sipaddr;
#endif

static void _TP_BusEthernetInit(void)
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
//	int		n;
#if	DT_ETHER_TCPIP_CLIENT
	ipaddr.sin_family = AF_INET;
	ipaddr.sin_addr.s_addr = inet_addr("192.168.10.200");
	ipaddr.sin_port = htons(5000);
#elif DT_ETHER_TCPIP_SERVER
	sipaddr.sin_family = AF_INET;
	sipaddr.sin_addr.s_addr = INADDR_ANY;
	sipaddr.sin_port = htons(3000);
#else
	ipaddr.sin_family = AF_INET;
	ipaddr.sin_addr.s_addr = inet_addr("10.15.21.141");
	ipaddr.sin_port = htons(27000);
#endif
#if DT_ETHER_TCPIP_CLIENT
	sock = socket( AF_INET, SOCK_STREAM, 0);
#elif DT_ETHER_TCPIP_SERVER
	ssock = socket( AF_INET, SOCK_STREAM, 0);
#else
	sock = socket( AF_INET,  SOCK_DGRAM, 0 );
#endif
#if DT_ETHER_TCPIP_CLIENT
	while( 0 != connect( sock, (struct sockaddr *)&ipaddr, sizeof(ipaddr)));
#elif DT_ETHER_TCPIP_SERVER
	bind( ssock, (struct sockaddr *)&sipaddr, sizeof(sipaddr) );
	listen( ssock, 1 );
	n = sizeof( ipaddr );
	sock = accept( ssock, (struct sockaddr *)&ipaddr, &n );
#endif
}

//------------------------------------------------------------------------------
//	Func: _TP_BusOutDrv
//	Desc: Test Point Output Function
//------------------------------------------------------------------------------
static int init = 0;
#if DT_ETHER_TCPIP_SERVER || DT_ETHER_TCPIP_CLIENT
DT_INLINE void _TP_BusOutDrv( unsigned char *buff, DT_UINT count )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	if( init == 0 ){
		_TP_BusEthernetInit();
		init = 1;
	}
	send( sock, (const char *)buff, count, 0 );
}
#else
DT_INLINE void _TP_BusOutDrv( unsigned char *buff, DT_UINT count )
{	
	int ret = 0;
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	if( init == 0 ){
		_TP_BusEthernetInit();
		init = 1;
	}
	ret = sendto( sock, (const char *)buff, count, 0, (struct sockaddr *)&ipaddr, sizeof(ipaddr) );
	DBG_PRINTF(LOG_SDH_ERR, "%s %d: sendto ret: %d\n", __func__, __LINE__, ret);
}
#endif

//------------------------------------------------------------------------------
//	Macro:	DT_MAX_VAL_SIZE
//	Desc:	Please set the valule of Variables lenght.
//			But, never set the value over 256.
//------------------------------------------------------------------------------
#define DT_MAX_VAL_SIZE		256

//------------------------------------------------------------------------------
//	Macro:	DT_ADD_EVENT_INFO
//	Desc:	Please set 1 when you add Event Value to Test Point.
//------------------------------------------------------------------------------
#define	DT_ADD_EVENT_INFO	0

//------------------------------------------------------------------------------
//	Func: _TP_EventInfoSet
//	Desc: Please describe the code to set the Event Value.
//------------------------------------------------------------------------------
#if DT_ADD_EVENT_INFO
DT_INLINE void _TP_EventInfoSet( unsigned char *buff, DT_UINT *count )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	static int Event = 0;
	Event ++ ;
	buff[0] = Event;
	buff[1] = Event >> 8;
	buff[2] = Event >> 16;
	buff[3] = Event >> 24;
	*count += 4;
}
#endif

//------------------------------------------------------------------------------
//	Macro:	DT_ADD_TIME_INFO
//	Desc:	Please set 1 when you add Event Value to Test Point.
//------------------------------------------------------------------------------
#define DT_ADD_TIME_INFO	0

//------------------------------------------------------------------------------
//	Func: _TP_TimeInfoSet
//	Desc: Please describe the code to set the Time Info.
//------------------------------------------------------------------------------
#if DT_ADD_TIME_INFO
DT_INLINE void _TP_TimeInfoSet( unsigned char *buff, DT_UINT *count )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	static int Time = 0;
	Time ++ ;
	buff[0] = Time;
	buff[1] = Time >> 8;
	buff[2] = Time >> 16;
	buff[3] = Time >> 24;
	buff[4] = 0x00;
	buff[5] = 0x00;
	buff[6] = 0x00;
	buff[7] = 0x00;
	*count += 8;
}
#endif

// =============================================================================
// 	Don't change the code from here as possible.
// =============================================================================

//------------------------------------------------------------------------------
//	Macro:	Command Macros
//	Desc:	Please do not change the value.
//------------------------------------------------------------------------------
#define	DT_VARIABLE_BIT			0x02
#define	DT_EVTTRG_BIT			0x08
#if (DT_ADD_EVENT_INFO == 1)
#define DT_EVENT_INFO_SIZE	4
#else
#define DT_EVENT_INFO_SIZE	0
#endif
#if (DT_ADD_TIME_INFO == 1)
#define DT_TIME_INFO_SIZE	8
#else
#define DT_TIME_INFO_SIZE	0
#endif
#if (DT_ADD_EVENT_INFO == 1 && DT_ADD_TIME_INFO == 1)
#define DT_NRMLTP_ID		0x15		// Normal TP with Event & Time info.
#elif (DT_ADD_EVENT_INFO == 1)
#define DT_NRMLTP_ID		0x05		// Normal TP with Event info.
#elif (DT_ADD_TIME_INFO == 1)
#define DT_NRMLTP_ID		0x11		// Normal TP with Time info.
#else
#define DT_NRMLTP_ID		0x01		// Normal TP
#endif
#if (DT_ADD_EVENT_INFO == 1 && DT_ADD_TIME_INFO == 1)
#define DT_VALTP_ID			0x16		// Variable TP with Event & Time info.
#elif (DT_ADD_EVENT_INFO == 1)
#define DT_VALTP_ID			0x06		// Variable TP with Event info.
#elif (DT_ADD_TIME_INFO == 1)
#define DT_VALTP_ID			0x12		// Variable TP with Time info.
#else
#define DT_VALTP_ID			0x02		// Variable TP
#endif
#define DT_HEADER_SIZE		4
#define	DT_NRMLTP_SIZE		(DT_HEADER_SIZE+6+DT_EVENT_INFO_SIZE+DT_TIME_INFO_SIZE)
#define DT_MAX_VALTP_SIZE	(DT_NRMLTP_SIZE+1+DT_MAX_VAL_SIZE)
#define	DT_EVENTTP_SIZE		(DT_NRMLTP_SIZE)

//------------------------------------------------------------------------------
//	Func: _TP_BusOut
//	Desc: Called by Test Point
//------------------------------------------------------------------------------
void _TP_BusOut( DT_UINT addr, DT_UINT dat )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	DT_UINT count = 0;
	unsigned char buff[DT_NRMLTP_SIZE];

	buff[0] = 0xFF;							// Header
	buff[1] = 0xFF;							// Header
	buff[2] = DT_NRMLTP_SIZE;				// Data Size
	buff[3] = 0x00;							// Data Size
	buff[4] = DT_NRMLTP_ID;					// Var TP
	buff[5] = dat;							// dat
	buff[6] = dat >> 8;						// dat
	buff[7] = addr;							// addr
	buff[8] = addr >> 8;					// addr
	buff[9] = addr >> 16;					// addr
	count	= 10;
#if (DT_ADD_TIME_INFO == 1)
	_TP_TimeInfoSet( buff + count, &count );
#endif
#if (DT_ADD_EVENT_INFO == 1)
	_TP_EventInfoSet( buff + count, &count );
#endif
	_TP_BusOutDrv( buff, count );
}

//------------------------------------------------------------------------------
//	Func: _TP_MemoryOutput
//	Desc: Called by Variable Test Point
//------------------------------------------------------------------------------
void _TP_MemoryOutput( DT_UINT addr, DT_UINT dat, void *value, DT_UINT size )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	DT_UINT i, count = 0;
	unsigned char buff[DT_MAX_VALTP_SIZE];
	unsigned char *p = (unsigned char *)value;

	if( size >= DT_MAX_VALTP_SIZE ) size = DT_MAX_VALTP_SIZE;
	buff[0] = 0xFF;							// Header
	buff[1] = 0xFF;							// Header
	buff[2] = (DT_NRMLTP_SIZE+1+size);		// Data Size
	buff[3] = (DT_NRMLTP_SIZE+1+size) >> 8;	// Data Size
	buff[4] = DT_VALTP_ID;					// Var TP
	buff[5] = dat;							// dat
	buff[6] = dat >> 8;						// dat
	buff[7] = addr | 0x02;				// addr
	buff[8] = addr >> 8;					// addr
	buff[9] = addr >> 16;				// addr
	count	= 10;
#if (DT_ADD_TIME_INFO == 1)
	_TP_TimeInfoSet( buff + count, &count );
#endif
#if (DT_ADD_EVENT_INFO == 1)
	_TP_EventInfoSet( buff + count, &count );
#endif
	buff[count++] = size;					// Size
	for( i = 0; i < size; i++, p++, count++ ){
		buff[count] = *p;
	}
	_TP_BusOutDrv( buff, count );
}

//------------------------------------------------------------------------------
//	Func: _TP_EventTriggerFast
//	Desc: Called by Event Trigger Test Point
//------------------------------------------------------------------------------
void _TP_EventTrigger( DT_UINT addr, DT_UINT dat )
{
	DBG_PRINTF(LOG_SDH_ERR, "%s %d\n", __func__, __LINE__);
	_TP_BusOut( addr | DT_EVTTRG_BIT  , ((unsigned short)(dat&0x0FFF))<<4 );
}

// =============================================================================
