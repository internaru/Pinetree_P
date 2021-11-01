/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/**
 * @file gcpp_oem_config.h
 *
 * @brief OEM-specific GCPP print configuration options
 *  
 * The purpose of the OEM configuration files for this module is to provide
 * a mechanism for the user to modify configurable options without having 
 * to recompile the core module that depend on them. The goal is to isolate
 * OEM and core specific configuration values, and provide an abstraction layer
 * to simplify the porting
 */ 

#ifndef GCPP_OEM_CONFIG_H
#define GCPP_OEM_CONFIG_H

//Includes
#include "fifolist.h"
#include "sm_job_api.h"
#include "net_gcpp_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#define OEM_THREAD_STACK_SIZE   (MAX((32 * 1024), POSIX_MIN_STACK_SIZE))

//=========================================================================================================================
// Debug configuration
//=========================================================================================================================

#ifndef DBG_PRFX
 #define DBG_PRFX "GCPP OEM: "
 #define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT( 8 ) 

 // General debug trace commands
 #define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
 #define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
 #define DBG_MSG_NOPRFX(...) DBG_PRINTF(LOG_NOTICE, __VA_ARGS__)
 #define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
 #define DBG_DUMP(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)
 #define DBG_CMND(...) cmd_printf(__VA_ARGS__)
#endif


#define GCPP_REG_DATE_TIME_MAX_LEN  64
//=========================================================================================================================
// OEM-specific enumeration and types
//=========================================================================================================================
typedef enum
{
    COLOR_SPACE_GRAY,
    COLOR_SPACE_RGB,
    COLOR_SPACE_CMY,    
    COLOR_SPACE_CMYK,    
} gcpp_oem_color_space_t;

typedef enum
{
    LANDSCAPE_ANY,
    LANDSCAPE_PLUS90,
    LANDSCAPE_MINUS90,
} gcpp_oem_landscape_t;

typedef enum
{
    RASTERIZER_NONE,
    RASTERIZER_ACCEPT64K,
    RASTERIZER_TYPE42,
    RASTERIZER_TRUEIMAGE,
} gcpp_oem_rasterizer_t;

typedef enum
{
    RESOLUTION_300,
    RESOLUTION_600,
} gcpp_oem_resolution_t;


typedef struct 
{
    mediatype_t type;
    char        *name;
	char		*description;
	bool		manualFeed;
} gcpp_oem_mediatype_mapping_t;

typedef struct 
{
    mediatype_t type;
	char		*invocation;
} gcpp_oem_mediatype_invocation_t;

typedef struct 
{
    mediasize_t type;
    char        *name;
	char		*description;
	int			width;
	int			height;
	int			marginX;
	int			marginY;
} gcpp_oem_mediasize_mapping_t;

typedef struct 
{
    mediasize_t type;
	char		*invocation;
} gcpp_oem_mediasize_invocation_t;

typedef struct 
{
	gcpp_oem_resolution_t resolution;
	char	*name;
	char	*description;
    uint32_t width;
    uint32_t height;
} gcpp_oem_resolution_mapping_t;

typedef struct 
{
    gcpp_oem_resolution_t 	resolution;
	char				*invocation;
} gcpp_oem_resolution_invocation_t;

typedef struct 
{
    gcpp_oem_color_space_t 	color_space;
    char    *name;
	char	*description;
} gcpp_oem_color_space_mapping_t;

typedef struct 
{
    gcpp_oem_color_space_t 	color_space;
    char    *invocation;
} gcpp_oem_color_space_invocation_t;

typedef struct 
{
    gcpp_oem_landscape_t 	landscape;
    char    			*name;
} gcpp_oem_landscape_mapping_t;

typedef struct 
{
    gcpp_oem_rasterizer_t 	rasterizer;
    char    			*name;
} gcpp_oem_rasterizer_mapping_t;


/**
 * @brief GCPP OEM Job Entry structure
 * 
 * This structure is used to keep a current
 * list of jobs submitted to the SM interface.
 */
typedef struct _gcpp_oem_job_entry_t
{
	gcpp_job_t *job;
	uint32_t sm_id;
	smjob_state_t state;
	
	struct _gcpp_oem_job_entry_t* next;
} gcpp_oem_job_entry_t;

typedef enum _gcpp_client_event_t
{
 GCPP_EVENT_REGISTRATION_ANONYMOUS_COMPLETE=0, ///< Indicates completion of the GCP Anonymous Registration request
 GCPP_EVENT_REGISTRATION_SUCCESS,            ///< GCPP registration successful
 GCPP_EVENT_REGISTRATION_FAILED,             ///< GCPP registration failed 
 GCPP_EVENT_PRINTER_ADDED,                   ///< Already registered GCPP printer is added (if Google loses our printer ID !)
 GCPP_EVENT_PRINTER_UPDATED,                 ///< Already registered GCPP printer is updated
 GCPP_EVENT_PRINTER_REMOVED,                 ///< Notification that GCP Printer was removed/cancellation is complete
 GCPP_EVENT_NETWORK_CONNECTED,               ///< Network connected
 GCPP_EVENT_NETWORK_DISCONNECTED,            ///< Network disconnected 
 GCPP_EVENT_XMPP_CONNECTED,                  ///< XMPP connected 
 GCPP_EVENT_XMPP_DISCONNECTED,               ///< XMPP disconnected 
} gcpp_client_event_t;

typedef struct
{
    void     *buf;
    uint32_t len;
    uint32_t read;
} gcpp_print_job_data_t;

typedef struct
{
    gcpp_job_t              *job;
    
    uint32_t                job_id;
    smjob_ticket_t          job_ticket;
    
    uint32_t                doc_id;
    smjob_doc_ticket_t          doc_ticket;
    
    FIFOLIST                datafifo;
    gcpp_print_job_data_t   *data;
} gcpp_print_job_request_t;


typedef gcpp_error_t (*gcpp_client_notification_func_t)(gcpp_client_event_t event, void *event_context);

/**
 * @brief GCPP OEM Print Settings structure
 * 
 * This structure contains the print settings that apply
 * to the gcpp printer.
 */
typedef struct gcpp_oem_oem_print_settings
{
	pthread_mutex_t oem_lock_mutex;
	gcpp_oem_job_entry_t	*sm_active_job_list;

	char		*manufacturer;
	char		*serial_number;
	
	bool 		auto_print_registration_page;
	bool 		auto_claim_printer_for_registration_complete;
	bool 		xmpp_connect_pre_job_check;
	
	time_t 		token_expiration;
	time_t 		start_expiration;
	
	uint32_t	max_retries;

	
	char 		*printer_id;
	
	char        *invite_url;
	char 		*invite_page_url;
    char        *complete_invite_url;
	char 		*polling_url;
	char 		*registration_token;
    char        *user_email;

    char        supported_mime_types[128];

	bool		colors_supported;
	bool		collation_supported;
    bool        jog_offset_supported;
	bool 		duplexer_supported;
	bool 		stapler_supported;
	bool 		hole_punch_supported;
	
    uint32_t    min_copies;
    uint32_t    max_copies;
    uint32_t    min_priority;
    uint32_t    max_priority;
    uint32_t    ppm_mono;
    uint32_t    ppm_color;
	
	gcpp_oem_color_space_t	default_colorspace;
	gcpp_oem_landscape_t	default_landscape_orientation;
	gcpp_oem_rasterizer_t	default_rasterizer;
	gcpp_oem_resolution_t	default_resolution;
	mediatype_t			default_mediatype;
	mediasize_t 		default_mediasize;
    time_t              reg_token_duration;
    bool                cancel_req;
	gcpp_client_notification_func_t client_notify_func;
} gcpp_oem_print_settings_t;

/**
 * @brief GCPP OEM registration status enum
 * 
 * This enum contains the gcpp registration status if the printer
 */
typedef enum
{
    INVALID_STATE = -1,
    GCP_NOT_REGISTERED,
    GCP_REG_IN_PROGRESS,
    GCP_CLAIM_PENDING,
    GCP_REGISTERED,
}GCP_REG_STATUS;

/**************************************************/
/********* APIs exposed to EWS/UI clients *********/
/**************************************************/

/**
 *
 * \brief Client registers the event notification with the GCPP OEM module
 *
 * This routine should be called by the clients like EWS or UI 
 * which initiates operations with GCPP like - register GCP
 * suspend/resume GCP operation, un-register GCP. 
 * [sen: March-30] As of now the design allows only one notification function.
 *
 */
void gcpp_oem_register_client_notification_func( gcpp_client_notification_func_t );

/** 
 * \brief start anonymous registration
 * 
 **/
bool gcpp_oem_register(void);

/** 
 * \brief cancel the whole registration process
 * 
 **/
bool gcpp_oem_cancel_registration(void);

/** 
 * \brief un-register the existing GCP printer
 *  
 * this is a async call, hence the client will
 * receive the GCPP_EVENT_PRINTER_REMOVED event
 * when un-register operation is done.
 **/
bool gcpp_oem_unregister_printer(void);

/** 
 * \brief suspend the GCPP operation
 * 
 **/
bool gcpp_oem_suspend_operation(void);

/** 
 * \brief resume the GCP operation
 * 
 **/
bool gcpp_oem_resume_operation(void);

/** 
 * \brief suspend_resume the GCPP operation
 * 
 **/
bool gcpp_oem_suspend_resume_operation(void);
/** 
 * \brief GCP get operations for client to tap reg info
 * 
 **/
char* gcpp_oem_get_printer_id( void );
char* gcpp_oem_get_registration_token( void );
char* gcpp_oem_get_invite_page_url( void );
char* gcpp_oem_get_complete_invite_url( void );
char* gcpp_oem_get_invite_url( void );
unsigned long gcpp_oem_get_token_duration( void );
char* gcpp_oem_get_user_email( void );

bool gcpp_oem_get_network_status(void);

/** 
 * \brief to set the registration status enum
 * 
 **/
void gcpp_oem_set_gcp_reg_status(GCP_REG_STATUS status);

/** 
 * \brief to get the registration status enum
 * 
 **/
GCP_REG_STATUS gcpp_oem_get_gcp_reg_status(void);

/** 
 * \brief to get the registration status
 * 
 **/
bool gcpp_oem_is_registration_in_process( void );

/** 
 * \brief to initialize the GCP OEM NVRAM block
 * 
 **/
void gcpp_oem_var_init(void);

/** 
 * \brief to get the GCP registration date/time
 * 
 **/
gcpp_error_t gcpp_oem_get_var_reg_date_time(char *reg_date_time, uint32_t len);

/** 
 * \brief to set the GCP registration date/time
 * 
 **/
gcpp_error_t gcpp_oem_set_var_reg_date_time(const char *reg_date_time);

/** 
 * \brief to get the GCP print output settings (duplex/non-duplex)
 * 
 **/
gcpp_error_t gcpp_oem_get_var_print_output(uint8_t* print_output);

/** 
 * \brief to set the GCP print output settings (duplex/non-duplex)
 * 
 **/
gcpp_error_t gcpp_oem_set_var_print_output(uint8_t print_output);

/**
 * \brief is gcp registered
 * 
 **/
bool gcpp_oem_is_registered(bool val);

/** 
 * \brief return suspend resume status
 * 
 *  true if suspended false if resumed
 **/
bool gcpp_oem_get_suspend_resume_state(void);

void gcpp_oem_set_suspend_resume_state(bool state);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // GCPP_OEM_CONFIG_H
