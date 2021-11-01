#ifndef __SEND_EMAIL_H__
#define __SEND_EMAIL_H__

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <libgen.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <commonutils.h>
#include <curl/curl.h>


struct  mime_list_struct
{
   const char   *file_suffix;
   unsigned int suffix_length;
   const char   *mime_name;
};

/**
 * identify MIME details of file  - Internal function
 * @name identify_mime
 * @function
 * @param char *filename - name of the file
 * @return char * - string containg mime details of file
 */
const char *identify_mime(char *filename);

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
                        const char *subject, const char *content, int isDebugMode);*/

unsigned long sendEmail(email_info_message_t *email_message);
#endif /* __SEND_EMAIL_H__ */
