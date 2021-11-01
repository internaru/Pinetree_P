/** 
 * $Header:  $
 *
 * ============================================================================
 * Copyright (c) 2008,2013   Marvell Semiconductor, Inc. All Rights
 * Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *  This is the header file to export the interface to jbigUtils.c
 * 
 *
 **/

//-jrs#include "asic.h"
#include "jbig.h"
#include "jbig_api.h"
#include "mrvl_jbig_config.h"
#include "dma_alloc_api.h"

#define LOGGER_JBIG_UTILS 0x40000000    // pick a bit for this file.
#define LOGGER_JBIG_API   0x20000000    // pick a bit for this file.
#define LOGGER_JBIG_TEST  0x80000000    // pick a bit for this file.


#define REG_SET_BITS(addr, val) *(addr) |= (val);
#define REG_CLEAR_BITS(addr, val) *(addr) &= ~(val);
#define REG_CLEAR(addr) *(addr) = 0x0;

#define JBIG_IDMA_REGS_t JBIG_DECODE_IDMA_REGS_t
#define JBIG_ODMA_REGS_t JBIG_DECODE_ODMA_REGS_t
#define JBIG_CONTROL_REGS_t JBIG_DECODE_REGS_t


typedef enum
{
   JBIG_MSG_FREE_MEMORY = 0x5,
   JBIG_MSG_IDMA_COMPLETE,          // input dma complete
   JBIG_MSG_ODMA_COMPLETE,          // output dma complete
   JBIG_MSG_OPERATION_COMPLETE,
   JBIG_MSG_ODMA_EOI,
   JBIG_MSG_STOP_THREAD
} JBIGMSG;


typedef struct 
{
    unsigned int            : 16;
    unsigned int enAtmov    : 1; 
    unsigned int line3      : 1;   
    unsigned int typLineDet : 1; 
    unsigned int            : 1;    
    unsigned int encode     : 1;
    unsigned int decode     : 1;
    unsigned int            : 2;
    unsigned int streaming  : 1;
    unsigned int atmov      : 7;
} tJBIGControlRegDef;

typedef union 
{
    UINT32 shadow;
    tJBIGControlRegDef JBIGControlReg;
} tJBIGControlShadow;

/**
 * \brief This structure defines the layout of the video DMA descriptor. DMA
 *  descriptors reside in memory, and must NOT be cached. Descriptors are
 *  "chained", and the DMA hardware "follows the chain", as long as the next
 *  descriptor in the chain is marked as ready.
 **/
typedef struct jbigDescriptor
{
    volatile ALIGN32 uint32_t control;
    volatile uint32_t *dataBuffer;
    volatile uint32_t length;
    volatile struct jbigDescriptor * next_desc_phys_addr;
    struct jbigDescriptor * next_desc_virt_addr;
    void * context;            ///< context pointer to allow recovery of callback and dma tracking info
    void* hw_addr;             ///< physical HW address of this descriptor; 
                               ///needed to free with MEM_FREE_UNCACHED and needed when starting DMA
} tJBIGDMADescriptor;

typedef struct jbig_dma_context_s
{
//    dma_alloc_t* dma_alloc;
    BigBuffer_t* buf;
    uint32_t len;
    void* user_callback_data;
}jbig_dma_context_t;

typedef struct
{
    JBIGMSG msgType;
    tJBIGDMADescriptor * descriptor;
    jbig_handle_t * handle;
}JBIG_MESSAGE;


// Generic JBIG descriptor List structure
typedef struct jbigDescriptorList
{
    tJBIGDMADescriptor * ListHead;
    tJBIGDMADescriptor * ListTail;
} tJBIGDescriptorList;


struct jbig_handle_s
{
    uint32_t    channel;
    JBIG_CONFIG config;
    uint8_t		inUse;
    uint8_t     dirty;
    uint32_t    lastError;
    uint8_t		started;
    bool        complete;
    bool        odma_complete;
    uint32_t    linesRemaining;

    uint32_t    bytes_output;       ///< number of bytes output by ODMA during last transaction

    pthread_mutex_t lock;
    sem_t open_sem;             ///< block is in use
    sem_t completion_sem;       ///< completion semaphore used by blocking encode/decode apis
    
    // linked list of pending descriptors
    tJBIGDescriptorList  pendingReadDescriptorList;
    tJBIGDescriptorList  currentReadDescriptorList;
    
    // These counters are used to track how many descriptors we
    // have in flight at once. If we exceed the number of messages
    // we can handle then they will be lost.
    uint32_t    numPendingMessages;
    uint32_t    maxNumMessages;

    // The pending list is used to build a linked
    // list of descriptors the current list is the 
    // list that is being worked on by the JBIG block
    tJBIGDescriptorList  pendingWriteDescriptorList;
    tJBIGDescriptorList  currentWriteDescriptorList;
    
    // pointer to JBIG event callback given by user
    JBIG_CALLBACK  jbigCallback;
    void* jbig_callback_context;
    uint32_t control_status;

    JBIG_DMA_CALLBACK odma_callback;
    JBIG_DMA_CALLBACK idma_callback;
        
    // JBIG block data including block type, register interface pointers, and int nums.
    const jbig_block_config_t* jbig_block_interface;
};

// JBIG descriptor configuration bit definitions
#define JBIG_DESCRIPTOR_OWNERSHIP (1<<16)
#define JBIG_DESCRIPTOR_END_OF_IMAGE (1<<7)
#define JBIG_DESCRIPTOR_START_OF_IMAGE (1<<6)
#define JBIG_DESCRIPTOR_INTERRUPT_ON_COMPLETION (1<<1)
#define JBIG_DESCRIPTOR_STOP (1<<0)

void jbigResetHardware( jbig_handle_t * handle );
error_type_t jbigAddWriteDMABuffer(jbig_handle_t * handle, BigBuffer_t* buf, uint32_t size, void * userData );
error_type_t jbigAddReadDMABuffer( jbig_handle_t * handle, BigBuffer_t* buf, uint32_t size, void * userData );

void jbigSetCallback( jbig_handle_t * inHandle, JBIG_CALLBACK callback );
void jbigClearCallback( jbig_handle_t * inHandle );

void jbigStartReadDMA( jbig_handle_t * inHandle );
void jbigStartWriteDMA( jbig_handle_t * inHandle );
void jbigInit(const jbig_block_config_t * jbig_block_config);
void jbigResetReadDMA(  jbig_handle_t * handle );
void jbigResetWriteDMA( jbig_handle_t * handle );
void jbigStart( jbig_handle_t * handle );
void jbigStop( jbig_handle_t * handle );
error_type_t jbigCommitConfig( jbig_handle_t *handle );
void jbigInitDescriptors( jbig_handle_t *handle );
void jbigFreeMemory( jbig_handle_t * handle );
jbig_handle_t * jbigAssignHandle( JBIG_BLOCK block );
void jbigSetCurrentTransaction( jbig_handle_t * inHandle );
jbig_handle_t * jbigGetCurrentTransaction( JBIG_BLOCK inBlock);
void jbigClearCurrentTransaction( jbig_handle_t * inHandle );
error_type_t jbigStartThread( void );
error_type_t jbigStopThread( void );

// Debug functions
void jbig_dump_control_regs(jbig_handle_t *handle);
void jbig_dump_odma_regs(jbig_handle_t *handle);
void jbig_dump_idma_regs(jbig_handle_t *handle);
void jbig_dump_all_regs(jbig_handle_t * handle);

void jbigFreeHandleDescriptors( jbig_handle_t * handle );

extern tJBIGControlShadow JBIGControlShadow;

