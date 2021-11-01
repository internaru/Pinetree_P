/*+---------------------------------------------------------------------------+
 ** 								    								
 **	 File Name :	ipc.c
 **	 Revision  :	0.1
 **	 Date	   : 	April 29, 2009
 **	 Author    :	Inhwan, Oh
 **  Overview of this file
	 A wrapping function of System V message queue (send, read, remove...)
 **	 
 **	 Modified Information :											
 ** 
 +---------------------------------------------------------------------------+*/

  
/********************************************************************
*  INCLUDES
********************************************************************/
#include <stdio.h>
#include	<time.h>
#include	<unistd.h>
#include	<math.h> /* [KSR032913] */
#include "ipc.h"
#include "farch.h"
#include "em.h"
#include "version.h" /* [KSR032601] for SMC (s/w version) */

#define DBG_PRFX "engipc: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */

#define ROUNDING(x, dig)    (floor((x) * pow(10, dig) + 0.5f) / pow(10, dig))/* [KSR032913] */

struct ipcbuf sndbuf, rcvbuf;
key_t key_engine, key_system;

Uint8 Requested_Info_ID;
Uint8 Requested_Inventory_ID;
Uint8 fRequested_Power_Saver;
Uint32 Toner_Count_Of_Page;
Uint8 Toner_Count_Buf[4];
Uint8 Total_Print_Count_Buf[4];
Uint32 cPrintData;/* [KSR032913] */
Uint32	cPageCount5perTransOnChip;
double FactorRatePerPage;/* [KSR032913] */
Uint16 SmartICWrite_TonerHisFactorRateADDRESS;/* [KSR032913] */
Uint32 SmartICWrite_TonerHisFactorRate;/* [KSR032913] */
Uint32 SmartICWrite_DrumHisFactorRate;/* [KSR032913] */
extern Uint8 fSmartICWrite_TonerHisFactorRate;/* [KSR032913] */
extern Uint8 fSmartICWrite_DrumHisFactorRate;/* [KSR032913] */

extern Boolean	EarlyStartCmd;
extern Boolean 	ColdWakeUpCmd;
extern Uint8	fFullStack_Error;
extern Uint8	fFullStackMsgSend;

extern Uint32	CurrentDarknessLevel;
extern Uint32	CurrentDotCountOnChip;
extern Uint32 cFreeMotorRunCheck_ForTonerCount;
extern Uint8 fJob_Toner_Correction_value;

//char fFactor_Page;/* [KSR032913] */
unsigned char fFactor_Page;/* [KSR032913] */

extern unsigned int Factor_Page[NUM_TONER_FACTOR];/* [KSR032913] */
extern unsigned int Drum_Factor_Page[NUM_TONER_FACTOR];/* [KSR032913] */
void *System_IPC(void *arg);
int Set_PrintProcessedInfoNotify(Uint8);
int Set_CartridgeTLISerialNotify(void);
int Set_PaperJamNotify(Uint8, Uint8);
void CheckIPC(void);
void Save_PrintPageParameter(void);

extern Uint8 CheckFeedSensor(void);
extern Uint8 ftempEngineRestartCmd;
extern Struct_Temp_PageMsg		Temp_PageMsg;
extern ST_FLAG CommandReceiveFlag;

extern Struct_Fuser_Control Fuser_Control;

extern ST_FLAG PaperJam;
extern ST_FLAG	TempPaperJam;
extern ST_FLAG EngineFatalError1;
extern Uint8	CodeOfJamStatus;
extern Uint32 PendingPrintPageData[RIPDATA];
extern Uint8 Tray2_Version_Major;
extern Uint8 Tray2_Version_Minor;
extern Uint8 	fPreRunBeforePrintCmd;
extern Uint8	fHSync_Detacted;
extern Uint8	fClearEmpty;
extern Uint8	temp_fPaperEmptyForT1;
extern Uint8	temp_fPaperEmptyForT2;
extern Uint8	temp_fPaperEmptyForMPF;
extern ST_FLAG IpcFlag1;
extern ST_FLAG IpcFlag2;
extern ST_FLAG IpcFlag3;
extern Uint32 	cPreRunTimeoutCounter;

extern int 		cProcessCounter;
#if 1 /* [KSR032501] for SMC */
extern Uint32 	cTotal_Print_Count;
extern Uint8 	fSmartICWrite_DotCount;
extern Uint8 	fSmartICWrite_DrumDotCount;/* [KSR032913] */
extern Uint32	cDotCount;
extern Uint32	cDrumDotCount;/* [KSR032913] */
extern Uint32 	cPageCount5perTrans;
extern Uint32 	cDrumPageCount5perTrans;/* [KSR032913] */

extern Uint32	TonerCartridgeCapacity;
#endif
extern Uint32 	CartridgeSerialNum;
#if 1 /* [KSR032501] for SMC */
extern Uint8 	fState_TonerNearEnd;
extern Uint8 	fState_DrumNearEnd;/* [KSR032913] */
extern Uint8 	fState_TonerEnd;
extern Uint8 	fState_DrumEnd;/* [KSR032913] */
extern Uint8 	fState_TonerBlock;
extern Uint8 	fState_DrumBlock;/* [KSR032913] */
extern Uint8 	fSmartICWrite_TonerNearEnd;
extern Uint8 	fSmartICWrite_DrumNearEnd;/* [KSR032913] */
extern Uint8 	fSmartICWrite_TonerEnd;
extern Uint8 	fSmartICWrite_DrumEnd;/* [KSR032913] */
extern Uint8 	fSmartICWrite_TonerBlock;
extern Uint8 	fSmartICWrite_DrumBlock;/* [KSR032913] */
#endif
extern Uint8 	TLI_GET[13];
extern Uint8	f2ndDrawerState;
extern Uint16	CodeOfMachineCheck;
extern Uint32 	TonerNearEndValue;
extern Uint32 	TonerBlockValue;
extern Uint8	fMotorRunWithoutPrintCmd;
extern Uint8 	fSC_Writefault;
extern Struct_Fuser_Control FuserControlMsgSet; /* [KSR032501] for SMC */
extern Uint32	PaperLength[2][26];
extern Uint8	fCartridgeRecycleType; /* [KSR032501] for SMC */
extern Uint8 	fMotorSpeedMode;
extern Uint8	fState_FullStack;
extern Uint8	fCoolingState;
#if SMART_IC_LOCK_ENABLE
extern Uint8 ftoner_count_write_for_coveropen;
#endif

extern Uint8 IsPaperPresentOnMPF(void);
extern Uint8 IsPaperPresentOnTray1(void);	// added	//21Apr09 yjlee 
extern Uint8 IsPaperPresentOnPins(void);
extern Uint8 IsPaperPresentOnPexs(void);
extern Uint8 IsPaperPresentOnDis(void);

extern void Check_Sensor_Status(void);
extern void Sensor_Test_Off(void);
extern void All_Solenoid_Off(void);
extern void Solenoid_Test(void);
extern int Get_VerticalMagValue(void);
extern int Get_HorizontalMagValue(void);

extern Uint8 IsSourceGood(Uint8);

extern void Sensor_Power_On(void);

/********************************************************************
*  FUNCTIONS
********************************************************************/

/*#######################################################
 *  Function Name: send_message
 *
 *  Description:
 *    This function is for sending message queue 
 *
 *  Inputs:
 *	  int qid - message queue ID
 *    struct ipcbuf *qbuf - pointer of message buffer
 *    long type - messageQ type
 *
 #######################################################*/
int send_message(int qid, struct ipcbuf *qbuf, long type)
{
	int result, length;
	length = sizeof(struct ipcbuf) - sizeof(long);
	qbuf->msgtype = type;
#if KSR_DBG_THREE	
//	printf("(IPC)sending msg...  "); /* [KSF050806] */
#endif
	if( (result = msgsnd(qid, qbuf, length, 0)) == -1 )
	{
		perror("msg send error\n");
		exit(1);
	}
#if KSR_DBG_THREE		
//	printf("(IPC)sending msg : Done\n"); /* [KSF050806] */
#endif
	return(result);
}
/*#######################################################
 *  Function Name:  read_message
 *
 *  Description:
 *    This function is for receiving message queue 
 *
 *  Inputs:
 *	  int qid - message queue ID
 *    struct ipcbuf *qbuf - pointer of message buffer
 *    long type - messageQ type
 *
 #######################################################*/
int read_message(int qid, struct ipcbuf *qbuf, long type)
{
	int result, length;
	length = ( sizeof(struct ipcbuf) - sizeof(long) );

	while(1)
	{
		if( (result = msgrcv(qid, qbuf, length, type, 0)) != -1 )
			break;
	}
	return(result);
}

/*#######################################################
 *  Function Name:  remove_queue
 *
 *  Description:
 *    This function is for removing message queue 
 *
 *  Inputs:
 *	  int qid - message queue ID
 *
 #######################################################*/
int remove_queue(int qid)
{
	//printf("(Eng)removing queue...");
	if( msgctl(qid, IPC_RMID, 0) == -1 )
	{
		perror("queue remove error\n");
		exit(1);
	}	
	return(0);
}

void *System_IPC(void *arg)
{
	setlinebuf(stdout);

	// puts("IPC Start!");
	/* Create and Open each msgQs for sending and receiving */
	if ( (key_system = msgget((key_t)1111, IPC_CREAT|0666)) == -1 )
	{
		perror("msgget call error from system\n");
		exit(1);
	}

	if ( (key_engine = msgget((key_t)2222, IPC_CREAT|0666)) == -1 )
	{
		perror("msgget call error from system\n");
		exit(1);
	}
	
	// receive and send msg
	while(1)
	{
		if (read_message(key_system, &rcvbuf, 1) == -1)
		{
			perror("receive_msg call error\n");
			exit(1);   
		}
#if	ENG_TRACE		
		MSG("(Eng)##IPC(SYSTEM to Engine <%d>) = 0x%x, %d, %d, %d, %d##\n",cProcessCounter,rcvbuf.cmd,rcvbuf.cd1,rcvbuf.cd2,rcvbuf.cd3,rcvbuf.cd4);
#endif
		switch (rcvbuf.cmd)
		{
			case Engine_Restart:
				ftempEngineRestartCmd = RECEIVED;
				fEngine_Restart_Cmd = ACTIVE;
#if KSR_DBG_THREE
		MSG("[systemIPC] Engine_Restart\n");
#endif					
				break;
				
			case Print_Page:
				Temp_PageMsg.temp_PageID = rcvbuf.cd1;
				Temp_PageMsg.temp_Source = rcvbuf.cd2;
				Temp_PageMsg.temp_Narrow = rcvbuf.cd3;
				Temp_PageMsg.temp_MediaDescription = rcvbuf.cd4;
				Temp_PageMsg.temp_Length = rcvbuf.cd5;
				Temp_PageMsg.temp_Darkness = rcvbuf.cd6;
				Temp_PageMsg.temp_TonerSaver = rcvbuf.cd7;
				Temp_PageMsg.temp_OutputDestination = rcvbuf.cd8;
				Temp_PageMsg.temp_cheat_gap = rcvbuf.cd9;
#if KSR_DBG_THREE
		MSG("[systemIPC] Print_Page : PageID(%d) Source(%d) Narrow(%d) MediaDescription(%d) Length(%d) Darkness(%d) TonerSaver(%d) OutputDestination(%d) cheat_gap(%d)\n",Temp_PageMsg.temp_PageID, Temp_PageMsg.temp_Source, Temp_PageMsg.temp_Narrow, Temp_PageMsg.temp_MediaDescription, Temp_PageMsg.temp_Length, Temp_PageMsg.temp_Darkness, Temp_PageMsg.temp_TonerSaver, Temp_PageMsg.temp_OutputDestination, Temp_PageMsg.temp_cheat_gap);
#endif				
				fPrint_Page_Cmd = ACTIVE;
				break;

			case Early_Start_Print:
#if KSR_DBG_THREE
		MSG("[systemIPC] Early_Start_Print\n");
#endif			
				if(FuserControlMsgSet.fMakeMotorRun != SET)
				{
					fEarly_Start_Print_Cmd = ACTIVE;
				}
				else
				{
					fEarly_Start_Print_Cmd = NOT_ACTIVE;
				}
				break;
				
			case Query_Jammed_Paper_Locations:
#if KSR_DBG_THREE
		MSG("[systemIPC] Query_Jammed_Paper_Locations\n");
#endif			
				fQuery_Jammed_Paper_Locations_Cmd = ACTIVE;
				break;
				
			case Clear_Intervention_Required_Errors:
#if KSR_DBG_THREE
		MSG("[systemIPC] Clear_Intervention_Required_Errors\n");
#endif			
				if(rcvbuf.cd1 != 1)
				{
					fClear_Intervention_Required_Errors_Cmd = ACTIVE;
				}
				if ((temp_fPaperEmptyForT1==EMPTY) || (temp_fPaperEmptyForMPF==EMPTY) || (temp_fPaperEmptyForT2==EMPTY))
				{
					fClearEmpty = TRUE;
#if KSR_DBG_THREE				
					MSG("(Eng) fClearEmpty = TRUE !!! AutoTrayLink = <%d>\n",rcvbuf.cd1);
#endif					
				}
				break;
				
			case Query_Attached_Devices:
#if KSR_DBG_THREE
		MSG("[systemIPC] Query_Attached_Devices\n");
#endif			
				fQuery_Attached_Devices_Cmd = ACTIVE;
				break;
				
			case Query_Device_Information:
#if KSR_DBG_THREE
		MSG("[systemIPC] Query_Device_Information\n");
#endif			
				Requested_Info_ID = rcvbuf.cd1;
				fQuery_Device_Information_Cmd = ACTIVE;
				break;
				
			case Query_Device_Inventory:
#if KSR_DBG_THREE
		MSG("[systemIPC] Query_Device_Inventory\n");
#endif			
				Requested_Inventory_ID = rcvbuf.cd1;
				fQuery_Device_Inventory_Cmd = ACTIVE;
				break;
				
			case Power_Saver_mode:
#if KSR_DBG_THREE
		MSG("[systemIPC] Power_Saver_mode\n");
#endif			
				fPower_Saver_mode_Cmd = ACTIVE;
				break;
				
			case Wake_Up:
#if KSR_DBG_THREE
		MSG("[systemIPC] fPower_Saver_mode_Wake_Up_Cmd\n");
#endif			
				fPower_Saver_mode_Wake_Up_Cmd = ACTIVE;
				break;
				
			case Query_Machine_Check_Information:
#if KSR_DBG_THREE
		MSG("[systemIPC] fQuery_Machine_Check_Information_Cmd\n");
#endif			
				fQuery_Machine_Check_Information_Cmd = ACTIVE;
				break;
				
			case HSync_FAIL:
#if KSR_DBG_THREE
		MSG("[systemIPC] HSync_FAIL\n");
#endif			
				fHSync_FAIL_Cmd = ACTIVE;
				break;
				
			case HSync_OKAY:
				fHSync_OKAY_Cmd = ACTIVE;
#if KSR_DBG_THREE
		MSG("[systemIPC] HSync_OKAY\n");
#endif					
				break;
				
			case Toner_Count: /* 0x42 */	
				Toner_Count_Buf[0] = rcvbuf.cd1;
				Toner_Count_Buf[1] = rcvbuf.cd2;
				Toner_Count_Buf[2] = rcvbuf.cd3;
				Toner_Count_Buf[3] = rcvbuf.cd4;
				fToner_Count_Cmd = ACTIVE;
#if KSR_DBG_THREE
		MSG("[systemIPC] Toner_Count_Buf[%d][%d][%d][%d]\n",Toner_Count_Buf[0],Toner_Count_Buf[1],Toner_Count_Buf[2],Toner_Count_Buf[3]);
#endif					
				break;
				
			case Total_Print_Count:	/* 0x43 */		
				Total_Print_Count_Buf[0] = rcvbuf.cd1;
				Total_Print_Count_Buf[1] = rcvbuf.cd2;
				Total_Print_Count_Buf[2] = rcvbuf.cd3;
				Total_Print_Count_Buf[3] = rcvbuf.cd4;
				fTotal_Print_Count_Cmd = ACTIVE;
#if KSR_DBG_THREE
		MSG("[systemIPC] Total_Print_Count_Buf[%d][%d][%d][%d]\n",Total_Print_Count_Buf[0],Total_Print_Count_Buf[1],Total_Print_Count_Buf[2],Total_Print_Count_Buf[3]);
#endif				
				break;
				
			case Query_Engine_Software_Info:
#if KSR_DBG_THREE
		MSG("[systemIPC] Query_Engine_Software_Info\n");
#endif			
				fQuery_Engine_Software_Info_Cmd = ACTIVE;
				break;
				
			case Magnification_Chg:
#if KSR_DBG_THREE
		MSG("[systemIPC] Magnification_Chg\n");
#endif			
				fMagnification_Chg_Cmd = ACTIVE;
				break;
				
			case Sensor_Test_Start:
#if KSR_DBG_THREE
		MSG("[systemIPC] Sensor_Test_Start\n");
#endif			
				fSensor_Test_Start_Cmd = ACTIVE;
				break;
				
			case Sensor_Test_End:
#if KSR_DBG_THREE
		MSG("[systemIPC] Sensor_Test_End\n");
#endif			
				fSensor_Test_End_Cmd = ACTIVE;
				break;
				
			case Solenoid_Test_Start:
#if KSR_DBG_THREE
		MSG("[systemIPC] Solenoid_Test_Start\n");
#endif			
				fSolenoid_Test_Start_Cmd = ACTIVE;
				break;
				
			case Solenoid_Test_End:
#if KSR_DBG_THREE
		MSG("[systemIPC] Solenoid_Test_End\n");
#endif			
				fSolenoid_Test_End_Cmd = ACTIVE;
				break;
				
			case Cold_Wake_Up:
#if KSR_DBG_THREE
		MSG("[systemIPC] Cold_Wake_Up\n");
#endif			
				fCold_Wake_Up_Cmd = ACTIVE;
				break;

			default:
#if KSR_DBG_THREE
		MSG("[systemIPC] Not correct msg from system\n");
#endif			
				perror("Not correct msg from system\n");
				break;
		}
	}
		
	/* Remove Sending msgQ */
	if (remove_queue(key_system) == -1)
	{
		perror("msgQ_remove call error\n");
		exit(1);
	}
#if KSR_DBG_THREE	
	MSG("(Eng)##IPC_Thread EXIT##\n");
#endif	
//	return(0);
//	pthread_exit(NULL);
}

int Set_CoolingNotify(Uint8 status)
{
	//sndbuf.cmd = Cooling_Info;
	//sndbuf.cd1 = status;
	//if(sndbuf.cd1 == SET)
	//{
		//fCoolingState = SET;
	//}
	//else
	//{
		//fCoolingState = CLEAR;
	//}
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Cooling_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_FullStackNotify(Uint8 status)
{
	//sndbuf.cmd = Full_Stack_Info;
	//sndbuf.cd1 = status;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
	//if(status)
	//{
		//fFullStackMsgSend = SET; 
	//}
	//else
	//{
		//fFullStackMsgSend = CLEAR;
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Full_Stack_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_WakeKeyNotify(void)
{
	//sndbuf.cmd = Wake_Key_Info;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Wake_Key_Info)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_IgnoreEarlyStartNotify(void)
{
	//sndbuf.cmd = Ignore_EarlyStart;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Ignore_EarlyStart)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_SensorStatusInfoNotify(Uint8 SensorID, Uint8 status)
{
	//sndbuf.cmd = Sensor_Status_Info;
	//sndbuf.cd1 = SensorID;
	//sndbuf.cd2 = status;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, 0x%x (Sensor_Status_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, sndbuf.cd2);
//#endif
	return 1;
}

int Set_EngineVersionInfo(void)
{
	//sndbuf.cmd =  Tray_SW_Ver_Info;
	//sndbuf.cd1 =  SW_SET_Major_Version;
	//sndbuf.cd2 =  SW_SET_Mid_Version;
	//sndbuf.cd3 =  SW_SET_Minor_Version;
	//sndbuf.cd4 =  f2ndDrawerState;
	//sndbuf.cd5 =  Tray2_Version_Major;
	//sndbuf.cd6 =  Tray2_Version_Minor;
	
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC_MSG(Engine to SYSTEM <%d>) = 0x%x (EngineVersionInfo) Engine ver : %d. %d. %d , Tray2 Detacted : %d, SW Version %d. %d##\n"
	//,cProcessCounter, sndbuf.cmd,sndbuf.cd1,sndbuf.cd2,sndbuf.cd3,sndbuf.cd4,sndbuf.cd5,sndbuf.cd6);
//#endif
	return 1;
}
	
int Set_CartridgeTLISerialNotify(void)
{
	MESSAGE message;
	
	message.msgType = MSG_CARTRIDGE_INFO;
	message.param1 = CartridgeSerialNum;
	message.param2 = TonerCartridgeCapacity;
	message.param3 = (char*)TLI_GET;
	
	SYMsgSend(PRINTMGRID, &message);
	//sndbuf.cmd =  Cartridge_TLI_Serial_Info;
	//sndbuf.cd1 =  TLI_GET[0];
	//sndbuf.cd2 =  TLI_GET[1];
	//sndbuf.cd3 =  TLI_GET[2];
	//sndbuf.cd4 =  TLI_GET[3];
	//sndbuf.cd5 =  TLI_GET[4];
	//sndbuf.cd6 =  TLI_GET[5];
	//sndbuf.cd7 =  TLI_GET[6];
	//sndbuf.cd8 =  TLI_GET[7];
	//sndbuf.cd9 =  TLI_GET[8];
	//sndbuf.cd10 = TLI_GET[9];
	//sndbuf.cd11 = TLI_GET[10];
	//sndbuf.cd12 = TLI_GET[11];
	//sndbuf.cd13 = (Uint8)(CartridgeSerialNum>>24);
	//sndbuf.cd14 = (Uint8)(CartridgeSerialNum>>16);
	//sndbuf.cd15 = (Uint8)(CartridgeSerialNum>>8);
	//sndbuf.cd16 = (Uint8)(CartridgeSerialNum>>0);
	
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Cartridge_TLI_Serial_Info) : TLI = %c%c%c%c%c%c%c%c%c%c%c%c, Serial = %d##\n",cProcessCounter, sndbuf.cmd
	//,TLI_GET[0],TLI_GET[1],TLI_GET[2],TLI_GET[3],TLI_GET[4],TLI_GET[5],TLI_GET[6],TLI_GET[7],TLI_GET[8],TLI_GET[9],TLI_GET[10],TLI_GET[11],CartridgeSerialNum);
//#endif
	return 1;
}

int Set_LSUOffNotify(void)
{
	//sndbuf.cmd = LSU_OFF;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (LSU_OFF)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_LSUOnNotify(void)
{
	//sndbuf.cmd = LSU_ON;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (LSU_On)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_PrintReadyNotify(void)
{
	//sndbuf.cmd = Print_Ready;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Print_Ready)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_PowerSaveActiveNotify(void)
{
	//sndbuf.cmd = Power_Saver_Active;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Power_Saver_Active)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_ConfigurationChangeErrorNotify(void)
{
	//sndbuf.cmd = Configuration_Change_Error;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Configuration_Change_Error)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_PrintCompleteNotify(Uint32 PageID)
{
	//sndbuf.cmd = Print_Complete;
	//sndbuf.cd1 = PageID;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Print_Complete)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_CoverOpenNotify(void)
{
	//sndbuf.cmd = Cover_Open;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Cover_Open)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_CoverClosedNotify(void)
{
	//sndbuf.cmd = Cover_Closed;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x (Cover_Closed)##\n",cProcessCounter, sndbuf.cmd);
//#endif
	return 1;
}

int Set_MachineCheckNotify(Uint8 ErrorCode)
{
	//sndbuf.cmd = Machine_Check;
	//sndbuf.cd1 = ErrorCode;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Machine_Check)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_MotorStatusNotify(Uint8 status, Uint32 Runtime, Uint8 AmountUsedToner, Uint32 PageCount_5)
{
	//if (AmountUsedToner > 100)
	//{
		//AmountUsedToner = 100;
	//}
	
	//sndbuf.cmd = Transport_Motor_Status;
	//sndbuf.cd1 = status;
	//sndbuf.cd2 = (Uint8)(Runtime>>24);
	//sndbuf.cd3 = (Uint8)(Runtime>>16);
	//sndbuf.cd4 = (Uint8)(Runtime>>8);
	//sndbuf.cd5 = (Uint8)(Runtime>>0);
	//sndbuf.cd6 = AmountUsedToner;
	//sndbuf.cd7 = (Uint8)(PageCount_5>>24);
	//sndbuf.cd8 = (Uint8)(PageCount_5>>16);
	//sndbuf.cd9 = (Uint8)(PageCount_5>>8);
	//sndbuf.cd10 = (Uint8)(PageCount_5>>0);
	
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, Runtime = %d, AmountUsedToner = %d, PageCount5 = %d (Transport_Motor_Status)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, Runtime, AmountUsedToner, PageCount_5);
//#endif
	return 1;
}

int Set_SourceEmptyNotify(Uint8 DeviceID, Uint8 status)
{
	//sndbuf.cmd = Source_Empty;
	//sndbuf.cd1 = DeviceID;
	//sndbuf.cd2 = status;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, 0x%x (Source_Empty)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, sndbuf.cd2);
//#endif
	return 1;
}

int Set_PaperJamNotify(Uint8 JamLocation, Uint8 JamCode)
{
	//sndbuf.cmd = Paper_Jam;
	//sndbuf.cd1 = JamLocation;
	//sndbuf.cd2 = JamCode;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, %d (Paper_Jam)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, sndbuf.cd2);
//#endif
	return 1;
}

int Set_PrintProcessedInfoNotify(Uint8 status)
{
	//sndbuf.cmd = Print_Processed_Info;
	//sndbuf.cd1 = status;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Print_Processed_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_AttachedDeviceInfoNotify(Uint8 DevInfo)
{
	//sndbuf.cmd = Attached_Devices_Info;
	//sndbuf.cd1 = DevInfo;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Attached_Devices_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

int Set_CapacityOfSheetsInfoNotify(Uint8 DeviceID, Uint16 capacity)
{
	//sndbuf.cmd = Capacity_of_Sheets_Info;
	//sndbuf.cd1 = DeviceID;
	//sndbuf.cd2 = capacity/0x100;
	//sndbuf.cd3 = capacity%0x100;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, 0x%x, 0x%x (Capacity_of_Sheets_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, sndbuf.cd2, sndbuf.cd3);
//#endif
	return 1;
}

int Set_CurrentMediaLevelNotify(Uint8 DeviceID, Uint8 status)
{
	//sndbuf.cmd = Current_Media_Level_Info;
	//sndbuf.cd1 = DeviceID;
	//sndbuf.cd2 = status;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x, 0x%x (Current_Media_Level_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1, sndbuf.cd2);
//#endif
	return 1;
}

int Set_MachineCheckInfoNotify(Uint8 ErrorCode)
{
	//sndbuf.cmd = Machine_Check_Info;
	//sndbuf.cd1 = ErrorCode;
	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, 0x%x (Machine_Check_Info)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

extern engine_state_t engine_state;

int Set_SmartICInfoNotify(Uint8 ErrorCode, Uint8 sub_info) /* [KSR071101] */
{
	/* [KSR061102] */
	MESSAGE message;
	message.msgType = MSG_STATUSUPDATE;

	/* [KSR070401] */
	if( ErrorCode == CARTRIDGE_CMD_CERT_FAILULE )
	{
		engine_state.cartridge_rec_fail = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_REINSERTCARTRIDGE ); 
	
		//message.param1 = STATUS_PAPER_JAM_REINSERTCARTRIDGE;			
		SYMsgSend(PRINTMGRID, &message);
#if KSR_DBG_ERR		
		MSG("[%d][%d] => STATUS_PAPER_JAM_REINSERTCARTRIDGE\n",ErrorCode,sub_info);
#endif		
	}
	else if( ErrorCode == CARTRIDGE_CMD_CLOSE_FAILULE )
	{	
		engine_state.cartridge_lock = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_TONER_BLOCK ); 
		
		//message.param1 = STATUS_PAPER_JAM_TONER_BLOCK;
		SYMsgSend(PRINTMGRID, &message);
#if KSR_DBG_ERR
		MSG("[%d][%d] => STATUS_PAPER_JAM_TONER_BLOCK\n",ErrorCode,sub_info);			
#endif		
	}
	else if( ErrorCode == CARTRIDGE_CMD_TONER_LOW_END )
	{
		if( sub_info == PARAM_TONER_LOW )
		{
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_TONER_LOW );
			SYMsgSend(PRINTMGRID, &message);
#if KSR_DBG_ERR
			MSG("[%d][%d] => STATUS_TONER_LOW\n",ErrorCode,sub_info);							
#endif			
		}
		else if( sub_info == PARAM_TONER_END )
		{
			//message.param1 = STATUS_TONER_END; /* [KSR071101] */
			slogit( engine_state.pm_statusrequest = message.param1 = STATUS_TONER_LOW ); /* [KSR071201] */			
			SYMsgSend(PRINTMGRID, &message);			
#if KSR_DBG_ERR
			MSG("[%d][%d] => STATUS_TONER_END\n",ErrorCode,sub_info);				
#endif
		}
		else
		{
#if KSR_DBG_ERR
			MSG("[%d][%d] => STATUS_TONER_UNKNOWEN\n",ErrorCode,sub_info);				
#endif			
		}
	}
#if SMART_IC_LOCK_ENABLE
	else if( ErrorCode == CARTRIDGE_CMD_WRONG_DATA )
	{
//		engine_state.cartridge_rec_fail = true;
		slogit( engine_state.pm_statusrequest = message.param1 = STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE ); 
	
		//message.param1 = STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE;			
		SYMsgSend(PRINTMGRID, &message);
#if KSR_DBG_ERR		
		MSG("[%d][%d] => STATUS_PAPER_JAM_ILLEGAL_CARTRIDGE(Wrong SmartIC)\n",ErrorCode,sub_info);
#endif		
	}
#endif
	else if( ErrorCode == CARTRIDGE_CMD_NO_ISSUE )
	{
#if KSR_DBG_ERR		
			MSG("[%d][%d] => CARTRIDGE_CMD_NO_ISSUE\n",ErrorCode,sub_info);		
#endif
			engine_state.cartridge_rec_fail = false;
			engine_state.cartridge_lock = false;
			
			engine_state.pm_statusrequest = message.param1 = STATUS_TONER_NO_ISSUE; /* [KSR071502] */			
			SYMsgSend(PRINTMGRID, &message);			
	}
	else
	{
#if KSR_DBG_ERR		
			MSG("[%d][%d] => UNKNOWEN CMD\n",ErrorCode,sub_info);		
#endif
			engine_state.cartridge_rec_fail = false;
			engine_state.cartridge_lock = false;
			
			message.param1 = STATUS_TONER_NO_ISSUE; /* [KSR071502] */			
			SYMsgSend(PRINTMGRID, &message);			
	}
	return 1;
}

int Set_AmountTonerUsedNotify(Uint8 AmountUsed)
{
	//sndbuf.cmd = Amount_Toner_Used;
	//sndbuf.cd1 = AmountUsed;

	//if (send_message(key_engine, &sndbuf, 1) == -1)
	//{
		//perror("send_msg call error\n");
		//exit(1);
	//}
//#if	ENG_TRACE	
	//MSG("(Eng)##IPC(Engine to SYSTEM <%d>) = 0x%x, %d (Amount_Toner_Used)##\n",cProcessCounter, sndbuf.cmd, sndbuf.cd1);
//#endif
	return 1;
}

void CheckIPC(void) /* [KSR032501] for SMC */
{
//	printf("[CheckIPC] Check IPC !!\n");	
	if(fEngine_Restart_Cmd == ACTIVE)
	{
		CommandReceiveFlag.DR.BYTE = (Uint8)0 ;
		fEngineRestartCmd = ftempEngineRestartCmd;
		ftempEngineRestartCmd = CLEAR;
		fEngine_Restart_Cmd = NOT_ACTIVE;
#if KSR_DBG_THREE			
		MSG("(Eng)Tray2 Paper Check before POR!!!\n");
#endif			
//		fTray2Notify = TRUE; /* [KSR032501] for SMC */
	}
	
	if(fPrint_Page_Cmd == ACTIVE)
	{
// #if FULL_STACK
//~ #if MAC_TYPE_MH
		if(((fSC_Writefault==CLEAR) && (fState_FullStack==CLEAR)) || 
		((fState_FullStack==SET) && (PendingPrintPageData[7] == OutputDuplexor)))
//~ #else
		//~ if(fSC_Writefault==CLEAR)	
//~ #endif
		{
			Save_PrintPageParameter();
			fPrint_Page_Cmd = NOT_ACTIVE;
		}
// #if FULL_STACK
//~ #if MAC_TYPE_MH
		else if(fState_FullStack==SET)
		{
#if	ENG_TRACE		
			MSG("(Eng)## fFullStack_Error : SET ##\n");
#endif				
			// fFullStack_Error = SET;
			Set_FullStackNotify(FullStackSet);
			fPrint_Page_Cmd = NOT_ACTIVE;
		}
//~ #endif		
	}
	
	if(fEarly_Start_Print_Cmd == ACTIVE)
	{
		if(fPendingPrintPageCmd != SET)
		{
			if((Fuser_Control.EM_State == EMS_Standby) || (Fuser_Control.EM_State == EMS_PowerSave))
			{
				fPreRunBeforePrintCmd = SET;
				cPreRunTimeoutCounter = cProcessCounter;
			}
		}
		fEarly_Start_Print_Cmd = NOT_ACTIVE;
	}
	
	if(fQuery_Jammed_Paper_Locations_Cmd == ACTIVE)
	{
		if(IsPaperPresentOnPins() == PRESENT) 
		{
			if(IsPaperPresentOnPexs() == PRESENT) 
			{
				fPaperJamBetweenPinsAndPexs = JAM;
			}
			else 
			{
				fPaperJamOnPins	= JAM;
			}
		}
		else if(IsPaperPresentOnPexs() == PRESENT) 
		{
			fPaperJamOnPexs = JAM;
		}
		CodeOfJamStatus = JAM_IDLE_STATE;
		Set_PaperJamNotify(PaperJam.DR.BYTE, CodeOfJamStatus);
	
		fQuery_Jammed_Paper_Locations_Cmd = NOT_ACTIVE;
	}
	
	if(fClear_Intervention_Required_Errors_Cmd == ACTIVE)
	{
#if KSR_DBG_THREE		
		MSG("(Eng)## Clear Intervention Required Error CMD (0xF0) Detacted ##\n");
#endif		
		fClearErrorsCmdRcv = RECEIVED;
		fClear_Intervention_Required_Errors_Cmd = NOT_ACTIVE;
	}
	
	if(fQuery_Attached_Devices_Cmd == ACTIVE)
	{
		// bit0 : Tray1, bit1 : Tray2, bit2~6 : Reserved, bit7 : MPT
		if(1)
		{
			// Tray2 is not connected
			Set_AttachedDeviceInfoNotify(0x81);
		}
		else
		{
			// Tray2 is connected
			Set_AttachedDeviceInfoNotify(0x83);
		}
		fQuery_Attached_Devices_Cmd = NOT_ACTIVE;
	}
	
	if(fQuery_Device_Information_Cmd == ACTIVE)
	{
		switch(Requested_Info_ID)
		{
			case MS_Tray1:
				Set_CapacityOfSheetsInfoNotify(MS_Tray1, 250);
				break;
			case MS_Tray2:
				Set_CapacityOfSheetsInfoNotify(MS_Tray2, 250);
				break;
			case MS_MultiPurposeTray:
				Set_CapacityOfSheetsInfoNotify(MS_MultiPurposeTray, 50);
				break;
		}
		fQuery_Device_Information_Cmd = NOT_ACTIVE;
	}
	
	if(fQuery_Device_Inventory_Cmd == ACTIVE)
	{
		if(IsSourceGood(Requested_Inventory_ID) == TRUE)
		{
			Set_CurrentMediaLevelNotify(Requested_Inventory_ID, NO_EMPTY);
		}
		else
		{
			Set_CurrentMediaLevelNotify(Requested_Inventory_ID, EMPTY);
		}
		fQuery_Device_Inventory_Cmd = NOT_ACTIVE;
	}
	
	if(fPower_Saver_mode_Cmd == ACTIVE)
	{
#if 1 /* 20130212 del by Ryu */
		if((FuserControlMsgSet.EM_State == EMS_Standby) || (FuserControlMsgSet.EM_State == EMS_PowerSave))
		{
			fRequested_Power_Saver = ACTIVE;
		}
		fPower_Saver_mode_Cmd = NOT_ACTIVE;
#endif
	}
	
	if(fPower_Saver_mode_Wake_Up_Cmd == ACTIVE)
	{
		EarlyStartCmd = ACTIVE;
		Sensor_Power_On();
		fPower_Saver_mode_Wake_Up_Cmd = NOT_ACTIVE;
	}
	
	if(fQuery_Machine_Check_Information_Cmd == ACTIVE)
	{
		Set_MachineCheckInfoNotify(CodeOfMachineCheck);
		fQuery_Machine_Check_Information_Cmd = NOT_ACTIVE;
	}
	
	if(fHSync_FAIL_Cmd == ACTIVE)
	{
		fNoFirstHsyncSignal = SET;
		CodeOfMachineCheck = PRINTHEAD_LOST_HSYNC;
		fHSync_FAIL_Cmd = NOT_ACTIVE;
	}
	
	if(fHSync_OKAY_Cmd == ACTIVE)
	{
		fHSync_Detacted = SET;
		fNoFirstHsyncSignal = CLEAR;
		fHSync_OKAY_Cmd = NOT_ACTIVE;
	}
	
	if(fToner_Count_Cmd == ACTIVE) /* [KSR050902] need cmd (system -> eng) for reset DotCount */
	{
		//Toner_Count_Of_Page = (Uint32)(Toner_Count_Buf[0]<<24) 
		//+ (Uint32)(Toner_Count_Buf[1]<<16) 
		//+ (Uint32)(Toner_Count_Buf[2]<<8) 
		//+ (Uint32)(Toner_Count_Buf[3]);	
			
		//cDotCount += cDotCount;
#if SMART_IC_LOCK_ENABLE		
		ftoner_count_write_for_coveropen = SET;
#endif
		if(TonerCartridgeCapacity == DEFAULT_TONER_CART_130)
		{
#if TONER_LOCKUP_13750
			if(cDotCount > DOTCOUNT_OF_5PERCHART_13750)
			{
				Toner_Count_Of_Page /= 3;
				MSG("(T)13750 Over : Toner_Count_Of_Page = %d!!\n", Toner_Count_Of_Page);
			}
#elif TONER_LOCKUP_14250
			if(cDotCount > DOTCOUNT_OF_5PERCHART_14250)
			{
				Toner_Count_Of_Page /= 3;
				MSG("(T)14250 Over : Toner_Count_Of_Page = %d!!\n", Toner_Count_Of_Page);
			}
#else
			if(cDotCount > DOTCOUNT_OF_5PERCHART_15K)
			{
				Toner_Count_Of_Page /= 3;
				MSG("(T)15K Over : Toner_Count_Of_Page = %d!!\n", Toner_Count_Of_Page);
			}
#endif
		}
		
		cDotCount += (Toner_Count_Of_Page * CurrentDarknessLevel / 100);
		cDotCount += (cFreeMotorRunCheck_ForTonerCount * NIP_TONER_CONSUMPTION);
		
		if(fJob_Toner_Correction_value == SET)
		{
			cDotCount += JOB_TONER_CORRECTION_VALUE;
		}
		if(cDotCount >= MAX_VALUE_IN_CHIP)
		{
			MSG("(T)DotCount Overflow!! %u\n", cDotCount);
			cDotCount = MAX_VALUE_IN_CHIP; // dotcount MAX!!
		}
		cDrumDotCount += Toner_Count_Of_Page; /* [KSR032913] */
#if KSR_DBG_THREE
		MSG("[IPC] cDotCount[%d] cDrumDotCount[%d] Toner_Count_Of_Page[%d] CurrentDarknessLevel[%d] cFreeMotorRunCheck_ForTonerCount[%d], fJob_Toner_Correction_value[%d]\n"
			,cDotCount,cDrumDotCount,Toner_Count_Of_Page, CurrentDarknessLevel, cFreeMotorRunCheck_ForTonerCount, fJob_Toner_Correction_value);
#endif
		cFreeMotorRunCheck_ForTonerCount = 0;
		fJob_Toner_Correction_value = CLEAR;
		
		cPageCount5perTrans = cDotCount / TRANS_COEFFICIENT_5PERCENT;
		cPageCount5perTransOnChip = CurrentDotCountOnChip / TRANS_COEFFICIENT_5PERCENT;
		cDrumPageCount5perTrans = cDrumDotCount / TRANS_COEFFICIENT_5PERCENT;/* [KSR032913] */
//		FactorRatePerPage = ROUNDING((((double)Toner_Count_Of_Page / TRANS_COEFFICIENT_100PERCENT) * 100), 2);/* [KSR032913] */
#if KSR_DBG_THREE
//		printf("[IPC] cPageCount5perTrans[%d] cDrumPageCount5perTrans[%d] FactorRatePerPage[%f]\n",cPageCount5perTrans,cDrumPageCount5perTrans,FactorRatePerPage);
		MSG("[IPC] cPageCount5perTrans[%d] cDrumPageCount5perTrans[%d] Toner_Count_Of_Page[%d]\n",cPageCount5perTrans,cDrumPageCount5perTrans,Toner_Count_Of_Page);
#endif		
		fToner_Count_Cmd = NOT_ACTIVE;
		fSmartICWrite_DotCount = TRUE;
//		fSmartICWrite_DrumDotCount = TRUE;/* [KSR032913] */
/*		
		if(FactorRatePerPage <= 1.24) fFactor_Page = 0;
		else if((FactorRatePerPage >= 1.25) && (FactorRatePerPage <= 2.4)) fFactor_Page = 1;
		else if((FactorRatePerPage >= 2.5) && (FactorRatePerPage <= 4.24)) fFactor_Page = 2;
		else if((FactorRatePerPage >= 4.25) && (FactorRatePerPage <= 7.44)) fFactor_Page = 3;
		else if((FactorRatePerPage >= 7.45) && (FactorRatePerPage <= 13.12)) fFactor_Page = 4;
		else if((FactorRatePerPage >= 13.13) && (FactorRatePerPage <= 25.73)) fFactor_Page = 5;
		else if((FactorRatePerPage >= 25.74) && (FactorRatePerPage <= 49)) fFactor_Page = 6;
		else if(FactorRatePerPage >= 50) fFactor_Page = 7;
		else {}
*/
		if(Toner_Count_Of_Page < NUM_DOTS_1P25PERCENT) fFactor_Page = 0;
		else if(Toner_Count_Of_Page < NUM_DOTS_2P5PERCENT) fFactor_Page = 1;
		else if(Toner_Count_Of_Page < NUM_DOTS_4P25PERCENT) fFactor_Page = 2;
		else if(Toner_Count_Of_Page < NUM_DOTS_10PERCENT) fFactor_Page = 3;
		else if(Toner_Count_Of_Page < NUM_DOTS_50PERCENT) fFactor_Page = 4;
		else fFactor_Page = 5;	
		
		(Factor_Page[fFactor_Page])++;
		(Drum_Factor_Page[fFactor_Page])++;
		MSG("(eng)- Factor_Page[%d] = %d\n", fFactor_Page, Factor_Page[fFactor_Page]);
		MSG("(eng)- Drum_Factor_Page[%d] = %d\n", fFactor_Page, Drum_Factor_Page[fFactor_Page]);
		SmartICWrite_TonerHisFactorRateADDRESS = fFactor_Page + SC_TONER_HIS_FACTOR_PERCENT_1_ADDR;
		SmartICWrite_TonerHisFactorRate = Factor_Page[fFactor_Page];
		SmartICWrite_DrumHisFactorRate = Drum_Factor_Page[fFactor_Page];
		fSmartICWrite_TonerHisFactorRate = TRUE;
		fSmartICWrite_DrumHisFactorRate = TRUE;/* [KSR032913] - end */
		
		MESSAGE message;
		
		message.msgType = MSG_CARTRIDGE_PRINTED_BY_COVERAGE_INFO;
		message.param3 = (int*)Factor_Page;
		
		SYMsgSend(PRINTMGRID, &message);

		MSG("<fToner_Count_Cmd> TonerNearEnd : Near End (5p:%d >= (TCC:%d - TNV:%d)) fState_TonerNearEnd:%d\n",cPageCount5perTrans,TonerCartridgeCapacity,TonerNearEndValue,fState_TonerNearEnd);
#if SMART_IC_LOCK_ENABLE			
		if((cPageCount5perTransOnChip >= (TonerCartridgeCapacity - TonerNearEndValue)) && (fState_TonerNearEnd == CLEAR))
#else
		if((cPageCount5perTrans >= (TonerCartridgeCapacity - TonerNearEndValue)) && (fState_TonerNearEnd == CLEAR))
#endif
		{
#if KSR_DBG_ERR
#if SMART_IC_LOCK_ENABLE
			MSG("(Eng)## TonerNearEnd : Near End (5p:%d >= (TCC:%d - TNV:%d))\n",cPageCount5perTransOnChip,TonerCartridgeCapacity,TonerNearEndValue);
#else
			MSG("(Eng)## TonerNearEnd : Near End (5p:%d >= (TCC:%d - TNV:%d))\n",cPageCount5perTrans,TonerCartridgeCapacity,TonerNearEndValue);
#endif
#endif			
			fSmartICWrite_TonerNearEnd = TRUE;
		}
#if SMART_IC_LOCK_ENABLE			
		else if((cPageCount5perTransOnChip >= TonerCartridgeCapacity) && (fState_TonerEnd == CLEAR))
#else
		else if((cPageCount5perTrans >= TonerCartridgeCapacity) && (fState_TonerEnd == CLEAR))
#endif
		{
#if KSR_DBG_ERR
#if SMART_IC_LOCK_ENABLE
			MSG("(Eng)## Toner End : End (5p:%d >= TCC:%d)\n",cPageCount5perTransOnChip, TonerCartridgeCapacity);
#else
			MSG("(Eng)## Toner End : End (5p:%d >= TCC:%d)\n",cPageCount5perTrans, TonerCartridgeCapacity);
#endif
#endif
			fSmartICWrite_TonerEnd = TRUE;
		}
#if SMART_IC_LOCK_ENABLE
		else if((cPageCount5perTransOnChip >= (TonerCartridgeCapacity + TonerBlockValue)) && (fState_TonerBlock == CLEAR) && (fCartridgeRecycleType != NON_PREBATE_TYPE))
#else
		else if((cPageCount5perTrans >= (TonerCartridgeCapacity + TonerBlockValue)) && (fState_TonerBlock == CLEAR) && (fCartridgeRecycleType != NON_PREBATE_TYPE))
#endif
		{
#if KSR_DBG_ERR
#if SMART_IC_LOCK_ENABLE
			MSG("(Eng)## Smart IC Close : Close (5p:%d >= (TCC:%d + TBV:%d))\n",cPageCount5perTransOnChip,TonerCartridgeCapacity,TonerBlockValue);
#else
			MSG("(Eng)## Smart IC Close : Close (5p:%d >= (TCC:%d + TBV:%d))\n",cPageCount5perTrans,TonerCartridgeCapacity,TonerBlockValue);
#endif
#endif /* [KSR032913] */
			fSmartICWrite_TonerBlock = TRUE;
		}
		
		/* Drum Check */
		if((cDrumPageCount5perTrans >= (40000 - 2000)) && (fState_DrumNearEnd == CLEAR))
		{
#if KSR_DBG_ERR
			MSG("(Eng)## DrumNearEnd : Near End ##\n");
#endif			
			fSmartICWrite_DrumNearEnd = TRUE;
		}
		else if((cPageCount5perTrans >= 40000) && (fState_DrumEnd == CLEAR))
		{
#if KSR_DBG_ERR
			MSG("(Eng)## Drum End : End ##\n");
#endif
			fSmartICWrite_DrumEnd = TRUE;
		}
		else if((cPageCount5perTrans >= (40000 + DRUM_BLOCK_VALUE)) && (fState_DrumBlock == CLEAR) && (fCartridgeRecycleType != NON_PREBATE_TYPE))
		{
#if KSR_DBG_ERR
			MSG("(Eng)## Drum Smart IC Close : Close ##\n"); /* [KSR032913] - end */
#endif
			fSmartICWrite_DrumBlock = TRUE;
		}
		
#if KSR_DBG_THREE
		MSG("(Eng)## Toner_Count_Of_Page = %d , FactorRatePerPage = %f ##\n",Toner_Count_Of_Page, FactorRatePerPage);
#if SMART_IC_LOCK_ENABLE
		MSG("(Eng)## 5 percent Chart Trans Page Count = %d, OnChip = %d ##\n",cPageCount5perTrans, cPageCount5perTransOnChip);
#else
		MSG("(Eng)## 5 percent Chart Trans Page Count = %d ##\n",cPageCount5perTrans);
#endif
		//printf("(Eng)## 5 percent Chart Trans Page Count_Drum = %d ##\n",cDrumPageCount5perTrans);
#endif
	}
	
	if(fTotal_Print_Count_Cmd == ACTIVE)	
	{
		cTotal_Print_Count = (Uint32)(Total_Print_Count_Buf[0]<<24) 
		+ (Uint32)(Total_Print_Count_Buf[1]<<16) 
		+ (Uint32)(Total_Print_Count_Buf[2]<<8) 
		+ (Uint32)(Total_Print_Count_Buf[3]);
	
		fTotal_Print_Count_Cmd = NOT_ACTIVE;
#if KSR_DBG_THREE		
		MSG("(Eng)## cTotal_Print_Count = %d ##\n",cTotal_Print_Count);
#endif
		//printf("(Eng)## cTotal_Print_Count = %d ##\n",cTotal_Print_Count);		
	}
	
	if(fQuery_Engine_Software_Info_Cmd == ACTIVE)
	{
		Set_EngineVersionInfo();
		fQuery_Engine_Software_Info_Cmd = NOT_ACTIVE;
	}
#if 0 /* [KSR032501] for SMC */
	if(fMagnification_Chg_Cmd == ACTIVE)
	{
		MotorSpeedparam = Get_VerticalMagValue();
		MirrorMotorSpeedparam = Get_HorizontalMagValue();
		MainMotorSpeedChange(MotorSpeedparam, 0);
		MirrorMotorSpeedChange(MirrorMotorSpeedparam, 0);
		fMagnification_Chg_Cmd = NOT_ACTIVE;
	}
#endif
#if 0	
	if(fSensor_Test_Start_Cmd == ACTIVE)
	{
		Check_Sensor_Status();
		fSensor_Test_Start_Cmd = NOT_ACTIVE;
	}
	
	if(fSensor_Test_End_Cmd == ACTIVE)
	{
		Sensor_Test_Off();
		fSensor_Test_End_Cmd = NOT_ACTIVE;
	}
	
	if(fSolenoid_Test_Start_Cmd == ACTIVE)
	{
#if KSR_DBG_THREE
		MSG("(Eng)## SP MODE : Solenoid_Test_Start ##\n");
#endif
		Solenoid_Test();
		fSolenoid_Test_Start_Cmd = NOT_ACTIVE;
	}
	
	if(fSolenoid_Test_End_Cmd == ACTIVE)
	{
		All_Solenoid_Off();
		fSolenoid_Test_End_Cmd = NOT_ACTIVE;
	}
#endif /* [KSR032501] for SMC */
	if(fCold_Wake_Up_Cmd == ACTIVE)
	{
		ColdWakeUpCmd = ACTIVE;
		Sensor_Power_On();
		fCold_Wake_Up_Cmd = NOT_ACTIVE;
	}
}

void Save_PrintPageParameter(void)
{
	PendingPrintPageData[0] = Temp_PageMsg.temp_PageID;
	PendingPrintPageData[1] = Temp_PageMsg.temp_Source;
	PendingPrintPageData[2] = Temp_PageMsg.temp_Narrow;
	if(Temp_PageMsg.temp_MediaDescription >= TransferSpecNumber)
	{
		// Not defined media : Set as plain paper
#if KSR_DBG_THREE
		MSG("(Eng)## Not defined media type (temp_MediaDescription=%d) : Set as plain paper ##\n",Temp_PageMsg.temp_MediaDescription);
#endif
		Temp_PageMsg.temp_MediaDescription=0x05;
	}
	PendingPrintPageData[3] = Temp_PageMsg.temp_MediaDescription;
	PendingPrintPageData[4] = Temp_PageMsg.temp_Length;
	PendingPrintPageData[5] = Temp_PageMsg.temp_Darkness;
	PendingPrintPageData[6] = Temp_PageMsg.temp_TonerSaver;
	PendingPrintPageData[7] = Temp_PageMsg.temp_OutputDestination;
	PendingPrintPageData[8] = Temp_PageMsg.temp_cheat_gap;
	
//	fPendingPrintPageCmd = SET;

#if KSR_DBG_THREE	
	MSG("(Eng)=========================================\n");
	// MSG("(Eng)Print Start CMD is received \n");
	MSG("(Eng)PageID  = %d\n",PendingPrintPageData[0]);
	MSG("(Eng)Source  = 0x%x\n",PendingPrintPageData[1]);
	MSG("(Eng)Narrow  = 0x%x\n",PendingPrintPageData[2]);
	MSG("(Eng)Media  = 0x%x\n",PendingPrintPageData[3]);
	MSG("(Eng)Length  = 0x%x <%d>\n",PendingPrintPageData[4], PaperLength[fMotorSpeedMode][PendingPrintPageData[4]]);
	MSG("(Eng)Darkness  = 0x%x\n",PendingPrintPageData[5]);
	// MSG("(Eng)TonerSaver  = 0x%x\n",PendingPrintPageData[6]);
	MSG("(Eng)OutputDest  = 0x%x\n",PendingPrintPageData[7]);
	MSG("(Eng)CheatGap  = 0x%x\n",PendingPrintPageData[8]);
	MSG("(Eng)=========================================\n");	
#endif
	
	if((PendingPrintPageData[1] == MS_Tray2) && (f2ndDrawerState == CLEAR))
	{
		PendingPrintPageData[1] = MS_Tray1;
#if KSR_DBG_THREE
		MSG("(Eng)Source Change = 0x%x (Tray2 is not detacted)\n",PendingPrintPageData[1]);
		MSG("(Eng)=========================================\n");		
#endif		
	}
}
