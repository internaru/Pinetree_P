/**
 * ============================================================================
 * Copyright (c) 2006-2008   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *
 * Description:  Implements Support for ACL language.
 *
 *****************************************************************************/

#include <string.h>

#include "lassert.h"
#include "posix_ostools.h"
#include "dprintf.h"
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "logger.h"
#include "sys_init_api.h"
#include "list.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "debug.h"
#include "ResourceMap.h"
#include "acl_api.h"
#include "acl_register_cmds.h"
#include "ioutils.h"




static ATLISTENTRY acl_cmd_list;    ///< The acl commands that are registered.

// globals


const char UEL[] = "\x1b%-12345X";
const UINT16 ACL_PREFIX = 0xAC;

SINT32 agiACLInput( IOReg *, MODULE_ID );
void   agiACLParserInit( void );
void * agiACLGetDownloadAddr(unsigned int size);
void   agiACLHandleDownload(unsigned int codeLoc);

#define NUM_ACL_MESSAGES 10
#define ACL_STACK_SIZE POSIX_MIN_STACK_SIZE
static pthread_t acl_thd_id;
static mqd_t acl_queue;


// subtract 1 in lengths to account for trailing NULL
#define LEN_UEL         (sizeof(UEL)-1)
#define LEN_ACL_PREFIX  (sizeof(ACL_PREFIX))


typedef struct
{
    ATLISTENTRY node;
    uint32_t  ACLCmd;         ///< The acl command this function processes.
    acl_function_t  ACLCmdProcess;
    void *cmd_data;             ///< the cmd_data pointer.
} ACLCMDSTR;
//
// The align 8 below is because some of the function we could call use UINT64's.
//
UINT8 ACLStack[ACL_STACK_SIZE] __attribute__ ((aligned (8)));

char   ACLParseString[]="ACL";



/* FUNCTION NAME: register_acl_cmd */

/**
 * \brief Register an acl command with the parser.
 *
 * \param ACLCmd The command value.
 *
 * \param AclProcess The fuction to call when this command is invoked.
 *
 * \param Reserved
 *
 * \retval None
 *
 * \author Dirk Bodily
 *
 * \date 4/20/2007
 *
 **/
void register_acl_cmd(uint32_t ACLCmd, acl_function_t AclProcess, void *cmd_data)
{
    ATLISTENTRY* node;
    ACLCMDSTR   *TempAcl;
    //
    // see if this cmd already is registered.
    //
    node = ATListHead(&acl_cmd_list);
    while(node != NULL)
    {
        TempAcl = CONTAINING_RECORD(node, ACLCMDSTR, node);
        if(TempAcl->ACLCmd == ACLCmd)
        {
            ASSERT(0 == "ACL Cmd already registered");
        }
        node = ATListNext(&acl_cmd_list, node);
    }

    //
    // Add a new acl command to the command list.
    //
    TempAcl = MEM_MALLOC(sizeof(ACLCMDSTR));
    TempAcl->ACLCmd = ACLCmd;
    TempAcl->ACLCmdProcess = AclProcess;
    TempAcl->cmd_data = cmd_data;
    ATInsertTailList(&acl_cmd_list, &TempAcl->node);
    DPRINTF(DBG_SOFT|DBG_OUTPUT,("ACL: Added command %04X\n",ACLCmd));
}


/* FUNCTION NAME: ACLParserEntry */

/**
 * \brief Main parse look for acl.
 *
 * \retval None
 *
 * \author Dirk Bodily
 *
 * \date 4/20/2007
 *
 **/
void *ACLParserEntry(void *unused)
{
    MESSAGE     nextMsg;
    IOReg *Pipe = NULL;
    CURRENT_RESOURCE  *OurResource = 0;
    int err;

    SysWaitForInit ();
    //
    DPRINTF((DBG_SOFT | DBG_OUTPUT),("ZJP: start parse\n"));


    while (1)
    {
        err = mq_receive( acl_queue, (char*)&nextMsg, sizeof(MESSAGE), 0); 
        XASSERT( err == sizeof(MESSAGE), thread_errno);
	UNUSED_VAR(err);

        switch (nextMsg.msgType)
        {
            case MSG_START_PARSE:
                {
                    STARTRECIPE *StartIt;

                    StartIt = (STARTRECIPE *)MEM_MALLOC(sizeof(STARTRECIPE));
                    ASSERT(StartIt != NULL);
                    // we have a connection, get the resources and start.
                    //
                    Pipe = (IOReg *) nextMsg.param3;
                    nextMsg.msgType = MSG_STARTRECIPE;
                    StartIt->Job = ejob_ACL;
                    StartIt->AdditionalResource = e_ACLParser;
                    StartIt->Pipe = NULL;              // no status reported back by this method.
                    StartIt->Wait = e_WaitForever;  // don't need sending module if waitforever
                    nextMsg.param3 = StartIt;           // will be freed later

                    SYMsgSend(SJMID, &nextMsg);         // send the message.

                    break;
                }
            case MSG_RESOURCES:
                {

                    OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    break;
                }
            case MSG_ACKRECIPE:
                {
                    SINT32 Response;
                    // We have the resources, start the job.
                    //
                   
                    // This ASSERT verifies we have the resources
                    ASSERT( NULL != OurResource );
                    ASSERT( NULL != Pipe );
                    Response = agiACLInput (Pipe, OurResource->Destination);
                    //
                    // finished parsing, free the resources.
                    nextMsg.msgType = MSG_FREERECIPE;
                    SYMsgSend (SJMID, &nextMsg); // param3 is untouched and now sent back.
                    nextMsg.param1 = Response;         // Parse Status.
                    nextMsg.msgType = MSG_END_PARSE;
                    nextMsg.param3 = Pipe;
                    SYMsgSend (CMPARSER, &nextMsg);      // send the response.
                    break;      // done
                }
            default:
                break;
        }
    }
    return 0;
}


/* FUNCTION NAME: InitACLResponse */

/**
 * \brief construct a response buffer
 * This allocates a response buffer.  The caller must free this buffer.  It then
 * clears it and initializes the header, status, and the cmdid.
 *
 * \param[out] Buffer pointer to the pointer for the buffer
 * \param[in] aclCmd The cmd id for this command.
 *
 * \retval None
 *
 **/
acl_response_base_struct_t *construct_acl_response_buffer(uint16_t aclCmd)
{
    acl_response_base_struct_t *Buffer;
    while((Buffer = (acl_response_base_struct_t *)MEM_MALLOC(sizeof(acl_response_base_struct_t))) == NULL)
          sleep(1);
    
    memset(Buffer, 0, sizeof(acl_response_base_struct_t));
    Buffer->prefix = cpu_to_be16(ACL_PREFIX);
    Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_SUCCESS);
    Buffer->cmd_id = cpu_to_be16(aclCmd);
    return Buffer;

}


/* FUNCTION NAME: agiACLInput */

/**
 * \brief This is the command processor for acl.
 *
 * \param Pipe
 *
 * \param Destination
 *
 * \retval SINT32
 *
 * \author Dirk Bodily
 *
 * \date 4/20/2007
 *
 **/
SINT32 agiACLInput( IOReg *Pipe, MODULE_ID Destination )
{
    int bytesRead;
    acl_cmd_base_struct_t g_aclCmd;

    ASSERT( NULL != Pipe );

    while (1)
    {
        // At this point the things that could legally be in the input stream are:
        //   - a 16 byte ACL command (all ACL commands begin with a known 2 byte prefix)
        //   - a UEL that ends the prior ACL command

        // The only way to distinquish these cases is to start reading from the
        // input pipe.  But we must be careful to not insist upon reading more
        // bytes than may be available, and we must consume the UEL that
        // terminates the block of ACL commands before we return to our caller.

        // The PARSEREAD_SOME option to ReadConnect() blocks until at least
        // 1 byte is available.  However we also specify that no more than
        // LEN_ACL_CMD bytes should be returned.

        bytesRead = Pipe->ReadConnect(Pipe, &g_aclCmd, LEN_ACL_CMD, PARSEREAD_SOME);

        if ( ( bytesRead >= LEN_UEL ) &&
             ( strncmp( (char*)&g_aclCmd, UEL, LEN_UEL ) == 0 ))
        {
            // We have read a UEL (possibly followed by other bytes).
            // This signals that it's time to exit from the ACL Parser.

            if ( bytesRead > LEN_UEL )
            {
                // We have read a UEL plus other bytes.  We want to return whatever
                // follows the UEL to the top-level so the next parser can get these bytes.


                Pipe->RewindData(Pipe, (char*)&g_aclCmd + LEN_UEL, bytesRead - LEN_UEL);
            }
            return PARSER_SUCCESS;
        }
        else if ( bytesRead < 0)
        {
            // there was an error reading, just return and finish the parse.
            // there is nothing to rewind so don't even try it.
            //
            return PARSER_SUCCESS;
        }
        else if ( bytesRead == 0 )
        {
            // there was no date in the pipe -- perhaps we have finished
            // parsing a command with no UEL termination

            return PARSER_SUCCESS;
        }
        else if ( bytesRead < LEN_ACL_CMD )
        {
            // We also want to exit from the ACL Parser if we receive illegal input.
            // In this case we want to rewind everything we have read back into the
            // channel so it is available to the next parser.


            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Less than 16 bytes received\r\n"));
            Pipe->RewindData(Pipe, (char*)&g_aclCmd, bytesRead);

            return PARSER_ERROR;
        }
        else if ( be16_to_cpu(g_aclCmd.prefix) != ACL_PREFIX )
        {
            // We also want to exit from the ACL Parser if we receive illegal input.
            // In this case we want to rewind everything we have read back into the
            // channel so it is available to the next parser.


            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: ACL_Prefix not found\r\n"));
            Pipe->RewindData(Pipe, (char*)&g_aclCmd, bytesRead);

            return PARSER_ERROR;
        }
        else
        {
            DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Received CMD=%04X\n", be16_to_cpu(g_aclCmd.cmd_id)));
        }
        //
        // Look for the command in the table and execute it if possible.
        // This checks every registered acl command and if it finds one
        // calls the function tied to that command.
        //
        {
            ATLISTENTRY* node;
            ACLCMDSTR *TempAcl;
            //
            // Try and find the command in the table.
            //
            node = ATListHead(&acl_cmd_list);
            while(node != NULL)
            {
                TempAcl = CONTAINING_RECORD(node, ACLCMDSTR, node);
                if(TempAcl->ACLCmd == be16_to_cpu(g_aclCmd.cmd_id))
                {
                    // We found the command.  Process it now.
                    //
                    TempAcl->ACLCmdProcess(Pipe, &g_aclCmd, TempAcl->cmd_data);
                    break;  // done.

                }
                node = ATListNext(&acl_cmd_list, node);
            }
            if(node == NULL)
            {
                //
                // We looked through our registered commands and we do not have one
                // for this code.  Return a failure to the host.  We then return
                // a parser error.  This will force the connection manager to
                // read data till it finds something it recognizes.  So if there
                // is data for this command the connection manager will eat it.
                //
                acl_response_base_struct_t *Buffer;

                // Send a failure response

                Buffer = construct_acl_response_buffer(be16_to_cpu(g_aclCmd.cmd_id));

                // set up a failure for this command.
                Buffer->cmd_status = cpu_to_be16(ACLCMD_STATUS_FAILURE);

                Pipe->WriteConnect(Pipe, Buffer, LEN_ACL_CMD);
                DPRINTF((DBG_SOFT | DBG_OUTPUT),("ACL: Received unhandled CMD=%d\n", g_aclCmd.cmd_id));
                return PARSER_ERROR;
            }
        }
    }
    return PARSER_SUCCESS;
}

/* FUNCTION NAME: agiACLParserInit */

/**
 * \brief Initialize the acl parser
 * This inits the command structure, registers the parser with the connection
 * manager, registers with the status manager and registers with the resource
 * manager.  Registers the common acl commands, and starts the tasks.
 *
 * \retval None
 *
 * \author Dirk Bodily
 *
 * \date 2/27/2006
 *
 **/
void agiACLParserInit( void )
{
    PARSE_SETUP Parser;

    ATInitList(&acl_cmd_list);

    Parser.ParserID = ACLPARSER;
    Parser.ParseCmpString = NULL;
    Parser.StringSize = 1;             // DON'T SET to 0 or to strlen(Parser.ParseCmpString) !!
                                       // The value 0 is used in agConnectMgr.c:SysParser() to
                                       // identify the garbage disposal parser.  You can only have
                                       // 1 garbage disposal parser and this normally is PCL.
    Parser.ParseString = ACLParseString;
    Parser.ParseType = e_Print;
    // We only want to get here from PJL,  put in a dummy compare string so we will never get here through the system
    // parser.
    // Pass in a NULL for string and a 1 for the size.  A 1 for the size insures that this will not be called on
    // a non-compare.
    //
    ParserRegister(&Parser);
    rm_register(e_ACLParser, ACLPARSER);

    register_acl_cmds();

    posix_create_message_queue(&acl_queue, "/ACLParseQ", NUM_ACL_MESSAGES, sizeof(MESSAGE));
    router_register_queue(ACLPARSER, acl_queue);  // register

    posix_create_thread(&acl_thd_id, ACLParserEntry, 0, "acl_parser", ACLStack, ACL_STACK_SIZE, POSIX_THR_PRI_NORMAL);

    return;
}


/*
 * End of acl.c
 */
