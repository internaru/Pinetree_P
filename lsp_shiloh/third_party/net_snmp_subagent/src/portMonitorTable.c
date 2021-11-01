/*
 * ============================================================================
 * Copyright (c) 2014   SINDOH International, Ltd. All Rights Reserved
 * ============================================================================
 *
 * portMonitorTable Printer MIB
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

// debug headers
#define DBG_PRFX "snmp: "
#include "net_debug.h" // DBG_ family macros

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/library/snmp_assert.h>

#include "printer_mib_validation.h"
#include "portMonitorTable.h"

#include "PrintJobInfo.h"
#include "pjl_extra_api.h"
#include "print_events_common_api.h"
#include "SysServiceMgr.h"

#include "db_api.h"
#include "memAPI.h"

#define BASE_OID 1, 3, 6, 1, 4, 1, 2699, 1, 2, 1

//#define SNMP_LOG     //PRINT LOG (/var/log/snmp-log.txt)

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


REGISTER_OID( ppmGeneralNaturalLanguage, 1, 1, 0);
REGISTER_OID( ppmGeneralNumberOfPrinters, 1, 2, 0);
REGISTER_OID( ppmGeneralNumberOfPorts, 1, 3, 0);

REGISTER_OID( ppmPrinterName, 2, 1, 1, 2, 1);
REGISTER_OID( ppmPrinterIEEE1284DeviceId, 2, 1, 1, 3, 1);
REGISTER_OID( ppmPrinterNumberOfPorts, 2, 1, 1, 4, 1);
REGISTER_OID( ppmPrinterPreferredPortIndex, 2, 1, 1, 5, 1);
REGISTER_OID( ppmPrinterHrDeviceIndex, 2, 1, 1, 6, 1);
REGISTER_OID( ppmPrinterSnmpCommunityName, 2, 1, 1, 7, 1);
REGISTER_OID( ppmPrinterSnmpQueryEnabled, 2, 1, 1, 8, 1);

REGISTER_OID( ppmPortEnabled, 3, 1, 1, 2, 1, 1);
REGISTER_OID( ppmPortProtocolType, 3, 1, 1, 5, 1, 1);

void init_portMonitorTable( void ){

	printf( "portMonitorTable Start!\n" );
	
	NETSNMP_REGISTER_INSTANCE_RO( ppmGeneralNaturalLanguage );
	NETSNMP_REGISTER_INSTANCE_RO( ppmGeneralNumberOfPrinters );
	NETSNMP_REGISTER_INSTANCE_RO( ppmGeneralNumberOfPorts );
	
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterName );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterIEEE1284DeviceId );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterNumberOfPorts );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterPreferredPortIndex );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterHrDeviceIndex );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterSnmpCommunityName );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPrinterSnmpQueryEnabled );
	
	NETSNMP_REGISTER_INSTANCE_RO( ppmPortEnabled );
	NETSNMP_REGISTER_INSTANCE_RO( ppmPortProtocolType );

}

int
handler_ppmGeneralNaturalLanguage( netsnmp_mib_handler *handler, 
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
			char str[256] = "en-US";
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value(requests -> requestvb, ASN_OCTET_STR, str, strlen(str));
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmGeneralNumberOfPrinters( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_UNSIGNED,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmGeneralNumberOfPorts( netsnmp_mib_handler *handler, 
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
			unsigned int var = 2;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_UNSIGNED,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterName( netsnmp_mib_handler *handler, 
				netsnmp_handler_registration *reginfo,
				netsnmp_agent_request_info *reqinfo,
				netsnmp_request_info *requests ){

	switch( reqinfo -> mode ){
		case MODE_GET:
		case MODE_GETNEXT:
		{
			error_type_t db_retval = FAIL;
			char *MachineSerial;
			char *MachineTLI;
			char str[64];

			MachineSerial = db_get_var("INST_machine","MachineSerialNo",&db_retval);
			MachineTLI = db_get_var("INST_machine","MachineTLI",&db_retval);
			if((MachineSerial == NULL) || (db_retval == FAIL)){
				sprintf(str,"SINDOH_611_612Series");
			}    
			else{
				sprintf(str,"%s_%s",MachineTLI,MachineSerial);
			}   

			snmp_set_var_typed_value(requests -> requestvb, ASN_OCTET_STR, str, strlen(str));

			MEM_FREE_AND_NULL(MachineSerial);
			MEM_FREE_AND_NULL(MachineTLI);
#ifdef SNMP_LOG
			FILE *fp;

			fp = fopen("/var/log/snmp-log.txt","a+");
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
handler_ppmPrinterIEEE1284DeviceId( netsnmp_mib_handler *handler, 
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
			char str[256] = "CMD:PCL, PJL, PostScript, XPS;";
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value(requests -> requestvb, ASN_OCTET_STR, str, strlen(str));
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterNumberOfPorts( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_UNSIGNED,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterPreferredPortIndex( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterHrDeviceIndex( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterSnmpCommunityName( netsnmp_mib_handler *handler, 
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
			char str[32] = "public";
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
snmp_set_var_typed_value(requests -> requestvb, ASN_OCTET_STR, str, strlen(str));
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPrinterSnmpQueryEnabled( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPortEnabled( netsnmp_mib_handler *handler, 
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
			unsigned int var = 1;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

int
handler_ppmPortProtocolType( netsnmp_mib_handler *handler, 
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
			unsigned int var = 11;
			
#ifdef SNMP_LOG
			fprintf(fp,"portMonitor \n");
			fclose(fp);
#endif
			snmp_set_var_typed_value( requests -> requestvb, ASN_INTEGER,
					( u_char* )&var, sizeof( var ) );
		}
		break;

		default:
				return SNMP_ERR_GENERR;
	}
			
	return SNMP_ERR_NOERROR;
}

