/*
 *  $RCSfile: download.inc,v $
 * $Revision: 1.11 $
 *   $Author: miurat $
 *     $Date: 2012/08/21 22:15:57 $
 */
/************************************************************************
 *
 *      download.inc
 *
 *  Copyright 2004 - 2012 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#include "cpx_ram.inc"
#include "spx_ram.inc"


#define RAM_VERSION    1800

#define OFFSET_RAM1800 361
#define OFFSET_RAM1700 361
#define OFFSET_RAM1600 318
#define OFFSET_RAM1500 318
#define OFFSET_RAM1400 273
#define OFFSET_RAM1300 273
#define OFFSET_RAM1195 189

static BOOL SSFDownload( INT8 type );

typedef struct tagPRAMHeaderInfo
{
	UINT8 cModule[ 2 ];
	UINT8 cID[ 2 ];
	UINT8 cType[ 2 ];
	UINT8 cCRC;
	UINT8 cReserved;
	UINT8 cStart[ 4 ];
	UINT8 cEnd[ 4 ];
	UINT8 cLength[ 4 ];
	UINT8 cPosition[ 4 ];
	UINT8 cNext[ 4 ];
	UINT8 cName;
} PRAM_HEADER_INFO, *PPRAM_HEADER_INFO;

typedef struct tagDownloadPRAMInfo
{
	UINT8 cFileHead[ 4 ];
	UINT8 cVersion[ 2 ];
	UINT8 cNram[ 2 ];
	PRAM_HEADER_INFO cPRAMHeader;
} DOWNLOAD_PRAM_INFO, *PDOWNLOAD_PRAM_INFO;

typedef enum
{
	DL_NO_MODULE = 0,
	DL_FAX       = 1,
	DL_VOICE     = 2,
	DL_CPX       = 4,
        DL_SPX       = 8,
        DL_SLEEP     = 16
} DP_DOWNLOAD_STATE;


static const UINT8 CPX_Module_ID[] =
{
	0x4B, 0x4E, 0x44, 0x4A, 0x43, 0, // Download order for 15xx FAX module and 0 stop
	0x4B, 0x4E, 0x42, 0x4A, 0x43, 0, // Download order for 15xx Voice module and 0 stop
	0x4B, 0x4E, 0x44, 0x43, 0,       // Download order for 13xx - 14xx FAX module and 0 stop
	0x4B, 0x4E, 0x42, 0x43, 0,       // Download order for 13xx - 14xx VOICE module and 0 stop
	0x4B, 0x4E, 0x43, 0,             // Download order for 12xx or old and 0 stop
	0x44, 0,                         // Download FAX module
	0x42, 0,                         // Download VOICE module
	0x46, 0,                         // Download SLEEP module
};

static const UINT8 SPX_Module_ID[] =
{
	0x4C, 0x53, 0x52, 0,             // Download order for 1502 and 0 stop
	0x4C, 0x52, 0,                   // Download order and 0 stop
};

static PDOWNLOAD_PRAM_INFO pCPX_Code, pSPX_Code;


static UINT16 select_pram_code( UINT16 version )
{
	UINT16 offset;

	pCPX_Code = ( PDOWNLOAD_PRAM_INFO ) &cpx_ram;
	pSPX_Code = ( PDOWNLOAD_PRAM_INFO ) &spx_ram;

	switch ( version )
	{
		case 1800:

			offset = OFFSET_RAM1800;
			break;

		case 1700:

			offset = OFFSET_RAM1700;
			break;

		case 1600:

			offset = OFFSET_RAM1600;
			break;

		case 1500:

			offset = OFFSET_RAM1500;
			break;

		case 1400:

			offset = OFFSET_RAM1400;
			break;

		case 1300:

			offset = OFFSET_RAM1300;
			break;

		default:

			offset = OFFSET_RAM1195;
			break;

	}

	return offset;
}


UINT16 SSFGetCodeVersion( void )
{
	UINT8 *pDataOffset;
	UINT16 offset;

	offset = select_pram_code( RAM_VERSION );

	pDataOffset = (UINT8 *) pCPX_Code + offset;
	pDataOffset += 43;

	return ( *pDataOffset << 8 ) + *( pDataOffset + 1 );
}


void SSFShowCodeVersion( void )
{
	UINT8 *pDataOffset;
	UINT8  cBuff[ 40 ];
	UINT16 offset;
	int i;

	offset =  select_pram_code( RAM_VERSION );

	pDataOffset = (UINT8 *) pCPX_Code + offset;

	for ( i = 0; i < 24; i++ )
	{
		cBuff[ i ] = *( pDataOffset + i );
	}

	cBuff[ i++ ] = ' ';
	cBuff[ i++ ] = 'V';
	cBuff[ i++ ] = 'e';
	cBuff[ i++ ] = 'r';
	cBuff[ i++ ] = ' ';

	pDataOffset += 43;

	if ( *pDataOffset > 9 )
	{
		cBuff[ i++ ] = ( *pDataOffset >> 4 ) + 0x30;
		cBuff[ i++ ] = ( *pDataOffset & 0x0F ) + 0x30;
	}
	else
		cBuff[ i++ ] = *pDataOffset + 0x30;

	cBuff[ i++ ] = '.';

	pDataOffset += 1;

	if ( *pDataOffset > 9 )
	{
		cBuff[ i++ ] = ( *pDataOffset >> 4 ) + 0x30;
		cBuff[ i++ ] = ( *pDataOffset & 0x0F ) + 0x30;
	}
	else
		cBuff[ i++ ] = *pDataOffset + 0x30;

	cBuff[ i++ ] = 0;

	TRACE_MODEM( "%s\n", cBuff );
}


static BOOL SSFDownload( INT8 type )
{
	PPRAM_HEADER_INFO    pPRAM_Header;
	UINT8  *pDataOffset;
	UINT16 wNumberOfModule, wCodeLength, wNextHeaderOffset;
	int    i, _id;
	UINT8  cData, cCRC;
	UINT16 version;

//	TRACE_MODEMDEBUG( " -> Enter download\n" );
	
	version = SSFGetCodeVersion();

	if ( type & ( DL_CPX + DL_VOICE + DL_FAX + DL_SLEEP ) )
	{
		/***************************************/
		/* Host Request Download CPX RAM code */
		/***************************************/

		wNumberOfModule = ( pCPX_Code->cNram[ 1 ] * 0x100 ) + pCPX_Code->cNram[ 0 ];

		if ( version < 0x1300 )
		{
			_id = 22;
		}
		else if ( ( type & ( DL_CPX + DL_FAX ) ) == ( DL_CPX + DL_FAX ) )
		{
			if ( version >= 0x1500 )
				_id = 0;
			else
				_id = 12;
		}
		else if ( ( type & ( DL_CPX + DL_VOICE ) ) == ( DL_CPX + DL_VOICE ) )
		{
			if ( version >= 0x1500 )
			   _id = 6;
			else
			   _id = 17;
		}
		else if ( ( type & DL_VOICE ) == DL_VOICE )
		{
			_id = 26;
		}
		else if ( ( type & DL_FAX ) == DL_FAX )
		{
			_id = 28;
		}
		else if ( ( type & DL_SLEEP ) == DL_SLEEP )
		{
			if ( version >= 0x1700 )
				_id = 30;
			else
				return FALSE;
		}
		else
		{
			_id = 0;
		}

		while ( ( cData = CPX_Module_ID[ _id++ ] ) != 0 )
		{
			pPRAM_Header = ( PPRAM_HEADER_INFO ) &pCPX_Code->cPRAMHeader;

			for ( i = 0; ( i < wNumberOfModule ) && ( pPRAM_Header->cModule[ 0 ] != cData ); i++ )
			{
				wNextHeaderOffset = ( pPRAM_Header->cNext[ 1 ] * 0x100 ) + pPRAM_Header->cNext[ 0 ];
				pPRAM_Header = ( PPRAM_HEADER_INFO ) ( ( UINT8 * ) pCPX_Code + wNextHeaderOffset );
			}

//			TRACE_MODEMDEBUG( "DL : CPX Requested ID is 0x%02X, Module ID is 0x%02X\n", cData, pPRAM_Header->cModule[ 0 ] );

			pDataOffset = ( UINT8 * ) pCPX_Code + ( pPRAM_Header->cPosition[ 1 ] * 0x100 ) + pPRAM_Header->cPosition[ 0 ];
			wCodeLength = ( pPRAM_Header->cLength[ 1 ] * 0x100 ) + pPRAM_Header->cLength[ 0 ];    // CPX is byte size

			cData = 0x4F;

			MDM_WRITE( MODEM_ABCODE, cData );

			i = 10;

			while ( MDM_READ( MODEM_1B ) != cData )
			{
                                OSC_Sleep( 10 );

				if ( --i == 0 )   // 100 ms
				{
					TRACE_ERROR( "CPX : Timeout Error\n" );
					return FALSE;
				}
			}

			MDM_WRITE( MODEM_RXHSKSTAT, pPRAM_Header->cStart[ 0 ] );

			MDM_WRITE( MODEM_TXHSKSTAT, pPRAM_Header->cStart[ 1 ] );

//			TRACE_MODEMDEBUG( "CPX : Set RXHSKSTAT 0x%02X, TXHSKSTAT 0x%02X\n",
//			             pPRAM_Header->cStart[ 0 ], pPRAM_Header->cStart[ 1 ] );

			while ( wCodeLength > 0 )
			{
				if ( MDM_BITSET( MODEM_0D, TXFNF ) )
				{
					MDM_WRITE( MODEM_TBUFF, *pDataOffset );
					pDataOffset++;
					wCodeLength--;
				}
			}

//			TRACE_MODEMDEBUG( "CPX : end writing\n" );

                        OSC_Sleep( 10 );

			MDM_WRITE( MODEM_ABCODE, 0x00 );

			i = 10;

			while ( MDM_READ( MODEM_TXHSKSTAT ) != 0xFF )
			{
                                OSC_Sleep( 10 );

				if ( --i == 0 )   // 100 ms
				{
					TRACE_ERROR( "CPX : Timeout Error\n" );
					return FALSE;
				}
			}

			cCRC = MDM_READ( MODEM_RXHSKSTAT );    // Read CRC

			if ( cCRC == pPRAM_Header->cCRC )
			{
//				TRACE_MODEMDEBUG( "CPX : *** CRC is OK! File is 0x%02X : 0x%02X ***\n", pPRAM_Header->cCRC, cCRC );
			}
			else
			{
				TRACE_ERROR( "CPX : ### CRC Error!! File is 0x%02X : 0x%02X ###\n", pPRAM_Header->cCRC, cCRC );
				return FALSE;
			}
                        OSC_Sleep( 10 );
		}

//                TRACE_MODEMDEBUG( "CPX : *** Finished download, 0x%02X ***\n", MDM_READ( MODEM_ABCODE ) );

	}  // end of CPX


	if ( type & DL_SPX )
	{
		/**************************************/
		/* Host Request Download SPX RAM code */
		/**************************************/

		wNumberOfModule = ( pSPX_Code->cNram[ 1 ] * 0x100 ) + pSPX_Code->cNram[ 0 ];

		if ( version >= 0x1502 )
			_id = 0;
		else
			_id = 4;

		while ( ( cData = SPX_Module_ID[ _id++ ] ) != 0 )
		{
			pPRAM_Header = ( PPRAM_HEADER_INFO ) &pSPX_Code->cPRAMHeader;

			for ( i = 0; ( i < wNumberOfModule ) && ( pPRAM_Header->cModule[ 0 ] != cData ); i++ )
			{
				wNextHeaderOffset = ( pPRAM_Header->cNext[ 1 ] * 0x100 ) + pPRAM_Header->cNext[ 0 ];
				pPRAM_Header = ( PPRAM_HEADER_INFO ) ( ( UINT8 * ) pSPX_Code + wNextHeaderOffset );
			}

//			TRACE_MODEMDEBUG( "SPX : Requested ID is 0x%02X, Module ID is 0x%02X\n", cData, pPRAM_Header->cModule[ 0 ] );

			pDataOffset = ( UINT8 * ) pSPX_Code + ( pPRAM_Header->cPosition[ 1 ] * 0x100 ) + pPRAM_Header->cPosition[ 0 ];
			wCodeLength = ( ( pPRAM_Header->cLength[ 1 ] * 0x100 ) + pPRAM_Header->cLength[ 0 ] ) * 4;

			cData = 0x5F;

			MDM_WRITE( MODEM_ABCODE, cData );

			i = 10;

			while ( MDM_READ( MODEM_1B ) != cData )
			{
                                OSC_Sleep( 10 );

				if ( --i == 0 )   // 100 ms
				{
					TRACE_ERROR( "SPX : Timeout Error\n" );
					return FALSE;
				}
			}

			MDM_WRITE( MODEM_RXHSKSTAT, pPRAM_Header->cStart[ 0 ] );

			MDM_WRITE( MODEM_TXHSKSTAT, pPRAM_Header->cStart[ 1 ] );

			while ( wCodeLength > 0 )
			{
				if ( MDM_BITSET( MODEM_0D, TXFNF ) )
				{
					MDM_WRITE( MODEM_TBUFF, *pDataOffset );

					pDataOffset++;
					wCodeLength--;
                                }
			}

//			TRACE_MODEMDEBUG( "SPX : end writing\n" );

                        OSC_Sleep( 10 );

			MDM_WRITE( MODEM_ABCODE, 0x00 );

			i = 10;

			while ( MDM_READ( MODEM_TXHSKSTAT ) != 0xFF )
			{
                                OSC_Sleep( 10 );

				if ( --i == 0 )   // 100 ms
				{
					TRACE_ERROR( "SPX : timeout 3 ###\n" );
					return FALSE;
				}
			}

			cCRC = MDM_READ( MODEM_RXHSKSTAT );    // Read CRC

			if ( cCRC == pPRAM_Header->cCRC )
			{
//				TRACE_MODEMDEBUG( "SPX : *** CRC is OK! File is 0x%02X : 0x%02X ***\n", pPRAM_Header->cCRC, cCRC );
			}
			else
			{
				TRACE_ERROR( "SPX : ### CRC Error!! File is 0x%02X : 0x%02X ###\n", pPRAM_Header->cCRC, cCRC );
				return FALSE;
			}
                        OSC_Sleep( 50 );
		}

//		TRACE_MODEMDEBUG( "SPX : *** Finished, download, 0x%02X ***\n", MDM_READ( MODEM_ABCODE ) );

	}  // end of SPX        
	
//	TRACE_MODEMDEBUG( " <- Exit download\n" );

	return TRUE;
}

