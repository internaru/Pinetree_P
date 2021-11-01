/* 
 *
 * ============================================================================
 * Copyright (c) 2007-2010   Marvell Semiconductor, Inc. All Rights Reserved
*
*               Marvell Confidential
 * ============================================================================
 *
*/
/**
 * \file ResourceMap.h
 *
 * \brief Defines the jobs in the system.
 *
 */

#ifndef RESOURCEMAP_H
#define RESOURCEMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This defines the jobs in the system.  Note that this enum must match the job
 * list in ResourceMap.c.  This enum is used to index into the jobs array so
 * order is very important!!
 */
typedef enum
{
    ejob_NULL = 0,
    ejob_PrintIO,
    ejob_InternalPagePrint,
    ejob_InternalPageHttp,    
    ejob_PJL,
    ejob_ACL,
    ejob_CMD,
    ejob_Copy,
    ejob_CopyToHost,
    ejob_ScanToHost,
    ejob_ScanCal,
    ejob_DemoScanApp,
    ejob_ScanFax,
    ejob_Fax,
    ejob_FaxParse,
    ejob_FaxPrint,
    ejob_ThumbnailPhoto,
    ejob_HttpScan,
    ejob_zjs_host_print,
    ejob_ScanToEmail,
    ejob_ScanToMemCard,
    ejob_ParseMisc,
    ejob_PrintFile,

#ifdef DEBUG
    //,ejob_ConfigToHost
#endif // DEBUG

} JOB_TYPES;

#ifdef HAVE_DBG_PRINTF
    const char* to_string_job_types(JOB_TYPES jobtype);
#else
    static inline const char* to_string_job_types(JOB_TYPES jobtype) {return 0;}
#endif

#ifdef __cplusplus
}
#endif

#endif
