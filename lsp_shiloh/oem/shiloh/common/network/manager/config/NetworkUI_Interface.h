#ifndef _NETWORKUIINTERFACE_H_
#define _NETWORKUIINTERFACE_H_

#define	SDH_NETUI_VERSION	"1.0.1"	

/*
*/
int NUI_getIPAddrV4_wired();

/*
*/
int NUI_getIPAddrV6_wired();

/*
*/
int NUI_getMacAddr_wired();

/*
*/
int NUI_setDHCPDynamic_wired();

/*
*/
int NUI_setDHCPStatic_wired();

/*
*/
int NUI_getIPAddrV4_wifi();

/*
*/
int NUI_getIPAddrV6_wifi();

/*
*/
int NUI_getMacAddr_wifi();

/*
*/
int NUI_getSSIDList_wifi();

/*
*/
int NUI_setDHCPDynamic_wifi();

/*
*/
int NUI_setDHCPStatic_wifi();

/*
*/
int NUI_ConnectOn_wifi();

/*
*/
int NUI_ConnectOff_wifi();

/*
*/
int NUI_ConnectSSID_wifi();

/*
*/
int NUI_ClearSSID_wifi();


#endif
