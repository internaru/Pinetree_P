/*
 *  $RCSfile: dialer.c,v $
 * $Revision: 1.9 $
 *   $Author: yooh1 $
 *     $Date: 2010/11/22 19:07:25 $
 */
/************************************************************************
 *
 *      dialer.c
 *
 *  Dialer functions.
 *
 *  Copyright 2009 Conexant Systems, Inc.
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

/*============================================================================
*
*    phone_auto_dial
*
*   Uses the modem to transmit digits
*
*   Input:   pointer of dial buffer, 0-9, $A-$D, $E = *, $F = #
*            type   0 = DTMF, 1 = pulse
*
*   Output:     Modem configured to dialing mode
*               DTMF or pulse digits transmitted
*
*============================================================================*/
void phone_auto_dial( UINT8 *digits_buffer, UINT8 type )
{
	UINT8 on_time, off_time, i;
	UINT8 lband_level, hband_level;

	if ( *digits_buffer == 0xff )
	{
		return ;
	}
	else if ( *digits_buffer == PHONE_DIAL_PLUSE_CHAR )
	{
		type = 1;
		digits_buffer++;
	}
	else if ( *digits_buffer == PHONE_DIAL_DTMF_CHAR )
	{
		type = 0;
		digits_buffer++;
	}

	if ( type )
	{
		on_time  = PDP.PulseMakeTime  + PDP.PulseMakeOffset;
		off_time = PDP.PulseBreakTime + PDP.PulseBreakOffset;

		if ( PDP.PulseSpeed == 20 )
		{
			on_time  /= 2;
			off_time /= 2;
		}

		TRACE_PHONE( "phone_auto_dial: make %dms, break %dms\n", on_time, off_time );

		i = ModemTxPulseNumbers( digits_buffer, PDP.PulseMode, on_time, off_time, PDP.PulseInterDelay );

		if ( i )
		{
			digits_buffer += i;
			type = 0;
		}
	} 

	if ( !type )
	{
		on_time     = PDP.DTMFOnTime;
		off_time    = PDP.DTMFOffTime;
		lband_level = PDP.DTMFLowLevel;
		hband_level = PDP.DTMFHighLevel;

		ModemControl->Options.DTMFCompensation = PDP.DTMFCompensation;

		TRACE_PHONE( "phone_auto_dial: on %dms, off %dms\n", on_time, off_time );
		TRACE_PHONE( "low band level %d, high band level %d\n", lband_level, hband_level );

		ModemTxDTMFNumbers( digits_buffer, on_time, off_time, lband_level, hband_level );
	}
}


/*============================================================================
*
*    phone_dial_digit
*
*   Uses the modem to transmit a digit
*
*   Input:  digit  0-9, $A-$D, $E = *, $F = #
*           type   0 = DTMF, 1 = pulse, 2 = DTMF continuously
*
*   Output:     Modem configured to dialing mode
*               DTMF or pulse digit transmitted
*
*============================================================================*/
void phone_dial_digit( UINT8 digit, UINT8 type )
{
	UINT8 on_time, off_time;
	UINT8 lband_level, hband_level;

	if ( type == 1 )
	{
		on_time  = PDP.PulseMakeTime  + PDP.PulseMakeOffset;
		off_time = PDP.PulseBreakTime + PDP.PulseBreakOffset;

		if ( PDP.PulseSpeed == 20 )
		{
			on_time  /= 2;
			off_time /= 2;
		}

		TRACE_PHONE( "phone_auto_dial: make %dms, break %dms\n", on_time, off_time );

		ModemTxPulseDigit( digit, PDP.PulseMode, on_time, off_time );
	} 
	else
	{
		if ( type == 2 )
			on_time = 0;
		else
			on_time = PDP.DTMFOnTime;

		off_time    = PDP.DTMFOffTime;
		lband_level = PDP.DTMFLowLevel;
		hband_level = PDP.DTMFHighLevel;

		ModemControl->Options.DTMFCompensation = PDP.DTMFCompensation;

		TRACE_PHONE( "phone_auto_dial: on %dms, off %dms\n", on_time, off_time );
		TRACE_PHONE( "low band level %d, high band level %d\n", lband_level, hband_level );

		ModemTxDTMFDigit( digit, on_time, off_time, lband_level, hband_level );
	}
}

