/*
 *  $RCSfile: HDLC.H,v $
 * $Revision: 1.11 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:28:15 $
 */
/************************************************************************
 *
 *      HDLC.H
 *
 *  Definitions for HDLC Framing.
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

/* HDLC Frame Format - Basic */
/*
---------------------------------------------) )-------------------------------
| FLAG  | ADDRESS  | CONTROL  | INFORMATION	(  (      |  FCS	      | FLAG  |
| (7E)  | (1 byte) | (1 byte) | (n bytes)    )  )     |  2 bytes)     | (7E)  |
--------------------------------------------(  (-------------------------------
*/
#if !defined (__HDLC_H__)
#define __HDLC_H__

/* The maximum size of information field is dictated by an ECM frame */
#define MAX_INFORMATION_FIELD   260

/* Max frames is dictated by number of ECM frames to receive */
#define MAX_HDLC_RX_FRAMES      20
#define MAX_HDLC_TX_FRAMES      3

/* The HDLC Frame Structure used to transmit and receive frames */
typedef struct hdlcframe {
	UINT8  addr;
	UINT8  control;
	UINT8  info[ MAX_INFORMATION_FIELD ];
	UINT16 length;
	UINT16 readbytenum;
	UINT16 writebytenum;
	UINT8  status;
	UINT8  fAddressField;
	UINT8  fControlField;
	UINT16 maxlength;
	UINT8  final;
} T_HDLC_FRAME;

/* The HDLC Control structure */
typedef struct hdlcctl {
	T_HDLC_FRAME  TxFrames[ MAX_HDLC_TX_FRAMES ];
	T_HDLC_FRAME  RxFrames[ MAX_HDLC_RX_FRAMES ];
	UINT8  TxFrameFront;
	UINT8  TxFrameBack;
	UINT8  TxFrameCount;
	UINT8  RxFrameFront;
	UINT8  RxFrameBack;
	UINT8  RxFrameCount;
	UINT8  VfyFlags;
} T_HDLC_CONTROL;

/* Frame Error Codes */
#define HDLC_NO_ERROR               0
#define HDLC_NO_CARRIER_ERROR       1
#define HDLC_FRAME_ERROR            2
#define HDLC_CHECKSUM_ERROR         3
#define HDLC_CARRIER_LOST_ERROR     4
#define HDLC_WRONG_CARRIER_ERROR    5
#define HDLC_PC_TO_CC_TRANSITION    6
#define HDLC_INVALID_EOF            7
#define HDLC_CI_TONE                8
#define HDLC_TIME_OUT               9
#define HDLC_NO_FRAME_YET           99

#define HDLCRxReadAddress()      HDLCControl->RxFrames[HDLCControl->RxFrameFront].addr
#define HDLCRxWriteAddress(x)    HDLCControl->RxFrames[HDLCControl->RxFrameBack].addr = (x)
#define HDLCTxReadAddress()      HDLCControl->TxFrames[HDLCControl->TxFrameFront].addr
#define HDLCTxWriteAddress(x)    HDLCControl->TxFrames[HDLCControl->TxFrameBack].addr = (x)
#define HDLCRxReadControl()      HDLCControl->RxFrames[HDLCControl->RxFrameFront].control
#define HDLCTxReadControl()      HDLCControl->TxFrames[HDLCControl->TxFrameFront].control
#define HDLCTxWriteControl(x)    HDLCControl->TxFrames[HDLCControl->TxFrameBack].control = (x)
#define HDLCRxWriteControl(x)    HDLCControl->RxFrames[HDLCControl->RxFrameBack].control = (x)
#define HDLCTxWriteFinalFrame()  HDLCControl->TxFrames[HDLCControl->TxFrameBack].final = 1
#define HDLCTxReadFinalFrame()   HDLCControl->TxFrames[HDLCControl->TxFrameFront].final
#define HDLCRxWriteMaxLength(x)  HDLCControl->RxFrames[HDLCControl->RxFrameBack].maxlength = (x)
#define HDLCRxReadLength()       HDLCControl->RxFrames[HDLCControl->RxFrameFront].length
#define HDLCTxReadLength()       HDLCControl->TxFrames[HDLCControl->TxFrameFront].length
#define HDLCRxWriteStatus(x)     HDLCControl->RxFrames[HDLCControl->RxFrameBack].status = (x)
#define HDLCRxReadStatus()       HDLCControl->RxFrames[HDLCControl->RxFrameFront].status
#define HDLCRxWriteFinalFrame()  HDLCControl->RxFrames[HDLCControl->RxFrameBack].final = 1
#define HDLCRxReadFinalFrame()   HDLCControl->RxFrames[HDLCControl->RxFrameFront].final
#define HDLCRxReadInitFrame()    HDLCControl->RxFrames[HDLCControl->RxFrameFront].readbytenum = 0
#define HDLCRxReadCommand()      HDLCControl->RxFrames[HDLCControl->RxFrameFront].info[0]
#define HDLCTxWriteFrameAvailable() (HDLCControl->TxFrameCount < MAX_HDLC_TX_FRAMES)
#define HDLCTxReadFrameAvailable() HDLCControl->TxFrameCount
#define HDLCRxReadFrameAvailable() HDLCControl->RxFrameCount


extern T_HDLC_CONTROL *HDLCControl;

extern UINT8 HDLCAllocateMemory(void);
extern void HDLCDeAllocateMemory(void);
extern void HDLCTxWritePreviousFrameFinal(void);
extern void HDLCRxWriteInitFrame(void);
extern void HDLCTxWriteInitFrame(void);
extern void HDLCRxWriteInitAllFrames(void);
extern void HDLCTxWriteInitAllFrames(void);
extern UINT8 HDLCRxReadDataByte(void);
extern void HDLCRxWriteDataByte(UINT8);
extern UINT8 HDLCTxReadDataByte(void);
extern void HDLCTxWriteDataByte(UINT8);
extern void HDLCTxWriteInitResendFrame(void);
extern UINT16 HDLCRxReadDataToBuff(UINT8 *cdb);
extern void HDLCTxWriteDateFromCDB(CDB *cdb);
extern void HDLCTxReadNextFrame(void);
extern void HDLCTxWriteNextFrame(void);
extern void HDLCRxReadNextFrame(void);
extern void HDLCRxWriteNextFrame(void);
extern UINT8 HDLCRxVerifyFlags(UINT16);
extern UINT8 HDLCRxCheckFinalFrame(void);
extern UINT8 HDLCRxControlFrame(void);
extern void HDLCTxControlFrame(void);
extern void HDLCRxDataFrame(UINT8,UINT8,UINT8,void (*)(void),void (*)(void));
extern void HDLCTxInitialFlags (UINT8 speed, UINT8 training);
extern void HDLCTxDataFrame(UINT8,UINT8,UINT8, void (*frame_done_notify)(void));
extern UINT8 HDLCRxCommand(UINT8, void *);
extern UINT8 HDLCRxResponse(UINT8,UINT8 *,UINT16,void (*)(void),void (*)(void));

/* Frame Format - Extended */
/*
----------------------------------------------) )--------------------------------
| FLAG  | ADDRESS  | CONTROL   | INFORMATION (  (      |  FCS	      | FLAG	|
| (7E)  | (N bytes)|(1-2 bytes)| (n bytes)    )  )     |  2 bytes)    | (7E)	|
---------------------------------------------(  (--------------------------------
*/

#endif // __HDLC_H__
