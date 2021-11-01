/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
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
#include <errno.h>
#include <string.h>
#include "oid_api.h"
#include "oid_producer_api.h"
#include "platform_api.h"
#include "dprintf.h"
#include "lassert.h"

#include "net_iface_vars.h"
#include "net_gcpp_vars.h"
#include "net_gcpp_api.h"

//#include "dhcpd_vars.h"


/**
 * 
 * \brief The backend oid GCPP module function for retreiving
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
static error_type_t oid_module_gcpp_get_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t * value)
{
    error_type_t e_res = OK;

	switch ( oid )
	{
		/**
		 * OID_GCPP_INITIALIZED - indicates whether GCPP has been initialized.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_INITIALIZED:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool init = net_gcpp_is_initialized();
				*value = (uint32_t)init;
			}
			break;
		}

		/**
		 * OID_GCPP_ENABLED - indicates whether GCPP is enabled.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_ENABLED:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool enabled = net_gcpp_is_enabled(true);
				*value = (uint32_t)enabled;
			}
			break;
		}

		/**
		 * OID_GCPP_REGISTERED - indicates whether GCPP is registered.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_REGISTERED:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool registered = net_gcpp_is_registered(true);
				*value = (uint32_t)registered;
			}
			break;
		}

		/**
		 * OID_GCPP_NETWORK_CONNECTED - indicates whether a network connection has
		 *    been established.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_NETWORK_CONNECTED:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool network_connected = net_gcpp_is_network_connected();
				*value = (uint32_t)network_connected;
			}
			break;
		}

		/**
		 * OID_GCPP_XMPP_CONNECTED - indicates whether a XMPP connection has
		 *    been established.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_XMPP_CONNECTED:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool xmpp_connected = net_gcpp_is_xmpp_connected();
				*value = (uint32_t)xmpp_connected;
			}
			break;
		}

		/**
		 * OID_GCPP_TOKEN_DURATION - provides the token duration.
		 *
		 *    Return Value: token duration in seconds
		 *                  0 if invalid
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid anonymous request or anonymous 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_TOKEN_DURATION:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				if (anon_req && anon_req->hdr.success) 
				{
					if (anon_req->token_duration)
					{
						*value = (uint32_t)strtoul( anon_req->token_duration, NULL, 10 );
					}
					else
					{
						*value = 0;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_REGISTRATION_START_ANONYMOUS - provides anonymous request results.
		 *
		 *    Return Value: index 0 - returns 1 if results are available, 0 if not.
		 *                  index 1 - returns GCPP result code.
		 *                  index 2 - returns http response code, or 0 if invalid.
		 *                  index 3 - returns GCP response success (1) or failure (0).
		 *                  index 4 - returns GCP response error code, or 0 if invalid.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0 thru 4
		 *            FAIL - requesting index 1 thru 4 while results not available 
		 * 
		 */
		case OID_GCPP_REGISTRATION_START_ANONYMOUS:
		{ 
			gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
			if (anon_req) 
			{
				switch (index)
				{
					case 0: // register anonymous request results available
					{
						*value = 1;
						break;
					}
					
					case 1: // register anonymous request result code
					{
						*value = (uint32_t)anon_req->hdr.result;
						break;
					}
					
					case 2: // register anonymous request http response code
					{
						*value = (uint32_t)anon_req->hdr.response_code;
						break;
					}
					
					case 3: // register anonymous response success/failure
					{
						*value = (uint32_t)anon_req->hdr.success;
						break;
					}
					
					case 4: // register anonymous response error code (if not successful)
					{
						*value = (uint32_t)anon_req->hdr.error_code;
						break;
					}
					
					default:
						e_res = OID_ERROR_INDEX_INVALID;
						break;
				}
			}
			else
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = FAIL;
				}
				else
				{
					// register anonymous request results not available
					*value = 0;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_REGISTRATION_CLAIM_PRINTER - provides printer claim request results.
		 *
		 *    Return Value: index 0 - returns 1 if results are available, 0 if not.
		 *                  index 1 - returns GCPP result code.
		 *                  index 2 - returns http response code, or 0 if invalid.
		 *                  index 3 - returns GCP response success (1) or failure (0).
		 *                  index 4 - returns GCP response error code, or 0 if invalid.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0 thru 4
		 *            FAIL - requesting index 1 thru 4 while results not available 
		 * 
		 */
		case OID_GCPP_REGISTRATION_CLAIM_PRINTER:
		{ 
			gcpp_registration_claim_printer_context_t *claim_printer_req = net_gcpp_get_claim_printer_request_response();
			if (claim_printer_req) 
			{
				switch (index)
				{
					case 0: // claim printer request result code
					{
						*value = 1;
						break;
					}
					
					case 1: // claim printer request result code
					{
						*value = (uint32_t)claim_printer_req->hdr.result;
						break;
					}
					
					case 2: // claim printer request http response code
					{
						*value = (uint32_t)claim_printer_req->hdr.response_code;
						break;
					}
					
					case 3: // claim printer request response success/failure
					{
						*value = (uint32_t)claim_printer_req->hdr.success;
						break;
					}
					
					case 4: // claim printer request response error code
					{
						*value = (uint32_t)claim_printer_req->hdr.error_code;
						break;
					}
					
					default:
						e_res = OID_ERROR_INDEX_INVALID;
						break;
				}
			}
			else
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = FAIL;
				}
				else
				{
					// claim printer request results not available
					*value = 0;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_REGISTRATION_CANCEL - indicates whether registration can be cancelled.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_REGISTRATION_CANCEL:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool can_cancel = net_gcpp_is_registered(true);
				*value = (uint32_t)can_cancel;
			}
			break;
		}
		
		/**
		 * OID_GCPP_PRINTER_ADD - indicates whether printer can be added.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_PRINTER_ADD:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_get_printer_id(true))
				{
					*value = 0;
				}
				else
				{
					*value = 1;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_PRINTER_REMOVE - indicates whether printer can be removed.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_PRINTER_REMOVE:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_get_printer_id(true))
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
		
		/**
		 * OID_GCPP_PRINTER_UPDATE - indicates whether printer can be updated.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_PRINTER_UPDATE:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_get_printer_id(true))
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
		
		/**
		 * OID_GCPP_PRINTER_RESET - indicates whether printer can be reset.
		 *
		 *    Return Value: 1 (true) - always true for now
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_PRINTER_RESET:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				*value = 1;
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_ENABLED - indicates whether HTTP proxy feature is enabled.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_ENABLED:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool enabled = net_gcpp_is_http_proxy_enabled(true);
				*value = (uint32_t)enabled;
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_PORT - returns HTTP proxy port value.
		 *
		 *    Return Value: http proxy port
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_PORT:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				uint32_t port = net_gcpp_get_http_proxy_port(true);
				*value = (uint32_t)port;
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_AUTH_REQ - indicates whether HTTP proxy authorization is required.
		 *
		 *    Return Value: 1 (true) or 0 (false)
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_AUTH_REQ:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				bool auth = net_gcpp_is_http_proxy_auth_required(true);
				*value = (uint32_t)auth;
			}
			break;
		}

		default:
		{
			/* invalid oid */
			e_res = OID_ERROR_OID_INVALID;

			break;
		}
	}

    return e_res;
}

/**
 * 
 * \brief The backend oid GCPP API module function for 
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
 * OID_ERROR_ILLEGAL_CHARS 
 * FAIL - bad oid, other error 
 *
 */
static error_type_t oid_module_gcpp_set_int( oid_t oid,
                                             uint32_t index,
                                             uint32_t value)
{
    error_type_t e_res = OK;
	
    switch ( oid )
    {
		/**
		 * OID_GCPP_ENABLED - enables or disables GCPP.
		 *
		 *    Value Parameter: 1 to enable, 0 to disable
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - unable to enable/disable GCPP
		 * 
		 */
		case OID_GCPP_ENABLED:
		{ 
            uint8_t enabled;
            enabled = (uint8_t)value;
			
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}

			/* check range */
			if ( ( 0 != enabled ) && ( 1 != enabled ) )
			{
				e_res = OID_ERROR_ILLEGAL_CHARS;
			}
			
            if ( OK == e_res ) 
            {
				if (net_gcpp_enable(enabled) != NET_GCPP_SUCCESS)
				{
					e_res = FAIL;
				}
            }
			
			break;
		}

		/**
		 * OID_GCPP_REGISTERED - enable or disable GCPP registration.
		 *
		 *    Value Parameter: 1 to enable, 0 to disable
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - unable to enable/disable GCPP registration
		 * 
		 */
		case OID_GCPP_REGISTERED:
		{
            uint8_t registered;
            registered = (uint8_t)value;
			
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}

			/* check range */
			if ( ( 0 != registered ) && ( 1 != registered ) )
			{
				e_res = OID_ERROR_ILLEGAL_CHARS;
			}
			
            if ( OK == e_res ) 
            {
				if (net_gcpp_registered(registered) != NET_GCPP_SUCCESS)
				{
					e_res = FAIL;
				}
            }
			
			break;
		}
			
		/**
		 * OID_GCPP_REGISTRATION_START_ANONYMOUS - starts anonymous registration request.
		 *
		 *    Value Parameter: 0 issue request synchronously
		 *                     1 issue request asynchronously
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - anonymous registration request failed
		 * 
		 */
		case  OID_GCPP_REGISTRATION_START_ANONYMOUS:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			
			/* check range */
			if ( ( 0 != value ) && ( 1 != value ) )
			{
				e_res = OID_ERROR_ILLEGAL_CHARS;
			}

            if ( OK == e_res ) 
			{
				gcpp_error_t result;
				bool async = (uint8_t)value;
				
				result = net_gcpp_issue_register_anonymous_request(NULL, async);
				
				if (result == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_REGISTRATION_CLAIM_PRINTER - issues claim printer request.
		 *
		 *    Value Parameter: 0 issue request synchronously
		 *                     1 issue request asynchronously
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - anonymous registration not issued or successful
		 *                   claim printer request failed
		 * 
		 */
		case  OID_GCPP_REGISTRATION_CLAIM_PRINTER:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			
			/* check range */
			if ( ( 0 != value ) && ( 1 != value ) )
			{
				e_res = OID_ERROR_ILLEGAL_CHARS;
			}

            if ( OK == e_res ) 
			{
				bool async = (uint8_t)value;
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				
				if (anon_req &&	anon_req->hdr.success && anon_req->polling_url) 
				{
					gcpp_error_t result;
					
					result = net_gcpp_issue_claim_printer_request(anon_req->polling_url, NULL, async);
					
					if (result == NET_GCPP_SUCCESS)
					{
						e_res = OK;
					}
					else
					{
						e_res = FAIL;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_REGISTRATION_CANCEL - cancel and clear any pending registration request.
		 *
		 *    Value Parameter: ignored
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - unable to cancel GCPP registration request
		 * 
		 */
		case  OID_GCPP_REGISTRATION_CANCEL:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_registered(false) == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_PRINTER_ADD - add GCPP printer
		 *
		 *    Value Parameter: ignored
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - unable to add GCPP printer
		 * 
		 */
		case OID_GCPP_PRINTER_ADD:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_printer_add() == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_PRINTER_REMOVE - remove GCPP printer
		 *
		 *    Value Parameter: ignored
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - unable to remove GCPP printer
		 * 
		 */
		case OID_GCPP_PRINTER_REMOVE:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_printer_remove() == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
				break;
			}
		}
		
		/**
		 * OID_GCPP_PRINTER_UPDATE - update GCPP printer
		 *
		 *    Value Parameter: ignored
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - unable to update GCPP printer
		 * 
		 */
		case OID_GCPP_PRINTER_UPDATE:
		{ 
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_printer_update() == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_PRINTER_RESET - perform a complete reset of the GCPP printer
		 *
		 *    Value Parameter: ignored
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - unable to reset GCPP printer
		 * 
		 */
		case  OID_GCPP_PRINTER_RESET:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				if (net_gcpp_printer_reset() == NET_GCPP_SUCCESS)
				{
					e_res = OK;
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_ENABLED - enable or disable HTPP proxy feature.
		 *
		 *    Value Parameter: 1 to enable, 0 to disable
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - unable to enable/disable GCPP
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_ENABLED:
		{
            uint8_t enabled;
            enabled = (uint8_t)value;
			
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				/* check range */
				if ( ( 0 != enabled ) && ( 1 != enabled ) )
				{
					e_res = OID_ERROR_ILLEGAL_CHARS;
				}
			}
			
            if ( OK == e_res ) 
            {
				if (net_gcpp_http_proxy_enable(enabled) != NET_GCPP_SUCCESS)
				{
					e_res = FAIL;
				}
            }
			
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_PORT - set HTTP proxy port.
		 *
		 *    Value Parameter: http proxy port to set
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_VALUE_TOO_BIG - value greater than 0xFFFF (unsigned short)
		 *            FAIL - unable to set http proxy port
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_PORT:
		{
            uint32_t port;
            port = (uint32_t)value;
			
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				/* check range */
				if ( port > 0xFFFF )
				{
					e_res = OID_ERROR_VALUE_TOO_BIG;
				}
			}
			
            if ( OK == e_res ) 
            {
				if (net_gcpp_set_http_proxy_port(port) != NET_GCPP_SUCCESS)
				{
					e_res = FAIL;
				}
            }
			
			break;
		}
		
		
		/**
		 * OID_GCPP_HTTP_PROXY_AUTH_REQ - enable or disable HTPP proxy 
		 *    authorization requirement.
		 *
		 *    Value Parameter: 1 to enable, 0 to disable
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            OID_ERROR_ILLEGAL_CHARS - value is not 0 or 1
		 *            FAIL - unable to enable/disable HTPP proxy authorization
		 *                   requirement
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_AUTH_REQ:
		{
            uint8_t auth;
            auth = (uint8_t)value;
			
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				/* check range */
				if ( ( 0 != auth ) && ( 1 != auth ) )
				{
					e_res = OID_ERROR_ILLEGAL_CHARS;
				}
			}
			
            if ( OK == e_res ) 
            {
				if (net_gcpp_http_proxy_auth_required(auth) != NET_GCPP_SUCCESS)
				{
					e_res = FAIL;
				}
            }
			
			break;
		}

		default:
		{
			/* invalid oid */
			e_res = OID_ERROR_OID_INVALID;

			break;
		}
    }
	
    return e_res;
}


/**
 * 
 * \brief The backend oid GCPP API module function for retreiving
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
static error_type_t oid_module_gcpp_get( oid_t oid,
                                         uint32_t index,
                                         char * data_ptr,
                                         uint32_t max_string_len_in_bytes )
{
    error_type_t e_res = OK;
	
	switch ( oid )
	{
		/**
		 * OID_GCPP_PROXY - gets the GCPP proxy string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid GCPP proxy string
		 * 
		 */
		case OID_GCPP_PROXY:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *proxy = net_gcpp_get_proxy(true);
				if (proxy) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											proxy);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_PRINTER_NAME - gets the GCPP printer name string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid GCPP printer name string
		 * 
		 */
		case OID_GCPP_PRINTER_NAME:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *name = net_gcpp_get_printer_name(true);
				if (name) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											name);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_DISPLAY_NAME - gets the GCPP printer display name string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid GCPP printer display name string
		 * 
		 */
		case OID_GCPP_DISPLAY_NAME:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *name = net_gcpp_get_display_name(true);
				if (name) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											name);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_PRINTER_ID - gets the GCPP printer id string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid GCPP printer id string
		 * 
		 */
		case OID_GCPP_PRINTER_ID:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *id = net_gcpp_get_printer_id(false);
				if (id) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											id);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_REGISTRATION_TOKEN - gets the anonymous registration token.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid anonymous request or anonymous 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_REGISTRATION_TOKEN:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				if (anon_req && anon_req->hdr.success && anon_req->registration_token) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											anon_req->registration_token);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_POLLING_URL - gets the registration polling URL.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid anonymous request or anonymous 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_POLLING_URL:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				if (anon_req && anon_req->hdr.success && anon_req->polling_url) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											anon_req->polling_url);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_INVITE_URL - gets the registration invite URL.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid anonymous request or anonymous 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_INVITE_URL:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				if (anon_req && anon_req->hdr.success && anon_req->invite_url) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											anon_req->invite_url);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_INVITE_PAGE_URL - gets the registration invite page URL.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid anonymous request or anonymous 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_INVITE_PAGE_URL:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
				if (anon_req && anon_req->hdr.success && anon_req->invite_page_url) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											anon_req->invite_page_url);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		/**
		 * OID_GCPP_CONFIRMATION_PAGE_URL - gets the registration confirmation page URL.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid claim printer request or claim printer 
		 *                   request was not successful
		 * 
		 */
		case OID_GCPP_CONFIRMATION_PAGE_URL:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				gcpp_registration_claim_printer_context_t *claim_printer_req = net_gcpp_get_claim_printer_request_response();
				if (claim_printer_req && claim_printer_req->hdr.success && claim_printer_req->confirmation_page_url) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											claim_printer_req->confirmation_page_url);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_SERVER - gets the HTTP proxy server string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid HTTP proxy server string
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_SERVER:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *value = net_gcpp_get_http_proxy_server(true);
				if (value) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											value);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_USERNAME - gets the HTTP proxy username string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid HTTP proxy username string
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_USERNAME:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *value = net_gcpp_get_http_proxy_username(true);
				if (value) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											value);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}
		
		/**
		 * OID_GCPP_HTTP_PROXY_PASSWORD - gets the HTTP proxy password string.
		 *
		 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
		 *            FAIL - invalid HTTP proxy password string
		 * 
		 */
		case OID_GCPP_HTTP_PROXY_PASSWORD:
		{
			/* check index */
			if ( 0 != index )
			{
				e_res = OID_ERROR_INDEX_INVALID;
			}
			else
			{
				unsigned int string_size;
				char *value = net_gcpp_get_http_proxy_password(true);
				if (value) 
				{
					e_res = min_n_k_sprintf(data_ptr, 
											max_string_len_in_bytes,
											&string_size,
											"%s",
											value);
					ASSERT(string_size <= max_string_len_in_bytes);
					if(DEBUG_STRING_CAPPED == e_res) 
					{
						e_res = OID_ERROR_VALUE_CAPPED;
					}
				}
				else
				{
					e_res = FAIL;
				}
			}
			break;
		}

		default:
		{
			/* invalid oid */
			e_res = OID_ERROR_OID_INVALID;

			break;
		}
    }
	
    return e_res;
}

/**
 * 
 * \brief The backend oid module function for the GCPP module set
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
error_type_t oid_module_gcpp_set( oid_t oid,
                                  uint32_t index,
                                  const char * data,
                                  uint32_t string_len_in_bytes )
{
    error_type_t e_res;

    e_res = OK;

    if ( NULL == data )
    {
        e_res = FAIL;
    }
    if ( string_len_in_bytes == 0 )
    {
        e_res = OID_ERROR_VALUE_TOO_SMALL;
    }
    if ( string_len_in_bytes > OID_MAX_STRING_SIZE_IN_BYTES )
    {
        e_res = OID_ERROR_VALUE_TOO_BIG;
    }

    if ( OK == e_res )
    {
        switch ( oid )
        {
			/**
			 * OID_GCPP_PROXY - sets the GCPP proxy string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case  OID_GCPP_PROXY:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_NAME_LEN) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_NAME_LEN+1];
					
					memset(temp, 0, GCPP_MAX_NAME_LEN+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_proxy(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}

			/**
			 * OID_GCPP_PRINTER_NAME - sets the GCPP printer name string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case  OID_GCPP_PRINTER_NAME:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_NAME_LEN) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_NAME_LEN+1];
					
					memset(temp, 0, GCPP_MAX_NAME_LEN+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_printer_name(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}

			/**
			 * OID_GCPP_DISPLAY_NAME - sets the GCPP printer display name string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case  OID_GCPP_DISPLAY_NAME:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_NAME_LEN) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_NAME_LEN+1];
					
					memset(temp, 0, GCPP_MAX_NAME_LEN+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_display_name(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}
		
			/**
			 * OID_GCPP_HTTP_PROXY_SERVER - sets the HTTP proxy server string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case OID_GCPP_HTTP_PROXY_SERVER:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_URL_LENGTH) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_URL_LENGTH+1];
					
					memset(temp, 0, GCPP_MAX_URL_LENGTH+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_http_proxy_server(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}
			
			/**
			 * OID_GCPP_HTTP_PROXY_USERNAME - sets the HTTP proxy username string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case OID_GCPP_HTTP_PROXY_USERNAME:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_URL_LENGTH) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_URL_LENGTH+1];
					
					memset(temp, 0, GCPP_MAX_URL_LENGTH+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_http_proxy_username(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}
			
			/**
			 * OID_GCPP_HTTP_PROXY_PASSWORD - sets the HTTP proxy password string.
			 *
			 *    Errors: OID_ERROR_INDEX_INVALID - index not 0
			 *            OID_ERROR_VALUE_TOO_BIG - string length too big
			 *            OID_ERROR_ILLEGAL_CHARS - illegal chars in string, or unable to set string
			 * 
			 */
			case OID_GCPP_HTTP_PROXY_PASSWORD:
			{
				/* check index */
				if ( 0 != index )
				{
					e_res = OID_ERROR_INDEX_INVALID;
				}
				else
				{
					if(string_len_in_bytes >= GCPP_MAX_URL_LENGTH) 
					{
						e_res= OID_ERROR_VALUE_TOO_BIG;
					}
				}
				
				if(OK == e_res) 
				{
					char temp[GCPP_MAX_URL_LENGTH+1];
					
					memset(temp, 0, GCPP_MAX_URL_LENGTH+1);
					memcpy(temp, data, string_len_in_bytes);
					
					if (net_gcpp_set_http_proxy_password(temp) != NET_GCPP_SUCCESS)
					{
						e_res = OID_ERROR_ILLEGAL_CHARS;
					}
				}
				break;
			}
			
			default:
			{
			/* invalid oid */
			e_res = OID_ERROR_OID_INVALID;

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
void oid_register_module_gcpp( void )
{
    error_type_t e_res;

    e_res = oid_register_callbacks( OID_GCPP_INITIALIZED,
                                    oid_module_gcpp_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );

    e_res = oid_register_callbacks( OID_GCPP_ENABLED,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );

    e_res = oid_register_callbacks( OID_GCPP_REGISTERED,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );

    e_res = oid_register_callbacks( OID_GCPP_NETWORK_CONNECTED,
                                    oid_module_gcpp_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );

    e_res = oid_register_callbacks( OID_GCPP_XMPP_CONNECTED,
                                    oid_module_gcpp_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PROXY,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_NAME,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_DISPLAY_NAME,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_ID,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_REGISTRATION_START_ANONYMOUS,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_REGISTRATION_CLAIM_PRINTER,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_REGISTRATION_CANCEL,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_REGISTRATION_TOKEN,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_TOKEN_DURATION,
                                    oid_module_gcpp_get_int,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_POLLING_URL,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_INVITE_URL,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_INVITE_PAGE_URL,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_CONFIRMATION_PAGE_URL,
                                    oid_module_gcpp_get,
                                    oid_module_null_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_ADD,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_REMOVE,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_UPDATE,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_PRINTER_RESET,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_ENABLED,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_SERVER,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_PORT,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_AUTH_REQ,
                                    oid_module_gcpp_get_int,
                                    oid_module_gcpp_set_int,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_USERNAME,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    e_res = oid_register_callbacks( OID_GCPP_HTTP_PROXY_PASSWORD,
                                    oid_module_gcpp_get,
                                    oid_module_gcpp_set,
                                    oid_module_null_test );
									
    ASSERT( FAIL != e_res );
}

