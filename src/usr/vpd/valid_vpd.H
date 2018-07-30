/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/vpd/valid_vpd.H $                                     */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2013,2015                        */
/* [+] Google Inc.                                                        */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
#ifndef _VALID_VPD_H
#define _VALID_VPD_H

#include <list>
#include <pnor/pnorif.H>
#include <devicefw/driverif.H>
#include "vpd.H"

/** @file valid_vpd.H
 *  @brief Provides base support for i/p-Series style IBM VPD
 */

namespace VPD
{
    /** @class ValidatorVPD
     *  This class contains methods for verifying the vendor specified
     *  records in VPD for some devices in the server system.
     *
     */
    class ValidatorVPD
    {
    public:
        /**
        * @brief typdef for vpdKeywordsSize values
        */
        enum vpdKeywordsSize
        {
            VINI_RT_SZ = 4,  // VINI Record name
            VINI_DR_SZ = 16, // Card description
            VINI_CE_SZ = 1,  // Card identification number (CCIN) extension
            VINI_VZ_SZ = 2,  // Overall VPD version; VPD change history
            VINI_FN_SZ = 7,  // Card FRU number
            VINI_PN_SZ = 7,  // Card part number
            VINI_SN_SZ = 12, // Card serial number
            VINI_CC_SZ = 4,  // Card identification number (CCIN)
            VINI_HE_SZ = 4,  // Hardware EC version
            VINI_CT_SZ = 4,  // Card type
            VINI_HW_SZ = 2,  // Hardware version
            VINI_B3_SZ = 6,  // Aunique FRU
            VINI_B4_SZ = 1,  // Manufacturing FRU control (MFC)
            VINI_B7_SZ = 12, // Test-specific information
        };

        /**
        * @brief enum for SPD Keywords
        */
        enum
        {
            YADRO_SPD_MANUFACTORY_ID = 0x80CE,
        };

        /**
         * @brief This function will perform the steps required to do a validation
         *       vendor specific record in VPD for all devices in system.
         *
         * @return ValidatorVPD - this class
         */
        ValidatorVPD  &validateVpdSpd();

        /**
         * @brief This function return number of unsupported devices, which has not
         *       vendor specific record in VPD.
         *
         * @return uint64_t - number of unsuported devices
         */
        ALWAYS_INLINE
        uint64_t  unsupportedNumGet( )
        {
            return  static_cast<uint64_t>(unsupportedTargetList.size());
        }

        /**
         * @brief This function deconfigures all unsupported devices and adds
         *       them to the GARD list.
         *
         * @return errlHndl_t - NULL if successful, otherwise a pointer to the
         *       error log.
         */
        errlHndl_t  unsupportedToGard();

    private:
        /**
         * @brief This function will perform the steps required to do a validation
         *       records in Hostboot VPD data segment.
         *
         * @param[in] i_target - Target device
         *
         * @param[in] reference_val - Reference value for comparison
         *
         * @param [in/out] buffer - Pointer to the data that was read from
         *       the target device. This parameter, when set to NULL, will return
         *       the keyword size value in io_buflen.
         *
         * @param [in/out] buflen - Length of the buffer to be read or written
         *       to/from the target.  This value should indicate the size of the
         *       <buffer> parameter that has been allocated.  Being returned it
         *       will indicate the number of valid bytes in the buffer being
         *       returned. This parameter will contain the size of a keyword when
         *       the buffer parameter is passed in NULL.
         *
         * @param [in] record - Record in VPD/SPD
         *
         * @param [in] keyword - Keyword in VPD/SPD
         *
         * @return errlHndl_t - NULL if successful, otherwise a pointer to the
         *       error log.
         */
        errlHndl_t  validateVpdKeyword(TARGETING::Target* target,
                                        const void *reference_val,
                                        void* buffer,
                                        size_t& buflen,
                                        DeviceFW::AccessType accessType,
                                        uint64_t record,
                                        uint64_t keyword = 0 );
        /**
         * @brief This function will perform the steps required to do a validation
         *       record in VPD for specified target device.
         *
         * @param[in] i_target - Target device
         *
         * @param[in] severity - severity of creating SEL
         *
         * @param [in] info - Pointer to the information string
         *
         * @return errlHndl_t - otherwise a pointer to the error log (SEL).
         */
        errlHndl_t  createSEL(TARGETING::Target* target,
                              const ERRORLOG::errlSeverity_t severity,
                              const char * info);

    protected:
        TARGETING::TargetHandleList  unsupportedTargetList;
    };
}; //end vpd namespace

#endif /* _VPD_IPVPD_H */
