#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "jsonrpc.h"

#ifdef __cplusplus
extern "C" {
#endif

//#include "../../Component/NetworkIFModuleLib/NetworkIFModule.h"

#ifdef __cplusplus
}
#endif

int main()
{
//  parse_jsonrpc("hi");
//  T_IPADDRv4 ip_address;
//
//  NIF_getIPAddrV4_int_wired(&ip_address);
//  printf ("wifi IP Addr: %d.%d.%d.%d\n",ip_address.ipAddr[0],ip_address.ipAddr[1],ip_address.ipAddr[2],ip_address.ipAddr[3]);
//
//  char addr_string[64];
//  inet_ntop(AF_INET, &ip_address, addr_string, sizeof(addr_string));
//  printf("converted %s\n", addr_string);
//
//  inet_pton(AF_INET, "192.168.0.224", (void *)&ip_address);
//  printf("%lu\n", ip_address);

    //NIF_setDHCPStatic_wired(T_IPADDRv4 *ipAddr, T_IPADDRv4 *netMask, T_IPADDRv4 *gateWay, T_IPADDRv4 *DNS1Addr, T_IPADDRv4 *DNS2Addr);
    //int inet_aton(int af, const char *src, void *dst);

    char command[32*1024];
    char response[32*1024];

    ReadFromFile("/home/get_params.json", command, 32*1024);
    ParseJSONRPC(command, response);

    ReadFromFile("/home/set_params.json", command, 32*1024);
    ParseJSONRPC(command, response);

//  T_SSID SSIDList;
//  memset(&SSIDList, 0, sizeof(SSIDList));
//  SSIDList.name = "iptime";
//  SSIDList.authenticationType = NIF_AUTH_NOSECURE;
//  NIF_connectSSID(0, &SSIDList);

    return 0;
}

//int main()
//{
//    void *setting = NULL;
//    char value[1024];
//    setting = openSetting("test.json");
//    readSetting(setting, "encoding", value);
//    printf("result: %s\n", value);
//    readSetting(setting, "haha", value);
//    printf("result: %s\n", value);
//    closeSetting(setting);
//    return 0;
//}
