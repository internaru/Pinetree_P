/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file mrvl_cdma.c
 * 
 * \brief See cdma_api.h.
 * 
 **/

#include <stdint.h>
#include <stdbool.h>
#include "tx_api.h"
#include "ATypes.h"
#include "ctype.h"
#include "cpu_api.h"
#include "interrupt_api.h"
#include "string.h" // used for memcpy and memset
#include "lassert.h"
#include "memAPI.h"
#include "logger.h"
#include "error_types.h"
#include "dprintf.h"

#include "cdma_api.h"
#include "mrvl_cdma_config.h"
#include "CDMA_regmasks.h"
#include "CDMA_regstructs.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
#endif

#if defined (HAVE_UNIT_TEST) || defined (HAVE_CMD)
#include "cdma_test.h"
#endif

/**
 * 
 * DEFINES
 * 
 **/

#define CDMA_QUEUE_SIZE 64  // the max number of links for a chained dma, 64 is 4MB
#define MAXXFERSIZE 0xffff  ///< max xfer size in bytes of a single transfer of the cdma

#define LINK_OFFSET(base, link) ((base * CDMA_QUEUE_SIZE) + link)   // compute chained dma link offset


/** 
 * \brief Callback flags for /a status parameter of callback
 * function specified in CdmaTransfer().
 **/
#define CHANNEL_FREE    1<<0
#define HANDLE_NOT_USED -1

/**
 * 
 * TYPEDEFS
 * 
 **/

/**
 * Continue point
 **/
typedef struct cdma_continue_s
{
    uint8_t *Src;                      // source address
    uint8_t *Dest;                     // destination address
    uint32_t Length;                  // remaining bytes
    uint32_t Timeout;                 // timeout
} cdma_continue_t;

/**
 * Channel info
 **/
typedef struct cdma_channel_s
{
    TX_EVENT_FLAGS_GROUP free;      // channel is in use                                    
    cdma_continue_t cont;           // continue point
    void *callback;                 // user callback for non-blocking transfer
    void *context;                  // user data passed to callback
    bool open;                      // channel is open
    CDMA_CONFIG config;             // channel configuration
} cdma_channel_t;

struct cdma_handle_s
{
    uint32_t    channel;
};


/**
 * 
 * LOCALS
 * 
 **/
static cdma_channel_t *cdma_channel;              // pointer to channel info list
static CDMA_PLATFORM_CONFIG *config = NULL;     // platform configuration parameters
static cdma_link_t *CdmaLink;                     // pointer to chained list storage
static uint32_t dma_base;                         // dma base address
static uint32_t dma_channel_offset;               // dma channel offset
static TX_SEMAPHORE cdma_open_semaphore;        // CdmaOpen is in use
static TX_EVENT_FLAGS_GROUP CdmaFlags;          // blocking transfer channel flags
uint32_t maxxfersize;                           // max source width words transferred
static cdma_handle_t *cdma_handles = NULL;

/**
 * 
 * PROTOTYPES
 * 
 **/
static int CdmaProcess(uint8_t *Src, uint8_t *Dest, uint32_t Length, uint32_t Timeout, uint32_t base, CDMA_CALLBACK callback);
#ifdef HAVE_UNIT_TEST
static int cdma_unit_test( void );
#endif
#ifdef HAVE_CMD
static int cdma_debug_callback(int argc, char *argv[]);
#endif


/**
 * 
 * FUNCTIONS
 * 
 **/

/** 
 * \brief Get the device flow control.
 * 
 * \param flowcontrol The api type.
 * 
 * \return Device flow control value.
 **/
static uint8_t cdma_get_device_flow_control(CDMA_FLOWCONTROL flowcontrol)
{
    switch(flowcontrol) {
        case CDMA_MEM_TO_MEM:
            return 0;
            break;
        case CDMA_MEM_TO_DEV:
            return 1;
            break;
        case CDMA_DEV_TO_MEM:
            return 2;
            break;
        case CDMA_DEV_TO_DEV:
            return 3;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_flow_control: unsupported flow control");
            return 0;
            break;
    }
}


/** 
 * \brief Get the device endian.
 * 
 * \param endian The api type.
 * 
 * \return Device endian value.
 **/
static uint8_t cdma_get_device_endian(CDMA_ENDIAN endian)
{
    switch(endian) {
        case CDMA_ABCD_TO_ABCD:
            return 0;
            break;
        case CDMA_ABCD_TO_CDAB:
            return 1;
            break;
        case CDMA_ABCD_TO_BADC:
            return 2;
            break;
        case CDMA_ABCD_TO_DCBA:
            return 3;
            break;
        case CDMA_LITTLE_ENDIAN:
            return 0;
            break;
        case CDMA_BIG_ENDIAN:
            return 1;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_endian: unsupported endian");
            return 0;
            break;
    }
}


/** 
 * \brief Get the device xfer width.
 * 
 * \param width The api type.
 * 
 * \return Device xfer width value.
 **/
static uint8_t cdma_get_device_xfer_width(CDMA_XFER_WIDTH width)
{
    switch(width) {
        case CDMA_8_BITS:
            return 0;
            break;
        case CDMA_16_BITS:
            return 1;
            break;
        case CDMA_32_BITS:
            return 2;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_xfer_width: unsupported width");
            return 0;
            break;
    }
}


/** 
 * \brief Get the device burst size.
 * 
 * \param size The api type.
 * 
 * \return Device burst size.
 **/
static uint8_t cdma_get_device_burst_size(CDMA_BURST_SIZE size)
{
    switch(size) {
        case CDMA_BURST_1:
            return 0;
            break;
        case CDMA_BURST_4:
            return 1;
            break;
        case CDMA_BURST_8:
            return 2;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_burst_size: unsupported size");
            return 0;
            break;
    }
}


/** 
 * \brief Get the device interrupt time-out base.
 * 
 * \param base The api type.
 * 
 * \return Device interrupt time-out base.
 **/
static uint8_t cdma_get_device_timebase(CDMA_TIMEBASE_CMD base)
{
    switch(base) {
        case CDMA_1_US:
            return 0;
            break;
        case CDMA_10_US:
            return 1;
            break;
        case CDMA_100_US:
            return 2;
            break;
        case CDMA_1_MS:
            return 3;
            break;
        case CDMA_10_MS:
            return 4;
            break;
        case CDMA_100_MS:
            return 5;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_timebase: unsupported timebase");
            return 0;
            break;
    }
}


/** 
 * \brief CDMA continue callback.
 * 
 * This function is used to flag a CDMA continue sequence.
 **/
static void cdma_continue_callback(void *dummyvoid, uint32_t dummy32)
{
}


/** 
 * \brief CDMA ISR.
 * 
 * This function will service all CDMA channels.
 **/
void CdmaISR(uint32_t input)
{
    uint32_t i;
    CDMA_CALLBACK MyRoutine;
    uint32_t status;
    CDMA_REGS_t *cdma_registers;


    // find the channel to be serviced
    for(i = 0; i < config->number_of_channels; i++)
    {
        cdma_registers = (CDMA_REGS_t *)(config->dma_base + ( i * dma_channel_offset ));
        if( CDMA_INTPEND_CHAINDONEPEND_MASK_SHIFT( cdma_registers->intPend ))
        {
            // should this continue?
            if(cdma_channel[i].cont.Length) {
                // clear the interrupts
                cdma_registers->intAck = CDMA_INTACK_CHAINDONEACK_REPLACE_VAL( 0, 1 );
                cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL( 0, 1 );
                cdma_registers->intAck = CDMA_INTACK_TERMCNTACK_REPLACE_VAL( 0, 1 );
                // Disable the DMA channel
                cdma_registers->CFG = CDMA_CFG_ENABLE_REPLACE_VAL( cdma_registers->CFG, 0 );
                CdmaProcess(cdma_channel[i].cont.Src, cdma_channel[i].cont.Dest, cdma_channel[i].cont.Length, cdma_channel[i].cont.Timeout, i, cdma_continue_callback);
            } else {
                // is this blocking or non-blocking?
                if(cdma_channel[i].callback != NULL) {
                    // non-blocking
                    MyRoutine = cdma_channel[i].callback;
                    // get the interrupt status
                    status = CDMA_INTPEND_CHAINDONEPEND_MASK_SHIFT( cdma_registers->intPend );
                    // clear the interrupts
                    cdma_registers->intAck = CDMA_INTACK_CHAINDONEACK_REPLACE_VAL( 0, 1 );
                    cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL( 0, 1 );
                    cdma_registers->intAck = CDMA_INTACK_TERMCNTACK_REPLACE_VAL( 0, 1 );
                    // Disable the DMA channel
                    cdma_registers->CFG = CDMA_CFG_ENABLE_REPLACE_VAL( cdma_registers->CFG, 0 );
                    MyRoutine(cdma_channel[i].context, status);         // call the callback
                    
                    // return channel to not in use
                    status = tx_event_flags_set(&cdma_channel[i].free, CHANNEL_FREE, TX_OR);
                    ASSERT(status == TX_SUCCESS);
                } else {
                    // blocking
                    // clear the interrupts
                    cdma_registers->intAck = CDMA_INTACK_CHAINDONEACK_REPLACE_VAL( 0, 1 );
                    cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL( 0, 1 );
                    cdma_registers->intAck = CDMA_INTACK_TERMCNTACK_REPLACE_VAL( 0, 1 );
                    // Disable the DMA channel
                    cdma_registers->CFG = CDMA_CFG_ENABLE_REPLACE_VAL( cdma_registers->CFG, 0 );
                    tx_event_flags_set(&CdmaFlags, 1<<i, TX_OR);        // notify CdmaProcess() of the interrupt
                }
            }
        }
        if(cdma_registers->intPend & CDMA_INTPEND_TIMEOUTPEND_MASK)
        {
            // just clear this register.
            cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL( 0, 1 );

    }
}
}


void CdmaInit()
{
    uint32_t status;
    uint32_t i;
    error_type_t result;
    #ifdef HAVE_CMD
    int i_res;
    #endif
    #ifdef HAVE_UNIT_TEST
    int unit_test_res;
    #endif

    ASSERT(config == NULL);                         // has CdmaInit() already been called?
    result = cdma_platform_get_config(&config); 
    ASSERT(result == OK);
    ASSERT(config != NULL);

    dma_base = config->dma_base;
    dma_channel_offset = config->dma_channel_offset;
    // Align max size to CPU cache alignment size (max number of bytes)
    maxxfersize = MAXXFERSIZE - (MAXXFERSIZE % cpu_get_dcache_line_size());

    cdma_handles = (cdma_handle_t *)MEM_MALLOC((sizeof(cdma_handle_t) * config->number_of_channels));
    ASSERT( NULL != cdma_handles );

    // allocate memory for cdma channel list
    cdma_channel = (cdma_channel_t *)MEM_MALLOC( (sizeof(cdma_channel_t)*config->number_of_channels));
    ASSERT(cdma_channel != NULL);
    memset(cdma_channel,0,sizeof(cdma_channel_t)*config->number_of_channels);

    // allocate memory for cdma chain lists
    CdmaLink = (cdma_link_t *)MEM_MALLOC( (sizeof(cdma_link_t)*CDMA_QUEUE_SIZE*config->number_of_channels) );
    ASSERT(CdmaLink != NULL);

    status = tx_event_flags_create(&CdmaFlags, "Cdma");
    ASSERT(status == TX_SUCCESS);

    // initialize each channel
    for(i = 0; i < config->number_of_channels; i++) {

        cdma_handles[i].channel = HANDLE_NOT_USED;
        cdma_channel[i].open = false;

        status = tx_event_flags_create(&cdma_channel[i].free, "CDMA");
        ASSERT(status == TX_SUCCESS);
        status = tx_event_flags_set(&cdma_channel[i].free, CHANNEL_FREE, TX_OR);
        ASSERT(status == TX_SUCCESS);

        // setup the interrupt
        if(config->intnums[i] != CDMA_INTNUM_IGNORE) {
            intAttach(config->intnums[i], 0, (PFN_ISR)CdmaISR, config->intnums[i]);
            intEnable(config->intnums[i]);
        }
    }

    status = tx_semaphore_create(&cdma_open_semaphore, "CDMA", 1);
    ASSERT(status == TX_SUCCESS);

#ifdef HAVE_CMD
    i_res = cmd_register( "cdma",
                          NULL,
                          NULL,
                          NULL,
                          cdma_debug_callback);
    ASSERT( CMD_OK == i_res );
#endif

#ifdef HAVE_UNIT_TEST
    /* also register with the unit test manager */
    unit_test_res = unit_test_register_testcase("cdma", cdma_unit_test);
    ASSERT( 0 == unit_test_res );
#endif
}

error_type_t CdmaGetNumberOfChannels(uint32_t *number_of_channels)
{
    ASSERT(config != NULL);                         // has the driver been initialized?

    *number_of_channels = config->number_of_channels;
    return OK;
}

error_type_t CdmaOpen(int32_t channel, cdma_handle_t **handle)
{
    uint32_t status;
    uint32_t i;
    error_type_t return_result = FAIL;

    ASSERT(config != NULL);                         // has the driver been initialized?

    // is CdmaOpen already in use?
    status = tx_semaphore_get(&cdma_open_semaphore, TX_WAIT_FOREVER);
    ASSERT(status == TX_SUCCESS);

    // is channel valid?
    ASSERT(channel >= CDMA_FIND_CHANNEL && channel < (int32_t)config->number_of_channels);

    // find the channel
    for(i = 0; i < config->number_of_channels; i++)
        if((i == channel ||                                                      // specified
            channel == CDMA_FIND_CHANNEL) &&                                     // or find first available?
           cdma_channel[i].open == false) {                                      // is this channel free?
            cdma_channel[i].open = true;                                         // open the channel
            cdma_handles[i].channel = i;
            *handle = &cdma_handles[i];
            return_result = OK;
            break;
        }
    // return CdmaOpen to not in use
    status = tx_semaphore_put(&cdma_open_semaphore);
    ASSERT(status == TX_SUCCESS);
    return return_result;
}

error_type_t CdmaClose(cdma_handle_t *handle)
{
    uint32_t channel = handle->channel;
    uint32_t status, flags;
    error_type_t return_result = FAIL;

    ASSERT(config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < config->number_of_channels);
    if(channel < config->number_of_channels) {
        // is this channel is use?
        status = tx_event_flags_get(&cdma_channel[channel].free, CHANNEL_FREE, TX_AND_CLEAR, &flags, TX_NO_WAIT);
        if(status == TX_SUCCESS) {
            // has this channel been opened?
            ASSERT(cdma_channel[channel].open == true);
            if(cdma_channel[channel].open == true) {
                cdma_channel[channel].open = false;
                cdma_handles[channel].channel = HANDLE_NOT_USED;
                return_result = OK;
            }
            // return channel to not in use
            status = tx_event_flags_set(&cdma_channel[channel].free, CHANNEL_FREE, TX_OR);
            ASSERT(status == TX_SUCCESS);
        } else {
            ASSERT(NULL == &"CdmaClose: close failed channel in use");
        }
    }
    return return_result;
}


error_type_t CdmaSetConfig(cdma_handle_t *handle, const CDMA_CONFIG *MyCfg)
{
    uint32_t channel = handle->channel;
    uint32_t status, flags;
    int transfer_type;
    CDMA_REGS_t *cdma_registers;

    ASSERT(config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < config->number_of_channels);
    if(!(channel < config->number_of_channels))
        return FAIL;

    // has this channel been opened?
    ASSERT(cdma_channel[channel].open == true);
    if(cdma_channel[channel].open == false)
        return FAIL;

    // is this channel is use?
    if(tx_event_flags_get(&cdma_channel[channel].free, CHANNEL_FREE, TX_AND_CLEAR, &flags, TX_NO_WAIT) != TX_SUCCESS)
        return FAIL;

    cdma_registers = (CDMA_REGS_t *)( config->dma_base + ( channel * dma_channel_offset ));

    // Clear interrupts
    cdma_registers->intAck = CDMA_INTACK_CHAINDONEACK_REPLACE_VAL( 0, 1 );
    cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL( 0, 1 );
    cdma_registers->intAck = CDMA_INTACK_TERMCNTACK_REPLACE_VAL( 0, 1 );

    // set source address increment
    if(MyCfg->SrcIncrement == CDMA_INC)
        cdma_registers->Control = CDMA_CONTROL_SRCADDRINC_REPLACE_VAL( cdma_registers->Control, 1 );
    else if(MyCfg->SrcIncrement == CDMA_NO_INC)
        cdma_registers->Control = CDMA_CONTROL_SRCADDRINC_REPLACE_VAL( cdma_registers->Control, 0 );
    else
        ASSERT(NULL == &"CdmaSetConfig: unsupported source address increment");

    // set destination address increment
    if(MyCfg->DestIncrement == CDMA_INC)
        cdma_registers->Control = CDMA_CONTROL_DESTADDRINC_REPLACE_VAL( cdma_registers->Control, 1 );
    else if(MyCfg->DestIncrement == CDMA_NO_INC)
        cdma_registers->Control = CDMA_CONTROL_DESTADDRINC_REPLACE_VAL( cdma_registers->Control, 0 );
    else
        ASSERT(NULL == &"CdmaSetConfig: unsupported destination address increment");

    // Are we doing a fill?
    if( MyCfg->DestModify != CDMA_NO_MOD_DEST )
    {
        cdma_registers->FillValue = MyCfg->DestFill;
        cdma_registers->CFG = CDMA_CFG_FILL_REPLACE_VAL( cdma_registers->CFG, 1 );

    }
    else  // No fill
    {
        cdma_registers->CFG = CDMA_CFG_FILL_REPLACE_VAL( cdma_registers->CFG, 0 );
    }

    transfer_type = cdma_get_device_flow_control( MyCfg->FlowControl );

    // set transfer widths
    // Memory access only supports 32 bit access so we enforce it here
    if(( transfer_type == CDMA_MEM_TO_MEM ) || ( transfer_type == CDMA_MEM_TO_DEV ))
    {
        cdma_registers->Control = CDMA_CONTROL_SRCXFERWIDTH_REPLACE_VAL( cdma_registers->Control, 
                cdma_get_device_xfer_width( CDMA_32_BITS ));
    }
    else
    {
        cdma_registers->Control = CDMA_CONTROL_SRCXFERWIDTH_REPLACE_VAL( cdma_registers->Control, 
                cdma_get_device_xfer_width( MyCfg->SrcXferWidth ));
    }
    if(( transfer_type == CDMA_MEM_TO_MEM ) || ( transfer_type == CDMA_DEV_TO_MEM ))
    {
        cdma_registers->Control = CDMA_CONTROL_DESTXFERWIDTH_REPLACE_VAL( cdma_registers->Control, 
                cdma_get_device_xfer_width( CDMA_32_BITS ));
    }
    else
    {
        cdma_registers->Control = CDMA_CONTROL_DESTXFERWIDTH_REPLACE_VAL( cdma_registers->Control, 
                cdma_get_device_xfer_width( MyCfg->DestXferWidth ));
    }

    // set burst sizes
    cdma_registers->Control = CDMA_CONTROL_SRCBURSTSIZE_REPLACE_VAL( cdma_registers->Control, 
                                       cdma_get_device_burst_size( MyCfg->SrcBurst ));
    cdma_registers->Control = CDMA_CONTROL_DESTBURSTSIZE_REPLACE_VAL( cdma_registers->Control, 
                                       cdma_get_device_burst_size( MyCfg->DestBurst ));
    // set peripheral ids
    if(MyCfg->FlowControl == CDMA_DEV_TO_DEV || MyCfg->FlowControl == CDMA_DEV_TO_MEM)
    {
        cdma_registers->CFG = CDMA_CFG_SRCPID_REPLACE_VAL( cdma_registers->CFG,
                                                           MyCfg->SourcePerfID );
    }

    if(MyCfg->FlowControl == CDMA_DEV_TO_DEV || MyCfg->FlowControl == CDMA_MEM_TO_DEV)
    {
        cdma_registers->CFG = CDMA_CFG_DESTPID_REPLACE_VAL( cdma_registers->CFG,
                                                            MyCfg->DestPerfID );
    }

    // set flow control
    cdma_registers->CFG = CDMA_CFG_FLOWCTRL_REPLACE_VAL( cdma_registers->CFG, transfer_type );

    // set endians
    cdma_registers->CFG = CDMA_CFG_SRCENDIAN_REPLACE_VAL( cdma_registers->CFG, 
            cdma_get_device_endian( MyCfg->SourceEndian ));
    cdma_registers->CFG = CDMA_CFG_DESTENDIAN_REPLACE_VAL( cdma_registers->CFG, 
            cdma_get_device_endian( MyCfg->DestEndian ));
    cdma_registers->CFG = CDMA_CFG_DATAUNITSIZE_REPLACE_VAL( cdma_registers->CFG, 
            cdma_get_device_xfer_width( MyCfg->SrcXferWidth ));

    // set interrupt time-out base
    cdma_registers->TimerControl = CDMA_TIMERCONTROL_TIMEBASE_REPLACE_VAL( cdma_registers->TimerControl,
            cdma_get_device_timebase( MyCfg->TimeBase ));

    // save the config
    memcpy(&cdma_channel[channel].config, MyCfg, sizeof(CDMA_CONFIG));

    // return channel to not in use
    status = tx_event_flags_set(&cdma_channel[channel].free, CHANNEL_FREE, TX_OR);
    ASSERT(status == TX_SUCCESS);
 
    return OK;
} 


error_type_t CdmaGetConfig(cdma_handle_t *handle, CDMA_CONFIG *MyCfg)
{
    uint32_t channel = handle->channel;

    ASSERT(config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < config->number_of_channels);
    if(!(channel < config->number_of_channels))
        return FAIL;

    // has this channel been opened?
    ASSERT(cdma_channel[channel].open == true);
    if(cdma_channel[channel].open == false)
        return FAIL;

    // get the config
    memcpy(MyCfg, &cdma_channel[channel].config, sizeof(CDMA_CONFIG));

    return OK;
}


uint32_t CdmaTransfer(cdma_handle_t *handle, uint8_t *src, uint8_t *dest, uint32_t length, uint32_t timeout, CDMA_CALLBACK callback, void *context)
{
    int channel = handle->channel;

    uint32_t flags;

    ASSERT(config != NULL);                         // has the driver been initialized?

    // is channel valid?
    ASSERT(channel < config->number_of_channels);
    if(!(channel < config->number_of_channels))
        return 0;

    // has this channel been opened?
    ASSERT(cdma_channel[channel].open == true);
    if(cdma_channel[channel].open == false)
        return 0;

    // is this channel in use?
    if(tx_event_flags_get(&cdma_channel[channel].free, CHANNEL_FREE, TX_AND_CLEAR, &flags, TX_NO_WAIT) != TX_SUCCESS)
        return 0;

    //save context
    cdma_channel[channel].context = context;
    return(CdmaProcess((uint8_t *)src, (uint8_t *)dest, length, timeout, handle->channel, callback));
}


error_type_t CdmaTransferHalt(cdma_handle_t *handle)
{
    int channel = handle->channel;
    uint32_t result, flags;
    CDMA_REGS_t *cdma_registers;

    ASSERT(config != NULL);                         // has the driver been initialized?

    cdma_registers = (CDMA_REGS_t *)config->dma_base + ( channel * dma_channel_offset);

    // is channel valid?
    ASSERT(channel < config->number_of_channels);
    if(!(channel < config->number_of_channels))
        return FAIL;

    // has this channel been opened?
    ASSERT(cdma_channel[channel].open == true);
    if(cdma_channel[channel].open == false)
        return FAIL;

    // is this blocking?
    ASSERT(cdma_channel[channel].callback != NULL);
    if(cdma_channel[channel].callback == NULL)
        return FAIL;

    // is this channel not in use?
    if(tx_event_flags_get(&cdma_channel[channel].free, CHANNEL_FREE, TX_AND, &flags, TX_NO_WAIT) == TX_SUCCESS)
        return FAIL;

    // Halt the CDMA
    cdma_registers->CFG = CDMA_CFG_HALT_REPLACE_VAL( cdma_registers->CFG, 1 );
    // Disable DMA channel
    cdma_registers->CFG = CDMA_CFG_ENABLE_REPLACE_VAL( cdma_registers->CFG, 0 );
    // Clear interrupts
    cdma_registers->intAck = CDMA_INTACK_CHAINDONEACK_REPLACE_VAL(0, 1);
    cdma_registers->intAck = CDMA_INTACK_TIMEOUTACK_REPLACE_VAL(0, 1);
    cdma_registers->intAck = CDMA_INTACK_TERMCNTACK_REPLACE_VAL(0, 1);

    // return channel to not in use
    result = tx_event_flags_set(&cdma_channel[channel].free, CHANNEL_FREE, TX_OR);
    ASSERT(result == TX_SUCCESS);

    return OK;
}

 
 
/* FUNCTION NAME: CdmaProcess */
 
/** 
 * \brief This routine copies data from src to dest using the cdma block as much as possible.
 * 
 * \param Src The source address to copy from
 * 
 * \param Dest The destination to copy to.
 * 
 * \param Length The number of bytes to copy
 * 
 * \param Timeout How many clock ticks to wait for this to complete.
 * 
 * \param base The cdma channel to use.
 * 
 * \param callback When using as a non-blocking function, this
 * is the call back function.  First parameter(void*) is user
 * defined data.  2nd parameter(uint32_t) is the dma interrupt
 * status.
 * 
 * \retval int 0, aborted; -1, timeout; >0 number of bytes
 * transfered, or in process of being xfered.
 * 
 * 
 **/
static int CdmaProcess(uint8_t *Src, uint8_t *Dest, uint32_t Length, uint32_t Timeout, uint32_t base, CDMA_CALLBACK callback)
{
    int number_of_words, AlignBytes, transfer_size, BytesToCopy, transfer_type, Queued=0, threshold;
    uint32_t result;
    int32_t bytes_copied;
    uint32_t flags;
    uint8_t *AlignedSrc, *AlignedDest;
    uint32_t dcache_line_size = cpu_get_dcache_line_size();
    CDMA_REGS_t *cdma_registers;

    cdma_registers = (CDMA_REGS_t *)( config->dma_base + ( base * dma_channel_offset ));

    // Determine transfer type 
    switch( CDMA_CFG_FLOWCTRL_MASK_SHIFT( cdma_registers->CFG ))
    {
        case 0: //  CDMA_MEM_TO_MEM
            transfer_type = CDMA_MEM_TO_MEM;
            break;
        case 1: // CDMA_MEM_TO_DEV
            transfer_type = CDMA_MEM_TO_DEV;
            break;
        case 2: // CDMA_DEV_TO_MEM
            transfer_type = CDMA_DEV_TO_MEM;
            break;
        case 3: // CDMA_DEV_TO_DEV
            transfer_type = CDMA_DEV_TO_DEV;
            break;
        default:
            transfer_type = 0;
            XASSERT(NULL == &"CdmaProcess: unsupported transfer type", CDMA_CFG_FLOWCTRL_MASK_SHIFT( cdma_registers->CFG ) );
            break;            
    }


    if(((transfer_type == CDMA_MEM_TO_MEM) || (transfer_type == CDMA_MEM_TO_DEV)) && ((uint32_t)Src & 0x3)) {
        ASSERT(NULL == &"CdmaProcess: Source address is not 4 byte aligned");
        return 0;
    }
    if(((transfer_type == CDMA_MEM_TO_MEM) || (transfer_type == CDMA_DEV_TO_MEM)) && ((uint32_t)Dest & 0x3)) {
        ASSERT(NULL == &"CdmaProcess: Destination address is not 4 byte aligned");
        return 0;
    }
    if((transfer_type == CDMA_MEM_TO_MEM) &&
            ((Src == Dest) ||
             (CDMA_CONTROL_SRCADDRINC_MASK_SHIFT( cdma_registers->Control ) && ((Src < Dest) && (Src + Length > Dest))) ||
             (CDMA_CONTROL_DESTADDRINC_MASK_SHIFT( cdma_registers->Control ) && ((Src > Dest) && (Dest + Length > Src))))) {
        ASSERT(NULL == &"CdmaProcess: Src/Dest overlap error");
        return 0;
    }

    bytes_copied = Length;

    // is this a continue sequence?
    if(callback != cdma_continue_callback)
        cdma_channel[base].callback = callback;     // nope, set the callback

    // is this a blocking transfer and block forever?
    if(callback == NULL && Timeout == 0)
        Timeout = TX_WAIT_FOREVER;                          // yep, block forever                          

    //
    // This next section is for making sure we work correctly with the cache.  If we
    // are transferring into a partial cache line it is possible that after we
    // invalidate the cache line and before we transfer that the first of the
    // cache line might get read in and we will lose any data we dma into the memory.
    // The way to fix this is to see if we are xfering to memory and if so move the
    // first part of the xfer using the normal memcpy.
    //
    // Get the flow control bits.
    //

    AlignBytes=((uint32_t)Dest % dcache_line_size);

    // destination address must be aligned for APB (peripheral device) to memory transfer
    if((transfer_type == CDMA_DEV_TO_MEM) && AlignBytes) {
        // return channel to not in use
        result = tx_event_flags_set(&cdma_channel[base].free, CHANNEL_FREE, TX_OR);
        ASSERT(result == TX_SUCCESS);
        return 0; //destination is not aligned
    }

    if((transfer_type == CDMA_MEM_TO_MEM) && AlignBytes)
    {
        //
        // We have a partial cache line.  Figure out how much to xfer.
        //
        AlignBytes = dcache_line_size - AlignBytes;
        // if we have less to xfer than the cache line, limit the xfer here.
        //
        if(AlignBytes > Length)
            AlignBytes = Length;
        memcpy(Dest, Src, AlignBytes);
        //
        // adjust the pointers here.
        //
        Dest += AlignBytes;
        Length -= AlignBytes;
        Src += AlignBytes;
    }
    if (Length > 0)     // if there is more to do, do it here.
    {
        // update the src register in the cdma block.
        cdma_registers->SrcAddr = (uint32_t)Src;

        AlignedSrc = Src;
        AlignedDest = Dest;

        number_of_words = Length; 

        // if dest is memory
        if (((transfer_type == CDMA_MEM_TO_MEM) || (transfer_type == CDMA_DEV_TO_MEM)) && 
                (Length >= dcache_line_size))       // if we span a line size invalidate.
        {
            // Get the cache out of the way (length chomped to 32 byte alignment).
            cpu_dcache_invalidate_region(AlignedDest, Length& ~(dcache_line_size-1)); 
        }

        threshold = dcache_line_size;

        // now move the data.  If num < threshold just use memcpy
        while ((((transfer_type != CDMA_MEM_TO_MEM) && (number_of_words > 0)) // not memory to memory transfer and data?
                    || (number_of_words >= threshold))                       // is memory to memory transfer large enough?
                && (Queued < CDMA_QUEUE_SIZE))                // is queue not full?
        {
            transfer_size = number_of_words;
            //
            // The cdma only supports xfer sizes of maxxfersize bytes, so limit that for each xfer.
            //
            if (transfer_size > maxxfersize)
                transfer_size = maxxfersize;

            if (transfer_type == CDMA_MEM_TO_MEM &&
                    (transfer_size % dcache_line_size) != 0)
            { // Is this total byte transfer not a multiple of a cache line?
                transfer_size = transfer_size - (transfer_size % dcache_line_size );     
            }

            // Now do the transfer.
            //
            // The first block of the chained dma is put into the chip, the
            // rest is stored in memory.
            //
            if (Queued == 0)
            {

                // Program the part directly, this is the first block.
                cdma_registers->DestAddr = (uint32_t)Dest;
                // Put in the size in bytes
                cdma_registers->Control = CDMA_CONTROL_TRANSSIZE_REPLACE_VAL( cdma_registers->Control, 
                        transfer_size );
                // If this is the last xfer, finish it up here.
                if ((number_of_words - transfer_size) < threshold)
                {
                    // only block to xfer, no chain
                    cdma_registers->LLIR = CDMA_LLIR_LLINDEX_REPLACE_VAL( cdma_registers->LLIR, 0 );
                    // interrupt on
                    cdma_registers->intEn = CDMA_INTEN_CHAINDONEEN_REPLACE_VAL( cdma_registers->intEn, 1 );
                }
                else
                {
                    // More stuff to do, set up for that.
                    cdma_registers->LLIR = (uint32_t) &CdmaLink[LINK_OFFSET(base, 0)];
                    // don't interrupt here.
                    cdma_registers->intEn = CDMA_INTEN_CHAINDONEEN_REPLACE_VAL( cdma_registers->intEn, 0 );
                    cdma_registers->intEn = CDMA_INTEN_TERMCNTEN_REPLACE_VAL( cdma_registers->intEn, 0 );
                }
                Queued++;       // how many are queued.
                number_of_words -= transfer_size;    // figure out how many left.
                if ( CDMA_CONTROL_DESTADDRINC_MASK_SHIFT( cdma_registers->Control ))
                {
                    Dest += transfer_size;
                }
                if ( CDMA_CONTROL_SRCADDRINC_MASK_SHIFT( cdma_registers->Control ))
                {
                    Src += transfer_size;
                }
            }
            else
            {
                // Setup a linked xfer.
                // Put in the source address.
                if ( ( transfer_type != CDMA_DEV_TO_MEM ) && ( transfer_type != CDMA_DEV_TO_DEV ) )
                {
                    CdmaLink[LINK_OFFSET(base, Queued-1)].SourceAddress = 
                        (uint8_t *)( cdma_registers->SrcAddr + Queued * maxxfersize );
                }
                else
                {
                    CdmaLink[LINK_OFFSET(base, Queued-1)].SourceAddress = (uint8_t *)( cdma_registers->SrcAddr );
                }
                // Now add in the destination address.
                if ( ( transfer_type != CDMA_MEM_TO_DEV ) && ( transfer_type != CDMA_DEV_TO_DEV ) )
                {
                    CdmaLink[LINK_OFFSET(base, Queued-1)].DestAddress = 
                        (uint8_t *)( cdma_registers->DestAddr + Queued * maxxfersize );
                }
                else
                {
                    CdmaLink[LINK_OFFSET(base, Queued-1)].DestAddress = (uint8_t *)( cdma_registers->DestAddr );
                }
                // set the link to no next link.
                CdmaLink[LINK_OFFSET(base, Queued-1)].NextLink = 0;
                // setup the control word with the length.
                CdmaLink[LINK_OFFSET(base, Queued-1)].CntrlWord = 
                    (transfer_size << CDMA_CONTROL_TRANSSIZE_SHIFT) + 
                    ( cdma_registers->Control & ~CDMA_CONTROL_TRANSSIZE_MASK );
                // update the length to what is left to do.
                number_of_words -= transfer_size;
                // see if we are going to be finished now.
                if (number_of_words < threshold || Queued == CDMA_QUEUE_SIZE - 1)
                {
                    // This is the last xfer, set the interrupt bit.
                    cdma_registers->intEn = CDMA_INTEN_CHAINDONEEN_REPLACE_VAL( cdma_registers->intEn, 1 );
                }
                if (Queued > 1)
                {
                    // 
                    // If this is greater than the 1st time, set up the link for last time.
                    CdmaLink[LINK_OFFSET(base, Queued-2)].NextLink = &CdmaLink[LINK_OFFSET(base, Queued-1)];  // update the queue
                }
                // set up the number queued.
                Queued++;
                // update the transfer pointers.
                if (CDMA_CONTROL_DESTADDRINC_MASK_SHIFT( cdma_registers->Control ))
                {
                    Dest += transfer_size;
                }
                if (CDMA_CONTROL_SRCADDRINC_MASK_SHIFT( cdma_registers->Control ))
                {
                    Src += transfer_size;
                }
            }
        }

        // is the queue maxed out and is there data still left?
        if (Queued >= CDMA_QUEUE_SIZE && Length > (Queued * maxxfersize))
        {
            // to be continued...  setup the continue point for the isr
            cdma_channel[base].cont.Src = Src;
            cdma_channel[base].cont.Dest = Dest;
            cdma_channel[base].cont.Length = Length - (Queued * maxxfersize);
            cdma_channel[base].cont.Timeout = Timeout;
        }
        else
            cdma_channel[base].cont.Length = 0;

        // If doing an M2M copy and either 1) The dma chain is done -or- 2) The chain is full but there are no more transfers
        if (transfer_type == CDMA_MEM_TO_MEM && ((Queued < CDMA_QUEUE_SIZE) || ((Queued >= CDMA_QUEUE_SIZE) && (number_of_words < threshold))))
        {
            // copy the extra junk at the end of the buffer.  If the xfer width > 1 we have
            // to copy the few extra bytes at the end to where they belong.
            //
            BytesToCopy = number_of_words;   // the amount to copy.
            //  if non-zero, copy it.
            if (BytesToCopy)
            {
                memcpy(Dest,Src, BytesToCopy);
            }
        }

        if (transfer_type == CDMA_MEM_TO_DEV)
        {
            //return bytes transfered
            bytes_copied = Length;
        }

        // Enable DMA timer if requested
        if (callback != NULL && Timeout)
        {
            cdma_registers->TimerControl = CDMA_TIMERCONTROL_COUNT_REPLACE_VAL( cdma_registers->TimerControl, Timeout );
            cdma_registers->TimerControl = CDMA_TIMERCONTROL_TIMERENABLE_REPLACE_VAL( cdma_registers->TimerControl, 1 );
            // Turn on interrupt
            cdma_registers->intEn = CDMA_INTEN_TIMEOUTEN_REPLACE_VAL( cdma_registers->intEn, 1 );
        }

        // We wait to invalidate the cache till here to insure the code above does 
        // not mess up the caching.
        // If the destination is memory, invalidate the cache at the destination.
        if ((transfer_type == CDMA_MEM_TO_MEM) || (transfer_type == CDMA_DEV_TO_MEM))
        {
            // get the cache out of the way.
            cpu_dcache_invalidate_region(AlignedDest, Length& ~(dcache_line_size-1));
        }

        // If the source is memory, write back the cache for the source
        if ((transfer_type == CDMA_MEM_TO_MEM) || (transfer_type == CDMA_MEM_TO_DEV))
            cpu_dcache_writeback_region(AlignedSrc, Length);

        // Make sure the descriptor is in memory, not just hanging out in the cache
        cpu_dcache_writeback_region(&CdmaLink[LINK_OFFSET(base, 0)], sizeof(cdma_link_t)*Queued); 
        // start the xfer here.
        cdma_registers->intEn = CDMA_INTEN_CHAINDONEEN_REPLACE_VAL( cdma_registers->intEn, 1 );
        cdma_registers->CFG =  CDMA_CFG_ENABLE_REPLACE_VAL( cdma_registers->CFG, 1 );
        //
        // If we have a non-blocking write, return here.
        if (callback != NULL)
            return bytes_copied;
        // wait for it to complete.
        result = tx_event_flags_get(&CdmaFlags, 1<<base, TX_AND_CLEAR,&flags, Timeout);
        ASSERT(result == TX_SUCCESS || result == TX_NO_EVENTS);
        if (result == TX_NO_EVENTS) //!= TX_SUCCESS)
            bytes_copied = -1;      // timeout return.

    }
    result = tx_event_flags_set(&cdma_channel[base].free, CHANNEL_FREE, TX_OR);
    ASSERT(result == TX_SUCCESS);

    return bytes_copied;  // return the amount copied.
}

#ifdef HAVE_UNIT_TEST
static int cdma_unit_test( void )
{
    int i_res;

    /* run both for the plain unit test */
    i_res = cdma_test();

    return i_res;
}
#endif

#ifdef HAVE_CMD
int cdma_debug_callback(int argc, char *argv[])
{
    int pos, line;
    char *char_ptr;

    if( argc < 2 ) 
    {
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("Supported commands:\n"));
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("'cdma test' - runs all cdma tests\n"));
        return 0;
    }

    pos = 1;

    char_ptr = argv[pos];
    while(*char_ptr) {
        *char_ptr = tolower(*char_ptr);
        char_ptr++;
    }

    if((strncmp( argv[pos],"test",5 ) == 0 ) && ( argc == 2 ) )
    {
        line = 0;

        #ifdef HAVE_UNIT_TEST
        line = cdma_test();
        #else
        line = __LINE__;
        #endif
        if(line) 
        {
            DPRINTF((DBG_LOUD|DBG_OUTPUT), ("ERROR in mrvl_cdma.c line %d\n", line));
        }
    }
    else
    {
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("Unsupported command\n"));
    }
    return 0;
}
#endif
