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
/*==========================================================================
 *
 *  @file:     wlan_hal_cfg.h
 *
 *  @brief:    Exports and types WLAN HAL configuration
 *
 *  @author:   Kumar Anand
 *
 *             Copyright (C) 2010, Qualcomm Technologies, Inc. 
 *             All rights reserved.
 *
 *=========================================================================*/

#ifndef __WLAN_HAL_CFG_H__
#define __WLAN_HAL_CFG_H__

/*-------------------------------------------------------------------------
  Include Files
-------------------------------------------------------------------------*/
#include "qwlanfw_defs.h"

/*-------------------------------------------------------------------------
  Preprocessor definitions and constants
-------------------------------------------------------------------------*/
/* RIVA API version in format W.X.Y.Z is converted to a UINT32 integer */
#define WLAN_HAL_CONSTRUCT_API_VERSION(W,X,Y,Z) (((W)<<24)+((X)<<16)+((Y)<<8)+(Z))
#define IS_VERSION_BEFORE_VOWIFI(VER_FROM_HOST) \
                 ((VER_FROM_HOST)<=WLAN_HAL_CONSTRUCT_API_VERSION(0,0,2,0))?1:0

/*-------------------------------------------------------------------------
  Configuration Parameter IDs
-------------------------------------------------------------------------*/
#define QWLAN_HAL_CFG_STA_ID                             0
#define QWLAN_HAL_CFG_CURRENT_TX_ANTENNA                 1
#define QWLAN_HAL_CFG_CURRENT_RX_ANTENNA                 2
#define QWLAN_HAL_CFG_LOW_GAIN_OVERRIDE                  3
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN              4
#define QWLAN_HAL_CFG_CAL_PERIOD                         5
#define QWLAN_HAL_CFG_CAL_CONTROL                        6
#define QWLAN_HAL_CFG_PROXIMITY                          7
#define QWLAN_HAL_CFG_NETWORK_DENSITY                    8
#define QWLAN_HAL_CFG_MAX_MEDIUM_TIME                    9
#define QWLAN_HAL_CFG_MAX_MPDUS_IN_AMPDU                 10
#define QWLAN_HAL_CFG_RTS_THRESHOLD                      11
#define QWLAN_HAL_CFG_SHORT_RETRY_LIMIT                  12
#define QWLAN_HAL_CFG_LONG_RETRY_LIMIT                   13
#define QWLAN_HAL_CFG_FRAGMENTATION_THRESHOLD            14
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ZERO             15
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ONE              16
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_TWO              17
#define QWLAN_HAL_CFG_FIXED_RATE                         18
#define QWLAN_HAL_CFG_RETRYRATE_POLICY                   19
#define QWLAN_HAL_CFG_RETRYRATE_SECONDARY                20
#define QWLAN_HAL_CFG_RETRYRATE_TERTIARY                 21
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION            22
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_24GHZ         23
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_5GHZ          24
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_24GHZ           25
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_5GHZ            26
#define QWLAN_HAL_CFG_MAX_BA_SESSIONS                    27
#define QWLAN_HAL_CFG_PS_DATA_INACTIVITY_TIMEOUT         28
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_FILTER               29
#define QWLAN_HAL_CFG_PS_ENABLE_RSSI_MONITOR             30
#define QWLAN_HAL_CFG_NUM_BEACON_PER_RSSI_AVERAGE        31
#define QWLAN_HAL_CFG_STATS_PERIOD                       32
#define QWLAN_HAL_CFG_CFP_MAX_DURATION                   33
#define QWLAN_HAL_CFG_FRAME_TRANS_ENABLED                34
#define QWLAN_HAL_CFG_DTIM_PERIOD                        35
#define QWLAN_HAL_CFG_EDCA_WMM_ACBK                      36
#define QWLAN_HAL_CFG_EDCA_WMM_ACBE                      37
#define QWLAN_HAL_CFG_EDCA_WMM_ACVO                      38
#define QWLAN_HAL_CFG_EDCA_WMM_ACVI                      39
#define QWLAN_HAL_CFG_BA_THRESHOLD_HIGH                  40
#define QWLAN_HAL_CFG_MAX_BA_BUFFERS                     41
#define QWLAN_HAL_CFG_RPE_POLLING_THRESHOLD              42
#define QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC0_REG    43
#define QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC1_REG    44
#define QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC2_REG    45
#define QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC3_REG    46
#define QWLAN_HAL_CFG_NO_OF_ONCHIP_REORDER_SESSIONS      47
#define QWLAN_HAL_CFG_PS_LISTEN_INTERVAL                 48
#define QWLAN_HAL_CFG_PS_HEART_BEAT_THRESHOLD            49
#define QWLAN_HAL_CFG_PS_NTH_BEACON_FILTER               50
#define QWLAN_HAL_CFG_PS_MAX_PS_POLL                     51
#define QWLAN_HAL_CFG_PS_MIN_RSSI_THRESHOLD              52
#define QWLAN_HAL_CFG_PS_RSSI_FILTER_PERIOD              53
#define QWLAN_HAL_CFG_PS_BROADCAST_FRAME_FILTER_ENABLE   54
#define QWLAN_HAL_CFG_PS_IGNORE_DTIM                     55
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_EARLY_TERM           56
#define QWLAN_HAL_CFG_DYNAMIC_PS_POLL_VALUE              57
#define QWLAN_HAL_CFG_PS_NULLDATA_AP_RESP_TIMEOUT        58
#define QWLAN_HAL_CFG_TELE_BCN_WAKEUP_EN                 59
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI                  60
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_IDLE_BCNS        61
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI                    62
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_IDLE_BCNS          63
#define QWLAN_HAL_CFG_TX_PWR_CTRL_ENABLE                 64
#define QWLAN_HAL_CFG_VALID_RADAR_CHANNEL_LIST           65
#define QWLAN_HAL_CFG_TX_POWER_24_20                     66
#define QWLAN_HAL_CFG_TX_POWER_24_40                     67
#define QWLAN_HAL_CFG_TX_POWER_50_20                     68
#define QWLAN_HAL_CFG_TX_POWER_50_40                     69
#define QWLAN_HAL_CFG_MCAST_BCAST_FILTER_SETTING         70
#define QWLAN_HAL_CFG_BCN_EARLY_TERM_WAKEUP_INTERVAL     71
#define QCOM_WLAN_CFG_MAX_TX_POWER_2_4                   72
#define QCOM_WLAN_CFG_MAX_TX_POWER_5                     73
#define QWLAN_HAL_CFG_INFRA_STA_KEEP_ALIVE_PERIOD        74
#define QWLAN_HAL_CFG_ENABLE_CLOSE_LOOP                  75
#define QWLAN_HAL_CFG_BTC_EXECUTION_MODE                 76
#define QWLAN_HAL_CFG_BTC_DHCP_BT_SLOTS_TO_BLOCK         77
#define QWLAN_HAL_CFG_BTC_A2DP_DHCP_BT_SUB_INTERVALS     78
#define QWLAN_HAL_CFG_PS_TX_INACTIVITY_TIMEOUT           79
#define QWLAN_HAL_CFG_WCNSS_API_VERSION                  80
#define QWLAN_HAL_CFG_AP_KEEPALIVE_TIMEOUT               81
#define QWLAN_HAL_CFG_GO_KEEPALIVE_TIMEOUT               82
#define QWLAN_HAL_CFG_ENABLE_MC_ADDR_LIST                83
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_BT              84
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_BT             85
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_BT             86
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_BT               87
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_WLAN            88
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_WLAN           89
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_WLAN           90
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_WLAN             91
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_BT                 92
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_WLAN               93
#define QWLAN_HAL_CFG_BTC_MAX_SCO_BLOCK_PERC             94
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_A2DP              95
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_SCO               96
#define QWLAN_HAL_CFG_ENABLE_UNICAST_FILTER              97
#define QWLAN_HAL_CFG_MAX_ASSOC_LIMIT                    98
#define QWLAN_HAL_CFG_ENABLE_LPWR_IMG_TRANSITION         99
#define QWLAN_HAL_CFG_ENABLE_MCC_ADAPTIVE_SCHEDULER     100
#define QWLAN_HAL_CFG_ENABLE_DETECT_PS_SUPPORT          101
#define QWLAN_HAL_CFG_AP_LINK_MONITOR_TIMEOUT           102
#define QWLAN_HAL_CFG_BTC_DWELL_TIME_MULTIPLIER         103
#define QWLAN_HAL_CFG_ENABLE_TDLS_OXYGEN_MODE           104
#define QWLAN_HAL_CFG_ENABLE_NAT_KEEP_ALIVE_FILTER      105
#define QWLAN_HAL_CFG_ENABLE_SAP_OBSS_PROT              106
#define QWLAN_HAL_CFG_PSPOLL_DATA_RECEP_TIMEOUT         107
#define QWLAN_HAL_CFG_TDLS_PUAPSD_BUFFER_STA_CAPABLE    108
#define QWLAN_HAL_CFG_TDLS_PUAPSD_MASK                  109
#define QWLAN_HAL_CFG_TDLS_PUAPSD_INACTIVITY_TIME       110
#define QWLAN_HAL_CFG_TDLS_PUAPSD_RX_FRAME_THRESHOLD_IN_SP 111
#define QWLAN_HAL_CFG_ANTENNA_DIVERSITY                 112
#define QWLAN_HAL_CFG_ATH_DISABLE                       113
#define QWLAN_HAL_CFG_FLEXCONNECT_POWER_FACTOR          114
#define QWLAN_HAL_CFG_ENABLE_ADAPTIVE_RX_DRAIN_FEATURE  115
#define QWLAN_HAL_CFG_GO_LINK_MONITOR_TIMEOUT           116
#define QWLAN_HAL_CFG_MAX_PARAMS                        117




/* Total number of Integer CFGs. This is used while allocating the memory for TLV */
#define QWLAN_HAL_CFG_INTEGER_PARAM                      117


/*-------------------------------------------------------------------------
  Configuration Parameter min, max, defaults
-------------------------------------------------------------------------*/

/* QWLAN_HAL_CFG_STA_ID*/
#define QWLAN_HAL_CFG_STA_ID_STADEF                  "000AF5898989"
#define QCOM_WLAN_CFG_STA_ID_LEN                     6

/* QWLAN_HAL_CFG_CURRENT_TX_ANTENNA */
#define QWLAN_HAL_CFG_CURRENT_TX_ANTENNA_STAMIN          1
#define QWLAN_HAL_CFG_CURRENT_TX_ANTENNA_STAMAX          1
#define QWLAN_HAL_CFG_CURRENT_TX_ANTENNA_STADEF          1

/* QWLAN_HAL_CFG_CURRENT_RX_ANTENNA */
#define QWLAN_HAL_CFG_CURRENT_RX_ANTENNA_STAMIN          1
#define QWLAN_HAL_CFG_CURRENT_RX_ANTENNA_STAMAX          2
#define QWLAN_HAL_CFG_CURRENT_RX_ANTENNA_STADEF          1

/* QWLAN_HAL_CFG_LOW_GAIN_OVERRIDE */
#define QWLAN_HAL_CFG_LOW_GAIN_OVERRIDE_STAMIN           0
#define QWLAN_HAL_CFG_LOW_GAIN_OVERRIDE_STAMAX           1
#define QWLAN_HAL_CFG_LOW_GAIN_OVERRIDE_STADEF           0

/* QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN */
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_STAMIN             0
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_STAMAX             65535
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_STADEF             785
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_OFF                0
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_ON                 1
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_TX                 2
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_RX                 3
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_MASK               15
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_CHAIN_0_OFFSET     0
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_CHAIN_1_OFFSET     4
#define QWLAN_HAL_CFG_POWER_STATE_PER_CHAIN_CHAIN_2_OFFSET     8

/* QWLAN_HAL_CFG_CAL_PERIOD */
#define QWLAN_HAL_CFG_CAL_PERIOD_STAMIN                  2
#define QWLAN_HAL_CFG_CAL_PERIOD_STAMAX                  10
#define QWLAN_HAL_CFG_CAL_PERIOD_STADEF                  5

/* QWLAN_HAL_CFG_CAL_CONTROL */
#define QWLAN_HAL_CFG_CAL_CONTROL_STAMIN    0
#define QWLAN_HAL_CFG_CAL_CONTROL_STAMAX    1
#define QWLAN_HAL_CFG_CAL_CONTROL_STADEF    0
#define QWLAN_HAL_CFG_CAL_CONTROL_CAL_ON    0
#define QWLAN_HAL_CFG_CAL_CONTROL_CAL_OFF   1

/* QWLAN_HAL_CFG_PROXIMITY */
#define QWLAN_HAL_CFG_PROXIMITY_STAMIN    0
#define QWLAN_HAL_CFG_PROXIMITY_STAMAX    1
#define QWLAN_HAL_CFG_PROXIMITY_STADEF    0
#define QWLAN_HAL_CFG_PROXIMITY_OFF       0
#define QWLAN_HAL_CFG_PROXIMITY_ON        1

/* QWLAN_HAL_CFG_NETWORK_DENSITY */
#define QWLAN_HAL_CFG_NETWORK_DENSITY_STAMIN    0
#define QWLAN_HAL_CFG_NETWORK_DENSITY_STAMAX    3
#define QWLAN_HAL_CFG_NETWORK_DENSITY_STADEF    3
#define QWLAN_HAL_CFG_NETWORK_DENSITY_LOW       0
#define QWLAN_HAL_CFG_NETWORK_DENSITY_MEDIUM    1
#define QWLAN_HAL_CFG_NETWORK_DENSITY_HIGH      2
#define QWLAN_HAL_CFG_NETWORK_DENSITY_ADAPTIVE  3

/* QWLAN_HAL_CFG_MAX_MEDIUM_TIME */
#define QWLAN_HAL_CFG_MAX_MEDIUM_TIME_STAMIN    0
#define QWLAN_HAL_CFG_MAX_MEDIUM_TIME_STAMAX    65535
#define QWLAN_HAL_CFG_MAX_MEDIUM_TIME_STADEF    1024

/* QWLAN_HAL_CFG_MAX_MPDUS_IN_AMPDU */
#define QWLAN_HAL_CFG_MAX_MPDUS_IN_AMPDU_STAMIN    0
#define QWLAN_HAL_CFG_MAX_MPDUS_IN_AMPDU_STAMAX    65535
#define QWLAN_HAL_CFG_MAX_MPDUS_IN_AMPDU_STADEF    64

/* QWLAN_HAL_CFG_RTS_THRESHOLD */
#define QWLAN_HAL_CFG_RTS_THRESHOLD_STAMIN               0
#define QWLAN_HAL_CFG_RTS_THRESHOLD_STAMAX               2347
#define QWLAN_HAL_CFG_RTS_THRESHOLD_STADEF               2347

/* QWLAN_HAL_CFG_SHORT_RETRY_LIMIT */
#define QWLAN_HAL_CFG_SHORT_RETRY_LIMIT_STAMIN    0
#define QWLAN_HAL_CFG_SHORT_RETRY_LIMIT_STAMAX    255
#define QWLAN_HAL_CFG_SHORT_RETRY_LIMIT_STADEF    15

/* QWLAN_HAL_CFG_LONG_RETRY_LIMIT */
#define QWLAN_HAL_CFG_LONG_RETRY_LIMIT_STAMIN    0
#define QWLAN_HAL_CFG_LONG_RETRY_LIMIT_STAMAX    255
#define QWLAN_HAL_CFG_LONG_RETRY_LIMIT_STADEF    15

/* QWLAN_HAL_CFG_FRAGMENTATION_THRESHOLD */
#define QWLAN_HAL_CFG_FRAGMENTATION_THRESHOLD_STAMIN    256
#define QWLAN_HAL_CFG_FRAGMENTATION_THRESHOLD_STAMAX    8000
#define QWLAN_HAL_CFG_FRAGMENTATION_THRESHOLD_STADEF    8000

/* QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ZERO */
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ZERO_STAMIN    0
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ZERO_STAMAX    255
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ZERO_STADEF    5

/* QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ONE */
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ONE_STAMIN    0
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ONE_STAMAX    255
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_ONE_STADEF    10

/* QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_TWO */
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_TWO_STAMIN    0
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_TWO_STAMAX    255
#define QWLAN_HAL_CFG_DYNAMIC_THRESHOLD_TWO_STADEF    15

/* QWLAN_HAL_CFG_FIXED_RATE
 * Follwing rates in user configuration are mapped to TPE rates
 * Mapping is defined in the gHalUserFixedRateCfgToTpeRateTable
 */
#define QWLAN_HAL_CFG_FIXED_RATE_STAMIN                        0
#define QWLAN_HAL_CFG_FIXED_RATE_STAMAX                        226
#define QWLAN_HAL_CFG_FIXED_RATE_STADEF                        0
#define QWLAN_HAL_CFG_FIXED_RATE_AUTO                          0
#define QWLAN_HAL_CFG_FIXED_RATE_1MBPS                         1
#define QWLAN_HAL_CFG_FIXED_RATE_2MBPS                         2
#define QWLAN_HAL_CFG_FIXED_RATE_5_5MBPS                       3
#define QWLAN_HAL_CFG_FIXED_RATE_11MBPS                        4
#define QWLAN_HAL_CFG_FIXED_RATE_6MBPS                         5
#define QWLAN_HAL_CFG_FIXED_RATE_9MBPS                         6
#define QWLAN_HAL_CFG_FIXED_RATE_12MBPS                        7
#define QWLAN_HAL_CFG_FIXED_RATE_18MBPS                        8
#define QWLAN_HAL_CFG_FIXED_RATE_24MBPS                        9
#define QWLAN_HAL_CFG_FIXED_RATE_36MBPS                        10
#define QWLAN_HAL_CFG_FIXED_RATE_48MBPS                        11
#define QWLAN_HAL_CFG_FIXED_RATE_54MBPS                        12
#define QWLAN_HAL_CFG_FIXED_RATE_6_5MBPS_MCS0_20MHZ_SIMO       13
#define QWLAN_HAL_CFG_FIXED_RATE_13MBPS_MCS1_20MHZ_SIMO        14
#define QWLAN_HAL_CFG_FIXED_RATE_19_5MBPS_MCS2_20MHZ_SIMO      15
#define QWLAN_HAL_CFG_FIXED_RATE_26MBPS_MCS3_20MHZ_SIMO        16
#define QWLAN_HAL_CFG_FIXED_RATE_39MBPS_MCS4_20MHZ_SIMO        17
#define QWLAN_HAL_CFG_FIXED_RATE_52MBPS_MCS5_20MHZ_SIMO        18
#define QWLAN_HAL_CFG_FIXED_RATE_58_5MBPS_MCS6_20MHZ_SIMO      19
#define QWLAN_HAL_CFG_FIXED_RATE_65MBPS_MCS7_20MHZ_SIMO        20
#define QWLAN_HAL_CFG_FIXED_RATE_7_2MBPS_MCS0_20MHZ_SIMO_SGI   21
#define QWLAN_HAL_CFG_FIXED_RATE_14_4MBPS_MCS1_20MHZ_SIMO_SGI  22
#define QWLAN_HAL_CFG_FIXED_RATE_21_7MBPS_MCS2_20MHZ_SIMO_SGI  23
#define QWLAN_HAL_CFG_FIXED_RATE_28_9MBPS_MCS3_20MHZ_SIMO_SGI  24
#define QWLAN_HAL_CFG_FIXED_RATE_43_3MBPS_MCS4_20MHZ_SIMO_SGI  25
#define QWLAN_HAL_CFG_FIXED_RATE_57_8MBPS_MCS5_20MHZ_SIMO_SGI  26
#define QWLAN_HAL_CFG_FIXED_RATE_65MBPS_MCS6_20MHZ_SIMO_SGI    27
#define QWLAN_HAL_CFG_FIXED_RATE_72_2MBPS_MCS7_20MHZ_SIMO_SGI  28
#define QWLAN_HAL_CFG_FIXED_RATE_13_5MBPS_MCS0_40MHZ_SIMO      29
#define QWLAN_HAL_CFG_FIXED_RATE_27MBPS_MCS1_40MHZ_SIMO        30
#define QWLAN_HAL_CFG_FIXED_RATE_40_5MBPS_MCS2_40MHZ_SIMO      31
#define QWLAN_HAL_CFG_FIXED_RATE_54MBPS_MCS3_40MHZ_SIMO        32
#define QWLAN_HAL_CFG_FIXED_RATE_81MBPS_MCS4_40MHZ_SIMO        33
#define QWLAN_HAL_CFG_FIXED_RATE_108MBPS_MCS5_40MHZ_SIMO       34
#define QWLAN_HAL_CFG_FIXED_RATE_121_5MBPS_MCS6_40MHZ_SIMO     35
#define QWLAN_HAL_CFG_FIXED_RATE_135MBPS_MCS7_40MHZ_SIMO       36
#define QWLAN_HAL_CFG_FIXED_RATE_15MBPS_MCS0_40MHZ_SIMO_SGI    37
#define QWLAN_HAL_CFG_FIXED_RATE_30MBPS_MCS1_40MHZ_SIMO_SGI    38
#define QWLAN_HAL_CFG_FIXED_RATE_45MBPS_MCS2_40MHZ_SIMO_SGI    39
#define QWLAN_HAL_CFG_FIXED_RATE_60MBPS_MCS3_40MHZ_SIMO_SGI    40
#define QWLAN_HAL_CFG_FIXED_RATE_90MBPS_MCS4_40MHZ_SIMO_SGI    41
#define QWLAN_HAL_CFG_FIXED_RATE_120MBPS_MCS5_40MHZ_SIMO_SGI   42
#define QWLAN_HAL_CFG_FIXED_RATE_135MBPS_MCS6_40MHZ_SIMO_SGI   43
#define QWLAN_HAL_CFG_FIXED_RATE_150MBPS_MCS7_40MHZ_SIMO_SGI   44

/* QWLAN_HAL_CFG_RETRYRATE_POLICY */
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_STAMIN         0
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_STAMAX         255
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_STADEF         4
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_MIN_SUPPORTED  0
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_PRIMARY        1
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_RESERVED       2
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_CLOSEST        3
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_AUTOSELECT     4
#define QWLAN_HAL_CFG_RETRYRATE_POLICY_MAX            5

/* QWLAN_HAL_CFG_RETRYRATE_SECONDARY */
#define QWLAN_HAL_CFG_RETRYRATE_SECONDARY_STAMIN    0
#define QWLAN_HAL_CFG_RETRYRATE_SECONDARY_STAMAX    255
#define QWLAN_HAL_CFG_RETRYRATE_SECONDARY_STADEF    0

/* QWLAN_HAL_CFG_RETRYRATE_TERTIARY */
#define QWLAN_HAL_CFG_RETRYRATE_TERTIARY_STAMIN    0
#define QWLAN_HAL_CFG_RETRYRATE_TERTIARY_STAMAX    255
#define QWLAN_HAL_CFG_RETRYRATE_TERTIARY_STADEF    0

/* QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION */
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_STAMIN     0
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_STAMAX     5
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_STADEF     5
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_DISABLE    0
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_CTS        1
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_RTS        2
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_DUAL_CTS   3
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_RTS_ALWAYS 4
#define QWLAN_HAL_CFG_FORCE_POLICY_PROTECTION_AUTO       5

/* QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_24GHZ */
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_24GHZ_STAMIN    0
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_24GHZ_STAMAX   QWLAN_HAL_CFG_FIXED_RATE_STAMAX
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_24GHZ_STADEF   QWLAN_HAL_CFG_FIXED_RATE_1MBPS

/* QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_5GHZ */
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_5GHZ_STAMIN    0
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_5GHZ_STAMAX    QWLAN_HAL_CFG_FIXED_RATE_STAMAX
#define QWLAN_HAL_CFG_FIXED_RATE_MULTICAST_5GHZ_STADEF    QWLAN_HAL_CFG_FIXED_RATE_6MBPS

/* QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_24GHZ */
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_24GHZ_STAMIN    0
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_24GHZ_STAMAX     QWLAN_HAL_CFG_FIXED_RATE_STAMAX
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_24GHZ_STADEF     QWLAN_HAL_CFG_FIXED_RATE_1MBPS

/* QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_5GHZ */
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_5GHZ_STAMIN    0
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_5GHZ_STAMAX      QWLAN_HAL_CFG_FIXED_RATE_STAMAX
#define QWLAN_HAL_CFG_DEFAULT_RATE_INDEX_5GHZ_STADEF      QWLAN_HAL_CFG_FIXED_RATE_6MBPS

/* QWLAN_HAL_CFG_MAX_BA_SESSIONS */
#define QWLAN_HAL_CFG_MAX_BA_SESSIONS_STAMIN    0
#define QWLAN_HAL_CFG_MAX_BA_SESSIONS_STAMAX    64
#define QWLAN_HAL_CFG_MAX_BA_SESSIONS_STADEF    40

/* QWLAN_HAL_CFG_PS_DATA_INACTIVITY_TIMEOUT */
#define QWLAN_HAL_CFG_PS_DATA_INACTIVITY_TIMEOUT_STAMIN    1
#define QWLAN_HAL_CFG_PS_DATA_INACTIVITY_TIMEOUT_STAMAX    255
#define QWLAN_HAL_CFG_PS_DATA_INACTIVITY_TIMEOUT_STADEF    20

/* QWLAN_HAL_CFG_PS_TX_INACTIVITY_TIMEOUT */
#define QWLAN_HAL_CFG_PS_TX_INACTIVITY_TIMEOUT_STAMIN    0
#define QWLAN_HAL_CFG_PS_TX_INACTIVITY_TIMEOUT_STAMAX    255
#define QWLAN_HAL_CFG_PS_TX_INACTIVITY_TIMEOUT_STADEF    300

/* QWLAN_HAL_CFG_PS_ENABLE_BCN_FILTER */
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_FILTER_STAMIN    0
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_FILTER_STAMAX    1
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_FILTER_STADEF    1

/* QWLAN_HAL_CFG_PS_ENABLE_RSSI_MONITOR */
#define QWLAN_HAL_CFG_PS_ENABLE_RSSI_MONITOR_STAMIN    0
#define QWLAN_HAL_CFG_PS_ENABLE_RSSI_MONITOR_STAMAX    1
#define QWLAN_HAL_CFG_PS_ENABLE_RSSI_MONITOR_STADEF    1

/* QWLAN_HAL_CFG_NUM_BEACON_PER_RSSI_AVERAGE */
#define QWLAN_HAL_CFG_NUM_BEACON_PER_RSSI_AVERAGE_STAMIN    1
#define QWLAN_HAL_CFG_NUM_BEACON_PER_RSSI_AVERAGE_STAMAX    20
#define QWLAN_HAL_CFG_NUM_BEACON_PER_RSSI_AVERAGE_STADEF    20

/* QWLAN_HAL_CFG_STATS_PERIOD */
#define QWLAN_HAL_CFG_STATS_PERIOD_STAMIN    1
#define QWLAN_HAL_CFG_STATS_PERIOD_STAMAX    10
#define QWLAN_HAL_CFG_STATS_PERIOD_STADEF    10

/* QWLAN_HAL_CFG_CFP_MAX_DURATION */
#define QWLAN_HAL_CFG_CFP_MAX_DURATION_STAMIN    0
#define QWLAN_HAL_CFG_CFP_MAX_DURATION_STAMAX    65535
#define QWLAN_HAL_CFG_CFP_MAX_DURATION_STADEF    30000

/* QWLAN_HAL_CFG_FRAME_TRANS_ENABLED */
#define QWLAN_HAL_CFG_FRAME_TRANS_ENABLED_STAMIN  0
#define QWLAN_HAL_CFG_FRAME_TRANS_ENABLED_STAMAX  1
#define QWLAN_HAL_CFG_FRAME_TRANS_ENABLED_STADEF  0

/* QWLAN_HAL_CFG_DTIM_PERIOD */
#define QWLAN_HAL_CFG_DTIM_PERIOD_STAMIN    0
#define QWLAN_HAL_CFG_DTIM_PERIOD_STAMAX    65535
#define QWLAN_HAL_CFG_DTIM_PERIOD_STADEF    1

/* QWLAN_HAL_CFG_BA_THRESHOLD_HIGH */
#define QWLAN_HAL_CFG_BA_THRESHOLD_HIGH_STAMIN    0
#define QWLAN_HAL_CFG_BA_THRESHOLD_HIGH_STAMAX    65535
#define QWLAN_HAL_CFG_BA_THRESHOLD_HIGH_STADEF    1

/* QWLAN_HAL_CFG_MAX_BA_BUFFERS */
#define QWLAN_HAL_CFG_MAX_BA_BUFFERS_STAMIN    0
#define QWLAN_HAL_CFG_MAX_BA_BUFFERS_STAMAX    2560
#define QWLAN_HAL_CFG_MAX_BA_BUFFERS_STADEF    2560



/* ACM, AIFSN, [CWmin, CWmax, TxOp]-11A/11B/11G
 * Cwmin and Cwmax are two bytes each, MSB first. So Cwmax of [03 FF] is
 * equivalent to 0x03ff = 1023*/
#define QWLAN_HAL_CFG_EDCA_PROFILE_ACM_IDX       0   /* byte[0] */
#define QWLAN_HAL_CFG_EDCA_PROFILE_AIFSN_IDX     1   /* byte[1] */
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMINA_IDX    2   /* byte[2] & byte [3]   */
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMAXA_IDX    4   /* byte[4] & byte [5]   */
#define QWLAN_HAL_CFG_EDCA_PROFILE_TXOPA_IDX     6   /* byte[6] */
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMINB_IDX    7   /* byte[7] & byte [8]   */
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMAXB_IDX    9   /* byte[9] & byte [10]  */
#define QWLAN_HAL_CFG_EDCA_PROFILE_TXOPB_IDX     11  /* byte[11]*/
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMING_IDX    12  /* byte[12] & byte [13] */
#define QWLAN_HAL_CFG_EDCA_PROFILE_CWMAXG_IDX    14  /* byte[14] & byte [15] */
#define QWLAN_HAL_CFG_EDCA_PROFILE_TXOPG_IDX     16  /* byte[16]*/
#define QWLAN_HAL_CFG_EDCA_PARAM_MAX_LEN         20

/* QWLAN_HAL_CFG_EDCA_WMM_ACBK */
#define QWLAN_HAL_CFG_EDCA_WMM_ACBK_DEFAULT  "0007000F03FF00001F03FF00000F03FF00"

/* QWLAN_HAL_CFG_EDCA_WMM_ACBE */
#define QWLAN_HAL_CFG_EDCA_WMM_ACBE_DEFAULT  "0003000F03FF00001F03FF00000F03FF00"

/* QWLAN_HAL_CFG_EDCA_WMM_ACVI */
#define QWLAN_HAL_CFG_EDCA_WMM_ACVI_DEFAULT  "00020007000F5E000F001FBC0007000F5E"

/* QWLAN_HAL_CFG_EDCA_WMM_ACVO */
#define QWLAN_HAL_CFG_EDCA_WMM_ACVO_DEFAULT  "0002000300072F0007000F66000300072F"


/* QWLAN_HAL_CFG_RPE_POLLING_THRESHOLD */
#define QWLAN_CFG_RPE_POLLING_THRESHOLD_STAMIN            0
#define QWLAN_CFG_RPE_POLLING_THRESHOLD_STAMAX            65535
#define QWLAN_CFG_RPE_POLLING_THRESHOLD_STADEF            30

/* QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC0_REG */
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC0_REG_STAMIN   0
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC0_REG_STAMAX   65535
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC0_REG_STADEF   30

/* QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC1_REG */
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC1_REG_STAMIN   0
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC1_REG_STAMAX   65535
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC1_REG_STADEF   30

/* QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC2_REG */
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC2_REG_STAMIN   0
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC2_REG_STAMAX   65535
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC2_REG_STADEF   30

/* QWLAN_HAL_CFG_RPE_AGING_THRESHOLD_FOR_AC3_REG */
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC3_REG_STAMIN   0
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC3_REG_STAMAX   65535
#define QWLAN_CFG_RPE_AGING_THRESHOLD_FOR_AC3_REG_STADEF   30

/* QWLAN_HAL_CFG_NO_OF_ONCHIP_REORDER_SESSIONS */
#define QWLAN_CFG_NO_OF_ONCHIP_REORDER_SESSIONS_STAMIN     0
#define QWLAN_CFG_NO_OF_ONCHIP_REORDER_SESSIONS_STAMAX     2
#define QWLAN_CFG_NO_OF_ONCHIP_REORDER_SESSIONS_STADEF     1

#define QWLAN_HAL_CFG_PS_LISTEN_INTERVAL_STAMIN    0
#define QWLAN_HAL_CFG_PS_LISTEN_INTERVAL_STAMAX    65535
#define QWLAN_HAL_CFG_PS_LISTEN_INTERVAL_STADEF    1

#define QWLAN_HAL_CFG_PS_HEART_BEAT_THRESHOLD_STAMIN    0
#define QWLAN_HAL_CFG_PS_HEART_BEAT_THRESHOLD_STAMAX    65535
#define QWLAN_HAL_CFG_PS_HEART_BEAT_THRESHOLD_STADEF    40

#define QWLAN_HAL_CFG_PS_NTH_BEACON_FILTER_STAMIN    0
#define QWLAN_HAL_CFG_PS_NTH_BEACON_FILTER_STAMAX    255
#define QWLAN_HAL_CFG_PS_NTH_BEACON_FILTER_STADEF    10

#define QWLAN_HAL_CFG_PS_MAX_PS_POLL_STAMIN    0
#define QWLAN_HAL_CFG_PS_MAX_PS_POLL_STAMAX    255
#define QWLAN_HAL_CFG_PS_MAX_PS_POLL_STADEF    0

#define QWLAN_HAL_CFG_PS_MIN_RSSI_THRESHOLD_STAMIN    0
#define QWLAN_HAL_CFG_PS_MIN_RSSI_THRESHOLD_STAMAX    10
#define QWLAN_HAL_CFG_PS_MIN_RSSI_THRESHOLD_STADEF    10

#define QWLAN_HAL_CFG_PS_RSSI_FILTER_PERIOD_STAMIN    0
#define QWLAN_HAL_CFG_PS_RSSI_FILTER_PERIOD_STAMAX    255
#define QWLAN_HAL_CFG_PS_RSSI_FILTER_PERIOD_STADEF    20

#define QWLAN_HAL_CFG_PS_BROADCAST_FRAME_FILTER_ENABLE_STAMIN    0
#define QWLAN_HAL_CFG_PS_BROADCAST_FRAME_FILTER_ENABLE_STAMAX    1
#define QWLAN_HAL_CFG_PS_BROADCAST_FRAME_FILTER_ENABLE_STADEF    0

#define QWLAN_HAL_CFG_PS_IGNORE_DTIM_STAMIN    0
#define QWLAN_HAL_CFG_PS_IGNORE_DTIM_STAMAX    1
#define QWLAN_HAL_CFG_PS_IGNORE_DTIM_STADEF    0

#define QWLAN_HAL_CFG_PS_ENABLE_BCN_EARLY_TERM_STAMIN    0
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_EARLY_TERM_STAMAX    1
#define QWLAN_HAL_CFG_PS_ENABLE_BCN_EARLY_TERM_STADEF    0

#define QWLAN_HAL_CFG_DYNAMIC_PS_POLL_VALUE_STAMIN    0
#define QWLAN_HAL_CFG_DYNAMIC_PS_POLL_VALUE_STAMAX    255
#define QWLAN_HAL_CFG_DYNAMIC_PS_POLL_VALUE_STADEF    0

#define QWLAN_HAL_CFG_PS_NULLDATA_AP_RESP_TIMEOUT_STAMIN    0
#define QWLAN_HAL_CFG_PS_NULLDATA_AP_RESP_TIMEOUT_STAMAX    80
#define QWLAN_HAL_CFG_PS_NULLDATA_AP_RESP_TIMEOUT_STADEF    0

/* QWLAN_HAL_CFG_TX_PWR_CTRL_ENABLE */
#define QWLAN_CFG_TX_PWR_CTRL_ENABLE_STAMIN    0
#define QWLAN_CFG_TX_PWR_CTRL_ENABLE_STAMAX    1
#define QWLAN_CFG_TX_PWR_CTRL_ENABLE_STADEF    1

#define QWLAN_HAL_CFG_TELE_BCN_WAKEUP_EN_STAMIN    0
#define QWLAN_HAL_CFG_TELE_BCN_WAKEUP_EN_STAMAX    1
#define QWLAN_HAL_CFG_TELE_BCN_WAKEUP_EN_STADEF    0

#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_STAMIN    0
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_STAMAX    7
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_STADEF    3

#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_IDLE_BCNS_STAMIN    5
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_IDLE_BCNS_STAMAX    255
#define QWLAN_HAL_CFG_TELE_BCN_TRANS_LI_IDLE_BCNS_STADEF    10

#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_STAMIN    0
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_STAMAX    7
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_STADEF    5

#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_IDLE_BCNS_STAMIN    5
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_IDLE_BCNS_STAMAX    255
#define QWLAN_HAL_CFG_TELE_BCN_MAX_LI_IDLE_BCNS_STADEF    15

#define QWLAN_HAL_CFG_MCAST_BCAST_FILTER_SETTING_STAMIN    0
#define QWLAN_HAL_CFG_MCAST_BCAST_FILTER_SETTING_STAMAX    3
#define QWLAN_HAL_CFG_MCAST_BCAST_FILTER_SETTING_STADEF    0

#define QWLAN_HAL_CFG_BCN_EARLY_TERM_WAKEUP_INTERVAL_STAMIN    1
#define QWLAN_HAL_CFG_BCN_EARLY_TERM_WAKEUP_INTERVAL_STAMAX    255
#define QWLAN_HAL_CFG_BCN_EARLY_TERM_WAKEUP_INTERVAL_STADEF    5

/*Values to be added in hexadecimal*/
/* QWLAN_HAL_CFG_VALID_RADAR_LIST */
#define QWLAN_HAL_CFG_VALID_RADAR_CHANNEL_LIST_DEFAULT "36"
#define QWLAN_HAL_CFG_RADAR_CHANNEL_LIST_LEN    20

/* QWLAN_HAL_CFG_TX_POWER_24_20 */
#define QWLAN_WLAN_TX_POWER_24_20_DEFAULT    299
#define QWLAN_WLAN_TX_POWER_24_20_MIN        299
#define QWLAN_WLAN_TX_POWER_24_20_MAX        299

/* QWLAN_HAL_CFG_TX_POWER_24_40 */
#define QWLAN_WLAN_TX_POWER_24_40_DEFAULT    300
#define QWLAN_WLAN_TX_POWER_24_40_MIN        299
#define QWLAN_WLAN_TX_POWER_24_40_MAX        299

/* QWLAN_HAL_CFG_TX_POWER_50_20 */
#define QWLAN_WLAN_TX_POWER_50_20_DEFAULT    301
#define QWLAN_WLAN_TX_POWER_50_20_MIN        299
#define QWLAN_WLAN_TX_POWER_50_20_MAX        299

/* QWLAN_HAL_CFG_TX_POWER_50_40 */
#define QWLAN_WLAN_TX_POWER_50_40_DEFAULT    302
#define QWLAN_WLAN_TX_POWER_50_40_MIN        299
#define QWLAN_WLAN_TX_POWER_50_40_MAX        299

/* QCOM_WLAN_CFG_MAX_TX_POWER_2_4 */
#define QCOM_WLAN_CFG_MAX_TX_POWER_2_4_LEN      128
/* byte[0] = 0x01 = First Channel; byte[1] =  0x0E = 14 = MaxChannels; byte[2] = 0x14 = 20 = Tx Power (dBm) */
#define QCOM_WLAN_CFG_MAX_TX_POWER_2_4_DEFAULT  "010E14"

/* QCOM_WLAN_CFG_MAX_TX_POWER_5 */
#define QCOM_WLAN_CFG_MAX_TX_POWER_5_LEN     128
/* byte[0] = 0x24 = 36 = First Channel; byte[1] =  0x82 = 130 = MaxChannels; byte[2] = 0x14 = 20 = Tx Power (dBm) */
#define QCOM_WLAN_CFG_MAX_TX_POWER_5_DEFAULT  "248214"


#define QWLAN_HAL_CFG_INFRA_STA_KEEP_ALIVE_PERIOD_STAMIN    0
#define QWLAN_HAL_CFG_INFRA_STA_KEEP_ALIVE_PERIOD_STAMAX    65535
#define QWLAN_HAL_CFG_INFRA_STA_KEEP_ALIVE_PERIOD_STADEF    0

#define QWLAN_HAL_CFG_ENABLE_CLOSE_LOOP_MIN  0
#define QWLAN_HAL_CFG_ENABLE_CLOSE_LOOP_MAX  1
#define QWLAN_HAL_CFG_ENABLE_CLOSE_LOOP_DEF  0

/* QWLAN_HAL_CFG_BTC_EXECUTION_MODE */
#define QWLAN_HAL_CFG_BTC_EXECUTION_MODE_MIN  0
#define QWLAN_HAL_CFG_BTC_EXECUTION_MODE_MAX  5
#define QWLAN_HAL_CFG_BTC_EXECUTION_MODE_DEF  0

/* QWLAN_HAL_CFG_BTC_DHCP_BT_SLOTS_TO_BLOCK */
#define QWLAN_HAL_CFG_BTC_DHCP_BT_SLOTS_TO_BLOCK_MIN  0
#define QWLAN_HAL_CFG_BTC_DHCP_BT_SLOTS_TO_BLOCK_MAX  255
#define QWLAN_HAL_CFG_BTC_DHCP_BT_SLOTS_TO_BLOCK_DEF  0

/* QWLAN_HAL_CFG_BTC_A2DP_DHCP_BT_SUB_INTERVALS */
#define QWLAN_HAL_CFG_BTC_A2DP_DHCP_BT_SUB_INTERVALS_MIN  0
#define QWLAN_HAL_CFG_BTC_A2DP_DHCP_BT_SUB_INTERVALS_MAX  255
#define QWLAN_HAL_CFG_BTC_A2DP_DHCP_BT_SUB_INTERVALS_DEF  15

/* QWLAN_HAL_CFG_WCNSS_API_VERSION */
#define QWLAN_HAL_CFG_WCNSS_API_VERSION_MIN  0           /* equivalent to 0.0.0.0 */
#define QWLAN_HAL_CFG_WCNSS_API_VERSION_MAX  4294967295U /* equivalent to 255.255.255.255 */
#define QWLAN_HAL_CFG_WCNSS_API_VERSION_DEF  0           /* equivalent to 0.0.0.0 */

/* QWLAN_HAL_CFG_AP_KEEPALIVE_TIMEOUT */
#define QWLAN_HAL_CFG_AP_KEEPALIVE_TIMEOUT_MIN  3
#define QWLAN_HAL_CFG_AP_KEEPALIVE_TIMEOUT_MAX  20
#define QWLAN_HAL_CFG_AP_KEEPALIVE_TIMEOUT_DEF  5

/* QWLAN_HAL_CFG_GO_KEEPALIVE_TIMEOUT */
#define QWLAN_HAL_CFG_GO_KEEPALIVE_TIMEOUT_MIN  3
#define QWLAN_HAL_CFG_GO_KEEPALIVE_TIMEOUT_MAX  20
#define QWLAN_HAL_CFG_GO_KEEPALIVE_TIMEOUT_DEF  5

/* QWLAN_HAL_CFG_ENABLE_MC_ADDR_LIST */
#define QWLAN_HAL_CFG_ENABLE_MC_ADDR_LIST_MIN  0
#define QWLAN_HAL_CFG_ENABLE_MC_ADDR_LIST_MAX  1
#define QWLAN_HAL_CFG_ENABLE_MC_ADDR_LIST_DEF  0

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_BT */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_BT_MIN 5000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_BT_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_BT_DEF 120000

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_BT */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_BT_MIN 5000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_BT_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_BT_DEF 10000

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_BT */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_BT_MIN 5000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_BT_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_BT_DEF 10000

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_BT */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_BT_MIN 5000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_BT_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_BT_DEF 10000

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_WLAN */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_WLAN_MIN 0
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_WLAN_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_INQ_WLAN_DEF 30000

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_WLAN */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_WLAN_MIN 0
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_WLAN_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_PAGE_WLAN_DEF 0

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_WLAN */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_WLAN_MIN 0
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_WLAN_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_CONN_WLAN_DEF 0

/* QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_WLAN */
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_WLAN_MIN 0
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_WLAN_MAX 500000
#define QWLAN_HAL_CFG_BTC_STATIC_LEN_LE_WLAN_DEF 0

/* QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_BT */
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_BT_MIN 25000
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_BT_MAX 500000
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_BT_DEF 250000

/* QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_WLAN */
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_WLAN_MIN 15000
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_WLAN_MAX 500000
#define QWLAN_HAL_CFG_BTC_DYN_MAX_LEN_WLAN_DEF 45000

/* QWLAN_HAL_CFG_BTC_MAX_SCO_BLOCK_PERC */
#define QWLAN_HAL_CFG_BTC_MAX_SCO_BLOCK_PERC_MIN 0
#define QWLAN_HAL_CFG_BTC_MAX_SCO_BLOCK_PERC_MAX 100
#define QWLAN_HAL_CFG_BTC_MAX_SCO_BLOCK_PERC_DEF 1

/* QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_A2DP */
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_A2DP_MIN 0
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_A2DP_MAX 1
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_A2DP_DEF 1

/* QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_SCO */
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_SCO_MIN 0
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_SCO_MAX 1
#define QWLAN_HAL_CFG_BTC_DHCP_PROT_ON_SCO_DEF 0

/* QWLAN_HAL_CFG_ENABLE_UNICAST_FILTER */
#define QWLAN_HAL_CFG_ENABLE_UNICAST_FILTER_MIN  0
#define QWLAN_HAL_CFG_ENABLE_UNICAST_FILTER_MAX  1
#define QWLAN_HAL_CFG_ENABLE_UNICAST_FILTER_DEF  0

/* QWLAN_HAL_CFG_MAX_ASSOC_LIMIT */
#define QWLAN_HAL_CFG_MAX_ASSOC_LIMIT_MIN  10
#define QWLAN_HAL_CFG_MAX_ASSOC_LIMIT_MAX  32
#define QWLAN_HAL_CFG_MAX_ASSOC_LIMIT_DEF  10

/* QWLAN_HAL_CFG_ENABLE_LPWR_IMG_TRANSITION */
#define QWLAN_HAL_CFG_ENABLE_LPWR_IMG_TRANSITION_MIN    0
#define QWLAN_HAL_CFG_ENABLE_LPWR_IMG_TRANSITION_MAX    1
#define QWLAN_HAL_CFG_ENABLE_LPWR_IMG_TRANSITION_DEF    0

/* QWLAN_HAL_CFG_ENABLE_MCC_ADAPTIVE_SCHEDULER */
#define QWLAN_HAL_CFG_ENABLE_MCC_ADAPTIVE_SCHEDULER_MIN    0
#define QWLAN_HAL_CFG_ENABLE_MCC_ADAPTIVE_SCHEDULER_MAX    1
#define QWLAN_HAL_CFG_ENABLE_MCC_ADAPTIVE_SCHEDULER_DEF    0

/* QWLAN_HAL_CFG_ENABLE_DETECT_PS_SUPPORT */
#define QWLAN_HAL_CFG_ENABLE_DETECT_PS_SUPPORT_MIN    0
#define QWLAN_HAL_CFG_ENABLE_DETECT_PS_SUPPORT_MAX    1
#define QWLAN_HAL_CFG_ENABLE_DETECT_PS_SUPPORT_DEF    0

/* QWLAN_HAL_CFG_AP_LINK_MONITOR_TIMEOUT */
#define QWLAN_HAL_CFG_AP_LINK_MONITOR_TIMEOUT_MIN  3
#define QWLAN_HAL_CFG_AP_LINK_MONITOR_TIMEOUT_MAX  50
#define QWLAN_HAL_CFG_AP_LINK_MONITOR_TIMEOUT_DEF  10

/* QWLAN_HAL_CFG_GO_LINK_MONITOR_TIMEOUT */
#define QWLAN_HAL_CFG_GO_LINK_MONITOR_TIMEOUT_MIN  3
#define QWLAN_HAL_CFG_GO_LINK_MONITOR_TIMEOUT_MAX  50
#define QWLAN_HAL_CFG_GO_LINK_MONITOR_TIMEOUT_DEF  10

/*QWLAN_HAL_CFG_BTC_DWELL_TIME_MULTIPLIER*/
/*This value is multiplied to ChannelDwellTime
 *i.e If value is 300 then ChannelDwellTime is (3*ChannelDwelltime)*/
#define QWLAN_HAL_CFG_BTC_DWELL_TIME_MULTIPLIER_MIN    0
#define QWLAN_HAL_CFG_BTC_DWELL_TIME_MULTIPLIER_MAX    300
#define QWLAN_HAL_CFG_BTC_DWELL_TIME_MULTIPLIER_DEF    300

/* QWLAN_HAL_CFG_ENABLE_TDLS_OXYGEN_MODE */
#define QWLAN_HAL_CFG_ENABLE_TDLS_OXYGEN_MODE_MIN    0
#define QWLAN_HAL_CFG_ENABLE_TDLS_OXYGEN_MODE_MAX    1
#define QWLAN_HAL_CFG_ENABLE_TDLS_OXYGEN_MODE_DEF    0

/* QWLAN_HAL_CFG_ENABLE_NAT_KEEP_ALIVE_FILTER */
#define QWLAN_HAL_CFG_ENABLE_NAT_KEEP_ALIVE_FILTER_MIN   0
#define QWLAN_HAL_CFG_ENABLE_NAT_KEEP_ALIVE_FILTER_MAX   1
#define QWLAN_HAL_CFG_ENABLE_NAT_KEEP_ALIVE_FILTER_DEF   0

/* QWLAN_HAL_CFG_ENABLE_SAP_OBSS_PROT */
#define QWLAN_HAL_CFG_ENABLE_SAP_OBSS_PROT_MIN   0
#define QWLAN_HAL_CFG_ENABLE_SAP_OBSS_PROT_MAX   1
#define QWLAN_HAL_CFG_ENABLE_SAP_OBSS_PROT_DEF   0

/* QWLAN_HAL_CFG_PSPOLL_DATA_RECEP_TIMEOUT */
#define QWLAN_HAL_CFG_PSPOLL_DATA_RECEP_TIMEOUT_STAMIN    1
#define QWLAN_HAL_CFG_PSPOLL_DATA_RECEP_TIMEOUT_STAMAX    255
#define QWLAN_HAL_CFG_PSPOLL_DATA_RECEP_TIMEOUT_STADEF    20

/* QWLAN_HAL_CFG_TDLS_PUAPSD_BUFFER_STA_CAPABLE */
#define QWLAN_HAL_CFG_TDLS_PUAPSD_BUFFER_STA_CAPABLE_MIN    0
#define QWLAN_HAL_CFG_TDLS_PUAPSD_BUFFER_STA_CAPABLE_MAX    1
#define QWLAN_HAL_CFG_TDLS_PUAPSD_BUFFER_STA_CAPABLE_DEF    0

/* QWLAN_HAL_CFG_TDLS_PUAPSD_MASK */
#define QWLAN_HAL_CFG_TDLS_PUAPSD_MASK_MIN    0
#define QWLAN_HAL_CFG_TDLS_PUAPSD_MASK_MAX    0xf
#define QWLAN_HAL_CFG_TDLS_PUAPSD_MASK_DEF    0

/* QWLAN_HAL_CFG_TDLS_PUAPSD_INACTIVITY_TIME */
#define QWLAN_HAL_CFG_TDLS_PUAPSD_INACTIVITY_TIME_MIN    0
#define QWLAN_HAL_CFG_TDLS_PUAPSD_INACTIVITY_TIME_MAX    10
#define QWLAN_HAL_CFG_TDLS_PUAPSD_INACTIVITY_TIME_DEF    0

/* QWLAN_HAL_CFG_TDLS_PUAPSD_RX_FRAME_THRESHOLD_IN_SP */
#define QWLAN_HAL_CFG_TDLS_PUAPSD_RX_FRAME_THRESHOLD_IN_SP_MIN    10
#define QWLAN_HAL_CFG_TDLS_PUAPSD_RX_FRAME_THRESHOLD_IN_SP_MAX    20
#define QWLAN_HAL_CFG_TDLS_PUAPSD_RX_FRAME_THRESHOLD_IN_SP_DEF    10

/* QWLAN_HAL_CFG_ANTENNA_DIVERSITY */
#define QWLAN_HAL_CFG_ANTENNA_DIVERSITY_DEF  0
#define QWLAN_HAL_CFG_ANTENNA_DIVERSITY_MIN  0
#define QWLAN_HAL_CFG_ANTENNA_DIVERSITY_MAX  3

#define QWLAN_HAL_CFG_ATH_DEF   0
#define QWLAN_HAL_CFG_ATH_MIN   0
#define QWLAN_HAL_CFG_ATH_MAX   1

/* QWLAN_HAL_CFG_FLEXCONNECT_POWER_FACTOR */
#define QWLAN_HAL_CFG_FLEXCONNECT_POWER_FACTOR_DEF  0
#define QWLAN_HAL_CFG_FLEXCONNECT_POWER_FACTOR_MIN  0
#define QWLAN_HAL_CFG_FLEXCONNECT_POWER_FACTOR_MAX  9

#endif //__WLAN_HAL_CFG_H__


