/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/vpd/valid_vpd.C $                                     */
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
// ----------------------------------------------
// Includes
// ----------------------------------------------

#include <stdio.h>
#include <trace/interface.H>
#include <errl/errlentry.H>
#include <errl/errlmanager.H>
#include <errl/errludtarget.H>
#include <errl/errludstring.H>
#include <vpd/vpdreasoncodes.H>
#include <initservice/initserviceif.H>
#include <hwas/common/deconfigGard.H>
#include <devicefw/driverif.H>
#include <hwas/hwasPlat.H>
#include <sys/mm.h>
#include "vpd.H"
#include "mvpd.H"
#include "cvpd.H"
#include "pvpd.H"
#include "spd.H"
#include "ipvpd.H"
#include "valid_vpd.H"
#include <map>

#include <stdarg.h>
#include <builtins.h>

// ----------------------------------------------
// Namespaces
// ----------------------------------------------
using namespace VPD;
using namespace TARGETING;

// ----------------------------------------------
// Trace definitions
// ----------------------------------------------
extern trace_desc_t* g_trac_vpd;

// ----------------------------------------------
// These static variables are used as reference
// parameters for comparison.
// ----------------------------------------------
static const char * vpdYadroProcDR = "PROCESSOR MODULE";
static const char * vpdYadroMemBuffDR = "xxxGB DDR4 MEMCD";
static uint16_t     dimmRefId;

// ------------------------------------------------------------------
// ValidatorVPD::validateVpdSpd
// ------------------------------------------------------------------
ValidatorVPD& ValidatorVPD::validateVpdSpd( )
{
    errlHndl_t   err = NULL;
    char         buffVpdSector[ VINI_DR_SZ + 1 ];
    size_t       vpdSectorSize = 0;

    TargetHandleList::const_iterator it;
    TargetHandleList  targetList;

    TRACFCOMP( g_trac_vpd, ENTER_MRK"validateVpdSpd() " );
    memset(buffVpdSector, 0, sizeof(buffVpdSector));


    vpdSectorSize = VINI_DR_SZ;
    TRACFCOMP( g_trac_vpd,
               "validateVpdSpd(): "
               "Verification VPD of all processors" );
    // Get all OpenPOWER Processors and create a new list
    getAllChips(targetList, TYPE_PROC);
    for ( it = targetList.begin(); it != targetList.end(); ++it )
    {
        // Check record of card description for all processors
        err = validateVpdKeyword(*it,
                                 vpdYadroProcDR,
                                 buffVpdSector,
                                 vpdSectorSize,
                                 DeviceFW::MVPD,
                                 static_cast<uint64_t>(MVPD::VINI),
                                 static_cast<uint64_t>(MVPD::DR)   );
        if ( err )
        {            
            TRACFCOMP( g_trac_vpd,
                       ERR_MRK
                       "validateVpdSpd(): "
                       "The VPD of all functional PROCESSORs has failed with an error");
            errlCommit(err, VPD_COMP_ID);
            unsupportedTargetList.push_back(*it);
        }
    } // end of PROC parsing


    TRACFCOMP( g_trac_vpd,
               "validateVpdSpd(): "
               "Verification VPD of all functional Centaur chips" );
    vpdSectorSize = VINI_DR_SZ;
    // Get the functional Centaur chips
    targetList.clear();
    getAllChips(targetList, TYPE_MEMBUF);
    for ( it = targetList.begin(); it != targetList.end(); ++it )
    {
        // Check record of card description for all CENTAUR
        err = validateVpdKeyword(*it,
                                 vpdYadroMemBuffDR,
                                 buffVpdSector,
                                 vpdSectorSize,
                                 DeviceFW::CVPD,
                                 static_cast<uint64_t>(CVPD::VINI),
                                 static_cast<uint64_t>(CVPD::DR));
        if ( err )
        {
            TRACFCOMP( g_trac_vpd,
                       ERR_MRK
                       "validateVpdSpd(): "
                       "The VPD of all functional CENTAURs has failed with an error");
            errlCommit(err, VPD_COMP_ID);
            unsupportedTargetList.push_back(*it);
        }
    } // end of CENTAURs parsing


    // SPD DIMM contains entries in little endian
    dimmRefId = htole16(YADRO_SPD_MANUFACTORY_ID);
    vpdSectorSize = sizeof(dimmRefId);
    TRACFCOMP( g_trac_vpd,
               "validateVpdSpd(): "
               "Verification SPD of all dimm" );
    // Let's create a separate list for all DIMM,
    // which are used in the system.
    targetList.clear();
    getAllLogicalCards(targetList, TYPE_DIMM);
    for ( it = targetList.begin();
          it != targetList.end();
          ++it )
    {
        // Verification of the vendor record for all DIMM
        err = validateVpdKeyword(*it,
                                 &dimmRefId,
                                 buffVpdSector,
                                 vpdSectorSize,
                                 DeviceFW::SPD,
                                 static_cast<uint64_t>(SPD::MODULE_MANUFACTURER_ID));
        if ( err )
        {
            TRACFCOMP( g_trac_vpd,
                       ERR_MRK
                       "validateVpdSpd(): "
                       "The VPD of all functional DIMMs has failed with an error");
            errlCommit(err, VPD_COMP_ID);
            unsupportedTargetList.push_back(*it);
        }
    } // end of DIMM parsing


    TRACFCOMP( g_trac_vpd, EXIT_MRK"validateVpdSpd() " );
    return *this;
}

// ------------------------------------------------------------------
// ValidatorVPD::validateVpdKeyword
// ------------------------------------------------------------------
errlHndl_t  ValidatorVPD::validateVpdKeyword( Target* target,
                                              const void* reference_val,
                                              void* buffer,
                                              size_t& buflen,
                                              DeviceFW::AccessType accessType,
                                              uint64_t record,
                                              uint64_t keyword )
{
    errlHndl_t  err = NULL;

    TRACFCOMP( g_trac_vpd, ENTER_MRK"validateVpdKeyword() " );
    // Read Keyword from the special VPD-HDATA section in PNOR
    switch (accessType)
    {
    case DeviceFW::MVPD:
    case DeviceFW::CVPD:
    case DeviceFW::PVPD:
        err = DeviceFW::deviceOp( DeviceFW::READ,
                                  target,
                                  buffer,
                                  buflen,
                                  accessType,
                                  record,
                                  keyword,
                                  SEEPROM );
        break;

    case DeviceFW::SPD:
        err = DeviceFW::deviceOp( DeviceFW::READ,
                                  target,
                                  buffer,
                                  buflen,
                                  DeviceFW::SPD,
                                  record,
                                  SEEPROM );
        break;

    default:
        err = createSEL( target,
                         ERRORLOG::ERRL_SEV_UNRECOVERABLE,
                         "Incorrent <accessType> parameter!" );
        break;
    }

    if( err )
    {
        EntityPath entPath = (target)->getAttr<ATTR_PHYS_PATH>();
        TRACFCOMP( g_trac_vpd,
                   ERR_MRK"validateVpdKeyword(): "
                   "failure reading Record VPD/SPD from target %s, RC=%.4X",
                   entPath.toString(),
                   err->reasonCode() );
    }
    else if ( memcmp(reference_val, buffer, buflen) )
    {
        // The entry in vpd does not match.
        err = createSEL( target,
                         ERRORLOG::ERRL_SEV_UNRECOVERABLE,
                         "TARGET IS UNSUPPORTED ON THIS SYSTEM!" );
    }

    TRACFCOMP( g_trac_vpd, EXIT_MRK"validateVpdKeyword() " );
    return  err;
}

// ------------------------------------------------------------------
// ValidatorVPD::createSEL
// ------------------------------------------------------------------
errlHndl_t  ValidatorVPD::createSEL( Target * target,
                                 const ERRORLOG::errlSeverity_t severity,
                                 const char * info )
{
    errlHndl_t  err;
    err = new ERRORLOG::ErrlEntry( severity,
                                   VPD::VPD_GET_PN_AND_SN,
                                   VPD::VPD_MOD_SPECIFIC_UNSUPPORTED,
                                   target->getAttr<ATTR_TYPE>(),
                                   target->getAttr<ATTR_HUID>(),
                                   false );

    // Add a details: target descriptor and string of information
    ERRORLOG::ErrlUserDetailsTarget(target).addToLog(err);
    ERRORLOG::ErrlUserDetailsString(info).addToLog(err);
    return  err;
}

// ------------------------------------------------------------------
// ValidatorVPD::unsupportedToGard
// ------------------------------------------------------------------
errlHndl_t  ValidatorVPD::unsupportedToGard( )
{
    errlHndl_t  gardErr = NULL;
    uint64_t    devicesNum;

    HWAS::DeconfigGard::GardRecords_t  gardRecords;
    TargetHandleList::const_iterator   it;


    TRACFCOMP( g_trac_vpd, ENTER_MRK"unsupportedToGard() " );

    do
    {
        // Get number of unsupported devices
        devicesNum = unsupportedNumGet();
        if ( !devicesNum )
        {
            TRACFCOMP( g_trac_vpd,
                       "unsupportedToGard(): "
                       "List of unsupported devices is empty" );
            break;
        }

        TRACFCOMP( g_trac_vpd,
                   "unsupportedToGard(): "
                   "List of unsupported devices has %llu entries",
                   devicesNum );

        for ( it = unsupportedTargetList.begin();
              it != unsupportedTargetList.end();
              ++it )
        {
            // Create a GARD Record for the current target
            gardErr = HWAS::theDeconfigGard().
                    platCreateGardRecord(*it, 0x23, HWAS::GARD_Predictive);
            if ( gardErr )
            {
               TRACFCOMP( g_trac_vpd,
                          ERR_MRK
                          "unsupportedToGard(): "
                          "Error with creating a new entry in GARD list" );
               break;
            }

            // Deconfigure this target
            gardErr = HWAS::theDeconfigGard().deconfigureTarget(**it, 0xA3);
            if  ( gardErr )
            {
                TRACFCOMP( g_trac_vpd,
                           ERR_MRK
                           "unsupportedToGard(): "
                           "Error with configuring the target" );
                break;
            }

            // Check if the device is added to the sheet.
            // Get the GARD Records for the target
            gardErr = HWAS::theDeconfigGard().getGardRecords(*it, gardRecords);
            if  ( gardErr )
            {
                TRACFCOMP( g_trac_vpd,
                           ERR_MRK
                           "unsupportedToGard(): "
                           "Error with reading record from GARD list" );
                break;
            }

            if ( !gardRecords.size() )
            {
                TRACFCOMP( g_trac_vpd,
                           ERR_MRK
                           "unsupportedToGard(): "
                           "The device was not added to the GARD list" );
                // The entry in vpd does not match.
                gardErr = createSEL( *it,
                                     ERRORLOG::ERRL_SEV_CRITICAL_SYS_TERM,
                                     "ERROR WITH OPERATION IN GARD LIST!" );
                break;
            }

            // Check if the device was nonfunctional.
            if ( (*it)->getAttr<ATTR_HWAS_STATE>().functional )
            {
                TRACFCOMP( g_trac_vpd,
                           ERR_MRK
                           "unsupportedToGard(): "
                           "The device functions after being placed in a sheet" );
                break;
            }

        } // endfor

        if ( gardErr )
        {
            // Get logical path of current target
            EntityPath entPath = (*it)->getAttr<ATTR_PHYS_PATH>();
            TRACFCOMP( g_trac_vpd,
                       "unsupportedToGard(): "
                       "Error deconfiguring target %s. RC=%.4X",
                       entPath.toString(),
                       gardErr->reasonCode() );

            // Commit new error
            errlCommit(gardErr, VPD_COMP_ID);
        }
    }
    while(false);

    TRACFCOMP( g_trac_vpd, EXIT_MRK"validateVpdKeyword() " );
    return  gardErr;
}
