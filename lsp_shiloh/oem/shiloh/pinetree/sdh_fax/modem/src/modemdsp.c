/*
 *  $RCSfile: modemdsp.c,v $
 * $Revision: 1.10 $
 *   $Author: miurat $
 *     $Date: 2011/12/16 02:27:20 $
 */
/************************************************************************
 *
 *      modemdsp.c
 *
 *  SSF336 Modem DSP Interface Functions.
 *
 *  Copyright 2004 - 2011 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "dc_common_includes.h"
#include "cxshell.h"
#include "ATTRIB.H"
#include "BUFFERS.H"
#include "ERROR.H"
#include "MODEMX.H"
#include "T30X.H"
#include "ECMX.H"
#include "modem336.h"

#define OSC_DISABLE_IRQ()
#define OSC_ENABLE_IRQ()


/*===========================================================================
*
*    ModemDSPRead1
*
*   Reads DSP RAM using Method 1 per SSF336 Designer's Guide
*
*   Input:      Ram address
*   Output:     desired value
*
*===========================================================================*/
UINT8 ModemDSPRead1( UINT16 addr )
{
	UINT8 retval;

	OSC_DISABLE_IRQ();

	/* Set up to read the current value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addr ) );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addr ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for the operation to be complete */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	retval = MDM_READ( MODEM_MEDAL );

	OSC_ENABLE_IRQ();

	return ( retval );

}

/*===========================================================================
*
*    ModemDSPWrite1
*
*   Updates DSP RAM using Method 1 per SSF336 Designer's Guide
*
*   Input:      Ram address
*               New Value
*               flag indicating whether we only set a specific bit
*   Output:     Modem DSP modified
*
*===========================================================================*/
void ModemDSPWrite1( UINT16 addr, UINT8 data, BOOL bitset_only )
{
	UINT8 oldval;

	oldval = 0;

	/* If we're only supposed to set certain bits, first get */
	/* the current value and OR it with the new value */
	if ( bitset_only )
	{
		oldval = ModemDSPRead1( addr );
		data |= oldval;
	}

	OSC_DISABLE_IRQ();

	/* Set up the new value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addr ) | MEMW );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addr ) );
	MDM_WRITE( MODEM_MEDAL, data );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	OSC_ENABLE_IRQ();
}


/*===========================================================================
*
*   ModemDSPRead2
*
*   Reads DSP RAM using Method 2 per SSF336 Designer's Guide
*
*   Input:  Ram address
*   Output: desired value
*   Note:   It works like two ModemDSPRead1()
*
*===========================================================================*/
UINT16 ModemDSPRead2( UINT16 addrH, UINT16 addrL )
{
	UINT16 retval;

	OSC_DISABLE_IRQ();

	/* Set up the LSB value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addrL ) );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addrL ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_LO_BYTE( retval, MDM_READ( MODEM_MEDAL ) );

	/* Set up the MSB value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addrH ) );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addrH ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_HI_BYTE( retval, MDM_READ( MODEM_MEDAL ) );

#ifdef DBG_DSP_PRINTF
	TRACE_MODEMDEBUG( "ModemDSPRead2() addr=0x%08X, data=0x%04X\n", addrH << 8 + addrL, retval );
#endif

	OSC_ENABLE_IRQ();

	return ( retval );
}

/*===========================================================================
*
*    ModemDSPWrite2
*
*   Updates DSP RAM using Method 2 per SSF336 Designer's Guide
*
*   Input:      Ram address
*               New Value
*   Output:     Modem DSP modified
*   Note:   It works like two ModemDSPWrite1()
*
*===========================================================================*/
void ModemDSPWrite2( UINT16 addrH, UINT16 addrL, UINT16 data )
{
#ifdef DBG_DSP_PRINTF
	TRACE_MODEMDEBUG( "ModemDSPWrite2() addr=0x%08X, data=0x%04X\n", addrH << 8 + addrL, data );
#endif

	OSC_DISABLE_IRQ();

	/* Set up the LSB value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addrL ) | MEMW );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addrL ) );
	MDM_WRITE( MODEM_MEDAL,  GET_LO_BYTE( data ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	/* Set up the MSB value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addrH ) | MEMW );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addrH ) );
	MDM_WRITE( MODEM_MEDAL,  GET_HI_BYTE( data ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	OSC_ENABLE_IRQ();
}


/*===========================================================================
*
*    ModemDSPWrite3
*
*   Updates DSP RAM using Method 3 per SSF336 Designer's Guide
*
*   Input:      Ram address
*               New Value
*   Output:     Modem DSP modified
*
*===========================================================================*/
void ModemDSPWrite3( UINT16 addr, UINT16 data )
{
	OSC_DISABLE_IRQ();

	/* Set up the new value */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addr ) | MEMW );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addr ) );
	MDM_WRITE( MODEM_MEDAL, GET_LO_BYTE( data ) );
	MDM_WRITE( MODEM_MEDAM, GET_HI_BYTE( data ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	OSC_ENABLE_IRQ();
}


/*===========================================================================
*
*    ModemDSPRead4
*
*   Reads DSP RAM using Method 4 per SSF336 Designer's Guide
*
*   Input:      Ram address
*   Output:     desired value
*
*===========================================================================*/
UINT16 ModemDSPRead4( UINT16 addr )
{
	UINT16 retval;

	OSC_DISABLE_IRQ();

	/* Set up for the read operation */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( addr ) );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( addr ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_HI_BYTE( retval, MDM_READ( MODEM_MEDAM ) );
	PUT_LO_BYTE( retval, MDM_READ( MODEM_MEDAL ) );

	OSC_ENABLE_IRQ();

	return ( retval );
}


/*-------------------------------------------------------------------
    Name: ModemDSPWrite5 - Updates DSP RAM using Method 5 per
                           SSF336 Designer's Guide.

    Input:  UINT16 DspAddr   - DSP RAM Address
            UINT16 Setbits   - Set bits want to change to 1
            UINT16 Resetbits - Set bits want to change to 0
    Output: Modem DSP modified
    Return: UINT16 DspData - Data read from DSP RAM Address
 *-------------------------------------------------------------------*/
UINT16 ModemDSPWrite5( UINT16 DspAddr, UINT16 Resetbits, UINT16 Setbits )
{
	UINT16 DspData;

	// Stop OS Task switching
	OSC_DISABLE_IRQ();

	/* Set DSP RAM address */
	MDM_WRITE( MODEM_MEADDH, GET_HI_BYTE( DspAddr ) );
	MDM_WRITE( MODEM_MEADDL, GET_LO_BYTE( DspAddr ) );

	/* Read DAP RAM data, Wait for it to complete */
	MDM_SETBIT( MODEM_1D, MEACC );

	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	/* Modify DAP RAM data, and write back */
	PUT_HI_BYTE( DspData, MDM_READ( MODEM_MEDAM ) );
	PUT_LO_BYTE( DspData, MDM_READ( MODEM_MEDAL ) );
	DspData &= ~Resetbits;
	DspData |= Setbits;
	MDM_WRITE( MODEM_MEDAM, GET_HI_BYTE( DspData ) );
	MDM_WRITE( MODEM_MEDAL, GET_LO_BYTE( DspData ) );

	/* Write new value to DAP RAM, Wait for it to complete */
	MDM_SETBIT( MODEM_1D, MEACC + MEMW );

	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	OSC_ENABLE_IRQ();

	return DspData;
}


/*===========================================================================
*
*    ModemDSPRead6
*
*   Reads DSP RAM using Method 6 per SSF336 Designer's Guide
*
*   Input:      Ram address
*   Output:     desired value
*
*===========================================================================*/
UINT16 ModemDSPRead6( UINT16 addrH, UINT16 addrL )
{
	UINT16 retval;

	OSC_DISABLE_IRQ();

	/* Set up for the read operation */
	MDM_WRITE( MODEM_1D, 0x41 );
	MDM_WRITE( MODEM_17, GET_HI_BYTE( addrL ) );
	MDM_WRITE( MODEM_16, GET_LO_BYTE( addrL ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_LO_BYTE( retval, MDM_READ( MODEM_MEDAL ) );

	/* Set up the MSB value */
	MDM_WRITE( MODEM_1D, 0x41 );
	MDM_WRITE( MODEM_17, GET_HI_BYTE( addrH ) );
	MDM_WRITE( MODEM_16, GET_LO_BYTE( addrH ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_HI_BYTE( retval, MDM_READ( MODEM_MEDAL ) );

#ifdef DBG_DSP_PRINTF
	TRACE_MODEMDEBUG( "ModemDSPRead6() addr=0x%08X, data=0x%04X\n", ( addrH << 16 ) + addrL, retval );
#endif

	OSC_ENABLE_IRQ();

	return ( retval );
}


/*===========================================================================
*
*    ModemDSPRead7
*
*   Reads DSP RAM using Method 7 per SSF336 Designer's Guide
*
*   Input:      Ram address
*   Output:     desired value
*
*===========================================================================*/
UINT32 ModemDSPRead7( UINT16 addrH, UINT16 addrL )
{
	UINT16 retvalH, retvalL;

	OSC_DISABLE_IRQ();

	/* Set up for the read operation */
	MDM_WRITE( MODEM_1D, 0x4A );
	MDM_WRITE( MODEM_MEDAM, GET_HI_BYTE( addrL ) );
	MDM_WRITE( MODEM_MEDAL, GET_LO_BYTE( addrL ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_HI_BYTE( retvalL, MDM_READ( MODEM_MEDAM ) );
	PUT_LO_BYTE( retvalL, MDM_READ( MODEM_MEDAL ) );

	/* Set up the MSB value */
	MDM_WRITE( MODEM_1D, 0x4A );
	MDM_WRITE( MODEM_MEDAM, GET_HI_BYTE( addrH ) );
	MDM_WRITE( MODEM_MEDAL, GET_LO_BYTE( addrH ) );
	MDM_SETBIT( MODEM_1D, MEACC );

	/* Wait for it to change */
	ModemBitOrBust( MODEM_1D, MEACC, 0 );

	PUT_HI_BYTE( retvalH, MDM_READ( MODEM_MEDAM ) );
	PUT_LO_BYTE( retvalH, MDM_READ( MODEM_MEDAL ) );

#ifdef DBG_DSP_PRINTF
	TRACE_MODEMDEBUG( "ModemDSPRead7() addr=0x%08X, data=0x%08X\n", ( addrH << 16 ) + addrL, ( retvalH << 16 ) + retvalL );
#endif

	OSC_ENABLE_IRQ();

	return ( ( retvalH << 16 ) + retvalL );
}

