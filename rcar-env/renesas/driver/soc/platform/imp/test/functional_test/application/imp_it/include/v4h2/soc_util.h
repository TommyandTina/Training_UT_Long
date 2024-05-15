#include "r_impdrv.h"
#include "r_impdrv_private.h"

#define IMP_CORE_MAX					(4)
#define IMP_SLIM_CORE_MAX				(0)
#define OCV_CORE_MAX					(4)
#define DMA_CORE_MAX					(4)
#define DMA_SLIM_CORE_MAX				(4)
#define PSCEXE_CORE_MAX					(1)
#define CNN_CORE_MAX					(1)
#define DSP_CORE_MAX					(4)
#define READ_NUM_FOR_INIT				(19)
#define READ_NUM_FOR_START				(60)
#define TOTAL_IRQ_MASK_CORES			(9)

#define IMPDRV_REGVAL_VDSP_TRAP             (0x00010000U)
#define IMPDRV_REGVAL_VDSP_INT              (0x00020000U)
#define IMPDRV_REGVAL_VDSP_RESUME           (0x00020000U)
#define IMPDRV_REGVAL_VDSP_ERROR            (0x00030000U)   /* for VDSP error notice */

#define IMPDRV_REGVAL_VDSP_GPIO_START       (0x00000008U)
#define IMPDRV_REGVAL_VDSP_GPIO_STOP        (0x00000080U)
#define IMPDRV_REGVAL_VDSP_GPIO_F_STANDBY   (0x00000800U)
#define IMPDRV_REGVAL_VDSP_GPIO_STANDBY     (0x00000001U)
#define IMPDRV_REGVAL_VDSP_GPIO_INIT        (0x00000000U)
#define IMPDRV_REGVAL_VDSP_GPIO_RESUME      (0x00002000U)

#define IMPDRV_REGVAL_DMBE_IDLE             (0x00011FFFU)
#define IMPDRV_REGVAL_PSVM_PMOD_SBY         (0x00000003U)   /* 00:FreeRun, 01:DPS, 10:LightSleep, 11:Standby */
#define IMPDRV_REGVAL_PSU_STATE_SBY         (0x00000004U)   /* 0:AXI_LOWPWR_INDI,1:AXI_LOWPWR_ACK,2:STANDBY_MD,4:LIGHT_SLP_MD  */
#define IMPDRV_REGVAL_PSU_STATE_LSP         (0x00000008U)   /* 0:AXI_LOWPWR_INDI,1:AXI_LOWPWR_ACK,2:STANDBY_MD,4:LIGHT_SLP_MD  */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_VCR
 * Offset address of VCR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_VCR				(0x0004U)			/**< Offset address of VCR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_VCR0
 * Offset address of VCR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_VCR0				(0x0000U)			/**< Offset address of VCR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_VCR
 * Offset address of VCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_VCR				(0x0000U)			/**< Offset address of VCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMAS_VCR
 * Offset address of VCR register for DMAS.
***********************************************************************************************************************/
#define IMPDRV_REG_DMAS_VCR				(0x0000U)			/**< Offset address of VCR register for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_VCR
 * Offset address of VCR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_VCR				(0x0000U)			/**< Offset address of VCR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_CNN_VCR
 * Offset address of VCR register for CNN.
***********************************************************************************************************************/
#define IMPDRV_REG_CNN_VCR				(0x0000U)			/**< Offset address of VCR register for CNN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INSTANCETABLE_NUM
 * Maximum number of Instance number table.
***********************************************************************************************************************/
#define IMPDRV_INSTANCETABLE_NUM		(4U)				/**< Maximum number of Instance number table. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DUMMY_UINT32
 * Uint32_t dummy value.
***********************************************************************************************************************/
#define IMPDRV_DUMMY_UINT32				(0xFFFFFFFFU)		/**< Uint32_t dummy value. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN00
 * Offset address of hardware register for INTEN00.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN00				(0x0020U)			/**< Offset address of hardware register for INTEN00. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN01
 * Offset address of hardware register for INTEN01.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN01				(0x0024U)			/**< Offset address of hardware register for INTEN01. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN02
 * Offset address of hardware register for INTEN02.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN02				(0x0028U)			/**< Offset address of hardware register for INTEN02. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_VCR
 * Hardware version value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_VCR					(0x00800000)		/**< Hardware version value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_VCR
 * Hardware version value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_VCR					(0x00000006U)		/**< Hardware version value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_VCR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_VCR					(0x20021013U)		/**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_VCR
 * Hardware version value for Slim-DMA.
***********************************************************************************************************************/
#define IMPDRV_DMAS_VCR					(0x20021013U)		/**< Hardware version value for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_VCR
 * Hardware version value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_VCR					(0x20040718U)		/**< Hardware version value for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_VCR
 * Hardware version value for CNN.
***********************************************************************************************************************/
#define IMPDRV_CNN_VCR					(0x00040001U)		/**< Hardware version value for CNN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_INDEX_IMP_TOP
 * Index number of IMP top resource.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_INDEX_IMP_TOP		(0U)				/** Index number of IMP top resource			*/
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_RESET_STATUS_VAL
 * RESET_STATUS value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_RESET_STATUS_VAL		(0x00010000U)		/**< RESET_STATUS value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_SCR1
 * Offset address of SCR1 register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_SCR1				(0x0018U)			/**< Offset address of SCR1 register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCF
 * Offset address of CNF register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNF				(0x0008U)			/**< Offset address of CNF register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN_INIT_VAL
 * Initial value of interrupt enable register.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN_INIT_VAL		(0x00000000U)		/**< Initial value of interrupt enable register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INTEN_CORE_MAX
 * Maximum number of cores for the Interrupt.
***********************************************************************************************************************/
#define IMPDRV_INTEN_CORE_MAX			(18U)				/**< Maximum number of cores for the Interrupt. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G00INTSEL
 * Offset address of hardware register for G00INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G00INTSEL			(0x0030U)			/**< Offset address of hardware register for G00INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G01INTSEL
 * Offset address of hardware register for G01INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G01INTSEL			(0x0034U)			/**< Offset address of hardware register for G01INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G02INTSEL
 * Offset address of hardware register for G02INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G02INTSEL			(0x0038U)			/**< Offset address of hardware register for G02INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_GROUP_MAX
 * Maximum number of interrupt groups.
***********************************************************************************************************************/
#define IMPDRV_GROUP_MAX				(5U)				/**< Maximum number of interrupt groups. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_GROUP_INIT_VAL
 * Initial value of group interrupt select register.
***********************************************************************************************************************/
#define IMPDRV_REG_GROUP_INIT_VAL		(0x00000000U)		/**< Initial value of group interrupt select register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTSTS
 * Offset address of INTSTS register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTSTS			(0x0010U)			/**< Offset address of INTSTS register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTEN
 * Offset address of INTEN register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTEN			(0x0014U)			/**< Offset address of INTEN register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_TRAP
 *	TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_TRAP			(0x00000040U)		/**< TRAP interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_MEMINITR
 * Offset address of MEMINITR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_MEMINITR			(0x00F8U)			/**< Offset address of MEMINITR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_MEM_FUNC_SA
 * Offset address of SA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_MEM_FUNC_SA		(0x0B78U)			/**< Offset address of SA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_HMDATA_VAL
 * HMDATA_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_HMDATA_VAL			(0x0000U)			/**< HMDATA_VAL register value for IMP.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCTL
 * Offset address of CIFCTL register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCTL			(0x001CU)			/**< Offset address of IFCTL register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCTL_EXE
 * EXE register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCTL_EXE			(0x00000001U)		/**< EXE register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCFG
 * Offset address of CNFG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCFG			(0x0018U)			/**< Offset address of CNFG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCF
 * Offset address of CNF register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNF				(0x0008U)			/**< Offset address of CNF register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCFG_VAL
 * IFCFG_VAL register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCFG_VAL			(0x010010CCU)		/**< IFCFG_VAL register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_MASKCPU
 *	MASKCPU interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_MASKCPU		(0x10000000U)		/**< MASKCPU interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_APIPINT
 *	APIPINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_APIPINT		(0x00000001U)		/**< APIPINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_HPINT
 *	HPINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_HPINT			(0x00000002U)		/**< HPINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_WUP
 *	WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_WUP			(0x04000000U)		/**< WUP interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_INT
 *	INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_INT			(0x00000020U)		/**< INT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_INTSTS_IER
 *	IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_IMP_INTSTS_IER			(0x00000080U)		/**< IER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR0
 * Offset address of SYNCCR0 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR0			(0x0040U)			/**< Offset address of SYNCCR0 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR1
 * Offset address of SYNCCR1 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR1			(0x0044U)			/**< Offset address of SYNCCR1 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR2
 * Offset address of SYNCCR2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR2			(0x0048U)			/**< Offset address of SYNCCR2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR3
 * Offset address of SYNCCR3 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR3			(0x004CU)			/**< Offset address of SYNCCR03 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_NAME_MAX
 * Maximum length of hardware resource name.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_NAME_MAX			(32U)				/** Maximum length of hardware resource name	*/
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCTLR
 * Offset address of SCTLR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCTLR			(0x0004U)			/**< Offset address of SCTLR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SR
 * Offset address of SR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SR				(0x0008U)			/**< Offset address of SR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCR
 * Offset address of SCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCR				(0x000CU)			/**< Offset address of SCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SER
 * Offset address of VCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SER				(0x0010U)			/**< Offset address of VCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_IMR
 * Offset address of IMR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_IMR				(0x0014U)			/**< Offset address of IMR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SCTLR1
 * Offset address of SCTLR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SCTLR1			(0x0018U)			/**< Offset address of SCTLR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_CLSAR
 * Offset address of CLSAR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_CLSAR			(0x0028U)			/**< Offset address of CLSAR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SR1
 * Offset address of SR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SR1				(0x0030U)			/**< Offset address of SR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR0
 * Offset address of SYNCCR0 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR0			(0x00E8U)			/**< Offset address of SYNCCR0 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR1
 * Offset address of SYNCCR1 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR1			(0x00ECU)			/**< Offset address of SYNCCR1 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR2
 * Offset address of SYNCCR2 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR2			(0x00F0U)			/**< Offset address of SYNCCR2 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_SYNCCR3
 * Offset address of SYNCCR3 register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_SYNCCR3			(0x00F4U)			/**< Offset address of SYNCCR3 register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SCTLR_SWRST
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR_SWRST			(0x80000000U)		/**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SCTLR_CLR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR_CLR			(0x00000000U)		/**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_VIMPDRV_DMA_SCTLR1_PSCR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_SCTLR1_PS			(0x80000000U)		/**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_THREAD_OFFSET
 * H/W Register offset for DMA sub-thread.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_THREAD_OFFSET	(0x00001000U)		/**< H/W Register offset for DMA sub-thread. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_CLBRK
 * CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_CLBRK             (0x00000400U)       /**< CLBRK interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_TEND
 *	SA_TEND interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_TEND				(0x00000001U)		/**<SA_TEND interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_TRAP				(0x00000010U)		/**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_IER
 * IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_IER				(0x00000020U)		/**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_INT
 * INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_INT				(0x00000040U)		/**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SR_WUP
 * WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMA_SR_WUP				(0x00000080U)		/**< WUP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_SER_FIX
 * Status Enable Registers that are always bit 1.
***********************************************************************************************************************/
#define IMPDRV_DMA_SER_FIX				(0x0001F300U)		/**< Status Enable Registers that are always bit 1.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_RAMTSTR
 * Offset address of RAMTSTR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_RAMTSTR				(0x00D0U)			/**< Offset address of RAMTSTR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_MASK
 * Bit mask of RAMTSTR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_MASK					(0x8000U)			/**< Bit mask of RAMTSTR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_LOOPTIME_VAL
 * read count for register.
***********************************************************************************************************************/
#define IMPDRV_DMA_LOOPTIME_VAL			(8U)				/**< read count for register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_TRAP				(0x00000001U)		/**< TRAP interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_IER
 * IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_IER				(0x00000002U)		/**< IER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_INT
 * INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_INT				(0x00000004U)		/**< INT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_PBCOVF
 *PBCOVF interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_PBCOVF			(0x00000008U)		/**< PBCOVF interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_SBO0ME
 * SBO0ME interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_SBO0ME			(0x00000010U)		/**< SBO0ME interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_USIER
 * USIER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_USIER			(0x00001000U)		/**< USIER interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_USINT
 * USINT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_USINT			(0x00002000U)		/**< USINT interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_CLBRK
 * CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_CLBRK			(0x00000400U)		/**< CLBRK interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_WUPCOVF
 * WUPCOVF interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_WUPCOVF			(0x00004000U)		/**< WUPCOVF interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_ICR2_MASK
 * Shader thread mask bits of used in ICR2 register.
***********************************************************************************************************************/
#define IMPDRV_OCV_ICR2_MASK			(0xFFFFFFFFU)		/**< Shader thread mask bits of used in ICR2 register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_MCR0_DCBANKSEL0
 * DCBANKSEL0 register value for OCV.
 * TODO: confirm setting
***********************************************************************************************************************/
#define IMPDRV_OCV_MCR0_DCBANKSEL0		(0x00040000U)		/**< DCBANKSEL0register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_MCR0_DCBANKSEL1
 * DCBANKSEL1 register value for OCV.
 * TODO: confirm setting
***********************************************************************************************************************/
#define IMPDRV_OCV_MCR0_DCBANKSEL1		(0x05000000U)		/**< DCBANKSEL1 register value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_UDIPSBRK
 * UDIPSBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_UDIPSBRK     (0x10000000U)
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_SR1_UDIVSBRK
 * UDIVSBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_OCV_SR1_UDIVSBRK     (0x20000000U)       /**< UDIVSBRK interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_TEND
 *	SA_TEND interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_TEND				(0x00000001U)		/**<SA_TEND interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_TRAP				(0x00000010U)		/**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_IER
 * IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_IER				(0x00000020U)		/**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_INT
 * INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_INT				(0x00000040U)		/**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_WUP
 * WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_WUP				(0x00000080U)		/**< WUP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SR_CLBRK
 * CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SR_CLBRK             (0x00000400U)       /**< CLBRK interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SCTLR_CLR
 * Hardware version value for DMAS.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SCTLR_CLR            (0x00000000U)       /**< Hardware version value for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_VIMPDRV_DMAS_SCTLR1_PSCR
 * Hardware version value for DMAS.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SCTLR1_PS            (0x80000000U)       /**< Hardware version value for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SCTLR_SWRST
 * Hardware version value for DMAS.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SCTLR_SWRST          (0x80000000U)       /**< Hardware version value for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_SER_FIX
 * Status Enable Registers that are always bit 1.
***********************************************************************************************************************/
#define IMPDRV_DMAS_SER_FIX				(0x0001F300U)		/**< Status Enable Registers that are always bit 1.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_RAMTSTR
 * Offset address of RAMTSTR register for DMAS.
***********************************************************************************************************************/
#define IMPDRV_DMAS_RAMTSTR				(0x00D0U)			/**< Offset address of RAMTSTR register for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_MASK
 * Bit mask of RAMTSTR register for DMAS.
***********************************************************************************************************************/
#define IMPDRV_DMAS_MASK				(0x8000U)			/**< Bit mask of RAMTSTR register for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMAS_LOOPTIME_VAL
 * read count for register.
***********************************************************************************************************************/
#define IMPDRV_DMAS_LOOPTIME_VAL		(8U)				/**< read count for register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMAS_SYNCCR0
 * Offset address of SYNCCR0 register for DMAS.
***********************************************************************************************************************/
#define IMPDRV_REG_DMAS_SYNCCR0			(0x00E8U)			/**< Offset address of SYNCCR0 register for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMAS_SCR
 * Offset address of SCR register for DMAS.
***********************************************************************************************************************/
#define IMPDRV_REG_DMAS_SCR				(0x000CU)			/**< Offset address of SCR register for DMAS. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_TEND
 *	TEND interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_TEND				(0x00000001U)		/**< TEND interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_IMR
 * Offset address of IMR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_IMR          (0x0014U)           /**< Offset address of IMR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_TRAP
 * TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_TRAP				(0x00000010U)		/**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_IER
 *	IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_IER				(0x00000020U)		/**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_CLBRK
 *  CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_CLBRK         (0x00000400U)       /**< CL break interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_INT
 *	INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_INT				(0x00000040U)		/**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_SR_WUP
 *	WUP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_PSC_SR_WUP				(0x00010000U)		/**< WUP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SCTLR0
 * Offset address of CTLR0 register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SCTLR0			(0x0004U)			/**< Offset address of CTLR0 register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_SCR
 * Offset address of SCR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_SCR				(0x000CU)			/**< Offset address of SCR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_TRAP
 *	TRAP interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_TRAP				(0x00000001U)		/**< TRAP interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_IER
 *	IER interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_IER				(0x00000002U)		/**< IER interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_INT
 *	INT interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_INT				(0x00000004U)		/**< INT interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_MSCO
 *	MSCO interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_MSCO				(0x00000008U)		/**< MSCO interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_WUPCOVF
 *	WUPCOVF interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_WUPCOVF			(0x00000010U)		/**< WUPCOVF interrupt enable registration.*/
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SR_CLBRK
 *  CLBRK interrupt enable registration.
***********************************************************************************************************************/
#define IMPDRV_CNN_SR_CLBRK         (0x00000020U)       /**< CL break interrupt enable registration. */
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_SCLP_START
 * CNN_SCLP_START register value for CNN.
***********************************************************************************************************************/
#define IMPDRV_CNN_SCLP_START			(0x00000001U)		/**< CNN_SCLP_START register value for CNN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_CNN_SWRST
 * Offset address of hardware register for SWRST.
***********************************************************************************************************************/
#define IMPDRV_REG_CNN_SWRST			(0x0008U)			/**< Offset address of hardware register for SWRST. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_CNN_SRC
 * Offset address of hardware register for SRC.
***********************************************************************************************************************/
#define IMPDRV_REG_CNN_SRC				(0x0018U)			/**< Offset address of hardware register for SRC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_MEM_SIZE_BUSCHK
 *	Memory size for Bus interface check.
***********************************************************************************************************************/
#define IMPDRV_IMP_MEM_SIZE_BUSCHK		(512U)				/** Memory size for Bus interface check. */

#define IMPDRV_VDSP_VERSION1_VAL             (0x20111119U)
#define RCVDRV_IMP_OFFSET_ALL			     (0xFFFFFFFF)
#define RCVDRV_DEINITASK_TIMEOUT		     (1000)
#define IMPDRV_DEV_LIST_SIZE        	     (255U)
#define IMPDRV_OCV_CLCNDGSBR_MASK   	     (0x00000001U)
#define IMPDRV_IMP_CLCNDGSBR_MASK   	     (0x00000001U)
#define IMPDRV_DMA_CLCNDGSBR_MASK            (0x00000001U)
#define IMPDRV_CNN_CLCNDGSBR_MASK   	     (0x00000001U)
#define IMPDRV_PSC_CLCNDGSBR_MASK   	     (0x00000001U)
#define IMPDRV_REGVAL_OUT_INT_TIMER00        (0x0001U)           /**< I_TIMER00 value for VDSP */
#define IMPDRV_REGVAL_OUT_INT_TIMER01        (0x0002U)           /**< I_TIMER01 value for VDSP */
#define IMPDRV_REGVAL_VDSP_CRC_ERROR         (0x80000000U)
#define IMPDRV_REGVAL_VDSP_NORMAL_INT        (0x40000000U)
#define IMPDRV_REGVAL_VDSP_CRITICAL_INT      (0x20000000U)
#define IMPDRV_REGVAL_VDSP_SAFETY_ERROR      (0x10000000U)
#define IMPDRV_REGVAL_VDSP_DIV0              (0x08000000U)
#define IMPDRV_REGVAL_VDSP_WDOG_EXPIRE       (0x04000000U)
#define IMPDRV_REGVAL_VDSP_STACK_VIOLATION   (0x02000000U)
#define IMPDRV_REGVAL_VDSP_TIM_FAULT         (0x01000000U)

#define IMPDRV_REGVAL_CLR               (0x00000000U)       /**< register clear value for VDSP */
#define IMPDRV_REGVAL_VDSP_GPIO_RESUME  (0x00002000U)       /**< GPIO_OUT register resume value for VDSP */
#define IMPDRV_CNN_CNF_CLR          	(0x00000000U)       /**< CLR register value for CCN.*/
#define IMPDRV_VDSP_REGVAL_PDMA_BUSY    ((uint32_t)1U << 29U)   /**< Hardware version valuer for VDSP */
#define IMPDRV_REGVAL_VDSP_GPIO_STDBY   (0x00000001U)       /**< GPIO_OUT register standby value for VDSP */
#define IMPDRV_VDSP_CHANNEL_OPE_BITS    (0x0FU)             /**< Channel operate bits */
#define IMPDRV_VDSP_CB_CODE_TRAP        (0)                 /**< Callback code TRAP */

/* define macros for MSCO callback */
#define OFS_MAX_VAL_THRES				(0x228u)
#define OFS_MIN_VAL_THRES				(0x22Cu)
#define OFS_MAXSUM_CNT_THRES			(0x230u)
#define OFS_MINSUM_CNT_THRES			(0x234u)
#define OFS_MAXSUM_CNT					(0x220u)
#define OFS_MINSUM_CNT					(0x224u)

/* Physical address of SPMI0 */
#define OFFSETADDR_SPMI0				(0xED300000U)

/* Area size of SPMI0 */
#define SIZE_SPMI0						(0x00100000U)

/* Convert at INTEN register value. */
uint32_t imp_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);

uint32_t ocv_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);

uint32_t dmac_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);

uint32_t dmacs_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);

uint32_t psc_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);

uint32_t cnn_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
);
