/*
 *  $RCSfile: callerid.c,v $
 * $Revision: 1.7 $
 *   $Author: miurat $
 *     $Date: 2010/08/11 22:52:15 $
 */
/************************************************************************
 *
 *      callerid.c
 *
 *  Caller Id. (CID/CND) Detection functions.
 *  The decoder design is OS independent and hardware independent.
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
#include "fsu_api.h"


extern void SSFStopMonitorAPI( void );
extern void SSFStartMonitorAPI( void );
extern void CxPhoneReport( UINT8 type, UINT8 *buffer );
extern void process_cid( void );

void CIDType2Start( void );

/* It's not fast enough to receive data by polling.
   The final code use ISR to receive data.
 */


/*********************************************************************/

#define _DEBUG

/***    This section of code is for CID (Caller Id.) Detection     ***/

#define CID_CHANNEL_SEISURE   0x55 /* Channel Seisure Signal (On-Hook only) */

/* CID/CND Message Types definitions: (SDMF and MDMF)*/
/* Note: only  CID_MSG_TYPE_CNDI and CID_MSG_TYPE_SETUP are supported */
#define CID_MSG_TYPE_CNDI    0x04 /*SDMF: Calling Number Delivery Information */
#define CID_MSG_TYPE_MSGWTI  0x0A /*SDMF: Message Waiting Indicator (TBD) */
#define CID_MSG_TYPE_MSGDESK 0x0B /*SDMF: Message Desk Information (TBD) */
#define CID_MSG_TYPE_MDMF    0x80 /*MDMF indentified by MSB bit set */
#define CID_MSG_TYPE_SETUP   0x80 /*MDMF: Call Setup */
#define CID_MSG_TYPE_TEST    0x81 /*MDMF: Test For Calling Number Deli' (TBD) */
#define CID_MSG_TYPE_MSGWTN  0x82 /*MDMF: Message Waiting Notification (TBD) */

/* CID/CND MDMF Parameter Types definitions: */
/* Note: not all supported */
#define CID_PAR_TYPE_DATETIME 0x01 /* Param Type: Date/Time */
#define CID_PAR_TYPE_LINEID   0x02 /* Param Type: Calling Line Identification */
#define CID_PAR_TYPE_NUMBER   0x03 /* Param Type: Dialable Directory Number */
#define CID_PAR_TYPE_REASONA  0x04 /* Param Type: Reason for Absence of DN */
#define CID_PAR_TYPE_REASONR  0x05 /* Param Type: Reason for Redirection */
#define CID_PAR_TYPE_CALLQ    0x06 /* Param Type: Call Qualifier */
#define CID_PAR_TYPE_NAME     0x07 /* Param Type: Directory Name */

/* CID/CND Timing definitions: */
/* Note: minimum time is twice of SamplingTime (2x 40 ms) */
#define CID_TYPE1_TIMEOUT     2000 /* Type 1 Msg shall start between .5-1.5 s */
/* after the end of first ringing pattern */
/* + 300 bits of Channel Seisure Signal */
/* + 180 bits of Mark */
#define CID_TYPE2_TIMEOUT     1000 /* Type 2 Msg shall start between 50-500 ms*/
/* after detection of an ACK */
/* + 80 bits of Mark */
#define CID_CAS_TIMEOUT       200  /* 80-85 Length of CPE Alerting Signal */
#define CID_MSG_TIMEOUT       500  /* 8.3 ms (10 bits) Msg Inter-Byte Timeout */

#define CID_ACK_TIME          65   /* 55-65 ms ACT (DTMF D or A) */
#define CID_ACK_DTMF          'D'  /* ACT (DTMF D or A) */


/* CID/CND data size definitions: */
#define CID_DATA_SIZE         0x3F /* Max. valid CID Data size supported */
#define CID_BUFFER_SIZE       0x44 /* CID Raw Data buffer size supported */


typedef enum                  /* Caler Id. Decoder State Machine states */
{
    CID_WAIT_NONE,               /* Idle State */
    CID_WAIT_CAS_BEGIN,          /* Wait for CPE Alerting Signal (Type 2) */
    CID_WAIT_CAS_END,
    CID_WAIT_ACK_BEGIN,          /* Wait to send ACK to CAS (Type 2) */
    CID_WAIT_ACK_END,
    CID_WAIT_CHANNEL_SEISURE,    /* Channel Seisure Signal (Type 1 only) */
    CID_WAIT_MESSAGE_TYPE,       /* Wait for Message Type */
    CID_WAIT_MESSAGE_LENGTH,     /* Wait for Message Length */
    CID_WAIT_MESSAGE_BODY,       /* Wait for Message Body */
    CID_WAIT_PARAMETER_TYPE,     /* Wait for Message Body: Parameter Type */
    CID_WAIT_PARAMETER_LENGTH,   /* Wait for Message Body: Parameter Length */
    //  CID_WAIT_PARAMETER_BODY,    /* Wait for Message Body: Parameter Body */
    //  CID_WAIT_PARAM_DATETIME,    /* Wait for Param Body: Date/Time */
    //  CID_WAIT_PARAM_LINEID,      /* Wait for Param Body: Calling Line Id. */
    //  CID_WAIT_PARAM_NUMBER,      /* Wait for Param Body: Directory Number */
    CID_WAIT_PARAM_REASONA,      /* Wait for Param Body: Reason for Absence */
    //  CID_WAIT_PARAM_NAME,        /* Wait for Param Body: Directory Name */
    CID_WAIT_NEXT_PARAM,         /* Wait for Next Parameter */
    CID_DATA_READY               /* Rx done, Data ready to decode */
} CID_DECODE_STATES;


static CID_INFORMATION CidInformation;
static CID_DECODE_STATES CidDecodeState;
static UINT8 CidIndex;
static UINT16 CidTimer;
static BOOL bCidType1;

static UINT8 CidBuffer[ CID_BUFFER_SIZE ];

/*-------------------------------------------------------------------
    Name: CidDecodeDateTime
          Decode Data/Time ASCII string in the Raw data buffer and
          store the result in CidInformation structure.

    Input:  pChecksum  - address where checksum stored
            pCidRaw    - address where Data/Time ASCII string started
    Output: *pChecksum - current checksum
            CidInformation
    Return: none
 *-------------------------------------------------------------------*/

static void CidDecodeDateTime( UINT8 *pChecksum, UINT8 *pCidRaw )
{
	UINT8 data;

	/* Convert ASCII Month to binary */
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Month = ( data - '0' ) * 10;
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Month += ( data - '0' );

	/* Convert ASCII Day to binary */
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Day = ( data - '0' ) * 10;
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Day += ( data - '0' );

	/* Convert ASCII Hour to binary */
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Hour = ( data - '0' ) * 10;
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Hour += ( data - '0' );

	/* Convert ASCII Minute to binary */
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Minute = ( data - '0' ) * 10;
	data = *pCidRaw++;
	*pChecksum += data;
	CidInformation.Minute += ( data - '0' );
}


/*-------------------------------------------------------------------
    Name: CidDecodeMsg
          Decode Caller Id. message structure in the raw data buffer and
          store the result in CidInformation structure.

    Input:  pCidRaw     - address of the raw data buffer
            MaxDataSixe - Max data size in the raw data buffer
            OnHook      - On Hook (TRUE) or Off Hook (FALSE) data communication
                          On Hook data start from Channel Seisure
                          Off Hook data start from Message Type directly
    Output: CidInformation
    Return: none

    Note: There are noises before Channel Seisure and before Message Type.
          It might match with Message Type, therefor, error correction
          may become necessary.
 *-------------------------------------------------------------------*/

static void CidDecodeMsg( UINT8 *pCidRaw, UINT8 MaxDataSize, BOOL OnHook )
{
	CID_DECODE_STATES DecodeState;
	UINT8 data, Checksum, MsgByteCount = 0, CidDataCount, Index;
	BOOL bSDMF = TRUE;

	TRACE_PHONE( "CidDecodeMsg() entered.\n" );
	//if (OnHook)
	//    DecodeState = CID_WAIT_CHANNEL_SEISURE;
	//else
	DecodeState = CID_WAIT_MESSAGE_TYPE; /* Take care CHANNEL_SEISURE too */

	for ( CidDataCount = 0; CidDataCount < MaxDataSize; CidDataCount++ )
	{
		data = *pCidRaw++;

		switch ( DecodeState )
		{
			case CID_WAIT_CHANNEL_SEISURE:

				//TRACE_PHONE( "CID_WAIT_CHANNEL_SEISURE:\n" );

				/* Remove noises before Channel Seisure */
				if ( data == CID_CHANNEL_SEISURE )
					DecodeState = CID_WAIT_MESSAGE_TYPE;
				break;

			case CID_WAIT_MESSAGE_TYPE:

				//TRACE_PHONE( "CID_WAIT_MESSAGE_TYPE:\n" );

				bSDMF = FALSE;

				switch ( data )
				{
					case CID_MSG_TYPE_CNDI:
					case CID_MSG_TYPE_MSGWTI:
					case CID_MSG_TYPE_MSGDESK:
						bSDMF = TRUE;
					case CID_MSG_TYPE_SETUP:
					case CID_MSG_TYPE_TEST:
					case CID_MSG_TYPE_MSGWTN:
						DecodeState = CID_WAIT_MESSAGE_LENGTH;
						Checksum = data;
						break;
				}
				break;

			case CID_WAIT_MESSAGE_LENGTH:

				//TRACE_PHONE( "CID_WAIT_MESSAGE_LENGTH:\n" );

				switch ( data )
				{
					case CID_MSG_TYPE_SETUP:
					case CID_MSG_TYPE_TEST:      /* TBD */
					case CID_MSG_TYPE_MSGWTN:    /* TBD */

						TRACE_PHONE( "CidDecodeMsg: Error correction.\n" );
						Checksum = data;
						bSDMF = FALSE;
						break;

					case CID_MSG_TYPE_CNDI:

						if ( bSDMF )
						{
							TRACE_PHONE( "CidDecodeMsg: Error correction.\n" );
							Checksum = data;
							break;
						}
					case CID_MSG_TYPE_MSGWTI:    /* TBD */
					case CID_MSG_TYPE_MSGDESK:   /* TBD */
					default:

						DecodeState = CID_WAIT_MESSAGE_BODY;
						Checksum += data;
						MsgByteCount = data;    /* exclude checksum byte */
#ifdef _DEBUG
						if ( data > CID_DATA_SIZE )
							TRACE_PHONE( "CidDecodeMsg: MessageLength Error!\n" );
						TRACE_PHONE( "MessageLength: %u.\n", data );
#endif
						break;
				}
				break;

			default:

				TRACE_PHONE( "CidDecodeMsg: Design Error! Undefined state.\n" );
				break;
		}

		if ( DecodeState == CID_WAIT_MESSAGE_BODY )
			break;
	}

	/* Reset the CID Information Buffer */
	if ( OnHook )
		CidInformation.Type = 1;    /* Type 1 CID */
	else
		CidInformation.Type = 2;    /* Type 2 CID */

	CidInformation.Reason = CID_NO_DATA;
	CidInformation.LineId[ 0 ] = 0;
	CidInformation.Number[ 0 ] = 0;
	CidInformation.Name[ 0 ]   = 0;

	if ( DecodeState != CID_WAIT_MESSAGE_BODY )
	{
		TRACE_PHONE( "CidDecodeMsg: Error!\n" );
		return ;
	}

	if ( bSDMF )
	{
		//TRACE_PHONE( "CidDecodeMsg() SDMF.\n" );
		CidDecodeDateTime( &Checksum, pCidRaw );
		MsgByteCount -= 8;
		CidDataCount += 8;
		pCidRaw += 8;

		/* Copy DN Information */
		for ( Index = 0;
		      ( Index < CID_ID_LENGTH ) && ( MsgByteCount > 0 );
		      Index++, MsgByteCount-- )
		{
			data = *pCidRaw++;
			CidInformation.LineId[ Index ] = data;
			Checksum += data;
		}

		CidInformation.LineId[ Index ] = 0;
		data = CidInformation.LineId[ 0 ];

		if ( data == CID_PRIVATE || data == CID_OUT_OF_AREA )
			CidInformation.Reason = data;
	}
	else    /* MDMF */
	{
		UINT8 ParameterType = 0, ParameterLength = 0;

#ifdef _DEBUG

		TRACE_PHONE( "CidDecodeMsg() MDMF.\n" );
		TRACE_PHONE( "MsgByteCount: %u, CidDataCount: %u, MaxDataSize: %u.\n",
		             MsgByteCount, CidDataCount, MaxDataSize );
#endif

		DecodeState = CID_WAIT_PARAMETER_TYPE;

		for ( ; MsgByteCount > 0 && CidDataCount < MaxDataSize; )
		{
			data = *pCidRaw++;
			Checksum += data;
			MsgByteCount--;
			CidDataCount++;

			switch ( DecodeState )
			{
				case CID_WAIT_PARAMETER_TYPE:
					//TRACE_PHONE("CID_WAIT_PARAMETER_TYPE:\n" );
					ParameterType = data;
					DecodeState = CID_WAIT_PARAMETER_LENGTH;
					break;

				case CID_WAIT_PARAMETER_LENGTH:

					//TRACE_PHONE("CID_WAIT_PARAMETER_LENGTH:\n" );

					ParameterLength = data;
					//TRACE_PHONE("ParameterLength: %u.\n", ParameterLength);
					//DecodeState = CID_WAIT_PARAMETER_BODY:
					//break;

				//case CID_WAIT_PARAMETER_BODY:

					switch ( ParameterType )
					{
						case CID_PAR_TYPE_DATETIME:

#ifdef _DEBUG
							//TRACE_PHONE( "CID_PAR_TYPE_DATETIME:\n" );
							//DecodeState = CID_WAIT_PARAM_DATETIME;
							//TRACE_PHONE("CID_WAIT_PARAM_DATETIME:\n" );
							if ( ParameterLength != 8 )
								TRACE_PHONE( "CidDecodeMsg: Error! Wrong Param size.\n" );
#endif
							DecodeState = CID_WAIT_PARAMETER_TYPE;
							CidDecodeDateTime( &Checksum, pCidRaw );
							MsgByteCount -= 8;
							CidDataCount += 8;
							pCidRaw += 8;
							break;

						case CID_PAR_TYPE_LINEID:

							//TRACE_PHONE( "CID_PAR_TYPE_LINEID:\n" );

							for ( Index = 0;
							      ParameterLength > 0 && Index < CID_ID_LENGTH;
							      ParameterLength--, Index++ )
							{
								data = *pCidRaw++;
								CidInformation.LineId[ Index ] = data;
								Checksum += data;
								MsgByteCount--;
								CidDataCount++;
							}

							CidInformation.LineId[ Index ] = 0;

							if ( ParameterLength == 0 )
								DecodeState = CID_WAIT_PARAMETER_TYPE;
							else
							{
								DecodeState = CID_WAIT_NEXT_PARAM;
#ifdef _DEBUG
								TRACE_PHONE( "Error! ParameterLength: %u.\n",
								             ParameterLength );
#endif
							}
							break;

						case CID_PAR_TYPE_NUMBER:

							//TRACE_PHONE( "CID_PAR_TYPE_NUMBER:\n" );

							for ( Index = 0;
							      ParameterLength > 0 && Index < CID_DN_LENGTH;
							      ParameterLength--, Index++ )
							{
								data = *pCidRaw++;
								CidInformation.Number[ Index ] = data;
								Checksum += data;
								MsgByteCount--;
								CidDataCount++;
							}

							CidInformation.Number[ Index ] = 0;

							if ( ParameterLength == 0 )
								DecodeState = CID_WAIT_PARAMETER_TYPE;
							else
							{
								DecodeState = CID_WAIT_NEXT_PARAM;
#ifdef _DEBUG
								TRACE_PHONE( "Error! ParameterLength: %u.\n",
								             ParameterLength );
#endif
							}
							break;

						case CID_PAR_TYPE_REASONA:

							//TRACE_PHONE( "CID_PAR_TYPE_REASONA:\n" );
							DecodeState = CID_WAIT_PARAM_REASONA;
							break;

						case CID_PAR_TYPE_NAME:

							//TRACE_PHONE( "CID_PAR_TYPE_NAME:\n" );

							for ( Index = 0;
							      ParameterLength > 0 && Index < CID_NAME_LENGTH;
							      ParameterLength--, Index++ )
							{
								data = *pCidRaw++;
								CidInformation.Name[ Index ] = data;
								Checksum += data;
								MsgByteCount--;
								CidDataCount++;
							}

							CidInformation.Name[ Index ] = 0;

							if ( ParameterLength == 0 )
								DecodeState = CID_WAIT_PARAMETER_TYPE;
							else
							{
								TRACE_PHONE( "CidDecodeMsg: Error! Wrong Param size.\n" );
								DecodeState = CID_WAIT_NEXT_PARAM;
							}
							break;

						case CID_PAR_TYPE_REASONR:
						case CID_PAR_TYPE_CALLQ:
#ifdef _DEBUG
							TRACE_PHONE( "CidDecodeMsg: Not supported ypes: %u.\n",
							             DecodeState );
#endif
							DecodeState = CID_WAIT_NEXT_PARAM;
							break;

						default:
#ifdef _DEBUG
							TRACE_PHONE( "CidDecodeMsg: Undefined types: %u.\n",
							             DecodeState );
#endif
							DecodeState = CID_WAIT_NEXT_PARAM;
							break;
					}
					break;

				case CID_WAIT_PARAM_REASONA:

					//TRACE_PHONE( "CID_WAIT_PARAM_REASONA:\n" );
					CidInformation.Reason = data;

					if ( --ParameterLength == 0 )
						DecodeState = CID_WAIT_PARAMETER_TYPE;
					else
					{
						TRACE_PHONE( "CidDecodeMsg: Error! Wrong Param size.\n" );
						DecodeState = CID_WAIT_NEXT_PARAM;
					}
					break;

				case CID_WAIT_NEXT_PARAM:         /* Wait for Next Parameter */

					if ( --ParameterLength == 0 )
						DecodeState = CID_WAIT_PARAMETER_TYPE;
					break;

				default:
#ifdef _DEBUG
					TRACE_PHONE( "CidDecodeMsg: Error! Undefined types: %u.\n",
					           DecodeState );
#endif
					if ( --ParameterLength == 0 )
						DecodeState = CID_WAIT_PARAMETER_TYPE;
					break;

			}   /* switch(DecodeState) */
		}   /* for(;;) */
	}

#ifdef _DEBUG
	//TRACE_PHONE("MsgByteCount: %u, CidDataCount: %u, MaxDataSize: %u, "
	//            "Index: %u.\n", MsgByteCount, CidDataCount, MaxDataSize, Index);
	TRACE_PHONE( "Checksum: 0x%2X, 0x%2X, Reason: %u.\n",
	             Checksum, 256 - Checksum, CidInformation.Reason );
#endif

	Checksum += *pCidRaw;

	if ( Checksum != 0 )
		CidInformation.Reason = CID_DATA_ERROR;
	else if ( CidInformation.Reason == CID_NO_DATA )
		CidInformation.Reason = CID_DATA_VALID;

	DecodeState = CID_WAIT_NONE;

#if 1//_DEBUG
	TRACE_PHONE( "Date: %u/%u, Time: %u:%u.  Reason: %u, '%c'.\n",
	           CidInformation.Month, CidInformation.Day,
	           CidInformation.Hour, CidInformation.Minute,
	           CidInformation.Reason, CidInformation.Reason );
	TRACE_PHONE( "LineId: %s, Number: %s, Name: %s.\n",
	           CidInformation.LineId, CidInformation.Number, CidInformation.Name );
	TRACE_PHONE( "CidDecodeMsg() done.\n" );
#endif

	CxPhoneReport( PHONE_CALLERID, ( UINT8 * ) &CidInformation );
}


/*-------------------------------------------------------------------
    Name: CidTestData
          Test Caller Id. data stream to decide where is the message and
          the end of message.

    Input:  Data       - raw data buffer
            CidDecodeState - Current Decode State
    Output: CidDecodeState - Next Decode State
    Return: BOOL - TRUE: Message, FALSE: Channel Seisure

    Note: There are noises before Channel Seisure and before Message Type.
          It might match with Message Type, therefor, error correction
          may become necessary.
 *-------------------------------------------------------------------*/

static BOOL CidTestData( UINT8 Data )
{
	static UINT8 Checksum, MsgByteCount;
	static BOOL bSDMF;
	//ERCODE ercode = DummyBug();     /* to work around a compiler bug */

	//TRACE_PHONE("CidTestData() entered.\n" );

	switch ( CidDecodeState )
	{
		case CID_WAIT_CHANNEL_SEISURE:

			//TRACE_PHONE("CID_WAIT_CHANNEL_SEISURE:\n" );

			/* Remove noises before Channel Seisure */
			if ( Data == CID_CHANNEL_SEISURE )
			{
				CidDecodeState = CID_WAIT_MESSAGE_TYPE;
				SSFStopMonitorAPI();
			}

			return FALSE;

		case CID_WAIT_MESSAGE_TYPE:

			//TRACE_PHONE("CID_WAIT_MESSAGE_TYPE:\n" );
			bSDMF = FALSE;

			switch ( Data )
			{
				case CID_MSG_TYPE_CNDI:
				case CID_MSG_TYPE_MSGWTI:
				case CID_MSG_TYPE_MSGDESK:
					bSDMF = TRUE;
				case CID_MSG_TYPE_SETUP:
				case CID_MSG_TYPE_TEST:
				case CID_MSG_TYPE_MSGWTN:
					Checksum = Data;
					CidDecodeState = CID_WAIT_MESSAGE_LENGTH;
					break;

				case CID_CHANNEL_SEISURE:
				default:
					return FALSE;
					break;
			}
			break;

		case CID_WAIT_MESSAGE_LENGTH:

			//TRACE_PHONE("CID_WAIT_MESSAGE_LENGTH:\n" );

			CidTimer = CID_MSG_TIMEOUT;

			switch ( Data )
			{
				case CID_MSG_TYPE_SETUP:
				case CID_MSG_TYPE_TEST:      /* TBD */
				case CID_MSG_TYPE_MSGWTN:    /* TBD */

					TRACE_PHONE( "CidTestData: Error correction.\n" );
					Checksum = Data;
					break;

				case CID_MSG_TYPE_CNDI:

					if ( bSDMF )
					{
						TRACE_PHONE( "CidTestData: Error correction.\n" );
						Checksum = Data;
						break;
					}

				case CID_MSG_TYPE_MSGWTI:    /* TBD */
				case CID_MSG_TYPE_MSGDESK:   /* TBD */
				default:

					CidDecodeState = CID_WAIT_MESSAGE_BODY;
					Checksum += Data;
					MsgByteCount = Data + 1;  /* include checksum byte */
#ifdef _DEBUG
					if ( Data > CID_DATA_SIZE )
						TRACE_PHONE( "CidTestData: MessageLength Error!\n" );
					TRACE_PHONE( "MessageLength: %u.\n", Data );
#endif
					break;
			}
			break;

		case CID_WAIT_MESSAGE_BODY:

			CidTimer = CID_MSG_TIMEOUT;
			Checksum += Data;

			if ( --MsgByteCount == 0 )
			{
#ifdef _DEBUG
				if ( Checksum != 0 )
					TRACE_PHONE( "CidTestData: Error! Checksum: 0x%2X, 0x%2X\n",
					           Checksum, 256 - ( Checksum - Data ) );
				TRACE_PHONE( "CidTestData() done\n\n" );
#endif
				CidDecodeState = CID_DATA_READY;    /* End Of Message */
			}
			break;

		default:

			TRACE_PHONE( "CidTestData: Design Error! Undefined state.\n" );
			break;
	}
	return TRUE;                        /* Message in progress */
}


/*********************************************************************/

/*-------------------------------------------------------------------
    Name: CidCASSetup
          Setup Modem to receive CAS for Type 2 Caller Id. information

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

static void CidCASSetup( void )
{
	CidDecodeState = CID_WAIT_CAS_BEGIN;
}


/*-------------------------------------------------------------------
    Name: CidRxSetup
          Setup Modem to receive Caller Id. information

    Input:  OnHook - On Hook (TRUE) or Off Hook (FALSE) data communication
                     On Hook data start from Channel Seisure
                     Off Hook data start from Message Type directly
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

static void CidRxSetup( BOOL OnHook )
{
	if ( OnHook )
		CidDecodeState = CID_WAIT_CHANNEL_SEISURE;
	else
		CidDecodeState = CID_WAIT_MESSAGE_TYPE; /* Take care CHANNEL_SEISURE */

	CidInformation.Reason = CID_NO_DATA; /* Reset the CID Information Buffer */
	CidIndex = 0;

	ModemRxCIDSetup();
	TRACE_PHONE( "CidRxSetup() done (ISR).\n" );
}


/*-------------------------------------------------------------------
    Name: CidShutdown
          Shutdown Caller Id. reception

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

void CIDShutdown( void )
{
	CidTimer = 0;
	ModemRxCIDShutdown();
	SSFStartMonitorAPI();
	set_phone_state( PHONE_STATE_RING_CHECK );
	TRACE_PHONE( "CidShutdown() done.\n" );
}


/*-------------------------------------------------------------------
    Name: CidRxReset - Decode received Caller Id. data then shutdown reception.

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

static void CidRxReset( void )
{
	//extern void StopType1CID( void );

	if ( CidDecodeState == CID_DATA_READY )
	{
		CidDecodeMsg( &CidBuffer[ 0 ], CID_BUFFER_SIZE, bCidType1 );
		CidDecodeState = CID_WAIT_NONE;
	}

	if ( bCidType1 )
	{
		//StopType1CID();
		CIDShutdown();
	}
	else
	{
		CIDShutdown();
		CIDType2Start();
	}
}


/*-------------------------------------------------------------------
    Name: CidRxData
          Receiving Caller Id. information, test it and store in buffer.
          It is normally call by an ISR.
          It will reset the Message Inter-Byte timeout.

    Input:  None
    Output: CidBuffer - Raw data buffer
    Return: TRUE - End Of Message or buffer full, FALSE - others
 *-------------------------------------------------------------------*/

BOOL CIDRxData( UINT8 data )
{
	BOOL bCidEOM;

	if ( CidIndex >= CID_BUFFER_SIZE )
	{
		bCidEOM = TRUE;
	}
	else
	{
		if ( CidTestData( data ) )
			CidBuffer[ CidIndex++ ] = data;
		bCidEOM = ( CidDecodeState == CID_DATA_READY ) ? TRUE : FALSE;
	}

	if ( bCidEOM )
	{
		CidRxReset();

		process_cid();
	}

	return bCidEOM;
}


/*********************************************************************/

/** Gateway to external functions in/for other modules **/

/*-------------------------------------------------------------------
    Name: CidType1Start
          Type 1 Caller Id. reception initialization

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

void CIDType1Start( void )
{
	bCidType1 = TRUE;
	CidTimer = CID_TYPE1_TIMEOUT;
	CidRxSetup( TRUE );
}


/*-------------------------------------------------------------------
    Name: CIDType2Start
          Type 2 Caller Id. reception initialization

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

void CIDType2Start( void )
{
	bCidType1 = FALSE;
	CidCASSetup();
}


/*-------------------------------------------------------------------
    Name: CIDGetInforAddr
          Get address of Caller Id's information structure.

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

CID_INFORMATION *CIDGetInforAddr( void )
{
	return &CidInformation;
}


/*-------------------------------------------------------------------
    Name: CIDMonitor
          Caller Id. reception monitor function.

    Input:  SamplingTime in milli-second
    Output: None
    Return: TRUE:  CID_DATA_READY
            FALSE: Not ready
 *-------------------------------------------------------------------*/

BOOL CIDMonitor( UINT8 SamplingTime )
{
	if ( CidDecodeState == CID_DATA_READY )
	{
		CidRxReset();
		return TRUE;
	}

	if ( CidTimer == 0 )
		return FALSE;
	else if ( CidTimer > SamplingTime )
	{
		CidTimer -= SamplingTime;
		return FALSE;
	}
	else
		CidTimer = 0;

	switch ( CidDecodeState )
	{
		case CID_WAIT_CAS_END:
		case CID_WAIT_ACK_BEGIN:
		case CID_WAIT_ACK_END:
		case CID_WAIT_CHANNEL_SEISURE:
		case CID_WAIT_MESSAGE_TYPE:
		case CID_WAIT_MESSAGE_LENGTH:
		case CID_WAIT_MESSAGE_BODY:

#ifdef _DEBUG
			TRACE_PHONE( "CidMonitor: Error! Message Timeout. state: %u, "
			             "CidIndex: %u.\n", CidDecodeState, CidIndex );
#endif
			CidRxReset();
			break;

		case CID_WAIT_NONE:
		case CID_WAIT_CAS_BEGIN:
			/* No Operation */
		//case CID_DATA_READY:
		default:

#ifdef _DEBUG
			TRACE_PHONE( "CidMonitor: Design Error! Undefined state: %u, "
			           "CidIndex: %u.\n", CidDecodeState, CidIndex );
#endif
			break;
	}
	return FALSE;
}


/*-------------------------------------------------------------------
    Name: CIDBufferAddr
          Get address of Caller Id receive buffer.

    Input:  None
    Output: None
    Return: None
 *-------------------------------------------------------------------*/

void* CIDBufferAddr( void )
{
	return CidBuffer;
}


/*********************************************************************/

