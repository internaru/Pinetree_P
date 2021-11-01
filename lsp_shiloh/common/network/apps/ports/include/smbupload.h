#ifndef __SMBUPLOAD_H__
#define __SMBUPLOAD_H__
#include <commonutils.h>

/**
 * Interface to upload a file on remote smb server
 *
 * @name smbUpload
 * @function
 * @param   const char *username - SMB server username, NULL in case of anonymous logon
 * @param   const char *password - SMB server password, NULL in case of anonymous logon
 * @param   const char *smbshare - smb server location to upload the file
 * @param   int port - smb server listen port usually 445, keep it 0 to select default port
 * @param   const char *remotePath - remote SMB server path where file/folder will be uploaded to server
 * @param   const char *localPath - path to local file that will be uploaded to server
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   UINT32 - 0 on success, Non-Zero error code on Failure
 */
UINT32 smbUpload(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, const char *localPath, int isDebug);

/**
 * Interface to test SMB connection
 *
 * @name smbConnTest
 * @function
 * @param   const char *username - SMB server username, NULL in case of anonymous logon
 * @param   const char *password - SMB server password, NULL in case of anonymous logon
 * @param   const char *smbshare - smb server location
 * @param   int port - smb server listen port usually 445, keep it 0 to select default port
 * @param   const char *remotePath - remote SMB server path, NULL in case smbshare contains complete path
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   UINT32 - 0 on success, Non-Zero error code on Failure
 */
UINT32 smbConnTest(const char *username, const char *password, const char *smbshare, int port, const char *remotePath, int isDebug);


UINT32 executeSmbClient(char *cmd);

void getSMBUploadDetails(const char *dir, unsigned long *totalsize, int *filecount);
#endif /* __SMBUPLOAD_H__ */
