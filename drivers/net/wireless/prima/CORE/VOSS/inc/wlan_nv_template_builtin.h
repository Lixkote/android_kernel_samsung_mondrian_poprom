/*
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name of The Linux Foundation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 *WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 *ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#if !defined _WLAN_NV_TEMPLATE_BUILTIN_H
#define _WLAN_NV_TEMPLATE_BUILTIN_H


/*===========================================================================
                       EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header:$ $DateTime: $ $Author: $

  when        who        what, where, why
  --------    ---        -----------------------------------------------
  04/10/13    kumarpra   nv built in  creation
===========================================================================*/


/*
  -----------------------------------------
  This file is generated by nvTmplRun Parser
  -----------------------------------------
  To be auto-generated, or
  This is the source file and the header file can be generated from this
  template file.

  Template constructs
     1. TABLE_: struct
     2. INDEX_ENUM: enums, e.g. {RATE_OFDM_6M,RATE_OFDM_54M}
     3. INDEX_INT: int, e.g.{min, max, increment}
     3. Basic data types: tANI_U8, tANI_S8, tANI_U32, tANI_S32
     4. Storage types:
        4.1 SINGULAR: one element of basic data type
        4.2 ARRAY_1: one dimensional array, x-axis
        4.3 ARRAY_2: two dimensional array, (x, y)
        4.4 ARRAY_3: three dimensional array, (x, y, z)
        4.5 ARRAY_4: four dimensional array, (x, y, z, t)

  Implementation notes
     1. Flow of changing NV data format: (TBD) Either change the template and
        generate the header file, or modify header file and auto-generate
        the template.
     2. Flow of writing NV data: encode the template in the data stream, so the
        NV data is "self-sufficient". No separate template, no compability
        issue, no need of version control.
     3. Flow of reading NV data: parse the binary NV data stream based on the
        template info in the data stream.
     4. The above NV logic is decoupled from the actual data content, a generic,
        content ergonostic parser (reading) and encoder (writing).
        The NV logic is common code shared by tools, s/w
        (both host and firmware), and off-line utilities.
     5. NV data parsing and "acceptanace" into an s/w moduel data structure can
        be "configured" in several ways:
        5.1 only total matching of all fields, otherwise, reject the whole data
        stream (a table).
        5.2 partial matching of fields allowed and the rest fields assume
        reasonal default values,
        The choice can be determined later, but the capability is provided.
     6. We could also design in this selection on an individua table base.
        To design such capability, reserve some header bits in the data stream.
     7. The NV data streams can be modified, replaced, or intact with a new data
        stream of the same table ID added to NV data.
        The choice can be determined later, but the NV scheme provides such
        capability.
     8. The template construct definitions can be common to all tables
        (tbd: in a common section) or table specific, or updated in a
        subsequent format section.
        The use cases are:
        - An index enum (e.g. RF channels) is common to all tables when the NV
          data is created. Later new enums are added (e.g.
        additional channels), one can choose to add the new index enum for new
        tables appended to the NV data, or replace the
        old table with new template info and data.
        The template precedence is table specific then common, and later
        "common" overwrites "earlier" commmon.
        - A new field is added to the table, the user decides to replace the
          old table data, he can simply encode the template info in the
          data stream.
        - In the same scenario (a new field is added), the user decides to
          append a new table, he can encode the template
        in the new data table and append it to NV data, or write a new common
        template section and append the data.

  Key "ingredients", (re-iterate the most important features and capabilities)
     1. How to parse the data is embedded in the NV data itself. It removes the
        dependency on header file matching,
        version checking, compatibility among tools, host and firmware.
     2. Table field ID enables "partial" data acceptance in an s/w module data
        structure. Whether full matching or reject the whole table, or "partial"
        acceptance, the capabiilty is in place and further ensures the robust
        NV data extensibility and compatibility.
     3. The table granularity, data stream based NV data has variable length
        and flexibility of modifying an existing table data, replacing the
        whole data, or leaving the existing data table intact and appending
        a new table.
  Misc notes:
     1. For endianness, support only 4 bytes integer or 4 1-byte
     2. String identifier needs to be shortened to save storage
     3. string_field_name,  field type,  field storage class,  storage size
*/


#include "wlan_nv_types.h"

typedef enum _nvFieldSizeEnums {
   INDEX_ENUM_ALL = 0,
   INDEX_ENUM_NUM_PHY_MAX_TX_CHAINS,
   INDEX_ENUM_NUM_REG_DOMAINS,
   INDEX_ENUM_NUM_RF_SUBBANDS,
   INDEX_ENUM_NUM_RF_CHANNELS,
   INDEX_ENUM_NUM_2_4GHZ_CHANNELS,
   INDEX_ENUM_NUM_802_11_MODES,
   INDEX_ENUM_NUM_HAL_PHY_RATES,
   INDEX_ENUM_BUILTIN_LAST,
   INDEX_ENUM_BUILTIN_MAX,
} _NV_FIELD_SIZE_ENUMS;

#define INDEX_ENUM_MAX  MAX(INDEX_ENUM_PREDEFINED_MAX, INDEX_ENUM_BUILTIN_MAX)

int getEnumNoOfFields(int enumIdx);
extern _NV_TEMPLATE_ENUM NvEnumsBuiltIn[/*INDEX_ENUM_MAX*/][ENUM_ENTRIES_MAX];

typedef enum _nvTableIDs {
   TABLE_sHalNv,
   TABLE_sNvFields,
   TABLE_sRegulatoryChannel,
   TABLE_sRssiChannelOffsets,
   TABLE_sCalData,
   TABLE_sTxBbFilterMode,
   TABLE_sOfdmCmdPwrOffset,
   TABLE_sDefaultCountry,
   TABLE_sFwConfig,
   TABLE_tTpcPowerTable,
   TABLE_tRateGroupPwr,
   TABLE_tRateGroupPwrVR,
   TABLE_sRegulatoryDomains,
   TABLE_sHwCalValues,
   TABLE_sNvTables,
   TABLE_BUILTIN_LAST,
   TABLE_BUILTIN_MAX,
} _NV_TABLE_ID;

#define TABLES_MAX    MAX(TABLE_PREDEFINED_MAX, TABLE_BUILTIN_MAX)

int getTableNoOfFields(int tblIdx);
extern _NV_TEMPLATE_TABLE NvTablesBuiltIn[/*TABLES_MAX*/][TABLE_ENTRIES_MAX];

#endif //#if !defined(_WLAN_NV_TEMPLATEBUILTIN_H)
