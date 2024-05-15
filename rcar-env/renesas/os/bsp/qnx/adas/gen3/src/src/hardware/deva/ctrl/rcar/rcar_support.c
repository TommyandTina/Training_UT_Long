/*
 * $QNXLicenseC:
 * Copyright 2014, 2016 QNX Software Systems.
 * Copyright 2021, Renesas Electronics Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You
 * may not reproduce, modify or distribute this software except in
 * compliance with the License. You may obtain a copy of the License
 * at: http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as
 * contributors under the License or as licensors under other terms.
 * Please review this entire file for other proprietary rights or license
 * notices, as well as the QNX Development Suite License Guide at
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */

#include <string.h>
#include "rcar_support.h"
#include "rcar.h"

/* Using two different rcar versions on the same board is not supported;
 * the hwi_find_device, hwi_tag_find, confstr calls assume a single rcar
 * version is supported on a given board, and their corresponding
 * implementation is picked up by the BSP package of the board;
 * given the above, the rcar_version can be stored using a static global
 * variable; attempting to set it twice to different valid values will
 * result in an error
 */
static rcar_version_t rcar_version = RCAR_VERSION_UNKNOWN;
static rcar_generation_t rcar_generation = RCAR_GENERATION_UNKNOWN;

inline rcar_version_t rcar_version_parse( const char * str )
{
    if( strstr( str, "H2") || strstr( str, "h2") ){
        return RCAR_VERSION_H2;
    } else if ( strstr(str, "M2" ) || strstr( str, "m2") ){
        return RCAR_VERSION_M2;
    } else if ( strstr(str, "E2" ) || strstr( str, "e2") ){
        return RCAR_VERSION_E2;
    } else if ( strstr(str, "V2" ) || strstr( str, "v2") ){
        return RCAR_VERSION_V2;
    } else if ( strstr(str, "H3" ) || strstr( str, "h3") ){
        return RCAR_VERSION_H3;
    } else if ( strstr(str, "M3" ) || strstr( str, "m3") ){
        return RCAR_VERSION_M3;
    } else if ( strstr(str, "D3" ) || strstr( str, "d3") ){
        return RCAR_VERSION_D3;
    } else if ( strstr(str, "E3" ) || strstr( str, "e3") ){
        return RCAR_VERSION_E3;
    } else if ( strstr(str, "W2H" ) || strstr( str, "w2h") ){
        return RCAR_VERSION_W2H;
    } else {
        ado_debug( DB_LVL_DRIVER, "version not set" );
        return RCAR_VERSION_UNKNOWN;
    }
}

inline void rcar_version_init( void )
{
#if _NTO_VERSION >= 700
#define CS_MACHINE_LEN (64 + 1)
    char     cs_machine_str[CS_MACHINE_LEN];
    if( confstr(_CS_MACHINE, cs_machine_str, CS_MACHINE_LEN) > 0 ) {
        ado_debug( DB_LVL_DRIVER, "detected version string=%s", cs_machine_str );
        rcar_version = rcar_version_parse(cs_machine_str);
        switch( rcar_version ) {
            case RCAR_VERSION_H2:
            case RCAR_VERSION_M2:
            case RCAR_VERSION_E2:
            case RCAR_VERSION_V2:
            case RCAR_VERSION_W2H:
                rcar_generation = RCAR_GENERATION_2;
                break;
            case RCAR_VERSION_H3:
            case RCAR_VERSION_M3:
            case RCAR_VERSION_D3:
            case RCAR_VERSION_E3:
                rcar_generation = RCAR_GENERATION_3;
                break;
            default:
                ;
        }
    }
#endif
}

inline int rcar_version_set( rcar_version_t version ) {
    if (rcar_version != RCAR_VERSION_UNKNOWN &&
        version != rcar_version) {
        return EINVAL;
    }
    switch( version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
            rcar_version = version;
            rcar_generation = RCAR_GENERATION_2;
            break;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
            rcar_version = version;
            rcar_generation = RCAR_GENERATION_3;
            break;
        default:
            return EINVAL;
    }
    return EOK;
}

inline rcar_version_t rcar_version_get( void ) {
    return rcar_version;
}

inline rcar_generation_t rcar_generation_get( void ) {
    return rcar_generation;
}

int rcar_query_hwinfo( const char* dev, unsigned long* base, uint32_t* len ) {
    int ret = ENOTSUP;
#if _NTO_VERSION >= 700
    unsigned hwi_off = hwi_find_device(dev, 0);

    if(hwi_off != HWI_NULL_OFF) {
        hwi_tag *tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if (tag) {
            if (base) {
                *base = tag->location.base;
            }
            if (len) {
                *len = tag->location.len;
            }
            ret = EOK;
        }
    }
#endif
    return ret;
}

bool rcar_ssi_supported( uint32_t ssi_channel ) {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_W2H:
        default:
            if( ssi_channel <= 9 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_V2:
        case RCAR_VERSION_D3:
            if( ssi_channel >= 3 && ssi_channel <= 4 ) {
                return true;
            }
            return false;
    }
}

bool rcar_ssi_tdm_supported( uint32_t ssi_channel ) {
    if( !rcar_ssi_supported( ssi_channel ) || rcar_version == RCAR_VERSION_D3) {
        return false;
    }

    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
            return true;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            return false;
    }
}

bool rcar_ssi_tdm_voices_supported( uint32_t voices ) {
    if (rcar_version == RCAR_VERSION_D3) {
        return false;
    }
    switch( voices ) {
        case 1:
        case 2: // are these considered TDM?
        case 4:
        case 6:
        case 8:
            return true;
        case 16:
            if( (rcar_version == RCAR_VERSION_H3) ||
                (rcar_version == RCAR_VERSION_M3) ) {
                return true;
            }
            return false;
        default:
            return false;
    }
}

bool rcar_ssi_tdmext_supported( uint32_t ssi_channel ) {
    if( !rcar_ssi_supported( ssi_channel ) || rcar_version == RCAR_VERSION_D3) {
        return false;
    }
    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
            return true;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            return false;
    }
}

bool rcar_ssi_tdmsplit_supported( uint32_t ssi_channel ) {
#ifdef RCAR_TDMSPLIT_SUPPORT
    if( !rcar_ssi_supported( ssi_channel ) || rcar_version == RCAR_VERSION_D3) {
        return false;
    }
    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 9:
            return true;
        case 3:
        case 4:
            if( (rcar_version == RCAR_VERSION_H3) ||
                (rcar_version == RCAR_VERSION_M3) ||
                (rcar_version == RCAR_VERSION_E3) ){
                return true;
            }
            return false;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            return false;
    }
#else
    return false;
#endif
}

bool rcar_ssi_tdmexsplit_supported( uint32_t ssi_channel ) {
#ifdef RCAR_TDMEXSPLIT_SUPPORT
    if( !rcar_ssi_supported( ssi_channel ) || rcar_version == RCAR_VERSION_D3) {
        return false;
    }
    if( (rcar_version != RCAR_VERSION_H3) &&
        (rcar_version != RCAR_VERSION_M3) &&
        (rcar_version != RCAR_VERSION_E3)) {
        return false;
    }
    switch( ssi_channel ) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
            return true;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            return false;
    }
#else
    return false;
#endif
}

int rcar_ssi_get_supported_range( uint32_t* min_ssi_channel, uint32_t* max_ssi_channel) {
    if( !min_ssi_channel || !max_ssi_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_W2H:
        default:
            *min_ssi_channel = 0;
            *max_ssi_channel = 9;
            break;
        case RCAR_VERSION_V2:
        case RCAR_VERSION_D3:
            *min_ssi_channel = 3;
            *max_ssi_channel = 4;
            break;
    }
    return EOK;
}

bool rcar_ssi_subchan_supported( uint32_t ssi_channel, uint32_t subchannel ) {
    if( !rcar_ssi_supported( ssi_channel ) ) {
        return false;
    }

    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        default:
            switch( ssi_channel ) {
                case 0:
                case 1:
                case 2:
                case 9:
                    if( subchannel <= 3 ) {
                        return true;
                    }
                    return false;
                default:
                    if( subchannel == 0 ) {
                        return true;
                    }
                    return false;
            }
            break;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
            switch( ssi_channel ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 9:
                    if( subchannel <= (rcar_version == RCAR_VERSION_E3 ? 3 : 7) ) {
                        return true;
                    }
                    return false;
                default:
                    if( subchannel == 0 ) {
                        return true;
                    }
                    return false;
                break;
            }
            break;
        case RCAR_VERSION_D3:
            if( subchannel == 0 ) {
                return true;
            }
            return false;
    }
}

int rcar_ssi_subchan_get_supported_range( uint32_t ssi_channel, uint32_t* min_subchannel, uint32_t* max_subchannel ) {
    if( !rcar_ssi_supported( ssi_channel ) ) {
        return EINVAL;
    }
    if( !min_subchannel || !max_subchannel ) {
        return EINVAL;
    }

    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        default:
            switch( ssi_channel ) {
                case 0:
                case 1:
                case 2:
                case 9:
                    *min_subchannel = 0;
                    *max_subchannel = 3;
                    break;
                default:
                    *min_subchannel = 0;
                    *max_subchannel = 0;
                break;
            }
            break;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
            switch( ssi_channel ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 9:
                    *min_subchannel = 0;
                    *max_subchannel = (rcar_version == RCAR_VERSION_E3) ? 3 : 7;
                    break;
                default:
                    *min_subchannel = 0;
                    *max_subchannel = 0;
                break;
            }
            break;
        case RCAR_VERSION_D3:
            *min_subchannel = 0;
            *max_subchannel = 0;
            break;
    }

    return EOK;
}

// support for BUSIF2 registers introduced for Rcar-H3
bool rcar_ssi_busif2_reg_supported( uint32_t ssi_channel ) {
    if( !rcar_ssi_supported( ssi_channel ) ) {
        return false;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
        default:
            return false;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
            switch( ssi_channel ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 9:
                    return true;
                default:
                    return false;
            }
    }
}

// support for extended common SSIU registers introduced for Rcar-H3
bool rcar_ssi_common_ext_reg_supported() {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
        default:
            return false;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
            return true;
    }
}

// support for extended SSI registers introduced for Rcar-H3
bool rcar_ssi_ext_reg_supported() {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
        default:
            return false;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
            return true;
    }
}

bool rcar_adg_ext_reg_supported() {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_D3:
        default:
            return false;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
            return true;
    }
}

bool rcar_hdmi_supported( uint32_t hdmi_channel ) {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
        default:
            return false;
        case RCAR_VERSION_H3:
            if( hdmi_channel <= 1 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_M3:
            if( hdmi_channel == 0 ) {
                return true;
            }
            return false;
    }
}

int rcar_hdmi_get_supported_range( uint32_t* min_hdmi_channel, uint32_t* max_hdmi_channel ) {
    if( !min_hdmi_channel || !max_hdmi_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_V2:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
        default:
            return EINVAL;
        case RCAR_VERSION_H3:
            *min_hdmi_channel = 0;
            *max_hdmi_channel = 1;
            return EOK;
        case RCAR_VERSION_M3:
            *min_hdmi_channel = 0;
            *max_hdmi_channel = 0;
            return EOK;
    }
}

bool rcar_src_supported( uint32_t src_channel ) {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        default:
            if( src_channel <= 9 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_E2:
        case RCAR_VERSION_W2H:
            if( src_channel >= 1 && src_channel <= 6 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_V2:
            return false; /* no SRC support */
        case RCAR_VERSION_D3:
            if( src_channel >= 5 && src_channel <= 6 ) {
                return true;
            }
            return false;
    }
}

bool rcar_src_multichan_supported( uint32_t src_channel ) {
    if( !rcar_src_supported( src_channel ) ) {
        return false;
    }

    switch( rcar_version ) {
        case RCAR_VERSION_E3:
            if ((src_channel == 1) || (src_channel == 3)) {
                return true;
            }
            break;
        case RCAR_VERSION_D3:
            return false;  // SRC5, SRC6
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_W2H:
        default:
            switch( src_channel ) {
                case 0:
                case 1:
                case 3:
                case 4:
                    return true;
                case 2:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                default:
                    break;
            }
        break;
    }

    return false;
}

bool rcar_src_highsound_supported( uint32_t src_channel ) {
    if( !rcar_src_supported( src_channel ) || rcar_version == RCAR_VERSION_D3) {
        return false;
    }
    switch( src_channel) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 9:
            return true;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            return false;
    }
}

bool rcar_src_inline_supported( uint32_t src_channel ) {
    if( !rcar_src_supported( src_channel ) ) {
        return false;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
            switch( src_channel ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 9:
                    return true;
                case 7:
                case 8:
                default:
                    return false;
            }
        case RCAR_VERSION_D3:
            return true; // SRC5, SRC6 both inline
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_W2H:
        default:
            switch( src_channel ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    return true;
                case 6:
                case 7:
                case 8:
                case 9:
                default:
                    return false;
            }
    }
}

int rcar_src_get_supported_range( uint32_t* min_src_channel, uint32_t* max_src_channel) {
    if( !min_src_channel || !max_src_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        default:
            *min_src_channel = 0;
            *max_src_channel = 9;
            break;
        case RCAR_VERSION_E2:
        case RCAR_VERSION_W2H:
            *min_src_channel = 1;
            *max_src_channel = 6;
            break;
        case RCAR_VERSION_V2:
            return ENOTSUP;
        case RCAR_VERSION_D3:
            *min_src_channel = 5;
            *max_src_channel = 6;
            break;
    }
    return EOK;
}

bool rcar_cmd_supported( uint32_t cmd_channel ) {
    if( cmd_channel <= 1 && rcar_version != RCAR_VERSION_V2 ) {
        return true;
    }
    return false;
}

bool rcar_cmd_src_sync_timing_supported( void ) {
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_W2H:
        case RCAR_VERSION_V2:
        default:
            return false;
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_D3:
            return true;
    }
}

int rcar_cmd_get_supported_range( uint32_t* min_cmd_channel, uint32_t* max_cmd_channel ) {
    if( !min_cmd_channel || !max_cmd_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_D3:
        case RCAR_VERSION_W2H:
        default:
            *min_cmd_channel = 0;
            *max_cmd_channel = 1;
            break;
        case RCAR_VERSION_V2:
            return ENOTSUP;
    }
    return EOK;
}

bool rcar_mlm_supported( uint32_t mlm_channel ) {
#ifdef RCAR_MLP_SUPPORT
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_W2H:
        default:
            if( mlm_channel <= 7 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_E2:
            if( mlm_channel <= 3 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_V2:
            return false; /* no MLM support */
        case RCAR_VERSION_D3:
            if( mlm_channel >= 4 && mlm_channel <= 7 ) {
                return true;
            }
            return false;
    }
#endif
    return false;
}

int rcar_mlm_get_supported_range( uint32_t* min_mlm_channel, uint32_t* max_mlm_channel) {
#ifdef RCAR_MLP_SUPPORT
    if( !min_mlm_channel || !max_mlm_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_W2H:
        default:
            *min_mlm_channel = 0;
            *max_mlm_channel = 7;
            break;
        case RCAR_VERSION_E2:
            *min_mlm_channel = 0;
            *max_mlm_channel = 3;
            break;
        case RCAR_VERSION_V2:
            return ENOTSUP;
        case RCAR_VERSION_D3:
            *min_mlm_channel = 4;
            *max_mlm_channel = 7;
            break;
    }
    return EOK;
#else
    return ENOTSUP;
#endif
}


static bool __attribute__((__unused__))rcar_dtcpp_supported( uint32_t dtcpp ) {
#if defined(RCAR_MLP_SUPPORT) && defined(RCAR_DTCP_SUPPORT)
    if( dtcpp <= 1 && rcar_version != RCAR_VERSION_V2 ) {
        return true;
    }
#endif
    return false;
}

static bool __attribute__((__unused__))rcar_dtcpc_supported( uint32_t dtcpc ) {
#if defined(RCAR_MLP_SUPPORT) && defined(RCAR_DTCP_SUPPORT)
    if( dtcpc <= 1 && rcar_version != RCAR_VERSION_V2 ) {
        return true;
    }
#endif
    return false;
}

int rcar_dtcp_get_supported_range( uint32_t* min_dtcp, uint32_t* max_dtcp) {
#if defined(RCAR_MLP_SUPPORT) && defined(RCAR_DTCP_SUPPORT)
    if( !min_dtcp || !max_dtcp ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_E2:
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_E3:
        case RCAR_VERSION_W2H:
        default:
            *min_dtcp = 0;
            *max_dtcp = 1;
            break;
        case RCAR_VERSION_V2:
        case RCAR_VERSION_D3:
            return ENOTSUP;
    }
    return EOK;
#else
    return ENOTSUP;
#endif
}

bool rcar_dmac_channel_supported( uint32_t dma_channel ) {
    switch( rcar_version ) {
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
            if( dma_channel <= 31 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_W2H:
        default:
            if( dma_channel <= 25 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_E2:
            if( dma_channel <= 12 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_V2:
            if( dma_channel <= 1 ) {
                return true;
            }
            return false;
        case RCAR_VERSION_E3:
        case RCAR_VERSION_D3:
            if( dma_channel <= 15 ) {
                return true;
            }
            return false;
    }
}

int rcar_dmac_get_supported_range( uint32_t* min_dma_channel, uint32_t* max_dma_channel) {
    if( !min_dma_channel || !max_dma_channel ) {
        return EINVAL;
    }
    switch( rcar_version ) {
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
            *min_dma_channel = 0;
            *max_dma_channel = 31;
            break;
        case RCAR_VERSION_H2:
        case RCAR_VERSION_M2:
        case RCAR_VERSION_W2H:
        default:
            *min_dma_channel = 0;
            *max_dma_channel = 25;
            break;
        case RCAR_VERSION_E2:
            *min_dma_channel = 0;
            *max_dma_channel = 12;
            break;
        case RCAR_VERSION_V2:
            *min_dma_channel = 0;
            *max_dma_channel = 1;
            break;
        case RCAR_VERSION_D3:
        case RCAR_VERSION_E3:
            *min_dma_channel = 0;
            *max_dma_channel = 15;
            break;
    }
    return EOK;
}

int rcar_get_i2c_bus_num(int *i2c_bus) {
    if( !i2c_bus ) {
        return EINVAL;
    }

    switch( rcar_version ) {
        case RCAR_VERSION_H3:
        case RCAR_VERSION_M3:
        case RCAR_VERSION_W2H:
            *i2c_bus = 2;
            break;
        case RCAR_VERSION_D3:
            *i2c_bus = 0;
            break;
        case RCAR_VERSION_E3:
            *i2c_bus = 3;
            break;
        default:
            *i2c_bus = -1;
            return EINVAL;
    }
    return EOK;
}

uint32_t rcar_get_sharedclk_ssi ( uint32_t ssi_channel_mask ) {
    if ( ( ( ssi_channel_mask & SSI_CHANNEL_MASK_0129 ) == ssi_channel_mask ) ||
         ( ( ssi_channel_mask & SSI_CHANNEL_MASK_0123 ) == ssi_channel_mask ) ) {
        return SSI_CHANNEL_0;
    }
    if ( ( ssi_channel_mask & SSI_CHANNEL_MASK_349 ) == ssi_channel_mask ) {
        return SSI_CHANNEL_3;
    }
    if ( ( ssi_channel_mask & SSI_CHANNEL_MASK_78 ) == ssi_channel_mask ) {
        return SSI_CHANNEL_7;
    }
    return SSI_CHANNEL_NUM;
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.1.0/trunk/hardware/deva/ctrl/rcar/rcar_support.c $ $Rev: 911285 $")
#endif
