/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef __HTMLSSI_H__
#define __HTMLSSI_H__

#include "html.h"
#include "http_parse.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
    HttpPageContext *context;
    void* data;
}HTMLDATA;

#define HTTP_POST 1
#define HTTP_GET  2

#define SSITagStart "<!--#ssi"
#define SSITagEnd "-->"


//APIs defined in htmlssi.c
void HTMLGetArgs(SSIARGS* args, char* tag, uint8_t operation);
void HTMLParseTag(SSIARGS* args, SSITAG* parsedTag, PARSEOPTION option);
BOOL HTMLDoConditional(HTMLSTATE* htmlState, SSITAG* parsedTag);
void HTMLGetOptions(char* tagStr, uint32_t* options);
void HTMLGetOidInfo(char* oidStr, uint32_t* oid, uint32_t* index);
HTMLSTATE* HTMLGetState(HttpPageContext *context, uint8_t httpType);
void HTMLReleaseState(HttpPageContext *context);
BOOL HTMLTestCond(SINT32 cmpRes, SSIOPER tagOper);
void HttpCgiHandler(HttpPageContext *conHandle, char *pathPtr, char *argNames[], 
                    char *argValues[], int argCnt);
void HtmlStartHidden(HTMLSTATE* pState, oid_t oid, int32_t index, uint32_t val);
int32_t HtmlGetHiddenIndex(HTMLSTATE* pState, oid_t oid, int32_t index );
int32_t HtmlEndHidden( HTMLSTATE* pState, oid_t oid, int32_t);
int HttpSsiHandler (HttpPageContext *context, const char *tag);


#ifdef __cplusplus
}
#endif

#endif
