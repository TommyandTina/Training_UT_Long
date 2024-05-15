/*!
 *  @file       drv.h
 *  @brief      V3M driver
 *  @date       2016/Dec/10 latest update
 *  @author     Kaneko
 *  @version    0.01
 *  @par        history
 *  @note
 *  Copyright (C) 2012-2018, Renesas Electronics Corporation, all rights reserved.
 */
#ifndef DRV_H
#define DRV_H

#include <stdbool.h>
#include <stdint.h>

/* Common type definition for drivers */
#ifdef __FOR_UNIT_TEST__
#else
/* Changed for the error at std=c++03	 */
typedef bool BOOL;
//typedef _Bool               BOOL;
#endif
typedef int8_t   INT8;
typedef int16_t  INT16;
typedef int32_t  INT32;
typedef int64_t  INT64;
typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;

/* Boolean value */
#define drv_FALSE ((BOOL) false)
#define drv_TRUE ((BOOL) true)

/* Return code of API */
#define drv_OK (0U)             /* API completed without any error. */
#define drv_FAIL (1U)           /* Failed. */
#define drv_INVALID_SPI (2U)    /* Specified SPI number is not available. */
#define drv_NULL_FUNC (3U)      /* Callback function is not specified. */
#define drv_INVALID_TARGET (4U) /* Invalid target CPU  for the interrupt was specified. */
#define drv_INVALID_PRIO (5U)   /* Invalid priority  for the interrupt is specified. */
#define drv_IS_ENABLED (6U)     /* Specified SPI number was already enabled. */
#define drv_NOT_SETUP_SPI (7U)  /* Specified SPI number is not registered. */
#define drv_COMSTART (8U)       /* Communication has been started. */
#define drv_COMNOTSTART (9U)    /* Communication has not been started. */

#define drv_ERR_PARAM (101U)           /* Parameter error */
#define drv_ERR_NOT_INITIALIZED (102U) /* API called before the driver initialized. */
#define drv_ERR_NOT_OPENED (103U)      /* API called before the driver opened. */
#define drv_ERR_OPENED (104U)
#define drv_ERR_ALREADY_OPENED (105U)                /* The driver re-opened while it is already opened */
#define drv_ERR_ALREADY_RUNNING (106U)               /* The driver re-run while it is already running */
#define drv_ERR_DETECTED_UNEXPECTED_INTERRUPT (107U) /* Unexpected interrupt has been detected */
#define drv_ERR_AL_DETECTED (108U)                   /* Arbitration lost in I2C has occurred */
#define drv_ERR_NACK_RECEIVED (109U)                 /* NACK in I2C has occurred */
#define drv_ERR_UNUSABLE_CH (110U)                   /* Invalid channel was specified */
#define drv_ERR_NOT_RUNNING_YET (111U)               /* API was called before the driver had been running. */
#define drv_ERR_CLOCK_START (112U)                   /* Clock supply could not be started. */
#define drv_ERR_CLOCK_STOP (113U)                    /* Clock supply could not be stopped. */
#define drv_ERR_ALREADY_INITIALIZED (114U)           /* Initialization API called while the driver is already initialized */
#define drv_ERR_NOT_RUNNING (115U)                   /* Not running before */
#define drv_ERR_CANNOT_ENABLE_INTERRUPT (116U)
#define drv_ERR_OS_SEMAPHORE (117U)    /* Detected semaphore error from OS */
#define drv_ERR_BUF_OVER_FLOW (118U)   /* Buffer overflow */
#define drv_ERR_INTERNAL_ISRDRV (119U) /* Internal error of ISR-Driver */
#define drv_ERR_NOT_CLOSED (120U)      /* API called before the driver closed. */
#define drv_ERR_NOT_CONF_SET (121U)    /* Not config set before */
#define drv_ERR_INTERNAL (199U)

#define drv_ERR_BUS_BUSY (201U)     /* Bus busy in I2C has occurred             */
#define drv_ERR_DMA_TRANSFER (202U) /* DMA transfer error in I2C has occurred   */

#define drv_SDMAC_CH_INVALID (301U)      /* Invalid channel was specified. */
#define drv_SDMAC_CH_BUSY (302U)         /* Specified channel is used. */
#define drv_SDMAC_CH_NO_FREE (303U)      /* No allocation channel. */
#define drv_SDMAC_DESC_NO_FREE (304U)    /* Descriptor memory could not be allocated. */
#define drv_SDMAC_ADDRESS_INVALID (305U) /* Specified address could not be found from DDRM table. */
#define drv_SDMAC_NOT_STOPPED (306U)     /* Not stoprd SDMAC */

#define drv_VSPD_ERR_HW_VERSION (401U)       /* Invalid VSPD Hardware IP version */
#define drv_VSPD_ERR_HW_STATUS (402U)        /* VSPD Hardware is "using DisplayList" or "already HW stopped" */
#define drv_VSPD_ERR_NOT_READY (403U)        /* VSPD driver does not have DisplayList-data */
#define drv_VSPD_ERR_NOT_SET_CALLBACK (404U) /* Callback function for DISCOM not set. */

#define DRV_DU_ERR_PARAM (501U)               /* Illegal parameters were specified. */
#define DRV_DU_ERR_ALREADY_INITIALIZED (503U) /* DU driver is already initialized.                                */
#define DRV_DU_ERR_NOT_INITIALIZED (504U)     /* DU driver is NOT initialized.                                    */
#define DRV_DU_ERR_ALREADY_OPEN (505U)        /* DU driver is already opened.                                     */
#define DRV_DU_ERR_NOT_OPEN (506U)            /* DU driver is NOT opened.                                         */
#define DRV_DU_ERR_ALREADY_STARTED (507U)     /* DU driver is already started.                                    */
#define DRV_DU_ERR_NOT_STARTED (508U)         /* DU driver is NOT started.                                        */
#define DRV_DU_ERR_ALREADY_DISPLAY_OFF (509U) /* DU driver is NOT stopped.                                        */
#define DRV_DU_ERR_ALREADY_STOPPED (510U)     /* DU driver is already stopped.                                    */
#define DRV_DU_ERR_NOT_STOPPED (511U)         /* DU driver is NOT stopped.                                        */
#define DRV_DU_ERR_ALREADY_CLOSED (512U)      /* DU driver is already closed.                                     */
#define DRV_DU_ERR_NOT_CLOSED (513U)          /* DU driver is NOT closed.                                         */

#define drv_ERR_UNUSABLE_PORT (711U)      /* Unuseable channel */
#define drv_ERR_NOT_CONFIGURED (712U)     /* Internal state is not configured */
#define drv_ERR_NOT_OUTPUT_MODE (713U)    /* Not output mode */
#define drv_ERR_NOT_INTERRUPT_MODE (714U) /* Not interrupt mode */
#define drv_ERR_OUTPUT_MODE (715U)        /* Output mode */

#define drv_ERR_NOT_PSEUDO_SUPPORTED (800U)     /* Pseudo error funtionality is not support                          */
#define drv_ERR_ALREADY_PSEUDO_SUPPORTED (801U) /*Pseudo error functionality is already supported                   */

#define drv_ERR_TRANSIENT_FAULT (901U)     /* transient fault detected */
#define drv_ERR_PERMANENT_FAULT (902U)     /* permanent fault detected */
#define drv_ERR_BUS_INTERFACE_FAULT (903U) /* fault detected at bus interface check */

#define drv_CPURTT_ERR_FAIL (1001U)    /* Fail CPU RuntimeTest */
#define drv_CPURTT_ERR_TIMEOUT (1002U) /* Timeout CPU RuntimeTest */
#define drv_CPURTT_ERR_BUS (1003U)     /* Bus Error CPU RuntimeTest */

#define drv_CPURTT_ERR_PARALLEL (1004U)   /* Parallel error detected by secure monitor. */
#define drv_CPURTT_ERR_RFSODRV (1005U)    /* RFSO driver is error. */
#define drv_CPURTT_ERROR_INTERNAL (1006U) /* fatal error occurs in Secure Monitor internal functions. */

#define drv_RFSO_ERR_CH_INVALID (1101U)       /* RFSO channel is invalid. */
#define drv_RFSO_ERR_NOT_SET_CALLBACK (1102U) /* The callback function is not set. */
#define drv_RFSO_ERR_TIMER_PERIOD (1103U)     /* Timer period is invalid. */
#define drv_RFSO_ERR_FAULT_TYPE (1104U)       /* An unexpected error occurred internally. */

#define drv_CSI2_ERR_NOT_WAIT_SET (1201U) /* Not set user wait function of CSI2 driver. */

/* Disable debug log in driver for test */
//#define DBG_VALID_FLG
#ifndef DBG_VALID_FLG /* For deleting dbg	 */
#    define DBG_LOG(comp_id, level, ...)
#endif

/*!
 *  @brief          Write 64-bit data to the register
 *  @details
 *  @param[in]      addr register address
 *  @param[in]      data 64-bit data to be written to the register
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline void drvCommon_WriteReg64(uintptr_t aAddr, UINT64 aData)
{
    *(volatile UINT64*)aAddr = aData;
}

/*!
 *  @brief          Write 32-bit data to the register
 *  @details
 *  @param[in]      addr register address
 *  @param[in]      data 32-bit data to be written to the register
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline void drvCommon_WriteReg32(uintptr_t aAddr, UINT32 aData)
{
    *(volatile UINT32*)aAddr = aData;
}

/*!
 *  @brief          Write 16-bit data to the register
 *  @details
 *  @param[in]      addr register address
 *  @param[in]      data 16-bit data to be written to the register
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline void drvCommon_WriteReg16(uintptr_t aAddr, UINT16 aData)
{
    *(volatile UINT16*)aAddr = aData;
}

/*!
 *  @brief          Write 8-bit data to the register
 *  @details
 *  @param[in]      addr register address
 *  @param[in]      data 8-bit data to be written to the register
 *  @return         None
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline void drvCommon_WriteReg8(uintptr_t aAddr, UINT8 aData)
{
    *(volatile UINT8*)aAddr = aData;
}

/*!
 *  @brief          Read 64-bit data from the register
 *  @details
 *  @param[in]      addr register address
 *  @return         64-bit data to be read from the register
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline UINT64 drvCommon_ReadReg64(uintptr_t aAddr)
{
    UINT64 aData;
    aData = *(volatile UINT64*)aAddr;
    return aData;
}

/*!
 *  @brief          Read 32-bit data from the register
 *  @details
 *  @param[in]      addr register address
 *  @return         32-bit data to be read from the register
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline UINT32 drvCommon_ReadReg32(uintptr_t aAddr)
{
    UINT32 aData;
    aData = *(volatile UINT32*)aAddr;
    return aData;
}

/*!
 *  @brief          Read 16-bit data from the register
 *  @details
 *  @param[in]      addr register address
 *  @return         16-bit data to be read from the register
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline UINT16 drvCommon_ReadReg16(uintptr_t aAddr)
{
    UINT16 aData;
    aData = *(volatile UINT16*)aAddr;
    return aData;
}

/*!
 *  @brief          Read 8-bit data from the register
 *  @details
 *  @param[in]      addr register address
 *  @return         8-bit data to be read from the register
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
static inline UINT8 drvCommon_ReadReg8(uintptr_t aAddr)
{
    UINT8 aData;
    aData = *(volatile UINT8*)aAddr;
    return aData;
}

#endif /* DRV_H */
