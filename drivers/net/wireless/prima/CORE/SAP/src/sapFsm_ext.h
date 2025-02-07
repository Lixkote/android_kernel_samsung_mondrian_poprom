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

/*
 * Copyright (c) 2008 QUALCOMM Incorporated. All Rights Reserved.
 * Qualcomm Confidential and Proprietary 
 */

/* This file is generated from btampFsm.cdd - do not edit manually*/
/* Generated on: Thu Oct 16 15:40:39 PDT 2008 */


#ifndef __SAPFSM_EXT_H__
#define __SAPFSM_EXT_H__

/* Events that can be sent to the SAP state-machine */
typedef enum
{
  eSAP_TIMER_CONNECT_ACCEPT_TIMEOUT=0U,
  eSAP_MAC_CONNECT_COMPLETED,
  eSAP_CHANNEL_SELECTION_FAILED,
  eSAP_MAC_CONNECT_INDICATION,
  eSAP_MAC_KEY_SET_SUCCESS,
  eSAP_RSN_FAILURE,
  eSAP_MAC_SCAN_COMPLETE,
  eSAP_HDD_START_INFRA_BSS,
  eSAP_MAC_READY_FOR_CONNECTIONS,
  eSAP_MAC_START_BSS_SUCCESS,
  eSAP_MAC_START_BSS_FAILURE,
  eSAP_RSN_SUCCESS,
  eSAP_MAC_START_FAILS,
  eSAP_HDD_STOP_INFRA_BSS,
  eSAP_WRITE_REMOTE_AMP_ASSOC,

  eSAP_NO_MSG
}eSapMsg_t;


#endif
