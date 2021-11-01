#ifndef __FAXAPP_PARAM__
#define __FAXAPP_PARAM__

#include "SDHFaxCommonHeader.h"


#define GET     0
#define SET     1


typedef unsigned char BOOLEAN;


enum {
    STATE_IDLE,
    STATE_RX_PHASE_A,
    STATE_RX_PHASE_B,
    STATE_RX_PHASE_B_SIGNAL_REC,
    STATE_RX_PHASE_C,
    STATE_RX_PHASE_C_ECM,
    STATE_RX_PHASE_D_SIGNAL_REC,
    STATE_RX_PHASE_D,
    STATE_RX_PHASE_E,
    STATE_RX_DISCONN,
    STATE_RX_ABORT,
    STATE_TX_PHASE_A,
    STATE_TX_PHASE_B,
    STATE_TX_SET_MODE,
    STATE_TX_PHASE_C,
    STATE_TX_PHASE_C_ECM,
    STATE_TX_PHASE_D,
    STATE_TX_PHASE_D2,
    STATE_TX_PHASE_E,
    STATE_TX_DISCONN,
    STATE_TX_VOICE_REQ,
    STATE_TX_ABORT,
    STATE_HOST_ABORT
};


typedef enum {
    FXA_INIT_ATTRIBUTE,
    FXA_COUNTRY_CODE,
    FXA_ATTENUATION,
    FXA_BUFFERED_DELAFTERTX,
    FXA_BUFFEREDPRINT,
    FXA_CEDDETECT_TIMEOUT,      // 5
    FXA_COVERPAGE,
    FXA_COVERPAGE_ID,
    FXA_DELAYEDTXDOCID,
    FXA_DOCID,
    FXA_ECMMODE,                // 10
    FXA_EQUALIZER,
    FXA_FALLBACK_NOOFTRIES,
    FXA_FAXFORWARD,
    FXA_FAXFORWARD_NO,
    FXA_FAXLOGGING,             // 15
    FXA_IGNOREECHO,
    FXA_IGNOREECM,
    FXA_IGNOREMMR,
    FXA_IGNOREMR,
    FXA_IGNOREJBIG,             // 20
    FXA_IGNOREJBIGL0,
    FXA_IGNOREJPEG,
    FXA_IGNOREFULLCOLOR,
    FXA_IGNOREPAGEMEMORY,
    FXA_IGNORE_VOICEREQ,        // 25
    FXA_IMGPROC,
    FXA_INTLMODE,
    FXA_LASTTXLOG,
    FXA_LINEREQ,
    FXA_LOCALID,                // 30
    FXA_LOGTYPE_INIT,
    FXA_LOG_ERRORRESULT,
    FXA_LOG_PRINT,
    FXA_LOG_REMOTENUMBER,
    FXA_PAGENO,                 // 35
    FXA_REMOTEID,
    FXA_TXRESOLUTION,
    FXA_RXRESOLUTION,
    FXA_FORCERXTOMEM,
    FXA_FORCERXTOMEM_COLOR,     // 40
    FXA_RXMAXGAIN,
    FXA_SPEAKER_CALLMODE,
    FXA_SPEAKER_ANSWERMODE,
    FXA_SPEAKERVOLUME,
    FXA_TXSTARTSPEED,           // 45
    FXA_RXSTARTSPEED,
    FXA_T1TIMEOUT,
    FXA_T5TIMEOUT,
    FXA_TCFERRORS,
    FXA_TEXTHEADER,             // 50
    FXA_V29EPT,
    FXA_VOICEREQ,
    FXA_TXREQUIREPASSWORD,
    FXA_RXREQUIREPASSWORD,
    FXA_TXPASSWORD,             // 55
    FXA_RXPASSWORD,
    FXA_REMOTEPASSWORD,
    FXA_TXREQUIRESUBADDRESS,
    FXA_RXREQUIRESUBADDRESS,
    FXA_TXSUBADDRESS,           // 60
    FXA_RXSUBADDRESS,
    FXA_REMOTESUBADDRESS,
    FXA_POLLTXMODE,
    FXA_POLLRXMODE,
    FXA_POLLTXREQUIREPASSWORD,  // 65
    FXA_POLLRXREQUIREPASSWORD,
    FXA_POLLTXPASSWORD,
    FXA_POLLRXPASSWORD,
    FXA_POLLREMOTEPASSWORD,
    FXA_POLLTXREQUIRESUBADDRESS,// 70
    FXA_POLLRXREQUIRESUBADDRESS,
    FXA_POLLTXSUBADDRESS,
    FXA_POLLRXSUBADDRESS,
    FXA_POLLREMOTESUBADDRESS,
    FXA_ENABLENSF,              // 75
    FXA_LOCALNSF,
    FXA_LOCALNSC,
    FXA_LOCALNSS,
    FXA_REMOTENSF,
    FXA_REMOTENSC,              // 80
    FXA_REMOTENSS,
    FXA_TXIMAGEWIDTH,
    FXA_RXIMAGEWIDTH,
    FXA_TXDATASOURCE,
    FXA_DIALMODE,               // 85
    FXA_ANSWERMODE,
    FXA_AUTOANSWERRINGS,
    FXA_RETRY_NUMBER,
    FXA_RETRY_INTERVAL,
    FXA_BRIGHTNESS,             // 90
    FXA_CONTRAST,
    FXA_TTI_ENABLE,
    FXA_SPEAKERV8OFF,
#if __SUPPORT_MANUAL_RECEIVE__
    FXA_RXV34ABORTBADEQM,
    FXA_MANUALRECEIVE,            // 95
    FXA_RECEIVESOURCE
#else
    FXA_RXV34ABORTBADEQM
#endif
}T30_ATTRIB;


enum {
    T30_SPEED_V21_300,
    T30_SPEED_V27_2400,
    T30_SPEED_V27_4800,
    T30_SPEED_V29_7200,
    T30_SPEED_V29_9600,
    T30_SPEED_V17_7200,
    T30_SPEED_V17_9600,
    T30_SPEED_V17_12000,
    T30_SPEED_V17_14400,
    T30_SPEED_V34_2400,
    T30_SPEED_V34_4800,
    T30_SPEED_V34_7200,
    T30_SPEED_V34_9600,
    T30_SPEED_V34_12000,
    T30_SPEED_V34_14400,
    T30_SPEED_V34_16800,
    T30_SPEED_V34_19200,
    T30_SPEED_V34_21600,
    T30_SPEED_V34_24000,
    T30_SPEED_V34_26400,
    T30_SPEED_V34_28800,
    T30_SPEED_V34_31200,
    T30_SPEED_V34_33600
};


enum {
    PHONE_SELECT_COUNTRY,
    PHONE_DIAL_TIMEOUT,
    PHONE_TRANSMIT_LEVEL,
    PHONE_DTMF_LOW_LEVEL,
    PHONE_DTMF_HIGH_LEVEL,
    PHONE_PULSE_MAKE_TIME,
    PHONE_PULSE_BREAK_TIME,
    PHONE_PULSE_DIAL_TYPE,
    PHONE_INTERDIGIT_DELAY,
    PHONE_DC_CHARACTERISTIC,
    PHONE_IMPEDANCE,
    PHONE_CALLER_ID_TYPE,
    PHONE_BUSY_TONE_CYCLE,
    PHONE_BUSY_TONE_MIN_ON_TIME,
    PHONE_BUSY_TONE_MAX_ON_TIME,
    PHONE_BUSY_TONE_MIN_OFF_TIME,
    PHONE_BUSY_TONE_MAX_OFF_TIME,
    PHONE_EXTENSION_SUPPORT,
    PHONE_PULSE_FALL_TIME,
    PHONE_EXTENSION_VOLTAGE_THRESHOLD,
    PHONE_BLIND_DIAL,
    PHONE_REF_ONHOOK_VOLTAGE,
    PHONE_REF_LINEINUSE_VOLTAGE,
    PHONE_LINE_IN_USE_THRESHOLD
};


typedef enum {
    LINE_MONITOR_OFF, // 0
    LINE_MONITOR_ON_TILL_CONNECT, // 1
    LINE_MONITOR_ALWAYS_ON, // 2

    LINE_MONITOR_LAST
} LINE_MONITOR_t;


typedef enum {
    FAXAPP_UPDATE_PARAM_STATION_ID = 0,
    FAXAPP_UPDATE_PARAM_STATION_NUMBER, // 1
    FAXAPP_UPDATE_PARAM_TTI, // 2
    FAXAPP_UPDATE_PARAM_LINE_MONITOR, // 3
    FAXAPP_UPDATE_PARAM_VOLUME, // 4
    FAXAPP_UPDATE_PARAM_TX_RETRY_NUMBER, // 5
    FAXAPP_UPDATE_PARAM_TX_RETRY_INTERVAL, // 6
    FAXAPP_UPDATE_PARAM_ECM, // 7
    FAXAPP_UPDATE_PARAM_FAX_FORWARD, // 8
    FAXAPP_UPDATE_PARAM_FORWARD_NUMBER, // 9
    FAXAPP_UPDATE_PARAM_CONFIDENTIAL_RX, // 10
    FAXAPP_UPDATE_PARAM_PASSWORD_FOR_CONFIDENTIAL_RX, // 11
    FAXAPP_UPDATE_PARAM_RX_RING_NUM, // 12
    FAXAPP_UPDATE_PARAM_RX_REDUCTION_SEL, // 13
    FAXAPP_UPDATE_PARAM_CONFIDENTIAL_TX, // 14
    FAXAPP_UPDATE_PARAM_PASSWORD_FOR_CONFIDENTIAL_TX, // 15
    FAXAPP_UPDATE_PARAM_PRINT_MODE, // 16
    FAXAPP_UPDATE_PARAM_DISCARD_LENGTH, // 17
    FAXAPP_UPDATE_PARAM_DUPLEX_PRINT, // 18
    FAXAPP_UPDATE_PARAM_INPUT_TRAY, // 19

    FAXAPP_UPDATE_PARAM_TX_START_SPEED = 30,
    FAXAPP_UPDATE_PARAM_RX_START_SPEED, // 31
    FAXAPP_UPDATE_PARAM_ENCODING_TYPE, // 32
    FAXAPP_UPDATE_PARAM_COUNTRY_CODE, // 33
    FAXAPP_UPDATE_PARAM_TRANSMIT_LEVEL, // 34
    FAXAPP_UPDATE_PARAM_DTMF_LOW_LEVEL, // 35
    FAXAPP_UPDATE_PARAM_DTMF_HIGH_LEVEL, // 36
    FAXAPP_UPDATE_PARAM_PULSE_MAKE_TIME, // 37
    FAXAPP_UPDATE_PARAM_PULSE_BREAK_TIME, // 38
    FAXAPP_UPDATE_PARAM_INTER_DIGIT_DELAY, // 39
    FAXAPP_UPDATE_PARAM_DC_CHARACTERISTICS, // 40
    FAXAPP_UPDATE_PARAM_IMPEDANCE, // 41
    FAXAPP_UPDATE_PARAM_BUSY_TONE_CYCLE, // 42
    FAXAPP_UPDATE_PARAM_BUSY_TONE_MIN_ON_TIME, // 43
    FAXAPP_UPDATE_PARAM_BUSY_TONE_MAX_ON_TIME, // 44
    FAXAPP_UPDATE_PARAM_BUSY_TONE_MIN_OFF_TIME, // 45
    FAXAPP_UPDATE_PARAM_BUSY_TONE_MAX_OFF_TIME, // 46
    FAXAPP_UPDATE_PARAM_EXTENSION_SUPPORT, // 47
    FAXAPP_UPDATE_PARAM_EXTENSION_VOLTAGE_THRESHOLD, // 48
    FAXAPP_UPDATE_PARAM_MIN_DETECTABLE_TIP_RING_VOLT, // 49
    FAXAPP_UPDATE_PARAM_DIAL_TONE_DETECTION, // 50
    FAXAPP_UPDATE_PARAM_DIAL_TIMEOUT, // 51
    FAXAPP_UPDATE_PARAM_CED_DETECTION_TIME, // 52
    FAXAPP_UPDATE_PARAM_T1_TIMEOUT, // 53
    FAXAPP_UPDATE_PARAM_T5_TIMEOUT, // 54
    FAXAPP_UPDATE_PARAM_TCF_ERROR_RATE, // 55
    FAXAPP_UPDATE_PARAM_NO_OF_TRIES_ON_FALLBACK, // 56
    FAXAPP_UPDATE_PARAM_RECEIVE_MAX_GAIN, // 57
    FAXAPP_UPDATE_PARAM_DELAY_FOR_PAUSE_KEY, // 58
    FAXAPP_UPDATE_PARAM_AUTO_DARKNESS, //59
    FAXAPP_UPDATE_PARAM_BACKGROUND_REMOVE, //60
    FAXAPP_UPDATE_PARAM_SPAM_FAX, // 61
    FAXAPP_UPDATE_PARAM_LINE_IN_USE_THRESHOLD, // 62

#if __SUPPORT_CID__
    FAXAPP_UPDATE_PARAM_CID, // 63
#else
#endif

#if __TX_REPORT_IMAGE_PRINT__
	FAXAPP_UPDATE_PARAM_TX_RESULT_RPT_IMAGE = 64,
#endif

    FAXAPP_UPDATE_PARAM_SAVE_FAX_FILE = 70,
    FAXAPP_UPDATE_PARAM_FILE_FORMAT, // 71
    FAXAPP_UPDATE_PARAM_PAGE_TYPE, // 72
    FAXAPP_UPDATE_PARAM_SEND_DESTINATION, // 73
    FAXAPP_UPDATE_PARAM_EMAIL_SERVER_ADDRESS, // 74
    FAXAPP_UPDATE_PARAM_EMAIL_ID, // 75
    FAXAPP_UPDATE_PARAM_EMAIL_PW, // 76
    FAXAPP_UPDATE_PARAM_EMAIL_PORT, // 77
    FAXAPP_UPDATE_PARAM_SENDER_EMAIL_ADDRESS, // 78
    FAXAPP_UPDATE_PARAM_RECEIVER_EMAIL_ADDRESS, // 79
    FAXAPP_UPDATE_PARAM_EMAIL_SUBJECT, // 80
    FAXAPP_UPDATE_PARAM_EMAIL_TEXT, // 81
    FAXAPP_UPDATE_PARAM_FTP_ADDRESS, // 82
    FAXAPP_UPDATE_PARAM_FTP_PATH, // 83
    FAXAPP_UPDATE_PARAM_FTP_ID, // 84
    FAXAPP_UPDATE_PARAM_FTP_PW, // 85
    FAXAPP_UPDATE_PARAM_FTP_PORT, // 86
    FAXAPP_UPDATE_PARAM_SMB_ADDRESS, // 87
    FAXAPP_UPDATE_PARAM_SMB_PATH, // 88
    FAXAPP_UPDATE_PARAM_SMB_ID, // 89
    FAXAPP_UPDATE_PARAM_SMB_PW, // 90
    FAXAPP_UPDATE_PARAM_SMB_PORT, // 91
    FAXAPP_UPDATE_PARAM_WEBDAV_ADDRESS, // 92
    FAXAPP_UPDATE_PARAM_WEBDAV_PATH, // 93
    FAXAPP_UPDATE_PARAM_WEBDAV_ID, // 94
    FAXAPP_UPDATE_PARAM_WEBDAV_PW, // 95
    FAXAPP_UPDATE_PARAM_WEBDAV_PORT, // 96
#if __SUPPORT_MANUAL_RECEIVE__
    FAXAPP_UPDATE_MANUAL_RECEIVE, // 97
    FAXAPP_UPDATE_REMOTE_ACCESS_CODE, // 98
#else
#endif

	FAXAPP_UPDATE_PARAM_FAXTOX_SET = 115,//115
	FAXAPP_UPDATE_PARAM_RING_DETECT = 116,

    FAXAPP_UPDATE_PARAM_LAST
} FAXAPP_UPDATE_PARAM_t;
#endif // #ifndef __FAXAPP_PARAM__


#if __SUPPORT_MANUAL_RECEIVE__
/* Answer mode defines */
#define AUTO_ANSWER      0
#define MANUAL_ANSWER    1

/* Receive mode defines */
#define RECEIVE_MODE_FAX       0
#define RECEIVE_MODE_PHONE     1

/* Receive source defines */
#define RECEIVE_SOURCE_MACHINE     0
#define RECEIVE_SOURCE_PHONE       1
#else
#endif
