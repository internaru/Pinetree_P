#include "commonutils.h"
#include <curl/curl.h>
#define START_MIME_TAG    "=?UTF-8?B?"
#define END_MIME_TAG      "?="

/**
 * Check whether the specified string is direcory or not - Internal function
 * @name isDirectory
 * @function
 * @param const char *dirPath - absolute path of directory (IN parameter)
 * @return int -> 1 - dirPath is directory, 0 - dirPath is not a directory
 */
int isDirectory(const char *dirPath)
{
   struct stat fsEntryDetail;
   if(dirPath == NULL)
   {
      return 0;
   }
   memset(&fsEntryDetail, 0, sizeof(struct stat));
   if (stat(dirPath, &fsEntryDetail) == 0)
   {
      if (S_ISDIR(fsEntryDetail.st_mode))
      {
         return(1);
      }
      else
      {
         return(0);
      }
   }
   return(0);
}


/**
 * Get the file size in bytes - Internal function
 * @function
 * @name getSize
 * @function
 * @param const char *filePath - location of file (IN parameter)
 * @return off_t - file size in bytes
 */
off_t getSize(const char *filePath)
{
   struct stat fsEntryDetail;
   if(filePath == NULL)
   {
      return 0;
   }
   memset(&fsEntryDetail, 0, sizeof(struct stat));
   if (stat(filePath, &fsEntryDetail) == 0)
   {
      return(fsEntryDetail.st_size);
   }
   return(0);
}


/**
 * Check whether the specified string is direcory or not - Internal function
 * @name file_exist
 * @function
 * @param const char *filename - name of the file with complete path (IN parameter)
 * @return int -> 1 - file exists, 0 - file does not exist
 */
int file_exist(const char *filename)
{
   struct stat fsEntryDetail;
   if(filename == NULL)
   {
      return 0;
   }
   memset(&fsEntryDetail, 0, sizeof(struct stat));
   if (stat(filename, &fsEntryDetail) == 0)
   {
      return(1);
   }
   return(0);
}


/**
 * Execute the system command
 * @name runCommand
 * @function
 * @param const char *command- command string
 * @return int -> 0 - success, fail - 1
 */
int runCommand(const char *command)
{
   int ret = 0;

   ret = system(command);
   if (WEXITSTATUS(ret) != 0)
   {
      return(1);
   }
   return(0);
}


//Message Send API START

int PosixMessageSend(mqd_t msgq, MESSAGE *msg)
{
   int mqret = mq_send(msgq, (char *)msg, sizeof(MESSAGE), MQ_DEFAULT_PRIORITY);
   return mqret;
}


void SendMsgToSystem(mqd_t msgq, MESSAGE *msg)
{
   if (msgq != (mqd_t)-1)
   {
      PosixMessageSend(msgq, msg);
   }
}


bool checkBlankorNull(const char *str)
{
   if (str == NULL)
   {
      return true;
   }

   int i = 0;
   while(str[i])
   {
      if(str[i] == ' ')
      {
         i++;
         continue;
      }
      else
      {
         return false;
      }
   }

   return true;
}


error_type_t sendMSGToSysManager(stSend_SendResult *jobResult, UINT16 command, UINT32 sender)
{
   SYS_UI_MESSAGE msg;

   memset(&msg, 0, sizeof(SYS_UI_MESSAGE));
   msg.msgSender = sender;
   msg.cmd       = command;
   memcpy(msg.data, jobResult, sizeof(stSend_SendResult));
   error_type_t err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msg);
   // XASSERT( err==OK, err );
   return err;
}

//Added handling for every curl code even if it is not related to solution supported protocol
UINT32 getError(int errorCode)
{
   UINT32 err = ERROR_FAILED;

   switch (errorCode)
   {
   case CURLE_UNSUPPORTED_PROTOCOL:
      err = ERROR_CONNECT;
      break;

   case CURLE_FAILED_INIT:
      err = ERROR_CONNECT;
      break;

   case CURLE_URL_MALFORMAT:
      err = ERROR_CONNECT;
      break;

   case CURLE_NOT_BUILT_IN:
      err = ERROR_CONNECT;
      break;

   case CURLE_COULDNT_RESOLVE_PROXY:
      err = ERROR_CONNECT;
      break;

   case CURLE_COULDNT_RESOLVE_HOST:
      err = ERROR_CONNECT;
      break;

   case CURLE_COULDNT_CONNECT:
      err = ERROR_CONNECT;
      break;

   case CURLE_FTP_WEIRD_SERVER_REPLY:
      err = ERROR_CONNECT;
      break;

   case CURLE_REMOTE_ACCESS_DENIED:
      err = ERROR_AUTH;
      break;

   case CURLE_FTP_ACCEPT_FAILED:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_WEIRD_PASS_REPLY:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_ACCEPT_TIMEOUT:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_WEIRD_PASV_REPLY:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_WEIRD_227_FORMAT:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_CANT_GET_HOST:
      err = ERROR_CONNECT;
      break;

   case CURLE_OBSOLETE16:
      err = ERROR_FAILED;
      break;

   case CURLE_FTP_COULDNT_SET_TYPE:
      err = ERROR_CONNECT;
      break;

   case CURLE_PARTIAL_FILE:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_COULDNT_RETR_FILE:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_OBSOLETE20:
      err = ERROR_FAILED;
      break;

   case CURLE_QUOTE_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_HTTP_RETURNED_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_WRITE_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_OBSOLETE24:
      err = ERROR_FAILED;
      break;

   case CURLE_UPLOAD_FAILED:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_READ_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_OUT_OF_MEMORY:
      err = ERROR_FAILED;
      break;

   case CURLE_OPERATION_TIMEDOUT:
      err = ERROR_FAILED;
      break;

   case CURLE_OBSOLETE29:
      err = ERROR_FAILED;
      break;

   case CURLE_FTP_PORT_FAILED:
      err = ERROR_CONNECT;
      break;

   case CURLE_FTP_COULDNT_USE_REST:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_OBSOLETE32:
      err = ERROR_FAILED;
      break;

   case CURLE_RANGE_ERROR:
      err = ERROR_FAILED;
      break;

   case CURLE_HTTP_POST_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_SSL_CONNECT_ERROR:
      err = ERROR_CONNECT;
      break;

   case CURLE_BAD_DOWNLOAD_RESUME:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FILE_COULDNT_READ_FILE:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_OBSOLETE40:
      err = ERROR_FAILED;
      break;

   case CURLE_FUNCTION_NOT_FOUND:
      err = ERROR_FAILED;
      break;

   case CURLE_ABORTED_BY_CALLBACK:
      err = ERROR_FAILED;
      break;

   case CURLE_BAD_FUNCTION_ARGUMENT:
      err = ERROR_FAILED;
      break;

   case CURLE_OBSOLETE44:
      err = ERROR_FAILED;
      break;

   case CURLE_INTERFACE_FAILED:
      err = ERROR_CONNECT;
      break;

   case CURLE_OBSOLETE46:
      err = ERROR_FAILED;
      break;

   case CURLE_TOO_MANY_REDIRECTS:
      err = ERROR_CONNECT;
      break;

   case CURLE_UNKNOWN_OPTION:
      err = ERROR_CONFIG;
      break;

   case CURLE_TELNET_OPTION_SYNTAX:
      err = ERROR_CONNECT;
      break;

   case CURLE_OBSOLETE50:
      err = ERROR_FAILED;
      break;

   case CURLE_PEER_FAILED_VERIFICATION:
      err = ERROR_CONNECT;
      break;

   case CURLE_GOT_NOTHING:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_ENGINE_NOTFOUND:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_ENGINE_SETFAILED:
      err = ERROR_CONNECT;
      break;

   case CURLE_SEND_ERROR:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_RECV_ERROR:
      err = ERROR_FAILED;
      break;

   case CURLE_OBSOLETE57:
      err = ERROR_FAILED;
      break;

   case CURLE_SSL_CERTPROBLEM:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_CIPHER:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_CACERT:
      err = ERROR_CONNECT;
      break;

   case CURLE_BAD_CONTENT_ENCODING:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_LDAP_INVALID_URL:
      err = ERROR_CONFIG;
      break;

   case CURLE_FILESIZE_EXCEEDED:
      err = ERROR_BEYOND_ALLOWED_LIMIT;
      break;

   case CURLE_USE_SSL_FAILED:
      err = ERROR_CONNECT;
      break;

   case CURLE_SEND_FAIL_REWIND:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_SSL_ENGINE_INITFAILED:
      err = ERROR_CONNECT;
      break;

   case CURLE_LOGIN_DENIED:
      err = ERROR_AUTH;
      break;

   case CURLE_TFTP_NOTFOUND:
      err = ERROR_CONFIG;
      break;

   case CURLE_TFTP_PERM:
      err = ERROR_AUTH;
      break;

   case CURLE_REMOTE_DISK_FULL:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_TFTP_ILLEGAL:
      err = ERROR_FAILED;
      break;

   case CURLE_TFTP_UNKNOWNID:
      err = ERROR_FAILED;
      break;

   case CURLE_REMOTE_FILE_EXISTS:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_TFTP_NOSUCHUSER:
      err = ERROR_AUTH;
      break;

   case CURLE_CONV_FAILED:
      err = ERROR_FAILED;
      break;

   case CURLE_CONV_REQD:
      err = ERROR_FAILED;
      break;

   case CURLE_SSL_CACERT_BADFILE:
      err = ERROR_CONNECT;
      break;

   case CURLE_REMOTE_FILE_NOT_FOUND:
      err = ERROR_FAILED;
      break;

   case CURLE_SSH:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_SHUTDOWN_FAILED:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_AGAIN:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_SSL_CRL_BADFILE:
      err = ERROR_CONNECT;
      break;

   case CURLE_SSL_ISSUER_ERROR:
      err = ERROR_CONNECT;
      break;

   case CURLE_FTP_PRET_FAILED:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_FTP_BAD_FILE_LIST:
      err = ERROR_DATA_SEND;
      break;

   case CURLE_CHUNK_FAILED:
      err = ERROR_DATA_SEND;
      break;
/* //only available in 7.36.0
   case CURLE_NO_CONNECTION_AVAILABLE:
      err = ERROR_CONNECT;
      break;
*/
   default:
      err = ERROR_FAILED;
   }

   return(err);
}


unsigned long getBase64Size(unsigned long value)
{
   //Encoded size + CRLF count (after 64 char) + padded chars
   return(((4 * value) / 3) + (((4 * value)) / (3 * 64)) + (value % 3));
}
bool checkOK(unsigned long value)
{
   struct statvfs fsEntry;

   if (statvfs("/tmp", &fsEntry))
   {
      return false;
   }
   return(fsEntry.f_bavail > ((value / fsEntry.f_bsize) + 1));
}

bool checkBase64OK(unsigned long value)
{
   struct statvfs fsEntry;
   if (statvfs("/tmp", &fsEntry))
   {
      return false;
   }
   //printf("\nAvailable size = %lu", (unsigned long )(fsEntry.f_bavail * fsEntry.f_bsize));
   //printf("\nRequired size = %lu\n", getBase64Size(value));
   return((unsigned long long)fsEntry.f_bavail > ((getBase64Size(value)) / fsEntry.f_bsize) + 1);
}
int encodeFPBase64(FILE *inFile, FILE *outFile)
{
   static const char _code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/";
   unsigned char in[3], out[4];
   int           l = 0;
   int           c = 0;
   int           p = 0;
   int           i = 0;
   if (!inFile || !outFile)
   {
      return 0;
   }
   while ((c = fgetc(inFile)) != EOF)
   {
      p     = 0;
      in[0] = c;
      for (i = 1; i < 3; i++)
      {
         if ((c = fgetc(inFile)) == EOF)
         {
            c = 0;
            p++;
         }
         in[i] = c;
      }

      out[0] = _code[in[0] >> 2];
      out[1] = _code[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
      out[2] = (p < 2) ? _code[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : '=';
      out[3] = (p < 1) ? _code[in[2] & 0x3f] : '=';
      for (i = 0; i < 4; i++)
      {
         fputc(out[i], outFile);
      }
      l++;
      if (l % 16 == 0)
      {
         fputc('\r', outFile);
         fputc('\n', outFile);
      }
   }
   return l * 4;
}


int encodeCharBase64(const char *inStr, FILE *outFile, bool isABNF)
{
   static const char _code[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/";
   unsigned char in[3], out[4];
   bool          last = false;
   int           l    = 0;
   int           c    = 0;
   int           p    = 0;
   int           i    = 0;

   if (!inStr || !outFile)
   {
      return 0;
   }
   if (isABNF)
   {
      fputs(START_MIME_TAG, outFile);
   }
   char *curchar = (char *)inStr;
   while (*curchar)
   {
      last  = false;
      c     = *curchar;
      p     = 0;
      in[0] = c;
      for (i = 1; i < 3; i++)
      {
         if (*curchar)
         {
            curchar++;
         }

         if (*curchar == '\0')
         {
            c = 0;
            p++;
         }
         else
         {
            c = *curchar;
         }
         in[i] = c;
      }

      out[0] = _code[in[0] >> 2];
      out[1] = _code[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
      out[2] = (p < 2) ? _code[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : '=';
      out[3] = (p < 1) ? _code[in[2] & 0x3f] : '=';
      for (i = 0; i < 4; i++)
      {
         fputc(out[i], outFile);
      }
      l++;
      if (l % 16 == 0)
      {
         if (isABNF)
         {
            fputs(END_MIME_TAG, outFile);
         }
         if (*curchar && *(curchar + 1))
         {
            fputc('\r', outFile);
            fputc('\n', outFile);
         }
         if (isABNF && *curchar && *(curchar + 1))
         {
            fputc('\t', outFile);
            fputs(START_MIME_TAG, outFile);
         }
         else
         {
            last = true;
         }
      }
      if (*curchar)
      {
         curchar++;
      }
      else
      {
         break;
      }
   }
   if (!last && isABNF)
   {
      fputs(END_MIME_TAG, outFile);
   }
   return l * 4;
}

void removeFile(char *filename)
{
   if (filename == NULL)
   {
      printf("\nInvalid file name");
      return;
   }
   if (file_exist(filename) && !isDirectory(filename))
   {
      if (unlink(filename) != 0)
      {
         printf("\nUnable to remove file %s", filename);
      }
   }
}


bool isDirEmpty(const char *dirLocation)
{
   int ret = 0;
   bool isEmpty = true;
   DIR *dirPtr = NULL;
   struct dirent *dirItem = NULL;
   struct dirent *result = NULL;
   if(dirLocation == NULL)
   {
     printf("Directory ptr is NULL\n");
     return true;
   }
   if ((dirPtr = opendir(dirLocation)) == NULL)
   {
      printf("Unable to open Directory [%s]\n", dirLocation);
      return true;
   }
   dirItem = (struct dirent *) malloc(sizeof(struct dirent));
   if (dirItem == NULL)
   {
      if (dirPtr)
      {
         closedir(dirPtr);
      }
      printf("No memory Available..Traversal aborted[%s]\n", dirLocation);
      return true;
   }
   while (true)
   {
      ret = readdir_r(dirPtr, dirItem, &result);
      if (ret != 0 || result == NULL)
      {
         isEmpty = true;
         printf("Empty Directory [%s]\n", dirLocation);
         break;
      }
      if (strcmp(dirItem->d_name, ".") != 0 && strcmp(dirItem->d_name, "..") != 0)
      {
         isEmpty = false;
         break;
      }
   }
   if (dirPtr)
   {
      closedir(dirPtr);
   }
   if (dirItem)
   {
      free(dirItem);
   }
   return isEmpty;
}

bool shiftEndfile(char **fileList, int filecount)
{
   int  k            = 0;
   bool foundEndFile = false;
   int  endFileIndex = -1;

   while (k < filecount)
   {
      int length = strlen(fileList[k]);
      if (length >= 4 && strncasecmp(fileList[k] + length - 4,".end", 4) == 0)
      {
         endFileIndex = k;
         foundEndFile = true;
         break;
      }
      k++;
   }
   if (foundEndFile)
   {
      char endfilePath[PATH_MAX];
      snprintf(endfilePath, sizeof(endfilePath), "%s", fileList[k]);
      while (k < filecount - 1)
      {
         snprintf(fileList[k], PATH_MAX, "%s", fileList[k + 1]);
         k++;
      }
      snprintf(fileList[k], PATH_MAX, "%s", endfilePath);

      return true;
   }
   else
   {
      return false;
   }
}

#if defined(HAVE_POP) || defined(HAVE_IMAP)

int createFolder(const char *folderPath)
{
   char *curDir, *curPosition;
   if ((folderPath == NULL) || (*folderPath == '\0'))
   {
      return(-1);
   }
   curDir = strdup(folderPath);
   if (*curDir != '/')
   {
      free(curDir);
      return(-1);
   }
   curPosition = curDir + 1;
   do
   {
      while (*curPosition && *curPosition != '/')
      {
         curPosition++;
      }
      if (!*curPosition)
      {
         curPosition = NULL;
      }
      else
      {
         *curPosition = '\0';
      }
      if (!file_exist(curDir))
      {
         if (mkdir(curDir, 0755) == -1)
         {
            free(curDir);
            return(-1);
         }
      }
      else if (!isDirectory(curDir))
      {
         free(curDir);
         return(-1);
      }
      if (curPosition)
      {
         *curPosition++ = '/';
         while (*curPosition && *curPosition != '/')
         {
            curPosition++;
         }
      }
   } while (curPosition);
   free(curDir);
   return(0);
}


int base64_decode_tofile(char *start, char *end, FILE *attfile)
{
   // printf("start decode");
   unsigned char in[4], out[3];
   int           p, c, i, l = 0;
   if (!start || !end || !attfile)
   {
      printf("invalid base64_decode args");
      return(-1);
   }
   char *cur = start;
   while (cur <= end && *cur)
   {
      p = 0;
      for (i = 0; i < 4; )
      {
         c = *cur;
         if (c < 0)
         {
            return(l);
         }
         else if (c >= 'a')
         {
            in[i++] = c - 71;
         }
         else if (c >= 'A')
         {
            in[i++] = c - 65;
         }
         else if (c == '=')
         {
            in[i++] = 0, p += 1;
         }
         else if (c >= '0')
         {
            in[i++] = c + 4;
         }
         else if (c == '/')
         {
            in[i++] = 63;
         }
         else if (c == '+')
         {
            in[i++] = 62;
         }
         cur++;
         /* else - what the heck - unexpected input char - just skip for now */
      }
      out[0] = (unsigned char )(in[0] << 2 | in[1] >> 4);
      out[1] = (unsigned char )(in[1] << 4 | in[2] >> 2);
      out[2] = (unsigned char )(((in[2] << 6) & 0xc0) | in[3]);
      for (i = 0; i < (3 - p); i++)
      {
         fputc(out[i], attfile);
      }
      l += i;
   }
   return(l);
}


int extractAttachment(char *emailFile)
{
   FILE *fpEmailFile   = NULL, *fpAttachmentFile = NULL;
   char filename[1024] = { 0 };
   if (emailFile == NULL)
   {
      printf("NULL - Invalid mbox format\n");
      return(-1);
   }
   if ((fpEmailFile = fopen(emailFile, "r")) == NULL)
   {
      printf("mbox file doesnot exist\n");
      return(-1);
   }
   fseek(fpEmailFile, 0, SEEK_END);
   long fsize = ftell(fpEmailFile);
   fseek(fpEmailFile, 0, SEEK_SET);
   char *strEmailFile = (char *)malloc(fsize + 1);
   if(strEmailFile == NULL)
   {
      printf("Cannot allocate memory exiting\n");
	  fclose(fpEmailFile);
      return(-1);
   }
   fread(strEmailFile, fsize, 1, fpEmailFile);
   fclose(fpEmailFile);
   strEmailFile[fsize] = 0;
   //char *strend   = strEmailFile + fsize - 1;
   char *startatt = strstr(strEmailFile, ATTACHMENT_START1);
   if (startatt == NULL)
   {
      printf("No attachment exist\n");
      free(strEmailFile);
      return(-1);
   }
   if (startatt)
   {
      startatt = strstr(startatt, ATTACHMENT_START2);
   }
   while (startatt && *startatt)
   {
      startatt = startatt + strlen(ATTACHMENT_START2);
      while (*startatt == ' ' || *startatt == '\t' || *startatt == '\"')
      {
         startatt++;
      }
      char *end = startatt;
      while (*end != '\n' && *end != '\r' && *end != '\0' && *end != '\"')
      {
         end++;
      }
      strcpy(filename, emailFile);
      strcat(filename, "_");
      strncat(filename, startatt, end - startatt);
      printf("\nAttachment filename for Email file [%s] is [%s]", emailFile, filename);
      if (*end == '\"')
      {
         end++;
      }
      char *attend = strstr(end, ATTACHMENT_END);
      if (attend)
      {
         while (*attend != '\n' && *attend != '\r' && *attend != '\0')
         {
            attend--;
         }
         char *tmp  = NULL;
         char *last = NULL;
         tmp = strstr(end, ":");
         while (tmp && tmp < attend)
         {
            last = tmp;
            tmp  = strstr(tmp + 1, ":");
         }
         if (last)
         {
            while (*last != '\n' && *last != '\r' && last < attend)
            {
               last++;
            }
            end = last + 1;
         }
         if ((fpAttachmentFile = fopen(filename, "wt")) == NULL)
         {
            printf("can't open mail %s file: %s\n", filename, strerror(errno));
            return(-1);
         }
         while (!strchr(VALID_BASE64, *end))
         {
            end++;
         }
         base64_decode_tofile(end, attend, fpAttachmentFile);
         fclose(fpAttachmentFile);
		 //send message ifax
      }
      startatt = strstr(attend, ATTACHMENT_START1);
      if (startatt)
      {
         startatt = strstr(startatt, ATTACHMENT_START2);
      }
   }
   free(strEmailFile);
   return(0);
}

size_t getDataCB(void *contents, size_t size, size_t nmemb, void *userp)
{
   size_t            realsize = size * nmemb;
   struct receiveBuf *mem     = (struct receiveBuf *)userp;
   mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
   if (mem->memory == NULL)
   {
      printf("Cannot allocate memory\n");
      return(0);
   }
   memcpy(&(mem->memory[mem->size]), contents, realsize);
   mem->size += realsize;
   mem->memory[mem->size] = 0;
   return(realsize);
}


size_t getFileCB(void *contents, size_t size, size_t nmemb, void *userp)
{
   return(fwrite(contents, size, nmemb, (FILE *)userp));
}
#endif
