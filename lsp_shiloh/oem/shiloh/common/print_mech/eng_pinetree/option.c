#include 	"stdio.h"
#include 	"unistd.h"
#include 	"logger.h"
#include 	"debug.h"
#include	"uart.h"
#include 	"option.h"
#include 	"uart.h"
#include 	"farch.h"
#include	"em.h"
#include "print_status_codes.h"
#include "printvars_api.h"
#include <termios.h>


#define DBG_PRFX "engoption: "
#define LOGGER_SUBMOD  LOGGER_SUBMODULE_BIT(20)
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_PRINT | LOGGER_SUBMOD

#define ERR  DBG_PRINTF_ERR
#define WARN DBG_PRINTF_WARNING
#define MSG  DBG_PRINTF_NOTICE
#define DBG1 DBG_PRINTF_DEBUG

#define SLOG_ERR  DBG_MEMLOG_ERR
#define SLOG_WARN DBG_MEMLOG_WARNING
#define SLOG_MSG  DBG_MEMLOG_INFO		/* ADD : @@2013.01.08. By CSPark */


#define	KMIF_MAX_LENGTH	2048
unsigned char 	tmp_rcv[KMIF_MAX_LENGTH];
unsigned char 	buf[KMIF_MAX_LENGTH];
unsigned char 	Tray2DetectionBuf[KMIF_MAX_LENGTH];
unsigned char	Tray2DetectionBufPosition;
int 	ReadCount;
char	SendBuffer[KMIF_MAX_LENGTH];
unsigned char	ReceiveBuffer[KMIF_MAX_LENGTH];
int TxCnt;
int RxCnt;

extern engine_state_t engine_state;

#define OPTION_CHATTERING_TIME 1

extern int cProcessCounter;
int cTimerStart_UART;
char fTray2Notify;
char fTray2MotorStop;
char fTray2MotorRun;
char fTray2PickUp;
char fTray3MotorStop;
char fTray3MotorRun;
char fTray3PickUp;
extern Uint8 f2ndDrawerState;
extern Uint8 f3ndDrawerState;
extern Uint8	f2ndDrawerState_1stdetect;
extern Uint8	f3rdDrawerState_1stdetect;
char fOptionTray1Set;
char fOptionTray2Set;
unsigned char fTray2FeedSensorDetectState;
unsigned char fTray3FeedSensorDetectState;
unsigned char OptionTray1Ver[6];
unsigned char OptionTray2Ver[6];
unsigned char OptionTray1State[2];
unsigned char OptionTray2State[2];
unsigned char OptionTray1Pro;
unsigned char OptionTray2Pro;
unsigned char OptionPrePickState;
unsigned char fRetryDetectInStandby;
char SendDataToOption1;
char SendDataToOption2;
char fOption1Disconnection;
char fOption2Disconnection;
unsigned int cTray2detectionSuccess;
unsigned int cTray2detection;
extern Uint8 cFatalOptionCommFail;

Uint8 fTray2IsNotifying;
unsigned char fOptionTray2Chattering;
unsigned int cOptionTray2ChatteringCnt;
unsigned char fOptionTray3Chattering;
unsigned int cOptionTray3ChatteringCnt;
//unsigned char fOptionPaperVolChattering;
//unsigned int cOptionPaperVolChatteringCnt;

Uint8 Tray2FeedSensorOn;
Uint8 Tray3FeedSensorOn;

extern Uint8 f1stTimeCheckTray1;
extern Uint8 f1stTimeCheckTray2;
extern Uint8 f1stTimeCheckTray3;

extern STRUCT_TRAY_STATUS Tray_Status;
extern Struct_Fuser_Control Fuser_Control;

extern char TrayCloseRecoveryEnable(char);
extern Uint8    Get_MainMotorState(void);
extern void Sensor_Power_On(void);
extern void	 	Motor_FWD(void);
extern uint32_t clear_engine_state(void);
extern Uint32	Set_MeasuredMediaSize(Uint8 MediaSource, Uint8 MediaSize);
extern Uint16	MeasuredPaperLength;
extern Uint16	MeasuredPaperLengthInTray1;
extern Uint16	MeasuredPaperLengthInTray2;
extern Uint16	MeasuredPaperLengthInTray3;
extern Uint8	fWaitPageCmd;
extern Uint8	fPOR_PageSupervisor_Act;

extern Uint32	tray1detect_count;
extern Uint32	tray2detect_count;
extern Uint32	tray3detect_count;
extern Uint8 	Tray1OnlineChattering;
extern Uint8 	Tray2OnlineChattering;
extern Uint8 	Tray3OnlineChattering;
extern Uint8 	fFirstTray1LevelDetect;
extern Uint8 	fFirstTray2LevelDetect;
extern Uint8 	fFirstTray3LevelDetect;

extern Uint8 fQuickly_sensor_check_T2;
extern Uint8 fQuickly_sensor_check_T3;
extern Uint8 	fPowerSave;
extern Uint8 fDebugOptionCommFail;
extern Uint8 cOptionCommFail;

extern Uint8	fPaperIsEmptyBeforeMotorRun;

extern Uint8 	Tray1Online;
extern Uint8 	Tray2Online;
extern Uint8	Tray3Online;

extern char fUartReadEnable;

extern ST_FLAG		CoverError;
extern ST_FLAG		CoverState;
extern ST_FLAG CommandReceiveFlag;

//int STOP=FALSE;

void rx_option_handler(int length, char *buf) {
	int i;
	buf[length] = 0;
//	MSG("rx_option_handler:%s:%d\n", buf, length);
	
	for ( i = 0 ; i < length ; i ++ ) 
	{
		tmp_rcv[i] = buf[i];
//		if(i == 0) 	MSG("(eng)#[%d]%02x ", i, tmp_rcv[i] );
//		else
//		{
//			printf( "#[%d]%02x ", i, tmp_rcv[i] );
//			if(i%10 == 0) printf("\n");
//		}
	}
//	printf("\n");
	ReadCount = length;
//	MSG("ReadCount=%d, length=%d\n", ReadCount, length);
	PrtSigHandler();
	return;
}

void UartInitialize(void)
{
//	INFO("\nUART DEVICE DRIVER Initialize\n");
//	uart_init(VTIME_OUT, VMIN_READ_CHAR);
	uart_init(VTIME_OUT_ForNoti, VMIN_READ_CHAR_ForNoti);
	OptionInitialize();
}
void OptionInitialize(void)
{
	int i;
	for ( i=0; i<KMIF_MAX_LENGTH; i++ )
	{
		SendBuffer[i] = 0;
		ReceiveBuffer[i] = 0;
	}
	RxCnt = 0;
  	TxCnt = 0;
  	
  	fOptionTray2Chattering = FALSE;
  	cOptionTray2ChatteringCnt = 0;
  	fOptionTray3Chattering = FALSE;
  	cOptionTray3ChatteringCnt = 0;
  	
  	fTray2IsNotifying = CLEAR;
  	
  	fTray2FeedSensorDetectState = CLEAR;
  	fTray3FeedSensorDetectState = CLEAR;
  	OptionPrePickState = CLEAR;
  	fRetryDetectInStandby = CLEAR;
  	Tray2DetectionBufPosition = CLEAR;
  	SendDataToOption1 = CLEAR;
  	SendDataToOption2 = CLEAR;
  	fOption1Disconnection = CLEAR;
  	fOption2Disconnection = CLEAR;
  	
  	cTray2detectionSuccess = CLEAR;
  	cTray2detection = CLEAR;
  	
	Tray2FeedSensorOn = CLEAR;
  	Tray3FeedSensorOn = CLEAR;
}

void PrtSigHandler(void)
{	
//	MSG("(eng)PtrSigHandler\n");	
	int checksum;
	int i;
/*	while(1)
	{
		ReceiveBuffer[i] = tmp_rcv[i];
		if(ReceiveBuffer[i] == RX_TRAY2_SYNC_1)
		{
			fOptionTray1Set = TRUE;
			MSG("(eng)Option Tray1 is detected\n");
			break;
		}
		if(i == ReadCount) break;
		i++;
	}
	*/
	if(f2ndDrawerState == CLEAR)
	{
		for(i = 0; i < ReadCount; i++)
		{
			Tray2DetectionBuf[Tray2DetectionBufPosition] = tmp_rcv[i];
			//MSG("uartrecv %d(%d):%x\n", Tray2DetectionBufPosition, i, Tray2DetectionBuf[Tray2DetectionBufPosition]);
			Tray2DetectionBufPosition++;
		}
	}
	else
	{
		if(ReadCount != 1)
		{
			checksum = CheckSum(ReadCount, tmp_rcv);
			if(checksum > 0)
			{
				parsing(tmp_rcv);
			}
			else
			{
				if(checksum < 0)
				{
					MSG("(eng)Checksum NG (%d)\n", checksum);
					for(i = 0; i < ReadCount; i++)
					{
						if(i == 0) MSG("(eng)Option tray Rx Data\n");
						MSG("%d(%02x) ", i, tmp_rcv[i]);
						if(i == ReadCount - 1) MSG("\n");
					}
				}
			}
		}
		ReadCount = CLEAR;
	}	
}

void SendPacketData(void)
{
	unsigned char i;
    
	send_to_eng( TxCnt, SendBuffer );

	for(i = 0; i < TxCnt; i++)
	{
		if(i == 0)
		{
//			MSG("(ENG)T>%02X ",SendBuffer[i]);
		}
		else
		{
//			printf("%02X ", SendBuffer[i]);
			if(i == TxCnt-1)
			{
//				printf("\n");
			}
		}
	}
}

void Option_Tx_Detect(void)
{
	MSG("(eng)Tray2_Detection_Check\n");
	Option_Tx_Sync(OPTION_T1);
	posix_sleep_ms(1);
	Option_Tx_Version(OPTION_T1);
	posix_sleep_ms(1);
	Option_Tx_Product(OPTION_T1);
	posix_sleep_ms(1);
	Option_Tx_Sync(OPTION_T2);
	posix_sleep_ms(1);
	Option_Tx_Version(OPTION_T2);
	posix_sleep_ms(1);
	Option_Tx_Product(OPTION_T2);

	// TODO : 아래는 완전 임시 대책임. Tray2와 sync시도하고 100msec 동안 되면 인식하고 무조건 Tray2 정보를 Paper not empty에 Letter로 날림.
	posix_sleep_ms(1000);
}
void Option_Tx_Sync(char tray_num)
{
	char tray2_sync;
	if(tray_num == OPTION_T1) tray2_sync = CMD_TRAY2_SYNC_1;
	else tray2_sync = CMD_TRAY2_SYNC_2;
	
//	MSG("(eng) Option_Tx_Sync %x (%x)\n", tray_num, tray2_sync);
	send_to_eng(1, &tray2_sync);
}

void Option_Tx_Version(char tray_num)
{
	SendBuffer[0] = tray_num;
	SendBuffer[1] = CMD_TRAY2_SW_VERSION_REQ;
	SendBuffer[2] = UA_DATA_LENGTH_0;
	SendBuffer[3] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2];
	TxCnt = 4;
//	MSG("(eng) Option_Tx_Version %x (%x)\n", tray_num, SendBuffer[1]);
	SendPacketData();
}

void Option_Tx_Product(char tray_num)
{
	SendBuffer[0] = tray_num;
	SendBuffer[1] = CMD_TRAY2_PRODUCT_ID_SET;
	SendBuffer[2] = UA_DATA_LENGTH_1;
	SendBuffer[3] = PRODUCT_NAME_KARA;
	SendBuffer[4] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2]+SendBuffer[3];
	TxCnt = 5;
//	MSG("(eng) Option_Tx_Product %x (%x)\n", tray_num, SendBuffer[1]);
	SendPacketData();
}

void Option_Tx_MotorRun(char tray_num, int motor_speed)
{
	SendBuffer[0] = tray_num;
	SendBuffer[1] = CMD_TRAY2_MTR_RUN;
	SendBuffer[2] = UA_DATA_LENGTH_2;
	SendBuffer[3] = motor_speed / 0x100;
	SendBuffer[4] = motor_speed % 0x100;
	SendBuffer[5] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2]+SendBuffer[3]+SendBuffer[4];
	MSG("Option_Tx_MotorRun(%x, %x, %x, %x, %x, %x)\n", SendBuffer[0], SendBuffer[1], SendBuffer[2], SendBuffer[3], SendBuffer[4], SendBuffer[5]);
	TxCnt = 6;
	SendPacketData();
}

void Option_Tx_Pickup(char tray_num, int motor_speed, unsigned char fpre_pick)
{
	SendBuffer[0] = tray_num;
	if(tray_num == OPTION_T2) 	SendBuffer[1] = fpre_pick? CMD_TRAY2_PICK_PREPICK:CMD_TRAY2_PICK;
	else 						SendBuffer[1] = CMD_TRAY2_PICK;
	SendBuffer[2] = UA_DATA_LENGTH_2;
	SendBuffer[3] = motor_speed / 0x100;
	SendBuffer[4] = motor_speed % 0x100;
	SendBuffer[5] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2]+SendBuffer[3]+SendBuffer[4];
	MSG("Option_Tx_Pickup(%x, %x, %x, %x, %x, %x)\n", SendBuffer[0], SendBuffer[1], SendBuffer[2], SendBuffer[3], SendBuffer[4], SendBuffer[5]);
	TxCnt = 6;
	SendPacketData();
	if(tray_num == ((char)OPTION_T2)) OptionPrePickState = fpre_pick;
}

void Option_Tx_Notify(char tray_num)
{
	SendBuffer[0] = tray_num;
	SendBuffer[1] = CMD_TRAY2_SENSOR_REQ;
	SendBuffer[2] = UA_DATA_LENGTH_0;
	SendBuffer[3] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2];
	TxCnt = 4;
//	MSG("(eng)Option_Tx_Notify (%x, %x, %x, %x)\n", SendBuffer[0], SendBuffer[1], SendBuffer[2], SendBuffer[3]);
	if(tray_num == OPTION_T1) 	SendDataToOption1++;
	else 						SendDataToOption2++;
	if((SendDataToOption1 >= 2) || (SendDataToOption2 >= 2)) { MSG("(eng)Send : SendDataToOption = (%d, %d)\n", SendDataToOption1, SendDataToOption2); }
	SendPacketData();
	if(tray_num == ((char)OPTION_T1)) 	fTray2IsNotifying = SET;
	else 								fTray2IsNotifying = CLEAR;
}

void Option_Tx_AccuSpeedSet(char tray_num, unsigned char motor_mode)
{
	SendBuffer[0] = tray_num;
	SendBuffer[1] = CMD_TRAY2_ACCUSPEEDSET;
	SendBuffer[2] = UA_DATA_LENGTH_2;
	if(motor_mode == MTR_MODE_NORMAL)
	{
		SendBuffer[3] = KARAS_ACCUFEED_SPEED / 0x100;
		SendBuffer[4] = KARAS_ACCUFEED_SPEED % 0x100;
	}
	else
	{
		SendBuffer[3] = KARAS_ACCUFEED_SPEED_FOR_LOW / 0x100;
		SendBuffer[4] = KARAS_ACCUFEED_SPEED_FOR_LOW % 0x100;
	}
	SendBuffer[5] = SendBuffer[0]+SendBuffer[1]+SendBuffer[2]+SendBuffer[3]+SendBuffer[4];
	TxCnt = 6;
	MSG("(eng)Option_Tx_AccuSpeedSet (%x, %x, %x, %x, %x, %x)\n", SendBuffer[0], SendBuffer[1], SendBuffer[2], SendBuffer[3], SendBuffer[4], SendBuffer[5]);
	SendPacketData();
}

unsigned char* Get_OptionTray1_Version(void)
{
	return &OptionTray1Ver[0];
}

unsigned char* Get_OptionTray2_Version(void)
{
	return &OptionTray2Ver[0];
}

void Tray2_Detection_Check(void)
{
	int checksum, i;
	fUartReadEnable = TRUE;
	Tray2DetectionBufPosition = CLEAR;
	memset(Tray2DetectionBuf, 0, KMIF_MAX_LENGTH);
	posix_sleep_ms(1);
	Option_Tx_Detect();
	cTray2detection++;
	if(f2ndDrawerState == CLEAR)
	{
		MSG("(eng)Tray2DetectionBufPosition = %d\n", Tray2DetectionBufPosition);
		if(Tray2DetectionBufPosition != 0)
		{
			checksum = CheckSum(Tray2DetectionBufPosition, Tray2DetectionBuf);
			if(checksum < 0){
				MSG("(eng)Checksum NG (%d)\n", checksum);
				for(i = 0; i < Tray2DetectionBufPosition; i++)
				{
					if(i == 0) MSG("(eng)Tray2Detection Rx Data\n");
					MSG("%d(%02x) ", i, Tray2DetectionBuf[i]);
					if(i == Tray2DetectionBufPosition - 1) MSG("\n");
				}
			}
			else MSG("(Eng)checksum OK (%d)\n", checksum);
			MSG("(eng)fTray2Notify = %d\n", fTray2Notify);
			parsing(Tray2DetectionBuf);
			//Option_Tx_Detect();
		}
	}
	MSG("(eng)Tray2_Detection_Test = %d / %d\n", cTray2detectionSuccess, cTray2detection);
}

void Pending_Tray2_Detection_Result(void)
{
	MESSAGE message = {0};
	printer_setup_t printer_setup;
	
	message.msgType = MSG_STATUSUPDATE;
	message.param2 = STATUS_TRAY;
    if(f2ndDrawerState == TRUE)
    {
		MSG("(eng)### TRAY2 DETECTED ###\n");
		if(fPowerSave == CLEAR)
		{
			message.param1 = STATUS_TRAY2_ATTACHED;
			SYMsgSend(PRINTMGRID, &message);
		}

//		message.param1 = STATUS_TRAY2_PAPER_NORMAL;
//		SYMsgSend(PRINTMGRID, &message);

//		message.param1 = STATUS_TRAY2_MEDIA_LETTER;
//		SYMsgSend(PRINTMGRID, &message);

	}
	else
	{
		MSG("(eng)### TRAY2 NOT DETECTED ###\n");
		if(fPowerSave == CLEAR)
		{
			message.param1 = STATUS_TRAY2_NOT_ATTACHED;
			SYMsgSend(PRINTMGRID, &message);
		}
	}
	
	if(f3ndDrawerState == TRUE)
    {
		MSG("(eng)### TRAY3 DETECTED ###\n");
		if(fPowerSave == CLEAR)
		{
			message.param1 = STATUS_TRAY3_ATTACHED;
			SYMsgSend(PRINTMGRID, &message);
		}
//		message.param1 = STATUS_TRAY3_PAPER_NORMAL;
//		SYMsgSend(PRINTMGRID, &message);

//		message.param1 = STATUS_TRAY3_MEDIA_LETTER;
//		SYMsgSend(PRINTMGRID, &message);

	}
	else
	{
		MSG("(eng)### TRAY3 NOT DETECTED ###\n");
		if(fPowerSave == CLEAR)
		{
			message.param1 = STATUS_TRAY3_NOT_ATTACHED;
			SYMsgSend(PRINTMGRID, &message);
		}
	}
	
	printvar_get_printersetup( &printer_setup );
	printer_setup.inputTrays[2].inputPresent = (f2ndDrawerState == SET)? true:false;
	printer_setup.inputTrays[3].inputPresent = (f3ndDrawerState == SET)? true:false;
	printvar_set_printersetup( &printer_setup );
	
	message.msgType = MSG_TRAY_VERSION;
	message.param1 = 0;
	message.param2 = 0;
	if(f2ndDrawerState == SET)
	{
		message.param1 = (Uint32)(OptionTray1Ver[0]<<16) + (Uint32)(OptionTray1Ver[1]<<8) + (Uint32)(OptionTray1Ver[2]);
	}
	if(f3ndDrawerState == SET)
	{
		message.param2 = (Uint32)(OptionTray2Ver[0]<<16) + (Uint32)(OptionTray2Ver[1]<<8) + (Uint32)(OptionTray2Ver[2]);
	}
	SYMsgSend(PRINTMGRID, &message);
	
	if(f2ndDrawerState == SET)
	{
//		uart_exit();
//		posix_sleep_ms(1);
//		MSG("(eng)Uart re-init\n");
//		uart_init(VTIME_OUT_ForNoti, VMIN_READ_CHAR_ForNoti);
//		uart_recv_handler_register(UCH_OPTION, (UART_RECV_HANDLER *)&rx_option_handler);
	}
}
void Set_Timer_UART(void)
{
	cTimerStart_UART = cProcessCounter;
}

int Get_Elapse_UART(void)
{
	return cProcessCounter - cTimerStart_UART;
}

int CheckSum(int readcount, unsigned char* rec_data)
{
	unsigned char i=0;
	unsigned char k=0;
	unsigned char PosCRC=0;
	unsigned int check_sum = 0;
	unsigned char ACK = TRUE;
	int res=0;
	
	for(i = 0; i < readcount; i++)
	{		
//		if(i != readcount-1)	MSG("%d:%x(%d) ", i, rec_data[i], ACK?1:0);
//		else 					MSG("%d:%x(%d)\n", i, rec_data[i], ACK?1:0);
		if((ACK == TRUE) && (SendDataToOption1 != CLEAR) && (fTray2IsNotifying == SET))
		{
			if(rec_data[i] == RX_TRAY2_SYNC_1) {
				SendDataToOption1 = CLEAR; 
				if(fOption1Disconnection == TRUE) {
					fOption1Disconnection = FALSE;
					cFatalOptionCommFail = CLEAR;
				}
			}
			else {
				MSG("(eng)Recv(0x%x) : SendDataToOption1 = %d\n", rec_data[i], SendDataToOption1);
			}
		}
		if((ACK == TRUE) && (SendDataToOption2 != CLEAR) && (fTray2IsNotifying == CLEAR))
		{
			if(rec_data[i] == RX_TRAY2_SYNC_2) {
				SendDataToOption2 = CLEAR; 
				if(fOption2Disconnection == TRUE){
					fOption2Disconnection = FALSE;
					cFatalOptionCommFail = CLEAR;
				}
			}
			else {
				MSG("(eng)Recv(0x%x) : SendDataToOption2 = %d\n", rec_data[i], SendDataToOption2);
			}
		}
		
		if((ACK == FALSE) || ((rec_data[i] != RX_TRAY2_SYNC_1) && (rec_data[i] != RX_TRAY2_SYNC_2)))
		{
			k++;
			check_sum += rec_data[i];
			//MSG("(eng)#### %d, rec_data[%x], check_sum = %x\n", k, rec_data[i], check_sum);
			if(k == 3)
			{
				ACK = FALSE;
				PosCRC = k+rec_data[i]+1;
				//MSG("(eng)PosCRC = %d\n", PosCRC);
			}
			else if(k == PosCRC)
			{
				ACK = TRUE;
				//MSG("(eng) CRC(%x)-----------------CheckSum(%x)\n", rec_data[i], (unsigned char)(check_sum - rec_data[i]));
				if(rec_data[i] != (unsigned char)(check_sum - rec_data[i]))
				{
					return -1;
				}
				else
				{
					res++;
				}
				k = 0;
				check_sum = 0;
				PosCRC = 0;
			}
		}
	}
	return res;
}

void OptionTray2_State(unsigned char Tray_vol, unsigned char Tray_size, unsigned char Tray_capa)
{
	MESSAGE message;
	printer_setup_t printer_setup;
	Uint8 papervol;

	if(Tray_capa == OPTION_TRAY_500)
	{
		if((Tray_size&0x0C) == 0x00)
		{
			Tray2FeedSensorOn = ((Tray_vol & 0x40) == 0x40)?true:false;
			
			if(fQuickly_sensor_check_T2 == SET)
			{
				MSG("(Eng)Quickly_sensor_check_T2\n");
				Tray2Online = TRUE;
				fQuickly_sensor_check_T2 = CLEAR;
				fFirstTray2LevelDetect = TRUE;
			}
			else
			{
				if(Tray2OnlineChattering == TRUE)
				{
					if(Tray_size == 0x70)
					{
						tray2detect_count++;
						Tray2Online = FALSE;
						if(tray2detect_count == TRAY2CHATTERING)
						{
							Tray2OnlineChattering = FALSE;
							tray2detect_count = CLEAR;
							Tray2Online = TRUE;
							fFirstTray2LevelDetect = TRUE;
						}
					}
					else
					{
						tray2detect_count = CLEAR;
						Tray2Online = TRUE;
					}
				}
				else
				{
					if(Tray_size != 0x70)
					{
						tray2detect_count++;
						Tray2Online = FALSE;
						if(tray2detect_count == TRAY2CHATTERING)
						{
							Tray2OnlineChattering = TRUE;
							tray2detect_count = CLEAR;
							Tray2Online = TRUE;
							fFirstTray2LevelDetect = TRUE;
						}
					}
					else
					{
						tray2detect_count = CLEAR;
						Tray2Online = TRUE;
					}
				}
			}
			
			if(Tray2Online == TRUE)
			{
				Tray_Status.Tray2.oldTrayStatus = Tray_Status.Tray2.newTrayStatus;
			
				switch (Tray_size & 0x70)
				{
					case 0x00: // Unknown
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_UNKNOWN;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Unknown);
						break;
					case 0x10: // B5
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_B5);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_B5);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_B5;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_B5);
						break;
					case 0x20: // Exec
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_EXEC);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_EXEC);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_EXEC;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Exec);
						break;
					case 0x30: // A5
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_A5);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_A5);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_A5;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_A5);
						break;
					case 0x40: // Letter
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_LT);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_LT);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_LETTER;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Letter);
						break;
					case 0x50: // Legal
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_LG);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_LG);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_LEGAL;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Legal);
						break;
					case 0x60: // A4
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_A4);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_A4);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_A4;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_A4);
						break;
					case 0x70: // Tray Open
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOff(Tray_Status.Tray2.newTrayStatus);
						SetTrayMediaEmpty(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_OPEN;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Unknown);
						break;
					default:
						MSG("(eng)Unkown Tray2 Paper size : %x\n", Tray_size & 0x70);
						SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray2.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
						message.param1 = STATUS_TRAY2_MEDIA_UNKNOWN;
						MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Unknown);
						break;
				}
				
				if(Tray_Status.Tray2.oldTrayStatus != Tray_Status.Tray2.newTrayStatus)
				{
					message.msgType = MSG_STATUSUPDATE;
					message.param2 = STATUS_TRAY;
					SYMsgSend(PRINTMGRID, &message);
					MSG("(eng)Send Tray2 STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray2.oldTrayStatus,Tray_Status.Tray2.newTrayStatus);
					message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
				}
				
				if(GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus) == TRUE)
				{
					switch (Tray_vol & 0x30)
					{
						case TRAY2_EMPTY:
							SetTrayMediaEmpty(Tray_Status.Tray2.newTrayStatus);
							break;
						case TRAY2_LOW:
							SetTrayMediaLow(Tray_Status.Tray2.newTrayStatus);
							break;
						case TRAY2_FULL:
							SetTrayMediaNormal(Tray_Status.Tray2.newTrayStatus);
							break;
					}
					
					if(fFirstTray2LevelDetect == FALSE)
					{
						if(GetTrayMediaLevel(Tray_Status.Tray2.oldTrayStatus) < GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus))
						{
							SetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus, Tray_Status.Tray2.oldTrayStatus);
						}
					}
					fFirstTray2LevelDetect = FALSE;
				}
				
				if(GetTrayMediaLevel(Tray_Status.Tray2.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus))
				{
					printvar_get_printersetup( &printer_setup );
					switch(GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus))
					{
						case TRAY_MEDIA_EMPTY:
							message.param1 = STATUS_PAPER_OUT_TRAY2;
							if ( printer_setup.inputTrays[2].paperPresent ) {
							    printer_setup.inputTrays[2].paperPresent = false;
							    printvar_set_printersetup( &printer_setup );
							}
							break;
						case TRAY_MEDIA_LOW:
							message.param1 = STATUS_TRAY2_PAPER_LOW;
							if ( !printer_setup.inputTrays[2].paperPresent ) {
							    printer_setup.inputTrays[2].paperPresent = true;
							    printvar_set_printersetup( &printer_setup );
							}
							break;
						case TRAY_MEDIA_NORMAL:
							message.param1 = STATUS_TRAY2_PAPER_NORMAL;
							if ( !printer_setup.inputTrays[2].paperPresent ) {
							    printer_setup.inputTrays[2].paperPresent = true;
							    printvar_set_printersetup( &printer_setup );
							}
							break;	
						default:
							break;
					}
					message.msgType = MSG_STATUSUPDATE;
					message.param2 = STATUS_TRAY;
					SYMsgSend(PRINTMGRID, &message);
					
					MSG("(eng)Send Option Tray2 Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray2.oldTrayStatus,Tray_Status.Tray2.newTrayStatus);
					message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
				}
	
				if((GetTrayAttachInfo(Tray_Status.Tray2.oldTrayStatus) == FALSE) && (GetTrayAttachInfo(Tray_Status.Tray2.newTrayStatus) == TRUE)
				&& (engine_state.front_door_open == FALSE)
				&& (engine_state.pm_statusrequest != STATUS_ONLINE)
				&& (fPOR_PageSupervisor_Act == CLEAR)
				&& (Get_MainMotorState() == STOP)
				&& (fPaperIsEmptyBeforeMotorRun == CLEAR)
				&& (Fuser_Control.EM_State == EMS_RecoverError)
				&& (fClearErrorsCmdRcv == CLEAR)
				&& TrayCloseRecoveryEnable(2))
				{
					Sensor_Power_On();
					Motor_FWD();
					fClearErrorsCmdRcv = CLEAR;
					fWaitPageCmd = TRUE;
					clear_engine_state();
					MSG("(eng) ## Send OptionTray2 online STATUS_UPDATE <%x> ##\n", message.param1);
					fClearErrorsCmdRcv = SET;
					Fuser_Control.EM_State = EMS_RecoverError;
				}
				
				fTray2FeedSensorDetectState = FeedSensorDetect(Tray_vol);
			}
		}
	}
	else
	{
		if(f1stTimeCheckTray2 == SET)
		{
			f1stTimeCheckTray2 = CLEAR;
			SetTrayMediaSize(Tray_Status.Tray2.newTrayStatus, TRAY_MEDIA_UNKNOWN);
			SetTrayAttachOn(Tray_Status.Tray2.newTrayStatus);
			message.param1 = STATUS_TRAY2_MEDIA_UNKNOWN;
			MeasuredPaperLengthInTray2 = Set_MeasuredMediaSize(MS_Tray2, MS_Unknown);
					
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray2 STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray2.oldTrayStatus,Tray_Status.Tray2.newTrayStatus);
			message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
		}
		
		Tray_Status.Tray2.oldTrayStatus = Tray_Status.Tray2.newTrayStatus;
		
		Tray2FeedSensorOn = ((Tray_vol & 0x40) == 0x40)?true:false;
		
		papervol = ((Tray_vol & 0x30) == TRAY2_FULL_250)?PRESENT:NOT_PRESENT;
		
		if(papervol == NOT_PRESENT)
		{
			SetTrayMediaEmpty(Tray_Status.Tray2.newTrayStatus);
		}
		else
		{
			SetTrayMediaNormal(Tray_Status.Tray2.newTrayStatus);
		}
		
		if(GetTrayMediaLevel(Tray_Status.Tray2.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus))
		{
			printer_setup_t printer_setup;
			printvar_get_printersetup( &printer_setup );
			switch(GetTrayMediaLevel(Tray_Status.Tray2.newTrayStatus))
			{
				case TRAY_MEDIA_EMPTY:
					message.param1 = STATUS_PAPER_OUT_TRAY2;
					if ( printer_setup.inputTrays[2].paperPresent ) {
					    printer_setup.inputTrays[2].paperPresent = false;
					    printvar_set_printersetup( &printer_setup );
					}
					break;
				case TRAY_MEDIA_NORMAL:
					message.param1 = STATUS_TRAY2_PAPER_NORMAL;
					if ( !printer_setup.inputTrays[2].paperPresent ) {
					    printer_setup.inputTrays[2].paperPresent = true;
					    printvar_set_printersetup( &printer_setup );
					}
					break;	
				default:
					MSG("(eng)Tray2:Unknown Media Level\n");
					break;
			}
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray2 Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray2.oldTrayStatus,Tray_Status.Tray2.newTrayStatus);
			message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
		}
	}
}

void OptionTray3_State(unsigned char Tray_vol, unsigned char Tray_size, unsigned char Tray_capa)
{
	MESSAGE message;
	printer_setup_t printer_setup;
	Uint8 papervol;

	if(Tray_capa == OPTION_TRAY_500)
	{
		if((Tray_size&0x0C) == 0x00)
		{
			Tray3FeedSensorOn = ((Tray_vol & 0x40) == 0x40)?true:false;
			
			if(fQuickly_sensor_check_T3 == SET)
			{
				MSG("(eng)Quickly_sensor_check_T3\n");
				Tray3Online = TRUE;
				fQuickly_sensor_check_T3 = CLEAR;
				fFirstTray3LevelDetect = TRUE;
			}
			else
			{
				if(Tray3OnlineChattering == TRUE)
				{
					if(Tray_size == 0x70)
					{
						tray3detect_count++;
						Tray3Online = FALSE;
						if(tray3detect_count == TRAY3CHATTERING)
						{
							Tray3OnlineChattering = FALSE;
							tray3detect_count = CLEAR;
							Tray3Online = TRUE;
							fFirstTray3LevelDetect = TRUE;
						}
					}
					else
					{
						tray3detect_count = CLEAR;
						Tray3Online = TRUE;
					}
				}
				else
				{
					if(Tray_size != 0x70)
					{
						tray3detect_count++;
						Tray3Online = FALSE;
						if(tray3detect_count == TRAY3CHATTERING)
						{
							Tray3OnlineChattering = TRUE;
							tray3detect_count = CLEAR;
							Tray3Online = TRUE;
							fFirstTray3LevelDetect = TRUE;
						}
					}
					else
					{
						tray3detect_count = CLEAR;
						Tray3Online = TRUE;
					}
				}
			}
			
			if(Tray3Online == SET)
			{
				Tray_Status.Tray3.oldTrayStatus = Tray_Status.Tray3.newTrayStatus;
				
				switch (Tray_size & 0x70)
				{
					case 0x00: // Unknown
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_UNKNOWN;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Unknown);
						break;
					case 0x10: // B5
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_B5);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_B5);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_B5;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_B5);
						break;
					case 0x20: // Exec
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_EXEC);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_EXEC);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_EXEC;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Exec);
						break;
					case 0x30: // A5
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_A5);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_A5);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_A5;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_A5);
						break;
					case 0x40: // Letter
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_LT);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_LT);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_LETTER;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Letter);
						break;
					case 0x50: // Legal
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_LG);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_LG);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_LEGAL;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Legal);
						break;
					case 0x60: // A4
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_A4);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_A4);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_A4;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_A4);
						break;
					case 0x70: // Tray Open
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOff(Tray_Status.Tray3.newTrayStatus);
						SetTrayMediaEmpty(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_OPEN;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Unknown);
						break;
					default:
						MSG("(eng)Unkown Tray3 Paper size : %x\n", Tray_size & 0x70);
						SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayMediaSize(Tray_Status.Tray3.emptyTrayStatus, TRAY_MEDIA_UNKNOWN);
						SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
						message.param1 = STATUS_TRAY3_MEDIA_UNKNOWN;
						MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Unknown);
						break;
				}
				
				if(Tray_Status.Tray3.oldTrayStatus != Tray_Status.Tray3.newTrayStatus)
				{
					message.msgType = MSG_STATUSUPDATE;
					message.param2 = STATUS_TRAY;
					SYMsgSend(PRINTMGRID, &message);
					MSG("(eng)Send Tray3 STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray3.oldTrayStatus,Tray_Status.Tray3.newTrayStatus);
					message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
				}
				
				if(GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus) == TRUE)
				{
					switch (Tray_vol & 0x30)
					{
						case TRAY2_EMPTY:
							SetTrayMediaEmpty(Tray_Status.Tray3.newTrayStatus);
							break;
						case TRAY2_LOW:
							SetTrayMediaLow(Tray_Status.Tray3.newTrayStatus);
							break;
						case TRAY2_FULL:
							SetTrayMediaNormal(Tray_Status.Tray3.newTrayStatus);
							break;
					}
					
					if(fFirstTray3LevelDetect == FALSE)
					{
						if(GetTrayMediaLevel(Tray_Status.Tray3.oldTrayStatus) < GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus))
						{
							SetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus, Tray_Status.Tray3.oldTrayStatus);
						}
					}
					fFirstTray3LevelDetect = FALSE;
				}
				
				if(GetTrayMediaLevel(Tray_Status.Tray3.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus))
				{
					printvar_get_printersetup( &printer_setup );
					switch(GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus))
					{
						case TRAY_MEDIA_EMPTY:
							message.param1 = STATUS_PAPER_OUT_TRAY3;
							if ( printer_setup.inputTrays[3].paperPresent ) {
							    printer_setup.inputTrays[3].paperPresent = false;
							    printvar_set_printersetup( &printer_setup );
							}
							break;
						case TRAY_MEDIA_LOW:
							message.param1 = STATUS_TRAY3_PAPER_LOW;
							if ( !printer_setup.inputTrays[3].paperPresent ) {
							    printer_setup.inputTrays[3].paperPresent = true;
							    printvar_set_printersetup( &printer_setup );
							}
							break;
						case TRAY_MEDIA_NORMAL:
							message.param1 = STATUS_TRAY3_PAPER_NORMAL;
							if ( !printer_setup.inputTrays[3].paperPresent ) {
							    printer_setup.inputTrays[3].paperPresent = true;
							    printvar_set_printersetup( &printer_setup );
							}
							break;	
						default:
							break;
					}
					message.msgType = MSG_STATUSUPDATE;
					message.param2 = STATUS_TRAY;
					SYMsgSend(PRINTMGRID, &message);
					
					MSG("(eng)Send Option Tray3 Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray3.oldTrayStatus,Tray_Status.Tray3.newTrayStatus);
					message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
				}
	
				if((GetTrayAttachInfo(Tray_Status.Tray3.oldTrayStatus) == FALSE) && (GetTrayAttachInfo(Tray_Status.Tray3.newTrayStatus) == TRUE)
				&& (engine_state.front_door_open == FALSE)
				&& (engine_state.pm_statusrequest != STATUS_ONLINE)
				&& (fPOR_PageSupervisor_Act == CLEAR)
				&& (Get_MainMotorState() == STOP)
				&& (fPaperIsEmptyBeforeMotorRun == CLEAR)
				&& (Fuser_Control.EM_State == EMS_RecoverError)
				&& (fClearErrorsCmdRcv == CLEAR)
				&& TrayCloseRecoveryEnable(3))
				{
					Sensor_Power_On();
					Motor_FWD();
					fClearErrorsCmdRcv = CLEAR;
					fWaitPageCmd = TRUE;
					clear_engine_state();
					MSG("(eng) ## Send OptionTray3 online STATUS_UPDATE <%x> ##\n", message.param1);
					fClearErrorsCmdRcv = SET;
					Fuser_Control.EM_State = EMS_RecoverError;
				}
				
				fTray3FeedSensorDetectState = FeedSensorDetect(Tray_vol);
			}
		}
	}
	else
	{
		if(f1stTimeCheckTray3 == SET)
		{
			f1stTimeCheckTray3 = CLEAR;
			SetTrayMediaSize(Tray_Status.Tray3.newTrayStatus, TRAY_MEDIA_UNKNOWN);
			SetTrayAttachOn(Tray_Status.Tray3.newTrayStatus);
			message.param1 = STATUS_TRAY3_MEDIA_UNKNOWN;
			MeasuredPaperLengthInTray3 = Set_MeasuredMediaSize(MS_Tray3, MS_Unknown);
					
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray3 STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray3.oldTrayStatus,Tray_Status.Tray3.newTrayStatus);
			message.param2 = 0;	//Reset param2 value. Because more messages are using same message structure.
		}
		
		Tray_Status.Tray3.oldTrayStatus = Tray_Status.Tray3.newTrayStatus;
		
		Tray3FeedSensorOn = ((Tray_vol & 0x40) == 0x40)?true:false;
		
		papervol = ((Tray_vol & 0x30) == TRAY2_FULL_250)?PRESENT:NOT_PRESENT;
		
		if(papervol == NOT_PRESENT)
		{
			SetTrayMediaEmpty(Tray_Status.Tray3.newTrayStatus);
		}
		else
		{
			SetTrayMediaNormal(Tray_Status.Tray3.newTrayStatus);
		}
		
		if(GetTrayMediaLevel(Tray_Status.Tray3.oldTrayStatus) != GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus))
		{
			printer_setup_t printer_setup;
			printvar_get_printersetup( &printer_setup );
			switch(GetTrayMediaLevel(Tray_Status.Tray3.newTrayStatus))
			{
				case TRAY_MEDIA_EMPTY:
					message.param1 = STATUS_PAPER_OUT_TRAY3;
					if ( printer_setup.inputTrays[3].paperPresent ) {
					    printer_setup.inputTrays[3].paperPresent = false;
					    printvar_set_printersetup( &printer_setup );
					}
					break;
				case TRAY_MEDIA_NORMAL:
					message.param1 = STATUS_TRAY3_PAPER_NORMAL;
					if ( !printer_setup.inputTrays[3].paperPresent ) {
					    printer_setup.inputTrays[3].paperPresent = true;
					    printvar_set_printersetup( &printer_setup );
					}
					break;	
				default:
					MSG("(eng)Tray3:Unknown Media Level\n");
					break;
			}
			message.msgType = MSG_STATUSUPDATE;
			message.param2 = STATUS_TRAY;
			SYMsgSend(PRINTMGRID, &message);
			MSG("(eng)Send Tray3 Media Level STATUS_UPDATE <%x> old=%x, new=%x\n", message.param1,Tray_Status.Tray3.oldTrayStatus,Tray_Status.Tray3.newTrayStatus);
			message.param2 = 0; //Reset param2 value. Because more messages are using same message structure.
		}
	}
}

void parsing(unsigned char* parsing_buf)
{
	int j;
	int k = 0;
	
	if((fTray2Notify == FALSE)&&(parsing_buf[0] == RX_TRAY2_SYNC_1))
	{
		f2ndDrawerState = TRUE;
		MSG("##########################\n");
		MSG("(eng)Option Tray1 is detected\n");
		cTray2detectionSuccess++;
	}
	if((fTray2Notify == FALSE)&&(f2ndDrawerState == TRUE))
	{
		for(j=0;j<6;j++)
		{
			OptionTray1Ver[j] = parsing_buf[j+5];
		}
						
		OptionTray1Pro = parsing_buf[16];
		MSG("(eng)Option Tray1 AppVer. %d.%d.%d , BootVer. %d.%d.%d\n", OptionTray1Ver[0], OptionTray1Ver[1], OptionTray1Ver[2], OptionTray1Ver[3], OptionTray1Ver[4], OptionTray1Ver[5]);
		if(OptionTray1Pro == OPTION_TRAY_500) 		MSG("(eng) Option Tray1 Product Name : KARA 500\n");
		else if(OptionTray1Pro == OPTION_TRAY_250) 	MSG("(eng) Option Tray1 Product Name : KARA 250\n");
		else 										MSG("(eng) Option Tray1 Product Name : Unknown\n");
		
		if(parsing_buf[18] == RX_TRAY2_SYNC_2)
		{
			f3ndDrawerState = TRUE;
			MSG("##########################\n");
			MSG("(eng)Option Tray2 is detected\n");
		}
		if(f3ndDrawerState == TRUE)
		{
			for(j=0;j<6;j++)
			{
				OptionTray2Ver[j] = parsing_buf[j+23];
			}
			OptionTray2Pro = parsing_buf[34];
			MSG("(eng)Option Tray2 AppVer. %d.%d.%d , BootVer. %d.%d.%d\n", OptionTray2Ver[0], OptionTray2Ver[1], OptionTray2Ver[2], OptionTray2Ver[3], OptionTray2Ver[4], OptionTray2Ver[5]);
			if(OptionTray2Pro == OPTION_TRAY_500)	 	MSG("(eng)Option Tray2 Product Name : KARA 500\n");
			else if(OptionTray2Pro == OPTION_TRAY_250) 	MSG("(eng)Option Tray2 Product Name : KARA 250\n");
			else 										MSG("(eng)Option Tray2 Product Name : Unknown\n");
		}
	}
	else if(fTray2Notify == TRUE)
	{
		for(j=0; j<ReadCount; j++)
		{
			ReceiveBuffer[k] = parsing_buf[j];
			if((parsing_buf[j] == RX_TRAY2_SYNC_1)||(parsing_buf[j] == RX_TRAY2_SYNC_2)){}
			else k++;
		}
		//MSG("(Option Tray State : %x %x %x %x %x %x\n", ReceiveBuffer[0], ReceiveBuffer[1], ReceiveBuffer[2], ReceiveBuffer[3], ReceiveBuffer[4], ReceiveBuffer[5]);
		if(ReceiveBuffer[1] == CMD_TRAY2_CMD_FAULT)
		{
			if(fDebugOptionCommFail == TRUE)
			{
				MSG("(Eng)Option Comm. Fault\n");
				cOptionCommFail++;
				fDebugOptionCommFail = FALSE;
			}
		}	
		if(ReceiveBuffer[1] == CMD_TRAY2_SENSOR_REQ)
		{
			for(j = 0; j < 2; j++)
			{
				if(ReceiveBuffer[0] == OPTION_T1)
				{
					OptionTray1State[j] = ReceiveBuffer[j+3];
				}
				else
				{
					OptionTray2State[j] = ReceiveBuffer[j+3];
				}
			}
		
			//MSG("(eng)Option Tray1 State (0x%x, 0x%x)\n", OptionTray1State[0] , OptionTray1State[1]);
			if(ReceiveBuffer[0] == OPTION_T1)
			{
				//MSG("(eng)Option Tray2 State (0x%x, 0x%x)\n", OptionTray1State[0] , OptionTray1State[1]);
				OptionTray2_State(OptionTray1State[0], OptionTray1State[1], OptionTray1Pro);
			}
			else
			{
				//MSG("(eng)Option Tray3 State (0x%x, 0x%x)\n", OptionTray2State[0] , OptionTray2State[1]);
				OptionTray3_State(OptionTray2State[0], OptionTray2State[1], OptionTray2Pro);
			}
		}
	}
}
