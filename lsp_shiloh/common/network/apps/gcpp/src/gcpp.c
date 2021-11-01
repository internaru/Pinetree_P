/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

 
//--------------------------------------
// Included files
//--------------------------------------
#include "dprintf.h"
#include "lassert.h"
#include "memAPI.h"
#include "sys_init_api.h"

#include "cmd_proc_api.h"
#include "cmd_wrapper_api.h"

#include "net_gcpp_api.h"
#include "net_gcpp_vars.h"

#include "gcpp.h"
#include "gcpp_helper.h"


//--------------------------------------
// OEM-specific configuration functions. 
//--------------------------------------
static gcpp_oem_print_func_table_t gcpp_oem_print_func_table;

//--------------------------------------
// OEM-specific configuration values. 
//--------------------------------------
static gcpp_oem_print_config_t gcpp_oem_print_config;

//--------------------------------------
// Module context
//--------------------------------------
static net_gcpp_context_t   gcpp_context = {false};
static gcpp_async_context_t gcpp_async_context;

//--------------------------------------
// Global Definitions
//--------------------------------------

static pthread_t                gcpp_run_thread;
static ALIGN8 uint8_t           gcpp_run_stack[ THREAD_STACK_SIZE ];

static pthread_t                gcpp_async_request_thread;
static ALIGN8 uint8_t           gcpp_async_request_stack[ THREAD_STACK_SIZE ];

static gcpp_run_state_t         gcpp_run_state = GCPP_INIT;
static gcpp_job_state_t         gcpp_job_state = GCPP_JOBS_INIT;

static int                      gcpp_xmpp_cycle_port = 0;

//static net_iface_event_handle_t gcpp_iface_handle = NULL;
//static TX_EVENT_FLAGS_GROUP     gcpp_ready_flags;
pthread_mutex_t gcpp_ready_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t  gcpp_ready_cond  = PTHREAD_COND_INITIALIZER;
uint32_t gcpp_ready_flags = 0;

pthread_mutex_t gcpp_suspend_mutex = POSIX_MUTEX_INITIALIZER;
pthread_cond_t  gcpp_resume_cond  = PTHREAD_COND_INITIALIZER;
bool gcpp_suspended = false;

#define GCPP_GLOBAL_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&gcpp_context.global_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_GLOBAL_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&gcpp_context.global_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_REGISTRATION_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&gcpp_context.registration_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_REGISTRATION_IS_LOCKED() (pthread_mutex_trylock(&gcpp_context.registration_lock_mutex) == 0)

#define GCPP_REGISTRATION_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&gcpp_context.registration_lock_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_ASYNC_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&gcpp_async_context.mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_ASYNC_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&gcpp_async_context.mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 
        
#define GCPP_ASYNC_IS_LOCKED() (pthread_mutex_trylock(&gcpp_async_context.mutex) == 0)
        
//#define GCPP_ASYNC_RESUME()  (tx_thread_resume(&gcpp_async_request_thread))
//#define GCPP_ASYNC_SUSPEND() (tx_thread_suspend(&gcpp_async_request_thread))

#define GCPP_ASYNC_REQUESTS_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&gcpp_async_context.requests_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define GCPP_ASYNC_REQUESTS_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&gcpp_async_context.requests_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 
        
//--------------------------------------
// Local Definitions and Types
//--------------------------------------

static void* gcpp_main( void*  input );
//static void gcpp_network_event_func( net_iface_event_flags_t events, void *context );

void gcpp_set_flags( uint32_t flags );
void gcpp_clear_flags( uint32_t flags );

void gcpp_async_suspend();
void gcpp_async_resume();

char *net_gcpp_new_stanza_id(xmpp_conn_t * const conn);
int gcpp_xmpp_ping(xmpp_conn_t * const conn);

void gcpp_register_anonymous_context_cleanup(gcpp_registration_anonymous_context_t *anon_reg);
void gcpp_claim_printer_context_cleanup(gcpp_registration_claim_printer_context_t *reg_completion);
void gcpp_authorization_tokens_context_cleanup(gcpp_authorization_tokens_context_t *auth_tokens);

//--------------------------------------
// Local Function Definitions
//--------------------------------------

/** 
 * \brief Setup Google Cloud Print Proxy
 * 
 **/
static void gcpp_setup( void )
{
    gcpp_error_t rcode;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    ASSERT(gcpp_context.oem_config);
    
    /* retrieve GCPP enabled state from NVRAM */
    net_gcpp_is_enabled(true);
    
    /* retrieve GCPP registered state from NVRAM */
    net_gcpp_is_registered(true);
    
    /* retrieve GCPP proxy from NVRAM storage */
    net_gcpp_get_proxy(true);
    
    /* retrieve GCPP printer name from NVRAM storage */
    net_gcpp_get_printer_name(true);
    
    /* retrieve GCPP display name from NVRAM storage */
    net_gcpp_get_display_name(true);
    
    /* retrieve XMPP JID from NVRAM storage */
    net_gcpp_get_xmpp_jid(true);
    
    /* retrieve GCPP refresh token from NVRAM storage*/
    net_gcpp_get_refresh_token(true);
        
    /* retrieve GCPP http_proxy from NVRAM storage */
    if (net_gcpp_is_http_proxy_enabled(true))
    {
        net_gcpp_get_http_proxy_server(true);
        net_gcpp_get_http_proxy_port(true);
        
        if (net_gcpp_is_http_proxy_auth_required(true))
        {
            net_gcpp_get_http_proxy_username(true);
            net_gcpp_get_http_proxy_password(true);
        }
    }
    
    GCPP_GLOBAL_LOCK();
    /* clear GCPP xsrf token */
    if (gcpp_context.xsrf_token)
    {
        MEM_FREE_AND_NULL(gcpp_context.xsrf_token);
        gcpp_context.xsrf_token = NULL;
    }
    GCPP_GLOBAL_UNLOCK();

    
    /* execute OEM-specific setup */
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->setup)
    {
        rcode = gcpp_context.oem_func_table->setup();
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }

    
    /* if GCPP enabled, set enabled flag */
    if (net_gcpp_is_enabled(false))
    {
        gcpp_set_flags( GCPP_FLAG_ENABLED );
    }
    else
    {
        gcpp_clear_flags( GCPP_FLAG_ENABLED );
    }
    
    /* if GCPP registered, set register flag */
    if (net_gcpp_is_registered(false))
    {
        gcpp_set_flags( GCPP_FLAG_REGISTERED );
    }
    else
    {
        gcpp_clear_flags( GCPP_FLAG_REGISTERED );
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief Prepare Google Cloud Print Proxy for start
 * 
 **/
static void gcpp_start( void )
{
    gcpp_error_t rcode;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* execute OEM-specific start */
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->start)
    {
        rcode = gcpp_context.oem_func_table->start();
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief Prepare Google Cloud Print Proxy for stopped
 * 
 **/
static void gcpp_stop( void )
{
    gcpp_error_t rcode;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    

    /* execute OEM-specific stop */
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->stop)
    {
        rcode = gcpp_context.oem_func_table->stop();
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}


/** 
 * \brief Cleanup  after Google Cloud Print Proxy has stopped
 * 
 **/
static void gcpp_cleanup( void )
{
    gcpp_error_t rcode;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* execute OEM-specific cleanup */
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->cleanup)
    {
        rcode = gcpp_context.oem_func_table->cleanup();
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    GCPP_GLOBAL_LOCK();
    
    if (gcpp_context.printer_id != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.printer_id);
        gcpp_context.printer_id = NULL;
    }
    
    if (gcpp_context.printer_name != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.printer_name);
        gcpp_context.printer_name = NULL;
    }
    
    if (gcpp_context.display_name != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.display_name);
        gcpp_context.display_name = NULL;
    }
    
    if (gcpp_context.proxy != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.proxy);
        gcpp_context.proxy = NULL;
    }
    
    if (gcpp_context.http_proxy_server != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.http_proxy_server);
        gcpp_context.http_proxy_server = NULL;
    }
    
    if (gcpp_context.http_proxy_username != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.http_proxy_username);
        gcpp_context.http_proxy_username = NULL;
    }
    
    if (gcpp_context.http_proxy_password != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.http_proxy_password);
        gcpp_context.http_proxy_password = NULL;
    }

    if (gcpp_context.xsrf_token)
    {
        MEM_FREE_AND_NULL(gcpp_context.xsrf_token);
        gcpp_context.xsrf_token = NULL;
    }

    if (gcpp_context.xmpp_jid)
    {
        MEM_FREE_AND_NULL(gcpp_context.xmpp_jid);
        gcpp_context.xmpp_jid = NULL;
    }
        
    if (gcpp_context.refresh_token)
    {
        MEM_FREE_AND_NULL(gcpp_context.refresh_token);
        gcpp_context.refresh_token = NULL;
    }

    if (gcpp_context.anonymous_registration)
    {
        gcpp_register_anonymous_context_cleanup(gcpp_context.anonymous_registration);
        MEM_FREE_AND_NULL(gcpp_context.anonymous_registration);
        gcpp_context.anonymous_registration = NULL;
    }

    if (gcpp_context.claim_printer)
    {
        gcpp_claim_printer_context_cleanup(gcpp_context.claim_printer);
        MEM_FREE_AND_NULL(gcpp_context.claim_printer);
        gcpp_context.claim_printer = NULL;
    }

    if (gcpp_context.auth_tokens)
    {
        gcpp_authorization_tokens_context_cleanup(gcpp_context.auth_tokens);
        MEM_FREE_AND_NULL(gcpp_context.auth_tokens);
        gcpp_context.auth_tokens = NULL;
    }
    
    GCPP_GLOBAL_UNLOCK();
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief Update GCPP run and job states
 * 
 **/
void gcpp_update_states(gcpp_run_state_t run, gcpp_job_state_t job, gcpp_error_t result )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    GCPP_GLOBAL_LOCK();    
    
    if (gcpp_run_state == run && gcpp_job_state == job)
    {
        /* no state changes, just exit */
        GCPP_GLOBAL_UNLOCK();
        return;
    }
    
    if (gcpp_run_state != run)
    {
        gcpp_run_state = run;
    }
    else
    {
        run = GCPP_UNCHANGED;
    }
    
    if (gcpp_job_state != job)
    {
        gcpp_job_state = job;
    }
    else
    {
        job = GCPP_JOBS_UNCHANGED;
    }
    
    GCPP_GLOBAL_UNLOCK();

    /* execute OEM-specific event STATE_CHANGED */
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->event_notification)
    {
        gcpp_state_context_t states;
        
        memset(&states, 0, sizeof(gcpp_state_context_t));
        states.hdr.success = (result == NET_GCPP_SUCCESS);
        states.hdr.result = result;
        states.run_state = run;
        states.job_state = job;
        
        gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_PRINTER_STATE_CHANGED, &states, NULL);
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief generates a new (rolling) stanza ID for GCPP xmpp requests
 * 
 **/
static char gcpp_stanza_id_counter = 0;
static char gcpp_stanza_id_buffer[32] = {0};
char *net_gcpp_new_stanza_id(xmpp_conn_t * const conn)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    sprintf(gcpp_stanza_id_buffer, "GCPP%X", gcpp_stanza_id_counter++);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return &gcpp_stanza_id_buffer[0];
}

/** 
 * \brief GCPP message event handler
 * 
 **/
int gcpp_message_handler(xmpp_conn_t * const conn, xmpp_stanza_t * const stanza, void * const userdata)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    char *from = xmpp_stanza_get_attribute(stanza, "from");

    DBG_VERBOSE("Received message request from %s\n", from);
    
    if (strcmp(from, "cloudprint.google.com") == 0) 
    {
        /* indicate jobs are now pending in the queue */
        gcpp_update_states(gcpp_run_state, GCPP_JOBS_PENDING, NET_GCPP_SUCCESS);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return 1;
}

/** 
 * \brief GCPP handler for connection events
 * 
 **/
void gcpp_conn_handler(xmpp_conn_t * const conn, const xmpp_conn_event_t status, 
          const int error, xmpp_stream_error_t * const stream_error,
          void * const userdata)
{
    net_gcpp_context_t *gcpp = (net_gcpp_context_t *)userdata;
    xmpp_stanza_t *iq, *subscribe, *item;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (status == XMPP_CONN_CONNECT) 
    {
        GCPP_GLOBAL_LOCK();
        gcpp_context.xmpp_connected = true;
        GCPP_GLOBAL_UNLOCK();
        
        /* execute OEM-specific event XMPP_CONNECTED */
        ASSERT(gcpp_context.oem_func_table);
        if(gcpp_context.oem_func_table->event_notification)
        {
            gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_XMPP_CONNECTED, NULL, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        /* after connecting, reset xmpp cycle port to start */
        gcpp_xmpp_cycle_port = 0;
        
        xmpp_handler_add(conn, gcpp_message_handler, NULL, "message", NULL, gcpp);
        
        /* create iq stanza for request */
        iq = xmpp_stanza_new(gcpp->ctx);
        xmpp_stanza_set_name(iq, "iq");
        xmpp_stanza_set_type(iq, "set");
        xmpp_stanza_set_id(iq, net_gcpp_new_stanza_id(conn));
        xmpp_stanza_set_attribute(iq, "to", gcpp->xmpp_jid);

        subscribe = xmpp_stanza_new(gcpp->ctx);
        xmpp_stanza_set_name(subscribe, "subscribe");
        xmpp_stanza_set_attribute(subscribe, "xmlns", "google:push");

        item = xmpp_stanza_new(gcpp->ctx);
        xmpp_stanza_set_name(item, "item");
        xmpp_stanza_set_attribute(item, "channel", "cloudprint.google.com");
        xmpp_stanza_set_attribute(item, "from", "cloudprint.google.com");
        
        xmpp_stanza_add_child(subscribe, item);
        xmpp_stanza_add_child(iq, subscribe);

        /* we can release the stanzas since they belong to iq now */
        xmpp_stanza_release(item);
        xmpp_stanza_release(subscribe);

        /* send out the stanza */
        xmpp_send(conn, iq);

        /* release the stanza */
        xmpp_stanza_release(iq);
    }
    else 
    {
        int result = -1; /* default to being disconnected */
        
        /* cycle connection through all possible alternate XMPP ports */
        if ((!gcpp_context.xmpp_connected) &&
            (gcpp_xmpp_cycle_port < gcpp_context.oem_config->xmpp_num_alt_ports) && 
            (gcpp_xmpp_cycle_port < GCPP_MAX_XMPP_PORTS) 
            )
        {
            /* release the XMPP connection */
            if (gcpp_context.conn)
            {
                xmpp_conn_release(gcpp_context.conn);
                gcpp_context.conn = NULL;
            }

            gcpp_context.conn = xmpp_conn_new(gcpp_context.ctx);
            if (gcpp_context.conn)
            {
                /* set default timeout for connection */
                gcpp_context.conn->connect_timeout = gcpp_context.oem_config->xmpp_connect_timeout_ms;

                /* set XMPP jid and access token */
                xmpp_conn_set_jid(gcpp_context.conn, (net_gcpp_get_xmpp_jid(false) == NULL) ? "" : net_gcpp_get_xmpp_jid(false));
                xmpp_conn_set_auth_token(gcpp_context.conn, (net_gcpp_get_access_token() == NULL) ? "" : net_gcpp_get_access_token());
                
                uint32_t xmpp_port = gcpp_context.oem_config->xmpp_alternate_ports[gcpp_xmpp_cycle_port++];
                    
                DBG_MSG("Retry XMPP connection on port %d\n", xmpp_port);
                result = xmpp_connect_client(gcpp_context.conn, gcpp_context.oem_config->xmpp_domain, (unsigned short)xmpp_port, gcpp_conn_handler, &gcpp_context);
            }
        }
        
        /* determine if XMPP should actually be disconnected */
        if (result == -1)
        {
            DBG_MSG("Disconnecting XMPP connection\n");
            
            GCPP_GLOBAL_LOCK();
            gcpp_context.xmpp_connected = false;
            GCPP_GLOBAL_UNLOCK();
        
            /* reset xmpp cycle port to start */
            gcpp_xmpp_cycle_port = 0;
        
            /* execute OEM-specific event XMPP_DISCONNECTED */
            ASSERT(gcpp_context.oem_func_table);
            if(gcpp_context.oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_XMPP_DISCONNECTED, NULL, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
            
            /* stop XMPP */
            DBG_MSG("Stop XMPP connection\n");
            xmpp_stop(gcpp->ctx);
        }
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief GCPP main program thread
 * 
 **/
static void* gcpp_main(void*  input )
{
    ULONG requested_flags;
    ULONG actual_flags;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* wait for all levels to start */
    SysWaitForInit( );
    
    // TODO must account for multiple simultaneous interfaces
//    unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);
    
//    gcpp_iface_handle = net_iface_register_events( if_index,
//                                                   NET_IFACE_EVENT_IPV4_CONFIG_CHANGE | NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING, 
//                                                   gcpp_network_event_func, 
//                                                   NULL );
    
    net_gcpp_var_init();
    

#ifdef DEBUG
    /* set default debug levels */
    GCPP_GLOBAL_LOCK();
    gcpp_context.debug_curl_level = GCPP_CURL_DEBUG_LEVEL;
    gcpp_context.debug_xmpp_level = GCPP_XMPP_DEBUG_LEVEL;
    gcpp_context.debug_xmpp_enabled = TRUE;
    GCPP_GLOBAL_UNLOCK();
#endif
    
    while ( 1 )
    {
        uint32_t px_res;
        
        /* initialize run and job states */
        gcpp_update_states(GCPP_INIT, GCPP_JOBS_INIT, NET_GCPP_SUCCESS);

        /* setup Google Cloud Print Proxy */
        gcpp_setup( );

        gcpp_update_states(GCPP_WAIT_FOR_CONFIGURED, gcpp_job_state, NET_GCPP_SUCCESS);
        
        /* wait for IPV4 to be configure, GCPP to be enabled and registered */
        actual_flags = 0;
        requested_flags = GCPP_FLAG_IPV4_CONFIGURED | GCPP_FLAG_ENABLED | GCPP_FLAG_REGISTERED;

        pthread_mutex_lock(&gcpp_ready_mutex);

        // Check if all flags are set
        if (!((gcpp_ready_flags & GCPP_FLAG_IPV4_CONFIGURED) && 
                 (gcpp_ready_flags & GCPP_FLAG_ENABLED)         &&
                 (gcpp_ready_flags & GCPP_FLAG_REGISTERED)))
        { 
          // Check for blocking with wait forever
          if (gcpp_context.oem_config->start_timeout_ms == POSIX_WAIT_FOREVER) 
          {
            px_res = pthread_cond_wait(&gcpp_ready_cond, &gcpp_ready_mutex);
          }
          else
          {
            struct timespec wait_time;
            clock_gettime( CLOCK_REALTIME, &wait_time );
            posix_calc_future_usec(&wait_time, 
                                   (gcpp_context.oem_config->start_timeout_ms*USEC_PER_MILLISEC));
            px_res = pthread_cond_timedwait(&gcpp_ready_cond, &gcpp_ready_mutex, &wait_time);
          }
        }
        else
        {
          px_res = 0;
        }
        actual_flags = gcpp_ready_flags;
        pthread_mutex_unlock(&gcpp_ready_mutex);
//        tx_res = tx_event_flags_get( &gcpp_ready_flags, requested_flags, TX_AND, &actual_flags, gcpp_context.oem_config->start_timeout_ms);
//       if ((TX_SUCCESS == tx_res) && (actual_flags & GCPP_FLAG_REGISTERED) && (actual_flags & GCPP_FLAG_ENABLED) && (actual_flags & GCPP_FLAG_IPV4_CONFIGURED) )

        /* if GCPP is registered, enabled and IPV4 is configured, then begin GCPP */
        if ((px_res == 0) && 
            (actual_flags & GCPP_FLAG_REGISTERED) && 
            (actual_flags & GCPP_FLAG_ENABLED) && 
            (actual_flags & GCPP_FLAG_IPV4_CONFIGURED) )
        {
            /* indicate GCPP has been configured */
            gcpp_update_states(GCPP_CONFIGURED, gcpp_job_state, NET_GCPP_SUCCESS);
            
            if (net_gcpp_get_access_token() != NULL)
            {
                /* attempt to get current printer id */
                if (net_gcpp_get_printer_id(true) != NULL)
                {
                    /* start Google Cloud Print Proxy */
                    gcpp_start( );
                    
                    /* indicate GCPP has started and is waiting for a job */
                    gcpp_update_states(GCPP_STARTED, GCPP_JOBS_IDLE, NET_GCPP_SUCCESS);
                        
                    while ( gcpp_run_state == GCPP_STARTED && (gcpp_job_state == GCPP_JOBS_IDLE || gcpp_job_state == GCPP_JOBS_PENDING) )
                    {
                        xmpp_log_t *log = NULL;

                        int result = 0;
                                                
                        /* setup the XMPP connection to GoogleTalk, this is used to get notifications for
                           any new print jobs submitted to this printer/proxy */
                        xmpp_initialize();

#ifdef DEBUG                            
                        /* SET XMPP logging level */
                        log = xmpp_get_default_logger(gcpp_context.debug_xmpp_level); 
#else
                        log = xmpp_get_default_logger(XMPP_LEVEL_ERROR); 
#endif
                        
#ifdef DEBUG                            
                        if (gcpp_context.debug_xmpp_enabled)
#endif
                        {
                            /* create an XMPP context and connection*/
                            gcpp_context.ctx = xmpp_ctx_new(NULL, log);
                            gcpp_context.conn = xmpp_conn_new(gcpp_context.ctx);
                        }
                        
                        if (gcpp_context.ctx && gcpp_context.conn)
                        {
                            /* set default timeout for connection */
                            gcpp_context.conn->connect_timeout = gcpp_context.oem_config->xmpp_connect_timeout_ms;

                            /* set XMPP jid and access token */
                            xmpp_conn_set_jid(gcpp_context.conn, (net_gcpp_get_xmpp_jid(false) == NULL) ? "" : net_gcpp_get_xmpp_jid(false));
                            xmpp_conn_set_auth_token(gcpp_context.conn, (net_gcpp_get_access_token() == NULL) ? "" : net_gcpp_get_access_token());
                            
                            uint32_t xmpp_port = gcpp_context.oem_config->xmpp_port; /* default XMPP port */
                                
                            DBG_MSG("Attempting XMPP connection on port %d\n", xmpp_port);
                            result = xmpp_connect_client(gcpp_context.conn, gcpp_context.oem_config->xmpp_domain, (unsigned short)xmpp_port, gcpp_conn_handler, &gcpp_context);
                            
                            /* check for XMPP connection failure */
                            if (result == -1)
                            {
                                DBG_ERR("XMPP connection failure on port %d\n", xmpp_port);
                                gcpp_context.ctx->loop_status = XMPP_LOOP_QUIT;
                            }
                            
                            /* start the event loop */
                            if (gcpp_context.ctx->loop_status == XMPP_LOOP_NOTSTARTED)
                            {
                                gcpp_context.ctx->loop_status = XMPP_LOOP_RUNNING;
                                
                                DBG_MSG("Waiting for print job notification...\n");

                                int loop_count_in_ms = 0;
                                while (gcpp_context.ctx->loop_status == XMPP_LOOP_RUNNING) 
                                {
                                    if (gcpp_job_state == GCPP_JOBS_PENDING)
                                    {
                                        /* reset back to idle */
                                        gcpp_update_states(gcpp_run_state, GCPP_JOBS_IDLE, NET_GCPP_SUCCESS);
                                    }
                                    
                                    if (gcpp_run_state != GCPP_STARTED)
                                    {
                                        DBG_MSG( "GCPP run state no longer started, exiting XMPP loop\n" );
                                        break;
                                    }
                                    
                                    if (gcpp_context.oem_config->xmpp_keep_alive_delay_ms > 0)
                                    {
                                        if (loop_count_in_ms > gcpp_context.oem_config->xmpp_keep_alive_delay_ms)
                                        {
                                            gcpp_xmpp_ping(gcpp_context.conn);
                                            loop_count_in_ms = 0;
                                        }
                                    }
                                    else
                                    {
                                        loop_count_in_ms = 0;
                                    }

                                    
                                    xmpp_run_once(gcpp_context.ctx, gcpp_context.oem_config->xmpp_event_timeout_ms);
                                    loop_count_in_ms += gcpp_context.oem_config->xmpp_event_timeout_ms;
                                    
                                    /* execute OEM-specific event XMPP_IDLE */
                                    ASSERT(gcpp_context.oem_func_table);
                                    if(gcpp_context.oem_func_table->event_notification)
                                    {
                                        gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_XMPP_IDLE, NULL, NULL);
                                        if(rcode != NET_GCPP_SUCCESS)
                                        {
                                            XASSERT(0, rcode);
                                        }
                                    }
                                }
                                
                                if (gcpp_context.ctx->loop_status == XMPP_LOOP_RUNNING) 
                                {
                                    xmpp_stop(gcpp_context.ctx);
                                }

                                if (gcpp_context.xmpp_connected)
                                {
                                    xmpp_disconnect(gcpp_context.conn);
                                    
                                    GCPP_GLOBAL_LOCK();
                                    gcpp_context.xmpp_connected = false;
                                    GCPP_GLOBAL_UNLOCK();
                                    
                                    /* execute OEM-specific event XMPP_DISCONNECTED */
                                    ASSERT(gcpp_context.oem_func_table);
                                    if(gcpp_context.oem_func_table->event_notification)
                                    {
                                        gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_XMPP_DISCONNECTED, NULL, NULL);
                                        if(rcode != NET_GCPP_SUCCESS)
                                        {
                                            XASSERT(0, rcode);
                                        }
                                    }
                                }
                                
                                gcpp_update_states(gcpp_run_state, GCPP_JOBS_END, NET_GCPP_SUCCESS);
                            }
                            else
                            {
                                DBG_ERR( "Invalid CTX loop state - state should be XMPP_LOOP_NOTSTARTED!\n" );
                                gcpp_update_states(gcpp_run_state, GCPP_JOBS_END, NET_GCPP_XMPP_CONNECTION_FAILED);
                            }
                        }
                        else
                        {
                            DBG_ERR( "Invalid XMPP context or connection!\n" );
                            gcpp_update_states(gcpp_run_state, GCPP_JOBS_END, NET_GCPP_XMPP_CONNECTION_FAILED);
                        }
                    
                        /* release the XMPP connection */
                        if (gcpp_context.conn)
                        {
                            xmpp_conn_release(gcpp_context.conn);
                            gcpp_context.conn = NULL;
                        }
                        
                        /* release the XMPP context */
                        if (gcpp_context.ctx)
                        {
                            xmpp_ctx_free(gcpp_context.ctx);
                            gcpp_context.ctx = NULL;
                        }

                        xmpp_shutdown();
                    }
                        
                    /* Stop the Google Cloud Print Proxy */
                    gcpp_stop( ); 
                }
                else
                {
                    DBG_ERR( "No current printer found, attempting to add printer...\n" );
                    if (net_gcpp_printer_add() != NET_GCPP_SUCCESS)
                    {
                        DBG_ERR( "ERROR: Cannot add Printer, might need to be registered...\n" );
                        posix_sleep_ticks(DEFAULT_DELAY_SECS*100);
                    }
                }
            }
            else
            {
                DBG_ERR( "No access token!\n" );

                if (net_gcpp_get_refresh_token(true))
                {
                    /* no access token, attempt to update auth tokens */
                    if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
                    {
                        DBG_ERR( "ERROR: Unable to refresh tokens, registeration may not yet be completed...\n" );
                        posix_sleep_ticks(DEFAULT_DELAY_SECS*100);
                    }
                }
                else
                {
                    DBG_ERR( "No refresh token, registeration may not yet be completed...\n" );
                    posix_sleep_ticks(DEFAULT_DELAY_SECS*100);
                }
            }
        }
        else
        {
            /* expect no events error here */
//            if (tx_res == TX_NO_EVENTS)
            if (px_res == ETIMEDOUT)
            {
                /* if GCPP is enabled, display reason for start failure */
                if (net_gcpp_is_enabled(false))
                {
                    DBG_MSG( "GCPP failed to start\n" );
                    
                    if (!(net_gcpp_is_registered(false)))
                    {
                        DBG_MSG( " - GCPP not registered\n" );
                    }
                    
                    if (!gcpp_context.network_connected)
                    {
                        DBG_MSG( " - Network is down\n" );
                    }
                }
            }
            else
            {
                DBG_ERR( "GCPP failed to start (px error = 0x%x)\n", px_res );
                posix_sleep_ticks(DEFAULT_DELAY_SECS*100);
            }
        }
        
        gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
        
        gcpp_cleanup();
        
        // prevent any tight loops due to failures
        posix_sleep_ticks(100);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    return 0;
}
#if 0
/** 
 * \brief GCPP network event handler
 * 
 **/
static void gcpp_network_event_func( net_iface_event_flags_t events, void *context )
{
    bool configured = false;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if ( events & NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING )
    {
        DBG_MSG( "Net down pending...\n" );
        
        gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_NETWORK_DISCONNECTED);
        
        GCPP_GLOBAL_LOCK();
        gcpp_context.network_connected = false;
        GCPP_GLOBAL_UNLOCK();
        
        gcpp_clear_flags( GCPP_FLAG_IPV4_CONFIGURED );
        
        /* execute OEM-specific event NETWORK_DISCONNECTED */
        ASSERT(gcpp_context.oem_func_table);
        if(gcpp_context.oem_func_table->event_notification)
        {
            gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_NETWORK_DISCONNECTED, NULL, NULL);
            if(rcode != NET_GCPP_SUCCESS)
            {
                XASSERT(0, rcode);
            }
        }
        
        net_iface_continue_shutdown( gcpp_iface_handle, NET_IFACE_EVENT_IPV4_SHUTDOWN_PENDING );
    }
    else if ( events & NET_IFACE_EVENT_IPV4_CONFIG_CHANGE )
    {
        // TODO must account for multiple simultaneous interfaces
        unsigned int if_index = if_nametoindex(NET_IFACE_INTERFACE_NAME);

        net_iface_get_var_ipv4_configured( if_index, &configured );
        
        if ( configured )
        {
            DBG_MSG( "Net up!\n" );
            
            if (gcpp_run_state == GCPP_CONFIGURED || gcpp_run_state == GCPP_STARTED)
            {
                /* was running and network state changed, so restart */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_NETWORK_CHANGED);
            }
        
            GCPP_GLOBAL_LOCK();
            gcpp_context.network_connected = true;
            GCPP_GLOBAL_UNLOCK();
            
            gcpp_set_flags( GCPP_FLAG_IPV4_CONFIGURED );
        
            /* execute OEM-specific event NETWORK_CONNECTED */
            ASSERT(gcpp_context.oem_func_table);
            if(gcpp_context.oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_NETWORK_CONNECTED, NULL, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
        }
        else 
        {
            DBG_MSG( "Net down!\n" );
        
            GCPP_GLOBAL_LOCK();
            gcpp_context.network_connected = false;
            GCPP_GLOBAL_UNLOCK();
            
            gcpp_clear_flags( GCPP_FLAG_IPV4_CONFIGURED );
            
            if (gcpp_run_state == GCPP_CONFIGURED || gcpp_run_state == GCPP_STARTED)
            {
                /* was running and network was disconnected, so stop */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_NETWORK_DISCONNECTED);
            }
            
            /* execute OEM-specific event NETWORK_DISCONNECTED */
            ASSERT(gcpp_context.oem_func_table);
            if(gcpp_context.oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_NETWORK_DISCONNECTED, NULL, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
        }
    }
    else
    {
        ASSERT( 0 );
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}
#endif
/** 
 * \brief set GCPP flags
 * 
 **/
void gcpp_set_flags( uint32_t flags )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    pthread_mutex_lock(&gcpp_ready_mutex);
    gcpp_ready_flags |= flags;
    pthread_cond_broadcast(&gcpp_ready_cond);
    pthread_mutex_unlock(&gcpp_ready_mutex);

//    tx_event_flags_set( &gcpp_ready_flags, flags, TX_OR );
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief clear GCPP flags
 * 
 **/
void gcpp_clear_flags( uint32_t flags )
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
      
    pthread_mutex_lock(&gcpp_ready_mutex);
    gcpp_ready_flags &= ~flags;
    pthread_cond_broadcast(&gcpp_ready_cond);
    pthread_mutex_unlock(&gcpp_ready_mutex);
    
//    tx_event_flags_set( &gcpp_ready_flags, ~flags, TX_AND );
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/**
 * \brief suspend GCPP async thread
 */
void gcpp_async_suspend()
{
  DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
  pthread_mutex_lock(&gcpp_suspend_mutex);
  gcpp_suspended = true;

  if(gcpp_suspended) 
  {
      pthread_cond_wait(&gcpp_resume_cond, &gcpp_suspend_mutex);
  }

  pthread_mutex_unlock(&gcpp_suspend_mutex);
  DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/**
 * \brief resume GCPP async thread
 */
void gcpp_async_resume()
{
  DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
  pthread_mutex_lock(&gcpp_suspend_mutex);
  gcpp_suspended = false;
  pthread_cond_broadcast(&gcpp_resume_cond);
  pthread_mutex_unlock(&gcpp_suspend_mutex);
  DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

/** 
 * \brief GCPP xmpp ping reply handler
 * 
 **/
int gcpp_handle_ping_reply(xmpp_conn_t * const conn,
         xmpp_stanza_t * const stanza,
         void * const userdata)
{
    net_gcpp_context_t *gcpp = (net_gcpp_context_t *)userdata;
    xmpp_ctx_t *ctx = gcpp->ctx;
    char *type;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    DBG_VERBOSE("Received reply request from %s\n", xmpp_stanza_get_attribute(stanza, "from"));
    
    type = xmpp_stanza_get_type(stanza);
    if (strcmp(type, "error") == 0) {
        DBG_ERR("ERROR: reply failed\n");

        /* disconnect */
        xmpp_stop(ctx);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return 1;
}

/** 
 * \brief issue GCPP xmpp ping
 * 
 **/
int gcpp_xmpp_ping(xmpp_conn_t * const conn)
{
    xmpp_stanza_t *iq, *ping;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* create iq stanza for ping */
    iq = xmpp_stanza_new(conn->ctx);
    xmpp_stanza_set_name(iq, "iq");
    xmpp_stanza_set_type(iq, "get");
    xmpp_stanza_set_id(iq, net_gcpp_new_stanza_id(conn));

    ping = xmpp_stanza_new(conn->ctx);
    xmpp_stanza_set_name(ping, "ping");
    xmpp_stanza_set_attribute(ping, "xmlns", "urn:xmpp:ping");

    xmpp_stanza_add_child(iq, ping);

    /* release the stanzas since they belong to iq now */
    xmpp_stanza_release(ping);

    /* set up reply handler */
    xmpp_id_handler_add(conn, gcpp_handle_ping_reply, "321", conn->ctx);

    /* send out the stanza */
    xmpp_send(conn, iq);

    /* release the stanza */
    xmpp_stanza_release(iq);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return 1;
}


/** 
 * \brief returns a pointer to the gcpp context
 * 
 **/
net_gcpp_context_t *gcpp_get_context(void)
{
    return &gcpp_context;
}


void gcpp_async_requests_insert_into_fifo(gcpp_async_request_t *request)
{
    gcpp_async_request_t *cur, **prev;
    
    ASSERT( request != NULL );

    GCPP_ASYNC_REQUESTS_LOCK();
    
    cur = (gcpp_async_request_t *)gcpp_async_context.requests_fifo;
    prev = (gcpp_async_request_t **)&gcpp_async_context.requests_fifo;

    // go to end of list
    while (cur)
    {
        prev = (gcpp_async_request_t **)&cur->next;
        cur = cur->next;
    }
    
    // add to end of fifo
    *prev = request;
    
    GCPP_ASYNC_REQUESTS_UNLOCK();
}

gcpp_async_request_t *gcpp_async_requests_remove_from_fifo()
{
    gcpp_async_request_t *cur, **fifo;

    GCPP_ASYNC_REQUESTS_LOCK();
    
    cur = (gcpp_async_request_t *)gcpp_async_context.requests_fifo;
    fifo = (gcpp_async_request_t **)&gcpp_async_context.requests_fifo;

    // remove from beginning of fifo and return
    if (cur)
    {
        *fifo = cur->next;
    }
    else
    {
        *fifo = NULL;
    }

    GCPP_ASYNC_REQUESTS_UNLOCK();

    return cur;
}

static void* gcpp_async_requests_thread(void* entry_input)
{
    gcpp_async_request_t *async_request = NULL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    while (1)
    {
        GCPP_ASYNC_LOCK();
        DBG_VERBOSE( "%s running\n", __FUNCTION__ );
        
        async_request = gcpp_async_requests_remove_from_fifo();
        while (async_request)
        {
            DBG_VERBOSE( "%s processing async request\n", __FUNCTION__ );
            async_request->async_func_ptr(async_request->gcpp_context, async_request->callback_context);
            MEM_FREE_AND_NULL(async_request);
            
            async_request = gcpp_async_requests_remove_from_fifo();
        }
        
        GCPP_ASYNC_UNLOCK();
        
        DBG_VERBOSE( "%s suspending\n", __FUNCTION__ );
        gcpp_async_suspend();
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );    
    return 0;
}

gcpp_error_t gcpp_async_submit(const char *name, gcpp_async_func_ptr_t async_func_ptr, void *callback_context)
{
    gcpp_async_request_t *async_request = NULL;

    DBG_VERBOSE( "%s building async request\n", __FUNCTION__ );
    async_request = (gcpp_async_request_t *)MEM_MALLOC(sizeof(gcpp_async_request_t));
    if(!async_request)
    {
        DBG_VERBOSE( "%s building async request ERROR!\n", __FUNCTION__ );
        goto error;
    }

    memset(async_request, 0, sizeof(gcpp_async_request_t));
    async_request->name = name;
    async_request->async_func_ptr = async_func_ptr;
    async_request->callback_context = callback_context;
    async_request->gcpp_context = &gcpp_context;

    DBG_VERBOSE( "%s insert async request into fifo\n", __FUNCTION__ );
    gcpp_async_requests_insert_into_fifo(async_request);
    
    // check to see if async thread needs to be resumed
    if ( GCPP_ASYNC_IS_LOCKED() )
    {
        // able to obtain lock, async thread must need to be resumed
        DBG_VERBOSE( "%s resume async request thread\n", __FUNCTION__ );
        GCPP_ASYNC_UNLOCK();
        gcpp_async_resume();
    }
    
    return NET_GCPP_SUCCESS;

error:
    // cleanup
    if(async_request)
    {
        MEM_FREE_AND_NULL(async_request);
    }
    
    return NET_GCPP_FATAL_ERROR;
}

void gcpp_register_anonymous_context_cleanup(gcpp_registration_anonymous_context_t *anon_reg)
{
    if (anon_reg)
    {
        anon_reg->hdr.success = false;
        anon_reg->hdr.result = 0;
        anon_reg->hdr.response_code = 0;
        anon_reg->hdr.error_code = 0;
        
        if (anon_reg->registration_token)
        {
            MEM_FREE_AND_NULL(anon_reg->registration_token);
            anon_reg->registration_token = NULL;
        }
            
        if (anon_reg->token_duration)
        {
            MEM_FREE_AND_NULL(anon_reg->token_duration);
            anon_reg->token_duration = NULL;
        }
            
        if (anon_reg->invite_url)
        {
            MEM_FREE_AND_NULL(anon_reg->invite_url);
            anon_reg->invite_url = NULL;
        }
            
        if (anon_reg->complete_invite_url)
        {
            MEM_FREE_AND_NULL(anon_reg->complete_invite_url);
            anon_reg->complete_invite_url = NULL;
        }
            
        if (anon_reg->invite_page_url)
        {
            MEM_FREE_AND_NULL(anon_reg->invite_page_url);
            anon_reg->invite_page_url = NULL;
        }
            
        if (anon_reg->polling_url)
        {
            MEM_FREE_AND_NULL(anon_reg->polling_url);
            anon_reg->polling_url = NULL;
        }

        if (anon_reg->printer_id)
        {
            MEM_FREE_AND_NULL(anon_reg->printer_id);
            anon_reg->printer_id = NULL;
        }        
    }
}

gcpp_error_t gcpp_register_anonymous_request(net_gcpp_context_t *pContext, void *callback_context)
{
    gcpp_error_t ret;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );//DBG_VERBOSE
    
    if (net_gcpp_is_registration_in_process())
    {
        return NET_GCPP_NOT_PERMITTED;
    }
    
    GCPP_REGISTRATION_LOCK();
    
    GCPP_GLOBAL_LOCK();
    if (pContext->anonymous_registration)
    {
        gcpp_register_anonymous_context_cleanup(pContext->anonymous_registration);
        MEM_FREE_AND_NULL(pContext->anonymous_registration);
    }

    pContext->anonymous_registration = MEM_MALLOC(sizeof(gcpp_registration_anonymous_context_t));
    ASSERT(pContext->anonymous_registration);
    memset(pContext->anonymous_registration, 0, sizeof(gcpp_registration_anonymous_context_t));
    GCPP_GLOBAL_UNLOCK();

    ret = gcpp_initiate_anonymous_registration(pContext, pContext->anonymous_registration, callback_context);
    
    GCPP_REGISTRATION_UNLOCK();
    DBG_VERBOSE( "%s exit ret %d\n", __FUNCTION__ , ret);//DBG_VERBOSE
    
    return ret;
}

void gcpp_claim_printer_context_cleanup(gcpp_registration_claim_printer_context_t *reg_completion)
{
    if (reg_completion)
    {
        reg_completion->hdr.success = false;
        reg_completion->hdr.result = 0;
        reg_completion->hdr.response_code = 0;
        reg_completion->hdr.error_code = 0;
        
        if (reg_completion->xmpp_jid)
        {
            MEM_FREE_AND_NULL(reg_completion->xmpp_jid);
            reg_completion->xmpp_jid = NULL;
        }
            
        if (reg_completion->authorization_code)
        {
            MEM_FREE_AND_NULL(reg_completion->authorization_code);
            reg_completion->authorization_code = NULL;
        }
            
        if (reg_completion->user_email)
        {
            MEM_FREE_AND_NULL(reg_completion->user_email);
            reg_completion->user_email = NULL;
        }
            
        if (reg_completion->confirmation_page_url)
        {
            MEM_FREE_AND_NULL(reg_completion->confirmation_page_url);
            reg_completion->confirmation_page_url = NULL;
        }

        if (reg_completion->printerid)
        {
            MEM_FREE_AND_NULL(reg_completion->printerid);
            reg_completion->printerid = NULL;
    }
}
}

gcpp_error_t gcpp_claim_printer_request(net_gcpp_context_t *pContext, void *request_context)
{
    gcpp_error_t ret;
    gcpp_request_params_t *reg_params = (gcpp_request_params_t *)request_context;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    GCPP_REGISTRATION_LOCK();

    GCPP_GLOBAL_LOCK();
    if (pContext->claim_printer != NULL)
    {
        gcpp_claim_printer_context_cleanup(pContext->claim_printer);
        MEM_FREE_AND_NULL(pContext->claim_printer);
    }
    
    pContext->claim_printer = MEM_MALLOC(sizeof(gcpp_registration_claim_printer_context_t));
    ASSERT(pContext->claim_printer);
    memset(pContext->claim_printer, 0, sizeof(gcpp_registration_claim_printer_context_t));
    GCPP_GLOBAL_UNLOCK();
    
    /* determine if printer has been claimed */
    ret = gcpp_claim_printer(pContext, (char *)reg_params->request_param, pContext->claim_printer, reg_params->callback_context);

    /* cleanup */
    if (reg_params->request_param)
        MEM_FREE_AND_NULL(reg_params->request_param);
        
    MEM_FREE_AND_NULL(reg_params);
    
    GCPP_REGISTRATION_UNLOCK();
    
    return ret;
}

void gcpp_authorization_tokens_context_cleanup(gcpp_authorization_tokens_context_t *auth_tokens)
{
    if (auth_tokens)
    {
        auth_tokens->hdr.success = false;
        auth_tokens->hdr.result = 0;
        auth_tokens->hdr.response_code = 0;
        auth_tokens->hdr.error_code = 0;
        
        if (auth_tokens->access_token)
        {
            MEM_FREE_AND_NULL(auth_tokens->access_token);
            auth_tokens->access_token = NULL;
        }
            
        if (auth_tokens->refresh_token)
        {
            MEM_FREE_AND_NULL(auth_tokens->refresh_token);
            auth_tokens->refresh_token = NULL;
        }
    }
}

gcpp_error_t gcpp_authorization_tokens_request(net_gcpp_context_t *pContext, void *request_context)
{
    gcpp_error_t ret;
    gcpp_request_params_t *reg_params = (gcpp_request_params_t *)request_context;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    GCPP_REGISTRATION_LOCK();

    GCPP_GLOBAL_LOCK();
    if (pContext->auth_tokens != NULL)
    {
        gcpp_authorization_tokens_context_cleanup(pContext->auth_tokens);
        MEM_FREE_AND_NULL(pContext->auth_tokens);
    }
    
    pContext->auth_tokens = MEM_MALLOC(sizeof(gcpp_authorization_tokens_context_t));
    ASSERT(pContext->auth_tokens);
    memset(pContext->auth_tokens, 0, sizeof(gcpp_authorization_tokens_context_t));
    GCPP_GLOBAL_UNLOCK();
    
    ret = gcpp_get_authorization_tokens(pContext, (char *)reg_params->request_param, pContext->auth_tokens, reg_params->callback_context);
    if (ret == NET_GCPP_SUCCESS)
    {
        DBG_VERBOSE("Auth Tokens request successful:\n");
        DBG_VERBOSE("  access_token = %s\n", pContext && pContext->auth_tokens ? 
                    pContext->auth_tokens->access_token : "0xdeadbeef");
        DBG_VERBOSE("  refresh_token = %s\n", pContext && pContext->auth_tokens ? 
                    pContext->auth_tokens->refresh_token : "0xdeadbeef");
    }
    
    /* cleanup */
    if (reg_params->request_param)
        MEM_FREE_AND_NULL(reg_params->request_param);
        
    MEM_FREE_AND_NULL(reg_params);
    
    GCPP_REGISTRATION_UNLOCK();

    if (ret == NET_GCPP_SUCCESS)
    {
        /* next, attempt to refresh xsrf token */
        gcpp_update_xsrf_token(pContext);
        if (ret == NET_GCPP_SUCCESS)
        {
            DBG_VERBOSE("Update of XSRF token request successful:\n");
            DBG_VERBOSE("  xsrf_token = %s\n", 
                        pContext->xsrf_token != NULL ?  pContext->xsrf_token : "NULL");
        }
    }
        
    return ret;
}

gcpp_error_t gcpp_refresh_authorization_tokens( net_gcpp_context_t *pContext, void *callback_context )
{
    gcpp_error_t ret;
    
    GCPP_REGISTRATION_LOCK();

    GCPP_GLOBAL_LOCK();
    if (pContext->auth_tokens != NULL)
    {
        gcpp_authorization_tokens_context_cleanup(pContext->auth_tokens);
        MEM_FREE_AND_NULL(pContext->auth_tokens);
    }
    
    pContext->auth_tokens = MEM_MALLOC(sizeof(gcpp_authorization_tokens_context_t));
    ASSERT(pContext->auth_tokens);
    memset(pContext->auth_tokens, 0, sizeof(gcpp_authorization_tokens_context_t));
    GCPP_GLOBAL_UNLOCK();

    ret = gcpp_update_authorization_tokens(pContext, pContext->refresh_token, pContext->auth_tokens, callback_context);
    if (ret == NET_GCPP_SUCCESS)
    {
        DBG_VERBOSE("Update of Auth Tokens request successful:\n");
        DBG_VERBOSE("  access_token = %s\n", 
                    (pContext->auth_tokens->access_token != NULL) ? 
                    pContext->auth_tokens->access_token : "NULL");
        DBG_VERBOSE("  refresh_token = %s\n", 
                    (pContext->auth_tokens->refresh_token != NULL) ?
                    pContext->auth_tokens->refresh_token : "NULL");
    }
    GCPP_REGISTRATION_UNLOCK();

    if (ret == NET_GCPP_SUCCESS)
    {
        /* next, attempt to refresh xsrf token */
        gcpp_update_xsrf_token(pContext);
        if (ret == NET_GCPP_SUCCESS)
        {
            DBG_VERBOSE("Update of XSRF token request successful:\n");
            DBG_VERBOSE("  xsrf_token = %s\n", 
						pContext->xsrf_token != NULL ?  pContext->xsrf_token : "NULL");
        }
    }
    
    return ret;
}


/*
 * CMD Callback Function Definitions
 */
#ifdef HAVE_CMD

/** 
 * \brief Display the GCPP context values
 * 
 **/
static void display_gcpp_context(const net_gcpp_context_t* pContext)
{
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    DBG_CMD("   GCPP Initialized       = %s\n", (pContext->initialized ? "Yes" : "No"));
    DBG_CMD("   GCPP Enabled           = %s\n", (net_gcpp_is_enabled(false) ? "Yes" : "No"));
    DBG_CMD("   GCPP Registered        = %s\n", (net_gcpp_is_registered(false) ? "Yes" : "No"));
        
    if (pContext->printer_id)
        DBG_CMD("   printer_id             = %s\n", pContext->printer_id);
    
    if (pContext->proxy)
        DBG_CMD("   proxy                  = %s\n", pContext->proxy);
        
    if (pContext->printer_name)
        DBG_CMD("   printer_name           = %s\n", pContext->printer_name);
        
    if (pContext->display_name)
        DBG_CMD("   display_name           = %s\n", pContext->display_name);
    
    if (pContext->http_proxy_enabled)
    {
        DBG_CMD("   http_proxy_server     = %s\n", pContext->http_proxy_server);
        DBG_CMD("   http_proxy_port       = %d\n", (int)pContext->http_proxy_port);
        
        if (pContext->http_proxy_auth_req)
        {
            DBG_CMD("   http_proxy_username   = %s\n", pContext->http_proxy_username);
            DBG_CMD("   http_proxy_password   = %s\n", pContext->http_proxy_password);
        }
    }
        
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}


/*
 * CMD Interface Routines
 */

static const char* _enable_desc = "Enables the Google Cloud Print Proxy";
static const char* _enable_usage = "";
static const char* _enable_notes = "";
static int _enable(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    if (!(net_gcpp_is_enabled(false)))
    {
        net_gcpp_enable(true);
    }
    
    return CMD_OK;
}

static const char* _printer_desc = "Display or Set the Google Cloud Printer Name";
static const char* _printer_usage = "<printer name>";
static const char* _printer_notes = "";
static int _printer(int argc, char* argv[])
{
    if (argc > 2) 
        return CMD_USAGE_ERROR;
        
    if (argc == 2)
	{
        net_gcpp_set_printer_name(argv[1]);
	}
	
	char *name = net_gcpp_get_printer_name(true);
	if (name) 
	{
		DBG_CMD("printer_name = %s\n", name);
	}
    
    return CMD_OK;
}

static const char* _restart_desc = "Restarts the Google Cloud Print Proxy";
static const char* _restart_usage = "";
static const char* _restart_notes = "";
static int _restart(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    if (net_gcpp_is_enabled(false))
    {
        net_gcpp_printer_restart();
    }
    
    return CMD_OK;
}

static const char* _disable_desc = "Disables the Google Cloud Print Proxy";
static const char* _disable_usage = "";
static const char* _disable_notes = "";
static int _disable(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    if (net_gcpp_is_enabled(false))
    {
        net_gcpp_enable(false);
    }
    
    return CMD_OK;
}

static const char* _info_desc = "Display Google Cloud Print Proxy information";
static const char* _info_usage = "";
static const char* _info_notes = "";
static int _info(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    display_gcpp_context(&gcpp_context);
    
    return CMD_OK;
}

static const char* _add_desc = "Add Google Cloud Print Proxy printer";
static const char* _add_usage = "";
static const char* _add_notes = "";
static int _add(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    /* 
        This must be initiated by user interaction, not automatically.
    */
    net_gcpp_printer_add();
    
    return CMD_OK;
}

static const char* _update_desc = "Update Google Cloud Print Proxy printer";
static const char* _update_usage = "";
static const char* _update_notes = "";
static int _update(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    /* 
        This must be initiated by user interaction, not automatically.
    */
    net_gcpp_printer_update();
    
    return CMD_OK;
}

static const char* _remove_desc = "Remove Google Cloud Print Proxy printer";
static const char* _remove_usage = "";
static const char* _remove_notes = "";
static int _remove(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;

    net_gcpp_printer_remove();
    
    return CMD_OK;
}

static const char* _register_desc = "Initiates anonymous Google Cloud Print Proxy registration";
static const char* _register_usage = "";
static const char* _register_notes = "";
static int local_cmd_register(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    /* 
        This must be initiated by user interaction, not automatically.
    */
    DBG_VERBOSE("%s() .. \n", __func__);
    gcpp_error_t rcode = net_gcpp_issue_register_anonymous_request(NULL, true);
    DBG_VERBOSE("%s() .. ret %d \n", __func__, rcode);
    if (rcode != NET_GCPP_SUCCESS)
    {
        DBG_ERR("Anonymous Registration request Failed, result = %d!\n", rcode);
    }
    
    return CMD_OK;
}

static const char* _claim_printer_desc = "Claim printer for anonymous Google Cloud Print Proxy registration completion";
static const char* _claim_printer_usage = "";
static const char* _claim_printer_notes = "";
static int _claim_printer(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    gcpp_registration_anonymous_context_t *anon_req = net_gcpp_get_anonymous_request_response();
    
    if (anon_req != NULL && 
        anon_req->hdr.success &&
        anon_req->polling_url != NULL) 
    {
        /* 
            Polling should not be more frequent than once every 30 seconds.  It is recommended this be initiated 
            by user interaction, right after the user is done with the previous step in the anonymous registration,
            to avoid the user having to wait for a new cycle.
        */
        gcpp_error_t rcode = net_gcpp_issue_claim_printer_request(anon_req->polling_url, NULL, true);
        if (rcode != NET_GCPP_SUCCESS)
        {
            DBG_ERR("Claim Printer request Failed, result = %d!\n", rcode);
        }
    }
    else
    {
        DBG_ERR("Unable to claim printer, Pulling URL not valid\n");
    }
    
    return CMD_OK;
}

static const char* _reset_desc = "Reset Google Cloud Print Proxy printer";
static const char* _reset_usage = "";
static const char* _reset_notes = "";
static int _reset(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;

    net_gcpp_printer_reset();
    
    return CMD_OK;
}


#ifdef DEBUG
static const char* _dbgon_desc = "Enable verbose debug messages";
static const char* _dbgon_usage = "";
static const char* _dbgon_notes = "";
static int _dbgon(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    GCPP_GLOBAL_LOCK();
    gcpp_context.debug_on = TRUE;
    gcpp_context.debug_curl_level = 1L;
    gcpp_context.debug_xmpp_level = XMPP_LEVEL_DEBUG;
    GCPP_GLOBAL_UNLOCK();
    
    return CMD_OK;
}

static const char* _dbgoff_desc = "Disable verbose debug messages";
static const char* _dbgoff_usage = "";
static const char* _dbgoff_notes = "";
static int _dbgoff(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    GCPP_GLOBAL_LOCK();
    gcpp_context.debug_on = FALSE;
    gcpp_context.debug_curl_level = 0L;
    gcpp_context.debug_xmpp_level = XMPP_LEVEL_ERROR;
    GCPP_GLOBAL_UNLOCK();
    
    return CMD_OK;
}
static const char* _xmppon_desc = "Enable xmpp messaging";
static const char* _xmppon_usage = "";
static const char* _xmppon_notes = "";
static int _xmppon(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    GCPP_GLOBAL_LOCK();
    gcpp_context.debug_xmpp_enabled = TRUE;
    GCPP_GLOBAL_UNLOCK();
    
    return CMD_OK;
}

static const char* _xmppoff_desc = "Disable xmpp messaging";
static const char* _xmppoff_usage = "";
static const char* _xmppoff_notes = "";
static int _xmppoff(int argc, char* argv[])
{
    if (argc != 1) 
        return CMD_USAGE_ERROR;
        
    GCPP_GLOBAL_LOCK();
    gcpp_context.debug_xmpp_enabled = FALSE;
    GCPP_GLOBAL_UNLOCK();
    
    return CMD_OK;
}
#endif // DEBUG

#endif //HAVE_CMD


/*
 * Public API Function Implmentations
 */

void net_gcpp_init( void )
{
    gcpp_error_t rcode;
    uint32_t pthread_rcode;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    /* initialize context structures */
    memset(&gcpp_context, 0, sizeof(gcpp_context));
    memset(&gcpp_async_context, 0, sizeof(gcpp_async_context));

    /* initialize and fetch OEM-specific configuration functions */
    memset(&gcpp_oem_print_func_table, 0, sizeof(gcpp_oem_print_func_table));
    rcode = gcpp_oem_print_populate_func_table(&gcpp_oem_print_func_table);
    ASSERT(NET_GCPP_SUCCESS == rcode);
    gcpp_context.oem_func_table = &gcpp_oem_print_func_table;
    
    /* initialize and fetch OEM-specific configuration values */
    memset(&gcpp_oem_print_config, 0, sizeof(gcpp_oem_print_config));
    rcode = gcpp_oem_print_populate_config(&gcpp_oem_print_config);
    ASSERT(NET_GCPP_SUCCESS == rcode);
    
    /* ensure defaults are set */
    if (gcpp_oem_print_config.cloudprint_url == NULL)
        gcpp_oem_print_config.cloudprint_url = GCPP_DEFAULT_CLOUDPRINT_URL;
        
    if (gcpp_oem_print_config.oauth2_url == NULL)
        gcpp_oem_print_config.oauth2_url = GCPP_DEFAULT_OAUTH2_URL;
        
    if (gcpp_oem_print_config.xmpp_domain == NULL)
        gcpp_oem_print_config.xmpp_domain = GCPP_DEFAULT_XMPP_DOMAIN;
        
    gcpp_context.oem_config = &gcpp_oem_print_config;
    
    /* create GCPP context global lock mutex */
//    threadx_rcode = tx_mutex_create(&gcpp_context.global_lock_mutex, "gcppLock", TX_INHERIT);
    pthread_rcode = posix_mutex_init(&gcpp_context.global_lock_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating GCPP global_lock_mutex (pthread_rcode=0x%02x)\n", pthread_rcode);
        XASSERT(pthread_rcode == 0, pthread_rcode);
    }

    /* create GCPP context registration lock mutex */
    pthread_rcode = posix_mutex_init(&gcpp_context.registration_lock_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating GCPP registration_lock_mutex (pthread_rcode=0x%02x)\n",
                 pthread_rcode);
        XASSERT(pthread_rcode == 0, pthread_rcode);
    }
    
    /* create the event flags that are used to signal that various interrupts have occurred */
//    tx_res = tx_event_flags_create( &gcpp_ready_flags, "gcpp_ready_flags" );
//    XASSERT( tx_res == 0, tx_res );

    /* create async context lock mutex */
    pthread_rcode = posix_mutex_init(&gcpp_async_context.mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating async context lock mutex (pthread_rcode=0x%02x)\n", 
                pthread_rcode);
        XASSERT(pthread_rcode == 0, pthread_rcode);
    }

    /* create async context requests mutex */
    pthread_rcode = posix_mutex_init(&gcpp_async_context.requests_mutex);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating async context lock mutex (pthread_rcode=0x%02x)\n", 
                pthread_rcode);
        XASSERT(pthread_rcode == 0, pthread_rcode);
    }
    
#ifdef HAVE_CMD
    cmd_register_cmd("gcpp", NULL, "GCPP Commands", NULL, NULL, NULL);

    cmd_register_subcmd("gcpp", "info", 
                            _info_desc, _info_usage, _info_notes, _info);
    
    cmd_register_subcmd("gcpp", "enable", 
                            _enable_desc, _enable_usage, _enable_notes, _enable);
    cmd_register_subcmd("gcpp", "printer", 
                            _printer_desc, _printer_usage, _printer_notes, _printer);
							
    cmd_register_subcmd("gcpp", "restart", 
                            _restart_desc, _restart_usage, _restart_notes, _restart);
    cmd_register_subcmd("gcpp", "disable", 
                            _disable_desc, _disable_usage, _disable_notes, _disable);
                            
    cmd_register_subcmd("gcpp", "add",
                            _add_desc, _add_usage, _add_notes, _add);
    cmd_register_subcmd("gcpp", "update",
                            _update_desc, _update_usage, _update_notes, _update);
    cmd_register_subcmd("gcpp", "remove",
                            _remove_desc, _remove_usage, _remove_notes, _remove);
                            
    cmd_register_subcmd("gcpp", "register",
                            _register_desc, _register_usage, _register_notes, local_cmd_register);
    cmd_register_subcmd("gcpp", "claim",
                            _claim_printer_desc, _claim_printer_usage, _claim_printer_notes, _claim_printer);
    cmd_register_subcmd("gcpp", "reset",
                            _reset_desc, _reset_usage, _reset_notes, _reset);
                            
#ifdef DEBUG 
    cmd_register_subcmd("gcpp", "dbgon", 
                            _dbgon_desc, _dbgon_usage, _dbgon_notes, _dbgon);
    cmd_register_subcmd("gcpp", "dbgoff", 
                            _dbgoff_desc, _dbgoff_usage, _dbgoff_notes, _dbgoff);
    cmd_register_subcmd("gcpp", "xmppon", 
                            _xmppon_desc, _xmppon_usage, _xmppon_notes, _xmppon);
    cmd_register_subcmd("gcpp", "xmppoff", 
                            _xmppoff_desc, _xmppoff_usage, _xmppoff_notes, _xmppoff);
#endif // DEBUG

#endif // HAVE_CMD
    
    // execute OEM-specific configuration init
    ASSERT(gcpp_context.oem_func_table);
    if(gcpp_context.oem_func_table->init)
    {
        rcode = gcpp_context.oem_func_table->init();
        if(rcode != NET_GCPP_SUCCESS)
        {
            XASSERT(0, rcode);
        }
    }
    
    /* create and kick-off GCPP async request thread */
    posix_create_thread(&gcpp_async_request_thread, 
                        gcpp_async_requests_thread,
                        0,
                        "gcppASYNC",
                        gcpp_async_request_stack,
                        THREAD_STACK_SIZE,
                        POSIX_THR_PRI_NORMAL);

    /* create and kick-off GCPP main thread */
    posix_create_thread( &gcpp_run_thread,
                         gcpp_main,
                         0,
                         "GCPP",
                         gcpp_run_stack,
                         THREAD_STACK_SIZE,
                         POSIX_THR_PRI_NORMAL);

    /* indicate initialization is complete */
    gcpp_context.initialized = true;

    // revisit: Hard coding network connection state 
    // until this can be determined on the fly   
gcpp_set_flags( GCPP_FLAG_IPV4_CONFIGURED ); // no worries
gcpp_context.network_connected = true; //jp

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
}

bool net_gcpp_is_initialized( void )
{
    if (!gcpp_context.initialized)
        return false;
        
    return true;
}

bool net_gcpp_is_enabled( bool refresh )
{
    if (!gcpp_context.initialized)
        return false;
        
    if (refresh)
    {
        net_gcpp_get_var_enabled(&gcpp_context.enabled);
    }
        
    return (gcpp_context.enabled);
}

gcpp_error_t net_gcpp_enable( bool enable )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if trying to enable and GCPP is already enabled, just return NET_GCPP_SUCCESS */
    if (enable && net_gcpp_is_enabled(false))
        return NET_GCPP_SUCCESS;
        
    /* if trying to disable and GCPP is already disbled, just return NET_GCPP_SUCCESS */
    if (!enable && !net_gcpp_is_enabled(false))
        return NET_GCPP_SUCCESS;

    /* attempt to set GCPP enable var */
    ret = net_gcpp_set_var_enabled(enable);
    if (ret == NET_GCPP_SUCCESS)
    {
        /* set GCPP enabled context */
        gcpp_context.enabled = enable;
        
        /* if being enabled */
        if (enable)
        {
            /* if GCPP is currently running */
            if (gcpp_run_state == GCPP_STARTED)
            {
                /* stop GCPP */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
            }
            
            /* set GCPP enabled event flag */
            gcpp_set_flags( GCPP_FLAG_ENABLED );
        }
        /* if being disabled */
        else 
        {
            /* if GCPP is currently running */
            if (gcpp_run_state == GCPP_STARTED)
            {
                /* stop GCPP */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
            }

            /* clear GCPP enabled event flag */
            gcpp_clear_flags( GCPP_FLAG_ENABLED );
        }
    }
    else
    {
        DBG_ERR("error setting GCPP Enabled config var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

gcpp_error_t net_gcpp_printer_restart( void )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if GCPP is currently disbled, just return NET_GCPP_SUCCESS */
    if (!net_gcpp_is_enabled(false))
        return NET_GCPP_SUCCESS;

    /* stop GCPP - this will trigger current GCPP process to end and then restart  */
    gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

bool net_gcpp_is_registered( bool refresh )
{
    if (!gcpp_context.initialized)
        return false;
        
    if (refresh)
    {
        net_gcpp_get_var_registered(&gcpp_context.registered);
    }
    
    return (gcpp_context.registered);
}

bool net_gcpp_is_registration_in_process( void )
{
    bool ret = false;
    
    if (!gcpp_context.initialized)
        return false;

    if (GCPP_REGISTRATION_IS_LOCKED())
    {
        /* check for registration in process */
        if (!net_gcpp_is_registered(false))
        {
            GCPP_GLOBAL_LOCK();
            if (gcpp_context.anonymous_registration && gcpp_context.anonymous_registration->hdr.success)
            {
                ret = true;
            }
            GCPP_GLOBAL_UNLOCK();
        }
        
        GCPP_REGISTRATION_UNLOCK();
    }
    else
    {
        /* unable to obtain lock, registration must be in progress */
        ret = true;
    }
    
    return ret;
}

gcpp_error_t net_gcpp_registered( bool registered )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    bool disable_event_notification = false;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if trying to register and GCPP is already registered, then don't send event notification */
    if (registered && net_gcpp_is_registered(false))
        disable_event_notification = true;
        
    /* if trying to unregister and GCPP is not registered, then don't send event notification */
    if (!registered && !net_gcpp_is_registered(false))
        disable_event_notification = true;

    /* attempt to set GCPP registered var */
    ret = net_gcpp_set_var_registered(registered);
    if (ret == NET_GCPP_SUCCESS)
    {
        if (!registered)
        {
            /* Attempt to remove registered GCPP printer */
            net_gcpp_printer_remove();
        }
        
        GCPP_GLOBAL_LOCK();
        /* set GCPP registered context */
        gcpp_context.registered = registered;

        if (gcpp_context.anonymous_registration)
        {
            gcpp_register_anonymous_context_cleanup(gcpp_context.anonymous_registration);
            MEM_FREE_AND_NULL(gcpp_context.anonymous_registration);
            gcpp_context.anonymous_registration = NULL;
        }

        if (gcpp_context.claim_printer)
        {
            gcpp_claim_printer_context_cleanup(gcpp_context.claim_printer);
            MEM_FREE_AND_NULL(gcpp_context.claim_printer);
            gcpp_context.claim_printer = NULL;
        }

        if (gcpp_context.auth_tokens)
        {
            gcpp_authorization_tokens_context_cleanup(gcpp_context.auth_tokens);
            MEM_FREE_AND_NULL(gcpp_context.auth_tokens);
            gcpp_context.auth_tokens = NULL;
        }
        GCPP_GLOBAL_UNLOCK();
        
        /* if state being registered */
        if (registered)
        {
            /* if GCPP is currently running */
            if (gcpp_run_state == GCPP_STARTED)
            {
                /* stop GCPP */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
            }
            
            /* set GCPP registered event flag */
            gcpp_set_flags( GCPP_FLAG_REGISTERED );
            
            /* execute OEM-specific event REGISTERED */
            ASSERT(gcpp_context.oem_func_table);
            if(!disable_event_notification && gcpp_context.oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_REGISTERED, NULL, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
        }
        /* if state being unregistered */
        else 
        {
            /* clear GCPP registered event flag */
            gcpp_clear_flags( GCPP_FLAG_REGISTERED );
            
            /* clear all registration vars */
            net_gcpp_set_xmpp_jid("");
            net_gcpp_set_refresh_token("");
            
            GCPP_GLOBAL_LOCK();
            /* clear printer id */
            if (gcpp_context.printer_id)
            {
                MEM_FREE_AND_NULL(gcpp_context.printer_id);
                gcpp_context.printer_id = NULL;
            }

            /* clear GCPP xsrf token */
            if (gcpp_context.xsrf_token)
            {
                MEM_FREE_AND_NULL(gcpp_context.xsrf_token);
                gcpp_context.xsrf_token = NULL;
            }
            GCPP_GLOBAL_UNLOCK();
            
            /* execute OEM-specific event UNREGISTERED */
            ASSERT(gcpp_context.oem_func_table);
            if(!disable_event_notification && gcpp_context.oem_func_table->event_notification)
            {
                gcpp_error_t rcode = gcpp_context.oem_func_table->event_notification(NET_GCPP_EVENT_UNREGISTERED, NULL, NULL);
                if(rcode != NET_GCPP_SUCCESS)
                {
                    XASSERT(0, rcode);
                }
            }
            
            /* if GCPP is currently running */
            if (gcpp_run_state == GCPP_STARTED)
            {
                /* stop GCPP */
                gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
            }
        }
    }
    else
    {
        DBG_ERR("error setting GCPP Registered config var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

bool net_gcpp_is_network_connected( void )
{
    if (!gcpp_context.initialized)
        return false;
        
    return (gcpp_context.network_connected);
}

bool net_gcpp_is_xmpp_connected( void )
{
    if (!gcpp_context.initialized)
        return false;
        
    return (gcpp_context.xmpp_connected);
}

char *net_gcpp_get_proxy( bool refresh )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        char temp[GCPP_MAX_NAME_LEN];
        
        /* retrieve proxy from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_proxy(&temp[0], sizeof(temp));

        GCPP_GLOBAL_LOCK();
        if (gcpp_context.proxy)
            MEM_FREE_AND_NULL(gcpp_context.proxy);
        gcpp_context.proxy = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
        
    return ( (gcpp_context.proxy) );
}

gcpp_error_t net_gcpp_set_proxy( char *proxy )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP proxy var */
    ret = net_gcpp_set_var_proxy(proxy);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.proxy)
            MEM_FREE_AND_NULL(gcpp_context.proxy);
            
        gcpp_context.proxy = gcpp_strdup(proxy);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP proxy var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_printer_name( bool refresh )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        char temp[GCPP_MAX_NAME_LEN];
        
        /* retrieve printer_name from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_printer_name(&temp[0], sizeof(temp));
        
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.printer_name)
            MEM_FREE_AND_NULL(gcpp_context.printer_name);
        gcpp_context.printer_name = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
        
    return ( (gcpp_context.printer_name) );
}

gcpp_error_t net_gcpp_set_printer_name( char *printer_name )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP printer_name var */
    ret = net_gcpp_set_var_printer_name(printer_name);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.printer_name)
            MEM_FREE_AND_NULL(gcpp_context.printer_name);
            
        gcpp_context.printer_name = gcpp_strdup(printer_name);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP printer_name var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_display_name( bool refresh )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        char temp[GCPP_MAX_NAME_LEN];
        
        /* retrieve display_name from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_display_name(&temp[0], sizeof(temp));
        
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.display_name)
            MEM_FREE_AND_NULL(gcpp_context.display_name);
        gcpp_context.display_name = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
        
    return ( (gcpp_context.display_name) );
}

gcpp_error_t net_gcpp_set_display_name( char *display_name )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP display_name var */
    ret = net_gcpp_set_var_display_name(display_name);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.display_name)
            MEM_FREE_AND_NULL(gcpp_context.display_name);
            
        gcpp_context.display_name = gcpp_strdup(display_name);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP display_name var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

bool net_gcpp_is_http_proxy_enabled( bool refresh )
{
    if (!gcpp_context.initialized)
        return false;
        
    if (refresh)
    {
        /* retrieve GCPP HTTP Proxy Enabled from storage */
        net_gcpp_get_var_http_proxy_enabled(&gcpp_context.http_proxy_enabled);
    }

    return ( gcpp_context.http_proxy_enabled );
}

gcpp_error_t net_gcpp_http_proxy_enable( bool enable )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if trying to enable and GCPP HTTP Proxy is already enabled, just return NET_GCPP_SUCCESS */
    if (enable && net_gcpp_is_http_proxy_enabled(false))
        return NET_GCPP_SUCCESS;
        
    /* if trying to disable and GCPP HTTP Proxy is already disbled, just return NET_GCPP_SUCCESS */
    if (!enable && !net_gcpp_is_http_proxy_enabled(false))
        return NET_GCPP_SUCCESS;

    /* attempt to set GCPP HTTP Proxy enable var */
    ret = net_gcpp_set_var_http_proxy_enabled(enable);
    if (ret == NET_GCPP_SUCCESS)
    {
        /* set GCPP HTTP Proxy enabled context */
        gcpp_context.http_proxy_enabled = enable;
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP Proxy Enabled config var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_http_proxy_server( bool refresh )
{
    char temp[GCPP_MAX_NAME_LEN];
    
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        /* retrieve GCPP HTTP proxy server from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_http_proxy_server(&temp[0], sizeof(temp));

        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_server)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_server);
        gcpp_context.http_proxy_server = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
    
    return ( gcpp_context.http_proxy_server );
}

gcpp_error_t net_gcpp_set_http_proxy_server( char *server )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP HTTP proxy server var */
    ret = net_gcpp_set_var_http_proxy_server(server);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_server)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_server);
            
        gcpp_context.http_proxy_server = gcpp_strdup(server);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP proxy server var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}


uint32_t net_gcpp_get_http_proxy_port( bool refresh )
{
    if (!gcpp_context.initialized)
        return 0;
        
    if (refresh)
    {
        /* retrieve GCPP HTTP proxy port from storage */
        net_gcpp_get_var_http_proxy_port(&gcpp_context.http_proxy_port);
    }

    return ( gcpp_context.http_proxy_port );
}

gcpp_error_t net_gcpp_set_http_proxy_port( uint32_t port )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP HTTP proxy port var */
    ret = net_gcpp_set_var_http_proxy_port(port);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        gcpp_context.http_proxy_port = port;
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP proxy port var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

bool net_gcpp_is_http_proxy_auth_required( bool refresh )
{
    if (!gcpp_context.initialized)
        return false;

    if (refresh)
    {
        /* retrieve proxy from storage */
        net_gcpp_get_var_http_proxy_auth(&gcpp_context.http_proxy_auth_req);
    }

    return ( gcpp_context.http_proxy_auth_req );
}

gcpp_error_t net_gcpp_http_proxy_auth_required( bool required )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if trying to require and GCPP HTTP Proxy authorization is already required, just return NET_GCPP_SUCCESS */
    if (required && net_gcpp_is_http_proxy_auth_required(false))
        return NET_GCPP_SUCCESS;
        
    /* if trying to not require  and GCPP HTTP Proxy authorization is already not required, just return NET_GCPP_SUCCESS */
    if (!required && !net_gcpp_is_http_proxy_auth_required(false))
        return NET_GCPP_SUCCESS;

    /* attempt to set GCPP HTTP Proxy authorization var */
    ret = net_gcpp_set_var_http_proxy_auth(required);
    if (ret == NET_GCPP_SUCCESS)
    {
        /* set GCPP HTTP Proxy authorization context */
        gcpp_context.http_proxy_auth_req = required;
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP Proxy authorization config var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_http_proxy_username( bool refresh )
{
    char temp[GCPP_MAX_URL_LENGTH];
    
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        /* retrieve proxy from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_http_proxy_username(&temp[0], sizeof(temp));

        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_username)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_username);
        gcpp_context.http_proxy_username = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
    
    return ( gcpp_context.http_proxy_username );
}

gcpp_error_t net_gcpp_set_http_proxy_username( char *username )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP HTTP proxy username var */
    ret = net_gcpp_set_var_http_proxy_username(username);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_username)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_username);
            
        gcpp_context.http_proxy_username = gcpp_strdup(username);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP proxy username var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_http_proxy_password( bool refresh )
{
    char temp[GCPP_MAX_URL_LENGTH];
    
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        /* retrieve proxy from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_http_proxy_password(&temp[0], sizeof(temp));

        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_password)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_password);
        gcpp_context.http_proxy_password = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
    
    return ( gcpp_context.http_proxy_password );
}

gcpp_error_t net_gcpp_set_http_proxy_password( char *password )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP HTTP proxy password var */
    ret = net_gcpp_set_var_http_proxy_password(password);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.http_proxy_password)
            MEM_FREE_AND_NULL(gcpp_context.http_proxy_password);
            
        gcpp_context.http_proxy_username = gcpp_strdup(password);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP HTTP proxy password var\n");
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_printer_id( bool forceUpdate )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (gcpp_context.printer_id == NULL && forceUpdate)
    {
        /* attempt to obtain printer ID */
        gcpp_get_printer_id(&gcpp_context, &gcpp_context.printer_id);
    }
        
    return ( (gcpp_context.printer_id) );
}

gcpp_error_t net_gcpp_set_printer_id( char *id )
{
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    if (gcpp_context.printer_id != NULL)
    {
        MEM_FREE_AND_NULL(gcpp_context.printer_id);
        gcpp_context.printer_id = NULL;
    }
    
    gcpp_context.printer_id = gcpp_strdup(id);
        
    return NET_GCPP_SUCCESS;
}

char *net_gcpp_get_xmpp_jid( bool refresh )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        char temp[GCPP_MAX_AUTH_CREDENTIALS_LEN];
        
        /* retrieve xmpp JID from storage */
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_xmpp_jid(&temp[0], sizeof(temp));

        GCPP_GLOBAL_LOCK();
        if (gcpp_context.xmpp_jid)
            MEM_FREE_AND_NULL(gcpp_context.xmpp_jid);
        gcpp_context.xmpp_jid = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
    
    return ( (gcpp_context.xmpp_jid) );
}

gcpp_error_t net_gcpp_set_xmpp_jid( char *jid )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP xmpp jid var */
    ret = net_gcpp_set_var_xmpp_jid(jid);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.xmpp_jid)
            MEM_FREE_AND_NULL(gcpp_context.xmpp_jid);
            
        gcpp_context.xmpp_jid = gcpp_strdup(jid);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP xmpp jid var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

char *net_gcpp_get_refresh_token( bool refresh )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (refresh)
    {
        char temp[GCPP_MAX_AUTH_CREDENTIALS_LEN];
        
        /* retrieve GCPP refresh token from storage*/
        memset(temp, 0, sizeof(temp));
        net_gcpp_get_var_refresh_token(&temp[0], sizeof(temp));
        
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.refresh_token)
            MEM_FREE_AND_NULL(gcpp_context.refresh_token);
        gcpp_context.refresh_token = gcpp_strdup(temp);
        GCPP_GLOBAL_UNLOCK();
    }
        
    return ( (gcpp_context.refresh_token) );
}

gcpp_error_t net_gcpp_set_refresh_token( char *refresh_token )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* attempt to set GCPP refresh token var */
    ret = net_gcpp_set_var_refresh_token(refresh_token);
    if (ret == NET_GCPP_SUCCESS)
    {
        GCPP_GLOBAL_LOCK();
        if (gcpp_context.refresh_token)
            MEM_FREE_AND_NULL(gcpp_context.refresh_token);
            
        gcpp_context.refresh_token = gcpp_strdup(refresh_token);
        GCPP_GLOBAL_UNLOCK();
    }
    else
    {
        DBG_ERR("error setting GCPP refresh token var\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

gcpp_run_state_t net_gcpp_get_run_state( void )
{
    if (!gcpp_context.initialized)
        return GCPP_UNCHANGED;
        
    return ( gcpp_run_state );
}

gcpp_job_state_t net_gcpp_get_job_state( void )
{
    if (!gcpp_context.initialized)
        return GCPP_JOBS_UNCHANGED;
        
    return ( gcpp_job_state );
}

char *net_gcpp_get_access_token( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    if (!gcpp_context.auth_tokens)
        return NULL;
        
    return ( gcpp_context.auth_tokens->access_token );
}

gcpp_oem_print_func_table_t *net_gcpp_get_oem_print_func_table( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    return &gcpp_oem_print_func_table;
}

gcpp_oem_print_config_t *net_gcpp_get_oem_print_config( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    return &gcpp_oem_print_config;
}

gcpp_error_t net_gcpp_printer_add( void )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("Adding GCP Printer...\n");
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* need GCPP to be registered */
    if (!net_gcpp_is_registered(false))
    {
        /* GCPP printer NOT registered */
        DBG_ERR("ERROR: GCP Proxy NOT registered.\n" );
        return NET_GCPP_NOT_REGISTERED;
    }

    /* check to see if printer is already added */
    if (net_gcpp_get_printer_id(true) != NULL)
    {
        /* GCPP printer already added */
        DBG_ERR("ERROR: GCP Printer already added!\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }
        
    /* need GCPP access token */
    if (net_gcpp_get_access_token() == NULL)
    {
        if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
        {
            DBG_ERR("ERROR: cannot obtain access_token!\n" );
            return NET_GCPP_INVALID_PARAMETER;
        }
    }
        
    /* attempt to add printer */
    ret = gcpp_add_printer(&gcpp_context, &gcpp_context.printer_id);
    if (ret == NET_GCPP_SUCCESS)
    {
        /* restart GCPP if currently running */
        if (gcpp_run_state == GCPP_STARTED)
        {
            /* restart GCPP */
            gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
        }
    }
    else
    {
            DBG_ERR("ERROR: Failed to add printer\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );

    return ret;
}

gcpp_error_t net_gcpp_printer_update( void )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;

    /* check to see if printer has been added */
    if (net_gcpp_get_printer_id(true) == NULL)
    {
        /* GCPP printer not yet added */
        DBG_ERR("ERROR: GCP Printer not added!\n" );
        return NET_GCPP_INVALID_PARAMETER;
    }

    DBG_MSG("Updating printer, ID = %s...\n", net_gcpp_get_printer_id(false));
        
    /* need GCPP access token */
    if (net_gcpp_get_access_token() == NULL)
    {
        if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
        {
            DBG_ERR("ERROR: cannot obtain access_token!\n" );
            return NET_GCPP_INVALID_PARAMETER;
        }
    }
        
    ret = gcpp_update_printer(&gcpp_context, net_gcpp_get_printer_id(false));
    if (ret != NET_GCPP_SUCCESS)
    {
        DBG_ERR("ERROR: Unable to update GCPP printer\n");
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_error_t net_gcpp_printer_remove( void )
{
    gcpp_error_t ret = NET_GCPP_SUCCESS;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* check if GCPP printer is NOT registered */
    if (!net_gcpp_is_registered(false))
    {
        /* GCPP printer NOT registered */
        DBG_ERR("ERROR: GCPP printer NOT registered.\n" );
        return NET_GCPP_NOT_REGISTERED;
    }

    DBG_MSG("Removing printer, ID = %s...\n", (net_gcpp_get_printer_id(true) == NULL) ? "All Printers" : net_gcpp_get_printer_id(false));
        
    /* need GCPP access token */
    if (net_gcpp_get_access_token() == NULL)
    {
        if (net_gcpp_issue_refresh_authorization_tokens_request(NULL, false) != NET_GCPP_SUCCESS)
        {
            DBG_ERR("ERROR: cannot obtain access_token!\n" );
            return NET_GCPP_INVALID_PARAMETER;
        }
    }
    
    /* remove the printers */
    if (net_gcpp_get_printer_id(false) == NULL)
    {
        /* remove all printers associated to proxy */
        gcpp_remove_printers(&gcpp_context);
    }
    else
    {
        /* remove printer id associated to proxy */
        gcpp_remove_printer(&gcpp_context, net_gcpp_get_printer_id(false));
    }

    /* restart GCPP if currently running */
    if (gcpp_run_state == GCPP_STARTED)
    {
        /* restart GCPP */
        gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
    }

    /* clear printer id */
    if (gcpp_context.printer_id)
    {
        GCPP_GLOBAL_LOCK();
        MEM_FREE_AND_NULL(gcpp_context.printer_id);
        gcpp_context.printer_id = NULL;
        GCPP_GLOBAL_UNLOCK();
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_error_t net_gcpp_printer_reset( void )
{
    gcpp_error_t ret = NET_GCPP_SUCCESS;
    
    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("Reset GCPP...\n");
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;

    /* disable GCP Proxy */
    net_gcpp_enable(false);

    /* unregister GCP Proxy */
    net_gcpp_registered(false);

    /* clear nvram values */
    net_gcpp_http_proxy_enable(false);
    net_gcpp_set_http_proxy_server("");
    net_gcpp_set_http_proxy_port(0);
    net_gcpp_http_proxy_auth_required(false);
    net_gcpp_set_http_proxy_username("");
    net_gcpp_set_http_proxy_password("");
    
    /* stop GCPP if currently running */
    if (gcpp_run_state == GCPP_STARTED)
    {
        /* stop GCPP */
        gcpp_update_states(GCPP_STOPPED, GCPP_JOBS_END, NET_GCPP_SUCCESS);
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_error_t net_gcpp_printer_get_job_ticket( char *job_ticket_url, gcpp_recv_job_header_t recv_job_header_func, char **job_ticket_data, uint32_t *job_ticket_size, void *context)
{
    return gcpp_printer_get_job_ticket(&gcpp_context, job_ticket_url, recv_job_header_func, job_ticket_data, job_ticket_size, context);
}

gcpp_error_t net_gcpp_printer_get_job(char *job_url, uint32_t skip_pages, char *mime_types, gcpp_recv_job_header_t recv_job_header_func, gcpp_recv_job_data_t recv_job_data_func, void *context)
{
    return gcpp_printer_get_job(&gcpp_context, job_url, skip_pages, mime_types, recv_job_header_func, recv_job_data_func, context);
}

gcpp_error_t net_gcpp_printer_get_jobs_list( gcpp_job_t ***jobs )
{

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* get job list from GCPP printer */
    gcpp_error_t rcode = gcpp_printer_get_jobs(&gcpp_context, net_gcpp_get_printer_id(true), jobs);
    if(rcode != NET_GCPP_SUCCESS)
    {
        DBG_ERR( "ERROR: Unable to get jobs list for printer!\n" );
    }
    
    return rcode;
}

gcpp_job_t *net_gcpp_job_new( void )
{
    return gcpp_printer_new_job();
}

gcpp_job_t *net_gcpp_job_copy( gcpp_job_t *job )
{
    return gcpp_printer_copy_job(job);
}

gcpp_error_t net_gcpp_job_delete( gcpp_job_t *job )
{
    gcpp_printer_delete_job(job);
    
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_job_delete_list( gcpp_job_t **jobs )
{
    gcpp_printer_delete_jobs(jobs);
    
    return NET_GCPP_SUCCESS;
}

gcpp_error_t net_gcpp_job_status_update( char *job_id, gcpp_job_status_t status, char *code, char *message )
{
    gcpp_error_t ret = NET_GCPP_FAIL;
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    switch (status)
    {
        case GCPP_JOB_STATUS_QUEUED:
            ret = gcpp_job_update_status(&gcpp_context, job_id, GCPP_JOB_STATUS_QUEUED_STR, code, message);
            break;
            
        case GCPP_JOB_STATUS_IN_PROGRESS:
            ret = gcpp_job_update_status(&gcpp_context, job_id, GCPP_JOB_STATUS_IN_PROGRESS_STR, code, message);
            break;
            
        case GCPP_JOB_STATUS_DONE:
            ret = gcpp_job_update_status(&gcpp_context, job_id, GCPP_JOB_STATUS_DONE_STR, code, message);
            break;
            
        case GCPP_JOB_STATUS_ERROR:
            ret = gcpp_job_update_status(&gcpp_context, job_id, GCPP_JOB_STATUS_ERROR_STR, code, message);
            break;
                
        default:
            ret = NET_GCPP_INVALID_PARAMETER;
            break;
    }
    
    return ret;
}

gcpp_error_t net_gcpp_issue_register_anonymous_request( void *callback_context, bool async )
{
    gcpp_error_t ret;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("Issuing register anonymous request...\n");
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if GCPP is already registered, then requires proper unregister first */
    if (net_gcpp_is_registered(false))
    {
        DBG_ERR("error - already registered\n");
        return NET_GCPP_ALREADY_REGISTERED;
    }

    if (async)
    {
        /* submit the anonymous registration request */
        ret = gcpp_async_submit("Anonymous Registration Request", &gcpp_register_anonymous_request, callback_context);
    }
    else
    {
        /* issue anonymous registration request */
        ret = gcpp_register_anonymous_request(&gcpp_context, callback_context);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_registration_anonymous_context_t *net_gcpp_get_anonymous_request_response( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    return ( gcpp_context.anonymous_registration );
}

gcpp_error_t net_gcpp_issue_claim_printer_request( char *polling_url, void *callback_context, bool async )
{
    gcpp_error_t ret;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("Issuing claim printer request...\n");
    
    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    if (polling_url == NULL)
    {
        DBG_ERR("error - Polling URL is NULL\n");
        return NET_GCPP_INVALID_PARAMETER;
    }
    
    if (net_gcpp_is_registered(false))
    {
        DBG_ERR("error - already registered\n");
        return NET_GCPP_ALREADY_REGISTERED;
    }
    
    gcpp_request_params_t *context = MEM_MALLOC(sizeof(gcpp_request_params_t));
    ASSERT(context);
    
    memset(context, 0, sizeof(gcpp_request_params_t));
    context->request_param = gcpp_strdup(polling_url);
    context->callback_context = callback_context;

    if (async)
    {
        /* submit the claim printer request */
        ret = gcpp_async_submit("Claim Printer Request", &gcpp_claim_printer_request, context);
    }
    else
    {
        /* issue claim printer request */
        ret = gcpp_claim_printer_request(&gcpp_context, context);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_registration_claim_printer_context_t *net_gcpp_get_claim_printer_request_response( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    return ( gcpp_context.claim_printer );
}


gcpp_error_t net_gcpp_issue_authorization_tokens_request( char *auth_code, void *callback_context, bool async )
{
    gcpp_error_t ret;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    DBG_MSG("Issuing authorization tokens request...\n");

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    if (auth_code == NULL)
    {
        DBG_ERR("error - authorization code is NULL\n");
        return NET_GCPP_INVALID_PARAMETER;
    }

    gcpp_request_params_t *context = MEM_MALLOC(sizeof(gcpp_request_params_t));
    ASSERT(context);
    
    memset(context, 0, sizeof(gcpp_request_params_t));
    context->request_param = gcpp_strdup(auth_code);
    context->callback_context = callback_context;
    
    if (async)
    {
        /* submit the authorization tokens request */
        ret = gcpp_async_submit("Authorization Tokens Request", &gcpp_authorization_tokens_request, context);
    }
    else
    {
        /* issue authorization tokens request */
        ret = gcpp_authorization_tokens_request(&gcpp_context, context);
    }
    
    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_error_t net_gcpp_issue_refresh_authorization_tokens_request( void *callback_context, bool async )
{
    gcpp_error_t ret;

    DBG_VERBOSE( "%s enter\n", __FUNCTION__ );

    if (!gcpp_context.initialized)
        return NET_GCPP_NOT_INITIALIZED;
        
    /* if GCPP is not already registered, then requires proper registeration first */
    if (!net_gcpp_is_registered(false))
    {
        DBG_ERR("ERROR: GCPP needs to be registered\n");
        return NET_GCPP_NOT_REGISTERED;
    }
    
        if (async)
    {
        /* submit the refresh authorization request */
        ret = gcpp_async_submit("Refresh Authorization Request", &gcpp_refresh_authorization_tokens, callback_context);
    }
    else
    {
        /* issue refresh authorization request */
        ret = gcpp_refresh_authorization_tokens(&gcpp_context, callback_context);
    }

    DBG_VERBOSE( "%s exit\n", __FUNCTION__ );
    
    return ret;
}

gcpp_authorization_tokens_context_t *net_gcpp_get_authorization_tokens_request_response( void )
{
    if (!gcpp_context.initialized)
        return NULL;
        
    return ( gcpp_context.auth_tokens );
}
