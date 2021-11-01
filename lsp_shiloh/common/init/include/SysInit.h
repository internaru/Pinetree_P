/*
 *
 * ============================================================================
 * Copyright (c) 2006-2011   Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \file SysInit.h
 *
 * \brief
 *
 */

#ifndef __SYSINIT_H__
#define __SYSINIT_H__

#include "logger.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _DO_NOTHING do { } while(0)

//
// Clear endpoints
//
#define USB_PRINT           1       ///< Set to 1 to get a usb print endpoint
#define USB_SCAN            2       ///< Set to 2 to get a usb scan endpoint
#define USB_MASS_STORAGE    4       ///< Set to 4 to get a mass storage endpoint
#define USB_FAX             8       ///< Set to 8 to get a fax endpoint
#define USB_HTTP            16      ///< Set to 16 to get a ews endpoint.
#define LPR_PRINT           1       ///< Set to 1 for
#define NET_SCAN            2

#ifdef HAVE_CMD
    #include "cmd_sys_api.h"
    #define INITIALIZE_CMD() do \
    { \
        cmd_init(); \
    } \
    while( 0 );
#else
    #define INITIALIZE_CMD() _DO_NOTHING
#endif

#if defined( HAVE_CMD ) && defined ( HAVE_SYSTEM )
    #define INITIALIZE_CMD_PARSER() do \
                             { \
                                void cmd_parser_init( void );\
                                cmd_parser_init();\
                             } \
                             while(0)
    #define CMD_STATUS  1
#else
    #define INITIALIZE_CMD_PARSER() _DO_NOTHING
    #define CMD_STATUS 0 
#endif

#ifdef HAVE_UFS
    #define INITIALIZE_UFS() do \
                             { \
                                void ufs_init( void ); \
                                ufs_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_UFS() _DO_NOTHING
#endif

#ifdef HAVE_DATAFS
    #define INITIALIZE_DATAFS() do \
                             { \
                                void datafs_init( void ); \
                                datafs_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_DATAFS() _DO_NOTHING
#endif

#ifdef HAVE_DATAFILES
    #define INITIALIZE_DATAFILES() do \
                             { \
                                void datafiles_init( void ); \
                                datafiles_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_DATAFILES() _DO_NOTHING
#endif

#ifdef HAVE_COMMON_CONSUMABLES_API
    #define INITIALIZE_COMMON_CONSUMABLES() do \
                             { \
                                void consumable_api_init( void ); \
                                consumable_api_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_COMMON_CONSUMABLES() _DO_NOTHING
#endif

#ifdef HAVE_EFUSE
#include "e-fuse_api.h"
    #define INITIALIZE_EFUSE() do \
                              {        \
                                 efuse_init();    \
                              }        \
                              while(0)

#else
    #define INITIALIZE_EFUSE() _DO_NOTHING
#endif

#ifdef HAVE_GENERATORS
    #define INITIALIZE_GENERATORS() do \
                             { \
                                void generators_init( void ); \
                                generators_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_GENERATORS() _DO_NOTHING
#endif

#ifdef HAVE_IMAGELOG
    #define INITIALIZE_IMAGELOG() do \
		                     { \
								 void il_init( void ); \
								 il_init();\
							 } \
		                     while(0)
#else
    #define INITIALIZE_IMAGELOG() _DO_NOTHING
#endif

#ifdef HAVE_SCANBOX
#include "scanbox.h"
    #define INITIALIZE_SCANBOX() do \
                         { \
                             ScanBoxInit();\
                         } \
                         while(0)
#else
    #define INITIALIZE_SCANBOX() _DO_NOTHING
#endif

#ifdef HAVE_UART
#include "uart_api.h"
    #define INITIALIZE_UART() do \
                                    {\
    uartInit();\
                                    } \
                                    while(0)
#else
    #define INITIALIZE_UART()   do \
                                      { \
                                      } \
                                      while(0)
#endif

#ifdef HAVE_SDHFAX
    #define INITIALIZE_SDH_FAX() do \
                                    {\
                                        void faxapp_main_init(void);\
                                        void fsu_process_start(void);\
                                        void FaxPrintJobMgrInit(void);\
                                        void FaxScanJobMgrInit(void);\
                                        void FaxAppJobMgrInit(void);\
                                        void job_modem_start(void);\
                                        void job_t30_start(void);\
                                        void job_phone_start(void);\
                                        void FaxSolutionJobMgrInit(void);\
                                        faxapp_main_init();\
                                        fsu_process_start();\
                                        FaxPrintJobMgrInit();\
                                        FaxScanJobMgrInit();\
                                        FaxAppJobMgrInit();\
                                        job_modem_start();\
                                        job_t30_start();\
                                        job_phone_start();\
                                        FaxSolutionJobMgrInit();\
                                    } \
                                    while(0)
#else
#endif

#ifdef HAVE_STATUS
#include "agStatusMgrAPI.h"
#define INITIALIZE_STATUS_MGR() do \
    { \
        StatusMgrInit(); \
    } \
    while(0)
#else
#define INITIALIZE_STATUS_MGR() do \
    { \
    } \
    while(0)
#endif

#ifdef HAVE_SYSTEM
#include "agRouter.h"
#include "SJMgr.h"
#include "agResourceMgr.h"
#include "agConnectMgr.h"
    #define INITIALIZE_SYSTEM_MSG_ROUTER() do \
                                    {\
    SYInitRouter();\
                                    } \
                                    while(0)

    #define INITIALIZE_SYSTEM_RESOURCE_MGR() do \
                                    {\
    ResourceInit();\
                                    } \
                                    while(0)

    #define INITIALIZE_SYSTEM_JOB_MGR() do \
                                    {\
    SysJbMgrInit();\
                                    } \
                                    while(0)

    #define INITIALIZE_CONNECTION_MGR() do \
                                    {\
    SysParserInit();\
                                    } \
                                    while(0)

#else
//#error These are required system components
    #define INITIALIZE_SYSTEM_MSG_ROUTER() do \
                                    {\
                                    } \
                                    while(0)

    #define INITIALIZE_SYSTEM_RESOURCE_MGR() do \
                                    {\
                                    } \
                                    while(0)

    #define INITIALIZE_SYSTEM_JOB_MGR() do \
                                    {\
                                    } \
                                    while(0)

    #define INITIALIZE_CONNECTION_MGR() do \
                                    {\
                                    } \
                                    while(0)
#endif

#ifdef HAVE_NVRAM
#include "nvram_api.h"
    /* nvram_sys_init returns an error_type_t */
    #define INITIALIZE_NVRAM() nvram_sys_init()
    #define SHUTDOWN_NVRAM() nvram_shutdown()
#else
    /* nvram_sys_init returns an error_type_t */
    #define INITIALIZE_NVRAM() FAIL; 
    #define SHUTDOWN_NVRAM()
#endif


#ifdef HAVE_MEMORY_LOG
    #define INITIALIZE_MEMORY_LOG() do \
                                    {\
                                        void logStringInit( void );\
                                        logStringInit();\
                                    } \
                                    while(0)
#else
    #define INITIALIZE_MEMORY_LOG()   do \
                                      { \
                                      } \
                                      while(0)
#endif

#ifdef HAVE_CDMA
    #define INITIALIZE_CDMA()   do \
                                { \
                                   void CdmaInit( void );\
                                   CdmaInit();\
                                } \
                                while(0)
#else
    #define INITIALIZE_CDMA()   do \
                                { \
                                } \
                                while(0)
#endif

#ifdef HAVE_AUDIO
    #define INITIALIZE_AUDIO()   do \
                                { \
                                   void AudioInit( void );\
                                   AudioInit();\
                                } \
                                while(0)
#else
    #define INITIALIZE_AUDIO()   do \
                                { \
                                } \
                                while(0)
#endif


#ifdef HAVE_JBIG
#define INITIALIZE_JBIG() do \
                           { \
                              void jbig_init( void );\
                              jbig_init();\
                           } \
                           while(0)
#else
#define INITIALIZE_JBIG() do \
                           { \
                           } \
                           while(0)
#endif

#ifdef HAVE_PRINT_SUPPORT

    #define INITIALIZE_PRINT() do \
                               { \
                                  void JobMgrInit( void );\
                                  void PrintMgrInit( void );\
                                  JobMgrInit();\
                                  PrintMgrInit();\
                               } \
                               while(0)

    #define USB_PRINT_EP USB_PRINT
    #define PRINT_STATUS 1
    #define NET_PRINT_PORT 1

    #define INITIALIZE_ENGINE() do \
                                { \
                                   void engine_init( void );\
                                   engine_init();\
                                } \
                                while(0)

    extern void engine_shutdown( void );
    #define SHUTDOWN_ENGINE engine_shutdown 


#ifdef HAVE_VIDEO_STUB    
    extern void videomgr_init( void );

    #define INITIALIZE_VIDEO() do \
                               {\
                                   videomgr_init();\
                                }\
                                while(0)
                                    
#else                                    

#ifdef HAVE_VIDEO_APP    
#ifdef __linux__
    #define INITIALIZE_VIDEO() do \
                               {\
                                   void video_thread_init(void);        \
                                   video_thread_init();                 \
                                }\
                                while(0)
#else
    #define INITIALIZE_VIDEO() do \
                               {\
                                   void video_init(void);               \
                                   void video_thread_init(void);        \
                                   video_init();                        \
                                   video_thread_init();                 \
                                }\
                                while(0)
#endif
#else                                    
    #define INITIALIZE_VIDEO() do \
                               {\
                                   void video_init(void);               \
                                   video_init();                        \
                                }\
                                while(0)
#endif
                                    
#endif


#else  //printing support not enabled
    #define INITIALIZE_PRINT()  do \
                                { \
                                } \
                                while(0)

    #define INITIALIZE_ENGINE() do \
                                { \
                                } \
                                while(0)

    #define SHUTDOWN_ENGINE()  do \
                                { \
                                } \
                                while(0)

    #define USB_PRINT_EP 0
    #define PRINT_STATUS 0
    #define NET_PRINT_PORT 0

    #define INITIALIZE_VIDEO() do \
                               { \
                               } \
                               while(0)
#endif

#ifdef HAVE_URF_SUPPORT
    #define INITIALIZE_URF_PARSER() do \
                                    { \
                                        void urf_parser_init( void );\
                                        urf_parser_init(); \
                                    } while(0)
#else
    #define INITIALIZE_URF_PARSER() do \
                                   { \
                                   } \
                                   while(0)
#endif




#ifdef HAVE_ZJS_SUPPORT
    #define INITIALIZE_ZJ_PARSER() do \
                                   { \
                                      void agzjParserInit( void );\
                                      agzjParserInit();\
                                   } \
                                   while(0)
    #define ZJS_STATUS 1
#else
    #define INITIALIZE_ZJ_PARSER() do \
                                   { \
                                   } \
                                   while(0)

    #define ZJS_STATUS 0
#endif

#ifdef HAVE_ZPF_SUPPORT
    #define INITIALIZE_PF_PARSER() do \
                                   { \
                                      void agpfParserInit( void );\
                                      agpfParserInit();\
                                   } \
                                   while(0)
    #define ZPF_STATUS 1
#else
    #define INITIALIZE_PF_PARSER() do \
                                   { \
                                   } \
                                   while(0)

    #define ZPF_STATUS 0
#endif

#ifdef HAVE_ZPCFAX_SUPPORT
    #define INITIALIZE_PCFAX_PARSER() do \
                                   { \
                                      void agpcfaxParserInit( void );\
                                      agpcfaxParserInit();\
                                   } \
                                   while(0)
    #define ZPCFAX_STATUS 1
#else
    #define INITIALIZE_PCFAX_PARSER() do \
                                   { \
                                   } \
                                   while(0)

    #define ZPCFAX_STATUS 0
#endif

#ifdef HAVE_GGS_RIP_SUPPORT
    #define INITIALIZE_GGS_PARSER() do \
                                   { \
                                      void ggs_ParserInit( void );\
                                      ggs_ParserInit();\
                                   } \
                                   while(0)
#else
    #define INITIALIZE_GGS_PARSER() do \
                                   { \
                                   } \
                                   while(0)
#endif

#ifdef HAVE_SYSTEM_SERVICE_LAYER
#include "SysServiceMgr.h"
#define INITIALIZE_SYS_SERVICE() do \
				   { \
						  SystemServiceInit();\
				   } \
				   while(0)
#define INITIALIZE_SYS_UI_SENDER_INIT() do \
				   { \
						  SysServiceSenderInit();\
				   } \
				   while(0)
#else
#define INITIALIZE_SYS_SERVICE() do \
				{ \
				} \
				while(0)
#define INITIALIZE_SYS_UI_SENDER_INIT() do \
				{ \
				} \
				while(0)
#endif

#ifdef HAVE_TINY_DEBUGGER
#include "TinyDebugger.h"
#define INITIALIZE_TINY_DEBUGGER() do \
				   { \
						  TinyDebuggerInit();\
				   } \
				   while(0)
#else
#define INITIALIZE_TINY_DEBUGGER() do \
				{ \
				} \
				while(0)
#endif

#ifdef HAVE_CARD_READER
#include "SysCardReaderMgr.h"
#define INITIALIZE_CARD_READER() do \
				   { \
						  SysServiceCardReaderInit();\
				   } \
				   while(0)
#else
#define INITIALIZE_CARD_READER() do \
				{ \
				} \
				while(0)
#endif

///TODO: Warning:Remove or add proper UI initialization
#define INITIALIZE_UI_MANAGER() do \
			{ \
				system("/app/UIManager -qws -nomouse &"); \
			} \
			while(0)

#ifdef HAVE_HB_SUPPORT
    #define INITIALIZE_HB_PARSER() do \
                                   { \
                                      void aghbParserInit( void );\
                                      aghbParserInit();\
                                   } \
                                   while(0)
#else
    #define INITIALIZE_HB_PARSER()  do \
                                    { \
                                    } \
                                    while(0)
#endif

#ifdef HAVE_USB_HOST_PARSER
    #define INITIALIZE_USB_HOST_PARSER() do \
                                   { \
                                     void print_to_usb_host_parser_init(void);\
                                      void passthru_parser_init(void);\
                                      print_to_usb_host_parser_init(); \
                                      passthru_parser_init();\
                                   } \
                                   while(0)
#else
    #define INITIALIZE_USB_HOST_PARSER()  do \
                                    { \
                                    } \
                                    while(0)
#endif

#ifdef HAVE_USB_GADGET
    #define INITIALIZE_USB_GADGET() do \
                                   { \
                                      pipe_io_device_init();       \
                                   } \
                                   while(0)
#else
    #define INITIALIZE_USB_GADGET()  do \
                                    { \
                                    } \
                                    while(0)
#endif 

#ifdef HAVE_PHOTO
    #include "photo_api.h"
    #define INITIALIZE_PHOTO() do \
                               { \
                                  photo_init();\
                               } \
                               while(0)
    #define PHOTO_STATUS 1    // allows photo to register with status manager
#else
    #define INITIALIZE_PHOTO() do \
                               { \
                               } \
                               while(0)

    #define PHOTO_STATUS 0
#endif



#ifdef HAVE_CURL
    //moved to main.c #include <curl/curl.h> 
    //moved to main.c #define INITIALIZE_CURL() curl_global_init(CURL_GLOBAL_ALL)
#else
    #define INITIALIZE_CURL() do { } while (0)
#endif



#ifdef HAVE_EMAIL
    #define INITIALIZE_EMAIL() do                          \
                               {                           \
                                   void email_init( void );\
                                   email_init();           \
                               }                           \
                               while(0)
#else
    #define INITIALIZE_EMAIL() _DO_NOTHING
#endif

#ifdef HAVE_EMAIL_APPS
    #define INITIALIZE_EMAIL_APPS() do                                 \
                               {                                       \
                                   void file_to_email_init( void );    \
                                   void scan_to_email_init( void );    \
                                   void jpg2pdf_to_email_init( void ); \
                                   file_to_email_init();               \
                                   scan_to_email_init();               \
                                   jpg2pdf_to_email_init();            \
                               }                                       \
                               while(0)
#else
    #define INITIALIZE_EMAIL_APPS() _DO_NOTHING
#endif

#ifdef HAVE_USB_PHY
#include "usb_phy_api.h"
    #define INITIALIZE_USB_PHY() do \
                                 {  \
                                     usb_phy_init();            \
                                 }  \
                                 while(0)
#else
    #define INITIALIZE_USB_PHY() do \
                                 {  \
                                 }  \
                                 while(0)
#endif


#ifdef HAVE_USB_HOST
    #define INITIALIZE_USB_HOST() do \
                                  { \
                                     void usb_host_init( void );\
                                     usb_host_init();\
                                  } \
                                  while(0)
#else
    #define INITIALIZE_USB_HOST() do \
                                  { \
                                  } \
                                  while(0)

#endif


#ifdef HAVE_MEDIA_MANAGER
    #include "media_manager_api.h"
    #define INITIALIZE_MEDIA_MANAGER() do \
                                      { \
                                          media_manager_init(); \
                                      } \
                                      while(0)
#else
    #define INITIALIZE_MEDIA_MANAGER() do \
                                      { \
                                      } \
                                      while(0)

#endif

#ifdef HAVE_USB_MASS_STORAGE_DEVICE
    #define INITIALIZE_MASS_STORAGE_DEVICE() do                            \
                                             {                             \
                                                 void mass_storage_init(); \
                                                 mass_storage_init();      \
                                             }                             \
                                             while(0)
#else
    #define INITIALIZE_MASS_STORAGE_DEVICE() do \
                                      { \
                                      } \
                                      while(0)

#endif


#ifdef HAVE_USB_MASS_STORAGE
    #include "mass_storage_app_api.h"
    #define INITIALIZE_MASS_STORAGE() do \
                                      { \
                                          mass_storage_app_init(); \
                                      } \
                                      while(0)
#else
    #define INITIALIZE_MASS_STORAGE() do \
                                      { \
                                      } \
                                      while(0)

#endif

#ifdef HAVE_MASS_STORAGE_REFLECTOR
    #define INITIALIZE_MASS_STORAGE_REFLECTOR() do \
                                      { \
                                          mass_storage_reflector_init(); \
                                      } \
                                      while(0)
    #define USB_MS_EP USB_MASS_STORAGE
#else
    #define USB_MS_EP 0
    #define INITIALIZE_MASS_STORAGE_REFLECTOR() do \
                                      { \
                                      } \
                                      while(0)

#endif

#ifdef HAVE_CUNIT_TEST
    #define CUNIT_STATUS 1
    #define INITIALIZE_CUNIT_TEST() do \
                                    { \
                                        void cunitInit(); \
                                        cunitInit(); \
                                    } \
                                    while(0)
#else
    #define CUNIT_STATUS 0
    #define INITIALIZE_CUNIT_TEST() do \
                                    { \
                                    } \
                                    while(0)

#endif

#ifdef HAVE_PICTBRIDGE_SUPPORT
    #define INITIALIZE_PICTBRIDGE() do \
                                    { \
                                       void pbMgrInit( void );\
                                       pbMgrInit();\
                                    } \
                                    while(0)
#else
    #define INITIALIZE_PICTBRIDGE()  do \
                                     { \
                                     } \
                                     while(0)

#endif

#ifdef HAVE_SCAN_SUPPORT
    #define INITIALIZE_SCANNER() do \
                                 { \
                                    void ScanManInit( void ); \
                                    void ScanAppInit( void ); \
                                    void ScanAppUiScanInit( void );\
                                    ScanManInit();\
                                    ScanAppInit();\
                                    ScanAppUiScanInit();\
                                 } \
                                 while(0)
    #define USB_SCANNER_EP USB_SCAN
    #define NET_SCAN_PORT NET_SCAN
    #define SCANNER_STATUS 3    ///< Scanner reports status; one for ScanApp, one for CopyApp
#else
  #ifdef HAVE_POWER_MGR
    #if 0 /* not implemented here */
    #define INITIALIZE_SCANNER() do \
                                 { \
                                       void ice_pwr_down(void);\
                                       ice_pwr_down();\
                                 } \
                                 while(0)
    #endif
    #define INITIALIZE_SCANNER() do \
                                { \
                                } while(0)

    #define USB_SCANNER_EP 0
    #define NET_SCAN_PORT 0
    #define SCANNER_STATUS  0

  #else
    #define INITIALIZE_SCANNER() do \
                                { \
                                } while(0)
    #define USB_SCANNER_EP 0
    #define NET_SCAN_PORT 0
    #define SCANNER_STATUS  0
  #endif
#endif
#ifdef HAVE_COPY_SUPPORT
    #define INITIALIZE_COPY() do {\
                                 void CopyAppInit(void); \
                                 CopyAppInit(); \
    } \
    while(0)
#else
    #define INITIALIZE_COPY() do \
                                { \
                                } while(0)
#endif

#ifdef HAVE_COPY_TO_HOST
    #define INITIALIZE_COPY_TO_HOST() do {\
                                 void cth_init(void); \
                                 cth_init(); \
    } \
    while(0)
#else
    #define INITIALIZE_COPY_TO_HOST() do \
                                { \
                                } while(0)
#endif

#ifdef HAVE_DELAY
    #if  (!defined( __linux__) && !defined( HAVE_TIMER))
      #error "Must have timer drvr defined to use delay mgr"
    #endif
    #define INITIALIZE_DELAY_MGR() do { \
                                void DelayMgrInit(void); \
                                DelayMgrInit(); \
                                } while(0)
#else
    #define INITIALIZE_DELAY_MGR() do {} while(0)
#endif

#ifdef HAVE_TIMER
    #define INITIALIZE_TIMER() do { \
            void TimerInit(void); \
            TimerInit(); \
        } while(0)
#else
    #define INITIALIZE_TIMER() do {} while(0)
#endif

#ifdef HAVE_FAX_SUPPORT
    #define INITIALIZE_FAX() do \
                             { \
                                void fax_app_init( void ); \
                                fax_app_init();\
                                void fax_demo_init(void);\
                                fax_demo_init();\
                             } \
                             while(0)
    #define USB_FAX_EP USB_FAX
    #define FAX_STATUS 2        ///< 2 resources in fax so 2 status slots.
#else
    #define INITIALIZE_FAX() do \
                             { \
                             } \
                             while(0)

    #define USB_FAX_EP 0
    #define FAX_STATUS 0
#endif

#ifdef HAVE_RTC
#include "rtc_api.h"
    #define INITIALIZE_RTC() do \
                             { \
                                rtc_init();\
                             } \
                             while(0)
#else
    #define INITIALIZE_RTC() do \
                             { \
                             } \
                             while(0)
#endif

#ifdef HAVE_PJL_SUPPORT
    #define INITIALIZE_PJL() do \
                             { \
                                void pjl_init( void );\
                                pjl_init();\
                             } \
                             while(0)
    #define PJL_STATUS 1
#else
    #define INITIALIZE_PJL() do \
                             { \
                             } \
                             while(0)

    #define PJL_STATUS 0
#endif

#ifdef HAVE_ACL
    #define INITIALIZE_ACL() do \
                             { \
                                void localACLInit( void );\
                                void agiACLParserInit( void );\
                                agiACLParserInit();\
                             } \
                             while(0)
    #define ACL_STATUS  1
#else
    #define INITIALIZE_ACL() do \
                             { \
                             } \
                             while(0)

    #define ACL_STATUS 0 ///< ACL will not report status.
#endif

#ifdef HAVE_HTTP
    #define INITIALIZE_HTTP() do \
                              { \
                                 void http_init( void );\
                                 http_init();\
                              } \
                              while(0)

    #define USB_HTTP_EP USB_HTTP
#else
    #define INITIALIZE_HTTP() do \
                              { \
                              } \
                              while(0)

    #define USB_HTTP_EP 0
#endif

#if(defined(HAVE_HTTP) && defined(HAVE_DEBUG_OVER_HTTP))

    #define INITIALIZE_DEBUG_OVER_HTTP() do \
                              { \
                                 void dbg_http_task_init( void );\
                                 dbg_http_task_init();\
                              } \
                              while(0)
#else

    #define INITIALIZE_DEBUG_OVER_HTTP() do \
                              { \
                              } \
                              while(0)
#endif


#ifdef HAVE_FAT_FILESYSTEM

#include "fat_api.h"

    #define INITIALIZE_FAT_FILESYSTEM() do \
                                    { \
                                        fat_init();\
                                    } \
                                    while(0)
#else
    #define INITIALIZE_FAT_FILESYSTEM() do \
                                    { \
                                    } \
                                    while(0)

#endif

// The usb init must come after anything that might define a new endpoint above.
#ifdef HAVE_USB_DEVICE
    #define INITIALIZE_USB_CLIENT() do \
                                    { \
                                       void usb_device_init(UINT32);\
                                       usb_device_init(USB_MS_EP | USB_PRINT_EP | USB_SCANNER_EP | USB_HTTP_EP| USB_FAX_EP);\
                                    } \
                                    while(0)



#else
    #define INITIALIZE_USB_CLIENT() do     \
                                    {    \
                                    } \
                                    while(0)
#endif

#ifdef HAVE_USB_CLIENT
    #define SHUTDOWN_USB_CLIENT() do  \
                                  {            \
                                    void ShutDownUSB( void );  \
                                    ShutDownUSB();   \
                                  } \
                                  while(0)
#else

    #define SHUTDOWN_USB_CLIENT() do  \
                                  {    \
                                  } \
                                  while(0)

#endif


#ifdef HAVE_NETWORK
     #define INITIALIZE_NETWORK() do \
                                  {\
                                     void net_iface_init(void); \
                                     net_iface_init();\
                                  }        \
                                  while(0)

     #define SHUTDOWN_NETWORK()   do \
                                  {\
                                     void net_iface_shutdown(void); \
                                     net_iface_shutdown(); \
                                  }        \
                                  while(0)
#else
     #define INITIALIZE_NETWORK() do \
                                  { \
                                  } \
                                  while(0) 

     #define SHUTDOWN_NETWORK() do \
                                  {\
                                  }        \
                                  while(0)
#endif

#ifdef HAVE_IPP

     #define INITIALIZE_IPP() do \
                                  {\
                                     void ipp_init(void); \
                                     ipp_init();\
                                  }        \
                                  while(0)
#else
     #define INITIALIZE_IPP() do \
                                  { \
                                  } \
                                  while(0)
#endif

#ifdef HAVE_NET_SM
     #define INITIALIZE_NET_SM() do \
                                  {\
                                     void net_sm_init(void); \
                                     net_sm_init();\
                                  }        \
                                  while(0)
#else
     #define INITIALIZE_NET_SM() do \
                                  { \
                                  } \
                                  while(0)
#endif

#ifdef HAVE_SM_JOB
    #include "sm_job_api.h"
    #define INITIALIZE_SM_JOB() do                  \
                                {                   \
                                    smjob_init();  \
                                }                   \
                                while(0)

#else
    #define INITIALIZE_SM_JOB() do  \
                                {   \
                                }   \
                                while(0)
#endif

#if 0
#ifdef HAVE_ETHERNET
     #include <net_eth_api.h>
     #define INITIALIZE_ETHERNET() do \
                                  {\
                                     net_eth_init();\
                                  }        \
                                  while(0)
#else // !HAVE_ETHERNET
     #define INITIALIZE_ETHERNET() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_ETHERNET
#endif // 0

#ifdef HAVE_MDNS
     #include <net_mdns_api.h>
     #define INITIALIZE_MDNS() do \
                                  {\
                                     net_mdns_init();\
                                  }        \
                                  while(0)
#else // !HAVE_MDNS
     #define INITIALIZE_MDNS() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_MDNS


#ifdef HAVE_NBNS
     #include <net_nbns_api.h>
     #define INITIALIZE_NBNS() do \
                                  {\
                                     net_nbns_init();\
                                  }        \
                                  while(0)
#else // !HAVE_NBNS
     #define INITIALIZE_NBNS() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_NBNS


#ifdef HAVE_SLP
     #include <net_slp_api.h>
     #define INITIALIZE_SLP() do \
                                  {\
                                     net_slp_init();\
                                  }        \
                                  while(0)
#else // !HAVE_SLP
     #define INITIALIZE_SLP() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_SLP


#if defined HAVE_SNMP || HAVE_SNMP_SUBAGENT
     #define INITIALIZE_SNMP() do \
                                  {\
                                     void net_snmp_init(void); \
                                     net_snmp_init();\
                                  }        \
                                  while(0)
#else // !HAVE_SNMP
     #define INITIALIZE_SNMP() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_SNMP


#ifdef HAVE_SNTP
     #include <net_sntp_api.h>
     #define INITIALIZE_SNTP() do \
                                  {\
                                     net_sntp_init();\
                                  }        \
                                  while(0)
#else // !HAVE_SNTP
     #define INITIALIZE_SNTP() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_SNTP


#ifdef HAVE_LLTD
     #include "lltd_api.h"
     #define INITIALIZE_LLTD() do \
                                  {\
                                     lltd_init();\
                                  }        \
                                  while(0)
#else // !HAVE_LLTD
     #define INITIALIZE_LLTD() do \
                                  { \
                                  } \
                                  while(0) 
#endif // HAVE_LLTD

#ifdef HAVE_WSD
    #include "net_wsd_api.h"
    #define INITIALIZE_WSD() do                     \
        {                                           \
            net_wsd_init( );                        \
        } while ( 0 );
#else
    #define INITIALIZE_WSD() do                     \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_WSD_PRINT
    #include "net_wsd_api.h"
    #define INITIALIZE_WSD_PRINT() do               \
        {                                           \
            wsd_print_init( );                      \
        } while ( 0 );
#else
    #define INITIALIZE_WSD_PRINT() do               \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_WSD_SCAN
    #define INITIALIZE_WSD_SCAN() do               \
        {                                           \
            void wsd_scan_init( void );            \
            wsd_scan_init( );                      \
        } while ( 0 );
#else
    #define INITIALIZE_WSD_SCAN() do               \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_GCPP

    #define INITIALIZE_GCPP() do                    \
        {                                           \
			void net_gcpp_init( void );             \
            net_gcpp_init( );                       \
        } while ( 0 );
#else
    #define INITIALIZE_GCPP() do                    \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_SPI
#include "spi_api.h"
    #define INITIALIZE_SPI() do \
                               { \
                                  spi_init();\
                               } \
                               while (0)
#else
    #define INITIALIZE_SPI() do \
                               { \
                               } \
                               while(0)

#endif

#ifdef HAVE_PWM
#include "pwm_api.h"
    #define INITIALIZE_PWM() do \
                               { \
                                  pwm_init();\
                               } \
                               while (0)
#else
    #define INITIALIZE_PWM() do \
                               { \
                               } \
                               while(0)

#endif
#ifdef HAVE_GPIO_LED
#include "gpio_led.h"
    #define INITIALIZE_GPIO_LED() do \
                               { \
                                  gpio_led_init();\
                               } \
                               while (0)
#else
    #define INITIALIZE_GPIO_LED() do \
                               { \
                               } \
                               while(0)

#endif

#ifdef HAVE_SWITCHED_PWR
    #define INITIALIZE_SWITCHED_PWR() do \
                               { \
                                  void switched_pwr_init();\
                                  switched_pwr_init();\
                               } \
                               while (0)
#else
    #define INITIALIZE_SWITCHED_PWR() do \
                               { \
                               } \
                               while(0)

#endif
#ifdef HAVE_TEXT_PAD
#include "text_pad_api.h"
    #define INITIALIZE_TEXT_PAD()   do \
                                    {\
                                       text_pad_init();\
                                    }\
                                    while(0)
#else
    #define INITIALIZE_TEXT_PAD()   do \
                                    {\
                                    }\
                                    while(0)
#endif

#ifdef HAVE_CONTROL_PANEL
#include "control_panel_api.h"
      #define INITIALIZE_CONTROL_PANEL() do \
                                     { \
                                        control_panel_init();\
                                     }        \
                                     while(0)
#else
      #define INITIALIZE_CONTROL_PANEL() do \
                                     { \
                                     } \
                                     while(0)
#endif


#ifdef HAVE_MIP 
#include "mip_api.h"
#define INITIALIZE_MIP()   do \
                                    {\
                                       mip_init();\
                                    }\
                                    while(0)
#else
#define INITIALIZE_MIP()   do \
                                    {\
                                    }\
                                    while(0)
#endif


#if defined (HAVE_ODMA) && !defined(__linux__)
#include "odma_api.h"
#define INITIALIZE_ODMA()   do \
                                    {\
                                       odma_init();\
                                    }\
                                    while(0)
#else
#define INITIALIZE_ODMA()   do \
                                    {\
                                    }\
                                    while(0)
#endif


#ifdef HAVE_PIP 
#include "pip_api.h"
#define INITIALIZE_PIP()   do \
                                    {\
                                       pip_init();\
                                    }\
                                    while(0)
#else
#define INITIALIZE_PIP()   do \
                                    {\
                                    }\
                                    while(0)
#endif


#ifdef HAVE_UI
#include "ui_api.h"
      #define INITIALIZE_UI() do \
                                     { \
                                        ui_init();\
                                     }        \
                                     while(0)

      /* TODO */
      #define SHUTDOWN_UI() do \
                                     { \
                                     } \
                                     while(0)
#else
      #define INITIALIZE_UI() do \
                                     { \
                                     } \
                                     while(0)
      #define SHUTDOWN_UI() do \
                                     { \
                                     } \
                                     while(0)
#endif

#ifdef HAVE_KINOMA
#include "kinoma_api.h"
      #define INITIALIZE_KINOMA() do \
                                     { \
                                        KplInit();\
                                     }        \
                                     while(0)
#else
      #define INITIALIZE_KINOMA() do \
                                     { \
                                     } \
                                     while(0)
#endif

#if defined(WIRELESS) || defined(NETWORK)
      #define INITIALIZE_RAW_SOCKETS() {\
                                        void raw_socket_init();\
                                        raw_socket_init();\
                                       }
#else
    #define INITIALIZE_RAW_SOCKETS()
#endif

#if 0
#ifdef WIRELESS
#ifndef USB8682
      #include <net_wlan_api.h>
      #define INITIALIZE_WIRELESS() do \
                                    { \
                                       net_wlan_init();\
                                    }        \
                                    while(0)
#else
      #define INITIALIZE_WIRELESS()  do \
                                   { \
                                   } \
                                   while(0)
#endif // #ifndef USB8682
#else
    #define INITIALIZE_WIRELESS()  do \
                                   { \
                                   } \
                                   while(0)
#endif
#endif // 0

#ifdef HAVE_INTERNAL_PAGE_MGR
#include "internal_page_api.h"
    #define INITIALIZE_INTERNAL_PAGE_MGR() do \
                                           { \
                                               ipage_init();\
                                           } \
                                           while(0)

   #define INTRLPG_STATUS 1
#else
    #define INITIALIZE_INTERNAL_PAGE_MGR() do \
                                           { \
                                           } \
                                           while(0)

    #define INTRLPG_STATUS 0
#endif

#ifdef SPIMEM
    #define INITIALIZE_SPIMEM()  do \
                                 { \
                                    ATSTATUS SPIInit( void ); \
                                    SPIInit(); \
                                 } \
                                 while(0)

#else
    #define INITIALIZE_SPIMEM()  do \
                                 { \
                                 } \
                                 while(0)
#endif

// Begin NAND, BootSPI and BootI2C

#ifdef HAVE_NAND
    #define INITIALIZE_NAND()  do \
                                 { \
                                    void nand_init( void ); \
                                    nand_init(); \
                                 } \
                                 while(0)

#else
    #define INITIALIZE_NAND()  do \
                                 { \
                                 } \
                                 while(0)
#endif

#if defined(HAVE_NAND) && defined(HAVE_NAND_PLUS)
    #define INITIALIZE_NAND_PLUS()  do \
                                 { \
                                    void nand_plus_init( void ); \
                                    nand_plus_init(); \
                                 } \
                                 while(0)

#else
    #define INITIALIZE_NAND_PLUS()  do \
                                 { \
                                 } \
                                 while(0)
#endif

#ifdef HAVE_BOOTSPI
    #define INITIALIZE_BOOTSPI()  do \
                                 { \
                                    void init_boot_spi_api( void ); \
                                    init_boot_spi_api(); \
                                 } \
                                 while(0)

#else
    #define INITIALIZE_BOOTSPI()  do \
                                 { \
                                 } \
                                 while(0)
#endif

#ifdef HAVE_I2C
    #define INITIALIZE_BOOTI2C()  do \
                                 { \
                                    void i2cInit( void ); \
                                    i2cInit(); \
                                 } \
                                 while(0)

#else
    #define INITIALIZE_BOOTI2C()  do \
                                 { \
                                 } \
                                 while(0)
#endif

#ifdef HAVE_EEPROM
     #define INITIALIZE_EEPROM() do \
                                 { \
                                    void eepromInit(void); \
                                    eepromInit(); \
                                  } \
                                  while(0)

#else
     #define INITIALIZE_EEPROM() do\
                                  {\
                                  }\
                                  while(0)

#endif

// End NAND, BootSPI and BootI2C

#ifdef HAVE_PROD_PJL
    #define INITIALIZE_PRODPJL()   do \
                                   { \
                                      void PJLProdInit( void );\
                                      PJLProdInit();\
                                   } \
                                   while(0)
#else
    #define INITIALIZE_PRODPJL()   do \
                                   { \
                                   } \
                                   while(0)

#endif

#ifdef CPU_USAGE_PROFILER
    #define INITIALIZE_PROFILE()   do \
                                   { \
                                      void InitProfileCode( void ); \
                                      InitProfileCode(); \
                                   }  \
                                   while(0)
#else
    #define INITIALIZE_PROFILE()   do \
                                   { \
                                   } \
                                   while(0)
#endif

#ifdef HAVE_WATCHDOG
#ifdef DISABLE_WATCHDOG
    #define INITIALIZE_WATCHDOG() do \
                                  {    \
                                    void disable_watchdog_timer(void); \
                                     disable_watchdog_timer();\
                                  }        \
                                  while(0)

#else
    #define INITIALIZE_WATCHDOG() do \
                                  {     \
                                     void init_watchdog(uint32_t, uint32_t); \
                                     init_watchdog(5, 1); \
                                  }        \
                                  while(0)

#endif // DISABLE_WATCHDOG
#else
    #define INITIALIZE_WATCHDOG() do \
                                  { \
                                  } \
                                  while(0)

#endif // HAVE_WATCHDOG

#ifdef HAVE_UIO_LIB
#include "uio_lib_api.h"
    #define INITIALIZE_UIO_LIB() do \
                              {        \
                                 uio_lib_init();    \
                              }        \
                              while(0)

#else
    #define INITIALIZE_UIO_LIB() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_GPIO
#include "gpio_api.h"
    #define INITIALIZE_GPIO() do \
                              {        \
                                 gpio_init();    \
                              }        \
                              while(0)

#else
    #define INITIALIZE_GPIO() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_STRMGR
    #define INITIALIZE_STRMGR() do \
                                { \
                                   void StrMgrInit ( void ); \
                                   StrMgrInit();\
                                }        \
                                while(0)
#else
    #define INITIALIZE_STRMGR() do \
                                { \
                                } \
                                while(0)

#endif

#ifdef HAVE_FILESYSTEM

    #define INITIALIZE_FILESYSTEM() do                         \
                                    {                          \
					void FileSystemInit(); \
                                        FileSystemInit();      \
                                    }                          \
                                    while(0)
#else
    #define INITIALIZE_FILESYSTEM() do \
                                { \
                                } \
                                while(0)

#endif

#ifdef HAVE_SD
#include "sd_api.h"
    #define INITIALIZE_SD() do \
                            { \
                                sd_init(); \
                            } \
                            while (0)
#else
    #define INITIALIZE_SD() do \
                            { \
                            } \
                            while (0)
#endif

#ifdef HAVE_PAWN
    #define INITIALIZE_PAWN()   do \
                                   { \
                                      int32_t vm_api_init( void );\
                                      vm_api_init();\
                                   } \
                                   while(0)
#else
    #define INITIALIZE_PAWN()   do \
                                   { \
                                   } \
                                   while(0)
#endif

#ifdef HAVE_JPEG
    #define INITIALIZE_JPEG_SUPPORT() do \
                                         { \
                                         } \
                                      while(0)    
#else
    #define INITIALIZE_JPEG_SUPPORT() do \
                                         { \
                                         } \
                                      while(0)    
#endif

#ifdef HAVE_JPEG_HW
    #define INITIALIZE_JPEG_HW() do \
                                         { \
                                             void jediSysInit(void);\
                                             jediSysInit();\
                                         } \
                                      while(0)    
#else
    #define INITIALIZE_JPEG_HW() do \
                                         { \
                                         } \
                                      while(0)    
#endif

#ifdef HAVE_BOREALIS  
    #define INITIALIZE_BOREALIS_PARSER() do \
                                         { \
                                             UINT32 BorParserInit( void );\
                                             BorParserInit();\
                                         } \
                                      while(0)    
#else
    #define INITIALIZE_BOREALIS_PARSER() do \
                                         { \
                                         } \
                                      while(0)    
#endif

#ifdef HAVE_COPY_COMPRESSION  
    #define INITIALIZE_COMPRESSION() do \
                                         { \
                                             UINT32 CompressionInit( void );\
                                             CompressionInit();\
                                         } \
                                      while(0)    
#else
    #define INITIALIZE_COMPRESSION() do \
                                         { \
                                         } \
                                      while(0)    
#endif

#ifdef HAVE_OID
    #define INITIALIZE_OID() do \
                             { \
                                 void oid_controller_init( void );\
                                 oid_controller_init(); \
                             } \
                             while(0)
#else
    #define INITIALIZE_OID() do \
                             { \
                             } \
                             while(0)    
#endif


#ifdef HAVE_STRING_MGR
    #include "string_mgr_api.h"
    #define INITIALIZE_STRING_MGR() do \
                                { \
                                    string_mgr_init(); \
                                } \
                                while(0)
#else
    #define INITIALIZE_STRING_MGR() do \
                             { \
                             } \
                             while(0)    
#endif


#ifdef HAVE_ACL_DEV_PROTOCOL
    #define INITIALIZE_ACL_DEV_PROTOCOL() do \
                                { \
                                    AclDevProtocolHandlerInit(); \
                                } \
                                while(0)
#else
    #define INITIALIZE_ACL_DEV_PROTOCOL() do \
                             { \
                             } \
                             while(0)    
#endif

#ifdef HAVE_PLATFORM
#include "platform_api.h"
#define INITIALIZE_PLATFORM_VARS() do \
                                     { \
                                         platvars_init();\
                                     }        \
                                     while(0)

#define INITIALIZE_PLATFORM_1284_STRING() do \
                                     { \
                                         platform_init_1284_string();\
                                     }        \
                                     while(0)

#define SHUTDOWN_PLATFORM_VARS() do \
                             { \
                                 void platvars_shutdown( void );\
                                 platvars_shutdown();\
                             } \
                             while(0)
#else
#define INITIALIZE_PLATFORM_VARS() do \
                                     { \
                                     }        \
                                     while(0)

#define INITIALIZE_PLATFORM_1284_STRING() do \
                                     { \
                                     }        \
                                     while(0)

#define SHUTDOWN_PLATFORM_VARS() do \
                             { \
                             } \
                             while(0)
#endif

#ifdef HAVE_PAPER
#include "paper_size_api.h"
    #define INITIALIZE_PAPER_SIZE_API() do \
                              { \
                                 paper_size_init();\
                              } \
                              while(0)
#else
    #define INITIALIZE_PAPER_SIZE_API() do \
                              { \
                              } \
                              while(0)
#endif


#ifdef HAVE_UNIT_TEST
#include "unit_test_api.h"
    #define INITIALIZE_UNIT_TEST() do \
                              { \
                                 unit_test_init();\
                              } \
                              while(0)
#else
    #define INITIALIZE_UNIT_TEST() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_UTILS
#include "utils.h"
    #define INITIALIZE_UTILS() do \
                              { \
                                 utils_init();\
                              } \
                              while(0)
#else
    #define INITIALIZE_UTILS() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_STATISTICS
#include "statistics_api.h"
    #define INITIALIZE_STATISTICS() do \
                              { \
                                 statistics_init();\
                              } \
                              while(0)
#else
    #define INITIALIZE_STATISTICS() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef BOARD
#define INITIALIZE_BOARD_PADS() do                  \
    {                                               \
        void pad_config_init(void);                 \
        pad_config_init();                          \
    }                                               \
    while(0)
#else
#define INITIALIZE_BOARD_PADS() do { } while(0)
#endif

#ifdef HAVE_POWER_MGR
    #define INITIALIZE_POWER_MGR() do \
        { \
            error_type_t pwr_mgr_init( void );\
            void local_pwr_mgr_init(void);\
            pwr_mgr_init(); \
            local_pwr_mgr_init(); \
        } while(0)
#else
    #define INITIALIZE_POWER_MGR() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_ASIC_POWER_DRIVER
    #define INITIALIZE_POWER_ASIC() do \
        { \
            void asic_power_init(void);\
            asic_power_init(); \
        } while(0)
#else
    #define INITIALIZE_POWER_ASIC() do \
                              { \
                              } \
                              while(0)
#endif

#ifdef HAVE_POWER_MGR
    #define INITIALIZE_POWER_CPU() do \
        { \
            void cpu_power_init(void); \
            cpu_power_init(); \
        } while(0)
#else
    #define INITIALIZE_POWER_CPU() do \
                              { \
                              } \
                              while(0)
#endif


#ifdef HAVE_DEC
    #define INITIALIZE_DEC() do                     \
        {                                           \
            void dec_init( void );                  \
            dec_init();                             \
        } while(0)
#else
    #define INITIALIZE_DEC() do                     \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_DEC_SENSOR
    #define INITIALIZE_DEC_SENSOR() do              \
        {                                           \
            void dec_sensor_init( void );           \
            dec_sensor_init();                      \
        } while(0)
#else
    #define INITIALIZE_DEC_SENSOR() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_DEC_ADC
    #define INITIALIZE_DEC_ADC() do              \
        {                                           \
            void dec_adc_init( void );           \
            dec_adc_init();                      \
        } while(0)
#else
    #define INITIALIZE_DEC_ADC() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_DEC_FUSER
    #define INITIALIZE_DEC_FUSER() do              \
        {                                           \
            void dec_fuser_init( void );           \
            dec_fuser_init();                      \
        } while(0)
#else
    #define INITIALIZE_DEC_FUSER() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_DEC_LASER
    #define INITIALIZE_DEC_LASER() do              \
        {                                           \
            void dec_laser_init( void );           \
            dec_laser_init();                      \
        } while(0)
#else
    #define INITIALIZE_DEC_LASER() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_STEPPER
    #define INITIALIZE_STEPPER() do              \
        {                                           \
            void stepper_init( void );           \
            stepper_init();                      \
        } while(0)
#else
    #define INITIALIZE_STEPPER() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_DCMOTOR
    #define INITIALIZE_DCMOTOR() do              \
        {                                           \
            void dcmotor_init( void );           \
            dcmotor_init();                      \
        } while(0)
#else
    #define INITIALIZE_DCMOTOR() do              \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef HAVE_LOGGER
    #define INITIALIZE_LOGGER() do                      \
        {                                               \
            void logger_init(void);                     \
            logger_init();                              \
        } while(0);
#else
    #define INITIALIZE_LOGGER() do                      \
        {                                               \
            void logger_init(void);                     \
            logger_init();                              \
        }                                               \
        while(0)
#endif

#ifdef HAVE_ZLIB_APPS
    #define INITIALIZE_Z_APPS() do                      \
        {                                               \
            void z_apps_init(void);                     \
            z_apps_init();                              \
        } while(0);
#else
    #define INITIALIZE_Z_APPS() do                      \
        {                                               \
        }                                               \
        while(0)
#endif


#ifdef HAVE_HIPS_PLL
#include "hips_pll_api.h"
    #define INITIALIZE_HIPS_PLL() do                \
        {                                           \
             hips_pll_init();                       \
        } while(0)
#else
    #define INITIALIZE_HIPS_PLL() do                \
        {                                           \
        }                                           \
        while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif
