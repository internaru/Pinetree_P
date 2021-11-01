#ifndef _A3MERGELIB_H_
#define _A3MERGELIB_H_

#include "ATypes.h"

#define SDH_A3MergeLib_Version	"0.0.1"

// v2.1.1 (2010.07.14) : CONRO를 이용한 이미지 rotation
// v2.1.2 (2010.08.09) : A3MergeLib_Version을 Function Process 전달 추가
// v2.2.0 (2010.09.02) : Image Warpping Bug Fix
// v2.2.1 (2010.09.03) : v2.2.0의 Bug Fix
// v2.3.0 (2010.10.29) : Image Warpping 수정 및 Bug Fix
// v2.4.0 (2010.11.24) : CONRO 관련 최적화
// v2.4.1 (2010.12.09) : border margin 추가, 특징점 제한 및 탐색옵션 scan과 동일화
// v2.5.0 (2011.01.06) : overlap영역 200에서 142로 변경
//                       A3 scan코드 일치화: makeIntergalimg / linear blending
//                       bright compen.에서 linear compen.으로 변경
//                       merge debug 추가
// v2.5.1 (2011.01.13) : blending 변경에 따른 fill value 변경
// v2.5.2 (2011.01.27) : setlinebuf(stdout); 추가
// v2.6.0 (2011.01.28) : 매칭알고리즘 수정, 예외처리 코드 추가
// v2.6.1 (2011.01.31) : linear blending으로 변경에 따른 fill value (255 -> 0) 변경
// v2.6.2 (2011.01.31) : findmotionvector 서울대 디버깅 코드 추가 수정
// v2.6.3 (2011.02.08) : Binary 파일에서 version 정보 확인할 수 있도록 기능 추가, 타임체크로그 정리
// v2.6.4 (2011.04.20) : added the cancel processing by bong.
// v2.6.5 (2011.10.14) : Fix the bug for the cancel processing by bong.
//for DLT v2.7.0_DLT (2012.05.29) : changed Double Letter Merge
//for DLT v2.7.1_DLT (2012.05.30) : added PID
// v2.7.0 (2012.08.14) : added DPL/A3 mode, added PID
// v2.7.1 (2012.08.14) : bug fix
// v2.7.3 (2013.05.09) : 좌단 보더링 24px에서 48px로 변경
// 2. definition ////////////////////////////////////////////////////////////////////////

// define ////////////////////////////////////////////////////////////////////////
#define WIDTHBYTES(bits)	(((bits) + 31) / 32 * 4)	//4바이트의 배수여야 함
#define SQUARE(a)			((a) * (a))
#define round(a)			((int)((a) + 0.5))
#define myround(a)			(int)((a) + ((a > 0) ? 0.5f : -0.5f))
#define floor(a)			((int)(a))
#define ceil(a)				((int)((a) + 1.0))
#define myabs(a)			((a) > 0 ? (a) : (-(a)))
#define PI					3.141592653589793f

#define a_half				500
#define one					1000
#define a_half2				500000
#define one2				1000000
#define pseudo_round(a)		(((a) + a_half) / one)
#define pseudo_floor(a)		((a) / one)
#define pseudo_ceil(a)		(((a) + one) / one)
#define BYTE				unsigned char

//#define UINT unsigned int
#define DWORD			unsigned long
#define pseudo_float	int

//////////////////////////////////////////////////////////////////////////
// 이전 A3copy설정
//#define CORNER_STRENGTH 5	//default 10	//if you increase this value, more corner would be found
//#define SUB_PIXEL_ACCURACY 0	//default 1	//if you want to get better align result, change value into 1
//#define LINEAR_INTERPOLATION 1		//default 1	 //if you want to get better image quality, change value into 1 <- 반드시 1로
//#define FAST_CONV 1	//default 0	//if you want to get fast speed instead of accuracy in convolution, change value into 1
//#define RESIZE_QUALITY 0	//default 1	// 1 : high quality  0 : low quality
//#define MAX_FEATURE 500	//default 5000
//#define FLOAT_ROUND 1	//default 1
//#define MAX_DIFF 100	//default 100
//#define SAVE_CORNER_IMG 0	//default 0	//if you want save corner image, change value into 1
//#define PARAM_BLEND_TYPE	3	//3. bright compen.	/ 1. linear compen.//후처리 블랜딩 타입
// A3scan설정
//#define CORNER_STRENGTH 10 //if you increase this value, more corner would be found
//#define SUB_PIXEL_ACCURACY 1 //if you want to get better align result, change value into 1
//#define LINEAR_INTERPOLATION 1 //if you want to get better image quality, change value into 1
//#define FAST_CONV 1 //if you want to get fast speed instead of accuracy in convolution, change value into 1
//#define RESIZE_QUALITY 1 // 1 : high quality  0 : low quality
//#define MAX_FEATURE 3000
//#define FLOAT_ROUND 0
#define CORNER_STRENGTH			10	//if you increase this value, more corner would be found
#define SUB_PIXEL_ACCURACY		1	//if you want to get better align result, change value into 1
#define LINEAR_INTERPOLATION	1	//if you want to get better image quality, change value into 1
#define FAST_CONV				1	//if you want to get fast speed instead of accuracy in convolution, change value into 1
#define RESIZE_QUALITY			1	// 1 : high quality  0 : low quality
#define MAX_FEATURE				3000
#define FLOAT_ROUND				0	//1이면 반올림함
#define MAX_DIFF				100

#define BORDER_MARGIN			24
#define PARAM_BLEND_TYPE		1	//3. bright compen.	/ 1. linear compen.//후처리 블랜딩 타입
#define WIDTH					2550	//300dpi->8.5inch : Letter Width
#define A3WIDTH					4960	//300dpi->16.5inch : A3 Height
#define HEIGHT					3508	//300dpi->11.69inch : A4 Height

//A3 merge
#define A3_OVERLAP_WIDTH_SCAN	142 //오버랩 넓이
#define A3_OVERLAP_WIDTH_COPY	96	//오버랩 넓이
#define A3_OVERLAP_SCALE		1	//오버랩 스케일

//DLT merge
#define DLT_OVERLAP_WIDTH_SCAN	142 //오버랩 넓이
#define DLT_OVERLAP_WIDTH_COPY	96	//오버랩 넓이
#define DLT_OVERLAP_SCALE		1	//오버랩 스케일

//Legal merge
#define LG_OVERLAP_WIDTH_SCAN	152 //오버랩 넓이
#define LG_OVERLAP_WIDTH_COPY	76	//오버랩 넓이
#define LG_OVERLAP_SCALE		1	//오버랩 스케일
#define TIME_CHECK				0	//시간 측정
#define A3_DEBUG_LOG_VIEW		1	//A3 디버그 로그
#define MEMORY_FAIL				(-1)
#define WHITE_VALUE				0xff

//////////////////////////////////////////////////////////////////////////

// 3. struct ////////////////////////////////////////////////////////////////////////
typedef struct Point2D32fTAG
{
	float x;
	float y;
} Point2D32f;

typedef struct RgbPixelTAG
{
	BYTE r, g, b;
} RgbPixel;

typedef struct BYTE2DgrayTAG
{
	int width;
	int height;
	BYTE **data;
} BYTE2Dgray;

typedef struct Char2DgrayTAG
{
	int width;
	int height;
	char **data;
} Char2Dgray;

typedef struct Pseudo_float2DgrayTAG
{
	int width;
	int height;
	pseudo_float **data;
} Pseudo_float2Dgray;

typedef struct Float2DgrayTAG
{
	int width;
	int height;
	float **data;
} Float2Dgray;

typedef struct Short2DgrayTAG
{
	int width;
	int height;
	short **data;
} Short2Dgray;

typedef struct Int2DgrayTAG
{
	int width;
	int height;
	int **data;
} Int2Dgray;

typedef struct UINT2DgrayTAG
{
	int width;
	int height;
	unsigned int **data;
} UINT2Dgray;

typedef struct BYTE2DcolorTAG
{
	int width;
	int height;
	RgbPixel **data;
} BYTE2Dcolor;

// 5. functions ////////////////////////////////////////////////////////////////////////
BOOL make2Dpseudo_floatarray(Pseudo_float2Dgray *, int, int);
BOOL make2Dfloatarray(Float2Dgray *, int, int);
BOOL make2DBYTEarray(BYTE2Dgray *, int, int);
BOOL make2DBYTEarrayfrom1Darray(BYTE *, BYTE2Dgray *, int, int);
BOOL make2Dchararray(Char2Dgray *, int, int);
BOOL make2DUINTarray(UINT2Dgray *, int, int);
BOOL make2Dshortarray(Short2Dgray *, int, int);
BOOL make2Dintarray(Int2Dgray *, int, int);
BOOL make2Dcolorarray(BYTE2Dcolor *, int, int);
void setZerofloat(Float2Dgray *);
void deletefloat2D(Float2Dgray *);
void setZeropseudo_float(Pseudo_float2Dgray *);
void deletepseudo_float2D(Pseudo_float2Dgray *);
void deleteBYTE2D(BYTE2Dgray *);
void deletecolor2D(BYTE2Dcolor *);
void deleteint2D(Int2Dgray *);
void deleteshort2D(Short2Dgray *);
void conv2(Float2Dgray *, Float2Dgray *, Float2Dgray *);
int gaussian(Float2Dgray *, Float2Dgray *, float);
int matMul(Float2Dgray *, Float2Dgray *, Float2Dgray *, BOOL, BOOL);
BOOL invert(Float2Dgray *);
int paste(BYTE2Dgray *, BYTE2Dgray *, int, int);
float linear_itp_float(Float2Dgray *, float, float);
BYTE linear_itp_BYTE(BYTE2Dgray *, pseudo_float, pseudo_float);
int resize(BYTE2Dgray *, int);
void circle(BYTE2Dgray *, Point2D32f, int, int, BYTE);
void line(BYTE2Dgray *, Point2D32f, Point2D32f, int, BYTE);
int imgWarpAffine(BYTE2Dgray *, BYTE2Dgray *, Float2Dgray *);
int imgWarpAffineColor(BYTE2Dcolor *, BYTE2Dcolor *, Float2Dgray *);
int makeIntergalimg(BYTE2Dgray *, Int2Dgray *);
int blobdetector(BYTE2Dgray *, Point2D32f *, int *, int);
int harrisCorner(Float2Dgray *, Point2D32f *, int *, int);
int findmotionvector(BYTE **, BYTE **, int, int, int, float *, float *);
int *computePutativeCorrespondence(int, int, Point2D32f *, Point2D32f *, BYTE2Dgray *, BYTE2Dgray *, int, int *, int *);
int *computePutativeCorrespondenceColor(int, int, Point2D32f *, Point2D32f *, BYTE2Dcolor *, BYTE2Dcolor *, int, int *, int *);
int locateFeaturePoints(BYTE2Dgray *, int, int, Point2D32f *, int, int, BOOL);
int locateFeaturePointsColor(BYTE2Dcolor *, int, int, Point2D32f *, int, int, BOOL);
int computeTransform(BYTE2Dgray *, BYTE2Dgray *, Point2D32f *, Point2D32f *, int, int, int, Float2Dgray *);
int computeTransformColor(BYTE2Dcolor *, BYTE2Dcolor *, Point2D32f *, Point2D32f *, int, int, int, Float2Dgray *);
void getStitchingCut(UINT2Dgray *, UINT2Dgray *, int *, int, int);
void overlap_boundary(BYTE2Dgray *, int *, int, int, int);
void overlap_boundaryColor(BYTE2Dcolor *, int *, int, int, int);
int Qsort_compair(const void *, const void *);
int blending(BYTE2Dgray *, BYTE2Dgray *, int, int, int, int, int);
int blendingColor(BYTE2Dcolor *, BYTE2Dcolor *, int, int, int, int, int);
void dumpimage(BYTE2Dgray *, char *);
void BorderMargin(BYTE2Dgray *, int, unsigned char);
void BorderMarginColor(BYTE2Dcolor *, int);

#define A3MergeLib_ScanReady_1Page	0x00000001
#define A3MergeLib_ScanEnd_1Page	0x00000002
#define A3MergeLib_ScanReady_2Page	0x00000004
#define A3MergeLib_ScanEnd_2page	0x00000008
#define A3MergeLib_ScanPaperEjected 0x00000010
#define A3MergeLib_ScanCancelled	0x00000020

#define BODDER_SIDE_TOP				0x01
#define BODDER_SIDE_BOTTOM			0x02
#define BODDER_SIDE_LEFT			0x04
#define BODDER_SIDE_RIGHT			0x08
#define BODDER_SIDE_ALL				0x0F

#define A3MergeLib_EVENT_TOTAL		(A3MergeLib_ScanReady_1Page | A3MergeLib_ScanEnd_1Page | A3MergeLib_ScanReady_2Page | \
	A3MergeLib_ScanEnd_2page | A3MergeLib_ScanPaperEjected)

typedef struct
{
	char *name;
	int channels;
	int bitdepth;
	int format;
	void *base;
	UINT32 stride;
	UINT32 ySize;
	UINT32 yStart;
	UINT32 width;
	UINT32 height;
} A3Merge_IMGBUF_CONFIG_t;
#endif
