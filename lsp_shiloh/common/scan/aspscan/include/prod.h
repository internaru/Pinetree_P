/*
 * ============================================================================
 * Copyright (c) 2011 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

#ifndef PROD_H
#define PROD_H

#define SCANNER_VENDOR_ID   0x8086
#define SCANNER_PRODUCT_ID  0xac85

#define SCAN_INTERFACE     0
#define SCAN_OUT_ENDPOINT  1
#define SCAN_IN_ENDPOINT   (0x80 | SCAN_OUT_ENDPOINT)

#define PRINTER_INTERFACE  1
#define PRINT_ENDPOINT     2

#endif

