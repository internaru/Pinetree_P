/*
 *  $RCSfile: ringdet.c,v $
 * $Revision: 1.8 $
 *   $Author: yooh1 $
 *     $Date: 2010/08/23 21:52:57 $
 */
/************************************************************************
 *
 *      ringdet.c
 *
 *  Incoming Phone Detection functions, including ring detection and
 *  Caller Id. (CID/CND) detection.
 *  The algorithms used here is OS independent and hardware independent.
 *
 *  Copyright 2004 - 2009 Conexant Systems, Inc.
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

#include "fsu_api.h"

extern PRingParams PRP;

//#define _DEBUG
//#define ADD_GLITCH_TIME_IN

/*********************************************************************/

/***    This section of code is for Distinctive Ringing Detection     ***/

/* When there are set of parameters, the first set (e.g. 670-930 ms) is the
   ANSI standard, the second set (e.g. 580-1075 ms) is also commonly used in
   US and Canada.  Not all patterns are defined here.  Such as some PBX ring
   patterns are not defined here.
 */


typedef enum                /* special codes are assigned for debug purpose */
{
	RING_IDLE      = 0,     /* to RING_ON_1 */
	RING_ON_1      = 0x11,  /* to RING_OFF_1 */
	RING_OFF_1     = 0x10,  /* to RING_ON_2 or RING_OFF_1_CID or RING_OFF_1_GO */
	RING_OFF_1_GO  = 0x18,  /* to RING_IDLE */
	RING_ON_2      = 0x21,  /* to RING_OFF_2 */
	RING_OFF_2     = 0x20,  /* to RING_ON_3 or RING_OFF_2_CID or RING_OFF_2_GO */
	RING_OFF_2_GO  = 0x28,  /* to RING_IDLE */
	RING_ON_3      = 0x31,  /* to RING_OFF_3 */
	RING_OFF_3     = 0x30,  /* to RING_OFF_3_CID */
	RING_OFF_3_GO  = 0x38,  /* to RING_IDLE or RING_OFF_2_GO */
	RING_OFF_1_CID = 0xF1,  /* to RING_IDLE */
	RING_OFF_2_CID = 0xF2,  /* to RING_IDLE */
	RING_OFF_3_CID = 0xF3   /* to RING_IDLE */
} DISTINCTIVE_RING_STATES;


/* Basic Patterns and Fuzzy membership set */
typedef enum                /* special codes are assigned for debug purpose */
{
	/** Basic Patterns: **/
	/* ANSI Standard */
	PATTERN_0 = 0x00,       /* Undefined pattern */
	PATTERN_1 = 0x11,       /* RING_ON_1 to RING_OFF_1_GO or RING_OFF_1_CID */
	PATTERN_2,
	PATTERN_3,
	PATTERN_4,
	PATTERN_5,              /* RING_OFF_1_GO to _OFF_1_GO or RING_OFF_1_CID */
	/* PBX */
	PATTERN_X = 0x20,       /* RING_OFF_1_GO to _OFF_1_GO or RING_OFF_1_CID */
	PATTERN_Y,

	/* Pseudo Ring */
	PATTERN_R = 0x30,       /* for ringer pattern generation only */
	PATTERN_T,

	PATTERN_END = 0x40,


	/** Fuzzy membership set: **/

	/* RING_ON_1 generated new membership set */
	PATTERN_2X = 0xA0,      /* life time to RING_OFF_1_GO */
	PATTERN_2XY,            /* life time to RING_OFF_1_GO */
	PATTERN_2345Y,          /* life time to RING_OFF_1_GO */
	PATTERN_345,            /* life time to RING_OFF_1_GO */
	PATTERN_34,             /* life time to RING_ON_2 */
	PATTERN_2345,           /* life time to RING_OFF_1_GO */
	PATTERN_XY,             /* life time to RING_OFF_1_GO */

	/* RING_OFF_1_GO (RingOff1Go) generated new membership set */
	PATTERN_5X = 0xB0,      /* life time to RING_OFF_1_GO */
	/* RING_OFF_1_GO (RingOff1End) generated new membership set */
	PATTERN_2Y,             /* life time to RING_OFF_2_GO */
	PATTERN_234Y,           /* life time to RING_ON_2 */
	PATTERN_234,            /* life time to RING_ON_2 */

	/* RING_ON_2 generated new membership set */
	PATTERN_24Y = 0xC0,     /* life time to RING_OFF_2_GO */
	PATTERN_24,             /* life time to RING_OFF_2_GO */
	PATTERN_23Y,            /* life time to RING_OFF_2_GO */
	PATTERN_23,             /* life time to RING_OFF_2_GO */

	PATTERN_00
} DISTINCTIVE_RING_PATTERNS;

/* Local Variables */
static RING_INFORMATION RingInformation;
static DISTINCTIVE_RING_STATES RingState;

static DISTINCTIVE_RING_PATTERNS RingPattern, OldRingPattern = PATTERN_1;

static UINT16 RingMissTime;    /* milli-seconds */

static UINT8 ReportTime[ 2 ];  /* for UINT16 time */

/*********************************************************************/

/** Gateway to call Caller Id detection functions **/

/* Turn on Type 1 CID detection */
static void StartType1CID( void )
{
	TRACE_PHONE( "Start CID detection.\n" );

	set_phone_state( PHONE_STATE_CID_START );
}


/* Turn off Type 1 CID detection */
void StopType1CID( void )
{
	TRACE_PHONE( "Stop CID detection.\n" );

	CIDShutdown();

	switch ( RingState )
	{
		case RING_OFF_1_CID: /* RI off to on */

			/* continue to RING_OFF_1_GO for error correction */
			TRACE_PHONE( "RING_OFF_1_GO\n" );

			RingState = RING_OFF_1_GO;
			break;

		case RING_OFF_2_CID: /* RI off to on */

			/* continue to RING_OFF_2_GO for error correction */
			TRACE_PHONE( "RING_OFF_2_GO\n" );

			RingState = RING_OFF_2_GO;
			break;

		case RING_OFF_3_CID: /* RI off to on */

			TRACE_PHONE( "RING_OFF_3_GO\n" );

			RingState = RING_OFF_3_GO;
			break;

		default:
			break;
	}
}


static void EndOfThisRingCycle( void )
{
	if ( ( RingPattern == PATTERN_END ) && ( RingInformation.RingOffTime < PRP.RingOffMax ) )
		return ;     /* Try to do error correction */

	/* inform CP for ring in signal, CP will display Ring... */

	TRACE_PHONE( "End of ring cycle. %d\n", RingInformation.RingCount );
	TRACE_PHONE( "RING_IDLE\n" );
	RingState = RING_IDLE;
}


static void PrematureRingEnd( void )
{
	/* Ignore this RingPattern ? */
	TRACE_PHONE( "Warning! Prematured Ring Termination.\n" );
}


static void ReportRingCycleTimeout( void )
{
	TRACE_PHONE( "Warning! RI timeout.\n" );
}


/*********************************************************************/

static void RingOn1End( void )
{
	/* Check "on time" for the first ring burst */
	/* Assume RingPattern start with PATTERN_0 when (RingInformation.RingCount <= 1) */
	/* Pattern Prediction:  Use the knowledge from previous ring cycle
	   "RingPattern" to speed up the pattern recognizatin precess */

	TRACE_PHONE( "RingOn1End: RingOnTime %d, RingPattern 0x%02X\n", RingInformation.RingOnTime, RingPattern );

	if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn0 )
	{
		RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn1 )
	{
		RingPattern = PATTERN_1;
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn2x )
	{
		if ( ( RingPattern == PATTERN_2 )
		  || ( RingPattern == PATTERN_X ) )
		{
			;
		}
		else
		{
			RingPattern = PATTERN_2X;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn2xy )
	{
		if ( ( RingPattern == PATTERN_2 )
		  || ( RingPattern == PATTERN_X )
		  || ( RingPattern == PATTERN_Y ) )
		{
			;
		}
		else
		{
			RingPattern = PATTERN_2XY;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn2345y )
	{
		if ( ( RingPattern == PATTERN_2 )
		  || ( RingPattern == PATTERN_3 )
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_5 )
		  || ( RingPattern == PATTERN_Y ) )
		{
			;
		}
		else
		{
			RingPattern = PATTERN_2345Y;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn345 )
	{
		if ( ( RingPattern == PATTERN_3 )
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_5 ) )
		{
			;
		}
		else
		{
			RingPattern = PATTERN_345;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn1Ptn34 )
	{
		if ( ( RingPattern == PATTERN_3 )
		  || ( RingPattern == PATTERN_4 ) )
		{
			;
		}
		else if ( OldRingPattern == PATTERN_3 )
		{
			RingPattern = PATTERN_3;
		}
		else if ( OldRingPattern == PATTERN_4 )
		{
			RingPattern = PATTERN_4;
		}
		else
		{
			RingPattern = PATTERN_34;
		}
	}
	else    /* ignore this pattern */
	{
		PrematureRingEnd();
	}

	/* PATTERN_1 should be identified now */
#ifdef _DEBUG
	if ( RingPattern == PATTERN_0 )
		TRACE_PHONE( "Warning! RingOn1End()\n" );
//  else
//      TRACE_PHONE("RingOn1End()");
#endif

}

/*-------------------------------------------------------------------*/

/* It might be better to disable the error correction to prevent the confusion
   of prematured ring termination. */

static void RingOff1Go( void )
{
	/* Keep checking "off time" for the first silence */
	/* This timing has little value (too late for CID) to the standard patterns
	   recognition.  However, it would help to identify non-standard ring
	   patterns, especially for PBX.
	 */

	if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn15 )   /* Long Silence */
	{
		if ( RingPattern == PATTERN_5X )
		{
			RingPattern = PATTERN_5;
		}
		else if ( ( RingPattern != PATTERN_1 )
		       && ( RingPattern != PATTERN_5 ) )
		{
			PrematureRingEnd();
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn15x )  /* Long Silence */
	{
		if ( ( RingPattern == PATTERN_2X )
		  || ( RingPattern == PATTERN_2XY )
		  || ( RingPattern == PATTERN_XY ) )
		{
			RingPattern = PATTERN_X;
		}
		else if ( ( RingPattern == PATTERN_2345Y )
		       || ( RingPattern == PATTERN_345 )
		       || ( RingPattern == PATTERN_2345 )
		       || ( RingPattern == PATTERN_34 )   /* Err correction */
		       || ( RingPattern == PATTERN_3 )
		       || ( RingPattern == PATTERN_4 ) )
		{
			RingPattern = PATTERN_5;
		}
		/* Err correction */
		else if ( ( RingPattern == PATTERN_2 )
		       || ( RingPattern == PATTERN_Y ) )
		{
			RingPattern = PATTERN_5X;
		}
		else if ( ( RingPattern != PATTERN_1 )
		       && ( RingPattern != PATTERN_5 )
		       && ( RingPattern != PATTERN_X )
		       && ( RingPattern != PATTERN_5X ) )
		{
			RingPattern = PATTERN_0;
		}

#ifdef _DEBUG
		if ( RingPattern == PATTERN_0 )
			TRACE_PHONE( "Warning! case RING_OFF_1_GO:\n" );
#endif

	}
	/* (PATTERN_1,) PATTERN_5, _X, should be identified */
}

/*-------------------------------------------------------------------*/

static BOOL RingOff1End( void )
{
	/* Check "off time" for the first silence */
	/* This timing has little value (too late for CID) to the standard patterns
	   recognition.  However, it would help to identify non-standard ring
	   patterns, especially for PBX.
	 */

	TRACE_PHONE( "RingOff1End: RingOffTime %d, RingPattern 0x%02X\n", RingInformation.RingOffTime, RingPattern );

	if ( RingInformation.RingOffTime >= PRP.RingOn1Ptn0 )
	{
		TRACE_PHONE( "Sync. Error! RingOff1End() Too Long!\n" );
		RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn15 )   /* for RING_OFF_1_CID */
	{
		if ( ( RingPattern == PATTERN_2345Y )
		  || ( RingPattern == PATTERN_345 )
		  || ( RingPattern == PATTERN_2345 )
		  || ( RingPattern == PATTERN_5X )
		  || ( RingPattern == PATTERN_34 )   /* Err correction */
		  || ( RingPattern == PATTERN_2 )
		  || ( RingPattern == PATTERN_3 )
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_Y ) )
		{
			RingPattern = PATTERN_5;
		}
		else if ( ( RingPattern != PATTERN_1 )
		       && ( RingPattern != PATTERN_5 ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptnx )
	{
		if ( ( RingPattern == PATTERN_2X )
		  || ( RingPattern == PATTERN_2XY )
		  || ( RingPattern == PATTERN_XY )
		  || ( RingPattern == PATTERN_5X )
		  || ( RingPattern == PATTERN_2 )   /* Err correction */
		  || ( RingPattern == PATTERN_Y ) )
		{
			RingPattern = PATTERN_X;
		}
		else if ( RingPattern != PATTERN_X )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn234y )
	{
		if ( ( RingPattern == PATTERN_2X )
		  || ( RingPattern == PATTERN_2XY )
		  || ( RingPattern == PATTERN_X ) )
		{
			RingPattern = PATTERN_2Y;
		}
		else if ( ( RingPattern == PATTERN_2345Y )
		       || ( RingPattern == PATTERN_5 ) )
		{
			RingPattern = PATTERN_234Y;
		}
		else if ( RingPattern == PATTERN_345 )
		{
			RingPattern = PATTERN_34;
		}
		else if ( RingPattern == PATTERN_XY )
		{
			RingPattern = PATTERN_Y;
		}
		else if ( RingPattern == PATTERN_2345 )
		{
			RingPattern = PATTERN_234;
		}
		else if ( ( RingPattern != PATTERN_2 )
		       && ( RingPattern != PATTERN_3 )
		       && ( RingPattern != PATTERN_4 )
		       && ( RingPattern != PATTERN_Y )
		       && ( RingPattern != PATTERN_34 ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn34 )
	{
		if ( ( RingPattern == PATTERN_2345Y )
		  || ( RingPattern == PATTERN_345 )
		  || ( RingPattern == PATTERN_2345 )
		  || ( RingPattern == PATTERN_2 )    /* Error Correction */
		  || ( RingPattern == PATTERN_5 )
		  || ( RingPattern == PATTERN_Y ) )
		{
			RingPattern = PATTERN_34;
		}
		else if ( ( RingPattern != PATTERN_3 )
		       && ( RingPattern != PATTERN_4 )
		       && ( RingPattern != PATTERN_34 ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else
	{
		RingPattern = PATTERN_0;
	}

	if ( RingPattern == PATTERN_0 )
	{
		TRACE_PHONE( "Warning! RingOff1End()\n" );
		TRACE_PHONE( "RING_IDLE\n" );
		RingState = RING_IDLE;
		return FALSE;
	}
	/* (PATTERN_1,) PATTERN_5, _X, should be identified */
	return TRUE;
}

/*-------------------------------------------------------------------*/

static void RingOn2End( void )
{

	/* Check "on time" for the second ring burst */
	if ( RingInformation.RingOnTime >= PRP.RingOn2Ptn0 )
	{
		RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn2Ptn24y )
	{
		if ( RingPattern == PATTERN_34 )
		{
			RingPattern = PATTERN_4;
		}
		else if ( ( RingPattern == PATTERN_234Y )
		       || ( RingPattern == PATTERN_3 ) )
		{
			RingPattern = PATTERN_24Y;
		}
		else if ( RingPattern == PATTERN_234 )
		{
			RingPattern = PATTERN_24;
		}
		else if ( ( RingPattern != PATTERN_2 )
		       && ( RingPattern != PATTERN_4 )
		       && ( RingPattern != PATTERN_Y )
		       && ( RingPattern != PATTERN_2Y ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn2Ptn23y )
	{
		if ( RingPattern == PATTERN_34 )
		{
			RingPattern = PATTERN_3;
		}
		else if ( ( RingPattern == PATTERN_234Y )
		       || ( RingPattern == PATTERN_4 ) )
		{
			RingPattern = PATTERN_23Y;
		}
		else if ( RingPattern == PATTERN_234 )
		{
			RingPattern = PATTERN_23;
		}
		else if ( ( RingPattern != PATTERN_2 )
		       && ( RingPattern != PATTERN_3 )
		       && ( RingPattern != PATTERN_Y )
		       && ( RingPattern != PATTERN_2Y ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn2Ptn3 )
	{
		if ( ( RingPattern == PATTERN_34 )
		  || ( RingPattern == PATTERN_234Y )
		  || ( RingPattern == PATTERN_234 )
		  || ( RingPattern == PATTERN_2 )    /* Error Correction */
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_Y )
		  || ( RingPattern == PATTERN_2Y ) )
		{
			RingPattern = PATTERN_3;
		}
		else if ( RingPattern != PATTERN_3 )
		{
			RingPattern = PATTERN_0;
		}
	}
	else    /* ignore this pattern */
	{
		PrematureRingEnd();
	}

#ifdef _DEBUG

	if ( RingPattern == PATTERN_0 )
		TRACE_PHONE( "Warning! RingOn2End()\n" );
#endif
	/* (PATTERN_1, _5, _X,) PATTERN_2Y, _23Y, _24Y should be identified now. */

}

/*-------------------------------------------------------------------*/

/* It might be better to disable the error correction to prevent the confusion
   of prematured ring termination. */

static void RingOff2Go( void )
{

	/* Keep checking "off time" for the first silence */
	/* This timing has little value (too late for CID) to the standard patterns
	   recognition.  However, it would help to identify non-standard ring
	   patterns, especially for PBX.
	 */
	if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn0 )
	{
		PrematureRingEnd();
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn2 )
	{
		if ( RingPattern == PATTERN_2Y )
		{
			RingPattern = PATTERN_2;
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn2y )
	{
		if ( ( RingPattern == PATTERN_24Y )
		  || ( RingPattern == PATTERN_23Y )
		  || ( RingPattern == PATTERN_3 )   /* Error Correction */
		  || ( RingPattern == PATTERN_4 ) )
		{
			RingPattern = PATTERN_2Y;
		}
		else if ( ( RingPattern == PATTERN_24 )
		       || ( RingPattern == PATTERN_23 ) )
		{
			RingPattern = PATTERN_2;
		}
		else if ( ( RingPattern != PATTERN_2 )
		       && ( RingPattern != PATTERN_Y )
		       && ( RingPattern != PATTERN_2Y ) )
		{
			RingPattern = PATTERN_0;
		}
	}
#ifdef _DEBUG
	else
	{
		TRACE_PHONE( "Design Error! RingOff2Go() Too Short!\n" );
		RingPattern = PATTERN_0;
	}

	if ( RingPattern == PATTERN_0 )
		TRACE_PHONE( "Warning! RingOff2Go()\n" );
#endif
	/* (PATTERN_1, _5, _X,) PATTERN_2, _3, _4, _Y should be identified now. */

}

/*-------------------------------------------------------------------*/

static BOOL RingOff2End( void )
{
	/* Check "off time" for the second silence */
	/* This timing has little value (too late for CID) to the standard patterns
	   recognition.  However, it would help to identify non-standard ring
	   patterns, especially for PBX.
	 */

	if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn0 )
	{
		TRACE_PHONE( "Sync. Error! RingOff2End() Too Long\n" );
		RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn2 )
	{
		if ( ( RingPattern == PATTERN_2Y )
		  || ( RingPattern == PATTERN_24Y )
		  || ( RingPattern == PATTERN_24 )
		  || ( RingPattern == PATTERN_23Y )
		  || ( RingPattern == PATTERN_23 )
		  || ( RingPattern == PATTERN_3 )   /* Error Correction */
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_Y ) )
		{
			RingPattern = PATTERN_2;
		}
		else if ( RingPattern != PATTERN_2 )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptny )
	{
		if ( ( RingPattern == PATTERN_2Y )
		  || ( RingPattern == PATTERN_24Y )
		  || ( RingPattern == PATTERN_23Y )
		  || ( RingPattern == PATTERN_2 )   /* Error Correction */
		  || ( RingPattern == PATTERN_3 )
		  || ( RingPattern == PATTERN_4 )
		  || ( RingPattern == PATTERN_24 )
		  || ( RingPattern == PATTERN_23 ) )
		{
			RingPattern = PATTERN_Y;
		}
		else if ( RingPattern != PATTERN_Y )
		{
		}
			RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn34 )
	{
		if ( ( RingPattern == PATTERN_24Y )
		  || ( RingPattern == PATTERN_24 ) )
		{
			RingPattern = PATTERN_4;
		}
		else if ( ( RingPattern == PATTERN_23Y )
		       || ( RingPattern == PATTERN_23 ) )
		{
			RingPattern = PATTERN_3;
		}
		else if ( ( RingPattern != PATTERN_3 )
		       && ( RingPattern != PATTERN_4 ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else
	{
		RingPattern = PATTERN_0;
	}

	if ( RingPattern == PATTERN_0 )
	{
		TRACE_PHONE( "Warning! RingOff2End()\n" );
		TRACE_PHONE( "RING_IDLE\n" );
		RingState = RING_IDLE;
		return FALSE;
	}

	/* (PATTERN_1, _5, _X,) PATTERN_2, _3, _4, _Y should be identified now. */
	return TRUE;
}

/*-------------------------------------------------------------------*/

static void RingOn3End( void )
{

	/* Check "on time" for the third ring burst */
	if ( RingInformation.RingOnTime >= PRP.RingOn3Ptn0 )
	{
		RingPattern = PATTERN_0;
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn3Ptn3 )
	{
		if ( RingPattern != PATTERN_3 )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn3Ptn34 )
	{
		if ( ( RingPattern != PATTERN_3 )
		  && ( RingPattern != PATTERN_4 )
		  && ( RingPattern != PATTERN_34 ) )
		{
			RingPattern = PATTERN_0;
		}
	}
	else if ( RingInformation.RingOnTime >= PRP.RingOn3Ptn4 )
	{
		if ( RingPattern != PATTERN_4 )
		{
			RingPattern = PATTERN_0;
		}
	}
	else    /* ignore this pattern */
	{
		PrematureRingEnd();
	}

#ifdef _DEBUG

	if ( RingPattern == PATTERN_0 )
		TRACE_PHONE( "Warning! RingOn3End()\n" );
#endif
	/* (PATTERN_1, _5, _X, _2, _3, _4, _Y) should be identified now. */

}

/*-------------------------------------------------------------------*/


static void StartLongSilence( void )
{
	if ( RingInformation.RingCount != 1 )
		return ;

	switch ( RingState )
	{
		case RING_OFF_1:
		case RING_OFF_1_GO:

			RingState = RING_OFF_1_CID;
			TRACE_PHONE( "RING_OFF_1_CID\n" );
			break;

		case RING_OFF_2:
		case RING_OFF_2_GO:

			RingState = RING_OFF_2_CID;
			TRACE_PHONE( "RING_OFF_2_CID\n" );
			break;

		case RING_OFF_3:
		case RING_OFF_3_GO:

			RingState = RING_OFF_3_CID;
			TRACE_PHONE( "RING_OFF_3_CID\n" );
			break;

		default:
			break;
	}

	StartType1CID();   /* Turn on Type 1 CID detection */
}

/*-------------------------------------------------------------------

    Name: Detect_Ring_CID
    The latest version of this function is operating system independent and
    hardware independent.

    Typical power ring is a 20-Hz ac signal with 6-second ringing cycle,
    which consist of 2-sec of ring patterns and 4-sec of long silence.
    In the worst case, the ring patterns' time should not exceed 3-sec,
    and the long silence should exceed 3-sec.
    The phone company might send CID signals during the first long silence
    interval of the first ringing cycle.  It starts as early as 300-ms since
    the beginning of the long silence interval, and ends at least 475-ms before
    next (cycle of) ring burst.

    The most popular distinctive ringing patterns are defined in ANSI T1.401.02,
    and summarized as following:

    Pattern 1: 2-sec ON (standard)
    Pattern 2: 800-ms ON, 400-ms OFF, 800-ms ON
    Pattern 3: 400-ms ON, 200-ms OFF, 400-ms ON, 200-ms OFF, 800-ms ON
    Pattern 4: 300-ms ON, 200-ms OFF, 1-sec ON,  200-ms OFF, 300-ms ON
    Pattern 5: 500-ms ON, one single ringing burst.

    Note: The shortest ringing burst is 150-ms.
          The longest ringing burst is 1550-ms.
          The shortest silence interval is 95-ms.
          The longest short silence interval is 700-ms! (problem for CID det.)
          This implementation tries to map a non-standard pattern to an
          ANSI one; however, not all pattens are detectable by this program.
          Such as some PBX use 1-sec ON and 3-sec OFF pattern.

    Pattern X: 1-sec ON, 3-sec OFF, (4-sec cycle)
    Pattern Y: 800-ms ON, 400-ms OFF, 800-ms ON, 2000-ms OFF, (4-sec cycle)

    Input: SamplingTime - Sampling Time in milli-second, < 100 ms.
                          (for future use)
    Output: CntlMessage(CM_RING, RingInformation.RingCount)
    Return: RingInformation.RingCount - Current Ring Count, negative number if timeout

 *-------------------------------------------------------------------*/

INT8 Detect_Ring_CID( UINT8 SamplingTime )
{
	BOOL bOK;

    if ( ModemDetectRing() )  /** ===== =====  RI is on  ===== ===== **/
	{
		if ( ( RingInformation.RingOnTime == 0 )
		  && ( RingState == RING_OFF_1_CID )
		  && ( RingState == RING_OFF_2_CID )
		  && ( RingState == RING_OFF_3_CID ) )     /* RI off to on */
		{
			TRACE_PHONE( "Timing Error Correction! RING_OFF_x_CID\n" );
			StopType1CID();    /* Turn off Type 1 CID detection */
		}

		RingInformation.RingOnTime += SamplingTime;

		if ( RingInformation.RingOnTime < PRP.RingOnDebounce )
		{
			return RingInformation.RingCount;
		}

#ifdef ADD_GLITCH_TIME_IN
		if ( ( 0 < RingInformation.RingOffTime ) && ( RingInformation.RingOffTime < PRP.RingOffDebounce ) )
		{
			TRACE_PHONE( "Warning! RI ON has OFF glitch in CID\n" );
			RingInformation.RingOnTime += RingInformation.RingOffTime;
		}
		/* Adjust missing ring on time */
		RingInformation.RingOnTime  += RingMissTime;
		RingInformation.RingOffTime -= RingMissTime;
#endif

#ifdef _DEBUG
		if ( RingInformation.RingOnTime >= PRP.RingOnMax )
			TRACE_PHONE( "Warning! RI ON too long Input Error! in CID\n" );
#endif

		switch ( RingState )
		{
			case RING_IDLE:      /* RI off to on */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOffTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOffTime >> 8 );
				CxPhoneReport( PHONE_RINGON, ReportTime );

				if ( RingInformation.RingCount <= 0 )
				{
					RingInformation.RingCount = 1;
					RingPattern = PATTERN_0;    /* Reset the state machine */
				}
				else
				{
					RingInformation.RingCount++;
				}

				/* inform CP for ring in signal, CP will display Ring... */
				RingState = RING_ON_1;  /* check RI bit off (on to off) */
				TRACE_PHONE( "RING_ON_1 in CID\n" );
				break;

			case RING_OFF_1_CID: /* RI off to on */

				StopType1CID();    /* Turn off Type 1 CID detection */

			case RING_OFF_1:     /* RI off to on */
			case RING_OFF_1_GO:  /* RI off to on */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOffTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOffTime >> 8 );
				CxPhoneReport( PHONE_RINGON, ReportTime );

				bOK = RingOff1End();
				if ( ( RingPattern == PATTERN_1 )
				  || ( RingPattern == PATTERN_5 )
				  || ( RingPattern == PATTERN_X ) )
				{
					EndOfThisRingCycle();
				}
				else if ( bOK )
				{
					RingState = RING_ON_2;
					TRACE_PHONE( "RING_ON_2 in CID\n" );
				}
				break;

			case RING_OFF_2_CID: /* RI off to on */

				StopType1CID();    /* Turn off Type 1 CID detection */

			case RING_OFF_2:     /* RI off to on */
			case RING_OFF_2_GO:  /* RI off to on */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOffTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOffTime >> 8 );
				CxPhoneReport( PHONE_RINGON, ReportTime );

#ifdef _DEBUG
				if ( ( RingPattern == PATTERN_1 )
				  || ( RingPattern == PATTERN_5 )
				  || ( RingPattern == PATTERN_X ) )
					TRACE_PHONE( "Error! RI ON in CID\n" );
#endif

				bOK = RingOff2End();

				if ( ( RingPattern == PATTERN_2 )
				  || ( RingPattern == PATTERN_Y )
				  || ( RingPattern == PATTERN_2Y ) )
				{
					EndOfThisRingCycle();
				}
				else if ( bOK )
				{
					RingState = RING_ON_3;
					TRACE_PHONE( "RING_ON_3 in CID\n" );
				}
				break;

			case RING_ON_1:      /* RI on to on */
			case RING_ON_2:      /* RI on to on */
			case RING_ON_3:      /* RI on to on */
				break;

			case RING_OFF_3_CID: /* RI off to on */
			case RING_OFF_3:     /* RI off to on */
			case RING_OFF_3_GO:  /* RI off to on */
			default:

				TRACE_PHONE( "Error! RI ON in CID\n" );
				StopType1CID();    /* Turn off Type 1 CID detection */
				RingPattern = PATTERN_0;
				RingState = RING_IDLE;
				TRACE_PHONE( "RING_IDLE in CID\n" );
				break;

		}   /* switch(RingState) */

		RingInformation.RingOffTime = 0;
		RingMissTime = 0;
	}   /** RI is on **/
	else    /** ===== ===== =====  RI is off ===== ===== ===== **/
	{

#ifdef ADD_GLITCH_TIME_IN
		if ( ( 0 < RingInformation.RingOnTime ) && ( RingInformation.RingOnTime < PRP.RingOnDebounce ) )
		{
#ifdef _DEBUG
			if ( RingMissTime == 0 )
				TRACE_PHONE( "Warning! RI OFF has ON glitch in CID\n" );
#endif
			/* With poor line termination, sometimes we see glitches
			   at the beginning of ring burst, may consider it as ON
			   in the following new ON cycle. */
			RingMissTime += SamplingTime;

			if ( ( RingInformation.RingOnTime + RingMissTime ) > PRP.RingMaxGlitch )
			{
				if ( RingState != RING_IDLE )
					RingInformation.RingOffTime += RingInformation.RingOnTime;
				RingMissTime = 0;
				RingInformation.RingOnTime = 0;
			}
		}
#endif

		if ( RingInformation.RingCount == 0 )
			return RingInformation.RingCount;

		RingInformation.RingOffTime += SamplingTime;

		if ( RingInformation.RingOffTime < PRP.RingOffDebounce )
			return RingInformation.RingCount;

		if ( ( RingInformation.RingCount < 0 ) && ( RingInformation.RingOffTime >= PRP.RingTimeoutNotify ) )
		{
			ReportRingCycleTimeout();
			RingState = RING_IDLE;
			TRACE_PHONE( "RING_IDLE in CID\n" );
			return RingInformation.RingCount;
		}
		else if ( ( RingInformation.RingCount > 0 ) && ( RingInformation.RingOffTime >= PRP.RingOffTimeout ) )
		{
			/* Check if the calling party hangup already, or
			 * the external phone goes off hook, or
			 * the built-in handset goes off hook, or
			 * the control program decides to go off hook.
			 */
			if ( RingState != RING_IDLE )
				EndOfThisRingCycle();
			RingInformation.RingCount = -RingInformation.RingCount;     /* Timeout Indication */
			return RingInformation.RingCount;
		}

		switch ( RingState )
		{
			case RING_IDLE:      /* RI off to off */
				/* it comes here after identify the long silence */
				break;

			case RING_ON_1:      /* RI on to off */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOnTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOnTime >> 8 );
				CxPhoneReport( PHONE_RINGOFF, ReportTime );

				RingOn1End();
				RingState = RING_OFF_1;
				TRACE_PHONE( "RING_OFF_1 in CID\n" );
				RingInformation.RingOnTime = 0;

				if ( RingPattern == PATTERN_END )
					EndOfThisRingCycle();
				break;

			case RING_ON_2:      /* RI on to off */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOnTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOnTime >> 8 );
				CxPhoneReport( PHONE_RINGOFF, ReportTime );

				RingOn2End();
				RingState = RING_OFF_2;
				TRACE_PHONE( "RING_OFF_2 in CID\n" );
				RingInformation.RingOnTime = 0;

				if ( RingPattern == PATTERN_END )
					EndOfThisRingCycle();
				break;

			case RING_ON_3:      /* RI on to off */

				ReportTime[ 0 ] = ( UINT8 )( RingInformation.RingOnTime & 0xff );
				ReportTime[ 1 ] = ( UINT8 )( RingInformation.RingOnTime >> 8 );
				CxPhoneReport( PHONE_RINGOFF, ReportTime );

				RingOn3End();
				RingState = RING_OFF_3;
				TRACE_PHONE( "RING_OFF_3 in CID\n" );
				RingInformation.RingOnTime = 0;

				if ( RingPattern == PATTERN_END )
					EndOfThisRingCycle();
				break;

			case RING_OFF_1:     /* RI off to off */

				if ( RingInformation.RingOffTime >= PRP.RingOffCidOn )
				{
					RingState = RING_OFF_1_GO;
					TRACE_PHONE( "RING_OFF_1_GO in CID 0x%02X\n", RingPattern );
					/* Is it better to make a guess and correct error later? */
                    /* Set to 0 doesn't gain extra CID at all */

					if ( ( RingPattern == PATTERN_1 )
					  || ( RingPattern == PATTERN_5 )
					  || ( RingPattern == PATTERN_X )
					  || ( RingPattern == PATTERN_END )
                      || ( RingPattern == PATTERN_2X )
                      || ( RingPattern == PATTERN_345) )
					{
						StartLongSilence();
					}
				}
				break;

			case RING_OFF_1_GO:  /* RI off to off */

				if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn15x )   /* Long Silence */
				{
					RingOff1Go();

					if ( ( RingPattern == PATTERN_1 )
					  || ( RingPattern == PATTERN_5 )
					  || ( RingPattern == PATTERN_X )
					  || ( RingPattern == PATTERN_END ) )
					{
						EndOfThisRingCycle();
					}
				}
				break;

			case RING_OFF_2:     /* RI off to off */

				if ( RingInformation.RingOffTime >= PRP.RingOffCidOn )
				{
					TRACE_PHONE( "RING_OFF_2_GO in CID\n" );

					RingState = RING_OFF_2_GO;

					/* Is it better to make a guess and correct error later? */

					if ( ( RingPattern == PATTERN_2 )
					  || ( RingPattern == PATTERN_Y )
					  || ( RingPattern == PATTERN_2Y )
					  || ( RingPattern == PATTERN_END )
					  || ( RingPattern == PATTERN_234Y )
					  || ( RingPattern == PATTERN_234 )
					  || ( RingPattern == PATTERN_24Y )
					  || ( RingPattern == PATTERN_24 )
					  || ( RingPattern == PATTERN_23Y )
					  || ( RingPattern == PATTERN_23 )
					  || ( RingPattern == PATTERN_0 ) )
					{
						StartLongSilence();
					}
				}
				break;

			case RING_OFF_2_GO:  /* RI off to off */

				if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn2y )    /* Long Silence */
				{
					RingOff2Go();

					if ( ( RingPattern == PATTERN_2 )
					  || ( RingPattern == PATTERN_Y )
					  || ( RingPattern == PATTERN_END ) )
					{
						EndOfThisRingCycle();
					}
				}
				break;

			case RING_OFF_3:     /* RI off to off */     /* Long Silence */

				if ( RingInformation.RingOffTime >= PRP.RingOffCidOn )
				{
					TRACE_PHONE( "RING_OFF_3_GO in CID\n" );

					RingState = RING_OFF_3_GO;
					StartLongSilence();
				}
				break;

			case RING_OFF_3_GO:  /* RI off to off */     /* Long Silence */

				EndOfThisRingCycle();
				break;

			case RING_OFF_1_CID: /* RI off to off */

				if ( ( RingInformation.RingOffTime >= PRP.RingOffCidOff125 )   /* 15, 0 */
				  || ( ( RingInformation.RingOffTime >= PRP.RingOffCidOffx )   /* X */
				    && ( RingPattern == PATTERN_X ) ) )
				{
					StopType1CID();    /* Turn off Type 1 CID detection */
				}
				else if ( RingInformation.RingOffTime >= PRP.RingOff1Ptn15x )  /* Long Silence */
				{
					RingOff1Go();
				}
				break;

			case RING_OFF_2_CID: /* RI off to off */

				if ( ( RingInformation.RingOffTime >= PRP.RingOffCidOff125 )   /* 2, 2Y, 0 */
				  || ( ( RingInformation.RingOffTime >= PRP.RingOffCidOffy )   /* Y */
				    && ( RingPattern == PATTERN_Y ) ) )
				{
					StopType1CID();    /* Turn off Type 1 CID detection */
				}
				else if ( RingInformation.RingOffTime >= PRP.RingOff2Ptn2y )   /* Long Silence */
				{
					RingOff2Go();
				}
				break;

			case RING_OFF_3_CID: /* RI off to off */

				if ( RingInformation.RingOffTime >= PRP.RingOffCidOff34 )      /* 34, 0 */
				{
					StopType1CID();    /* Turn off Type 1 CID detection */
				}
				break;

			default:

				TRACE_PHONE( "Error! RI OFF: in CID\n" );
				TRACE_PHONE( "RING_IDLE in CID\n" );

				RingPattern = PATTERN_0;
				RingState = RING_IDLE;
				break;

		}   /* switch(RingState) */
	}   /** RI is off **/
	return RingInformation.RingCount;
}


/*********************************************************************/

/** Public functions used by other modules **/

/*-------------------------------------------------------------------

    Name: Init_RingState
    This function initialize Ring and CID detection state machine.

    Input:  None
    Output: None
    Return: None

 *-------------------------------------------------------------------*/

void Init_RingState( BOOL bConf )
{
	TRACE_PHONE( "Init_RingState() RingCount %d -> 0\n", RingInformation.RingCount );

	RingState = RING_IDLE;
	RingInformation.RingCount = 0;
	RingInformation.RingOnTime = 0;
	RingMissTime = 0;
	if ( bConf )
		ModemRxConfigRing();
}


