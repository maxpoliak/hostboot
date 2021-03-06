/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/hwpf/hwp/occ/occ_procedures/p8_pm_pba_firinit.C $     */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2013,2014                        */
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

// $Id: p8_pm_pba_firinit.C,v 1.22 2014/07/09 14:49:32 daviddu Exp $
// $Source: /afs/awd/projects/eclipz/KnowledgeBase/.cvsroot/eclipz/chips/p8/working/procedures/ipl/fapi/p8_pm_pba_firinit.C,v $
//------------------------------------------------------------------------------
// *! (C) Copyright International Business Machines Corp. 2011
// *! All Rights Reserved -- Property of IBM
// *! ***  ***
//------------------------------------------------------------------------------
// *! OWNER NAME: Pradeep CN         Email: pradeepcn@in.ibm.com
// *! OWNER NAME: Greg Still         Email: stillgs@us.ibm.com
/// \file p8_pm_pba_firinit.C
/// \brief Configure the PBA FIR
///
/// \verbatim
///
///   if RESET
///       masks all bits of the FIR k
///
///   else
///       using macros defined in p8_pm.H to establish the respective
///       mask/action bits in the relevant ecmdbuffer for one of the following
///       settings:
///           a) Masked
///           b) Recoverable Attention
///           c) Checkstop
///
///  Procedure Prereq:
///    o System clocks are running
///
/// \endverbatim
/// buildfapiprcd p8_pm_pba_firinit.C
//------------------------------------------------------------------------------


// ----------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------
#include <fapi.H>
#include "p8_scom_addresses.H"
#include "p8_pm_pba_firinit.H"

extern "C" {

using namespace fapi;

// ----------------------------------------------------------------------
// Constant definitions
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------

fapi::ReturnCode
p8_pm_pba_firinit(const fapi::Target&  i_target , uint32_t mode )
{

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// function: FAPI p8_pm_pba_firinit  HWP entry point
//           operates on chips passed in i_target argument to perform
//           desired settings of FIRS of PBA macro
// parameters: i_target        => chip target

// returns: FAPI_RC_SUCCESS if all specified operations complete successfully,
//          else return code for failing operation
//------------------------------------------------------------------------------

    fapi::ReturnCode    rc;
    ecmdDataBufferBase  fir(64);
    ecmdDataBufferBase  action_0(64);
    ecmdDataBufferBase  action_1(64);
    ecmdDataBufferBase  mask(64);
    ecmdDataBufferBase  data(64);
    uint32_t            e_rc = 0;
    uint64_t            attr_pm_pba_fir_mask;
    uint8_t             attr_pm_firinit_done_once_flag;

    FAPI_DBG("Executing p8_pm_pba_firinit  ....");
    do
    {
        if (mode == PM_RESET)
        {

            // -----------
            // SW260003
            // -----------
            rc = FAPI_ATTR_GET(ATTR_PM_FIRINIT_DONE_ONCE_FLAG, &i_target, attr_pm_firinit_done_once_flag);
            if (!rc.ok()) {
              FAPI_ERR("fapiGetAttribute of ATTR_PM_FIRINIT_DONE_ONCE_FLAG failed.");
              break;
            }
            if (attr_pm_firinit_done_once_flag == 1) { 
                rc = fapiGetScom(i_target, PBA_FIR_MASK_0x02010843, data );
                if (!rc.ok())
                {
                        FAPI_ERR("fapiGetScom(PBA_FIR_MASK_0x02010843) failed.");
                        break;
                }
                attr_pm_pba_fir_mask = data.getDoubleWord(0);
                rc = FAPI_ATTR_SET(ATTR_PM_PBA_FIR_MASK, &i_target, attr_pm_pba_fir_mask);
                if (!rc.ok()) {
                  FAPI_ERR("fapiSetAttribute of ATTR_PM_PBA_FIR_MASK failed");
                  break;
                }
            }    

            e_rc  = mask.flushTo1();
            if (e_rc)
            {
                rc.setEcmdError(e_rc);
                break;
            }

            //--******************************************************************************
            //-- PBA_FIR_MASK (W0_OR_45) (WR_43) (WO_AND_44)
            //--******************************************************************************
            rc = fapiPutScom(i_target, PBA_FIR_MASK_0x02010843, mask );
            if (rc)
            {
	            FAPI_ERR("fapiPutScom(PBA_FIR_MASK_0x02010843) failed.");
                break;
            }
        }
        else
        {
            e_rc |= fir.flushTo0();
            e_rc |= action_0.flushTo0();
            e_rc |= action_1.flushTo0();
            e_rc |= mask.flushTo0()    ;
            if (e_rc)
            {
                rc.setEcmdError(e_rc);
                break;
            }

            SET_RECOV_ATTN  (PBAFIR_OCI_APAR_ERR      ) ; // 0   PBAFIR_OCI_APAR_ERR
            SET_RECOV_ATTN  (PBAFIR_PB_RDADRERR_FW    ) ; // 1   PBAFIR_PB_RDADRERR_FW
            SET_RECOV_ATTN  (PBAFIR_PB_RDDATATO_FW    ) ; // 2   PBAFIR_PB_RDDATATO_FW
            SET_RECOV_ATTN  (PBAFIR_PB_SUE_FW         ) ; // 3   PBAFIR_PB_SUE_FW
            SET_RECOV_ATTN  (PBAFIR_PB_UE_FW          ) ; // 4   PBAFIR_PB_UE_FW
            SET_RECOV_ATTN  (PBAFIR_PB_CE_FW          ) ; // 5   PBAFIR_PB_CE_FW
            SET_RECOV_ATTN  (PBAFIR_OCI_SLAVE_INIT    ) ; // 6   PBAFIR_OCI_SLAVE_INIT
            SET_RECOV_ATTN  (PBAFIR_OCI_WRPAR_ERR     ) ; // 7   PBAFIR_OCI_WRPAR_ERR
            SET_FIR_MASKED  (PBAFIR_OCI_REREQTO       ) ; // 8   PBAFIR_OCI_REREQTO
            SET_FIR_MASKED  (PBAFIR_PB_UNEXPCRESP     ) ; // 9   PBAFIR_PB_UNEXPCRESP
            SET_RECOV_ATTN  (PBAFIR_PB_UNEXPDATA      ) ; // 10  PBAFIR_PB_UNEXPDATA
            SET_RECOV_ATTN  (PBAFIR_PB_PARITY_ERR     ) ; // 11  PBAFIR_PB_PARITY_ERR
            SET_RECOV_ATTN  (PBAFIR_PB_WRADRERR_FW    ) ; // 12  PBAFIR_PB_WRADRERR_FW
            SET_RECOV_ATTN  (PBAFIR_PB_BADCRESP       ) ; // 13  PBAFIR_PB_BADCRESP
            SET_RECOV_ATTN  (PBAFIR_PB_ACKDEAD_FW_RD  ) ; // 14  PBAFIR_PB_ACKDEAD_FW_RD
            SET_RECOV_ATTN  (PBAFIR_PB_CRESPTO        ) ; // 15  PBAFIR_PB_CRESPTO
            SET_FIR_MASKED  (PBAFIR_BCUE_SETUP_ERR    ) ; // 16  PBAFIR_BCUE_SETUP_ERR
            SET_FIR_MASKED  (PBAFIR_BCUE_PB_ACK_DEAD  ) ; // 17  PBAFIR_BCUE_PB_ACK_DEAD
            SET_FIR_MASKED  (PBAFIR_BCUE_PB_ADRERR    ) ; // 18  PBAFIR_BCUE_PB_ADRERR
            SET_FIR_MASKED  (PBAFIR_BCUE_OCI_DATERR   ) ; // 19  PBAFIR_BCUE_OCI_DATERR
            SET_FIR_MASKED  (PBAFIR_BCDE_SETUP_ERR    ) ; // 20  PBAFIR_BCDE_SETUP_ERR
            SET_FIR_MASKED  (PBAFIR_BCDE_PB_ACK_DEAD  ) ; // 21  PBAFIR_BCDE_PB_ACK_DEAD
            SET_FIR_MASKED  (PBAFIR_BCDE_PB_ADRERR    ) ; // 22  PBAFIR_BCDE_PB_ADRERR
            SET_FIR_MASKED  (PBAFIR_BCDE_RDDATATO_ERR ) ; // 23  PBAFIR_BCDE_RDDATATO_ERR
            SET_FIR_MASKED  (PBAFIR_BCDE_SUE_ERR      ) ; // 24  PBAFIR_BCDE_SUE_ERR
            SET_FIR_MASKED  (PBAFIR_BCDE_UE_ERR       ) ; // 25  PBAFIR_BCDE_UE_ERR
            SET_FIR_MASKED  (PBAFIR_BCDE_CE           ) ; // 26  PBAFIR_BCDE_CE
            SET_FIR_MASKED  (PBAFIR_BCDE_OCI_DATERR   ) ; // 27  PBAFIR_BCDE_OCI_DATERR
            SET_RECOV_ATTN  (PBAFIR_INTERNAL_ERR      ) ; // 28  PBAFIR_INTERNAL_ERR
            SET_RECOV_ATTN  (PBAFIR_ILLEGAL_CACHE_OP  ) ; // 29  PBAFIR_ILLEGAL_CACHE_OP
            SET_RECOV_ATTN  (PBAFIR_OCI_BAD_REG_ADDR  ) ; // 30  PBAFIR_OCI_BAD_REG_ADDR
            SET_FIR_MASKED  (PBAFIR_AXPUSH_WRERR      ) ; // 31  PBAFIR_AXPUSH_WRERR
            SET_FIR_MASKED  (PBAFIR_AXRCV_DLO_ERR     ) ; // 32  PBAFIR_AXRCV_DLO_ERR
            SET_FIR_MASKED  (PBAFIR_AXRCV_DLO_TO      ) ; // 33  PBAFIR_AXRCV_DLO_TO
            SET_FIR_MASKED  (PBAFIR_AXRCV_RSVDATA_TO  ) ; // 34  PBAFIR_AXRCV_RSVDATA_TO
            SET_FIR_MASKED  (PBAFIR_AXFLOW_ERR        ) ; // 35  PBAFIR_AXFLOW_ERR
            SET_FIR_MASKED  (PBAFIR_AXSND_DHI_RTYTO   ) ; // 36  PBAFIR_AXSND_DHI_RTYTO
            SET_FIR_MASKED  (PBAFIR_AXSND_DLO_RTYTO   ) ; // 37  PBAFIR_AXSND_DLO_RTYTO
            SET_FIR_MASKED  (PBAFIR_AXSND_RSVTO       ) ; // 38  PBAFIR_AXSND_RSVTO
            SET_FIR_MASKED  (PBAFIR_AXSND_RSVERR      ) ; // 39  PBAFIR_AXSND_RSVERR
            SET_RECOV_ATTN  (PBAFIR_PB_ACKDEAD_FW_WR  ) ; // 40  PBAFIR_PB_ACKDEAD_FW_WR
            SET_FIR_MASKED  (PBAFIR_RESERVED_41       ) ; // 41  PBAFIR_RESERVED_41
            SET_FIR_MASKED  (PBAFIR_RESERVED_42       ) ; // 42  PBAFIR_RESERVED_42
            SET_FIR_MASKED  (PBAFIR_RESERVED_43       ) ; // 43  PBAFIR_RESERVED_43
            SET_RECOV_ATTN  (PBAFIR_FIR_PARITY_ERR2   ) ; // 44  PBAFIR_FIR_PARITY_ERR2
            SET_RECOV_ATTN  (PBAFIR_FIR_PARITY_ERR    ) ; // 45  PBAFIR_FIR_PARITY_ERR

            if (e_rc)
            {
                rc.setEcmdError(e_rc);
                break;
            }

            // ---------------
            // PBA_FIR - cleared
            // ---------------
            rc = fapiPutScom(i_target, PBA_FIR_0x02010840, fir);
            if (!rc.ok())
            {
                FAPI_ERR("fapiPutScom(PBA_FIR_0x02010840) failed.");
                 break;
            }

            FAPI_DBG(" action_0  => 0x%16llx ",  action_0.getDoubleWord(0));
            FAPI_DBG(" action_1  => 0x%16llx ",  action_1.getDoubleWord(0));
            FAPI_DBG(" mask      => 0x%16llx ",  mask.getDoubleWord(0));

            //#--******************************************************************************
            //#-- PBA_FIR_ACTION0
            //#--******************************************************************************

            rc = fapiPutScom(i_target, PBA_FIR_ACTION0_0x02010846, action_0 );
            if (rc)
            {
	            FAPI_ERR("fapiPutScom(PBA_FIR_ACTION0_0x02010846) failed.");
                break;
            }

            //#--******************************************************************************
            //#-- PBA_FIR_ACTION1
            //#--******************************************************************************

            rc = fapiPutScom(i_target, PBA_FIR_ACTION1_0x02010847, action_1 );
            if (rc)
            {
	            FAPI_ERR("fapiPutScom(PBA_FIR_ACTION1_0x02010847) failed.");
                break;
            }

            // -----------
            // SW260003
            // -----------
            rc = FAPI_ATTR_GET(ATTR_PM_FIRINIT_DONE_ONCE_FLAG, &i_target, attr_pm_firinit_done_once_flag);
            if (!rc.ok()) {
              FAPI_ERR("fapiGetAttribute of ATTR_PM_FIRINIT_DONE_ONCE_FLAG failed.");
              break;
            }
            if (attr_pm_firinit_done_once_flag) {
                rc = FAPI_ATTR_GET(ATTR_PM_PBA_FIR_MASK, &i_target, attr_pm_pba_fir_mask);
                if (!rc.ok()) {
                  FAPI_ERR("fapiGetAttribute of ATTR_PM_PBA_FIR_MASK failed.");
                  break;
                }
                e_rc |= data.setDoubleWord(0, attr_pm_pba_fir_mask);
                e_rc |= mask.setOr(data, 0, 64);
                if (e_rc)
                {
                    rc.setEcmdError(e_rc);
                    break;
                }
            }

            //--******************************************************************************
            //-- PBA_FIR_MASK (W0_OR_45) (WR_43) (WO_AND_44)
            //--******************************************************************************
            rc = fapiPutScom(i_target, PBA_FIR_MASK_0x02010843, mask );
            if (rc)
            {
	            FAPI_ERR("fapiPutScom(PBA_FIR_MASK_0x02010843) failed.");
                 break;
            }
        } // Mode
    } while(0);
    return rc;

} // Procedure

} //end extern C
