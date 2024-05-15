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

#include "rcar_isp.h"

void rcar_isp_fini(rcar_vin_t *vin)
{
	if (vin->isp_vbase) {	   
        munmap_device_io(vin->isp_vbase, RCAR_GEN3_ISP_SIZE);
    }
}

int rcar_isp_init(rcar_vin_t *vin, uint32_t isp_chan)
{
	unsigned 		hwi_off;
    hwi_tag 		*tag;
	decoder_info_t 	*dec = &vin->dec;
	int 			ch, vc;
	uint32_t 		tmp;
	
	/* ISP Physical base address */
    hwi_off = hwi_find_device(RCAR_HWI_ISP, isp_chan);
    if(hwi_off != HWI_NULL_OFF) {
        tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
        if(tag) {
            vin->isp_pbase = tag->location.base;
        }
    }
	
	if((vin->isp_vbase = (uintptr_t)mmap_device_io(RCAR_GEN3_ISP_SIZE, vin->isp_pbase)) == (uintptr_t)MAP_FAILED) {
        fprintf(stderr, "%s: CSI2 base mmap_device_io (0x%x) failed \r\n", __FUNCTION__, (uint32_t)vin->isp_pbase);
		return -1;
    }
	
	/* Virtual channel */	
	for (ch = 4, vc = 0; ch < 8 && vc < 4; ch++, vc++) {
		out32(vin->isp_vbase + RCAR_GEN3_ISPCS_FILTER_ID_CHn(ch), (0x01 << vc));
		out32(vin->isp_vbase + RCAR_GEN3_ISPCS_LC_MODULO_CHn(ch), 0x00000000);

		/* MD Filter and pixel reconstructor method */
		switch(dec->dfmt)
		{
			case SCREEN_FORMAT_RGBA8888:
				out32(vin->isp_vbase + RCAR_GEN3_ISPCS_DT_CODE03_CHn(ch), RCAR_CSI2_RGB888_DT_FORMAT | RCAR_GEN3_ISPCS_DT_CODE03_ENABLE0);
				tmp = in32(vin->isp_vbase + RCAR_GEN3_ISP_PROCMODE_DT(RCAR_CSI2_RGB888_DT_FORMAT));
				out32(vin->isp_vbase + RCAR_GEN3_ISP_PROCMODE_DT(RCAR_CSI2_RGB888_DT_FORMAT), tmp | (RCAR_GEN3_ISP_PROCMODE_DT_RGB888 << (vc * 8)));
				break;
			case SCREEN_FORMAT_UYVY:
				out32(vin->isp_vbase + RCAR_GEN3_ISPCS_DT_CODE03_CHn(ch), RCAR_CSI2_UYVY_DT_FORMAT | RCAR_GEN3_ISPCS_DT_CODE03_ENABLE0);
				tmp = in32(vin->isp_vbase + RCAR_GEN3_ISP_PROCMODE_DT(RCAR_CSI2_UYVY_DT_FORMAT));
				out32(vin->isp_vbase + RCAR_GEN3_ISP_PROCMODE_DT(RCAR_CSI2_UYVY_DT_FORMAT), tmp | (RCAR_GEN3_ISP_PROCMODE_DT_YU422_8BIT << (vc * 8)));
				break;
		}
	}
	
	/* Write Protect */
	out32(vin->isp_vbase + RCAR_GEN3_ISPWP_CTRL, RCAR_GEN3_ISPWP_CTRL_UNLOCK_CODE_U | RCAR_GEN3_ISPWP_CTRL_UNLOCK_CODE_L);
	
	/* Output Channnel select */
	out32(vin->isp_vbase + RCAR_GEN3_ISPINPUTSEL0, 0x40010000 | RCAR_GEN3_ISPINPUTSEL0_CSI_SEL);
	if(isp_chan == 1 || isp_chan == 3)
		out32(vin->isp_vbase + RCAR_GEN3_ISPINPUTSEL0, (1 << 31) | 0x40010000 | RCAR_GEN3_ISPINPUTSEL0_CSI_SEL);

	/* Number of cycles for padding */
	out32(vin->isp_vbase + RCAR_GEN3_ISP_PADDING_CTRL, 0x7008000 | RCAR_GEN3_ISP_PADDING_CTRL_PADDING_NUM(32));
	
	/* ISP Start */
	out32(vin->isp_vbase + RCAR_GEN3_ISPSTART, RCAR_GEN3_ISPSTART_START_ISP);
	
	
	/* Number of cycles for padding */
	out32(vin->isp_vbase + RCAR_GEN3_ISP_PADDING_CTRL, (0x70080 << 12) | RCAR_GEN3_ISP_PADDING_CTRL_PADDING_NUM(32));
	
	/* Output mode: Channel selector output */
	out32(vin->isp_vbase + RCAR_GEN3_ISPCS_OUTPUT_0TO3_MODE_CHn(isp_chan), RCAR_GEN3_ISPCS_OUTPUT_MODE_OUTPUT_SEL(0));

	/* FIFO control */
	out32(vin->isp_vbase + RCAR_GEN3_ISPFIFOCTRL, RCAR_GEN3_ISPFIFOCTRL_FIFO_PUSH_CSI);

	return 0;
}
