#include "spec.h"

#ifndef OPTION_H
#define OPTION_H

#define CMD_TRAY2_PICK				0xC1 // feeding
#define CMD_TRAY2_PICK_PREPICK	0xC2 // feeding
#define CMD_TRAY2_MTR_RUN			0xDA // passing
#define CMD_TRAY2_SENSOR_REQ		0xCE
#define CMD_TRAY2_SW_VERSION_REQ	0xBD
#define CMD_TRAY2_PRODUCT_ID_SET	0xBA
#define CMD_TRAY2_ACCUSPEEDSET		0xAE
#define CMD_TRAY2_CMD_FAULT			0x18

#define CMD_TRAY2_SYNC_1			0xE0
#define CMD_TRAY2_SYNC_2			0xE1
#define RX_TRAY2_SYNC_1			0xF0
#define RX_TRAY2_SYNC_2			0xF1

/*
#define CMD_TRAY2_PICK				0xC1
#define CMD_TRAY2_MTR_RUN			0xC3
#define CMD_TRAY2_MTR_STOP			0xC4
#define CMD_TRAY2_NOTIFY_CHK		0xC5
#define CMD_TRAY2_SW_VERSION_REQ	0xCA
#define CMD_TRAY2_PRODUCT_ID_REQ	0xCB
*/
// Receive Commands from tray2
#define CMD_TRAY2_SENSOR_RESPONSE	0xCE
#define CMD_TRAY2_SW_VERSION_RESPONSE 0xBD
#define CMD_TRAY2_PRODUCT_ID_RESPONSE 0xBA
/*
#define CMD_TRAY2_SW_VERSION_INFO	0xD1
#define CMD_TRAY2_PRODUCT_ID_INFO	0xD2
#define CMD_TRAY2_NOTIFY_RESULT		0xD3
*/

#define OPTION_T1	0xD0
#define OPTION_T2	0xD1
#define OPTION_T3	0xD2
#define OPTION_T4	0xD3

#define TRAY2_EMPTY			0x20
#define TRAY2_LOW			0x30
#define TRAY2_FULL			0x10
#define TRAY2_EMPTY_250		0x00
#define TRAY2_FULL_250		0x10

#if MAC_TYPE_M
#define TRAY2_MOTORRUNSPEED_KARAS_250	0xE29
#else
#define TRAY2_MOTORRUNSPEED_KARAS_250	0xC60
#endif

#if ABBA2 // Abba2
#if MAC_TYPE_M
#define TRAY2_MOTORRUNSPEED_KARAS	0x2493 // 534pps
#else
#define TRAY2_MOTORRUNSPEED_KARAS	0x201F // 608pps
#endif
#else // Kara/-S, Pinetree/-S
#if MAC_TYPE_L
#define TRAY2_MOTORRUNSPEED_KARAS	0x1965 // 769pps
#elif MAC_TYPE_M
#define TRAY2_MOTORRUNSPEED_KARAS	0x1D10
#else
#define TRAY2_MOTORRUNSPEED_KARAS	0x1965 // 769pps
#endif
#endif
//#define TRAY2_MOTORRUNSPEED_KARAS	0x17AD // 825pps
//#define TRAY2_MOTORRUNSPEED_KARAS	0x16ED // 851pps 3%
//#define TRAY2_MOTORRUNSPEED_KARAS	0x1680 // 868pps 5%
//#define TRAY2_MOTORRUNSPEED_KARAS	0x1618 // 884pps 7%
//#define TRAY2_MOTORRUNSPEED_KARAS	0x15B4 // 899pps 9%
//#define TRAY2_MOTORRUNSPEED_KARAS_T2	0x14A1 // 946pps 15%
//#define TRAY2_MOTORRUNSPEED_KARAS	0x13AE // 992pps 20%
//#define TRAY2_MOTORRUNSPEED_KARAS_T3	0x1213 // 1080pps 30%
#define TRAY2_MOTORRUNSPEED_KARAS_QUIET	0x2DD9//0x2E00//0xADD
#define TRAY2_MOTORSTOPSPEED 0xFFFF

#define TRAY2_CHECKSUM_ERROR		1
#define TRAY2_CHECKSUM_OK			0

#define OPTION_OPEN 0x70
#define NOTRECVBYTE	0x00

#define OPTION_TRAY_250 0x01
#define OPTION_TRAY_500 0x02
#define PRODUCT_NAME_KARA	0x02

#define FEEDSENSOR 0x40
#define FeedSensorDetect(x) (((x & FEEDSENSOR) == FEEDSENSOR) ? 1:0)

#define UA_DATA_LENGTH_0				0
#define UA_DATA_LENGTH_1				1
#define UA_DATA_LENGTH_2				2
#define UA_DATA_LENGTH_3				3
#define UA_DATA_LENGTH_4				4
#define UA_DATA_LENGTH_5				5
#define UA_DATA_LENGTH_6				6

#define ABBA_PPM_H					35
#define ABBA_PPM_M					32
#define ABBA_PPM_L					25
#define KARA_PPM					40
#define KARA_PPM_S					45

//#define KARAS_ACCUFEED_SPEED	0x193 // 403pps 30%
//#define KARAS_ACCUFEED_SPEED	0x174 // 372pps 20%
#if MAC_TYPE_L
#define KARAS_ACCUFEED_SPEED			0x129
#elif MAC_TYPE_M
#define KARAS_ACCUFEED_SPEED			0x104 // 40ppm
#else
#define KARAS_ACCUFEED_SPEED			0x129 // 45ppm
#endif
#define KARAS_ACCUFEED_SPEED_FOR_LOW	0xA5 // 25ppm

enum {
	T2_Motor_Stop,
	T2_Motor_Run,
	T2_Pick_Up,
	T3_Motor_Stop,
	T3_Motor_Run,
	T3_Pick_Up
};
void UartInitialize(void);
void OptionInitialize(void);
void PrtSigHandler(void);
void SendPacketData(void);
void Option_Tx_Detect(void);
void Option_Tx_Sync(char tray_num);
void Option_Tx_Version(char tray_num);
void Option_Tx_Product(char tray_num);
void Option_Tx_MotorRun(char tray_num, int motor_speed);
void Option_Tx_Pickup(char tray_num, int motor_speed, unsigned char fpre_pick);
void Option_Tx_Notify(char tray_num);
void Option_Tx_AccuSpeedSet(char tray_num, unsigned char motor_mode);
unsigned char* Get_OptionTray1_Version(void);
unsigned char* Get_OptionTray2_Version(void);
//void OptionTray_State(unsigned char Tray_num, unsigned char Tray_vol, unsigned char Tray_size);
void OptionTray2_State(unsigned char Tray_vol, unsigned char Tray_size, unsigned char Tray_capa);
void OptionTray3_State(unsigned char Tray_vol, unsigned char Tray_size, unsigned char Tray_capa);
void Tray2_Detection_Check(void);
void Pending_Tray2_Detection_Result(void);
void rx_option_handler(int length, char *buf);
void Set_Timer_UART(void);
int Get_Elapse_UART(void);
int CheckSum(int, unsigned char*);
void parsing(unsigned char*);
#endif
