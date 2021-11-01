#ifndef JSONPRC_H_
#define JSONRPC_H_

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#define JSON_BUFFER_SIZE 64*1024


int ParseJSONRPC(const char *data, char *response);
bool ReadFromFile(const char* filename, char* buffer, int len);
void * openSetting(const char *fileName);
int closeSetting(void *setting);
int readSetting(void *setting, const char *key, char *value);
void ReadJson(const char *data, char *response);

#ifdef __cplusplus
}
#endif

#endif
