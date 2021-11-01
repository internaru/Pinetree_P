/*
 *  $RCSfile: cxshell.c,v $
 * $Revision: 1.28 $
 *   $Author: miurat $
 *     $Date: 2012/02/29 21:54:08 $
 */
/************************************************************************
 *
 *      cxshell.c
 *
 *  Copyright 2008 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "cxshell.h"
#include "oem_cty.h"
#include "mosi.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "MODEMX.H"
#include "PHONEX.H"
#include "T30X.H"
#include "fsu_api.h"

// String version of Product HW-Platform Version MAJOR.MINOR.(RAM)
#define PRODVER "SSF336 1.15.\0"

void *CxModemOpen( void *homolofunc, UINT8 *status );
void CxSelectCountry( UINT8 nCountryCode );

extern void phone_init_params( CtryPrmsStruct *Ctry );
extern void t30_init_params( CtryPrmsStruct *Ctry );
extern void SSFInitParams( CtryPrmsStruct *Ctry );

extern const CtryPrmsStruct* Country_Table[];

static CtryPrmsStruct  CtryPrms;
static void (*callback_homolo)(void);
static void (*callback_host)( UINT8, UINT8*);

UINT8 gFaxT35CountryID[40] = {0, };
#ifdef CX_DEBUG
UINT8 _CxDebugLevel = 0;
#endif

static UINT8 Convert_T30_Table[ 23 ] =
{
	PHONE_DETECTEDFAX, //STATE_IDLE
	RX_PHASE_A,    //STATE_RX_PHASE_A
	RX_PHASE_B,    //STATE_RX_PHASE_B
	RX_PHASE_B,    //STATE_RX_PHASE_B_SIGNAL_REC
	RX_PHASE_C,    //STATE_RX_PHASE_C
	RX_PHASE_C,    //STATE_RX_PHASE_C_ECM
	RX_PHASE_D,    //STATE_RX_PHASE_D_SIGNAL_REC
	RX_PHASE_D,    //STATE_RX_PHASE_D
	RX_PHASE_E,    //STATE_RX_PHASE_E
	RX_DISCONN,    //STATE_RX_DISCONN
	RX_ABORT,      //STATE_RX_ABORT
	TX_PHASE_A,    //STATE_TX_PHASE_A
	TX_PHASE_B,    //STATE_TX_PHASE_B
	TX_PHASE_B,    //STATE_TX_SET_MODE
	TX_PHASE_C,    //STATE_TX_PHASE_C
	TX_PHASE_C,    //STATE_TX_PHASE_C_ECM
	TX_PHASE_D,    //STATE_TX_PHASE_D
	TX_PHASE_D,    //STATE_TX_PHASE_D2
	TX_PHASE_E,    //STATE_TX_PHASE_E
	TX_DISCONN,    //STATE_TX_DISCONN
	TX_VOICE_REQ,  //STATE_TX_VOICE_REQ
	TX_ABORT,      //STATE_TX_ABORT
	HOST_ABORT     //STATE_HOST_ABORT
};


void *CxModemOpen( void *homolofunc, UINT8 *status )
{
	CtryPrms.T35Code = 0xff;
	*status = ModemInitJob();
	callback_homolo = ( void (*))homolofunc;
	return (void *)&CtryPrms;
}


UINT8 CxModemRingStatus( void )
{
	return ModemDetectRing();
}


UINT8 CxModemLineStatus( void )
{
	return SSFLineStatus();
}


void CxSelectCountry( UINT8 nCountryCode )
{
	UINT16 Cnum, i;

	TRACE_SHELL( "Country code %02X\n", nCountryCode );

	if ( nCountryCode == 0xff )
	{
		t30_init_params( 0 );  // copy default
		T30Attribute( GET, FXA_COUNTRY_CODE, &nCountryCode );
		TRACE_SHELL( " change to default %02X\n", nCountryCode );
	}

	if ( CtryPrms.T35Code != nCountryCode )      // Country code changed
	{
		Cnum = 0xffff;

		for ( i = 0; Country_Table[ i ] != 0; i++ )
		{
			if ( Country_Table[ i ]->T35Code == nCountryCode )
			{
				Cnum = i;
				break;
			}
		}

		if ( Cnum != 0xffff )
		{
			if ( memcmp( CTY_SIGNATURE, Country_Table[ Cnum ]->Signature, sizeof( Country_Table[ Cnum ]->Signature ) ) == 0 )
			{
				memcpy( &CtryPrms, Country_Table[ Cnum ], sizeof( CtryPrmsStruct ) );
				TRACE_SHELL( "%s %02X\n", CtryPrms.cInter, CtryPrms.T35Code );

                memset(gFaxT35CountryID, 0x00, sizeof(gFaxT35CountryID));
                for (i = 0; i < 40 && i < sizeof(CtryPrms.cInter); i++)
                {
                    if (CtryPrms.cInter[i])
                    {
                        gFaxT35CountryID[i] = CtryPrms.cInter[i];
                    }
                    else
                    {
                        break;
                    }
                }
			}
			else
            {
				TRACE_ERROR( "### Country parameter error ### (%02x)%02X\n", nCountryCode, Cnum );
            }
		}
	}

	if ( callback_homolo )
	{
		callback_homolo();
	}

	SSFInitParams( &CtryPrms );
	phone_init_params( &CtryPrms );
	t30_init_params( &CtryPrms );
}


#ifdef OEM_FAX_REPORT

void CxProtocolReport( UINT8 fcf, UINT16 *buffer, UINT8 length, UINT8 direction )
{
	FAX_EVENT sFaxEvent;

	sFaxEvent.type = ( FAX_EVENT_STATE ) ( Convert_T30_Table[ T30Control->State ] );
	sFaxEvent.dir  = ( FAX_DIRECTION ) direction;
	sFaxEvent.fcf  = ( UINT16 ) fcf;
	sFaxEvent.size = ( UINT16 ) length;
	sFaxEvent.data = buffer;
#ifdef SIGNED_LEVEL
	sFaxEvent.lvl  = ( INT8 ) ModemControl->Level;
#else
	sFaxEvent.lvl  = ( UINT8 )~ModemControl->Level + 1;
#endif
	sFaxEvent.eqm  = ModemControl->EQM;
	fax_events( &sFaxEvent );
}
#endif


void CxSendTestSignal( UINT8 mode )
{
	phone_send_test_signal( ( MODEM_TX_TEST_INDEX ) mode );
}



void CxSetCallBackHost( void *callback )
{
	callback_host = ( void (*))callback;
}


void CxCallBackHost( UINT8 info, UINT8* buff )
{
	if ( callback_host )
	{
		callback_host( info, buff );
	}
}
