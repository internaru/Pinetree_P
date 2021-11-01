#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "NetworkIFModule.h"
#include "agMessage.h"
#include "agRouter.h"
#include "lassert.h"
#include "SysUI_Interface.h"
#ifdef __cplusplus
}
#endif

#include "jsonrpc_network.h"

using namespace std;
using namespace Json;

#define WIRED_IF_NAME	(char *)"eth0"
#define WIFI_IF_NAME	(char *)"ra0"

enum
{
    JSON_WEP = 0,
    JSON_WPA_PSK,
    JSON_WPA2_PSK,
    JSON_OPEN,
    JSON_WPA_EAP,
    JSON_WPA2_EAP
};

//const int BufferLength = 32 * 1024;
//char readBuffer[BufferLength] = {0,};
//char writeBuffer[BufferLength];

string outputConfig;

char jsonrpc_c[] = "jsonrpc";
char method_c[] = "method";
char set_params_c[] = "set params";
char get_params_c[] = "get params";
char params_c[] = "params";

char ftcpip_c[] = "TCPIP";

char ethernet_c[] = "Ethernet";
char wireless_c[] = "Wireless";

char tcpip_ver4_c[] = "TCPIPv4";
char tcpip_ver6_c[] = "TCPIPv6";

char use_dhcp_ip_c[] = "DHCP IP";
char ipaddress1_c[] = "IP Address";
char netmask1_c[] = "Netmask";
char gateway1_c[] = "Gateway";
char use_dhcp_dns_c[] = "DHCP DNS";
char dnsserver1_c[]= "PDNS";
char dnsservera1_c[]= "ADNS";

char wirelessconfig_c[] = "Wireless Config";
char ssid_c[] = "SSID";
char encryption_c[] = "Encryption Method";
char wep_c[] = "WEP";
char wpapsk_c[] = "WPA-PSK";
char wpa2psk_c[] = "WPA2-PSK";
char wpaeap_c[] = "WPA-EAP";
char wpa2eap_c[] = "WPA2-EAP";
char gcipher_c[] = "Gcipher";
char pcipher_c[] = "Pchiper";
char key_c[] = "Key";

char security_c[] = "Security";

char ipfilter_c[] = "IP Filtering";
char ssltls_c[] = "SSL/TLS";
char ieee9021_c[] = "IEEE902.1x";
char ipsec_c[] = "IPSec";
char smtp_c[] = "SMTP-auth";
char wpa_c[] = "WPA/IEEE802.11i";

char services_c[] = "SERVICES";
char ftp_c[] = "FTP";
char snmp_c[] = "SNMP";
char smtp2_c[] = "SMTP";
char cifs_c[] = "CIFS";

char use_c[] = "Use";
char use_ssl_c[] = "Use SSL";
char saddress_c[] ="Server Address";
char id_c[] = "ID";
char pw_c[] = "PW";

char snmp_type_c[] ="Type";
char snmp_community_c[] ="Community";
char snmp_trapcommunity_c[] ="Trap Community";

char smtp_portno_c[] ="Port Number";
char smtp_emai_c[] ="Email Address";

//TCP/IP:
//Ethernet
BOOL tcp_dhcp_ip;
char ipaddr[64];
char netmask[64];
char gateway[64];
BOOL tcp_dhcp_dns;
char dnsserverp[64];
char dnsservera[64];

BOOL tcp_dhcp_ip6;
char ipaddr6[64];
char netmask6[64];
char gateway6[64];
BOOL tcp_dhcp_dns6;
char dnsserverp6[64];
char dnsservera6[64];

BOOL w_dhcp_ip;
char wipaddr[64];
char wnetmask[64];
char wgateway[64];
BOOL w_dhcp_dns;
char wdnsserver[64];
char wdnsservera[64];

BOOL w_dhcp_ip6;
char wipaddr6[64];
char wnetmask6[64];
char wgateway6[64];
BOOL w_dhcp_dns6;
char wdnsserver6[64];
char wdnsservera6[64];

//wireless configuration
char ssid[64];
int encryption;
char wep_key[128]; 
char wpa_key[128];
char wpa2_key[128];
char wpaeap_id[128];
char wpaeap_pw[128];
char wpa2eap_id[128];
char wpa2eap_pw[128];
BOOL wpa_gc;
BOOL wpa_pc;
BOOL wpa2_gc;
BOOL wpa2_pc;

//Security
BOOL ipfilter = FALSE;
BOOL ssl = FALSE;
BOOL ieee = FALSE;
BOOL ipsec = FALSE;
BOOL smtp = FALSE;
BOOL wpa = FALSE;

//FTP:
BOOL ftp_use;
BOOL ftp_ssl;
char ftp_saddress[40];
char ftp_id[40];
char ftp_pw[20];

//SNMP:
BOOL snmp_use;
BOOL snmp_type;
char snmp_saddress[40];
char snmp_community[40];
char snmp_trapcommunity[40];

//SMTP:
BOOL smtp_use;
BOOL smtp_ssl;
char smtp_saddress[40];
char smtp_portno[40];
char smtp_email[40];

//cifs:
BOOL cifs_use;
char cifs_saddress[40];
char cifs_id[40];
char cifs_pw[20];

int loadSettings()
{
    T_IPADDRv4 dns1;
    T_IPADDRv4 dns2;
    char *temp;
//    char *temp2;

    tcp_dhcp_ip = (NIF_getDHCPStatus_wired() == NIF_Status_Static) ? FALSE : TRUE;
    strcpy(ipaddr, "");
    strcpy(netmask, "");
    strcpy(gateway, "");
    if (NIF_getIpAddrV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(ipaddr, temp);
    free(temp);
    if (NIF_getNetmaskV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(netmask, temp);
    free(temp);
    if (NIF_getGatewayV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(gateway, temp);
    free(temp);
    tcp_dhcp_dns = tcp_dhcp_ip;
    if (tcp_dhcp_dns)
    {
        strcpy(dnsserverp, "");
        strcpy(dnsservera, "");
    }
    else
    {
        NIF_getDNSAddr_int(&dns1,&dns2);
        inet_ntop(AF_INET, &dns1, dnsserverp, 64);  
        inet_ntop(AF_INET, &dns2, dnsservera, 64);
    }
    
    
    tcp_dhcp_ip6 = TRUE;
    strcpy(ipaddr6, "");
    strcpy(netmask6, "");
    strcpy(gateway6, "");
    tcp_dhcp_dns6 = TRUE; //tcp_dhcp_ip6;
    strcpy(dnsserverp6, "");
    strcpy(dnsservera6, "");
    if (NIF_getIPAddrV6_str_wired(&temp) == NIF_Status_IfUp) strcpy(ipaddr6, temp);
    free(temp);
    
    w_dhcp_ip = (NIF_getDHCPStatus_wifi() == NIF_Status_Static) ? FALSE : TRUE;
    strcpy(wipaddr, "");
    strcpy(wnetmask, "");
    strcpy(wgateway, "");
    w_dhcp_dns = tcp_dhcp_ip6; //w_dhcp_ip;
    strcpy(wdnsserver, "");
    strcpy(wdnsservera, "");
    if (NIF_getIpAddrV4FromFile(WIFI_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(wipaddr, temp);
    free(temp);
    if (NIF_getNetmaskV4FromFile(WIFI_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(wnetmask, temp);
    free(temp);
    if (NIF_getGatewayV4FromFile(WIFI_IF_NAME, &temp) == NIF_Status_IfUp) strcpy(wgateway, temp);
    free(temp);
    if (w_dhcp_dns)
    {
        strcpy(wdnsserver, "");
        strcpy(wdnsservera, "");
    }
    else
    {
        NIF_getDNSAddr_int(&dns1,&dns2);
        inet_ntop(AF_INET, &dns1, wdnsserver, 64);  
        inet_ntop(AF_INET, &dns2, wdnsservera, 64);
    }
    
    w_dhcp_ip6 = TRUE;
    strcpy(wipaddr6, "");
    strcpy(wnetmask6, "");
    strcpy(wgateway6, "");
    if (NIF_getIPAddrV6_str_wired(&temp) == NIF_Status_IfUp) strcpy(ipaddr6, temp);
    free(temp);
    w_dhcp_dns6 = TRUE;
    strcpy(wdnsserver6, "");
    strcpy(wdnsservera6, "");
    
    //wireless configuration
    T_SSID * ptrAPList;
    int NumberOfSSID;
    strcpy(ssid, "");
    encryption = 3;
    strcpy(wep_key, "");
    strcpy(wpa_key, "");
    strcpy(wpa2_key, "");
    strcpy(wpaeap_id, "");
    strcpy(wpaeap_pw, "");
    strcpy(wpa2eap_id, "");
    strcpy(wpa2eap_pw, "");

    wpa_gc = 0;
    wpa_pc = 0;
    wpa2_gc = 0;
    wpa2_pc = 0;

    if (NIF_getOneSSIDinDB(&NumberOfSSID,&ptrAPList) == NIF_Status_IfUp)
    {
        strcpy(ssid, ptrAPList->name);
        if (ptrAPList->authenticationType == NIF_AUTH_WEP)
        {
            encryption = JSON_WEP;
            strcpy(wep_key, ptrAPList->passKey1);
        }
        else if (ptrAPList->authenticationType == NIF_AUTH_WPAPSK)
        {
            encryption = JSON_WPA_PSK;
            strcpy(wpa_key, ptrAPList->passKey1);
            wpa_gc = ptrAPList->groupCipher - 1;
            wpa_pc = ptrAPList->pairCipher - 1;
        }
        else if (ptrAPList->authenticationType == NIF_AUTH_WPA2PSK || ptrAPList->authenticationType == NIF_AUTH_WPAPSK_WPA2PSK)
        {
            encryption = JSON_WPA2_PSK;
            strcpy(wpa2_key, ptrAPList->passKey1);
            wpa2_gc = ptrAPList->groupCipher - 1;
            wpa2_pc = ptrAPList->pairCipher - 1;
        }
        else encryption = JSON_OPEN;
//      if (ptrAPList->passKey1 != NULL)
//      {
//          strcpy(wep_key, ptrAPList->passKey1);
//          strcpy(wpa_key, ptrAPList->passKey1);
//          strcpy(wpa2_key, ptrAPList->passKey1);
//      }
        
        NIF_freeSSID(NumberOfSSID,ptrAPList);
    }

    //Security
    ipfilter = FALSE;
    ssl = FALSE;
    ieee = FALSE;
    ipsec = FALSE;
    smtp = FALSE;
    wpa = FALSE;
    
    //FTP:
    ftp_use = TRUE;
    ftp_ssl = FALSE;
    strcpy(ftp_saddress, "");
    strcpy(ftp_id, "");
    strcpy(ftp_pw, "");
    
    //SNMP:
    snmp_use = TRUE;
    snmp_type = FALSE;
    strcpy(snmp_saddress, "");
    strcpy(snmp_community, "");
    strcpy(snmp_trapcommunity, "");
    
    //SMTP:
    smtp_use = TRUE;
    smtp_ssl = FALSE;
    strcpy(smtp_saddress, "");
    strcpy(smtp_portno, "");
    strcpy(smtp_email, "");
    
    //cifs:
    cifs_use = FALSE;
    strcpy(cifs_saddress, "");
    strcpy(cifs_id, "");
    strcpy(cifs_pw, "");

    return 0;
}

int saveSettings()
{
    T_IPADDRv4 ip_address;
    T_IPADDRv4 ip_netmask;
    T_IPADDRv4 ip_gateway;
    T_IPADDRv4 ip_dns1;
    T_IPADDRv4 ip_dns2;
    T_IPADDRv4 dns1;
    T_IPADDRv4 dns2;
    char *temp; 

    error_type_t err;
    //stNetwork_Request stNetwork;
    SYS_UI_MESSAGE msgToSend;
    //bool msgSendFlag = true;

    if (tcp_dhcp_ip)
    {
        NIF_setDHCPDynamic_wired();
    }
    else
    {
        inet_pton(AF_INET, ipaddr, (void *)&ip_address);
        inet_pton(AF_INET, netmask, (void *)&ip_netmask);
        inet_pton(AF_INET, gateway, (void *)&ip_gateway);
        inet_pton(AF_INET, dnsserverp, (void *)&ip_dns1);
        inet_pton(AF_INET, dnsservera, (void *)&ip_dns2);
        NIF_setDHCPStatic_wired(&ip_address, &ip_netmask, &ip_gateway, &ip_dns1, &ip_dns2);
    }
    msgToSend.msgSender = WEB_MONITOR_ID; 
    msgToSend.cmd = CMD_US_CONFIG_CHANGED;    //Network Response
    memset(&msgToSend.data, 0, SYS_UI_MESSAGE_DATA_LEN);
    
    err = SysUI_MsgSend(SYSTEM_SERVICE_RECEIVER_ID, &msgToSend);
    XASSERT( err==OK, err );
    if (!tcp_dhcp_ip) 
    {
        printf("[JSON_NET] Static!! Make config files!\n");
        FILE *setstatic, *fp1, *fp2, *fp3, *fp4, *fp5;
        setstatic = fopen("/etc/network/SETUP_STATIC","w+");
        fp1 = fopen("/etc/network/SETUP_IP","w+");
        fp2 = fopen("/etc/network/SETUP_MASK","w+");
        fp3 = fopen("/etc/network/SETUP_GATE","w+");
        fp4 = fopen("/etc/network/SETUP_DNS1","w+");
        fp5 = fopen("/etc/network/SETUP_DNS2","w+");
      
        if (NIF_getIpAddrV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp)
            inet_pton(AF_INET, temp, (void *)&ip_address);
        free(temp);
        if (NIF_getNetmaskV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp)
            inet_pton(AF_INET, temp, (void *)&ip_netmask);
        free(temp);
        if (NIF_getGatewayV4FromFile(WIRED_IF_NAME, &temp) == NIF_Status_IfUp)
            inet_pton(AF_INET, temp, (void *)&ip_gateway);
        free(temp);
    
        //sprintf(ip,"%03d.%03d.%03d.%03d",ipAddr.ipAddr[0],ipAddr.ipAddr[1],ipAddr.ipAddr[2],ipAddr.ipAddr[3]);

        NIF_getDNSAddr_int(&dns1,&dns2);
        //inet_ntop(AF_INET, &dns1, dnsserverp, 64);  
        //inet_ntop(AF_INET, &dns2, dnsservera, 64);

        fprintf(setstatic,"%03d.%03d.%03d.%03d\n",ip_address.ipAddr[0],ip_address.ipAddr[1],ip_address.ipAddr[2],ip_address.ipAddr[3]);
        fprintf(fp1,"%03d.%03d.%03d.%03d\n",ip_address.ipAddr[0],ip_address.ipAddr[1],ip_address.ipAddr[2],ip_address.ipAddr[3]);
        fprintf(setstatic,"%03d.%03d.%03d.%03d\n",ip_netmask.ipAddr[0],ip_netmask.ipAddr[1],ip_netmask.ipAddr[2],ip_netmask.ipAddr[3]);
        fprintf(fp2,"%03d.%03d.%03d.%03d\n",ip_netmask.ipAddr[0],ip_netmask.ipAddr[1],ip_netmask.ipAddr[2],ip_netmask.ipAddr[3]);
        fprintf(setstatic,"%03d.%03d.%03d.%03d\n",ip_gateway.ipAddr[0],ip_gateway.ipAddr[1],ip_gateway.ipAddr[2],ip_gateway.ipAddr[3]);
        fprintf(fp3,"%03d.%03d.%03d.%03d\n",ip_gateway.ipAddr[0],ip_gateway.ipAddr[1],ip_gateway.ipAddr[2],ip_gateway.ipAddr[3]);
        fprintf(setstatic,"%03d.%03d.%03d.%03d\n",dns1.ipAddr[0],dns1.ipAddr[1],dns1.ipAddr[2],dns1.ipAddr[3]);
        fprintf(fp4,"%03d.%03d.%03d.%03d\n",dns1.ipAddr[0],dns1.ipAddr[1],dns1.ipAddr[2],dns1.ipAddr[3]);
        fprintf(setstatic,"%03d.%03d.%03d.%03d\n",dns2.ipAddr[0],dns2.ipAddr[1],dns2.ipAddr[2],dns2.ipAddr[3]);
        fprintf(fp5,"%03d.%03d.%03d.%03d\n",dns2.ipAddr[0],dns2.ipAddr[1],dns2.ipAddr[2],dns2.ipAddr[3]);
        fclose(setstatic);
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
        fclose(fp4);
        fclose(fp5);
    }
//  tcp_dhcp_dns;
//  dnsserverp;
//  dnsservera;
    
//  tcp_dhcp_ip6;
//  ipaddr6;
//  netmask6;
//  gateway6;
//  tcp_dhcp_dns6;
//  dnsserverp6;
//  dnsservera6;
    /*
    if (w_dhcp_ip)
    {
        NIF_setDHCPDynamic_wifi();
    }
    else
    {
        inet_pton(AF_INET, wipaddr, (void *)&ip_address);
        inet_pton(AF_INET, wnetmask, (void *)&ip_netmask);
        inet_pton(AF_INET, wgateway, (void *)&ip_gateway);
        inet_pton(AF_INET, wdnsserver, (void *)&ip_dns1);
        inet_pton(AF_INET, wdnsservera, (void *)&ip_dns2);
        NIF_setDHCPStatic_wifi(&ip_address, &ip_netmask, &ip_gateway, &ip_dns1, &ip_dns2);
    }
//  w_dhcp_dns;
//  wdnsserver;
//  wdnsservera;

    //wireless configuration
    if (strlen(ssid) > 0)
    {
        T_SSID SSIDList;
        SSIDList.name = ssid;
        if (encryption == JSON_WEP)
        {
            SSIDList.authenticationType = NIF_AUTH_WEP;
            SSIDList.passKey1 = wep_key;
        }
        else if (encryption == JSON_WPA_PSK)
        {
            SSIDList.authenticationType = NIF_AUTH_WPAPSK;
            SSIDList.passKey1 = wpa_key;
            SSIDList.groupCipher = wpa_gc + 1;
            SSIDList.pairCipher = wpa_pc + 1;
        }
        else if (encryption == JSON_WPA2_PSK)
        {
            SSIDList.authenticationType = NIF_AUTH_WPA2PSK;
            SSIDList.passKey1 = wpa2_key;
            SSIDList.groupCipher = wpa2_gc + 1;
            SSIDList.pairCipher = wpa2_pc + 1;
        }
        else if (encryption == JSON_OPEN)
        {
            SSIDList.authenticationType = NIF_AUTH_NOSECURE;
        }
        else
        {
            SSIDList.authenticationType = NIF_AUTH_NOSECURE;
            //JSON_WPA_EAP
            //JSON_WPA2_EAP
        }
    
        //  wpa_gc;
        //  wpa_pc;
        //  wpa2_gc;
        //  wpa2_pc;
        NIF_updateSSIDinDB(0, &SSIDList);
        if (NIF_getStatus_wifi() == NIF_Status_IfUp)
        {
            NIF_connectSSID(0, &SSIDList);
            for (int i = 1; i < 10; i++)
            {
                if (NIF_connectStatus_wifi() == NIF_Status_IfUp) break;
                printf("--jeseo-- trying to connect wifi\n");
                sleep(1);
            }
            if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
            {
                printf ("(Boot Wifi) Connection Established, Setup IP Address\n\n");
                NIF_autoIF_Wifi();			
            }
            else
            {
                printf("--jeseo-- wifi connect error\n");
            }
        }
    }
    */
    
    
//  w_dhcp_ip6;
//  wipaddr6;
//  wnetmask6;
//  wgateway6;
//  w_dhcp_dns6;
//  wdnsserver6;
//  wdnsservera6;
    
    //Security
//  ipfilter = FALSE;
//  ssl = FALSE;
//  ieee = FALSE;
//  ipsec = FALSE;
//  smtp = FALSE;
//  wpa = FALSE;
    
    //FTP:
//  ftp_use;
//  ftp_ssl;
//  ftp_saddress[40];
//  ftp_id[40];
//  ftp_pw[20];
    
    //SNMP:
//  snmp_use;
//  snmp_type;
//  snmp_saddress[40];
//  snmp_community[40];
//  snmp_trapcommunity[40];
    
    //SMTP:
//  smtp_use;
//  smtp_ssl;
//  smtp_saddress[40];
//  smtp_portno[40];
//  smtp_email[40];
    
    //cifs:
//  cifs_use;
//  cifs_saddress[40];
//  cifs_id[40];
//  cifs_pw[20];

    return 0;
}

void networkGetParams(Value *root, Value *resultRoot)
{
    Value Params;
    Value TCPIP;
    Value Ethernet;
    Value Wireless;
    Value WirelessConfig;
    Value TCPIPv4;
    Value TCPIPv6;
    Value WTCPIPv4;
    Value WTCPIPv6;
    Value Wep;
    Value Wpa;
    Value Wpa2;
    Value Wpaeap;
    Value Wpa2eap;
    Value Security;
    Value Service;
    Value Ftp;
    Value Snmp;
    Value Smtp;
    Value Cifs;

    loadSettings();
    
    //TCPIP
    //Ethernet

    TCPIPv4[use_dhcp_ip_c] = tcp_dhcp_ip;
    TCPIPv4[ipaddress1_c] = ipaddr;
    TCPIPv4[netmask1_c] = netmask;
    TCPIPv4[gateway1_c] = gateway;
    TCPIPv4[use_dhcp_dns_c] = tcp_dhcp_dns;
    TCPIPv4[dnsserver1_c] = dnsserverp;
    TCPIPv4[dnsservera1_c] = dnsservera;
    Ethernet[tcpip_ver4_c] = TCPIPv4;

    TCPIPv6[use_dhcp_ip_c] = tcp_dhcp_ip6;
    TCPIPv6[ipaddress1_c] = ipaddr6;
    TCPIPv6[netmask1_c] = netmask6;
    TCPIPv6[gateway1_c] = gateway6;
    TCPIPv6[use_dhcp_dns_c] = tcp_dhcp_dns6;
    TCPIPv6[dnsserver1_c] = dnsserverp6;
    TCPIPv6[dnsservera1_c] = dnsservera6;
    Ethernet[tcpip_ver6_c] = TCPIPv6;

    TCPIP[ethernet_c] = Ethernet;
    Params[ftcpip_c] = TCPIP;

    //Wireless
//  	GetWindowText(GetDlgItem(hwireless,WL_STATIC_IPV4_ADDRESS),(LPSTR)wipaddr,64);
//  	GetWindowText(GetDlgItem(hwireless,WL_STATIC_IPV4_NETMASK),(LPSTR)wnetmask,64);
//  	GetWindowText(GetDlgItem(hwireless,WL_STATIC_IPV4_GATEWAY),(LPSTR)wgateway,64);
//  	GetWindowText(GetDlgItem(hwireless,WL_STATIC_IPV4_DNS),(LPSTR)wdnsserver,64);
//  	GetWindowText(GetDlgItem(hwireless,WL_STATIC_IPV4_DNS2),(LPSTR)wdnsservera,64);

    WTCPIPv4[use_dhcp_ip_c] = w_dhcp_ip;
    WTCPIPv4[ipaddress1_c] = wipaddr;
    WTCPIPv4[netmask1_c] = wnetmask;
    WTCPIPv4[gateway1_c] = wgateway;
    WTCPIPv4[use_dhcp_dns_c] = w_dhcp_dns;
    WTCPIPv4[dnsserver1_c] = wdnsserver;
    WTCPIPv4[dnsservera1_c] = wdnsservera;
    Wireless[tcpip_ver4_c] = WTCPIPv4;

    WTCPIPv6[use_dhcp_ip_c] = w_dhcp_ip6;
    WTCPIPv6[ipaddress1_c] = wipaddr6;
    WTCPIPv6[netmask1_c] = wnetmask6;
    WTCPIPv6[gateway1_c] = wgateway6;
    WTCPIPv6[use_dhcp_dns_c] = w_dhcp_dns6;
    WTCPIPv6[dnsserver1_c] = wdnsserver6;
    WTCPIPv6[dnsservera1_c] = wdnsservera6;
    Wireless[tcpip_ver6_c] = WTCPIPv6;

    TCPIP[wireless_c] = Wireless;
    Params[ftcpip_c] = TCPIP;

    //Wireless Config
//  	GetWindowText(GetDlgItem(hwireless,WL_SSID_EDIT),(LPSTR)ssid,64);

    WirelessConfig[ssid_c] = ssid;
    WirelessConfig[encryption_c] = encryption;

    Wep[key_c] = wep_key;
    WirelessConfig[wep_c] = Wep;

    Wpaeap[id_c] = wpaeap_id;
    Wpaeap[pw_c] = wpaeap_pw;
    WirelessConfig[wpaeap_c] = Wpaeap;

    Wpa2eap[id_c] = wpa2eap_id;
    Wpa2eap[pw_c] = wpa2eap_pw;
    WirelessConfig[wpa2eap_c] = Wpa2eap;

    Wpa[gcipher_c] = wpa_gc;
    Wpa[pcipher_c] = wpa_pc;
    Wpa[key_c] = wpa_key;
    WirelessConfig[wpapsk_c] = Wpa;

    Wpa2[gcipher_c] = wpa2_gc;
    Wpa2[pcipher_c] = wpa2_pc;
    Wpa2[key_c] = wpa2_key;
    WirelessConfig[wpa2psk_c] = Wpa2;

    TCPIP[wirelessconfig_c] = WirelessConfig;
    Params[ftcpip_c] = TCPIP;

    //Security
    Security[ipfilter_c] = ipfilter;
    Security[ssltls_c] = ssl;
    Security[ieee9021_c] = ieee;
    Security[ipsec_c] = ipsec;
    Security[smtp_c] = smtp;
    Security[wpa_c] = wpa;

    Params[security_c] = Security;

    //Service
    //FTP
//  	GetWindowText(GetDlgItem(hftp,IDC_FTP_SADDRESS),(LPSTR)ftp_saddress,64);
//  	GetWindowText(GetDlgItem(hftp,IDC_FTP_ID),(LPSTR)ftp_id,64);
//  	GetWindowText(GetDlgItem(hftp,IDC_FTP_PW),(LPSTR)ftp_pw,64);

    Ftp[use_c] = ftp_use;
    Ftp[use_ssl_c] = ftp_ssl;
    Ftp[saddress_c] = ftp_saddress;
    Ftp[id_c] = ftp_id;
    Ftp[pw_c] = ftp_pw;

    Service[ftp_c] = Ftp;
    Params[services_c] = Service;

    //SNMP
//  	GetWindowText(GetDlgItem(hsnmp,IDC_SNMP_SADDRESS),(LPSTR)snmp_saddress,64);
//  	GetWindowText(GetDlgItem(hsnmp,IDC_SNMP_COMMUNITY),(LPSTR)snmp_community,64);
//  	GetWindowText(GetDlgItem(hsnmp,IDC_SNMP_COMMUNITY2),(LPSTR)snmp_trapcommunity,64);

    Snmp[use_c] = snmp_use;
    Snmp[snmp_type_c] = snmp_type;
    Snmp[saddress_c] = snmp_saddress;
    Snmp[snmp_community_c] = snmp_community;
    Snmp[snmp_trapcommunity_c] = snmp_trapcommunity;

    Service[snmp_c] = Snmp;
    Params[services_c] = Service;

    //SMTP
//  	GetWindowText(GetDlgItem(hsmtp,IDC_SMTP_SADDRESS),(LPSTR)smtp_saddress,64);
//  	GetWindowText(GetDlgItem(hsmtp,IDC_SMTP_PORTNO),(LPSTR)smtp_portno,64);
//  	GetWindowText(GetDlgItem(hsmtp,IDC_SMTP_EMAIL),(LPSTR)smtp_email,64);

    Smtp[use_c] = smtp_use;
    Smtp[use_ssl_c] = smtp_ssl;
    Smtp[saddress_c] = smtp_saddress;
    Smtp[smtp_portno_c] = smtp_portno;
    Smtp[smtp_emai_c] = smtp_email;

    Service[smtp2_c] = Smtp;
    Params[services_c] = Service;

    //CIFS
//  	GetWindowText(GetDlgItem(hcifs,IDC_CIFS_SADDRESS),(LPSTR)cifs_saddress,64);
//  	GetWindowText(GetDlgItem(hcifs,IDC_CIFS_ID),(LPSTR)cifs_id,64);
//  	GetWindowText(GetDlgItem(hcifs,IDC_CIFS_PW),(LPSTR)cifs_pw,64);

    Cifs[use_c] = cifs_use;
    Cifs[saddress_c] = cifs_saddress;
    Cifs[id_c] = cifs_id;
    Cifs[pw_c] = cifs_pw;

    Service[cifs_c] = Cifs;
    Params[services_c] = Service;

    (*resultRoot)["result"] = Params;
}

void networkSetParams(Value *root, Value *resultRoot)
{
    //TCPIP-Ethernet
    tcp_dhcp_ip = (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c][use_dhcp_ip_c].asBool();
    strcpy(ipaddr, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c].get(ipaddress1_c,"").asCString());
    strcpy(netmask, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c].get(netmask1_c,"").asCString());
    strcpy(gateway, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c].get(gateway1_c,"").asCString());
    tcp_dhcp_dns = (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c][use_dhcp_dns_c].asBool();
    strcpy(dnsserverp, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c].get(dnsserver1_c,"").asCString());
    strcpy(dnsservera, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver4_c].get(dnsservera1_c,"").asCString());
    printf("[JSON_NET] Check tcp_dhcp_ip:%d, ipaddr:%s, netmask:%s, gateway:%s, tcp_dhcp_dns:%d, dnsserverp:%s, dnsservera:%s\n", tcp_dhcp_ip, ipaddr, netmask, gateway, tcp_dhcp_dns, dnsserverp, dnsservera);

    tcp_dhcp_ip6 = (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c][use_dhcp_ip_c].asBool();
    strcpy(ipaddr6, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c].get(ipaddress1_c,"").asCString());
    strcpy(netmask6, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c].get(netmask1_c,"").asCString());
    strcpy(gateway6, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c].get(gateway1_c,"").asCString());
    tcp_dhcp_dns6 = (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c][use_dhcp_dns_c].asBool();
    strcpy(dnsserverp6, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c].get(dnsserver1_c,"").asCString());
    strcpy(dnsservera6, (*root)[params_c][ftcpip_c][ethernet_c][tcpip_ver6_c].get(dnsservera1_c,"").asCString());

    //TCPIP-Wireless
    w_dhcp_ip = (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c][use_dhcp_ip_c].asBool();
    strcpy(wipaddr, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c].get(ipaddress1_c,"").asCString());
    strcpy(wnetmask, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c].get(netmask1_c,"").asCString());
    strcpy(wgateway, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c].get(gateway1_c,"").asCString());
    w_dhcp_dns = (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c][use_dhcp_dns_c].asBool();
    strcpy(wdnsserver, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c].get(dnsserver1_c,"").asCString());
    strcpy(wdnsservera, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver4_c].get(dnsservera1_c,"").asCString());

    w_dhcp_ip6 = (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c][use_dhcp_ip_c].asBool();
    strcpy(wipaddr6, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c].get(ipaddress1_c,"").asCString());
    strcpy(wnetmask6, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c].get(netmask1_c,"").asCString());
    strcpy(wgateway6, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c].get(gateway1_c,"").asCString());
    w_dhcp_dns6 = (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c][use_dhcp_dns_c].asBool();
    strcpy(wdnsserver6, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c].get(dnsserver1_c,"").asCString());
    strcpy(wdnsservera6, (*root)[params_c][ftcpip_c][wireless_c][tcpip_ver6_c].get(dnsservera1_c,"").asCString());

    //TCPIP-WirelessConfig
    strcpy(ssid, (*root)[params_c][ftcpip_c][wirelessconfig_c].get(ssid_c,"").asCString());
    encryption = (*root)[params_c][ftcpip_c][wirelessconfig_c][encryption_c].asInt();
    strcpy(wep_key, (*root)[params_c][ftcpip_c][wirelessconfig_c][wep_c].get(key_c,"").asCString());
    wpa_gc = (*root)[params_c][ftcpip_c][wirelessconfig_c][wpapsk_c][gcipher_c].asBool();
    wpa_pc = (*root)[params_c][ftcpip_c][wirelessconfig_c][wpapsk_c][pcipher_c].asBool();
    strcpy(wpa_key, (*root)[params_c][ftcpip_c][wirelessconfig_c][wpapsk_c].get(key_c,"").asCString());
    wpa2_gc = (*root)[params_c][ftcpip_c][wirelessconfig_c][wpa2psk_c][gcipher_c].asBool();
    wpa2_pc = (*root)[params_c][ftcpip_c][wirelessconfig_c][wpa2psk_c][pcipher_c].asBool();
    strcpy(wpa2_key, (*root)[params_c][ftcpip_c][wirelessconfig_c][wpa2psk_c].get(key_c,"").asCString());

    //Security
    ipfilter = (*root)[params_c][security_c][ipfilter_c].asBool();
    ssl = (*root)[params_c][security_c][ssltls_c].asBool();
    ieee = (*root)[params_c][security_c][ieee9021_c].asBool();
    ipsec = (*root)[params_c][security_c][ipsec_c].asBool();
    smtp = (*root)[params_c][security_c][smtp_c].asBool();
    wpa = (*root)[params_c][security_c][wpa_c].asBool();

    //Service
    //FTP
    ftp_use = (*root)[params_c][services_c][ftp_c][use_c].asBool();
    ftp_ssl = (*root)[params_c][services_c][ftp_c][use_ssl_c].asBool();
    strcpy(ftp_saddress, (*root)[params_c][services_c][ftp_c].get(saddress_c,"").asCString());
    strcpy(ftp_id, (*root)[params_c][services_c][ftp_c].get(id_c,"").asCString());
    strcpy(ftp_pw, (*root)[params_c][services_c][ftp_c].get(pw_c,"").asCString());

    //SNMP
    snmp_use = (*root)[params_c][services_c][snmp_c][use_c].asBool();
    snmp_type = (*root)[params_c][services_c][snmp_c][use_ssl_c].asBool();
    strcpy(snmp_saddress, (*root)[params_c][services_c][snmp_c].get(saddress_c,"").asCString());
    strcpy(snmp_community, (*root)[params_c][services_c][snmp_c].get(snmp_community_c,"").asCString());
    strcpy(snmp_trapcommunity, (*root)[params_c][services_c][snmp_c].get(snmp_trapcommunity_c,"").asCString());

    //SMTP
    smtp_use = (*root)[params_c][services_c][smtp2_c][use_c].asBool();
    smtp_ssl = (*root)[params_c][services_c][smtp2_c][use_ssl_c].asBool();
    strcpy(smtp_saddress, (*root)[params_c][services_c][smtp2_c].get(saddress_c,"").asCString());
    strcpy(smtp_portno, (*root)[params_c][services_c][smtp2_c].get(smtp_portno_c,"").asCString());
    strcpy(smtp_email, (*root)[params_c][services_c][smtp2_c].get(smtp_emai_c,"").asCString());

    //CIFS
    cifs_use = (*root)[params_c][services_c][cifs_c][use_c].asBool();
    strcpy(cifs_saddress, (*root)[params_c][services_c][cifs_c].get(saddress_c,"").asCString());
    strcpy(cifs_id, (*root)[params_c][services_c][cifs_c].get(id_c,"").asCString());
    strcpy(cifs_pw, (*root)[params_c][services_c][cifs_c].get(pw_c,"").asCString());

    saveSettings();
    (*resultRoot)["result"] = "";
}
