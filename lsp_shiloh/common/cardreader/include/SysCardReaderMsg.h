#ifndef __SYS_CARD_READER_MSG_H__
#define __SYS_CARD_READER_MSG_H__

#include "ATypes.h"


//Group 03 Sysmgr and AuthMgr(CardReader) communication
#define CMD_INT_SC_AUTH_REQ    0x0361
#define CMD_INT_SC_AUTH_CONF_CHANGED    0x0362
#define CMD_INT_CS_AUTH_RESULT 0x0321

///<Grp:03H Cmd:01h [ syscard ← sysmgr ]  Login/logout/countUP request.
typedef struct stAuth_AuthReq_ {
    UINT8       action; // See AUTH_REQUEST_TYPE. 
    UINT8       jobType; // bit(0) : Print, bit(1) : Scan, bit(2) : Fax, bit(3) : Copy, bit(4) : Admin
                 // 0 for no permission, 1 for allowed.
                                 // Login = 0, Print = 1, Scan = 2, Fax = 4, Copy = 8, Logout = 16
    UINT8       jobCount; // each count of job
}stAuth_Req;

///<Grp:03H Cmd:20h [ syscard → sysmgr ]  login result
typedef struct stAuth_AuthResult_ {
    UINT8       act; // 1: login, 2: logout, 4: CardTag, 5: Sol Auth 6:Print Auth
    UINT8       result;   // See AUTH_RESULT_TYPE.
    UINT8       permission; // bit(0) : Print, bit(1) : Scan, bit(2) : Fax, bit(3) : Copy, bit(4) : Admin
                    // 0 for no permission, 1 for allowed.
                    // Bit(4) Admin is don't care about other permissions.
                    // Ex) 3 = P+S, 5 = P+F, 11 = P+S+C,
}stAuth_Result;

// auth request status code
typedef enum {
    AUTH_LOGIN = 1,
    AUTH_LOGOUT = 2,
    AUTH_COUNTUP = 3,
    AUTH_CARD_TAG = 4,
    AUTH_SOL_AUTH = 5,
    AUTH_NET_PRINT = 6,
}AUTH_REQUEST_TYPE;

// auth result code
typedef enum {
    AUTH_SUCCESS = 0,
    AUTH_FAIL_NOID = 1,
    AUTH_FAIL_WRONGID = 2,
    AUTH_FAIL_QUOTA = 3,
    AUTH_FAIL_CON_SERVER = 4,
    AUTH_FAIL_INTERFACE = 5,
    AUTH_FAIL_PC_CLIENT = 6,
    AUTH_FAIL_PC_NODATA = 7,
}AUTH_RESULT_TYPE;

// Prefer type code
typedef enum {
    AUTH_PREFER_EMAIL = 0,
    AUTH_PREFER_FTP = 1,
}AUTH_PREFER_TYPE;

typedef enum {
    AUTH_INT_LOGIN_CARD = 0,
    AUTH_INT_LOGIN_PANEL = 1,
    AUTH_INT_VERIFY = 2,
    AUTH_INT_LOGOUT = 3,
    AUTH_INT_COUNTUP = 4,
}AUTH_INT_CONNECTION_TYPE;

typedef enum {
    AUTH_INT_SUCCESS = 0,
    AUTH_INT_FAIL_NOID = 1,
    AUTH_INT_FAIL_WRONGID = 2,
}AUTH_INT_ACC_CHECK_TYPE;


#define MAX_CODE_LENGTH 201     // 200+1
#define AUTH_BASIC_LENGTH 33    // 32+1
#define AUTH_EMAIL_LENGTH 129   // 128+1
#define AUTH_FTPADDR_LENGTH 193 // 192+1

typedef struct stAuth_AuthInfo_{
    char userID[MAX_CODE_LENGTH];
    char userPwd[AUTH_BASIC_LENGTH];
    char authID[AUTH_BASIC_LENGTH];
    char email[AUTH_EMAIL_LENGTH];
    char ftpServer[AUTH_FTPADDR_LENGTH];  
    char ftpID[AUTH_BASIC_LENGTH];
    char ftpPwd[AUTH_BASIC_LENGTH];
    int ftpPort;
    char printJobID[AUTH_BASIC_LENGTH];
    char scanJobID[AUTH_BASIC_LENGTH];
    char faxJobID[AUTH_BASIC_LENGTH];
    char copyJobID[AUTH_BASIC_LENGTH];
    int quotaRemain;
    int prefer;
    char printAuthID[AUTH_BASIC_LENGTH];
    char printAuthPW[AUTH_BASIC_LENGTH];
}stAuth_AuthInfo;


typedef struct stAuth_ParsedData_{
    char upID[AUTH_BASIC_LENGTH];
    char upIP[AUTH_BASIC_LENGTH];
    char email[AUTH_EMAIL_LENGTH];
    char ftpServer[AUTH_FTPADDR_LENGTH];         
    char ftpID[AUTH_BASIC_LENGTH];
    char ftpPwd[AUTH_BASIC_LENGTH];
    int ftpPort;
    int quotaRemain;
    int prefer;
    //
    int permission;
    int result; 
}stAuth_ParsedData;

typedef struct stPrintInformationJobLog_ {
	int		count;
	char	serial[20];
	int		jobId;
	char	fileName[255];
	char	paperSize[10];
	int		duplex;
} stPrintInformationJobLog;

void GoJobLog(stPrintInformationJobLog* inform);
#ifdef SYSCARD_MAIN_C
stAuth_AuthInfo AuthInfo;
#else
extern stAuth_AuthInfo AuthInfo;
#endif

#endif //__SYS_CARD_READER_MSG_H__
