/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * systemSystem Printer MIB
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
#include "systemSystem.h"

#include "db_api.h"
#include "memAPI.h"

#define BASE_OID 1, 3, 6, 1, 2, 1, 1
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

REGISTER_OID( SyssysName, 5, 0 );

void init_systemSystem( void ){
	printf( "systemSystem Start!\n" );
	NETSNMP_REGISTER_INSTANCE_RO( SyssysName );
}

int
handler_SyssysName ( netsnmp_mib_handler *handler, 
		netsnmp_handler_registration *reginfo,
		netsnmp_agent_request_info *reqinfo,
		netsnmp_request_info *requests ){

	//jeseo
	static char modelName[64];
	FILE *fp;
	fp = popen("/app/karas_pinetree_util.sh --get-product-string", "r");
	if (fp == NULL)
	{
		perror("erro : ");
		printf("%s[%d] use default sysdecr\n", __FILE__, __LINE__);
	}
	else
	{
		fgets(modelName, 64, fp);
		if (pclose(fp) != 0)
		{
			printf("%s[%d] use default sysdecr\n", __FILE__, __LINE__);
		}
		else
		{
			modelName[strlen(modelName) - 1] = '\0';
		}
	}

	snmp_set_var_typed_value(requests -> requestvb, ASN_OCTET_STR, modelName, strlen(modelName));

	return SNMP_ERR_NOERROR;
}
