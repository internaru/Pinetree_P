/***********************************************************
* (c) Copyright 2008 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

/******************************************************************************
 *
 * Description:  Generate the pjl status.
 *
 *****************************************************************************/
 
 
#include "agRouter.h"
#include "ATypes.h"
#include "agConnectMgr.h"
#include "logger.h"
#include "dprintf.h"
#include "agMessage.h"
#include "print_job_types.h"
#include "memAPI.h"
#include "lassert.h"
#include <string.h>
#include "print_events_common_api.h"
#include "pjl_extra_api.h"


#define AT_PJL "@PJL USTATUS"
#define cJOB "JOB"
#define cSTART "START"
#define cEND "END"
#define cNAME "NAME=\""
#define cDEVICE "DEVICE"
#define cCODE "CODE="
#define cTIMED  "TIMED"
#define QUOTE "\""
#define cRESULT "RESULT="
#define cOK "OK"
#define cCANCELLED "CANCELLED"
#define cPAGE "PAGES="
#define cPARSEERROR "PARSE ERROR"
#define cCRLF          "\r\n"
#define cFF            "\f"
#define cWS " "

void pjl_output_string(IOReg *Pipe, char *outputString)
{
    char *data_ptr;

    data_ptr = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), strlen(outputString) + 1);
    strcpy(data_ptr, outputString);                     // put the string into the data buffer.

    Pipe->WriteConnect(Pipe, data_ptr, strlen(data_ptr));
    memFree(outputString);
}

void pjl_cmd_ustatus_job_end(IOReg *Pipe, char *Name, uint32_t Pages, uint32_t JobStatus)
{
    char *pjlLine;

    pjlLine = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), 256);
    ASSERT(pjlLine);

    strcpy(pjlLine,AT_PJL);
    strcat(pjlLine,cWS);
    strcat(pjlLine,cJOB);
    strcat(pjlLine,cCRLF);
    strcat(pjlLine,cEND);
    strcat(pjlLine,cCRLF);
    if(strlen(Name) != 0)
    {
        strcat(pjlLine,cNAME);
        strcat(pjlLine, "\"");
        strcat(pjlLine, Name);
        strcat(pjlLine,"\"");
        strcat(pjlLine, cCRLF);
    }
    strcat(pjlLine,cRESULT);
    switch(JobStatus)
    {
        case JOBOK:
            strcat(pjlLine, cOK);
            break;
        case PARSEERROR:
            strcat(pjlLine,cPARSEERROR); 
            break;   
        case CANCELLED:
            strcat(pjlLine, cCANCELLED);
            break;
    }
    strcat(pjlLine,cCRLF);
    strcat(pjlLine,cPAGE);
    minSprintf(&pjlLine[strlen(pjlLine)],"%d",Pages);

    strcat(pjlLine,cCRLF);
    strcat(pjlLine,cFF);

    Pipe->WriteConnect(Pipe, pjlLine, strlen(pjlLine));
//    pjl_output_string(Pipe, pjlLine);
}


void pjl_cmd_ustatus_page(IOReg *Pipe, uint32_t PageNumber)
{
    char *pjlLine;

    pjlLine = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), 256);

    strcpy(pjlLine,AT_PJL);
    strcat(pjlLine,cWS);
    strcat(pjlLine,cPAGE);
    strcat(pjlLine,cCRLF);
    minSprintf(&pjlLine[strlen(pjlLine)],"%d",PageNumber);
    DPRINTF(DBG_SOFT|DBG_OUTPUT, ("PAGE NUMBER %d\n",PageNumber));
    strcat(pjlLine,cCRLF);
    strcat(pjlLine,cFF);
    pjl_output_string(Pipe, pjlLine);
}

void pjl_cmd_ustatus_start_job(IOReg *Pipe, char *Name)
{
    char *pjlLine;

    pjlLine = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), 256);
    ASSERT(pjlLine); 

    strcpy(pjlLine,AT_PJL);
    strcat(pjlLine,cWS);
    strcat(pjlLine,cJOB);
    strcat(pjlLine,cCRLF);
    strcat(pjlLine,cSTART);
    strcat(pjlLine,cCRLF);
    if(strlen(Name) != 0)
    {
        strcat(pjlLine,cNAME);
        strcat(pjlLine,"\"");
        strcat(pjlLine, Name);
        strcat(pjlLine,"\"");
        strcat(pjlLine, cCRLF);
    }
    strcat(pjlLine,cFF);

    pjl_output_string(Pipe, pjlLine);
}
/**
 * \brief Output a pjl string either device or timed based on deviceStatus.
 * \param[in] Pipe The io pipe to send the data to.
 * \param[in] statusCode The status to map into pjl status and send.
 * \param[in] deviceStatus true is this is a device request, false it is timed.
 */

void pjl_cmd_ustatus_device(IOReg *Pipe, uint32_t statusCode, bool deviceStatus)
{
    char *pjlLine;

    pjlLine = MEM_MALLOC_LIMITED(mlimiter_by_name("sys_retry_forever"), 256);

    //we don't want to send device status for unknown codes
    //we don't map all status codes to pjl messages
    UINT32 tmpStatus = map_print_status_to_pjl(statusCode);
    DPRINTF(LOG_ERR,("%s Map %d & Out %d \n", __FUNCTION__, statusCode, tmpStatus));	// 2014.02.21 	by Juny */
    if(tmpStatus == 0)
    {
        if(deviceStatus)
        {
            memFree(pjlLine);
            return;
        }
        tmpStatus = 10001;      // on line status
    }

    strcpy(pjlLine,AT_PJL);
    strcat(pjlLine,cWS);
    if(deviceStatus)
        strcat(pjlLine,cDEVICE);
    else
        strcat(pjlLine, cTIMED);
    strcat(pjlLine, cCRLF);
    strcat(pjlLine, cCODE);
    minSprintf(&pjlLine[strlen(pjlLine)],"%d",tmpStatus);
    strcat(pjlLine, cCRLF);
    strcat(pjlLine,cFF);

    pjl_output_string(Pipe, pjlLine);
}
