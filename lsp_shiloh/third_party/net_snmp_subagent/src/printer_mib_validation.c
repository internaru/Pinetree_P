/*
 * ============================================================================
 * Copyright (c) 2012   Marvell International, Ltd. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 * Net-SNMP Sub-Agent
 * Printer MIB Validation Module
 *
 * based on an auto-generated file from Printer-MIB by mib2c
 * using mib2c.array-user.conf (15997 2007-03-25 22:28:35Z dts12)
 */

#include "printer_mib_validation.h"

int prtCapacityUnit_values[] = {
    1,  /* other */
    2,  /* unknown */
    3,  /* tenThousandthsOfInches */
    4,  /* micrometers */
    8,  /* sheets */
    16, /* feet */
    17, /* meters */
    18, /* items */
    19, /* percent */
    0   /* -- end of list -- */
};

int IANACharset_values[] = {
    1, /* other */
    2, /* unknown */
    3, /* csASCII */
    4, /* csISOLatin1 */
    5, /* csISOLatin2 */
    6, /* csISOLatin3 */
    7, /* csISOLatin4 */
    8, /* csISOLatinCyrillic */
    9, /* csISOLatinArabic */
    10, /* csISOLatinGreek */
    11, /* csISOLatinHebrew */
    12, /* csISOLatin5 */
    13, /* csISOLatin6 */
    14, /* csISOTextComm */
    15, /* csHalfWidthKatakana */
    16, /* csJISEncoding */
    17, /* csShiftJIS */
    18, /* csEUCPkdFmtJapanese */
    19, /* csEUCFixWidJapanese */
    20, /* csISO4UnitedKingdom */
    21, /* csISO11SwedishForNames */
    22, /* csISO15Italian */
    23, /* csISO17Spanish */
    24, /* csISO21German */
    25, /* csISO60DanishNorwegian */
    26, /* csISO69French */
    27, /* csISO10646UTF1 */
    28, /* csISO646basic1983 */
    29, /* csINVARIANT */
    30, /* csISO2IntlRefVersion */
    31, /* csNATSSEFI */
    32, /* csNATSSEFIADD */
    33, /* csNATSDANO */
    34, /* csNATSDANOADD */
    35, /* csISO10Swedish */
    36, /* csKSC56011987 */
    37, /* csISO2022KR */
    38, /* csEUCKR */
    39, /* csISO2022JP */
    40, /* csISO2022JP2 */
    41, /* csISO13JISC6220jp */
    42, /* csISO14JISC6220ro */
    43, /* csISO16Portuguese */
    44, /* csISO18Greek7Old */
    45, /* csISO19LatinGreek */
    46, /* csISO25French */
    47, /* csISO27LatinGreek1 */
    48, /* csISO5427Cyrillic */
    49, /* csISO42JISC62261978 */
    50, /* csISO47BSViewdata */
    51, /* csISO49INIS */
    52, /* csISO50INIS8 */
    53, /* csISO51INISCyrillic */
    54, /* csISO54271981 */
    55, /* csISO5428Greek */
    56, /* csISO57GB1988 */
    57, /* csISO58GB231280 */
    58, /* csISO61Norwegian2 */
    59, /* csISO70VideotexSupp1 */
    60, /* csISO84Portuguese2 */
    61, /* csISO85Spanish2 */
    62, /* csISO86Hungarian */
    63, /* csISO87JISX0208 */
    64, /* csISO88Greek7 */
    65, /* csISO89ASMO449 */
    66, /* csISO90 */
    67, /* csISO91JISC62291984a */
    68, /* csISO92JISC62991984b */
    69, /* csISO93JIS62291984badd */
    70, /* csISO94JIS62291984hand */
    71, /* csISO95JIS62291984handadd */
    72, /* csISO96JISC62291984kana */
    73, /* csISO2033 */
    74, /* csISO99NAPLPS */
    75, /* csISO102T617bit */
    76, /* csISO103T618bit */
    77, /* csISO111ECMACyrillic */
    78, /* csa71 */
    79, /* csa72 */
    80, /* csISO123CSAZ24341985gr */
    81, /* csISO88596E */
    82, /* csISO88596I */
    83, /* csISO128T101G2 */
    84, /* csISO88598E */
    85, /* csISO88598I */
    86, /* csISO139CSN369103 */
    87, /* csISO141JUSIB1002 */
    88, /* csISO143IECP271 */
    89, /* csISO146Serbian */
    90, /* csISO147Macedonian */
    91, /* csISO150 */
    92, /* csISO151Cuba */
    93, /* csISO6937Add */
    94, /* csISO153GOST1976874 */
    95, /* csISO8859Supp */
    96, /* csISO10367Box */
    97, /* csISO158Lap */
    98, /* csISO159JISX02121990 */
    99, /* csISO646Danish */
    100, /* csUSDK */
    101, /* csDKUS */
    102, /* csKSC5636 */
    103, /* csUnicode11UTF7 */
    104, /* csISO2022CN */
    105, /* csISO2022CNEXT */
    106, /* csUTF8 */
    109, /* csISO885913 */
    110, /* csISO885914 */
    111, /* csISO885915 */
    112, /* csISO885916 */
    113, /* csGBK */
    114, /* csGB18030 */
    115, /* csOSDEBCDICDF0415 */
    116, /* csOSDEBCDICDF03IRV */
    117, /* csOSDEBCDICDF041 */
    118, /* csISO115481 */
    119, /* csKZ1048 */
    1000, /* csUnicode */
    1001, /* csUCS4 */
    1002, /* csUnicodeASCII */
    1003, /* csUnicodeLatin1 */
    1004, /* csUnicodeJapanese */
    1005, /* csUnicodeIBM1261 */
    1006, /* csUnicodeIBM1268 */
    1007, /* csUnicodeIBM1276 */
    1008, /* csUnicodeIBM1264 */
    1009, /* csUnicodeIBM1265 */
    1010, /* csUnicode11 */
    1011, /* csSCSU */
    1012, /* csUTF7 */
    1013, /* csUTF16BE */
    1014, /* csUTF16LE */
    1015, /* csUTF16 */
    1016, /* csCESU8 */
    1017, /* csUTF32 */
    1018, /* csUTF32BE */
    1019, /* csUTF32LE */
    1020, /* csBOCU1 */
    2000, /* csWindows30Latin1 */
    2001, /* csWindows31Latin1 */
    2002, /* csWindows31Latin2 */
    2003, /* csWindows31Latin5 */
    2004, /* csHPRoman8 */
    2005, /* csAdobeStandardEncoding */
    2006, /* csVenturaUS */
    2007, /* csVenturaInternational */
    2008, /* csDECMCS */
    2009, /* csPC850Multilingual */
    2010, /* csPCp852 */
    2011, /* csPC8CodePage437 */
    2012, /* csPC8DanishNorwegian */
    2013, /* csPC862LatinHebrew */
    2014, /* csPC8Turkish */
    2015, /* csIBMSymbols */
    2016, /* csIBMThai */
    2017, /* csHPLegal */
    2018, /* csHPPiFont */
    2019, /* csHPMath8 */
    2020, /* csHPPSMath */
    2021, /* csHPDesktop */
    2022, /* csVenturaMath */
    2023, /* csMicrosoftPublishing */
    2024, /* csWindows31J */
    2025, /* csGB2312 */
    2026, /* csBig5 */
    2027, /* csMacintosh */
    2028, /* csIBM037 */
    2029, /* csIBM038 */
    2030, /* csIBM273 */
    2031, /* csIBM274 */
    2032, /* csIBM275 */
    2033, /* csIBM277 */
    2034, /* csIBM278 */
    2035, /* csIBM280 */
    2036, /* csIBM281 */
    2037, /* csIBM284 */
    2038, /* csIBM285 */
    2039, /* csIBM290 */
    2040, /* csIBM297 */
    2041, /* csIBM420 */
    2042, /* csIBM423 */
    2043, /* csIBM424 */
    2044, /* csIBM500 */
    2045, /* csIBM851 */
    2046, /* csIBM855 */
    2047, /* csIBM857 */
    2048, /* csIBM860 */
    2049, /* csIBM861 */
    2050, /* csIBM863 */
    2051, /* csIBM864 */
    2052, /* csIBM865 */
    2053, /* csIBM868 */
    2054, /* csIBM869 */
    2055, /* csIBM870 */
    2056, /* csIBM871 */
    2057, /* csIBM880 */
    2058, /* csIBM891 */
    2059, /* csIBM903 */
    2060, /* csIBBM904 */
    2061, /* csIBM905 */
    2062, /* csIBM918 */
    2063, /* csIBM1026 */
    2064, /* csIBMEBCDICATDE */
    2065, /* csEBCDICATDEA */
    2066, /* csEBCDICCAFR */
    2067, /* csEBCDICDKNO */
    2068, /* csEBCDICDKNOA */
    2069, /* csEBCDICFISE */
    2070, /* csEBCDICFISEA */
    2071, /* csEBCDICFR */
    2072, /* csEBCDICIT */
    2073, /* csEBCDICPT */
    2074, /* csEBCDICES */
    2075, /* csEBCDICESA */
    2076, /* csEBCDICESS */
    2077, /* csEBCDICUK */
    2078, /* csEBCDICUS */
    2079, /* csUnknown8BiT */
    2080, /* csMnemonic */
    2081, /* csMnem */
    2082, /* csVISCII */
    2083, /* csVIQR */
    2084, /* csKOI8R */
    2085, /* csHZGB2312 */
    2086, /* csIBM866 */
    2087, /* csPC775Baltic */
    2088, /* csKOI8U */
    2089, /* csIBM00858 */
    2090, /* csIBM00924 */
    2091, /* csIBM01140 */
    2092, /* csIBM01141 */
    2093, /* csIBM01142 */
    2094, /* csIBM01143 */
    2095, /* csIBM01144 */
    2096, /* csIBM01145 */
    2097, /* csIBM01146 */
    2098, /* csIBM01147 */
    2099, /* csIBM01148 */
    2100, /* csIBM01149 */
    2101, /* csBig5HKSCS */
    2102, /* csIBM1047 */
    2103, /* csPTCP154 */
    2104, /* csAmiga1251 */
    2105, /* csKOI7switched */
    2106, /* csBRF */
    2107, /* csTSCII */
    2108, /* csCP51932 */
    2109, /* cswindows874 */
    2250, /* cswindows1250 */
    2251, /* cswindows1251 */
    2252, /* cswindows1252 */
    2253, /* cswindows1253 */
    2254, /* cswindows1254 */
    2255, /* cswindows1255 */
    2256, /* cswindows1256 */
    2257, /* cswindows1257 */
    2258, /* cswindows1258 */
    2259, /* csTIS620 */
    2260, /* cs50220 */
    3000, /* reserved */
    0, /* end of list */
};

int prtMarkerCounterUnit_values[] = {
    3, /* tenThousandthsOfInches */
    4, /* micrometers */
    5, /* characters */
    6, /* lines */
    7, /* impressions */
    8, /* sheets */
    9, /* dotRow */
    11, /* hours */
    16, /* feet */
    17, /* meters */
    0, /* end of list */
};

int prtMarkerSuppliesClass_values[] = {
    1, /* other */
    3, /* supplyThatIsConsumed */
    4, /* receptacleThatIsFilled */
    0, /* end of list */
};

int prtMarkerSuppliesSupplyUnit_values[] = {
    1, /* other */
    2, /* unknown */
    3, /* tenThousandthsOfInches */
    4, /* micrometers */
    7, /* impressions */
    8, /* sheets */
    11, /* hours */
    12, /* thousandthsOfOunces */
    13, /* tenthsOfGrams */
    14, /* hundrethsOfFluidOunces */
    15, /* tenthsOfMilliliters */
    16, /* feet */
    17, /* meters */
    18, /* items */
    19, /* percent */
    0, /* end of list */
};

int prtMarkerColorantRole_values[] = {
    1, /* other */
    3, /* process */
    4, /* spot */
    0, /* end of list */
};

int prtMediaPathMaxSpeedPrintUnit_values[] = {
    3, /* tenThousandthsOfInchesPerHour */
    4, /* micrometersPerHour */
    5, /* charactersPerHour */
    6, /* linesPerHour */
    7, /* impressionsPerHour */
    8, /* sheetsPerHour */
    9, /* dotRowPerHour */
    16, /* feetPerHour */
    17, /* metersPerHour */
    0, /* end of list */
};

int prtPrintOrientation_values[] = {
    1, /* other */
    3, /* portrait */
    4, /* landscape */
    0, /* end of list */
};

int isIntPartOfList(int validList[], int value)
{
    int i = 0;
    while (validList[i]) {
        if (value == validList[i])
            return 1;
        i++;
    }
    return 0;
}
