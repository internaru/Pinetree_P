#ifndef __IMAP_H__
#define __IMAP_H__

#include <commonutils.h>
#include <curl/curl.h>

int receive_imap_emails( const char *username,  const char *password,  const char *mailhost, int port, const char *emailStorePath, int isDebugMode);

#endif /* __IMAP_H__ */

