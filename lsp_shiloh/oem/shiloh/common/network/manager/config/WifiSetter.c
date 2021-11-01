#include <stdio.h>
#include <unistd.h>
#include "NetworkIFModule.h"
#include "pthread.h"


#define __WIFISETTER_VERSION__	"1.0.2"
#define TIMEOUT	10
#define CONNECTION_RETRY_TIME	60

void* WifiSetter (void *args)
{
	printf ("(Boot Wifi) WifiSetter %s\n",__WIFISETTER_VERSION__);
	printf ("(Boot Wifi) Should be run at boot-up Time, once\n");

	static int tryconnection = 1;
	static int isconnected = 0;
	while(1)
	{
		char *IpAddr;
		int returnValue;
		returnValue =  NIF_getConnectedSSID(&IpAddr); 
		//Wifi 어댑터가 있는지 없는지 검사.
		if(returnValue == NIF_Status_NotFound && isconnected == 0)
		{
			printf("\n####EXIT WIFI SETTER Return = %d",returnValue);
			return NULL;
		}
		//printf("\n####WIFI SETTER Return = %d",returnValue);

		if (returnValue == NIF_Status_IfDown)  //Disconnected
		{
			T_SSID * ptrAPList;
			int NumberOfSSID;
			int result;
			//DB에서 최근값하나를 불러온 후
			result=NIF_getOneSSIDinDB(&NumberOfSSID,&ptrAPList );
			//printf ("2\n");
			if (result == NIF_Status_IfUp)
			{
				//NIF_viewSSIDInformation(0,ptrAPList);
				//Connection 을 때린 다음
				printf ("(Boot Wifi) Try to connect, wait %d sec...\n",TIMEOUT);
				NIF_connectSSID(0,ptrAPList);
				//TIMEOUT초간 기다리고 :10초
				int i;
				for (i=0;i<TIMEOUT;i++)
				{	
					printf ("Wait for %d Sec...\n",i);
					if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
						break;
					else
						sleep(1);
				}
				//Connection이 되었는지를 확인한 후
				if (NIF_connectStatus_wifi() == NIF_Status_IfUp)
				{
					printf ("(Boot Wifi) Connection Established, Setup IP Address\n\n");
					//ifup을 시킨다.
					NIF_autoIF_Wifi();
					isconnected = 1;
					tryconnection = 0;
				}
				printf ("(Boot Wifi) IP Address/Gateway/DNS Set up complete\n");
				//아님 말고.
				NIF_freeSSID(NumberOfSSID,ptrAPList);	
			}
			else
			{
				printf ("(Boot Wifi) Connection aborted, FindResult: %d\n",result);
				tryconnection = 1;
			}
		}
		else
		{
			tryconnection = 1;
		}

		if(tryconnection == 1)
			sleep(CONNECTION_RETRY_TIME);	
	}
	return NULL;
}




int init_wifisetter()
{
   pthread_attr_t threadAttributes;

   int result = 0;

   if ((result = pthread_attr_init(&threadAttributes)))
   {
      return result;
   }
   result = pthread_attr_setdetachstate(&threadAttributes, PTHREAD_CREATE_DETACHED);
   if (result)
   {
      pthread_attr_destroy(&threadAttributes);
      return result;
   }
   pthread_t     wifisetter_thread;
   result = pthread_create(&wifisetter_thread, &threadAttributes, WifiSetter,(void *)NULL);
   pthread_attr_destroy(&threadAttributes);
   return result;
}

