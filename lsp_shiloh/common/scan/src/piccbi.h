/*
 * ============================================================================
 * Copyright (c) 2012 Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 */

/****************************************************************************** 
 * (c) Copyright 2003 Agilent Technologies, Inc.
 *
 *
===================================================================
 * AGILENT TECHNOLOGIES, INC. / ASIC Products Division (APD)
 *
 * AGILENT INTELLECTUAL PROPERTY 
 * Agilent Intellectual Property (IP) provides significant value
 * to customer applications. Agilent HSD desires to attract potential 
 * customers by enabling them to create differentiated end-products 
 * by using its value-adding IP.
 * 
===================================================================
 * NOTICE TO USERS - LICENSE REQUIRED - AGILENT TECHNOLOGIES, INC. 
 * PROPRIETARY AND CONFIDENTIAL
 *
 * The information contained herein embodies intellectual property that 
 * belongs to Agilent Technologies, Inc., and in some cases to other 
 * companies as well. NO LICENSE IS GRANTED by the mere distribution 
 * or possession of this information.
 *
 * USAGE RESTRICTIONS. The information contained herein may not be used unless
 * a written license to use has been granted by Agilent HSD.  Possession of
 * this information does not grant permission to the user to decompile,
 * disassemble, redistribute, or reverse engineer any portion of the code,
 * including derivative works. In the event that a license is granted to do so,
 * a license fee or royalty may be charged for such a license.
 *
 *****************************************************************************/

#ifndef PICCBI_H
#define PICCBI_H

/* Both the Scan block (scanhw.h) and Pic block (pichw.h) use these values.
 * Put into one header file to avoid duplicate information.
 *
 *
 *   Common Block Interface for PIC
 *
 *
 * Port List
 *
 *    * Data (16 bit bus)
 *    * Exposure (1 bit, only implemented in some blocks)
 *         o 0 - Pixel Data
 *               + Color and Datatype are defined below
 *         o 1 - Exposure Timing Data
 *               + Color is set to 000 and Datatype is set to 00
 *   * Color (3 bits)
 *         o 000 - Color0 (Even or Non-Staggered)
 *         o 001 - Color1 (Even or Non-Staggered)
 *         o 010 - Color2 (Even or Non-Staggered)
 *         o 011 - Monochrome (Even or Non-Staggered)
 *         o 100 - Color0 Odd
 *         o 101 - Color1 Odd
 *         o 110 - Color2 Odd
 *         o 111 - Monochrome Odd
 *   * Datatype (2 bits)
 *         o 00 - Normal Data Pixel
 *         o 01 - First Data Pixel in Line
 *         o 10 - Last Data Pixel in Line
 *         o 11 - First Data Pixel in Last Line
 *   * Valid
 *   * Ready
 * 
 */

/* Color (3 bits) */
#define PIC_CBI_CEVEN_0    0   /* Color 0 (even or non-staggered) */
#define PIC_CBI_CEVEN_1    1   /* Color 1 (even or non-staggered) */
#define PIC_CBI_CEVEN_2    2   /* Color 2 (even or non-staggered) */
#define PIC_CBI_MEVEN      3   /* Monochrome (even or non-staggereed) */
#define PIC_CBI_CODD_0     4   /* Color 0 (odd) */
#define PIC_CBI_CODD_1     5   /* Color 1 (odd) */
#define PIC_CBI_CODD_2     6   /* Color 2 (odd) */
#define PIC_CBI_MODD       7   /* Monochrome (odd) */

/* Datatype (2 bits) */
#define PIC_CBI_NORMP      0   /* normal data pixel */
#define PIC_CBI_FIRP       1   /* first data pixel in line */
#define PIC_CBI_LASTP      2   /* last data pixel in line */
/* davep 19-Feb-2013 ; update to EOI (End of Image); the FIRPLL gone */
#define PIC_CBI_EOI        3   /* EOI last pixel in the image */
//#define PIC_CBI_FIRPLL     3   /* first data pixel in last line */

#endif /* PICCBI_H */

