/***********************************************************
* (c) Copyright 2008-2009 Marvell International Ltd. 
*
*               Marvell Confidential
* ==========================================================
*/

#ifndef PJLAPI
#define PJLAPI
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void pjl_cmd_ustatus_page(IOReg */*pipe*/, uint32_t /* Page Number */);
void pjl_cmd_ustatus_start_job(IOReg */* Pipe*/, char */* Job Name*/);
void pjl_cmd_ustatus_job_end(IOReg *Pipe, char *Name, uint32_t Pages, uint32_t JobStatus);
void pjl_cmd_ustatus_job_cancel(IOReg */* Pipe*/, char */* Job Name*/, uint32_t /* Pages printed */);
void pjl_cmd_ustatus_device(IOReg *, uint32_t, bool);
void pjl_cmd_ustatus_timed(IOReg *Pipe, uint32_t statusCode);
typedef struct UstatusStruct
{
   SINT32  (*UStatusFunction)(UINT32 statusCode, void * Details );                ///< Function to process the status for the pipe.
} UstatusReg;
void UStatusUnregisterFunction(UstatusReg* function);
void UStatusRegisterFunction(UstatusReg* function);

// Only systems with automatic language switching outside of PJL need this; PCL for instance.
void MaybeResetPJLVars(void);

/** 
 * \brief Map the internal print status to the correct pjl status code.
 * To report status to the host we need to map our internal status to the pjl
 * status that the host will know.  This routine does that.
 *          
 * \param statusCode status event from print_status_codes.h
 *            
 * \retval uint32_t PJL status to report to the host.
 * 
 **/
uint32_t map_print_status_to_pjl(uint32_t status_code);       

#ifdef HAVE_DBG_PRINTF
    const char* to_string_pjl_status(uint16_t pjl_status);
#else
    static inline const char* to_string_pjl_status(uint16_t pjl_status) { return 0; }
#endif

#ifdef __cplusplus
}
#endif
#endif


