#ifndef _REMERGELIB_H_
#define _REMERGELIB_H_

#include "ATypes.h"

#define __DEFAULT_MSG__ 1

#define __A3_ROLLER_DETECT__		(1)	//use when roller detect during a3 scan. add.jong

#if __ENABLE_ERR_MSG__
	#define RELibErrMsg(_X_) {printf _X_;printf("\n");}
#else
	#define RELibErrMsg(_X_) {;}
#endif

#if __ENABLE_DBG_MSG__
	#define RELibDbgMsg(_X_) {printf _X_;printf("\n");}
#else
	#define RELibDbgMsg(_X_) {;}
#endif

#if __DEFAULT_MSG__
	#define RELibDftMsg(_X_) {printf _X_;printf("\n");}
#else
	#define RELibDbgMsg(_X_) {;}
#endif

// 2. definition ////////////////////////////////////////////////////////////////////////
#define WIDTHBYTES(bits) (((bits)+31)/32*4)			//4바이트의 배수여야 함

#define REVERSE 0 //0 - original(PC) / 1 - inverse BW(ABBA/OAK)
#define CHECK_ROLLER_INSIDE 1

//#define THR1 120
//#define THR2 80
//#define THR3 45

#define delta 4

//#define roller_height 582
//#define roller_width 418
//#define REF1_X 146
//#define REF1_Y 2306
//#define REF2_X 146
//#define REF2_Y 4076

//#define CHECK_LENGTH 80
//#define inside1 11
//#define inside2 30
//#define edge_bound 40
//#define black_part 40

// 3. struct ////////////////////////////////////////////////////////////////////////
typedef unsigned char BYTE;

typedef struct
{
	int 	roller_height;
	int 	roller_width;

	int 	pwl;
	int 	pwr;
	int 	checklength;
	int 	inside;
	int 	edgebound;
	int 	blackpart;
	
	int 	REF1_X;
	int 	REF1_Y;
	int 	REF2_X;
	int 	REF2_Y;
	
	int 	bgrbound;
	int 	paperbound;

	int		thr1;
	int		thr2;
	int		thr3;
#if CHECK_ROLLER_INSIDE
	int		thr4;
	//int		thr5;

	int		roller_inside;
#endif
} erase_roller_LibParam_t;

typedef struct{
	char*		name;
	UINT32		channels;
	UINT32		bitdepth;
	UINT32		format;
	void*		base;
	UINT32		stride;
	UINT32		ySize;
	UINT32		yStart;
	UINT32		width;
	UINT32		height;    
} RollerErase_IMGBUF_CONFIG_t;

// memory structure
#if REVERSE == 0
typedef struct  
{
	int width;
	int height;
	int channel;
	BYTE **data;
} BYTE2Dgray;
#endif

typedef enum {
	ROLLER_DETECT_NONE = 0,
	ROLLER_DETECT_TRUE = 1,
}ROLLER_DETECT_RET_VAL;

void get_erase_roller_flatbed_margin(UINT16 dpi, scan_cmode_t mode);

// 5. functions ////////////////////////////////////////////////////////////////////////
//void dumpimage(UINT8 **img, char* fname, UINT32 startX, UINT32 startY, UINT32 width, UINT32 height);
//roller erase function
BOOL erase_roller_part(UINT8 **inimg, UINT16 start_x, UINT16 start_y, UINT8 channels, UINT16 dataWidth, UINT16 dataHeight, erase_roller_LibParam_t *er_param);
void set_erase_roller_param(erase_roller_LibParam_t *er_param, UINT32 scan_dpi, UINT32 scan_cmode);

//#define RollerEraseLib_ScanReady	0x00000001
//#define RollerEraseLib_ScanEnd	0x00000002

#endif
