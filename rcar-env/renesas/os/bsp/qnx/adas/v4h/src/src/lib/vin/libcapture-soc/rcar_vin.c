/*
* $QNXLicenseC:
* Copyright 2020, QNX Software Systems.
* Copyright 2022, Renesas Electronics Corporation
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
#include <stdio.h>
#include "rcar_vin.h"
#include "soc.h"
#include "rcar_isp.h"
#include <hw/rcar_ipmmu-api.h>
#include <sys/syspage.h>

extern int rcar_csi2_init(rcar_vin_t *vin, int input);
extern int rcar_csi2_fini(rcar_vin_t *vin);

pthread_mutex_t mutex;

static inline unsigned
hash_func(const uint64_t key) {
    const uint32_t hash = (uint32_t)(key >> 32) + (uint32_t)key;
    return (hash >> 12) % NUM_IPMMU_OBJECTS;
}

static inline int
find_ipmmu_object(rcar_vin_t *vin, uint64_t paddr, uint32_t len, struct ipmmu_object **pobj) {
    uint32_t index = hash_func(paddr);
    struct ipmmu_object *cur = vin->ipmmu_objects[index];
    while (cur != NULL) {
        if (cur->paddr == paddr) {
            *pobj = cur;
            if (cur->len == len) {
                return 0;
            } else {
                return EBUSY;
            }
        }
        cur = cur->next;
    }

    return ENOENT;
}

static inline void
add_ipmmu_object(rcar_vin_t *vin, struct ipmmu_object *obj) {
    uint32_t index = hash_func(obj->paddr);
    struct ipmmu_object *cur = vin->ipmmu_objects[index];
    obj->next = cur;
    vin->ipmmu_objects[index] = obj;
}

static inline void
remove_ipmmu_object(rcar_vin_t *vin, struct ipmmu_object *obj) {
    uint32_t index = hash_func(obj->paddr);
    struct ipmmu_object *cur, *prev;
    cur = vin->ipmmu_objects[index];
    prev = NULL;
    while (cur != NULL) {
        if (cur == obj) {
            if (prev == NULL) {
                vin->ipmmu_objects[index] = cur->next;
            } else {
                prev->next = cur->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    free(obj);
}

static inline void
unmap_ipmmu_object(rcar_vin_t *vin, struct ipmmu_object *obj) {
    ipmmu_unmap(vin->ipmmu_handle, obj->paddr, obj->iova, obj->len);
}

#define ALIGN_MASK(x, mask)    (((x) + (mask)) & ~(mask))
#define ALIGN(x, a)            ALIGN_MASK(x, (typeof(x))(a) - 1)

static struct ipmmu_object *
map_and_insert_ipmmu_object(rcar_vin_t *vin, uint64_t paddr, uint32_t len) {
    struct ipmmu_object *obj;
    int ret;
    uint32_t aligned_len = ALIGN(len, 4096);

    if (aligned_len == 0) {
        return NULL;
    }

    if ((obj = calloc(1, sizeof(*obj))) == NULL) {
        return NULL;
    }

    ret = ipmmu_map(vin->ipmmu_handle, paddr, aligned_len, &obj->iova);
    if (ret != 0) {
        free(obj);
        return NULL;
    }

    obj->paddr = paddr;
    obj->len = aligned_len;
    add_ipmmu_object(vin, obj);

    return obj;
}

static void
cleanup_ipmmu_objects(rcar_vin_t *vin) {
    int i;
    struct ipmmu_object *obj, *next;
    for (i = 0; i < NUM_IPMMU_OBJECTS; i++) {
        next = vin->ipmmu_objects[i];
        while (next != NULL) {
            obj = next;
            unmap_ipmmu_object(vin, obj);
            next = obj->next;
            free(obj);
        }
    }

    ipmmu_close(vin->ipmmu_handle);
}

static int rcar_vin_ipmmu_map(rcar_vin_t *vin, uint64_t phy_64bit_addr, uint32_t *phy_32bit_addr, int len)
{
    struct ipmmu_object *obj;
    int ret = find_ipmmu_object(vin, phy_64bit_addr, len, &obj);
    if (ret != 0) {
        if (ret == EBUSY) {
            unmap_ipmmu_object(vin, obj);
            remove_ipmmu_object(vin, obj);
        }
        obj = map_and_insert_ipmmu_object(vin, phy_64bit_addr, len);
        if (obj == NULL) {
            //printf("%s: failed to map_and_insert_ipmmu_object paddr 0x%lx, len %d\n",
            //__func__, phy_64bit_addr, len);
            return ENOMEM;
        }
    }
    *phy_32bit_addr = obj->iova;
     return EOK;
}

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
    vin_ints = in32(vin->vbase[idx] + RCAR_VIN_INTS);
    out32(vin->vbase[idx] + RCAR_VIN_INTS, vin_ints);
    if (vin_ints & (1 << 1)) {
        slot = in32(vin->vbase[idx] + RCAR_VIN_MS);
        slot = (slot >> 3) & 3;
        if(slot < vin->nbufs[idx]) {
            vin->dec.slot[idx] = slot;
            vin->dec.latest[idx][slot] = 1;
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

        switch (pulse.code)   {
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

    /* Create vin event handler */
    if (pthread_create(&vin->tid, &vin->attr, (void *)rcar_vin_event_handler, vin)) {
        fprintf(stderr, "%s:  Unable to create event handler\n", __FUNCTION__);
        goto fail;
    }
    for (int i = 0; i < vin->vin_num; i++) {
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
    int ntsc, i;

    decoder_info_t *dec = &vin->dec;

    ratio_h = rcar_vin_ratio(dec->cw, dec->dw);
    ratio_v = rcar_vin_ratio(dec->ch, dec->dh);

    if (vin->decoder == DECODER_ADV7180) {
        /* Check captured video format */
        ntsc = rcar_check_is_ntsc(vin);
        ratio_h = rcar_vin_ratio(dec->cw - 12, dec->dw);
        if(ntsc) {
            ratio_v = rcar_vin_ratio(dec->ch - (RCAR_NTSC_TOP_GARBAGE +1)/2, dec->dh);
        } else {
            ratio_v = rcar_vin_ratio(dec->ch - (RCAR_PAL_TOP_GARBAGE +1)/2, dec->dh);
        }
    }

    bwidth_h = rcar_vin_bwidth(ratio_h);
    bwidth_v = rcar_vin_bwidth(ratio_v);

    if (dec->interlace) {
        clip_size = ((dec->dw - 1) << 16) | (dec->dh/2 - 1);
    }
    else {
        clip_size = ((dec->dw - 1) << 16) | (dec->dh - 1);
    }

    for (i = 0; i < vin->vin_num; i++) {
        if(!vin->enable[i]) continue;
        out32(vin->vbase[i] + RCAR_VIN_MC, in32(vin->vbase[i] + RCAR_VIN_MC) | RCAR_VIN_MC_SCLE);
        out32(vin->vbase[i] + RCAR_VIN_UDS_CTRL, RCAR_VIN_UDS_CTRL_AMD);
        out32(vin->vbase[i] + RCAR_VIN_UDS_SCALE, (ratio_h << SCALE_HFRAC_SHIFT) | ratio_v);
        out32(vin->vbase[i] + RCAR_VIN_UDS_PASS_BW, (bwidth_h << PASS_BW_BWIDTH_H_SHIFT) | bwidth_v);
        out32(vin->vbase[i] + RCAR_VIN_UDS_CLIPSIZE, clip_size);
        out32(vin->vbase[i] + RCAR_VIN_IS, (dec->dw + 15) & ~0xf);
    }
}

void rcar_vin_crop(rcar_vin_t* vin)
{
    decoder_info_t *dec = &vin->dec;
    int ntsc;
    int i;

    if(dec->cw == 0)
        dec->cw = dec->sw - dec->cx;

    if(dec->ch == 0)
        dec->ch = dec->sh - dec->cy;

    for (i = 0; i < vin->vin_num; i++) {
        if(!vin->enable[i]) continue;
        /* Set Start/End Pixel/Line Pre-Clip */
        out32(vin->vbase[i] + RCAR_VIN_SPPRC, dec->cx);
        if((dec->format & RCAR_VIN_MC_INF_MASK) == RCAR_VIN_MC_INF_RAW10)
            out32(vin->vbase[i] + RCAR_VIN_EPPRC, dec->cx + (dec->cw * 2) - 1);
        else
            out32(vin->vbase[i] + RCAR_VIN_EPPRC, dec->cx + dec->cw - 1);

        switch (dec->interlace) {
        case 1:
            out32(vin->vbase[i] + RCAR_VIN_SLPRC, dec->cy/2);
            out32(vin->vbase[i] + RCAR_VIN_ELPRC, (dec->cy + dec->ch)/2 - 1);

            if(vin->decoder == DECODER_ADV7180) {
                /* Check captured video format */
                ntsc = rcar_check_is_ntsc(vin);
                if(ntsc)
                    out32(vin->vbase[i] + RCAR_VIN_SLPRC, (dec->cy + RCAR_NTSC_TOP_GARBAGE)/2);
                else
                    out32(vin->vbase[i] + RCAR_VIN_SLPRC, (dec->cy + RCAR_PAL_TOP_GARBAGE)/2);
            }
            break;
        case 0:
            out32(vin->vbase[i] + RCAR_VIN_SLPRC, dec->cy);
            out32(vin->vbase[i] + RCAR_VIN_ELPRC, dec->cy + dec->ch - 1);
            break;
        }
    }
}

int rcar_vin_setup(rcar_vin_t *vin)
{
    uint32_t yuv_input = 0;
    uint32_t yuv_output = 0;
    uint32_t vnmc = 0, dmr = 0, dmr2 = 0, ifmd = 0, interrupt = 0;
    decoder_info_t *dec = &vin->dec;

    /* Input format */
    vnmc = dec->format & RCAR_VIN_MC_MASK;
    yuv_input = dec->format >> 31;

    if(vin->input_type & CAPTURE_VIA_CSI) {
       /* Capture from CSI2 */
       vnmc &= ~RCAR_VIN_MC_DPINE;
    } else if(vin->input_type & CAPTURE_VIA_ISP) {
       //For ISP if ISP core is used
    } else {
       /* Capture from Digital pins */
       vnmc |= RCAR_VIN_MC_DPINE;
    }

    /* Output format */
    switch(dec->dfmt) {
        case SCREEN_FORMAT_RGBA8888:
            dmr = RCAR_VIN_DMR_EXRGB | RCAR_VIN_DMR_DTMD_ARGB | RCAR_VIN_DMR_A8BIT;

            /* Fixme: Using SCREEN_FORMAT_RGBA8888 instead of API for RAW10 format */
            if((dec->format & RCAR_VIN_MC_INF_MASK) == RCAR_VIN_MC_INF_RAW10) {
                dmr = RCAR_VIN_DMR_RMODE_RAW10 | RCAR_VIN_DMR_YC_THR;
            }
            break;
        case SCREEN_FORMAT_RGB565:
            dmr = 0;
            break;
        case SCREEN_FORMAT_UYVY:
            dmr = 0;
            yuv_output = 1;
            break;
        case SCREEN_FORMAT_RGBA5551:
            dmr = RCAR_VIN_DMR_DTMD_ARGB | RCAR_VIN_DMR_ABIT;
            break;
    }

    /* Update on field change */
    if (vin->board != RCAR_D3_DRAAK) {
       vnmc |= RCAR_VIN_MC_VUP;
    }
    if ((yuv_input == yuv_output) && ((vin->board != RCAR_V3U_FALCON) || (vin->board != RCAR_V4H_WHITEHAWK))) {
        vnmc |= RCAR_VIN_MC_BPS;
    }

    if (vin->input_type & CAPTURE_VIA_CSI) {
        /* Select CSI virtual channel to capture */
        if(vin->input_type & INPUT_FROM_HDMI) {
            ifmd = RCAR_VIN_CSI_IFMD_CSI_CHSEL(0);
        } else if(vin->input_type & INPUT_FROM_CVBS) {
            if((vin->board == RCAR_E3_EBISU) || (vin->board == RCAR_V3M_EAGLE)) {
                ifmd = RCAR_VIN_CSI_IFMD_CSI_CHSEL(0);
                ifmd |= RCAR_VIN_CSI_IFMD_DES0;
            } else if((vin->board == RCAR_H3_SALVATOR) || (vin->board == RCAR_M3_SALVATOR)) {
                ifmd = RCAR_VIN_CSI_IFMD_CSI_CHSEL(1);
                ifmd |= RCAR_VIN_CSI_IFMD_DES1;
            }
        } else {
            ifmd = RCAR_VIN_CSI_IFMD_CSI_CHSEL(3);
            ifmd |= RCAR_VIN_CSI_IFMD_DES0;
        }
    }

    /* Bus param */
    dmr2 = RCAR_VIN_DMR2_FTEV;
    if((vin->board != RCAR_D3_DRAAK) || (vin->board != RCAR_V3U_FALCON) || (vin->board != RCAR_V4H_WHITEHAWK))
       dmr2 |= (RCAR_VIN_DMR2_VPS | RCAR_VIN_DMR2_HPS);

    for (int i = 0; i < vin->vin_num; i++) {
        if(!vin->enable[i]) continue;

        for (int j = 0; j < vin->nbufs[i]; j++) {
            if(vin->ipmmu_support) {
                uint64_t paddr64 = rcar_vin_mphys(vin->frm_bufs[i][j]);
                uint32_t paddr32;
                if (rcar_vin_ipmmu_map(vin, paddr64, &paddr32, vin->dec.dstride * vin->dec.sh) == EOK) {
                    out32(vin->vbase[i] + RCAR_VIN_MB(j), paddr32);
                } else {
                    out32(vin->vbase[i] + RCAR_VIN_MB(j), paddr64);
                }
            } else {
                out32(vin->vbase[i] + RCAR_VIN_MB(j), rcar_vin_mphys(vin->frm_bufs[i][j]));
            }
        }
    }

    /* Interrupt type */
    interrupt = dec->interlace ? RCAR_VIN_IE_EFE : RCAR_VIN_IE_FIE;

    /* Apply setup */
    for (int i = 0; i < vin->vin_num; i++) {
        if(!vin->enable[i]) continue;
        if(vin->input_type & CAPTURE_VIA_CSI) {
            out32(vin->vbase[i] + RCAR_VIN_CSI_IFMD, ifmd);
        }
        out32(vin->vbase[i] + RCAR_VIN_DMR, dmr);
        out32(vin->vbase[i] + RCAR_VIN_DMR2, RCAR_VIN_DMR2_FTEV);
        out32(vin->vbase[i] + RCAR_VIN_MC, vnmc | RCAR_VIN_MC_ME);
        out32(vin->vbase[i] + RCAR_VIN_IS, (dec->dw + 15) & ~0xF);
    }

    /* Crop and scale */
    rcar_vin_crop(vin);
    if (vin->scale_support) {
        rcar_vin_scale(vin);
    }

    for(int i = 0; i < vin->vin_num; i++) {
        if(!vin->enable[i]) continue;
        /* Start */
        out32(vin->vbase[i] + RCAR_VIN_IE, interrupt);
        out32(vin->vbase[i] + RCAR_VIN_FC, RCAR_VIN_FC_C_FRAME);
    }

    return 0;
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
    pthread_condattr_t attr;
    int i;

    rcar_vin_query_hwi_device(vin);

    /* Map base address */
    for(i = 0; i < vin->vin_num; i++) {
        if ((vin->vbase[i] = (uintptr_t)mmap_device_io(RCAR_VIN_SIZE, vin->pbase[i])) == (uintptr_t)MAP_FAILED) {
            fprintf(stderr, "%s: VIN base mmap_device_io (0x%x) failed", __FUNCTION__, (uint32_t)vin->pbase[i]);
            rcar_vin_fini(vin);
            return (errno);
        }
    }

    /* Signal */
    pthread_mutex_init(&vin->mutex, NULL);

    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    for(i = 0; i < vin->vin_num; i++) {
        pthread_cond_init(&vin->cond[i], &attr);
    }

    if (vin->csi_lanes > vin->csi_lanes_max)
        vin->csi_lanes = vin->csi_lanes_max;

    for(i = 0; i < vin->vin_num; i++) {
        vin->nbufs[i] = vin->frm_nbufs[i];
        if (vin->nbufs[i] > RCAR_VIN_MAX_BUFFER) {
            vin->nbufs[i] = RCAR_VIN_MAX_BUFFER;
        }
    }

    /* Thread for handle interrupt */
    if(rcar_vin_create_thread(vin)) {
        fprintf(stderr, "%s: create interrupt handler failed\n", __FUNCTION__);
        return -1;
    }

    /* CSI2->VIN / CSI2->ISP->VIN */
    if((vin->input_type & CAPTURE_VIA_CSI) || vin->input_type & CAPTURE_VIA_ISP) {
        if (vin->csi_phy_mode == 0)
            vin->csi_phy_mode = CSI2_PHY_MODE_CPHY;
        if(rcar_csi2_init(vin, vin->csi_chan) != 0)
            return -1;

        /* CSI2->ISP->VIN */
        if(vin->input_type & CAPTURE_VIA_ISP) {
            if(rcar_isp_init(vin, vin->csi_chan) != 0)
                return -1;
        }
    }

    if(vin->ipmmu_support) {
        vin->ipmmu_handle = ipmmu_open("vin", vin->vin_chan);
        if (vin->ipmmu_handle < 0) {
            printf("%s: Unable to ipmmu_open %s", __func__, strerror(errno));
            return -1;
        }
    }

    /* Enable VIN */
    rcar_vin_setup(vin);

    return 0;
}

int rcar_vin_fini(rcar_vin_t *vin)
{
    int i = 0, j = 0, loop;

     /* Stop VIN */
    for(i = 0; i < vin->vin_num; i++){
        out32(vin->vbase[i] + RCAR_VIN_FC, 0);
        out32(vin->vbase[i] + RCAR_VIN_IE, 0);
        out32(vin->vbase[i] + RCAR_VIN_MC, 0);
        vin->enable[i] = 0;
    }

    loop = 100;
    while (1) {
        --loop;
        if (!loop || !(vin->frm_end & 1))
        break;
        delay(1);
    }

    for (i = 0; i < vin->vin_num; i++) {
        for (j = 0; j < vin->nbufs[i]; j++) {
            out32(vin->vbase[i] + RCAR_VIN_MB(j), 0);
        }
    }

    MsgSendPulse(vin->coid, 21, RCAR_VIN_END, 0);
    pthread_join(vin->tid, NULL);
    for(i = 0; i < vin->vin_num; i++) {
        InterruptDetach(vin->iid[i]);
    }

    ConnectDetach(vin->coid);
    ChannelDestroy(vin->chid);

    pthread_mutex_destroy(&vin->mutex);

    for(i = 0; i < vin->vin_num; i++) {
        pthread_cond_destroy(&vin->cond[i]);
        munmap_device_io(vin->vbase[i], RCAR_VIN_SIZE);
    }

    /* Stop CSI2 */
    if((vin->input_type & CAPTURE_VIA_CSI)) {
        rcar_csi2_fini(vin);
    }

    if((vin->input_type & CAPTURE_VIA_ISP)) {
        rcar_csi2_fini(vin);

        rcar_isp_fini(vin);
    }

    if(vin->ipmmu_support)
        cleanup_ipmmu_objects(vin);

    return 0;
}

int rcar_get_frame(rcar_vin_t *vin, uint64_t timeout, uint32_t idx)
{
    int ret;
    int slot;
    struct timespec from;
    struct timespec to;
    uint64_t time_from;
    uint64_t time_to;

    pthread_mutex_lock(&mutex);

#if 1
    slot = vin->dec.release[idx];
    for (int i = 0; i < vin->nbufs[idx]; i++) {
        ++slot;
        slot %= vin->nbufs[idx];
        if (vin->dec.latest[idx][slot] == 1) {
            vin->dec.release[idx] = slot;
            pthread_mutex_unlock(&mutex);
            return slot;
        }
    }
#else
    slot = vin->dec.slot[idx];
    if (vin->dec.latest[idx][slot] == 1) {
        vin->dec.release[idx] = slot;
        pthread_mutex_unlock(&mutex);
        return slot;
    }
#endif
    clock_gettime(CLOCK_MONOTONIC, &from);
    time_from = timespec2nsec(&from);
    if(timeout == CAPTURE_TIMEOUT_INFINITE) {
        time_to = CAPTURE_TIMEOUT_INFINITE;
    }
    else {
        time_to = time_from + timeout;
    }
    nsec2timespec(&to, time_to * 10);
    ret = pthread_cond_timedwait(&vin->cond[idx], &mutex, &to);
    if (ret == EOK) {
        slot = vin->dec.slot[idx];
        if (vin->dec.latest[idx][slot] == 1) {
            vin->dec.release[idx] = slot;
            pthread_mutex_unlock(&mutex);
            return slot;
        } else {
            errno = ENOTSUP;
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    else if(ret == ETIMEDOUT) {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    else {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
}

int rcar_release_frame(rcar_vin_t *vin, uint32_t idx, uint32_t slot)
{
    if(slot > vin->nbufs[idx]) {
        errno = EINVAL;
        return -1;
    }

    vin->dec.latest[idx][slot] = 0;

    return 0;
}
