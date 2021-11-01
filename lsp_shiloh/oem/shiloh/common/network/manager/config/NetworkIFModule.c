#include <net/if.h>
#include <netinet/in.h>
#include <string.h>
#include <linux/sockios.h>
#include <sys/ioctl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "NetworkIFModule.h"
#include <sqlite3.h>
#include "os_system.h" 

#include "./wpaSupport/common.h"
#include "./wpaSupport/includes.h"
#include "./wpaSupport/sha1.h"
//#include "../../WebServer/cgi-bin/commonLibrary/ipc_msg.h"

#include "logger.h"
#include "db_api.h"

#define WIRED_IF_NAME	"eth0"
#define WIFI_IF_NAME	"mlan0"

#define WPA_PID_FILENAME	"/var/run/wpa_supplicant.mlan0.pid"
#define DNS_PID_FILENAME	"/var/run/udhcpc.mlan0.pid"
#define ADDR	(((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr)
#define DBTRACKINGSSID	5

#define DBG_PRFX "[NUI]:"
#define LOGGER_MODULE_MASK DEBUG_LOGGER_MODULE_NETWORK | LOGGER_SUBMODULE_BIT(0)

enum
{
	ENUM_IF_IPADDR,
	ENUM_IF_NETMASK,
	ENUM_IF_GATEWAY,
};

#define IPADDR_STR_SIZE			20
#define IPADDRV6_STR_SIZE		80
#define INTERFACE_FILENAME	"/etc/network/interfaces"
#define DNSSERVER_FILENAME	"/etc/resolv.conf"
#define MAX_CHAR_PER_LINE		200

static int HexToInt(char *Str);
static int NIF_getStatus(char *ifName);
//static int NIF_setIFUp(char *ifName);
//static int NIF_setIFDown(char *ifName);	
static int NIF_getAddrV4_int(char *ifName, int AddressType, T_IPADDRv4 *ipAddr);
static int NIF_getAddrV4_str(char *ifName, int AddressType, char **ipAddr);
static int NIF_getIPAddrV6_int(char *ifName, int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen);
static int NIF_getIPAddrV6_str(char *ifName, char ** ipAddr);
static int NIF_getMacAddr_int(char *ifName, int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6);	
static int NIF_getMacAddr_str(char *ifName, char ** ipAddr);	
static int NIF_getDHCPStatus(char *ifName);
static int NIF_setDHCPStatic(char * ifName,T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr);
static int NIF_setDHCPDynamic(char *ifName);

static int NIF_DBOpen(sqlite3 ** dbInsPtr);
static int NIF_DBClose(sqlite3 * dbInsPtr);
static int NIF_trySQLiteGetTable(char *Query,char ***QueryResult,int * ResultRow,int * ResultCol);
static int NIF_trySQLiteExec(char *Query);
static int NIF_autoIF(char * ifName);

const char * getVer_NIF(void)
{
	return (char *)SDH_NETIF_VERSION;
}


const char INTERFACE_PREFIX[] = 
"#       Do not change this file: automatically generated\n"
"# iface eth0 inet dhcp\n"
"#\thostname dc_00\n"
"# iface eth0 inet static\n"
"#\taddress 192.168.0.1\n"
"#\tnetmask 255.255.254.0\n"
"\n"
"auto lo\n"
"iface lo inet loopback\n"
"\n"
;

int NIF_setDHCPStatic_wired(T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr)
{
	return NIF_setDHCPStatic(WIRED_IF_NAME,ipAddr,netMask,gateWay,DNS1Addr,DNS2Addr);
}
int NIF_setDHCPStatic_wifi(T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr)
{
	return NIF_setDHCPStatic(WIFI_IF_NAME,ipAddr,netMask,gateWay,DNS1Addr,DNS2Addr);
}
static int NIF_setDHCPStatic(char * ifName,T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr)
{
	int fEtherAddrWired=0;		char *EtherAddrWired=0;
	char *EtherAddrWifi=0;
	
	int fIpAddrWifi=0;			char *IpAddrWifi=0;
	int fNetmaskWifi=0;			char *NetmaskWifi=0;
	int fGatewayWifi=0;			char *GatewayWifi=0;
	
	int fIpAddrWired=0;			char *IpAddrWired=0;
	int fNetmaskWired=0;		char *NetmaskWired=0;
	int fGatewayWired=0;		char *GatewayWired=0;
	int fDHCPWifi=0;			int fDHCPWired=0;
	
	FILE *fp;
	
	//printf("!");
	//printf("%x(@%x)\n",EtherAddrWired, &EtherAddrWired);
	if (NIF_getMacAddrFromFile(WIRED_IF_NAME,&EtherAddrWired) == NIF_Status_NotFound) fEtherAddrWired=0; else fEtherAddrWired=1;
	if (NIF_getIpAddrV4FromFile(WIRED_IF_NAME,&IpAddrWired) == NIF_Status_NotFound) fIpAddrWired=0; else fIpAddrWired=1;	
	if (NIF_getNetmaskV4FromFile(WIRED_IF_NAME,&NetmaskWired) == NIF_Status_NotFound) fNetmaskWired=0; else fNetmaskWired=1;			
	if (NIF_getGatewayV4FromFile(WIRED_IF_NAME,&GatewayWired) == NIF_Status_NotFound) fGatewayWired=0; else fGatewayWired=1;	
		
	//if (NIF_getMacAddrFromFile(WIFI_IF_NAME,&EtherAddrWifi) == NIF_Status_NotFound) fEtherAddrWifi=0; else fEtherAddrWifi=1;
	if (NIF_getIpAddrV4FromFile(WIFI_IF_NAME,&IpAddrWifi) == NIF_Status_NotFound) fIpAddrWifi=0; else fIpAddrWifi=1;	
	if (NIF_getNetmaskV4FromFile(WIFI_IF_NAME,&NetmaskWifi) == NIF_Status_NotFound) fNetmaskWifi=0; else fNetmaskWifi=1;			
	if (NIF_getGatewayV4FromFile(WIFI_IF_NAME,&GatewayWifi) == NIF_Status_NotFound) fGatewayWifi=0; else fGatewayWifi=1;		
	
	if (NIF_getDHCPStatus(WIFI_IF_NAME) == NIF_Status_Static) fDHCPWifi=0; else fDHCPWifi=1;
	if (NIF_getDHCPStatus(WIRED_IF_NAME) == NIF_Status_Static) fDHCPWired=0; else fDHCPWired=1;
	
	//printf("WIRED DHCP:%d(%d), WIFIDHCP:%d(%d)\n",fDHCPWired,NIF_getDHCPStatus(WIRED_IF_NAME),fDHCPWifi,NIF_getDHCPStatus(WIFI_IF_NAME));
	
	fp=fopen(INTERFACE_FILENAME,"wt");
	if (fp==NULL)
		return NIF_Status_NotFound;			
		
	fprintf (fp,"%s",INTERFACE_PREFIX);
	fprintf (fp,"auto eth0\n");
	if (strcmp(ifName,WIRED_IF_NAME)==0)
	{
		fprintf (fp,"iface eth0 inet static\n");
		fprintf (fp,"\taddress %d.%d.%d.%d\n",ipAddr->ipAddr[0],ipAddr->ipAddr[1],ipAddr->ipAddr[2],ipAddr->ipAddr[3]);
		fprintf (fp,"\tnetmask %d.%d.%d.%d\n",netMask->ipAddr[0],netMask->ipAddr[1],netMask->ipAddr[2],netMask->ipAddr[3]);
		fprintf (fp,"\tgateway %d.%d.%d.%d\n",gateWay->ipAddr[0],gateWay->ipAddr[1],gateWay->ipAddr[2],gateWay->ipAddr[3]);
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
		fprintf (fp,"\n");
		
				
		fprintf (fp,"auto mlan0\n");
		//printf ("NIF_getDHCPStatus(WIRED_IF_NAME):%d\n",NIF_getDHCPStatus(WIFI_IF_NAME));
		if (fDHCPWifi==0)
		{
			fprintf (fp,"iface mlan0 inet static\n");
			if (fIpAddrWifi == 1)
				fprintf (fp,"\taddress %s\n",IpAddrWifi);
			if (fNetmaskWifi == 1)
				fprintf (fp,"\tnetmask %s\n",NetmaskWifi);
			if (fGatewayWifi == 1)
				fprintf (fp,"\tgateway %s\n",GatewayWifi);
		}
		else
		{
			fprintf (fp,"iface mlan0 inet dhcp\n");
		}
		fprintf (fp,"\n");
		//if (fEtherAddrWifi == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWifi);				
	}
	else if (strcmp(ifName,WIFI_IF_NAME) == 0)
	{
		//printf("WIFI:%d\n",NIF_getDHCPStatus(WIRED_IF_NAME));
		if (fDHCPWired==0)
		{
			fprintf (fp,"iface eth0 inet static\n");
			if (fIpAddrWired == 1)
				fprintf (fp,"\taddress %s\n",IpAddrWired);
			if (fNetmaskWired == 1)
				fprintf (fp,"\tnetmask %s\n",NetmaskWired);
			if (fGatewayWired == 1)
				fprintf (fp,"\tgateway %s\n",GatewayWired);
		}
		else
		{
			fprintf (fp,"iface eth0 inet dhcp\n");
		}
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
			
			
		fprintf (fp,"\n");
		fprintf (fp,"auto mlan0\n");
		fprintf (fp,"iface mlan0 inet static\n");
		fprintf (fp,"\taddress %d.%d.%d.%d\n",ipAddr->ipAddr[0],ipAddr->ipAddr[1],ipAddr->ipAddr[2],ipAddr->ipAddr[3]);
		fprintf (fp,"\tnetmask %d.%d.%d.%d\n",netMask->ipAddr[0],netMask->ipAddr[1],netMask->ipAddr[2],netMask->ipAddr[3]);
		fprintf (fp,"\tgateway %d.%d.%d.%d\n",gateWay->ipAddr[0],gateWay->ipAddr[1],gateWay->ipAddr[2],gateWay->ipAddr[3]);
		fprintf (fp,"\n");
		//if (fEtherAddrWifi == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWifi);			
	}
	else
	{
		//Do Nothing.
		fprintf (fp,"iface eth0 inet dhcp\n");
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
		fprintf (fp,"\n");	
		fprintf (fp,"auto mlan0\n");
		fprintf (fp,"iface mlan0 inet dhcp\n");
		//if (fEtherAddrWired == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
		fprintf (fp,"\n");		
	}
	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(fp);
	
	free(EtherAddrWired); 	//puts("1");
	free(EtherAddrWifi);	//puts("2");
	
	free(IpAddrWifi);free(NetmaskWifi);free(GatewayWifi);free(IpAddrWired);free(NetmaskWired);free(GatewayWired);
	
	NIF_autoIF(ifName);
	
	
	//DNS 처리
	if ( (DNS1Addr == NULL) && (DNS2Addr != NULL) )
	{
		//NullPointer 처리
		printf ("(NIF) ChangeDNS at 1\n");
		DNS1Addr = DNS2Addr;
		DNS2Addr = NULL;
	}
	
	if ( (DNS1Addr!=NULL) && (DNS1Addr->ipAddr_4Byte!=0) )
	{
		//시작
		fp=fopen(DNSSERVER_FILENAME,"wt");
		if (fp==NULL)
			return NIF_Status_NotFound;	
		printf ("(NIF) nameserver.1: %d.%d.%d.%d\n",DNS1Addr->ipAddr[0],DNS1Addr->ipAddr[1],DNS1Addr->ipAddr[2],DNS1Addr->ipAddr[3]);
		fprintf (fp,"nameserver %d.%d.%d.%d\n",DNS1Addr->ipAddr[0],DNS1Addr->ipAddr[1],DNS1Addr->ipAddr[2],DNS1Addr->ipAddr[3]);
		
		if ( (DNS2Addr!=NULL) && (DNS2Addr->ipAddr_4Byte!=0) )
		{
			printf ("(NIF) nameserver.2: %d.%d.%d.%d\n",DNS2Addr->ipAddr[0],DNS2Addr->ipAddr[1],DNS2Addr->ipAddr[2],DNS2Addr->ipAddr[3]);
			fprintf (fp,"nameserver %d.%d.%d.%d\n",DNS2Addr->ipAddr[0],DNS2Addr->ipAddr[1],DNS2Addr->ipAddr[2],DNS2Addr->ipAddr[3]);
		}
		else
			printf ("(NIF) DNS2Addr is 0x%08x or ipAddr==0\n",(unsigned int)DNS2Addr);
		
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
	}
	else
	{
		printf ("(NIF) DNS1Addr is 0x%08x or ipAddr==0\n",(unsigned int)DNS1Addr);
	}
	
	
	return NIF_Status_IfUp;
}


int NIF_setDHCPDynamic_wired(void)
{
	return NIF_setDHCPDynamic(WIRED_IF_NAME);
}	
int NIF_setDHCPDynamic_wifi(void)
{
	return NIF_setDHCPDynamic(WIFI_IF_NAME);
}
int NIF_setDHCPDynamic_all(void)
{
	return NIF_setDHCPDynamic("ALL");
}

static int NIF_setDHCPDynamic(char *ifName)
{
	
	int fEtherAddrWired=0;		char *EtherAddrWired=0;
	//char *EtherAddrWifi=0;
	
	int fIpAddrWifi=0;			char *IpAddrWifi=0;
	int fNetmaskWifi=0;			char *NetmaskWifi=0;
	int fGatewayWifi=0;			char *GatewayWifi=0;
	
	int fIpAddrWired=0;			char *IpAddrWired=0;
	int fNetmaskWired=0;		char *NetmaskWired=0;
	int fGatewayWired=0;		char *GatewayWired=0;
	int fDHCPWifi=0;			int fDHCPWired=0;
	
	FILE *fp;
	
	//printf("%x(@%x)\n",EtherAddrWired, &EtherAddrWired);
	if (NIF_getMacAddrFromFile(WIRED_IF_NAME,&EtherAddrWired) == NIF_Status_NotFound) fEtherAddrWired=0; else fEtherAddrWired=1;
	if (NIF_getIpAddrV4FromFile(WIRED_IF_NAME,&IpAddrWired) == NIF_Status_NotFound) fIpAddrWired=0; else fIpAddrWired=1;	
	if (NIF_getNetmaskV4FromFile(WIRED_IF_NAME,&NetmaskWired) == NIF_Status_NotFound) fNetmaskWired=0; else fNetmaskWired=1;			
	if (NIF_getGatewayV4FromFile(WIRED_IF_NAME,&GatewayWired) == NIF_Status_NotFound) fGatewayWired=0; else fGatewayWired=1;	
		
	//if (NIF_getMacAddrFromFile(WIFI_IF_NAME,&EtherAddrWifi) == NIF_Status_NotFound) fEtherAddrWifi=0; else fEtherAddrWifi=1;
	if (NIF_getIpAddrV4FromFile(WIFI_IF_NAME,&IpAddrWifi) == NIF_Status_NotFound) fIpAddrWifi=0; else fIpAddrWifi=1;	
	if (NIF_getNetmaskV4FromFile(WIFI_IF_NAME,&NetmaskWifi) == NIF_Status_NotFound) fNetmaskWifi=0; else fNetmaskWifi=1;			
	if (NIF_getGatewayV4FromFile(WIFI_IF_NAME,&GatewayWifi) == NIF_Status_NotFound) fGatewayWifi=0; else fGatewayWifi=1;		
	if (NIF_getDHCPStatus(WIFI_IF_NAME) == NIF_Status_Dynamic) fDHCPWifi=1; else fDHCPWifi=0;
	if (NIF_getDHCPStatus(WIRED_IF_NAME) == NIF_Status_Dynamic) fDHCPWired=1; else fDHCPWired=0;	
	
	fp=fopen(INTERFACE_FILENAME,"wt");
	if (fp==NULL)
	{
		if(EtherAddrWired)
		{
			free(EtherAddrWired);
			EtherAddrWired =NULL;
		}
		if(IpAddrWired)
		{
			free(IpAddrWired);
			IpAddrWired =NULL;
		}
		if(NetmaskWifi)
		{
			free(NetmaskWifi);
			NetmaskWifi =NULL;
		}
		if(IpAddrWifi)
		{
			free(IpAddrWifi);
			IpAddrWifi =NULL;
		}
		if(NetmaskWired)
		{
			free(NetmaskWired);
			NetmaskWired =NULL;
		}
		if(GatewayWifi)
		{
			free(GatewayWifi);
			GatewayWifi =NULL;
		}
		if(GatewayWired)
		{
			free(GatewayWired);
			GatewayWired =NULL;
		}
		return NIF_Status_NotFound;
	}
	fprintf (fp,"%s",INTERFACE_PREFIX);
	fprintf (fp,"auto eth0\n");
	if (strcmp(ifName,WIRED_IF_NAME)==0)
	{
		fprintf (fp,"iface eth0 inet dhcp\n");
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
		fprintf (fp,"\n");
		
		fprintf (fp,"auto mlan0\n");
		if (fDHCPWifi==0)
		{
			fprintf (fp,"iface mlan0 inet static\n");
			if (fIpAddrWifi == 1)
				fprintf (fp,"\taddress %s\n",IpAddrWifi);
			if (fNetmaskWifi == 1)
				fprintf (fp,"\tnetmask %s\n",NetmaskWifi);
			if (fGatewayWifi == 1)
				fprintf (fp,"\tgateway %s\n",GatewayWifi);
		}
		else
		{
			fprintf (fp,"iface mlan0 inet dhcp\n");
		}
		
		//if (fEtherAddrWifi == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWifi);		
	}
	else if (strcmp(ifName,WIFI_IF_NAME)==0)
	{
		if (fDHCPWired==0)
		{
			fprintf (fp,"iface eth0 inet static\n");
			if (fIpAddrWired == 1)
				fprintf (fp,"\taddress %s\n",IpAddrWired);
			if (fNetmaskWired == 1)
				fprintf (fp,"\tnetmask %s\n",NetmaskWired);
			if (fGatewayWired == 1)
				fprintf (fp,"\tgateway %s\n",GatewayWired);
		}
		else
		{
			fprintf (fp,"iface eth0 inet dhcp\n");
		}
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);				
		
		fprintf (fp,"\n");
		fprintf (fp,"auto mlan0\n");
		fprintf (fp,"iface mlan0 inet dhcp\n");
		//if (fEtherAddrWifi == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWifi);
		fprintf (fp,"\n");		
	}
	else
	{
		//Do Nothing.
		fprintf (fp,"iface eth0 inet dhcp\n");
		if (fEtherAddrWired == 1)
			fprintf (fp,"\thwaddress ether %s\n",EtherAddrWired);
		fprintf (fp,"\n");	
		
		fprintf (fp,"auto mlan0\n");
		fprintf (fp,"iface mlan0 inet dhcp\n");
		//if (fEtherAddrWifi == 1)
		//	fprintf (fp,"\thwaddress ether %s\n",EtherAddrWifi);
		fprintf (fp,"\n");		
	}
	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(fp);
	
	free(EtherAddrWired); 	//puts("1");
	//free(EtherAddrWifi);	//puts("2");
	
	free(IpAddrWifi);free(NetmaskWifi);free(GatewayWifi);free(IpAddrWired);free(NetmaskWired);free(GatewayWired);
	
	NIF_autoIF(ifName);
	
	return NIF_Status_IfUp;
}

int NIF_getDHCPStatus_wired(void)
{
	return NIF_getDHCPStatus(WIRED_IF_NAME);
}
int NIF_getDHCPStatus_wifi(void)
{
	return NIF_getDHCPStatus(WIFI_IF_NAME);
}

int NIF_getIpAddrV4FromFile(char *ifName, char **IpAddr)
{
	//int ifDHCP=0;
	FILE * fp;
	fp=fopen(INTERFACE_FILENAME,"rt");
	
	if (fp==NULL)
	{
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		return NIF_Status_NotFound;
	}
	else
	{
		char str[MAX_CHAR_PER_LINE];
		int result=0;
		int parsingStg=0;
		memset(str,0,MAX_CHAR_PER_LINE);
		result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		
		while (!feof(fp))
		{
			if (result!=0)
			{
				//printf ("%s",str);
				//파일의 끝이 아니면
				if (str[0] == '#' || str[0] == '\n')
				{
					//주석
				}
				else
				{
					char * token;
					token = str;
					while (1)
					{
						token=strtok(token," \t\n");
						if (token == NULL)
						{
							//printf("!!!\n");
							break;
						}	
						//printf ("token: %s\n",token);
						switch (parsingStg)
						{
							case 0: if(strcmp(token,"auto")==0) parsingStg=1; else parsingStg=0; break;
							case 1: if(strcmp(token,ifName)==0) parsingStg=2; else parsingStg=0; break;
							case 2: if(strcmp(token,"iface")==0) parsingStg=3; else parsingStg=0; break;
							case 3: if(strcmp(token,ifName)==0) parsingStg=4; else parsingStg=0; break;
							case 4: if(strcmp(token,"address")==0) parsingStg=5; break;
							case 5: 
								//printf("%s",token);
								if(fflush(fp) != 0){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
								}
								if(fsync(fp->_fileno) == -1){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
								}
								fclose(fp);
								*IpAddr=calloc(1,strlen(token)+1);
								strcpy(*IpAddr,token);
								return NIF_Status_IfUp;
								
							default: parsingStg=0;
						}
						token=NULL;
					}
				}
			}
			
			result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		}
		//printf("NotFound\n");
		//printf("%x(@%x)\n",macAddr, &macAddr);
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
		return (NIF_Status_NotFound);
	}		
}

int NIF_getGatewayV4FromFile(char *ifName, char **IpAddr)
{
	//int ifDHCP=0;
	FILE * fp;
	fp=fopen(INTERFACE_FILENAME,"rt");
	
	if (fp==NULL)
	{
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		return NIF_Status_NotFound;
	}
	else
	{
		char str[MAX_CHAR_PER_LINE];
		int result=0;
		int parsingStg=0;
		memset(str,0,MAX_CHAR_PER_LINE);
		result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		
		
		while (!feof(fp))
		{
			if (result!=0)
			{
				//printf ("%s",str);
				//파일의 끝이 아니면
				if (str[0] == '#' || str[0] == '\n')
				{
					//주석
				}
				else
				{
					char * token;
					token = str;
					while (1)
					{
						token=strtok(token," \t\n");
						if (token == NULL)
						{
							//printf("!!!\n");
							break;
						}	
						//printf ("token: %s\n",token);
						switch (parsingStg)
						{
							case 0: if(strcmp(token,"auto")==0) parsingStg=1; else parsingStg=0; break;
							case 1: if(strcmp(token,ifName)==0) parsingStg=2; else parsingStg=0; break;
							case 2: if(strcmp(token,"iface")==0) parsingStg=3; else parsingStg=0; break;
							case 3: if(strcmp(token,ifName)==0) parsingStg=4; else parsingStg=0; break;
							case 4: if(strcmp(token,"gateway")==0) parsingStg=5; break;
							case 5: 
								//printf("%s",token);
								if(fflush(fp) != 0){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
								}
								if(fsync(fp->_fileno) == -1){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
								}
								fclose(fp);
								*IpAddr=calloc(1,strlen(token)+1);
								strcpy(*IpAddr,token);
								return NIF_Status_IfUp;
								
							default: parsingStg=0;
						}
						token=NULL;
					}
				}
			}
			
			result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		}
		//printf("NotFound\n");
		//printf("%x(@%x)\n",macAddr, &macAddr);
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
		return (NIF_Status_NotFound);
	}		
}

int NIF_getNetmaskV4FromFile(char *ifName, char **IpAddr)
{
	//int ifDHCP=0;
	FILE * fp;
	fp=fopen(INTERFACE_FILENAME,"rt");
	
	if (fp==NULL)
	{
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		return NIF_Status_NotFound;
	}
	else
	{
		char str[MAX_CHAR_PER_LINE];
		int result=0;
		int parsingStg=0;
		memset(str,0,MAX_CHAR_PER_LINE);
		result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		
		
		while (!feof(fp))
		{
			if (result!=0)
			{
				//printf ("%s",str);
				//파일의 끝이 아니면
				if (str[0] == '#' || str[0] == '\n')
				{
					//주석
				}
				else
				{
					char * token;
					token = str;
					while (1)
					{
						token=strtok(token," \t\n");
						if (token == NULL)
						{
							//printf("!!!\n");
							break;
						}	
						//printf ("token: %s\n",token);
						switch (parsingStg)
						{
							case 0: if(strcmp(token,"auto")==0) parsingStg=1; else parsingStg=0; break;
							case 1: if(strcmp(token,ifName)==0) parsingStg=2; else parsingStg=0; break;
							case 2: if(strcmp(token,"iface")==0) parsingStg=3; else parsingStg=0; break;
							case 3: if(strcmp(token,ifName)==0) parsingStg=4; else parsingStg=0; break;
							case 4: if(strcmp(token,"netmask")==0) parsingStg=5; break;
							case 5: 
								//printf("%s",token);
								if(fflush(fp) != 0){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
								}
								if(fsync(fp->_fileno) == -1){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
								}	
								fclose(fp);
								*IpAddr=calloc(1,strlen(token)+1);
								strcpy(*IpAddr,token);
								return NIF_Status_IfUp;
								
							default: parsingStg=0;
						}
						token=NULL;
					}
				}
			}
			
			result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		}
		//printf("NotFound\n");
		//printf("%x(@%x)\n",macAddr, &macAddr);
		*IpAddr=calloc(1,strlen("None")+1);
		strcpy(*IpAddr,"None");
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
		return (NIF_Status_NotFound);
	}		
}

int NIF_getMacAddrFromFile(char *ifName, char **macAddr)
{
	//int ifDHCP=0;
	FILE * fp;
	fp=fopen(INTERFACE_FILENAME,"rt");
	
	if (fp==NULL)
	{
		*macAddr=calloc(1,strlen("None")+1);
		strcpy(*macAddr,"None");
		return NIF_Status_NotFound;
	}
	else
	{
		char str[MAX_CHAR_PER_LINE];
		int result=0;
		int parsingStg=0;
		memset(str,0,MAX_CHAR_PER_LINE);
		result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		
		
		while (!feof(fp))
		{
			if (result!=0)
			{
				//printf ("%s",str);
				//파일의 끝이 아니면
				if (str[0] == '#' || str[0] == '\n')
				{
					//주석
				}
				else
				{
					char * token;
					token = str;
					while (1)
					{
						token=strtok(token," \t\n");
						if (token == NULL)
						{
							//printf("!!!\n");
							break;
						}	
						//printf ("token: %s\n",token);
						switch (parsingStg)
						{
							case 0: if(strcmp(token,"auto")==0) parsingStg=1; else parsingStg=0; break;
							case 1: if(strcmp(token,ifName)==0) parsingStg=2; else parsingStg=0; break;
							case 2: if(strcmp(token,"iface")==0) parsingStg=3; else parsingStg=0; break;
							case 3: if(strcmp(token,ifName)==0) parsingStg=4; else parsingStg=0; break;
							case 4: if(strcmp(token,"hwaddress")==0) parsingStg=5; break;
							case 5: if(strcmp(token,"ether")==0) parsingStg=6; break;
							case 6: 
								//printf("%s",token);
								if(fflush(fp) != 0){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
								}
								if(fsync(fp->_fileno) == -1){
									DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
								}
								fclose(fp);
								*macAddr=calloc(1,strlen(token)+1);
								strcpy(*macAddr,token);
								return NIF_Status_IfUp;
								
							default: parsingStg=0;
						}
						token=NULL;
					}
				}
			}
			
			result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		}
		//printf("NotFound\n");
		//printf("%x(@%x)\n",macAddr, &macAddr);
		*macAddr=calloc(1,strlen("None")+1);
		strcpy(*macAddr,"None");
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
		return (NIF_Status_NotFound);
	}	
}


static int NIF_getDHCPStatus(char *ifName)
{
	//int ifDHCP=0;
	FILE * fp;
	fp=fopen(INTERFACE_FILENAME,"rt");
	//printf ("fp:%d\n",fp);
	if (fp==NULL)
	{
		//printf ("1");
		return NIF_Status_NotFound;
	}
	else
	{
		char str[MAX_CHAR_PER_LINE];
		int result=0;
		int parsingStg=0;
		memset(str,0,MAX_CHAR_PER_LINE);
		result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		
		
		while (!feof(fp))
		{
			if (result!=0)
			{
				//printf ("%s",str);
				//파일의 끝이 아니면
				if (str[0] == '#' || str[0] == '\n')
				{
					//주석
				}
				else
				{
					char * token;
					token = str;
					while (1)
					{
						token=strtok(token," \t\n");
						if (token == NULL)
						{
							//printf("!!!\n");
							break;
						}	
						//printf ("token[%d]: %s\n",parsingStg,token);
						switch (parsingStg)
						{
							case 0: if(strcmp(token,"auto")==0) parsingStg=1; else parsingStg=0; break;
							case 1: if(strcmp(token,ifName)==0) parsingStg=2; else parsingStg=0; break;
							case 2: if(strcmp(token,"iface")==0) parsingStg=3; else parsingStg=0; break;
							case 3: if(strcmp(token,ifName)==0) parsingStg=4; else parsingStg=0; break;
							case 4: if(strcmp(token,"inet")==0) parsingStg=5; else parsingStg=0; break;
							case 5:
								if(strcmp(token,"static")==0) 
								{
									if(fflush(fp) != 0){
										DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
									}
									if(fsync(fp->_fileno) == -1){
										DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
									}
									fclose(fp);
									parsingStg=1;
									return (NIF_Status_Static);
								}
								else if (strcmp(token,"dhcp")==0)
								{
									if(fflush(fp) != 0){
										DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
									}
									if(fsync(fp->_fileno) == -1){
										DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
									}
									fclose(fp);
									parsingStg=0;
									return (NIF_Status_Dynamic);
								}
								else parsingStg=5;
									break;
								
							default: parsingStg=0;
						}
						token=NULL;
					}
				}
			}
			
			result=(int)fgets(str,MAX_CHAR_PER_LINE,fp);
		}
		if(fflush(fp) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fp->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fp);
		return (NIF_Status_Unknown);
	}
	
}

//Check interface
int NIF_getStatus_wired(void)
{
	return NIF_getStatus(WIRED_IF_NAME);
}
int NIF_getStatus_wifi(void)
{
	return NIF_getStatus(WIFI_IF_NAME);
}
static int NIF_getStatus(char *ifName)
{
	int sockfd=0;
	if (ifName == NULL)
	{
		//Integrity Check or... Die.
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}	
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd > 0)
	{
		struct ifreq ifr;
		int result;
		strncpy(ifr.ifr_name, ifName, 16);
		result=ioctl(sockfd, SIOCGIFFLAGS, &ifr);
		if (result < 0)
		{
			close(sockfd);
			return NIF_Status_NotFound;
		} 
		else
		{
			if (ifr.ifr_ifru.ifru_flags&IFF_UP)
			{
				close(sockfd);
				return NIF_Status_IfUp;
			}
			else
			{
				close(sockfd);
				return NIF_Status_IfDown;
			}
		}
		
	}
	else
	{
		return NIF_Status_NotFound;
	}
}

int NIF_autoIF_Wired(void)
{
	return NIF_autoIF(WIRED_IF_NAME);
}
int NIF_autoIF_Wifi(void)
{
	return NIF_autoIF(WIFI_IF_NAME);
}
static int NIF_autoIF(char * ifName)
{
	char cmd[256];
	memset(cmd,0x0,200);
	snprintf(cmd,200,"kill `cat /var/run/udhcpc.%s.pid` 2>/dev/null",ifName);
	system(cmd);
			
	if (strcmp(ifName,WIRED_IF_NAME) == 0){
		//system("ifconfig "WIRED_IF_NAME" down");
		system("ifdown -fv "WIRED_IF_NAME" ");
	}
			
	if (strcmp(ifName,WIFI_IF_NAME) == 0){
		system("iwconfig "WIFI_IF_NAME" power off");
	}
	memset(cmd,0x0,200);
	snprintf(cmd,200,"ifup -fv %s",ifName);
	system(cmd);
	return NIF_Status_IfUp;
}


//IPv4 related functions
int NIF_getIPAddrV4_int_wired(T_IPADDRv4 *ipAddr)
{
	return NIF_getAddrV4_int(WIRED_IF_NAME,ENUM_IF_IPADDR,ipAddr);
}
int NIF_getIPAddrV4_int_wifi(T_IPADDRv4 *ipAddr)
{
	return NIF_getAddrV4_int(WIFI_IF_NAME,ENUM_IF_IPADDR,ipAddr);
}
static int NIF_getAddrV4_int(char *ifName, int AddressType, T_IPADDRv4 *ipAddr)
{
	int sockfd=0;
	if ((ifName == NULL) || (ipAddr == NULL))
	{
		//Integrity Check or... Die.
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd > 0)
	{
		struct ifreq ifr;
		int result;
		strncpy(ifr.ifr_name, ifName, 16);
		switch(AddressType)
		{
			case ENUM_IF_NETMASK:
				result=ioctl(sockfd, SIOCGIFNETMASK, &ifr);
			break;
			case ENUM_IF_IPADDR:
			default:
				result=ioctl(sockfd, SIOCGIFADDR, &ifr);
			break;
		}
		if (result < 0)
		{
			close(sockfd);
			ipAddr->ipAddr_4Byte=0;
			return NIF_Status_NotFound;
		}
		else
		{
			//printf ("\tIP Address: %s\n",inet_ntoa( ((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));
			//printf("%x\n", ntohl(ADDR.s_addr) );
			close(sockfd);
			ipAddr->ipAddr_4Byte=ADDR.s_addr;
			/**ipAddr1 = ntohl(ADDR.s_addr) / 0x1000000;
			*ipAddr2 = ( ntohl(ADDR.s_addr) / 0x10000 )%0x100;
			*ipAddr3 = ( ntohl(ADDR.s_addr) / 0x100 )%0x100;
			*ipAddr4 = ( ntohl(ADDR.s_addr) / 0x1 )%0x100;
			*/
			return NIF_Status_IfUp;
		}
	}	
	else
	{
		//Not Close
		ipAddr->ipAddr_4Byte=0;
		return NIF_Status_NotFound;
	}
}

int NIF_getIPAddrV4_str_wired(char ** ipAddr)
{
	return NIF_getAddrV4_str(WIRED_IF_NAME,ENUM_IF_IPADDR,ipAddr);
}
int NIF_getIPAddrV4_str_wifi(char ** ipAddr)
{
	return NIF_getAddrV4_str(WIFI_IF_NAME,ENUM_IF_IPADDR,ipAddr);
}
static int NIF_getAddrV4_str(char *ifName, int AddressType, char **ipAddr)
{
	//printf ("ipAddr:0x%08x\n",ipAddr);
	int sockfd=0;
	if ((ifName == NULL) || (ipAddr == NULL))
	{
		//Integrity Check or... Die.
		//printf("1");
		//*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
		//strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);	
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd > 0)
	{
		struct ifreq ifr;
		int result;
		strncpy(ifr.ifr_name, ifName, 16);
		switch (AddressType)
		{
			case ENUM_IF_NETMASK:
				result=ioctl(sockfd, SIOCGIFNETMASK, &ifr);
			break;
			case ENUM_IF_IPADDR:
			default:
				result=ioctl(sockfd, SIOCGIFADDR, &ifr);
			break;
		}
		if (result < 0)
		{
			close(sockfd);
			*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
			strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);
			return NIF_Status_NotFound;
		}
		else
		{
			//printf ("\tIP Address: %s\n",inet_ntoa( ((struct sockaddr_in *)&(ifr.ifr_addr))->sin_addr));
			//printf("%x\n", ntohl(ADDR.s_addr) );
			/*
			*ipAddr1 = ntohl(ADDR.s_addr) / 0x1000000;
			*ipAddr2 = ( ntohl(ADDR.s_addr) / 0x10000 )%0x100;
			*ipAddr3 = ( ntohl(ADDR.s_addr) / 0x100 )%0x100;
			*ipAddr4 = ( ntohl(ADDR.s_addr) / 0x1 )%0x100;
			*/
			close(sockfd);
			//printf ("2");
			*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
			snprintf(*ipAddr,IPADDR_STR_SIZE,"%d.%d.%d.%d", ntohl(ADDR.s_addr) / 0x1000000,( ntohl(ADDR.s_addr) / 0x10000 )%0x100,( ntohl(ADDR.s_addr) / 0x100 )%0x100, ( ntohl(ADDR.s_addr) / 0x1 )%0x100);
			//printf ("%s\n",ipAddr);
			return NIF_Status_IfUp;
		}
	}	
	else
	{
		//Not close
		*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
		strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);
		return NIF_Status_NotFound;
	}
}

//IPv4 Netmask Address related functions
int NIF_getNetmaskV4_int_wired(T_IPADDRv4 *ipAddr)
{
	return NIF_getAddrV4_int(WIRED_IF_NAME,ENUM_IF_NETMASK,ipAddr);
}
int NIF_getNetmaskV4_int_wifi(T_IPADDRv4 *ipAddr)
{
	return NIF_getAddrV4_int(WIFI_IF_NAME,ENUM_IF_NETMASK,ipAddr);
}
	
int NIF_getNetmaskV4_str_wired(char **ipAddr)
{
	return NIF_getAddrV4_str(WIRED_IF_NAME,ENUM_IF_NETMASK,ipAddr);
}
int NIF_getNetmaskV4_str_wifi(char **ipAddr)
{
	return NIF_getAddrV4_str(WIFI_IF_NAME,ENUM_IF_NETMASK,ipAddr);
}
	
//IPv4 Gateway Address related functions
int NIF_getGatewayV4_int_wired(T_IPADDRv4 *ipAddr)
{
	//return NIF_getAddrV4_int(WIRED_IF_NAME,ENUM_IF_GATEWAY,ipAddr);
	char *GatewayWired=0;
	if (NIF_getGatewayV4FromFile(WIRED_IF_NAME,&GatewayWired) == NIF_Status_NotFound) 
	{
		free(GatewayWired);
		return NIF_Status_NotFound; 
	}
	else
	{

		//printf("Gateway:%s\n",GatewayWired);
		ipAddr->ipAddr[0]=atoi(strtok(GatewayWired,"."));
		ipAddr->ipAddr[1]=atoi(strtok(NULL,"."));
		ipAddr->ipAddr[2]=atoi(strtok(NULL,"."));
		ipAddr->ipAddr[3]=atoi(strtok(NULL,"."));
		free(GatewayWired);
		return NIF_Status_IfUp;
	}
}
int NIF_getGatewayV4_int_wifi(T_IPADDRv4 *ipAddr)
{
	//return NIF_getAddrV4_int(WIRED_IF_NAME,ENUM_IF_GATEWAY,ipAddr);
	char *GatewayWifi=0;
	if (NIF_getGatewayV4FromFile(WIFI_IF_NAME,&GatewayWifi) == NIF_Status_NotFound) 
	{
		free(GatewayWifi);
		return NIF_Status_NotFound; 
	}
	else
	{

		//printf("Gateway:%s\n",GatewayWifi);
		ipAddr->ipAddr[0]=atoi(strtok(GatewayWifi,"."));
		ipAddr->ipAddr[1]=atoi(strtok(NULL,"."));
		ipAddr->ipAddr[2]=atoi(strtok(NULL,"."));
		ipAddr->ipAddr[3]=atoi(strtok(NULL,"."));
		free(GatewayWifi);
		return NIF_Status_IfUp;
	}
}
	
int NIF_getGatewayV4_str_wired(char **ipAddr)
{
	return (NIF_getGatewayV4FromFile(WIRED_IF_NAME,ipAddr));
}
int NIF_getGatewayV4_str_wifi(char **ipAddr)
{
	return (NIF_getGatewayV4FromFile(WIFI_IF_NAME,ipAddr));
}






//IPv6 related functions
int NIF_getIPAddrV6_int_wired(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen)
{
	return NIF_getIPAddrV6_int(WIRED_IF_NAME,ipAddr1,ipAddr2,ipAddr3,ipAddr4,ipAddr5,ipAddr6,ipAddr7,ipAddr8,plen);
}
int NIF_getIPAddrV6_int_wifi(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen)
{
	return NIF_getIPAddrV6_int(WIFI_IF_NAME,ipAddr1,ipAddr2,ipAddr3,ipAddr4,ipAddr5,ipAddr6,ipAddr7,ipAddr8,plen);
}
static int NIF_getIPAddrV6_int(char *ifName, int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4,int *ipAddr5,int *ipAddr6,int *ipAddr7,int *ipAddr8,int *plen)
{
	FILE *fpInet6;
	
	if ( (ifName == NULL) || (ipAddr1 == NULL) || (ipAddr2 == NULL) || (ipAddr3 == NULL) || (ipAddr4 == NULL) || 
		 (ipAddr5 == NULL) || (ipAddr6 == NULL) || (ipAddr7 == NULL) || (ipAddr8 == NULL) || (plen == NULL) )
	{
		//*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6=*ipAddr7=*ipAddr8=*plen=0;
		return NIF_Status_NotFound;
	}
	fpInet6 = fopen("/proc/net/if_inet6","rt");
	if (fpInet6 != NULL)
	{
		char devname[20];
		char addr6p[8][5];
		int tplen, tscope, tdad_status, tif_idx;
		
		while (fscanf(fpInet6, "%4s%4s%4s%4s%4s%4s%4s%4s %08x %02x %02x %02x %20s\n",addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],addr6p[5], addr6p[6], addr6p[7], &tif_idx, &tplen, &tscope,&tdad_status, devname) != EOF)
		{
			//printf ("%s\n",devname);
			if (strcmp(devname,ifName) == 0)
			{
				fclose(fpInet6);
				*ipAddr1=HexToInt(addr6p[0]);
				*ipAddr2=HexToInt(addr6p[1]);
				*ipAddr3=HexToInt(addr6p[2]);
				*ipAddr4=HexToInt(addr6p[3]);
				*ipAddr5=HexToInt(addr6p[4]);
				*ipAddr6=HexToInt(addr6p[5]);
				*ipAddr7=HexToInt(addr6p[6]);
				*ipAddr8=HexToInt(addr6p[7]);
				*plen=tplen;
				
				//printf("%s:%s:%s:%s:%s:%s:%s:%s/%d\n",addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],addr6p[5], addr6p[6], addr6p[7], tplen);
				return NIF_Status_IfUp;
			}
		}
		if(fflush(fpInet6) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fpInet6->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fpInet6);
		*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6=*ipAddr7=*ipAddr8=*plen=0;
		return NIF_Status_NotFound;
	}	
	else
	{
		*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6=*ipAddr7=*ipAddr8=*plen=0;
		return NIF_Status_NotFound;
	}	
}

static int HexToInt(char *Str)
{
	int i,result;
	i=0;result=0;
	for (i=0;i<4;i++)
	{
		result=result<<4;
		//printf ("%c",Str[i]);
		if (Str[i]>='A' && Str[i]<='F')
		{
			//printf("%d\n",Str[i]-'A'+0x10);
			result|=Str[i]-'A'+0xA;
		}
		else if (Str[i]>='a' && Str[i]<='f')
		{
			//printf("%x\n",Str[i]-'a'+0x10);
			result|=Str[i]-'a'+0xa;
		}
		else if (Str[i]>='0' && Str[i]<='9')
		{
			//printf("%x\n",Str[i]-'0'+0x0);
			result|=Str[i]-'0'+0x0;
		}
		else
			return 0;
	}
	//printf("-(%x)-",result);
	return result;
}


int NIF_getIPAddrV6_str_wired(char ** ipAddr)
{
	return NIF_getIPAddrV6_str(WIRED_IF_NAME,ipAddr);
}
int NIF_getIPAddrV6_str_wifi(char ** ipAddr)
{
	return NIF_getIPAddrV6_str(WIFI_IF_NAME,ipAddr);
}
static int NIF_getIPAddrV6_str(char *ifName, char ** ipAddr)
{
	FILE *fpInet6;
	if ((ifName == NULL) || (ipAddr == NULL))
	{
		//Integrity Check or... Die.
		//printf("1");
		//*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
		//strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);	
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}	
	
	fpInet6 = fopen("/proc/net/if_inet6","rt");
	if (fpInet6 != NULL)
	{
		char devname[20];
		char addr6p[8][5];
		int tplen, tscope, tdad_status, tif_idx;
		
		while (fscanf(fpInet6, "%4s%4s%4s%4s%4s%4s%4s%4s %08x %02x %02x %02x %20s\n",addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],addr6p[5], addr6p[6], addr6p[7], &tif_idx, &tplen, &tscope,&tdad_status, devname) != EOF)
		{
			//printf ("%s\n",devname);
			if (strcmp(devname,ifName) == 0)
			{
				fclose(fpInet6);
				*ipAddr=(char*)calloc(1,IPADDRV6_STR_SIZE);
				snprintf(*ipAddr,IPADDRV6_STR_SIZE,"%s:%s:%s:%s:%s:%s:%s:%s/%d",addr6p[0], addr6p[1], addr6p[2], addr6p[3], addr6p[4],addr6p[5], addr6p[6], addr6p[7], tplen);
				return NIF_Status_IfUp;
			}
		}
		if(fflush(fpInet6) != 0){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
		}
		if(fsync(fpInet6->_fileno) == -1){
			DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
		}
		fclose(fpInet6);
		*ipAddr=(char*)calloc(1,IPADDRV6_STR_SIZE);
		strncpy(*ipAddr,"Interface not found",IPADDRV6_STR_SIZE);
		return NIF_Status_NotFound;
	}	
	else
	{
		*ipAddr=(char*)calloc(1,IPADDRV6_STR_SIZE);
		strncpy(*ipAddr,"Interface not found",IPADDRV6_STR_SIZE);
		return NIF_Status_NotFound;
	}		
}

//Ethernet Address Functions
int NIF_getMacAddr_int_wired(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6)
{
	return NIF_getMacAddr_int(WIRED_IF_NAME,ipAddr1,ipAddr2,ipAddr3,ipAddr4,ipAddr5,ipAddr6);
}
int NIF_getMacAddr_int_wifi(int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6)
{
	return NIF_getMacAddr_int(WIFI_IF_NAME,ipAddr1,ipAddr2,ipAddr3,ipAddr4,ipAddr5,ipAddr6);
}
static int NIF_getMacAddr_int(char *ifName, int *ipAddr1,int *ipAddr2,int *ipAddr3,int *ipAddr4, int *ipAddr5, int *ipAddr6)
{
	int sockfd=0;
	if ((ifName == NULL) || (ipAddr1 == NULL) || (ipAddr2 == NULL) || (ipAddr3 == NULL) || (ipAddr4 == NULL)|| (ipAddr5 == NULL) || (ipAddr6 == NULL))
	{
		//Integrity Check or... Die.
		//*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6=0;
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}	
	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd > 0)
	{
		struct ifreq ifr;
		int result;
		strncpy(ifr.ifr_name, ifName, 16);
		result=ioctl(sockfd, SIOCGIFHWADDR, &ifr);
		if (result < 0)
		{
			close(sockfd);
			*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6=0;
			return NIF_Status_NotFound;
		} 
		else
		{
			*ipAddr1=ifr.ifr_addr.sa_data[0];
			*ipAddr2=ifr.ifr_addr.sa_data[1];
			*ipAddr3=ifr.ifr_addr.sa_data[2];
			*ipAddr4=ifr.ifr_addr.sa_data[3];
			*ipAddr5=ifr.ifr_addr.sa_data[4];
			*ipAddr6=ifr.ifr_addr.sa_data[5];
			close(sockfd);
			return NIF_Status_IfUp;
		}
		
	}
	else
	{
		*ipAddr1=*ipAddr2=*ipAddr3=*ipAddr4=*ipAddr5=*ipAddr6;
		return NIF_Status_NotFound;
	}	
}
int NIF_getMacAddr_str_wired(char ** ipAddr)
{
	return NIF_getMacAddr_str(WIRED_IF_NAME,ipAddr);
}
int NIF_getMacAddr_str_wifi(char ** ipAddr)
{
	return NIF_getMacAddr_str(WIFI_IF_NAME,ipAddr);
}
static int NIF_getMacAddr_str(char *ifName, char ** ipAddr)
{
	if ((ifName == NULL) || (ipAddr == NULL))
	{
		//Integrity Check or... Die.
		//printf("1");
		//*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
		//strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);	
		printf ("FATAL!!!!!!!\n");
		return NIF_Status_NotFound;
	}
	else 
	{
		int a,b,c,d,e,f;
		a=b=c=d=e=f=0;
		if (NIF_getMacAddr_int(ifName,&a,&b,&c,&d,&e,&f)!=NIF_Status_NotFound)
		{
			*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
			snprintf(*ipAddr,IPADDR_STR_SIZE,"%02X:%02X:%02X:%02X:%02X:%02X",a%0x100,b%0x100,c%0x100,d%0x100,e%0x100,f%0x100);
			return NIF_Status_IfUp;
		}
		else
		{
			*ipAddr=(char*)calloc(1,IPADDR_STR_SIZE);
			strncpy(*ipAddr,"Interface not found",IPADDR_STR_SIZE);
			return NIF_Status_NotFound;
		}
	}	
}



int NIF_findSSID(int * NumberOfSSID, T_SSID ** FoundAPList, int minimumQuality)
{
	#define QUALIFY_KEY  ( (strcmp(tmpMode,"Master") == 0)  && (tmpQuality > minimumQuality ) && (ifHiddenSSID == 0) )
	FILE * fp;
	//printf ("(NIF) NotAlloc 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
	//iwlist mlan0 scanning
	fp=popen("iwlist mlan0 scanning","r");
	DBG_PRINTF_SDH_DEBUG("iwlist mlan0 scanning");
	*FoundAPList = NULL;
	char buf[2048];
	//printf ("(2) 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
	*NumberOfSSID = 0;
	int UnqualifiedSSID = 0;
	//printf ("%d\n",*NumberOfSSID);
	if (fp == NULL)
	{
		DBG_PRINTF_SDH_ERR("pipe open failed[Not Found Case 1]");
		//printf ("Not Found Case 1\n");
		return NIF_Status_NotFound;
	}
	else
	{
		if (gWifiModuleType == SDH_NET_WIFI_MODULE_MARVELL) {
			//////////
			char tmplinebuf[512]; memset(tmplinebuf,0,sizeof(tmplinebuf));
			char tmpHWAddr[64]; memset(tmpHWAddr,0,sizeof(tmpHWAddr));
			char tmpESSID[64]; memset(tmpESSID,0,sizeof(tmpESSID));
			char tmpMode[16]; memset(tmpMode,0,sizeof(tmpMode));
			int tmpEncryptionType=0; int tmpAuthType=0;
			int tmpGroupwiseCipher=0; int tmpPairwiseCipher=0; int tmpAuthSuit=0;
			int ifHiddenSSID = 0; int tmpQuality = 0; int tmpPreAuthSupport = 0;
			char tmpUnknownKeyOption[512]; memset(tmpUnknownKeyOption,0,sizeof(tmpUnknownKeyOption));

			// UnqualifiedSSID -> ++ when found
			// SSID -> ++ when parsed
			if(fgets(tmplinebuf,sizeof(tmplinebuf),fp) != NULL){
				DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,tmplinebuf);
			}
			else{
				DBG_PRINTF_SDH_ERR("[%s]%d:First String returns NULL!pclose(%d)\n",__func__,__LINE__,pclose(fp));
				system("chmod 777 /sbin/iwlist; ls -al /sbin/iwlist");
				return NIF_Status_NotFound;
			}

			while ( !feof(fp) )
			{
				if ( strlen (tmplinebuf) > 0 )
					if ( tmplinebuf[strlen(tmplinebuf)-1] == 0x0A )
						tmplinebuf[strlen(tmplinebuf)-1] = 0x0;
				//printf ("strlen:%d, %s\n",strlen(tmplinebuf),tmplinebuf);
				if (tmplinebuf[0] == 'r')
				{
					//Do Nothing
				}
				else if ( (tmplinebuf[0] == 0x20) && (tmplinebuf[10] != 0x20) )
				{
					//Finalize Before ParsedArg.
					if (UnqualifiedSSID != 0)
					{
						//If this is not the first, parsing
						if ( QUALIFY_KEY )
						{
							#if 0	//Only for Debug Purpose
							sprintf (buf, "\n");
							print_log(buf);
							sprintf (buf, "Parsing infomation -------------------------------------------------------------");
							print_log(buf);
							sprintf (buf, "HWAddr: %s, SSID: %s, Mode: %s",tmpHWAddr, tmpESSID,tmpMode);
							print_log(buf);
							sprintf (buf, "EncType:%d, AuthType:%d[%d], GCipher:%d, PCipher:%d, A.Suit:%d",
									tmpEncryptionType,tmpAuthType,tmpAuthType&(~NIF_AUTH_UNKNOWN),tmpGroupwiseCipher,tmpPairwiseCipher,tmpAuthSuit);
							print_log(buf);
							sprintf (buf, "Hidden?:%d, quality:%d, PreAuthSupport:%d",ifHiddenSSID,tmpQuality,tmpPreAuthSupport);
							print_log(buf);
							sprintf (buf, "EtcOption:%s",tmpUnknownKeyOption);
							print_log(buf);
							sprintf (buf, "------------------------------------------------------------- Parsing infomation\n");
							print_log(buf);
							#endif

							if ( *NumberOfSSID == 0)
							{
								*FoundAPList=calloc(sizeof(T_SSID),1);
							//	printf ("(1) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
							//		*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID) -1,FoundAPList,sizeof(T_SSID));
							} else
							{
								*FoundAPList=realloc(*FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
								//printf ("(2) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
								//	*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID)*((*NumberOfSSID)+1) -1,FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
								//printf ("(3) Clean         0x%08X -> 0x%08X\n",
								//	(unsigned int)(*FoundAPList)+(sizeof(T_SSID)*(*NumberOfSSID)), (unsigned int)(*FoundAPList)+(sizeof(T_SSID)*((*NumberOfSSID)+1)) -1);
								memset((void*)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID))),0,sizeof(T_SSID));
								//printf ("(4) 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
							}
							T_SSID * CurrentItem = (T_SSID *)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID)));
							//printf ("CurrentItem 0x%08x at 0x%08x\n",CurrentItem, &CurrentItem);

							CurrentItem->name=calloc(strlen(tmpESSID)+1,1); strcpy(CurrentItem->name,tmpESSID);
							CurrentItem->hwAddr=calloc(strlen(tmpHWAddr)+1,1); strcpy(CurrentItem->hwAddr,tmpHWAddr);
							CurrentItem->radioQuality=tmpQuality;
							CurrentItem->unknownOption=calloc(strlen(tmpUnknownKeyOption)+1,1); strcpy(CurrentItem->unknownOption,tmpUnknownKeyOption);
							CurrentItem->ifHiddenSSID=ifHiddenSSID;
							CurrentItem->groupCipher=tmpGroupwiseCipher;
							CurrentItem->pairCipher=tmpPairwiseCipher;
							CurrentItem->authSuite=tmpAuthSuit;
							CurrentItem->ifSupportPreAuth=tmpPreAuthSupport;

							sprintf(buf, "sizeof ssid in module: %d", sizeof(T_SSID));
							//print_log(buf);

							//Temporary
							tmpAuthType = tmpAuthType &(~NIF_AUTH_UNKNOWN);
							//Check AuthType.
							if (tmpEncryptionType == 0)		//Not use secure
							{
								CurrentItem->authenticationType = NIF_AUTH_NOSECURE;
							}
							else
							{
								if (tmpAuthType == NIF_AUTH_NOSECURE)
									CurrentItem->authenticationType = NIF_AUTH_WEP;
								else
									CurrentItem->authenticationType = tmpAuthType;
							}

							(*NumberOfSSID)++;
						}
						//Initialize for NextParsing
						memset(tmpHWAddr,0,sizeof(tmpHWAddr));
						memset(tmpESSID,0,sizeof(tmpESSID));
						memset(tmpMode,0,sizeof(tmpMode));
						tmpEncryptionType = 0; tmpAuthType = 0;
						tmpGroupwiseCipher=0; tmpPairwiseCipher=0; tmpAuthSuit=0;
						ifHiddenSSID = 0; tmpQuality = 0; tmpPreAuthSupport = 0;
						memset(tmpUnknownKeyOption,0,sizeof(tmpUnknownKeyOption));
					}


					//putchar('.');
					//Prepare Next ParsedArg -> get EtherMacAddr
					char * addressStartPtr = NULL;
					strtok(&tmplinebuf[10],"-"); addressStartPtr = strtok(NULL,"-");
					//printf("0x%02X ",*(char*)(addressStartPtr+(strlen(addressStartPtr)-1)));
					strcpy(tmpHWAddr,addressStartPtr+10);
					UnqualifiedSSID++;
					//printf("%s\n",tmpHWAddr);
				}
				else if ( (tmplinebuf[0] == 0x20) && (tmplinebuf[10] == 0x20) && (tmplinebuf[20] != 0x20) )
				{
					//SubParameter;
					char * paramKindPtr = NULL;
					char * paramStartPtr = NULL;
					//printf ("%s\n",&tmplinebuf[20]);

					#ifdef OLD_RT3070_DRV
						paramKindPtr = strtok(&tmplinebuf[20],":");
						paramStartPtr = strtok(NULL,":");
					#else
						paramKindPtr = strtok(&tmplinebuf[20],":=");
						paramStartPtr = strtok(NULL,":=");
					#endif


					if (strcmp(paramKindPtr,"ESSID") == 0)
					{
						//printf("%s -> ",paramStartPtr);
						//ESSID
						paramStartPtr++;
						if (strlen(paramStartPtr) > 0)
							if ( *(char*)(paramStartPtr+(strlen(paramStartPtr)-1)) == '"' )
								*(char*)(paramStartPtr+(strlen(paramStartPtr)-1)) = 0;
						//printf ("**********************%s\n",paramStartPtr);
						if ( (paramStartPtr[0] == '"') && (paramStartPtr[1] == ' ') && (paramStartPtr[2] == '['))
						{
							//printf ("Hidden!\n")
							ifHiddenSSID = 1;
							strcpy(tmpESSID,"[Hidden]");
						}
						else
						{
							int i;
							strcpy(tmpESSID,paramStartPtr);
							for(i=0; i<strlen(tmpESSID)-1; ++i){
								if(tmpESSID[i] == ' ' && tmpESSID[i+1] == '['){
									tmpESSID[i-1] = 0;
									break;
								}
							}
						}
						//printf ("tmpESSID: %s\n",tmpESSID);
					}
					else if (strcmp(paramKindPtr,"Mode") == 0)
					{
						if (strcmp(paramStartPtr,"Master") == 0)
							strcpy(tmpMode,"Master");
						else
							strcpy(tmpMode,"None");
						//printf ("tmpMode:%s\n",tmpMode);
					}
					else if (strcmp(paramKindPtr,"Quality") == 0)
					{
						tmpQuality=atoi(strtok(paramStartPtr,"/"));
						//printf("quality = %d\n",tmpQuality);
					}
					else if (strcmp(paramKindPtr,"Protocol") == 0)
					{
						//A/B/G/N... Not use this parameter
					}
					else if (strcmp(paramKindPtr,"Frequency") == 0)
					{
						//2.47, Channel... Not to use
					}
					else if (strcmp(paramKindPtr,"Bit Rates") == 0)
					{
						//54Mb/s ... Not to use.
					}
					else if (strcmp(paramKindPtr,"Encryption key") == 0)
					{
						//printf("EncrypKEy:%s,%s\n",paramKindPtr,paramStartPtr);
						if (strcmp(paramStartPtr,"on") == 0)
							tmpEncryptionType = 1;
						else
							tmpEncryptionType = 0;
						//printf ("Encryption: %d\n",tmpEncryption);
					}
					else if (strcmp(paramKindPtr,"IE") == 0)
					{
						//printf ("[%s] %s\n",tmpESSID,paramStartPtr);
						if (strcmp(paramStartPtr," IEEE 802.11i/WPA2 Version 1") == 0)
							tmpAuthType|=NIF_AUTH_WPA2PSK;
						else if (strcmp(paramStartPtr," WPA Version 1") == 0)
							tmpAuthType|=NIF_AUTH_WPAPSK;
						else if (strcmp(paramStartPtr," Unknown") == 0)
						{
							tmpAuthType|=NIF_AUTH_UNKNOWN;
							char * UnknownParamOption;
							UnknownParamOption = strtok(NULL,":");
							strcpy(tmpUnknownKeyOption,UnknownParamOption);
							//printf ("UnknownParamOption: %s[%d]\n",tmpUnknownKeyOption,strlen(tmpUnknownKeyOption));
						}
						else
							printf ("Unknown Auth Type: %s\n",paramStartPtr);
						//printf ("[%s] %d\n",tmpESSID,tmpAuthType);
					}
					else
					{

						//printf ("%s -> %s\n",paramKindPtr,paramStartPtr);
					}
				}
				else if (strcmp(tmplinebuf,"") == 0)
				{
					//NULL: Do Nothing;
				}
				else 	//tmplinebuf[20] == 0x20 / BlankSpace;
				{
					//SubParameter;
					char * paramKindPtr;
					char * paramStartPtr;
					paramKindPtr = strtok(&tmplinebuf[23],":");
					paramStartPtr = strtok(NULL,":");

					if((paramKindPtr == NULL) || (paramStartPtr == NULL)){
						DBG_PRINTF_SDH_DEBUG("[%s]%d:Null String\n",__func__,__LINE__);
					}
					else{
						if ( (strcmp(paramKindPtr,"Preauthentication Supported") == 0) )
							tmpPreAuthSupport = 1;
						else if ( (strcmp(paramKindPtr," Group Cipher ") == 0) )
						{
							if (strcmp(paramStartPtr," CCMP") == 0)
								tmpGroupwiseCipher |= 1;
							else if (strcmp(paramStartPtr," TKIP") == 0)
								tmpGroupwiseCipher |= 2;
							else
								printf ("\tUnknown G. Cipher Method: %s\n",paramStartPtr);
							//printf ("G. Cipher Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Pairwise Ciphers (1) ") == 0) )
						{
							if (strcmp(paramStartPtr," CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr," TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method: %s\n",paramStartPtr);
							//printf ("P. Cipher(1) Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Pairwise Ciphers (2) ") == 0) )
						{
							paramStartPtr=strtok(paramStartPtr," ");
							if (strcmp(paramStartPtr,"CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr,"TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method(1): %s\n",paramStartPtr);

							paramStartPtr=strtok(NULL," ");
							if (strcmp(paramStartPtr,"CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr,"TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method(2): %s\n",paramStartPtr);

							//printf ("P. Cipher(2) Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
							//printf ("Two Pairwise Cipher:%d\n",tmpPairwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Authentication Suites (1) ") == 0) )
						{
							if (strcmp(paramStartPtr," 802.1x") == 0)
								tmpAuthSuit=1;
							else if (strcmp(paramStartPtr," PSK") == 0)
								tmpAuthSuit=2;
							else
								printf ("\tUnknown A. Suites Method: %s\n",paramStartPtr);

							//printf ("Auth Method: %s,%d\n",paramStartPtr,tmpAuthSuit);
						}
						else{
							//printf("\tNot Parsed:%s [%s][%s]\n",tmplinebuf,paramKindPtr,paramStartPtr);
						}
					}
				}

				memset(tmplinebuf,0,sizeof(tmplinebuf));
				fgets(tmplinebuf,sizeof(tmplinebuf),fp);
				DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,tmplinebuf);
			}
			printf("[NIF:%s]^^^^^^^^^^^^^^^^^^^^PARSING DONE^^^^^^^^^^^^^^^^^^^^\n",__func__);
			//Finalize Before ParsedArg. (Exactly same above)
			if (UnqualifiedSSID != 0)
			{
				//If this is not the first, parsing
				if ( QUALIFY_KEY )
				{
					#if 0	//Only for Debug Purpose
					sprintf (buf, "\n");
					print_log(buf);
					sprintf (buf, "Parsing infomation -------------------------------------------------------------");
					print_log(buf);
					sprintf (buf, "HWAddr: %s, SSID: %s, Mode: %s",tmpHWAddr, tmpESSID,tmpMode);
					print_log(buf);
					sprintf (buf, "EncType:%d, AuthType:%d[%d], GCipher:%d, PCipher:%d, A.Suit:%d",
							tmpEncryptionType,tmpAuthType,tmpAuthType&(~NIF_AUTH_UNKNOWN),tmpGroupwiseCipher,tmpPairwiseCipher,tmpAuthSuit);
					print_log(buf);
					sprintf (buf, "Hidden?:%d, quality:%d, PreAuthSupport:%d",ifHiddenSSID,tmpQuality,tmpPreAuthSupport);
					print_log(buf);
					sprintf (buf, "EtcOption:%s",tmpUnknownKeyOption);
					print_log(buf);
					sprintf (buf, "------------------------------------------------------------- Parsing infomation\n");
					print_log(buf);
					#endif

					if ( *NumberOfSSID == 0)
					{
						*FoundAPList=calloc(sizeof(T_SSID),1);
					//	printf ("(1) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
					//		*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID) -1,FoundAPList,sizeof(T_SSID));
					} else
					{
						*FoundAPList=realloc(*FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
						//printf ("(2) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
						//	*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID)*((*NumberOfSSID)+1) -1,FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
						//printf ("(3) Clean         0x%08X -> 0x%08X\n",
						//	(unsigned int)(*FoundAPList)+(sizeof(T_SSID)*(*NumberOfSSID)), (unsigned int)(*FoundAPList)+(sizeof(T_SSID)*((*NumberOfSSID)+1)) -1);
						memset((void*)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID))),0,sizeof(T_SSID));
						//printf ("(4) 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
					}
					T_SSID * CurrentItem = (T_SSID *)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID)));
					//printf ("CurrentItem 0x%08x at 0x%08x\n",CurrentItem, &CurrentItem);

					CurrentItem->name=calloc(strlen(tmpESSID)+1,1); strcpy(CurrentItem->name,tmpESSID);
					CurrentItem->hwAddr=calloc(strlen(tmpHWAddr)+1,1); strcpy(CurrentItem->hwAddr,tmpHWAddr);
					CurrentItem->radioQuality=tmpQuality;
					CurrentItem->unknownOption=calloc(strlen(tmpUnknownKeyOption)+1,1); strcpy(CurrentItem->unknownOption,tmpUnknownKeyOption);
					CurrentItem->ifHiddenSSID=ifHiddenSSID;
					CurrentItem->groupCipher=tmpGroupwiseCipher;
					CurrentItem->pairCipher=tmpPairwiseCipher;
					CurrentItem->authSuite=tmpAuthSuit;
					CurrentItem->ifSupportPreAuth=tmpPreAuthSupport;

					sprintf(buf, "sizeof ssid in module: %d", sizeof(T_SSID));
					//printf("sizeof ssid in module: %d", sizeof(T_SSID));
					DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,buf);
					//print_log(buf);

					//Temporary
					tmpAuthType = tmpAuthType &(~NIF_AUTH_UNKNOWN);
					//Check AuthType.
					if (tmpEncryptionType == 0)		//Not use secure
					{
						CurrentItem->authenticationType = NIF_AUTH_NOSECURE;
					}
					else
					{
						if (tmpAuthType == NIF_AUTH_NOSECURE)
							CurrentItem->authenticationType = NIF_AUTH_WEP;
						else
							CurrentItem->authenticationType = tmpAuthType;
					}

					(*NumberOfSSID)++;
				}
			}

			printf ("(NIF) Found SSID: %d, Qualified SSID: %d\n",UnqualifiedSSID,*NumberOfSSID);
			printf ("(NIF) Alloc 0x%08X at 0x%08X\n",(unsigned int)*FoundAPList,(unsigned int)FoundAPList);
			pclose(fp);//fp=popen("iwlist mlan0 scanning","r");pclose(fp);
			return NIF_Status_IfUp;
			//////////
		} else if (gWifiModuleType == SDH_NET_WIFI_MODULE_REALTEK) {
			////////////////////////////////////////////////////////////
			/// Realtek support
			///
			char tmplinebuf[512]; memset(tmplinebuf,0,sizeof(tmplinebuf));
			char tmpHWAddr[64]; memset(tmpHWAddr,0,sizeof(tmpHWAddr));
			char tmpESSID[64]; memset(tmpESSID,0,sizeof(tmpESSID));
			char tmpMode[16]; memset(tmpMode,0,sizeof(tmpMode));
			int tmpEncryptionType=0; int tmpAuthType=0;
			int tmpGroupwiseCipher=0; int tmpPairwiseCipher=0; int tmpAuthSuit=0;
			int ifHiddenSSID = 0; int tmpQuality = 0; int tmpPreAuthSupport = 0;
			char tmpUnknownKeyOption[512]; memset(tmpUnknownKeyOption,0,sizeof(tmpUnknownKeyOption));

			// UnqualifiedSSID -> ++ when found
			// SSID -> ++ when parsed
			if(fgets(tmplinebuf,sizeof(tmplinebuf),fp) != NULL){
				DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,tmplinebuf);
			}
			else{
				DBG_PRINTF_SDH_ERR("[%s]%d:First String returns NULL!pclose(%d)\n",__func__,__LINE__,pclose(fp));
				system("chmod 777 /sbin/iwlist; ls -al /sbin/iwlist");
				return NIF_Status_NotFound;
			}

			while ( !feof(fp) )
			{
				if ( strlen (tmplinebuf) > 0 )
					if ( tmplinebuf[strlen(tmplinebuf)-1] == 0x0A )
						tmplinebuf[strlen(tmplinebuf)-1] = 0x0;
				// printf("[Net/wifi] debugggggggggggggggggggggg\n");
				// printf("strlen:%d, %s\n",strlen(tmplinebuf),tmplinebuf);
				if (tmplinebuf[0] == 'r')
				{
					//Do Nothing
				}
				else if ( (tmplinebuf[0] == 0x20) && (tmplinebuf[10] != 0x20) )
				{
					//Finalize Before ParsedArg.
					if (UnqualifiedSSID != 0)
					{
						//If this is not the first, parsing
						if ( QUALIFY_KEY )
						{
							#if 0	//Only for Debug Purpose
							printf("Parsing information -------------------------------------------------------------\n");
							printf ("HWAddr: %s, SSID: %s, Mode: %s",tmpHWAddr, tmpESSID,tmpMode);
							printf ("EncType:%d, AuthType:%d[%d], GCipher:%d, PCipher:%d, A.Suit:%d\n",
									tmpEncryptionType,tmpAuthType,tmpAuthType&(~NIF_AUTH_UNKNOWN),tmpGroupwiseCipher,tmpPairwiseCipher,tmpAuthSuit);
							printf ("Hidden?:%d, quality:%d, PreAuthSupport:%d\n",ifHiddenSSID,tmpQuality,tmpPreAuthSupport);
							printf ("EtcOption:%s\n",tmpUnknownKeyOption);
							printf ("------------------------------------------------------------- Parsing information\n");
							#endif

							if ( *NumberOfSSID == 0)
							{
								*FoundAPList=calloc(sizeof(T_SSID),1);
							//	printf ("(1) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
							//		*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID) -1,FoundAPList,sizeof(T_SSID));
							} else
							{
								*FoundAPList=realloc(*FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
								//printf ("(2) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
								//	*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID)*((*NumberOfSSID)+1) -1,FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
								//printf ("(3) Clean         0x%08X -> 0x%08X\n",
								//	(unsigned int)(*FoundAPList)+(sizeof(T_SSID)*(*NumberOfSSID)), (unsigned int)(*FoundAPList)+(sizeof(T_SSID)*((*NumberOfSSID)+1)) -1);
								memset((void*)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID))),0,sizeof(T_SSID));
								//printf ("(4) 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
							}
							T_SSID * CurrentItem = (T_SSID *)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID)));
							//printf ("CurrentItem 0x%08x at 0x%08x\n",CurrentItem, &CurrentItem);

							CurrentItem->name=calloc(strlen(tmpESSID)+1,1); strcpy(CurrentItem->name,tmpESSID);
							CurrentItem->hwAddr=calloc(strlen(tmpHWAddr)+1,1); strcpy(CurrentItem->hwAddr,tmpHWAddr);
							CurrentItem->radioQuality=tmpQuality;
							CurrentItem->unknownOption=calloc(strlen(tmpUnknownKeyOption)+1,1); strcpy(CurrentItem->unknownOption,tmpUnknownKeyOption);
							CurrentItem->ifHiddenSSID=ifHiddenSSID;
							CurrentItem->groupCipher=tmpGroupwiseCipher;
							CurrentItem->pairCipher=tmpPairwiseCipher;
							CurrentItem->authSuite=tmpAuthSuit;
							CurrentItem->ifSupportPreAuth=tmpPreAuthSupport;

							// sprintf(buf, "sizeof ssid in module: %d", sizeof(T_SSID));
							//print_log(buf);

							//Temporary
							tmpAuthType = tmpAuthType &(~NIF_AUTH_UNKNOWN);
							//Check AuthType.
							if (tmpEncryptionType == 0)		//Not use secure
							{
								CurrentItem->authenticationType = NIF_AUTH_NOSECURE;
							}
							else
							{
								if (tmpAuthType == NIF_AUTH_NOSECURE)
									CurrentItem->authenticationType = NIF_AUTH_WEP;
								else
									CurrentItem->authenticationType = tmpAuthType;
							}

							(*NumberOfSSID)++;
						}
						//Initialize for NextParsing
						memset(tmpHWAddr,0,sizeof(tmpHWAddr));
						memset(tmpESSID,0,sizeof(tmpESSID));
						memset(tmpMode,0,sizeof(tmpMode));
						tmpEncryptionType = 0; tmpAuthType = 0;
						tmpGroupwiseCipher=0; tmpPairwiseCipher=0; tmpAuthSuit=0;
						ifHiddenSSID = 0; tmpQuality = 0; tmpPreAuthSupport = 0;
						memset(tmpUnknownKeyOption,0,sizeof(tmpUnknownKeyOption));
					}


					//putchar('.');
					//Prepare Next ParsedArg -> get EtherMacAddr
					char * addressStartPtr = NULL;
					strtok(&tmplinebuf[10],"-"); addressStartPtr = strtok(NULL,"-");
					//printf("0x%02X ",*(char*)(addressStartPtr+(strlen(addressStartPtr)-1)));
					strcpy(tmpHWAddr,addressStartPtr+10);
					UnqualifiedSSID++;
					//printf("%s\n",tmpHWAddr);
				}
				else if ( (tmplinebuf[0] == 0x20) && (tmplinebuf[10] == 0x20) && (tmplinebuf[20] != 0x20) )
				{
					//SubParameter;
					char * paramKindPtr = NULL;
					char * paramStartPtr = NULL;
					// char * paramStartPtr2 = NULL;
					// printf ("[Net/wifi] tmplinebuf[20]: %s\n",&tmplinebuf[20]);

					#ifdef OLD_RT3070_DRV
						paramKindPtr = strtok(&tmplinebuf[20],":");
						paramStartPtr = strtok(NULL,":");
					#else
						paramKindPtr = strtok(&tmplinebuf[20],":=");
						paramStartPtr = strtok(NULL,":=");
					#endif

						// printf("[Net/wifi] paramKind: %s\n", paramKindPtr);
						// printf("[Net/wifi] paramStart: %s\n", paramStartPtr);
					
					if (strcmp(paramKindPtr,"ESSID") == 0)
					{
						//printf("%s -> ",paramStartPtr);
						//ESSID
						paramStartPtr++;
						if (strlen(paramStartPtr) > 0)
							if ( *(char*)(paramStartPtr+(strlen(paramStartPtr)-1)) == '"' )
								*(char*)(paramStartPtr+(strlen(paramStartPtr)-1)) = 0;
						// printf ("**********************%s, Length:%d\n",paramStartPtr, strlen(paramStartPtr));
						// if ( (paramStartPtr[0] == '"') && (paramStartPtr[1] == ' ') && (paramStartPtr[2] == '['))
						if (strlen(paramStartPtr) == 0)
						{
							// printf ("Hidden!\n");
							ifHiddenSSID = 1;
							strcpy(tmpESSID,"[Hidden]");
						}
						else
						{
							//int i;
							strcpy(tmpESSID,paramStartPtr);
							/*// The scan result of realtek doesn't contain square bracket. by mspyo 2021-02-16
							for(i=0; i<strlen(tmpESSID)-1; ++i){
								if(tmpESSID[i] == ' ' && tmpESSID[i+1] == '['){
									tmpESSID[i-1] = 0;
									break;
								}
							}
							*/
						}
						// printf ("[Net/wifi] tmpESSID: %s\n",tmpESSID);
					}
					else if (strcmp(paramKindPtr,"Mode") == 0)
					{
						if (strcmp(paramStartPtr,"Master") == 0)
							strcpy(tmpMode,"Master");
						else
							strcpy(tmpMode,"None");
						//printf ("tmpMode:%s\n",tmpMode);
					}
					else if (strcmp(paramKindPtr,"Quality") == 0)
					{
						char * paramStartPtr2 = NULL;
						paramStartPtr2 = strtok(NULL,":=");
						// printf("[Net/wifi] quality param: %s\n", paramStartPtr);
						// printf("[Net/wifi] paramStart2: %s\n", paramStartPtr2);

						int rtlQuality = atoi(strtok(paramStartPtr,"/"));
						int rtlSignal = atoi(strtok(paramStartPtr2,"/"));

						if (rtlQuality < 10) {
							tmpQuality = 0;
						} else {
							tmpQuality= rtlSignal / 20;
							if ((rtlSignal % 20) >= 10) {
								tmpQuality++;
							}
						}
						// printf("[Net/wifi] RTL quality: %d\n", rtlQuality);
						// printf("[Net/wifi] RTL signal: %d\n", rtlSignal);
						// printf("[Net/wifi] RTL calculated quality: %d\n", tmpQuality);
						// tmpQuality=atoi(strtok(paramStartPtr,"/"));
						//printf("quality = %d\n",tmpQuality);
					}
					else if (strcmp(paramKindPtr,"Protocol") == 0)
					{
						//A/B/G/N... Not use this parameter
					}
					else if (strcmp(paramKindPtr,"Frequency") == 0)
					{
						//2.47, Channel... Not to use
					}
					else if (strcmp(paramKindPtr,"Bit Rates") == 0)
					{
						//54Mb/s ... Not to use.
					}
					else if (strcmp(paramKindPtr,"Encryption key") == 0)
					{
						//printf("EncrypKEy:%s,%s\n",paramKindPtr,paramStartPtr);
						if (strcmp(paramStartPtr,"on") == 0)
							tmpEncryptionType = 1;
						else
							tmpEncryptionType = 0;
						//printf ("Encryption: %d\n",tmpEncryption);
					}
					else if (strcmp(paramKindPtr,"IE") == 0)
					{
						//printf ("[%s] %s\n",tmpESSID,paramStartPtr);
						if (strcmp(paramStartPtr," IEEE 802.11i/WPA2 Version 1") == 0)
							tmpAuthType|=NIF_AUTH_WPA2PSK;
						else if (strcmp(paramStartPtr," WPA Version 1") == 0)
							tmpAuthType|=NIF_AUTH_WPAPSK;
						else if (strcmp(paramStartPtr," Unknown") == 0)
						{
							tmpAuthType|=NIF_AUTH_UNKNOWN;
							char * UnknownParamOption;
							UnknownParamOption = strtok(NULL,":");
							strcpy(tmpUnknownKeyOption,UnknownParamOption);
							//printf ("UnknownParamOption: %s[%d]\n",tmpUnknownKeyOption,strlen(tmpUnknownKeyOption));
						}
						else 
							printf ("Unknown Auth Type: %s\n",paramStartPtr);
						//printf ("[%s] %d\n",tmpESSID,tmpAuthType);
					}
					else
					{

						//printf ("%s -> %s\n",paramKindPtr,paramStartPtr);
					}
				}
				else if (strcmp(tmplinebuf,"") == 0)
				{
					//NULL: Do Nothing;
				}
				else 	//tmplinebuf[20] == 0x20 / BlankSpace;
				{
					//SubParameter;
					char * paramKindPtr;
					char * paramStartPtr;
					paramKindPtr = strtok(&tmplinebuf[23],":");
					paramStartPtr = strtok(NULL,":");

					if((paramKindPtr == NULL) || (paramStartPtr == NULL)){
						DBG_PRINTF_SDH_DEBUG("[%s]%d:Null String\n",__func__,__LINE__);
					}
					else{
						if ( (strcmp(paramKindPtr,"Preauthentication Supported") == 0) )
							tmpPreAuthSupport = 1;
						else if ( (strcmp(paramKindPtr," Group Cipher ") == 0) )
						{
							if (strcmp(paramStartPtr," CCMP") == 0)
								tmpGroupwiseCipher |= 1;
							else if (strcmp(paramStartPtr," TKIP") == 0)
								tmpGroupwiseCipher |= 2;
							else
								printf ("\tUnknown G. Cipher Method: %s\n",paramStartPtr);
							//printf ("G. Cipher Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Pairwise Ciphers (1) ") == 0) )
						{
							if (strcmp(paramStartPtr," CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr," TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method: %s\n",paramStartPtr);
							//printf ("P. Cipher(1) Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Pairwise Ciphers (2) ") == 0) )
						{
							paramStartPtr=strtok(paramStartPtr," ");
							if (strcmp(paramStartPtr,"CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr,"TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method(1): %s\n",paramStartPtr);

							paramStartPtr=strtok(NULL," ");
							if (strcmp(paramStartPtr,"CCMP") == 0)
								tmpPairwiseCipher |= 1;
							else if (strcmp(paramStartPtr,"TKIP") == 0)
								tmpPairwiseCipher |= 2;
							else
								printf ("\tUnknown P. Cipher Method(2): %s\n",paramStartPtr);

							//printf ("P. Cipher(2) Method: %s,%d\n",paramStartPtr,tmpGroupwiseCipher);
							//printf ("Two Pairwise Cipher:%d\n",tmpPairwiseCipher);
						}
						else if ( (strcmp(paramKindPtr," Authentication Suites (1) ") == 0) )
						{
							if (strcmp(paramStartPtr," 802.1x") == 0)
								tmpAuthSuit=1;
							else if (strcmp(paramStartPtr," PSK") == 0)
								tmpAuthSuit=2;
							else
								printf ("\tUnknown A. Suites Method: %s\n",paramStartPtr);

							//printf ("Auth Method: %s,%d\n",paramStartPtr,tmpAuthSuit);
						}
						else{
							//printf("\tNot Parsed:%s [%s][%s]\n",tmplinebuf,paramKindPtr,paramStartPtr);
						}
					}
				}

				memset(tmplinebuf,0,sizeof(tmplinebuf));
				fgets(tmplinebuf,sizeof(tmplinebuf),fp);
				DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,tmplinebuf);
			}
			printf("[NIF:%s]^^^^^^^^^^^^^^^^^^^^PARSING DONE^^^^^^^^^^^^^^^^^^^^\n",__func__);
			//Finalize Before ParsedArg. (Exactly same above)
			if (UnqualifiedSSID != 0)
			{
				//If this is not the first, parsing
				if ( QUALIFY_KEY )
				{
					#if 0	//Only for Debug Purpose
					printf("Parsing information -------------------------------------------------------------\n");
					printf ("HWAddr: %s, SSID: %s, Mode: %s",tmpHWAddr, tmpESSID,tmpMode);
					printf ("EncType:%d, AuthType:%d[%d], GCipher:%d, PCipher:%d, A.Suit:%d\n",
							tmpEncryptionType,tmpAuthType,tmpAuthType&(~NIF_AUTH_UNKNOWN),tmpGroupwiseCipher,tmpPairwiseCipher,tmpAuthSuit);
					printf ("Hidden?:%d, quality:%d, PreAuthSupport:%d\n",ifHiddenSSID,tmpQuality,tmpPreAuthSupport);
					printf ("EtcOption:%s\n",tmpUnknownKeyOption);
					printf ("------------------------------------------------------------- Parsing information\n");
					#endif

					if ( *NumberOfSSID == 0)
					{
						*FoundAPList=calloc(sizeof(T_SSID),1);
					//	printf ("(1) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
					//		*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID) -1,FoundAPList,sizeof(T_SSID));
					} else
					{
						*FoundAPList=realloc(*FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
						//printf ("(2) 0x%08X ->               0x%08X at 0x%08X [%02X]\n",
						//	*FoundAPList,(unsigned int)(*FoundAPList)+sizeof(T_SSID)*((*NumberOfSSID)+1) -1,FoundAPList,sizeof(T_SSID)*((*NumberOfSSID)+1));
						//printf ("(3) Clean         0x%08X -> 0x%08X\n",
						//	(unsigned int)(*FoundAPList)+(sizeof(T_SSID)*(*NumberOfSSID)), (unsigned int)(*FoundAPList)+(sizeof(T_SSID)*((*NumberOfSSID)+1)) -1);
						memset((void*)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID))),0,sizeof(T_SSID));
						//printf ("(4) 0x%08X at 0x%08X\n",*FoundAPList,FoundAPList);
					}
					T_SSID * CurrentItem = (T_SSID *)((unsigned int)*FoundAPList+(sizeof(T_SSID)*(*NumberOfSSID)));
					//printf ("CurrentItem 0x%08x at 0x%08x\n",CurrentItem, &CurrentItem);
				
					CurrentItem->name=calloc(strlen(tmpESSID)+1,1); strcpy(CurrentItem->name,tmpESSID);
					CurrentItem->hwAddr=calloc(strlen(tmpHWAddr)+1,1); strcpy(CurrentItem->hwAddr,tmpHWAddr);
					CurrentItem->radioQuality=tmpQuality;
					CurrentItem->unknownOption=calloc(strlen(tmpUnknownKeyOption)+1,1); strcpy(CurrentItem->unknownOption,tmpUnknownKeyOption);
					CurrentItem->ifHiddenSSID=ifHiddenSSID;
					CurrentItem->groupCipher=tmpGroupwiseCipher;
					CurrentItem->pairCipher=tmpPairwiseCipher;
					CurrentItem->authSuite=tmpAuthSuit;
					CurrentItem->ifSupportPreAuth=tmpPreAuthSupport;

					sprintf(buf, "sizeof ssid in module: %d", sizeof(T_SSID));
					//printf("sizeof ssid in module: %d", sizeof(T_SSID));
					DBG_PRINTF_SDH_DEBUG("[%s]%d:%s\n",__FILE__,__LINE__,buf);
					//print_log(buf);

					//Temporary
					tmpAuthType = tmpAuthType &(~NIF_AUTH_UNKNOWN);
					//Check AuthType.
					if (tmpEncryptionType == 0)		//Not use secure
					{
						CurrentItem->authenticationType = NIF_AUTH_NOSECURE;
					}
					else
					{
						if (tmpAuthType == NIF_AUTH_NOSECURE)
							CurrentItem->authenticationType = NIF_AUTH_WEP;
						else
							CurrentItem->authenticationType = tmpAuthType;
					}

					(*NumberOfSSID)++;
				}
			}

			printf ("(NIF) Found SSID: %d, Qualified SSID: %d\n",UnqualifiedSSID,*NumberOfSSID);
			printf ("(NIF) Alloc 0x%08X at 0x%08X\n",(unsigned int)*FoundAPList,(unsigned int)FoundAPList);
			pclose(fp);//fp=popen("iwlist mlan0 scanning","r");pclose(fp);
			return NIF_Status_IfUp;
			////////////////////////////////////////////////////////////
			/// Realtek wifi support
			///
		} else {
			// Neither marvell nor realtek. by mspyo, 2021-02-15
			return NIF_Status_NotFound;
		}
	}
}

int NIF_freeSSID(int NumberOfSSID, T_SSID * FoundAPList)
{
	//return 1;
	int i=0;
	
	printf ("(NIF) Free 0x%08X @ 0x%08X\n",(unsigned int)FoundAPList,(unsigned int)&FoundAPList);
	
	if (FoundAPList == NULL)
		return NIF_Status_NotFound;
		
			
	for (i=0;i<NumberOfSSID;i++)
	{
		T_SSID * CurrentItem;
		CurrentItem = (T_SSID *)((unsigned int)(FoundAPList)+i*sizeof(T_SSID));
		
	//	printf("(NIF)      FreeSub 0x%08x\n",CurrentItem);
		
		if (CurrentItem == NULL)
			return NIF_Status_NotFound;
		if (CurrentItem->name != NULL)	{ free(CurrentItem->name); CurrentItem->name = NULL; };
		if (CurrentItem->hwAddr != NULL) { free (CurrentItem->hwAddr); CurrentItem->hwAddr = NULL; };
		if (CurrentItem->unknownOption != NULL) { free (CurrentItem->unknownOption); CurrentItem->unknownOption = NULL; };
		if (CurrentItem->passKey1 != NULL) { free (CurrentItem->passKey1); CurrentItem->passKey1 = NULL; };
		if (CurrentItem->passKey2 != NULL) { free (CurrentItem->passKey2); CurrentItem->passKey2 = NULL; };
		if (CurrentItem->passKey3 != NULL) { free (CurrentItem->passKey3); CurrentItem->passKey3 = NULL; };
		if (CurrentItem->passKey4 != NULL) { free (CurrentItem->passKey4); CurrentItem->passKey4 = NULL; };
		
	}

		
	free (FoundAPList);
	return NIF_Status_IfUp;
};


int NIF_connectSSID(int RowOfSSID, T_SSID * FoundAPList)
{
#define WEP_FILENAME	"/etc/network/wep.conf"
#define WPA_FILENAME	"/etc/network/wpa.conf"
#define NONE_FILENAME	"/etc/network/none.conf"
	T_SSID * CurrentSSID = NULL;
	char tmpLineBuf[256] = {0};
	unsigned char psk[32]= {0};
	char pskStr[256]= {0};
	int i = 0;
	
	CurrentSSID = (T_SSID *)((unsigned int)(FoundAPList) + (RowOfSSID*sizeof(T_SSID)));
	if(CurrentSSID == NULL)
	{
	   return NIF_Status_NotFound;
	}
	DBG_PRINTF_SDH_DEBUG("(NIF) Connect to \"%s\", Sec:%d, Q:%d\n",CurrentSSID->name,CurrentSSID->authenticationType,CurrentSSID->radioQuality);

	///*	
	DBG_PRINTF_SDH_DEBUG("(NIF) unlink setting file "WEP_FILENAME", "WPA_FILENAME", "NONE_FILENAME"\n");
	unlink ( WEP_FILENAME );
	unlink ( WPA_FILENAME );
	unlink ( NONE_FILENAME );
	//*/
	int killTry = 0;
	while (1)
	{
		killTry++;
		DBG_PRINTF_SDH_DEBUG("(NIF) kill udhdpc (\"kill `cat "DNS_PID_FILENAME"` 2>/dev/null\") %d Times\n",killTry);
		system ("kill `cat "DNS_PID_FILENAME"` 2>/dev/null");
		
		if (killTry > 12)
		{
			DBG_PRINTF_SDH_ERR("(NIF) Error: Cannot kill udhdpc\n");
			//return NIF_Status_Unknown;
			break;
		}
		else
		{
			if ( access(DNS_PID_FILENAME,F_OK )!=0) 	break;	
			else 				usleep(250*1000);	//250ms
		}
	}
	usleep(250*1000);
	
	killTry = 0;		
	while (1)
	{
		killTry++;
		DBG_PRINTF_SDH_DEBUG("(NIF) kill wpa_supplicant (\"killall wpa_supplicant\") %d Times\n",killTry);
		system ("killall wpa_supplicant");
		
		if (killTry > 12)
		{
			DBG_PRINTF_SDH_DEBUG("(NIF) Error: Cannot kill wpa_supplicant\n");
			//return NIF_Status_Unknown;
			break;
		}
		else
		{
			if ( access(WPA_PID_FILENAME,F_OK )!= 0) 	break;	
			else 							usleep(250*1000);	//250ms
		}
	}
	usleep(250*1000);

	DBG_PRINTF_SDH_DEBUG("(NIF) ifconfig mlan0 down...pAss\n");
	system ("ifdown mlan0");
	//printf ("(NIF) ifconfig mlan0 up...\n");
	//system ("ifconfig mlan0 up");
	usleep(250*1000);
	
	FILE * fp = NULL;
	enum getStatusReturnType result = NIF_Status_NotFound;
	switch (CurrentSSID->authenticationType)
	{
		case NIF_AUTH_NOSECURE:
			DBG_PRINTF_SDH_DEBUG("(NIF) No Secure\n");
			//printf ("(NIF) Turnoff Radio...pAss\n");
			//NIF_turnRadio_off_wifi();
			//printf ("(NIF) Turnon Radio...pAss\n");
			//NIF_turnRadio_on_wifi();
			//system("iwpriv mlan0 set NetworkType=Infra");
			//system("iwpriv mlan0 set AuthMode=OPEN");
			//system("iwpriv mlan0 set EncrypType=NONE");
			//snprintf(tmpLineBuf,sizeof(tmpLineBuf),"iwpriv mlan0 set SSID=\"%s\"",CurrentSSID->name);
			//system(tmpLineBuf); //printf("tmpLineBuf: %s\n",tmpLineBuf);
			
			fp = fopen( NONE_FILENAME, "wt" );
			if(fp == NULL)
			{
				break;
			}
//			fprintf (fp,"update_config=1 \nctrl_interface=/var/run/wpa_supplicant \neapol_version=1 \nap_scan=2 \nfast_reauth=1\n");
			fprintf (fp,"network={\n");
			fprintf (fp,"\tssid=\"%s\"\n",CurrentSSID->name);
			fprintf (fp,"\tkey_mgmt=NONE\n");
			fprintf (fp,"\t}\n");
			if(fflush(fp) != 0){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
			}
			if(fsync(fp->_fileno) == -1){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
			}
			fclose(fp);
			
			snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c"NONE_FILENAME" -P"WPA_PID_FILENAME); // WTF
			// snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c%s -P%s", NONE_FILENAME, WPA_PID_FILENAME); // by mspyo, 2020-12-23
			printf ("(NIF) CMD:%s\n",tmpLineBuf);
			system(tmpLineBuf);			
			printf ("(NIF) NON-secure try to connect\n");					
			result = NIF_Status_IfUp;
		    break;
		case NIF_AUTH_WEP:
			DBG_PRINTF_SDH_DEBUG("(NIF) WEP\n");
			fp = fopen( WEP_FILENAME, "wt" );
			if(fp == NULL)
			{
				break;
			}
//			fprintf (fp,"update_config=1 \nctrl_interface=/var/run/wpa_supplicant \neapol_version=1 \nap_scan=2 \nfast_reauth=1\n");
			fprintf (fp,"network={\n");
			fprintf (fp,"\tssid=\"%s\"\n",CurrentSSID->name);
			fprintf (fp,"\tkey_mgmt=NONE\n");
			fprintf (fp,"\tpairwise=NONE\n");
			fprintf (fp,"\tgroup=WEP40 WEP104\n");
			fprintf (fp,"\tauth_alg=OPEN SHARED\n");
			if ( (strlen(CurrentSSID->passKey1) == 5) || (strlen(CurrentSSID->passKey1) == 13) )
				fprintf (fp,"\twep_key0=\"%s\"\n",CurrentSSID->passKey1);
			else
				fprintf (fp,"\twep_key0=%s\n",CurrentSSID->passKey1);
				
			fprintf (fp,"\twep_key1=0000000000\n");
			fprintf (fp,"\twep_key2=0000000000\n");
			fprintf (fp,"\twep_key3=0000000000\n");
			fprintf (fp,"\twep_tx_keyidx=0\n");
			fprintf (fp,"\t}\n");

			if(fflush(fp) != 0){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
			}
			if(fsync(fp->_fileno) == -1){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
			}
			fclose(fp);
			
			//system("cat "WEP_FILENAME);
			
			//snprintf(tmpLineBuf,sizeof(tmpLineBuf),"iwconfig mlan0 essid \"%s\"",CurrentSSID->name);
			//printf ("(NIF) CMD:%s\n",tmpLineBuf);
			//system(tmpLineBuf);
			
			snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c"WEP_FILENAME" -P"WPA_PID_FILENAME); // WTF
			// snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c%s -P%s", WEP_FILENAME, WPA_PID_FILENAME); // by mspyo, 2020-12-23
			printf ("(NIF) CMD:%s\n",tmpLineBuf);
			system(tmpLineBuf);
			
			printf ("(NIF) WEP try to connect\n");
			
			result = NIF_Status_IfUp;
		    break;
		case NIF_AUTH_WPAPSK:
		case NIF_AUTH_WPA2PSK:
		case NIF_AUTH_WPAPSK_WPA2PSK:
			DBG_PRINTF_SDH_DEBUG("(NIF) WPA/PSK\n");
			
			if (strlen(CurrentSSID->passKey1) < 8 || strlen(CurrentSSID->passKey1) > 63) 
			{
				printf("(NIF) Passkey size missmatch: Should be 8<=Size<=63 \n");
				return NIF_Status_Unknown;
			}
			pbkdf2_sha1(CurrentSSID->passKey1, CurrentSSID->name, strlen(CurrentSSID->name), 4096, psk, 32);
			memset(pskStr,0,sizeof(pskStr));
			printf("SIZE : %d\n",sizeof(pskStr));
			for (i = 0; i < 32; i++){ 
				sprintf(pskStr,"%s%02x",pskStr,psk[i]);
				//printf("%d : %s\n",i,pskStr);
			}
			
			//printf ("\tssid=\"%s\"\n",CurrentSSID->name);
			//printf ("(NIF) Psk:%s\n",pskStr);
			
			
			fp = fopen( WPA_FILENAME, "wt" );
			if(fp == NULL)
			{
				break;
			}

//			fprintf (fp,"update_config=1 \nctrl_interface=/var/run/wpa_supplicant \neapol_version=1 \nap_scan=2 \nfast_reauth=1\n");
			fprintf (fp,"network={\n");
			fprintf (fp,"\tssid=\"%s\"\n",CurrentSSID->name);
			fprintf (fp,"\tpsk=%s\n",pskStr);
			fprintf (fp,"\tkey_mgmt=WPA-PSK\n");
			switch (CurrentSSID->groupCipher)
			{
				case NIF_CIPHER_TKIP:
					fprintf (fp,"\tgroup=TKIP\n");
				break;
				case NIF_CIPHER_AES:
					fprintf (fp,"\tgroup=CCMP\n");
				break;
				case NIF_CIPHER_AES_TKIP:
					fprintf (fp,"\tgroup=CCMP TKIP\n");
				break;
			}
			switch (CurrentSSID->pairCipher)
			{
				case NIF_CIPHER_TKIP:
					fprintf (fp,"\tpairwise=TKIP\n");
				break;
				case NIF_CIPHER_AES:
					fprintf (fp,"\tpairwise=CCMP\n");
				break;
				case NIF_CIPHER_AES_TKIP:
					fprintf (fp,"\tpairwise=CCMP TKIP\n");
				break;
			}
			switch (CurrentSSID->authenticationType)
			{
				case NIF_AUTH_WPAPSK:
					fprintf (fp,"\tproto=WPA\n");
				break;
				case NIF_AUTH_WPA2PSK:
					fprintf (fp,"\tproto=RSN\n");
				break;
				case NIF_AUTH_WPAPSK_WPA2PSK:
					fprintf (fp,"\tproto=WPA RSN\n");
				break;
			}
			
			fprintf (fp,"\t}\n");

			if(fflush(fp) != 0){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
			}
			if(fsync(fp->_fileno) == -1){
				DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
			}
			fclose(fp);
			
			//snprintf(tmpLineBuf,sizeof(tmpLineBuf),"iwconfig mlan0 essid \"%s\"",CurrentSSID->name);
			//printf ("(NIF) CMD:%s\n",tmpLineBuf);
			//system(tmpLineBuf);
			
			snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c"WPA_FILENAME" -P"WPA_PID_FILENAME); // WTF! by Who???!!!
			// snprintf(tmpLineBuf,sizeof(tmpLineBuf),"wpa_supplicant -B -d -Dwext -imlan0 -c%s -P%s", WPA_FILENAME, WPA_PID_FILENAME); // by mspyo, 2020-11-20
			printf ("(NIF) CMD:%s\n",tmpLineBuf);
			system(tmpLineBuf);
			
			printf ("(NIF) WPA/WPA2 (%d) try to connect\n",CurrentSSID->authenticationType);
			result = NIF_Status_IfUp;
  		    break;
		default:
			printf ("(NIF) Not support secure type: %d\n",CurrentSSID->authenticationType);
			result = NIF_Status_Unknown;
		    break;
	}

	return result;
}

int NIF_turnRadio_on_wifi(void)
{
	system("iwpriv mlan0 radio_on");
	return NIF_Status_IfUp;
}
int NIF_turnRadio_off_wifi(void)
{
	system("iwpriv mlan0 radio_off");
	return NIF_Status_IfDown;
}

#define SDH_NET_OPERSTATE_LENGTH 16
int NIF_connectStatus_wifi(void)
{
	FILE *fp;
	char operState[SDH_NET_OPERSTATE_LENGTH] = {0};
	const char *upState = "up";

	// printf("[Net] Wifi module type: %d\n", gWifiModuleType); // Temp. by mspyo, 2021-02-15
	fp = fopen("/sys/class/net/mlan0/operstate", "r"); // Changed from carrier by mspyo, 2021-02-15

	if (fp == NULL) {
		printf("[Net] Wifi operstate file is not found\n");
		return NIF_Status_NotFound;
	} else {
		fgets(operState, SDH_NET_OPERSTATE_LENGTH, fp); // by mspyo, 2021-02-15
		// Possible values: "unknown", "notpresent", "down", "lowerlayerdown", "testing", "dormant", "up".
		fclose(fp);
	}

	if (strstr(operState, upState) != NULL) {
		// printf("[Net] Wifi carrier is up.\n"); // Temp. by mspyo. 2021-02-15
		return NIF_Status_IfUp;
	} else {
		// printf("[Net] Wifi carrier is down.\n"); // Temp. by mspyo. 2021-02-15
		return NIF_Status_IfDown;
	}

#if 0 // Legacy code for marvell module
	FILE *fp1;
	char tmplinebuf1[128]; memset(tmplinebuf1,0,sizeof(tmplinebuf1));
	int result;
	if (access("/proc/mwlan/mlan0/info", F_OK) != 0)
	{
        return NIF_Status_NotFound;
	}
	fp1=popen("cat /proc/mwlan/mlan0/info | grep media_state","r");
	if(fp1 == NULL){
		printf ("(NIF) Status Not found [1]\n");
		return NIF_Status_NotFound;
	}

    if(fgets(tmplinebuf1,sizeof(tmplinebuf1),fp1) != NULL){
		if( strcmp(tmplinebuf1,"media_state=\"Connected\"\n") == 0){
//			if(fgets(tmplinebuf2,sizeof(tmplinebuf2),fp2) != NULL){
				result = NIF_Status_IfUp;
//			}
		}
		else{
			//printf("%s",tmplinebuf1);
			result = NIF_Status_NotFound;
		}
	}
	else{
			result = NIF_Status_NotFound;
	}

	pclose(fp1);
	//pclose(fp2);
	
	return result;
#endif
}

#if 0
	fp=popen("iwpriv mlan0 connStatus","r");
	
	if (fp == NULL)
	{
		printf ("(NIF) Status Not found [1]\n");
		return NIF_Status_NotFound;
	}
	
	char tmplinebuf[200]; memset(tmplinebuf,0,sizeof(tmplinebuf));
	fgets(tmplinebuf,sizeof(tmplinebuf),fp);
	
	if (strcmp(tmplinebuf,"") == 0)
	{
		pclose(fp);
		printf ("(NIF) Interface down?\n");
		return NIF_Status_NotFound;
	}
	else if (strcmp(tmplinebuf,"mlan0       connStatus:\n")==0)
	{
		pclose(fp);
		printf ("(NIF) DisConnect ... 10\n");
		//*SSID=calloc(strlen(MSG_DISCONN)+1,1);
		//strcpy(*SSID,MSG_DISCONN);		
		return NIF_Status_IfDown;		
	}	
	
	char *firstStr,*secondStr;
	strtok(tmplinebuf," ");
	firstStr=strtok(NULL," :");
	if (strcmp(firstStr,"connStatus")!=0)
	{
		pclose(fp);
		printf ("(NIF) Status Not found [2]\n");
		return NIF_Status_NotFound;
	}
		
	//printf("First:%s\n",firstStr);
	secondStr=strtok(NULL," :");
	//printf("second:[%s]\n",secondStr);
	if (strcmp(secondStr,"\n") == 0)
	{
		pclose(fp);
		printf ("(NIF) if Down...[1]\n");
		return NIF_Status_IfDown;
	}
	else if (strcmp(secondStr,"Disconnected\n")==0) 
	{
		char tmpLineBuf[200];
		pclose(fp);
		
		//iwconfig Encryption key find.
		//fp=popen("iwconfig mlan0 | grep Encryption","r");
		fp=popen("iwconfig mlan0 | grep \"Bit Rate\"","r");
		fgets(tmpLineBuf,sizeof(tmpLineBuf),fp);
		pclose(fp);
		
		//printf ("iwconfig Key: [%s]\n",tmpLineBuf);
		//if (strcmp(tmpLineBuf,"          Encryption key:off\n")==0)
		if ( (strcmp(tmpLineBuf,"          Bit Rate:1 Mb/s   \n") == 0) || (strcmp(tmpLineBuf,"          Bit Rate=1 Mb/s   \n") == 0) )
		{
			//int i=0;	for (i=0;i<strlen(tmpLineBuf);i++)	printf ("0x%02X ",tmpLineBuf[i]);
			printf ("(NIF) if Down...[2]\n");
			return NIF_Status_IfDown;
		}
		else
		{
			//int i=0;	for (i=0;i<strlen(tmpLineBuf);i++)	printf ("0x%02X ",tmpLineBuf[i]);
			printf ("(NIF) if UP...[3]: [%s][%s]",tmpLineBuf,secondStr);
			return NIF_Status_IfUp;
		}
		
	}
	else
	{
		printf("(NIF) if UP...[4]: [%s]\n",secondStr);
		pclose(fp);
		return NIF_Status_IfUp;
	}
#endif

int NIF_checkSSIDinDB(int RowOfSSID, T_SSID * FoundAPList)
{
	T_SSID * CurrentItem;
	CurrentItem = FoundAPList+RowOfSSID;
	
	char sqlQuery[400] = {0};
	int row = -1;
	int col = -1;
	char **queryResult=NULL;
	int qresult = SQLITE_ERROR;
		
	snprintf(sqlQuery,sizeof(sqlQuery),"select passkey1,passkey2,passkey3,passkey4 from recd_SSIDBOOK where SSID=\"%s\";",CurrentItem->name);
	//printf ("sqlQuery:%s\n",sqlQuery);
	qresult=NIF_trySQLiteGetTable(sqlQuery,&queryResult,&row,&col);
	//printf("qresult:%d, row:%d, col:%d\n",qresult,row,col);
	
	if ( (row == 0) || (qresult !=SQLITE_OK) )
	{
		sqlite3_free_table(queryResult);
		return NIF_Status_NotFound;
	}
	else
	{
		if (CurrentItem->authenticationType != NIF_AUTH_NOSECURE)
		{
			//printf("%s-%s-%s-%s\n",queryResult[0],queryResult[1],queryResult[2],queryResult[3]);
			if(strcmp(queryResult[4],"")!=0) { CurrentItem->passKey1=calloc(strlen(queryResult[4])+1,1); strcpy (CurrentItem->passKey1,queryResult[4]); }
			if(strcmp(queryResult[5],"")!=0) { CurrentItem->passKey2=calloc(strlen(queryResult[5])+1,1); strcpy (CurrentItem->passKey2,queryResult[5]); }
			if(strcmp(queryResult[6],"")!=0) { CurrentItem->passKey3=calloc(strlen(queryResult[6])+1,1); strcpy (CurrentItem->passKey3,queryResult[6]); }
			if(strcmp(queryResult[7],"")!=0) { CurrentItem->passKey4=calloc(strlen(queryResult[7])+1,1); strcpy (CurrentItem->passKey4,queryResult[7]); }
		}
		//printf("%s-%s-%s-%s\n",queryResult[4],queryResult[5],queryResult[6],queryResult[7]);
		//printf("%s-%s-%s-%s\n",CurrentItem->passKey1,CurrentItem->passKey2,CurrentItem->passKey3,CurrentItem->passKey4);
		sqlite3_free_table(queryResult);
		return NIF_Status_IfUp;
	}
}

int NIF_getConnectedSSID(char **SSID)
{
	#define MSG_NOTFOUND	"Interface not found"
	#define MSG_DISCONN		"Wifi Not Connected"
	*SSID = NULL;
	FILE *fp = popen("iwpriv mlan0 connStatus","r");
	
	if (fp == NULL)
	{
//		printf ("(NIF) Not Found ... 1\n");
		*SSID=calloc(strlen(MSG_NOTFOUND)+1,1);
		strcpy(*SSID,MSG_NOTFOUND);
		return NIF_Status_NotFound;
	}
	
	char tmplinebuf[200]; memset(tmplinebuf,0,sizeof(tmplinebuf));
	fgets(tmplinebuf,sizeof(tmplinebuf),fp);
	
//	printf ("(NIF) ParsingStr:[%s]\n",tmplinebuf);
	
	if (strcmp(tmplinebuf,"") == 0)
	{
		pclose(fp);
//		printf ("(NIF) Not Found ... 2\n");
		*SSID=calloc(strlen(MSG_NOTFOUND)+1,1);
		strcpy(*SSID,MSG_NOTFOUND);		
		return NIF_Status_NotFound;
	}
	else if (strcmp(tmplinebuf,"mlan0       connStatus:\n")==0)
	{
		pclose(fp);
//		printf ("(NIF) DisConnect ... 1\n");
		*SSID=calloc(strlen(MSG_DISCONN)+1,1);
		strcpy(*SSID,MSG_DISCONN);		
		return NIF_Status_IfDown;		
	}
	
	char *firstStr=NULL,*secondStr=NULL;
	strtok(tmplinebuf," ");
	//printf("(NIF) zeroStr-1:%s,[%s]\n",zeroStr,tmplinebuf);
	
	firstStr=strtok(NULL," :");
	//printf("(NIF) First-1:%s\n",firstStr);
	if (strcmp(firstStr,"connStatus")!=0)
	{
//		printf ("(NIF) Not Found ... 3\n");
		*SSID=calloc(strlen(MSG_NOTFOUND)+1,1);
		strcpy(*SSID,MSG_NOTFOUND);			
		pclose(fp);
		return NIF_Status_NotFound;
	}
		
	//printf("(NIF) First-2:%s\n",firstStr);
	secondStr=strtok(NULL," :");
	//printf("(NIF) second-3:[%s]\n",secondStr);
	
	if (strcmp(secondStr,"Disconnected\n")==0)
	{
		char tmpLineBuf[200];
		pclose(fp);
		
		//iwconfig Encryption key find.
		fp=popen("iwconfig mlan0 | grep Encryption","r");
		if(fp == NULL)
		{
			return NIF_Status_Unknown;
		}		
		fgets(tmpLineBuf,sizeof(tmpLineBuf),fp);
//		printf ("(NIF) iwconfig Key: [%s]\n",tmpLineBuf);
		pclose(fp);
		if (strcmp(tmpLineBuf,"          Encryption key:off\n")==0)
		{
			*SSID=calloc(strlen(MSG_DISCONN)+1,1);
			strcpy(*SSID,MSG_DISCONN);	
//			printf ("(NIF) DisConnect ... 2\n");
			return NIF_Status_IfDown;
		}
		else
		{
			fp=popen("iwconfig mlan0 | grep ESSID | awk '{print $2}' -F\\\"","r");
			if(fp == NULL)
			{
				return NIF_Status_Unknown;
			}
			fgets(tmpLineBuf,sizeof(tmpLineBuf),fp);
			pclose(fp);
			
			if ( (strlen(tmpLineBuf)>0) && (tmpLineBuf[strlen(tmpLineBuf)-1] == '\n') )
				tmpLineBuf[strlen(tmpLineBuf)-1] = '\0';
			
			*SSID=calloc(strlen(tmpLineBuf)+1,1);
			strcpy(*SSID,tmpLineBuf);	
			
			
//			printf ("(NIF) if UP... by psk[%s]...\n",tmpLineBuf);
			return NIF_Status_IfUp;
		}
//		printf ("(NIF) DisConnect ... 3\n");
		return NIF_Status_IfDown;
	}
	else
	{
//		printf ("(NIF) Case 5n");
		char *thirdStr=NULL;
		thirdStr=strtok(NULL," [");
		//printf ("thirdStr: %s\n",thirdStr);
		*SSID=calloc(strlen(thirdStr)+1,1);
		strcpy(*SSID,thirdStr);
		pclose(fp);
		return NIF_Status_IfUp;
	}	
}

int NIF_getOneSSIDinDB(int * NumberOfSSID, T_SSID ** FoundAPList)
{
	*NumberOfSSID = 0;
	*FoundAPList = NULL;
	
	char sqlQuery[400] = {0};
	int row = -1;
	int col = -1;
	char **queryResult=NULL;
	int qresult = SQLITE_ERROR;
	
	snprintf(sqlQuery,sizeof(sqlQuery),
		"select SSID,APMacAddr,AuthType,GroupCipher,PairCipher,ifSupportPreAuth,AuthSuite,"
		"Passkey1, Passkey2, Passkey3, Passkey4 from recd_SSIDBOOK order by no desc limit 0,1;");
		
	qresult=NIF_trySQLiteGetTable(sqlQuery,&queryResult,&row,&col);
	
	if (qresult!=SQLITE_OK)
	{
		//printf("1\n");
		sqlite3_free_table(queryResult);
		return NIF_Status_Unknown;
	} 
	else if ( row < 1 )
	{
		sqlite3_free_table(queryResult);
		return NIF_Status_NotFound;
	} 
	else
	{
		*NumberOfSSID = 1;
		*FoundAPList = calloc(sizeof(T_SSID),1);
		
		//DB값 Copy.
		printf ("Read Col: %d, Row: %d\n",col,row);
		(*FoundAPList)->name=calloc(strlen(queryResult[col+0])+1,1); strcpy((*FoundAPList)->name,queryResult[col+0]);	//Name;
		(*FoundAPList)->hwAddr=calloc(strlen(queryResult[col+1])+1,1); strcpy((*FoundAPList)->hwAddr,queryResult[col+1]);	//MacAddr;
		(*FoundAPList)->authenticationType=atoi(queryResult[col+2]);	//AuthType
		(*FoundAPList)->groupCipher=atoi(queryResult[col+3]);		//GroupCipher
		(*FoundAPList)->pairCipher=atoi(queryResult[col+4]);	//PairCipher
		(*FoundAPList)->ifSupportPreAuth=atoi(queryResult[col+5]);	//support PreAuth
		(*FoundAPList)->authSuite=atoi(queryResult[col+6]);	//AuthSuite
		
		if (strcmp(queryResult[col+7],"") !=0) { (*FoundAPList)->passKey1=calloc(strlen(queryResult[col+7])+1,1); strcpy((*FoundAPList)->passKey1,queryResult[col+7]);	}//Name; 
		if (strcmp(queryResult[col+8],"") !=0) { (*FoundAPList)->passKey2=calloc(strlen(queryResult[col+8])+1,1); strcpy((*FoundAPList)->passKey2,queryResult[col+8]);	}//Name; 
		if (strcmp(queryResult[col+9],"") !=0) { (*FoundAPList)->passKey3=calloc(strlen(queryResult[col+9])+1,1); strcpy((*FoundAPList)->passKey3,queryResult[col+9]);	}//Name; 
		if (strcmp(queryResult[col+10],"") !=0) { (*FoundAPList)->passKey4=calloc(strlen(queryResult[col+10])+1,1); strcpy((*FoundAPList)->passKey4,queryResult[col+10]);	}//Name; 
		
		sqlite3_free_table(queryResult);
		return NIF_Status_IfUp;
	}
};

int NIF_getOneSSIDinDBbySSID(int * NumberOfSSID, T_SSID ** FoundAPList, char *SSIDName)
{
	*NumberOfSSID = 0;
	*FoundAPList = NULL;
	
	char sqlQuery[400] = {0};
	int row = -1;
	int col = -1;
	char **queryResult=NULL;
	int qresult = SQLITE_ERROR;
	
	snprintf(sqlQuery,sizeof(sqlQuery),
		"select SSID,APMacAddr,AuthType,GroupCipher,PairCipher,ifSupportPreAuth,AuthSuite,"
		"Passkey1, Passkey2, Passkey3, Passkey4 from recd_SSIDBOOK where SSID=\"%s\" order by no desc limit 0,1;",SSIDName);
		
	qresult=NIF_trySQLiteGetTable(sqlQuery,&queryResult,&row,&col);
	
	if (qresult!=SQLITE_OK)
	{
		//printf("1\n");
		sqlite3_free_table(queryResult);
		return NIF_Status_Unknown;
	} 
	else if ( row < 1 )
	{
		sqlite3_free_table(queryResult);
		return NIF_Status_NotFound;
	} 
	else
	{
		*NumberOfSSID = 1;
		*FoundAPList = calloc(sizeof(T_SSID),1);	
			
		//DB값 Copy.
		printf ("Read Col: %d, Row: %d\n",col,row);
		(*FoundAPList)->name=calloc(strlen(queryResult[col+0])+1,1); strcpy((*FoundAPList)->name,queryResult[col+0]);	//Name;
		(*FoundAPList)->hwAddr=calloc(strlen(queryResult[col+1])+1,1); strcpy((*FoundAPList)->hwAddr,queryResult[col+1]);	//MacAddr;
		(*FoundAPList)->authenticationType=atoi(queryResult[col+2]);	//AuthType
		(*FoundAPList)->groupCipher=atoi(queryResult[col+3]);		//GroupCipher
		(*FoundAPList)->pairCipher=atoi(queryResult[col+4]);	//PairCipher
		(*FoundAPList)->ifSupportPreAuth=atoi(queryResult[col+5]);	//support PreAuth
		(*FoundAPList)->authSuite=atoi(queryResult[col+6]);	//AuthSuite
		
		if (strcmp(queryResult[col+7],"") !=0) { (*FoundAPList)->passKey1=calloc(strlen(queryResult[col+7])+1,1); strcpy((*FoundAPList)->passKey1,queryResult[col+7]);	}//Name; 
		if (strcmp(queryResult[col+8],"") !=0) { (*FoundAPList)->passKey2=calloc(strlen(queryResult[col+8])+1,1); strcpy((*FoundAPList)->passKey2,queryResult[col+8]);	}//Name; 
		if (strcmp(queryResult[col+9],"") !=0) { (*FoundAPList)->passKey3=calloc(strlen(queryResult[col+9])+1,1); strcpy((*FoundAPList)->passKey3,queryResult[col+9]);	}//Name; 
		if (strcmp(queryResult[col+10],"") !=0) { (*FoundAPList)->passKey4=calloc(strlen(queryResult[col+10])+1,1); strcpy((*FoundAPList)->passKey4,queryResult[col+10]);	}//Name; 
		
		sqlite3_free_table(queryResult);
		return NIF_Status_IfUp;
	}
};


int NIF_viewSSIDInformation(int RowOfSSID, T_SSID * FoundAPList)
{
	T_SSID * CurrentItem;
	CurrentItem = FoundAPList+RowOfSSID;	
	
	char buf[1024];
	sprintf (buf, "(NIF) Information of %03d -----------------------------------------------",RowOfSSID);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Name: %s",CurrentItem->name);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID MacAddr: %s",CurrentItem->hwAddr);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Radio Quality: %d",CurrentItem->radioQuality);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Authentification Type: %d",CurrentItem->authenticationType);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Authentify Suite: %d",CurrentItem->authSuite);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID If Hidden SSID? :%d (May not support yet)",CurrentItem->ifHiddenSSID);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID If Support preAuth :%d (May not support yet)",CurrentItem->ifSupportPreAuth);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Group Cipher Method :%d (only for WPA/WPA2)",CurrentItem->groupCipher);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Pair Cipher Method: %d (only for WPA/WPA2)",CurrentItem->pairCipher);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Password String 1: %s",CurrentItem->passKey1);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Password String 2: %s",CurrentItem->passKey2);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Password String 3: %s",CurrentItem->passKey3);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Password String 4: %s",CurrentItem->passKey4);
	//print_log(buf);
	sprintf (buf, "(NIF) SSID Unhandled String: %s",CurrentItem->unknownOption);
	//print_log(buf);
	sprintf (buf, "(NIF) ----------------------------------------------- Information of %03d",RowOfSSID);
	//print_log(buf);
	return NIF_Status_IfUp;
}

int NIF_updateSSIDinDB(int RowOfSSID, T_SSID * FoundAPList)
{
	T_SSID * CurrentItem;
	CurrentItem = FoundAPList+RowOfSSID;	
	char sqlQuery[400] = {0};
	int row = -1;
	int col = -1;
	char **queryResult=NULL;
	int qresult = SQLITE_ERROR;
	
	//snprintf(sqlQuery,sizeof(sqlQuery),"delete from recd_SSIDBOOK where SSID=\"%s\";",CurrentItem->name);
	snprintf(sqlQuery,sizeof(sqlQuery),"delete from recd_SSIDBOOK");
	qresult=NIF_trySQLiteExec(sqlQuery);
	//printf("(NIF) Delete SSID result: %d\n",qresult);
	if (qresult!=SQLITE_OK)	return NIF_Status_NotFound;
	
	char * keyptr1=NULL;
	char * keyptr2=NULL;
	char * keyptr3=NULL;
	char * keyptr4=NULL;
	
	char STRNUL[]="";
	
	if (CurrentItem->passKey1 == NULL) keyptr1=STRNUL; else keyptr1=CurrentItem->passKey1;
	if (CurrentItem->passKey2 == NULL) keyptr2=STRNUL; else keyptr2=CurrentItem->passKey2;
	if (CurrentItem->passKey3 == NULL) keyptr3=STRNUL; else keyptr3=CurrentItem->passKey3;
	if (CurrentItem->passKey4 == NULL) keyptr4=STRNUL; else keyptr4=CurrentItem->passKey4;
	
	snprintf(sqlQuery,sizeof(sqlQuery),
		"insert into recd_SSIDBOOK(\"ifSelected\",\"SSID\",\"APMacAddr\",\"ifHidden\","
		"\"AuthType\",\"GroupCipher\",\"PairCipher\",\"ifSupportPreAuth\",\"AuthSuite\","
		"\"Passkey1\",\"Passkey2\",\"Passkey3\",\"Passkey4\") values "
		"(%d, \"%s\", \"%s\", %d,"
		" %d, %d, %d, %d, %d,"
		" \"%s\",\"%s\",\"%s\",\"%s\");",
		1, CurrentItem->name, CurrentItem->hwAddr, 0,
		CurrentItem->authenticationType, CurrentItem->groupCipher, CurrentItem->pairCipher, CurrentItem->ifSupportPreAuth, CurrentItem->authSuite,
		keyptr1, keyptr2, keyptr3, keyptr4);

	//printf ("sqlQuery:%s\n",sqlQuery);
	qresult=NIF_trySQLiteExec(sqlQuery);
	//printf("(NIF) Insert SSID result: %d\n",qresult);
	if (qresult!=SQLITE_OK)	return NIF_Status_NotFound;
	
	while (1)
	{
		snprintf(sqlQuery,sizeof(sqlQuery),"select NO from recd_SSIDBOOK order by NO asc;");
		qresult=NIF_trySQLiteGetTable(sqlQuery,&queryResult,&row,&col);
		
		
		if (qresult!=SQLITE_OK)	return NIF_Status_NotFound;
		
		if ( DBTRACKINGSSID >= row)
			break;	//Do Nothing
		else if ( row > 0 )
		{
			snprintf(sqlQuery,sizeof(sqlQuery),"delete from recd_SSIDBOOK where NO=%s",queryResult[1]);
			qresult=NIF_trySQLiteExec(sqlQuery);
			//printf("(NIF) Delete SSID result: %d\n",qresult);
			if (qresult!=SQLITE_OK)	{ sqlite3_free_table(queryResult); return NIF_Status_NotFound; }
		}
		
		printf("(NIF) Q.Ctrl - Find SSID result: %d (row=%d)\n",qresult,row);
	}
	sqlite3_free_table(queryResult);
	return NIF_Status_IfUp;
}


int NIF_updateConnStatus_wifi(int IsOnOff){

	int i = 0;
	FILE *fp1 = NULL;
	const int TIME_OUT = 3;
	//TIMEOUT초간 기다리고 : 3초
	int result = NIF_Status_NotFound;

	if(IsOnOff==1){
		for (i=0;i<TIME_OUT;i++){

			char tmplinebuf1[128]; memset(tmplinebuf1,0,sizeof(tmplinebuf1));
			fp1=popen("ifconfig mlan0 | grep 'inet addr'","r");
			DBG_PRINTF_SDH_DEBUG("[%s]:Wait for %d Sec... (get ip)\n",__func__,i);

			if(fp1 == NULL){
				DBG_PRINTF_SDH_ERR("[%s:%d](NIF) Status Not found [1]\n",__func__,__LINE__);
				sleep(1); // Add sleep by mspyo, 2020-11-19
				continue;
			}

			if(fgets(tmplinebuf1,sizeof(tmplinebuf1),fp1) != NULL){
				//UPDATE DATABASE RECD_SSIDBOOK, IFSELECTED = 1;
				char sqlQuery[256] = {0};
				int qresult = SQLITE_ERROR;

				result = NIF_Status_IfUp;
				snprintf(sqlQuery,sizeof(sqlQuery),"update recd_SSIDBOOK set ifSelected=\"%d\";",IsOnOff);
				qresult=NIF_trySQLiteExec(sqlQuery);
				if (qresult!=SQLITE_OK){
					printf("[%s:%d]%s : FAILED!!!",__func__,__LINE__,sqlQuery);
					result = NIF_Status_NotFound; // by mspyo, 2020-11-19
				}
				break;
			}
			else{
				sleep(1);
			}
		}
		if (fp1 != NULL) { // Add to avoid crash by mspyo, 2020-11-19
			pclose(fp1);
		}
	}
	else if(IsOnOff==0){
		char sqlQuery[256];
		int qresult;

		snprintf(sqlQuery,sizeof(sqlQuery),"update recd_SSIDBOOK set ifSelected=\"%d\";",IsOnOff);
		qresult=NIF_trySQLiteExec(sqlQuery);
		if (qresult!=SQLITE_OK){
			printf("[%s:%d]%s : FAILED!!!",__func__,__LINE__,sqlQuery);
			result = NIF_Status_NotFound; // by mspyo, 2020-11-19
		}
	}
	return result;
}

void NIF_turnoffUAP_wifi(){

	char cmd[64];
	char sqlQuery[256];
	int qresult;

	int IsOnOff;
	error_type_t db_retval = FAIL;

	IsOnOff= db_get_var_int("CONF_UAPSTATUS","ifActive",&db_retval);
//	printf("[[[[%d]]]]\n",IsOnOff);
	if(IsOnOff){
		memset(cmd,0x0,64);
		snprintf(cmd,64,"/etc/init.d/stop_uap.sh");
		DBG_PRINTF_SDH_DEBUG("[%s:%d]%s",__func__,__LINE__,cmd);
		system(cmd);

//		printf("IIIIIIIISSSSSSSSSSS[[[[%d]]]]\n",IsOnOff);
		snprintf(sqlQuery,sizeof(sqlQuery),"update conf_uapstatus set ifActive=0;");
		qresult=NIF_trySQLiteExec(sqlQuery);
		if (qresult!=SQLITE_OK){
			printf("[%s:%d]%s : FAILED!!!",__func__,__LINE__,sqlQuery);
		}
	}
}

int NIF_deleteSSIDinDB(int RowOfSSID, T_SSID * FoundAPList)
{
	return NIF_Status_Unknown;
}

int NIF_deleteAllSSIDinDB(void)
{
	char sqlQuery[]="delete from recd_SSIDBOOK;";
	if (NIF_trySQLiteExec(sqlQuery)!=SQLITE_OK)	
		return NIF_Status_Unknown;
	else
		return NIF_Status_IfUp;
}





static int NIF_trySQLiteExec(char *Query)
{
	int db_return;
	char *ErrMsg;
	sqlite3 * NIF_dbInstance = NULL;
	
	if (NIF_DBOpen(&NIF_dbInstance)!=SQLITE_OK)
	{
		return SQLITE_ERROR;
	}
	//SQL문 실행
	while (1)
	{
		db_return=sqlite3_exec (NIF_dbInstance,Query,NULL,NULL,&ErrMsg);
		
		if ( db_return == SQLITE_OK )	
			break;	//부레끼!
		else if ( db_return == SQLITE_ERROR)
		{
			printf ("(NIF) Query Sentence:%s\n",Query);
			printf ("(NIF) Query Error(%s) @%d, ",ErrMsg,__LINE__);
			sqlite3_free(ErrMsg);
			break;
		}
		else
		{
			//printf ("(NIF) Query Error(%s) @%d, ",ErrMsg,__LINE__);
			sqlite3_free(ErrMsg);
			switch (db_return%0x100)
			{
				case SQLITE_BUSY:
					break;
				case SQLITE_LOCKED:
					break;
				case SQLITE_IOERR:
					printf ("Detailed Error Code: %d ",db_return>>8);
					break;
				default:
					break;
			}
			//printf ("try access again\n");
			usleep(5000);
		}
	}
	NIF_DBClose(NIF_dbInstance);
	return db_return;	
}

static int NIF_trySQLiteGetTable(char *Query,char ***QueryResult,int * ResultRow,int * ResultCol)
{
	int db_return;
	char *ErrMsg;
	sqlite3 * NIF_dbInstance = NULL;
	
	//printf("1");
	if (NIF_DBOpen(&NIF_dbInstance)!=SQLITE_OK)
	{
		//printf("1\n");
		return SQLITE_ERROR;
	}
	//printf ("1");
	//SQL문 실행
	while (1) 
	{
		db_return=sqlite3_get_table(NIF_dbInstance,Query,QueryResult,ResultRow,ResultCol,&ErrMsg);
		if ( db_return == SQLITE_OK )	
			break;	//부레끼!
		else if ( db_return == SQLITE_ERROR)
		{
			printf ("(Func P.) Query Sentence:%s\n",Query);
			printf ("(Func P.) Query Error(%s) @%d, ",ErrMsg,__LINE__);
			sqlite3_free(ErrMsg);
			break;
		}
		else
		{
			//printf ("(Func P.) Query Error(%s) @%d, ",ErrMsg,__LINE__);
			sqlite3_free(ErrMsg);
			switch (db_return%0x100)
			{
				case SQLITE_BUSY:
					break;
				case SQLITE_LOCKED:
					break;
				case SQLITE_IOERR:
					printf ("Detailed Error Code: %d ",db_return>>8);
					break;
				default:
					break;
			}
			//printf ("try access again\n");
			usleep(5000);
		}
	}
	NIF_DBClose(NIF_dbInstance);
	return db_return;	
}

static int NIF_DBOpen( sqlite3 ** dbInsPtr )
{	
	int db_return;
	#define DB_REGULAR_FILENAME 	"/app/nvram/karas.db"	
	//printf ("(NIF) SQLite ThreadSafe Check: ");
	db_return=sqlite3_threadsafe();
	if (db_return == 0)
	{
		printf ("NG: Please recompile sqlite3.so (return:%d)\n",db_return);
		return SQLITE_ERROR;
	} 
	//else
		//printf ("OK\n");
	
	FILE *fp;
	fp=fopen(DB_REGULAR_FILENAME,"rb");
	if (fp==NULL)
	{
		printf ("FATAL: Please should reset DB\n");
		//printf ("1\n");
		return SQLITE_ERROR;
	}

	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose (fp);
	
	db_return=sqlite3_open (DB_REGULAR_FILENAME,dbInsPtr);
	if (db_return != SQLITE_OK)
	{
		printf ("(NIF) SQLite open failed:%d\n",db_return);
		return SQLITE_ERROR;
	}
	else
		//printf ("(NIF) SQLite open success\n");
	return SQLITE_OK;
}
static int NIF_DBClose( sqlite3 * dbIns )
{
	if (dbIns != NULL)
	{
		sqlite3_close(dbIns);
		return SQLITE_OK;
	}
	else
		return SQLITE_ERROR;
}

int NIF_pingToHost(T_IPADDRv4 *ipAddr)
{
	#define		PINGCMD		"ping" 
	#define 	PINGOPTION	"-w 5 -c 1 | grep received"
	
	int result=NIF_Status_IfDown;
	FILE * fp;
	
	char cmdBuf[400];	memset(cmdBuf,0,sizeof(cmdBuf));
	snprintf(cmdBuf,sizeof(cmdBuf),"%s %d.%d.%d.%d %s",PINGCMD,ipAddr->ipAddr[0],ipAddr->ipAddr[1],ipAddr->ipAddr[2],ipAddr->ipAddr[3],PINGOPTION);
	printf ("(NIF) Pinging CMD: %s\n",cmdBuf);
	
	fp=popen(cmdBuf,"r");
	sleep(1);
	while(!(feof(fp)))
	{
		memset(cmdBuf,0,sizeof(cmdBuf));
		fgets(cmdBuf,sizeof(cmdBuf),fp);
		//printf ("cmdBuf:%s(%d)\n",cmdBuf,strlen(cmdBuf));
		
		if (strlen(cmdBuf) > 23)
		{
			printf ("(NIF) PingResult: %s",cmdBuf);
			char * token = &cmdBuf[0];
			//char sentBuf[400];
			token=strtok(token," ");
			
			if (token != NULL)
			{
				//printf ("Sent Packet[%s]\n",token);
				token=strtok(NULL," ");
				if (token != NULL)
				{
					//printf ("Delim[%s]\n",token);
					token=strtok(NULL," ");
					if ( (token != NULL) && (strcmp("transmitted,",token) == 0) )
					{
						//printf ("Delim[%s]\n",token);
						token=strtok(NULL," ");
						if (token != NULL)
						{
							//printf ("Recv:%s\n",token);
							//token=strtok(NULL," ");
							int NofRecv=0;
							NofRecv=atoi(token);
							//printf ("...:%d\n",NofRecv);
							if (NofRecv > 0)
								result = NIF_Status_IfUp;
						}
					}
				}
			}
		}
	}
	pclose(fp);	
	
	if (result == NIF_Status_IfUp)
		printf ("(NIF) Pinging Success\n");
	else
		printf ("(NIF) Pinging Fail\n");
	
	return result;
}

int NIF_arpingToHost(T_IPADDRv4 *ipAddr)
{
	#define		ARPINGCMD		"arping" 
	#define 	ARPINGOPTION	"-f -w 5 | grep Received"
	#define     SUCESS_LENGTH_ARPING 48
	
	int result=NIF_Status_IfDown;
	FILE * fp;
	
	char cmdBuf[400];	memset(cmdBuf,0,sizeof(cmdBuf));
	snprintf(cmdBuf,sizeof(cmdBuf),"%s %d.%d.%d.%d %s",ARPINGCMD,ipAddr->ipAddr[0],ipAddr->ipAddr[1],ipAddr->ipAddr[2],ipAddr->ipAddr[3],ARPINGOPTION);
	printf ("(NIF) Arpinging CMD: %s\n",cmdBuf);
	
	fp=popen(cmdBuf,"r");
	while(!(feof(fp)))
	{
		memset(cmdBuf,0,sizeof(cmdBuf));
		fgets(cmdBuf,sizeof(cmdBuf),fp);
		printf ("cmdBuf:%s(%d)\n",cmdBuf,strlen(cmdBuf));
		
		if (strlen(cmdBuf) > SUCESS_LENGTH_ARPING)
		{
			printf ("(NIF) PingResult: %s",cmdBuf);
			char * token = &cmdBuf[0];
			//char sentBuf[400];
			token=strtok(token," ");
			
			if (token != NULL)
			{
				//printf ("Sent Packet[%s]\n",token);
				token=strtok(NULL," ");
				if (token != NULL)
				{
					int NofRecv=0;
					NofRecv=atoi(token);
					//printf ("...:%d\n",NofRecv);
					if (NofRecv > 0)
						result = NIF_Status_IfUp;
				}
			}
		}
	}
	pclose(fp);	
	
	if (result == NIF_Status_IfUp)
		printf ("(NIF) Pinging Success\n");
	else
		printf ("(NIF) Pinging Fail\n");
	
	return result;
}

int NIF_getDNSAddr_int(T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr)
{
	#define DNSSTR		"nameserver"
	if ( (DNS1Addr == NULL) || (DNS2Addr == NULL) )
	{
		printf ("(NIF) Error, Parameter NULL (0x%08x, 0x%08x)\n",(unsigned int)DNS1Addr,(unsigned int)DNS2Addr);
		return NIF_Status_NotFound;
	}
	else
	{
		DNS1Addr->ipAddr_4Byte = 0;
	}		
	FILE *fp = fopen(DNSSERVER_FILENAME,"rt");
	if (fp == NULL)
	{
		printf ("(NIF) Fileopen Error\n");
		return NIF_Status_NotFound;
	}

	T_IPADDRv4 * tmpDnsAddr=NULL;
	
	char tmpStrBuf[200]; 
	char *tmpTok=0;
	int dnsEnum = 1;
	while(!feof(fp))
	{
		memset(tmpStrBuf,0,sizeof(tmpStrBuf));
		fgets(tmpStrBuf,sizeof(tmpStrBuf),fp); printf ("(NIF) ReadStr: %s\n",tmpStrBuf);
		if ( (tmpStrBuf[0] != 0) && (tmpStrBuf[0] != '#') && (strlen (tmpStrBuf) > strlen (DNSSTR)) )
		{
			tmpTok = strtok(tmpStrBuf," \t\n\r");
			printf ("(NIF) DNS 1st QStr:%s\n",tmpTok);
			if (strcmp(tmpTok,DNSSTR) == 0)
			{
				tmpTok = strtok(NULL," \t\n\r");
				printf ("(NIF) DNS 2nd QStr:%s\n",tmpTok);
				
				if (dnsEnum == 1) 
					tmpDnsAddr = DNS1Addr;
				else
					tmpDnsAddr = DNS2Addr;
				
					
				
				tmpTok = strtok(tmpTok,".\t\n\r");tmpDnsAddr->ipAddr[0] = atoi(tmpTok);
				tmpTok = strtok(NULL,".\t\n\r");tmpDnsAddr->ipAddr[1] = atoi(tmpTok);
				tmpTok = strtok(NULL,".\t\n\r");tmpDnsAddr->ipAddr[2] = atoi(tmpTok);
				tmpTok = strtok(NULL,".\t\n\r");tmpDnsAddr->ipAddr[3] = atoi(tmpTok);
				
				printf ("(NIF) Got DNS server (%d): %d.%d.%d.%d\n",dnsEnum,tmpDnsAddr->ipAddr[0],tmpDnsAddr->ipAddr[1],tmpDnsAddr->ipAddr[2],tmpDnsAddr->ipAddr[3]);
				
				dnsEnum++;
				
				if (dnsEnum > 2)
					break;
			}
		}
	}

	if(fflush(fp) != 0){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fflush ic failed!\n",__func__,__LINE__);
	}
	if(fsync(fp->_fileno) == -1){
		DBG_PRINTF_SDH_ERR("[%s:%d:NETWORK]:fsync ic failed!\n",__func__,__LINE__);
	}
	fclose(fp);
	return NIF_Status_IfUp;
}

int NIF_turnMDNS_on(void){

	char cmd[64];
	memset(cmd,0x0,64);
	snprintf(cmd,64,"/etc/init.d/stop_mdns.sh");
	DBG_PRINTF_SDH_DEBUG("%s",cmd);
	os_system(cmd);
	sleep(2);
	memset(cmd,0x0,64);
	snprintf(cmd,64,"/etc/init.d/mdns_start.sh");
	DBG_PRINTF_SDH_DEBUG("%s",cmd);
	os_system(cmd);

	return NIF_Status_IfUp;
}

int NIF_PowerSave_on_wifi(void){
	
	char cmd[64];
	memset(cmd,0x0,64);
	snprintf(cmd,64,"iwconfig mlan0 power on");
	DBG_PRINTF_SDH_ERR("[%s:%d]%s",__func__,__LINE__,cmd);
	system(cmd);

	return NIF_Status_IfUp;
}
int NIF_PowerSave_off_wifi(void){

	char cmd[64];
	memset(cmd,0x0,64);
	snprintf(cmd,64,"iwconfig mlan0 power off");
	DBG_PRINTF_SDH_ERR("[%s:%d]%s",__func__,__LINE__,cmd);
	system(cmd);

	return NIF_Status_IfUp;
}
