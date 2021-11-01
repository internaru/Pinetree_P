/****************************************************************************** 
 * Copyright (c) 2004-2010  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

/**
 *
 * \brief Print the internal pages on this printer.
 *
 */

#include <stdint.h>
#include <string.h>

#include "agRouter.h"
#include "ATypes.h"
#include "agMessage.h"
#include "memAPI.h"
#include "lassert.h"
#include "agConnectMgr.h"
#include "agResourceMgr.h"
#include "logger.h"
#include "SJMgr.h"
#include "ResourceMap.h"
#include "internalPage.h"

#include "debug.h"
#include "error_types.h"
#include "sys_init_api.h"
#include "internal_page_config.h"
#include "sjm_observers_api.h"

#ifdef UNIT_TEST
#include "unit_test_api.h"
#endif // #ifdef UNIT_TEST


//OS stuff - 
#define NUM_IPM_MESSAGES 20
pthread_t       ipm_thread_id;
static mqd_t    ipm_msgq;
//#define IPMSTACK_SIZE POSIX_MIN_STACK_SIZE
#define IPMSTACK_SIZE 1024 * 1024  // 1MB
static ALIGN8 UINT8 IPMStack[IPMSTACK_SIZE];

#define IPAGE_MAX_NAME_LEN 512
#define IPAGE_NUM_PERMANENT_PAGE_ENTRIES 512

typedef struct ipage_permanent_page_entry_s
{
    char name[IPAGE_MAX_NAME_LEN]; /*!< The page name as registered with ipage_register_page */
    ipage_invoke_page_t invoke_function; /*!< The page invoke function */
    void *context; /*!< opaque data passed to caller when his page is invoked */
    bool starts_own_job; /*!< true if this page starts its own job, false otherwise */
} ipage_permanent_page_entry_t;

static uint32_t ipage_permanent_page_index;
static ipage_permanent_page_entry_t _ipage_permanent_page_list[IPAGE_NUM_PERMANENT_PAGE_ENTRIES];

static void* InternalPageMgrThread(void* unused);

static void ipm_notify(DOCTYPE const doc_type, ipm_job_status_t const status);

// support for pacing overlapping internal page requests
static MESSAGE SavedMsg;
#define IPAGE_MGR_IDLE()     ( SavedMsg.msgType == 0 ) 
#define SET_IPAGE_MGR_IDLE() ( SavedMsg.msgType = 0 ) 


#if defined(HAVE_CMD) || defined(UNIT_TEST)

#include "cmd_proc_api.h"

/**
 * \brief Utility function to print the requested internal page.
 *
 * \param index permanent page index.
 * 
 * \return void
 *
 */
static void invoke_one_ipage( uint32_t index )
{
    MESSAGE msg;
    error_type_t err;

    memset( &msg, 0, sizeof( msg ) );
    msg.msgType = MSG_INTERNAL_PAGE;
    msg.param1 = index;
    err = SYMsgSend( INTERNALMGRID, &msg );
    XASSERT( err==OK, err );
}

/**
 * \brief Utility function to request that all of the supported internal pages
 * be printed.
 *
 * \param none.
 * 
 * \return void
 *
 */
static void invoke_all_ipages( void )
{
    uint32_t i;

    // invoke each supported page, waiting for completion between pages 
    for ( i = 0; i < ipage_permanent_page_index; i++ )
    {
        // wait for any previous internal page to finish
        while ( !IPAGE_MGR_IDLE() )
        {
            sleep(1);
        }

        // invoke our desired page
        invoke_one_ipage( i );

        // allow the ipage mgr thread to get this request started
        sleep(1);
    }
}


static const char *ipage_cmd_list_desc = "get a list of all internal pages";
static const char *ipage_cmd_list_usage = NULL;
static const char *ipage_cmd_list_notes = NULL;

static int ipage_cmd_list_cb( int argc, char *argv[] )
{
    uint32_t i;

    if ( argc != 1 )
    {
        return CMD_USAGE_ERROR;
    }

    cmd_printf("List of all internal pages supported:\n");
    for(i = 0; i < ipage_permanent_page_index; i++)
    {
        cmd_append_result( "%s ", _ipage_permanent_page_list[i].name );
    }
    cmd_append_result( "all\n" );

    return CMD_OK;
}

static const char *ipage_cmd_print_desc = "print specific, or all, internal page(s)";
static const char *ipage_cmd_print_usage = "<page>";
static const char *ipage_cmd_print_notes = "page is one of the values listed when the 'ipage list' cmd is used, note that 'all' will print all of the internal pages";

static int ipage_cmd_print_cb( int argc, char *argv[] )
{
    uint32_t i;
    bool invoked;

    if ( argc != 2 )
    {
        return CMD_USAGE_ERROR;
    }

    invoked = false;

    if ( strncmp( argv[1], "all", strlen( "all" ) ) == 0 )
    {
        // print all the supported user pages
        invoke_all_ipages();
    }
    else
    {
        // search the supported pages table for the requested page
        for ( i = 0; i < ipage_permanent_page_index; i++ )
        {
            if ( strncmp( argv[1], _ipage_permanent_page_list[i].name, strlen( _ipage_permanent_page_list[i].name ) ) == 0 )
            {
                // print the requested page
                invoke_one_ipage( i );
                invoked = true;
                break;
            }
        }
    }

    if ( false == invoked )
    {
        cmd_printf( "Invalid internal page specified '%s' is not a valid internal page\n", argv[1] );
        return CMD_USAGE_ERROR;
    }

    return CMD_OK;
}
#endif
 
#ifdef UNIT_TEST
/**
 * \brief Unit test function to print all the supported user internal pages in
 *        the currently configured language.
 *
 * \param none.
 * 
 * \return int
 * \retval 0, test completed successfully
 * \retval non-zero, test failed
 *
 */
static int ipages_current_lang_unit_test( void )
{
    char *argv[2];

    // setup for our registered debug callback
    argv[0] = "ipage print";

    // print "all" pages in the current language
    argv[1] = "all";
    return ipage_cmd_print_cb( 2, argv );
}

#endif


/** 
 *
 * \brief Initialize the internal page manager.
 * 
 */
void ipage_init(void)
{
    ipage_permanent_page_index = 0;

    posix_create_message_queue(&ipm_msgq, "/iPageQ", NUM_IPM_MESSAGES, sizeof(MESSAGE));
	DPRINTF(DBG_ERROR|DBG_OUTPUT,("INTERNAL MQ_PTR : %p\n", ipm_msgq));
    router_register_queue(INTERNALMGRID, ipm_msgq);

    rm_register(e_InternalPages, INTERNALMGRID);

    posix_create_thread(&ipm_thread_id, InternalPageMgrThread, 0, "internal_pg_mgr", IPMStack, IPMSTACK_SIZE, POSIX_THR_PRI_NORMAL);
	DPRINTF(DBG_ERROR|DBG_OUTPUT,("INTERNAL MQ_SIZE : %d\n", IPMSTACK_SIZE));

    #ifdef UNIT_TEST
    {
        // register our unit test cases
        error_type_t unit_test_res;
        unit_test_res = unit_test_register_testcase("ipages_current_lang", ipages_current_lang_unit_test);
        ASSERT( OK == unit_test_res );
    }
    #endif

    /* initialize the internal page manager's config layer */
    internal_page_config_init();

}

 
/** 
 * \brief Send a message to the sjm requesting resources for this job.
 * 
 * \param nextMsg The message we received 
 * 
 * \retval None
 * 
 */
void GetResources(MESSAGE *nextMsg)
{
    STARTRECIPE *StartIt;

    StartIt = (STARTRECIPE *) MEM_MALLOC(sizeof(STARTRECIPE));
    ASSERT(StartIt != NULL);

    // we have a connection, get the resources and start.
    //
    StartIt->Job = ejob_InternalPagePrint;
    StartIt->AdditionalResource = e_InternalPages;
    StartIt->Wait = e_WaitForever;
    // for all message types except MSG_CLEAN_PRINTER the param3 should be null
    // MSG_CLEAN_PRINTER will have the param3 set to its Pipe address and is needed 
    // later for USTATUS TIMED calls. We may discover in future that other internal 
    // pages that take more than 1 minute to print may need to be added to this list.
    // GetResources used to set this paw
    if( nextMsg->msgType == MSG_CLEAN_PRINTER )
    {
        StartIt->Pipe = (IOReg *) nextMsg->param3;
    }
    else
    {
        StartIt->Pipe = NULL;
    }

    nextMsg->msgType = MSG_STARTRECIPE;



    StartIt->SendingModule = INTERNALMGRID;

    nextMsg->param3 = StartIt;

    SYMsgSend(SJMID, nextMsg);         // send the message.

}

UINT32 checkIPMCancel(UINT32 timeOutTicks)
{
    MESSAGE msg;
    if(0 == posix_wait_for_sys_message(ipm_msgq, (char*)&msg, timeOutTicks))
    {
        if(msg.msgType == MSG_CANCELJOB)
           return (UINT32)(msg.param3);
        else
        {
            SYMsgSend(INTERNALMGRID, &msg);   //Forward this to myself
            posix_sleep_ms(timeOutTicks*MILLISEC_PER_TICK); //Sleep for a while so I won't get into a loop without delay
        }
        return 0;
    }else
        return 0;

} 

void ackIPMCancel(UINT32 param3)
{
    MESSAGE msg;
    msg.msgType = MSG_CANCELJOB;
    msg.param1 = SYS_ACK;
    msg.param2 = INTERNALMGRID;
    msg.param3 = (void *)param3;
    SYMsgSend( SJMID, &msg); 
}
 
 
/** 
 * \brief Internal page manager thread
 * This thread looks at the message box and handles messages that are received.
 * The main purpose of this thread is to generate and print internal pages.
 * 
 * \param unused ditto
 * 
 * \retval None
 * 
 */
static void* InternalPageMgrThread(void* unused)
{
    error_type_t e_res;
    CURRENT_RESOURCE  *OurResource = 0;
    MESSAGE nextMsg;
    DOCTYPE doc_type = e_NONE;  //init to invalid doc type    
     

    //Mark the SavedMsg as unused
    SET_IPAGE_MGR_IDLE();
    //wait for system init to finish
    SysWaitForInit();

    #ifdef HAVE_CMD
    int cmd_res;
    /* register a debug command with the serial debug thread */
    cmd_res = cmd_register_cmd( "ipage",
                                NULL,
                                "internal page commands",
                                NULL,
                                NULL,
                                NULL );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "ipage",
                                   "list",
                                   ipage_cmd_list_desc,
                                   ipage_cmd_list_usage,
                                   ipage_cmd_list_notes,
                                   ipage_cmd_list_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    cmd_res = cmd_register_subcmd( "ipage",
                                   "print",
                                   ipage_cmd_print_desc,
                                   ipage_cmd_print_usage,
                                   ipage_cmd_print_notes,
                                   ipage_cmd_print_cb );
    XASSERT( cmd_res == CMD_OK, cmd_res );
    #endif


    while (1)
    {
        mq_receive(ipm_msgq, (char*)&nextMsg, sizeof(MESSAGE), 0);

        DPRINTF(DBG_LOUD|DBG_OUTPUT,("Received message %x\n",nextMsg.msgType));

        //
        // get the resources for this job.
        //

        switch (nextMsg.msgType)
        {
        case MSG_INTERNAL_PAGE:
                if ( false == _ipage_permanent_page_list[nextMsg.param1].starts_own_job )
                {
                    //Start a new one if previous report is done by checking the msgType in SavedMsg
                    if ( IPAGE_MGR_IDLE() ) 
                    {
                        //
                        //IPM receives internal-page-printing request, but hasn't get the resource,
                        //send "Job Received" notification      
                        //
                        ASSERT(nextMsg.param1 >= e_SYSTEMREPORT && nextMsg.param1 <= e_DDPAGE);
                        doc_type = (DOCTYPE)nextMsg.param1;
                        ipm_notify(nextMsg.param1, ipm_JobReceived);

                        //SavedMsg is free
                        memcpy(&SavedMsg, &nextMsg, sizeof(MESSAGE));
                        GetResources(&nextMsg);
                    }
                    else
                    {
                        //Wait and resend the msg to myself to keep the msg alive
                        sleep(1);
                        SYMsgSend(INTERNALMGRID, &nextMsg); 
                    }
                }
                else
                {
                    /* handle pages that start their own job */
                    /* param1 is the page id */
                    e_res = _ipage_permanent_page_list[nextMsg.param1].invoke_function( _ipage_permanent_page_list[nextMsg.param1].name,
                                                                                        _ipage_permanent_page_list[nextMsg.param1].context );
					DPRINTF(DBG_ERROR|DBG_OUTPUT,("INTERNAL MQ_PTR : %p[%p]\n", &ipm_msgq, ipm_msgq));

                }
                break;

            case MSG_RESOURCES:
                {
                    // save the job's resources
                    OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                    break;
                }

                //
                // We got the resources, start the page requested.
                //
            case MSG_ACKRECIPE:
                {
                    //IPM has get the resource from SJM, could start the internal-page-printing
                    //send "Job Start" notification  
                    ipm_notify(doc_type, ipm_JobStart);

                    mlimiter_start( mlimiter_by_name("ipage_print"));

                    ASSERT( MSG_INTERNAL_PAGE == SavedMsg.msgType );
                    #ifdef RENDERER

                    DPRINTF( ( DBG_LOUD|DBG_OUTPUT ),
                             ( "INTERNAL PAGE (%s) PRINTING......\n", _ipage_permanent_page_list[SavedMsg.param1].name ) );

                    sleep(1);

                    e_res = _ipage_permanent_page_list[SavedMsg.param1].invoke_function( _ipage_permanent_page_list[SavedMsg.param1].name,
                                                                                         _ipage_permanent_page_list[SavedMsg.param1].context );

                    nextMsg.msgType = MSG_FREERECIPE;
                    nextMsg.param3 = OurResource;
                    SYMsgSend(SJMID, &nextMsg); // param3 is untouched and now sent back.
                    #endif

                    mlimiter_stop( mlimiter_by_name("ipage_print"));

                    SET_IPAGE_MGR_IDLE(); //Mark the end of current report printing

                    //Internal-page-printing has done
                    //send "Job Done" notification
                    ipm_notify(doc_type, ipm_JobDone);

                    break;
                }


            case MSG_CANCELJOB:
                //
                // When we get to here we just ack the cancel and exit.
                //
                nextMsg.param1 = SYS_ACK;
                nextMsg.param2 = INTERNALMGRID;
                SYMsgSend(SJMID, &nextMsg);

                ipm_notify(doc_type, ipm_JobCancel);

                break;

            default:
                XASSERT( 0, nextMsg.msgType );
                break;
        }
    }

    return 0;
}


error_type_t ipage_register_page( const char *name,
                                  bool starts_own_job,
                                  ipage_invoke_page_t invoke_function,
                                  void *context )
{
    error_type_t e_res;

    ASSERT( NULL != name );
    ASSERT( NULL != invoke_function );

    e_res = OK;

    if ( ipage_permanent_page_index >= IPAGE_NUM_PERMANENT_PAGE_ENTRIES )
    {
        e_res = FAIL;
    }

    if ( OK == e_res )
    {
        strncpy( _ipage_permanent_page_list[ ipage_permanent_page_index ].name,
                 name,
                 IPAGE_MAX_NAME_LEN );

        _ipage_permanent_page_list[ ipage_permanent_page_index ].invoke_function = invoke_function;

        _ipage_permanent_page_list[ ipage_permanent_page_index ].starts_own_job = starts_own_job;

        _ipage_permanent_page_list[ ipage_permanent_page_index ].context = context;

        ipage_permanent_page_index++;
    }

    return e_res;
}

typedef struct ipm_subject_s
{
    Subject_T_BaseClassMembers;

    MLISTENTRY      observers;

    DOCTYPE         doc_type;
    ipm_job_status_t status;
} ipm_subject_t;


static ipm_subject_t* ipm_notification(void)
{
    static ipm_subject_t    notification;
    static ipm_subject_t    *pnotification = NULL;

    if(NULL == pnotification)
    {
        notification.subject_observer_oid_key = IPM_NOTIFICATION_ID;
        notification.doc_type = e_NONE;     //invalid doc type
        notification.status = ipm_JobNone;
        MInitList(&(notification.observers));

        pnotification = &notification;
    }
    
    return  pnotification;
}


void ipm_notify(DOCTYPE const doc_type, ipm_job_status_t const status)
{
    ipm_notification()->doc_type = doc_type;
    ipm_notification()->status = status;
    NOTIFY_OBSERVERS(ipm_notification(), observers);
}

ipm_job_status_t ipm_get_job_status( Subject_t *s)
{
    ASSERT(NULL != s);
    ASSERT(VALIDATE_OBJECT_KEY(s, ipm_subject_t, subject_observer_oid_key, IPM_NOTIFICATION_ID));
    return  ipm_notification()->status;
}

DOCTYPE ipm_get_doc_type(Subject_t *s)
{
    ASSERT(NULL != s);
    ASSERT(VALIDATE_OBJECT_KEY(s, ipm_subject_t, subject_observer_oid_key, IPM_NOTIFICATION_ID));
    return  ipm_notification()->doc_type;
}

void ipm_attach_observer(Observer_t *o)
{    
    ASSERT(NULL != o);
    ATTACH_OBSERVER(ipm_notification()->observers, o);
}

void ipm_detach_observer(Observer_t *o)
{
    ASSERT(NULL != o);
    DETACH_OBSERVER(ipm_notification()->observers, o);
}

