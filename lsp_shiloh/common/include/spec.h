/********************************************************************
*	Copyright (c) 2013 SINDOH.Crp.  All right reserved.
*********************************************************************/
#ifndef		SPEC_H
#define		SPEC_H

#undef EXTERN
#ifdef SPEC_H_MAIN
#define	EXTERN
#else	//SPEC_H_MAIN
#define	EXTERN	extern
#endif	//SPEC_H_MAIN

EXTERN	int gMachineType;
EXTERN int gScannerAttached;
EXTERN int gFaxAttached;
EXTERN int gTFTLCDAttached;

#define		MTYPE_KARAS			0
#define		MTYPE_PINETREE		1
#define		MTYPE_PINETREES		2
#define		MTYPE_ABBA2H		3
#define		MTYPE_OAK2M			4
#define		MTYPE_OAK2H			5
#define		MTYPE_JUNIPERC		6
#define		MTYPE_JUNIPERS		7
#define		MTYPE_PINETREEL		8

#define KARAS_2USB 0
#define ABBA2 0	// must be 0 (1->Abba2, 0->Kara-S,Pinetree/-S)
#define PINETREE_L 0 // 1->Pinetree-L, 0->Pinetree/-S
#define MAC_TYPE_M 0 // 1->Pinetree(40ppm),Abba2-M(32ppm), 0->Pinetree-S,Kara-S(47ppm),Abba2-H(36ppm)
#define MAC_TYPE_L 0 // must be 0

#define ZX_ENABLE 0 // zerocrossing
#define BLI_TEST 0 // BLI test
#define AUTO_REBOOT_ON_SWSC 0 // execute reboot automatically when SW SC such as 910 occurs 

#define		IS_KARAS		((gMachineType==MTYPE_KARAS)?1:0)
#define		IS_PINETREE	((gMachineType==MTYPE_PINETREE)?1:0)
#define		IS_PINETREES	((gMachineType==MTYPE_PINETREES)?1:0)
#define		IS_ABBA2H		((gMachineType==MTYPE_ABBA2H)?1:0)
#define		IS_OAK2M		((gMachineType==MTYPE_OAK2M)?1:0)
#define		IS_OAK2H		((gMachineType==MTYPE_OAK2H)?1:0)
#define		IS_JUNIPERC		((gMachineType==MTYPE_JUNIPERC)?1:0)
#define		IS_JUNIPERS		((gMachineType==MTYPE_JUNIPERS)?1:0)
#define		IS_PINETREEL	((gMachineType==MTYPE_PINETREEL)?1:0)

#define		IS_SCANNER_ATTACHED	(gScannerAttached)
#define		IS_FAX_ATTACHED	(gFaxAttached)
#define		IS_TFTLCD_ATTACHED	(gTFTLCDAttached)

#define		IS_MFP (IS_PINETREE|IS_PINETREES|IS_PINETREEL|IS_OAK2M|IS_OAK2H|IS_JUNIPERC|IS_JUNIPERS)
#define		IS_A3DEVICE (IS_JUNIPERC|IS_JUNIPERS)

EXTERN	void	machine_type_init();

EXTERN int gPaperType;
#define     A4_BASE    0
#define     LT_BASE    1

EXTERN int gCountry;
enum {
	IS_UNKNOWN,
	IS_KOREA,
	IS_CHINA,
	IS_NORTH_AMERICA,
	IS_BRAZIL,
	IS_TURKEY,
	IS_SOUTH_AFRICA,
	IS_TUNISIA,
	IS_ARAB_EMIRATES,
	IS_IRAN,
	IS_RUSSIA,
	IS_INDIA,
	IS_MALAYSIA,
	IS_SAUDI_ARABIA,
	IS_AUSTRALIA,
	IS_ARGENTINA,
	IS_COLOMBIA,
	IS_SPAIN,
	IS_PARAGUAY,
	IS_MEXICO,
	IS_PERU,
	IS_CHILE,
	IS_ABROAD,
	IS_RUSSIA_OEM,
};

#if PINETREE_L
#define MAX_TRAY 2  // except MPT
#else
#define MAX_TRAY 3  // except MPT
#endif

#endif /* SPEC_H */
