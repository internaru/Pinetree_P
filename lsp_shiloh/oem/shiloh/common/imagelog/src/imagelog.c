/**
 *
 * ============================================================================
 * Copyright (c) 2015 SINDOH, Inc. All Rights Reserved
 *
 *                         SINDOH Confidential
 * ============================================================================
 *
 *
 **/

 /**
 * \file imagelog.c 
 * \brief Implements a imagelog generator.
 */

/** include files **/
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <dirent.h>

#include "agRouter.h"
#include "memAPI.h"
#include "logger.h"
#include "debug.h"
#include "lassert.h"
#include "agMessage.h"
#include "agResourceMgr.h"
#include "sys_init_api.h"
#include "print_job_types.h"
#include "dma_buffer.h"
#include "dbg_pnm.h"
#define IL_MAIN_C
#include "imagelog.h"
#undef IL_MAIN_C
#include "imagelogMonitor.h"
#include "SysUI_Interface.h"
#include "db_api.h"

/* -------------------------------------------------------------------------- */
/*                               D E F I N E S                                */
/* -------------------------------------------------------------------------- */
#define IL_STACK_SIZE 1 * 1024 * 1024 // 30MB -> 1MB
#define IL_QUEUE_SIZE 30

#define DBG_PRFX "IL: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_IMAGELOG | LOGGER_SUBMODULE_BIT( 1 )
#define IL_SDH_ERR DBG_PRINTF_SDH_ERR
#define IL_SDH_DEBUG DBG_PRINTF_SDH_DEBUG

#define META_FILENAME ".meta"
#define IMAGE_LOG_MIN_MEMSIZE (100)	//In MB
#define IMAGE_LOG_DEL_OLD_JOB_MEMSIZE (IMAGE_LOG_MIN_MEMSIZE*5)	//In MB

#define IMAGE_LOG_PATH_MAX 256
#define FILE_SYSTEM_EXT4      0xEF53

/* -------------------------------------------------------------------------- */
/*                         L O C A L  V A R I A B L E S                       */
/* -------------------------------------------------------------------------- */
static mqd_t il_queue;
static pthread_t il_thread;
static ALIGN8 uint8_t il_stack[IL_STACK_SIZE];
static pthread_mutex_t il_global_lock;
static pthread_mutex_t il_data_lock;
//static gen_compress_context_t il_context;
uint32_t g_nImageLogMemFull = 0; ///0: Not Full 1: Memory Full (Do not allow job in SJM)
uint32_t g_nImageLogSDCardError = 0; ///0: Valid 1: Invalid format (Do not allow job in SJM)
uint32_t g_nSaveImageLog = 99; ///<Save Image Log 0: Off 1: On
uint32_t g_nIsEnableImageLog = 0; ///<Image Log 0: Off 1: On
uint32_t g_nILFileTransfer = 0; ///<File Transfer 0: Off 1: On
uint32_t i_FlagPrintLog = 0; //<MSG Flag of Print/copy Image Log 0: off 1: on
uint32_t i_IsCopy = 0; //< Internal flag for scale 0: print / 1: copy
static uint32_t g_nILResolution = 0; ///<Set Resolution of Image Log 100~600 - Default: 300
static uint32_t g_nIsPrintImgLogEnable = 0; ///<Save Print Image Log 0: Off 1: On
static uint32_t g_nIsCopyImgLogEnable = 0; ///<Save Copy Image Log 0: Off 1: On
static uint32_t g_nIsScanImgLogEnable = 0; ///<Save Scan Image Log 0: Off 1: On
static uint32_t g_nIsFaxRxImgLogEnable = 0; ///<Save Fax Receive Image Log 0: Off 1: On
static uint32_t g_nIsFaxTxImgLogEnable = 0; ///<Save Fax Transfer/Send Image Log 0: Off 1: On
static uint32_t g_nILSavedPageCount = 0; ///<Count Image Log Saved page counts
static uint32_t g_nILSDCardFullSCDisplay = 0; ///<0: Do not display 1: Display SC occur
//static uint32_t g_nIsEnableAria = 0; ///<ARIA Encrypt 0: off 1: on
uint32_t gImageLogSNMPStatus = 0;	/// <Current Image Log Status for SNMP

#define MAX_IL_PLANE_LIST 2
#define MAX_IL_PAGE_COUNT_LIST 50

MLISTENTRY planeList; 	///< Array of lists of plane data buffers not yet processed

typedef struct il_struct
{
	ATLISTENTRY node;
	int width;
	int height;
	int bpp;
	int dpi;
	bool isEndPlane; // Save PDF Doc
	void* data;
}IL_STRUCT;

#define DPI_600 600
#define DPI_300 300

#define LOG_PATH_LENGTH 257

#define JOBID 12313 //test

static char il_filename[LOG_PATH_LENGTH];
static char il_p_filename[LOG_PATH_LENGTH];
static char il_c_filename[LOG_PATH_LENGTH];
static char il_s_filename[LOG_PATH_LENGTH];
static char il_f_filename[LOG_PATH_LENGTH];

static ATLISTENTRY il_filename_list;

typedef struct il_struct_filename
{
    int _jobid;
    char list_il_filename[LOG_PATH_LENGTH];
    ATLISTENTRY ListNode;
}IL_FILENAME;
static char list_return_filename[LOG_PATH_LENGTH];
static char list_return_pdffilename[LOG_PATH_LENGTH];

/* -------------------------------------------------------------------------- */
/*                    F O R W A R D  D E C L A R A T I O N S                  */
/* -------------------------------------------------------------------------- */

extern void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);
void SetInitData();
int GetSDCardStatus();
int GetFileSystemMemUsage(char *path);
int GetFileSystemType(char *path);
void ImageLogMonitorNotify(int notifyType);
int DeleteImageLog(int option);
void SetImageLogStatus(uint32_t status);
extern int ImageLogDeleteFolder(char *logPath);
int make_newjob_info(char* buffer, char _jobtype, char* machineSerial, char* timestamp, int _jobid);
int make_endjob_info(char _jobtype, char* buffer, int _jobid);

static int get_ILfilename(int _jobid, int RemoveFlag); // Remove Flag 1: remove
static void save_ILfilename(int _jobid, char *filename);

static void* il_task( void *unused );

//static int plane_count = 0;
static HPDF_Doc pdf;
static HPDF_Page pdfpage;
static int page_height = 0;

int il_init()
{
	int px_status = 0;

    ATInitList(&il_filename_list);
    ATInitList(&il_fax_broadcast_list);

	MInitList(&planeList);

    px_status = posix_mutex_init( &il_global_lock );
    XASSERT( px_status==0, px_status );

    px_status = posix_mutex_init( &il_data_lock );
    XASSERT( px_status==0, px_status );

	// Create a mail box and register it.
	posix_create_MESSAGE_queue( &il_queue, "/il_mq", IL_QUEUE_SIZE );
	// Register the queue with the router.
	router_register_queue(IMAGE_LOG_ID, il_queue);
	// start the generator task
	posix_create_thread(&il_thread,
						il_task,
						0,
						"ImageLogThread",
						il_stack,
						IL_STACK_SIZE,
						POSIX_THR_PRI_LOW);


	//Check DB for ImageLog Enable and then check SD Card
	SetInitData();

	//Initialize ImageLog Monitor Thread
	ImageLogMonitorInit();
	return 0;
}

void* il_task( void *unused )
{
    MESSAGE msg;
    error_type_t err;
    int iret;
    int page_number = 0;
    //	error_type_t ret;
    //	error_type_t err;
    PAGE_DATA *page = NULL;
    PLANED *plane = NULL;

    int line_cnt=0;
    int strip_cnt=0;

    SysWaitForInit();

    ImageLogMonitorNotify(IL_NOTIFY_SYS_INIT);

    // main loop
    while(1)
    {
        iret = posix_wait_for_message(il_queue, (char*)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
        if (iret != 0)
            continue;  // signal interrupt

        if(g_nSaveImageLog == 0){ //Do not save Image Log data
            if(msg.msgType != MSG_CONFIG_CHANGED){
                continue;
            }
        }

        switch(msg.msgType)
        {
            case MSG_JOBSTART:
                {
                    //			char username[] = "test";
                    //			set_metafile_start_info('C', 3, username);
                    //			set_metafile_start_info('P', 5, username);

                    //			IL_SDH_DEBUG("IL: MSG_JOBSTART\n");
                    page_number = 0;
                    break;
                }
            case MSG_JOBPAGESTART:
                {
                    line_cnt = 0;
                    strip_cnt = 0;

                    page = (PAGE_DATA *)msg.param3;
                    ASSERT(page != NULL);
                    ASSERT(IS_PAGE_DATA(page));

                    IL_PDF_NewDoc();
#if 0  // dpi choice(temporary)
                    // if(600dpi)
                    IL_PDF_NewPage(page->image_output_info.image_width_in_pixels, page->image_output_info.image_height_in_lines, CONVERT_600DPI);
#else
                    // elseif(300dpi)
                    IL_PDF_NewPage(page->image_output_info.image_width_in_pixels, page->image_output_info.image_height_in_lines, CONVERT_600DPI);
                    // endif
#endif
                    usleep(20);

                    //IL_SDH_ERR("IL: MSG_JOBPAGESTART %d\n", page->PageCount);
                    //page_number = page->PageCount;
                    IL_SDH_ERR("IL: MSG_JOBPAGESTART %d\n", page_number);
                    break;
                }
            case MSG_JOBPLANEDATA:
                {
                    plane = (PLANED *)(msg.param3);
                    ASSERT(IS_PLANED(plane));

                    strip_cnt++;
                    line_cnt += plane->image_info.image_height_in_lines;
                    //			if (line_cnt > 6000)
                    //				IL_SDH_ERR("received data[%d] : %d/%d, last(%d)\n", strip_cnt, plane->image_info.image_height_in_lines, line_cnt, plane->LastData);

                    IL_get_raw_data(plane);

                    BigBuffer_Free(plane->big_buffer);
                    MEM_FREE_AND_NULL(plane);
                    plane = NULL;

                    break;
                }
            case MSG_JOBPAGEEND:
                {
                    IL_SDH_ERR("MSG_PAGEEND: Received\n");
                    HPDF_REAL page_offset = 0.0;
                    ATLISTENTRY *next = 0;
                    //			ATLISTENTRY *temp = 0;
                    int nCnt=0;
                    MLISTENTRY* pList;
                    IL_STRUCT* il_list;
                    HPDF_REAL convert_dpi = 0;

                    int _jobid = 0;
                    _jobid = (int)(msg.param3);

                    IL_SDH_ERR("MSG_PAGEEND: msg copy\n");


                    pList = &planeList;
                    //			IL_SDH_ERR("MSG_JOBPAGEEND! Plane is %d. \n", MNumListNodes(pList));

                    //			next = MListHead(pList);
                    for (next = MListHead(pList); next; )
                    {
                        //				temp = MListNext(pList, next);
                        il_list = CONTAINING_RECORD(next, IL_STRUCT, node);

                        if(il_list->dpi == DPI_300)
                            convert_dpi = CONVERT_300DPI;
                        else
                            convert_dpi = CONVERT_600DPI;
                        page_offset = page_offset + (HPDF_REAL)(il_list->height/convert_dpi);
                        IL_PDF_DrawImage((HPDF_BYTE*)il_list->data, il_list->width, il_list->height, (HPDF_REAL)page_height-page_offset, il_list->bpp, convert_dpi);

                        next = MListNext(pList, next);

                        MRemoveEntryList(pList, &il_list->node);
                        MEM_FREE_AND_NULL(il_list->data);
                        MEM_FREE_AND_NULL(il_list);
                        nCnt++;
                    }
                    //            _jobid = job_data->Resources->JobResources->JobId;
                    //            JOB_RESOURCES *tempJRPtr = ((CURRENT_RESOURCE *)job_data->Resources)->JobResources;
                    //            IL_SDH_ERR("MSG_PAGEEND: %d\n",_jobid);
                    //            _jobid = tempJRPtr->JobId;
                    //            _jobid = ((CURRENT_RESOURCE *)job_data->Resources)->JobResources->JobId;
                    /*            if (_jobid == NULL) {
                                  IL_SDH_ERR("MSG_PAGEEND: Jobid is NULL!!!!!\n");
                                  }
                                  else{
                                  IL_SDH_ERR("MSG_PAGEEND: HAVE JOBDATA!!!!!\n");
                                  }*/
                    if(get_ILfilename(_jobid, 0) == 0){ //revomeflag on (1)
                        IL_PDF_SaveDoc(page_number);
                        IL_SDH_ERR("MSG_PAGEEND: %d [%s]\n",_jobid, list_return_pdffilename);
                    }
                    else
                        IL_SDH_ERR("MSG_PAGEEND: No file name\n");  

                    g_nILSavedPageCount++;
                    page_number++;

                    if(g_nILSavedPageCount >= MAX_IL_PAGE_COUNT_LIST){
                        g_nILSavedPageCount = 0;
                        //Check SD Card Memory State
                        CheckImgLogMemDoCancelAct('P', 0);
                    }
                    IL_SDH_ERR("[Info] nCnt : %d\n", nCnt);
                    _jobid = 0;
                    /*
                       IL_SDH_ERR("[Info] pri mem current=%d available=%d max=%d\n", mlimiter_by_name("print")->current,
                       mlimiter_available(mlimiter_by_name("print")), mlimiter_by_name("print")->max);
                       IL_SDH_ERR("[Info] sys mem current=%d available=%d max=%d\n", mlimiter_by_name("syslimiter")->current,
                       mlimiter_available(mlimiter_by_name("syslimiter")), mlimiter_by_name("syslimiter")->max);
                       */
                    break;
                }
            case MSG_JOBABORT:
            case MSG_JOBEND:
                {
                    page_number = 0;            
                    //			char filename[] = "print.pdf";

                    //			set_metafile_end_info('P', NULL, 2, filename, 12);
                    //			set_metafile_end_info('C', NULL, 4, filename, 22);

                    break;
                }
            case MSG_SENDJOB_RESULT:
                {
                    err = SYMsgSend(IMAGE_LOG_MONITOR_ID, &msg);
                    XASSERT( err==OK, err );
                    if((msg.param2 == 0) && g_nImageLogMemFull){
                        IL_SDH_ERR("ImageLog delivery in progress\n");
                        GetFileSystemMemUsage(IMAGE_LOG_PATH);
                    }
                    break;
                }
            case MSG_CONFIG_CHANGED:
                {
                    IL_SDH_ERR("ImageLog MSG_CONFIG_CHANGED received\n");
                    //On Config Change msg:
                    //Check DB for ImageLog Enable and then check SD Card
                    SetInitData();

                    //Send CONFIG_CHANGE to ImageLogMonitor
                    err = SYMsgSend(IMAGE_LOG_MONITOR_ID, &msg);
                    XASSERT( err==OK, err );
                    break;
                }
            default:
                {
                    break;
                }
        } // switch

    } // while
    return 0;
}

void IL_get_raw_data(PLANED *plane)
{
	BigBuffer_t *output_buffer;
	void* il_buffer;
	MLISTENTRY* pList;
	IL_STRUCT* il_list;

//	plane->UseCount++;
	output_buffer = plane->big_buffer;
	dma_buffer_map_single(output_buffer, DMA_TO_DEVICE);
	dma_buffer_unmap_single(output_buffer, DMA_TO_DEVICE);
	void* data = dma_buffer_mmap_forcpu(output_buffer);

	il_list = (IL_STRUCT*)MEM_MALLOC(sizeof(IL_STRUCT));
    ASSERT(il_list != NULL);
    if(il_list  == NULL)
    {
        IL_SDH_ERR("%s: out of memory\n", __func__);
		return;
    }
    ATInitNode(&il_list->node);

	il_buffer = (void*)MEM_MALLOC(plane->DataLength);
//	il_buffer = (void*)MEM_MALLOC_LIMITED(mlimiter_by_name("print"), plane->DataLength);
	
	if(il_buffer == NULL)
	{
		IL_SDH_ERR("Warning : print mem current=%d available=%d max=%d\n", mlimiter_by_name("print")->current,
													mlimiter_available(mlimiter_by_name("print")), mlimiter_by_name("print")->max);
		IL_SDH_ERR("Warning : sys mem current=%d available=%d max=%d\n", mlimiter_by_name("syslimiter")->current,
													mlimiter_available(mlimiter_by_name("syslimiter")), mlimiter_by_name("syslimiter")->max);
//        IL_SDH_ERR("%s: out of memory(data)\n", __func__);
	    ASSERT(il_buffer != NULL);
	}    
    else
    {
//        if(g_nILResolution == 600)
        if(g_nILResolution > 300 || i_IsCopy == 1)
            memcpy(il_buffer, data, plane->DataLength);
        else if(g_nILResolution <= 300)
        {
            int i = 0;
            int buffer_index = 0;
            char bit_buffer_src1 = 0;
            char bit_buffer_src2 = 0;
            char bit_buffer_src3 = 0;
            char bit_buffer_src4 = 0;
            char bit_buffer = 0;
            char bit_buffer1 = 0;
            char bit_buffer2 = 0;
#if 0
            IL_SDH_ERR("Length : %d, Height : %d, Width : %d, Bpp: %d\n", plane->DataLength, plane->image_info.image_height_in_lines, plane->image_info.image_width_in_pixels, plane->image_info.image_bpp);
#endif

            if (plane->image_info.image_bpp == 2)
            {
                for(i = 0; i < plane->DataLength; i=i+2)
                {
                    if((i/(plane->image_info.image_width_in_pixels/(8/plane->image_info.image_bpp))%2) == 0)  // plane->image_info.image_width_in_pixels/8 : data/bit -> data/byte
                    {
                        bit_buffer = 0;
                        bit_buffer_src1 = *(char*)(data+i);
                        bit_buffer_src2 = *(char*)(data+i+1);

                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x80); // 10000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x40); // 01000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x20); // 00100000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x10); // 00010000

                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x01); // 00000001
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x02); // 00000010
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x04); // 00000100
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x08); // 00001000

                        memcpy(il_buffer+buffer_index, &bit_buffer, 1);
                        buffer_index++;
                    }
                }
            }
            else if(plane->image_info.image_bpp == 999) // 2bpp image needs widthx2
            {
                for(i = 0; i < plane->DataLength; i=i+4)
                {
                    //				if((i/(plane->image_info.image_width_in_pixels/8)%2) == 0)  // plane->image_info.image_width_in_pixels/8 : data/bit -> data/byte
                    if((i/(plane->image_info.image_width_in_pixels/(8/plane->image_info.image_bpp))%2) == 0)  // plane->image_info.image_width_in_pixels/8 : data/bit -> data/byte
                    {
                        bit_buffer = 0;
                        bit_buffer1 = 0;
                        bit_buffer2 = 0;
                        bit_buffer_src1 = *(char*)(data+i);
                        bit_buffer_src2 = *(char*)(data+i+1);
                        bit_buffer_src3 = *(char*)(data+i+2);
                        bit_buffer_src4 = *(char*)(data+i+3);

                        bit_buffer1 = bit_buffer1 | (bit_buffer_src1 & 0x80); // 10000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src1 & 0x40); // 01000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src1 & 0x20); // 00100000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src1 & 0x10); // 00010000

                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src2 & 0x01); // 00000001
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src2 & 0x02); // 00000010
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src2 & 0x04); // 00000100
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer1 = bit_buffer1 | (bit_buffer_src2 & 0x08); // 00001000

                        bit_buffer2 = bit_buffer2 | (bit_buffer_src3 & 0x80); // 10000000
                        bit_buffer_src3 = bit_buffer_src3 << 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src3 & 0x40); // 01000000
                        bit_buffer_src3 = bit_buffer_src3 << 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src3 & 0x20); // 00100000
                        bit_buffer_src3 = bit_buffer_src3 << 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src3 & 0x10); // 00010000

                        bit_buffer_src4 = bit_buffer_src4 >> 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src4 & 0x01); // 00000001
                        bit_buffer_src4 = bit_buffer_src4 >> 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src4 & 0x02); // 00000010
                        bit_buffer_src4 = bit_buffer_src4 >> 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src4 & 0x04); // 00000100
                        bit_buffer_src4 = bit_buffer_src4 >> 1;
                        bit_buffer2 = bit_buffer2 | (bit_buffer_src4 & 0x08); // 00001000

                        bit_buffer = bit_buffer | (bit_buffer1 & 0x80); // 10000000
                        bit_buffer1 = bit_buffer1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer1 & 0x40); // 01000000
                        bit_buffer1 = bit_buffer1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer1 & 0x20); // 00100000
                        bit_buffer1 = bit_buffer1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer1 & 0x10); // 00010000

                        bit_buffer2 = bit_buffer2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer2 & 0x01); // 00000001
                        bit_buffer2 = bit_buffer2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer2 & 0x02); // 00000010
                        bit_buffer2 = bit_buffer2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer2 & 0x04); // 00000100
                        bit_buffer2 = bit_buffer2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer2 & 0x08); // 00001000

                        memcpy(il_buffer+buffer_index, &bit_buffer, 1);
                        buffer_index++;
                    }
                }
            }
            else
            {
                for(i = 0; i < plane->DataLength; i=i+2)
                {
                    if((i/(plane->image_info.image_width_in_pixels/8)%2) == 0)  // plane->image_info.image_width_in_pixels/8 : data/bit -> data/byte
                    {
                        bit_buffer = 0;
                        bit_buffer_src1 = *(char*)(data+i);
                        bit_buffer_src2 = *(char*)(data+i+1);

                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x80); // 10000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x40); // 01000000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x20); // 00100000
                        bit_buffer_src1 = bit_buffer_src1 << 1;
                        bit_buffer = bit_buffer | (bit_buffer_src1 & 0x10); // 00010000

                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x01); // 00000001
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x02); // 00000010
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x04); // 00000100
                        bit_buffer_src2 = bit_buffer_src2 >> 1;
                        bit_buffer = bit_buffer | (bit_buffer_src2 & 0x08); // 00001000

                        memcpy(il_buffer+buffer_index, &bit_buffer, 1);
                        buffer_index++;
                    }
                }
            }
        }
    }

//    if(g_nILResolution == 600)
    if(g_nILResolution > 300 || i_IsCopy == 1)
    {
	    il_list->width = plane->image_info.image_width_in_pixels;
	    il_list->height = plane->image_info.image_height_in_lines;
	    il_list->bpp = plane->image_info.image_bpp;
	    il_list->dpi = DPI_600;
    }
    else if(g_nILResolution <= 300)
    {
	    /*if(plane->image_info.image_bpp == 2) // 2bpp image needs widthx2
	    {
		    il_list->width = plane->image_info.image_width_in_pixels/2;
		    il_list->height = plane->image_info.image_height_in_lines/2;
	    }
	    else
	    {
		    il_list->width = plane->image_info.image_width_in_pixels/2;
		    il_list->height = plane->image_info.image_height_in_lines/2;
	    }*/
	    il_list->width = plane->image_info.image_width_in_pixels/2;
	    il_list->height = plane->image_info.image_height_in_lines/2;
	    il_list->bpp = plane->image_info.image_bpp;
	    il_list->dpi = DPI_300;
    }
	
    il_list->data = il_buffer;
	if(plane->LastData)
		il_list->isEndPlane = true;

//	IL_SDH_ERR("IL: %p data size : (%d) %d end %d\n", il_buffer, plane_count, plane->DataLength, il_list->isEndPlane);

	pList = &planeList;
    MInsertTailList(pList, &il_list->node);
//	IL_SDH_ERR("IL: ListNum %d\n", MNumListNodes(pList));
//	IL_SDH_ERR("IL: planeList %p\n", pList);

//	IL_SDH_ERR("IL: Next %p\n", MListNext(pList, &il_list->node));

//	plane_count++;
	dma_buffer_unmmap_forcpu( output_buffer );
//	plane->UseCount--;

	return;
}

HPDF_STATUS IL_PDF_NewDoc()
{
	HPDF_Error_Handler error_handler = NULL;

	pdf = HPDF_New(error_handler, NULL);
	if(!pdf){
		IL_SDH_ERR("error: cannot create pdf handle");
		return -1;
	}
	HPDF_SetCompressionMode(pdf, HPDF_COMP_IMAGE);

	return HPDF_OK;
}

HPDF_STATUS IL_PDF_NewPage(int width, int height, HPDF_REAL convert_dpi)
{
	/* Add a new page object. */
	pdfpage = HPDF_AddPage( pdf );
    ASSERT(pdfpage != NULL);
//	IL_SDH_DEBUG("PDF Page size %d, %d\n", width, height);

	HPDF_Page_SetWidth(pdfpage, width/convert_dpi);    // original is 600 dpi
	HPDF_Page_SetHeight(pdfpage, height/convert_dpi);  // original is 600 dpi

	page_height = height/convert_dpi;                  // original is 600 dpi

	return HPDF_OK;
}

HPDF_STATUS IL_PDF_DrawImage(HPDF_BYTE* rawdata, int width, int height, HPDF_REAL offset, int bpp, HPDF_REAL convert_dpi)
{
	HPDF_Image image = NULL;
	HPDF_REAL pdf_width = width/convert_dpi;
	HPDF_REAL pdf_height = height/convert_dpi;

	// Load the raw data
    if (rawdata == NULL){
        IL_SDH_ERR("IL_PDF_DrawImage : No raw data\n");
    }
    else {
	    image = HPDF_LoadRawImageFromMem(pdf, rawdata, width*bpp, height, HPDF_CS_DEVICE_GRAY, 1, FALSE);
    }

	if(image == NULL)
	{
		IL_SDH_ERR("IL_PDF_DrawImage : No raw image data\n");
	}

	// Draw Image into PDF File
    if(HPDF_OK == HPDF_Page_DrawImage(pdfpage, image, 0, offset, pdf_width, pdf_height))
	{
		IL_SDH_DEBUG("HPDF_Page_DrawImage[%f] : %d, %d, %f, %f\n", offset, width, height, pdf_width, pdf_height);
	}
	else
	{
		IL_SDH_ERR("HPDF_Page_DrawImage : Write fail\n");
	}
	return HPDF_OK;
}

#define IMAGELOG_FILENAME_LENGTH 300

HPDF_STATUS IL_PDF_SaveDoc(int page_number)
{
	char fileName[IMAGELOG_FILENAME_LENGTH];
	memset(fileName, 0, sizeof(fileName));
	snprintf(fileName, IMAGELOG_FILENAME_LENGTH, "%s/%s/%s_%d.pdf", IMAGE_LOG_PATH, list_return_pdffilename, list_return_pdffilename, page_number);
//    snprintf(fileName, IMAGELOG_FILENAME_LENGTH, "%s/%s/%s_%d.pdf", IMAGE_LOG_PATH, il_filename, il_filename, page_number);
	HPDF_SaveToFile(pdf, fileName);
	IL_SDH_ERR("PDF SAVE [%s]\n", fileName);
	HPDF_Free(pdf);
//    IL_SDH_ERR("PDF END [%s]\n", fileName);
    IL_SDH_ERR("PDF END & ENCRYPT START\n");
    ARIA_encode(fileName, 0);
    IL_SDH_ERR("ENCRYPT END [%s]\n", fileName);
//	HPDF_Free(pdf);
	return HPDF_OK;
}

void ImageLogMonitorNotify(int notifyType)
{
	MESSAGE msgTosend;
	error_type_t err;
	bool msgSendFlag = true;
	msgTosend.param1 = 0;
	msgTosend.param2 = 0;
	msgTosend.param3 = NULL;

	switch(notifyType){
	case IL_NOTIFY_SYS_INIT:
		msgTosend.msgType = MSG_IMAGE_LOG_NOTIFY;
		msgTosend.param1 = IL_NOTIFY_SYS_INIT;
		break;
	case IL_NOTIFY_JOB_END:
		msgTosend.msgType = MSG_IMAGE_LOG_NOTIFY;
		msgTosend.param1 = IL_NOTIFY_JOB_END;
		break;
	default:
		msgSendFlag = false;
		break;
	}

	if(msgSendFlag){
		err = SYMsgSend(IMAGE_LOG_MONITOR_ID, &msgTosend);
		XASSERT( err==OK, err );
	}
}

#define META_BUFFER_SIZE 256
#define METAINFO_BUFFER_SIZE 30
int set_metafile_start_info(char _jobtype, int _subJobType, int _jobid, char* _username)
{
	IL_SDH_DEBUG("[%s] JobType:%c subJobType:%d ID:%d \n", __FUNCTION__, _jobtype, _subJobType, _jobid);
	if(g_nSaveImageLog == 0){
		IL_SDH_ERR("[%s] Save ImageLog is disabled:%d\n", __FUNCTION__, g_nSaveImageLog);
		return -1;
	}
    
	if(IsImageLogEnabled(_jobtype, _subJobType) == FALSE){
		IL_SDH_ERR("[%s] Do not SaveImageLog, type:%c subJobType:%d\n", __FUNCTION__, _jobtype, _subJobType);
		i_FlagPrintLog = 0;   
        return -2;
	}
    else
        i_FlagPrintLog = 1;
	i_IsCopy = 0;
    /*
    switch(_jobtype)
	{
		case 'C':
		{
			i_IsCopy = 1;
			break;
		}
		default:
		{
			i_IsCopy = 0;
			break;
		}
	}
	*/

	int ret_value = 0;
    int i = 0;
	char buffer[META_BUFFER_SIZE];
	char* pMachineSerial = NULL;
	char machineSerial[METAINFO_BUFFER_SIZE];
	error_type_t db_retval = FAIL;
	char timestamp[METAINFO_BUFFER_SIZE];
	memset(buffer, 0, META_BUFFER_SIZE);
	memset(machineSerial, 0, METAINFO_BUFFER_SIZE);
	memset(timestamp, 0, METAINFO_BUFFER_SIZE);

	// Get serial number
	pMachineSerial = db_get_var(INSTANCE_MACHINE, "MachineSerialNo", &db_retval);
	if((pMachineSerial == NULL) || (db_retval == FAIL)){
		snprintf(machineSerial, METAINFO_BUFFER_SIZE, "NOSERIAL");
		IL_SDH_ERR("No serial number\n");
	}
	else{
		snprintf(machineSerial, METAINFO_BUFFER_SIZE, "%s", pMachineSerial);
		IL_SDH_ERR("Serial No. : %s\n", machineSerial);
		MEM_FREE_AND_NULL(pMachineSerial);
	}

	ret_value = get_current_timestamp(timestamp);
	if(ret_value != 0){
		return -1;
	}

	// Make meta info.
    if (_jobid == 0)
        snprintf(buffer, META_BUFFER_SIZE, "xml_file_name=\"%c_%s_%s.log\"\nPrinterSerialNumber=\"%s\"\nUserName=\"%s\"", _jobtype, machineSerial, timestamp, machineSerial, _username);
    else
	    snprintf(buffer, META_BUFFER_SIZE, "xml_file_name=\"%c_%s_%s.log\"\nPrinterSerialNumber=\"%s\"\nJobID=\"%d\"\nUserName=\"%s\"", _jobtype, machineSerial, timestamp, machineSerial, _jobid, _username);
	IL_SDH_ERR("START_META_INFO: %s", buffer);
    for(i=0; i<META_BUFFER_SIZE; i++){
        if(buffer[i] == 0x00) buffer[i] = '\n';
    }
	make_newjob_info(buffer, _jobtype, machineSerial, timestamp, _jobid);

	return 0;
}

int get_PrintImageLogEnabled(void)
{
    if(g_nIsEnableImageLog && g_nSaveImageLog){
    	if(i_FlagPrintLog == 1)
            return 1;
        else
            return 0; 
    }
    else
        return 0;
}

int get_current_timestamp(char* timestamp)
{
	struct tm *tm;
	struct timeval tv;

	// Get timestamp info.
	gettimeofday( &tv, NULL );
	tm = localtime( &tv.tv_sec );
	if(tm != NULL){
		snprintf(timestamp, METAINFO_BUFFER_SIZE, "%02d%02d%02d%02d%02d%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
		IL_SDH_ERR("Timestamp: %s\n", timestamp);
	}
	else{
		IL_SDH_ERR("Cannot create the file name. tm is NULL!!!");
		return 1;
	}

	return 0;
}

int make_newjob_info(char* buffer, char _jobtype, char* machineSerial, char* timestamp, int _jobid)
{
	FILE *mf = NULL;
    int nResult = 0;
	char logPath[LOG_PATH_LENGTH];
	char path_buffer[LOG_PATH_LENGTH];
	char cmd_line[LOG_PATH_LENGTH*2];

	// make new job filename
	memset(il_filename, 0, LOG_PATH_LENGTH);
	snprintf(il_filename, LOG_PATH_LENGTH, "%c_%s_%s", _jobtype, machineSerial, timestamp);
	IL_SDH_ERR("FILENAME : %s\n", il_filename);

	switch(_jobtype)
	{
		case 'C':
		{
			memset(il_c_filename, 0, LOG_PATH_LENGTH);
			snprintf(il_c_filename, LOG_PATH_LENGTH, "%c_%s_%s", _jobtype, machineSerial, timestamp);
            save_ILfilename(_jobid, il_c_filename);
			break;
		}
		case 'S':
		{
			memset(il_s_filename, 0, LOG_PATH_LENGTH);
			snprintf(il_s_filename, LOG_PATH_LENGTH, "%c_%s_%s", _jobtype, machineSerial, timestamp);
            save_ILfilename(_jobid, il_s_filename);
			break;
		}
		case 'P':
		{
			memset(il_p_filename, 0, LOG_PATH_LENGTH);
			snprintf(il_p_filename, LOG_PATH_LENGTH, "%c_%s_%s", _jobtype, machineSerial, timestamp);
           //save_ILfilename(JOBID, il_p_filename);
            save_ILfilename(_jobid, il_p_filename);
			break;
		}
		case 'F':
		{
			memset(il_f_filename, 0, LOG_PATH_LENGTH);
			snprintf(il_f_filename, LOG_PATH_LENGTH, "%c_%s_%s", _jobtype, machineSerial, timestamp);
            save_ILfilename(_jobid, il_f_filename);
			break;
		}
		default:
		{
			IL_SDH_ERR("%s: Wrong Jobtype [%c]", __FUNCTION__, _jobtype);
			break;
		}
	}

	// Make new job directory
	memset(logPath, 0, LOG_PATH_LENGTH);
	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_filename);
	nResult = mkdir(logPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (nResult == -1 ) {
        IL_SDH_ERR("Folder make fail\n");
    }

	memset(path_buffer, 0, LOG_PATH_LENGTH);
	snprintf(path_buffer, LOG_PATH_LENGTH, "%s/%s", logPath, META_FILENAME);

	// Write meta info. in file
	mf = fopen(path_buffer, "w");
	if(mf == NULL){
		IL_SDH_ERR("File open fail\n");
		return 1;
	}
	else{
		fwrite(buffer, 1, strlen(buffer), mf);
	}
	fclose(mf);
    ARIA_encode(path_buffer, 0);

	memset(cmd_line, 0, LOG_PATH_LENGTH*2);
	snprintf(cmd_line, LOG_PATH_LENGTH*2, "md5sum %s/%s > %s/%s.md5sum", logPath, META_FILENAME, logPath, META_FILENAME);
	system(cmd_line);

	return 0;
}

char* get_il_filename(int _jobid)
{
	char* ilf = NULL;
    if(get_ILfilename(_jobid, 0) == 0){ //revomeflag on (1)
        ilf = list_return_pdffilename;
        IL_SDH_ERR("MSG_PAGEEND: %d [%s]\n",_jobid, list_return_pdffilename);
    }
    else
        IL_SDH_ERR("%s: Wrong jobid [%d]\n", __FUNCTION__, _jobid);

	IL_SDH_DEBUG("JobID:%d IL_Filename:%s", _jobid, ilf);
	return ilf;
}


static int get_ILfilename(int _jobid, int RemoveFlag) // Remove Flag 1: remove
{
	int foundFileName = -1;
	if(_jobid != 0){
		IL_FILENAME *TempResource = NULL;
		ATLISTENTRY* node;
		node = ATListHead(&il_filename_list);
		while (node != NULL)
		{
			TempResource = CONTAINING_RECORD(node, IL_FILENAME, ListNode);
			if(TempResource != NULL)
			{
				if(TempResource->_jobid == _jobid) {
                    if(RemoveFlag == 1) {
                        memset(list_return_filename, 0x00, LOG_PATH_LENGTH);
                        strncpy(list_return_filename, TempResource->list_il_filename, LOG_PATH_LENGTH-1);
    					ATRemoveEntryList(&TempResource->ListNode);
                        MEM_FREE_AND_NULL(TempResource);
                    }
                    else {
                        memset(list_return_pdffilename, 0x00, LOG_PATH_LENGTH);
                        strncpy(list_return_pdffilename, TempResource->list_il_filename, LOG_PATH_LENGTH-1);
                    }
					foundFileName = 0;
					break;
				}
			}
			node = ATListNext(&il_filename_list, node);
		}
	}
    return foundFileName;
}

static void save_ILfilename(int _jobid, char *filename){
    IL_FILENAME *TempResource;
    TempResource = (IL_FILENAME*)MEM_CALLOC(1, sizeof(IL_FILENAME) + 10 );
    ATLISTENTRY* node;
    node = ATListHead(&il_filename_list);
    TempResource->_jobid = _jobid;
    memset(TempResource->list_il_filename, 0x00, LOG_PATH_LENGTH);
    strncpy(TempResource->list_il_filename, filename, LOG_PATH_LENGTH-1);
    ATInsertTailList(&il_filename_list, &TempResource->ListNode);
}

void save_fax_jobinfo(int _jobid, int _faxpage, int _faxresult, char *faxnum){
    FAX_BROADCAST *TempResource;
    TempResource = (FAX_BROADCAST*)MEM_CALLOC(1, sizeof(FAX_BROADCAST) + 10 );
    ATLISTENTRY* node;
    node = ATListHead(&il_fax_broadcast_list);
    TempResource->_jobid = _jobid;
    TempResource->fax_page = _faxpage;
    TempResource->fax_result = _faxresult;
    memset(TempResource->fax_num, 0x00, FAX_NUM_LENGTH);
    strncpy(TempResource->fax_num, faxnum, FAX_NUM_LENGTH-1);
    ATInsertTailList(&il_fax_broadcast_list, &TempResource->ListNode);

    IL_SDH_ERR("jobid: %d, page: %d, result: %d, num: %s \n"
				,TempResource->_jobid, TempResource->fax_page, TempResource->fax_result, TempResource->fax_num);
}

#ifdef META_END_WITH_JOBID
int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid)
#else
int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages, int _jobid)
//int set_metafile_end_info(char _jobtype, int _subJobType, char* _destination, int _copy, char* _filename, int _originalpages)
#endif
{
	if(_filename){
		IL_SDH_DEBUG("[%s] JobType:%c subJobType:%d Copies:%d Pages:%d, FileName:%s\n",
				__FUNCTION__, _jobtype, _subJobType, _copy, _originalpages, _filename);
	}
	else{
		IL_SDH_DEBUG("[%s] JobType:%c%d Copies:%d Pages:%d\n", __FUNCTION__, _jobtype, _subJobType, _copy, _originalpages);
	}

	if(g_nSaveImageLog == 0){
		IL_SDH_ERR("[%s] Save ImageLog is disabled:%d\n", __FUNCTION__, g_nSaveImageLog);
		return -1;
	}
    
	if(IsImageLogEnabled(_jobtype, _subJobType) == FALSE){
		IL_SDH_ERR("[%s] Do not SaveImageLog, type:%c subJobType:%d\n", __FUNCTION__, _jobtype, _subJobType);
		i_FlagPrintLog = 0;
        return -2;
	}
    else
        i_FlagPrintLog = 1;

	char buffer[META_BUFFER_SIZE];
#if 0
	char endtime[METAINFO_BUFFER_SIZE];
	struct tm *tm;
	struct timeval tv;
	time_t current_time = 0;
	memset(buffer, 0, META_BUFFER_SIZE);
	memset(endtime, 0, METAINFO_BUFFER_SIZE);

	// Get timestamp info.
	gettimeofday( &tv, NULL );
	tm = localtime( &tv.tv_sec );
	if(tm != NULL){
		IL_SDH_ERR("Timestamp : %d\n", current_time);
		current_time = mktime(tm);
	}
	else{
		IL_SDH_ERR("Cannot get the time. tm is NULL!!!");
		return -2;
	}

    // Make meta info.
	if(_jobtype == 'C' || _jobtype == 'P'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nFileName=\"%s\"\nFinishTime=\"%ld\"\nOriginalPages=\"%d\"", _copy, _filename, current_time, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else if(_jobtype == 'S' || _jobtype == 'F'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nDestination=\"%s\"\nFinishTime=\"%ld\"\nOriginalPages=\"%d\"", _copy, _destination, current_time, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else{
		IL_SDH_ERR("Unknown Job type %c\n", _jobtype);
	}

#else
    int ret_value = 0;
	char endtime[METAINFO_BUFFER_SIZE];
	memset(buffer, 0, META_BUFFER_SIZE);
	memset(endtime, 0, METAINFO_BUFFER_SIZE);
    ret_value = get_current_timestamp(endtime);
    if(ret_value != 0){
        return -1;
    }

	// Make meta info.
#ifdef META_END_WITH_JOBID
    if(_jobid != 0) {
	  if(_jobtype == 'C' || _jobtype == 'P'){
		snprintf(buffer, META_BUFFER_SIZE, "\nJobID=\"%d\"\nCopies=\"%d\"\nFileName=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _jobid, _copy, _filename, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else if(_jobtype == 'S' || _jobtype == 'F'){
		snprintf(buffer, META_BUFFER_SIZE, "\nJobID=\"%d\"\nCopies=\"%d\"\nDestination=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _jobid, _copy, _destination, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else{
		IL_SDH_ERR("Unknown Job type %c\n", _jobtype);
	}

  }
  else {
	if(_jobtype == 'C' || _jobtype == 'P'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nFileName=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _copy, _filename, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else if(_jobtype == 'S' || _jobtype == 'F'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nDestination=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _copy, _destination, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else{
		IL_SDH_ERR("Unknown Job type %c\n", _jobtype);
	}
  }
#else
	if(_jobtype == 'C' || _jobtype == 'P'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nFileName=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _copy, _filename, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else if(_jobtype == 'S' || _jobtype == 'F'){
		snprintf(buffer, META_BUFFER_SIZE, "\nCopies=\"%d\"\nDestination=\"%s\"\nFinishTime=\"%s\"\nOriginalPages=\"%d\"", _copy, _destination, endtime, _originalpages);
		IL_SDH_ERR("END_META_INFO: %s", buffer);
	}
	else{
		IL_SDH_ERR("Unknown Job type %c\n", _jobtype);
	}
#endif
#endif
	//make_endjob_info(_jobtype, buffer, JOBID);
	if(make_endjob_info(_jobtype, buffer, _jobid) == 0){
		//Notify to ImageLogMonitor
		ImageLogMonitorNotify(IL_NOTIFY_JOB_END);
	}

	return 0;
}

int make_endjob_info(char _jobtype, char* buffer, int _jobid)
{
	FILE *mf = NULL;
	char logPath[LOG_PATH_LENGTH];
	char xmlPath[LOG_PATH_LENGTH*2];
	char path_buffer[LOG_PATH_LENGTH];
	char cmd_line[LOG_PATH_LENGTH*2];
    char faxnum_buffer[META_BUFFER_SIZE];

	switch(_jobtype)
	{
		case 'C':
		{
			memset(logPath, 0, LOG_PATH_LENGTH);
			memset(xmlPath, 0, LOG_PATH_LENGTH*2);
            if(get_ILfilename(_jobid, 1) == 0){ //revomeflag on (1)
            	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, list_return_filename);
				snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, list_return_filename);
            }
			//snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_c_filename);
			//snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, il_c_filename);
			break;
        }
		case 'S':
		{
			memset(logPath, 0, LOG_PATH_LENGTH);
			memset(xmlPath, 0, LOG_PATH_LENGTH*2);
            if(get_ILfilename(_jobid, 1) == 0){ //revomeflag on (1)
            	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, list_return_filename);
				snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, list_return_filename);
            }
			//snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_s_filename);
			//snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, il_s_filename);
			break;
		}
		case 'P':
		{
			memset(logPath, 0, LOG_PATH_LENGTH);
			memset(xmlPath, 0, LOG_PATH_LENGTH*2);
            if(get_ILfilename(_jobid, 1) == 0){ //revomeflag on (1)
            	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, list_return_filename);
				snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, list_return_filename);
            }
			//snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_p_filename);
			//snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, il_p_filename);
           // get_ILfilename(_jobid, 1);
           // snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, list_return_filename);
			break;
		}
		case 'F':
		{
			memset(logPath, 0, LOG_PATH_LENGTH);
			memset(xmlPath, 0, LOG_PATH_LENGTH*2);
//			snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_f_filename);
            if(get_ILfilename(_jobid, 1) == 0){ //revomeflag on (1)
            	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, list_return_filename);
				snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, list_return_filename);
            }
            else{
            	snprintf(logPath, LOG_PATH_LENGTH, "%s/%s", IMAGE_LOG_PATH, il_f_filename);
			snprintf(xmlPath, LOG_PATH_LENGTH, "%s/%s.log", logPath, il_f_filename);
            }
			break;
		}
		default:
		{
			IL_SDH_ERR("%s: Wrong Jobtype [%c]", __FUNCTION__, _jobtype);
			break;
		}
	}

	memset(path_buffer, 0, LOG_PATH_LENGTH);
	snprintf(path_buffer, LOG_PATH_LENGTH, "%s/%s", logPath, META_FILENAME);

	// Write meta info. in file
    ARIA_encode(path_buffer, 1);
	mf = fopen(path_buffer, "a");
	if(mf == NULL){
		IL_SDH_ERR("File open fail\n");
		return 1;
	}
    else{
        fwrite(buffer, 1, strlen(buffer), mf);
        if(_jobtype == 'F')
        {
            FAX_BROADCAST *TempResource = NULL;
            ATLISTENTRY* node;
            int num_count = 0;
            node = ATListHead(&il_fax_broadcast_list);
            while (node != NULL)
            {
                TempResource = CONTAINING_RECORD(node, FAX_BROADCAST, ListNode);
//                node = ATListNext(&il_filename_list, node);
                memset(faxnum_buffer, 0x00, META_BUFFER_SIZE);
                if(TempResource != NULL)
                {
                    if(TempResource->_jobid == _jobid) {
                        num_count++;
                        snprintf(faxnum_buffer, META_BUFFER_SIZE, "\nTel%d=\"%s\"\nTelRe%d=\"%d\"\nTelPage%d=\"%d\"", num_count, TempResource->fax_num, num_count, TempResource->fax_result, num_count, TempResource->fax_page);
                        fwrite(faxnum_buffer, 1, strlen(faxnum_buffer), mf);
                        IL_SDH_ERR("put FAX num: %s \n", faxnum_buffer); 
                        ATRemoveEntryList(&TempResource->ListNode);
                        IL_SDH_ERR("Free TempResource!\n");
                        MEM_FREE_AND_NULL(TempResource);
                        node = NULL;
                        
                    }
                    else { 
                        IL_SDH_ERR("job ID not match\n");
//                        usleep(1000000);  // 1s 
                        node = ATListNext(&il_fax_broadcast_list, node);
                        if (node == NULL)
                            IL_SDH_ERR("Next node is empty\n");
                    }
                }
                else
                    IL_SDH_ERR("TempResource is NULL\n");

                node = ATListNext(&il_fax_broadcast_list, node);
                node = ATListHead(&il_fax_broadcast_list);
            }
            if(num_count > 0) {
                snprintf(faxnum_buffer, META_BUFFER_SIZE, "\nCount=\"%d\"", num_count);
                fwrite(faxnum_buffer, 1, strlen(faxnum_buffer), mf);
            }
        }
    }
    fclose(mf);
    //ARIA_encode(path_buffer, 0);

	memset(cmd_line, 0, LOG_PATH_LENGTH*2);
	snprintf(cmd_line, LOG_PATH_LENGTH*2, "md5sum %s/%s > %s/%s.md5sum", logPath, META_FILENAME, logPath, META_FILENAME);
	system(cmd_line);

	memset(cmd_line, 0, LOG_PATH_LENGTH*2);
	snprintf(cmd_line, LOG_PATH_LENGTH*2, "/app/image_log_daemon.sh %s", logPath);
	system(cmd_line);

	memset(cmd_line, 0, LOG_PATH_LENGTH*2);
	snprintf(cmd_line, LOG_PATH_LENGTH*2, "%s/%s", logPath, META_FILENAME);
	IL_SDH_ERR("for encode %s, %s \n", cmd_line, xmlPath);

    //ARIA_encode(cmd_line, 0);
    //ARIA_encode(xmlPath, 0);

	return 0;
}

bool IsImageLogEnabled(char jobtype, int subJobType)
{
	bool retVal = FALSE;
	if(g_nIsEnableImageLog && g_nSaveImageLog){
		switch(jobtype){
			case 'C':
				if(g_nIsCopyImgLogEnable)
					retVal = TRUE;
				break;
			case 'S':
				if(g_nIsScanImgLogEnable)
					retVal = TRUE;
				break;
			case 'P':
				if(g_nIsPrintImgLogEnable)
					retVal = TRUE;
				break;
			case 'F':
				//subJobType 1: Fax Transfer 2: Fax Receive
				if(subJobType == IMAGELOG_FAX_SUBJOB_TYPE_TX){
					if(g_nIsFaxTxImgLogEnable)
						retVal = TRUE;
				}
				else if(subJobType == IMAGELOG_FAX_SUBJOB_TYPE_RX){
					if(g_nIsFaxRxImgLogEnable)
						retVal = TRUE;
				}
				break;
			default:
				IL_SDH_ERR("%s: Wrong jobtype:%c\n", __FUNCTION__, jobtype);
				break;
		}
	}
	else{
		retVal = FALSE;
	}
	return retVal;
}

void ReadDBConfig()
{
	int getIntVal = 0;
	error_type_t db_retval = FAIL;

	getIntVal = db_get_var_int("CONF_IMAGELOG", "IsEnabled", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:IsEnabled\n", __FUNCTION__);
		getIntVal = 0; //Disable
	}
	g_nIsEnableImageLog = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "FileTransfer", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:FileTransfer\n", __FUNCTION__);
		getIntVal = 0; //FileTransfer:OFF
	}
	g_nILFileTransfer = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "Resolution", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:Resolution\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILResolution = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "Print", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:Print\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsPrintImgLogEnable = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "Copy", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:Copy\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsCopyImgLogEnable = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "Scan", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:Scan\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsScanImgLogEnable = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "FaxTransfer", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:FaxTransfer\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsFaxTxImgLogEnable = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "FaxReception", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:FaxReception\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsFaxRxImgLogEnable = getIntVal;

	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "SDCardFullSCDisplay", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:SDCardFullSCDisplay\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nILSDCardFullSCDisplay = getIntVal;

/*
	getIntVal = 0;
	getIntVal = db_get_var_int("CONF_IMAGELOG", "AriaEncrypt", &db_retval);
	if(db_retval == FAIL){
		IL_SDH_ERR("%s: Fail to get DB data:CONF_IMAGELOG:AriaEnable\n", __FUNCTION__);
		getIntVal = 0;
	}
	g_nIsEnableAria = getIntVal;
*/
	IL_SDH_DEBUG("IsEnableImageLog:%d FileTransfer:%d Resolution:%d Print:%d Copy:%d Scan:%d Fax Tx:%d Rx:%d\n",
			g_nIsEnableImageLog, g_nILFileTransfer, g_nILResolution, g_nIsPrintImgLogEnable, g_nIsCopyImgLogEnable,
			g_nIsScanImgLogEnable, g_nIsFaxTxImgLogEnable, g_nIsFaxRxImgLogEnable);
}

void SetInitData()
{
	ReadDBConfig();

	//Commented below line Just for test, For release un-comment below line
#ifdef IMAGE_LOG_TEST_FOLDER
//	g_nIsEnableImageLog = 1;
#endif
	g_nSaveImageLog = 0; //First set to 0. After checking SD card status set to 1
	if(g_nIsEnableImageLog){
		if(GetSDCardStatus() == 0){
			g_nSaveImageLog = 1;
			if(GetFileSystemMemUsage(IMAGE_LOG_PATH) < 0){
				IL_SDH_ERR("[%s] Error: SD Card is full \n", __FUNCTION__);
				if(g_nILSDCardFullSCDisplay){
					SendSCStatusToUI(SC_IMAGELOG_SDCARD_FULL, SYSTEM_MODULE_SCID);
					g_nSaveImageLog = 0;
				}
				else{
					if(DeleteImageLog(0) != 0){
						g_nSaveImageLog = 0;
					}
				}
			}
		}
		else{
			g_nSaveImageLog = 0;
		}
	}
}

int GetSDCardStatus()
{
	int retVal = 0;
#ifdef IMAGE_LOG_TEST_FOLDER
	if(access("/tmp/", F_OK)){
#else
	//if(access("/dev/mmcblk0p1", F_OK)){
	if(access(IMAGE_LOG_SD_MOUNT_PATH, F_OK)){
#endif
		IL_SDH_ERR("[ERROR] can't find SD Card\n");
		SendSCStatusToUI(SC_IMAGELOG_SDCARD_NOT_DETECTED, SYSTEM_MODULE_SCID);
		SetImageLogStatus(ILS_SDCARD_NOT_DETECTED);
		retVal = -1;
	}
	else{
		int ret = GetFileSystemType(IMAGE_LOG_SD_MOUNT_PATH);
		if(ret == 0){
			if(access(IMAGE_LOG_PATH, F_OK)){
				IL_SDH_ERR("[ERROR] ImageLog folder NG\n");
				if(mkdir(IMAGE_LOG_PATH, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0){
					SendSCStatusToUI(SC_IMAGELOG_SDCARD_WRITE_ERR, SYSTEM_MODULE_SCID);
					SetImageLogStatus(ILS_SDCARD_WRITE_ERR);
					retVal  = -2;
				}
			}
			else{
				IL_SDH_ERR("ImageLog folder OK\n");
			}
		}
		else{
			SendSCStatusToUI(SC_IMAGELOG_SDCARD_INVALID_FILE_SYSTEM, SYSTEM_MODULE_SCID);
			SetImageLogStatus(ILS_SDCARD_INVALID_FILE_SYSTEM);
			retVal  = -3;
		}
	}

	if(retVal == 0){
		g_nImageLogSDCardError = 0;
	}
	else{
		g_nImageLogSDCardError = 1;
	}
	return retVal;
}

int CheckImgLogMemDoCancelAct(char jobtype, int subJobType)
{
	if(GetFileSystemMemUsage(IMAGE_LOG_PATH) < 0){
		if(g_nILSDCardFullSCDisplay){
			error_type_t err;
			MESSAGE msg;
			RESOURCES resourceType;

			switch(jobtype){
				case 'C':
					resourceType = e_CopyApp;
					break;
				case 'S':
					resourceType = e_Scanner;
					break;
				case 'P':
					resourceType = e_Printer;
					break;
				case 'F':
					//subJobType 1: Fax Transfer 2: Fax Receive
					if(subJobType == 1){
						resourceType = e_ScanToFax;
					}
					else if(subJobType == 2){
						resourceType = e_FaxPrintApp;
					}
					break;
				default:
					IL_SDH_ERR("%s: Wrong jobtype:%c subJobType:%d\n", __FUNCTION__, jobtype, subJobType);
					resourceType = e_NoResource;
					break;
			}

			//Start Job Canceling. Send CANCELJOB msg to SJM.
			msg.msgType = MSG_CANCELJOB;
			msg.param1 = SYS_REQUEST;	//SYS_REQUEST or SYS_REQUSER;
			msg.param2 = 0;
			msg.param3 = (void*) resourceType;
			err = SYMsgSendNoWait( SJMID, &msg );
			XASSERT( err==OK, err );

			IL_SDH_ERR("[%s] Error: SD Card is full: Canceling Job Resource:%d from SJM\n", __FUNCTION__, resourceType);
			SendSCStatusToUI(SC_IMAGELOG_SDCARD_FULL, SYSTEM_MODULE_SCID);
		}
		else{
			if(DeleteImageLog(0) != 0){
				g_nSaveImageLog = 0;
			}
		}
		return 1;
	}
	else{
		return 0;
	}
}

static void QuickSortDateModified(char* path, struct dirent **nameList, int left, int right)
{
	register int i, j;

	struct dirent *temp;
	struct stat ibuf;
	struct stat jbuf;
	struct stat xbuf;

	i = left; j = right;
	//IL_SDH_ERR("[%s] l:%d r:%d\n", __FUNCTION__, left, right);
	//Get date modified info
	snprintf(path, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, nameList[(left+right)/2]->d_name);
	stat(path, &xbuf);

	do {
		//Set name
		snprintf(path, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, nameList[i]->d_name);
		stat(path, &ibuf);
		snprintf(path, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, nameList[j]->d_name);
		stat(path, &jbuf);
		while((ibuf.st_mtime > xbuf.st_mtime) && (i < right)) {
			i++;
			snprintf(path, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, nameList[i]->d_name);
			stat(path, &ibuf);
		}
		while((jbuf.st_mtime < xbuf.st_mtime) && (j > left))  {
			j--;
			snprintf(path, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, nameList[j]->d_name);
			stat(path, &jbuf);
		}
		if(i <= j) {
			temp = nameList[i];
			nameList[i] = nameList[j];
			nameList[j] = temp;

			i++;
			j--;
		}
		//IL_SDH_ERR("8: while i:%d j:%d\n", __FUNCTION__, i, j);
	} while(i <= j);

	if(left < j){
		QuickSortDateModified(path, nameList, left, j);
	}
	if(i < right){
		QuickSortDateModified(path, nameList, i, right);
	}
}

static void QuickSortMe(char* path, struct dirent **nameList, int listCount)
{
	QuickSortDateModified(path, nameList, 0, listCount-1);
}

/**
 * \brief Delete Image Log
 *
 * \param option 0: Delete IMAGE_LOG_MIN_MEMSIZE*5 MB old data 1: Delete all
 *
 */
int DeleteImageLog(int option)
{
	IL_SDH_ERR("[%s] starting... opt:%d\n", __FUNCTION__, option);
	int deleteFlag = true;
	int remainingMemInMB;
	int ret = 0;
	int sResult = 0;
	char *pImageLogDirPath;
	pImageLogDirPath = (char*)MEM_MALLOC(IMAGE_LOG_PATH_MAX);
	ASSERT(pImageLogDirPath != NULL);
	memset(pImageLogDirPath, 0x00, IMAGE_LOG_PATH_MAX);

	if(option == 0) { //Delete IMAGE_LOG_MIN_MEMSIZE*5 MB old data
		struct dirent **namelist;
		struct stat buf;
		int n;
		//Scan directory
		n = scandir(IMAGE_LOG_PATH, &namelist, NULL, alphasort);
		IL_SDH_ERR("Total entries: %d\n", n);
		if (n < 0){
			IL_SDH_ERR("[%s] scandir failed\n", __FUNCTION__);
		}
		else {

			if (n > 10){
				//Sort by last modified date
				QuickSortMe(pImageLogDirPath, namelist, n);
			}

			while (n--) {
				if(n > 1){
					//Set full path before stat
					snprintf(pImageLogDirPath, IMAGE_LOG_PATH_MAX, "%s/%s", IMAGE_LOG_PATH, namelist[n]->d_name);
					sResult = stat(pImageLogDirPath, &buf); //sResult = stat(namelist[n]->d_name, &buf);
					//IL_SDH_ERR("%s\n",pImageLogDirPath);
					if (sResult < 0){
						IL_SDH_ERR("Stat fail: %d\n", sResult);
					}
					IL_SDH_DEBUG("%-35s  %10ld \t %s \n", namelist[n]->d_name, buf.st_mtime, ctime(&buf.st_mtime));
					if (!strcmp(namelist[n]->d_name, ".") || !strcmp(namelist[n]->d_name, "..")){
						//Nothing to do
					}
					else{
						//Delete directory and check remaining memory
						if(deleteFlag){
							size_t logPathLength = strlen(namelist[n]->d_name) + strlen(IMAGE_LOG_PATH) + 2;
							if (logPathLength < IMAGE_LOG_PATH_MAX) {
								ImageLogDeleteFolder(pImageLogDirPath);
								remainingMemInMB = GetFileSystemMemUsage(IMAGE_LOG_PATH);
								if(remainingMemInMB >= IMAGE_LOG_DEL_OLD_JOB_MEMSIZE){
									deleteFlag = false;
								}
							}
							else{
								IL_SDH_ERR("[%s] Path length has got too long = %d\n", __FUNCTION__, logPathLength);
							}
						}
					}
				}
				free(namelist[n]);
			}
			free(namelist);
		}
	}
	else{
		///TODO: In future
	}

	if(deleteFlag){
		ret = -1;
	}
	else{
		ret = 0;
	}

	MEM_FREE_AND_NULL(pImageLogDirPath);
	return ret;
}

/**
 * \brief Delete Image Log
 *
 * \param option 0: Delete IMAGE_LOG_MIN_MEMSIZE*5 MB old data 1: Delete all
 *
 * \return Return memory size in MB. On error return < 0
 */
int GetFileSystemMemUsage(char *path)
{
	int retVal = 0;
	int curMemSizeInMB = 0;
	struct statfs stStatFS;

	if(statfs(path, &stStatFS) != 0){
		IL_SDH_ERR("[%s] statfs failed on path:%s\n", __FUNCTION__, path);
		retVal = -2;
	}
	else{
		//f_type: type of filesystem
		//f_bsize: optimal transfer block size
		//bavail: free blocks available to unprivileged user
		//f_blocks: total data blocks in filesystem
		//
		curMemSizeInMB = (stStatFS.f_bavail * (stStatFS.f_bsize >> 10))>>10;
		IL_SDH_ERR("FileSystem Path:%s MemorySize:%d\n", path, curMemSizeInMB);

		if(curMemSizeInMB >= IMAGE_LOG_MIN_MEMSIZE){
			g_nImageLogMemFull = 0;
			retVal = curMemSizeInMB;
			SetImageLogStatus(ILS_NO_PROBLEM);
		}
		else{
			g_nImageLogMemFull = 1;
			SetImageLogStatus(ILS_SDCARD_FULL);
			IL_SDH_ERR("[%s] ********** Error: SD Card is full **********\n", __FUNCTION__);
			IL_SDH_ERR("[%s] SD Card is full Minimun_req:%dMB Available:%dMB \n", __FUNCTION__, IMAGE_LOG_MIN_MEMSIZE, curMemSizeInMB);
			retVal = -1;
		}
	}
	return retVal;
}

int GetFileSystemType(char *path)
{
	int retVal = 0;
	struct statfs stStatFS;

	if(statfs(path, &stStatFS) != 0){
		IL_SDH_ERR("[%s] statfs failed on path:%s\n", __FUNCTION__, path);
		retVal = -2;
	}
	else{
		//f_type: type of filesystem
		if(stStatFS.f_type == FILE_SYSTEM_EXT4){
			retVal = 0;
		}
		else{
			IL_SDH_ERR("[%s] ********** Error: Invalid filesystem **********\n", __FUNCTION__);
			IL_SDH_ERR("[%s] Path:%s Detected FileSystemType:0x%X\n", __FUNCTION__, path, stStatFS.f_type);
			retVal = -1;
		}
	}
	return retVal;
}

void SetImageLogStatus(uint32_t status)
{
	if( ((gImageLogSNMPStatus == ILS_SDCARD_FULL) && (status == ILS_TRANSFER_ERR)) ||
			((gImageLogSNMPStatus == ILS_TRANSFER_ERR) && (status == ILS_SDCARD_FULL)) ||
			((gImageLogSNMPStatus == ILS_SDCARD_FULL_TRANSFER_ERR) && ((status == ILS_TRANSFER_ERR) || (status == ILS_SDCARD_FULL)) ) ){
		gImageLogSNMPStatus = ILS_SDCARD_FULL_TRANSFER_ERR;
	}
	else{
		gImageLogSNMPStatus = status;
	}
	//IL_SDH_ERR("[%s] %d\n", __FUNCTION__, gImageLogSNMPStatus);
}

uint32_t GetImageLogStatus()
{
	return gImageLogSNMPStatus;
}

int ARIA_encode(char *in_file, int crypt_flag)
{
    //  if( g_nIsEnableAria == 1)
    //ARIA_do_encode(in_file, crypt_flag);
    return 0;
}
//EOF
