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


#include "dros.h"
#include "lassert.h"
#include "asic.h"
#include "interrupt_api.h"
#include "error_types.h"
#include "jbig_decode_api.h"
#include "jbig_decode_utils.h"
#include "memAPI.h"

#ifdef ASIC_gemstone

#if ASIC_REV < ASIC_REV_Z1
#error "Unsupported gemstone rev"
#endif

#elif defined(ASIC_SS1000)
// all revs of SS1000 supported
#elif defined(ASIC_basalt)
// all revs of basalt supported
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

#ifdef __KERNEL__
#define DBG_PRFX "JBIGD: "
//#define DBG_SLOG_ENTRY printk(KERN_DEBUG DBG_PRFX "==> %s\n", __func__);
//#define DBG_SLOG_EXIT printk(KERN_DEBUG DBG_PRFX "<== %s\n", __func__);

//#define MEMLOG(...) printk(KERN_DEBUG DBG_PRFX __VA_ARGS__)
#define DBG_SLOG_ENTRY
#define DBG_SLOG_EXIT
#define MEMLOG(...)

#define ERRLOG(...) printk(KERN_ERR DBG_PRFX __VA_ARGS__)

#else
#define LOGGER_MODULE_MASK (DEBUG_LOGGER_MODULE_JBIG | LOGGER_JBIG_UTILS)  // pick a module from logger.h 
#define DBG_SLOG_ENTRY DBG_MEMLOG(LOG_DEBUG_H,"%s ENTRY\n", __func__);
#define DBG_SLOG_EXIT DBG_MEMLOG(LOG_DEBUG_H,"%s EXIT\n", __func__);

#define MEMLOG(...) DBG_MEMLOG(LOG_DEBUG_H, __VA_ARGS__);

#endif
 
#ifdef __KERNEL__
static irqreturn_t isrJbigIDMA(int32_t interrupt_count, void *context);
static irqreturn_t isrJbigODMA(int32_t interrupt_count, void *context);
static irqreturn_t isrJbigControl(int32_t interrupt_count, void *context);
#else
static void isrJbigIDMA( void *context );
static void isrJbigODMA( void *context );
static void isrJbigControl( void *context );
#endif

static void enableJBIGInterrupts(jbig_decode_handle_t * inHandle );
static void disableJBIGInterrupts(jbig_decode_handle_t * inHandle );

//-jrsstatic void jbigFreeDescriptors( void );

extern void jbig_init(void);
extern void jbig_decode_set_last_error_code( uint32_t errorCode );

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
static int jbig_callback(int argc, char *argv[]);
#endif

#ifdef HAVE_POWER_MGR
static void jbig_pwr_init(void);
#endif

// pointers to the current transaction. At this time we only allow
// one transaction per JBIG block
static jbig_decode_handle_t * CurrentDecodeTransaction;

jbig_decode_handle_t jbig_handles[NUM_JBIG_BLOCKS];
static dros_smp_lock_t jbig_smplock;


#define JBIG_WAIT_LOOP_MAX 100

/**
 * jbig_block_init - modularize jbigInit, to allow pwrmgr to 
 * reinit hardware. 
 * 
 * @author  (10/13/2011)
 * 
 * @param jbig_block_interface 
 */
void jbig_block_init(const jbig_decode_block_config_t* jbig_block_interface)
{
    MEMLOG("JBIG BLOCK INIT FROM POWER MANAGER!!!\n");
    //jbig_block_interface->jbig_regs->JCTL = 0;
    jbig_block_interface->jbig_idma_regs->int_cl = 0x1FF;
    jbig_block_interface->jbig_odma_regs->int_ack = 0xFFF;
    jbig_block_interface->jbig_idma_regs->int_en = 0x1FF/*JBIG_DMA_INT_MASK*/;
    jbig_block_interface->jbig_odma_regs->int_en = 0x1FF/*JBIG_DMA_INT_MASK*/;
}

void jbig_free_desc_block(jbig_dma_descriptor_block_t* desc_block)
{
    int i;
    for(i = 0; i < NUM_JBIG_DMA_DESCS_PER_BLOCK; i++)
    {
        if(desc_block->jbig_descs[i] != NULL)
        {
            MEM_FREE_UNCACHED(desc_block->jbig_descs[i]->hw_address, desc_block->jbig_descs[i]);
        }
    }
    memset(desc_block, 0, sizeof(jbig_dma_descriptor_block_t));
}

error_type_t jbig_alloc_desc_block(jbig_dma_descriptor_block_t* desc_block)
{
    int i;
    void* hw_addr;

    memset(desc_block, 0, sizeof(jbig_dma_descriptor_block_t));

    // allocate and init all descriptors for this channel; the dma_next_address field
    // is initialized to form a circular list of descriptors
    for(i = 0; i < NUM_JBIG_DMA_DESCS_PER_BLOCK; i++)
    {
        desc_block->jbig_descs[i] = (jbig_dma_desc_t *)MEM_MALLOC_UNCACHED((void*)&hw_addr, sizeof(jbig_dma_desc_t), e_32_byte);
        if(desc_block->jbig_descs[i] == NULL)
        {
            goto ERROR;
        }
        memset(desc_block->jbig_descs[i], 0, sizeof(jbig_dma_desc_t));
        desc_block->jbig_descs[i]->hw_address = hw_addr;
        if(i > 0)
        {
            desc_block->jbig_descs[i-1]->next_desc = desc_block->jbig_descs[i]->hw_address;
        }
    }
    desc_block->jbig_descs[i-1]->next_desc = desc_block->jbig_descs[0]->hw_address;  // link the last desc back to first desc
    
    return OK;

ERROR:
    jbig_free_desc_block(desc_block);

    return FAIL;
}

static void jbig_reset_desc_block(jbig_dma_descriptor_block_t* desc_block)
{
    int i;
    uint32_t lockflags;

    dros_smplock_acquire_isr(&jbig_smplock, &lockflags);
    if(desc_block->num_descriptors_in_use)
    {
        ERRLOG( "%s: WARNING: num_in_use = %d\n", __func__, desc_block->num_descriptors_in_use );
        for (i = 0; i < NUM_JBIG_DMA_DESCS_PER_BLOCK; i++)
        {
            desc_block->jbig_descs[i]->control = 0;
            desc_block->jbig_descs[i]->dataBuffer = 0;
            desc_block->jbig_descs[i]->user_callback_data = 0;
        }
    }
    desc_block->last_descriptor = 0;
    desc_block->next_descriptor = 0;
    desc_block->num_descriptors_in_use = 0;
    dros_smplock_release_isr(&jbig_smplock, lockflags);
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
void jbigInit(const jbig_decode_block_config_t * jbig_plat_config)
{
	error_type_t result;
    uint32_t block_count;

    CurrentDecodeTransaction = NULL;


#ifdef HAVE_CMD
    int i_res;
    i_res = cmd_register( "jbig",
                          NULL,
                          NULL,
                          NULL,
                          jbig_callback );
    ASSERT( CMD_OK == i_res );
#endif

    dros_smplock_init(&jbig_smplock);

    for(block_count = 0; block_count < NUM_JBIG_BLOCKS; block_count++)
    {
        memset(&jbig_handles[block_count], 0, sizeof(jbig_decode_handle_t));

        // attach the low level block config to the handle
        jbig_handles[block_count].jbig_block_interface = &jbig_plat_config[block_count];

        jbig_block_init(jbig_handles[block_count].jbig_block_interface);

        result = jbig_alloc_desc_block(&jbig_handles[block_count].idma_desc_block);
        ASSERT(result == OK);

        result = jbig_alloc_desc_block(&jbig_handles[block_count].odma_desc_block);
        ASSERT(result == OK);

        intAttach(jbig_handles[block_count].jbig_block_interface->intnum_jbig, 0, isrJbigControl, "isrJbigControl", &jbig_handles[block_count]);
        intAttach(jbig_handles[block_count].jbig_block_interface->intnum_odma, 0, isrJbigODMA, "isrJbigODMA", &jbig_handles[block_count]);
        intAttach(jbig_handles[block_count].jbig_block_interface->intnum_idma, 0, isrJbigIDMA, "isrJbigIDMA", &jbig_handles[block_count]);
    }

#ifdef HAVE_POWER_MGR
    jbig_pwr_init();
#endif
}

static jbig_dma_desc_t *jbig_get_descriptor_from_block(jbig_dma_descriptor_block_t* desc_block)
{
    jbig_dma_desc_t *desc = NULL;
    uint32_t lockflags;

    XASSERT(desc_block->num_descriptors_in_use <= NUM_JBIG_DMA_DESCS_PER_BLOCK, desc_block->num_descriptors_in_use);

    dros_smplock_acquire_isr(&jbig_smplock, &lockflags);
    if(desc_block->num_descriptors_in_use < NUM_JBIG_DMA_DESCS_PER_BLOCK)
    {
        desc = desc_block->jbig_descs[desc_block->next_descriptor];
        XASSERT(!(desc->control & JBIG_DESCRIPTOR_OWNERSHIP), desc->control);

        if(++desc_block->next_descriptor == NUM_JBIG_DMA_DESCS_PER_BLOCK)
        {
            desc_block->next_descriptor = 0;
        }
        desc_block->num_descriptors_in_use++;
    }
    dros_smplock_release_isr(&jbig_smplock, lockflags);

    return desc;
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

error_type_t jbigAddDMABuffer( jbig_decode_handle_t * inHandle, jbig_dma_descriptor_block_t* desc_block, uint8_t *addr, uint32_t size, void * userData )
{

    error_type_t returnValue = FAIL;

    // If the specified size is 0, simply return fail.
    if (size == 0)
    {
        return returnValue;
    }
    
    if( inHandle && inHandle->inUse)
    {
        jbig_dma_desc_t* jbig_desc;

        jbig_desc = jbig_get_descriptor_from_block(desc_block);

        if( jbig_desc )
        {

            MEMLOG("add descriptor %x addr %x size %d\n", (uint32_t)jbig_desc, (uint32_t)addr, size );
    
            jbig_desc->dataBuffer = (uint32_t *)addr;
            jbig_desc->length = size;
            jbig_desc->user_callback_data = userData;
    
            jbig_desc->control = (JBIG_DESCRIPTOR_INTERRUPT_ON_COMPLETION | JBIG_DESCRIPTOR_OWNERSHIP);

            returnValue = OK;

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

error_type_t jbigStartWriteDMAIfPossible( jbig_decode_handle_t * inHandle )
{
    
    error_type_t returnValue = FAIL;
    uint32_t dmaBusy = 0;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    jbig_dma_desc_t * jbig_desc = NULL;
    uint32_t lockflags;
    
//-jrs    DBG_SLOG_ENTRY
    
    if( inHandle && inHandle->inUse)
    { 
        p_JBIG_ODMA_regs = inHandle->jbig_block_interface->jbig_odma_regs;
        
        dmaBusy = p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK;

        dros_smplock_acquire_isr(&jbig_smplock, &lockflags);
        if( jbig_is_decode_started( inHandle ) )
        {
            // if dma not currently in progress and descriptors are ready to go
            if( !dmaBusy && (inHandle->odma_desc_block.num_descriptors_in_use > 0) && 
                (inHandle->odma_desc_block.jbig_descs[inHandle->odma_desc_block.last_descriptor]->control & JBIG_DESCRIPTOR_OWNERSHIP))
            {
                MEMLOG("Start Write DMA now %d\n", jbig_is_decode_started( inHandle ));

                XASSERT(!(p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_SOFTRESET_MASK), p_JBIG_ODMA_regs->status);
                        
                intDisable(inHandle->jbig_block_interface->intnum_odma);
                                
                // this is the first descriptor for this page so we                
                // set up the ODMA registers here

                // -jrs TODO: spec recomments setting data width to 32 bits
                // set up the output Data Width to 8 bits
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_IN_WIDTH_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 3);
    
                // -jrs TODO: HW team recommends increasing this value for better bus efficiency; experiment
                // set the DMA burst length 4 words
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_BURST_LEN_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0);
    
                jbig_desc = inHandle->odma_desc_block.jbig_descs[inHandle->odma_desc_block.last_descriptor];
                        
                MEMLOG("Write desc %d @ %#x status %#x\n", inHandle->odma_desc_block.last_descriptor, (uint32_t)jbig_desc, p_JBIG_ODMA_regs->status );
                
                p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 1 );
                
                p_JBIG_ODMA_regs->desc_write = (uint32_t)jbig_desc->hw_address;
        
                intEnable(inHandle->jbig_block_interface->intnum_odma);
                        
                returnValue = OK;                    
            }   
        }
        dros_smplock_release_isr(&jbig_smplock, lockflags);
    }

//-jrs    DBG_SLOG_EXIT
        
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

error_type_t jbigStartReadDMAIfPossible( jbig_decode_handle_t * inHandle )
{
    error_type_t returnValue = FAIL;
    uint8_t dmaBusy = 0;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs = NULL;
    jbig_dma_desc_t * jbig_desc = NULL;
    uint32_t lockflags;

    if (inHandle && inHandle->inUse )
    {
        p_JBIG_IDMA_regs = inHandle->jbig_block_interface->jbig_idma_regs;

        dmaBusy = p_JBIG_IDMA_regs->status & IDMA_SLI_STATUS_DMA_BUSY_MASK;

        dros_smplock_acquire_isr(&jbig_smplock, &lockflags);
        if ( jbig_is_decode_started( inHandle ) )
        {
            // if dma not currently in progress and descriptors are ready to go
            if( !dmaBusy && (inHandle->idma_desc_block.num_descriptors_in_use > 0) && 
                (inHandle->idma_desc_block.jbig_descs[inHandle->idma_desc_block.last_descriptor]->control & JBIG_DESCRIPTOR_OWNERSHIP))
            {
                MEMLOG("Start Read DMA now %d!\n",jbig_is_decode_started( inHandle ));

                XASSERT(!(p_JBIG_IDMA_regs->status & IDMA_SLI_STATUS_SOFTRESET_MASK), p_JBIG_IDMA_regs->status);

                intDisable(inHandle->jbig_block_interface->intnum_idma);

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

                jbig_desc = inHandle->idma_desc_block.jbig_descs[inHandle->idma_desc_block.last_descriptor];

                MEMLOG("Read desc %d @ %#x status %#x\n", inHandle->idma_desc_block.last_descriptor, (uint32_t)jbig_desc, p_JBIG_IDMA_regs->status );

                p_JBIG_IDMA_regs->desc_write = (uint32_t)jbig_desc->hw_address;

                intEnable(inHandle->jbig_block_interface->intnum_idma);

                returnValue = OK;
            }
        }
        dros_smplock_release_isr(&jbig_smplock, lockflags);

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

static void disableJBIGInterrupts( jbig_decode_handle_t * inHandle )
{
    if( inHandle && inHandle->inUse)
    {
        intDisable( inHandle->jbig_block_interface->intnum_jbig );
        intDisable( inHandle->jbig_block_interface->intnum_odma );
        intDisable( inHandle->jbig_block_interface->intnum_idma );
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

static void enableJBIGInterrupts( jbig_decode_handle_t * inHandle)
{
    if( inHandle && inHandle->inUse)
    {
        intEnable( inHandle->jbig_block_interface->intnum_jbig );
        intEnable( inHandle->jbig_block_interface->intnum_odma );
        intEnable( inHandle->jbig_block_interface->intnum_idma );
    }
}

/**
 * jbigSetCurrentTransaction
 *
 * \brief There is a global pointer to the current transaction handle. This
 *        function is used to set this global pointer.
 * 
 * \param inHandle - pointer to current transaction handle.
 * 
 * \return 
 *
 *
 **/

void jbigSetCurrentTransaction( jbig_decode_handle_t * inHandle )
{
    if( inHandle )
    {
        // TODO: smp lock this
        disableJBIGInterrupts( inHandle );

        //intDisable( inHandle->jbig_block_interface->intnum_jbig );
        //intDisable( inHandle->jbig_block_interface->intnum_odma );
        //intDisable( inHandle->jbig_block_interface->intnum_idma );

        CurrentDecodeTransaction = inHandle ;

        //intEnable( inHandle->jbig_block_interface->intnum_jbig );
        //intEnable( inHandle->jbig_block_interface->intnum_odma );
        //intEnable( inHandle->jbig_block_interface->intnum_idma );

        enableJBIGInterrupts( inHandle );
    }    
}

/**
 * jbigGetCurrentTransaction
 *
 * \brief There is a global pointer to the current transaction handle. This
 *        function is used to get this global pointer.
 * 
 * \param inBlock - indentify the current JBIG block
 * 
 * \return jbig_dma_desc_t *
 *
 *
 **/

jbig_decode_handle_t * jbigGetCurrentTransaction( )
{
 
    jbig_decode_handle_t * returnVal = NULL;
    
    returnVal = CurrentDecodeTransaction;
    
    return returnVal;
}

/**
 * jbigClearCurrentTransaction
 *
 * \brief There is a global pointer to the current transaction handle. This
 *        function is used to clear this pointer when a transaction is complete.
 * 
 * \param inHandle pointer to current transaction handle
 * 
 * \return 
 *
 *
 **/

void jbigClearCurrentTransaction( jbig_decode_handle_t * inHandle )
{
    if( inHandle )
    {
        // TODO: smp lock this
        disableJBIGInterrupts( inHandle );

        CurrentDecodeTransaction = NULL ;
        
        enableJBIGInterrupts( inHandle );

    }    
}


/**
 * 
 *  This function will find the first available jbig handle in the
 *  jbig handle array.
 *
 **/

int JbigGetNextAvailableHandle( void )
{
	int i;
	int available_handle = -1;


	for ( i = 0; i < NUM_JBIG_BLOCKS; i++)
	{
		if( !jbig_handles[i].inUse )
		{
			available_handle = i;
			break;
		}
	}

    ASSERT( available_handle >= 0 );
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

jbig_decode_handle_t * jbig_decode_assign_handle( void )
{
    int handle_index;
    jbig_decode_handle_t * handle = NULL;
    
    handle_index = JbigGetNextAvailableHandle();
    if( handle_index >= 0 )
    {
        handle = &jbig_handles[handle_index];
        if( handle )
        {
            // TODO: Need to refresh descriptors & clear ownership bits if any
            handle->inUse = true;
            handle->lastError = 0;
            memset((void*)&handle->config, 0x0, sizeof(JBIG_CONFIG));
        }
    }
    else
    {
        ASSERT(0);
    }
    
    return handle;
}


#ifdef HAVE_CMD
/**
 * jbig_callback
 *
 * \brief 
 * 
 * \param 
 * 
 * \return 
 *
 *
 **/


static int jbig_callback(int argc, char *argv[])
{
    int32_t pos;

    pos = 1;

    if ( strncmp( (char *)argv[pos], "reset", 5 ) == 0 )
    {
        //hwPlatformReset();
    }
#ifdef HAVE_UNIT_TEST
    else if (strncmp((char *)argv[pos], "test",4) == 0)
    {
    	if (strncmp((char *)argv[pos+1], "1",1) == 0)
    	{
            DPRINTF(DBG_LOUD|DBG_OUTPUT,("Start JBIG Test 1\n"));
            jbigTest();
    	}
    	else if (strncmp((char *)argv[pos+1], "2",1) == 0)
    	{
    		DPRINTF(DBG_LOUD|DBG_OUTPUT,("Start JBIG Test 2\n"));
    		jbigSimpleTest();
    	}
    	else if (strncmp((char *)argv[pos+1], "3",1) == 0)
    	{
    		DPRINTF(DBG_LOUD|DBG_OUTPUT,("Start JBIG Test 3\n"));
    		jbigDelayedOutputTest();
    	}
    }
#endif // HAVE_UNIT_TEST
    else
    {
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("Supported cmds,\n\treset\n"));
#ifdef HAVE_UNIT_TEST
        DPRINTF(DBG_SOFT|DBG_OUTPUT,("\ttest 1\n\ttest 2\n\ttest 3\n"));
#endif
    }
    return OK;
}
#endif


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

static void isrJbigControl_common( uint32_t context )
{
    JBIG_REGS_t * p_JBIG_control_regs = NULL;
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs = NULL;
    uint32_t errorStatus;
    uint32_t status;
    jbig_decode_handle_t * currentHandle = (jbig_decode_handle_t*)context;


    ASSERT(currentHandle != NULL);
    
    p_JBIG_control_regs = currentHandle->jbig_block_interface->jbig_regs;
    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    status = p_JBIG_control_regs->JSTAT;
    errorStatus = status & JBIG_JSTAT_E_MASK;

    MEMLOG("isrJbigControl currentHandle = %#x status = %x\n", (uint32_t)currentHandle, status); 

    /*  This isr gets hit only when the decoder signals that it's done,
     *  successfully or not.
     */

    if(errorStatus)
    {
       ERRLOG("isrJbigControl: decoder stopped with an error: 0x%x\n", status );

        /* If the Error bit is set in the status register we need to record it so the
    	 * current application can query it later. We also will set the event flag so
    	 * that the calling application can be aware that an error occurred. The application
    	 * can then call the jbig_get_last_error_code function to retrieve the exact error code.
    	 */

//-jrs        jbig_decode_set_last_error_code( errorStatus );
    }

    if( status & JBIG_JSTAT_DD_MASK ) // Decode is done.
    {
//-jrs    	MEMLOG("  isrJbigControl: decoder completed successfully.\n" );
    	/* we need to clear the done bit */
    	REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_DD_MASK );
    }

    if( status & JBIG_JSTAT_ED_MASK ) // Encode is done.
    {
//-jrs    	MEMLOG("  isrJbigControl: encoder completed successfully.\n" );
    	/* we need to clear the done bit */
    	REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_ED_MASK );
    }

    if( status & JBIG_JSTAT_I_MASK) // if these bits are set then a Write or Read DMA has completed and is pending
    {
    	if( (status & JBIG_JSTAT_I_MASK) == 0x4000 )
    	{
//-jrs    	   MEMLOG("  isrJbigControl: Write DMA Complete.\n" );
    	}
    	if( (status & JBIG_JSTAT_I_MASK) == 0x8000 )
    	{
//-jrs    	   MEMLOG("  isrJbigControl: Read DMA Complete.\n" );
    	}
    }

    if( status & JBIG_JSTAT_D_MASK ) // Jbig operation is done.
    {
        MEMLOG("isrJbigControl: JBIG DONE.\n" );
        /* we need to clear the done bit */
        REG_CLEAR_BITS(&p_JBIG_control_regs->JSTAT, JBIG_JSTAT_D_MASK );

        // clear the enable bit in the ODMA to cause it to complete as described in the programming guide
        p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );

        // call user callback if any and pass back the error status
        if (currentHandle->jbigCallback != NULL)
        {
            currentHandle->jbigCallback(currentHandle, currentHandle->jbig_callback_context, errorStatus);
        }
    }
}

static inline bool is_jbig_descriptor_complete(jbig_dma_desc_t *desc)
{
    return !(desc->control & JBIG_DESCRIPTOR_OWNERSHIP);
}

static void jbig_decode_complete_last_descriptor(jbig_decode_handle_t* handle, jbig_dma_descriptor_block_t* desc_block, void* last_desc_hw_addr)
{
    jbig_dma_desc_t *desc = NULL;
    void* context;

//    SLOG_INFO("%s: handle %#x, num in use: %d\n", __func__, (uint32_t) handle, desc_block->num_descriptors_in_use);
    XASSERT(desc_block->num_descriptors_in_use <= NUM_JBIG_DMA_DESCS_PER_BLOCK, desc_block->num_descriptors_in_use);
    dros_smplock_acquire(&jbig_smplock);
    if(desc_block->num_descriptors_in_use > 0)
    {
        desc = desc_block->jbig_descs[desc_block->last_descriptor];
//        SLOG_INFO("%s: desc %d @ %#x\n", __func__, desc_block->last_descriptor, (uint32_t)desc);
        if(desc->hw_address == last_desc_hw_addr && !is_jbig_descriptor_complete(desc))
        {
            MEMLOG("%s: forcing completion on last desc\n", __func__ );
            desc->control = 0;
            if(++desc_block->last_descriptor == NUM_JBIG_DMA_DESCS_PER_BLOCK)
            {
                desc_block->last_descriptor = 0;
            }
            desc_block->num_descriptors_in_use--;
            context = desc->user_callback_data;

            desc->dataBuffer = 0;
            desc->user_callback_data = 0;

            // notify upper level driver of descriptor completion
            if(desc_block->dma_callback)
            {
                dros_smplock_release(&jbig_smplock);
                desc_block->dma_callback( handle, context );
                dros_smplock_acquire(&jbig_smplock);
            }
        }
    }
    dros_smplock_release(&jbig_smplock);
}

static void jbig_decode_complete_descriptors(jbig_decode_handle_t* handle, jbig_dma_descriptor_block_t* desc_block)
{
    jbig_dma_desc_t *desc = NULL;
    void* context;

//    SLOG_INFO("%s: handle %#x, num in use: %d\n", __func__, (uint32_t) handle, desc_block->num_descriptors_in_use);
    XASSERT(desc_block->num_descriptors_in_use <= NUM_JBIG_DMA_DESCS_PER_BLOCK, desc_block->num_descriptors_in_use);
    dros_smplock_acquire(&jbig_smplock);
    while(desc_block->num_descriptors_in_use > 0)
    {
        desc = desc_block->jbig_descs[desc_block->last_descriptor];
//        SLOG_INFO("%s: desc %d @ %#x\n", __func__, desc_block->last_descriptor, (uint32_t)desc);
        if(is_jbig_descriptor_complete(desc))
        {
            if(++desc_block->last_descriptor == NUM_JBIG_DMA_DESCS_PER_BLOCK)
            {
                desc_block->last_descriptor = 0;
            }
            desc_block->num_descriptors_in_use--;
            context = desc->user_callback_data;

            desc->dataBuffer = 0;
            desc->user_callback_data = 0;
            desc->control = 0;

            // notify upper level driver of descriptor completion
            if(desc_block->dma_callback)
            {
                dros_smplock_release(&jbig_smplock);
                desc_block->dma_callback( handle, context );
                dros_smplock_acquire(&jbig_smplock);
            }
        }
        else
        {
            // we're done once we reach the first descriptor that is owned by the hw
            break;
        }
    }
    dros_smplock_release(&jbig_smplock);
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
static void isrJbigIDMA_common( uint32_t context )
{
    jbig_decode_handle_t * currentHandle;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs;
    JBIG_REGS_t * p_JBIG_control_regs;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs;
    uint32_t status;
    uint32_t odma_status;
    uint32_t idma_status;

    uint32_t pendingInterrupts;

    currentHandle = (jbig_decode_handle_t*)context;
    ASSERT(currentHandle != NULL);
    
    p_JBIG_IDMA_regs = currentHandle->jbig_block_interface->jbig_idma_regs;
    p_JBIG_control_regs = currentHandle->jbig_block_interface->jbig_regs;
    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    status = p_JBIG_control_regs->JSTAT;
    odma_status = p_JBIG_ODMA_regs->status;
    idma_status = p_JBIG_IDMA_regs->status;

    pendingInterrupts = p_JBIG_IDMA_regs->int_st;

    MEMLOG("isrJbigIDMA currentHandle = %#x pending %#x control stat = %#x idma stat = %#x odma status = %#x\n", (uint32_t)currentHandle, pendingInterrupts, status, idma_status, odma_status );

    // Error Ownership bit was not set on descriptor
    if( pendingInterrupts & IDMA_SLI_INT_ST_WHO_INT_ST_MASK)
    {
    	MEMLOG("isrJbigIDMA: Ownership Error!\n" );
    }

    // Finished any completed descriptors
    if( pendingInterrupts & IDMA_SLI_INT_ST_FIN_INT_ST_MASK)
    {
        jbig_decode_complete_descriptors(currentHandle, &currentHandle->idma_desc_block);
    }

    // End of Image
    if( pendingInterrupts & IDMA_SLI_INT_ST_EOI_INT_ST_MASK)
    {
    	MEMLOG("             End Of Image\n" );
    }

    if( pendingInterrupts & IDMA_SLI_INT_ST_RST_INT_ST_MASK)
    {
    	ERRLOG("isrJbigIDMA: Soft Reset Complete\n" );
    }

    REG_SET_BITS(&p_JBIG_IDMA_regs->int_cl, pendingInterrupts);
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
static volatile uint32_t waitForODMAResetBit = 0;

static void isrJbigODMA_common( uint32_t context )
{	
	uint32_t pendingInterrupts;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
	jbig_decode_handle_t * currentHandle;

    currentHandle = (jbig_decode_handle_t*)context;
    ASSERT(currentHandle != NULL);

    p_JBIG_ODMA_regs = currentHandle->jbig_block_interface->jbig_odma_regs;

    pendingInterrupts = p_JBIG_ODMA_regs->int_pend;

    MEMLOG("isrJbigODMA: Pending Int %#x status = %#x, currentHandle = %#x\n", pendingInterrupts, p_JBIG_ODMA_regs->status, (uint32_t)currentHandle );

    // Error Ownership bit was not set on descriptor
    if( pendingInterrupts & ODMA_SLI_INT_PEND_WHO_INT_PEND_MASK)
    {
    	MEMLOG("isrJbigODMA: Ownership Error!\n" );
    }

    if( pendingInterrupts & ODMA_SLI_INT_PEND_RST_INT_PEND_MASK)
    {
        waitForODMAResetBit = 1;  // TODO: semaphore or other construct would be MUCH better way to do this
    }

    // Finished a descriptor
    if( pendingInterrupts & ODMA_SLI_INT_PEND_FIN_INT_PEND_MASK )
    {
        if( pendingInterrupts & ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK )
        {
//-jrs            MEMLOG("isrJbigODMA: data valid register = %x desc cfg = %x\n",p_JBIG_ODMA_regs->data_val, pDescriptor->control);
        }
        jbig_decode_complete_descriptors(currentHandle, &currentHandle->odma_desc_block);
    }
    else if( pendingInterrupts & (ODMA_SLI_INT_PEND_EOI_INT_PEND_MASK | ODMA_SLI_INT_PEND_EOI_ERR_INT_PEND_MASK ) )
    {
        // End of Image - triggers with error if not using EOIs, but that is OK.  Just ignore
        // the error and complete.  This happens when we complete the transfer by clearing the ODMA enable
        // bit in the jbig block complete ISR.
    	MEMLOG("isrJbigODMA: EOI!\n" );

        // complete outstanding descriptors
        jbig_decode_complete_descriptors(currentHandle, &currentHandle->odma_desc_block);

        // force completion of last descriptor if necessary
        jbig_decode_complete_last_descriptor(currentHandle, &currentHandle->odma_desc_block, (void*)p_JBIG_ODMA_regs->desc_read);
    }

    REG_SET_BITS(&p_JBIG_ODMA_regs->int_ack, pendingInterrupts);
}

#ifdef __KERNEL__
static irqreturn_t isrJbigIDMA(int32_t interrupt_count, void *context)
{
    isrJbigIDMA_common((uint32_t)context);
    return IRQ_HANDLED;
}
static irqreturn_t isrJbigODMA(int32_t interrupt_count, void *context)
{
    isrJbigODMA_common((uint32_t)context);
    return IRQ_HANDLED;
}
static irqreturn_t isrJbigControl(int32_t interrupt_count, void *context)
{
    isrJbigControl_common((uint32_t)context);
    return IRQ_HANDLED;
}
#else
static void isrJbigIDMA( void *context )
{
    isrJbigIDMA_common(context);
}
static void isrJbigODMA( void *context )
{
    isrJbigODMA_common(context);
}
static void isrJbigControl( void *context )
{
    isrJbigControl_common(context);
}
#endif

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

void jbigResetHardware( jbig_decode_handle_t * handle )
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    
    ASSERT(handle != NULL);
    p_JBIG_regs = handle->jbig_block_interface->jbig_regs;

    MEMLOG("%s core status = %x\n",__func__, p_JBIG_regs->JSTAT);

    jbigResetReadDMA( handle );
    jbigResetWriteDMA( handle );

    p_JBIG_regs->JBIG_EN = 0;   // disable block                                                                                
    p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL(p_JBIG_regs->JBIG_EN, 1); // reset block
    p_JBIG_regs->JBIG_EN = 0;   // clear reset and leave block disabled
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

void jbigFreeHandleDescriptors( jbig_decode_handle_t * handle )
{
//TODO -jrs    	
    MEMLOG("Free Descriptors for handle %x started = %d\n",(uint32_t)handle, handle->started);
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

void jbigStart( jbig_decode_handle_t * inHandle )
{    
    JBIG_REGS_t * p_JBIG_regs = NULL;
    
    if( inHandle )
    {
        inHandle->linesRemaining = inHandle->config.Yd;
        enableJBIGInterrupts( inHandle );
            
        p_JBIG_regs = (JBIG_REGS_t *) inHandle->jbig_block_interface->jbig_regs;
    
        MEMLOG("Start DECODE\n");
        p_JBIG_regs->JCTL = JBIG_JCTL_E_REPLACE_VAL(p_JBIG_regs->JCTL, 0);
        p_JBIG_regs->JCTL = JBIG_JCTL_D_REPLACE_VAL(p_JBIG_regs->JCTL, 1);
        
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
 
void jbigStop( jbig_decode_handle_t * inHandle )
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    ODMA_SLI_REGS_t * p_JBIG_ODMA_regs = NULL;
    IDMA_SLI_REGS_t * p_JBIG_IDMA_regs = NULL;
    int loopCount = 0;
    
    DBG_SLOG_ENTRY
    
    if( inHandle )
    {
        p_JBIG_ODMA_regs = inHandle->jbig_block_interface->jbig_odma_regs;
        p_JBIG_IDMA_regs = inHandle->jbig_block_interface->jbig_idma_regs;
        p_JBIG_regs = (JBIG_REGS_t *) inHandle->jbig_block_interface->jbig_regs;
        
        MEMLOG("%s core status = %x\n",__func__, p_JBIG_regs->JSTAT);

        // reset the dmas - Originally we were disabling the DMAs and then resetting later
        // however, this was causing a hang during reset. So instead we now just reset.
//        p_JBIG_IDMA_regs->cfg = IDMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_IDMA_regs->cfg, 0 );
//        p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );
        p_JBIG_IDMA_regs->reset = IDMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_IDMA_regs->reset, 1);
        p_JBIG_IDMA_regs->reset = IDMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_IDMA_regs->reset, 0);
        p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 1);
        p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 0);
        
        // Wait for the DMA to flush any pending data out. Seems unlikely...
        while((p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK) && (loopCount++ < 2000) );
        if (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK)
        {
            MEMLOG("%s Wait for ODMA idle failed.  ODMA status = %#x\n",__func__, p_JBIG_ODMA_regs->status);
            MEMLOG("%s core status = %x\n",__func__, p_JBIG_regs->JSTAT);
        }
        MEMLOG("%s ODMA reset wait count = %d\n",__func__, loopCount);

        // reset the jbig block
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 );
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 1 ); 
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_S_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 );

        p_JBIG_regs->JSTAT = 0;

        // disable the jbig block
        p_JBIG_regs->JBIG_EN = JBIG_JBIG_EN_E_REPLACE_VAL( p_JBIG_regs->JBIG_EN, 0 ); 

        // clean up the dma descriptors
        jbig_reset_desc_block(&inHandle->idma_desc_block);
        jbig_reset_desc_block(&inHandle->odma_desc_block);
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

void jbigResetWriteDMA( jbig_decode_handle_t * handle )
{
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs;
    IDMA_SLI_REGS_t* p_JBIG_IDMA_regs;
    uint32_t loopCount = 0;
	MEMLOG("jbig: jbigResetWriteDMA\n");

    DBG_SLOG_ENTRY
    ASSERT(handle != NULL);

    waitForODMAResetBit = 0;

    p_JBIG_ODMA_regs= handle->jbig_block_interface->jbig_odma_regs;
    p_JBIG_IDMA_regs = handle->jbig_block_interface->jbig_idma_regs;


    MEMLOG("jbigODMA: status = %#x\n", p_JBIG_ODMA_regs->status);

   // while(1 == (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_DMA_BUSY_MASK) &&( loopCount++ <1000) );
    // it is not uncommon for the dma busy flag to be set on write dma if the buffer length given is larger than decompresse/compressed data
    // provided by the IDMA.


   // p_JBIG_ODMA_regs->int_ack = 0xFF/*JBIG_DMA_INT_MASK */;


    //while((ODMA_SLI_STATUS_EMPTY_DBUF_MASK | ODMA_SLI_STATUS_EMPTY_CBUF_MASK) != (p_JBIG_ODMA_regs->status & (ODMA_SLI_STATUS_EMPTY_DBUF_MASK | ODMA_SLI_STATUS_EMPTY_CBUF_MASK)) && ( loopCount++ <1000) )
    while(ODMA_SLI_STATUS_EMPTY_CBUF_MASK != (p_JBIG_ODMA_regs->status & ODMA_SLI_STATUS_EMPTY_CBUF_MASK) && ( loopCount++ <1000) )
    {
        ERRLOG("%s: jbigODMA: status = %#x\n", __func__, p_JBIG_ODMA_regs->status);
    }

    p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 1);
	p_JBIG_ODMA_regs->reset = ODMA_SLI_RESET_SOFT_RESET_REPLACE_VAL(p_JBIG_ODMA_regs->reset, 0);


    MEMLOG("jbigODMA: after reset status = %#x\n", p_JBIG_ODMA_regs->status);

    /* Wait for the DMA operations to shutdown. This needs to happen
     * before the DMA can be safely used again.
     */
    
    loopCount = 0;

    while(( waitForODMAResetBit == 0 ) && ( loopCount++ < 2000 ))
    {
        // just in case someone has interrupts disabled check the bit directly here
        if( p_JBIG_ODMA_regs->int_pend & ODMA_SLI_INT_PEND_RST_INT_PEND_MASK )
        {
            break;
        }
        if((loopCount % 200) == 0)
        {
            ERRLOG("jbigODMA: waiting for reset %d status %#x\n",loopCount,p_JBIG_ODMA_regs->status);
        }
    }
    if(loopCount >= 2000)
    {
        ERRLOG("jbigODMA: WARNING JBIG ODMA RESET WAIT EXPIRED\n");
    }

    p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_ENABLE_REPLACE_VAL(p_JBIG_ODMA_regs->cfg, 0);
    p_JBIG_ODMA_regs->cfg = ODMA_SLI_CFG_UPPER_HALF_REPLACE_VAL( p_JBIG_ODMA_regs->cfg, 0 );

    p_JBIG_ODMA_regs->int_ack = 0xFF/*JBIG_DMA_INT_MASK */;

    MEMLOG("jbigIDMA: status = %x\n", p_JBIG_IDMA_regs->status);
    MEMLOG("jbigODMA: status = %x\n", p_JBIG_ODMA_regs->status);
	
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

void jbigResetReadDMA( jbig_decode_handle_t * handle )
{
    IDMA_SLI_REGS_t* p_JBIG_IDMA_regs;
    uint32_t loopCount = 0;

    DBG_SLOG_ENTRY

    ASSERT(handle != NULL);

    p_JBIG_IDMA_regs = handle->jbig_block_interface->jbig_idma_regs;

    MEMLOG("jbigIDMA: cfg = %x \n\t int_en %x \n\tint_pend %x \n\tint_ack %x\n\tstatus = %x\n", p_JBIG_IDMA_regs->cfg, p_JBIG_IDMA_regs->int_en,
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

error_type_t jbigAddReadDMABuffer( jbig_decode_handle_t * inHandle, uint8_t *addr, uint32_t size, void * userData )
{

    error_type_t returnValue = FAIL;
    
    if( inHandle && inHandle->inUse )
    {
        returnValue = jbigAddDMABuffer( inHandle, &inHandle->idma_desc_block, addr, size, userData );
        if(returnValue == OK)
        {
            jbigStartReadDMAIfPossible( inHandle );
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

void jbigStartReadDMA( jbig_decode_handle_t * inHandle )
{

    if( inHandle )
    {
//        MEMLOG("%s\n", __func__);
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

void jbigStartWriteDMA( jbig_decode_handle_t * inHandle )
{


    if( inHandle )
    {
        // enable the DMA
//        MEMLOG("%s\n", __func__);        
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

error_type_t jbigAddWriteDMABuffer( jbig_decode_handle_t * inHandle, uint8_t *addr, uint32_t size, void * userData )
{
    error_type_t returnValue = FAIL;
    
//-jrs    DBG_SLOG_ENTRY

    if( inHandle )
    {
        MEMLOG( "%s user data 0x%x, addr 0x%x size = %d\n", __func__, (uint32_t)userData, (uint32_t)addr, size);
            
        returnValue = jbigAddDMABuffer( inHandle, &inHandle->odma_desc_block, addr, size, userData );

        if(returnValue == OK)
        {
            jbigStartWriteDMAIfPossible( inHandle );
        }
    }
    
//-jrs    DBG_SLOG_EXIT

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

error_type_t jbigCommitConfig(jbig_decode_handle_t *handle)
{
    JBIG_REGS_t * p_JBIG_regs = NULL;
    ODMA_SLI_REGS_t* p_JBIG_ODMA_regs = NULL;
	JBIG_CONFIG* config;

    ASSERT(handle != NULL);
    // get the register interfaces
    p_JBIG_regs = handle->jbig_block_interface->jbig_regs;
    p_JBIG_ODMA_regs = handle->jbig_block_interface->jbig_odma_regs;

    config = &handle->config;

    MEMLOG("%s core status = %x\n",__func__, p_JBIG_regs->JSTAT);

    // set TPBON
    p_JBIG_regs->JCTL = JBIG_JCTL_T_REPLACE_VAL( p_JBIG_regs->JCTL, config->TpbonState);

    //setup ATMOV
    p_JBIG_regs->JCTL = JBIG_JCTL_A_REPLACE_VAL( p_JBIG_regs->JCTL, config->AtmovState);
    if ( config->AtmovState == ATMOV_ENABLED )
    {
        p_JBIG_regs->JCTL = JBIG_JCTL_M_REPLACE_VAL( p_JBIG_regs->JCTL, config->ATMOVvalue);
    }
    else
    {
        p_JBIG_regs->JCTL = JBIG_JCTL_M_REPLACE_VAL( p_JBIG_regs->JCTL, 0);
    }        

    // Set image dimensions
    p_JBIG_regs->JX = config->Xd;
    p_JBIG_regs->JY = config->Yd;
    p_JBIG_regs->JSL = config->Ld;

    // set 3Line
    p_JBIG_regs->JCTL = JBIG_JCTL_L_REPLACE_VAL( p_JBIG_regs->JCTL, config->LineTemplate);

    // set BPEND
    p_JBIG_regs->JCTL = JBIG_JCTL_BE_REPLACE_VAL( p_JBIG_regs->JCTL, config->Endian);

    p_JBIG_ODMA_regs->line_size = config->LineWidth;

	return OK;
}


void jbig_dump_control_regs(jbig_decode_handle_t *handle)
{
    JBIG_REGS_t * jbig_regs = handle->jbig_block_interface->jbig_regs;
    ERRLOG("JBIG Control Reg Dump:\n");
    ERRLOG("\tJCTL    = %#x\n", jbig_regs->JCTL);
    ERRLOG("\tJSTAT   = %#x\n", jbig_regs->JSTAT);
    ERRLOG("\tJX      = %#x\n", jbig_regs->JX);
    ERRLOG("\tJY      = %#x\n", jbig_regs->JY);
    ERRLOG("\tJBIG_EN = %#x\n", jbig_regs->JBIG_EN);
    ERRLOG("\tJSL     = %#x\n", jbig_regs->JSL);
}

void jbig_dump_odma_regs(jbig_decode_handle_t *handle)
{
    ODMA_SLI_REGS_t* odma_regs = handle->jbig_block_interface->jbig_odma_regs;
    ERRLOG("JBIG ODMA Reg Dump:\n");
    ERRLOG("\tcfg = %#x\n", odma_regs->cfg);
    ERRLOG("\tstatus = %#x\n", odma_regs->status);
    ERRLOG("\tline_size = %#x\n", odma_regs->line_size);
    ERRLOG("\tint_en = %#x\n", odma_regs->int_en);
    ERRLOG("\tint_pend = %#x\n", odma_regs->int_pend);
    ERRLOG("\tint_ack = %#x\n", odma_regs->int_ack);
    ERRLOG("\tdesc_write = %#x\n", odma_regs->desc_write);
    ERRLOG("\tdesc_read = %#x\n", odma_regs->desc_read);
    ERRLOG("\txfer_length = %#x\n", odma_regs->xfer_length);
    ERRLOG("\txfer_addr = %#x\n", odma_regs->xfer_addr);
    ERRLOG("\txfer_burst = %#x\n", odma_regs->xfer_burst);
    ERRLOG("\treset = %#x\n", odma_regs->reset);
}

void jbig_dump_idma_regs(jbig_decode_handle_t *handle)
{
    IDMA_SLI_REGS_t* idma_regs = handle->jbig_block_interface->jbig_idma_regs;
    ERRLOG("JBIG IDMA Reg Dump:\n");
    ERRLOG("\tcfg = %#x\n", idma_regs->cfg);
    ERRLOG("\tstatus = %#x\n", idma_regs->status);
    ERRLOG("\tline_width = %#x\n", idma_regs->line_width);
    ERRLOG("\tint_en = %#x\n", idma_regs->int_en);
    ERRLOG("\tint_pend = %#x\n", idma_regs->int_st);
    ERRLOG("\tint_ack = %#x\n", idma_regs->int_cl);
    ERRLOG("\tdesc_write = %#x\n", idma_regs->desc_write);
    ERRLOG("\tdesc_read = %#x\n", idma_regs->desc_read);
    ERRLOG("\txfer_length = %#x\n", idma_regs->xfer_length);
    ERRLOG("\txfer_addr = %#x\n", idma_regs->xfer_addr);
    ERRLOG("\tctrl_word = %#x\n", idma_regs->ctrl_word);
    ERRLOG("\treset = %#x\n", idma_regs->reset);
}

/**
 * \brief Debug routine to dump all jbig registers for the blocks associated with the given handle
 */
void jbig_dump_all_regs(jbig_decode_handle_t * handle)
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

