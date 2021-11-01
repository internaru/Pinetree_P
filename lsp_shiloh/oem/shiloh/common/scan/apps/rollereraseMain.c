
#ifdef __AGMESSAGE_H__
#undef __AGMESSAGE_H__
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "scantypes.h"
#include "scanmech.h"
#include "scanmsg.h"
#include "scanmem.h"
#include "rollereraseMain.h"
#include "rollereraseLib.h"
#include "a3mergeInterface.h"
#include "scanvars.h"
#include "db_api.h"

#define DBG_PRFX "[ERASE] "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN   4096
#endif
#ifndef PTHREAD_CANCELED
#define PTHREAD_CANCELED    0
#endif

#define ERASEROLLER_TASK_QUEUE_SIZE 200  ///< Size of the message queue.
#define ERASEROLLER_TASK_STACK_SIZE (PTHREAD_STACK_MIN*4) ///< Size of the stack.
#define MESSAGE_FROM_SCANTASK_FLAG (1<<31)

#define ERASE_ROLLER_MSGQUEUE_DEBUG	0
#define ERASE_ROLLER_DUMP			0
#define ERASE_ROLLER_ERASE_ROLLER	1
#define DEBUG_TEST					0

#define SEND_DATA_HEIGHT		64
#define SEND_DATA_HEIGHT_MIN	SEND_DATA_HEIGHT/2
#define SEND_DATA_HEIGHT_MAX	SEND_DATA_HEIGHT*2

ALIGN(8) static UINT8 eraseRoller_task_stack[ERASEROLLER_TASK_STACK_SIZE];
mqd_t eraseRoller_task_msgq;
static pthread_t eraseRoller_task_id;

static erase_roller_MainParam_t tEraseRollerMainParam;	//parameter for use in Main Erase Roller App
static erase_roller_LibParam_t	tEraseRollerLibParams;		//parameter for use in Lib (erase roller)
static struct scanvars *sv;
static uint32_t g_adjust_y_value;
static uint32_t g_host_app;
static uint32_t g_merge_paper_size;
static uint32_t g_first_strip_size;
static uint32_t g_first_roller_end;
static uint32_t g_second_roller_end;
static mlimiter_t* limiter = NULL;

#if ERASE_ROLLER_MSGQUEUE_DEBUG
static uint32_t msgQueue_index = 0;
#endif

/**
 * \brief Erase Roller Application Flow 
 * 
 * 		1.MSG_ERASE_ROLLER_SET_PARAM : ScanMan -> EraseRoller --> set_erase_roller_param()
 * 		2.SMSG_SCAN_SIZE             : ScanMan -> EraseRoller
 * 		3.SMSG_PAGE_DATA             : ScanMan -> EraseRoller 
 * 		4.MSG_ERASE_ROLLER_PAGE_DATA : EraseRoller -> ScanMan --> eraseRoller_send_pageData()
 */
 
 /**
 * \brief Roller Position of SINDOH's machine [Pinetree(-S)] 
 * 
 *
 **************************************************************************************************************************************************
 * Roller Position (only test : Younghoo Lee)
 * 		- dpi, 1st_roller_start, 1st_roller_width, 1st_roller_height, 2nd_roller_start, 2nd_roller_width, 2nd_roller_height
 * 			- 1st Roller : [9.5cm,   2cm,     2.5cm], 2nd Roller : [17cm,    2cm,     2.5cm]
 * 			- 1st Roller : [3.7inch, 0.8inch, 1inch], 2nd Roller : [6.7inch, 0.8inch, 1inch]
 * dpi
 * roller_position_1st_start, roller_position_1st_width, roller_position_1st_height
 * roller_position_2nd_start, roller_position_2nd_width, roller_position_2nd_height
 * roller_height, roller_width, pwl, pwr, checklength, inside, edgebound, blackpart, REF1_X, REF2_X, REF1_Y, REF2_Y, bgrbound, paperbound, roller_inside
 *************************************************************************************************************************************************
 */
 
static scan_flatbed_roller_position_t 	flatbed_roller_position_table[] = {
	{  75,		 290,  69,  84, 	 512,  69,  84, 		 84,69, 17, 19,	19,	13,	13,	13, 8,	8,	290, 512, 1, 1, 3},	// Line Number
	{ 100,		 387,  92, 112, 	 682,  92, 112, 		112,92,	22,	25,	25,	17,	17,	17,	11,	11,	387, 682, 1, 1, 4},	// Line Number
	{ 150,		 581, 137, 167, 	1024, 137, 167, 		167,137,31, 35,	35,	25,	25,	25,	16,	16,	581, 1024, 2, 2, 5},	// Line Number
	{ 200,		 774, 191, 223, 	1361, 191, 223, 		223,191,44, 50,	50,	33,	33,	33,	22,	22,	774, 1361, 2, 3, 7},	// Line Number
	{ 300,		1162, 274, 334, 	2048, 274, 334, 		334,274,61, 70,	70,	50,	50,	50,	33,	33,	1162, 2048, 3, 4, 10},	// Line Number
	{ 600,		2324, 548, 667, 	4096, 548, 667, 		667,548,131,150,150, 100, 100, 100, 66, 66, 2324, 4096, 6, 8, 20},	// Line Number
};



/**
 * \brief When Merge job, Notify Roller Detect to Merge Task
 *
 */
static scan_err_t eraseRoller_notify_rollerDect()
{
    error_type_t err;
	MESSAGE msg;
	memset(&msg, 0x00, sizeof(MESSAGE));
	
	msg.msgType = MSG_A3MERGE_ROLLER_DETECTED;
	err = SYMsgSend( A3_MERGE_ID, &msg );

	if( err != OK ) {
		XASSERT( err==OK, err );
	}
			
	return SCANERR_NONE;
}


/**
 * \brief Call Erase Roller Library
 *
 */
static int eraseRoller_erase_roller_execute( unsigned char *imageSrc, uint32_t imageSrcSize, uint8_t rollerNum )
{
	int eraseResult = ROLLER_DETECT_NONE;
	UINT8 channel_num = 0;
	UINT16	dataWidth = 0;
	UINT16	dataHeight = 0;
	UINT16 i;
#if ERASE_ROLLER_DUMP
	static int dump_no = 0;
#endif
	
	dataWidth = tEraseRollerMainParam.bytes_per_row;
	dataHeight = imageSrcSize / dataWidth;

	// Make 2d pointer of ScanData (will transfer to App)
	UINT8 **rollerImage = (UINT8 **)malloc(dataHeight * sizeof(UINT8 *));
//	UINT8 **rollerImage = (UINT8 **)scanmem_alloc(dataHeight);
	for(i = 0; i < dataHeight; i++)
	{
		rollerImage[i] = &imageSrc[i * dataWidth];
	}
	
	if(tEraseRollerMainParam.scanvar_cmode == SCAN_CMODE_MONO) {
		channel_num = 1;
	}
	else if(tEraseRollerMainParam.scanvar_cmode == SCAN_CMODE_COLOR) {
		channel_num = 3;
	}
	else {
		channel_num = 1;
	}

	DBG_PRINTF_SDH_DEBUG("SourceSize(%d Byte),Wid(%d),Hei(%d),ChannelNum(%d),RollerNum(#%d)\n", imageSrcSize, dataWidth, dataHeight, channel_num, rollerNum);

	//Execute Erase
	if(rollerNum == 0) {

#if ERASE_ROLLER_DUMP
		DBG_PRINTF_SDH_DEBUG("Dump #1 START\n");
		FILE *fs;
		char path[200];
		
		sprintf(path, "/mnt/sda1/first_%d_%d_%d_%d.raw",  ++dump_no, dataWidth, dataHeight, tEraseRollerMainParam.roller_1st_check_line_gaps);
		fs = fopen( path, "wb" );
		fwrite(imageSrc, 1, imageSrcSize, fs);
		fclose(fs);
		DBG_PRINTF_SDH_DEBUG("Dump #1 END\n");
#endif

#if ERASE_ROLLER_ERASE_ROLLER
		DBG_PRINTF_SDH_DEBUG("%s ERASE Roller Part #1 [X:%d, Y:%d]\n", __FUNCTION__, tEraseRollerMainParam.roller_start_col_index, tEraseRollerMainParam.roller_1st_check_line_gaps);	
		eraseResult = erase_roller_part(rollerImage, tEraseRollerMainParam.roller_start_col_index, tEraseRollerMainParam.roller_1st_check_line_gaps, channel_num, 
											dataWidth, dataHeight, &tEraseRollerLibParams);
#endif
	}
	else {
#if ERASE_ROLLER_DUMP
		DBG_PRINTF_SDH_DEBUG("Dump #2 START\n");
		FILE *fs;
		char path[200];
		
		sprintf(path, "/mnt/sda1/second_%d_%d_%d_%d.raw",  ++dump_no, dataWidth, dataHeight, tEraseRollerMainParam.roller_2nd_check_line_gaps);
		fs = fopen( path, "wb" );
		fwrite(imageSrc, 1, imageSrcSize, fs);
		fclose(fs);
		DBG_PRINTF_SDH_DEBUG("Dump #2 END\n");
#endif

#if ERASE_ROLLER_ERASE_ROLLER
		DBG_PRINTF_SDH_DEBUG("%s ERASE Roller Part #2 [X:%d, Y:%d]\n", __FUNCTION__, tEraseRollerMainParam.roller_start_col_index, tEraseRollerMainParam.roller_2nd_check_line_gaps);	
		eraseResult = erase_roller_part(rollerImage, tEraseRollerMainParam.roller_start_col_index, tEraseRollerMainParam.roller_2nd_check_line_gaps, channel_num, 
												dataWidth, dataHeight, &tEraseRollerLibParams);
#endif
	}
	
	free(rollerImage);

	DBG_PRINTF_SDH_DEBUG("%s ERASE Result is(%d)\n", __FUNCTION__, eraseResult);	

	if(tEraseRollerMainParam.scan_job_type == ERASE_JOB_TYPE_MERGE) {
		if(eraseResult == ROLLER_DETECT_TRUE) {
			eraseRoller_notify_rollerDect();
		}
	}

	return eraseResult;
}

static scan_err_t eraseRoller_get_FBMargin_PixelValue(unsigned int dpi, unsigned int color_mode, int *fb_margin_x_pixel, int *fb_margin_y_piexl)
{
	scan_err_t scerr = SCANERR_NONE;
	struct scan_flatbed_margins fb_margins;
	int leftMarginHinch = 0; 
	int topMarginHinch = 0;
	
	scerr = scanmech_get_fb_margins( dpi, color_mode, &fb_margins );
	if(scerr == SCANERR_NONE) {
		leftMarginHinch = fb_margins.left_margin_hinches_x;
		topMarginHinch = fb_margins.top_margin_hinches_y;
		
		*fb_margin_x_pixel = (leftMarginHinch * dpi)/100;
		*fb_margin_y_piexl = (topMarginHinch * dpi)/100;
		
		//DBG_PRINTF_SDH_DEBUG("%s DPI(%d), LEFT(%d), RIGHT(%d)\n", __FUNCTION__, dpi, *fb_margin_x_pixel, *fb_margin_y_piexl );
	}
	
	return scerr;
}

/**
 * \brief Change roller parameter in special mode (Legal Merge)
 *
 */
static scan_err_t eraseRoller_change_roller_check_position(scan_flatbed_roller_position_t flatbed_roller_position_table[], uint32_t merge_size, uint32_t dpi)
{
	int i;
	long int scan_start_y_hinch = 0;
	long int scan_height_hinch = 0;
	long int scan_start_y_pixel = 0;
	long int scan_end_y_pixel = 0;
	long int scan_height_pixel = 0;

	scan_start_y_hinch = sv->user_area_hinch.y;
	scan_height_hinch = sv->user_area_hinch.height;

	scan_start_y_pixel = (scan_start_y_hinch * dpi) / 100;
	scan_height_pixel = (scan_height_hinch * dpi) / 100;
	scan_end_y_pixel = scan_start_y_pixel + scan_height_pixel;

	DBG_PRINTF_SDH_DEBUG("scan_START POINT(Y) Inch(%d) pixel(%d)\n", scan_start_y_hinch, scan_start_y_pixel);
	DBG_PRINTF_SDH_DEBUG("scan___END POINT(Y) Inch(%d) pixel(%d)\n", scan_start_y_hinch+scan_height_hinch, scan_end_y_pixel);

	for (i=0 ; ; i++)
	{
		if (flatbed_roller_position_table[i].dpi==0) {
			return SCANERR_NO_ENTRY;
		}
		
		if (flatbed_roller_position_table[i].dpi==dpi) {
			/*
			 * 	Legal Merge Example
			 * 	1st scan area(height) :   0 ~  725 (hinch)	-> [300dpi] :    0 ~ 2175 (pixel)
			 * 	2nd scan area(height) : 445 ~ 1170 (hinch) 	-> [300dpi] : 1335 ~ 3510 (pixel)
			 */

			DBG_PRINTF_SDH_DEBUG("Roller_Check Index Orig: 1st_start(%d)\n", tEraseRollerMainParam.roller_1st_check_start_row_index);	
			DBG_PRINTF_SDH_DEBUG("Roller_Check Index Orig: 2nd_start(%d)\n", tEraseRollerMainParam.roller_2nd_check_start_row_index);	
										 
			//Scan Start Point Check
			if( (scan_start_y_pixel >= flatbed_roller_position_table[i].REF1_Y) && 
				(scan_start_y_pixel <= flatbed_roller_position_table[i].REF1_Y + flatbed_roller_position_table[i].roller_height) ) {
				tEraseRollerMainParam.roller_1st_check_start_row_index -= (flatbed_roller_position_table[i].roller_height * 4);
				
				DBG_PRINTF_SDH_DEBUG("Roller_Check Index Change: 1st_start(%d)\n", tEraseRollerMainParam.roller_1st_check_start_row_index);	
			}
			
			if( (scan_start_y_pixel >= flatbed_roller_position_table[i].REF2_Y) &&
				(scan_start_y_pixel <= flatbed_roller_position_table[i].REF2_Y + flatbed_roller_position_table[i].roller_height) ) {
				tEraseRollerMainParam.roller_1st_check_start_row_index -= (flatbed_roller_position_table[i].roller_height * 4);

				DBG_PRINTF_SDH_DEBUG("Roller_Check Index Change: 1st_start(%d)\n", tEraseRollerMainParam.roller_1st_check_start_row_index);
			}

			//Scan End Point Check
			if( (scan_end_y_pixel >= flatbed_roller_position_table[i].REF1_Y) && 
				(scan_end_y_pixel <= flatbed_roller_position_table[i].REF1_Y + flatbed_roller_position_table[i].roller_height) ) {
				tEraseRollerMainParam.roller_2nd_check_start_row_index += (flatbed_roller_position_table[i].roller_height * 4);

				DBG_PRINTF_SDH_DEBUG("Roller_Check Index Change: 2nd_start(%d)\n", tEraseRollerMainParam.roller_2nd_check_start_row_index);	
			}
			
			if( (scan_end_y_pixel >= flatbed_roller_position_table[i].REF2_Y) &&
				(scan_end_y_pixel <= flatbed_roller_position_table[i].REF2_Y + flatbed_roller_position_table[i].roller_height) ) {
				tEraseRollerMainParam.roller_2nd_check_start_row_index += (flatbed_roller_position_table[i].roller_height * 4);

				DBG_PRINTF_SDH_DEBUG("Roller_Check Index Change: 2nd_start(%d)\n", tEraseRollerMainParam.roller_2nd_check_start_row_index);	
			}
			break;
		}
	}

    return SCANERR_NONE;	
}

/**
 * \brief Set the roller library parameter.
 *
 */
static scan_err_t eraseRoller_set_roller_libParam( scan_flatbed_roller_position_t flatbed_roller_position_table[], unsigned int dpi, unsigned int color_mode)
{
	int i;
	int x_num, x_denom, y_num, y_denom;
	int leftMarginPixel = 0;
	int topMarginPixel = 0;
	
	scanvar_get_xyscale(sv, &x_num, &x_denom, &y_num, &y_denom);
	eraseRoller_get_FBMargin_PixelValue(dpi, color_mode, &leftMarginPixel, &topMarginPixel);
	
	for (i=0 ; ; i++)
	{
		if (flatbed_roller_position_table[i].dpi==0) {
			return SCANERR_NO_ENTRY;
		}	

		if (flatbed_roller_position_table[i].dpi==dpi) {
			//setup lib param
			tEraseRollerLibParams.roller_height	= flatbed_roller_position_table[i].roller_height;
			tEraseRollerLibParams.roller_width		= flatbed_roller_position_table[i].roller_width;
			tEraseRollerLibParams.pwl				= flatbed_roller_position_table[i].pwl;
			tEraseRollerLibParams.pwr				= flatbed_roller_position_table[i].pwr;			
			tEraseRollerLibParams.checklength		= flatbed_roller_position_table[i].checklength;
			tEraseRollerLibParams.inside			= flatbed_roller_position_table[i].inside;
			tEraseRollerLibParams.edgebound		= flatbed_roller_position_table[i].edgebound;
			tEraseRollerLibParams.blackpart		= flatbed_roller_position_table[i].blackpart;
			tEraseRollerLibParams.REF1_X 			= flatbed_roller_position_table[i].REF1_X;
			tEraseRollerLibParams.REF2_X 			= flatbed_roller_position_table[i].REF2_X;
			tEraseRollerLibParams.REF1_Y 			= flatbed_roller_position_table[i].REF1_Y;
			tEraseRollerLibParams.REF2_Y 			= flatbed_roller_position_table[i].REF2_Y;
			tEraseRollerLibParams.bgrbound 		= flatbed_roller_position_table[i].bgrbound;
			tEraseRollerLibParams.paperbound 		= flatbed_roller_position_table[i].paperbound;
			tEraseRollerLibParams.roller_inside		= flatbed_roller_position_table[i].roller_inside;
			break;
		}
	}
	
	if (y_num != y_denom)
	{
			tEraseRollerLibParams.roller_height	= (tEraseRollerLibParams.roller_height*y_num)/y_denom;
			tEraseRollerLibParams.roller_width		= (tEraseRollerLibParams.roller_width*y_num)/y_denom;
			tEraseRollerLibParams.pwl				= (tEraseRollerLibParams.pwl*y_num)/y_denom;
			tEraseRollerLibParams.pwr				= (tEraseRollerLibParams.pwr*y_num)/y_denom;
			tEraseRollerLibParams.checklength		= (tEraseRollerLibParams.checklength*y_num)/y_denom;
			tEraseRollerLibParams.inside			= (tEraseRollerLibParams.inside*y_num)/y_denom;	
			tEraseRollerLibParams.edgebound		= (tEraseRollerLibParams.edgebound*y_num)/y_denom;
			tEraseRollerLibParams.blackpart		= (tEraseRollerLibParams.blackpart*y_num)/y_denom;
			tEraseRollerLibParams.REF1_X 			= (tEraseRollerLibParams.REF1_X*y_num)/y_denom;
			tEraseRollerLibParams.REF2_X 			= (tEraseRollerLibParams.REF2_X*y_num)/y_denom;
			tEraseRollerLibParams.REF1_Y 			= (tEraseRollerLibParams.REF1_Y*y_num)/y_denom;
			tEraseRollerLibParams.REF2_Y 			= (tEraseRollerLibParams.REF2_Y*y_num)/y_denom;
			tEraseRollerLibParams.bgrbound 		= (tEraseRollerLibParams.bgrbound*y_num)/y_denom;
			tEraseRollerLibParams.paperbound 		= (tEraseRollerLibParams.paperbound*y_num)/y_denom;
			tEraseRollerLibParams.roller_inside		= (tEraseRollerLibParams.roller_inside*y_num)/y_denom;
	}
	
	if (g_adjust_y_value)
	{
		tEraseRollerLibParams.REF1_Y -= g_adjust_y_value;
		tEraseRollerLibParams.REF2_Y -= g_adjust_y_value;
	}

	if(g_host_app == e_HOST_SCAN)
	{
		tEraseRollerLibParams.thr1 = 120;
		tEraseRollerLibParams.thr2 = 80;
		tEraseRollerLibParams.thr3 = 45;
#if CHECK_ROLLER_INSIDE
		tEraseRollerLibParams.thr4 = 220;
		//tEraseRollerLibParams.thr5 = 40;
#endif
	}
	else //if (g_host_app == e_HOST_COPY)
	{
		tEraseRollerLibParams.thr1 = 120;
		tEraseRollerLibParams.thr2 = 80;
		tEraseRollerLibParams.thr3 = 45;
#if CHECK_ROLLER_INSIDE
		tEraseRollerLibParams.thr4 = 220;
		//tEraseRollerLibParams.thr5 = 40;
#endif
	}
	
	DBG_PRINTF_SDH_DEBUG("Roller LibParam =========================================================\n");
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  DPI (%d)\n", 	  flatbed_roller_position_table[i].dpi);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  rescale %d/%d\n", y_num, y_denom );
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  roller_height : %d\n", tEraseRollerLibParams.roller_height);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  roller_width  : %d\n", tEraseRollerLibParams.roller_width);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  pwl           : %d\n", tEraseRollerLibParams.pwl);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  pwr           : %d\n", tEraseRollerLibParams.pwr);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  checklength   : %d\n", tEraseRollerLibParams.checklength);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  inside        : %d\n", tEraseRollerLibParams.inside);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  edgebound     : %d\n", tEraseRollerLibParams.edgebound);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  blackpart     : %d\n", tEraseRollerLibParams.blackpart);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  REF1_X        : %d\n", tEraseRollerLibParams.REF1_X);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  REF2_X        : %d\n", tEraseRollerLibParams.REF2_X);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  REF1_Y        : %d\n", tEraseRollerLibParams.REF1_Y);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  REF2_Y        : %d\n", tEraseRollerLibParams.REF2_Y);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  bgrbound      : %d\n", tEraseRollerLibParams.bgrbound);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  paperbound    : %d\n", tEraseRollerLibParams.paperbound);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  thr1          : %d\n", tEraseRollerLibParams.thr1);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  thr2          : %d\n", tEraseRollerLibParams.thr2);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  thr3          : %d\n", tEraseRollerLibParams.thr3);
#if CHECK_ROLLER_INSIDE
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  thr4          : %d\n", tEraseRollerLibParams.thr4);
	//DBG_PRINTF_SDH_DEBUG("Roller LibParam  thr5          : %d\n", tEraseRollerLibParams.thr5);
	DBG_PRINTF_SDH_DEBUG("Roller LibParam  roller_inside : %d\n", tEraseRollerLibParams.roller_inside);
#endif

	return SCANERR_NONE;
}


/**
 * \brief Get the roller position.
 *
 */
static scan_err_t eraseRoller_get_roller_position( UINT32 scanJobType, scan_flatbed_roller_position_t flatbed_roller_position_table[], unsigned int dpi, unsigned int color_mode)
{
	int i;
	int x_num, x_denom, y_num, y_denom;
	int leftMarginPixel = 0;
	int topMarginPixel = 0;
	
	scanvar_get_xyscale(sv, &x_num, &x_denom, &y_num, &y_denom);
	eraseRoller_get_FBMargin_PixelValue(dpi, color_mode, &leftMarginPixel, &topMarginPixel);
		
	for (i=0 ; ; i++)
	{
		if (flatbed_roller_position_table[i].dpi==0) {
			return SCANERR_NO_ENTRY;
		}

		if (flatbed_roller_position_table[i].dpi==dpi) {
			tEraseRollerMainParam.roller_1st_check_start_row_index	= flatbed_roller_position_table[i].roller_position_1st_start;
			tEraseRollerMainParam.roller_1st_check_line_nums		= flatbed_roller_position_table[i].roller_position_1st_height;
			tEraseRollerMainParam.roller_1st_check_end_row_index	= flatbed_roller_position_table[i].roller_position_1st_start + flatbed_roller_position_table[i].roller_position_1st_height;
			tEraseRollerMainParam.roller_2nd_check_start_row_index	= flatbed_roller_position_table[i].roller_position_2nd_start;
			tEraseRollerMainParam.roller_2nd_check_line_nums		= flatbed_roller_position_table[i].roller_position_2nd_height;
			tEraseRollerMainParam.roller_2nd_check_end_row_index	= flatbed_roller_position_table[i].roller_position_2nd_start + flatbed_roller_position_table[i].roller_position_2nd_height;
			tEraseRollerMainParam.roller_start_col_index			= flatbed_roller_position_table[i].REF1_X;

			//Change Roller's Parameter Table (Only in Legal Merge)
			if((tEraseRollerMainParam.scan_job_type == ERASE_JOB_TYPE_MERGE) && (g_merge_paper_size == A3MERGE_LG)) {
				eraseRoller_change_roller_check_position(flatbed_roller_position_table, g_merge_paper_size, dpi);
			}
			break;
		}
	}
	
	if (y_num != y_denom)
	{
		tEraseRollerMainParam.roller_1st_check_start_row_index = (tEraseRollerMainParam.roller_1st_check_start_row_index*y_num)/y_denom;
		tEraseRollerMainParam.roller_1st_check_line_nums = (tEraseRollerMainParam.roller_1st_check_line_nums*y_num)/y_denom;
		tEraseRollerMainParam.roller_1st_check_end_row_index = (tEraseRollerMainParam.roller_1st_check_end_row_index*y_num)/y_denom;
		tEraseRollerMainParam.roller_2nd_check_start_row_index = (tEraseRollerMainParam.roller_2nd_check_start_row_index*y_num)/y_denom;
		tEraseRollerMainParam.roller_2nd_check_line_nums = (tEraseRollerMainParam.roller_2nd_check_line_nums*y_num)/y_denom;
		tEraseRollerMainParam.roller_2nd_check_end_row_index = (tEraseRollerMainParam.roller_2nd_check_end_row_index*y_num)/y_denom;
		tEraseRollerMainParam.roller_start_col_index = (tEraseRollerMainParam.roller_start_col_index*y_num)/y_denom;
	}
	
	if (g_adjust_y_value)
	{
		tEraseRollerMainParam.roller_1st_check_start_row_index -= g_adjust_y_value;
		tEraseRollerMainParam.roller_1st_check_end_row_index -= g_adjust_y_value;
		tEraseRollerMainParam.roller_2nd_check_start_row_index -= g_adjust_y_value;
		tEraseRollerMainParam.roller_2nd_check_end_row_index -= g_adjust_y_value;
	}

	if ((scanJobType != ERASE_JOB_TYPE_MERGE) && (g_host_app == e_HOST_COPY))
		tEraseRollerMainParam.roller_start_col_index = 0;

	DBG_PRINTF_SDH_DEBUG("Roller Position =========================================================\n");	
	DBG_PRINTF_SDH_DEBUG("Roller Position [%d]DPI\n", flatbed_roller_position_table[i].dpi);
	DBG_PRINTF_SDH_DEBUG("Roller JobType %d, HostApp %d\n", scanJobType, g_host_app);
	DBG_PRINTF_SDH_DEBUG("Roller Position rescale %d/%d, adjust_y %d\n", y_num, y_denom, g_adjust_y_value );
	DBG_PRINTF_SDH_DEBUG("Roller Position #1 Start : %d\n", tEraseRollerMainParam.roller_1st_check_start_row_index);
	DBG_PRINTF_SDH_DEBUG("Roller Position #1 End   : %d\n", tEraseRollerMainParam.roller_1st_check_end_row_index);
	DBG_PRINTF_SDH_DEBUG("Roller Position #2 Start : %d\n", tEraseRollerMainParam.roller_2nd_check_start_row_index);
	DBG_PRINTF_SDH_DEBUG("Roller Position #2 End   : %d\n", tEraseRollerMainParam.roller_2nd_check_end_row_index);
		
    return SCANERR_NONE;
}

/**
\brief eraseRoller_send_pageData
 *	parameter :  
 *	In Here we send the modified scan data into 'ScanMan' (or another destination)
 */
static scan_err_t eraseRoller_send_pageData(scan_data_type dataType, uint16_t num_rows, unsigned char *data, bool is_last_buffer, uint8_t page_side) 
{
    error_type_t err;
	MESSAGE send_msg;	

	int remain_height = 0;
	int send_height = 0;
	int data_width = 0;
	int send_strip_size = 0;

	data_width = tEraseRollerMainParam.bytes_per_row;
	remain_height = num_rows;
	unsigned char *send_plane_buffer;

	send_strip_size = g_first_strip_size;
	do
	{
		if (((num_rows%send_strip_size) > SEND_DATA_HEIGHT_MIN) || ((num_rows%send_strip_size)==0))
			break;
		send_strip_size++;
	}while(send_strip_size<SEND_DATA_HEIGHT_MAX);

	DBG_PRINTF_SDH_ERR( "%s send_strip_size = %d(%d rows, FSS %d)\n", __func__, send_strip_size, num_rows, g_first_strip_size);
	
	memset(&send_msg, 0x00,sizeof(MESSAGE));
	send_msg.msgType = MSG_ERASE_ROLLER_PAGE_DATA;

	while(remain_height)
	{									
		if (remain_height > send_strip_size)
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(send_strip_size); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dataType) | SMSG_DATA_SET_PAGE_SIDE(page_side);
			send_plane_buffer = scanmem_alloc(data_width * send_strip_size);

			if (send_plane_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (data_width * send_strip_size), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			
			memcpy(send_plane_buffer, data+(data_width*send_height), data_width * send_strip_size);
			remain_height -= send_strip_size;
			send_height += send_strip_size;
		}
		else
		{
			send_msg.param1 = SMSG_DATA_SET_NUM_ROWS(remain_height); 
			send_msg.param2 = SMSG_DATA_SET_DATATYPE(dataType) | SMSG_DATA_SET_PAGE_SIDE(page_side);
			if(is_last_buffer) {
				send_msg.param2 |= SMSG_DATA_LAST_BUFFER_FLAG;
			}
			send_plane_buffer = scanmem_alloc(data_width * remain_height);
			
			if (send_plane_buffer==NULL)
			{
				DBG_PRINTF_SDH_ERR( "%s out of memory num_bytes=%d(%d/%d)\n", __func__, (data_width * remain_height), scanmem_get_heap_size(), scanmem_get_free_size() );
				return SCANERR_OUT_OF_MEMORY;
			}
			
			memcpy(send_plane_buffer, data+(data_width*send_height), data_width * remain_height);
			send_height += remain_height;
			remain_height = 0;
		}
		
		send_msg.param3 = send_plane_buffer;

		if(tEraseRollerMainParam.scan_job_type == ERASE_JOB_TYPE_NORMAL) {
			err = SYMsgSend( SCANMANID, &send_msg );
		}
		else {
			send_msg.msgType = (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG);
			err = SYMsgSend( A3_MERGE_ID, &send_msg );
		}
		if( err != OK ) {
			XASSERT( err==OK, err );
		}

#if ERASE_ROLLER_MSGQUEUE_DEBUG
		DBG_PRINTF_SDH_DEBUG("MSGQUEUE_INDEX(%d) - SEND\n", --msgQueue_index);
#endif
	}
	
	memFree(data);

	return 0;

}

static scan_err_t eraseRoller_send_pageData_bypass(scan_data_type dataType, uint16_t num_rows, unsigned char *data, bool is_last_buffer, uint8_t page_side) 
{
    error_type_t err;
	MESSAGE send_msg;	

	memset(&send_msg, 0x00,sizeof(MESSAGE));
	send_msg.msgType = MSG_ERASE_ROLLER_PAGE_DATA;

	send_msg.param1 = num_rows;
	send_msg.param2 = SMSG_DATA_SET_DATATYPE(dataType) | SMSG_DATA_SET_PAGE_SIDE(page_side);
	if(is_last_buffer) {
		send_msg.param2 |= SMSG_DATA_LAST_BUFFER_FLAG;
	}
	send_msg.param3 = data;

	if(tEraseRollerMainParam.scan_job_type == ERASE_JOB_TYPE_NORMAL) {
		err = SYMsgSend( SCANMANID, &send_msg );
	}
	else {
		send_msg.msgType = (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG);
		err = SYMsgSend( A3_MERGE_ID, &send_msg );
	}
	if( err != OK ) {
		XASSERT( err==OK, err );
	}

#if ERASE_ROLLER_MSGQUEUE_DEBUG
	DBG_PRINTF_SDH_DEBUG("MSGQUEUE_INDEX(%d) - BYPASS\n", --msgQueue_index);
#endif
	
	return 0;
}

static int eraseRoller_receive_strip_data(scan_data_type dataType, uint16_t num_rows, unsigned char *data, bool is_last_buffer, uint8_t pageSide,
																			uint8_t *firstRoller_check_data, uint8_t *secondRoller_check_data)
{
	unsigned int received_strip_data_size = 0;
	unsigned int saved_1st_roller_chcker_rows = 0;
	unsigned int saved_2nd_roller_chcker_rows = 0;
	unsigned int current_row_index = 0;
	unsigned int next_row_index = 0;
	unsigned int prev_state = 0;
	
	received_strip_data_size = num_rows*tEraseRollerMainParam.bytes_per_row;
	current_row_index = tEraseRollerMainParam.roller_check_current_row_index;
	next_row_index = current_row_index + num_rows;
	prev_state = tEraseRollerMainParam.roller_check_current_mode;	
	
	if (!g_first_strip_size)
		g_first_strip_size = num_rows;
	/// 2. Check index of received scan data
	//setup the Current Mode
	//Step 1 Area
	if( current_row_index < tEraseRollerMainParam.roller_1st_check_start_row_index )
	{
		if(next_row_index >= tEraseRollerMainParam.roller_1st_check_start_row_index) {
			//DBG_PRINTF_SDH_DEBUG("[MODE] Change : WAIT_1ST_ROLLER -> START_1ST_ROLLER (Index:%d)\n", current_row_index);
			tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_START_1ST_ROLLER;
			tEraseRollerMainParam.roller_1st_check_line_gaps = tEraseRollerMainParam.roller_1st_check_start_row_index - current_row_index;
		}
		else {
			tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_WAIT_1ST_ROLLER;
		}
	}
	//Step 2 Area
	else if( (current_row_index >= tEraseRollerMainParam.roller_1st_check_start_row_index) && (current_row_index <= tEraseRollerMainParam.roller_1st_check_end_row_index) )
	{		
		tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_START_1ST_ROLLER;
	}
	//Step 3 Area	
	else if( (current_row_index > tEraseRollerMainParam.roller_1st_check_end_row_index) && (current_row_index < tEraseRollerMainParam.roller_2nd_check_start_row_index) )
	{
		if(prev_state == ROLLER_ERASE_CURRENT_MODE_START_1ST_ROLLER) {
			//DBG_PRINTF_SDH_DEBUG("[MODE] Change : START_1ST_ROLLER -> END_1ST_ROLLER (Index:%d)\n", current_row_index);
			tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_END_1ST_ROLLER;			
		}
		else {
			if(next_row_index >= tEraseRollerMainParam.roller_2nd_check_start_row_index) {
				//DBG_PRINTF_SDH_DEBUG("[MODE] Change : END_1ST_ROLLER -> START_2ND_ROLLER (Index:%d)\n", current_row_index);					
				tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_START_2ND_ROLLER;
				tEraseRollerMainParam.roller_2nd_check_line_gaps = tEraseRollerMainParam.roller_2nd_check_start_row_index - current_row_index;
			}
			else {
				tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_WAIT_2ND_ROLLER;
			}
		}
	}
	//Step 4 Area
	else if( (current_row_index >= tEraseRollerMainParam.roller_2nd_check_start_row_index) && (current_row_index <= tEraseRollerMainParam.roller_2nd_check_end_row_index) )
	{
		tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_START_2ND_ROLLER;
	}
	//Step 5 Area
	else if( (current_row_index > tEraseRollerMainParam.roller_2nd_check_end_row_index) )
	{
		if(prev_state == ROLLER_ERASE_CURRENT_MODE_START_2ND_ROLLER) {
			//DBG_PRINTF_SDH_DEBUG("[MODE] Change : START_2ND_ROLLER -> END_2ND_ROLLER (Index:%d)\n", current_row_index);					
			tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_END_2ND_ROLLER;
		}
		else {
			tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_NORMAL;
		}		
	}
	else {
		tEraseRollerMainParam.roller_check_current_mode = ROLLER_ERASE_CURRENT_MODE_NORMAL;
	}
	
	//DBG_PRINTF_SDH_DEBUG("[MODE](%d) : current_row[%d]\n", tEraseRollerMainParam.roller_check_current_mode, current_row_index);
	

	/// 3. Copy memory from message into local memory (free original scan data)
	if( (tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_START_1ST_ROLLER) ||
		(tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_END_1ST_ROLLER) ) {
		/*
		 ***************************************
		 * 1st Roller 
		 ***************************************
		 */
		if(firstRoller_check_data) {
			memcpy(firstRoller_check_data + tEraseRollerMainParam.roller_1st_buffer_save_point, data, received_strip_data_size);
			tEraseRollerMainParam.roller_1st_buffer_save_point += received_strip_data_size;
			memFree(data);
			
			//erase 1st Roller Image
#if DEBUG_TEST
			DBG_PRINTF_SDH_DEBUG("[Roller #1] Stack START! RowIndex(%d), RcvRows(%d), SavePoint(%d)\n", 
												current_row_index, num_rows, tEraseRollerMainParam.roller_1st_buffer_save_point);
#endif
											
			if(tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_END_1ST_ROLLER) {

				//erase 1st Roller Image
				DBG_PRINTF_SDH_DEBUG("[Roller #1] Stack ENDED! RowIndex(%d), RcvRows(%d), SavePoint(%d)\n", 
												next_row_index, num_rows, tEraseRollerMainParam.roller_1st_buffer_save_point);
												
				eraseRoller_erase_roller_execute(firstRoller_check_data, tEraseRollerMainParam.roller_1st_buffer_save_point, 0);

				//send modified data to 'ScanMan' (or another destination)
				saved_1st_roller_chcker_rows = tEraseRollerMainParam.roller_1st_buffer_save_point/tEraseRollerMainParam.bytes_per_row;		 
				eraseRoller_send_pageData(dataType, saved_1st_roller_chcker_rows, firstRoller_check_data, is_last_buffer, pageSide);
				g_first_roller_end = TRUE;
			}
		}
		else {
			//send data to 'ScanMan' (or another destination)
			eraseRoller_send_pageData_bypass(dataType, num_rows, data, is_last_buffer, pageSide);
		}
	}
	else if( (tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_START_2ND_ROLLER) ||
			(tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_END_2ND_ROLLER) ) {
		/*
		 ***************************************
		 * 2nd Roller 
		 ***************************************
		 */
		if(secondRoller_check_data) {
			memcpy(secondRoller_check_data + tEraseRollerMainParam.roller_2nd_buffer_save_point, data, received_strip_data_size);		
			tEraseRollerMainParam.roller_2nd_buffer_save_point += received_strip_data_size;
			memFree(data);

#if DEBUG_TEST
			DBG_PRINTF_SDH_DEBUG("[Roller #2] Stack START! RowIndex(%d), RcvRows(%d), SavePoint(%d)\n", 
												current_row_index, num_rows, tEraseRollerMainParam.roller_2nd_buffer_save_point);
#endif

			if(tEraseRollerMainParam.roller_check_current_mode == ROLLER_ERASE_CURRENT_MODE_END_2ND_ROLLER) {
				
				//erase 2nd Roller Image
				DBG_PRINTF_SDH_DEBUG("[Roller #2] Stack ENDED! RowIndex(%d), RcvRows(%d), SavePoint(%d)\n", 	
												next_row_index, num_rows, tEraseRollerMainParam.roller_2nd_buffer_save_point);
												
				eraseRoller_erase_roller_execute(secondRoller_check_data, tEraseRollerMainParam.roller_2nd_buffer_save_point, 1);

				//send modified data to 'ScanMan' (or another destination)
				saved_2nd_roller_chcker_rows = tEraseRollerMainParam.roller_2nd_buffer_save_point/tEraseRollerMainParam.bytes_per_row;
				eraseRoller_send_pageData(dataType, saved_2nd_roller_chcker_rows, secondRoller_check_data, is_last_buffer, pageSide);
				g_second_roller_end = TRUE;
			}
		}
		else {
			//send data to 'ScanMan' (or another destination)
			eraseRoller_send_pageData_bypass(dataType, num_rows, data, is_last_buffer, pageSide);
		}
	}
	else {
		/*
		 ***************************************
		 * Not Roller 
		 ***************************************
		 */
		//send data to 'ScanMan' (or another destination)
#if DEBUG_TEST
		DBG_PRINTF_SDH_DEBUG("[Roller NO] ------ RowIndex(%d), RcvRows(%d)\n", current_row_index, num_rows);
#endif
		eraseRoller_send_pageData_bypass(dataType, num_rows, data, is_last_buffer, pageSide);			
	}
	
	tEraseRollerMainParam.roller_check_current_row_index += num_rows;
	
	return 0;
}

static int eraseRoller_get_rollerImageMemSize(UINT32 scanJobType, uint32_t *firstRollerMemSize, uint32_t *secondRollerMemSize)
{
	*firstRollerMemSize = tEraseRollerMainParam.bytes_per_row * (tEraseRollerMainParam.roller_1st_check_line_nums + 64 * 4);
	*secondRollerMemSize = tEraseRollerMainParam.bytes_per_row * (tEraseRollerMainParam.roller_2nd_check_line_nums + 64 * 4);
	
	DBG_PRINTF_SDH_DEBUG("Roller Image MemSize[#1]:%d, W(%d),H(%d)\n", 
								*firstRollerMemSize, tEraseRollerMainParam.bytes_per_row, tEraseRollerMainParam.roller_1st_check_line_nums + 64 * 4);
	DBG_PRINTF_SDH_DEBUG("Roller Image MemSize[#2]:%d, W(%d),H(%d)\n", 
								*secondRollerMemSize, tEraseRollerMainParam.bytes_per_row, tEraseRollerMainParam.roller_2nd_check_line_nums + 64 * 4);
	return 0;
}

static scan_err_t eraseRoller_setup_parameter(UINT32 scanJobType, UINT32 dpi, UINT32 color_mode)
{
	memset(&tEraseRollerMainParam, 0x00, sizeof(erase_roller_MainParam_t));
	memset(&tEraseRollerLibParams, 0x00, sizeof(erase_roller_LibParam_t));
				
	//Setup Main Variable
	tEraseRollerMainParam.scan_job_type = scanJobType;
	tEraseRollerMainParam.scanvar_dpi = dpi;
	tEraseRollerMainParam.scanvar_cmode = color_mode;
	
	DBG_PRINTF_SDH_DEBUG("tEraseRollerMainParam.jobType       : %d\n", tEraseRollerMainParam.scan_job_type);
	DBG_PRINTF_SDH_DEBUG("tEraseRollerMainParam.scanvar_dpi   : %d\n", tEraseRollerMainParam.scanvar_dpi);
	DBG_PRINTF_SDH_DEBUG("tEraseRollerMainParam.scanvar_cmode : %d\n", tEraseRollerMainParam.scanvar_cmode);
	
	//Get Roller Positon
	eraseRoller_get_roller_position(scanJobType, flatbed_roller_position_table, dpi, color_mode);

	//Setup Lib Variable
	eraseRoller_set_roller_libParam(flatbed_roller_position_table, dpi, color_mode);
		
    return SCANERR_NONE;
}

void* eraseRoller_task(void *unused)
{
    error_type_t err;
	MESSAGE rcv_msg;
	uint32_t pxretcode;
	uint32_t scanJobType=0;
	uint32_t scanvar_dpi;
	uint32_t scanvar_cmode;
	uint8_t *scan_data;

	uint32_t firstRollerMemSize = 0;
	uint32_t secondRollerMemSize = 0;

	unsigned char *firstRoller_check_data = NULL;	//this memory will free in receiver (Do not free Here!)
	unsigned char *secondRoller_check_data = NULL;	//this memory will free in receiver (Do not free Here!)

	bool last_buffer;
	uint16_t num_rows = 0;
	scan_data_type dtype = SCAN_DATA_TYPE_RGB;
	uint8_t page_side;
	
	while (1)
	{
		// get a message
		pxretcode = posix_wait_for_message(eraseRoller_task_msgq, (char *)&rcv_msg, sizeof(MESSAGE), POSIX_WAIT_FOREVER);
		XASSERT(pxretcode == 0, pxretcode);

		switch (rcv_msg.msgType)
		{
			/*
			****************************************************************************************************************
			* MSG_A3MERGE_SET_START (Received From 'App' -> 'ScanMan') Only for Merge Job
			****************************************************************************************************************
			*/
			case MSG_A3MERGE_SET_START:
				g_merge_paper_size = rcv_msg.param2; // A3MERGE_A3, A3MERGE_DLT, A3MERGE_LG
				DBG_PRINTF_SDH_ERR("setting merge jobtype is (%d)\n", g_merge_paper_size);
				break;
			
			/*
			****************************************************************************************************************
			* MSG_ERASE_ROLLER_SET_PARAM (Received From 'ScanMan')
			****************************************************************************************************************
			*/
			case MSG_ERASE_ROLLER_SET_PARAM:
				scanJobType 		= rcv_msg.param1;
				g_adjust_y_value	= rcv_msg.param2;
				sv 					= rcv_msg.param3;
				
				//Setup Parameter
				scanvar_dpi = sv->dpi;
				scanvar_cmode = sv->cmode;	

				if ((sv->id >= 200) && (sv->id < 300))
					g_host_app = e_HOST_COPY;
				else if ((sv->id >= 300) && (sv->id < 400))
					g_host_app = e_HOST_SCAN;
				else if ((sv->id >= 400) && (sv->id < 500))
					g_host_app = e_HOST_FAX;
				else
					DBG_PRINTF_SDH_ERR("setting host app failed : 1st\n");
					
				eraseRoller_setup_parameter(scanJobType, scanvar_dpi, scanvar_cmode);
				
				//Turn on limiter		
				limiter = mlimiter_by_name("scan");
				mlimiter_start(limiter);
				break;
			/*
			****************************************************************************************************************
			* SMSG_SCAN_SIZE (Received From 'ScanMan')
			****************************************************************************************************************
			*/
			case SMSG_SCAN_SIZE:
				msg_scan_size_decode(	&rcv_msg, 
											&tEraseRollerMainParam.pixels_per_row_padded, 
											&tEraseRollerMainParam.pixels_per_row, 
											&tEraseRollerMainParam.total_rows, 
											&tEraseRollerMainParam.BitsPerPixel 
										);
										
				tEraseRollerMainParam.bytes_per_row = (tEraseRollerMainParam.BitsPerPixel/8) * tEraseRollerMainParam.pixels_per_row_padded;

				/// 1. Malloc memory for erase roller image
				eraseRoller_get_rollerImageMemSize(scanJobType, &firstRollerMemSize, &secondRollerMemSize);

				//1st Roller Area Data	
				if(tEraseRollerMainParam.total_rows < tEraseRollerMainParam.roller_1st_check_start_row_index) {
					DBG_PRINTF_SDH_DEBUG("Roller #1 Memory Do not need, not malloc. TotalRow(%d),#1 RollerStart(%d)\n", 
													tEraseRollerMainParam.total_rows, tEraseRollerMainParam.roller_1st_check_start_row_index);
				}
				else {
					firstRoller_check_data = scanmem_alloc(firstRollerMemSize);
					if (firstRoller_check_data == NULL) {
						DBG_PRINTF_SDH_ERR("MEM_MALLOC failed : 1st\n");
					}
					else {
						DBG_PRINTF_SDH_DEBUG("1st EraseArea MALLOCED, ptr(%d), size is:%d\n", firstRoller_check_data, firstRollerMemSize);
						memset(firstRoller_check_data, 0xff, firstRollerMemSize);
						g_first_roller_end = FALSE;
					}
				}
				
				//2nd Roller Area Data	
				if(tEraseRollerMainParam.total_rows < tEraseRollerMainParam.roller_2nd_check_start_row_index) {
					DBG_PRINTF_SDH_DEBUG("Roller #2 Memory Do not need, not malloc. TotalRow(%d),#2 RollerStart(%d)\n", 
													tEraseRollerMainParam.total_rows, tEraseRollerMainParam.roller_2nd_check_start_row_index);
				}
				else {
					secondRoller_check_data = scanmem_alloc(secondRollerMemSize);
					if (secondRoller_check_data == NULL) {
						DBG_PRINTF_SDH_ERR("MEM_MALLOC failed : 2nd\n");
					}
					else {
						DBG_PRINTF_SDH_DEBUG("2nd EraseArea MALLOCED, ptr(%d), size is:%d\n", secondRoller_check_data, secondRollerMemSize);
						memset(secondRoller_check_data, 0xff, secondRollerMemSize);
						g_second_roller_end = FALSE;
					}
				}

#if DEBUG_TEST
				eraseRoller_erase_roller_execute(firstRoller_check_data, firstRollerMemSize, 0);
#endif
				break;

			/*
			****************************************************************************************************************
			* SMSG_PAGE_START (Received From 'ScanMan')
			****************************************************************************************************************
			*/
			case (SMSG_PAGE_START | MESSAGE_FROM_SCANTASK_FLAG):
				if(scanJobType == ERASE_JOB_TYPE_NORMAL) {
					rcv_msg.msgType = MSG_ERASE_ROLLER_PAGE_START;
					err = SYMsgSend( SCANMANID, &rcv_msg );
				}
				else {
					err = SYMsgSend( A3_MERGE_ID, &rcv_msg );
				}
				if( err != OK ) {
					XASSERT( err==OK, err );
				}
				g_first_strip_size = 0;
				break;
				
			/*
			****************************************************************************************************************
			* SMSG_PAGE_DATA (Received From 'ScanMan')
			*
			* 	@parameter : 
			* 		- rcv_msg     : message pointer from 'ScanMan'
			* 		- num_rows    : data height (number of rows) of part of scan data
			* 		- dtype       : data type of scan data (SCAN_DATA_TYPE_MONO, SCAN_DATA_TYPE_RGB)
			* 		- scan_data   : data pointer (have to free somewhere)
			* 		- last_buffer : this means received packet is last or not
			****************************************************************************************************************
			*/
            case (SMSG_PAGE_DATA | MESSAGE_FROM_SCANTASK_FLAG):
				msg_scan_planedata_decode(&rcv_msg, &num_rows, &dtype, &scan_data, &last_buffer);
				msg_scan_planedata_get_page_side( &rcv_msg, &page_side );

				//DBG_PRINTF_SDH_DEBUG( "%s nr=%d dtype=%d data_ptr=%p last=%d\n", __FUNCTION__, num_rows, dtype, scan_data, last_buffer );
#if 1

#if ERASE_ROLLER_MSGQUEUE_DEBUG
				DBG_PRINTF_SDH_DEBUG("MSGQUEUE_INDEX(%d) - RCV\n", ++msgQueue_index);
#endif
				if (g_host_app == e_HOST_COPY)
				{
					int nLoop=0;
					for(nLoop=0; nLoop<num_rows; nLoop++)
					{
						scan_data[nLoop*tEraseRollerMainParam.pixels_per_row_padded] = 255;
					}
				}
				
				eraseRoller_receive_strip_data(dtype, num_rows, scan_data, last_buffer, page_side, firstRoller_check_data, secondRoller_check_data);
#else
				//TEST For not use erase lib, just bypass scan data
				eraseRoller_send_pageData(dtype, num_rows, scan_data, last_buffer, page_side);
#endif //#if 1
				break;

			/*
			****************************************************************************************************************
			* SMSG_PAGE_END (Received From 'ScanMan')
			****************************************************************************************************************
			*/				
			case (SMSG_PAGE_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("received SMSG_PAGE_END\n");
		
				if (!g_first_roller_end)
				{
					if(firstRoller_check_data) {
						if(tEraseRollerMainParam.roller_1st_buffer_save_point > 0) {
							//send stacked data (erase not executed) to 'ScanMan' (In Merge job this could not occured)
							unsigned int rows_num = 0;
							uint8_t *firstRollerRemainData;
							
							firstRollerRemainData = scanmem_alloc(tEraseRollerMainParam.roller_1st_buffer_save_point);
							memcpy(firstRollerRemainData, firstRoller_check_data, tEraseRollerMainParam.roller_1st_buffer_save_point);
							
							rows_num = tEraseRollerMainParam.roller_1st_buffer_save_point/tEraseRollerMainParam.bytes_per_row;		 
							eraseRoller_send_pageData(dtype, rows_num, firstRollerRemainData, TRUE, 0);
							DBG_PRINTF_SDH_DEBUG("[Roller #1] Roller Stacked Data Sent to App\n");
						}
						memFree(firstRoller_check_data);
						firstRoller_check_data = NULL;
					}
					g_first_roller_end = TRUE;
				}
				if (!g_second_roller_end)
				{
					if(secondRoller_check_data) {
						if(tEraseRollerMainParam.roller_2nd_buffer_save_point > 0) {
							//send stacked data (erase not executed) to 'ScanMan' (In Merge job this could not occured)
							unsigned int rows_num = 0;	
							uint8_t *secondRollerRemainData;
							
							secondRollerRemainData = scanmem_alloc(tEraseRollerMainParam.roller_2nd_buffer_save_point);
							memcpy(secondRollerRemainData, secondRoller_check_data, tEraseRollerMainParam.roller_2nd_buffer_save_point);
														
							rows_num = tEraseRollerMainParam.roller_2nd_buffer_save_point/tEraseRollerMainParam.bytes_per_row;
							eraseRoller_send_pageData(dtype, rows_num, secondRollerRemainData, TRUE, 0);
							DBG_PRINTF_SDH_DEBUG("[Roller #2] Roller Stacked Data Sent to App\n");
						}						
						memFree(secondRoller_check_data);
						secondRoller_check_data = NULL;
					}
					g_second_roller_end = TRUE;					
				}
				
				if(scanJobType == ERASE_JOB_TYPE_NORMAL) {
					rcv_msg.msgType = MSG_ERASE_ROLLER_PAGE_END;
					err = SYMsgSend( SCANMANID, &rcv_msg );
				}
				else {
					err = SYMsgSend( A3_MERGE_ID, &rcv_msg );
				}
				if( err != OK ) {
					XASSERT( err==OK, err );
				}
				
#if ERASE_ROLLER_MSGQUEUE_DEBUG
				DBG_PRINTF_SDH_DEBUG("MSGQUEUE_INDEX Clear(%d)\n", msgQueue_index);
				msgQueue_index = 0;
#endif				
				
				if (limiter)
				{
					mlimiter_stop(limiter);
					limiter = NULL;
				}
				break;

			/*
			****************************************************************************************************************
			* SMSG_SCAN_END (Received From 'ScanMan')
			****************************************************************************************************************
			*/
			case (SMSG_SCAN_END | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("received SMSG_SCAN_END\n");
				if(scanJobType == ERASE_JOB_TYPE_NORMAL) {
					rcv_msg.msgType = MSG_ERASE_ROLLER_SCAN_END;
					err = SYMsgSend( SCANMANID, &rcv_msg );
				}
				else {
					err = SYMsgSend( A3_MERGE_ID, &rcv_msg );
				}
				if( err != OK ) {
					XASSERT( err==OK, err );
				}				
				break;
						
			/*
			****************************************************************************************************************
			* SMSG_SCAN_READY (Received From 'ScanMan')
			****************************************************************************************************************
			*/			
			case (SMSG_SCAN_READY | MESSAGE_FROM_SCANTASK_FLAG):
				DBG_PRINTF_SDH_DEBUG("received SMSG_SCAN_READY\n");
				if(scanJobType == ERASE_JOB_TYPE_NORMAL) {
					rcv_msg.msgType = MSG_ERASE_ROLLER_SCAN_READY;
					err = SYMsgSend( SCANMANID, &rcv_msg );
				}
				else {
					err = SYMsgSend( A3_MERGE_ID, &rcv_msg );
				}				
				if( err != OK ) {
					XASSERT( err==OK, err );
				}
				break;
			/*
			****************************************************************************************************************
			* MSG_CANCELJOB (Received From 'ScanMan')
			****************************************************************************************************************				
			*/
			case MSG_CANCELJOB :
				DBG_PRINTF_SDH_DEBUG("received MSG_CANCELJOB\n");
				break;
			default:
				break;
		}
	}
	
	return 0;
}

int SDH_eraseRoller_isUse()
{
	/* Check DB save in Setting */
	int isUseRollerErase = 1;
	error_type_t db_retval = FAIL;
	
	isUseRollerErase = db_get_var_int("CONF_SYSTEM", "RollerDetection", &db_retval);
	if(db_retval == FAIL) {
		isUseRollerErase = 1;
	}
	return isUseRollerErase;
}

void eraseRoller_onetime_init()
{
	return;
}

int SDH_eraseRoller_init(void)
{
	UINT pxretcode;

	DBG_PRINTF_SDH_DEBUG ( "Erase Roller Main init...\n");

	/* Create the queue */
	posix_create_message_queue( &eraseRoller_task_msgq, "/eraseRoller", ERASEROLLER_TASK_QUEUE_SIZE, sizeof(MESSAGE));    

	/* Register a message queue */
	router_register_queue(ERASE_ROLLER_ID, eraseRoller_task_msgq);  

	/* Register the resource */
	rm_register(e_EraseRoller, ERASE_ROLLER_ID);

	/* Start the task */
	pxretcode = posix_create_thread( &eraseRoller_task_id, &eraseRoller_task, 0, "Erase_Roller_App",  eraseRoller_task_stack,
													ERASEROLLER_TASK_STACK_SIZE, POSIX_THR_PRI_NORMAL );
	XASSERT(pxretcode==0, pxretcode);

	eraseRoller_onetime_init();

	return 1;
}


