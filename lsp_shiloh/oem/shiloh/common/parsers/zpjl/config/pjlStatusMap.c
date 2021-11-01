/*
 *
 * ============================================================================
 * Copyright (c) 2006-2010   Marvell International Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file pjlStatusMap.c 
 *
 */

#include <stdint.h>
#include "agRouter.h"
#include "ATypes.h"
#include "print_status_codes.h"
#include "agprint.h"
#include "paper_size_api.h"

#include "logger.h"
#include "dprintf.h" // include header for agprintf
#include "debug.h"

/**
 * This is the map from internal status to pjl status.
 */
uint16_t StatMap[]=
{
    (uint16_t)STATUS_TRAY2_ATTACHED,               10001,
    (uint16_t)STATUS_TRAY2_NOT_ATTACHED,               10001,
    (uint16_t)STATUS_TRAY3_ATTACHED,			   10001,
    (uint16_t)STATUS_TRAY3_NOT_ATTACHED,			   10001,
    (uint16_t)STATUS_TONER_NO_ISSUE,			   10001,
    (uint16_t)STATUS_PAPER_OUT_TRAYS_UNSTABLE,			   10001,    //Temp
    (uint16_t)STATUS_JOBINFO_END_JOB,			   10001,
    (uint16_t)STATCAT_INITIALIZING,               10003,
    (uint16_t)STATUS_INFO_CANCELING,              10007,
    (uint16_t)STATUS_INFO_PRINTING_INTERNAL,      10014,
    (uint16_t)STATUS_INFO_PRINTING_DEMO,          10017,
    (uint16_t)STATUS_INFO_PRINTING,               10014, //10023,
    (uint16_t)STATUS_JOBINFO_START_JOB,               10014,
    (uint16_t)STATUS_INFO_CLEANING,               10031,
    (uint16_t)STATUS_INPUT_PAPER_MISPICK,         30119,
    (uint16_t)STATUS_MISPRINT_RESERVATION,        30200,
    (uint16_t)STATUS_MISPRINT_BD,                 30201,
    (uint16_t)STATUS_MISPRINT_SENDING_ERROR,      30202,
    (uint16_t)STATUS_PAPER_JAM_REINSERTCARTRIDGE,      40005,	//Invalid Catridge
    (uint16_t)STATUS_TONER_LOW,      40038,
    (uint16_t)STATUS_TONER_END,      40039,
    (uint16_t)STATUS_PAPER_JAM_TONER_BLOCK,      40039,	//Toner End
    (uint16_t)STATUS_DRUM_END,      40129,
    (uint16_t)STATUS_DRUM_LOW_MODE_1,      40130,	//Drum near end LOW_1
    (uint16_t)STATUS_DRUM_LOW_MODE_2,      40130,	//Drum near end LOW_2
    (uint16_t)STATUS_DRUM_LOW_MODE_3,      40130,	//Drum near end LOW_3
    (uint16_t)STATUS_INPUT_TRAY_SIZE_MISMATCH,      40146,	//Paper Mismatch    
    (uint16_t)STATUS_PAPER_JAM_FULLSTACK,      40147,	//Output Tray Full
    (uint16_t)STATUS_DOOROPEN,                    42200,  //40021,
    (uint16_t)STATUS_DOORCLOSE_FRONT,              42200,
    (uint16_t)STATUS_DOOROPEN_FD,                 40020,  // Rear door -jrs
    (uint16_t)STATUS_DOOROPEN_FRONT,              42200,  //40021,
    (uint16_t)STATUS_INPUT_MANUAL_FEED,           41100,
    (uint16_t)STATUS_INFO_BEGIN_MANUALDUPLEX,     40120,
    (uint16_t)STATUS_INPUT_MANUAL_DUPLEX,         40121,

    (uint16_t)STATUS_PAPER_OUT_TRAY1,             42060, //41000,
    (uint16_t)STATUS_PAPER_OUT_TRAY2,             42060, //41200,
    (uint16_t)STATUS_PAPER_OUT_TRAY3,             42060, //41300,
    (uint16_t)STATUS_PAPER_OUT_ALL,               42060, //41900,
    (uint16_t)STATUS_PAPER_OUT_TRAY4,               42060,
    (uint16_t)STATUS_PAPER_OUT_TRAY1_FINAL,               42060,
    (uint16_t)STATUS_PAPER_OUT_TRAY2_FINAL,               42060,
    (uint16_t)STATUS_PAPER_OUT_TRAY3_FINAL,               42060,
    (uint16_t)STATUS_PAPER_OUT_TRAY4_FINAL,               42060,
    (uint16_t)STATUS_PAPER_JAM_INPUT,             42000,
    (uint16_t)STATUS_PAPER_JAM_TRAY1,             42000,  //42001,
    (uint16_t)STATUS_PAPER_JAM_TRAY2,             42000,  //42002,
    (uint16_t)STATUS_PAPER_JAM_TRAY3,             42000, //42003,
    (uint16_t)STATUS_PAPER_JAM_TRAY4,             42000,
    (uint16_t)STATUS_PAPER_JAM_OUTPUT,            42000, //42005,
    (uint16_t)STATUS_PAPERJAM,                    42000, //42006,
    (uint16_t)STATUS_MAINTENANCE_JAM,             42000, //42007,
    (uint16_t)STATUS_PAPER_JAM_OUTPUT,             42000,
    (uint16_t)STATUS_PAPER_JAM_EXTSEN_NOT_DET,             42000,
    (uint16_t)STATUS_PAPER_JAM_INPSEN_NOT_REL,             42000,
    (uint16_t)STATUS_PAPER_JAM_EXTSEN_NOT_REL,             42000,
    (uint16_t)STATUS_PAPER_JAM_DUPLEX,             42000,
    (uint16_t)STATUS_PAPER_JAM_FULLSTACK,             42000,
    (uint16_t)STATUS_CONNECTION_DUPLEXOR_FAIL,    50015,
    (uint16_t)STATUS_PAPERPATH_ERROR,         	  50022,
};  
  
#ifdef HAVE_DBG_PRINTF

#define ENUM_TO_STRING( enum ) #enum

const char* to_string_pjl_status(uint16_t pjl_status)
{
    switch(pjl_status)
    {
    case 0:     return    "null";
    case 10001: return    "Ready"; ///< pjl spec?
    case 10003: return    ENUM_TO_STRING(STATCAT_INITIALIZING);
    case 10007: return    ENUM_TO_STRING(STATUS_INFO_CANCELING);             
    case 10014: return    ENUM_TO_STRING(STATUS_INFO_PRINTING_INTERNAL);
    case 10017: return    ENUM_TO_STRING(STATUS_INFO_PRINTING_DEMO);
    case 10023: return    ENUM_TO_STRING(STATUS_INFO_PRINTING);
    case 10031: return    ENUM_TO_STRING(STATUS_INFO_CLEANING);
    case 30119: return    ENUM_TO_STRING(STATUS_INPUT_PAPER_MISPICK);
    case 30200: return    ENUM_TO_STRING(STATUS_MISPRINT_RESERVATION);
    case 30201: return    ENUM_TO_STRING(STATUS_MISPRINT_BD);
    case 30202: return    ENUM_TO_STRING(STATUS_MISPRINT_SENDING_ERROR);
    case 40021: return    ENUM_TO_STRING(STATUS_DOOROPEN);
    case 40020: return    ENUM_TO_STRING(STATUS_DOOROPEN_FD);
    case 41100: return    ENUM_TO_STRING(STATUS_INPUT_MANUAL_FEED);
    case 40120: return    ENUM_TO_STRING(STATUS_INFO_BEGIN_MANUALDUPLEX);
    case 40121: return    ENUM_TO_STRING(STATUS_INPUT_MANUAL_DUPLEX);
    case 41000: return    ENUM_TO_STRING(STATUS_PAPER_OUT_TRAY1);
    case 41200: return    ENUM_TO_STRING(STATUS_PAPER_OUT_TRAY2);
    case 41300: return    ENUM_TO_STRING(STATUS_PAPER_OUT_TRAY3);
    case 41900: return    ENUM_TO_STRING(STATUS_PAPER_OUT_ALL);
    case 42000: return    ENUM_TO_STRING(STATUS_PAPER_JAM_INPUT);
    case 42001: return    ENUM_TO_STRING(STATUS_PAPER_JAM_TRAY1);
    case 42002: return    ENUM_TO_STRING(STATUS_PAPER_JAM_TRAY2);
    case 42003: return    ENUM_TO_STRING(STATUS_PAPER_JAM_TRAY3);
    case 42005: return    ENUM_TO_STRING(STATUS_PAPER_JAM_OUTPUT);
    case 42006: return    ENUM_TO_STRING(STATUS_PAPERJAM);
    case 42007: return    ENUM_TO_STRING(STATUS_MAINTENANCE_JAM);
    case 50015: return    ENUM_TO_STRING(STATUS_CONNECTION_DUPLEXOR_FAIL);
    case 50022: return    ENUM_TO_STRING(STATUS_PAPERPATH_ERROR);

    DEFAULT_CASE_ENUM_RETURN_STRING(pjl_status);
    }
    return "bug";
}
#endif
  
/* FUNCTION NAME: MapPaperSize */
 
/** 
 * \brief Map paper size from internal representation to pjl status size.
 * PJL sends status back to the host.  When paper size is desired a specific
 * pjl status code is returned.  This routine maps the internal paper sizes
 * to the pjl status code.
 * 
 * \param Size Internal paper size value.
 * 
 * \retval uint32_t The mapped size or 0 if none found.
 * 
 **/
uint32_t MapPaperSize(mediasize_t Size)
{

    return paper_get_pjl_code(Size);

}    


 
 
/** 
 * \brief Map the internal status to the correct pjl status code.
 * To report status to the host we need to map our internal status to the pjl
 * status that the host will know.  This routine does that.
 * 
 * \param statusCode Internal status code.
 * 
 * \retval uint32_t PJL status to report to the host.
 * 
 **/
uint32_t map_print_status_to_pjl(uint32_t statusCode)
{
    uint32_t i;
    uint32_t TempStatus;
    uint32_t pjlStatus=0;
	trayenum_t tray;
	uint32_t TrayProperties = 0;
        
    TempStatus = statusCode; 
    
    TempStatus &= STATUS_CATEGORY_MASK | STATUS_CODE_MASK;  // save only the bits I am interested in.

    //if online (this is the online with no warnings)
    if(ISONLINE(TempStatus))
    {
        pjlStatus = 10001;        // everything is ok, return
        goto MAPSTATUS_DONE;
    }
    //if status is not supposed to be reported to PJL
    else if(statusCode & STATUS_NOPJL_MASK)
    {
        //For warnings, just report online
        //TODO:  what is the correct behavior for offline messages?
        if(!ISOFFLINE(statusCode))
        {
            pjlStatus = 10001;        // everything is ok, return
            goto MAPSTATUS_DONE;
        }
		if( STATCATCMP(TempStatus, STATCAT_PAPEROUT) )
		{
			pjlStatus = 10023;        // We should only get this while printing
			goto MAPSTATUS_DONE;

		}

    }

    //
    // Do load status message
    //

    //if input
#if 0    
    if(STATCATCODECMP(TempStatus, STATUS_INPUT_LOAD_TRAY) ||
       STATCATCODECMP(TempStatus, STATUS_INPUT_MANUAL_FEED) ||
       STATCATCMP(TempStatus, STATCAT_PAPEROUT) )
#else       
    if(STATCATCODECMP(TempStatus, STATUS_INPUT_LOAD_TRAY) ||
       STATCATCODECMP(TempStatus, STATUS_INPUT_MANUAL_FEED) )
#endif       
    {
        //Here is how the load tray messages map:
        //410xx load tray 1
        //411xx load manual feed tray
        //412xx load tray 2
        //413xx load tray 3
        //419xx paper out all trays
        //NOTE: manual feed is: 41100 (it is handled here) 
        tray = TRAY_1;

        if(STATCATCODECMP(TempStatus, STATUS_INPUT_MANUAL_FEED))
		{		
            pjlStatus += 100;       // set the status for manual feed.
			DPRINTF((DBG_SOFT | DBG_OUTPUT),("MapStatus: manual feed tray\n"));
		}
		else
		{
			// not the manual feed tray look up physical tray
			switch( TrayProperties & INPUTPROPERTY_PHYSICAL_TRAY_MASK )
			{
				// This printer supports "virtual trays" so we need to map the PJL status to the physical tray
				case INPUTPROPERTY_PHYSICAL_TRAY_1:
					DPRINTF((DBG_SOFT | DBG_OUTPUT),("MapStatus: physical tray 1\n"));
					break;
				case INPUTPROPERTY_PHYSICAL_TRAY_2:
					pjlStatus += 200;
					DPRINTF((DBG_SOFT | DBG_OUTPUT),("MapStatus: physical tray 2\n"));
					break;
				case INPUTPROPERTY_PHYSICAL_TRAY_3:
					DPRINTF((DBG_SOFT | DBG_OUTPUT),("MapStatus: physical tray 3\n"));
					pjlStatus += 300;
					break;
				default:
					// this is the default case for printers that don't support virtual trays
					if(tray != TRAY_1)
					{
						DPRINTF((DBG_SOFT | DBG_OUTPUT),("MapStatus: no physical tray given: tray = %x\n", tray +1));
						pjlStatus += ((tray+1) * 100);
					}
					break;
			}
		}
        
        goto MAPSTATUS_DONE;
    }    

    for (i = 0; i < (sizeof(StatMap)/sizeof(StatMap[0])); i+=2)
    {
        if(TempStatus == StatMap[i])
        {
            pjlStatus = StatMap[i+1];           // return the value
            break;
        }
    }


    MAPSTATUS_DONE:

    //     agprintf("MapStatus: ******* returning %d ********\n",pjlStatus); 

    return pjlStatus;               // code not found, error
}

