/*
 *  $RCSfile: tonedet.c,v $
 * $Revision: 1.13 $
 *   $Author: miurat $
 *     $Date: 2011/11/15 22:03:38 $
 */
/************************************************************************
 *
 *      tonedet.c
 *
 *  Low Level Phone Module Functions for Modem DTMF/HDLC/CNG detection
 *  This module is OS independent.
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"

#include "ATTRIB.H"
#include "BUFFERS.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "phone.h"

#define _DEBUG

/*********************************************************************/

#define TONE_DETECTION_OFF  0xFFFF  /* Disable Tone Detection */

/* The follow time constant are in milli-seconds */
#define TONE_DEBOUNCE_TIME  100     /* Min. time to confirm TONE state */

extern PToneParams PTP;

static UINT16 MaxSilenceTime;                       /* milli-seconds */
static UINT16 DetectionTime, SilenceTime, FEDTime;  /* milli-seconds */
static UINT16 CNGOnTime, CNGOffTime, FSKTime;       /* milli-seconds */
static UINT16 CIOnTime, ANSOnTime;                  /* milli-seconds */
static UINT8  CNGHalfCycle;

extern UINT16 DialTimeout;

/*********************************************************************/


/** This section is for Modem DTMF detection **/

/*-------------------------------------------------------------------
    Name: SetUpDTMFDetection - Set up modem for DTMF detection.
          If the caller already in certain mode (e.g. Voice Codec Mode) that
          is able to detect DTMF, no mode change will take place.  Otherwise,
          ModemRxConfigDTMF() will set to proper mode for DTMF detection.
    Input:  none
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

void SetUpDTMFDetection( void )
{
	/* Set Up modem for DTMF detection */
	ModemRxConfigDTMF();
}


/*-------------------------------------------------------------------
    Name: CheckDTMFDetection - Check if a DTMF digit detected.
          If yet, notify high level phone module.
    Input:  none
    Output: none
    Return: UINT8 DTMFDigit - ASCII DTMF digit or 0 for no data available
    Note:   The original DTMF detection/decode code has been moved to modem.c.
 *-------------------------------------------------------------------*/

UINT8 CheckDTMFDetection( void )
{
	UINT8 DTMFDigit;

	if ( ModemRxDTMFDigit( &DTMFDigit ) )
		return DTMFDigit;
	else
		return 0;                   /* No data available */
}


/*********************************************************************/

/* New Call Progress Tone Detection */

/** This section is for Call Progress monitoring **/

/*
  According to ITU-T Q.35, the new standard dial tone is 425 Hz (400-450 Hz)
  single frequency continuous tone.
  In the USA, according to ANSI T1.401 Annex D, the dial tone is
  350 Hz + 440 Hz continuous tone, and the busy tone is 480 Hz + 620 Hz
  tone pulses with 0.5 sec on and 0.5 sec off.
  However, in some area, the dial tone is 600 Hz modulated by 120 Hz or 133 Hz
  continuous tone, and the busy tone is 600 Hz modulated by 120 Hz with
  0.5 s on and 0.5 s off.
  Therefore, we'd like to detect either 340-360 Hz or 400-450 Hz as dial tones,
  and 470-630 Hz for either dial tone or busy tone.
  In order to reuse the existed tone tablese, we have to detect 340-360 Hz
  or 430-450 Hz as dial tones, and 315-695 Hz for either dial tone or busy tone.
 */

/*
  BIL requires 5 seconds continuous detection of Dial Tone, Busy Tone, Reorder/
  Congestion Tone or Silence to terminate ICM recording.  It is also required
  to detect CNG tone at the same time.
 */

/* Note: Test Phone Number on TLS-5 and PTT-5102
                                    TLS-5   PTT-5102
         Dial Tone:                 83781   -
         Busy Tone:                 83782   -
         Congestion/Reorder Tone:   83783   7367337
         Ring Back Tone:            83784   -
 */

/*-------------------------------------------------------------------*/

/*********************************************************************/

/** This section is for Modem BUSY TONE/DIAL TONE/CNG/HDLC detection **/

/*  Definition of MaxCNGHalfCycle, CNGHalfCycle, BusyToneHalfCycle:
     0 - The beginning of 1'st CNG/BUSY ON
     1 - The beginning of 1'st CNG/BUSY OFF
     2 - The beginning of 2'nd CNG/BUSY ON  (about 1 sec. busy tone)
     3 - The beginning of 2'nd CNG/BUSY OFF
     4 - The beginning of 3'rd CNG/BUSY ON  (about 2 sec. busy tone)
     5 - The beginning of 3'rd CNG/BUSY OFF
     6 - The beginning of 4'rd CNG/BUSY ON  (about 3 sec. busy tone)
     7 - The beginning of 4'rd CNG/BUSY OFF
     8 - The beginning of 5'rd CNG/BUSY ON  (about 4 sec. busy tone)
     9 - The beginning of 5'rd CNG/BUSY OFF
    10 - The beginning of 6'rd CNG/BUSY ON  (about 5 sec. busy tone)
 */

static UINT16 DialToneOnTime, DialToneOffTime;      /* milli-seconds */
static UINT16 BusyToneOnTime, BusyToneOffTime;      /* milli-seconds */

static UINT16 MaxDialToneTime, MaxFSKTime;          /* milli-seconds */
static UINT16 MaxBusyToneTime, TotalBusyToneTime;   /* milli-seconds */
static UINT16 MaxCongToneTime, TotalCongToneTime;   /* milli-seconds */

static UINT8 MaxCNGHalfCycle;      /* Half cycle counts */

#ifdef _DEBUG
static UINT8 BusyToneHalfCycle;    /* Half cycle counts */
static UINT8 CongToneHalfCycle;    /* Half cycle counts */
#endif


/*-------------------------------------------------------------------
    Name: ResetAllDetection - Reset detection parameters to stop
          simutaneous Busy Tone, Dial Tone, and CNG Tone detection.

    Input:  none
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static void ResetAllDetection( void )
{
	TRACE_PHONE( "Reset Busy/Dial/CNG Tone Detection\n" );

	BusyToneOnTime = 0;
	DialToneOnTime = 0;
	BusyToneOffTime = 0;
	DialToneOffTime = 0;
	TotalBusyToneTime = 0;
	TotalCongToneTime = 0;

#ifdef _DEBUG
	BusyToneHalfCycle = 0;
	CongToneHalfCycle = 0;
#endif

	CNGHalfCycle = 0;
	CNGOnTime = 0;
	CNGOffTime = TONE_DEBOUNCE_TIME;    /* To detect the first CNG ON */
}


/*-------------------------------------------------------------------
    Name: CheckAnsDetection - Check line signal detection
          Notify higher levle phone about result (ANSam/CED Tone).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static UINT8 CheckANSDetection( UINT8 SamplingTimeInMS )
{
	/** ANSam or CED tone detection **/

	if ( ModemToneDetected( MODEM_ANS_TONE ) && ModemCompareAGCGain() )
	{
		if ( ANSOnTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			ANSOnTime += SamplingTimeInMS;
		}

		if ( ANSOnTime > PTP.AnswerStable )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckAnsDetection: accepted ANS\n" );
#endif
			ModemControl->Level = ModemReadRxLevel();
			ModemControl->EQM   = 0xffff;
			return TONE_DETECT_ANS;
		}
#ifdef _DEBUG
		else
		{
//			TRACE_DEBUG( " ANS %d %d\n", PTP.AnswerStable, ANSOnTime );
		}
#endif
	}
	else if ( ANSOnTime >= SamplingTimeInMS )
	{
		ANSOnTime -= SamplingTimeInMS;
	}

	return TONE_DETECT_OFF;
}


/*-------------------------------------------------------------------
    Name: CheckDialBusyDetection - Check line signal detection
          Notify higher levle phone about result (Dial/Busy Tone).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static UINT8 CheckDialBusyDetection( UINT8 SamplingTimeInMS )
{
	if ( ( MaxBusyToneTime == 0 )
	  && ( MaxCongToneTime == 0 )
	  && ( MaxDialToneTime == 0 ) )
	{
#ifdef _DEBUG
		TRACE_PHONE( "CheckDialBusyDetection: all zero\n");
#endif
		return TONE_DETECT_OFF;
	}

	/** Dial tone detection **/

	if ( MaxDialToneTime )
	{
		if ( ModemToneDetected( MODEM_DIAL_TONE ) && ModemCompareAGCGain() )
		{   /* TONEB: Dial tone detected */
#ifdef _DEBUG
			if ( DialToneOnTime == 0 )
			{
				TRACE_PHONE( "New Dial Tone detected ON TONEA. OffTime: %d ms\n",
				             DialToneOffTime );
			}
			else if ( ( DialToneOnTime <= 6000 )
			     && ( ( DialToneOnTime % 1000 ) == 0 ) )
			{
				TRACE_PHONE( "CheckDialBusyDetection:1 TONEA Dial Tone %d ms\n",
				             DialToneOnTime );    /* Display every second */
			}
#endif

			SilenceTime = 0;

			if ( DialToneOnTime < ( 0xFFFF - SamplingTimeInMS ) )
			{
				DialToneOnTime += SamplingTimeInMS;
			}

#ifdef _DEBUG
			else
			{
				TRACE_PHONE( "Warning! DialToneOnTime Overflow!\n" );
			}
#endif
			if ( ( DialToneOffTime != 0 )
			  && ( DialToneOnTime >= TONE_DEBOUNCE_TIME ) )
			{
				/* Ignore the short Dial Tone OFF, and count as Dial Tone ON */
				if ( DialToneOffTime < TONE_DEBOUNCE_TIME )
				{
#ifdef _DEBUG
					TRACE_PHONE( "Noisy OFF detection! DialToneOffTime:%u ms\n",
					             DialToneOffTime );
#endif
					DialToneOnTime += DialToneOffTime;    /* should not overflow */
				}
				DialToneOffTime = 0;
			}

            if ( DialToneOnTime >= (UINT16)(MaxDialToneTime * 3 / 4) )
			{
#ifdef _DEBUG
				TRACE_PHONE( "CheckDialBusyDetection:2 TONEA Dial Tone %d ms\n",
				             DialToneOnTime );
#endif

				MaxDialToneTime = 0;
				ResetAllDetection();
				return TONE_DETECT_DIAL;
			}
		}
		else    /* Dial Tone OFF */
		{
#ifdef _DEBUG
			if ( DialToneOffTime == 0 )
				TRACE_PHONE( "New Dial Tone OFF detected. TONEA OnTime: %d ms\n",
				             DialToneOnTime );
#endif

			if ( DialToneOffTime < ( 0xFFFF - SamplingTimeInMS ) )
			{
				DialToneOffTime += SamplingTimeInMS;
			}

#ifdef _DEBUG
			else
			{
				TRACE_PHONE( "Warning! DialToneOffTime Overflow!\n" );
			}
#endif

			if ( ( DialToneOnTime != 0 )
			  && ( DialToneOffTime >= TONE_DEBOUNCE_TIME ) )
			{
				/* Ignore the short Dial Tone ON, and count as Dial Tone OFF */
				if ( DialToneOnTime < TONE_DEBOUNCE_TIME )
				{
#ifdef _DEBUG
					TRACE_PHONE( "Noisy ON detection! DialToneOnTime:%u ms\n",
					             DialToneOnTime );
#endif
					DialToneOffTime += DialToneOnTime;    /* should not overflow */
				}
				DialToneOnTime = 0;
			}
		}
	}

	/** Busy/Dial tone detection **/

	if ( ( MaxBusyToneTime == 0 )
	  && ( MaxCongToneTime == 0 )
	  && ( MaxDialToneTime == 0 ) )
		return TONE_DETECT_OFF;

	if ( ModemToneDetected( MODEM_BUSY_TONE ) && ModemCompareAGCGain() )
	{   /* Busy/Dial tone detected */
#ifdef _DEBUG
		if ( BusyToneOnTime == 0 )
		{
			TRACE_PHONE( "New Busy/Dial Tone detected on TONEA. OffTime: %d ms\n",
			             BusyToneOffTime );
		}
		else if ( ( BusyToneOnTime <= 6000 )
		       && ( ( BusyToneOnTime % 1000 ) == 0 ) )
		{
			TRACE_PHONE( "CheckDialBusyDetection: TONEA Busy/Dial Tone %d ms\n",
			             BusyToneOnTime );        /* Display every half second */
		}
#endif
		SilenceTime = 0;

		if ( BusyToneOnTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			BusyToneOnTime += SamplingTimeInMS;
		}

#ifdef _DEBUG
		else
		{
			TRACE_PHONE( "Warning! DialBusyOnTime Overflow!\n" );
		}
#endif

		if ( ( BusyToneOffTime != 0 )
		  && ( BusyToneOnTime >= TONE_DEBOUNCE_TIME ) )
		{
			/* Ignore the short Busy Tone OFF, and count as Busy Tone ON */
			if ( BusyToneOffTime < TONE_DEBOUNCE_TIME )
			{
#ifdef _DEBUG
				TRACE_PHONE( "Noisy OFF detection! BusyToneOffTime:%u ms\n",
				             BusyToneOffTime );
#endif
				BusyToneOnTime += BusyToneOffTime;    /* should not overflow */
			}
			else if ( ( BusyToneOffTime > PTP.BusyToneOffMin )
			       && ( BusyToneOffTime < PTP.BusyToneOffMax ) )
			{
				TotalBusyToneTime += BusyToneOffTime;
				TotalCongToneTime = 0;  /* False detection start over again */
#ifdef _DEBUG
				BusyToneHalfCycle++;
				CongToneHalfCycle = 0;  /* False detection start over again */
				TRACE_PHONE( "CheckDialBusyDetection: BusyTone HalfCycleCount:"
				             " %u,\n     BusyToneOffTime: %u, TotalBusyToneTime: %u\n",
				             BusyToneHalfCycle, BusyToneOffTime, TotalBusyToneTime );
#endif
			}
			else if ( ( BusyToneOffTime > PTP.CongToneOffMin )
			       && ( BusyToneOffTime < PTP.CongToneOffMax ) )
			{
				TotalCongToneTime += BusyToneOffTime;
				TotalBusyToneTime = 0;  /* False detection start over again */
#ifdef _DEBUG
				CongToneHalfCycle++;
				BusyToneHalfCycle = 0;  /* False detection start over again */
				TRACE_PHONE( "CheckDialBusyDetection: CongTone HalfCycleCount:"
				             " %u,\n     CongToneOffTime: %u, TotalCongToneTime: %u\n",
				             CongToneHalfCycle, BusyToneOffTime, TotalCongToneTime );
#endif
			}
			else
			{
#ifdef _DEBUG
				TRACE_PHONE( "False OFF detection! BusyTone Half Cycle Count:%u,"
				             " OnTime:%u ms, OffTime:%u ms\n",
				             BusyToneHalfCycle, BusyToneOnTime, BusyToneOffTime );
				BusyToneHalfCycle = 0;  /* False detection start over again */
				CongToneHalfCycle = 0;  /* False detection start over again */
#endif
				TotalBusyToneTime = 0;  /* False detection start over again */
			}

			BusyToneOffTime = 0;
		}   /* >= TONE_DEBOUNCE_TIME */

		/* Check for Busy Tone report */

		if ( MaxBusyToneTime
		  && ( BusyToneOnTime < PTP.BusyToneOnMax )
		  && ( ( TotalBusyToneTime + BusyToneOnTime ) >= MaxBusyToneTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckDialBusyDetection:1 Busy Tone %d ms\n",
			             TotalBusyToneTime + BusyToneOnTime );
#endif
			ResetAllDetection();
			return TONE_DETECT_BUSY;
		}

		/* Check for Congestion/Reorder Tone report.  Report as Busy Tone */

		if ( MaxCongToneTime
		  && ( BusyToneOnTime < PTP.CongToneOnMax )
		  && ( ( TotalCongToneTime + BusyToneOnTime ) >= MaxCongToneTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckDialBusyDetection:1 Congestion Tone %d ms\n",
			             TotalCongToneTime + BusyToneOnTime );
#endif
		}

		/* Check for Dial Tone report */

		if ( MaxDialToneTime && ( BusyToneOnTime >= MaxDialToneTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckDialBusyDetection: TONEA Dial Tone %d ms\n",
			             BusyToneOnTime );
#endif
		}
	}
	else    /* Busy Tone OFF */
	{
#ifdef _DEBUG
		if ( BusyToneOffTime == 0 )
		{
			TRACE_PHONE( "New Busy/Dial Tone OFF detected. TONEA OnTime: %d ms\n",
			             BusyToneOnTime );
		}
		else if ( ( BusyToneOffTime <= 6000 )
		       && ( ( BusyToneOffTime % 1000 ) == 0 ) )
		{
			TRACE_PHONE( "CheckDialBusyDetection: TONEA BusyToneOff %d ms\n",
			             BusyToneOffTime );       /* Display every half second */
		}
#endif

		if ( BusyToneOffTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			if ( BusyToneOffTime == 0 )
				BusyToneOffTime += 30;

			BusyToneOffTime += SamplingTimeInMS;
		}

#ifdef _DEBUG
		else
		{
			TRACE_PHONE( "Warning! DialBusyOffTime Overflow!\n" );
		}
#endif
		if ( ( BusyToneOnTime != 0 )
		  && ( BusyToneOffTime >= TONE_DEBOUNCE_TIME ) )
		{
			/* Ignore the short Busy Tone ON, and count as Busy Tone OFF */
			if ( BusyToneOnTime < TONE_DEBOUNCE_TIME )
			{
#ifdef _DEBUG
				TRACE_PHONE( "Noisy ON detection! BusyToneOnTime:%u ms\n",
				             BusyToneOnTime );
#endif
				BusyToneOffTime += BusyToneOnTime;  /* should not overflow */
			}
			else if ( ( BusyToneOnTime > PTP.BusyToneOnMin )
			       && ( BusyToneOnTime < PTP.BusyToneOnMax ) )
			{
				TotalBusyToneTime += BusyToneOnTime;
				TotalCongToneTime = 0;  /* False detection start over again */
#ifdef _DEBUG
				BusyToneHalfCycle++;
				CongToneHalfCycle = 0;  /* False detection start over again */
				TRACE_PHONE( "CheckDialBusyDetection: BusyTone HalfCycleCount:"
				             " %u,\n     BusyToneOnTime: %u, TotalBusyToneTime: %u\n",
				             BusyToneHalfCycle, BusyToneOnTime, TotalBusyToneTime );
#endif
			}
			else if ( ( BusyToneOnTime > PTP.CongToneOnMin )
			       && ( BusyToneOnTime < PTP.CongToneOnMax ) )
			{
				TotalCongToneTime += BusyToneOnTime;
				TotalBusyToneTime = 0;  /* False detection start over again */
#ifdef _DEBUG
				CongToneHalfCycle++;
				BusyToneHalfCycle = 0;  /* False detection start over again */
				TRACE_PHONE( "CheckDialBusyDetection: CongTone HalfCycleCount:"
				             " %u,\n     CongToneOnTime: %u, TotalCongToneTime: %u\n",
				             CongToneHalfCycle, BusyToneOnTime, TotalCongToneTime );
#endif
			}
			else
			{
#ifdef _DEBUG
				TRACE_PHONE( "False ON detection! BusyTone Half Cycle Count:%u,"
				             " OnTime:%u ms, OffTime:%u ms\n",
				             BusyToneHalfCycle, BusyToneOnTime, BusyToneOffTime );
				BusyToneHalfCycle = 0;  /* False detection start over again */
				CongToneHalfCycle = 0;  /* False detection start over again */
#endif
				TotalBusyToneTime = 0;  /* False detection start over again */
			}

			BusyToneOnTime = 0;
		}   /* >= TONE_DEBOUNCE_TIME */

		/* Check for Busy Tone report */

		if ( MaxBusyToneTime
		  && ( BusyToneOffTime < PTP.BusyToneOffMax )
		  && ( ( TotalBusyToneTime + BusyToneOffTime ) >= MaxBusyToneTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckDialBusyDetection:2 Busy Tone %d ms\n",
			             TotalBusyToneTime + BusyToneOffTime );
#endif
			ResetAllDetection();
			return TONE_DETECT_BUSY;
		}

		/* Check for Congestion/Reorder Tone report.  Report as Busy Tone */

		if ( MaxCongToneTime
		  && ( BusyToneOffTime < PTP.CongToneOffMax )
		  && ( ( TotalCongToneTime + BusyToneOffTime ) >= MaxCongToneTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckDialBusyDetection:2 Congestion Tone %d ms\n",
			             TotalCongToneTime + BusyToneOnTime );
#endif
			return TONE_DETECT_BUSY;
		}
	}

	return TONE_DETECT_OFF;
}


/*-------------------------------------------------------------------
    Name: CheckFSKDetection - Check line signal detection
          Notify higher levle phone about result (FSK flag).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static UINT8 CheckFSKDetection( UINT8 SamplingTimeInMS )
{
	UINT8 FEDStatus = ModemFEDStatus();
	UINT8 VOXStatus = ModemVOXStatus();

	if ( ( MaxSilenceTime == 0 )
	  && ( MaxFSKTime     == 0 ) )
		return TONE_DETECT_OFF;

	/** HDLC flag detection **/
	if ( ModemFSKFlags() )         /* HDLC flag detected */
	{
#ifdef _DEBUG
//		TRACE_DEBUG( " FSK %d %d\n", MaxFSKTime, FSKTime );
#endif
		if ( FSKTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			FSKTime += SamplingTimeInMS;   /* should not overflow */
		}

#ifdef _DEBUG
		else
		{
			TRACE_PHONE( "Warning! FSKTime Overflow!\n" );
		}
#endif
		if ( MaxFSKTime && ( FSKTime >= MaxFSKTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckFSKDetection: accepted FSK\n" );
#endif
			FSKTime = 0;
			return TONE_DETECT_FSK;
		}
		else if ( FSKTime >= TONE_DEBOUNCE_TIME )
		{
			if ( SilenceTime < TONE_DEBOUNCE_TIME )
			{
				FSKTime += SilenceTime;
			}
			else if ( FEDTime < TONE_DEBOUNCE_TIME )
			{
				FSKTime += FEDTime;
			}

			SilenceTime = 0;
			FEDTime = 0;

#ifdef _DEBUG
			if ( ( FSKTime <= 6000 )
			  && ( ( FSKTime % 1000 ) == 0 ) )
				TRACE_PHONE( "CheckFSKDetection: HDLC Time %d sec\n",
				             FSKTime / 1000 );     /* Display every second */
#endif
		}
	}
	/** Silence detection **/
	else if ( ( FEDStatus == 0 )
	       && ( VOXStatus == 0 ) )
	{   /* Silence detected: FED = 00, and VOX = 0 */
		if ( SilenceTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			SilenceTime += SamplingTimeInMS;
		}

#ifdef _DEBUG
		else
		{
			TRACE_PHONE( "Warning! SilenceTime Overflow!\n" );
		}
#endif

		if ( MaxSilenceTime && ( SilenceTime >= MaxSilenceTime ) )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckFSKDetection: Silence %d ms\n", SilenceTime );
#endif
			SilenceTime = 0;
		}
		else if ( SilenceTime >= TONE_DEBOUNCE_TIME )
		{
			if ( FEDTime < TONE_DEBOUNCE_TIME )
			{
				SilenceTime += FEDTime;
			}
			else if ( FSKTime < TONE_DEBOUNCE_TIME )
			{
				SilenceTime += FSKTime;
			}

			FEDTime = 0;
			FSKTime = 0;

#ifdef _DEBUG
			if ( ( SilenceTime <= 6000 )
			  && ( ( SilenceTime % 1000 ) == 0 ) )
				TRACE_PHONE( "CheckFSKDetection: Silence %d sec\n",
				             SilenceTime / 1000 );      /* Display every second */
#endif
		}
	}
	else                            /* energy detected: FED = 10 or 11 */
	{
		if ( FEDTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			FEDTime += SamplingTimeInMS;
		}

#ifdef _DEBUG
		else
		{
			TRACE_PHONE( "Warning! FEDTime Overflow!\n" );
		}
#endif
		if ( FEDTime >= TONE_DEBOUNCE_TIME )
		{
			if ( SilenceTime < TONE_DEBOUNCE_TIME )
			{
				FEDTime += SilenceTime;
			}

			SilenceTime = 0;

#ifdef _DEBUG
			if ( ( FEDTime <= 6000 )
			  && ( ( FEDTime % 1000 ) == 0 ) )
			{   /* Display every second */
				if ( FEDStatus == 0x80 )   /* Below Turn-on Threshold: FED = 10 */
					TRACE_PHONE( "CheckFSKDetection: FED Below Turn-on Threshold\n" );
			}
#endif
		}
	}

	return TONE_DETECT_OFF;
}


/*-------------------------------------------------------------------
    Name: CheckCNGDetection - Check line signal detection
          Notify higher levle phone about result (CNG tone).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static UINT8 CheckCNGDetection( UINT8 SamplingTimeInMS )
{
	if ( MaxCNGHalfCycle )
	{
		if ( ModemToneDetected( MODEM_CNG_TONE ) && ModemCompareAGCGain() )
		{   /* TONEB CNG tone detected */
#ifdef _DEBUG
			if ( CNGOnTime == 0 )
			{
				TRACE_PHONE( "New CNG ON detected on TONEB. OffTime: %d ms\n",
				             CNGOffTime );
			}
			else if ( ( CNGOnTime <= 2000 )
			       && ( ( CNGOnTime % 1000 ) == 0 ) )
			{
				TRACE_PHONE( "CheckCNGDetection: TONEB CNG On %d ms\n",
				             CNGOnTime );         /* Display every half second */
			}
#endif
			SilenceTime = 0;

			if ( CNGOnTime < ( 0xFFFF - SamplingTimeInMS ) )
			{
				CNGOnTime += SamplingTimeInMS;
			}

#ifdef _DEBUG
			else
			{
				TRACE_PHONE( "Warning! CNGOnTime Overflow!\n" );
			}
#endif
			if ( ( CNGOffTime != 0 )
			  && ( CNGOnTime >= TONE_DEBOUNCE_TIME ) )
			{
				if ( CNGOffTime < TONE_DEBOUNCE_TIME )
				{
#ifdef _DEBUG
					TRACE_PHONE( "Noisy OFF detection! CNGOffTime:%u ms\n",
					             CNGOffTime );
#endif
					/* Ignore the short CNG OFF glitch, and count as CNG ON */
					CNGOnTime += CNGOffTime;    /* should not overflow */
				}
				else if ( ( CNGHalfCycle == 0 )
				       || ( CNGOffTime < PTP.CNGOffMin )
					   || ( CNGOffTime > PTP.CNGOffMax ) )
				{
#ifdef _DEBUG
					if ( CNGHalfCycle != 0 )
					{
						TRACE_PHONE( "CNG false OFF Detection: CNGHalfCycle: %u,"
						             " CNGOffTime: %u\n", CNGHalfCycle, CNGOffTime );
					}
					TRACE_PHONE( "CheckCNGDetection: CNGHalfCycle: %d/0\n",
					             CNGHalfCycle );
#endif
					/* First CNG ON or false detection restart */
					CNGHalfCycle = 1;
				}
				else
				{
#ifdef _DEBUG
					TRACE_PHONE( "CheckCNGDetection: CNGHalfCycle: %d\n",
					             CNGHalfCycle );
#endif
					/* CNGHalfCycle: 0, 2, 4 */
					if ( CNGHalfCycle >= MaxCNGHalfCycle )
					{
#ifdef _DEBUG
						TRACE_PHONE( "CheckCNGDetection: accepted CNG\n" );
#endif
						CNGOffTime = 0;
						return TONE_DETECT_CNG;
					}

					CNGHalfCycle++;
				}

				CNGOffTime = 0;
			}
		}
		else    /* CNG OFF */
		{
#ifdef _DEBUG
			if ( CNGOffTime == 0 )
			{
				TRACE_PHONE( "New CNG OFF detected on TONEC. OnTime: %d ms\n",
				             CNGOnTime );
			}
			else if ( ( CNGOffTime <= 6000 )
			     && ( ( CNGOffTime % 1000 ) == 0 ) )
			{
				TRACE_DEBUG( "CheckCNGDetection: TONEC CNG OFF %d ms\n",
				                  CNGOffTime );    /* Display every second */
			}
#endif
			if ( CNGOffTime < ( 0xFFFF - SamplingTimeInMS ) )
			{
				CNGOffTime += SamplingTimeInMS;
			}

#ifdef _DEBUG
			else
			{
				TRACE_DEBUG( "Warning! CNGOffTime Overflow!\n" );
			}
#endif
			if ( CNGOffTime > PTP.CNGOffMax )
			{
				if ( CNGHalfCycle != 0 )
				{
					CNGHalfCycle = 0;       /* False detection, start over again */
					CNGOnTime = 0;
				}
			}
			else if ( ( CNGOnTime != 0 )
			       && ( CNGOffTime >= TONE_DEBOUNCE_TIME ) )
			{
				/* Ignore the short CNG ON glitch, and count as CNG OFF */
				if ( CNGOnTime < TONE_DEBOUNCE_TIME )
				{
#ifdef _DEBUG
					TRACE_PHONE( "Noisy ON detection! CNGOnTime:%u ms\n",
					             CNGOnTime );
#endif
					CNGOffTime += CNGOnTime;    /* should not overflow */
				}
				else if ( ( CNGOnTime < PTP.CNGOnMin )
				       || ( CNGOnTime > PTP.CNGOnMax ) )
				{
#ifdef _DEBUG
					TRACE_PHONE( "CNG false ON Detection: CNGHalfCycle: %u,"
					             " CNGOnTime: %u\n", CNGHalfCycle, CNGOnTime );
#endif
					CNGHalfCycle = 0;       /* False detection, start over again */
				}
				else
				{
#ifdef _DEBUG
					TRACE_PHONE( "CheckCNGDetection: CNGHalfCycle: %d\n",
					             CNGHalfCycle );
#endif
					/* CNGHalfCycle: 1, 3, 5 */
					if ( CNGHalfCycle >= MaxCNGHalfCycle )
					{
#ifdef _DEBUG
						TRACE_PHONE( "CheckCNGDetection: accepted CNG\n" );
#endif
						CNGOnTime = 0;
						return TONE_DETECT_CNG;
					}

					CNGHalfCycle++;
				}
				CNGOnTime = 0;
			}
		}
	}

	return TONE_DETECT_OFF;
}


/*-------------------------------------------------------------------
    Name: CheckCIDetection - Check line signal detection
          Notify higher levle phone about result (CI tone).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: none
    Return: none
 *-------------------------------------------------------------------*/

static UINT8 CheckCIDetection( UINT8 SamplingTimeInMS )
{
	if ( ModemToneDetected( MODEM_CI_TONE ) && ModemCompareAGCGain() )
	{
		if ( CIOnTime < ( 0xFFFF - SamplingTimeInMS ) )
		{
			CIOnTime += SamplingTimeInMS;
		}

		if ( CIOnTime > TONE_DEBOUNCE_TIME )
		{
#ifdef _DEBUG
			TRACE_PHONE( "CheckCIDetection: accepted CI\n" );
#endif
			return TONE_DETECT_CI;
		}
#ifdef _DEBUG
		else
		{
			TRACE_DEBUG( "CheckCIDetection: found CI %d %d\n", TONE_DEBOUNCE_TIME, CIOnTime );
		}
#endif
	}
	else if ( CIOnTime >= SamplingTimeInMS )
	{
		CIOnTime -= SamplingTimeInMS;
	}

	return TONE_DETECT_OFF;
}


/*-------------------------------------------------------------------
    Name: SetMaxSilenceTime - Setup Silence detection confirmation time for
            CheckFSKDetection()
    Input:  TimeInMs - Confirmation time in milli-seconds
                   0 - Reset time of Silence detection
    Output: MaxSilenceTime
    Return: none
 *-------------------------------------------------------------------*/

static void SetMaxSilenceTime( UINT16 TimeInMs )
{
	SilenceTime = 0;

	if ( TimeInMs == 1 )
	{
		MaxSilenceTime = PTP.MaxSilence;
	}
	else if ( TimeInMs > 1 )
	{
		MaxSilenceTime = TimeInMs;
	}


#ifdef _DEBUG
	TRACE_PHONE( " MaxSilenceTime: %u ms\n", MaxSilenceTime );
#endif
}


/*-------------------------------------------------------------------
    Name: SetMaxFSKTime - Setup HDLC Flag detection confirmation time for
            CheckFSKDetection()
    Input:  TimeInMs - Confirmation time in milli-seconds
                   0 - Reset time of HDLC Flag detection
    Output: MaxFSKTime
    Return: none
 *-------------------------------------------------------------------*/

static void SetMaxFSKTime( UINT16 TimeInMs )
{
	FSKTime = 0;

	if ( TimeInMs == 1 )
	{
		MaxFSKTime = PTP.FlagVerifyTime;
	}
	else if ( TimeInMs > 1 )
	{
		MaxFSKTime = TimeInMs;
	}

#ifdef _DEBUG
	TRACE_PHONE( " MaxFSKTime: %u ms\n", MaxFSKTime );
#endif
}


/*-------------------------------------------------------------------
    Name: SetMaxDialToneTime - Setup Dial Tone detection confirmation time for
            CheckDialBusyDetection()
    Input:  TimeInMs - Confirmation time in milli-seconds
                   0 - Reset time of Dial Tone detection
    Output: MaxDialToneTime
    Return: none
 *-------------------------------------------------------------------*/

static void SetMaxDialToneTime( UINT16 TimeInMs )
{
	DialToneOffTime = 0;
	DialToneOnTime  = 0;

	if ( TimeInMs == 1 )
	{
		MaxDialToneTime = PTP.DialToneLen;
	}
	else if ( TimeInMs > 1 )
	{
		MaxDialToneTime = TimeInMs;
	}

#ifdef _DEBUG
	TRACE_PHONE( " MaxDialToneTime: %u ms\n", MaxDialToneTime );
#endif
}


/*-------------------------------------------------------------------
    Name: SetMaxBusyToneTime - Setup Busy Tone detection time for
           CheckDialBusyDetection()
    Input:  TimeInMs - Confirmation time in milli-seconds
                   0 - Reset time of Dial Tone detection
    Output: MaxBusyToneTime
    Return: none
 *-------------------------------------------------------------------*/

void SetMaxBusyToneTime( UINT16 TimeInMs )
{
	TotalBusyToneTime = 0;
	BusyToneOffTime   = 0;
	BusyToneOnTime    = 0;
	TotalCongToneTime = 0;

	if ( TimeInMs == 1 )
	{
		MaxBusyToneTime = PTP.BusyToneLen;
		MaxCongToneTime = PTP.BusyToneLen;
		if( MaxBusyToneTime < 2400 ) {
			MaxBusyToneTime = 2400;
			MaxCongToneTime = 2400;
		}
	}
	else if ( TimeInMs > 1 )
	{
		MaxBusyToneTime = TimeInMs;
		MaxCongToneTime = TimeInMs;
	}

#ifdef _DEBUG
	TRACE_PHONE( " MaxBusyToneTime: %u ms\n", MaxBusyToneTime );
    TRACE_PHONE( " MaxCongToneTime: %u ms\n", MaxCongToneTime );
#endif
}


/*-------------------------------------------------------------------
    Name: SetMaxCNGHalfCycle - Setup CNG detection half cycle counts for
            CheckBusyDialCNGDetection()
          The Half Cycle Counts is defined in the beginning of this section.
    Input:  HalfCycleCount
            0 - Reset time of CNG Tone detection
    Output: MaxCNGHalfCycle
    Return: none
 *-------------------------------------------------------------------*/

void SetMaxCNGHalfCycle( UINT8 HalfCycleCount )
{
	CNGHalfCycle = 0;
	CNGOnTime    = 0;
	CNGOffTime   = 0;

	if ( HalfCycleCount > 0 )
	{
		MaxCNGHalfCycle = HalfCycleCount;
	}

#ifdef _DEBUG
	TRACE_PHONE( " MaxCNGHalfCycle: %u\n", MaxCNGHalfCycle );
#endif
}


/*********************************************************************/

/*-------------------------------------------------------------------
    Name: SetUpToneDetection - Set up modem for CNG/HDLC/Silence detection
          or DIAL/BUSY/Silence detection.

    Input:  ToneType - TONE_DETECT_OFF       : Disable detection
                       TONE_DETECT_CI        : V8
                       TONE_DETECT_CNG       : CNG/Silence
                       TONE_DETECT_FSK       : FSK/Silence
                       TONE_DETECT_DIAL_BUSY : DIAL/BUSY/Silence
            DetTimeout - detection timeout.
                           0: No timeout
                      0xffff: disable detection
    Output: none
    Return: E_OK, E_PARAM
 *-------------------------------------------------------------------*/

ERCODE SetUpToneDetection( TONE_TYPES ToneType, UINT16 DetTimeout )
{
#ifdef _DEBUG
	TRACE_PHONE( "SetUpToneDetection: Type %d, Timeout %dms\n", ToneType, DetTimeout );
#endif

	if ( DetTimeout == 0xffff )
	{
		DetectionTime = TONE_DETECTION_OFF;
		ToneType = TONE_DETECT_OFF;
	}

	if ( DetTimeout == 0 )
	{
		switch ( ToneType )
		{
			case TONE_DETECT_DIAL:

				SetMaxDialToneTime( 0 );
				break;

			case TONE_DETECT_BUSY:

				SetMaxBusyToneTime( 0 );
				break;

			case TONE_DETECT_ANS:

				ANSOnTime = 0;
				break;

			case TONE_DETECT_CI:

				CIOnTime = 0;
				break;

			case TONE_DETECT_CNG:

				SetMaxCNGHalfCycle( 0 );
				break;

			case TONE_DETECT_FSK:

				SetMaxFSKTime( 0 );
				break;

			case TONE_DETECT_DIAL_BUSY:

				SetMaxDialToneTime( 0 );
				SetMaxBusyToneTime( 0 );
				break;

			case TONE_DETECT_ANS_FSK_BUSY:

				ANSOnTime = 0;
				SetMaxFSKTime( 0 );
				SetMaxBusyToneTime( 0 );
				break;

			default:
				return E_OK;
		}
	}
	else
	{
		switch ( ToneType )
		{
			case TONE_DETECT_DIAL_BUSY:

				ModemSetupToneDet( MODEM_DIAL_TONE ); /* Dial Tone */
				SetMaxDialToneTime( 1 );
				DetectionTime = PTP.DialToneTimeOut;
				break;

			case TONE_DETECT_ANS_FSK_BUSY:

				ModemSetupToneDet( MODEM_ANS_TONE );  /* ANSam/CED signal */
				ModemSetupToneDet( MODEM_FSK_TONE );  /* V.21 signal */
				ModemSetupToneDet( MODEM_BUSY_TONE ); /* Busy/Dial/Congestion */
				SetMaxSilenceTime( 1 );
				SetMaxFSKTime( 1 );
				ANSOnTime = 0;

                if (DialTimeout == 0)
                {
                    DetectionTime = PTP.BusyToneTimeOut; // Use Country-Dependent Value
                }
                else
                {
                    DetectionTime = DialTimeout; // Use User-Set Value
                }
                DetectionTime = (DetectionTime / 3) * 2; /* for the exact dial timeout */

                printf("\n(YHCHO) %s() - DialTimeout: %d, DetectionTime: %d\n", __FUNCTION__, DialTimeout, DetectionTime);

				break;

			case TONE_DETECT_CI:

				CIOnTime = 0;
				DetectionTime = DetTimeout;
				ModemSetupToneDet( MODEM_CI_TONE );
				break;

			case TONE_DETECT_CNG:

				CNGHalfCycle = 0;
				CNGOnTime = 0;
				CNGOffTime = TONE_DEBOUNCE_TIME;    /* To detect the first CNG ON */
				DetectionTime = DetTimeout;
				ModemSetupToneDet( MODEM_CNG_TONE );
				break;

			case TONE_DETECT_OFF:

				ResetAllDetection();
				SetMaxCNGHalfCycle( 0 );
				SetMaxFSKTime( 0 );
				SetMaxSilenceTime( 0 );
				SetMaxBusyToneTime( 0 );
				SetMaxDialToneTime( 0 );
				SilenceTime = 0;
				FEDTime = 0;
				FSKTime = 0;
				DetectionTime = TONE_DETECTION_OFF;
				set_phone_status( TONE_DETECT_OFF, TRUE );
				break;

			default:

				TRACE_ERROR( "### SetUpToneDetection: unknown type %d ###\n", ToneType );
				return E_PARAM;
		}

#ifdef _DEBUG
        TRACE_PHONE( " DetectionTime %d ms, MaxSilenceTime %d ms\n", DetectionTime, MaxSilenceTime );
#endif
	}

	return E_OK;
}


/*-------------------------------------------------------------------
    Name: CheckToneDetection - Check line signal detection
          Notify higher levle phone about result (CNG/HDLC/BUSY/DIAL/Silence).

    Input:  SamplingTimeInMS - Sampling Time in milli-second.
    Output: see children functions
    Return: none
 *-------------------------------------------------------------------*/

UINT8 CheckToneDetection( TONE_TYPES ToneType, UINT8 SamplingTimeInMS )
{
	UINT8 ret = TONE_DETECT_OFF;

	/** 1. Check if Answer Mode Line Signal detection is necessary **/
	if ( DetectionTime == TONE_DETECTION_OFF )
	{
#ifdef _DEBUG
//		TRACE_DEBUG( "CheckToneDetection: TONE_DETECTION_OFF!\n");
#endif
		return TONE_DETECT_OFF;
	}
	else if ( DetectionTime > 0 )
	{
#ifdef _DEBUG
		if ( DetectionTime < SamplingTimeInMS )
			TRACE_PHONE( "CheckToneDetection: Data error! "
			             "DetectionTime: %d ms, SamplingTimeInMS %d ms\n",
			             DetectionTime, SamplingTimeInMS );
#endif

		DetectionTime -= SamplingTimeInMS;

		if ( DetectionTime < SamplingTimeInMS )
		{
			DetectionTime = TONE_DETECTION_OFF;
			TRACE_PHONE( "CheckToneDetection: TIMEOUT!\n");
			return TONE_DETECT_ABORT;
		}
	}

	/** 2. Check either CNG/HDLC/Silence or DIAL/BUSY/Silence **/
	switch ( ToneType )
	{
		case TONE_DETECT_DIAL_BUSY:

			ret = CheckDialBusyDetection( SamplingTimeInMS );
			break;

		case TONE_DETECT_ANS_FSK_BUSY:

			ret = CheckANSDetection( SamplingTimeInMS );
			if ( ret == TONE_DETECT_ANS )
				break;

			ret = CheckFSKDetection( SamplingTimeInMS );
			if ( ret == TONE_DETECT_FSK )
				break;

			ret = CheckDialBusyDetection( SamplingTimeInMS );
			break;

		case TONE_DETECT_CI:

			ret = CheckCIDetection( SamplingTimeInMS );
			break;

		case TONE_DETECT_CNG:

			ret = CheckCNGDetection( SamplingTimeInMS );
			break;

		case TONE_DETECT_OFF:
		default:

			TRACE_ERROR( "### CheckToneDetection: No ToneType: %d, "
			             "DetectionTime: %d ###\n", ToneType, DetectionTime );
			break;
	}

	return ret;
}

/*********************************************************************/

