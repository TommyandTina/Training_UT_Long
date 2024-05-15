/*
 * $QNXLicenseC:
 * Copyright 2020, QNX Software Systems.
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


#ifndef _HW_SYSC_H_INCLUDED
#define _HW_SYSC_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <devctl.h>

typedef struct _sysc_ctl {
    int pdr_number;   // Power domain number (from 0 to 63)
    int is_off;       // 1 : Power domain is OFF; 0 : Power domain is ON; other : unknown state
} sysc_devctl_t;

#define MY_SYSC_CMD_CODE      2
#define RCAR_SYSC_DEVCTL_IS_POWER_OFF     __DIOTF(_DCMD_MISC, MY_SYSC_CMD_CODE + 1, sysc_devctl_t)
#define RCAR_SYSC_DEVCTL_POWER_ON         __DIOT(_DCMD_MISC,  MY_SYSC_CMD_CODE + 2, sysc_devctl_t)
#define RCAR_SYSC_DEVCTL_POWER_OFF        __DIOT(_DCMD_MISC,  MY_SYSC_CMD_CODE + 3, sysc_devctl_t)
#define RCAR_SYSC_DEVCTL_READ_PDMODE      __DIOTF(_DCMD_MISC, MY_SYSC_CMD_CODE + 4, sysc_devctl_t)

#define SYSC_ATTACH_NAME         "sysc"
#define SYSC_DEV_NAME            "/dev/" SYSC_ATTACH_NAME


/* 
 * GEN3 SYSC Power Domain Register Numbers
 */
#define SYSC_PDR_GEN3_CA57_SCU          0
#define SYSC_PDR_GEN3_CA53_SCU          1
#define SYSC_PDR_GEN3_CR7               2
#define SYSC_PDR_GEN3_A3IR              3
#define SYSC_PDR_GEN3_A2IR0             4
#define SYSC_PDR_GEN3_A2IR1             5
#define SYSC_PDR_GEN3_A2IR2             6
#define SYSC_PDR_GEN3_A2IR3             7
#define SYSC_PDR_GEN3_A2IR4             8
#define SYSC_PDR_GEN3_A2IR5             10
#define SYSC_PDR_GEN3_A2SC0             11
#define SYSC_PDR_GEN3_A2SC1             12
#define SYSC_PDR_GEN3_A2SC2             13
#define SYSC_PDR_GEN3_A2SC3             14
#define SYSC_PDR_GEN3_A2SC4             15
#define SYSC_PDR_GEN3_A2DP0             16
#define SYSC_PDR_GEN3_A2DP1             17
#define SYSC_PDR_GEN3_A2DP              18
#define SYSC_PDR_GEN3_A2CN0             19
#define SYSC_PDR_GEN3_A3VIP0            20
#define SYSC_PDR_GEN3_A3VIP1            21
#define SYSC_PDR_GEN3_A3VIP2            22
#define SYSC_PDR_GEN3_A3VP              23
#define SYSC_PDR_GEN3_A3VC              24
#define SYSC_PDR_GEN3_A2VC0             25
#define SYSC_PDR_GEN3_A2VC1             26
#define SYSC_PDR_GEN3_3DGA              27
#define SYSC_PDR_GEN3_3DGB              28
#define SYSC_PDR_GEN3_3DGC              29
#define SYSC_PDR_GEN3_3DGD              30
#define SYSC_PDR_GEN3_3DGE              31
#define POWER_DOMAIN_UNKNOWN            32

#define SYSC_PDR_V3U_A1E0D0C0           0
#define SYSC_PDR_V3U_A1E0D0C1           1
#define SYSC_PDR_V3U_A1E0D1C0           2
#define SYSC_PDR_V3U_A1E0D1C1           3
#define SYSC_PDR_V3U_A1E1D0C0           4
#define SYSC_PDR_V3U_A1E1D0C1           5
#define SYSC_PDR_V3U_A1E1D1C0           6
#define SYSC_PDR_V3U_A1E1D1C1           7
#define SYSC_PDR_V3U_A2E0D0             16
#define SYSC_PDR_V3U_A2E0D1             17
#define SYSC_PDR_V3U_A2E1D0             18
#define SYSC_PDR_V3U_A2E1D1             19
#define SYSC_PDR_V3U_A3E0               20
#define SYSC_PDR_V3U_A3E1               21
#define SYSC_PDR_V3U_3DGA               24
#define SYSC_PDR_V3U_3DGB               25
#define SYSC_PDR_V3U_A1CNN2             32
#define SYSC_PDR_V3U_A1DSP0             33
#define SYSC_PDR_V3U_A2IMP0             34
#define SYSC_PDR_V3U_A2DP0              35
#define SYSC_PDR_V3U_A2CV0              36
#define SYSC_PDR_V3U_A2CV1              37
#define SYSC_PDR_V3U_A2CV4              38
#define SYSC_PDR_V3U_A2CV6              39
#define SYSC_PDR_V3U_A2CN2              40
#define SYSC_PDR_V3U_A1CNN0             41
#define SYSC_PDR_V3U_A2CN0              42
#define SYSC_PDR_V3U_A3IR               43
#define SYSC_PDR_V3U_A1CNN1             44
#define SYSC_PDR_V3U_A1DSP1             45
#define SYSC_PDR_V3U_A2IMP2             46
#define SYSC_PDR_V3U_A2DP1              47
#define SYSC_PDR_V3U_A2CV2              48
#define SYSC_PDR_V3U_A2CV3              49
#define SYSC_PDR_V3U_A2CV5              50
#define SYSC_PDR_V3U_A2CV7              51
#define SYSC_PDR_V3U_A2CN1              52
#define SYSC_PDR_V3U_A3VIP0             56
#define SYSC_PDR_V3U_A3VIP1             57
#define SYSC_PDR_V3U_A3VIP2             58
#define SYSC_PDR_V3U_A3VIP3             59
#define SYSC_PDR_V3U_A3ISP01            60
#define SYSC_PDR_V3U_A3ISP23            61

/*
 * Return codes
 */
#define SYSC_RET_OK                   (0)  // OK
#define SYSC_RET_INVALPARA            (1)  // Invalid input parameter 
#define SYSC_RET_TIMEOUT              (2)  // Hardware flag not set 
#define SYSC_RET_HWFAIL               (3)  // Hardware failure
#define SYSC_RET_UNKNOWNSTA           (4)  // Unknown hardware state
#define SYSC_RET_ERROR                (-1) // Other errors

/*
 * Hardware Access Function Table
 */
typedef struct {

    /* size of this structure */
    size_t size;

    /*
     * open a instant of sysc resourse manager (start #sysc_resmgr-rcar)
     *
     * Input Parameters:
     *  None
     *
     * Return Value :
     *     EOK - Success
     *     Another - Failure if sysc_resmgr-rcar is not run
     */
    int (*init) (void);

    /*
     * DeInitialize memory map for SYSC regsiter
     * management.
     *
     * Input Parameters:
     *     None
     *
     * Return Value :
     *     EOK - Success
     *     Another - Failure
     */
    void (*deinit) (void);

    /*
     * Function power_is_off used to check the current power state of a module
     *
     * Input Parameters:
     *     int pdr_number: Power Domain number
     *
     * Return Value :
     *     1 - Power is OFF
     *     0 - Power is ON
     *  < 0 - Failure
     */
    int (*power_is_off) (int pdr_number);

    /*
     * The power_off function is used to power
     * a domain off
     *
     * Input Parameters:
     *     int pdr_number: Power Domain number
     *
     * Return Value :
     *     EOK - Success
     *     < 0 - Failure
     *
     */
    int (*power_off) (int pdr_number);

    /*
     * The power_on function is used to power
     * a domain on
     *
     * Input Parameters:
     *     int pdr_number: Power Domain number
     *
     * Return Value :
     *     EOK - Success
     *     < 0 - Failure
     *
     */
    int (*power_on) (int pdr_number);

    /*
     * The read_pdmode function is used to read PDMODE value
     *
     *
     * Return Value :
     *     >= 0 - Success
     *     <  0 - Failure
     *
     */
    int (*read_pdmode) (void);

} sysc_mgr_funcs_t;


/*
 * Fills in the given table with the hardware-specific functions.
 * If a function table entry is unimplemented, it should
 * be left unchanged (and not set to NULL, etc.).
 * "funcs->size" should be set by whomever has allocated the
 * table. Don't change this.
 * Input Parameters:
 *  sysc_mgr_funcs_t *funcs      Function table
 *    int tabsize                 size of "funcs" in bytes
 *
 * Returns:
 * EOK        success
 * Other    failure
 */

int sysc_mgr_getfuncs(sysc_mgr_funcs_t *funcs, int tabsize);

#define SYSC_MGR_ADD_FUNC(tabletype, table, entry, func, limit) \
    do { \
        if (offsetof(tabletype, entry) + sizeof((table)->entry) <= (size_t)(limit)) \
            (table)->entry = (func); \
    } while (0)

#endif /* _HW_SYSC_H_INCLUDED */
