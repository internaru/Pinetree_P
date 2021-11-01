#include "commonutils.h"
#include "ftpupload.h"
#include "webdavupload.h"
#include "smbupload.h"
#include "send-email.h"
#include "solution.h"

#include "SysSharedMemory.h"
#include "lassert.h"
#define SDH_SEND_DESTINATION_MAX 257
#define SHM_KEY_SCAN_SEND_MAX 60
#define SHM_KEY_SCAN_SEND 50
#define safefree(x)                   \
   do { if (x) { free(x); x = NULL; } \
   }                                  \
   while (0)
typedef enum
{
	JOBTYPE_EMAIL=1,
	JOBTYPE_FTP=2,
	JOBTYPE_SMB=3,
	JOBTYPE_WEBDAV=4
}JOB_TYPE;
#ifdef HAVE_IMAGELOG
#define MAX_STR_SIZE 1024
extern sqlite3* dbInstance;
int g_nProtocolType;
char g_PROTO_ID[MAX_STR_SIZE];
char g_PROTO_PW[MAX_STR_SIZE];
char g_PROTO_Address[MAX_STR_SIZE];
char g_PROTO_Path[MAX_STR_SIZE];
int g_PROTO_Port;
#endif

int notifyJobStatusToSSM(stIC_ScanSendResult *networkJobResult);

int notifyJobStatusToSSM( stIC_ScanSendResult *networkJobResult)
{
   // printf("\n Hello ~notifyJobStatusToSSM %d \n",__LINE__); // Commented by mspyo, 2017-10-25
   networkJobResult->addInfo = 0;
   SYS_UI_MESSAGE sndToSysMsg;
   sndToSysMsg.cmd = CMD_SU_SCAN_GOSCANSEND_RES;   //0x0683
   memcpy(sndToSysMsg.data, (UINT8 *)networkJobResult, sizeof(stIC_ScanSendResult));
   error_type_t err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &sndToSysMsg);
   XASSERT(err == OK, err);
   return(0);
}



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
int ftpUploadWrapper(const char *username, const char *password, const char *destination, int port, const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request *ftp_msg, REQUEST_TYPE reqType)
{
   pthread_attr_t threadAttributes;

   int result = 0;
   int strSize = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }

   pthread_t  ftp_thread;
   ftp_info_t *ftp_info = (ftp_info_t *)malloc(sizeof(ftp_info_t));
   if (ftp_info == NULL)
   {
      pthread_attr_destroy(&threadAttributes);
      return ERROR_SOL_INIT;
   }
   if (username)
   {
      strSize = strlen(username) + 1;
      ftp_info->username = (char *)malloc(strSize);
	  if(ftp_info->username)
         memcpy(ftp_info->username, username,strSize);
   }
   else
   {
      ftp_info->username = NULL;
   }

   if (password)
   {
      strSize = strlen(password) + 1;
      ftp_info->password = (char *)malloc(strSize);
	  if(ftp_info->password)
         memcpy(ftp_info->password, password,strSize);
   }
   else
   {
      ftp_info->password = NULL;
   }

   if (destination)
   {
      strSize = strlen(destination) + 1;
      ftp_info->destination = (char *)malloc(strSize);
	  if(ftp_info->destination)
         memcpy(ftp_info->destination, destination, strSize);
   }
   else
   {
      ftp_info->destination = NULL;
   }


   if (filePath)
   {
      strSize = strlen(filePath) + 1;
      ftp_info->filePath = (char *)malloc(strSize);
	  if(ftp_info->filePath)
         memcpy(ftp_info->filePath,filePath,strSize);
   }
   else
   {
      ftp_info->filePath = NULL;
   }


   ftp_info->port = port;

   ftp_info->serverType = serverType;
   ftp_info->timeout= FTP_TIMEOUT;
   if (proxyserver)
   {
      strSize = strlen(proxyserver) + 1;
      ftp_info->proxyserver = (char *)malloc(strSize);
	  if(ftp_info->proxyserver)
         memcpy(ftp_info->proxyserver, proxyserver, strSize);
   }
   else
   {
      ftp_info->proxyserver = NULL;
   }
   ftp_info->isDebugMode = isDebugMode;
   ftp_info->reqType     = reqType;
   if (ftp_msg)
   {
      ftp_info->ftp_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
	  if(ftp_info->ftp_msg)
         memcpy(ftp_info->ftp_msg, ftp_msg, sizeof(stSolution_Request));
   }
   else
   {
      ftp_info->ftp_msg = NULL;
   }
   result = pthread_create(&ftp_thread, &threadAttributes, FTP_Thread, (void *)ftp_info);
   pthread_attr_destroy(&threadAttributes);
   return result;
}


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
 * @param   stSolution_Request *smb_msg - pointer to struct containing info received from App
 * @param   REQUEST_TYPE reqType - type of request. (included for connection test functionality)
 * @return  int - 0 on success
 */
int webDAVUploadWrapper(const char *username, const char *password, const char *destination, int port, const char *filePath, int serverType, const char *proxyserver, int isDebugMode, stSolution_Request *webdav_msg, REQUEST_TYPE reqType)
{
   pthread_attr_t threadAttributes;

   int result = 0;
   int strSize = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }
   pthread_t     webdav_thread;
   webdav_info_t *webdav_info = (webdav_info_t *)malloc(sizeof(webdav_info_t));
   if (webdav_info == NULL)
   {
      pthread_attr_destroy(&threadAttributes);
      return ERROR_SOL_INIT;
   }

   if (username)
   {
      strSize = strlen(username) + 1;
      webdav_info->username = (char *)malloc(strSize);
	  if(webdav_info->username)
         memcpy(webdav_info->username, username, strSize);
   }
   else
   {
      webdav_info->username = NULL;
   }

   if (password)
   {
      strSize = strlen(password) + 1;
      webdav_info->password = (char *)malloc(strSize);
	  if(webdav_info->password)
         memcpy(webdav_info->password, password, strSize);
   }
   else
   {
      webdav_info->password = NULL;
   }

   if (destination)
   {
      strSize = strlen(destination) + 1;
      webdav_info->destination = (char *)malloc(strSize);
	  if(webdav_info->destination)
         memcpy(webdav_info->destination, destination, strSize);
   }
   else
   {
      webdav_info->destination = NULL;
   }


   if (filePath)
   {
      strSize = strlen(filePath) + 1;
      webdav_info->filePath = (char *)malloc(strSize);
	  if(webdav_info->filePath)
         memcpy(webdav_info->filePath, filePath, strSize);
   }
   else
   {
      webdav_info->filePath = NULL;
   }


   webdav_info->port = port;

   webdav_info->serverType = serverType;
   webdav_info->timeout = WEBDAV_TIMEOUT;

   if (proxyserver)
   {
      strSize = strlen(proxyserver) + 1;
      webdav_info->proxyserver = (char *)malloc(strSize);
	  if(webdav_info->proxyserver)
         memcpy(webdav_info->proxyserver, proxyserver, strSize);
   }
   else
   {
      webdav_info->proxyserver = NULL;
   }
   webdav_info->isDebugMode = isDebugMode;
   webdav_info->reqType     = reqType;
   if (webdav_msg)
   {
      webdav_info->webdav_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
	  if(webdav_info->webdav_msg)
         memcpy(webdav_info->webdav_msg, webdav_msg, sizeof(stSolution_Request));
   }
   else
   {
      webdav_info->webdav_msg = NULL;
   }
   result = pthread_create(&webdav_thread, &threadAttributes, WebDAV_Thread, (void *)webdav_info);
   pthread_attr_destroy(&threadAttributes);
   return result;
}


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
 * @return  int - 0 on success, 1 on failure
 */
int smbUploadWrapper(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, const char *filePath, int isDebugMode, stSolution_Request *smb_msg, REQUEST_TYPE reqType)
{
   pthread_attr_t threadAttributes;
   int            result = 0;
   int            strSize = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }

   pthread_t  smb_thread;
   smb_info_t *smb_info = (smb_info_t *)malloc(sizeof(smb_info_t));
   if (smb_info == NULL)
   {
      pthread_attr_destroy(&threadAttributes);
      return ERROR_SOL_INIT;
   }
   if (username)
   {
      strSize = strlen(username) + 1;  
      smb_info->username = (char *)malloc(strSize);
	  if(smb_info->username)
         memcpy(smb_info->username, username, strSize);
   }
   else
   {
      smb_info->username = NULL;
   }

   if (password)
   {
      strSize = strlen(password) + 1; 
      smb_info->password = (char *)malloc(strSize);
	  if(smb_info->password)
         memcpy(smb_info->password, password, strSize);
   }
   else
   {
      smb_info->password = NULL;
   }

   if (smbshare)
   {
      strSize = strlen(smbshare) + 1;
      smb_info->smbshare = (char *)malloc(strSize);
	  if(smb_info->smbshare)
         memcpy(smb_info->smbshare, smbshare, strSize);
   }
   else
   {
      smb_info->smbshare = NULL;
   }

   if (filePath)
   {
      strSize = strlen(filePath) + 1;
	  smb_info->filePath = (char *)malloc(strSize);
	  if(smb_info->filePath)
         memcpy(smb_info->filePath, filePath, strSize);
   }
   else
   {
      smb_info->filePath = NULL;
   }

   smb_info->port = port;

   if (remotePath)
   {
      strSize = strlen(remotePath) + 1;
      smb_info->remotePath = (char *)malloc(strSize);
	  if(smb_info->remotePath)
         memcpy(smb_info->remotePath, remotePath, strSize);
   }
   else
   {
      smb_info->remotePath = NULL;
   }
   smb_info->isDebugMode = isDebugMode;
   smb_info->reqType     = reqType;
   if (smb_msg)
   {
      smb_info->smb_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
      if(smb_info->smb_msg)
         memcpy(smb_info->smb_msg, smb_msg, sizeof(stSolution_Request));
   }
   else
   {
      smb_info->smb_msg = NULL;
   }
   result = pthread_create(&smb_thread, &threadAttributes, SMB_Thread, (void *)smb_info);
   pthread_attr_destroy(&threadAttributes);
   return result;
}


/**
 * Thread to execute smb function
 * @name SMB_Thread
 * @function
 * @param void *smb_info_void - pointer to smb info
 * @return void * - result of thread execution
 */
void *SMB_Thread(void *smb_info_void)
{
   smb_info_t *smb_info = (smb_info_t *)smb_info_void;
   UINT32     result;

   if (smb_info == NULL)
   {
      printf("\nsmb_info is NULL");
      return NULL;
   }

   if (smb_info->reqType == TYPESYSMGR)
   {
      printf("\nSMB Upload");
	  stIC_ScanSendResult networkJobResult;

	  memset(&networkJobResult, 0, sizeof(stIC_ScanSendResult));

	  networkJobResult.jobID     = smb_info->smb_msg->sendJobId;;
	  networkJobResult.scanJobID =  smb_info->smb_msg->systemJobID;
	  networkJobResult.jobType = JOBTYPE_SMB;// 1-mail, 2-ftp, 3-smb, 4-webdav
	  networkJobResult.errorCode = 0;
	  networkJobResult.result    = 2;
	  networkJobResult.addInfo   = 0;
	  networkJobResult.destCount = 1;
	  notifyJobStatusToSSM( &networkJobResult);
      result = smbUpload(smb_info->username, smb_info->password, smb_info->smbshare, smb_info->port, smb_info->remotePath, smb_info->filePath, smb_info->isDebugMode);
      networkJobResult.errorCode =result;
      networkJobResult.result = networkJobResult.errorCode ? 6 : 5;
      notifyJobStatusToSSM( &networkJobResult);
   }
   else
   {
      printf("\nSMB Connection Test");
      result = smbConnTest(smb_info->username, smb_info->password, smb_info->smbshare, smb_info->port, smb_info->remotePath, smb_info->isDebugMode);
   }


   if (smb_info->smb_msg == NULL)
   {
      deleteSMBInfo(smb_info);
      smb_info = NULL;
      return NULL;
   }
   if (smb_info->reqType == TYPESYSMGR)
   {
      MESSAGE *smb_msg = (MESSAGE *)malloc(sizeof(MESSAGE));
	  if(smb_msg)
  	  {
         smb_msg->msgType = MSG_SENDJOB_RESULT;
         smb_msg->param1  = smb_info->smb_msg->sendJobId;
         smb_msg->param2  = result;
         SYMsgSend((MODULE_ID)smb_info->smb_msg->sendRequestAppMsgQueueId, smb_msg);
         free(smb_msg);
         smb_msg = NULL;
      }
   }

   stSend_SendResult *jobResult = (stSend_SendResult *)malloc(sizeof(stSend_SendResult));
   if (jobResult)
   {
      jobResult->result      = result;
      jobResult->reqAppType  = smb_info->smb_msg->sendRequestAppMsgQueueId;
      jobResult->sendJobType = SEND_TO_SMB;
      sendMSGToSysManager(jobResult, CMD_INT_TX_SEND_RESULT, smb_info->smb_msg->sendRequestAppMsgQueueId);
      free(jobResult);
      jobResult = NULL;
   }

   deleteSMBInfo(smb_info);
   smb_info = NULL;

   return NULL;
}


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
                    const char *subject, const char *content, int secureType, int authType, int isDebugMode, stSolution_Request *email_msg)
{
   int            result = 0;
   pthread_attr_t threadAttributes;
   int strSize    = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }

   pthread_t       email_thread;
   email_message_t *email_message = (email_message_t *)malloc(sizeof(email_message_t));
   if (email_message == NULL)
   {
      pthread_attr_destroy(&threadAttributes);
      return ERROR_SOL_INIT;
   }
   if (username)
   {
      strSize = strlen(username) + 1;
      email_message->username = (char *)malloc(strSize);
	  if(email_message->username)
         memcpy(email_message->username, username, strSize);
   }
   else
   {
      email_message->username = NULL;
   }

   if (password)
   {
      strSize = strlen(password) + 1;
      email_message->password = (char *)malloc(strSize);
	  if(email_message->password)
         memcpy(email_message->password, password, strSize);
   }
   else
   {
      email_message->password = NULL;
   }

   if (sender)
   {
      strSize = strlen(sender) + 1;
      email_message->sender = (char *)malloc(strSize);
	  if(email_message->sender)
         memcpy(email_message->sender, sender, strSize);
   }
   else
   {
      email_message->sender = NULL;
   }

   if (mailhost)
   {
      strSize = strlen(mailhost) + 1;
      email_message->mailhost = (char *)malloc(strSize);
	  if(email_message->mailhost)
         memcpy(email_message->mailhost, mailhost, strSize);
   }
   else
   {
      email_message->mailhost = NULL;
   }


   email_message->port = port;

   if (attachmentPath)
   {
      strSize = strlen(attachmentPath) + 1;
      email_message->attachmentPath = (char *)malloc(strSize);
	  if(email_message->attachmentPath)
         memcpy(email_message->attachmentPath, attachmentPath, strSize);
   }
   else
   {
      email_message->attachmentPath = NULL;
   }

   if (toAddress)
   {
      strSize = strlen(toAddress) + 1;
      email_message->toAddress = (char *)malloc(strSize);
	  if(email_message->toAddress)
         memcpy(email_message->toAddress, toAddress, strSize);
   }
   else
   {
      email_message->toAddress = NULL;
   }

   if (subject)
   {
      strSize = strlen(subject) + 1;
      email_message->subject = (char *)malloc(strSize);
	  if(email_message->subject)
         memcpy(email_message->subject, subject, strSize);
   }
   else
   {
      email_message->subject = NULL;
   }

   if (content)
   {
      strSize = strlen(content) + 1;
      email_message->content = (char *)malloc(strSize);
	  if(email_message->content)
         memcpy(email_message->content, content, strSize);
   }
   else
   {
      email_message->content = NULL;
   }

   email_message->secureType  = secureType;
   email_message->authType    = authType;
   email_message->isDebugMode = isDebugMode;

   if (email_msg)
   {
      email_message->email_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
	  if(email_message->email_msg)
         memcpy(email_message->email_msg, email_msg, sizeof(stSolution_Request));
   }
   else
   {
      email_message->email_msg = NULL;
   }
   result = pthread_create(&email_thread, &threadAttributes, Email_Thread, (void *)email_message);
   pthread_attr_destroy(&threadAttributes);
   return result;
}


//   New Email API..sendMailWrapper deprecated
//   For Scan/Fax to email or email alert -> email_type should be of type EMAIL_OTHER,email_report and ifax_request should be set to NULL
int sendMail(email_info_message_t *email_info_message)
{
   int            result = 0;
   pthread_attr_t threadAttributes;
   if (email_info_message == NULL)
   {
      return(result);
   }
   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return(result);
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return(result);
   }
   pthread_t            email_thread;
   email_info_message_t *email_message = (email_info_message_t *)malloc(sizeof(email_info_message_t));
   memset(email_message, 0, sizeof(email_info_message_t));
   if (email_info_message->username)
   {
      email_message->username = (char *)malloc(strlen(email_info_message->username) + 1);
      strcpy(email_message->username, email_info_message->username);
   }
   else
   {
      email_message->username = NULL;
   }
   if (email_info_message->password)
   {
      email_message->password = (char *)malloc(strlen(email_info_message->password) + 1);
      strcpy(email_message->password, email_info_message->password);
   }
   else
   {
      email_message->password = NULL;
   }
   if (email_info_message->senderEmail)
   {
      email_message->senderEmail = (char *)malloc(strlen(email_info_message->senderEmail) + 1);
      strcpy(email_message->senderEmail, email_info_message->senderEmail);
   }
   else
   {
      email_message->senderEmail = NULL;
   }
   if (email_info_message->senderName)
   {
      email_message->senderName = (char *)malloc(strlen(email_info_message->senderName) + 1);
      strcpy(email_message->senderName, email_info_message->senderName);
   }
   else
   {
      email_message->senderName = NULL;
   }
   if (email_info_message->mailhost)
   {
      email_message->mailhost = (char *)malloc(strlen(email_info_message->mailhost) + 1);
      strcpy(email_message->mailhost, email_info_message->mailhost);
   }
   else
   {
      email_message->mailhost = NULL;
   }
   email_message->port = email_info_message->port;
   if (email_info_message->attachmentPath)
   {
      email_message->attachmentPath = (char *)malloc(strlen(email_info_message->attachmentPath) + 1);
      strcpy(email_message->attachmentPath, email_info_message->attachmentPath);
   }
   else
   {
      email_message->attachmentPath = NULL;
   }
   if (email_info_message->toAddress)
   {
      email_message->toAddress = (char *)malloc(strlen(email_info_message->toAddress) + 1);
      strcpy(email_message->toAddress, email_info_message->toAddress);
   }
   else
   {
      email_message->toAddress = NULL;
   }
   if (email_info_message->subject)
   {
      email_message->subject = (char *)malloc(strlen(email_info_message->subject) + 1);
      strcpy(email_message->subject, email_info_message->subject);
   }
   else
   {
      email_message->subject = NULL;
   }
   if (email_info_message->content)
   {
      email_message->content = (char *)malloc(strlen(email_info_message->content) + 1);
      strcpy(email_message->content, email_info_message->content);
   }
   else
   {
      email_message->content = NULL;
   }
   email_message->secureType       = email_info_message->secureType;
   email_message->authType         = email_info_message->authType;
   email_message->isDebugMode      = email_info_message->isDebugMode;
   if (email_info_message->email_msg)
   {
      email_message->email_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
      memcpy(email_message->email_msg, email_info_message->email_msg, sizeof(stSolution_Request));
   }
   else
   {
      email_message->email_msg = NULL;
   }
   result = pthread_create(&email_thread, &threadAttributes,Email_Thread, (void *)email_message);

   //Delete Should be called by caller
   //deleteEmailInfoMessage(email_info_message);
   //email_info_message = NULL;
   pthread_attr_destroy(&threadAttributes);
   return(result);
}

#ifdef HAVE_IFAX
//TODO:
// Add Receiver Name
//iFax send API
int sendIFaxRequest(ifax_request_t *ifax_request)
{
   if (!ifax_request)
   {
      return(IFAX_FAIL);
   }
   email_info_message_t *email_message = getIfaxConfInfo(ifax_request);
   deleteIfaxRequest(ifax_request);
   if (email_message)
   {
      email_message = getIfaxServerInfo(email_message);
      // Where to call this?
      // txt to pdf conversion?
      // Skip Printing Sent Report.
      //printSentEmailReport(email_message);
      int            result = 0;
      pthread_attr_t threadAttributes;
      // Skip calling sendMail to avoid redundant copied of email request struct
      if (email_message == NULL)
      {
         return(IFAX_FAIL);
      }
      if ((result = pthread_attr_init(&threadAttributes)))
      {
         return(result);
      }
      result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
      if (result)
      {
         pthread_attr_destroy(&threadAttributes);
         return(result);
      }
      pthread_t email_thread;
      result = pthread_create(&email_thread, &threadAttributes, SendEmail_Thread, (void *)email_message);
      pthread_attr_destroy(&threadAttributes);
      return(result);
   }
   return(IFAX_FAIL);
}


IFAX_RESULT sendMDNResponse(email_report_t *email_report, char *senderAddress, char *toAddress)
{
   if (!email_report || !email_report->id || !toAddress || !senderAddress)
   {
      return(IFAX_FAIL);
   }
   int            result        = IFAX_FAIL;
   ifax_request_t *mdn_response = (ifax_request_t *)malloc(sizeof(ifax_request_t));
   if (mdn_response)
   {
      memset(mdn_response, 0, sizeof(ifax_request_t));
      mdn_response->attachmentPath = NULL;
      // strcpy(mdn_response->attachmentPath, argv[2]);
      mdn_response->toAddress = (char *)malloc(strlen(toAddress) + 1);
      if (mdn_response->toAddress)
      {
         strcpy(mdn_response->toAddress, toAddress);
      }
      mdn_response->subject = (char *)malloc(MAX_IFAX_STR + 1);
      if (mdn_response->subject)
      {
         mdn_response->content = (char *)malloc(MAX_REPORT_CONTENT);
      }
      if (mdn_response->content)
      {
/*	  #define  EMAIL_MULTIPART_NOTIFICATION_UA_STR  "Reporting UA: ifax-sindoh-mfp;"
 * #define  EMAIL_MULTIPART_ORIGINAL_RECIPIENT "Original-Recipient:"
 * #define  EMAIL_MULTIPART_MESSAGE_ID "Original-Recipient: Original-Message-ID:"
 * #define  EMAIL_MULTIPART_FINAL_RECIPIENT "Final-Recipient:"
 * #define  EMAIL_MULTIPART_RFC_822_STR "rfc822;"
 * #define  EMAIl_MULTIPART_MDN_ACTION_AUTO "automatic-action/MDN-sent-automatically"
 #define EMAIL_BOUNDRY                       "----=_Part_1_1111111111"*/
         char inline_report[MAX_IFAX_STR] = { 0 };
         snprintf(inline_report, sizeof(inline_report),                                                                                          \
                  "\n\n--%s\n%s %s\n%s %s\n\n%s\n%s %s;%s\n%s %s;%s\n%s <%s>\n%s %s", EMAIL_BOUNDRY, EMAIL_HEADER_CONTENT_TYPE, \
                  EMAIL_MULTIPART_CONTENT_TYPE_MDN,                                                                                              \
                  EMAIL_MULTIPART_CONTENT_TRANSFER_ENCODING_STR, EMAIL_MULTIPART_CONTENT_TRANSFER_ENCODING_7BIT,                                 \
                  EMAIL_MULTIPART_NOTIFICATION_UA_STR, EMAIL_MULTIPART_ORIGINAL_RECIPIENT,                                                       \
                  EMAIL_MULTIPART_RFC_822_STR, senderAddress, EMAIL_MULTIPART_FINAL_RECIPIENT,                                                   \
                  EMAIL_MULTIPART_RFC_822_STR, senderAddress, EMAIL_MULTIPART_ORIGINAL_MESSAGE_ID_MDN,                                           \
                  email_report->id, EMAIL_MULTIPART_DISPOSITION, EMAIL_MULTIPART_MDN_ACTION_AUTO);
         if (email_report->mdn_dsn_result == MDN_FAIL)
         {
            snprintf(mdn_response->subject, MAX_IFAX_STR, "%s", MDN_FAILED_SUBJECT_STR);
            snprintf(mdn_response->content, MAX_IFAX_STR, MDN_FAILED_CONTENT_STR, senderAddress);
            strncat(mdn_response->content, inline_report, MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
            strncat(mdn_response->content, ";\n processed/error", MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
         }
         else if (email_report->mdn_dsn_result == MDN_PROCESSED)
         {
            snprintf(mdn_response->subject, MAX_IFAX_STR, "%s", MDN_PROCESSED_SUBJECT_STR);
            snprintf(mdn_response->content, MAX_IFAX_STR, MDN_PROCESSED_CONTENT_STR, senderAddress);
            strncat(mdn_response->content, inline_report, MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
            strncat(mdn_response->content, ";\n processed", MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
         }
         else if (email_report->mdn_dsn_result == MDN_DISPATCHED)
         {
            snprintf(mdn_response->subject, MAX_IFAX_STR, "%s", MDN_PROCESSED_SUBJECT_STR);
            snprintf(mdn_response->content, MAX_IFAX_STR, MDN_PROCESSED_CONTENT_STR, senderAddress);
            strncat(mdn_response->content, inline_report, MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
            strncat(mdn_response->content, ";\n dispatched", MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
         }
         else if (email_report->mdn_dsn_result == MDN_DISPLAYED)
         {
            snprintf(mdn_response->subject, MAX_IFAX_STR, "%s", MDN_PROCESSED_SUBJECT_STR);
            snprintf(mdn_response->content, MAX_IFAX_STR, MDN_PROCESSED_CONTENT_STR, senderAddress);
            strncat(mdn_response->content, inline_report, MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
            strncat(mdn_response->content, ";\n displayed", MAX_REPORT_CONTENT - strlen(mdn_response->content) - 1);
         }
         printf("\nSend Response Subject[%s]\n\ncontent[%s]", mdn_response->subject, mdn_response->content);
         email_info_message_t *email_message = getSendIfaxResponseConfInfo(mdn_response);
         if (email_message && (email_message->email_type == EMAIL_IFAX_RESPONSE_MDN))
         {
            email_message->email_report = email_report;
            email_message = getIfaxServerInfo(email_message);
            deleteIfaxRequest(mdn_response);
            result = sendMail(email_message);           //delete email_message in thread
            return(result);
         }
      }
      deleteIfaxRequest(mdn_response);
   }
   return(result);
}
#endif

/**
 * Thread to execute email function
 * @name Email_Thread
 * @function
 * @param void *email_message_void - pointer to email message data
 * @return void * - result of thread execution
 */
void *Email_Thread(void *email_message_void)
{
	email_info_message_t *email_message = (email_info_message_t *)email_message_void;

   if (email_message == NULL)
   {
      printf("\nemail_message  is NULL");
      return NULL;
   }
   UINT32 result = 0;

   if(email_message->email_msg && (email_message->email_msg->sendRequestAppMsgQueueId == SCANTOEMAILAPPID || email_message->email_msg->sendRequestAppMsgQueueId == FAXAPPSOLUTIONID))
   {
      if ((email_message->attachmentPath == NULL) || (strcmp(email_message->attachmentPath, "") == 0) || !file_exist(email_message->attachmentPath) || (isDirectory(email_message->attachmentPath) && isDirEmpty(email_message->attachmentPath)))
      {
      	 printf("\nAttachment [%s] Empty or does not exist..Aborting email",email_message->attachmentPath?email_message->attachmentPath:"NULL");
         char cmd[1024];
         snprintf(cmd,sizeof(cmd),"ls %s",email_message->attachmentPath?email_message->attachmentPath:"/tmp/*");
         printf("\nListing [%s] directory\n",cmd);
         system(cmd);
         result = ERROR_CONFIG;
      }
   }
   if(result == 0)
   {
	stIC_ScanSendResult networkJobResult;
	memset(&networkJobResult, 0, sizeof(stIC_ScanSendResult));

	if (email_message->email_msg) { // Added by mspyo, 2017-10-25
		networkJobResult.jobID     = email_message->email_msg->sendJobId;;
		networkJobResult.scanJobID =  email_message->email_msg->systemJobID;
		networkJobResult.jobType = JOBTYPE_EMAIL;
		networkJobResult.errorCode = 0;
		networkJobResult.result    = 2;
		networkJobResult.addInfo   = 0;
		networkJobResult.destCount = 1;
		notifyJobStatusToSSM( &networkJobResult);
	}

	result = sendEmail(email_message);

	if (email_message->email_msg) { // Added by mspyo, 2017-10-25
		networkJobResult.errorCode = result;
		networkJobResult.result = networkJobResult.errorCode ? 6 : 5;
		notifyJobStatusToSSM( &networkJobResult);
	}
   }

   if (email_message->email_msg == NULL)
   {
      deleteEmailInfo(email_message);
      email_message = NULL;
      return NULL;
   }
   MESSAGE *email_msg = (MESSAGE *)malloc(sizeof(MESSAGE));
   if(email_msg)
   {
      email_msg->msgType = MSG_SENDJOB_RESULT;
      email_msg->param1  = email_message->email_msg->sendJobId;
      email_msg->param2  = result;
      SYMsgSend((MODULE_ID)email_message->email_msg->sendRequestAppMsgQueueId, email_msg);
   }

   stSend_SendResult *jobResult = (stSend_SendResult *)malloc(sizeof(stSend_SendResult));
   if(jobResult)
   {
      jobResult->result      = result;
      jobResult->reqAppType  = email_message->email_msg->sendRequestAppMsgQueueId;
      jobResult->sendJobType = SEND_TO_EMAIL;
      sendMSGToSysManager(jobResult, CMD_INT_TX_SEND_RESULT, email_message->email_msg->sendRequestAppMsgQueueId);
      free(jobResult);
      jobResult = NULL;
   }

   deleteEmailInfo(email_message);
   email_message = NULL;

   if (email_msg)
   {
      free(email_msg);
      email_msg = NULL;
   }
   return NULL;
}


/**
 * Thread to execute ftp function
 * @name FTP_Thread
 * @function
 * @param void *ftp_info_void - pointer to ftp info
 * @return void * - result of thread execution
 */
void *FTP_Thread(void *ftp_info_void)
{
   ftp_info_t *ftp_info = (ftp_info_t *)ftp_info_void;
   UINT32     result;
   APP_TYPE apptype = APP_OTHER;
   if (ftp_info == NULL)
   {
      printf("\nftp_info is NULL");
      return NULL;
   }
   if (ftp_info->reqType == TYPESYSMGR)
   {
      printf("\nFTP Upload");
	  if(ftp_info->ftp_msg && ftp_info->ftp_msg->sendRequestAppMsgQueueId == IMAGE_LOG_ID)
      {
         apptype = APP_IMAGE_LOG;
      }

	  stIC_ScanSendResult networkJobResult;

	  memset(&networkJobResult, 0, sizeof(stIC_ScanSendResult));

	  networkJobResult.jobID     = ftp_info->ftp_msg->sendJobId;;
	  networkJobResult.scanJobID =  ftp_info->ftp_msg->systemJobID;
	  networkJobResult.jobType = JOBTYPE_FTP;
	  networkJobResult.errorCode = 0;
	  networkJobResult.result    = 2;
	  networkJobResult.addInfo   = 0;
	  networkJobResult.destCount = 1;
	  notifyJobStatusToSSM( &networkJobResult);
      result = ftpUpload(ftp_info->username, ftp_info->password, ftp_info->destination, ftp_info->port, ftp_info->filePath, ftp_info->serverType, ftp_info->proxyserver, ftp_info->isDebugMode,ftp_info->timeout,apptype);
      networkJobResult.errorCode = result;
      networkJobResult.result = networkJobResult.errorCode ? 6 : 5;
      notifyJobStatusToSSM( &networkJobResult);
   }
   else
   {
	   printf("\nFTP Connection Test");
      result = ftpConnTest(ftp_info->username, ftp_info->password, ftp_info->destination, ftp_info->port, ftp_info->serverType, ftp_info->proxyserver, ftp_info->isDebugMode,ftp_info->timeout);
   }
   if (ftp_info->ftp_msg == NULL)
   {
      deleteFTPInfo(ftp_info);
      ftp_info = NULL;
      return NULL;
   }
   if (ftp_info->reqType == TYPESYSMGR)
   {
      MESSAGE *ftp_msg = (MESSAGE *)malloc(sizeof(MESSAGE));
	  if (ftp_msg)
      {
         ftp_msg->msgType = MSG_SENDJOB_RESULT;
         ftp_msg->param1  = ftp_info->ftp_msg->sendJobId;
         ftp_msg->param2  = result;
         SYMsgSend((MODULE_ID)ftp_info->ftp_msg->sendRequestAppMsgQueueId, ftp_msg);
         free(ftp_msg);
         ftp_msg = NULL;
      }
   }
   if(ftp_info->ftp_msg->sendRequestAppMsgQueueId != IMAGE_LOG_ID)
   {
      stSend_SendResult *jobResult = (stSend_SendResult *)malloc(sizeof(stSend_SendResult));
      if (jobResult)
      {
         jobResult->result      = result;
         jobResult->reqAppType  = ftp_info->ftp_msg->sendRequestAppMsgQueueId;
         jobResult->sendJobType = SEND_TO_FTP;
         sendMSGToSysManager(jobResult, CMD_INT_TX_SEND_RESULT, ftp_info->ftp_msg->sendRequestAppMsgQueueId);
         free(jobResult);
         jobResult = NULL;
      }
   }
   deleteFTPInfo(ftp_info);
   ftp_info = NULL;
   return NULL;
}


/**
 * Thread to execute webdav function
 * @name WebDAV_Thread
 * @function
 * @param void *webdav_info_void - pointer to webdav info
 * @return void * - result of thread execution
 */
void *WebDAV_Thread(void *webdav_void)
{
   webdav_info_t *webdav_info = (webdav_info_t *)webdav_void;
   APP_TYPE apptype = APP_OTHER;
   if (webdav_info == NULL)
   {
      printf("\nwebdav_info is NULL");
      return NULL;
   }
   UINT32 result;

   if (webdav_info->reqType == TYPESYSMGR)
   {
      if(webdav_info->webdav_msg && webdav_info->webdav_msg->sendRequestAppMsgQueueId == IMAGE_LOG_ID)
      {
         apptype = APP_IMAGE_LOG;
      }

	  stIC_ScanSendResult networkJobResult;

	  memset(&networkJobResult, 0, sizeof(stIC_ScanSendResult));

	  networkJobResult.jobID     = webdav_info->webdav_msg->sendJobId;;
	  networkJobResult.scanJobID =  webdav_info->webdav_msg->systemJobID;
	  networkJobResult.jobType = JOBTYPE_WEBDAV;
	  networkJobResult.errorCode = 0;
	  networkJobResult.result    = 2;
	  networkJobResult.addInfo   = 0;
	  networkJobResult.destCount = 1;
	  notifyJobStatusToSSM( &networkJobResult);
      result = webDAVUpload(webdav_info->username, webdav_info->password, webdav_info->destination, webdav_info->port, webdav_info->filePath, webdav_info->serverType, webdav_info->proxyserver, webdav_info->isDebugMode,webdav_info->timeout,apptype);
      networkJobResult.errorCode   = result;
      networkJobResult.result = networkJobResult.errorCode ? 6 : 5;
      notifyJobStatusToSSM( &networkJobResult);
   }
   else
   {
      result = webDAVConnTest(webdav_info->username, webdav_info->password, webdav_info->destination, webdav_info->port, webdav_info->serverType, webdav_info->proxyserver, webdav_info->isDebugMode,webdav_info->timeout);
   }

   printf("Error = %lu", result);


   if (webdav_info->webdav_msg == NULL)
   {
      deleteWebDAVInfo(webdav_info);
      webdav_info = NULL;
      return NULL;
   }

   if (webdav_info->reqType == TYPESYSMGR)
   {
      MESSAGE *webdav_msg = (MESSAGE *)malloc(sizeof(MESSAGE));
	  if(webdav_msg)
      {
         webdav_msg->msgType = MSG_SENDJOB_RESULT;
         webdav_msg->param1  = webdav_info->webdav_msg->sendJobId;
         webdav_msg->param2  = result;
         SYMsgSend((MODULE_ID)webdav_info->webdav_msg->sendRequestAppMsgQueueId, webdav_msg);
         free(webdav_msg);
         webdav_msg = NULL;
      }
   }
   if(webdav_info->webdav_msg->sendRequestAppMsgQueueId != IMAGE_LOG_ID)
   {
	   stSend_SendResult *jobResult = (stSend_SendResult *)malloc(sizeof(stSend_SendResult));
	   if (jobResult)
	   {  
	      jobResult->result      = result;
	      jobResult->reqAppType  = webdav_info->webdav_msg->sendRequestAppMsgQueueId;
	      jobResult->sendJobType = SEND_TO_WEBDAV;
	      sendMSGToSysManager(jobResult, CMD_INT_TX_SEND_RESULT, webdav_info->webdav_msg->sendRequestAppMsgQueueId);
	      free(jobResult);
	      jobResult = NULL;
	   }
   }
   deleteWebDAVInfo(webdav_info);
   webdav_info = NULL;
   return NULL;
}


void deleteEmailInfo(email_info_message_t *email_message)
{

   if (email_message)
   {
	  safefree(email_message->username);
	  safefree(email_message->password);
	  safefree(email_message->senderName);
	  safefree(email_message->senderEmail);
	  safefree(email_message->mailhost);
	  safefree(email_message->subject);
	  safefree(email_message->toAddress);
	 // safefree(email_message->ccAddress);
	  //safefree(email_message->bccAddress);
	  safefree(email_message->content);
	  safefree(email_message->attachmentPath);
	  safefree(email_message->email_msg);
	  //safefree(email_message->contentsPath);
	  //deleteIfaxInfoRequest(email_message->ifax_request);
	  //deleteEmailReport(email_message->email_report);
	  free(email_message);
   }
}


void deleteFTPInfo(ftp_info_t *ftp_info)
{
   if (ftp_info)
   {
      if (ftp_info->username)
      {
         free(ftp_info->username);
         ftp_info->username = NULL;
      }

      if (ftp_info->password)
      {
         free(ftp_info->password);
         ftp_info->password = NULL;
      }

      if (ftp_info->destination)
      {
         free(ftp_info->destination);
         ftp_info->destination = NULL;
      }

      if (ftp_info->filePath)
      {
         free(ftp_info->filePath);
         ftp_info->filePath = NULL;
      }


      if (ftp_info->proxyserver)
      {
         free(ftp_info->proxyserver);
         ftp_info->proxyserver = NULL;
      }


      if (ftp_info->ftp_msg)
      {
         free(ftp_info->ftp_msg);
         ftp_info->ftp_msg = NULL;
      }

      free(ftp_info);
   }
}


void deleteWebDAVInfo(webdav_info_t *webdav_info)
{
   if (webdav_info)
   {
      if (webdav_info->username)
      {
         free(webdav_info->username);
         webdav_info->username = NULL;
      }

      if (webdav_info->password)
      {
         free(webdav_info->password);
         webdav_info->password = NULL;
      }

      if (webdav_info->destination)
      {
         free(webdav_info->destination);
         webdav_info->destination = NULL;
      }

      if (webdav_info->filePath)
      {
         free(webdav_info->filePath);
         webdav_info->filePath = NULL;
      }


      if (webdav_info->proxyserver)
      {
         free(webdav_info->proxyserver);
         webdav_info->proxyserver = NULL;
      }


      if (webdav_info->webdav_msg)
      {
         free(webdav_info->webdav_msg);
         webdav_info->webdav_msg = NULL;
      }
      free(webdav_info);
   }
}


void deleteSMBInfo(smb_info_t *smb_info)
{
   if (smb_info)
   {
      if (smb_info->username)
      {
         free(smb_info->username);
         smb_info->username = NULL;
      }

      if (smb_info->password)
      {
         free(smb_info->password);
         smb_info->password = NULL;
      }

      if (smb_info->smbshare)
      {
         free(smb_info->smbshare);
         smb_info->smbshare = NULL;
      }

      if (smb_info->remotePath)
      {
         free(smb_info->remotePath);
         smb_info->remotePath = NULL;
      }


      if (smb_info->filePath)
      {
         free(smb_info->filePath);
         smb_info->filePath = NULL;
      }


      if (smb_info->smb_msg)
      {
         free(smb_info->smb_msg);
         smb_info->smb_msg = NULL;
      }
      free(smb_info);
   }
}

#ifdef HAVE_IMAGELOG
void PopulateImageLogVars()
{
	const char *val_PROTO_ID = NULL;
	const char *val_PROTO_PW = NULL;
	const char *val_PROTO_Address = NULL;
	const char *val_PROTO_Path = NULL;
	sqlite3		  *g_pDB = dbInstance;
	sqlite3_stmt	*pStmt = NULL;
	const char	  *pTail = NULL;
	int nResult = 0;
	char szQuery[2048];
	memset(g_PROTO_ID,0x00,sizeof(g_PROTO_ID));
	memset(g_PROTO_PW,0x00,sizeof(g_PROTO_PW));
	memset(g_PROTO_Address,0x00,sizeof(g_PROTO_Address));
	memset(g_PROTO_Path,0x00,sizeof(g_PROTO_Path));
	g_PROTO_Port = 0;
	g_nProtocolType = PROTO_SFTP;
	snprintf(szQuery,sizeof(szQuery),"SELECT * FROM %s WHERE NO=4","CONF_IMAGELOG");
	nResult = sqlite3_prepare(g_pDB, szQuery, 2048, &pStmt, &pTail);
	if(nResult == SQLITE_OK)
	{
		if(sqlite3_step(pStmt) == SQLITE_ROW)
		{
			g_nProtocolType =  sqlite3_column_int(pStmt, 24);
			if(g_nProtocolType >= PROTO_FTP && g_nProtocolType < PROTO_WEBDAV_HTTP)
			{
				val_PROTO_Address =  (char *)sqlite3_column_text(pStmt, 14);
				if(val_PROTO_Address)
				{
					strncpy(g_PROTO_Address,val_PROTO_Address,MAX_STR_SIZE-1);
					g_PROTO_Address[MAX_STR_SIZE-1] = '\0';
				}
				val_PROTO_Path =  (char *)sqlite3_column_text(pStmt, 15);
				if(val_PROTO_Path)
				{
					strncpy(g_PROTO_Path,val_PROTO_Path,MAX_STR_SIZE-1);
					g_PROTO_Path[MAX_STR_SIZE-1] = '\0';
				}
				val_PROTO_ID =  (char *)sqlite3_column_text(pStmt, 16);
				if(val_PROTO_ID)
				{
					strncpy(g_PROTO_ID,val_PROTO_ID,MAX_STR_SIZE-1);
					g_PROTO_ID[MAX_STR_SIZE-1] = '\0';
				}
				val_PROTO_PW =  (char *)sqlite3_column_text(pStmt, 17);
				if(val_PROTO_PW)
				{
					strncpy(g_PROTO_PW,val_PROTO_PW,MAX_STR_SIZE-1);
					g_PROTO_PW[MAX_STR_SIZE-1] = '\0';
				}
				g_PROTO_Port =  sqlite3_column_int(pStmt, 18);
			}
			else
			{
				val_PROTO_Address =  (char *)sqlite3_column_text(pStmt, 19);
				if(val_PROTO_Address)
				{
					strncpy(g_PROTO_Address,val_PROTO_Address,MAX_STR_SIZE-1);
					g_PROTO_Address[MAX_STR_SIZE-1] = '\0';
				}
				val_PROTO_Path =  (char *)sqlite3_column_text(pStmt, 20);
				if(val_PROTO_Path)
				{
					strncpy(g_PROTO_Path,val_PROTO_Path,MAX_STR_SIZE-1);
					g_PROTO_Path[MAX_STR_SIZE-1] = '\0';
				}
	 			val_PROTO_ID =  (char *)sqlite3_column_text(pStmt, 21);
				if(val_PROTO_ID)
				{
					strncpy(g_PROTO_ID,val_PROTO_ID,MAX_STR_SIZE-1);
					g_PROTO_ID[MAX_STR_SIZE-1] = '\0';
				}
				val_PROTO_PW =  (char *)sqlite3_column_text(pStmt, 22);
				if(val_PROTO_PW)
				{
					strncpy(g_PROTO_PW,val_PROTO_PW,MAX_STR_SIZE-1);
					g_PROTO_PW[MAX_STR_SIZE-1] = '\0';
				}
				g_PROTO_Port =  sqlite3_column_int(pStmt, 23);
			}
		}
	}
	sqlite3_finalize(pStmt);
	pStmt = NULL;
	return ;
}

int Image_log_Upload(char *filePath)
{
   pthread_attr_t threadAttributes;
   int result = 0;
   int strSize = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }
   if(g_nProtocolType >= PROTO_FTP && g_nProtocolType < PROTO_WEBDAV_HTTP)
   {
      pthread_t ftp_thread;
      ftp_info_t *ftp_info = (ftp_info_t *)malloc(sizeof(ftp_info_t));
      if (ftp_info == NULL)
      {
         pthread_attr_destroy(&threadAttributes);
         return ERROR_SOL_INIT;
      }
      strSize = strlen(g_PROTO_ID) + 1;
      if (strSize > 1)
      {
         ftp_info->username = (char *)malloc(strSize);
         if(ftp_info->username)
            memcpy(ftp_info->username, g_PROTO_ID,strSize);
      }
      else
      {
         ftp_info->username = NULL;
      }
      strSize = strlen(g_PROTO_PW) + 1;
      if (strSize > 1)
      {
         ftp_info->password = (char *)malloc(strSize);
         if(ftp_info->password)
            memcpy(ftp_info->password, g_PROTO_PW,strSize);
      }
      else
      {
         ftp_info->password = NULL;
      }  
      strSize = strlen(g_PROTO_Address) + 1;
      if (strSize > 1)
      {
         ftp_info->destination = (char *)malloc(MAX_URL_SIZE);
         if(ftp_info->destination)
         {
            if ((strncasecmp(g_PROTO_Address, "ftps://", strlen("ftps://")) == 0) || (strncasecmp(g_PROTO_Address, "ftpes://", strlen("ftpes://")) == 0) || (strncasecmp(g_PROTO_Address, "sftp://", strlen("sftp://")) == 0) ||(strncasecmp(g_PROTO_Address, "ftp://", strlen("ftp://")) == 0) )
            {
               memcpy(ftp_info->destination, g_PROTO_Address, strlen(g_PROTO_Address)+1);
            }
            else
            {
               if(g_nProtocolType == PROTO_FTP)
                  strcpy(ftp_info->destination,"ftp://");
               else if(g_nProtocolType == PROTO_SFTP)
                  strcpy(ftp_info->destination,"sftp://");
               else if(g_nProtocolType == PROTO_FTPS)
                  strcpy(ftp_info->destination,"ftps://");
  
               strcat(ftp_info->destination,g_PROTO_Address);
            }
		    strSize = strlen(g_PROTO_Path) + 1;
            if (strSize > 1)
            {
               strcat(ftp_info->destination,"/");
               strcat(ftp_info->destination,g_PROTO_Path);
            }
         }
      }
      else
      {
         ftp_info->destination = NULL;
      }
	  if (filePath)
      {
         strSize = strlen(filePath) + 1;
         ftp_info->filePath = (char *)malloc(strSize);
         if(ftp_info->filePath)
            memcpy(ftp_info->filePath,filePath,strSize);
      }
      else
      {
         ftp_info->filePath = NULL;
      }
      ftp_info->port = g_PROTO_Port;
      ftp_info->serverType = 0;
      ftp_info->proxyserver = NULL;
      ftp_info->isDebugMode = 0;
      ftp_info->reqType     = TYPESYSMGR;
      ftp_info->ftp_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
      if(ftp_info->ftp_msg)
      {
         ftp_info->ftp_msg->sendRequestAppMsgQueueId = IMAGE_LOG_ID;
         ftp_info->ftp_msg->sendJobId = 0; //Check
      }
      ftp_info->timeout = FTP_TIMEOUT;
      result = pthread_create(&ftp_thread, &threadAttributes, FTP_Thread, (void *)ftp_info);
   }
   else
   {
      pthread_t webdav_thread;
      webdav_info_t *webdav_info = (webdav_info_t *)malloc(sizeof(webdav_info_t));
      if (webdav_info == NULL)
      {
         pthread_attr_destroy(&threadAttributes);
         return ERROR_SOL_INIT;
      }
  
      strSize = strlen(g_PROTO_ID) + 1;
      if (strSize > 1)
      {
         webdav_info->username = (char *)malloc(strSize);
         if(webdav_info->username)
            memcpy(webdav_info->username, g_PROTO_ID, strSize);
      }
      else
      {
         webdav_info->username = NULL;
      }
      strSize = strlen(g_PROTO_PW) + 1;
      if (strSize > 1)
      {
         webdav_info->password = (char *)malloc(strSize);
         if(webdav_info->password)
            memcpy(webdav_info->password, g_PROTO_PW, strSize);
      }
      else
      {
         webdav_info->password = NULL;
      }  
      strSize = strlen(g_PROTO_Address) + 1;
      if (strSize > 1)
      {
         webdav_info->destination = (char *)malloc(MAX_URL_SIZE);
         if(webdav_info->destination)
         {
            if ((strncasecmp(g_PROTO_Address, "https://", strlen("https://")) == 0) || (strncasecmp(g_PROTO_Address, "http://", strlen("http://")) == 0) )
            {
               memcpy(webdav_info->destination, g_PROTO_Address, strlen(g_PROTO_Address)+1);
            }
            else
            {
               if(g_nProtocolType == PROTO_WEBDAV_HTTP)
                  strcpy(webdav_info->destination,"http://");
               else if(g_nProtocolType == PROTO_WEBDAV_HTTPS)
                  strcpy(webdav_info->destination,"https://");
               strcat(webdav_info->destination,g_PROTO_Address);
            }  
            strSize = strlen(g_PROTO_Path) + 1;
	        if (strSize > 1)
            {
               strcat(webdav_info->destination,"/");
               strcat(webdav_info->destination,g_PROTO_Path);
            }
  
         }
  
      }
      else
      {
         webdav_info->destination = NULL;
      }
      if (filePath)
      {
         strSize = strlen(filePath) + 1;
         webdav_info->filePath = (char *)malloc(strSize);
         if(webdav_info->filePath)
            memcpy(webdav_info->filePath, filePath, strSize);
      }
      else
      {
         webdav_info->filePath = NULL;
      }
      webdav_info->port = g_PROTO_Port;
      webdav_info->serverType = 0;
      webdav_info->proxyserver = NULL;
      webdav_info->isDebugMode = 0;
      webdav_info->reqType     = TYPESYSMGR;
      webdav_info->webdav_msg = (stSolution_Request *)malloc(sizeof(stSolution_Request));
      if(webdav_info->webdav_msg)
      {
         webdav_info->webdav_msg->sendRequestAppMsgQueueId = IMAGE_LOG_ID;
         webdav_info->webdav_msg->sendJobId= 0;
      }
      webdav_info->timeout = WEBDAV_TIMEOUT;
      result = pthread_create(&webdav_thread, &threadAttributes, WebDAV_Thread, (void *)webdav_info);
   }
   pthread_attr_destroy(&threadAttributes);
   return 0;
}
#endif
