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
//#include <errno.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "dprintf.h"
#include "lassert.h"
#include <pthread.h>
#include <posix_ostools.h>
#include "memAPI.h"
#include <arpa/inet.h>
#include <unistd.h>

#include "NetworkIFModule.h"

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
                                            uint32_t index,
                                            char * data_ptr,
                                            uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = 0;
    //unsigned int string_size;

    // TODO must account for multiple simultaneous interfaces
    //unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    switch ( oid )
    {
        case OID_SINDOH_GET_IP:
        {
            uint32_t ipv4_addr;
            printf("--jeseo-- get ip addr");
            NIF_getIPAddrV4_int_wired((T_IPADDRv4 *)&ipv4_addr);

            //e_res = net_iface_get_var_ipv4_addr( if_index, &ipv4_addr );

            if ( 0 == e_res )
            {
                inet_ntop(AF_INET, (void *)(&ipv4_addr), data_ptr, INET_ADDRSTRLEN);
            }

            break;
        }
		/*
		case OID_SINDOH_WIFI_SCAN_SSID:
		{
			int N=0;
			T_SSID *ptrSSIDList=NULL;

			NIF_findSSID(&N, &ptrSSIDList, 40);

			if (ptrSSIDList != NULL){
    		   
			    int i=0;
	        	for (i=0;i<N;i++)
	        	{   
    	        	T_SSID * currentItem = (void *)( (unsigned int)ptrSSIDList+(sizeof(T_SSID)*i) );
		              
		            printf ("\n");
		            printf ("Parsing infomation -------------------------------------------------------------\n");
		            printf ("HWAddr: %s, SSID: %s [q:%d]\n",currentItem->hwAddr, currentItem->name, currentItem->radioQuality);
		            printf ("AuthType:%d, GCipher:%d, PCipher:%d, A.Suit:%d\n",
		                    currentItem->authenticationType,currentItem->groupCipher,currentItem->pairCipher,currentItem->authSuite);
		            printf ("Hidden?:%d, PreAuthSupport:%d\n",currentItem->ifHiddenSSID,currentItem->ifSupportPreAuth);
		            printf ("EtcOption:%s\n",currentItem->unknownOption);
		            printf ("------------------------------------------------------------- Parsing infomation\n\n");
		            

				}
				int result=NIF_freeSSID(N, ptrSSIDList);
		        printf("NIF Free Reesult:%d\n",result);
			}
		}
		//*/
        default:
            {
                /* bad oid */
                e_res = -1;

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
static error_type_t oid_module_network_set( oid_t oid,
        uint32_t index,
        const char * data,
        uint32_t string_len_in_bytes )
{
    error_type_t e_res = 0;

    if ( NULL == data )
    {
        e_res = -1;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    // TODO must account for multiple simultaneous interfaces
    //unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

    if ( 0 == e_res )
    {
        switch ( oid )
        {
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
                    //e_res = net_iface_set_var_ipv4_manual_addr(if_index, &ipv4_addr);

                    if ( 0 != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }
            case OID_SINDOH_WIFI_CONNECT_SSID:
            {
                uint32_t ipv4_addr;
                ipv4_addr = inet_addr( (char*)data );

                if ( ipv4_addr == -1 )
                {
                    e_res = OID_ERROR_ILLEGAL_CHARS;
                }
                else
                {
                    //e_res = net_iface_set_var_ipv4_manual_addr(if_index, &ipv4_addr);

                    if ( 0 != e_res )
                    {
                        e_res = OID_ERROR_ILLEGAL_CHARS;
                    }
                }

                break;
            }
            default:
            {
                /* bad oid */
                e_res = -1;

                break;
            }
        }
    }

    return e_res;
}


/**
 *
 * \brief Registers each Network API OID with the oid controller
 *
 */
void oid_register_module_vendor( void )
{

    error_type_t cmd_res;

    cmd_res = oid_register_callbacks( OID_SINDOH_GET_IP,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
	XASSERT( ( 0 == cmd_res ), cmd_res );

/*	cmd_res = oid_register_callbacks( OID_SINDOH_WIFI_SCAN_SSID,
                                    oid_module_network_get,
                                    oid_module_network_set,
                                    oid_module_null_test );
	XASSERT( ( 0 == cmd_res ), cmd_res );
//*/
}
