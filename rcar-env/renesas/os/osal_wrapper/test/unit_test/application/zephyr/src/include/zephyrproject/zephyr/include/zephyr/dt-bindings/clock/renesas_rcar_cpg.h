/*
 * Copyright (c) 2021 IoT.bzh
 * Copyright (c) 2022 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_RENESAS_RCAR_CPG_MSSR_H_
#define ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_RENESAS_RCAR_CPG_MSSR_H_

#define CPG_CORE                        0UL       /* Core Clock */
#define CPG_MOD                         1UL       /* Module Clock */
#define CPG_RESET                       2UL       /* Software Reset */
#define CPG_DIVISION_RATIO              3UL       /* Division Ratio */

#ifdef CONFIG_SOC_SERIES_RCAR_GEN4
/**
 * @enum    r_cpg_ClockName_t
 * @brief   List of clock name
 */
typedef enum {
	R_CPG_ZS_CLOCK,                 /* ZS Clock     */
	R_CPG_ZT_CLOCK,                 /* ZT Clock     */
	R_CPG_ZTR_CLOCK,                /* ZTR Clock    */
	R_CPG_Z0_CLOCK,                 /* Z0 Clock     */
	R_CPG_ZB3_ZB3D2_ZB3D4_CLOCK,    /* ZB3,ZB3D2,ZB3D4 Clock    */
	R_CPG_ZG_CLOCK,                 /* ZG Clock     */
	R_CPG_SDSRC_CLOCK,              /* SDSRC Clock  */
	R_CPG_SD0H_CLOCK,               /* SD0H Clock   */
	R_CPG_SD0_CLOCK,                /* SD0 Clock    */
	R_CPG_RPC_CLOCK,                /* RPC Clock    */
	R_CPG_RPCD2_CLOCK,              /* RPCD2 Clock  */
	R_CPG_MSO_CLOCK,                /* MSO Clock    */
	R_CPG_CANFD_CLOCK,              /* CANFD Clock  */
	R_CPG_CSI_CLOCK,                /* CSI Clock    */
	R_CPG_POST2_CLOCK,              /* POST2 Clock  */
	R_CPG_POST3_CLOCK,              /* POST3 Clock  */
	R_CPG_POST4_CLOCK,              /* POST4 Clock  */
	R_CPG_POST_CLOCK,               /* POST Clock   */
	R_CPG_DSIEXT_CLOCK,             /* DSIEXT Clock */
	R_CPG_ZR0_CLOCK,                /* ZR0 Clock    */
	R_CPG_ZR1_CLOCK,                /* ZR1 Clock    */
	R_CPG_ZR2_CLOCK,                /* ZR2 Clock    */
	R_CPG_CLOCK_LAST
} r_cpg_ClockName_t;

#define CPG_CORE_CLK_S0D12              2UL       /* S0D12 Clock */

#else /* CONFIG_SOC_SERIES_RCAR_GEN3 */
#define CPG_CORE_CLK_CANFD              0UL       /* CANFD clock */
#define CPG_CORE_CLK_S3D4               1UL       /* S3D4 Clock */
#define CPG_CORE_CLK_S0D12              2UL       /* S0D12 Clock */
#endif

#endif /* ZEPHYR_INCLUDE_DT_BINDINGS_CLOCK_RENESAS_RCAR_CPG_MSSR_H_ */
