/*
 * $QNXLicenseC:
 * Copyright 2017, QNX Software Systems.
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

#include "rswitch.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <qnx/ptp.h>

#define NSEC_PER_SEC        1000000000
#define PTP_INCR            0x19000000


ptp_extts_t rx_ts[RSWITCH2_RX_TIMESTAMP_BUF_SZ];
uint32_t    rx_ts_cnt;
ptp_extts_t tx_ts[RSWITCH2_TX_TIMESTAMP_BUF_SZ];
uint32_t    tx_ts_cnt;


void rswitch2_ptp_init(rswitch2_dev_t *rswitch2)
{
    mtx_lock(&rswitch2->mtx);

    bus_write_4(rswitch2->res[0], PTPTIVC0, PTP_INCR);
    bus_write_4(rswitch2->res[0], PTPTMEC, PTPTMEC_TE);

    mtx_unlock(&rswitch2->mtx);
}

int rswitch2_ptp_is_eventmsg(struct mbuf *m, ptpv2hdr_t **ph)
{
    int                 remain, pktlen;
    struct ether_header *eh;
    struct ip           *iph;
    struct udphdr       *udph;

    if (m == NULL) {
        //device_printf(rswitch2->dev, "%s: NULL mbuf", __FUNCTION__);
        return 0;
    }

    if (m->m_pkthdr.len <
        sizeof(struct ether_header) + sizeof(ptpv2hdr_t)) {
        return 0;
    }

    eh = (struct ether_header *)m->m_data;

    switch(ntohs(eh->ether_type)) {
        case ETHERTYPE_PTP:
            /* This is a native ethernet frame
             * defined for IEEE1588v2
             */
            remain = sizeof(struct ether_header);
            while (remain >= m->m_len) {
                remain -= m->m_len;
                m = m->m_next;
            }
            if ((m->m_len - remain) < sizeof(ptpv2hdr_t)) {
                return 0;
            }
            if (ph != NULL) {
                *ph = (ptpv2hdr_t *)(m->m_data + remain);
            }
            if((((ptpv2hdr_t *)(m->m_data + remain))->messageId & 0x0f)>7) {
                return 0;
            }
            return 1;
            break;
        case ETHERTYPE_IP:
            pktlen = m->m_pkthdr.len;

            remain = sizeof(struct ether_header);
            while (remain >= m->m_len) {
                remain -= m->m_len;
                m = m->m_next;
            }
            iph = (struct ip*)(m->m_data + remain);

            if (iph->ip_p != IPPROTO_UDP) {
                return 0;
            }

            if (pktlen < sizeof(struct ether_header) + (iph->ip_hl << 2) +
                sizeof(struct udphdr) + sizeof (ptpv2hdr_t)) {
                return 0;
            }

            remain += iph->ip_hl << 2;
            while (remain >= m->m_len) {
                remain -= m->m_len;
                m = m->m_next;
            }
            udph = (struct udphdr*)(m->m_data + remain);

            if (ntohs(udph->uh_dport) != PTP_PORT) {
                return 0;
            }

            remain += sizeof(struct udphdr);
            while (remain >= m->m_len) {
                remain -= m->m_len;
                m = m->m_next;
            }
            if ((m->m_len - remain) < sizeof(ptpv2hdr_t)) {
                return 0;
            }
            if (ph != NULL) {
                *ph = (ptpv2hdr_t*)(m->m_data + remain);
            }
            return 1;
            break;
        default:
            // VLAN & IPv6 protocols are not supported
            break;
    }
    return 0;
}

void rswitch2_ptp_add_rx_timestamp(rswitch2_dev_t *rswitch2, ptpv2hdr_t *ph, RSWITCH2_RX_DESC *bd)
{
    if ((ph == NULL) || (bd == NULL) ||
        ((ph->version & 0x0f) != 0x2) ) {
        /* Only PTPv2 currently supported */
        return;
    }

    /* Add the details */
    rx_ts[rx_ts_cnt].msg_type = ph->messageId & 0x0f;
    rx_ts[rx_ts_cnt].sequence_id = ntohs(ph->sequenceId);
    memcpy(rx_ts[rx_ts_cnt].clock_identity, ph->clockIdentity,
       sizeof(rx_ts[rx_ts_cnt].clock_identity));
    rx_ts[rx_ts_cnt].sport_id = ntohs(ph->sportId);
    rx_ts[rx_ts_cnt].ts.nsec = bd->ts_n;
    rx_ts[rx_ts_cnt].ts.sec = bd->ts_sl;

    if(rswitch2->cfg.verbose & VERBOSE_PTP) {
        device_printf(rswitch2->dev, "%s msgid=0x%x sequence_id=%u ts=%u.%u", __FUNCTION__,
        rx_ts[rx_ts_cnt].msg_type, rx_ts[rx_ts_cnt].sequence_id, rx_ts[rx_ts_cnt].ts.sec, rx_ts[rx_ts_cnt].ts.nsec);
    }

    /* Advance the counter including wrapping */
    rx_ts_cnt = (rx_ts_cnt + 1) % RSWITCH2_RX_TIMESTAMP_BUF_SZ;
}

void rswitch2_ptp_add_tx_timestamp(rswitch2_dev_t *rswitch2)
{
    uint32_t    idx = 0, queue = 0;
    ptpv2hdr_t  *ph;
    
    while( idx < RSWITCH2_TX_TIMESTAMP_BUF_SZ) {
        ph = &tx_ph[queue][idx];
        if ((ph == NULL) || ((ph->version & 0x0f) != 0x2)) {
            /* Only PTPv2 currently supported */
            return;
        }
        /* Only add timestamp for supported eventmsgs */
        if((ph = &(tx_ph[queue][idx])) && ((ph->version & 0x0f) == 0x2)) {
            /* Add the details */
            ph = &tx_ph[queue][idx];
            tx_ts[tx_ts_cnt].msg_type = ph->messageId & 0x0f;
            tx_ts[tx_ts_cnt].sequence_id = ntohs(ph->sequenceId);
            memcpy(tx_ts[tx_ts_cnt].clock_identity, ph->clockIdentity,
               sizeof(tx_ts[tx_ts_cnt].clock_identity));
            tx_ts[tx_ts_cnt].sport_id = ntohs(ph->sportId);
            
            bus_write_4(rswitch2->res[0], PTPMCCC0, PTPMCCC0_MCCR);
            tx_ts[tx_ts_cnt].ts.nsec = bus_read_4(rswitch2->res[0], PTPGPTPTM00);
            tx_ts[tx_ts_cnt].ts.sec = bus_read_4(rswitch2->res[0], PTPGPTPTM10);

            if(rswitch2->cfg.verbose & VERBOSE_PTP) {
                device_printf(rswitch2->dev, "%s queue=%d idx=%d msgid=0x%x sequence_id=%u ts=%u.%u", __FUNCTION__, 
                    queue, idx, tx_ts[tx_ts_cnt].msg_type, tx_ts[tx_ts_cnt].sequence_id, tx_ts[tx_ts_cnt].ts.sec, tx_ts[tx_ts_cnt].ts.nsec);
            }

            /* Advance the counter including wrapping */
            tx_ts_cnt = (tx_ts_cnt + 1) % RSWITCH2_TX_TIMESTAMP_BUF_SZ;
        }
        idx++;
    }
}

static int rswitch2_ptp_get_rx_timestamp (rswitch2_dev_t *rswitch2, ptp_extts_t *ts)
{
    int i;

    if (ts == NULL) {
        return 0;
    }

    for (i = 0; i < RSWITCH2_RX_TIMESTAMP_BUF_SZ; i++) {
        if ((ts->msg_type == rx_ts[i].msg_type) &&
            (ts->sequence_id == rx_ts[i].sequence_id) &&
            (ts->sport_id == rx_ts[i].sport_id) &&
            !memcmp(ts->clock_identity, rx_ts[i].clock_identity,
            sizeof(ts->clock_identity))) {
            ts->ts.nsec =  rx_ts[i].ts.nsec;
            ts->ts.sec =  rx_ts[i].ts.sec;

            if(rswitch2->cfg.verbose & VERBOSE_PTP) {
                device_printf(rswitch2->dev, "%s msgid=0x%x sequence_id=%u ts=%u.%u", __FUNCTION__,
                ts->msg_type, ts->sequence_id, ts->ts.sec, ts->ts.nsec);
            }

            return 1;
        }
    }
    return 0;
}

static int rswitch2_ptp_get_tx_timestamp (rswitch2_dev_t *rswitch2, ptp_extts_t *ts)
{
    int i;
    
    if (ts == NULL) {
        return 0;
    }

    for (i = 0; i < RSWITCH2_TX_TIMESTAMP_BUF_SZ; i++) {
        if ((ts->msg_type == tx_ts[i].msg_type) &&
            (ts->sequence_id == tx_ts[i].sequence_id) &&
            (ts->sport_id == tx_ts[i].sport_id) &&
            !memcmp(ts->clock_identity, tx_ts[i].clock_identity,
                sizeof(ts->clock_identity))) {
            ts->ts.nsec = tx_ts[i].ts.nsec;
            ts->ts.sec = tx_ts[i].ts.sec;
            if(rswitch2->cfg.verbose & VERBOSE_PTP) {
                device_printf(rswitch2->dev, "%s msgid=0x%x sequence_id=%u ts=%u.%u", __FUNCTION__,
                ts->msg_type, ts->sequence_id, ts->ts.sec, ts->ts.nsec);
            }

            return 1;
        }
    }
    return 0;
}

static void rswitch2_ptp_get_cnt(rswitch2_dev_t *rswitch2, ptp_time_t *cnt)
{

    if(cnt != NULL) {
        /* Request timestamp capture */
        bus_write_4(rswitch2->res[0], PTPMCCC0, PTPMCCC0_MCCR);
        /* Get captured timestamp */
        cnt->nsec = bus_read_4(rswitch2->res[0], PTPGPTPTM00);
        cnt->sec = bus_read_4(rswitch2->res[0], PTPGPTPTM10);

        if(rswitch2->cfg.verbose & VERBOSE_PTP) {
            device_printf(rswitch2->dev, "%s: ts=%u.%u", __FUNCTION__, cnt->sec, cnt->nsec);
        }
    }
}

static void rswitch2_ptp_set_cnt(rswitch2_dev_t *rswitch2, ptp_time_t *cnt)
{
    /* Reset Timer and load the offset */
    bus_write_4(rswitch2->res[0], PTPTMDC, PTPTMDC_TD);
    bus_write_4(rswitch2->res[0], PTPTOVC10, 0);
    bus_write_4(rswitch2->res[0], PTPTOVC00, 0);
    bus_write_4(rswitch2->res[0], PTPTMEC, PTPTMEC_TE);
    bus_write_4(rswitch2->res[0], PTPTOVC10, cnt->sec);
    bus_write_4(rswitch2->res[0], PTPTOVC00, cnt->nsec);

    if(rswitch2->cfg.verbose & VERBOSE_PTP) {
        device_printf(rswitch2->dev, "%s: ts=%u.%u", __FUNCTION__, cnt->sec, cnt->nsec);
    }
}

static void rswitch2_ptp_set_compensation(rswitch2_dev_t *rswitch2, ptp_comp_t ptc)
{
    uint32_t deviation, comp;
    
    if(ptc.comp == 0) {
        /* Reset increment to default */
        comp = PTP_INCR;
        deviation = 0;
    } else {
        comp = PTP_INCR;
        deviation = (uint64_t)comp * ptc.comp / NSEC_PER_SEC;
        if (ptc.positive) {
            comp += deviation;
        } else {
            comp -= deviation;
        }
    }
    mtx_lock(&rswitch2->mtx);
    // if(bus_read_4(rswitch2->res[0], PTPMCCC0)){
    //     mtx_unlock(&rswitch2->mtx);
    //     return;
    // }
    bus_write_4(rswitch2->res[0], PTPTIVC0, comp & TIV_MASK);
    mtx_unlock(&rswitch2->mtx);

    if(rswitch2->cfg.verbose & VERBOSE_PTP) {
        device_printf(rswitch2->dev, "%s ppb=%u dir=%u deviation=%u current_gti=%u", __FUNCTION__,
            ptc.comp, ptc.positive, deviation, comp);
    }
}

int rswitch2_ptp_ioctl(rswitch2_dev_t *rswitch2, struct ifdrv *ifd)
{
    ptp_time_t      time;
    ptp_comp_t      comp;
    ptp_extts_t     ts;
    uint8_t         found;

    if (ifd != NULL) {
        switch(ifd->ifd_cmd) {

            case PTP_GET_RX_TIMESTAMP:
            case PTP_GET_TX_TIMESTAMP:
                if (ifd->ifd_len != sizeof(ts)) {
                    return EINVAL;
                }

                // if (ISSTACK) {
                //     if (copyin((((uint8_t *)ifd) + sizeof(*ifd)),
                //            &ts, sizeof(ts))) {
                //         return EINVAL;
                //     }
                // } else {
                    memcpy(&ts, (((uint8_t *)ifd) + sizeof(*ifd)), sizeof(ts));
                //}

                if (ifd->ifd_cmd == PTP_GET_RX_TIMESTAMP) {
                    found = rswitch2_ptp_get_rx_timestamp(rswitch2, &ts);
                } else {
                    found = rswitch2_ptp_get_tx_timestamp(rswitch2, &ts);
                }

                if (found) {
                    // if (ISSTACK) {
                    //     return (copyout(&ts, (((uint8_t *)ifd) + sizeof(*ifd)),
                    //             sizeof(ts)));
                    // } else {
                        memcpy((((uint8_t *)ifd) + sizeof(*ifd)), &ts, sizeof(ts));
                        return EOK;
                    //}
                }
                return ENOENT;
                break;

        case PTP_GET_TIME:
            if (ifd->ifd_len != sizeof(time)) {
                return EINVAL;
            }
            mtx_lock(&rswitch2->mtx);
            rswitch2_ptp_get_cnt(rswitch2, &time);
            mtx_unlock(&rswitch2->mtx);
            // if (ISSTACK) {
            //     return (copyout(&time, (((uint8_t *)ifd) + sizeof(*ifd)),
            //             sizeof(time)));
            // } else {
                memcpy((((uint8_t *)ifd) + sizeof(*ifd)), &time, sizeof(time));
                return EOK;
            //}
            break;

        case PTP_SET_TIME:
            if (ifd->ifd_len != sizeof(time)) {
                return EINVAL;
            }
            // if (ISSTACK) {
            //     if (copyin((((uint8_t *)ifd) + sizeof(*ifd)),
            //            &time, sizeof(time))) {
            //         return EINVAL;
            //     }
            // } else {
                memcpy(&time, (((uint8_t *)ifd) + sizeof(*ifd)), sizeof(time));
            //}
            mtx_lock(&rswitch2->mtx);
            rswitch2_ptp_set_cnt(rswitch2, &time);
            mtx_unlock(&rswitch2->mtx);
            /* Clock has changed so all old ts are invalid */
            memset(rx_ts, 0, sizeof(rx_ts));
            memset(tx_ts, 0, sizeof(tx_ts));
            return EOK;
            break;

        case PTP_SET_COMPENSATION:
            if (ifd->ifd_len != sizeof(comp)) {
            return EINVAL;
            }
            // if (ISSTACK) {
            //     if (copyin((((uint8_t *)ifd) + sizeof(*ifd)),
            //            &comp, sizeof(comp))) {
            //         return EINVAL;
            //     }
            // } else {
                memcpy(&comp, (((uint8_t *)ifd) + sizeof(*ifd)), sizeof(comp));
            //}
            rswitch2_ptp_set_compensation(rswitch2, comp);
            return EOK;
            break;

        case PTP_GET_COMPENSATION:
            return ENOTTY;

        default:
            break;
        }
    }
    return ENOTTY;
}
