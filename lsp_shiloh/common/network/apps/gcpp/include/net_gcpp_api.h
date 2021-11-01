/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file net_gcpp_api.h
 *
 * @brief Google Cloud Print Proxy API layer
 *
 * This API provides a method to initialize and start GCPP on a
 * system.  GCPP requires that the network be configured as well
 * as having GCPP enabled.
 */
#ifndef NET_GCPP_API_H
#define NET_GCPP_API_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// GCPP HTPP Defines
#define GCPP_PROXY_HEADER            "X-CloudPrint-Proxy: %s"
#define GCPP_ACCEPT_HEADER             "Accept: %s"
#define GCPP_ACCEPT_ENCODING_HEADER "Accept-Encoding: %s"
#define GCPP_ALL_CONTENT_ENCODINGS     "deflate, gzip"
#define GCPP_MIMETYPE_PDF                "application/pdf"
#define GCPP_MIMETYPE_IMAGE            "image/pwg-raster"
#define GCPP_MIMETYPE_JPEG              "image/jpeg"

// GCPP Max Lengths Allowed
#define GCPP_MAX_URL_LENGTH               512+1
#define GCPP_MAX_AUTH_CREDENTIALS_LEN     128+1
#define GCPP_MAX_NAME_LEN                  64+1
#define GCPP_MAX_PASSWORD_LEN              64+1
#define GCPP_MAX_XMPP_PORTS                10


// Function prototypes for the receive function for print job data
typedef uint32_t (*gcpp_recv_job_data_t)(char *buffer, uint32_t size, void *context);
typedef uint32_t (*gcpp_recv_job_header_t)(char *header, uint32_t size, void *context);

// GCPP job struct typedef
typedef struct 
{
    char                        *id;
    char                        *printer_id;
    char                        *title;
    char                        *user;
    char                        *ticket_url;
    char                        *file_url;
    uint32_t                    skip_pages;
    
    char                        **tags;
} gcpp_job_t;

/**
 * @name GCPP run states
 *
 */
typedef uint32_t gcpp_run_state_t;
#define GCPP_UNCHANGED                        0        ///< No change
#define GCPP_INIT                            1        ///< Initializing
#define GCPP_WAIT_FOR_CONFIGURED            2        ///< Waiting to be configured (GCPP enabled, registered, net configured)
#define GCPP_CONFIGURED                        3        ///< Configured (GCPP enabled, registered, net configured)
#define GCPP_STARTED                        4        ///< Started
#define GCPP_STOPPED                        5        ///< Stopped

/**
 * @name GCPP job states
 *
 */
typedef uint32_t gcpp_job_state_t;
#define GCPP_JOBS_UNCHANGED                    0        ///< No change
#define GCPP_JOBS_INIT                        1        ///< Initializing
#define GCPP_JOBS_IDLE                        2        ///< Waiting for print jobs
#define GCPP_JOBS_PENDING                    3        ///< Jobs are now pending in the queue
#define GCPP_JOBS_END                        4        ///< Done processing jobs

/**
 * @name GCPP job status
 *
 */
typedef uint32_t gcpp_job_status_t;
#define GCPP_JOB_STATUS_QUEUED               1        ///< Job just added and has not yet been downloaded
#define GCPP_JOB_STATUS_IN_PROGRESS            2        ///< Job downloaded and has been added to the client-side native printer queue
#define GCPP_JOB_STATUS_DONE                3        ///< Job printed successfully
#define GCPP_JOB_STATUS_ERROR                4        ///< Job cannot be printed due to an error

/**
 * @name GCPP return codes
 *
 * NOTE: error code values 0 through -10 are reserved in error_types.h
 */
typedef uint32_t gcpp_error_t;
#define NET_GCPP_SUCCESS                        OK        ///< Indicates succcessful or TRUE.
#define NET_GCPP_FAIL                            FAIL    ///< Indicates failure or FALSE.
#define NET_GCPP_NOT_INITIALIZED                -11     ///< The GCPP module has not been initialized. Operation will continue to fail until the GCPP module is initialized.
#define NET_GCPP_FATAL_ERROR                    -12     ///< An unrecoverable error occurred. Operation will likely continue to fail on repeated attempts.
#define NET_GCPP_RESOURCE_FAILURE               -13     ///< The operation failed because a required resource was unavailable.  A future attempt may succeed. 
#define NET_GCPP_LOW_MEM                        -14     ///< The operation failed because system memory is low. A future attempt may succeed. 
#define NET_GCPP_NOT_PERMITTED                  -15     ///< The operation is not permitted at this time.
#define NET_GCPP_INVALID_PARAMETER              -16     ///< The operation is was called with an invalid parameter.
#define NET_GCPP_ALREADY_REGISTERED                -17     ///< The printer is already registered. Unregister printer and try again.
#define NET_GCPP_NOT_REGISTERED                    -18     ///< The printer is yet registered. Register printer and try again.
#define NET_GCPP_NETWORK_DISCONNECTED            -20     ///< Network was disconnected.
#define NET_GCPP_NETWORK_CHANGED                -21     ///< Network state changed.
#define NET_GCPP_HTTP_BAD_REQUEST                -40     ///< Forbidden (HTTP response code 403).
#define NET_GCPP_HTTP_FORBIDDEN                    -41     ///< Forbidden (HTTP response code 403).
#define NET_GCPP_HTTP_FILE_NOT_FOUND            -42     ///< File not found on server (HTTP response code 404).
#define NET_GCPP_HTTP_FILE_CONVERSION_FAILED    -43     ///< File conversion failed (HTTP response code 502, 415).
#define NET_GCPP_HTTP_UNSUPPORTED_RESPONSE_CODE    -59     ///< Unsupported HTTP response code error.
#define NET_GCPP_XMPP_CONNECTION_FAILED            -60     ///< XMPP conenction failure.
#define NET_GCPP_PROCESS_PRINT_JOBS_FAILED        -80     ///< Failed to process print GCP jobs.

/**
 * @name GCPP event codes
 *
 */
typedef uint32_t gcpp_event_t;
#define NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE      1    ///< Indicates completion of the GCP Anonymous Registration request
#define NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE  2    ///< Indicates completion of the GCP Registration Claim Printer request
#define NET_GCPP_EVENT_REGISTERED                           3    ///< GCPP has been registered
#define NET_GCPP_EVENT_UNREGISTERED                         4    ///< GCPP has been unregistered
#define NET_GCPP_EVENT_NETWORK_CONNECTED                    5    ///< Network had been connected
#define NET_GCPP_EVENT_NETWORK_DISCONNECTED                 6    ///< Network had been disconnected
#define NET_GCPP_EVENT_XMPP_CONNECTED                       7    ///< XMPP has been connected
#define NET_GCPP_EVENT_XMPP_DISCONNECTED                    8    ///< XMPP has been disconnected
#define NET_GCPP_EVENT_XMPP_IDLE                            9    ///< XMPP idle state
#define NET_GCPP_EVENT_AUTH_TOKENS                          10   ///< Notification of new Authorization tokens
#define NET_GCPP_EVENT_GET_PRINTER_ID                       11   ///< Notification of current GCP Printer ID
#define NET_GCPP_EVENT_PRINTER_ADDED                        12   ///< Notification that GCP Printer was added
#define NET_GCPP_EVENT_PRINTER_UPDATED                      13   ///< Notification that the GCP Printer was updated
#define NET_GCPP_EVENT_PRINTER_REMOVED                      14   ///< Notification that GCP Printer was removed
#define NET_GCPP_EVENT_PRINTER_STATUS_UPDATED               15   ///< Notification of GCP Printer Status updates
#define NET_GCPP_EVENT_PRINTER_STATE_CHANGED                16   ///< Notification of GCP Printer changes in run and job states
#define NET_GCPP_EVENT_XSRF_TOKEN_UPDATED                   17   ///< Notification that the XSRF Token was updated
#define NET_GCPP_EVENT_PRINTER_JOBS                         18   ///< Notification of Jobs for GCP Printer

// standard event context header
typedef struct
{
    gcpp_error_t result;            // GCPP result/error
    long         response_code;     // HTTP response code
    
    bool         success;           // Google Cloudprint success/failure
    long         error_code;        // Google Cloudprint error code
} gcpp_event_context_header_t;

// NET_GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE context
typedef struct
{
    gcpp_event_context_header_t hdr;
    
    char *registration_token;
    char *token_duration;
    char *invite_url;
    char *complete_invite_url;
    char *invite_page_url;
    char *polling_url;
    char *printer_id;    
} gcpp_registration_anonymous_context_t;

// NET_GCPP_EVENT_REGISTRATION_CLAIM_PRINTER_COMPLETE context
typedef struct
{
    gcpp_event_context_header_t hdr;
    
    char *xmpp_jid;
    char *authorization_code;
    char *user_email;
    char *confirmation_page_url;
    char *printerid;
} gcpp_registration_claim_printer_context_t;

// NET_GCPP_EVENT_AUTH_TOKENS context
typedef struct
{
    gcpp_event_context_header_t hdr;
    
    char *access_token;
    char *refresh_token;
    uint32_t expires_in;
} gcpp_authorization_tokens_context_t;

// NET_GCPP_EVENT_PRINTER_STATUS_UPDATED context
typedef struct
{
    gcpp_event_context_header_t hdr;
    
    char *job_id;
    char *status;
} gcpp_printer_status_context_t;

// NET_GCPP_EVENT_PRINTER_STATE_CHANGED context
typedef struct
{
    gcpp_event_context_header_t hdr;
    
    gcpp_run_state_t run_state;
    gcpp_job_state_t job_state;
} gcpp_state_context_t;


/*************************************/
/********* Synchronous calls *********/
/*************************************/

/**
 *
 * \brief Initializes the Google Cloud Print Proxy code
 *
 * This routine should be called exactly once following a system
 * startup.  It starts the initialization of the Google Cloud Print proxy.
 *
 */
void net_gcpp_init( void );

/** 
 *
 * \brief Determines if Google Cloud Print Proxy has been initialized
 *
 * \return bool
 * \retval TRUE if initialized, FALSE if not
 *
 */
bool net_gcpp_is_initialized( void );

/** 
 *
 * \brief Determines if Google Cloud Print Proxy is enabled
 *
 * \return bool
 * \retval TRUE if enabled, FALSE if disabled
 *
 */
bool net_gcpp_is_enabled( bool refresh );

/** 
 *
 * \brief Set Google Cloud Print Proxy as enabled(true) or disabled(false)
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_enable( bool enable );

/**
 *
 * \brief Determines if Google Cloud Print Proxy has been registered.
 *
 * \return bool
 * \retval TRUE if registered, FALSE if not registered
 *
 */
bool net_gcpp_is_registered( bool refresh );

/**
 *
 * \brief Determines if Google Cloud Print Proxy registration is in process.
 *
 * \return bool
 * \retval TRUE if registration in process, FALSE if not
 *
 */
bool net_gcpp_is_registration_in_process( void );

/** 
 *
 * \brief Set Google Cloud Print Proxy as registered(true) or unregistered(false)
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_registered( bool registered );

/**
 *
 * \brief Determines if Google Cloud Print Proxy Network status is connected or disconnected.
 *
 * \return bool
 * \retval TRUE if network connected, FALSE if not
 *
 */
bool net_gcpp_is_network_connected( void );

/**
 *
 * \brief Determines if Google Cloud Print Proxy XMPP status is connected or disconnected.
 *
 * \return bool
 * \retval TRUE if xmpp connected, FALSE if not
 *
 */
bool net_gcpp_is_xmpp_connected( void );

/**
 *
 * \brief Returns pointer to GCPP printer proxy
 *
 * \return char *
 * \retval pointer to GCPP printer proxy
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_proxy( bool refresh );

/** 
 *
 * \brief Set GCPP printer proxy
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_proxy( char *proxy );

/**
 *
 * \brief Returns pointer to GCPP printer name
 *
 * \return char *
 * \retval pointer to GCPP printer name
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_printer_name( bool refresh );

/** 
 *
 * \brief Set GCPP printer name
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_printer_name( char *name );

/**
 *
 * \brief Returns pointer to GCPP display name
 *
 * \return char *
 * \retval pointer to GCPP display name
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_display_name( bool refresh );

/** 
 *
 * \brief Set GCPP display name
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_display_name( char *name );

/**
 *
 * \brief Returns pointer to GCPP printer id
 *
 * \return char *
 * \retval pointer to GCPP printer id
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_printer_id( bool forceUpdate );

/**
 *
 * \brief Set GCPP printer id
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_printer_id( char *id );

/**
 *
 * \brief Returns pointer to current GCPP xmpp JID
 *
 * \return char *
 * \retval pointer to GCPP xmpp JID
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_xmpp_jid( bool refresh );

/** 
 *
 * \brief Set Google Cloud Print Proxy XMPP JID and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_xmpp_jid( char *jid );

/**
 *
 * \brief Returns pointer to GCPP refresh token
 *
 * \return char *
 * \retval pointer to GCPP refresh token
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_refresh_token( bool refresh );

/** 
 *
 * \brief Set Google Cloud Print Proxy refresh token and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_refresh_token( char *refesh_token );

/**
 *
 * \brief Determines if GCPP HTTP Proxy is enabled
 *
 * \return bool
 * \retval TRUE if enabled, FALSE if disabled
 *
 */
bool net_gcpp_is_http_proxy_enabled( bool refresh );

/**
 *
 * \brief Set GCPP HTTP Proxy as enabled(true) or disabled(false)
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_http_proxy_enable( bool enable );

/**
 *
 * \brief Returns pointer to GCPP HTTP proxy server
 *
 * \return char *
 * \retval pointer to GCPP HTPP proxy server
 * \retval NULL if not set or upon failure 
 *
 */
char *net_gcpp_get_http_proxy_server( bool refresh );

/**
 *
 * \brief Set GCPP HTTP proxy server and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_http_proxy_server( char *server );

/**
 *
 * \brief Returns pointer to GCPP HTTP proxy port
 *
 * \return uint32_t
 * \retval GCPP HTPP proxy port
 * \retval 0 if not set or upon failure 
 *
 */
uint32_t net_gcpp_get_http_proxy_port( bool refresh );

/**
 *
 * \brief Set GCPP HTTP proxy port and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_http_proxy_port( uint32_t port );

/**
 *
 * \brief Determines if GCPP HTTP Proxy authorization is required
 *
 * \return bool
 * \retval TRUE if authorization is required, FALSE if not
 *
 */
bool net_gcpp_is_http_proxy_auth_required( bool refresh );

/**
 *
 * \brief Set GCPP HTTP Proxy authorization requirement as enabled(true) 
 *        or disabled(false) and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_http_proxy_auth_required( bool required );

/**
 *
 * \brief Returns pointer to GCPP HTTP proxy username
 *
 * \return char *
 * \retval pointer to GCPP HTPP proxy username
 * \retval NULL if not set or upon failure 
 *
 */
char *net_gcpp_get_http_proxy_username( bool refresh );

/**
 *
 * \brief Set GCPP HTTP proxy username and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_http_proxy_username( char *username );

/**
 *
 * \brief Returns pointer to GCPP HTTP proxy password
 *
 * \return char *
 * \retval pointer to GCPP HTPP proxy password
 * \retval NULL if not set or upon failure 
 *
 */
char *net_gcpp_get_http_proxy_password( bool refresh );

/**
 *
 * \brief Set GCPP HTTP proxy password and
 *        and store value to NVRAM.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_set_http_proxy_password( char *password );

/**
 *
 * \brief Returns current GCPP run state
 *
 * \return gcpp_run_state_t
 *
 */
gcpp_run_state_t net_gcpp_get_run_state( void );

/**
 *
 * \brief Returns current GCPP job state
 *
 * \return gcpp_job_state_t
 *
 */
gcpp_job_state_t net_gcpp_get_job_state( void );

/**
 *
 * \brief Returns pointer to GCPP access token
 *
 * \return char *
 * \retval pointer to GCPP access token
 * \retval NULL upon failure 
 *
 */
char *net_gcpp_get_access_token( void );

/** 
 *
 * \brief Add Google Cloud Print Proxy printer
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_add( void );

/**
 *
 * \brief Remove Google Cloud Print Proxy printer
 *
 * NOTE: This must be initiated by user interaction only, never automatically.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_remove( void );

/**
 *
 * \brief Update GCPP printer, using OEM config parameters 
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_update( void );

/** 
 *
 * \brief Restart the Google Cloud Print Proxy printer.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_restart( void );

/**
 *
 * \brief Reset Google Cloud Print Proxy printer and reset NVRAM registration variables
 *
 * NOTE: This must be initiated by user interaction only, never automatically.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_reset( void );

/**
 *
 * \brief Get GCPP printer job ticket data
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_get_job_ticket( char *job_ticket_url, gcpp_recv_job_header_t recv_job_header_func, char **job_ticket_data, uint32_t *job_ticket_size, void *context);

/**
 *
 * \brief Get GCPP printer job
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_get_job(char *job_url, uint32_t skip_pages, char *mime_types, gcpp_recv_job_header_t recv_job_header_func, gcpp_recv_job_data_t recv_job_data_func, void *context);

/**
 *
 * \brief Get list of currently queued GCPP jobs for Google Cloud Print Proxy printer
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_printer_get_jobs_list( gcpp_job_t ***jobs );

/**
 *
 * \brief Deletes provided GCPP Job list
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_job_delete_list( gcpp_job_t **jobs );

/**
 *
 * \brief Create a new GCPP job structure
 *
 *
 * \return gcpp_job_t *
 * \retval pointer to new GCPP job structure
 * \retval NULL upon failure 
 *
 */
gcpp_job_t *net_gcpp_job_new( void );

/**
 *
 * \brief Copy an existing GCPP job structure
 *
 *
 * \return gcpp_job_t *
 * \retval pointer to new copy of GCPP job structure
 * \retval NULL upon failure 
 *
 */
gcpp_job_t *net_gcpp_job_copy( gcpp_job_t *job );

/**
 *
 * \brief Deletes an existing GCPP job structure
 *
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_job_delete( gcpp_job_t *job );

/**
 *
 * \brief Updates GCP Job Status
 *
 * NOTE: If status is ERROR, the code and message parameters 
 *       can be used for displaying helpful information to the 
 *       user via the user interface.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_job_status_update( char *job_id, gcpp_job_status_t status, char *code, char *message );


/**
 *
 * \brief Issues request to perform anonymous Google Cloud Print registration 
 * for the printer
 *
 * NOTE: This must be initiated by user interaction only, never automatically.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_issue_register_anonymous_request( void *callback_context, bool async );

/**
 *
 * \brief Returns pointer to GCPP gcpp_registration_anonymous_context_t structure
 *
 * \return gcpp_registration_anonymous_context_t *
 * \retval pointer to GCPP gcpp_registration_anonymous_context_t structure
 * \retval NULL upon failure 
 *
 */
gcpp_registration_anonymous_context_t *net_gcpp_get_anonymous_request_response( void );


/**
 *
 * \brief Issues request to determine if the user has claimed the Google Cloud Printer
 *
 * NOTE: This can be initiated by user interaction or automatically.  If automatically
 *       then it should not be issued more than once every 30 seconds in back-to-back 
 *       requests.
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_issue_claim_printer_request( char *polling_url, void *callback_context, bool async );

/**
 *
 * \brief Returns pointer to GCPP gcpp_registration_claim_printer_context_t structure
 *
 * \return gcpp_registration_claim_printer_context_t *
 * \retval pointer to GCPP gcpp_registration_claim_printer_context_t structure
 * \retval NULL upon failure 
 *
 */
gcpp_registration_claim_printer_context_t *net_gcpp_get_claim_printer_request_response( void );


/**
 *
 * \brief Issues asynchronous request to obtain Google Cloud Print Proxy authorization tokens
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_issue_authorization_tokens_request( char *auth_code, void *callback_context, bool async );

/**
 *
 * \brief Issues asynchronous request to refresh Google Cloud Print Proxy authorization tokens
 *
 * \return gcpp_error_t            refer to enum "gcpp_error_t"
 *
 */
gcpp_error_t net_gcpp_issue_refresh_authorization_tokens_request( void *callback_context, bool async );

/**
 *
 * \brief Returns pointer to GCPP gcpp_authorization_tokens_context_t structure
 *
 * \return gcpp_authorization_tokens_context_t *
 * \retval pointer to GCPP gcpp_authorization_tokens_context_t structure
 * \retval NULL upon failure 
 *
 */
gcpp_authorization_tokens_context_t *net_gcpp_get_authorization_tokens_request_response( void );



/**************************************************/
/********* OEM-specific abstraction layer *********/
/**************************************************/

/* 
 *  OEM-specific function prototypes
 */
typedef gcpp_error_t (*gcpp_oem_print_init_func_t)(void);
typedef gcpp_error_t (*gcpp_oem_print_setup_func_t)(void);
typedef gcpp_error_t (*gcpp_oem_print_start_func_t)(void);
typedef gcpp_error_t (*gcpp_oem_print_stop_func_t)(void);
typedef gcpp_error_t (*gcpp_oem_print_cleanup_func_t)(void);

typedef gcpp_error_t (*gcpp_oem_print_event_notification_func_t)(gcpp_event_t event, void *event_context, void *callback_context);
typedef gcpp_error_t (*gcpp_oem_print_event_data_func_t)(gcpp_event_t event, char *data, size_t size, void *callback_context);

typedef char* (*gcpp_oem_print_get_printer_capabilities_func_t)(void);
typedef char* (*gcpp_oem_print_get_printer_defaults_func_t)(void);
typedef char* (*gcpp_oem_print_get_printer_tags_func_t)(void);
typedef char* (*gcpp_oem_print_get_printer_status_func_t)(void);
typedef char* (*gcpp_oem_print_get_printer_description_func_t)(void);
typedef char* (*gcpp_oem_print_get_printer_capshash_func_t)(void);

/*
 * GCPP print function table                                              
 *                                                                                         
 * Contains the OEM-specific print functions for this module.
 * 
 * IMPORTANT: To maintiain compatibility with pre-existing binaries, any new function      
 * pointers must be added to the *end* of this table. Existing function pointers may be    
 * depricated but should not be removed.                                                    
 */
typedef struct 
{
    gcpp_oem_print_init_func_t                      init;
    gcpp_oem_print_setup_func_t                     setup;
    gcpp_oem_print_start_func_t                     start;
    gcpp_oem_print_stop_func_t                      stop;
    gcpp_oem_print_cleanup_func_t                   cleanup;

    gcpp_oem_print_event_notification_func_t        event_notification;
    
    gcpp_oem_print_get_printer_capabilities_func_t  get_printer_capabilities;
    gcpp_oem_print_get_printer_defaults_func_t      get_printer_defaults;
    gcpp_oem_print_get_printer_tags_func_t          get_printer_tags;
    gcpp_oem_print_get_printer_status_func_t        get_printer_status;
    gcpp_oem_print_get_printer_description_func_t   get_printer_description;
    gcpp_oem_print_get_printer_capshash_func_t      get_printer_capshash;

    gcpp_oem_print_event_data_func_t                event_data;
    
    // add new functions here
} gcpp_oem_print_func_table_t;

/*
 * GCPP Print OEM configuration structure                                          
 *                                                                                         
 * Contains the OEM-specific configuration functions and values for this module.
 * 
 * IMPORTANT:  To maintiain compatibility with pre-existing binaries, any new values must 
 * be added to the *end* of this structure. Existing values may be depricated but should 
 * not be removed.                                                                        
 */
typedef struct
{
    char        *cloudprint_url;
    char         *oauth2_url;
    char        *xmpp_domain;
    uint32_t     xmpp_port;
    uint32_t     xmpp_alternate_ports[GCPP_MAX_XMPP_PORTS];
    uint8_t     xmpp_num_alt_ports;
    
    char        *oauth_client_id;
    char        *oauth_client_secret;
    
    uint32_t    xmpp_connect_timeout_ms;
    uint32_t    xmpp_event_timeout_ms;
    uint32_t    xmpp_keep_alive_delay_ms;
    uint32_t    polling_delay_ms;
    uint32_t    start_timeout_ms;
    
    // add new values here
} gcpp_oem_print_config_t;


/**
 *
 * \brief Returns pointer to OEM-specific GCPP print configuration fucntion table
 *
 * \return gcpp_oem_print_func_table_t *
 * \retval pointer to OEM-specific GCPP print configuration fucntion table
 * \retval NULL upon failure 
 *
 */
gcpp_oem_print_func_table_t *net_gcpp_get_oem_print_func_table( void );

/**
 *
 * \brief Returns pointer to OEM-specific GCPP print configuration values
 *
 * \return gcpp_oem_print_config_t *
 * \retval pointer to OEM-specific GCPP print configuration values
 * \retval NULL upon failure 
 *
 */
gcpp_oem_print_config_t *net_gcpp_get_oem_print_config( void );


/* 
 * Populate the OEM-specific configuration functions and values. 
 * 
 * NOTE: this routine must be implemented for every OEM, even if no OEM-specific
 * configuration functions or values are provided by the corresponding module.
 */
extern gcpp_error_t gcpp_oem_print_populate_func_table(gcpp_oem_print_func_table_t* func_table_ptr);
extern gcpp_error_t gcpp_oem_print_populate_config(gcpp_oem_print_config_t* config_ptr);

#ifdef __cplusplus
}
#endif

#endif // NET_GCPP_API_H

