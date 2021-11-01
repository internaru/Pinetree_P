/*
 * $Id: dc_return_codes.h,v 1.4 2010/03/04 17:26:59 boog Exp $
 *
 * Copyright (C) 2009 Conexant Systems Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _dc_return_codes_h_
# define _dc_return_codes_h_
/**************************************************************************//**
*  @file dc_return_code.h
*  @brief This file contains a common list of return codes returned from
*  core functions. 
*
*  To insure backwards compatibility, return codes should not be changed.
*  Return codes are always added at the end of this list. Please do not
*  remove return codes. Some firmware is delivered in binary-only form so
*  the return codes are compiled in. If they values are changed or removed,
*  issues could be introduced. If any return code definitions are changed
*  then the meaning of the return codes might be ambiguous.
*
*  Error codes greater than or equal to 0x40000000 are reserved for customer
*  use.
*
*  Note: Special consideration is made for a successful return value. Both
*  DC_SUCCESS and OSPL_SUCCESS both indicate success and both are zero. The
*  name DC_SUCCESS is typically used in the SDK API, while OSPL_SUCCESS' is
*  used in the ThreadX API.
*
*  Note: This file is automatically processed to create the "dc_errstr_get"
*  function.  All defines should be in the format shown:
*  # define <symbol name> <value> <optional comments>
*  Other formats could cause processing errors.
*
******************************************************************************/
/******************************************************************************
*  FILE FORMAT: <PUBLIC>
*      1. INCLUDES
*      2. ENUMERATIONS
*      3. DEFINES
*      4. STRUCTURES
*      5. GLOBALS
*      6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/
/******************************************************************************
*  1. INCLUDES
******************************************************************************/
/******************************************************************************
*  2. ENUMERATIONS
******************************************************************************/
/******************************************************************************
*  3. DEFINES
******************************************************************************/
/** Not an error */
# define DC_SUCCESS  0

/**
** General error
**
** Please do not use this error and add a more descriptive define whenever
** possible.
*/
# define DC_INTERNAL_ERROR 0x100

/**
** General error
**
** API called has run out of memory.
*/
# define DC_OUT_OF_MEMORY 0x101

/**
** General error
**
** An invalid argument was passed to a function.
*/
# define DC_BAD_ARGUMENT 0x102

/**
** General error
**
** Please do not use this error and add a more descriptive define whenever
** possible.
*/
# define DC_FAILURE 0x103

/**
** General error
**
** Please do not use this error and add a more descriptive define whenever
** possible.
*/
# define DC_UNKNOWN 0x104

/**
** General error
**
** A firmware call was made without required intialization.
*/
# define DC_NOT_INITIALIZED 0x105

/**
** General error
**
** A firmware call was made out of order.
*/
# define DC_INVALID_EXECUTION_ORDER 0x106

/**
** General error
**
** This return indicates the system is in an invalid state to perform the requested
** activity
*/
# define DC_INVALID_STATE 0x107

/**
** General error
**
** An attempt was made to initialize something twice, which is not allowed.
*/
# define DC_ALREADY_INITIALIZED 0x108

/**
** MI Channels
**
** MI channel within activeMask already registered as a system MI channel
*/
# define DC_MI_ADD_ERROR_SYSTEM  0x109

/**
** MI Channels
**
** MI channel within activeMask already registered by calling thread.
*/
# define DC_MI_ADD_ERROR_REGISTERED 0x10A

/**
** MI Channels
**
** The requested MI channels had not been registered.
*/
# define DC_MI_DEL_ERROR_UNREGISTERED 0x10B

/**
** Context Manager
**
** Bad context received.
*/
# define DC_CM_BAD_CONTEXT       0x10C

/**
** Context Manager
**
** Bad work element received.
*/
# define DC_CM_BAD_WORK_ELEMENT  0x10D

/**
** Context Manager
**
** No data callback function received.
*/
# define DC_CM_NO_DATA_CALLBACK  0x10E

/**
** Context Manager
**
** Invalid priority received.
*/
# define DC_CM_PRIORITY_ERROR    0x10F

/**
** DMB Manager.
**
** There are still DMBs allocated in the system.
*/
# define DC_DMB_INIT_PAGES_AVAILABLE 0x110


/**
** DMB Manager.
**
** Memory size was invalid, such as Zero
*/
# define DC_DMB_INIT_BAD_MEM_SIZE 0x111


/**
** DMB Manager.
**
** Memory base address was not aligned to a page size boundary
*/
# define DC_DMB_INIT_BAD_MEM_BASE 0x112


/**
** DMB Manager.
**
** DMB size was not a power-of-2 multiple of 1Kbyte
*/
# define DC_DMB_INIT_BAD_PAGE_SIZE 0x113


/**
** Tick Handler
**
** The maximum number of tick functions has been execeeded.
*/
# define DC_TOO_MANY_TICK_FUNCTIONS 0x114

/**
** Time
**
** The input microseconds was converted to system ticks with some rounding
** error.
*/
# define DC_INEXACT_CONVERSION 0x115

/**
** Cache Management
**
** An error was detected from the CPU during the cache operation.
*/
# define DC_CACHE_CPU_FAILURE 0x116

/**
** MI Channels
**
** An invalid MI Channel ID was received.
*/
# define DC_MI_BAD_IDENTIFIER 0x117

/**
** MI Channels
**
** The MI Channel requested was not registered.
*/
# define DC_MI_NOT_REGISTERED 0x118

/**
** DMB Manager.
**
** The filter functions were invalid
*/
# define DC_DMB_BAD_FILTER 0x119

/**
** DMB Manager.
**
** A bad dynamic memory block identifier was received
*/
# define DC_DMB_BAD_PAGE 0x11A

/** General Error.
**
** The function cannot be called from the current operating mode
** (Initialization, Thread, IRQ, FIRQ, Timer, or Tick Handler)
*/
# define DC_INVALID_OPERATING_MODE 0x11B


/**
** General Error.
**
** The function cannot be called from the current operating mode
** (Initialization, Thread, IRQ, FIRQ, Timer, or Tick Handler)
*/
# define DC_NULL_POINTER 0x11C

/**
** USB Host Controller Error.
**
** A command was sent to a device on the USB bus that was unsupported
** by the device. This is can only be returned by
** dc_usb_hcd_root_hub_control().
*/
# define DC_USB_COMMAND_STALLED 0x11D

/**
** USB Host Controller Error.
**
** The endpoint type specified is not supported by the USB Host
** Controller Driver. This can only be returned by
** dc_usb_hcd_urb_enqueue().
*/
# define DC_USB_UNSUPPORTED_EP_TYPE 0x11E

/**
** USB Host Controller Error.
**
** The URB (transfer control structure) pointer given does not point to
** an URB that the USB Host Controller Driver knows about. This can
** only be returned by dc_usb_hcd_urb_dequeue().
*/
# define DC_USB_URB_NOT_FOUND 0x11F

/**
** USB Device Driver
**
** The USB Device Driver could not initiate the transfer requested
** because the selected endpoint is still busy. This could also return
** if an Endpoint zero transfer is requested and a setup packet is
** received that requires firmware interaction.
*/
# define DC_USB_DEV_ENDPOINT_BUSY 0x120

/**
** USB Device Driver
**
** The USB Device Driver could not initiate the transfer requested
** because the selected endpoint is not enabled.
*/
# define DC_USB_DEV_ENDPOINT_DISABLED 0x121

/**
** USB Device Driver
**
** There is not active transfer on the selected endpoint.
*/
# define DC_USB_DEV_TRANSFER_NOT_ACTIVE 0x122

/**
** USB Device Driver
**
** There is not enough space in the buffer for requested amount.
*/
# define DC_USB_DEV_NO_SPACE 0x123

/**
** General Error
**
** A version conflict was detected.
*/
# define DC_BAD_VERSION 0x124

/**
** General Error
**
** This error indicates an address passed to the function is invalid.
** Most often this is because the address is not properly aligned (e.g.
** 32 byte aligned).
*/
# define DC_BAD_ADDRESS 0x125

/**
** ROM Resource Manager
**
** This error indicates the resource name given could not be found
** by the ROM Resource Manager.
*/
# define DC_RESOURCE_NOT_FOUND 0x126

/**
** USB Host Controller Error.
**
** The USB pipe was not found on the list of open pipes.
*/
# define DC_USB_PIPE_NOT_OPEN 0x127

/**
** USB Host Controller Error.
**
** The USB pipe has already been opened.
*/
# define DC_USB_PIPE_ALREADY_OPEN 0x128

/**
** Serial Flash Driver
**
** The ROM address is out of range
*/
# define DC_ROM_ADR_RANGE 0x129

/**
** General Error
**
** The function called has timed out
*/
# define DC_ERROR_TIMEOUT 0x12A

/**
** USB Device Driver
**
** A USB event has been detected causing the function to exit
*/
# define DC_USB_DEV_EVENT_DETECTED 0x12B

/**
** General Error
**
** An unlock call was made on an object that was not properly
** locked.  This may be returned by the NVRAM unlock call.
*/
# define DC_ERROR_NOT_LOCKED 0x131

/* Start of Laser Engine Print Reference Application error codes */
# define DC_DMBIMG_CONTINUE 0x132
# define DC_DMBIMG_NO_DATA 0x133
# define DC_DMBIMG_NEXT_PLANE 0x134
# define DC_BUFFER_FULL 0x135
# define DC_LEC_INVALID_LOCKID 0x136
# define DC_LEC_LOCK_FAIL 0x137
# define DC_INVALID_ENGINE_CMD 0x138
# define DC_LEC_INVALID_CONTROL_SIGNAL 0x139
# define DC_LEC_RESERVED  0x13A
# define DC_BAD_PO_CONFIG 0x13B
/* End of Laser Engine Print Reference Application error codes */

/**
** General Error
**
** A name required to be supplied and unique was not.
*/
# define DC_BAD_NAME 0x13c

/**
** NVRAM
**
** A thread waiting for an nvram flash write to finish is not responding after
** 30 update cycles.
*/
# define DC_NVRAM_WAITER_NOT_RESPONDING 0x13d

/**
** General Error
**
** A numerical argument was too large or too small.
*/
# define DC_OUT_OF_RANGE 0x13e

/**
** General Error
**
** An operation was rendered invalid by some asynchronous processing
** and must be retried.  Currently used only internally.
*/
# define DC_OPERATION_INTERRUPTED 0x13f

/**
** Debug Logging
**
** The log group requested already exists.
** This is not usually an error.
*/
# define DC_LOG_GROUP_EXISTS 0x140

/**
** Debug Logging
**
** A requested log group does not exist.
*/
# define DC_LOG_GROUP_DOES_NOT_EXIST 0x141

/**
** Debug Logging
**
**
** A debug level log message was not printed because it
** was not enabled.
*/
# define DC_LOG_SUPRESSED    0x142

/**
** Arbiter
**
** The Arbiter device is locked
*/
# define DC_ARB_LOCKED 0x14F

/**
** Arbiter
**
** The Arbiter device is write protected
*/
# define DC_ARB_WRITE_PROTECT_ON 0x150

/**
** Arbiter
**
** The Arbiter device is not write protected
*/
# define DC_ARB_WRITE_PROTECT_OFF 0x151

/**
** USB Host Controller Error.
**
** The USB transfer received less than requested
*/
# define DC_TRANSFER_RECEIVED_SHORT  0x152

/**
** General Error 
**
**  The operation requested is not supported
*/
# define DC_OPERATION_UNSUPPORTED 0x161

/**
** General Error
**
** The operation requested is unknown
*/
# define DC_OPERATION_UNRECOGNIZED 0x162


/**
** Data container.
** 
** Attempt to register producer after production has begun.
**
*/
# define DC_CONTAINER_PRODUCTION_ALREADY_BEGUN   0x163

/**
** Data container.
** 
** Attempt to register consumer after consumption has begun.
**
*/
# define DC_CONTAINER_CONSUMPTION_ALREADY_BEGUN   0x164

/**
** Data container.
** 
** Attempt to produce data is too large for space available.
**
*/
# define DC_CONTAINER_PRODUCE_TOO_LARGE 0x165

/**
** Data container.
** 
** Attempt to consume data is too large for data available.
**
*/
# define DC_CONTAINER_CONSUME_TOO_LARGE 0x166

/**
** Data container.
** 
** Fail because data container is still active.
**
*/
# define DC_CONTAINER_STILL_ACTIVE 0x167

/**
** General DMB usage.
** 
** Cannot allocate because already at maximum specified number of DMBs.
**
*/
# define DC_AT_MAX_DMB_PAGES 0x168

/**
** General.
** 
** Fail because specified element is not registered.
**
*/
# define DC_NOT_REGISTERED 0x169

/**
** DBGEXT (dbgmon extension).
** 
** Firmware does not support dbgmon extensions.
**
*/
#  define DC_DBGEXT_UNSUPPORTED 0x16a

/**
** DBGEXT (dbgmon extension).
** 
** Specified extension is unknown.
**
*/
#  define DC_DBGEXT_UNKNOWN_EXTENSION 0x16b

/**
** DBGEXT (dbgmon extension).
** 
** Not enough memory to execute the command extension.
**
*/
#  define DC_DBGEXT_OUT_OF_MEMORY 0x16c

/**
** DBGEXT (dbgmon extension).
** 
** Generally malformed command.
**
*/
#  define DC_DBGEXT_BAD_COMMAND 0x16d

/**
** DBGEXT (dbgmon extension).
** 
** Command specifies unexpected number of arguments.
**
*/
#  define DC_DBGEXT_BAD_NUMBER_OF_ARGUMENTS 0x16e

/**
** DBGEXT (dbgmon extension).
** 
** Version specified in command does not match.
**
*/
#  define DC_DBGEXT_VERSION_MISMATCH 0x16f

/**
** DBGEXT (dbgmon extension).
** 
** Function index specified in command is unknown.
**
*/
#  define DC_DBGEXT_UNKNOWN_FUNCTION 0x170

/**
** DBGMON general.
** 
** DBGMON communication failure.
**
*/
#  define DC_DBGMON_COMMUNICATION_FAIL 0x171

/**
** Data container.
** 
** Fail because incorrect container class or type.
**
*/
# define DC_CONTAINER_BAD_CLASS_OR_TYPE 0x172

/** usb class driver counter error
 **
 **
 */

#define DC_USB_CLASS_COUNTER_DUPLICATE_INIT        0x173
#define DC_USB_CLASS_COUNTER_REGISTER_ID_DUPLICATE 0x174
#define DC_USB_CLASS_COUNTER_REGISTER_OUT_OF_SPACE 0x175
#define DC_USB_CLASS_COUNTER_ID_NOT_REGISTER       0x176
#define DC_USB_CLASS_COUNTER_NO_REGISTER_FUNC      0x177


/**
** Data container.
** 
** Operation requires monitoring to be enabled.
**
*/
# define DC_CONTAINER_MONITOR_NOT_ENABLED 0x178

/**
** Data container.
** 
** Attempt to monitor data is too large for data available.
**
*/
# define DC_CONTAINER_MONITOR_TOO_LARGE 0x179

/** Network Error
**
** WiFi device not connected
*/
# define DC_NET_DEV_NOTCONNECTED 0x17a

/**
** File Upgrade API
** 
** File upgrade API in operation
**
*/
# define DC_FILE_UPGRADE_ACTIVE 0x17b

/**
** General Error
**
** A command was carried out successfully but the result was that it failed.
*/
# define DC_COMMAND_STATUS_FAILED 0x17c

/**
** Mass Storage or Flash Card
**
** A command was carried out successfully but the result was that it failed.
*/
# define DC_MEDIA_NOT_PRESENT 0x17d

/**
** Data container.
*/
# define DC_CONTAINER_NOT_CREATED 0x17e
# define DC_CONTAINER_PRODUCER_CONSUMER_NOT_INITIALIZED 0x17f
# define DC_CONTAINER_PRODUCER_CONSUMER_NOT_REGISTRED   0x180
# define DC_CONTAINER_PRODUCER_CONSUMER_ALREADY_REGISTERED 0x181

# define DC_CONTAINER_HOLDBACK_UNITS_ALREADY_SPECIFIED 0x182

/**
** LAS API Errors
*/
/**
** LAS API called has run out of ADDRESS SPACE.
*/
# define DC_OUT_OF_ADDRESS_SPACE 0x183
# define DC_LAS_BAD_MARK_CHAIN   0x184

/*
** Data container.
*/
# define DC_CONTAINER_NOT_DELETED  0x190

/* JPEG PIPE */ 
# define DC_PIPE_ALREADY_CREATED 0x191
# define DC_PIPE_NOT_CREATED     0x192
# define DC_PIPE_ALREADY_IDLE    0x193
# define DC_PIPE_NOT_IDLE        0x194


/***********************************************************
**  Media Store Interface (MSI) related errors
***********************************************************/
/**
**  MSI initialization failure.
*/
#define DC_MSI_INIT_FAILED                              0x195

/**
**  Parsing function for the given id does not exist or have
**  not been registered yet.
*/
#define DC_MSI_CUST_OP_INVALID_ID                       0x196

/**
**  Customized parsing function failed to complete parsing.
*/
#define DC_MSI_CUST_OP_PARSE_EOF                        0x197

/**
**  File handle size exceeds the memory provided, not enough
**  to put the whole handle.
*/
#define DC_MSI_FILE_HANDLE_NEED_MORE_MEM                0x198

/**
**  Invalid request operation ID.
*/
#define DC_MSI_INVALID_REQ_OP_ID                        0x199

/**
**  Attempt to unlock an object that's not locked.
*/
#define DC_MSI_OBJECT_NOT_LOCKED                        0x19a

/**
**  Either the object is locked by someone else or the client
**  already has the lock for the object.
*/
#define DC_MSI_ACCESS_DENIED                            0x19b

/**
**  General read error.
*/
#define DC_MSI_READ_ERROR                               0x19c

/**
**  General write error.
*/
#define DC_MSI_WRITE_ERROR                              0x19d

/**
**  An operation request that's not supported by the SAL.
**  E.g. trying to delete a file from a SAL that does not
**  support write functionalities.
*/
#define DC_MSI_OP_NOT_SUPPORTED_BY_SAL                  0x19e

/**
**  Failed to find the given client.
*/
#define DC_MSI_EVENT_NO_SUCH_CLIENT                     0x19f

/**
**  Invalid object id/handle.
*/
#define DC_MSI_INVALID_SAL_ID                           0x1a0
#define DC_MSI_INVALID_STORE_ID                         0x1a1
#define DC_MSI_INVALID_FILE_HANDLE                      0x1a2

/**
**  The message queue for the sal-process is empty.
*/
#define DC_MSI_SAL_REQ_Q_EMPTY                          0x1a3

/**
**  The message queue for the sal-process is full, no more
**  requests can be placed until some of the current reqs 
**  are done processing.
*/
#define DC_MSI_SAL_REQ_Q_FULL                           0x1a4

/**
**  The max number concurrent requests can be processed by 
**  the sal-process has been reached, therefore unable to 
**  process the request at this time.
*/
#define DC_MSI_SAL_MAX_CONCURRENT_REQ_REACHED           0x1a5

/**
**  The client is not registered with MSI.
*/
#define DC_MSI_INVALID_CLIENT                           0x1a6

/**
**  Error returned upon duplicate registration.
*/
#define DC_MSI_CLIENT_ALREADY_REGISTERED                0x1a7

/**
**  SALs have general communication error with their device.
*/
#define DC_MSI_COMMUNICATION_ERROR                      0x1a8

/**
**  Failed to find a matching entry.
*/
#define DC_MSI_HANDLE_NO_MORE_MATCHES                   0x1a9


// CM
# define DC_CM_ALREADY_INITIALIZED                      0x1aa
# define DC_CM_NOT_INITIALIZED                          0x1ab

# define DC_CM_CONTEXT_ALREADY_CREATED                  0x1ac
# define DC_CM_CONTEXT_NOT_CREATED                      0x1ad

/**
** CDB error codes
*/

#define DC_CDB_NODE_LOCKED                              0x1b0
#define DC_CDB_TYPE_MISMATCH                            0x1b1
#define DC_CDB_PLACEHOLDER_2                            0x1b2
#define DC_CDB_PLACEHOLDER_3                            0x1b3
#define DC_CDB_PLACEHOLDER_4                            0x1b4
#define DC_CDB_PLACEHOLDER_5                            0x1b5
#define DC_CDB_PLACEHOLDER_6                            0x1b6
#define DC_CDB_PLACEHOLDER_7                            0x1b7
#define DC_CDB_PLACEHOLDER_8                            0x1b8
#define DC_CDB_PLACEHOLDER_9                            0x1b9
#define DC_CDB_PLACEHOLDER_a                            0x1ba
#define DC_CDB_PLACEHOLDER_b                            0x1bb
#define DC_CDB_PLACEHOLDER_c                            0x1bc
#define DC_CDB_PLACEHOLDER_d                            0x1bd
#define DC_CDB_PLACEHOLDER_e                            0x1be
#define DC_CDB_PLACEHOLDER_f                            0x1bf

/**
 * MDK MENU system return codes
 */

#define DC_MENU_EFFECT_HIGHLIGHTED                      0x1c0
#define DC_MENU_EFFECT_UNHIGHLIGHTED                    0x1c1
#define DC_MENU_EFFECT_SELECTED                         0x1c2
#define DC_MENU_EFFECT_ABANDONED                        0x1c3
#define DC_MENU_EFFECT_1c4                              0x1c4
#define DC_MENU_EFFECT_1c5                              0x1c5
#define DC_MENU_EFFECT_1c6                              0x1c6
#define DC_MENU_EFFECT_1c7                              0x1c7
#define DC_MENU_EFFECT_1c8                              0x1c8
#define DC_MENU_EFFECT_1c9                              0x1c9
#define DC_MENU_EFFECT_1ca                              0x1ca
#define DC_MENU_EFFECT_1cb                              0x1cb
#define DC_MENU_EFFECT_1cc                              0x1cc
#define DC_MENU_EFFECT_1cd                              0x1cd
#define DC_MENU_EFFECT_1ce                              0x1ce
#define DC_MENU_EFFECT_1cf                              0x1cf

/******************************************************************************
*  4. STRUCTURES
******************************************************************************/
typedef int DC_ERROR_CODE;
typedef int DC_RETURN_CODE;
/******************************************************************************
*  5. GLOBALS
******************************************************************************/
/******************************************************************************
*  6. PUBLIC FUNCTION PROTOTYPES
******************************************************************************/

/*
** Returns a string which matches the code.
** Note: Using this function will add several K to the load module.
*/
char *dc_errstr_get(int return_code);

#endif /*  _dc_return_codes_h_ */
