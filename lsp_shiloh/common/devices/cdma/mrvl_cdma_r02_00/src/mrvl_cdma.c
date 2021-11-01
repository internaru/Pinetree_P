/*
 * ============================================================================
 * Copyright (c) 2008   Marvell International, Ltd. All Rights Reserved
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
#include "intnums.h"
#include "string.h" // used for memcpy and memset
#include "lassert.h"
#include "memAPI.h"
#include "logger.h"

#include "cdma_api.h"
#include "mrvl_cdma_config.h"

#ifdef HAVE_CMD
#include "cmd_proc_api.h"
#endif

#ifdef UNIT_TEST
#include "unit_test_api.h"
#endif

/**
 * 
 * DEFINES
 * 
 **/

#define CDMA_QUEUE_SIZE 65  // the max number of links for a chained dma, 65 is 1MB with 32-bit transfers

#define THRESHOLD 5 // memory to memory transfer threshold, use memcpy if # of transfers <= this value

#define MAXXFERSIZE 0xfff  ///< max xfer size of a single transfer of the cdma

#define LINK_OFFSET(base, link) ((base * CDMA_QUEUE_SIZE) + link)   // compute chained dma link offset

//#define DMA_BASE            dma_base
//#define DMA_CHANNEL_OFFSET  dma_channel_offset

#define CDMAREG(Base, Offset) *((volatile uint32_t *)(((uint32_t)Base * dma_channel_offset) + Offset + dma_base))    // compute register address

#define OFFSET_CDMA_INT_CONTROL 0x4
#define OFFSET_CDMA_TIMER_CTRL  0x8
#define OFFSET_CDMA_SRC_ADDR    0xc
#define OFFSET_CDMA_DST_ADDR    0x10
#define OFFSET_CDMA_CTRL        0x14
#define OFFSET_CDMA_CONFIG      0x18
#define OFFSET_CDMA_LLIR        0x34
// Central DMA Controller Offsets
#define OFFSET_DMA_INT_CONTROL(x)   ((x*dma_channel_offset) + OFFSET_CDMA_INT_CONTROL)
#define OFFSET_DMA_TIMER_CONTROL(x) ((x*dma_channel_offset) + OFFSET_CDMA_TIMER_CTRL)
#define OFFSET_DMA_SRCADDR(x)       ((x*dma_channel_offset) + OFFSET_CDMA_SRC_ADDR)
#define OFFSET_DMA_DSTADDR(x)       ((x*dma_channel_offset) + OFFSET_CDMA_DST_ADDR)
#define OFFSET_DMA_CONTROL(x)       ((x*dma_channel_offset) + OFFSET_CDMA_CTRL)
#define OFFSET_DMA_CONFIG(x)        ((x*dma_channel_offset) + OFFSET_CDMA_CONFIG)
#define OFFSET_DMA_LLIR(x)          ((x*dma_channel_offset) + OFFSET_CDMA_LLIR)

// Timer Control Register
#define DMA_TIMECYCLES_POS      0
#define DMA_TIMECYCLES_MASK     (0x000007FF << DMA_TIMECYCLES_POS)
#define DMA_TIMECYCLES_VAL(a)   ((a) << DMA_TIMECYCLES_POS) // number of "timebase" units for the timeout
#define DMA_TIMEBASE_POS        12
#define DMA_TIMEBASE_MASK       (0x00000007 << DMA_TIMEBASE_POS)
#define DMA_TIMEBASE_VAL(a)     ((a) << DMA_TIMEBASE_POS) // timebase
#define DMA_TB_1us              0x0
#define DMA_TB_10us             0x1
#define DMA_TB_100us            0x2
#define DMA_TB_1ms              0x3
#define DMA_TB_10ms             0x4
#define DMA_TB_100ms            0x5
#define DMA_TIMER_ENABLE        (1 << 15)

// Control Register
#define DMA_XFER_SIZE_POS           0
#define DMA_XFER_SIZE_MASK          (0x00000FFF << DMA_XFER_SIZE_POS)
#define DMA_XFER_SIZE_VAL(a)        ((a) << DMA_XFER_SIZE_POS)
#define DMA_SRC_BURST_SIZE_POS      12
#define DMA_SRC_BURST_SIZE_MASK     (0x0000000F << DMA_SRC_BURST_SIZE_POS)
#define DMA_SRC_BURST_SIZE_VAL(a)   ((a) << DMA_SRC_BURST_SIZE_POS)
#define DMA_DST_BURST_SIZE_POS      16
#define DMA_DST_BURST_SIZE_MASK     (0x0000000F << DMA_DST_BURST_SIZE_POS)
#define DMA_DST_BURST_SIZE_VAL(a)   ((a) << DMA_DST_BURST_SIZE_POS)
#define DMA_SRC_XFER_WIDTH_POS      20
#define DMA_SRC_XFER_WIDTH_MASK     (0x00000003 << DMA_SRC_XFER_WIDTH_POS)
#define DMA_SRC_XFER_WIDTH_VAL(a)   ((a) << DMA_SRC_XFER_WIDTH_POS)
#define DMA_DST_XFER_WIDTH_POS      22
#define DMA_DST_XFER_WIDTH_MASK     (0x00000003 << DMA_DST_XFER_WIDTH_POS)
#define DMA_DST_XFER_WIDTH_VAL(a)   ((a) << DMA_DST_XFER_WIDTH_POS)
#define DMA_SRC_INCREMENT           (1 << 24)
#define DMA_DST_INCREMENT           (1 << 25)
#define DMA_ZERO_DST_POS            26
#define DMA_ZERO_DST                (1 << 26)
#define DMA_MOD_DST_POS            DMA_ZERO_DST_POS
#define DMA_MOD_DST                DMA_ZERO_DST
#define DMA_TERM_CNT_ENABLE         (1 << 31)

// Config Register
#define DMA_SRC_PERIPH_ID_POS       0
#define DMA_SRC_PERIPH_ID_MASK      (0x0000001F << DMA_SRC_PERIPH_ID_POS)
#define DMA_SRC_PERIPH_ID_VAL(a)    ((a) << DMA_SRC_PERIPH_ID_POS)
#define DMA_DST_PERIPH_ID_POS       8
#define DMA_DST_PERIPH_ID_MASK      (0x0000001F << DMA_DST_PERIPH_ID_POS)
#define DMA_DST_PERIPH_ID_VAL(a)    ((a) << DMA_DST_PERIPH_ID_POS)
#define DMA_FCTT_POS                16                                   // Flow control and xfer type pos
#define DMA_FCTT_MASK               (0x00000007 << DMA_FCTT_POS)        // Flow control and xfer type mask
#define DMA_FCTT_VAL(a)             ((a) << DMA_FCTT_POS)               // Flow control and xfer type val macro
#define DMA_FIFO_EMPTY              (1 << 19)
#define DMA_HALT                    (1 << 20)
#define DMA_CHAN_ENABLE             (1 << 21)
#define DMA_SRC_ENDIAN_POS          22
#define DMA_SRC_ENDIAN_MASK         (0x00000003 << DMA_SRC_ENDIAN_POS)
#define DMA_SRC_ENDIAN_VAL(a)       ((a) << DMA_SRC_ENDIAN_POS)
#define DMA_DST_ENDIAN_POS          24
#define DMA_DST_ENDIAN_MASK         (0x00000003 << DMA_DST_ENDIAN_POS)
#define DMA_DST_ENDIAN_VAL(a)       ((a) << DMA_DST_ENDIAN_POS)
#define DMA_TERMINAL_COUNT_INT_MASK (1 << 26)
#define DMA_TIMEOUT_INT_MASK        (1 << 27)

/** 
 * \brief Callback flags for /a status parameter of callback
 * function specified in CdmaTransfer().
 **/
#define CDMA_TERMINAL_COUNT_INTSTAT      (1 << 0)
#define CDMA_TIMEOUT_INTSTAT             (1 << 1)
#define CDMA_TERMINAL_COUNT_RAWINTSTAT   (1 << 4)
#define CDMA_TIMEOUT_RAWINTSTAT          (1 << 5)   //!< Device timed out between transfers
#define CDMA_CHAIN_DONE                  (1 << 6)

#define CHANNEL_FREE    1<<0

#define HANDLE_NOT_USED -1

/**
 * 
 * TYPEDEFS
 * 
 **/

/**
 * Chained DMA
 **/
typedef struct CDMA_LINK0
{
    volatile uint8_t *SourceAddress;            // source address
    volatile uint8_t *DestAddress;              // destination address
    volatile struct CDMA_LINK0 *NextLink;    // next link in chain
    volatile uint32_t  CntrlWord;              // control register
} CDMA_LINK;

/**
 * Continue point
 **/
typedef struct CDMA_CONTINUE_STRUCT
{
    uint8_t *Src;                      // source address
    uint8_t *Dest;                     // destination address
    uint32_t Length;                  // remaining bytes
    uint32_t Timeout;                 // timeout
} CDMA_CONTINUE;

/**
 * Channel info
 **/
typedef struct CDMA_CHANNEL_STRUCT
{
    TX_EVENT_FLAGS_GROUP free;      // channel is in use                                    
    CDMA_CONTINUE cont;             // continue point
    void *callback;                 // user callback for non-blocking transfer
    void *context;                  // user data passed to callback
    bool open;                      // channel is open
    CDMA_CONFIG config;             // channel configuration
} CDMA_CHANNEL;

struct cdma_handle_s
{
    uint32_t    channel;
};


/**
 * 
 * LOCALS
 * 
 **/
static CDMA_CHANNEL *cdma_channel;              // pointer to channel info list
static CDMA_PLATFORM_CONFIG *config = NULL;     // platform configuration parameters
static CDMA_LINK *CdmaLink;                     // pointer to chained list storage
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
static int CdmaProcess(uint8_t *Src, uint8_t *Dest, uint32_t Length, uint32_t Timeout, uint32_t Base, CDMA_CALLBACK callback);
#ifdef UNIT_TEST
static int cdma_unit_test( void );
#endif
#ifdef HAVE_CMD
static int cdma_debug_callback(int argc, char *argv[]);
#endif
#if defined (HAVE_CMD) || defined (UNIT_TEST)
static int cdma_test(void);
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
            return 2;
            break;
        case CDMA_16_BITS:
            return 1;
            break;
        case CDMA_32_BITS:
            return 0;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_xfer_width: unsupported width");
            return 0;
            break;
    }
}


/** 
 * \brief Get the device destination modify command.
 * 
 * \param cmd The api type.
 * 
 * \return Device destination modify command value.
 **/
static uint8_t cdma_get_device_dest_modify_cmd(CDMA_DEST_MODIFY_CMD cmd)
{
    switch(cmd) {
        case CDMA_NO_MOD_DEST:
            return 0;
            break;
        case CDMA_ZERO_DEST:
            return 1;
            break;
/*
        case CDMA_FILL_DEST:
            return 2;
            break;
*/
        default:
            ASSERT(NULL == &"cdma_get_device_dest_modify_cmd: unsupported command");
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
            return 1;
            break;
        case CDMA_BURST_2:
            return 2;
            break;
        case CDMA_BURST_4:
            return 4;
            break;
        case CDMA_BURST_8:
            return 8;
            break;
        default:
            ASSERT(NULL == &"cdma_get_device_burst_size: unsupported size");
            return 1;
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

    // find the channel to be serviced
    for(i = 0; i < config->number_of_channels; i++)
    {
        // is it this channel?
        if(CDMAREG(i, OFFSET_CDMA_INT_CONTROL) & CDMA_TERMINAL_COUNT_INTSTAT)
        {
            // should this continue?
            if(cdma_channel[i].cont.Length) {
                CDMAREG(i, OFFSET_CDMA_INT_CONTROL) = CDMA_TERMINAL_COUNT_RAWINTSTAT | CDMA_TIMEOUT_RAWINTSTAT | CDMA_CHAIN_DONE; // clear the interrupt
                CdmaProcess(cdma_channel[i].cont.Src, cdma_channel[i].cont.Dest, cdma_channel[i].cont.Length, cdma_channel[i].cont.Timeout, i, cdma_continue_callback);
            } else {
                // is this blocking or non-blocking?
                if(cdma_channel[i].callback != NULL) {
                    // non-blocking
                    MyRoutine = cdma_channel[i].callback;
                    status = CDMAREG(i, OFFSET_CDMA_INT_CONTROL);       // get the interrupt status
                    CDMAREG(i, OFFSET_CDMA_INT_CONTROL) = CDMA_TERMINAL_COUNT_RAWINTSTAT | CDMA_TIMEOUT_RAWINTSTAT | CDMA_CHAIN_DONE; // clear the interrupt
                    MyRoutine(cdma_channel[i].context, status);         // call the callback

                    // return channel to not in use
                    status = tx_event_flags_set(&cdma_channel[i].free, CHANNEL_FREE, TX_OR);
                    ASSERT(status == TX_SUCCESS);
                } else {
                    // blocking
                    CDMAREG(i, OFFSET_CDMA_INT_CONTROL) = CDMA_TERMINAL_COUNT_RAWINTSTAT | CDMA_TIMEOUT_RAWINTSTAT | CDMA_CHAIN_DONE; // clear the interrupt
                    tx_event_flags_set(&CdmaFlags, 1<<i, TX_OR);        // notify CdmaProcess() of the interrupt
                }
            }
        }
    }
}


void CdmaInit()
{
    uint32_t status;
    uint32_t i;
    error_type_t result;
    #ifdef UNIT_TEST
    int unit_test_res;
    #endif
    #ifdef HAVE_CMD
    int i_res;
    #endif

    ASSERT(config == NULL);                         // has CdmaInit() already been called?
    result = cdma_platform_get_config(&config); 
    ASSERT(result == OK);
    ASSERT(config != NULL);

    dma_base = config->dma_base;
    dma_channel_offset = config->dma_channel_offset;

    maxxfersize = MAXXFERSIZE - (MAXXFERSIZE % cpu_get_dcache_line_size());

    cdma_handles = (cdma_handle_t *)MEM_MALLOC((sizeof(cdma_handle_t) * config->number_of_channels));
    ASSERT( NULL != cdma_handles );

    // allocate memory for cdma channel list
    cdma_channel = (CDMA_CHANNEL *)MEM_MALLOC( (sizeof(CDMA_CHANNEL)*config->number_of_channels));
    ASSERT(cdma_channel != NULL);
    memset(cdma_channel,0,sizeof(CDMA_CHANNEL)*config->number_of_channels);

    // allocate memory for cdma chain lists
    CdmaLink = (CDMA_LINK *)MEM_MALLOC( (sizeof(CDMA_LINK)*CDMA_QUEUE_SIZE*config->number_of_channels) );
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

#ifdef UNIT_TEST
    /* also register with the unit test manager */
    unit_test_res = unittest_register_testcase("cdma", cdma_unit_test);
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
    void *Base = (void *)handle->channel;
    uint32_t status, flags;

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

    // set source address increment
    if(MyCfg->SrcIncrement == CDMA_INC)
        CDMAREG(Base, OFFSET_CDMA_CTRL) |= DMA_SRC_INCREMENT;
    else if(MyCfg->SrcIncrement == CDMA_NO_INC)
        CDMAREG(Base, OFFSET_CDMA_CTRL) &= ~DMA_SRC_INCREMENT;
    else
        ASSERT(NULL == &"CdmaSetConfig: unsupported source address increment");

    // set destination address increment
    if(MyCfg->DestIncrement == CDMA_INC)
        CDMAREG(Base, OFFSET_CDMA_CTRL) |= DMA_DST_INCREMENT;
    else if(MyCfg->DestIncrement == CDMA_NO_INC)
        CDMAREG(Base, OFFSET_CDMA_CTRL) &= ~DMA_DST_INCREMENT;
    else
        ASSERT(NULL == &"CdmaSetConfig: unsupported destination address increment");

    // set destination modify
//    ASSERT(MyCfg->DestModify == CDMA_NO_MOD_DEST);
    CDMAREG(Base, OFFSET_CDMA_CTRL) = (CDMAREG(Base, OFFSET_CDMA_CTRL) & ~DMA_MOD_DST) |
                                       cdma_get_device_dest_modify_cmd(MyCfg->DestModify)<<DMA_MOD_DST_POS;

    // set transfer widths
    CDMAREG(Base, OFFSET_CDMA_CTRL) = (CDMAREG(Base, OFFSET_CDMA_CTRL) & ~DMA_SRC_XFER_WIDTH_MASK) |
                                       cdma_get_device_xfer_width(MyCfg->SrcXferWidth)<<DMA_SRC_XFER_WIDTH_POS;
    CDMAREG(Base, OFFSET_CDMA_CTRL) = (CDMAREG(Base, OFFSET_CDMA_CTRL) & ~DMA_DST_XFER_WIDTH_MASK) |
                                       cdma_get_device_xfer_width(MyCfg->DestXferWidth)<<DMA_DST_XFER_WIDTH_POS;

    // set burst sizes
    CDMAREG(Base, OFFSET_CDMA_CTRL) = (CDMAREG(Base, OFFSET_CDMA_CTRL) & ~DMA_SRC_BURST_SIZE_MASK) |
                                       cdma_get_device_burst_size(MyCfg->SrcBurst)<<DMA_SRC_BURST_SIZE_POS;
    CDMAREG(Base, OFFSET_CDMA_CTRL) = (CDMAREG(Base, OFFSET_CDMA_CTRL) & ~DMA_DST_BURST_SIZE_MASK) |
                                       cdma_get_device_burst_size(MyCfg->DestBurst)<<DMA_DST_BURST_SIZE_POS;
    // set peripheral ids
    if(MyCfg->FlowControl == CDMA_DEV_TO_DEV || MyCfg->FlowControl == CDMA_DEV_TO_MEM)
        CDMAREG(Base, OFFSET_CDMA_CONFIG) = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & ~DMA_SRC_PERIPH_ID_MASK) |
                                            MyCfg->SourcePerfID<<DMA_SRC_PERIPH_ID_POS;

    if(MyCfg->FlowControl == CDMA_DEV_TO_DEV || MyCfg->FlowControl == CDMA_MEM_TO_DEV)
        CDMAREG(Base, OFFSET_CDMA_CONFIG) = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & ~DMA_DST_PERIPH_ID_MASK) |
                                            MyCfg->DestPerfID<<DMA_DST_PERIPH_ID_POS;

    // set flow control
    CDMAREG(Base, OFFSET_CDMA_CONFIG) = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & ~DMA_FCTT_MASK) |
                                         cdma_get_device_flow_control(MyCfg->FlowControl)<<DMA_FCTT_POS;

    // set endians
    CDMAREG(Base, OFFSET_CDMA_CONFIG) = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & ~DMA_SRC_ENDIAN_MASK) |
                                         cdma_get_device_endian(MyCfg->SourceEndian)<<DMA_SRC_ENDIAN_POS;
    CDMAREG(Base, OFFSET_CDMA_CONFIG) = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & ~DMA_DST_ENDIAN_MASK) |
                                         cdma_get_device_endian(MyCfg->DestEndian)<<DMA_DST_ENDIAN_POS;

    // set interrupt time-out base
    CDMAREG(Base, OFFSET_CDMA_TIMER_CTRL) = (CDMAREG(Base, OFFSET_CDMA_TIMER_CTRL) &
                                             ~(DMA_TIMER_ENABLE | DMA_TIMEBASE_MASK | DMA_TIMECYCLES_MASK)) |
                                             cdma_get_device_timebase(MyCfg->TimeBase)<<DMA_TIMEBASE_POS;

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

    // is this channel is use?
    if(tx_event_flags_get(&cdma_channel[channel].free, CHANNEL_FREE, TX_AND_CLEAR, &flags, TX_NO_WAIT) != TX_SUCCESS)
        return 0;

    //save context
    cdma_channel[channel].context = context;
    return(CdmaProcess((uint8_t *)src, (uint8_t *)dest, length, timeout, handle->channel, callback));
}


error_type_t CdmaTransferHalt(cdma_handle_t *handle)
{
    int channel = handle->channel;
    void *Base = (void *)handle->channel;;
    uint32_t result, flags;

    ASSERT(config != NULL);                         // has the driver been initialized?

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

    // halt the CDMA
    CDMAREG(Base, OFFSET_CDMA_CONFIG) |= DMA_HALT;
    CDMAREG(Base, OFFSET_CDMA_CONFIG) &= ~(DMA_CHAN_ENABLE | DMA_HALT);
    CDMAREG(Base, OFFSET_CDMA_INT_CONTROL) = CDMA_TERMINAL_COUNT_RAWINTSTAT | CDMA_TIMEOUT_RAWINTSTAT | CDMA_CHAIN_DONE; // clear the interrupt

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
 * \param Base The cdma channel to use.
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
static int CdmaProcess(uint8_t *Src, uint8_t *Dest, uint32_t Length, uint32_t Timeout, uint32_t Base, CDMA_CALLBACK callback)
{
    int Num, AlignBytes, TransferSize, BytesToCopy, XferType, XferWidth, Queued=0, threshold;
    uint32_t result;
    int32_t Len;
    uint32_t flags;
//    CDMA_XFER_WIDTH TempWidth;
    uint32_t TransferWidthInBytes;
    uint8_t *AlignedSrc, *AlignedDest;
    uint32_t dcache_line_size = cpu_get_dcache_line_size();

    XferType = (CDMAREG(Base, OFFSET_CDMA_CONFIG) & DMA_FCTT_MASK)>>DMA_FCTT_POS;    
    
    if(((XferType == CDMA_MEM_TO_MEM) || (XferType == CDMA_MEM_TO_DEV)) && ((uint32_t)Src & 0x3)) {
        ASSERT(NULL == &"CdmaProcess: Source address is not 4 byte aligned");
        return 0;
    }
    if(((XferType == CDMA_MEM_TO_MEM) || (XferType == CDMA_DEV_TO_MEM)) && ((uint32_t)Dest & 0x3)) {
        ASSERT(NULL == &"CdmaProcess: Destination address is not 4 byte aligned");
        return 0;
    }
    if((XferType == CDMA_MEM_TO_MEM) &&
       ((Src == Dest) ||
        ((CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_SRC_INCREMENT) && ((Src < Dest) && (Src + Length > Dest))) ||
        ((CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_DST_INCREMENT) && ((Src > Dest) && (Dest + Length > Src))))) {
        ASSERT(NULL == &"CdmaProcess: Src/Dest overlap error");
        return 0;
    }

    Len = Length;

    // is this a continue sequence?
    if(callback != cdma_continue_callback)
        cdma_channel[Base].callback = callback;     // nope, set the callback

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

    // destination address must be aligned for APB to memory transfer
    if((XferType == CDMA_DEV_TO_MEM) && AlignBytes) {
        // return channel to not in use
        result = tx_event_flags_set(&cdma_channel[Base].free, CHANNEL_FREE, TX_OR);
        ASSERT(result == TX_SUCCESS);
        return 0; //destination is not aligned
    }

    if((XferType == CDMA_MEM_TO_MEM) && AlignBytes)
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
        //
        CDMAREG(Base, OFFSET_CDMA_SRC_ADDR) = (uint32_t) Src;

        AlignedSrc = Src;
        AlignedDest = Dest;
        //
        if (((XferType == CDMA_MEM_TO_MEM) || (XferType == CDMA_DEV_TO_MEM)) && // if dest is memory
            (Length >= dcache_line_size))       // if we span a line size invalidate.
        {
            cpu_dcache_invalidate_region(AlignedDest, Length& ~(dcache_line_size-1)); // get the cache out of the way.
        }
        // Find the xfer width and figure how many xfer width's to xfer.
        //
        TransferWidthInBytes = (CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_SRC_XFER_WIDTH_MASK) >> 
                               DMA_SRC_XFER_WIDTH_POS; // get the xfer width of the source

        switch (TransferWidthInBytes)
        {
            case 2: // CDMA_8_BITS
                XferWidth = 1;
                Num = Length; 
                break;
            case 1: // CDMA_16_BITS
                XferWidth = 2;
                Num = Length >> 1; 
                break;
            case 0: // CDMA_32_BITS
                XferWidth = 4;
                Num = Length >> 2; 
                break;
            default:
                ASSERT(NULL == &"CdmaProcess: unsupported TransferWidthInBytes");
                XferWidth = 1;
                Num = Length; 
                break;            
        }

        threshold = dcache_line_size >> (XferWidth >> 1);

        // now move the data.  If num<= THRESHOLD just use memcpy
        while ((((XferType != CDMA_MEM_TO_MEM) && (Num > 0)) // not memory to memory transfer and data?
                || (Num >= threshold))                       // is memory to memory transfer large enough?
               && (Queued < CDMA_QUEUE_SIZE))                // is queue not full?
        {
            TransferSize = Num;
            //
            // The cdma only supports xfer sizes of maxxfersize words, so limit that for each xfer.
            //
            if (TransferSize > maxxfersize)
                TransferSize = maxxfersize;

            if (XferType == CDMA_MEM_TO_MEM &&
                ((TransferSize * XferWidth) % dcache_line_size) != 0)
            { // Is this total byte transfer not a multiple of a cache line?
                TransferSize = TransferSize - (((TransferSize * XferWidth) % cpu_get_dcache_line_size()) >> (XferWidth >> 1));     
            }

            // Now do the transfer.
            //
            // The first block of the chained dma is put into the chip, the
            // rest is stored in memory.
            //
            if (Queued == 0)
            {

                // Program the part directly, this is the first block.
                CDMAREG(Base, OFFSET_CDMA_DST_ADDR) = (uint32_t) Dest;
                // put in the size
                CDMAREG(Base, OFFSET_CDMA_CTRL) = TransferSize + (CDMAREG(Base, OFFSET_CDMA_CTRL)& ~DMA_XFER_SIZE_MASK);
                // if this is the last xfer, finish it up here.
                if (TransferSize == Num)
                {
                    CDMAREG(Base,OFFSET_CDMA_LLIR) = 0;    // only block to xfer, no chain
                    CDMAREG(Base, OFFSET_CDMA_CTRL) |= DMA_TERM_CNT_ENABLE;
                    CDMAREG(Base, OFFSET_CDMA_CONFIG) |= DMA_TERMINAL_COUNT_INT_MASK;// interrupt on
                }
                else
                {
                    // More stuff to do, set up for that.
                    //
                    CDMAREG(Base, OFFSET_CDMA_LLIR) = (uint32_t) &CdmaLink[LINK_OFFSET(Base, 0)]; //Base];
                    CDMAREG(Base, OFFSET_CDMA_CTRL) &= ~DMA_TERM_CNT_ENABLE; // don't interrupt here.
                }
                Queued++;       // how many are queued.
                Num -= TransferSize;    // figure out how many left.
                if (CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_DST_INCREMENT)
                    Dest += TransferSize * XferWidth;
                if (CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_SRC_INCREMENT)
                    Src += TransferSize * XferWidth;
            }
            else
            {
                // Setup a linked xfer.
                // Put in the source address.
                CdmaLink[LINK_OFFSET(Base, Queued-1)].SourceAddress = (uint8_t *)(CDMAREG(Base, OFFSET_CDMA_SRC_ADDR) + 
                                                                                  Queued * maxxfersize * XferWidth * (XferType != CDMA_DEV_TO_MEM && XferType != CDMA_DEV_TO_DEV));
                // Now add in the destination address.
                CdmaLink[LINK_OFFSET(Base, Queued-1)].DestAddress = (uint8_t *)(CDMAREG(Base, OFFSET_CDMA_DST_ADDR) +
                                                                                Queued * maxxfersize * XferWidth * (XferType != CDMA_MEM_TO_DEV && XferType != CDMA_DEV_TO_DEV));
                // set the link to no next link.
                CdmaLink[LINK_OFFSET(Base, Queued-1)].NextLink = 0;
                // setup the control word with the length.
                CdmaLink[LINK_OFFSET(Base, Queued-1)].CntrlWord = TransferSize + (CDMAREG(Base, OFFSET_CDMA_CTRL)& ~DMA_XFER_SIZE_MASK);
                // update the length to what is left to do.
                Num -= TransferSize;
                // see if we are going to be finished now.
                if (Num < threshold || Queued == CDMA_QUEUE_SIZE - 1)
                {
                    // This is the last xfer, set the interrupt bit.
                    CdmaLink[LINK_OFFSET(Base, Queued-1)].CntrlWord |= DMA_TERM_CNT_ENABLE;
                }
                if (Queued > 1)
                {
                    // 
                    // If this is greater than the 1st time, set up the link for last time.
                    CdmaLink[LINK_OFFSET(Base, Queued-2)].NextLink = &CdmaLink[LINK_OFFSET(Base, Queued-1)];  // update the queue
                }
                // set up the number queued.
                Queued++;
                // update the transfer pointers.
                if (CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_DST_INCREMENT)
                    Dest += TransferSize * XferWidth;
                if (CDMAREG(Base, OFFSET_CDMA_CTRL) & DMA_SRC_INCREMENT)
                    Src += TransferSize * XferWidth;
            }
        }

        // is the queue maxed out and is there data still left?
        if (Queued >= CDMA_QUEUE_SIZE && Length > (Queued * maxxfersize * XferWidth))
        {
            // to be continued...  setup the continue point for the isr
            cdma_channel[Base].cont.Src = Src;
            cdma_channel[Base].cont.Dest = Dest;
            cdma_channel[Base].cont.Length = Length - (Queued * maxxfersize * XferWidth);
            cdma_channel[Base].cont.Timeout = Timeout;
        }
        else
            cdma_channel[Base].cont.Length = 0;


        if (XferType == CDMA_MEM_TO_MEM && Queued < CDMA_QUEUE_SIZE)
        {
            // copy the extra junk at the end of the buffer.  If the xfer width > 1 we have
            // to copy the few extra bytes at the end to where they belong.
            //
            BytesToCopy = Num * XferWidth + (Length % XferWidth);   // the amount to copy.
            //  if non-zero, copy it.
            if (BytesToCopy)
            {
                memcpy(Dest,Src, BytesToCopy);
            }
        }

        if (XferType == CDMA_MEM_TO_DEV)
        {
            //return bytes transfered
            Len = Length - (Length % XferWidth);        // subtract leftover bytes NOT transfered
        }

        // Enable DMA timer if requested
        if (callback != NULL && Timeout)
        {
            CDMAREG(Base, OFFSET_CDMA_TIMER_CTRL) |= (DMA_TIMER_ENABLE | DMA_TIMECYCLES_VAL(Timeout));
            CDMAREG(Base, OFFSET_CDMA_CONFIG) |= DMA_TIMEOUT_INT_MASK;
        }

        // We wait to invalidate the cache till here to insure the code above does 
        // not mess up the caching.
        // If the destination is memory, invalidate the cache at the destination.
        if ((XferType == CDMA_MEM_TO_MEM) || (XferType == CDMA_DEV_TO_MEM))
        {
            uint32_t new_length;
            if (0 != Length % dcache_line_size)
            {
                new_length = Length + (dcache_line_size - Length);  
            }
            else
            {
                new_length = Length;
            }
            cpu_dcache_invalidate_region(AlignedDest, new_length); // get the cache out of the way.
        }

        // If the source is memory, write back the cache for the source
        if ((XferType == CDMA_MEM_TO_MEM) || (XferType == CDMA_MEM_TO_DEV))
            cpu_dcache_writeback_region(AlignedSrc, Length);

        // Make sure the descriptor is in memory, not just hanging out in the cache
        cpu_dcache_writeback_region(&CdmaLink[LINK_OFFSET(Base, 0)], sizeof(CDMA_LINK)*Queued); 
        // start the xfer here.
        CDMAREG(Base, OFFSET_CDMA_CONFIG) |= DMA_TERMINAL_COUNT_INT_MASK | DMA_CHAN_ENABLE;
        //
        // If we have a non-blocking write, return here.
        if (callback != NULL)
            return Len;
        // wait for it to complete.
        result = tx_event_flags_get(&CdmaFlags, 1<<Base, TX_AND_CLEAR,&flags, Timeout);
        ASSERT(result == TX_SUCCESS || result == TX_NO_EVENTS);
        if (result == TX_NO_EVENTS) //!= TX_SUCCESS)
            Len = -1;      // timeout return.

    }
    result = tx_event_flags_set(&cdma_channel[Base].free, CHANNEL_FREE, TX_OR);
    ASSERT(result == TX_SUCCESS);

    return Len;  // return the amount copied.
}


#ifdef UNIT_TEST
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
        /* silently ignore incomplete command lines */
        return 0;
    }

    pos = 1;

    char_ptr = argv[pos];
    while(*char_ptr) {
        *char_ptr = tolower(*char_ptr);
        char_ptr++;
    }

    if(strncmp(argv[pos],"test",4) == 0) {
        line = cdma_test();
        if(line) {
            DPRINTF((DBG_LOUD|DBG_OUTPUT), ("ERROR in mrvl_cdma.c line %d\n", line));
        }
    }

    return 0;
}
#endif

#if defined (HAVE_CMD) || defined (UNIT_TEST)
#define DMA_TEST_SIZE 16*1024*1024
static int cdma_test(void)
{
    UINT32 start1, start2, end1, end2, test_size = DMA_TEST_SIZE, i;
    BOOL flag = FALSE;
    cdma_handle_t *handle = NULL;
    CDMA_CONFIG MyCfg;
    uint8_t *destbuf, *srcbuf;
    int result = 0;

    destbuf = (uint8_t *)MEM_MALLOC_ALIGN((sizeof(uint8_t) * DMA_TEST_SIZE), 4);
    ASSERT( destbuf != NULL );
    srcbuf = (uint8_t *)MEM_MALLOC_ALIGN((sizeof(uint8_t) * DMA_TEST_SIZE), 4);
    ASSERT( srcbuf != NULL );

    MyCfg.SrcBurst = CDMA_BURST_8;
    MyCfg.DestBurst = CDMA_BURST_8;
    MyCfg.SrcXferWidth = CDMA_32_BITS;
    MyCfg.DestXferWidth = CDMA_32_BITS;
    MyCfg.SourceEndian = CDMA_ABCD_TO_ABCD;
    MyCfg.DestEndian = CDMA_ABCD_TO_ABCD;
    MyCfg.FlowControl = CDMA_MEM_TO_MEM;
    MyCfg.SrcIncrement = CDMA_INC;
    MyCfg.DestIncrement = CDMA_INC;
    MyCfg.DestModify = CDMA_NO_MOD_DEST;
    MyCfg.TimeBase = CDMA_1_US;

    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("\n  bytes * CDMA clock count * memcpy clock count\n"));

    for(i = 0; i < DMA_TEST_SIZE; i = i + 4) {
        srcbuf[i + 0] = (i & 0x000000ff);
        srcbuf[i + 1] = (i & 0x0000ff00) >> 8;
        srcbuf[i + 2] = (i & 0x00ff0000) >> 16;
        srcbuf[i + 3] = (i & 0xff000000) >> 24;
    }

    while(test_size) {

        memset(destbuf,0,sizeof(destbuf));

        if(CdmaOpen(CDMA_FIND_CHANNEL, &handle) == OK) {
            if(CdmaSetConfig(handle, &MyCfg) == FAIL) {
                result = __LINE__;
                goto bail;
            }
    
            memset(&MyCfg,0,sizeof(MyCfg));
            if(CdmaGetConfig(handle, &MyCfg) == FAIL) {
                result = __LINE__;
                goto bail;
            }

            start1 = cpu_get_ccount();
            if(test_size != CdmaTransfer(handle, (UINT8 *)srcbuf, (UINT8 *)destbuf, test_size, 0, NULL, NULL)) {
                DPRINTF((DBG_LOUD|DBG_OUTPUT), ("INCOMPLETE CDMA transfer\n"));
                result = __LINE__;
                goto bail;
            }

            if(CdmaClose(handle) == FAIL) {
                result = __LINE__;
                goto bail;
            }

        } else {
            result = __LINE__;
            goto bail;
        }

        end1 = cpu_get_ccount();
        
        if (test_size >= 512)
            for(i = 0; i < test_size; i = i + 4)
                if (!(destbuf[i + 0] == (i & 0x000000ff) &&
                      destbuf[i + 1] == (i & 0x0000ff00) >> 8 &&
                      destbuf[i + 2] == (i & 0x00ff0000) >> 16 &&
                      destbuf[i + 3] == (i & 0xff000000) >> 24)) {
                    DPRINTF((DBG_LOUD|DBG_OUTPUT), ("%7d  COMPARE MISMATCH %d\n", i));
                    result = __LINE__;
                    goto bail;
                }
                
        start2 = cpu_get_ccount();                
        memcpy(destbuf, srcbuf, test_size);
        
        //cpu_dcache_writeback_region(destbuf, test_size); 
        
        end2 = cpu_get_ccount();
        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("%7d           %8d             %8d", test_size, end1-start1, end2-start2));
        if (!flag  && (end2 - start2 < end1-start1)) {
            DPRINTF((DBG_LOUD|DBG_OUTPUT), (" <<<"));
            flag = TRUE;
        }

        DPRINTF((DBG_LOUD|DBG_OUTPUT), ("\n"));
        test_size >>= 1;
    }

bail:
    MEM_FREE_AND_NULL(destbuf);
    MEM_FREE_AND_NULL(srcbuf);

    return result;
}
#endif
