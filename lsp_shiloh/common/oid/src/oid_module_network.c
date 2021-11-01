/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "net_api.h"
#ifdef HAVE_PRINT_SUPPORT
#include "printvars_api.h"
#endif
#include "dprintf.h"
#include "lassert.h"
#include <pthread.h>
#include <posix_ostools.h>
#include "memAPI.h"
#include "http_api.h"

#ifdef HAVE_SLP
#include "net_slp_vars.h"
#endif
#ifdef HAVE_MDNS
#include "net_mdns_vars.h"
#endif
#ifdef HAVE_SNMP
#include "net_snmp_vars.h"
#endif
#ifdef HAVE_WSD
#include "net_wsd_vars.h"
#endif
#ifdef HAVE_UAP
#include "net_uap_api.h" // oid_register_module_uap
#endif

#ifdef HAVE_NETWORK
#include "NetworkIFModule.h"        //ABBA/OAK Network Interface Module
#endif

#ifdef __linux__
#include "net_iface.h" // net_interface_t
#include "net_iface_api.h"
#include "net_intf_api.h" // get_intf_mac_addr()
#define DBG_PRFX "oid.net: "
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
#include "net_debug.h" // DBG_ family macros
#endif

/** \brief The base we want to use for strtoul function */
#define BASE_10 10

// index can range from 0 -> number of interfaces
//
//    index == 0 returns 0 (1st offset implied)
//    index  > 0 returns if_nametoindex()-like index
//
#ifdef __linux__
int sizeof_intf_map = NET_MAX_INTERFACES; // default initially -- updated at bootup
inline
#endif

// lookup local intf table offset from it's current if_index
static int if_index_to_local_index(unsigned int if_index)
{
    int index = if_index; // zero passes through ..
#if __linux__
    {
        net_interface_t* intf = net_iface_if_index_to_local_context(if_index);
        if (!intf) return OID_ERROR_INDEX_INVALID;
        DBG_VERBOSE("%s(%d) %s -> offset %d\n", __func__, if_index, intf->name, intf->local_idx);
        index = intf->local_idx;
    }
#else
    index = if_index - 1; // 1-based -> 0-based, e.g, treck_idx -> local_idx
#endif
    return index;
}

// map if_index to local os-interface inventory table intf_mapp[] 
static int if_index_to_mapp_index(unsigned int if_index)
{
    int index = if_index; // zero passes through ..
#if __linux__
    if (if_index > 0)
    {
        index = 0; // start from the beginning of intf_mapp[]
        while (index < sizeof_intf_map)
        {
            if (if_index == intf_mapp[index].if_index) return index; // found
            index++;
        }
        if (index >= sizeof_intf_map) return OID_ERROR_INDEX_INVALID;
    }
#else
    index = if_index - 1; // 1-based -> 0-based, e.g, treck_idx -> local_idx
#endif
    return index;
}

// map current ifaces[index] to (dynamic) if_index
unsigned int current_if_index(uint32_t index)
{
    if (index < NET_MAX_INTERFACES)
    {
        net_interface_t* intf = g_net_context->ifaces[index];
        return intf? if_nametoindex(intf->name): 0;
    }
    return 0; // invalid if_index
}

#ifdef HAVE_IPV6
static error_type_t oid_module_find_ipv6_addr( net_iface_ipv6_config_method_t config_method,
                                               uint32_t index,
                                               net_iface_ipv6_addr_info_t *addr_info, 
                                               net_iface_ipv6_addr_t *ipv6_addr )
{
    error_type_t e_res;
    bool addr_found = false;
    net_iface_ipv6_addr_hndl_t addr_hndl;
    net_iface_ipv6_addr_hndl_t prev_addr_hndl;

    // TODO must account for multiple simultaneous interfaces
    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    e_res = net_iface_get_var_ipv6_preferred_addr( if_index, ipv6_addr, &addr_hndl );
    while ( ( OK == e_res ) && 
            ( NET_IFACE_IPV6_ADDR_UNSPECIFIED( *ipv6_addr ) == false ) ) 
    {
        e_res = net_iface_get_var_ipv6_addr_info( if_index, addr_info, addr_hndl );
        if ( ( OK == e_res ) && ( addr_info->config_method == config_method ) )
        {
            if ( index == 0 )
            {
                addr_found = true;
                break;
            }
            else
            {
                index--;
            }
        }

        prev_addr_hndl = addr_hndl;
        e_res = net_iface_get_var_ipv6_next_addr( if_index, ipv6_addr, &addr_hndl, prev_addr_hndl );
    }

    if ( ( addr_found == false ) ||
         ( OK != e_res ) )
    {
        e_res = FAIL;
    }

    return e_res;
}
#endif // HAVE_IPV6


/**
 * 
 * \brief The backend oid Network API module function for retreiving
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
static error_type_t oid_module_network_get( oid_t oid,
                                            uint32_t if_index,
                                            char * data_ptr,
                                            uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = NET_IFACE_OK;
//    unsigned int string_size;

    // lookup local intf table offset from it's current if_index
    int index = if_index_to_local_index(if_index);
    if (index < 0)
    {
        return e_res = OID_ERROR_INDEX_INVALID;
    }

    switch ( oid )
    {
        case OID_NETWORK_INTF_NAME:
        {
            refresh_iface_bindings(); // update os-specific intf_mapp & sizeof_intf_map
            int index = if_index_to_mapp_index(if_index);
            if (index < 0)
            {
                return e_res = OID_ERROR_INDEX_INVALID;
            }
        
            DBG_VERBOSE("%s() if_index %d -> mapp %d\n", __func__, if_index, index);
            char* if_name = intf_mapp[index].if_name;
            if (if_name && if_name[0])
            {
                strncpy(data_ptr, intf_mapp[index].if_name, max_string_len_in_bytes);
                e_res = OK;
            }
            else
            {
                *data_ptr = (char)0;
                e_res = FAIL;
            }
        } break;

        case OID_NETWORK_MAC_ADDRESS_BYTES:
        {
            uint8_t mac_addr[NET_IFACE_PHYS_ADDR_SIZE];
            if (index > sizeof_intf_map) // index (offset) not if_index
            {
                e_res = FAIL;
            }
            else
            {
                char* if_name = intf_mapp[index].if_name;
                if (if_name && if_name[0])
                {
                    get_intf_mac_addr(intf_mapp[index].if_name, mac_addr);
                    memcpy(data_ptr, mac_addr, MIN(sizeof(mac_addr),max_string_len_in_bytes));
                    e_res = NET_IFACE_OK;
                }
                else e_res = FAIL;
            }
        } break;

        case OID_NETWORK_MAC_ADDRESS:
        {
			char *macAddr = 0;

		    if( NIF_getMacAddr_str_wired(&macAddr) == NIF_Status_NotFound ){    
				sprintf(data_ptr,"00:00:00:00:00:00");
		        #ifdef PrintDebugLOG
		        printf("[%s]%d:getMacAddr_str_wired FAILED!\n",__FILE__,__LINE__);
		        #endif
		    }
			else{
				sprintf(data_ptr,"%s",macAddr);
			}
			free(macAddr);

			/*
            uint8_t mac_addr[NET_IFACE_PHYS_ADDR_SIZE];

#if __linux__ // stuff eth0's mac unconditionally
            get_intf_mac_addr(NET_ETH_LINK_NAME, mac_addr); // revisit map index->name
            e_res = NET_IFACE_OK;
#else
            e_res = net_iface_get_var_phys_addr(mac_addr, NET_IFACE_PHYS_ADDR_SIZE);
#endif

            if ( NET_IFACE_OK == e_res )
            {

                e_res = min_n_k_sprintf( data_ptr,
                        max_string_len_in_bytes,
                        &string_size,
                        "%02x-%02x-%02x-%02x-%02x-%02x",
                        mac_addr[0],
                        mac_addr[1],
                        mac_addr[2],
                        mac_addr[3],
                        mac_addr[4],
                        mac_addr[5] );

                ASSERT( string_size <= max_string_len_in_bytes );

                if ( DEBUG_STRING_CAPPED == e_res )
                {
                    e_res = OID_ERROR_VALUE_CAPPED;
                }
            }
			*/
            break;
        }

        case OID_NETWORK_HOST_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_HOSTNAME_SIZE );
            e_res = net_iface_get_var_hostname( if_index, data_ptr,  NET_IFACE_HOSTNAME_SIZE );
            break;

        case OID_NETWORK_HOST_NAME_DEFAULT:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_HOSTNAME_SIZE );
            e_res = net_iface_get_var_default_hostname(data_ptr,  NET_IFACE_HOSTNAME_SIZE );
            break;

        case OID_NETWORK_DOMAIN_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_DOMAINNAME_SIZE );
            e_res = net_iface_get_var_domain( if_index, data_ptr,  NET_IFACE_DOMAINNAME_SIZE  );
            break;

        case OID_NETWORK_DOMAIN_NAME_MANUAL:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_DOMAINNAME_SIZE );
            e_res = net_iface_get_var_manual_domain( if_index, data_ptr, NET_IFACE_DOMAINNAME_SIZE );
            break;

        case OID_NETWORK_IPV4_DOMAIN_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_DOMAINNAME_SIZE );
            e_res = net_iface_get_var_dhcpv4_domain( if_index, data_ptr, NET_IFACE_DOMAINNAME_SIZE );
            break;

        case OID_NETWORK_IPV4_HOST_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_HOSTNAME_SIZE );
            e_res = net_iface_get_var_dhcpv4_hostname( if_index, data_ptr,  NET_IFACE_HOSTNAME_SIZE );
            break;

        case OID_NETWORK_IPV4_DHCP_SERVER_ADDR:
        {
            uint32_t addr;

            e_res = net_iface_get_var_ipv4_boot_server( if_index, &addr );

            if ( NET_IFACE_OK == e_res )
            {
                inet_ntop(AF_INET, (void *)(&addr), data_ptr, INET_ADDRSTRLEN);
            }

            break;
        }

        case OID_NETWORK_IPV4_ADDRESS:
        {
			T_IPADDRv4 ipAddr;

		    if( NIF_getIPAddrV4_int_wired(&ipAddr) == NIF_Status_NotFound ){    
				sprintf(data_ptr,"000.000.000.000");
		        #ifdef PrintDebugLOG
		        printf("[%s]%d:getIPv4Addr_str_wired FAILED!\n",__FILE__,__LINE__);
		        #endif
		    }
			else{
				sprintf(data_ptr,"%d.%d.%d.%d",ipAddr.ipAddr[0],ipAddr.ipAddr[1],ipAddr.ipAddr[2],ipAddr.ipAddr[3]);
			}
			/*
            uint32_t ipv4_addr;

            e_res = net_iface_get_var_ipv4_addr( if_index, &ipv4_addr );

            if ( NET_IFACE_OK == e_res )
            {
                inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
            }
			*/
            break;
        }

        case OID_NETWORK_IPV4_SUBNET_MASK:
        {
			char *subnetmask = 0;

		    if( NIF_getNetmaskV4_str_wired(&subnetmask) == NIF_Status_NotFound ){    
				sprintf(data_ptr,"000.000.000.000");
		        #ifdef PrintDebugLOG
		        printf("[%s]%d:getNetmask_str_wired FAILED!\n",__FILE__,__LINE__);
		        #endif
		    }
			else{
				sprintf(data_ptr,"%s",subnetmask);
			}
			free(subnetmask);
/*
            uint32_t ipv4_addr;

            e_res = net_iface_get_var_ipv4_subnet_mask( if_index, &ipv4_addr );

            if ( NET_IFACE_OK == e_res )
            {
                inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
            }
*/
            break;
        }

        case OID_NETWORK_IPV4_WINS_SERVER:
        {
            uint32_t ipv4_addr;

            e_res = net_iface_get_var_ipv4_wins_addr( if_index, &ipv4_addr );

            if ( NET_IFACE_OK == e_res )
            {
                inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
            }

            break;
        }

        case OID_NETWORK_IPV4_DEFAULT_GATEWAY:
        {
			#define WIRED_IF_NAME "eth0"
			if (NIF_getDHCPStatus_wired() != NIF_Status_Static){
				FILE *fp;
				char gateway[32];

				fp = popen("ip route show default | grep eth0 | awk '/default/ {print $3}'","r");

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
			}
			else{
				char *GatewayWired=0;
				if (NIF_getGatewayV4FromFile(WIRED_IF_NAME,&GatewayWired) == NIF_Status_NotFound){
					sprintf(data_ptr,"000.000.000.000");
				}
				else{
					sprintf(data_ptr,"%s",GatewayWired);
				}
				free(GatewayWired);
			}

/*
		   uint32_t ipv4_addr;

            e_res = net_iface_get_var_ipv4_gateway( if_index, &ipv4_addr );

            if ( NET_IFACE_OK == e_res )
            {
                inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
            }
*/
            break;
        }

#ifdef HAVE_SNMP
            case OID_NETWORK_SNMP_COMMUNITY_SET_STRING:
                {
                    uint8_t len;
                    len =127; //MAX UINT8 value
                    e_res = net_snmp_get_var_user_write_community_name( data_ptr,  len);
                    break;
                }

            case OID_NETWORK_SNMP_COMMUNITY_GET_STRING:
                {
                    uint8_t len;
                    len =127; //MAX UINT8 value
                    e_res = net_snmp_get_var_user_read_community_name( data_ptr,  len);
                    break;
                }

#endif // HAVE_SNMP

            case OID_NETWORK_IP_ADDRESS_FROM_CONNECTION:
            {
#ifdef __linux__
                char* link_addr = get_intf_ipv4_addr(NET_ETH_LINK_NAME);
                strncpy(data_ptr, (link_addr ? link_addr : "0.0.0.0"), max_string_len_in_bytes);
                data_ptr[max_string_len_in_bytes-1] = (char)0; // c-string
                e_res = OK;
#else
                struct sockaddr_storage *from_addr;
                int32_t size;

                size = sizeof( struct sockaddr_storage );
                from_addr = ( struct sockaddr_storage* )malloc( size );
                if ( from_addr == NULL )
                {
                    e_res = FAIL;
                    break;
                }
                memset( from_addr, 0x00, size );

                e_res = http_resource_get_sock_addr( ( http_request_t  )index, from_addr );
                if ( e_res != OK )
                {
                    uint32_t ipv4_addr;
                    e_res = net_iface_get_var_ipv4_addr( if_index, &ipv4_addr );
                    if ( NET_IFACE_OK == e_res )
                    {
                        inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
                    }
                }
                else
                {
                    #ifdef HAVE_IPV6
                    if ( ( from_addr->addr.ipv6.sin6_family == AF_INET6 ) &&
                         ( from_addr->addr.ipv6.sin6_addr.ip6Addr.ip6U32[ 0 ] != 0 ) )
                    {   
                        inet_ntop(AF_INET6, (void *)(&from_addr->addr.ipv6.sin6_addr), data_ptr, INET6_ADDRSTRLEN);
                    }
                    else
                    {
                        inet_ntop(AF_INET, (void *)(&from_addr->addr.ipv6.sin6_addr.ip6Addr.ip6U32[3]), data_ptr, INET6_ADDRSTRLEN);
                    }
                    #else
                    inet_ntop(AF_INET, (void *)(&from_addr->addr.ipv4.sin_addr), data_ptr, INET_ADDRSTRLEN);
                    #endif // HAVE_IPV6
                }
                free( from_addr );
                break;
#endif
            }

#ifdef HAVE_IPV6
        case OID_NETWORK_IPV6_DOMAIN_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_DOMAINNAME_SIZE );
            e_res = net_iface_get_var_dhcpv6_domain( if_index, data_ptr, NET_IFACE_DOMAINNAME_SIZE );
            break;

        case OID_NETWORK_IPV6_HOST_NAME:
            ASSERT( max_string_len_in_bytes >  NET_IFACE_HOSTNAME_SIZE );
            e_res = net_iface_get_var_dhcpv6_hostname( if_index, data_ptr, NET_IFACE_HOSTNAME_SIZE );
            break;

        case OID_NETWORK_IPV6_LINK_LOCAL_ADDRESS:
            {
                net_iface_ipv6_addr_t ipv6_addr;
                net_iface_ipv6_addr_hndl_t addr_hndl;

                e_res = net_iface_get_var_ipv6_link_local_addr( if_index, &ipv6_addr, &addr_hndl );
                if ( OK == e_res )
                {
                    ASSERT( max_string_len_in_bytes > INET6_ADDRSTRLEN );
                    inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), data_ptr, INET6_ADDRSTRLEN);
                }
            }
            break;

        case OID_NETWORK_IPV6_DHCPV6_ADDRESS:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;
                data_ptr[0] = '\0';
                // REVISIT TODO validate index is an acceptable range - rowberry
                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_DHCPV6, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    ASSERT( max_string_len_in_bytes > INET6_ADDRSTRLEN );
                    inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), data_ptr, INET6_ADDRSTRLEN);
                }
            }
            break;

        case OID_NETWORK_IPV6_STATELESS_ADDRESS:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;
                                                              
                // REVISIT TODO validate index is an acceptable range - rowberry
                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_STATELESS,
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    ASSERT( max_string_len_in_bytes > INET6_ADDRSTRLEN );
                    inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), data_ptr, INET6_ADDRSTRLEN);
                }

            }
            break;

    case OID_NETWORK_IPV6_PREFERRED_ADDR:
         {
            net_iface_ipv6_addr_t      ipv6_addr;
            net_iface_ipv6_addr_hndl_t addr_hndl;

            e_res = net_iface_get_var_ipv6_preferred_addr(if_index, &ipv6_addr,&addr_hndl);
            if ( ( NET_IFACE_OK == e_res ) && ( NET_IFACE_IPV6_ADDR_UNSPECIFIED( ipv6_addr ) == false ) )
            {
                ASSERT( max_string_len_in_bytes > INET6_ADDRSTRLEN );
                inet_ntop(AF_INET6, (void *)(&ipv6_addr.addr), data_ptr, INET6_ADDRSTRLEN);
            }
            else
            {
                data_ptr[ 0 ] = '\0';
            }
        }
        break;  
             


#endif // HAVE_IPV6


        case OID_NETWORK_DNS_ADDR:
            {
                uint32_t ipv4_addr;
                //uint8_t dns_index;
                //dns_index = ( uint8_t )index;
                e_res = net_iface_get_var_dns_server_addr( if_index, index, &ipv4_addr );
                if ( NET_IFACE_OK == e_res )
                {
                    inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
                }
            }
            break;

#ifdef TM_USE_PPPOE_CLIENT
        case OID_NETWORK_PPPOE_USERNAME:
            e_res = net_iface_get_var_pppoe_username(if_index, data_ptr, max_string_len_in_bytes);
            break;

        case OID_NETWORK_PPPOE_PASSWORD:
            e_res = net_iface_get_var_pppoe_password(if_index, data_ptr, max_string_len_in_bytes);
            break;
#endif // TM_USE_PPPOE_CLIENT

        default:
            {
                /* bad oid */
                e_res = FAIL;

                break;
            }
        }

    return e_res;
}


/**
 * 
 * \brief The backend oid module function for the network module set
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
error_type_t oid_module_network_set( oid_t oid,
        uint32_t if_index,
        const char * data,
        uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    // lookup local intf table offset from it's current if_index
    int index = if_index_to_local_index(if_index);
    if (index < 0)
    {
        return e_res = OID_ERROR_INDEX_INVALID;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
            case OID_NETWORK_HOST_NAME:
            {
                /* too large */
                if ( string_len_in_bytes > NET_IFACE_HOSTNAME_SIZE )
                {
                    e_res = OID_ERROR_VALUE_TOO_BIG;
                }

                if ( OK == e_res )
                {
                    e_res = net_iface_set_var_manual_hostname(if_index, data);
                    if (  OK == e_res )
                    {
                        net_iface_config_origin_t origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
                        e_res = net_iface_set_var_hostname_preferred_origin(if_index, &origin);
                    }
                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

            case OID_NETWORK_DOMAIN_NAME_MANUAL:
            {
                /* too large */
                if ( string_len_in_bytes > NET_IFACE_DOMAINNAME_SIZE )
                {
                    e_res = OID_ERROR_VALUE_TOO_BIG;
                }

                if ( OK == e_res )
                {
                    e_res = net_iface_set_var_manual_domain(if_index, data);
                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }

                    net_iface_config_origin_t origin = NET_IFACE_CONFIG_ORIGIN_MANUAL;
                    e_res = net_iface_set_var_domain_preferred_origin(if_index, &origin);
                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_ADDRESS:
            {
                uint32_t ipv4_addr;
                ipv4_addr = inet_addr( (char*)data );

                if ( ipv4_addr == -1 )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                else
                {
                    e_res = net_iface_set_var_ipv4_manual_addr(if_index, &ipv4_addr);

                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_WINS_SERVER:
            {
                uint32_t ipv4_addr;
                ipv4_addr = inet_addr( (char*)data );

                if ( ipv4_addr == -1 )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                else
                {
                    e_res = net_iface_set_var_ipv4_manual_wins_addr(if_index, &ipv4_addr);

                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_SUBNET_MASK:
            {
                uint32_t ipv4_addr;
                ipv4_addr = inet_addr( (char*)data );

                if ( ipv4_addr == -1 )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                else
                {
                    e_res = net_iface_set_var_ipv4_manual_subnet_mask(if_index, &ipv4_addr);

                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

            case OID_NETWORK_IPV4_DEFAULT_GATEWAY:
            {
                uint32_t ipv4_addr;
                ipv4_addr = inet_addr( (char*)data );

                if ( ipv4_addr == -1 )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                else
                {
                    e_res = net_iface_set_var_ipv4_manual_gateway(if_index, &ipv4_addr);

                    if ( OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }

#ifdef HAVE_SNMP
            case OID_NETWORK_SNMP_COMMUNITY_SET_STRING:
                e_res = net_snmp_set_var_user_write_community_name(data);
                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
				break;

            case OID_NETWORK_SNMP_COMMUNITY_GET_STRING:
                e_res = net_snmp_set_var_user_read_community_name(data);
                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
				break;
#endif // HAVE_SNMP
     
            case OID_NETWORK_DNS_ADDR:
                {
                    uint32_t ipv4_addr;
                    ipv4_addr = inet_addr( (char*)data );
                    if ( ipv4_addr == -1 )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                    else
                    {
                        uint8_t dns_index;
                        dns_index = (uint8_t)index;
                        if ( net_iface_set_var_manual_dns_addr( if_index, dns_index, &ipv4_addr ) != OK )
                        {
                            e_res = OID_ERROR_ILLEGAL_CHARS;
                        }
                    }
                }
                break;

#ifdef TM_USE_PPPOE_CLIENT
            case OID_NETWORK_PPPOE_USERNAME:
                e_res = net_iface_set_var_pppoe_username(if_index, (char*)data);
                break;

            case OID_NETWORK_PPPOE_PASSWORD:
                e_res = net_iface_set_var_pppoe_password(if_index, (char*)data);
                break;
#endif // TM_USE_PPPOE_CLIENT

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
 * \brief The backend oid network module function for retreiving
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
static error_type_t oid_module_network_get_int( oid_t oid,
                                                uint32_t if_index,
                                                uint32_t * value)
{
    error_type_t e_res;
    e_res = OK;

    // lookup local intf table offset from it's current if_index
    int index = if_index_to_local_index(if_index);
    if (index < 0)
    {
        DBG_VERBOSE("%s(%d) Invalid index (offset %d)\n", __func__, if_index, index);
        return e_res = OID_ERROR_INDEX_INVALID;
    }

    switch ( oid )
    {
        case OID_NETWORK_HOST_NAME_ORIGIN:
        {
            net_iface_config_origin_t origin;
            e_res = net_iface_get_var_hostname_origin( if_index, &origin );
            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)origin;
            }
            break;
        }

        case OID_NETWORK_STATUS:
        {
            net_iface_status_t link_status;

            e_res = net_iface_get_var_network_status( if_index, &link_status );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)link_status;
            }
            break;
        }
        

        case OID_NETWORK_CONNECTION_TIMEOUT:
        {
            uint16_t timeout;
            /* timeout is in seconds */
            e_res = platvars_get_io_timeout( &timeout );
            if ( OK == e_res )
            {
                *value = (uint32_t)timeout;
            }

            break;
        }

#if defined HAVE_ETHERNET
        case OID_NETWORK_LINK_SPEED:
        {
            eth_mac_link_status_t link_status;

            e_res = net_eth_get_var_link_status( &link_status );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)link_status;
            }

            break;
        }

        case OID_NETWORK_LINK_CONFIG:
        {
            eth_mac_link_config_t config;

            e_res = net_eth_get_var_link_config( &config );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)config;
            }

            break;
        }
#endif // HAVE_ETHERNET

        case OID_NETWORK_LPD_ENABLED:
        {
            bool lpd_enabled;

//            e_res = netapps_get_var_lpd_enabled( &lpd_enabled );
            // TODO implement this
            lpd_enabled = true;

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)lpd_enabled;
            }

            break;
        }
// TODO move to SLP module
#ifdef HAVE_SLP 
        case OID_NETWORK_SLP_ENABLED:
        {
            bool slp_enabled;

            e_res = net_slp_get_var_enabled( &slp_enabled );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)slp_enabled;
            }

            break;
        }
#endif

        case OID_NETWORK_DOMAIN_NAME_ORIGIN:
            {
                net_iface_config_origin_t origin;
                e_res = net_iface_get_var_domain_origin( if_index, &origin );
                if ( NET_IFACE_OK == e_res )
                {
                    *value = (uint32_t)origin;
                }
            }
            break;

	case OID_NETWORK_HOST_NAME_PREF_ORIGIN:                                         
	    {                                                                             
		  net_iface_config_origin_t origin;                                         
		  e_res = net_iface_get_var_hostname_preferred_origin( if_index, &origin );   
		  if ( NET_IFACE_OK == e_res )                                              
		{                                                                         
		      *value = (uint32_t)origin;                                            
		}                                                                         
	    }                                                                             
	    break;                                                                        																							   
        case OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN:
            {
                net_iface_config_origin_t origin;
                e_res = net_iface_get_var_domain_preferred_origin( if_index, &origin );
                if ( NET_IFACE_OK == e_res )
                {
                    *value = (uint32_t)origin;
                }
            }
            break;

        case OID_NETWORK_IPV4_ENABLED:
        {
            bool ipv4_enabled;

            e_res = net_iface_get_var_ipv4_enabled( if_index, &ipv4_enabled );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)ipv4_enabled;
            }

            break;
        }

        case OID_NETWORK_IPV4_WINS_CONFIGURED:
        case OID_NETWORK_IPV4_LAST_CONFIGURED_BY:
        {
            net_iface_ipv4_config_method_t method;

            e_res = net_iface_get_var_ipv4_last_config_method( if_index, &method );

            if ( NET_IFACE_OK == e_res )
            {
                *value = method;
            }

            break;
        }

        case OID_NETWORK_IPV4_MANUAL_CONFIG:
        {
            net_iface_ipv4_config_options_t flags;

            e_res = net_iface_get_var_ipv4_config_options( if_index, &flags );
            if ( NET_IFACE_OK == e_res )
            {
                if ( flags & NET_IFACE_IPV4_USE_MANUAL )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }
            }

            break;
        }

        case OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG:
        {
            net_iface_ipv4_config_options_t flags;

            e_res = net_iface_get_var_ipv4_config_options( if_index, &flags );
            if ( NET_IFACE_OK == e_res )
            {
                if ( flags & NET_IFACE_IPV4_USE_DHCP )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }
            }

            break;
        }

        case OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG:
        {
            net_iface_ipv4_config_options_t flags;

            e_res = net_iface_get_var_ipv4_config_options( if_index, &flags );

            if ( NET_IFACE_OK == e_res )
            {
                if ( flags & NET_IFACE_IPV4_USE_BOOTP )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }
            }

            break;
        }

        case OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG:
        {
            net_iface_ipv4_config_options_t flags;

            e_res = net_iface_get_var_ipv4_config_options( if_index, &flags );
            if ( NET_IFACE_OK == e_res )
            {
                if ( flags & NET_IFACE_IPV4_USE_AUTOIP )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }
            }

            break;
        }

        case OID_NETWORK_IPV4_DHCP_LEASE_TIME:
        {
            uint32_t lease_remaining;

            e_res = net_iface_get_var_dhcpv4_lease_remaining( if_index, &lease_remaining );

            if ( NET_IFACE_OK == e_res )
            {
                *value = lease_remaining;
            }

            break;
        }
        case OID_NETWORK_IPV6_SUPPORT:
            #ifdef HAVE_IPV6
            *value = 1;
            #else
            *value = 0;
            #endif // HAVE_IPV6
            break;

#ifdef HAVE_IPV6
        case OID_NETWORK_IPV6_STATUS:   
            {
                net_iface_status_t link_status;

                e_res = net_iface_get_var_network_status( if_index, &link_status );

                if ( NET_IFACE_OK == e_res )
                {
                    *value = (uint32_t)link_status;
                }
                break;
            }

        case OID_NETWORK_IPV6_ENABLED:
            {
                bool enabled;
                e_res = net_iface_get_var_ipv6_enabled( if_index, &enabled );
                if ( OK == e_res )
                {
                    *value = (uint32_t)enabled;
                }
            }
            break;

        case OID_NETWORK_IPV6_LINK_LOCAL_PREFIX_LEN:
            {
                net_iface_ipv6_addr_t ipv6_addr;
                net_iface_ipv6_addr_hndl_t addr_hndl;

                e_res = net_iface_get_var_ipv6_link_local_addr( if_index, &ipv6_addr, &addr_hndl );
                if ( OK == e_res )
                {
                    *value = (uint32_t) ipv6_addr.prefix;
                }
            }
            break;

        case OID_NETWORK_IPV6_DHCPV6_PREFIX_LEN:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_DHCPV6, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = (uint32_t)ipv6_addr.prefix;
                }
            }
            break;

        case OID_NETWORK_IPV6_DHCPV6_PREFERRED_LIFETIME:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_DHCPV6, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = addr_info.preferred_lifetime_remaining;
                }
            }
            break;

        case OID_NETWORK_IPV6_DHCPV6_VALID_LIFETIME:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_DHCPV6, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = addr_info.valid_lifetime_remaining;
                }
            }
            break;

        case OID_NETWORK_IPV6_STATELESS_PREFIX_LEN:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_STATELESS, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = (uint32_t)ipv6_addr.prefix;
                }
            }
            break;

        case OID_NETWORK_IPV6_STATELESS_PREFERRED_LIFETIME:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_STATELESS, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = addr_info.preferred_lifetime_remaining;
                }
            }
            break;

        case OID_NETWORK_IPV6_STATELESS_VALID_LIFETIME:
            {
                net_iface_ipv6_addr_info_t addr_info;
                net_iface_ipv6_addr_t ipv6_addr;

                e_res = oid_module_find_ipv6_addr( NET_IFACE_IPV6_CONFIGURED_STATELESS, 
                                                   index, 
                                                   &addr_info,
                                                   &ipv6_addr );
                if ( OK == e_res )
                {
                    *value = addr_info.valid_lifetime_remaining;
                }
            }
            break;

        case OID_NETWORK_DHCPV6_ENABLED:
            {
                net_iface_ipv6_config_options_t options;
                e_res = net_iface_get_var_ipv6_config_options( if_index, &options );
                if ( OK == e_res )
                {
                    if ( options & NET_IFACE_IPV6_USE_DHCPV6 )
                    {
                        *value = 1;
                    }
                    else
                    {
                        *value = 0;
                    }
                }
            }
            break;

#endif // HAVE_IPV6


// TODO move to mDNS module
#ifdef HAVE_MDNS
        case OID_NETWORK_BONJOUR_ENABLED:
        {
            bool mdns_enabled;
            e_res = net_mdns_get_var_enabled( index, &mdns_enabled );

            if ( NET_IFACE_OK == e_res )
            {
                *value = (uint32_t)mdns_enabled;
            }

            break;
        }
#endif 
                                                            // Variable names from eth_mac_api.h
        case OID_NETWORK_STAT_TOT_PACKETS:                  // rxPktCount + txPktCount
        case OID_NETWORK_STAT_TOT_PACKETS_RECV:             // rxPktCount
        case OID_NETWORK_STAT_TOT_PACKETS_XMITTED:          // txPktCount
        case OID_NETWORK_STAT_UNICAST_PACKETS:              // rxUniPktCount + txUniPktCount
        case OID_NETWORK_STAT_UNICAST_PACKETS_RECV:         // rxUniPktCount
        case OID_NETWORK_STAT_UNICAST_PACKETS_XMITTED:      // txUniPktCount
        case OID_NETWORK_STAT_FRAMING_ERRORS:               // rxFrameErrors + ???
        case OID_NETWORK_STAT_FRAMING_ERRORS_RECV:          // rxFrameErrors
        case OID_NETWORK_STAT_FRAMING_ERRORS_XMITTED:       // ???
        case OID_NETWORK_STAT_BAD_PACKETS:                  // rxErrors + txErrors
        case OID_NETWORK_STAT_BAD_PACKETS_RECV:             // rxErrors
        case OID_NETWORK_STAT_BAD_PACKETS_XMITTED:          // txErrors
        case OID_NETWORK_STAT_COLLISIONS_XMITTED:           // txCollisionErrors
        case OID_NETWORK_STAT_COLLISIONS_XMITTED_LATE:      // txLateCollisionErrors
        case OID_NETWORK_STAT_BCAST_PACKETS_XMITTED:
        case OID_NETWORK_STAT_BCAST_PACKETS_RECV:
        {
    //            net_iface_stats_t stats;
            uint32_t val;
    
    //            e_res = net_iface_get_var_statistics( &stats );
            // TODO implement this
            val = 0;
    
            if ( NET_IFACE_OK == e_res )
            {
                *value = val;
    //                        ( stats.tx_total + stats.rx_total ) );
            }
            break;
        }
    
#ifdef HAVE_SNMP
        case OID_NETWORK_SNMP_ENABLED:
        {
            bool snmp_enabled;
            e_res = net_snmp_get_var_enabled( &snmp_enabled );
            if ( OK == e_res )
            {
                *value = (uint32_t)snmp_enabled;
            }
    
            break;
        }
    
        case OID_NETWORK_SNMP_RW_ENABLED:
        {
            net_snmp_access_mode_t snmp_access;
    
            e_res = net_snmp_get_var_access_mode( &snmp_access );
    
            if ( OK == e_res )
            {
                if ( NET_SNMP_ACCESS_READ_WRITE == snmp_access )
                {
                    *value = 1;
                }
                else
                {
                    *value = 0;
                }
            }
    
            break;
        }

        case OID_NETWORK_SNMP_PUBLIC_ACCESS_EN:
        {
            bool rw_enabled;
    
            // return 'enabled' if either read or write or both are enabled
            e_res = net_snmp_get_var_default_read_community_enabled( &rw_enabled );
            if ( OK == e_res && !rw_enabled)
            {
                e_res = net_snmp_get_var_default_write_community_enabled( &rw_enabled );
            }
    
            if ( OK == e_res )
            {
                *value = (uint32_t)rw_enabled;
            }
    
            break;
        }
#endif // HAVE_SNMP
    
#ifdef HAVE_WSD
        case OID_NETWORK_WSD_ENABLED:
        {
            bool enabled;
            e_res = net_wsd_get_var_enabled( &enabled );
            if ( OK == e_res )
            {
                *value = (uint32_t)enabled;
            }
            break;
        }
#endif // HAVE_WSD

        case OID_NETWORK_IN_USE: // currently active?
            {
                // TODO net_iface_link_type_t is deprecated -- use direct mapping to link name via OEM config
                net_iface_link_type_t link_type;
                char link_name[NET_LINK_NAME_SIZE];
                unsigned int link_index;
                net_iface_get_var_active_link(if_index, &link_index);
                net_link_get_name(link_index, link_name, sizeof(link_name));
                link_type = net_link_name_to_type(link_name);
                if ( OK == e_res )
                {
                    *value = (uint32_t)link_type;
                }
             break;
            }
    
#ifdef TM_USE_PPPOE_CLIENT
        case OID_NETWORK_PPPOE_ENABLE:
            {
                bool enabled;
                e_res = net_iface_get_var_pppoe_enabled(if_index, &enabled);
                if ( e_res == OK )
                {
                    *value = (uint32_t)enabled;
                }
            }
            break;

        case OID_NETWORK_PPPOE_STATE:
            e_res = net_iface_get_var_pppoe_state( if_index, value );
            break;
#endif // TM_USE_PPPOE_CLIENT

        case OID_NETWORK_INTF_COUNT:
        {
            e_res = OK;
            if (index > sizeof_intf_map)
            {
                e_res = FAIL;
                DBG_ERR("OID_NETWORK_INTF_COUNT[%d] undefined\n", index);
            }
            else
            {
                if (sizeof_intf_map > NET_MAX_INTERFACES)
                {
                    DBG_ERR("OID_NETWORK_INTF_COUNT[%d] %d -> %d\n", index, sizeof_intf_map, NET_MAX_INTERFACES);
                    sizeof_intf_map = NET_MAX_INTERFACES; // constrain
                }
                *value = (uint32_t)sizeof_intf_map;
            }
        } break;

        case OID_NETWORK_INTF_MAP: // return if_index
        {
            e_res = OK;
            refresh_iface_bindings(); // dynamic update os-specific intf_mapp & sizeof_intf_map
            if (index > sizeof_intf_map)
            {
                e_res = OID_ERROR_INDEX_INVALID;
                DBG_ERR("OID_NETWORK_INTF_MAP[%d] undefined\n", index);
            }
            else
            {
                *value = (uint32_t)intf_mapp[index].if_index; // cast
                DBG_VERBOSE("OID_NETWORK_INTF_MAP[%d] if_index %d (%d)\n", *value, intf_mapp[index].if_index);
            }
        } break;

        case OID_NETWORK_INTF_ACTIVE:
        {
            e_res = FAIL;
            if (value) // resultant?
            {
                char if_name[IFNAMSIZ];
                char* active = if_indextoname(if_index, if_name);
                *value = 0; // bool false
                if (if_name == active)
                {
                    *value = 1; // bool true
                    DBG_VERBOSE("OID_NETWORK_INTF_ACTIVE[%d] if_name %s\n", if_index, if_name);
                }
                e_res = OK;
            }
        } break;

        default:
        {
            /* bad oid */
            e_res = FAIL;
            break;
        }
    }

    return e_res;
}

/**
 * 
 * \brief The backend oid network API module function for 
 * retrieving data associated with an OID. 
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
static error_type_t oid_module_network_set_int( oid_t oid,
                                                uint32_t if_index,
                                                uint32_t value)
{
    error_type_t e_res;
    e_res = OK;    

    // lookup local intf table offset from it's current if_index
    int index = if_index_to_local_index(if_index);
    if (index < 0)
    {
        return e_res = OID_ERROR_INDEX_INVALID;
    }

    switch ( oid )
    {
        case OID_NETWORK_CONNECTION_TIMEOUT:
        {
            uint16_t timeout;
            timeout = (uint16_t)value;

            e_res = platvars_set_io_timeout( &timeout );
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
        }

#if defined HAVE_ETHERNET
        case OID_NETWORK_LINK_SPEED:
        {
            eth_mac_link_config_t link_speed;
            link_speed = (eth_mac_link_config_t)value;

            /* now set the link speed and write it back */
            e_res = net_eth_set_var_link_config( link_speed );

            if ( NET_IFACE_OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
        }

        case OID_NETWORK_LINK_CONFIG:
        {
            eth_mac_link_config_t link_config;
            link_config = (eth_mac_link_config_t)value;

            /* now set the link config and write it back */
            e_res = net_eth_set_var_link_config( link_config );

            if ( NET_IFACE_OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
            break;
        }
#endif // HAVE_ETHERNET

        case OID_NETWORK_LPD_ENABLED:
        {
            uint8_t enabled;

            enabled = (uint8_t)value;
            /* check range */
            if ( ( 0 != enabled ) && ( 1 != enabled ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            if ( OK == e_res )
            {
//                    e_res = netapps_set_var_lpd_enabled( (const uint8_t *)&enabled );
                // TODO implement this
                e_res = OK;
            }

            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }
// TODO move to SLP module
#ifdef HAVE_SLP 
        case OID_NETWORK_SLP_ENABLED:
        {
            bool enabled;
            enabled = (uint8_t)value;

            /* check range */
            if ( ( 0 != enabled ) && ( 1 != enabled ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            if ( OK == e_res )
            {
                e_res = net_slp_set_var_enabled( enabled );
            }

            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }
#endif // SLP
	case OID_NETWORK_HOST_NAME_PREF_ORIGIN:                                              
	     {                                                                                  
	     	   net_iface_config_origin_t origin;                                              
	     	   origin = (net_iface_config_origin_t)value;                                     
	     	   e_res = net_iface_set_var_hostname_preferred_origin( if_index, &origin );    
	     }                                                                                  
	      break; 

        case OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN:
            {
                net_iface_config_origin_t origin;
                origin = (net_iface_config_origin_t)value;
                e_res = net_iface_set_var_domain_preferred_origin( if_index, &origin );
            }
            break;

        case OID_NETWORK_RESTORE_HOST_DOMAIN_NAMES:
            // REVISIT TODO - how does this work in Unity?? rowberry
            break;

        case OID_NETWORK_IPV4_ENABLED:
        {
            bool ipv4_enabled;

            if ( ( 0 != value ) && ( 1 != value ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            if ( OK == e_res )
            {
                ipv4_enabled = (bool)value;
                e_res = net_iface_set_var_ipv4_enabled( if_index, ipv4_enabled );
            }
            break;
        }

        case OID_NETWORK_IPV4_MANUAL_CONFIG:
        {
            net_iface_ipv4_config_options_t config_options;
            bool use_feature;

            /* first get the network config options */
            e_res = net_iface_get_var_ipv4_config_options( if_index, &config_options );
            if ( NET_IFACE_OK == e_res )
            {
                /* check range */
                if ( ( 0 != value ) && ( 1 != value ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }

            if ( NET_IFACE_OK == e_res )
            {
                use_feature = (bool)value;

                /* clear the boot field */
                config_options &= ~( NET_IFACE_IPV4_USE_MANUAL );
                /* then set it to the new value */
                if ( true == use_feature )
                {
                    config_options |= ( NET_IFACE_IPV4_USE_MANUAL );
                }

                e_res = net_iface_set_var_ipv4_config_options( if_index, &config_options );
                if ( NET_IFACE_OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            break;
        }

        case OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG:
        {
            net_iface_ipv4_config_options_t config_options;
            bool use_feature;

            /* first get the network config options */
            e_res = net_iface_get_var_ipv4_config_options( if_index, &config_options );
            if ( NET_IFACE_OK == e_res )
            {
                /* check range */
                if ( ( 0 != value ) && ( 1 != value ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }

            if ( NET_IFACE_OK == e_res )
            {
                use_feature = (bool)value;

                /* clear the boot field */
                config_options &= ~( NET_IFACE_IPV4_USE_DHCP );
                /* then set it to the new value */
                if ( true == use_feature )
                {
                    config_options |= ( NET_IFACE_IPV4_USE_DHCP );
                }

                e_res = net_iface_set_var_ipv4_config_options( if_index, &config_options );
                if ( NET_IFACE_OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            break;

        }

        case OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG:
        {
            net_iface_ipv4_config_options_t config_options;
            bool use_feature;

            /* first get the network config options */
            e_res = net_iface_get_var_ipv4_config_options( if_index, &config_options );

            if ( NET_IFACE_OK == e_res )
            {
                /* check range */
                if ( ( 0 != value ) && ( 1 != value ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }

            if ( NET_IFACE_OK == e_res )
            {
                use_feature = (bool)value;

                /* clear the boot field */
                config_options &= ~( NET_IFACE_IPV4_USE_BOOTP );
                /* then set it to the new value */
                if ( true == use_feature )
                {
                    config_options |= ( NET_IFACE_IPV4_USE_BOOTP );
                }

                e_res = net_iface_set_var_ipv4_config_options( if_index, &config_options );

                if ( NET_IFACE_OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            break;
        }

        case OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG:
        {
            net_iface_ipv4_config_options_t config_options;
            bool use_feature;

            /* first get the network config options */
            e_res = net_iface_get_var_ipv4_config_options( if_index, &config_options );

            if ( NET_IFACE_OK == e_res )
            {
                /* check range */
                if ( ( 0 != value ) && ( 1 != value ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }

            if ( NET_IFACE_OK == e_res )
            {
                use_feature = (bool)value;

                /* clear the boot field */
                config_options &= ~( NET_IFACE_IPV4_USE_AUTOIP );
                /* then set it to the new value */
                if ( true == use_feature )
                {
                    config_options |= ( NET_IFACE_IPV4_USE_AUTOIP );
                }

                e_res = net_iface_set_var_ipv4_config_options( if_index, &config_options );

                if ( NET_IFACE_OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
            }
            break;
        }

#ifdef HAVE_IPV6
        case OID_NETWORK_IPV6_ENABLED:
        {
            bool ipv6_enabled;

            if ( ( 0 != value ) && ( 1 != value ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            if ( OK == e_res )
            {
                ipv6_enabled = (bool)value;
                e_res = net_iface_set_var_ipv6_enabled( if_index, ipv6_enabled );
            }
            break;
        }

        case OID_NETWORK_DHCPV6_ENABLED:
            {
                net_iface_ipv6_config_options_t config_options;
                bool use_feature;

                /* first get the network config options */
                e_res = net_iface_get_var_ipv6_config_options( if_index, &config_options );

                if ( NET_IFACE_OK == e_res )
                {
                    /* check range */
                    if ( ( 0 != value ) && ( 1 != value ) )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                if ( NET_IFACE_OK == e_res )
                {
                    use_feature = (bool)value;

                    /* clear the boot field */
                    config_options &= ~( NET_IFACE_IPV6_USE_DHCPV6 );
                    /* then set it to the new value */
                    if ( true == use_feature )
                    {
                        config_options |= ( NET_IFACE_IPV6_USE_DHCPV6 );
                    }

                    e_res = net_iface_set_var_ipv6_config_options( if_index, &config_options );

                    if ( NET_IFACE_OK != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }
                break;
            }

#endif // HAVE_IPV6

    // TODO move to mDNS module
#ifdef HAVE_MDNS 
            case OID_NETWORK_BONJOUR_ENABLED:
            {
                bool mdns_enabled;
                mdns_enabled = (uint8_t)value;

                /* check range */
                if ( ( 0 != mdns_enabled ) && ( 1 != mdns_enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                if ( OK == e_res )
                {
                    e_res = net_mdns_set_var_enabled( index, mdns_enabled );
                }

                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                break;
            }
#endif 

#ifdef HAVE_SNMP
            case OID_NETWORK_SNMP_ENABLED:
            {
                uint8_t enabled;

                enabled = (uint8_t)value;

                /* check range */
                if ( ( 0 != enabled ) && ( 1 != enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                if ( OK == e_res )
                {
                    e_res = net_snmp_set_var_enabled( enabled );
                }

                if ( OK != e_res )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                break;
            }

            case OID_NETWORK_SNMP_RW_ENABLED:
            {
                uint8_t enabled;

                enabled = (uint8_t)value;

                /* check range */
                if ( ( 0 != enabled ) && ( 1 != enabled ) )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }

                if ( OK == e_res )
                {
                    /* we will set the read-write snmp */
                    if ( 1 == enabled )
                    {
                        e_res = net_snmp_set_var_access_mode(NET_SNMP_ACCESS_READ_WRITE);
                        if ( OK != e_res )
                        {
                            e_res = OID_ERROR_ILLEGAL_CHARS;
                        }
                    }
                    /* we will set the read-only snmp */
                    else
                    {
                        e_res = net_snmp_set_var_access_mode(NET_SNMP_ACCESS_READ_ONLY);
                        if ( OK != e_res )
                        {
                            e_res = OID_ERROR_ILLEGAL_CHARS;
                        }
                    }
                }

                break;
            }
        case OID_NETWORK_SNMP_PUBLIC_ACCESS_EN:
        {
            uint8_t enabled;
            enabled = (uint8_t)value;
    
            /* check range */
            if ( ( 0 != enabled ) && ( 1 != enabled ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
    
            if ( OK == e_res )
            {
                e_res = net_snmp_set_var_default_read_community_enabled( enabled );
            }
    
            if ( OK == e_res )
            {
                e_res = net_snmp_set_var_default_write_community_enabled( enabled );
            }
    
            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }
    
            break;
        }
#endif // HAVE_SNMP

#ifdef HAVE_WSD
        case OID_NETWORK_WSD_ENABLED:
        {
            bool enabled;
            enabled = (bool)value;

            /* check range */
            if ( ( 0 != value ) && ( 1 != value ) )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            if ( OK == e_res )
            {
                e_res = net_wsd_set_var_enabled( enabled );
            }

            if ( OK != e_res )
            {
                e_res = OID_ERROR_ILLEGAL_CHARS;
            }

            break;
        }
#endif // HAVE_WSD

#ifdef TM_USE_PPPOE_CLIENT
        case OID_NETWORK_PPPOE_ENABLE:
            {
                bool enabled;
                if ( value )
                {
                    enabled = true;
                }
                else
                {
                    enabled = false;
                }
                e_res = net_iface_set_var_pppoe_enabled(if_index, enabled);
            }
            break;
#endif // TM_USE_PPPOE_CLIENT

        case OID_NETWORK_RESET:
        {
            // revisit intf-specific impl
            //noisy DBG_VERBOSE("%s(%d) if_index %d val %d\n", __func__, index, if_index, value);
        } break;

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
void oid_register_module_network( void )
{

    error_type_t e_res;

    e_res = oid_register_callbacks( OID_NETWORK_STATUS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_MAC_ADDRESS,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_HOST_NAME,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_HOST_NAME_ORIGIN,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_HOST_NAME_DEFAULT,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DOMAIN_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DOMAIN_NAME_ORIGIN,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_HOST_NAME_PREF_ORIGIN,   
	 			    oid_module_network_get_int,            
	 			    oid_module_network_set_int,            
	 			    oid_module_null_test );                
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DOMAIN_NAME_PREF_ORIGIN,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DOMAIN_NAME_MANUAL,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_RESTORE_HOST_DOMAIN_NAMES,
                                    oid_module_null_get,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_CONNECTION_TIMEOUT,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    #ifdef HAVE_ETHERNET
    e_res = oid_register_callbacks( OID_NETWORK_LINK_SPEED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_LINK_CONFIG,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
    #endif // HAVE_ETHERNET

    e_res = oid_register_callbacks( OID_NETWORK_LPD_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_SLP
    e_res = oid_register_callbacks( OID_NETWORK_SLP_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
#endif
    e_res = oid_register_callbacks( OID_NETWORK_IPV4_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_DOMAIN_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_HOST_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_LAST_CONFIGURED_BY,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_MANUAL_CONFIG,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_ALLOW_DHCP_CONFIG,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_ALLOW_BOOTP_CONFIG,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_ALLOW_AUTOIP_CONFIG,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_DHCP_SERVER_ADDR,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );


    e_res = oid_register_callbacks( OID_NETWORK_IPV4_DHCP_LEASE_TIME,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_ADDRESS,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_SUBNET_MASK,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_WINS_SERVER,
                                    oid_module_network_get,
                                    oid_module_network_set, 
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_DEFAULT_GATEWAY,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_IPV6
    e_res = oid_register_callbacks( OID_NETWORK_IPV6_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_DOMAIN_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_HOST_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_LINK_LOCAL_ADDRESS,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_LINK_LOCAL_PREFIX_LEN,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_DHCPV6_ADDRESS,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_DHCPV6_PREFIX_LEN,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_DHCPV6_PREFERRED_LIFETIME,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_DHCPV6_VALID_LIFETIME,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_STATELESS_ADDRESS,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_STATELESS_PREFIX_LEN,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_STATELESS_PREFERRED_LIFETIME,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_STATELESS_VALID_LIFETIME,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DHCPV6_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_PREFERRED_ADDR,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

     e_res = oid_register_callbacks( OID_NETWORK_IPV6_STATUS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#endif // HAVE_IPV6

    e_res = oid_register_callbacks( OID_NETWORK_STAT_TOT_PACKETS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_TOT_PACKETS_RECV,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_TOT_PACKETS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_BCAST_PACKETS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_BCAST_PACKETS_RECV,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_UNICAST_PACKETS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_UNICAST_PACKETS_RECV,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_UNICAST_PACKETS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_FRAMING_ERRORS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_FRAMING_ERRORS_RECV,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_FRAMING_ERRORS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_BAD_PACKETS,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_BAD_PACKETS_RECV,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_BAD_PACKETS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_COLLISIONS_XMITTED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_STAT_COLLISIONS_XMITTED_LATE,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_SNMP
    e_res = oid_register_callbacks( OID_NETWORK_SNMP_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_SNMP_RW_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_SNMP_COMMUNITY_SET_STRING,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_SNMP_COMMUNITY_GET_STRING,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_SNMP_PUBLIC_ACCESS_EN,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
#endif // HAVE_SNMP

    e_res = oid_register_callbacks( OID_NETWORK_IP_ADDRESS_FROM_CONNECTION,
            oid_module_network_get,
            oid_module_network_set,
            oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_WSD
    e_res = oid_register_callbacks( OID_NETWORK_WSD_ENABLED,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
#endif // HAVE_WSD

    e_res = oid_register_callbacks( OID_NETWORK_IN_USE,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_DNS_ADDR,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV6_SUPPORT,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_IPV4_WINS_CONFIGURED,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef TM_USE_PPPOE_CLIENT
    e_res = oid_register_callbacks( OID_NETWORK_PPPOE_ENABLE,
                                    oid_module_network_get_int,
                                    oid_module_network_set_int,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_PPPOE_STATE,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_PPPOE_USERNAME,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_PPPOE_PASSWORD,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );
  
#endif // TM_USE_PPPOE_CLIENT  

    e_res = oid_register_callbacks( OID_NETWORK_RESET, 
                                    oid_module_null_test,
                                    oid_module_network_set_int, // trigger only
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_INTF_COUNT,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_INTF_MAP,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_INTF_ACTIVE,
                                    oid_module_network_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_INTF_NAME,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

    e_res = oid_register_callbacks( OID_NETWORK_MAC_ADDRESS_BYTES,
                                    oid_module_network_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
    ASSERT( FAIL != e_res );

#ifdef HAVE_UAP
    oid_register_module_uap();
#endif

}
