

/*
 * ============================================================================
 * Copyright (c) 2011   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */


#include <string.h>
#include <fcntl.h>

#include "lassert.h"
#include "posix_ostools.h"
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "memAPI.h"
#include "mlimiter.h"
#include "logger.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "sys_init_api.h"
//#include "printer_class_app_api.h"
#include "list.h"
#include <sys/stat.h>

#define NUM_UPT_MESSAGES 10

#define UPT_STACK_SIZE POSIX_MIN_STACK_SIZE 
#define DBG_PRFX "PTH: "                      // simple string prepend to the front of all strings.
#define LOGGER_SUBMOD 0x00000800              // submodule/channel is this bit, if set then log.
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_SYSTEM | LOGGER_SUBMOD  // 

static pthread_t px_UPTParser_thd_id;
static mqd_t px_UPTParseQ;

static uint8_t upt_stack[UPT_STACK_SIZE] __attribute__ ((aligned (8)));
//static int32_t printer_present=0;
#define WRITE_SUCCESS 0
#define WRITE_TO_MASK 1

// HACK- NEEDS TO BE REMOVER WHEN CPU cpu_get_dcache_line_size function is ported
#define CACHE_LINE_SIZE 32

static ATLISTENTRY data_list;
typedef enum
{
    NODE_READY,
    NODE_SENT
} data_node_status_t;

typedef struct
{
    ATLISTENTRY list_node;
    uint8_t *data;
    uint8_t *data_ptr;
    uint32_t length;
    data_node_status_t status;
} data_node_t;

// HACK- NEEDS TO BE REMOVER WHEN CPU cpu_get_dcache_line_size function is ported
#define CACHE_LINE_SIZE 32
#define cpu_get_dcache_line_size() CACHE_LINE_SIZE

static void *usb_host_entry(void * input);

void print_to_usb_host_parser_init(void)
{
    rm_register(e_UsbHostParser, USBHOSTPARID);

    posix_create_MESSAGE_queue( &px_UPTParseQ, "/UPT", NUM_UPT_MESSAGES );

    router_register_queue(USBHOSTPARID, px_UPTParseQ); // Register a message queue
    ATInitList(&data_list);

    posix_create_thread(&px_UPTParser_thd_id, usb_host_entry, 0, "USBHostPar", upt_stack,
                        UPT_STACK_SIZE, POSIX_THR_PRI_NORMAL);
}
/*
static void usb_notify(printer_class_app_notify_event_t event, uint32_t index)
{
    DPRINTF(DBG_LOUD|DBG_OUTPUT,("Got notify event %d\n",index));
    if(event == printer_class_app_notify_event_inserted)
    {
        printer_present = index;
    } else
        printer_present = -1;
}
*/

#define USB_INPUT_BUF_SIZE 8192
static void *usb_host_entry(void * input)
{
    MESSAGE     nextMsg;
    CURRENT_RESOURCE  *OurResource = 0;
    uint32_t status;
    //uint8_t write_status;
    uint32_t timeout = POSIX_WAIT_FOREVER;
    data_node_t *data_node;
    uint8_t *buffer = 0;
    int32_t amount_data = 0;
    int printer_fd = -1 ;
    struct stat st;

    SysWaitForInit();
 //   printer_class_app_notify_register(usb_notify);
    
    if (stat("/dev/usb/lp0", &st) == 0)  // ubuntu usb printer
    {
        printer_fd = open("/dev/usb/lp0", O_RDWR | O_NONBLOCK);
        dbg_printf("opened /dev/usb/lp0 with fd %d\n", printer_fd);
    }
    if (printer_fd < 0 )                 // fallback local directory file 
    {
      if (stat("/dev/lp0", &st) == 0)      // gemstone usb printer
      {
        printer_fd = open("/dev/lp0", O_RDWR | O_NONBLOCK);
        dbg_printf("opened /dev/lp0 with fd %d\n", printer_fd);
      }
    }
    if (printer_fd < 0 )                 // fallback local directory file 
    {
        printer_fd = open("lp0_tmp", O_RDWR | O_CREAT | O_TRUNC, 0666);
        dbg_printf("opened lp0_tmp with fd %d\n", printer_fd);
    }
    if (printer_fd < 0)
        dbg_printf("open fail lp0 fd %d errno %d \n", printer_fd, errno);
    ASSERT( printer_fd != -1 );

    while (1)
    {
        //
        // In order to insure that I will not have my queue fill, read everything
        // from the queue before doing any sort of xfers.
        if ( timeout ) {
            posix_sleep_us(timeout);
        }
        status = posix_wait_for_message(px_UPTParseQ, (char*)&nextMsg, sizeof(MESSAGE), 0);
        if (status == 0 ) 
        {
            switch (nextMsg.msgType)
            {
            case MSG_RESOURCES:
            {
                // we are about to be included in a job.
                OurResource = (CURRENT_RESOURCE *) nextMsg.param3;
                break;
            }
            case MSG_ACKRECIPE:
            {
                // start the job
                break;      // done
            }
            case MSG_PRINT_DATA:
            {
                // set the timeout to zero so we wait on the writes.
                timeout = 0;
                // we got some data from the host, put it onto
                // the write queue.
                //
                //ASSERT(printer_present >= 0);  // do we have somewhere to go?
                // get memory for the list entry.
                data_node = MEM_MALLOC(sizeof(data_node_t));
                ASSERT(data_node != 0); // should never fail
                // initialize the list entry
                memset(data_node, 0, sizeof(data_node_t));
                // populate the list.
                data_node->data = data_node->data_ptr = nextMsg.param3;
                data_node->length = nextMsg.param2;
                DBG_PRINTF_ERR("Got %x length %d\n",data_node->data_ptr, data_node->length);
                data_node->status = NODE_READY;
                ATInitNode(&data_node->list_node);
                // put the list on the queue.  Note we do not
                // need to semaphore protect this since this is only
                // used in this thread.
                ATInsertTailList(&data_list, &data_node->list_node);
                break;
            }
            case MSG_JOBEND:
                nextMsg.msgType = MSG_FREERECIPE;
                nextMsg.param3 = OurResource;
                SYMsgSend(SJMID, &nextMsg);
                break;
            default:
                break;
            } // end switch
        } // end have message
        //
        // See if we have anything coming back from the printer
        //       
        if (!buffer) 
            buffer = MEM_MALLOC_LIMITED(mlimiter_by_name("print"), USB_INPUT_BUF_SIZE ); //  , cpu_get_dcache_line_size() );
        if(buffer)
        {
            // we have a buffer, try and read something.
            //
//            write_status = printer_class_app_read(printer_present, buffer, USB_INPUT_BUF_SIZE,10, &amount_data);
            amount_data = read(printer_fd, buffer, USB_INPUT_BUF_SIZE);
            if(amount_data > 0)
            {
                // buffer = MEM_REALLOC_LIMITED(mlimiter_by_name("print"),buffer, amount_data);  // cut the buffer down to size.
                DBG_PRINTF_ERR("send backchannel data from usb to passthru %x\n", amount_data);
                nextMsg.msgType = MSG_PRINT_DATA;
                nextMsg.param3 = buffer;
                nextMsg.param2 = amount_data;
                SYMsgSend(OurResource->Source, &nextMsg);   // send it back.
                buffer = 0; // pass ownership
            } 
            else if (errno == EWOULDBLOCK) 
            { 
              timeout = 0;
            }
        }
        //
        // We have finished emptying the queue, now see if we have anything
        // to send on the write list.
        // First get the top of the write queue.
        data_node = (data_node_t *) ATListHead(&data_list);
        // do we have anything to do
        if (data_node != NULL)
        {
            // yes, see if it has already been sent.
            if (data_node->status == NODE_READY)
            {
                // this has not been sent, send it now.
//                write_status = printer_class_app_write(printer_present, data_node->data_ptr, data_node->length, 30);

          // LSPTODO : write timeout???

               // DBG_PRINTF_ERR("before writing %d\n",data_node->length);
                amount_data = write(printer_fd, data_node->data_ptr, data_node->length);
               // DBG_PRINTF_ERR("write_status %d, errno %d\n",amount_data, errno);
                // see if we timedout.
//                if ((write_status & WRITE_TO_MASK) == WRITE_SUCCESS)
                // if amount of data wrote == length of data, write completed.
                if (amount_data == data_node->length)  
                {
                    DBG_PRINTF_ERR("wrote %d, remaining length %d\n", amount_data, data_node->length - amount_data);
                    // write completed, free everything.
                    data_node = (data_node_t *)ATRemoveHeadList(&data_list);
                    MEM_FREE_AND_NULL(data_node->data);
                    MEM_FREE_AND_NULL(data_node);

                }
                else if (amount_data > 0)
                {
                    DBG_PRINTF_ERR("wrote %d, remaining length %d\n", amount_data, data_node->length - amount_data);
                    data_node->length -= amount_data;
                    data_node->data_ptr += amount_data;
                }
                else
                {
                    //DBG_PRINTF_ERR("write_status check status %d\n", amount_data);  
                    // set the status to do a get status write.
                    // data_node->status = NODE_SENT;

                   // DBG_PRINTF_ERR("write error status %d, data_ptr %p, length %d, errno %d \n", amount_data, data_node->data, data_node->length, errno);

                    if( errno != EAGAIN) // This may not be needed.
                    {
                        data_node = (data_node_t *) ATRemoveHeadList(&data_list);
                        MEM_FREE_AND_NULL(data_node->data);
                        MEM_FREE_AND_NULL(data_node);
                    }
                }
            }
        }
        else
        {
            timeout = 30;       // nothing to send, wait on the queue receive
        }
    }
    close(printer_fd);
    return 0;
}

