#ifndef __FTP_UPLOAD_H__
#define __FTP_UPLOAD_H__

#include <commonutils.h>
#include <curl/curl.h>

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
 * @return   unsigned long - 0 on success, Non-Zero Error code on failure
 */
unsigned long ftpUpload(const char *username, const char *password, const char *destination, int port, const char *filePath, int ServerType, const char *proxyserver, int isDebugMode, int timeout,APP_TYPE apptype);


/**
 * Interface to Check FTP connection
 *
 * @name ftpConnTest
 * @function
 * @param   const char *username - FTP server username, "anonymous" in case of anonymous logon
 * @param   const char *password - FTP server password, "anonymous" in case of anonymous logon
 * @param   const char *destination - ftp server location
 * @param   int port - ftp server listen port usually 21
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   unsigned long - 0 on success, Non-Zero Error code on failure
 */
unsigned long ftpConnTest(const char *username, const char *password, const char *destination, int port, int ServerType, const char *proxyserver, int isDebugMode, int timeout);
#endif /* __FTP_UPLOAD_H__ */
