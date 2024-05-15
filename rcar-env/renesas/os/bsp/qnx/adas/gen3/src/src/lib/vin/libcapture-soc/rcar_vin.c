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

#include "rcar_vin.h"
#include "soc.h"
#include "rcar_isp.h"

extern int rcar_csi2_init(rcar_vin_t *vin, int input);
extern int rcar_csi2_fini(rcar_vin_t *vin);

pthread_mutex_t mutex;

paddr_t rcar_vin_mphys(void *addr)
{
    off64_t offset;

    if(mem_offset64(addr, NOFD, 1, &offset, 0) == -1) {
        return -1;
    }
    return offset;
}

void rcar_vin_get_slot(rcar_vin_t *vin, int idx)
{
    uint32_t vin_ints = 0;
    uint32_t slot = 0;

    atomic_set(&vin->frm_end, 1);
    vin_ints = in32(vin->vbase[idx] + RCAR_GEN3_VIN_INTS);
    out32(vin->vbase[idx] + RCAR_GEN3_VIN_INTS, vin_ints);  
    if((vin_ints & (1 << 1))||(vin_ints & (1 << 4))) {
        slot = in32(vin->vbase[idx] + RCAR_GEN3_VIN_MS);
        slot &= (3 << 3);
        slot = slot >> 3;
        if(slot < vin->nbufs) {
            vin->dec.latest[idx] = slot;
            pthread_cond_broadcast(&vin->cond[idx]);
        }
    }
    InterruptUnmask(vin->irq[idx], vin->iid[idx]);
    atomic_clr_value(&vin->frm_end, 1);
}

void *rcar_vin_event_handler(void *data)
{
    rcar_vin_t *vin = (rcar_vin_t *)data;
    struct _pulse pulse;
    iov_t iov;
    int rcvid;

    SETIOV(&iov, &pulse, sizeof(pulse));

    vin->frm_end = 0;
    for (;;) {
        if ((rcvid = MsgReceivev(vin->chid, &iov, 1, NULL)) == -1)
            continue;
        
        switch (pulse.code) 
        {
            case RCAR_VIN_PULSE(0):     
            case RCAR_VIN_PULSE(1): 
            case RCAR_VIN_PULSE(2):     
            case RCAR_VIN_PULSE(3): 
                rcar_vin_get_slot(vin, pulse.code - RCAR_VIN_PULSE(0));             
                break;
            case RCAR_VIN_END:
                return NULL;
            default:
                if (rcvid)
                    MsgReplyv(rcvid, ENOTSUP, &iov, 1);
                break;
        }       
    }
    
    return NULL;
}

int rcar_vin_create_thread(rcar_vin_t* vin)
{
    ThreadCtl(_NTO_TCTL_IO, 0);

    if ((vin->chid = ChannelCreate(_NTO_CHF_DISCONNECT | _NTO_CHF_UNBLOCK)) == -1)
        return -1;

    if ((vin->coid = ConnectAttach(0, 0, vin->chid, _NTO_SIDE_CHANNEL, 0)) == -1)
        goto fail;

    pthread_attr_init(&vin->attr);
    pthread_attr_setschedpolicy(&vin->attr, SCHED_RR);
    vin->param.sched_priority = RCAR_VIN_PRIORITY;
    pthread_attr_setschedparam(&vin->attr, &vin->param);
    pthread_attr_setinheritsched(&vin->attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setstacksize(&vin->attr, RCAR_VIN_STACK_SIZE);

    // Create vin event handler 
    if (pthread_create(&vin->tid, &vin->attr, (void *)rcar_vin_event_handler, vin)) {
        fprintf(stderr, "%s:  Unable to create event handler\n", __FUNCTION__);
        goto fail;
    }
    for(int i = 0; i < vin->vin_num; i++){
        vin->event[i].sigev_notify   = SIGEV_PULSE;
        vin->event[i].sigev_coid     = vin->coid;
        vin->event[i].sigev_code     = RCAR_VIN_PULSE(i);
        vin->event[i].sigev_priority = 21;
        if ((vin->iid[i] = InterruptAttachEvent(vin->irq[i], &vin->event[i], _NTO_INTR_FLAGS_TRK_MSK|_NTO_INTR_FLAGS_END)) == -1){
                fprintf(stderr,"%s: Interrupt attach failed.\n", __FUNCTION__);
                goto fail;
        }
    }
    return 0;

fail:
    ConnectDetach(vin->coid);
    ChannelDestroy(vin->chid);

    return -1;
}

uint32_t rcar_vin_ratio(uint32_t input, uint32_t output)
{
    if (output > input)
        return input * 4096 / output;
    else
        return (input - 1) * 4096 / (output - 1);
}

uint32_t rcar_vin_bwidth(uint32_t ratio)
{
    uint32_t bwidth;
    uint32_t mant, frac;

    mant = (ratio & 0xF000) >> 12;
    frac = ratio & 0xFF;

    if (mant)
        bwidth = 64 * 4096 * mant / (4096 * mant + frac);
    else
        bwidth = 64;

    return bwidth;
}

int rcar_check_is_ntsc(rcar_vin_t* vin)
{
    decoder_info_t *dec = &vin->dec;

    if(strstr( dec->sfmt, "NTSC"))
        return 1;
    else 
        return 0;   
}

void rcar_vin_scale(rcar_vin_t* vin)
{
    uint32_t ratio_h = 0, ratio_v = 0;
    uint32_t bwidth_h, bwidth_v;
    uint32_t clip_size;
    int ntsc;

    decoder_info_t *dec = &vin->dec;

    ratio_h = rcar_vin_ratio(dec->cw, dec->dw);
    ratio_v = rcar_vin_ratio(dec->ch, dec->dh);

    if(vin->decoder == DECODER_ADV7180) {
        /* Check captured video format */
        ntsc = rcar_check_is_ntsc(vin);
        ratio_h = rcar_vin_ratio(dec->cw - 12, dec->dw);
        if(ntsc)
            ratio_v = rcar_vin_ratio(dec->ch - (RCAR_NTSC_TOP_GARBAGE + 1) / 2, dec->dh);
        else
            ratio_v = rcar_vin_ratio(dec->ch - (RCAR_PAL_TOP_GARBAGE + 1) / 2, dec->dh);
    }

    bwidth_h = rcar_vin_bwidth(ratio_h);
    bwidth_v = rcar_vin_bwidth(ratio_v);

    if(dec->interlace) {
        clip_size = ((dec->dw - 1) << 16) | (dec->dh / 2 - 1);
    }
    else {
        clip_size = ((dec->dw - 1) << 16) | (dec->dh - 1);
    }

    for(int i = 0; i < vin->vin_num; i++){
        out32(vin->vbase[i] + RCAR_GEN3_VIN_MC, in32(vin->vbase[i] + RCAR_GEN3_VIN_MC) | RCAR_GEN3_VIN_MC_SCLE);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_UDS_CTRL, RCAR_GEN3_VIN_UDS_CTRL_AMD);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_UDS_SCALE, (ratio_h << SCALE_HFRAC_SHIFT) | ratio_v);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_UDS_PASS_BW, (bwidth_h << PASS_BW_BWIDTH_H_SHIFT) | bwidth_v);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_UDS_CLIPSIZE, clip_size);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_IS, (dec->dw + 15) & ~0xf);
    }
}

void rcar_vin_crop(rcar_vin_t* vin)
{
    decoder_info_t *dec = &vin->dec;
    int ntsc;

    if(dec->cw == 0)
        dec->cw = dec->sw - dec->cx;

    if(dec->ch == 0)
        dec->ch = dec->sh - dec->cy;
     /* Set Start/End Pixel/Line Pre-Clip */
    for(int i = 0; i < vin->vin_num; i++) {
        /* Set Start/End Pixel/Line Pre-Clip */
        out32(vin->vbase[i] + RCAR_GEN3_VIN_SPPRC, dec->cx);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_EPPRC, dec->cx + dec->cw - 1);
        switch (dec->interlace)
        {
            case 1:
                out32(vin->vbase[i] + RCAR_GEN3_VIN_SLPRC, dec->cy / 2);
                out32(vin->vbase[i] + RCAR_GEN3_VIN_ELPRC, (dec->cy + dec->ch) / 2 - 1);
    
                if(vin->decoder == DECODER_ADV7180) {
                    /* Check captured video format */
                    ntsc = rcar_check_is_ntsc(vin);
                    if(ntsc)
                        out32(vin->vbase[i] + RCAR_GEN3_VIN_SLPRC, (dec->cy + RCAR_NTSC_TOP_GARBAGE) / 2);
                    else
                        out32(vin->vbase[i] + RCAR_GEN3_VIN_SLPRC, (dec->cy + RCAR_PAL_TOP_GARBAGE) / 2);
                }
                break;
            case 0:
                out32(vin->vbase[i] + RCAR_GEN3_VIN_SLPRC, dec->cy);
                out32(vin->vbase[i] + RCAR_GEN3_VIN_ELPRC, dec->cy + dec->ch - 1);
                break;
        }
    }
}

void rcar_vin_setup(rcar_vin_t *vin)
{
    uint32_t yuv_input = 0;
    uint32_t yuv_output = 0;
    uint32_t vnmc = 0, dmr, dmr2, ifmd, interrupt;
    decoder_info_t *dec = &vin->dec;

    vnmc = dec->format & RCAR_GEN3_VIN_MC_MASK;
    yuv_input = dec->format >> 31;
    yuv_output = 0;

    if(vin->input_type & CAPTURE_VIA_CSI) {
       /* Capture from CSI2 */
		vnmc &= ~RCAR_GEN3_VIN_MC_DPINE;
    } else if(vin->input_type & CAPTURE_VIA_ISP) {
		//For ISP if ISP core is used
		
	} else {
		/* Capture from Digital pins */
		vnmc |= RCAR_GEN3_VIN_MC_DPINE;
    }

    /* Output format */
    switch(dec->dfmt)
    {
        case SCREEN_FORMAT_RGBA8888:
            dmr = RCAR_GEN3_VIN_DMR_EXRGB | RCAR_GEN3_VIN_DMR_DTMD_ARGB | RCAR_GEN3_VIN_DMR_A8BIT;
            break;
        case SCREEN_FORMAT_RGB565:
            dmr = 0;
            break;
        case SCREEN_FORMAT_UYVY:
            dmr = 0;
            yuv_output = 1;
            break;
        case SCREEN_FORMAT_RGBA5551:
            dmr = RCAR_GEN3_VIN_DMR_DTMD_ARGB | RCAR_GEN3_VIN_DMR_ABIT;
            break;
    }

    /* Update on field change */
    if(vin->board != RCAR_D3_DRAAK) {
       vnmc |= RCAR_GEN3_VIN_MC_VUP;
    }
    if((yuv_input == yuv_output) && (vin->board != RCAR_V3U_FALCON)){
        vnmc |= RCAR_GEN3_VIN_MC_BPS;
    }

    if (vin->input_type & CAPTURE_VIA_CSI) {
        /* Select CSI virtual channel to capture */
        if(vin->input_type & INPUT_FROM_HDMI) {
            ifmd = RCAR_GEN3_VIN_CSI_IFMD_CSI_CHSEL(0);
        } else if(vin->input_type & INPUT_FROM_CVBS) {
            if((vin->board == RCAR_E3_EBISU) || (vin->board == RCAR_V3M_EAGLE)) {
                ifmd = RCAR_GEN3_VIN_CSI_IFMD_CSI_CHSEL(0);
                ifmd |= RCAR_GEN3_VIN_CSI_IFMD_DES0;
            } else if((vin->board == RCAR_H3_SALVATOR) || (vin->board == RCAR_M3_SALVATOR)) {
                ifmd = RCAR_GEN3_VIN_CSI_IFMD_CSI_CHSEL(1);
                ifmd |= RCAR_GEN3_VIN_CSI_IFMD_DES1;
            }
        } else {
            ifmd = RCAR_GEN3_VIN_CSI_IFMD_CSI_CHSEL(3);
            ifmd |= RCAR_GEN3_VIN_CSI_IFMD_DES0;
        }
    }

    /* Bus param */
    dmr2 = RCAR_GEN3_VIN_DMR2_FTEV;
    if((vin->board != RCAR_D3_DRAAK) && (vin->board != RCAR_V3U_FALCON))
       dmr2 |= (RCAR_GEN3_VIN_DMR2_VPS | RCAR_GEN3_VIN_DMR2_HPS);

    /* Maximum supported capture buffer */
    vin->nbufs = vin->frm_nbufs;
    if(vin->frm_nbufs >= RCAR_VIN_MAX_BUFFER)
        vin->nbufs = RCAR_VIN_MAX_BUFFER;
    
    for(int i = 0; i < vin->vin_num; i++){      
        for(int j = 0; j < vin->nbufs; j++) { 
            out32(vin->vbase[i] + RCAR_GEN3_VIN_MB(j), rcar_vin_mphys(vin->frm_bufs[i][j]));	
        }
    }

    /* Interrupt type */
    interrupt = dec->interlace ? RCAR_GEN3_VIN_IE_EFE : RCAR_GEN3_VIN_IE_FIE;

    /* Apply setup */
    for(int i = 0; i < vin->vin_num; i++){
        if(vin->input_type & CAPTURE_VIA_CSI) {
            out32(vin->vbase[i] + RCAR_GEN3_VIN_CSI_IFMD, ifmd);      		
		}
        out32(vin->vbase[i] + RCAR_GEN3_VIN_DMR, dmr);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_DMR2, dmr2);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_MC, vnmc | RCAR_GEN3_VIN_MC_ME);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_IS, (dec->dw + 15) & ~0xF);
    }

    /* Crop and scale */
    rcar_vin_crop(vin);
    if(vin->scale_support) {
        rcar_vin_scale(vin);
    }

    /* Wait for interrupt */
    if(rcar_vin_create_thread(vin)) {
        fprintf(stderr, "%s: create interrupt handler failed\n", __FUNCTION__);
    }

    for(int i = 0; i < vin->vin_num; i++){
        /* Start */
        out32(vin->vbase[i] + RCAR_GEN3_VIN_IE, interrupt);
        out32(vin->vbase[i] + RCAR_GEN3_VIN_FC, RCAR_GEN3_VIN_FC_C_FRAME);
    }
}

int rcar_vin_update(rcar_vin_t *vin)
{
    decoder_info_t* dec = &vin->dec;

    if(dec->update & SOC_SCALING_UPDATE) {
        if(vin->scale_support) {
            rcar_vin_scale(vin);
        }

        dec->update &= ~SOC_SCALING_UPDATE;
    }

    if(dec->update & SOC_CROPPING_UPDATE) {
        rcar_vin_crop(vin);
        dec->update &= ~SOC_CROPPING_UPDATE;
    }

    return 0;
}

int rcar_vin_fini(rcar_vin_t *vin)
{
    int i = 0, j = 0, loop;

     /* Stop VIN */
    for(i = 0; i < vin->vin_num; i++){
        out32(vin->vbase[i] + RCAR_GEN3_VIN_FC, 0);  
        out32(vin->vbase[i] + RCAR_GEN3_VIN_IE, 0); 
        out32(vin->vbase[i] + RCAR_GEN3_VIN_MC, 0);
    }

    loop = 100;
    while (1) {
        --loop;
        if (!loop || !(vin->frm_end & 1))
        break;
        delay(1);
    }

    for(i = 0; i < vin->vin_num; i++){      
        for(j = 0; j < vin->nbufs; j++) { 
            out32(vin->vbase[i] + RCAR_GEN3_VIN_MB(j), 0);
        }
    }

    MsgSendPulse(vin->coid, 21, RCAR_VIN_END, 0);
    pthread_join(vin->tid, NULL);
    for(i = 0; i < vin->vin_num; i++)
        InterruptDetach(vin->iid[i]);
    ConnectDetach(vin->coid);
    ChannelDestroy(vin->chid);

    for(i = 0; i < vin->vin_num; i++)
        pthread_cond_destroy(&vin->cond[i]);
    pthread_mutex_destroy(&vin->mutex);

    for(i = 0; i < vin->vin_num; i++)
        munmap_device_io(vin->vbase[i], RCAR_GEN3_VIN_SIZE);
    
    /* Stop CSI2 */
    if((vin->input_type & CAPTURE_VIA_CSI)) {
        rcar_csi2_fini(vin);
    } 
	
	if((vin->input_type & CAPTURE_VIA_ISP)) {
        rcar_csi2_fini(vin);
        rcar_isp_fini(vin);
    } 

    return 0;
}

void rcar_vin_query_hwi_device(rcar_vin_t *vin)
{
    int idx;
    unsigned hwi_off;
    hwi_tag *tag;
        
    for(idx = 0; idx < vin->vin_num; idx++){
        hwi_off = hwi_find_device(RCAR_HWI_VIN, vin->vin_chan + idx);
        if(hwi_off != HWI_NULL_OFF){
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_location, 0);
            if(tag) {
                vin->pbase[idx] = tag->location.base;
            }
            tag = hwi_tag_find(hwi_off, HWI_TAG_NAME_irq, 0);
            if(tag){
                vin->irq[idx] = tag->irq.vector;
            }
        }
    }
}

int rcar_vin_init(rcar_vin_t *vin)
{
    pthread_condattr_t attr[vin->vin_num];    

    rcar_vin_query_hwi_device(vin);
    
    /* Map base address */
    for(int i = 0; i < vin->vin_num; i++){
        if ((vin->vbase[i] = (uintptr_t)mmap_device_io(RCAR_GEN3_VIN_SIZE, vin->pbase[i])) == (uintptr_t)MAP_FAILED) {
            fprintf(stderr, "%s: VIN base mmap_device_io (0x%x) failed", __FUNCTION__, (uint32_t)vin->pbase[i]);
            rcar_vin_fini(vin);
            return (errno);
        }   
    }

    /* Signal */
    pthread_mutex_init(&vin->mutex, NULL);
    for(int i = 0; i < vin->vin_num; i++){
        pthread_condattr_init(&attr[i]);
        pthread_condattr_setclock(&attr[i], CLOCK_MONOTONIC);
        pthread_cond_init(&vin->cond[i], &attr[i]);
    }

    /* CSI2->VIN */ 
    if(vin->input_type & CAPTURE_VIA_CSI) {
        if(rcar_csi2_init(vin, vin->csi2_chan) != 0) 
			return -1;
    }
	
	/* CSI2->ISP->VIN */	
	if(vin->input_type & CAPTURE_VIA_ISP) {	
		if(rcar_csi2_init(vin, vin->csi2_chan) != 0) 
			return -1;
		if(rcar_isp_init(vin, vin->csi2_chan) != 0) 
			return -1;
	}

    /* Enable VIN */
    rcar_vin_setup(vin);

    return 0;
}

int rcar_get_frame(rcar_vin_t *vin, uint64_t timeout, uint32_t idx)
{   
    int ret;
    struct timespec from;
    struct timespec to;
    uint64_t time_from;
    uint64_t time_to;

    clock_gettime(CLOCK_MONOTONIC, &from);
    time_from = timespec2nsec(&from);

    if(timeout == CAPTURE_TIMEOUT_INFINITE) {
        time_to = CAPTURE_TIMEOUT_INFINITE;
    }
    else {
        time_to = time_from + timeout;
    }
    nsec2timespec(&to, time_to);
    while(1) {
        ret = pthread_cond_timedwait(&vin->cond[idx], &mutex, &to);
        if (ret == EOK) {
            break;
        }   
        else if(ret == ETIMEDOUT)
        {
            errno = ETIMEDOUT;
            return -1;
        }
        else {
            return -1;  
        }               
    }

    if(vin->dec.latest[idx] < vin->nbufs) {
        vin->dec.release[idx] = vin->dec.latest[idx];
        return vin->dec.release[idx];
    }

    return 0;
}

int rcar_release_frame(rcar_vin_t *vin, uint32_t idx)
{   
    if(idx > vin->nbufs) {
        errno = EINVAL;
        return -1;
    }

    vin->dec.done[idx] = 1;

    return 0;
}
