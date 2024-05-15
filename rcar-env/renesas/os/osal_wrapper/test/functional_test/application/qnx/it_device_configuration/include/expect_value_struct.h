/* Table 3{NULL}33 Device Configuration of Interrupt */
typedef struct interrupt_struct
{
    int interrupt_spi_number;
}interrupt_struct_t;

/* Table 3{NULL}35 Device Configuration of AXI Bus control */
typedef struct assignment_struct
{
    int utlbs;
}assignment_struct_t;

typedef struct power_struct
{
    e_pma_hwa_id_t          pma_id;
    e_osal_pm_postckcr_t    postckcr;
}power_struct_t;

/* Table 3{NULL}32 Device Configuration of IO access */
struct expect_config
{
    const char              *id;
    const char              *type;
    uint64_t                address;
    uint64_t                range;
    size_t                  number_of_interrupts;
    size_t                  number_of_int;
    interrupt_struct_t      *st_int_info;
    const char              *axi_bus_name;
    size_t                  num_of_utlb;
    assignment_struct_t     *st_axi_info;
    power_struct_t          *st_pm_info;
}expect_config_t;