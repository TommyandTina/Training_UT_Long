/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2021, Renesas Electronics Corporation
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

#include "rcar_csi2.h"

/* CSI2 bandwidth */
enum rcar_csi2_freq {
    CSI2_FREQ_80MBPS,
    CSI2_FREQ_90MBPS,
    CSI2_FREQ_100MBPS,
    CSI2_FREQ_110MBPS,
    CSI2_FREQ_120MBPS,
    CSI2_FREQ_130MBPS,
    CSI2_FREQ_140MBPS,
    CSI2_FREQ_150MBPS,
    CSI2_FREQ_160MBPS,
    CSI2_FREQ_170MBPS,
    CSI2_FREQ_180MBPS,
    CSI2_FREQ_190MBPS,
    CSI2_FREQ_200MBPS,
    CSI2_FREQ_205MBPS,
    CSI2_FREQ_220MBPS,
    CSI2_FREQ_235MBPS,
    CSI2_FREQ_240MBPS,
    CSI2_FREQ_250MBPS,
    CSI2_FREQ_270MBPS, 
    CSI2_FREQ_275MBPS,
    CSI2_FREQ_300MBPS,
    CSI2_FREQ_325MBPS,
    CSI2_FREQ_330MBPS,
    CSI2_FREQ_350MBPS,
    CSI2_FREQ_360MBPS,
    CSI2_FREQ_400MBPS,
    CSI2_FREQ_450MBPS,
    CSI2_FREQ_500MBPS,
    CSI2_FREQ_550MBPS,
    CSI2_FREQ_600MBPS,
    CSI2_FREQ_650MBPS,
    CSI2_FREQ_700MBPS,
    CSI2_FREQ_750MBPS,
    CSI2_FREQ_800MBPS,
    CSI2_FREQ_850MBPS,
    CSI2_FREQ_900MBPS,
    CSI2_FREQ_950MBPS,
    CSI2_FREQ_1000MBPS,
    CSI2_FREQ_1050MBPS,
    CSI2_FREQ_1100MBPS,
    CSI2_FREQ_1125MBPS,
    CSI2_FREQ_1150MBPS,
    CSI2_FREQ_1200MBPS,
    CSI2_FREQ_1250MBPS,
    CSI2_FREQ_1300MBPS,
    CSI2_FREQ_1350MBPS,
    CSI2_FREQ_1400MBPS,
    CSI2_FREQ_1450MBPS,
    CSI2_FREQ_1500MBPS,
    CSI2_FREQ_1550MBPS,
    CSI2_FREQ_1600MBPS,
    CSI2_FREQ_1650MBPS,
    CSI2_FREQ_1700MBPS,
    CSI2_FREQ_1750MBPS,
    CSI2_FREQ_1800MBPS,
    CSI2_FREQ_1850MBPS,
    CSI2_FREQ_1900MBPS,
    CSI2_FREQ_1950MBPS,
    CSI2_FREQ_2000MBPS,
    CSI2_FREQ_2050MBPS,
    CSI2_FREQ_2100MBPS,
    CSI2_FREQ_2150MBPS,
    CSI2_FREQ_2200MBPS,
    CSI2_FREQ_2300MBPS,
    CSI2_FREQ_2350MBPS,
    CSI2_FREQ_2400MBPS,
    CSI2_FREQ_2450MBPS,
    CSI2_FREQ_2500MBPS,
    CSI2_FREQ_MAX
};

uint16_t csi2_freqs_h3_v3h_m3n[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23, 
    0xff, 0x33, 0xff, 0x04, 0x14, 0x25, 0xff, 0x35, 0xff, 0x05, 0x26, 0x36, 0x37, 0x07, 0x18, 0x28, 
    0x39, 0x09, 0x19, 0x29, 0x3a, 0x0a, 0x1a, 0x2a, 0xff, 0x3b, 0x0b, 0x1b, 0x2b, 0x3c, 0x0c, 0x1c, 
    0x2c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_m3w_h3es1[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23, 
    0xff, 0x33, 0xff, 0x04, 0x14, 0x05, 0xff, 0x15, 0xff, 0x25, 0x06, 0x16, 0x07, 0x17, 0x08, 0x18, 
    0x09, 0x19, 0x29, 0x39, 0x0a, 0x1a, 0x2a, 0x3a, 0xff, 0x0b, 0x1b, 0x2b, 0x3b, 0x0c, 0x1c, 0x2c, 
    0x3c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_v3m_e3[CSI2_FREQ_MAX] = {
    0x00, 0x20, 0x40, 0x02, 0xff, 0x22, 0x42, 0x04, 0xff, 0x24, 0x44, 0xff, 0x06, 0xff, 0x26, 0xff, 
    0x46, 0x08, 0x28, 0xff, 0x0a, 0xff, 0x2a, 0xff, 0x4a, 0x0c, 0x2c, 0x0e, 0x2e, 0x10, 0x30, 0x12, 
    0x32, 0x52, 0x72, 0x14, 0x34, 0x54, 0x74, 0xff, 0x16, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff
};

uint16_t csi2_freqs_v3u[CSI2_FREQ_MAX] = {
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31, 0x02, 0x12, 0x22, 0x32, 0xff, 0x03, 0x13, 0x23,
	0xff, 0x33, 0xff, 0x04, 0x14, 0x25, 0xff, 0x35, 0xff, 0x05, 0x16, 0x26, 0x37, 0x07, 0x18, 0x28,
	0x39, 0x09, 0x19, 0x29, 0x3a, 0x0a, 0x1a, 0x2a, 0xff, 0x3b, 0x0b, 0x1b, 0x2b, 0x3c, 0x0c, 0x1c,
	0x2c, 0x3d, 0x0d, 0x1d, 0x2e, 0x3e, 0x0e, 0x1e, 0x2f, 0x3f, 0x0f, 0x40, 0x41, 0x42, 0x43, 0x45,
	0x46, 0x47, 0x48, 0x49
};

static int rcar_csi2_phy_freq_setting(rcar_vin_t *vin, uint32_t prr_val)
{
    uint32_t  mpbs = CSI2_FREQ_80MBPS;

    switch (vin->csi_lanes) {
    case 1:
        mpbs = CSI2_FREQ_300MBPS;
        break;
    case 2:
        if (!vin->csi_interlace) {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080))
                mpbs = CSI2_FREQ_1100MBPS;
            else if ((vin->csi_width == 1280) && (vin->csi_height == 720))
                mpbs = CSI2_FREQ_900MBPS;
            else if ((vin->csi_width == 720) && (vin->csi_height == 480))
                mpbs = CSI2_FREQ_330MBPS;
            else if ((vin->csi_width == 720) && (vin->csi_height == 576))
                mpbs = CSI2_FREQ_400MBPS;
            else if ((vin->csi_width == 640) && (vin->csi_height == 480))
                mpbs = CSI2_FREQ_300MBPS;
            else
                goto reso_failed;
        } else {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080))
                mpbs = CSI2_FREQ_1000MBPS;
            else if (((vin->csi_width == 720) && (vin->csi_height == 576))
                || ((vin->csi_width == 720) && (vin->csi_height == 480)))
                mpbs = CSI2_FREQ_80MBPS;
            else 
                goto reso_failed;
        }
        break;
    case 4:
        if (!vin->csi_interlace) {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080))
                mpbs = CSI2_FREQ_900MBPS;
             /* vin frequency setting for OV490/OV10640 only */
            else if ((vin->csi_width == 1280) && (vin->csi_height == 1080)) {
                if (vin->csi_rate == 200) mpbs = CSI2_FREQ_220MBPS;
                else if (vin->csi_rate == 400) mpbs = CSI2_FREQ_400MBPS;
                else if (vin->csi_rate == 600) mpbs = CSI2_FREQ_600MBPS;
                else if (vin->csi_rate == 800) mpbs = CSI2_FREQ_800MBPS;
            }
            else if ((vin->csi_width == 1280) && (vin->csi_height == 720))
                mpbs = CSI2_FREQ_450MBPS;
            else if ((vin->csi_width == 720) && (vin->csi_height == 480))
                mpbs = CSI2_FREQ_180MBPS;
            else if ((vin->csi_width == 720) && (vin->csi_height == 576))
                mpbs = CSI2_FREQ_180MBPS;
            else if ((vin->csi_width == 640) && (vin->csi_height == 480))
                mpbs = CSI2_FREQ_100MBPS;
            else
                goto reso_failed;
        } else {
            if ((vin->csi_width == 1920) && (vin->csi_height == 1080))
                mpbs = CSI2_FREQ_450MBPS;
            else
                goto reso_failed;
        }
        break;
    default:
        fprintf(stderr, "%s: lanes is invalid (%d)\n", __FUNCTION__, vin->csi_lanes);
        return -EINVAL;
    }

	switch RCAR_GEN3_PRODUCT_ID(prr_val) 
	{
		case PRODUCT_ID_RCAR_H3:
			switch RCAR_GEN3_PRODUCT_REV(prr_val) 
			{
				case PRODUCT_REV_1_0:
				case PRODUCT_REV_1_1:
					if(csi2_freqs_m3w_h3es1[mpbs] != FREQ_MBPS_ERROR)
						out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYPLL, csi2_freqs_m3w_h3es1[mpbs] << 16);
					else 
						goto fregs_failed;
					break;
				case PRODUCT_REV_2_0:
				case PRODUCT_REV_3_0:
					if(csi2_freqs_h3_v3h_m3n[mpbs] != FREQ_MBPS_ERROR)
						out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYPLL, csi2_freqs_h3_v3h_m3n[mpbs] << 16);
					else 
						goto fregs_failed;
					break;
				default:
					break;
			}
			break;
		case PRODUCT_ID_RCAR_M3W:
			if(csi2_freqs_m3w_h3es1[mpbs] != FREQ_MBPS_ERROR)
				out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYPLL, csi2_freqs_m3w_h3es1[mpbs] << 16);
			else 
				goto fregs_failed;
			break;
		case PRODUCT_ID_RCAR_V3H:
		case PRODUCT_ID_RCAR_M3N:
			if(csi2_freqs_h3_v3h_m3n[mpbs] != FREQ_MBPS_ERROR)
				out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYPLL, csi2_freqs_h3_v3h_m3n[mpbs] << 16);
			else 
				goto fregs_failed;
			break;
		case PRODUCT_ID_RCAR_V3M:
		case PRODUCT_ID_RCAR_E3:
			if(csi2_freqs_v3m_e3[mpbs] != FREQ_MBPS_ERROR)
				out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHTW, (RCAR_CSI2_PHTW_DWEN | (csi2_freqs_v3m_e3[mpbs] << 16) |
															 RCAR_CSI2_PHTW_CWEN | RCAR_CSI2_PHTW_TESTDIN_CODE(0x44)));   
			else 
				goto fregs_failed;
			break;
		case PRODUCT_ID_RCAR_V3U:
			if(csi2_freqs_v3u[mpbs] != FREQ_MBPS_ERROR) 
				out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYPLL, csi2_freqs_v3u[mpbs] << 16);  
			else 
				goto fregs_failed;
			break;
		default:
			fprintf(stderr, "%s: Board is not supported CSI2 \n", __FUNCTION__);
			return -EINVAL;
	}

    return 0;

reso_failed:
    fprintf(stderr, "%s: not support resolution (%dx%d%c)\n",
         __FUNCTION__, vin->csi_width, vin->csi_height, (vin->csi_interlace) ? 'i' : 'p');

    return -EINVAL;

fregs_failed:
    fprintf(stderr, "%s: Set CSI2 frequency failed\n",  __FUNCTION__);
	
    return -EINVAL;
}

int rcar_csi2_fini(rcar_vin_t *vin)
{
   if (vin->csi_vbase) {	   
		out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYCNT, 0);

		/* Reset CSI2 hardware */
		out32(vin->csi_vbase + RCAR_GEN3_CSI2_SRST, 0x01);
		delay(1);
		out32(vin->csi_vbase + RCAR_GEN3_CSI2_SRST, 0x00);
	
        munmap_device_io(vin->csi_vbase, RCAR_GEN3_CSI2_SIZE);
    }

    return 0;
}

int rcar_csi2_init(rcar_vin_t *vin, int channel)
{
    unsigned 	hwi_off;
    hwi_tag 	*tag;
    int 		ret = 0;
    uint32_t 	phycnt = RCAR_CSI2_PHYCNT_ENABLECLK;
    uint32_t 	vcdt = 0;
    uint32_t 	vcdt2 = 0;
    int 		timeout;
	uintptr_t 	prr_reg = 0;
    uint32_t  	prr_val;

    vin->csi_interlace 	= vin->dec.interlace;
    vin->csi_width 		= vin->dec.sw;
    vin->csi_height 	= vin->dec.sh;

    /* CSI Physical base address */
    hwi_off = hwi_find_device(RCAR_HWI_CSI, channel);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            vin->csi_pbase = tag->location.base;
        }
    }

    /* Map base address */
    if((vin->csi_vbase = (uintptr_t)mmap_device_io(RCAR_GEN3_CSI2_SIZE, vin->csi_pbase)) == (uintptr_t)MAP_FAILED) {
        fprintf(stderr, "%s: CSI2 base mmap_device_io (0x%x) failed \r\n", __FUNCTION__, (uint32_t)vin->csi_pbase);
		return -1;
    }
	
	 if((prr_reg = mmap_device_io(4, RCAR_GEN3_PRODUCT_REGISTER)) == (uintptr_t)MAP_FAILED) {
		fprintf(stderr, "%s: CSI2 base mmap_device_io (0x%x) failed \r\n", __FUNCTION__, RCAR_GEN3_PRODUCT_REGISTER);
		ret = -1;
		goto failed;
	 }
	 
	/* Get Product ID */
	prr_val = in32(prr_reg);

    /* Registers are immediately reflected*/
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_TREF, 0x01);

    /* Reset CSI2 circuit */
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_SRST, 0x01);
    delay(1);
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_SRST, 0);
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHTC, 0);
	
    /* Set even field number and use even field detection */
    switch(vin->csi_lanes)
    {
        case 1:
            out32(vin->csi_vbase + RCAR_GEN3_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN);
            phycnt 	|= RCAR_CSI2_PHYCNT_ENABLE_0;
            vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
            break;
        case 2:
            out32(vin->csi_vbase + RCAR_GEN3_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN | 
													   RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 | 
													   RCAR_CSI2_FLD_FLD_EN4);
            phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1);
			vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
			vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(0));
            break;
        case 3:
            out32(vin->csi_vbase + RCAR_GEN3_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(1) | RCAR_CSI2_FLD_FLD_EN | 
													   RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 | 
													   RCAR_CSI2_FLD_FLD_EN4);
            phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1 | 
					    RCAR_CSI2_PHYCNT_ENABLE_2);
			vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
			vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(1));
			vcdt2 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT3_EN | RCAR_CSI2_VCDT_SEL_VC3(2));
            break;
        case 4:
			out32(vin->csi_vbase + RCAR_GEN3_CSI2_FLD, RCAR_CSI2_FLD_FLD_NUM(2) | RCAR_CSI2_FLD_FLD_EN | 
													   RCAR_CSI2_FLD_FLD_EN2 | RCAR_CSI2_FLD_FLD_EN3 | 
													   RCAR_CSI2_FLD_FLD_EN4);
            phycnt 	|= (RCAR_CSI2_PHYCNT_ENABLE_0 | RCAR_CSI2_PHYCNT_ENABLE_1 | 
					    RCAR_CSI2_PHYCNT_ENABLE_2 | RCAR_CSI2_PHYCNT_ENABLE_3);
			vcdt 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT_EN | RCAR_CSI2_VCDT_SEL_VC(0));
			vcdt 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT2_EN | RCAR_CSI2_VCDT_SEL_VC2(1));
			vcdt2 	|= (RCAR_CSI2_UYVY_DT_FORMAT | RCAR_CSI2_VCDT_VCDT3_EN | RCAR_CSI2_VCDT_SEL_VC3(2));
			vcdt2 	|= ((RCAR_CSI2_UYVY_DT_FORMAT << 16) | RCAR_CSI2_VCDT_VCDT4_EN | RCAR_CSI2_VCDT_SEL_VC4(3));
            break;
        default:
            fprintf(stderr, "%s: lanes is not supported (%d)\n", __FUNCTION__, vin->csi_lanes);
			ret = -EINVAL;
			goto failed;
    }

    /* PHY frequency setting */
    if((ret = rcar_csi2_phy_freq_setting(vin, prr_val)) < 0)
		goto failed;

    /* Enable data lanes operation */
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYCNT, phycnt);
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYCNT, phycnt | RCAR_CSI2_PHYCNT_SHUTDOWNZ);
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_PHYCNT, phycnt | RCAR_CSI2_PHYCNT_SHUTDOWNZ |
														   RCAR_CSI2_PHYCNT_RSTZ);
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_CHKSUM, RCAR_GEN3_CSI2_CHKSUM_CRC_EN | 
												  RCAR_GEN3_CSI2_CHKSUM_ECC_EN);
												  
	if(RCAR_GEN3_PRODUCT_ID(prr_val) != PRODUCT_ID_RCAR_V3U) {		
		out32(vin->csi_vbase + RCAR_GEN3_CSI2_VCDT, vcdt);
		out32(vin->csi_vbase + RCAR_GEN3_CSI2_VCDT2, vcdt2);
	}
	
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_FRDT, RCAR_GEN3_CSI2_FRDT_DT_FE);   
	delay(1);	
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_LINKCNT, RCAR_CSI2_LINKCNT_MONITOR_EN |
												   RCAR_CSI2_LINKCNT_MONI_PACT_EN |
												   RCAR_CSI2_LINKCNT_ICLK_NONSTOP);												   
    out32(vin->csi_vbase + RCAR_GEN3_CSI2_LSWAP, RCAR_CSI2_LSWAP_L0SEL(0) |
												 RCAR_CSI2_LSWAP_L1SEL(1) |
												 RCAR_CSI2_LSWAP_L2SEL(2) |
												 RCAR_CSI2_LSWAP_L3SEL(3));												 												 

    /* Wait for video decoder power down */
    delay(10);
    timeout = 100;
    /* Check stop state on the clock lane */
    while(!(in32(vin->csi_vbase + RCAR_GEN3_CSI2_PHCLM) & 0x01) ) {
        nanospin_ns(10000);
        if(!timeout--)
            break;
    }

    if (timeout == 0)
        fprintf(stderr, "%s: Clock lane is stopped\n", __FUNCTION__);

    timeout = 100;
    /* Check stop state on the data lane */
    while(!(in32(vin->csi_vbase + RCAR_GEN3_CSI2_PHDLM) & 0x01) ) {
        nanospin_ns(10000);
        if(!timeout--)
            break;
    }

    if (timeout == 0) {
        fprintf(stderr, "%s: Data lane is stopped\n", __FUNCTION__);
	}
		
failed:	

	if(prr_reg) {
		munmap_device_io(prr_reg, 4);
	}
	
    return ret;
}

