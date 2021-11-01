/*
 * =========================================================
 * (c) Copyright 2009-2010 Marvell International Ltd. 
 *
 *               Marvell Confidential
 * ==========================================================
 */
/**
 * \file paper_size_config.h
 *
 * \brief Paper sizes
 *
 */

#ifndef PAPERSIZE_CONFIG_H
#define PAPERSIZE_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// ********* PAPER SIZES, TYPES, and BIN DEFINES ****************
// Based off of defines in wingdi.h from the win32 SDK
// These are the values that the driver will send down.
typedef enum
{
   
    MEDIASIZE_ANY =                          0,
    MEDIASIZE_LETTER =                       1, /*!< Letter                        8.5 x 11 in    - 215.9 x 279.4*/
    MEDIASIZE_LETTERSMALL =                  2, /*!< Letter Small                  8.5 x 11 in    - 215.9 x 279.4*/
    MEDIASIZE_TABLOID =                      3, /*!< Tabloid                        11 x 17 in    - 279.4 x 431.8*/
    MEDIASIZE_LEDGER =                       4, /*!< Ledger                         17 x 11 in    - 431.8 x 279.4*/
    MEDIASIZE_LEGAL =                        5, /*!< Legal                         8.5 x 14 in    - 215.9 x 355.6*/
    MEDIASIZE_STATEMENT =                    6, /*!< Statement                     5.5 x  8.5 in  - 139.7 x 215.9*/
    MEDIASIZE_EXECUTIVE =                    7, /*!< Executive                    7.25 x 10.5 in  - 184.15 x 266.7 */
    MEDIASIZE_A3 =                           8, /*!< A3                                           - 297 x 420 mm */
    MEDIASIZE_A4 =                           9, /*!< A4                           8.25 x 11.7     - 210 x 297 mm */
    MEDIASIZE_A4SMALL =                     10, /*!< A4 Small                                     - 210 x 297 mm */
    MEDIASIZE_A5 =                          11, /*!< A5                                           - 148 x 210 mm */
    MEDIASIZE_B4 =                          12, /*!< B4 (JIS)                                     - 250 x 354 mm */
    MEDIASIZE_B5 =                          13, /*!< B5 (JIS)                                     - 182 x 257 mm */
    MEDIASIZE_FOLIO =                       14, /*!< Folio                         8.5 x 13 in    - 215.9 x 330.2 */
    MEDIASIZE_QUARTO =                      15, /*!< Quarto                                       - 215 x 275 mm  */
    MEDIASIZE_10X14 =                       16, /*!<                                10 x 14 in    - 254 x 355.6          */
    MEDIASIZE_11X17 =                       17, /*!<                                11 x 17 in    - 279.4 x 431.8 */
    MEDIASIZE_NOTE =                        18, /*!< Note                          8.5 x 11 in    - 215.9 x 279.4*/
    MEDIASIZE_ENV_9 =                       19, /*!< Envelope #9                 3 7/8 x 8 7/8    -              */
    MEDIASIZE_ENV_10 =                      20, /*!< Envelope #10                4 1/8 x 9 1/2    - 104.8 x 241.3            */
    MEDIASIZE_ENV_11 =                      21, /*!< Envelope #11                4 1/2 x 10 3/8   -              */
    MEDIASIZE_ENV_12 =                      22, /*!< Envelope #12                4 3/4 x 11       -       x 279.4*/
    MEDIASIZE_ENV_14 =                      23, /*!< Envelope #14                    5 x 11 1/2   -              */
    MEDIASIZE_CSHEET =                      24, /*!< C size sheet                                 -              */
    MEDIASIZE_DSHEET =                      25, /*!< D size sheet                                 -              */
    MEDIASIZE_ESHEET =                      26, /*!< E size sheet                                 -              */
    MEDIASIZE_ENV_DL =                      27, /*!< Envelope DL                 4.3x8.7          - 110 x 220 mm */
    MEDIASIZE_ENV_C5 =                      28, /*!< Envelope C5                 6.38 x 9         - 162 x 229 mm */
    MEDIASIZE_ENV_C3 =                      29, /*!< Envelope C3                                  - 324 x 458 mm */
    MEDIASIZE_ENV_C4 =                      30, /*!< Envelope C4                                  - 229 x 324 mm */
    MEDIASIZE_ENV_C6 =                      31, /*!< Envelope C6                                  - 114 x 162 mm */
    MEDIASIZE_ENV_C65 =                     32, /*!< Envelope C65                                 - 114 x 229 mm */
    MEDIASIZE_ENV_B4 =                      33, /*!< Envelope B4                                  - 250 x 353 mm */
    MEDIASIZE_ENV_B5 =                      34, /*!< Envelope B5                                  - 176 x 250 mm */
    MEDIASIZE_ENV_B6 =                      35, /*!< Envelope B6                                  - 176 x 125 mm */
    MEDIASIZE_ENV_ITALY =                   36, /*!< Envelope                                     - 110 x 230 mm */
    MEDIASIZE_ENV_MONARCH =                 37, /*!< Envelope Monarch            3.875 x 7.5 in   - 98.43 x 190.5             */
    MEDIASIZE_ENV_PERSONAL =                38, /*!< 6 3/4 Envelope              3 5/8 x 6 1/2 in - 92.075 x 165.10 */
    MEDIASIZE_FANFOLD_US =                  39, /*!< US Std Fanfold             14 7/8 x 11 in    -     x 279.4  */
    MEDIASIZE_FANFOLD_STD_GERMAN =          40, /*!< German Std Fanfold          8 1/2 x 12 in    - 215.9 x   */
    MEDIASIZE_FANFOLD_LGL_GERMAN =          41, /*!< German Legal Fanfold        8 1/2 x 13 in    - 215.9 x   */
    MEDIASIZE_ISO_B4 =                      42, /*!< B4 (ISO)                                     - 250 x 353 mm */
    MEDIASIZE_JAPANESE_POSTCARD =           43, /*!< Japanese Postcard                            - 100 x 148 mm */
    MEDIASIZE_9X11 =                        44, /*!<                                 9 x 11 in    -     x 279.4  */
    MEDIASIZE_10X11 =                       45, /*!<                                10 x 11 in    -     x 279.4         */
    MEDIASIZE_15X11 =                       46, /*!<                                15 x 11 in    -     x 279.4         */
    MEDIASIZE_ENV_INVITE =                  47, /*!< Envelope Invite                              - 220 x 220 mm */
    MEDIASIZE_RESERVED_48 =                 48, /*!< RESERVED--DO NOT USE                         -              */
    MEDIASIZE_RESERVED_49 =                 49, /*!< RESERVED--DO NOT USE                         -              */
    MEDIASIZE_LETTER_EXTRA =                50, /*!< Letter Extra                9 1/2 x 12 in    -              */
    MEDIASIZE_LEGAL_EXTRA =                 51, /*!< Legal Extra                 9 1/2 x 15 in    -              */
    MEDIASIZE_TABLOID_EXTRA =               52, /*!< Tabloid Extra               11.69 x 18 in    -              */
    MEDIASIZE_A4_EXTRA =                    53, /*!< A4 Extra                     9.27 x 12.69 in -              */
    MEDIASIZE_LETTER_TRANSVERSE =           54, /*!< Letter Transverse           8 1/2 x 11 in    -     x 279.4  */
    MEDIASIZE_A4_TRANSVERSE =               55, /*!< A4 Transverse                                - 210 x 297 mm */
    MEDIASIZE_LETTER_EXTRA_TRANSVERSE =     56, /*!< Letter Extra Transverse     9 1/2 x 12 in    -              */
    MEDIASIZE_A_PLUS =                      57, /*!< SuperA/SuperA/A4                             - 227 x 356 mm */
    MEDIASIZE_B_PLUS =                      58, /*!< SuperB/SuperB/A3                             - 305 x 487 mm */
    MEDIASIZE_LETTER_PLUS =                 59, /*!< Letter Plus                   8.5 x 12.69 in -              */
    MEDIASIZE_A4_PLUS =                     60, /*!< A4 Plus                                      - 210 x 330 mm */
    MEDIASIZE_A5_TRANSVERSE =               61, /*!< A5 Transverse                                - 148 x 210 mm */
    MEDIASIZE_B5_TRANSVERSE =               62, /*!< B5 (JIS) Transverse                          - 182 x 257 mm */
    MEDIASIZE_A3_EXTRA =                    63, /*!< A3 Extra                                     - 322 x 445 mm */
    MEDIASIZE_A5_EXTRA =                    64, /*!< A5 Extra                                     - 174 x 235 mm */
    //c.a. MEDIASIZE_B5_EXTRA =                    65, /*!< B5 (ISO) Extra                               - 201 x 276 mm */
    MEDIASIZE_A2 =                          66, /*!< A2                                           - 420 x 594 mm */
    MEDIASIZE_A3_TRANSVERSE =               67, /*!< A3 Transverse                                - 297 x 420 mm */
    MEDIASIZE_A3_EXTRA_TRANSVERSE =         68, /*!< A3 Extra Transverse                          - 322 x 445 mm */
    MEDIASIZE_DBL_JAPANESE_POSTCARD =       69, /*!< Japanese Double Postcard                      - 200 x 148 mm */
    MEDIASIZE_A6 =                          70, /*!< A6                                           - 105 x 148 mm */
    MEDIASIZE_JENV_KAKU2 =                  71, /*!< Japanese Envelope Kaku #2                    -              */
    MEDIASIZE_JENV_KAKU3 =                  72, /*!< Japanese Envelope Kaku #3                    -              */
    MEDIASIZE_JENV_CHOU3 =                  73, /*!< Japanese Envelope Chou #3                    -              */
    MEDIASIZE_JENV_CHOU4 =                  74, /*!< Japanese Envelope Chou #4                    -              */
    MEDIASIZE_LETTER_ROTATED =              75, /*!< Letter Rotated                 11 x 8 1/2 in             */
    MEDIASIZE_A3_ROTATED =                  76, /*!< A3 Rotated                                   - 420 x 297 mm */
    MEDIASIZE_A4_ROTATED =                  77, /*!< A4 Rotated                                   - 297 x 210 mm */
    MEDIASIZE_A5_ROTATED =                  78, /*!< A5 Rotated                                   - 210 x 148 mm */
    MEDIASIZE_B4_JIS_ROTATED =              79, /*!< B4 (JIS) Rotated                             - 364 x 257 mm */
    MEDIASIZE_B5_JIS_ROTATED =              80, /*!< B5 (JIS) Rotated                             - 257 x 182 mm */
    MEDIASIZE_JAPANESE_POSTCARD_ROTATED =   81, /*!< Japanese Postcard Rotated                     - 148 x 100 mm */
    MEDIASIZE_DBL_JAPANESE_POSTCARD_ROTATED=82, /*!< Double Japanese Postcard Rotated              - 148 x 200 mm */
    MEDIASIZE_A6_ROTATED =                  83, /*!< A6 Rotated                                   - 148 x 105 mm */
    MEDIASIZE_JENV_KAKU2_ROTATED =          84, /*!< Japanese Envelope Kaku #2 Rotated            -              */
    MEDIASIZE_JENV_KAKU3_ROTATED =          85, /*!< Japanese Envelope Kaku #3 Rotated            -              */
    MEDIASIZE_JENV_CHOU3_ROTATED =          86, /*!< Japanese Envelope Chou #3 Rotated            -              */
    MEDIASIZE_JENV_CHOU4_ROTATED =          87, /*!< Japanese Envelope Chou #4 Rotated            -              */
    MEDIASIZE_B6_JIS =                      88, /*!< B6 (JIS)                                     - 128 x 182 mm */
    MEDIASIZE_B6_JIS_ROTATED =              89, /*!< B6 (JIS) Rotated                             - 182 x 128 mm */
    MEDIASIZE_12X11 =                       90, /*!<                                12 x 11 in    -     x 279.4         */
    MEDIASIZE_JENV_YOU4 =                   91, /*!< Japanese Envelope You #4                     -              */
    MEDIASIZE_JENV_YOU4_ROTATED =           92, /*!< Japanese Envelope You #4 Rotated             -              */
    MEDIASIZE_P16K =                        93, /*!< PRC 16K                                      - 146 x 215 mm */
    MEDIASIZE_P32K =                        94, /*!< PRC 32K                                      -  97 x 151 mm */
    MEDIASIZE_P32KBIG =                     95, /*!< PRC 32K(Big)                                 -  97 x 151 mm */
    MEDIASIZE_PENV_1 =                      96, /*!< PRC Envelope #1                              - 102 x 165 mm */
    MEDIASIZE_PENV_2 =                      97, /*!< PRC Envelope #2                              - 102 x 176 mm */
    MEDIASIZE_PENV_3 =                      98, /*!< PRC Envelope #3                              - 125 x 176 mm */
    MEDIASIZE_PENV_4 =                      99, /*!< PRC Envelope #4                              - 110 x 208 mm */
    MEDIASIZE_PENV_5 =                      100,/*!< PRC Envelope #5                              - 110 x 220 mm */
    MEDIASIZE_PENV_6 =                      101,/*!< PRC Envelope #6                              - 120 x 230 mm */
    MEDIASIZE_PENV_7 =                      102,/*!< PRC Envelope #7                              - 160 x 230 mm */
    MEDIASIZE_PENV_8 =                      103,/*!< PRC Envelope #8                              - 120 x 309 mm */
    MEDIASIZE_PENV_9 =                      104,/*!< PRC Envelope #9                              - 229 x 324 mm */
    MEDIASIZE_PENV_10 =                     105,/*!< PRC Envelope #10                             - 324 x 458 mm */
    MEDIASIZE_P16K_ROTATED =                106,/*!< PRC 16K Rotated                              -              */
    MEDIASIZE_P32K_ROTATED =                107,/*!< PRC 32K Rotated                              -              */
    MEDIASIZE_P32KBIG_ROTATED =             108,/*!< PRC 32K(Big) Rotated                         -              */
    MEDIASIZE_PENV_1_ROTATED =              109,/*!< PRC Envelope #1 Rotated                      - 165 x 102 mm */
    MEDIASIZE_PENV_2_ROTATED =              110,/*!< PRC Envelope #2 Rotated                      - 176 x 102 mm */
    MEDIASIZE_PENV_3_ROTATED =              111,/*!< PRC Envelope #3 Rotated                      - 176 x 125 mm */
    MEDIASIZE_PENV_4_ROTATED =              112,/*!< PRC Envelope #4 Rotated                      - 208 x 110 mm */
    MEDIASIZE_PENV_5_ROTATED =              113,/*!< PRC Envelope #5 Rotated                      - 220 x 110 mm */
    MEDIASIZE_PENV_6_ROTATED =              114,/*!< PRC Envelope #6 Rotated                      - 230 x 120 mm */
    MEDIASIZE_PENV_7_ROTATED =              115,/*!< PRC Envelope #7 Rotated                      - 230 x 160 mm */
    MEDIASIZE_PENV_8_ROTATED =              116,/*!< PRC Envelope #8 Rotated                      - 309 x 120 mm */
    MEDIASIZE_PENV_9_ROTATED =              117,/*!< PRC Envelope #9 Rotated                      - 324 x 229 mm */
    MEDIASIZE_PENV_10_ROTATED =             118,/*!< PRC Envelope #10 Rotated                     - 458 x 324 mm */
//  MEDIASIZE_B6_ISO =                      119,/*!< PRC Envelope #10 Rotated                     - 458 x 324 mm */

    /* paper sizes we added */                                                                  
    MEDIASIZE_USER_DEFINED =                256,                                                
    MEDIASIZE_CUSTOM_USER  =                MEDIASIZE_USER_DEFINED,
    MEDIASIZE_CUSTOM_16K   =                257, /*!< 16K 197x273 mm */
    MEDIASIZE_CUSTOM_85x13 =                258,
    MEDIASIZE_CUSTOM_B5_ISO =               259,
    MEDIASIZE_CUSTOM_POSTCARD_JIS =         260,
    MEDIASIZE_CUSTOM_DBL_POSTCARD_JIS =     261,
    MEDIASIZE_CUSTOM_EXECUTIVE_JIS  =       262,
    MEDIASIZE_CUSTOM_16K_184_260 =          263,
    MEDIASIZE_CUSTOM_16K_195_270 =          264,
    MEDIASIZE_CUSTOM_8K =                   265,
    MEDIASIZE_B6_ISO =                      266,/*!< PRC Envelope #10 Rotated                     - 458 x 324 mm */
    MEDIASIZE_ANY_TEMP =                    267, //Added an alias for MEDIASIZE_ANY, needs to be removed                    
    MEDIASIZE_CUSTOM_4x6 =                  268,
    MEDIASIZE_CUSTOM_5x8 =                  269,
    MEDIASIZE_CUSTOM_10x15cm =              270,
    MEDIASIZE_CUSTOM_8K_270_390 =           298,
    MEDIASIZE_CUSTOM_8K_273_394 =           299,
    MEDIASIZE_B5_ISO =                      357,
    MEDIASIZE_OFICIO_216x340 =              358,
    MEDIASIZE_CUSTOM_8x10 =                 359,
    MEDIASIZE_CUSTOM_8x105 =                360,
    MEDIASIZE_CUSTOM_825x14 =               361,
    
    MEDIASIZE_UNKNOWN =                     400,
    MEDIASIZE_END_USER_DEFINED,                                                                 
    MEDIASIZE_CUSTOM_USER2 = 512,
    //not real paper sizes, used for calibration                                                
    MEDIASIZE_EXTRA =                        MEDIASIZE_END_USER_DEFINED + 1,                    
    MEDIASIZE_CAL_WIDTH_CPR,
    MEDIASIZE_CAL_WIDTH_DHALF,

    MEDIASIZE_BAD = 0XFFFFFFFF,
} mediasize_t;

#ifdef __cplusplus
}
#endif

#endif
