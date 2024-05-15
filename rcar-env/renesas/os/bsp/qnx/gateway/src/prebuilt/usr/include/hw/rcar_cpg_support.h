/*
 * $QNXLicenseC:
 * Copyright 2020 QNX Software Systems.
 * Copyright 2020 Renesas Electronics Corporation.
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

#ifndef _RCAR_CPG_SUPPORT_H_INCLUDED
#define _RCAR_CPG_SUPPORT_H_INCLUDED

#include <hw/rcar_cpg.h>

typedef enum pmic_model_ {
    PMIC_MODEL_UNKNOWN = 0,
    PMIC_MODEL_BOHM_BD9571MWF,
} pmic_model_t;

typedef struct cpg_opts_ {
    uint32_t     base;
    uint32_t     size;
    uint32_t     ext_clk;
    int          verbose;
} cpg_opts_t;

typedef struct pmic_opts_ {
    pmic_model_t model;
    int          i2c_bus;
    int          i2c_slave_addr;
} pmic_opts_t;

typedef struct avs_opts_ {
    uint32_t     base;
    uint32_t     size;
    pmic_opts_t  pmic;
} avs_opts_t;

/*
 * Hardware Access Function Table
 */
typedef struct {
    int   (*init)(const cpg_opts_t* const cpg_opts, const avs_opts_t* const avs_opts);
    void  (*deinit)(void);

    int (*mstp_config)(const int mod_id, const int enable);
    int (*mstp_status)(const int mod_id, int *status);
    int (*mstp_reset)(const int mod_id);
    int (*mstp_reset_config)(const int mod_id, const int assert);
    int (*mstp_reset_state)(const int mod_id, int *status);

    int (*clk_config)(const cpg_clk_id_t clk_id, const int idx, const int enable);
    int (*clk_rate)(const cpg_clk_id_t clk_id, const int idx, unsigned *rate, const int set);
    int (*clk_divider)(const cpg_clk_id_t clk_id, const int idx, unsigned *clk_div, const int set);
    int (*clk_status)(const cpg_clk_id_t clk_id, const int idx, int *status);
} cpg_support_funcs_t;

extern cpg_support_funcs_t cpg_support_funcs;

#endif /* _RCAR_CPG_SUPPORT_H_INCLUDED */
