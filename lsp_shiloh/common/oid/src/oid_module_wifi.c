/*
 * ============================================================================
 * Copyright (c) 2009-2010,2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file oid_module_network.c
 *
 * \brief This file implements the OID back-end functions for the
 * Network API module OIDs
 *
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "oid_api.h"
#include "oid_producer_api.h"
#include "net_iface_vars.h"

#define DBG_PRFX "wifi.oid: "
#define DBG_VERBOSE_ON
#define DBG_ERR_ON
#include "net_debug.h"

#ifdef __linux__

#include <sys/socket.h>
#include "net_wifi_api.h"
#include "net_wifi_api.h"
#include "net_wifi_vars.h"
#include "wifi_intf_vars.h"
#ifdef HAVE_WLAN
#include "net_wlan_api.h"
#endif
#ifdef HAVE_UAP
#include "net_uap_api.h"
#include "net_uap_vars.h"
#endif

#else // ! __linux__

#include "trsocket.h"
#include "ioutils.h"
#include "platform_api.h"
#include "dprintf.h"
#include "lassert.h"
#include "wifi_intf_vars.h"
#include "wifi_intf_api.h"
#include "dhcpd_vars.h"
#include <pthread.h>

#endif

#include "memAPI.h"
#include <posix_ostools.h>

#include "oid_module_wifi.h"
#include "NetworkIFModule.h"

#define MAX_FILTERED_ADDRS 16


/** \brief The base we want to use for strtoul function */
#define BASE_10 10
/** \brief Number of bytes for a null character */
#define LENGTH_OF_NULL          1
//local functions

#define DHCP_ENTRY_LINE_SIZE 36
/**
 * 
 * \brief The backend oid wifi API module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] data_ptr A pointer to the data buffer that receives the
 * string data.  This buffer must be at least "max_string_len_in_bytes" long.
 *
 * \param[in] max_string_len_in_bytes The maximum length in bytes that the 
 * character buffer can hold, NOT including the NULL terminating character. The
 * buffer won't be filled past this length + 1 for the NULL terminating
 * character.
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 *
 * OID_ERROR_VALUE_CAPPED - producer string was not able to fit into buffer
 *                          max_string_len_in_bytes was too small to fit the
 *                          entire string.
 *
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_wifi_get( oid_t oid,
                                         uint32_t index,
                                         char * data_ptr,
                                         uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = NET_IFACE_OK;

    //noisy DBG_VERBOSE("%s(x%x, %d)\n", __func__, (unsigned int)oid, index);

    if(index != WIFI_IFACE_ID_STATION && index !=WIFI_IFACE_ID_UAP )
        e_res = OID_ERROR_ILLEGAL_CHARS;

    if(OK == e_res) 
    {
        unsigned int active_if = index; // get_active_wifi_if(); // revisit lookup uap0 or wlan ..
        switch ( oid )
        {
            case OID_WIFI_INTF_SSID:
                XASSERT( max_string_len_in_bytes >= MAX_SSID_NAME_LEN, max_string_len_in_bytes );
                e_res = wifi_intf_get_var_ssid(index, (uint8_t*)data_ptr, MAX_SSID_NAME_LEN );
                break;
            case OID_WIFI_INTF_BSSID:
                {
                    uint8_t bssid[MAC_ADDR_LENGTH];
                    wifi_intf_get_var_bssid(index,bssid,MAC_ADDR_LENGTH);
                    sprintf(data_ptr, 
                            "%02x%02x%02x%02x%02x%02x",
                            bssid[0],
                            bssid[1],
                            bssid[2],
                            bssid[3],
                            bssid[4],
                            bssid[5]);
                    break;
                }
            case OID_WIFI_INTF_BSSID_BYTES:
                {
                    uint8_t bssid[MAC_ADDR_LENGTH];
                    wifi_intf_get_var_bssid(index,bssid,MAC_ADDR_LENGTH);
                    memcpy((void *) data_ptr,
                            (const void *) bssid, MAC_ADDR_LENGTH);
                    break;
                }
            case OID_WIFI_INTF_WPA_PASSPHRASE:
                ASSERT( MAX_PASS_PHRASE <= max_string_len_in_bytes );
                e_res = wifi_intf_get_var_passphrase(index, (uint8_t*)data_ptr, MAX_PASS_PHRASE);
                break;

            case OID_WIFI_INTF_WEP_KEY_1:
                e_res = wifi_intf_get_var_wep_key((uint8_t)index, 0, (uint8_t*)data_ptr, WEP_KEY_LEN+1);
                break;

            case OID_WIFI_INTF_WEP_KEY_2:
                e_res = wifi_intf_get_var_wep_key((uint8_t)index, 1, (uint8_t*)data_ptr, WEP_KEY_LEN+1);
                break;

            case OID_WIFI_INTF_WEP_KEY_3:
                e_res = wifi_intf_get_var_wep_key((uint8_t)index, 2, (uint8_t*)data_ptr, WEP_KEY_LEN+1);
                break;

            case OID_WIFI_INTF_WEP_KEY_4:
                e_res = wifi_intf_get_var_wep_key((uint8_t)index, 3, (uint8_t*)data_ptr, WEP_KEY_LEN+1);
                break;

            case OID_WIFI_INTF_WEP_CUR_KEY_VALUE:
                {
                    uint8_t cur_wep_key;
                    e_res = wifi_intf_get_var_wep_cur_key(index, &cur_wep_key);
                    if(OK == e_res) 
                    {
                        e_res = wifi_intf_get_var_wep_key((uint8_t)index, cur_wep_key, (uint8_t*)data_ptr, WEP_KEY_LEN+1);
                    }
                }
                break;

            case OID_WIFI_INTF_SSID_PREFIX:
                {
                    uint8_t len;
                    len = MAX_SSID_NAME_LEN;
                    e_res = wifi_intf_get_var_ssid_prefix((uint8_t)index, (uint8_t*)data_ptr, len);
                    break;
                }

            case OID_WIFI_INTF_MAC_ADDR:
                {
					char *macAddr = 0;

					if( NIF_getMacAddr_str_wifi(&macAddr) == NIF_Status_NotFound ){    
						sprintf(data_ptr,"00:00:00:00:00:00");
						printf("[%s]%d:getMacAddr_str_wired FAILED!\n",__FILE__,__LINE__);
					}
					else{
						sprintf(data_ptr,"%s",macAddr);
					}
					free(macAddr);
					/*
                    uint8_t mac_addr[MAC_ADDR_LENGTH];
                    e_res= wifi_intf_get_var_mac_addr(index, mac_addr, MAC_ADDR_LENGTH);
                    if(OK == e_res) 
                    {
                        sprintf( data_ptr,
                                 "%02x-%02x-%02x-%02x-%02x-%02x",
                                 mac_addr[0],
                                 mac_addr[1],
                                 mac_addr[2],
                                 mac_addr[3],
                                 mac_addr[4],
                                 mac_addr[5] );
                    }
					*/
                    break;
                }
 
            case OID_WIFI_UAP_DHCP_ADDR:
                {
					T_IPADDRv4 ipAddr;

					if( NIF_getIPAddrV4_int_wifi(&ipAddr) == NIF_Status_NotFound ){    
						sprintf(data_ptr,"000.000.000.000");
						printf("[%s]%d:getIPv4Addr_str_wired FAILED!\n",__FILE__,__LINE__);
					}
					else{
						sprintf(data_ptr,"%d.%d.%d.%d",ipAddr.ipAddr[0],ipAddr.ipAddr[1],ipAddr.ipAddr[2],ipAddr.ipAddr[3]);
					}

					/*
                    uint32_t addr;
#ifdef __linux__
                    if ( wifi_intf_udhcp_get_var_server_addr(active_if, &addr ) != OK )
#else
                    if ( dhcpd_get_var_server_addr( &addr ) != OK )
#endif
                    {
                        e_res = FAIL;
                    }
                    else
                    {
                        inet_ntop(AF_INET, (void *)(&addr), data_ptr, INET_ADDRSTRLEN);
                    }
					*/
                }      
                break;  

            case OID_WIFI_UAP_DHCP_ADDR_PREFIX:
                {
					FILE *fp;
					char gateway[32];

					fp = popen("ip route show default | grep mlan0 | awk '/default/ {print $3}'","r");

					if(NULL == fp){
						printf("[%s]%d:getNetmask_str_wired FAILED!\n",__FILE__,__LINE__);
						return -1;
					}
					else{
						if( fgets(gateway,sizeof(gateway),fp) == NULL){
							sprintf(data_ptr,"000.000.000.000");
						}
						else{
							// remove '\n'
							*(gateway+(strlen(gateway)-1)) = 0;
							sprintf(data_ptr,"%s",gateway);
						}
						pclose(fp);
					}
/*
                    uint32_t addr;
#ifdef __linux__
                    if ( wifi_intf_udhcp_get_var_server_addr(active_if, &addr) != OK )
#else
                    if ( dhcpd_get_var_server_addr( &addr ) != OK )
#endif
                    {
                        e_res = FAIL;
                    }
                    else
                    {
                        uint8_t *octet;
                        octet = ( uint8_t*)addr;
                        sprintf( data_ptr, "%u.%u.%u", octet[0], octet[1], octet[2] );
                    }
*/
                }      
                break;

            case OID_WIFI_UAP_DHCP_SUBNET_MASK:                      
                {
					char *subnetmask = 0;

					if( NIF_getNetmaskV4_str_wifi(&subnetmask) == NIF_Status_NotFound ){    
						sprintf(data_ptr,"000.000.000.000");
						printf("[%s]%d:getNetmask_str_wired FAILED!\n",__FILE__,__LINE__);
					}
					else{
						sprintf(data_ptr,"%s",subnetmask);
					}
					free(subnetmask);

					/*
                    uint32_t addr;
#ifdef __linux__
                    if ( wifi_intf_udhcp_get_var_subnet_mask(active_if, &addr) != OK )
#else
                    if ( dhcpd_get_var_subnet_mask(&addr) != OK )
#endif
                    {
                        e_res = FAIL;
                    }
                    else
                    {
                        inet_ntop(AF_INET, (void *)(&addr), data_ptr, INET_ADDRSTRLEN);
                    }
					*/
                }
				break;

            case OID_WIFI_UAP_DHCP_START_ADDR:
                {
                    uint32_t addr;
#ifdef __linux__
                    if ( wifi_intf_udhcp_get_var_starting_addr(active_if, &addr) != OK )
#else
                    if ( dhcpd_get_var_start_addr(&addr) != OK )
#endif
                    {
                        e_res = FAIL;
                    }
                    else
                    {
                        inet_ntop(AF_INET, (void *)(&addr), data_ptr, INET_ADDRSTRLEN);
                    }

                }
                break;

            case OID_WIFI_UAP_DHCP_END_ADDR:
                {
                    uint32_t addr;
                    uint8_t last_byte;
                    uint8_t num_addrs;
#ifdef __linux__
                    if ( wifi_intf_udhcp_get_var_starting_addr(active_if, &addr) != OK )
#else
                    if ( dhcpd_get_var_starting_addr(&addr) != OK )
#endif
                    {
                        e_res = FAIL;
                    }
                    else
                    {
                        addr = htonl( addr ); // addr bytes in network-order
                        last_byte = addr & 0xFF;
#ifdef __linux__
                        if ( wifi_intf_udhcp_get_var_num_addrs(active_if, &num_addrs) != OK )
#else
                        if ( dhcpd_get_var_num_addrs( &num_addrs ) != OK )
#endif
                        {
                            e_res = FAIL;
                        }
                        else
                        {
                            // Subtract one because the start address is the 1st valid value
                            last_byte += num_addrs - 1;

                            // 255 is not a valid address to assign - so stop at 254
                            if ( last_byte > 254 )
                            {
                                last_byte = 254;
                            }
                            addr &= ~0xFF;
                            addr |= last_byte;
                            addr = htonl( addr ); // addr bytes in network-order
                            inet_ntop(AF_INET, (void *)(&addr), data_ptr, INET_ADDRSTRLEN);
                        }
                    }
                }
                break;

#ifdef HAVE_WPS
            case OID_WIFI_INTF_GENERATE_WPS_PIN:
                {
                    char *generated_pin ;
                    generated_pin = generate_wps_pin();
                    if ( generated_pin == NULL )
                    {
                        e_res = OID_ERROR_VALUE_CAPPED;
                    }
                    else
                    {
                        strcpy( data_ptr, generated_pin );
                        MEM_FREE_AND_NULL( generated_pin );
                        e_res = OK;
                    }
                    break;
                }
#endif

#ifdef HAVE_WIRELESS_FILTERS
            case OID_WIFI_INTF_MAC_FILTER_LIST_UAP:
                if ( index > MAX_FILTERED_ADDRS )
                {
                    e_res = OID_ERROR_INDEX_INVALID;
                }
                else
                {
                    uint8_t filter_list[ MAX_FILTERED_ADDRS ][ MAC_ADDR_LENGTH ];
                    e_res = wifi_intf_get_var_mac_filter_list(WIFI_IFACE_ID_UAP, filter_list );
                    if ( e_res == OK )
                    {
                        sprintf( data_ptr,
                                 "%02x-%02x-%02x-%02x-%02x-%02x",
                                 filter_list[index][0],
                                 filter_list[index][1],
                                 filter_list[index][2],
                                 filter_list[index][3],
                                 filter_list[index][4],
                                 filter_list[index][5] );
                    }
                }
                break;
#endif

            default:
                {
                    /* bad oid */
                    e_res = FAIL;

                    break;
                }
        }
    }
    return e_res;
}


error_type_t save_wep_key( uint8_t inteface_id, uint8_t web_key_index, const char * data, uint32_t string_len_in_bytes )
{
    error_type_t e_res;
    e_res = OK;
    if ( (string_len_in_bytes == WEP_KEY_LENGTH_OF_ALPHANUM_64BIT_KEY) ||
         (string_len_in_bytes == WEP_KEY_LENGTH_OF_ALPHANUM_128BIT_KEY) ||
         (string_len_in_bytes == WEP_KEY_LENGTH_OF_HEX_64BIT_KEY) ||
         (string_len_in_bytes == WEP_KEY_LENGTH_OF_HEX_128BIT_KEY) ||
         (string_len_in_bytes == 0 ) )
    {
        e_res = wifi_intf_set_var_wep_key(inteface_id, web_key_index,(const uint8_t* )data, string_len_in_bytes);
        if ( ( OK == e_res ) && ( string_len_in_bytes != 0 ) )
        {
            e_res = wifi_intf_set_var_wep_key_len(inteface_id, string_len_in_bytes);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
        }
    }
    else if ( strncmp( data, MASKED_TEXT, strlen( MASKED_TEXT ) ) != 0 )
    {
        e_res= OID_ERROR_VALUE_TOO_BIG;  
    }
    if(OK != e_res)
    {
        e_res = OID_ERROR_ILLEGAL_CHARS;
    }
    return e_res;
}

/**
 * 
 * \brief The backend oid module function for the wifi module set
 * functionality.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[out] data A pointer to the data buffer that contains the
 * string data.  This buffer must be at least "string_len_in_bytes" long.
 *
 * \param[in] string_len_in_bytes The length in bytes that the 
 * data buffer, "data", is, NOT the null terminating character.
 * The buffer must be at least string_len_in_bytes + 1 bytes long.
 *
 * \return error_type_t
 * OK                        - successful
 *
 * OID_ERROR_ILLEGAL_CHARS   - the string data had characters that the 
 *                             consumer could not use or interpret
 *
 * OID_ERROR_VALUE_TOO_BIG   - the sring_len_in_bytes was too large
 *                             or the data value represented by the string
 *                             was too large for the intended use
 *
 * OID_ERROR_VALUE_TOO_SMALL - the string_len_in_bytes was too small
 *                             or the data value represented by the
 *                             string was too small for the intended use
 *
 * FAIL                      - bad oid, other error
 *
 */
error_type_t oid_module_wifi_set( oid_t oid,
                                  uint32_t index,
                                  const char * data,
                                  uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    //noisy DBG_VERBOSE("%s() i%d bytes %d '%s'\n", __func__, index, string_len_in_bytes, (data?data:"<no data>"));

    if(index != WIFI_IFACE_ID_STATION && index !=WIFI_IFACE_ID_UAP )
        e_res = OID_ERROR_ILLEGAL_CHARS;


    if ( NULL == data )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        unsigned int active_if = index; // get_active_net_if() lookup?
        switch ( oid )
        {
            case  OID_WIFI_INTF_SSID:
            {
                if(string_len_in_bytes > MAX_SSID_NAME_LEN) 
                {
		    dbg_printf("%s() len %d > %d \n", __func__, string_len_in_bytes, MAX_SSID_NAME_LEN);
		    ASSERT(0);
                    e_res= OID_ERROR_VALUE_TOO_BIG;
                }
                if(OK == e_res) 
                {
#ifdef __linux__
                    e_res = wifi_intf_set_var_ssid(index, (uint8_t* )data, string_len_in_bytes);
#else
                    e_res = wifi_intf_set_var_ssid(index, (const uint8_t* )data, string_len_in_bytes);
#endif
                }
                if(OK != e_res)
                {
		    dbg_printf("%s() ILLEGAL CHARS '%s'?\n", __func__, data);
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            } break;

            case OID_WIFI_INTF_WPA_PASSPHRASE:
            {
                if(string_len_in_bytes > MAX_PASS_PHRASE) 
                {
                    e_res= OID_ERROR_VALUE_TOO_BIG;
                }
                else if ( strncmp( data, MASKED_TEXT, strlen( MASKED_TEXT ) ) == 0 )
                {
                    // Don't save the mask value
                    e_res = OK;
                }
                else if ( ( string_len_in_bytes < MIN_PASS_PHRASE ) &&
                          ( string_len_in_bytes != 0 ) )
                {
                    e_res= OID_ERROR_VALUE_TOO_SMALL;
                }
                else 
                {
                    e_res = wifi_intf_set_var_passphrase(index, (const uint8_t* )data, string_len_in_bytes);
                }
                if(OK != e_res)
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            } break;

            case OID_WIFI_INTF_WEP_KEY_1:
                e_res = save_wep_key( (uint8_t)index, 0, data,string_len_in_bytes);
                break;

            case OID_WIFI_INTF_WEP_KEY_2:
                e_res = save_wep_key( (uint8_t)index, 1, data,string_len_in_bytes);
                break;

            case OID_WIFI_INTF_WEP_KEY_3:
                e_res = save_wep_key( (uint8_t)index, 2, data,string_len_in_bytes);
                break;

            case OID_WIFI_INTF_WEP_KEY_4:
                e_res = save_wep_key( (uint8_t)index, 3, data,string_len_in_bytes);
                break;

            case OID_WIFI_INTF_SSID_PREFIX:
                e_res = wifi_intf_set_var_ssid_prefix(index, (const uint8_t* )data, string_len_in_bytes);
                if(OK != e_res)
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                break;

#ifdef HAVE_WPS
            case OID_WIFI_INTF_START_WPS:
                wifi_intf_start_wps((uint8_t *)NULL, 0, (uint8_t *)NULL, 0);
                break;

            case OID_WIFI_INTF_STOP_WPS:
                wifi_intf_stop_wps();
                break;

            case OID_WIFI_INTF_WPS_PIN:
                wifi_intf_start_wps((uint8_t *)NULL, 0, (uint8_t *)data, string_len_in_bytes );
                break;
#endif

            case OID_WIFI_UAP_DHCP_ADDR:
                {  
                    uint32_t addr;                                                        
                    addr = inet_addr( (char*)data );                                      
                    if ( addr == -1 )                                                     
                    {                                                                          
                        e_res = OID_ERROR_ILLEGAL_CHARS;                                       
                    }                                                                          
                    else                                                                       
                    {                                                                          
#ifdef __linux__
                        e_res = wifi_intf_udhcp_set_var_server_addr(active_if, addr);         
#else
                        e_res = dhcpd_set_var_server_addr(addr);         
#endif
                        if ( OK != e_res )                                                     
                        {                                                                      
                            e_res = OID_ERROR_ILLEGAL_CHARS;                                   
                        }                                                                      
                    }                                                                          
                }
                break;  

            case OID_WIFI_UAP_DHCP_SUBNET_MASK:
                {
                    uint32_t addr;                                                        
                    addr = inet_addr( (char*)data );                                      
                    if ( addr == -1 )                                                     
                    {                                                                          
                        e_res = OID_ERROR_ILLEGAL_CHARS;                                       
                    }                                                                          
                    else                                                                       
                    {                                                                          
#ifdef __linux__
                        e_res = wifi_intf_udhcp_set_var_subnet_mask(active_if, addr);         
#else
                        e_res = dhcpd_set_var_subnet_mask(addr);         
#endif
                        if ( OK != e_res )                                                     
                        {                                                                      
                            e_res = OID_ERROR_ILLEGAL_CHARS;                                   
                        }                                                                      
                    }                                                                          
                }
                break;                                                                     

            case OID_WIFI_UAP_DHCP_START_ADDR:
                {  
                    uint32_t addr;                                                        
                    addr = inet_addr( (char*)data );                                      
                    if ( addr == -1 )                                                     
                    {                                                                          
                        e_res = OID_ERROR_ILLEGAL_CHARS;                                       
                    }                                                                          
                    else                                                                       
                    {                                                                          
#ifdef __linux__
                        e_res = wifi_intf_udhcp_set_var_starting_addr(active_if, addr);
#else
                        e_res = dhcpd_set_var_start_addr(addr);         
#endif
                        if ( OK != e_res )                                                     
                        {                                                                      
                            e_res = OID_ERROR_ILLEGAL_CHARS;                                   
                        }                                                                      
                    }                                                                          
                }
                break;

            case OID_WIFI_UAP_DHCP_END_ADDR:
                {
                    uint32_t start_addr;
                    uint32_t end_addr;
                    end_addr = inet_addr( (char*)data );                                      
                    if ( end_addr == -1 )                                                     
                    {                                                                          
                        e_res = OID_ERROR_ILLEGAL_CHARS;                                       
                    }                                                                          
                    else                                                                       
                    {
#ifdef __linux__
                        if ( wifi_intf_udhcp_get_var_starting_addr(active_if, &start_addr) != OK )
#else
                        if ( dhcpd_get_var_start_addr(&start_addr) != OK )
#endif
                        {
                            e_res = FAIL;
                        }
                        else
                        {
                            // Verify addresses are in the same subnet
                            if ( ( start_addr & ~0xFF ) != ( end_addr & ~0xFF ) )
                            {
                                e_res = OID_ERROR_ILLEGAL_CHARS;
                            }
                            // Verify addresses end address is after the start address
                            else if ( ( start_addr & 0xFF ) < ( end_addr & 0xFF ) )
                            {
                                e_res = OID_ERROR_ILLEGAL_CHARS;
                            }
                            else
                            {
                                // Add one to cover fact that start address is the first valid address
#ifdef __linux__
                                e_res = wifi_intf_udhcp_set_var_num_addrs(active_if, end_addr - start_addr + 1 );
#else
                                e_res = dhcpd_set_var_num_addrs( end_addr - start_addr + 1 );
#endif
                            }
                        }
                    }
                }
                break;

#ifdef HAVE_WIRELESS_FILTERS
            case OID_WIFI_INTF_MAC_ADD_FILTER:
                e_res = wifi_intf_var_mac_filter_add( index, (char*)data );
                break;

            case OID_WIFI_INTF_MAC_REMOVE_FILTER:
                e_res = wifi_intf_var_mac_filter_remove( index, (char*)data );
                break;
#endif

            default:
                /* bad oid */
                e_res = FAIL;
                break;
        }
    }

    return e_res;
}


/**
 * 
 * \brief The backend oid wifi module function for retreiving
 * data associated with an OID.
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value Pointer to integer where the oid output is 
 *       stored
 *
 * \return One of the error_type_t return codes:
 * OK                     - Successful
 * FAIL                   - bad oid, other error
 *
 */
static error_type_t oid_module_wifi_get_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t * value)
{
    error_type_t e_res = NET_IFACE_OK;

    //noisy DBG_VERBOSE("%s(x%x, %d) -> x%x\n", __func__, (unsigned int)oid, index, (unsigned int)value);

    if( ( index != WIFI_IFACE_ID_STATION ) && 
        ( index !=WIFI_IFACE_ID_UAP ) )
    {
        e_res = OID_ERROR_ILLEGAL_CHARS;
    }
    if( OK == e_res ) 
    {
        unsigned int active_if = index; // get_active_net_if() lookup?
        switch ( oid )
        {
            case OID_WIFI_INTF_HW_ENABLED:
            {
                bool wifi_intf_enabled = false;
#ifdef __linux__
                e_res = wifi_hw_get_var_is_enabled(active_if, &wifi_intf_enabled);
#else
                e_res = wifi_hw_get_var_is_enabled(&wifi_intf_enabled);
#endif
                if(OK == e_res) 
                {
                    *value = (uint32_t)wifi_intf_enabled;
                }
                break;
            }
                
            case OID_WIFI_INTF_ENABLED:
            { 
                bool wifi_intf_enabled = false;
                e_res = wifi_intf_get_var_is_enabled(index, &wifi_intf_enabled);
                if(OK == e_res) 
                {
                    *value = (uint32_t)wifi_intf_enabled;
                }
                break;
            }

            case OID_WIFI_INTF_SEC_MODE:
            {
                wifi_intf_security_mode_t sec_mode = OPEN_SYSTEM;
                e_res = wifi_intf_get_var_sec_mode(index, &sec_mode);
                if(OK == e_res) 
                {
                    *value = (uint32_t)sec_mode;
                }
                break;
            }

            case OID_WIFI_INTF_WPA_ENCR_MODE:
            {
                wifi_intf_wpa_encryp_mode_t wpa_encr_mode = WPA_TKIP;
                e_res = wifi_intf_get_var_wpa_encryp_mode(index,&wpa_encr_mode );
                if(OK == e_res) 
                {
                    *value = (uint32_t)wpa_encr_mode;
                }

                break;
            }

            case OID_WIFI_INTF_WEP_AUTH_MODE:
            {
                wifi_intf_wep_auth_mode_t auth_mode = AUTH_OPEN;
                e_res= wifi_intf_get_var_wep_auth_mode(index, &auth_mode);
                if(OK == e_res) 
                {
                    *value = (uint32_t)auth_mode;
                }
                break;
            }

            case OID_WIFI_INTF_WEP_KEY_LEN:
            {
                uint8_t wep_key_len = 0;
                e_res = wifi_intf_get_var_wep_key_len(index, &wep_key_len);
                if(OK == e_res) 
                {
                    *value = (uint32_t)wep_key_len;
                }

                break;
            }

            case OID_WIFI_INTF_WEP_CUR_KEY:
            {
                uint8_t cur_wep_key = 0;

                e_res = wifi_intf_get_var_wep_cur_key(index, &cur_wep_key);
                if(OK == e_res) 
                {
                    *value = (uint32_t)cur_wep_key;
                }
                break;
            }

            case OID_WIFI_INTF_CHANNEL:
            {
                uint8_t channel = 0;
                e_res = wifi_intf_get_var_channel(index, &channel);
                if(OK == e_res) 
                {
                    *value = (uint32_t)channel;
                }
                break;
            }

            case OID_WIFI_INTF_SIGNAL_STRENGTH:
            {
                int32_t rssi = 0;
                e_res = wifi_intf_get_var_rssi_dbm(index, &rssi);
                if ( e_res == OK )
                {
                    *value = (uint32_t)rssi_dbm_to_signal_strength(rssi);  // output range 1-5
                }

                break;
            }

            case OID_WIFI_INTF_MAC_FILTER_MODE:
            {
                wifi_intf_mac_filter_mode_t filter = MAC_FILTER_MODE_DISABLED;
                e_res = wifi_intf_get_var_mac_filter_mode(index, &filter);
                if(OK == e_res) 
                {
                    *value = (uint32_t)filter;
                }
                break;
            }

            case OID_WIFI_INTF_MAC_FILTER_COUNT:
                {
                    uint8_t filter_count = 0;
                    e_res = wifi_intf_get_var_mac_filter_count(index, &filter_count);
                    if ( OK == e_res )
                    {
                        *value = (uint32_t)filter_count;
                    }
                }
                break;

            case OID_WIFI_INTF_COMM_MODE:
            {
                wifi_intf_comm_mode_t comm_mode = AD_HOC;
                e_res = wifi_intf_get_var_comm_mode(index, &comm_mode);
                if(OK == e_res) 
                {
                    *value = (uint32_t)comm_mode;
                }
                break;
            }

            case OID_WIFI_INTF_SSID_SUFFIX_LEN:
            {
                uint8_t len = MAX_SSID_NAME_LEN;
                uint8_t prefix[ MAX_SSID_NAME_LEN ]={0};
                e_res = wifi_intf_get_var_ssid_prefix((uint8_t)index, prefix, len);
                if ( e_res == OK )
                {
                    *value = MAX_SSID_NAME_LEN - strlen( (char*)prefix ) - LENGTH_OF_NULL;
                }
                break; 
            }
                
            case OID_WIFI_INTF_NUM_ATTACHED_DEVICES:
            {
                 uap_mlan_ds_sta_list sta_list;
                 *value = wifi_intf_get_var_station_list(WIFI_IFACE_ID_UAP, &sta_list);
                 break;
            }

            case OID_WIFI_INTF_LOCALE:
            {
                uint8_t region_code; 
#ifdef __linux__
                e_res = wifi_intf_get_var_hw_locale(active_if, &region_code); 
#else
                e_res = wifi_hw_get_var_region_code(&region_code); 
#endif
                if( e_res == OK)
                {
                    *value =  (uint32_t) region_code;
                }
                break;
            }

            case OID_WIFI_INTF_WLAN_STATUS:
                {
                    NET_WLAN_STATS *stats = net_iface_wlan_get_stats();
                    *value = stats->link_status;
                }
                break;

            case OID_WIFI_INTF_DOWNLINK_COUNT:
                {
                    NET_WLAN_STATS *stats = net_iface_wlan_get_stats();
                    *value = stats->down_link_count;
                }
                break;

            case OID_WIFI_UAP_DHCP_NUM_ADDRS:
                {
                    uint8_t num_addrs;
#ifdef __linux__
                    e_res = wifi_intf_udhcp_get_var_num_addrs(active_if, &num_addrs);
#else
                    e_res = dhcpd_get_var_num_addrs( &num_addrs );
#endif
                    if ( e_res == OK )
                    {
                        *value = num_addrs;
                    }
                }
                break;
               
            case OID_WIFI_UAP_DHCP_LEASE_TIME:
#ifdef __linux__
                e_res = wifi_intf_udhcp_get_var_lease_time(active_if, value );
#else
                e_res = dhcpd_get_var_lease_time( value );
#endif
                break;

            default:
            {
                /* bad oid */
                e_res = FAIL;

                break;
            }
        }
    }
    return e_res;
}


/**
 * 
 * \brief The backend oid wifi API module function for 
 * retreiving data associated with an OID. 
 *
 * \param[in] oid A valid oid_t created using the oid_make() macro.
 *
 * \param[in] index Index for oid
 *
 * \param[out] value New value for oid
 *
 * \return One of the error_type_t return codes:
 * OK - Successful 
 * OID_ERROR_VALUE_TOO_BIG 
 * OID_ERROR_VALUE_TOO_SMALL 
 * FAIL - bad oid, other error 
 *
 */
static error_type_t oid_module_wifi_set_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t value)
{
    error_type_t e_res;
    e_res = OK;
    unsigned int active_if = index; // get_active_net_if(); // active "mlan" or "uap" interface lookup?
    switch ( oid )
    {
        case OID_WIFI_INTF_HW_ENABLED:
        {
            uint8_t wifi_intf_enabled;
            wifi_intf_enabled = (uint8_t)value;
            if ( OK == e_res )
            {
                /* check range */
                if ( ( 0 != wifi_intf_enabled ) && ( 1 != wifi_intf_enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            if(OK == e_res) 
            {
#ifdef __linux__
                e_res = wifi_hw_set_var_is_enabled(active_if, wifi_intf_enabled);
#else
                e_res = wifi_hw_set_var_is_enabled(wifi_intf_enabled);
#endif
            }
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_ENABLED:
        {
            uint8_t wifi_intf_enabled;
            wifi_intf_enabled = (uint8_t)value;
            if ( OK == e_res )
            {
                /* check range */
                if ( ( 0 != wifi_intf_enabled ) && ( 1 != wifi_intf_enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            if(OK == e_res) 
            {
                e_res = wifi_intf_set_var_is_enabled(index, wifi_intf_enabled);
            }
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_SEC_MODE:
        {
            wifi_intf_security_mode_t sec_mode;
            sec_mode = (wifi_intf_security_mode_t)value;

            e_res = wifi_intf_set_var_sec_mode(index, sec_mode);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_WPA_ENCR_MODE:
        {
            wifi_intf_wpa_encryp_mode_t encr_mode;
            encr_mode = (wifi_intf_wpa_encryp_mode_t)value;

            e_res = wifi_intf_set_var_wpa_encryp_mode(index, encr_mode);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_WEP_AUTH_MODE:
        {
            wifi_intf_wep_auth_mode_t auth_mode;
            auth_mode = (wifi_intf_wep_auth_mode_t)value;

            e_res = wifi_intf_set_var_wep_auth_mode(index, auth_mode);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_WEP_KEY_LEN:
        {
            uint8_t key_len;
            key_len = (uint8_t)value;

            e_res = wifi_intf_set_var_wep_key_len(index, key_len);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_WEP_CUR_KEY:
        {
            uint8_t wep_cur_key;
            wep_cur_key = (uint8_t)value;

            e_res = wifi_intf_set_var_wep_cur_key(index, wep_cur_key);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_CHANNEL:
        {
            uint8_t channel;
            channel = (uint8_t)value;

            e_res = wifi_intf_set_var_channel(index, channel);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_MAC_FILTER_MODE:
        {
            uint8_t filter_mode;
            filter_mode = (uint8_t)value;

            e_res = wifi_intf_set_var_mac_filter_mode(index, filter_mode);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }

        case OID_WIFI_INTF_COMM_MODE:
        {
            wifi_intf_comm_mode_t comm_mode;
            comm_mode = (wifi_intf_comm_mode_t)value;
            e_res = wifi_intf_set_var_comm_mode(index, comm_mode);
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
        }

        case OID_WIFI_INTF_LOCALE:
        {
            uint8_t region;
            region = (uint8_t)value;
#ifdef __linux__
            e_res = wifi_intf_set_var_hw_locale(active_if, region);   
#else
            e_res = wifi_hw_set_var_region_code(region);   
#endif
            if(OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
        }

        case OID_WIFI_UAP_DHCP_NUM_ADDRS:
#ifdef __linux__
            e_res = wifi_intf_udhcp_set_var_num_addrs( active_if, ( uint8_t ) value );
#else
            e_res = dhcpd_set_var_num_addrs( ( uint8_t ) value );
#endif
            break;

        case OID_WIFI_UAP_DHCP_LEASE_TIME:
#ifdef __linux__
            e_res = wifi_intf_udhcp_set_var_lease_time(active_if, value );
#else
            e_res = dhcpd_set_var_lease_time( value );
#endif
            break;

        default:
            e_res = FAIL;
            break;
    }
    return e_res;
}


/**
 *
 * \brief Registers each Network API OID with the oid controller
 *
 */
void oid_register_module_wifi( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_WIFI_INTF_HW_ENABLED,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_ENABLED,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_SSID,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_SEC_MODE,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WPA_ENCR_MODE,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WPA_PASSPHRASE,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_AUTH_MODE,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_KEY_LEN,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_KEY_1,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_KEY_2,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_KEY_3,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_KEY_4,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_CUR_KEY,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WEP_CUR_KEY_VALUE,
                                    oid_module_wifi_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_CHANNEL,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_SIGNAL_STRENGTH,
                                    oid_module_wifi_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_SSID_PREFIX,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_FILTER_MODE,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_FILTER_COUNT,
                                    oid_module_wifi_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_WIRELESS_FILTERS
    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_ADD_FILTER,
                                    oid_module_null_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_REMOVE_FILTER,
                                    oid_module_null_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_FILTER_LIST_UAP,
                                    oid_module_wifi_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
#endif

    e_res = oid_register_callbacks( OID_WIFI_INTF_MAC_ADDR,
                                    oid_module_wifi_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_WPS
    e_res = oid_register_callbacks( OID_WIFI_INTF_START_WPS,
                                    oid_module_null_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_WPS_PIN,
                                    oid_module_null_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
#endif

    e_res = oid_register_callbacks( OID_WIFI_INTF_COMM_MODE,
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_ADDR,
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_ADDR_PREFIX,
                                    oid_module_wifi_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_SSID_SUFFIX_LEN,
                                    oid_module_wifi_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );        

    e_res = oid_register_callbacks( OID_WIFI_INTF_BSSID,
                                    oid_module_wifi_get,      
                                    oid_module_null_set,          
                                    oid_module_null_test );       
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_INTF_BSSID_BYTES,
                                    oid_module_wifi_get,      
                                    oid_module_null_set,          
                                    oid_module_null_test );       
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_INTF_NUM_ATTACHED_DEVICES,      
                                    oid_module_wifi_get_int,      
                                    oid_module_null_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_WIFI_INTF_LOCALE,      
                                    oid_module_wifi_get_int,      
                                    oid_module_wifi_set_int,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );       

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_SUBNET_MASK,      
                                    oid_module_wifi_get,      
                                    oid_module_wifi_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_START_ADDR,      
                                    oid_module_wifi_get,      
                                    oid_module_wifi_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_END_ADDR,      
                                    oid_module_wifi_get,
                                    oid_module_wifi_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_NUM_ADDRS,      
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );  

    e_res = oid_register_callbacks( OID_WIFI_UAP_DHCP_LEASE_TIME,      
                                    oid_module_wifi_get_int,
                                    oid_module_wifi_set_int,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );  
#ifdef HAVE_WPS
    e_res = oid_register_callbacks( OID_WIFI_INTF_GENERATE_WPS_PIN,
                                    oid_module_wifi_get,
                                    oid_module_null_set,
                                    oid_module_null_test);
    ASSERT( FAIL != e_res);
#endif

    e_res = oid_register_callbacks( OID_WIFI_INTF_WLAN_STATUS,      
                                    oid_module_wifi_get_int,      
                                    oid_module_null_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );       

    e_res = oid_register_callbacks( OID_WIFI_INTF_DOWNLINK_COUNT,      
                                    oid_module_wifi_get_int,      
                                    oid_module_null_set,      
                                    oid_module_null_test );   
    ASSERT( FAIL != e_res );       

}

