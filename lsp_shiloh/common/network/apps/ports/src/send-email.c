#include "send-email.h"
#include "emailvalidate.h"

#define MAX_ATTACHMENTS                     500
#define EMAIL_ERROR                         50001
#define EMAIL_CONFIG_ERROR                  50002
#define EMAIL_ATTACHMENT_SIZE_EXCEEDED      50003
#define EMAIL_ATTACHMENT_NUMBER_EXCEEDED    50004
#define EMAIL_TO_ADDRESS_INVALID            50005
#define MAX_ATTACHMENT_SIZE                 1024 * 1024 * 100
#define EMAIL_TIMEOUT                       120
#define MBOX_FILECONTENT_SIZE               1024 * 1024
#define EMAIL_BOUNDRY                       "----=_Part_1_1111111111"

/**
 * Recusively get the list of files in a specified directory  - Internal function
 * @name getEmailFileList
 * @function
 * @param const char *dir - input directory path (IN parameter)
 * @param  char **strs - Two dimensional array of strings to store list of files. All strings in strs need to be freed by caller (OUT parameter)
 * @param  unsigned long *totalsize - total size of directory (OUT parameter)
 * @param  int *filecount - number of files in input directory (OUT parameter)
 * @return void
 */
void getEmailFileList(const char *dir, char **strs, unsigned long *totalsize, int *filecount)
{
   DIR           *dp    = NULL;
   struct dirent *entry = NULL;
   struct stat   statbuf;

   if ((dp = opendir(dir)) == NULL)
   {
      printf("cannot open directory: %s\n", dir);
      return;
   }
   while ((entry = readdir(dp)) != NULL)
   {
      char tempfsEntry[PATH_MAX]={0};
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
         getEmailFileList(tempfsEntry, strs, totalsize, filecount);
      }
      else
      {
         if (*filecount >= MAX_ATTACHMENTS)
         {
            closedir(dp);
            return;
         }
         if (*totalsize > MAX_ATTACHMENT_SIZE)
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
 * @name smtp_read
 * @function
 * @param void *ptr - Pointer to a block of memory with a size of at least (size*readCount) bytes
 * @param  size_t size - Chunk size
 * @param  size_t readCount - Read count
 * @param void *stream - FILE pointer
 * @return int retcode - return value of read operation
 */
static size_t smtp_read(void *ptr, size_t size, size_t readCount, void *stream)
{
   return(fread(ptr, size, readCount, (FILE *)stream));
}


/**
 * Interface to send email
 *
 * @name sendEmail
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
 * @return  int - 0 on success
 */
/*unsigned long sendEmail(const char *username, const char *password, const char *sender,
                        const char *mailhost, int port, const char *attachmentPath, const char *toAddress,
                        const char *subject, const char *content, int isDebugMode)*/
unsigned long sendEmail(email_info_message_t *email_message)
{
   //int               secureType           = 1;
   //int               authType             = 1;
   int               count             = 0;
   char              *email_server     = NULL;
   FILE              *fpEmailFile      = NULL;
   FILE              *fpAttachmentFile = NULL;
   char              *attachment_file  = NULL;
   char              emailfile[256]={0};
   struct curl_slist *recipients = NULL;
   CURL              *curl       = NULL;
   CURLcode          res;
   char              strUsernamePassword[256]={0};
   char              strReceipient[2048]={0};
   char              toReceipient[2048]={0};
   bool attachmentExist=false;
   //printf("\n username %s paswsord %s senderEmail %s senderName %s mailhost %s port %d attachmentPath %s toAddress %s  subject %s content %s\n", email_message->username,email_message->password,email_message->senderEmail,email_message->senderName,email_message->mailhost,email_message->port,email_message->attachmentPath,email_message->toAddress,email_message->subject,email_message->content);
   if(email_message == NULL)
   {
	   return (ERROR_CONFIG);
   }

   if (email_message->toAddress == NULL)
   {
      return(ERROR_CONFIG);
   }
   if (email_message->mailhost == NULL)
   {
      return(ERROR_CONFIG);
   }
   if (strcmp(email_message->toAddress, "") == 0)
   {
      return(ERROR_CONFIG);
   }
   if (strcmp(email_message->mailhost, "") == 0)
   {
      return(ERROR_CONFIG);
   }
   curl = curl_easy_init();
   if (curl == NULL)
   {
      return(ERROR_CONFIG);
   }
   //memset(strReceipient, 0, 1024);
   //memset(toReceipient, 0, 1024);
   strcpy(strReceipient, email_message->toAddress);
   if (email_message->username && email_message->password && (strcmp(email_message->username, "") != 0) && (strcmp(email_message->password, "") != 0))
   {
      sprintf(strUsernamePassword, "%s:%s", email_message->username, email_message->password);
      curl_easy_setopt(curl, CURLOPT_USERNAME, email_message->username);
      curl_easy_setopt(curl, CURLOPT_PASSWORD, email_message->password);
      curl_easy_setopt(curl, CURLOPT_USERPWD, strUsernamePassword);
   }
   char *fromEmailAddress = NULL;
   char *fromname         = NULL;
   char tmpEmail[256]={0};
   fromEmailAddress = getValidEmail((char *)email_message->senderEmail);
   if (fromEmailAddress == NULL)
   {
	  printf("\n[Email] Missing from Email address..NULL");
	      return(ERROR_CONFIG);
   }
   else
   {
	   strncpy(email_message->senderEmail, fromEmailAddress, strlen(fromEmailAddress) + 1);
	//   free(fromEmailAddress);
   }
   if (email_message->senderName)
   {
      fromname = calloc(1,strlen(email_message->senderName));
      strcpy(fromname,email_message->senderName);
   }
   if (email_message->mailhost)
   {
      email_server = (char *)malloc(256);
      if ((strncasecmp(email_message->mailhost, "smtp://", strlen("smtp://")) == 0))
      {
         sprintf(email_server, "%s", email_message->mailhost);
      }
	  else if ((strncasecmp(email_message->mailhost, "smtps://", strlen("smtps://")) == 0))
      {
         sprintf(email_server, "%s", email_message->mailhost);
      }
      else
      {
         sprintf(email_server, "smtp://%s", email_message->mailhost);
      }
   }
   if (email_message->port > 0)
   {
      curl_easy_setopt(curl, CURLOPT_PORT, email_message->port);
   }
   curl_easy_setopt(curl, CURLOPT_URL, email_server);
   curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY);
   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, EMAIL_TIMEOUT);   
   curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
   if (email_message->senderEmail)
   {
      curl_easy_setopt(curl, CURLOPT_MAIL_FROM, email_message->senderEmail);
   }
   else if (fromname)
   {
      curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromname);
   }
   else
   {
      printf("\nSender email address does not exist...SMTP server may reject the request");	
	  //Temporary fix to avoid rejection...Incomplete....
	  strcpy(tmpEmail,email_message->username);
	  strcat(tmpEmail,"@");
	  if(isdigit(*email_message->mailhost))
	  {
     	strcat(tmpEmail,"[");
      	strcat(tmpEmail,email_message->mailhost);
   		strcat(tmpEmail,"]");	  			
	  }
	  else
	  {
	      char *tmpDomain=strstr(email_message->mailhost,".");
	  	  if(tmpDomain)
 	         strcat(tmpEmail,strstr(email_message->mailhost,".") + 1);
		  else
		  	 strcat(tmpEmail,email_message->mailhost);
	  }
	  curl_easy_setopt(curl, CURLOPT_MAIL_FROM, tmpEmail);
   }
   curl_easy_setopt(curl, CURLOPT_READFUNCTION, smtp_read);
   curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

   if (email_message->isDebugMode)
   {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   }

   char *saveptr = NULL;
   char *s       = NULL;
   s = strtok_r((char *)strReceipient, ";", &saveptr);

   while (s)
   {
      char *emailaddress = getValidEmail(s);
      if (emailaddress)
      {
         recipients = curl_slist_append(recipients, emailaddress);
         if (count > 0)
         {
            strcat(toReceipient, ",");
         }
         strcat(toReceipient, emailaddress);
         count++;
         free(emailaddress);
         emailaddress = NULL;
      }
      if (email_message->isDebugMode)
      {
         printf("token[%d][%s]", count, s);
      }
      s = strtok_r(NULL, ";", &saveptr);
   }

   if (count == 0)
   {
      printf("Atleast one valid \"To\" address required\n");
      if (email_server)
      {
         free(email_server);
         email_server = NULL;
      }
      /*if (fromEmailAddress)
      {
         free(fromEmailAddress);
         fromEmailAddress = NULL;
      }*/
      if (fromname)
      {
         free(fromname);
         fromname = NULL;
      }
      curl_easy_cleanup(curl);
      return(ERROR_CONFIG);
   }
   curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
   tmpnam(emailfile);
   while (file_exist(emailfile))
   {
      tmpnam(emailfile);
   }
   if ((fpEmailFile = fopen(emailfile, "wt")) == NULL)
   {
      printf("can't open mail file: %s\n", strerror(errno));
      if (email_server)
      {
         free(email_server);
         email_server = NULL;
      }
      if (recipients)
      {
         curl_slist_free_all(recipients);
         recipients = NULL;
      }
     if (fromEmailAddress)
      {
         free(fromEmailAddress);
         fromEmailAddress = NULL;
      }
      if (fromname)
      {
         free(fromname);
         fromname = NULL;
      }
      curl_easy_cleanup(curl);
      return(ERROR_CONFIG);
   }
   //Start writing Email File
   fprintf(fpEmailFile, "To: %s\r\n", toReceipient);   
   fprintf(fpEmailFile, "From: ");
   if (!checkBlankorNull(email_message->senderName))
   {
         encodeCharBase64(email_message->senderName, fpEmailFile, true);
         fprintf(fpEmailFile, " <%s>\r\n", email_message->senderEmail);
   }
   else
   {
      fprintf(fpEmailFile, "%s\r\n", email_message->senderEmail);
   }
   if (fromEmailAddress)
   {
      free(fromEmailAddress);
      fromEmailAddress = NULL;
   }
   if (fromname)
   {
      free(fromname);
      fromname = NULL;
   }
   fprintf(fpEmailFile, "Subject: ");
   if(email_message->subject)
   {
      encodeCharBase64(email_message->subject, fpEmailFile, true);
   }
   else
   {
      fprintf(fpEmailFile, "Sindoh MFP");
   }
   fprintf(fpEmailFile, "\r\nMIME-Version: 1.0\r\n");
   if ((email_message->attachmentPath == NULL) || (strcmp(email_message->attachmentPath, "") == 0))
   {
      fprintf(fpEmailFile, "Content-Type: multipart/alternative; boundary=\"%s\"\r\n\r\n", EMAIL_BOUNDRY);
      fprintf(fpEmailFile, "--%s\r\n", EMAIL_BOUNDRY);
      fprintf(fpEmailFile, "Content-Type: text/plain; charset=\"UTF-8\"\r\n");
      fprintf(fpEmailFile, "Content-Transfer-Encoding: base64\r\n\r\n");
      if (email_message->content)
      {
         encodeCharBase64(email_message->content, fpEmailFile, false);
      }
      fprintf(fpEmailFile, "\r\n--%s--", EMAIL_BOUNDRY);
	  attachmentExist=false;
   }
   else if(!file_exist(email_message->attachmentPath))
   {
      printf("\nFile/Path <%s> does not exist..retrying after sync",email_message->attachmentPath);
      sync();	  
	  if(!file_exist(email_message->attachmentPath))
	  {
         if (fpEmailFile)
         {
            fclose(fpEmailFile);
            fpEmailFile = NULL;
         }
         if (email_server)
         {
            free(email_server);
            email_server = NULL;
         }
         if (recipients)
         {
            curl_slist_free_all(recipients);
            recipients = NULL;
         }
         curl_easy_cleanup(curl);
         removeFile(emailfile);
         return(ERROR_CONFIG);
	  }
	  attachmentExist=true;
   }
   else
   {
   	attachmentExist = true;
   }

   if(attachmentExist)
   {
      if (isDirectory(email_message->attachmentPath))
      {
         char *fileList[MAX_ATTACHMENTS] = {0};
         //printf("Directory scan of %s\n", attachmentPath);
         int           filecount = 0;
         unsigned long totalsize = 0;
   
         int k = 0;
         getEmailFileList(email_message->attachmentPath, fileList, &totalsize, &filecount);
         if ((totalsize > MAX_ATTACHMENT_SIZE) || !checkBase64OK(totalsize + MBOX_FILECONTENT_SIZE) || (totalsize == 0))
         {
            while (k < filecount)
            {
               if (fileList[k])
               {
                  free(fileList[k]);
                  fileList[k] = NULL;
               }
               k++;
            }
            if (fpEmailFile)
            {
               fclose(fpEmailFile);
               fpEmailFile = NULL;
            }
            if (email_server)
            {
               free(email_server);
               email_server = NULL;
            }
            if (recipients)
            {
               curl_slist_free_all(recipients);
               recipients = NULL;
            }
            curl_easy_cleanup(curl);
            removeFile(emailfile);
            if(totalsize == 0)
            {
               printf("Empty Attachment Dir/Files\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("Attachment size beyong allowed\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         if (filecount >= MAX_ATTACHMENTS || filecount == 0)
         {
            while (k < filecount)
            {
               if (fileList[k])
               {
                  free(fileList[k]);
                  fileList[k] = NULL;
               }
               k++;
            }
            if (fpEmailFile)
            {
               fclose(fpEmailFile);
               fpEmailFile = NULL;
            }
            if (email_server)
            {
               free(email_server);
               email_server = NULL;
            }
            if (recipients)
            {
               curl_slist_free_all(recipients);
               recipients = NULL;
            }
            curl_easy_cleanup(curl);
            removeFile(emailfile);
            if(filecount >= MAX_ATTACHMENTS)
            {
                printf("Number of Attachments beyond allowed\n");
                return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
            else
            {
                printf("Empty Directory..Aborting email\n");
                return(ERROR_CONFIG);
            }
	    
         }
   
         fprintf(fpEmailFile, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n", EMAIL_BOUNDRY);
         fprintf(fpEmailFile, "--%s\r\n", EMAIL_BOUNDRY);
         fprintf(fpEmailFile, "Content-Type: text/plain; charset=\"UTF-8\"\r\n");
         fprintf(fpEmailFile, "Content-Transfer-Encoding: base64\r\n\r\n");
         if (email_message->content)
         {
            encodeCharBase64(email_message->content, fpEmailFile, false);
         }
         fprintf(fpEmailFile, "\r\n--%s\r\n", EMAIL_BOUNDRY);
         while (k < filecount)
         {
            attachment_file = basename((char *)fileList[k]);
			if(attachment_file)
			{
			   const char *mimeType = identify_mime(attachment_file);
               fprintf(fpEmailFile, "Content-Type: %s", mimeType?mimeType:"application/octet-stream");
               fprintf(fpEmailFile, "; name=\"");
               encodeCharBase64(attachment_file, fpEmailFile, true);
               fprintf(fpEmailFile, "\"\r\nContent-Transfer-Encoding: base64\r\n");
               fprintf(fpEmailFile, "Content-Disposition: attachment; filename=\"");
               encodeCharBase64(attachment_file, fpEmailFile, true);
               fprintf(fpEmailFile, "\"\r\n\r\n");
               if ((fpAttachmentFile = fopen(fileList[k], "r")) == NULL)
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
                  if (email_server)
                  {
                     free(email_server);
                     email_server = NULL;
                  }
                  if (fpEmailFile)
                  {
                     fclose(fpEmailFile);
                     fpEmailFile = NULL;
                  }
                  if (recipients)
                  {
                     curl_slist_free_all(recipients);
                     recipients = NULL;
                  }
                  curl_easy_cleanup(curl);
                  removeFile(emailfile);
                  return ERROR_CONFIG;
               }
               encodeFPBase64(fpAttachmentFile, fpEmailFile);
               fclose(fpAttachmentFile);
               if(k < filecount-1)
                  fprintf(fpEmailFile, "\r\n--%s\r\n", EMAIL_BOUNDRY);
            }
            free(fileList[k]);
            k++;
         }
         fprintf(fpEmailFile, "\r\n--%s--", EMAIL_BOUNDRY);
      }
      else
      {
         unsigned long filesize = getSize(email_message->attachmentPath);
         if ((filesize > MAX_ATTACHMENT_SIZE) || !checkBase64OK(filesize + MBOX_FILECONTENT_SIZE) || (filesize == 0))
         {
            printf("Attachment size beyond allowed or enough space not available");
            if (email_server)
            {
               free(email_server);
               email_server = NULL;
            }
            if (fpEmailFile)
            {
               fclose(fpEmailFile);
               fpEmailFile = NULL;
            }
            if (recipients)
            {
               curl_slist_free_all(recipients);
               recipients = NULL;
            }
            curl_easy_cleanup(curl);
            removeFile(emailfile);
            if(filesize == 0)
            {
               printf("Empty Attachment File\n");
               return(ERROR_CONFIG);
            }
            else
            {
               printf("Attachment size beyong allowed\n");
               return(ERROR_BEYOND_ALLOWED_LIMIT);
            }
         }
         fprintf(fpEmailFile, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n", EMAIL_BOUNDRY);
         fprintf(fpEmailFile, "--%s\r\n", EMAIL_BOUNDRY);
         fprintf(fpEmailFile, "Content-Type: text/plain; charset=\"UTF-8\"\r\n");
         fprintf(fpEmailFile, "Content-Transfer-Encoding: base64\r\n\r\n");
         if (email_message->content)
         {
            encodeCharBase64(email_message->content, fpEmailFile, false);
         }
         fprintf(fpEmailFile, "\r\n--%s\r\n", EMAIL_BOUNDRY);
         attachment_file = basename((char *)email_message->attachmentPath);
         fprintf(fpEmailFile, "Content-Type: %s", identify_mime(attachment_file));
         fprintf(fpEmailFile, "; name=\"");
         encodeCharBase64(attachment_file, fpEmailFile, true);
         fprintf(fpEmailFile, "\"\r\nContent-Transfer-Encoding: base64\r\n");
         fprintf(fpEmailFile, "Content-Disposition: attachment; filename=\"");
         encodeCharBase64(attachment_file, fpEmailFile, true);
         fprintf(fpEmailFile, "\"\r\n\r\n");
   
         if ((fpAttachmentFile = fopen(email_message->attachmentPath, "r")) == NULL)
         {
            if (email_server)
            {
               free(email_server);
               email_server = NULL;
            }
            if (fpEmailFile)
            {
               fclose(fpEmailFile);
               fpEmailFile = NULL;
            }
            curl_easy_cleanup(curl);
            removeFile(emailfile);
            return(ERROR_CONFIG);
         }
         encodeFPBase64(fpAttachmentFile, fpEmailFile);
         fclose(fpAttachmentFile);
         fprintf(fpEmailFile, "\r\n--%s--", EMAIL_BOUNDRY);
      }
   }
   if (fpEmailFile)
   {
      fclose(fpEmailFile);
      fpEmailFile = NULL;
   }
   if ((fpEmailFile = fopen(emailfile, "r")) == NULL)
   {
      printf("can't open  emailfile %s\n", strerror(errno));
      if (email_server)
      {
         free(email_server);
         email_server = NULL;
      }
      curl_easy_cleanup(curl);
      return(ERROR_CONFIG);
   }
   curl_easy_setopt(curl, CURLOPT_READDATA, fpEmailFile);
   curl_easy_setopt(curl, CURLOPT_INFILESIZE, getSize(emailfile));
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_SESSIONID_CACHE, 0L);
   res = curl_easy_perform(curl);
   if (CURLE_OK != res)
   {
      if (email_server)
      {
         free(email_server);
         email_server = NULL;
      }
      if (fpEmailFile)
      {
         fclose(fpEmailFile);
         fpEmailFile = NULL;
      }
      curl_slist_free_all(recipients);
      curl_easy_cleanup(curl);
      removeFile(emailfile);
      printf("Email sending failed - return code[%d]: %s\n", res, curl_easy_strerror(res));
      return(getError(res));
   }
   else
   {
      printf("Email sent successfully - return code[%d]: %s\n", res, curl_easy_strerror(res));
   }
   curl_slist_free_all(recipients);
   curl_easy_cleanup(curl);
   if (fpEmailFile)
   {
      fclose(fpEmailFile);
   }
   if (email_server)
   {
      free(email_server);
      email_server = NULL;
   }
   removeFile(emailfile);
   return(CURLE_OK);
}


/**
 * identify MIME details of file  - Internal function
 * @name identify_mime
 * @function
 * @param char *filename - name of the file
 * @return char * - string containg mime details of file
 */
const char *identify_mime(char *filename)
{
   const char              *file_suffix=NULL;
   unsigned int            i, filename_length;
   struct mime_list_struct mime_list[] =
   {
      { ".html",    5, "text/html"                                                                 },
      { ".htm",     4, "text/html"                                                                 },
      { ".shtm",    5, "text/html"                                                                 },
      { ".shtml",   6, "text/html"                                                                 },
      { ".css",     4, "text/css"                                                                  },
      { ".js",      3, "application/x-javascript"                                                  },
      { ".ico",     4, "image/x-icon"                                                              },
      { ".gif",     4, "image/gif"                                                                 },
      { ".jpg",     4, "image/jpeg"                                                                },
      { ".jpeg",    5, "image/jpeg"                                                                },
      { ".png",     4, "image/png"                                                                 },
      { ".svg",     4, "image/svg+xml"                                                             },
      { ".txt",     4, "text/plain"                                                                },
      { ".torrent", 8, "application/x-bittorrent"                                                  },
      { ".wav",     4, "audio/x-wav"                                                               },
      { ".mp3",     4, "audio/x-mp3"                                                               },
      { ".mid",     4, "audio/mid"                                                                 },
      { ".m3u",     4, "audio/x-mpegurl"                                                           },
      { ".ogg",     4, "application/ogg"                                                           },
      { ".ram",     4, "audio/x-pn-realaudio"                                                      },
      { ".xml",     4, "text/xml"                                                                  },
      { ".json",    5, "text/json"                                                                 },
      { ".xslt",    5, "application/xml"                                                           },
      { ".xsl",     4, "application/xml"                                                           },
      { ".ra",      3, "audio/x-pn-realaudio"                                                      },
      { ".doc",     4, "application/msword"                                                        },
      { ".exe",     4, "application/octet-stream"                                                  },
      { ".zip",     4, "application/x-zip-compressed"                                              },
      { ".xls",     4, "application/excel"                                                         },
      { ".tgz",     4, "application/x-tar-gz"                                                      },
      { ".tar",     4, "application/x-tar"                                                         },
      { ".bz2",     4, "application/x-bzip2"                                                       },
      { ".gz",      3, "application/x-gunzip"                                                      },
      { ".arj",     4, "application/x-arj-compressed"                                              },
      { ".rar",     4, "application/x-arj-compressed"                                              },
      { ".rtf",     4, "application/rtf"                                                           },
      { ".pdf",     4, "application/pdf"                                                           },
      { ".swf",     4, "application/x-shockwave-flash"                                             },
      { ".mpg",     4, "video/mpeg"                                                                },
      { ".webm",    5, "video/webm"                                                                },
      { ".mpeg",    5, "video/mpeg"                                                                },
      { ".mov",     4, "video/quicktime"                                                           },
      { ".tiff",    5, "image/tiff"                                                                },
      { ".mp4",     4, "video/mp4"                                                                 },
      { ".m4v",     4, "video/x-m4v"                                                               },
      { ".asf",     4, "video/x-ms-asf"                                                            },
      { ".avi",     4, "video/x-msvideo"                                                           },
      { ".bmp",     4, "image/bmp"                                                                 },
      { ".c",       2, "text/x-c"                                                                  },
      { ".h",       2, "text/x-chdr"                                                               },
      { ".cpp",     4, "text/x-c++src"                                                             },
      { ".ttf",     4, "application/x-font-ttf"                                                    },
      { ".docx",    5, "application/vnd.openxmlformats-officedocument.wordprocessingml.document"   },
      { ".docm",    5, "application/vnd.ms-word.document.macroEnabled.12"                          },
      { ".dotx",    5, "application/vnd.openxmlformats-officedocument.wordprocessingml.template"   },
      { ".dotm",    5, "application/vnd.ms-word.template.macroEnabled.12"                          },
      { ".xlsx",    5, "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"         },
      { ".xlsm",    5, "application/vnd.ms-excel.sheet.macroEnabled.12"                            },
      { ".xltx",    5, "application/vnd.openxmlformats-officedocument.spreadsheetml.template"      },
      { ".xltm",    5, "application/vnd.ms-excel.template.macroEnabled.12"                         },
      { ".xlsb",    5, "application/vnd.ms-excel.sheet.binary.macroEnabled.12"                     },
      { ".xlam",    5, "application/vnd.ms-excel.addin.macroEnabled.12"                            },
      { ".pptx",    5, "application/vnd.openxmlformats-officedocument.presentationml.presentation" },
      { ".pptm",    5, "application/vnd.ms-powerpoint.presentation.macroEnabled.12"                },
      { ".ppsx",    5, "application/vnd.openxmlformats-officedocument.presentationml.slideshow"    },
      { ".ppsm",    5, "application/vnd.ms-powerpoint.slideshow.macroEnabled.12"                   },
      { ".potx",    5, "application/vnd.openxmlformats-officedocument.presentationml.template"     },
      { ".potm",    5, "application/vnd.ms-powerpoint.template.macroEnabled.12"                    },
      { ".ppam",    5, "application/vnd.ms-powerpoint.addin.macroEnabled.12"                       },
      { ".sldx",    5, "application/vnd.openxmlformats-officedocument.presentationml.slide"        },
      { ".sldm",    5, "application/vnd.ms-powerpoint.slide.macroEnabled.12"                       },
      { ".one",     4, "application/msonenote"                                                     },
      { ".onetoc2", 8, "application/msonenote"                                                     },
      { ".onetmp",  7, "application/msonenote"                                                     },
      { ".onepkg",  7, "application/msonenote"                                                     },
      { ".thmx",    5, "application/vnd.ms-officetheme"                                            },
      { NULL,       0, NULL                                                                        }
   };

   filename_length = strlen(filename);

   for (i = 0; mime_list[i].file_suffix != NULL; i++)
   {
      file_suffix = filename + (filename_length - mime_list[i].suffix_length);
      if ((filename_length > mime_list[i].suffix_length) &&
          (strcasecmp(file_suffix, mime_list[i].file_suffix) == 0))
      {
         return(mime_list[i].mime_name);
      }
   }

   return("application/octet-stream");
}
