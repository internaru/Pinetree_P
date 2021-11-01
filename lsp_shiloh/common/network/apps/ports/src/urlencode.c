#include <urlencode.h>
#include <commonutils.h>
#include <curl/curl.h>
void solutionURLEncode(CURL *curl, char *nonEncodedString, char *enCodedString, int length)
{
   char *start, *end;
   bool foundone = false;
   if ((nonEncodedString == NULL) || (enCodedString == NULL))
   {
      return;
   }
   if ((start = strstr(nonEncodedString, "://")))
   {
      start = start + 3;
      strncat(enCodedString, nonEncodedString, start - nonEncodedString);
   }
   else
   {
      start = nonEncodedString;
   }
   while (*start == '/')
   {
      start++;
   }
   if ((end = strchr(start, '/')))
   {
      end--;
   }
   else
   {
      end = start + strlen(start) - 1;
   }
   while (end && *start)
   {
      if (*start != '/')
      {
         char *escapedURL = curl_easy_escape(curl, start, end - start + 1);
         if (escapedURL)
         {
            strncat(enCodedString, escapedURL, length - strlen(enCodedString) - 1);
            curl_free(escapedURL);
         }
      }
      start = end + 1;
      while (*start == '/')
      {
         start++;
         foundone = true;
      }
      if (*start)
      {
         end = strchr(start, '/');
         if (end)
         {
            end--;
         }
         else
         {
            end = strlen(nonEncodedString) + nonEncodedString - 1;
         }
      }
      if (foundone)
      {
         strncat(enCodedString, "/", length - strlen(enCodedString) - 1);
         foundone = false;
      }
   }
}
