/*
 *  $RCSfile: ECM.H,v $
 * $Revision: 1.4 $
 *   $Author: yooh1 $
 *     $Date: 2010/11/01 23:21:43 $
 */
/************************************************************************
 *
 *      ECM.H
 *
 *  T.30 Error Correction Mode Definitions.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

/* ECM block structure */
typedef struct ecmblock   {
	UINT8 		Status;	 /* 0 not ready, 1 = ready for processing, 2 = done processing */
	UINT8 		FrameCnt;
	//CDB 	   *Frame[MAX_ECM_FRAMES];
	CDB_BUFFER 	   *Frame[MAX_ECM_FRAMES];
} T_ECM_BLOCK;

/* ECM control structure */
typedef struct ecmcontrol   {
	T_ECM_BLOCK Block[MAX_ECM_BLOCKS];
	UINT16 		BlockFront;       /* Front Block index (READ) */
	UINT16 		BlockBack;        /* Back Block index (WRITE) */
	UINT16 		BytesPerFrame;    /* 256 = 256 byte frame, 64 = 64 byte frame */
	UINT8		CDBFrameIn;		  /* The current frame being fed in from the T4 service */
	UINT8		NetBlockCount;	  /* Net number of blocks being processed in ECM module */
	UINT8  		PPRCnt;           /* Number of times PPR received during a block */
	UINT8  		PPS_FIF[4];       /* PPS FIF for ECM  */
	UINT8  		PPR_FIF[32];      /* PPR FIF for ECM */
    UINT8       FrameDone;        /* Frame done (transmitted or received) indication */
   	UINT8  		TxBlockNum;		  /* Block number being transmitted */
   	UINT8  		TxPageNum;		  /* Page number being transmitted */
   	UINT8  		TxTotalFrameCnt;  /* Total frames transmitted for current block - 1 */
	UINT8		TxEndOfPage;	  /* End of page flag (i.e., final CDB) */
	UINT8		TxFirstFrame;	  /* The first frame being sent for the block */
	UINT8  		RxBlockNum;       /* Block number being received and decompressed - 1 */
	UINT8  		RxPageNum;        /* Page number being received and decompressed - 1 */
	UINT16 		RxTotalFrameCnt;  /* Total frames received */
	UINT16 		RxGoodFrameCnt;   /* Good frames received  */
	UINT16		RxFrameSize[MAX_ECM_FRAMES];		// number of byte on the last frame	to use total bytes in pBuff  T4JOBNODE
} T_ECM_CONTROL;

typedef struct ecm_buffer_q
{
	UINT8 frame_num;
	void *next;
} ECM_BUFF_QUEUE;

extern T_ECM_CONTROL *ECMControl;

extern void ECMEndFrame(void);
extern void ECMRxCheckCopyQuality(void);
extern UINT8 ECMCheckPPR(UINT8 frame_number);
extern UINT8 ECMInitJob(UINT8 frame_size);
extern UINT8 ECMShutdownJob(void);
extern UINT8 ECMCDBCheckEndOfPage(UINT8 frame_number);
extern void ECMRxPrintBlock(void);
extern void ECMRxPrintRemainingBlocks(void);
extern void ECMRxBlock(void);
extern void ECMRxFrames(void);
extern UINT8 ECMRxVerifyFrameLength(void);
extern UINT8 ECMRxVerifyFrameHeader(void);
extern UINT8 ECMRxStoreFCDFrame(void);
extern UINT8 ECMRxStartPrint(void);
extern void ECMRxProcessPPS(void);
extern void ECMRxEndOfBlockResponse(void);
extern void ECMRxProcessRR(void);
extern void ECMRxProcessCTC(void);
extern void ECMRxProcessEOR(void);
extern void ECMRxProcessPPS_PRIQ(void);
extern void ECMNextBlock(void);
extern void ECMBlockDone(void);
extern void ECMTxBlock(void);
extern void ECMTxFCDFrame( UINT8 frame_number );
extern void ECMTxRCPFrames(void);
extern void ECMTxSetPPS( UINT8 fRetransmit );
extern T_T30_STATES ECMTxPhaseDProcessResponse(UINT8 response, UINT8 table_index);
extern T_T30_STATES ECMTxProcessPPR(void);
extern T_T30_STATES ECMTxProcessRNR(void);
extern void ECMTxSetPRIQ(void);
