/****************************************************************************** 
 * Copyright (c) 2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <string.h>
#include <stdio.h>
#include "posix_ostools.h"
#include "pthread.h"
#include "memAPI.h"
#include "platform.h"
#include "platform_api.h"
#include "debug.h"
#include "logger.h"
#include "error_types.h"
#include "http_api.h"
#include "net_logger.h"
#include "map_api.h"
#include "oid_sm_api.h"

// module local includes
#include "ipp_api.h"
#include "ipp_const.h"
#include "ipp.h"
#include "ipp_cntxt.h"
#include "ipp_printer_requested_attr.h"
#include "ipp_job_requested_attr.h"
#include "ipp_request.h"
#include "oid_api.h"

//=======================================================================================
// Debug configuration
//=======================================================================================

#define DBG_PRFX "IPP: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_IPP 

#define DBG_ENTER(fmt,...)  dbg_printf(DBG_PRFX "==>%s "fmt"\n", __func__ ,##__VA_ARGS__ )
#define DBG_RETURN(fmt,...)   dbg_printf(DBG_PRFX "<==%s "fmt"\n", __func__ ,##__VA_ARGS__ )

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)

#ifndef DBG_ENTER
    #define DBG_ENTER(...)
#endif
#ifndef DBG_RETURN
    #define DBG_RETURN(...)
#endif


//=======================================================================================
// Data types
//=======================================================================================
#define MS_PER_TICK ( 1000 / SYS_TICK_FREQ )

// thread stack size
#define IPP_STACK_SIZE      POSIX_MIN_STACK_SIZE

// IPP instance context signature
#define IPP_INST_CTXT_SIGNATURE     0x49505069 // ascii "IPPi"
// IPP request context signature
#define IPP_REQ_CTXT_SIGNATURE      0x49505072 // ascii "IPPr"

#ifdef ENABLE_SINDOH_MOBILE_APP
extern bool isSprintBusy;
extern pthread_mutex_t _ipp_mutex;
#endif
/*
 *  Message queue
 */

// IMPORTANT: after modifying this table, must also modify g_ipp_msg_strs
typedef enum
{
    IPP_MSG_PROCESS_HTTP_HEADER,
    IPP_MSG_PROCESS_REQUEST,
    IPP_MSG_SEND_RESPONSE,
    IPP_MSG_CLEANUP,

    // add new msgs above this line
    IPP_NUM_MSGS
} ipp_msg_type_t;

static const char *g_ipp_msg_strs[IPP_NUM_MSGS] = 
{
    "IPP_MSG_PROCESS_HTTP_HEADER",
    "IPP_MSG_PROCESS_REQUEST",
    "IPP_MSG_SEND_RESPONSE",
    "IPP_MSG_CLEANUP",
};

static const char *g_ipp_state_strs[IPP_NUM_STATES] = 
{
    "IPP_STATE_PROCESS_HTTP_HEADER",
    "IPP_STATE_PROCESS_REQUEST",
    "IPP_STATE_SEND_RESPONSE",
    "IPP_STATE_CLEANUP",
};


#define IPP_MQ_NUM_MSG_BUFS   20

/* message format */
#define IPP_MQ_MSG_BUF_SIZE   2 // TX_2_ULONG // buff byte size must be >= sizeof(ipp_msg_t)
typedef struct
{
    ipp_msg_type_t type;
    void *data; // message-specific opaque data
} ipp_msg_t;

/* message queue buffer */
#define IPP_MQ_MSG_BUF_BYTES  (IPP_MQ_MSG_BUF_SIZE*sizeof(uint32_t))
typedef union
{
    ipp_msg_t msg;
    uint8_t buf[IPP_MQ_MSG_BUF_BYTES]; // Forces ipp_msg_buf_t size into valid 
                                       // threadx queue size option.
} ipp_msg_buf_t;

static pthread_t          ipp_thread;
static ALIGN(8) uint8_t   ipp_stack[IPP_STACK_SIZE];

static mqd_t                ipp_msg_queue; // msg queue control block

bool g_ipp_initialized = false;

// internal translation functions
// returns binary data ready for insertion into IPP stream, e.g. cannot assume strings will be NULL-terminated.
static int32_t ipp_features_supported_func(ipp_req_hndl_t request_hndl, oid_t oid, uint32_t index, char *buf, uint32_t *len);

static ipp_attribute_t g_ipp_grp_names[] =
{
    {"job-template", OID_INVALID, IPP_TAG_CHARSET, NULL, IPP_GRP_JOB_TEMPLATE},
    {"printer-description", OID_INVALID, IPP_TAG_CHARSET, NULL, IPP_GRP_DESCRIPTION},
    {"all", OID_INVALID, IPP_TAG_CHARSET, NULL, IPP_GRP_MASK},
};
static uint32_t g_ipp_grp_name_cnt = sizeof(g_ipp_grp_names)/sizeof(ipp_attribute_t);

static ipp_attribute_t g_ipp_printer_attrs[] =
{
    {"charset-configured", OID_SM_CHARSET_CONFIGURED, IPP_TAG_CHARSET, NULL, IPP_GRP_DESCRIPTION},
    {"ipp-features-supported", OID_INVALID, IPP_TAG_KEYWORD, ipp_features_supported_func, IPP_GRP_DESCRIPTION},
};
static uint32_t g_ipp_printer_attr_cnt = sizeof(g_ipp_printer_attrs)/sizeof(ipp_attribute_t);


//=======================================================================================
// Local function prototypes
//=======================================================================================

// Thread Entry Point and State Loop
static void *ipp_state_loop(void *entry_input);

// Helper Routines
static ipp_rcode_t ipp_send_msg_wait_opt(ipp_msg_type_t type, void *data,  uint32_t posix_wait_opt);
static ipp_request_t *ipp_create_request(ipp_instance_t *ipp_instance, http_request_t http_hndl);
static void ipp_free_request(ipp_request_t *ipp_req);

// Callback Routines
static void ipp_http_event( http_request_t http_hndl, http_resource_event_flag_t event,
                            void *usr_ctxt );



//=======================================================================================
// Parameterized Macros
//=======================================================================================

#define IPP_LOCK_CONTEXT(ipp_mutex)                              \
        do                                                       \
        {                                                        \
            int32_t posix_rcode;                                  \
            posix_rcode = pthread_mutex_lock( &ipp_mutex ); \
            XASSERT(posix_rcode == 0, posix_rcode); \
        } while(0)

#define IPP_UNLOCK_CONTEXT(ipp_mutex)                            \
        do                                                       \
        {                                                        \
            int32_t posix_rcode;                                  \
            posix_rcode = pthread_mutex_unlock(&ipp_mutex);            \
            XASSERT(posix_rcode == 0, posix_rcode); \
        } while(0)

#define IPP_STATE_TRANSITION(ipp_request, next_state)                               \
        do{                                                                         \
            ASSERT(ipp_request->ipp_instance);                                      \
            DBG_VERBOSE("%s %08x from %s to %s\n", ipp_request->ipp_instance->name, \
                    ipp_request,                                                    \
                    g_ipp_state_strs[ipp_request->state],                           \
                    g_ipp_state_strs[next_state]);                                  \
                    ipp_request->state = next_state;                                \
        } while(0)

// send a message
// WARNING: do not call from a non-thread (e.g. timer)
// NOTE: ensures no messages are sent while in state IPP_STATE_CLEANUP
#define ipp_send_msg(type, ipp_request)                                        \
            ((ipp_request->state != IPP_STATE_CLEANUP)?                        \
            ipp_send_msg_wait_opt(type, (void *)ipp_request, POSIX_WAIT_FOREVER): \
            IPP_OK);

// send a message from a threadx non-thread (e.g. timer)
// NOTE: ensures no messages are sent while in state IPP_STATE_CLEANUP
#define ipp_send_msg_no_wait(type, ipp_request)                            \
             ((ipp_request->state != IPP_STATE_CLEANUP)?                   \
             ipp_send_msg_wait_opt(type, (void *)ipp_request, 0): \
             IPP_OK);


//=======================================================================================
// Public API
//=======================================================================================


void ipp_init(void)
{
    DBG_ENTER();

    int32_t posix_rcode;
    int32_t map_rcode;

    // TODO move to main.c
    map_rcode = map_init();
    XASSERT(map_rcode == SYS_OK, map_rcode);
#ifdef ENABLE_SINDOH_MOBILE_APP
    isSprintBusy=false;    
	posix_mutex_init(&_ipp_mutex);    
#endif
    /*
     *  create message queue
     */
    
    ASSERT(sizeof(ipp_msg_buf_t) == IPP_MQ_MSG_BUF_BYTES); // validation of msg buf size

    posix_create_message_queue(&ipp_msg_queue, "/ipp", IPP_MQ_NUM_MSG_BUFS, 
                               sizeof(ipp_msg_buf_t));
    /*
     *  create threads
     */
    posix_rcode = posix_create_thread(&ipp_thread, ipp_state_loop, (void *)0, "ipp", 
                                        &ipp_stack, IPP_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(posix_rcode)
    {
        DBG_ERR("error creating thread (posix_rcode=%d)\n", posix_rcode);
        goto error;
    }

    g_ipp_initialized = true;

    // rdj TODO TEMP: this should be called from outside module, e.g. at network init time
    // Update: with VPI support enabled, ipp_open is called outside module. 
#ifndef HAVE_VPI_SUPPORT
#ifdef HAVE_AIRPRINT
    void airprint_init(void);
    airprint_init();
#endif
#endif

    // Generate the printer and job attributes tables.
    generate_req_printer_attr_tbl();
    generate_req_job_attr_tbl();

    // success
    DBG_RETURN();
    return;

error:
    DBG_ERR("init failed!\n");
    DBG_RETURN();
    return;
}

ipp_inst_hndl_t ipp_open(const char *instance_name, const char *resource_str, int32_t port)
{
    ipp_instance_t *ipp_instance = NULL;
    map_handle_t attr_map = MAP_INVALID_HANDLE;
    int32_t ipp_rcode;

    DBG_VERBOSE("opening %s\n", instance_name);

    if(!g_ipp_initialized)
    {
        DBG_ERR("not initialized!\n");
        goto error;
    }

    attr_map = map_create(0); 
    if(attr_map == MAP_INVALID_HANDLE)
    {
        DBG_ERR("create attr_map failed!\n");
        goto error;
    }
    
    ipp_instance = (ipp_instance_t *)IPP_MALLOC(sizeof(ipp_instance_t));
    if(!ipp_instance)
    {
        DBG_ERR("low mem!\n");
        goto error;
    }

    memset(ipp_instance, 0, sizeof(ipp_instance_t));

#ifdef DEBUG
    ipp_instance->signature    = IPP_INST_CTXT_SIGNATURE;
#endif
    ipp_instance->name         = instance_name;
    ipp_instance->resource_str = resource_str;
    ipp_instance->port         = port;
    ipp_instance->ipp_requests = NULL;
    ipp_instance->attr_map     = attr_map;

    int i;
    for(i = 0; i < IPP_MAX_FEATURES; i++) 
    {
        ipp_instance->features[i] = NULL;
    }

    // add group names
    ipp_rcode = ipp_insert_attr_list(ipp_instance, g_ipp_grp_names, g_ipp_grp_name_cnt, IPP_CAT_GRP_NAME);
    if(ipp_rcode != SYS_OK)
    {
        DBG_ERR("add attr grps failed (ipp_rcode=%d)!\n", ipp_rcode);
        goto error;
    }

    // add our internal set of supported IPP attributes 
    ipp_rcode = ipp_insert_attr_list(ipp_instance, g_ipp_printer_attrs, g_ipp_printer_attr_cnt, IPP_CAT_ATTR);
    if(ipp_rcode != SYS_OK)
    {
        DBG_ERR("add attrs failed (ipp_rcode=%d)!\n", ipp_rcode);
        goto error;
    }

    error_type_t rcode;
    rcode = http_resource_register_event((char *)ipp_instance->resource_str, 
                                         ipp_instance->port, 
                                         0 /*flags*/,
                                         ipp_http_event, 
                                         (void *)ipp_instance);
    if(rcode != OK)
    {
        DBG_ERR("http error!\n");
        goto error;
    }

    return (ipp_inst_hndl_t)ipp_instance;

error:
    // cleanup
    if(ipp_instance)
    {
        IPP_FREE(ipp_instance);
    }
    if(attr_map != MAP_INVALID_HANDLE)
    {
        map_destroy(attr_map);
    }
    DBG_ERR("open %s failed!\n", instance_name); 

    return IPP_INVALID_HANDLE;
}

void ipp_close(ipp_inst_hndl_t instance_hndl)
{
    ipp_instance_t *ipp_instance;
    error_type_t status;

    ipp_instance = (ipp_instance_t *)instance_hndl;
#ifdef DEBUG
    XASSERT(ipp_instance->signature == IPP_INST_CTXT_SIGNATURE, (int)ipp_instance);
#endif

    status = http_resource_deregister( (char *)ipp_instance->resource_str, ipp_instance->port );
    XASSERT(status == OK, status);

#ifdef DEBUG
    ipp_instance->signature = 0;
#endif
    IPP_FREE(ipp_instance);

    return;
}

int32_t ipp_add_attributes(ipp_inst_hndl_t instance_hndl, ipp_attribute_t *attr_list, uint32_t attr_cnt)
{
    ipp_instance_t *ipp_instance = (ipp_instance_t *)instance_hndl;
#ifdef DEBUG
    XASSERT(ipp_instance->signature == IPP_INST_CTXT_SIGNATURE, (int)ipp_instance);
#endif

    int32_t map_rcode;
    map_rcode = ipp_insert_attr_list(ipp_instance, attr_list, attr_cnt, IPP_CAT_ATTR);
    return map_rcode;
}

int32_t ipp_replace_attributes(ipp_inst_hndl_t instance_hndl, ipp_attribute_t *attr_list, uint32_t attr_cnt)
{
    ipp_instance_t *ipp_instance = (ipp_instance_t *)instance_hndl;
#ifdef DEBUG
    XASSERT(ipp_instance->signature == IPP_INST_CTXT_SIGNATURE, (int)ipp_instance);
#endif

    int32_t map_rcode;
    map_rcode = ipp_insert_attr_list(ipp_instance, attr_list, attr_cnt, IPP_CAT_ATTR|IPP_MAP_OVERRIDE);
    return map_rcode;
}

int32_t ipp_add_feature(ipp_inst_hndl_t instance_hndl, const char *feature_name)
{
    ipp_instance_t *ipp_instance = (ipp_instance_t *)instance_hndl;
#ifdef DEBUG
    XASSERT(ipp_instance->signature == IPP_INST_CTXT_SIGNATURE, (int)ipp_instance);
#endif

    int i;
    for(i = 0; i < IPP_MAX_FEATURES; i++) 
    {
        if(ipp_instance->features[i] == NULL)
        {
            ipp_instance->features[i] = feature_name;
            break;
        }
    }
    if(i == IPP_MAX_FEATURES)
    {
        // no free slots
        XASSERT(0, i);
        return SYS_FAIL;
    }
    return SYS_OK;
}

int32_t ipp_get_response_hostname(ipp_req_hndl_t request_hndl, char *buf, uint32_t len)
{
    ipp_request_t *ipp_req = (ipp_request_t *)request_hndl;
#ifdef DEBUG
    XASSERT(ipp_req->signature == IPP_REQ_CTXT_SIGNATURE, (int)ipp_req);
#endif
    XASSERT(ipp_req->ipp_instance, (int)ipp_req);

    _get_correct_host_address(buf, len, ipp_req);

    return SYS_OK;
}

//=========================================================================================================================
// Private API - local to IPP module (declared in ipp.h)
//=========================================================================================================================

#define IPP_READER_TIMESLICE 500
int http_resource_read_friendly(ipp_request_t *ipp_req, char *buf, int len, uint32_t *timeout)
{
    ASSERT(timeout);
    
    int bytes_read = 0;
    uint32_t start_time, timeslice;
    int32_t timeleft;
    int cur_bytes_read;
    
   DBG_VERBOSE("%s - %x: (buf=%08x, len=%d, timeout=%d)\n",
               __func__, ipp_req->http_hndl, buf, len, *timeout);
               
    // Save off current time at start of function to determine timeout later
    start_time = posix_gettime_ticks();
              
    timeleft = MAX( 0, (int32_t)(*timeout - ( (posix_gettime_ticks() - start_time) * MS_PER_TICK)) );
    while (bytes_read < len)
    {
        if (timeleft <= 0)
        {
            timeleft = 0;
            break;
        }
        
        timeslice = (uint32_t)MIN(IPP_READER_TIMESLICE, timeleft);
        
        DBG_VERBOSE("%s - %x: http_resource_read timeleft=%d, timeslice=%d\n",
                   __func__, ipp_req->http_hndl, timeleft, timeslice);
                   
        cur_bytes_read = http_resource_read(ipp_req->http_hndl, buf + bytes_read, len - bytes_read, &timeslice);
        if (cur_bytes_read > 0)
        {
            bytes_read += cur_bytes_read;
        }
        else if (cur_bytes_read == 0)
        {
            // zero bytes read, not a timeout (EOF?)
            break;
        }
        else 
        {
            if (timeslice == 0)
            {
                // timed out, just continue
            }
            else
            {
                if (bytes_read == 0)
                {
                    DBG_ERR("%s - %x: http resource read error! (timeout=%d)\n",
                               __func__, ipp_req->http_hndl, timeslice);
                               
                    bytes_read = -1;
                }
                
                // no timeout, done reading
                break;
            }
        }
        
        timeleft = MAX( 0, (int32_t)(*timeout - ( (posix_gettime_ticks() - start_time) * MS_PER_TICK)) );
    }
                       
    *timeout = (uint32_t)timeleft;
    
    // return error on timeout
    if( (*timeout == 0) && (bytes_read == 0) )
    {
        DBG_ERR("%s - %x: http resource read timeout! (timeout=%d)\n",
                   __func__, ipp_req->http_hndl, *timeout);
        bytes_read = -1;
    }
    
    DBG_VERBOSE("%s - %x: bytes read=%d, timeout=%d\n",
               __func__, ipp_req->http_hndl, bytes_read, *timeout);
    return bytes_read;
}

// temp hook
int ipp_reader(void *data, void *buf, int len)
{
    ASSERT(data);
    ipp_request_t *ipp_req = (ipp_request_t *)data;
    uint32_t timeout = ipp_req->io_timeout;

    int32_t num_bytes = http_resource_read_friendly(ipp_req, (char *)buf, len, &timeout);

    return num_bytes;
}

int32_t ipp_smjob_reader(char *buf, uint32_t len, uint32_t *timeout_ms, void *user_data)
{
   ASSERT(user_data);
   ipp_request_t *ipp_req = (ipp_request_t *)user_data;

   DBG_VERBOSE("%s mutex lock ipp request %08x\n", __func__, ipp_req);
   IPP_LOCK_CONTEXT(ipp_req->access_mtx);

   DBG_VERBOSE("%s - %x: (timeout_ms=%d, buf=%08x, len=%d, tmp_req_len=%d)\n",
               __func__, ipp_req->http_hndl, *timeout_ms, buf, len,
               ipp_req->tmp_data_len);

   int32_t num_bytes = 0;

   ASSERT(ipp_req->ipp_ctxt);
   //XASSERT(ipp_req->tmp_data_len >= 0, ipp_req->tmp_data_len);

   if (ipp_req->tmp_data_len > 0)
   {
      size_t to_copy = (ipp_req->tmp_data_len < len) ? ipp_req->tmp_data_len : len;
      memcpy(buf, ipp_req->tmp_data + ipp_req->tmp_data_read, to_copy);
      ipp_req->tmp_data_read += to_copy;
      ipp_req->tmp_data_len -= to_copy;

      char *tmp_buf = buf + to_copy;
      XASSERT(len >= to_copy, to_copy);
      uint32_t tmp_len = len - to_copy;

      DBG_VERBOSE("%s - %x: (to_copy=%d, tmp_req_read=%d, tmp_req_len=%d, tmp_buf=%08x,"\
                     " tmp_len=%d)\n", __func__, ipp_req->http_hndl,
                  to_copy, ipp_req->tmp_data_read, ipp_req->tmp_data_len, tmp_buf,
                  tmp_len);

      num_bytes = to_copy;
      if (tmp_len)
      {
         int32_t tmp_num_bytes;
         tmp_num_bytes = http_resource_read_friendly(ipp_req,
                                            tmp_buf,
                                            tmp_len,
                                            timeout_ms);
         if (tmp_num_bytes >= 0)
         {
            num_bytes += tmp_num_bytes;
         }
      }
   }
   else if (ipp_req->tmp_data_len == 0)
   {
      num_bytes = http_resource_read_friendly(ipp_req, buf, len, timeout_ms);
   }
   else
   {
      // should not get here
      DBG_ERR("req %d: print job timeout!\n", 
              ipp_req->http_hndl);
   
      ipp_req->job_http_error = true;
      num_bytes = 0;
   }
   
   DBG_VERBOSE("%x: job reader num_bytes=%d, timeout=%d\n", ipp_req->http_hndl, num_bytes, *timeout_ms);

   if (num_bytes <= 0)
   {
      if (num_bytes == 0)
      {
         ipp_req->job_http_eof = true;
      }
      else if (*timeout_ms != 0) // ignore timeout errors - these are recoverable
      {
         ipp_req->job_http_error = true;
      }
   }

   DBG_VERBOSE("%s mutex unlock ipp request %08x\n", __func__, ipp_req);
   IPP_UNLOCK_CONTEXT(ipp_req->access_mtx);

   return num_bytes;
}

ipp_u16bit_t ipp_get_status_from_smjob_reason(smjob_status_t *job_status)
{
    ipp_u16bit_t status = IPP_STAT_OK;
    int i = 0;
    
    ASSERT(job_status);

    while (status == IPP_STAT_OK && i < job_status->num_reasons)
    {
        switch (job_status->reasons[i])
        {
            case SMJOB_STATE_REASON_JOB_CANCELED_BY_USER:
            case SMJOB_STATE_REASON_JOB_CANCELED_BY_OPERATOR:
            case SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE:
                status = IPP_STAT_SRV_JOB_CANCELED;
                break;
                
            case SMJOB_STATE_REASON_UNSUPPORTED_DOC_FORMAT:
               status = IPP_STAT_CLI_DOCUMENT_FORMAT_NOT_SUPPORTED;
               break;
              
            case SMJOB_STATE_REASON_DOC_FORMAT_ERROR:
               status = IPP_STAT_CLI_DOCUMENT_FORMAT_ERROR;
               break;
                     
            /* TODO: expand cases here */

            default:
                break;
        }
        
        i++;
    }
    
    return status;
}

void ipp_smjob_status_event(uint32_t job_id, smjob_status_events_t job_events, void *user_data)
{
    smjob_rcode_t  smjob_rcode;
    smjob_status_t job_status;

    ASSERT(user_data);
    ipp_request_t *ipp_req = (ipp_request_t *)user_data;

    smjob_rcode = smjob_get_status(job_id, &job_status);
    XASSERT(smjob_rcode == SMJOB_OK, smjob_rcode);
    DBG_VERBOSE("req %d: ipp_smjob_status_event (job_events=0x%x, smjob_state=%d)\n", 
                ipp_req->http_hndl, job_events, job_status.state);
        
    if (job_events & SMJOB_STATUS_EVENT_STATE_REASONS)
    {
        DBG_VERBOSE("req %d: change in job event state reason\n", ipp_req->http_hndl);
        if (job_status.num_reasons)
        {
           ipp_u16bit_t status = ipp_get_status_from_smjob_reason(&job_status);
           DBG_VERBOSE("req %d: updating status=%d\n", ipp_req->http_hndl, status);
            
          ipp_req->ipp_status = status;
        }
    }

    if (job_events & SMJOB_STATUS_EVENT_STATE)
    {
        DBG_VERBOSE("req %d: change in job event state(smjob_state=%d)\n", 
                    ipp_req->http_hndl, job_status.state);
        
        switch (job_status.state)
        {
            case SMJOB_STATE_CANCELED:
            case SMJOB_STATE_ABORTED:
            case SMJOB_STATE_COMPLETED:
                DBG_VERBOSE("req %d: got job done (smjob_state=%d)\n", ipp_req->http_hndl, 
                            job_status.state);
                ipp_request_processing_complete(ipp_req);
                break;
            
            default:
                // do nothing
                break;
        }
    }
}

void ipp_request_processing_complete(ipp_request_t *ipp_req)
{
    ASSERT(ipp_req);

    ipp_rcode_t ipp_rcode = ipp_send_msg(IPP_MSG_SEND_RESPONSE, ipp_req);
    UNUSED_VAR(ipp_rcode);
    XASSERT(ipp_rcode == IPP_OK, ipp_rcode);
    return;
}

//rdj added feb-2014
int32_t ipp_insert_attr_list(ipp_instance_t *ipp_inst, ipp_attribute_t *attr_list, uint32_t attr_cnt, ipp_flags_t flags)
{
    ASSERT(ipp_inst);
    int32_t ipp_rcode = SYS_OK;
    int32_t map_rcode;
    int i;
    for(i = 0; i < attr_cnt; i++) 
    {
        // only public group flags (defined in ipp_api.h) may be set directly from initializer list
        XASSERT(!(attr_list[i].flags & ~IPP_GRP_MASK), (int)attr_list[i].flags);
        attr_list[i].flags &= IPP_GRP_MASK;

        // apply any internal flags
        attr_list[i].flags |= flags; 

        // verify IPP_CAT_ATTR entries are assigned to one and only one group and that all entries
        // have one and only one category -- note that at most one bit is set if (x&(x-1))==0
        XASSERT((attr_list[i].flags & IPP_GRP_MASK) && (!(attr_list[i].flags & IPP_CAT_ATTR) ||
               !((attr_list[i].flags & IPP_GRP_MASK) & ((attr_list[i].flags & IPP_GRP_MASK)-1))), i);
        XASSERT((attr_list[i].flags & IPP_CAT_MASK) &&
               !((attr_list[i].flags & IPP_CAT_MASK) & ((attr_list[i].flags & IPP_CAT_MASK)-1)), i);

        if(flags & IPP_MAP_OVERRIDE)
        {
            // first remove any pre-existing IPP attribute of same name
            void *val;
            val = map_remove(ipp_inst->attr_map, attr_list[i].name, strlen(attr_list[i].name));
            if(!val)
            {
                // there was no pre-existing IPP attribute with this name so clear the flag
                attr_list[i].flags &= ~IPP_MAP_OVERRIDE;
            }
        }

        map_rcode = map_insert(ipp_inst->attr_map, attr_list[i].name, 
                               strlen(attr_list[i].name), (void *)&attr_list[i]);
        XASSERT(map_rcode == SYS_OK || map_rcode == MAP_DUP_KEY, map_rcode); 
        if(map_rcode == MAP_DUP_KEY)
        {
            // note this is not necessarily a fatal error
            ipp_rcode = IPP_DUP_ATTR;
        }
    }

    return ipp_rcode;
}


//=======================================================================================
// Thread Entry Point and State Loop
//=======================================================================================

/*   
 *  IMPORTANT NOTE:  Correct operation of this state machine depends on the following:
 * 
 *  1) The value of the state variable MUST NOT be directly modified outside of the state
 *  machine.
 * 
 *  2) Adding a message to the queue MUST NOT be conditional on the current value of the
 *  state variable.
 * 
 *  These conditions are required because the current value of the state variable does not
 *  account for messages pending in the message queue and therefore does not reflect
 *  actual state. The value of the state variable is only accurate relative to the last
 *  message processed, whereas actual state is a function of the current state variable
 *  value plus any messages still pending in the message queue. Once a message is sent it
 *  cannot be taken back, and its effect on the state variable is determined the moment it
 *  enters the message queue.
 * 
 *  When reading the state variable from outside the state machine, be aware that the
 *  the state variable value lags behind actual state.
 */
static void *ipp_state_loop(void *entry_input)
{
    int32_t posix_rcode;
    ipp_msg_buf_t msg_buf;
    ipp_msg_t *msg;
    ipp_request_t *ipp_req;
    ipp_instance_t *ipp_inst;

    DBG_VERBOSE("starting state loop\n");

    while(1)
    {
        ipp_req = NULL;
        ipp_inst = NULL;

        posix_rcode = posix_wait_for_message(ipp_msg_queue, (char *)&msg_buf, 
                                             sizeof(ipp_msg_t), POSIX_WAIT_FOREVER);
        if(posix_rcode != 0)
        {
            DBG_ERR("error reading ipp_msg_queue (posix_rcode=%d)\n", posix_rcode);
            break;
        }

        msg = &msg_buf.msg;
        ASSERT(msg->type < IPP_NUM_MSGS);

        ipp_req = (ipp_request_t *)msg->data;
        if(ipp_req)
        {
            DBG_VERBOSE("%s %08x got msg %s in state %s\n", ipp_req->ipp_instance->name,
                        ipp_req, g_ipp_msg_strs[msg->type], 
                        g_ipp_state_strs[ipp_req->state]);
        }
        else
        {
            DBG_VERBOSE("got stateless msg %s\n", g_ipp_msg_strs[msg->type]);
        }


        /*
         *  process stateful messages
         */

        if(!ipp_req)
        {
            // message is not stateful - ignore
            continue;
        }

        ipp_inst = ipp_req->ipp_instance;
        ASSERT(ipp_inst);

        ASSERT(ipp_req->state < IPP_NUM_STATES);
        switch(ipp_req->state)
        {
        /***************************************************
         * IPP_STATE_PROCESS_HTTP_HEADER
         ***************************************************/
        case IPP_STATE_PROCESS_HTTP_HEADER:

            switch(msg->type)
            {
                case IPP_MSG_PROCESS_HTTP_HEADER:
                {
                    // received a new HTTP request
                    IPP_STATE_TRANSITION(ipp_req, IPP_STATE_PROCESS_REQUEST);
                    ipp_rcode_t ipp_rcode = ipp_send_msg(IPP_MSG_PROCESS_REQUEST, ipp_req);
                    UNUSED_VAR(ipp_rcode);
                    XASSERT(ipp_rcode == IPP_OK, ipp_rcode);
                    break;
                }

                default:
                    // ignore other messages
                    DBG_VERBOSE("%s %08x ignoring msg %s!\n", ipp_inst->name, ipp_req,
                            g_ipp_msg_strs[msg->type]);
                    break;
            }
            break;

        /***************************************************
         * IPP_STATE_PROCESS_REQUEST
         ***************************************************/
        case IPP_STATE_PROCESS_REQUEST:

            switch(msg->type)
            {
            case IPP_MSG_PROCESS_REQUEST:
            {
            #ifdef HAVE_IPV6
               char network_address[INET6_ADDRSTRLEN];
            #else
               char network_address[INET_ADDRSTRLEN];
            #endif
               uint32_t error;

               error = ipp_get_network_ip_address_from_connection(
                                      ipp_req->http_hndl,
                                      &network_address[0],
                                      sizeof(network_address));
               UNUSED_VAR(error);
               ASSERT(error == OK);

               ipp_req->ipp_ctxt = ipp_create_context("ipp", 
                                                      &network_address[0], 
                                                      ipp_req->ipp_instance->port, 
                                                      "ipp");
               if(ipp_req->ipp_ctxt)
               {
                  ipp_req->ipp_ctxt->ipp_req = ipp_req;
                  int r = ipp_handle_request(ipp_req->ipp_ctxt, 
                                             "dummy_uri", 
                                             ipp_reader);
                  UNUSED_VAR(r);
               }
               else
               {
                    DBG_VERBOSE("req %d: no response data\n", 
                                ipp_req->http_hndl);
               }

               IPP_STATE_TRANSITION(ipp_req, IPP_STATE_SEND_RESPONSE);
               break;
            }

            default:
                // ignore other messages
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", ipp_inst->name, ipp_req,
                            g_ipp_msg_strs[msg->type]);
                break;
            }
            break;

        /***************************************************
         * IPP_STATE_SEND_RESPONSE
         ***************************************************/
        case IPP_STATE_SEND_RESPONSE:

            switch(msg->type)
            {
                case IPP_MSG_SEND_RESPONSE:
                {
                    char tmp_buf[512];
                    int bytes_read;
                    uint32_t timeout;
                    uint32_t tout;

                    // skip if EOF or HTTP ERROR detected
                    if (!ipp_req->job_http_eof && !ipp_req->job_http_error)
                    {
                        DBG_VERBOSE("req %d: checking for any remaining http data to dump...\n", 
                                    ipp_req->http_hndl);

                        // Reset if repsonse drain timeout is zero - this indicates its the 
                        // first time or it's been reset
                        if (ipp_req->job_resp_drain_timeout == 0)
                        {
                            DBG_VERBOSE("req %d: resetting timeout\n", 
                                        ipp_req->http_hndl);
                                        
                            ipp_req->job_resp_drain_timeout = ipp_req->io_timeout;
                        }
                                    
                        // determine timeout - should not be more than 500ms
                        timeout = (ipp_req->job_resp_drain_timeout) < 500 ? ipp_req->job_resp_drain_timeout : 500;
                        tout = timeout;
                        
                        // get start time - only want to drain data from pipe for appox 500ms at a time
                        uint32_t start_time =  posix_gettime_ticks();
                        
                        // loop while timeout has not expired, not EOF and no HTTP error
                        while (tout && !ipp_req->job_http_eof && !ipp_req->job_http_error)
                        {
                            // attempt to drain 32 bytes from pipe
                            bytes_read = ipp_smjob_reader(tmp_buf, 512, &tout, (void *)ipp_req);
                            
                            // determine if there was a read error or not
                            if (bytes_read < 0)
                            {
                                // check for read timeout
                                if (tout == 0)
                                {
                                    // decrement the drain timeout
                                    ipp_req->job_resp_drain_timeout -= timeout;
                                    
                                    // check to see if the drain timeout has expired (no data)
                                    if (ipp_req->job_resp_drain_timeout == 0)
                                    {
                                        DBG_MSG("req %d: timed out waiting to drain pipe, indicate HTTP error\n", 
                                                    ipp_req->http_hndl);
                                                    
                                        // When draining the pipe, if we hit a full IO timeout that's really
                                        // an error.
                                        ipp_req->job_http_error = true;
                                    }
                                    else
                                    {
                                        DBG_VERBOSE("req %d: timed out, IO time left = %d\n", 
                                                    ipp_req->http_hndl, ipp_req->job_resp_drain_timeout);
                                    }
                                }
                                else
                                {
                                    // something went wonky with the read
                                    DBG_MSG("req %d: HTTP error reading pipe\n", 
                                                ipp_req->http_hndl);
                                    ipp_req->job_http_error = true;
                                }
                            }
                            else
                            {
                                // determine elapse time in ms
                                uint32_t elapse_time = (posix_gettime_ticks() - start_time)*10;
                                
                                // decrement tout or set to 0 if greater than time left
                                if (elapse_time > tout)
                                    tout = 0;
                                else
                                    tout -= elapse_time;
                                    
                                DBG_VERBOSE("req %d: drained %d bytes from pipe, time left %d\n", 
                                            ipp_req->http_hndl, bytes_read, tout);
                                
                                // since we have read data, reset drain timeout
                                ipp_req->job_resp_drain_timeout = ipp_req->io_timeout;
                            }
                        }
                    }
                    
                    // check for EOF or HTTP error
                    if (ipp_req->job_http_eof || ipp_req->job_http_error)
                    {
                        DBG_VERBOSE("req %d: sending http reponse\n", 
                                    ipp_req->http_hndl);
                        
                        // if no HTTP error (then EOF), send reponse
                        if (!ipp_req->job_http_error)
                        {
                            // build the response
                            ipp_build_response(ipp_req);

                            http_resource_set_status_code(ipp_req->http_hndl, HTTP_200_OK);
                            http_resource_set_content_length(ipp_req->http_hndl, 
                                                             ipp_req->response_len);
                            http_resource_set_content_type(ipp_req->http_hndl, HTTP_TYPE_IPP);
                            http_resource_header_complete(ipp_req->http_hndl);
                            
                            DBG_VERBOSE("req %d: sending %d bytes of response data\n", 
                                          ipp_req->http_hndl, ipp_req->response_len);
                                          
                            if(ipp_req->response_len)
                            {
                                int32_t num_sent = http_resource_write(ipp_req->http_hndl,
                                                                       ipp_req->response_data,
                                                                       ipp_req->response_len );
                                // Note that if http_resource_write returns -1, then something 
                                // has gone wonky with the connection (probably aborted).  Not
                                //  really assert worthy, just let folks know what happened.
                                if (num_sent == -1)
                                {
                                    DBG_ERR("req %d: response http_resource_write failed\n", 
                                            ipp_req->http_hndl);
                                }
                                else
                                {
                                    XASSERT(num_sent == ipp_req->response_len, num_sent);
                                    //UNUSED_VAR(num_sent);
                                    
                                    // indicate we passed the buffer off to HTTP
                                    ipp_req->response_data = NULL;
                                    ipp_req->response_len = 0;
                                }
                            }
                            
                            http_resource_send_complete(ipp_req->http_hndl);
                        }
                        else
                        {
                            // if HTTP error, reset connection
                            DBG_ERR("req %d: HTTP error, reset connection\n", ipp_req->http_hndl);
                            http_resource_reset_connection(ipp_req->http_hndl);
                        }
                        
                        /*
                         * Must send cleanup msg BEFORE transition to cleanup state.
                         * 
                         * No messages are allowed to be sent while in the cleanup state, 
                         *  
                         * because the ipp request context is in process of being freed.
                         * 
                         * NOTE: This must be done from within state loop to guarantee msg is
                         * processed after transition to cleanup state.  Msg must be sent before
                         * transition but processed after transition.
                         */
                        ipp_rcode_t ipp_rcode = ipp_send_msg(IPP_MSG_CLEANUP, ipp_req);
                        UNUSED_VAR(ipp_rcode);
                        XASSERT(ipp_rcode == IPP_OK, ipp_rcode);
                        
                        // NOTE: any messages for this ipp_req sent after the transition to
                        // IPP_STATE_CLEANUP will be ignored.
                        IPP_STATE_TRANSITION(ipp_req, IPP_STATE_CLEANUP);
                    }
                    else
                    {
                        // if not EOF or HTTP error, then we are still trying to drain the pipe.
                        ipp_rcode_t ipp_rcode = ipp_send_msg(IPP_STATE_SEND_RESPONSE, ipp_req);
                        UNUSED_VAR(ipp_rcode);
                        XASSERT(ipp_rcode == IPP_OK, ipp_rcode);
                    }
                    break;
                }

                default:
                    // ignore other messages
                    DBG_VERBOSE("%s %08x ignoring msg %s!\n", ipp_inst->name, ipp_req,
                                g_ipp_msg_strs[msg->type]);
                    break;
            }
            break;

        /***************************************************
         * IPP_STATE_CLEANUP
         ***************************************************/
        case IPP_STATE_CLEANUP:

            switch(msg->type)
            {
            case IPP_MSG_CLEANUP:
                DBG_VERBOSE("%s free ipp request %08x\n", ipp_inst->name, ipp_req);
                ASSERT(ipp_req);
                ASSERT(ipp_req->ipp_ctxt);

                // must destroy map before cntxt since ipp group structs contain the map keys
                if(ipp_req->req_attrs.map != MAP_INVALID_HANDLE)
                {
                    map_destroy(ipp_req->req_attrs.map);
                }

                ipp_destroy_context(ipp_req->ipp_ctxt);
                ipp_free_request(ipp_req);
                // must not receive any futher messages referencing this ipp request context

                break;

            default:
                // ignore other messages
                DBG_VERBOSE("%s %08x ignoring msg %s!\n", ipp_inst->name, ipp_req,
                            g_ipp_msg_strs[msg->type]);
                break;
            }
            break;

        default:
            // unknown state - ignore
            XASSERT(0, ipp_req->state);
            break;
        }
    }

    DBG_VERBOSE("exiting\n");
    return 0;
}


//=======================================================================================
// State Loop Helper Routines
// NOTE: these routines must only be called directly from state machine
//=======================================================================================


//=======================================================================================
// Helper Routines
//=======================================================================================

static ipp_rcode_t ipp_send_msg_wait_opt(ipp_msg_type_t type, void *data, uint32_t posix_wait_opt)
{
    ipp_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.data = data;

    int32_t posix_rcode = posix_message_send(ipp_msg_queue, (char *)&buf, sizeof(buf),
                                             MQ_DEFAULT_PRIORITY, posix_wait_opt);
    if(posix_rcode == ETIMEDOUT)
    {
        /* 
         *  msg queue full
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and
         *  threadx_wait_opt must therefore be set to TX_NO_WAIT.
         */
        return IPP_WOULDBLOCK;
    }
    else if(posix_rcode)
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, posix_rcode);
        return IPP_FAIL;
    }

    return IPP_OK;
}

// WARNING: called from context of HTTP callback
static ipp_request_t *ipp_create_request(ipp_instance_t *ipp_instance,
                                         http_request_t http_hndl)
{
    ipp_rcode_t  ipp_rcode = IPP_OK;
    error_type_t rcode;

    ipp_request_t *ipp_req = (ipp_request_t *)IPP_MALLOC(sizeof(ipp_request_t));
    ASSERT(ipp_req);

    memset(ipp_req, 0, sizeof(ipp_request_t));

#ifdef DEBUG
    ipp_req->signature      = IPP_REQ_CTXT_SIGNATURE;
#endif
    ipp_req->ipp_instance   = ipp_instance;
    ipp_req->http_hndl      = http_hndl;
    ipp_req->state          = IPP_STATE_PROCESS_HTTP_HEADER;
    ipp_req->job_http_eof   = false;
    ipp_req->job_http_error = false;
    ipp_req->job_done_event = false;
    ipp_req->job_resp_drain_timeout = 0;
    ipp_req->job_id         = SMJOB_INVALID_JOB_ID;
    ipp_req->req_attrs.grp_mask = 0;
    ipp_req->req_attrs.map  = MAP_INVALID_HANDLE;
    ipp_req->oid_buf = (char *)IPP_MALLOC(OID_BUF_MAX_LEN);
    ASSERT(ipp_req->oid_buf);

    int32_t posix_rcode;
    posix_rcode =  posix_mutex_init(&ipp_req->access_mtx);
    XASSERT(posix_rcode == 0, posix_rcode);

#ifdef IPP_TIME_LOG
    ipp_req->start_ticks = posix_gettime_ticks();
#endif //IPP_TIME_LOG

    uint16_t io_timeout_sec;
    platvars_get_io_timeout(&io_timeout_sec);
    ipp_req->io_timeout = (io_timeout_sec * 1000);

    rcode = http_resource_get_header_length( http_hndl, &ipp_req->http_hdr_len );
    if(rcode != OK)
    {
        ipp_rcode = FAIL;
        goto done;
    }

    ipp_req->http_hdr = (char *)IPP_MALLOC( ipp_req->http_hdr_len );
    ASSERT(ipp_req->http_hdr);

    rcode = http_resource_get_header( http_hndl, ipp_req->http_hdr, ipp_req->http_hdr_len );
    if(rcode != OK)
    {
        ipp_rcode = FAIL;
        goto done;
    }

done:
    if(ipp_rcode != IPP_OK)
    {
        // free resources
        if(ipp_req)
        {
            ipp_free_request(ipp_req);
            ipp_req = NULL;
        }
    }

    return ipp_req;
}

static void ipp_free_request(ipp_request_t *ipp_req)
{
    ASSERT(ipp_req);

    MEM_FREE_AND_NULL(ipp_req->oid_buf);

    pthread_mutex_destroy(&ipp_req->access_mtx);

    // free response data if not passed to HTTP for some reason
    if (ipp_req->response_data) MEM_FREE_AND_NULL(ipp_req->response_data);

    IPP_FREE(ipp_req->http_hdr);

#ifdef DEBUG
    ipp_req->signature = 0;
#endif

    IPP_FREE(ipp_req);

    return;
}

//=======================================================================================
// Callback Routines
//=======================================================================================

static void ipp_http_event( http_request_t http_hndl, http_resource_event_flag_t event, void *usr_ctxt )
{
    ASSERT(usr_ctxt);
    ipp_rcode_t ipp_rcode;
    ipp_instance_t *ipp_instance = (ipp_instance_t *)usr_ctxt;

    DBG_VERBOSE("req %d: ipp_http_event - %d\n", http_hndl, event);
    
    switch (event)
    {
        case HTTP_RESOURCE_HEADER_DATA_AVAILABLE:
        {
            ipp_request_t *ipp_req = ipp_create_request(ipp_instance, http_hndl);
            if(ipp_req)
            {
                ipp_rcode = ipp_send_msg(IPP_MSG_PROCESS_HTTP_HEADER, ipp_req);
                XASSERT(ipp_rcode == IPP_OK, ipp_rcode);
            }
            else
            {
                // at this point all we can do is reset the TCP connection
                http_resource_reset_connection(http_hndl);
            }
            break;
        }
        case HTTP_RESOURCE_CONNECTION_ABORTED:
            DBG_VERBOSE( "http_test: CONNECTION ABORTED\n" );
            break;            
    }
}

//rdj added feb-2014
// translation function callback
// buf [out]
// len [in/out]
static int32_t ipp_features_supported_func(ipp_req_hndl_t request_hndl, oid_t oid, uint32_t index, char *buf, uint32_t *len)
{
    int32_t ret = SYS_OK;
    uint32_t out_len = 0; 

    ipp_request_t *ipp_req = (ipp_request_t *)request_hndl;
#ifdef DEBUG
    XASSERT(ipp_req->signature == IPP_REQ_CTXT_SIGNATURE, (int)ipp_req);
#endif
    XASSERT(ipp_req->ipp_instance, (int)ipp_req);

    // confirm no OID defined
    ASSERT(oid == OID_INVALID);

    if(index >= IPP_MAX_FEATURES || ipp_req->ipp_instance->features[index] == NULL)
    {
        ret = OID_ERROR_INDEX_INVALID;
        goto done;
    }

    out_len = strlen(ipp_req->ipp_instance->features[index]);
    if(out_len > *len)
    {
        out_len = 0;
        ret = OID_ERROR_BUFFER_TOO_SMALL;
        goto done;
    }
    memcpy(buf, ipp_req->ipp_instance->features[index], out_len);

done:
    *len = out_len;
    return ret;
}
