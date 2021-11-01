/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef GCPP_H
#define GCPP_H

//#include "tx_api.h"
#include <curl/curl.h>
#include <libstrophe/common.h>
#include <libstrophe/strophe.h>
#include <ATypes.h>


#include "os_network.h"
#include "net_iface_api.h"
//#include "net_app_api.h"
#include "net_iface_vars.h"

#include "gcpp_logger.h"


#ifdef __cplusplus
extern "C" {
#endif


#define THREAD_STACK_SIZE   (MAX((64 * 1024), POSIX_MIN_STACK_SIZE))
#define DEFAULT_DELAY_SECS              5

#define GCPP_DEFAULT_CLOUDPRINT_URL     "https://www.google.com/cloudprint"
#define GCPP_DEFAULT_OAUTH2_URL         "https://accounts.google.com/o/oauth2"
#define GCPP_DEFAULT_XMPP_DOMAIN        "xmpp.google.com"

// Flags for GCPP Events
#define GCPP_FLAG_ENABLED 				0x01
#define GCPP_FLAG_REGISTERED			0x02
#define GCPP_FLAG_IPV4_CONFIGURED 		0x04

// Strings for GCP Job Status
#define GCPP_JOB_STATUS_QUEUED_STR		"QUEUED"
#define GCPP_JOB_STATUS_IN_PROGRESS_STR	"IN_PROGRESS"
#define GCPP_JOB_STATUS_DONE_STR		"DONE"
#define GCPP_JOB_STATUS_ERROR_STR		"ERROR"

typedef struct
{
    bool                initialized;
	
	pthread_mutex_t		global_lock_mutex;
	pthread_mutex_t		registration_lock_mutex;
	
	gcpp_oem_print_func_table_t *oem_func_table;
	gcpp_oem_print_config_t     *oem_config;

	bool		enabled;
	bool 		registered;
	char		*proxy;
	char		*printer_name;
	char		*display_name;
	char 		*xmpp_jid;
	char 		*refresh_token;

	char		*printer_id;

	bool		http_proxy_enabled;
	char		*http_proxy_server;
	uint32_t	http_proxy_port;
	
	bool		http_proxy_auth_req;
	char		*http_proxy_username;
	char		*http_proxy_password;
	
	// current states
	bool		network_connected;
	bool		xmpp_connected;

	// latest xsrf token
	char		*xsrf_token;
	
	// async context responses
	gcpp_registration_anonymous_context_t		*anonymous_registration;
	gcpp_registration_claim_printer_context_t 	*claim_printer;
	gcpp_authorization_tokens_context_t 		*auth_tokens;

	// xmpp context
	xmpp_ctx_t 			*ctx;
	xmpp_conn_t 		*conn;
	
	// debug only
#ifdef DEBUG
	bool				debug_on;
	ULONG				debug_curl_level;
	ULONG				debug_xmpp_level;
	bool				debug_xmpp_enabled;
#endif
	
} net_gcpp_context_t;

net_gcpp_context_t *gcpp_get_context(void);


typedef struct
{
    void *request_param;
    void *callback_context;
} gcpp_request_params_t;

typedef gcpp_error_t (*gcpp_async_func_ptr_t)(net_gcpp_context_t *gcpp_context, void *callback_context);

typedef struct
{
    const char                  *name;
    void                        *callback_context;
    gcpp_async_func_ptr_t       async_func_ptr;
	net_gcpp_context_t          *gcpp_context;
    void                        *next;
} gcpp_async_request_t;

typedef struct
{
  pthread_mutex_t           mutex;
  pthread_mutex_t           requests_mutex;
  void                      *requests_fifo;
} gcpp_async_context_t;


#ifdef __cplusplus
}
#endif

#endif // GCPP_H
