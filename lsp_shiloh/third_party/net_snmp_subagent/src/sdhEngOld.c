/*
 * ============================================================================
 * Copyright (c) 2019 Sindoh Co., Ltd. All Rights Reserved
 * ============================================================================
 * sdhEngOld.c: Solve 'GET command doesn't work properly on some old OIDs'
 * mspyo@sindoh.com
 * 2019-04-23
 * 
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

#include "sdhEngOld.h"

#include "PrintJobInfo.h"
#include "print_events_common_api.h"
#include "pjl_extra_api.h"
#include "SysServiceMgr.h"

#include "db_api.h"

// 1.3.6.1.4.1.27278.103.1.x
#define BASE_OID 1, 3, 6, 1, 4, 1, 27278, 1
// #define SNMP_LOG     //PRINT LOG (/var/log/snmp-log.txt)

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

/*
sdhEngOldStatus
sdhEngOldDrumLife
sdhEngOldBridge
sdhEngOldOneBin
sdhEngOldCopyCount
sdhEngOldPrintCount
sdhEngOldFaxCount
sdhEngOldHostPrintCount
sdhEngOldReportPrintCount
sdhEngOldDuplexPrintCount
*/
REGISTER_OID( sdhEngOldStatus, 4, 0 );
REGISTER_OID( sdhEngOldDrumLife, 5, 0 );
REGISTER_OID( sdhEngOldBridge, 6, 0 );
REGISTER_OID( sdhEngOldOneBin, 7, 0 );
REGISTER_OID( sdhEngOldCopyCount, 8, 0 );
REGISTER_OID( sdhEngOldPrintCount, 9, 0 );
REGISTER_OID( sdhEngOldFaxCount, 10, 0 );
REGISTER_OID( sdhEngOldHostPrintCount, 11, 0 );
REGISTER_OID( sdhEngOldReportPrintCount, 12, 0 );
REGISTER_OID( sdhEngOldDuplexPrintCount, 13, 0 );

void init_sdhEngOld( void ){
	printf( "[Net/SNMP] sdhEngOld Start!\n" );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldStatus );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldDrumLife );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldBridge );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldOneBin );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldCopyCount );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldPrintCount );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldFaxCount );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldHostPrintCount );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldReportPrintCount );
	NETSNMP_REGISTER_INSTANCE_RO( sdhEngOldDuplexPrintCount );
}

// sdhEngOldStatus
int
handler_sdhEngOldStatus ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
	
	int num = 0;
	num = map_print_status_to_pjl(GetPrintSystemStatus());
	if(num ==  0){
		printf("[%s:%d]##########STATUS##############%x\n",__func__,__LINE__,pec_get_current_printstatus());
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &num, sizeof(num));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldDrumLife
int
handler_sdhEngOldDrumLife ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
	
	int drumLevel = 0;
	error_type_t db_retval = FAIL;

	drumLevel = db_get_var_int("INST_DRUMCOUNT","CountInPercent",&db_retval);
	if(db_retval == FAIL){
		drumLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &drumLevel, sizeof(drumLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldBridge
int
handler_sdhEngOldBridge ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
	
	int num = 0;
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &num, sizeof(num));
	
	return SNMP_ERR_NOERROR;
}

// sdhEngOldOneBin
int
handler_sdhEngOldOneBin ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
	
	int num = 0;
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &num, sizeof(num));
	
	return SNMP_ERR_NOERROR;
}

// sdhEngOldCopyCount
int
handler_sdhEngOldCopyCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){

	int counterLevel = 0;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","CopyPrint",&db_retval); 
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldPrintCount
int
handler_sdhEngOldPrintCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){

	int counterLevel;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","TotalPrint",&db_retval); 
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldFaxCount
int
handler_sdhEngOldFaxCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){

	int counterLevel;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","FaxPrint",&db_retval); 
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldHostPrintCount
int
handler_sdhEngOldHostPrintCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){

	int counterLevel;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","HostPrint",&db_retval); 
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldReportPrintCount
int
handler_sdhEngOldReportPrintCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
			
	int counterLevel;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","ReportPrint",&db_retval); 
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}

// sdhEngOldDuplexPrintCount
int
handler_sdhEngOldDuplexPrintCount ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){
			
	int counterLevel;
	error_type_t db_retval = FAIL;

	counterLevel = db_get_var_int("INST_MACHINECOUNT","DuplexPrint",&db_retval);
	if(db_retval == FAIL){
		counterLevel = -1;
	}
	snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, (char *) &counterLevel, sizeof(counterLevel));

	return SNMP_ERR_NOERROR;
}
