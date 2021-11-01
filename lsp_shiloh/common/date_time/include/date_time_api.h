/*
 * ============================================================================
 * Copyright (c) 2008-2011   Marvell Semiconductor, Inc. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

#ifndef DATE_TIME_API_H
#define DATE_TIME_API_H

#include <stdbool.h>
#include <time.h>

/**
 * \brief The date/time structure is used in two ways.  The first,
 * and primary, use is for specifying a specific date and time.  The
 * value ranges given below as part of the structure show the expected
 * ranges for the fields when used in this manner.  The other usage of the
 * structure is to specify an offset to be added to a time for getting
 * a future time.  This is a bit more tricky, but once you understand it,
 * it makes sense.  The time field is still being thought of as a date, but
 * all the fields range is available.  The date you specify is still based
 * on time since Jan 1 2000 though, so adding 13 months would get you as many
 * days as there are from Jan 1 2000 to 13 months from then (Feb 1, 2001).
 * Remember to think about leap years too.
 *
 */

//formats for time display
typedef enum
{
	e_12HOUR = 0,
	e_24HOUR
} e_TIME_FORMAT; 

//formats for date display
typedef enum
{
	e_MONTH_FIRST = 0,
	e_DAY_FIRST,
    e_YEAR_FIRST
} e_DATE_FORMAT; 

//structure to hold current time & date display format
typedef struct
{
    e_TIME_FORMAT timeFormat;
    e_DATE_FORMAT dateFormat;
} TIME_DATE_FORMATS;

//returns true if input hour corresponds to PM in 12 hour format, false otherwise
#define isPM_12HrFmt( hour ) (hour >= 12)


/** traditionally, on unix-based systems, epoch time is represented based on a
 * starting date of 1970-01-01 00:00:00 +0000 (UTC).  This macro represents the
 * number of seconds between this "epoch" and the more modern epoch tiem of:
 * 2000-01-01 00:00:00 +0000 (UTC) */
#define DATE_TIME_EPOCH_SECONDS_UNIX_TO_MODERN 946684800 


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
 */
void formatTime( char * timeStr, struct tm *pTime, int timeFormat, bool showSecs, bool show_AM_PM );

void formatDate( char * dateStr, struct tm *pDate, int dateFormat, bool abbrevYear );

char dispHr_12HrFmt( char hour );

bool validDate( struct tm *pDate );




#endif /* DATE_TIME_API_H */
