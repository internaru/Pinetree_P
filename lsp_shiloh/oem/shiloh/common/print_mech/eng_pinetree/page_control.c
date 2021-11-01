/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	page_control.c					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

#include "pioname.h"
#include "farch.h"
#include "em.h"
#include <stdio.h>

// #include "te_api.h"

#define DBG_PRFX "engpc: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */


extern Uint32	Get_MeasuredMediaSize(Uint8);
extern Uint32	Get_MeasuredPaperPath(Uint8 Source, Uint32 T1_Value);

extern Uint32	MeasuredRealPaperLength;
extern Uint32	PaperLength[2][26];
extern Uint8	MediaSize[NumberOfBoundary + 2];
extern Uint32	HeartBeatPaperLength[2][NumberOfBoundary];
extern Uint8 PinsIsUsedByOtherPageSupervisor;
extern Uint8   bNarrowMedia;

extern Uint32	SavedPaperPathT1ToPins;
extern Uint32	SavedPaperPathT2ToPins;
extern Uint32	SavedPaperPathT3ToPins;
extern Uint32	SavedPaperPathMPFToPins;
extern Uint32 NarrowBoundary;
extern Uint8	PSTicks_Gap32mm;
extern Uint8	PSTicks_Gap53mm;		// 14Aug09 : CHG 1 (new)

extern Uint8	SavedSourceInPrePage;
extern Uint8 	machineType;

extern STRUCT_TRAY_STATUS Tray_Status;

extern Uint8 fMotorSpeedMode;

Uint8	Get_MeasuredPaperLength(Uint32 RealPaperLength, PageSupervisor *PGSPVSR);
Uint32	Set_MeasuredMediaSize(Uint8 MediaSource, Uint8 MediaSize);
/*VerADP Uint32	Get_PaperPathLengthMinusGap(Uint8 Source, Uint8 PreSource, Uint8 PrePreSource);*/
int	Get_PaperPathLengthMinusGap(PageSupervisor *PGSPVSR, Uint8 PrePreSource);
Uint32	Get_WaitTimeForNextPickup(PageSupervisor *PGSPVSR);
Uint32	Get_WaitTimeForNarrowMedia (PageSupervisor *PGSPVSR);

Uint32	SavedRealPaperLength;
Uint32	SavedRealMediaSizeInMPF;
Uint32	SavedRealMediaSizeInTray1;
Uint32	SavedRealMediaSizeInTray2;
Uint32	SavedRealMediaSizeInTray3;
Uint32	SavedRealMediaSizeInDuplex;

Uint32	SavedPaperLength;
Uint32	SavedPaperPath;
Uint32	SavedPaperLengthInDuplex;
Uint32	SavedPaperPathInDuplex;

Uint32	NarrowMediaCheck;
Uint32	PaperLengthForJamCheck;

Uint16	MeasuredPaperLength;
Uint16	MeasuredPaperLengthInTray1;
Uint16	MeasuredPaperLengthInTray2;
Uint16	MeasuredPaperLengthInTray3;

Uint8 MPF_Gap[NUM_THROUGHPUTS] = {
         MPF_Gap_0,
         MPF_Gap_1,
         MPF_Gap_2,
         MPF_Gap_3
         };

Uint8 Tray_NormalGap[NUM_THROUGHPUTS] = {
         Tray_NormalGap_0,
         Tray_NormalGap_1,
         Tray_NormalGap_2,
         Tray_NormalGap_3
         };

Uint8 Tray_LabelGap[NUM_THROUGHPUTS] = {
         Tray_LabelGap_0,
         Tray_LabelGap_1,
         Tray_LabelGap_2,
         Tray_LabelGap_3
         };

Uint32	Tray1FeedTime;
Uint32	Tray2FeedTime;
Uint32	Tray3FeedTime;
Uint8	SecondPage;

Uint8	ThroughputRate;
Uint32 	InterpageGap=0;

Uint32	DetectStartPaperToPins(PageSupervisor *PGSPVSR)
{

	Uint32 SourcePaperLength, PIP_PaperLength, 
		LeftPaperLength, PaperPath, RealPaperLength;

	PinsIsUsedByOtherPageSupervisor = TRUE;
	
	SourcePaperLength = Get_MeasuredMediaSize(PGSPVSR->Source);
	PaperLengthForJamCheck = SourcePaperLength + JAM_MARGIN;
	PIP_PaperLength = PGSPVSR->Length;
	
	MSG("### SourcePaperLength=%d, PIP_PaperLength=%d ###\n",SourcePaperLength, PIP_PaperLength);
	/* RIP Data & Source Data */
	if(SourcePaperLength < PIP_PaperLength)
		RealPaperLength = PIP_PaperLength;
	else
		RealPaperLength = SourcePaperLength;
	
	PaperPath = Get_MeasuredPaperPath(PGSPVSR->Source, PGSPVSR->T1_Value);
#if	ENG_TRACE
	MSG("(Eng)## PaperPath = %d ##\n", PaperPath);
#endif	
	if(PGSPVSR->Source == MS_Duplex) 
	{
		SavedPaperLengthInDuplex = RealPaperLength;
		SavedPaperPathInDuplex = PaperPath;
	}
	else 
	{
		SavedPaperLength = RealPaperLength;
		SavedPaperPath = PaperPath;
	}
	
	if(RealPaperLength > PaperPath)
		LeftPaperLength = RealPaperLength - PaperPath;
	else
		LeftPaperLength = PSTicks_Minimum;
		
	return LeftPaperLength;
}
	
Uint32	DetectEndPaperToPins(PageSupervisor *PGSPVSR)
{
	Uint8	RealPaperSize;
	Uint32	MeasuredMediaSize;
	
	RealPaperSize = Get_MeasuredPaperLength(PGSPVSR->T1_Value, PGSPVSR);
	
	if(((GetTrayMediaInfo(Tray_Status.Tray1.newTrayStatus) == TRAY_MEDIA_UNKNOWN) && (PGSPVSR->Source == MS_Tray1)) 
	|| ((GetTrayMediaInfo(Tray_Status.Tray2.newTrayStatus) == TRAY_MEDIA_UNKNOWN) && (PGSPVSR->Source == MS_Tray2))
	|| ((GetTrayMediaInfo(Tray_Status.Tray3.newTrayStatus) == TRAY_MEDIA_UNKNOWN) && (PGSPVSR->Source == MS_Tray3))
	|| (PGSPVSR->Source == MS_MultiPurposeTray))
	{
		MeasuredMediaSize = Set_MeasuredMediaSize(PGSPVSR->Source, RealPaperSize);
		MeasuredPaperLength = (Uint16)MeasuredMediaSize;
		NarrowMediaCheck = MeasuredPaperLength;
#if	ENG_TRACE	
		MSG("(Eng)1_RealPaperSize = %d, MeasuredPaperLength = %d, PGSPVSR->T1_Value= %d \n",RealPaperSize,MeasuredPaperLength,PGSPVSR->T1_Value);
#endif	
	}
	else
	{
		switch(PGSPVSR->Source)
		{
			case MS_Tray1:
				MeasuredPaperLength = MeasuredPaperLengthInTray1;
				NarrowMediaCheck = MeasuredPaperLength;
				break;
			case MS_Tray2:
				MeasuredPaperLength = MeasuredPaperLengthInTray2;
				NarrowMediaCheck = MeasuredPaperLength;
				break;
			case MS_Tray3:
				MeasuredPaperLength = MeasuredPaperLengthInTray3;
				NarrowMediaCheck = MeasuredPaperLength;
				break;
			case MS_Duplex:
				MeasuredPaperLength = Get_MeasuredMediaSize(MS_Duplex);
				break;
		}
		MSG("(Eng)2_RealPaperSize = %d, MeasuredPaperLength = %d, PGSPVSR->T1_Value= %d \n",RealPaperSize,MeasuredPaperLength,PGSPVSR->T1_Value);
	}

//	return MeasuredMediaSize;
	return MeasuredPaperLength;
}
		
Uint8	Get_MeasuredPaperLength(Uint32 TempRealPaperLength, PageSupervisor *PGSPVSR)
{
	Uint8 i;
	Uint32 *Ref_Len;
	
	//Chattering time add (6 => 10 => 8: adjust paper length)
	//~ TempRealPaperLength = TempRealPaperLength - CHATTERING_TIME_ADJ;
	
	if(TempRealPaperLength) 
	{
		if(fMotorSpeedMode == MTR_MODE_QUIET)
		{
#if ABBA2
			if(TempRealPaperLength > PSTicks_PinsActuate_L) 
				TempRealPaperLength = TempRealPaperLength - PSTicks_PinsActuate_L + 3/* - CHATTERING_TIME*/;
#else
			if(TempRealPaperLength > PSTicks_PinsActuate_L) 
				TempRealPaperLength = TempRealPaperLength - PSTicks_PinsActuate_L + 3/* - CHATTERING_TIME*/;
#endif
		}
		else
		{
			if(TempRealPaperLength > PSTicks_PinsActuate) 
				TempRealPaperLength = TempRealPaperLength - PSTicks_PinsActuate /* - CHATTERING_TIME*/;
		}
		SavedRealPaperLength = TempRealPaperLength;
		Ref_Len = (Uint32 *)&HeartBeatPaperLength[fMotorSpeedMode][0];
		for (i = 0; i < NumberOfBoundary; i++)
		{
			if (SavedRealPaperLength < (Uint32)*Ref_Len++)
			{
//				if (i == 0)
				break ;
			}
		}
#if	ENG_TRACE		
		MSG("(Eng)MediaSize[%d] = %d \n",i,MediaSize[i]);
#endif
		
#if MAC_TYPE_MH
		if(i >= 14)
		{
			PGSPVSR->fFullStackCheckSize = TRUE;
#if	ENG_TRACE			
			MSG("(Eng)## FULL STACK CHECK PAPER ## \n");
#endif			
		}
		else
		{
			PGSPVSR->fFullStackCheckSize = FALSE;
		}
#endif
		return (MediaSize[i]) ;
	}
	
	SavedRealPaperLength = TempRealPaperLength;
	return MS_NotDefine;
}

Uint32	Set_MeasuredMediaSize(Uint8 MediaSource, Uint8 MediaSize)
{
	switch(MediaSource) {
		case MS_MultiPurposeTray:
			SavedRealMediaSizeInMPF = MediaSize;
			break;
		case MS_Tray1:
			SavedRealMediaSizeInTray1 = MediaSize;
			break;
		case MS_Tray2:
			SavedRealMediaSizeInTray2 = MediaSize;
			break;
		case MS_Tray3:
			SavedRealMediaSizeInTray3 = MediaSize;
			break;
		case MS_Duplex:
			SavedRealMediaSizeInDuplex = MediaSize;
			break;
		default:
			break;
	}
	
	return PaperLength[fMotorSpeedMode][MediaSize];
}

Uint32	Get_WaitTimeForNextPickup(PageSupervisor *PGSPVSR)
{
	Uint32	MovingLength, RIPPaperLength, WaitTimeOfNextPaperPickup;

	if(PGSPVSR->PageAhead == NULL)
		return PSTicks_Minimum;

	MovingLength = PGSPVSR->PageAhead->T1_Value + SavedRealPaperLength 
			+ Get_PaperPathLengthMinusGap(PGSPVSR, SavedSourceInPrePage); /* VerADP */
			
	RIPPaperLength = PGSPVSR->PageAhead->Length;
	
	if (MovingLength >= RIPPaperLength) {
		WaitTimeOfNextPaperPickup = PSTicks_Minimum;
		return PSTicks_Minimum;
	}

	RIPPaperLength -= MovingLength;
	WaitTimeOfNextPaperPickup = RIPPaperLength;

	return WaitTimeOfNextPaperPickup;
}

int	Get_PaperPathLengthMinusGap(PageSupervisor *PGSPVSR, Uint8 PrePreSource)
{
	Uint32	temp_media_size;
	Uint32	legal_media_size;

	temp_media_size = Get_MeasuredMediaSize(PGSPVSR->PageAhead->Source);
	if(fMotorSpeedMode == MTR_MODE_NORMAL)
		legal_media_size = (Uint32)(PL_Legal /*PL_A4*/ * Modifier_HeartBeat * MMTimerPerPaperSpeed);
	else
		legal_media_size = (Uint32)(PL_Legal /*PL_A4*/ * Modifier_HeartBeat * MMTimerPerPaperSpeed_L);
	
	if(machineType == ABBA_OAK_H)
	{
		if(fMotorSpeedMode == MTR_MODE_NORMAL)
		{
			if(SecondPage == SET) 		InterpageGap = InterpageGap_H_Plus70;
			else 						InterpageGap = InterpageGap_H;
		}
		else
		{
			if(SecondPage == SET)		InterpageGap = InterpageGap_H_Plus70_QUIET;
			else 						InterpageGap = InterpageGap_H_QUIET;
		}

		if(temp_media_size == legal_media_size)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Legal_MH;
			else
				InterpageGap = InterpageGap_Legal_MH_QUIET;
		}
		else if(temp_media_size < NarrowBoundary)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Narrow_H;
			else
				InterpageGap = InterpageGap_Narrow_H_QUIET;
		}
		if(PGSPVSR->MediaDescription.BIT.Weight == MW_Heavy)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Legal_MH;
			else
				InterpageGap = InterpageGap_Legal_MH_QUIET;
		}
		if(PGSPVSR->PageAhead->Source == MS_Duplex)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_H_Duplex;
			else
				InterpageGap = InterpageGap_H_Duplex_QUIET;
		}
		if(PGSPVSR->cheat_gap == 1)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_M_SAGI;
			else
				InterpageGap = InterpageGap_M_SAGI_QUIET;
		}
	}
	else if(machineType == ABBA_OAK_M)
	{
		if(fMotorSpeedMode == MTR_MODE_NORMAL)
		{
			if(SecondPage == SET) 	InterpageGap = InterpageGap_M_Plus70;
			else 					InterpageGap = InterpageGap_M;
		}
		else
		{
			if(SecondPage == SET) 	InterpageGap = InterpageGap_M_Plus70_QUIET;
			else 					InterpageGap = InterpageGap_M_QUIET;
		}

		if(temp_media_size == legal_media_size)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Legal_MH;
			else
				InterpageGap = InterpageGap_Legal_MH_QUIET;
		}
		else if(temp_media_size < NarrowBoundary)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Narrow_M;
			else
				InterpageGap = InterpageGap_Narrow_M_QUIET;
		}
		if(PGSPVSR->MediaDescription.BIT.Weight == MW_Heavy)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Legal_MH;
			else
				InterpageGap = InterpageGap_Legal_MH_QUIET;
		}
		if(PGSPVSR->PageAhead->Source == MS_Duplex)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_M_Duplex;
			else
				InterpageGap = InterpageGap_M_Duplex_QUIET;
		}
		if(PGSPVSR->cheat_gap == 1)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_M_SAGI;
			else
				InterpageGap = InterpageGap_M_SAGI_QUIET;
		}
	}
	else
	{
		if(0/*fInterpageGapExportL == TRUE*/)
		{
			InterpageGap = InterpageGap_Export_L;
		}
		else
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_L;
			else
				InterpageGap = InterpageGap_L_QUIET;
		}
		
		if(temp_media_size == legal_media_size)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Legal_MH;
			else
				InterpageGap = InterpageGap_Legal_MH_QUIET;
		}
		else if(temp_media_size < NarrowBoundary)
		{
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				InterpageGap = InterpageGap_Narrow_L;
			else
				InterpageGap = InterpageGap_Narrow_L_QUIET;
		}
		if(PGSPVSR->PageAhead->Source == MS_Duplex)
		{
			// InterpageGap = InterpageGap_MH_Duplex;
		}
	}
	
#if	ENG_TRACE	
	MSG("(Eng)## InterpageGap = %d, temp_media_size = %d ##\n",InterpageGap,temp_media_size);
#endif	
	switch(PGSPVSR->Source) {
		case MS_MultiPurposeTray:

#if REGAL_UPDATE
			if(PGSPVSR->PageAhead->Source == MS_Duplex) {
				if(PGSPVSR->Source == PrePreSource) {
				}
				else {
					/* Switch Source */
					SavedRealMediaSizeInMPF = MS_Unknown;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
					else
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;
				}
			}
			if(PGSPVSR->Source == PGSPVSR->PageAhead->Source) {
			}
			else {
				/* Switch Source */
				SavedRealMediaSizeInMPF = MS_Unknown;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
				else
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;
			}
#endif
			if(fMotorSpeedMode == MTR_MODE_NORMAL)
				return PSTicks_MPFToPins - InterpageGap;
			else
				return PSTicks_MPFToPins_L - InterpageGap;
			break;
		case MS_Tray1:
			if(PGSPVSR->PageAhead->Source == MS_Duplex) {
				if(PGSPVSR->Source == PrePreSource) {
					if(SecondPage) {
						/* After Main Motor Start, First Page */
						SecondPage = CLEAR;
						Tray1FeedTime = SavedPaperPathT1ToPins;
#if	ENG_TRACE				
						MSG("(Eng)Path1_SavedPaperPathT1ToPins = %d,  InterpageGap = %d\n", SavedPaperPathT1ToPins,InterpageGap);
#endif
						return SavedPaperPathT1ToPins - InterpageGap;
					}
					/* Continuous Print in Same Source */
#if	ENG_TRACE				
						MSG("(Eng)Path2_SavedPaperPathT1ToPins = %d,  Tray1FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT1ToPins,Tray1FeedTime,InterpageGap);
#endif
					if(SavedPaperPathT1ToPins <= Tray1FeedTime) {
						Tray1FeedTime = SavedPaperPathT1ToPins;
					}
					else {
						Tray1FeedTime = Tray1FeedTime + 1;
					}
					return Tray1FeedTime - InterpageGap;

				}
				else {
					/* Switch Source */
#if REGAL_UPDATE
					SavedRealMediaSizeInTray1 = MS_Unknown;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
					else
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;
#endif
					Tray1FeedTime = SavedPaperPathT1ToPins;
					
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
					{
#if	ENG_TRACE				
						MSG("(Eng)Path3_SavedPaperPathT1ToPins = %d,  PSTicks_T1ToPins = %d, InterpageGap = %d\n", SavedPaperPathT1ToPins,PSTicks_T1ToPins,InterpageGap);
#endif
						return PSTicks_T1ToPins - InterpageGap;
					}
					else
					{
#if	ENG_TRACE				
						MSG("(Eng)Path3_SavedPaperPathT1ToPins = %d,  PSTicks_T1ToPins_L = %d, InterpageGap = %d\n", SavedPaperPathT1ToPins,PSTicks_T1ToPins_L,InterpageGap);
#endif
						return PSTicks_T1ToPins_L - InterpageGap;
					}
				}
			}
			if(PGSPVSR->Source == PGSPVSR->PageAhead->Source) {
				if(SecondPage) {
					/* After Main Motor Start, First Page */
					SecondPage = CLEAR;
					Tray1FeedTime = SavedPaperPathT1ToPins;
#if	ENG_TRACE				
				MSG("(Eng)Path4_SavedPaperPathT1ToPins = %d,  Tray1FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT1ToPins,Tray1FeedTime,InterpageGap);
#endif
					return SavedPaperPathT1ToPins - InterpageGap;
				}
				/* Continuous Print in Same Source */ 
				if(SavedPaperPathT1ToPins <= Tray1FeedTime) {
					Tray1FeedTime = SavedPaperPathT1ToPins;
				}
				else {
					Tray1FeedTime = Tray1FeedTime + 1;
				}
#if	ENG_TRACE				
				MSG("(Eng)Path5_SavedPaperPathT1ToPins = %d,  Tray1FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT1ToPins,Tray1FeedTime,InterpageGap);
#endif				
				return Tray1FeedTime - InterpageGap;

			}
			else {
				/* Switch Source */
#if REGAL_UPDATE
				SavedRealMediaSizeInTray1 = MS_Unknown;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
				else
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;
#endif
				Tray1FeedTime = SavedPaperPathT1ToPins;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					return PSTicks_T1ToPins - InterpageGap;
				else
					return PSTicks_T1ToPins_L - InterpageGap;
			}
			break;

		case MS_Tray2:
			if(PGSPVSR->PageAhead->Source == MS_Duplex) {
				if(PGSPVSR->Source == PrePreSource) {
					if(SecondPage) {
						/* After Main Motor Start, First Page */
						SecondPage = CLEAR;
						Tray2FeedTime = SavedPaperPathT2ToPins;
						MSG("(eng)-Tray2-Path1_SavedPaperPathT2ToPins = %d,  InterpageGap = %d\n", SavedPaperPathT2ToPins,InterpageGap);
						return SavedPaperPathT2ToPins - InterpageGap;
					}
					
					MSG("(eng)-Tray2-Path2_SavedPaperPathT2ToPins = %d,  Tray2FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT2ToPins,Tray2FeedTime,InterpageGap);
					
					/* Continuous Print in Same Source */ 
					if(SavedPaperPathT2ToPins <= Tray2FeedTime) {
						Tray2FeedTime = SavedPaperPathT2ToPins;
					}
					else {
						Tray2FeedTime = Tray2FeedTime + 1;
					}
					
					return Tray2FeedTime - InterpageGap;
				}
				else {
					/* Switch Source */
					
					SavedRealMediaSizeInTray2 = MS_Unknown;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
					else
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;

					Tray2FeedTime = SavedPaperPathT2ToPins;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
					{
						MSG("(Eng)-Tray2-Path3_SavedPaperPathT2ToPins = %d,  PSTicks_T2ToPins = %d, InterpageGap = %d\n", SavedPaperPathT2ToPins,PSTicks_T2ToPins,InterpageGap);
						return PSTicks_T2ToPins - InterpageGap;
					}
					else
					{
						MSG("(Eng)-Tray2-Path3_SavedPaperPathT2ToPins = %d,  PSTicks_T2ToPins_L = %d, InterpageGap = %d\n", SavedPaperPathT2ToPins,PSTicks_T2ToPins_L,InterpageGap);
						return PSTicks_T2ToPins_L - InterpageGap;
					}
				}
			}
			if(PGSPVSR->Source == PGSPVSR->PageAhead->Source) {
				if(SecondPage) {
					/* After Main Motor Start, First Page */
					SecondPage = CLEAR;
					Tray2FeedTime = SavedPaperPathT2ToPins;
					MSG("(Eng)-Tray2-Path4_SavedPaperPathT2ToPins = %d,  Tray2FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT2ToPins,Tray2FeedTime,InterpageGap);
					return SavedPaperPathT2ToPins - InterpageGap;
				}
				/* Continuous Print in Same Source */ 
				if(SavedPaperPathT2ToPins <= Tray2FeedTime) {
					Tray2FeedTime = SavedPaperPathT2ToPins;
				}
				else {
					Tray2FeedTime = Tray2FeedTime + 1;
				}
				MSG("(Eng)SavedPaperPathT2ToPins = %d,  Tray2FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT2ToPins,Tray2FeedTime,InterpageGap);
				return Tray2FeedTime - InterpageGap;
			}
			else {
				/* Switch Source */
				SavedRealMediaSizeInTray2 = MS_Unknown;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
				else
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;

				Tray2FeedTime = SavedPaperPathT2ToPins;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					return PSTicks_T2ToPins - InterpageGap;
				else
					return PSTicks_T2ToPins_L - InterpageGap;
			}
			break;
			
		case MS_Tray3:
			if(PGSPVSR->PageAhead->Source == MS_Duplex) {
				if(PGSPVSR->Source == PrePreSource) {
					if(SecondPage) {
						/* After Main Motor Start, First Page */
						SecondPage = CLEAR;
						Tray3FeedTime = SavedPaperPathT3ToPins;
						MSG("(eng)-Tray3-Path1_SavedPaperPathT3ToPins = %d,  InterpageGap = %d\n", SavedPaperPathT3ToPins,InterpageGap);
						return SavedPaperPathT3ToPins - InterpageGap;
					}
					
					MSG("(eng)-Tray3-Path2_SavedPaperPathT3ToPins = %d,  Tray3FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT3ToPins,Tray3FeedTime,InterpageGap);
					/* Continuous Print in Same Source */ 
					if(SavedPaperPathT3ToPins <= Tray3FeedTime) {
						Tray3FeedTime = SavedPaperPathT3ToPins;
					}
					else {
						Tray3FeedTime = Tray3FeedTime + 1;
					}
					return Tray3FeedTime - InterpageGap;
				}
				else {
					/* Switch Source */

					SavedRealMediaSizeInTray3 = MS_Unknown;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
					else
						MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;

					Tray3FeedTime = SavedPaperPathT3ToPins;
					if(fMotorSpeedMode == MTR_MODE_NORMAL)
					{
						MSG("(Eng)-Tray3-Path3_SavedPaperPathT3ToPins = %d,  PSTicks_T3ToPins = %d, InterpageGap = %d\n", SavedPaperPathT3ToPins,PSTicks_T3ToPins,InterpageGap);
						return PSTicks_T3ToPins - InterpageGap;
					}
					else
					{
						MSG("(Eng)-Tray3-Path3_SavedPaperPathT3ToPins = %d,  PSTicks_T3ToPins_L = %d, InterpageGap = %d\n", SavedPaperPathT3ToPins,PSTicks_T3ToPins_L,InterpageGap);
						return PSTicks_T3ToPins_L - InterpageGap;
					}
				}
			}
			if(PGSPVSR->Source == PGSPVSR->PageAhead->Source) {
				if(SecondPage) {
					/* After Main Motor Start, First Page */
					SecondPage = CLEAR;
					Tray3FeedTime = SavedPaperPathT3ToPins;
					MSG("(Eng)-Tray3-Path4_SavedPaperPathT3ToPins = %d,  Tray3FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT3ToPins,Tray3FeedTime,InterpageGap);
					return SavedPaperPathT3ToPins - InterpageGap;
				}
				/* Continuous Print in Same Source */ 
				if(SavedPaperPathT3ToPins <= Tray3FeedTime) {
					Tray3FeedTime = SavedPaperPathT3ToPins;
				}
				else {
					Tray3FeedTime = Tray3FeedTime + 1;
				}
#if	ENG_TRACE				
				MSG("(Eng)SavedPaperPathT3ToPins = %d,  Tray3FeedTime = %d, InterpageGap = %d\n", SavedPaperPathT3ToPins,Tray3FeedTime,InterpageGap);
#endif
				return Tray3FeedTime - InterpageGap;		
			}
			else {
				/* Switch Source */
				SavedRealMediaSizeInTray3 = MS_Unknown;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH;
				else
					MeasuredPaperLength = UNKNOWN_PAPER_LENGTH_L;

				Tray3FeedTime = SavedPaperPathT3ToPins;
				if(fMotorSpeedMode == MTR_MODE_NORMAL)
					return PSTicks_T3ToPins - InterpageGap;
				else
					return PSTicks_T3ToPins_L - InterpageGap;
			}
			break;
			
		default:
			return (Uint32)ERROR;
	}
}




Uint32	Get_WaitTimeForNarrowMedia (PageSupervisor *PGSPVSR)
{
	int RIP_PaperLength, MovingLength, SourcePaperLength, 
			RealPaperLength, WaitTimeOfNextPaperPickup;

	if(PGSPVSR->PageAhead == NULL)
		return PSTicks_Minimum;

	RIP_PaperLength = PGSPVSR->PageAhead->Length;

	MovingLength = PGSPVSR->PageAhead->T1_Value 
					+ Get_PaperPathLengthMinusGap(PGSPVSR, SavedSourceInPrePage);

	if(ThroughputRate == FAST || ThroughputRate == FASTEST) 
	{
		if(ThroughputRate == FASTEST) 
		{
			SourcePaperLength = Get_MeasuredMediaSize(PGSPVSR->PageAhead->Source) ;
		}
		else 
		{
			// if(MeasuredPaperLength >= LETTER_LENGTH) 
			// {
				// SourcePaperLength = Get_MeasuredMediaSize(PGSPVSR->PageAhead->Source);
			// }
			// else 
			// {
				// SourcePaperLength = (Interval_22ppm - PSTicks_Gap32mm) ; 
			// }
			SourcePaperLength = Get_MeasuredMediaSize(PGSPVSR->PageAhead->Source);
		}
	}
	else 
	{
		// SourcePaperLength = (IntervalNarrowMedia - PSTicks_Gap32mm/* +PSTicks_10mm*/) ; 
		// SourcePaperLength = (Interval_12ppm - PSTicks_Gap53mm/* +PSTicks_10mm*/) ; 
		SourcePaperLength = (Interval_12ppm - InterpageGap/* +PSTicks_10mm*/) ;
	}

	/* Narrow Media Length > RIP Real Paper Length */
	
	if(SourcePaperLength < RIP_PaperLength)
		/* Media Length < RIP Paper Length */
		RealPaperLength = RIP_PaperLength;
	else
		RealPaperLength = SourcePaperLength;
					
	if(MovingLength >= RealPaperLength) 
	{
		WaitTimeOfNextPaperPickup = PSTicks_Minimum;
	}
	else 
	{
		/* Narrow Media RIP Procssing time > Real Paper Length */
		WaitTimeOfNextPaperPickup = RealPaperLength - MovingLength;
	}
#if	ENG_TRACE	
	MSG("(Eng)PGSPVSR->PageAhead->T1_Value = %d, MovingLength = %d, SourcePaperLength = %d, RealPaperLength = %d, WaitTimeOfNextPaperPickup = %d \n",PGSPVSR->PageAhead->T1_Value,MovingLength,SourcePaperLength,RealPaperLength,WaitTimeOfNextPaperPickup);
#endif	
	return WaitTimeOfNextPaperPickup;
	
}		
