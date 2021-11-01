// include ////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>	// for message queue
#include <sys/ipc.h>		// for message queue
#include <sys/msg.h>		// for message queue
#include <string.h>		// strcpy()
#include <sys/time.h>	// for chech time
#include "memAPI.h"
#include "scantypes.h"
#include "scanmech.h"
#include "db_api.h"
#include "logger.h"
#include "rollereraseLib.h"
#include "rollereraseMain.h"

#define DBG_PRFX "[ERASE_LIB] "
#define LOGGER_MODULE_MASK  DEBUG_LOGGER_MODULE_SCAN | LOGGER_SUBMODULE_BIT(1)
#define DBG_PRINTF_SDH_ERR(...) DBG_PRINTF(LOG_SDH_ERR, DBG_PRFX __VA_ARGS__)
#define DBG_PRINTF_SDH_DEBUG(...) DBG_PRINTF(LOG_SDH_DEBUG, DBG_PRFX __VA_ARGS__)

static UINT8 hor_offset, ver_offset;

// function ////////////////////////////////////////////////////////////////////////
//void dumpimage(UINT8 **img, char* fname, UINT32 startX, UINT32 startY, UINT32 width, UINT32 height)
//{
//	int c, i, j;
//
//	FILE *ifp;
//	if( (ifp = fopen(fname, "wb")) == NULL )
//	{
//		return;
//	}
//	else
//	{
//		for( i = startY; i < startY+height; i++ )
//		{
//			for( j = startX; j < startX+width; j++ )
//			{
//				c=putc(img[i][j], ifp);
//				if( c==EOF )
//				{
//					printf(("Reach to End Of File"));
//					break;
//				}
//			}
//		}
//	}
//
//	fclose(ifp);
//}

void get_erase_roller_flatbed_margin(UINT16 dpi, scan_cmode_t mode)
{
	scan_err_t scerr = SCANERR_NONE;
	struct scan_flatbed_margins fb_margins;
	
	scerr = scanmech_get_fb_margins( dpi, mode, &fb_margins );
	
	//margin value is inch (1/100 step) --> [example] (hor_offset == 7) --> 0.07inch
	hor_offset = fb_margins.left_margin_hinches_x;
	ver_offset = fb_margins.top_margin_hinches_y;
	
	DBG_PRINTF_SDH_DEBUG("(EraseRoller)Scan FB Margin : hor_offset(%d), ver_offset(%d)\n", hor_offset, ver_offset);
}

void set_erase_roller_param(erase_roller_LibParam_t *er_param, UINT32 scan_dpi, UINT32 scan_cmode)
{
	// Service Menu (Get FB Scan Margin in Marvell Margin Table)
	get_erase_roller_flatbed_margin(scan_dpi, scan_cmode);
	
	// Service Menu (Get Roller Position)
	int rollerUnit = 12;
	signed short rollerPosition_X = 0;
	signed short rollerPosition_Y = 0;
	
#if 0
	error_type_t db_retval = FAIL;
	rollerPosition_X = db_get_var_int("CONF_SCANSUB", "RollerPositionX", &db_retval);
	if (db_retval == FAIL) {
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR : Get RollerPosition_X From DB!!!");
		rollerPosition_X = 0;
	}

	rollerPosition_Y = db_get_var_int("CONF_SCANSUB", "RollerPositionY", &db_retval);
	if (db_retval == FAIL)
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR : Get RollerPosition_Y From DB!!!");
		rollerPosition_Y = 0;
	}
#endif

	// parameter set 300/600dpi
	if(scan_dpi == 300)	// 300dpi parameter set
	{
		er_param->roller_height = 337;
		er_param->roller_width = 255;
		
		er_param->pwl = 61;
		er_param->pwr = 70;
		er_param->checklength = 70;
		er_param->inside = 40;
		er_param->edgebound = 40;
		er_param->blackpart = 40;

		er_param->REF1_X = 24 - hor_offset + (rollerUnit * rollerPosition_X);
		er_param->REF2_X = 24 - hor_offset + (rollerUnit * rollerPosition_X);
		er_param->REF1_X = er_param->REF1_X > 0 ? er_param->REF1_X : 0;
		er_param->REF2_X = er_param->REF2_X > 0 ? er_param->REF2_X : 0;
		er_param->REF1_Y = 1149 - ver_offset + (rollerUnit * rollerPosition_Y);
		er_param->REF2_Y = 2033 - ver_offset + (rollerUnit * rollerPosition_Y);
		
		er_param->bgrbound = 3;
		er_param->paperbound = 4;

		DBG_PRINTF_SDH_DEBUG("(EraseRoller)RollerPosition_X: %d(%d)\n", rollerPosition_X, rollerUnit * rollerPosition_X);
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)RollerPosition_Y: %d(%d)\n", rollerPosition_Y, rollerUnit * rollerPosition_Y);
	}
	else if(scan_dpi == 600)	// 600dpi parameter set
	{
		er_param->roller_height = 673;
		er_param->roller_width = 509;
		
		er_param->pwl = 121;
		er_param->pwr = 140;
		er_param->checklength = 140;
		er_param->inside = 80;
		er_param->edgebound = 80;
		er_param->blackpart = 80;

		er_param->REF1_X = 48 - hor_offset + (rollerUnit * 2 * rollerPosition_X);
		er_param->REF2_X = 48 - hor_offset + (rollerUnit * 2 * rollerPosition_X);
		er_param->REF1_X = er_param->REF1_X > 0 ? er_param->REF1_X : 0;
		er_param->REF2_X = er_param->REF2_X > 0 ? er_param->REF2_X : 0;
		er_param->REF1_Y = 2299 - ver_offset + (rollerUnit * 2 * rollerPosition_Y);
		er_param->REF2_Y = 4067 - ver_offset + (rollerUnit * 2 * rollerPosition_Y);
		
		er_param->bgrbound = 6;
		er_param->paperbound = 8;
		
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)RollerPosition_X: %d(%d\n)", rollerPosition_X, rollerUnit * 2 * rollerPosition_X);
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)RollerPosition_Y: %d(%d\n)", rollerPosition_Y, rollerUnit * 2 * rollerPosition_Y);
	}
}

BOOL erase_roller_part(UINT8 **inimg, UINT16 start_x, UINT16 start_y, UINT8 channels,
					   UINT16 dataWidth, UINT16 dataHeight, erase_roller_LibParam_t *er_param)//, BOOL only_detect)
{
	BOOL only_detect = FALSE;

	int roller_height = er_param->roller_height;
	int roller_width = er_param->roller_width;

	int pwl = er_param->pwl;
	int pwr = er_param->pwr;
	int pw = pwr - pwl + 1;
	
	int check_length = er_param->checklength;
	int inside = er_param->inside;
	int edge_bound = er_param->edgebound;
	int black_part = er_param->blackpart;

	int bgr_bound = er_param->bgrbound;
	int paper_bound = er_param->paperbound;
	
	int th1 = er_param->thr1 * pw;
	int th2 = er_param->thr2 * pw;
	int th3 = er_param->thr3 * pw;
#if CHECK_ROLLER_INSIDE
	int th4 = er_param->thr4 * pw;
	//int th5 = er_param->thr5 * pw;

	int roller_inside = er_param->roller_inside;
#endif

	int c1x = 0, c1y = 0;
	int c2x = 0, c2y = 0;
	int c3x = 0, c3y = 0;
	int c4x = 0, c4y = 0;
	int c1flag, c2flag, c3flag, c4flag;
	int number_of_corners = 0;

	int right_bottom_edge, bottom_right_edge, top_right_edge, right_top_edge;
	
	int *pl, *gl, *pr, *gr, *pu, *gu, *pd, *gd;
		
	UINT8 **imgdata = NULL;

	int flag1 = 0, flag2 = 0;
	
	int i, j;

	double background = 0.0f;
	double background_r = 0.0f, background_b = 0.0f;

#if CHECK_ROLLER_INSIDE
	BOOL is_real_roller = TRUE;
#endif

#if __A3_ROLLER_DETECT__
	UINT32 roller_detected = ROLLER_DETECT_NONE;	//0: no roller / 1: roller detected, so cancel
#endif

	DBG_PRINTF_SDH_DEBUG("(EraseRoller) X(%d),Y(%d),Channels(%d),dataWidth(%d), DataHeight\n", start_x, start_y, channels, dataWidth, dataHeight);

	
	if(channels == 1)
	{
		imgdata = inimg;
		//DBG_PRINTF_SDH_DEBUG("(EraseRoller)Ptr --> imgdata:(%d),imimg(%d)\n", imgdata, inimg);
	}
	else if(channels == 3)
	{
		imgdata = (UINT8 **)malloc(dataHeight * sizeof(UINT8 *));
		if(imgdata == NULL)
		{
			DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:malloc[%d]\n",__LINE__,dataHeight);
			goto Memoryfree;
		}
		else 
		{
			for(i = 0; i < dataHeight; i++)
			{
				imgdata[i] = (UINT8 *)malloc(dataWidth * sizeof(UINT8));
				if(imgdata[i] == NULL)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:malloc[%d]\n",__LINE__,dataWidth);
					goto Memoryfree;
				}
			}
		}
		
		for(i = 0; i < dataHeight; i++)
		{
			for(j = 0; j < dataWidth; j ++)
			{
				imgdata[i][j] = inimg[i][j * 3 + 1]; // G channel
			}
		}
	}

	pl = (int*)calloc(roller_height, sizeof(int));
	if( pl == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}
	
	gl = (int*)calloc(roller_height, sizeof(int));
	if( gl == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}

	pu = (int*)calloc(roller_width, sizeof(int));
	if( pu == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}
	
	gu = (int*)calloc(roller_width, sizeof(int));
	if( gu == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}
	
	pd = (int*)calloc(roller_width, sizeof(int));
	if( pd == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}

	gd = (int*)calloc(roller_width, sizeof(int));
	if( gd == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}

	pr = (int*)calloc(roller_height, sizeof(int));
	if( pr == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}
	
	gr = (int*)calloc(roller_height, sizeof(int));
	if( gr == NULL )
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)ERROR[%d]:calloc[%d]\n",__LINE__,roller_height);
		goto Memoryfree;
	}
	
	/***************** check corner #1 ***************************/
	for(i = 0; i < roller_height; i++)
	{
		for(j = 0; j < pw; j++)
		{
			pl[i] += imgdata[start_y + i][start_x + pwl + j];
		}
	}
	for(i = delta; i < roller_height - delta; i++)
	{
		gl[i] = pl[i - delta] - pl[i + delta];
	}
	for(i = delta; i < check_length; i++)
	{
#if REVERSE
		if(-gl[i] > th1)
		{
			c1y = start_y + i;
			flag1 = 1;
			break;
		}
#else
		if(gl[i] > th1)
		{
			c1y = start_y + i;
			flag1 = 1;
			break;
		}
#endif
	}

	for(j = 0; j < roller_width; j++)
	{
		for(i = 0; i < pw; i++)
		{
			pu[j] += imgdata[start_y + pwl + i][start_x + j];
		}		
	}
	for(j = delta; j < roller_width - delta; j++)
	{
		gu[j] = pu[j - delta] - pu[j + delta];
	}
	for(i = delta; i < check_length; i++)
	{
#if REVERSE
		if(-gu[i] > th2)
		{
			c1x = start_x + i;
			flag2 = 1;
			break;
		}
#else
		if(gu[i] > th2)
		{
			c1x = start_x + i;
			flag2 = 1;
			break;
		}
#endif
	}

	c1flag = flag1 * flag2;
	DBG_PRINTF_SDH_DEBUG("(EraseRoller)C1: %d(%d, %d)\n", c1flag, c1x - start_x, c1y - start_y);

	/***************** check corner #2 ***************************/
	flag1 = 0; flag2 = 0;
	for(i = roller_height - delta - 1; i > roller_height - delta - check_length; i--)
	{
#if REVERSE
		if(gl[i] > th1)
		{
			c2y = start_y + i;
			flag1 = 1;
			break;
		}
#else
		if(-gl[i] > th1)
		{
			c2y = start_y + i;
			flag1 = 1;
			break;
		}
#endif
	}

	for(j = 0; j < roller_width; j++)
	{
		for(i = 0; i < pw; i++)
		{
			pd[j] += imgdata[start_y + roller_height - pwr + i][start_x + j];
		}		
	}
	for(j = delta; j < roller_width - delta; j++)
	{
		gd[j] = pd[j - delta] - pd[j + delta];
	}
	for(i = delta; i < check_length; i++)
	{
#if REVERSE
		if(-gd[i] > th2)
		{
			c2x = start_x + i;
			flag2 = 1;
			break;
		}
#else
		if(gd[i] > th2)
		{
			c2x = start_x + i;
			flag2 = 1;
			break;
		}
#endif
	}

	c2flag = flag1 * flag2;
	DBG_PRINTF_SDH_DEBUG("(EraseRoller)C2: %d(%d, %d)\n", c2flag, c2x - start_x, c2y - start_y);

	number_of_corners = c1flag + c2flag;

#if CHECK_ROLLER_INSIDE
	if(number_of_corners == 2)
	{
		for(i = c1y - start_y + black_part + 1; i <= c2y - start_y - black_part - 1; i++)
		{
			if(pl[i] > th4)// || pl[i] < th5)
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)C1, C2 are not corners of the real roller\n");
				number_of_corners = 0;
				break;
			}

			if(i == c2y - start_y - black_part - 1)
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)C1, C2 are corners of the real roller\n");
			}
		}
	}
#endif

	if(number_of_corners > 0)
	{
		// Roller exists!!
		background = 0;
		for(i = 0; i < roller_height; i++)
		{
			background += imgdata[start_y + i][start_x];
		}
		background /= roller_height;

		if(channels == 3)
		{
			background_r = 0;
			background_b = 0;
			for(i = 0; i < roller_height; i++)
			{
				background_b += inimg[start_y + i][3*start_x+0];
				background_r += inimg[start_y + i][3*start_x+2];
				//background_b += pImgBuf2D_B[start_y + i][start_x];
				//background_r += pImgBuf2D_R[start_y + i][start_x];
			}
			background_b /= roller_height;
			background_r /= roller_height;
		}

		/***************** check corner #3 ***************************/
		flag1 = 0; flag2 = 0;
		for(i = 0; i < roller_height; i++)
		{
			for(j = 0; j < pw; j++)
			{
				pr[i] += imgdata[start_y + i][start_x + roller_width - pwr + j];
			}			
		}
		for(i = delta; i < roller_height - delta; i++)
		{
			gr[i] = pr[i - delta] - pr[i + delta];
		}
		for(i = delta; i < check_length; i++)
		{
#if REVERSE
			if(-gr[i] > th1)
			{
				c3y = start_y + i;
				flag1 = 1;
				break;
			}
#else
			if(gr[i] > th1)
			{
				c3y = start_y + i;
				flag1 = 1;
				break;
			}
#endif
		}

		for(j = roller_width - delta - 1; j > roller_width - delta - check_length; j--)
		{
#if REVERSE
			if(gu[j] > th2)
			{
				c3x = start_x + j;
				flag2 = 1;
				break;
			}
#else
			if(-gu[j] > th2)
			{
				c3x = start_x + j;
				flag2 = 1;
				break;
			}
#endif
		}

		c3flag = flag1 * flag2;

		DBG_PRINTF_SDH_DEBUG("(EraseRoller)C3: %d(%d, %d)\n", c3flag, c3x - start_x, c3y - start_y);

		/***************** check corner #4 ***************************/
		flag1 = 0; flag2 = 0;
		for(i = roller_height - delta - 1; i > roller_height - delta - check_length; i--)
		{
#if REVERSE
			if(gr[i] > th1)
			{
				c4y = start_y + i;
				flag1 = 1;
				break;
			}
#else
			if(-gr[i] > th1)
			{
				c4y = start_y + i;
				flag1 = 1;
				break;
			}
#endif
		}

		for(j = roller_width - delta - 1; j > roller_width - delta - check_length; j--)
		{
#if REVERSE
			if(gd[j] > th2)
			{
				c4x = start_x + j;
				flag2 = 1;
				break;
			}
#else
			if(-gd[j] > th2)
			{
				c4x = start_x + j;
				flag2 = 1;
				break;
			}
#endif
		}

		c4flag = flag1 * flag2;
		number_of_corners += c3flag + c4flag;

		DBG_PRINTF_SDH_DEBUG("(EraseRoller)C4: %d(%d, %d)\n", c4flag, c4x - start_x, c4y - start_y);

#if __A3_ROLLER_DETECT__
		if(only_detect == TRUE && number_of_corners != 4)
		{
			number_of_corners = 0;
		}
#endif

		switch(number_of_corners)
		{
#if __A3_ROLLER_DETECT__
		case 0:
			DBG_PRINTF_SDH_DEBUG("(EraseRoller)ROLLER IS NOT DETECTED(%d)\n", number_of_corners);
			break;
#endif
		case 1:
			DBG_PRINTF_SDH_DEBUG("(EraseRoller)NONE ERASE(%d)\n", number_of_corners);
			break;
			
		case 2:
			/*******************  CASE #2 ***********************/
			if(c1flag == 1 && c2flag == 1)
			{
				memset(pd, 0, roller_width * sizeof(int));
				for(j = 0; j < roller_width; j++)
				{
					for(i = 0; i < pw; i++)
					{
						pd[j] += imgdata[c2y - inside + i][start_x + j];
					}						
				}
				for(j = delta; j < roller_width - delta; j++)
				{
					gd[j] = pd[j - delta] - pd[j + delta];
				}

				right_bottom_edge = c2x - start_x + edge_bound;
				for(i= c2x - start_x + edge_bound + 1; i < roller_width - delta; i++)
				{
					//if(-gd[i] > th3)
					if(abs(gd[i]) > th3)
					{
						right_bottom_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(j = c2x - start_x + edge_bound + 1; j <= right_bottom_edge; j++)
				for(j = c2x - start_x + roller_inside; j <= right_bottom_edge; j++)
				{
					if(pd[j] > th4)// || pd[j] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE2 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE2 is real roller\n");
				}
#endif

				if(channels == 1)
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_bottom_edge + paper_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_bottom_edge + paper_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
				
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE2(right edge = %d)\n", right_bottom_edge - start_x);
			}
			/*******************  CASE #3 ***********************/
			else if(c1flag == 1 && c3flag == 1)
			{
				memset(pr, 0, roller_height * sizeof(int));
				for(i = 0; i < roller_height; i++)
				{
					for(j = 0; j < pw; j++)
					{
						pr[i] += imgdata[start_y + i][c3x - inside + j];
					}						
				}
				for(i = delta; i < roller_height - delta; i++)
				{
					gr[i] = pr[i - delta] - pr[i + delta];
				}

				bottom_right_edge = c3y - start_y + black_part;
				for(i = c3y - start_y + black_part + 1; i < roller_height - delta; i++)
				{
					//if(-gr[i] > th3)
					if(abs(gr[i]) > th3)
					{
						bottom_right_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(i = c3y - start_y + black_part + 1; i <= bottom_right_edge; i++)
				for(i = c3y - start_y + roller_inside; i <= bottom_right_edge; i++)
				{
					if(pr[i] > th4)// || pr[i] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE3 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE3 is real roller\n");
				}
#endif

				background = 0;
				for(i = 0; i < roller_width; i++)
				{
					background += imgdata[start_y][start_x + i];
				}
				background /= roller_width;

				if(channels == 3)
				{
					background_r = 0;
					background_b = 0;
					for(i = 0; i < roller_width; i++)
					{
						background_b += inimg[start_y][3 * (start_x + i) + 0];
						background_r += inimg[start_y][3 * (start_x + i) + 2];
						//background_b += pImgBuf2D_B[start_y][start_x + i];
						//background_r += pImgBuf2D_R[start_y][start_x + i];
					}
					background_b /= roller_width;
					background_r /= roller_width;
				}

				if(channels == 1)
				{
					for(i = c1y - bgr_bound; i <= start_y + bottom_right_edge + paper_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = c1y - bgr_bound; i <= start_y + bottom_right_edge + paper_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
				
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE3(bottom edge = %d)\n", bottom_right_edge - start_y);
			}
			/*******************  CASE #4 ***********************/
			else if(c2flag == 1 && c4flag == 1)
			{
				memset(pr, 0, roller_height * sizeof(int));
				for(i = 0; i < roller_height; i++)
				{
					for(j = 0; j < pw; j++)
					{
						pr[i] += imgdata[start_y + i][c4x - inside + j];
					}						
				}
				for(i = delta; i < roller_height - delta; i++)
				{
					gr[i] = pr[i - delta] - pr[i + delta];
				}

				top_right_edge = c2y - start_y - black_part;
				for(i = c2y - start_y - black_part - 1; i >= delta; i--)
				{
					//if(gr[i] > th3)
					if(abs(gr[i]) > th3)
					{
						top_right_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(i = top_right_edge; i <= c2y - start_y - black_part - 1; i++)
				for(i = top_right_edge; i <= c2y - start_y - roller_inside; i++)
				{
					if(pr[i] > th4)// || pr[i] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE4 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE4 is real roller\n");
				}
#endif

				background = 0;
				for(i = 0; i < roller_width; i++)
				{
					background += imgdata[start_y + roller_height - 1][start_x + i]; //mem bug fix
				}
				background /= roller_width;

				if(channels == 3)
				{
					background_r = 0;
					background_b = 0;
					for(i = 0; i < roller_width; i++)
					{
						background_b += inimg[start_y + roller_height - 1][3 * (start_x + i) + 0];
						background_r += inimg[start_y + roller_height - 1][3 * (start_x + i) + 2];
						//background_b += pImgBuf2D_B[start_y + roller_height - 1][start_x + i]; //mem bug fix
						//background_r += pImgBuf2D_R[start_y + roller_height - 1][start_x + i];
					}
					background_b /= roller_width;
					background_r /= roller_width;
				}

				if(channels == 1)
				{
					for(i = start_y + top_right_edge - paper_bound; i <= c4y + bgr_bound; i++)
					{
						for(j = c2x - bgr_bound; j <= c4x + bgr_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = start_y + top_right_edge - paper_bound; i <= c4y + bgr_bound; i++)
					{
						for(j = c2x - bgr_bound; j <= c4x + bgr_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
				
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE4(top edge = %d)\n", top_right_edge - start_y);
			}
			else
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)NONE ERASE(%d)\n", number_of_corners);
			}
			break;
			
		case 3:
			/*******************  CASE #5 ***********************/
			if(c1flag == 1 && c2flag == 1 && c3flag == 1)
			{
				memset(pd, 0, roller_width * sizeof(int));
				for(j = 0; j < roller_width; j++)
				{
					for(i = 0; i < pw; i++)
					{
						pd[j] += imgdata[c2y - inside + i][start_x + j];
					}						
				}
				for(j = delta; j < roller_width - delta; j++)
				{
					gd[j] = pd[j - delta] - pd[j + delta];
				}

				right_bottom_edge = c2x - start_x + edge_bound;
				for(i = c2x - start_x + edge_bound + 1; i < roller_width - delta; i++)
				{
					//if(-gd[i] > th3)
					if(abs(gd[i]) > th3)
					{
						right_bottom_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(j = c2x - start_x + edge_bound + 1; j <= right_bottom_edge; j++)
				for(j = c2x - start_x + roller_inside; j <= right_bottom_edge; j++)
				{
					if(pd[j] > th4)// || pd[j] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE5 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE5 is real roller\n");
				}
#endif

				memset(pr, 0, roller_height * sizeof(int));
				for(i = 0; i < roller_height; i++)
				{
					for(j = 0; j < pw; j++)
					{
						pr[i] += imgdata[start_y + i][c3x - inside + j];
					}						
				}
				for(i = delta; i < roller_height - delta; i++)
				{
					gr[i] = pr[i - delta] - pr[i + delta];
				}

				bottom_right_edge = c3y - start_y + black_part;
				for(i = c3y - start_y + black_part + 1; i < roller_height - delta; i++)
				{
					//if(-gr[i] > th3)
					if(abs(gr[i]) > th3)
					{
						bottom_right_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(i = c3y - start_y + black_part + 1; i <= bottom_right_edge; i++)
				for(i = c3y - start_y + roller_inside; i <= bottom_right_edge; i++)
				{
					if(pr[i] > th4)// || pr[i] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE5 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE5 is real roller\n");
				}
#endif

				if(channels == 1)
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_bottom_edge + paper_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
					for(i = c1y - bgr_bound; i <= start_y + bottom_right_edge + paper_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_bottom_edge + paper_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
					for(i = c1y - bgr_bound; i <= start_y + bottom_right_edge + paper_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
				
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE5(right edge = %d, bottom edge = %d)\n", right_bottom_edge - start_x, bottom_right_edge - start_y);
			}
			/*******************  CASE #6 ***********************/
			else if(c1flag == 1 && c2flag == 1 && c4flag == 1)
			{
				memset(pr, 0, roller_height * sizeof(int));
				for(i = 0; i < roller_height; i++)
				{
					for(j = 0; j < pw; j++)
					{
						pr[i] += imgdata[start_y + i][c4x - inside + j];
					}						
				}
				for(i = delta; i < roller_height - delta; i++)
				{
					gr[i] = pr[i - delta] - pr[i + delta];
				}

				top_right_edge = c2y - start_y - black_part;
				for(i = c4y - start_y - black_part - 1; i >= delta; i--)
				{
					//if(gr[i] > th2)
					if(abs(gr[i]) > th2)
					{
						top_right_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(i = top_right_edge; i <= c4y - start_y - black_part - 1; i++)
				for(i = top_right_edge; i <= c4y - start_y - roller_inside; i++)
				{
					if(pr[i] > th4)// || pr[i] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE6 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE6 is real roller\n");
				}
#endif

				memset(pu, 0, roller_width * sizeof(int));
				for(j = 0; j < roller_width; j++)
				{
					for(i = 0; i < pw; i++)
					{
						pu[j] += imgdata[c1y + inside - i][start_x + j];
					}						
				}
				for(j = delta; j < roller_width - delta; j++)
				{
					gu[j] = pu[j - delta] - pu[j + delta];
				}

				right_top_edge = c1x - start_x + edge_bound;
				for(i = c1x - start_x + edge_bound + 1; i < roller_width - delta; i++)
				{
					//if(-gu[i] > th3)
					if(abs(gu[i]) > th3)
					{
						right_top_edge = i;
						break;
					}
				}

#if CHECK_ROLLER_INSIDE
				//for(j = c1x - start_x + edge_bound + 1; j <= right_top_edge; j++)
				for(j = c1x - start_x + roller_inside; j <= right_top_edge; j++)
				{
					if(pu[j] > th4)// || pu[j] < th5)
					{
						is_real_roller = FALSE;
						break;
					}
				}
				if(is_real_roller == FALSE)
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE6 is not real roller\n");
					break;
				}
				else
				{
					DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE6 is real roller\n");
				}
#endif

				if(channels == 1)
				{
					for(i = start_y + top_right_edge - paper_bound; i <= c4y + bgr_bound; i++)
					{
						for(j = c2x - bgr_bound; j <= c4x + bgr_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_top_edge + paper_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = start_y + top_right_edge - paper_bound; i <= c4y + bgr_bound; i++)
					{
						for(j = c2x - bgr_bound; j <= c4x + bgr_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= start_x + right_top_edge + paper_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
				
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE6(top edge = %d, right edge = %d)\n", top_right_edge - start_y, right_top_edge - start_x);
			}
			else
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)NONE ERASE(%d)\n", number_of_corners);
			}
			break;
			
		case 4:
			/*******************  CASE #1 ***********************/
			DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE1\n");

#if CHECK_ROLLER_INSIDE
			for(i = c1y - start_y + black_part + 1; i <= c2y - start_y - black_part - 1; i++)
			{
				if(pl[i] > th4)// || pl[i] < th5)
				{
					is_real_roller = FALSE;
					break;
				}
			}
			if(is_real_roller == FALSE)
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE1 is not real roller\n");
				break;
			}
			else
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE1 is real roller\n");
			}

			for(i = c3y - start_y + black_part + 1; i <= c4y - start_y - black_part - 1; i++)
			{
				if(pr[i] > th4)// || pr[i] < th5)
				{
					is_real_roller = FALSE;
					break;
				}
			}
			if(is_real_roller == FALSE)
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE1 is not real roller\n");
				break;
			}
			else
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)CASE1 is real roller\n");
			}
#endif

#if __A3_ROLLER_DETECT__
			roller_detected = ROLLER_DETECT_TRUE;	//0: no roller / 1: roller detected, so cancel
			if(only_detect == TRUE)
			{
				DBG_PRINTF_SDH_DEBUG("(EraseRoller)Roller is detected, not erased\n");
			}
			else //only_detect == FALSE
			{
#endif
				if(channels == 1)
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j= c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							imgdata[i][j] = (BYTE)background;
						}
					}
				}
				else
				{
					for(i = c1y - bgr_bound; i <= c2y + bgr_bound; i++)
					{
						for(j = c1x - bgr_bound; j <= c3x + bgr_bound; j++)
						{
							inimg[i][3 * j] = (BYTE)background_b;
							inimg[i][3 * j + 1] = (BYTE)background;
							inimg[i][3 * j + 2] = (BYTE)background_r;
							//pImgBuf2D_B[i][j] = (BYTE)background_b;
							//pImgBuf2D_G[i][j] = (BYTE)background;
							//pImgBuf2D_R[i][j] = (BYTE)background_r;
						}
					}
				}
#if __A3_ROLLER_DETECT__
			}
#endif
			break;
			
		default:
			DBG_PRINTF_SDH_DEBUG("(EraseRoller)NONE ERASE(%d)\n", number_of_corners);
			break;
		}
	} //if(number_of_corners > 0)
	else
	{
		DBG_PRINTF_SDH_DEBUG("(EraseRoller)NONE ERASE(%d)\n", number_of_corners);
	}

Memoryfree:
	if(channels == 3)
	{
		for(i = 0; i < dataHeight; i++)
		{
			if(imgdata[i] != NULL)
			{
				free(imgdata[i]);
			}
		}
		if(imgdata != NULL)
		{
			free(imgdata);
		}
	}

	if(pl != NULL)
	{
		free(pl);
	}
	if(gl != NULL)
	{
		free(gl);
	}
	if(pu != NULL)
	{
		free(pu);
	}
	if(gu != NULL)
	{
		free(gu);
	}
	if(pd != NULL)
	{
		free(pd);
	}
	if(gd != NULL)
	{
		free(gd);
	}
	if(pr != NULL)
	{
		free(pr);
	}
	if(gr != NULL)
	{
		free(gr);
	}

	DBG_PRINTF_SDH_DEBUG("(EraseRoller)erase roller END\n");

#if __A3_ROLLER_DETECT__
	return roller_detected;
#else
	return TRUE;
#endif
}
