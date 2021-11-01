
#ifndef __FAXAPP_NOTIFY__
#define __FAXAPP_NOTIFY__

#include "faxapp_sc_code.h"
#include "../../CommonHeader/SDHFaxCommonHeader.h"

typedef enum {
    FAX_APP_STATUS_MSG_SCANNING_FROM_FB, // 0
    FAX_APP_STATUS_MSG_SCANNING_FROM_ADF, // 1
    FAX_APP_STATUS_MSG_SCANNING_COMPLETE, // 2
    FAX_APP_STATUS_MSG_DIALING, // 3
    FAX_APP_STATUS_MSG_DIAL_NO_TRANSMIT, // 4
    FAX_APP_STATUS_MSG_CONNECTING, // 5
    FAX_APP_STATUS_MSG_CONNECTED_SPEED, // 6
    FAX_APP_STATUS_MSG_SENDING, // 7
    FAX_APP_STATUS_MSG_SENDING_COMPLETE, // 8
    FAX_APP_STATUS_MSG_RINGING, // 9
    FAX_APP_STATUS_MSG_RECEIVING, // 10
    FAX_APP_STATUS_MSG_RECEIVING_COMPLETE, // 11
    FAX_APP_STATUS_MSG_PRINTING, // 12
    FAX_APP_STATUS_MSG_PRINTING_COMPLETE, // 13
    FAX_APP_STATUS_MSG_NO_LINE, // 14
    FAX_APP_STATUS_MSG_LINE_IS_ON, // 15
    FAX_APP_STATUS_MSG_OFF_HOOK_DONE, // 16
    FAX_APP_STATUS_MSG_ON_HOOK_DONE, // 17
    FAX_APP_STATUS_MSG_SENDING_FAIL, //18
    FAX_APP_STATUS_MSG_RECEIVING_FAIL, //19
    FAX_APP_STATUS_MSG_NON_COMPLETED_FAIL, //20
    FAX_APP_STATUS_MSG_FB_EMPTY_FOR_MERGE, //21
    FAX_APP_STATUS_MSG_REQUEST_FOR_LEGAL_SIZE_MERGE, //22
    FAX_APP_STATUS_MSG_SCAN_FAIL, //23
    FAX_APP_STATUS_MSG_CANCEL_COMPLETE, //24
    FAX_APP_STATUS_MSG_POLL_TX_START, // 25
    FAX_APP_STATUS_MSG_ADF_PAPER_IN_MERGE, // 26
    FAX_APP_STATUS_MSG_DIAL_NO_RECEIVE, // 27
    FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_FROM_PC, //28
    FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_COMPLETE, //29
    FAX_APP_STATUS_MSG_PCFAX_IMAGE_SAVING_FAIL, //30
    FAX_APP_STATUS_MSG_CLEAR_STATUS_BAR, // 31
    FAX_APP_STATUS_MSG_PCFAX_DIAL_NO, // 32
    FAX_APP_STATUS_MSG_PAGE_PRINT_END, // 33
    FAX_APP_STATUS_MSG_NO_LEGAL_PAPER_IN_ADF, //34
    FAX_APP_STATUS_MSG_POLL_RX_START, // 26

#if __SUPPORT_MANUAL_TRANSMIT__ || __SUPPORT_MANUAL_RECEIVE__
    FAX_APP_STATUS_MSG_PICK_UP_RECEIVER, // 27, e.g.) Press Fax No.
    FAX_APP_STATUS_MSG_HANG_UP_RECEIVER, // 28
    FAX_APP_STATUS_MSG_PUT_SPECIAL_NUMBER, // 29, e.g.) Press *, 0, * To Receive
    FAX_APP_STATUS_MSG_PRESS_START_TO_SEND, // 30, e.g.) Press Start To Send
    FAX_APP_STATUS_MSG_PRESS_START_TO_RECEIVE, // 31, e.g.) Press Start To Receive
#else
#endif

FAX_APP_STATUS_MSG_FAX_RECEIVE_JOB_CREATED = 52,//52

	FAX_APP_STATUS_MSG_NEXT_DOCUMENT_WAIT = 70,
#if __HANA_FINANCIAL_GROUP_REQ_014_5__
	FAX_APP_STATUS_MSG_DELETE_COMPLETE, //71
	FAX_APP_STATUS_MSG_DELETE_FAIL, //72
	FAX_APP_STATUS_MSG_FILENAME_MISMATCH, //73
	FAX_APP_STATUS_MSG_JOB_CANCELED, //74
#endif

    FAX_APP_STATUS_MSG_LAST
} FAX_APP_STATUS_MSG_t;

#define EMPTY_JOB_NUM 600

void faxapp_notify_SendFaxStatus(FAX_APP_STATUS_MSG_t StatusType, int CurrentPageNo, int TotalPageNo, int ConnectedSpeed, unsigned int SharedMemID, int JobID);
void faxapp_notify_SendFaxMemoryStatus(int FaxMemory);
void faxapp_notify_SendFaxBroadcastJobID(int firstJobID, int number, int jobID);
void faxapp_notify_SendFaxSCStatus(int ErrorNum);
void faxapp_notify_SendFaxRequestReadDB(int JobCount);
void faxapp_notify_SendPCFaxJobID(int firstJobID, int number, int jobID);

extern void SendSCStatusToUI(UINT32 scCode, UINT32 senderModule);

#endif // #ifndef __FAXAPP_NOTIFY__