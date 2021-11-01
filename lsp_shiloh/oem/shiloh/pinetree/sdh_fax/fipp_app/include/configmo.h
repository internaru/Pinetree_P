/*
 *  $RCSfile: configmo.h,v $
 * $Revision: 1.3 $
 *   $Author: miurat $
 *     $Date: 2010/04/22 20:29:01 $
 */
/************************************************************************
 *
 *      configmo.h
 *
 *  Copyright 2009 Conexant Systems, Inc.
 *  4000 MacArthur Blvd., Newport Beach, CA 92660 USA.
 *  All Rights Reserved Worldwide.
 *  Information Contained Herein is Proprietary to Conexant Systems, Inc.
 *
 ************************************************************************/

#ifndef __CONFIFMO_H_
#define __CONFIFMO_H_

#ifndef CONEXANT_MODEM
	#define CONEXANT_MODEM
#endif

#define SPI_MODEM_INTERFACE       // Use SPI interface to modem

#ifdef CONEXANT_MODEM

//	#define USE_CFS               // T.32 fax module
	#define MODEM_SSF336
	#define EVALUATION_BOARD
//	#define USE_WALLAROO_USART    // AT command test via UA2 on Wallaroo board
	#define MODEM_SPEED 12000000  // 12 MHz SPICLK
	#define UA1_SPEED   0


	#ifdef DC2250
		#define MODEM_CHANNEL_NO  0   // Use channel 0
		#define USART_CHANNEL_NO  2
	#else
		#define MODEM_CHANNEL_NO  2   // Use channel 2
		#define USART_CHANNEL_NO  0
	#endif

	#ifdef EVALUATION_BOARD

		#define STANDALONE_FAX_DVT    // set auto forwarding

		#ifdef USE_WALLAROO_USART
			#define UA2_SPEED 115200
		#else
			#define UA2_SPEED 0
		#endif

	#else
		#define UA2_SPEED   0
	#endif  //EVALUATION_BOARD


	#define USE_EXCLUSION_RELAY
    #define EX_RELAY_PORT 1       // SSF336 Port No. ( 0 - 5 ) used for Exclusion Relay Control
    #define EX_RELAY_MASK ( 1 << EX_RELAY_PORT )

#endif  //CONEXANT_MODEM

#endif //__CONFIFMO_H_
