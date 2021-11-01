/*
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef INC_HTTP_FILE_MAPPINGS_H
#define INC_HTTP_FILE_MAPPINGS_H

#ifdef __cplusplus
extern "C" {
#endif


#define HTTP_GET_SUPPORT    0x01
#define HTTP_PUT_SUPPORT    0x02
#define HTTP_POST_SUPPORT   0x04
#define HTTP_DELETE_SUPPORT 0x08
#define HTTP_HEAD_SUPPORT   0x10


typedef struct {
    char    *file_path;
    char    *url_path;
    uint8_t supported_http_methods;
    uint8_t supported_ports;
} http_filename_mappings_t;

http_filename_mappings_t http_file_mappings[ ] = 
{
    { "/black.html",                                "/view/supplies/black",                 HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },              
    { "/bonjourConfig.html",                        "/view/network/bonjourConfig",          HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/cyan.html",                                 "/view/supplies/cyan",                  HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/eventLog.html",                             "/view/eventLog",                       HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/functions/host-print-content.xml",          "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/functions/host-print.xml",                  "/config/functions/hostPrint",          HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/functions/copy-content.xml",                "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/functions/copy.xml",                        "/config/functions/copy",               HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/functions/functions.xml",                   "/config/functions",                    HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/functions/scan-content.xml",                "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/functions/scan.xml",                        "/config/functions/scan",               HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/general.html",                              "/view/general",                        HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
#ifdef HAVE_PRINT_SUPPORT
    { "/hostPrint.html",                            "/view/hostPrint",                      HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
#endif
    { "/home.html",                                 "/view/home",                           HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/private.html",                              "/view/private",                        HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/io-connections/io-connections.xml",         "/config/ioConnections",                HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/io-connections/usb-connection-content.xml", "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/io-connections/usb-connection.xml",         "/config/ioConnections/usbConnection",  HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 }, 
#ifdef WIRED_NETWORK
    { "/io-connections/wired-network-content.xml",  "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/io-connections/wired-network.xml",          "/config/ioConnections/wiredNetwork",   HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
#endif    
    { "/ipv4Config.html",                           "/view/network/ipv4Config",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/leftNav.html",                              "/view/leftNav",                        HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/magenta.html",                              "/view/supplies/magenta",               HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/maint.html",                                "/view/maint",                          HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/midas-footer.xml",                          "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/midas-header.xml",                          "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/netAdmin.html",                             "/view/network/admin",                  HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/printHead.html",                            "/view/supplies/printHead",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/product-info-content.xml",                  "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/product-info.xml",                          "/config/productInfo",                  HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/device.xml",                                "/config/device",                       HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/service-content.xml",                       "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/service.xml",                               "/config/service",                      HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/snmpConfig.html",                           "/view/network/snmpConfig",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/special.html",                              "/view/supplies/special",               HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/status-content.xml",                        "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/status.xml",                                "/config/status",                       HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/storage-content.xml",                       "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/storage.xml",                               "/config/storage",                      HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/marker-supply-content.xml",        "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/marker-supply.xml",                "/config/supplies/markerSupply",        HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/media-supply-content.xml",         "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/media-supply.xml",                 "/config/supplies/mediaSupply",         HTTP_HEAD_SUPPORT | HTTP_PUT_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/supplies/print-head-content.xml",           "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/print-head.xml",                   "/config/supplies/printHead",           HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/supplies/supplies.xml",                     "/config/supplies",                     HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/supplies.html",                             "/view/supplies/summary",               HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/toolbox.css",                               "/view/toolbox.css",                    HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/usage-content.xml",                         "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/usage.html",                                "/view/usage",                          HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/usage.xml",                                 "/config/usage",                        HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/version-content.xml",                       "",                                     0                                   , HTTP_SOCKET_TCP_80 },
    { "/version.xml",                               "/config/version",                      HTTP_HEAD_SUPPORT                   , HTTP_SOCKET_TCP_80 },
    { "/yellow.html",                               "/view/supplies/yellow",                HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/js/client-tools.js",                        "/js/client-tools.js",                  HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/logo.gif",                           "/images/logo.gif",                     HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/alert-icon.gif",                     "/images/alert-icon.gif",               HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/frame-flag-0.gif",                   "/images/frame-flag-0.gif",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/frame-flag-1.gif",                   "/images/frame-flag-1.gif",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/frame-flag-2.gif",                   "/images/frame-flag-2.gif",             HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/info-icon.gif",                      "/images/info-icon.gif",                HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/supply-flag-0.gif",                  "/images/supply-flag-0.gif",            HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/supply-flag-1.gif",                  "/images/supply-flag-1.gif",            HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/images/supply-flag-2.gif",                  "/images/supply-flag-2.gif",            HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },
    { "/gcp.html",                                  "/view/network/gcp",                    HTTP_HEAD_SUPPORT | HTTP_GET_SUPPORT, HTTP_SOCKET_TCP_80 },

    /* EWS files not accessible by HTTP client */
    { "/templates.html",                            "/templates.html",                      0, 0 },
    { "/toolbox.css",                               "/toolbox.css",                         0, 0 },

    { NULL,                                         NULL,                                   0                                   , 0 },
};
                                            


#ifdef __cplusplus
}
#endif

#endif //INC_HTTP_FILE_MAPPINGS_H
