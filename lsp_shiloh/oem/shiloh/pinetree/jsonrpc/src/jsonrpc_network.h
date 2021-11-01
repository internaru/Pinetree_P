#ifndef JSONPRC_NETWORK_H_
#define JSONRPC_NETWORK_H_

//#include <json/json.h>
#include "json/json.h"

#undef BOOL
#undef TRUE
#undef FALSE
#define BOOL bool
#define TRUE true
#define FALSE false

void networkGetParams(Json::Value *root, Json::Value *resultRoot);
void networkSetParams(Json::Value *root, Json::Value *resultRoot);

#endif
