/*+-----------------------------------------------------------------+
 ** 	Microcode for ABBA/OAK  Engine Application								    							|
 **								    								
 **	 File Name :	i2c_protocol.h					    				
 **	 Revision  :	ver 1.4								    			
 **	 Date	   : 	Dec 21, 2009										
 **	 Author    :	Youngjin Lee		
 +-----------------------------------------------------------------+*/

/*****************************************************
* I2C Protocol Header file							 *
*****************************************************/


/*+-----------------------------------------------------------------+
 <Communication with Option Tray>
+-----------------------------------------------------------------+*/

// Request Commands to tray2
#define CMD_TRAY2_PICK				0xC1
#define CMD_TRAY2_MTR_RUN			0xC3
#define CMD_TRAY2_MTR_STOP			0xC4
#define CMD_TRAY2_NOTIFY_CHK		0xC5
#define CMD_TRAY2_SW_VERSION_REQ	0xCA
#define CMD_TRAY2_PRODUCT_ID_REQ	0xCB

// Receive Commands from tray2
#define CMD_TRAY2_SW_VERSION_INFO	0xD1
#define CMD_TRAY2_PRODUCT_ID_INFO	0xD2
#define CMD_TRAY2_NOTIFY_RESULT		0xD3

#define TRAY2_CHECKSUM_ERROR		1
#define TRAY2_CHECKSUM_OK			0

#define DATA_LENGTH_0				0
#define DATA_LENGTH_1				1
#define DATA_LENGTH_2				2
#define DATA_LENGTH_3				3

#define ABBA_PPM_H					35
#define ABBA_PPM_M					32
#define ABBA_PPM_L					25
        
enum {
	T2_Motor_Stop,
	T2_Motor_Run,
	T2_Pick_Up
};

/*+-----------------------------------------------------------------+
 <Communication with Smart IC>

* Header = Command(3bit:7~5) + Length(5bit:4~0)
 1. Command & Length
  - Command = 0 : Certificate key send (Length : 16)
  - Command = 1 : Certificate key receive (Length : 3)
  - Command = 2 : Memory Write (Length : 4)
  - Command = 3 : Memory Read (Length : 4)
  
* Packet
 1. Certificate Key Send
  - Header(0x10 : 000 10000) + Key1 + ... + Key16 + Check Sum
 2. Certificate Key Receive
  - Header(0x23 : 001 00011) + Check Sum > Return value = Key1 + Key2 + Key3 + Check Sum 
 3. Memory Write
  - Header(0x44 : 010 00100) + Offset(0x00~0x7f) + Data1 + ... + DataN + Check Sum
 4. Memory Read
  - Header(0x64 : 011 00100) + Offset(0x00~0x7f) + Check Sum > Return value = Data1 + Data2 + Data3 + Data4 + Check Sum 
 
+-----------------------------------------------------------------+*/
 
#define SMART_IC_CERTIFICATION_SEND_CMD		0x10
#define SMART_IC_CERTIFICATION_RECEIVE_CMD	0x23
#define SMART_IC_ADVENCED_CERTIFICATION_RECEIVE_CMD	0x2C
#define SMART_IC_MEMORY_WRITE_CMD			0x44
#define SMART_IC_MEMORY_READ_CMD			0x64
#define SMART_IC_MEMORY_WRITE_ENC_CMD		0x48
#define SMART_IC_MEMORY_PRE_READ_ENC_CMD	0x61
#define SMART_IC_MEMORY_READ_ENC_CMD		0x68

