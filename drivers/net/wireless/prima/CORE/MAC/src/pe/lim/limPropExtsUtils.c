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
 *
 * Airgo Networks, Inc proprietary. All rights reserved.
 * This file limPropExtsUtils.cc contains the utility functions
 * to populate, parse proprietary extensions required to
 * support ANI feature set.
 *
 * Author:        Chandra Modumudi
 * Date:          11/27/02
 * History:-
 * Date           Modified by    Modification Information
 * --------------------------------------------------------------------
 *
 */
#include "aniGlobal.h"
#include "wniCfgSta.h"
#include "sirCommon.h"
#include "sirDebug.h"
#include "utilsApi.h"
#include "cfgApi.h"
#include "limApi.h"
#include "limTypes.h"
#include "limUtils.h"
#include "limAssocUtils.h"
#include "limPropExtsUtils.h"
#include "limSerDesUtils.h"
#include "limTrace.h"
#include "limSession.h"
#define LIM_GET_NOISE_MAX_TRY 5
/**
 * limExtractApCapability()
 *
 *FUNCTION:
 * This function is called to extract AP's HCF/WME/WSM capability
 * from the IEs received from it in Beacon/Probe Response frames
 *
 *LOGIC:
 *
 *ASSUMPTIONS:
 * NA
 *
 *NOTE:
 *
 * @param   pMac      Pointer to Global MAC structure
 * @param   pIE       Pointer to starting IE in Beacon/Probe Response
 * @param   ieLen     Length of all IEs combined
 * @param   qosCap    Bits are set according to capabilities
 * @return  0 - If AP does not assert HCF capability & 1 - otherwise
 */
void
limExtractApCapability(tpAniSirGlobal pMac, tANI_U8 *pIE, tANI_U16 ieLen,
                       tANI_U8 *qosCap, tANI_U16 *propCap, tANI_U8 *uapsd, 
                       tPowerdBm *localConstraint,
                       tpPESession psessionEntry
                       )
{
    tSirProbeRespBeacon *pBeaconStruct;
#if !defined WLAN_FEATURE_VOWIFI
    tANI_U32            localPowerConstraints = 0;
#endif
    
    pBeaconStruct = vos_mem_malloc(sizeof(tSirProbeRespBeacon));

    if ( NULL == pBeaconStruct )
    {
        limLog(pMac, LOGE, FL("Unable to allocate memory in limExtractApCapability") );
        return;
    }

    vos_mem_set( (tANI_U8 *) pBeaconStruct, sizeof(tSirProbeRespBeacon), 0);
    *qosCap = 0;
    *propCap = 0;
    *uapsd = 0;
    PELOG3(limLog( pMac, LOG3,
        FL("In limExtractApCapability: The IE's being received are:"));
    sirDumpBuf( pMac, SIR_LIM_MODULE_ID, LOG3, pIE, ieLen );)
    if (sirParseBeaconIE(pMac, pBeaconStruct, pIE, (tANI_U32)ieLen) == eSIR_SUCCESS)
    {
        if (pBeaconStruct->wmeInfoPresent || pBeaconStruct->wmeEdcaPresent)
            LIM_BSS_CAPS_SET(WME, *qosCap);
        if (LIM_BSS_CAPS_GET(WME, *qosCap) && pBeaconStruct->wsmCapablePresent)
            LIM_BSS_CAPS_SET(WSM, *qosCap);
        if (pBeaconStruct->propIEinfo.aniIndicator &&
            pBeaconStruct->propIEinfo.capabilityPresent)
            *propCap = pBeaconStruct->propIEinfo.capability;
        if (pBeaconStruct->HTCaps.present)
            pMac->lim.htCapabilityPresentInBeacon = 1;
        else
            pMac->lim.htCapabilityPresentInBeacon = 0;

#ifdef WLAN_FEATURE_11AC
        VOS_TRACE(VOS_MODULE_ID_PE, VOS_TRACE_LEVEL_INFO_MED,
            "***beacon.VHTCaps.present*****=%d",pBeaconStruct->VHTCaps.present);
        VOS_TRACE(VOS_MODULE_ID_PE, VOS_TRACE_LEVEL_INFO_MED,
           "***beacon.SU Beamformer Capable*****=%d",pBeaconStruct->VHTCaps.suBeamFormerCap);

        if ( pBeaconStruct->VHTCaps.present && pBeaconStruct->VHTOperation.present)
        {
            psessionEntry->vhtCapabilityPresentInBeacon = 1;
            psessionEntry->apCenterChan = pBeaconStruct->VHTOperation.chanCenterFreqSeg1;
            psessionEntry->apChanWidth = pBeaconStruct->VHTOperation.chanWidth;
        }
        else
        {
            psessionEntry->vhtCapabilityPresentInBeacon = 0;
        }
#endif
        // Extract the UAPSD flag from WMM Parameter element
        if (pBeaconStruct->wmeEdcaPresent)
            *uapsd = pBeaconStruct->edcaParams.qosInfo.uapsd;
#if defined FEATURE_WLAN_CCX
        /* If there is Power Constraint Element specifically,
         * adapt to it. Hence there is else condition check
         * for this if statement.
         */
        if ( pBeaconStruct->ccxTxPwr.present)
        {
            *localConstraint = pBeaconStruct->ccxTxPwr.power_limit;
        }
#endif
        if (pBeaconStruct->powerConstraintPresent)
#if 0
        //Remove this check. This function is expected to return localPowerConsraints
        //and it should just do that. Check for 11h enabled or not can be done at the caller
#if defined WLAN_FEATURE_VOWIFI
          && ( pMac->lim.gLim11hEnable
           || pMac->rrm.rrmPEContext.rrmEnable
#endif
#endif
        {
#if defined WLAN_FEATURE_VOWIFI 
           *localConstraint -= pBeaconStruct->localPowerConstraint.localPowerConstraints;
#else
           localPowerConstraints = (tANI_U32)pBeaconStruct->localPowerConstraint.localPowerConstraints;
#endif
        }
#if !defined WLAN_FEATURE_VOWIFI
        if (cfgSetInt(pMac, WNI_CFG_LOCAL_POWER_CONSTRAINT, localPowerConstraints) != eSIR_SUCCESS)
        {
            limLog(pMac, LOGP, FL("Could not update local power constraint to cfg."));
        }
#endif
    }
    vos_mem_free(pBeaconStruct);
    return;
} /****** end limExtractApCapability() ******/

/**
 * limGetHTCBState
 *
 *FUNCTION:
 * This routing provides the translation of Airgo Enum to HT enum for determining 
 * secondary channel offset.
 * Airgo Enum is required for backward compatibility purposes.
 *
 *
 *NOTE:
 *
 * @param  pMac - Pointer to Global MAC structure
 * @return The corresponding HT enumeration
 */
ePhyChanBondState  limGetHTCBState(ePhyChanBondState aniCBMode) 
{
    switch ( aniCBMode )
    {
#ifdef WLAN_FEATURE_11AC
        case PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_LOW:
        case PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_CENTERED:
        case PHY_QUADRUPLE_CHANNEL_20MHZ_HIGH_40MHZ_HIGH:
#endif
        case PHY_DOUBLE_CHANNEL_HIGH_PRIMARY:
        return PHY_DOUBLE_CHANNEL_HIGH_PRIMARY;
#ifdef WLAN_FEATURE_11AC
        case PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_LOW:
        case PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_CENTERED:
        case PHY_QUADRUPLE_CHANNEL_20MHZ_LOW_40MHZ_HIGH:
#endif
        case PHY_DOUBLE_CHANNEL_LOW_PRIMARY:
        return PHY_DOUBLE_CHANNEL_LOW_PRIMARY;
#ifdef WLAN_FEATURE_11AC
        case PHY_QUADRUPLE_CHANNEL_20MHZ_CENTERED_40MHZ_CENTERED:
           return PHY_SINGLE_CHANNEL_CENTERED;
#endif
        default :
           return PHY_SINGLE_CHANNEL_CENTERED;
     }
}

 /*
 * limGetStaPeerType
 *
 *FUNCTION:
 * Based on a combination of the following -
 * 1) tDphHashNode.aniPeer
 * 2) tDphHashNode.propCapability
 * this API determines if a given STA is an ANI peer or not
 *
 *LOGIC:
 *
 *ASSUMPTIONS:
 *
 *NOTE:
 *
 * @param  pMac - Pointer to Global MAC structure
 * @param  pStaDs - Pointer to the tpDphHashNode of the STA
 *         under consideration
 * @return tStaRateMode
 */
tStaRateMode limGetStaPeerType( tpAniSirGlobal pMac,
    tpDphHashNode pStaDs,
    tpPESession   psessionEntry)
{
tStaRateMode staPeerType = eSTA_11b;
  // Determine the peer-STA type
  if( pStaDs->aniPeer )
  {
    if(PROP_CAPABILITY_GET( TAURUS, pStaDs->propCapability ))
        staPeerType = eSTA_TAURUS;
    else if( PROP_CAPABILITY_GET( TITAN, pStaDs->propCapability ))
        staPeerType = eSTA_TITAN;
    else
        staPeerType = eSTA_POLARIS;
  }
#ifdef WLAN_FEATURE_11AC
  else if(pStaDs->mlmStaContext.vhtCapability)
      staPeerType = eSTA_11ac;
#endif
  else if(pStaDs->mlmStaContext.htCapability)
        staPeerType = eSTA_11n;
  else if(pStaDs->erpEnabled)
        staPeerType = eSTA_11bg;
  else if(psessionEntry->limRFBand == SIR_BAND_5_GHZ)
        staPeerType = eSTA_11a;
  return staPeerType;
}

