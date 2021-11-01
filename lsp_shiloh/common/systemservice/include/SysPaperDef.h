/*
	<paper_def.h>
	
	Paper size and type definition.
	Copyright(c) 2010, SINDOH Co., Ltd.
	
	Author: 
		Pyo,Minseok (mspyo@sindoh.com)
		Kim,Jaemin (jaemin@sindoh.com)
	
	History:
		2009-11-25: First draft released
		2010-02-19: Edited by mspyo - Modify the paper type and add paper texture & weight.
		2010-06-08: mpsyo - Assign paper size enum number. (2010-06-08, 12:20)
		2010-06-12: mspyo - Arrange the paper type sequence. (2010-06-12, 07:45)
		2010-07-05: J.Min - Fill all of paper size.
	
*/

#ifndef SS_PAPER_DEF_H
	#define SS_PAPER_DEF_H
	
enum	SIZE_ENUM	{
	/* LongEdge Feed */
	CODE_NO_PAPER = 0,
	CODE_A0,		CODE_A1,		CODE_A2,		CODE_A3,
	CODE_A4,		CODE_A5,		CODE_A6,		CODE_A7,
	CODE_B0,		CODE_B1,		CODE_B2,		CODE_B3,
	CODE_B4,		CODE_B5,		CODE_B6,		CODE_B7,
	CODE_WMAIL,		CODE_MAIL,		CODE_LINE1,		CODE_LINE2,
	CODE_LIB6,		CODE_LIB8,		CODE_210x170,	CODE_210x182,
	CODE_267x388,

	CODE_FREEmm = 31,
	CODE_11x17,
	CODE_11x14,		CODE_10x15,		CODE_10x14,		CODE_8Hx14,
	CODE_8Hx13,		CODE_8Hx11,		CODE_8Qx14,		CODE_8Qx13,
	CODE_8x13,		CODE_8x10H,		CODE_8x10,		CODE_5Hx8H,
	CODE_7Qx10H,	CODE_GLG,

	CODE_12x14H = 48,
	CODE_11x15,		CODE_9Hx11,		CODE_8Hx12,
	CODE_8KAI = 66,
	CODE_16KAI,
	CODE_COM10 = 80,
	CODE_MONARCH,
	CODE_C5 = 83,
	CODE_C6,		CODE_DL,		CODE_200X148,

	/* ShortEdge Feed */
	CODE_NO_SIZE = 128,
	CODE_A0T,		CODE_A1T,		CODE_A2T,		CODE_A3T,
	CODE_A4T,		CODE_A5T,		CODE_A6T,		CODE_A7T,
	CODE_B0T,		CODE_B1T,		CODE_B2T,		CODE_B3T,
	CODE_B4T,		CODE_B5T,		CODE_B6T,		CODE_B7T,
	CODE_WMAILT,	CODE_MAILT,		CODE_LINE1T,	CODE_LINE2T,
	CODE_LIB6T,		CODE_LIB8T,		CODE_210x170T,	CODE_210x182T,
	CODE_267x388T,

	CODE_FREEmmT = 159,
	CODE_11x17T,
	CODE_11x14T,	CODE_10x15T,	CODE_10x14T,	CODE_8Hx14T,
	CODE_8Hx13T,	CODE_8Hx11T,	CODE_8Qx14T,	CODE_8Qx13T,
	CODE_8x13T,		CODE_8x10HT,	CODE_8x10T,		CODE_5Hx8HT,
	CODE_7Qx10HT,	CODE_GLGT,

	CODE_12x14HT = 176,
	CODE_11x15T,	CODE_9Hx11T,	CODE_8Hx12T,
	CODE_8KAIT = 194,
	CODE_16KAIT,
	CODE_COM10T = 208,
	CODE_MONARCHT,	CODE_COM9T,
	CODE_C5T = 211,
	CODE_C6T,		CODE_DL_T,		CODE_100X148,	CODE_148X200,

	CODE_AUTO = 254,	/* Auto Size Detect */
	CODE_UNKNOWN = 255
};

	struct	paper_size {
		int		code;
		const char	*name;
	};

#define		PAPER_KIND_NUM	25
#ifdef	PAPER_H_MAIN
	extern const	struct	paper_size   PaperSize[];
	const	struct	paper_size   PaperSize[PAPER_KIND_NUM] = {
		{ CODE_A4T,				"A4"},
		{ CODE_A5T,				"A5 SEF"},
		{ CODE_A5,				"A5 LEF"},
		{ CODE_A6T,				"A6"},
		{ CODE_B5T,				"B5"},
		{ CODE_B6T,				"B6"},
		{ CODE_8Hx11T,			"Letter"},
		{ CODE_8Hx14T,			"Legal"},
		{ CODE_7Qx10HT,			"Executive"},
		{ CODE_5Hx8HT,			"Statement"},

		{ CODE_8Qx13T,			"US Folio"},
		{ CODE_8x10T,			"Quatro"},
		{ CODE_8x10HT,			"G. Letter"},
		{ CODE_8Qx14T,			"G. Legal"},
		{ CODE_100X148,			"Card 100x148"},
		{ CODE_148X200,			"Card 148x200"},
		{ CODE_200X148,			"Card 200x148"},
		{ CODE_MONARCHT,		"Monarch Envelope"},
		{ CODE_COM9T,			"Com9 Envelope"},
		{ CODE_COM10T,			"Com10 Envelope"},

		{ CODE_DL_T,			"DL Envelope"},
		{ CODE_C5T,				"C5 Envelope"},
		{ CODE_C6T,				"C6 Envelope"},
		{ CODE_FREEmm,			"Custom Size"},
		{ CODE_AUTO,			"Auto Detect"}
	};
#else
extern const	struct	paper_size   PaperSize[];
#endif
#if 0
	static int PAPER_WidthHeight[PAPER_KIND_NUM][3] =	//In Pixel
	{
		//Enum, Width, Height
		{CODE_A4T,				4960,7016},				//EVT2
		{CODE_A5T,				3496,4960},			//EVT2
		{CODE_A5,				4960,3496},	//Undefined-_-
		{CODE_A6T,				2480,3496},
		{CODE_B5T,				4300,6070},	//Japanese B5 182x257
		{CODE_B6T,				3024,4300},	//Japanese B6 128x182
		{CODE_8Hx11T,			5100,6600},	//EVT2
		{CODE_8Hx14T,			5100,8400},				//EVT2
		{CODE_7Qx10HT,			4352,6300},			//EVT2
		{CODE_5Hx8HT,			3300,5100},			//EVT2
		{CODE_8Qx13T,			5100,7800},
		{CODE_8x10T,			4800,6000},	//8"x10", Pixel
		{CODE_8x10HT,			4800,6300},	//Undefined-_-8"*10.5"
		{CODE_8Qx14T,			4952,8400},	//Undefined-_-8 1/4"x14"
		{CODE_100X148,			2362,3496},//Undefined-_-
		{CODE_148X200,			3496,4724},//Undefined-_-
		{CODE_200X148,			4724,3496},//Undefined-_-
		{CODE_MONARCHT,			2324,4500},	//Undefined-_-
		{CODE_COM9T,			2324,5324},	//Undefined-_-
		{CODE_COM10T,			2476,5700},	//Undefined-_-
		{CODE_DL_T,				2598,5196},		//Undefined-_-
		{CODE_C5T,				3826,5410},		//Undefined-_-
		{CODE_C6T,				2692,3826},		//Undefined-_-
		{CODE_FREEmm,			500,500},		//Undefined-_-
		{CODE_AUTO,				500,500},		//Undefined-_-
	};
#endif	
	
	enum PAPER_Type {	// 2010-06015 inhwan5 changed enum numbers
		TYPE_DEFAULT,
		TYPE_PLAIN = 1,
		TYPE_PAPERLABEL = 2,
		TYPE_ENVELOPE = 3,
		TYPE_CARDSTOCK = 4,
		TYPE_TRANSPARENCY = 5,
		TYPE_VINYLLABEL = 6,
		TYPE_POSTCARD = 7, 	// 2010-06-08
		TYPE_RECYCLED = 8, 	// 2014-08-29
		TYPE_LAST = 9,		
	};
	
	enum PAPER_Texture {
		TEXTURE_DEFAULT,
		TEXTURE_SMOOTH = 1,
		TEXTURE_NORMAL = 2,
		TEXTURE_ROUGH = 3,
		TEXTURE_LAST = 4,
	};
	
	enum PAPER_Weight {
		WEIGHT_DEFAULT,
		WEIGHT_LIGHT = 1,
		WEIGHT_NORMAL = 2,
		WEIGHT_HEAVY = 3,
		WEIGHT_LAST = 4,
	};
	

#endif			// SS_PAPER_DEF_H


