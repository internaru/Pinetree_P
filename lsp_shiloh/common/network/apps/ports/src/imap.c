//Prototype implementation for downloading emails and extracting attachments from IMAP server.

// TODO: decoding UTF8 filenames. Extract FROM/TO/SUBJECT/CONTENT 
// TODO:Email files along with attachement will not be deleted..Need to decide when to delete.
#include <imap.h>
#define IMAP_CONNECT_TIMEOUT    120
int receive_imap_emails(const char *username, const char *password, const char *mailhost, int port, const char *emailStorePath, int isDebugMode)
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
   char inbox[256]        = { 0 };
   if (mailhost)
   {
      if ((strncasecmp(mailhost, "imap://", strlen("imap://")) == 0))
      {
         sprintf(email_server, "%s", mailhost);
      }
      else if ((strncasecmp(mailhost, "imaps://", strlen("imaps://")) == 0))
      {
         sprintf(email_server, "%s", mailhost);
      }
      else
      {
         sprintf(email_server, "imaps://%s", mailhost);
      }
   }
   if (port > 0)
   {
      curl_easy_setopt(curl, CURLOPT_PORT, port);
   }
   sprintf(inbox, "%s/INBOX/", email_server);
   curl_easy_setopt(curl, CURLOPT_URL, inbox);
   curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, IMAP_CONNECT_TIMEOUT);
   curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
   curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH UNSEEN");
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getDataCB);
   if (isDebugMode)
   {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   }
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
   curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl/1.0");
   CURLcode res = curl_easy_perform(curl);
   char     nstrMessage[256];
   memset(nstrMessage, 0, sizeof(nstrMessage));
   int ncurMessage = 0;
   if (res != CURLE_OK)
   {
      printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
      if (chunk.memory)
      {
         free(chunk.memory);
      }
      curl_easy_cleanup(curl);
      return(-1);
   }
   else
   {
      printf("Received data \n[%s]\n", chunk.memory);
      char *start = strstr(chunk.memory, "SEARCH");
      char *end   = chunk.memory + chunk.size;
      if (start)
      {
         start = start + strlen("search") + 1;
         while (start < end && !isdigit(*start))
         {
            start++;
         }
      }
      if (end)
      {
         while (start <= end && !isdigit(*end))
         {
            end--;
         }
      }
      if (!start || (end - start < 1))
      {
         printf("no new messages");
         if (chunk.memory)
         {
            free(chunk.memory);
            chunk.memory = NULL;
         }
         curl_easy_cleanup(curl);
         return(-1);
      }
      char *curStart = end;
      char *curEnd   = end;
      while (curStart >= start && isdigit(*curStart))
      {
         curStart--;
      }
      curStart++;
      while (curStart >= start)
      {
         ncurMessage = 0;
         strncpy(nstrMessage, curStart, curEnd - curStart + 1);
         nstrMessage[curEnd - curStart + 1] = '\0';
         ncurMessage = atoi(nstrMessage);
         if (ncurMessage <= 0)
         {
            break;
         }
         memset(nstrMessage, 0, sizeof(nstrMessage));
         curEnd = curStart - 1;
         while (curEnd >= start && !isdigit(*curEnd))
         {
            curEnd--;
         }
         curStart = curEnd;
         while (curStart >= start && isdigit(*curStart))
         {
            curStart--;
         }
         curStart++;
         printf("Download Message No = %d\n", ncurMessage);
         char command[1024];
         char fname[1024];
         sprintf(command, "%s/INBOX/;UID=%d", email_server, ncurMessage);
         sprintf(fname, "%s/Message%d", folderPath, ncurMessage);
         curl_easy_setopt(curl, CURLOPT_URL, command);
         curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
         if ((fpEmailFile = fopen(fname, "wt")) == NULL)
         {
            printf("\nCannot open %s", fname);
            continue;
         }
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpEmailFile);
         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFileCB);
         res = curl_easy_perform(curl);
         if (res != CURLE_OK)
         {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
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
         }
      }
   }
   if (chunk.memory)
   {
      free(chunk.memory);
   }
   printf("\nIMAP Download Complete..All downloaded files are available at %s\n", folderPath);
   curl_easy_cleanup(curl);
   return(OK);
}
