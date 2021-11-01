#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <arpa/inet.h>
//#include <json/json.h>
#include "json/json.h"
#include "jsonrpc.h"
#include "jsonrpc_network.h"
//#include "misc_util.h"

#define JSONRPC_VERSION "0.2.0"
char jsonrpc_version[] __attribute__ ((section (".version"))) = JSONRPC_VERSION;

using namespace std;
using namespace Json;

bool ReadFromFile(const char* filename, char* buffer, int len)
{
    FILE* r = fopen(filename, "rb");
    if (NULL == r)
        return false;
    size_t fileSize = fread(buffer, 1, len, r);
    printf("fileSize: %d\n", fileSize);
    fclose(r);
    return true;
}

void * openSetting(const char *fileName)
{
    char readBuffer[JSON_BUFFER_SIZE] = {0,};

    if (false == ReadFromFile("test.json", readBuffer, JSON_BUFFER_SIZE)) return NULL;

    std::string config_doc = readBuffer;
    Json::Value *root = new Json::Value;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( config_doc, *root );
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse configuration\n"
          << reader.getFormatedErrorMessages();
        return NULL;
    }
    
    return (void *)root;
}

int closeSetting(void *setting)
{
    delete (Json::Value *)setting;
    return 0;
}

int readSetting(void *setting, const char *key, char *value)
{
    Json::Value *root = (Json::Value *)setting;
    strcpy(value, root->get(key, "" ).asCString());
    return 0;
}

void ReadJson(const char *data, char *response)
{
    string requestString;
    string responseString;
    Reader reader;
    StyledWriter writer;

    Value root;
    Value resultRoot;
    Value params;
    
    resultRoot["jsonrpc"] = "2.0";
    resultRoot["id"] = Value::null;

    requestString = data;
	if(reader.parse(requestString, root))
	{
        resultRoot["id"] = root["id"];
        printf("--jeseo-- JSON command %s\n", root["method"].asCString());
        if (!strcmp(root["method"].asCString(), "get params"))
        {
            networkGetParams(&root, &resultRoot);
        }
        else if (!strcmp(root["method"].asCString(), "set params"))
        {
            networkSetParams(&root, &resultRoot);
        }
        else if (!strcmp(root["method"].asCString(), "set mac") ||
                 !strcmp(root["method"].asCString(), "setMac"))
        {
            char buffer[256];
            printf("setting mac address %s\n", root["method"].asCString());
            sprintf(buffer, "/app/karas_pinetree_util.sh --set-mac-and-show %s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            //sprintf(buffer, "Mac addr   %c%c%c", root["params"].asCString()[0], root["params"].asCString()[1], root["params"].asCString()[2]);
//            sendMessageToSysmgr(0, buffer);
//            sendMessageToSysmgr(1, &root["params"].asCString()[3]);
            //while (1) usleep(1024*1024);
            //system("reboot");
        }
        else if (!strcmp(root["method"].asCString(), "firmwareUpdate"))
        {
//            firmwareUpdate(root["params"]["size"].asInt(), root["params"]["crc32"].asInt());            
            char buffer[256];
            printf("Firmware update\n");
            sprintf(buffer, "/app/karas_pinetree_util.sh --pc-update");
            printf(buffer);
            printf("\n");
            system(buffer);
        }
        else if (!strcmp(root["method"].asCString(), "showMac"))
        {
            char buffer[256];
            printf("Display MAC\n");
            sprintf(buffer, "/app/karas_pinetree_util.sh");
            printf(buffer);
            printf("\n");
            system(buffer);
        }
        else if (!strcmp(root["method"].asCString(), "factoryReset"))
        {
            char buffer[256];
            printf("Factory reset & Check RTC %s\n", root["params"]["unixTime"].asCString());
            sprintf(buffer, ". /app/karas_pinetree_util.sh --test-end %s", root["params"]["unixTime"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
//            system("sync");
//            system("/app/kara_pine_util.sh --reboot");
/*        	if (factoryReset() < 0)
        	{
                printf("Error: Factory reset failed\n");
//                sendMessageToSysmgr2("Factory reset", "failed");
        		while (1) sleep(1000);
                
        	}
//        	sendMessageToSysmgr2("Factory reset", "succeed");
*/            printf("Factory reset succeed\n");
        }
        else if (!strcmp(root["method"].asCString(), "updateTLI"))
        {
            char buffer[256];
            printf("update TLI %s\n", root["method"].asCString());
            sprintf(buffer, ". /app/karas_pinetree_util.sh --set-tli %s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");

/*        	if (updateTLI(root["params"].asCString()) < 0)
        	{
                printf("Error: Update TLI failed\n");
//                sendMessageToSysmgr2("TLI update", "failed");
        		while (1) sleep(1000);
        		
        	}
//            sendMessageToSysmgr2("TLI update", "succeed");
*/        	printf("Update TLI succeed\n");
        }
        else if (!strcmp(root["method"].asCString(), "UpdateSerial"))
        {
            char buffer[256];
            printf("Update Serial %s\n", root["method"].asCString());
            sprintf(buffer, "/app/karas_pinetree_util.sh --set-serial %s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
/*        	if (updateSerial(root["params"].asCString()) < 0)
        	{
                printf("Error: Update Serial failed\n");
//                sendMessageToSysmgr2("Serial update", "failed");
        		while (1) sleep(1000);
        	}
//            sendMessageToSysmgr2("Serial update", "succeed");
*/            printf("Update Serial succeed\n");
        }
        else if (!strcmp(root["method"].asCString(), "updateDrumTLI"))
        {
            char buffer[256];
            printf("update Drum TLI %s\n", root["method"].asCString());
            sprintf(buffer, "/app/karas_pinetree_util.sh --set-drum-tli %s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
/*            if (updateDrumTLI(root["params"].asCString()) < 0)
            {
                printf("Error: Update Drum TLI failed\n");
//                sendMessageToSysmgr2("Drum TLI update", "failed");
                while (1) sleep(1000);

            }
//            sendMessageToSysmgr2("Drum TLI update", "succeed");
*/            printf("Update Drum TLI succeed\n");
        }
        else if (!strcmp(root["method"].asCString(), "UpdateDrumSerial"))
        {
            char buffer[256];
            printf("Update Drum Serial %s\n", root["method"].asCString());
            sprintf(buffer, "/app/karas_pinetree_util.sh --set-drum-serial %s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
/*            if (updateDrumSerial(root["params"].asCString()) < 0)
            {
                printf("Error: Update Drum Serial failed\n");
//                sendMessageToSysmgr2("Drum Serial update", "failed");
                while (1) sleep(1000);
            }
//            sendMessageToSysmgr2("Drum Serial update", "succeed");
*/            printf("Update Drum Serial succeed\n");
        } 
        else if (!strcmp(root["method"].asCString(), "resetCPU"))
        {
            char buffer[256];
            printf("Reboot %s\n", root["method"].asCString());
            sprintf(buffer, "/app/karas_pinetree_util.sh --reboot");
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
//            resetCPU();
        }
        else if (!strcmp(root["method"].asCString(), "runShellCommand"))
        {
            char buffer[256];
            printf("Run Shell Command: ");
            sprintf(buffer, "%s", root["params"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");

//            runShellCommand(root["params"].asCString());
        }
        else if (!strcmp(root["method"].asCString(), "GDIMode"))
        {
//            GDIMode();
        }
        else if (!strcmp(root["method"].asCString(), "setUnixTime"))
        {
//            setUnixTime(root["params"]["unixTime"].asInt());
            char buffer[256];
            printf("setUnixTime %s\n", root["params"]["unixTime"].asCString());
            sprintf(buffer, ". /app/karas_pinetree_util.sh --set-rtc %s", root["params"]["unixTime"].asCString());
            printf(buffer);
            printf("\n");
            system(buffer);
            printf("\n");
        }
        else if (resultRoot["id"] != Value::null)
        {
            Value error;
            printf("%s[%d] Error: unsupported command\n", __FILE__, __LINE__);
            error["code"] = -32601;
            error["message"] = "Unsupported command";
    		resultRoot["error"] = error;
        }
        else
        {
            printf("%s[%d] Error: unsupported command\n", __FILE__, __LINE__);
        }
	}
    else
    {
        Value error;
        printf("Json parse failed\n");
        error["code"] = -32700;
        error["message"] = "Json parse failed";
		resultRoot["error"] = error;
    }

    if (response != NULL)
    {
        if (root["id"] == Value::null && resultRoot["error"] == Value::null)
        {
            response[0] = '\0';
        }
        else
        {
            responseString = writer.write(resultRoot);
            strcpy(response, responseString.c_str());
        }
    }
}

#define UEL_HEADER "\033%-12345X\n"
#define OPL_HEADER "JSONRPC="
int ParseJSONRPC(const char *data, char *response)
{
    printf("--jeseo-- jsonrpc v%s\n", JSONRPC_VERSION);
    printf("--jeseo-- received json data\n %s\n", data);

    int pos = 0;
    if (!strncmp(UEL_HEADER, data + pos, strlen(UEL_HEADER)))
    {
        printf("--jeseo-- cut off UEL header\n");
        pos += strlen(UEL_HEADER);
    }
    if (!strncmp(OPL_HEADER, data + pos, strlen(OPL_HEADER)))
    {
        printf("--jeseo-- cut off JSONRPC OPL header\n");
        pos += strlen(OPL_HEADER);
    }
    ReadJson(data + pos, response);
    if (response != NULL)
    {
        printf("--jeseo-- send json data\n %s\n", response);
    }
    return 0;
}

