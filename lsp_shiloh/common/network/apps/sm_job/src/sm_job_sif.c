/******************************************************************************
 * Copyright (c) 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 * @file sm_job_sif.c
 *
 * @brief The scan interface between the sm job manager and the ScanMan layer.
 *
 */

#include <stdint.h>             
#include <stdbool.h>            
#include <string.h> // for memset()
#include <stdio.h>
#include "sys_init_api.h"
#include "memAPI.h"
#include "error_types.h"
#include "lassert.h"
#include "debug.h"
#include "logger.h"
#include "net_logger.h"
#include "agRouter.h"
#include "agResourceMgr.h"
#include "scantypes.h"
#include "scanvars.h"
#include "scanmsg.h"
#include "scancap.h"
#include "SJMgr.h"
#include "agMessage.h"
#include "ResourceMap.h"
#include "jpeglib.h"

// local to module
#include "sm_job.h"

//=========================================================================================================================
// Defines
//=========================================================================================================================

#define DBG_PRFX "SMJOBSIF: "             // DBG_PRINTF_DEBUG, DBG_PRINTF_ERR, ...
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | NET_LOGGER_SUBMOD_SM_JOB

//// TODO TEMP release debug
//#define HAVE_DBG_PRINTF
//#define DBG_ERR(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_MSG(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) dbg_printf(DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE2(...) dbg_printf(DBG_PRFX __VA_ARGS__)

#define DBG_ERR(...) DBG_PRINTF(LOG_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_MSG(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
//#define DBG_VERBOSE(...) DBG_PRINTF(LOG_DEBUG, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__)
#define DBG_VERBOSE2(...) DBG_PRINTF(LOG_INFO, DBG_PRFX __VA_ARGS__) // iterative logging of data flow - slow
//#define DBG_VERBOSE2(...) DBG_PRINTF(LOG_NOTICE, DBG_PRFX __VA_ARGS__) // iterative logging of data flow - slow
#define DBG_CMD(...) DBG_PRINTF(LOG_ERR, __VA_ARGS__)


#define SMJOB_SIF_STACK_SIZE   (MAX((6 * 1024), POSIX_MIN_STACK_SIZE))
#define SMJOB_SIF_QUEUE_SIZE   20   // max pending messages supported

#define SMJOB_SIF_DATA_STACK_SIZE   (MAX((6 * 1024), POSIX_MIN_STACK_SIZE))
#define SMJOB_SIF_DATA_QUEUE_SIZE   5   // max pending messages supported


static pthread_t    smjob_sif_thread; 
static uint8_t      smjob_sif_stack[SMJOB_SIF_STACK_SIZE] ALIGN(8);

static pthread_t    smjob_sif_data_thread; 
static uint8_t      smjob_sif_data_stack[SMJOB_SIF_DATA_STACK_SIZE] ALIGN(8); 

static mqd_t        smjob_sif_msg_queue; 

/*
 *  local data message queue
 */

// IMPORTANT: after modifying this table, must also modify g_sif_data_msg_strs
typedef enum
{
    SIF_DATA_MSG_PROCESS_DATA,
    SIF_DATA_MSG_PAGE_END,

    // add new msgs above this line
    SIF_DATA_NUM_MSGS
} sif_data_msg_type_t;

static const char *g_sif_data_msg_strs[SIF_DATA_NUM_MSGS] = 
{
    "SIF_DATA_MSG_PROCESS_DATA",
    "SIF_DATA_MSG_PAGE_END",
};

typedef enum
{
    SIF_DATA_STATE_PROCESS_DATA,

    // add new states above this line
    SIF_DATA_NUM_STATES
} sif_data_state_t;

static const char *g_sif_data_state_strs[SIF_DATA_NUM_STATES] = 
{
    "SIF_DATA_STATE_PROCESS_DATA",
};


// message format
#define SIF_DATA_MQ_MSG_BUF_SIZE   2 //TX_2_ULONG buff byte size must be >= sizeof(ipp_msg_t)
typedef struct
{
    sif_data_msg_type_t type;
    void *data; // message-specific opaque data
} sif_data_msg_t;

// message queue buffer
#define SIF_DATA_MQ_MSG_BUF_BYTES  (SIF_DATA_MQ_MSG_BUF_SIZE*sizeof(uint32_t))
typedef union
{
    sif_data_msg_t msg;
    uint8_t buf[SIF_DATA_MQ_MSG_BUF_BYTES]; // forces ipp_msg_buf_t size into valid threadx queue size option
} sif_data_msg_buf_t;

static mqd_t     sif_data_msg_queue; 


typedef struct
{
    smjob_ctxt_t               *cur_job;
    sif_data_state_t            data_state; // TODO put this in job state
    uint32_t                    padded_bytes_per_row;
    smjob_sif_buf_node_t       *raw_queue;
    smjob_sif_buf_node_t       **raw_queue_tail;
    bool                        raw_queue_emptied; // used to coordinate resending of SIF_DATA_MSG_PROCESS_DATA
    struct jpeg_compress_struct jpeg_cinfo;
    struct jpeg_destination_mgr jpeg_cdest;
    struct jpeg_error_mgr       jpeg_err_mgr;
} smjob_jpeg_ctxt_t;

/*
 * local context
 */
typedef struct 
{
    CURRENT_RESOURCE        *resource;
    struct scanvars         *svars;
    smjob_ctxt_t           *cur_job;
    smjob_jpeg_ctxt_t      *data_ctxt;
    uint32_t                data_bytes_per_row;
    uint32_t                padded_bytes_per_row;
    uint32_t                pixels_per_row;
    uint32_t                total_rows;
    uint8_t                 pixel_depth;
    bool                    abort;
    pthread_mutex_t         mtx;
} smjob_sif_ctxt_t;


static smjob_sif_ctxt_t g_sif_ctxt = {0};


//=========================================================================================================================
// Local function prototypes
//=========================================================================================================================

static void* smjob_sif_state_loop(void* entry_input);
static void* sif_data_process_loop(void* entry_input);
static void smjob_sif_start_job(smjob_sif_ctxt_t *sif_ctxt);
static void smjob_sif_end_job(smjob_sif_ctxt_t *sif_ctxt);
static void smjob_sif_ack_cancel(void *param3, CURRENT_RESOURCE *job_resources);

// jpeg lib callbacks
static void smjob_jpeg_init_destination(j_compress_ptr cinfo);
static void smjob_jpeg_term_destination(j_compress_ptr cinfo);
static boolean smjob_jpeg_empty_output_buffer(j_compress_ptr cinfo);

static smjob_rcode_t sif_data_send_msg_wait_opt(sif_data_msg_type_t type, void *data, uint32_t pthread_wait_opt);


//=========================================================================================================================
// Parameterized Macros
//=========================================================================================================================

#define SMJOB_SIF_LOCK_CONTEXT() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_sif_ctxt.mtx); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define SMJOB_SIF_UNLOCK_CONTEXT() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_sif_ctxt.mtx); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

// send a message
// WARNING: do not call from a non-thread (e.g. timer)
#define sif_data_send_msg(type, sif_ctxt) \
            sif_data_send_msg_wait_opt(type, (void *)sif_ctxt, POSIX_WAIT_FOREVER);


//=========================================================================================================================
// API
//=========================================================================================================================

void smjob_sif_init(void)
{
    DBG_VERBOSE( "==>%s\n", __FUNCTION__ );

    bool    success = false;  
    bool    mtx_created = false;
    bool    data_queue_created = false;
    bool    scan_queue_created = false;
    bool    queue_registered = false;
    bool    resource_registered = false;
    uint32_t    pthread_rcode;
    uint32_t retval;

    // create the sif context mutex
    pthread_rcode = posix_mutex_init(&g_sif_ctxt.mtx);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error creating mutex (pthread_rcode=0x%02x)\n", pthread_rcode);
        goto done;
    }
    mtx_created = true;

    // create data message queue
    ASSERT(sizeof(sif_data_msg_buf_t) == SIF_DATA_MQ_MSG_BUF_BYTES); // validation of msg buf size
    posix_create_message_queue(&sif_data_msg_queue, "/smjob_sif_data", SMJOB_SIF_DATA_QUEUE_SIZE, 
                               sizeof(sif_data_msg_buf_t));
    data_queue_created = true;

    // create the scan message queue
    posix_create_message_queue( &smjob_sif_msg_queue, "/smjob_sif", SMJOB_SIF_QUEUE_SIZE,
                                sizeof(MESSAGE));    
    scan_queue_created = true;

    // register the queue with the system router
    router_register_queue(SMJOB_SCAN_APP_ID, smjob_sif_msg_queue);
    queue_registered = true;

    // register resource with the system resource manager
    retval = rm_register(e_SmJobScanApp, SMJOB_SCAN_APP_ID);
    if(retval != 1)
    {
        DBG_ERR("error registering resource\n");
        goto done;
    }
    resource_registered = true;

    // initialize context
    g_sif_ctxt.resource = NULL;
    g_sif_ctxt.svars = NULL;

    // start the scan data processing task
    pthread_rcode = posix_create_thread(&smjob_sif_data_thread, &sif_data_process_loop, 
                                        (void *)0, "smjob_sif_data",  smjob_sif_data_stack,
                                        SMJOB_SIF_DATA_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error starting thread (pthread_rcode=0x%02x)\n", pthread_rcode);
        goto done;
    }

    // start the scanapp task
    pthread_rcode = posix_create_thread( &smjob_sif_thread, &smjob_sif_state_loop, 
                                         (void *) 0, "smjob_sif", smjob_sif_stack,
                                         SMJOB_SIF_STACK_SIZE, POSIX_THR_PRI_NORMAL);
    if(pthread_rcode != 0)
    {
        DBG_ERR("error starting thread (pthread_rcode=0x%02x)\n", pthread_rcode);
        goto done;
    }

    success = true;

done:
    if(!success)
    {
        // cleanup 
        if(mtx_created)
        {
            pthread_mutex_destroy(&g_sif_ctxt.mtx);
        }
        if(resource_registered)
        {
            // TODO anything to do here?
        }
        DBG_ERR("init failed!\n");
    }

    DBG_VERBOSE( "<==%s\n", __FUNCTION__ );
    return;
}

smjob_rcode_t smjob_sif_start_scan( smjob_ctxt_t *job )
{
    DBG_VERBOSE( "==>%s\n", __FUNCTION__);

    MESSAGE      msg;
    error_type_t err;
    STARTRECIPE* StartIt;

    SMJOB_SIF_LOCK_CONTEXT();
    if(g_sif_ctxt.cur_job)
    {
        // scan job in progress

        // TODO queue the job
        SMJOB_SIF_UNLOCK_CONTEXT();
        DBG_VERBOSE( "%s: scan job already in progress!\n", __func__ );
        return SMJOB_FAIL;
    }
    // initialize scan job context
    g_sif_ctxt.cur_job = job;
    g_sif_ctxt.data_ctxt = 0;
    g_sif_ctxt.data_bytes_per_row = 0;
    g_sif_ctxt.padded_bytes_per_row = 0;
    g_sif_ctxt.pixels_per_row = 0;
    g_sif_ctxt.total_rows = 0;
    g_sif_ctxt.abort = false;
    SMJOB_SIF_UNLOCK_CONTEXT();

    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
    ASSERT(StartIt != NULL);
    memset( StartIt, 0, sizeof(STARTRECIPE) );

    // We have a connection, get the resources and start.
    msg.msgType  = MSG_STARTRECIPE;
    StartIt->Job = e_SmJobScanApp;
    StartIt->AdditionalResource = 0;
    StartIt->Pipe = 0;
    StartIt->Wait = e_WaitForever;
    msg.param3 = StartIt;       // freed by sjm
  
    // Send a message to the System Job Manager telling it to start our job
    err = SYMsgSend(SJMID, &msg);
    XASSERT( err==OK, err );

    DBG_VERBOSE( "<==%s\n", __FUNCTION__ );
    return SMJOB_OK;
}

smjob_rcode_t smjob_sif_cancel_scan(smjob_ctxt_t *job)
{
    ASSERT(job);
    smjob_sif_buf_node_t *node;

    DBG_VERBOSE( "==>%s\n", __FUNCTION__);

    SMJOB_SIF_LOCK_CONTEXT();
    g_sif_ctxt.abort = true;
    if(g_sif_ctxt.data_ctxt)
    {
        ASSERT(g_sif_ctxt.data_ctxt->cur_job);
        XASSERT(g_sif_ctxt.data_ctxt->cur_job == job, (unsigned int)g_sif_ctxt.data_ctxt->cur_job);

        sif_data_send_msg(SIF_DATA_MSG_PAGE_END, g_sif_ctxt.data_ctxt);
        g_sif_ctxt.data_ctxt = NULL; // will be freed by smjob data processor
    }
    SMJOB_SIF_UNLOCK_CONTEXT();

    smjob_lock_ctxt();

    // signal end of multi-page job data
    // must do this before deleting tx_flags which may unblock user read thread
    if(g_sif_ctxt.cur_job)
    {
        g_sif_ctxt.cur_job->data_done = true;
        g_sif_ctxt.cur_job->data_abort = true; // only partial data was delivered
    }

    // free any outstanding processed bufs for current page (e.g. jpeg compressed data)
    while(job->data_queue)
    {
        node = job->data_queue;
        job->data_queue = job->data_queue->next;
        if(!job->data_queue)
        {
            // queue is now empty
            ASSERT(!job->data_queue_emptied);
            job->data_queue_tail = &job->data_queue;
            job->data_queue_emptied = true;
        }

        ASSERT(node);
        memFree(node->buf);
        memFree(node);
    }

    if(!SMJOB_IS_COMPLETED(job))
    {
        job->state = SMJOB_STATE_CANCELED;
        job->reasons[0] = SMJOB_STATE_REASON_JOB_CANCELED_BY_USER;
        job->num_reasons = 1;

        // this will unblock any user thread waiting for data
        // unblock in case waiting for data when cancel request arrives
        uint32_t pthread_rcode;
        pthread_rcode = sem_post(&job->semaphore);
        XASSERT(pthread_rcode == 0, pthread_rcode);
        pthread_rcode = sem_destroy(&job->semaphore);
        XASSERT(pthread_rcode == 0, pthread_rcode);
        UNUSED_VAR(pthread_rcode);

        smjob_unlock_ctxt();

        if( job->status_func && (job->event_mask & SMJOB_STATUS_EVENT_STATE) )
        {
            job->status_func(job->job_id, SMJOB_STATUS_EVENT_STATE, job->user_data);
        }
    }
    else
    {
        smjob_unlock_ctxt();
    }

    DBG_VERBOSE( "<==%s\n", __FUNCTION__ );
    return SMJOB_OK;
}


//=========================================================================================================================
// sif_data_process_loop
//=========================================================================================================================

static void* sif_data_process_loop(void* entry_input)
{
    uint32_t pthread_rcode;
    sif_data_msg_buf_t msg_buf;
    sif_data_msg_t *msg;
    smjob_jpeg_ctxt_t *data_ctxt;

    DBG_MSG("starting scan data loop\n");

    while(1)
    {
        data_ctxt = NULL;
        pthread_rcode = posix_wait_for_message(sif_data_msg_queue, (char *)&msg_buf,
                                               sizeof(sif_data_msg_buf_t), POSIX_WAIT_FOREVER);
        if(pthread_rcode != 0)
        {
            DBG_ERR("error reading sif_data_msg_queue (pthread_rcode=0x%02x)\n", pthread_rcode);
            break;
        }
        msg = &msg_buf.msg;
        ASSERT(msg->type < SIF_DATA_NUM_MSGS);

        data_ctxt = (smjob_jpeg_ctxt_t *)msg->data;
        if(data_ctxt)
        {
            if(msg->type == SIF_DATA_MSG_PROCESS_DATA)
            {
                // this can create too much output for scan (asserts on unserviced PIC interrupt)
                DBG_VERBOSE2("%08x got msg %s in state %s\n", data_ctxt, 
                            g_sif_data_msg_strs[msg->type], g_sif_data_state_strs[data_ctxt->data_state]);
            }
            else
            {
                DBG_VERBOSE("%08x got msg %s in state %s\n", data_ctxt, 
                            g_sif_data_msg_strs[msg->type], g_sif_data_state_strs[data_ctxt->data_state]);
            }
        }
        else
        {
            DBG_VERBOSE("got stateless msg %s\n", g_sif_data_msg_strs[msg->type]);
            XASSERT(0, msg->type); // currently no stateless messages
        }

        /*
         *  process stateless messages
         */

//        switch(msg->type)
//        {
//        case SIF_DATA_MSG_
//            break;
//
//        default:
//            // ignore other messages
//            break;
//        }

        /*
         *  process stateful messages
         */

        if(!data_ctxt)
        {
            // message is not stateful - ignore
            continue;
        }

        ASSERT(data_ctxt->data_state < SIF_DATA_NUM_STATES);
        switch(data_ctxt->data_state)
        {
            /***************************************************
             * SIF_DATA_STATE_PROCESS_DATA
             ***************************************************/
            case SIF_DATA_STATE_PROCESS_DATA:

                switch(msg->type)
                {
                    case SIF_DATA_MSG_PROCESS_DATA:
                    {
                        // perform jpeg compression

                        JSAMPROW jpeg_row_ptr;
                        JDIMENSION written;
                        uint32_t rows;
                        uint32_t i;
                        smjob_sif_buf_node_t *node;
                        bool done = false;

                        ASSERT(data_ctxt->raw_queue_emptied == false);
                        ASSERT(data_ctxt->raw_queue);
                        while(!done)
                        {
                            SMJOB_SIF_LOCK_CONTEXT();
                            node = data_ctxt->raw_queue;
                            data_ctxt->raw_queue = data_ctxt->raw_queue->next;
                            if(!data_ctxt->raw_queue)
                            {
                                // queue is now empty
                                data_ctxt->raw_queue_tail = &data_ctxt->raw_queue;
                                data_ctxt->raw_queue_emptied = true;
                                done = true;
                            }
                            SMJOB_SIF_UNLOCK_CONTEXT();

                            ASSERT(node);
                            ASSERT(node->buf);
                            XASSERT(node->len % data_ctxt->padded_bytes_per_row == 0, 
                                    node->len % data_ctxt->padded_bytes_per_row); // verify aligned on row boundary
                            rows = node->len / data_ctxt->padded_bytes_per_row;
                            for(i = 0; i < rows; i++)
                            {
                                // must write one row at a time to strip pad bytes
                                jpeg_row_ptr = (JSAMPROW)(node->buf + (i * data_ctxt->padded_bytes_per_row));
                                written = jpeg_write_scanlines(&data_ctxt->jpeg_cinfo, &jpeg_row_ptr, 1);
                                XASSERT(written == 1, written);
                                UNUSED_VAR(written);
                            }

                            // jpeg compressor has finished compressing the data from this buffer, and has allocated
                            // a new buffer into which the compressed data has been stored
                            memFree(node->buf);
                            memFree(node);
                        }
                        break;
                    }

                    case SIF_DATA_MSG_PAGE_END:
                    {
                        if(data_ctxt->jpeg_cinfo.next_scanline < data_ctxt->jpeg_cinfo.image_height)
                        {
                            DBG_VERBOSE("received too little data, modifying jpeg height\n" );
                            data_ctxt->jpeg_cinfo.image_height = data_ctxt->jpeg_cinfo.next_scanline;
                        }
                        jpeg_finish_compress(&data_ctxt->jpeg_cinfo);
                        jpeg_destroy_compress(&data_ctxt->jpeg_cinfo);

                        SMJOB_SIF_LOCK_CONTEXT();

                        // dispose of any extra data in the raw queue (e.g. in case of cancel)
                        smjob_sif_buf_node_t *node;
                        while(data_ctxt->raw_queue)
                        {
                            ASSERT(!data_ctxt->raw_queue_emptied);
                            node = data_ctxt->raw_queue;
                            data_ctxt->raw_queue = data_ctxt->raw_queue->next;

                            memFree(node->buf);
                            memFree(node);
                        }
                        // raw queue is now empty
                        data_ctxt->raw_queue_tail = &g_sif_ctxt.data_ctxt->raw_queue;
                        data_ctxt->raw_queue_emptied = true;

                        // we are done compressing current page to jpeg and will receive no new
                        // messages for this page
                        MEM_FREE_AND_NULL(data_ctxt);

                            SMJOB_SIF_UNLOCK_CONTEXT();

                        break;
                    }

                    default:
                        // ignore other messages
                        DBG_VERBOSE("%08x ignoring msg %s!\n", data_ctxt, g_sif_data_msg_strs[msg->type]);
                        break;
                }
                break;

            default:
                // unknown state - ignore
                XASSERT(0, data_ctxt->data_state);
                break;
        }
    }

    DBG_MSG("exiting\n");
    return 0;
}

static void* smjob_sif_state_loop(void* entry_input)
{
    MESSAGE msg;
    uint32_t pthread_rcode;

    //wait for system init to finish
    SysWaitForInit();

    while (1) {

        /* get a message */
        pthread_rcode = posix_wait_for_message(smjob_sif_msg_queue, (char *)&msg, 
                                               sizeof(MESSAGE),POSIX_WAIT_FOREVER); 
        if(pthread_rcode != 0)
        {
            DBG_ERR("error reading smjob_sif_msg_queue (pthread_rcode=0x%02x)\n", pthread_rcode);
            break;
        }

        switch (msg.msgType)
        {
            case MSG_RESOURCES:
            {
                DBG_VERBOSE("MSG_RESOURCES %08x\n", msg.param3);

                // system job manager should never send us overlapping resource requests
                XASSERT(g_sif_ctxt.resource == NULL, (uint32_t)g_sif_ctxt.resource);

                // note that we are responsible for freeing svars following each scan
                XASSERT( g_sif_ctxt.svars == NULL, g_sif_ctxt.svars->id );

                g_sif_ctxt.resource = (CURRENT_RESOURCE *)msg.param3;

                /* must wait for all other needed resources to be notified by the job
                 * manager, after which we will receive MSG_ACKRECIPE
                 */
                break; 
            }

            case MSG_ACKRECIPE:
            {
                ASSERT(g_sif_ctxt.resource);
                DBG_VERBOSE("MSG_ACKRECIPE %08x\n", g_sif_ctxt.resource);

                XASSERT( g_sif_ctxt.svars == NULL, g_sif_ctxt.svars->id );

                g_sif_ctxt.pixel_depth = 0;
                ASSERT(g_sif_ctxt.cur_job);
                if (g_sif_ctxt.cur_job->is_color)
                {
                    g_sif_ctxt.svars = scanplat_sv_color_scan_new();
                    ASSERT(g_sif_ctxt.svars); // TODO handle NULL case
                    scanvar_set_pieout(g_sif_ctxt.svars, PIE_PIXEL_RGB);
                    g_sif_ctxt.pixel_depth = 3;
                }
                else // monochrome
                {
                    g_sif_ctxt.svars = scanplat_sv_mono_scan_new();
                    scanvar_set_pieout(g_sif_ctxt.svars, PIE_PIXEL_MONO);
                    g_sif_ctxt.pixel_depth = 1;
                }

                struct scan_platform_capabilities scan_capabilities;
                scancap_get_capabilities( &scan_capabilities );
                if( !scan_capabilities.has_adf )
                {
                    DBG_MSG("no adf - scan flatbed\n");
                    scanvar_set_document_source( g_sif_ctxt.svars, SCAN_DOCUMENT_SOURCE_FLATBED);
                }
                else
                { 
// rdj TODO: use ticket.doc_processing.type.scan.input_source
//                    switch( docsrc )
//                    {
//
//                        case SCAN_JOB_SETTINGS_DOC_SRC_FLATBED:
//                            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_FLATBED);
//                            break;
//
//                        case SCAN_JOB_SETTINGS_DOC_SRC_ADF:
//                            scanvar_set_document_source(g_scanvars, SCAN_DOCUMENT_SOURCE_ADF);
//                            break;
//
//                        default:
                            // auto-detect source if not specified
                            bool adf_sensor_paper_present();
                            if( adf_sensor_paper_present() ) {
                                scanvar_set_document_source(g_sif_ctxt.svars, SCAN_DOCUMENT_SOURCE_ADF);
                                DBG_MSG("scan adf\n");
                            }
                            else {
                                scanvar_set_document_source(g_sif_ctxt.svars, SCAN_DOCUMENT_SOURCE_FLATBED);
                                DBG_MSG("scan flatbed\n");
                            }
//                            break;
//                    }

                }

                scanvar_set_dpi( g_sif_ctxt.svars, 300 );

                // tell upstream to start the job
                smjob_sif_start_job( &g_sif_ctxt );

                break;
            }

            case MSG_SCAN_SIZE:
            {
                ASSERT(g_sif_ctxt.resource);
                DBG_VERBOSE("MSG_SCAN_SIZE %08x\n", g_sif_ctxt.resource);

                uint32_t pixels_per_row_padded, pixels_per_row, total_rows, bits_per_pixel;
                // note total_rows is not accurate for ADF jobs
                msg_scan_size_decode(&msg, &pixels_per_row_padded, &pixels_per_row, &total_rows, &bits_per_pixel);
                XASSERT((pixels_per_row * bits_per_pixel) % 8 == 0, pixels_per_row * bits_per_pixel); // check no remainder
                XASSERT((pixels_per_row_padded * bits_per_pixel) % 8 == 0, pixels_per_row_padded * bits_per_pixel); // check no remainder
                g_sif_ctxt.data_bytes_per_row = (pixels_per_row * bits_per_pixel) / 8;
                g_sif_ctxt.padded_bytes_per_row = ((pixels_per_row_padded * bits_per_pixel) / 8);
                g_sif_ctxt.pixels_per_row = pixels_per_row;
                g_sif_ctxt.total_rows = total_rows; // WARNING: this value is not accurate for ADF jobs
                DBG_VERBOSE("pprp=%d ppr=%d bpp=%d dbpr=%d pbpr=%d tr=%d\n", 
                            pixels_per_row_padded, pixels_per_row, bits_per_pixel, 
                            g_sif_ctxt.data_bytes_per_row, g_sif_ctxt.padded_bytes_per_row,
                            total_rows);
                break;
            }

            case MSG_SCAN_PAGESTART:
            {
                ASSERT(g_sif_ctxt.resource);
                DBG_VERBOSE("MSG_SCAN_PAGESTART %08x\n", g_sif_ctxt.resource);

                // TODO add support for overlapping pages and then remove this assert
                XASSERT(!g_sif_ctxt.data_ctxt, (unsigned long int)g_sif_ctxt.data_ctxt);

                SMJOB_SIF_LOCK_CONTEXT();

                if(g_sif_ctxt.abort)
                { 
                    // do not create data_ctxt or call jpeg compression startup routines
                    SMJOB_SIF_UNLOCK_CONTEXT();
                    break;
                }

                g_sif_ctxt.data_ctxt = (smjob_jpeg_ctxt_t *)MEM_MALLOC(sizeof(smjob_jpeg_ctxt_t));
                if(!g_sif_ctxt.data_ctxt)
                {
                    // TODO not sure what to do here
                    ASSERT(0);
                    SMJOB_SIF_UNLOCK_CONTEXT();
                    break;
                }
                g_sif_ctxt.data_ctxt->raw_queue = NULL;
                g_sif_ctxt.data_ctxt->raw_queue_tail = &g_sif_ctxt.data_ctxt->raw_queue;
                g_sif_ctxt.data_ctxt->raw_queue_emptied = true;
                g_sif_ctxt.data_ctxt->data_state = SIF_DATA_STATE_PROCESS_DATA;
                g_sif_ctxt.data_ctxt->cur_job = g_sif_ctxt.cur_job;
                ASSERT(g_sif_ctxt.data_bytes_per_row);
                g_sif_ctxt.data_ctxt->padded_bytes_per_row = g_sif_ctxt.data_bytes_per_row;
                SMJOB_SIF_UNLOCK_CONTEXT();

                g_sif_ctxt.data_ctxt->jpeg_cinfo.err = jpeg_std_error(&g_sif_ctxt.data_ctxt->jpeg_err_mgr);

                /* Now we can initialize the JPEG compression object. */
                jpeg_create_compress(&g_sif_ctxt.data_ctxt->jpeg_cinfo);

                // setup some info on the data that is coming.
                g_sif_ctxt.data_ctxt->jpeg_cinfo.image_width = g_sif_ctxt.pixels_per_row;

                // ***TODO***: this value is not accurate for ADF jobs -- in the case of ADF we must store
                // the entire page locally in mem before compressing, so we can determine correct height
                g_sif_ctxt.data_ctxt->jpeg_cinfo.image_height = g_sif_ctxt.total_rows;

                ASSERT(g_sif_ctxt.cur_job);
                g_sif_ctxt.data_ctxt->jpeg_cinfo.in_color_space = JCS_GRAYSCALE;   /* colorspace of input image */
                if (g_sif_ctxt.cur_job->is_color)
                {
                    g_sif_ctxt.data_ctxt->jpeg_cinfo.in_color_space = JCS_RGB;     /* colorspace of input image */
                }
                ASSERT(g_sif_ctxt.pixel_depth);
                g_sif_ctxt.data_ctxt->jpeg_cinfo.input_components = g_sif_ctxt.pixel_depth;

                // Set up the destination callbacks for the jpeg
                jpeg_set_defaults(&g_sif_ctxt.data_ctxt->jpeg_cinfo);
                g_sif_ctxt.data_ctxt->jpeg_cinfo.dest = &g_sif_ctxt.data_ctxt->jpeg_cdest;      

                //defining our own memory management through the following functions
                g_sif_ctxt.data_ctxt->jpeg_cdest.init_destination = smjob_jpeg_init_destination;
                g_sif_ctxt.data_ctxt->jpeg_cdest.empty_output_buffer = smjob_jpeg_empty_output_buffer;
                g_sif_ctxt.data_ctxt->jpeg_cdest.term_destination = smjob_jpeg_term_destination;

                g_sif_ctxt.data_ctxt->jpeg_cinfo.density_unit = 1; // Inches
                g_sif_ctxt.data_ctxt->jpeg_cinfo.X_density = 300;  // DPI
                g_sif_ctxt.data_ctxt->jpeg_cinfo.Y_density = 300;  // DPI

                // quality is a number from 0 to 100 that specifies how much to compress.
                // 0 = highest compress, lowest quality, 100=lowest compress, highest quality.uint32_t CurrentLines = 0;
                jpeg_set_quality(&g_sif_ctxt.data_ctxt->jpeg_cinfo, 80, TRUE /* limit to baseline-JPEG values */);//FROM true to FALSE

                jpeg_start_compress(&g_sif_ctxt.data_ctxt->jpeg_cinfo, TRUE);

                break;
            }

            case MSG_SCAN_PLANEDATA:
            {
                ASSERT(g_sif_ctxt.resource);

                DBG_VERBOSE2("MSG_SCAN_PLANEDATA %08x\n", g_sif_ctxt.resource);

                uint16_t num_rows;
                scan_data_type dtype;
                uint8_t *data;
                bool last_buffer;
                msg_scan_planedata_decode(&msg, &num_rows, &dtype, &data, &last_buffer);

                DBG_VERBOSE2("planedata rows=%d dtype=%d last=%d data=%p\n", num_rows, dtype, last_buffer, data);

                SMJOB_SIF_LOCK_CONTEXT();

                if(g_sif_ctxt.abort)
                { 
                    // just dump the data
                    memFree(data);
                    SMJOB_SIF_UNLOCK_CONTEXT();
                    break;
                }

                smjob_sif_buf_node_t *buf_node;
                buf_node = (smjob_sif_buf_node_t *)MEM_MALLOC(sizeof(smjob_sif_buf_node_t));
                if(!buf_node)
                {
                    // TODO not sure what to do here
                    ASSERT(0);
                    memFree(data);
                    SMJOB_SIF_UNLOCK_CONTEXT();
                    break;
                }

                buf_node->next = NULL;
                buf_node->buf = data;
                buf_node->len = g_sif_ctxt.padded_bytes_per_row * num_rows;

                if(g_sif_ctxt.data_ctxt)
                {
                    *g_sif_ctxt.data_ctxt->raw_queue_tail = buf_node;
                    g_sif_ctxt.data_ctxt->raw_queue_tail = &buf_node->next;

                    if(g_sif_ctxt.data_ctxt->raw_queue_emptied)
                    {
                        // after initial msg, resend only after queue is emptied by data process loop
                        g_sif_ctxt.data_ctxt->raw_queue_emptied = false;
                        sif_data_send_msg(SIF_DATA_MSG_PROCESS_DATA, g_sif_ctxt.data_ctxt);
                    }
                }
                SMJOB_SIF_UNLOCK_CONTEXT();

                break;
            }

            case MSG_SCAN_PAGEEND:
            {
                ASSERT(g_sif_ctxt.resource);
                DBG_VERBOSE("MSG_SCAN_PAGEEND %08x\n", g_sif_ctxt.resource);

                // notify the smjob data processor that we are done receiving data for this page
                SMJOB_SIF_LOCK_CONTEXT();
                if(g_sif_ctxt.data_ctxt)
                {
                    sif_data_send_msg(SIF_DATA_MSG_PAGE_END, g_sif_ctxt.data_ctxt);
                    g_sif_ctxt.data_ctxt = NULL; // will be freed by smjob data processor
                }
                SMJOB_SIF_UNLOCK_CONTEXT();

                break;
            }

            case MSG_SCAN_JOBEND:
            {
                DBG_VERBOSE("MSG_SCAN_JOBEND %08x\n", g_sif_ctxt.resource);

                SMJOB_SIF_LOCK_CONTEXT();

                ASSERT(g_sif_ctxt.resource);
                ASSERT(g_sif_ctxt.cur_job);
                smjob_ctxt_t *job = g_sif_ctxt.cur_job;

                smjob_lock_ctxt();

                // signal end of multi-page job data
                // must do this before deleting tx_flags which may unblock user read thread
                XASSERT(!g_sif_ctxt.data_ctxt, (int)g_sif_ctxt.data_ctxt); // confirm last page was completed
                g_sif_ctxt.cur_job->data_done = true;

                if(!SMJOB_IS_COMPLETED(job))
                {
                    job->state = SMJOB_STATE_COMPLETED;
                    job->reasons[0] = SMJOB_STATE_REASON_UNKNOWN;
                    job->num_reasons = 0;

                    // this will unblock any user thread waiting for data
//                    threadx_rcode = tx_event_flags_delete(&job->flgs);
//                    XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

                    // this will unblock any user thread waiting for data
                    // unblock in case waiting for data when cancel request arrives
                    uint32_t pthread_rcode;
                    pthread_rcode = sem_post(&job->semaphore);
                    XASSERT(pthread_rcode == 0, pthread_rcode);
                    pthread_rcode = sem_destroy(&job->semaphore);
                    XASSERT(pthread_rcode == 0, pthread_rcode);
                    UNUSED_VAR(pthread_rcode);

                    smjob_unlock_ctxt();

                    if( job->status_func && (job->event_mask & SMJOB_STATUS_EVENT_STATE) )
                    {
                        job->status_func(job->job_id, SMJOB_STATUS_EVENT_STATE, job->user_data);
                    }
                }
                else
                {
                    smjob_unlock_ctxt();
                }

                SMJOB_SIF_UNLOCK_CONTEXT();

                // We are passed a pointer to the scanvar we originally sent
                ASSERT(msg.param3 != NULL );
                XASSERT(msg.param3 == g_sif_ctxt.svars, (uint32_t)msg.param3);
                XASSERT(g_sif_ctxt.svars->cookie == SCANVAR_COOKIE, g_sif_ctxt.svars->cookie);

                // Tell the source that we are done talking to him
                MESSAGE      out_msg;
                error_type_t err;
                memset( &out_msg, 0, sizeof(msg) );
                out_msg.msgType = MSG_SCAN_JOBEND;
                out_msg.param3  = g_sif_ctxt.resource;
                out_msg.param2  = SMJOB_SCAN_APP_ID;
                err = SYMsgSend(g_sif_ctxt.resource->Source, &out_msg); // tell the scanner to finish.
                XASSERT( err==OK, err );
                memset( &msg, 0, sizeof(out_msg) );
                out_msg.msgType = MSG_FREERECIPE;
                out_msg.param1 = 0;
                out_msg.param2 = 0;
                out_msg.param3 = g_sif_ctxt.resource;
                err = SYMsgSend( SJMID, &out_msg );
                XASSERT( err==OK, err );

                smjob_sif_end_job(&g_sif_ctxt);

                break;
            }

            case MSG_CANCELJOB:
            {
                // job canceled at the device

                ASSERT(g_sif_ctxt.resource);
                DBG_VERBOSE("MSG_CANCELJOB %08x\n", g_sif_ctxt.resource);

                /* davep 18-Oct-2007 ; WTF is param3? SJM seems to want it
                 * on a cancel
                 */
                smjob_sif_ack_cancel(msg.param3, g_sif_ctxt.resource);

                SMJOB_SIF_LOCK_CONTEXT();

                smjob_ctxt_t *job = g_sif_ctxt.cur_job;
                ASSERT(job);

                // signal end of current page data
                if(g_sif_ctxt.data_ctxt)
                {
                    sif_data_send_msg(SIF_DATA_MSG_PAGE_END, g_sif_ctxt.data_ctxt);
                    g_sif_ctxt.data_ctxt = NULL; // will be freed by smjob data processor
                }

                smjob_lock_ctxt();

                // signal end of multi-page job data
                // must do this before deleting tx_flags which may unblock user read thread
                if(g_sif_ctxt.cur_job)
                {
                    g_sif_ctxt.cur_job->data_done = true;
                    g_sif_ctxt.cur_job->data_abort = true; // only partial data was delivered
                }

                if(!SMJOB_IS_COMPLETED(job))
                {
                    job->state = SMJOB_STATE_CANCELED;
                    job->reasons[0] = SMJOB_STATE_REASON_JOB_CANCELED_AT_DEVICE;
                    job->num_reasons = 1;

                    // this will unblock any user thread waiting for data
                    //threadx_rcode = tx_event_flags_delete(&job->flgs);
                    //XASSERT(threadx_rcode == TX_SUCCESS, threadx_rcode);

                    // this will unblock any user thread waiting for data
                    // unblock in case waiting for data when cancel request arrives
                    uint32_t pthread_rcode;
                    pthread_rcode = sem_post(&job->semaphore);
                    XASSERT(pthread_rcode == 0, pthread_rcode);
                    pthread_rcode = sem_destroy(&job->semaphore);
                    XASSERT(pthread_rcode == 0, pthread_rcode);
                    UNUSED_VAR(pthread_rcode);

                    smjob_unlock_ctxt();

                    if( job->status_func && (job->event_mask & SMJOB_STATUS_EVENT_STATE) )
                    {
                        job->status_func(job->job_id, SMJOB_STATUS_EVENT_STATE, job->user_data);
                    }
                }
                else
                {
                    smjob_unlock_ctxt();
                }

                SMJOB_SIF_UNLOCK_CONTEXT();

                smjob_sif_end_job(&g_sif_ctxt);

                break;
            }

            default:
                XASSERT( 0, msg.msgType );
                break;
        }
    }

    DBG_MSG("exiting\n");
    return 0;
}


//=========================================================================================================================
// local functions
//=========================================================================================================================

static void smjob_sif_start_job(smjob_sif_ctxt_t *sif_ctxt)
{
    DBG_VERBOSE( "==>%s\n", __FUNCTION__ );

    MESSAGE msg;
    error_type_t err;

    ASSERT(sif_ctxt);
    smjob_ctxt_t *job = sif_ctxt->cur_job;

    XASSERT(job, (unsigned long int)sif_ctxt);
    smjob_lock_ctxt();
    job->state = SMJOB_STATE_PROCESSING;
    job->reasons[0] = SMJOB_STATE_REASON_UNKNOWN;
    job->num_reasons = 0;
    smjob_unlock_ctxt();
    if( job->status_func && (job->event_mask & SMJOB_STATUS_EVENT_STATE) )
    {
        job->status_func(job->job_id, SMJOB_STATUS_EVENT_STATE, job->user_data);
    }

    // send the startjob upstream; we pass control of the scanvar to the scanner itself
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_SCAN_JOBSTART;
    msg.param2 = SMJOB_SCAN_APP_ID;
    msg.param3 = sif_ctxt->svars;
    err = SYMsgSend( sif_ctxt->resource->Source, &msg );
    XASSERT( err==OK, err );

    DBG_VERBOSE( "<==%s\n", __FUNCTION__ );
    return;
}

static void smjob_sif_end_job(smjob_sif_ctxt_t *sif_ctxt)
{
    DBG_VERBOSE( "==>%s\n", __FUNCTION__ );

    /* free the configuration memory */
    if( sif_ctxt->svars != NULL) {
        scanvar_delete( &sif_ctxt->svars );
        XASSERT( sif_ctxt->svars==NULL, (uint32_t)sif_ctxt->svars );
    }

    ASSERT(sif_ctxt);

    SMJOB_SIF_LOCK_CONTEXT();
    sif_ctxt->cur_job = NULL;
    sif_ctxt->resource = NULL;
    // rdj TODO check for queued jobs here
    SMJOB_SIF_UNLOCK_CONTEXT();

    DBG_VERBOSE( "<==%s\n", __FUNCTION__ );
    return;
}

// acknowledge the cancel request
static void smjob_sif_ack_cancel(void *param3, CURRENT_RESOURCE *job_resources)
{
    MESSAGE msg;
    error_type_t err;

    /* davep 18-Oct-2007 ; WTF is param3? SJM seems to want it */
    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_ACK;
    msg.param2 = SMJOB_SCAN_APP_ID;
    msg.param3 = param3;
    err = SYMsgSend( SJMID, &msg);
    XASSERT( err==OK, err );

    memset( &msg, 0, sizeof(msg) );
    msg.msgType = MSG_FREERECIPE;
    msg.param1 = 0;
    msg.param2 = 0;
    msg.param3 = job_resources;
    err = SYMsgSend( SJMID, &msg );
    XASSERT( err==OK, err );

    return;
}

/**
 * This is called by the sw jpeg compressor to allocate a buffer to be used in 
 * compression. 
 */
#define SIF_JPEG_OUT_BUF_SIZE (8 * 1024)
static uint8_t *jpeg_cur_buf = NULL;
static void smjob_jpeg_init_destination(j_compress_ptr cinfo)
{
    cinfo->dest->next_output_byte = MEM_MALLOC(SIF_JPEG_OUT_BUF_SIZE);
    ASSERT(cinfo->dest->next_output_byte);
    jpeg_cur_buf = cinfo->dest->next_output_byte;
    cinfo->dest->free_in_buffer = SIF_JPEG_OUT_BUF_SIZE;
}

/**
 * This is called by the sw jpeg compressor when the compression is complete.
 * So it should send the data, and shut down everything else.
 */
static void smjob_jpeg_term_destination(j_compress_ptr cinfo)
{
    uint32_t pthread_rcode;
    bool data_notify = false;
    smjob_sif_buf_node_t *buf_node = NULL;
    smjob_sif_buf_node_t *null_buf_node = NULL;
    smjob_jpeg_ctxt_t *data_ctxt;
    smjob_ctxt_t *job;

    // recover context
    data_ctxt = SMJOB_CONTAINING_RECORD(cinfo, smjob_jpeg_ctxt_t, jpeg_cinfo);

    ASSERT(data_ctxt);
    ASSERT(data_ctxt->cur_job);
    job = data_ctxt->cur_job;

    buf_node = (smjob_sif_buf_node_t *)MEM_MALLOC(sizeof(smjob_sif_buf_node_t));
    if(!buf_node)
    {
        // TODO not sure what to do here
        ASSERT(0);
        goto error;
    }

    // add NULL buf to indicate EOF
    null_buf_node = (smjob_sif_buf_node_t *)MEM_MALLOC(sizeof(smjob_sif_buf_node_t));
    if(!null_buf_node)
    {
        // TODO not sure what to do here
        ASSERT(0);
        goto error;
    }

    buf_node->next = NULL;
    buf_node->buf = jpeg_cur_buf; // buffer will be freed by application
    buf_node->len = SIF_JPEG_OUT_BUF_SIZE - cinfo->dest->free_in_buffer;

    null_buf_node->next = NULL;
    null_buf_node->buf = NULL;
    null_buf_node->len = 0;

    smjob_lock_ctxt();
    *job->data_queue_tail = buf_node;
    job->data_queue_tail = &buf_node->next;

    *job->data_queue_tail = null_buf_node;
    job->data_queue_tail = &null_buf_node->next;

    if(job->data_queue_emptied)
    {
        job->data_queue_emptied = false;

        pthread_rcode = sem_post(&job->semaphore);
        XASSERT(pthread_rcode == 0, pthread_rcode);
        UNUSED_VAR(pthread_rcode);

        data_notify = true;
    }
    smjob_unlock_ctxt();

    // TODO implement support for smjob_send_func_t callback for asynchronous delivery of
    // scan data -- currently the smjob_retrieve_image() API is the only way to acquire scan data 
    if( data_notify && job->status_func
        && (job->event_mask & SMJOB_STATUS_EVENT_SCAN_DATA) )
    {
            job->status_func(job->job_id, SMJOB_STATUS_EVENT_SCAN_DATA, job->user_data);
    }

    return;

error:
    MEM_FREE_AND_NULL(jpeg_cur_buf);
    if(buf_node)
    {
        MEM_FREE_AND_NULL(buf_node);
    }
    if(null_buf_node)
    {
        MEM_FREE_AND_NULL(null_buf_node);
    }
    return;
}

/**
 * This is called by the sw jpeg compressor when it has filled one of 
 * our buffers.  Send the buffer to the host and call the init to get the next one.
 */
static boolean smjob_jpeg_empty_output_buffer(j_compress_ptr cinfo)
{
    bool data_notify;
    uint32_t pthread_rcode;
    smjob_jpeg_ctxt_t *data_ctxt;
    smjob_ctxt_t *job;

    // recover context
    data_ctxt = SMJOB_CONTAINING_RECORD(cinfo, smjob_jpeg_ctxt_t, jpeg_cinfo);

    ASSERT(data_ctxt);
    ASSERT(data_ctxt->cur_job);
    job = data_ctxt->cur_job;

    smjob_sif_buf_node_t *buf_node;
    buf_node = (smjob_sif_buf_node_t *)MEM_MALLOC(sizeof(smjob_sif_buf_node_t));
    if(!buf_node)
    {
        // TODO not sure what to do here
        ASSERT(0);
        goto error;
    }

    buf_node->next = NULL;
    buf_node->buf = jpeg_cur_buf; // buffer will be freed by application
    buf_node->len = SIF_JPEG_OUT_BUF_SIZE;

    smjob_lock_ctxt();
    *job->data_queue_tail = buf_node;
    job->data_queue_tail = &buf_node->next;

    if(job->data_queue_emptied)
    {
        job->data_queue_emptied = false;

        pthread_rcode = sem_post(&job->semaphore);
        XASSERT(pthread_rcode == 0, pthread_rcode);
        UNUSED_VAR(pthread_rcode);

        data_notify = true;
    }
    smjob_unlock_ctxt();

    // TODO implement support for smjob_send_func_t callback for asynchronous delivery of
    // scan data -- currently the smjob_retrieve_image() API is the only way to acquire scan data 
    if( data_notify && job->status_func
        && (job->event_mask & SMJOB_STATUS_EVENT_SCAN_DATA) )
    {
            job->status_func(job->job_id, SMJOB_STATUS_EVENT_SCAN_DATA, job->user_data);
    }

    // reallocate memory for buffer
    smjob_jpeg_init_destination(cinfo);
    
    //return true for non I/O suspension (refer to jpeblib.doc in jpeg lib)
    //return false to enable I/O suspension
    return true;

error:
    MEM_FREE_AND_NULL(jpeg_cur_buf);
    if(buf_node)
    {
        MEM_FREE_AND_NULL(buf_node);
    }
    return false;
}

static smjob_rcode_t sif_data_send_msg_wait_opt(sif_data_msg_type_t type, void *data, uint32_t pthread_wait_opt)
{
    sif_data_msg_buf_t buf;

    buf.msg.type = type;
    buf.msg.data = data;

    uint32_t pthread_rcode = posix_message_send(sif_data_msg_queue, (char *)&buf, 
                                                sizeof(sif_data_msg_buf_t),
                                                MQ_DEFAULT_PRIORITY, pthread_wait_opt);
    if(pthread_rcode == ETIMEDOUT)
    {
        /* 
         *  msg queue full
         * 
         *  NOTE: should only get here when caller is a non-thread (e.g. timer) and pthread_wait_opt must
         *  therefore be set to 0
         */
        return SMJOB_WOULDBLOCK;
    }
    else if(pthread_rcode != 0)
    {
        // any other errors are unexpected and unhandled
        XASSERT(0, pthread_rcode);
        return SMJOB_FAIL;
    }

    return SMJOB_OK;
}



