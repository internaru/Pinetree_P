/****************************************************************************** 
 * Copyright (c) 2013  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#ifndef SMJOB_H
#define SMJOB_H

#include <stdint.h>
#include <stdio.h>
#include "sm_job_api.h"
#ifdef HAVE_JPEG_SW
#include "jpeglib.h"
#endif // HAVE_JPEG_SW

typedef struct smjob_sif_buf_node_s
{
    struct smjob_sif_buf_node_s *next; // linked list
    uint8_t     *buf;
    uint32_t    len;
} smjob_sif_buf_node_t;


/**
 * @brief Job context
 * 
 * Internal representation of a job, whether pending, active, or historical. 
 */
typedef struct
{
    uint32_t                job_id;
    smjob_ticket_t         ticket;
    smjob_state_t          state;
    uint32_t                num_reasons; 
    smjob_state_reason_t   reasons[SMJOB_MAX_STATE_REASONS]; 
    smjob_status_func_t    status_func;
    smjob_status_events_t  event_mask;
    void                    *user_data;

//    TX_EVENT_FLAGS_GROUP    flgs;
    sem_t                   semaphore;

    // TODO TEMP
    bool                    is_color;
    
    // done processing data - no new data will be added to data queue for this job
    bool                    data_done; 

    // job was canceled prior to EOF - data is incomplete for this job
    bool                    data_abort;

    smjob_sif_buf_node_t   *data_queue;
    smjob_sif_buf_node_t   **data_queue_tail;

    // used to coordinate toggling of SMJOB_TX_FLAG_SCAN_DATA_AVAIL synchronization flag
    bool                    data_queue_emptied;
} smjob_ctxt_t;

#define SMJOB_TX_FLAG_SCAN_DATA_AVAIL      0x00000001


#define SMJOB_CONTAINING_RECORD(address, type, field) ((address) ? ((type*)( ((char*)(address)) - ((unsigned int)((char *)&(((type *)0)->field) - (char *)0)))) : 0 )

#define SMJOB_IS_COMPLETED(job_ptr)     ((job_ptr)->state == SMJOB_STATE_CANCELED || \
                                         (job_ptr)->state == SMJOB_STATE_ABORTED || \
                                         (job_ptr)->state == SMJOB_STATE_COMPLETED)

void smjob_lock_ctxt(void);

void smjob_unlock_ctxt(void);


#endif // SMJOB_H
