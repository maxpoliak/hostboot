/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/common/framework/resolution/prdfCalloutMap.H $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2016                        */
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

#ifndef __prdfCalloutMap_H
#define __prdfCalloutMap_H

/** @file prdfCalloutMap.H */

#include <prdfEnums.H>

//------------------------------------------------------------------------------

#ifndef PRDF_SIM_ENUMS // needed to compile prdfsimEnumMap.C
namespace PRDF
{
#endif

//------------------------------------------------------------------------------

#ifdef PRDF_RULE_COMPILER_ENUMS

    // Used only in the rule compiler. Sets up a map between string keywords in
    // the rule code to the enum name.

    #define PRDF_SYM_CALLOUT_MAP_BEGIN \
        struct SymCallout_t { const char * str; SymbolicFru val; }; \
        SymCallout_t symCalloutArray[] = \
        {

    #define PRDF_SYM_CALLOUT_ALIAS(name1, name2) \
            { #name1 , name1 },

    #define PRDF_SYM_CALLOUT_MAP_END \
            { NULL , (SymbolicFru) 0 } \
        };

#elif defined PRDF_SIM_ENUMS

    // included from prdfsimEnumMap.C, nothing for now

#else

    #define PRDF_SYM_CALLOUT_MAP_BEGIN \
        enum SymbolicFru \
        {

    #define PRDF_SYM_CALLOUT_ALIAS(n1,n2) \
            n1 = n2,

    #define PRDF_SYM_CALLOUT_MAP_END \
        };

#endif

PRDF_SYM_CALLOUT_MAP_BEGIN

PRDF_SYM_CALLOUT_ALIAS( AllProcessors,              EPUB_PRC_ALL_PROCS         )
PRDF_SYM_CALLOUT_ALIAS( SP_CODE,                    EPUB_PRC_SP_CODE           )
PRDF_SYM_CALLOUT_ALIAS( SystemSW_Code_ENUM,         EPUB_PRC_PHYP_CODE         )
PRDF_SYM_CALLOUT_ALIAS( NextLevelSupport_ENUM,      EPUB_PRC_LVL_SUPP          )
PRDF_SYM_CALLOUT_ALIAS( PassiveFabric_OffNode_ENUM, EPUB_PRC_PROC_AB_BUS       )
PRDF_SYM_CALLOUT_ALIAS( ErrorCausedByMemUE_ENUM,    EPUB_PRC_MEMORY_UE         )
PRDF_SYM_CALLOUT_ALIAS( TODProcInterface_Enum,      EPUB_PRC_TOD_CLOCK_ERR     )
PRDF_SYM_CALLOUT_ALIAS( CoolingSystemError_ENUM,    EPUB_PRC_COOLING_SYSTEM_ERR)
PRDF_SYM_CALLOUT_ALIAS( GPUIsolation_ENUM,    EPUB_PRC_GPU_ISOLATION_PROCEDURE )

PRDF_SYM_CALLOUT_MAP_END

#undef PRDF_SYM_CALLOUT_MAP_BEGIN
#undef PRDF_SYM_CALLOUT_ALIAS
#undef PRDF_SYM_CALLOUT_MAP_END

//------------------------------------------------------------------------------

#ifdef PRDF_RULE_COMPILER_ENUMS

    // Used only in the rule compiler. Sets up a map between string keywords in
    // the rule code to the enum name.

    #define PRDF_CALLOUT_PRIORITY_MAP_BEGIN \
        struct CalloutPriority_t \
        { const char * str; CalloutPriorityEnum val; }; \
        CalloutPriority_t calloutPriorityArray[] = \
        {

    #define PRDF_CALLOUT_PRIORITY_ALIAS(name1, name2) \
            { #name1 , name1 },

    #define PRDF_CALLOUT_PRIORITY_MAP_END \
            { NULL , (CalloutPriorityEnum) 0 } \
        };

#elif defined PRDF_SIM_ENUMS

    // included from prdfsimEnumMap.C, nothing for now

#else

    #define PRDF_CALLOUT_PRIORITY_MAP_BEGIN \
        enum CalloutPriorityEnum \
        {

    #define PRDF_CALLOUT_PRIORITY_ALIAS(n1,n2) \
            n1 = n2,

    #define PRDF_CALLOUT_PRIORITY_MAP_END \
        };

#endif

PRDF_CALLOUT_PRIORITY_MAP_BEGIN

PRDF_CALLOUT_PRIORITY_ALIAS( MRU_HIGH, SRCI_PRIORITY_HIGH )
PRDF_CALLOUT_PRIORITY_ALIAS( MRU_MED,  SRCI_PRIORITY_MED  )
PRDF_CALLOUT_PRIORITY_ALIAS( MRU_MEDA, SRCI_PRIORITY_MEDA )
PRDF_CALLOUT_PRIORITY_ALIAS( MRU_MEDB, SRCI_PRIORITY_MEDB )
PRDF_CALLOUT_PRIORITY_ALIAS( MRU_MEDC, SRCI_PRIORITY_MEDC )
PRDF_CALLOUT_PRIORITY_ALIAS( MRU_LOW,  SRCI_PRIORITY_LOW  )

PRDF_CALLOUT_PRIORITY_MAP_END

#undef PRDF_CALLOUT_PRIORITY_MAP_BEGIN
#undef PRDF_CALLOUT_PRIORITY_ALIAS
#undef PRDF_CALLOUT_PRIORITY_MAP_END

//------------------------------------------------------------------------------

#ifndef PRDF_SIM_ENUMS
} // end namespace PRDF
#endif

#endif // __prdfCalloutMap_H

