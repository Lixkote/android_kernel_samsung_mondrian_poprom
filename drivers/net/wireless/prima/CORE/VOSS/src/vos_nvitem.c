/*
 * Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/*============================================================================
  FILE:         vos_nvitem.c
  OVERVIEW:     This source file contains definitions for vOS NV Item APIs
  DEPENDENCIES: NV, remote API client, WinCE REX
                Copyright (c) 2008 QUALCOMM Incorporated.
                All Rights Reserved.
                Qualcomm Confidential and Proprietary
============================================================================*/
/*============================================================================
  EDIT HISTORY FOR MODULE
============================================================================*/
// the following is used to disable warning for having too many labels in
// the 'nv_items_enum_type'

/*----------------------------------------------------------------------------
 * Include Files
 * -------------------------------------------------------------------------*/
#include "vos_types.h"
#include "aniGlobal.h"
#include "vos_nvitem.h"
#include "vos_trace.h"
#include "vos_api.h"
#include "wlan_hdd_misc.h"
#include "vos_sched.h"
#include "wlan_nv_parser.h"
#include "wlan_hdd_main.h"
#include <net/cfg80211.h>
static char crda_alpha2[2] = {0, 0}; /* country code from initial crda req */
static char run_time_alpha2[2] = {0, 0}; /* country code from none-default country req */
static v_BOOL_t crda_regulatory_entry_valid = VOS_FALSE;
static v_BOOL_t crda_regulatory_run_time_entry_valid = VOS_FALSE;

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 * -------------------------------------------------------------------------*/
#define VALIDITY_BITMAP_NV_ID    NV_WLAN_VALIDITY_BITMAP_I
#define VALIDITY_BITMAP_SIZE     32
#define MAX_COUNTRY_COUNT        300
//To be removed when NV support is fully functional
#define VOS_HARD_CODED_MAC    {0, 0x0a, 0xf5, 4, 5, 6}

#define DEFAULT_NV_VALIDITY_BITMAP 0xFFFFFFFF
#define MAGIC_NUMBER            0xCAFEBABE

/*----------------------------------------------------------------------------
 * Type Declarations
 * -------------------------------------------------------------------------*/
// this wrapper structure is identical to nv_cmd_type except the
// data_ptr type is changed void* to avoid exceeding the debug information
// module size as there are too many elements within nv_items_type union

// structure for code and regulatory domain of a single country
typedef struct
{
   v_U8_t            regDomain;
   v_COUNTRYCODE_t   countryCode;
} CountryInfo_t;
// structure of table to map country code and regulatory domain
typedef struct
{
   v_U16_t           countryCount;
   CountryInfo_t     countryInfo[MAX_COUNTRY_COUNT];
} CountryInfoTable_t;
/*----------------------------------------------------------------------------
 * Global Data Definitions
 * -------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Static Variable Definitions
 * -------------------------------------------------------------------------*/
// cache of country info table;
// this is re-initialized from data on binary file
// loaded on driver initialization if available
static CountryInfoTable_t countryInfoTable =
{
    254,
    {
        { REGDOMAIN_FCC,     {'U', 'S'}},  //USA - must be the first country code
        { REGDOMAIN_ETSI,    {'A', 'D'}},  //ANDORRA
        { REGDOMAIN_ETSI,    {'A', 'E'}},  //UAE
        { REGDOMAIN_N_AMER_EXC_FCC, {'A', 'F'}},  //AFGHANISTAN
        { REGDOMAIN_WORLD,   {'A', 'G'}},  //ANTIGUA AND BARBUDA
        { REGDOMAIN_FCC,     {'A', 'I'}},  //ANGUILLA
        { REGDOMAIN_ETSI,    {'A', 'L'}},  //ALBANIA
        { REGDOMAIN_N_AMER_EXC_FCC, {'A', 'M'}},  //ARMENIA
        { REGDOMAIN_ETSI,    {'A', 'N'}},  //NETHERLANDS ANTILLES
        { REGDOMAIN_NO_5GHZ, {'A', 'O'}},  //ANGOLA
        { REGDOMAIN_WORLD,   {'A', 'Q'}},  //ANTARCTICA
        { REGDOMAIN_WORLD,   {'A', 'R'}},  //ARGENTINA
        { REGDOMAIN_FCC,     {'A', 'S'}},  //AMERICAN SOMOA
        { REGDOMAIN_ETSI,    {'A', 'T'}},  //AUSTRIA
        { REGDOMAIN_WORLD,   {'A', 'U'}},  //AUSTRALIA
        { REGDOMAIN_ETSI,    {'A', 'W'}},  //ARUBA
        { REGDOMAIN_WORLD,   {'A', 'X'}},  //ALAND ISLANDS
        { REGDOMAIN_N_AMER_EXC_FCC, {'A', 'Z'}},  //AZERBAIJAN
        { REGDOMAIN_ETSI,    {'B', 'A'}},  //BOSNIA AND HERZEGOVINA
        { REGDOMAIN_APAC,    {'B', 'B'}},  //BARBADOS
        { REGDOMAIN_HI_5GHZ, {'B', 'D'}},  //BANGLADESH
        { REGDOMAIN_ETSI,    {'B', 'E'}},  //BELGIUM
        { REGDOMAIN_HI_5GHZ, {'B', 'F'}},  //BURKINA FASO
        { REGDOMAIN_ETSI,    {'B', 'G'}},  //BULGARIA
        { REGDOMAIN_APAC,    {'B', 'H'}},  //BAHRAIN
        { REGDOMAIN_NO_5GHZ, {'B', 'I'}},  //BURUNDI
        { REGDOMAIN_NO_5GHZ, {'B', 'J'}},  //BENIN
        { REGDOMAIN_FCC,     {'B', 'M'}},  //BERMUDA
        { REGDOMAIN_APAC,    {'B', 'N'}},  //BRUNEI DARUSSALAM
        { REGDOMAIN_HI_5GHZ, {'B', 'O'}},  //BOLIVIA
        { REGDOMAIN_WORLD,   {'B', 'R'}},  //BRAZIL
        { REGDOMAIN_APAC,    {'B', 'S'}},  //BAHAMAS
        { REGDOMAIN_NO_5GHZ, {'B', 'T'}},  //BHUTAN
        { REGDOMAIN_WORLD,   {'B', 'V'}},  //BOUVET ISLAND
        { REGDOMAIN_ETSI,    {'B', 'W'}},  //BOTSWANA
        { REGDOMAIN_ETSI,    {'B', 'Y'}},  //BELARUS
        { REGDOMAIN_HI_5GHZ, {'B', 'Z'}},  //BELIZE
        { REGDOMAIN_FCC,     {'C', 'A'}},  //CANADA
        { REGDOMAIN_WORLD,   {'C', 'C'}},  //COCOS (KEELING) ISLANDS
        { REGDOMAIN_NO_5GHZ, {'C', 'D'}},  //CONGO, THE DEMOCRATIC REPUBLIC OF THE
        { REGDOMAIN_NO_5GHZ, {'C', 'F'}},  //CENTRAL AFRICAN REPUBLIC
        { REGDOMAIN_NO_5GHZ, {'C', 'G'}},  //CONGO
        { REGDOMAIN_ETSI,    {'C', 'H'}},  //SWITZERLAND
        { REGDOMAIN_NO_5GHZ, {'C', 'I'}},  //COTE D'IVOIRE
        { REGDOMAIN_WORLD,   {'C', 'K'}},  //COOK ISLANDS
        { REGDOMAIN_APAC,    {'C', 'L'}},  //CHILE
        { REGDOMAIN_NO_5GHZ, {'C', 'M'}},  //CAMEROON
        { REGDOMAIN_APAC,    {'C', 'N'}},  //CHINA
        { REGDOMAIN_APAC,    {'C', 'O'}},  //COLOMBIA
        { REGDOMAIN_APAC,    {'C', 'R'}},  //COSTA RICA
        { REGDOMAIN_NO_5GHZ, {'C', 'U'}},  //CUBA
        { REGDOMAIN_ETSI,    {'C', 'V'}},  //CAPE VERDE
        { REGDOMAIN_WORLD,   {'C', 'X'}},  //CHRISTMAS ISLAND
        { REGDOMAIN_ETSI,    {'C', 'Y'}},  //CYPRUS
        { REGDOMAIN_ETSI,    {'C', 'Z'}},  //CZECH REPUBLIC
        { REGDOMAIN_ETSI,    {'D', 'E'}},  //GERMANY
        { REGDOMAIN_NO_5GHZ, {'D', 'J'}},  //DJIBOUTI
        { REGDOMAIN_ETSI,    {'D', 'K'}},  //DENMARK
        { REGDOMAIN_WORLD,   {'D', 'M'}},  //DOMINICA
        { REGDOMAIN_APAC,    {'D', 'O'}},  //DOMINICAN REPUBLIC
        { REGDOMAIN_ETSI,    {'D', 'Z'}},  //ALGERIA
        { REGDOMAIN_APAC,    {'E', 'C'}},  //ECUADOR
        { REGDOMAIN_ETSI,    {'E', 'E'}},  //ESTONIA
        { REGDOMAIN_N_AMER_EXC_FCC, {'E', 'G'}},  //EGYPT
        { REGDOMAIN_WORLD,   {'E', 'H'}},  //WESTERN SAHARA
        { REGDOMAIN_NO_5GHZ, {'E', 'R'}},  //ERITREA
        { REGDOMAIN_ETSI,    {'E', 'S'}},  //SPAIN
        { REGDOMAIN_ETSI,    {'E', 'T'}},  //ETHIOPIA
        { REGDOMAIN_ETSI,    {'E', 'U'}},  //Europe (SSGFI)
        { REGDOMAIN_ETSI,    {'F', 'I'}},  //FINLAND
        { REGDOMAIN_NO_5GHZ, {'F', 'J'}},  //FIJI
        { REGDOMAIN_WORLD,   {'F', 'K'}},  //FALKLAND ISLANDS (MALVINAS)
        { REGDOMAIN_WORLD,   {'F', 'M'}},  //MICRONESIA, FEDERATED STATES OF
        { REGDOMAIN_WORLD,   {'F', 'O'}},  //FAROE ISLANDS
        { REGDOMAIN_ETSI,    {'F', 'R'}},  //FRANCE
        { REGDOMAIN_NO_5GHZ, {'G', 'A'}},  //GABON
        { REGDOMAIN_ETSI,    {'G', 'B'}},  //UNITED KINGDOM
        { REGDOMAIN_WORLD,   {'G', 'D'}},  //GRENADA
        { REGDOMAIN_ETSI,    {'G', 'E'}},  //GEORGIA
        { REGDOMAIN_ETSI,    {'G', 'F'}},  //FRENCH GUIANA
        { REGDOMAIN_WORLD,   {'G', 'G'}},  //GUERNSEY
        { REGDOMAIN_WORLD,   {'G', 'H'}},  //GHANA
        { REGDOMAIN_WORLD,   {'G', 'I'}},  //GIBRALTAR
        { REGDOMAIN_ETSI,    {'G', 'L'}},  //GREENLAND
        { REGDOMAIN_NO_5GHZ, {'G', 'M'}},  //GAMBIA
        { REGDOMAIN_NO_5GHZ, {'G', 'N'}},  //GUINEA
        { REGDOMAIN_ETSI,    {'G', 'P'}},  //GUADELOUPE
        { REGDOMAIN_NO_5GHZ, {'G', 'Q'}},  //EQUATORIAL GUINEA
        { REGDOMAIN_ETSI,    {'G', 'R'}},  //GREECE
        { REGDOMAIN_WORLD,   {'G', 'S'}},  //SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS
        { REGDOMAIN_APAC,    {'G', 'T'}},  //GUATEMALA
        { REGDOMAIN_FCC,     {'G', 'U'}},  //GUAM
        { REGDOMAIN_NO_5GHZ, {'G', 'W'}},  //GUINEA-BISSAU
        { REGDOMAIN_HI_5GHZ, {'G', 'Y'}},  //GUYANA
        { REGDOMAIN_WORLD,   {'H', 'K'}},  //HONGKONG
        { REGDOMAIN_WORLD,   {'H', 'M'}},  //HEARD ISLAND AND MCDONALD ISLANDS
        { REGDOMAIN_WORLD,   {'H', 'N'}},  //HONDURAS
        { REGDOMAIN_ETSI,    {'H', 'R'}},  //CROATIA
        { REGDOMAIN_ETSI,    {'H', 'T'}},  //HAITI
        { REGDOMAIN_ETSI,    {'H', 'U'}},  //HUNGARY
        { REGDOMAIN_HI_5GHZ, {'I', 'D'}},  //INDONESIA
        { REGDOMAIN_ETSI,    {'I', 'E'}},  //IRELAND
        { REGDOMAIN_N_AMER_EXC_FCC, {'I', 'L'}},  //ISRAEL
        { REGDOMAIN_WORLD,   {'I', 'M'}},  //ISLE OF MAN
        { REGDOMAIN_APAC,    {'I', 'N'}},  //INDIA
        { REGDOMAIN_WORLD,   {'I', 'O'}},  //BRITISH INDIAN OCEAN TERRITORY
        { REGDOMAIN_NO_5GHZ, {'I', 'Q'}},  //IRAQ
        { REGDOMAIN_HI_5GHZ, {'I', 'R'}},  //IRAN, ISLAMIC REPUBLIC OF
        { REGDOMAIN_ETSI,    {'I', 'S'}},  //ICELAND
        { REGDOMAIN_ETSI,    {'I', 'T'}},  //ITALY
        { REGDOMAIN_JAPAN,   {'J', '1'}},  //Japan alternate 1
        { REGDOMAIN_JAPAN,   {'J', '2'}},  //Japan alternate 2
        { REGDOMAIN_JAPAN,   {'J', '3'}},  //Japan alternate 3
        { REGDOMAIN_JAPAN,   {'J', '4'}},  //Japan alternate 4
        { REGDOMAIN_JAPAN,   {'J', '5'}},  //Japan alternate 5
        { REGDOMAIN_WORLD,   {'J', 'E'}},  //JERSEY
        { REGDOMAIN_WORLD,   {'J', 'M'}},  //JAMAICA
        { REGDOMAIN_APAC,    {'J', 'O'}},  //JORDAN
        { REGDOMAIN_JAPAN,   {'J', 'P'}},  //JAPAN
        { REGDOMAIN_KOREA,   {'K', '1'}},  //Korea alternate 1
        { REGDOMAIN_KOREA,   {'K', '2'}},  //Korea alternate 2
        { REGDOMAIN_KOREA,   {'K', '3'}},  //Korea alternate 3
        { REGDOMAIN_KOREA,   {'K', '4'}},  //Korea alternate 4
        { REGDOMAIN_APAC,    {'K', 'E'}},  //KENYA
        { REGDOMAIN_NO_5GHZ, {'K', 'G'}},  //KYRGYZSTAN
        { REGDOMAIN_ETSI,    {'K', 'H'}},  //CAMBODIA
        { REGDOMAIN_WORLD,   {'K', 'I'}},  //KIRIBATI
        { REGDOMAIN_NO_5GHZ, {'K', 'M'}},  //COMOROS
        { REGDOMAIN_WORLD,   {'K', 'N'}},  //SAINT KITTS AND NEVIS
        { REGDOMAIN_WORLD,   {'K', 'P'}},  //KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF
        { REGDOMAIN_KOREA,   {'K', 'R'}},  //KOREA, REPUBLIC OF
        { REGDOMAIN_N_AMER_EXC_FCC, {'K', 'W'}},  //KUWAIT
        { REGDOMAIN_FCC,     {'K', 'Y'}},  //CAYMAN ISLANDS
        { REGDOMAIN_WORLD,   {'K', 'Z'}},  //KAZAKHSTAN
        { REGDOMAIN_WORLD,   {'L', 'A'}},  //LAO PEOPLE'S DEMOCRATIC REPUBLIC
        { REGDOMAIN_WORLD,   {'L', 'B'}},  //LEBANON
        { REGDOMAIN_WORLD,   {'L', 'C'}},  //SAINT LUCIA
        { REGDOMAIN_ETSI,    {'L', 'I'}},  //LIECHTENSTEIN
        { REGDOMAIN_WORLD,   {'L', 'K'}},  //SRI LANKA
        { REGDOMAIN_WORLD,   {'L', 'R'}},  //LIBERIA
        { REGDOMAIN_ETSI,    {'L', 'S'}},  //LESOTHO
        { REGDOMAIN_ETSI,    {'L', 'T'}},  //LITHUANIA
        { REGDOMAIN_ETSI,    {'L', 'U'}},  //LUXEMBOURG
        { REGDOMAIN_ETSI,    {'L', 'V'}},  //LATVIA
        { REGDOMAIN_NO_5GHZ, {'L', 'Y'}},  //LIBYAN ARAB JAMAHIRIYA
        { REGDOMAIN_APAC,    {'M', 'A'}},  //MOROCCO
        { REGDOMAIN_ETSI,    {'M', 'C'}},  //MONACO
        { REGDOMAIN_ETSI,    {'M', 'D'}},  //MOLDOVA, REPUBLIC OF
        { REGDOMAIN_ETSI,    {'M', 'E'}},  //MONTENEGRO
        { REGDOMAIN_NO_5GHZ, {'M', 'G'}},  //MADAGASCAR
        { REGDOMAIN_WORLD,   {'M', 'H'}},  //MARSHALL ISLANDS
        { REGDOMAIN_ETSI,    {'M', 'K'}},  //MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF
        { REGDOMAIN_NO_5GHZ, {'M', 'L'}},  //MALI
        { REGDOMAIN_WORLD,   {'M', 'M'}},  //MYANMAR
        { REGDOMAIN_WORLD,   {'M', 'N'}},  //MONGOLIA
        { REGDOMAIN_APAC,    {'M', 'O'}},  //MACAO
        { REGDOMAIN_FCC,     {'M', 'P'}},  //NORTHERN MARIANA ISLANDS
        { REGDOMAIN_ETSI,    {'M', 'Q'}},  //MARTINIQUE
        { REGDOMAIN_ETSI,    {'M', 'R'}},  //MAURITANIA
        { REGDOMAIN_ETSI,    {'M', 'S'}},  //MONTSERRAT
        { REGDOMAIN_ETSI,    {'M', 'T'}},  //MALTA
        { REGDOMAIN_ETSI,    {'M', 'U'}},  //MAURITIUS
        { REGDOMAIN_APAC,    {'M', 'V'}},  //MALDIVES
        { REGDOMAIN_HI_5GHZ, {'M', 'W'}},  //MALAWI
        { REGDOMAIN_APAC,    {'M', 'X'}},  //MEXICO
        { REGDOMAIN_APAC,    {'M', 'Y'}},  //MALAYSIA
        { REGDOMAIN_WORLD,   {'M', 'Z'}},  //MOZAMBIQUE
        { REGDOMAIN_WORLD,   {'N', 'A'}},  //NAMIBIA
        { REGDOMAIN_NO_5GHZ, {'N', 'C'}},  //NEW CALEDONIA
        { REGDOMAIN_WORLD,   {'N', 'E'}},  //NIGER
        { REGDOMAIN_WORLD,   {'N', 'F'}},  //NORFOLD ISLAND
        { REGDOMAIN_WORLD,   {'N', 'G'}},  //NIGERIA
        { REGDOMAIN_WORLD,   {'N', 'I'}},  //NICARAGUA
        { REGDOMAIN_ETSI,    {'N', 'L'}},  //NETHERLANDS
        { REGDOMAIN_ETSI,    {'N', 'O'}},  //NORWAY
        { REGDOMAIN_APAC,    {'N', 'P'}},  //NEPAL
        { REGDOMAIN_NO_5GHZ, {'N', 'R'}},  //NAURU
        { REGDOMAIN_WORLD,   {'N', 'U'}},  //NIUE
        { REGDOMAIN_APAC,    {'N', 'Z'}},  //NEW ZEALAND
        { REGDOMAIN_ETSI,    {'O', 'M'}},  //OMAN
        { REGDOMAIN_APAC,    {'P', 'A'}},  //PANAMA
        { REGDOMAIN_WORLD,   {'P', 'E'}},  //PERU
        { REGDOMAIN_ETSI,    {'P', 'F'}},  //FRENCH POLYNESIA
        { REGDOMAIN_WORLD,   {'P', 'G'}},  //PAPUA NEW GUINEA
        { REGDOMAIN_WORLD,   {'P', 'H'}},  //PHILIPPINES
        { REGDOMAIN_HI_5GHZ, {'P', 'K'}},  //PAKISTAN
        { REGDOMAIN_ETSI,    {'P', 'L'}},  //POLAND
        { REGDOMAIN_WORLD,   {'P', 'M'}},  //SAINT PIERRE AND MIQUELON
        { REGDOMAIN_WORLD,   {'P', 'N'}},  //WORLDPITCAIRN
        { REGDOMAIN_FCC,     {'P', 'R'}},  //PUERTO RICO
        { REGDOMAIN_WORLD,   {'P', 'S'}},  //PALESTINIAN TERRITORY, OCCUPIED
        { REGDOMAIN_ETSI,    {'P', 'T'}},  //PORTUGAL
        { REGDOMAIN_WORLD,   {'P', 'W'}},  //PALAU
        { REGDOMAIN_WORLD,   {'P', 'Y'}},  //PARAGUAY
        { REGDOMAIN_HI_5GHZ, {'Q', 'A'}},  //QATAR
        { REGDOMAIN_ETSI,    {'R', 'E'}},  //REUNION
        { REGDOMAIN_ETSI,    {'R', 'O'}},  //ROMANIA
        { REGDOMAIN_ETSI,    {'R', 'S'}},  //SERBIA
        { REGDOMAIN_APAC,    {'R', 'U'}},  //RUSSIA
        { REGDOMAIN_WORLD,   {'R', 'W'}},  //RWANDA
        { REGDOMAIN_WORLD,   {'S', 'A'}},  //SAUDI ARABIA
        { REGDOMAIN_NO_5GHZ, {'S', 'B'}},  //SOLOMON ISLANDS
        { REGDOMAIN_NO_5GHZ, {'S', 'C'}},  //SEYCHELLES
        { REGDOMAIN_WORLD,   {'S', 'D'}},  //SUDAN
        { REGDOMAIN_ETSI,    {'S', 'E'}},  //SWEDEN
        { REGDOMAIN_APAC,    {'S', 'G'}},  //SINGAPORE
        { REGDOMAIN_WORLD,   {'S', 'H'}},  //SAINT HELENA
        { REGDOMAIN_ETSI,    {'S', 'I'}},  //SLOVENNIA
        { REGDOMAIN_WORLD,   {'S', 'J'}},  //SVALBARD AND JAN MAYEN
        { REGDOMAIN_ETSI,    {'S', 'K'}},  //SLOVAKIA
        { REGDOMAIN_WORLD,   {'S', 'L'}},  //SIERRA LEONE
        { REGDOMAIN_ETSI,    {'S', 'M'}},  //SAN MARINO
        { REGDOMAIN_ETSI,    {'S', 'N'}},  //SENEGAL
        { REGDOMAIN_NO_5GHZ, {'S', 'O'}},  //SOMALIA
        { REGDOMAIN_NO_5GHZ, {'S', 'R'}},  //SURINAME
        { REGDOMAIN_WORLD,   {'S', 'T'}},  //SAO TOME AND PRINCIPE
        { REGDOMAIN_APAC,    {'S', 'V'}},  //EL SALVADOR
        { REGDOMAIN_NO_5GHZ, {'S', 'Y'}},  //SYRIAN ARAB REPUBLIC
        { REGDOMAIN_NO_5GHZ, {'S', 'Z'}},  //SWAZILAND
        { REGDOMAIN_ETSI,    {'T', 'C'}},  //TURKS AND CAICOS ISLANDS
        { REGDOMAIN_NO_5GHZ, {'T', 'D'}},  //CHAD
        { REGDOMAIN_ETSI,    {'T', 'F'}},  //FRENCH SOUTHERN TERRITORIES
        { REGDOMAIN_NO_5GHZ, {'T', 'G'}},  //TOGO
        { REGDOMAIN_WORLD,   {'T', 'H'}},  //THAILAND
        { REGDOMAIN_NO_5GHZ, {'T', 'J'}},  //TAJIKISTAN
        { REGDOMAIN_WORLD,   {'T', 'K'}},  //TOKELAU
        { REGDOMAIN_WORLD,   {'T', 'L'}},  //TIMOR-LESTE
        { REGDOMAIN_NO_5GHZ, {'T', 'M'}},  //TURKMENISTAN
        { REGDOMAIN_N_AMER_EXC_FCC, {'T', 'N'}},  //TUNISIA
        { REGDOMAIN_NO_5GHZ, {'T', 'O'}},  //TONGA
        { REGDOMAIN_ETSI,    {'T', 'R'}},  //TURKEY
        { REGDOMAIN_WORLD,   {'T', 'T'}},  //TRINIDAD AND TOBAGO
        { REGDOMAIN_NO_5GHZ, {'T', 'V'}},  //TUVALU
        { REGDOMAIN_FCC,     {'T', 'W'}},  //TAIWAN, PROVINCE OF CHINA
        { REGDOMAIN_HI_5GHZ, {'T', 'Z'}},  //TANZANIA, UNITED REPUBLIC OF
        { REGDOMAIN_WORLD,   {'U', 'A'}},  //UKRAINE
        { REGDOMAIN_KOREA,   {'U', 'G'}},  //UGANDA
        { REGDOMAIN_FCC,     {'U', 'M'}},  //UNITED STATES MINOR OUTLYING ISLANDS
        { REGDOMAIN_WORLD,   {'U', 'Y'}},  //URUGUAY
        { REGDOMAIN_FCC,     {'U', 'Z'}},  //UZBEKISTAN
        { REGDOMAIN_ETSI,    {'V', 'A'}},  //HOLY SEE (VATICAN CITY STATE)
        { REGDOMAIN_WORLD,   {'V', 'C'}},  //SAINT VINCENT AND THE GRENADINES
        { REGDOMAIN_HI_5GHZ, {'V', 'E'}},  //VENEZUELA
        { REGDOMAIN_ETSI,    {'V', 'G'}},  //VIRGIN ISLANDS, BRITISH
        { REGDOMAIN_FCC,     {'V', 'I'}},  //VIRGIN ISLANDS, US
        { REGDOMAIN_FCC,     {'V', 'N'}},  //VIET NAM
        { REGDOMAIN_NO_5GHZ, {'V', 'U'}},  //VANUATU
        { REGDOMAIN_WORLD,   {'W', 'F'}},  //WALLIS AND FUTUNA
        { REGDOMAIN_N_AMER_EXC_FCC, {'W', 'S'}},  //SOMOA
        { REGDOMAIN_NO_5GHZ, {'Y', 'E'}},  //YEMEN
        { REGDOMAIN_ETSI,    {'Y', 'T'}},  //MAYOTTE
        { REGDOMAIN_WORLD,   {'Z', 'A'}},  //SOUTH AFRICA
        { REGDOMAIN_APAC,    {'Z', 'M'}},  //ZAMBIA
        { REGDOMAIN_ETSI,    {'Z', 'W'}},  //ZIMBABWE
    }
};
typedef struct nvEFSTable_s
{
   v_U32_t    nvValidityBitmap;
   sHalNv     halnv;
} nvEFSTable_t;
nvEFSTable_t *gnvEFSTable;
/* EFS Table  to send the NV structure to HAL*/ 
static nvEFSTable_t *pnvEFSTable;
static v_U8_t *pnvEncodedBuf;
static v_U8_t *pDictFile;
static v_U8_t *pEncodedBuf;
static v_SIZE_t nvReadEncodeBufSize;
static v_SIZE_t nDictionarySize;
static v_U32_t magicNumber;

const tRfChannelProps rfChannels[NUM_RF_CHANNELS] =
{
    //RF_SUBBAND_2_4_GHZ
    //freq, chan#, band
    { 2412, 1  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_1,
    { 2417, 2  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_2,
    { 2422, 3  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_3,
    { 2427, 4  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_4,
    { 2432, 5  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_5,
    { 2437, 6  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_6,
    { 2442, 7  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_7,
    { 2447, 8  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_8,
    { 2452, 9  , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_9,
    { 2457, 10 , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_10,
    { 2462, 11 , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_11,
    { 2467, 12 , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_12,
    { 2472, 13 , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_13,
    { 2484, 14 , RF_SUBBAND_2_4_GHZ},        //RF_CHAN_14,
    { 4920, 240, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_240,
    { 4940, 244, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_244,
    { 4960, 248, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_248,
    { 4980, 252, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_252,
    { 5040, 208, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_208,
    { 5060, 212, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_212,
    { 5080, 216, RF_SUBBAND_4_9_GHZ},        //RF_CHAN_216,
    { 5180, 36 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_36,
    { 5200, 40 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_40,
    { 5220, 44 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_44,
    { 5240, 48 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_48,
    { 5260, 52 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_52,
    { 5280, 56 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_56,
    { 5300, 60 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_60,
    { 5320, 64 , RF_SUBBAND_5_LOW_GHZ},      //RF_CHAN_64,
    { 5500, 100, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_100,
    { 5520, 104, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_104,
    { 5540, 108, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_108,
    { 5560, 112, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_112,
    { 5580, 116, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_116,
    { 5600, 120, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_120,
    { 5620, 124, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_124,
    { 5640, 128, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_128,
    { 5660, 132, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_132,
    { 5680, 136, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_136,
    { 5700, 140, RF_SUBBAND_5_MID_GHZ},      //RF_CHAN_140,
    { 5745, 149, RF_SUBBAND_5_HIGH_GHZ},     //RF_CHAN_149,
    { 5765, 153, RF_SUBBAND_5_HIGH_GHZ},     //RF_CHAN_153,
    { 5785, 157, RF_SUBBAND_5_HIGH_GHZ},     //RF_CHAN_157,
    { 5805, 161, RF_SUBBAND_5_HIGH_GHZ},     //RF_CHAN_161,
    { 5825, 165, RF_SUBBAND_5_HIGH_GHZ},     //RF_CHAN_165,
    { 2422, 3  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_3,
    { 2427, 4  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_4,
    { 2432, 5  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_5,
    { 2437, 6  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_6,
    { 2442, 7  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_7,
    { 2447, 8  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_8,
    { 2452, 9  , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_9,
    { 2457, 10 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_10,
    { 2462, 11 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_11,
    { 4930, 242, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_242,
    { 4950, 246, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_246,
    { 4970, 250, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_250,
    { 5050, 210, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_210,
    { 5070, 214, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_214,
    { 5190, 38 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_38,
    { 5210, 42 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_42,
    { 5230, 46 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_46,
    { 5250, 50 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_50,
    { 5270, 54 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_54,
    { 5290, 58 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_58,
    { 5310, 62 , NUM_RF_SUBBANDS},           //RF_CHAN_BOND_62,
    { 5510, 102, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_102,
    { 5530, 106, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_106,
    { 5550, 110, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_110,
    { 5570, 114, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_114,
    { 5590, 118, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_118,
    { 5610, 122, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_122,
    { 5630, 126, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_126,
    { 5650, 130, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_130,
    { 5670, 134, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_134,
    { 5690, 138, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_138,
    { 5755, 151, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_151,
    { 5775, 155, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_155,
    { 5795, 159, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_159,
    { 5815, 163, NUM_RF_SUBBANDS},           //RF_CHAN_BOND_163,
};

extern const sHalNv nvDefaults;

const sRegulatoryChannel * regChannels = nvDefaults.tables.regDomains[0].channels;

/*----------------------------------------------------------------------------
   Function Definitions and Documentation
 * -------------------------------------------------------------------------*/
VOS_STATUS wlan_write_to_efs (v_U8_t *pData, v_U16_t data_len);
/**------------------------------------------------------------------------
  \brief vos_nv_init() - initialize the NV module
  The \a vos_nv_init() initializes the NV module.  This read the binary
  file for country code and regulatory domain information.
  \return VOS_STATUS_SUCCESS - module is initialized successfully
          otherwise  - module is not initialized
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_init(void)
{
   return VOS_STATUS_SUCCESS;
}

/**------------------------------------------------------------------------
  \brief vos_nv_get_dictionary_data() - get the dictionary data required for
  \ tools
  \return VOS_STATUS_SUCCESS - dictionary data is read successfully
          otherwise  - not successful
  \sa
-------------------------------------------------------------------------*/
VOS_STATUS vos_nv_get_dictionary_data(void)
{
   VOS_STATUS vosStatus = VOS_STATUS_E_FAILURE;

   if (MAGIC_NUMBER != magicNumber)
   {
      return VOS_STATUS_SUCCESS;
   }

   nDictionarySize = 0;

   vosStatus = vos_get_binary_blob( VOS_BINARY_ID_DICT_CONFIG, NULL,
                                                &nDictionarySize );
   if (VOS_STATUS_E_NOMEM != vosStatus)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 "Error obtaining binary size" );
/// NOTE:
/// We can still work without a dictionary file..
      return VOS_STATUS_SUCCESS;
   }

   // malloc a buffer to read in the Configuration binary file.
   pDictFile = vos_mem_malloc( nDictionarySize );
   if (NULL == pDictFile)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
              "Unable to allocate memory for the CFG binary [size= %d bytes]",
                 nDictionarySize );
      vosStatus = VOS_STATUS_E_NOMEM;
      goto fail;
   }

   /* Get the entire CFG file image... */
   vosStatus = vos_get_binary_blob( VOS_BINARY_ID_DICT_CONFIG, pDictFile,
                                                         &nDictionarySize );
   if (!VOS_IS_STATUS_SUCCESS( vosStatus ))
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
         "Error: Cannot retrieve CFG file image from vOSS. [size= %d bytes]",
                                                             nDictionarySize );
      return VOS_STATUS_SUCCESS;
   }

   VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO_HIGH,
         "Dict file image from vOSS. [size= %d bytes]", nDictionarySize );

fail:
   return vosStatus;
}

VOS_STATUS vos_nv_open(void)
{
    VOS_STATUS status = VOS_STATUS_SUCCESS;
    v_CONTEXT_t pVosContext= NULL;
    v_SIZE_t bufSize;
    v_SIZE_t nvReadBufSize;
    v_BOOL_t itemIsValid = VOS_FALSE;
    v_U32_t dataOffset;
    sHalNv *pnvData = NULL;

    /*Get the global context */
    pVosContext = vos_get_global_context(VOS_MODULE_ID_SYS, NULL);

    if (NULL == pVosContext)
    {
        return (eHAL_STATUS_FAILURE);
    }

    bufSize = sizeof(nvEFSTable_t);
    status = hdd_request_firmware(WLAN_NV_FILE,
                                  ((VosContextType*)(pVosContext))->pHDDContext,
                                  (v_VOID_t**)&pnvEncodedBuf, &nvReadBufSize);

    if ((!VOS_IS_STATUS_SUCCESS( status )) || (!pnvEncodedBuf))
    {
       VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_FATAL,
                   "%s: unable to download NV file %s",
                   __func__, WLAN_NV_FILE);
       return VOS_STATUS_E_RESOURCES;
    }

    memcpy(&magicNumber, &pnvEncodedBuf[sizeof(v_U32_t)], sizeof(v_U32_t));

    /// Allocate buffer with maximum length..
    pEncodedBuf = (v_U8_t *)vos_mem_malloc(nvReadBufSize);

    if (NULL == pEncodedBuf)
    {
        VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                    "%s : failed to allocate memory for NV", __func__);
        return VOS_STATUS_E_NOMEM;
    }

    gnvEFSTable = (nvEFSTable_t*)pnvEncodedBuf;

    if (MAGIC_NUMBER == magicNumber)
    {
        pnvData = (sHalNv *)vos_mem_malloc(sizeof(sHalNv));

        if (NULL == pnvData)
        {
            VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                      "%s : failed to allocate memory for NV", __func__);
            return VOS_STATUS_E_NOMEM;
        }

        memset(pnvData, 0, sizeof(sHalNv));

        /// Data starts from offset of validity bit map + magic number..
        dataOffset = sizeof(v_U32_t) + sizeof(v_U32_t);

        status = nvParser(&pnvEncodedBuf[dataOffset],
                     (nvReadBufSize-dataOffset), pnvData);

        ///ignore validity bit map
        nvReadEncodeBufSize = nvReadBufSize - sizeof(v_U32_t);

        vos_mem_copy(pEncodedBuf, &pnvEncodedBuf[sizeof(v_U32_t)],
            nvReadEncodeBufSize);

        VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                   "readEncodeBufSize %d",nvReadEncodeBufSize);

        if (VOS_STATUS_SUCCESS == status) {
           VOS_TRACE(VOS_MODULE_ID_VOSS,  VOS_TRACE_LEVEL_ERROR,
                       "Embedded NV parsed success !!productId %d couple Type %d wlan RevId %d",
                        pnvData->fields.productId,
                        pnvData->fields.couplerType,
                        pnvData->fields.wlanNvRevId);

           vos_mem_copy(&gnvEFSTable->halnv, pnvData, sizeof(sHalNv));

           nvReadBufSize = sizeof(sHalNv) + sizeof(v_U32_t);
        }
        else
        {
           VOS_TRACE(VOS_MODULE_ID_VOSS,  VOS_TRACE_LEVEL_ERROR,
                       "nvParser failed %d",status);

           nvReadBufSize = 0;

           vos_mem_copy(pEncodedBuf, &nvDefaults, sizeof(sHalNv));

           nvReadEncodeBufSize = sizeof(sHalNv);
        }
    }
    else
    {
       dataOffset = sizeof(v_U32_t);
       nvReadEncodeBufSize = sizeof(sHalNv);
       memcpy(pEncodedBuf, &pnvEncodedBuf[dataOffset], nvReadEncodeBufSize);
    }

    if (NULL != pnvData)
    {
       vos_mem_free(pnvData);
    }

    VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
           "INFO: NV binary file version=%d Driver default NV version=%d, continue...\n",
           gnvEFSTable->halnv.fields.nvVersion, WLAN_NV_VERSION);

     /* Copying the read nv data to the globa NV EFS table */
    {
        /* Allocate memory to global NV table */
        pnvEFSTable = (nvEFSTable_t *)vos_mem_malloc(sizeof(nvEFSTable_t));
        if ( NULL == pnvEFSTable )
        {
            VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                      "%s : failed to allocate memory for NV", __func__);
            return VOS_STATUS_E_NOMEM;
        }

        /*Copying the NV defaults */
        vos_mem_copy(&(pnvEFSTable->halnv), &nvDefaults, sizeof(sHalNv));

        /* Size mismatch */
        if ( nvReadBufSize != bufSize)
        {
            pnvEFSTable->nvValidityBitmap = DEFAULT_NV_VALIDITY_BITMAP;
            VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_FATAL,
                      "!!!WARNING: INVALID NV FILE, DRIVER IS USING DEFAULT CAL VALUES %d %d!!!",
                      nvReadBufSize, bufSize);
            return VOS_STATUS_SUCCESS;
        }

       /* Version mismatch */
       if (gnvEFSTable->halnv.fields.nvVersion != WLAN_NV_VERSION)
       {
           if ((WLAN_NV_VERSION == NV_VERSION_11N_11AC_FW_CONFIG) &&
               (gnvEFSTable->halnv.fields.nvVersion == NV_VERSION_11N_11AC_COUPER_TYPE))
           {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                     "!!!WARNING: Using Coupler Type field instead of Fw Config table,\n"
                     "Make sure that this is intented or may impact performance!!!\n");
           }
           else
           {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                     "!!!WARNING: NV binary file version doesn't match with Driver default NV version\n"
                     "Driver NV defaults will be used, may impact performance!!!\n");

               return VOS_STATUS_SUCCESS;
           }
       }

       pnvEFSTable->nvValidityBitmap = gnvEFSTable->nvValidityBitmap;
        /* Copy the valid fields to the NV Global structure */ 
        if (vos_nv_getValidity(VNV_FIELD_IMAGE, &itemIsValid) == 
           VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE) {

                if(vos_nv_read( VNV_FIELD_IMAGE, (v_VOID_t *)&pnvEFSTable->halnv.fields,
                   NULL, sizeof(sNvFields) ) != VOS_STATUS_SUCCESS)
                   goto error;
            }
        }

        if (vos_nv_getValidity(VNV_RATE_TO_POWER_TABLE, &itemIsValid) == 
             VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_RATE_TO_POWER_TABLE, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.pwrOptimum[0],
                  NULL, sizeof(tRateGroupPwr) * NUM_RF_SUBBANDS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_REGULARTORY_DOMAIN_TABLE, &itemIsValid) == 
               VOS_STATUS_SUCCESS)
        {
    
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_REGULARTORY_DOMAIN_TABLE,
                (v_VOID_t *)&pnvEFSTable->halnv.tables.regDomains[0],
                 NULL, sizeof(sRegulatoryDomains) * NUM_REG_DOMAINS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_DEFAULT_LOCATION, &itemIsValid) == 
            VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_DEFAULT_LOCATION,
                (v_VOID_t *)&pnvEFSTable->halnv.tables.defaultCountryTable,
                NULL, sizeof(sDefaultCountry) ) !=  VOS_STATUS_SUCCESS)
                    goto error;
            }
        }
    
        if (vos_nv_getValidity(VNV_TPC_POWER_TABLE, &itemIsValid) == 
            VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_TPC_POWER_TABLE, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.plutCharacterized[0],
                  NULL, sizeof(tTpcPowerTable) * NUM_RF_CHANNELS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }
    
        if (vos_nv_getValidity(VNV_TPC_PDADC_OFFSETS, &itemIsValid) == 
            VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_TPC_PDADC_OFFSETS,
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.plutPdadcOffset[0],
                  NULL, sizeof(tANI_U16) * NUM_RF_CHANNELS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }
        if (vos_nv_getValidity(VNV_RSSI_CHANNEL_OFFSETS, &itemIsValid) == 
           VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_RSSI_CHANNEL_OFFSETS,
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.rssiChanOffsets[0],
                  NULL, sizeof(sRssiChannelOffsets) * 2 ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }
    
        if (vos_nv_getValidity(VNV_HW_CAL_VALUES, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_HW_CAL_VALUES, (v_VOID_t *)&pnvEFSTable->halnv
    .tables.hwCalValues, NULL, sizeof(sHwCalValues) ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_FW_CONFIG, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_FW_CONFIG, (v_VOID_t *)&pnvEFSTable->halnv
    .tables.fwConfig, NULL, sizeof(sFwConfig) ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_ANTENNA_PATH_LOSS, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_ANTENNA_PATH_LOSS,
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.antennaPathLoss[0], NULL, 
                sizeof(tANI_S16)*NUM_RF_CHANNELS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }
        if (vos_nv_getValidity(VNV_PACKET_TYPE_POWER_LIMITS, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_PACKET_TYPE_POWER_LIMITS, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.pktTypePwrLimits[0], NULL, 
                sizeof(tANI_S16)*NUM_802_11_MODES*NUM_RF_CHANNELS ) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_OFDM_CMD_PWR_OFFSET, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
                if(vos_nv_read( VNV_OFDM_CMD_PWR_OFFSET, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.ofdmCmdPwrOffset, NULL, 
                                sizeof(sOfdmCmdPwrOffset)) != VOS_STATUS_SUCCESS)
                    goto error;
            }
        }

        if (vos_nv_getValidity(VNV_TX_BB_FILTER_MODE, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
               if(vos_nv_read(VNV_TX_BB_FILTER_MODE, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.txbbFilterMode, NULL, 
                sizeof(sTxBbFilterMode)) != VOS_STATUS_SUCCESS)
                   goto error;
            }
        }
        if (vos_nv_getValidity(VNV_TABLE_VIRTUAL_RATE, &itemIsValid) == 
         VOS_STATUS_SUCCESS)
        {
            if (itemIsValid == VOS_TRUE)
            {
               if(vos_nv_read(VNV_TABLE_VIRTUAL_RATE, 
                  (v_VOID_t *)&pnvEFSTable->halnv.tables.pwrOptimum_virtualRate, NULL, 
                sizeof(gnvEFSTable->halnv.tables.pwrOptimum_virtualRate)) != VOS_STATUS_SUCCESS)
                   goto error;
            }
        }
    }

    return VOS_STATUS_SUCCESS;
error:
    vos_mem_free(pnvEFSTable);
    vos_mem_free(pEncodedBuf);
    return eHAL_STATUS_FAILURE ;
}

VOS_STATUS vos_nv_close(void)
{
    VOS_STATUS status = VOS_STATUS_SUCCESS;
    v_CONTEXT_t pVosContext= NULL;
         /*Get the global context */
    pVosContext = vos_get_global_context(VOS_MODULE_ID_SYS, NULL);
    status = hdd_release_firmware(WLAN_NV_FILE, ((VosContextType*)(pVosContext))->pHDDContext);
    if ( !VOS_IS_STATUS_SUCCESS( status ))
    {
        VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                         "%s : vos_open failed\n",__func__);
        return VOS_STATUS_E_FAILURE;
    }
    vos_mem_free(pnvEFSTable);
    vos_mem_free(pEncodedBuf);
    vos_mem_free(pDictFile);

    gnvEFSTable=NULL;
    return VOS_STATUS_SUCCESS;
}
/**------------------------------------------------------------------------
  \brief vos_nv_getRegDomainFromCountryCode() - get the regulatory domain of
  a country given its country code
  The \a vos_nv_getRegDomainFromCountryCode() returns the regulatory domain of
  a country given its country code.  This is done from reading a cached
  copy of the binary file.
  \param pRegDomain  - pointer to regulatory domain
  \param countryCode - country code
  \return VOS_STATUS_SUCCESS - regulatory domain is found for the given country
          VOS_STATUS_E_FAULT - invalid pointer error
          VOS_STATUS_E_EMPTY - country code table is empty
          VOS_STATUS_E_EXISTS - given country code does not exist in table
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getRegDomainFromCountryCode( v_REGDOMAIN_t *pRegDomain,
      const v_COUNTRYCODE_t countryCode )
{
   int i;
   v_CONTEXT_t pVosContext = NULL;
   hdd_context_t *pHddCtx = NULL;
   struct wiphy *wiphy = NULL;
   int status;
   // sanity checks
   if (NULL == pRegDomain)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
            ("Invalid reg domain pointer\r\n") );
      return VOS_STATUS_E_FAULT;
   }
   *pRegDomain = REGDOMAIN_COUNT;

   if (NULL == countryCode)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
            ("Country code array is NULL\r\n") );
      return VOS_STATUS_E_FAULT;
   }
   if (0 == countryInfoTable.countryCount)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
            ("Reg domain table is empty\r\n") );
      return VOS_STATUS_E_EMPTY;
   }
   /* If CRDA regulatory settings is valid, i.e. crda is enabled
      and reg_notifier is called back.
      Intercept here and redirect to the Reg domain table's CRDA
      entry if country code is crda's country.
      last one NUM_REG_DOMAINS-1 is reserved for crda */
   VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO_HIGH,
          "vos_nv_getRegDomainFromCountryCode %c%c\n",
          countryCode[0], countryCode[1]);

   if (crda_regulatory_entry_valid == VOS_TRUE)
   {
       if (crda_alpha2[0]==countryCode[0] && crda_alpha2[1]==countryCode[1])
       {
          *pRegDomain = NUM_REG_DOMAINS-1;
              VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO_HIGH,
              "vos_nv_getRegDomainFromCountryCode return crda init entry\n");
          return VOS_STATUS_SUCCESS;
       }
       if (run_time_alpha2[0]==countryCode[0] &&
           run_time_alpha2[1]==countryCode[1] &&
           crda_regulatory_run_time_entry_valid == VOS_TRUE)
       {
          *pRegDomain = NUM_REG_DOMAINS-2;
              VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO_HIGH,
              "vos_nv_getRegDomainFromCountryCode return crda none-default country entry\n");
           return VOS_STATUS_SUCCESS;
       }
       else
       {
           crda_regulatory_run_time_entry_valid = VOS_FALSE;
           pVosContext = vos_get_global_context(VOS_MODULE_ID_SYS, NULL);
           if (NULL != pVosContext)
               pHddCtx = vos_get_context(VOS_MODULE_ID_HDD, pVosContext);
           else
               return VOS_STATUS_E_EXISTS;
           if (NULL == pHddCtx)
           {
              VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                    ("Invalid pHddCtx pointer\r\n") );
              return VOS_STATUS_E_FAULT;
           }

           wiphy = pHddCtx->wiphy;
           INIT_COMPLETION(pHddCtx->driver_crda_req);
           regulatory_hint(wiphy, countryCode);
           status = wait_for_completion_interruptible_timeout(
                   &pHddCtx->driver_crda_req,
                   msecs_to_jiffies(CRDA_WAIT_TIME));
           if (!status)
           {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                       "%s: Timeout waiting for CRDA REQ", __func__);
           }

           if (crda_regulatory_run_time_entry_valid == VOS_TRUE)
           {
              VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO_HIGH,
                 "vos_nv_getRegDomainFromCountryCode return crda new none-default country entry\n");
               return VOS_STATUS_SUCCESS;
           }
           VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
              "vos_nv_getRegDomainFromCountryCode failed to get crda new none-default country entry\n");
           return VOS_STATUS_E_EXISTS;
       }
   }

   // iterate the country info table until end of table or the country code
   // is found
   for (i = 0; i < countryInfoTable.countryCount &&
         REGDOMAIN_COUNT == *pRegDomain; i++)
   {
      if (memcmp(countryCode, countryInfoTable.countryInfo[i].countryCode,
               VOS_COUNTRY_CODE_LEN) == 0)
      {
         // country code is found
         *pRegDomain = countryInfoTable.countryInfo[i].regDomain;
      }
   }
   if (REGDOMAIN_COUNT != *pRegDomain)
   {
      return VOS_STATUS_SUCCESS;
   }
   else
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_WARN,
            ("country code is not found\r\n"));
      return VOS_STATUS_E_EXISTS;
   }
}
/**------------------------------------------------------------------------
  \brief vos_nv_getSupportedCountryCode() - get the list of supported
  country codes
  The \a vos_nv_getSupportedCountryCode() encodes the list of supported
  country codes with paddings in the provided buffer
  \param pBuffer     - pointer to buffer where supported country codes
                       and paddings are encoded; this may be set to NULL
                       if user wishes to query the required buffer size to
                       get the country code list
  \param pBufferSize - this is the provided buffer size on input;
                       this is the required or consumed buffer size on output
  \return VOS_STATUS_SUCCESS - country codes are successfully encoded
          VOS_STATUS_E_NOMEM - country codes are not encoded because either
                               the buffer is NULL or buffer size is
                               sufficient
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getSupportedCountryCode( v_BYTE_t *pBuffer, v_SIZE_t *pBufferSize,
      v_SIZE_t paddingSize )
{
   v_SIZE_t providedBufferSize = *pBufferSize;
   int i;
   // pBufferSize now points to the required buffer size
   *pBufferSize = countryInfoTable.countryCount * (VOS_COUNTRY_CODE_LEN + paddingSize );
   if ( NULL == pBuffer || providedBufferSize < *pBufferSize )
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_INFO,
            ("Insufficient memory for country code list\r\n"));
      return VOS_STATUS_E_NOMEM;
   }
   for (i = 0; i < countryInfoTable.countryCount; i++)
   {
      memcpy( pBuffer, countryInfoTable.countryInfo[i].countryCode, VOS_COUNTRY_CODE_LEN );
      pBuffer += (VOS_COUNTRY_CODE_LEN + paddingSize );
   }
   return VOS_STATUS_SUCCESS;
}
/**------------------------------------------------------------------------
  \brief vos_nv_readTxAntennaCount() - return number of TX antenna
  \param pTxAntennaCount   - antenna count
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_readTxAntennaCount( v_U8_t *pTxAntennaCount )
{
   sNvFields fieldImage;
   VOS_STATUS status;
   status = vos_nv_read( VNV_FIELD_IMAGE, &fieldImage, NULL,
         sizeof(fieldImage) );
   if (VOS_STATUS_SUCCESS == status)
   {
      *pTxAntennaCount = fieldImage.numOfTxChains;
   }
   return status;
}
/**------------------------------------------------------------------------
  \brief vos_nv_readRxAntennaCount() - return number of RX antenna
  \param pRxAntennaCount   - antenna count
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_readRxAntennaCount( v_U8_t *pRxAntennaCount )
{
   sNvFields fieldImage;
   VOS_STATUS status;
   status = vos_nv_read( VNV_FIELD_IMAGE, &fieldImage, NULL,
         sizeof(fieldImage) );
   if (VOS_STATUS_SUCCESS == status)
   {
      *pRxAntennaCount = fieldImage.numOfRxChains;
   }
   return status;
}

/**------------------------------------------------------------------------
  \brief vos_nv_readMacAddress() - return the MAC address
  \param pMacAddress - MAC address
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_readMacAddress( v_MAC_ADDRESS_t pMacAddress )
{
   sNvFields fieldImage;
   VOS_STATUS status;
   status = vos_nv_read( VNV_FIELD_IMAGE, &fieldImage, NULL,
         sizeof(fieldImage) );
   if (VOS_STATUS_SUCCESS == status)
   {
      memcpy( pMacAddress, fieldImage.macAddr, VOS_MAC_ADDRESS_LEN );
   }
   else
   {
      //This part of the code can be removed when NV is programmed
      const v_U8_t macAddr[VOS_MAC_ADDRESS_LEN] = VOS_HARD_CODED_MAC;
      memcpy( pMacAddress, macAddr, VOS_MAC_ADDRESS_LEN );
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_WARN,
          " fail to get MAC address from NV, hardcoded to %02X-%02X-%02X-%02X-%02X%02X",
          macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
      status = VOS_STATUS_SUCCESS;
   }
   return status;
}

/**------------------------------------------------------------------------

  \brief vos_nv_readMultiMacAddress() - return the Multiple MAC addresses

  \param pMacAddress - MAC address
  \param macCount - Count of valid MAC addresses to get from NV field

  \return status of the NV read operation

  \sa

  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_readMultiMacAddress( v_U8_t *pMacAddress,
                                              v_U8_t  macCount )
{
   sNvFields   fieldImage;
   VOS_STATUS  status;
   v_U8_t      countLoop;
   v_U8_t     *pNVMacAddress;

   if((0 == macCount) || (VOS_MAX_CONCURRENCY_PERSONA < macCount) ||
      (NULL == pMacAddress))
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
          " Invalid Parameter from NV Client macCount %d, pMacAddress %p",
          macCount, pMacAddress);
   }

   status = vos_nv_read( VNV_FIELD_IMAGE, &fieldImage, NULL,
                         sizeof(fieldImage) );
   if (VOS_STATUS_SUCCESS == status)
   {
      pNVMacAddress = fieldImage.macAddr;
      for(countLoop = 0; countLoop < macCount; countLoop++)
      {
         vos_mem_copy(pMacAddress + (countLoop * VOS_MAC_ADDRESS_LEN),
                      pNVMacAddress + (countLoop * VOS_MAC_ADDRESS_LEN),
                      VOS_MAC_ADDRESS_LEN);
      }
   }
   else
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 "vos_nv_readMultiMacAddress Get NV Field Fail");
   }

   return status;
}

/**------------------------------------------------------------------------
  \brief vos_nv_setValidity() - set the validity of an NV item.
  The \a vos_nv_setValidity() validates and invalidates an NV item.  The
  validity information is stored in NV memory.
  One would get the VOS_STATUS_E_EXISTS error when reading an invalid item.
  An item becomes valid when one has written to it successfully.
  \param type        - NV item type
  \param itemIsValid - boolean value indicating the item's validity
  \return VOS_STATUS_SUCCESS - validity is set successfully
          VOS_STATUS_E_INVAL - one of the parameters is invalid
          VOS_STATUS_E_FAILURE - unknown error
  \sa
  -------------------------------------------------------------------------*/

VOS_STATUS vos_nv_setValidity( VNV_TYPE type, v_BOOL_t itemIsValid )
{
   v_U32_t lastNvValidityBitmap;
   v_U32_t newNvValidityBitmap;
   VOS_STATUS status = VOS_STATUS_SUCCESS;

   // check if the current NV type is valid
   if (VNV_TYPE_COUNT <= type)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("%s: invalid type=%d"), __func__, type );
      return VOS_STATUS_E_INVAL;
   }
   // read the validity bitmap
   lastNvValidityBitmap = gnvEFSTable->nvValidityBitmap;
   // modify the validity bitmap
   if (itemIsValid)
   {
       newNvValidityBitmap = lastNvValidityBitmap | (1 << type);
              // commit to NV store if bitmap has been modified
       if (newNvValidityBitmap != lastNvValidityBitmap)
       {
           gnvEFSTable->nvValidityBitmap = newNvValidityBitmap;
       }
   }
   else
   {
       newNvValidityBitmap = lastNvValidityBitmap & (~(1 << type));
       if (newNvValidityBitmap != lastNvValidityBitmap)
       {
           gnvEFSTable->nvValidityBitmap = newNvValidityBitmap;
           status = wlan_write_to_efs((v_U8_t*)gnvEFSTable,sizeof(nvEFSTable_t));
           if (! VOS_IS_STATUS_SUCCESS(status)) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR, ("vos_nv_write_to_efs failed!!!\r\n"));
               status = VOS_STATUS_E_FAULT;
           }
       }
   }

   return status;
}
/**------------------------------------------------------------------------
  \brief vos_nv_getValidity() - get the validity of an NV item.
  The \a vos_nv_getValidity() indicates if an NV item is valid.  The
  validity information is stored in NV memory.
  One would get the VOS_STATUS_E_EXISTS error when reading an invalid item.
  An item becomes valid when one has written to it successfully.
  \param type        - NV item type
  \param pItemIsValid- pointer to the boolean value indicating the item's
                       validity
  \return VOS_STATUS_SUCCESS - validity is determined successfully
          VOS_STATUS_E_INVAL - one of the parameters is invalid
          VOS_STATUS_E_FAILURE - unknown error
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getValidity( VNV_TYPE type, v_BOOL_t *pItemIsValid )
{
   v_U32_t nvValidityBitmap = gnvEFSTable->nvValidityBitmap;
   // check if the current NV type is valid
   if (VNV_TYPE_COUNT <= type)
   {
      VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("%s: invalid type=%d"), __func__, type );
      return VOS_STATUS_E_INVAL;
   }
   *pItemIsValid = (v_BOOL_t)((nvValidityBitmap >> type) & 1);
   return VOS_STATUS_SUCCESS;
}
/**------------------------------------------------------------------------
  \brief vos_nv_read() - read a NV item to an output buffer
  The \a vos_nv_read() reads a NV item to an output buffer.  If the item is
  an array, this function would read the entire array. One would get a
  VOS_STATUS_E_EXISTS error when reading an invalid item.
  For error conditions of VOS_STATUS_E_EXISTS and VOS_STATUS_E_FAILURE,
  if a default buffer is provided (with a non-NULL value),
  the default buffer content is copied to the output buffer.
  \param type  - NV item type
  \param outputBuffer   - output buffer
  \param defaultBuffer  - default buffer
  \param bufferSize  - output buffer size
  \return VOS_STATUS_SUCCESS - NV item is read successfully
          VOS_STATUS_E_INVAL - one of the parameters is invalid
          VOS_STATUS_E_FAULT - defaultBuffer point is NULL
          VOS_STATUS_E_EXISTS - NV type is unsupported
          VOS_STATUS_E_FAILURE - unknown error
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_read( VNV_TYPE type, v_VOID_t *outputVoidBuffer,
      v_VOID_t *defaultBuffer, v_SIZE_t bufferSize )
{
    VOS_STATUS status             = VOS_STATUS_SUCCESS;
    v_SIZE_t itemSize;
    v_BOOL_t itemIsValid = VOS_TRUE;

    // sanity check
    if (VNV_TYPE_COUNT <= type)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("%s: invalid type=%d"), __func__, type );
       return VOS_STATUS_E_INVAL;
    }
    if (NULL == outputVoidBuffer)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
             ("Buffer provided is NULL\r\n") );
       return VOS_STATUS_E_FAULT;
    }
    if (0 == bufferSize)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
             ("NV type=%d is invalid\r\n"), type );
       return VOS_STATUS_E_INVAL;
    }
    // check if the NV item has valid data
    status = vos_nv_getValidity( type, &itemIsValid );
   if (!itemIsValid)
   {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_WARN,
            "NV type=%d does not have valid data\r\n", type );
       return VOS_STATUS_E_EMPTY;
   }
   switch(type)
   {
       case VNV_FIELD_IMAGE:
           itemSize = sizeof(gnvEFSTable->halnv.fields);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.fields,bufferSize);
           }
           break;
       case VNV_RATE_TO_POWER_TABLE:
           itemSize = sizeof(gnvEFSTable->halnv.tables.pwrOptimum);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.pwrOptimum[0],bufferSize);
           }
           break;
       case VNV_REGULARTORY_DOMAIN_TABLE:
           itemSize = sizeof(gnvEFSTable->halnv.tables.regDomains);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.regDomains[0],bufferSize);
           }
           break;
       case VNV_DEFAULT_LOCATION:
           itemSize = sizeof(gnvEFSTable->halnv.tables.defaultCountryTable);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.defaultCountryTable,bufferSize);
           }
           break;
       case VNV_TPC_POWER_TABLE:
           itemSize = sizeof(gnvEFSTable->halnv.tables.plutCharacterized);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.plutCharacterized[0],bufferSize);
           }
           break;
       case VNV_TPC_PDADC_OFFSETS:
           itemSize = sizeof(gnvEFSTable->halnv.tables.plutPdadcOffset);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.plutPdadcOffset[0],bufferSize);
           }
           break;
       case VNV_RSSI_CHANNEL_OFFSETS:

           itemSize = sizeof(gnvEFSTable->halnv.tables.rssiChanOffsets);

           if(bufferSize != itemSize) {

               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.rssiChanOffsets[0],bufferSize);
           }
           break;
       case VNV_HW_CAL_VALUES:

           itemSize = sizeof(gnvEFSTable->halnv.tables.hwCalValues);

           if(bufferSize != itemSize) {

               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.hwCalValues,bufferSize);
           }
           break;
       case VNV_FW_CONFIG:
       
           itemSize = sizeof(gnvEFSTable->halnv.tables.fwConfig);
       
           if(bufferSize != itemSize) {
       
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.fwConfig,bufferSize);
           }
           break;
       case VNV_ANTENNA_PATH_LOSS:
           itemSize = sizeof(gnvEFSTable->halnv.tables.antennaPathLoss);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.antennaPathLoss[0],bufferSize);
           }
           break;
       case VNV_PACKET_TYPE_POWER_LIMITS:
           itemSize = sizeof(gnvEFSTable->halnv.tables.pktTypePwrLimits);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,gnvEFSTable->halnv.tables.pktTypePwrLimits,bufferSize);
           }
           break;
       case VNV_OFDM_CMD_PWR_OFFSET:
           itemSize = sizeof(gnvEFSTable->halnv.tables.ofdmCmdPwrOffset);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.ofdmCmdPwrOffset,bufferSize);
           }
           break;
       case VNV_TX_BB_FILTER_MODE:
           itemSize = sizeof(gnvEFSTable->halnv.tables.txbbFilterMode);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.txbbFilterMode,bufferSize);
           }
           break;


       case VNV_TABLE_VIRTUAL_RATE:
           itemSize = sizeof(gnvEFSTable->halnv.tables.pwrOptimum_virtualRate);
           if(bufferSize != itemSize) {
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(outputVoidBuffer,&gnvEFSTable->halnv.tables.pwrOptimum_virtualRate,bufferSize);
           }
           break;

       default:
         break;
   }
   return status;
}

/**------------------------------------------------------------------------
  \brief vos_nv_write() - write to a NV item from an input buffer
  The \a vos_nv_write() writes to a NV item from an input buffer. This would
  validate the NV item if the write operation is successful.
  \param type - NV item type
  \param inputBuffer - input buffer
  \param inputBufferSize - input buffer size
  \return VOS_STATUS_SUCCESS - NV item is read successfully
          VOS_STATUS_E_INVAL - one of the parameters is invalid
          VOS_STATUS_E_FAULT - outputBuffer pointer is NULL
          VOS_STATUS_E_EXISTS - NV type is unsupported
          VOS_STATUS_E_FAILURE   - unknown error
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_write( VNV_TYPE type, v_VOID_t *inputVoidBuffer,
      v_SIZE_t bufferSize )
{
    VOS_STATUS status = VOS_STATUS_SUCCESS;
    v_SIZE_t itemSize;

    // sanity check
    if (VNV_TYPE_COUNT <= type)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                  ("%s: invalid type=%d"), __func__, type );
       return VOS_STATUS_E_INVAL;
    }
    if (NULL == inputVoidBuffer)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
             ("Buffer provided is NULL\r\n") );
       return VOS_STATUS_E_FAULT;
    }
    if (0 == bufferSize)
    {
       VOS_TRACE( VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
             ("NV type=%d is invalid\r\n"), type );
       return VOS_STATUS_E_INVAL;
    }
    switch(type)
    {
        case VNV_FIELD_IMAGE:
            itemSize = sizeof(gnvEFSTable->halnv.fields);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.fields,inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_RATE_TO_POWER_TABLE:
            itemSize = sizeof(gnvEFSTable->halnv.tables.pwrOptimum);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.pwrOptimum[0],inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_REGULARTORY_DOMAIN_TABLE:
            itemSize = sizeof(gnvEFSTable->halnv.tables.regDomains);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.regDomains[0],inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_DEFAULT_LOCATION:
            itemSize = sizeof(gnvEFSTable->halnv.tables.defaultCountryTable);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.defaultCountryTable,inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_TPC_POWER_TABLE:
            itemSize = sizeof(gnvEFSTable->halnv.tables.plutCharacterized);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.plutCharacterized[0],inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_TPC_PDADC_OFFSETS:
            itemSize = sizeof(gnvEFSTable->halnv.tables.plutPdadcOffset);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.plutPdadcOffset[0],inputVoidBuffer,bufferSize);
            }
            break;
         case VNV_RSSI_CHANNEL_OFFSETS:

            itemSize = sizeof(gnvEFSTable->halnv.tables.rssiChanOffsets);

            if(bufferSize != itemSize) {

                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.rssiChanOffsets[0],inputVoidBuffer,bufferSize);
            }
            break;
         case VNV_HW_CAL_VALUES:

            itemSize = sizeof(gnvEFSTable->halnv.tables.hwCalValues);

            if(bufferSize != itemSize) {

                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.hwCalValues,inputVoidBuffer,bufferSize);
            }
            break;
        case VNV_FW_CONFIG:
        
           itemSize = sizeof(gnvEFSTable->halnv.tables.fwConfig);
        
           if(bufferSize != itemSize) {
        
               VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                 itemSize);
               status = VOS_STATUS_E_INVAL;
           }
           else {
               memcpy(&gnvEFSTable->halnv.tables.fwConfig,inputVoidBuffer,bufferSize);
           }
           break;
        case VNV_ANTENNA_PATH_LOSS:
            itemSize = sizeof(gnvEFSTable->halnv.tables.antennaPathLoss);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.antennaPathLoss[0],inputVoidBuffer,bufferSize);
            }
            break;

        case VNV_PACKET_TYPE_POWER_LIMITS:
            itemSize = sizeof(gnvEFSTable->halnv.tables.pktTypePwrLimits);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(gnvEFSTable->halnv.tables.pktTypePwrLimits,inputVoidBuffer,bufferSize);
            }
            break;

        case VNV_OFDM_CMD_PWR_OFFSET:
            itemSize = sizeof(gnvEFSTable->halnv.tables.ofdmCmdPwrOffset);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.ofdmCmdPwrOffset,inputVoidBuffer,bufferSize);
            }
            break;

        case VNV_TX_BB_FILTER_MODE:
            itemSize = sizeof(gnvEFSTable->halnv.tables.txbbFilterMode);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.txbbFilterMode,inputVoidBuffer,bufferSize);
            }
            break;
            

        case VNV_TABLE_VIRTUAL_RATE:
            itemSize = sizeof(gnvEFSTable->halnv.tables.pwrOptimum_virtualRate);
            if(bufferSize != itemSize) {
                VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                 ("type = %d buffer size=%d is less than data size=%d\r\n"),type, bufferSize,
                  itemSize);
                status = VOS_STATUS_E_INVAL;
            }
            else {
                memcpy(&gnvEFSTable->halnv.tables.pwrOptimum_virtualRate,inputVoidBuffer,bufferSize);
            }
            break;

        default:
          break;
    }
   if (VOS_STATUS_SUCCESS == status)
   {
      // set NV item to have valid data
      status = vos_nv_setValidity( type, VOS_TRUE );
      if (! VOS_IS_STATUS_SUCCESS(status)) {
          VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR, ("vos_nv_setValidity failed!!!\r\n"));
          status = VOS_STATUS_E_FAULT;
      }
      status = wlan_write_to_efs((v_U8_t*)gnvEFSTable,sizeof(nvEFSTable_t));

      if (! VOS_IS_STATUS_SUCCESS(status)) {
          VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR, ("vos_nv_write_to_efs failed!!!\r\n"));
          status = VOS_STATUS_E_FAULT;
      }
   }
   return status;
}

/**------------------------------------------------------------------------
  \brief vos_nv_getChannelListWithPower() - function to return the list of
          supported channels with the power limit info too.
  \param pChannels20MHz - list of 20 Mhz channels
  \param pNum20MHzChannelsFound - number of 20 Mhz channels
  \param pChannels40MHz - list of 20 Mhz channels
  \param pNum40MHzChannelsFound - number of 20 Mhz channels
  \return status of the NV read operation
  \Note: 40Mhz not currently supported
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getChannelListWithPower(tChannelListWithPower *channels20MHz /*[NUM_LEGIT_RF_CHANNELS] */,
                                          tANI_U8 *num20MHzChannelsFound,
                                          tChannelListWithPower *channels40MHz /*[NUM_CHAN_BOND_CHANNELS] */,
                                          tANI_U8 *num40MHzChannelsFound
                                          )
{
    VOS_STATUS status = VOS_STATUS_SUCCESS;
    int i, count;
    
    //TODO: Dont want to use pMac here...can we instead store the curRegDomain in NV
    // or pass it as a parameter to NV from SME?

    if( channels20MHz && num20MHzChannelsFound )
    {
        count = 0;
        for( i = 0; i <= RF_CHAN_14; i++ )
        {
            if( regChannels[i].enabled )
            {
                channels20MHz[count].chanId = rfChannels[i].channelNum;
                channels20MHz[count++].pwr  = regChannels[i].pwrLimit;
            }
        }
        for( i = RF_CHAN_36; i <= RF_CHAN_165; i++ )
        {
            if( regChannels[i].enabled )
            {
                channels20MHz[count].chanId = rfChannels[i].channelNum;
                channels20MHz[count++].pwr  = regChannels[i].pwrLimit;
            }
        }
        *num20MHzChannelsFound = (tANI_U8)count;
    }

    if( channels40MHz && num40MHzChannelsFound )
    {
        count = 0;
        //center channels for 2.4 Ghz 40 MHz channels
        for( i = RF_CHAN_BOND_3; i <= RF_CHAN_BOND_11; i++ )
        {
            
            if( regChannels[i].enabled )
            {
                channels40MHz[count].chanId = rfChannels[i].channelNum;
                channels40MHz[count++].pwr  = regChannels[i].pwrLimit;
            }
        }
        //center channels for 5 Ghz 40 MHz channels
        for( i = RF_CHAN_BOND_38; i <= RF_CHAN_BOND_163; i++ )
        {
            
            if( regChannels[i].enabled )
            {
                channels40MHz[count].chanId = rfChannels[i].channelNum;
                channels40MHz[count++].pwr  = regChannels[i].pwrLimit;
            }
        }
        *num40MHzChannelsFound = (tANI_U8)count;
    }
    return (status);
}

/**------------------------------------------------------------------------
  \brief vos_nv_getDefaultRegDomain() - return the default regulatory domain
  \return default regulatory domain
  \sa
  -------------------------------------------------------------------------*/

v_REGDOMAIN_t vos_nv_getDefaultRegDomain( void )
{
    return countryInfoTable.countryInfo[0].regDomain;
}

/**------------------------------------------------------------------------
  \brief vos_nv_getSupportedChannels() - function to return the list of
          supported channels
  \param p20MhzChannels - list of 20 Mhz channels
  \param pNum20MhzChannels - number of 20 Mhz channels
  \param p40MhzChannels - list of 40 Mhz channels
  \param pNum40MhzChannels - number of 40 Mhz channels
  \return status of the NV read operation
  \Note: 40Mhz not currently supported
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getSupportedChannels( v_U8_t *p20MhzChannels, int *pNum20MhzChannels,
                                        v_U8_t *p40MhzChannels, int *pNum40MhzChannels)
{
    VOS_STATUS status = VOS_STATUS_E_INVAL;
    int i, count = 0;

    if( p20MhzChannels && pNum20MhzChannels )
    {
        if( *pNum20MhzChannels >= NUM_RF_CHANNELS )
        {
            for( i = 0; i <= RF_CHAN_14; i++ )
            {
                p20MhzChannels[count++] = rfChannels[i].channelNum;
            }
            for( i = RF_CHAN_36; i <= RF_CHAN_165; i++ )
            {
                p20MhzChannels[count++] = rfChannels[i].channelNum;
            }
            status = VOS_STATUS_SUCCESS;
        }
        *pNum20MhzChannels = count;
    }

    return (status);
}

/**------------------------------------------------------------------------
  \brief vos_nv_readDefaultCountryTable() - return the default Country table
  \param table data - a union to return the default country table data in.
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_readDefaultCountryTable( uNvTables *tableData )
{
   
   VOS_STATUS status = VOS_STATUS_SUCCESS;
   memcpy(&tableData->defaultCountryTable, &pnvEFSTable->halnv.tables.defaultCountryTable, sizeof(sDefaultCountry));
   pr_info("DefaultCountry is %c%c\n",
            tableData->defaultCountryTable.countryCode[0],
            tableData->defaultCountryTable.countryCode[1]);
   return status;
}

/**------------------------------------------------------------------------
  \brief vos_nv_getBuffer - 
  \param pBuffer  - to return the buffer address
              pSize     - buffer size.
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getNVBuffer(v_VOID_t **pNvBuffer,v_SIZE_t *pSize)
{
   /* Send the NV structure and size */
   *pNvBuffer = (v_VOID_t *)(&pnvEFSTable->halnv);
   *pSize = sizeof(sHalNv);

   return VOS_STATUS_SUCCESS;
}

/**------------------------------------------------------------------------
  \brief vos_nv_getBuffer -
  \param pBuffer  - to return the buffer address
              pSize     - buffer size.
  \return status of the NV read operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_getNVEncodedBuffer(v_VOID_t **pNvBuffer,v_SIZE_t *pSize)
{
   /* Send the NV structure and size */
   VOS_STATUS status;

   status = vos_nv_isEmbeddedNV();

   if (VOS_STATUS_SUCCESS == status)
   {
      *pNvBuffer = (v_VOID_t *)(pEncodedBuf);
      *pSize = nvReadEncodeBufSize;
   }
   else
   {
      *pNvBuffer = (v_VOID_t *)(&pnvEFSTable->halnv);
      *pSize = sizeof(sHalNv);
   }

   return VOS_STATUS_SUCCESS;
}


VOS_STATUS vos_nv_getNVDictionary(v_VOID_t **pNvBuffer,v_SIZE_t *pSize)
{
    /* Send the NV structure and size */
   *pNvBuffer = (v_VOID_t *)(pDictFile);
   *pSize = nDictionarySize;

   return VOS_STATUS_SUCCESS;
}

VOS_STATUS vos_nv_isEmbeddedNV(v_VOID_t)
{
   if (MAGIC_NUMBER == magicNumber)
   {
      return VOS_STATUS_SUCCESS;
   }

   return VOS_STATUS_E_FAILURE;
}

VOS_STATUS vos_nv_setNVEncodedBuffer(v_U8_t *pNvBuffer, v_SIZE_t size)
{
    vos_mem_copy(pEncodedBuf, &pNvBuffer[sizeof(v_U32_t)],
            (size-sizeof(v_U32_t)));

    return VOS_STATUS_SUCCESS;
}

/**------------------------------------------------------------------------
  \brief vos_nv_setRegDomain - 
  \param clientCtxt  - Client Context, Not used for PRIMA
              regId  - Regulatory Domain ID
  \return status set REG domain operation
  \sa
  -------------------------------------------------------------------------*/
VOS_STATUS vos_nv_setRegDomain(void * clientCtxt, v_REGDOMAIN_t regId)
{
    v_CONTEXT_t pVosContext = NULL;
    hdd_context_t *pHddCtx = NULL;
    struct wiphy *wiphy = NULL;
    v_U8_t nBandCapability;
    int i, j, k, m;

   /* Client Context Argumant not used for PRIMA */
   if(regId >= REGDOMAIN_COUNT)
   {
      VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                "VOS set reg domain, invalid REG domain ID %d", regId);
      return VOS_STATUS_E_INVAL;
   }

   /* Set correct channel information based on REG Domain */
   regChannels = pnvEFSTable->halnv.tables.regDomains[regId].channels;

   pVosContext = vos_get_global_context(VOS_MODULE_ID_SYS, NULL);
   if (NULL != pVosContext)
       pHddCtx = vos_get_context(VOS_MODULE_ID_HDD, pVosContext);
   else
       return VOS_STATUS_E_EXISTS;

   if (NULL == pHddCtx)
       return VOS_STATUS_E_EXISTS;

   wiphy = pHddCtx->wiphy;
   nBandCapability = pHddCtx->cfg_ini->nBandCapability;

   for (i = 0, m = 0; i < IEEE80211_NUM_BANDS; i++)
   {

       if (NULL == wiphy->bands[i])
       {
          VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                    "error: wiphy->bands[i] is NULL, i = %d", i);
          continue;
       }

       // internal channels[] is one continous array for both 2G and 5G bands
       // m is internal starting channel index for each band
       if (0 == i)
           m = 0;
       else
           m = wiphy->bands[i-1]?wiphy->bands[i-1]->n_channels + m:m;

       for (j = 0; j < wiphy->bands[i]->n_channels; j++)
       {
           struct ieee80211_supported_band *band = wiphy->bands[i];

           // k = (m + j) is internal current channel index for 20MHz channel
           // n is internal channel index for corresponding 40MHz channel
           k = m + j;
           if (IEEE80211_BAND_2GHZ == i && eCSR_BAND_5G == nBandCapability) // 5G only
           {
               // Enable social channels for P2P
               if (WLAN_HDD_IS_SOCIAL_CHANNEL(band->channels[j].center_freq) &&
                   NV_CHANNEL_ENABLE == regChannels[k].enabled)
                   band->channels[j].flags &= ~IEEE80211_CHAN_DISABLED;
               else
                   band->channels[j].flags |= IEEE80211_CHAN_DISABLED;
               continue;
           }
           else if (IEEE80211_BAND_5GHZ == i && eCSR_BAND_24 == nBandCapability) // 2G only
           {
               band->channels[j].flags |= IEEE80211_CHAN_DISABLED;
               continue;
           }

           if (NV_CHANNEL_DISABLE == regChannels[k].enabled ||
               NV_CHANNEL_INVALID == regChannels[k].enabled)
           {
               band->channels[j].flags |= IEEE80211_CHAN_DISABLED;
           }
           else if (NV_CHANNEL_DFS == regChannels[k].enabled)
           {
               band->channels[j].flags &= ~IEEE80211_CHAN_DISABLED;
               band->channels[j].flags |= IEEE80211_CHAN_RADAR;
           }
           else
           {
               band->channels[j].flags &= ~(IEEE80211_CHAN_DISABLED
                                            |IEEE80211_CHAN_RADAR);
           }
       }
    }

   return VOS_STATUS_SUCCESS;
}

/**------------------------------------------------------------------------
  \brief vos_nv_getChannelEnabledState - 
  \param rfChannel  - input channel enum to know evabled state
  \return eNVChannelEnabledType enabled state for channel
             * enabled
             * disabled
             * DFS
  \sa
  -------------------------------------------------------------------------*/
eNVChannelEnabledType vos_nv_getChannelEnabledState
(
   v_U32_t     rfChannel
)
{
   v_U32_t       channelLoop;
   eRfChannels   channelEnum = INVALID_RF_CHANNEL;

   for(channelLoop = 0; channelLoop <= RF_CHAN_165; channelLoop++)
   {
      if(rfChannels[channelLoop].channelNum == rfChannel)
      {
         channelEnum = (eRfChannels)channelLoop;
         break;
      }
   }

   if(INVALID_RF_CHANNEL == channelEnum)
   {
      VOS_TRACE(VOS_MODULE_ID_VOSS, VOS_TRACE_LEVEL_ERROR,
                "vos_nv_getChannelEnabledState, invalid channel %d", rfChannel);
      return NV_CHANNEL_INVALID;
   }

   return regChannels[channelEnum].enabled;
}

/******************************************************************
 Add CRDA regulatory support
*******************************************************************/

static int bw20_ch_index_to_bw40_ch_index(int k)
{
   int m = -1;
   if (k >= RF_CHAN_1 && k <= RF_CHAN_14)
   {
      m = k - RF_CHAN_1 + RF_CHAN_BOND_3 ;
      if (m > RF_CHAN_BOND_11)
         m = RF_CHAN_BOND_11;
   }
   else if (k >= RF_CHAN_240 && k <= RF_CHAN_216)
   {
      m = k - RF_CHAN_240 + RF_CHAN_BOND_242 ;
      if (m > RF_CHAN_BOND_214)
         m = RF_CHAN_BOND_214;
   }
   else if (k >= RF_CHAN_36 && k <= RF_CHAN_64)
   {
      m = k - RF_CHAN_36 + RF_CHAN_BOND_38;
      if (m > RF_CHAN_BOND_62)
         m = RF_CHAN_BOND_62;
   }
   else if (k >= RF_CHAN_100 && k <= RF_CHAN_140)
   {
      m = k - RF_CHAN_100 + RF_CHAN_BOND_102;
      if (m > RF_CHAN_BOND_138)
         m = RF_CHAN_BOND_138;
   }
   else if (k >= RF_CHAN_149 && k <= RF_CHAN_165)
   {
      m = k - RF_CHAN_149 + RF_CHAN_BOND_151;
      if (m > RF_CHAN_BOND_163)
         m = RF_CHAN_BOND_163;
   }
return m;
}

void crda_regulatory_entry_default(v_U8_t *countryCode, int domain_id)
{
   int k;
   pr_info("Country %c%c domain_id %d\n enable ch 1 - 11.\n",
       countryCode[0], countryCode[1], domain_id);
   for (k = RF_CHAN_1; k <= RF_CHAN_11; k++) {
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].enabled =
           NV_CHANNEL_ENABLE;
       /* Max Tx Power 20dBm */
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].pwrLimit = 20;
   }
   /* enable ch 12 to ch 14 passive scan */
   pr_info(" enable ch 12 - 14 to scan passively by setting DFS flag.\n");
   for (k = RF_CHAN_12; k <= MAX_2_4GHZ_CHANNEL; k++) {
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].enabled =
           NV_CHANNEL_DFS;
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].pwrLimit = 0;
   }
   pr_info(" enable 5GHz to scan passively by setting DFS flag.\n");
   for (k = MIN_5GHZ_CHANNEL; k <= MAX_5GHZ_CHANNEL; k++) {
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].enabled =
           NV_CHANNEL_DFS;
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].pwrLimit = 0;
   }
#ifdef PASSIVE_SCAN_4_9GHZ
   pr_info(" enable 4.9 GHz to scan passively by setting DFS flag.\n");
   for (k = RF_CHAN_240; k <= RF_CHAN_216; k++) {
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].enabled =
           NV_CHANNEL_DFS;
       pnvEFSTable->halnv.tables.regDomains[domain_id].channels[k].pwrLimit = 0;
   }
#endif
  if (domain_id == NUM_REG_DOMAINS-1)
  { /* init time */
     crda_alpha2[0] = countryCode[0];
     crda_alpha2[1] = countryCode[1];
     crda_regulatory_entry_valid = VOS_TRUE;
     pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[0] = countryCode[0];
     pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[1] = countryCode[1];
     pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[2] = 'I';
     pnvEFSTable->halnv.tables.defaultCountryTable.regDomain = NUM_REG_DOMAINS-1;
  }
  if (domain_id == NUM_REG_DOMAINS-2)
  { /* none-default country */
     run_time_alpha2[0] = countryCode[0];
     run_time_alpha2[1] = countryCode[1];
     crda_regulatory_run_time_entry_valid = VOS_TRUE;
  }
}

static int crda_regulatory_entry_post_processing(struct wiphy *wiphy,
                struct regulatory_request *request,
                v_U8_t nBandCapability,
                int domain_id)
{
   if (request->alpha2[0] == '0' && request->alpha2[1] == '0') {
        pr_info("Country 00 special handling to enable passive scan.\n");
        crda_regulatory_entry_default(request->alpha2, domain_id);
   }
   return 0;
}

/* create_crda_regulatory_entry should be called from user command or 11d country IE */
static int create_crda_regulatory_entry(struct wiphy *wiphy,
                struct regulatory_request *request,
                v_U8_t nBandCapability)
{
   int i, j, m;
   int k = 0, n = 0;

   if (run_time_alpha2[0]==request->alpha2[0] &&
        run_time_alpha2[1]==request->alpha2[1] &&
        crda_regulatory_run_time_entry_valid == VOS_TRUE)
        return 0; /* already created */

   /* 20MHz channels */
   if (nBandCapability == eCSR_BAND_24)
       pr_info("BandCapability is set to 2G only.\n");
   for (i=0,m=0;i<IEEE80211_NUM_BANDS;i++)
   {
       if (i == IEEE80211_BAND_2GHZ && nBandCapability == eCSR_BAND_5G) // 5G only
          continue;
       else if (i == IEEE80211_BAND_5GHZ && nBandCapability == eCSR_BAND_24) // 2G only
          continue;
       if (wiphy->bands[i] == NULL)
       {
          pr_info("error: wiphy->bands[i] is NULL, i = %d\n", i);
          return -1;
       }
       // internal channels[] is one continous array for both 2G and 5G bands
       // m is internal starting channel index for each band
       if (i == 0)
           m = 0;
       else
           m = wiphy->bands[i-1]->n_channels + m;
       for (j=0;j<wiphy->bands[i]->n_channels;j++)
       {
           // k = (m + j) is internal current channel index for 20MHz channel
           // n is internal channel index for corresponding 40MHz channel
           k = m + j;
           n = bw20_ch_index_to_bw40_ch_index(k);
           if (n == -1)
              return -1;
           if (wiphy->bands[i]->channels[j].flags & IEEE80211_CHAN_DISABLED)
           {
              if (pnvEFSTable == NULL)
              {
                 pr_info("error: pnvEFSTable is NULL, probably not parsed nv.bin yet\n");
                 return -1;
              }
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[k].enabled =
                 NV_CHANNEL_DISABLE;
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[n].enabled =
                 NV_CHANNEL_DISABLE;
              //pr_info("CH %d disabled, no bonding centered on CH %d.\n", rfChannels[k].channelNum,
              //    rfChannels[n].channelNum);
           }
           else if (wiphy->bands[i]->channels[j].flags & IEEE80211_CHAN_RADAR)
           {
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[k].enabled =
                 NV_CHANNEL_DFS;
              // max_power is in mBm = 100 * dBm
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[k].pwrLimit =
                 (tANI_S8) ((wiphy->bands[i]->channels[j].max_power)/100);
              if ((wiphy->bands[i]->channels[j].flags & IEEE80211_CHAN_NO_HT40) == 0)
              {
                 pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[n].enabled =
                    NV_CHANNEL_DFS;
                 // 40MHz channel power is half of 20MHz (-3dB) ??
                 pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[n].pwrLimit =
                    (tANI_S8) (((wiphy->bands[i]->channels[j].max_power)/100)-3);
              }
           }
           else // Enable is only last flag we support
           {
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[k].enabled =
                 NV_CHANNEL_ENABLE;
              // max_power is in dBm
              pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[k].pwrLimit =
                 (tANI_S8) ((wiphy->bands[i]->channels[j].max_power)/100);
              if ((wiphy->bands[i]->channels[j].flags & IEEE80211_CHAN_NO_HT40) == 0)
              {
                 pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[n].enabled =
                    NV_CHANNEL_ENABLE;
                 // 40MHz channel power is half of 20MHz (-3dB) ??
                 pnvEFSTable->halnv.tables.regDomains[NUM_REG_DOMAINS-2].channels[n].pwrLimit =
                    (tANI_S8) (((wiphy->bands[i]->channels[j].max_power)/100)-3);
              }
           }
           /* ignore CRDA max_antenna_gain typical is 3dBi, nv.bin antennaGain is
           real gain which should be provided by the real design */
       }
   }
   if (k == 0)
       return -1;
   run_time_alpha2[0] = request->alpha2[0];
   run_time_alpha2[1] = request->alpha2[1];
   crda_regulatory_run_time_entry_valid = VOS_TRUE;
   crda_regulatory_entry_post_processing(wiphy, request, nBandCapability, NUM_REG_DOMAINS-2);
return 0;
}
v_BOOL_t is_crda_regulatory_entry_valid(void)
{
return crda_regulatory_entry_valid;
}

/* Handling routines for the conversion from regd rules (start/end freq) to channel index
start freq + 10000 = center freq of the 20MHz start channel
end freq - 10000 = center freq of the 20MHz end channel
start freq + 20000 = center freq of the 40MHz start channel
end freq - 20000 = center freq of the 40MHz end channel
*/
static int bw20_start_freq_to_channel_index(u32 freq_khz)
{
int i;
u32 center_freq = freq_khz + 10000;
  //Has to compare from low freq to high freq
  //RF_SUBBAND_2_4_GHZ
  for (i=RF_CHAN_1;i<=RF_CHAN_14;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_4_9_GHZ, Ch 240, 244, 248, 252, 208, 212, 216
  for (i=RF_CHAN_240;i<=RF_CHAN_216;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_LOW_GHZ
  for (i=RF_CHAN_36;i<=RF_CHAN_64;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_MID_GHZ
  for (i=RF_CHAN_100;i<=RF_CHAN_140;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_HIGH_GHZ
  for (i=RF_CHAN_149;i<=RF_CHAN_165;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
return -1;
}

static int bw20_end_freq_to_channel_index(u32 freq_khz)
{
int i;
u32 center_freq = freq_khz - 10000;
  //Has to compare from high freq to low freq
  //RF_SUBBAND_5_HIGH_GHZ
  for (i=RF_CHAN_165;i>=RF_CHAN_149;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_MID_GHZ
  for (i=RF_CHAN_140;i>=RF_CHAN_100;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_LOW_GHZ
  for (i=RF_CHAN_64;i>=RF_CHAN_36;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_4_9_GHZ, Ch 216, 212, 208, 252, 248, 244, 240
  for (i=RF_CHAN_216;i>=RF_CHAN_240;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_2_4_GHZ
  for (i=RF_CHAN_14;i>=RF_CHAN_1;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
return -1;
}

static int bw40_start_freq_to_channel_index(u32 freq_khz)
{
int i;
u32 center_freq = freq_khz + 20000;
  //Has to compare from low freq to high freq
  //RF_SUBBAND_2_4_GHZ
  for (i=RF_CHAN_BOND_3;i<=RF_CHAN_BOND_11;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_4_9_GHZ, Ch 242, 246, 250, 210, 214
  for (i=RF_CHAN_BOND_242;i<=RF_CHAN_BOND_214;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_LOW_GHZ
  for (i=RF_CHAN_BOND_38;i<=RF_CHAN_BOND_62;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_MID_GHZ
  for (i=RF_CHAN_BOND_102;i<=RF_CHAN_BOND_138;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_HIGH_GHZ
  for (i=RF_CHAN_BOND_151;i<=RF_CHAN_BOND_163;i++)
    if (center_freq <= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
return -1;
}

static int bw40_end_freq_to_channel_index(u32 freq_khz)
{
int i;
u32 center_freq = freq_khz - 20000;
  //Has to compare from high freq to low freq
  //RF_SUBBAND_5_HIGH_GHZ
  for (i=RF_CHAN_BOND_163;i>=RF_CHAN_BOND_151;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_MID_GHZ
  for (i=RF_CHAN_BOND_138;i>=RF_CHAN_BOND_102;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_5_LOW_GHZ
  for (i=RF_CHAN_BOND_62;i>=RF_CHAN_BOND_38;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_4_9_GHZ, Ch 214, 210, 250, 246, 242
  for (i=RF_CHAN_BOND_214;i>=RF_CHAN_BOND_242;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
  //RF_SUBBAND_2_4_GHZ
  for (i=RF_CHAN_BOND_11;i>=RF_CHAN_BOND_3;i--)
    if (center_freq >= (u32) (rfChannels[i].targetFreq) * 1000)
      return i;
return -1;
}

static v_BOOL_t channel_in_capable_band(int j, v_U8_t nBandCapability)
{
   switch (nBandCapability)
   {
      case eCSR_BAND_ALL:
           return VOS_TRUE;
      case eCSR_BAND_24:
           if (j >= RF_CHAN_1 && j <= RF_CHAN_14)
              return VOS_TRUE;
           if (j >= RF_CHAN_BOND_3 && j <= RF_CHAN_BOND_11)
              return VOS_TRUE; // 2.4G 40MHz channel
           break;
      case eCSR_BAND_5G:
           if (j >= RF_CHAN_240 && j <= RF_CHAN_165)
              return VOS_TRUE;
           if (j >= RF_CHAN_BOND_242 && j <= RF_CHAN_BOND_163)
              return VOS_TRUE; // 2.4G 40MHz channel
           break;
      default:
           break;
   }
   return VOS_FALSE;
}

/* create_crda_regulatory_entry_from_regd should be called during init time */
static int create_crda_regulatory_entry_from_regd(struct wiphy *wiphy,
                struct regulatory_request *request,
                v_U8_t nBandCapability)
{
   int i, j, n, domain_id;
   int bw20_start_channel_index, bw20_end_channel_index;
   int bw40_start_channel_index, bw40_end_channel_index;

   if (wiphy == NULL || wiphy->regd == NULL)
   {
      wiphy_dbg(wiphy, "error: wiphy->regd is NULL\n");
      return -1;
   }
   if (crda_regulatory_entry_valid == VOS_FALSE)
      domain_id = NUM_REG_DOMAINS-1; /* init time */
   else
      domain_id = NUM_REG_DOMAINS-2; /* none-default country */
   for (n = 0; n < NUM_RF_CHANNELS; n++)
      pnvEFSTable->halnv.tables.regDomains[domain_id].channels[n].enabled = NV_CHANNEL_DISABLE;

   for (i=0;i<wiphy->regd->n_reg_rules;i++)
   {
      wiphy_dbg(wiphy, "info: crda rule %d --------------------------------------------\n", i);
      bw20_start_channel_index =
         bw20_start_freq_to_channel_index(wiphy->regd->reg_rules[i].freq_range.start_freq_khz);
      bw20_end_channel_index =
      bw20_end_freq_to_channel_index(wiphy->regd->reg_rules[i].freq_range.end_freq_khz);
      if (bw20_start_channel_index == -1 || bw20_end_channel_index == -1)
      {
         wiphy_dbg(wiphy, "error: crda freq not supported, start freq (KHz) %d end freq %d\n",
          wiphy->regd->reg_rules[i].freq_range.start_freq_khz,
             wiphy->regd->reg_rules[i].freq_range.end_freq_khz);
         continue; // skip this rull, but continue to next rule
      }
      wiphy_dbg(wiphy, "20MHz start freq (KHz) %d end freq %d start ch index %d end ch index %d\n",
         wiphy->regd->reg_rules[i].freq_range.start_freq_khz,
          wiphy->regd->reg_rules[i].freq_range.end_freq_khz,
              bw20_start_channel_index, bw20_end_channel_index);
      for (j=bw20_start_channel_index;j<=bw20_end_channel_index;j++)
      {
         if (channel_in_capable_band(j, nBandCapability) == VOS_FALSE)
         {
             wiphy_dbg(wiphy, "info: CH %d is not in capable band\n",
                 rfChannels[j].channelNum);
             continue; // skip  this channel, continue to next
         }
         if (wiphy->regd->reg_rules[i].flags & NL80211_RRF_DFS)
         {
             pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].enabled = NV_CHANNEL_DFS;
             wiphy_dbg(wiphy, "info: CH %d is DFS, max EIRP (mBm) is %d\n", rfChannels[j].channelNum,
                wiphy->regd->reg_rules[i].power_rule.max_eirp);
         }
         else
         {
             pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].enabled = NV_CHANNEL_ENABLE;
             wiphy_dbg(wiphy, "info: CH %d is enabled, no DFS, max EIRP (mBm) is %d\n", rfChannels[j].channelNum,
                 wiphy->regd->reg_rules[i].power_rule.max_eirp);
         }
         /* max_eirp is in mBm (= 100 * dBm) unit */
         pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].pwrLimit =
            (tANI_S8) ((wiphy->regd->reg_rules[i].power_rule.max_eirp)/100);
      }
      /* ignore CRDA max_antenna_gain typical is 3dBi, nv.bin antennaGain is
         real gain which should be provided by the real design */
      if (wiphy->regd->reg_rules[i].freq_range.max_bandwidth_khz == 40000)
      {
         wiphy_dbg(wiphy, "info: 40MHz (channel bonding) is allowed\n");
         bw40_start_channel_index =
            bw40_start_freq_to_channel_index(wiphy->regd->reg_rules[i].freq_range.start_freq_khz);
         bw40_end_channel_index =
            bw40_end_freq_to_channel_index(wiphy->regd->reg_rules[i].freq_range.end_freq_khz);
         if (bw40_start_channel_index == -1 || bw40_end_channel_index == -1)
         {
            wiphy_dbg(wiphy, "error: crda freq not supported, start_freq_khz %d end_freq_khz %d\n",
                wiphy->regd->reg_rules[i].freq_range.start_freq_khz,
                   wiphy->regd->reg_rules[i].freq_range.end_freq_khz);
            continue; // skip this rull, but continue to next rule
         }
         wiphy_dbg(wiphy, "40MHz start freq (KHz) %d end freq %d start ch index %d end ch index %d\n",
            wiphy->regd->reg_rules[i].freq_range.start_freq_khz,
                wiphy->regd->reg_rules[i].freq_range.end_freq_khz,
                   bw40_start_channel_index, bw40_end_channel_index);
         for (j=bw40_start_channel_index;j<=bw40_end_channel_index;j++)
         {
            if (channel_in_capable_band(j, nBandCapability) == VOS_FALSE)
                continue; // skip  this channel, continue to next
            if (wiphy->regd->reg_rules[i].flags & NL80211_RRF_DFS)
            {
                pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].enabled = NV_CHANNEL_DFS;
                wiphy_dbg(wiphy, "info: 40MHz centered on CH %d is DFS\n", rfChannels[j].channelNum);
            }
            else
            {
                pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].enabled = NV_CHANNEL_ENABLE;
                wiphy_dbg(wiphy, "info: 40MHz centered on CH %d is enabled, no DFS\n", rfChannels[j].channelNum);
            }
            /* set 40MHz channel power as half (- 3 dB) of 20MHz */
            pnvEFSTable->halnv.tables.regDomains[domain_id].channels[j].pwrLimit =
                (tANI_S8) (((wiphy->regd->reg_rules[i].power_rule.max_eirp)/100)-3);
         }
      }
  }
  /* ToDo update other (than DFS) crda regulatory flags (NO_OUTDOOR,
     NO_OFDM, PASSIVE_SCAN, NO_IBSS) to pnvEFSTable which doesn't add
     these flags and has no implementation yet. */
  if (crda_regulatory_entry_valid == VOS_FALSE)
  { /* init time */
     crda_alpha2[0] = request->alpha2[0];
     crda_alpha2[1] = request->alpha2[1];
     crda_regulatory_entry_valid = VOS_TRUE;
  }
  else
  { /* none-default country */
     run_time_alpha2[0] = request->alpha2[0];
     run_time_alpha2[1] = request->alpha2[1];
     crda_regulatory_run_time_entry_valid = VOS_TRUE;
  }
  crda_regulatory_entry_post_processing(wiphy, request, nBandCapability, domain_id);
  return 0;
}

/*
 * Function: wlan_hdd_crda_reg_notifier
 * This function is called from cfg80211 core to provide regulatory settings
 * after new country is requested or intersected (init, user input or 11d)
 * This function is used to create a CRDA regulatory settings entry into internal
 * regulatory setting table.
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
void wlan_hdd_crda_reg_notifier(struct wiphy *wiphy,
                struct regulatory_request *request)
#else
int wlan_hdd_crda_reg_notifier(struct wiphy *wiphy,
                struct regulatory_request *request)
#endif
{
    hdd_context_t *pHddCtx = wiphy_priv(wiphy);
    wiphy_dbg(wiphy, "info: cfg80211 reg_notifier callback for country"
                     " %c%c\n", request->alpha2[0], request->alpha2[1]);
    if (request->initiator == NL80211_REGDOM_SET_BY_USER)
    {
       wiphy_dbg(wiphy, "info: set by user\n");
       if (create_crda_regulatory_entry(wiphy, request, pHddCtx->cfg_ini->nBandCapability) != 0)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
          return;
#else
          return 0;
#endif
       // ToDo
       /* Don't change default country code to CRDA country code by user req */
       /* Shouldcall sme_ChangeCountryCode to send a message to trigger read
          regd for new country settings */
       //sme_ChangeCountryCode(pHddCtx->hHal, NULL,
       //    &country_code[0], pAdapter, pHddCtx->pvosContext);
    }
    else if (request->initiator == NL80211_REGDOM_SET_BY_COUNTRY_IE)
    {
       wiphy_dbg(wiphy, "info: set by country IE\n");
       if (create_crda_regulatory_entry(wiphy, request, pHddCtx->cfg_ini->nBandCapability) != 0)
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
          return;
#else
          return 0;
#endif
       // ToDo
       /* Intersect of 11d and crda settings */

       /* Don't change default country code to CRDA country code by 11d req */
       /* for every adapter call sme_ChangeCountryCode to trigger read regd
          for intersected new country settings */
       // sme_ChangeCountryCode(pHddCtx->hHal, NULL,
       //    &country_code[0], pAdapter, pHddCtx->pvosContext);
    }
    else if (request->initiator == NL80211_REGDOM_SET_BY_DRIVER)
    {
       wiphy_dbg(wiphy, "info: set by driver\n");
       /* if set by driver itself, it means driver can accept the crda
          regulatory settings and wiphy->regd should be populated with crda
          settings. iwiphy->bands doesn't seem to set ht40 flags in kernel
          correctly, this may be fixed by later kernel */
       if (create_crda_regulatory_entry_from_regd(wiphy, request, pHddCtx->cfg_ini->nBandCapability) == 0)
       {
          pr_info("crda entry created.\n");
          if (crda_alpha2[0] == request->alpha2[0] && crda_alpha2[1] == request->alpha2[1])
          {  /* first CRDA request should be from init time */
             /* Change default country code to CRDA country code, assume indoor */
             pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[0] = request->alpha2[0];
             pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[1] = request->alpha2[1];
             pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[2] = 'I';
             pnvEFSTable->halnv.tables.defaultCountryTable.regDomain = NUM_REG_DOMAINS-1;
             wiphy_dbg(wiphy, "info: init time default country code is %c%c%c\n",
                pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[0],
                   pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[1],
                      pnvEFSTable->halnv.tables.defaultCountryTable.countryCode[2]);
          }
          else /* second or later CRDA request after init time */
          {
             wiphy_dbg(wiphy, "info: crda none-default country code is %c%c\n",
                request->alpha2[0], request->alpha2[1]);
          }
          // hdd will read regd for this country after complete
       }
       complete(&pHddCtx->driver_crda_req);

       /* Haven't seen any condition that will set by driver after init.
          If we do, then we should also call sme_ChangeCountryCode */
    }
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0))
    return;
#else
    return 0;
#endif
}
