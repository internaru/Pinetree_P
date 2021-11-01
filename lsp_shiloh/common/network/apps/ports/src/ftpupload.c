#include "ftpupload.h"
#include "urlencode.h"

#define MAX_FTP_FILES                         1500
#define MAX_FTP_SIZE                          1024 * 1024 * 512
#define FTP_RESPONSE_TIMEOUT                  30
#define FTP_UPLOAD_FAILED                     1000
#define FTP_ERROR_INVALID_LOCALPATH           1001
#define FTP_ERROR_INVALID_FTP_DESTINATION     1002
#define FTP_ERROR_CONFIG                      1003
#define FTP_UPLOAD_SUCCESS                    0
#define FTP_ERROR_UNSUPPORTED_PROTOCOL        1
#define FTP_ERROR_FAILED_INIT                 2
#define FTP_ERROR_URL_MALFORMAT               3
#define FTP_ERROR_COULDNT_RESOLVE_PROXY       5
#define FTP_ERROR_COULDNT_RESOLVE_HOST        6
#define FTP_ERROR_COULDNT_CONNECT             7
#define FTP_ERROR_FTP_WEIRD_SERVER_REPLY      8
#define FTP_ERROR_REMOTE_ACCESS_DENIED        9
#define FTP_ERROR_FTP_ACCEPT_FAILED           10
#define FTP_ERROR_FTP_ACCEPT_TIMEOUT          12
#define FTP_ERROR_FTP_CANT_GET_HOST           15
#if 0
#define FTP_ERROR_OBSOLETE16                  16
#define FTP_ERROR_FTP_COULDNT_SET_TYPE        17
#define FTP_ERROR_PARTIAL_FILE                18
#define FTP_ERROR_FTP_COULDNT_RETR_FILE       19
#define FTP_ERROR_OBSOLETE20                  20
#define FTP_ERROR_QUOTE_ERROR                 21
#define FTP_ERROR_WRITE_ERROR                 23
#define FTP_ERROR_OBSOLETE24                  24
#define FTP_ERROR_UPLOAD_FAILED               25
#define FTP_ERROR_READ_ERROR                  26
#define FTP_ERROR_OUT_OF_MEMORY               27
#define FTP_ERROR_OPERATION_TIMEDOUT          28
#define FTP_ERROR_OBSOLETE29                  29
#define FTP_ERROR_FTP_PORT_FAILED             30
#define FTP_ERROR_FTP_COULDNT_USE_REST        31
#define FTP_ERROR_OBSOLETE32                  32
#define FTP_ERROR_RANGE_ERROR                 33
#define FTP_ERROR_HTTP_POST_ERROR             34
#define FTP_ERROR_SSL_CONNECT_ERROR           35
#define FTP_ERROR_BAD_DOWNLOAD_RESUME         36
#define FTP_ERROR_FILE_COULDNT_READ_FILE      37
#define FTP_ERROR_OBSOLETE40                  40
#define FTP_ERROR_FUNCTION_NOT_FOUND          41
#define FTP_ERROR_ABORTED_BY_CALLBACK         42
#define FTP_ERROR_BAD_FUNCTION_ARGUMENT       43
#define FTP_ERROR_OBSOLETE44                  44
#define FTP_ERROR_INTERFACE_FAILED            45
#define FTP_ERROR_OBSOLETE46                  46
#define FTP_ERROR_TOO_MANY_REDIRECTS          47
#define FTP_ERROR_UNKNOWN_OPTION              48
#define FTP_ERROR_OBSOLETE50                  50
#define FTP_ERROR_PEER_FAILED_VERIFICATION    51
#define FTP_ERROR_GOT_NOTHING                 52
#define FTP_ERROR_SSL_ENGINE_NOTFOUND         53
#define FTP_ERROR_SSL_ENGINE_SETFAILED        54
#define FTP_ERROR_SEND_ERROR                  55
#define FTP_ERROR_RECV_ERROR                  56
#define FTP_ERROR_OBSOLETE57                  57
#define FTP_ERROR_SSL_CERTPROBLEM             58
#define FTP_ERROR_SSL_CIPHER                  59
#define FTP_ERROR_SSL_CACERT                  60
#define FTP_ERROR_BAD_CONTENT_ENCODING        61
#define FTP_ERROR_LDAP_INVALID_URL            62
#define FTP_ERROR_FILESIZE_EXCEEDED           63
#define FTP_ERROR_USE_SSL_FAILED              64
#define FTP_ERROR_SEND_FAIL_REWIND            65
#define FTP_ERROR_SSL_ENGINE_INITFAILED       66
#define FTP_ERROR_LOGIN_DENIED                67
#define FTP_ERROR_TFTP_NOTFOUND               68
#define FTP_ERROR_TFTP_PERM                   69
#define FTP_ERROR_REMOTE_DISK_FULL            70
#define FTP_ERROR_TFTP_ILLEGAL                71
#define FTP_ERROR_TFTP_UNKNOWNID              72
#define FTP_ERROR_REMOTE_FILE_EXISTS          73
#define FTP_ERROR_TFTP_NOSUCHUSER             74
#define FTP_ERROR_CONV_FAILED                 75
#define FTP_ERROR_CONV_REQD                   76
#define FTP_ERROR_SSL_CACERT_BADFILE          77
#define FTP_ERROR_REMOTE_FILE_NOT_FOUND       78
#define FTP_ERROR_SSH                         79
#define FTP_ERROR_SSL_SHUTDOWN_FAILED         80
#define FTP_ERROR_AGAIN                       81
#define FTP_ERROR_SSL_CRL_BADFILE             82
#define FTP_ERROR_SSL_ISSUER_ERROR            83
#define FTP_ERROR_FTP_PRET_FAILED             84
#define FTP_ERROR_RTSP_CSEQ_ERROR             85
#define FTP_ERROR_RTSP_SESSION_ERROR          86
#define FTP_ERROR_FTP_BAD_FILE_LIST           87
#define FTP_ERROR_CHUNK_FAILED                88
#define FTP_ERROR_NO_CONNECTION_AVAILABLE     89
#endif
typedef enum FTP_TYPE_S
{
   FTP_FTP,
   FTP_FTPS,
   FTP_FTPES,
   FTP_SFTP,
} FTP_TYPE;
//FTP Implementation Limitations - symlinks and empty directories will not be created
//FTP and FTPS supported. SFTP Not supported

/**
 * Recusively get the list of files in a specified directory  - Internal function
 * @name getFTPFileList
 * @function
 * @param const char *dir - input directory path (IN parameter)
 * @param  char **strs - Two dimensional array of strings to store list of files. (OUT parameter)
 * @param  unsigned long *totalsize - total size of directory (OUT parameter)
 * @param  int *filecount - number of files in input directory (OUT parameter)
 * @return void
 */
void getFTPFileList(const char *dir, char **strs, unsigned long *totalsize, int *filecount)
{
   DIR           *dp    = NULL;
   struct dirent *entry = NULL;
   struct stat   statbuf;
   if ((dp = opendir(dir)) == NULL)
   {
      fprintf(stderr, "cannot open directory: %s\n", dir);
      return;
   }
   while ((entry = readdir(dp)) != NULL)
   {
      char tempfsEntry[PATH_MAX] = { 0 };
      strcpy(tempfsEntry, dir);
      strcat(tempfsEntry, "/");
      strcat(tempfsEntry, entry->d_name);
      lstat(tempfsEntry, &statbuf);
      if (S_ISDIR(statbuf.st_mode))
      {
         if ((strcmp(".", entry->d_name) == 0) ||
             (strcmp("..", entry->d_name) == 0))
         {
            continue;
         }
         getFTPFileList(tempfsEntry, strs, totalsize, filecount);
      }
      else
      {
         if (*filecount >= MAX_FTP_FILES)
         {
            closedir(dp);
            return;
         }
         if (*totalsize > MAX_FTP_SIZE)
         {
            closedir(dp);
            return;
         }
         strs[*filecount] = (char *)malloc(PATH_MAX);
         strcpy(strs[*filecount], tempfsEntry);
         (*totalsize) += statbuf.st_size;
         (*filecount)++;
      }
   }
   closedir(dp);
}


/**
 * callback to Read the local file, to be used by curl  - Internal function
 * @name ftp_read
 * @function
 * @param void *ptr - Pointer to a block of memory with a size of at least (size*readCount) bytes
 * @param  size_t size - Chunk size
 * @param  size_t readCount - Read count
 * @param void *stream - FILE pointer
 * @return int retcode - return value of read operation
 */
static size_t ftp_read(void *ptr, size_t size, size_t readCount, void *stream)
{
   size_t retcode = fread(ptr, size, readCount, (FILE *)stream);
   return(retcode);
}


/**
 * Interface to upload a file on remote ftp server
 *
 * @name ftpUpload
 * @function
 * @param   const char *username - FTP server username, "anonymous" in case of anonymous logon
 * @param   const char *password - FTP server password, "anonymous" in case of anonymous logon
 * @param   const char *destination - ftp server location to upload the file
 * @param   int port - ftp server listen port usually 21
 * @param   const char *filePath - path to local file that will be uploaded to server
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   unsigned long - 0 on success, Non-Zero error code on Failure
 */
unsigned long ftpUpload(const char *username, const char *password, const char *destination, int port, const char *filePath, int ServerType, const char *proxyserver, int isDebugMode,int timeout,APP_TYPE apptype)
{
   CURL        *curl = NULL;
   CURLcode    res;
   FILE        *fileptr = NULL;
   curl_off_t  localFileSize;
   struct stat structLocalFileInfo;
   FTP_TYPE    ftp_upload_type = FTP_FTP;
   if (checkBlankorNull(filePath))
   {
      printf("\nInvalid FilePATH");
      return(ERROR_CONFIG);
   }
   if (checkBlankorNull(destination))
   {
      printf("\nInvalid FTP destination");
      return(ERROR_CONFIG);
   }
   curl = curl_easy_init();
   if (curl)
   {
      char strUsernamePassword[256] = { 0 };
      char strRemoteFileURL[1024]   = { 0 };
      char strUploadFileName[256]   = { 0 };
      char strURL[1024]             = { 0 };
      curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
      //curl_easy_setopt(curl, CURLOPT_ADDRESS_SCOPE, if_nametoindex("eth0"));
      if ((strncasecmp(destination, "ftps://", strlen("ftps://")) == 0))
      {
         sprintf(strURL, "ftp://%s", destination + strlen("ftps://"));
         ftp_upload_type = FTP_FTPS;
      }
      //The following URL format is just used to follow the URL format convention used in implementation
      //will be changed in future
      else if ((strncasecmp(destination, "ftpes://", strlen("ftpes://")) == 0))
      {
         sprintf(strURL, "ftp://%s", destination + strlen("ftpes://"));
         ftp_upload_type = FTP_FTPES;
      }
      else if (strncasecmp(destination, "sftp://", strlen("sftp://")) == 0)
      {
         strcpy(strURL, destination);
         ftp_upload_type = FTP_SFTP;
      }
      else if (strncasecmp(destination, "ftp://", strlen("ftp://")) == 0)
      {
         strcpy(strURL, destination);
         ftp_upload_type = FTP_FTP;
      }
      else
      {
         sprintf(strURL, "ftp://%s", destination);
         ftp_upload_type = FTP_FTP;
      }
      if (checkBlankorNull(username) == false)
      {
         sprintf(strUsernamePassword, "%s:%s", username, password ? password : "");
      }
      else
      {
         strcpy(strUsernamePassword, "");
      }
      if (stat(filePath, &structLocalFileInfo))
      {
         printf("\nLocal file [%s] not present", filePath);
         curl_easy_cleanup(curl);
         return(ERROR_CONFIG);
      }
      if (isDebugMode)
      {
         curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      }
      if (!isDirectory(filePath))
      {
         localFileSize = (curl_off_t)structLocalFileInfo.st_size;
         if (localFileSize > MAX_FTP_SIZE)
         {
            printf("\n[FTP Upload] localFileSize > MAX_FTP_SIZE");
            curl_easy_cleanup(curl);
            return(ERROR_BEYOND_ALLOWED_LIMIT);
         }
         if (localFileSize == 0)
         {
            printf("\n[FTP Upload]localFileSize == 0");
            curl_easy_cleanup(curl);
            return(ERROR_CONFIG);
         }
         fileptr = fopen(filePath, "rb");
         if (fileptr == NULL)
         {
            curl_easy_cleanup(curl);
            return(ERROR_CONFIG);
         }
         char *fileBaseName = basename((char *)filePath);
         if (fileBaseName == NULL)
         {
            fclose(fileptr);
            curl_easy_cleanup(curl);
            return(ERROR_CONFIG);
         }
         strcpy(strUploadFileName, fileBaseName);
         sprintf(strRemoteFileURL, "%s/%s", strURL, strUploadFileName);
         if (port > 0)
         {
            curl_easy_setopt(curl, CURLOPT_PORT, port);
         }
         curl_easy_setopt(curl, CURLOPT_READFUNCTION, ftp_read);
         char strEncodedURL[MAX_URL_LENGTH];
         memset(strEncodedURL,0,MAX_URL_LENGTH);
         solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
         curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
         if (checkBlankorNull(strUsernamePassword) == false)
         {
            curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
         }
         curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
         curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, FTP_RESPONSE_TIMEOUT);
         if (ftp_upload_type == FTP_FTPS)
         {
            curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
            curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
         }
         else if (ftp_upload_type == FTP_FTPES)
         {
            curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
            curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
         }
         else if (ftp_upload_type == FTP_SFTP)
         {
            curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PASSWORD);
         }
         curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
         curl_easy_setopt(curl, CURLOPT_READDATA, fileptr);
         curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)localFileSize);
         /* ServerType = 1 - proxy , 0 - normal ftp server*/
         if (ServerType == 1)
         {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
         }
         res = curl_easy_perform(curl);
         if (CURLE_OK != res)
         {
            fclose(fileptr);
            curl_easy_cleanup(curl);
            printf("Upload failed return code[%d]: %s\n", res, curl_easy_strerror(res));
            return(getError(res));
         }
         curl_easy_cleanup(curl);
         fclose(fileptr);
      }
      else
      {
         char *fileList[MAX_FTP_FILES] = { 0 };
         //printf("Directory scan of %s\n", filePath);
         int           filecount = 0;
         unsigned long totalsize = 0;

         int k = 0;
         getFTPFileList(filePath, fileList, &totalsize, &filecount);
         if (totalsize > MAX_FTP_SIZE || totalsize == 0)
         {
            k = 0;
            while (k < filecount)
            {
               if (fileList[k])
               {
                  free(fileList[k]);
                  fileList[k] = NULL;
               }
               k++;
            }
            curl_easy_cleanup(curl);
            if (totalsize == 0)
            {
               printf("[FTP Upload]Empty Directory/Files (totalsize = 0)\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("[FTP Upload] totalsize > MAX_FTP_SIZE\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         if ((filecount > MAX_FTP_FILES) || (filecount == 0))
         {
            k = 0;
            while (k < filecount)
            {
               if (fileList[k])
               {
                  free(fileList[k]);
                  fileList[k] = NULL;
               }
               k++;
            }
            curl_easy_cleanup(curl);
            if (filecount == 0)
            {
               printf("[FTP Upload]Empty Directory/Files (filecount = 0)\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("[FTP Upload] filecount > MAX_FTP_FILES\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         //char *tmpdir = basename((char *)filePath);
         shiftEndfile(fileList, filecount);
         while (k < filecount)
         {
            localFileSize = getSize(fileList[k]);
            // Skip empty file name..Related to Image Log upload file selection feature but applicable to other upload scenarios
            if(checkBlankorNull(fileList[k]))
            {
               k++;
               continue;
            }
            fileptr       = fopen(fileList[k], "rb");
            //printf("[%d]Copy file %s\n", k, fileList[k]);
            if (fileptr == NULL)
            {
               k = 0;
               while (k < filecount)
               {
                  if (fileList[k])
                  {
                     free(fileList[k]);
                     fileList[k] = NULL;
                  }
                  k++;
               }
               curl_easy_cleanup(curl);
               return(ERROR_CONFIG);
            }
            char *tempStr = NULL;
            tempStr = strstr(fileList[k], filePath);
            //printf("[%d]Copy file %s\n", k, tempStr + strlen(filePath));
            //printf("tmpdir %s\n", tmpdir);
            if (tempStr)
            {
               if (apptype == APP_IMAGE_LOG)
               {
                  char *tmpdir = basename((char *)filePath);
                  if (tmpdir)
                  {
                     sprintf(strRemoteFileURL, "%s/%s/%s", strURL, tmpdir, tempStr + strlen(filePath));
                  }
                  else
                  {
                     sprintf(strRemoteFileURL, "%s/%s", strURL, tempStr + strlen(filePath));
                  }
               }
               else
               {
                  sprintf(strRemoteFileURL, "%s/%s", strURL, tempStr + strlen(filePath));
               }
            }
            else
            {
               fclose(fileptr);
               k++;
               continue;
            }
            if (port > 0)
            {
               curl_easy_setopt(curl, CURLOPT_PORT, port);
            }
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
            curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, FTP_RESPONSE_TIMEOUT);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, ftp_read);
            char strEncodedURL[MAX_URL_LENGTH];
            memset(strEncodedURL,0,MAX_URL_LENGTH);
            solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
            curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
            if (checkBlankorNull(strUsernamePassword) == false)
            {
               curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
            }
            curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_READDATA, fileptr);
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)localFileSize);
            if (ftp_upload_type == FTP_FTPS)
            {
               curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
               curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
               curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
               curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
            }
            else if (ftp_upload_type == FTP_FTPES)
            {
               curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
               curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
               curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
               curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
            }
            else if (ftp_upload_type == FTP_SFTP)
            {
               curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PASSWORD);
            }
            /* ServerType = 1 - proxy , 0 - normal ftp server*/
            if (ServerType == 1)
            {
               curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
            }
            res = curl_easy_perform(curl);
            if (CURLE_OK != res)
            {
               fclose(fileptr);
               curl_easy_cleanup(curl);
               printf("Upload failed return code[%d]: %s\n", res, curl_easy_strerror(res));
               k = 0;
               while (k < filecount)
               {
                  if (fileList[k])
                  {
                     free(fileList[k]);
                     fileList[k] = NULL;
                  }
                  k++;
               }
               return(getError(res));
            }
            fclose(fileptr);
            k++;
         }
         k = 0;
         while (k < filecount)
         {
            if (fileList[k])
            {
               free(fileList[k]);
               fileList[k] = NULL;
            }
            k++;
         }
         curl_easy_cleanup(curl);
      }
   }
   else
   {
      printf("\nCurl Initialisation Error..FTP Upload failed");
      return(ERROR_CONFIG);
   }
   printf("\n FTP Upload Successful");
   return(CURLE_OK);
}


unsigned long ftpConnTest(const char *username, const char *password, const char *destination, int port, int ServerType, const char *proxyserver, int isDebugMode,int timeout)
{
   CURL     *curl = NULL;
   CURLcode res;
   FTP_TYPE ftp_upload_type = FTP_FTP;
   if (checkBlankorNull(destination))
   {
      return(ERROR_CONFIG);
   }
   curl = curl_easy_init();
   if (curl)
   {
      char strUsernamePassword[256] = { 0 };
      char strRemoteFileURL[1024]   = { 0 };
      char strURL[1024]             = { 0 };
      curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
      if ((strncasecmp(destination, "ftps://", strlen("ftps://")) == 0))
      {
         sprintf(strURL, "ftp://%s", destination + strlen("ftps://"));
         ftp_upload_type = FTP_FTPS;
      }
      //The following URL format is just used to follow the URL format convention used in implementation
      //will be changed in future
      else if ((strncasecmp(destination, "ftpes://", strlen("ftpes://")) == 0))
      {
         sprintf(strURL, "ftp://%s", destination + strlen("ftpes://"));
         ftp_upload_type = FTP_FTPES;
      }
      else if (strncasecmp(destination, "sftp://", strlen("sftp://")) == 0)
      {
         //Current curl library is not compiled with SFTP support..required libssh
         strcpy(strURL, destination);
         ftp_upload_type = FTP_SFTP;
         curl_easy_cleanup(curl);
         printf("\nSFTP not supported. Requires libssh");
         return(ERROR_CONFIG);
      }
      else if (strncasecmp(destination, "ftp://", strlen("ftp://")) == 0)
      {
         strcpy(strURL, destination);
         ftp_upload_type = FTP_FTP;
      }
      else
      {
         sprintf(strURL, "ftp://%s", destination);
         ftp_upload_type = FTP_FTP;
      }
      if (checkBlankorNull(username) == false)
      {
         sprintf(strUsernamePassword, "%s:%s", username, password ? password : "");
      }
      else
      {
         strcpy(strUsernamePassword, "");
      }
      if (isDebugMode)
      {
         curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      }
      sprintf(strRemoteFileURL, "%s/", strURL);
      if (port > 0)
      {
         curl_easy_setopt(curl, CURLOPT_PORT, port);
      }
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
      curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
      char strEncodedURL[MAX_URL_LENGTH];
      memset(strEncodedURL,0,MAX_URL_LENGTH);
      solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
      curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
      if (checkBlankorNull(strUsernamePassword) == false)
      {
         curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
      }
      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
      curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, FTP_RESPONSE_TIMEOUT);
      if (ftp_upload_type == FTP_FTPS)
      {
         curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
         curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
      }
      else if (ftp_upload_type == FTP_FTPES)
      {
         curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
         curl_easy_setopt(curl, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
      }
      else if (ftp_upload_type == FTP_SFTP)
      {
         curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PASSWORD);
      }
      /* ServerType = 1 - proxy , 0 - normal ftp server*/
      if (ServerType == 1)
      {
         curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
      }
      res = curl_easy_perform(curl);
      if (CURLE_OK != res)
      {
         curl_easy_cleanup(curl);
         printf("\nFTP Connection Test Failed : %s", curl_easy_strerror(res));
         return(getError(res));
      }
      else
      {
         long response;
         res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
         if (response / 100 != 2)
         {
            curl_easy_cleanup(curl);
            printf("\nFTP Connection Test Failed");
            return(ERROR_FAILED);
         }
      }
      curl_easy_cleanup(curl);
   }
   else
   {
      return(ERROR_CONFIG);
   }
   printf("\nFTP Connection Test OK");
   return(CURLE_OK);
}
