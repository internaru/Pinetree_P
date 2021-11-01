/*
 * ============================================================================
 * Copyright (c) 2011-2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_NET_GCPP_VARS_H
#define INC_NET_GCPP_VARS_H

#include <stdint.h>
#include <stdbool.h>
#include <error_types.h>
#include <net_gcpp_api.h>

#ifdef __cplusplus
extern "C" {
#endif

void net_gcpp_var_init(void);

gcpp_error_t net_gcpp_get_var_enabled(bool *enabled);
gcpp_error_t net_gcpp_set_var_enabled(bool enabled);

gcpp_error_t net_gcpp_get_var_registered(bool *registered);
gcpp_error_t net_gcpp_set_var_registered(bool registered);

gcpp_error_t net_gcpp_get_var_proxy(char *proxy, uint32_t len);
gcpp_error_t net_gcpp_set_var_proxy(const char *proxy);

gcpp_error_t net_gcpp_get_var_printer_name(char *printer_name, uint32_t len);
gcpp_error_t net_gcpp_set_var_printer_name(const char *printer_name);

gcpp_error_t net_gcpp_get_var_display_name(char *display_name, uint32_t len);
gcpp_error_t net_gcpp_set_var_display_name(const char *display_name);

gcpp_error_t net_gcpp_get_var_http_proxy_enabled(bool *enabled);
gcpp_error_t net_gcpp_set_var_http_proxy_enabled(bool enabled);

gcpp_error_t net_gcpp_get_var_http_proxy_server(char *server, uint32_t len);
gcpp_error_t net_gcpp_set_var_http_proxy_server(const char *server);

gcpp_error_t net_gcpp_get_var_http_proxy_port(uint32_t *proxy_port);
gcpp_error_t net_gcpp_set_var_http_proxy_port(uint32_t proxy_port);

gcpp_error_t net_gcpp_get_var_http_proxy_auth(bool *auth);
gcpp_error_t net_gcpp_set_var_http_proxy_auth(bool auth);

gcpp_error_t net_gcpp_get_var_http_proxy_username(char *username, uint32_t len);
gcpp_error_t net_gcpp_set_var_http_proxy_username(const char *username);

gcpp_error_t net_gcpp_get_var_http_proxy_password(char *password, uint32_t len);
gcpp_error_t net_gcpp_set_var_http_proxy_password(const char *password);

gcpp_error_t net_gcpp_get_var_xmpp_jid(char *token, uint32_t len);
gcpp_error_t net_gcpp_set_var_xmpp_jid(const char *token);

gcpp_error_t net_gcpp_get_var_refresh_token(char *token, uint32_t len);
gcpp_error_t net_gcpp_set_var_refresh_token(const char *token);

#ifdef __cplusplus
}
#endif

#endif // INC_NET_GCPP_VARS_H

