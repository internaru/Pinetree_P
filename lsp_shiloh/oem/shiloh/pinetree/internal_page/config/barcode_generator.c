#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "memAPI.h"

#if 0
#include "ctemplate.h"
#include "deviceid.h"
#include "string_mgr_api.h"
#include "printvars_api.h"

#include "oid_api.h"

#include "file_to_pipe_api.h"
#include "internal_page_api.h"

#include "report_manager.h"
#include "report_page.h"
#include "json_to_pcl.h"
#include "json_common.h"
#include "db_api.h"
#include "memAPI.h"
#include "logger.h"
#include "spec.h"
#include "fwVersion.h"
#include "net_api.h"

#include "report_templates.h"

#include <sys/time.h>
#include "../../sdh_fax/faxapp/include/faxapp_sc_code.h"
#include "../../sdh_fax/faxapp/include/faxapp_notify.h"

#define DBG_PRFX "REPORT: "
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_INTERNAL | LOGGER_SUBMODULE_BIT(1)

#define SDH_ERR DBG_PRINTF_SDH_ERR
#define SDH_DEBUG DBG_PRINTF_SDH_DEBUG
#endif

//CODE 128, Code B

enum ENUM_BAR
{
	BAR_0		=	16,
	BAR_A		=	33,
	BAR_a		=	65,
	BAR_START	=	104,
	BAR_STOP	=	106,
	BAR_QUITE	=	107,
	BAR_STOP_2	=	108,
};

static int fillBarLine (unsigned char * img, int * bitPosition, int barData, int lineWidth);

const char BARBW[109][11] =
{
	{1,1,0,1,1,0,0,1,1,0,0},	//0
	{1,1,0,0,1,1,0,1,1,0,0},	//1	
	{1,1,0,0,1,1,0,0,1,1,0},	//2
	{1,0,0,1,0,0,1,1,0,0,0},	//3
	{1,0,0,1,0,0,0,1,1,0,0},	//4
	
	{1,0,0,0,1,0,0,1,1,0,0},	//5
	{1,0,0,1,1,0,0,1,0,0,0},	//6
	{1,0,0,1,1,0,0,0,1,0,0},	//7
	{1,0,0,0,1,1,0,0,1,0,0},	//8
	{1,1,0,0,1,0,0,1,0,0,0},	//9
	
	{1,1,0,0,1,0,0,0,1,0,0},	//10
	{1,1,0,0,0,1,0,0,1,0,0},	//11
	{1,0,1,1,0,0,1,1,1,0,0},	//12
	{1,0,0,1,1,0,1,1,1,0,0},	//13
	{1,0,0,1,1,0,0,1,1,1,0},	//14
	
	{1,0,1,1,1,0,0,1,1,0,0},	//15
	{1,0,0,1,1,1,0,1,1,0,0},	//16
	{1,0,0,1,1,1,0,0,1,1,0},	//17
	{1,1,0,0,1,1,1,0,0,1,0},	//18
	{1,1,0,0,1,0,1,1,1,0,0},	//19
	
	{1,1,0,0,1,0,0,1,1,1,0},	//20
	{1,1,0,1,1,1,0,0,1,0,0},	//21
	{1,1,0,0,1,1,1,0,1,0,0},	//22
	{1,1,1,0,1,1,0,1,1,1,0},	//23
	{1,1,1,0,1,0,0,1,1,0,0},	//24
	
	{1,1,1,0,0,1,0,1,1,0,0},	//25
	{1,1,1,0,0,1,0,0,1,1,0},	//26
	{1,1,1,0,1,1,0,0,1,0,0},	//27
	{1,1,1,0,0,1,1,0,1,0,0},	//28
	{1,1,1,0,0,1,1,0,0,1,0},	//29
	
	{1,1,0,1,1,0,1,1,0,0,0},	//30
	{1,1,0,1,1,0,0,0,1,1,0},	//31
	{1,1,0,0,0,1,1,0,1,1,0},	//32
	{1,0,1,0,0,0,1,1,0,0,0},	//33
	{1,0,0,0,1,0,1,1,0,0,0},	//34
	
	{1,0,0,0,1,0,0,0,1,1,0},	//35
	{1,0,1,1,0,0,0,1,0,0,0},	//36
	{1,0,0,0,1,1,0,1,0,0,0},	//37
	{1,0,0,0,1,1,0,0,0,1,0},	//38
	{1,1,0,1,0,0,0,1,0,0,0},	//39
	
	{1,1,0,0,0,1,0,1,0,0,0},	//40
	{1,1,0,0,0,1,0,0,0,1,0},	//41
	{1,0,1,1,0,1,1,1,0,0,0},	//42
	{1,0,1,1,0,0,0,1,1,1,0},	//43
	{1,0,0,0,1,1,0,1,1,1,0},	//44
	
	{1,0,1,1,1,0,1,1,0,0,0},	//45
	{1,0,1,1,1,0,0,0,1,1,0},	//46
	{1,0,0,0,1,1,1,0,1,1,0},	//47
	{1,1,1,0,1,1,1,0,1,1,0},	//48
	{1,1,0,1,0,0,0,1,1,1,0},	//49
	
	{1,1,0,0,0,1,0,1,1,1,0},	//50
	{1,1,0,1,1,1,0,1,0,0,0},	//51
	{1,1,0,1,1,1,0,0,0,1,0},	//52
	{1,1,0,1,1,1,0,1,1,1,0},	//53
	{1,1,1,0,1,0,1,1,0,0,0},	//54
	
	{1,1,1,0,1,0,0,0,1,1,0},	//55
	{1,1,1,0,0,0,1,0,1,1,0},	//56
	{1,1,1,0,1,1,0,1,0,0,0},	//57
	{1,1,1,0,1,1,0,0,0,1,0},	//58
	{1,1,1,0,0,0,1,1,0,1,0},	//59
	
	{1,1,1,0,1,1,1,1,0,1,0},	//60
	{1,1,0,0,1,0,0,0,0,1,0},	//61
	{1,1,1,1,0,0,0,1,0,1,0},	//62
	{1,0,1,0,0,1,1,0,0,0,0},	//63
	{1,0,1,0,0,0,0,1,1,0,0},	//64
	
	{1,0,0,1,0,1,1,0,0,0,0},	//65
	{1,0,0,1,0,0,0,0,1,1,0},	//66
	{1,0,0,0,0,1,0,1,1,0,0},	//67
	{1,0,0,0,0,1,0,0,1,1,0},	//68
	{1,0,1,1,0,0,1,0,0,0,0},	//69
	
	{1,0,1,1,0,0,0,0,1,0,0},	//70
	{1,0,0,1,1,0,1,0,0,0,0},	//71
	{1,0,0,1,1,0,0,0,0,1,0},	//72
	{1,0,0,0,0,1,1,0,1,0,0},	//73
	{1,0,0,0,0,1,1,0,0,1,0},	//74
	
	{1,1,0,0,0,0,1,0,0,1,0},	//75
	{1,1,0,0,1,0,1,0,0,0,0},	//76
	{1,1,1,1,0,1,1,1,0,1,0},	//77
	{1,1,0,0,0,0,1,0,1,0,0},	//78
	{1,0,0,0,1,1,1,1,0,1,0},	//79
	
	{1,0,1,0,0,1,1,1,1,0,0},	//80
	{1,0,0,1,0,1,1,1,1,0,0},	//81
	{1,0,0,1,0,0,1,1,1,1,0},	//82
	{1,0,1,1,1,1,0,0,1,0,0},	//83
	{1,0,0,1,1,1,1,0,1,0,0},	//84
	
	{1,0,0,1,1,1,1,0,0,1,0},	//85
	{1,1,1,1,0,1,0,0,1,0,0},	//86
	{1,1,1,1,0,0,1,0,1,0,0},	//87
	{1,1,1,1,0,0,1,0,0,1,0},	//88
	{1,1,0,1,1,0,1,1,1,1,0},	//89
	
	{1,1,0,1,1,1,1,0,1,1,0},	//90
	{1,1,1,1,0,1,1,0,1,1,0},	//91
	{1,0,1,0,1,1,1,1,0,0,0},	//92
	{1,0,1,0,0,0,1,1,1,1,0},	//93
	{1,0,0,0,1,0,1,1,1,1,0},	//94
	
	{1,0,1,1,1,1,0,1,0,0,0},	//95
	{1,0,1,1,1,1,0,0,0,1,0},	//96
	{1,1,1,1,0,1,0,1,0,0,0},	//97
	{1,1,1,1,0,1,0,0,0,1,0},	//98	
	{1,0,1,1,1,0,1,1,1,1,0},	//99
	
	{1,0,1,1,1,1,0,1,1,1,0},	//100
	{1,1,1,0,1,0,1,1,1,1,0},	//101
	{1,1,1,1,0,1,0,1,1,1,0},	//102
	
	{1,1,0,1,0,0,0,0,1,0,0},	//103, Start A
	{1,1,0,1,0,0,1,0,0,0,0},	//104, Start B
	{1,1,0,1,0,0,1,1,1,0,0},	//105, Start C
	{1,1,0,0,0,1,1,1,0,1,0},	//106, Stop
	{0,0,0,0,0,0,0,0,0,0,0},	//107: QuiteZone
	{1,1,0,0,0,0,0,0,0,0,0},	//108: Stop-2
};

unsigned char * barcodeGen(const char * str, int barWidthInPixel, int height, int * width)
{
	int totalWidth=0;
	int totalLines=0;
	unsigned char *img=0;
	int byteAlignedWidth=0;
	int strPtr;
	int i=0,j=0;
	
	//SanityCheck
	if (barWidthInPixel <1)
	{
		printf ("(Report-Bar) Pixel Width is less then 1\r\n");
		return NULL;
	}
	else if (strlen(str)<1)
	{
		printf ("(Report-Bar) Strlength is less then 1\r\n");
		return NULL;
	}
	else if (height < 1)
	{
		printf ("(Report-Bar) Height is less then 1\r\n");
		return NULL;
	}
		
	//int strLength=strlen(str);
	for (strPtr=0; strPtr<strlen(str); strPtr++)
	{
		//Support Character Check
		if (! 	
				( str[strPtr]>=32 && str[strPtr]<=126)
			)
		{
			printf ("(Report-Bar) %s -> %dth Character\r\n",str,strPtr);
			printf ("(Report-Bar) '%c' (%d, 0x%02X) is not supported!\r\n",str[strPtr],str[strPtr],str[strPtr]);
			printf ("(Report-Bar) Error in return!\r\n");
			return NULL;
		}
	}
		
	//Sanity Check Done.
	
	totalLines=( (1+1+strlen(str)+1+1+1)*11 );	//Quite Code + StartCode + String + Checksum + StopChar + QuiteCode
	totalWidth =totalLines * barWidthInPixel;
	printf ("(Report-Bar) Barcode Width:%d->%d\r\n",totalLines,totalWidth);
	*width=totalWidth;
	
	byteAlignedWidth = totalWidth;
	
	if (byteAlignedWidth % 8 != 0)
		byteAlignedWidth=((byteAlignedWidth/8)+1)*8;
	byteAlignedWidth = byteAlignedWidth/8;
	
	printf ("(Report-Bar) Total Size:%d Allcated\r\n",byteAlignedWidth*height);
	img=(unsigned char *)MEM_CALLOC(byteAlignedWidth*height, sizeof(unsigned char));
	//printf ("(Report-Bar) BarBuf:0x%08X (*0x%08x)\r\n", img);
	
	for (j=0; j<height;j++)
	{
		//Barcode 적층
		unsigned char * nextLine = img + j*byteAlignedWidth;
		
		//Quite Code + StartCode + String + Checksum + StopChar + QuiteCode
		int bitPosition = 0;
		int checkSumValue = 0;
		fillBarLine (nextLine, &bitPosition, BAR_QUITE, barWidthInPixel);
		fillBarLine (nextLine, &bitPosition, BAR_START, barWidthInPixel);
		checkSumValue = BAR_START;
		
		for (i=0; i<strlen(str);i++)
		{
			//가로라인
			fillBarLine (nextLine, &bitPosition, str[i] - ( '0' - BAR_0 ) , barWidthInPixel);
			checkSumValue+=(str[i] - ( '0' - BAR_0 )) * (i+1);
			checkSumValue%=103;	//Avoid Overflow
		}
		//checkSumValue = checkSumValue % 103;
		fillBarLine (nextLine, &bitPosition, checkSumValue , barWidthInPixel);
		fillBarLine (nextLine, &bitPosition, BAR_STOP, barWidthInPixel);
		fillBarLine (nextLine, &bitPosition, BAR_STOP_2, barWidthInPixel);
		fillBarLine (nextLine, &bitPosition, BAR_QUITE, barWidthInPixel);
	}
	//printf ("(Report-Bar) Bit Position for Line:%d\r\n",bitPosition);
	
	return img;
}



static int fillBarLine (unsigned char * img, int * bitPosition, int barData, int lineWidth)
{
	int p,k;
	//Quite Code
	for (k=0; k<11; k++)
	{
		unsigned char slimLineBW = BARBW[barData][k];	//0 or 1
		for (p=0;p<lineWidth;p++)
		{
			//Fill one pixel.
			int bytePtr = (*bitPosition)/8;	//0
			int bitPtr = (7 - ((*bitPosition)%8) );	//0
			
			unsigned char bitValue = slimLineBW << bitPtr;
			
			img[bytePtr] |= bitValue;
			(*bitPosition)++;
		}
	}
	return (int)bitPosition;
}

unsigned char * graphGen(int width, int height, int lineWidth, int gridHeight, int axisNumber, int current, int total,  int fullThreshold)
{
	int currentUsed = current;
	int byteAlignedWidth=0;
	unsigned char *img = 0;
	int lineN=0;
	int lineV=0;
	int totalAvailBox = 0;
	int haveToFillBox = 0;			
	axisNumber++;
	
	
	if (lineWidth*2+gridHeight>=height)
	{
		printf ("(Report-Grp) Sanity Check Failed (%d,%d,%d,%d)\r\n",lineWidth, gridHeight, lineWidth*2+gridHeight,height);
		return NULL;
	}
	if (lineWidth*2 >= width)
	{
		printf ("(Report-Grp) Sanity Check Failed (%d,%d,%d)\r\n",lineWidth, lineWidth*2, width);
		return NULL;
	}
	if (currentUsed > total)
	{
		printf ("(Report-Grp) Sanity Check Warning (%d->%d)\r\n",current, total);
		currentUsed=total;
	}

	
	byteAlignedWidth = width;
	
	if (byteAlignedWidth % 8 != 0)
		byteAlignedWidth=((byteAlignedWidth/8)+1)*8;
	byteAlignedWidth = byteAlignedWidth/8;
	
	printf ("(Report-Grp) Total Size:%d Allcated\r\n",byteAlignedWidth*height);
	img=(unsigned char *)MEM_CALLOC(byteAlignedWidth*height, sizeof(unsigned char));
	
	//Draw Box

	for (lineN=0; lineN<lineWidth;lineN++)
	{
		//printf ("(Report-Grp) %d->%d, %d\r\n",lineN, gridHeight + lineN, byteAlignedWidth * (gridHeight + lineN) );
		//printf ("(Report-Grp) %d-[0x%02X]\r\n",lineN, *(img+byteAlignedWidth*lineN));
		memset ( (void *) (img + ( byteAlignedWidth * (gridHeight + lineN) ) ), 0xFF, byteAlignedWidth); //Horizontal 1.
		memset ( (void *) (img + ( byteAlignedWidth * (height - lineWidth + lineN) ) ), 0xFF, byteAlignedWidth); //Horizontal 1.
	}
	for (lineN=gridHeight; lineN < height; lineN++)
	{
		
		for (lineV=0;lineV<lineWidth;lineV++)
		{
			int vMod = lineV / 8;			//0~;
			int vOff = 7-(lineV % 8);		//7->0;
			int bitslide = 0x01 << vOff;

			int altLineV = width - 1 - lineV;
			int altVMod = altLineV / 8;
			int altVOff = 7-(altLineV % 8);		//7->0;			
			
			
			 *( (char *)(img + lineN*byteAlignedWidth + vMod) ) |= bitslide;
			bitslide = 0x01 << altVOff;
			*( (char *)(img + lineN*byteAlignedWidth + altVMod) ) |= bitslide;
		}
	}
	//Draw Box Done.
	
	//Fill Box
	totalAvailBox = width-lineWidth*2;
	haveToFillBox = totalAvailBox * current / total;
	#define CHECK_SPACE		3	//체크격자 간격
	for (lineN=0; lineN < height - ( gridHeight + lineWidth*2 ); lineN++)
	{
		for (lineV=0; lineV < haveToFillBox; lineV++)
		{
			int targetPosX = lineWidth+lineV;
			int targetPosY = gridHeight+lineWidth+lineN;
			int vMod = targetPosX / 8;
			int vOff = 7-(targetPosX % 8);
			int bitslide = 0x01 << vOff;
			
			if  (fullThreshold > current)
			{
				if (lineV % CHECK_SPACE == 0) 
					*( (char *)(img + targetPosY*byteAlignedWidth + vMod) ) |= bitslide;	//Half Color
			}
			else
				*( (char *)(img + targetPosY*byteAlignedWidth + vMod) ) |= bitslide;	//Full Color
		}
	}
	
	//Fill the Axis
	totalAvailBox = width-lineWidth;
	//#define AXIS_NUMBER 11
	
	if (axisNumber >= 2)
	{
		for (lineN=0;lineN < gridHeight; lineN++)
		{
			for (lineV=0; lineV < axisNumber; lineV++)
			{
				int lineO=0;
				int Xoffset=(totalAvailBox * lineV)/( axisNumber - 1);
				for (lineO = 0; lineO < lineWidth; lineO++)
				{
					int vMod = ( Xoffset+lineO ) / 8;
					int vOff = 7 - ((Xoffset+lineO) % 8);
					int bitslide = 0x01 << vOff;
					*( (char *)(img + lineN*byteAlignedWidth + vMod) ) |= bitslide;
				}
			}
		}
	}
	
	return img;
}

#if 0
int fillMemory (unsigned char *img, unsigned char *dest, unsigned long imgSizeX, unsigned long  imgSizeY, unsigned long  targetOffsetX, unsigned long targetOffsetY, unsigned long targetStride)
{
	if ( (img == NULL) || (dest == NULL) )
	{
		printf ("(Report) Error In Img (0x%08x) or Dest (0x%08x)\r\n",img,dest);
		return -1;
	}
	
	unsigned long targetYAddr=0;
	unsigned long i=0,j=0;
	for (j=0;j<imgSizeY;j++)
	{
		targetYAddr=dest + (targetOffsetY+j)*targetStride;
		for (i=0;i<imgSizeX;i++)
		{
			if (img[i/8] & (1<<(7-(i%8))))	//one Bit = 1;
			{
				//true
				unsigned long targetAddr=targetYAddr+(targetOffsetX/8)+(i/8)+(((targetOffsetX%8)+(i%8))/8);
				*((unsigned char *)(targetAddr)) |=  1 << (7-((targetOffsetX+i)%8));
			} 
		}
	}
	return 1;
}
#endif

