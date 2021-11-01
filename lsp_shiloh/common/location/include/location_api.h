/* 
 *
 * ============================================================================
 * Copyright (c) 2006-2010  Marvell Semiconductor, Inc. All Rights Reserved
 *
 *                         Marvell Confidential
 * ============================================================================
 *
 **/
/**
 * \file location_api.h
 *
 * \brief Country Code defines.
 *
 **/



#ifndef LOCATION_API_H
#define LOCATION_API_H

#ifdef __cplusplus
extern "C" {
#endif

/** These are order dependant as they are used as indices into the 
 * CountryDefaults array. */
typedef enum
{
    ARGENTINA = 1,
    AUSTRALIA,
    AUSTRIA,
    BELARUS,
    BELGIUM,
    BOLIVIA,
    BRAZIL,
    BRUNEI,
    BULGARIA,
    CAMBODIA,
    CANADA,
    CHAD,
    CHILE,
    CHINA,
    COLUMBIA,
    COSTA_RICA,
    CROATIA,
    CUBA,
    CYPRUS,
    CZECH_REPUBLIC,
    DENMARK,
    DOMINICAN_REPUBLIC,
    ECUADOR,
    EGYPT,
    ESTONIA,
    FIJI,
    FINLAND,
    FRANCE,
    GERMANY,
    GREECE,
    GUATEMALA,
    HONDURAS,
    HONG_KONG,
    HUNGARY,
    ICELAND,
    INDIA,
    INDONESIA,
    IRAN,
    IRAQ,
    IRELAND,
    ISRAEL,
    ITALY,
    JAPAN,
    JORDAN,
    KAZAKHSTAN,
    KOREA,
    KUWAIT,
    LATVIA,
    LEBANON,
    LIECHTENSTEIN,
    LITHUANIA,
    LUXEMBURG,
    MACEDONIA,
    MALAYSIA,
    MALTA,
    MEXICO,
    MONOCO,
    MONGOLIA,
    MOROCCO,
    NETHERLANDS,
    NEW_ZEALAND,
    NORTH_AFRICA,
    NORWAY,
    OMAN,
    PAKISTAN,
    PANAMA,
    PARAGUAY,
    PERU,
    PHILIPPINES,
    POLAND,
    PORTUGAL,
    ROMANIA,
    RUSSIA,
    SAMOA,
    SAUDI_ARABIA,
    SINGAPORE,
    SLOVAK_REP,
    SLOVENIA,
    SOUTH_AFRICA,
    SOUTH_KOREA,
    SPAIN,
    SRI_LANKA,
    SUDAN,
    SWEDEN,
    SWITZERLAND,
    SYRIA,
    TAIWAN,
    THAILAND,
    TUNISIA,
    TURKEY,
    UAE, // United Arab Emirates
    UK,
    UKRAINE,
    USA,
    URUGUAY,
    VENEZUELA,
    VIETNAM,
    YEMEN,
    END_COUNTRY,
    NO_COUNTRY = 0xFF
} e_CountryCode;

#ifdef __cplusplus
}
#endif

#endif /*LOCATIONS_H_*/
