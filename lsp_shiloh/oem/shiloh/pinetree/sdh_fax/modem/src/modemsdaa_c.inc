/*
 *  $RCSfile: modemsdaa_c.inc,v $
 * $Revision: 1.28 $
 *   $Author: miurat $
 *     $Date: 2012/03/21 00:27:23 $
 */
/************************************************************************
 *
 *      modemsdaa_c.inc
 *
 *  SSF336 SDAA driver.
 *
 *  Copyright 2007 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "modemsdaa_h.inc"

#define TR_VOLTAGE_SAMPLE_TIME  10  /* 100ms */
#define LINE_IN_USE_VALID_TIME  8   /* 800ms */
#define AFTER_ONHOOK_DELAY_TIME 15  /* 1500ms */
#define MAXVOLTAGE_UPDATE_TIME  20  /* 2000ms */

extern OSC_MUTEX modem_dsp_mutex;

extern UINT8 MinDetectableTipRingVolt;
extern UINT8 gFaxLineInUseThreshold;
static UINT16 CoVoltage, MaxTRVoltage, LineInUseVoltage;
static UINT8  TRSampleCount, TRUpdateCount, LineInUseValidCount;

static LINE_STATUS LineStatus;

////////////////////////////////////////////////////////////////////////
// internal functions
static BOOL SSFSetAPICommand( UINT8 cmd, UINT8 parm1, UINT8 parm2 );
static BOOL SSFGetAPIStatus( UINT32 code );
static void SSFAPIInitialize( BOOL offhook );
static void SSFCoVoltage( void );
void SSFInitMonitorParams( BOOL clear );

////////////////////////////////////////////////////////////////////////
static SDAADATA SDAA_Data_Table;

typedef enum
{
	NO_CONTROL     = 0,
	ENABLE_CONTROL = 1,
	STOP_MONITOR   = 2,
	START_MONITOR  = 3
} _monitorAPI_state_t;

static _monitorAPI_state_t _MonitorAPIState = NO_CONTROL;

static const SDAA_COUNTRY_INFO H_country =
{
	Z540OHM,                       /* Line Impedance */
	SDAA_DC2,                      /* DC Curve */
	SDAA_MAKE_100,                 /* Make Resistance */
	SDAA_PULSE_FASTEDGE,           /* Pulse edge */
	0x40,                          /* Initial DAC value */
	0,                             /* Ring Max Frequency */
	0,                             /* Ring Min Frequency */
	16,                            /* Ring Threshold */
	0,                             /* South Korea Ring Impedance */
	0,                             /* Brazil Ring Detection */
	0,                             /* Line In Use Threshold */
	0,                             /* Extension Threshold */
	0,                             /* Digital Threshold */
	0                              /* DIB power adjust */
};

static const UINT8 H_DCLoopVoltage[ SDAA_DC_POINTS ] =
{
	8, 13, 18, 28, 32,             /* DC0 default */
};

static const UINT8 H_DCLoopCurrent[ SDAA_DC_POINTS ] =
{
	0, 15, 40, 110, 150,           /* DC0 default */
};

static const struct
{
	UINT8 ring_volt;
	UINT8 modem_param;
} ring_voltage_lookup[] =
{
	{ 11, 0xF0 },
	{ 12, 0xEE },
	{ 13, 0xEC },
	{ 14, 0xEB },
	{ 15, 0xE9 },
	{ 16, 0xE8 },
	{ 17, 0xE6 },
	{ 18, 0xE4 },
	{ 19, 0xE3 },
	{ 20, 0xE1 },
	{ 21, 0xDF },
	{ 22, 0xDE },
	{ 23, 0xDC },
	{ 24, 0xDA },
	{ 25, 0xD9 },
	{ 26, 0xD7 },
	{ 27, 0xD6 },
	{ 28, 0xD4 },
	{ 29, 0xD3 },
	{ 30, 0xD1 },
	{ 31, 0xD0 },
	{ 32, 0xCE },
	{ 33, 0xCC },
	{ 34, 0xCA },
	{ 35, 0xC8 },
	{  0,    0 }
};


void SSFOffHook( void )
{
	UINT16 i;

	TRACE_SDAA( "SDAA OffHook\n" );

	SSFAPIInitialize( TRUE );

	SSFSetAPICommand( API_CMD_OFFHOOKMONITOR, API_OFFHOOKMONITOR_ON, 0 );

	if ( SSFSetAPICommand( API_CMD_HOOKSTATE, API_HOOKPARM_OFFHOOK, 0 ) )
	{
		for ( i = 0 ; SSFGetAPIStatus( API_STA_HOOKDONE ) == FALSE && i < 100 ; i++ )
		{
			OSC_Sleep( MODEM_DELAY_TIME );
		}

		TRACE_SDAA( " Done, %dms\n", i * MODEM_DELAY_TIME );
	}
	else
	{
		TRACE_SDAA( " Command Error!!\n" );
	}

	LineStatus = LINE_OFFHOOK;
}


void SSFOnHook( void )
{
	UINT16 i;

	TRACE_SDAA( "SDAA OnHook\n" );

	if ( SSFSetAPICommand( API_CMD_HOOKSTATE, API_HOOKPARM_ONHOOK_STATUS, 0 ) )
	{
		for ( i = 0 ; SSFGetAPIStatus( API_STA_HOOKDONE ) == FALSE && i < 200 ; i++ )
		{
			OSC_Sleep( MODEM_DELAY_TIME );
		}

		TRACE_SDAA( " Done %dms\n", i * MODEM_DELAY_TIME );
	}
	else
	{
		TRACE_SDAA( " Command Error!!\n" );
	}

	SSFAPIInitialize( FALSE );
	SSFInitMonitorParams( TRUE );
}


void SSFPulseDial( UINT8 digit, UINT8 pulse_mode, UINT8 make_time, UINT8 break_time )
{
	switch ( pulse_mode )
	{
		case 1:

			digit = 10 - digit;
			break;

		case 2:

			digit = digit + 1;
			break;

		case 0:
		default:

			if ( digit == 0 )
			{
				digit = 10;
			}

			break;
	}
	if ( make_time == 0 )
	{
		make_time = 0x24;
	}
	else
	{
		make_time = ( make_time * 100 ) / 106;
	}

	if ( break_time == 0 )
	{
		break_time = 0x38;
	}
	else
	{
		break_time = ( break_time * 100 ) / 106;
	}

	TRACE_SDAA( "SDAA PulseDial digit = %d\n", digit );
	TRACE_SDAA( "          value make = 0x%02X, break = 0x%02X\n", make_time, break_time );

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	ModemDSPWrite1( API_MakeTime, make_time, FALSE );

	ModemDSPWrite1( API_BreakTime, break_time, FALSE );

	ModemDSPWrite1( API_PulseInterDigit, ( UINT8 ) 1, FALSE );      /* set minimum */

	OSC_Release_Mutex( &modem_dsp_mutex );

	if ( SSFSetAPICommand( API_CMD_PULSEDIALDIGIT, digit, 0 ) )
	{
		UINT16 i;

		for ( i = 0 ; SSFGetAPIStatus( API_STA_PULSEDONE ) == FALSE && i < 2000 ; i++ )
		{
			OSC_Sleep( MODEM_DELAY_TIME );
		}

		TRACE_SDAA( " Digit Done, %dms\n", i * MODEM_DELAY_TIME );

		if ( SSFSetAPICommand( API_CMD_HOOKSTATE, API_HOOKPARM_OFFHOOK, 0 ) )
		{
			TRACE_SDAA( " Set OffHook state\n" );
		}
		else
		{
			TRACE_SDAA( " OffHook Command Error!!\n" );
		}
	}
}


void SSFPulseSetup( void )
{
	UINT8 setuptime = 0, cleartime = 0;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	if ( setuptime > 1 )
		setuptime = ( UINT8 ) ( ( ( UINT16 ) setuptime * 100 ) / 106 );

	ModemDSPWrite1( API_PulseSetup, setuptime, FALSE );

	if ( cleartime > 1 )
		cleartime = ( UINT8 ) ( ( ( UINT16 ) cleartime * 100 ) / 106 );

	ModemDSPWrite1( API_PulseClear, cleartime, FALSE );

	OSC_Release_Mutex( &modem_dsp_mutex );

	TRACE_SDAA( "SDAA PulseSetup setup %dms, clear %dms\n", setuptime, cleartime );
}


void SSFPulseCleanup( void )
{
	TRACE_SDAA( "SDAA PulseCleanup\n" );
}


void SSFInitMonitorParams( BOOL clear )
{
	if ( clear )
	{
		CoVoltage = 0;
		MaxTRVoltage = 0;
		LineInUseVoltage = 0;
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
	}
	else
	{
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME * 3;
	}

	TRSampleCount = AFTER_ONHOOK_DELAY_TIME;
	LineInUseValidCount = LINE_IN_USE_VALID_TIME;

	LineStatus = LINE_ONHOOK;
}


void SSFInitParams( CtryPrmsStruct *Ctry )
{
	UINT16 i;

	SSFInitMonitorParams( TRUE );

	if ( Ctry != 0 )
	{
		if ( Ctry->T35Code != 0xff )
		{
			SDAA_Data_Table.T35Code = Ctry->T35Code;
			memcpy( &SDAA_Data_Table.m_country, &Ctry->SDAA, sizeof( SDAA_COUNTRY_INFO ) );

                        if (gFaxLineInUseThreshold != 0)
                        {
                                SDAA_Data_Table.m_country.LineInUseThreshold = gFaxLineInUseThreshold;
                        }
		}
	}
	else
	{
		/* set default parameters */
		SDAA_Data_Table.T35Code   = 0xff;
		SDAA_Data_Table.m_country = H_country;

		for ( i = 0; i < SDAA_DC_POINTS; i++ )
		{
			SDAA_Data_Table.m_DC0CurrentPoint[ i ] = H_DCLoopCurrent[ i ];
			SDAA_Data_Table.m_DC0VoltagePoint[ i ] = H_DCLoopVoltage[ i ];
		}
	}

	if ( SDAA_Data_Table.RefLineInUseVoltage == 0 )
	{
		if ( ( SDAA_Data_Table.RefTRVoltage != 0 )
                  && ( SDAA_Data_Table.RefTRVoltage < ( MinDetectableTipRingVolt + 2 ) ) )
		{
			SDAA_Data_Table.RefLineInUseVoltage = ( SDAA_Data_Table.RefTRVoltage + 4 ) / 2;
		}
		else
		{
                        SDAA_Data_Table.RefLineInUseVoltage = MinDetectableTipRingVolt;
		}
	}
	else if ( SDAA_Data_Table.RefTRVoltage != 0 )
	{
		if ( SDAA_Data_Table.RefLineInUseVoltage > ( SDAA_Data_Table.RefTRVoltage - 2 ) )
		{
			SDAA_Data_Table.RefLineInUseVoltage = ( SDAA_Data_Table.RefTRVoltage - 2 );
		}
	}

	TRACE_SDAA( "SDAA Init for T.35(%02X)\n", SDAA_Data_Table.T35Code );
}


BOOL SSFSpeakerVolume( UINT8 Volume )
{
	if ( Volume & 7 )
	{
		if ( SSFSetAPICommand( API_CMD_VOLUME, Volume, 0 ) )
		{
			if ( SSFSetAPICommand( API_CMD_SPEAKERSOURCE, 0x11, 0 ) )
			{
				TRACE_SDAA( "SDAA SetSpeakerVolume Set %d\n", Volume );
				return TRUE;
			}
			else
			{
				TRACE_SDAA( "SDAA SetSpeakerVolume Command Error!!\n" );
				return FALSE;
			}
		}
		else
		{
			TRACE_SDAA( "SDAA SetSpeakerVolume Command Error!!\n" );
			return FALSE;
		}
	}
	else
	{
		if ( SSFSetAPICommand( API_CMD_VOLUME, 0, 0 ) )
		{
			if ( SSFSetAPICommand( API_CMD_SPEAKERSOURCE, 0x00, 0 ) )
			{
				TRACE_SDAA( "SDAA SetSpeakerVolume Set OFF\n" );
				return TRUE;
			}
			else
			{
				TRACE_SDAA( "SDAA SetSpeakerVolume(OFF) Command Error!!\n" );
				return FALSE;
			}
		}
		else
		{
			TRACE_SDAA( "SDAA SetSpeakerVolume(OFF) Command Error!!\n" );
			return FALSE;
		}
	}
}


UINT8 SSFReadSSD( UINT8 cAddr )
{
	UINT8 cData;

	if ( SSFSetAPICommand( API_CMD_READSSDLSD, cAddr, 0 ) )
	{
		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		cData = ModemDSPRead1( API_TaskParmB );

		OSC_Release_Mutex( &modem_dsp_mutex );

#ifdef DBG_SDAA_PRINTF
		TRACE_SDAADEBUG( "SDAA ReadSSD Addr:0x%02X Data:0x%02X\n", cAddr, cData );
#endif
		return cData;
	}
	else
	{
		TRACE_SDAA( "SDAA ReadSSD Error!! Addr:0x%02X\n", cAddr );
		return 0xff;
	}
}


void SSFWriteSSD( UINT8 cAddr, UINT8 cData )
{
	if ( SSFSetAPICommand( API_CMD_WRITESSDLSD, cAddr, cData ) )
	{
#ifdef DBG_SDAA_PRINTF
		TRACE_SDAADEBUG( "SDAA WriteSSD Addr:0x%02X Data:0x%02X\n", cAddr, cData );
#endif
	}
	else
	{
		TRACE_SDAA( "SDAA WriteSSD Error!! Addr:0x%02X Data:0x%02X\n", cAddr, cData );
	}
}


UINT8 SSFReadLSD( UINT8 cAddr )
{
	UINT8 cData;

	if ( SSFSetAPICommand( API_CMD_READSSDLSD, cAddr | 0x80, 0 ) )
	{
		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		cData = ModemDSPRead1( API_TaskParmB );

		OSC_Release_Mutex( &modem_dsp_mutex );

#ifdef DBG_SDAA_PRINTF
		TRACE_SDAADEBUG( "SDAA ReadLSD Addr:0x%02X Data:0x%02X\n", cAddr, cData );
#endif
		return cData;
	}
	else
	{
		TRACE_SDAA( "SDAA ReadLSD Error!! Addr:0x%02X\n", cAddr );
		return 0xff;
	}
}


void SSFWriteLSD( UINT8 cAddr, UINT8 cData )
{
	if ( SSFSetAPICommand( API_CMD_WRITESSDLSD, cAddr | 0x80, cData ) )
	{
#ifdef DBG_SDAA_PRINTF
		TRACE_SDAADEBUG( "SDAA WriteLSD Addr:0x%02X Data:0x%02X\n", cAddr, cData );
#endif
	}
	else
	{
		TRACE_SDAA( "SDAA WriteLSD Error!! Addr:0x%02X Data:0x%02X\n", cAddr, cData );
	}
}


static BOOL SSFSetAPICommand( UINT8 cmd, UINT8 parm1, UINT8 parm2 )
{
	UINT16 i = 0;
	UINT8 sdbs, tsk = 0;
	BOOL   ret = TRUE;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	while ( ( ( sdbs = MDM_BITSET( MODEM_0C, SDBS ) ) != 0 ) ||
	        ( ( tsk = ModemDSPRead1( API_TaskCommand ) ) != 0 ) )
	{
		if ( i++ < 1000 )
			OSC_Sleep( 1 );
		else
		{
			TRACE_SDAA( "SDAA SetAPICommand SDBS 0x%02X or TaskCommand 0x%02X NOT Ready!!\n", sdbs, tsk );
			ret = FALSE;
			break;
		}
	}

	if ( ret )
	{
		ModemDSPWrite1( API_TaskParmA, parm1, FALSE );
		ModemDSPWrite1( API_TaskParmB, parm2, FALSE );
		ModemDSPWrite1( API_TaskCommand, cmd, FALSE );

		i = 0;

		do
		{
			if ( i++ < 2000 )
			{
				OSC_Sleep( 1 );
			}
			else
			{
				TRACE_SDAA( "SDAA SetAPICommand SDBS 0x%02X or TaskCommand 0x%02X NOT Finished!!\n", sdbs, tsk );
				ret = FALSE;
				break;
			}
		} while ( ( ( sdbs = MDM_BITSET( MODEM_0C, SDBS ) ) != 0 ) ||
		          ( ( tsk = ModemDSPRead1( API_TaskCommand ) ) != 0 ) );
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

	TRACE_SDAADEBUG( "SDAA SetAPICommand %02X Process time %dms\n", cmd, i * 10 );

	return ret;
}


static BOOL SSFGetAPIStatus( UINT32 code )
{
	UINT8 data;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	data = ModemDSPRead1( ( UINT16 ) code );

	switch ( code )
	{
		case API_STA_LINEINUSE:
		case API_STA_NOLINE:
		case API_STA_EXTENSION:
			break;

		default:

			if ( data & ( UINT8 ) ( code >> 24 ) )
				ModemDSPWrite1( ( UINT16 ) code, data & ~( UINT8 ) ( code >> 24 ), FALSE );

			break;
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

	return ( data & ( UINT8 ) ( code >> 24 ) ) == ( UINT8 ) ( code >> 24 );
}


static void SSFAPIInitialize( BOOL offhook )
{
	UINT8  data, i;
	UINT16 wdata;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	/* Set DIB power */

	if ( SDAA_Data_Table.m_country.DIBPower != 0 )
	{
		data = ModemDSPRead1( 0x4C2 );
		data &= 3;
		data |= SDAA_Data_Table.m_country.DIBPower & ~3;
		ModemDSPWrite1( 0x4C2, data, FALSE );
	}

	/* Set DC mask */

	if ( SDAA_Data_Table.m_country.DCCurve != SDAA_DC2 )
	{
		ModemDSPWrite1( API_DCCurve, SDAA_Data_Table.m_country.DCCurve, FALSE );
	}

	if ( SDAA_Data_Table.m_country.DCCurve == SDAA_DC0 )
	{
		ModemDSPWrite1( API_DC0VoltagePoint0, SDAA_Data_Table.m_DC0VoltagePoint[ 0 ], FALSE );
		ModemDSPWrite1( API_DC0VoltagePoint1, SDAA_Data_Table.m_DC0VoltagePoint[ 1 ], FALSE );
		ModemDSPWrite1( API_DC0VoltagePoint2, SDAA_Data_Table.m_DC0VoltagePoint[ 2 ], FALSE );
		ModemDSPWrite1( API_DC0VoltagePoint3, SDAA_Data_Table.m_DC0VoltagePoint[ 3 ], FALSE );
		ModemDSPWrite1( API_DC0VoltagePoint4, SDAA_Data_Table.m_DC0VoltagePoint[ 4 ], FALSE );

		ModemDSPWrite1( API_DC0CurrentPoint0, SDAA_Data_Table.m_DC0CurrentPoint[ 0 ], FALSE );
		ModemDSPWrite1( API_DC0CurrentPoint1, SDAA_Data_Table.m_DC0CurrentPoint[ 1 ], FALSE );
		ModemDSPWrite1( API_DC0CurrentPoint2, SDAA_Data_Table.m_DC0CurrentPoint[ 2 ], FALSE );
		ModemDSPWrite1( API_DC0CurrentPoint3, SDAA_Data_Table.m_DC0CurrentPoint[ 3 ], FALSE );
		ModemDSPWrite1( API_DC0CurrentPoint4, SDAA_Data_Table.m_DC0CurrentPoint[ 4 ], FALSE );
	}

	/* Set line impedance */
	if ( SDAA_Data_Table.m_country.LineImpedance != Z540OHM )
	{
		ModemDSPWrite1( API_Complex, SDAA_Data_Table.m_country.LineImpedance, FALSE );
	}

	/* Set initial DAC value */
	if ( SDAA_Data_Table.m_country.InitialDACvalue != 0 )
	{
		ModemDSPWrite1( API_InitialDACforDC0 + SDAA_Data_Table.m_country.DCCurve, SDAA_Data_Table.m_country.InitialDACvalue, FALSE );
	}

	if ( offhook )
	{
		/* Set pulse make resistance */
		if ( SDAA_Data_Table.m_country.RMake != SDAA_MAKE_100 )
		{
			ModemDSPWrite1( API_RMake, SDAA_Data_Table.m_country.RMake, FALSE );
		}

		/* Set pulse edge */
		if ( SDAA_Data_Table.m_country.PulseEdge != SDAA_PULSE_FASTEDGE )
		{
			data = ModemDSPRead1( ( UINT16 ) API_SEL_FASTEDGEPULSEDIAL );
			ModemDSPWrite1( ( UINT16 ) API_SEL_FASTEDGEPULSEDIAL, data & ~0x01, FALSE );

			if ( SDAA_Data_Table.T35Code == 0x09 )
				ModemDSPWrite1( 0x732, 0x01, FALSE );
			else
				ModemDSPWrite1( 0x732, 0x02, FALSE );
		}
		else
		{
			data = ModemDSPRead1( ( UINT16 ) API_SEL_FASTEDGEPULSEDIAL );
			ModemDSPWrite1( ( UINT16 ) API_SEL_FASTEDGEPULSEDIAL, data | 0x01, FALSE );
			ModemDSPWrite1( 0x732, 0x00, FALSE );
		}

		/* Clear low ring impedance */
		data = ModemDSPRead1( ( UINT16 ) API_SEL_KOREARINGIMPEDANCE );
		ModemDSPWrite1( ( UINT16 ) API_SEL_KOREARINGIMPEDANCE, data & ~0x10, FALSE );

		/* Set Brazil ring */
		if ( SDAA_Data_Table.m_country.BrazilRingDetection != 0 )
		{
			ModemDSPWrite1( ( UINT16 ) API_SEL_BRAZILRINGENABLE, 0x20, TRUE );
		}

		/* Set digital threshold */
		if ( SDAA_Data_Table.m_country.DigitalThreshold != 0 )
		{
			wdata = ( UINT16 )SDAA_Data_Table.m_country.DigitalThreshold;
			ModemDSPWrite2( API_DigitalThreshold_H, API_DigitalThreshold_L, wdata );
		}
	}
	else
	{
		/* Set ring threshold */
		if ( SDAA_Data_Table.m_country.RingThreshold != 0 )
		{
			/* look up the settings in the table */
			for ( i = 0; ring_voltage_lookup[i].ring_volt != 0; i++ )
			{
				if ( SDAA_Data_Table.m_country.RingThreshold == ring_voltage_lookup[i].ring_volt )
				{
					ModemDSPWrite1( API_RingThreshold, ring_voltage_lookup[i].modem_param, FALSE );
					break;
				}
			}
		}

		/* Set low ring impedance */
		if ( SDAA_Data_Table.m_country.SouthKoreaRingImpedance != 0 )
		{
			ModemDSPWrite1( ( UINT16 ) API_SEL_KOREARINGIMPEDANCE, 0x10, TRUE );
		}

		/* Set line in use threshold */
		if ( SDAA_Data_Table.m_country.LineInUseThreshold != 0 )
		{
			wdata = SDAA_Data_Table.m_country.LineInUseThreshold * 100;
		}
		else if ( SDAA_Data_Table.RefLineInUseDelta != 0 )
		{
			wdata = SDAA_Data_Table.RefLineInUseDelta * 100;
		}
		else if ( MaxTRVoltage > ( SDAA_Data_Table.RefLineInUseVoltage * 2 ) )
		{
			wdata = ( MaxTRVoltage - SDAA_Data_Table.RefLineInUseVoltage ) * 100;
		}
		else
		{
			wdata = ( MaxTRVoltage / 2 ) * 100;
		}

		if ( wdata > 500 )
			ModemDSPWrite2( API_LineInUseThreshold_H, API_LineInUseThreshold_L, wdata );

		/* Set extension threshold */
		if ( SDAA_Data_Table.m_country.ExtensionThreshold != 0 )
		{
			wdata = SDAA_Data_Table.m_country.ExtensionThreshold * 100;
			ModemDSPWrite2( API_ExtensionThreshold_H, API_ExtensionThreshold_L, wdata );
		}

		if ( SDAA_Data_Table.m_country.RingMaxFrequency != 0 )
		{
			wdata = SDAA_Data_Table.m_country.RingMaxFrequency * 106;
			data = ( UINT8 )( 100000 / wdata );
			ModemDSPWrite1( API_RingMax,  data, FALSE );
		}

		if ( SDAA_Data_Table.m_country.RingMinFrequency != 0 )
		{
			wdata = SDAA_Data_Table.m_country.RingMinFrequency * 106;
			data = ( UINT8 )( 100000 / wdata );
			ModemDSPWrite1( API_RingMin, data, FALSE );
		}

		OSC_Release_Mutex( &modem_dsp_mutex );

		SSFSetAPICommand( API_CMD_INITRINGTHRESHOLD, 0, 0 );

		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );
	}

	TRACE_SDAA( "SDAA API params\n" );
	TRACE_SDAA( " T.35 code    = 0x%02X\n", SDAA_Data_Table.T35Code );

	data = ModemDSPRead1( ( UINT16 ) API_DCCurve );
	TRACE_SDAA( " DCCurve      = %d\n", data );
	TRACE_SDAA( " InitDAC      = 0x%X\n",
	            ModemDSPRead1( ( UINT16 ) API_InitialDACforDC0 + data ) );

	data = ModemDSPRead1( ( UINT16 ) API_Complex );

	switch ( data )
	{
		case 0:
			TRACE_SDAA( " Impedance    = 600 ohm\n" );
			break;
		case 1:
			TRACE_SDAA( " Impedance    = complex\n" );
			break;
		case 2:
			TRACE_SDAA( " Impedance    = 540 ohm\n" );
			break;
		default:
			TRACE_SDAA( " Impedance    = invalid %d\n", data );
			break;
	}

	TRACE_SDAA( " RMake        = %d ohm\n", ModemDSPRead1( ( UINT16 ) API_RMake ) * 100 );
	TRACE_SDAA( " RingMaxFreq  = %d Hz\n", 100000 / ( ModemDSPRead1( API_RingMax ) * 106 ) );
	TRACE_SDAA( " RingMinFreq  = %d Hz\n", 100000 / ( ModemDSPRead1( API_RingMin ) * 106 ) );

	data = ModemDSPRead1( ( UINT16 ) API_RingThreshold );
	/* look up the settings in the table */
	for ( i = 0; ring_voltage_lookup[i].modem_param != 0; i++ )
	{
		if ( ring_voltage_lookup[i].modem_param == data )
		{
			TRACE_SDAA( " RingThreshold= %d V\n", ring_voltage_lookup[i].ring_volt );
			break;
		}
	}

	data = ModemDSPRead1( ( UINT16 ) API_SEL_FASTEDGEPULSEDIAL );

	TRACE_SDAA( " FastEdgePulse= %s, %d\n", ( data & 0x01 ) ? "ON" : "OFF", ModemDSPRead1( 0x732 ) );
	TRACE_SDAA( " LowRingImpeda= %s\n", ( data & 0x10 ) ? "ON" : "OFF" );
	TRACE_SDAA( " BrazilRing   = %s\n", ( data & 0x20 ) ? "ON" : "OFF" );
	TRACE_SDAA( " LineInUseThld= %d V\n",
	              ModemDSPRead2( API_LineInUseThreshold_H, API_LineInUseThreshold_L ) / 100 );
	TRACE_SDAA( " ExtensionThld= %d V\n",
	              ModemDSPRead2( API_ExtensionThreshold_H, API_ExtensionThreshold_L ) / 100 );
	TRACE_SDAA( " DigitalThld  = %d mA\n",
	              ModemDSPRead2( API_DigitalThreshold_H, API_DigitalThreshold_L ) );

	TRACE_SDAA( " DIBpower     = 0x%X\n", ModemDSPRead1( 0x4C2 ) );

	OSC_Release_Mutex( &modem_dsp_mutex );
}


void SSFStopOffHookMonitor( void )
{
	SSFSetAPICommand( API_CMD_OFFHOOKMONITOR, API_OFFHOOKMONITOR_OFF, 0 );
}


void SSFStartOffHookMonitor( void )
{
	SSFSetAPICommand( API_CMD_OFFHOOKMONITOR, API_OFFHOOKMONITOR_ON, 0 );
}


void SSFStopMonitorAPI( void )
{
	if ( _MonitorAPIState != STOP_MONITOR )
	{
		_MonitorAPIState = STOP_MONITOR;
		SSFSetAPICommand( API_CMD_DISABLEMONITOR, 1, 0 );
	}
}


void SSFStartMonitorAPI( void )
{
	if ( _MonitorAPIState == STOP_MONITOR )
	{
		_MonitorAPIState = START_MONITOR;
		SSFSetAPICommand( API_CMD_ENABLEMONITOR, 1, 0 );
	}
}


static void SSFEnableSnoop( void )
{
	UINT8 cdata;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	cdata = ModemDSPRead1( 0x69E );

	if ( cdata & 0x08 )
	{
		TRACE_SDAA( "SDAA EnableSnoop OK\n" );
	}
	else
	{
		ModemDSPWrite1( 0x69E, 0x08, TRUE );
		ModemDSPWrite5( 0x4821, 0x0000, 0x0800 );
		TRACE_SDAADEBUG( "SDAA EnableSnoop force setting #\n" );
	}

	OSC_Release_Mutex( &modem_dsp_mutex );

	cdata = SSFReadLSD( 0x09 );

	if ( ( cdata & 0x60 ) != 0x00 )
	{
		SSFWriteLSD( 0x09, cdata & ~0x60 );
		TRACE_SDAADEBUG( "SDAA EnableSnoop force LSD setting #\n" );
	}

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	MDM_SETBIT( MODEM_09, DATA | DTR );

	ModemDSPWrite5( 0x480C, 0x0000, 0x8000 );
	ModemDSPWrite5( 0x480E, 0x0000, 0x1000 );

	OSC_Release_Mutex( &modem_dsp_mutex );
}


static void SSFDisableSnoop( void )
{
	UINT8 cdata;

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	cdata = ModemDSPRead1( 0x69E );

	if ( cdata & 0x08 )
	{
		ModemDSPWrite1( 0x69E, cdata & ~0x08, FALSE );
		ModemDSPWrite5( 0x4821, 0x0800, 0x0000 );
		TRACE_SDAADEBUG( "SDAA DisableSnoop force setting #\n" );
	}
	else
		TRACE_SDAA( "SDAA DisableSnoop OK\n" );

	OSC_Release_Mutex( &modem_dsp_mutex );

	cdata = SSFReadLSD( 0x09 );

	if ( ( cdata & 0x60 ) != 0x20 )
	{
		cdata &= ~0x60;
		cdata |= 0x20;
		SSFWriteLSD( 0x09, cdata );
		TRACE_SDAADEBUG( "SDAA DisableSnoop force LSD setting #\n" );
	}

	OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

	ModemDSPWrite5( 0x480C, 0x8000, 0x0000 );
	ModemDSPWrite5( 0x480E, 0x1000, 0x0000 );

	MDM_CLEARBIT( MODEM_09, DTMF | DATA | DTR );

	OSC_Release_Mutex( &modem_dsp_mutex );

	SSFSetAPICommand( API_CMD_HOOKSTATE, API_HOOKPARM_ONHOOK_STATUS, 0 );

#if 0
	if ( CALLER_ID_ALWAYS )
	{
		set_phone_state( PHONE_STATE_INIT );
	}
#endif
}


static void SSFCoVoltage( void )
{
	UINT32 dwVolt, dwTemp, dwRaw;
	static int diff = 0;

	if ( MDM_BITSET( MODEM_0F, RI ) )
		return ;

	if ( SSFGetAPIStatus( API_STA_NOLINE ) )    // Check no line
	{
		dwVolt = 0;
	}
	else
	{
		OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

		if ( LineStatus == LINE_OFFHOOK )
			dwVolt = ( ( ModemDSPRead1( API_TnRVoltageHi ) * 0x100 ) + ModemDSPRead1( API_TnRVoltageLo ) );
		else
			dwVolt = ( ( ModemDSPRead1( 0x743 ) * 0x100 ) + ModemDSPRead1( 0x742 ) );

		OSC_Release_Mutex( &modem_dsp_mutex );

		if ( dwVolt < 7500 )
		{
			dwRaw = dwVolt * 10;

			if ( dwRaw > 500 )
			{
				dwTemp = dwVolt % 100; // check 1V boundary to round

				if ( dwTemp < 50 )
				{
					dwVolt -= dwTemp;
				}
				else if ( dwTemp >= 50 )
				{
					dwVolt -= dwTemp;
					dwVolt += 100;
				}
				dwVolt /= 100;
			}
			else
			{
				dwVolt = 0;
			}
		}
	}

	if ( CoVoltage != dwVolt )
	{
		diff = diff + ( dwVolt - CoVoltage);

		if(diff < -1 || diff > 1)
		{
			TRACE_SDAA( " TipRing voltage %ldV\n", dwVolt );
			diff = 0;
		}
	}

	CoVoltage = dwVolt;
}


static LINE_STATUS SSFFixedMeasurment( void )
{
	MaxTRVoltage = SDAA_Data_Table.RefLineInUseVoltage;

	if ( CoVoltage == 0 )
	{
		if ( LineStatus != LINE_NOLINE )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* No Line\n" );

				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_NOLINE;
			}
		}

		return LineStatus;
	}

	if ( CoVoltage < SDAA_Data_Table.RefLineInUseVoltage )
	{
		if ( ( LineStatus == LINE_ONHOOK )
		  || ( LineStatus == LINE_NOLINE ) )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* EXTENSION Off-hook\n"
				            "  TipRing %dV, Ref %dV\n", CoVoltage, MaxTRVoltage );

				SSFEnableSnoop();

				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_INUSE;
				return LINE_INUSE;
			}
		}
		else
		{
			LineInUseValidCount = LINE_IN_USE_VALID_TIME;
		}
	}
	else if ( CoVoltage >= SDAA_Data_Table.RefLineInUseVoltage )
	{
		if ( ( LineStatus == LINE_INUSE )
		  || ( LineStatus == LINE_NOLINE ) )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* EXTENSION On-hook\n"
				            "  TipRing %dV, Ref %dV\n", CoVoltage, MaxTRVoltage );

				SSFDisableSnoop();
				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_ONHOOK;
				return LINE_ONHOOK;
			}
		}
		else
		{
			LineInUseValidCount = LINE_IN_USE_VALID_TIME;
		}
	}

	return LineStatus;
}


static LINE_STATUS SSFAutoMeasurment( void )
{
	UINT16 wdata;

	if ( CoVoltage > ( MaxTRVoltage + 2 ) )
	{
		if ( --TRUpdateCount == 0 )
		{
			TRACE_SDAA( "  TipRingMax %dV -> %dV\n", MaxTRVoltage, CoVoltage );

			MaxTRVoltage = CoVoltage;
			TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;

			if ( SDAA_Data_Table.m_country.LineInUseThreshold != 0 )
			{
				wdata = SDAA_Data_Table.m_country.LineInUseThreshold;
			  if( MaxTRVoltage > SDAA_Data_Table.m_country.LineInUseThreshold ) {
				LineInUseVoltage = MaxTRVoltage - SDAA_Data_Table.m_country.LineInUseThreshold;
			  } else {
			  	LineInUseVoltage = 0;
			  }
			}
			else if ( SDAA_Data_Table.RefLineInUseDelta != 0 )
			{
				wdata = SDAA_Data_Table.RefLineInUseDelta;
				LineInUseVoltage = MaxTRVoltage - SDAA_Data_Table.RefLineInUseDelta;
			}
			else
			{
				if ( MaxTRVoltage > ( SDAA_Data_Table.RefLineInUseVoltage * 2 ) )
				{
					wdata = MaxTRVoltage - SDAA_Data_Table.RefLineInUseVoltage;
					LineInUseVoltage = SDAA_Data_Table.RefLineInUseVoltage;
				}
				else
				{
					wdata = MaxTRVoltage / 2;
					LineInUseVoltage = wdata;
				}
			}

			TRACE_SDAA( "  LineInUseThres = %dV\n", LineInUseVoltage );

			if ( wdata > 0 )
			{
				TRACE_SDAA( "  LineInUseDelta = %dV\n", wdata );
				wdata *= 100;

				OSC_Obtain_Mutex( &modem_dsp_mutex, OSC_SUSPEND );

				ModemDSPWrite2( API_LineInUseThreshold_H, API_LineInUseThreshold_L, wdata );

				OSC_Release_Mutex( &modem_dsp_mutex );
			}
		}
	}
	else if ( TRUpdateCount < MAXVOLTAGE_UPDATE_TIME )
	{
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
	}

	if ( MaxTRVoltage == 0 )
	{
//		TRACE_SDAADEBUG( "# MaxTRVoltage 0, CoVoltage %d\n", CoVoltage );

		if ( ( LineStatus != LINE_NOLINE )
		  && ( CoVoltage == 0 ) )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* No Line\n" );

				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_NOLINE;
			}
		}

		return LineStatus;
	}

	if ( ( CoVoltage < MaxTRVoltage )
	  && ( CoVoltage == 24 ) )
	{
		TRACE_SDAADEBUG( "# Skip by 24\n" );
		return LineStatus;
	}

	if ( ( LineStatus != LINE_NOLINE )
	  && ( CoVoltage == 0 ) )
	{
		TRACE_SDAA( "* No Line\n" );

		MaxTRVoltage  = 0;
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;

		LineStatus = LINE_NOLINE;
		return LineStatus;
	}

	/* Check extension pick-up */

	if ( SSFGetAPIStatus( API_STA_LINEINUSE )
	  || ( ( CoVoltage > 0 )
	    && ( ( CoVoltage < LineInUseVoltage )
	      || ( CoVoltage < SDAA_Data_Table.RefLineInUseVoltage ) ) ) )
	{
		if ( ( LineStatus == LINE_ONHOOK )
		  || ( LineStatus == LINE_NOLINE ) )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* EXTENSION Off-hook\n"
				            "  TipRing %dV, Ref %dV\n", CoVoltage, LineInUseVoltage );

				SSFEnableSnoop();

				if ( MaxTRVoltage > ( CoVoltage + 1 ) )
				{
					if ( !SSFGetAPIStatus( API_STA_LINEINUSE ) )
					{
						TRACE_SDAA( "# EXTENSION On-hook\n"
						            "  TipRing %dV, Ref %dV\n", CoVoltage, LineInUseVoltage );
						TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
						LineInUseValidCount++;
						LineStatus = LINE_ONHOOK;
						return LINE_ONHOOK;
					}
				}

				TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_INUSE;
				return LINE_INUSE;
			}
		}
		else
		{
			LineInUseValidCount = LINE_IN_USE_VALID_TIME;
		}
	}
	else if ( ( CoVoltage > ( LineInUseVoltage + 1 ) )
	       && ( CoVoltage >= SDAA_Data_Table.RefLineInUseVoltage ) )
	{
		if ( ( LineStatus == LINE_INUSE )
		  || ( LineStatus == LINE_NOLINE ) )
		{
			if ( --LineInUseValidCount == 0 )
			{
				TRACE_SDAA( "* EXTENSION On-hook\n"
				            "  TipRing %dV, Ref %dV\n", CoVoltage, LineInUseVoltage );

				SSFDisableSnoop();
				TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
				LineInUseValidCount = LINE_IN_USE_VALID_TIME;
				LineStatus = LINE_ONHOOK;
				return LINE_ONHOOK;
			}
		}
		else
		{
			LineInUseValidCount = LINE_IN_USE_VALID_TIME;
		}
	}

	return LineStatus;
}


LINE_STATUS SSFLineInUse( void )
{
	if ( LineStatus == LINE_OFFHOOK )
	{
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME;
		LineInUseValidCount = LINE_IN_USE_VALID_TIME;

		if ( --TRSampleCount == 0 )
		{
			TRSampleCount = 100;
			SSFCoVoltage();

                        //TRACE_SDAADEBUG( " line St%d %dV\n", LineStatus, CoVoltage );
		}
	}
	else if ( MDM_BITSET( MODEM_0F, RI ) )
	{
		TRSampleCount = TR_VOLTAGE_SAMPLE_TIME;
		TRUpdateCount = MAXVOLTAGE_UPDATE_TIME * 3;
		LineInUseValidCount = LINE_IN_USE_VALID_TIME;
	}
	else
	{
		if ( --TRSampleCount == 0 )
		{
			TRSampleCount = TR_VOLTAGE_SAMPLE_TIME;

			SSFCoVoltage();

			if ( SDAA_Data_Table.RefTRVoltage == 0 )
			{
				LineStatus = SSFAutoMeasurment();
			}
			else
			{
				LineStatus = SSFFixedMeasurment();
			}
		}
	}

	return LineStatus;
}


LINE_STATUS SSFLineStatus( void )
{
	return LineStatus;
}


UINT16 SSFLineVoltage( void )
{
	static UINT8 count;

	if ( ++count >= 100 )
	{
		count = 0;
		SSFCoVoltage();
	}

	return CoVoltage;
}


void SSFSetOnHookVoltage( UINT8 vol )
{
	SDAA_Data_Table.RefTRVoltage = vol;
	SSFInitMonitorParams( TRUE );
}


void SSFSetLineInUseVoltage( UINT8 vol )
{
	if ( vol != 0 )
	{
		SDAA_Data_Table.RefLineInUseVoltage = vol;
	}
	else
	{
                SDAA_Data_Table.RefLineInUseVoltage = MinDetectableTipRingVolt;
	}

	SSFInitMonitorParams( TRUE );
}


void SSFSetLineInUseDelta( UINT8 vol )
{
	SDAA_Data_Table.RefLineInUseDelta = vol;
	SSFInitMonitorParams( TRUE );
}


void SSFSetLineInUseThreshold( UINT8 vol )
{
        SDAA_Data_Table.m_country.LineInUseThreshold = vol;
        SSFInitMonitorParams( TRUE );
}

