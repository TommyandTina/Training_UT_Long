/** \addtogroup soc_abstraction 
 *  @{
 */

#include "target/simulation/soc_abstraction/dev_cfg.hpp"
#include "rcar-xos/utils/r_utils.hpp"

using namespace renesas::rcarxos::utils::literals;
#if defined(RCAR_V3M2)
#define IMP_SCRATCH_PAD_SIZE (7 * 128_KB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED000000U)
#define SYSTEM_RAM_SIZE (448_KB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V3H1) || defined(RCAR_V3H2)
#define IMP_SCRATCH_PAD_SIZE (16 * 128_KB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED000000U)
#define SYSTEM_RAM_SIZE (384_KB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V3U)
#define IMP_SCRATCH_PAD_SIZE (0xEDB00000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4H2)
#define IMP_SCRATCH_PAD_SIZE (0xED680000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4H)
#define IMP_SCRATCH_PAD_SIZE (0xED600000U - 0xED300000U)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED300000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#elif defined(RCAR_V4M)
#define IMP_SCRATCH_PAD_SIZE (0xED680000U - 0xED200000U) // 4.5MB: imp_spmi_00 (2MB) + imp_spmc_00 (2MB) + imp_spmc_01 (0.5MB)
#define IMP_SCRATCH_PAD_BASE_ADDR (0xED200000U)
#define SYSTEM_RAM_SIZE (1_MB)
#define SYSTEM_RAM_BASE_ADDR (0xE6300000U)
#else
#error "unknown SoC. Only V3M2, V3H1 V3H2 and V3U are supported. Please set RCAR_V3M2, RCAR_V3H1, RCAR_V3H2, RCAR_V3U or RCAR_V4H defintions."
#endif

/** @todo identify device by name, type, range and number of irq channels only. Architecture must be changed! */
/** @todo get rid of this kind of device configuration. We need a solution that better fits the OSAL API concept, e.g.
  * device based approach instead of Memory Location approach 
**/

const dev_cfg::DeviceArray dev_cfg::m_devices = {
// OSAL_DEVICE("device name", "device type", "axi bus domain",base,range, irq_channels)
    // System RAM
    Osal_Device("system_ram_00", "system_ram", "invalid", {UTLB_ID_INVALID}, SYSTEM_RAM_BASE_ADDR, SYSTEM_RAM_SIZE),

#if defined(RCAR_V3M2)
    // IMP_TOP
    Osal_Device("imp_top_00", "imp_top", "ir", {0, 1}, 0xFFA00000, 0x00010000, {281, 282, 283, 284, 285}),

    // IMP
    Osal_Device("imp_00", "imp", "ir", {0, 1}, 0xFF900000, 0x00020000),
    Osal_Device("imp_01", "imp", "ir", {0, 1}, 0xFF920000, 0x00020000),
    Osal_Device("imp_02", "imp", "ir", {0, 1}, 0xFF940000, 0x00020000),
    Osal_Device("imp_03", "imp", "ir", {0, 1}, 0xFF960000, 0x00020000),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {0, 1}, 0xFF980000, 0x00010000),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {0, 1}, 0xFF990000, 0x00010000),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {0, 1}, 0xFFA10000, 0x00001000),

    // IMP_PSC
    Osal_Device("imp_psc_00", "imp_psc", "ir", {0, 1}, 0xFFA20000, 0x00004000),

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {0, 1}, 0xFF9E0000, 0x00010000),

    // IMPC
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {0, 1}, IMP_SCRATCH_PAD_BASE_ADDR, IMP_SCRATCH_PAD_SIZE),

    // IMR
    Osal_Device("ims_00", "ims", "vi0", {2}, 0xFE860000U, 0x00002000U, {192}),
    Osal_Device("ims_01", "ims", "vi0", {2}, 0xFE870000U, 0x00002000U, {193}),
    Osal_Device("ims_02", "ims", "vi0", {3}, 0xFE880000U, 0x00002000U, {194}),
    Osal_Device("ims_03", "ims", "vi0", {3}, 0xFE890000U, 0x00002000U, {195}),

#elif defined(RCAR_V3H1)
    Osal_Device("imp_top_00", "imp_top", "ir", {0, 1}, 0xFFA00000, 0x00010000, {281, 282, 283, 284, 285}),

    // IMP
    Osal_Device("imp_00", "imp", "ir", {0, 1}, 0xFF900000, 0x00020000),
    Osal_Device("imp_01", "imp", "ir", {0, 1}, 0xFF920000, 0x00020000),
    Osal_Device("imp_02", "imp", "ir", {0, 1}, 0xFF940000, 0x00020000),
    Osal_Device("imp_03", "imp", "ir", {0, 1}, 0xFF960000, 0x00020000),
    Osal_Device("imp_04", "imp", "ir", {0, 1}, 0xFFA80000, 0x00020000),

    // IMP_SLIM
    Osal_Device("imp_slim_00", "imp_slim", "ir", {0, 1}, 0xFF9C0000, 0x00010000),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {0, 1}, 0xFF980000, 0x00010000),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {0, 1}, 0xFF990000, 0x00010000),
    Osal_Device("imp_ocv_02", "imp_ocv", "ir", {0, 1}, 0xFF9A0000, 0x00010000),
    Osal_Device("imp_ocv_03", "imp_ocv", "ir", {0, 1}, 0xFF9B0000, 0x00010000),
    Osal_Device("imp_ocv_04", "imp_ocv", "ir", {0, 1}, 0xFFA40000, 0x00010000),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {0, 1}, 0xFFA10000, 0x00001000),
    Osal_Device("imp_dma_01", "imp_dma", "ir", {0, 1}, 0xFFA14000, 0x00001000),

    // IMP_PSC
    Osal_Device("imp_psc_00", "imp_psc", "ir", {0, 1}, 0xFFA20000, 0x00004000),
    Osal_Device("imp_psc_01", "imp_psc", "ir", {0, 1}, 0xFFA24000, 0x00004000),

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {0, 1}, 0xFF9E0000, 0x00010000),

    // IMPC
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {0, 1}, IMP_SCRATCH_PAD_BASE_ADDR, IMP_SCRATCH_PAD_SIZE),

    // IMR
    Osal_Device("ims_00", "ims", "vc0", {4}, 0xFE860000U, 0x00002000U, {192}),
    Osal_Device("ims_01", "ims", "vc0", {5}, 0xFE870000U, 0x00002000U, {193}),
    Osal_Device("ims_02", "ims", "vc0", {6}, 0xFE880000U, 0x00002000U, {194}),
    Osal_Device("ims_03", "ims", "vc0", {7}, 0xFE890000U, 0x00002000U, {195}),
    Osal_Device("imr_00", "imr", "vc0", {0, 1}, 0xFE8A0000U, 0x00002000U, {254}),
    Osal_Device("imr_01", "imr", "vc0", {2, 3}, 0xFE8B0000U, 0x00002000U, {255}),

#elif defined(RCAR_V3H2)
    Osal_Device("imp_top_00", "imp_top", "ir", {0, 1}, 0xFFA00000, 64_KB, {281, 282, 283, 284, 285}),

    // IMP
    Osal_Device("imp_00", "imp", "ir", {0, 1}, 0xFF900000, 128_KB),
    Osal_Device("imp_01", "imp", "ir", {0, 1}, 0xFF920000, 128_KB),
    Osal_Device("imp_02", "imp", "ir", {0, 1}, 0xFF940000, 128_KB),
    Osal_Device("imp_03", "imp", "ir", {0, 1}, 0xFF960000, 128_KB),
    Osal_Device("imp_04", "imp", "ir", {0, 1}, 0xFFA80000, 128_KB),

    // IMP_SLIM
    Osal_Device("imp_slim_00", "imp_slim", "ir", {0, 1}, 0xFF9C0000, 64_KB),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {0, 1}, 0xFF980000, 64_KB),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {0, 1}, 0xFF990000, 64_KB),
    Osal_Device("imp_ocv_02", "imp_ocv", "ir", {0, 1}, 0xFF9A0000, 64_KB),
    Osal_Device("imp_ocv_03", "imp_ocv", "ir", {0, 1}, 0xFF9B0000, 64_KB),
    Osal_Device("imp_ocv_04", "imp_ocv", "ir", {0, 1}, 0xFFA40000, 64_KB),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {0, 1}, 0xFFA10000, 16_KB),
    Osal_Device("imp_dma_01", "imp_dma", "ir", {0, 1}, 0xFFA14000, 16_KB),

    //IMP_SLIM_DMA
    Osal_Device("imp_dma_slim_00", "imp_dma_slim", "ir", {0 , 1}, 0xFFA18000, 16_KB),

    // IMP_PSC
    //Osal_Device("imp_psc_00", "imp_psc", "ir", {0, 1}, 0xFFA24000, 16_KB),    //old HWM
    /*check with HW manual (changed in HWM newest version) PSC HW manual R-CarV3H_2_050_U30B_IMP_PSC_0001_C.docx*/
    Osal_Device("imp_psc_00", "imp_psc", "ir", {0, 1}, 0xFFA20000, 16_KB),      //new HWM??

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {0, 1}, 0xFF9E0000, 64_KB),

    // IMPC 
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {0, 1}, IMP_SCRATCH_PAD_BASE_ADDR, IMP_SCRATCH_PAD_SIZE),

    // IMR
    Osal_Device("ims_00", "ims", "vc0", {4}, 0xFE860000U, 0x00002000U, {192}),
    Osal_Device("ims_01", "ims", "vc0", {5}, 0xFE870000U, 0x00002000U, {193}),
    Osal_Device("ims_02", "ims", "vc0", {6}, 0xFE880000U, 0x00002000U, {194}),
    Osal_Device("imr_00", "imr", "vc0", {0, 1}, 0xFE8A0000U, 0x00002000U, {254}),
    Osal_Device("imr_01", "imr", "vc0", {2, 3}, 0xFE8B0000U, 0x00002000U, {255}),

    //ISP
    Osal_Device("cisp_00", "cisp", "vi0", {11}, 0xFEC00000U, 128_KB, {26}),
    Osal_Device("tisp_00", "tisp", "vi0", {10}, 0xFED00000U, 64_KB , {25}),

    Osal_Device("tisp_01", "tisp", "vi0", {13}, 0xFED20000U, 64_KB , {27}),
    Osal_Device("cisp_01", "cisp", "vi0", {14}, 0xFEE00000U, 128_KB, {109}),

    // DOF
    Osal_Device("dof_00", "dof", "vip0", {1}, 0xE7A10000U, 64_KB , {242, 243}),

    // STV/ACF
    Osal_Device("stv_00", "stv", "vip0", {0} , 0xE7A00000U, 64_KB, {238, 239}),
    Osal_Device("acf_00", "acf", "vip1", {0} , 0xE7A50000U, 64_KB, {71, 97}),
    Osal_Device("acf_01", "acf", "vip1", {1} , 0xE7A60000U, 64_KB, {98, 99}),
    Osal_Device("acf_02", "acf", "vip1", {2} , 0xE7A70000U, 64_KB, {100, 101}),
    Osal_Device("acf_03", "acf", "vip1", {3} , 0xE7A80000U, 64_KB, {102, 103}),
    Osal_Device("acf_04", "acf", "vip1", {4} , 0xE7A90000U, 64_KB, {104, 108}),

#elif defined(RCAR_V3U)
    Osal_Device("imp_top_00", "imp_top", "ir", {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
                0xFF900000, 16_KB, {576, 577, 578, 579, 580, 581, 582}),
    //Osal_Device("imp_top_00", "imp_top", "ir", {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
    //          0xFF900000, 16_KB, {576, 577, 578, 580, 581, 582, 579}),


    // IMP
    Osal_Device("imp_00", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA00000, 128_KB),
    Osal_Device("imp_01", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA20000, 128_KB),
    Osal_Device("imp_02", "imp", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB00000, 128_KB),
    Osal_Device("imp_03", "imp", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB20000, 128_KB),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA40000, 64_KB),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA50000, 64_KB),
    Osal_Device("imp_ocv_02", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB40000, 64_KB),
    Osal_Device("imp_ocv_03", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB50000, 64_KB),
    Osal_Device("imp_ocv_04", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA60000, 64_KB),
    Osal_Device("imp_ocv_05", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB60000, 64_KB),
    Osal_Device("imp_ocv_06", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA70000, 64_KB),
    Osal_Device("imp_ocv_07", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB70000, 64_KB),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA80000U, 16_KB),
    Osal_Device("imp_dma_01", "imp_dma", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB80000U, 16_KB),

    // IMP_PSC
    Osal_Device("imp_psc_00", "imp_psc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA84000U, 16_KB),
    Osal_Device("imp_psc_01", "imp_psc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB84000U, 16_KB),

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFAA0000U, 64_KB),
    Osal_Device("imp_cnn_01", "imp_cnn", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFBC0000U, 64_KB),
    Osal_Device("imp_cnn_02", "imp_cnn", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFAC0000U, 64_KB),

    // IMPC aka SPMI/SPMC
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xED300000, 1_MB),
    Osal_Device("imp_spmi_01", "imp_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xEDA00000, 1_MB),
    Osal_Device("imp_spmc_00", "imp_spmc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xED600000, 2_MB),
    Osal_Device("imp_spmc_01", "imp_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED800000, 2_MB),
    Osal_Device("imp_spmc_02", "imp_spmc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xED400000, 2_MB),

    // IMP Regs
    Osal_Device("imp_reg_spmi_00", "imp_reg_spmi", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA8C000, 16_KB),
    Osal_Device("imp_reg_spmi_01", "imp_reg_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB8C000, 16_KB),
    Osal_Device("imp_reg_spmc_00", "imp_reg_spmc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFAB0000, 16_KB),
    Osal_Device("imp_reg_spmc_01", "imp_reg_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFBD0000, 16_KB),
    Osal_Device("imp_reg_spmc_02", "imp_reg_spmc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFAD0000, 16_KB),

    // IMP LDMAC
    Osal_Device("imp_ldmac_master_00", "imp_ldmac_master",   "ir", {2}, 0xF1F00000, 4_KB, {584}),
    Osal_Device("imp_ldmac_checker_00", "imp_ldmac_checker", "ir", {2}, 0xF1F10000, 4_KB),

    // IMR
    Osal_Device("ims_00", "ims", "vc0", {8},    0xFE860000U, 64_KB, {150}),
    Osal_Device("ims_01", "ims", "vc0", {9},    0xFE870000U, 64_KB, {151}),
    Osal_Device("imr_00", "imr", "vc0", {0, 4}, 0xFE880000U, 64_KB, {146}),
    Osal_Device("imr_01", "imr", "vc0", {1, 5}, 0xFE890000U, 64_KB, {147}),
    Osal_Device("imr_02", "imr", "vc0", {2, 6}, 0xFE8A0000U, 64_KB, {148}),
    Osal_Device("imr_03", "imr", "vc0", {3, 7}, 0xFE8B0000U, 64_KB, {149}),

    // ISP
    Osal_Device("cisp_00", "cisp", "vi0", {14}, 0xFEC00000, 128_KB, {153}),
    Osal_Device("tisp_00", "tisp", "vi0", {10}, 0xFED00000, 64_KB , {152}),

    Osal_Device("cisp_01", "cisp", "vi0", {15}, 0xFEE00000, 128_KB, {155}),
    Osal_Device("tisp_01", "tisp", "vi0", {11}, 0xFED20000, 64_KB , {154}),

    Osal_Device("cisp_02", "cisp", "vi0", {16}, 0xFEF00000, 128_KB, {157}),
    Osal_Device("tisp_02", "tisp", "vi0", {12}, 0xFED30000, 64_KB , {156}),

    Osal_Device("cisp_03", "cisp", "vi0", {17}, 0xFE400000, 128_KB, {159}),
    Osal_Device("tisp_03", "tisp", "vi0", {13}, 0xFED40000, 64_KB , {158}),

    // DOF
    Osal_Device("dof_00", "dof", "vip0", {3} , 0xE7A10000U, 64_KB , {570, 571}),
    Osal_Device("dof_01", "dof", "vip1", {10}, 0xE7BB0000U, 64_KB , {572, 573}),

    // STV/ACF # TBD
    Osal_Device("stv_00", "stv", "vip1", {5} , 0xE7A00000U, 64_KB, {554,555}),
    Osal_Device("stv_01", "stv", "vip1", {6} , 0xE7BA0000U, 64_KB, {556,557}),
    Osal_Device("acf_00", "acf", "vip1", {0} , 0xE7A50000U, 64_KB, {544,545}),
    Osal_Device("acf_01", "acf", "vip1", {1} , 0xE7A60000U, 64_KB, {546,547}),
    Osal_Device("acf_02", "acf", "vip1", {2} , 0xE7A70000U, 64_KB, {548,549}),
    Osal_Device("acf_03", "acf", "vip1", {3} , 0xE7A80000U, 64_KB, {550,551}),

#elif defined(RCAR_V4H) /* V4H: V4H1 and V4H2 */
    //Osal_Device("imp_top_00", "imp_top", "ir", {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
    //            0xFF900000, 16_KB, {576, 577, 578, 579, 580, 581, 582}),

    // IMP_TOP # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_top_00", "imp_top", "ir", {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
                0xFF900000U, 24_KB, {565, 566, 567}),

    // IMP
    Osal_Device("imp_00", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA00000, 128_KB),
    Osal_Device("imp_01", "imp", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA20000, 128_KB),
    Osal_Device("imp_02", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFB00000, 128_KB),
    Osal_Device("imp_03", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFB20000, 128_KB),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA40000, 64_KB),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA50000, 64_KB),
    Osal_Device("imp_ocv_02", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA60000, 64_KB),
    Osal_Device("imp_ocv_03", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA70000, 64_KB),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA80000U, 16_KB),
    Osal_Device("imp_dma_01", "imp_dma", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB80000U, 16_KB),

    // IMP_PSC
    Osal_Device("imp_psc_00", "imp_psc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA84000U, 16_KB),

    // IMP DTA # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_dta_00", "imp_dta", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA88000U, 16_KB),

    // IMP SLIM DMAC # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_dma_slim_00", "imp_dma_slim", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA90000U, 16_KB),
    Osal_Device("imp_dma_slim_01", "imp_dma_slim", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA94000U, 16_KB),

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFAA0000U, 64_KB),

    // IMPC aka SPMI/SPMC
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED300000, 1_MB),
    Osal_Device("imp_spmc_00", "imp_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED400000, 2_MB),

#if defined(RCAR_V4H2) /* V4H2 Only */
    Osal_Device("imp_spmc_01", "imp_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED600000, 1_MB / 2),
#endif

    // IMP Regs
    Osal_Device("imp_reg_spmi_00", "imp_reg_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA8C000, 16_KB),
    Osal_Device("imp_reg_spmc_00", "imp_reg_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFAB0000, 16_KB),

    // IMP LDMAC
    // Osal_Device("imp_ldmac_master_00", "imp_ldmac_master",   "ir", {2}, 0xF1F00000, 4_KB, {584}),
    // Osal_Device("imp_ldmac_checker_00", "imp_ldmac_checker", "ir", {2}, 0xF1F10000, 4_KB),

    // IMP VDSP # Note that Bus Name and uTLB are TBD
    Osal_Device("vdsp00_00", "vdsp00",   "ir", {13, 14}, 0xF1400000, 0x1A0000, {478, 479, 480, 481, 482}),
    Osal_Device("vdsp01_00", "vdsp01",   "ir", {13, 14}, 0xF1600000, 0x1A0000, {483, 484, 485, 486, 487}),
    Osal_Device("vdsp02_00", "vdsp02",   "ir", {13, 14}, 0xF1800000, 0x1A0000, {488, 489, 490, 491, 492}),
    Osal_Device("vdsp03_00", "vdsp03",   "ir", {13, 14}, 0xF1A00000, 0x1A0000, {493, 494, 495, 496, 497}),

    // IMR
    Osal_Device("ims_00", "ims", "vc", {8},    0xFE860000U, 64_KB, {579}),
    Osal_Device("ims_01", "ims", "vc", {9},    0xFE870000U, 64_KB, {580}),
    Osal_Device("imr_00", "imr", "vc", {0, 4}, 0xFE880000U, 64_KB, {581}),
    Osal_Device("imr_01", "imr", "vc", {1, 5}, 0xFE890000U, 64_KB, {582}),
    Osal_Device("imr_02", "imr", "vc", {2, 6}, 0xFE8A0000U, 64_KB, {583}),

    // ISP # TBD
    Osal_Device("cisp_00", "cisp", "vi0", {10, 14, 18, 22, 26}, 0xFEC00000, 128_KB, {475}),
    Osal_Device("tisp_00", "tisp", "invalid", {UTLB_ID_INVALID}, 0xFED00000, 64_KB , {473}),

    Osal_Device("cisp_01", "cisp", "vi0", {11, 15, 19, 23, 27}, 0xFEE00000, 128_KB, {476}),
    Osal_Device("tisp_01", "tisp", "invalid", {UTLB_ID_INVALID}, 0xFED20000, 64_KB , {474}),

    // DOF
    Osal_Device("dof_00", "dof", "vip0", {3} , 0xE7A10000U, 64_KB , {242, 243}),

    // VIP_WRAP
    Osal_Device("vip_wrap_00", "vip_wrap", "vip0", {1,2,3} , 0xE7B3B000U, 4_KB ),

    // SMD
    Osal_Device("smd_ps_00", "smd_ps", "vip0", {2} , 0xE7A20000U, 64_KB , {112, 151}),
    Osal_Device("smd_post_00", "smd_post", "vip0", {1} , 0xE7A40000U, 64_KB , {173, 235}),

#elif defined(RCAR_V4M) /* V4H */
    // IMP_TOP # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_top_00", "imp_top", "ir", {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
                0xFF900000U, 24_KB, {565, 566, 567}),

    // IMP
    Osal_Device("imp_00", "imp", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA00000, 128_KB),
    Osal_Device("imp_01", "imp", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA20000, 128_KB),

    // IMP_OCV
    Osal_Device("imp_ocv_00", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA40000, 64_KB),
    Osal_Device("imp_ocv_01", "imp_ocv", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA50000, 64_KB),
    Osal_Device("imp_ocv_02", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA60000, 64_KB),
    Osal_Device("imp_ocv_03", "imp_ocv", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA70000, 64_KB),

    // IMP_DMA
    Osal_Device("imp_dma_00", "imp_dma", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA80000U, 16_KB),
    Osal_Device("imp_dma_01", "imp_dma", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFB80000U, 16_KB),

    // IMP_PSC
    Osal_Device("imp_psc_00", "imp_psc", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA84000U, 16_KB),

    // IMP DTA # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_dta_00", "imp_dta", "ir", {16, 17, 18, 19, 20, 21, 22, 23}, 0xFFA88000U, 16_KB),

    // IMP SLIM DMAC # Note that Bus Name and uTLB are TBD.
    Osal_Device("imp_dma_slim_00", "imp_dma_slim", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA90000U, 16_KB),
    Osal_Device("imp_dma_slim_01", "imp_dma_slim", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA94000U, 16_KB),

    // IMP_CNN
    Osal_Device("imp_cnn_00", "imp_cnn", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFAA0000U, 64_KB),

    // IMPC aka SPMI/SPMC
    Osal_Device("imp_spmi_00", "imp_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED200000, 2_MB),
    Osal_Device("imp_spmc_00", "imp_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED400000, 2_MB),
    Osal_Device("imp_spmc_01", "imp_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xED600000, 1_MB / 2),

    // IMP Regs
    Osal_Device("imp_reg_spmi_00", "imp_reg_spmi", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFA8C000, 16_KB),
    Osal_Device("imp_reg_spmc_00", "imp_reg_spmc", "ir", {24, 25, 26, 27, 28, 29, 30, 31}, 0xFFAB0000, 16_KB),

    // IMP VDSP # Note that Bus Name and uTLB are TBD
    Osal_Device("vdsp00_00", "vdsp00",   "ir", {13, 14}, 0xF1400000, 0x1A0000, {478, 479, 480, 481, 482}),
    Osal_Device("vdsp01_00", "vdsp01",   "ir", {13, 14}, 0xF1600000, 0x1A0000, {483, 484, 485, 486, 487}),

    // IMR
    Osal_Device("ims_00", "ims", "vc", {8},    0xFE860000U, 64_KB, {579}),
    Osal_Device("ims_01", "ims", "vc", {9},    0xFE870000U, 64_KB, {580}),
    Osal_Device("imr_00", "imr", "vc", {0, 4}, 0xFE880000U, 64_KB, {581}),
    Osal_Device("imr_01", "imr", "vc", {1, 5}, 0xFE890000U, 64_KB, {582}),

    // ISP # TBD
    Osal_Device("cisp_00", "cisp", "vi0", {10, 14, 18, 22, 26}, 0xFEC00000, 128_KB, {475}),
    Osal_Device("tisp_00", "tisp", "invalid", {UTLB_ID_INVALID}, 0xFED00000, 64_KB , {473}),
    Osal_Device("tisp_01", "tisp", "invalid", {UTLB_ID_INVALID}, 0xFED20000, 64_KB , {474}),

    // DOF
    Osal_Device("dof_00", "dof", "vip0", {3} , 0xE7A10000U, 64_KB , {861, 862}),

    // VIP_WRAP
    Osal_Device("vip_wrap_00", "vip_wrap", "vip0", {1,2,3} , 0xE7B3B000U, 4_KB ),

    // SMD
    Osal_Device("smd_ps_00", "smd_ps", "vip0", {2} , 0xE7A20000U, 64_KB , {873, 874}),
    Osal_Device("smd_post_00", "smd_post", "vip0", {1} , 0xE7A40000U, 64_KB , {871, 872}),

#else
#error                                                                                                                 \
    "unknown SoC. Only V3M2 and V3Hx.x and V3U are supported. Please set RCAR_V3M2, RCAR_V3H1, RCAR_V3U or RCAR_V4H defintions."
#endif
};
/** @} */
