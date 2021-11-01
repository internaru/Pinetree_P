/** @file include.h
 * 
 * @brief This file contains all the necessary include file.
 *  
 *  Copyright © Marvell International Ltd. and/or its affiliates, 2003-2007
 */
/********************************************************
Change log:
	10/11/05: Add Doxygen format comments
	01/11/06: Conditional include file removal/addition
	01/30/06: Add kernel 2.6 support
	
********************************************************/

#ifndef _INCLUDE_H_
#define _INCLUDE_H_


#include	"moal_main.h"
#include 	"moal_usb.h"
#include    "wlan_thread.h"


#include    "os_headers.h"
//#include    "wlan_types.h"
//#include    "wlan_defs.h"
//#include    "wlan_thread.h"

//#include    "mlan_wmm.h"
//#include    "mlan_11d.h"

#include    "os_timers.h"

//#include    "host.h"
//#include    "hostcmd.h"

//#include    "mlan_scan.h"
//#include    "mlan_join.h"

//#include    "wlan_dev.h"
#include    "os_macros.h"
//#include    "sbi.h"

//#if defined(GSPI8686)
//#include    <gspi_io.h>
//#elif defined(SPI8385)
//#include    <sdio_spi.h>
//#else
//#error "WIRELESS CONFIG ERROR: chip and interface undefined."
//#endif

//#include    "wlan_wext.h"
//#include    "wlan_decl.h"
#endif /* _INCLUDE_H_ */
