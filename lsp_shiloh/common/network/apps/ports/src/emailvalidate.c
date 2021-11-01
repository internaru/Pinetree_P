#include "emailvalidate.h"
#define SPACE_CHAR                32
#define DOUBLEQUOTE_CHAR          34
#define DOT_CHAR                  46
#define AT_CHAR                   64
#define BACKSLASH_CHAR            92
#define TILDE_CHAR                126
#define MAX_EMAIL_LENGTH          254
#define MIN_EMAIL_LENGTH          3
#define MAX_LOCAL_LENGTH          64
#define PREVCHAR(x)               (x - 1)
#define NOT_ALLOWED_LOCALPART     "()<>@,;:\\\"[]"
#define NOT_ALLOWED_DOMAINPART    "()<>@,;\\\""

/**
 * Trim the selected string and check whether it is valid email address
 * @function
 * @name getValidEmail
 * @function
 * @param char *emailaddress - string to be checked for validity
 * @return char * - trimmed valid string, NULL in case of Invalid
 */

char *getValidEmail(char *emailaddress)
{
   int  i                = 0;
   char *selected        = NULL;
   char *after_at_domain = NULL;

   if (emailaddress == NULL)
   {
      return(NULL);
   }
   while (emailaddress[i] && isspace(emailaddress[i]))
   {
      i++;
   }
   if (emailaddress[i] == '\0')
   {
      return(NULL);
   }
   unsigned int len = strlen(emailaddress);
   char *validmail = (char *)malloc(len + 1);
   if(validmail == NULL)
   {
      return(NULL);
   }
   strncpy(validmail, emailaddress + i,len-i);
   validmail[len - i]='\0';
   len = strlen(validmail);
   i = len;
   while (validmail[i - 1] == SPACE_CHAR)
   {
      i--;
   }
   validmail[i] = '\0';
   selected     = validmail;
   if (*selected == DOT_CHAR)
   {
      if (validmail)
      {
         free(validmail);
         validmail = NULL;
      }
      return(NULL);
   }
   len = strlen(validmail);
   if ((len > MAX_EMAIL_LENGTH) || (len < MIN_EMAIL_LENGTH))
   {
      if (validmail)
      {
         free(validmail);
         validmail = NULL;
      }
      return(NULL);
   }
   for ( ; *selected; selected++)
   {
      if ((*selected == DOUBLEQUOTE_CHAR) && ((selected == validmail) || ((selected > validmail) && (*PREVCHAR(selected) == DOT_CHAR)) || ((selected > validmail) && (*PREVCHAR(selected) == DOUBLEQUOTE_CHAR))))
      {
         while (*++selected)
         {
            if (*selected == DOUBLEQUOTE_CHAR)
            {
               break;
            }
            if ((*selected == BACKSLASH_CHAR) && (*++selected == SPACE_CHAR))
            {
               continue;
            }
            if ((*selected < SPACE_CHAR) || (*selected > TILDE_CHAR))
            {
               if (validmail)
               {
                  free(validmail);
                  validmail = NULL;
               }
               return(NULL);
            }
         }
         if (!*selected++)
         {
            if (validmail)
            {
               free(validmail);
               validmail = NULL;
            }
            return(NULL);
         }
         if (*selected == AT_CHAR)
         {
            if ((selected - validmail) > MAX_LOCAL_LENGTH)
            {
               if (validmail)
               {
                  free(validmail);
                  validmail = NULL;
               }
               return(NULL);
            }
            break;
         }
         if (*selected != DOT_CHAR)
         {
            if (validmail)
            {
               free(validmail);
               validmail = NULL;
            }
            return(NULL);
         }
         continue;
      }
      if (*selected == AT_CHAR)
      {
         if ((selected - validmail) > MAX_LOCAL_LENGTH)
         {
            if (validmail)
            {
               free(validmail);
               validmail = NULL;
            }
            return(NULL);
         }
         break;
      }
      if ((*selected <= SPACE_CHAR) || (*selected > TILDE_CHAR))
      {
         if (validmail)
         {
            free(validmail);
            validmail = NULL;
         }
         return(NULL);
      }

      if ((selected > validmail) && (*PREVCHAR(selected) == DOT_CHAR) && (*selected == DOT_CHAR))
      {
         if (validmail)
         {
            free(validmail);
            validmail = NULL;
         }
         return(NULL);
      }


      if (strchr(NOT_ALLOWED_LOCALPART, *selected))
      {
         if (validmail)
         {
            free(validmail);
            validmail = NULL;
         }
         return(NULL);
      }
   }
   if ((selected == validmail) || (*PREVCHAR(selected) == DOT_CHAR))
   {
      if (validmail)
      {
         free(validmail);
         validmail = NULL;
      }
      return(NULL);
   }
   if (!(*selected) || !*(after_at_domain = ++selected))
   {
      if (validmail)
      {
         free(validmail);
         validmail = NULL;
      }
      return(NULL);
   }
   do
   {
      if (*selected == DOT_CHAR)
      {
         if ((selected == after_at_domain) || (*PREVCHAR(selected) == DOT_CHAR))
         {
            if (validmail)
            {
               free(validmail);
               validmail = NULL;
            }
            return(NULL);
         }
      }

      if ((*selected <= SPACE_CHAR) || (*selected >= TILDE_CHAR))
      {
         if (validmail)
         {
            free(validmail);
            validmail = NULL;
         }
         return(NULL);
      }
      if (strchr(NOT_ALLOWED_DOMAINPART, *selected))
      {
         if (validmail)
         {
            free(validmail);
            validmail = NULL;
         }
         return(NULL);
      }
   } while (*++selected);

   return(validmail);
}
