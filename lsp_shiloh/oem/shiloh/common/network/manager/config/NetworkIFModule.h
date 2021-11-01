#ifndef _NETWORKIFMODULE_H_
#define _NETWORKIFMODULE_H_
	#define	SDH_NETIF_VERSION	"1.2.1"	//New Driver support // Realtek support, by mspyo
	const char * getVer_NIF(void);
	
	extern int gWifiModuleType; // Global. by mspyo. 2021-02-15
	enum {
		SDH_NET_WIFI_MODULE_NONE = 0,
		SDH_NET_WIFI_MODULE_MARVELL = 10,
		SDH_NET_WIFI_MODULE_REALTEK = 20,
	}; // Wifi module type. by mspyo. 2021-02-15

	typedef union 
	{
		unsigned char ipAddr[4];
		unsigned int ipAddr_4Byte;
	} T_IPADDRv4 ;
	
	typedef struct
	{
		//Below: Should fill by function call
		char * name;
		char * hwAddr;
		
		int radioQuality;			//Percent
		char authenticationType;	//0: Not secure, 1: WEP, 2: WPA1, 3: WPA2 ...
		char ifHiddenSSID;			//0, 1:In this case, name shall be "[Hidden]"
		char groupCipher;			//1: CCMP(AES), 2:TKIP ...
		char pairCipher;			//1: CCMP(AES), 2:TKIP ...
		char ifSupportPreAuth;		//0, 1:???
		char authSuite;				//1: 802.1x, 2:PSK
		char * unknownOption;
		
		//Below: Should fill by user.
		char PassKeySelection;		//Only for WEP: 1,2,3,4
		char * passKey1;
		char * passKey2;
		char * passKey3;
		char * passKey4;
		//int tmp;
	} T_SSID;
	
	enum ENUM_authenticationType
	{
		NIF_AUTH_NOSECURE		=0,
		NIF_AUTH_WEP 			=1,
		NIF_AUTH_WPAPSK			=2,
		NIF_AUTH_WPA2PSK		=4,
		NIF_AUTH_WPAPSK_WPA2PSK	=6,
		NIF_AUTH_802X1			=8,
		NIF_AUTH_UNKNOWN		=0x10	
	};
	
	enum ENUM_cipherType
	{
		NIF_CIPHER_NONE			= 0,
		NIF_CIPHER_CCMP			= 1,
		NIF_CIPHER_AES			= 1,
		NIF_CIPHER_TKIP			= 2,
		
		NIF_CIPHER_CCMP_TKIP	= 3,	//Support Both.
		NIF_CIPHER_AES_TKIP		= 3,
	};
	
	enum ENUM_authSuite
	{
		NIF_AUTHSUITE_802X1		= 1,
		NIF_AUTHSUITE_PSK		= 2,
	};
	
	//Check interface
	enum getStatusReturnType
	{
		NIF_Status_NotFound	= -1,
		NIF_Status_IfDown	= 0,
		NIF_Status_IfUp		= 1,
		
		NIF_Status_Static	= 10,
		NIF_Status_Dynamic	= 11,
		NIF_Status_Unknown	= 12,
	};
	
	enum ifEnumType
	{
		NIF_IF_WIRED		=	1,
		NIF_IF_WIFI			=	2,
	};
	
	//----- Ping Functions
	int NIF_pingToHost(T_IPADDRv4 *ipAddr);
	int NIF_arpingToHost(T_IPADDRv4 *ipAddr);
	
	//----- Interface up/down related functions (normaly does not required)
	int NIF_getStatus_wired(void);
	int NIF_getStatus_wifi(void);
	int NIF_autoIF_Wired(void);
	int NIF_autoIF_Wifi(void);
	
	//----- IPv4 IP Address related functions	
	int NIF_getIPAddrV4_int_wired(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getIPAddrV4_int_wifi(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getIPAddrV4_str_wired(char **ipAddr);				//real set value.
	int NIF_getIPAddrV4_str_wifi(char **ipAddr);				//real set value.
	int NIF_getIpAddrV4FromFile(char *ifName, char **IpAddr);	//saved setting value (which may not be used, actually)
	
	//----- IPv4 Netmask Address related functions
	int NIF_getNetmaskV4_int_wired(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getNetmaskV4_int_wifi(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getNetmaskV4_str_wired(char **ipAddr);				//real set value.
	int NIF_getNetmaskV4_str_wifi(char **ipAddr);				//real set value.
	int NIF_getNetmaskV4FromFile(char *ifName, char **IpAddr);	//saved setting value (which may not be used, actually)
	
	//----- IPv4 Gateway Address related functions
	int NIF_getGatewayV4_int_wired(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getGatewayV4_int_wifi(T_IPADDRv4 *ipAddr);			//real set value.
	int NIF_getGatewayV4_str_wired(char **ipAddr);				//real set value.
	int NIF_getGatewayV4_str_wifi(char **ipAddr);				//real set value.
	int NIF_getGatewayV4FromFile(char *ifName, char **IpAddr);	//saved setting value (which may not be used, actually)
	
	//----- IPv6 related functions
	int NIF_getIPAddrV6_int_wired(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen);
	int NIF_getIPAddrV6_int_wifi(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen);
	int NIF_getIPAddrV6_str_wired(char ** ipAddr);
	int NIF_getIPAddrV6_str_wifi(char ** ipAddr);
	
	//----- Ethernet Address Functions
	int NIF_getMacAddr_int_wired(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6);
	int NIF_getMacAddr_int_wifi(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6);
	int NIF_getMacAddr_str_wired(char ** ipAddr);
	int NIF_getMacAddr_str_wifi(char ** ipAddr);
	int NIF_getMacAddrFromFile(char *ifName, char **macAddr);


	//----- Address setting releated functions
	int NIF_getDHCPStatus_wired(void);		//Done
	int NIF_getDHCPStatus_wifi(void);		//Done

	int NIF_setDHCPDynamic_wired(void);		//Done
	int NIF_setDHCPDynamic_wifi(void);		//Done
	int NIF_setDHCPDynamic_all(void);		//Done	
	
	int NIF_setDHCPStatic_wired(T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr);
	int NIF_setDHCPStatic_wifi(T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr);
	
	
	//----- Find/Connect/Check SSID related functions
	int NIF_findSSID(int * NumberOfSSID, T_SSID ** FoundAPList, int minimumQuality);		//Fill NumberOfSSID, FoundAPList
	int NIF_freeSSID(int NumberOfSSID, T_SSID * FoundAPList);		
		//NumberOfSSID should be exact same of findSSID!
		//If success NIF_Status_IfUp returned, else NIF_Status_NotFound returned	
	int NIF_connectSSID(int RowOfSSID, T_SSID * FoundAPList);
		//RowOfSSID: Selected SSID Number should be 0 <= RowOfSSID < NumberOfSSID
		//NumberOfSSID should be reserved for free
		//If kind of secure mode, passkey1,2,3,4 (one of them) should be filled.
	int NIF_connectStatus_wifi(void);			//return connected or not
	int NIF_getConnectedSSID(char **SSID);		//return connected or not / if connected, connected SSID.
	
	
	//Wifi machine related functions
	int NIF_turnRadio_on_wifi(void);
	int NIF_turnRadio_off_wifi(void);
	
		
	//DB Releated Functions
	int NIF_checkSSIDinDB(int RowOfSSID, T_SSID * FoundAPList);					//if return == NIF_Status_IfUp then fill passkey1,2,3,4 
	int NIF_getOneSSIDinDB(int * NumberOfSSID, T_SSID ** FoundAPList);			//Get the lastest saved SSID
	int NIF_getOneSSIDinDBbySSID(int * NumberOfSSID, T_SSID ** FoundAPList, char *SSIDName);			//if return == NIF_Status_IfUp, success to find saved SSID
	
	int NIF_updateSSIDinDB(int RowOfSSID, T_SSID * FoundAPList);		
	int NIF_updateConnStatus_wifi(int IsOnOff);
	void NIF_turnoffUAP_wifi(void);
	int NIF_deleteAllSSIDinDB(void);
	int NIF_viewSSIDInformation(int RowOfSSID, T_SSID * FoundAPList);


	//--------Will be implemented--------//
	//DNS
	int NIF_getDNSAddr_int(T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr);
	int NIF_getDNSAddr_str(char ** DNS1Addr, char ** DNS2Addr);
	//int NIF_getDNSAddr_str_wifi(char ** DNS1Addr, char ** DNS2Addr);
	int NIF_turnMDNS_on(void);

	int NIF_PowerSave_on_wifi(void);
	int NIF_PowerSave_off_wifi(void);

#endif
