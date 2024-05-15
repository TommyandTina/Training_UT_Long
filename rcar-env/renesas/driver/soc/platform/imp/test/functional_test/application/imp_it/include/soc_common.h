#include "soc_util.h"

#define SYNC_MAX_NUM             (6)
#define CMD_LESS_THAN_6_CORES    (0)
#define CMD_6_CORES              (1)

#define ARRAY_SIZE(arr)		(sizeof(arr)/sizeof(arr[0]))

#define list_element(__num, __type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, __type5, __num5, __type6, __num6) \
	{__num, \
	 {{IMPDRV_CORE_TYPE_##__type1, __num1}, \
	  {IMPDRV_CORE_TYPE_##__type2, __num2}, \
	  {IMPDRV_CORE_TYPE_##__type3, __num3}, \
	  {IMPDRV_CORE_TYPE_##__type4, __num4}, \
	  {IMPDRV_CORE_TYPE_##__type5, __num5}, \
	  {IMPDRV_CORE_TYPE_##__type6, __num6}} \
	}

#define list_1element(__type1, __num1) \
	list_element(1, __type1, __num1, INVALID, 0, INVALID, 0, INVALID, 0, INVALID, 0, INVALID, 0) \

#define list_2element(__type1, __num1, __type2, __num2) \
	list_element(2, __type1, __num1, __type2, __num2, INVALID, 0, INVALID, 0, INVALID, 0, INVALID, 0)

#define list_3element(__type1, __num1, __type2, __num2, __type3, __num3) \
	list_element(3, __type1, __num1, __type2, __num2, __type3, __num3, INVALID, 0, INVALID, 0, INVALID, 0) \

#define list_4element(__type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4) \
	list_element(4, __type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, INVALID, 0, INVALID, 0) \

#define list_5element(__type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, __type5, __num5) \
	list_element(5, __type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, __type5, __num5, INVALID, 0) \

#define list_6element(__type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, __type5, __num5, __type6, __num6) \
	list_element(6, __type1, __num1, __type2, __num2, __type3, __num3, __type4, __num4, __type5, __num5, __type6, __num6) \

#define list_endelement(__dummy) \
	list_element(0, INVALID, 0,	INVALID, 0,	INVALID, 0, INVALID, 0,	INVALID, 0,	INVALID, 0)

typedef enum {
    IT_IMPDRV_CMD_PLATFORM_SOC  = 0,
    IT_IMPDRV_CMD_INVALID_CORES,
    IT_IMPDRV_CMD_COREMAP,
    IT_IMPDRV_CMD_NO_EXEC,
    IT_IMPDRV_CMD_INSTANCES,
    IT_IMPDRV_CMD_BUSIF,
    IT_IMPDRV_CMD_NO_EXEC_BUSIF,
    IT_IMPDRV_CMD_BUSIF_TEST_PATTERN,
    IT_IMPDRV_CMD_DMA_TRANSFER,
    IT_IMPDRV_CMD_CONF_REG_CHECK,
    IT_IMPDRV_CMD_MODULE_STOP_CHECK,
    IT_IMPDRV_CMD_PLATFORM_SOC_NO_DMA
} tp_cmd_t;

typedef struct {
    uint32_t offset;
    uint32_t value;
} reg_valid_value_t;

typedef struct {
    uint32_t    type;
    bool        value;
} irq_mask_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_inten_tbl_t
 * Definition for bit allocation conversion of the Interrupt enable register
***********************************************************************************************************************/
typedef struct {
    e_impdrv_core_type_t        core_type;              /**< IMP Core type */
    uint32_t                    core_num;               /**< Core number in the core type */
    uint32_t                    bit;                    /**< Bit value to convert */
} st_impdrv_inten_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_group_tbl_t
 * Definition for bit allocation conversion of the Interrupt group register
***********************************************************************************************************************/
typedef struct {
    uint32_t                    group_bit;              /**< Bit value to convert */
    uint32_t                    group_reg;              /**< Offset address value to convert */
} st_impdrv_group_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_inten_reg_table_t
 * Definition for conversion of register table
***********************************************************************************************************************/
typedef struct {
    const st_impdrv_group_tbl_t     *p_inten_group_table;   /**< Convert Int group register table */
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;     /**< Convert Int enable register table */
    uint32_t                        cnv_table_num;          /**< Number of convert Int enable register table */
} st_impdrv_inten_reg_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_dma_write_info_t
 * Multi-bank memory init information
***********************************************************************************************************************/
typedef struct {
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_dma_write_info_tbl_t;

typedef struct {
	int32_t num;
	st_impdrv_core_info_t core[SYNC_MAX_NUM]
} sync_core_t;

typedef struct {
    uint32_t size;
    uint32_t num;
    st_impdrv_core_info_t *core_info
} tp_core_list_t;

typedef struct
{
    uint32_t                  offset;           /**< Offset to get to register */
    uint32_t                  val;              /**< Write values to registers */
    bool                      read_back;        /**< Whether to read back */
    uint32_t                  chk_val;          /**< Value to read back register */
} st_impdrv_imp_write_info_tbl_t;

typedef struct {
    uint32_t range;
    void *info
} tp_arr_info_t;

typedef struct {
    uint32_t                pattern;
    size_t                  size;
} tp_test_pattern_t;

extern const uint32_t read_value_for_init[READ_NUM_FOR_INIT];
extern const uint32_t read_value_for_start[READ_NUM_FOR_START];
extern const uint32_t s_impdrv_inten_addr[IMPDRV_INSTANCETABLE_NUM];
extern const irq_mask_t irq_mask_by_attr_init_validator[TOTAL_IRQ_MASK_CORES][5];
extern const uint32_t param_by_attr_init_validator[10][IMPDRV_EXEPARAM_MAX];
extern const st_impdrv_irq_group_t g_p_irq_param[5];
extern const st_impdrv_inten_reg_tbl_t s_impdrv_inten_reg_tbl[IMPDRV_INSTANCETABLE_NUM];
#if (defined (V3M2) || defined (V3H1) || defined (V3H2))
extern const st_impdrv_dma_write_info_tbl_t s_mb_init_info1[IMPDRV_DMA_MB_INFO_SIZE1];
extern const st_impdrv_dma_write_info_tbl_t s_mb_init_info2[IMPDRV_DMA_MB_INFO_SIZE2];
extern const st_impdrv_dma_write_info_tbl_t s_mb_init_info3[IMPDRV_DMA_MB_INFO_SIZE3];
#endif
extern const st_impdrv_inten_tbl_t s_impdrv_inten_cnv_table[IMPDRV_INTEN_CORE_MAX];
extern bool close_device_io_flg;

tp_core_list_t get_core_list(
    tp_cmd_t cmd,
    int8_t sub_cmd
);

int8_t get_correct_hw_info(
	const st_impdrv_hwrsc_def_t	*const p_hwrsc_def_table,
	const char					*const p_device_id,
	st_impdrv_reg_info_t		*const p_reg_info
);

e_impdrv_errorcode_t util_get_group(
    e_impdrv_irq_group_t            *const p_irq_group,
    const e_impdrv_instance_t       instance_num
);

void cnv_int_core_bit(
    const st_impdrv_core_info_t     *const p_core_info,
    const uint32_t                  use_core_num,
    const st_impdrv_inten_tbl_t     *const p_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    uint32_t                        *const p_val
);

uint32_t get_irq_mask_val(
    const e_impdrv_core_type_t core_type
);

void get_syncc_val(
    const st_impdrv_corectl_t   *const p_corectl,
    uint32_t                    syncc_val[IMPDRV_SYNCC_REGNUM]
);

sync_core_t *get_sync_cores(
    int32_t cmd
);

tp_arr_info_t get_data_info(
	const e_impdrv_core_type_t	core_type,
	const tp_cmd_t				cmd
);

extern void prepare_test_for_init(
    st_impdrv_initdata_t *p_cmn_init_data
);

extern void prepare_test_for_execute(
    impdrv_ctrl_handle_t        handle,
    const st_impdrv_core_info_t *const p_core_info,
    const uint32_t              claddr_phys
);