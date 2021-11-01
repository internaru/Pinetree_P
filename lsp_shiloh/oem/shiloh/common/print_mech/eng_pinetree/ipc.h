/*+---------------------------------------------------------------------------+
 ** 								    								
 **	 File Name :	mymsg.h
 **	 Revision  :	0.2
 **	 Date	   : 	JUL 1, 2009
 **	 Author    :	Inhwan, Oh
 **  Overview of this file
 **
 **	 Modified Information :											
 ** 
 +---------------------------------------------------------------------------+*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h> 


/********************************************************************
*  3. DEFINES
********************************************************************/

// Video Interface Command Code
/*HVT CMD*//////////////////////////////////////////
// #define	Engine_Restart						0x00
// #define	Print_Page							0x50
// #define	Print_Ready							0x02
#define	HVT_Complete						0x99
/*HVT CMD*//////////////////////////////////////////

#define	Engine_Restart						0xA0
#define	Set_Engine_POR_Parameter			0xA1
#define	Clear_Intervention_Required_Errors	0xF0
#define	Power_Saver_mode					0xE0
#define Wake_Up                       		0xE1
#define	Print_Page							0xB0
#define	Early_Start_Print					0xB1
#define	Query_Attached_Devices				0xC0
#define	Query_Device_Information			0xC1
#define	Query_Device_Inventory				0xC2
#define	Query_Machine_Check_Information		0xC3
#define	Query_Engine_Software_Info			0xC4
#define	Query_Jammed_Paper_Locations		0xC5
#define HSync_FAIL							0x40
#define HSync_OKAY							0x41	
#define Toner_Count							0x42
#define Total_Print_Count					0x43
#define	Magnification_Chg					0x60
#define	Sensor_Test_Start					0x61
#define	Sensor_Test_End						0x62
#define	Solenoid_Test_Start					0x63
#define	Solenoid_Test_End					0x64
#define	Motor_Test_Start_T1					0x65
#define	Motor_Test_Start_T2					0x66
#define	Motor_Test_Start_T3					0x67
#define	hardware_test_off					0x6F
#define Cold_Wake_Up						0xE2	

//#define	Set_Engine_Test_Parms				0x01
//#define	Now_Querying_Notify					0x06
//#define	Request_Download_Code				0x0c
//#define	Download_Code_Block					0x0d
//#define	Query_Notify						0x15
//#define	Query_Boot_Software					0x18
//#define	Start_Warmup						0x53
//#define	Request_Device_Information			0x58
//#define	Query_Debug_Data					0x5d
//#define	Query_Cover_Status					0x61
//#define	Query_Tray_Software					0x74
//#define Engine_Shutdown						0x89
//#define Send_Notify                         0xac
//#define HVT_Complete	                    0x99

// Video Interface Notify Set Code
#define	Print_Ready							0x01
#define	Power_Saver_Active					0x04
#define	Configuration_Change_Error			0x90
#define	Print_Complete						0x24
#define	Cover_Open							0x81
#define	Cover_Closed						0x82
#define	Machine_Check						0x91
#define Transport_Motor_Status				0x20
#define Source_Empty						0x52
#define Paper_Jam							0x92
#define Print_Processed_Info				0x10
#define Attached_Devices_Info				0x11
#define Capacity_of_Sheets_Info				0x12
#define Current_Media_Level_Info			0x13
#define Machine_Check_Info					0x14
#define Tray_SW_Ver_Info					0x15
#define LSU_ON								0x30
#define LSU_OFF								0x31
#define Smart_IC_Error						0x93
#define Amount_Toner_Used					0x94
#define Cartridge_TLI_Serial_Info			0x55
#define Sensor_Status_Info					0x50	
#define Ignore_EarlyStart					0xB3
#define Wake_Key_Info						0x96
#define Full_Stack_Info						0x95
#define Cooling_Info						0x94
//#define	Expansion_Notifies_Min				0x32
//#define	Expansion_Notifies_Max				0x3F
//#define	Engine_Warming_Up					0x59
//#define	Data_Available                      0x60
//#define	No_Notify_Pending					0x7f

   
struct ipcbuf
{
    long msgtype;
	unsigned int cmd;	// cmd 1 byte
	unsigned int cd1, cd2, cd3, cd4, cd5, cd6, cd7, cd8, cd9, cd10, cd11, cd12, cd13, cd14, cd15, cd16;	// cmd data 8 byte
};

/********************************************************************
*  FUNCTION PROTOTYPES
********************************************************************/

extern int send_message(int qid, struct ipcbuf *qbuf, long type);
extern int read_message(int qid, struct ipcbuf *qbuf, long type);
extern int remove_queue(int qid);

void message_queue_open();
void send_msgQ();
void remove_msgQ();

#define MEDIA_SIZE  0x04    // A4
