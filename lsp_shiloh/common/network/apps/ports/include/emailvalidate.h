#ifndef __EMAIL_VALIDATE_H__
#define __EMAIL_VALIDATE_H__

#include "commonutils.h"

/**
 * Trim the selected string and check whether it is valid email address
 * @function
 * @name getValidEmail
 * @function
 * @param char *emailaddress - string to be checked for validity
 * @return char * - trimmed valid string, NULL in case of Invalid
 */

char *getValidEmail(char *emailaddress);
#endif /* __EMAIL_VALIDATE_H__ */
