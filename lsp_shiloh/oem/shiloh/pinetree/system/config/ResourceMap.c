/* 
 *
 * ============================================================================
 * Copyright (c) 2006-2013   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *               Marvell Confidential
 * ============================================================================
 *
*/
 
 /**
 * Description: File to define jobs and resources for the system
**/

#include "agRouter.h"
#include "agResourceMgr.h"
#include "SJMgr.h"
#include "ResourceMap.h" 
#include "platform.h"
#include "debug.h"


//
// System Jobs.  Note that the order of these jobs must match the enum in 
// resourceMap.h.  The enum is
// used to index into this array to get the job information.
//
JOB_RECIPE SystemJobs[] =
{
    //Printing
#ifdef HAVE_USB_HOST_PARSER 
 {
        .myJob = ejob_PrintIO,
        .NumResources = 1,
        .PercentCPU = 20,
        .RequiredResources = {e_UsbHostParser},
        .CancelAction = e_YesCancel,
        .Queued = e_Unlimited
     },
#elif defined(HAVE_PIP)
  #if defined(HAVE_GENERATOR_COMPRESS)
    {
        .myJob = ejob_PrintIO,
        .NumResources = 3,
        .PercentCPU = 20,
        .RequiredResources = {e_GenPip, e_GenCompressJBIG, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_Unlimited
    },
  #else
    {
        .myJob = ejob_PrintIO,
        .NumResources = 2,
        .PercentCPU = 20,
        .RequiredResources = {e_GenPip, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_Unlimited
    },
  #endif  // HAVE_GENERATOR_COMPRESS
#else
  #if defined(HAVE_GENERATOR_COMPRESS)
    {
        ejob_PrintIO,
        2,
        20,
        100000000,
        40000000,
        {e_GenCompressJBIG, e_Printer},
        e_YesCancel,
        e_Unlimited
    },
  #else
    {
        ejob_PrintIO,
        1,
        20,
        100000000,
        40000000,
        {e_Printer},
        e_YesCancel,
        e_Unlimited
    },
  #endif
#endif

    //Internal Page Print
    {
        .myJob                  = ejob_InternalPagePrint,
        .NumResources           = 1,
        .PercentCPU             = 10,
        .MemoryRequired         = 0,  // obsolete
        .ScratchReserved        = 0, // obsolete
        .RequiredResources      = {e_Printer},
        .CancelAction           = e_YesCancel,
        .Queued                 = e_Unlimited
    },

    //Internal Page HTTP 
    {
        ejob_InternalPageHttp, 
        0, 
        20, 
        0x600000, 
        8192,
        {e_NoResource},
        e_YesCancel,
        e_Unlimited
    },     

    //PJL
    {
        ejob_PJL,
        0,
        0,
        100000,
        0,
        {e_NoResource},
        e_NoCancel,
        e_Unlimited
    },

    //ACL
    {
        ejob_ACL,
        0,
        0,
        12000000,
        11000000,
        {e_NoResource},
        e_NoCancel,
        e_Unlimited
    },

    //CMD
    {
        ejob_CMD,
        0,
        0,
        12000000,
        11000000,
        {e_NoResource},
        e_NoCancel,
        e_Unlimited
    },

    //Copy
#if defined(HAVE_GENERATOR_COMPRESS)
    {
        .myJob = ejob_Copy,
        .NumResources = 6,
        .PercentCPU = 0,
        .RequiredResources = {e_Scanner, e_CopyApp, e_GenPip, e_Gen_NUp, e_GenCompressJBIG, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_One  //e_Two ->Two Copy job queued not supported. 
    },
#else
    {
        .myJob = ejob_Copy,
        .NumResources = 5,
        .PercentCPU = 0,
        .RequiredResources = {e_Scanner, e_CopyApp, e_GenPip, e_Gen_NUp, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_One  //e_Two ->Two Copy job queued not supported. 
    },
#endif

    //Scan to Host
    {
        ejob_ScanToHost,
        2,
        0,
        20000000,
        4000000,
        {e_Scanner, e_IOScanParser},
        e_YesCancel,
        e_One
    },

    //Copy to Host
    {
        ejob_CopyToHost,
        3,
        0,
        12000000,
        8000000,
        {e_Scanner, e_CopyApp, e_CopyToHostParser},
        e_YesCancel,
        e_One  //e_Two ->Two Copy job queued not supported. 
    },

    //ACL Scan Calibration
    {
        ejob_ScanCal,
        1,
        0,
        12000000,
        0,
        {e_Scanner},
        e_YesCancel,
        e_Unlimited
    },


    /* adding demo_scanapp */
    {
        ejob_DemoScanApp,
        2,
        0,
        12000000,
        0,
        {e_Scanner, e_DemoScanApp},
        e_YesCancel,
        e_One
    },

    {
        ejob_ScanFax,
        2,
        CPU_SCAN,
        10000000,
        4000000,
        {e_Scanner, e_ScanToFax},
        e_YesCancel,
        e_Unlimited
    },

    {
        ejob_Fax, 
        1, 
        CPU_FAX, 
        2000000,
        2000000,
        {e_FaxPhoneline},
        e_YesCancel,
        e_One
    },

    {
        ejob_FaxParse,
        1,
        0,
        500000,
        0,
        {e_IOFaxParser},
        e_NoCancel,
        e_One
    },
#if 0
    // Fax Printing 
    {
        ejob_FaxPrint, 
        1, 
        20, 
        0x500000, 
        8192,
        {e_Printer},
        e_YesCancel, 
        e_Unlimited
    },       
#else
    {
        .myJob = ejob_FaxPrint,
        .NumResources = 3,
        .PercentCPU = 0,
        .RequiredResources = {e_GenPip, e_GenCompressJBIG, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_Unlimited
    },
#endif
    {
        .myJob =                    ejob_ThumbnailPhoto,
        .NumResources =             1,
        .PercentCPU =               10,
        /* MemoryRequired is the total memory required for the job.  The difference
         * between MemoryRequired and ScratchReserved is the size of the job's
         * Data heap.  In the photo Job, the Data heap is allocated from
         * until an allocation fails.  The allocations that succeeded are used
         * to calculate the number of thumbnails that can be cached in memory at
         * one time.  */
        .MemoryRequired =           9247168,
        .ScratchReserved =          5497128,
        .RequiredResources =        {e_ResPhoto, e_NoResource, e_NoResource, e_NoResource, e_NoResource, e_NoResource, e_NoResource},
        .CancelAction =             e_YesCancel,
        .Queued =                   e_One
    },

    {
        ejob_HttpScan, 
        2, 
        0, 
        2000000, 
        4000000,
        {e_Scanner, e_HttpScanApp},
        e_YesCancel, 
        e_Unlimited
    },

    //Printing
    {
        .myJob                = ejob_zjs_host_print,
        .NumResources         = 1,
        .PercentCPU           = 20,
        .RequiredResources    = { e_Printer },
        .CancelAction         = e_YesCancel,
        .Queued               = e_Unlimited
    },

    //Scan to Email
    {
        ejob_ScanToEmail,
        2,
        0,
        20000000,
        4000000,
        {e_Scanner, e_ScanToEmailApp},
        e_YesCancel,
        e_One
    },

    //Scan to Memory Card
    {
        ejob_ScanToMemCard,
        2,
        0,
        20000000,
        4000000,
        {e_Scanner, e_ScanToMemCardApp},
        e_YesCancel,
        e_One
    },

    //Parse job like PF_Parser or PcFax_Parser
    {
        ejob_ParseMisc,
        0,
        0,
        2000000,
        4000000,
        {e_NoResource},
        e_YesCancel,
        e_Unlimited
    },

    //File Print job (MobileApp print)
    {
        .myJob = ejob_PrintFile,
        .NumResources = 4,
        .PercentCPU = 20,
        .RequiredResources = {e_GenPip, e_Gen_NUp, e_GenCompressJBIG, e_Printer},
        .CancelAction = e_YesCancel,
        .Queued = e_Unlimited
    },

};

#ifdef HAVE_DBG_PRINTF
const char* to_string_job_types(JOB_TYPES jobtype)
{
    switch(jobtype)
    {
        CASE_ENUM_RETURN_STRING(ejob_NULL);
        CASE_ENUM_RETURN_STRING(ejob_PrintIO);
        CASE_ENUM_RETURN_STRING(ejob_InternalPagePrint);
        CASE_ENUM_RETURN_STRING(ejob_PJL);
        CASE_ENUM_RETURN_STRING(ejob_ACL);
        CASE_ENUM_RETURN_STRING(ejob_CMD);
        CASE_ENUM_RETURN_STRING(ejob_Copy);
        CASE_ENUM_RETURN_STRING(ejob_CopyToHost);
        CASE_ENUM_RETURN_STRING(ejob_ScanToHost);
        CASE_ENUM_RETURN_STRING(ejob_ScanCal);
        CASE_ENUM_RETURN_STRING(ejob_ScanFax);
//        CASE_ENUM_RETURN_STRING(ejob_Fax);
        CASE_ENUM_RETURN_STRING(ejob_FaxParse);
        CASE_ENUM_RETURN_STRING(ejob_FaxPrint);
        CASE_ENUM_RETURN_STRING(ejob_ThumbnailPhoto);
        CASE_ENUM_RETURN_STRING(ejob_HttpScan);
        CASE_ENUM_RETURN_STRING(ejob_zjs_host_print);
        CASE_ENUM_RETURN_STRING(ejob_ScanToEmail);
        CASE_ENUM_RETURN_STRING(ejob_ScanToMemCard);
        CASE_ENUM_RETURN_STRING(ejob_ParseMisc);
        CASE_ENUM_RETURN_STRING(ejob_PrintFile);
        DEFAULT_CASE_ENUM_RETURN_STRING(JOB_TYPES);
    }
    return "bug";
}
#endif
