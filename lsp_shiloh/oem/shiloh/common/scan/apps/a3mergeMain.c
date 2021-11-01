
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "posix_ostools.h"

#include "list.h"
#include "memAPI.h"
#include "lassert.h"
#include "agMessage.h"
#include "agConnectMgr.h"
#include "agJobUtils.h"
#include "ResourceMap.h"
#include "sys_init_api.h"
#include "agResourceMgr.h"
#include "logger.h"

#include "a3mergeLib.h"
#include "a3mergeInterface.h"
#include "scantypes.h"
#include "scanmsg.h"
#include "scanmem.h"
#include "db_api.h"

#define DBG_PRFX "SCN MERGE: "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)


#define A3_COPY_MERGED_IMG_SIZE		(18*1024*1024)	//memory size for saving merged image. add.bong.
#define A3_COPY_ROTATED_IMG_SIZE	(5*1024*1024)	//memory size for saving rotated image. add.bong.

#define A3MERGETASK_QUEUE_SIZE 20  ///< Size of the message queue.
#define A3MERGETASK_STACK_SIZE (PTHREAD_STACK_MIN*4) ///< Size of the stack.
#define MESSAGE_FROM_SCANTASK_FLAG (1<<31)

#define A3MERGE_NONE_PAGE	0
#define A3MERGE_FIRST_PAGE	1
#define A3MERGE_SECOND_PAGE	2

#define A3MERGE_USE_DUMP		0
#define A3MERGE_RUN_WITH_APP	1

#define COPY_SEND_DATA_HEIGHT	64

//#define A3_PRINTF(...) {printf("[A3M] "); printf(__VA_ARGS__);}

static void* a3merge_task(void *unused);
static void a3merge_receive_strip_data(unsigned char scan_page, unsigned char *dst, unsigned char *src, unsigned int *savepoint, uint16_t num_rows
										,int merged_width, int merged_height, int partial_width, int partial_height);
static scan_err_t a3merge_send_plane_data_for_copy(unsigned char *full_page_data, int merged_width, int merged_height, scan_data_type dtype, unsigned char page_side);

int a3_dlt_height = 0;	//for dlt - a3 height. add.jong.2012-08-14

ALIGN(8) static UINT8 a3mergetask_stack[A3MERGETASK_STACK_SIZE];
mqd_t a3mergetask_msgq;
static pthread_t a3mergetask_id;
static struct A3MERGE_PARAM tMainParam;
static mlimiter_t* limiter = NULL;

int SDH_A3MergeLib_notifyScanStates(UINT32 arg1,UINT32 arg2,UINT32 arg3)
{
	return 1;
}

const char * getVer_A3CopyComponent(void)
{
	return SDH_A3MergeLib_Version;
}
	
void showVer_A3Merge(void)		
{
	//DBG_PRINTF_SDH_DEBUG("(a3m)$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	DBG_PRINTF_SDH_DEBUG("(a3m)	SDH A3 Merge LIbrary Version : %s\n",SDH_A3MergeLib_Version);
	//DBG_PRINTF_SDH_DEBUG("(a3m)$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	//DBG_PRINTF_SDH_DEBUG("(a3m)		compiled @ %s,%s\n",__DATE__,__TIME__);
	//DBG_PRINTF_SDH_DEBUG("(a3m)		programmed by S.B.Kim (1126)\n");
	//DBG_PRINTF_SDH_DEBUG("(a3m)$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

void a3merge_continuous_onetime_init()
{
	return;
}

bool a3merge_get_rollerDetect()
{
	return tMainParam.rollerDetected;
}

void a3merge_set_rollerDetect(bool detect)
{
	tMainParam.rollerDetected = detect;
}

int SDH_A3MergeLib_init(void)
{
    UINT pxretcode;

    DBG_PRINTF_SDH_DEBUG ( "Merge Main init...\n");

    /* create the queue */
    posix_create_message_queue( &a3mergetask_msgq, "/a3merge",  
                                            A3MERGETASK_QUEUE_SIZE, sizeof(MESSAGE));    

    router_register_queue(A3_MERGE_ID, a3mergetask_msgq);  // Register a message queue.

    //
    // Register the resource
    rm_register(e_A3MergeLib, A3_MERGE_ID);

    // start the scan task
    pxretcode = posix_create_thread( &a3mergetask_id, &a3merge_task, 0, "A3_Merge_App",  a3mergetask_stack,
                     A3MERGETASK_STACK_SIZE, POSIX_THR_PRI_NORMAL );
    XASSERT(pxretcode==0, pxretcode);

    /* davep 16-Aug-2011 ; create the continuous copy thread */
    a3merge_continuous_onetime_init();
 
	return 1;
}

static void a3merge_receive_strip_data(unsigned char scan_page, unsigned char *dst, unsigned char *data, unsigned int *savepoint, uint16_t num_rows
										,int merged_width, int merged_height, int partial_width, int partial_height)
{
	int i, j;

	if(tMainParam.ucScanedPaper == A3MERGE_LG)
	{
		if(tMainParam.BitsPerPixel == 8) // gray
		{
			if (scan_page == A3MERGE_FIRST_PAGE)
			{
				for(j = 0; j < num_rows; j++)
				{
					for(i = 0; i < tMainParam.image_pixels_per_row; i++)
					{
						dst[merged_width * (merged_height - 1 - i) + (j + *savepoint)] = 
							data[tMainParam.image_pixels_per_row_with_padding * j + i];
					}
				}

				*savepoint += num_rows;

			}
			else if (scan_page == A3MERGE_SECOND_PAGE)
			{
				for(j = 0; j < num_rows; j++)
				{
					for(i = 0; i < tMainParam.image_pixels_per_row; i++)
					{
						dst[partial_width * (partial_height - 1 - i) + (j + *savepoint)] = 
							data[tMainParam.image_pixels_per_row_with_padding * j + i];
					}
				}

				*savepoint += num_rows;
			}
		} 
		else if(tMainParam.BitsPerPixel == 24) // color
		{
			if (scan_page == A3MERGE_FIRST_PAGE)
			{
				for(j = 0; j < num_rows; j++)
				{
					for(i = 0; i < tMainParam.image_pixels_per_row; i++)
					{
						dst[(merged_width * 3) * (merged_height - 1 - i) + (j + *savepoint) * 3] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3)]; // R
						dst[(merged_width * 3) * (merged_height - 1 - i) + (j + *savepoint) * 3 + 1] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3) + 1]; // G
						dst[(merged_width * 3) * (merged_height - 1 - i) + (j + *savepoint) * 3 + 2] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3) + 2]; // B
					}
				}

				*savepoint += num_rows;

			}
			else if (scan_page == A3MERGE_SECOND_PAGE)
			{
				for(j = 0; j < num_rows; j++)
				{
					for(i = 0; i < tMainParam.image_pixels_per_row; i++)
					{
						dst[(partial_width * 3) * (partial_height - 1 - i) + (j + *savepoint) * 3] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3)]; // R
						dst[(partial_width * 3) * (partial_height - 1 - i) + (j + *savepoint) * 3 + 1] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3) + 1]; // G
						dst[(partial_width * 3) * (partial_height - 1 - i) + (j + *savepoint) * 3 + 2] = 
							data[(tMainParam.image_pixels_per_row_with_padding * 3) * j + (i * 3) + 2]; // B
					}
				}

				*savepoint += num_rows;
			}
		}
	} 
	else if((tMainParam.ucScanedPaper == A3MERGE_A3) || (tMainParam.ucScanedPaper == A3MERGE_DLT) || (tMainParam.ucScanedPaper == A3SEPARATE_A3) || (tMainParam.ucScanedPaper == A3SEPARATE_DLT) )
	{
		if (scan_page == A3MERGE_FIRST_PAGE)
		{
			//memcpy(first_page+savepoint, data, (tMainParam.image_pixels_per_row * num_rows));
			for(i = 0; i < num_rows; i++)
			{
				memcpy(dst + *savepoint, data + (tMainParam.image_pixels_per_row_with_padding * i), partial_width);

				*savepoint += merged_width;
			}

		}
		else if (scan_page == A3MERGE_SECOND_PAGE)
		{
			//memcpy(second_page+savepoint, data, (tMainParam.image_pixels_per_row * num_rows));
			for(i = 0; i < num_rows; i++)
			{
				memcpy(dst + *savepoint, data + (tMainParam.image_pixels_per_row_with_padding * i), partial_width);

				*savepoint += partial_width;
			}
		}
	}
	
}

static scan_err_t a3merge_send_plane_data_for_copy(unsigned char *full_page_data, int merged_width, int merged_height, scan_data_type dtype, unsigned char page_side)
{
	error_type_t err;
	MESSAGE send_msg;
	
	int remain_height;
	int send_height=0;
	remain_height = merged_height;
	unsigned char *send_page_buffer;
	
	memset(&send_msg, 0x00,sizeof(MESSAGE));
	send_msg.msgType = SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG;
	
	DBG_PRINTF_SDH_ERR("full_page_data %p\n", full_page_data);
	
	while(remain_height)
	{
		if (remain_height > COPY_SEND_DATA_HEIGHT)
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(COPY_SEND_DATA_HEIGHT); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dtype) | SMSG_DATA_SET_PAGE_SIDE(page_side);
			send_page_buffer = scanmem_alloc(merged_width * COPY_SEND_DATA_HEIGHT);

			if (send_page_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (merged_width * COPY_SEND_DATA_HEIGHT), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			
			memcpy(send_page_buffer, full_page_data+(merged_width*send_height), merged_width * COPY_SEND_DATA_HEIGHT);
			remain_height -= COPY_SEND_DATA_HEIGHT;
			send_height += COPY_SEND_DATA_HEIGHT;
		}
		else
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(remain_height); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dtype) | SMSG_DATA_LAST_BUFFER_FLAG | SMSG_DATA_SET_PAGE_SIDE(page_side);
			send_page_buffer = scanmem_alloc(merged_width * remain_height);
			
			if (send_page_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (merged_width * COPY_SEND_DATA_HEIGHT), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			
			memcpy(send_page_buffer, full_page_data+(merged_width*send_height), merged_width * remain_height);
			send_height += remain_height;
			remain_height = 0;
		}
		
		send_msg.param3 = send_page_buffer;

		err = SYMsgSend( SCANMANID, &send_msg );
		if( err != OK ) {
			XASSERT( err==OK, err );
		}
	}
	memFree(full_page_data);
	
	return SCANERR_NONE;
}

static scan_err_t a3merge_send_plane_data_for_separate_copy(unsigned int pageno, unsigned char *full_page_data, int merged_width, int merged_height, scan_data_type dtype, unsigned char page_side)
{
	error_type_t err;
	MESSAGE send_msg;
	
	int remain_height;
	int send_height=0;
	int copy_width=0;
	int send_width=0;
	int offset=0;
	int nLoop=0;
	int A3SeparationWithDot;
	error_type_t db_retval = FAIL;
		
	unsigned char *send_page_buffer;
	
	remain_height = merged_height;
//	copy_width = (merged_width/2);
	copy_width = tMainParam.printed_width;
	
	A3SeparationWithDot = db_get_var_int("CONF_COPYSET", "A3SeparationWithDot", &db_retval);

	if(db_retval == FAIL){
		DBG_PRINTF_SDH_ERR("[%s] DB Fail to get A3SeparationWithDot\n", __FUNCTION__);
		A3SeparationWithDot = 0;
	}
	else
	{
		DBG_PRINTF_SDH_ERR("[%s] DB succes to get A3SeparationWithDot %d\n", __FUNCTION__, A3SeparationWithDot);
	}
	
	
	if (A3SeparationWithDot)
	{
		send_width = copy_width+2;
	}
	else
	{
		send_width = copy_width;
	}
	
	if (pageno == A3MERGE_FIRST_PAGE)
	{
		offset = BORDER_MARGIN*2;
	}
	else
	{
		offset = copy_width+(BORDER_MARGIN*2);
		if (merged_width < (offset+copy_width))
		{
			copy_width = merged_width - offset;
		}
	}
	
	memset(&send_msg, 0x00,sizeof(MESSAGE));
	send_msg.msgType = SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG;
	
	DBG_PRINTF_SDH_ERR("full_page_data %p\n", full_page_data );
	DBG_PRINTF_SDH_ERR("w%d, h%d, sw%d, cw%d, o%d\n", merged_width, merged_height, send_width, copy_width, offset);
	
	while(remain_height)
	{
		if (remain_height > COPY_SEND_DATA_HEIGHT)
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(COPY_SEND_DATA_HEIGHT); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dtype) | SMSG_DATA_SET_PAGE_SIDE(page_side);
			send_page_buffer = scanmem_alloc(send_width * COPY_SEND_DATA_HEIGHT);

			if (send_page_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (merged_width * COPY_SEND_DATA_HEIGHT), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			memset(send_page_buffer, 0xFF, (send_width * COPY_SEND_DATA_HEIGHT));
			for(nLoop=0; nLoop<COPY_SEND_DATA_HEIGHT; nLoop++)
			{
				if (pageno == A3MERGE_FIRST_PAGE)
				{
					memcpy(send_page_buffer+(nLoop*send_width), full_page_data+(merged_width*send_height)+(nLoop*merged_width)+offset, copy_width);
					if ((nLoop > (COPY_SEND_DATA_HEIGHT/2)) && (A3SeparationWithDot))
					{
						send_page_buffer[(nLoop*send_width)+copy_width] = 0x00;
						send_page_buffer[(nLoop*send_width)+copy_width+1] = 0x00;
					}
				}
				else
				{
					memcpy(send_page_buffer+(nLoop*send_width)+2, full_page_data+(merged_width*send_height)+(nLoop*merged_width)+offset, copy_width);
					if ((nLoop > (COPY_SEND_DATA_HEIGHT/2)) && (A3SeparationWithDot))
					{
						send_page_buffer[(nLoop*send_width)] = 0x00;
						send_page_buffer[(nLoop*send_width)+1] = 0x00;
					}
				}
			}
			
			remain_height -= COPY_SEND_DATA_HEIGHT;
			send_height += COPY_SEND_DATA_HEIGHT;
		}
		else
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(remain_height); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dtype) | SMSG_DATA_LAST_BUFFER_FLAG | SMSG_DATA_SET_PAGE_SIDE(page_side);
			send_page_buffer = scanmem_alloc(send_width * remain_height);
			
			if (send_page_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (merged_width * COPY_SEND_DATA_HEIGHT), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			memset(send_page_buffer, 0xFF, (send_width * remain_height));			
			for(nLoop=0; nLoop<remain_height; nLoop++)
			{
				if (pageno == A3MERGE_FIRST_PAGE)
				{
					memcpy(send_page_buffer+(nLoop*send_width), full_page_data+(merged_width*send_height)+(nLoop*merged_width)+offset, copy_width);
					if ((nLoop > (COPY_SEND_DATA_HEIGHT/2)) && (A3SeparationWithDot))
					{
						send_page_buffer[(nLoop*send_width)+copy_width] = 0x00;
						send_page_buffer[(nLoop*send_width)+copy_width+1] = 0x00;
					}
				}
				else
				{
					memcpy(send_page_buffer+(nLoop*send_width)+2, full_page_data+(merged_width*send_height)+(nLoop*merged_width)+offset, copy_width);
					if ((nLoop > (COPY_SEND_DATA_HEIGHT/2)) && (A3SeparationWithDot))
					{
						send_page_buffer[(nLoop*send_width)] = 0x00;
						send_page_buffer[(nLoop*send_width)+1] = 0x00;
					}
				}
			}
			
			send_height += remain_height;
			remain_height = 0;
		}
		
		send_msg.param3 = send_page_buffer;

		err = SYMsgSend( SCANMANID, &send_msg );
		if( err != OK ) {
			XASSERT( err==OK, err );
		}
	}
	
	if (pageno == A3MERGE_SECOND_PAGE)
		memFree(full_page_data);

	return SCANERR_NONE;
}

/******************************************************
 * Name: sc_client_a3merge_task
 * Function:
 *
 *
 ******************************************************/
void* a3merge_task(void *unused)
{
#if A3MERGE_RUN_WITH_APP
	error_type_t err;
	MESSAGE send_msg;
#endif
	MESSAGE backupMsg_page_start;
	MESSAGE backupMsg_scan_end;
	MESSAGE backupMsg_scan_ready;
	
	MESSAGE msg;
	unsigned int pxretcode;
	unsigned char *data;
	bool last_buffer;
	uint16_t num_rows = 0;
	scan_data_type dtype = SCAN_DATA_TYPE_RGB;
	unsigned char page_side;
	unsigned char scan_page;
	unsigned char *first_page = NULL;
	unsigned char *second_page = NULL;
	unsigned char *merge_result_rotated_page = NULL;
	unsigned int savepoint;
	
	// jwjung: for merge scan
	int i, j;
	BYTE2Dgray imgL, imgR, imgL_tmp;
	BYTE2Dcolor imgCL, imgCR, imgCL_tmp;
	Point2D32f *featurePointL, *featurePointR;
	Float2Dgray transformation, t1;
	int nPointL, nPointR;
	int match_cnt, LL, LL2;
	int merged_width, merged_height;
	int partial_width, partial_height;
	int overlap_scale, overlap_width;
	
	showVer_A3Merge();

	scan_page = A3MERGE_NONE_PAGE;

	while (1)
	{
		// get a message
		pxretcode = posix_wait_for_message(a3mergetask_msgq, (char *)&msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
		XASSERT(pxretcode == 0, pxretcode);

		switch (msg.msgType)
		{
			/*
			******************************************************************************************
			* MSG_A3MERGE_SET_START
			******************************************************************************************
			*/
			case MSG_A3MERGE_SET_START:
				DBG_PRINTF_SDH_DEBUG("MSG_A3MERGE_SET_START\n");
				memset(&tMainParam, 0x00, sizeof(tMainParam));
				tMainParam.ucMainApp = msg.param1;
				tMainParam.ucScanedPaper = msg.param2;
				
				if ( (tMainParam.ucScanedPaper == A3SEPARATE_A3) || (tMainParam.ucScanedPaper == A3SEPARATE_DLT) )
				{
					tMainParam.printed_width = (unsigned int)msg.param3;
					tMainParam.printed_width = ((tMainParam.printed_width*300)/2540) - (BORDER_MARGIN*2) ;

					DBG_PRINTF_SDH_ERR("[RYU] printed_width:%d\n", tMainParam.printed_width);
				}
				
				
				scan_page = A3MERGE_FIRST_PAGE;
                limiter = mlimiter_by_name("scan");
                mlimiter_start(limiter);
			break;

			/*
			******************************************************************************************
			* MSG_STARTRECIPE
			******************************************************************************************
			*/
			case MSG_STARTRECIPE:
			break;

			/*
			******************************************************************************************
			* MSG_RESOURCES
			******************************************************************************************
			*/
			case MSG_RESOURCES:
			break;

			/*
			******************************************************************************************
			* MSG_ACKRECIPE
			******************************************************************************************
			*/
			case MSG_ACKRECIPE :
			break;

			/*
			******************************************************************************************
			* SMSG_SCAN_SIZE
			******************************************************************************************
			*/
			case (SMSG_SCAN_SIZE | MESSAGE_FROM_SCANTASK_FLAG):
				msg_scan_size_decode(&msg, &tMainParam.image_pixels_per_row_with_padding, &tMainParam.image_pixels_per_row,
									&tMainParam.LinesPerSheet, &tMainParam.BitsPerPixel);
				DBG_PRINTF_SDH_DEBUG( "ppr=%d pprp=%d tr=%d bpp=%d\n", tMainParam.image_pixels_per_row, tMainParam.image_pixels_per_row_with_padding,
							tMainParam.LinesPerSheet, tMainParam.BitsPerPixel);
							
				savepoint = 0;

				if (scan_page == A3MERGE_FIRST_PAGE)
				{
					// jwjung: set merged page width, height
					switch(tMainParam.ucScanedPaper)
					{
					case A3MERGE_A3:
					case A3SEPARATE_A3:
						merged_width = tMainParam.image_pixels_per_row*2;
						if (tMainParam.LinesPerSheet%2)
							merged_height = tMainParam.LinesPerSheet+1;
						else
							merged_height = tMainParam.LinesPerSheet;
						partial_width = tMainParam.image_pixels_per_row;
						partial_height = merged_height;
						a3_dlt_height = merged_height;

						if(tMainParam.ucMainApp == A3MERGE_SCAN) // A4_LETTER
						{
							DBG_PRINTF_SDH_DEBUG("A3_MERGE_SCAN mode\n");

							tMainParam.mergeResultShape = A3MERGE_ROTATE_MERGE_RESULT;
							overlap_scale = A3_OVERLAP_SCALE;	
							overlap_width = A3_OVERLAP_WIDTH_SCAN;			

							nPointL = nPointR = MAX_FEATURE / overlap_scale;
						}
						else if(tMainParam.ucMainApp == A3MERGE_COPY) // A5_HALFLETTER
						{
							DBG_PRINTF_SDH_DEBUG("A3_MERGE_COPY mode\n");

							overlap_scale = A3_OVERLAP_SCALE;
							tMainParam.mergeResultShape = A3MERGE_RETAIN_MERGE_RESULT;
							if (tMainParam.ucScanedPaper == A3MERGE_A3)
							{
								overlap_width = A3_OVERLAP_WIDTH_COPY;
							}
							else
							{
								overlap_width = A3_OVERLAP_WIDTH_SCAN;
								
							}

							nPointL = nPointR = MAX_FEATURE / overlap_scale;
						}
						break;

					case A3MERGE_DLT:
					case A3SEPARATE_DLT:
						merged_width = tMainParam.image_pixels_per_row*2;
						if (tMainParam.LinesPerSheet%2)
							merged_height = tMainParam.LinesPerSheet+1;
						else
							merged_height = tMainParam.LinesPerSheet;
						partial_width = tMainParam.image_pixels_per_row;
						partial_height = merged_height;
						a3_dlt_height = merged_height;

						if(tMainParam.ucMainApp == A3MERGE_SCAN) // A4_LETTER
						{
							DBG_PRINTF_SDH_DEBUG("DLT_MERGE_SCAN mode\n");

							tMainParam.mergeResultShape = A3MERGE_ROTATE_MERGE_RESULT;
							overlap_scale = DLT_OVERLAP_SCALE;	
							overlap_width = DLT_OVERLAP_WIDTH_SCAN;			

							nPointL = nPointR = MAX_FEATURE / overlap_scale;
						}
						else if(tMainParam.ucMainApp == A3MERGE_COPY) // A5_HALFLETTER
						{
							DBG_PRINTF_SDH_DEBUG("DLT_MERGE_COPY mode\n");

							overlap_scale = DLT_OVERLAP_SCALE;
							tMainParam.mergeResultShape = A3MERGE_RETAIN_MERGE_RESULT;
							if (tMainParam.ucScanedPaper == A3MERGE_DLT)
							{
								overlap_width = DLT_OVERLAP_WIDTH_COPY;
							}
							else
							{
								overlap_width = DLT_OVERLAP_WIDTH_SCAN;
							}

							nPointL = nPointR = MAX_FEATURE / overlap_scale;
						}
						break;

					case A3MERGE_LG:
						merged_width = tMainParam.LinesPerSheet * 2;
						if (tMainParam.image_pixels_per_row%2)
							merged_height = tMainParam.image_pixels_per_row+1;
						else
							merged_height = tMainParam.image_pixels_per_row;
							
						
						if (tMainParam.LinesPerSheet%2)
							partial_width = tMainParam.LinesPerSheet+1;
						else
							partial_width = tMainParam.LinesPerSheet;

						if (tMainParam.image_pixels_per_row%2)
							partial_height = tMainParam.image_pixels_per_row+1;
						else
							partial_height = tMainParam.image_pixels_per_row;
						a3_dlt_height = tMainParam.image_pixels_per_row;

//						if(tMainParam.ucMainApp == A3MERGE_SCAN) // LEGAL
						{
							DBG_PRINTF_SDH_DEBUG("LG_MERGE_SCAN mode\n");

							tMainParam.mergeResultShape = A3MERGE_ROTATE_MERGE_RESULT;
							overlap_scale = LG_OVERLAP_SCALE;	
							overlap_width = LG_OVERLAP_WIDTH_SCAN;			

							nPointL = nPointR = MAX_FEATURE / overlap_scale;
						}
/*						else if(tMainParam.ucMainApp == A3MERGE_COPY) // HALF_LEGAL
						{
							DBG_PRINTF_SDH_DEBUG("LG_MERGE_COPY mode\n");

							tMainParam.mergeResultShape = A3MERGE_RETAIN_MERGE_RESULT;
							overlap_scale = (LG_OVERLAP_SCALE == 1) ? 1 : (LG_OVERLAP_SCALE / 2);
							overlap_width = LG_OVERLAP_WIDTH_COPY;

							nPointL = nPointR = MAX_FEATURE / (overlap_scale * 2);
						}
*/						break;
					}
					merged_width -= overlap_width;

					DBG_PRINTF_SDH_DEBUG("merged_width = %d, merged_height = %d, partial_width = %d, partial_height = %d\n",
						merged_width, merged_height, partial_width, partial_height);
					DBG_PRINTF_SDH_DEBUG("overlap_scale = %d, overlap_width = %d, nPointL = %d, nPointR = %d\n",
						overlap_scale, overlap_width, nPointL, nPointR);

					// yhlee: 'first_page' memory free in Application(MSG_SCAN_PLANEDATA)(Copy,Scan,Fax, etc...)

					// jwjung: 1st page malloc, memset and page setting
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						DBG_PRINTF_SDH_DEBUG("Gray mode\n");
						
						first_page = scanmem_alloc(merged_width * merged_height);
						if (!first_page)
						{
							DBG_PRINTF_SDH_ERR("scanmem_alloc failed : first page\n");
							/* error handling code is necessary */
						}

						memset(first_page, WHITE_VALUE, merged_width * merged_height);

						imgL.width = merged_width;
						imgL.height = merged_height;

						// make 2d pointer
						imgL.data = (unsigned char **)malloc(imgL.height * sizeof(unsigned char *));
						if(imgL.data == NULL)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : first page 2d pointer\n");
							/* error handling code is necessary */
						}

						for(i = 0; i < imgL.height; i++)
						{
							imgL.data[i] = &first_page[i * imgL.width];
						}
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						DBG_PRINTF_SDH_DEBUG("Color mode\n");

						first_page = scanmem_alloc(merged_width * merged_height * 3);
						if (!first_page)
						{
							DBG_PRINTF_SDH_ERR("scanmem_alloc failed : first page\n");
							/* error handling code is necessary */
						}

						memset(first_page, WHITE_VALUE, merged_width * merged_height * 3);

						imgCL.width = merged_width;
						imgCL.height = merged_height;

						// make 2d pointer
						imgCL.data = (RgbPixel **)malloc(imgCL.height * sizeof(RgbPixel *));
						if(imgCL.data == NULL)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : first page 2d pointer\n");
							/* error handling code is necessary */
						}

						for(i = 0; i < imgCL.height; i++)
						{
							imgCL.data[i] = (RgbPixel *)&first_page[i * imgCL.width * 3];
						}
					}
				}
				else if (scan_page == A3MERGE_SECOND_PAGE)
				{
					// yhlee: 'second_page' memory free in this task after merge finish

					// jwjung: 2nd malloc, memset and page setting
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						second_page = scanmem_alloc(partial_width * partial_height);
						
						if (!second_page)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : second page\n");
							/* error handling code is necessary */
						}

						memset(second_page, WHITE_VALUE, partial_width * partial_height);

						imgR.width = partial_width;
						imgR.height = partial_height;

						// make 2d pointer
						imgR.data = (unsigned char **)malloc(imgR.height * sizeof(unsigned char *));
						if(imgR.data == NULL)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : second page 2d pointer\n");
							/* error handling code is necessary */
						}

						for(i = 0; i < imgR.height; i++)
						{
							imgR.data[i] = &second_page[i * imgR.width];
						}
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						second_page = scanmem_alloc(partial_width * partial_height * 3);
						if (!second_page)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : second page\n");
							/* error handling code is necessary */
						}

						memset(second_page, WHITE_VALUE, partial_width * partial_height * 3);

						imgCR.width = partial_width;
						imgCR.height = partial_height;

						// make 2d pointer
						imgCR.data = (RgbPixel **)malloc(imgCR.height * sizeof(RgbPixel *));
						if(imgCR.data == NULL)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : second page 2d pointer\n");
							/* error handling code is necessary */
						}

						for(i = 0; i < imgCR.height; i++)
						{
							imgCR.data[i] = (RgbPixel *)&second_page[i * imgCR.width * 3];
						}
					}
				}
				break;

			/*
			******************************************************************************************
			* SMSG_PAGE_START
			******************************************************************************************
			*/
			case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("MSG_SCAN_PAGESTART\n");
				//backup the message
				memset(&backupMsg_page_start, 0x00, sizeof(MESSAGE));
				memcpy(&backupMsg_page_start, &msg, sizeof(MESSAGE));
			break;

			/*
			******************************************************************************************
			* SMSG_PAGE_DATA
			******************************************************************************************
			*/
			case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG):
				msg_scan_planedata_decode(&msg, &num_rows, &dtype, &data, &last_buffer);
				msg_scan_planedata_get_page_side( &msg, &page_side );
				//DBG_PRINTF_SDH_DEBUG("received planedata side=%d rows=%d dtype=%d last=%d data=%p\n", page_side, num_rows, dtype, last_buffer, data);
				
				if (scan_page == A3MERGE_FIRST_PAGE)
					a3merge_receive_strip_data(scan_page, first_page, data, &savepoint, num_rows, merged_width, merged_height, partial_width, partial_height);
				else
					a3merge_receive_strip_data(scan_page, second_page, data, &savepoint, num_rows, merged_width, merged_height, partial_width, partial_height);
				
				memFree(data);
				//savepoint += (tMainParam.image_pixels_per_row * num_rows);
				
			break;

			/*
			******************************************************************************************
			* SMSG_PAGE_END
			******************************************************************************************
			*/
			case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("MSG_SCAN_PAGEEND : scan_page(%d)\n", scan_page);
#if A3MERGE_USE_DUMP
				FILE *fs;
#endif

				if (scan_page == A3MERGE_FIRST_PAGE)
			    {
#if A3MERGE_USE_DUMP
					fs = fopen(  "/mnt/sda1/first_page.raw", "wb" );
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						fwrite(first_page, 1, merged_width * merged_height, fs);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						fwrite(first_page, 1, merged_width * merged_height * 3, fs);
					}
#endif
					if( a3merge_get_rollerDetect() == FALSE )
					{
						// jwjung: memory for feature points of 1st page
						featurePointL = (Point2D32f *)malloc(nPointL * sizeof(Point2D32f));
						if(featurePointL == NULL)
						{
							DBG_PRINTF_SDH_ERR("malloc failed : featurePointL memory\n");
							/* error handling code is necessary */
						}
						
						// find feature points of 1st page
						if(tMainParam.BitsPerPixel == 8) // gray
						{
							nPointL = locateFeaturePoints(&imgL, partial_width, partial_height, featurePointL,
								overlap_scale, overlap_width, TRUE);
						} 
						else if(tMainParam.BitsPerPixel == 24) // color
						{
							nPointL = locateFeaturePointsColor(&imgCL, partial_width, partial_height, featurePointL,
								overlap_scale, overlap_width, TRUE);
						}
						if(nPointL == MEMORY_FAIL)
						{
							DBG_PRINTF_SDH_ERR("locateFeaturePoints() failed : 1st page\n");
							/* error handling code is necessary */
						}
						DBG_PRINTF_SDH_DEBUG("locateFeaturePoints() done : 1st page, nPointL = %d\n", nPointL);
					}
				}
#if A3MERGE_USE_DUMP
				else if (scan_page == A3MERGE_SECOND_PAGE)
				{
					fs = fopen(  "/mnt/sda1/second_page.raw", "wb" );
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						fwrite(second_page, 1, partial_width * partial_height, fs);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						fwrite(second_page, 1, partial_width * partial_height * 3, fs);
					}
				}

				if (fs)
					fclose(fs);
				fs = NULL;
#endif
			break;
			
			/*
			******************************************************************************************
			* SMSG_SCAN_END
			******************************************************************************************
			*/
			case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("SMSG_SCAN_END : scan_page(%d)\n", scan_page);
				
				//backup the message
				memset(&backupMsg_scan_end, 0x00,sizeof(MESSAGE));
				memcpy(&backupMsg_scan_end, &msg, sizeof(MESSAGE));			
				
				if (scan_page == A3MERGE_FIRST_PAGE)
				{
					scan_page = A3MERGE_SECOND_PAGE;
					
					/*
					 ***********************************************************************************************************
					 * [Coding Here] : 
					 * Check Roller Detect
					 * 		1) If Roller Not Detected : Request Next page Scan
					 * 		2) If Roller Detected : No Paper on FB (FB empty) -> send message to Finish Job
					 ***********************************************************************************************************
					 */
					if( a3merge_get_rollerDetect() == FALSE ) {
						memset(&send_msg, 0x00,sizeof(MESSAGE));
						send_msg.msgType = MSG_A3MERGE_WAIT_NEXTPAGE_JOB_START;
						send_msg.param1 = A3MERGE_STATUS_1PageEnd;
						err = SYMsgSend( SCANMANID, &send_msg );
						if( err != OK ) {
							XASSERT( err==OK, err );
						}
					}
					//else --> We wait until 'SMSG_SCAN_READY' received from 'ScanMan'
				}
				else if (scan_page == A3MERGE_SECOND_PAGE)
				{
					scan_page = A3MERGE_NONE_PAGE;
											
					// jwjung: memory for feature points of 2nd page
					featurePointR = (Point2D32f *)malloc(nPointR * sizeof(Point2D32f));
					if(featurePointL == NULL)
					{
						DBG_PRINTF_SDH_ERR("malloc failed : featurePointR memory\n");
						/* error handling code is necessary */
					}
					
					// find feature points of 2nd page
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						nPointR = locateFeaturePoints(&imgR, partial_width, partial_height, featurePointR,
							overlap_scale, overlap_width, FALSE);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						nPointR = locateFeaturePointsColor(&imgCR, partial_width, partial_height, featurePointR,
							overlap_scale, overlap_width, FALSE);
					}
					if(nPointR == MEMORY_FAIL)
					{
						DBG_PRINTF_SDH_ERR("locateFeaturePoints() failed : 2nd page\n");
						/* error handling code is necessary */
					}
					DBG_PRINTF_SDH_DEBUG("locateFeaturePoints() done : 2nd page, nPointR = %d\n", nPointR);
					
					// jwjung: merge start
					// for transformation data
					if(make2Dfloatarray(&transformation, 3, 3) == FALSE)
					{
						DBG_PRINTF_SDH_ERR("make2Dfloatarray() failed : transformation\n");
						/* error handling code is necessary */
					}
					
					// find match point among feature points of 1st and 2nd page
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						match_cnt = computeTransform(&imgL, &imgR, featurePointL, featurePointR, nPointL, nPointR,
							overlap_width, &transformation);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						match_cnt = computeTransformColor(&imgCL, &imgCR, featurePointL, featurePointR, nPointL, nPointR,
							overlap_width, &transformation);
					}
					featurePointL = NULL;
					featurePointR = NULL;
					if(match_cnt == MEMORY_FAIL)
					{
						DBG_PRINTF_SDH_ERR("computeTransform() failed\n");
						/* error handling code is necessary */
					}
					DBG_PRINTF_SDH_DEBUG("computeTransform() done : match_cnt = %d\n", match_cnt);
					
					// calculate transformation matrix
					if(make2Dfloatarray(&t1, 3, 3) == FALSE)
					{
						DBG_PRINTF_SDH_ERR("make2Dfloatarray() failed : t1\n");
						/* error handling code is necessary */
					}
					
					t1.data[0][0] = 1.0f;
					t1.data[0][1] = 0.0f;
					t1.data[0][2] = (float)(partial_width - overlap_width);
					t1.data[1][0] = 0.0f;
					t1.data[1][1] = 1.0f;
					t1.data[1][2] = 0.0f;
					t1.data[2][0] = 0.0f;
					t1.data[2][1] = 0.0f;
					t1.data[2][2] = 1.0f;
					
					if(matMul(&t1, &transformation, &transformation, 0, 0) == MEMORY_FAIL)
					{
						DBG_PRINTF_SDH_ERR("matMul() failed\n");
						/* error handling code is necessary */
					}
					
					deletefloat2D(&t1);
					
					LL = (int)(transformation.data[0][2]);
					
					// overlap region temp memory
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						if(make2DBYTEarray(&imgL_tmp, partial_width - LL, partial_height) == FALSE)
						{
							DBG_PRINTF_SDH_ERR("make2DBYTEarray() failed : imgL_tmp\n");
							/* error handling code is necessary */
						}

						for(j = 0; j < imgL.height; j++)
						{
							for(i = LL; i < partial_width; i++)
							{
								imgL_tmp.data[j][i - LL] = imgL.data[j][i];
								imgL.data[j][i] = WHITE_VALUE;
							}
						}
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						if(make2Dcolorarray(&imgCL_tmp, partial_width - LL, partial_height) == FALSE)
						{
							DBG_PRINTF_SDH_ERR("make2DBYTEarray() failed : imgL_tmp\n");
							/* error handling code is necessary */
						}

						for(j = 0; j < imgCL.height; j++)
						{
							for(i = LL; i < partial_width; i++)
							{
								imgCL_tmp.data[j][i - LL].r = imgCL.data[j][i].r;
								imgCL_tmp.data[j][i - LL].g = imgCL.data[j][i].g;
								imgCL_tmp.data[j][i - LL].b = imgCL.data[j][i].b;
								imgCL.data[j][i].r = WHITE_VALUE;
								imgCL.data[j][i].g = WHITE_VALUE;
								imgCL.data[j][i].b = WHITE_VALUE;
							}
						}
					}
					DBG_PRINTF_SDH_DEBUG("overlap region copy done\n");
					
					// warp and affine images
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						LL2 = imgWarpAffine(&imgR, &imgL, &transformation);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						LL2 = imgWarpAffineColor(&imgCR, &imgCL, &transformation);
					}
					if(LL2 == MEMORY_FAIL)
					{
						DBG_PRINTF_SDH_ERR("imgWarpAffine() failed\n");
						/* error handling code is necessary */
					}
					
					deletefloat2D(&transformation);
					
					DBG_PRINTF_SDH_DEBUG("imgWarpAffine() done\n");
					
					// blend dividing line
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						if(blending(&imgL, &imgL_tmp, partial_width, partial_height,
							LL, LL2, PARAM_BLEND_TYPE) == MEMORY_FAIL)
						{
							DBG_PRINTF_SDH_ERR("blending() failed\n");
							/* error handling code is necessary */
						}
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						if(blendingColor(&imgCL, &imgCL_tmp, partial_width, partial_height,
							LL, LL2, PARAM_BLEND_TYPE) == MEMORY_FAIL)
						{
							DBG_PRINTF_SDH_ERR("blending() failed\n");
							/* error handling code is necessary */
						}
					}
					DBG_PRINTF_SDH_DEBUG("blending() done\n");
					
					// bordering
					if ((tMainParam.ucScanedPaper == A3MERGE_A3) || (tMainParam.ucScanedPaper == A3MERGE_DLT) || (tMainParam.ucScanedPaper == A3MERGE_LG))
					{
						if(tMainParam.BitsPerPixel == 8) // gray
						{
							if ((tMainParam.ucMainApp == A3MERGE_COPY) && (tMainParam.ucScanedPaper == A3MERGE_A3))
								BorderMargin(&imgL, BORDER_MARGIN, BODDER_SIDE_TOP | BODDER_SIDE_LEFT | BODDER_SIDE_RIGHT);
							else if ((tMainParam.ucMainApp == A3MERGE_COPY) && (tMainParam.ucScanedPaper == A3MERGE_LG))
								BorderMargin(&imgL, BORDER_MARGIN, BODDER_SIDE_TOP | BODDER_SIDE_BOTTOM | BODDER_SIDE_LEFT);
							else
								BorderMargin(&imgL, BORDER_MARGIN, BODDER_SIDE_ALL);
						} 
						else if(tMainParam.BitsPerPixel == 24) // color
						{
							BorderMarginColor(&imgCL, BORDER_MARGIN);
						}
						DBG_PRINTF_SDH_DEBUG("BorderMargin() done\n");
					}
					else
					{
						BorderMargin(&imgL, BORDER_MARGIN*2, BODDER_SIDE_TOP );
						DBG_PRINTF_SDH_DEBUG("BorderMargin() done\n");
					}
						
					// delete malloc
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						if(imgL.data) {
							free(imgL.data);
							imgL.data = NULL;
						}
						if(imgR.data) {
							free(imgR.data);
							imgR.data = NULL;
						}
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						if(imgCL.data) {
							free(imgCL.data);
							imgCL.data = NULL;
						}
						if(imgCR.data) {
							free(imgCR.data);
							imgCR.data = NULL;
						}
					}
					
#if A3MERGE_USE_DUMP
					FILE *fs;
					
					fs = fopen(  "/mnt/sda1/merged_page.raw", "wb" );
					if(tMainParam.BitsPerPixel == 8) // gray
					{
						fwrite(first_page, 1, merged_width * merged_height, fs);
					} 
					else if(tMainParam.BitsPerPixel == 24) // color
					{
						fwrite(first_page, 1, merged_width * merged_height * 3, fs);
					}

					if (fs)
						fclose(fs);
					fs = NULL;
					DBG_PRINTF_SDH_DEBUG("merged image dump done\n");
#endif			
					if(tMainParam.mergeResultShape == A3MERGE_ROTATE_MERGE_RESULT) {
						//Malloc buffer for rotate the merge result image buffer
						merge_result_rotated_page = scanmem_alloc(merged_width * merged_height);
						if (!merge_result_rotated_page)
						{
							DBG_PRINTF_SDH_ERR("scanmem_alloc failed : merge_result_rotated_page\n");
							/* error handling code is necessary */
						}
						else
						{
							//Rotate image buffer and save it to 'merge_result_rotated_page'
							DBG_PRINTF_SDH_DEBUG("rotate_buffer : START!\n");					
							rotate_buffer(first_page, merged_width, merged_height, merge_result_rotated_page);
							DBG_PRINTF_SDH_DEBUG("rotate_buffer : END!\n");					
							
							///It Takes about 1,670ms == 1.67s
							//74360: SCN MERGE: rotate_buffer : START!
							//76030: SCN MERGE: rotate_buffer : END!
						}
						if (first_page)
						{
							memFree(first_page);
							first_page = NULL;
						}
					}
										
#if !A3MERGE_RUN_WITH_APP
					//If just only test with DUMP, free 'first_page' memory here, in Actual function 'first_page' free in Application
					if (first_page)
					{
						memFree(first_page);
						first_page = NULL;
					}
#endif
					if (second_page)
					{
						memFree(second_page);
						second_page = NULL;
					}
				}
			break;
			
			/*
			******************************************************************************************
			* SMSG_SCAN_READY
			******************************************************************************************
			*/
            case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):	
				DBG_PRINTF_SDH_DEBUG("SMSG_SCAN_READY : scan_page(%d)\n", scan_page);
				
				//backup the message
				memset(&backupMsg_scan_ready, 0x00,sizeof(MESSAGE));
				memcpy(&backupMsg_scan_ready, &msg, sizeof(MESSAGE));
				
#if A3MERGE_RUN_WITH_APP
				//It means all merge scan job finished.
				if(scan_page == A3MERGE_NONE_PAGE) {					
					/**
					 * This routine is made by SINDOH, some message is defined in "scantypes.h"
					 *
					 * \brief make meesage to Scan Man (Send below message to ScanMan sequence by sequence)
					 * 		-MSG_A3MERGE_NOTIFY_MERGE_END
					 * 		-SMSG_SCAN_SIZE
					 * 		-SMSG_PAGE_START
					 * 		-SMSG_PAGE_DATA
					 * 		-SMSG_PAGE_END
					 * 		-SMSG_SCAN_END
					 *		-SMSG_SCAN_READY
					 **/
 					if(tMainParam.mergeResultShape == A3MERGE_ROTATE_MERGE_RESULT) {
						int last_merged_height = merged_height;
						int last_merged_width = merged_width;
						merged_height = last_merged_width;
						merged_width = last_merged_height;
					}
					 
					/* Notify merge finished to 'ScanMan' */
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = MSG_A3MERGE_NOTIFY_MERGE_END;
					send_msg.param1 = A3MERGE_STATUS_OutputEnd;
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
										
					//Send 'SMSG_SCAN_SIZE' to 'ScanMan'
					uint32_t size_info = 0;
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = SMSG_SCAN_SIZE | MESSAGE_FROM_SCANTASK_FLAG;
					if ( (tMainParam.ucScanedPaper == A3MERGE_A3) || (tMainParam.ucScanedPaper == A3MERGE_DLT) || (tMainParam.ucScanedPaper == A3MERGE_LG) )
					{
						send_msg.param1 = merged_width;		//pixels_per_row_padded
						send_msg.param2 = merged_width; 	//pixels_per_row
					}
					else
					{
						int A3SeparationWithDot;
						error_type_t db_retval = FAIL;
						
						A3SeparationWithDot = db_get_var_int("CONF_COPYSET", "A3SeparationWithDot", &db_retval);
						if(db_retval == FAIL){
							DBG_PRINTF_SDH_ERR("[%s] DB Fail to get A3SeparationWithDot\n", __FUNCTION__);
							A3SeparationWithDot = 0;
						}
						else
						{
							DBG_PRINTF_SDH_ERR("[%s] DB succes to get A3SeparationWithDot %d\n", __FUNCTION__, A3SeparationWithDot);
						}
						
						if (A3SeparationWithDot)
						{
							send_msg.param1 = tMainParam.printed_width + 2;		//pixels_per_row_padded
							send_msg.param2 = tMainParam.printed_width + 2;		//pixels_per_row
						}
						else
						{
							send_msg.param1 = tMainParam.printed_width;			//pixels_per_row_padded
							send_msg.param2 = tMainParam.printed_width;			//pixels_per_row
						}
					}
					size_info |= SMSG_SIZE_SET_NUM_ROWS( merged_height );
					size_info |= SMSG_SIZE_SET_PIXEL_SIZE( tMainParam.BitsPerPixel );					
					send_msg.param3 = (void *)size_info;
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					
					//Send 'SMSG_PAGE_START' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					memcpy(&send_msg, &backupMsg_page_start, sizeof(MESSAGE));
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					
					//Send 'SMSG_PAGE_DATA' to 'ScanMan'
					if(tMainParam.ucMainApp == A3MERGE_SCAN)
					{
						memset(&send_msg, 0x00,sizeof(MESSAGE));
						send_msg.msgType = SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG;
						send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(merged_height); 
						send_msg.param2 = SMSG_DATA_SET_DATATYPE(dtype) | SMSG_DATA_LAST_BUFFER_FLAG | SMSG_DATA_SET_PAGE_SIDE(page_side);
	 					if(tMainParam.mergeResultShape == A3MERGE_ROTATE_MERGE_RESULT) {
							send_msg.param3 = merge_result_rotated_page;	//pointer to the data
							DBG_PRINTF_SDH_DEBUG("SMSG_PAGE_DATA: merge_result_rotated_page ptr=%p\n", merge_result_rotated_page);
							merge_result_rotated_page = NULL;
						}
						else {
							send_msg.param3 = first_page;	//pointer to the data
							DBG_PRINTF_SDH_DEBUG("SMSG_PAGE_DATA: first_page ptr=%p\n", first_page);
						}
						err = SYMsgSend( SCANMANID, &send_msg );
						if( err != OK ) {
							XASSERT( err==OK, err );
						}
					}
					else if(tMainParam.ucMainApp == A3MERGE_COPY)
					{
						if ((tMainParam.ucScanedPaper == A3MERGE_A3) || (tMainParam.ucScanedPaper == A3MERGE_DLT) || (tMainParam.ucScanedPaper == A3MERGE_LG) )
						{
		 					if(tMainParam.mergeResultShape == A3MERGE_ROTATE_MERGE_RESULT) {
								a3merge_send_plane_data_for_copy(merge_result_rotated_page, merged_width, merged_height, dtype, page_side);
								merge_result_rotated_page = NULL;
							}
							else {
								a3merge_send_plane_data_for_copy(first_page, merged_width, merged_height, dtype, page_side);
								first_page = NULL;
							}
						}
						else
						{
							a3merge_send_plane_data_for_separate_copy(A3MERGE_FIRST_PAGE, first_page, merged_width, merged_height, dtype, page_side);
							
							//Send 'SMSG_PAGE_END' to 'ScanMan'
							memset(&send_msg, 0x00,sizeof(MESSAGE));
							send_msg.msgType = SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG;
							err = SYMsgSend( SCANMANID, &send_msg );
							if( err != OK ) {
								XASSERT( err==OK, err );
							}

							//Send 'SMSG_PAGE_START' to 'ScanMan'
							memset(&send_msg, 0x00,sizeof(MESSAGE));
							memcpy(&send_msg, &backupMsg_page_start, sizeof(MESSAGE));
							err = SYMsgSend( SCANMANID, &send_msg );
							if( err != OK ) {
								XASSERT( err==OK, err );
							}							
									
							a3merge_send_plane_data_for_separate_copy(A3MERGE_SECOND_PAGE, first_page, merged_width, merged_height, dtype, page_side);
						}
					}
					
					//Send 'SMSG_PAGE_END' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG;
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					
					//Send 'SMSG_SCAN_END' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					memcpy(&send_msg, &backupMsg_scan_end, sizeof(MESSAGE));
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					
					//Send 'SMSG_SCAN_READY' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					memcpy(&send_msg, &msg, sizeof(MESSAGE));
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					if (limiter)
					{
						mlimiter_stop(limiter);
						limiter = NULL;
					}
				}
				else if( a3merge_get_rollerDetect() == TRUE ) {
					///Coding Here : FB Empty for merge(Roller Detected)
					/* Send 'MSG_A3MERGE_NOTIFY_MERGE_END' by self */
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = MSG_A3MERGE_NOTIFY_MERGE_END;
					send_msg.param1 = A3MERGE_STATUS_RollerDetected;
					err = SYMsgSend( A3_MERGE_ID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
				}
#endif
            break;

			/*
			******************************************************************************************
			* MSG_CANCELJOB
			******************************************************************************************
			*/
			case MSG_CANCELJOB :
				DBG_PRINTF_SDH_DEBUG("MSG_A3MERGE_CANCEL\n");
				//free Memory
				if(imgL.data) {
					free(imgL.data);
					imgL.data = NULL;
				}
				if(imgR.data) {
					free(imgR.data);
					imgR.data = NULL;
				}
				if (first_page)
				{
					memFree(first_page);
					first_page = NULL;
				}
				if (second_page)
				{
					memFree(second_page);
					second_page = NULL;
				}
				if (merge_result_rotated_page)
				{
					memFree(merge_result_rotated_page);
					merge_result_rotated_page = NULL;
				}
				if (featurePointL)
				{
					free(featurePointL);
					featurePointL = NULL;
				}
				if (featurePointR)
				{
					free(featurePointR);
					featurePointR = NULL;
				}
				if (limiter)
				{
					mlimiter_stop(limiter);
					limiter = NULL;
				}
				
				scan_page = A3MERGE_NONE_PAGE;
			break;
			
			/*
			******************************************************************************************
			* MSG_A3MERGE_ROLLER_DETECTED (Received From 'Erase Roller')
			******************************************************************************************
			*/
			case MSG_A3MERGE_ROLLER_DETECTED:
				DBG_PRINTF_SDH_DEBUG("MSG_A3MERGE_ROLLER_DETECTED\n");
				a3merge_set_rollerDetect(TRUE);
			break;
			
			/*
			******************************************************************************************
			* MSG_A3MERGE_NOTIFY_MERGE_END
			******************************************************************************************
			*/
			case MSG_A3MERGE_NOTIFY_MERGE_END:
				DBG_PRINTF_SDH_DEBUG("MSG_A3MERGE_NOTIFY_MERGE_END\n");
				if(msg.param1 == A3MERGE_STATUS_RollerDetected) {
					/**
					 * This routine is for finish the Merge job caused by Roller Detected
					 *
					 * \brief When roller detected send below message to 'ScanMan' for finishing the job, and free the memory
					 * 		-MSG_A3MERGE_NOTIFY_MERGE_END
					 * 		-SMSG_SCAN_END
					 * 		-SMSG_SCAN_READY
					 **/
					/* Send 'MSG_A3MERGE_NOTIFY_MERGE_END' to 'ScanMan' */
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = MSG_A3MERGE_NOTIFY_MERGE_END;
					send_msg.param1 = A3MERGE_STATUS_RollerDetected;
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
					XASSERT( err==OK, err );
					}
				
					//Send 'SMSG_SCAN_FAILED' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					send_msg.msgType = (SMSG_SCAN_FAILED | MESSAGE_FROM_SCANTASK_FLAG);					
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}

					//Send 'SMSG_SCAN_READY' to 'ScanMan'
					memset(&send_msg, 0x00,sizeof(MESSAGE));
					memcpy(&send_msg, &backupMsg_scan_ready, sizeof(MESSAGE));
					err = SYMsgSend( SCANMANID, &send_msg );
					if( err != OK ) {
						XASSERT( err==OK, err );
					}
					
					//free Memory
					if(imgL.data) {
						free(imgL.data);
						imgL.data = NULL;
					}
					if(imgR.data) {
						free(imgR.data);
						imgR.data = NULL;
					}
					if (first_page)
					{
						memFree(first_page);
						first_page = NULL;
					}
					if (second_page)
					{
						memFree(second_page);
						second_page = NULL;
					}
					if (merge_result_rotated_page)
					{
						memFree(merge_result_rotated_page);
						merge_result_rotated_page = NULL;
					}
					if (featurePointL)
					{
						free(featurePointL);
						featurePointL = NULL;
					}
					if (featurePointR)
					{
						free(featurePointR);
						featurePointR = NULL;
					}
					if (limiter)
					{
						mlimiter_stop(limiter);
						limiter = NULL;
					}
					
					scan_page = A3MERGE_NONE_PAGE;
				}
			break;

			default:
                DBG_PRINTF_SDH_ERR("scanman unknown message type=%#x param1=%#lx\n", msg.msgType, msg.param1);
			break;
		}
	}
	return 0;
}

int rotate_buffer(unsigned char *source, unsigned int source_width, unsigned int source_height, unsigned char *destination)
{
	//rotate image buffer +90 for Scan...(etc)
	rotate(source, destination, source_width, source_height);

	return 1;
}
 
void rotate(unsigned char *pS, unsigned char *pD, unsigned int row, unsigned int col)
{
	unsigned int r, c;
	unsigned int source_row = row;
	unsigned int source_col = col;
	unsigned int dest_row = col;

	// Rotate +90
	for(c = 0; c < source_col; c++)
	{
		for(r = 0; r < source_row; r++) 
		{
			*(pD + r * dest_row + (dest_row - c - 1 )) = *(pS + c * source_row + r);
		}
	}
}
