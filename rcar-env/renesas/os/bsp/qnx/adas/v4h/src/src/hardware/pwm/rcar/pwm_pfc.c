/*
 * $QNXLicenseC:
 * Copyright 2022, QNX Software Systems.
 * Copyright 2022, Renesas Electronics Corporation.
 *
 * You must obtain a written license from and pay applicable license fees to QNX
 * Software Systems before you may reproduce, modify or distribute this software,
 * or any work that includes all or part of this software. Free development
 * licenses are available for evaluation and non-commercial purposes. For more
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *
 * This file may contain contributions from others. Please review this entire
 * file for other proprietary rights or license notices, as well as the QNX
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/
 * for other information.
 * $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <aarch64/r-car.h>

#include "pwm_priv.h"
#include "pwm_log.h"

extern pwm_device_t *g_device;

static inline uint32_t pfc_read(uintptr_t vbase, uint32_t reg)
{
    return in32(vbase + reg);
}

static inline void pfc_write(uintptr_t vbase, uint32_t reg, uint32_t val)
{
    out32(vbase + RCAR_GEN4_GPIO_PMMR, ~val);
    out32(vbase + reg, val);
}

/*
 *  Chan  |  Pin Name   |  GPIO    |  IPSR
 *  ====================================================
 *  PWM0  |  HSCK0      |  GP1_15  |  IP1SR1[31:28] = H'2
 *  PWM1  |  CANFD2_RX  |  GP2_13  |  IP1SR2[23:20] = H'2
 *  PWM2  |  CANFD3_TX  |  GP2_14  |  IP1SR2[27:24] = H'2
 *  PWM3  |  CANFD3_RX  |  GP2_15  |  IP1SR2[31:28] = H'2
 *  PWM4  |  CANFD4_TX  |  GP2_16  |  IP2SR2[3:0]   = H'2
 *  PWM5  |  CANFD4_RX  |  GP2_17  |  IP2SR2[7:4]   = H'2
 *  PWM6  |  CANFD7_TX  |  GP2_18  |  IP2SR2[11:8]  = H'2
 *  PWM7  |  CANFD7_RX  |  GP2_19  |  IP2SR2[15:12] = H'2
 *  PWM8  |  HCTS0#     |  GP1_13  |  IP1SR1[23:20] = H'2
 *  PWM9  |  HRTS0#     |  GP1_14  |  IP1SR1[27:24] = H'2
 */
void pwm_pfc_config(void)
{
    uintptr_t vgp1, vgp2;
    uint32_t gpsr1, gpsr2, ip1sr1, ip1sr2, ip2sr2;

    if ((vgp1 = mmap_device_io(RCAR_GEN4_GPIO_GP_SIZE, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP1)) == MAP_DEVICE_FAILED) {
        fprintf(stderr, "Unable to map device memory (%s)\n", strerror(errno));
        abort();
    }

    if ((vgp2 = mmap_device_io(RCAR_GEN4_GPIO_GP_SIZE, RCAR_GEN4_GPIO_DM0_BASE + RCAR_V4H_GPIO_GP2)) == MAP_DEVICE_FAILED) {
        fprintf(stderr, "Unable to map device memory (%s)\n", strerror(errno));
        abort();
    }

    gpsr1 = pfc_read(vgp1, RCAR_GEN4_GPIO_GPSR);
    gpsr2 = pfc_read(vgp2, RCAR_GEN4_GPIO_GPSR);

    ip1sr1 = pfc_read(vgp1, RCAR_GEN4_GPIO_IP1SR);
    ip1sr2 = pfc_read(vgp2, RCAR_GEN4_GPIO_IP1SR);
    ip2sr2 = pfc_read(vgp2, RCAR_GEN4_GPIO_IP2SR);
 
    for (int i = 0; i < g_device->n_chan; i++) {
        if (!g_device->chans[i].active) {
            continue;
        }
        switch(i) {
            case 0:
                ip1sr1 &= ~(0xf << 28);
                ip1sr1 |=  (0x2 << 28);
                gpsr1  |=  (0x1 << 15);
                break;
            case 1:
                ip1sr2 &= ~(0xf << 20);
                ip1sr2 |=  (0x2 << 20);
                gpsr2  |=  (0x1 << 13);
                break;
            case 2:
                ip1sr2 &= ~(0xf << 24);
                ip1sr2 |=  (0x2 << 24);
                gpsr2  |=  (0x1 << 14);
                break;
            case 3:
                ip1sr2 &= ~(0xf << 28);
                ip1sr2 |=  (0x2 << 28);
                gpsr2  |=  (0x1 << 15);
                break;
            case 4:
                ip2sr2 &= ~(0xf << 0);
                ip2sr2 |=  (0x2 << 0);
                gpsr2  |=  (0x1 << 16);
                break;
            case 5:
                ip2sr2 &= ~(0xf << 4);
                ip2sr2 |=  (0x2 << 4);
                gpsr2  |=  (0x1 << 17);
                break;
            case 6:
                ip2sr2 &= ~(0xf << 8);
                ip2sr2 |=  (0x2 << 8);
                gpsr2  |=  (0x1 << 18);
                break;
            case 7:
                ip2sr2 &= ~(0xf << 12);
                ip2sr2 |=  (0x2 << 12);
                gpsr2  |=  (0x1 << 19);
                break;
            case 8:
                ip1sr1 &= ~(0xf << 20);
                ip1sr1 |=  (0x2 << 20);
                gpsr1  |=  (0x1 << 13);
                break;
            case 9:
                ip1sr1 &= ~(0xf << 24);
                ip1sr1 |=  (0x2 << 24);
                gpsr1  |=  (0x1 << 14);
            default:
                break;
        }
    }

    pfc_write(vgp1, RCAR_GEN4_GPIO_IP1SR, ip1sr1);
    pfc_write(vgp1, RCAR_GEN4_GPIO_GPSR,  gpsr1);

    pfc_write(vgp2, RCAR_GEN4_GPIO_IP1SR, ip1sr2);
    pfc_write(vgp2, RCAR_GEN4_GPIO_IP2SR, ip2sr2);
    pfc_write(vgp2, RCAR_GEN4_GPIO_GPSR,  gpsr2);

    pwm_slogf(_SLOG_DEBUG1, "Pinconf: gpsr1=0x%08x, ip1sr1=0x%08x", gpsr1, ip1sr1);
    pwm_slogf(_SLOG_DEBUG1, "Pinconf: gpsr2=0x%08x, ip1sr2=0x%08x, ip2sr2=0x%08x", gpsr2, ip1sr2, ip2sr2);

    munmap_device_io(vgp1, RCAR_GEN4_GPIO_GP_SIZE);
    munmap_device_io(vgp2, RCAR_GEN4_GPIO_GP_SIZE);
}
