#ifndef SOLUTION_H_
#define SOLUTION_H_
#include "commonutils.h"
#include "sqlite3.h"

#define SEND_TO_EMAIL     1
#define SEND_TO_FTP       2
#define SEND_TO_SMB       3
#define SEND_TO_WEBDAV    4


#define ERROR_SOL_INIT    -1

/*
Enum - REQUEST_TYPE
TYPESYSMGR - Normal Upload, TYPECONNTEST_UI - Connection Test from UI, 
TYPECONNTEST_WEB - Connection Test from WebServer
*/

typedef enum
{
   TYPESYSMGR,
   TYPECONNTEST_UI,
   TYPECONNTEST_WEB,
   TYPENONE = 4
} REQUEST_TYPE;


typedef enum
{
   PROTO_FTP,
   PROTO_SFTP,
   PROTO_FTPS,
   PROTO_WEBDAV_HTTP = 10,
   PROTO_WEBDAV_HTTPS
} PROTO_TYPE;

typedef struct
{
   char               *username;
   char               *password;
   char               *destination;
   int                port;
   char               *filePath;
   int                serverType;
   char               *proxyserver;
   int                isDebugMode;
   stSolution_Request *ftp_msg;
   REQUEST_TYPE       reqType;
   int                timeout;
} ftp_info_t;

typedef struct
{
   char               *username;
   char               *password;
   char               *sender;
   char               *mailhost;
   int                port;
   char               *attachmentPath;
   char               *toAddress;
   char               *subject;
   char               *content;
   int                secureType;
   int                authType;
   int                isDebugMode;
   stSolution_Request *email_msg;
} email_message_t;


typedef struct
{
   char               *username;
   char               *password;
   char               *destination;
   int                port;
   char               *filePath;
   int                serverType;
   char               *proxyserver;
   int                isDebugMode;
   stSolution_Request *webdav_msg;
   REQUEST_TYPE       reqType;
   int                timeout;
} webdav_info_t;


typedef struct
{
   char               *username;
   char               *password;
   char               *smbshare;
   int                port;
   char               *remotePath;
   char               *filePath;
   int                isDebugMode;
   stSolution_Request *smb_msg;
   REQUEST_TYPE       reqType;
} smb_info_t;

/**
 * Thread to execute email function
 * @name Email_Thread
 * @function
 * @param void *email_message_void - pointer to email message data
 * @return void * - result of thread execution
 */
void *Email_Thread(void *email_message_void);
int sendMail(email_info_message_t *email_message);
/**
 * Wrapper function to send email
 *
 * @name sendMailWrapper
 * @function
 * @param   const char *username  - email server login username
 * @param   const char *password  - email server login password
 * @param   const char *sender - name of email sender
 * @param   const char *mailhost - SMTP server address
 * @param   int port - SMTP server listen port, usually 25 or 587 or 465.
 * @param   const char *attachmentPath - path to local file/directory that will be attached to email, NULL in case of no attachment
 * @param   const char *toAddress - semicolon (;) separated list of destination email addresses
 * @param   const char *subject - subject of email message
 * @param   const char *content - content of email message
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @param   stSolution_Request *email_msg - pointer to struct containing info received from App
 * @return  int - 0 on success
 */
int sendMailWrapper(const char *username, const char *password, const char *sender,
                    const char *mailhost, int port, const char *attachmentPath, const char *toAddress,
                    const char *subject, const char *content, int secureType, int authType, int isDebugMode, stSolution_Request *email_msg);

/**
 * Thread to execute ftp function
 * @name FTP_Thread
 * @function
 * @param void *ftp_info_void - pointer to ftp info
 * @return void * - result of thread execution
 */
void *FTP_Thread(void *ftp_info_void);

/**
 * Wrapper function to upload a file on remote ftp server
 *
 * @name ftpUploadWrapper
 * @function
 * @param   const char *username - FTP server username, "anonymous" in case of anonymous logon
 * @param   const char *password - FTP server password, "anonymous" in case of anonymous logon
 * @param   const char *destination - ftp server location to upload the file
 * @param   int port - ftp server listen port usually 21
 * @param   const char *filePath - path to local file that will be uploaded to server
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @param   stSolution_Request *ftp_msg - pointer to struct containing info received from App
 * @param   REQUEST_TYPE reqType - type of request. (included for connection test functionality)
 * @return  int - 0 on success
 */
int ftpUploadWrapper(const char *username, const char *password, const char *destination, int port, const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request *ftp_msg, REQUEST_TYPE reqType);



/**
 * Thread to execute webdav function
 * @name WebDAV_Thread
 * @function
 * @param void *webdav_info_void - pointer to webdav info
 * @return void * - result of thread execution
 */
void *WebDAV_Thread(void *webdav_info_void);

/**
 * Wrapper function to upload a file on remote http/webdav server
 *
 * @name webDAVUpload
 * @function
 * @param   const char *username - WEBDAV server username
 * @param   const char *password - WEBDAV server password
 * @param   const char *destination - http server location to upload the file
 * @param   int port - http server listen port usually 80
 * @param   const char *filePath - path to local file that will be uploaded to server
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @param   stSolution_Request *webdav_msg - pointer to struct containing info received from App
 * @param   REQUEST_TYPE reqType - type of request. (included for connection test functionality)
 * @return  int - 0 on success
 */
int webDAVUploadWrapper(const char *username, const char *password, const char *destination, int port, const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request *webdav_msg, REQUEST_TYPE reqType);



/**
 * Wrapper function to upload a file on remote smb server
 *
 * @name smbUploadWrapper
 * @function
 * @param   const char *username - SMB server username, NULL in case of anonymous logon
 * @param   const char *password - SMB server password, NULL in case of anonymous logon
 * @param   const char *smbshare - smb server location to upload the file
 * @param   int port - smb server listen port usually 445, keep it 0 to select default port
 * @param   const char *remotePath - remote SMB server path where file/folder will be uploaded to server
 * @param   const char *localPath - path to local file that will be uploaded to server
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @param   stSolution_Request *smb_msg - pointer to struct containing info received from App
 * @param   REQUEST_TYPE reqType - type of request. (included for connection test functionality)
 * @return  int - 0 on success
 */
int smbUploadWrapper(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, const char *filePath, int isDebug, stSolution_Request *smb_msg, REQUEST_TYPE reqType);

/**
 * Thread to execute smb function
 * @name SMB_Thread
 * @function
 * @param void *smb_info_void - pointer to smb info
 * @return void * - result of thread execution
 */
void *SMB_Thread(void *smb_info_void);

void deleteEmailInfo(email_info_message_t *email_message);
void deleteFTPInfo(ftp_info_t *ftp_info);
void deleteSMBInfo(smb_info_t *smb_info);
void deleteWebDAVInfo(webdav_info_t *wevdav_info);
#ifdef HAVE_IMAGELOG
void PopulateImageLogVars();
int Image_log_Upload(char *filePath);
#endif

#endif
