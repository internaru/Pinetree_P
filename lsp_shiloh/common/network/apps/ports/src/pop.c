//Prototype implementation for downloading emails from POP3 server. 
//Post download the program will extract attachments from downloded folder.

//Email files along with attachement will not be deleted..Need to decide when to delete.

#include <pop.h>


#define POP_CONNECT_TIMEOUT    120

int receive_pop3_emails(const char *username, const char *password, const char *mailhost, int port, const char *emailStorePath, int isDebugMode)
{
   struct receiveBuf chunk;
   CURL              *curl = NULL;
   if (mailhost == NULL)
   {
      return(ERROR_CONFIG);
   }
   if (strcmp(mailhost, "") == 0)
   {
      return(ERROR_CONFIG);
   }
   char folderPath[1024];
   if (emailStorePath && (strcmp(username, "") != 0))
   {
      sprintf(folderPath, "/tmp/%s/Emails", emailStorePath);
   }
   else
   {
      sprintf(folderPath, "/tmp/Emails");
   }
   createFolder(folderPath);
   chunk.memory = (char *)malloc(1);
   chunk.size   = 0;
   curl = curl_easy_init();
   FILE *fpEmailFile = NULL;
   if (username && password && (strcmp(username, "") != 0) && (strcmp(password, "") != 0))
   {
      curl_easy_setopt(curl, CURLOPT_USERNAME, username);
      curl_easy_setopt(curl, CURLOPT_PASSWORD, password);
   }
   char email_server[256] = { 0 };
   if (mailhost)
   {
      if ((strncasecmp(mailhost, "pop3://", strlen("pop3://")) == 0))
      {
         sprintf(email_server, "%s", mailhost);
      }
      else if ((strncasecmp(mailhost, "pop3s://", strlen("pop3s://")) == 0))
      {
         sprintf(email_server, "%s", mailhost);
      }
      else
      {
         sprintf(email_server, "pop3s://%s", mailhost);
      }
   }
   if (port > 0)
   {
      curl_easy_setopt(curl, CURLOPT_PORT, port);
   }
   curl_easy_setopt(curl, CURLOPT_URL, email_server);
   curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, POP_CONNECT_TIMEOUT);
   curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
   curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST");
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getDataCB);
   if (isDebugMode)
   {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   }
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
   curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl/1.0");
   CURLcode  res = curl_easy_perform(curl);
   char m[256];
   memset(m, 0, 256);
   int totalmessages = 0;
   if (res != CURLE_OK)
   {
      printf("curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
      if (chunk.memory)
      {
         free(chunk.memory);
      }
      curl_easy_cleanup(curl);
      return(-1);
   }
   else
   {
      if (chunk.size > 2)
      {
         char *end = chunk.memory + chunk.size - 2;
         while (*end != '\n' && end >= chunk.memory)
         {
            end--;
         }
         end++;
         char *cur = end;
         while (isdigit(*cur))
         {
            cur++;
         }
         if (cur > end)
         {
            strncpy(m, end, cur - end);
            free(chunk.memory);
            totalmessages = atoi(m);
         }
         printf("\nTotal Number of Email Messages = %d\n", totalmessages);
         if (totalmessages > 0)
         {
            while (totalmessages > 0)
            {
               char command[1024];
               char fname[1024];
               sprintf(command, "RETR %d", totalmessages);
               sprintf(fname, "%s/Message%d", folderPath, totalmessages);
               curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, command);
               if ((fpEmailFile = fopen(fname, "wt")) == NULL)
               {
                  printf("\nCannot open %s",fname);
                  totalmessages--;
                  continue;
               }
               curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpEmailFile);
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFileCB);
               res = curl_easy_perform(curl);
               if (res != CURLE_OK)
               {
                  fprintf(stderr, "curl_easy_perform() failed: %s\n",
                          curl_easy_strerror(res));
                  totalmessages--;
                  fclose(fpEmailFile);
                  continue;
               }
               else
               {
                  if (fpEmailFile)
                  {
                     fclose(fpEmailFile);
                  }
                  extractAttachment(fname);
                  removeFile(fname);
                  totalmessages--;
               }
            }
         }
      }
      else
      {
         if (chunk.memory)
         {
            free(chunk.memory);
         }
         curl_easy_cleanup(curl);
         printf("\nNo New Messages..Exiting\n");
         return(0);
      }
   }
   printf("\nPOP Download Complete..All downloaded files are available at %s\n", folderPath);
   curl_easy_cleanup(curl);
   return(atoi(m));
}
