/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/include/usr/pnor/pnorif.H $                               */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2011,2018                        */
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
#ifndef __PNOR_PNORIF_H
#define __PNOR_PNORIF_H

#include <stdint.h>
#include <builtins.h>
#include <errl/errlentry.H>
#include <utility>
#include <config.h>

namespace PNOR
{

/**
 * PNOR Sections
 */
enum SectionId
{
    TOC,            /**< Table of Contents */
            // Value of HB_EXT_CODE must be 1 for debug framework.
    HB_EXT_CODE,    /**< Hostboot Extended Image */
    GLOBAL_DATA,    /**< Global Data */
    HB_BASE_CODE,   /**< Hostboot Base Image */
    CENTAUR_SBE,    /**< Centaur Self-Boot Engine image */
    SBE_IPL,        /**< Self-Boot Engine IPL image */
    WINK,           /**< Sleep Winkle Reference image */
    PAYLOAD,        /**< HAL/OPAL */
    HB_RUNTIME,     /**< Hostboot Runtime (for Sapphire) */
    HB_DATA,        /**< Hostboot Data */
    HB_DATA_RW,     /**< Hostboot Data : Read-Write (optional) */
    GUARD_DATA,     /**< Guard Data */
    HB_ERRLOGS,     /**< Hostboot Error log Repository */
    DIMM_JEDEC_VPD, /**< DIMM JEDEC VPD */
    MODULE_VPD,     /**< Module VPD */
    CENTAUR_VPD,    /**< Centaur VPD */
    NVRAM,          /**< Opal NVRAM */
    OCC,            /**< OCC lid */
    FIRDATA,        /**< FIRDATA */
    ATTR_TMP,       /**< Temporary Attribute Override */
    ATTR_PERM,      /**< Permanent Attribute Override */
    CAPP,           /**< CAPP lid */
    VERSION,        /**< PNOR version string section */
    TEST,           /**< Scratch space for PNOR test cases */
    TESTRO,         /**< Scratch space for PNOR ReadOnly test cases */
    BMC_INV,        /**< BMC Inventory */
    SBKT,           /**< SecureBoot Key Transition */

    NUM_SECTIONS,   /**< Number of defined sections */

    FIRST_SECTION = TOC,  /**< First section (for looping) */
    LAST_SECTION = SBKT,  /**< Last section (for looping) */
    INVALID_SECTION = NUM_SECTIONS,  /**< Used for error cases, initialization */
};

/**
 * Information about a section of PNOR
 */
    // Size and layout of this structure must be maintained for debug framework.
struct SectionInfo_t
{
    SectionInfo_t(): id(INVALID_SECTION) {}
    SectionId id;       /**< Identifier for this section */
    const char* name;   /**< Name of the section */
    uint64_t vaddr;     /**< Virtual address of the start of the section */
    uint32_t flashAddr; /**< Flash address of the start of the section*/
    uint64_t size;      /**< Size of partition in bytes */
    bool eccProtected;  /**< Section is ECC protected */
    bool sha512Version; /**< Version Checking */
    bool sha512perEC;   /**< Version Checking perEC */
    bool readOnly;      /**< Section is read only */
    bool reprovision;   /**< Erase this section during a reprovision */
#ifdef CONFIG_SECUREBOOT
    size_t secureProtectedPayloadSize; /**< Cache the secure payload size so
                                              that the secure container only
                                              needs to be parsed once */
#endif
};

/**
 * PNOR Sides
 */
enum SideId
{
    WORKING = 0,
#ifdef CONFIG_PNOR_TWO_SIDE_SUPPORT
    ALTERNATE = 1,
#endif
    NUM_SIDES,
    INVALID_SIDE = NUM_SIDES,
    FIRST_SIDE   = WORKING,
};

/**
 * Information about a side of PNOR
 */
struct SideInfo_t
{
    SideId id;              /**< Side ID */
    char side;              /**< name of the side either A or B */
    bool isGolden;          /**< True if side is golden */
    bool isGuardPresent;    /**< True if guard section is present */
    bool hasOtherSide;      /**< True if a valid alternate side exists*/
    uint64_t primaryTOC;    /**< Address of the primary TOC */
    uint64_t backupTOC;     /**< Address of the backup TOC  */
    uint64_t hbbAddress;    /**< HBB Address associated with this side */
    uint64_t hbbMmioOffset; /**< HBB MMIO Offset associated with hbbAddress*/
};

enum
{
    INVALID_OFFSET = 0xFFFFFFF,  // Invalid primary or alternate TOC
    SBE_HEADER_SIZE = 24,
};

/**
 * @brief Returns information about a given side of pnor
 *
 * @param[in]  i_side PNOR side
 * @param[out] o_info side information
 *
 * @return errlHndl_t Error log if request was invalid
 */
errlHndl_t getSideInfo (SideId i_side, SideInfo_t& o_info);

/**
 * @brief  Return the size and address of a given section of PNOR data
 *
 * @param[in] i_section  PNOR section
 * @param[out] o_info  Location and size information
 *
 * @return errlHndl_t  Error log if request was invalid
 */
errlHndl_t getSectionInfo( SectionId i_section,
                           SectionInfo_t& o_info );

#ifdef CONFIG_SECUREBOOT
/**
 *  @brief Loads requested PNOR section to secure virtual address space
 *
 *  @par Detailed Description:
 *      Loads requested PNOR section to secure virtual address space.  If secure
 *      mode is enabled, the load will fully validate the section before
 *      placing it in the address space.  Returns error if the requested
 *      section does not have secure space support.
 *
 *  @param[in] i_section PNOR section to load. Section must not already be
 *      loaded.
 *
 *  @return errlHndl_t Error log handle
 *  @retval NULL Successfully loaded PNOR section
 *  @retval !NULL Failed to load PNOR section
 */
errlHndl_t loadSecureSection(SectionId i_section);

/**
 *  @brief Flushes any applicable pending writes and unloads requested PNOR
 *      section from secure virtual address space
 *
 *  @param[in] i_section PNOR section to unload.  No-op if already unloaded.
 *
 *  @return errlHndl_t Error log handle
 *  @retval NULL Successfully unloaded PNOR section
 *  @retval !NULL Failed to unload PNOR section
 */
errlHndl_t unloadSecureSection(SectionId i_section);

/**
  * @brief  Memcmp a vaddr to the known secureboot magic number
  *
  * @param[in]   i_vaddr: vaddr of secureboot header to check for magic number
  *                       Note: must point to a buffer of size >= 4 bytes
  *
  * @return  bool - True if the magic number and starting bytes of the vaddr
  *                 match. False otherwise.
  */
bool cmpSecurebootMagicNumber(const uint8_t* i_vaddr);

/**
  * @brief  Returns true if a PNOR section has the secureboot container
  *         header magic number at the beginning. This is mainly used to
  *         ignore unwanted PNOR sections like secureboot key transition.
  *         It indicates the section has valid content to be securely
  *         loaded, otherwise the section content will not be loaded.
  *         If a section does not have the header but needs to be loaded,
  *         it will fail ROM verify later on anyhow.
  *         Note: Does not work with HBB section and will assert if attempted
  *
  * @param[in]   i_section: PNOR section to check first bytes of.
  * @param[out]  o_valid:  true if section has the correct magic number at
  *                        the beginning
  *
  * @return  errlHndl_t - NULL if success, errlHndl_t otherwise.
  */
errlHndl_t hasSecurebootMagicNumber(SectionId i_section, bool &o_valid);
#endif // CONFIG_SECUREBOOT

/**
 *  @brief Determines whether the given section is secured by secure boot
 *
 *  @param[in] i_section PNOR section to test.
 *
 *  @return bool True if secure section, false otherwise.
 */
bool isSecureSection(const uint32_t i_section);

/**
 * @brief Determines whether the given section is inhibited by secure boot
 *        for containing attribute overrides.
 *
 * @param[in] i_section PNOR section to test.
 *
 * @return bool True if inhibited section, false otherwise.
 */
bool isInhibitedSection(const uint32_t i_section);

/**
 * @brief  Write the data back from hostboot memory to PNOR of a given section
 *         of PNOR
 *
 * @param[in] i_section  PNOR section
 *
 * @return errlHndl_t  Error log if request was invalid
 */
errlHndl_t flush (SectionId i_section);

/**
 * @brief checks and fixes correctable ECC errors in the PNOR
 *
 * @param[in] i_secion:   PNOR section
 * @return errlHndl_t:    Error Log
 */
errlHndl_t fixECC (SectionId i_section);

/**
 * @brief Informs caller if PNORDD is using
 *        L3 Cache for fake PNOR or not.
 *
 * @return Indicate state of fake PNOR
 *         true = PNOR DD is using L3 Cache for fake PNOR
 *         false = PNOR DD not using L3 Cache for fake PNOR
 */
 bool usingL3Cache();

/**
 * @brief  Clears the specified PNOR section with all FF's (w/ good ECC)
 *
 * @param[in] i_section  PNOR section to clear
 *
 * @return Error if fails
 */
errlHndl_t clearSection(PNOR::SectionId i_section);

/**
 * @brief Validate the Alternative Master Processor's LPC
 *        Connection to PNOR
 *
 * @return errlHndl_t  Error log if validation failed
 */
errlHndl_t validateAltMaster( void );

/** @brief PNOR::TEST section offsets for test cases to prevent
 *         concurrency problems
 */
enum TestSectionOffset{
    pnorTestSec_readwrite_offset = 0x100,
    pnorTestSec_smartwrite_offset = 0x120,
    pnorTestSec_rt_readwrite_offset = 0x6000,
};

/** Information about PNOR Layout */
struct PnorInfo_t
{
    uint32_t mmioOffset; //< Address of MMIO access
    uint32_t norWorkarounds; //< NOR flash workarounds
    uint32_t flashSize; //< Size of PNOR in bytes
};

/**
 * @brief Retrieve some information about the PNOR/SFC hardware
 *
 * @param[out] o_pnorInfo Information about PNOR
 */
void getPnorInfo( PnorInfo_t& o_pnorInfo );

}

#endif
