/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * date_time.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ATypes.h"
#include "date_time_api.h"
#ifdef HAVE_STRING_MGR
#include "string_mgr_api.h"
#endif
#include "dprintf.h"
#include "lassert.h"

// array to give the number of days in a given month (except February in leap year) 
static char daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


/**
*\brief Check to see if the year given is a leap year
*
* \param year
*
* \return true  Leap year
*         false Non-leap year
*
*/
static bool isLeapYear(int year)
{
    if((year%4))
        return false;
    else if(!(year%400))
        return true;
    else if(!(year%100))
        return false;
    
    return true;
}

/**
 *
 * \brief helper function which maps a 24 hour format hour to a 12 hour format hour
 */
char dispHr_12HrFmt( char hour )
{
    if ( hour == 0 )           //midnight to 12:59AM
    {
        return 12;
    }
    else if ( hour < 13 )     //1:00AM to 12:59PM
    {
        return hour;
    }
    return ( hour - 12 ); //1:00PM to 11:59PM
}
                                   
/** 
 * \brief   Format a string with the passed in time.
 * 
 * \param   timeStr:    The string to populate.
 *          pTime:      Pointer to the structure containing the
 *                      time.
 *          timeFormat: Specifies 12 or 24 hour format.
 *          showSecs:   True if seconds should be displayed, false
 *                      otherwise.
 *          show_AM_PM: True if an AM/PM indicator should be displayed,
 *                      false otherwise.
 * 
 * \retval None
 * 
 * 
 */
void formatTime( char * timeStr, struct tm *pTime, int timeFormat, bool showSecs, bool show_AM_PM )
{
    const char *AM_PM_Str = "";
    char dispHr;

    switch (timeFormat)
    {
        case e_12HOUR:
            //select AM/PM designator
            if (show_AM_PM)
            { // pick an AM/PM indicator
                if ( isPM_12HrFmt( pTime->tm_hour ) )
                {
#ifdef HAVE_STRING_MGR
                    // get translation for PM
                    AM_PM_Str = string_mgr_get_string( STRING_CP_PM, string_mgr_loc_full );
#else
                    // hardcode English PM string
                    AM_PM_Str = "PM";
#endif
                }
                else
                {
#ifdef HAVE_STRING_MGR
                    // get translation for AM
                    AM_PM_Str = string_mgr_get_string( STRING_CP_AM, string_mgr_loc_full );
#else
                    // hardcode English AM string
                    AM_PM_Str = "AM";
#endif
                }
            }
            else
            { // suppress an AM/PM indicator
                AM_PM_Str = "";
            }

            //select the hour value to display
            dispHr = dispHr_12HrFmt( pTime->tm_hour );

            // format the time, including seconds if needed
            if (!showSecs)
                minSprintf( timeStr, "%02.2d:%02.2d%s", dispHr, pTime->tm_min, AM_PM_Str );
            else
                minSprintf( timeStr, "%02.2d:%02.2d:%02.2d%s", dispHr, pTime->tm_min, pTime->tm_sec, AM_PM_Str );
            break;

        case e_24HOUR:
            // format the time, including seconds if needed
            if (!showSecs)
                minSprintf( timeStr, "%02.2d:%02.2d", pTime->tm_hour, pTime->tm_min );
            else
                minSprintf( timeStr, "%02.2d:%02.2d:%02.2d", pTime->tm_hour, pTime->tm_min, pTime->tm_sec );
            break;

        default:
            break;
    }
}

 
/** 
 * \brief   Format a string with the passed in date.
 * 
 * \param   timeStr:    The string to populate.
 *          pDate:      Pointer to the structure containing the date.
 *          dateFormat: Specifies month first, day first, or year first date
 *                      format.
 *          abbrevYear: True if only last 2 digits of year should be displayed,
 *                      false if year shouldn't be abbreviated.
 * 
 * \retval None
 * 
 * 
 */
void formatDate( char * dateStr, struct tm *pDate, int dateFormat, bool abbrevYear )
{
    switch (dateFormat)
    {
        case e_MONTH_FIRST:
            if (abbrevYear)
                minSprintf( dateStr, "%02.2d/%02.2d/%02.2d", (pDate->tm_mon+1)%256, (pDate->tm_mday+1)%256, pDate->tm_year );
            else
                minSprintf( dateStr, "%02.2d/%02.2d/%04.4d", (pDate->tm_mon+1)%256, (pDate->tm_mday+1)%256, 2000 + pDate->tm_year );
            break;

        case e_DAY_FIRST:
            if (abbrevYear)
                minSprintf( dateStr, "%02.2d/%02.2d/%02.2d", (pDate->tm_mday+1)%256, (pDate->tm_mon+1)%256, pDate->tm_year );
            else
                minSprintf( dateStr, "%02.2d/%02.2d/%04.4d", (pDate->tm_mday+1)%256, (pDate->tm_mon+1)%256, 2000 + pDate->tm_year );
            break;

        case e_YEAR_FIRST:
            if (abbrevYear)
                minSprintf( dateStr, "%02.2d/%02.2d/%02.2d", pDate->tm_year, (pDate->tm_mon+1)%256, (pDate->tm_mday+1)%256 );
            else
                minSprintf( dateStr, "%04.4d/%02.2d/%02.2d", 2000 + pDate->tm_year, (pDate->tm_mon+1)%256, (pDate->tm_mday+1)%256 );
            break;

        default:
            break;
    }
}


/** 
 * \brief   Function to verify the passed in date is valid.
 * 
 * \param   pDate:      Pointer to the structure containing the date.
 * 
 * \retval true if the passed in date is valid, false otherwise.
 * 
 * 
 */
bool validDate( struct tm *pDate )
{
    //confirm year between 2000 and 2099
    if ( pDate->tm_year > 99 )
    {
        return false;
    }

    //confirm month between 1 and 12 (0 based var, so 0..11)
    if ( pDate->tm_mon > 11 ) 
    {
        return false;
    }

    //confirm day is valid for current month & year
    if ( ((pDate->tm_mon+1) == 2 ) && (isLeapYear(pDate->tm_year)))
    { // date is February of a leap year
        if ((pDate->tm_mday) > 28 /*29-1 for 0 based*/)
        {
            return false;
        }
    }
    else
    {  // not February of leap year
        if ( pDate->tm_mday >= daysInMonth[pDate->tm_mon] )
        {
            return false;
        }
    }

    return true;
}



