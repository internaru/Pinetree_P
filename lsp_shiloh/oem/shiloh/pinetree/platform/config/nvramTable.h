/*
 *
 * ============================================================================
 * Copyright (c) 2007-2010  Marvell International, Ltd. All Rights Reserved.
 *
 *                         Marvell Confidential
 * ============================================================================
 */
/**
 *
 * \brief 
 *
 */

#ifndef _NVRAMTABLE_H
#define _NVRAMTABLE_H

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_EEPROM_SIZE         0x8000 // 32K

#define NVRAM_STATIC_DATA_SIZE  0x0200  // 512 Byte static area
#define NVRAM_DYNAMIC_DATA_SIZE 0x0100  // 256 Byte dynamic area
//#define NVRAM_TOTAL_DATA_SIZE (NVRAM_STATIC_DATA_SIZE + NVRAM_DYNAMIC_DATA_SIZE)

// NVOFF_RTC_SET_VALID bit definitions (we are using them for more than RTC)

#define USE_USB_PHY 0x40
#define PS_MOD_ON 0x20
#define PLL_OVERRIDE_VALID 0x10
#define PLLDITHER_ON 0x8
#define ICACHE_ENABLE_ON 0x4
#define DCACHE_ENABLE_ON 0x2
#define RTCWTC_VALID  0x1

// NOTE: the nvram bootcode position defines should NEVER be used directly
// by the bootcode.  They are for use by external utility applicaitons
// such as nvImage.exe that generate the nvram image used to burn the nvram.
// The bootcode itself locates the backup image at runtime using the BOOTCODE_PTR
// defined in the static section.  This allows the dynamic data area to be
// expanded if necessary after the bootcode itself is finalized.
#define NVOFF_BOOTCODE_LEN      (NVRAM_STATIC_DATA_SIZE + NVRAM_DYNAMIC_DATA_SIZE)
#define NVLEN_BOOTCODE_LEN      2
#define NVOFF_BOOTCODE_CHECKSUM (NVOFF_BOOTCODE_LEN + NVLEN_BOOTCODE_LEN)
#define NVLEN_BOOTCODE_CHECKSUM 2
#define NVOFF_BOOTCODE_IMAGE    (NVOFF_BOOTCODE_CHECKSUM + NVLEN_BOOTCODE_CHECKSUM)


// static section (0x0000-0x0100) value offsets & lengths
#define NVOFF_STATIC_CHECKSUM   0x0000  // Checksum
#define NVLEN_STATIC_CHECKSUM   2
#define NVOFF_REGION_ID         NVOFF_STATIC_CHECKSUM+NVLEN_STATIC_CHECKSUM // address 2
#define NVLEN_REGION_ID         1
#define NVOFF_RESERVED          NVOFF_REGION_ID+NVLEN_REGION_ID // address 3
#define NVLEN_RESERVED          1
//
// The next 1 item needs to be aligned on a 4 byte boundary for boot code to use correctly
// 
#define NVOFF_PLL_SETTINGS      (NVOFF_RESERVED + NVLEN_RESERVED) // pll settings   address 4
#define NVLEN_PLL_SETTINGS      4      // 4 bytes are clock speed
#define NVOFF_NVRAM_REV         (NVOFF_PLL_SETTINGS+ NVLEN_PLL_SETTINGS)  // NVRAMRevision ad 8
#define NVLEN_NVRAM_REV         2
#define NVOFF_INSTALLDATE       (NVOFF_NVRAM_REV+NVLEN_NVRAM_REV)  // address 10
#define NVLEN_INSTALLDATE       7
#define NVOFF_PROD_SER_NUM      (NVOFF_INSTALLDATE+NVLEN_INSTALLDATE)  // ad 17 Product Serial Number - typically programmed with the engine serial number
#define NVLEN_PROD_SER_NUM      13
#define NVOFF_USB_VENDORID      (NVOFF_PROD_SER_NUM+NVLEN_PROD_SER_NUM)  // ad 30 USBVendorId
#define NVLEN_USB_VENDORID      2
#define NVOFF_USB_PRODUCTID     (NVOFF_USB_VENDORID+ NVLEN_USB_VENDORID) // ad 32 USBProductID
#define NVLEN_USB_PRODUCTID     2
#define NVOFF_USB_LANGDESC      (NVOFF_USB_PRODUCTID+ NVLEN_USB_PRODUCTID) // ad 34 USBLangDesc
#define NVLEN_USB_LANGDESC      2
#define NVOFF_FORMATTER_SER_NUM (NVOFF_USB_LANGDESC+ NVLEN_USB_LANGDESC) // ad 36 Formatter Serial number and USB Serial Number
#define NVLEN_FORMATTER_SER_NUM 17
#define NVOFF_USB_MFGSTR        (NVOFF_FORMATTER_SER_NUM+NVLEN_FORMATTER_SER_NUM)  // ad 53 USBMfgStr - Manufacturer String Descriptor
#define NVLEN_USB_MFGSTR        25
#define NVOFF_USB_PRODSTR       (NVOFF_USB_MFGSTR+NVLEN_USB_MFGSTR) // ad 78 USBProdStr - Product String Descriptor
#define NVLEN_USB_PRODSTR       34
#define NVOFF_PRINTER_NAMESTR   (NVOFF_USB_PRODSTR+NVLEN_USB_PRODSTR)  // ad 112 Printer Name
#define NVLEN_PRINTER_NAMESTR   34
#define NVOFF_MACADDR           (NVOFF_PRINTER_NAMESTR+NVLEN_PRINTER_NAMESTR)  // ad 146 MAC address
#define NVLEN_MACADDR           6
#define NVOFF_CMD_LANG          (NVOFF_MACADDR+NVLEN_MACADDR)  // ad 152 Printer Command Lang
#define NVLEN_CMD_LANG          4
#define NVOFF_DEFAULT_LANG      (NVOFF_CMD_LANG+NVLEN_CMD_LANG)  // ad 156 Default Language (localization)
#define NVLEN_DEFAULT_LANG      1
#define NVOFF_DEV_CLASS         (NVOFF_DEFAULT_LANG+NVLEN_DEFAULT_LANG)  // ad 157 Device Class
#define NVLEN_DEV_CLASS         10
#define NVOFF_FLAGS             (NVOFF_DEV_CLASS + NVLEN_DEV_CLASS) // ad 167
#define NVLEN_FLAGS             1
#define NVOFF_NAND_TR0          (NVOFF_FLAGS + NVLEN_FLAGS) // ad 168
#define NVLEN_NAND_TRO          4
#define NVOFF_NAND_TR1          (NVOFF_NAND_TR0 + NVLEN_NAND_TRO) // ad 172
#define NVLEN_NAND_TR1          4
#define NVOFF_PLATFORM_ID       (NVOFF_NAND_TR1 + NVLEN_NAND_TR1) // ad 176
#define NVLEN_PLATFORM_ID       2
#define NVOFF_VERSION_ID        (NVOFF_PLATFORM_ID + NVLEN_PLATFORM_ID) // ad 178
#define NVLEN_VERSION_ID        2
#define NVOFF_NAND_PG_SHIFT     (NVOFF_VERSION_ID + NVLEN_VERSION_ID) // ad 180
#define NVLEN_NAND_PG_SHIFT     1
#define NVOFF_NAND_BLPERDEVICE  (NVOFF_NAND_PG_SHIFT + NVLEN_NAND_PG_SHIFT) // ad 181
#define NVLEN_NAND_BLPERDEVICE  3
#define NVOFF_NAND_PGSIZE       (NVOFF_NAND_BLPERDEVICE + NVLEN_NAND_BLPERDEVICE) // ad 184
#define NVLEN_NAND_PGSIZE       2
#define NVOFF_NAND_MISCSIZE     (NVOFF_NAND_PGSIZE + NVLEN_NAND_PGSIZE) // ad 186
#define NVLEN_NAND_MISCSIZE     3       // first 12 bits are spare size, last 12 bits are pages/block
#define NVOFF_NAND_SPAREFIND    (NVOFF_NAND_MISCSIZE + NVLEN_NAND_MISCSIZE) // ad 189
#define NVLEN_NAND_SPAREFIND    7   // defines how to find spares.
#define NVOFF_OVERRIDE          (NVOFF_NAND_SPAREFIND + NVLEN_NAND_SPAREFIND) // ad 196
#define NVLEN_OVERRIDE          52
#define NVOFF_PS_DR0_VALS_LOW   (NVOFF_OVERRIDE + NVLEN_OVERRIDE) // ad 248 Must be even
#define NVLEN_PS_DR0_VALS_LOW   2
#define NVOFF_PS_DR0_VALS_HIGH  (NVOFF_PS_DR0_VALS_LOW + NVLEN_PS_DR0_VALS_LOW)// must be even
#define NVLEN_PS_DR0_VALS_HIGH  2
#define NVOFF_RESERVED1         (NVOFF_PS_DR0_VALS_HIGH + NVLEN_PS_DR0_VALS_HIGH)
#define NVLEN_RESERVED1         1
#define NVOFF_BOOTOPTIONS       (NVOFF_RESERVED1 + NVLEN_RESERVED1)
#define NVLEN_BOOTOPTIONS       1
#define NVOFF_NAND_TBL_LEN      (NVOFF_BOOTOPTIONS + NVLEN_BOOTOPTIONS) /* ad 254 */
#define NVLEN_NAND_TBL_LEN      2
#define NVOFF_MEMCTL_WORDS      (NVOFF_NAND_TBL_LEN + NVLEN_NAND_TBL_LEN) /* ad 256 */
#define NVLEN_MEMCTL_WORDS      120
#define NVOFF_RTC_SETTINGS      (NVOFF_MEMCTL_WORDS + NVLEN_MEMCTL_WORDS) // ad 376 cpu ccr register, 
#define NVLEN_RTC_SETTINGS      16
#define NVOFF_RTC_SET_VALID		(NVOFF_RTC_SETTINGS + NVLEN_RTC_SETTINGS) // ad 392
#define NVLEN_RTC_SET_VALID     1
#define NVOFF_PS_SETTINGS_LOW   (NVOFF_RTC_SET_VALID + NVLEN_RTC_SET_VALID) // add 393
#define NVLEN_PS_SETTINGS_LOW   1
#define NVOFF_PS_SETTINGS_MID   (NVOFF_PS_SETTINGS_LOW + NVLEN_PS_SETTINGS_LOW)
#define NVLEN_PS_SETTINGS_MID    1
#define NVOFF_PS_SETTINGS_HIGH  (NVOFF_PS_SETTINGS_MID + NVLEN_PS_SETTINGS_MID)
#define NVLEN_PS_SETTINGS_HIGH   1
#define NVOFF_USB_PHY_SETTINGS   (NVOFF_PS_SETTINGS_HIGH + NVLEN_PS_SETTINGS_HIGH)
#define NVLEN_USB_PHY_SETTINGS   (7 * 4)        // 7 words of 4 bytes each.
#define NVOFF_OVERRIDE_1        (NVOFF_USB_PHY_SETTINGS + NVLEN_USB_PHY_SETTINGS) // ad 396 must be aligned on 4 byte boundary
#define NVLEN_OVERRIDE_1        NVRAM_STATIC_DATA_SIZE - NVOFF_OVERRIDE_1 

#define NVRAM_TOTAL_LENGTH 512      // round up to 512 for the static section.
//dynamic section

#define NVFLAG_BIT_0 0x1


#ifdef __cplusplus
}
#endif

#endif // _NVRAMTABLE_H

