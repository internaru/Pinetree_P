#ifndef __WEBDAV_UPLOAD_H__
#define __WEBDAV_UPLOAD_H__

#include <commonutils.h>
#include <curl/curl.h>

/**
 * Interface to upload a file on remote http/webDAV server
 *
 * @name webDAVUpload
 * @function
 * @param   const char *username - WEBDAV server username
 * @param   const char *password - WEBDAV server password
 * @param   const char *destination - http server appended with path to upload the file
 * @param   int port - http server listen port usually 80
 * @param   const char *filePath - path to local file that will be uploaded to server
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return   unsigned long - 0 on success, Non-Zero Error code on failure
 */
unsigned long webDAVUpload(const char *username, const char *password, const char *destination, int port, const char *filePath, int ServerType, const char *proxyserver, int isDebugMode,int timeout,APP_TYPE apptype);

/**
 * Interface to Check WebDAV connection 
 *
 * @name webDAVConnTest
 * @function
 * @param   const char *username - WEBDAV server username
 * @param   const char *password - WEBDAV server password
 * @param   const char *destination - http server appended with path
 * @param   int port - http server listen port usually 80
 * @param   int ServerType - 0 - no proxy server, 1 - proxy server present
 * @param   const  char *proxyserver - proxy server name or NULL -> when no proxy,
 * @param   int isDebugMode - 0 - no debug, 1 - debug
 * @return  unsigned long - 0 on success, Non-Zero Error code on failure
 */
unsigned long webDAVConnTest(const char *username, const char *password, const char *destination, int port, int ServerType, const char *proxyserver, int isDebugMode,int timeout);
#endif /* __WEBDAV_UPLOAD_H__ */
