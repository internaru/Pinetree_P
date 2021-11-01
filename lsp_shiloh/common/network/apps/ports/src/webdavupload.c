#include "webdavupload.h"
#include "urlencode.h"

#define MAX_WEBDAV_FILES    500
#define MAX_WEBDAV_SIZE     1024 * 1024 * 512
struct httpchunk
{
   char   *data;
   size_t cur;
   size_t length;
};
size_t send_data(void *ptr, size_t size, size_t nmemb, void *dataPtr)
{
   size_t           nreadbytes = size * nmemb;
   struct httpchunk *data      = (struct httpchunk *)dataPtr;
   if (nreadbytes > (data->length - data->cur))
   {
      nreadbytes = (data->length - data->cur);
   }
   memcpy(ptr, &(data->data[data->cur]), nreadbytes);
   data->cur += nreadbytes;
   return nreadbytes;
}
void trimTrailingChars(char *url)
{
   if (url == NULL)
   {
      return;
   }
   int length = strlen(url);
   length--;
   while (length > 0)
   {
      if ((*(url + length) == ' ') || (*(url + length) == '/'))
      {
         *(url + length) = '\0';
      }
      else
      {
         break;
      }
      length--;
   }
}

unsigned long getWebDAVError(unsigned long errorCode)
{
   if(CURLE_OK != errorCode)
      return(getError(errorCode));
   else
      return ERROR_DATA_SEND; //TODO: Report error sub class
}

//Dummy function to skip displaying response
size_t recv_data(__attribute__((unused)) void *ptr, size_t size, size_t nmemb, __attribute__((unused)) void *dataPtr)
{
   return size * nmemb;
}


//WEBDAV Implementation Limitations/TODO - error code handling, timeout need to be adjusted, http response parsing

/**
 * Recusively get the list of files in a specified directory  - Internal function
 * @name getDAVFileList
 * @function
 * @param const char *dir - input directory path (IN parameter)
 * @param  char **strs - Two dimensional array of strings to store list of files. (OUT parameter)
 * @param  unsigned long *totalsize - total size of directory (OUT parameter)
 * @param  int *filecount - number of files in input directory (OUT parameter)
 * @return void
 */
void getDAVFileList(const char *dir, char **strs, unsigned long *totalsize, int *filecount)
{
   DIR           *dirPtr;
   struct dirent *dirElement;
   struct stat   fsEntryDetail;
   if ((dirPtr = opendir(dir)) == NULL)
   {
      fprintf(stderr, "cannot open directory: %s\n", dir);
      return;
   }
   while ((dirElement = readdir(dirPtr)) != NULL)
   {
      char tempfsEntry[PATH_MAX] = { 0 };
      strcpy(tempfsEntry, dir);
      strcat(tempfsEntry, "/");
      strcat(tempfsEntry, dirElement->d_name);
      lstat(tempfsEntry, &fsEntryDetail);
      if (S_ISDIR(fsEntryDetail.st_mode))
      {
         if ((strcmp(".", dirElement->d_name) == 0) ||
             (strcmp("..", dirElement->d_name) == 0))
         {
            continue;
         }
         if (*filecount >= MAX_WEBDAV_FILES)
         {
            closedir(dirPtr);
            return;
         }
         if (*totalsize > MAX_WEBDAV_SIZE)
         {
            closedir(dirPtr);
            return;
         }
         strs[*filecount] = (char *)malloc(PATH_MAX);
         strcpy(strs[*filecount], tempfsEntry);
         // (*totalsize) += fsEntryDetail.st_size;
         (*filecount)++;
         getDAVFileList(tempfsEntry, strs, totalsize, filecount);
      }
      else
      {
         if (*filecount >= MAX_WEBDAV_FILES)
         {
            closedir(dirPtr);
            return;
         }
         if (*totalsize > MAX_WEBDAV_SIZE)
         {
            closedir(dirPtr);
            return;
         }
         strs[*filecount] = (char *)malloc(PATH_MAX);
         strcpy(strs[*filecount], tempfsEntry);
         (*totalsize) += fsEntryDetail.st_size;
         (*filecount)++;
      }
   }
   closedir(dirPtr);
}


/**
 * callback to Read the local file, to be used by curl  - Internal function
 * @name http_read
 * @function
 * @param void *ptr - Pointer to a block of memory with a size of at least (size*readCount) bytes
 * @param size_t size - Chunk size
 * @param size_t readCount - Read count
 * @param void *stream - FILE pointer
 * @return int retcode - return value of read operation
 */
static size_t http_read(void *ptr, size_t size, size_t readCount, void *stream)
{
   return(fread(ptr, size, readCount, (FILE *)stream));
}


/**
 * Interface to upload a file on remote http/webdav server
 *
 * @name webDAVUpload
 * @function
 * @param   const char *username - WEBDAV server username, "anonymous" in case of anonymous logon
 * @param   const char *password - WEBDAV server password, "anonymous" in case of anonymous logon
 * @param   const char *destination - http server location to upload the file
 * @param   int port - http server listen port usually 80
 * @param   const char *filePath - path to local file that will be uploaded to server
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   unsigned long - 0 on success, Non-Zero Error code on failure
 */
unsigned long webDAVUpload(const char *username, const char *password, const char *destination, int port, const char *filePath, int ServerType, const char *proxyserver, int isDebugMode, int timeout,APP_TYPE apptype)
{
   CURL        *curl = NULL;
   CURLcode    res;
   FILE        *fileptr      = NULL;
   curl_off_t  localFileSize = 0;
   struct stat structLocalFileInfo;
   int         isSSLEnabled = 0;
   if (checkBlankorNull(filePath))
   {
      printf("\nInvalid Local file Path");
      return(ERROR_CONFIG);
   }
   if (checkBlankorNull(destination))
   {
      printf("\nInvalid WebDAV destination");
      return(ERROR_CONFIG);
   }
   curl = curl_easy_init();
   if (curl)
   {
      char strUsernamePassword[256] = { 0 };
      char strRemoteFileURL[1024]   = { 0 };
      char strUploadFileName[256]   = { 0 };
      char strURL[1024]             = { 0 };
      //For Reference
      //char scope_interface[256];
      //int  isIPv6 = 0;
      struct curl_slist *post = NULL;
      if (strncasecmp(destination, "https://", strlen("https://")) == 0)
      {
         strcpy(strURL, destination);
         isSSLEnabled = 1;
      }
      else if (strncasecmp(destination, "http://", strlen("http://")) == 0)
      {
         strcpy(strURL, destination);
         //isSSLEnabled=0;
      }
      else
      {
         snprintf(strURL, sizeof(strURL), "http://%s", destination);
      }
      trimTrailingChars(strURL);
      snprintf(strUsernamePassword, sizeof(strUsernamePassword), "%s:%s", username ? username : "", password ? password : "");
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
      curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
#ifdef FORREFERENCE
      if (isIPv6)
      {
         curl_easy_setopt(curl, CURLOPT_ADDRESS_SCOPE, if_nametoindex(scope_interface));
      }
#endif
      if (!isDirectory(filePath))
      {
         //printf("Copy file %s\n", filePath);
         // printf("tmpdir %s\n", tmpdir);
         localFileSize = (curl_off_t)structLocalFileInfo.st_size;
         if (localFileSize > MAX_WEBDAV_SIZE)
         {
            printf("\n[WebDAV Upload]File size beyond allowed limit");
            curl_easy_cleanup(curl);
            return(ERROR_BEYOND_ALLOWED_LIMIT);
         }
         if (localFileSize == 0)
         {
            printf("\n[WebDAV Upload]Empty File");
            curl_easy_cleanup(curl);
            return(ERROR_CONFIG);
         }
         fileptr = fopen(filePath, "rb");
         if (fileptr == NULL)
         {
            curl_easy_cleanup(curl);
            return(ERROR_CONFIG);
         }
         post = NULL;
         post = curl_slist_append(post, "Expect:");
         post = curl_slist_append(post, "Connection: keep-alive");
         post = curl_slist_append(post, "User-Agent: Sindoh Printer");
         strcpy(strUploadFileName, basename((char *)filePath));
         snprintf(strRemoteFileURL, sizeof(strRemoteFileURL), "%s/%s", strURL, strUploadFileName);
         if (port > 0)
         {
            curl_easy_setopt(curl, CURLOPT_PORT, port);
         }
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, post);
         curl_easy_setopt(curl, CURLOPT_READFUNCTION, http_read);
         char strEncodedURL[MAX_URL_LENGTH];
         memset(strEncodedURL,0,MAX_URL_LENGTH);
         solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
         curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
         curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
         printf("strRemoteFileURL%s\n", strRemoteFileURL);
         curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
         curl_easy_setopt(curl, CURLOPT_READDATA, fileptr);
         curl_easy_setopt(curl, CURLOPT_PUT, 1L);
         curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)localFileSize);
         if (isSSLEnabled)
         {
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
         }
         //Curl time out
         // curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS,localFileSize>WEBDAV_TIMEOUT?localFileSize:WEBDAV_TIMEOUT);
         curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
         /* ServerType = 1 - proxy , 0 - normal http server*/
         if (ServerType == 1)
         {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
         }
         res = curl_easy_perform(curl);
         curl_slist_free_all(post);
         post = NULL;
         long response = 200;
         curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
         if ((CURLE_OK != res) || (response / 100 != 2))
         {
            if (fileptr)
            {
               fclose(fileptr);
               fileptr = NULL;
            }
            curl_easy_cleanup(curl);
            printf("Upload failed curl return code[%d:%s] HTTP Code [%ld]\n", res, curl_easy_strerror(res), response);
            return(getWebDAVError(res));
         }
         curl_easy_cleanup(curl);
         if (fileptr)
         {
            fclose(fileptr);
            fileptr = NULL;
         }
         fileptr = NULL;
      }
      else
      {
         char *fileList[MAX_WEBDAV_FILES] = { 0 };
         if (isDebugMode)
         {
            printf("Directory scan of %s\n", filePath);
         }
         int           filecount = 1;
         unsigned long totalsize = 0;
         int           k         = 0;
         fileList[0] = (char *)malloc(PATH_MAX);
         realpath(filePath, fileList[0]);
         // (*totalsize) += fsEntryDetail.st_size;
         //(*filecount)++;
         getDAVFileList(filePath, fileList, &totalsize, &filecount);
         if ((totalsize > MAX_WEBDAV_SIZE) || (totalsize == 0))
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
               printf("[WebDAV Upload]Empty Directory/Files (totalsize = 0)\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("[WebDAV Upload] totalsize > MAX_WEBDAV_SIZE\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         if ((filecount >= MAX_WEBDAV_FILES) || (filecount == 0))
         {
            printf("filecount > MAX_WEBDAV_FILES\n");
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
            //curl_slist_free_all(post);
            curl_easy_cleanup(curl);
            if (filecount == 0)
            {
               printf("[WebDAV Upload]Empty Directory/Files (filecount = 0)\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("[WebDAV Upload] filecount > MAX_WEBDAV_FILES\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         k = strlen(filePath);
         char tFilePath[1024];
         strncpy(tFilePath, filePath, sizeof(tFilePath) - 1);
         tFilePath[sizeof(tFilePath) - 1] = '\0';
         while (tFilePath[k - 1] == '/')
         {
            tFilePath[k - 1] = '\0';
            k--;
         }
         k = 0;
         int fileLength = strlen(tFilePath);
         shiftEndfile(fileList, filecount);
         while (k < filecount)
         {
            char *tempStr = NULL;
            // Skip empty file names..Related to Image Log upload file selection feature but applicable to other upload scenarios
            if(checkBlankorNull(fileList[k]))
            {
               k++;
               continue;
            }
            tempStr = strstr(fileList[k], tFilePath);
            //printf("[%d]Copy file %s\n", k, tempStr + strlen(filePath));
            //printf("tmpdir %s\n", tmpdir);
            if (tempStr)
            {
               if (apptype == APP_IMAGE_LOG)
               {
                  char *tmpdir = basename(tFilePath);
                  if (tmpdir)
                  {
                     snprintf(strRemoteFileURL, sizeof(strRemoteFileURL), "%s/%s/%s", strURL, tmpdir, tempStr + fileLength);
                  }
                  else
                  {
                     snprintf(strRemoteFileURL, sizeof(strRemoteFileURL), "%s/%s", strURL, tempStr + fileLength);
                  }
               }
               else
               {
                  if (*(tempStr + fileLength) == '\0')
                  {
                     k++;
                     continue;
                  }
                  else
                  {
                     snprintf(strRemoteFileURL, sizeof(strRemoteFileURL), "%s/%s", strURL, tempStr + fileLength);
                  }
               }
            }
            else
            {
               k++;
               continue;
            }
            //printf("strRemoteFileURL %s\n", strRemoteFileURL);
            if (port > 0)
            {
               curl_easy_setopt(curl, CURLOPT_PORT, port);
            }
            char strEncodedURL[MAX_URL_LENGTH];
            memset(strEncodedURL,0,MAX_URL_LENGTH);
            solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
            curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
            curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
            printf("fileList[%d] %s\n", k, fileList[k]);
            if (isDirectory(fileList[k]))
            {
               //printf("Create Directory\n");
               curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "MKCOL");
               post = NULL;
               post = curl_slist_append(post, "Expect:");
               post = curl_slist_append(post, "Connection: keep-alive");
               post = curl_slist_append(post, "User-Agent: Sindoh Printer");
               post = curl_slist_append(post, "Content-Length: 0");
               curl_easy_setopt(curl, CURLOPT_READDATA, NULL);
               curl_easy_setopt(curl, CURLOPT_HTTPHEADER, post);
               curl_easy_setopt(curl, CURLOPT_WRITEHEADER, NULL);
               curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);
               curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
               curl_easy_setopt(curl, CURLOPT_UPLOAD, 0);
               curl_easy_setopt(curl, CURLOPT_PUT, 0);
               curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, 0);
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
               //curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS,WEBDAV_TIMEOUT);
               curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
               if (isSSLEnabled)
               {
                  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
               }
            }
            else
            {
               printf("Upload File fileList[%d] = %s\n", k, fileList[k]);
               localFileSize = getSize(fileList[k]);
               fileptr       = fopen(fileList[k], "rb");
               printf("[%d]Copy file %s\n", k, fileList[k]);
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
               post = NULL;
               post = curl_slist_append(post, "Content-Type: application/octet-stream");
               post = curl_slist_append(post, "Expect:");
               post = curl_slist_append(post, "Connection: keep-alive");
               post = curl_slist_append(post, "User-Agent: Sindoh Printer");
               curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
               curl_easy_setopt(curl, CURLOPT_HTTPHEADER, post);
               curl_easy_setopt(curl, CURLOPT_READFUNCTION, http_read);
               curl_easy_setopt(curl, CURLOPT_PUT, 1);
               curl_easy_setopt(curl, CURLOPT_READDATA, fileptr);
               curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
               curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)localFileSize);
               //curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS,localFileSize>WEBDAV_TIMEOUT?localFileSize:WEBDAV_TIMEOUT);
               curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
               if (isSSLEnabled)
               {
                  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
                  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
               }
            }
            /* ServerType = 1 - proxy , 0 - normal http server*/
            if (ServerType == 1)
            {
               //Need to be checked
               curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
            }
            res = curl_easy_perform(curl);
            curl_slist_free_all(post);
            long response = 200;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
            if ((CURLE_OK != res) || (response / 100 != 2))
            {
               if (fileptr)
               {
                  fclose(fileptr);
                  fileptr = NULL;
               }
               curl_easy_cleanup(curl);
               printf("Upload failed curl return code[%d:%s] HTTP Code [%ld]\n", res, curl_easy_strerror(res), response);
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
               return(getWebDAVError(res));
            }
            if (!isDirectory(fileList[k]) && fileptr)
            {
               fclose(fileptr);
               fileptr = NULL;
            }
            k++;
            // printf("\nNext File [%s]", fileList[k]);
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
         //curl_slist_free_all(post);
         curl_easy_cleanup(curl);
      }
   }
   else
   {
      printf("\nWEBDAV Upload failed");
      return(ERROR_CONFIG);
   }
   if (isDebugMode)
   {
      printf("\n WEBDAV Upload Successfull");
   }
   return(CURLE_OK);
}


//HTTP Response code check ignored currently
unsigned long webDAVConnTest(const char *username, const char *password, const char *destination, int port, int ServerType, const char *proxyserver, int isDebugMode, int timeout)
{
   CURL     *curl = NULL;
   CURLcode res;
   int      isSSLEnabled = 0;
   if (checkBlankorNull(destination))
   {
      return(ERROR_CONFIG);
   }
   curl = curl_easy_init();
   if (curl)
   {
      char              strUsernamePassword[256] = { 0 };
      char              strRemoteFileURL[1024]   = { 0 };
      char              strURL[1024]             = { 0 };
      char              temp[256] = { 0 };
      struct curl_slist *post     = NULL;
      if (strncasecmp(destination, "https://", strlen("https://")) == 0)
      {
         strcpy(strURL, destination);
         isSSLEnabled = 1;
      }
      else if (strncasecmp(destination, "http://", strlen("http://")) == 0)
      {
         strcpy(strURL, destination);
      }
      else
      {
         snprintf(strURL, sizeof(strURL), "http://%s", destination);
      }
      snprintf(strUsernamePassword, sizeof(strUsernamePassword), "%s:%s", username ? username : "", password ? password : "");
      if (isDebugMode)
      {
         curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      }
      curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_WHATEVER);
      curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
      post = NULL;
      post = curl_slist_append(post, "Expect:");
      post = curl_slist_append(post, "Connection: keep-alive");
      post = curl_slist_append(post, "User-Agent: Sindoh Printer");
      post = curl_slist_append(post, "Depth: 0");
      snprintf(strRemoteFileURL, sizeof(strRemoteFileURL), "%s/", strURL);
      if (port > 0)
      {
         curl_easy_setopt(curl, CURLOPT_PORT, port);
      }
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PROPFIND");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, post);
      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
      char strEncodedURL[MAX_URL_LENGTH];
      memset(strEncodedURL,0,MAX_URL_LENGTH);
      solutionURLEncode(curl, strRemoteFileURL, strEncodedURL, MAX_URL_LENGTH);
      curl_easy_setopt(curl, CURLOPT_URL, strEncodedURL);
      curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
      if (isSSLEnabled)
      {
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      }
      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
      /* ServerType = 1 - proxy , 0 - normal http server*/
      if (ServerType == 1)
      {
         curl_easy_setopt(curl, CURLOPT_PROXY, proxyserver);
      }
      struct httpchunk httprequest  = { 0 };
      struct httpchunk httpresponse = { 0 };
      httprequest.data   = "<?xml version=\"1.0\" encoding=\"utf-8\"?><D:propfind xmlns:D=\"DAV:\"><D:prop><D:allprop/></D:prop></D:propfind>";
      httprequest.length = strlen(httprequest.data);
      httprequest.cur    = 0;
      curl_easy_setopt(curl, CURLOPT_READFUNCTION, send_data);
      /* coverity[bad_sizeof] */
      curl_easy_setopt(curl, CURLOPT_READDATA, (void *)&httprequest);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recv_data);
      /* coverity[bad_sizeof] */
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&httpresponse);
      post = curl_slist_append(post, "Transfer-Encoding: ");
      snprintf(temp, sizeof(temp), "Content-Length: %u", httprequest.length);
      post = curl_slist_append(post, temp);
      post = curl_slist_append(post, "Content-Type: application/xml");
      res  = curl_easy_perform(curl);
      curl_slist_free_all(post);
      post = NULL;
      long response = 200;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response);
      if ((CURLE_OK != res) || (response / 100 != 2))
      {
         curl_easy_cleanup(curl);
         printf("\nWebDAV Connection Test Failed");
         return(getWebDAVError(res));
      }
      curl_easy_cleanup(curl);
   }
   else
   {
      return(ERROR_CONFIG);
   }
   printf("\nWebDAV Connection Test OK");
   return(CURLE_OK);
}
