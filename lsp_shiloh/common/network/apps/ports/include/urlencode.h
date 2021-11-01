#ifndef __SOLUTION_URL_ENCODE_H__
#define __SOLUTION_URL_ENCODE_H__
#include <curl/curl.h>
#define MAX_URL_LENGTH 1024
void solutionURLEncode(CURL *curl, char *nonEncodedString, char *enCodedString, int length);
#endif
