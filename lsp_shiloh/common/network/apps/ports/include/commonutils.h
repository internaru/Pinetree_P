#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <libgen.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <mqueue.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include "agMessage.h"
#include "agRouter.h"
#include "SysUI_Interface.h"
//#include <curl/curl.h>
#include <sys/statvfs.h>
#include <ctype.h>
#define MAX_URL_SIZE    1024
#define FTP_TIMEOUT             120
#define WEBDAV_TIMEOUT      120

#define MQ_DEFAULT_PRIORITY    0
#define PORTS_MQUEUE_NAME      "/mq_ui_to_sys"


#define ERROR_CONFIG 100001
#define ERROR_AUTH 100002
#define ERROR_CONNECT 100003
#define ERROR_DATA_SEND 100004
#define ERROR_BEYOND_ALLOWED_LIMIT 100005
#define ERROR_FAILED 100006


#ifndef stSolution_Request
typedef struct stSolution_Response_
{
   UINT32 sendRequestAppMsgQueueId;
   UINT32 sendJobId;
   UINT32 systemJobID;       // unique system JobID
   void   *reserved;
} stSolution_Request;
#endif

typedef struct
{
   char                *username;
   char                *password;
   char                *senderEmail;
   char                *senderName;
   char                *mailhost;
   int                 port;
   char                *attachmentPath;
   char                *toAddress;
   char                *subject;
   char                *content;
   int                 secureType;
   int                 authType;
   int                 isDebugMode;
   stSolution_Request  *email_msg;
} email_info_message_t;
typedef enum
{
   APP_IMAGE_LOG,
   APP_OTHER
} APP_TYPE;


void SendMsgToSystem(mqd_t msgq, MESSAGE *msg);
int PosixMessageSend(mqd_t msgq, MESSAGE *msg);

/**
 * Execute the system command
 * @name runCommand
 * @function
 * @param const char *command- command string
 * @return int -> 0 - success, fail - 1
 */
int runCommand(const char *command);

/**
 * Check whether the specified string is direcory or not - Internal function
 * @name isDirectory
 * @function
 * @param const char *dirPath - absolute path of directory (IN parameter)
 * @return int -> 1 - dirPath is directory, 0 - dirPath is not a directory
 */
int isDirectory(const char *dirPath);

/**
 * Get the file size in bytes - Internal function
 * @function
 * @name getSize
 * @function
 * @param const char *filePath - location of file (IN parameter)
 * @return off_t - file size in bytes
 */
off_t getSize(const char *filePath);

/**
 * Check whether the specified string is direcory or not - Internal function
 * @name file_exist
 * @function
 * @param const char *filename - name of the file with compleye path (IN parameter)
 * @return int -> 1 - file exists, 0 - file does not exist
 */
int file_exist(const char *filename);

bool checkBlankorNull(const char *str);
error_type_t sendMSGToSysManager(stSend_SendResult *jobResult, UINT16 command, UINT32 sender);
UINT32 getError(int  errorCode);
unsigned long getBase64Size(unsigned long value);
bool checkOK(unsigned long value);
bool checkBase64OK(unsigned long value);

int encodeFPBase64(FILE *inFile, FILE *outFile);

int encodeCharBase64(const char *inStr, FILE *outFile,bool isABNF);

void removeFile(char *filename);
bool isDirEmpty(const char *dirLocation);
bool shiftEndfile(char **fileList, int filecount);
#if defined(HAVE_POP) || defined(HAVE_IMAP)

#define ATTACHMENT_START1      "Content-Disposition: attachment;"
#define ATTACHMENT_START2      "filename="
#define ATTACHMENT_END         "--"
#define VALID_BASE64           "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
struct receiveBuf
{
   char   *memory;
   size_t size;
};
int createFolder(const char *folderPath);
int base64_decode_tofile(char *start, char *end, FILE *attfile);
int extractAttachment(char *emailFile);
size_t getDataCB(void *contents, size_t size, size_t nmemb, void *userp);
size_t getFileCB(void *contents, size_t size, size_t nmemb, void *userp);
#endif
#endif /* __COMMON_UTILS_H__ */
