/***********************************************************************************************************************
* Copyright [2022-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_dev_cfg_io.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Device Configurator IO Related Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Private data types
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef st_osal_inner_thread_control_t
 * @brief structure to utlb list control.
 **********************************************************************************************************************/
typedef struct 
{
    int32_t     current_utlb;
    size_t      match_count;
} st_osal_utlb_list_control_t;

/*======================================================================================================================
 Public extern variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gp_osal_device_control
 * @brief The value for the user to operate the device without being aware of the device information.
***********************************************************************************************************************/
st_osal_device_control_t *gp_osal_device_control = NULL;

/*******************************************************************************************************************//**
 * @var g_osal_num_of_device_table
 * @brief Number of the device control information table.
***********************************************************************************************************************/
size_t g_osal_num_of_device_table = 0;

/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_io_is_initialized_enu
 * @brief IO Manager initial state flag.
***********************************************************************************************************************/
OSAL_STATIC e_osal_init_status_t gs_osal_io_is_initialized_enu = OSAL_MANAGER_STATE_UNINITIALIZED;

/*******************************************************************************************************************//**
 * @var gs_osal_axi_bus_control
 * @brief The value for the user to operate the device without being aware of the AXI bus information.
***********************************************************************************************************************/
OSAL_STATIC st_osal_axi_bus_control_t gs_osal_axi_bus_control;

/*******************************************************************************************************************//**
 * @var gsp_axi_bus_name_info
 * @brief AXI Bus name information table.
***********************************************************************************************************************/
OSAL_STATIC st_osal_axi_bus_name_info_t *gsp_axi_bus_name_info = NULL;

/*******************************************************************************************************************//**
 * @var gs_axi_bus_name_info_array_size
 * @brief Size of the AXI Bus name information table.
***********************************************************************************************************************/
OSAL_STATIC size_t gs_axi_bus_name_info_array_size = 0;

/*======================================================================================================================
 Private function prototypes
======================================================================================================================*/
/*******************************************************************************************************************//**
* @brief            Initialize the AXI Bus name list.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_axibus_namelist(void);

/*******************************************************************************************************************//**
* @brief            Initialize the AXI Bus name information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_axibus_info(void);

/*******************************************************************************************************************//**
* @brief            Initialize device information.
* @param[in]        mutex_attr      The mutex attribute handle
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_device_information(osal_os_mutexattr_t mutex_attr);

/*******************************************************************************************************************//**
* @brief            Register the AXI Bus name list.
* @param[in,out]    p_device_control    The pointer to st_osal_device_control_t
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_namelist(st_osal_device_control_t *p_device_control);

/*******************************************************************************************************************//**
* @brief            Register the AXI Bus name information.
* @param[in]        p_device_control    The pointer to st_osal_device_control_t
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info(const st_osal_device_control_t *p_device_control);

/*******************************************************************************************************************//**
* @brief            Register the AXI Bus name information process.
* @param[in]        p_bus_domain        The pointer of Bus domain
* @param[in]        p_device_name       The pointer of Device name
* @param[in]        utlb                The uTLB number
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info_proc(const char* const p_bus_domain,
                                                                     const char* const p_device_name,
                                                                     int32_t utlb);

/*******************************************************************************************************************//**
* @brief            Connecting device names.
* @param[in]        p_device_name       The pointer of Device name
* @param[in]        num                 Destination numbers
* @param[in]        add_dev_name_len    The length of Device name
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_device_name_connect(const char* const p_device_name,
                                                               size_t num,
                                                               size_t add_dev_name_len);

/***********************************************************************************************************************
* @brief            Sort axi bus list.
* @param[in]        array_num         The num of array.
* @param[in,out]    p_axi_bus_list    The pointer of st_osal_axi_bus_list_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_io_sort_axi_bus_list(size_t array_num,
                                                  st_osal_axi_bus_list_t *p_axi_bus_list);

/***********************************************************************************************************************
* @brief            Sort axi bus name information.
* @param[in]        array_num              The num of array.
* @param[in,out]    p_axi_bus_name_info    The pointer of st_osal_axi_bus_name_info_t.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_io_sort_axibus_name_info(size_t array_num,
                                                      st_osal_axi_bus_name_info_t *p_axi_bus_name_info);

/*******************************************************************************************************************//**
* @brief            Register last data with AXI Bus name information
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_MEM
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info_last_data(void);

/*******************************************************************************************************************//**
* @brief            Deinitialize device information.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_deinit_device_information(void);

/*******************************************************************************************************************//**
* @brief            Create device list.
* @param[in]        p_device_control    The pointer of st_osal_device_control_t.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        buffer_size         The value of p_buffer size by byte.
* @param[in]        p_buffer            The buffer address.
* @param[in]        p_total_length      The value of device list total length.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_PAR
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_create_device_list(const st_osal_device_control_t *p_device_control,
                                                              const char *p_device_type_name,
                                                              size_t buffer_size,
                                                              char *p_buffer,
                                                              size_t *p_total_length);

/*******************************************************************************************************************//**
* @brief            Calculate device list length.
* @param[in]        p_device_control    The pointer of st_osal_device_control_t.
* @param[in]        p_device_type_name  The string of device type.
* @param[in]        p_total_length      The value of device list total length.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_calculate_device_list_length(const st_osal_device_control_t *p_device_control,
                                                                        const char *p_device_type_name,
                                                                        size_t *p_total_length);

/*******************************************************************************************************************//**
* @brief            Check the registration of the interrupt handler.
* @param[in]        handle              The device handle.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_FAIL
* @retval           OSAL_RETURN_BUSY
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_interrupt_handler(osal_device_handle_t handle);

/*******************************************************************************************************************//**
* @brief            Check the registration of the interrupt handler.
* @param[in]        num                     The number of AXI Bus name
* @param[in,out]    p_axi_bus_name_length   The length of AXI Bus name
* @param[out]       p_axi_bus_name          The pointer of AXI Bus name
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_io_create_axibus_namelist(size_t num,
                                                       size_t *p_axi_bus_name_length,
                                                       char *p_axi_bus_name);

/*******************************************************************************************************************//**
* @brief            Get domain name from axi_bus_name
* @param[in]        p_axi_bus_name      The string of axi_bus_name
* @param[out]       p_domain_name       Destination pointer for the domain name string
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_get_domain_name(const char* p_axi_bus_name,
                                                           char *p_domain_name);

/*******************************************************************************************************************//**
* @brief            Check axi id list by domain name and utlb number
* @param[in]        p_axi_bus_name      The string of axi_bus_name
* @param[in]        p_domain_name       The string of domain name 
* @param[in,out]    p_list_buffer       uTLB number list
* @param[in]        list_size           uTLB number list size
* @param[out]       p_num_of_axi_ids    Pointer to set the number of available AXI IDs.
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_domainname(const char* p_axi_bus_name,
                                                                          const char* p_domain_name,
                                                                          osal_axi_bus_id_t* p_list_buffer,
                                                                          size_t list_size,
                                                                          size_t * const p_num_of_axi_ids);

/*******************************************************************************************************************//**
* @brief            Add uTLB number to the list
* @param[in]        p_domain_name   domain name string
* @param[in]        p_num_str       uTLB number with delimiter.
* @param[in]        list_size       The value of p_list_buffer size by byte
* @param[in,out]    p_list_buffer   The buffer address
* @param[in,out]    p_utlb_control  uTLB list control
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list(const char* p_domain_name,
                                                         const char *p_num_str,
                                                         size_t list_size,
                                                         osal_axi_bus_id_t* p_list_buffer,
                                                         st_osal_utlb_list_control_t *p_utlb_control);

/*******************************************************************************************************************//**
* @brief            Add uTLB number with the delimiter '_' to the list.
* @param[in]        utlb            uTLB number
* @param[in]        p_domain_name   domain name string
* @param[in]        list_size       The value of p_list_buffer size by byte
* @param[in,out]    p_list_buffer   The buffer address
* @param[in,out]    p_utlb_control  uTLB list control
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list_single(int32_t utlb,
                                                                const char* p_domain_name,
                                                                size_t list_size,
                                                                osal_axi_bus_id_t* p_list_buffer,
                                                                st_osal_utlb_list_control_t *p_utlb_control);

/*******************************************************************************************************************//**
* @brief            Add uTLB number with the delimiter '-' to the list.
* @param[in]        utlb            uTLB number
* @param[in]        p_domain_name   domain name string
* @param[in]        list_size       The value of p_list_buffer size by byte
* @param[in,out]    p_list_buffer   The buffer address
* @param[in,out]    p_utlb_control  uTLB list control
* @return           #e_osal_return_t
* @retval           OSAL_RETURN_OK
* @retval           OSAL_RETURN_UNKNOWN_NAME
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list_multi(int32_t utlb,
                                                               const char* p_domain_name,
                                                               size_t list_size,
                                                               osal_axi_bus_id_t* p_list_buffer,
                                                               st_osal_utlb_list_control_t *p_utlb_control);

/*******************************************************************************************************************//**
* @brief            Create the number of AXI IDs from the device name
* @param[in]        p_device_name           The string of device_name
* @param[in,out]    p_array                 AXI ID list
* @param[in]        array_size              The value of p_array size
* @param[out]       p_num_of_axi_ids        Pointer to set the number of available AXI IDs.
* @param[out]       p_num_of_invalid_ids    Pointer to set the number of invalid.
* @return           void
***********************************************************************************************************************/
OSAL_STATIC void r_osal_dcfg_create_axiid_list(const char* p_device_name,
                                               osal_axi_bus_id_t *p_array,
                                               size_t array_size,
                                               size_t * const p_num_of_axi_ids,
                                               size_t * const p_num_of_invalid_ids);

/*******************************************************************************************************************//**
* @brief            Search for the specified string from the source string.
* @param[in]        p_src_string        Search source string
* @param[in]        p_search_string     The string to search for
* @return           char *
* @retval           A pointer to the located string, or NULL if the string isn't found.
***********************************************************************************************************************/
OSAL_STATIC char * r_osal_dcfg_io_search_strings(const char * p_src_string, 
                                                 const char * p_search_string);

/*******************************************************************************************************************//**
* @brief            Compare function for the argument of R_OSAL_OS_BinarySearch().
* @param[in]        p_key           The key.
* @param[in]        p_search_info   The search information.
* @return           int
* @retval           Returns the number value of the comparison result.
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_dcfg_io_search(const void *p_key, 
                                      const void *p_search_info);


/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_041
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetInitStatus()
***********************************************************************************************************************/
/* PRQA S 9106 1 # This function is intended to be used in common. */
e_osal_init_status_t R_OSAL_DCFG_IoGetInitStatus(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/

    return gs_osal_io_is_initialized_enu;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_012]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_012]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_042
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoSetInitStatus()
***********************************************************************************************************************/
void R_OSAL_DCFG_IoSetInitStatus(e_osal_init_status_t status)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    gs_osal_io_is_initialized_enu = status;

    return;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoSetInitStatus()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_013]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_013]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_043
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoControlInit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlInit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret;
    e_osal_return_t             local_ret;
    size_t                      loop_count;
    size_t                      num_of_device_config;
    osal_device_config_addr_t   device_config_index;
    osal_os_mutexattr_t         mutex_attr;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IoStaticInit(&num_of_device_config, &device_config_index);
    if ((OSAL_RETURN_OK == osal_ret) && (0U != num_of_device_config))
    {
        g_osal_num_of_device_table = num_of_device_config;
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        gp_osal_device_control = (st_osal_device_control_t*)R_OSAL_OS_Malloc(sizeof(st_osal_device_control_t) * g_osal_num_of_device_table);
        if (NULL == gp_osal_device_control)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            for (loop_count = 0U; g_osal_num_of_device_table > loop_count; loop_count++)
            {
                gp_osal_device_control[loop_count].dev_info      = device_config_index[loop_count].p_dev_info;
                gp_osal_device_control[loop_count].base_addr     = device_config_index[loop_count].virtual_addr;
                gp_osal_device_control[loop_count].fd            = device_config_index[loop_count].fd;
                gp_osal_device_control[loop_count].handle_id     = (uint64_t)OSAL_DEV_CFG_INVALID_DEVICE_HANDLE_ID;
                gp_osal_device_control[loop_count].inner_timeout = (osal_milli_sec_t)OSAL_DEV_CFG_INNER_TIMEOUT;
            }
        }

        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_OS_MutexInitAttr(&mutex_attr);
            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = r_osal_dcfg_io_init_axibus_namelist();
                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = r_osal_dcfg_io_init_axibus_info();
                }
                else
                {
                    /* Do nothing */
                }

                if (OSAL_RETURN_OK == osal_ret)
                {
                    osal_ret = r_osal_dcfg_io_init_device_information(mutex_attr);
                }
                else
                {
                    /* Do nothing */
                }

                local_ret = R_OSAL_OS_MutexDeinitAttr(mutex_attr);
                if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
                {
                    osal_ret = local_ret;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            } /* end of if (OSAL_RETURN_OK == osal_ret) */

        }
        else
        {
            /* Do nothing */
        }   /* end of if (OSAL_RETURN_OK == osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if ((OSAL_RETURN_OK == osal_ret) && (0U != num_of_device_config)). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoControlInit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_044
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoControlDeinit()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoControlDeinit(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = R_OSAL_OS_IoStaticDeinit();
    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != gs_osal_axi_bus_control.axi_bus_list)
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            R_OSAL_OS_Free((void*)gs_osal_axi_bus_control.axi_bus_list);
            gs_osal_axi_bus_control.axi_bus_list = NULL;
        }
        else
        {
            /* Do nothing */
        }

        if (NULL != gsp_axi_bus_name_info)
        {
            for (loop_count = 0U; gs_axi_bus_name_info_array_size > loop_count; loop_count++)
            {
                if (NULL != gsp_axi_bus_name_info[loop_count].dev_names)
                {
                    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                    R_OSAL_OS_Free((void*)gsp_axi_bus_name_info[loop_count].dev_names);
                    gsp_axi_bus_name_info[loop_count].dev_names = NULL;
                }
                else
                {
                    /* Do nothing */
                }
            }
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            R_OSAL_OS_Free((void*)gsp_axi_bus_name_info);
            gsp_axi_bus_name_info = NULL;
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != gsp_axi_bus_name_info). */

        if (NULL != gp_osal_device_control)
        {
            osal_ret = r_osal_dcfg_io_deinit_device_information();
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != gp_osal_device_control). */
        g_osal_num_of_device_table = 0;
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoControlDeinit()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_015]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_015]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_COM_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_045
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoCheckBusy()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoCheckBusy(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                osal_ret = OSAL_RETURN_OK;
    e_osal_return_t                local_ret;
    size_t                         loop_count = 0U;
    const st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((g_osal_num_of_device_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &gp_osal_device_control[loop_count];

        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_device_control->inner_mtx, p_device_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (false != p_device_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_device_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }
        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoCheckBusy()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_016]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_016]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_046
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfDevices()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfDevices(const char* p_deviceType,
                                              size_t* const p_numOfDevices)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret        = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    size_t                          device_name_length;
    size_t                          loop_count      = 0U;
    size_t                          device_count    = 0U;
    const st_osal_device_control_t  *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    device_name_length = R_OSAL_OS_Strnlen(p_deviceType, (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH + 1U);
    if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < device_name_length)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        while (g_osal_num_of_device_table > loop_count)
        {
            p_device_control = &gp_osal_device_control[loop_count];

            os_ret = strncmp(p_deviceType,
                             p_device_control->dev_info->type,
                             (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
            if (0 == os_ret)
            {
                device_count++;
            }
            else
            {
                /* Do nothing */
            }

            loop_count++;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_numOfDevices = device_count;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfDevices()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_017]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_017]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_047
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetDeviceList()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetDeviceList(const char * p_device_type_name,
                                            char * p_buffer,
                                            size_t buffer_size,
                                            size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret           = OSAL_RETURN_OK;
    size_t                          device_name_length;
    size_t                          loop_count         = 0U;
    size_t                          total_length       = 0U;
    const st_osal_device_control_t  *p_device_control;
    const char                      eos[2]             = "\0";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    device_name_length = R_OSAL_OS_Strnlen(p_device_type_name, (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH + 1U);
    if (OSAL_DEVICE_TYPE_NAME_MAX_LENGTH < device_name_length)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        if (NULL != p_buffer)
        {
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            (void)memset((void*)p_buffer, 0x00, buffer_size);
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        while ((g_osal_num_of_device_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            p_device_control = &gp_osal_device_control[loop_count];

            if (NULL != p_buffer)
            {
                osal_ret = r_osal_dcfg_io_create_device_list(p_device_control,
                                                             p_device_type_name,
                                                             buffer_size,
                                                             p_buffer,
                                                             &total_length);
            }
            else    /* For OSAL_RETURN_OK_RQST, only the length is calculated. */
            {
                osal_ret = r_osal_dcfg_io_calculate_device_list_length(p_device_control,
                                                                       p_device_type_name,
                                                                       &total_length);
            }

            loop_count++;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    if ((OSAL_RETURN_OK == osal_ret) && (NULL != p_buffer))
    {
        /* p_buffer is not NULL, because it is not OSAL_RETURN_OK_RQST */
        p_buffer[total_length - (size_t)1U] = eos[0];
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (0U == total_length)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            *p_num_of_byte = total_length;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetDeviceList()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_018]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_048
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoDeviceOpen()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceOpen(const char * p_id,
                                         osal_device_handle_t *const p_handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t          osal_ret;
    e_osal_return_t          local_ret;
    st_osal_device_control_t *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0314 3 # No problem because it follows the specification of the calling function. */
    /* PRQA S 0316 1 # No problem because it is returned to the type of the second argument. */
    p_device_control = (st_osal_device_control_t*)R_OSAL_OS_BinarySearch((const void*)p_id,
                                                                         (const void*)gp_osal_device_control,
                                                                         g_osal_num_of_device_table,
                                                                         sizeof(st_osal_device_control_t),
                                                                         r_osal_dcfg_io_search);
    if (NULL == p_device_control)
    {
        osal_ret = OSAL_RETURN_ID;
    }
    else
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(p_device_control->inner_mtx, p_device_control->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            if (OSAL_MANAGER_STATE_UNINITIALIZED == gs_osal_io_is_initialized_enu)
            {
                osal_ret = OSAL_RETURN_STATE;
            }
            else if (false != p_device_control->active)
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                p_device_control->active    = true;
                p_device_control->handle_id = (uint64_t)OSAL_DEV_CFG_DEVICE_HANDLE_ID;
            }

            local_ret = R_OSAL_OS_MutexUnlock(p_device_control->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if(OSAL_RETURN_OK != osal_ret). */
    }   /* end of if(NULL == p_device_control). */

    if (OSAL_RETURN_OK != osal_ret)
    {
        if ((OSAL_RETURN_BUSY != osal_ret) && (NULL != p_handle))
        {
            *p_handle = OSAL_DEVICE_HANDLE_INVALID;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
       *p_handle = p_device_control;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoDeviceOpen()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_019]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_019]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_049
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoDeviceClose()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoDeviceClose(osal_device_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    e_osal_return_t local_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) || (false == handle->active))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = R_OSAL_HIF_MutexLockForTimePeriod(handle->inner_mtx, handle->inner_timeout);
        if (OSAL_RETURN_OK != osal_ret)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            osal_ret = r_osal_dcfg_io_check_interrupt_handler(handle);

            if ((OSAL_RETURN_OK == osal_ret) && (true == handle->pm_wait_state))
            {
                osal_ret = OSAL_RETURN_BUSY;
            }
            else
            {
                /* Do nothing */
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                handle->active    = false;
                handle->handle_id = (uint64_t)OSAL_DEV_CFG_INVALID_DEVICE_HANDLE_ID;
            }
            else
            {
                /* Do nothing */
            }

            local_ret = R_OSAL_OS_MutexUnlock(handle->inner_mtx);
            if ((OSAL_RETURN_OK != local_ret) && (OSAL_RETURN_OK == osal_ret))
            {
                osal_ret = OSAL_RETURN_FAIL;
            }
            else
            {
                /* Do nothing */
            }
        }   /* end of if (OSAL_RETURN_OK != osal_ret). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(OSAL_RETURN_OK == osal_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoDeviceClose()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_020]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_050
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoRead()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoRead(osal_device_handle_t const handle,
                                   uintptr_t offset,
                                   size_t size,
                                   size_t unit_size,
                                   void* p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) || (false == handle->active))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    else if (((uintptr_t)NULL == handle->base_addr) ||
             (NULL == handle->dev_info) ||
             (0U != (handle->base_addr & (unit_size - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if ((offset >= handle->dev_info->range) ||
             (handle->dev_info->range < size) ||
             ((handle->dev_info->range - offset) < size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = R_OSAL_OS_IoRead(handle, offset, size, unit_size, p_data);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoRead()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_021]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_021]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_051
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoWrite()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoWrite(osal_device_handle_t const handle,
                                    uintptr_t offset,
                                    size_t size,
                                    size_t unit_size,
                                    const void* p_data)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 6 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != handle->handle_id) || (false == handle->active))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 0326 1 # Cast between a pointer to void and uintptr_t is possible. */
    else if (((uintptr_t)NULL == handle->base_addr) ||
             (NULL == handle->dev_info) ||
             (0U != (handle->base_addr & (unit_size - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else if ((offset >= handle->dev_info->range) ||
             (handle->dev_info->range < size) ||
             ((handle->dev_info->range - offset) < size))
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        osal_ret = R_OSAL_OS_IoWrite(handle, offset, size, unit_size, p_data);
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)handle < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)handle > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)handle & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoWrite()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_022]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_022]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_052
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetDeviceAxiBusId()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_IoGetDeviceAxiBusId(osal_device_handle_t device,
                                                osal_axi_bus_id_t *const p_axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    char const      *p_string;
    size_t          count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)device < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)device > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)device & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != device->handle_id) || (false == device->active) || (NULL == device->dev_info))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        count    = 0U;
        while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb)
        {
            p_string = r_osal_dcfg_io_search_strings(gsp_axi_bus_name_info[count].dev_names, device->dev_info->id);
            if (NULL != p_string)
            {
                *p_axi_id = count;
                break;
            }
            else
            {
                /* Do nothing */
            }
            count++;
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)device < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)device > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)device & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetDeviceAxiBusId()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_023]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_023]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_053
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName(const char* p_device_name,
                                                        osal_axi_bus_id_t* const p_axi_id)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    char const      *p_string;
    size_t          count = 0U;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb)
    {
        p_string = r_osal_dcfg_io_search_strings(gsp_axi_bus_name_info[count].dev_names, p_device_name);
        if (NULL != p_string)
        {
            *p_axi_id = count;
            break;
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != qnx_ret) */
        count++;
    }

    if (count == (gs_axi_bus_name_info_array_size - 1U))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_024]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_024]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_054
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusNameList()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusNameList(char * p_buffer,
                                                size_t buffer_size,
                                                size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          loop_count;
    size_t          axi_bus_name_length;
    size_t          total_length        = 0U;
    char            tmp_axi_bus_name[OSAL_DEV_CFG_DOMAIN_UTLB_MAX_STRINGS] = {0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 0U; loop_count < gs_osal_axi_bus_control.num; loop_count++)
    {
        axi_bus_name_length = 0U;
        r_osal_dcfg_io_create_axibus_namelist(loop_count, &axi_bus_name_length, tmp_axi_bus_name);

        if (NULL != p_buffer)
        {
            if ((total_length + axi_bus_name_length) <= buffer_size)
            {
                /* PRQA S 0315 2 # There is no problem because it is copying to the same type. */
                /* Use memcpy for data including strings and '\0'. */
                (void)memcpy(&p_buffer[total_length], tmp_axi_bus_name, axi_bus_name_length);
                total_length += axi_bus_name_length;
            }
            else /* ((total_length + axi_bus_name_length) > buffer_size). */
            {
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                (void)memset((void*)p_buffer, 0x00, buffer_size);
                total_length = 0U;
                osal_ret = OSAL_RETURN_PAR;
                break;
            }
        }
        else
        {
            total_length += axi_bus_name_length;
        }   /* end of if (NULL != p_buffer). */
    }   /* end of for (loop_count = 0U; loop_count < gs_osal_axi_bus_control.num; loop_count++). */

    *p_num_of_byte = total_length;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusNameList()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_025]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_025]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_055
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiBusName(osal_axi_bus_id_t axi_id,
                                            char * p_buffer,
                                            size_t buffer_size,
                                            size_t *const p_num_of_byte)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret = OSAL_RETURN_OK;
    size_t                    axi_bus_name_length;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (axi_id >= gs_axi_bus_name_info_array_size)
    {
        osal_ret = OSAL_RETURN_PAR;
    }
    else
    {
        axi_bus_name_length = R_OSAL_OS_Strnlen(gsp_axi_bus_name_info[axi_id].domain_name, OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
        if (0U == axi_bus_name_length)
        {
            osal_ret = OSAL_RETURN_PAR;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if (NULL != p_buffer)
        {
            if ((axi_bus_name_length + (size_t)4U) <= buffer_size)
            {
                (void)strncpy(p_buffer, gsp_axi_bus_name_info[axi_id].domain_name, buffer_size);

                if ((0U != axi_id) && (1U != axi_id)) /* if domain_name is "invalid" or "mm(ipa), does not bind utlb. */
                {
                    p_buffer[axi_bus_name_length]              = '_';
                    p_buffer[axi_bus_name_length + (size_t)1U] = (char)('0' + (gsp_axi_bus_name_info[axi_id].utlb / 10));
                    p_buffer[axi_bus_name_length + (size_t)2U] = (char)('0' + (gsp_axi_bus_name_info[axi_id].utlb % 10));
                }
                else
                {
                    /* Do nothing */
                }
                p_buffer[axi_bus_name_length + (size_t)3U] = '\0';
            }
            else /* ((axi_bus_name_length + (size_t)4U) > buffer_size). */
            {
                osal_ret = OSAL_RETURN_PAR;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_buffer). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_byte = axi_bus_name_length + (size_t)4;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_026]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_056
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName(const char* p_axi_bus_name,
                                                          size_t *p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    char            domain_name[OSAL_DEV_CFG_BUS_DOMAIN_SIZE] = "";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_dcfg_io_get_domain_name(p_axi_bus_name, domain_name);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_domainname(p_axi_bus_name, domain_name, NULL, 0, p_num_of_axi_ids);
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_001]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_057
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForAxiBusName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForAxiBusName(const char* p_axi_bus_name,
                                                        osal_axi_bus_id_t* p_array,
                                                        size_t array_size,
                                                        size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          numofaxiids                       = 0;
    char            domain_name[OSAL_DEV_CFG_BUS_DOMAIN_SIZE] = "";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    osal_ret = r_osal_dcfg_io_get_domain_name(p_axi_bus_name, domain_name);
    if (OSAL_RETURN_OK == osal_ret)
    {
        osal_ret = r_osal_dcfg_io_check_axiidlist_for_domainname(p_axi_bus_name, domain_name, p_array, array_size, &numofaxiids);
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            (void)memset((void*)p_array, 0x00, array_size);
        }
        else
        {
            if (NULL == p_array)  /* Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Do nothing */
            }
            *p_num_of_written_axi_ids = numofaxiids;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForAxiBusName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_002]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_002]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_058
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl(osal_device_handle_t device_hdl,
                                                         size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)device_hdl < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)device_hdl > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)device_hdl & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != device_hdl->handle_id) || (false == device_hdl->active) || (NULL == device_hdl->dev_info))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        osal_ret = OSAL_RETURN_OK;
        if (NULL == device_hdl->dev_info->utlb_assignment)
        {
            *p_num_of_axi_ids = 0U;
        }
        else
        {
            *p_num_of_axi_ids = device_hdl->dev_info->utlb_assignment->table_size;
        }
        /* PRQA S 2053 3 #  Comments for coding rule EST3.1 compliance. */
    }   /* end of if (((uintptr_t)device_hdl < (uintptr_t)&gp_osal_device_control[0]) ||
                      ((uintptr_t)device_hdl > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
                      (0U != ((uintptr_t)device_hdl & (sizeof(uintptr_t) - 1U)))). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_003]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_003]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_059
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl()
***********************************************************************************************************************/
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl(osal_device_handle_t device_hdl,
                                                       osal_axi_bus_id_t* p_array,
                                                       size_t array_size,
                                                       size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    size_t          numofaxiids;
    size_t          invalid_axiids;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0306 3 # Cast between a pointer and uintptr_t is possible. */
    if (((uintptr_t)device_hdl < (uintptr_t)&gp_osal_device_control[0]) ||
        ((uintptr_t)device_hdl > (uintptr_t)&gp_osal_device_control[(g_osal_num_of_device_table - (size_t)1)]) ||
        (0U != ((uintptr_t)device_hdl & (sizeof(uintptr_t) - 1U))))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    /* PRQA S 2995,2991,2880 7 # QAC false indication (CCT change). */
    else if ((OSAL_DEV_CFG_DEVICE_HANDLE_ID != device_hdl->handle_id) || (false == device_hdl->active) || (NULL == device_hdl->dev_info))
    {
        osal_ret = OSAL_RETURN_HANDLE;
    }
    else
    {
        r_osal_dcfg_create_axiid_list(device_hdl->dev_info->id, p_array, array_size, &numofaxiids, &invalid_axiids);

        if ((0U == numofaxiids) && (0U == invalid_axiids))
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            osal_ret = OSAL_RETURN_OK;
        }
    }

    /* PRQA S 2996,2992,2880 3 # QAC false indication (CCT change). */
    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            (void)memset((void*)p_array, 0x00, array_size);
        }
        else
        {
            if (NULL == p_array)  /* Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Do nothing */
            }

            *p_num_of_written_axi_ids = numofaxiids;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_060
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName(const char* p_device_name,
                                                          size_t* const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t                 osal_ret   = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                             os_ret;
    size_t                          loop_count = 0;
    bool                            find_flg   = false;
    const st_osal_device_control_t  *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (loop_count < g_osal_num_of_device_table)
    {
        p_device_control = &gp_osal_device_control[loop_count];

        os_ret = strncmp(p_device_name,
                         p_device_control->dev_info->id,
                         (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
        if (0 == os_ret)
        {
            find_flg = true;
            if (NULL == p_device_control->dev_info->utlb_assignment)
            {
                *p_num_of_axi_ids = 0;
            }
            else
            {
                *p_num_of_axi_ids = p_device_control->dev_info->utlb_assignment->table_size;
            }
            break;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }   /* end of while(loop_count < g_osal_num_of_device_table). */

    if (false == find_flg)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_005]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_005]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_061
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_DCFG_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_IoGetAxiIdListForDeviceName(const char* p_device_name,
                                                        osal_axi_bus_id_t* p_array,
                                                        size_t array_size,
                                                        size_t* const p_num_of_written_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret        = OSAL_RETURN_OK;
    size_t          numofaxiids     = 0;
    size_t          invalid_axiids  = 0;
    size_t          loop_count      = 0;
    char const      *p_string;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[loop_count].utlb)
    {
        p_string = r_osal_dcfg_io_search_strings(gsp_axi_bus_name_info[loop_count].dev_names, p_device_name);
        if (NULL != p_string)
        {
            if (OSAL_DEV_CFG_INVALID_UTLB != gsp_axi_bus_name_info[loop_count].utlb)
            {
                if ((NULL != p_array) && (array_size > numofaxiids))
                {
                    p_array[numofaxiids] = loop_count;
                }
                else
                {
                    /* Do nothing */
                }
                numofaxiids++;
            }
            else
            {
                invalid_axiids++;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_string). */
        loop_count++;
    }   /* end of while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[loop_count].utlb). */

    if ((0U == numofaxiids) && (0U == invalid_axiids))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        /* Do nothing */
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        if ((NULL != p_array) && (array_size < numofaxiids))
        {
            osal_ret = OSAL_RETURN_PAR;
            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            (void)memset((void*)p_array, 0x00, array_size);
        }
        else
        {
            if (NULL == p_array)  /* Already checked && (0U == array_size) */
            {
                osal_ret = OSAL_RETURN_OK_RQST;
            }
            else
            {
                /* Do nothing */
            }

            *p_num_of_written_axi_ids = numofaxiids;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (OSAL_RETURN_OK == osal_ret) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function R_OSAL_DCFG_IoGetAxiIdListForDeviceName()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_006]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_006]
***********************************************************************************************************************/

/*======================================================================================================================
 Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_065
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_init_axibus_namelist()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoControlInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_axibus_namelist(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret            = OSAL_RETURN_OK;
    const char      str_invalid[8]      = "invalid";
    const char      str_mmipa[8]        = "mm(ipa)";
    const size_t    local_str_size      = sizeof(str_invalid);

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
    gs_osal_axi_bus_control.axi_bus_list = (st_osal_axi_bus_list_t*)R_OSAL_OS_Malloc(
                                                sizeof(st_osal_axi_bus_list_t) * g_osal_num_of_device_table);
    if (NULL == gs_osal_axi_bus_control.axi_bus_list)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* Set fixed data. */
        (void)strncpy(gs_osal_axi_bus_control.axi_bus_list[0].axi_bus_name, str_invalid, local_str_size);
        (void)strncpy(gs_osal_axi_bus_control.axi_bus_list[1].axi_bus_name, str_mmipa,   local_str_size);
        gs_osal_axi_bus_control.num = 2;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_init_axibus_namelist()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_066
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_init_axibus_info()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoControlInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_axibus_info(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret            = OSAL_RETURN_OK;
    const char      str_invalid[8]      = "invalid";
    const char      str_mmipa[8]        = "mm(ipa)";
    const char      str_null[2]         = "\0";
    const size_t    local_str_size      = sizeof(str_invalid);
    const size_t    local_str_null_size = sizeof(str_null);

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
    gsp_axi_bus_name_info = (st_osal_axi_bus_name_info_t*)R_OSAL_OS_Malloc(sizeof(st_osal_axi_bus_name_info_t) * 2U);
    if (NULL == gsp_axi_bus_name_info)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        gsp_axi_bus_name_info[0].dev_names = (char*)R_OSAL_OS_Malloc(local_str_null_size);
        if (NULL == gsp_axi_bus_name_info[0].dev_names)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
            gsp_axi_bus_name_info[1].dev_names = (char*)R_OSAL_OS_Malloc(local_str_null_size);
            if (NULL == gsp_axi_bus_name_info[1].dev_names)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                /* Do nothing */
            }
        }
    }   /* end of if (NULL == gsp_axi_bus_name_info). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        gs_axi_bus_name_info_array_size = 2U;
        /* set invalid */
        (void)strncpy(gsp_axi_bus_name_info[0].domain_name, str_invalid, local_str_size);
        (void)strncpy(gsp_axi_bus_name_info[0].dev_names,   str_null,    local_str_null_size);
        gsp_axi_bus_name_info[0].utlb             = OSAL_DEV_CFG_INVALID_UTLB;
        gsp_axi_bus_name_info[0].dev_names_length = 0U;
        /* set mm(ipa) */
        (void)strncpy(gsp_axi_bus_name_info[1].domain_name, str_mmipa, local_str_size);
        (void)strncpy(gsp_axi_bus_name_info[1].dev_names,   str_null,  local_str_null_size);
        gsp_axi_bus_name_info[1].utlb             = OSAL_DEV_CFG_INVALID_UTLB;
        gsp_axi_bus_name_info[1].dev_names_length = 0U;
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_init_axibus_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_067
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_init_device_information()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoControlInit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_init_device_information(osal_os_mutexattr_t mutex_attr)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret   = OSAL_RETURN_OK;
    size_t                      loop_count = 0U;
    st_osal_device_control_t    *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((g_osal_num_of_device_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &gp_osal_device_control[loop_count];

        osal_ret = R_OSAL_OS_MutexCreate(mutex_attr, &p_device_control->inner_mtx);
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_register_axibus_namelist(p_device_control);
        }
        else
        {
            /* Do nothing */
        }

        if(OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = r_osal_dcfg_io_register_axibus_info(p_device_control);
        }
        else
        {
            /* Do nothing */
        }

        if(OSAL_RETURN_OK == osal_ret)
        {
            p_device_control->active = false;
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }   /* end of while((g_osal_num_of_device_table > loop_count) && (OSAL_RETURN_OK == osal_ret)). */

    if (OSAL_RETURN_OK == osal_ret)
    {
        /* The first and second are fixed, so sort the third and subsequent ones. */
        r_osal_dcfg_io_sort_axi_bus_list((gs_osal_axi_bus_control.num - 2U), &gs_osal_axi_bus_control.axi_bus_list[2]);
        r_osal_dcfg_io_sort_axibus_name_info((gs_axi_bus_name_info_array_size - 2U), &gsp_axi_bus_name_info[2]);

        osal_ret = r_osal_dcfg_io_register_axibus_info_last_data();
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_init_device_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_068
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_register_axibus_namelist()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_init_device_information() in order to improve the result 
   of static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_namelist(st_osal_device_control_t *p_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret     = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          loop_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_device_control->dev_info->axi_bus_name)
    {
        for (loop_count = 0U; loop_count < gs_osal_axi_bus_control.num; loop_count++)
        {
            os_ret = strncmp(gs_osal_axi_bus_control.axi_bus_list[loop_count].axi_bus_name,
                             p_device_control->dev_info->axi_bus_name,
                             (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);

            if (0 == os_ret)
            {
                p_device_control->axi_bus_id = loop_count;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }

        if (loop_count == gs_osal_axi_bus_control.num)
        {
            p_device_control->axi_bus_id = gs_osal_axi_bus_control.num;

            (void)strncpy(gs_osal_axi_bus_control.axi_bus_list[gs_osal_axi_bus_control.num].axi_bus_name,
                          p_device_control->dev_info->axi_bus_name,
                          (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
            gs_osal_axi_bus_control.num++;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if(NULL != p_device_control->dev_info->axi_bus_name). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_register_axibus_namelist()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_062
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_register_axibus_info()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info(const st_osal_device_control_t *p_device_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    uint32_t        table_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (NULL != p_device_control->dev_info->utlb_assignment)
    {
        for (table_count = 0U;
             (table_count < p_device_control->dev_info->utlb_assignment->table_size) && (OSAL_RETURN_OK == osal_ret);
             table_count++)
        {
            osal_ret = r_osal_dcfg_io_register_axibus_info_proc(p_device_control->dev_info->axi_bus_name,
                                                                p_device_control->dev_info->id,
                                                                p_device_control->dev_info->utlb_assignment->table[table_count]);
        }
    }
    else
    {
        osal_ret = r_osal_dcfg_io_register_axibus_info_proc(p_device_control->dev_info->axi_bus_name,
                                                            p_device_control->dev_info->id,
                                                            OSAL_DEV_CFG_INVALID_UTLB);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_register_axibus_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_034]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_034]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_069
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_register_axibus_info_proc()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info_proc(const char* const p_bus_domain,
                                                                     const char* const p_device_name,
                                                                     int32_t utlb)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    uint32_t                    match_flag = 0U;
    size_t                      device_name_len;
    st_osal_axi_bus_name_info_t *p_tmp_table;
    char const                  *p_string;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;
    size_t                      array_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    device_name_len = R_OSAL_OS_Strnlen(p_device_name, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH) + 1U;

    for (array_count = 0U; gs_axi_bus_name_info_array_size > array_count; array_count++)
    {
        os_ret = strncmp(gsp_axi_bus_name_info[array_count].domain_name, p_bus_domain, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
        if ((0 == os_ret) && (gsp_axi_bus_name_info[array_count].utlb == utlb))
        {
            match_flag = 1U;
            p_string = r_osal_dcfg_io_search_strings(gsp_axi_bus_name_info[array_count].dev_names, p_device_name);
            if (NULL == p_string)
            {
                osal_ret = r_osal_dcfg_io_device_name_connect(p_device_name, array_count, device_name_len);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }

    if (0U == match_flag)
    {
        /* Use p_tmp_buffer to avoid memory leak in case realloc fail */
        /* PRQA S 0314 3 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_DCFG_IoControlDeinit(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gsp_axi_bus_name_info,
                         (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL != p_tmp_table)
        {
            gsp_axi_bus_name_info = p_tmp_table;

            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = utlb;
            (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, p_bus_domain, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);

            /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names = (char *)R_OSAL_OS_Malloc(device_name_len + 1U);
            if (NULL == gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names)
            {
                osal_ret = OSAL_RETURN_MEM;
            }
            else
            {
                (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names, p_device_name, device_name_len);
                gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names_length = device_name_len + 1U;
            }

            gs_axi_bus_name_info_array_size += 1U;
        }
        else
        {
            osal_ret = OSAL_RETURN_MEM;
        }   /* end of if (NULL != p_tmp_table) */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U == match_flag) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_register_axibus_info_proc()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_036]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_070
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_device_name_connect()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_register_axibus_info_proc() in order to improve the result 
   of static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_device_name_connect(const char* const p_device_name,
                                                               size_t num,
                                                               size_t add_dev_name_len)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          axi_bus_name_len;
    char            *p_tmp_dev_names;
    char            *p_dummy_tmp_dev_names;
    const char      ch[2]    = "\n";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    axi_bus_name_len = gsp_axi_bus_name_info[num].dev_names_length + add_dev_name_len + 1U;

    /* PRQA S 0314 3 # The 1st parameter must be (void *) and there is no side-effect in this case. */
    /* PRQA S 2706 2 # There is no problem because it will be released at R_OSAL_DCFG_IoControlDeinit(). */
    /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
    p_tmp_dev_names = (char *)realloc((void *)gsp_axi_bus_name_info[num].dev_names, axi_bus_name_len);
    if (NULL == p_tmp_dev_names)
    {
        osal_ret = OSAL_RETURN_MEM;
    }
    else
    {
        /* PRQA S 0316 1 # No problem because it follows the specification of the calling function. */
        p_dummy_tmp_dev_names = (char *)R_OSAL_OS_Malloc(axi_bus_name_len);
        if (NULL == p_dummy_tmp_dev_names)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gsp_axi_bus_name_info[num].dev_names = p_tmp_dev_names;
            (void)strncpy(p_dummy_tmp_dev_names,
                          gsp_axi_bus_name_info[num].dev_names,
                          gsp_axi_bus_name_info[num].dev_names_length);
            (void)strncat(p_dummy_tmp_dev_names, ch, 1U);
            (void)strncat(p_dummy_tmp_dev_names, p_device_name, add_dev_name_len);

            (void)strncpy(gsp_axi_bus_name_info[num].dev_names, p_dummy_tmp_dev_names, axi_bus_name_len);
            gsp_axi_bus_name_info[num].dev_names_length = axi_bus_name_len;

            /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
            R_OSAL_OS_Free((void *)p_dummy_tmp_dev_names);
        }   /* end of if (NULL == p_dummy_tmp_dev_names). */
    }   /* end of if (NULL == p_tmp_dev_names). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_device_name_connect()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_036]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_036]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_071
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_sort_axi_bus_list()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_init_device_information() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC void r_osal_dcfg_io_sort_axi_bus_list(size_t array_num,
                                                  st_osal_axi_bus_list_t *p_axi_bus_list)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                      os_ret;
    st_osal_axi_bus_list_t   tmp;
    size_t                   loop_count;
    size_t                   cmp_count;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 1U; loop_count < array_num; loop_count++)
    {
        (void)strncpy(tmp.axi_bus_name, p_axi_bus_list[loop_count].axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
        for (cmp_count = loop_count; cmp_count > 0U; cmp_count--)
        {
            os_ret = strncmp(p_axi_bus_list[(cmp_count - 1U)].axi_bus_name, tmp.axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
            if (0 < os_ret)
            {
                (void)strncpy(p_axi_bus_list[cmp_count].axi_bus_name, p_axi_bus_list[(cmp_count - 1U)].axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
            }
            else
            {
                break;
            }
        }
        (void)strncpy(p_axi_bus_list[cmp_count].axi_bus_name, tmp.axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
    }

    return;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_sort_axi_bus_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_072
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_sort_axibus_name_info()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_init_device_information() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC void r_osal_dcfg_io_sort_axibus_name_info(size_t array_num,
                                                      st_osal_axi_bus_name_info_t *p_axi_bus_name_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                            os_ret;
    st_osal_axi_bus_name_info_t    tmp;
    size_t                         loop_count;
    size_t                         cmp_count;
    size_t                         reg_count;
    bool                           loop_end;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 1U; loop_count < array_num; loop_count++)
    {
        (void)strncpy(tmp.domain_name, p_axi_bus_name_info[loop_count].domain_name, (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
        tmp.utlb             = p_axi_bus_name_info[loop_count].utlb;
        tmp.dev_names        = p_axi_bus_name_info[loop_count].dev_names;
        tmp.dev_names_length = p_axi_bus_name_info[loop_count].dev_names_length;

        loop_end  = false;
        for (cmp_count = loop_count; ((cmp_count > 0U) && (false == loop_end)); cmp_count--)
        {
            os_ret = strncmp(p_axi_bus_name_info[(cmp_count - 1U)].domain_name, tmp.domain_name, (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
            if (0 == os_ret)
            {
                if (p_axi_bus_name_info[(cmp_count - 1U)].utlb > tmp.utlb)
                {
                    reg_count = cmp_count - 1U;
                    (void)strncpy(p_axi_bus_name_info[cmp_count].domain_name, p_axi_bus_name_info[(cmp_count - 1U)].domain_name, (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
                    p_axi_bus_name_info[cmp_count].utlb             = p_axi_bus_name_info[(cmp_count - 1U)].utlb;
                    p_axi_bus_name_info[cmp_count].dev_names        = p_axi_bus_name_info[(cmp_count - 1U)].dev_names;
                    p_axi_bus_name_info[cmp_count].dev_names_length = p_axi_bus_name_info[(cmp_count - 1U)].dev_names_length;
                }
                else
                {
                    reg_count = cmp_count;
                    loop_end  = true;
                }
            }
            else if (0 < os_ret)
            {
                reg_count = cmp_count - 1U;
                (void)strncpy(p_axi_bus_name_info[cmp_count].domain_name, p_axi_bus_name_info[(cmp_count - 1U)].domain_name, (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
                p_axi_bus_name_info[cmp_count].utlb             = p_axi_bus_name_info[(cmp_count - 1U)].utlb;
                p_axi_bus_name_info[cmp_count].dev_names        = p_axi_bus_name_info[(cmp_count - 1U)].dev_names;
                p_axi_bus_name_info[cmp_count].dev_names_length = p_axi_bus_name_info[(cmp_count - 1U)].dev_names_length;
            }
            else
            {
                reg_count = cmp_count;
                loop_end  = true;
            }   /* end of if (0 == os_ret). */
        }   /* end of for (cmp_count = loop_count; ((cmp_count > 0) && (false == loop_end)); cmp_count--). */

        (void)strncpy(p_axi_bus_name_info[reg_count].domain_name, tmp.domain_name, (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
        p_axi_bus_name_info[reg_count].utlb             = tmp.utlb;
        p_axi_bus_name_info[reg_count].dev_names        = tmp.dev_names;
        p_axi_bus_name_info[reg_count].dev_names_length = tmp.dev_names_length;
    }   /* end of for (loop_count = 0; loop_count < array_num; loop_count++). */

    return;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_sort_axibus_name_info()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_014]
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_007]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_014]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_063
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_register_axibus_info_last_data()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_register_axibus_info_last_data(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret     = OSAL_RETURN_OK;
    st_osal_axi_bus_name_info_t *p_tmp_table;
    uint32_t                    match_flag   = 0U;
    size_t                      array_count;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                         os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for(array_count = 0U; gs_axi_bus_name_info_array_size > array_count; array_count++)
    {
        os_ret = strncmp(gsp_axi_bus_name_info[array_count].domain_name, "", 1U);
        if ((0 == os_ret) && (gsp_axi_bus_name_info[array_count].utlb == OSAL_DEV_CFG_AXI_BUS_NAME_LAST))
        {
            match_flag = 1U;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (0U == match_flag)
    {
        /* Use p_tmp_table to avoid memory leak in case realloc fail */
        /* PRQA S 0314 3 # The 1st parameter must be (void *) and there is no side-effect in this case. */
        /* PRQA S 2707 2 # There is no problem because it will be released at R_OSAL_DCFG_IoControlDeinit(). */
        /* PRQA S 5118 1 # Memory allocation functions are available during initialization. */
        p_tmp_table = (st_osal_axi_bus_name_info_t *)realloc((void*)gsp_axi_bus_name_info,
                        (gs_axi_bus_name_info_array_size + 1U) * sizeof(st_osal_axi_bus_name_info_t));
        if (NULL == p_tmp_table)
        {
            osal_ret = OSAL_RETURN_MEM;
        }
        else
        {
            gsp_axi_bus_name_info = p_tmp_table;

            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].utlb = OSAL_DEV_CFG_AXI_BUS_NAME_LAST;
            (void)strncpy(gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].domain_name, "", 1U);
            gsp_axi_bus_name_info[gs_axi_bus_name_info_array_size].dev_names = NULL;

            gs_axi_bus_name_info_array_size++;
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0U == match_flag) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_register_axibus_info_last_data()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_035]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_035]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_073
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_deinit_device_information()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoControlDeinit() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_deinit_device_information(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret = OSAL_RETURN_OK;
    size_t                      loop_count;
    st_osal_device_control_t    *p_device_control;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    loop_count = 0U;
    while ((g_osal_num_of_device_table > loop_count) && (OSAL_RETURN_OK == osal_ret))
    {
        p_device_control = &gp_osal_device_control[loop_count];

        if (NULL != p_device_control->inner_mtx)
        {
            osal_ret = R_OSAL_OS_MutexDestroy(p_device_control->inner_mtx);
            if (OSAL_RETURN_OK == osal_ret)
            {
                p_device_control->handle_id  = (uint64_t)OSAL_DEV_CFG_INVALID_DEVICE_HANDLE_ID;
                p_device_control->active     = false;
                p_device_control->axi_bus_id = OSAL_AXI_BUS_ID_INVALID;
                p_device_control->inner_mtx  = NULL;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        loop_count++;
    }
    /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
    R_OSAL_OS_Free((void*)gp_osal_device_control);
    gp_osal_device_control = NULL;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_deinit_device_information()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_015]
 * [Covers: XOS3_OSAL_QNX_UD_DD_COM_001]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_015]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_074
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_create_device_list()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoGetDeviceList() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_create_device_list(const st_osal_device_control_t *p_device_control,
                                                              const char *p_device_type_name,
                                                              size_t buffer_size,
                                                              char *p_buffer,
                                                              size_t *p_total_length)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret           = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          device_name_length;
    size_t          local_total_length = *p_total_length;
    const char      ch[2]              = "\n";

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = strncmp(p_device_type_name,
                     p_device_control->dev_info->type,
                     (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
    if (0 == os_ret)
    {
        device_name_length = R_OSAL_OS_Strnlen(p_device_control->dev_info->id,
                                               (size_t)OSAL_DEVICE_NAME_MAX_LENGTH + 1U);
        if (OSAL_DEVICE_NAME_MAX_LENGTH < device_name_length)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            local_total_length += device_name_length;
            if (buffer_size >= (local_total_length + (size_t)1U))
            {
                (void)strncat(p_buffer, p_device_control->dev_info->id, device_name_length);
                p_buffer[local_total_length] = ch[0];
            }
            else
            {
                osal_ret = OSAL_RETURN_PAR;
                /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
                (void)memset((void*)p_buffer, 0x00, buffer_size);
            }
            local_total_length++;

            *p_total_length = local_total_length;
        }   /* end of if (OSAL_DEVICE_NAME_MAX_LENGTH < device_name_length). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if (0 == os_ret). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_create_device_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_018]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_075
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_calculate_device_list_length()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoGetDeviceList() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_calculate_device_list_length(const st_osal_device_control_t *p_device_control,
                                                                        const char *p_device_type_name,
                                                                        size_t *p_total_length)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret           = OSAL_RETURN_OK;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;
    size_t          device_name_length;
    size_t          local_total_length = *p_total_length;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = strncmp(p_device_type_name,
                     p_device_control->dev_info->type,
                     (size_t)OSAL_DEVICE_TYPE_NAME_MAX_LENGTH);
    if (0 == os_ret)
    {
        device_name_length = R_OSAL_OS_Strnlen(p_device_control->dev_info->id,
                                               (size_t)OSAL_DEVICE_NAME_MAX_LENGTH + 1U);
        if (OSAL_DEVICE_NAME_MAX_LENGTH < device_name_length)
        {
            osal_ret = OSAL_RETURN_FAIL;
        }
        else
        {
            local_total_length += device_name_length;
            local_total_length++;

            *p_total_length = local_total_length;
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_calculate_device_list_length()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_018]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_018]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_076
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_check_interrupt_handler()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoDeviceClose() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
/* PRQA S 3673 1 # From the concept of handles, const is unnecessary. */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_interrupt_handler(osal_device_handle_t handle)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    size_t          loop_count;
    bool            result;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (0U != handle->dev_info->irq_channels)
    {
        loop_count = 0U;
        while ((handle->dev_info->irq_channels > loop_count) && (OSAL_RETURN_OK == osal_ret))
        {
            if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX != handle->interrupt_index[loop_count])
            {
                osal_ret = R_OSAL_OS_InterruptIsIsrHandler((uint32_t)handle->interrupt_index[loop_count], &result);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    osal_ret = OSAL_RETURN_FAIL;
                }
                else
                {
                    /* Do nothing */
                }

                if ((OSAL_RETURN_OK == osal_ret) && (false != result))
                {
                    osal_ret = OSAL_RETURN_BUSY;
                }
                else
                {
                    /* Do nothing */
                }
            }
            else
            {
                /* Do nothing */
            }   /* end of if (OSAL_DEV_CFG_INTERRUPT_NONE_INDEX != handle->interrupt_index[loop_count]). */
            loop_count++;
        }   /* end of while((handle->dev_info->irq_channels > loop_count) && (OSAL_RETURN_OK == osal_ret)). */
    }
    else
    {
        /* Do nothing */
    }   /* end of if(0U != handle->dev_info->irq_channels). */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_check_interrupt_handler()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_020]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_020]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_077
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_create_axibus_namelist()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoGetAxiBusNameList() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC void r_osal_dcfg_io_create_axibus_namelist(size_t num,
                                                       size_t *p_axi_bus_name_length,
                                                       char *p_axi_bus_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int         os_ret;
    size_t      cnt;
    size_t      cnt2;
    size_t      utlb_cnt;
    int32_t     utlb_list[64];
    int32_t     tmp_utlb;
    size_t      length;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    (void)strncpy(p_axi_bus_name,
                  gs_osal_axi_bus_control.axi_bus_list[num].axi_bus_name,
                  (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);

    length = R_OSAL_OS_Strnlen(gs_osal_axi_bus_control.axi_bus_list[num].axi_bus_name,
                               (size_t)OSAL_DEV_CFG_BUS_DOMAIN_SIZE);

    cnt      = 0U;
    utlb_cnt = 0U;
    while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[cnt].utlb)
    {
        os_ret = strncmp(gs_osal_axi_bus_control.axi_bus_list[num].axi_bus_name,
                         gsp_axi_bus_name_info[cnt].domain_name,
                         (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
        if (0 == os_ret)
        {
            if (OSAL_DEV_CFG_INVALID_UTLB != gsp_axi_bus_name_info[cnt].utlb)
            {
                utlb_list[utlb_cnt] = gsp_axi_bus_name_info[cnt].utlb;
                utlb_cnt++;
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
        cnt++;
    }   /* end of while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[cnt].utlb). */

    cnt = 0U;
    while (cnt < utlb_cnt)
    {
        p_axi_bus_name[length]             = '_';
        p_axi_bus_name[length + (size_t)1] = (char)('0' + (utlb_list[cnt] / 10));
        p_axi_bus_name[length + (size_t)2] = (char)('0' + (utlb_list[cnt] % 10));
        length += (size_t)3U;

        tmp_utlb = OSAL_DEV_CFG_INVALID_UTLB;
        for (cnt2 = cnt + 1U; cnt2 < utlb_cnt; cnt2++)
        {
            if ((utlb_list[cnt] + 1) == utlb_list[cnt2])
            {
                if ('-' != p_axi_bus_name[length - (size_t)1U])
                {
                    p_axi_bus_name[length] = '-';
                    length++;
                }
                tmp_utlb = utlb_list[cnt2];
                cnt = cnt2;
            }
        }

        if (OSAL_DEV_CFG_INVALID_UTLB != tmp_utlb)
        {
            p_axi_bus_name[length]             = (char)('0' + (tmp_utlb / 10));
            p_axi_bus_name[length + (size_t)1U] = (char)('0' + (tmp_utlb % 10));
            length += (size_t)2U;
        }
        else
        {
            /* Do nothing */
        }

        cnt++;
    }   /* end of while (cnt < utlb_cnt). */

    p_axi_bus_name[length] = '\0';
    length++;
    *p_axi_bus_name_length = length;

    return;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_create_axibus_namelist()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_025]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_026]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_078
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_get_domain_name()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_get_domain_name(const char* p_axi_bus_name,
                                                           char *p_domain_name)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t           osal_ret = OSAL_RETURN_OK;
    size_t                    loop_count;
    size_t                    axi_bus_len;
    bool                      find_flg         = false;
    char                      local_axi_bus_name[OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH + 1U] = "";
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                       os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    for (loop_count = 0; loop_count < gs_osal_axi_bus_control.num; loop_count++)
    {
        /* check for [bus domain and uTLB are connected by "_"] */
        axi_bus_len = R_OSAL_OS_Strnlen(gs_osal_axi_bus_control.axi_bus_list[loop_count].axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
        (void)strncpy(local_axi_bus_name, gs_osal_axi_bus_control.axi_bus_list[loop_count].axi_bus_name, axi_bus_len);

        local_axi_bus_name[axi_bus_len] = '_';
        os_ret = strncmp(p_axi_bus_name, local_axi_bus_name, (axi_bus_len + (size_t)1));
        if (0 == os_ret)
        {
            find_flg = true;
            break;
        }
        else
        {
            /* Do nothing */
        }
        /* PRQA S 0314 1 # No problem because it follows the specification of the calling function. */
        (void)memset((void*)local_axi_bus_name, 0x00, (size_t)(OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH + 1U));
    }

    if (false == find_flg)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;
    }
    else
    {
        (void)strncpy(p_domain_name, gs_osal_axi_bus_control.axi_bus_list[loop_count].axi_bus_name, (size_t)OSAL_DEV_CFG_AXI_BUS_NAME_MAX_LENGTH);
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_get_domain_name()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_037]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_037]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_079
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_check_axiidlist_for_domainname()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_check_axiidlist_for_domainname(const char* p_axi_bus_name,
                                                                          const char* p_domain_name,
                                                                          osal_axi_bus_id_t* p_list_buffer,
                                                                          size_t list_size,
                                                                          size_t * const p_num_of_axi_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t             osal_ret     = OSAL_RETURN_OK;
    size_t                      domain_len;
    size_t                      axi_bus_name_len;
    char                        tmp_str[4]   = "";
    size_t                      offset;
    st_osal_utlb_list_control_t utlb_control = {0};

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    domain_len       = R_OSAL_OS_Strnlen(p_domain_name,  OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
    axi_bus_name_len = R_OSAL_OS_Strnlen(p_axi_bus_name, (size_t)OSAL_DEV_CFG_DOMAIN_UTLB_MAX_STRINGS);
    offset           = domain_len;

    while ((OSAL_RETURN_OK == osal_ret) && (axi_bus_name_len >= (offset + (size_t)3U)))
    {
        /* Extracts uTLB information including delimiters. '_xx' or '-xx'  */
        (void)strncpy(tmp_str, &p_axi_bus_name[offset], (size_t)3U);
        tmp_str[3] = '\0';

        osal_ret = r_osal_dcfg_io_add_utlb_list(p_domain_name, tmp_str, list_size, p_list_buffer, &utlb_control);
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* uTLB one block size */
            offset += (size_t)3U;
        }
        else
        {
            /* Do nothing */
        }
    }

    if (OSAL_RETURN_OK == osal_ret)
    {
        *p_num_of_axi_ids = utlb_control.match_count;

        if (0U == utlb_control.match_count)
        {
            osal_ret = OSAL_RETURN_UNKNOWN_NAME;
        }
        else
        {
            /* Do nothing */
        }
    }
    else
    {
        /* Do nothing */
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_check_axiidlist_for_domainname()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_038]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_080
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_add_utlb_list()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_check_axiidlist_for_domainname() in order to improve the 
   result of static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list(const char* p_domain_name,
                                                         const char *p_num_str,
                                                         size_t list_size,
                                                         osal_axi_bus_id_t* p_list_buffer,
                                                         st_osal_utlb_list_control_t *p_utlb_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret;
    int32_t         num;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    if (('0' > p_num_str[1]) || ('9' < p_num_str[1]) || ('0' > p_num_str[2]) || ('9' < p_num_str[2]))
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0_a4, ir0_6d) */
    }
    else
    {
        num = (int32_t)((p_num_str[1] - '0') * 10) + (p_num_str[2] - '0');

        switch (p_num_str[0])
        {
            case '_':
                osal_ret = r_osal_dcfg_io_add_utlb_list_single(num,
                                                               p_domain_name,
                                                               list_size,
                                                               p_list_buffer,
                                                               p_utlb_control);
                p_utlb_control->current_utlb = num;
                break;

            case '-':
                if ((size_t)0 == p_utlb_control->match_count)
                {
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0-04)*/
                }
                else if (num <= p_utlb_control->current_utlb)
                {
                    osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0_04-02)*/
                }
                else
                {
                    osal_ret = r_osal_dcfg_io_add_utlb_list_multi(num,
                                                                  p_domain_name,
                                                                  list_size,
                                                                  p_list_buffer,
                                                                  p_utlb_control);
                }
                break;

            default:
                osal_ret = OSAL_RETURN_UNKNOWN_NAME;  /* axi_bus_name is invalid (e.g. ir0:04:05)*/
                break;
        }   /* end of switch (p_num_str[0]) */
    }   /* end of if (('0' > p_num_str[1]) || ('9' < p_num_str[1]) || ('0' > p_num_str[2]) || ('9' < p_num_str[2])) */

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_add_utlb_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_038]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_081
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_add_utlb_list_single()
***********************************************************************************************************************/
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list_single(int32_t utlb,
                                                                const char* p_domain_name,
                                                                size_t list_size,
                                                                osal_axi_bus_id_t* p_list_buffer,
                                                                st_osal_utlb_list_control_t *p_utlb_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret    = OSAL_RETURN_OK;
    size_t          domain_len;
    size_t          count;
    size_t          local_count = p_utlb_control->match_count;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int             os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    domain_len = R_OSAL_OS_Strnlen(p_domain_name, OSAL_DEV_CFG_BUS_DOMAIN_SIZE);
    count      = 0;

    while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb)
    {
        os_ret = strncmp(gsp_axi_bus_name_info[count].domain_name, p_domain_name, domain_len);
        if (0 == os_ret)
        {
            if (gsp_axi_bus_name_info[count].utlb == utlb)
            {
                if ((NULL != p_list_buffer) && (list_size > local_count))
                {
                    p_list_buffer[local_count] = count; /* count is axi_id */
                }
                else
                {
                    /* Do nothing */
                }

                local_count++;
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
        count++;
    }   /* end of while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[count].utlb) */

    if (OSAL_DEV_CFG_AXI_BUS_NAME_LAST == gsp_axi_bus_name_info[count].utlb)
    {
        osal_ret = OSAL_RETURN_UNKNOWN_NAME; /* domain name not found or utlb number is not assigned to the domain */
    }
    else
    {
        /* Do nothing */
    }

    p_utlb_control->match_count  = local_count;

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_add_utlb_list_single()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_039]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_039]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_082
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_add_utlb_list_multi()
***********************************************************************************************************************/
/* This function is implemented as a part of r_osal_dcfg_io_add_utlb_list() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC e_osal_return_t r_osal_dcfg_io_add_utlb_list_multi(int32_t utlb,
                                                               const char* p_domain_name,
                                                               size_t list_size,
                                                               osal_axi_bus_id_t* p_list_buffer,
                                                               st_osal_utlb_list_control_t *p_utlb_control)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    e_osal_return_t osal_ret          = OSAL_RETURN_OK;
    int32_t         current_val       = p_utlb_control->current_utlb;
    int32_t         loop_count  = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while ((loop_count < (utlb - current_val)) && (OSAL_RETURN_OK == osal_ret))
    {
        osal_ret = r_osal_dcfg_io_add_utlb_list_single((current_val + loop_count + 1),
                                                       p_domain_name,
                                                       list_size,
                                                       p_list_buffer,
                                                       p_utlb_control);
        loop_count++;
    }

    return osal_ret;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_add_utlb_list_multi()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_038]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_038]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_083
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_create_axiid_list()
***********************************************************************************************************************/
/* This function is implemented as a part of R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl() in order to improve the result of 
   static code analysis (STMIF of HIS code metrics). */
OSAL_STATIC void r_osal_dcfg_create_axiid_list(const char* p_device_name,
                                               osal_axi_bus_id_t *p_array,
                                               size_t array_size,
                                               size_t * const p_num_of_axi_ids,
                                               size_t * const p_num_of_invalid_ids)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    char const  *p_string;
    size_t      numofaxiids     = 0;
    size_t      invalid_axiids  = 0;
    size_t      loop_count      = 0;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[loop_count].utlb)
    {
        p_string = r_osal_dcfg_io_search_strings(gsp_axi_bus_name_info[loop_count].dev_names, p_device_name);
        if (NULL != p_string)
        {
            if (OSAL_DEV_CFG_INVALID_UTLB != gsp_axi_bus_name_info[loop_count].utlb)
            {
                if ((NULL != p_array) && (array_size > numofaxiids))
                {
                    p_array[numofaxiids] = loop_count;
                }
                else
                {
                    /* Do nothing */
                }
                numofaxiids++;
            }
            else
            {
                invalid_axiids++;
            }
        }
        else
        {
            /* Do nothing */
        }   /* end of if (NULL != p_string). */
        loop_count++;
    }   /* end of while (OSAL_DEV_CFG_AXI_BUS_NAME_LAST != gsp_axi_bus_name_info[loop_count].utlb). */

    *p_num_of_axi_ids     = numofaxiids;
    *p_num_of_invalid_ids = invalid_axiids;

    return;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_create_axiid_list()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_004]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_004]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_084
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_search_strings()
***********************************************************************************************************************/
OSAL_STATIC char * r_osal_dcfg_io_search_strings(const char * p_src_string, 
                                                 const char * p_search_string)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    char *p_string;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_string = strstr(p_src_string, p_search_string);
    if (NULL != p_string)
    {
        /* Found at beginning of string */
        if (p_string == p_src_string)
        {
            /* Do nothing */
        }
        else
        {
            /* Check separator(LF) */
            if ( '\n' == p_string[-1])
            {
                /* Do nothing */
            }
            else
            {
                p_string = NULL;
            }
        }
    }
    else
    {
        /* Do nothing */
    }   /* end of if (NULL != p_string) */

    return p_string;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_search_strings()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_040]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_040]
***********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_QNX_CD_CD_IO_064
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function r_osal_dcfg_io_search()
***********************************************************************************************************************/
/* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
OSAL_STATIC int r_osal_dcfg_io_search(const void *p_key, 
                                      const void *p_search_info)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /* PRQA S 0316 1 # There is no problem because it is restored to the type before calling this function. */
    const char                     *p_device_id = (const char *)p_key;
    /* PRQA S 0316 1 # There is no problem because it is restored to the type before calling this function. */
    const st_osal_device_control_t *p_control   = (const st_osal_device_control_t*)p_search_info;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                            result;
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                            os_ret;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    os_ret = strncmp(p_device_id, p_control->dev_info->id, (size_t)OSAL_DEVICE_NAME_MAX_LENGTH);
    if (0 > os_ret)
    {
        result = -1;
    }
    else if (0 < os_ret)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}
/***********************************************************************************************************************
* End of function r_osal_dcfg_io_search()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_QNX_UD_DD_IO_011]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_011]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
