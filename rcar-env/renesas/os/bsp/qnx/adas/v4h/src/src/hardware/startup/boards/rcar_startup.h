/*
* $QNXLicenseC:
* Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
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

#ifndef __RCAR_STARTUP_H_INCLUDED
#define __RCAR_STARTUP_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <hw/rcar_cpg_support.h>
#include <hw/rcar_sysc_support.h>
#include <aarch64/r-car.h>

//#define PFC_DEBUG

#ifdef PFC_DEBUG
 #define RCAR_DEBUG_PRINT(format, ...) kprintf(format, ##__VA_ARGS__)
#else
 #define RCAR_DEBUG_PRINT(format, ...)
#endif

#define CLUSTERS_NUM_MAX    4
#define MPIDR_CLUSTER_ID(x) (((x) >> 8) & 0xFF)
#define MIDR_CPU_ID(x)      (((x) >> 4) & 0xFFF)
#define MIDR_CPU_A53_ID     0xD03
#define MIDR_CPU_A55_ID     0xD05
#define MIDR_CPU_A57_ID     0xD07
#define MIDR_CPU_A76_ID     0xD0B

typedef enum {
    RCAR_H3,
    RCAR_M3W,
    RCAR_M3N,
    RCAR_V3M,
    RCAR_V3H,
    RCAR_V3U,
    RCAR_D3,
    RCAR_E3,
    RCAR_S4,
    RCAR_V4H,
    ID_NUM
} product_id_t;

typedef enum {
    REV_1_0,
    REV_1_1,
    REV_1_2,
    REV_1_3,
    REV_2_0,
    REV_2_1,
    REV_2_2,
    REV_3_0,
    REV_NUM
} product_rev_t;

extern product_id_t  product_id;
extern product_rev_t product_rev;
extern uint32_t      rcar_product_id;
extern uint8_t board_type;
extern uint8_t board_rev;
extern unsigned board_ext_clk;
struct rcar_reserved_ram {
    struct rcar_reserved_ram    *next;
    char                        *name;
    paddr_t                     size;
    paddr_t                     addr;
};

typedef struct {
    uint32_t base;
    unsigned irq;
} dev_info_t;

typedef struct {
    uint32_t pwr_vdd_base;      /* GPIO Module Base */
    uint32_t pwr_vdd_size;      /* GPIO Module Size */
    uint32_t pwr_vdd_offset;    /* Vdd GPIO Offset */
    uint16_t pwr_vdd_shift;     /* Vdd pin */
    uint32_t pwr_if_base;       /* GPIO Module Base */
    uint32_t pwr_if_size;       /* GPIO Module Size */
    uint32_t pwr_if_offset;     /* If GPIO Offset */
    uint16_t pwr_if_shift;      /* Vddqva pin */
    uint32_t pfc_base;          /* PFC Module Base */
    uint16_t pfc_offset;        /* POCCTRL Offset */
    uint16_t pfc_mask;          /* Pin mask */
    uint16_t pfc_shift;         /* Mask shift */
} sd_info_t;

typedef struct {
    uint32_t dma_dreq_txi;
    uint32_t dma_dreq_rxi;
    uint32_t mstp_id;
} i2c_info_t;

#define BITMASK(x,y)     (((2ULL << (y-x)) - 1) << x)
#ifndef BIT
#define BIT(n)           (1 << n)
#endif

typedef struct {
    char        *name;
	uint32_t	adr;
    uint32_t	wpa;
    uint32_t	msk;
	uint32_t	val;
} scif_debug_config_t;

extern struct rcar_reserved_ram *rcar_reserved_ram_list;
void init_board_clock();
uint32_t cpu_freq_get(uint32_t midr_cpu_id);
void cpu_freq_set(uint32_t cpufreq);
void power_domain_on(uint8_t domain, uint8_t irq_bit, uint8_t pwroncr_bit);

uint32_t aarch64_midr(void);
uint32_t aarch64_mpidr(void);

void usec_delay(uint32_t usec);

void init_timer(void);

int wdt_enable(void);

void init_product_id(void);
char *get_soc_name(void);
const char *get_board_name(void);
bool inter_core_intr_errata(void);

void init_board(void);

void init_dma(void);
void init_debug_scif(char *s);

#endif //__RCAR_STARTUP_H_INCLUDED
