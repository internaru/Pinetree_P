/******************************************************************************
 * Copyright (c) 2008-2011  Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_NETWORK
#include <pthread.h>
#include <posix_ostools.h>
#endif
#include "agMessage.h"
#include "agRouter.h"
#include "lassert.h"
#include "logger.h"
#include "dprintf.h"
#include "memAPI.h"

#include "html.h"
#include "htmlssi.h"
#include "http_parse.h"

#include "debug.h"
#include "string_mgr_api.h"
#include "oid_api.h"
#include "http_config.h"
#include "http_methods_support.h"

#ifndef __linux__
#include "cpu_api.h"
#endif

// Submodule ID for debug message logging
#define LOGGER_SUBMOD_IFACE HTTP_SUBMOD_SSI
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_HTTP | LOGGER_SUBMOD_IFACE 
#define DBG_PRFX    "HTTP SSI: "

/** \brief The base we want to use for strtoul function */
#define BASE_10 10

static HTMLDATA *gHtmlData = NULL;

static HTMLMAP gHtmlTags[NUM_TAGS] = {
    {SSI_IDc,    SSITAG_IDc,    3}, 
    {SSI_IDr,    SSITAG_IDr,    3}, 
    {SSI_IDs,    SSITAG_IDs,    3}, 
    {SSI_IDe,    SSITAG_IDe,    3}, 
    {SSI_ID,     SSITAG_ID,     2}, 
    {SSI_IF,     SSITAG_IF,     2}, 
    {SSI_ELSEIF, SSITAG_ELSEIF, 6},
    {SSI_ELSE,   SSITAG_ELSE,   4},
    {SSI_ENDIF,  SSITAG_ENDIF,  5},
    {SSI_INCLUDE,SSITAG_INCLUDE,7},
};

static HTMLMAP gHtmlOpers[NUM_OPERS] = {
    {SSI_EQ,   OPER_EQ,   2},
    {SSI_NEQ,  OPER_NE,   3}, 
    {SSI_GT,   OPER_GT,   2}, 
    {SSI_LT,   OPER_LT,   2}, 
    {SSI_GTE,  OPER_GTE,  3},
    {SSI_LTE,  OPER_LTE,  3},
    {SSI_ERR,  OPER_ERR,  3},
    {SSI_AND,  OPER_AND,  3},
    {SSI_NAND, OPER_NAND,  3},
};          


/**
 * Function Prototypes
 **/

/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/19/2004
 *
 *
 * Description: Parse the ag tag
 *
 ******************************************************************************/
int HttpSsiHandler (HttpPageContext *context, const char *tag)
{
    SSIARGS args;
    SSITAG parsedTag;
    BOOL condRes;
    HTMLSTATE* pState;
    char *oid_result;
    error_type_t e_res;
    BOOL outEnabled;   
    uint32_t locNestCount;

    char* outStr;
    BOOL error;
    uint32_t outLen;
    BOOL condCmd;

    oid_result = NULL;
    outStr = NULL;
    error = FALSE;
    outLen=-1;
    condCmd = FALSE;

    DBG_MSG( "Parse Tag: %s\n", tag );

    //get state ptr for this conHandle, if doesn't exist create new one
    pState = HTMLGetState(context, HTTP_GET);

    if ( pState == NULL )
    {
        DBG_ERR( "pState is NULL - exiting now\n" );
        return FALSE;
    }
    locNestCount = pState->nestCount;

    //make local copy of tag so we can modify it as we process it
    strcpy(pState->tagBuff, tag);
    ASSERT(strlen(tag) < HTTP_MAX_SSI_TAG);

    //first get the cmd, we always process conditional commands to see when we should
    //enable and disable output, all other commands we only process when output enabled
    HTMLGetArgs(&args, (char*)pState->tagBuff, HTTP_GET);
    HTMLParseTag(&args, &parsedTag, POPT_CMD);

    //always enabled if not in a conditional (nestCount == -1 means not in a conditional)
    if ( locNestCount == 0 )
    {
        outEnabled = TRUE;
    }
    else
    {
        //inside conditional only enabled if the conditionals output is enabled
        if ( pState->condOutEnabled[locNestCount-1] == TRUE )
        {
            outEnabled = TRUE;
        }
        else
        {
            outEnabled = FALSE;
        }
    }
    //see if this is a conditional command (IF, ELSEIF, ELSE, ENDIF)
    if ( parsedTag.tagType >= SSITAG_IF && parsedTag.tagType <= SSITAG_ENDIF )
    {
        condCmd = TRUE;
    }

    //process all non conditional commands here if we are enabled, skip them on disabled since
    //would be a waste of time
    if ( outEnabled == TRUE && condCmd == FALSE )
    {

        //parse the rest of the arguments
        HTMLParseTag(&args, &parsedTag, POPT_ARGS);
        // Substitute connection handle for oid index
        if ( parsedTag.oid1Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
        {
            parsedTag.oid1Index = context->id;
        }
        if ( parsedTag.oid2Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
        {
            parsedTag.oid2Index = context->id;
        }

        switch ( parsedTag.tagType )
        {
            case SSITAG_ID:
                //leave the tag in the data stream. It is resolved during http POST
                outStr = (char*)tag;
                break;
            case SSITAG_IDc:
                //Format: IDc_op oper oid val output
                // Options: _s val is a string enum
                //          _o val is an oid
                //
                //
                condRes = HTMLDoConditional(pState, &parsedTag);
                if ( condRes == TRUE )
                {
                    outStr = parsedTag.output;
                }
                break;
            case SSITAG_IDe:
                //Format: IDe oid
                //Shouldn't see these here, the compile utility should convert for us
                error = TRUE;
                break;
            case SSITAG_IDr:
                //Format: IDr_op oid  - resolve the oid to a value
                // options: _p oid is a password, send dummy password instead
                if ( parsedTag.options & SSIOPTION_P )
                {
                    outStr = DUMMY_PASSWORD;
                }
                else
                {
                    oid_result = MEM_MALLOC( OID_MAX_STRING_SIZE_IN_BYTES );
                    if ( oid_result == NULL )
                    {
                        DBG_ERR( "failed to malloc memory for oid\n" );
                        e_res = FAIL;
                    }
                    else
                    {
                        // get the internal value for the oid and translate for 
                        // the product specific protocol.
                        if ( ( HTTP_TYPE_HTML == context->contextTypeIndex) ||
                             ( HTTP_TYPE_JAVASCRIPT == context->contextTypeIndex ) )
                        {

                            e_res = http_translate_oid_to_html_protocol( parsedTag.oid1,
                                                                         parsedTag.oid1Index,
                                                                         oid_result,
                                                                         OID_MAX_STRING_SIZE_IN_BYTES );
                        }
                        else if ( HTTP_TYPE_XML == context->contextTypeIndex )
                        {
                            e_res = http_translate_oid_to_xml_protocol( parsedTag.oid1,
                                                                        parsedTag.oid1Index,
                                                                        oid_result,
                                                                        OID_MAX_STRING_SIZE_IN_BYTES );
                        }
                        else
                        {
                            e_res = FAIL;
                        }
                    }
                    
                    if ( e_res != OK )
                    {
                        DBG_ERR( "IDr failed to resolve oid %s - %#08X.%d\n", tag, parsedTag.oid1, parsedTag.oid1Index );
                    }
                    else
                    {
                        outStr = oid_result;
                    }
                }
                break;
            case SSITAG_IDs:
                //Format: IDs strcode
                outStr = (char *)string_mgr_get_string(parsedTag.oid1, string_mgr_loc_full);
                if ( outStr == NULL )
                {
                    DBG_ERR( "IDs failed to resolve strcode to string %s - %#08X\n", tag, parsedTag.oid1 );
                }
                break;

            case SSITAG_INCLUDE:
                DBG_VERBOSE( "Got include tag: %s\n", tag );
                {
                    char    *original_page_ptr; 
                    uint32_t  original_page_size;
                    uint32_t  original_at_byte;
                    char *original_uri;  
                    http_state_t original_state;
                    http_response_type_t original_type;
                    uint32_t original_parse_ssi;
                    void *original_tmp_data;

                    //push some critical variables onto this include stack
                    original_page_ptr   = context->pagePtr;
                    original_page_size  = context->pageSize;
                    original_at_byte    = context->atByte;
                    original_uri        = context->requested_uri;
                    original_state      = context->state;
                    original_type       = context->contextTypeIndex;
                    original_parse_ssi  = context->parseSSI;
                    original_tmp_data   = context->tmpData;
                                        
                    // Put the original URI into the tmp data field -
                    // Then the included file can use it if necessary
                    context->tmpData = (void*)context->requested_uri;

                    // Fetch data for the included page
                    context->atByte = 0;
                    context->requested_uri = args.arg[ 1 ];
                    parseHttpGet( context, true ) ;
                    context->requested_uri = original_uri;
                    fillContextWithData( context, false, true );

                    // pop the critical variables back off the stack
                    context->pagePtr = original_page_ptr;
                    context->pageSize = original_page_size;
                    context->atByte = original_at_byte;
                    context->state = original_state;
                    context->contextTypeIndex = original_type;
                    context->parseSSI = original_parse_ssi;
                    context->tmpData = original_tmp_data;
                }
                break;
            default:
                break;
        }
    }
    //always process conditionals since they are the ones that determine when we enable or
    //disable output
    else
    {
        if ( condCmd == TRUE )
        {
            //parse the rest of the arguments
            HTMLParseTag(&args, &parsedTag, POPT_ARGS);
            // Substitute connection handle for oid index
            if ( parsedTag.oid1Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
            {
                DBG_VERBOSE( "Setting index1 to id: %#X\n", context->id );
                parsedTag.oid1Index = (uint32_t)context->id;
            }
            if ( parsedTag.oid2Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
            {
                DBG_VERBOSE( "Setting index2 to id: %#X\n", context->id );
                parsedTag.oid2Index = (uint32_t)context->id;
            }

            DBG_VERBOSE( "cond tag: %s\n", tag );

            switch ( parsedTag.tagType )
            {
                case SSITAG_IF:
                    //Nest count is the conditional nesting stack pointer. It starts at -1
                    locNestCount ++;
                    if ( locNestCount >= MAX_NESTS )
                    {
                        //too many nests
                        DBG_ERR( "too many nested IF statements!!!\n" );
                        error = TRUE;
                        break;
                    }

                    //if output is enabled coming into an IF then we need to test the conditional to
                    //see if it should stay on
                    if ( outEnabled == TRUE )
                    {
                        pState->condRes[locNestCount-1] = HTMLDoConditional(pState, &parsedTag);

                        //evaluated to false so tell parser to dump output until next
                        //conditional
                        if ( pState->condRes[locNestCount-1] == FALSE )
                        {
                            DBG_VERBOSE( "IF disable output\n" );
                            pState->condOutEnabled[locNestCount-1] = FALSE;
                            outEnabled = FALSE;
                        }
                        else
                        {
                            DBG_VERBOSE( "IF enable output\n" );
                            pState->condOutEnabled[locNestCount-1] = TRUE;
                        }
                    }
                    //else output is already disabled so it will have to stay disabled for the
                    //entire contional (this can only happen if this a nested IF and the outer
                    //conditional evaluated to FALSE so all the nested stuff has to be false as well)
                    else
                    {
                        //set this to TRUE to prevent any ELSEIF's from resolving the conditional
                        //since output is disabled for the entire conditional construct
                        DBG_VERBOSE( "IF disable output (already disabled)\n" );
                        pState->condRes[locNestCount-1] = TRUE;
                        pState->condOutEnabled[locNestCount-1] = FALSE;
                        outEnabled = FALSE;
                    }
                    pState->nestCount = locNestCount;
                    break;

                case SSITAG_ELSEIF:
                    //if we previously hit a true condition then drop everything in the elseif, don't
                    //need to evaluate
                    if ( pState->condRes[locNestCount-1] == TRUE )
                    {
                        pState->condOutEnabled[locNestCount-1] = FALSE;
                        outEnabled = FALSE;
                        DBG_VERBOSE( "ELSEIF output disabled\n" );
                    }
                    else
                    {
                        //haven't hit true yet, evaluate this one
                        pState->condRes[locNestCount-1] = HTMLDoConditional(pState, &parsedTag);

                        //if still not true keep dropping output, else keep output enabled (default return value)
                        if ( pState->condRes[locNestCount-1] == FALSE )
                        {
                            pState->condOutEnabled[locNestCount-1] = FALSE;
                            outEnabled = FALSE;
                            DBG_VERBOSE( "ELSEIF disable output\n" );
                        }
                        else
                        {
                            DBG_VERBOSE( "ELSEIF enable output\n" );
                            pState->condOutEnabled[locNestCount-1] = TRUE;
                            outEnabled = TRUE;
                        }
                    }
                    break;

                case SSITAG_ELSE:
                    //if previous conditional was true, then dump this one
                    if ( pState->condRes[locNestCount-1] == TRUE )
                    {
                        pState->condOutEnabled[locNestCount-1] = FALSE;
                        outEnabled = FALSE;
                        DBG_VERBOSE( "ELSE output disabled\n" );
                    }
                    else
                    {
                        //else turn output on for this one (output on is default return value)
                        DBG_VERBOSE( "ELSE output enabled\n" );
                        pState->condOutEnabled[locNestCount-1] = TRUE;
                        outEnabled = TRUE;
                    }
                    break;
                case SSITAG_ENDIF:
                    //done with conditional reenable output
                    ASSERT(locNestCount > 0);
                    locNestCount --;

                    if ( locNestCount == 0 )
                    {
                        DBG_VERBOSE( "ENDIF output reenabled\n" );
                        outEnabled = TRUE;
                    }
                    else
                    {
                        if ( pState->condOutEnabled[locNestCount-1] == TRUE )
                        {
                            DBG_VERBOSE( "ENDIF nested output reenabled\n" );
                            outEnabled = TRUE;
                        }
                        else
                        {
                            DBG_VERBOSE( "ENDIF nested output, still disabled\n" );
                            outEnabled = FALSE;
                        }
                    }
                    pState->nestCount = locNestCount;
                    break;

                default:
                    break;
            }
        }
    }
    //else output disabled and not a conditional

    //REVISIT: if error write out some error message and write out the tag
    UNUSED_VAR( error );
    //write output here
    if ( (outStr != NULL) && (*outStr != '\0') && (outEnabled == TRUE) )
    {
        char *encodedStr = NULL;
#ifdef HAVE_USER_TEXT_FIELDS
        bool free_memory = false;
        encodedStr = xml_encode(outStr);
        if ( encodedStr != NULL )
        {
            free_memory = true;
        }
#else
        encodedStr = outStr;
#endif
        outLen = strlen(encodedStr);

        addDataToHttpContext( encodedStr, outLen, false, context, false );
#ifdef HAVE_USER_TEXT_FIELDS
        if ( free_memory == true )
        {
            MEM_FREE_AND_NULL(encodedStr);
        }
#endif

    }
    if ( oid_result != NULL )
    {
        MEM_FREE_AND_NULL( oid_result );
    }
    
    if ( outEnabled )
    {
        return HTTP_OUTPUT_ENABLE;
    }
    else
    {
        return HTTP_OUTPUT_DISABLE;
    }
}



/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/19/2004
 *
 *
 * Description: Get all the args, null terminate them
 * HTTP_GET Format:
 *             <!--#ssi tag arg1 arg2 arg3 arg4 -->
 *             <!--#ssi IDs strcode -->
 *             <!--#ssi IDr_op oid -->
 *             <!--#ssi IDc_op oid oper val output -->
 *             <!--#ssi IF_op oid oper val -->
 *
 *             op: _p oid is a password
 *                 _s val is a string code
 *                 _o val is an oid
 *
 * HTTP_POST Format:
 *             ID oid
 * 
 ******************************************************************************/
void HTMLGetArgs(SSIARGS* args, char* tag, uint8_t operation)
{
    char* pCur = tag;

    args->numArgs = 0;

    //scan until find #ssi
    if ( operation == HTTP_GET )
    {
        while ( *pCur != '#' && *pCur != 0 )
        {
            pCur ++;
        }

        //invalid tag return with no args
        if ( *pCur == 0 )
        {
            return;
        }
        //move index to right after the "ssi"
        pCur += 4;
    }

    while ( *pCur != 0 )
    {
        //skip leading white spaces
        while ( (*pCur == ' ') && (*pCur != 0) )
        {
            pCur ++;
        }

        //done if hit end of string
        if ( *pCur == 0 )
        {
            DBG_ERR( "Invalid Arg: %s\n", tag );
            return;
        }

        //if we find a quotation mark, the arg is all the chars until the next quotation mark, 
        //including white space
        if ( *pCur == '\"' )
        {
            //skip the quote
            pCur ++;
            args->arg[args->numArgs] = pCur;

            while ( (*pCur != '\"') && (*pCur != 0) )
            {
                pCur ++;
            }
        }
        //find next white space
        else
        {
            //save beginning of arg
            args->arg[args->numArgs] = pCur;

            while ( (*pCur != ' ') && (*pCur != 0) )
            {
                pCur ++;
            }
        }

        if ( *pCur != 0 )
        {
            //null terminate the tag and move on
            *pCur = 0;
            pCur ++;
        }
        args->numArgs ++;
    }

    //The code above counts the "-->" at the end of the tag as an arg so decr count
    //to get true value
    if ( operation == HTTP_GET )
    {
        args->numArgs --;
    }
    else
    {
        //The only valid tags for POST are ID.  See if this is an ID, we will also
        //process submit buttons and other stuff in this function, we want to reject
        //them by setting numargs to 0
        if ( args->numArgs > 0 )
        {
            if ( (args->arg[0][0] != 'I') || (args->arg[0][1] != 'D') )
            {
                args->numArgs = 0;
            }
        }
    }
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/19/2004
 *
 *
 * Description: Parse the args into the AGTAG struct for easier handling
 *
 *              option determines how much of the string we look at. For
 *              POPT_CMD we just see what type of tag it is (IDr, IDc, ID etc)
 *              for POPT_ALL we look at the whole string
 ******************************************************************************/
void HTMLParseTag(SSIARGS* args, SSITAG* parsedTag, PARSEOPTION option)
{
    uint32_t i;
    uint32_t len;
    char* tag;

    if ( (option == POPT_CMD) || (option == POPT_ALL) )
    {
        //start as invalid then mark with the cmd we find
        parsedTag->tagType = SSITAG_INVALID;

        //grab the command tag
        tag = args->arg[INDEX_CMD];

        //figure out length of tag excluding options
        for ( len = 0; (tag[len] != '_') && (tag[len] != 0); len ++ );

        //scan through the tag map and find out what type of tag this is
        for ( i = 0; i < NUM_TAGS; i++ )
        {
            if ( (gHtmlTags[i].mapStrLen == len) && 
                 (strncmp(gHtmlTags[i].mapStr, tag, len) == 0) )
            {
                parsedTag->tagType = gHtmlTags[i].mapType;
                break;
            }
        }

        if ( parsedTag->tagType == SSITAG_INVALID )
        {
            return;
        }
        //find any options the tag may have
        HTMLGetOptions(args->arg[INDEX_CMD], &parsedTag->options);
    }

    if ( option == POPT_ARGS || option == POPT_ALL )
    {
        uint32_t oidOffset;

        parsedTag->oid1 = 0;
        parsedTag->oid2 = 0;
        parsedTag->oid1Index = 0;
        parsedTag->oid2Index = 0;
        parsedTag->oper = OPER_NONE;
        parsedTag->output = NULL;
        parsedTag->val = NULL;
        parsedTag->numArgs = args->numArgs;

        //the oid or strcode is second arg for all tags with more 
        //than one arg (tag counts as one)
        if ( args->numArgs > 1 )
        {
            if ( (parsedTag->tagType == SSITAG_IDc) || 
                 (parsedTag->tagType == SSITAG_IF) ||
                 (parsedTag->tagType == SSITAG_ELSEIF) )
            {
                oidOffset = INDEX_COND_OID;
            }
            else
            {
                oidOffset = INDEX_IDx_OID;
            }

            //get the oid and oidindex - if the arg at the offset is a numerical value 
            if ( ( args->arg[ oidOffset ][ 0 ] >= '0' ) && ( args->arg[ oidOffset ][ 0 ] <= '9' ) )
            {
                HTMLGetOidInfo(args->arg[oidOffset], &parsedTag->oid1, &parsedTag->oid1Index);
            }
            parsedTag->oidType = oid_type_id_get( parsedTag->oid1 );

            switch ( parsedTag->tagType )
            {
                case SSITAG_IDc:
                    /*
                     * Fall through
                     */
                case SSITAG_IF:
                    /*
                     * Fall through
                     */
                case SSITAG_ELSEIF:
                    //Basic contional format: <!--#ssi cond oper oid val -->
                    //for conditionals, scan through the operator map and find what operation this is
                    for ( i = 0; i < NUM_OPERS; i ++ )
                    {
                        if ( strcmp(gHtmlOpers[i].mapStr, args->arg[INDEX_COND_OPER]) == 0 )
                        {
                            parsedTag->oper = gHtmlOpers[i].mapType;
                            break;
                        }
                    }

                    //For _o and _s options, the val part of the conditional is an oid or string code
                    //so convert it to decimal for easier resolutions
                    if ( parsedTag->options & (SSIOPTION_O | SSIOPTION_S) )
                    {
                        HTMLGetOidInfo(args->arg[INDEX_COND_VAL], &parsedTag->oid2, &parsedTag->oid2Index);
                    }
                    else
                    {
                        //else val is a string
                        parsedTag->val = args->arg[INDEX_COND_VAL];
                    }

                    //IDc format: <!--#ssi cond oper oid val output -->
                    //save the output string
                    if ( parsedTag->tagType == SSITAG_IDc )
                    {
                        parsedTag->output = args->arg[INDEX_COND_OUTPUT];
                    }
                    break;
                default:
                    break;
            }
        }
    }
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
BOOL HTMLDoConditional(HTMLSTATE* htmlState, SSITAG* parsedTag)
{
    //types of compares we have to support:
    //    1. oid to oid  integer  => compare the ints 
    //    2. oid to oid  string   => resolve both oids to strings and do string compare
    //    3. oid to val  integer  => convert val to integer, do integer compare
    //    4. oid to val  string   => resolve oid to string, do string compare
    //    5. oid to strenum       => resolve oid to string and strenum to string and do string compare
    //
    //Tags we will see here:
    //    <!--#ssi IDc_op oid oper val output -->
    //    <!--#ssi IF_op oid oper val -->
    //
    //    IDc   => this is a #3 or #4 depending on the oid's type
    //    IDc_o => this is a #1 or #2 depending on the oid's type (_o means val is an oid not an ascii const)
    //    IDc_s => this is a #5   (_s means val is a strcode not an ascii const)
    //    IF    => same as IDc
    //    IF_o  => same as IDc_o
    //    IF_s  => same as IDc_s
    BOOL result;
    char *oidVal1 = NULL;
    char *oidVal2 = NULL;
    error_type_t res1 = FAIL;
    error_type_t res2 = FAIL;
    SINT32 cmpRes;
    uint32_t i;

    result = FALSE;

    //first evaluate the err condition
    if ( parsedTag->oper == OPER_ERR )
    {
        //Two types of IF ERR to look for:
        //  <!--#ssi IF ERR -->       :this one is true if any errors
        //  <!--#ssi IF ERR oid -->   :this one true only if set on oid had an error
        if ( parsedTag->numArgs == 2 )
        {
            if ( htmlState->oidErrCount > 0 )
            {
                DBG_VERBOSE( "if ERR is true, count %ld\n", htmlState->oidErrCount );
                result = TRUE;
            }
        }
        else
        {
            //Check the list of oids and indices that had errors in them looking
            //for a match
            for ( i = 0; i < htmlState->oidErrCount; i ++ )
            {
                if ( ( htmlState->oidErrList[i] == parsedTag->oid1 ) &&
                     ( htmlState->oidIndexErrList[i] == parsedTag->oid1Index ) )
                {
                    result = TRUE;
                    break;
                }
            }
        }
    }
    else
    {
        if ( parsedTag->options & SSIOPTION_O )
        {
            if ( parsedTag->oidType == OID_TYPE_STRING )
            {
                //#2 - convert both to strings and do string compare
                oidVal1 = (char*)MEM_MALLOC_LIMITED(mlimiter_by_name("html"), OID_MAX_STRING_SIZE_IN_BYTES );
                oidVal2 = (char*)MEM_MALLOC_LIMITED(mlimiter_by_name("html"), OID_MAX_STRING_SIZE_IN_BYTES );
                
                if(oidVal1 != NULL && oidVal2 != NULL)
                {
                    res1 = oid_get_string(parsedTag->oid1, parsedTag->oid1Index, oidVal1, OID_MAX_STRING_SIZE_IN_BYTES);  
                    res2 = oid_get_string(parsedTag->oid2, parsedTag->oid2Index, oidVal2, OID_MAX_STRING_SIZE_IN_BYTES); 
        
                    if ( res1 == OK && res2 == OK )
                    {
                        cmpRes = strcmp(oidVal1, oidVal2);
                        result = HTMLTestCond(cmpRes, parsedTag->oper);
                    }
                    else
                    {
                        if ( res1 != OK )
                        {
                            DBG_ERR( "DoConditional oid option string failed to resolve oid1 %#08X\n", parsedTag->oid1 );
                        }
                        if ( res2 != OK )
                        {
                            DBG_ERR( "DoConditional oid option string failed to resolve oid2 %#08X\n", parsedTag->oid2 );
                        }
                    }
                }
                MEM_FREE_AND_NULL(oidVal1);
                MEM_FREE_AND_NULL(oidVal2);
            }
            else
            {
                uint32_t val1;
                uint32_t val2;

                res1 = oid_get_uint32(parsedTag->oid1, parsedTag->oid1Index, &val1);  
                res2 = oid_get_uint32(parsedTag->oid2, parsedTag->oid2Index, &val2); 
                if ( res1 == OK && res2 == OK )
                {
                    if ( parsedTag->oper == OPER_AND )
                    {
                        if ( ( val1 & val2 ) == val2)
                        {
                            result = TRUE;
                        }
                    }
                    else if ( parsedTag->oper == OPER_NAND )
                    {
                        if ( ( ~val1 & val2 ) == val2 )
                        {
                            result = TRUE;
                        }
                    }
                    else
                    {
                        if ( val1 == val2 )
                        {
                            cmpRes = 0;
                        }
                        else if ( val1 > val2 )
                        {
                            cmpRes = 1;
                        }
                        else
                        {
                            cmpRes = -1;
                        }
                        result = HTMLTestCond( cmpRes, parsedTag->oper );
                    }
                }
                else
                {
                    if ( res1 != OK )
                    {
                        DBG_ERR( "DoConditional oid option int failed to resolve oid1 %#08X\n", parsedTag->oid1 );
                    }
                    if ( res2 != OK )
                    {
                        DBG_ERR( "DoConditional oid option int failed to resolve oid2 %#08X\n", parsedTag->oid2 );
                    }
                }
            }
        }
        else if ( parsedTag->options & SSIOPTION_S )
        {
            //#5 - resolve strcode to string do string compare
            char* tmpStr1;
            const char* tmpStr2;

            oidVal1 = (char*)MEM_MALLOC_LIMITED( mlimiter_by_name("html"), OID_MAX_STRING_SIZE_IN_BYTES );
            if(oidVal1 != NULL)
            {
                res1 = oid_get_string(parsedTag->oid1, parsedTag->oid1Index, oidVal1, OID_MAX_STRING_SIZE_IN_BYTES);
            }
            if ( res1 == OK )
            {
                tmpStr1 = oidVal1;

                //convert the string enum to a string manager code
                tmpStr2 = string_mgr_get_string(parsedTag->oid2, string_mgr_loc_full);

                if ( tmpStr1 != NULL && tmpStr2 != NULL )
                {
                    cmpRes = strcmp(tmpStr1, tmpStr2);

                    result = HTMLTestCond(cmpRes, parsedTag->oper);
                }
                else
                {
                    DBG_ERR( "DoConditional strcode option failed to resolve strcode %#08X\n", parsedTag->oid2 );
                }
            }
            else
            {
                DBG_ERR( "DoConditional strcode option failed to resolve oid1 %#08X\n", parsedTag->oid1 );
            }
            MEM_FREE_AND_NULL(oidVal1);
        }
        else
        {
            if ( parsedTag->oidType == OID_TYPE_STRING )
            {
                //#4
                SINT32 cmpRes;

                oidVal1 = (char*)MEM_MALLOC_LIMITED(mlimiter_by_name("html"), OID_MAX_STRING_SIZE_IN_BYTES );
                if(oidVal1 != NULL)
                {
                    res1 = oid_get_string(parsedTag->oid1, parsedTag->oid1Index, oidVal1, OID_MAX_STRING_SIZE_IN_BYTES);  
                }

                if ( res1 == OK )
                {
                    if ( !(strcmp(parsedTag->val,"NULL")) )
                    {
                        *(parsedTag->val) = 0;
                    }

                    cmpRes = strcmp(oidVal1, parsedTag->val);
                    result = HTMLTestCond(cmpRes, parsedTag->oper);
                    DBG_VERBOSE( "oidVal1: %s - parsedTag: %s, cmpRes: %d, operation: %d\n",
                                 oidVal1,
                                 parsedTag->val,
                                 cmpRes,
                                 parsedTag->oper );
                }
                else
                {
                    DBG_ERR( "DoConditional string failed to resolve oid1 %#08X\n", parsedTag->oid1 );
                }

                MEM_FREE_AND_NULL(oidVal1);
            }
            else
            {
                uint32_t temp;
                //#3 
                uint32_t val1;

                res1 = oid_get_uint32(parsedTag->oid1, parsedTag->oid1Index, &val1);  
                if ( res1 == OK )
                {
                    temp = atoi(parsedTag->val);

                    if ( parsedTag->oper == OPER_AND )
                    {
                        if ( (val1 & temp) == temp )
                        {
                            result = TRUE;
                        }
                    }
                    else
                    {
                        if ( parsedTag->oper == OPER_NAND )
                        {
                            if ( (~val1 & temp) == temp )
                            {
                                result = TRUE;
                            }
                        }
                        else
                        {
                            if ( val1 == temp )
                            {
                                cmpRes = 0;
                            }
                            else
                            {
                                if ( val1 > temp )
                                {
                                    cmpRes = 1;
                                }
                                else
                                {
                                    cmpRes = -1;
                                }
                            }
                            result = HTMLTestCond(cmpRes, parsedTag->oper);
                        }
                    }
                }
                else
                {
                    DBG_ERR( "DoConditional int failed to resolve oid1 %#08X\n", parsedTag->oid1 );
                }
            }
        }
    }

    DBG_VERBOSE( "%s exit result: %d\n", __FUNCTION__, result );

    return result;
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
BOOL HTMLTestCond(SINT32 cmpRes, SSIOPER tagOper)
{
    BOOL res;
    res = FALSE;

    switch ( tagOper )
    {
        case OPER_EQ: 
            if ( cmpRes == 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_NE:
            if ( cmpRes != 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_GT:
            if ( cmpRes > 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_LT:
            if ( cmpRes < 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_GTE:
            if ( cmpRes >= 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_LTE:
            if ( cmpRes <= 0 )
            {
                res = TRUE;
            }
            break;
        case OPER_ERR:
            //REVISIT: need to implement this
            break;
        default:
            DBG_ERR( "unknown oper: %ld \n", tagOper );
            break;
    }
    return res;
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
void HTMLGetOptions(char* tagStr, uint32_t* options)
{
    *options = 0;

    while ( (*tagStr != 0) && 
            (*tagStr != '_') )
    {
        tagStr ++;
    }

    while ( *tagStr != 0 )
    {
        switch ( *tagStr )
        {
            case 'p':
                *options |= SSIOPTION_P;
                break;
            case 'o':
                *options |= SSIOPTION_O;
                break;
            case 's':
                *options |= SSIOPTION_S;
                break;
            case 'h':
                *options |= SSIOPTION_H;
                break;
            case 'b':
                *options |= SSIOPTION_B;
                break;
            case 'd':
                *options |= SSIOPTION_D;
                break;
        }
        tagStr ++;
    }
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description: Convert ascii oid to decimal
 *              oid can be:  n.m where n is the oid and m is the index or
 *              it can be just n
 *
 ******************************************************************************/
void HTMLGetOidInfo(char* oidStr, uint32_t* oid, uint32_t* index)
{
    SINT32 len;
    SINT32 i;
    uint32_t tmp;
    uint32_t mult;
    char ch;

    mult = 1;

    len = strlen(oidStr);
    *oid = 0;
    *index = 0;
    tmp = 0;

    //Start at the right of the string and work our way left
    //mult is set to 1 and then multiplied by 10 for each ascii digit we find
    //we only support decimal so can subtract hex 30 to get integer
    for ( i = len-1; len > 0; len --, i -- )
    {
        ch = oidStr[i];
        
        // if we find a period, we have been calculating the index, save it
        // and start over for the oid.  If no period, the entire value is
        // the oid
        if ( ( ( ch < '0') || ( ch > '9' ) ) && ( ch != '.' ) )
        {
            DBG_ERR( "Invalid characters in oid value: %s\n", oidStr );
            break;
        }
        if ( ch == '.' )
        {
            *index = tmp;
            tmp  = 0;
            mult = 1;
        }
        else
        {
            tmp  += (ch - 0x30) * mult;
            mult *= 10;
        }
    }

    *oid = tmp;

    DBG_VERBOSE( "oidStr: %s --> oid: %d -- index: %d\n", oidStr, *oid, *index );

}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description: allocate the structure needed to keep state on this 
 *              connection. The state is only good for either a single
 *              HTTP_GET or for a pair HTTP_POST, HTTP_GET. On posts
 *              we give the server a URL of a page to get with the
 *              results of the post. The results are stored in the
 *              state structure. Some browsers leave the connection
 *              open and will do multiple operations over it. We
 *              need to detect and clear the state and start over on
 *              these.
 *
 *              We save what operation caused us to create the state
 *              and how many GETs and POSTs have been done on it.
 *
 *              We get called once per variable on Gets and once per page on
 *              Posts. On Gets we get a start page call at the beginning of the
 *              page. We set the newPage flag on this.
 *
 *              Scenarios for operations on same connection:
 *              1. GET - create state with GET, cnt = 1
 *                 GET - delete state and recreate with GET, cnt = 1
 *              2. GET - create state with GET, cnt = 1
 *                 POST - delete state and recreate with POST, cnt=1
 *              3. POST - create state with POST, cnt = 1
 *                 GET  - increment cnt to 2
 *                 GET  - delete state and recreate with GET, cnt = 1
 *              4. POST - create state with POST, cnt = 1
 *                 POST - delete state and recreate with POST, cnt = 1
 *
 *              NOTE: ca 10/19/2004 - the above scenarios are the ideal
 *              It turns out I can't currently tell when I am starting
 *              a new GET request. I can only tell a new POST request.
 *              I want to make sure we don't have any error variables
 *              in the state variable from a previous post when we
 *              display a results page. Since we kill the state every
 *              time we get another POST this will work. For now I
 *              am leaving the code in hoping to come up with a way
 *              to detect GETs so I can handle the scenarios correctly
******************************************************************************/
HTMLSTATE* HTMLGetState(HttpPageContext *context, uint8_t httpType)
{

    uint8_t i;
    HTMLSTATE* pState;
    http_config_t http_config;

    pState = NULL;

    if ( OK != http_config_get( &http_config ) ) 
    {
        DBG_ERR( "%s: Failed to get http config\n", __FUNCTION__ );
        return pState;
    }

    //do init if haven't done yet
    if ( gHtmlData == NULL )
    {
        gHtmlData = MEM_MALLOC( sizeof( HTMLDATA ) * http_config.max_active_http_connections );
        if ( gHtmlData == NULL )
        {
            DBG_ERR( "Failed to get memory for gHtmlData\n" );
            return NULL;
        }
        for ( i = 0; i < http_config.max_active_http_connections; i ++ )
        {
            gHtmlData[i].data = NULL;
        }
    }

    //see if already have a pstate for this conHandle
    for ( i = 0; i < http_config.max_active_http_connections; i ++ )
    {
        if ( gHtmlData[i].data != NULL && gHtmlData[i].context == context )
        {
            pState = gHtmlData[i].data;
            break;
        }
    }

    //test the scenarios above and see if we need to delete the state struct
    if ( (pState != NULL) && 
         ((httpType == HTTP_GET && pState->newpage == TRUE) ||
          (httpType == HTTP_POST)) )
    {
        pState->newpage = FALSE;

        //if we get to here the state structure was created from a previous
        //operation. Test the scenarios based on the current operation
        if ( pState->creationType == HTTP_GET )
        {
            //scenarios #1 and #2 - delete the state and start new
            DBG_MSG( "multiple get/post, recreate state\n" );
            HTMLReleaseState(context);
            pState = NULL;
        }
        else
        {
            //HTTP_POST
            //state was created with a post, see if this is a scenario 3 or 4
            if ( httpType == HTTP_GET )
            {
                //scenario #3
                pState->opCount ++;

                if ( pState->opCount > 2 )
                {
                    //more than one GET after the POST, delete and recreate
                    HTMLReleaseState(context);
                    DBG_MSG( "multiple gets after post, recreate state\n" );
                    pState = NULL;
                }
            }
            else
            {
                //HTTP_POST
                //scenario #4 - delete state and start over
                HTMLReleaseState(context);
                DBG_MSG( "post after post, recreate state\n" );
                pState = NULL;
            }
        }
    }

    //didn't find it, allocate and initialize
    if ( pState == NULL )
    {
        pState = (HTMLSTATE*)MEM_MALLOC_LIMITED( mlimiter_by_name("html"), sizeof(HTMLSTATE) );
        ASSERT(pState != NULL);

        //allocate state ptr and initialize
        //put in the user ptr in conHandle struct
        DBG_MSG( "start ssi page handle: %#X\n", context );


        //creating a new state struct, save operation and count so we
        //can detect the scenarios listed above
        pState->newpage = FALSE;
        pState->creationType = httpType;
        pState->opCount = 1;
        for ( i = 0; i < MAX_NESTS; i ++ )
        {
            //by default output is enabled
            pState->condRes[i] = TRUE;
            pState->condOutEnabled[i] = TRUE;
        }

        pState->nestCount = 0;
        pState->oidErrCount = 0;
        pState->hiddenCount = 0;

        // Initialize to max value to indicate if entry has been used yet
        for ( i = 0; i < MAX_HIDDEN; i++ )
        {
            pState->hiddenOids[ i ].index = -1;
        }

        //find an empty space for it
        for ( i = 0; i < http_config.max_active_http_connections; i ++ )
        {
            if ( gHtmlData[i].data == NULL )
            {
                gHtmlData[i].data = pState;
                gHtmlData[i].context = context;
                break;
            }
        }
        ASSERT(i < http_config.max_active_http_connections);
    }

    return pState;
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/20/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
void HTMLReleaseState(HttpPageContext *context)
{
    uint8_t i;
    http_config_t http_config;

    if ( OK != http_config_get( &http_config ) ) 
    {
        DBG_ERR( "%s: Failed to get http config\n", __FUNCTION__ );
        return;
    }
    
    // check to see if gHtmlData has been initialized
    if ( gHtmlData == NULL )
    {
        DBG_ERR( "%s: gHtmlData is NULL!\n", __FUNCTION__ );
        return;
    }

    for ( i = 0; i < http_config.max_active_http_connections; i ++ )
    {
        if ( gHtmlData[i].data != NULL && gHtmlData[i].context == context )
        {
            MEM_FREE_AND_NULL(gHtmlData[i].data);
            gHtmlData[i].data = NULL;
        }
    }
    // not sure if I can do this revisit !
//    mlimiter_stop(mlimiter_by_name("html"));
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 5/26/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
void HttpCgiHandler(HttpPageContext *context, char *pathPtr, char *argNames[], 
                    char *argValues[], int argCnt)
{
    uint32_t i;
    SSIARGS args;
    SSITAG parsedTag;
    char  *oid_val_str;
    uint32_t oid_val_uint32;
    error_type_t res;
    char* tagBuff;
    int passwordIndex = -1;
    HTMLSTATE* pState;
    oid_val_uint32 = 0;

    DBG_MSG( "start post processing context: %#X,  path: %s\n", context, pathPtr );

    pState = HTMLGetState(context, HTTP_POST);
    ASSERT(pState != NULL);

    oid_val_str= (char*)MEM_MALLOC_LIMITED( mlimiter_by_name("html"), OID_MAX_STRING_SIZE_IN_BYTES );
    ASSERT(oid_val_str != NULL);
    tagBuff = (char*)MEM_MALLOC_LIMITED( mlimiter_by_name("html"), HTTP_MAX_SSI_TAG );
    ASSERT(tagBuff != NULL);

    //process each of the name value pairs
    for ( i = 0; i < argCnt; i ++ )
    {
        res = OK;
        //copy tag into tagBuff so we can parse it and put nulls in it
        strcpy(tagBuff, argNames[i]);
        HTMLGetArgs(&args, tagBuff, HTTP_POST);

        if ( args.numArgs > 0 )
        {
            HTMLParseTag(&args, &parsedTag, POPT_ALL);
            // Substitute connection handle for oid index
            if ( parsedTag.oid1Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
            {
                parsedTag.oid1Index = (uint32_t)context->id;
            }
            if ( parsedTag.oid2Index == HTTP_USE_REQUEST_HANDLE_AS_INDEX )
            {
                parsedTag.oid2Index = (uint32_t)context->id;
            }

            //only process id tags
            if ( parsedTag.tagType == SSITAG_ID )
            {
                BOOL doSet = TRUE;
                int32_t hiddenIndex;

                DBG_VERBOSE( "post oid: %ld\n", parsedTag.oid1 );
                if ( parsedTag.oidType == OID_TYPE_UINT32 )
                {
                    uint32_t len;
                    uint32_t j;
                    len = strlen( argValues[ i ] );
                    j = 0;
                    // skip leading whitespace and + - characters -jrs
                    while( j < len && ( argValues[ i ][ j ] == ' ' || argValues[ i ][ j ] == '+' || argValues[ i ][ j ] == '-' ) )
                    {
                        j++;
                    }
                    for ( ; j < len; j++)
                    {
                        if (argValues[ i ][j] < '0' || argValues[ i ][j] > '9')
                        {
                            res = OID_ERROR_ILLEGAL_CHARS;
                            break;
                        }
                    }
                    if ( res == OID_ERROR_ILLEGAL_CHARS ) 
                    {
                        DBG_ERR( "Illegal Chars in value: oid 0x%08X - value: '%s'\n", parsedTag.oid1, argValues[i] );
                        pState->oidErrList[ pState->oidErrCount ] = parsedTag.oid1;
                        pState->oidIndexErrList[ pState->oidErrCount ] = parsedTag.oid1Index;
                        pState->oidErrCount++;
                        continue;
                    }
                    oid_val_uint32 = strtoul(argValues [ i ], NULL, BASE_10);
                    //oid_val_uint32 = atoi ( argValues [ i ] );
                }
                else
                {
                    strcpy( oid_val_str, argValues [ i ] );
                }

                //Check options here.
                if ( parsedTag.options & SSIOPTION_P )
                {
                    //Check password. There will always be two passwords in the POST, one for
                    //the new password and one for the verify. If the passwords arent' equal
                    //to each other we need to mark it as an error, if they are equal and
                    //aren't equal to the dummy password then we set them
                    if ( passwordIndex == -1 )
                    {
                        //save the first password we find
                        passwordIndex = i; 
                        doSet = FALSE;
                    }
                    else
                    {
                        //this is the second password so compare them
                        if ( strcmp(argValues[i], argValues[passwordIndex]) == 0 )
                        {
                            //They match, see if they are both the dummy password
                            //if so then don't set them (this means the user didn't
                            //modify them)
                            if ( strcmp(argValues[i], DUMMY_PASSWORD) == 0 )
                            {
                                doSet = FALSE;
                            }
                        }
                        else
                        {
                            DBG_ERR( "Password doesn't match: oid 0x%08X - value: %s\n", parsedTag.oid1, argValues[i] );
                            //don't match, mark as error
                            pState->oidErrList[pState->oidErrCount] = parsedTag.oid1;
                            pState->oidIndexErrList[pState->oidErrCount] = parsedTag.oid1Index;
                            pState->oidErrCount++;
                            doSet = FALSE;
                        }
                        // set back to -1 so another password on the page will work. 
                        passwordIndex = -1;
                    }
                    
                }
                else if ( parsedTag.options & SSIOPTION_H )
                {
                    //save the initial value for the hidden oid
                    DBG_MSG( "start hidden oid: %08X\n", parsedTag.oid1 );
                    HtmlStartHidden( pState, parsedTag.oid1, (int32_t)parsedTag.oid1Index, oid_val_uint32 );
                    doSet = FALSE;
                }
                else if ( parsedTag.options & SSIOPTION_B )
                {
                    //bitmask option means OR the value into the initial value of the hidden
                    hiddenIndex = HtmlGetHiddenIndex(pState, parsedTag.oid1, (int32_t)parsedTag.oid1Index );
                    if ( hiddenIndex == -1 )
                    {
                        DBG_ERR( "Trying to bitmask in value but failed to start hidden segement\n" );
                    }
                    else
                    {
                        pState->hiddenOids[hiddenIndex].val |= oid_val_uint32;
                        DBG_MSG( "_b update hidden oid: %08X, val:%lx\n", parsedTag.oid1, oid_val_uint32 );
                    }
                    doSet = FALSE;
                }
                else if ( parsedTag.options & SSIOPTION_D )
                {
                    //done option means done with hidden, save its current value
                    hiddenIndex = HtmlGetHiddenIndex(pState, parsedTag.oid1, (int32_t)parsedTag.oid1Index );
                    if ( hiddenIndex == -1 )
                    {
                        DBG_ERR( "Trying to end hidden segement that does not exist\n" );
                    }
                    else
                    {
                        oid_val_uint32 = pState->hiddenOids[hiddenIndex].val;
                        HtmlEndHidden( pState, parsedTag.oid1, (int32_t)parsedTag.oid1Index );
                        DBG_MSG( "_d set hidden oid: %08X, val:%lx\n", parsedTag.oid1, oid_val_uint32 );
                    }
                } 
                //if have a hidden oid, see if this is an update for it
                else if ( pState->hiddenCount > 0 )
                {
                    hiddenIndex = HtmlGetHiddenIndex(pState, parsedTag.oid1, (int32_t)parsedTag.oid1Index );
                    if ( ( hiddenIndex >= 0 ) && ( hiddenIndex < pState->hiddenCount ) )
                    {
                        DBG_MSG( "update hidden oid: %08X, val:%lx\n", parsedTag.oid1, oid_val_uint32 );
                        pState->hiddenOids[hiddenIndex].val = oid_val_uint32;
                        doSet = FALSE;
                    }
                } 
 
                if ( doSet == TRUE )
                {
                    //set the value
                    switch ( parsedTag.oidType )
                    {
                        case OID_TYPE_UINT32:
                            DBG_VERBOSE( "Set uint32 - oid: %08X.%d - value: %d\n", parsedTag.oid1, parsedTag.oid1Index, oid_val_uint32 );
                            res = oid_set_uint32(parsedTag.oid1, parsedTag.oid1Index, oid_val_uint32 ); 
                            break;
                            
                        case OID_TYPE_STRING:
                            DBG_VERBOSE( "Set string - oid: %08X.%d - value: %s\n", parsedTag.oid1, parsedTag.oid1Index, oid_val_str );
                            res = oid_set_string(parsedTag.oid1, parsedTag.oid1Index, oid_val_str, strlen( oid_val_str ) ); 
                            break;
                        default:
                            ASSERT( 0 );
                    }
                    
                    //here do error handling, if bad value we need to serve up the orginal page
                    //with error fields set
                    if ( res != OK )
                    {
                        DBG_ERR( "post failed on %#08X\n", parsedTag.oid1 );
                        pState->oidErrList[pState->oidErrCount] = parsedTag.oid1;
                        pState->oidIndexErrList[pState->oidErrCount] = parsedTag.oid1Index;
                        pState->oidErrCount ++;
                    }
                }
            }
            else
            {
                DBG_ERR( "unexpected tag in post %s\n", argNames[i] );
            }
        }
    }

    DBG_MSG( "end post processing, send result file: %s\n", pathPtr );
    if ( getContextInfoForFilename( pathPtr, context, false ) == false )
    {
        DBG_ERR( "post response file failed\n" );
    }
    MEM_FREE_AND_NULL( oid_val_str );
    MEM_FREE_AND_NULL( tagBuff );
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 6/7/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
void HtmlStartHidden(HTMLSTATE* pState, oid_t oid, int32_t index, uint32_t val)
{
    uint32_t i;

    for ( i = 0; i < MAX_HIDDEN; i ++ )
    {
        if ( pState->hiddenOids[ i ].index == -1 )
        {
            pState->hiddenOids[i].oid   = oid;
            pState->hiddenOids[i].index = index;
            pState->hiddenOids[i].val   = val;
            break;
        }
    }

    pState->hiddenCount++;
    ASSERT(pState->hiddenCount < MAX_HIDDEN);
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 6/7/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
int32_t HtmlGetHiddenIndex( HTMLSTATE* pState, oid_t oid, int32_t index )
{
    int32_t i;

    for ( i = 0; i < MAX_HIDDEN; i ++ )
    {
        if ( ( pState->hiddenOids[ i ].oid == oid ) &&
             ( pState->hiddenOids[ i ].index == index ) )
        {
            return i;
        }
    }
    return -1;
}


/******************************************************************************
 * Function Name:
 * Created by: Cory Atkin 6/7/2004
 *
 *
 * Description:
 *
 ******************************************************************************/
int32_t HtmlEndHidden( HTMLSTATE* pState, oid_t oid, int32_t index )
{
    int32_t i;

    for ( i = 0; i < MAX_HIDDEN; i ++ )
    {
        if ( ( pState->hiddenOids[ i ].oid == oid ) &&
             ( pState->hiddenOids[ i ].index == index ) )
        {
            pState->hiddenOids[ i ].index = -1;
            pState->hiddenCount--;
            break;
        }
    }
    return -1;
}

/******************************************************************************
 * Function Name: xml_encode
 * Created by: Dan Scofield 10/22/2007
 *
 * Description:  Replaces characters that are reserved by the XML standard with
 *               the correct encoding.  EX: "Me & you" becomes "Me &amp; you"
 *
 * Note: The returned buffer is a new memory allocation and must be freed by the
 *       caller.
 *
 ******************************************************************************/
char *xml_encode(char *str)
{
    int len = strlen(str);
    int subLen;
    int i, j;
    char *newSubString;
    char needsReplacing;
    char *encoded;
    int newLen;

    // First, figure out how long the encoded string will be.
    newLen = len;
    for ( i = 0; i < len; i++ )
    {
        switch ( str[i] )
        {
            case '<': newLen += 3; break;
            case '>': newLen += 3; break;
            case '&': newLen += 4; break;
            case '%': newLen += 4; break;
            case '"': newLen += 5; break;
            default : break;
        }
    }

    // Allocate a big enough buffer and copy characters while replacing the ones
    // that need to be encoded.

    encoded = (char *)MEM_MALLOC( newLen + 1 );
    if ( encoded == NULL )
    {
        return NULL;
    }

    j = 0;
    for ( i = 0; i < len; i++ )
    {
        needsReplacing = TRUE;
        switch ( str[i] )
        {
            case '<': newSubString = "&lt;";  break;
            case '>': newSubString = "&gt;";  break;
            case '&': newSubString = "&amp;"; break;
            case '%': newSubString = "&#37;"; break;
            case '"': newSubString = "&quot;"; break;
            default : needsReplacing = FALSE; break;
        }
        if ( needsReplacing )
        {
            subLen = strlen(newSubString);
            memcpy(encoded + j, newSubString, subLen);
            j += subLen;
        }
        else
        {
            encoded[j] = str[i];
            j++;
        }
    }
    encoded[newLen] = 0;

    return encoded;
}
/*
 * End of htmlssi.c
 */

