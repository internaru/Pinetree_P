/*
 *
 * ============================================================================
 * Copyright (c) 2011   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __INC_IPP_API_H__
#define __INC_IPP_API_H__

#include "pthread.h"
#include "error_types.h"
#include "map_api.h"
#include "http_api.h"
#include "sm_job_api.h"
#include "map_api.h"
#include "ipp_api.h"

// module local includes
#include "ipp_types.h"

#ifdef __cplusplus
extern "C" {
#endif

//Uncomment/Comment the following line to Enable/Disable Sindoh Mobile App.
#define ENABLE_SINDOH_MOBILE_APP
#if DEBUG
 //#define TRACK_ALLOC 1
 #ifdef TRACK_ALLOC
  void *ipp_malloc(const char *func_name, unsigned int line, unsigned int size);
  void *ipp_realloc(const char *func_name, unsigned int line, void *ptr, unsigned int size);
  void *ipp_calloc(const char *func_name, unsigned int line, unsigned int blocks, unsigned int bytes);
  void ipp_free(const char *func_name, unsigned int line, void *ptr);
  #define IPP_MALLOC(size) ipp_malloc(__FILE__, __LINE__, size)
  #define IPP_REALLOC(ptr, size) ipp_realloc(__FILE__, __LINE__, ptr, size)
  #define IPP_CALLOC(blocks, bytes) ipp_calloc(__FILE__, __LINE__, blocks, bytes)
  #define IPP_FREE(ptr) ipp_free(__FILE__, __LINE__, ptr)
 #else
 #define IPP_MALLOC(size) MEM_MALLOC(size)
 #define IPP_REALLOC(ptr, size) MEM_REALLOC(ptr, size)
 #define IPP_CALLOC(blocks, bytes) MEM_CALLOC(blocks, bytes)
 #define IPP_FREE(ptr) MEM_FREE_AND_NULL(ptr)
 #endif
#else
 #define IPP_MALLOC(size) MEM_MALLOC(size)
 #define IPP_REALLOC(ptr, size) MEM_REALLOC(ptr, size)
 #define IPP_CALLOC(blocks, bytes) MEM_CALLOC(blocks, bytes)
 #define IPP_FREE(ptr) MEM_FREE_AND_NULL(ptr)
#endif


//#define IPP_TIME_LOG

#ifdef HAVE_VPI_SUPPORT
 // VPI is slower at pulling data, so don't buffer up so much
 // as it causes APVT to fail because response can be 10+ seconds
 // after all print data has been received
 #define IPP_BULK_TCP_RECV_BUF       (8 * 1024)
#else
 #define IPP_BULK_TCP_RECV_BUF       (30 * 1024)
#endif

/* This is a data IO accessor function */
typedef int (*ipp_reader_t)(void* data, void* dst, int maxlen);

/* Not that it is not expected for IPP to be multiply instantiated but there is
   nothing to prevent this from happening. Even though, therefore, we would not
   normally need a context handle using this model allows us to hang certain
   pieces of 'global' data within the context handle. */
struct ipp_context_t;

struct ipp_context_t* ipp_create_context(const char*  protocol,
                                         const char*  server_address,
                                         unsigned int port_number,
                                         const char*  root_uri);

int          ipp_handle_request(struct ipp_context_t* context,
                                const char* uri,
                                ipp_reader_t reader);

void         ipp_destroy_context(struct ipp_context_t* context);




/*
 *  NEW DEFINES 15-DEC-2012
 */

#define IPP_MAX_FEATURES 2

typedef enum
{
    IPP_STATE_PROCESS_HTTP_HEADER,
    IPP_STATE_PROCESS_REQUEST,
    IPP_STATE_SEND_RESPONSE,
    IPP_STATE_CLEANUP,

    // add new states above this line
    IPP_NUM_STATES
} ipp_state_t;

struct ipp_request_s;
struct ipp_context_t;

typedef struct ipp_instance_s
{
#ifdef DEBUG
    uint32_t                signature;
#endif                      
    struct ipp_request_s    *ipp_requests; // list of all active IPP requests
    const char              *name; // instance name, e.g. "airprint"
    const char              *resource_str;
    int32_t                 port;
    map_handle_t            attr_map; // supported IPP attributes
    const char              *features[IPP_MAX_FEATURES];

    // hooks for IPP
    struct ipp_context_t    *ipp_ctxt;

} ipp_instance_t;


#ifdef HAVE_VPI_SUPPORT
 // VPI needs to be at least 2048 bytes for first read, so let double that to just be sure
 #define IPP_TMP_DATA_SIZE   4096
#else
 #define IPP_TMP_DATA_SIZE   100
#endif

// rdj moved from ipp_request.c
// How much memory to acquire for the OIDs.
#define OID_BUF_MAX_LEN 1024
#define OID_BYTE_ARRAY_SIZE 16

// ipp hash table member categories
#define IPP_CAT_ATTR        0x00000100 // user-defined attribute e.g. added via ipp_add_attributes()
#define IPP_CAT_GRP_NAME    0x00000200 // IPP group name e.g. "job-template", "printer-description", "all"
#define IPP_CAT_MASK       (IPP_CAT_ATTR|IPP_CAT_GRP_NAME)
#define IPP_MAP_OVERRIDE    0x01000000 // used either to request that a new attribute override and replace any
                                       // pre-exiting attribute of same name or to mark an attribute as an override

// structure used to track requested attributes
typedef struct 
{
    uint32_t grp_mask; // for attribute groups
    map_handle_t map; // for individual attributes
} requested_attrs_t;


typedef struct ipp_request_s    
{                               
#ifdef DEBUG
    uint32_t                signature;
#endif                     
    struct ipp_request_s    *next;
    ipp_instance_t          *ipp_instance;
    http_request_t          http_hndl;
    ipp_state_t             state;
    uint32_t                http_hdr_len;
    char                    *http_hdr;
    uint32_t                io_timeout; // milliseconds

    // hooks for _&_ IPP    
    struct ipp_context_t    *ipp_ctxt;
                            
    void                    *response_data;
    unsigned int            response_len;
                            
    uint32_t                job_id;
    unsigned int            ipp_version;
    unsigned int            ipp_request_op;
    unsigned int            ipp_request_id;
    ipp_u16bit_t            ipp_status;
                            
    char                    tmp_data[IPP_TMP_DATA_SIZE];
    int                     tmp_data_len;
    int                     tmp_data_read;

    bool                    job_http_eof;
    bool                    job_http_error;
    bool                    job_done_event;
                            
    uint32_t                job_resp_drain_timeout;
    pthread_mutex_t         access_mtx;
    char                    name[32];

    requested_attrs_t       req_attrs; // "requested-attributes"
    char                    *oid_buf;

#ifdef IPP_TIME_LOG         
    uint32_t                start_ticks;
#endif // IPP_TIME_LOG

} ipp_request_t;

int ipp_reader(void *data, void *buf, int len);
int32_t ipp_smjob_reader(char *buf, uint32_t len, uint32_t *timeout_ms, void *user_data);
void ipp_smjob_status_event(uint32_t job_id, smjob_status_events_t job_events, void *user_data);
void ipp_request_processing_complete(ipp_request_t *ipp_req);
int32_t ipp_insert_attr_list(ipp_instance_t *ipp_inst, ipp_attribute_t *attr_list, uint32_t attr_cnt, ipp_flags_t flags);


void _get_correct_host_address(char          *host_adrs_str,
                               uint32_t      host_adrs_str_len,
                               ipp_request_t *ipp_req);

#ifdef IPP_TIME_LOG
#define IPP_TIME(ipp_request, tag) \
              DBG_PRINTF(LOG_ERR, "req %d: time %dms " tag "\n", \
              ipp_request->http_hndl, (posix_gettime_ticks()-ipp_request->start_ticks)*10)
#else // !IPP_TIME_LOG
#define IPP_TIME(ipp_request, tag)
#endif // IPP_TIME_LOG

#ifdef __cplusplus
}
#endif
#endif /* __INC_IPP_API_H__ */
