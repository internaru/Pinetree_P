#ifndef __POP_H__
#define __POP_H__

#include <commonutils.h>
#include <curl/curl.h>

int receive_pop3_emails( const char *username,  const char *password,  const char *mailhost, int port, const char *emailStorePath, int isDebugMode);

#endif /* __POP_H__ */

