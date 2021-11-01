/*
 * ============================================================================
 * Copyright (c) 2009-2013   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

// net_wifi_main.c -- provides wifi startup support via net_wifi_init()

#ifdef __cplusplus
extern "C" {
#endif

// os headers
#include <stdio.h>
#include <string.h>

// common headers
#include "oid_api.h"
#include "net_api.h" // NET_IF_LINK_NAME_TYPES
#include "net_link_api.h" // start_ms_timer()
#include "net_conf_api.h" // WIFI_DEV_DRIVER
#include "net_wifi_api.h"
#include "net_wifi_vars.h"
#include "os_system.h"
#include "net_intf_api.h" // get_intf_ipv4_addr()

#include "wifi_intf_api.h" // MAX_SSID_NAME_LEN
#include "wifi_intf_vars.h"

//for auto_connect_wifi
#include "NetworkIFModule.h"
#include "db_api.h"

// local defines
#define WIFI_STARTUP_DELAY 1101 /* ~1.1 seconds */
// from mlanInterface.c (.../common/devices/wifi/src/mlaninterface)
#define WIFI_CONFIG_REACTION_DELAY 1102 /* ~1.1 seconds */


// Debug configuration
#define DBG_PRFX "net.wifi: "
#define DBG_ON
#define DBG_ERR_ON
//off #define DBG_VERBOSE_ON
//off #define DBG_RELEASE_ON
#include "net_debug.h"    // DBG_ macro family

#define WIFI_AUTOCONN_STACK_SIZE POSIX_MIN_STACK_SIZE
#define UAP_AUTOCONN_STACK_SIZE POSIX_MIN_STACK_SIZE
static ALIGN8 UINT8 WifiAutoconnStack[WIFI_AUTOCONN_STACK_SIZE];
static ALIGN8 UINT8 UAPAutoconnStack[WIFI_AUTOCONN_STACK_SIZE];

// static wpa_supplicant conf file settings and capabilities
static const char* install_wifi_drivers_script[] = {
    "#!/bin/sh",
    "# " WIFI_DEV_DRIVER "/install_wifi_drivers.sh",
    "",
    "export PATH=$PATH:" SBIN,
    "mkdir -p /lib/firmware/mrvl",
    "",
    "cp " WIFI_DEV_DRIVER "/usb8782_uapsta.bin /lib/firmware/mrvl",
    "",
    "modprobe cfg80211",
    "",
    "cd " WIFI_DEV_DRIVER,
    "",
    "test ! -z \"$(lsmod | grep mlan)\" || insmod mlan.ko",
    "test ! -z \"$(lsmod | grep usb8xxx)\" || insmod usb8xxx.ko",
    "exit $?",
    NULL // required end-of-boilerplate
};

int gWifiModuleType = SDH_NET_WIFI_MODULE_NONE; // Global. by mspyo. 2021-02-15
void checkWifiModuleType(void); // by mspyo. 2021-02-15

//
// returns 'given_buffer' (filled as appropriate)
//
char* format_wifi_drivers_sh(char* given_buffer, int buf_len)
{
char*    next = given_buffer; // string buffer starts here

    // output sh script header/banner
    char* n = next;

    // for each script line
    {
        n = add_txt_records(n, (char**)install_wifi_drivers_script);
    }
    DBG_VERBOSE("%s():'\n%s\n'\n", __func__, given_buffer);

return given_buffer;
}

// replace configuration file (with backup)
error_type_t update_wifi_drivers_sh(void)
{
    error_type_t rcode = ~OK;
    char* conf_file_image = (char*) malloc(MAX_CONF_FILE_SIZE);

    if (conf_file_image != NULL)
    {
        memset(conf_file_image, 0, MAX_CONF_FILE_SIZE);
        // generate updated file content
        format_wifi_drivers_sh(conf_file_image, MAX_CONF_FILE_SIZE);

        rcode = write_conf_file(WIFI_drivers_e, conf_file_image);
        free(conf_file_image);
    }

    if (rcode != OK)
    {
        DBG_ERR("%s() '%s' (err %d)\n", __func__, strerror(rcode), rcode);
    }
    else
    {
        char chmod_req[MAX_CONF_FILE_SIZE + 16];
        char filename[MAX_CONF_FILE_SIZE];
        lookup_fs_name(WIFI_drivers_e, filename, MAX_CONF_FILE_SIZE);
        sprintf(chmod_req, "chmod +x %s", filename);
        rcode = os_system(chmod_req);
    }
    return rcode;
}

// initiate loading of wireless kernel modules
error_type_t init_wireless_drivers(void)
{
    error_type_t ret = ~OK;
    char script_name[256];
    lookup_fs_name(WIFI_drivers_e, script_name, 256);
    if (!file_exists(script_name))
    {
        update_wifi_drivers_sh(); // update wifi driver startup sh
    }

    ret = os_system(script_name); // initialize wireless ko modules
    return ret;
}

bool WlanIsWirelessHWActive(void)
{
    return wifi_sta_hw_ready();
}


//===========================================================================================================================
//      wifi timer callbacks
//===========================================================================================================================

// wifi settings
timer_t defer_settings_change_timer; // wifi settings change deferral timer
static void wifi_settings_timer_cb(union sigval sig)
{
    DBG_VERBOSE("%s() timer signal %d .. (now %d)\n", __func__, sig, posix_gettime_ticks());
#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_CONFIG, NET_SERVICE_STA, NULL, NULL);
#else
    start_sta_service();
#endif
}

// start defer-changes timer (if not currently active)
long initiate_wifi_settings_change_timer(long ms_delta)
{
    // any remaining time on current timer?
    timer_t* timer = &defer_settings_change_timer;
    if (*timer)
    {
        struct itimerspec time_left;
        timer_gettime(*timer, &time_left);
        if ((time_left.it_value.tv_sec + time_left.it_value.tv_nsec) > 0)
        {
            long ms_remaining = time_left.it_value.tv_nsec/1000000 + time_left.it_value.tv_sec*1000;
            // ignore reset if current timer fits within ms_delta
            if (ms_remaining < ms_delta*1000000)
            {
                DBG_VERBOSE("%s() %d remaining .. still ticking\n", __func__, ms_remaining);
                return ms_remaining; // return ms unit
            }
        }
    }

    // refresh timer interval
    return start_ms_timer(timer, ms_delta, wifi_settings_timer_cb);
}

// wifi service
timer_t schedule_wifi_start_timer; // wifi start timer
static void start_wifi_sta_service_cb(union sigval sig)
{
    DBG_VERBOSE("%s() timer signal %d .. (now %d)\n", __func__, sig, posix_gettime_ticks());

#ifdef HAVE_NET_SERVICES
    net_service_request(NET_SERVICE_RENEW, NET_SERVICE_STA, NULL, NULL);
#else
    start_sta_service(); // trigger service
#endif
}

// refresh the timer if not already running OR new time < remaining delay
long initiate_wifi_service_timer(long ms_delta)
{
    // time remaining on current timer?
    timer_t* timer = &schedule_wifi_start_timer;
    if (*timer)
    {
        struct itimerspec time_left;
        timer_gettime(*timer, &time_left);
        long long ns_remaining = (long long)time_left.it_value.tv_nsec*1000000000L + (long long)time_left.it_value.tv_sec*1000L;
        if (ns_remaining > 0)
        {
            // ignore reset if remaining fits within ms_delta
            if (ns_remaining < (long long)ms_delta*1000000)
            {
                DBG_VERBOSE("%s(%d) remaining (%d:%d)\n", __func__, ms_delta, time_left.it_value.tv_sec, time_left.it_value.tv_nsec);
                return (long)(ns_remaining/1000000L); // return ms unit
            }
        }
    }

    // refresh timer interval
    return start_ms_timer(timer, ms_delta, start_wifi_sta_service_cb);
}

void startApplyInfoTimer(void) // defer wifi_startup
{
    //noisy DBG_VERBOSE("%s()\n", __func__);
    initiate_wifi_settings_change_timer(WIFI_CONFIG_REACTION_DELAY);

    if(WlanIsWirelessHWActive())
    {
#ifdef __linux__
        DBG_VERBOSE("WLAN: %s() activate delay timer .. (now %d)\n", __func__, posix_gettime_ticks());
#else // revisit timer window
        DBG_VERBOSE("WLAN: startApplyInfoTimer\n");

        // Make sure they use a valid SSID.  Re-init to default if necessary.
        wirelessEnsureValidSSID();

        // Stop trying to rejoin
        tx_timer_deactivate(&wcfgAdHocRejoinTimer);

        /* update timer; when timeout done,
         * get and apply settings
         */
        tx_timer_deactivate(&wcfgInfoTimer);
        tx_timer_change(&wcfgInfoTimer, 175, 0);
        tx_timer_activate(&wcfgInfoTimer);

        wcfgStartLinkBlackout();
#endif
    }
}

//===========================================================================================================================
//      wifi_sta states
//===========================================================================================================================

bool wifi_sta_enabled(void) // oid-lookup
{
    uint32_t enabled = 0;
    oid_get_uint32(OID_WIFI_INTF_ENABLED, 0, &enabled); // 1st-intf index
    return (enabled == 0 ? false : true);
}

bool wifi_sta_hw_ready(void) // hw-lookup (wifi/sta interface)
{
    if ((bool)get_intf_ipv4_addr(NET_IF_NAME_STA)) // hw addr active?
        return true;
    return false;
}

bool wifi_sta_ready(void) // intf lookup
{
    return wifi_sta_hw_ready();
}

bool wifi_sta_enabled_and_available(void) // oid- and hw- composite
{
    return (wifi_sta_enabled() & wifi_sta_ready());
}

bool wifi_sta_active(void) // intf-lookup
{
    return intf_active(NET_IF_NAME_STA);
}

// wpa_supplicant_running() -- query os-processes for wpa_supplicant presence
bool wpa_supplicant_running(void) // true/false running or not-running
{
    bool running = os_system("test -z \"$(ps | grep -v grep | grep wpa_supplicant)\"");

    // changed state?
    static bool running_last_time = false; // 1-time init
    if (running & !running_last_time)
    {
        DBG_VERBOSE("%s() %s\n", __func__, (running?"up":"DOWN"));
    }
    running_last_time = running;

    return running;
}

bool wifi_sta_running(void) // true/false running or not-running
{
    static bool running_last_time = false; // 1-time init
    bool service_running = false; // wpa_supplicant_running();

    bool hw_running = wifi_sta_hw_ready();
    if (hw_running)
    {
        service_running = wpa_supplicant_running();
    }
    bool running = hw_running & service_running; // by definition: hw available and service started?

    // state changed?
    if (running & !running_last_time)
    {
        DBG_VERBOSE("%s() %s\n", __func__, (running?"up":"DOWN"));
    }
    running_last_time = running;

    return running; // this time
}

//===========================================================================================================================
//     auto connect wifi 
//===========================================================================================================================
#define ENABLE_WIFI_AUTO_CONNECT
#ifdef ENABLE_WIFI_AUTO_CONNECT
enum getStatusReturnType checkifConnected(char *ifName)
{
    if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
    {
        int sockfd = 0;
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd > 0)
        {
            struct ifreq ifr;
            int          result;
            strncpy(ifr.ifr_name, ifName, 16);
            result = ioctl(sockfd, SIOCGIFADDR, &ifr);
            if (result < 0)
            {
                close(sockfd);
                return NIF_Status_NotFound;
            }
            else
            {
                close(sockfd);
                return NIF_Status_IfUp;
            }
        }
        else
        {
            return NIF_Status_NotFound;
        }
    }
    return NIF_Status_IfDown;
}

pthread_mutex_t g_wifi_autoconnect_mutex;

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

bool isWifiConnecting=false;

void setWiFiConnecting(bool isWifiConnecting)
{
    WIFI_AUTO_CONNECT_LOCK();
    isWifiConnecting=isWifiConnecting;
    WIFI_AUTO_CONNECT_UNLOCK();
}
#define WIFI_AUTO_CONNECT_TIMEOUT 60
void *auto_connect_wifi_thread(void *unused){

    T_SSID * ptrAPList;
    int NumberOfSSID;
    int result;
    setWiFiConnecting(false);
    while(1)
    {
        error_type_t db_retval = FAIL;
        if(db_get_var_int("recd_ssidbook","ifselected",&db_retval) <= 0)
        {
            //DBG_PRINTF_SDH_ERR("[Auto_Conn]Wifi  set OFF!");
            sleep(WIFI_AUTO_CONNECT_TIMEOUT);
            continue;
        }
        if(db_retval == FAIL)
        {
            DBG_PRINTF_SDH_ERR("[Auto_Conn]DB Connection Failed!");
            sleep(WIFI_AUTO_CONNECT_TIMEOUT);
            continue;
        }
        result=NIF_getOneSSIDinDB(&NumberOfSSID,&ptrAPList);
        if (result == NIF_Status_IfUp)
        {
            int i;
            const int TIMEOUT = 30;
            WIFI_AUTO_CONNECT_LOCK();
            if(isWifiConnecting == false)
            {
                isWifiConnecting= true;
                WIFI_AUTO_CONNECT_UNLOCK();
                NIF_connectSSID(0,ptrAPList);
                for (i=0;i<TIMEOUT;i++)
                {
                    DBG_PRINTF_SDH_DEBUG("Wait for %d Sec...\n",i);
                    if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
                        break;
                    else
                        sleep(1);
                }
                if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
                {
                    NIF_autoIF_Wifi();
                    NIF_updateConnStatus_wifi(1); //TurnON
                    DBG_PRINTF_SDH_DEBUG("[%s]%d:WIFI_Connection_SUCCESS!!!\n",__func__,__LINE__);
                }
                else
                {
                    DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__func__,__LINE__);
                    result = false;
                }
                NIF_freeSSID(NumberOfSSID,ptrAPList);
                NIF_turnMDNS_on();
                WIFI_AUTO_CONNECT_LOCK();
                isWifiConnecting= false;
                WIFI_AUTO_CONNECT_UNLOCK();
            }
            else
            {
                sleep(WIFI_AUTO_CONNECT_TIMEOUT);
            }
        }
		sleep(WIFI_AUTO_CONNECT_TIMEOUT);
        while(checkifConnected("mlan0") == NIF_Status_IfUp)
        {
            sleep(WIFI_AUTO_CONNECT_TIMEOUT);
        }
    }
    return 0;
}
#else
void *auto_connect_wifi_thread(void *unused){

	T_SSID * ptrAPList;
	int NumberOfSSID;
	int result;

	error_type_t db_retval = FAIL;
	if(!db_get_var_int("recd_ssidbook","ifselected",&db_retval)){
        DBG_PRINTF_SDH_DEBUG("[Auto_Conn]Wifi is set OFF!");
		return 0;
	}
	if(db_retval == FAIL){
        DBG_PRINTF_SDH_ERR("[Auto_Conn]DB Connection Failed!");
		return 0;
	}

	//DB에서 최근값하나를 불러온 후
	result=NIF_getOneSSIDinDB(&NumberOfSSID,&ptrAPList);
	
	if (result == NIF_Status_IfUp)
	{
		int i;
		const int TIMEOUT = 30;

		NIF_connectSSID(0,ptrAPList);
	 	//TIMEOUT초간 기다리고 :30초
        for (i=0;i<TIMEOUT;i++){
           
            DBG_PRINTF_SDH_DEBUG("Wait for %d Sec...\n",i);
            if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
                break;
            else
                sleep(1);
        }   
		//check if connect or not
	    if (NIF_connectStatus_wifi() == NIF_Status_IfUp){
			NIF_autoIF_Wifi();
			//printf("#################NIF_getStatus_wifi:%d\n",NIF_getStatus_wifi());
			NIF_updateConnStatus_wifi(1); //TurnON
			DBG_PRINTF_SDH_DEBUG("[%s]%d:WIFI_Connection_SUCCESS!!!\n",__func__,__LINE__);
		}
		else{
			DBG_PRINTF_SDH_ERR("[%s]%d:WIFI_Connection_FAILED!!!\n",__func__,__LINE__);
			result = false;
		}
        NIF_freeSSID(NumberOfSSID,ptrAPList);   
		NIF_turnMDNS_on();
	}
	else{
		DBG_PRINTF_SDH_ERR("(Boot Wifi) Connection aborted, FindResult: %d\n",result);
	}

	return 0;
}

#endif
void *auto_connect_uap_thread(void *unused){

	#define start_uap_sh "/etc/init.d/uap_start.sh"
	error_type_t db_retval = FAIL;

	if(!db_get_var_int("CONF_UAPSTATUS","ifActive",&db_retval)){
        DBG_PRINTF_SDH_DEBUG("[Auto_Conn_UAP]UAP is set OFF!");
		return 0;
	}
	if(db_retval == FAIL){
        DBG_PRINTF_SDH_ERR("[Auto_Conn_UAP]DB Connection Failed!");
		return 0;
	}

	system(start_uap_sh);
    DBG_PRINTF_SDH_ERR("[Auto_Conn_UAP]UAP turned ON!");
	sleep(3);
	NIF_turnMDNS_on();

	return 0;
}

void auto_connect_wifi_uap(void){
	
	unsigned int os_rcode = -1;
	static pthread_t auto_conn_wifi_thread;
	static pthread_t auto_conn_uap_thread;
#ifdef ENABLE_WIFI_AUTO_CONNECT
	int px_status = 0; UNUSED_VAR(px_status);
	px_status = posix_mutex_init( &g_wifi_autoconnect_mutex);
	XASSERT( px_status==0, px_status );
#endif

	os_rcode = posix_create_thread(&auto_conn_wifi_thread, auto_connect_wifi_thread, (void *)0, "wifi_autoconn_thr",
									WifiAutoconnStack, WIFI_AUTOCONN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
	if(os_rcode != OS_SUCCESS)
	{
		DBG_PRINTF_SDH_ERR("error creating thread %s (os_rcode=0x%02x)\n", os_rcode, NET_MAC_THREAD_NAME);
	}
	else{
		DBG_PRINTF_SDH_DEBUG("[Auto Conn Wifi]Wifi connection creating thread SUCCESS\n");
		//DBG_PRINTF_SDH_DEBUG("[Auto Conn Wifi]creating thread SUCCESS\n");
	}

	os_rcode = posix_create_thread(&auto_conn_uap_thread, auto_connect_uap_thread, (void *)0, "uap_autoconn_thr",
									UAPAutoconnStack, UAP_AUTOCONN_STACK_SIZE, POSIX_THR_PRI_NORMAL);
	if(os_rcode != OS_SUCCESS)
	{
		DBG_PRINTF_SDH_ERR("error creating thread %s (os_rcode=0x%02x)\n", os_rcode, NET_MAC_THREAD_NAME);
	}
	else{
		DBG_PRINTF_SDH_DEBUG("[Auto Conn UAP]UAP connection creating thread SUCCESS\n");
	}

}
//===========================================================================================================================
//      wifi_sta init
//===========================================================================================================================

//   generate a wifi-driver init file and store in the filesystem
error_type_t net_wifi_init(void)
{
	// Check wifi module type. by mspyo. 2021-02-15
	checkWifiModuleType();

	// initialize global (shadow) vars from non-volatile set(s)
    wifi_intf_var_init();

    // initiate wireless ko/modules (regardless whether wifi enabled or not)
    init_wireless_drivers();

    register_wifi_cmds(); // register 'wifi' cmds

    // activate wifi (if enabled)
    initiate_wifi_service_timer(WIFI_STARTUP_DELAY); // revisit tuning wifi startup

	// auto connect wifi (if connected before shut down)
	auto_connect_wifi_uap();

    return OK;
}

void wifi_startup(uint8_t iface_enum)
{
    uint32_t enabled = wifi_sta_enabled();
    
    // refresh state of requested interface
    char wifi_ssid[MAX_SSID_NAME_LEN];
    oid_get_string(OID_WIFI_INTF_SSID, iface_enum, wifi_ssid, MAX_SSID_NAME_LEN);
    DBG_VERBOSE("%s ssid '%s' enabled %d\n", __func__, wifi_ssid, enabled);

    if (enabled) {
	start_sta_service();
    }

    return;
}

void checkWifiModuleType(void) { // by mspyo, 2021-02-15
	char *wifiModuleType = NULL;
	wifiModuleType = getenv("SDH_NET_WIFI_MODULE_TYPE");

	if (wifiModuleType != NULL) {
		int nWifiModuleType = atoi(wifiModuleType);
		if (nWifiModuleType == SDH_NET_WIFI_MODULE_MARVELL) {
			printf("[Net] Marvell wifi module.\n");
			gWifiModuleType = SDH_NET_WIFI_MODULE_MARVELL;
		} else if (nWifiModuleType == SDH_NET_WIFI_MODULE_REALTEK) {
			printf("[Net] Realtek wifi module.\n");
			gWifiModuleType = SDH_NET_WIFI_MODULE_REALTEK;
		} else {
			printf("[Net] %d, No wifi module.\n", __LINE__);
			gWifiModuleType = SDH_NET_WIFI_MODULE_NONE;
		}
	} else {
		printf("[Net] %d, No wifi module.\n", __LINE__);
		gWifiModuleType = SDH_NET_WIFI_MODULE_NONE;
	}
}

#ifdef __cplusplus
}
#endif

// eof
