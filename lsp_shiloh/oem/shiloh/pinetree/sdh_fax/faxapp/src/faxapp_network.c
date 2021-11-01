#include "oid_api.h"
#include "net_api.h"


static char gFaxMachineIPAddress[16] = {0, };


char *faxapp_get_ip_address (void)
{
    char buffer[128] = {0, };
    unsigned int if_wired_index = 2;

    if_wired_index = if_nametoindex("eth0");

    oid_get_string(OID_NETWORK_IPV4_ADDRESS, (uint32_t)if_wired_index, buffer, 128);

    printf("\n(YHCHO) %s() - buffer: %s\n", __FUNCTION__, buffer);

    snprintf(gFaxMachineIPAddress, sizeof(gFaxMachineIPAddress), "%s", buffer);

    return gFaxMachineIPAddress;
}

