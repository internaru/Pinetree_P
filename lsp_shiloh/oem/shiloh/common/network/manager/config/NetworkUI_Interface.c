#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "NetworkIFModule.h"
#include "NetworkUI_Interface.h"
#include "logger.h"

#define NETWORKD 		"/etc/network/"

#define SHOW_IPV4 		"SHOW_IPV4"
#define SHOW_IPV6		"SHOW_IPV6"
#define SHOW_MAC		"SHOW_MAC"
#define SETUP_STATIC_WIRED	"SETUP_STATIC"
#define SETUP_STATIC_WIFI	"SETUP_STATIC_WIFI"
#define SSID_LIST		"SSID_LIST"
#define CONN_SSID       "CONN_SSID"

#define DBG_PRFX "[NUI]:"
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT(0)
#define PrintDebugLOG

#define false 0
#define true 1
#define NET_CONN_TIMEOUT 20

T_SSID *ptrSSIDList=NULL;
int N_SSIDLIST = 0;

#define ENABLE_WIFI_AUTO_CONNECT
#ifdef ENABLE_WIFI_AUTO_CONNECT
extern pthread_mutex_t g_wifi_autoconnect_mutex;
extern bool isWifiConnecting;
#define WIFI_AUTO_CONNECT_LOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_lock(&g_wifi_autoconnect_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 

#define WIFI_AUTO_CONNECT_UNLOCK() \
        { \
            uint32_t pthread_rcode; \
            pthread_rcode = pthread_mutex_unlock(&g_wifi_autoconnect_mutex); \
            XASSERT(pthread_rcode == 0, pthread_rcode); \
        } 
#endif

int FilePrint(const char* filename, const char* message){

	    if (!filename){
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:FilePrint filename MISSED!\n",__FILE__,__LINE__);
			#endif
			return false;
		}
		if (!message){
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:FilePrint message MISSED!\n",__FILE__,__LINE__);
			#endif
			return false;

		}

		FILE* file;
		if((file = fopen(filename,"w+")) == NULL){		//fileopen
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:FilePrint fileopen ERROR!\n",__FILE__,__LINE__);
			#endif

			return false;
		}
		else{
			DBG_PRINTF_SDH_DEBUG("[NETWORK]:File Open SUCCESS!\n");
			fprintf(file,"%s",message);
		}
		if(fflush(file) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(file->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(file);
		DBG_PRINTF_SDH_DEBUG("[NETWORK]:File Closed\n");

	    return true;
}       


int NUI_getIPAddrV4_wired(){

	T_IPADDRv4 ipAddr;
	char ipAddrStr[32];

	//DBG_PRINTF_SDH_ERR("Hello world. %d\n", 626);
                                                                                                                                  
    if( NIF_getIPAddrV4_int_wired(&ipAddr) == NIF_Status_NotFound ){	//getIpAddrV4                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv4_int_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
    }
	else{
		DBG_PRINTF_SDH_DEBUG("[NETWORK]:getIPAddrv4_int_wired SUCCESS!\n");
	}
    
	sprintf(ipAddrStr,"%03d.%03d.%03d.%03d",ipAddr.ipAddr[0],ipAddr.ipAddr[1],ipAddr.ipAddr[2],ipAddr.ipAddr[3]);
    if(FilePrint(NETWORKD SHOW_IPV4,ipAddrStr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv4_int_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
	}
	else{
		DBG_PRINTF_SDH_DEBUG("[NETWORK]:getIPAddrv4_int_wired FilePrint SUCCESS!\n");
	}
   
	return true;                                                                                                                 
}

int NUI_getIPAddrV6_wired(){

    char *ipAddr = 0;
	int result = true;
 
    if( NIF_getIPAddrV6_str_wired(&ipAddr) == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv6_str_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;
    }                                                                                                                
    
	if(FilePrint(NETWORKD SHOW_IPV6,ipAddr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv6_str_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;
	}
	free(ipAddr);	  	
 
	return result;                                                                                                                 
}

int NUI_getMacAddr_wired(){

    char *macAddr = 0;
	int result = true;

    if( NIF_getMacAddr_str_wired(&macAddr) == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getMacAddr_str_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
    }                                                                                                                
    
	if(FilePrint(NETWORKD SHOW_MAC,macAddr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getMacAddr_str_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;                                                                                
	}
	free(macAddr);

	return result;                                                                                                                 
}

int NUI_setDHCPDynamic_wired(){

	if( NIF_setDHCPDynamic_wired() == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:setDHCPDynamic_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
    }

	return true;
}

int NUI_setDHCPStatic_wired(){

	FILE *fp;
	enum {ENUM_IPADDR, ENUM_NETMASK, ENUM_GATEWAY, ENUM_DNS1, ENUM_DNS2, ENUM_SIZE};	
	T_IPADDRv4 ptrAddr[ENUM_SIZE]; memset(&ptrAddr,    0, sizeof(ptrAddr));
	char tmpStrBuf[64];
	int i;
	int result = true;
	

	if((fp = fopen(NETWORKD SETUP_STATIC_WIRED,"r")) == NULL){		//fileopen
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:setDHCPStatic_wired fileopen ERROR!\n",__FILE__,__LINE__);
		#endif
		return false;
	}

	for(i = 0; i<ENUM_SIZE; ++i){
        
		memset(tmpStrBuf,0,sizeof(tmpStrBuf));
        fgets(tmpStrBuf,sizeof(tmpStrBuf),fp);

		if(tmpStrBuf == NULL){
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:fgets FAILED!!\n",__FILE__,__LINE__);
			#endif
		}
		else{
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:%s",__FILE__,__LINE__,tmpStrBuf);
			#endif

			ptrAddr[i].ipAddr[0]=atoi(strtok(tmpStrBuf,"."));
    	    ptrAddr[i].ipAddr[1]=atoi(strtok(NULL,"."));
	        ptrAddr[i].ipAddr[2]=atoi(strtok(NULL,"."));
    	    ptrAddr[i].ipAddr[3]=atoi(strtok(NULL,"."));

			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:%d.%d.%d.%d",__FILE__,__LINE__,ptrAddr[i].ipAddr[0],ptrAddr[i].ipAddr[1],ptrAddr[i].ipAddr[2],ptrAddr[i].ipAddr[3]);
			#endif
		}
	}
	
	if(NIF_setDHCPStatic_wired(&ptrAddr[ENUM_IPADDR],&ptrAddr[ENUM_NETMASK],&ptrAddr[ENUM_GATEWAY],&ptrAddr[ENUM_DNS1],&ptrAddr[ENUM_DNS2])==NIF_Status_NotFound){
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:setDHCPStatic_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;                                                                                
    }

	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(fp);

	//sleep(1);
	NIF_arpingToHost(&ptrAddr[ENUM_GATEWAY]);

	return result;
}

int NUI_getIPAddrV4_wifi(){

	T_IPADDRv4 ipAddr;
	char ipAddrStr[32];
                                                                                                                                  
    if( NIF_getIPAddrV4_int_wifi(&ipAddr) == NIF_Status_NotFound ){	//getIpAddrV4                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv4_int_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
    }                                                                                                                
    
	sprintf(ipAddrStr,"%03d.%03d.%03d.%03d",ipAddr.ipAddr[0],ipAddr.ipAddr[1],ipAddr.ipAddr[2],ipAddr.ipAddr[3]);
    if(FilePrint(NETWORKD SHOW_IPV4,ipAddrStr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv4_int_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
	}
   
	return true;                                                                                                                 
}

int NUI_getIPAddrV6_wifi(){

    char *ipAddr = 0;
	int result = true;
 
    if( NIF_getIPAddrV6_str_wifi(&ipAddr) == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv6_str_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;
    }                                                                                                                
    
	if(FilePrint(NETWORKD SHOW_IPV6,ipAddr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getIPAddrv6_str_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;
	}
	free(ipAddr);	  	
 
	return result;                                                                                                                 
}

int NUI_getMacAddr_wifi(){

    char *macAddr = 0;
	int result = true;

    if( NIF_getMacAddr_str_wifi(&macAddr) == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getMacAddr_str_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		if(macAddr)
		{
		   free(macAddr);
		   macAddr=NULL;
		}
		return false;                                                                                
    }                                                                                                                
    
	if(FilePrint(NETWORKD SHOW_MAC,macAddr) == false){				//file print
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:getMacAddr_str_wired FilePrint FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;                                                                                
	}

	if(macAddr)
	{
	   free(macAddr);
	   macAddr=NULL;
	}
	return result;                                                                                                                 
}

int NUI_getSSIDList_wifi(){

	const int minQuality = 2;
	int result = true;

	system("ifconfig mlan0 up"); // by mspyo, 2021-08-19

	//TurnOff UAP
	NIF_turnoffUAP_wifi();
/*
	DBG_PRINTF_SDH_DEBUG("********FORDEBUG******\n");
	DBG_PRINTF_SDH_DEBUG("installwifi\n");
	system("./installwifi.sh");
	DBG_PRINTF_SDH_DEBUG("startwifi\n");
	system("./startwifi.sh &");
*/
	//iwlist mlan0 scanning
    if(NIF_findSSID(&N_SSIDLIST, &ptrSSIDList, minQuality) != NIF_Status_IfUp){
		return false;
	} 

    if (ptrSSIDList != NULL){
    
    	int i=0;
		char *message = 0;
	
		//memory allocation
		if ((message = (char *) malloc(N_SSIDLIST*128*sizeof(char))) == NULL) {
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:Memory Allocation FAILED!\n",__FILE__,__LINE__);
			#endif
			return false;
	  	}
		else{
			memset(message, 0x00, sizeof(message));	
		}

	    for (i=0;i<N_SSIDLIST;i++){   

			char temp_message[128];
			char *token = " ";
			T_SSID * currentItem = (void *)( (unsigned int)ptrSSIDList+(sizeof(T_SSID)*i) );
			
//			sprintf(temp_message,"%d%s%s%s%d%s%d\n",i,token,currentItem->name,token,currentItem->radioQuality,
//					token,currentItem->authenticationType);
			sprintf(temp_message,"%d%s%d%s%s\n",currentItem->radioQuality,token,currentItem->authenticationType,token,currentItem->name);
			strcat(message,temp_message);
		}

		if(FilePrint(NETWORKD SSID_LIST,message) == false){				//file print
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:getSSID_LIST FilePrint FAILED!\n",__FILE__,__LINE__);
			#endif
			result = false;                                                                                
		}
		free(message);
	}   

	#ifdef PrintDebugLOG
	DBG_PRINTF_SDH_DEBUG("[%s]%d:NIF Free Result:%d\n",__FILE__,__LINE__,result);
	#endif
	
	return result;
	
	//return true;
}

int NUI_setDHCPDynamic_wifi(){

	if( NIF_setDHCPDynamic_wifi() == NIF_Status_NotFound ){                                                 
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:setDHCPDynamic_wired FAILED!\n",__FILE__,__LINE__);
		#endif
		return false;                                                                                
    }

	return true;
}

int NUI_setDHCPStatic_wifi(){
	
	FILE *fp;
	enum {ENUM_IPADDR, ENUM_NETMASK, ENUM_GATEWAY, ENUM_DNS1, ENUM_DNS2, ENUM_SIZE};	
	T_IPADDRv4 ptrAddr[ENUM_SIZE]; memset(&ptrAddr,    0, sizeof(ptrAddr));
	char tmpStrBuf[64];
	int i;
	int result = true;
	

	if((fp = fopen(NETWORKD SETUP_STATIC_WIFI,"r")) == NULL){		//fileopen
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:setDHCPStatic_wifi fileopen ERROR!\n",__FILE__,__LINE__);
		#endif
		return false;
	}

	for(i = 0; i<ENUM_SIZE; ++i){
        
		memset(tmpStrBuf,0,sizeof(tmpStrBuf));
        fgets(tmpStrBuf,sizeof(tmpStrBuf),fp);

		if(tmpStrBuf == NULL){
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:fgets FAILED!!\n",__FILE__,__LINE__);
			#endif
		}
		else{
			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:%s",__FILE__,__LINE__,tmpStrBuf);
			#endif

			ptrAddr[i].ipAddr[0]=atoi(strtok(tmpStrBuf,"."));
    	    ptrAddr[i].ipAddr[1]=atoi(strtok(NULL,"."));
	        ptrAddr[i].ipAddr[2]=atoi(strtok(NULL,"."));
    	    ptrAddr[i].ipAddr[3]=atoi(strtok(NULL,"."));

			#ifdef PrintDebugLOG
			DBG_PRINTF_SDH_DEBUG("[%s]%d:%d.%d.%d.%d",__FILE__,__LINE__,ptrAddr[i].ipAddr[0],ptrAddr[i].ipAddr[1],ptrAddr[i].ipAddr[2],ptrAddr[i].ipAddr[3]);
			#endif
		}
	}
	
	if(NIF_setDHCPStatic_wifi(&ptrAddr[ENUM_IPADDR],&ptrAddr[ENUM_NETMASK],&ptrAddr[ENUM_GATEWAY],&ptrAddr[ENUM_DNS1],&ptrAddr[ENUM_DNS2])==NIF_Status_NotFound){
    	#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_ERR("[%s]%d:setDHCPStatic_wifi FAILED!\n",__FILE__,__LINE__);
		#endif
		result = false;                                                                                
    }
	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(fp);

	return result;
}
#ifdef ENABLE_WIFI_AUTO_CONNECT
extern enum getStatusReturnType checkifConnected(char *ifName);
extern void setWiFiConnecting(bool isWifiConnecting);
#endif
int NUI_ConnectOn_wifi(){

	//DB에서 최근값하나를 불러온 후
	T_SSID * ptrAPList;
	int NumberOfSSID;
	int result;
	//printf ("1\n");
#ifdef ENABLE_WIFI_AUTO_CONNECT
	while(isWifiConnecting == true)
	{
		DBG_PRINTF_SDH_ERR("Already in Connecting State..Wait and try again");
		sleep(3);
	}
	setWiFiConnecting(true);
#endif
	NIF_turnoffUAP_wifi();
	result=NIF_getOneSSIDinDB(&NumberOfSSID,&ptrAPList);
	//printf ("2\n");
	if (result == NIF_Status_IfUp)
	{
		int i;

		NIF_connectSSID(0,ptrAPList);
	 	//TIMEOUT초간 기다리고 :8초
        for (i=0;i<NET_CONN_TIMEOUT;i++){
           
            DBG_PRINTF_SDH_DEBUG("Wait for %d Sec...\n",i);
            if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
                break;
            else
                sleep(1);
        }   
		//check if connect or not
	    if (NIF_connectStatus_wifi() == NIF_Status_IfUp){
			NIF_autoIF_Wifi();

			if(NIF_updateConnStatus_wifi(1)== NIF_Status_IfUp){ //TurnON
				result = true;
				DBG_PRINTF_SDH_DEBUG("[%s]%d:WIFI_Connection_SUCCESS!!!\n",__FILE__,__LINE__);
			}
		}
		else{
			DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__FILE__,__LINE__);
			result = false;
		}
        NIF_freeSSID(NumberOfSSID,ptrAPList);   
		NIF_turnMDNS_on();//turn on airprint again.
	}
	else{
		DBG_PRINTF_SDH_ERR("(Boot Wifi) Connection aborted, FindResult: %d\n",result);
		result = false;
	}
#ifdef ENABLE_WIFI_AUTO_CONNECT
	setWiFiConnecting(false);
#endif
	return result;
}

int NUI_ConnectOff_wifi(){

	#define stop_wifi_sh "/etc/init.d/stop_wifi_sta.sh"


	DBG_PRINTF_SDH_DEBUG("[%s]%d:WIFI_ConnectOff!!!\n",__FILE__,__LINE__);
	system(stop_wifi_sh);
    NIF_updateConnStatus_wifi(0); //TurnOff

	return true;
}

int NUI_ConnectSSID_wifi(){

	FILE* file = NULL;
	int selectedSSID = 0;
	char passKey[128];
	int result = 0;
	int i;

	if(ptrSSIDList == NULL){
		return false;
	}
	memset(passKey, 0x00, sizeof(passKey));

	if((file = fopen(NETWORKD CONN_SSID,"r")) == NULL){		//fileopen
		#ifdef PrintDebugLOG
		DBG_PRINTF_SDH_DEBUG("[%s]%d:FilePrint fileopen ERROR!\n",__FILE__,__LINE__);
		#endif
		return false;
	}
	else{
		fscanf(file,"%d %s",&selectedSSID,passKey);
		DBG_PRINTF_SDH_ERR("SSID : %d key : %s",selectedSSID,passKey);
		printf("SSID : %d key : %s",selectedSSID,passKey);
	}
	if(fflush(file) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
	if(fsync(file->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(file);

	if(strlen(passKey) > 1){
		printf("STRLEN(PASSKEY)%d \n",strlen(passKey));
		if(ptrSSIDList == NULL){
			DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__FILE__,__LINE__);
			return false;
		}
		else{
			// Enter the password
			ptrSSIDList[selectedSSID].passKey1 =(char*)calloc(strlen(passKey)+1,1);
		}
		//DBG_PRINTF_SDH_DEBUG("%dPASSWORD:::::::::%s\n",selectedSSID,passKey);
	    strcpy(ptrSSIDList[selectedSSID].passKey1,passKey);
	}
	else{
		if(ptrSSIDList[selectedSSID].authenticationType != NIF_AUTH_NOSECURE){	
			NIF_freeSSID(N_SSIDLIST, ptrSSIDList);
			return false;
		}
	}

	//connectSSID
    if(NIF_connectSSID(selectedSSID,ptrSSIDList) == NIF_Status_IfUp){
		
	 	//TIMEOUT초간 기다리고 :8초
        for (i=0;i<NET_CONN_TIMEOUT;i++){
           
            DBG_PRINTF_SDH_DEBUG("Wait for %d Sec...\n",i);
            if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
                break;
            else
                sleep(1);
        }   
		//check if connect or not
	    if (NIF_connectStatus_wifi() == NIF_Status_IfUp){
			NIF_autoIF_Wifi();
			if(NIF_updateConnStatus_wifi(1)== NIF_Status_IfUp){ //TurnON
				NIF_updateSSIDinDB(selectedSSID,ptrSSIDList); // save SSID in DB
				DBG_PRINTF_SDH_DEBUG("[%s]%d:WIFI_Connection_SUCCESS!!!\n",__FILE__,__LINE__);
				result = true;
			}
		}
		else{
			DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__FILE__,__LINE__);
			result = false;
		}
	}
	else{
		DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__FILE__,__LINE__);
		result = false;
	}

	//freeSSID
	NIF_freeSSID(N_SSIDLIST, ptrSSIDList);
	NIF_turnMDNS_on();//turn on airprint again.

	return result;
}

int NUI_ClearSSID_wifi(){

	#define stop_wifi_sh "/etc/init.d/stop_wifi_sta.sh"


	if(NIF_deleteAllSSIDinDB() == NIF_Status_Unknown){
		DBG_PRINTF_SDH_ERR("[%s]%d:ClearSSID_FAILED!!!\n",__FILE__,__LINE__);
		return false;
	}
	else{
		DBG_PRINTF_SDH_DEBUG("[%s]%d:ClearSSID__SUCCESS!!!\n",__FILE__,__LINE__);
		system(stop_wifi_sh);
        NIF_updateConnStatus_wifi(0); //TurnOff
	}

	return true;
}
