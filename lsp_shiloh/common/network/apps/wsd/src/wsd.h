/*
 * ============================================================================
 * Copyright (c) 2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef WSD_H
#define WSD_H

#include <signal.h>
#include "dc/dc_Dpws.h"	
#include "dc/dc_Constants.h"
#include "net_logger.h"
#include "net_io_api.h"
#include "net_wsd_config.h"
#include "posix_ostools.h"
#include "list.h"

/* temporary value */
#define DEFAULT_DELAY_SECS 5

/* message format */
#define NET_WSD_MQ_MSG_BUF_SIZE   2

/* message queue buffer */
#define NET_WSD_MQ_MSG_BUF_BYTES  ( NET_WSD_MQ_MSG_BUF_SIZE * sizeof( unsigned long ) )
 
// Flags for WSD Event flags group
#define WSD_FLAG_IP_CONFIGURED  0x01
#define WSD_FLAG_ENABLED        0x02

#define WSD_DEVICE_STATUS  1   // id used for timer callback function
#define WSD_JOB_STATUS     2   // id used for timer callback function

// Submodule ID for debug message logging
#ifndef LOGGER_MODULE_MASK
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_WSD 
#endif

#ifdef __linux__
#define DBG_ON /* DBG_MSG */
#define DBG_ERR_ON
#define DBG_VERBOSE_ON
#include "net_debug.h"
#else // ~ __linux__
// General debug trace commands - DBG_PRFX defined in individual file
#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)
#endif

// IMPORTANT: after modifying this table, must also modify g_netprint_msg_strs
typedef enum
{
    NET_WSD_PRINTER_STATUS_SUMMARY,   ///< data: (uint32_t) Status
    NET_WSD_PRINTER_ELEMENTS_CHANGE,  ///< data: (net_wsd_element_change_t) structure
    NET_WSD_PRINTER_STATUS_CONDITION, ///< data: (uint32_t) Status
    NET_WSD_JOB_STATUS,               ///< data: (uint32_t) job id - 0 indicates use saved status
    NET_WSD_JOB_END_STATE,            ///< data: (uint32_t) job id

    // add new msgs above this line
    NET_WSD_NUM_MSGS
} net_wsd_msg_type_t;

typedef struct
{
    net_wsd_msg_type_t type;
    void *data; 
} net_wsd_msg_t;

typedef union
{
    net_wsd_msg_t msg;
    uint8_t buf [ NET_WSD_MQ_MSG_BUF_BYTES ];
} net_wsd_msg_buf_t;

typedef enum 
{
    WSD_PRINT,
    WSD_NUM_PROTOCOLS,
} wsd_protocols_t;

// Defined in WSDPrintService.doc and WSDScanService.doc
// Order must match wsd_job_states array in wsd.c!
typedef enum
{
    WSD_JOB_STATE_ABORTED,
    WSD_JOB_STATE_CANCELED,
    WSD_JOB_STATE_COMPLETED,
    WSD_JOB_STATE_CREATING,
    WSD_JOB_STATE_PENDING,
    WSD_JOB_STATE_PENDING_HELD,
    WSD_JOB_STATE_PROCESSING,
    WSD_JOB_STATE_PROCESSING_STOPPED,
    WSD_JOB_STATE_STARTED,
    WSD_JOB_STATE_TERMINATING,

    NUM_WSD_JOB_STATES, // Must be last element
} wsd_job_states_t;

// Defined in WSDPrintService.doc and WSDScanService.doc
typedef enum
{                                                           // WS-Print
    WSD_JOB_STATE_REASON_COMPRESSION_ERROR,                 // WS-Print
    WSD_JOB_STATE_REASON_DOCUMENT_ACCESS_ERROR,             // WS-Print
    WSD_JOB_STATE_REASON_DOCUMENT_FORMAT_ERROR,             // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_DOCUMENT_TIMEOUT_ERROR,            // WS-Print
    WSD_JOB_STATE_REASON_DOCUMENT_TRANSFER_ERROR,           // WS-Print
    WSD_JOB_STATE_REASON_JOB_CANCELED_AT_DEVICE,            // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_JOB_CANCELED_BY_USER,              // WS-Print
    WSD_JOB_STATE_REASON_JOB_COMPLETED_SUCCESSFULLY,        // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_JOB_COMPLETED_WITH_ERRORS,         // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_JOB_COMPLETED_WITH_WARNINGS,       // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_JOB_INCOMING,                      // WS-Print
    WSD_JOB_STATE_REASON_JOB_PASSWORD_WAIT,                 // WS-Print
    WSD_JOB_STATE_REASON_JOB_PRINTING,                      // WS-Print
    WSD_JOB_STATE_REASON_NONE,                              // WS-Print, WS-Scan
    WSD_JOB_STATE_REASON_PRINTER_STOPPED,                   // WS-Print
    WSD_JOB_STATE_REASON_UNSUPPORTED_COMPRESSION,           // WS-Print
    WSD_JOB_STATE_REASON_UNSUPPORTED_DOCUMENT_FORMAT,       // WS-Print
    WSD_JOB_STATE_REASON_INVALID_SCAN_TICKET,               // WS-Scan
    WSD_JOB_STATE_REASON_IMAGE_TRANSFER_ERROR,              // WS-Scan
    WSD_JOB_STATE_REASON_JOB_SCANNING,                      // WS-Scan
    WSD_JOB_STATE_REASON_JOB_SCANNING_AND_TRANSFERRING,     // WS-Scan
    WSD_JOB_STATE_REASON_JOB_TIMEDOUT,                      // WS-Scan
    WSD_JOB_STATE_REASON_JOB_TRANSFERRING,                  // WS-Scan
    WSD_JOB_STATE_REASON_SCANNER_STOPPED,                   // WS-Scan 

    NUM_WSD_JOB_STATE_REASON_END, // Must be last element
} wsd_job_state_reasons_t;

typedef struct wsd_job_info_s
{
    bool                    active; 
    wsd_protocols_t         protocol;
    uint32_t                id;
    wsd_job_states_t        state;
    wsd_job_state_reasons_t reasons;
    char                    *name;
    char                    *user_name;
    uint32_t                koctets_processed;  // print only
    uint32_t                sheets_completed;   // for scan this is scans completed
    uint32_t                num_documents;
    char                    *doc_name;
    void                    *ticket;            // valid ticket
    void                    *requested_ticket;      
    net_io_session_hndl_t   io_session_hndl;
    net_io_job_hndl_t       job_hndl;
    ATLISTENTRY             job_data_list;
    uint32_t                job_data_bytes_buffered;
    bool                    io_ready_sent;
    bool                    job_data_recv_done;
    uint32_t                job_created_time;
    uint32_t                job_completed_time;
} wsd_job_info_t;

// Structure used by DPWS SDK 
// Device instance runtime data
typedef struct wsd_dpws_data_s
{	            
    struct dpws dpws;	            // runtime structure for event notification & client invocation
    short       device;             // Handler to look up info (UUID) for device
    short       event_source[ WSD_NUM_PROTOCOLS ];	// event source handle reference used for wsd event notification
    uint32_t    event_rate;         // Rate at which completed event states are sent
} wsd_dpws_data_t;

/**
 * @brief WSD Protocol startup function
 * 
 * This function is implemented at the WSD protocol layer and is
 * called by the overlying WSD layer to initialize the protocol
 */
typedef void ( *wsd_protocol_startup )( void );

/**
 * @brief WSD Protocol service class info for the DPWS SDK
 * 
 * This function registers a service class with the DPWS DK
 */
typedef int16_t ( *wsd_protocol_dpws_service_class )( int16_t );

/**
 * @brief WSD Protocol device type info for the DPWS SDK
 * 
 * This function registers a device type with the DPWS DK
 */
typedef void ( *wsd_protocol_dpws_device_type )( int16_t );

/**
 * @brief WSD Protocol shutdown function
 * 
 * This function is implemented at the WSD protocol layer and is
 * called by the overlying WSD layer to shutdown the protocol
 */
typedef void ( *wsd_protocol_shutdown )( void );

/**
 * @brief Mapping functions for gsoap functions into the various 
 *        protocol functions.  Used at registration time with
 *        gsoap
 */
typedef void *(*wsd_protocol_mime_read_open)(struct soap*, void*, const char*, const char*, const char*);
typedef size_t (*wsd_protocol_mime_read)(struct soap*, void*, char*, size_t);
typedef void (*wsd_protocol_mime_read_close)(struct soap*, void*);
typedef void *(*wsd_protocol_mime_write_open)(struct soap*, void*, const char*, const char*, const char*, enum soap_mime_encoding);
typedef int (*wsd_protocol_mime_write)(struct soap*, void*, const char*, size_t);
typedef void (*wsd_protocol_mime_write_close)(struct soap*, void*);
typedef void (*wsd_protocol_free_ticket)(void*);

/**
 * @brief WSD dispatch table
 *
 * This dispatch table must be populated by WSD protocols prior
 * to registering with the overlying WSD layer via
 * wsd_register_protocol(). 
 *
 */
typedef struct 
{
    wsd_protocol_startup            startup;            ///< Populate with the protocol initialization routine (required).
    wsd_protocol_dpws_service_class dpws_service_class; ///< Populate with the protocol registration for DPWS service class routine (required).
    wsd_protocol_dpws_device_type   dpws_device_type;   ///< Populate with the protocol registration for DPWS device type routine (required).
    wsd_protocol_shutdown           shutdown;           ///< Populate with the protocol shutdown routine (required).
    wsd_protocol_mime_read_open     mime_read_open;
    wsd_protocol_mime_read          mime_read;
    wsd_protocol_mime_read_close    mime_read_close;
    wsd_protocol_mime_write_open    mime_write_open;
    wsd_protocol_mime_write         mime_write;
    wsd_protocol_mime_write_close   mime_write_close;
    wsd_protocol_free_ticket        free_ticket;
} wsd_func_table_t;

typedef struct
{
    wsd_protocols_t  type;
    wsd_func_table_t func_table;
} wsd_protocol_instance_t;


// private datastore accessor functions

/**
 * @brief Get the meta data checksum
 * 
 * @param[out] checksum Pointer to storage allocated by caller
 *       used to pass back the variable value.
 * 
 * @retval WSD_VARS_OK The checksum has been filled with value from
 * from NVRam
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will return the checksum calculated from the meta data for 
 * by WSD.  This can be then used to determine if the meta data 
 * version should be updated.
 */
error_type_t wsd_get_var_meta_data_checksum( uint32_t *checksum );

/**
 * @brief Set the checksum to to the current WSD meta data
 * 
 * @param[in] checksum Newly calculated checksum for the WSD 
 *       meta data
 * 
 * @retval WSD_VARS_OK The checksum has been saved to NVRAM
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will set the checksum for the WSD meta data. 
 * Used to determine if the meta data verion should be updated 
 * on next boot 
 */
error_type_t wsd_set_var_meta_data_checksum( uint32_t checksum );

/**
 * @brief Get the current meta version for WSD
 * 
 * @param[out] meta_version Pointer to storage allocated by
 *       caller used to pass back the variable value.
 * 
 * @retval WSD_VARS_OK The variable has been filled with current
 *         value
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will return the current meta version which is used by
 * WSD. 
 */
error_type_t wsd_get_var_meta_version( uint32_t *meta_version );

/**
 * @brief Set the meta version for WSD
 * 
 * @param[in] meta_version new value to save to NVRAM
 * 
 * @retval WSD_VARS_OK The variable has been saved to NVRAM
 * 
 * @retval WSD_VARS_FAIL Failed to obtain lock on variable
 * 
 * This will set the meta version value used by WSD.  
 */
error_type_t wsd_set_var_meta_version( uint32_t meta_version );

/**
 * @brief Register a WSD Protocol with the overlying WSD layer.
 * 
 * @param protocol_type One of the protocol types 
 * 
 * @param[in] func_table Pointer to a link layer dispatch table that has been allocated and
 *       populated by the caller. Upon return, the caller may free the memory allocated for this
 *       table.
 * 
 */
void wsd_register_protocol( wsd_protocols_t protocol_type, wsd_func_table_t *func_table ); 
void wsd_lock_job_history( void );
void wsd_unlock_job_history( void );
wsd_job_info_t *wsd_get_job_history( void );
error_type_t wsd_new_job( wsd_protocols_t type, wsd_job_info_t **job_info );
net_wsd_config_t *wsd_get_config( void );
wsd_job_info_t *wsd_get_job_info( uint32_t job_id );
char *wsd_job_state_reason_str( wsd_job_state_reasons_t job_state_reason );
bool wsd_initializing(void);
bool wsd_ready(void);
bool wsd_busy(void);
void wsd_set_enabled(void);
void show_wsd_device(void);

//=========================================================================================================================
// Private function prototypes
//=========================================================================================================================

// Initialization/shutdown functions
void wsd_register_directed_probe( void );
void wsd_deregister_directed_probe( void );
void wsd_set_flags( void* intf, uint32_t ); // set general flags if intf==NULL
void wsd_clear_flags( void* intf, uint32_t flags ); // set general flags if intf==NULL

// Status/helper functions
error_type_t wsd_get_thread( pthread_t **thread_ptr, uint8_t **stack_ptr, char **thread_name );
wsd_dpws_data_t *wsd_get_dpws_data( void );
char *wsd_get_job_state_string( wsd_job_states_t state );

#ifdef HAVE_DBG_PRINTF
const char *wsd_msg_type_strs( net_wsd_msg_type_t index );
void wsd_print_job(const char* function, const wsd_job_info_t* job);
void wsd_print_history(void);
#endif // HAVE_DBG_PRINTF

#endif // WSD_H
