/*
* $QNXLicenseC:
* Copyright (c) 2017, QNX Software Systems. All Rights Reserved.
* Copyright 2021, Renesas Electronics Corporation.
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

#include <hw/rcar_cpg_support.h>
#include <hw/rcar_sysc_support.h>

//#define PFC_DEBUG

#ifdef PFC_DEBUG
 #define PFC_DEBUG_PRINT(format, ...) kprintf(format, ##__VA_ARGS__)
#else
 #define PFC_DEBUG_PRINT(format, ...)
#endif

#define CLUSTERS_NUM_MAX    4
#define MPIDR_CLUSTER_ID(x) (((x) >> 8) & 0xFF)
#define MIDR_CPU_ID(x)      (((x) >> 4) & 0xFFF)
#define MIDR_CPU_A53_ID     0xD03
#define MIDR_CPU_A57_ID     0xD07
#define MIDR_CPU_A76_ID     0xD0B

#define BD9571_IIC_ADDR     (0x30)
#define AVS_ADVADJP         (0xE60A0000 + 0x013C)

typedef enum {
    RCAR_H3,
    RCAR_M3W,
    RCAR_M3N,
    RCAR_V3M,
    RCAR_V3H,
    RCAR_V3U,
    RCAR_D3,
    RCAR_E3,
    ID_NUM
} product_id_t;

typedef enum {
    REV_1_0,
    REV_1_1,
    REV_1_3,
    REV_2_0,
    REV_2_1,
    REV_3_0,
    REV_NUM
} product_rev_t;

extern product_id_t  product_id;
extern product_rev_t product_rev;
extern uint32_t      rcar_soc_id;

extern uint8_t board_type;
extern uint8_t board_rev;
extern unsigned board_ext_clk;
struct rcar_reserved_ram {
    struct rcar_reserved_ram    *next;
    char                        *name;
    paddr_t                     size;
    paddr_t                     addr;
};

extern struct sysc_factory sysc_support;
extern struct clk_factory clk_support;
extern struct rcar_reserved_ram *rcar_reserved_ram_list;
uint32_t cpu_freq_get(uint32_t midr_cpu_id);
void cpu_freq_set(uint32_t cpufreq);
void init_board_clock();
void power_domain_on(uint8_t domain, uint8_t irq_bit, uint8_t pwroncr_bit);

int i2c_read_reg(uint8_t bus_no, uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);
int iic_read_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);
int iic_write_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);


uint32_t aarch64_midr(void);
uint32_t aarch64_mpidr(void);

void usec_delay(uint32_t usec);

void init_timer(void);

int wdt_enable(void);

void init_product_id(void);
char *get_soc_name(void);
const char *get_board_name(void);

void init_board(void);
void rcar_sec_vec(void);

#endif //__RCAR_STARTUP_H_INCLUDED
