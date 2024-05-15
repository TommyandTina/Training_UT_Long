/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2020, Renesas Electronics Corporation
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

#include "rcar_isp.h"

void rcar_isp_fini(rcar_vin_t *vin)
{
    if (vin->isp_vbase) {       
        munmap_device_io(vin->isp_vbase, RCAR_ISP_SIZE);
    }
}

static void rcar_isp_reset(int isp_chan)
{
    unsigned   hwi_off;
    hwi_tag    *tag;
    uintptr_t  reset_base;
    uint32_t   tmp, product_id = PRODUCT_ID_RCAR_UNKNOWN;

    hwi_off = hwi_find_device("r-car", 0);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_hwversion, 0);
        if(tag) {
             product_id = tag->hwversion.version;
         }
    }

    if (IS_RCAR_GEN4(RCAR_PRODUCT_ID(product_id))) {
        if((reset_base = (uintptr_t)mmap_device_io(RCAR_GEN4_CPG_SIZE, RCAR_GEN4_CPG_DM0_BASE)) == (uintptr_t)MAP_FAILED) {
            return;
        }

        tmp = 1 << (12 + isp_chan);

        /* Reset */
        out32(reset_base + RCAR_GEN4_CPG_CPGWPR, ~tmp);
        out32(reset_base + RCAR_GEN4_CPG_SRCR(6), tmp);

        /* Wait for at least one cycle of the RCLK clock (32 kHz) */
        nanospin_ns(35000);

        /* Release reset */
        out32(reset_base + RCAR_GEN4_CPG_CPGWPR, ~tmp);
        out32(reset_base + RCAR_GEN4_CPG_SRSTCLR(6), tmp);

        mmap_device_io(reset_base, RCAR_GEN4_CPG_SIZE);
    }
}

int rcar_isp_init(rcar_vin_t *vin, int isp_chan)
{
    unsigned        hwi_off;
    hwi_tag        *tag;
    decoder_info_t *dec = &vin->dec;
    int             ch, vc, i;
    uint32_t        data_type = 0;
    uint32_t        data_proc = 0;

    /* ISP Physical base address */
    hwi_off = hwi_find_device(RCAR_HWI_ISP, isp_chan);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            vin->isp_pbase = tag->location.base;
        }
    }

    if((vin->isp_vbase = (uintptr_t)mmap_device_io(RCAR_ISP_SIZE, vin->isp_pbase)) == (uintptr_t)MAP_FAILED) {
        fprintf(stderr, "%s: CSI2 base mmap_device_io (0x%x) failed \r\n", __FUNCTION__, (uint32_t)vin->isp_pbase);
        return -1;
    }

    rcar_isp_reset(isp_chan);

    /* Virtual channel */
    for (ch = 4, vc = 0; ch < 8 && vc < 4; ch++, vc++) {
        out32(vin->isp_vbase + RCAR_ISPCS_FILTER_ID_CHn(ch), (0x01 << vc));
        out32(vin->isp_vbase + RCAR_ISPCS_LC_MODULO_CHn(ch), 0x00000000);

        /* MD Filter and pixel reconstructor method */
        switch(dec->format & RCAR_VIN_MC_INF_MASK) {
            case RCAR_VIN_MC_INF_RGB888:
                data_type = RCAR_CSI2_RGB888_DT_FORMAT;
                data_proc = RCAR_ISP_PROCMODE_DT_RGB888;
                out32(vin->isp_vbase + RCAR_ISPCS_DT_CODE03_CHn(ch),
                        (data_type <<  0) | RCAR_ISPCS_DT_CODE03_ENABLE0 |
                        (data_type <<  8) | RCAR_ISPCS_DT_CODE03_ENABLE1 |
                        (data_type << 16) | RCAR_ISPCS_DT_CODE03_ENABLE2 |
                        (data_type << 24) | RCAR_ISPCS_DT_CODE03_ENABLE3);
                break;
            case RCAR_VIN_MC_INF_YUV8_BT601:
                data_type = RCAR_CSI2_UYVY_DT_FORMAT;
                data_proc = RCAR_ISP_PROCMODE_DT_YU422_8BIT;
                out32(vin->isp_vbase + RCAR_ISPCS_DT_CODE03_CHn(ch),
                        (data_type <<  0) | RCAR_ISPCS_DT_CODE03_ENABLE0 |
                        (data_type <<  8) | RCAR_ISPCS_DT_CODE03_ENABLE1 |
                        (data_type << 16) | RCAR_ISPCS_DT_CODE03_ENABLE2 |
                        (data_type << 24) | RCAR_ISPCS_DT_CODE03_ENABLE3);
                break;
            case RCAR_VIN_MC_INF_YUV10_BT601:
                data_type = RCAR_CSI2_YUV422_10;
                data_proc = RCAR_ISP_PROCMODE_DT_YU422_8BIT;
                out32(vin->isp_vbase + RCAR_ISPCS_DT_CODE03_CHn(ch),
                        (data_type <<  0) | RCAR_ISPCS_DT_CODE03_ENABLE0 |
                        (data_type <<  8) | RCAR_ISPCS_DT_CODE03_ENABLE1 |
                        (data_type << 16) | RCAR_ISPCS_DT_CODE03_ENABLE2 |
                        (data_type << 24) | RCAR_ISPCS_DT_CODE03_ENABLE3);
                break;
	    case RCAR_VIN_MC_INF_RAW10:
                data_type = RCAR_CSI2_UYVY_DT_RAW10;
                data_proc = RCAR_ISP_PROCMODE_DT_RAW10;
                out32(vin->isp_vbase + RCAR_ISPCS_DT_CODE03_CHn(ch),
                        (data_type <<  0) | RCAR_ISPCS_DT_CODE03_ENABLE0 |
                        (data_type <<  8) | RCAR_ISPCS_DT_CODE03_ENABLE1 |
                        (data_type << 16) | RCAR_ISPCS_DT_CODE03_ENABLE2 |
                        (data_type << 24) | RCAR_ISPCS_DT_CODE03_ENABLE3);
                        
                       
                break;
            default:
                fprintf(stderr, "%s: ISP unsupports Format type 0x%x", __FUNCTION__, dec->dfmt);
                return -1;
        }

        /* Filer slot4,5,6,7 are not used */
        out32(vin->isp_vbase + RCAR_ISPCS_OUTPUT_4to7_MODE_CHn(ch), 0x00000000);

        /* Set default value */
        for (i = 0; i < 4; i++) {
            out32(vin->isp_vbase + RCAR_ISPCS_H_CLIP_DT_CODEm_CHn(i, ch), 0x0fff0000);
            out32(vin->isp_vbase + RCAR_ISPCS_V_CLIP_DT_CODEm_CHn(i, ch), 0x0fff0000);
        }
    }

    /* Write Protect */
    out32(vin->isp_vbase + RCAR_ISPWP_CTRL, RCAR_ISPWP_CTRL_UNLOCK_CODE_U | RCAR_ISPWP_CTRL_UNLOCK_CODE_L);

    /* Output Channnel select */
    out32(vin->isp_vbase + RCAR_ISPINPUTSEL0, in32(vin->isp_vbase + RCAR_ISPINPUTSEL0) | ((isp_chan % 2) ? RCAR_ISPINPUTSEL0_SEL_CSI0 : 0));

    /* Number of cycles for padding */
    out32(vin->isp_vbase + RCAR_ISP_PADDING_CTRL, in32(vin->isp_vbase + RCAR_ISP_PADDING_CTRL) | RCAR_ISP_PADDING_CTRL_PADDING_NUM(32));

    out32(vin->isp_vbase + RCAR_ISP_PROCMODE_DT(data_type), (data_proc << 0) | (data_proc << 8) | (data_proc << 16) | (data_proc << 24));

    /* Write Protect */
    out32(vin->isp_vbase + RCAR_ISPWP_CTRL, RCAR_ISPWP_CTRL_UNLOCK_CODE_U | RCAR_ISPWP_CTRL_UNLOCK_CODE_L);

    /* FIFO control */
    out32(vin->isp_vbase + RCAR_ISPFIFOCTRL, RCAR_ISPFIFOCTRL_FIFO_PUSH_CSI);

    /* ISP Start */
    out32(vin->isp_vbase + RCAR_ISPSTART, RCAR_ISPSTART_START_ISP);

    return 0;
}
