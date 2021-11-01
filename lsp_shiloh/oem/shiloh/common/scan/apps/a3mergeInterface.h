#ifndef _A3MERGEMAIN_H_
#define _A3MERGEMAIN_H_
	typedef int (*PRINTREADY_CB)(void *image);
	typedef int (*A3MERGE_CB)(int arg1, int arg2, int arg3);

	void showVer_A3Merge(void);
	int SDH_A3MergeLib_notifyScanStates(UINT32 arg1,UINT32 arg2,UINT32 arg3);
	int SDH_A3MergeLib_registerPrintCBFunc(PRINTREADY_CB);
	int SDH_A3MergeLib_registerStatusCBFunc (A3MERGE_CB);

	int SDH_A3MergeLib_init(void);

	int rotate_buffer(unsigned char *source, unsigned int source_width, unsigned int source_height, unsigned char *destination);
	void rotate(unsigned char *pS, unsigned char *pD, unsigned int row, unsigned int col);


	enum
	{
		A3MERGE_STATUS_1PageStart,	//0
		A3MERGE_STATUS_1PageEnd,	//1
		A3MERGE_STATUS_2PageStart,	//2
		A3MERGE_STATUS_2PageEnd,	//3
		A3MERGE_STATUS_OutputStart,	//4
		A3MERGE_STATUS_OutputEnd,	//5
		A3MERGE_STATUS_Error,
		A3MERGE_STATUS_CancelDone,
		A3MERGE_STATUS_RollerDetected,
	};
	enum
	{
		A3MERGE_INPUTSTATE_SetImageBuf,
		A3MERGE_INPUTSTATE_2PageEnd,
		A3MERGE_INPUTSTATE_A3PaperReturn,
		A3MERGE_INPUTSTATE_JobCancel
	};

	enum
	{
		A3MERGE_SCAN,
		A3MERGE_COPY
	};

	enum
	{
		A3MERGE_A3,
		A3MERGE_DLT,
		A3MERGE_LG,
		A3SEPARATE_A3,
		A3SEPARATE_DLT
	};
	
	enum
	{
		A3MERGE_RETAIN_MERGE_RESULT,
		A3MERGE_ROTATE_MERGE_RESULT
	};		
	
	typedef struct A3MERGE_PARAM{
		unsigned char	ucMainApp;
		unsigned char	ucScanedPaper;
		unsigned int	image_pixels_per_row_with_padding;
		unsigned int	image_pixels_per_row;
		unsigned int	LinesPerSheet;
		unsigned int	BitsPerPixel;
		unsigned int	printed_width;
		unsigned char	mergeResultShape;
		bool			rollerDetected;
	} A3merge_Param;
#endif
