/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    osal_test_dev_conf.c
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  main of check about OSAL Device Configuration
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           0.1.0     10.02.2023    Create New
 *           0.2.0     10.11.2023    Add definition for V4M
***********************************************************************************************************************/

/* include */
#include "osal_test_dev_conf.h"

#if defined(RCAR_V4H) || defined(RCAR_V4M) || defined(RCAR_S4)

/* include V4H, V4M, S4 */
#if defined(RCAR_V4H)
#include "expect_value_v4h.h"
#elif defined(RCAR_V4M)
#include "expect_value_v4m.h"
#elif defined(RCAR_S4)
#include "expect_value_s4.h"
#endif

/* Check Device Configuration value for IO access */
int check_device_basic_info(osal_device_handle_t handle, struct expect_config io_access_config)
{
    int app_ret = 0;

    /* Check Device Name */
    if(0!=strcmp(handle->dev_info->id, io_access_config.id))
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: id is NG, impl=%s expect=%s\n", io_access_config.id, handle->dev_info->id, io_access_config.id);
    }

    /* Check Device Type */
    if(0!=strcmp(handle->dev_info->type, io_access_config.type))
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: type is NG, impl=%s expect=%s\n", io_access_config.id, handle->dev_info->type, io_access_config.type);
    }

    /* Check Device Address */
    if(handle->dev_info->address != io_access_config.address)
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: address is NG, impl=0x%x expect=0x%x\n", io_access_config.id, handle->dev_info->address, io_access_config.address);
    }

    /* Check Device Address Range */
    if(handle->dev_info->range != io_access_config.range)
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: range is NG, impl=0x%x expect=0x%x\n", io_access_config.id, handle->dev_info->range, io_access_config.range);
    }

    return app_ret;
}

/* check Device Configuration value for Interrupt */
int check_device_int_info(osal_device_handle_t handle, struct expect_config test)
{
    int app_ret = 0;
    size_t int_num = 0;

    /* Check Number of Interrupt Channels */
    if((handle->dev_info->irq_channels != test.number_of_interrupts) && handle->dev_info->irq_channels != test.number_of_int)
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: number_of_interrupts is NG\n", test.id);
    }
    else
    {
        if(0 != test.number_of_interrupts)
        {
            /* Check SPI Number */
            for(int_num = 0; int_num < test.number_of_interrupts; int_num++)
            {
                if(handle->dev_info->interrupt[int_num].irq_number != test.st_int_info[int_num].interrupt_spi_number)
                {
                    app_ret -= 1;
                    OSAL_CHECK_ERR("%s: difference spi number:impl=%d expect=%d\n", 
                            test.id,
                            handle->dev_info->interrupt[int_num].irq_number,
                            test.st_int_info[int_num].interrupt_spi_number);
                }
            }
        }
    }

    return app_ret;
}

/* Device Configuration value for AXI bus control */
int check_device_axi_info(osal_device_handle_t handle, struct expect_config test)
{
    int app_ret = 0;
    size_t num_of_axi_ids = 0;
    size_t num = 0;

    /* Get Number of UTLB from Wrapper Implementation */
    if(R_OSAL_IoGetNumOfAxiIdsForDeviceHdl(handle, &num_of_axi_ids))
    {
        OSAL_CHECK_ERR("%s: Device Handle NG\n", test.id);
        return -1;
    }

    /* Check AXI Bus Name */
    if(0!=strcmp(handle->dev_info->axi_bus_name, test.axi_bus_name))
    {
        app_ret -= 1;
        OSAL_CHECK_ERR("%s: axi_bus_name is NG, impl=%s expect=%s\n", test.id, handle->dev_info->axi_bus_name, test.axi_bus_name);
    }
 
    /* Check UTLB Setting */
    if(num_of_axi_ids != test.num_of_utlb)
    {
        if(0!=strcmp("invalid", test.axi_bus_name))
        {
            app_ret -= 1;
            OSAL_CHECK_ERR("%s: num of axi bus is NG, impl=%d expect=%d\n", test.id, num_of_axi_ids, test.num_of_utlb);
        }
    }
    else
    {
        for(num = 0; num < num_of_axi_ids; num++)
        {
            if(handle->dev_info->utlb_assignment->table[num] != test.st_axi_info[num].utlbs)
            {
                app_ret -= 1;
                OSAL_CHECK_ERR("%s: UTLB setting is NG, impl=%d expect=%d\n", test.id, handle->dev_info->utlb_assignment->table[num], test.st_axi_info[num].utlbs);
            }
        }
    }

    return app_ret;
}

/* Check Device Configuration value for PMA Setting */
int check_device_pm_info(osal_device_handle_t handle, struct expect_config test)
{
    int app_ret = 0;
    size_t num_of_axi_ids = 0;
    size_t num = 0;

    /* Check PMA ID */
    if(handle->dev_info->pma_id != test.st_pm_info[0].pma_id)
    {
        OSAL_CHECK_ERR("%s: PMA ID is NG\n", test.id);
        app_ret -= 1;
    }

    /* Check POST Clock Setting */
    if(handle->dev_info->postckcr != test.st_pm_info[0].postckcr)
    {
        OSAL_CHECK_ERR("%s: POST Clock is NG\n", test.id);
        app_ret -= 1;
    }

    return app_ret;
}

/* Check Device Configuration value for Number of Device information */
int check_device_num(st_osal_device_info_t *gs_device_info, struct expect_config *test, size_t expect_dev_num, size_t wrapper_dev_num)
{
    int app_ret = 0;
    size_t expect_num = 0;
    size_t num = 0;
    bool exit_flag = false;

    /* Compare to UM and Wrapper Implementation */
    if(expect_dev_num != wrapper_dev_num)
    {
        if(expect_dev_num < wrapper_dev_num)
        {
            for(num = 0; num < wrapper_dev_num -1; num++)
            {
                /* Get about lack of Device information in UM */
                for(expect_num = 0; expect_num < expect_dev_num -1; expect_num++)
                {
                    if(0!=strcmp(gs_device_info[num].id, test[expect_num].id))
                    {
                        exit_flag = false;
                    }
                    else if(0==strcmp(gs_device_info[num].id, test[expect_num].id))
                    {
                        exit_flag = true;
                        break;
                    }
                }

                /* Output about lack of Device in QNX UM */
                if(false == exit_flag)
                {
                    app_ret -= 1;
                    OSAL_CHECK_ERR("%s is not in QNX UM. Please check of Description in UM about IO access, Interrupt, PM, AXI\n", gs_device_info[num].id);
                }
            }
        }
        else if(expect_dev_num > wrapper_dev_num)
        {
            for(num = 0; num < expect_dev_num -1; num++)
            {
                for(expect_num = 0; expect_num < wrapper_dev_num-1; expect_num++)
                {
                    /* Get about lack of Device information in Wrapper Implementation */
                    if(0!=strcmp(gs_device_info[num].id, test[expect_num].id))
                    {
                        exit_flag = false;
                    }
                    else if(0==strcmp(gs_device_info[num].id, test[expect_num].id))
                    {
                        exit_flag = true;
                        break;
                    }
                }
                if(false == exit_flag)
                {
                    /* Output about lack of Device in Implementation */
                    OSAL_CHECK_ERR("%s is not in implementation\n", test[num].id);
                }
            }
        }
    }
    else
    {
        OSAL_CHECK_INFO("Test Data number is same to implementation\n");
    }
    return app_ret;
}

/* checker main */
int simple_dev_conf_test(void)
{
    int app_ret = 0;
    size_t num = 0;
    size_t expect_dev_num = 0;
    size_t wrapper_dev_num = 0;
    e_osal_return_t osal_ret;
    osal_device_handle_t handle;

    /* Start OSAL */
    OSAL_CHECK_INFO("start simple_dev_conf_test\n");
    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_CHECK_ERR("R_OSAL_Initialize() error = %d\n", osal_ret);
        return -1;
    }

    /**
     * 1. Device Search
     * 2. Device Open
     * 3. check IO access(device name, device type, address, range)
     * 4. check Interrupt(num of interrupts, SPI Numbers)
     * 5. check AXI(num of interrupts, SPI Numbers)
     * 6. check PMA
     */
    expect_dev_num = sizeof(test)/sizeof(expect_config_t);
#if defined(RCAR_V4H)
    wrapper_dev_num = sizeof(gs_osal_device_info_v4h)/sizeof(st_osal_device_info_t);
    app_ret += check_device_num(gs_osal_device_info_v4h, test, expect_dev_num, wrapper_dev_num);
#elif defined(RCAR_V4M)
    wrapper_dev_num = sizeof(gs_osal_device_info_v4m)/sizeof(st_osal_device_info_t);
    app_ret += check_device_num(gs_osal_device_info_v4m, test, expect_dev_num, wrapper_dev_num);
#elif defined(RCAR_S4)
    wrapper_dev_num = sizeof(gs_osal_device_info_s4)/sizeof(st_osal_device_info_t);
    app_ret += check_device_num(gs_osal_device_info_s4, test, expect_dev_num, wrapper_dev_num);
#endif

    for(num = 0; num < expect_dev_num - 1; num++)
    {
        osal_ret = R_OSAL_IoDeviceOpen(test[num].id, &handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            OSAL_CHECK_ERR("%s: DeviceOpen error = %d\n", test[num].id, osal_ret);
            app_ret = -1;
        }

        if(OSAL_RETURN_OK == osal_ret)
        {
            app_ret += check_device_basic_info(handle, test[num]);
            app_ret += check_device_int_info(handle, test[num]);
            app_ret += check_device_axi_info(handle, test[num]);
            app_ret += check_device_pm_info(handle, test[num]);

            osal_ret = R_OSAL_IoDeviceClose(handle);
            if (OSAL_RETURN_OK != osal_ret)
            {
                OSAL_CHECK_ERR("DeviceClose error = %d\n", osal_ret);
                return -1;
            }
        }
    }

    /* End OSAL */
    osal_ret = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != osal_ret)
    {
        OSAL_CHECK_ERR("R_OSAL_Deinitialize() error = %d\n", osal_ret);
        return -1;
    }

    if(0 == app_ret)
    {
        OSAL_CHECK_INFO("UM and Device Config is same!!.\n");
    }
    else
    {
        OSAL_CHECK_ERR("UM and Device Config is not same!!. Please check UM and implementation.\n");
    }

    return app_ret;
}
#endif
