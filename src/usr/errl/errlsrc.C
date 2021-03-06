/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/errl/errlsrc.C $                                      */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2011,2014              */
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
/**
 *  @file errlsrc.C
 *
 *  @brief Manage the data items that make up the 'PS' section in an
 *  error log PEL.  PS stands for Primary System Reference Code, or SRC.
 *  ErrlSrc is a derivation of ErrlSctn.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hbotcompid.H>
#include <errl/errlentry.H>



namespace ERRORLOG
{


extern trace_desc_t* g_trac_errl;



//**************************************************************************
//  constructor


ErrlSrc::ErrlSrc( srcType_t i_srcType,
                  uint8_t   i_modId,
                  uint16_t  i_reasonCode,
                  uint64_t  i_user1,
                  uint64_t  i_user2 ) :

    ErrlSctn( ERRL_SID_PRIMARY_SRC,
              ErrlSrc::SLEN,
              ErrlSrc::VER,
              ErrlSrc::SST,
              0 ),  // Component ID zero for now.
    iv_srcType( i_srcType ),
    iv_modId( i_modId ),
    iv_reasonCode( i_reasonCode ),
    iv_ssid( EPUB_FIRMWARE_SUBSYS ),
    iv_user1( i_user1 ),
    iv_user2( i_user2 ),
    iv_deconfig(false),
    iv_gard(false)
{

}


//****************************************************************************
//

ErrlSrc::~ErrlSrc()
{

}



//**************************************************************************
// Flatten the PS primary SRC data to a minimum standard 72-byte structure.
// Page numbers refer to Platform Event Log and SRC PLDD
// https://mcdoc.boeblingen.de.ibm.com/out/out.ViewDocument.php?documentid=1675
// Version 0.8 (markup). See also src/include/usr/errl/hberrltypes.H
// for the typedef pelSRCSection_t.

uint64_t ErrlSrc::flatten( void * o_pBuffer, const uint64_t i_cbBuffer )
{
    uint64_t l_rc = 0;

    do
    {
        if( i_cbBuffer < flatSize() )
        {
            TRACFCOMP( g_trac_errl, "ErrlSrc::flatten: buffer too small");
            break;
        }

        pelSRCSection_t * psrc = static_cast<pelSRCSection_t *>(o_pBuffer);

        // memset zero up to the char array.
        memset( psrc, 0, flatSize() - sizeof( psrc->srcString ));

        // memset spaces into the char array
        memset( psrc->srcString, ' ', sizeof( psrc->srcString ));

        l_rc = iv_header.flatten( o_pBuffer, i_cbBuffer );
        if( 0 == l_rc )
        {
            // Rare.
            TRACFCOMP( g_trac_errl, "ErrlSrc::flatten: header flatten error");
            break;
        }


        // Place data into the flat structure.
        psrc->ver         = ErrlSrc::SRCVER;    // 2;
        // psrc->flags    = 0;                  //
        psrc->wordcount   = ErrlSrc::WORDCOUNT; // 9;

        // Use reserved1 word to stash the reason code for easy extract in
        // unflatten rather than parsing srcString
        psrc->reserved1   = iv_reasonCode;

        CPPASSERT( ErrlSrc::SLEN == sizeof(pelSRCSection_t)-iv_header.flatSize());
        psrc->srcLength   = ErrlSrc::SLEN;

        // SRC format
        psrc->word2       =  0x000000E0; // SRCI_HBT_FORMAT

        // Stash the Hostboot module id into hex word 3
        psrc->moduleId    = iv_modId;

        // set deconfigure and/or gard bits
        if (iv_deconfig)
        {
            psrc->word5 |= ErrlSrc::DECONFIG_BIT; // deconfigure
        }
        if (iv_gard)
        {
            psrc->word5 |= ErrlSrc::GARD_BIT; // GARD
        }

        // set ACK bit - means unacknowledged
        psrc->word5 |= ErrlSrc::ACK_BIT; // ACK

        // Stash the Hostboot long long words into the hexwords of the SRC.
        psrc->word6       = iv_user1;    // spans 6-7
        psrc->word8       = iv_user2;    // spans 8-9

        // Build the char string for the SRC.
        uint32_t l_u32;
        l_u32 = (iv_srcType<< 24)|(iv_ssid<<16)| iv_reasonCode;

        char l_tmpString[ 20 ];
        uint64_t cb = sprintf( l_tmpString, "%08X", l_u32 );
        memcpy( psrc->srcString, l_tmpString, cb );

        l_rc = flatSize();
    }
    while( 0 );

    return l_rc;
}

uint64_t ErrlSrc::unflatten( const void * i_buf)
{
    const pelSRCSection_t * p =
        static_cast<const pelSRCSection_t *>(i_buf);

    iv_header.unflatten(&(p->sectionheader));

    iv_srcType      = (srcType_t)((16 * aschex2bin(p->srcString[0])) +
                        aschex2bin(p->srcString[1]));
    iv_modId        = p->moduleId;
    iv_reasonCode   = p->reserved1;
    iv_ssid         = (epubSubSystem_t)((16 * aschex2bin(p->srcString[2])) +
                       aschex2bin(p->srcString[3]));
    iv_user1        = p->word6;
    iv_user2        = p->word8;

    if(p->word5 & ErrlSrc::DECONFIG_BIT) // deconfigure
    {
        iv_deconfig = true;
    }
    if(p->word5 & ErrlSrc::GARD_BIT) // GARD
    {
        iv_gard = true;
    }

    return flatSize();
}

// Quick hexdigit to binary converter.
// Hopefull someday to replaced by strtoul
uint64_t ErrlSrc::aschex2bin(char c) const
{
    if(c >= 'a' && c <= 'f')
    {
        c = c + 10 - 'a';
    }
    else if (c >= 'A' && c <= 'F')
    {
        c = c + 10 - 'A';
    }
    else if (c >= '0' && c <= '9')
    {
       c -= '0';
    }
    // else it's not a hex digit, ignore

    return c;
}

}  // namespace



