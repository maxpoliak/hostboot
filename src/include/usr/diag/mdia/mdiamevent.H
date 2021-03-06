/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/diag/mdia/mdiamevent.H $                      */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2012,2014              */
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
#ifndef __MDIA_MDIAMEVENT_H
#define __MDIA_MDIAMEVENT_H

#include <diag/mdia/mdia.H>

/**
 * @file mdiamevent.H
 * @brief mdia maint command event
 */

namespace MDIA
{

/**
 * @brief MaintCommandEvent maint command event
 */
struct MaintCommandEvent
{
    /**
     * @brief type the type associated with the event
     */
    MaintCommandEventType type;

    /**
     * @brief target the target associated with the event
     */
    TARGETING::TargetHandle_t target;
};
}
#endif
