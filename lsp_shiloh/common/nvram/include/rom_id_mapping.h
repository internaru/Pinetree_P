/* 
 *
 * ============================================================================
 * Copyright (c) 2009-2010   Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 */

 #ifndef ROM_ID_MAPPING_H
 #define ROM_ID_MAPPING_H

#ifdef __cplusplus
extern "C" {
#endif

 typedef enum
 {
     ROMMGR_DEMO_PAGE = 4,
     ROMMGR_ENGLISH_FONT = 5,
     ROMMGR_SIMP_CHINESE,
     ROMMGR_SSL_CERT = 7,
     ROMMGR_SSL_KEY = 8,
     ROMMGR_GUI_IMAGES = 0x100, // next 256 reserved for gui images.
     
 } rommgr_id_mapping_t;

#ifdef __cplusplus
}
#endif

 #endif

