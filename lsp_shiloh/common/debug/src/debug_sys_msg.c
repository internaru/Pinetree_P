/****************************************************************************** 
 * ============================================================================
 * Copyright (c) 2004-2008 Marvell Semiconductor, Inc. All Rights Reserved.
 *                      
 *                         Marvell Confidential
 * ============================================================================
 *
 * Description:
 *
 *****************************************************************************/
#include <string.h>

#include "ATypes.h"
#include "lassert.h"
#include "agMessage.h"
#include "logger.h"
#include "debug.h"
#include "dprintf.h"


#ifdef HAVE_DBG_PRINTF

/* FUNCTION NAME: DecodeSysMsgEnum */
 
/** 
 * \brief Return a string with the symbolic name of the given enum value.
 * 
 * \param msg (AGMSG)
 * 
 * \return const char*
 * 
 **/
const char * DecodeSysMsgEnum( uint32_t m ) 
{
    AGMSG msg = (AGMSG) m;

    switch( msg )
    {
    // these are found in the AGMSG enum in agMessage.h
    CASE_ENUM_RETURN_STRING(MSG_JOBSTART);
    CASE_ENUM_RETURN_STRING(MSG_JOBEND);
    CASE_ENUM_RETURN_STRING(MSG_JOBSHEETSTART);
    CASE_ENUM_RETURN_STRING(MSG_JOBPAGESTART);
    CASE_ENUM_RETURN_STRING(MSG_JOBPAGEEND);
    CASE_ENUM_RETURN_STRING(MSG_JOBPLANESTART);
    CASE_ENUM_RETURN_STRING(MSG_JOBPLANEEND);
    CASE_ENUM_RETURN_STRING(MSG_JOBPLANEDATA);
    CASE_ENUM_RETURN_STRING(MSG_PAGESTART);
    CASE_ENUM_RETURN_STRING(MSG_START_NEXT_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_PAGE_PLANE_START);
    CASE_ENUM_RETURN_STRING(MSG_PAGEPRESTART);
    CASE_ENUM_RETURN_STRING(MSG_PAGEPRESTART_RSP);
    CASE_ENUM_RETURN_STRING(MSG_PAGEDONE);
    CASE_ENUM_RETURN_STRING(MSG_CREDITUPDATE);
    CASE_ENUM_RETURN_STRING(MSG_STATUSUPDATE);
    CASE_ENUM_RETURN_STRING(MSG_STATUSREQUEST);
    CASE_ENUM_RETURN_STRING(MSG_ERRCLEAR);
    CASE_ENUM_RETURN_STRING(MSG_USER_ERR_RECOVERY);
    CASE_ENUM_RETURN_STRING(MSG_DISPLAY_STATUS);
    CASE_ENUM_RETURN_STRING(MSG_OUT_OF_MEMORY);
    CASE_ENUM_RETURN_STRING(MSG_RACE_ENGINE);
    CASE_ENUM_RETURN_STRING(MSG_WAIT);
    CASE_ENUM_RETURN_STRING(MSG_DISPLAY_MSG);
    CASE_ENUM_RETURN_STRING(MSG_PRINT_STATUS_EVENT);
//    CASE_ENUM_RETURN_STRING(MSG_STATUS_EVENT_LOG_ONLY);    
    CASE_ENUM_RETURN_STRING(MSG_USER_INPUT);
    CASE_ENUM_RETURN_STRING(MSG_JOBABORT);
    CASE_ENUM_RETURN_STRING(MSG_PAPER_ADDED);
    CASE_ENUM_RETURN_STRING(MSG_OUTPUT_BIN_CLEARED);
    CASE_ENUM_RETURN_STRING(MSG_CONFIGPAGE);
    CASE_ENUM_RETURN_STRING(MSG_ALIGNMENT_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_SUPPLY_STATUS);
    CASE_ENUM_RETURN_STRING(MSG_CONTPRINT);
    CASE_ENUM_RETURN_STRING(MSG_DEMOPAGE);
//    CASE_ENUM_RETURN_STRING(MSG_NETWORKPAGE);
//    CASE_ENUM_RETURN_STRING(MSG_PCL5FONTLIST);
//    CASE_ENUM_RETURN_STRING(MSG_PCL6FONTLIST);
//    CASE_ENUM_RETURN_STRING(MSG_PSFONTLIST);
//    CASE_ENUM_RETURN_STRING(MSG_SERVICEPAGE);
//    CASE_ENUM_RETURN_STRING(MSG_USAGEPAGE);
//    CASE_ENUM_RETURN_STRING(MSG_PQPAGE);
//    CASE_ENUM_RETURN_STRING(MSG_CONFIG_AND_SSP);
    CASE_ENUM_RETURN_STRING(MSG_HTML_DATA);
//    CASE_ENUM_RETURN_STRING(MSG_ELABEL_PAGE_DONE);
    CASE_ENUM_RETURN_STRING(MSG_CLEAN_PRINTER);
    CASE_ENUM_RETURN_STRING(MSG_CLEAN_FINISHED);
    CASE_ENUM_RETURN_STRING(MSG_MENUPAGE);
    CASE_ENUM_RETURN_STRING(MSG_ERRORREPORT);
    CASE_ENUM_RETURN_STRING(MSG_INTERNAL_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_DELAY_NOTIFY);
    CASE_ENUM_RETURN_STRING(MSG_DELAY_SUBSCRIPTION);
    CASE_ENUM_RETURN_STRING(MSG_UPDATE_DISPLAY);
    CASE_ENUM_RETURN_STRING(MSG_RESTART_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_FORCE_STALL);
    CASE_ENUM_RETURN_STRING(MSG_MDNS_EVENT);
    CASE_ENUM_RETURN_STRING(MSG_BUTTON_WAKEUP);
    CASE_ENUM_RETURN_STRING(MSG_ENGCMD);
#ifdef WIRELESS
    CASE_ENUM_RETURN_STRING(MSG_WIRELESS_TEST_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_WIRELESS_UPDATE);
    CASE_ENUM_RETURN_STRING(MSG_WIRELESS_START_SCAN);
    CASE_ENUM_RETURN_STRING(MSG_WIRELESS_ATTEMPT_REJOIN);
#ifdef WPS
    CASE_ENUM_RETURN_STRING(MSG_RUN_WPS);
    CASE_ENUM_RETURN_STRING(MSG_CANCEL_WPS);
#endif // ifdef WPS
#endif // ifdef WIRELESS
    CASE_ENUM_RETURN_STRING(MSG_RESOURCES);
    CASE_ENUM_RETURN_STRING(MSG_SCANJOBS);
    CASE_ENUM_RETURN_STRING(MSG_STARTRECIPE);
    CASE_ENUM_RETURN_STRING(MSG_FREERECIPE);
    CASE_ENUM_RETURN_STRING(MSG_START_PARSE);
    CASE_ENUM_RETURN_STRING(MSG_END_PARSE);
    CASE_ENUM_RETURN_STRING(MSG_RTCALARM);
    CASE_ENUM_RETURN_STRING(MSG_CALGRAPH);
    CASE_ENUM_RETURN_STRING(MSG_ACKRECIPE);
    CASE_ENUM_RETURN_STRING(MSG_NAKRECIPE);
    CASE_ENUM_RETURN_STRING(MSG_STATUS_UNREGISTER);

    CASE_ENUM_RETURN_STRING(MSG_CANCELJOB);
    CASE_ENUM_RETURN_STRING(MSG_CANCELPAGE);
    CASE_ENUM_RETURN_STRING(MSG_RESTARTPAGE);
    CASE_ENUM_RETURN_STRING(MSG_RETRYPAGE);
    CASE_ENUM_RETURN_STRING(MSG_PRINTONLINE);
    CASE_ENUM_RETURN_STRING( MSG_TRAYCONFIG );
    CASE_ENUM_RETURN_STRING( MSG_PRINTSYSSTATUS );
    CASE_ENUM_RETURN_STRING( MSG_PRINTNUMPAGESUPDATE );

//    CASE_ENUM_RETURN_STRING(MSG_PCLPARSE_DONE);

    /* added _EX messages; needed the same meaning as the
     * original JOBSTART, etc, messages, but needed platform agnostic messages
     * that could be handled by both print and scan. 
     *
     * Except where noted, these messages have no parameters.
     */
    CASE_ENUM_RETURN_STRING(MSG_SCAN_JOBSTART);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_JOBEND);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_SHEETSTART);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_SHEETEND);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_PAGESTART);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_PAGEEND);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_PLANEDATA);

   

#ifdef HAVE_SCAN_SUPPORT
// #include "scanmsg.h"
    CASE_ENUM_RETURN_STRING(MSG_SCAN_SIZE);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_CAL);
    CASE_ENUM_RETURN_STRING(MSG_SCAN_DEV_REQUEST);
// #include "copymsg.h"
    CASE_ENUM_RETURN_STRING(MSG_COPY_CONTINUOUS);
#endif // ifdef HAVE_SCAN_SUPPORT

#ifdef HAVE_FAX_SUPPORT
// #include "faxmsg.h"
    CASE_ENUM_RETURN_STRING(MSG_FAX_DEBUG);
    CASE_ENUM_RETURN_STRING(MSG_START_FAX_SEND);
    CASE_ENUM_RETURN_STRING(MSG_START_FAX_RECEIVE);
    CASE_ENUM_RETURN_STRING(MSG_FAX_POLLING_RECEIVE);
    CASE_ENUM_RETURN_STRING(MSG_FAX_COMPELTE);
    CASE_ENUM_RETURN_STRING(MSG_FAX_PRINT);
    CASE_ENUM_RETURN_STRING(MSG_PAGEEND_CMP);
    CASE_ENUM_RETURN_STRING(MSG_JOBEND_CMP);
    CASE_ENUM_RETURN_STRING(MSG_FAX_FSEND);
    CASE_ENUM_RETURN_STRING(MSG_FAX_FAIL);
    CASE_ENUM_RETURN_STRING(MSG_NEWPAGE_CMP);
    CASE_ENUM_RETURN_STRING(MSG_HANDSET_HOOKSWITCH);
    CASE_ENUM_RETURN_STRING(MSG_HANDSET_RECORD);
    CASE_ENUM_RETURN_STRING(MSG_HANDSET_DIAL);
    CASE_ENUM_RETURN_STRING(MSG_HANDSET_HANGUP);
    CASE_ENUM_RETURN_STRING(MSG_HANDSET_SWITCH_TO_FAX);
    CASE_ENUM_RETURN_STRING(MSG_FAX_XMITTEST);
    CASE_ENUM_RETURN_STRING(MSG_FAX_ACTIVITYLOG);
    CASE_ENUM_RETURN_STRING(MSG_FAX_CALLREPORT);
    CASE_ENUM_RETURN_STRING(MSG_FAX_PHONEBOOK);
    CASE_ENUM_RETURN_STRING(MSG_FAX_BLOCKLIST);
    CASE_ENUM_RETURN_STRING(MSG_FAX_T30TRACE);
    CASE_ENUM_RETURN_STRING(MSG_FAX_BILLING);
    CASE_ENUM_RETURN_STRING(MSG_FAX_START_REPRINT);
    CASE_ENUM_RETURN_STRING(MSG_FAX_REPRINT);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_BEEP);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_CPA_ON);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_CPA_OFF);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_RING);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_ALARM);
    CASE_ENUM_RETURN_STRING(MSG_AUDIO_DONE);
#endif // ifdef HAVE_FAX_SUPPORT

    CASE_ENUM_RETURN_STRING(MSG_COMPCMD);
    CASE_ENUM_RETURN_STRING(MSG_DBGSTAT);
    CASE_ENUM_RETURN_STRING(MSG_DBGSTATEVT);
    CASE_ENUM_RETURN_STRING(MSG_SYSSTATUS_EVENT);
    CASE_ENUM_RETURN_STRING(MSG_TIME_TO_END);
    CASE_ENUM_RETURN_STRING(MSG_COMPRESSION);

    // KEYPAD and CONTROL PANEL (CTRLPANEL) Messages
    CASE_ENUM_RETURN_STRING(MSG_CP_KEY_PRESSED);
    CASE_ENUM_RETURN_STRING(MSG_CP_START_COPY);
    CASE_ENUM_RETURN_STRING(MSG_CP_BLINK_LED);
//    CASE_ENUM_RETURN_STRING(MSG_CP_SLEEP);
    CASE_ENUM_RETURN_STRING(MSG_JPEG_PAGE);
    CASE_ENUM_RETURN_STRING(MSG_JPEG_THUMB);

    // CGD MESSAGES
//    CASE_ENUM_RETURN_STRING(MSG_CGD_SET_LANGUAGE);
//    CASE_ENUM_RETURN_STRING(MSG_CGD_DISPLAY_SCREEN);
//    CASE_ENUM_RETURN_STRING(MSG_CGD_DISPLAY_BITMAP);
//    CASE_ENUM_RETURN_STRING(MSG_CGD_DISPLAY_MENU);
//   CASE_ENUM_RETURN_STRING(MSG_CGD_DISPLAY_ANIMATION);

    // UI Messages
    CASE_ENUM_RETURN_STRING(MSG_UI_DISK_LOADING);
    CASE_ENUM_RETURN_STRING(MSG_UI_DISK_INSERTED);
    CASE_ENUM_RETURN_STRING(MSG_UI_DISK_REMOVED);

    CASE_ENUM_RETURN_STRING(MSG_FREEIO);
    CASE_ENUM_RETURN_STRING(MSG_UI_KEY_HELD);
    CASE_ENUM_RETURN_STRING(MSG_UI_VIRTUAL_KEY_PRESSED);
    CASE_ENUM_RETURN_STRING(MSG_UI_TIMER_EXPIRE);
//    CASE_ENUM_RETURN_STRING(MSG_UI_RESTORE);
//    CASE_ENUM_RETURN_STRING(MSG_WIRELESS_WPS);

    // LangMgr Messages
//    CASE_ENUM_RETURN_STRING(MSG_LANG_START);
//    CASE_ENUM_RETURN_STRING(MSG_LANG_END_PARSE);
//    CASE_ENUM_RETURN_STRING(MSG_LANG_DOWN);
//    CASE_ENUM_RETURN_STRING(MSG_LANG_CANCEL_JOB);
//    CASE_ENUM_RETURN_STRING(MSG_LANG_CANCEL_PAGE);
//    CASE_ENUM_RETURN_STRING(MSG_LANG_CRC_PAGE);

//    CASE_ENUM_RETURN_STRING(MSG_RENDER_COMPLETE);
    // USB configuration message
    CASE_ENUM_RETURN_STRING(MSG_USB_CONFIG);

    // Thread control messages
//    CASE_ENUM_RETURN_STRING(MSG_SCAN_THREADIT);

    /* This must always be the last element of the enum.
     * Do not use this message for anything.
     */

    CASE_ENUM_RETURN_STRING(MSG_LAST_MSG);
    default:
        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("DecodeSysMsgEnum: enum=0x%X=%d out of range: MSG_LAST_MSG=0x%X=%d\n", msg, msg, MSG_LAST_MSG, MSG_LAST_MSG));
        return "*** Unrecognized AGMSG";
    }

    return "*** Unrecognized AGMSG";
} // end DecodeSysMsgEnum()

const char* ToString_DOACTION(DOACTION actionEnum)
{
    switch (actionEnum)
    {
    // these are found in the DOACTION enum in agMessage.h
    CASE_ENUM_RETURN_STRING(SYS_REQUEST);
    CASE_ENUM_RETURN_STRING(SYS_REQUSER);
    CASE_ENUM_RETURN_STRING(SYS_START);
    CASE_ENUM_RETURN_STRING(SYS_ACK);
    CASE_ENUM_RETURN_STRING(SYS_RESUME);
    CASE_ENUM_RETURN_STRING(LOCAL_START);
    CASE_ENUM_RETURN_STRING(LOCAL_ACK);
    CASE_ENUM_RETURN_STRING(LOCAL_RESUME);

//    CASE_ENUM_RETURN_STRING(DOACTION_NUM_ENUMS);
    default:
//        DPRINTF((DBG_SOFT|DBG_OUTPUT), ("ToString_DOACTION: enum=0x%X=%d out of range: DOACTION_NUM_ENUMS=0x%X=%d\n", actionEnum, actionEnum, DOACTION_NUM_ENUMS, DOACTION_NUM_ENUMS));
        return "*** Unrecognized DOACTION";
    }

    return "*** Unrecognized DOACTION";
} // end ToString_DOACTION()
#else
const char * DecodeSysMsgEnum( uint32_t m ) {
	return "\0";
}
#endif // ifdef HAVE_DBG_PRINTF

