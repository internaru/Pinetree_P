/*
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**

  \file acl_api.h

  \brief ACL External prototypes and command defines
 * ACL is a simple control language that provides access to many internal fw 
 functions.  ACL consists of a command block of 16 bytes followed by optional 
 data.  The response consists of a response block of 16 bytes followed by
 optional data.  All acl commands will follow this response format.  Within 
 the command and response blocks are a 2 byte header of 0x00ac, and a command id.

 */


#ifndef INC_ACL_API_H
#define INC_ACL_API_H

#include "ATypes.h"
#include <stdint.h>
#include "agConnectMgr.h"
#include "error_types.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LEN_ACL_CMD     16      ///< The length of the command and response blocks

/**
 * \brief Define a range for acl cmd ids.  
 * This is the start and the cmds go for 100 hex from this base.  Each command
 * should have this define in its define.  So one can search to find the next
 * one to access.
 */
#define ACLCMD_SYSTEM_BASE         0x0000   ///< Used for acl commands for basic system info
#define ACLCMD_KEYBOARD_BASE       0x1200   
#define ACLCMD_MOTOR_BASE          0x1300
#define ACLCMD_RTC_BASE            0x1400
#define ACLCMD_MOTOR_BASE1         0x1410
#define ACLCMD_PRINT               0x1500
#define ACLCMD_SENSORS1            0x2100
#define ACLCMD_CM_BASE             0x3000
#define ACLCMD_DEV_PROTOCOL_BASE   0x3100
#define ACLCMD_SCAN                0x3200
#define ACLCMD_USB                 0xf000

///< acl cmd results
#define ACLCMD_STATUS_FAILURE   0x0000  ///< A failure will return this in the response block
#define ACLCMD_STATUS_SUCCESS   0x0001  ///< A success will return this in the response block


/**
 * \brief Base class defines for acl cmds  
 * This is the cmd block that is received from the host to initiate an acl cmd.
 * This struct is the base class, each acl cmd can use the last 12 bytes in
 * any way they choose.  This usage is defined in each module that implements
 * that acl cmd.
 * The acl commands are always 16 bytes long.  When this is included 4 bytes are 
 * used.  12 more bytes must exist at the end of the structure.
 */

#pragma pack(1)
typedef struct
{
#define ACL_CMD_BASE_CLASS_MEMBERS \
                    uint16_t prefix;   /** The acl prefix 0xAC */\
                    uint16_t cmd_id      /** This is the acl cmd to execute.*/

    ACL_CMD_BASE_CLASS_MEMBERS;
    uint8_t temp[12];                      ///< has to be expanded to 16 bytes total, this does that
} acl_cmd_base_struct_t;
/**
 * \brief Base class defines for acl responses
 * The acl commands are always 16 bytes long.  When this is included 6 bytes 
 * are used.  This must be extended by 10 bytes to the full length.
 */

typedef struct 
{
#define ACL_RESPONSE_BASE_CLASS_MEMBERS \
                    uint16_t prefix;   /** The acl prefix --- 0xac */ \
                    uint16_t cmd_id;    /** This is the acl cmd we are responding to*/ \
                    uint16_t cmd_status  /** This is the status of the command.*/

    ACL_RESPONSE_BASE_CLASS_MEMBERS;        ///< define the base items
    uint8_t temp[10];                  ///< has to be expanded to 16 bytes total, this does that.
} acl_response_base_struct_t;
#pragma pack()


/**
 * \brief Define the form of an acl command function calling sequence.
 * When an acl command is registered you pass in the command value, and the function
 * that is used to execute the acl command.  This typedef defines the calling
 * sequence for that function.
 * \param[in] pipe, The io pipe on which to respond
 * \param[in] acl_cmd Pointer to the received acl command block
 * \param[in] cmd_data This is a pointer to the data that was passed into the acl parser 
 *              during registration.
 * \return int32_t
 * \retval 0 for success 
 * \retval != 0 for failure.
*/
typedef  error_type_t (*acl_function_t)(IOReg *pipe, acl_cmd_base_struct_t *acl_cmd, void *cmd_data);
/**
 * \brief Register an acl command with the acl system.
 * This function takes and makes a one-to-one mapping between the acl_cmd_value
 * and the acl_callback function.  When an acl command is received of the acl_cmd_value
 * the acl_callback is called to execute the function.  If an acl_cmd_value
 * already exists this function will assert.
 * \param[in] acl_cmd_value The acl command function.  Fits into categories listed above
 * \param[in] acl_callback The function to be called when the acl command is received.  
 *                          This does the cmd.
 * \param[in] cmd_data Whatever is here will be passed to the acl function when initiated.
 * \returns nothing.
 */
void register_acl_cmd(uint32_t acl_cmd_value, acl_function_t acl_callback , void *cmd_data);
/**
 * \brief A helper function to construct the response buffer and do some init.
 * The response buffer is 16 bytes and consists of at least a status, id, and command.
 * This function allocates the buffer, initalizes the status to success, initializes
 * the id to 0xac, and sets the command according to acl_cmd.  Then it returns
 * the buffer.  Note that if no memory is available this will suspend till it
 * is able to allocate the buffer.
 * \param[in] acl_cmd The acl_cmd to put into the response structure.
 * \returns acl_response_base_struct_t pointer to the initialized structure
 */
acl_response_base_struct_t *construct_acl_response_buffer(uint16_t acl_cmd);

#ifdef __cplusplus
}
#endif

#endif

