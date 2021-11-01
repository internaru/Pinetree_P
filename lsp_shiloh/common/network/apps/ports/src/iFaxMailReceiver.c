#include <stdio.h>
#include <unistd.h>
#include "pthread.h"
#include "commonutils.h"
#include "sqlite3.h"
#include "iFaxMailInformation.h"
#include "imap.h"
#include "pop.h"
static iFaxReceiveInformation_t giFaxReceiveInformation = { 0 };
extern sqlite3                  *dbInstance;
#define DATABASE_IFAX    "CONF_IFAXSERVER"
int getIFaxReceiveInformation()
{
   char         *strCur = NULL;
   sqlite3      *g_pDB  = dbInstance;
   sqlite3_stmt *pStmt  = NULL;
   const char   *pTail  = NULL;
   int          nResult = 0;
   char         szQuery[2048];
   memset(&giFaxReceiveInformation, 0x00, sizeof(iFaxReceiveInformation_t));
   snprintf(szQuery, sizeof(szQuery), "SELECT RECEIVEMAILTYPE,RECEIVEMAILSERVER,RECEIVEMAILPORT,RECEIVEMAILAUTHENTICATION,RECEIVEMAILSECURITY,RECEIVEMAILUSER,RECEIVEMAILPASSWORD,CHECKINTERVAL %s WHERE NO=4", DATABASE_IFAX);
   nResult = sqlite3_prepare(g_pDB, szQuery, 2048, &pStmt, &pTail);
   if (nResult == SQLITE_OK)
   {
      if (sqlite3_step(pStmt) == SQLITE_ROW)
      {
         giFaxReceiveInformation.receiveMailType = sqlite3_column_int(pStmt, 0);
         strCur = (char *)sqlite3_column_text(pStmt, 1);
         if (strCur)
         {
            strncpy(giFaxReceiveInformation.receiveMailAddress, strCur, MAX_IFAX_STR - 1);
            giFaxReceiveInformation.receiveMailAddress[MAX_IFAX_STR - 1] = '\0';
         }
         giFaxReceiveInformation.receiveMailPort           = sqlite3_column_int(pStmt, 2);
         giFaxReceiveInformation.receiveMailAuthentication = sqlite3_column_int(pStmt, 3);
         giFaxReceiveInformation.receiveMailSecurity       = sqlite3_column_int(pStmt, 4);
         strCur = (char *)sqlite3_column_text(pStmt, 5);
         if (strCur)
         {
            strncpy(giFaxReceiveInformation.receiveMailUser, strCur, MAX_IFAX_STR - 1);
            giFaxReceiveInformation.receiveMailUser[MAX_IFAX_STR - 1] = '\0';
         }
         strCur = (char *)sqlite3_column_text(pStmt, 6);
         if (strCur)
         {
            strncpy(giFaxReceiveInformation.receiveMailPassword, strCur, MAX_IFAX_STR - 1);
            giFaxReceiveInformation.receiveMailPassword[MAX_IFAX_STR - 1] = '\0';
         }
         strCur = (char *)sqlite3_column_text(pStmt, 7);
         if (strCur)
         {
            strncpy(giFaxReceiveInformation.receiveMailPassword, strCur, MAX_IFAX_STR - 1);
            giFaxReceiveInformation.receiveMailPassword[MAX_IFAX_STR - 1] = '\0';
         }
         giFaxReceiveInformation.checkInterval = sqlite3_column_int(pStmt, 8);
      }
   }
   sqlite3_finalize(pStmt);
   pStmt = NULL;
   return(OK);
}


void *iFax_receive_messages(void *args)
{
   printf("\nStart Internet Fax Receiver thread\n");
   char *username       = NULL;
   char *password       = NULL;
   char *mailhost       = NULL;
   int  port            = 0;
   char *emailStorePath = NULL;
   while (1)
   {
      if (giFaxReceiveInformation.checkInterval > 0)
      {
         sleep(60 * giFaxReceiveInformation.checkInterval);
      }
      else
      {
         sleep(600);
      }
      username = (char *)malloc(MAX_IFAX_STR);
      if (username)
      {
         memset(username, 0, MAX_IFAX_STR);
         memcpy(username, giFaxReceiveInformation.receiveMailUser, strlen(giFaxReceiveInformation.receiveMailUser) + 1);
      }
      password = (char *)malloc(MAX_IFAX_STR);
      if (password)
      {
         memset(password, 0, MAX_IFAX_STR);
         memcpy(password, giFaxReceiveInformation.receiveMailUser, strlen(giFaxReceiveInformation.receiveMailPassword) + 1);
      }
      if (mailhost)
      {
         memset(mailhost, 0, MAX_IFAX_STR);
         if ((strncasecmp(giFaxReceiveInformation.receiveMailAddress, "imap://", strlen("imap://")) == 0))
         {
            snprintf(mailhost, MAX_IFAX_STR, "%s", giFaxReceiveInformation.receiveMailAddress);
         }
         else if ((strncasecmp(mailhost, "imaps://", strlen("imaps://")) == 0))
         {
            snprintf(mailhost, MAX_IFAX_STR, "%s", giFaxReceiveInformation.receiveMailAddress);
         }
         else if ((strncasecmp(giFaxReceiveInformation.receiveMailAddress, "pop3://", strlen("pop3://")) == 0))
         {
            snprintf(mailhost, MAX_IFAX_STR, "%s", giFaxReceiveInformation.receiveMailAddress);
         }
         else if ((strncasecmp(mailhost, "pop3s://", strlen("pop3s://")) == 0))
         {
            snprintf(mailhost, MAX_IFAX_STR, "%s", giFaxReceiveInformation.receiveMailAddress);
         }
         else
         {
            if (giFaxReceiveInformation.receiveMailType == 0)
            {
               if (giFaxReceiveInformation.receiveMailSecurity == 0)
               {
                  snprintf(mailhost, MAX_IFAX_STR, "pop3://%s", giFaxReceiveInformation.receiveMailAddress);
               }
               else
               {
                  snprintf(mailhost, MAX_IFAX_STR, "pop3s://%s", giFaxReceiveInformation.receiveMailAddress);
               }
            }
            else
            {
               if (giFaxReceiveInformation.receiveMailSecurity == 0)
               {
                  snprintf(mailhost, MAX_IFAX_STR, "imap://%s", giFaxReceiveInformation.receiveMailAddress);
               }
               else
               {
                  snprintf(mailhost, MAX_IFAX_STR, "imaps://%s", giFaxReceiveInformation.receiveMailAddress);
               }
            }
         }
      }
      if (giFaxReceiveInformation.receiveMailPort > 0)
      {
         port = giFaxReceiveInformation.receiveMailPort;
      }
      if (giFaxReceiveInformation.receiveMailType == 0)
      {
         receive_pop3_emails(username, password, mailhost, port, emailStorePath, 1);
      }
      else
      {
         receive_imap_emails(username, password, mailhost, port, emailStorePath, 1);
      }
      if (username)
      {
         free(username);
         username = NULL;
      }
      if (password)
      {
         free(password);
         password = NULL;
      }
      if (mailhost)
      {
         free(mailhost);
         mailhost = NULL;
      }
   }
   return(NULL);
}


int init_iFax()
{
   pthread_attr_t threadAttributes;
   int            result = 0;
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
   pthread_t iFax_thread;
   result = pthread_create(&iFax_thread, &threadAttributes, iFax_receive_messages, (void *)NULL);
   pthread_attr_destroy(&threadAttributes);
   return(result);
}
