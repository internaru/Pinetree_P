/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **
 **	 File Name :	security.c
 **	 Revision  :	0.1
 **	 Date	   : 	Sep 03, 2009
 **	 Author    :	Youngjin Lee
 +-----------------------------------------------------------------+*/
/* Modified By CSPark */

/* Initialize the parameters for printer control */
#include "farch.h"
#include "em.h"
#include "i2c_protocol.h"
#include "encryption.h"
#include <string.h>				/* ADD @@2013.01.10. By CSPark */
#include <sys/ioctl.h>			/* ADD @@2013.01.10. By CSPark */
#include <stdio.h>				/* ADD @@2013.01.10. By CSPark */
#include <stdlib.h>				/* ADD @@2013.01.10. By CSPark */
#include "i2c_interface.h"		/* 20130207 Add by Ryu */
#include <linux/i2c-dev.h>		/* 20130207 Add by Ryu */
#include <linux/i2c.h>			/* 20130207 Add by Ryu */
#include "logger.h"				/* [KSR072601] */

#define DBG_PRFX "smc: "
#define	LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMODULE_BIT(20)

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

Uint8 Get_SmartIC_KeyData(Uint8);/* [KSR032904] */
Uint8 CheckKaraMachineSmartchip(Uint8);

extern void EncMachine1 (Uint8 *Key, Uint8 *EncData);
extern void DecMachine1 (Uint8 *Key, Uint8 *EncData);
extern void EncMachine2 (Uint8 *Key, Uint8 *Challange);

extern void watchdog_service(void);

extern int Set_SmartICInfoNotify(Uint8, Uint8);
extern int Set_AmountTonerUsedNotify(Uint8);
extern int Set_MotorStatusNotify(Uint8, Uint32, Uint8, Uint32);
extern int Set_CartridgeTLISerialNotify(void);
extern int 	Set_MachineCheckNotify(Uint8);
extern void WatchDogClear(unsigned int);
extern ST_FLAG EngineFatalError1;
extern Uint16	CodeOfMachineCheck;
extern int cProcessCounter;
extern Uint8 TransferBiasPWMForServo;
extern int cTimerStart_Motor;
extern Struct_Fuser_Control FuserControlMsgSet;
extern Struct_Fuser_Control Fuser_Control;
extern Uint32	CurrentDotCountOnChip;

extern unsigned int Factor_Page[NUM_TONER_FACTOR];/* [KSR032904] */
extern unsigned int Mode_Job[NUM_TONER_JOB];/* [KSR032904] */
extern unsigned int Drum_Factor_Page[NUM_TONER_FACTOR];/* [KSR032904] */
extern unsigned int Drum_Mode_Job[NUM_TONER_JOB];/* [KSR032904] */

extern Uint32 cDrumRest;/* [KSR032904] */
extern Uint32 cDotCount;
extern Uint32 cDrumDotCount;/* [KSR032904] */
extern Uint32 cPageCount;
extern Uint32 cDrumPageCount;/* [KSR032904] */
extern Uint32 cPageCount5perTrans;
extern Uint32 cDrumPageCount5perTrans;/* [KSR032904] */
extern Uint32 cFreeMotorOn;/* [KSR032904] */
extern Uint32 cDrumFreeMotorOn;/* [KSR032904] */
extern Uint8 fNewTonerCheck;
extern Uint8 Machine_TLI;
extern int SmartICSeed;

extern Uint8 fSmartICWrite_TonerBlock;
extern Uint8 fSmartICWrite_WrongCount;
extern Uint8 machineLocation;
extern Uint8 cJobEndDotCountWriteFailCnt;

extern Uint8 	fSC_Writefault;
// const Uint8 u8_SerialNr[2]={0x00,0x11};
const Uint8 u8_SerialNr[2]={0x03,0x11};
Uint32 u32_SecurityCounter;
Uint32 u32_fakeData;
// sqlite3		*dbInstance;

extern const Uint8 cu8_KeyTable[];
Uint8 u8_AthuKey[16];
Uint8 u8_Key[16];
Uint8 TLI_GET[13];
Uint8 SC_State;
#if SMART_IC_LOCK_ENABLE
TONERCHECK gTonerCheckinfo;
#endif

Uint8 u8_I2CSendDatas[35];
Uint8 u8_I2CReceiveData[128];
Uint8 u8_Random[5];
Uint8 u8_Challenge[5];

Uint8 Toner_Used_Percentage;
Uint8 Toner_Remain_Percentage_New;
Uint8 Toner_Remain_Percentage_Old;
Uint8 Drum_Used_Percentage;/* [KSR032904] */
Uint32 TonerNearEndValue;
Uint32 TonerBlockValue;

Uint32 TonerCartridgeCapacity;
Uint32 DrumCartridgeCapacity; /* [KSR040101] */
Uint32 CartridgeSerialNum;
Uint8 fState_TonerNearEnd;
Uint8 fState_DrumNearEnd;/* [KSR032904] */
Uint8 fState_TonerEnd;
Uint8 fState_DrumEnd;/* [KSR032904] */
Uint8 fState_TonerBlock;
#if SMART_IC_LOCK_ENABLE
Uint8 fState_TonerWrong;
#endif
Uint8 fState_DrumBlock;/* [KSR032904] */
Uint8 fOld_SmartIC;
Uint8 rand_seed_add;
Uint8 fTonerLowMsgPending;
Uint8 fCartridgeRecycleType;

Uint32 TonerCapacityChk;
Uint32 SCVersionInfo;

#if SMART_IC_LOCK_ENABLE
Uint8 fDataEncrytionMode;
#endif

extern int i2c_dev;

int fd; /* req RAO */	

void SmartICDevDrvInit(void)/* 20130207 Add by Ryu */
{ 
	/* Execution Only One Time */
#if KSR_DBG_THREE	
	MSG("<%s>\n",__func__); /* [KSF050806] */
#endif
	
	fd = open(SMARTIC_DEVICE, O_RDWR);
    	if (fd < 0){
		ERR("SmartIC Device Open Err\n");
		exit(1);
	}
}

#if SMART_IC_LOCK_ENABLE
/********************************************************
Uint8 MemoryLockWrite(Uint16 Offset_add)
* 484번지~499번지 : Memory Lock Area : 각 Bit가 Memory 영역 0~459 에 맵핑됨.
*                 Bit를 set 할 시 해당 Memory 영역이 Lock 됨 (Write 불가)
* 484번지 : 0 ~ 31 
* 485번지 : 32 ~ 63
* 486번지 : 64 ~ 95
* 487번지 : 96 ~ 127
* 488번지 : 128 ~ 159
* 489번지 : 160 ~ 191
* 490번지 : 192 ~ 223
* 491번지 : 224 ~ 255
* 492번지 : 256 ~ 287
* 493번지 : 288 ~ 319
* 494번지 : 320 ~ 351
* 495번지 : 352 ~ 383
* 496번지 : 384 ~ 415
* 497번지 : 416 ~ 447
* 498번지 : 448 ~ 459
* 예1) 490 번지에 0x00000001 을 Write하면 192 번지가 Lock 됨. 
* 예2) 495 번지에 0x00000014 을 Write하면 354 번지, 356 번지가 Lock 됨.
* 
* Uint8 MemoryWrite(Uint16 Offset_add, Uint32 Memory_Data, Uint8 Drum)
* 함수는 무조건 최상위 bit를 1로 셋팅하여 별도의 Lock용 함수를 만든 것임.  
*********************************************************/

Uint8 MemoryLockWrite(Uint16 Offset_add)
{
	int result = 0;
	Uint16 LockAreaAddr;
	Uint32 LockBit;
	
	LockAreaAddr = SC_MEMORY_LOCK_AREA_FIRST_ADDR + (Uint16)(Offset_add / 32);
	LockBit = (Uint32)(0x00000001 << (Offset_add%32));
#if	SMART_IC_TRACE	
	MSG("(Eng) %s : LockAreaAddr = %d, LockBit = %x \n",__func__, LockAreaAddr, LockBit);
#endif	
	u8_I2CSendDatas[0] = SMART_IC_MEMORY_WRITE_CMD;
	u8_I2CSendDatas[1] = (Uint8)(LockAreaAddr>>8);
	u8_I2CSendDatas[2] = (Uint8)(LockAreaAddr>>0);
	u8_I2CSendDatas[3] = (Uint8)(LockBit>>24);
	u8_I2CSendDatas[4] = (Uint8)(LockBit>>16);
	u8_I2CSendDatas[5] = (Uint8)(LockBit>>8);
	u8_I2CSendDatas[6] = (Uint8)(LockBit>>0);
	
	u8_I2CSendDatas[7] = 0xFF - (Uint8)(u8_I2CSendDatas[0] + u8_I2CSendDatas[1] + u8_I2CSendDatas[2] + u8_I2CSendDatas[3] + u8_I2CSendDatas[4] + u8_I2CSendDatas[5] + u8_I2CSendDatas[6]);
	
#ifdef USE_TONER_IC
	//Opeartion : memory write (8)
	/* ### Write ### */
	if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
		perror("ioctl error2\n");
		result = -1;
		//exit(0);
	}

	if(write(fd, u8_I2CSendDatas, 8) != 8){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) + Data(4) + CheckSum(1) => 8 */
		MSG("write error::%s()::%d\n", __func__, __LINE__);
		//result = -1;
	}
#endif /* USE_TONER_IC - end */	

#if	ENG_TRACE
		MSG("(Eng)IIC : result 14 = %d\n",result);
#endif
	if(result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case14\n");
#endif					
	}
	
#if	SMART_IC_TRACE
	MSG("(Eng)MemoryLock write : Result = %d (Offset Location : %x%x) Data =%x,%x,%x,%x (Check Sum=%x)\n",result, u8_I2CSendDatas[1], u8_I2CSendDatas[2], u8_I2CSendDatas[3], u8_I2CSendDatas[4], u8_I2CSendDatas[5], u8_I2CSendDatas[6], u8_I2CSendDatas[7]);
#endif
	return result;
}

Uint32 MemoryLockRead(Uint16 Offset_add, Uint8 Drum)
{
	Uint32 read_result;
	Uint8 CheckSum;
	
	struct i2c_msg msg[2]; /* [KSR062801] */
	
	u8_I2CReceiveData[0] = SMART_IC_MEMORY_READ_CMD;
	u8_I2CReceiveData[1] = (Uint8)(Offset_add>>8);
	u8_I2CReceiveData[2] = (Uint8)(Offset_add>>0);
	
#ifdef USE_TONER_IC
	/* [KSR062801] */
	//	struct i2c_msg msg[2];

	msg[0].addr = SMARTIC_ADDR; 
	msg[0].flags = 0; 				//write
	msg[0].len = 3;
	msg[0].buf = u8_I2CReceiveData;

	msg[1].addr = SMARTIC_ADDR; 
	msg[1].flags = I2C_M_RD; 			//Read	
	msg[1].len = 5;
	msg[1].buf = u8_I2CReceiveData;

	struct i2c_rdwr_ioctl_data rdwr_arg;
	rdwr_arg.msgs = msg;
	rdwr_arg.nmsgs = 2;
	
	read_result = ioctl(fd, I2C_RDWR, &rdwr_arg);
#endif /* USE_TONER_IC - end */
	
	#if	ENG_TRACE
//	printf("(Eng)IIC : result 15 = %d\n",read_result);
#endif
	if(read_result < 0)
	{
		perror("ioctl error1\n"); /* [KSR070301] */
//		fSoftwareFailure = ERROR; /* [KSR070801] */
//		CodeOfMachineCheck = SOFTWARE_ERROR; /* [KSR070801] */
#if	ENG_TRACE
		ERR("(Eng)IIC : STATE_STUCK_LINE - Fatal Case15\n");
#endif
	}

#if SMART_IC_TRACE
	MSG("(Eng)MemoryLock Read : read_result = %d Data =%x,%x,%x,%x (Check Sum=%x)\n",read_result, u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2],u8_I2CReceiveData[3],u8_I2CReceiveData[4]);
#endif
	if(fOld_SmartIC == TRUE)
	{
		CheckSum = (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3]);
	}
	else
	{
		CheckSum = 0xFF - (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3]);
	}

	if(CheckSum!=u8_I2CReceiveData[4])
	{
		return 0xFFFFFFFF;
	}
	read_result = (Uint32)((Uint32)((Uint8)u8_I2CReceiveData[0] & 0x7F)<<24) + (Uint32)((Uint32)u8_I2CReceiveData[1]<<16) + (Uint32)((Uint32)u8_I2CReceiveData[2]<<8) + (Uint32)((Uint32)u8_I2CReceiveData[3]);

#if	SMART_IC_TRACE
	MSG("(Eng)MemoryLock READ = %d \n",read_result);
#endif
	return read_result;
}

Uint32 MemoryWriteEnc(Uint16 Offset_add, Uint32 Memory_Data)
{
	int result = 0;
	Uint8 temp_key[8];
	Uint8 u8_EncData[4];
	
	temp_key[0] = (Uint8)((CartridgeSerialNum >> 24) | 0x80);
	temp_key[1] = (Uint8)(CartridgeSerialNum >> 16);
	temp_key[2] = (Uint8)(CartridgeSerialNum >> 8);
	temp_key[3] = (Uint8)(CartridgeSerialNum >> 0);
	temp_key[4] = (Uint8)(cDotCount >> 24);
	temp_key[5] = (Uint8)(cDotCount >> 16);
	temp_key[6] = (Uint8)(cDotCount >> 8);
	temp_key[7] = (Uint8)(cDotCount >> 0);
	
	u8_EncData[0] = (Uint8)(Memory_Data >> 24);
	u8_EncData[1] = (Uint8)(Memory_Data >> 16);
	u8_EncData[2] = (Uint8)(Memory_Data >> 8);
	u8_EncData[3] = (Uint8)(Memory_Data >> 0);
		
	DecMachine1((Uint8 *)&temp_key[0], u8_EncData);
	
	u8_I2CSendDatas[0]  = SMART_IC_MEMORY_WRITE_ENC_CMD;
	u8_I2CSendDatas[1]  = (Uint8)(Offset_add>>8);
	u8_I2CSendDatas[2]  = (Uint8)(Offset_add>>0);
	u8_I2CSendDatas[3]  = u8_EncData[0];
	u8_I2CSendDatas[4]  = u8_EncData[1];
	u8_I2CSendDatas[5]  = u8_EncData[2];
	u8_I2CSendDatas[6]  = u8_EncData[3];
	u8_I2CSendDatas[7]  = temp_key[4];
	u8_I2CSendDatas[8]  = temp_key[5];
	u8_I2CSendDatas[9]  = temp_key[6];
	u8_I2CSendDatas[10] = temp_key[7];
	u8_I2CSendDatas[11] = 0xFF - (Uint8)(u8_I2CSendDatas[0] + u8_I2CSendDatas[1] + u8_I2CSendDatas[2] + u8_I2CSendDatas[3] + u8_I2CSendDatas[4] + u8_I2CSendDatas[5] + u8_I2CSendDatas[6] + u8_I2CSendDatas[7] + u8_I2CSendDatas[8] + u8_I2CSendDatas[9] + u8_I2CSendDatas[10] );

#ifdef USE_TONER_IC
	//Opeartion : memory write (8)
	/* ### Write ### */
	if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
		perror("ioctl error2\n");
		result = -1;
		//exit(0);
	}

	if(write(fd, u8_I2CSendDatas, 12) != 12){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) + Data(8) + CheckSum(1) => 12 */
		MSG("write error::%s()::%d\n", __func__, __LINE__);
		//result = -1;
	}
#endif /* USE_TONER_IC - end */	
#if	ENG_TRACE
	MSG("(Eng)IIC : result 14 = %d\n",result);
#endif

	if(result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case30\n");
#endif					
	}
//	result -= 4;			// Normal Write의 경우 8이지만, 암호화의 경우 4바이트가 더 붙어 12가 됨. -4를 하여 Write의 경우 8이 Ok가 되도록 통일 
	
#if KSR_DBG_ONE
	MSG("(Eng)Memory write Enc : Result = %d (Offset Location : %x%x) Data =%x,%x,%x,%x,%x,%x,%x,%x (Check Sum=%x)\n"
		,result, u8_I2CSendDatas[1], u8_I2CSendDatas[2], u8_I2CSendDatas[3], u8_I2CSendDatas[4], u8_I2CSendDatas[5], u8_I2CSendDatas[6], u8_I2CSendDatas[7], u8_I2CSendDatas[8], u8_I2CSendDatas[9], u8_I2CSendDatas[10], u8_I2CSendDatas[11]); /* [KSF050806] */
#endif
	return result;
}

Uint32 MemoryReadEnc(Uint16 Offset_add, Uint8 readmode)
{
	Uint32 read_result;
	Uint8 CheckSum;
	Uint8 temp_key[8];
	Uint8 u8_EncData[4];
	
	struct i2c_msg msg[2]; /* [KSR062801] */
	
	if((Offset_add < SC_DATA_ENCRYPTION_AREA_START_ADDR) || (Offset_add > SC_DATA_ENCRYPTION_AREA_END_ADDR))
	{
#if	ENG_TRACE
		MSG("(Eng)MemoryReadEnc : Addr Error <%d> \n", Offset_add);
#endif		
		return 0xFFFFFFFF;
	}
	
	if(readmode==ENCRYPTION_PRE_READ)
	{
		u8_I2CReceiveData[0] = SMART_IC_MEMORY_PRE_READ_ENC_CMD;
	}
	else
	{
		u8_I2CReceiveData[0] = SMART_IC_MEMORY_READ_ENC_CMD;
	}
	u8_I2CReceiveData[1] = (Uint8)(Offset_add>>8);
	u8_I2CReceiveData[2] = (Uint8)(Offset_add>>0);
	
	msg[0].addr = SMARTIC_ADDR; 
	msg[0].flags = 0; 				//write
	msg[0].len = 3;
	msg[0].buf = u8_I2CReceiveData;  	//Header

	msg[1].addr = SMARTIC_ADDR; 
	msg[1].flags = I2C_M_RD; 			//Read	
	msg[1].len = 9;
	msg[1].buf = u8_I2CReceiveData;  	//Header

	struct i2c_rdwr_ioctl_data rdwr_arg;
	rdwr_arg.msgs = msg;
	rdwr_arg.nmsgs = 2;
	
	read_result = ioctl(fd, I2C_RDWR, &rdwr_arg);
	
	if(readmode==ENCRYPTION_PRE_READ)
	{
		return 0xFFFFFFFF;
	}
	
	if(read_result < 0)
	{
		perror("ioctl error1\n"); /* [KSR070301] */
//		fSoftwareFailure = ERROR;
//		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case15\n");
#endif					
		return 0xFFFFFFFF;
	}
	
#if SMART_IC_TRACE
	MSG("(Eng)Memory Read Enc : read_result = %d Data =%x,%x,%x,%x,%x,%x,%x,%x (Check Sum=%x)\n",read_result, u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2]
	,u8_I2CReceiveData[3],u8_I2CReceiveData[4],u8_I2CReceiveData[5],u8_I2CReceiveData[6],u8_I2CReceiveData[7],u8_I2CReceiveData[8]);
#endif
	CheckSum = 0xFF - (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3] + u8_I2CReceiveData[4] + u8_I2CReceiveData[5] + u8_I2CReceiveData[6] + u8_I2CReceiveData[7]);
	
	if(CheckSum!=u8_I2CReceiveData[8])
	{
#if	SMART_IC_TRACE
		MSG("(Eng)Memory READ Checksum Fail!!!\n");
#endif
		return 0xFFFFFFFF;
	}
	
	temp_key[0] = (Uint8)((CartridgeSerialNum >> 24) | 0x80);
	temp_key[1] = (Uint8)(CartridgeSerialNum >> 16);
	temp_key[2] = (Uint8)(CartridgeSerialNum >> 8);
	temp_key[3] = (Uint8)(CartridgeSerialNum >> 0);
	temp_key[4] = u8_I2CReceiveData[4];
	temp_key[5] = u8_I2CReceiveData[5];
	temp_key[6] = u8_I2CReceiveData[6];
	temp_key[7] = u8_I2CReceiveData[7];
	
	//printf("(Eng)Key : %x,%x,%x,%x,%x,%x,%x,%x \n", temp_key[0],temp_key[1],temp_key[2]
	//,temp_key[3],temp_key[4],temp_key[5],temp_key[6],temp_key[7]);
	
	u8_EncData[0] = u8_I2CReceiveData[0];
	u8_EncData[1] = u8_I2CReceiveData[1];
	u8_EncData[2] = u8_I2CReceiveData[2];
	u8_EncData[3] = u8_I2CReceiveData[3];
	
	//printf("(Eng)Read Data before : Data =%x,%x,%x,%x\n",u8_EncData[0],u8_EncData[1],u8_EncData[2],u8_EncData[3]);
	
	DecMachine1((Uint8 *)&temp_key[0], u8_EncData);
	
	//printf("(Eng)Read Data after : Data =%x,%x,%x,%x\n",u8_EncData[0],u8_EncData[1],u8_EncData[2],u8_EncData[3]);
	
	read_result = (Uint32)((Uint32)u8_EncData[0]<<24) + (Uint32)((Uint32)u8_EncData[1]<<16) + (Uint32)((Uint32)u8_EncData[2]<<8) + (Uint32)((Uint32)u8_EncData[3]);
	//read_result = Uint32((Uint32)((Uint8)u8_I2CReceiveData[0] & 0x7F)<<24) + Uint32((Uint32)u8_I2CReceiveData[1]<<16) + Uint32((Uint32)u8_I2CReceiveData[2]<<8) + Uint32((Uint32)u8_I2CReceiveData[3]);
#if	SMART_IC_TRACE
	MSG("(Eng)Memory READ = %d \n",read_result);
#endif
	return read_result;
}

Uint32 MemoryReadEncGet(Uint16 Offset_add)
{
	Uint32 read_result;
	
	MemoryReadEnc(Offset_add, ENCRYPTION_PRE_READ);
	usleep(10000);
	read_result = MemoryReadEnc(Offset_add, ENCRYPTION_MAIN_READ);
	
	return read_result;
}
#endif

Uint8 MemoryWrite(Uint16 Offset_add, Uint32 Memory_Data, Uint8 Drum)/* [KSR032101] */
{
	int result = 0;	/* @@2013.01.10. By CSPark */

	u8_I2CSendDatas[0] = SMART_IC_MEMORY_WRITE_CMD;
	u8_I2CSendDatas[1] = (Uint8)(Offset_add>>8);
	u8_I2CSendDatas[2] = (Uint8)(Offset_add>>0);
	u8_I2CSendDatas[3] = (Uint8)((Memory_Data>>24)|0x80);
	u8_I2CSendDatas[4] = (Uint8)(Memory_Data>>16);
	u8_I2CSendDatas[5] = (Uint8)(Memory_Data>>8);
	u8_I2CSendDatas[6] = (Uint8)(Memory_Data>>0);
	if(fOld_SmartIC == TRUE)
	{
		u8_I2CSendDatas[7] = (Uint8)(u8_I2CSendDatas[0] + u8_I2CSendDatas[1] + u8_I2CSendDatas[2] + u8_I2CSendDatas[3] + u8_I2CSendDatas[4] + u8_I2CSendDatas[5] + u8_I2CSendDatas[6]);
	}
	else
	{
		u8_I2CSendDatas[7] = 0xFF - (Uint8)(u8_I2CSendDatas[0] + u8_I2CSendDatas[1] + u8_I2CSendDatas[2] + u8_I2CSendDatas[3] + u8_I2CSendDatas[4] + u8_I2CSendDatas[5] + u8_I2CSendDatas[6]);
	}

	if(Drum == SET)/* Drum */
	{
//#ifdef USE_DRUM_IC
#if 0
	//130110	result = ioctl(i2c_dev,CMD_WRITE_SMARTIC_MEMORY | IOCTL_MAGIC,&u8_I2CSendDatas);
		//Opeartion : memory write (8)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CSendDatas, 8) != 8){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) + Data(4) + CheckSum(1) => 8 */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			//result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else /* Toner */
	{
#ifdef USE_TONER_IC
		//Opeartion : memory write (8)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error2\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CSendDatas, 8) != 8){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) + Data(4) + CheckSum(1) => 8 */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			//result = -1;
		}
#endif /* USE_TONER_IC - end */	
	}

#if	ENG_TRACE
	MSG("(Eng)IIC : result 14 = %d\n",result);
#endif
	if(result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case14\n");
#endif
	}

#if KSR_DBG_ONE
	MSG("(Eng)Memory write : Result = %d (Offset Location : %x%x) Data =%x,%x,%x,%x (Check Sum=%x)\n",result, u8_I2CSendDatas[1], u8_I2CSendDatas[2], u8_I2CSendDatas[3], u8_I2CSendDatas[4], u8_I2CSendDatas[5], u8_I2CSendDatas[6], u8_I2CSendDatas[7]); /* [KSF050806] */
#endif
	return result;
}

/* [KSF061703] new key value */
// 아래 테이블은 enum값과 순서가 일치해야 함 
Uint8 Drum_KeyData[][NumOfKeyData] = {
	{0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06},		//WRONGTLI		=0		//임의의 Key값으로 인증 시도 실패 유도 (주의!!! 초기 chip이 0x00 16개를 초기 key값으로 가지고 있어 0x00 16개로 인증 시도 하면 안됨)
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},		//TEMPORARILY	=1 현재 chip에 키값을 넣을 수가 없어 초기값 상태임. 임시 
	{0x53,0x20,0x00,0x6A,0x73,0x61,0x4D,0x43,0x61,0x67,0xE4,0xCE,0x8A,0xBE,0xA9,0xE3},		//A610R40K		=2
};

Uint8 Toner_KeyData[][NumOfKeyData] = {
	{0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x00,0x01,0x02,0x03,0x04,0x05,0x06},		//WRONGTLI		=0
	{0x4C,0x65,0x65,0x59,0x6F,0x75,0x6E,0x67,0x4A,0x69,0x6E,0x20,0x95,0x96,0x40,0x15},		//RNDMASTER		=1
	{0x50,0x79,0x6F,0x4D,0x69,0x6E,0x53,0x65,0x6F,0x6B,0x20,0x11,0x90,0x23,0x31,0x41},		//S400T2HK		=2
	{0x4F,0x68,0x49,0x6E,0x48,0x77,0x61,0x6E,0x20,0x00,0x10,0x34,0x34,0x76,0x37,0x55},		//S400T5K		=3
	{0x4B,0x69,0x6D,0x4A,0x61,0x65,0x4D,0x69,0x6E,0x20,0x00,0x10,0x33,0x71,0x23,0x59},		//S400T8K		=4
	{0x4A,0x61,0x6E,0x67,0x59,0x6F,0x75,0x6E,0x67,0x4A,0x61,0x65,0x20,0x33,0x77,0x98},		//S400ST2HK		=5
	{0x4C,0x65,0x65,0x42,0x79,0x6F,0x75,0x6E,0x67,0x59,0x75,0x6E,0x20,0x61,0x75,0x57},		//S400ST5K		=6
	{0x4B,0x69,0x6D,0x53,0x75,0x6E,0x67,0x48,0x6F,0x20,0x00,0x10,0x32,0x39,0x93,0x10},		//LA25T1HKA		=7
	{0x4C,0x65,0x65,0x4A,0x75,0x6E,0x57,0x6F,0x6F,0x20,0x00,0x11,0x95,0x81,0x59,0x45},		//LA25T1HKB		=8
	{0x53,0x65,0x6F,0x4A,0x6F,0x6F,0x45,0x75,0x6E,0x20,0x00,0x10,0x38,0x65,0x01,0x20},		//LA25T2HK		=9
	{0x59,0x69,0x59,0x6F,0x6E,0x67,0x4A,0x75,0x6E,0x20,0x00,0x01,0x94,0x24,0x04,0x47},		//LA25T8K		=10
	{0x2B,0x05,0x3C,0x4A,0x71,0x25,0x17,0x0A,0x6F,0x2F,0x0E,0x15,0x9E,0x81,0x5C,0x4B},		//LA30T2HK		=11
	{0x90,0x68,0x65,0x6A,0x35,0xAE,0x57,0x0C,0x4F,0x90,0x12,0x11,0x85,0x81,0x52,0x4E},		//LA35T5K		=12
	{0xAE,0x67,0xEA,0x6A,0x55,0xC8,0x17,0x41,0x52,0xDE,0x92,0x58,0x41,0x8E,0xC9,0x30},		//LA35T8K		=13
	{0x4B,0x6B,0x53,0x75,0x69,0x74,0x79,0x6F,0x75,0x72,0x73,0x65,0x6C,0x66,0x20,0x18},		//LA25T1HK-C    =14
	{0x29,0x6E,0x59,0x6F,0x51,0x6F,0x4A,0x75,0x6E,0x20,0x90,0x01,0xE4,0x24,0x01,0x4C},		//LA30T2HK-C    =15
	{0xAB,0xC7,0xE1,0x6A,0x35,0xC8,0x12,0x34,0x52,0xDE,0x92,0xB8,0x4D,0x8E,0xC2,0x50},		//LA35T5K-C     =16
	{0x9F,0x87,0xE1,0xC4,0xF9,0x0E,0x12,0x61,0xA4,0x67,0x55,0x3C,0x7C,0x55,0x18,0xE8},		//S400T2HK-C    =17
	{0x55,0xAB,0x4E,0x28,0xDF,0x4E,0x13,0x58,0xBE,0x31,0xE5,0xC8,0x82,0x2E,0x77,0x6F},		//S400T5K-C     =18
	{0xCC,0xBC,0x43,0x56,0x73,0x19,0xE3,0x7F,0xCE,0x73,0x36,0x93,0xA2,0x72,0x44,0xF8},		//S400T8K-C     =19
	{0x5E,0x79,0x6F,0x4D,0x6A,0x6E,0x53,0xD5,0x67,0x6B,0x20,0x11,0x90,0x43,0x3C,0x41},		//M400T2HK	    =20
	{0x9F,0x68,0x49,0x63,0x48,0x77,0x6A,0x6E,0x20,0x0E,0x10,0x34,0x34,0x7F,0x37,0x57},		//M400T5K       =21
	{0xAB,0x69,0xED,0x4B,0x61,0x63,0x4D,0x62,0x6E,0x20,0x0F,0x10,0x33,0xD1,0x23,0x19},		//M400T8K       =22
	{0x5A,0x61,0x6E,0x67,0xA9,0x6F,0x75,0x6E,0x6F,0x4A,0x61,0x6F,0x20,0xE3,0x77,0xA8},		//M400ST2HK     =23
	{0x3C,0x65,0x6C,0x42,0x7E,0x6F,0x75,0xDE,0x67,0x5F,0x75,0x6E,0x20,0x61,0xF5,0x5C},		//M400ST5K      =24
	{0x18,0x52,0xEF,0x4D,0x6A,0x6F,0x53,0xD5,0x67,0x61,0x57,0x7C,0x85,0x43,0x32,0xC8},		//M400ST8K      =25
	{0x4B,0x61,0x8D,0x53,0x75,0x6E,0x67,0x48,0x6F,0x20,0xAC,0x80,0x32,0x39,0xE3,0x1D},		//M012T1HKA		=26
	{0x5D,0x65,0xDF,0x4A,0x6F,0x6F,0xF5,0x75,0x6E,0xA0,0xB0,0x10,0xD8,0x65,0xA1,0x20},		//M012T2HK      =27
	{0x59,0xE9,0x59,0x6F,0xCE,0x67,0x4A,0x7F,0x6E,0x20,0x80,0x0E,0x94,0x24,0xA4,0x45},		//M012T8K       =28
	{0xFC,0xAB,0x07,0x89,0x02,0x5A,0xC1,0x56,0x72,0x1E,0x08,0x49,0x10,0xE1,0x32,0x9E},		//M032T1HK      =29
	{0x97,0x21,0x0A,0x3B,0xC1,0x20,0x3B,0x61,0x81,0x4E,0x7B,0x16,0x97,0x52,0x10,0xA9},		//M032T2HK		=30
	{0x5C,0x8A,0x18,0x4A,0x75,0x6E,0x5E,0x6F,0x6F,0xEA,0x00,0xA1,0x9B,0xA1,0x5F,0x45},		//M012T1HKB     =31
	{0xA1,0x57,0x7F,0xAC,0x8F,0x6F,0x45,0x75,0xBE,0x20,0x50,0x10,0x38,0xE5,0x91,0xB0},		//M012T2HKB     =32
	{0x10,0x61,0x59,0x62,0x51,0x65,0x4A,0x75,0x4E,0x20,0x91,0x01,0xE4,0x2D,0x01,0x4C},		//M032T2HK-C    =33
	{0x9E,0x17,0xE1,0xCA,0xF9,0x0E,0x12,0x60,0xA4,0x6E,0xF5,0x3C,0x70,0x55,0x18,0xE8},		//M400T2HK-C    =34
	{0x11,0x12,0x4E,0xE8,0xDF,0xAE,0x13,0x58,0xBE,0x35,0xE5,0xC8,0x12,0x8E,0x77,0x9F},		//M400T5K-C     =35
	{0xA0,0xBC,0xD3,0xF6,0x70,0x19,0xE3,0x7F,0xCE,0x71,0x36,0x36,0x02,0xE2,0x14,0xF1},		//M400T8K-C     =36
	{0xFE,0x77,0x6F,0x4D,0x6A,0x65,0x5B,0xD5,0xEC,0x6B,0x20,0x17,0x70,0x61,0x4C,0x45},		//F400T2HK      =37
	{0x55,0x68,0x49,0x63,0x48,0x78,0x12,0x14,0xEF,0xBE,0x10,0x3A,0x34,0x7E,0xE7,0xB7},		//F400T5K       =38
	{0x7E,0xEC,0xDE,0x67,0xA4,0x6F,0x85,0x69,0x6F,0x4A,0x61,0x6F,0x2C,0xE3,0x7E,0xAB},		//F400ST2HK		=39
	{0x8C,0x8B,0x6C,0x82,0x7E,0x6F,0xFF,0xDE,0x6B,0x5F,0x71,0x6E,0x2F,0x61,0x11,0x5A},		//F400ST5K		=40
	{0x3B,0xE2,0xAD,0x53,0x45,0xCE,0x6E,0x4C,0x8F,0x20,0xAC,0x80,0x32,0x19,0xAE,0x1D},		//M015T1HKA     =41
	{0x8C,0x8E,0x18,0xAC,0x75,0xEB,0x5E,0xB1,0x6F,0xEA,0x05,0xB1,0x7F,0xA1,0x5F,0x83},		//M015T1HKB     =42
	{0x71,0x19,0xAD,0x53,0x45,0x18,0xAF,0xE7,0x8F,0x20,0xA6,0x16,0x31,0x19,0xA1,0x05},		//NA400T2HKR    =43
	{0x10,0xE2,0x3E,0x62,0x6E,0x00,0x11,0x54,0x21,0x7A,0xAC,0xFE,0x02,0x05,0x16,0x39},		//NA400T5KR     =44
	{0x92,0x18,0x51,0x23,0xFD,0xAC,0x18,0x55,0x24,0x0C,0xCD,0x21,0x90,0xBE,0x10,0x2F},		//NA400T8KR     =45
	{0x40,0x03,0x21,0x38,0x75,0x32,0x12,0xAE,0xFF,0xA1,0x03,0xB8,0x28,0x74,0xE1,0xCD},		//NA400T2HKN    =46
	{0x18,0xCD,0xBB,0x10,0x68,0x95,0xA3,0x5B,0x81,0x20,0x4C,0x50,0xE2,0xA9,0x3E,0x12},		//NA400T5KN     =47
	{0x31,0x02,0xA1,0x5E,0x4B,0x14,0x22,0x23,0xFE,0x18,0x9C,0x76,0x02,0xE1,0x0C,0x10},		//NA400T8KN     =48
	{0x0E,0x5D,0x27,0x2E,0x01,0xB1,0x83,0x10,0x40,0x78,0xB1,0x9F,0xC1,0x24,0x1D,0x97},		//LA30T2HK-N    =49
	{0x00,0x17,0x3E,0xD2,0x6E,0x90,0x11,0x54,0x21,0x7A,0xEC,0x12,0x02,0x0F,0x16,0x39},		//NM400T2HKR 	=50
	{0x12,0x1E,0x50,0x07,0x3D,0xEC,0x18,0x12,0x21,0x8C,0xCD,0x21,0x90,0xBE,0x10,0x18},		//NM400T5KR     =51
	{0x30,0x01,0x81,0x3E,0x75,0xA2,0x92,0xBB,0x10,0xA1,0x9E,0xB8,0x12,0x75,0x10,0xCD},		//NM400T8KR		=52
	{0x68,0xC8,0xBA,0x10,0xE8,0x07,0xD3,0x50,0x8F,0x70,0x44,0x50,0xE2,0x19,0x31,0x10},		//NM400T2HKN    =53
	{0x82,0x60,0x01,0x0E,0x4B,0x19,0x63,0x83,0xF0,0x14,0xF1,0x76,0x92,0xA2,0x7C,0x54},		//NM400T5KN     =54
	{0x01,0x0D,0x27,0x25,0x10,0x91,0xE3,0xA0,0x4C,0x78,0xF1,0x9B,0xCE,0x24,0x40,0x15},		//NM400T8KN     =55
	{0x0C,0x3D,0x1E,0x53,0x40,0x18,0x0F,0xE4,0x8F,0xC0,0xE6,0x23,0x41,0x99,0x01,0x0E},		//M032T2HK-N    =56
	{0x74,0xA8,0xC8,0x3A,0x54,0x8B,0x0E,0xE0,0x6F,0x23,0x9F,0x3F,0x17,0xBE,0xFA,0x22},		//LA40T12K      =57
	{0xD2,0x47,0x12,0x0A,0xFF,0x6C,0x91,0xE2,0x12,0x88,0x63,0x46,0xEA,0x01,0x2A,0x8D},		//AMTONERCART   =58
	{0xC2,0x17,0x02,0x1A,0xCF,0xC8,0x18,0x43,0xA1,0x29,0x60,0x3E,0xB1,0xC8,0x44,0x1B},		//A610T3K       =59
	{0x21,0x7A,0xFE,0xAA,0x18,0x44,0x45,0x72,0xCC,0x8A,0x45,0x81,0xBA,0x55,0x98,0x32},		//A610T6K 	   	=60
	{0x64,0x27,0x29,0x14,0x12,0x10,0x10,0x99,0x28,0x49,0xCA,0xDE,0xFE,0x1F,0x3C,0x67},		//A610T12K      =61
	{0x57,0x87,0xAC,0x18,0xC8,0x44,0x28,0xCD,0xDC,0x72,0x54,0x45,0x3E,0x2F,0x81,0xBD},		//LA40T6K       =62
	{0xDB,0x54,0x01,0x03,0xC3,0x2D,0x3D,0xE1,0x98,0x41,0x39,0x64,0xA3,0x9E,0xE9,0x18},		//LA40T9K       =63
	{0x3B,0x0A,0x09,0xDA,0xEF,0xC8,0x18,0x62,0x03,0x4E,0xFF,0xFE,0x3D,0xC2,0x49,0x3B},		//M610T3K       =64
	{0x73,0x4E,0x12,0x46,0xED,0x31,0x60,0x31,0x82,0x1E,0xAE,0xEA,0x12,0x11,0x21,0x04},		//M610T6K 		=65
	{0x09,0x80,0xA0,0x29,0x64,0x82,0x1E,0xD9,0x41,0x29,0x08,0x20,0xE1,0x59,0x63,0xA2},		//M610T12K		=66
	{0x17,0xE2,0xC1,0xD3,0xFF,0xAC,0xCA,0xDB,0x5F,0x1E,0x80,0x82,0x36,0xFE,0xAE,0x18},		//M013T6K		=67
	{0xDC,0x11,0xE1,0xE9,0x67,0x89,0x04,0xD8,0xFF,0xF1,0x67,0x10,0xC8,0x90,0xC1,0xE8},		//M013T9K		=68
	{0x64,0x28,0x2B,0x17,0x16,0x15,0x16,0xA0,0x30,0x40,0xCB,0xDF,0xFF,0x10,0x3D,0x68},		//A610T13K		=69
	{0x29,0x65,0x10,0x00,0x99,0xF9,0xAA,0xD9,0x34,0xCC,0xC8,0x77,0x43,0x29,0xE0,0x77},		//M610T13K		=70
	{0x80,0xC3,0x10,0x27,0x05,0x72,0xA7,0x68,0x53,0x1A,0x38,0x46,0xF5,0xB7,0x5F,0xE3},		//LA40T6K_C		=71
	{0x1D,0xAC,0x1A,0xE4,0x4E,0x83,0x72,0xEC,0xBE,0xCE,0xBF,0x09,0x37,0xA9,0x0E,0xBD},		//A610T3K_C     =72
	{0x79,0x75,0x6A,0x91,0x40,0x36,0x4C,0xBF,0x08,0xDA,0xC5,0x3A,0x58,0xE7,0x67,0x30},		//A610T6K_C     =73
	{0x52,0xC7,0x52,0x3C,0xE0,0x89,0xD9,0x3E,0x97,0xA1,0xE5,0xD2,0x7B,0xEC,0x85,0xD1},		//A610T13K_C    =74
	{0x69,0xB5,0xCE,0xB7,0xDC,0x15,0x70,0x3F,0x42,0x92,0x67,0x66,0xD2,0xC4,0x22,0x48},		//M013T3K   	=75
	{0xB9,0xC3,0x32,0xE2,0x81,0x86,0x3D,0x4F,0x88,0xAB,0x8D,0xFB,0x0B,0x12,0xBA,0x51},		//A610T3K_W   	=76
	{0x3F,0x23,0x90,0xF9,0xC8,0x11,0xBD,0x7A,0x1F,0x4E,0x87,0x6B,0x97,0x28,0x1A,0x38},		//A610T6K_W   	=77
	{0xF3,0x1D,0xBA,0x13,0xDB,0x95,0xAA,0x24,0xA9,0x9E,0x33,0x15,0xCB,0xFC,0xFC,0x9F},		//A610T13K_W   	=78
	{0x93,0x9B,0x40,0x78,0x56,0xCB,0x74,0x23,0x74,0x43,0x15,0x96,0xB9,0x21,0x4E,0x76},		//LA40T6K_W   	=79
	{0x18,0x7A,0xE7,0x99,0xC3,0x99,0x64,0x9B,0x4D,0x30,0xAA,0x2E,0x91,0xF3,0xB8,0x51},		//ABBA2T9K		=80 // for abba2 test
	{0x14,0xF5,0x60,0xA7,0xFC,0xE6,0x01,0x0D,0x2E,0xE2,0xF8,0xAA,0xD7,0x2C,0x4E,0x6F},		//M013T3K_C		=81
	{0x49,0x74,0x83,0x22,0xC4,0x6D,0x7C,0x5C,0x8A,0x1B,0xCF,0x06,0xCC,0xA2,0x14,0x6F},		//M610T3K_C		=82
	{0xB3,0x61,0xF2,0x84,0xD7,0x93,0x71,0x8C,0xF3,0x73,0xF0,0x14,0xE7,0x09,0xE3,0xEC},		//M610T6K_C		=83
	{0x0D,0xE1,0xE9,0xD3,0x15,0xC9,0x2B,0xF7,0xBF,0x75,0x0C,0x2F,0xA4,0x98,0x58,0x48},		//M610T13K_C	=84
	{0xFA,0x4D,0x47,0x1B,0xA4,0xD8,0x2D,0x4A,0x28,0x62,0xBF,0x01,0x03,0xAC,0xDA,0x06},		//M013T3K_W		=85
	{0x09,0xD4,0x3A,0xB5,0xFD,0xE7,0xB8,0xE2,0x0A,0xC8,0x4E,0x9F,0xAE,0x0A,0x7D,0x57},		//LA40T9K_W		=86
	{0xC7,0xC8,0x48,0x82,0x2C,0x1A,0xCD,0x53,0xB9,0x05,0xA0,0xC3,0xEE,0x3E,0x72,0x15},		//L236T3K		=87
	{0xE7,0x6C,0x91,0x8E,0xB7,0x5E,0xAB,0xC0,0x89,0xCD,0x44,0x31,0x45,0x60,0xAA,0xD4},		//L236T6K		=88
	{0xE5,0x48,0x19,0x8D,0x3E,0x29,0xDC,0x1B,0x1A,0x15,0xC0,0xC3,0xDE,0xD6,0xA1,0xB4},		//A410T3K		=89
	{0x60,0xEB,0x2B,0xC3,0x21,0xB0,0x8D,0xBA,0xA6,0xA5,0x02,0x29,0x7C,0x87,0xEC,0x95},		//A410T6K		=90
	{0x56,0x21,0x20,0xB2,0xB5,0x00,0x2C,0x08,0x3A,0x84,0x97,0x2A,0x11,0xD5,0xC4,0xDA},		//A410T9K		=91
	{0x33,0x22,0xF4,0x1B,0x09,0x67,0x26,0x95,0xE6,0x51,0x95,0x55,0xBC,0xE7,0x7A,0x6A},		//LA40T3K_W		=92
	{0x7B,0xD1,0xA9,0x06,0xF0,0xDA,0x40,0x43,0xCF,0x78,0x21,0x13,0x47,0x5B,0xF8,0x66},		//A616DNST6K	=93
	{0xE4,0xD4,0xFC,0x86,0x35,0x13,0x31,0x78,0x42,0x7C,0x3E,0xB8,0x84,0x61,0x9D,0x18},		//A616DNST13K	=94
	{0x42,0x2B,0x0D,0x26,0x6E,0x1F,0x96,0x6A,0x10,0x97,0x10,0x92,0xF1,0x35,0xB8,0x7E},		//THM247B01	=95
	{0xA9,0x14,0xCF,0xBE,0xB3,0xD5,0x5E,0x71,0x4F,0xD4,0x5A,0x89,0x8D,0x9C,0x6E,0xDC},		//P247T3K_WV	=96
	{0xEE,0x75,0x24,0xD0,0x28,0x4D,0x36,0x21,0x19,0x97,0xAD,0xDD,0x3F,0xD2,0x37,0xDB},		//M247T3K_WV	=97
	{0x93,0x1C,0x5D,0x55,0x44,0x1F,0xE2,0x62,0x10,0x96,0x87,0x53,0xF4,0x35,0x38,0x1E},		//THM247A01	=98
};
/* [KSF061703] new key value */

//void GenerateCipherCode(Uint8 Drum)/* [KSR032904] */
void GenerateCipherCode(Uint8 Drum, Uint8 fake)/* [KSR032904] */
{
	Uint8 u8_EncData1[4];
	Uint8 u8_EncData2[4];
	Uint8 cnt;
	int write_result = 0;	/* @@2013.01.10. By CSPark */

	/*STEP1 :  Making Random Number*/
	// Random Number 1(Upper)
	u32_SecurityCounter++;
	u8_EncData1[0] = u8_SerialNr[1];
	u8_EncData1[1] = (Uint8)(u32_SecurityCounter & 0XFF);
	u8_EncData1[2] = (Uint8)(u32_SecurityCounter >> 8);
	u8_EncData1[3] = (Uint8)(u32_SecurityCounter >> 16);
	DecMachine1((Uint8 *)&u8_Key[0], u8_EncData1);

	// Random Number 2(Lower)
	u32_SecurityCounter++;
	u8_EncData2[0] = u8_SerialNr[0];
	u8_EncData2[1] = (Uint8)(u32_SecurityCounter & 0XFF);
	u8_EncData2[2] = (Uint8)(u32_SecurityCounter >> 8);
	u8_EncData2[3] = (Uint8)(u32_SecurityCounter >> 16);
	DecMachine1((Uint8 *)&u8_Key[8], u8_EncData2);

	// Merge Random Number 1 & 2 : Make 40 bits Random Number
	u8_Random[0] = u8_EncData2[0];
	u8_Random[1] = u8_EncData1[0] ^ u8_EncData2[1];
	u8_Random[2] = u8_EncData1[1] ^ u8_EncData2[2];
	u8_Random[3] = u8_EncData1[2] ^ u8_EncData2[3];
	u8_Random[4] = u8_EncData1[3];

	#if KSR_DBG_THREE /* SMC DBG LV1 */
	MSG("(Eng)Step1 : u8_Random=%x,%x,%x,%x,%x\n",u8_Random[0], u8_Random[1],u8_Random[2],u8_Random[3],u8_Random[4]); /* [KSF050806] */
	#endif
	/*STEP2 : 24 bits Encrypted data1 */
	memcpy(u8_Challenge,u8_Random,5);

	EncMachine2((Uint8 *)&u8_Key[0], u8_Challenge);
	#if KSR_DBG_THREE /* SMC DBG LV1 */
	MSG("(Eng)Step2 : u8_Challenge=%x,%x,%x\n",u8_Challenge[0], u8_Challenge[1],u8_Challenge[2]); /* [KSF050806] */
	#endif
	/*STEP3 : 32 bits Encrypted data2  */
	u32_SecurityCounter++;
	u8_EncData1[0] = u8_SerialNr[0];
	u8_EncData1[1] = (Uint8)(u32_SecurityCounter & 0XFF);
	u8_EncData1[2] = (Uint8)(u32_SecurityCounter >> 8);
	u8_EncData1[3] = (Uint8)(u32_SecurityCounter >> 16);
	DecMachine1((Uint8 *)&u8_Key[0], u8_EncData1);
	#if KSR_DBG_THREE /* SMC DBG LV1 */	
	MSG("(Eng)Step3 : u8_EncData1=%x,%x,%x,%x\n",u8_EncData1[0], u8_EncData1[1],u8_EncData1[2],u8_EncData1[3]); /* [KSF050806] */
	#endif
	/*STEP4 : 32 bits Encrypted data3  */
	u32_SecurityCounter++;
	u8_EncData2[0] = u8_SerialNr[1];
	u8_EncData2[1] = (Uint8)(u32_SecurityCounter & 0XFF);
	u8_EncData2[2] = (Uint8)(u32_SecurityCounter >> 8);
	u8_EncData2[3] = (Uint8)(u32_SecurityCounter >> 16);
	DecMachine1((Uint8 *)&u8_Key[8], u8_EncData2);
	#if KSR_DBG_THREE /* SMC DBG LV1 */
	MSG("(Eng)Step4 : u8_EncData2=%x,%x,%x,%x\n",u8_EncData2[0], u8_EncData2[1],u8_EncData2[2],u8_EncData2[3]); /* [KSF050806] */
	#endif
	/*STEP5 : Merge random number & Encrypted data 1,2,3 => 128 bits (16bytes) */
	u8_I2CSendDatas[0] = SMART_IC_CERTIFICATION_SEND_CMD;
	memcpy(&u8_I2CSendDatas[1], u8_EncData2, 4);
	memcpy(&u8_I2CSendDatas[5], u8_EncData1, 4);
	memcpy(&u8_I2CSendDatas[9], u8_Challenge, 3);
	memcpy(&u8_I2CSendDatas[12], u8_Random, 5);

	if(fake == 1)
	{
		u8_I2CSendDatas[5] = (Uint8)(u32_fakeData & 0XFF);
		u8_I2CSendDatas[7] = (Uint8)(u32_fakeData >> 8);
		u8_I2CSendDatas[8] = (Uint8)(u32_fakeData >> 16);
		u8_I2CSendDatas[10] = (Uint8)(u32_fakeData >> 24);
	}

	/*STEP6 : Make Check Sum (1bytes)*/
	u8_I2CSendDatas[17] = 0;
	for(cnt=0; cnt<17; cnt++)
	{
		u8_I2CSendDatas[17] += u8_I2CSendDatas[cnt];
	}

	/*Send Encrypted Data (16bytes) to Security IC*/

	if(Drum == SET)/* [KSR032905] */
	{ 
//#ifdef USE_DRUM_IC 
#if 0  
		//130110	write_result = ioctl(i2c_dev,CMD_WRITE_SMARTIC_CERTKEY_DRUM | IOCTL_MAGIC,&u8_I2CSendDatas);
		//Opeartion : cipher code send (18)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			write_result = -1;
			exit(0);
		}
	
		if(write(fd, u8_I2CSendDatas, 18) != 18){ /* Header(1) + Cipher Code(16) + CheckSum(1) => 18 */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			//write_result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else
	{
//#ifdef USE_TONER_IC
		//Opeartion : cipher code send (18)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error3\n");
			write_result = -1;
			exit(0);
		}
	
		if(write(fd, u8_I2CSendDatas, 18) != 18){ /* Header(1) + Cipher Code(16) + CheckSum(1) => 18 */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			//write_result = -1;
		}
//#endif /* USE_TONER_IC - end */
	}

#if KSR_DBG_THREE
	MSG("(Eng)IIC : result 11 = %d\n",write_result); /* [KSF050806] */
#endif
	if(write_result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case11\n");
#endif
	}

#if KSR_DBG_THREE /* [KSF050806] */
	MSG("(Eng)u8_I2CSendDatas=%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x\n"
	,u8_I2CSendDatas[0], u8_I2CSendDatas[1],u8_I2CSendDatas[2],u8_I2CSendDatas[3]
	,u8_I2CSendDatas[4],u8_I2CSendDatas[5],u8_I2CSendDatas[6],u8_I2CSendDatas[7]
	,u8_I2CSendDatas[8],u8_I2CSendDatas[9],u8_I2CSendDatas[10],u8_I2CSendDatas[11]
	,u8_I2CSendDatas[12],u8_I2CSendDatas[13],u8_I2CSendDatas[14],u8_I2CSendDatas[15]
	,u8_I2CSendDatas[16],u8_I2CSendDatas[17]);
#endif
}

#if ABBA2
Uint8 IsAuthenticationOK(Uint8 Drum)/* [KSR032906] */
{
	int result = 0, i;	/* @@2013.01.10. By CSPark */
	Uint8 CalculatedCheckSum = 0;
	Uint8 CheckSum = 0;
	Uint8 DecValue1[4];
	Uint8 DecValue2[4];
	Uint8 CheckSumDataLength;
	Uint32 cDecValue1,cDecValue2;
	
	if(SCVersionInfo <= 4)
	{
		u8_I2CReceiveData[0] = SMART_IC_ADVENCED_CERTIFICATION_RECEIVE_CMD; /* 0x2C */
	}
	else
	{
		u8_I2CReceiveData[0] = SMART_IC_CERTIFICATION_RECEIVE_CMD; /* 0x23 */
	}

	if(Drum == SET) /* DRUM IC */
	{
//#ifdef USE_DRUM_IC
#if 0
	//130110	result = ioctl(i2c_dev, CMD_READ_SMARTIC_CERTKEY | IOCTL_MAGIC, &u8_I2CReceiveData);
		//Opeartion : Cipher code receive(9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR_DRUM => (0x20 >> 1) what? */
			perror("ioctl error\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 1) != 1){ /* Header(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			//result = -1;
		}

		memset(u8_I2CReceiveData, 0, 6);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error4\n");
			result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 4) != 4){ /* CipherCode(3) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			//result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else /* TONER IC */
	{
//#ifdef USE_TONER_IC
		//Opeartion : Cipher code receive(9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error5\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 1) != 1){ /* Header(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			//result = -1;
		}

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error6\n");
			result = -1;
			//exit(0);
		}
		memset(u8_I2CReceiveData, 0, 13);
		if(SCVersionInfo <= 4)
		{
			if(read(fd, u8_I2CReceiveData, 13) != 13){ /* CipherCode(11) + KeyIndex(1) + CheckSum(1) */
				MSG("read error::%s()::%d\n", __func__, __LINE__);
				//result = -1;
			}
		}
		else
		{
			if(read(fd, u8_I2CReceiveData, 4) != 4){ /* CipherCode(3) + CheckSum(1) */
				MSG("read error::%s()::%d\n", __func__, __LINE__);
				//result = -1;
			}
		}
//#endif /* USE_TONER_IC - end */
	}	
#if	ENG_TRACE
	MSG("(Eng)IIC : result 12 = %d\n",result);
#endif
	if(result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case12\n");
#endif
	}
#if KSR_DBG_THREE /* SMC DBG LV1 */	
	 MSG("(Eng)### %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d ### \n",u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2],u8_I2CReceiveData[3],u8_I2CReceiveData[4],
	 u8_I2CReceiveData[5],u8_I2CReceiveData[6],u8_I2CReceiveData[7],u8_I2CReceiveData[8],u8_I2CReceiveData[9],u8_I2CReceiveData[10],u8_I2CReceiveData[11],u8_I2CReceiveData[12]);
#endif
	if(result != -1)
	{
		if(SCVersionInfo <= 4)
		{
			CheckSumDataLength = 12;
			CheckSum = u8_I2CReceiveData[12];
		}
		else
		{
			CheckSumDataLength = 3;
			CheckSum = u8_I2CReceiveData[3];
		}
		
		for(i = 0; i < CheckSumDataLength; i++)
		{
			CalculatedCheckSum += u8_I2CReceiveData[i];
		}
		
		if(CheckSum == CalculatedCheckSum)
		{
			memcpy(u8_Challenge, u8_Random, 5);
#if	KSR_DBG_THREE		
			MSG("(Eng)u8_Challenge=%x,%x,%x,%x,%x\n",u8_Challenge[0], u8_Challenge[1],u8_Challenge[2],u8_Challenge[3],u8_Challenge[4]);
#endif
			EncMachine2((Uint8 *)&u8_Key[6], u8_Challenge);
			if(memcmp(u8_Challenge, u8_I2CReceiveData, 3)!=0)
			{
#if KSR_DBG_THREE
				MSG("(eng)Authentication 1st fail\n");
#endif
				return 0;
			}
			if(SCVersionInfo < 4)
			{
#if KSR_DBG_THREE
				MSG("(eng)Authentication OK!!\n");
#endif
				return 1; /* OK */
			}
			
			memcpy(u8_AthuKey, &cu8_KeyTable[u8_I2CReceiveData[11]] , 16);
#if KSR_DBG_THREE
			MSG("(eng)u8_AthuKey = %d, %d,%d, %d,%d, %d,%d, %d,%d, %d,%d, %d,%d, %d,%d, %d\n", u8_AthuKey[0], u8_AthuKey[1], u8_AthuKey[2], u8_AthuKey[3], u8_AthuKey[4], u8_AthuKey[5], u8_AthuKey[6]
			, u8_AthuKey[7], u8_AthuKey[8], u8_AthuKey[9], u8_AthuKey[10], u8_AthuKey[11], u8_AthuKey[12], u8_AthuKey[13], u8_AthuKey[14], u8_AthuKey[15]);
#endif
			memcpy(DecValue1, &u8_I2CReceiveData[3] , 4);
			memcpy(DecValue2, &u8_I2CReceiveData[7] , 4);
			
			DecMachine1((Uint8 *)&u8_AthuKey[0], DecValue1);
			DecMachine1((Uint8 *)&u8_AthuKey[8], DecValue2);
			
			cDecValue1 = (Uint32)((DecValue1[3] << 24) + (DecValue1[2] << 16) + (DecValue1[1] << 8) + DecValue1[0]);
			cDecValue2 = (Uint32)((DecValue2[3] << 24) + (DecValue2[2] << 16) + (DecValue2[1] << 8) + DecValue2[0]);
#if KSR_DBG_THREE
			MSG("(eng)DecValue1 = %x, %x, %x, %x\n", DecValue1[3], DecValue1[2], DecValue1[1], DecValue1[0]);
			MSG("(eng)DecValue2 = %x, %x, %x, %x\n", DecValue2[3], DecValue2[2], DecValue2[1], DecValue2[0]);
			MSG("(eng) ######## result = %x, %x\n", cDecValue1, cDecValue2);
#endif
			cDecValue1++;
			if(cDecValue1 != cDecValue2)
			{
#if KSR_DBG_THREE
				MSG("Authentication 2nd fail\n");
#endif
				return 0;
			}
#if KSR_DBG_THREE
			MSG("(eng)Authentication OK!!\n");
#endif
			return 1; /* OK */
		}
		else
		{
#if KSR_DBG_THREE
			MSG("(eng)Authentication data Checksum fail\n");
#endif
			return 3;
		}
	}
	else
	{
#if KSR_DBG_THREE
		MSG("(eng)Authentication data structure fail\n");
#endif
		return 2;
	}
}
#else
Uint8 IsAuthenticationOK(Uint8 Drum)/* [KSR032906] */
{
	int result = 0;	/* @@2013.01.10. By CSPark */
	Uint8 CheckSum;

	u8_I2CReceiveData[0] = SMART_IC_CERTIFICATION_RECEIVE_CMD; /* 0x23 */

	if(Drum == SET) /* DRUM IC */
	{
//#ifdef USE_DRUM_IC
#if 0
	//130110	result = ioctl(i2c_dev, CMD_READ_SMARTIC_CERTKEY | IOCTL_MAGIC, &u8_I2CReceiveData);
		//Opeartion : Cipher code receive(9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR_DRUM => (0x20 >> 1) what? */
			perror("ioctl error\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 1) != 1){ /* Header(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			//result = -1;
		}

		memset(u8_I2CReceiveData, 0, 6);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error4\n");
			result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 4) != 4){ /* CipherCode(3) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			//result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else /* TONER IC */
	{
//#ifdef USE_TONER_IC
		//Opeartion : Cipher code receive(9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error5\n");
			result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 1) != 1){ /* Header(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			//result = -1;
		}

		memset(u8_I2CReceiveData, 0, 6);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error6\n");
			result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 4) != 4){ /* CipherCode(3) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			//result = -1;
		}
//#endif /* USE_TONER_IC - end */
	}	
#if	ENG_TRACE
	MSG("(Eng)IIC : result 12 = %d\n",result);
#endif
	if(result==-1)
	{
		fSoftwareFailure = ERROR;
		CodeOfMachineCheck = SOFTWARE_ERROR;
#if	ENG_TRACE
		MSG("(Eng)IIC : STATE_STUCK_LINE - Fatal Case12\n");
#endif
	}
#if KSR_DBG_THREE /* SMC DBG LV1 */	
	MSG("(Eng)### %d, %d, %d, %d ## \n",u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2],u8_I2CReceiveData[3]); /* [KSF050806] */
#endif
	if(result != -1)
	{
		memcpy(u8_Challenge, u8_Random, 5);
#if KSR_DBG_THREE
		MSG("(Eng)1: u8_Challenge=%x,%x,%x,%x,%x\n",u8_Challenge[0], u8_Challenge[1],u8_Challenge[2],u8_Challenge[3],u8_Challenge[4]); /* [KSF050806] */
#endif
		EncMachine2((Uint8 *)&u8_Key[6], u8_Challenge);
		CheckSum = u8_Challenge[0] + u8_Challenge[1] + u8_Challenge[2];

		if(CheckSum==u8_I2CReceiveData[3])
		{
#if KSR_DBG_THREE
			MSG("(Eng)Authentication CheckSum OK!!!\n"); /* [KSF050806] */
#endif
		}
		else
		{
#if KSR_DBG_THREE
			MSG("(Eng)Authentication CheckSum Error!!!\n"); /* [KSF050806] */
#endif
			return 3;
		}

		if(memcmp(u8_Challenge, u8_I2CReceiveData, 3)==0)
		{
#if KSR_DBG_THREE /* SMC DBG LV1 */			
			/*authorization success*/
			MSG("(Eng)authorization success\n"); /* [KSF050806] */
#endif
			return 1;
		}
		else
		{
#if KSR_DBG_THREE /* SMC DBG LV1 */
			/*authorization fail*/
			MSG("(Eng)authorization fail\n"); /* [KSF050806] */
#endif
			return 0;
		}
	}
	else
	{
		return 2;
	}
}
#endif

Uint32 MemoryRead_TLI(Uint16 Offset_add, Uint8 buf_Add, Uint8 Drum)/* [KSR032907] */
{
	Uint32 read_result = 0;	/* @@2013.01.10. By CSPark */
	Uint8 CheckSum;
#if 1 /* new i2c */	
	struct i2c_msg msg[2]; /* [KSR062801] */	
#endif
	u8_I2CReceiveData[0] = SMART_IC_MEMORY_READ_CMD; /* 0x64 */
	u8_I2CReceiveData[1] = (Uint8)(Offset_add>>8);
	u8_I2CReceiveData[2] = (Uint8)(Offset_add>>0);

	if(Drum == SET) /* [KSR032907] */
	{
//#ifdef USE_DRUM_IC
#if 0
		//130110	read_result = ioctl(i2c_dev,CMD_READ_SMARTIC_MEMORY | IOCTL_MAGIC,&u8_I2CReceiveData);
		//Opeartion : memory read TLI (9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			read_result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 3) != 3){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			//read_result = -1;
		}

		memset(u8_I2CReceiveData, 0, 9);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error7\n");
			read_result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 5) != 5){ /* EE Data1 ~ EE Data4(4) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			//read_result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else /* Toner */
	{
//#ifdef USE_TONER_IC
#if 0 /* old I2C driver */
		//Opeartion : memory read TLI (9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			MSG("ioctl error - 1\n");
			read_result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 3) != 3){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);
			perror("write error\n");
			//read_result = -1;
		}

		memset(u8_I2CReceiveData, 0, 9);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			MSG("ioctl error - 2\n");			
			read_result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 5) != 5){ /* EE Data1 ~ EE Data4(4) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			perror("read error\n");			
			//read_result = -1;
		}
#endif

#if 1 /* new i2c dd */
		/* [KSR062801] */
	//	struct i2c_msg msg[2];

		msg[0].addr = SMARTIC_ADDR; 
		msg[0].flags = 0; 				//write
		msg[0].len = 3;
		msg[0].buf = u8_I2CReceiveData;  	//Header

		msg[1].addr = SMARTIC_ADDR; 
		msg[1].flags = I2C_M_RD; 			//Read	
		msg[1].len = 5;
		msg[1].buf = u8_I2CReceiveData;  	//Header
	
    	struct i2c_rdwr_ioctl_data rdwr_arg;
    	rdwr_arg.msgs = msg;
		rdwr_arg.nmsgs = 2;
		
		read_result = ioctl(fd, I2C_RDWR, &rdwr_arg);

#endif		
		
//#endif /* USE_TONER_IC - end */
	}

#if	ENG_TRACE
	MSG("(Eng)IIC : result 13 = %d\n",read_result);
#endif
	if(read_result < 0)
	{
		perror("ioctl error8\n"); /* [KSR070301] */
//		fSoftwareFailure = ERROR; /* [KSR070801] */
//		CodeOfMachineCheck = SOFTWARE_ERROR; /* [KSR070801] */
#if	ENG_TRACE
		ERR("(Eng)IIC : STATE_STUCK_LINE - Fatal Case13\n");
#endif
	}
		
#if KSR_DBG_THREE
	MSG("(Eng)Memory Read_TLI : read_result = %d Data =%x,%x,%x,%x (Check Sum=%x)\n",read_result, u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2],u8_I2CReceiveData[3],u8_I2CReceiveData[4]); /* [KSF050806] */
#endif
	CheckSum = 0xFF - (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3]);
	if(CheckSum!=u8_I2CReceiveData[4])
	{
		if((0xFF-CheckSum) !=u8_I2CReceiveData[4])
			return 0xFFFFFFFF;
	}

	TLI_GET[buf_Add] = u8_I2CReceiveData[0];
	TLI_GET[buf_Add+1] = u8_I2CReceiveData[1];
	TLI_GET[buf_Add+2] = u8_I2CReceiveData[2];
	TLI_GET[buf_Add+3] = u8_I2CReceiveData[3];

	read_result = (Uint32)(u8_I2CReceiveData[0]<<24) + (Uint32)(u8_I2CReceiveData[1]<<16) + (Uint32)(u8_I2CReceiveData[2]<<8) + (Uint32)(u8_I2CReceiveData[3]);
	return read_result;
}

Uint32 MemoryRead(Uint16 Offset_add, Uint8 Drum)/* [KSR032907] */
{
	Uint32 read_result = 0;	/* @@2013.01.10. By CSPark */
	Uint8 CheckSum;
#if 1 /* new i2c */		
	struct i2c_msg msg[2]; /* [KSR062801] */	
#endif
	u8_I2CReceiveData[0] = SMART_IC_MEMORY_READ_CMD;
	u8_I2CReceiveData[1] = (Uint8)(Offset_add>>8);
	u8_I2CReceiveData[2] = (Uint8)(Offset_add>>0);

	if(Drum == SET)
	{
//#ifdef USE_DRUM_IC
#if 0
		//130110	read_result = ioctl(i2c_dev,CMD_READ_SMARTIC_MEMORY_DRUM | IOCTL_MAGIC,&u8_I2CReceiveData);
		//Opeartion : memory read (9)
		/* ### Write ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			MSG("ioctl error - 3\n");
			read_result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 3) != 3){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			perror("write error\n");
			//read_result = -1;
		}

		memset(u8_I2CReceiveData, 0, 9);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR_DRUM ) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			MSG("ioctl error - 4\n");			
			read_result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 5) != 5){ /* EE Data1 ~ EE Data4(4) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			perror("read error\n");			
			//read_result = -1;
		}
#endif /* USE_DRUM_IC - end */
	}
	else /* Toner */
	{
#ifdef USE_TONER_IC
		//Opeartion : memory read (9)
		/* ### Write ### */
#if 0 /* old i2c driver */		
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			read_result = -1;
			//exit(0);
		}
	
		if(write(fd, u8_I2CReceiveData, 3) != 3){ /* Header(1) + Add Offset High(1) + Add Offset Low(1) */
			MSG("write error::%s()::%d\n", __func__, __LINE__);	
			//read_result = -1;
		}

		memset(u8_I2CReceiveData, 0, 9);

		/* ### Read ### */
		if(ioctl(fd, I2C_SLAVE, SMARTIC_ADDR) < 0){ /* SMARTIC_ADDR => (0x20 >> 1) */
			perror("ioctl error\n");
			read_result = -1;
			//exit(0);
		}
	
		if(read(fd, u8_I2CReceiveData, 5) != 5){ /* EE Data1 ~ EE Data4(4) + CheckSum(1) */
			MSG("read error::%s()::%d\n", __func__, __LINE__);
			//read_result = -1;
		}
#endif /* old i2c driver */

#if 1 /* new i2c dd */
		/* [KSR062801] */
		//	struct i2c_msg msg[2];

		msg[0].addr = SMARTIC_ADDR; 
		msg[0].flags = 0; 				//write
		msg[0].len = 3;
		msg[0].buf = u8_I2CReceiveData;  	//Header

		msg[1].addr = SMARTIC_ADDR; 
		msg[1].flags = I2C_M_RD; 			//Read	
		msg[1].len = 5;
		msg[1].buf = u8_I2CReceiveData;  	//Header
	
    	struct i2c_rdwr_ioctl_data rdwr_arg;
    	rdwr_arg.msgs = msg;
		rdwr_arg.nmsgs = 2;
		
		read_result = ioctl(fd, I2C_RDWR, &rdwr_arg);
#endif

#endif /* USE_TONER_IC - end */
	}

#if	ENG_TRACE
//	printf("(Eng)IIC : result 15 = %d\n",read_result);
#endif
	if(read_result < 0)
	{
		perror("ioctl error1\n"); /* [KSR070301] */
//		fSoftwareFailure = ERROR; /* [KSR070801] */
//		CodeOfMachineCheck = SOFTWARE_ERROR; /* [KSR070801] */
#if	ENG_TRACE
		ERR("(Eng)IIC : STATE_STUCK_LINE - Fatal Case15\n");
#endif
		return 0xFFFFFFFF;
	}

#if SMART_IC_TRACE
	MSG("(Eng)Memory Read : read_result = %d Data =%x,%x,%x,%x (Check Sum=%x)\n",read_result, u8_I2CReceiveData[0],u8_I2CReceiveData[1],u8_I2CReceiveData[2],u8_I2CReceiveData[3],u8_I2CReceiveData[4]);
#endif
	if(fOld_SmartIC == TRUE)
	{
		CheckSum = (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3]);
	}
	else
	{
		CheckSum = 0xFF - (Uint8)(u8_I2CReceiveData[0] + u8_I2CReceiveData[1] + u8_I2CReceiveData[2] + u8_I2CReceiveData[3]);
	}

	if(CheckSum!=u8_I2CReceiveData[4])
	{
		return 0xFFFFFFFF;
	}
	read_result = (Uint32)((Uint32)((Uint8)u8_I2CReceiveData[0] & 0x7F)<<24) + (Uint32)((Uint32)u8_I2CReceiveData[1]<<16) + (Uint32)((Uint32)u8_I2CReceiveData[2]<<8) + (Uint32)((Uint32)u8_I2CReceiveData[3]);

#if	SMART_IC_TRACE
//	printf("(Eng)Memory READ = %d \n",read_result);
#endif
	return read_result;
}

void Get_SmartIC_TLI(Uint8 ChipKind) /* [KSR032907] */
{
	MemoryRead_TLI(SC_TLI_ADDR_H,0, ChipKind);
	usleep(10000);
	MemoryRead_TLI(SC_TLI_ADDR_M,4, ChipKind);
	usleep(10000);
	MemoryRead_TLI(SC_TLI_ADDR_L,8, ChipKind);
	usleep(10000);
}

Uint8 SecurityIC_Authentication_Laminate(void)
{
	Uint32 temp_SerialNum;
	Uint32 temp_DotCount;
	Uint8  result;
	
	temp_SerialNum = MemoryRead(SC_PRODUCTION_SN_ADDR, TONER);

	usleep(10000);
	watchdog_service();

	if(temp_SerialNum != 0xFFFFFFFF)
	{
		MSG("(Eng)## Cartridge detect! : <%d> ##\n", temp_SerialNum);
	}
	else
	{
		MSG("(Eng)## Cartridge NOT detect! ##\n");
	}
	
	usleep(10000);
	watchdog_service();
	// Dot Count Check
	temp_DotCount = MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, TONER);
	if(temp_DotCount != 0xFFFFFFFF)
	{
		MESSAGE message;
		
		MSG("## DotCount <%d> ##\n", temp_DotCount);

		cDotCount = temp_DotCount;
		CurrentDotCountOnChip = temp_DotCount;
		cPageCount5perTrans = cDotCount / TRANS_COEFFICIENT_5PERCENT;
		
		Toner_Used_Percentage = (cDotCount / ((TonerCartridgeCapacity * TRANS_COEFFICIENT_5PERCENT)/100));
		if(Toner_Used_Percentage >= 100)
		{
			message.param2 = 0;
		}
		else
		{
			message.param2 = 100 - Toner_Used_Percentage;
		}
		Toner_Remain_Percentage_Old = Toner_Remain_Percentage_New = message.param2;
		
		MSG("## Toner_Used_Percentage= %d ##\n",Toner_Used_Percentage);
		
		slogit( message.msgType = MSG_STATUS_TONER );
		message.param1 = cPageCount5perTrans; 
		SYMsgSend(PRINTMGRID, &message);
		
		// System Service에서 토너 잔량 확인 할 것!!! 
		slogit( message.msgType = MSG_STATUS_TONER_REMAIN_CHK_REQ );
		SYMsgSend(PRINTMGRID, &message);
		MSG("## 5 percent Chart Trans Page Count = %d ##\n",cPageCount5perTrans);
		
		result = TRUE;
	}
	else
	{
		result = FALSE;
	}
	
	return result;
}

/****************************************************************
* Name          	 : Security IC Authentication
* Author             : SW TEAM2 (Y.J.LEE & B.Y.LEE)
* Date First Issued  : 21/12/2009
*****************************************************************/
Uint8 SecurityIC_Authentication(void) /* [KSR032908] */
{
#if SECURITY_IC_ATTACHED	
	
#if 0 /* Toner + Drum  */
	int temp_rand_seed, i;
	int SecurityResult;
	Uint8 fSmartIC_Error;
	Uint32 temp_SerialNum;
	Uint32 temp_SerialChk_year;
	Uint32 temp_SerialChk_month;
	Uint32 temp_MemoryMapVer;
	Uint32 temp_cPageCount;
	Uint32 temp_cDrumPageCount;/* [KSR032908] */
	Uint32 temp_cDrumRest;/* [KSR032908] */
	Uint32 temp_cFreeMotorOn;/* [KSR032908] */
	Uint32 temp_BrandID;
	Uint32 temp_RegionID;
	Uint32 temp_DrumType;/* [KSR032908] */
	Uint32 temp_TonerDistNum;
	Uint32 temp_DrumCartUsage;/* [KSR032908] */
	Uint32 temp_NewInfo;
	Uint32 temp_Recycle;
	Uint32 temp_DotCount;
	Uint32 temp_DrumDotCount;/* [KSR032908] */
	Uint32 temp_DotCount_Backup;
	Uint32 temp_DrumDotCount_Backup;/* [KSR032908] */
	Uint32 temp_TonerNearEnd;
	Uint32 temp_DrumNearEnd;/* [KSR032908] */
	Uint32 temp_TonerEnd;
	Uint32 temp_DrumEnd;/* [KSR032908] */
	Uint32 temp_SmartICBlock;
	Uint32 temp_DrumSmartICBlock;/* [KSR032908] */
	Uint32 temp_Factor_Page[NUM_TONER_FACTOR];/* [KSR032908] */
	Uint32 temp_Mode_Job[NUM_TONER_JOB];/* [KSR032908] */
#else /* only Toner */
	int temp_rand_seed, i;
	int SecurityResult;
	Uint8 fSmartIC_Error;
	Uint32 temp_SerialNum;
	Uint32 temp_SerialChk_year;
	Uint32 temp_SerialChk_month;
	Uint32 temp_MemoryMapVer;
	Uint32 temp_cPageCount;

	Uint32 temp_cFreeMotorOn;/* [KSR032908] */
	Uint32 temp_BrandID;
	Uint32 temp_RegionID;

	Uint32 temp_TonerDistNum;

	Uint32 temp_NewInfo;
	Uint32 temp_Recycle;
	Uint32 temp_DotCount;

	Uint32 temp_DotCount_Backup;

	Uint32 temp_TonerNearEnd;

	Uint32 temp_TonerEnd;

	Uint32 temp_SmartICBlock;
	Uint8 SmartchipTLInum;
	
#if SMART_IC_LOCK_ENABLE
	Uint32 temp_SCVersionInfo;
	Uint32 temp_TonerCheck;
	Uint32 temp_ValCheck;
	Uint32 temp_lockbit;
	Uint32 lockbit;
	Uint8 countbit;
#endif

	Uint32 temp_Factor_Page[NUM_TONER_FACTOR];/* [KSR032908] */
	Uint32 temp_Mode_Job[NUM_TONER_JOB];/* [KSR032908] */
#if SMART_IC_LOCK_ENABLE
	Uint8 	TonerCheckBaseSeed[NUM_TONER_CHECKPOINT];
	Uint32 j;
#endif
#endif
#if SMART_IC_LOCK_ENABLE
	CurrentDotCountOnChip = 0;
	fState_TonerWrong = CLEAR;
#endif

	/* Toner SmartIC */
#ifdef USE_TONER_IC
	Get_SmartIC_TLI(TONER);/* [KSR032908] */

#if KSR_DBG_ONE
	MSG("(T)MemoryRead_TLI : %c ,%c, %c ,%c ,%c ,%c ,%c ,%c ,%c ,%c ,%c ,%c \n"
	, TLI_GET[0], TLI_GET[1], TLI_GET[2], TLI_GET[3], TLI_GET[4], TLI_GET[5], TLI_GET[6], TLI_GET[7], TLI_GET[8], TLI_GET[9], TLI_GET[10], TLI_GET[11] );
#endif

	SmartchipTLInum = Get_SmartIC_KeyData(TONER);/* [KSR032908] */

	usleep(10000);
	watchdog_service();
	
#if SMART_IC_LOCK_ENABLE
	temp_SCVersionInfo = MemoryRead(SC_CHIP_VERSION_ADDR, TONER);
#if	ENG_TRACE			
	MSG("(T)(Eng)## Smart IC SW Version : <%d> ##\n", temp_SCVersionInfo);
#endif
	if(temp_SCVersionInfo != 0xFFFFFFFF)
	{
		SCVersionInfo = temp_SCVersionInfo;
		if((temp_SCVersionInfo < 3) || (temp_SCVersionInfo > 100))
		{
			if(CheckKaraMachineSmartchip(SmartchipTLInum) == FALSE) // Do not check Smartchip Revision if kara(s) smartchip.
			{
				fState_TonerWrong = TRUE;
				SC_State = ST_TONER_WRONG;
				return CARTRIDGE_CHECK_TONER_ISSUE;
			}
			else
			{
				fDataEncrytionMode = FALSE;
			}
		}
		else
		{
			fDataEncrytionMode = TRUE;
		}
	}
	else
	{
#if	ENG_TRACE		
		MSG("(T)(Eng)## Smart IC SW Version = error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#endif
	
	// Serial Number Check
	temp_SerialNum = MemoryRead(SC_PRODUCTION_SN_ADDR, TONER);
	if(temp_SerialNum != 0xFFFFFFFF)
	{
		CartridgeSerialNum = temp_SerialNum;
		MSG("(T)## SerialNum = %d ##\n",temp_SerialNum);

		// 2014.01.16 비품 smart ic 구분
		temp_SerialChk_year = temp_SerialNum / 1000000;
		temp_SerialChk_month = temp_SerialChk_year % 100;		// 생산 월
		temp_SerialChk_year = temp_SerialChk_year / 100;		// 생산 년 
#if	SMART_IC_TRACE		
		MSG("(Eng)## SerialNum Year= %d, Month= %d ##\n", temp_SerialChk_year, temp_SerialChk_month);
#endif		
		if(((temp_SerialChk_year >= 10) && (temp_SerialChk_year <= 30)) && ((temp_SerialChk_month >= 1) && (temp_SerialChk_month <= 12)))		// 2010~30년, 1~12월 이 유효범위 
		{
#if	SMART_IC_TRACE		
			MSG("(Eng)## SerialNum Chk PASS!!! ##\n");
#endif				
		}
		else
		{
#if	SMART_IC_TRACE		
			MSG("(Eng)## SerialNum Chk FAIL!!! ##\n");
#endif
			fState_TonerWrong = TRUE;
			//return CARTRIDGE_CHECK_ERROR;
		}
		
#if SMART_IC_LOCK_ENABLE
//		if(machineLocation == LOCAT_KOREA)
		if(1)
		{
			gTonerCheckinfo.f_TonerCheck = 0;
		
			gTonerCheckinfo.sn_buf[0] = (Uint8)(temp_SerialNum>>24);
			gTonerCheckinfo.sn_buf[1] = (Uint8)(temp_SerialNum>>16);
			gTonerCheckinfo.sn_buf[2] = (Uint8)(temp_SerialNum>>8);
			gTonerCheckinfo.sn_buf[3] = (Uint8)(temp_SerialNum>>0);
#if	SMART_IC_TRACE			
			MSG("(T)(Eng)## sn_buf : 0x%x, 0x%x, 0x%x, 0x%x \n",gTonerCheckinfo.sn_buf[0], gTonerCheckinfo.sn_buf[1]
			, gTonerCheckinfo.sn_buf[2], gTonerCheckinfo.sn_buf[3]);
#endif			
			TonerCheckBaseSeed[0]  = gTonerCheckinfo.sn_buf[0] ^ gTonerCheckinfo.sn_buf[1] ^ gTonerCheckinfo.sn_buf[2] ^ gTonerCheckinfo.sn_buf[3];		// 1000매 사용 check 위치 
			TonerCheckBaseSeed[1]  = TonerCheckBaseSeed[0] ^ gTonerCheckinfo.sn_buf[0];					// 2000매 사용 check 위치 
			TonerCheckBaseSeed[2]  = TonerCheckBaseSeed[1] ^ gTonerCheckinfo.sn_buf[1];					// 3000매 사용 check 위치 
			TonerCheckBaseSeed[3]  = TonerCheckBaseSeed[2] ^ gTonerCheckinfo.sn_buf[2];					// 4000매 사용 check 위치 
			TonerCheckBaseSeed[4]  = TonerCheckBaseSeed[3] ^ gTonerCheckinfo.sn_buf[3];					// 5000매 사용 check 위치 
			TonerCheckBaseSeed[5]  = TonerCheckBaseSeed[4] ^ gTonerCheckinfo.sn_buf[0];					// 6000매 사용 check 위치 
			TonerCheckBaseSeed[6]  = TonerCheckBaseSeed[5] ^ gTonerCheckinfo.sn_buf[1];					// 7000매 사용 check 위치 
			TonerCheckBaseSeed[7]  = TonerCheckBaseSeed[6] ^ gTonerCheckinfo.sn_buf[2];					// 8000매 사용 check 위치 
			TonerCheckBaseSeed[8]  = TonerCheckBaseSeed[7] ^ gTonerCheckinfo.sn_buf[3];					// 9000매 사용 check 위치 
			TonerCheckBaseSeed[9]  = TonerCheckBaseSeed[8] ^ gTonerCheckinfo.sn_buf[0];					// 10000매 사용 check 위치 
			TonerCheckBaseSeed[10] = TonerCheckBaseSeed[9] ^ gTonerCheckinfo.sn_buf[1];					// 11000매 사용 check 위치 
			TonerCheckBaseSeed[11] = TonerCheckBaseSeed[10] ^ gTonerCheckinfo.sn_buf[2];				// 12000매 사용 check 위치 
			TonerCheckBaseSeed[12] = TonerCheckBaseSeed[11] ^ gTonerCheckinfo.sn_buf[3];				// 13000매 사용 check 위치 

#if	SMART_IC_TRACE		
			MSG("(T)(Eng)## TonerCheckBaseSeed : 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \n"
			,TonerCheckBaseSeed[0],TonerCheckBaseSeed[1],TonerCheckBaseSeed[2],TonerCheckBaseSeed[3],TonerCheckBaseSeed[4],TonerCheckBaseSeed[5],TonerCheckBaseSeed[6]
			,TonerCheckBaseSeed[7],TonerCheckBaseSeed[8],TonerCheckBaseSeed[9],TonerCheckBaseSeed[10],TonerCheckBaseSeed[11],TonerCheckBaseSeed[12]);
#endif		  
			do
			{
				for(i=0; i<NUM_TONER_CHECKPOINT; i++)
				{
					for(j=1; i+j<NUM_TONER_CHECKPOINT; j++)
					{		
						if(TonerCheckBaseSeed[i] == TonerCheckBaseSeed[i+j])
						{
							TonerCheckBaseSeed[i+j] += 1;
							i=0;
							j=0;
							break;
						}
					}
					if(j==0) break;
				}
			}while (i != NUM_TONER_CHECKPOINT);
#if	SMART_IC_TRACE		
			MSG("(T)(Eng)## TonerCheckBaseSeed Result: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \n"
			,TonerCheckBaseSeed[0],TonerCheckBaseSeed[1],TonerCheckBaseSeed[2],TonerCheckBaseSeed[3],TonerCheckBaseSeed[4],TonerCheckBaseSeed[5],TonerCheckBaseSeed[6]
			,TonerCheckBaseSeed[7],TonerCheckBaseSeed[8],TonerCheckBaseSeed[9],TonerCheckBaseSeed[10],TonerCheckBaseSeed[11],TonerCheckBaseSeed[12]);
#endif
			for(i=0; i<NUM_TONER_CHECKPOINT; i++)
			{
				gTonerCheckinfo.addr_TonerCheckPoint[i] = TonerCheckBaseSeed[i] + TONER_CHECK_AREA_OFFSET;
				if(fDataEncrytionMode == TRUE)
				{
					temp_TonerCheck = temp_ValCheck = MemoryReadEncGet(gTonerCheckinfo.addr_TonerCheckPoint[i]);
				}
				else
				{
					temp_TonerCheck = temp_ValCheck = MemoryRead(gTonerCheckinfo.addr_TonerCheckPoint[i], TONER);
				}
#if	SMART_IC_TRACE				
				MSG("(T)############ temp_ValCheck = %x, %x, %x ##############\n",temp_TonerCheck, temp_ValCheck, (Uint32)(1 << i));
#endif
				if((temp_ValCheck == (Uint32)(1 << i)) || (temp_ValCheck == 0))
				{
					gTonerCheckinfo.f_TonerCheck |= temp_TonerCheck;
				}
				else
				{
#if	KSR_DBG_ERR					
					MSG("(Eng)## f_TonerCheck Read FAIL!!! \n");
#endif					
					return CARTRIDGE_CHECK_ERROR;					
				}
#if	SMART_IC_TRACE				
				MSG("(T)(Eng)## f_TonerCheck Result: 0x%x (%d - addr = %d)## \n",gTonerCheckinfo.f_TonerCheck, i, gTonerCheckinfo.addr_TonerCheckPoint[i]);
#endif
				usleep(10000);
				watchdog_service();
			}
#if	SMART_IC_TRACE		
			MSG("(T)(Eng)## f_TonerCheck = %d ##\n",gTonerCheckinfo.f_TonerCheck);
#endif
		}
#endif
	}
	else
	{
#if SECURITY_IC_ATTACHED
#if ( KSR_DBG_THREE | KSR_DBG_ERR )
		MSG("(T)## SerialNum = error ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
#else
		CartridgeSerialNum = 0;
		for(Uint8 temp_cnt=0; temp_cnt<12; temp_cnt++)
		{
			TLI_GET[temp_cnt] = 0x00;
		}
#endif
	}

	//Set_CartridgeTLISerialNotify();

	usleep(10000);
	watchdog_service();
#if SECURITY_IC_ATTACHED
	// Memory Map Ver Check /
	temp_MemoryMapVer = MemoryRead(SC_MEMORY_MAP_VER_ADDR, TONER);/* [KSR032908] */
	if(temp_MemoryMapVer != 0xFFFFFFFF)
	{
#if KSR_DBG_THREE
		MSG("(T)## Memory Map Ver = %d ## \n",temp_MemoryMapVer);
#endif
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## MemoryMapVer = error ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Brand ID Check
	temp_BrandID = MemoryRead(SC_BRAND_ID_ADDR, TONER);/* [KSR032908] */
	if(temp_BrandID == 1)
	{
#if KSR_DBG_THREE
		MSG("(T)## BRAND : SINDOH KOREA ##\n");
#endif
	}
	else if(temp_BrandID == 2)
	{
#if KSR_DBG_THREE
		MSG("(T)## BRAND : SINDOH CHINA ##\n");
#endif
	}
	else if(temp_BrandID == 3)/* [KSR032908] */
	{
#if KSR_DBG_THREE		
		MSG("(T)## BRAND : SINDOH JAPAN ##\n");
#endif			
	}
	else if(temp_BrandID == 4)/* [KSR032908] */
	{
#if KSR_DBG_THREE		
		MSG("(T)## BRAND : SINDOH NA ##\n");
#endif			
	}
	else
	{
#if ( KSR_DBG_THREE | KSR_DBG_ERR )
		MSG("(T)## BRAND : Unknown ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Region ID Check
	temp_RegionID = MemoryRead(SC_REGION_ID_ADDR, TONER); /* [KSR032908] */
	if(temp_RegionID == 1)
	{
#if KSR_DBG_THREE
		MSG("(T)## REGION : KOREA ##\n");
#endif
	}
	else if(temp_RegionID == 2)
	{
#if KSR_DBG_THREE
		MSG("(T)## REGION : CHINA ##\n");
#endif
	}
	else if(temp_RegionID == 3)
	{
#if KSR_DBG_THREE
		MSG("(T)## REGION : JAPAN ##\n");
#endif
	}
	else if(temp_RegionID == 4)
	{
#if KSR_DBG_THREE
		MSG("(T)## REGION : NA ##\n");
#endif
	}
	else
	{
#if ( KSR_DBG_THREE | KSR_DBG_ERR )
		MSG("(T)## REGION : Unknown ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Toner Cartridge Check
	temp_TonerDistNum = MemoryRead(SC_CART_DIST_ADDR, TONER);/* [KSR032908] */
	
#if	SMART_IC_TRACE					
	MSG("(Eng)## Toner Capacity Chk (%d %d) \n", TonerCapacityChk, temp_TonerDistNum);
#endif		
	if(TonerCapacityChk != temp_TonerDistNum)		// 2014.01.16 비품 smart ic 구분
	{
#if	SMART_IC_TRACE					
		MSG("(Eng)## Toner Capacity Wrong : Bad Smart IC ##\n");
#endif		
		fState_TonerWrong = TRUE;
		//return CARTRIDGE_CHECK_ERROR;
	}
	
#if KSR_DBG_THREE
		MSG("(T)## TONER CARTRIDGE INFO : ");
#endif	
#if ABBA2
	switch(temp_TonerDistNum)
	{
		case 1:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_30 ##\n");
#endif	
			break;
		case 2:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_60;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_60 ##\n");
#endif	
			break;
		case 3:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_90;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_90 ##\n");
#endif	
			break;
		case 6:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_30 ##\n");
#endif	
			break;
		case 7:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_60;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_60 ##\n");
#endif	
			break;
		case 8:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_90;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_90 ##\n");
#endif	
			break;
		default:
#if	KSR_DBG_THREE			
			MSG("Unknown Toner ##\n");
#endif		
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
			return CARTRIDGE_CHECK_ERROR;
	}
#else
	switch(temp_TonerDistNum)
	{
		case 1:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_30 ##\n");
#endif	
			break;
		case 2:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_60;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_60 ##\n");
#endif	
			break;
		case 3:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_90;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_90 ##\n");
#endif	
			break;
		case 4:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_130;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_130 ##\n");
#endif	
			break;
		case 6:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_30 ##\n");
#endif	
			break;
		case 7:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_60;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_60 ##\n");
#endif	
			break;
		case 8:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_90;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_90 ##\n");
#endif	
		case 9:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_130;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_130 ##\n");
#endif	
			break;
		case 30:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_30 ##\n");
#endif	
			break;
		case 60:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_60;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_60 ##\n");
#endif	
			break;
		case 90:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_90;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_90 ##\n");
#endif	
			break;
		case 130:
			TonerCartridgeCapacity = DEFAULT_TONER_CART_130;
#if KSR_DBG_THREE
			MSG("DEFAULT_TONER_CART_130 ##\n");
#endif	
			break;
		default:
#if	KSR_DBG_THREE			
			MSG("Unknown Toner ##\n");
#endif		
			TonerCartridgeCapacity = DEFAULT_TONER_CART_30;
			return CARTRIDGE_CHECK_ERROR;
	}
#endif

	Set_CartridgeTLISerialNotify(); /* [KSR070302] */

#if ABBA2
	if(TonerCartridgeCapacity == DEFAULT_TONER_CART_30)
	{
		TonerNearEndValue = TONER_NEAR_END_3K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_3K;
	}
	else if(TonerCartridgeCapacity == DEFAULT_TONER_CART_60)
	{
		TonerNearEndValue = TONER_NEAR_END_6K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_6K;
	}
	else if(TonerCartridgeCapacity == DEFAULT_TONER_CART_90)
	{
		TonerNearEndValue = TONER_NEAR_END_9K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_9K;
	}
	else
	{
		TonerNearEndValue = TONER_NEAR_END_3K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_3K;
	}	
#else
	if(TonerCartridgeCapacity == DEFAULT_TONER_CART_30)
	{
		TonerNearEndValue = TONER_NEAR_END_3K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_3K;
	}
	else if(TonerCartridgeCapacity == DEFAULT_TONER_CART_60)
	{
		TonerNearEndValue = TONER_NEAR_END_6K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_6K;
	}
	else if(TonerCartridgeCapacity == DEFAULT_TONER_CART_90)
	{
		TonerNearEndValue = TONER_NEAR_END_9K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_9K;
	}
	else if(TonerCartridgeCapacity == DEFAULT_TONER_CART_130)
	{
		TonerNearEndValue = TONER_NEAR_END_13K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_13K;
	}
	else
	{
		TonerNearEndValue = TONER_NEAR_END_3K_CART_VALUE;
		TonerBlockValue = TONER_BLOCK_VALUE_3K;
	}
#endif
	MSG("(T)## TonerCartridgeCapacity : (%d, %d)\n", TonerNearEndValue, TonerBlockValue);
	
	usleep(10000);
	watchdog_service();
	// New Cartridge Check
	temp_NewInfo = MemoryRead(SC_NEW_INFO_ADDR, TONER);/* [KSR032908] */
	if(temp_NewInfo == SC_NEW_TONER_NUM) /* 100 */
	{

		fNewTonerCheck = TRUE;
#if KSR_DBG_THREE
		MSG("(T)## NEW_TONER_DETACTED ##\n");
#endif
	}
	else if(temp_NewInfo == SC_USED_TONER_NUM) /* 1 */
	{
#if KSR_DBG_THREE
		MSG("(T)## USED_TONER_DETACTED ##\n");
#endif
	}
	else
	{
#if ( KSR_DBG_THREE | KSR_DBG_ERR )
		MSG("(T)## Unexpected_TONER_DETACTED (%d)##\n", temp_NewInfo);
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Recycle Check
	temp_Recycle = MemoryRead(SC_RECYCLE_INFO_ADDR, TONER);/* [KSR032908] */
	if(temp_Recycle == 1)
	{
#if KSR_DBG_THREE
		MSG("(T)## Recycle is not permited ##\n");
#endif
		fCartridgeRecycleType = RETURN_TYPE;
	}
	else if(temp_Recycle == 9)
	{
#if KSR_DBG_THREE
		MSG("(T)## Recycle is permited ##\n");
#endif
		fCartridgeRecycleType = NON_PREBATE_TYPE;
	}
	else
	{
#if ( KSR_DBG_THREE | KSR_DBG_ERR )
		MSG("(T)## Unexpected Recycle option ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Toner Near End Check
#if SMART_IC_LOCK_ENABLE
	if(fDataEncrytionMode == TRUE)
	{
		temp_TonerNearEnd = MemoryReadEncGet(SC_V3_TONER_NEAR_END_ADDR); /* [KSR032908] */
	}
	else
#else
	if(TRUE)
#endif
	{
		temp_TonerNearEnd = MemoryRead(SC_TONER_NEAR_END_ADDR, TONER); /* [KSR032908] */
	}
	
	if(temp_TonerNearEnd == 0)
	{
		fState_TonerNearEnd = CLEAR;
#if KSR_DBG_THREE
		MSG("(T)## TonerNearEnd : OK ##\n");
#endif
	}
	else if(temp_TonerNearEnd == 1)
	{
		fState_TonerNearEnd = SET; /* [KSR070104] */
#if KSR_DBG_ERR
		MSG("(T)###########################\n");
		MSG("(T)# TonerNearEnd : Near End #\n");
		MSG("(T)###########################\n");
#endif
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Unexpected TonerNearEnd option ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Toner End Check
#if SMART_IC_LOCK_ENABLE
	if(fDataEncrytionMode == TRUE)
	{
		temp_TonerEnd = MemoryReadEncGet(SC_V3_TONER_END_ADDR); /* [KSR032908] */
	}
	else
#else
	if(1)
#endif
	{
		temp_TonerEnd = MemoryRead(SC_TONER_END_ADDR, TONER); /* [KSR032908] */
	}
	if(temp_TonerEnd == 0)
	{
		fState_TonerEnd = CLEAR;
#if KSR_DBG_THREE
		MSG("(T)## Toner End : OK ##\n");
#endif
	}
	else if(temp_TonerEnd == 1)
	{
		fState_TonerEnd = SET; /* [KSR070104] */
#if KSR_DBG_ERR
		MSG("(T)###################\n");
		MSG("(T)# Toner End : End #\n");
		MSG("(T)###################\n");
#endif
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Unexpected Toner End option ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Smart IC Close Check //
#if SMART_IC_LOCK_ENABLE
	if(fDataEncrytionMode == TRUE)
	{
		temp_SmartICBlock = MemoryReadEncGet(SC_V3_TONER_BLOCK_ADDR); /* [KSR032908] */
	}
	else
#else
	if(1)
#endif
	{
		temp_SmartICBlock = MemoryRead(SC_TONER_BLOCK_ADDR, TONER); /* [KSR032908] */
	}
	if(temp_SmartICBlock == 0)
	{
		fState_TonerBlock = CLEAR;
#if KSR_DBG_THREE
		MSG("(T)## Smart IC Close : OK ##\n");
#endif
	}
	else if(temp_SmartICBlock == 1)
	{
		fState_TonerBlock = SET; /* [KSR070104] */
#if KSR_DBG_ERR
		MSG("(T)##########################\n");
		MSG("(T)# Smart IC Close : Close #\n");
		MSG("(T)##########################\n");
#endif
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Unexpected Smart IC Close option ##\n");
#endif
		return CARTRIDGE_CHECK_ERROR;
	}

	usleep(10000);
	watchdog_service();
	// Dot Count backup Check
	temp_DotCount_Backup = MemoryRead(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, TONER); /* [KSR032908] */
	if(temp_DotCount_Backup != 0xFFFFFFFF)
	{
		// cDotCount = temp_DotCount_Backup;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Unexpected DotCount_Backup ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if KSR_DBG_THREE
	MSG("(T)## cDotCount_Backup = %d ##\n",temp_DotCount_Backup);
#endif

	usleep(10000);
	watchdog_service();
	// Dot Count Check
	temp_DotCount = MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, TONER);/* [KSR032908] */
	if(temp_DotCount != 0xFFFFFFFF)
	{
		MESSAGE message;
		
#if KSR_DBG_ERR
		MSG("(T)## DotCount_Backup <%d>, DotCount <%d> ##\n",temp_DotCount_Backup, temp_DotCount);
#endif
		if((temp_DotCount_Backup > temp_DotCount) && (fNewTonerCheck == FALSE))
		{
			temp_DotCount = temp_DotCount_Backup;
		}
		cDotCount = temp_DotCount;
		CurrentDotCountOnChip = temp_DotCount;
		cPageCount5perTrans = cDotCount / TRANS_COEFFICIENT_5PERCENT;
		
		Toner_Used_Percentage = (cDotCount / ((TonerCartridgeCapacity * TRANS_COEFFICIENT_5PERCENT)/100));
		if(Toner_Used_Percentage >= 100)
		{
			message.param2 = 0;
		}
		else
		{
			message.param2 = 100 - Toner_Used_Percentage;
		}
		Toner_Remain_Percentage_Old = Toner_Remain_Percentage_New = message.param2;
		
#if KSR_DBG_ERR
		MSG("(T)## Toner_Used_Percentage= %d ##\n",Toner_Used_Percentage);
#endif		
		slogit( message.msgType = MSG_STATUS_TONER );
		message.param1 = cPageCount5perTrans; 
		SYMsgSend(PRINTMGRID, &message);
		
		// System Service에서 토너 잔량 확인 할 것!!! 
		slogit( message.msgType = MSG_STATUS_TONER_REMAIN_CHK_REQ );
		SYMsgSend(PRINTMGRID, &message);
#if KSR_DBG_ERR
		MSG("(T)## 5 percent Chart Trans Page Count = %d ##\n",cPageCount5perTrans);
#endif
#if SMART_IC_LOCK_ENABLE
//		if(machineLocation == LOCAT_KOREA)
		if(1)
		{
			//gTonerCheckinfo.f_TonerCheck = 0;
			
			//for(i=SC_TONER_USED_CHECK_1K_ADDR; i<=SC_TONER_USED_CHECK_13K_ADDR; i++)
			//{
				//temp_TonerUsedCheck = MemoryRead(i,TONER);
				//if(temp_TonerUsedCheck != 0xFFFFFFFF)
				//{
					//gTonerCheckinfo.f_TonerCheck |= temp_TonerUsedCheck;
					//printf("(Eng)## f_TonerCheck Result: 0x%x (%d)## \n",gTonerCheckinfo.f_TonerCheck, i);
					//usleep(10000);
				//}
				//else
				//{
					//return CARTRIDGE_CHECK_ERROR;
				//}
			//}
			
			if( (cPageCount5perTrans < (TonerCartridgeCapacity - TonerNearEndValue)) 
			&& ((fState_TonerNearEnd == SET) || (fState_TonerEnd == SET) || (fState_TonerBlock == SET)) )
			{
#if	ENG_TRACE
				MSG("(Eng)## TonerCapacity <%d>, NearEnd <%d> : Wrong Toner Count!!! ##\n",TonerCartridgeCapacity, TonerNearEndValue);
#endif
				fSmartICWrite_TonerBlock = TRUE;
				fSmartICWrite_WrongCount = TRUE;
				fState_TonerWrong=SET;			
			}
			
			// 1000매 단위로 기록한 Toner 사용 정보와 인쇄 환산 매수 비교
			if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_13K)
			{
				if(cPageCount5perTrans < 13000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 13000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_12K)
			{
				if(cPageCount5perTrans < 12000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 12000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_11K)
			{
				if(cPageCount5perTrans < 11000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 11000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_10K)
			{
				if(cPageCount5perTrans < 10000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 10000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_9K)
			{
				if(cPageCount5perTrans < 9000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 9000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_8K)
			{
				if(cPageCount5perTrans < 8000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 8000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_7K)
			{
				if(cPageCount5perTrans < 7000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 7000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_6K)
			{
				if(cPageCount5perTrans < 6000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 6000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_5K)
			{
				if(cPageCount5perTrans < 5000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 5000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_4K)
			{
				if(cPageCount5perTrans < 4000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 4000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_3K)
			{
				if(cPageCount5perTrans < 3000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 3000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_2K)
			{
				if(cPageCount5perTrans < 2000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 2000 ##\n");}
			}
			else if(gTonerCheckinfo.f_TonerCheck >= fTonerUsed_1K)
			{
				if(cPageCount5perTrans < 1000)		{fSmartICWrite_TonerBlock = TRUE;	fSmartICWrite_WrongCount = TRUE;	fState_TonerWrong=SET;	MSG("(Eng)## Wrong : 1000 ##\n");}
			}
		}
#endif

		//Toner_Used_Percentage = (cDotCount / ((TonerCartridgeCapacity * TRANS_COEFFICIENT_5PERCENT)/100));
		// Set_MotorStatusNotify(MOTOR_STOP, 0x00, Toner_Used_Percentage, cPageCount5perTrans);
//#if KSR_DBG_THREE
		//printf("(T)## Toner_Used_Percentage= %d ##\n",Toner_Used_Percentage);
//#endif
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## 5 percent Chart Trans Page Count Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if KSR_DBG_THREE
	MSG("(T)## cDotCount = %d ##\n",cDotCount);
#endif

	usleep(10000);
	watchdog_service();
	// Page Count Check
	temp_cPageCount = MemoryRead(SC_PAGE_COUNT_ADDR, TONER); /* [KSR032908] */
	if(temp_cPageCount != 0xFFFFFFFF)
	{
		cPageCount = temp_cPageCount;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Page Count Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if KSR_DBG_THREE
	MSG("(T)## cPageCount = %d ##\n",cPageCount);
#endif
	/* [KSR032909] */
	usleep(10000);
	watchdog_service();
	// Free Motor Count Check 
	temp_cFreeMotorOn = MemoryRead(SC_TONER_FREE_MOTOR_ON_ADDR, TONER);
	if(temp_cFreeMotorOn != 0xFFFFFFFF)
	{
		cFreeMotorOn = temp_cFreeMotorOn;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(T)## Free MotorOn Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if KSR_DBG_THREE	
	MSG("(T)## cFreeMotorOn = %d ##\n",cFreeMotorOn);
#endif

#if SMART_IC_LOCK_ENABLE
	for(i=484 ; i<500; i++)
	{
		usleep(10000);
		watchdog_service();
		
		lockbit = MemoryLockRead(i, TONER);
		if(lockbit)
		{
			for(countbit = 0; countbit < 32; countbit++)
			{
				temp_lockbit = (lockbit >> countbit) & 0x1;
				if(temp_lockbit)
				{
#if	SMART_IC_TRACE	
					MSG("(eng)## ADDR = %d (%d) ##\n", (i-484)*32+countbit, i);
#endif
				}
			}
		}
#if	SMART_IC_TRACE			
		//printf("(Eng)## Addr %d = %x ##\n",i,MemoryLockRead(i, TONER));
#endif
	}
#endif

	for (i = 0; i < NUM_TONER_FACTOR; i++)
	{
		usleep(10000);
		watchdog_service();
		// Free Motor Count Check 
		temp_Factor_Page[i] = MemoryRead(SC_TONER_HIS_FACTOR_PERCENT_1_ADDR+i, TONER);
		if(temp_Factor_Page[i] != 0xFFFFFFFF)
		{
			Factor_Page[i] = temp_Factor_Page[i];
		}
		else
		{
#if KSR_DBG_ERR
			MSG("(T)## Factor Page Error ##\n");
#endif			
			return CARTRIDGE_CHECK_ERROR;
		}
#if KSR_DBG_THREE	
		MSG("(T)## Factor_Page[%d] = %d ##\n", i, Factor_Page[i]);
#endif
		MESSAGE message;
		
		message.msgType = MSG_CARTRIDGE_PRINTED_BY_COVERAGE_INFO;
		message.param3 = (int*)Factor_Page;
		
		SYMsgSend(PRINTMGRID, &message);
	}

	for(i = 0; i < NUM_TONER_JOB; i++)
	{
		usleep(10000);
		watchdog_service();
		// Free Motor Count Check 
		temp_Mode_Job[i] = MemoryRead(SC_TONER_HIS_MODE_JOB_1_ADDR+i, TONER);
		if(temp_Mode_Job[i] != 0xFFFFFFFF)
		{
			Mode_Job[i] = temp_Mode_Job[i];
		}
		else
		{
#if KSR_DBG_ERR
			MSG("(T)## Mode Job Error ##\n");
#endif			
			return CARTRIDGE_CHECK_ERROR;
		}
#if KSR_DBG_THREE	
		MSG("(T)## Mode_Job[%d] = %d ##\n", i, Mode_Job[i]);
#endif
	}
	MESSAGE message;
	
	message.msgType = MSG_CARTRIDGE_PRINT_JOB_MODE_INFO;
	message.param1 = cFreeMotorOn;
	message.param3 = (int*)Mode_Job;
		
	SYMsgSend(PRINTMGRID, &message);

	fSmartIC_Error = NO_ERROR;

	temp_rand_seed = cProcessCounter + TransferBiasPWMForServo + Fuser_Control.Current_Temperature + cTimerStart_Motor + rand_seed_add + SmartICSeed + cPageCount;
	srand(temp_rand_seed);
	u32_SecurityCounter = rand();
	
	srand(temp_rand_seed+100);
	u32_fakeData = rand();
	
#if SMART_IC_TRACE
	MSG("(T)## Rand Func : temp_rand_seed = %d, u32_SecurityCounter = %d, u32_fakeData = %d##\n",temp_rand_seed,u32_SecurityCounter, u32_fakeData); /* [KSF050806] */
#endif

	GenerateCipherCode(TONER, 1);/* [KSR032908] */
	usleep(100000);
	watchdog_service();
	
	if(IsAuthenticationOK(TONER) != 1)
	{
#if	SMART_IC_TRACE	
		MSG("(Eng)## Original SmartIC!!! ##\n");
#endif		
		GenerateCipherCode(TONER, 0);
		usleep(100000);
		WatchDogClear(WATCHDOG_DEFAULT);

		SecurityResult = IsAuthenticationOK(TONER);/* [KSR032908] */
	}
	else
	{
#if	SMART_IC_TRACE	
		MSG("(Eng)## BAD SmartIC!!! ##\n");
#endif
		GenerateCipherCode(TONER, 0);				// 필요 없지만 정상 chip 처럼 2번 인증 시킴 
		usleep(100000);
		WatchDogClear(WATCHDOG_DEFAULT);
		
		SecurityResult = IsAuthenticationOK(TONER);
		SecurityResult = 100;				// 인증 결과와 상관없이 비품 chip으로 판단 
	}

#if SMART_IC_TRACE	
	MSG("(T)Authentication result : SecurityResult=%d, i = %d\n",SecurityResult,i); /* [KSF050806] */
#endif
/*
	if(SecurityResult!=1)
	{
#if KSR_DBG_ERR
		printf("(T)## SecurityResult Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
*/
	if(SecurityResult == 1)
	{
	}
	else if(SecurityResult == 100)
	{
		fState_TonerWrong = TRUE;
	}
	else
	{
		return CARTRIDGE_CHECK_ERROR;
	}
	
#endif /* USE_TONER_IC - end */

/*########## [KSR032901] add DRUM SMC ##########*/
//#ifdef USE_DRUM_IC
#if 0
	Get_SmartIC_TLI(DRUM); /* [KSR032910] */
		
#if	ENG_TRACE	
	MSG("(D)MemoryRead_TLI_DRUM : %c ,%c, %c ,%c ,%c ,%c ,%c ,%c ,%c ,%c ,%c ,%c \n"
	, TLI_GET[0], TLI_GET[1], TLI_GET[2], TLI_GET[3], TLI_GET[4], TLI_GET[5], TLI_GET[6], TLI_GET[7], TLI_GET[8], TLI_GET[9], TLI_GET[10], TLI_GET[11] );
#endif	
	Get_SmartIC_KeyData(DRUM);
	
	usleep(10000);
	watchdog_service();
	// Serial Number Check 
	temp_SerialNum = MemoryRead(SC_PRODUCTION_SN_ADDR, DRUM);
	if(temp_SerialNum != 0xFFFFFFFF)
	{
		CartridgeSerialNum = temp_SerialNum;
#if	ENG_TRACE		
		MSG("(D)## SerialNum_drum = %d ##\n",temp_SerialNum);
#endif			
	}
	else
	{
#if SECURITY_IC_ATTACHED
#if	ENG_TRACE		
		MSG("(D)## SerialNum_drum = error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
#else				
		CartridgeSerialNum = 0;
		for(Uint8 temp_cnt=0; temp_cnt<12; temp_cnt++)
		{
			TLI_GET[temp_cnt] = 0x00;
		}
#endif
	}

//	Set_CartridgeTLISerialNotify(); // namja
	
	
	usleep(10000);
	watchdog_service();
	// Brand ID Check 
	temp_BrandID = MemoryRead(SC_BRAND_ID_ADDR, DRUM);
	if(temp_BrandID == 1)
	{
#if	ENG_TRACE		
		MSG("(D)## BRAND : SINDOH KOREA ##\n");
#endif			
	}
	else if(temp_BrandID == 2)
	{
#if	ENG_TRACE		
		MSG("(D)## BRAND : SINDOH CHINA ##\n");
#endif			
	}
	else if(temp_BrandID == 3)
	{
#if	ENG_TRACE		
		MSG("(D)## BRAND : SINDOH USA ##\n");
#endif			
	}
	else
	{
#if	( ENG_TRACE | KSR_DBG_ERR )		
		MSG("(D)## BRAND : Unknown ##\n");
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	
	usleep(10000);
	watchdog_service();
	// Region ID Check 
	temp_RegionID = MemoryRead(SC_REGION_ID_ADDR, DRUM);
	if(temp_RegionID == 1)
	{
#if	ENG_TRACE		
		MSG("(D)## REGION : KOREA ##\n");
#endif			
	}
	else if(temp_RegionID == 2)
	{
#if	ENG_TRACE		
		MSG("(D)## REGION : CHINA ##\n");
#endif			
	}
	else if(temp_RegionID == 3)
	{
#if	ENG_TRACE		
		MSG("(D)## REGION : USA ##\n");
#endif			
	}
	else
	{
#if	( ENG_TRACE	| KSR_DBG_ERR )	
		MSG("(D)## REGION : Unknown ##\n");
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	
	usleep(10000);
	watchdog_service();
	// Region ID Check 
	temp_DrumType = MemoryRead(SC_CART_DIST_ADDR, DRUM);
	if(temp_DrumType == 1)
	{
#if	ENG_TRACE		
		MSG("(D)## KARA drum ##\n");
#endif			
	}
	else if(temp_DrumType == 2)
	{
#if	ENG_TRACE		
		MSG("(D)## KARA-S drum ##\n");
#endif			
	}
	else if(temp_DrumType == 3)
	{
#if	ENG_TRACE		
		MSG("(D)## PINETREE drum ##\n");
#endif			
	}
	else if(temp_DrumType == 4)
	{
#if	ENG_TRACE		
		MSG("(D)## PINETREE-S drum ##\n");
#endif			
	}
	else
	{
#if	( ENG_TRACE | KSR_DBG_ERR )		
		MSG("(D)## Unknown drum (%d) ##\n", temp_DrumType);
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	DrumCartridgeCapacity = 4000; /* [KSR040101] test capacity only one type (4000 -> 40000) */

	usleep(10000);
	watchdog_service();
	// Toner Cartridge Check 
	temp_DrumCartUsage = MemoryRead(SC_DRUM_CART_USAGE_ADDR, DRUM);
	
	if(temp_DrumCartUsage == 1)
	{
#if	ENG_TRACE			
		MSG("(D)## DRUM CARTRIDGE USAGE : FORCE CLOSE (%d) ##\n", temp_DrumCartUsage);
#endif
		return CARTRIDGE_CHECK_ERROR;
	}
	else
	{
#if	ENG_TRACE			
		MSG("(D)## DRUM CARTRIDGE USAGE : CONTINUE (%d) ##\n", temp_DrumCartUsage);
#endif		
	}

	usleep(10000);
	watchdog_service();
	// Toner Near End Check 
	temp_DrumNearEnd = MemoryRead(SC_TONER_NEAR_END_ADDR, DRUM);
	if(temp_DrumNearEnd == 0)
	{
		fState_DrumNearEnd = CLEAR;
#if	ENG_TRACE			
		MSG("(D)## DrumNearEnd : OK ##\n");
#endif			
	}
	else if(temp_DrumNearEnd == 1)
	{
		fState_DrumNearEnd = SET;
#if	( ENG_TRACE | KSR_DBG_ERR )			
		MSG("(D)###########################\n");
		MSG("(D)# DrumNearEnd : Near End #\n");
		MSG("(D)###########################\n");
#endif			
	}
	else
	{
#if	( ENG_TRACE | KSR_DBG_ERR )
		MSG("(D)## Unexpected DrumNearEnd option ##\n");
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	
	usleep(10000);
	watchdog_service();
	// Toner End Check 
	temp_DrumEnd = MemoryRead(SC_TONER_END_ADDR, DRUM);
	if(temp_DrumEnd == 0)
	{
		fState_DrumEnd = CLEAR;
#if	ENG_TRACE			
		MSG("(D)## Drum End : OK ##\n");
#endif			
	}
	else if(temp_DrumEnd == 1)
	{
		fState_DrumEnd = SET;
#if	( ENG_TRACE | KSR_DBG_ERR )
		MSG("(D)###################\n");
		MSG("(D)# Drum End : End #\n");
		MSG("(D)###################\n");
#endif			
	}
	else
	{
#if	( ENG_TRACE | KSR_DBG_ERR )		
		MSG("(D)## Unexpected Drum End option ##\n");
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	
	usleep(10000);
	watchdog_service();
	// Smart IC Close Check //
	temp_DrumSmartICBlock = MemoryRead(SC_TONER_BLOCK_ADDR, DRUM);
	if(temp_DrumSmartICBlock == 0)
	{
		fState_DrumBlock = CLEAR;
#if	ENG_TRACE
		MSG("(D)## Drum Smart IC Close : OK ##\n");
#endif			
	}
	else if(temp_DrumSmartICBlock == 1)
	{
		fState_DrumBlock = SET;
#if	( ENG_TRACE | KSR_DBG_ERR )
		MSG("(D)##############################\n");
		MSG("(D)# Drum Smart IC Close : Close #\n");
		MSG("(D)##############################\n");
#endif		
	}
	else
	{
#if	( ENG_TRACE | KSR_DBG_ERR )
		MSG("(D)## Unexpected Drum Smart IC Close option ##\n");
#endif			
		return CARTRIDGE_CHECK_ERROR;
	}
	
	usleep(10000);
	watchdog_service();
	// Dot Count Check 
	temp_DrumDotCount_Backup = MemoryRead(SC_PAGE_COUNT_CONVERSION_BACKUP_ADDR, DRUM);
	if(temp_DrumDotCount_Backup != 0xFFFFFFFF)
	{
		// cDotCount = temp_DotCount_Backup;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(D)## DrumDotCount Backup Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if	ENG_TRACE	
	MSG("(D)## cDrumDotCount_Backup = %d ##\n",temp_DrumDotCount_Backup);
#endif

	usleep(10000);
	watchdog_service();
	// Dot Count Check 
	temp_DrumDotCount = MemoryRead(SC_PAGE_COUNT_CONVERSION_ADDR, DRUM);
	if(temp_DrumDotCount != 0xFFFFFFFF)
	{
#if	ENG_TRACE	
		MSG("(D)## DrumDotCount_Backup <%d>, DrumDotCount <%d> ##\n",temp_DrumDotCount_Backup, temp_DrumDotCount);
#endif		
		if((temp_DrumDotCount_Backup > temp_DrumDotCount) && (fNewTonerCheck == FALSE))
		{
			temp_DrumDotCount = temp_DrumDotCount_Backup;
		}
		cDrumDotCount = temp_DrumDotCount;
		cDrumPageCount5perTrans = cDrumDotCount / TRANS_COEFFICIENT_5PERCENT;
#if	ENG_TRACE		
		MSG("(D)## 5 percent Chart Trans Page Count_drum = %d ##\n",cDrumPageCount5perTrans);
#endif		
		Drum_Used_Percentage = (cDrumDotCount / ((DrumCartridgeCapacity/*4000*/ * TRANS_COEFFICIENT_5PERCENT)/10));
		// Set_MotorStatusNotify(MOTOR_STOP, 0x00, Toner_Used_Percentage, cPageCount5perTrans);
#if	ENG_TRACE		
		MSG("(D)## Drum_Used_Percentage= %d ##\n",Drum_Used_Percentage);
#endif		
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(D)## Drum_Used_Percentage Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if	ENG_TRACE	
	MSG("(D)## cDrumDotCount = %d ##\n",cDrumDotCount);
#endif	
	
	usleep(10000);
	watchdog_service();
	// Page Count Check 
	temp_cDrumPageCount = MemoryRead(SC_PAGE_COUNT_ADDR, DRUM);
	if(temp_cDrumPageCount != 0xFFFFFFFF)
	{
		cDrumPageCount = temp_cDrumPageCount;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(D)## Drum Page Count Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if	ENG_TRACE	
	MSG("(D)## cDrumPageCount = %d ##\n",cDrumPageCount);
#endif
	
	usleep(10000);
	watchdog_service();
	// Page Count Check 
	temp_cDrumRest = MemoryRead(SC_DRUM_REST_ADDR, DRUM);
	if(temp_cDrumRest != 0xFFFFFFFF)
	{
		cDrumRest = temp_cDrumRest;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(D)## Drum Rest Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if	ENG_TRACE	
	MSG("(D)## cDrumRest = %d ##\n",cDrumRest);
#endif

	usleep(10000);
	watchdog_service();
	// Free Motor Count Check 
	temp_cFreeMotorOn = MemoryRead(SC_TONER_FREE_MOTOR_ON_ADDR, DRUM);
	if(temp_cFreeMotorOn != 0xFFFFFFFF)
	{
		cDrumFreeMotorOn = temp_cFreeMotorOn;
	}
	else
	{
#if KSR_DBG_ERR
		MSG("(D)## Free MotorOn Error ##\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#if	ENG_TRACE	
	MSG("(D)## cDrumFreeMotorOn = %d ##\n",cDrumFreeMotorOn);
#endif

	for (i = 0; i < NUM_TONER_FACTOR; i++)
	{
		usleep(10000);
		watchdog_service();
		// Free Motor Count Check 
		temp_Factor_Page[i] = MemoryRead(SC_TONER_HIS_FACTOR_PERCENT_1_ADDR+i, DRUM);
		if(temp_Factor_Page[i] != 0xFFFFFFFF)
		{
			Drum_Factor_Page[i] = temp_Factor_Page[i];
		}
		else
		{
#if KSR_DBG_ERR
			MSG("(D)## Facrot Page Error ##\n");
#endif			
			return CARTRIDGE_CHECK_ERROR;
		}
#if	ENG_TRACE	
		MSG("(D)## Drum_Factor_Page[%d] = %d ##\n", i, Drum_Factor_Page[i]);
#endif
	}

	for(i = 0; i < NUM_TONER_JOB; i++)
	{
		usleep(10000);
		watchdog_service();
		// Free Motor Count Check 
		temp_Mode_Job[i] = MemoryRead(SC_TONER_HIS_MODE_JOB_1_ADDR+i, DRUM);
		if(temp_Mode_Job[i] != 0xFFFFFFFF)
		{
			Drum_Mode_Job[i] = temp_Mode_Job[i];
		}
		else
		{
#if KSR_DBG_ERR
		MSG("(D)## Drum Mode Job Error ##\n");
#endif			
			return CARTRIDGE_CHECK_ERROR;
		}
#if	ENG_TRACE	
		MSG("(D)## Drum_Mode_Job[%d] = %d ##\n", i, Drum_Mode_Job[i]);
#endif
	}


	fSmartIC_Error = NO_ERROR;
	
	temp_rand_seed = cProcessCounter + TransferBiasPWMForServo + Fuser_Control.Current_Temperature + cTimerStart_Motor + rand_seed_add + SmartICSeed + cPageCount;
	srand(temp_rand_seed);
	u32_SecurityCounter = rand();
	
#if	SMART_IC_TRACE		
	MSG("(D)## Rand Func : temp_rand_seed = %d, u32_SecurityCounter = %d ##\n",temp_rand_seed,u32_SecurityCounter);
#endif

	GenerateCipherCode(DRUM);
	usleep(100000);
	watchdog_service();

	SecurityResult = IsAuthenticationOK(DRUM);

#if	SMART_IC_TRACE	
	MSG("(D)Authentication result : SecurityResult=%d, i = %d\n",SecurityResult,i);
#endif
	/* [KSR032910] drum - end */

	if(SecurityResult!=1)
	{
#if	( SMART_IC_TRACE | KSR_DBG_ERR )
		MSG("(D) Drum CARTRIDGE_CHECK_ERROR\n");
#endif		
		return CARTRIDGE_CHECK_ERROR;
	}
#endif /* USE_DRUM_IC - end */

#ifdef USE_TONER_IC
#if SMART_IC_LOCK_ENABLE
	if(fState_TonerWrong==SET)
	{
		SC_State = ST_TONER_WRONG;
#if KSR_DBG_THREE
		MSG("(T)CARTRIDGE_CHECK_TONER_ISSUE : ST_TONER_WRONG\n");
#endif		
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
#endif
	if(fState_TonerBlock==SET)
	{
		SC_State = ST_TONER_BLOCK;
#if KSR_DBG_THREE
		MSG("(T)CARTRIDGE_CHECK_TONER_ISSUE : ST_TONER_BLOCK\n");
#endif		
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else if(fState_TonerEnd==SET)
	{
		SC_State = ST_TONER_END;
#if KSR_DBG_THREE
		MSG("(T)CARTRIDGE_CHECK_TONER_ISSUE : ST_TONER_END\n");
#endif		
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else if(fState_TonerNearEnd==SET)
	{
		SC_State = ST_TONER_NEAR_END;
#if KSR_DBG_THREE
		MSG("(T)CARTRIDGE_CHECK_TONER_ISSUE : ST_TONER_NEAR_END\n");
#endif			
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else
	{
		//FuserControlMsgSet.fSCerror = CLEAR; /* [KSR070304] */
		SC_State = ST_NO_ISSUE;
#if KSR_DBG_THREE
		MSG("(T)CARTRIDGE_CHECK_OK : ST_NO_ISSUE\n");  /* [KSR050901] */
#endif	
		return CARTRIDGE_CHECK_OK;
	}
#endif /* USE_TONER_IC - end */	
	
//#ifdef USE_DRUM_IC
#if 0	
	/* [KSR032910] */
	if(fState_DrumBlock==SET)
	{
		SC_State = ST_TONER_BLOCK;
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else if(fState_DrumEnd==SET)
	{
		SC_State = ST_TONER_END;
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else if(fState_DrumNearEnd==SET)
	{
		SC_State = ST_TONER_NEAR_END;
		return CARTRIDGE_CHECK_TONER_ISSUE;
	}
	else
	{
		SC_State = ST_NO_ISSUE;
		return CARTRIDGE_CHECK_OK;
	}
#endif /* USE_DRUM_IC - end */	
	
#else
	return CARTRIDGE_CHECK_OK;
#endif

#else /* SECURITY_IC_ATTACHED (no chip) */
	return CARTRIDGE_CHECK_OK;
#endif /* SECURITY_IC_ATTACHED */
}

void ReAuthenticationForWrite(Uint8 cnt)
{
	int temp_rand_seed;
	
	temp_rand_seed = cProcessCounter + TransferBiasPWMForServo + FuserControlMsgSet.Current_Temperature + cTimerStart_Motor + rand_seed_add + SmartICSeed + cPageCount;
	srand(temp_rand_seed);
	u32_SecurityCounter = rand();
	
	GenerateCipherCode(TONER, 0);
	usleep(100000);
	WatchDogClear(WATCHDOG_DEFAULT);

	if(IsAuthenticationOK(TONER) == 1)
	{
		//fSmartICWrite_DotCount = TRUE; 
		//fMemoryWrite = TRUE;
		usleep(100000);
#if	ENG_TRACE
		MSG("(Eng)## ReAuthenticationForWrite : PASS !!! ##\n");
#endif
		cJobEndDotCountWriteFailCnt = 0;
	}
	else if(cnt >= 2)
	{
#if	ENG_TRACE
		MSG("(Eng)## ReAuthenticationForWrite : Fail !!! ##\n");
#endif			
		//fSC_Writefault = TRUE;
		//SC_State = ST_CERT_FAILULE;
	}
}

void SecurityIC_Check(void)
{
#if SECURITY_IC_ATTACHED	
	Uint8 fSC_Authentication_Result;

	rand_seed_add = 0;
	fSC_Authentication_Result = SecurityIC_Authentication();
	if(fSC_Authentication_Result == CARTRIDGE_CHECK_ERROR)
	{
		rand_seed_add = 100;
#if KSR_DBG_THREE
		MSG("(Eng)## Smart IC READ FAIL : 2nd RETRY !!! ##\n");  /* [KSR050901] */
#endif
		fSC_Authentication_Result = SecurityIC_Authentication();
		if(fSC_Authentication_Result == CARTRIDGE_CHECK_ERROR)
		{
			rand_seed_add = 200;
#if KSR_DBG_THREE
			MSG("(Eng)## Smart IC READ FAIL !!! : 3rd RETRY !!! ##\n");  /* [KSR050901] */
#endif
			fSC_Authentication_Result = SecurityIC_Authentication();
			if(fSC_Authentication_Result == CARTRIDGE_CHECK_ERROR)
			{
#if ( KSR_DBG_ERR | KSR_DBG_THREE )
			MSG("(Eng)## Smart IC READ FAIL !!! : ERROR !!! ##\n");  /* [KSR050901] */
#endif
				//FuserControlMsgSet.fSCerror = SET; /* [KSR070305] */
				SC_State = ST_CERT_FAILULE;
			}
		}
	}
#endif /* SECURITY_IC_ATTACHED */	
}

Uint8 SmartICInfoNotify(void)
{
	if(SC_State == ST_NO_ISSUE)
	{
#if KSR_DBG_THREE
			MSG("(T) SmartICInfoNotify : ST_NO_ISSUE\n");  /* [KSR050901] */
#endif		
		Set_SmartICInfoNotify(CARTRIDGE_CMD_NO_ISSUE,0x00);
//		return SC_OK; /* [KSR070306] */
	}
	else if(SC_State == ST_TONER_NEAR_END)
	{
#if KSR_DBG_ERR
			MSG("(T) SmartICInfoNotify : ST_TONER_NEAR_END\n");  /* [KSR050901] */
#endif		
		Set_SmartICInfoNotify(CARTRIDGE_CMD_NO_ISSUE,0x00); /* [KSR070203] *//* [KSR070402] */
		fTonerLowMsgPending = TRUE;
		//Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_LOW); /* [KSR070402] */
//		return SC_OK; /* [KSR070306] */
	}
	else if(SC_State == ST_TONER_END)
	{
#if KSR_DBG_ERR
			MSG("(T) SmartICInfoNotify : ST_TONER_END\n"); /* [KSR050901] */
#endif		
		Set_SmartICInfoNotify(CARTRIDGE_CMD_NO_ISSUE,0x00);/* [KSR070402] */
		fTonerLowMsgPending = TRUE;
		//Set_SmartICInfoNotify(CARTRIDGE_CMD_TONER_LOW_END, PARAM_TONER_END); /* [KSR070402] */
//		return SC_OK;/* [KSR070306] */
	}
	else if (SC_State == ST_CERT_FAILULE)
	{
#if KSR_DBG_ERR
			MSG("(T) SmartICInfoNotify : ST_CERT_FAILULE\n");  /* [KSR050901] */
#endif			
		if(fSoftwareFailure == ERROR)
		{
			Set_MachineCheckNotify(CodeOfMachineCheck);
		}
//		Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE, 0x00);
		return SC_FAIL;
	}
#if SMART_IC_LOCK_ENABLE
	else if (SC_State == ST_TONER_WRONG)
	{
		if(fSoftwareFailure == ERROR)
		{
			Set_MachineCheckNotify(CodeOfMachineCheck);
		}
		Set_SmartICInfoNotify(CARTRIDGE_CMD_WRONG_DATA, 0x00);
		return SC_FAIL;
	}
#endif
	else if (SC_State == ST_TONER_BLOCK)
	{
#if KSR_DBG_ERR
			MSG("(T) SmartICInfoNotify : ST_TONER_BLOCK\n");  /* [KSR050901] */
#endif	
		Set_MotorStatusNotify(MOTOR_STOP, 0x00, Toner_Used_Percentage, cPageCount5perTrans);
		Set_SmartICInfoNotify(CARTRIDGE_CMD_CLOSE_FAILULE, 0x00);
		return SC_FAIL;
	}
	else
	{
#if KSR_DBG_ERR
			MSG("(T) SmartICInfoNotify : ST_CERT_FAILULE (default)\n"); /* [KSR050901] */
#endif		
		Set_MachineCheckNotify(CodeOfMachineCheck);
		Set_SmartICInfoNotify(CARTRIDGE_CMD_CERT_FAILULE, 0x00);
		return SC_FAIL;
	}
	
	return SC_OK;
}

#if 1 /* [KSF061702] new TLI */
Uint8 Get_SmartIC_KeyData(Uint8 Drum)
{
	Uint8	Toner_SmartIC_TLI;
	Uint8	Drum_SmartIC_TLI;
	
	Drum_SmartIC_TLI = DR_WRONGTLI;
	Toner_SmartIC_TLI = TN_WRONGTLI;
	
	TonerCapacityChk = 0;
	/* [KSF061703]  */
#if KSR_DBG_THREE	
	MSG("(1) before Machine_TLI:%d\n",Machine_TLI);
#endif	
//	Machine_TLI = TLI_DEFAULT;
#if KSR_DBG_THREE
	MSG("(2) after Machine_TLI:%d\n",Machine_TLI);
#endif
	if(Drum == SET)
	{
		if(strncmp((char*)TLI_GET,"A610R40K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610R40K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40) || (Machine_TLI == TLI_A615DN40) || (Machine_TLI == TLI_A611DN45) || (Machine_TLI == TLI_A616DN45) 
			|| (Machine_TLI == TLI_M61140) || (Machine_TLI == TLI_M61640) || (Machine_TLI == TLI_M61245) || (Machine_TLI == TLI_M61745) 
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Drum_SmartIC_TLI = DR_A610R40K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		
		else if(1)
		{
			Drum_SmartIC_TLI = DR_TEMPORARILY;
		}
		else
		{
#if	( ENG_TRACE | KSR_DBG_ERR )		
			MSG("(Eng)### TLI CHECK : Wrong Drum Smart IC ###\n");
#endif		
			Drum_SmartIC_TLI = DR_WRONGTLI;
		}
		int i;
		for(i=0; i<NumOfKeyData; i++)
		{
			u8_Key[i] = Drum_KeyData[Drum_SmartIC_TLI][i];
		}
	}
	else
	{
		if(strncmp((char*)TLI_GET,"S400T2HK",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : S400T2HK ###\n");
#endif
			TonerCapacityChk = 6;
#if MACHINE_TLI_READ
#if ABBA2
			if((Machine_TLI == TLI_A40025) || (Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40524) || (Machine_TLI == TLI_A405T24)  || (Machine_TLI == TLI_A611DN45) // abba2 test
			|| (Machine_TLI == TLI_A405G24) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632) 
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#else
			if((Machine_TLI == TLI_A40025) || (Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40524) || (Machine_TLI == TLI_A405T24)
			|| (Machine_TLI == TLI_A405G24) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632) 
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#endif
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"S400T5K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : S400T5K ###\n");
#endif
			TonerCapacityChk = 7;
#if MACHINE_TLI_READ
#if ABBA2
			if((Machine_TLI == TLI_A40025) || (Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40524) || (Machine_TLI == TLI_A405T24) || (Machine_TLI == TLI_A611DN45) // abba2 test
			|| (Machine_TLI == TLI_A405G24) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632) 
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#else
			if((Machine_TLI == TLI_A40025) || (Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40524) || (Machine_TLI == TLI_A405T24)
			|| (Machine_TLI == TLI_A405G24) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632) 
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#endif
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T5K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"S400T8K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : S400T8K ###\n");
#endif
			TonerCapacityChk = 8;
#if MACHINE_TLI_READ
#if ABBA2
			if((Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632) || (Machine_TLI == TLI_A611DN45) // abba2 test
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#else
			if((Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_A40632)
			|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#endif
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T8K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"S400ST2HK",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : S400ST2HK ###\n");
#endif
	
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400S25) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400ST2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"S400ST5K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : S400ST5K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400S25) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400ST5K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA25T1HKA",12)==0)
		{
#if	ENG_TRACE				
		MSG("(Eng)### TLI CHECK : LA25T1HKA ###\n");
#endif

#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40025) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA25T1HKA;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA25T1HKB",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA25T1HKB ###\n");
#endif
	
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400S25) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA25T1HKB;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA25T2HK",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA25T2HK ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40524) || (Machine_TLI == TLI_A405T24) || (Machine_TLI == TLI_A405G24) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA25T2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA25T8K",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA25T8K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400X25) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA25T8K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA30T2HK",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA30T2HK ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40132) || (Machine_TLI == TLI_A1A401DN32) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA30T2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif
			}
		}
		else if(strncmp((char*)TLI_GET,"LA35T5K",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA35T5K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A402DN36) || (Machine_TLI == TLI_A40632)	|| (Machine_TLI == TLI_A2A406DN32) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA35T5K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA35T8K",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA35T8K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A407DN36) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA35T8K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA25T1HK-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA25T1HK-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40025_C) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif 
			{
				Toner_SmartIC_TLI = TN_LA25T1HK_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif				
			}
		}
		else if(strncmp((char*)TLI_GET,"LA30T2HK-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA30T2HK-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40132_C) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA30T2HK_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"LA35T5K-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : LA35T5K-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40236_C) || (Machine_TLI == TLI_C1A40236_C) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_LA35T5K_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}	
		else if(strncmp((char*)TLI_GET,"S400T2HK-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : S400T2HK-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40025_C) || (Machine_TLI == TLI_A40132_C) || (Machine_TLI == TLI_A40236_C) || (Machine_TLI == TLI_C1A40236_C) 
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T2HK_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"S400T5K-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : S400T5K-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40025_C) || (Machine_TLI == TLI_A40132_C) || (Machine_TLI == TLI_A40236_C) || (Machine_TLI == TLI_C1A40236_C) 
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T5K_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"S400T8K-C",12)==0)
		{
#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : S400T8K-C ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A40025_C) || (Machine_TLI == TLI_A40132_C) || (Machine_TLI == TLI_A40236_C) || (Machine_TLI == TLI_C1A40236_C) 
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_S400T8K_C;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
	
	//Oak Smart IC
	
		else if(strncmp((char*)TLI_GET,"M400T2HK",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400T2HK ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40025) || (Machine_TLI == TLI_M40525) || (Machine_TLI == TLI_M400X25) || (Machine_TLI == TLI_M405G25) 
			|| (Machine_TLI == TLI_M40125) || (Machine_TLI == TLI_M40625) || (Machine_TLI == TLI_M401X25) || (Machine_TLI == TLI_M406G25)
			|| (Machine_TLI == TLI_M40230) || (Machine_TLI == TLI_M40335) || (Machine_TLI == TLI_M40730) || (Machine_TLI == TLI_M40835)
			|| (Machine_TLI == TLI_M402X30) || (Machine_TLI == TLI_M403X35) || (Machine_TLI == TLI_M407G30) || (Machine_TLI == TLI_M408G35)
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400T2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400T5K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400T5K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40025) || (Machine_TLI == TLI_M40525) || (Machine_TLI == TLI_M400X25) || (Machine_TLI == TLI_M405G25) 
			|| (Machine_TLI == TLI_M40125) || (Machine_TLI == TLI_M40625) || (Machine_TLI == TLI_M401X25) || (Machine_TLI == TLI_M406G25)
			|| (Machine_TLI == TLI_M40230) || (Machine_TLI == TLI_M40335) || (Machine_TLI == TLI_M40730) || (Machine_TLI == TLI_M40835)
			|| (Machine_TLI == TLI_M402X30) || (Machine_TLI == TLI_M403X35) || (Machine_TLI == TLI_M407G30) || (Machine_TLI == TLI_M408G35)
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400T5K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M400T8K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400T8K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M400X25) || (Machine_TLI == TLI_M401X25) || (Machine_TLI == TLI_M402X30) || (Machine_TLI == TLI_M40335)
			|| (Machine_TLI == TLI_M403X35) || (Machine_TLI == TLI_M40835) || (Machine_TLI == TLI_M408G35) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400T8K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400ST2HK",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400ST2HK ###\n");
#endif
	
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M400S25) || (Machine_TLI == TLI_M401S25) || (Machine_TLI == TLI_M402S30) || (Machine_TLI == TLI_M403S35)
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400ST2HK;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400ST5K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400ST5K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M400S25) || (Machine_TLI == TLI_M401S25) || (Machine_TLI == TLI_M402S30) || (Machine_TLI == TLI_M403S35)
			|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400ST5K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400ST8K",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M400ST8K ###\n");
#endif
		
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M403S35)	|| (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M400ST8K;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M012T1HKA",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M012T1HKA ###\n");
#endif

#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40025) || (Machine_TLI == TLI_M40125) || (Machine_TLI == TLI_DEFAULT))
#else
			if(1)
#endif
			{
				Toner_SmartIC_TLI = TN_M012T1HKA;
			}
			else
			{
#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M012T2HK",12)==0)
		{
#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M012T2HK ###\n");
#endif
	
#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40525) || (Machine_TLI == TLI_M405G25) || (Machine_TLI == TLI_M40625) || (Machine_TLI == TLI_M406G25)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M012T2HK;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M012T8K",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M012T8K ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M400X25) || (Machine_TLI == TLI_M401X25) || (Machine_TLI == TLI_M402X30) || (Machine_TLI == TLI_M403X35)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M012T8K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M032T1HK",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M032T1HK ###\n");
	#endif
	
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40230) || (Machine_TLI == TLI_M40335) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M032T1HK;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M032T2HK",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M032T2HK ###\n");
	#endif
		
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M40730) || (Machine_TLI == TLI_M40835) || (Machine_TLI == TLI_M407G30) || (Machine_TLI == TLI_M408G35) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M032T2HK;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M012T1HKB",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M012T1HKB ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M400S25) || (Machine_TLI == TLI_M401S25) || (Machine_TLI == TLI_M402S30) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M012T1HKB;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M012T2HKB",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M012T2HKB ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M403S35) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M012T2HKB;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
			else if(strncmp((char*)TLI_GET,"M032T2HK-C",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M032T2HK-C ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_C1M40230_C) || (Machine_TLI == TLI_C2M40335_C) || (Machine_TLI == TLI_C3M40230_C) || (Machine_TLI == TLI_C4M40335_C)
			|| (Machine_TLI == TLI_C5M40125_C) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M032T2HK_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}	
		else if(strncmp((char*)TLI_GET,"M400T2HK-C",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M400T2HK-C ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_C1M40230_C) || (Machine_TLI == TLI_C2M40335_C) || (Machine_TLI == TLI_C3M40230_C) || (Machine_TLI == TLI_C4M40335_C)
			|| (Machine_TLI == TLI_C5M40125_C) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M400T2HK_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400T5K-C",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M400T5K-C ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_C1M40230_C) || (Machine_TLI == TLI_C2M40335_C) || (Machine_TLI == TLI_C3M40230_C) || (Machine_TLI == TLI_C4M40335_C)
			|| (Machine_TLI == TLI_C5M40125_C) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M400T5K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M400T8K-C",12)==0)
		{
	#if	ENG_TRACE			
			MSG("(Eng)### TLI CHECK : M400T8K-C ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_C1M40230_C) || (Machine_TLI == TLI_C2M40335_C) || (Machine_TLI == TLI_C3M40230_C) || (Machine_TLI == TLI_C4M40335_C)
			|| (Machine_TLI == TLI_C5M40125_C) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M400T8K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		
		//Oak Fax Smart IC
		
		else if(strncmp((char*)TLI_GET,"F400T2HK",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : F400T2HK ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F40025) || (Machine_TLI == TLI_F40525) || (Machine_TLI == TLI_F405G25) || (Machine_TLI == TLI_F400X25) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_F400T2HK;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"F400T5K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : F400T5K ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F40025) || (Machine_TLI == TLI_F40525) || (Machine_TLI == TLI_F405G25) || (Machine_TLI == TLI_F400X25) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_F400T5K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
	
		else if(strncmp((char*)TLI_GET,"F400ST2HK",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : F400ST2HK ###\n");
	#endif
		
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F400S25) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_F400ST2HK;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"F400ST5K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : F400ST5K ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F400S25) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_F400ST5K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}
		}
		else if(strncmp((char*)TLI_GET,"M015T1HKA",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M015T1HKA , Machine_TLI = %d ###\n",Machine_TLI);
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F40025) || (Machine_TLI == TLI_F40525) || (Machine_TLI == TLI_F405G25) || (Machine_TLI == TLI_F400X25) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M015T1HKA;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M015T1HKB",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M015T1HKB ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_F400S25) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M015T1HKB;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		
		// 북미향 카트리지 (ABBA)
		else if(strncmp((char*)TLI_GET,"NA400T2HKR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T2HKR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T2HKR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NA400T5KR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T5KR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T5KR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NA400T8KR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T8KR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T8KR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NA400T2HKN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T2HKN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T2HKN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NA400T5KN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T5KN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T5KN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NA400T8KN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NA400T8KN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NA400T8KN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA30T2HK-N",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA30T2HK-N ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A400N10) || (Machine_TLI == TLI_A401N21) || (Machine_TLI == TLI_A402N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA30T2HK_N;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		
		// 북미향 카트리지 (OAK)
		else if(strncmp((char*)TLI_GET,"NM400T2HKR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T2HKR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T2HKR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NM400T5KR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T5KR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T5KR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NM400T8KR",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T8KR ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T8KR;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NM400T2HKN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T2HKN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T2HKN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NM400T5KN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T5KN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T5KN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"NM400T8KN",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : NM400T8KN ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_NM400T8KN;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M032T2HK-N",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M032T2HK-N ###\n");
	#endif
			
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M401N10) || (Machine_TLI == TLI_M402N21) || (Machine_TLI == TLI_M403N21) || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M032T2HK_N;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		
		// Kara Official
		else if(strncmp((char*)TLI_GET,"A610T3K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T3K ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40) || (Machine_TLI == TLI_A615DN40) || (Machine_TLI == TLI_A611DN45) || (Machine_TLI == TLI_A616DN45) || (Machine_TLI == TLI_A616DNS45)
			 || (Machine_TLI == TLI_A610DN40H) || (Machine_TLI == TLI_A615DN40H) || (Machine_TLI == TLI_A611DN45H) || (Machine_TLI == TLI_A616DN45H) || (Machine_TLI == TLI_A616DNS45H)
			 || (Machine_TLI == TLI_A611DN45HHU) || (Machine_TLI == TLI_A611DN45HH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T3K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T6K ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40) || (Machine_TLI == TLI_A615DN40) || (Machine_TLI == TLI_A611DN45) || (Machine_TLI == TLI_A616DN45) || (Machine_TLI == TLI_A616DNS45)
			 || (Machine_TLI == TLI_A610DN40H) || (Machine_TLI == TLI_A615DN40H) || (Machine_TLI == TLI_A611DN45H) || (Machine_TLI == TLI_A616DN45H) || (Machine_TLI == TLI_A616DNS45H)
			 || (Machine_TLI == TLI_A611DN45HHU) || (Machine_TLI == TLI_A611DN45HH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T13K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T13K ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40) || (Machine_TLI == TLI_A615DN40) || (Machine_TLI == TLI_A611DN45) || (Machine_TLI == TLI_A616DN45) || (Machine_TLI == TLI_A616DNS45)
			 || (Machine_TLI == TLI_A610DN40H) || (Machine_TLI == TLI_A615DN40H) || (Machine_TLI == TLI_A611DN45H) || (Machine_TLI == TLI_A616DN45H) || (Machine_TLI == TLI_A616DNS45H)
			 || (Machine_TLI == TLI_A611DN45HHU) || (Machine_TLI == TLI_A611DN45HH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T13K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T6K ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40) || (Machine_TLI == TLI_A611DN45) || (Machine_TLI == TLI_A616DNS45)
			 || (Machine_TLI == TLI_A610DN40H) || (Machine_TLI == TLI_A611DN45H) || (Machine_TLI == TLI_A616DNS45H)
			 || (Machine_TLI == TLI_A611DN45HHU) || (Machine_TLI == TLI_A611DN45HH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T9K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T9K ###\n");
	#endif
			TonerCapacityChk = 90;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A615DN40) || (Machine_TLI == TLI_A616DN45)
			|| (Machine_TLI == TLI_A615DN40H) || (Machine_TLI == TLI_A616DN45H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T9K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T6K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T6K-C ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40_C) || (Machine_TLI == TLI_A611DN45_C) || (Machine_TLI == TLI_A610DN40_CH) || (Machine_TLI == TLI_A611DN45_CH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T6K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T3K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T3K-C ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40_C) || (Machine_TLI == TLI_A611DN45_C) || (Machine_TLI == TLI_A610DN40_CH) || (Machine_TLI == TLI_A611DN45_CH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T3K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T6K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T6K-C ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40_C) || (Machine_TLI == TLI_A611DN45_C) || (Machine_TLI == TLI_A610DN40_CH) || (Machine_TLI == TLI_A611DN45_CH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T6K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T13K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T13K-C ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610DN40_C) || (Machine_TLI == TLI_A611DN45_C) || (Machine_TLI == TLI_A610DN40_CH) || (Machine_TLI == TLI_A611DN45_CH)
			 || (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T13K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T3K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T3K-W ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610NA40) || (Machine_TLI == TLI_A611NA47) || (Machine_TLI == TLI_A610BR40_L) || (Machine_TLI == TLI_A611BR47_L) || (Machine_TLI == TLI_A610BR40_H) || (Machine_TLI == TLI_A611BR47_H)
			|| (Machine_TLI == TLI_A610TR40) || (Machine_TLI == TLI_A611TR47) || (Machine_TLI == TLI_A610ZA40) || (Machine_TLI == TLI_A611ZA47) || (Machine_TLI == TLI_A610TN40) || (Machine_TLI == TLI_A611TN47)
			|| (Machine_TLI == TLI_A610AE40) || (Machine_TLI == TLI_A611AE47) || (Machine_TLI == TLI_A610IR40) || (Machine_TLI == TLI_A611IR47) || (Machine_TLI == TLI_A610RU40) || (Machine_TLI == TLI_A611RU47)
			|| (Machine_TLI == TLI_A610IN40_HD) || (Machine_TLI == TLI_A611IN47_HD) || (Machine_TLI == TLI_A610IN40) || (Machine_TLI == TLI_A611IN47) || (Machine_TLI == TLI_A610MY40) || (Machine_TLI == TLI_A611MY47)
			|| (Machine_TLI == TLI_A610SA40) || (Machine_TLI == TLI_A611SA47) || (Machine_TLI == TLI_A610AU40) || (Machine_TLI == TLI_A611AU47) || (Machine_TLI == TLI_A610AR40) || (Machine_TLI == TLI_A611AR47)
			|| (Machine_TLI == TLI_A610ES40) || (Machine_TLI == TLI_A611ES47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47) || (Machine_TLI == TLI_A610MX40) || (Machine_TLI == TLI_A611MX47) || (Machine_TLI == TLI_A610PE40) || (Machine_TLI == TLI_A611PE47)
			|| (Machine_TLI == TLI_A610CL40) || (Machine_TLI == TLI_A611CL47)
			|| (Machine_TLI == TLI_A610LV40H) || (Machine_TLI == TLI_A610HV40H) || (Machine_TLI == TLI_A611LV47H) || (Machine_TLI == TLI_A611HV47H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T3K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T6K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T6K-W ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610NA40) || (Machine_TLI == TLI_A611NA47) || (Machine_TLI == TLI_A610BR40_L) || (Machine_TLI == TLI_A611BR47_L) || (Machine_TLI == TLI_A610BR40_H) || (Machine_TLI == TLI_A611BR47_H)
			|| (Machine_TLI == TLI_A610TR40) || (Machine_TLI == TLI_A611TR47) || (Machine_TLI == TLI_A610ZA40) || (Machine_TLI == TLI_A611ZA47) || (Machine_TLI == TLI_A610TN40) || (Machine_TLI == TLI_A611TN47)
			|| (Machine_TLI == TLI_A610AE40) || (Machine_TLI == TLI_A611AE47) || (Machine_TLI == TLI_A610IR40) || (Machine_TLI == TLI_A611IR47) || (Machine_TLI == TLI_A610RU40) || (Machine_TLI == TLI_A611RU47)
			|| (Machine_TLI == TLI_A610IN40_HD) || (Machine_TLI == TLI_A611IN47_HD) || (Machine_TLI == TLI_A610IN40) || (Machine_TLI == TLI_A611IN47) || (Machine_TLI == TLI_A610MY40) || (Machine_TLI == TLI_A611MY47)
			|| (Machine_TLI == TLI_A610SA40) || (Machine_TLI == TLI_A611SA47) || (Machine_TLI == TLI_A610AU40) || (Machine_TLI == TLI_A611AU47) || (Machine_TLI == TLI_A610AR40) || (Machine_TLI == TLI_A611AR47)
			|| (Machine_TLI == TLI_A610ES40) || (Machine_TLI == TLI_A611ES47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47) || (Machine_TLI == TLI_A610MX40) || (Machine_TLI == TLI_A611MX47) || (Machine_TLI == TLI_A610PE40) || (Machine_TLI == TLI_A611PE47)
			|| (Machine_TLI == TLI_A610CL40) || (Machine_TLI == TLI_A611CL47)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T6K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"LA40T9K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : LA40T9K-W ###\n");
	#endif
			TonerCapacityChk = 90;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A615NA40) || (Machine_TLI == TLI_A616NA47)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_LA40T9K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T3K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T3K-W ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610NA40) || (Machine_TLI == TLI_A611NA47) || (Machine_TLI == TLI_A610BR40_L) || (Machine_TLI == TLI_A611BR47_L) || (Machine_TLI == TLI_A610BR40_H) || (Machine_TLI == TLI_A611BR47_H)
			|| (Machine_TLI == TLI_A610TR40) || (Machine_TLI == TLI_A611TR47) || (Machine_TLI == TLI_A610ZA40) || (Machine_TLI == TLI_A611ZA47) || (Machine_TLI == TLI_A610TN40) || (Machine_TLI == TLI_A611TN47)
			|| (Machine_TLI == TLI_A610AE40) || (Machine_TLI == TLI_A611AE47) || (Machine_TLI == TLI_A610IR40) || (Machine_TLI == TLI_A611IR47) || (Machine_TLI == TLI_A610RU40) || (Machine_TLI == TLI_A611RU47)
			|| (Machine_TLI == TLI_A610IN40_HD) || (Machine_TLI == TLI_A611IN47_HD) || (Machine_TLI == TLI_A610IN40) || (Machine_TLI == TLI_A611IN47) || (Machine_TLI == TLI_A610MY40) || (Machine_TLI == TLI_A611MY47)
			|| (Machine_TLI == TLI_A610SA40) || (Machine_TLI == TLI_A611SA47) || (Machine_TLI == TLI_A610AU40) || (Machine_TLI == TLI_A611AU47) || (Machine_TLI == TLI_A610AR40) || (Machine_TLI == TLI_A611AR47)
			|| (Machine_TLI == TLI_A610ES40) || (Machine_TLI == TLI_A611ES47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47)
			|| (Machine_TLI == TLI_A615NA40) || (Machine_TLI == TLI_A616NA47) || (Machine_TLI == TLI_A610MX40) || (Machine_TLI == TLI_A611MX47) || (Machine_TLI == TLI_A610PE40) || (Machine_TLI == TLI_A611PE47)
			|| (Machine_TLI == TLI_A610CL40) || (Machine_TLI == TLI_A611CL47)
			|| (Machine_TLI == TLI_M611NA40) || (Machine_TLI == TLI_M612NA47) || (Machine_TLI == TLI_M611BR40_L) || (Machine_TLI == TLI_M612BR47_L) || (Machine_TLI == TLI_M611BR40_H) || (Machine_TLI == TLI_M612BR47_H)
			|| (Machine_TLI == TLI_M611TR40) || (Machine_TLI == TLI_M612TR47) || (Machine_TLI == TLI_M611ZA40) || (Machine_TLI == TLI_M612ZA47) || (Machine_TLI == TLI_M611TN40) || (Machine_TLI == TLI_M612TN47)
			|| (Machine_TLI == TLI_M611AE40) || (Machine_TLI == TLI_M612AE47) || (Machine_TLI == TLI_M611IR40) || (Machine_TLI == TLI_M612IR47) || (Machine_TLI == TLI_M611RU40) || (Machine_TLI == TLI_M612RU47)
			|| (Machine_TLI == TLI_M611IN40_HD) || (Machine_TLI == TLI_M612IN47_HD) || (Machine_TLI == TLI_M611IN40) || (Machine_TLI == TLI_M612IN47) || (Machine_TLI == TLI_M611MY40) || (Machine_TLI == TLI_M612MY47)
			|| (Machine_TLI == TLI_M611SA40) || (Machine_TLI == TLI_M612SA47) || (Machine_TLI == TLI_M611AU40) || (Machine_TLI == TLI_M612AU47) || (Machine_TLI == TLI_M611AR40) || (Machine_TLI == TLI_M612AR47)
			|| (Machine_TLI == TLI_M611CO40) || (Machine_TLI == TLI_M612CO47) || (Machine_TLI == TLI_M611ES40) || (Machine_TLI == TLI_M612ES47) || (Machine_TLI == TLI_M611PY40) || (Machine_TLI == TLI_M612PY47)
			|| (Machine_TLI == TLI_M611MX40) || (Machine_TLI == TLI_M612MX47) || (Machine_TLI == TLI_M611PE40) || (Machine_TLI == TLI_M612PE47) || (Machine_TLI == TLI_M611CL40) || (Machine_TLI == TLI_M612CL47)
			|| (Machine_TLI == TLI_M610NA40) || (Machine_TLI == TLI_M610BR40_L) || (Machine_TLI == TLI_M610IR40) || (Machine_TLI == TLI_M610RU40) || (Machine_TLI == TLI_M610IN40_HD) || (Machine_TLI == TLI_M610IN40)
			|| (Machine_TLI == TLI_M610MY40) || (Machine_TLI == TLI_M610SA40) || (Machine_TLI == TLI_M610AU40) || (Machine_TLI == TLI_M610AR40) || (Machine_TLI == TLI_M610CO40) || (Machine_TLI == TLI_M610ES40)
			|| (Machine_TLI == TLI_M610PY40)
			|| (Machine_TLI == TLI_A610LV40H) || (Machine_TLI == TLI_A610HV40H) || (Machine_TLI == TLI_A611LV47H) || (Machine_TLI == TLI_A611HV47H)
			|| (Machine_TLI == TLI_M611LV40H) || (Machine_TLI == TLI_M611HV40H) || (Machine_TLI == TLI_M612LV47H) || (Machine_TLI == TLI_M612HV47H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T3K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T6K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T6K-W ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610NA40) || (Machine_TLI == TLI_A611NA47) || (Machine_TLI == TLI_A610BR40_L) || (Machine_TLI == TLI_A611BR47_L) || (Machine_TLI == TLI_A610BR40_H) || (Machine_TLI == TLI_A611BR47_H)
			|| (Machine_TLI == TLI_A610TR40) || (Machine_TLI == TLI_A611TR47) || (Machine_TLI == TLI_A610ZA40) || (Machine_TLI == TLI_A611ZA47) || (Machine_TLI == TLI_A610TN40) || (Machine_TLI == TLI_A611TN47)
			|| (Machine_TLI == TLI_A610AE40) || (Machine_TLI == TLI_A611AE47) || (Machine_TLI == TLI_A610IR40) || (Machine_TLI == TLI_A611IR47) || (Machine_TLI == TLI_A610RU40) || (Machine_TLI == TLI_A611RU47)
			|| (Machine_TLI == TLI_A610IN40_HD) || (Machine_TLI == TLI_A611IN47_HD) || (Machine_TLI == TLI_A610IN40) || (Machine_TLI == TLI_A611IN47) || (Machine_TLI == TLI_A610MY40) || (Machine_TLI == TLI_A611MY47)
			|| (Machine_TLI == TLI_A610SA40) || (Machine_TLI == TLI_A611SA47) || (Machine_TLI == TLI_A610AU40) || (Machine_TLI == TLI_A611AU47) || (Machine_TLI == TLI_A610AR40) || (Machine_TLI == TLI_A611AR47)
			|| (Machine_TLI == TLI_A610ES40) || (Machine_TLI == TLI_A611ES47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47)
			|| (Machine_TLI == TLI_A615NA40) || (Machine_TLI == TLI_A616NA47) || (Machine_TLI == TLI_A610MX40) || (Machine_TLI == TLI_A611MX47) || (Machine_TLI == TLI_A610PE40) || (Machine_TLI == TLI_A611PE47)
			|| (Machine_TLI == TLI_A610CL40) || (Machine_TLI == TLI_A611CL47)
			|| (Machine_TLI == TLI_M611NA40) || (Machine_TLI == TLI_M612NA47) || (Machine_TLI == TLI_M611BR40_L) || (Machine_TLI == TLI_M612BR47_L) || (Machine_TLI == TLI_M611BR40_H) || (Machine_TLI == TLI_M612BR47_H)
			|| (Machine_TLI == TLI_M611TR40) || (Machine_TLI == TLI_M612TR47) || (Machine_TLI == TLI_M611ZA40) || (Machine_TLI == TLI_M612ZA47) || (Machine_TLI == TLI_M611TN40) || (Machine_TLI == TLI_M612TN47)
			|| (Machine_TLI == TLI_M611AE40) || (Machine_TLI == TLI_M612AE47) || (Machine_TLI == TLI_M611IR40) || (Machine_TLI == TLI_M612IR47) || (Machine_TLI == TLI_M611RU40) || (Machine_TLI == TLI_M612RU47)
			|| (Machine_TLI == TLI_M611IN40_HD) || (Machine_TLI == TLI_M612IN47_HD) || (Machine_TLI == TLI_M611IN40) || (Machine_TLI == TLI_M612IN47) || (Machine_TLI == TLI_M611MY40) || (Machine_TLI == TLI_M612MY47)
			|| (Machine_TLI == TLI_M611SA40) || (Machine_TLI == TLI_M612SA47) || (Machine_TLI == TLI_M611AU40) || (Machine_TLI == TLI_M612AU47) || (Machine_TLI == TLI_M611AR40) || (Machine_TLI == TLI_M612AR47)
			|| (Machine_TLI == TLI_M611CO40) || (Machine_TLI == TLI_M612CO47) || (Machine_TLI == TLI_M611ES40) || (Machine_TLI == TLI_M612ES47) || (Machine_TLI == TLI_M611PY40) || (Machine_TLI == TLI_M612PY47)
			|| (Machine_TLI == TLI_M611MX40) || (Machine_TLI == TLI_M612MX47) || (Machine_TLI == TLI_M611PE40) || (Machine_TLI == TLI_M612PE47) || (Machine_TLI == TLI_M611CL40) || (Machine_TLI == TLI_M612CL47)
			|| (Machine_TLI == TLI_M610NA40) || (Machine_TLI == TLI_M610BR40_L) || (Machine_TLI == TLI_M610IR40) || (Machine_TLI == TLI_M610RU40) || (Machine_TLI == TLI_M610IN40_HD) || (Machine_TLI == TLI_M610IN40)
			|| (Machine_TLI == TLI_M610MY40) || (Machine_TLI == TLI_M610SA40) || (Machine_TLI == TLI_M610AU40) || (Machine_TLI == TLI_M610AR40) || (Machine_TLI == TLI_M610CO40) || (Machine_TLI == TLI_M610ES40)
			|| (Machine_TLI == TLI_M610PY40)
			|| (Machine_TLI == TLI_A610LV40H) || (Machine_TLI == TLI_A610HV40H) || (Machine_TLI == TLI_A611LV47H) || (Machine_TLI == TLI_A611HV47H)
			|| (Machine_TLI == TLI_M611LV40H) || (Machine_TLI == TLI_M611HV40H) || (Machine_TLI == TLI_M612LV47H) || (Machine_TLI == TLI_M612HV47H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T6K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A610T13K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A610T13K-W ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A610NA40) || (Machine_TLI == TLI_A611NA47) || (Machine_TLI == TLI_A610BR40_L) || (Machine_TLI == TLI_A611BR47_L) || (Machine_TLI == TLI_A610BR40_H) || (Machine_TLI == TLI_A611BR47_H)
			|| (Machine_TLI == TLI_A610TR40) || (Machine_TLI == TLI_A611TR47) || (Machine_TLI == TLI_A610ZA40) || (Machine_TLI == TLI_A611ZA47) || (Machine_TLI == TLI_A610TN40) || (Machine_TLI == TLI_A611TN47)
			|| (Machine_TLI == TLI_A610AE40) || (Machine_TLI == TLI_A611AE47) || (Machine_TLI == TLI_A610IR40) || (Machine_TLI == TLI_A611IR47) || (Machine_TLI == TLI_A610RU40) || (Machine_TLI == TLI_A611RU47)
			|| (Machine_TLI == TLI_A610IN40_HD) || (Machine_TLI == TLI_A611IN47_HD) || (Machine_TLI == TLI_A610IN40) || (Machine_TLI == TLI_A611IN47) || (Machine_TLI == TLI_A610MY40) || (Machine_TLI == TLI_A611MY47)
			|| (Machine_TLI == TLI_A610SA40) || (Machine_TLI == TLI_A611SA47) || (Machine_TLI == TLI_A610AU40) || (Machine_TLI == TLI_A611AU47) || (Machine_TLI == TLI_A610AR40) || (Machine_TLI == TLI_A611AR47)
			|| (Machine_TLI == TLI_A610ES40) || (Machine_TLI == TLI_A611ES47) || (Machine_TLI == TLI_A610PY40) || (Machine_TLI == TLI_A611PY47)
			|| (Machine_TLI == TLI_A615NA40) || (Machine_TLI == TLI_A616NA47) || (Machine_TLI == TLI_A610MX40) || (Machine_TLI == TLI_A611MX47) || (Machine_TLI == TLI_A610PE40) || (Machine_TLI == TLI_A611PE47)
			|| (Machine_TLI == TLI_A610CL40) || (Machine_TLI == TLI_A611CL47)
			|| (Machine_TLI == TLI_M611NA40) || (Machine_TLI == TLI_M612NA47) || (Machine_TLI == TLI_M611BR40_L) || (Machine_TLI == TLI_M612BR47_L) || (Machine_TLI == TLI_M611BR40_H) || (Machine_TLI == TLI_M612BR47_H)
			|| (Machine_TLI == TLI_M611TR40) || (Machine_TLI == TLI_M612TR47) || (Machine_TLI == TLI_M611ZA40) || (Machine_TLI == TLI_M612ZA47) || (Machine_TLI == TLI_M611TN40) || (Machine_TLI == TLI_M612TN47)
			|| (Machine_TLI == TLI_M611AE40) || (Machine_TLI == TLI_M612AE47) || (Machine_TLI == TLI_M611IR40) || (Machine_TLI == TLI_M612IR47) || (Machine_TLI == TLI_M611RU40) || (Machine_TLI == TLI_M612RU47)
			|| (Machine_TLI == TLI_M611IN40_HD) || (Machine_TLI == TLI_M612IN47_HD) || (Machine_TLI == TLI_M611IN40) || (Machine_TLI == TLI_M612IN47) || (Machine_TLI == TLI_M611MY40) || (Machine_TLI == TLI_M612MY47)
			|| (Machine_TLI == TLI_M611SA40) || (Machine_TLI == TLI_M612SA47) || (Machine_TLI == TLI_M611AU40) || (Machine_TLI == TLI_M612AU47) || (Machine_TLI == TLI_M611AR40) || (Machine_TLI == TLI_M612AR47)
			|| (Machine_TLI == TLI_M611CO40) || (Machine_TLI == TLI_M612CO47) || (Machine_TLI == TLI_M611ES40) || (Machine_TLI == TLI_M612ES47) || (Machine_TLI == TLI_M611PY40) || (Machine_TLI == TLI_M612PY47)
			|| (Machine_TLI == TLI_M611MX40) || (Machine_TLI == TLI_M612MX47) || (Machine_TLI == TLI_M611PE40) || (Machine_TLI == TLI_M612PE47) || (Machine_TLI == TLI_M611CL40) || (Machine_TLI == TLI_M612CL47)
			|| (Machine_TLI == TLI_M610NA40) || (Machine_TLI == TLI_M610BR40_L) || (Machine_TLI == TLI_M610IR40) || (Machine_TLI == TLI_M610RU40) || (Machine_TLI == TLI_M610IN40_HD) || (Machine_TLI == TLI_M610IN40)
			|| (Machine_TLI == TLI_M610MY40) || (Machine_TLI == TLI_M610SA40) || (Machine_TLI == TLI_M610AU40) || (Machine_TLI == TLI_M610AR40) || (Machine_TLI == TLI_M610CO40) || (Machine_TLI == TLI_M610ES40)
			|| (Machine_TLI == TLI_M610PY40)
			|| (Machine_TLI == TLI_A610LV40H) || (Machine_TLI == TLI_A610HV40H) || (Machine_TLI == TLI_A611LV47H) || (Machine_TLI == TLI_A611HV47H)
			|| (Machine_TLI == TLI_M611LV40H) || (Machine_TLI == TLI_M611HV40H) || (Machine_TLI == TLI_M612LV47H) || (Machine_TLI == TLI_M612HV47H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A610T13K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		
		
		// Pinetree Official
		else if(strncmp((char*)TLI_GET,"M610T3K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T3K ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M61140) || (Machine_TLI == TLI_M61640) || (Machine_TLI == TLI_M61245) || (Machine_TLI == TLI_M61745)
			|| (Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540)
			|| (Machine_TLI == TLI_M61040H) || (Machine_TLI == TLI_M61540H) || (Machine_TLI == TLI_M61140H) || (Machine_TLI == TLI_M61640H) || (Machine_TLI == TLI_M61245H) || (Machine_TLI == TLI_M61745H)
			|| (Machine_TLI == TLI_M61245HH)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T3K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M610T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T6K ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M61140) || (Machine_TLI == TLI_M61640) || (Machine_TLI == TLI_M61245) || (Machine_TLI == TLI_M61745)
			|| (Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540)
			|| (Machine_TLI == TLI_M61040H) || (Machine_TLI == TLI_M61540H) || (Machine_TLI == TLI_M61140H) || (Machine_TLI == TLI_M61640H) || (Machine_TLI == TLI_M61245H) || (Machine_TLI == TLI_M61745H)
			|| (Machine_TLI == TLI_M61245HH)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M610T13K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T13K ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M61140) || (Machine_TLI == TLI_M61640) || (Machine_TLI == TLI_M61245) || (Machine_TLI == TLI_M61745)
			|| (Machine_TLI == TLI_M61040) || (Machine_TLI == TLI_M61540)
			|| (Machine_TLI == TLI_M61040H) || (Machine_TLI == TLI_M61540H) || (Machine_TLI == TLI_M61140H) || (Machine_TLI == TLI_M61640H) || (Machine_TLI == TLI_M61245H) || (Machine_TLI == TLI_M61745H)
			|| (Machine_TLI == TLI_M61245HH)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T13K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M013T3K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M013T3K ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M61140) || (Machine_TLI == TLI_M61245) 
			|| (Machine_TLI == TLI_M61040)
			|| (Machine_TLI == TLI_M61040H) || (Machine_TLI == TLI_M61140H) || (Machine_TLI == TLI_M61245H) 
			|| (Machine_TLI == TLI_M61245HH)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M013T3K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M013T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M013T6K ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M61640) || (Machine_TLI == TLI_M61745) 
			|| (Machine_TLI == TLI_M61540)
			|| (Machine_TLI == TLI_M61540H) || (Machine_TLI == TLI_M61640H) || (Machine_TLI == TLI_M61745H) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M013T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M013T3K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M013T3K-C ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M611DN40_C) || (Machine_TLI == TLI_M612DN45_C) || (Machine_TLI == TLI_M611DN40_CH) || (Machine_TLI == TLI_M612DN45_CH) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M013T3K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M610T3K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T3K-C ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M611DN40_C) || (Machine_TLI == TLI_M612DN45_C) || (Machine_TLI == TLI_M611DN40_CH) || (Machine_TLI == TLI_M612DN45_CH) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T3K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M610T6K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T6K-C ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M611DN40_C) || (Machine_TLI == TLI_M612DN45_C) || (Machine_TLI == TLI_M611DN40_CH) || (Machine_TLI == TLI_M612DN45_CH) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T6K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M610T13K-C",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M610T13K-C ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M611DN40_C) || (Machine_TLI == TLI_M612DN45_C) || (Machine_TLI == TLI_M611DN40_CH) || (Machine_TLI == TLI_M612DN45_CH) 
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M610T13K_C;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M013T3K-W",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M013T3K-W ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M611NA40) || (Machine_TLI == TLI_M612NA47) || (Machine_TLI == TLI_M611BR40_L) || (Machine_TLI == TLI_M612BR47_L) || (Machine_TLI == TLI_M611BR40_H) || (Machine_TLI == TLI_M612BR47_H)
			|| (Machine_TLI == TLI_M611TR40) || (Machine_TLI == TLI_M612TR47) || (Machine_TLI == TLI_M611ZA40) || (Machine_TLI == TLI_M612ZA47) || (Machine_TLI == TLI_M611TN40) || (Machine_TLI == TLI_M612TN47)
			|| (Machine_TLI == TLI_M611AE40) || (Machine_TLI == TLI_M612AE47) || (Machine_TLI == TLI_M611IR40) || (Machine_TLI == TLI_M612IR47) || (Machine_TLI == TLI_M611RU40) || (Machine_TLI == TLI_M612RU47)
			|| (Machine_TLI == TLI_M611IN40_HD) || (Machine_TLI == TLI_M612IN47_HD) || (Machine_TLI == TLI_M611IN40) || (Machine_TLI == TLI_M612IN47) || (Machine_TLI == TLI_M611MY40) || (Machine_TLI == TLI_M612MY47)
			|| (Machine_TLI == TLI_M611SA40) || (Machine_TLI == TLI_M612SA47) || (Machine_TLI == TLI_M611AU40) || (Machine_TLI == TLI_M612AU47) || (Machine_TLI == TLI_M611AR40) || (Machine_TLI == TLI_M612AR47)
			|| (Machine_TLI == TLI_M611CO40) || (Machine_TLI == TLI_M612CO47) || (Machine_TLI == TLI_M611ES40) || (Machine_TLI == TLI_M612ES47) || (Machine_TLI == TLI_M611PY40) || (Machine_TLI == TLI_M612PY47)
			|| (Machine_TLI == TLI_M611MX40) || (Machine_TLI == TLI_M612MX47) || (Machine_TLI == TLI_M611PE40) || (Machine_TLI == TLI_M612PE47) || (Machine_TLI == TLI_M611CL40) || (Machine_TLI == TLI_M612CL47)
			|| (Machine_TLI == TLI_M610NA40) || (Machine_TLI == TLI_M610BR40_L) || (Machine_TLI == TLI_M610IR40) || (Machine_TLI == TLI_M610RU40) || (Machine_TLI == TLI_M610IN40_HD) || (Machine_TLI == TLI_M610IN40)
			|| (Machine_TLI == TLI_M610MY40) || (Machine_TLI == TLI_M610SA40) || (Machine_TLI == TLI_M610AU40) || (Machine_TLI == TLI_M610AR40) || (Machine_TLI == TLI_M610CO40) || (Machine_TLI == TLI_M610ES40)
			|| (Machine_TLI == TLI_M610PY40)
			|| (Machine_TLI == TLI_M611LV40H) || (Machine_TLI == TLI_M611HV40H) || (Machine_TLI == TLI_M612LV47H) || (Machine_TLI == TLI_M612HV47H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M013T3K_W;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		
		// Abba2 Smartchip
		else if(strncmp((char*)TLI_GET,"ABBA2T9K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : ABBA2T9K ###\n");
	#endif
			TonerCapacityChk = 8;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_ABBA2T9K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"L236T3K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : L236T3K ###\n");
	#endif
			TonerCapacityChk = 1;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_L236T3K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"L236T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : L236T6K ###\n");
	#endif
			TonerCapacityChk = 2;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_L236T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A410T3K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A410T3K ###\n");
	#endif
			TonerCapacityChk = 6;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A410T3K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A410T6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A410T6K ###\n");
	#endif
			TonerCapacityChk = 7;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A410T6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A410T9K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A410T9K ###\n");
	#endif
			TonerCapacityChk = 8;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A413DN36)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A410T9K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A616DNST6K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A616DNST6K ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A616DNS45H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A616DNST6K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"A616DNST13K",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : A616DNST13K ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_A616DNS45H)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_A616DNST13K;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"P247T3K-WV",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : P247T3K-WV ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_P247B01)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_P247T3K_WV;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"M247T3K-WV",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : M247T3K-WV ###\n");
	#endif
			TonerCapacityChk = 30;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_M247B01)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_M247T3K_WV;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"THM247B01",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : THM247B01 ###\n");
	#endif
			TonerCapacityChk = 130;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_P247B01) || (Machine_TLI == TLI_M247B01)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_THM247B01;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}
		else if(strncmp((char*)TLI_GET,"THM247A01",12)==0)
		{
	#if	ENG_TRACE				
			MSG("(Eng)### TLI CHECK : THM247A01 ###\n");
	#endif
			TonerCapacityChk = 60;
	#if MACHINE_TLI_READ
			if((Machine_TLI == TLI_P247B01) || (Machine_TLI == TLI_M247B01)
			|| (Machine_TLI == TLI_DEFAULT))
	#else
			if(1)
	#endif
			{
				Toner_SmartIC_TLI = TN_THM247A01;
			}
			else
			{
	#if	ENG_TRACE				
				MSG("(Eng)### This machine can't support this TLI cartridge !!! : ERROR ###\n");
	#endif			
			}		
		}

		// RNDMASTER Smart IC
		else if(strncmp((char*)TLI_GET,"RNDMASTER",12)==0)
		{
#if	ENG_TRACE		
			MSG("(Eng)### TLI CHECK : RNDMASTER ###\n");
#endif
			TonerCapacityChk = 130;
			Toner_SmartIC_TLI = TN_RNDMASTER;
		}
		else
		{
#if	( ENG_TRACE | KSR_DBG_ERR )		
			MSG("(Eng)### TLI CHECK : Wrong Smart IC ###\n");
#endif		
			Toner_SmartIC_TLI = TN_WRONGTLI;
		}
		int i;
		for(i=0; i<NumOfKeyData; i++)
		{
			u8_Key[i] = Toner_KeyData[Toner_SmartIC_TLI][i];
		}
	}
	return Toner_SmartIC_TLI;
}

#endif /* [KSF061702] new TLI */

Uint8 CheckKaraMachineSmartchip(Uint8 Smarchip)
{
	Uint8 res;
	
	if((Smarchip == TN_LA40T6K) || (Smarchip == TN_LA40T9K) || (Smarchip == TN_LA40T6K_C) || (Smarchip == TN_LA40T3K_W) || (Smarchip == TN_A610T3K) || (Smarchip == TN_A610T6K) || (Smarchip == TN_A610T13K)
	|| (Smarchip == TN_A610T3K_C) || (Smarchip == TN_A610T6K_C) || (Smarchip == TN_A610T13K_C) || (Smarchip == TN_A610T3K_W) || (Smarchip == TN_A610T6K_W) || (Smarchip == TN_A610T13K_W)
	|| (Smarchip == TN_A616DNST6K) || (Smarchip == TN_A616DNST13K))
	{
		res = TRUE;
	}
	else
	{
		res = FALSE;
	}
	return res;
}
