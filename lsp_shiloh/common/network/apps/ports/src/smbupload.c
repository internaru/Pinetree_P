#include "smbupload.h"


#define SMB_UPLOAD_SUCCESS                 0
#define SMBCLIENT                          ("smbclient -d 0")
#define SMBCLIENT_DEBUG                    ("smbclient -d 10")
#define SPACE_CHAR                         32
#define SMB_ERROR_CONFIG                   1
#define SMB_ERROR_ACCESS_DENIED            2
#define SMB_ERROR_OBJECT_NAME_NOT_FOUND    3
#define SMB_ERROR_OBJECT_PATH_NOT_FOUND    4
#define SMB_ERROR_HOST_UNREACHABLE         5
#define SMB_ERROR_OBJECT_NAME_COLLISION    6
#define SMB_ERROR_BAD_NETWORK_NAME         7
#define SMB_ERROR_LOGON_FAILURE            8
#define SMB_ERROR_UPLOAD_FAILED            222

#define MAX_SMB_FILES                      500
#define MAX_SMB_SIZE                       1024 * 1024 * 512


void getSMBUploadDetails(const char *dir, unsigned long *totalsize, int *filecount)
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
      char tempfsEntry[PATH_MAX] = {0};
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
         getSMBUploadDetails(tempfsEntry, totalsize, filecount);
      }
      else
      {
         if (*filecount >= MAX_SMB_FILES)
         {
            closedir(dp);
            return;
         }
         if (*totalsize > MAX_SMB_SIZE)
         {
            closedir(dp);
            return;
         }
         (*totalsize) += statbuf.st_size;
         (*filecount)++;
      }
   }
   closedir(dp);
}


//Known issue - NT_STATUS_OBJECT_NAME_COLLISION error handling

//Folder name with "/"

/**
 * Interface to upload a file on remote smb server, requires smbclient binary
 *
 * @name smbUpload
 * @function
 * @param   const char *username - SMB server username, NULL in case of anonymous logon
 * @param   const char *password - SMB server password, NULL in case of anonymous logon
 * @param   const char *smbshare - smb server location to upload the file
 * @param   int port - smb server listen port usually 445, keep it 0 to select default port
 * @param   const char *remotePath - remote SMB server path where file/folder will be uploaded to server, NULL in case of shared folder base
 * @param   const char *localPath - path to local file that will be uploaded to server
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return  UINT32 - 0 on success, Non-Zero error code on Failure
 */
UINT32 smbUpload(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, const char *localPath, int isDebug)
{
   char uploadcmd[4096]         = { 0 };
   char tempdir[256]            = { 0 };
   char smbSharePath[1024]      = { 0 };
   char smbClient[2048]         = { 0 };
   char testConnectioncmd[4096] = { 0 };

   if ((smbshare == NULL) || (localPath == NULL) || (file_exist(localPath) == 0))
   {
      return(ERROR_CONFIG);
   }

   if (strncasecmp(smbshare, "smb:", strlen("smb:")) == 0)
   {
      sprintf(smbSharePath, "%s", smbshare + 4);
   }
   else
   {
      strcpy(smbSharePath, smbshare);
   }

   int len = strlen(smbSharePath);
   int i = 0, j = 0;
   if(len)
   {
      len--;
      while (smbSharePath[len] == '/' || smbSharePath[len] == '\\')
      {
        smbSharePath[len] = '\0';
        len--;
      }
   }  

   while (smbSharePath[i] == SPACE_CHAR)
   {
      i++;
   }

   if (smbSharePath[i] == '\0')
   {
      return(ERROR_CONFIG);
   }
   char *fullPath = NULL;
   int pathLength;
   if (remotePath)
   {
      while (remotePath[j] == SPACE_CHAR)
      {
         j++;
      }
      while (remotePath[j] == '/' || remotePath[j] == '\\')
      {
         j++;
      }
   }

   if (remotePath && (remotePath[j] != '\0'))
   {
      pathLength = strlen(smbSharePath) + strlen(remotePath) + 2;
      fullPath = (char *)malloc(pathLength);
      if(NULL == fullPath)
      {
         return(ERROR_CONFIG);
      }
      strncpy(fullPath, smbSharePath + i,pathLength-1);
      fullPath[pathLength-1] = '\0';
      len = strlen(fullPath);
      i   = len;
      while (fullPath[i - 1] == SPACE_CHAR)
      {
         i--;
      }
      fullPath[i] = '\0';
      strncat(fullPath, "/",pathLength- strlen(fullPath)- 1);
      strncat(fullPath, remotePath + j,pathLength - strlen(fullPath)- 1);
   }
   else
   {
      pathLength = strlen(smbSharePath)+ 1;
      fullPath = (char *)malloc(pathLength);
      if(NULL == fullPath)
      {
         return(ERROR_CONFIG);
      }
      strncpy(fullPath, smbSharePath + i,pathLength-1);
      fullPath[pathLength-1] = '\0';
      len = strlen(fullPath);
      i   = len;
      while (fullPath[i - 1] == SPACE_CHAR)
      {
         i--;
      }
      fullPath[i] = '\0';
   }


   len = strlen(fullPath);
   i   = len;
   while (fullPath[i - 1] == SPACE_CHAR)
   {
      i--;
   }
   fullPath[i] = '\0';

   len = strlen(fullPath);
   int  indexService        = 0;
   int  indexServer         = 0;
   int  indexSubfolder      = 0;
   char serverName[1024]    = { 0 };
   char serviceName[1024]   = { 0 };
   char subFolderName[1024] = { 0 };
   bool strFound            = false;
   i = 0;
   for ( ; i < len; ++i)
   {
      if (strFound)
      {
         if (fullPath[i] == '/')
         {
            while (fullPath[i + 1] == '/')
            {
               ++i;
            }
            ++i;
            break;
         }
         else if (fullPath[i] == '\\')
         {
            while (fullPath[i + 1] == '\\')
            {
               ++i;
            }
            ++i;
            break;
         }
         else
         {
            serverName[indexServer] = fullPath[i];
            indexServer++;
            serverName[indexServer] = '\0';
         }
      }
      else
      {
         if (fullPath[i] == '/')
         {
            while (fullPath[i + 1] == '/')
            {
               ++i;
            }
            continue;
         }
         else if (fullPath[i] == '\\')
         {
            while (fullPath[i + 1] == '\\')
            {
               ++i;
            }
            continue;
         }
         else
         {
            serverName[indexServer] = fullPath[i];
            indexServer++;
            serverName[indexServer] = '\0';
            strFound = true;
         }
      }
   }

   for ( ; i < len; ++i)
   {
      if (fullPath[i] == '/')
      {
         while (fullPath[i + 1] == '/')
         {
            ++i;
         }
         ++i;
         break;
      }
      else if (fullPath[i] == '\\')
      {
         while (fullPath[i + 1] == '\\')
         {
            ++i;
         }
         ++i;
         break;
      }
      else
      {
         serviceName[indexService] = fullPath[i];
         indexService++;
         serviceName[indexService] = '\0';
      }
   }

   for ( ; i < len; ++i)
   {
      if (fullPath[i] == '\\')
      {
         while (fullPath[i + 1] == '\\')
         {
            ++i;
         }
         if (indexSubfolder > 0)
         {
            fullPath[i] = '/';
         }
         else
         {
            continue;
         }
      }

      subFolderName[indexSubfolder] = fullPath[i];
      indexSubfolder++;
      subFolderName[indexSubfolder] = '\0';
   }

   len = strlen(subFolderName);
   if (len)
   {
      i = len;
      while (subFolderName[i - 1] == SPACE_CHAR || subFolderName[i - 1] == '/')
      {
         i--;
      }
      subFolderName[i] = '\0';
   }

   if (fullPath)
   {
      free(fullPath);
      fullPath = NULL;
   }

   printf("\nServer name [%s]", serverName);
   printf("\nShare Name  [%s]", serviceName);
   printf("\nSubFolder Name	[%s]", subFolderName);

   if (strcmp(serverName, "") == 0)
   {
      return(ERROR_CONFIG);
   }

   if (strcmp(serviceName, "") == 0)
   {
      return(ERROR_CONFIG);
   }

   if (port)
   {
      sprintf(smbClient, "%s -p %d", isDebug ? SMBCLIENT_DEBUG : SMBCLIENT, port);
   }
   else
   {
      sprintf(smbClient, "%s", isDebug ? SMBCLIENT_DEBUG : SMBCLIENT);
   }
   bool isSubFlder = strcmp(subFolderName, "") ? true : false;
   if (isDirectory(localPath))
   {
      //strcpy(tempdir, localPath);
      char smbcmd[1024] = {0};
      //char localDirName[256];
      //strcpy(localDirName, basename(tempdir));
      int           filecount = 0;
      unsigned long totalsize = 0;
      getSMBUploadDetails(localPath, &totalsize, &filecount);
      if ((totalsize > MAX_SMB_SIZE) || (filecount >= MAX_SMB_FILES))
      {
         return(ERROR_BEYOND_ALLOWED_LIMIT);
      }
      if (totalsize == 0 || filecount == 0)
      {
         printf("[SMB Upload]Empty Directory/Files");
         return(ERROR_CONFIG);
      }
      if (isSubFlder)
      {
         sprintf(smbcmd, "lcd \\\"%s\\\";cd \\\"%s\\\";recurse;prompt;mput *", localPath, subFolderName);
      }
      else
      {
         sprintf(smbcmd, "lcd \\\"%s\\\";recurse;prompt;mput *", localPath);
      }

      if (username && password && (strcmp(username, "") > 0) && (strcmp(password, "") > 0))
      {
         if (isSubFlder)
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, username, password, subFolderName);
         }
         else
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"ls\"", smbClient, serverName, serviceName, username, password);
         }

         sprintf(uploadcmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"%s\"", smbClient, serverName, serviceName, username, password, smbcmd);
      }
      else
      {
         if (isSubFlder)
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, subFolderName);
         }
         else
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"ls\"", smbClient, serverName, serviceName);
         }

         sprintf(uploadcmd, "%s \"//%s/%s\"  -N -c \"%s\"", smbClient, serverName, serviceName, smbcmd);
      }
      printf("\n[SMB Upload]Command = [%s]", uploadcmd);
   }
   else
   {
      strcpy(tempdir, localPath);
      char smbcmd[1024] = {0};
      if (getSize(localPath) > MAX_SMB_SIZE)
      {
         return(ERROR_BEYOND_ALLOWED_LIMIT);
      }
      if (isSubFlder)
      {
         sprintf(smbcmd, "lcd \\\"%s\\\";cd \\\"%s\\\";put %s", dirname(tempdir), subFolderName, basename((char *)localPath));
      }
      else
      {
         sprintf(smbcmd, "lcd \\\"%s\\\";put %s", dirname(tempdir), basename((char *)localPath));
      }
      if (username && password && (strcmp(username, "") > 0) && (strcmp(password, "") > 0))
      {
         if (isSubFlder)
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, username, password, subFolderName);
         }
         else
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"ls\"", smbClient, serverName, serviceName, username, password);
         }


         sprintf(uploadcmd, "%s \"//%s/%s\" -U \"%s%%%s\" -c \"%s\"", smbClient, serverName, serviceName, username, password, smbcmd);
      }
      else
      {
         if (isSubFlder)
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, subFolderName);
         }
         else
         {
            sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"ls\"", smbClient, serverName, serviceName);
         }

         sprintf(uploadcmd, "%s \"//%s/%s\" -N -c \"%s\"", smbClient, serverName, serviceName, smbcmd);
      }
      printf("\n[SMB Upload]Command = [%s]", uploadcmd);
   }
   UINT result = executeSmbClient(testConnectioncmd);
   printf("\n[SMB Upload]Test Command Result = [%d]", result);
   if (result == 0)
   {
      printf("\n[SMB Upload] Command Result = [%d]", (result = executeSmbClient(uploadcmd)));
      return(result);
   }
   else
   {
      return result;
   }
}


UINT32 smbConnTest(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, int isDebug)
{
   char smbSharePath[1024]      = { 0 };
   char smbClient[2048]         = { 0 };
   char testConnectioncmd[4096] = { 0 };

   if (smbshare == NULL)
   {
      return(ERROR_CONFIG);
   }

   if (strncasecmp(smbshare, "smb:", strlen("smb:")) == 0)
   {
      sprintf(smbSharePath, "%s", smbshare + 4);
   }
   else
   {
      strcpy(smbSharePath, smbshare);
   }

   int len = strlen(smbSharePath);
   int i = 0, j = 0;
   if(len)
   {
      len--;
      while (smbSharePath[len] == '/' || smbSharePath[len] == '\\')
      {
        smbSharePath[len] = '\0';
        len--;
      }
   }  

   while (smbSharePath[i] == SPACE_CHAR)
   {
      i++;
   }

   if (smbSharePath[i] == '\0')
   {
      return(ERROR_CONFIG);
   }
   char *fullPath = NULL;
   int pathLength;
   if (remotePath)
   {
      while (remotePath[j] == SPACE_CHAR)
      {
         j++;
      }
      while (remotePath[j] == '/' || remotePath[j] == '\\')
      {
         j++;
      }
   }

   if (remotePath && (remotePath[j] != '\0'))
   {
      pathLength = strlen(smbSharePath) + strlen(remotePath) + 2;
      fullPath = (char *)malloc(pathLength);
      if(NULL == fullPath)
      {
         return(ERROR_CONFIG);
      }
      strncpy(fullPath, smbSharePath + i,pathLength-1);
      fullPath[pathLength-1] = '\0';
      len = strlen(fullPath);
      i   = len;
      while (fullPath[i - 1] == SPACE_CHAR)
      {
         i--;
      }
      fullPath[i] = '\0';
      strncat(fullPath, "/",pathLength- strlen(fullPath)- 1);
      strncat(fullPath, remotePath + j,pathLength - strlen(fullPath)- 1);      
   }
   else
   {
      pathLength = strlen(smbSharePath) + 1;
      fullPath = (char *)malloc(pathLength);
      if(NULL == fullPath)
      {
         return(ERROR_CONFIG);
      }
      strncpy(fullPath, smbSharePath + i,pathLength-1);
      fullPath[pathLength-1] = '\0';
      len = strlen(fullPath);
      i   = len;
      while (fullPath[i - 1] == SPACE_CHAR)
      {
         i--;
      }
      fullPath[i] = '\0';
   }


   len = strlen(fullPath);
   i   = len;
   while (fullPath[i - 1] == SPACE_CHAR)
   {
      i--;
   }
   fullPath[i] = '\0';

   len = strlen(fullPath);
   int  indexService        = 0;
   int  indexServer         = 0;
   int  indexSubfolder      = 0;
   char serverName[1024]    = { 0 };
   char serviceName[1024]   = { 0 };
   char subFolderName[1024] = { 0 };
   bool strFound            = false;
   //printf("Full Path %s", fullPath);
   i = 0;
   for ( ; i < len; ++i)
   {
      if (strFound)
      {
         if (fullPath[i] == '/')
         {
            while (fullPath[i + 1] == '/')
            {
               ++i;
            }
            ++i;
            break;
         }
         else if (fullPath[i] == '\\')
         {
            while (fullPath[i + 1] == '\\')
            {
               ++i;
            }
            ++i;
            break;
         }
         else
         {
            serverName[indexServer] = fullPath[i];
            indexServer++;
            serverName[indexServer] = '\0';
         }
      }
      else
      {
         if (fullPath[i] == '/')
         {
            while (fullPath[i + 1] == '/')
            {
               ++i;
            }
            continue;
         }
         else if (fullPath[i] == '\\')
         {
            while (fullPath[i + 1] == '\\')
            {
               ++i;
            }
            continue;
         }
         else
         {
            serverName[indexServer] = fullPath[i];
            indexServer++;
            serverName[indexServer] = '\0';
            strFound = true;
         }
      }
   }

   for ( ; i < len; ++i)
   {
      if (fullPath[i] == '/')
      {
         while (fullPath[i + 1] == '/')
         {
            ++i;
         }
         ++i;
         break;
      }
      else if (fullPath[i] == '\\')
      {
         while (fullPath[i + 1] == '\\')
         {
            ++i;
         }
         ++i;
         break;
      }
      else
      {
         serviceName[indexService] = fullPath[i];
         indexService++;
         serviceName[indexService] = '\0';
      }
   }

   for ( ; i < len; ++i)
   {
      if (fullPath[i] == '\\')
      {
         while (fullPath[i + 1] == '\\')
         {
            ++i;
         }
         if (indexSubfolder > 0)
         {
            fullPath[i] = '/';
         }
         else
         {
            continue;
         }
      }

      subFolderName[indexSubfolder] = fullPath[i];
      indexSubfolder++;
      subFolderName[indexSubfolder] = '\0';
   }

   len = strlen(subFolderName);
   if (len)
   {
      i = len;
      while (subFolderName[i - 1] == SPACE_CHAR || subFolderName[i - 1] == '/')
      {
         i--;
      }
      subFolderName[i] = '\0';
   }

   if (fullPath)
   {
      free(fullPath);
      fullPath = NULL;
   }

   if (strcmp(serverName, "") == 0)
   {
      return(ERROR_CONFIG);
   }

   if (strcmp(serviceName, "") == 0)
   {
      return(ERROR_CONFIG);
   }

   if (port)
   {
      sprintf(smbClient, "%s -p %d", isDebug ? SMBCLIENT_DEBUG : SMBCLIENT, port);
   }
   else
   {
      sprintf(smbClient, "%s", isDebug ? SMBCLIENT_DEBUG : SMBCLIENT);
   }
   bool isSubFlder = strcmp(subFolderName, "") ? true : false;

   if (username && password && (strcmp(username, "") > 0) && (strcmp(password, "") > 0))
   {
      if (isSubFlder)
      {
         sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, username, password, subFolderName);
      }
      else
      {
         sprintf(testConnectioncmd, "%s \"//%s/%s\"  -U \"%s%%%s\" -c \"ls\"", smbClient, serverName, serviceName, username, password);
      }
   }
   else
   {
      if (isSubFlder)
      {
         sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"cd \\\"%s\\\"\"", smbClient, serverName, serviceName, subFolderName);
      }
      else
      {
         sprintf(testConnectioncmd, "%s \"//%s/%s\"  -N -c \"ls\"", smbClient, serverName, serviceName);
      }
   }
   UINT result = executeSmbClient(testConnectioncmd);
   if (result == 0)
   {
      printf("\nSMB Connection Test OK");
   }
   else
   {
      printf("\nSMB Connection Test Failed %u", result);
   }
   return result;
}


UINT32 executeSmbClient(char *cmd)
{
   UINT32 nResult = SMB_UPLOAD_SUCCESS;
   FILE   *fp;
   char   szBuff[512 + 1] = { 0, };
   int    nState          = 0;

   fp = popen(cmd, "r");

   if (fp == NULL)
   {
      return ERROR_CONFIG;
   }
   const char *pDest = NULL;
   while (fgets(szBuff, 512, fp) != NULL)
   {
      nResult = 0;
      pDest   = strstr(szBuff, "NT_STATUS_LOGON_FAILURE");

      if (pDest != NULL)
      {
         nResult = ERROR_AUTH;
         break;
      }

      pDest = strstr(szBuff, "NT_STATUS_ACCESS_DENIED");

      if (pDest != NULL)
      {
         nResult = ERROR_AUTH;
         break;
      }

      pDest = strstr(szBuff, "NT_STATUS_OBJECT_NAME_NOT_FOUND");

      if (pDest != NULL)
      {
         nResult = ERROR_CONNECT;
         break;
      }

      pDest = strstr(szBuff, "NT_STATUS_OBJECT_PATH_NOT_FOUND");

      if (pDest != NULL)
      {
         nResult = ERROR_CONNECT;
         break;
      }
      pDest = strstr(szBuff, "NT_STATUS_HOST_UNREACHABLE");

      if (pDest != NULL)
      {
         nResult = ERROR_CONNECT;
         break;
      }
      pDest = strstr(szBuff, "NT_STATUS_OBJECT_NAME_COLLISION");

      if (pDest != NULL)
      {
         nResult = ERROR_FAILED;
         break;
      }

      pDest = strstr(szBuff, "NT_STATUS_BAD_NETWORK_NAME");

      if (pDest != NULL)
      {
         nResult = ERROR_CONNECT;
         break;
      }
   }

   nState = pclose(fp);

   if ((nState != 0) && (nResult == 0))
   {
      nResult = ERROR_FAILED;
   }

   return nResult;
}
