/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * sdhEngStatusTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

// debug headers
#define DBG_PRFX "snmp: "
//#define DBG_ERR
//#define DBG_VERBOSE_ON
//#define DBG_RELEASE_ON
#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "sdhEngStatus.h"

#include "PrintJobInfo.h"
#include "pjl_extra_api.h"
#include "print_events_common_api.h"
#include "SysServiceMgr.h"
#ifdef HAVE_IMAGELOG
#include "imagelog.h"
#endif

#include "db_api.h"
#include "memAPI.h"

#define BASE_OID 1, 3, 6, 1, 4, 1, 27278, 1
#define SDHENGSCANSTATUS 1

#define SNMP_LOG     //PRINT LOG (/var/log/snmp-log.txt)

#define REGISTER_OID( name, add_oid... )	\
	static oid name##_oid[] = \
	{ BASE_OID, ##add_oid }; \
	int name##_oid_len = OID_LENGTH( name##_oid );

#define NETSNMP_REGISTER_INSTANCE_RO( name ) \
	{	\
	netsnmp_register_read_only_instance( netsnmp_create_handler_registration( \
		#name, handler_##name, \
		name##_oid,	\
		OID_LENGTH( name##_oid ),	\
		HANDLER_CAN_RONLY ) );	\
	}

REGISTER_OID( EngScanStatus, 2, 1, 0 );
REGISTER_OID( EngFAXStatus, 2, 2, 0 );
REGISTER_OID( EngCopyStatus, 2, 3, 0 );
#ifdef HAVE_IMAGELOG
REGISTER_OID( EngImageLogStatus, 2, 4, 0 );
#endif

void init_sdhEngStatusTable( void ){
	printf( "sdhEngStatus Start!\n" );
	NETSNMP_REGISTER_INSTANCE_RO( EngScanStatus );
	NETSNMP_REGISTER_INSTANCE_RO( EngFAXStatus );
	NETSNMP_REGISTER_INSTANCE_RO( EngCopyStatus );
	#ifdef HAVE_IMAGELOG
	NETSNMP_REGISTER_INSTANCE_RO( EngImageLogStatus );
	#endif
}

int
handler_EngScanStatus( netsnmp_mib_handler *handler, 
				netsnmp_handler_registration *reginfo,
				netsnmp_agent_request_info *reqinfo,
				netsnmp_request_info *requests ){

	switch( reqinfo -> mode ){
		case MODE_GET:
		case MODE_GETNEXT:
		{
#ifdef SNMP_LOG
			FILE *fp;

			fp = fopen("/var/log/snmp-log.txt","a+");
#endif
			unsigned int Scan_Status = 0;
			Scan_Status = GetScanPJLStatusCode();

			if(Scan_Status == 62000){//jam
				Scan_Status = 0x00000004;
			}
			else if(Scan_Status == 67100){//fatal
				Scan_Status = 0x00000020;
			}
			else if(Scan_Status == 67200){//jam+fatal
				Scan_Status = 0x00000020;
			}
			else{
				Scan_Status = 0x00000000;
			}
#ifdef SNMP_LOG
			fprintf(fp,"SCAN :: AFTER:$$$$$$$$$$ %d $$$$$$$4444\n",Scan_Status);
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&Scan_Status, sizeof( Scan_Status ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_EngFAXStatus( netsnmp_mib_handler *handler, 
					netsnmp_handler_registration *reginfo,
					netsnmp_agent_request_info *reqinfo,
					netsnmp_request_info *requests ){
	switch( reqinfo -> mode ){
		case MODE_GET:
		case MODE_GETNEXT:
		{
			unsigned int iStatus = 0;
			unsigned int Scan_Status = 0;
			unsigned int printerStatus = 0;
			Scan_Status = GetScanPJLStatusCode();
			printerStatus = map_print_status_to_pjl(GetPrintSystemStatus());

#ifdef SNMP_LOG
			FILE *fp;

			fp = fopen("/var/log/snmp-log.txt","a+");
#endif			
			if(Scan_Status == 62000){//scan jam
				iStatus |= 0x00000100;
			}
			else if(Scan_Status == 67100){//scan fatal
				iStatus |= 0x00002000;
			}
			else if(Scan_Status == 67200){//scan jam+fatal
				iStatus |= 0x00002000;
			}

			if(printerStatus == 42060){//noPaper
				iStatus |= 0x00000080;
			}
			if(printerStatus == 40038){//lowToner
				iStatus |= 0x00000002;
			}
			if(printerStatus == 40039){//noToner
				iStatus |= 0x00000400;
			}
			if(printerStatus == 49999){//fatal
				iStatus |= 0x00002000;
			}
			if(printerStatus == 42060){//TrayEmpty
				iStatus |= 0x00000080;
			}
			if(printerStatus == 42200){//door open
				iStatus |= 0x00000040;
			}
			if(printerStatus == 42000){//paper jammed
				iStatus |= 0x00000200;
			}

			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&iStatus, sizeof( iStatus ) );
#ifdef SNMP_LOG
			fprintf(fp,"FAX_AFTER SCN:%d , IS%d\n",Scan_Status,iStatus);
			fclose(fp);
#endif
		}
		break;

		default:
			return SNMP_ERR_GENERR;
	}
	return SNMP_ERR_NOERROR;
}

int
handler_EngCopyStatus( netsnmp_mib_handler *handler, 
					netsnmp_handler_registration *reginfo,
					netsnmp_agent_request_info *reqinfo,
					netsnmp_request_info *requests ){
	switch( reqinfo -> mode ){
		case MODE_GET:
		case MODE_GETNEXT:
		{
			unsigned int iStatus = 0;
			unsigned int Scan_Status = 0;
			unsigned int printerStatus = 0;
			Scan_Status = GetScanPJLStatusCode();
			printerStatus = map_print_status_to_pjl(GetPrintSystemStatus());

			if(Scan_Status == 62000){//scan jam
				iStatus |= 0x00000100;
			}
			else if(Scan_Status == 67100){//scan fatal
				iStatus |= 0x00002000;
			}
			else if(Scan_Status == 67200){//scan jam+fatal
				iStatus |= 0x00002000;
			}

			if(printerStatus == 42060){//noPaper
				iStatus |= 0x00000080;
			}
			if(printerStatus == 40038){//lowToner
				iStatus |= 0x00000002;
			}
			if(printerStatus == 40039){//noToner
				iStatus |= 0x00000400;
			}
			if(printerStatus == 49999){//fatal
				iStatus |= 0x00002000;
			}
			if(printerStatus == 42060){//TrayEmpty
				iStatus |= 0x00000080;
			}
			if(printerStatus == 42200){//door open
				iStatus |= 0x00000040;
			}
			if(printerStatus == 42000){//paper jammed
				iStatus |= 0x00000200;
			}

			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&iStatus, sizeof( iStatus ) );
		}
		break;
		default:
			return SNMP_ERR_GENERR;
	}
	return SNMP_ERR_NOERROR;
}

#ifdef HAVE_IMAGELOG
int
handler_EngImageLogStatus( netsnmp_mib_handler *handler, 
					netsnmp_handler_registration *reginfo,
					netsnmp_agent_request_info *reqinfo,
					netsnmp_request_info *requests ){
	switch( reqinfo -> mode ){
		case MODE_GET:
		case MODE_GETNEXT:
		{
			unsigned int iStatus = 0;

			iStatus	= GetImageLogStatus(); // get status of ImageLog

#ifdef SNMP_LOG
			FILE *fp;

			fp = fopen("/var/log/snmp-log.txt","a+");
			/*
				ILS_NO_PROBLEM = 800,
			    ILS_SDCARD_FULL = 801,
			    ILS_SDCARD_WRITE_ERR = 802,
			    ILS_SDCARD_INVALID_FILE_SYSTEM = 803,
			    ILS_TRANSFER_ERR = 810,
			    ILS_SDCARD_FULL_TRANSFER_ERR = 811,
			*/
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&iStatus, sizeof( iStatus ) );
#ifdef SNMP_LOG
			fprintf(fp,"ImageLog Status:%d\n",iStatus);
			fclose(fp);
#endif
		}
		break;

		default:
			return SNMP_ERR_GENERR;
	}
	return SNMP_ERR_NOERROR;
}
#endif
