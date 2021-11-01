/** 
 * $Header:  $
 *
 * ============================================================================
 * Copyright (c) 2008   Marvell Semiconductor, Inc. All Rights
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

#include "asic.h"
//#include "jbig.h"
#include "jbig_decode_api.h"
//#include "intnums.h"
#include "jbig_decode_config.h"


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
   JBIG_MSG_DECODE_COMPLETE,
   JBIG_MSG_CODEC_COMPLETE,
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
    uint32_t shadow;
    tJBIGControlRegDef JBIGControlReg;
} tJBIGControlShadow;

/**
 * \brief This structure defines the layout of the video DMA descriptor. DMA
 *  descriptors reside in memory, and must NOT be cached. Descriptors are
 *  "chained", and the DMA hardware "follows the chain", as long as the next
 *  descriptor in the chain is marked as ready.
 **/
struct jbig_dma_desc_s
{
    volatile uint32_t control;
    volatile uint32_t *dataBuffer;
    volatile uint32_t length;
    volatile struct jbigDescriptor * next_desc;
    void* hw_address;
    void* user_callback_data; // This value is provided to the callback function
};
#if 0
typedef struct
{
    JBIGMSG msgType;
    tJBIGDMADescriptor * descriptor;
    jbig_decode_handle_t * handle;
}JBIG_MESSAGE;
#endif

// Generic JBIG descriptor List structure
#if 0
typedef struct jbigDescriptorList
{
    tJBIGDMADescriptor * ListHead;
    tJBIGDMADescriptor * ListTail;
} tJBIGDescriptorList;
#endif
#if 0
struct jbig_decode_handle_s
{
    uint32_t    channel;
    JBIG_CONFIG config;
    uint8_t		inUse;
    uint8_t     dirty;
    uint32_t    lastError;
    uint8_t		started;
    uint32_t    linesRemaining;
    
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
    
    // pointer to callback given by user
    JBIG_CALLBACK  jbigCallback;
    odma_event_callback_t odma_callback;
    
    // JBIG block data including block type, register interface pointers, and int nums.
    const jbig_block_config_t* jbig_block_interface;
};
#endif

// JBIG descriptor configuration bit definitions
#define JBIG_DESCRIPTOR_OWNERSHIP (1<<16)
#define JBIG_DESCRIPTOR_END_OF_IMAGE (1<<7)
#define JBIG_DESCRIPTOR_START_OF_IMAGE (1<<6)
#define JBIG_DESCRIPTOR_INTERRUPT_ON_COMPLETION (1<<1)
#define JBIG_DESCRIPTOR_STOP (1<<0)

#define JBIG_DMA_INT_MASK (JBIG_DECODE_IDMA_INT_EN_FIN_INT_EN_MASK | JBIG_DECODE_IDMA_INT_EN_WHO_INT_EN_MASK)


void jbigResetHardware( jbig_decode_handle_t * handle );
error_type_t jbigAddWriteDMABuffer(jbig_decode_handle_t * handle, uint8_t *addr, uint32_t size, void * userData );
error_type_t jbigAddReadDMABuffer( jbig_decode_handle_t * handle, uint8_t *addr, uint32_t size, void * userData );

void jbigStartReadDMA( jbig_decode_handle_t * inHandle );
void jbigStartWriteDMA( jbig_decode_handle_t * inHandle );
void jbigInit(const jbig_decode_block_config_t * jbig_block_config);
void jbigResetReadDMA(  jbig_decode_handle_t * handle );
void jbigResetWriteDMA( jbig_decode_handle_t * handle );
void jbigStart( jbig_decode_handle_t * handle );
void jbigStop( jbig_decode_handle_t * handle );
error_type_t jbigCommitConfig( jbig_decode_handle_t *handle );
void jbigInitDescriptors( jbig_decode_handle_t *handle );
void jbigFreeMemory( jbig_decode_handle_t * handle );
jbig_decode_handle_t * jbig_decode_assign_handle( void );
void jbigSetCurrentTransaction( jbig_decode_handle_t * inHandle );
jbig_decode_handle_t * jbigGetCurrentTransaction( void );
void jbigClearCurrentTransaction( jbig_decode_handle_t * inHandle );
error_type_t jbigStartThread( void );
error_type_t jbigStopThread( void );

// Debug functions
void jbig_dump_control_regs(jbig_decode_handle_t *handle);
void jbig_dump_odma_regs(jbig_decode_handle_t *handle);
void jbig_dump_idma_regs(jbig_decode_handle_t *handle);
void jbigFreeHandleDescriptors( jbig_decode_handle_t * handle );

extern tJBIGControlShadow JBIGControlShadow;

