/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/include/p8_istep_num.H $                     */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2012,2015                        */
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
#ifndef __P8_ISTEP_NUM_H
#define __P8_ISTEP_NUM_H

// $Id: p8_istep_num.H,v 1.29 2015/05/28 20:45:55 jmcgill Exp $

/// Istep number encoding for all SEEPROM and PNOR procedures.  Used to update
/// the SBEVITAL register to record procedure progress and to create unique
/// hooki bind points on procedure completion.

CONST_UINT64_T(proc_sbe_enable_seeprom_istep_num,           ULL(0x0101));

CONST_UINT64_T(proc_sbe_security_setup_istep_num,           ULL(0x0201));
CONST_UINT64_T(proc_sbe_standalone_setup_istep_num,         ULL(0x0202));
CONST_UINT64_T(proc_sbe_tp_chiplet_init1_istep_num,         ULL(0x0203));
CONST_UINT64_T(proc_sbe_tp_ld_image_istep_num,              ULL(0x0204));
CONST_UINT64_T(proc_sbe_npll_initf_istep_num,               ULL(0x0205));
CONST_UINT64_T(proc_sbe_npll_setup_istep_num,               ULL(0x0206));
CONST_UINT64_T(proc_sbe_tp_switch_gears_istep_num,          ULL(0x0207));
CONST_UINT64_T(proc_sbe_tp_chiplet_init2_istep_num,         ULL(0x0208));
CONST_UINT64_T(proc_sbe_tp_arrayinit_istep_num,             ULL(0x0209));
CONST_UINT64_T(proc_sbe_tp_initf_istep_num,                 ULL(0x020A));
CONST_UINT64_T(proc_sbe_tp_chiplet_init3_istep_num,         ULL(0x020B));
CONST_UINT64_T(proc_sbe_chiplet_init_istep_num,             ULL(0x020C));
CONST_UINT64_T(proc_sbe_nest_skewadjust_istep_num,          ULL(0x020D));
CONST_UINT64_T(proc_sbe_arrayinit_istep_num,                ULL(0x020E));
CONST_UINT64_T(proc_sbe_setup_evid_istep_num,               ULL(0x020F));
CONST_UINT64_T(proc_sbe_initf_istep_num,                    ULL(0x0210));
CONST_UINT64_T(proc_sbe_pb_startclocks_istep_num,           ULL(0x0211));
CONST_UINT64_T(proc_sbe_scominit_istep_num,                 ULL(0x0212));
CONST_UINT64_T(proc_sbe_fabricinit_istep_num,               ULL(0x0213));
CONST_UINT64_T(proc_sbe_check_master_istep_num,             ULL(0x0214));
CONST_UINT64_T(proc_sbe_select_ex_istep_num,                ULL(0x0215));
CONST_UINT64_T(proc_sbe_run_exinit_istep_num,               ULL(0x0216));

CONST_UINT64_T(proc_sbe_pnor_setup_istep_num,               ULL(0x0301));

CONST_UINT64_T(proc_sbe_ex_chiplet_reset_istep_num,         ULL(0x0401));
CONST_UINT64_T(proc_sbe_ex_gptr_time_initf_istep_num,       ULL(0x0402));
CONST_UINT64_T(proc_sbe_ex_core_gptr_time_initf_istep_num,  ULL(0x0403));
CONST_UINT64_T(proc_sbe_ex_dpll_initf_istep_num,            ULL(0x0404));
CONST_UINT64_T(proc_sbe_ex_dpll_setup_istep_num,            ULL(0x0405));
CONST_UINT64_T(proc_sbe_ex_chiplet_init_istep_num,          ULL(0x0406));
CONST_UINT64_T(proc_sbe_ex_repair_initf_istep_num,          ULL(0x0407));
CONST_UINT64_T(proc_sbe_ex_core_repair_initf_istep_num,     ULL(0x0408));
CONST_UINT64_T(proc_sbe_ex_arrayinit_istep_num,             ULL(0x0409));
CONST_UINT64_T(proc_sbe_ex_initf_istep_num,                 ULL(0x040A));
CONST_UINT64_T(proc_sbe_ex_core_initf_istep_num,            ULL(0x040B));
CONST_UINT64_T(proc_sbe_ex_do_manual_inits_istep_num,       ULL(0x040C));
CONST_UINT64_T(proc_sbe_ex_startclocks_istep_num,           ULL(0x040D));
CONST_UINT64_T(proc_sbe_ex_scominit_istep_num,              ULL(0x040E));
CONST_UINT64_T(proc_sbe_ex_core_scominit_istep_num,         ULL(0x040F));
CONST_UINT64_T(proc_sbe_ex_init_escape_istep_num,           ULL(0x0410));
CONST_UINT64_T(proc_sbe_ex_sp_runtime_scom_istep_num,       ULL(0x0411));
CONST_UINT64_T(proc_sbe_ex_occ_runtime_scom_istep_num,      ULL(0x0412));
CONST_UINT64_T(proc_sbe_ex_host_runtime_scom_istep_num,     ULL(0x0413));

CONST_UINT64_T(proc_sbe_enable_pnor_istep_num,              ULL(0x0500));
CONST_UINT64_T(proc_sbe_lco_loader_istep_num,               ULL(0x0501));
CONST_UINT64_T(proc_sbe_instruct_start_istep_num,           ULL(0x0502));

//NOTE: The following values must stay constant as HB looks for them
CONST_UINT64_T(proc_sbe_trigger_winkle_istep_num,            ULL(0x0F01));
CONST_UINT64_T(proc_sbe_scan_service_istep_num,              ULL(0x0FA0));
CONST_UINT64_T(proc_sbe_intr_service_istep_num,              ULL(0x0FB0));
CONST_UINT64_T(proc_sbe_check_master_magic_istep_num,        ULL(0x02FF));
CONST_UINT64_T(proc_sbe_ex_host_runtime_scom_magic_istep_num,ULL(0x04FF));
CONST_UINT64_T(proc_sbe_enable_pnor_magic_istep_num,         ULL(0x05FF));

// The following record progress through the SLW process and only are 
// logged in the virtual SBEVITAL in the OCC SRAM;  these are not recorded
// in the real SBEVITAL as they are not executed during the master core IPL.
CONST_UINT64_T(slw_fast_sleep_enter_istep_num,              ULL(0x0F80));
CONST_UINT64_T(slw_fast_sleep_exit_istep_num,               ULL(0x0F81));
CONST_UINT64_T(slw_fast_winkle_enter_istep_num,             ULL(0x0F82));
CONST_UINT64_T(slw_fast_winkle_exit_istep_num,              ULL(0x0F83));
CONST_UINT64_T(slw_deep_sleep_enter_istep_num,              ULL(0x0F84));
CONST_UINT64_T(slw_deep_sleep_exit_istep_num,               ULL(0x0F85));
CONST_UINT64_T(slw_deep_winkle_enter_istep_num,             ULL(0x0F86));
CONST_UINT64_T(slw_deep_winkle_exit_istep_num,              ULL(0x0F87));


#define PROC_SBE_TRIGGER_WINKLE_ISTEP_NUM proc_sbe_trigger_winkle_istep_num
#define PROC_SBE_CHECK_MASTER_ISTEP_NUM proc_sbe_check_master_istep_num
#define PROC_SBE_CHECK_MASTER_MAGIC_ISTEP_NUM proc_sbe_check_master_magic_istep_num
#define PROC_SBE_ENABLE_PNOR_ISTEP_NUM proc_sbe_enable_pnor_istep_num
#define PROC_SBE_ENABLE_PNOR_MAGIC_ISTEP_NUM proc_sbe_enable_pnor_magic_istep_num
#define PROC_SBE_EX_HOST_RUNTIME_SCOM_ISTEP_NUM proc_sbe_ex_host_runtime_scom_istep_num
#define PROC_SBE_EX_HOST_RUNTIME_SCOM_MAGIC_ISTEP_NUM proc_sbe_ex_host_runtime_scom_magic_istep_num
#define PROC_SBE_TP_LD_IMAGE_ISTEP_NUM proc_sbe_tp_ld_image_istep_num
#define PROC_SBE_SCAN_SERVICE_ISTEP_NUM proc_sbe_scan_service_istep_num
#define PROC_SBE_INTR_SERVICE_ISTEP_NUM proc_sbe_intr_service_istep_num

#endif  // __P8_ISTEP_NUM_H
