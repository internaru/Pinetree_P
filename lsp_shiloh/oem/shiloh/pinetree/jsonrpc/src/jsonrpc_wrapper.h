#ifndef JSONPRC_WRAPPER_H_
#define JSONRPC_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

int parse_jsonrpc(const char *data, char *response);

#ifdef __cplusplus
}
#endif

#endif
