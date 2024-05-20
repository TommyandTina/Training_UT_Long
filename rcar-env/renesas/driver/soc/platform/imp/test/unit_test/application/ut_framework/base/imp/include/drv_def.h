#ifndef _DRV_DEF_H
#define _DRV_DEF_H
/**************************
 * Driver definition
 * ***********************/
#include "rcar-xos/imp/r_impdrv.h"
#include "rcar-xos/imp/r_impdrv_version.h"
#include "r_impdrv_private.h"
#include "r_impdrv_osdep.h"
#include "r_impdrv_corectl.h"
#include "r_impdrv_cnnctl.h"
#include "r_impdrv_dmactl.h"
#include "r_impdrv_genctl.h"
#include "r_impdrv_impctl.h"
#include "r_impdrv_ocvctl.h"
#include "r_impdrv_pscctl.h"
#include "r_impdrv_udefctl.h"

#include "ut_osal_stub.h"

#define VALID_HANDLE              (3)
#define INVALID_HANDLE            (4)
#define IMPDRV_INTEN0_CORE_MAX      (11U)               /**< Maximum number of cores for the Interrupt group 1. */
#define IMPDRV_INTEN1_CORE_MAX      (10U)               /**< Maximum number of cores for the Interrupt group 2. */
#define IMPDRV_MINTEN_CORE_MAX      (21U)               /**< Maximum number of cores for the Merge interrupt group. */
#define IMPDRV_REG_G00INTSEL        (0x0030U)           /**< Offset address of hardware register for G00INTSEL. */
#define IMPDRV_REG_G01INTSEL        (0x0034U)           /**< Offset address of hardware register for G01INTSEL. */
#define IMPDRV_REG_G02INTSEL        (0x0038U)           /**< Offset address of hardware register for G02INTSEL. */
#define IMPDRV_REG_G10INTSEL        (0x0050U)           /**< Offset address of hardware register for G10INTSEL. */
#define IMPDRV_REG_G11INTSEL        (0x0054U)           /**< Offset address of hardware register for G11INTSEL. */
#define IMPDRV_REG_G12INTSEL        (0x0058U)           /**< Offset address of hardware register for G12INTSEL. */
#define IMPDRV_REG_MG0INTSEL        (0x00B4U)           /**< Offset address of hardware register for MG0INTSEL. */
#define IMPDRV_REG_MG1INTSEL        (0x00B8U)           /**< Offset address of hardware register for MG1INTSEL. */
#define IMPDRV_REG_MG2INTSEL        (0x00BCU)           /**< Offset address of hardware register for MG2INTSEL. */
#define IMPDRV_CNN_CNF_CLR          (0x00000000U)       /**< CLR register value for CCN.*/
#define IMPDRV_DMAS_VCR                  (0x19082120U) 
#define IMPDRV_DMAS_V1_VCR               (0x20021013U)
#define IMPDRV_DMAS_V2_VCR               (0x22032218U)
#define IMPDRV_DSP_CORE_0           (0U)                /**< DSP core number (0) */
#define IMPDRV_DSP_CORE_1           (1U)                /**< IRQ core number (1) */
#define IMPDRV_DSP_CORE_2           (2U)                /**< IRQ core number (2) */
#define IMPDRV_DSP_CORE_3           (3U)                /**< IRQ core number (3) */
#define IMPDRV_OSAL_DSP_DEV_NUM_MAX (IMPDRV_DSP_DEV_NUM_MAX + 1U)   /**< Num of max sub device for OSAL DSP core. */

#if defined(RCAR_V3U)
#define IMPDRV_IMP_VCR              (0x00700000U)       /**< Hardware version value for IMP. */
#define IMPDRV_OCV_VCR              (0x00000005U)       /**< Hardware version value for OCV. */
#define IMPDRV_DMA_VCR              (0x18071916U)       /**< Hardware version value for DMA. */
#define IMPDRV_PSC_VCR              (0x18082420U)       /**< Hardware version value for PSC. */
#define IMPDRV_CNN_VCR              (0x00030200U)       /**< Hardware version value for CNN. */

#define IMPDRV_OCV_1KB_VAL          (0x80064100U)       /**< 1KB value for OCV. */
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)       /**< MCR0 value for OCV. */
#define IMPDRV_IMP_IFCFG_VAL 		(0x010000CCU)       /**< IFCFG_VAL register value for IMP. */
#define IMPDRV_GROUP_MAX            (3U)                /**< Maximum number of interrupt groups. */
#define IMPDRV_INSTANCETABLE_NUM    (8U)                /**< Maximum number of Instance number table. */

/* Temporary defined */
#define IMPDRV_REG_IMR_INIT_VAL     (0xE007FFFFU)
#define IMPDRV_REG_G0INTSEL         (0x0110U)           /**< Offset address of hardware register for G0INTSEL. */
#define IMPDRV_REG_G1INTSEL         (0x0114U)           /**< Offset address of hardware register for G1INTSEL. */
#define IMPDRV_REG_G2INTSEL         (0x0118U)           /**< Offset address of hardware register for G2INTSEL. */
#define IMPDRV_INTEN_CORE_MAX       (10U)               /**< Maximum number of cores for the Interrupt. */

#elif defined(RCAR_V3H2)
#define IMPDRV_IMP_VCR              (0x00610000U)		/**< Hardware version value for IMP. */
#define IMPDRV_OCV_VCR              (0x00000014U)       /**< Hardware version value for OCV. */
#define IMPDRV_DMA_VCR              (0x19102516U)       /**< Hardware version value for DMA. */
#define IMPDRV_PSC_VCR              (0x17020121U)       /**< Hardware version value for PSC. */
#define IMPDRV_CNN_VCR              (0x00030001U)       /**< Hardware version value for CNN. */

#define IMPDRV_OCV_1KB_VAL          (0x80064100U)       /**< 1KB value for OCV. */
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)       /**< MCR0 value for OCV. */
#define IMPDRV_OCV_RESET_STATUS_VAL (0x00F10000U)          /**< RESET_STATUS value for OCV. */
#define IMPDRV_OCV_8KBNEXT_VAL      (0x2000U)           /**< dummy value becasue 8KBNEXT value for OCV does not have for V3H2. */
#define IMPDRV_REG_IMR_INIT_VAL     (0xE007FFFFU)
#define IMPDRV_IMP_IFCFG_VAL        (0x010010CCU)

#define IMPDRV_INTEN_CORE_MAX       (19U)               /**< Maximum number of cores for the Interrupt. */
#define IMPDRV_GROUP_MAX            (3U)                /**< Maximum number of interrupt groups. */
#define IMPDRV_INSTANCETABLE_NUM    (6U)                /**< Maximum number of Instance number table. */

#define IMPDRV_REG_G0INTSEL         (0x0110U)           /**< Offset address of hardware register for G0INTSEL. */
#define IMPDRV_REG_G1INTSEL         (0x0114U)           /**< Offset address of hardware register for G1INTSEL. */
#define IMPDRV_REG_G2INTSEL         (0x0118U)           /**< Offset address of hardware register for G2INTSEL. */

#define IMPDRV_MASK_L16BIT          (0x0000FFFFU)   /**< Bit mask for Lower 16 bits                    */
#define IMPDRV_REG_MB_MEM_INIT_ODD       (0x0000AAAAU)   /** Multi-Bank memory odd initialization value.    */
#define IMPDRV_REG_MB_MEM_INIT_EVEN      (0x00005555U)   /** Multi-Bank memory even initialization value.   */
#define IMPDRV_IMP_INTSTS_MASKCPU   (0x10000000U)       /**< MASKCPU interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_TRAP      (0x00000040U)       /**< TRAP interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_IER       (0x00000080U)       /**< IER interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U)       /**< INT interrupt enable registration. */
#define IMPDRV_IMPS_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for Slim-IMP. */
#define IMPDRV_IMP_INTSTS_APIPINT   (0x00000001U)       /**< APIPINT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_HPINT     (0x00000002U)       /**< HPINT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_WUP       (0x04000000U)       /**< WUP interrupt enable registration. */
#define IMPDRV_OCV_8KBSTART_VAL     (0x0000U)           /**< 8KBSTART value for OCV. */
#define IMPDRV_DMAS_CORENUM_VAL     (2U)
#define IMPDRV_DMAS_CORENUM0MAIN         (0U)
#define IMPDRV_DMAS_CORENUM0SUB          (1U)
#define IMPDRV_REG_DMAS_THREAD_OFFSET    (0x00000800U)
#define IMPDRV_DMAS_SER_FIX              (0x0000F300U)
#elif defined(RCAR_V3H1)
#define IMPDRV_IMP_VCR              (0x00600000U)       /**< Hardware version value for IMP. */
#define IMPDRV_OCV_VCR              (0x00000004U)       /**< Hardware version value for OCV. */
#define IMPDRV_DMA_VCR              (0x17021420U)       /**< Hardware version value for DMA. */
#define IMPDRV_PSC_VCR              (0x17020121U)       /**< Hardware version value for PSC. */
#define IMPDRV_CNN_VCR              (0x00030000U)       /**< Hardware version value for CNN. */
#define IMPDRV_OCV_1KB_VAL          (0x80064100U)       /**< 1KB value for OCV. */
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)       /**< MCR0 value for OCV. */
#define IMPDRV_OCV_RESET_STATUS_VAL (0x10000U)          /**< RESET_STATUS value for OCV. */
#define IMPDRV_OCV_8KBNEXT_VAL      (0x2000U)           /**< 8KBNEXT value for OCV. */
#define IMPDRV_IMP_INTSTS_APIPINT   (0x00000001U)       /**< APIPINT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_HPINT     (0x00000002U)       /**< HPINT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_WUP       (0x04000000U)       /**< WUP interrupt enable registration. */

#define IMPDRV_REG_IMR_INIT_VAL     (0xE003FFFFU)       /**< Initial value of interrupt mask register. */
#define IMPDRV_IMP_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for IMP. */

#define IMPDRV_INTEN_CORE_MAX       (18U)               /**< Maximum number of cores for the Interrupt. */
#define IMPDRV_GROUP_MAX            (3U)                /**< Maximum number of interrupt groups. */
#define IMPDRV_INSTANCETABLE_NUM    (6U)                /**< Maximum number of Instance number table. */

#define IMPDRV_REG_G0INTSEL         (0x0110U)           /**< Offset address of hardware register for G0INTSEL. */
#define IMPDRV_REG_G1INTSEL         (0x0114U)           /**< Offset address of hardware register for G1INTSEL. */
#define IMPDRV_REG_G2INTSEL         (0x0118U)           /**< Offset address of hardware register for G2INTSEL. */

#define IMPDRV_MASK_L16BIT          (0x0000FFFFU)   /**< Bit mask for Lower 16 bits                    */
#define IMPDRV_REG_MB_MEM_INIT_ODD       (0x0000AAAAU)   /** Multi-Bank memory odd initialization value.    */
#define IMPDRV_REG_MB_MEM_INIT_EVEN      (0x00005555U)   /** Multi-Bank memory even initialization value.   */
#define IMPDRV_IMP_INTSTS_MASKCPU   (0x10000000U)       /**< MASKCPU interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_TRAP      (0x00000040U)       /**< TRAP interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_IER       (0x00000080U)       /**< IER interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U)       /**< INT interrupt enable registration. */
#define IMPDRV_IMPS_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for Slim-IMP. */
#define IMPDRV_OCV_8KBSTART_VAL     (0x0000U)           /**< 8KBSTART value for OCV. */

#elif defined(RCAR_V3M2)
#define IMPDRV_IMP_VCR              (0x00600000U)       /**< Hardware version value for IMP. */
#define IMPDRV_OCV_VCR              (0x00000004U)       /**< Hardware version value for OCV. */
#define IMPDRV_DMA_VCR              (0x17021420U)       /**< Hardware version value for DMA. */
#define IMPDRV_PSC_VCR              (0x17020121U)       /**< Hardware version value for PSC. */
#define IMPDRV_CNN_VCR              (0x00030101U)       /**< Hardware version value for CNN. */

#define IMPDRV_OCV_1KB_VAL          (0x80064100U)       /**< 1KB value for OCV. */
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)       /**< MCR0 value for OCV. */
#define IMPDRV_OCV_RESET_STATUS_VAL (0x10000U)          /**< RESET_STATUS value for OCV. */
#define IMPDRV_OCV_8KBNEXT_VAL      (0x2000U)           /**< 8KBNEXT value for OCV. */
#define IMPDRV_REG_IMR_INIT_VAL     (0xE00003FFU)       /**< Initial value of interrupt mask register. */
#define IMPDRV_IMP_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for IMP. */
#define IMPDRV_INTEN_CORE_MAX       (10U)               /**< Maximum number of cores for the Interrupt. */

#define IMPDRV_REG_G0INTSEL         (0x0110U)           /**< Offset address of hardware register for G0INTSEL. */
#define IMPDRV_REG_G1INTSEL         (0x0114U)           /**< Offset address of hardware register for G1INTSEL. */
#define IMPDRV_REG_G2INTSEL         (0x0118U)           /**< Offset address of hardware register for G2INTSEL. */
#define IMPDRV_INSTANCETABLE_NUM    (6U)                /**< Maximum number of Instance number table. */

#define IMPDRV_MASK_L16BIT          (0x0000FFFFU)   /**< Bit mask for Lower 16 bits                    */
#define IMPDRV_REG_MB_MEM_INIT_ODD       (0x0000002AU)   /** Multi-Bank memory odd initialization value.    */
#define IMPDRV_REG_MB_MEM_INIT_EVEN      (0x00000055U)   /** Multi-Bank memory even initialization value.   */
#define IMPDRV_IMP_INTSTS_MASKCPU   (0x10000000U)       /**< MASKCPU interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_TRAP      (0x00000040U)       /**< TRAP interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_IER       (0x00000080U)       /**< IER interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U)       /**< INT interrupt enable registration. */
#define IMPDRV_IMPS_IFCFG_VAL        (0x010010CCU)       /**< IFCFG_VAL register value for Slim-IMP. */
#define IMPDRV_IMP_INTSTS_APIPINT   (0x00000001U)       /**< APIPINT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_HPINT     (0x00000002U)       /**< HPINT interrupt enable registration. */

#define IMPDRV_IMP_INTSTS_WUP       (0x04000000U)       /**< WUP interrupt enable registration. */
#define IMPDRV_OCV_8KBSTART_VAL     (0x0000U)           /**< 8KBSTART value for OCV. */

#elif defined(RCAR_V4H) || defined(RCAR_V4H2) || defined(RCAR_V4M)
#define IMPDRV_INSTANCETABLE_NUM    (6U)  
#define IMPDRV_IMP_INTSTS_APIPINT   (0x00000001U)
#define IMPDRV_IMP_INTSTS_HPINT     (0x00000002U)
#define IMPDRV_IMP_INTSTS_WUP       (0x04000000U)
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U) 
#define IMPDRV_IMP_INTSTS_INT       (0x00000020U)       /**< INT interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_IER       (0x00000080U)       /**< IER interrupt enable registration. */
#define IMPDRV_IMP_INTSTS_TRAP      (0x00000040U)   
#define IMPDRV_IMPS_INTSTS_MASKCPU  (0x10000000U)  
#define IMPDRV_IMP_INTSTS_MASKCPU   (0x10000000U)
#define IMPDRV_IMP_IFCFG_VAL        (0x010010CCU)
#define IMPDRV_IMP_VCR              (0x00800000U)
#define IMPDRV_REG_IMR_INIT_VAL     (0xE007FFFFU)
#define IMPDRV_INTEN_CORE_MAX       (18U)
#define IMPDRV_REG_G0INTSEL         (0x0110U)           /**< Offset address of hardware register for G0INTSEL. */
#define IMPDRV_REG_G1INTSEL         (0x0114U)           /**< Offset address of hardware register for G1INTSEL. */
#define IMPDRV_REG_G2INTSEL         (0x0118U) 
#define IMPDRV_REG_MB_MEM_INIT_EVEN      (0x00005555U)
#define IMPDRV_REG_MB_MEM_INIT_ODD       (0x0000AAAAU)
#define IMPDRV_MASK_L16BIT               (0x0000FFFFU)
#define IMPDRV_DMA_VCR                  (0x20021013U)
#define IMPDRV_IMPS_IFCFG_VAL        (0x010010CCU)
#define IMPDRV_OCV_MCR0_INIT_VAL    (0x01000000U)
#define IMPDRV_OCV_1KB_VAL          (0x80064100U)
#define IMPDRV_OCV_8KBNEXT_VAL      (0x2000U)
#define IMPDRV_OCV_RESET_STATUS_VAL (0x00F10000U)
#define IMPDRV_OCV_8KBSTART_VAL     (0x0000U)
#define IMPDRV_OCV_VCR              (0x00000006U)
#define IMPDRV_PSC_VCR              (0x20040718U)
#define IMPDRV_PSC_CORENUM_VAL      (1U)
#define IMPDRV_DTA_CORENUM_VAL      (0U)
#define IMPDRV_CNN_CORENUM_VAL      (1U)
#define IMPDRV_LDMA_CORENUM_VAL     (0U)
#define IMPDRV_DMA_CORENUM_VAL      (4U)
#define IMPDRV_DMAS_CORENUM_VAL     (4U)
#define IMPDRV_OCV_CORENUM_VAL      (4U)
#define IMPDRV_IMPS_CORENUM_VAL     (1U)
#define IMPDRV_IMP_CORENUM_VAL      (4U)
#define IMPDRV_DSP_CORENUM_VAL          (4U)
#define IMPDRV_IMP_EXEPARAM_CLBRK   (0U)
#define IMPDRV_IMP_INTSTS_CLBRK     (0x00000400U) 
#define IMPDRV_CNN_SR_WUPCOVF       (0x00000010U)
#define IMPDRV_CNN_SR_MSCO          (0x00000008U)
#define IMPDRV_CNN_SR_CLBRK         (0x00000020U)
#define IMPDRV_CNN_EXEPARAM_CLBRK   (0U)
#define IMPDRV_CNN_SR_INT           (0x00000004U)
#define IMPDRV_CNN_SR_IER           (0x00000002U)
#define IMPDRV_CNN_SR_TRAP          (0x00000001U)
#define IMPDRV_DMA_SR_WUP               (0x00000080U)
#define IMPDRV_DMA_SR_CLBRK             (0x00000400U) 
#define IMPDRV_DMA_EXEPARAM_CLBRK       (0U)
#define IMPDRV_DMA_SR_INT               (0x00000040U)
#define IMPDRV_DMA_SR_IER               (0x00000020U)
#define IMPDRV_DMA_SR_TRAP              (0x00000010U) 
#define IMPDRV_DMAS_CORENUM0MAIN         (0U)
#define IMPDRV_DMAS_CORENUM0SUB          (1U)
#define IMPDRV_DMAS_CORENUM1MAIN         (2U)
#define IMPDRV_DMAS_CORENUM1SUB          (3U)
#define IMPDRV_REG_DMAS_THREAD_OFFSET    (0x00001000U)
#define IMPDRV_DMAS_SER_FIX              (0x0001F300U)
#endif

#if defined(RCAR_V4H2) || defined(RCAR_V4M)
#define IMPDRV_CNN_VCR              (0x00040001U) 
#elif defined(RCAR_V4H)
#define IMPDRV_CNN_VCR              (0x00030200U)
#endif

typedef struct
{
    uint32_t                    group_bit;              /**< Bit value to convert */
    uint32_t                    group_reg;              /**< Offset address value to convert */
} st_impdrv_group_tbl_t;


typedef struct
{
    st_impdrv_device_handle_t   device_io_imptop;                       /**< OSAL device handle (IMP TOP).          */
    st_impdrv_mutex_handle_t    mutex_handle;                           /**< OSAL mutex handle                      */
    osal_mutex_id_t             mutex_id;                               /**< OSAL mutex id                          */
    osal_milli_sec_t            mutex_time_period;                      /**< Millisecond Order Mutex Timeout value  */
    st_impdrv_device_handle_t   device_io_dta;                          /**< OSAL device handle (DTA Core).         */
    st_impdrv_commonctl_t       *p_commonctl[IMPDRV_INSTANCETABLE_NUM]; /**< Address of the commonctl..             */
} st_impdrv_commonctl_ins_t;
extern st_impdrv_commonctl_ins_t g_impdrv_cmn_ctl_ins;

typedef struct
{
    uint32_t                    checksum;                               /**< Checksum value of Global variable.     */
    st_impdrv_device_handle_t   device_io_imptop;                       /**< Backup of OSAL device handle(IMP TOP). */
} st_impdrv_commonctl_chk_t;
extern st_impdrv_commonctl_chk_t g_impdrv_cmn_ctl_chk;

typedef struct
{
    uint32_t   checksum;
} st_impdrv_hwrsc_chk_t;
extern st_impdrv_hwrsc_chk_t g_hwrsc_chk_table;


typedef enum
{
    IMPDRV_MB_MBM_INIT_ODD,                        /**< Multi-Bank memory odd initialization       */
    IMPDRV_MB_MBM_INIT_EVEN                        /**< Multi-Bank memory even initialization      */
} e_impdrv_mb_mem_init_t;

typedef struct
{
    st_impdrv_device_handle_t *p_device_handle; /**< OSAL device handle */
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      is_sub_thread;    /**< Select for DMAC Internal thread */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_dma_write_info_t;

typedef struct
{
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_dma_write_info_tbl_t;

#if defined(RCAR_V4H) || defined(RCAR_V4H2) || defined(RCAR_V4M)
extern uint32_t impdrv_impctl_get_inten_val(
    const st_impdrv_corectl_t   *const p_corectl
);
#else
extern uint32_t impdrv_impctl_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
);
#endif
extern e_impdrv_errorcode_t impdrv_cmnctl_reg_find_resource(
    const char              *const p_device_id,
    uint32_t                *const p_index,
    st_impdrv_hwrsc_def_t   *const p_resource
);
#if !defined(RCAR_V4H) || defined(RCAR_V4H2) || defined(RCAR_V4M)
extern uint32_t impdrv_cnnctl_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
);
#endif

#if RCAR_V3H2
e_impdrv_errorcode_t impdrv_ocvctl_init_lwm(
    const st_impdrv_corectl_t   *const p_corectl
);
#endif

#endif /* _DRV_DEF_H */
