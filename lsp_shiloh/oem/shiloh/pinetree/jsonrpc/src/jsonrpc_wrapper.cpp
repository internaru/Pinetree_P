#include <stdio.h>
#include "jsonrpc_wrapper.h"
#include "jsonrpc.h"

int parse_jsonrpc(const char *data, char *response)
{
    return ParseJSONRPC(data, response);
}
