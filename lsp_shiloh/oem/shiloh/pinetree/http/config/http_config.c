/*
 * ============================================================================
 * Copyright (c) 2009   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file http_config.c
 *
 * \brief This file implements the public configuration API for the HTTP module.
 * This file should only be used the HTTP module.
 * 
 **/

#include <stdint.h>
#include <string.h>

#ifdef HAVE_OID
#include "oid_api.h"
#endif
#include "error_types.h"
#include "http_config.h"
#include "ATypes.h"


/* The XML Protocol version specific to this product's XML configuration layer */
#define HTTP_XML_VERSION "1.0"

/* The consumables supplies order link */
#define HTTP_ORDER_SUPPLIES_LINK "http://www.marvell.com/"

/* If a user simply goes to the IP address of the printer
 * What page should they land on? (i.e. http://10.0.0.1/) */
#define WEB_ROOT_PAGE "/view/home"

#define MAX_ACTIVE_HTTP_CONNECTIONS	8

#ifndef WEB_SERVER_PORT
#define WEB_SERVER_PORT 			8080
#endif

#ifndef HTTPS_SERVER_PORT
#define HTTPS_SERVER_PORT 			443
#endif

#define UDP_BUFFER_SIZE             100
#define TCP_BUFFER_SIZE             1025

static http_xml_to_oid_lut_t http_xml_to_oid_tag_wout_values_table[ ] = 
{
    /* Empty for this product for now */
};


static http_xml_to_oid_lut_t http_xml_to_oid_create_table[ ] = 
{
    /* Empty for this product for now */
};


#ifdef HAVE_OID
static http_xml_to_oid_lut_t http_xml_to_oid_normal_table[ ]=
{
    { "midas", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|version", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|status", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|devModelName", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|devPartNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|devSerialNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|firmwareRevision", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|controllerSerialNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|countryCode", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|productInfo|languageCode", HTTP_XML_TO_OID_LUT_ACTION, OID_STRING_MGR_LANGUAGE_CODE, 0 },
    { "midas|storage", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|storage|internalDevices", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|storage|internalDevices|internalStorage", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|storage|removableDevices", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|storage|removableDevices|removableStorage", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|networkStatus", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|macAddress", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|hostName", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_HOST_NAME, 0 },
    { "midas|ioConnections|wiredNetwork|connectionTimeout", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_CONNECTION_TIMEOUT, 0 },
    { "midas|ioConnections|wiredNetwork|linkSpeed", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_LINK_SPEED, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|ipv4Enabled", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|domainName", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_DOMAIN_NAME, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|lastConfiguredBy", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|allowDhcpConfig", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|allowBootpConfig", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|allowAutoipConfig", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|dhcpLeaseTime", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|ipv4Address", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_ADDRESS, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|subnetMask", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_SUBNET_MASK, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|winsServer", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_WINS_SERVER, 0 },
    { "midas|ioConnections|wiredNetwork|ipv4Config|defaultGateway", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_IPV4_DEFAULT_GATEWAY, 0 },
    { "midas|ioConnections|wiredNetwork|bonjourConfig", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|bonjourConfig|bonjourEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_BONJOUR_ENABLED, 0 },
    { "midas|ioConnections|wiredNetwork|bonjourConfig|bonjourServiceName", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_BONJOUR_SERVICE_NAME, 0 },
    { "midas|ioConnections|wiredNetwork|bonjourConfig|bonjourDomainName", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_BONJOUR_DOMAIN_NAME, 0 },
    { "midas|ioConnections|wiredNetwork|networkStats", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|lpdEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_LPD_ENABLED, 0 },
    { "midas|ioConnections|wiredNetwork|slpEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SLP_ENABLED, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig|snmpEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SNMP_ENABLED, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig|readWriteEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SNMP_RW_ENABLED, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig|communitySetString", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SNMP_COMMUNITY_SET_STRING, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig|communityGetString", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SNMP_COMMUNITY_GET_STRING, 0 },
    { "midas|ioConnections|wiredNetwork|snmpConfig|publicAccessEnabled", HTTP_XML_TO_OID_LUT_ACTION, OID_NETWORK_SNMP_PUBLIC_ACCESS_EN, 0 },
    { "midas|ioConnections|usbConnection", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|hostPrint", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|hostPrint|mediaSource", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_INPUT_TYPE_DEFAULT, 0 },
    { "midas|functions|hostPrint|mediaType", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_TYPE_DEFAULT, 0 },
    { "midas|functions|hostPrint|mediaSizeInfo", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|hostPrint|mediaSizeInfo|mediaSize", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_DEFAULT, 0 },
    { "midas|functions|hostPrint|mediaSizeInfo|customSize|width", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH, 0 },
    { "midas|functions|hostPrint|mediaSizeInfo|customSize|height", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT, 0 },
    { "midas|functions|hostPrint|mediaSizeInfo|customSize|unitOfMeasure", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE, 0 },
    { "midas|functions|copy", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|copy|numberOfCopies", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|copy|copyReduceEnlarge", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|copy|copyQuality", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|copy|copyLightDark", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|scan", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|scan|targetHosts", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|scan|targetHosts|targetHost", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|scan|targetHosts|targetHost|hostName", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|functions|scan|targetHosts|targetHost|hostScanPending", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyPosition", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyId", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyModelName", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplySerialNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyPartNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyInstallDate", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|percentRemaining", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|totalPagesPrinted", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|pagesRemaining", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|lowLevelIndicator", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|supplyOutIndicator", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|numberOfRefills", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|refillVendor", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|lastDateUsed", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|totalMaintCycles", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|markerSupply|factoryData", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplyPosition", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplyId", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplyModelName", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplySerialNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplyPartNumber", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|supplyInstallDate", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|totalPagesPrinted", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|lastDateUsed", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|totalMaintCycles", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|printHead|factoryData", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|mediaSupply", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|mediaSupply|trayId", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|mediaSupply|mediaSizeInfo", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|supplies|mediaSupply|mediaSizeInfo|mediaSize", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_INPUT_TRAY_MEDIA_SIZE, 0 },
    { "midas|supplies|mediaSupply|mediaSizeInfo|customSize|width", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_WIDTH, 0 },
    { "midas|supplies|mediaSupply|mediaSizeInfo|customSize|height", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_HEIGHT, 0 },
    { "midas|supplies|mediaSupply|mediaSizeInfo|customSize|unitOfMeasure", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_MEDIA_SIZE_CUSTOM_UNIT_OF_MEASURE, 0 },
    { "midas|supplies|mediaSupply|mediaType", HTTP_XML_TO_OID_LUT_ACTION, OID_PRINT_INPUT_TRAY_MEDIA_TYPE, 0 },
    { "midas|usage", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|devInstallDate", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|printStats", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|printStats|colorPages", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|printStats|monoPages", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|copyStats", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|copyStats|colorPages", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|copyStats|monoPages", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|scanStats", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|scanStats|totalScans", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaSize", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaSize|mediaSizeStatsRecord", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaSize|mediaSizeStatsRecord|mediaSize", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaSize|mediaSizeStatsRecord|totalPrints", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaType", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaType|mediaTypeStatsRecord", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaType|mediaTypeStatsRecord|mediaType", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|usage|statsByMediaType|mediaTypeStatsRecord|totalPrints", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|service", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|service|deviceLogs", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
    { "midas|service|deviceLogs|deviceLogRecord", HTTP_XML_TO_OID_LUT_NO_ACTION, 0, 0 },
};
#else
static http_xml_to_oid_lut_t http_xml_to_oid_normal_table[ ] = 
{
};
#endif

static http_socket_config_t http_socket_config[ ] =
{   
    {   WEB_SERVER_PORT,    HTTP_NET_SOCKET_TCP   },
#ifdef HAVE_HTTPS
    {   HTTPS_SERVER_PORT,  HTTP_NET_SOCKET_TCP  },
#endif // HAVE_HTTPS
};

static http_config_t my_http_config =
{
	.xml_version_string = HTTP_XML_VERSION,
    .order_supplies_link = HTTP_ORDER_SUPPLIES_LINK,

    .normal_table = http_xml_to_oid_normal_table,
    .normal_table_size = sizeof( http_xml_to_oid_normal_table ) / sizeof( http_xml_to_oid_lut_t ),

    .tag_wout_values_table = http_xml_to_oid_tag_wout_values_table,
    .tag_wout_values_table_size = sizeof( http_xml_to_oid_tag_wout_values_table ) / sizeof( http_xml_to_oid_lut_t ),

    .create_table = http_xml_to_oid_create_table,
    .create_table_size = sizeof( http_xml_to_oid_create_table ) / sizeof( http_xml_to_oid_lut_t ),

	.default_root_page = WEB_ROOT_PAGE,
	.max_active_http_connections = MAX_ACTIVE_HTTP_CONNECTIONS,

	.socket_table = http_socket_config,
	.socket_table_size = sizeof( http_socket_config ) / sizeof( http_socket_config_t ),

	.udp_read_buffer_size = UDP_BUFFER_SIZE,
	.tcp_read_buffer_size = TCP_BUFFER_SIZE,

};


error_type_t http_config_get( http_config_t * config )
{
	error_type_t e_res;

	e_res = OK;

	if ( NULL == config )
	{
		e_res = FAIL;
	}

	if ( OK == e_res )
	{
		memcpy( config, &my_http_config, sizeof(http_config_t) );
	}

	return e_res;
}


