/*+---------------------------------------------------------------------------+
 ** 								    								
 **	 File Name :	tray2_thread.c
 **	 Revision  :	0.1
 **	 Date	   : 	April 29, 2009
 **	 Author    :	Youngjin, Lee
 ** 
 +---------------------------------------------------------------------------+*/

/********************************************************************
*  INCLUDES
********************************************************************/
#include 	<stdio.h>
#include 	<sys/ioctl.h>
#include	<time.h>
#include	<unistd.h>
#include	"i2c_interface.h"

#include	"farch.h"
#include	"em.h"
#include	"i2c_protocol.h"

#define DBG_PRFX "engi2c: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */

Uint8 Tray2Status;
Uint32 cTimerStart_I2C;

//extern int i2c_dev;
Uint8 fSmartICWrite_PageCount;
Uint8 fSmartICWrite_PageCountInRun;
Uint8 fSmartICWrite_PageCountFailInRun;
Uint8 fSmartICWrite_DotCount;
Uint8 fSmartICWrite_DotCountInRun;
Uint8 fSmartICWrite_DotCountFailInRun;

Uint8 fSmartICWrite_DrumPageCount; /* [KSR032914] */
Uint8 fSmartICWrite_DrumPageCountInRun;
Uint8 fSmartICWrite_DrumPageCountFailInRun;
Uint8 fSmartICWrite_DrumDotCount;
Uint8 fSmartICWrite_DrumDotCountInRun;
Uint8 fSmartICWrite_DrumDotCountFailInRun;/* [KSR032914]-end */

Uint8 fSmartICWrite_UsedCheck;
Uint8 fSmartICWrite_TonerNearEnd;
Uint8 fSmartICWrite_DrumNearEnd;/* [KSR032914] */
Uint8 fSmartICWrite_TonerEnd;
Uint8 fSmartICWrite_DrumEnd;/* [KSR032914] */
Uint8 fSmartICWrite_TonerBlock;
Uint8 fSmartICWrite_DrumBlock;/* [KSR032914] */
Uint8 fSmartICWrite_TonerHistoy;
Uint8 fSmartICWrite_DrumHistoy;/* [KSR032914] */
Uint8 fSmartICWrite_FreeMotorOn;
Uint8 fSmartICWrite_DrumFreeMotorOn;
Uint8 fSmartICWrite_TonerHisFactorRate;
Uint8 fSmartICWrite_DrumHisFactorRate;
Uint8 fSmartICWrite_TonerHisJob;
Uint8 fSmartICWrite_TonerHisJob_Total;
Uint8 fSmartICWrite_DrumHisJob;
Uint8 fSmartICWrite_DrumHisJob_Total;/* [KSR032914]-end */
Uint8 fSmartICWrite_Dummy;
Uint8 fSmartICWrite_WrongCount;
Uint8 fSmartICWrite_DotCountBackup;
Uint8 fSmartICWrite_DrumDotCountBackup;/* [KSR032914] */
Uint8 fSmartICWrite_TonerCheck;
Uint8 fStore_TonerBlockState;
Uint8 fSC_Writefault;
Uint32 CurrentDotCountOnChip;
Uint8 cJobEndDotCountWriteFailCnt;

char TxData[128];
char RxData[128];
extern ST_FLAG 	PaperEmpty;
extern Uint8 f2ndDrawerState;
extern Uint32 cPageCount;
extern Uint32 cDrumPageCount;/* [KSR032914] */
extern Uint32 cDrumRest;/* [KSR032914] */
extern Uint8 SC_State;
extern Uint8 fMemoryWrite;
extern Uint32 cFreeMotorOn;/* [KSR032914] */
extern Uint32 cDrumFreeMotorOn;/* [KSR032914] */

extern Uint16 SmartICWrite_TonerHisJobADRESS;/* [KSR032914] */
extern Uint32 SmartICWrite_TonerHisJob;
extern Uint32 SmartICWrite_DrumHisJob;
extern Uint16 SmartICWrite_TonerHisFactorRateADDRESS;
extern Uint32 SmartICWrite_TonerHisFactorRate;
extern Uint32 SmartICWrite_DrumHisFactorRate;/* [KSR032914]-end */

extern Uint32 cDrumDotCount;
extern Uint32 cDotCount;
extern Uint32 Toner_Count_Of_Page;

extern Uint8 fState_TonerNearEnd;
extern Uint8 fState_DrumNearEnd;/* [KSR032914] */
extern Uint8 fState_TonerEnd;
extern Uint8 fState_DrumEnd;/* [KSR032914] */
extern Uint8 fState_TonerBlock;
extern Uint8 fState_DrumBlock;/* [KSR032914] */
extern Uint8 machineType;
extern Uint8 machineLocation;
extern Uint8 fDataEncrytionMode;

extern unsigned int Factor_Page[NUM_TONER_FACTOR];/* [KSR032914] */
extern unsigned int Mode_Job[NUM_TONER_JOB];
extern unsigned int Drum_Factor_Page[NUM_TONER_FACTOR];
extern unsigned int Drum_Mode_Job[NUM_TONER_JOB];/* [KSR032914]-end */

extern Uint8 Toner_Remain_Percentage_New;
extern Uint8 Toner_Remain_Percentage_Old;

extern Uint8 fDrum;
extern int 	cProcessCounter;
extern Uint32 	cTotal_Print_Count;
void Set_Timer_I2C(void);
int Get_Elapse_I2C(void);

Uint8 Tray2_Version_Major;
Uint8 Tray2_Version_Minor;
Uint8 TRAY2_STATE;

void *SmartIC_Read(void *arg);
Uint8 CheckSum_Check(int);

extern Uint16 CodeOfMachineCheck;
extern ST_FLAG EngineFatalError1;
extern Uint8 MemoryWrite(Uint16, Uint32, Uint8);/* [KSR032914] */
extern Uint32 MemoryRead(Uint16, Uint8);/* [KSR032914] */
extern int Set_SmartICInfoNotify(Uint8, Uint8);
extern int Set_EngineVersionInfo(void);/* [KSR032901] */
#if SMART_IC_LOCK_ENABLE
extern Uint8 MemoryLockWrite(Uint16);
extern Uint32 MemoryWriteEnc(Uint16, Uint32);
extern Uint32 MemoryReadEncGet(Uint16);

extern TONERCHECK gTonerCheckinfo;
#endif

/********************************************************************
*  FUNCTIONS
********************************************************************/

void *SmartIC_Read(void *arg) /* [KSR032501] for SMC */
{
#if SECURITY_IC_ATTACHED	
	setlinebuf(stdout);
	Uint8 cMemoryWriteFailCnt = 0;
#if SMART_IC_WRITE
	Uint32 cDotCountBackup = 0;
#if 0 /* only toner */
	Uint32 cDrumDotCountBackup = 0; /* [KSR032914] */
#endif
	Uint32 tempWriteValue;
#if SMART_IC_LOCK_ENABLE
	Uint32 tempDotCount = 0;
#endif
#endif
	Uint32 tempReadValue = 0;
	cJobEndDotCountWriteFailCnt = 0;
	
//	Uint32 tempWriteValue2; /* [KSR032601] for SMC only Test */	

//	printf("[SMC] SMC Thread (SmartIC_Read)\n"); /* [KSR032501] for SMC thread */

	while(1)
	{
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE
		fSmartICWrite_PageCount = CLEAR;
		fSmartICWrite_PageCountInRun = CLEAR;
		fSmartICWrite_PageCountFailInRun = CLEAR;
		fSmartICWrite_DotCount = CLEAR;
		fMemoryWrite = CLEAR;
		fSmartICWrite_DotCountInRun = CLEAR;
		fSmartICWrite_DotCountFailInRun = CLEAR;
		fSmartICWrite_DotCountBackup = CLEAR;
		fSC_Writefault = CLEAR;
		fSmartICWrite_TonerNearEnd = CLEAR;
		fSmartICWrite_TonerEnd = CLEAR;
		fSmartICWrite_TonerBlock = CLEAR;
		fSmartICWrite_TonerHistoy = CLEAR;
		fSmartICWrite_FreeMotorOn = CLEAR;
		fSmartICWrite_TonerHisFactorRate = CLEAR;
		fSmartICWrite_TonerHisJob = CLEAR;
		fSmartICWrite_TonerHisJob_Total = CLEAR;
#if SMART_IC_LOCK_ENABLE		
		fSmartICWrite_WrongCount = CLEAR;
#endif
#endif
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE
		if(0/*(fSmartICWrite_PageCount == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_PageCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC
//			printf("(T) [%d] [%d] [%d]\n",MemoryRead(SC_PAGE_COUNT_ADDR, TONER), cPageCount, cMemoryWriteFailCnt);
			if((MemoryRead(SC_PAGE_COUNT_ADDR, TONER) > cPageCount) || cMemoryWriteFailCnt >= 2)/* [KSR032914] */
			{
				fSmartICWrite_PageCount = FALSE;
				if((fSmartICWrite_PageCountInRun != TRUE) || ((fSmartICWrite_PageCountFailInRun == TRUE) && (fSmartICWrite_PageCountInRun == TRUE)))
				{
					//fSC_Writefault = TRUE;
					//SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
					MSG("(T)## ERROR cPageCount !!!##\n");
#endif	
				}
				else
				{
					fSmartICWrite_PageCountFailInRun = TRUE;
#if	ENG_TRACE					
					MSG("(T)## ERROR cPageCount in Run !!!##\n");
#endif
				}
				fSmartICWrite_PageCountInRun = FALSE;
				cMemoryWriteFailCnt = 0;			
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_ADDR,cPageCount, TONER) != 0)/* [KSR032914] */
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_ADDR, TONER); /* [KSR032914] */
#if KSR_DBG_ONE
				MSG("(T)## MemoryRead(PageCount : Address : %d)  Read <%d> PageCount:%d [%d] ##\n",SC_PAGE_COUNT_ADDR, tempWriteValue, cPageCount, cProcessCounter); /* [KSF050806] */
#endif				
				if(tempWriteValue == cPageCount)
				{
					fSmartICWrite_PageCount = FALSE;
					fSmartICWrite_PageCountInRun = FALSE;
					fSmartICWrite_PageCountFailInRun = FALSE;
					cMemoryWriteFailCnt = 0;
#if KSR_DBG_ONE			
					MSG("(T)## MemoryWrite(PageCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_ADDR, cPageCount, cProcessCounter); /* [KSF050806] */
#endif						
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif
		}
#endif /* USE_TONER_IC - end */
		
		/* [KSR032914] DRUM */
//#ifdef USE_DRUM_IC
#if 0
		if((fSmartICWrite_DrumPageCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
			if((MemoryRead(SC_PAGE_COUNT_ADDR, DRUM) > cDrumPageCount) || cMemoryWriteFailCnt >= 2)
			{
				fSmartICWrite_DrumPageCount = FALSE;
				if(fSmartICWrite_DrumPageCountInRun != TRUE)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
					MSG("(D)## ERROR cDrumPageCount !!!##\n");
#endif	
				}
				else
				{
					fSmartICWrite_DrumPageCountFailInRun = TRUE;
#if	ENG_TRACE					
					MSG("(D)## ERROR cDrumPageCount in Run !!!##\n");
#endif
				}
				fSmartICWrite_DrumPageCountInRun = FALSE;
				cMemoryWriteFailCnt = 0;			
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_ADDR,cDrumPageCount, DRUM) != 0)
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_ADDR, DRUM);
				MSG("(D)## MemoryRead(DrumPageCount : Address : %d)  Read <%d> DrumDotCnt <%d> [%d] ##\n",SC_PAGE_COUNT_ADDR, tempWriteValue, cDrumPageCount, cProcessCounter);
				if(tempWriteValue == cDrumPageCount)
				{
					fSmartICWrite_DrumPageCount = FALSE;
					fSmartICWrite_DrumPageCountInRun = FALSE;
					fSmartICWrite_DrumPageCountFailInRun = FALSE;
					cMemoryWriteFailCnt = 0;
#if	ENG_TRACE			
					MSG("(D)## MemoryWrite(DrumPageCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_ADDR, cDrumPageCount, cProcessCounter);
#endif						
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif
		}/* [KSR032914]-end */
#endif /* USE_DRUM_IC - end */	
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE
		if(0/*(fSmartICWrite_DotCount == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME) && (fMemoryWrite==TRUE))
#else
		if((fSmartICWrite_DotCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME) && (fMemoryWrite==TRUE))
#endif
		{
#ifdef USE_TONER_IC

#if KSR_DBG_ONE
//			printf("(T) [%d] [%d] [%d]\n",MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, TONER), cDotCount, cMemoryWriteFailCnt); /* [KSF050806] */
#endif
			tempReadValue = (MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, TONER));
			if(((tempReadValue > cDotCount) && (tempReadValue != 0xFFFFFFFF)) || cMemoryWriteFailCnt >= 2)/* [KSR032914] */
			{
				fSmartICWrite_DotCount = FALSE;
				fMemoryWrite = FALSE;
				if((fSmartICWrite_DotCountInRun != TRUE) || ((fSmartICWrite_DotCountFailInRun == TRUE) && (fSmartICWrite_DotCountInRun == TRUE)))
				{
					if(cJobEndDotCountWriteFailCnt >= 2)
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
						MSG("(Eng)## ERROR 3 times DotCount write fail!!! ##\n");
#endif			
					}
					else
					{
						cJobEndDotCountWriteFailCnt++;
					}
#if	ENG_TRACE					
					MSG("(Eng)## ERROR cDotCount (%d) !!! ##\n", cJobEndDotCountWriteFailCnt);
#endif	
				}
				else
				{
					fSmartICWrite_DotCountFailInRun = TRUE;
#if	ENG_TRACE					
					MSG("(T)## ERROR cDotCount in Run !!! <%d>##\n", cProcessCounter);
#endif	
				}
				fSmartICWrite_DotCountInRun = FALSE;
				cMemoryWriteFailCnt = 0;				
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_ADDR,cDotCount, TONER) != 0)/* [KSR032914] */
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				extern Uint32 cPageCount5perTrans;
				extern Uint8 Toner_Used_Percentage;
				extern Uint32 TonerCartridgeCapacity;
				
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, TONER);/* [KSR032914] */
#if KSR_DBG_ONE				
				MSG("(T)## MemoryRead(DotCount : Address : %d)  Read <%d> DotCnt <%d> [%d] <%d> ##\n",SC_PAGE_COUNT_CONVERSION_ADDR, tempWriteValue, cDotCount, cProcessCounter, cPageCount5perTrans); /* [KSF050806] */
#endif
				if(tempWriteValue == cDotCount)
				{
					MESSAGE message;
					
					slogit( message.msgType = MSG_STATUS_TONER );
					cPageCount5perTrans = cDotCount / TRANS_COEFFICIENT_5PERCENT;
					message.param1 = cPageCount5perTrans; 					
		
					Toner_Used_Percentage = (cDotCount / ((TonerCartridgeCapacity * TRANS_COEFFICIENT_5PERCENT)/100));
					if(Toner_Used_Percentage >= 100)
					{
						message.param2 = 0;
					}
					else
					{
						message.param2 = 100 - Toner_Used_Percentage;
					}
					Toner_Remain_Percentage_New = message.param2;
					
					SYMsgSend(PRINTMGRID, &message);
					
					if(Toner_Remain_Percentage_Old != Toner_Remain_Percentage_New)
					{
						if(((Toner_Remain_Percentage_Old >= 91) && (Toner_Remain_Percentage_New <= 90)) ||
						((Toner_Remain_Percentage_Old >= 71) && (Toner_Remain_Percentage_New <= 70)) ||
						((Toner_Remain_Percentage_Old >= 51) && (Toner_Remain_Percentage_New <= 50)) ||
						((Toner_Remain_Percentage_Old >= 31) && (Toner_Remain_Percentage_New <= 30)) 
						)
						{
							// System Service에서 토너 잔량 확인 할 것!!! 
							slogit( message.msgType = MSG_STATUS_TONER_REMAIN_CHK_REQ );
							SYMsgSend(PRINTMGRID, &message);
						}
					}
					Toner_Remain_Percentage_Old = Toner_Remain_Percentage_New;
		
					fMemoryWrite = FALSE;
					fSmartICWrite_DotCount = FALSE;
					fSmartICWrite_DotCountInRun = FALSE;
					fSmartICWrite_DotCountFailInRun = FALSE;
					cMemoryWriteFailCnt = 0;
					cJobEndDotCountWriteFailCnt = 0;
					fSmartICWrite_DotCountBackup = TRUE;
					cDotCountBackup = cDotCount;
					CurrentDotCountOnChip = cDotCount;
#if KSR_DBG_ONE			
					MSG("(T)## MemoryWrite(DotCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_CONVERSION_ADDR, cDotCount, cProcessCounter); /* [KSF050806] */
#endif	/* [KSR032914]-start */			
#if SMART_IC_LOCK_ENABLE					
//					if(machineLocation == LOCAT_KOREA)
					if(1)
					{
						tempDotCount = cDotCount / TRANS_COEFFICIENT_5PERCENT;
					
						if(tempDotCount >= 13000) 
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_13K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_13K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_13K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 13000 ##\n");
							}
						}
						else if(tempDotCount >= 12000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_12K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_12K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_12K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 12000 ##\n");
							}
						}
						else if(tempDotCount >= 11000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_11K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_11K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_11K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 11000 ##\n");
							}
						}
						else if(tempDotCount >= 10000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_10K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_10K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_10K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 10000 ##\n");
							}
						}
						else if(tempDotCount >= 9000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_9K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_9K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_9K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 9000 ##\n");
							}
						}
						else if(tempDotCount >= 8000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_8K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_8K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_8K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 8000 ##\n");
							}
						}
						else if(tempDotCount >= 7000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_7K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_7K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_7K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 7000 ##\n");
							}
						}
						else if(tempDotCount >= 6000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_6K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_6K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_6K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 6000 ##\n");
							}
						}
						else if(tempDotCount >= 5000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_5K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_5K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_5K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 5000 ##\n");
							}
						}
						else if(tempDotCount >= 4000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_4K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_4K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_4K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 4000 ##\n");
							}
						}
						else if(tempDotCount >= 3000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_3K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_3K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_3K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 3000 ##\n");
							}
						}
						else if(tempDotCount >= 2000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_2K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_2K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_2K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 2000 ##\n");
							}
						}
						else if(tempDotCount >= 1000)
						{
							if(!(gTonerCheckinfo.f_TonerCheck & fTonerUsed_1K))
							{
								gTonerCheckinfo.addr_TonerCheckWrite = gTonerCheckinfo.addr_TonerCheckPoint[TonerUsed_1K];
								gTonerCheckinfo.TonerUsedCheckData = fTonerUsed_1K;
								fSmartICWrite_TonerCheck = TRUE;
								MSG("(Eng)## Smart IC : 1000 ##\n");
							}
						}
					}
#endif
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
		
		if((fSmartICWrite_DrumDotCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME) && (fMemoryWrite==TRUE))
		{
//#ifdef USE_DRUM_IC
#if 0
			if((MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, DRUM) > cDrumDotCount) || cMemoryWriteFailCnt >= 2)
			{
				fSmartICWrite_DrumDotCount = FALSE;
				fMemoryWrite = FALSE;
				if(fSmartICWrite_DrumDotCountInRun != TRUE)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
					MSG("(D)## ERROR cDrumDotCount !!!##\n");
#endif					
				}
				else
				{
					fSmartICWrite_DrumDotCountFailInRun = TRUE;
#if	ENG_TRACE					
					MSG("(D)## ERROR cDrumDotCount in Run !!!##\n");
#endif	
				}
				fSmartICWrite_DrumDotCountInRun = FALSE;
				cMemoryWriteFailCnt = 0;				
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_ADDR,cDrumDotCount, DRUM) != 0)
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, DRUM);
				MSG("(D)## MemoryRead(DrumDotCount : Address : %d)  Read <%d> DrumDotCnt <%d> [%d] ##\n",SC_PAGE_COUNT_CONVERSION_ADDR, tempWriteValue, cDrumDotCount, cProcessCounter);
				if(tempWriteValue == cDrumDotCount)
				{
					fMemoryWrite = FALSE;
					fSmartICWrite_DrumDotCount = FALSE;
					fSmartICWrite_DrumDotCountInRun = FALSE;
					fSmartICWrite_DrumDotCountFailInRun = FALSE;
					cMemoryWriteFailCnt = 0;
					fSmartICWrite_DrumDotCountBackup = TRUE;
					cDrumDotCountBackup = cDrumDotCount;
#if	ENG_TRACE			
					MSG("(D)## MemoryWrite(DrumDotCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_CONVERSION_ADDR, cDrumDotCount, cProcessCounter);/* [KSR032914]-end */
#endif						
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE		
		if(0/*(fSmartICWrite_DotCountBackup == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_DotCountBackup == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			if(cMemoryWriteFailCnt >= 2)
			{
				fSmartICWrite_DotCountBackup = FALSE;
				cMemoryWriteFailCnt = 0;
				fSC_Writefault = TRUE;
				SC_State = ST_CERT_FAILULE;
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR,cDotCountBackup, TONER) != 0)/* [KSR032914] */
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, TONER);/* [KSR032914] */
#if KSR_DBG_ONE
				MSG("(T)## MemoryRead(DotCount_Backup : Address : %d)  Read <%d> DotCnt <%d> [%d] ##\n",SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, tempWriteValue, cDotCountBackup, cProcessCounter); /* [KSF050806] */
#endif
				if(tempWriteValue == cDotCountBackup)
				{
					fSmartICWrite_DotCountBackup = FALSE;
					cMemoryWriteFailCnt = 0;
					fSmartICWrite_Dummy = TRUE;
#if KSR_DBG_ONE			
					MSG("(T)## MemoryWrite(DotCount_Backup : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, cDotCountBackup, cProcessCounter); /* [KSF050806] */
#endif	/* [KSR032914]-start */					
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif

#endif /* USE_TONER_IC - end */		
		}
		
		if((fSmartICWrite_DrumDotCountBackup == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0
			if(cMemoryWriteFailCnt >= 2)
			{
				fSmartICWrite_DrumDotCountBackup = FALSE;
				cMemoryWriteFailCnt = 0;
				fSC_Writefault = TRUE;
				SC_State = ST_CERT_FAILULE;
				continue;
			}
			Set_Timer_I2C();
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR,cDrumDotCountBackup, DRUM) != 0)
			{
				cMemoryWriteFailCnt++; 
			}
			else
			{
				usleep(30000);
				tempWriteValue = MemoryRead(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, DRUM);
				MSG("(D)## MemoryRead(DrumDotCount_Backup : Address : %d)  Read <%d> DrumDotCnt <%d> [%d] ##\n",SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, tempWriteValue, cDrumDotCountBackup, cProcessCounter);
				if(tempWriteValue == cDrumDotCountBackup)
				{
					fSmartICWrite_DrumDotCountBackup = FALSE;
					cMemoryWriteFailCnt = 0;
					fSmartICWrite_Dummy = TRUE;
#if	ENG_TRACE			
					MSG("(D)## MemoryWrite(DrumDotCount_Backup : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, cDrumDotCountBackup, cProcessCounter); /* [KSR032914]-end */
#endif						
				}
				else
				{
					cMemoryWriteFailCnt++; 
				}		
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */		
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_Dummy == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_Dummy == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_Dummy = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_DUMMY_ADDR,0, TONER) != 0) /* [KSR032914] */
			{
#if	ENG_TRACE			
				MSG("(T)## Dummy Write Fail ##\n");
#endif				
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(Dummy : Address : %d) = %d [%d] ##\n",SC_DUMMY_ADDR, 0, cProcessCounter);
#endif		
			}
#endif
#endif

#endif /* USE_TONER_IC - end */			
		}
		// if((fSmartICWrite_PageCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		// {
			// Set_Timer_I2C();
			// fSmartICWrite_PageCount = FALSE;
// #if SECURITY_IC_ATTACHED
// #if SMART_IC_WRITE
			// if(MemoryWrite(SC_PAGE_COUNT_ADDR,cPageCount) !=8)
			// {
// #if	ENG_TRACE					
				// printf("(Eng)## MemoryWrite Error : PageCount (Retry!!!) ##\n");
// #endif				
				// usleep(30000);
				// if(MemoryWrite(SC_PAGE_COUNT_ADDR,cPageCount) !=8)
				// {
					// if(fSmartICWriteInRun == TRUE)
					// {
						// fMemoryWrite = FALSE;
// #if	ENG_TRACE					
						// printf("(Eng)## MemoryWrite Error In Run : PageCount ##\n");
// #endif							
					// }
					// else
					// {
						// usleep(30000);
						// if(MemoryWrite(SC_PAGE_COUNT_ADDR,cPageCount) !=8)
						// {
							// fSC_Writefault = TRUE;
							// SC_State = ST_CERT_FAILULE;
// #if	ENG_TRACE					
							// printf("(Eng)## MemoryWrite Error : PageCount ##\n");
// #endif						
						// }
					// }
				// }
				// else
				// {
// #if	ENG_TRACE			
					// printf("(Eng)## 2nd try MemoryWrite(PageCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_ADDR, cPageCount, cProcessCounter);
// #endif							
				// }
			// }
			// else
			// {
// #if	ENG_TRACE			
				// printf("(Eng)## MemoryWrite(PageCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_ADDR, cPageCount, cProcessCounter);
// #endif			
			// }
			// fSmartICWriteInRun = FALSE;
// #endif
// #endif
		// }
	
		// if((fSmartICWrite_DotCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME) && (fMemoryWrite==TRUE))
		// {
			// Set_Timer_I2C();
			// fSmartICWrite_DotCount = FALSE;
			// fMemoryWrite = FALSE;
// #if SECURITY_IC_ATTACHED
// #if SMART_IC_WRITE
			// if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_ADDR,cDotCount) !=8)
			// {
// #if	ENG_TRACE					
				// printf("(Eng)## MemoryWrite Error : DotCount (Retry!!!) ##\n");
// #endif				
				// usleep(30000);
				// if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_ADDR,cDotCount) !=8)
				// {
					// fSC_Writefault = TRUE;
					// SC_State = ST_CERT_FAILULE;

// #if	ENG_TRACE					
					// printf("(Eng)## MemoryWrite Error : DotCount ##\n");
// #endif
				// }
			// }
			// else
			// {
// #if	ENG_TRACE			
				// printf("(Eng)## MemoryWrite(DotCount : Address : %d) = %d [%d] ##\n",SC_PAGE_COUNT_CONVERSION_ADDR, cDotCount, cProcessCounter);
// #endif			
			// }
// #endif
// #endif
		// }

#if SMART_IC_LOCK_ENABLE		
//		if(machineLocation == LOCAT_KOREA)
		if(1)
		{
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
			if((0/*fSmartICWrite_WrongCount == TRUE*/) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
			if((fSmartICWrite_WrongCount == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
			{
				Set_Timer_I2C();
				fSmartICWrite_WrongCount = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
				if(MemoryWrite(SC_WRONG_TONER_COUNT_CHK_ADDR, 1, TONER) !=0)
				{
#if	ENG_TRACE			
					MSG("(Eng)## TONER_COUNT_CHK Write Fail ##\n");
#endif				
				}
				else
				{
#if	ENG_TRACE			
					MSG("(Eng)## MemoryWrite(TONER_COUNT_CHK : Address : %d) = %d [%d] ##\n",SC_WRONG_TONER_COUNT_CHK_ADDR, 1, cProcessCounter);
#endif		
				}
#endif
#endif			
			}
		}
#endif
		
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_UsedCheck == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_UsedCheck == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{	
			Set_Timer_I2C();
			fSmartICWrite_UsedCheck = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_NEW_INFO_ADDR,SC_USED_TONER_NUM, TONER) != 0) /* [KSR032914] */
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : UsedCheck (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_NEW_INFO_ADDR,SC_USED_TONER_NUM, TONER) != 0)/* [KSR032914] */
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(Eng)## MemoryWrite Error : UsedCheck ##\n");
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(USED_TONER : Address : %d) = %d [%d] ##\n",SC_NEW_INFO_ADDR, SC_USED_TONER_NUM, cProcessCounter);
#endif
				usleep(30000);
				if(MemoryWrite(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR,0, TONER) != 0)/* [KSR032914] */
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : Dot Count Back up CLEAR ##\n");
#endif							
				}
				else
				{
					usleep(30000);
					tempWriteValue = MemoryRead(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, TONER); /* [KSR032914] */
					if(tempWriteValue == 0)
					{
#if	ENG_TRACE					
						MSG("(T)## Dot Count Back up CLEAR!!! ##\n");
#endif							
					}
					else
					{
						MSG("(T)## MemoryWrite Error : Dot Count Back up CLEAR <%d>##\n",tempWriteValue);
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;						
					}			
				}
			}
#endif
#endif
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_TonerNearEnd == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerNearEnd == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC
			Set_Timer_I2C();
			fSmartICWrite_TonerNearEnd = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
#if SMART_IC_LOCK_ENABLE
			if(fDataEncrytionMode == TRUE)
			{
				if(MemoryWriteEnc(SC_V3_TONER_NEAR_END_ADDR, SET) !=0)
				{
	#if	ENG_TRACE					
					MSG("(Eng)## MemoryWrite Error : TONER_NEAR_END (Retry!!!) ##\n");
	#endif				
					usleep(30000);
					if(MemoryWriteEnc(SC_V3_TONER_NEAR_END_ADDR, SET) !=0)
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryWrite Error : TONER_NEAR_END ##\n");
	#endif
					}
				}
				else
				{
					fState_TonerNearEnd = SET;
					fSmartICWrite_TonerHistoy = TRUE;
					Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_LOW);
	#if	ENG_TRACE
					MSG("(Eng)## MemoryWrite(SC_V3_TONER_NEAR_END_ADDR : Address : %d) = %d [%d] ##\n",SC_V3_TONER_NEAR_END_ADDR, SET, cProcessCounter);
	#endif			
					usleep(30000);
					if(MemoryLockWrite(SC_V3_TONER_NEAR_END_ADDR) != 0)
					{
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryLockWrite Error : (Retry!!!) ##\n");
	#endif				
						usleep(30000);
						if(MemoryLockWrite(SC_V3_TONER_NEAR_END_ADDR) != 0)
						{
	#if	ENG_TRACE					
							MSG("(Eng)## MemoryLockWrite Error ##\n");
	#endif
						}
					}
					else
					{
	#if	ENG_TRACE			
						MSG("(Eng)## MemoryLockWrite(SC_V3_TONER_NEAR_END_ADDR : %d) ##\n",SC_V3_TONER_NEAR_END_ADDR);
	#endif
					}
				}
			}
			else
#else
			if(TRUE)
#endif
			{
				if(MemoryWrite(SC_TONER_NEAR_END_ADDR, SET, TONER) != 0) /* [KSR032914] */
				{
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : TONER_NEAR_END (Retry!!!) ##\n");
#endif				
					usleep(30000);
					if(MemoryWrite(SC_TONER_NEAR_END_ADDR, SET, TONER) != 0)/* [KSR032914] */
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
						// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
						MSG("(T)## MemoryWrite Error : TONER_NEAR_END ##\n");
#endif
					}
				}
				else
				{
					fState_TonerNearEnd = SET;
					fSmartICWrite_TonerHistoy = TRUE;
					Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_LOW);
#if	ENG_TRACE
					MSG("(T)## MemoryWrite(TONER_NEAR_END : Address : %d) = %d [%d] ##\n",SC_TONER_NEAR_END_ADDR, SET, cProcessCounter);
#endif			
				}
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_TonerEnd == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerEnd == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_TonerEnd = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
#if SMART_IC_LOCK_ENABLE
			if(fDataEncrytionMode == TRUE)
			{
				if(MemoryWriteEnc(SC_V3_TONER_END_ADDR, SET) !=0)
				{
	#if	ENG_TRACE					
					MSG("(Eng)## MemoryWrite Error : TONER_END (Retry!!!) ##\n");
	#endif				
					usleep(30000);
					if(MemoryWriteEnc(SC_V3_TONER_END_ADDR, SET) !=0)
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
						// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryWrite Error : TONER_END ##\n");
	#endif
					}
				}
				else
				{
					fState_TonerEnd = SET;
					fSmartICWrite_TonerHistoy = TRUE;
					Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_END);
	#if	ENG_TRACE			
					MSG("(Eng)## MemoryWrite(SC_V3_TONER_END_ADDR : Address : %d) = %d [%d] ##\n",SC_V3_TONER_END_ADDR, SET, cProcessCounter);
	#endif			
					if(MemoryLockWrite(SC_V3_TONER_END_ADDR) != 0)
					{
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryLockWrite Error : (Retry!!!) ##\n");
	#endif				
						usleep(30000);
						if(MemoryLockWrite(SC_V3_TONER_END_ADDR) != 0)
						{
	#if	ENG_TRACE					
							MSG("(Eng)## MemoryLockWrite Error ##\n");
	#endif
						}
					}
					else
					{
	#if	ENG_TRACE			
						MSG("(Eng)## MemoryLockWrite(SC_V3_TONER_END_ADDR : %d) ##\n",SC_V3_TONER_END_ADDR);
	#endif
					}
				}
			}
			else
#else
			if(TRUE)
#endif
			{
				if(MemoryWrite(SC_TONER_END_ADDR, SET, TONER) != 0) /* [KSR032914] */
				{
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : TONER_END (Retry!!!) ##\n");
#endif				
					usleep(30000);
					if(MemoryWrite(SC_TONER_END_ADDR, SET, TONER) != 0) /* [KSR032914] */
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
						// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
						MSG("(T)## MemoryWrite Error : TONER_END ##\n");
#endif
					}
				}
				else
				{
					fState_TonerEnd = SET;
					fSmartICWrite_TonerHistoy = TRUE;
					Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_END);
#if	ENG_TRACE			
					MSG("(T)## MemoryWrite(TONER_END : Address : %d) = %d [%d] ##\n",SC_TONER_END_ADDR, SET, cProcessCounter);
#endif			
				}
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}		
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_TonerBlock == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerBlock == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_TonerBlock = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
#if SMART_IC_LOCK_ENABLE
			if(fDataEncrytionMode == TRUE)
			{
				if(MemoryWriteEnc(SC_V3_TONER_BLOCK_ADDR, SET) !=0)
				{
	#if	ENG_TRACE					
					MSG("(Eng)## MemoryWrite Error : TONER_BLOCK (Retry!!!) ##\n");
	#endif				
					usleep(30000);
					if(MemoryWriteEnc(SC_V3_TONER_BLOCK_ADDR, SET) !=0)
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
						// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryWrite Error : TONER_BLOCK ##\n");
	#endif
					}
				}
				else
				{
					fState_TonerBlock = SET;
					fSmartICWrite_TonerHistoy = TRUE;
					fStore_TonerBlockState = SET;
					// SC_State = ST_TONER_BLOCK;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CLOSE_FAILULE,0x00);
	#if	ENG_TRACE			
					MSG("(Eng)## MemoryWrite(SC_V3_TONER_BLOCK_ADDR : Address : %d) = %d [%d] ##\n",SC_V3_TONER_BLOCK_ADDR, SET, cProcessCounter);
	#endif
					usleep(30000);
					if(MemoryLockWrite(SC_V3_TONER_BLOCK_ADDR) != 0)
					{
	#if	ENG_TRACE					
						MSG("(Eng)## MemoryLockWrite Error : (Retry!!!) ##\n");
	#endif				
						usleep(30000);
						if(MemoryLockWrite(SC_V3_TONER_BLOCK_ADDR) != 0)
						{
	#if	ENG_TRACE					
							MSG("(Eng)## MemoryLockWrite Error ##\n");
	#endif
						}
					}
					else
					{
	#if	ENG_TRACE			
						MSG("(Eng)## MemoryLockWrite(SC_V3_TONER_BLOCK_ADDR : %d) ##\n",SC_V3_TONER_BLOCK_ADDR);
	#endif
					}
				}
			}
			else
#else
			if(TRUE)
#endif
			{
				if(MemoryWrite(SC_TONER_BLOCK_ADDR, SET, TONER) != 0) /* [KSR032914] */
				{
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : TONER_BLOCK (Retry!!!) ##\n");
#endif				
					usleep(30000);
					if(MemoryWrite(SC_TONER_BLOCK_ADDR, SET, TONER) != 0) /* [KSR032914] */
					{
						fSC_Writefault = TRUE;
						SC_State = ST_CERT_FAILULE;
						// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
						MSG("(Eng)## MemoryWrite Error : TONER_BLOCK ##\n");
#endif
					}
				}
				else
				{
					fState_TonerBlock = SET; /* [KSR070203] *//* [KSR070404] */
					fSmartICWrite_TonerHistoy = TRUE;
					fStore_TonerBlockState = SET; /* [KSR070203] *//* [KSR070404] */
					// SC_State = ST_TONER_BLOCK;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CLOSE_FAILULE,0x00);
#if	ENG_TRACE			
					MSG("(T)## MemoryWrite(TONER_BLOCK : Address : %d) = %d [%d] ##\n",SC_TONER_BLOCK_ADDR, SET, cProcessCounter);
#endif			
				}
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}		
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_TonerHistoy == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerHistoy == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_TonerHistoy = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, TONER) != 0)/* [KSR032914] */
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : TonerHistoy (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, TONER) != 0) /* [KSR032914] */
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : TonerHistoy ##\n");
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(TonerHistoy : Address : %d) = %d [%d] ##\n",SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
#if SMART_IC_LOCK_ENABLE		
//		if(machineLocation == LOCAT_KOREA)
		if(1)
		{
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE
			if((0/*fSmartICWrite_TonerCheck == TRUE*/) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
			if((fSmartICWrite_TonerCheck == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
			{
				Set_Timer_I2C();
				fSmartICWrite_TonerCheck = FALSE;
			
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
				if(fDataEncrytionMode == TRUE)
				{
					tempWriteValue = MemoryWriteEnc(gTonerCheckinfo.addr_TonerCheckWrite, gTonerCheckinfo.TonerUsedCheckData);
				}
				else
				{
					tempWriteValue = MemoryWrite(gTonerCheckinfo.addr_TonerCheckWrite, gTonerCheckinfo.TonerUsedCheckData, TONER);
				}
				if(tempWriteValue !=0)
				{
#if	ENG_TRACE					
					MSG("(Eng)## MemoryWrite Error : TonerCheck (Retry!!!) ##\n");
#endif				
					usleep(30000);
					if(fDataEncrytionMode == TRUE)
					{
						tempWriteValue = MemoryWriteEnc(gTonerCheckinfo.addr_TonerCheckWrite, gTonerCheckinfo.TonerUsedCheckData);
					}
					else
					{
						tempWriteValue = MemoryWrite(gTonerCheckinfo.addr_TonerCheckWrite, gTonerCheckinfo.TonerUsedCheckData, TONER);
					}
					if(tempWriteValue !=0)
					{
						//fSC_Writefault = TRUE;
						//SC_State = ST_CERT_FAILULE;
#if	ENG_TRACE					
						MSG("(Eng)## MemoryWrite Error : TonerCheck ##\n");
#endif
					}
				}
				else
				{
					gTonerCheckinfo.f_TonerCheck |= gTonerCheckinfo.TonerUsedCheckData;
#if	ENG_TRACE			
					MSG("(Eng)## MemoryWrite(TonerCheck : Address : %d) = %d, f_TonerCheck = %d [%d] ##\n"
					,gTonerCheckinfo.addr_TonerCheckWrite, gTonerCheckinfo.TonerUsedCheckData, gTonerCheckinfo.f_TonerCheck, cProcessCounter);
#endif				
					usleep(30000);
					
					if(fDataEncrytionMode == TRUE)
					{
						tempWriteValue = MemoryReadEncGet(gTonerCheckinfo.addr_TonerCheckWrite);
					}
					else
					{
						tempWriteValue = MemoryRead(gTonerCheckinfo.addr_TonerCheckWrite, TONER);
					}
					MSG("(Eng)## MemoryRead(TonerCheckWrite : Address : %d)  Read <%d> <%d> [%d] ##\n",gTonerCheckinfo.addr_TonerCheckWrite, tempWriteValue, gTonerCheckinfo.TonerUsedCheckData, cProcessCounter);
					if(tempWriteValue == gTonerCheckinfo.TonerUsedCheckData)
					//if(1)
					{
				
						if(MemoryLockWrite(gTonerCheckinfo.addr_TonerCheckWrite) != 0)
						{
#if	ENG_TRACE					
							MSG("(Eng)## MemoryLockWrite Error : (Retry!!!) ##\n");
#endif				
							usleep(30000);
							if(MemoryLockWrite(gTonerCheckinfo.addr_TonerCheckWrite) != 0)
							{
#if	ENG_TRACE					
								MSG("(Eng)## MemoryLockWrite Error ##\n");
#endif
							}
						}
						else
						{
#if	ENG_TRACE			
							MSG("(Eng)## MemoryLockWrite(TonerCheck : Address : %d) ##\n"
							,gTonerCheckinfo.addr_TonerCheckWrite);
#endif
						}
					}
					else
					{
						MSG("(Eng)## MemoryWrite Error : TonerCheck ##\n");
					}
				}
#endif
#endif
			}
		}
#endif
		/* [KSR032914]-start */
		if((fSmartICWrite_DrumNearEnd == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0			
			Set_Timer_I2C();
			fSmartICWrite_DrumNearEnd = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_NEAR_END_ADDR, SET, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : DRUM_NEAR_END (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_NEAR_END_ADDR, SET, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : DRUM_NEAR_END ##\n");
#endif
				}
			}
			else
			{
				fState_DrumNearEnd = SET;
				fSmartICWrite_DrumHistoy = TRUE;
				Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_LOW);
#if	ENG_TRACE
				MSG("(D)## MemoryWrite(DRUM_NEAR_END : Address : %d) = %d [%d] ##\n",SC_TONER_NEAR_END_ADDR, SET, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
		
		if((fSmartICWrite_DrumEnd == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0
			Set_Timer_I2C();
			fSmartICWrite_DrumEnd = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_END_ADDR, SET, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : DRUM_END (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_END_ADDR, SET, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : DRUM_END ##\n");
#endif
				}
			}
			else
			{
				fState_DrumEnd = SET;
				fSmartICWrite_DrumHistoy = TRUE;
				Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_END);
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(DRUM_END : Address : %d) = %d [%d] ##\n",SC_TONER_END_ADDR, SET, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}		
		
		if((fSmartICWrite_DrumBlock == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0			
			Set_Timer_I2C();
			fSmartICWrite_DrumBlock = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_BLOCK_ADDR, SET, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : DRUM_BLOCK (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_BLOCK_ADDR, SET, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : DRUM_BLOCK ##\n");
#endif
				}
			}
			else
			{
				fState_DrumBlock = SET;
				fSmartICWrite_DrumHistoy = TRUE;
				fStore_TonerBlockState = SET;
				// SC_State = ST_TONER_BLOCK;
				// Set_SmartICInfoNotify(CARTRIDGE_CMD_CLOSE_FAILULE,0x00);
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(DRUM_BLOCK : Address : %d) = %d [%d] ##\n",SC_TONER_BLOCK_ADDR, SET, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
		
		if((fSmartICWrite_DrumHistoy == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0
			Set_Timer_I2C();
			fSmartICWrite_DrumHistoy = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : TONER_END_drum (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error_drum : TonerHistoy ##\n");
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(DrumHistoy : Address_drum : %d) = %d [%d] ##\n",SC_TONER_HISTORY_COUNT_ADDR, cTotal_Print_Count, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE	
		if(0/*(fSmartICWrite_FreeMotorOn == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_FreeMotorOn == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_FreeMotorOn = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_FREE_MOTOR_ON_ADDR, cFreeMotorOn, TONER) != 0)
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : FREE_MOTOR_ON (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_FREE_MOTOR_ON_ADDR, cFreeMotorOn, TONER) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : FREE_MOTOR_ON ##\n");
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(FREE_MOTOR_ON : Address : %d) = %d [%d] ##\n",SC_TONER_FREE_MOTOR_ON_ADDR, cFreeMotorOn, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
		
		if((fSmartICWrite_DrumFreeMotorOn == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0			
			Set_Timer_I2C();
			fSmartICWrite_DrumFreeMotorOn = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_FREE_MOTOR_ON_ADDR, cDrumFreeMotorOn, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : DRUM_FREE_MOTOR_ON (Retry!!!) ##\n");
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_FREE_MOTOR_ON_ADDR, cDrumFreeMotorOn, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : DRUM_FREE_MOTOR_ON ##\n");
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(DRUM_FREE_MOTOR_ON : Address : %d) = %d [%d] ##\n",SC_TONER_FREE_MOTOR_ON_ADDR, cDrumFreeMotorOn, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC- end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE			
		if(0/*(fSmartICWrite_TonerHisFactorRate == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerHisFactorRate == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC
			Set_Timer_I2C();
			fSmartICWrite_TonerHisFactorRate = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_TonerHisFactorRate, TONER) != 0)
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : HIS_FACTOR_PERCENT_%d_ADDR (Retry!!!) ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1);
#endif				
				usleep(30000);
				if(MemoryWrite(SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_TonerHisFactorRate, TONER) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : HIS_FACTOR_PERCENT_%d_ADDR ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1);
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(HIS_FACTOR_PERCENT_%d_ADDR : Address : %d) = %d [%d] ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1, SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_TonerHisFactorRate, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
		
		if((fSmartICWrite_DrumHisFactorRate == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0
			Set_Timer_I2C();
			fSmartICWrite_DrumHisFactorRate = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_DrumHisFactorRate, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : HIS_FACTOR_PERCENT_%d_ADDR_drum (Retry!!!) ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1);
#endif				
				usleep(30000);
				if(MemoryWrite(SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_DrumHisFactorRate, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : HIS_FACTOR_PERCENT_%d_ADDR_drum ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1);
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(HIS_FACTOR_PERCENT_%d_ADDR : Address_drum : %d) = %d [%d] ##\n", SmartICWrite_TonerHisFactorRateADDRESS-SC_TONER_HIS_FACTOR_PERCENT_1_ADDR + 1, SmartICWrite_TonerHisFactorRateADDRESS, SmartICWrite_DrumHisFactorRate, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE			
		if(0/*(fSmartICWrite_TonerHisJob == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerHisJob == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_TonerHisJob = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SmartICWrite_TonerHisJobADRESS, SmartICWrite_TonerHisJob, TONER) != 0)
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR (Retry!!!) ##\n", SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1);
#endif				
				usleep(30000);
				if(MemoryWrite(SmartICWrite_TonerHisJobADRESS, SmartICWrite_TonerHisJob, TONER) != 0)
				{
					//fSC_Writefault = TRUE;
					//SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR (Retry!!!) ##\n", SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1);
#endif
				}
				else
				{
					fSmartICWrite_TonerHisJob_Total = TRUE;
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(HIS_MODE_JOB_%d_ADDR : Address : %d) = %d [%d] ##\n",SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1, SmartICWrite_TonerHisJobADRESS,SmartICWrite_TonerHisJob, cProcessCounter);
#endif			
				fSmartICWrite_TonerHisJob_Total = TRUE;
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
#if NO_WRITE_IC_INFO_FOR_IC_PRODUCTIONLINE		
		if(0/*(fSmartICWrite_TonerHisJob_Total == TRUE)*/ && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#else
		if((fSmartICWrite_TonerHisJob_Total == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
#endif
		{
#ifdef USE_TONER_IC			
			Set_Timer_I2C();
			fSmartICWrite_TonerHisJob_Total = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_HIS_MODE_JOB_10_ADDR, Mode_Job[NUM_TONER_JOB-1], TONER) != 0)
			{
#if	ENG_TRACE					
				MSG("(T)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR(total) (Retry!!!) ##\n", SC_TONER_HIS_MODE_JOB_10_ADDR);
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_HIS_MODE_JOB_10_ADDR, Mode_Job[NUM_TONER_JOB-1], TONER) != 0)
				{
					//fSC_Writefault = TRUE;
					//SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(T)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR(total) (Retry!!!) ##\n", SC_TONER_HIS_MODE_JOB_10_ADDR);
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(T)## MemoryWrite(HIS_MODE_JOB_%d_ADDR(total) : Address : %d) = %d [%d] ##\n",SC_TONER_HIS_MODE_JOB_10_ADDR, SC_TONER_HIS_MODE_JOB_10_ADDR,SmartICWrite_TonerHisJob, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_TONER_IC - end */
		}
		
		if((fSmartICWrite_DrumHisJob == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0			
			Set_Timer_I2C();
			fSmartICWrite_DrumHisJob = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SmartICWrite_TonerHisJobADRESS, SmartICWrite_DrumHisJob, DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR_drum (Retry!!!) ##\n", SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1);
#endif				
				usleep(30000);
				if(MemoryWrite(SmartICWrite_TonerHisJobADRESS, SmartICWrite_TonerHisJob, DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR_drum (Retry!!!) ##\n", SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1);
#endif
				}
				else
				{
					fSmartICWrite_DrumHisJob_Total = TRUE;
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(HIS_MODE_JOB_%d_ADDR : Address_drum : %d) = %d [%d] ##\n",SmartICWrite_TonerHisJobADRESS-SC_TONER_HIS_MODE_JOB_1_ADDR + 1, SmartICWrite_TonerHisJobADRESS,SmartICWrite_DrumHisJob, cProcessCounter);
#endif			
				fSmartICWrite_DrumHisJob_Total = TRUE;
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
		
		if((fSmartICWrite_DrumHisJob_Total == TRUE) && (Get_Elapse_I2C() >= SC_WRITE_WAIT_TIME))
		{
//#ifdef USE_DRUM_IC
#if 0			
			Set_Timer_I2C();
			fSmartICWrite_DrumHisJob_Total = FALSE;
#if SECURITY_IC_ATTACHED
#if SMART_IC_WRITE
			if(MemoryWrite(SC_TONER_HIS_MODE_JOB_13_ADDR, Drum_Mode_Job[12], DRUM) != 0)
			{
#if	ENG_TRACE					
				MSG("(D)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR_drum(total) (Retry!!!) ##\n", SC_TONER_HIS_MODE_JOB_13_ADDR);
#endif				
				usleep(30000);
				if(MemoryWrite(SC_TONER_HIS_MODE_JOB_13_ADDR, Drum_Mode_Job[12], DRUM) != 0)
				{
					fSC_Writefault = TRUE;
					SC_State = ST_CERT_FAILULE;
					// Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE,0x00);
#if	ENG_TRACE					
					MSG("(D)## MemoryWrite Error : HIS_MODE_JOB_%d_ADDR_drum(total) (Retry!!!) ##\n", SC_TONER_HIS_MODE_JOB_13_ADDR);
#endif
				}
			}
			else
			{
#if	ENG_TRACE			
				MSG("(D)## MemoryWrite(HIS_MODE_JOB_%d_ADDR(total) : Address_drum : %d) = %d [%d] ##\n",SC_TONER_HIS_MODE_JOB_13_ADDR, SC_TONER_HIS_MODE_JOB_13_ADDR,SmartICWrite_TonerHisJob, cProcessCounter);
#endif			
			}
#endif
#endif

#endif /* USE_DRUM_IC - end */
		}
		usleep(100*1000);
	}
#else /* SECURITY_IC_ATTACHED */
	return 0;
#endif	
}

Uint8 CheckSum_Check(int data_length)
{
	Uint8 cnt;
	Uint8 Checksum;
	Checksum=0;
	
	for(cnt=0; cnt<data_length-1; cnt++)
	{
		Checksum += RxData[cnt];
	}
	
	if(Checksum==RxData[cnt])
	{
		// printf("(Eng)CheckSum Check OK : Checksum=%d, RxData=%d\n",Checksum,RxData[cnt]);
		// printf("(Eng)CheckSum Check OK :RxData[0]=%x, RxData[1]=%d, RxData[2]=%d, RxData[3]=%d, RxData[4]=%d \n",RxData[0],RxData[1],RxData[2],RxData[3],RxData[4]);
		return TRUE;
	}
	else
	{
		// printf("(Eng)CheckSum Check Fail : Checksum=%d, RxData=%d\n",Checksum,RxData[cnt]);
		// printf("(Eng)CheckSum Check Fail :RxData[0]=%x, RxData[1]=%d, RxData[2]=%d, RxData[3]=%d, RxData[4]=%d \n",RxData[0],RxData[1],RxData[2],RxData[3],RxData[4]);
		return FALSE;
	}	/* [KSR032914]-end */
}

void Set_Timer_I2C(void)
{
	cTimerStart_I2C = cProcessCounter;
#if	ENG_TRACE	
	MSG("(Eng)$$ Set_Timer_I2C = %d $$\n",cTimerStart_I2C);
#endif
}

int Get_Elapse_I2C(void)
{
	return cProcessCounter - cTimerStart_I2C;
}


