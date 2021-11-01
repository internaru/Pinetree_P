/*
 * ============================================================================
 * Copyright (c) 2011   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 *\file gcpp_test.c
 *\brief Tests the Google Cloud Print Proxy.
 */


#include <curl/curl.h>
#include <libstrophe/common.h>
#include <libstrophe/strophe.h>
#include <liboauth/oauth.h>
#include <liboauth/xmalloc.h>
#include "string.h"
#include "dprintf.h"
#include "gcpp_test.h"
#include "logger.h"
#include "debug.h"
#include "net_logger.h"

// Submodule ID for debug message logging
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_GCPP 


/* define a test handler for connection events */
void conn_test_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
          const int error, xmpp_stream_error_t * const stream_error,
          void * const userdata)
{
    xmpp_ctx_t *ctx = (xmpp_ctx_t *)userdata;

    if (status == XMPP_CONN_CONNECT) {
        dbg_printf("DEBUG: connected\n");

        /* good enough for now */
        
        /* disconnect */
        xmpp_stop(ctx);
    }
    else {
        dbg_printf("DEBUG: disconnected\n");
        xmpp_stop(ctx);
    }
}


#define XMPP_DEFAULT_TIMEOUT 10000
int gcpp_test( char *jid, char *password )
{
#if 1    
    CURL *curl;
    CURLcode res;

    DBG_PRINTF(LOG_DEBUG, "Init CURL\n" );
    curl = curl_easy_init();
    if(curl) {
        DBG_PRINTF(LOG_DEBUG, "Try easy HTTP example...\n" );
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        res = curl_easy_perform(curl);

        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
#endif

#if 0    
    xmpp_ctx_t *ctx;
    xmpp_conn_t *conn;
    xmpp_log_t *log;


    //xmpp_initialize();
    
    /* create a context */
    log = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* pass NULL instead to silence output */
    ctx = xmpp_ctx_new(NULL, log);

    /* create a connection */
    conn = xmpp_conn_new(ctx);
    
    conn->connect_timeout = XMPP_DEFAULT_TIMEOUT;

    /* setup authentication information */
    xmpp_conn_set_jid(conn, jid);
    xmpp_conn_set_pass(conn, password);

    /* initiate connection */
    xmpp_connect_client(conn, NULL, 0, conn_test_handler, ctx);
    xmpp_run(ctx);

    /* release our connection and context */
    xmpp_conn_release(conn);
    xmpp_ctx_free(ctx);
    
    //xmpp_shutdown();
#endif

    return 0;
}
