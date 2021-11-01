/** 
 * $Header:  $
 *
 * ============================================================================
 *  Copyright (c) 2008, 2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *  This is the header file to export the interface to jbigUtils.c
 * 
 *
 **/

 // Note: The JBENABLE register must be manipulated directly, and NOT with the
 // REG_SET_BITS, REG_WRITE, and REG_CLEAR_BITS macros. This is because the soft
 // reset bit operates incorrectly - when a "1" is written to reset it, the
 // register will read a "0". And to clear it by writing a "0" yeilds a "1" when
 // the register is read.
#include "posix_ostools.h"
#include "sys_init_api.h"
#include "string.h" 
#include "ATypes.h"
#include "logger.h"
#include "debug.h"
#include "memAPI.h"
#include "lassert.h"
#include "logger.h"
#include "jbigUtils.h"
#include "jbig_test.h"
#include "dma_buffer.h"
#ifdef HAVE_POWER_MGR
#include "pwr_mgr_api.h"
#endif

#ifdef ASIC_gemstone

#if ASIC_REV < ASIC_REV_Z1
#error "Unsupported gemstone rev"
#endif

#elif defined(ASIC_SS1000)
// all revs of SS1000 supported
#elif defined(ASIC_granite)
// all revs of granite supported
#else
#error "Unsupported ASIC "
#endif


/**
 * 
 * DEFINES
 * 
 **/

#define DBG_PRFX "JBIG: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_DEVICES | LOGGER_SUBMODULE_BIT( 14 )

#define DBG_SLOG_ENTRY DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);
 
 
#ifdef __linux__
static void isrJbigIDMA( int32_t interrupt_count, void* context );
static void isrJbigODMA( int32_t interrupt_count, void* context );
static void isrJbigControl( int32_t interrupt_count, void* context );
#else
static void isrJbigIDMA( uint32_t intPos );
static void isrJbigODMA( uint32_t intPos );
static void isrJbigControl( uint32_t intPos );
#endif

static void enableJBIGInterrupts(jbig_handle_t * inHandle );
void disableJBIGInterrupts(jbig_handle_t * inHandle );

static void jbigFreeDescriptors( );

static uint8_t isJbigStarted( jbig_handle_t * inHandle );

// Jbig block threads. There are indivi
static void* jbigThread( void* Temp);

extern void jbig_init(void);
extern void jbig_set_last_error_code( uint32_t errorCode );

#ifdef HAVE_POWER_MGR
static void jbig_pwr_init(void);
#endif

jbig_handle_t jbig_handles[NUM_JBIG_BLOCKS];

// ThreadX thread object
static pthread_t jbigMessageThread;

#define NUM_JBIG_MESSAGES_PER_BLOCK 25

// Stack
#define JBIG_STACK_SIZE  POSIX_MIN_STACK_SIZE
static ALIGN8 unsigned char JBIGStack[JBIG_STACK_SIZE];


// Message queue
static mqd_t jbig_msg_queue;

#define JBIG_WAIT_LOOP_MAX 100

/**
 * jbig_block_init - modularize jbigInit, to allow pwrmgr to 
 * reinit hardware. 
 * 
 * @author  (10/13/2011)
 * 
 * @param jbig_block_interface 
 */
void jbig_block_init(const jbig_block_config_t* jbig_block_interface)
{
    DBG_MEMLOG(LOG_INFO,"JBIG BLOCK INIT FROM POWER MANAGER!!!\n");
    //jbig_block_interface->jbig_regs->JCTL = 0;
    jbig_block_interface->jbig_idma_regs->int_en = 0x1FF/*JBIG_DMA_INT_MASK*/;
    jbig_block_interface->jbig_odma_regs->int_en = 0x1FF/*JBIG_DMA_INT_MASK*/;
    jbig_block_interface->jbig_idma_regs->int_cl = 0x1FF;
    jbig_block_interface->jbig_odma_regs->int_ack = 0xFFF;
}

/**
 * jbigInit
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
void jbigInit(const jbig_block_config_t * jbig_plat_config)
{
    uint32_t block_count;

    for(block_count = 0; block_count < NUM_JBIG_BLOCKS; block_count++)
    {
        memset(&jbig_handles[block_count], 0, sizeof(jbig_handle_t));

        // attach the low level block config to the handle
        jbig_handles[block_count].jbig_block_interface = &jbig_plat_config[block_count];

        jbig_block_init(jbig_handles[block_count].jbig_block_interface);

        uio_int_attach( jbig_handles[block_count].jbig_block_interface->dev_jbig, 0, isrJbigControl, &jbig_handles[block_count]);
        uio_int_attach( jbig_handles[block_count].jbig_block_interface->dev_odma, 0, isrJbigODMA, &jbig_handles[block_count]);
        uio_int_attach( jbig_handles[block_count].jbig_block_interface->dev_idma, 0, isrJbigIDMA, &jbig_handles[block_count]);

        posix_mutex_init(&jbig_handles[block_count].lock);
        sem_init(&jbig_handles[block_count].open_sem, 0, 1);
        sem_init(&jbig_handles[block_count].completion_sem, 0, 0);
    }

    posix_create_thread(&jbigMessageThread, jbigThread, 0, "jbig_thread", JBIGStack, JBIG_STACK_SIZE, POSIX_THR_PRI_IMAGE);

#ifdef HAVE_POWER_MGR
    jbig_pwr_init();
#endif
    // Create the message queue
    posix_create_message_queue(&jbig_msg_queue, "/jbig_queue", (NUM_JBIG_MESSAGES_PER_BLOCK*NUM_JBIG_BLOCKS), sizeof(JBIG_MESSAGE));
}

static inline void jbig_increment_pending_messages(jbig_handle_t* handle)
{
    pthread_mutex_lock(&handle->lock);
    handle->numPendingMessages++;
    pthread_mutex_unlock(&handle->lock);
}

static inline void jbig_decrement_pending_messages(jbig_handle_t* handle)
{
    pthread_mutex_lock(&handle->lock);
    if (handle->numPendingMessages > 0)
    {
        handle->numPendingMessages--;
    }
    pthread_mutex_unlock(&handle->lock);
}

/**
 * jbigAddDescriptorToList
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigAddDescriptorToList( tJBIGDescriptorList * inList, tJBIGDMADescriptor * inDescriptor)
{
    ASSERT( inList );
    ASSERT( inDescriptor );
    
    DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Add Pending Descriptor %#x list %#x head %#x tail %#x\n",inDescriptor, inList, inList->ListHead, inList->ListTail);
    
    if( inDescriptor && inList )
    {
        inDescriptor->next_desc_phys_addr = NULL;
        inDescriptor->next_desc_virt_addr = NULL;

        if( inList->ListHead == NULL )
        {
            inList->ListHead = inDescriptor;
            inList->ListTail = inDescriptor;
        }
        else
        {
            tJBIGDMADescriptor * tmpDescriptor = inList->ListTail;
            
            tmpDescriptor->next_desc_virt_addr = inDescriptor;
            tmpDescriptor->next_desc_phys_addr = inDescriptor->hw_addr;
            
            inList->ListTail = inDescriptor;
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: previous Descriptor next %#x\n",tmpDescriptor->next_desc_virt_addr);                                    
        }    
    }
}

/**
 * jbigGetDescriptorListHead
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

tJBIGDMADescriptor * jbigGetDescriptorListHead( tJBIGDescriptorList inList )
{
    return inList.ListHead;
}

/**
 * jbigGetDescriptorListTail
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

tJBIGDMADescriptor * jbigGetDescriptorListTail( tJBIGDescriptorList inList )
{
    return inList.ListTail;
}

static inline void jbigTransferDescriptorList(tJBIGDescriptorList* destList, tJBIGDescriptorList* srcList)
{
    destList->ListHead = srcList->ListHead;
    destList->ListTail = srcList->ListTail;
                    
    srcList->ListHead = NULL;
    srcList->ListTail = NULL;
}

/**
 * jbigRemoveDescriptorFromListHead
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

tJBIGDMADescriptor * jbigRemoveDescriptorFromListHead( tJBIGDescriptorList * inList )
{
    ASSERT(inList);
    tJBIGDMADescriptor * returnDescriptor = inList->ListHead;
    
    if( inList->ListHead )
    {
        inList->ListHead = inList->ListHead->next_desc_virt_addr;
        if( inList->ListHead == NULL )
        {
            inList->ListTail = NULL;
        }
        returnDescriptor->next_desc_virt_addr = NULL;
        returnDescriptor->next_desc_phys_addr = NULL;
    }
    
    
    DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Remove Descriptor %#x from list %#x\n",returnDescriptor, inList);
    
    return  returnDescriptor;
}

/**
 * @brief Search the virtual address space list and return the descriptor with the matching physical address.
 * @param inList 
 * @param desc_phys_addr 
 * 
 * @return tJBIGDMADescriptor* 
 */
tJBIGDMADescriptor * jbig_get_desc_vaddr_from_list(tJBIGDescriptorList * inList, tJBIGDMADescriptor * desc_phys_addr)
{
    tJBIGDMADescriptor *list_iter = inList->ListHead;
    while(list_iter != NULL)
    {
        if(list_iter->hw_addr == desc_phys_addr)
        {
            return list_iter;
        }
        list_iter = list_iter->next_desc_virt_addr;
    }
    return NULL;
}

/**
 * jbigAddDMABuffer
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

error_type_t jbigAddDMABuffer( jbig_handle_t * inHandle, tJBIGDescriptorList* inList, BigBuffer_t* buf, uint32_t size, void * context, uint32_t dma_direction)
{
    error_type_t returnValue = FAIL;
    void *phys_addr=0;

    // If the specified size is 0, simply return fail.
    if (size == 0)
    {
        return returnValue;
    }
    
    if( inHandle && inHandle->inUse)
    {
        tJBIGDMADescriptor * p_JBIG_Descriptor = NULL;

        p_JBIG_Descriptor = (tJBIGDMADescriptor *)MEM_MALLOC_UNCACHED(&phys_addr, sizeof(tJBIGDMADescriptor),e_32_byte);                            
        if( p_JBIG_Descriptor )
        {
            DBG_MEMLOG(LOG_DEBUG_M,"JBIG: add descriptor %#x addr %#x size %d context %#x\n", p_JBIG_Descriptor, buf, size, context);
            p_JBIG_Descriptor->hw_addr = phys_addr;

            // Set up JBIG read DMA. DMA starts when transfer count register
            // is programmed.
            BigBuffer_convert_to_dma_buffer(buf);

            p_JBIG_Descriptor->dataBuffer = dma_buffer_map_single(buf, dma_direction);
            p_JBIG_Descriptor->length = size;
            p_JBIG_Descriptor->control = 0;
            p_JBIG_Descriptor->context = context;
    
            REG_SET_BITS(&p_JBIG_Descriptor->control, JBIG_DESCRIPTOR_END_OF_IMAGE |
                                                      JBIG_DESCRIPTOR_INTERRUPT_ON_COMPLETION |
                                                      JBIG_DESCRIPTOR_OWNERSHIP |
                                                      JBIG_DESCRIPTOR_STOP);

            pthread_mutex_lock(&inHandle->lock);
            tJBIGDMADescriptor * lastJBIGDescriptor = jbigGetDescriptorListTail( *inList );
            
            if( lastJBIGDescriptor != NULL)
            {
                if( (lastJBIGDescriptor->control & JBIG_DESCRIPTOR_OWNERSHIP ) == 0 )
                {
                    DBG_MEMLOG(LOG_WARNING,"JBIG: last descriptor ownership bit is NULL\n" );
                }

                REG_CLEAR_BITS( &lastJBIGDescriptor->control, JBIG_DESCRIPTOR_END_OF_IMAGE | JBIG_DESCRIPTOR_STOP | JBIG_DESCRIPTOR_INTERRUPT_ON_COMPLETION);
            }
            else
            {
                DBG_MEMLOG(LOG_DEBUG_H,"JBIG: chained!!!! last descriptor is NULL\n" );
    
                REG_SET_BITS( &p_JBIG_Descriptor->control, JBIG_DESCRIPTOR_START_OF_IMAGE);                       
            }
            
            inHandle->dirty = 1; // indicates that we may need to clean up the descriptor list later
            jbigAddDescriptorToList( inList, p_JBIG_Descriptor);
            pthread_mutex_unlock(&inHandle->lock);
        
            returnValue = OK;

        }
        else
        {
           DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Descriptor MALLOC failed!!!\n");
        }

    }
    return ( returnValue );
}

/**
 * jbigStartWriteDMAIfPossible
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

error_type_t jbigStartWriteDMAIfPossible( jbig_handle_t * inHandle )
{
    
    error_type_t returnValue = FAIL;
    uint8_t dmaBusy = 0;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    tJBIGDMADescriptor * p_JBIG_Descriptor = NULL;
    
    DBG_SLOG_ENTRY
    
    if( inHandle && inHandle->inUse)
    { 
        p_JBIG_ODMA_regs = inHandle->jbig_block_interface->jbig_odma_regs;
        
        dmaBusy = p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK;
        
        pthread_mutex_lock(&inHandle->lock);
        if( inHandle->pendingWriteDescriptorList.ListHead && !inHandle->currentWriteDescriptorList.ListHead )
        {
            if( isJbigStarted( inHandle ) )
            {
                DBG_PRINTF_INFO("JBIG: Add Write buffer Start DMA now  %d!!!!!!!\n",isJbigStarted( inHandle ));

                inHandle->odma_complete = false;
                        
                jbig_increment_pending_messages(inHandle);

//-jrs                uio_int_disable(inHandle->jbig_block_interface->dev_odma);
                                
                // this is the first descriptor for this page so we
                
                // Set up the ODMA registers here

                // set up the output Data Width to 8 bits
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_IN_WIDTH_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 3);
    
                // set the DMA burst length 4 words
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_BURST_LEN_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0);
    
                
                // transfer the pending descriptor list to the current descriptor list
                jbigTransferDescriptorList(&inHandle->currentWriteDescriptorList, &inHandle->pendingWriteDescriptorList);
        
                p_JBIG_Descriptor = jbigGetDescriptorListHead( inHandle->currentWriteDescriptorList );
        
                dmaBusy = p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK;
                
                DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Write desc_write %#x status %#x  \n", p_JBIG_Descriptor, p_JBIG_ODMA_regs->status );
                
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 1 );
                
                p_JBIG_ODMA_regs->desc_write = (uint32_t)p_JBIG_Descriptor->hw_addr;
        
//-jrs                uio_int_enable(inHandle->jbig_block_interface->dev_odma);
                        
                returnValue = OK;
                
    
            }   
            else
            {
                inHandle->started = 2;
                DBG_PRINTF_INFO("JBIG: Can't Start Write DMA now %#x %#x \n",isJbigStarted( inHandle ), dmaBusy );
            }
        }
        pthread_mutex_unlock(&inHandle->lock);
    }

    DBG_SLOG_EXIT
        
    return returnValue;
}

/**
 * jbigStartReadDMAIfPossble
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

error_type_t jbigStartReadDMAIfPossible( jbig_handle_t * inHandle )
{
    error_type_t returnValue = FAIL;
    uint8_t dmaBusy = 0;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs = NULL;
    tJBIGDMADescriptor * p_JBIG_Descriptor = NULL;

    if (inHandle && inHandle->inUse )
    {
        p_JBIG_IDMA_regs = inHandle->jbig_block_interface->jbig_idma_regs;

        dmaBusy = p_JBIG_IDMA_regs->status & IDMA_SLI_STATUS_DMA_BUSY_MASK;

        pthread_mutex_lock(&inHandle->lock);
        if ( inHandle->pendingReadDescriptorList.ListHead && !inHandle->currentReadDescriptorList.ListHead )
        {
            if ( isJbigStarted( inHandle ) )
            {
                DBG_PRINTF_INFO("JBIG: Add Read buffer Start DMA now  %d!!!!!!!\n",isJbigStarted( inHandle ));

//-jrs                uio_int_disable(inHandle->jbig_block_interface->dev_idma);
                jbig_increment_pending_messages(inHandle);

                // enable the DMA
                p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_IDMA_regs->cfg, 1 );

                p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_REPLICATE_REPLACE_VAL(p_JBIG_IDMA_regs->cfg, 0 );

                // set up the output Data Width to 32 bits
                p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_OUT_WIDTH_REPLACE_VAL( p_JBIG_IDMA_regs->cfg, 5 );

                // set the DMA burst length 4 words
                p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_BURST_LEN_REPLACE_VAL( p_JBIG_IDMA_regs->cfg, 0 );

                // The JBIG line width must be set for proper operation when the
                // output of the decoder is connected directly to other hardware
                // via the CBI interface. Linewidth is used by the pipeline for
                // determining end of line and end of image for blocks that need
                // to know. Note that since JBIG assumes all pixels are 1 bit,
                // but the pipeline can unpack the data in 1 2 or 4 bit chunks,
                // this line width value might be different than the JBIG
                // register value JX. 
                
                // REVISIT DAB p_JBIG_IDMA_regs->line_width = IDMA_SLI_LINE_WIDTH_LINE_WIDTH_REPLACE_VAL(p_JBIG_IDMA_regs->line_width, 4864);

                jbigTransferDescriptorList(&inHandle->currentReadDescriptorList, &inHandle->pendingReadDescriptorList);

                p_JBIG_Descriptor = jbigGetDescriptorListHead( inHandle->currentReadDescriptorList );

                dmaBusy = p_JBIG_IDMA_regs->status & IDMA_SLI_STATUS_DMA_BUSY_MASK;

                DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Read desc_write %#x status %#x  \n", p_JBIG_Descriptor, p_JBIG_IDMA_regs->status );

                p_JBIG_IDMA_regs->desc_write = (uint32_t)p_JBIG_Descriptor->hw_addr;

//-jrs                uio_int_enable(inHandle->jbig_block_interface->dev_idma);

                returnValue = OK;
            }
            else
            {
                inHandle->started = 2;
                DBG_PRINTF_INFO("JBIG: Can't Start READ DMA now %#x %#x \n",isJbigStarted( inHandle ), dmaBusy );
            }
        }
        else
        {
            DBG_PRINTF_INFO("JBIG: Can't Start READ DMA now no pending list pend %#x current %#x \n", inHandle->pendingReadDescriptorList.ListHead, inHandle->currentReadDescriptorList.ListHead );
        }
        pthread_mutex_unlock(&inHandle->lock);
    }
    else
    {
        DBG_PRINTF_INFO("JBIG: Can't Start READ DMA handle not valid\n" );
    }

    return returnValue;
}


/**
 * disableJBIGInterrupts
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void disableJBIGInterrupts( jbig_handle_t * inHandle )
{
    if( inHandle && inHandle->inUse)
    {
        uio_int_disable( inHandle->jbig_block_interface->dev_jbig );
        uio_int_disable( inHandle->jbig_block_interface->dev_odma );
        uio_int_disable( inHandle->jbig_block_interface->dev_idma );
    }
}

/**
 * enableJBIGInterrupts
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

static void enableJBIGInterrupts( jbig_handle_t * inHandle)
{
    if( inHandle && inHandle->inUse)
    {
        uio_int_enable( inHandle->jbig_block_interface->dev_jbig );
        uio_int_enable( inHandle->jbig_block_interface->dev_odma );
        uio_int_enable( inHandle->jbig_block_interface->dev_idma );
    }
}


/**
 * \brief  Return the number of elements in an os queue
 *
 */

static uint32_t jbig_get_queue_pending( mqd_t *queue_ptr ) 
{
    struct mq_attr attr;

    mq_getattr(*queue_ptr, &attr);
    return attr.mq_curmsgs;
}


void jbig_update_lines_Remaining( jbig_handle_t * inHandle, uint32_t bytesProcessed)
{
    JBIG_CONFIG tempConfig;
    jbig_get_config( inHandle, &tempConfig);
    uint32_t linesProcessed = bytesProcessed / tempConfig.LineWidth;
    inHandle->linesRemaining -= linesProcessed;
}

/**
 * jbigMessageHandler
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
uint32_t jbig_buffers_completed = 0;

static void jbigMessageHandler(JBIG_MESSAGE message)
{
    tJBIGDescriptorList jbigCompletionList;
    tJBIGDMADescriptor *firstJBIGDescriptor = NULL;
    jbig_handle_t *currentHandle = NULL;
    jbig_dma_context_t *dma_context = NULL;

    currentHandle = message.handle;

    if (!currentHandle)
    {
        DBG_MEMLOG(LOG_WARNING, "JBIG: message without handle %#x\n", message.descriptor);
        return;
    }

    switch (message.msgType)
    {
        case JBIG_MSG_IDMA_COMPLETE:
            {
                DBG_PRINTF(LOG_DEBUG_M, "JBIG: JBIG_MSG_IDMA_COMPLETE\n");
                if (!currentHandle->inUse)
                {
                    DBG_MEMLOG(LOG_WARNING, "JBIG: Handle closed before messages processed!!!\n");
                    return;
                }

                jbig_decrement_pending_messages(currentHandle);

                pthread_mutex_lock(&currentHandle->lock);
                jbigTransferDescriptorList(&jbigCompletionList, &currentHandle->currentReadDescriptorList);
                pthread_mutex_unlock(&currentHandle->lock);

                firstJBIGDescriptor = jbigRemoveDescriptorFromListHead(&jbigCompletionList);

                //DBG_MEMLOG(LOG_DEBUG_H,"JBIG: IDMA descriptor %#x %#x\n", firstJBIGDescriptor, message.descriptor );

                while (firstJBIGDescriptor)
                {
                    ASSERT((firstJBIGDescriptor->control & JBIG_DESCRIPTOR_OWNERSHIP) == 0);

                    dma_context = (jbig_dma_context_t *)firstJBIGDescriptor->context;
                    dma_buffer_unmap_single(dma_context->buf, DMA_TO_DEVICE);

                    if (currentHandle->idma_callback)
                    {
                        currentHandle->idma_callback(currentHandle, dma_context->user_callback_data);
                    }

                    MEM_FREE_AND_NULL(dma_context);
                    MEM_FREE_UNCACHED(firstJBIGDescriptor->hw_addr, firstJBIGDescriptor);
                    firstJBIGDescriptor = jbigRemoveDescriptorFromListHead(&jbigCompletionList);
                }

                if (jbigStartReadDMAIfPossible(currentHandle) == OK)
                {
                    DBG_MEMLOG(LOG_DEBUG_M, "JBIG: IDMA start another list\n");
                }

            }
            break;

        case JBIG_MSG_ODMA_COMPLETE:
        case JBIG_MSG_ODMA_EOI:
            {
                DBG_PRINTF(LOG_DEBUG_M, "JBIG: %s\n", (message.msgType == JBIG_MSG_ODMA_EOI) ? "JBIG_MSG_ODMA_EOI" : "JBIG_MSG_ODMA_COMPLETE");
                if (!currentHandle->inUse)
                {
                    DBG_MEMLOG(LOG_WARNING, "JBIG: Handle closed before messages processed!!!\n");
                    return;
                }

                jbig_decrement_pending_messages(currentHandle);

                pthread_mutex_lock(&currentHandle->lock);
                jbigTransferDescriptorList(&jbigCompletionList, &currentHandle->currentWriteDescriptorList);
                //DBG_MEMLOG(LOG_DEBUG_M,"JBIG: ODMA removed %#x\n",firstJBIGDescriptor);
                pthread_mutex_unlock(&currentHandle->lock);

                firstJBIGDescriptor = jbigRemoveDescriptorFromListHead(&jbigCompletionList);

                while (firstJBIGDescriptor != NULL)
                {
                    jbig_buffers_completed++;

                    XASSERT((firstJBIGDescriptor->control & JBIG_DESCRIPTOR_OWNERSHIP) == 0, firstJBIGDescriptor->control);

                    currentHandle->bytes_output += firstJBIGDescriptor->length;

                    DBG_PRINTF(LOG_DEBUG_M, "JBIG: bytes_output = %d\n", currentHandle->bytes_output);

                    dma_context = (jbig_dma_context_t *)firstJBIGDescriptor->context;
                    dma_buffer_unmap_single(dma_context->buf, DMA_FROM_DEVICE);

                    if (currentHandle->odma_callback)
                    {
                        currentHandle->odma_callback(currentHandle, dma_context->user_callback_data);
                    }

                    MEM_FREE_AND_NULL(dma_context);
                    MEM_FREE_UNCACHED(firstJBIGDescriptor->hw_addr, firstJBIGDescriptor);

                    firstJBIGDescriptor = jbigRemoveDescriptorFromListHead(&jbigCompletionList);
                }

                currentHandle->odma_complete = true;
                if (jbigStartWriteDMAIfPossible(currentHandle) == OK)
                {
                    DBG_MEMLOG(LOG_DEBUG_H, "JBIG: ODMA  EOI start another list\n");
                }

                if (currentHandle->complete && currentHandle->jbigCallback)
                {
                    currentHandle->jbigCallback(currentHandle, currentHandle->jbig_callback_context, currentHandle->control_status);
                    currentHandle->complete = false;
                }
            }
            break;

        case JBIG_MSG_OPERATION_COMPLETE:
            {
                DBG_PRINTF(LOG_DEBUG_M, "JBIG: JBIG_MSG_OPERATION_COMPLETE\n");
                if (!currentHandle->inUse)
                {
                    DBG_MEMLOG(LOG_WARNING, "JBIG: Handle closed before messages processed!!!\n");
                    return;
                }

                jbig_decrement_pending_messages(currentHandle);

                // we used to always call callback directly here.  Now if odma is not complete we set flag so that 
                // completion isn't notified until after all ODMA callbacks are done.  This gives somewhat nicer behavior
                // in that the caller will not get further DMA notifications after jbig completion is signaled.
                if (currentHandle->odma_complete && currentHandle->jbigCallback)
                {
                    currentHandle->jbigCallback(currentHandle, currentHandle->jbig_callback_context, currentHandle->control_status);
                }
                else
                {
                    currentHandle->complete = true;
                }

                //intEnable( currentHandle->jbig_block_interface->intnum_jbig );
            }
            break;

        case JBIG_MSG_FREE_MEMORY:
            {
                DBG_MEMLOG(LOG_DEBUG_M, "JBIG: JBIG_MSG_FREE_MEMORY\n");
                jbigFreeDescriptors();
            }
            break;

        default:
            {
                DPRINTF(DBG_SOFT | DBG_OUTPUT, ("Jbig: Unknown Message %d received\n", message.msgType));
                break;
            }
    }
}


/**
 * jbigThread
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/


static void* jbigThread( void* Temp)
{
	JBIG_MESSAGE message;
	int result;
	uint32_t numMessagesRemaining = 0;
	uint32_t jbigMaxMessage = 0;
	
	SysWaitForInit();
	
	while(1)
	{
        result = posix_wait_for_message(jbig_msg_queue, (char*)&message, sizeof(JBIG_MESSAGE), POSIX_WAIT_FOREVER);
        XASSERT(result == 0, result);

        numMessagesRemaining = NUM_JBIG_MESSAGES_PER_BLOCK - jbig_get_queue_pending( &jbig_msg_queue );
		if( numMessagesRemaining && numMessagesRemaining < 2)
		{
		    DBG_PRINTF_ERR("JBIG: LOW ON MESSAGES %d %d \n",numMessagesRemaining, jbigMaxMessage);
		}
		if( jbigMaxMessage < numMessagesRemaining )
		{
		    jbigMaxMessage = numMessagesRemaining;
        }

        jbigMessageHandler( message ); 
		
	}
    return 0;
}


/**
 * jbigInitDescriptors
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigInitDescriptors( jbig_handle_t *handle )
{
	if( handle )
	{
    
        handle->pendingReadDescriptorList.ListHead = NULL;
        handle->pendingReadDescriptorList.ListTail = NULL;
        handle->currentReadDescriptorList.ListHead = NULL;
        handle->currentReadDescriptorList.ListTail = NULL;
        
        handle->pendingWriteDescriptorList.ListHead = NULL;
        handle->pendingWriteDescriptorList.ListTail = NULL;
        handle->currentWriteDescriptorList.ListHead = NULL;
        handle->currentWriteDescriptorList.ListTail = NULL;
	}
}


/**
 * 
 *  This function will find the first available jbig handle in the
 *  jbig handle array.
 *
 **/

int16_t JbigGetNextAvailableHandle( JBIG_BLOCK block )
{
	uint16_t i;
	int16_t available_handle = -1;


	for ( i = 0; i < NUM_JBIG_BLOCKS; i++)
	{
	    // if dirty flag is set the memory has not been cleaned up yet
	    // on that handle so we don't want to use it.
        // also make sure block matches requested type
		if( jbig_handles[i].jbig_block_interface->jbig_block_type == block && !jbig_handles[i].inUse && !jbig_handles[i].dirty )
		{
			available_handle = i;
			break;
		}
	}

    if (available_handle < 0)
    {
        // didn't find an available handle; return one of the inuse handles; caller will block until available
        DBG_PRINTF_ERR("JBIG: WARNING: all blocks in use.  Will block for available. Potential DEADLOCK.\n");
        for (i = 0; i < NUM_JBIG_BLOCKS; i++)
        {
            if (jbig_handles[i].jbig_block_interface->jbig_block_type == block)
            {
                available_handle = i;
                break;
            }
        }
    }
    return available_handle; 
}

/**
 * jbigAssignHandle 
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

jbig_handle_t * jbigAssignHandle( JBIG_BLOCK block )
{
    int16_t handle_index;
    jbig_handle_t * handle = NULL;
    int err;
    
    handle_index = JbigGetNextAvailableHandle(block);
    if( handle_index >= 0 )
    {
        err = sem_wait(&jbig_handles[handle_index].open_sem);
        ASSERT(err == 0);
        if (err == 0)
        {
            handle = &jbig_handles[handle_index];
            handle->inUse = 1;
            handle->lastError = 0;
            handle->numPendingMessages = 0;
            handle->maxNumMessages = NUM_JBIG_MESSAGES_PER_BLOCK;
            memset((void *)&handle->config, 0x0, sizeof(JBIG_CONFIG));
            handle->config.Block = block;
            handle->pendingReadDescriptorList.ListHead = NULL;
            handle->pendingReadDescriptorList.ListTail = NULL;
            handle->currentReadDescriptorList.ListHead = NULL;
            handle->currentReadDescriptorList.ListTail = NULL;
        }
    }
    else
    {
        XASSERT(handle_index >= 0, handle_index );
    }
    
    return handle;
}


/**
 * isrJbigControl
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
#ifdef __linux__
static void isrJbigControl( int32_t interrupt_count, void *context )
#else
static void isrJbigControl( uint32_t context )
#endif
{
    JBIG_REGS_t * p_JBIG_control_regs = NULL;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    uint32_t errorStatus;
    uint32_t status = 0;
    JBIG_MESSAGE jbigMsg;
    jbig_handle_t * currentHandle = (jbig_handle_t*)context;

    jbigMsg.descriptor = 0;
    jbigMsg.handle = 0;

    ASSERT(currentHandle != NULL);
    
    pthread_mutex_lock(&currentHandle->lock);
    
    p_JBIG_control_regs = currentHandle->jbig_block_interface->jbig_regs;
    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    status = p_JBIG_control_regs->JSTAT;
    errorStatus = status & JBIG_JSTAT_E_MASK;

    DBG_PRINTF_INFO("JBIG: isrJbigControl currentHandle = %#x status = %#x\n", currentHandle, status);

    DBG_PRINTF(LOG_DEBUG_M,"JBIG: isrJbigControl currentHandle = %#x status = %#x, block_type = %s\n", currentHandle, status, 
               (currentHandle->jbig_block_interface->jbig_block_type == JBIG_CODEC_BLOCK) ? "CODEC" : "DECODE");

    /*  This isr gets hit only when the decoder signals that it's done,
     *  successfully or not.
     */

    if(status & JBIG_JSTAT_E_MASK)
    {
       DBG_MEMLOG(LOG_WARNING, "  isrJbigControl: decoder stopped with an error: 0x%#x\n", status, 0 );
    }

    if( status & JBIG_JSTAT_DD_MASK ) // Decode is done.
    {
    	DBG_MEMLOG(LOG_DEBUG_M,"  isrJbigControl: decoder completed successfully.\n" );
    	/* we need to clear the done bit */
    	REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_DD_MASK );
    }

    if( status & JBIG_JSTAT_ED_MASK ) // Encode is done.
    {
    	DBG_MEMLOG(LOG_DEBUG_M,"  isrJbigControl: encoder completed successfully.\n" );
    	/* we need to clear the done bit */
    	REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_ED_MASK );

    }

    if( status & JBIG_JSTAT_I_MASK) // if these bits are set then a Write or Read DMA has completed and is pending
    {
    	if( (status & JBIG_JSTAT_I_MASK) == 0x4000 )
    	{
    	   DBG_MEMLOG(LOG_DEBUG_M,"  isrJbigControl: Write DMA Complete.\n" );
    	}
    	if( (status & JBIG_JSTAT_I_MASK) == 0x8000 )
    	{
    	   DBG_MEMLOG(LOG_DEBUG_M,"  isrJbigControl: Read DMA Complete.\n" );
    	}
    }

    if( status & JBIG_JSTAT_D_MASK ) // Decode/Encode is done.
    {
        DBG_PRINTF(LOG_DEBUG_M,"  isrJbigControl: JBIG DONE.\n" );

        /* we need to clear the done bit */
        REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_D_MASK );

        // clear the enable bit in the ODMA to cause it to complete as described in the programming guide.
        // NOTE that this will also cause the odma to not clear the ownership bit for the final descriptor.
        p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );

        currentHandle->control_status = errorStatus;
        jbigMsg.handle = currentHandle;
        jbigMsg.msgType = JBIG_MSG_OPERATION_COMPLETE;
        posix_message_send(jbig_msg_queue, (char*)&jbigMsg, sizeof(JBIG_MESSAGE), MQ_DEFAULT_PRIORITY, 0);
    }

    pthread_mutex_unlock(&currentHandle->lock);

}

/**
 * isrJbigIDMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
#ifdef __linux__
static void isrJbigIDMA( int32_t interrupt_count, void *context )
#else
static void isrJbigIDMA( uint32_t context )
#endif
{

    tJBIGDMADescriptor * pDescriptor;
    jbig_handle_t * currentHandle;
    uint32_t result;
    JBIG_MESSAGE jbigMsg;
    mqd_t* msgDest = &jbig_msg_queue;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs = NULL;
    JBIG_REGS_t * p_JBIG_control_regs = NULL;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    uint32_t status = 0;
    uint32_t odma_status = 0;

    uint32_t pendingInterrupts;

    currentHandle = (jbig_handle_t*)context;
    ASSERT(currentHandle != NULL);
    
    pthread_mutex_lock(&currentHandle->lock);
        
    p_JBIG_IDMA_regs = currentHandle->jbig_block_interface->jbig_idma_regs;
    p_JBIG_control_regs = currentHandle->jbig_block_interface->jbig_regs;
    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    status = p_JBIG_control_regs->JSTAT;
    odma_status = p_JBIG_ODMA_regs->status;

    pDescriptor = (tJBIGDMADescriptor *)p_JBIG_IDMA_regs->desc_read;
    pDescriptor = jbig_get_desc_vaddr_from_list(&currentHandle->currentReadDescriptorList, pDescriptor);

    jbigMsg.descriptor = 0;
    jbigMsg.handle = currentHandle;

    jbigMsg.descriptor = pDescriptor;

    result = p_JBIG_IDMA_regs->status;

    pendingInterrupts = p_JBIG_IDMA_regs->int_st;

    DBG_PRINTF_INFO("JBIG: isrJbigIDMA currentHandle = %#x pending %#x control stat = %#x idma stat = %#x odma status = %#x\n", currentHandle, pendingInterrupts, status, result,odma_status );

    // Error Ownership bit was not set on descriptor
    if( pendingInterrupts & IDMA_SLI_INT_ST_WHO_INT_ST_MASK)
    {
    	DBG_MEMLOG(LOG_WARNING,"isrJbigIDMA: Ownership Error!\n" );
    }

    // Finished a descriptor
    if( pendingInterrupts & IDMA_SLI_INT_ST_FIN_INT_ST_MASK)
    {
    	pDescriptor = (tJBIGDMADescriptor *)p_JBIG_IDMA_regs->desc_read;
        DBG_PRINTF_INFO("    desc_read = %#x, xfer_addr = %#x, xfer_len = %d, status = %#x, next = %08x\n", pDescriptor, p_JBIG_IDMA_regs->xfer_addr, p_JBIG_IDMA_regs->xfer_length, p_JBIG_IDMA_regs->status);
        if(currentHandle)
        {
            DBG_PRINTF_INFO("    read chain list head = %08x\n", currentHandle->currentReadDescriptorList.ListHead);
        }
    	DBG_MEMLOG(LOG_DEBUG_M,"            desc_read = %#x\n", pDescriptor);
    	DBG_MEMLOG(LOG_DEBUG_M,"            xfr_addr = %#x\n",p_JBIG_IDMA_regs->xfer_addr);
    	DBG_MEMLOG(LOG_DEBUG_M,"            xfr_length = %#x\n",p_JBIG_IDMA_regs->xfer_length);
    	DBG_MEMLOG(LOG_DEBUG_M,"            status = %#x\n",p_JBIG_IDMA_regs->status);

        jbigMsg.descriptor = pDescriptor;
        jbigMsg.msgType = JBIG_MSG_IDMA_COMPLETE;
        posix_message_send(*msgDest, (char*)&jbigMsg, sizeof(JBIG_MESSAGE),MQ_DEFAULT_PRIORITY, 0);
    }

    // End of Image
    if( pendingInterrupts & IDMA_SLI_INT_ST_EOI_INT_ST_MASK)
    {
    	DBG_MEMLOG(LOG_DEBUG_M,"             End Of Image\n" );
    }

    // Not sure what RST is at this time
    if( pendingInterrupts & IDMA_SLI_INT_ST_RST_INT_ST_MASK)
    {

    }

    REG_SET_BITS(&p_JBIG_IDMA_regs->int_cl, pendingInterrupts);
    pthread_mutex_unlock(&currentHandle->lock);    
}

/**
 * isrJbigODMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
static uint8_t waitForODMAResetBit = 0;
#ifdef __linux__
static void isrJbigODMA( int32_t interrupt_count, void *context )
#else
static void isrJbigODMA( uint32_t context )
#endif
{
	
	uint32_t pendingInterrupts;
	JBIG_MESSAGE jbigMsg;
    mqd_t* msgDest = &jbig_msg_queue;

    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
	tJBIGDMADescriptor * pDescriptor;
	jbig_handle_t * currentHandle;

    currentHandle = (jbig_handle_t*)context;
    ASSERT(currentHandle != NULL);

    pthread_mutex_lock(&currentHandle->lock);

    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    pendingInterrupts = p_JBIG_ODMA_regs->int_pend;
    pDescriptor = (tJBIGDMADescriptor *)p_JBIG_ODMA_regs->desc_read;
    pDescriptor = jbig_get_desc_vaddr_from_list(&currentHandle->currentWriteDescriptorList, pDescriptor);

    DBG_PRINTF_INFO("JBIG: isrJbigODMA: Pending Int %#x status = %#x currentHandle = %#x\n", pendingInterrupts, p_JBIG_ODMA_regs->status, currentHandle);

    DBG_PRINTF(LOG_DEBUG_M,"JBIG: isrJbigODMA: Pending Int %#x status = %#x\n", pendingInterrupts, p_JBIG_ODMA_regs->status );
    DBG_MEMLOG(LOG_DEBUG_M,"                 currentHandle = %#x\n", currentHandle );

    // the following values are for current descriptor not necessarily the one that
    // caused the interrupt to fire.
    DBG_MEMLOG(LOG_DEBUG_M,"                 xfr_addr = %#x\n",p_JBIG_ODMA_regs->xfer_addr );
    DBG_MEMLOG(LOG_DEBUG_M,"                 xfr_length = %#x\n",p_JBIG_ODMA_regs->xfer_length );
    DBG_MEMLOG(LOG_DEBUG_M,"                 status = %#x\n",p_JBIG_ODMA_regs->status );
    DBG_PRINTF(LOG_DEBUG_M,"                 desc_read = %#x\n", pDescriptor);
    if( pDescriptor)
    {
        DBG_PRINTF(LOG_DEBUG_M,"                 desc_next = %#x\n", pDescriptor->next_desc_virt_addr);
        if(pDescriptor->next_desc_virt_addr)
        {
            DBG_PRINTF(LOG_DEBUG_M,"                 desc_next_control = %#x\n", pDescriptor->next_desc_virt_addr->control);
        }
    }


    jbigMsg.descriptor = 0;
    jbigMsg.handle = currentHandle;
        
    // Error Ownership bit was not set on descriptor
    if( pendingInterrupts & ODMA_SLI_INT_PEND_WHO_INT_PEND_MASK)
    {
        DBG_MEMLOG(LOG_DEBUG_H,"                 xfr_addr = %#x\n",p_JBIG_ODMA_regs->xfer_addr );
        DBG_MEMLOG(LOG_DEBUG_H,"                 xfr_length = %#x\n",p_JBIG_ODMA_regs->xfer_length );
        DBG_MEMLOG(LOG_DEBUG_H,"                 status = %#x\n",p_JBIG_ODMA_regs->status );
        DBG_MEMLOG(LOG_DEBUG_H,"                 desc_read = %#x\n", pDescriptor);
        DBG_MEMLOG(LOG_DEBUG_H,"                 desc_ctrl = %#x\n", pDescriptor->control);
    }

    if( pendingInterrupts & ODMA_SLI_INT_PEND_RST_INT_PEND_MASK)
    {
        waitForODMAResetBit = 1;
    }

    // Finished a descriptor
    // End of Image
    if( pendingInterrupts & (ODMA_SLI_INT_PEND_EOI_INT_PEND_MASK | ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK ) )
    {
        DBG_PRINTF_DEBUG_M("ODMA_SLI_INT_PEND_EOI_INT_PEND_MASK\n");
        // Force clear ownership bit of last descriptor.  It may be left set due to disable of odma output to complete
        // the transaction.
        ASSERT(pDescriptor != NULL);
        pDescriptor->control &= ~(JBIG_DESCRIPTOR_OWNERSHIP);
        pDescriptor->length -= p_JBIG_ODMA_regs->xfer_length;  // adjust length of last descriptor based on what was actually transferred
        jbigMsg.descriptor = pDescriptor;
        jbigMsg.msgType = JBIG_MSG_ODMA_EOI;
        posix_message_send(*msgDest, (char*)&jbigMsg, sizeof(JBIG_MESSAGE),MQ_DEFAULT_PRIORITY, 0);
    }
    else if( pendingInterrupts & ( ODMA_SLI_INT_PEND_FIN_INT_PEND_MASK ) )
    {
        DBG_PRINTF_DEBUG_M("ODMA_SLI_INT_PEND_FIN_INT_PEND_MASK\n");
        jbigMsg.descriptor = pDescriptor;        
        
        jbigMsg.msgType = JBIG_MSG_ODMA_COMPLETE;
        posix_message_send(*msgDest, (char*)&jbigMsg, sizeof(JBIG_MESSAGE),MQ_DEFAULT_PRIORITY, 0);
    }
    

    REG_SET_BITS(&p_JBIG_ODMA_regs->int_ack, pendingInterrupts);
    pthread_mutex_unlock(&currentHandle->lock);
}


/**
 * jbigResetHardware
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigResetHardware( jbig_handle_t * handle )
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    
    ASSERT(handle != NULL);
    p_JBIG_regs = handle->jbig_block_interface->jbig_regs;

    DBG_MEMLOG(LOG_DEBUG_M,"%s core status = %#x\n",__func__, p_JBIG_regs->JSTAT);

    jbigResetWriteDMA( handle );
    jbigResetReadDMA( handle );

    p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL(p_JBIG_regs->JBIG_EN, 0); // disable block

    p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL(p_JBIG_regs->JBIG_EN, 1); // reset block
    p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL(p_JBIG_regs->JBIG_EN, 0);

    p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL(p_JBIG_regs->JBIG_EN, 0); // disable block
}

/**
 * jbigFreeMemory
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigFreeMemory( jbig_handle_t * handle )
{
    JBIG_MESSAGE jbigMsg;
    // We use an event flag here so that in the event that jbig close is
    // called in an interrupt context, which it is from video. We can still
    // free memory.    
    
    jbigMsg.msgType = JBIG_MSG_FREE_MEMORY;
    jbigMsg.handle = handle;
    posix_message_send(jbig_msg_queue, (char*)&jbigMsg, sizeof(JBIG_MESSAGE),MQ_DEFAULT_PRIORITY, 0);
}

/**
 * jbigFreeDescriptors
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

static void jbigFreeDescriptors( void )
{
    int16_t handleIndex = 0;
    jbig_handle_t * handle;
    	
    DBG_SLOG_ENTRY
	
    for( handleIndex = 0; handleIndex < NUM_JBIG_BLOCKS; handleIndex++ )
    {
        handle = &jbig_handles[handleIndex];

        jbigFreeHandleDescriptors(handle);
    }
    DBG_SLOG_EXIT
}

/**
 * jbigFreeHandleDescriptors
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigFreeHandleDescriptors( jbig_handle_t * handle )
{
    tJBIGDMADescriptor * tmpDescriptor = NULL;
    	
    DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Free Descriptors for handle %#x state = %d\n",handle, handle->started);
	        
    if( !handle->started && handle->dirty )
    {        
        while( jbigGetDescriptorListHead( handle->pendingReadDescriptorList ) )
        {
            
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Free Pending Read Descriptor %#x\n",jbigGetDescriptorListHead( handle->pendingReadDescriptorList ));
            tmpDescriptor = jbigRemoveDescriptorFromListHead( &handle->pendingReadDescriptorList );
            MEM_FREE_UNCACHED( tmpDescriptor->hw_addr, tmpDescriptor );
        }
        
        while(jbigGetDescriptorListHead( handle->currentReadDescriptorList ))
        {
            
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Free Current Read Descriptor %#x\n",jbigGetDescriptorListHead( handle->currentReadDescriptorList ));
            tmpDescriptor = jbigRemoveDescriptorFromListHead( &handle->currentReadDescriptorList );
            MEM_FREE_UNCACHED( tmpDescriptor->hw_addr, tmpDescriptor );
        }            
        
        while( jbigGetDescriptorListHead( handle->pendingWriteDescriptorList ) )
        {
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Free Pending Write Descriptor %#x\n",jbigGetDescriptorListHead( handle->pendingWriteDescriptorList ));
            tmpDescriptor = jbigRemoveDescriptorFromListHead( &handle->pendingWriteDescriptorList );
            MEM_FREE_UNCACHED( tmpDescriptor->hw_addr, tmpDescriptor );
        }
        
        while( jbigGetDescriptorListHead( handle->currentWriteDescriptorList ))
        {
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Free Current Write Descriptor %#x\n",jbigGetDescriptorListHead( handle->currentWriteDescriptorList ));
            tmpDescriptor = jbigRemoveDescriptorFromListHead( &handle->currentWriteDescriptorList );
            MEM_FREE_UNCACHED( tmpDescriptor->hw_addr, tmpDescriptor );
        } 
                    
        handle->dirty = 0;
    }

}


/**
 * jbigStart
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigStart( jbig_handle_t * inHandle )
{    
    JBIG_REGS_t * p_JBIG_regs = NULL;
    
    if( inHandle )
    {
        jbig_increment_pending_messages(inHandle);
        inHandle->linesRemaining = inHandle->config.Yd;
        enableJBIGInterrupts( inHandle );
            
        p_JBIG_regs = (JBIG_REGS_t *) inHandle->jbig_block_interface->jbig_regs;
    
        if( inHandle->config.Operation == JBIG_DECODE )
        {
            DBG_MEMLOG(LOG_DEBUG_M,"JBIG: Start DECODE\n");
            p_JBIG_regs->JCTL = JBIG_JCTL_E_REPLACE_VAL(p_JBIG_regs->JCTL, 0);
            p_JBIG_regs->JCTL = JBIG_JCTL_D_REPLACE_VAL(p_JBIG_regs->JCTL, 1);
        }
        else
        {
            DBG_MEMLOG(LOG_DEBUG_M,"JBIG: Start ENCODE\n");
            p_JBIG_regs->JCTL = JBIG_JCTL_E_REPLACE_VAL(p_JBIG_regs->JCTL, 1);
            p_JBIG_regs->JCTL = JBIG_JCTL_D_REPLACE_VAL(p_JBIG_regs->JCTL, 0);
        }
        
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 1 ); // enable the jbig block        
	}
}

/**
 * jbigStop
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/
 
void jbigStop( jbig_handle_t * inHandle )
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs = NULL;
    
    DBG_SLOG_ENTRY
    
    if( inHandle )
    {
        inHandle->jbigCallback = NULL;

        disableJBIGInterrupts( inHandle );

        p_JBIG_regs = (JBIG_REGS_t *) inHandle->jbig_block_interface->jbig_regs;
        p_JBIG_IDMA_regs = inHandle->jbig_block_interface->jbig_idma_regs;
        p_JBIG_ODMA_regs = inHandle->jbig_block_interface->jbig_odma_regs;
        
        DBG_MEMLOG(LOG_DEBUG_M,"%s core status = %#x\n",__func__, p_JBIG_regs->JSTAT);
        p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_IDMA_regs->cfg, 0 );
        p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );
        
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 ); // disable the jbig block
                                                                                     
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 1 ); // reset the block
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 );

        p_JBIG_regs->JSTAT = 0;

        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 ); // disable the jbig block
    }	
    DBG_SLOG_EXIT
}



/**
 * jbigResetWriteDMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigResetWriteDMA( jbig_handle_t * handle )
{
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs;
    IDMA_SLI_REGS_t* p_JBIG_IDMA_regs;
    uint32_t loopCount = 0;
	DBG_MEMLOG(LOG_DEBUG,"jbig: jbigResetWriteDMA\n");

    DBG_SLOG_ENTRY
    ASSERT(handle != NULL);

    waitForODMAResetBit = 0;

    p_JBIG_ODMA_regs= handle->jbig_block_interface->jbig_odma_regs;
    p_JBIG_IDMA_regs = handle->jbig_block_interface->jbig_idma_regs;


    DBG_MEMLOG(LOG_DEBUG_M,"jbigODMA: status = %#x\n", p_JBIG_ODMA_regs->status);

   // while(1 == (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK) &&( loopCount++ <1000) );
    // it is not uncommon for the dma busy flag to be set on write dma if the buffer length given is larger than decompresse/compressed data
    // provided by the IDMA.


   // p_JBIG_ODMA_regs->int_ack = 0xFF/*JBIG_DMA_INT_MASK */;


    //while((ODMA_SLI_STATUS_EMPTY_DBUF_MASK | ODMA_SLI_STATUS_EMPTY_CBUF_MASK) != (p_JBIG_ODMA_regs->status & (ODMA_SLI_STATUS_EMPTY_DBUF_MASK | ODMA_SLI_STATUS_EMPTY_CBUF_MASK)) && ( loopCount++ <1000) )
    while(ODMA_SLI_STATUS_EMPTY_CBUF_MASK != (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_EMPTY_CBUF_MASK) && ( loopCount++ <1000) )
    {
        DBG_MEMLOG(LOG_DEBUG_H,"jbigODMA: status = %#x\n", p_JBIG_ODMA_regs->status);
    }


    p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 1);
	p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 0);


    DBG_MEMLOG(LOG_DEBUG_M,"jbigODMA: after reset status = %#x\n", p_JBIG_ODMA_regs->status);

    /* Wait for the DMA operations to shutdown. This needs to happen
     * before the DMA can be safely used again.
     */
    
    loopCount = 0;

    while(( waitForODMAResetBit == 0 ) && ( loopCount++ < 2 ))
    {
        // just in case someone has interrupts disabled check the bit directly here
        if( p_JBIG_ODMA_regs->int_pend & ODMA_SLI_INT_PEND_RST_INT_PEND_MASK )
        {
            break;
        }
        DBG_MEMLOG(LOG_WARNING,"jbigODMA: waiting for reset %d status %#x\n",loopCount,p_JBIG_ODMA_regs->status);
        posix_sleep_ms(10);
    }

    p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(p_JBIG_ODMA_regs->cfg, 0);
    p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_UPPER_HALF_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );

    p_JBIG_ODMA_regs->int_ack = 0xFF/*JBIG_DMA_INT_MASK */;

    DBG_MEMLOG(LOG_DEBUG_M,"jbigIDMA: status = %#x\n", p_JBIG_IDMA_regs->status);
    DBG_MEMLOG(LOG_DEBUG_M,"jbigODMA: status = %#x\n", p_JBIG_ODMA_regs->status);
	
    DBG_SLOG_EXIT
}


/**
 * jbigResetReadDMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigResetReadDMA( jbig_handle_t * handle )
{
    IDMA_SLI_REGS_t* p_JBIG_IDMA_regs;
    uint32_t loopCount = 0;

    DBG_SLOG_ENTRY

    ASSERT(handle != NULL);

    p_JBIG_IDMA_regs = handle->jbig_block_interface->jbig_idma_regs;

    DBG_MEMLOG(LOG_DEBUG_M,"jbigIDMA: cfg = %#x \n\t int_en %#x \n\tint_pend %#x \n\tint_ack %#x\n\tstatus = %#x\n", p_JBIG_IDMA_regs->cfg, p_JBIG_IDMA_regs->int_en,
                                                                                                   p_JBIG_IDMA_regs->int_st,p_JBIG_IDMA_regs->int_cl,
                                                                                                   p_JBIG_IDMA_regs->status);


    while(1 == (p_JBIG_IDMA_regs->status & IDMA_SLI_STATUS_DMA_BUSY_MASK) &&( loopCount++ <1000) );


    p_JBIG_IDMA_regs->reset = IDMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_IDMA_regs->reset, 1);
    p_JBIG_IDMA_regs->reset = IDMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_IDMA_regs->reset, 0);


    p_JBIG_IDMA_regs->int_cl = 0xFF;

    p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_ENABLE_REPLACE_VAL(p_JBIG_IDMA_regs->cfg, 0);

    DBG_SLOG_EXIT
}

/**
 * isJbigStarted
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

static uint8_t isJbigStarted( jbig_handle_t * inHandle )
{
    uint8_t started = 0;
    error_type_t startederror = jbig_started(inHandle, &started );

    if ( startederror == OK )
    {
    	return started;
    }
    else
    	return 0;
}



/**
 * jbigAddReadDMABuffer
 * 
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

error_type_t jbigAddReadDMABuffer( jbig_handle_t * inHandle, BigBuffer_t* buf, uint32_t size, void * userData )
{

    error_type_t returnValue = FAIL;
    jbig_dma_context_t* dma_context = NULL;
    
    if( inHandle && inHandle->inUse )
    {
        if( inHandle->numPendingMessages < inHandle->maxNumMessages )
        {
            dma_context = (jbig_dma_context_t*)MEM_MALLOC(sizeof(jbig_dma_context_t));
            ASSERT(dma_context);
            if (dma_context)
            {
                dma_context->buf = buf;;
                dma_context->len = size;
                dma_context->user_callback_data = userData;
                jbigAddDMABuffer( inHandle, &inHandle->pendingReadDescriptorList, buf, size, dma_context, DMA_TO_DEVICE);
                returnValue = OK;
            }
        }
        else
        {
            DBG_MEMLOG(LOG_WARNING,"JBIG: too many buffers !!!!! %d\n",inHandle->numPendingMessages);
        }

        if( jbigStartReadDMAIfPossible( inHandle ) == OK )
        {
            DBG_MEMLOG(LOG_DEBUG_H,"JBIG: Read DMA Start Now\n");
        }
    
    }
    
    return ( returnValue );
}


/**
 * jbigStartReadDMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigStartReadDMA( jbig_handle_t * inHandle )
{

    if( inHandle )
    {
    	DBG_MEMLOG(LOG_INFO,"JBIG: start chained Read DMA\n");

        jbigStartReadDMAIfPossible( inHandle );
    }
}

/**
 * jbigStartWriteDMA
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

void jbigStartWriteDMA( jbig_handle_t * inHandle )
{
    if( inHandle )
    {
        // enable the DMA
    	DBG_MEMLOG(LOG_INFO,"JBIG: start chained Write DMA\n");
        
        jbigStartWriteDMAIfPossible( inHandle );
    }
}

/**
 * jbigAddWriteDMABuffer
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/

error_type_t jbigAddWriteDMABuffer( jbig_handle_t * inHandle, BigBuffer_t* buf, uint32_t size, void * userData )
{
    error_type_t returnValue = FAIL;
    jbig_dma_context_t* dma_context = NULL;
        
    DBG_SLOG_ENTRY

    if( inHandle )
    {
        if( inHandle->numPendingMessages < inHandle->maxNumMessages )
        {
            DBG_MEMLOG(LOG_DEBUG_H, "%s user data 0x%#x, buf 0x%#x size = %d\n", __func__, userData, buf, size);
            dma_context = (jbig_dma_context_t*)MEM_MALLOC(sizeof(jbig_dma_context_t));
            ASSERT(dma_context);
            if (dma_context)
            {
                dma_context->len = size;
                dma_context->buf = buf;
                dma_context->user_callback_data = userData;
                
                returnValue = jbigAddDMABuffer( inHandle, &inHandle->pendingWriteDescriptorList, buf, size, dma_context, DMA_FROM_DEVICE);
            }
        }
        else
        {
            DBG_MEMLOG(LOG_WARNING,"JBIG: too many write buffers !!!!! %d %d\n", inHandle->numPendingMessages, inHandle->maxNumMessages);
        }

        if( jbigStartWriteDMAIfPossible( inHandle ) == OK )
        {
            DBG_MEMLOG(LOG_INFO,"JBIG: Write DMA Start Now\n");
        }        

    }
    
    DBG_SLOG_EXIT

    return ( returnValue );
}
 

/** 
 * jbigCommitConfig
 *
 * \brief Commit current JBIG configuration to the JBIG hardware block
 * 
 * \param handle The handle of the current JBIG session
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will set the appropriate configuration bits based on
 * the configuration variables store in the the given sessions 
 * JBIG_CONFIG structure
 **/

error_type_t jbigCommitConfig(jbig_handle_t *handle)
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs = NULL;
	error_type_t return_result = FAIL;

    ASSERT(handle != NULL);
	if( handle )
	{
        // get the register interfaces
        p_JBIG_regs = handle->jbig_block_interface->jbig_regs;
        p_JBIG_ODMA_regs = handle->jbig_block_interface->jbig_odma_regs;

        DBG_MEMLOG(LOG_DEBUG_M,"%s core status = %#x\n",__func__, p_JBIG_regs->JSTAT);

        // set TPBON
        p_JBIG_regs->JCTL = JBIG_JCTL_T_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.TpbonState);

        //setup ATMOV
        p_JBIG_regs->JCTL = JBIG_JCTL_A_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.AtmovState);
        if( handle->config.AtmovState == ATMOV_ENABLED )
        {
            p_JBIG_regs->JCTL = JBIG_JCTL_M_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.ATMOVvalue);
		}
        else
        {
            p_JBIG_regs->JCTL = JBIG_JCTL_M_REPLACE_VAL( p_JBIG_regs->JCTL, 0);
        }        

        // sanity check
        ASSERT(handle->config.Xd != 0);
        ASSERT(handle->config.Yd != 0);
        ASSERT(handle->config.Ld != 0);
        ASSERT(handle->config.LineWidth != 0);

        // Set image dimensions
        p_JBIG_regs->JX = handle->config.Xd;
        p_JBIG_regs->JY = handle->config.Yd;
        p_JBIG_regs->JSL = handle->config.Ld;

        // set 3Line
        p_JBIG_regs->JCTL = JBIG_JCTL_L_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.LineTemplate);

        // set BPEND
        p_JBIG_regs->JCTL = JBIG_JCTL_BE_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.Endian);

        // set BYPASS CORE
        p_JBIG_regs->JCTL = JBIG_JCTL_B_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.BypassCore);

        // Enable CBI
        p_JBIG_regs->JCTL = JBIG_JCTL_C_REPLACE_VAL( p_JBIG_regs->JCTL, handle->config.CBI);

        p_JBIG_ODMA_regs->line_size = handle->config.LineWidth;

        return_result = OK;
	}
	return return_result;
}


void jbig_dump_control_regs(jbig_handle_t *handle)
{
#ifdef HAVE_DBG_PRINTF
    JBIG_REGS_t * jbig_regs = handle->jbig_block_interface->jbig_regs;
    DBG_PRINTF_CRIT("JBIG Control Reg Dump:\n");
    DBG_PRINTF_CRIT("\tJCTL    = %#x\n", jbig_regs->JCTL);
    DBG_PRINTF_CRIT("\tJSTAT   = %#x\n", jbig_regs->JSTAT);
    DBG_PRINTF_CRIT("\tJX      = %#x\n", jbig_regs->JX);
    DBG_PRINTF_CRIT("\tJY      = %#x\n", jbig_regs->JY);
    DBG_PRINTF_CRIT("\tJBIG_EN = %#x\n", jbig_regs->JBIG_EN);
    DBG_PRINTF_CRIT("\tJSL     = %#x\n", jbig_regs->JSL);
#endif
}

void jbig_dump_odma_regs(jbig_handle_t *handle)
{
#ifdef HAVE_DBG_PRINTF
    ODMA_SLI_REGS_t* odma_regs = handle->jbig_block_interface->jbig_odma_regs;
    DBG_PRINTF_CRIT("JBIG ODMA Reg Dump:\n");
    DBG_PRINTF_CRIT("\tcfg = %#x\n", odma_regs->cfg);
    DBG_PRINTF_CRIT("\tstatus = %#x\n", odma_regs->status);
    DBG_PRINTF_CRIT("\tline_size = %#x\n", odma_regs->line_size);
    DBG_PRINTF_CRIT("\tint_en = %#x\n", odma_regs->int_en);
    DBG_PRINTF_CRIT("\tint_pend = %#x\n", odma_regs->int_pend);
    DBG_PRINTF_CRIT("\tint_ack = %#x\n", odma_regs->int_ack);
    DBG_PRINTF_CRIT("\tdesc_write = %#x\n", odma_regs->desc_write);
    DBG_PRINTF_CRIT("\tdesc_read = %#x\n", odma_regs->desc_read);
    DBG_PRINTF_CRIT("\txfer_length = %#x\n", odma_regs->xfer_length);
    DBG_PRINTF_CRIT("\txfer_addr = %#x\n", odma_regs->xfer_addr);
    DBG_PRINTF_CRIT("\txfer_burst = %#x\n", odma_regs->xfer_burst);
    DBG_PRINTF_CRIT("\treset = %#x\n", odma_regs->reset);
#endif
}

void jbig_dump_idma_regs(jbig_handle_t *handle)
{
#ifdef HAVE_DBG_PRINTF
    IDMA_SLI_REGS_t* idma_regs = handle->jbig_block_interface->jbig_idma_regs;
    DBG_PRINTF_CRIT("JBIG IDMA Reg Dump:\n");
    DBG_PRINTF_CRIT("\tcfg = %#x\n", idma_regs->cfg);
    DBG_PRINTF_CRIT("\tstatus = %#x\n", idma_regs->status);
    DBG_PRINTF_CRIT("\tline_width = %#x\n", idma_regs->line_width);
    DBG_PRINTF_CRIT("\tint_en = %#x\n", idma_regs->int_en);
    DBG_PRINTF_CRIT("\tint_pend = %#x\n", idma_regs->int_st);
    DBG_PRINTF_CRIT("\tint_ack = %#x\n", idma_regs->int_cl);
    DBG_PRINTF_CRIT("\tdesc_write = %#x\n", idma_regs->desc_write);
    DBG_PRINTF_CRIT("\tdesc_read = %#x\n", idma_regs->desc_read);
    DBG_PRINTF_CRIT("\txfer_length = %#x\n", idma_regs->xfer_length);
    DBG_PRINTF_CRIT("\txfer_addr = %#x\n", idma_regs->xfer_addr);
    DBG_PRINTF_CRIT("\tctrl_word = %#x\n", idma_regs->ctrl_word);
    DBG_PRINTF_CRIT("\treset = %#x\n", idma_regs->reset);
#endif
}

/**
 * \brief Debug routine to dump all jbig registers for the blocks associated with the given handle
 */
void jbig_dump_all_regs(jbig_handle_t * handle)
{
    jbig_dump_control_regs(handle);
    jbig_dump_odma_regs(handle);
    jbig_dump_idma_regs(handle);   
}

#ifdef HAVE_POWER_MGR
#define PWR_MGR_MY_NAME  "jbig"

static uint32_t pwr_mgr_id;

static void pwr_on(void)
{
    uint32_t block_count;

    for(block_count = 0; block_count < NUM_JBIG_BLOCKS; block_count++)
    {
        jbig_block_init(jbig_handles[block_count].jbig_block_interface);
    }
}
static void pwr_off(void)
{
}
static bool jbig_power_save(pwr_mgr_level_t level, pwr_mgr_cmd_t cmd)
{
    static pwr_mgr_level_t pwr_level = pwr_mgr_on_e;
    switch(level)
    {
    case pwr_mgr_on_e:
        if (pwr_level == pwr_mgr_off_e)
        {
            pwr_on();
        }
        pwr_mgr_powered_up(pwr_mgr_id);
        pwr_level = level;
        break;
    case pwr_mgr_reduced_power_e:
    case pwr_mgr_lowest_power_e:
        ASSERT(pwr_level == pwr_mgr_on_e);
        pwr_mgr_powered_down(pwr_mgr_id);
        pwr_level = level;
        break;
    case pwr_mgr_off_e:
        ASSERT(pwr_level == pwr_mgr_on_e);
        pwr_off();
        pwr_mgr_powered_down(pwr_mgr_id);
        pwr_level = level;
        break;
    default:
        break;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// jbig_pwr_init
//
//    Inputs:  
//    Returns: 
//
////////////////////////////////////////////////////////////////////////////////
void jbig_pwr_init(void)
{
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("%s()\r\n",__FUNCTION__));

    pwr_mgr_id = pwr_mgr_reg_callback(PWR_MGR_MY_NAME, jbig_power_save, PWRMGR_JBIG_PRIORITY);

    // temp set pwr level here till unity get AOAO schema mgr
    pwr_mgr_set_module_pwr_level(PWR_MGR_MY_NAME, PWRMGR_JBIG_LEVEL);   

    return;
} 
#endif

