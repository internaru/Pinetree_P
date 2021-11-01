/*
 * ============================================================================
 * Copyright (c) 2008-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/** 
 * \file cdma_api.h
 * 
 * \brief CDMA driver.
 * 
 * This driver provides functionality to execute DMA transfers.
 *  This file contains all driver api functions and data types.
 *
 * Call CdmaInit() at system initialization before using any driver functions.
 *
 * To perform a DMA transfer:
 * - Open a channel with CdmaOpen().
 * - Set the transfer configuration parameters with
 * CdmaSetConfig().
 * - Execute the transfer with CdmaTransfer().
 * - Close channel with CdmaClose().
 *
 **/

#ifndef _CDMA_H
#define _CDMA_H

#include <stdint.h>

#include "error_types.h"
#include "cdma_peripheral_ids.h"


#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \brief Pass to CdmaOpen to open the first available channel.
  **/
#define CDMA_FIND_CHANNEL   -1

/** 
 * \brief Device to device flow control.
 * 
 * This specifies the devices used in the transfer. The first is
 * the source and the second is the destination.
 **/
typedef enum
{
    CDMA_MEM_TO_MEM,        //!< Memory to memory.
    CDMA_MEM_TO_DEV,        //!< Memory to device.
    CDMA_DEV_TO_MEM,        //!< Device to memory.
    CDMA_DEV_TO_DEV         //!< Device to device.
} CDMA_FLOWCONTROL;

/** 
 * \brief Device endian type.  This defines the byte placement
 * within a 32-bit word.  Only use #CDMA_ABCD_TO_ABCD for memory
 * to memory transfers.
 */
typedef enum
{
    CDMA_ABCD_TO_ABCD,      
    CDMA_ABCD_TO_CDAB,
    CDMA_ABCD_TO_BADC,
    CDMA_ABCD_TO_DCBA,
    CDMA_LITTLE_ENDIAN,
    CDMA_BIG_ENDIAN
} CDMA_ENDIAN;

/** 
 * \brief Transfer width in bits.
 * 
 * This defines the size of the word transfer.
 **/ 
typedef enum
{
    CDMA_8_BITS,            //!< 8 bit.
    CDMA_16_BITS,           //!< 16 bit.
    CDMA_32_BITS,           //!< 32 bit.
} CDMA_XFER_WIDTH;

/** 
 * \brief Burst transfer size.
 * 
 * This defines throughput performance of the device.  Consult
 * hardware specifications when selecting a size.  If the
 * transfer width is less than 32 bits or the device is
 * non-bursting, then the size must be #CDMA_BURST_1.
 * 
 * Typically a transfer to/from memory can use the maximum size.
 **/
typedef enum
{
    CDMA_BURST_1,           //!< Size 1.
    CDMA_BURST_2,           //!< Size 2.
    CDMA_BURST_4,           //!< Size 4.
    CDMA_BURST_8            //!< Size 8.
} CDMA_BURST_SIZE;

/** 
 * \brief Destination modify.
 * 
 * This defines the modify method on the destination location.
 **/
typedef enum
{
    CDMA_NO_MOD_DEST,       //!< Do data transfer from source address.
    CDMA_ZERO_DEST,         //!< Fill destination with zero (no data transfer).
    CDMA_FILL_DEST          //!< Fill destination with CDMA_CONFIG.DestFill value (no data transfer).
} CDMA_DEST_MODIFY_CMD;

/** 
 * \brief Address Increment.
 * 
 * This defines the increment method on the source/destination address after a
 * read/write.  Increment value is determined by
 * #CDMA_XFER_WIDTH.
 **/
typedef enum
{
    CDMA_NO_INC,            //!< Do not increment address.
    CDMA_INC                //!< Increment address.
} CDMA_INCREMENT_CMD;


/** 
 * \brief Interrupt time-out base.
 * 
 * This defines the unit time base for the timeout of a non-blocking
 * transfer.  Typically not used.  Refer to \a timeout in
 * CdmaTransfer() for more information.
 **/
typedef enum
{
    CDMA_1_US,              //!< 1 microsecond.         
    CDMA_10_US,             //!< 10 microseconds.
    CDMA_100_US,            //!< 100 microseconds.
    CDMA_1_MS,              //!< 1 millisecond.
    CDMA_10_MS,             //!< 10 milliseconds.
    CDMA_100_MS             //!< 100 milliseconds.
} CDMA_TIMEBASE_CMD;

/** 
 * \brief Channel transfer configuration.
 **/
typedef struct
{
    CDMA_BURST_SIZE         SrcBurst;       //!< Source burst transfer size.  
    CDMA_BURST_SIZE         DestBurst;      //!< Destination burst transfer size. 
    CDMA_XFER_WIDTH         SrcXferWidth;   //!< Source transfer width.
    CDMA_XFER_WIDTH         DestXferWidth;  //!< Destination transfer width.
    CDMA_ENDIAN             SourceEndian;   //!< Source endian.
    CDMA_ENDIAN             DestEndian;     //!< Destination endian. 
    CDMA_FLOWCONTROL        FlowControl;    //!< Flow control.
    uint32_t                SourcePerfID;   //!< Source peripheral ID.
    uint32_t                DestPerfID;     //!< Destination peripheral ID.
    CDMA_INCREMENT_CMD      SrcIncrement;   //!< Source address increment.
    CDMA_INCREMENT_CMD      DestIncrement;  //!< Destination address increment.
    CDMA_DEST_MODIFY_CMD    DestModify;     //!< Destination modify.
    CDMA_TIMEBASE_CMD       TimeBase;       //!< Interrupt time-out base.
    uint32_t                DestFill;       //!< Destination modify fill value.
} CDMA_CONFIG;

/**
 *
 * \brief CDMA handle type
 *
 */
typedef struct cdma_handle_s cdma_handle_t;


/**
 * \brief CDMA callback function type
 *
 * \param user_data User defined data.
 * \param int_status CDMA interrupt status.
 * 
 * This is the function type for the CDMA callback function.
 **/
typedef void (*CDMA_CALLBACK)(void *user_data, uint32_t int_status);

/** 
 * \brief Initialize CDMA driver.
 *
 * This function will initialize the driver. This must be called before using any other driver function.
 **/
void CdmaInit( void );

/** 
 * \brief Get the number of CDMA channels.
 * 
 * \param number_of_channels Return to the address
 * of this location.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will get the number of CDMA channels.
 **/
error_type_t CdmaGetNumberOfChannels(uint32_t *number_of_channels);

/** 
 * \brief Open a CDMA channel.
 * 
 * \param channel The channel to open. If CDMA_FIND_CHANNEL,
 * then return the first available channel.
 * \param handle Return pointer handle of opened channel to the address
 * of this location.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will attempt to open a CDMA channel.  The
 * channel can be either specific or the first available may be
 * requested.  After the channel is opened it will be reserved
 * until CdmaClose() is called.
 **/
error_type_t CdmaOpen(int32_t channel, cdma_handle_t **handle);

 
/** 
 * \brief Close a CDMA channel.
 * 
 * \param handle The handle of the channel to close.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will close a CDMA channel.  After the channel
 * is closed it is free.
 **/
error_type_t CdmaClose(cdma_handle_t *handle);


/** 
 * \brief Set the channel configuration.
 * 
 * \param handle The handle of the channel to configure.
 * \param config The address of the configuration.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will set the configuration for an opened channel.
 * 
 * \warning Do not set the channel configuration while waiting
 * for a CdmaTransfer() call to complete.
 **/
error_type_t CdmaSetConfig(cdma_handle_t *handle, const CDMA_CONFIG *config);

/** 
 * \brief Get the channel configuration
 * 
 * \param handle The handle of the channel.
 * \param config Return configuration to the address of this location.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will get the configuration for an opened channel.
 **/
error_type_t CdmaGetConfig(cdma_handle_t *handle, CDMA_CONFIG *config);

/** 
 * \brief Transfer data.
 * 
 * \param handle Transfer with this handle.
 * 
 * \param src Source data address.  This is either a memory or device address.
 * 
 * \param dest Destination data address.  This is either a
 * memory or device address.
 * 
 * \param length Length of transfer in bytes.
 * 
 * \param timeout If blocking, this is the number of ticks
 * before the function returns.  If non-blocking, this is
 * usually not required and should be set to zero.  If used,
 * it is the time limit not to be exceeded between word
 * transfers. This depends upon the response time of the source
 * and destination devices. It is based upon time units
 * specified in CDMA_CONFIG.TimeBase.
 * 
 * \param callback Callback function for a non-blocking
 * transfer. The callback function receives 2 parameters: user
 * data(void*) and the interrupt status(uint32_t). If NULL,
 * transfer is blocking.
 * \param context User data passed to callback function.
 * 
 * \return Bytes transferred(blocking) or pending(non-blocking):
 *  - If zero, then transfer has aborted due to error.
 *  - If -1 and the transfer was blocking, a timeout occurred.
 *  - If non-zero and the returned value does not equal \a length,
 * then the user must transfer the remaining bytes.  This occurs
 * when the length is not a multiple of the transfer width in
 * bytes and a #CDMA_MEM_TO_DEV or #CDMA_DEV_TO_DEV flow control
 * type is specified.
 * 
 * This function will perform a CDMA transfer. The operation
 * will transfer data of \a length from \a src to \a dest.  The
 * transfer call can either be blocking or non-blocking.  If
 * blocking, then \a timeout is a quantity of ticks and \a
 * callback must be NULL.  If non-blocking, then \a timeout
 * should be set to zero and \a callback is the address of the
 * callback function.  User data may be passed to the callback
 * function.
 * Source and destination address restrictions:
 * If the transfer is #CDMA_MEM_TO_MEM, \a src and \a dest addresses must be 4 byte aligned.
 * If the transfer is #CDMA_MEM_TO_DEV, the \a src address must be 4 byte aligned.
 * If the transfer is #CDMA_DEV_TO_MEM, the \a dest address must be aligned on a dcache boundary.  Also, the destination memory must be "owned" by the caller and be multiple of the dcache line size.  The \a length of transfer can be any size.
 * 
 * \warning When error_type_t has been expanded, the return type
 * will change.
 **/
uint32_t CdmaTransfer(cdma_handle_t *handle,
                      uint8_t *src, uint8_t *dest,
                      uint32_t length,
                      uint32_t timeout,
                      CDMA_CALLBACK callback,
                      void *context);


/** 
 * \brief Halt a non-blocking transfer.
 * 
 * \param handle The handle of the channel.
 * 
 * \return OK if successful, FAIL if not.
 * 
 * This function will halt a non-blocking CDMA transfer.
 * 
 * \warning This will not halt a blocking CdmaTransfer() call.
 **/
error_type_t CdmaTransferHalt(cdma_handle_t *handle);


/**
 * Chained DMA, uses a 4 word decriptor structure.
 * control word contents may vary with asic the
 * rest is fixed in stone.
 **/
typedef struct cdma_link_s
{
    volatile uint8_t *SourceAddress; /// source address
    volatile uint8_t *DestAddress;   /// destination address
    volatile struct cdma_link_s *NextLink; /// next link in chain
    volatile uint32_t  CntrlWord;    /// control register
} cdma_link_t;

/// A chain of word array transfers via dma.
/// 
typedef struct cdma_chain_cmds_s cdma_chain_cmds_t;

/// Construct a chain with a maximum number of transfers and 
/// and a maximum number of words per transfer.
/// supports memory to memory or memory to peripheral ( so far )
/// uses cache coherent or "uncached memory"
cdma_chain_cmds_t * ///< null on memory allocation failure 
cdma_chain_array_allocate_coherent(
    uint32_t max, ///< max * mem_element_words * 4 == src array size in bytes
    uint32_t mem_element_words ///< number of words per write on average
    );

/// Copy an array of words to the dma chain setting up a dma 
/// descriptor per array of words.
/// src_array[0-n] -> dest[0-n] as one dma word transfer.
void cdma_chain_add_cmd_copy(
    cdma_chain_cmds_t *chain, ///< chain to add to
    volatile uint32_t *dest, ///< physical destination to write array to.
    uint32_t *src_array,   ///< array of word to write
    uint32_t num_src_elements ///< number of words to write as 1 transfer.
    );


/// Transfer all the dma decriptors in the chain.
/// Non blocking uses callback to notify of completion from isr context.
/// channel is closed on completion.
error_type_t ///< Fail on bad inputs, or ASSERT in debug.
cdma_chain_transfer(
    cdma_handle_t *channel, ///< get a dma channel to use for transfer.
    cdma_chain_cmds_t *chain, ///< dma chain to transfer
    int32_t dest_pid, ///< -1 for mem - mem else a valid dma channel peripheral id.
    CDMA_CALLBACK callback, ///< isr callback normally just posts a semaphore
    void *context         ///< isr callback data
    );

/// Destructor for those cases where you don't want to keep this forever.
/// ptr = free(ptr);  ASSERT(ptr == 0);
cdma_chain_cmds_t* ///< returns NULL;
cdma_chain_array_free(
    cdma_chain_cmds_t *ptr ///< destruct this pointer to a dma chain 
    );

/** EXAMPLE DISABLED /example cdma_example.c
 * This is an example of a CDMA transfer.
 */

#ifdef __cplusplus
}
#endif
#endif

